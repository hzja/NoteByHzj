JavaSpringBootMybatis
<a name="cecPB"></a>
## 前言
Mybatis的流式查询，有点冷门，实际用的场景比较少，但是在某些特殊场景下，却是十分有效的一个方法。很多人没有听说过，实际上是对Mybatis没有太重视，对Mybatis想法还停留一个dao接口对应着mapper里的一个sql，mybatis的关键是如何写好sql以及sql的优化上；<br />其实Mybatis远不止这些，通过这篇文章，和大家一块来见识一下流式查询，Mybatis相对冷门的神秘面纱。文章的内容将从以下几个方面展开：

- 什么是Mybatis的流式查询；
- Cursor接口的主要方法；
- 代码层面如何实现；
- 具体的应用场景；
- 使用中的一些注意事项；
<a name="VdP85"></a>
## 什么是Mybatis流式查询？
使用Mybatis作为持久层的框架时，通过Mybatis执行查询数据的请求执行成功后，Mybatis返回的结果集不是一个集合或对象，而是一个迭代器，可以通过遍历迭代器来取出结果集，避免一次性取出大量的数据而占用太多的内存。
<a name="luaeH"></a>
## Cursor
org.apache.ibatis.cursor.Cursor接口有三个抽象方法，分别是

- `**isOpen()**`：判断cursor是否正处于打开状态;
- `**isConsumed()**`：判断查询结果是否全部读取完；
- `**getCurrentIndex()**`：查询已读取数据在全部数据里的索引位置；
```java
public interface Cursor<T> extends Closeable, Iterable<T> {
    //判断cursor是否正处于打开状态
    //当返回true，则表示cursor已经开始从数据库里刷新数据了；
    boolean isOpen();
    //判断查询结果是否全部读取完；
    //当返回true,则表示查询sql匹配的全部数据都消费完了；
    boolean isConsumed();
    //查询已读取数据在全部数据里的索引位置；
    //第一条数据的索引位置为0；当返回索引位置为-1时，则表示已经没有数据可以读取；
    int getCurrentIndex();
}
```
<a name="Aco2S"></a>
## 代码实现
Mybatis的所谓流式查询，就是服务端程序查询数据的过程中，与远程数据库一直保持连接，不断的去数据库拉取数据，提交事务并关闭sqlsession后，数据库连接断开，停止数据拉取，需要注意的是使用这种方式，需要自己手动维护sqlsession和事务的提交。<br />1、实现方式很简单，原来返回的类型是集合或对象，流式查询返回的的类型Curor，泛型内表示实际的类型，其他没有变化；
```java
@Mapper
public interface PersonDao {
    Cursor<Person> selectByCursor();
    Integer queryCount();

}
```
```xml
<select id="selectByCursor" resultMap="personMap">
  select * from sys_person order by id desc
</select>
<select id="queryCount" resultType="java.lang.Integer">
  select count(*) from sys_person
</select>
```
2、dao层向service层返回的是Cursor类型对象，只要不提交关闭sqlsession，服务端程序就可以一直从数据数据库读取数据，直到查询sql匹配到数据全部读取完；<br />示例里的主要业务逻辑是：从sys_person表中读取所有的人员信息数据，然后按照每1000条数据为一组，读取到内存里进行处理，以此类推，直到查询sql匹配到数据全部处理完，再提交事务，关闭sqlSession；
```java
@Service
@Slf4j
public class PersonServiceImpl implements IPersonService {
    @Autowired
    private SqlSessionFactory sqlSessionFactory;
 
    @Override
    public void getOneByAsync() throws InterruptedException {
        new Thread(new Runnable() {
            @SneakyThrows
            @Override
            public void run() {
                //使用sqlSessionFactory打开一个sqlSession，在没有读取完数据之前不要提交事务或关闭sqlSession
                log.info("----开启sqlSession");
                SqlSession sqlSession = sqlSessionFactory.openSession();
                 try {
                     //获取到指定mapper
                     PersonDao mapper = sqlSession.getMapper(PersonDao.class);
                     //调用指定mapper的方法，返回一个cursor
                     Cursor<Person> cursor = mapper.selectByCursor();
                     //查询数据总量
                     Integer total = mapper.queryCount();
                     //定义一个list，用来从cursor中读取数据，每读取够1000条的时候，开始处理这批数据；
                     //当前批数据处理完之后，清空list，准备接收下一批次数据；直到大量的数据全部处理完；
                     List<Person> personList = new ArrayList<>();
                     int i = 0;
                     if (cursor != null) {
                         for (Person person : cursor) {
                             if (personList.size() < 1000) {
//                            log.info("----id:{},userName:{}", person.getId(), person.getUserName());
                                 personList.add(person);
                             } else if (personList.size() == 1000) {
                                 ++i;
                                 log.info("----{}、从cursor取数据达到1000条，开始处理数据", i);
                                 log.info("----处理数据中...");
                                 Thread.sleep(1000);//休眠1s模拟处理数据需要消耗的时间；
                                 log.info("----{}、从cursor中取出的1000条数据已经处理完毕", i);
                                 personList.clear();
                                 personList.add(person);
                             }
                             if (total == (cursor.getCurrentIndex() + 1)) {
                                 ++i;
                                 log.info("----{}、从cursor取数据达到1000条，开始处理数据", i);
                                 log.info("----处理数据中...");
                                 Thread.sleep(1000);//休眠1s模拟处理数据需要消耗的时间；
                                 log.info("----{}、从cursor中取出的1000条数据已经处理完毕", i);
                                 personList.clear();
                             }
                         }
                         if (cursor.isConsumed()) {
                             log.info("----查询sql匹配中的数据已经消费完毕！");
                         }
                     }
                     sqlSession.commit();
                     log.info("----提交事务");
                 }catch (Exception e){
                     e.printStackTrace();
                     sqlSession.rollback();
                 }
                 finally {
                     if (sqlSession != null) {
                         //全部数据读取并且做好其他业务操作之后，提交事务并关闭连接；
                         sqlSession.close();
                         log.info("----关闭sqlSession");  
                     }
                 }
                
            }
        }).start();
    }
}
```
<a name="c6UJP"></a>
## 应用场景
其实Mybatis的流式查询适用范围很有限，这里举个例子，假如有这样一个需求：有50万员工的一年的工资数据明细，需要输出一张公司支出工资的数据报表。<br />需求很简单，估计有人是这样想：这太简单了，查询出员工的工资数据明细，然后按照套上公式逐条计算出结果，然后汇总计算结果，插入到新的结果表里不就行了。事实上这件事绝对不简单：

