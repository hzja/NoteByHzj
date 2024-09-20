Java SpringBoot SpringBatch Quartz
<a name="whOyL"></a>
### 1、pom文件
```xml
<dependencies>
 <dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-web</artifactId>
 </dependency>
 <dependency>
   <groupId>org.postgresql</groupId>
   <artifactId>postgresql</artifactId>
 </dependency>
 <dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-jdbc</artifactId>
 </dependency>
 <dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-batch</artifactId>
 </dependency>
 <dependency>
   <groupId>org.projectlombok</groupId>
   <artifactId>lombok</artifactId>
 </dependency>
 <dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-batch</artifactId>
 </dependency>
</dependencies>
```
<a name="OlnDt"></a>
### 2、application.yaml文件
```yaml
spring:
  datasource:
    username: thinklink
    password: thinklink
    url: jdbc:postgresql://172.16.205.54:5432/thinklink
    driver-class-name: org.postgresql.Driver
  batch:
    job:
      enabled: false
server:
  port: 8073

#upgrade-dispatch-base-url: http://172.16.205.125:8080/api/rpc/dispatch/command/
upgrade-dispatch-base-url: http://172.16.205.211:8080/api/noauth/rpc/dispatch/command/

# 每次批量处理的数据量，默认为5000
batch-size: 5000
```
<a name="L1Uox"></a>
### 3、Service实现类，触发批处理任务的入口，执行一个job
```java
@Service("batchService")
public class BatchServiceImpl implements BatchService {

// 框架自动注入
    @Autowired
    private JobLauncher jobLauncher;
    @Autowired
    private Job updateDeviceJob;
    /**
     * 根据 taskId 创建一个Job
     * @param taskId
     * @throws Exception
     */
    @Override
    public void createBatchJob(String taskId) throws Exception {
        JobParameters jobParameters = new JobParametersBuilder()
                .addString("taskId", taskId)
                .addString("uuid", UUID.randomUUID().toString().replace("-",""))
                .toJobParameters();
        // 传入一个Job任务和任务需要的参数
        jobLauncher.run(updateDeviceJob, jobParameters);
    }
}
```
<a name="E3SaC"></a>
### 4、SpringBatch配置类，此部分最重要
```java
@Configuration
public class BatchConfiguration {

    private static final Logger log = LoggerFactory.getLogger(BatchConfiguration.class);

    @Value("${batch-size:5000}")
    private int batchSize;

// 框架自动注入
    @Autowired
    public JobBuilderFactory jobBuilderFactory;

// 框架自动注入
    @Autowired
    public StepBuilderFactory stepBuilderFactory;

// 数据过滤器，对从数据库读出来的数据，注意进行操作
    @Autowired
    public TaskItemProcessor taskItemProcessor;

    // 接收job参数
    public Map<String, JobParameter> parameters;

    public Object taskId;

    @Autowired
    private JdbcTemplate jdbcTemplate;

// 读取数据库操作
    @Bean
    @StepScope
    public JdbcCursorItemReader<DispatchRequest> itemReader(DataSource dataSource) {

        String querySql = " SELECT " +
                " e. ID AS taskId, " +
                " e.user_id AS userId, " +
                " e.timing_startup AS startTime, " +
                " u.device_id AS deviceId, " +
                " d.app_name AS appName, " +
                " d.compose_file AS composeFile, " +
                " e.failure_retry AS failureRetry, " +
                " e.tetry_times AS retryTimes, " +
                " e.device_managered AS deviceManagered " +
                " FROM " +
                " eiot_upgrade_task e " +
                " LEFT JOIN eiot_upgrade_device u ON e. ID = u.upgrade_task_id " +
                " LEFT JOIN eiot_app_detail d ON e.app_id = d. ID " +
                " WHERE " +
                " ( " +
                " u.device_upgrade_status = 0 " +
                " OR u.device_upgrade_status = 2" +
                " )" +
                " AND e.tetry_times > u.retry_times " +
                " AND e. ID = ?";

        return new JdbcCursorItemReaderBuilder<DispatchRequest>()
                .name("itemReader")
                .sql(querySql)
                .dataSource(dataSource)
                .queryArguments(new Object[]{parameters.get("taskId").getValue()})
                .rowMapper(new DispatchRequest.DispatchRequestRowMapper())
                .build();
    }

// 将结果写回数据库
    @Bean
    @StepScope
    public ItemWriter<ProcessResult> itemWriter() {
        return new ItemWriter<ProcessResult>() {

            private int updateTaskStatus(DispatchRequest dispatchRequest, int status) {
                log.info("update taskId: {}, deviceId: {} to status {}", dispatchRequest.getTaskId(), dispatchRequest.getDeviceId(), status);

                Integer retryTimes = jdbcTemplate.queryForObject(
                        "select retry_times from eiot_upgrade_device where device_id = ? and upgrade_task_id = ?",
                        new Object[]{ dispatchRequest.getDeviceId(), dispatchRequest.getTaskId()}, Integer.class
                );
                retryTimes += 1;
                int updateCount = jdbcTemplate.update("update eiot_upgrade_device set device_upgrade_status = ?, retry_times = ? " +
                        "where device_id = ? and upgrade_task_id = ?", status, retryTimes, dispatchRequest.getDeviceId(), dispatchRequest.getTaskId());
                if (updateCount <= 0) {
                    log.warn("no task updated");
                } else {
                    log.info("count of {} task updated", updateCount);
                }

                // 最后一次重试
                if (status == STATUS_DISPATCH_FAILED && retryTimes == dispatchRequest.getRetryTimes()) {
                    log.info("the last retry of {} failed, inc deviceManagered", dispatchRequest.getTaskId());
                    return 1;
                } else {
                    return 0;
                }
            }

            @Override
            @Transactional
            public void write(List<? extends ProcessResult> list) throws Exception {
                Map taskMap = jdbcTemplate.queryForMap(
                        "select device_managered, device_count, task_status from eiot_upgrade_task where id = ?",
                        list.get(0).getDispatchRequest().getTaskId() // 我们认定一个批量里面，taskId都是一样的
                        );
                int deviceManagered = (int)taskMap.get("device_managered");
                Integer deviceCount = (Integer) taskMap.get("device_count");
                if (deviceCount == null) {
                    log.warn("deviceCount of task {} is null", list.get(0).getDispatchRequest().getTaskId());
                }
                int taskStatus = (int)taskMap.get("task_status");
                for (ProcessResult result: list) {
                    deviceManagered += updateTaskStatus(result.getDispatchRequest(), result.getStatus());
                }
                if (deviceCount != null && deviceManagered == deviceCount) {
                    taskStatus = 2; //任务状态 0:待升级，1:升级中，2:已完成
                }
                jdbcTemplate.update("update eiot_upgrade_task set device_managered = ?, task_status = ? " +
                        "where id = ?", deviceManagered, taskStatus, list.get(0).getDispatchRequest().getTaskId());
            }
        };
    }

    /**
     * 定义一个下发更新的 job
     * @return
     */
    @Bean
    public Job updateDeviceJob(Step updateDeviceStep) {
        return jobBuilderFactory.get(UUID.randomUUID().toString().replace("-", ""))
                .listener(new JobListener()) // 设置Job的监听器
                .flow(updateDeviceStep)// 执行下发更新的Step
                .end()
                .build();
    }

    /**
     * 定义一个下发更新的 step
     * @return
     */
    @Bean
    public Step updateDeviceStep(JdbcCursorItemReader<DispatchRequest> itemReader,ItemWriter<ProcessResult> itemWriter) {
        return stepBuilderFactory.get(UUID.randomUUID().toString().replace("-", ""))
                .<DispatchRequest, ProcessResult> chunk(batchSize)
                .reader(itemReader) //根据taskId从数据库读取更新设备信息
                .processor(taskItemProcessor) // 每条更新信息，执行下发更新接口
                .writer(itemWriter)
                .build();
    }

    // job 监听器
    public class JobListener implements JobExecutionListener {

        @Override
        public void beforeJob(JobExecution jobExecution) {
            log.info(jobExecution.getJobInstance().getJobName() + " before... ");
            parameters = jobExecution.getJobParameters().getParameters();
            taskId = parameters.get("taskId").getValue();
            log.info("job param taskId : " + parameters.get("taskId"));
        }

        @Override
        public void afterJob(JobExecution jobExecution) {

            log.info(jobExecution.getJobInstance().getJobName() + " after... ");
            // 当所有job执行完之后，查询设备更新状态，如果有失败，则要定时重新执行job
            String sql = " SELECT " +
                    " count(*) " +
                    " FROM " +
                    " eiot_upgrade_device d " +
                    " LEFT JOIN eiot_upgrade_task u ON d.upgrade_task_id = u. ID " +
                    " WHERE " +
                    " u. ID = ? " +
                    " AND d.retry_times < u.tetry_times " +
                    " AND ( " +
                    " d.device_upgrade_status = 0 " +
                    " OR d.device_upgrade_status = 2 " +
                    " ) ";

            // 获取更新失败的设备个数
            Integer count = jdbcTemplate.queryForObject(sql, new Object[]{taskId}, Integer.class);

            log.info("update device failure count : " + count);

            // 下面是使用Quartz触发定时任务
            // 获取任务时间,单位秒
// String time = jdbcTemplate.queryForObject(sql, new Object[]{taskId}, Integer.class);
            // 此处方便测试，应该从数据库中取taskId对应的重试间隔，单位秒
            Integer millSecond = 10;

            if(count != null && count > 0){
                String jobName = "UpgradeTask_" + taskId;
                String reTaskId = taskId.toString();
                Map<String,Object> params = new HashMap<>();
                params.put("jobName",jobName);
                params.put("taskId",reTaskId);
                if (QuartzManager.checkNameNotExist(jobName))
                {
                    QuartzManager.scheduleRunOnceJob(jobName, RunOnceJobLogic.class,params,millSecond);
                }
            }

        }
    }
}
```
<a name="h5K20"></a>
### 5、Processor，处理每条数据，可以在此对数据进行过滤操作
```java
@Component("taskItemProcessor")
public class TaskItemProcessor implements ItemProcessor<DispatchRequest, ProcessResult> {

    public static final int STATUS_DISPATCH_FAILED = 2;
    public static final int STATUS_DISPATCH_SUCC = 1;

    private static final Logger log = LoggerFactory.getLogger(TaskItemProcessor.class);

    @Value("${upgrade-dispatch-base-url:http://localhost/api/v2/rpc/dispatch/command/}")
    private String dispatchUrl;

    @Autowired
    JdbcTemplate jdbcTemplate;

    /**
     * 在这里，执行 下发更新指令 的操作
     * @param dispatchRequest
     * @return
     * @throws Exception
     */
    @Override
    public ProcessResult process(final DispatchRequest dispatchRequest) {
        // 调用接口，下发指令
        String url = dispatchUrl + dispatchRequest.getDeviceId()+"/"+dispatchRequest.getUserId();

        log.info("request url:" + url);
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON_UTF8);

        MultiValueMap<String, String> params = new LinkedMultiValueMap<String, String>();

        JSONObject jsonOuter = new JSONObject();
        JSONObject jsonInner = new JSONObject();
        try {
            jsonInner.put("jobId",dispatchRequest.getTaskId());
            jsonInner.put("name",dispatchRequest.getName());
            jsonInner.put("composeFile", Base64Util.bytesToBase64Str(dispatchRequest.getComposeFile()));
            jsonInner.put("policy",new JSONObject().put("startTime",dispatchRequest.getPolicy()));
            jsonInner.put("timestamp",dispatchRequest.getTimestamp());

            jsonOuter.put("method","updateApp");
            jsonOuter.put("params",jsonInner);
        } catch (JSONException e) {
            log.info("JSON convert Exception :" + e);
        }catch (IOException e) {
            log.info("Base64Util bytesToBase64Str :" + e);
        }

        log.info("request body json :" + jsonOuter);
        HttpEntity<String> requestEntity = new HttpEntity<String>(jsonOuter.toString(),headers);
        int status;
        try {
            ResponseEntity<String> response = restTemplate.postForEntity(url,requestEntity,String.class);
            log.info("response :" + response);
            if (response.getStatusCode() == HttpStatus.OK) {
                status = STATUS_DISPATCH_SUCC;
            } else {
                status = STATUS_DISPATCH_FAILED;
            }

        }catch (Exception e){
            status = STATUS_DISPATCH_FAILED;
        }

        return new ProcessResult(dispatchRequest, status);
    }
}
```
<a name="LKYvv"></a>
### 6、封装数据库返回数据的实体Bean，注意静态内部类
```java
public class DispatchRequest {

    private String taskId;
    private String deviceId;
    private String userId;
    private String name;
    private byte[] composeFile;
    private String policy;
    private String timestamp;
    private String md5;
    private int failureRetry;
    private int retryTimes;
    private int deviceManagered;

   // 省略构造函数，setter/getter/tostring方法
   //......
   
    public static class DispatchRequestRowMapper implements RowMapper<DispatchRequest> {
        @Override
        public DispatchRequest mapRow(ResultSet resultSet, int i) throws SQLException {

            DispatchRequest dispatchRequest = new DispatchRequest();
            dispatchRequest.setTaskId(resultSet.getString("taskId"));
            dispatchRequest.setUserId(resultSet.getString("userId"));
            dispatchRequest.setPolicy(resultSet.getString("startTime"));
            dispatchRequest.setDeviceId(resultSet.getString("deviceId"));
            dispatchRequest.setName(resultSet.getString("appName"));
            dispatchRequest.setComposeFile(resultSet.getBytes("composeFile"));
            dispatchRequest.setTimestamp(DateUtil.DateToString(new Date()));
            dispatchRequest.setRetryTimes(resultSet.getInt("retryTimes"));
            dispatchRequest.setFailureRetry(resultSet.getInt("failureRetry"));
            dispatchRequest.setDeviceManagered(resultSet.getInt("deviceManagered"));
            return dispatchRequest;
        }
    }
}
```
<a name="R1vF2"></a>
### 7、启动类上要加上注解
```java
@SpringBootApplication
@EnableBatchProcessing
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
