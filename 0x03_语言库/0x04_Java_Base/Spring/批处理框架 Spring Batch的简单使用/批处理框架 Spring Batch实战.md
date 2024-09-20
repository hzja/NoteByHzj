JavaSpringBatch
<a name="sqXSV"></a>
## 前言
概念词就不多说了，简单地介绍下 ， spring batch 是一个 方便使用的 较健全的 批处理 框架。<br />为什么说是方便使用的，因为这是 基于spring的一个框架，接入简单、易理解、流程分明。<br />为什么说是较健全的， 因为它提供了往常在对大批量数据进行处理时需要考虑到的 日志跟踪、事务粒度调配、可控执行、失败机制、重试机制、数据读写等。
<a name="xUFeL"></a>
## 正文
从实现的业务场景来说，有以下两个：

1. 从  csv文件 读取数据，进行业务处理再存储
2. 从 数据库 读取数据，进行业务处理再存储

也就是平时经常遇到的数据清理或者数据过滤，又或者是数据迁移备份等等。大批量的数据，自己实现分批处理需要考虑的东西太多了，又不放心，那么使用 Spring Batch 框架 是一个很好的选择。<br />首先，在进入实例教程前，看看这次的实例里，使用SpringBoot 整合spring batch 框架，要编码的东西有什么？<br />通过一张简单的图来了解：<br />![2022-11-29-10-55-41.287076400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669694494953-44e91dd2-0195-45e8-bbed-5b7c4b0abedf.png#averageHue=%23f1e2da&clientId=u8a4a0947-c497-4&from=ui&id=u9fb58d7a&originHeight=976&originWidth=1482&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5796140&status=done&style=none&taskId=u9aa74786-48c3-4631-8923-ab82b05e0e1&title=)<br />可能大家看到这个图，是不是多多少少想起来定时任务框架？确实有那么点像，这是一个批处理框架，不是一个schuedling 框架。但是前面提到它提供了可执行控制，也就是说，啥时候执行是可控的，那么显然就是自己可以进行扩展结合定时任务框架，实现心中所想。<br />ok，回到主题，相信大家能从图中简单明了地看到这次实例，需要实现的东西有什么了。所以就不在对各个小组件进行大批量文字的描述了。开始实例教程。<br />首先准备一个数据库，里面建一张简单的表，用于实例数据的写入存储或者说是读取等等。<br />**bloginfo表**<br />![2022-11-29-10-55-41.717275400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669694491043-7de53ae8-e954-41cb-8d8f-08d55d3669eb.png#averageHue=%23f6f6f6&clientId=u8a4a0947-c497-4&from=ui&id=HrbNt&originHeight=215&originWidth=859&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16629&status=done&style=none&taskId=u2f60c660-939d-4e31-b59a-f7b6057017b&title=)<br />相关建表sql语句：
```sql
CREATE TABLE `bloginfo`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `blogAuthor` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '博客作者标识',
  `blogUrl` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '博客链接',
  `blogTitle` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '博客标题',
  `blogItem` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '博客栏目',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 89031 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
```
pom文件里的核心依赖：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-test</artifactId>
  <scope>test</scope>
</dependency>

<!--  spring batch -->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-batch</artifactId>
</dependency>


<!-- hibernate validator -->
<dependency>
  <groupId>org.hibernate</groupId>
  <artifactId>hibernate-validator</artifactId>
  <version>6.0.7.Final</version>
</dependency>
<!--  mybatis -->
<dependency>
  <groupId>org.mybatis.spring.boot</groupId>
  <artifactId>mybatis-spring-boot-starter</artifactId>
  <version>2.0.0</version>
</dependency>
<!--  mysql -->
<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
  <scope>runtime</scope>
</dependency>


<!-- druid数据源驱动 1.1.10解决springboot从1.0——2.0版本问题-->
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>druid-spring-boot-starter</artifactId>
  <version>1.1.18</version>
</dependency>
```
yml文件：
```yaml
spring:
  batch:
    job:
#设置为 false -需要jobLaucher.run执行
      enabled: false
    initialize-schema: always
#    table-prefix: my-batch
 
  datasource:
    druid:
      username: root
      password: root
      url: jdbc:mysql://localhost:3306/hellodemo?useSSL=false&useUnicode=true&characterEncoding=UTF-8&serverTimezone=GMT%2B8&zeroDateTimeBehavior=convertToNull
      driver-class-name: com.mysql.cj.jdbc.Driver
      initialSize: 5
      minIdle: 5
      maxActive: 20
      maxWait: 60000
      timeBetweenEvictionRunsMillis: 60000
      minEvictableIdleTimeMillis: 300000
      validationQuery: SELECT 1 FROM DUAL
      testWhileIdle: true
      testOnBorrow: false
      testOnReturn: false
      poolPreparedStatements: true
      maxPoolPreparedStatementPerConnectionSize: 20
      useGlobalDataSourceStat: true
      connectionProperties: druid.stat.mergeSql=true;druid.stat.slowSqlMillis=5000
server:
  port: 8665
```
![2022-11-29-10-55-42.016647100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701730745-c0df7f43-c7c8-44d9-879d-8e4c157f9254.png#averageHue=%232f3238&clientId=u8a4a0947-c497-4&from=ui&id=H5ppC&originHeight=177&originWidth=762&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65234&status=done&style=none&taskId=u83c1635d-5125-4644-8912-5f1aaad8390&title=)<br />ps：这里用到了druid数据库连接池，其实有个小坑，后面文章会讲到。<br />因为这次的实例最终数据处理完之后，是写入数据库存储（当然也可以输出到文件等等）。<br />所以前面也建了一张表，pom文件里面也整合的mybatis，那么在整合spring batch 主要编码前，先把这些关于数据库打通用到的简单过一下。
<a name="xegDM"></a>
### pojo 层
BlogInfo.java ：
```java
public class BlogInfo {

    private Integer id;
    private String blogAuthor;
    private String blogUrl;
    private String blogTitle;
    private String blogItem;

    @Override
    public String toString() {
        return "BlogInfo{" +
            "id=" + id +
            ", blogAuthor='" + blogAuthor + '\'' +
            ", blogUrl='" + blogUrl + '\'' +
            ", blogTitle='" + blogTitle + '\'' +
            ", blogItem='" + blogItem + '\'' +
            '}';
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getBlogAuthor() {
        return blogAuthor;
    }

    public void setBlogAuthor(String blogAuthor) {
        this.blogAuthor = blogAuthor;
    }

    public String getBlogUrl() {
        return blogUrl;
    }

    public void setBlogUrl(String blogUrl) {
        this.blogUrl = blogUrl;
    }

    public String getBlogTitle() {
        return blogTitle;
    }

    public void setBlogTitle(String blogTitle) {
        this.blogTitle = blogTitle;
    }

    public String getBlogItem() {
        return blogItem;
    }

    public void setBlogItem(String blogItem) {
        this.blogItem = blogItem;
    }
}
```
<a name="smg3L"></a>
### mapper层
BlogMapper.java ：<br />ps：可以看到这个实例用的是注解的方式，为了省事，而且还不写servcie层和impl层，也是为了省事。
```java
import com.example.batchdemo.pojo.BlogInfo;
import org.apache.ibatis.annotations.*;
import java.util.List;
import java.util.Map;

@Mapper
public interface BlogMapper {
    @Insert("INSERT INTO bloginfo ( blogAuthor, blogUrl, blogTitle, blogItem )   VALUES ( #{blogAuthor}, #{blogUrl},#{blogTitle},#{blogItem}) ")
    @Options(useGeneratedKeys = true, keyProperty = "id")
    int insert(BlogInfo bloginfo);


    @Select("select blogAuthor, blogUrl, blogTitle, blogItem from bloginfo where blogAuthor < #{authorId}")
    List<BlogInfo> queryInfoById(Map<String , Integer> map);

}
```
接下来 ，重头戏，开始对前边那张图里涉及到的各个小组件进行编码。<br />首先创建一个 配置类， MyBatchConfig.java：<br />从起名来看，可以知道这基本就是咱们整合spring batch 涉及到的一些配置组件都会写在这里了。<br />首先按照咱们上面的图来看，里面包含内容有：
```
JobRepository job的注册/存储器
JobLauncher job的执行器 
Job job任务，包含一个或多个Step
Step 包含（ItemReader、ItemProcessor和ItemWriter) 
ItemReader 数据读取器 
ItemProcessor 数据处理器
ItemWriter 数据输出器
```
首先，在MyBatchConfig类前加入注解：<br />`@Configuration`  用于告诉spring，咱们这个类是一个自定义配置类，里面很多bean都需要加载到spring容器里面<br />`@EnableBatchProcessing` 开启批处理支持<br />![2022-11-29-10-55-42.213184400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701730780-38958318-8a07-44c0-93ef-a3180f219917.png#averageHue=%2337373a&clientId=u8a4a0947-c497-4&from=ui&id=u573b216d&originHeight=105&originWidth=779&originalType=binary&ratio=1&rotation=0&showTitle=false&size=67675&status=done&style=none&taskId=ud624f62b-7a7d-4918-98bd-919e7916b51&title=)<br />然后开始往MyBatchConfig类里，编写各个小组件。
<a name="tZMVY"></a>
### JobRepository
写在MyBatchConfig类里
```java
/**
 * JobRepository定义：Job的注册容器以及和数据库打交道（事务管理等）
 * @param dataSource
 * @param transactionManager
 * @return
 * @throws Exception
 */
@Bean
public JobRepository myJobRepository(DataSource dataSource, PlatformTransactionManager transactionManager) throws Exception{
    JobRepositoryFactoryBean jobRepositoryFactoryBean = new JobRepositoryFactoryBean();
    jobRepositoryFactoryBean.setDatabaseType("mysql");
    jobRepositoryFactoryBean.setTransactionManager(transactionManager);
    jobRepositoryFactoryBean.setDataSource(dataSource);
    return jobRepositoryFactoryBean.getObject();
}
```
<a name="jP6uB"></a>
### JobLauncher
写在MyBatchConfig类里
```java
/**
 * jobLauncher定义：job的启动器,绑定相关的jobRepository
 * @param dataSource
 * @param transactionManager
 * @return
 * @throws Exception
 */
@Bean
public SimpleJobLauncher myJobLauncher(DataSource dataSource, PlatformTransactionManager transactionManager) throws Exception{
    SimpleJobLauncher jobLauncher = new SimpleJobLauncher();
    // 设置jobRepository
    jobLauncher.setJobRepository(myJobRepository(dataSource, transactionManager));
    return jobLauncher;
}
```
<a name="flfcd"></a>
### Job
写在MyBatchConfig类里
```java
/**
 * 定义job
 * @param jobs
 * @param myStep
 * @return
 */
@Bean
public Job myJob(JobBuilderFactory jobs, Step myStep){
    return jobs.get("myJob")
            .incrementer(new RunIdIncrementer())
            .flow(myStep)
            .end()
            .listener(myJobListener())
            .build();
}
```
对于Job的运行，是可以配置监听器的
<a name="m6Ma3"></a>
### JobListener
写在MyBatchConfig类里
```java
/**
 * 注册job监听器
 * @return
 */
@Bean
public MyJobListener myJobListener(){
    return new MyJobListener();
}
```
这是一个自己自定义的监听器，所以是单独创建的，MyJobListener.java：
```java
/**
 * @Description :监听Job执行情况，实现JobExecutorListener，且在batch配置类里，Job的Bean上绑定该监听器
 **/
 
public class MyJobListener implements JobExecutionListener {
 
    private Logger logger = LoggerFactory.getLogger(MyJobListener.class);
 
    @Override
    public void beforeJob(JobExecution jobExecution) {
        logger.info("job 开始, id={}",jobExecution.getJobId());
    }
 
    @Override
    public void afterJob(JobExecution jobExecution) {
        logger.info("job 结束, id={}",jobExecution.getJobId());
    }
}
```
<a name="icqas"></a>
#### Step（ItemReader   ItemProcessor   ItemWriter）
step里面包含数据读取器，数据处理器，数据输出器三个小组件的的实现。<br />也是一个个拆解来进行编写。<br />文章前边说到，该篇实现的场景包含两种，一种是从csv文件读入大量数据进行处理，另一种是从数据库表读入大量数据进行处理。
<a name="rDrKL"></a>
### 从CSV文件读取数据
<a name="qiTOH"></a>
#### ItemReader
写在MyBatchConfig类里
```java
/**
 * ItemReader定义：读取文件数据+entirty实体类映射
 * @return
 */
@Bean
public ItemReader<BlogInfo> reader(){
    // 使用FlatFileItemReader去读cvs文件，一行即一条数据
    FlatFileItemReader<BlogInfo> reader = new FlatFileItemReader<>();
    // 设置文件处在路径
    reader.setResource(new ClassPathResource("static/bloginfo.csv"));
    // entity与csv数据做映射
    reader.setLineMapper(new DefaultLineMapper<BlogInfo>() {
        {
            setLineTokenizer(new DelimitedLineTokenizer() {
                {
                    setNames(new String[]{"blogAuthor","blogUrl","blogTitle","blogItem"});
                }
            });
            setFieldSetMapper(new BeanWrapperFieldSetMapper<BlogInfo>() {
                {
                    setTargetType(BlogInfo.class);
                }
            });
        }
    });
    return reader;
}
```
简单代码解析：<br />![2022-11-29-10-55-42.422337800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669694834865-52e2893f-ce12-46c8-a559-a6461ced7430.png#averageHue=%23323337&clientId=u8a4a0947-c497-4&from=ui&id=u90691f22&originHeight=586&originWidth=959&originalType=binary&ratio=1&rotation=0&showTitle=false&size=366596&status=done&style=none&taskId=ua9eb18a2-07bc-423a-8f88-09549aa4754&title=)<br />对于数据读取器 `ItemReader` ，给它安排了一个读取监听器，创建 MyReadListener.java ：
```java
/**
 * @Description :
 **/
 
public class MyReadListener implements ItemReadListener<BlogInfo> {
 
    private Logger logger = LoggerFactory.getLogger(MyReadListener.class);
 
    @Override
    public void beforeRead() {
    }
 
    @Override
    public void afterRead(BlogInfo item) {
    }
 
    @Override
    public void onReadError(Exception ex) {
        try {
            logger.info(format("%s%n", ex.getMessage()));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="woAhI"></a>
#### ItemProcessor
写在MyBatchConfig类里
```java
/**
 * 注册ItemProcessor: 处理数据+校验数据
 * @return
 */
@Bean
public ItemProcessor<BlogInfo, BlogInfo> processor(){
    MyItemProcessor myItemProcessor = new MyItemProcessor();
    // 设置校验器
    myItemProcessor.setValidator(myBeanValidator());
    return myItemProcessor;
}
```
数据处理器，是自定义的，里面主要是包含对数据处理的业务逻辑，并且设置了一些数据校验器，这里使用 JSR-303的Validator来作为校验器。
<a name="SFfSe"></a>
#### 校验器
写在MyBatchConfig类里
```java
/**
 * 注册校验器
 * @return
 */
@Bean
public MyBeanValidator myBeanValidator(){
    return new MyBeanValidator<BlogInfo>();
}
```
创建MyItemProcessor.java ：<br />ps：里面的数据处理逻辑是，获取出读取数据里面的每条数据的blogItem字段，如果是SpringBoot，那就对title字段值进行替换。<br />其实也就是模拟一个简单地数据处理场景。
```java
import com.example.batchdemo.pojo.BlogInfo;
import org.springframework.batch.item.validator.ValidatingItemProcessor;
import org.springframework.batch.item.validator.ValidationException;
 
/**
 * @Description :
 **/
public class MyItemProcessor extends ValidatingItemProcessor<BlogInfo> {
    @Override
    public BlogInfo process(BlogInfo item) throws ValidationException {
        /**
         * 需要执行super.process(item)才会调用自定义校验器
         */
        super.process(item);
        /**
         * 对数据进行简单的处理
         */
        if (item.getBlogItem().equals("springboot")) {
            item.setBlogTitle("springboot 系列");
        } else {
            item.setBlogTitle("未知系列");
        }
        return item;
    }
}
```
创建MyBeanValidator.java：
```java
import org.springframework.batch.item.validator.ValidationException;
import org.springframework.batch.item.validator.Validator;
import org.springframework.beans.factory.InitializingBean;
import javax.validation.ConstraintViolation;
import javax.validation.Validation;
import javax.validation.ValidatorFactory;
import java.util.Set;
 
/**
 * @Description :
 **/
public class MyBeanValidator<T> implements Validator<T>, InitializingBean {
 
    private javax.validation.Validator validator;
 
    @Override
    public void validate(T value) throws ValidationException {
        /**
         * 使用Validator的validate方法校验数据
         */
        Set<ConstraintViolation<T>> constraintViolations =
                validator.validate(value);
        if (constraintViolations.size() > 0) {
            StringBuilder message = new StringBuilder();
            for (ConstraintViolation<T> constraintViolation : constraintViolations) {
                message.append(constraintViolation.getMessage() + "\n");
            }
            throw new ValidationException(message.toString());
        }
    }
 
    /**
     * 使用JSR-303的Validator来校验我们的数据，在此进行JSR-303的Validator的初始化
     * @throws Exception
     */
    @Override
    public void afterPropertiesSet() throws Exception {
        ValidatorFactory validatorFactory =
                Validation.buildDefaultValidatorFactory();
        validator = validatorFactory.usingContext().getValidator();
    }
 
}
```
ps：其实该篇文章没有使用这个数据校验器，大家想使用的话，可以在实体类上添加一些校验器的注解`@NotNull` `@Max` `@Email`等等。偏向于直接在处理器里面进行处理，想把关于数据处理的代码都写在一块。
<a name="sPkeB"></a>
#### ItemWriter
写在MyBatchConfig类里
```java
/**
 * ItemWriter定义：指定datasource，设置批量插入sql语句，写入数据库
 * @param dataSource
 * @return
 */
@Bean
public ItemWriter<BlogInfo> writer(DataSource dataSource){
    // 使用jdbcBcatchItemWrite写数据到数据库中
    JdbcBatchItemWriter<BlogInfo> writer = new JdbcBatchItemWriter<>();
    // 设置有参数的sql语句
    writer.setItemSqlParameterSourceProvider(new BeanPropertyItemSqlParameterSourceProvider<BlogInfo>());
    String sql = "insert into bloginfo "+" (blogAuthor,blogUrl,blogTitle,blogItem) "
            +" values(:blogAuthor,:blogUrl,:blogTitle,:blogItem)";
    writer.setSql(sql);
    writer.setDataSource(dataSource);
    return writer;
}
```
简单代码解析：<br />![2022-11-29-10-55-43.230352300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701030788-b2e57270-1535-40d3-91e4-9be37937725f.png#averageHue=%23333438&clientId=u8a4a0947-c497-4&from=ui&id=uc440f10d&originHeight=369&originWidth=1099&originalType=binary&ratio=1&rotation=0&showTitle=false&size=295349&status=done&style=none&taskId=u2d9ee405-2773-4c45-9062-ad5f1e31f3e&title=)<br />同样 对于数据读取器 `ItemWriter` ，给它也安排了一个输出监听器，创建 MyWriteListener.java：
```java
import com.example.batchdemo.pojo.BlogInfo;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.batch.core.ItemWriteListener;
import java.util.List;
import static java.lang.String.format;

public class MyWriteListener implements ItemWriteListener<BlogInfo> {
    private Logger logger = LoggerFactory.getLogger(MyWriteListener.class);
 
    @Override
    public void beforeWrite(List<? extends BlogInfo> items) {
    }
 
    @Override
    public void afterWrite(List<? extends BlogInfo> items) {
    }
 
    @Override
    public void onWriteError(Exception exception, List<? extends BlogInfo> items) {
        try {
            logger.info(format("%s%n", exception.getMessage()));
            for (BlogInfo message : items) {
                logger.info(format("Failed writing BlogInfo : %s", message.toString()));
            }
 
        } catch (Exception e) {
            e.printStackTrace();
        }
 
    }
}
```
`ItemReader`、`ItemProcessor`、`ItemWriter`，这三个小组件到这里，都实现了，那么接下来就是把这三个小组件跟step去绑定起来。<br />写在MyBatchConfig类里
```java
/**
 * step定义：
 * 包括
 * ItemReader 读取
 * ItemProcessor  处理
 * ItemWriter 输出
 * @param stepBuilderFactory
 * @param reader
 * @param writer
 * @param processor
 * @return
 */

@Bean
public Step myStep(StepBuilderFactory stepBuilderFactory, ItemReader<BlogInfo> reader,
                 ItemWriter<BlogInfo> writer, ItemProcessor<BlogInfo, BlogInfo> processor){
    return stepBuilderFactory
            .get("myStep")
            .<BlogInfo, BlogInfo>chunk(65000) // Chunk的机制(即每次读取一条数据，再处理一条数据，累积到一定数量后再一次性交给writer进行写入操作)
            .reader(reader).faultTolerant().retryLimit(3).retry(Exception.class).skip(Exception.class).skipLimit(2)
            .listener(new MyReadListener())
            .processor(processor)
            .writer(writer).faultTolerant().skip(Exception.class).skipLimit(2)
            .listener(new MyWriteListener())
            .build();
}
```
这个Step，稍作讲解。<br />前边提到了，Spring Batch框架，提供了事务的控制，重启，检测跳过等等机制。<br />那么，这些东西的实现，很多都在于这个step环节的设置。<br />首先看到代码出现的第一个设置，`chunk( 6500 )` ，Chunk的机制(即每次读取一条数据，再处理一条数据，累积到一定数量后再一次性交给writer进行写入操作。<br />没错，对于整个step环节，就是数据的读取，处理最后到输出。<br />这个chunk机制里，传入的 6500，也就是是告诉它，读取处理数据，累计达到 6500条进行一次批次处理，去执行写入操作。<br />这个传值，是根据具体业务而定，可以是500条一次，1000条一次，也可以是20条一次，50条一次。<br />通过一张简单的小图来帮助理解：<br />![2022-11-29-10-55-43.750618400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701093786-8103beab-e076-49ea-a99c-90bb8282745b.png#averageHue=%23a7dd01&clientId=u8a4a0947-c497-4&from=ui&id=ud2f466b6&originHeight=826&originWidth=945&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3128237&status=done&style=none&taskId=u1e9edf90-9e7e-4c27-bcf8-999884233d7&title=)<br />在大量数据处理，不管是读取或者说是写入，都肯定会涉及到一些未知或者已知因素导致某条数据失败了。<br />那么如果说咱们啥也不设置，失败一条数据，那么就当作整个失败了？。显然这个太不人性，所以spring batch 提供了 retry 和 skip 两个设置（其实还有restart） ，通过这两个设置来人性化地解决一些数据操作失败场景。
```java
retryLimit(3).retry(Exception.class)
```
没错，这个就是设置重试，当出现异常的时候，重试多少次。设置为3，也就是说当一条数据操作失败，那会对这条数据进行重试3次，还是失败就是 当做失败了， 那么如果有配置skip（推荐配置使用），那么这个数据失败记录就会留到给 skip 来处理。
```java
skip(Exception.class).skipLimit(2)
```
skip，跳过，也就是说如果设置3， 那么就是可以容忍 3条数据的失败。只有达到失败数据达到3次，才中断这个step。<br />对于失败的数据，做了相关的监听器以及异常信息记录，供与后续手动补救。<br />那么记下来开始去调用这个批处理job，通过接口去触发这个批处理事件，新建一个Controller，TestController.java：
```java
@RestController
public class TestController {
    @Autowired
    SimpleJobLauncher jobLauncher;
 
    @Autowired
    Job myJob;
 
    @GetMapping("testJob")
    public  void testJob() throws JobParametersInvalidException, JobExecutionAlreadyRunningException, JobRestartException, JobInstanceAlreadyCompleteException {
     //    后置参数：使用JobParameters中绑定参数 addLong  addString 等方法
        JobParameters jobParameters = new JobParametersBuilder().toJobParameters();
        jobLauncher.run(myJob, jobParameters);
 
    }
}
```
准备了一个csv文件 bloginfo.csv，里面大概8万多条数据，用来进行批处理测试：<br />![2022-11-29-10-55-43.979594100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701196473-328a2489-8acc-436c-a76b-5140e139c727.png#averageHue=%23ebeaea&clientId=u8a4a0947-c497-4&from=ui&id=u161b941a&originHeight=660&originWidth=929&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104644&status=done&style=none&taskId=u8e15c9d8-32a2-432c-b7c8-3f787bd9b7e&title=)<br />这个文件的路径跟数据读取器里面读取的路径要一直，<br />![2022-11-29-10-55-44.281980300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701196410-ff01a6b5-6167-4230-bc62-aee3e2fae141.png#averageHue=%2336373b&clientId=u8a4a0947-c497-4&from=ui&id=i6S5p&originHeight=51&originWidth=749&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25347&status=done&style=none&taskId=ua1231fd8-cbd2-4723-9403-a4a9373099e&title=)<br />![2022-11-29-10-55-44.388993800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701196423-41867999-914e-42b3-9f0a-ce052b3b24fa.png#averageHue=%234d4c4d&clientId=u8a4a0947-c497-4&from=ui&id=iGf4S&originHeight=151&originWidth=264&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20176&status=done&style=none&taskId=u0f5b3d07-b24b-48c9-900c-771719f31af&title=)<br />目前数据库是这个样子，<br />![2022-11-29-10-55-44.566336500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701225752-80ad049a-2018-4643-b33b-0f644e81b753.png#averageHue=%23f2f2f2&clientId=u8a4a0947-c497-4&from=ui&id=u74b3b1a0&originHeight=68&originWidth=1336&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10011&status=done&style=none&taskId=ud8f6aad2-bc60-4582-b536-f5cbd3c1f26&title=)<br />接下来把项目启动起来，再看一眼数据库，生成了一些batch用来跟踪记录job的一些数据表：<br />![2022-11-29-10-55-44.586812100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701225766-0c219713-8154-4343-a19c-6a51e284851b.png#averageHue=%23f0eded&clientId=u8a4a0947-c497-4&from=ui&id=DyIja&originHeight=240&originWidth=260&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8687&status=done&style=none&taskId=ue7821af3-a069-4c8b-a2de-f1b0444d0e6&title=)<br />来调用一下testJob接口，<br />![2022-11-29-10-55-44.679883200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701226083-8b121e73-2bb2-4f1f-ad03-caff47619e28.png#averageHue=%232e2f32&clientId=u8a4a0947-c497-4&from=ui&id=Z8Jlu&originHeight=206&originWidth=1508&originalType=binary&ratio=1&rotation=0&showTitle=false&size=298718&status=done&style=none&taskId=ub05b3464-d876-4911-ab98-f4acfc907de&title=)<br />然后看下数据库，可以看的数据全部都进行了相关的逻辑处理并插入到了数据库。<br />到这里，对Springboot 整合 spring batch 其实已经操作完毕了，也实现了从csv文件读取数据处理存储的业务场景。<br />从数据库读取数据<br />ps：前排提示使用druid有坑。后面会讲到。<br />那么接下来实现场景，从数据库表内读取数据进行处理输出到新的表里面。<br />那么基于上边的整合，已经实现了
```
JobRepository job的注册/存储器
JobLauncher job的执行器 
Job job任务，包含一个或多个Step
Step 包含（ItemReader、ItemProcessor和ItemWriter) 
ItemReader 数据读取器 
ItemProcessor 数据处理器
ItemWriter 数据输出器
job 监听器
reader 监听器
writer 监听器
process 数据校验器
```
那么对于新写一个job完成 一个新的场景，需要全部重写么？<br />显然没必要，当然完全新写一套也是可以的。<br />那么该篇，对于一个新的也出场景，从csv文件读取数据转换到数据库表读取数据，重新新建的有：

1. **数据读取器：**  原先使用的是 `FlatFileItemReader` ，现在改为使用 `MyBatisCursorItemReader`
2. **数据处理器：**  新的场景，业务为了好扩展，所以处理器最好也新建一个
3. **数据输出器：**    新的场景，业务为了好扩展，所以数据输出器最好也新建一个
4. **step的绑定设置：** 新的场景，业务为了好扩展，所以step最好也新建一个
5. **Job：**  当然是要重新写一个了

其他照用原先的就行，`JobRepository`，`JobLauncher`以及各种监听器啥的，暂且不重新建了。<br />新建MyItemProcessorNew.java：
```java
import org.springframework.batch.item.validator.ValidatingItemProcessor;
import org.springframework.batch.item.validator.ValidationException;

public class MyItemProcessorNew extends ValidatingItemProcessor<BlogInfo> {
    @Override
    public BlogInfo process(BlogInfo item) throws ValidationException {
        /**
         * 需要执行super.process(item)才会调用自定义校验器
         */
        super.process(item);
        /**
         * 对数据进行简单的处理
         */
        Integer authorId= Integer.valueOf(item.getBlogAuthor());
        if (authorId<20000) {
            item.setBlogTitle("这是都是小于20000的数据");
        } else if (authorId>20000 && authorId<30000){
            item.setBlogTitle("这是都是小于30000但是大于20000的数据");
        }else {
            item.setBlogTitle("旧书不厌百回读");
        }
        return item;
    }
}
```
然后其他重新定义的小组件，写在MyBatchConfig类里：
```java
/**
 * 定义job
 * @param jobs
 * @param stepNew
 * @return
 */
@Bean
public Job myJobNew(JobBuilderFactory jobs, Step stepNew){
    return jobs.get("myJobNew")
            .incrementer(new RunIdIncrementer())
            .flow(stepNew)
            .end()
            .listener(myJobListener())
            .build();
}


@Bean
public Step stepNew(StepBuilderFactory stepBuilderFactory, MyBatisCursorItemReader<BlogInfo> itemReaderNew,
                    ItemWriter<BlogInfo> writerNew, ItemProcessor<BlogInfo, BlogInfo> processorNew){
    return stepBuilderFactory
            .get("stepNew")
            .<BlogInfo, BlogInfo>chunk(65000) // Chunk的机制(即每次读取一条数据，再处理一条数据，累积到一定数量后再一次性交给writer进行写入操作)
            .reader(itemReaderNew).faultTolerant().retryLimit(3).retry(Exception.class).skip(Exception.class).skipLimit(10)
            .listener(new MyReadListener())
            .processor(processorNew)
            .writer(writerNew).faultTolerant().skip(Exception.class).skipLimit(2)
            .listener(new MyWriteListener())
            .build();
}

@Bean
public ItemProcessor<BlogInfo, BlogInfo> processorNew(){
    MyItemProcessorNew csvItemProcessor = new MyItemProcessorNew();
    // 设置校验器
    csvItemProcessor.setValidator(myBeanValidator());
    return csvItemProcessor;
}



@Autowired
private SqlSessionFactory sqlSessionFactory;

@Bean
@StepScope
//Spring Batch提供了一个特殊的bean scope类（StepScope:作为一个自定义的Spring bean scope）。这个step scope的作用是连接batches的各个steps。这个机制允许配置在Spring的beans当steps开始时才实例化并且允许你为这个step指定配置和参数。
public MyBatisCursorItemReader<BlogInfo> itemReaderNew(@Value("#{jobParameters[authorId]}") String authorId) {

        System.out.println("开始查询数据库");

        MyBatisCursorItemReader<BlogInfo> reader = new MyBatisCursorItemReader<>();

        reader.setQueryId("com.example.batchdemo.mapper.BlogMapper.queryInfoById");

        reader.setSqlSessionFactory(sqlSessionFactory);
         Map<String , Object> map = new HashMap<>();

          map.put("authorId" , Integer.valueOf(authorId));
         reader.setParameterValues(map);
        return reader;
}

/**
 * ItemWriter定义：指定datasource，设置批量插入sql语句，写入数据库
 * @param dataSource
 * @return
 */
@Bean
public ItemWriter<BlogInfo> writerNew(DataSource dataSource){
    // 使用jdbcBcatchItemWrite写数据到数据库中
    JdbcBatchItemWriter<BlogInfo> writer = new JdbcBatchItemWriter<>();
    // 设置有参数的sql语句
    writer.setItemSqlParameterSourceProvider(new BeanPropertyItemSqlParameterSourceProvider<BlogInfo>());
    String sql = "insert into bloginfonew "+" (blogAuthor,blogUrl,blogTitle,blogItem) "
            +" values(:blogAuthor,:blogUrl,:blogTitle,:blogItem)";
    writer.setSql(sql);
    writer.setDataSource(dataSource);
    return writer;
}
```
<a name="J1njf"></a>
### 代码需要注意的点
数据读取器 MyBatisCursorItemReader<br />![2022-11-29-10-55-45.690382900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701385720-34644c69-026b-4d4f-b33e-f779fba1ce65.png#averageHue=%23323337&clientId=u8a4a0947-c497-4&from=ui&id=u758954bd&originHeight=430&originWidth=1193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=372957&status=done&style=none&taskId=u78350f56-0b39-4448-88f9-fc3596b026b&title=)<br />对应的mapper方法：<br />![2022-11-29-10-55-47.126667300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701385619-5c4e88c7-aa5f-444d-aaed-637336c56fb0.png#averageHue=%23333438&clientId=u8a4a0947-c497-4&from=ui&id=Vt2KQ&originHeight=259&originWidth=1140&originalType=binary&ratio=1&rotation=0&showTitle=false&size=196094&status=done&style=none&taskId=ud3bfac93-81ab-4118-b183-8ca934f0ece&title=)<br />数据处理器 MyItemProcessorNew：<br />![2022-11-29-10-55-48.155776400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701385708-3c0dca44-5013-4cb3-bf38-ddc9d687aa56.png#averageHue=%23343538&clientId=u8a4a0947-c497-4&from=ui&id=STKgA&originHeight=383&originWidth=742&originalType=binary&ratio=1&rotation=0&showTitle=false&size=234921&status=done&style=none&taskId=udd093e6b-5f3d-4d24-a63c-254c19ed6ca&title=)<br />数据输出器，新插入到别的数据库表去，特意这样为了测试：<br />![2022-11-29-10-55-49.790940800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701414375-54e43d7b-c8db-458d-a5fa-7b86236859f8.png#averageHue=%23303134&clientId=u8a4a0947-c497-4&from=ui&id=uaedb968b&originHeight=269&originWidth=1126&originalType=binary&ratio=1&rotation=0&showTitle=false&size=251029&status=done&style=none&taskId=u1964ad06-1963-4e05-a504-4775d95e3b2&title=)<br />当然数据库为了测试这个场景，也是新建了一张表，bloginfonew 表。<br />![2022-11-29-10-55-51.528876700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701414212-d414c7d0-b49b-4200-9028-0a8a7074c963.png#averageHue=%23ededed&clientId=u8a4a0947-c497-4&from=ui&id=WbepO&originHeight=77&originWidth=679&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8816&status=done&style=none&taskId=u102cfa98-27ca-41ae-8fa7-1e8361ecd5e&title=)<br />接下来，新写一个接口来执行新的这个job：<br />![2022-11-29-10-55-51.626661100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701414362-905be11f-e7a7-4fe8-a95c-b67b7b34a880.png#averageHue=%2332353a&clientId=u8a4a0947-c497-4&from=ui&id=nSMeE&originHeight=336&originWidth=1198&originalType=binary&ratio=1&rotation=0&showTitle=false&size=214343&status=done&style=none&taskId=u03c59d66-780e-4fe9-8bf9-360deff8875&title=)
```java
@Autowired
SimpleJobLauncher jobLauncher;

@Autowired
Job myJobNew;

@GetMapping("testJobNew")
public  void testJobNew(@RequestParam("authorId") String authorId) throws JobParametersInvalidException, JobExecutionAlreadyRunningException, JobRestartException, JobInstanceAlreadyCompleteException {

    JobParameters jobParametersNew = new JobParametersBuilder().addLong("timeNew", System.currentTimeMillis())
            .addString("authorId",authorId)
            .toJobParameters();
    jobLauncher.run(myJobNew,jobParametersNew);

}
```
来调用一些这个接口：<br />![2022-11-29-10-55-53.071878400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701467313-101477ac-624a-4b05-b3bc-a58359838976.png#averageHue=%23faf8f8&clientId=u8a4a0947-c497-4&from=ui&id=uffb5baa4&originHeight=199&originWidth=573&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9998&status=done&style=none&taskId=u92fc940c-66e6-4d3f-8c7e-ff8b48daa57&title=)<br />看下控制台：<br />![2022-11-29-10-55-53.171634900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701467535-05e8c120-0121-4b8a-9083-85f6412b1ab8.png#averageHue=%23343437&clientId=u8a4a0947-c497-4&from=ui&id=CDX7B&originHeight=429&originWidth=1284&originalType=binary&ratio=1&rotation=0&showTitle=false&size=452487&status=done&style=none&taskId=udc1269d6-a538-49b8-aa89-b0af6702c11&title=)<br />没错，这就是失败的，原因是因为跟druid有关，报了一个数据库功能不支持。这是在数据读取的时候报的错。<br />初步测试认为是MyBatisCursorItemReader ，druid 数据库连接池不支持。<br />那么，只需要：

1. 注释掉druid连接池 jar依赖

![2022-11-29-10-55-55.931146600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701467324-896b23ed-b170-4cc1-a05f-a8e2ec2764f5.png#averageHue=%23363537&clientId=u8a4a0947-c497-4&from=ui&id=IdJy3&originHeight=148&originWidth=943&originalType=binary&ratio=1&rotation=0&showTitle=false&size=99175&status=done&style=none&taskId=uaf292edc-3865-436a-8193-5a6a4816374&title=)

1. yml里替换连接池配置

![2022-11-29-10-55-56.348267200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701525003-e38a6d1a-6376-4d79-bda1-c2b6ee2fb296.png#averageHue=%23363639&clientId=u8a4a0947-c497-4&from=ui&id=uf628899f&originHeight=142&originWidth=607&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38263&status=done&style=none&taskId=ud46bec8e-b1dc-4a67-9b25-c5285ed97fc&title=)<br />其实不配置其他连接池，SpringBoot 2.X 版本已经整合了默认的连接池 HikariCP 。
:::danger
在Springboot2.X版本，数据库的连接池官方推荐使用HikariCP
:::
如果不是为了druid的那些后台监控数据，sql分析等等，完全是优先使用HikariCP的。<br />官方的原话：
> We preferHikariCPfor its performance and concurrency. If HikariCP is available, we always choose it.

翻译：<br />我们更喜欢hikaricpf的性能和并发性。如果有HikariCP，我们总是选择它。<br />所以就啥连接池也不配了，使用默认的HikariCP 连接池。<br />当然如果想配，也是可以的：<br />![2022-11-29-10-55-56.555095500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701525022-57f9121a-d8b5-471c-a2c2-b4129ed7e603.png#averageHue=%23444345&clientId=u8a4a0947-c497-4&from=ui&id=OTr1s&originHeight=287&originWidth=488&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43836&status=done&style=none&taskId=u79589c11-4f34-4d87-98cb-339ba610e0c&title=)<br />所以剔除掉druid连接池后，再来调用一下新接口：<br />![2022-11-29-10-55-56.954695700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701525019-f7ca1cff-6b03-4a37-baf5-61e1322eef61.png#averageHue=%23f9f6f5&clientId=u8a4a0947-c497-4&from=ui&id=jjwBO&originHeight=245&originWidth=636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12794&status=done&style=none&taskId=u17b98641-a5ab-4386-9ef4-7cc8cbe3ddf&title=)<br />可以看到，从数据库获取数据并进行批次处理写入job是成功的：<br />![2022-11-29-10-55-57.262231200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701591562-275ea5a4-3e98-4ca6-ae38-4a5737774299.png#averageHue=%232d2e31&clientId=u8a4a0947-c497-4&from=ui&id=uab62d21e&originHeight=217&originWidth=1507&originalType=binary&ratio=1&rotation=0&showTitle=false&size=265165&status=done&style=none&taskId=udd954d2e-dbee-4c38-9b9d-a85fde411d1&title=)<br />新的表里面插入的数据都进行了自己写的逻辑处理：<br />![2022-11-29-10-55-58.388169600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669701591432-54f8a6c5-1973-4abf-8027-7f23fe748d5a.png#averageHue=%23ebe9e9&clientId=u8a4a0947-c497-4&from=ui&id=bGdeE&originHeight=599&originWidth=897&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56878&status=done&style=none&taskId=u6317ea40-1c94-419d-9e43-209460b2a07&title=)