- 50万的数据全部读取到jvm的内存里得占用多大空间？
- 这么多对象的垃圾回收又需要多久？
- 这么多数据计算是高频行为还是低步行为？
- 如果计算到某条员工的数据发生异常，已经计算好的数据要不要全部回滚？...

总之，直接取出50万数据来计算，风险肯定不小。那怎么办呢？<br />在实际的开发中，也经常遇到一些百十万，说大不大，说小不小的数据报表处理，主要设计思路通常就是数据切隔+异步，具体怎么做呢？结合上面的例子，是这样的：<br />1、按照月份、省份或者部门，对工资明细数据进行数据切隔分组；<br />2、把不同月份、省份、部门的工资数据包装成多线程任务，放到线程池中去执行；<br />3、根据切隔的多线程任务数量，定义一个同步工具类`CountDownLatch`；<br />4、根据同步工具类`CountDownLatch`，来判断所有的多线程任务是否全部执行完；等到所有的多线程任务全部执行完成后，再执行汇总的逻辑；<br />5、在多线程任务里，查询具体月份、省份的员工工资数据明细的时候，如果数据量还是不少，就可以使用Mybatis的流式查询，分批获取员工工资明细数据，进行当前批的计算、汇总，然后所有分批数据都计算完成后，再汇总所有分批数据；
<a name="FvQ8f"></a>
## 注意事项
Mybatis的流式查询的本意，是避免大量数据的查询而导致内存溢出，因此dao层查询返回的是一个迭代器（Cursor），可以每次从迭代器中取出一条查询结果，在实际业务开发过程中，即是根据实际的jvm内存大小，从迭代器中取出一定数量的数据后，再进行数据处理，待处理完之后，继续取出一定数据再处理，以此类推直到全部数据处理完，这样做的最大好处就是能够降低内存使用和垃圾回收器的负担，使数据处理的过程相对更加高效、可控，内存溢出的风险较小；<br />好处很明显，缺点也很就明显，处理的时间可能会变长，需要引入多线程异步操作，并且在迭代器遍历和数据处理的过程中，数据库连接不能断开，即当前sqlSession要保持持续打开状态，一量断开，数据读取就会中断，所以关于这块的处理，使用Mybatis原生的sqlSession进行手动查询、提交事务、回滚和关闭sqlSession最为稳妥、最简单。
