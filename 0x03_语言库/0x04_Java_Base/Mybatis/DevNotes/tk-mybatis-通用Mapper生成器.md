Mybatis tk.mybatis
<a name="uWIGj"></a>
### 通用 Mapper
通用Mapper方便的使用 Mybatis 进行单表的增删改查，是一款优秀开源产品。它使用拦截器来实现具体的执行 Sql，完全使用原生的 Mybatis 进行操作。<br />它提供了所有单表的基本增删改查方法，大大节省了书写基本 mapper.xml 的时间。尤其对于新工程和新表来说，极大的提高了项目构建效率。使用这个通用 Mapper 甚至能改变对 Mybatis 单表基础操作不方便的想法，使用它能简单的使用单表的增删改查，包含动态的增删改查。<br />同时，在代码结构合理的前提下，更换 RDBMS 也无须修改 sql，只需修改部分配置即可实现。
<a name="Jr7qu"></a>
### 通用Mapper的pom依赖
```xml
<dependency>
    <groupId>tk.mybatis</groupId>
    <artifactId>mapper</artifactId>
    <version>x.x.x</version>
</dependency>
```
<a name="eQa6Q"></a>
### 项目中集成使用
<a name="rJDM8"></a>
#### Java编码方式
```java
MapperHelper mapperHelper = new MapperHelper();
//特殊配置
Config config = new Config();//具体支持的参数看后面的文档
config.setXXX(XXX);//设置配置
mapperHelper.setConfig(config);// 注册自己项目中使用的通用Mapper接口，这里没有默认值，必须手动注册
mapperHelper.registerMapper(Mapper.class);
//配置完成后，执行下面的操作
mapperHelper.processConfiguration(session.getConfiguration());
```
<a name="mzBG7"></a>
#### Spring配置方式实现
```xml
<bean >
    <property name="basePackage" value="com.fcant.mybatis.mapper"/>
    <property name="properties">
        <value>mappers=tk.mybatis.mapper.common.Mapper</value>
    </property>
</bean>
```
:::danger
注意这里是 tk.mybatis.xxx, 和 MyBatis 的唯一区别就是 org. 改成了 tk.，方便修改和记忆。
:::
通用 Mapper 的各项属性通过 properties 属性进行配置，如果默认配置就是一行 `mappers=tk.mybatis.mapper.common.Mapper` 时，可以不写，就会变成：
```xml
<bean >
    <property name="basePackage" value="com.fcant.mybatis.mapper"/>
</bean>
```
<a name="K4EUq"></a>
### 继承通用 Mapper 接口（注意必须要加泛型）：
```java
@Repository
public interface MaterialDao extends Mapper<MaterialMeta>, 
InsertUseGeneratedKeysMapperr<MaterialMeta> {}
```
示例继承了Mapper和InsertUseGeneratedKeysMapper，则直接拥有了这2个接口的所有方法。<br />实体类的代码：
```java
@Table(name = "tb\_helpcenter\_material")
public class MaterialMeta {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String title;
    private String tags;
    private Long classificationId;
    private String platform;
    private String lecturerName;
    // Setters&Getters
}
```
<a name="iVi3k"></a>
### 实体类的规则
1. 表名默认使用类名, 驼峰转下划线 (只对大写字母进行处理), 如 UserInfo 默认对应的表名为 user_info。<br />2. 表名可以使用 @Table(name = "tableName") 进行指定, 对不符合第一条默认规则的可以通过这种方式指定表名.<br />3. 字段默认和 @Column 一样, 都会作为表字段, 表字段默认为 Java 对象的 Field 名字驼峰转下划线形式.<br />4. 可以使用 @Column(name = "fieldName") 指定不符合第 3 条规则的字段名<br />5. 使用 @Transient 注解可以忽略字段, 添加该注解的字段不会作为表字段使用.<br />6. 建议一定是有一个 @Id 注解作为主键的字段, 可以有多个 @Id 注解的字段作为联合主键.<br />7. 默认情况下, 实体类中如果不存在包含 @Id 注解的字段, 所有的字段都会作为主键字段进行使用 (这种效率极低).<br />8. 实体类可以继承使用, 可以参考测试代码中的 tk.mybatis.mapper.model.UserLogin2 类.<br />9. 由于基本类型, 如 int 作为实体类字段时会有默认值 0, 而且无法消除, 所以实体类中建议不要使用基本类型.<br />10.@NameStyle 注解，用来配置对象名 / 字段和表名 / 字段之间的转换方式，该注解优先于全局配置 style，可选值：<br />另外，建议实体类的所有 Field 全部使用装箱类，不要使用基本类型。<br />id 字段上的 @GeneratedValue 注解用来表示该表使用的主键策略类型。
<a name="tob2a"></a>
### 关于主键策略
主键策略主要用于 insert 场景。通常情况下，可以不用设置表对象的主键策略。不设置时，默认会使用 JDBC 的 getGeneratedKeys 方法来取出由数据库内部生成的主键。<br />也可以以 `( @GeneratedValue(generator = "")`  的形式来指定主键策略命令。如：`@GeneratedValue(strategy = GenerationType.IDENTITY`<br />`,generator = "select last_insert_id()")` 等。<br />同时，可以根据实际需要，在全局配置中指定主键策略的执行 ORDER。
<a name="09CfH"></a>
### Service层的调用使用
在Service层直接调用即可
```java
MaterialMeta materialMeta = ReflectUtil.convertObj(MaterialMeta.class, material, false);
materialMeta.setPlatform(platformStr);
materialDao.insertSelective(materialMeta);
```
```java
Example example = new Example(MaterialMeta.class);
example.createCriteria().andEqualTo("classificationId", material.getClassificationId()).andEqualTo("deleted",false);

example.orderBy("sort").desc();
PageHelper.startPage(1, 1);
List<MaterialMeta> materialMetas = materialDao.selectByExample(example);
```
```java
materialMeta.setStatus(MaterialStatus.online);
materialMeta.setPublishDate(new Date());
materialDao.updateByPrimaryKeySelective(materialMeta);
```
:::danger
基本的SQL不需要手写了，特殊的SQL还是需要自定义
:::
<a name="pSzei"></a>
### tk.mybatis配合分页插件PageHelper的使用
PageHelper的Github的仓库地址：[https://github.com/pagehelper/Mybatis-PageHelper/blob/master/README_zh.md](https://github.com/pagehelper/Mybatis-PageHelper/blob/master/README_zh.md)
<a name="a64g3"></a>
#### A.引入PageHelper相关依赖
```xml
<dependency>
    <groupId>com.github.pagehelper</groupId>
    <artifactId>pagehelper</artifactId>
    <version>最新版本</version>
</dependency>
```
<a name="ZjklJ"></a>
#### B.直接在项目中集成使用即可
示例代码
```java
PageHelper.offsetPage(offset, limit);
Page<MaterialMeta> metas = (Page<MaterialMeta>) materialDao.selectByExample(example);
```
