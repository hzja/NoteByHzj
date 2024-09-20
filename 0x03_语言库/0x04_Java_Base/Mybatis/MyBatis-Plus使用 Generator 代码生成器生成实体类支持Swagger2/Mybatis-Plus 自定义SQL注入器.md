JavaMybatis-Plus
<a name="GgbTb"></a>
## 一、什么是SQL注入器
在使用Mybatis-Plus时，dao层都会去继承BaseMapper接口，这样就可以用BaseMapper接口所有的方法，BaseMapper中每一个方法其实就是一个SQL注入器<br />在Mybatis-Plus的核心(core)包下，提供的默认可注入方法有这些：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1685668289489-857de689-5ad1-47f6-b8b5-a79c02a948d0.jpeg#averageHue=%23615d54&clientId=u9e5255ff-518f-4&from=paste&id=u9fccd13f&originHeight=643&originWidth=399&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf40ec258-cd06-43e1-9254-49c1ad73a75&title=)<br />那如果想自定义SQL注入器呢，该如何去做？<br />比如在Mybatis-Plus中调用updateById方法进行数据更新默认情况下是不能更新空值字段的。<br />而在实际开发过程中，往往会遇到需要将字段值更新为空值的情况。<br />那如何让Mybatis-Plus支持空值更新呢？<br />如果仅是想实现支持更新空值字段并不需要自定义SQL注入器，因为Mybatis-Plus提供了几个扩展SQL注入器。
<a name="qcKo9"></a>
## 二、内置扩展SQL注入器有哪些？
<a name="cCAnA"></a>
### 1、自带扩展SQL注入器
Mybatis-Plus 扩展SQL注入器在扩展包下，提供了可扩展的可注入方法：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1685668289484-176d382d-962f-402a-961d-d25d2b9f18a6.jpeg#averageHue=%23636057&clientId=u9e5255ff-518f-4&from=paste&id=u3d6ac6b1&originHeight=269&originWidth=395&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5be4d32-d72e-4ea2-8997-23efd4fe744&title=)<br />`AlwaysUpdateSomeColumnById`：根据id更新字段(全量更新不忽略null字段)，updateById默认会自动忽略实体中null值字段。<br />`InsertBatchSomeColumn`：真实批量插入，saveBatch其实是伪批量插入。<br />`LogicDeleteBatchByIds`：逻辑删除增加填充功能，比如删除的时候填充更新时间、更新人。<br />`Upsert `: 插入一条数据（选择字段插入）。
<a name="xFjOY"></a>
### 2、SQL注入器全局配置
```java
@Component
public class MySqlInjector extends DefaultSqlInjector {

    @Override
    public List<AbstractMethod> getMethodList(Class<?> mapperClass, TableInfo tableInfo) {
        List<AbstractMethod> methodList = super.getMethodList(mapperClass, tableInfo);
        /**
         * 把两个扩展内置扩展SQL注入器注入
         */
        methodList.add(new InsertBatchSomeColumn(i -> i.getFieldFill() != FieldFill.UPDATE));
        methodList.add(new AlwaysUpdateSomeColumnById(i -> i.getFieldFill() != FieldFill.INSERT));
        return methodList;
    }
}
```
<a name="Knis3"></a>
### 3、自定义Mapper
```java
public interface MyBaseMapper<T> extends BaseMapper<T> {

    /**
     * 全字段更新，不会忽略null值
     *
     * @param entity 实体对象
     */
    int alwaysUpdateSomeColumnById(@Param("et") T entity);

    /**
     * 全量插入,等价于insert
     * 
     * @param entityList 实体集合
     */
    int insertBatchSomeColumn(List<T> entityList);
}
```
<a name="hiB9q"></a>
## 三、扩展SQL注入器示例测试
<a name="TgtcP"></a>
### 1、用户表
```sql
CREATE TABLE `user` (
  `id` int unsigned  AUTO_INCREMENT COMMENT '主键',
  `username` varchar(128)  COMMENT '用户名',
  `phone` varchar(32)  COMMENT '手机号',
  `sex` char(1)  COMMENT '性别',
  `create_time` datetime  COMMENT '创建时间',
  `update_time` datetime  COMMENT '更新时间',
  `deleted` tinyint DEFAULT '0' COMMENT '1、删除 0、未删除',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1
```
<a name="NyyU3"></a>
### 2、创建对应实体
```java
@Data
@Accessors(chain = true)
@TableName("user")
public class UserDO implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.AUTO)
    private Integer id;
    /**
     * 用户名
     */
    @TableField("username")
    private String username;
    /**
     * 手机号
     */
    @TableField("phone")
    private String phone;
    /**
     * 性别
     */
    @TableField("sex")
    private String sex;
    /**
     * 创建时间
     */
    @TableField(value = "create_time",fill = FieldFill.INSERT)
    private LocalDateTime createTime;
    /**
     * 更新时间
     */
    @TableField(value = "update_time",fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updateTime;
    /**
     * 1、删除 0、未删除
     */
    @TableField(value = "deleted",fill = FieldFill.INSERT)
    private Integer deleted;
}
```
其它有关代码这里就不粘贴了，具体看项目源码。<br />自定义的Mapper不再继承BaseMapper而是继承MyBaseMapper
```java
/**
 *  通用mapper接口，以后创建其他mapper接口时，不再继承BaseMapper，而是继承MyBaseMapper
 */
@Mapper
public interface UserMapper extends MyBaseMapper<UserDO> {

}
```
<a name="yRGxl"></a>
### 3、测试代码
```java
@SpringBootTest
@RunWith(SpringRunner.class)
@ComponentScan("com.jincou.mybatisplus.dao")
public class SqlInjectorTest  {

    @Autowired
    private UserMapper mapper;

    @Test
    public void alwaysUpdateSomeColumnById() {
        UserDO user = new UserDO();
        user.setUsername("小小");
        user.setPhone(null);
        user.setSex("女");
        user.setId(1);
        mapper.alwaysUpdateSomeColumnById(user);
    }

    @Test
    public void insertBatchSomeColumn() {
        UserDO user = new UserDO();
        user.setUsername("zhangsan");
        user.setPhone("13811111111");
        user.setSex("女");

        UserDO user1 = new UserDO();
        user1.setUsername("lisi");
        user1.setPhone("13822222222");
        user1.setSex("男");

        ArrayList<UserDO> userDOS = Lists.newArrayList(user, user1);
        mapper.insertBatchSomeColumn(userDOS);
    }
}
```
`alwaysUpdateSomeColumnById`方法运行结果<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1685668289539-0a4e56e4-a776-4287-ba2f-f274135629a4.jpeg#averageHue=%2350514e&clientId=u9e5255ff-518f-4&from=paste&id=ud9bac02f&originHeight=51&originWidth=710&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5a33b6d0-1fb5-4e04-a3e5-533a6da14c9&title=)<br />`insertBatchSomeColumn`方法运行结果<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1685668289531-487617d8-7135-4162-9f02-f3cb6631aa1e.jpeg#averageHue=%23525450&clientId=u9e5255ff-518f-4&from=paste&id=uf0e6aade&originHeight=77&originWidth=868&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua40ada30-198e-477a-969b-78a78bd64d0&title=)<br />成功！
<a name="yfyjN"></a>
## 四、如何自定义SQL注入器？
在实际开发过程中，当Mybatis-Plus自带的一些SQL注入器不满足条件时，就需要自定义SQL注入器，整个流程也非常简单<br />这里以一个很简单的`findAll`方法为例进行学习。
<a name="p1lsI"></a>
### 1、在MyBaseMapper中添加`findAll`方法
```java
public interface MyBaseMapper<T> extends BaseMapper<T> {

    /**
     *  查询所有用户
     */
    List<T> findAll();
}
```
<a name="uuYOw"></a>
### 2、编写FindAll SQL注入器
```java
public class FindAll extends AbstractMethod {

    public FindAll() {
        super("findAll");
    }


    public FindAll(String methodName) {
        super(methodName);
    }

    @Override
    public MappedStatement injectMappedStatement(Class<?> mapperClass, Class<?> modelClass, TableInfo tableInfo) {
        /* 执行 SQL ，动态 SQL 参考类 SqlMethod */
        String sql = "select *  from " + tableInfo.getTableName();
        SqlSource sqlSource = languageDriver.createSqlSource(configuration, sql, modelClass);
        return this.addSelectMappedStatementForTable(mapperClass, sqlSource, tableInfo);
    }
}
```
<a name="kt0jD"></a>
### 3、注册到Spring容器
```java
@Component
public class MySqlInjector extends DefaultSqlInjector {

    @Override
    public List<AbstractMethod> getMethodList(Class<?> mapperClass, TableInfo tableInfo) {
        List<AbstractMethod> methodList = super.getMethodList(mapperClass, tableInfo);
        /**
         * 自定义SQL注入器注入
         */
        methodList.add(new FindAll());
        return methodList;
    }
}
```
<a name="MJ2SZ"></a>
### 4、测试
```java
@Test
public void findAll() {
    List<UserDO> userDOS = mapper.findAll();
}
```
运行结果<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1685668289524-eef3edc7-4290-4705-848c-b7da4cfc6641.jpeg#averageHue=%23535350&clientId=u9e5255ff-518f-4&from=paste&id=u06943632&originHeight=167&originWidth=637&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub0157380-0ca7-4a83-846a-9b2613bdc92&title=)
