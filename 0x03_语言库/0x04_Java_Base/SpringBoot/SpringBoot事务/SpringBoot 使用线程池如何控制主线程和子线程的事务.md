JavaSpringBoot

- 使用场景
- 思路
- 代码及注释如下：
- 测试验证：
   - 情况1：子线程中有一个执行失败
   - 情况2：删除 t_person表中id为201的数据重新插入\编辑
   - 情况3：主线程报错就不演示了
<a name="ujuGn"></a>
## 一、使用场景
数据库有两张表 t_person 和 t_school 如下：前端传来10000条person数据要插入到t_person，同时要删除t_school表中id为1的数据（为提高效率采用线程池做）<br />![2023-02-08-09-57-20.888500400.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821834643-15fbe8d2-22e3-4f6e-bb86-c2389ce700fe.png#averageHue=%23f9f4f3&clientId=ubf643c22-3aa2-4&from=ui&id=u3840b09a&originHeight=375&originWidth=985&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1110369&status=done&style=none&taskId=u11b0c5f4-0e97-4603-9149-d0062e7b3b5&title=)![2023-02-08-09-57-21.808493300.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821834796-efe47239-b6cf-4b3b-877c-eab7dbf26e70.png#averageHue=%23faf7f6&clientId=ubf643c22-3aa2-4&from=ui&id=u63aacaac&originHeight=366&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1188198&status=done&style=none&taskId=uaee2e6ce-b395-465c-bba8-6cf2d7ae200&title=)
<a name="CgB4e"></a>
## 二、思路
1、要保证主线程和子线程使用的同一个sqlSession<br />2、手动控制提交和回滚<br />3、将10000条数据均分成10份，每份1000条，创建10个任务，放入线程池执行！
<a name="OyWdl"></a>
## 三、代码及注释如下
1、核心业务代码
```java
@Service
public class PersonServiceImpl extends ServiceImpl<PersonMapper, Person> implements IPersonService {

    @Autowired
    private SqlSessionTemplate sqlSessionTemplate;

    @Autowired
    private SchoolMapper schoolMapper;

    private ArrayBlockingQueue queue=new ArrayBlockingQueue(8,true);

    private ThreadPoolExecutor.CallerRunsPolicy policy=new ThreadPoolExecutor.CallerRunsPolicy();

    //1、创建核心线程为10的线程池
    private ThreadPoolExecutor executor = new ThreadPoolExecutor(10,15,10, TimeUnit.SECONDS
            ,queue,policy);


    @Override
    public int insertPerson(Person person) {
        return this.baseMapper.insert(person);
    }

    @Override
    @Transactional
    public void inserPersonBatch(List<Person> list) throws SQLException {

        //2、根据sqlSessionTemplate获取SqlSession工厂
        SqlSessionFactory sqlSessionFactory = sqlSessionTemplate.getSqlSessionFactory();
        SqlSession sqlSession = sqlSessionFactory.openSession();
        //3、获取Connection来手动控制事务
        Connection connection = sqlSession.getConnection();
        try{
            //4、设置手动提交
            connection.setAutoCommit(false);
            //5、获取PersonMapper（此处是由于无法通过this.baseMapper调用自定义的saveBatch方法）
            PersonMapper mapper = sqlSession.getMapper(PersonMapper.class);
            //6、主线程去删除t_school表中id为1的数据
            schoolMapper.deleteById("1");
            //7、将传入List中的10000个数据按1000一组均分成10组
            List<List<Person>> lists = ListUtils.averageAssign(list,1000);
            //8、新建任务列表
            List<Callable<Integer>> callableList = new ArrayList<>();
            //9、根据均分的5组数据分别新建5个Callable任务
            for(int i = 0; i < lists.size(); i++){
                List<Person> insertList = lists.get(i);
                Callable<Integer> callable = new Callable<Integer>() {
                    @Override
                    public Integer call() throws Exception {
                        int n = 0;
                        try{
                            n = mapper.saveBatch(insertList);
                        }catch (Exception e){
                            //插入失败返回0
                            return n;
                        }
                        //插入成功返回成功提交数
                        return n;
                    }
                };
                callableList.add(callable);
            }

            //10、任务放入线程池开始执行
            List<Future<Integer>> futures = executor.invokeAll(callableList);
            //11、对比每个任务的返回值 <= 0 代表执行失败
            for(Future<Integer> future : futures){
                if(future.get() <= 0){
                    //12、只要有一组任务失败回滚整个connection
                    connection.rollback();
                    return;
                }
            }
            //13、主线程和子线程都执行成功 直接提交
            connection.commit();
            System.out.println("添加成功！");

        }catch (Exception e){
            //14、主线程报错回滚
            connection.rollback();
            log.error(e.toString());
            throw new SQLException("出现异常！");
        }
        return;
    }
}
```
2、PersonMapper中自定义批量插入
```xml
<insert id="saveBatch" parameterType="list">
    insert into t_person(id,name,age,addr,classes,school_id)
    values
        <foreach collection="list" index="index" item="item" separator=",">
            (
             #{item.id},
            #{item.name},
            #{item.age},
            #{item.addr},
            #{item.classes},
            #{item.schoolId}
            )
        </foreach>
</insert>
```
3、均分List工具类
```java
public class ListUtils {

    public static <T> List<List<T>> averageAssign(List<T> source, int limit) {
        if (null == source || source.isEmpty()) {
            return Collections.emptyList();
        }
        List<List<T>> result = new ArrayList<>();
        int listCount = (source.size() - 1) / limit + 1;
        int remaider = source.size() % listCount; // (先计算出余数)
        int number = source.size() / listCount; // 然后是商
        int offset = 0;// 偏移量
        for (int i = 0; i < listCount; i++) {
            List<T> value;
            if (remaider > 0) {
                value = source.subList(i * number + offset, (i + 1) * number + offset + 1);
                remaider--;
                offset++;
            } else {
                value = source.subList(i * number + offset, (i + 1) * number + offset);
            }
            result.add(value);
        }
        return result;
    }
}
```
<a name="I2W6t"></a>
## 四、测试验证：
controller层如下：传入10000条数据
```java
@GetMapping("/addBatch")
public void addBatch() {
    List<Person> list = new ArrayList<>();
    for(int i = 1; i <= 10000; i++){
        Person p = new Person();
        p.setId(i);
        p.setName("张三" + i);
        p.setAge(i);
        p.setAddr("重庆");
        p.setClasses("一班");
        p.setSchoolId(i);
        list.add(p);
    }
    try{
        this.iPersonService.inserPersonBatch(list);
    }catch (Exception e){
        e.printStackTrace();
    }
}
```
<a name="bvj2T"></a>
#### 1、情况1：子线程中有一个执行失败
t_person表主键唯一  10000条Person数据id按1—10000设置<br />如图t_person表中已经有一条id为201的数据 所以线程池中有一个任务执行会失败！<br />![2023-02-08-09-57-22.097617100.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821904913-286cfee3-604b-45e3-8c02-e30ef754a7dc.png#averageHue=%23f9f4f4&clientId=ubf643c22-3aa2-4&from=ui&id=ud2f6740c&originHeight=397&originWidth=952&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1136139&status=done&style=none&taskId=u5b20ca45-459c-441e-a276-0d73d2695e0&title=)<br />打断点来看：此时已经分配好10个任务<br />![2023-02-08-09-57-22.342380500.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821906520-6c35c40c-1658-49d9-bce0-d2840ce88c14.png#averageHue=%235f6766&clientId=ubf643c22-3aa2-4&from=ui&id=WoMXU&originHeight=422&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1369983&status=done&style=none&taskId=u8e648593-5645-48dd-8402-75465f03d24&title=)<br />如下图：插入id为201的数据时失败，线程池第一个任务执行失败返回0，其余全部成功返回1000<br />![2023-02-08-09-57-22.590085200.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821909784-4f725fa6-ae88-4b20-afa5-99876b18a158.png#averageHue=%23566467&clientId=ubf643c22-3aa2-4&from=ui&id=aUXuv&originHeight=787&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2554868&status=done&style=none&taskId=u86c9f0ea-1187-49c4-bcfc-6a86fe80f33&title=)<br />执行rollback回滚<br />![2023-02-08-09-57-23.763673300.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821942121-680f3c1f-9082-44d1-9e7a-7eebbbae755c.png#averageHue=%232f2c2b&clientId=ubf643c22-3aa2-4&from=ui&id=uea2fb1fc&originHeight=576&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1869904&status=done&style=none&taskId=ubf4572c7-60a1-411e-8e5f-ccb70517ecb&title=)<br />执行完毕观察数据库：<br />t_school表数据没有被删，<br />![2023-02-08-09-57-24.475213700.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821940277-e102814e-9515-4d6e-be1c-1f568d81fee5.png#averageHue=%23f9f5f5&clientId=ubf643c22-3aa2-4&from=ui&id=BGFJA&originHeight=367&originWidth=964&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1063523&status=done&style=none&taskId=ub40b64ef-f8dc-4bd7-b1dc-d77227a1d71&title=)<br />t_person表数据也没有变化<br />![2023-02-08-09-57-24.584047900.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675821940175-4c815dc5-1ddd-4b95-9625-541fbce23107.png#averageHue=%23f8f7f6&clientId=ubf643c22-3aa2-4&from=ui&id=DJUAa&originHeight=361&originWidth=916&originalType=binary&ratio=1&rotation=0&showTitle=false&size=994080&status=done&style=none&taskId=u79468634-4755-41a3-aa63-4cfbe7beb7a&title=)
<a name="LckIt"></a>
#### 2、情况2、删除 t_person表中id为201的数据重新插入
![2023-02-08-09-57-24.665933800.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675822057285-4d5ca0bf-a7e2-40c2-8b7c-cdaec30e5f65.png#averageHue=%23f8f8f7&clientId=ubf643c22-3aa2-4&from=ui&id=u82e78ac4&originHeight=356&originWidth=960&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1027375&status=done&style=none&taskId=u1a79ff26-3ec2-440a-8c2b-3f851a51c75&title=)<br />此时10个任务全部执行成功：<br />![2023-02-08-09-57-24.889187900.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675822061519-9d481dfe-caea-4701-9634-5f41fd008646.png#averageHue=%233c4145&clientId=ubf643c22-3aa2-4&from=ui&id=PEF6z&originHeight=775&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2515911&status=done&style=none&taskId=ua7f133ff-4b2d-4f83-a409-c0b7dc849c4&title=)<br />执行commit<br />![2023-02-08-09-57-25.810930600.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675822093213-dbc858b1-325b-4fc8-9543-f07dce80f345.png#averageHue=%232e2b2b&clientId=ubf643c22-3aa2-4&from=ui&id=ua6721bfe&originHeight=640&originWidth=934&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1796902&status=done&style=none&taskId=u8ff81f0e-d887-4f92-bd32-45be58ef07a&title=)<br />执行完毕观察数据库：<br />t_school表数据已被删除<br />![2023-02-08-09-57-26.127652100.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675822092372-f45dc349-0052-4638-8663-ae99d85b9428.png#averageHue=%23f8f7f6&clientId=ubf643c22-3aa2-4&from=ui&id=WfljX&originHeight=382&originWidth=961&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1103545&status=done&style=none&taskId=u404200b0-5b56-4f97-80f1-6bb4230b1f5&title=)<br />t_person表中10000条数据也成功插入：<br />![2023-02-08-09-57-26.232931600.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675822089163-9da1f7c5-f1d1-4335-8295-ca52d82e9e90.png#averageHue=%23f6f4f3&clientId=ubf643c22-3aa2-4&from=ui&id=G7Cd5&originHeight=790&originWidth=1018&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75670&status=done&style=none&taskId=ud2d5b21d-6d05-462a-be1d-caf17feaff3&title=)
<a name="JwhUe"></a>
#### 3、情况3：主线程报错就不演示了
以上测试成功！
