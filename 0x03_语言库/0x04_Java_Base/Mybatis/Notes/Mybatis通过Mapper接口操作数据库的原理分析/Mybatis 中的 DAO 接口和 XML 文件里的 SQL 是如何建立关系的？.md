JavaMybatis
<a name="P4L8Z"></a>
## 一、解析XML
首先，Mybatis 在初始化 `SqlSessionFactoryBean` 时，会找到 `mapperLocations` 配置的路径下中所有的XML文件并进行解析，这里重点关注两部分：
<a name="EO1wR"></a>
### **1、创建**`**SqlSource**`
Mybatis会把每个SQL标签封装成`SqlSource`对象，然后根据SQL语句的不同，又分为动态SQL和静态SQL。其中，静态SQL包含一段String类型的sql语句；而动态SQL则是由一个个`SqlNode`组成。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660787009621-1ea2764b-415f-4960-8ad2-ead3ab9e22d2.jpeg)<br />假如有这样一个SQL：
```xml
<select id="getUserById" resultType="user">
    select * from user 
    <where>
        <if test="uid!=null">
            and uid=#{uid}
        </if>
    </where>
</select>    
```
它对应的`SqlSource`对象看起来应该是这样的：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660787501263-a68e5439-2f48-4956-9422-776311f02a39.jpeg)
<a name="KJSul"></a>
### **2、创建**`**MappedStatement**`
接下来，Mybatis会为XML中的每个SQL标签都生成一个`MappedStatement`对象，这里面有两个属性很重要：

- **id：全限定类名+方法名组成的ID**
- **sqlSource：当前SQL标签对应的**`**SqlSource**`**对象**

创建完的 `MappedStatement` 对象会被添加到 `Configuration` 中，`Configuration`对象就是Mybatis中的大管家，基本所有的配置信息都维护在这里。当把所有的XML都解析完成之后，`Configuration`就包含了所有的SQL信息。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660787726581-06826dd0-48a9-4b76-9931-41f6d9f3bc18.jpeg)<br />到目前为止，XML就解析完成了，当执行Mybatis方法的时候，就可以通过 “全限定类名+方法名” 找到 `MappedStatement` 对象，然后解析里面的SQL内容并进行执行即可。
<a name="bVtL9"></a>
## 二、Dao接口代理
但是Dao接口并没有具体的实现类，那么在被调用时，最终又是怎样找到SQL语句的呢？<br />首先，在Spring配置文件中，一般会这样配置：
```xml
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
  <property name="basePackage" value="com.viewscenes.netsupervisor.dao" />
  <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"></property>
</bean>
```
或者项目是基于SpringBoot的，那么肯定也见过这种：`@MapperScan("com.xxx.dao")` ，<br />它们的作用是一样的，就是将包路径下的所有类注册到 Spring Bean 中，并将它们的beanClass设置为 `MapperFactoryBean`，`MapperFactoryBean` 实现了 `FactoryBean` 接口，俗称工厂Bean。那么，当通过 `@Autowired` 注入这个Dao接口时，返回的对象就是 `MapperFactoryBean` 这个工厂Bean中的 `getObject()` 方法对象。<br />那么，这个方法干了些什么呢？简单来说，它就是通过JDK动态代理，返回了一个Dao接口的代理对象 `MapperProxy`，当通过 `@Autowired` 注入Dao接口时，注入的就是这个代理对象，调用 Dao接口中的方法时，则会调用到 `MapperProxy` 对象的`invoke()`方法。<br />那么，目前为止，通过Dao接口也有了代理实现，所以就可以执行到它里面的方法了。
<a name="ZhLOT"></a>
## 三、执行
如上所述，当调用Dao接口方法的时候，实际调用到代理对象的`invoke()`方法。在这里，实际上调用的就是SqlSession里面的东西了。
```java
public class DefaultSqlSession implements SqlSession {

    public <E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds) {
        try {
            MappedStatement ms = configuration.getMappedStatement(statement);
            return executor.query(ms, wrapCollection(parameter), rowBounds, Executor.NO_RESULT_HANDLER);
        }
    }
}
```
看到以上代码，说明思考的是正确的。它就是通过statement（全限定类型+方法名）拿到`MappedStatement` 对象，然后通过执行器`Executor`去执行具体SQL并返回。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660787943948-17be1942-48fb-4cd1-b678-00e4dc908ad0.jpeg)
<a name="uFWw3"></a>
## 四、总结
1、针对Mybatis中的Dao接口和XML文件里的SQL是如何建立关系的问题，主要可以归纳为下面几点小点：

- `**SqlSource**`**以及动态标签SqlNode**
- `**MappedStatement**`**对象**
- **Spring 工厂Bean 以及动态代理**
- `**SqlSession**`**以及执行器**

2、针对有两个XML文件和这个Dao建立关系是否会冲突的问题：不管有几个XML和Dao建立关系，只要保证namespace+id唯一即可。
