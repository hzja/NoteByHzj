Java Mybatis
<a name="QpZVR"></a>
## 目录
![MyBatis 插件原理与实战](https://cdn.nlark.com/yuque/0/2022/png/396745/1650850810152-a7aafc9c-066b-43cf-ba97-b18e795006ff.png#clientId=u23ed4c68-57e6-4&from=ui&id=uf3835554&originHeight=510&originWidth=789&originalType=binary&ratio=1&rotation=0&showTitle=true&size=1209713&status=done&style=shadow&taskId=ue97ae41b-40d2-4693-85f9-8bc9ffcd640&title=MyBatis%20%E6%8F%92%E4%BB%B6%E5%8E%9F%E7%90%86%E4%B8%8E%E5%AE%9E%E6%88%98 "MyBatis 插件原理与实战")
<a name="Sv0z3"></a>
## 什么是插件？
插件就是在具体的**执行流程**插一脚（触发点、拦截器）来实现具体的功能。<br />一般插件会对**执行流程**中的上下文有依赖，抽象的说，也可以把MyBatis看作是JDBC的**插件**，只是功能越来来多，越来越强大，最后给了他一个新名字，叫做**框架**。<br />不管怎样，JDBC的那一套还是不会变的，只是做了抽象、封装、归类等。<br />要想知道插件的原理，首先就要对它的执行流程有一定的把控。
<a name="EKmsq"></a>
## 执行流程
前边说到MyBatis是对JDBC的抽象、封装。<br />首先来回顾一下JDBC的执行流程。
<a name="eRdhW"></a>
### JDBC执行流程

1. 注册驱动；
2. 获取Connection连接；
3. 执行预编译；
4. 执行SQL；
5. 封装结果集；
6. 释放资源；

**给段伪代码通透理解下：**
```java
// 注册驱动
Class.forName("com.mysql.jdbc.Driver");
// 获取链接
Connection con = DriverManager.getConnection(url, username, password);
// 执行预编译
Statement stmt = con.createStatement();
// 执行SQL
ResultSet rs = stmt.executeQuery("SELECT * FROM ...");
// 封装结果
while (rs.next()) {
	String name = rs.getString("name");
	String pass = rs.getString(1); // 此方法比较高效
}
// 释放资源
if (rs != null) {  // 关闭记录集
	
}
if (stmt != null) {  // 关闭声明
	
}
if (conn != null) { // 关闭连接对象
	
}
```
上边的代码是不是很熟悉，相信每个入门写Java代码的人，都写过这段代码。<br />紧接着，继续来了解MyBatis的执行流程。
<a name="KYiEC"></a>
### MyBatis执行流程

1. 读取MyBatis的核心配置文件；
2. 加载映射文件；
3. 构造会话工厂获取`SqlSessionFactory`；
4. 创建会话对象`SqlSession`；
5. `Executor`执行器；
6. `MappedStatement`对象；
7. 输入参数映射；
8. 封装结果集；

上边的文字可能不太好理解，这里也画一幅执行流程图，来方便理解。<br />![MyBatis执行流程](https://cdn.nlark.com/yuque/0/2022/png/396745/1650852327588-efbcd8c1-0930-421d-baf0-4c25803d36ea.png#clientId=u23ed4c68-57e6-4&from=ui&id=hgEnD&originHeight=1437&originWidth=1786&originalType=binary&ratio=1&rotation=0&showTitle=true&size=7713451&status=done&style=shadow&taskId=u83488f1f-7a4d-4ca1-bc14-da6cd3a9cde&title=MyBatis%E6%89%A7%E8%A1%8C%E6%B5%81%E7%A8%8B "MyBatis执行流程")<br />有没有觉得MyBatis的执行流程和JDBC的执行流程主干也差不多，只是在主干过程中，把一些配置（mybatis-config.xml）、常用的定义文件单独抽离出来（mapper.xml）和一些附带扩展性的**拦截点**抽离了出来。<br />下面着重讲一讲**拦截点**，因为插件就是基于**拦截点**来做的扩展。
<a name="wqECW"></a>
## 拦截点
结合上边的MyBatis执行流程，看下图的各个拦截点：<br />![MyBatis拦截点](https://cdn.nlark.com/yuque/0/2022/png/396745/1650852327068-6b7532bb-8c10-4583-8c35-3c125feca4f4.png#clientId=u23ed4c68-57e6-4&from=ui&id=tBbsS&originHeight=1459&originWidth=1285&originalType=binary&ratio=1&rotation=0&showTitle=true&size=5635104&status=done&style=shadow&taskId=ua131cffa-810e-4dd9-89b7-1c7f6b1a61f&title=MyBatis%E6%8B%A6%E6%88%AA%E7%82%B9 "MyBatis拦截点")<br />文字描述，MyBatis允许使用插件来**拦截的方法调用**包括：

1. `Executor`：拦截执行器的方法 `(**update, query, flushStatements, commit, rollback,getTransaction, close, isClosed**)`，Mybatis的内部执行器，它负责调用`StatementHandler`操作数据库，并把结果集通过 ResultSetHandler进行自动映射，另外，他还处理了二级缓存的操作。从这里可以看出，也是可以通过插件来实现自定义的二级缓存的；
2. `ParameterHandler`：拦截参数的处理 `(**getParameterObject, setParameters**)` ，Mybatis直接和数据库执行sql脚本的对象。另外它也实现了Mybatis的一级缓存。这里可以使用插件来实现对一级缓存的操作(禁用等等)；
3. `ResultSetHandler`：拦截结果集的处理 `(**handleResultSets, handleOutputParameters**)` ，Mybatis实现Sql入参设置的对象。插件可以改变Sql的参数默认设置；
4. `StatementHandler`：拦截Sql语法构建的处理 `(**prepare, parameterize, batch, update, query**)` ，Mybatis把`ResultSet`集合映射成POJO的接口对象。可以定义插件对Mybatis的结果集自动映射进行修改。
<a name="nhWPs"></a>
## 拦截器为什么能够拦截
`org.apache.ibatis.session.Configuration`是在MyBatis初始化配置的类。<br />其中的`newParameterHandler`、`newResultSetHandler`、`newStatementHandler`、`newExecutor`这几个方法在创建指定的对象（`newParameterHandler`创建`ParameterHandler`、`newResultSetHandler`创建`ResultSetHandler`、`newStatementHandler`创建`StatementHandler`、`newExecutor`创建Executor）对象的时候，都会调用一个统一的方法：<br />![创建对象](https://cdn.nlark.com/yuque/0/2022/png/396745/1650850810183-6a1c9ff2-964f-4ad6-8f27-bdbdfc7d72f9.png#clientId=u23ed4c68-57e6-4&from=ui&id=uL7dL&originHeight=392&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=1272606&status=done&style=shadow&taskId=ucb8d4523-3fa4-44a8-a823-bb38906e310&title=%E5%88%9B%E5%BB%BA%E5%AF%B9%E8%B1%A1 "创建对象")<br />这4个方法实例化了对应的对象之后，都会调用`interceptorChain`的`pluginAll`方法，那么下面再来看`pluginAll`干了什么。<br />包路径：`org.apache.ibatis.plugin.InterceptorChain`
```java
public Object pluginAll(Object target) {
	Interceptor interceptor;
	for(Iterator var2 = this.interceptors.iterator(); var2.hasNext(); target = interceptor.plugin(target)) {
		interceptor = (Interceptor)var2.next();
	}
	
	return target;
}
```
原来这个`pluginAll`方法就是遍历所有的拦截器，然后顺序执行插件的plugin方法，一层一层返回原对象(`Executor`/`ParameterHandler`/`ResultSetHander`/`StatementHandler`)的代理对象。当调用四大接口对象的方法时候，实际上是调用代理对象的响应方法，代理对象又会调用四大接口对象的实例。<br />这里看到所有的拦截器`Interceptor`，其实它和平常写代码一样，也是多态的利用，存在一个拦截器`Interceptor`接口，**在实现插件的时候，也实现这个接口，就会被调用。**
<a name="P7u8C"></a>
## `Interceptor`接口
包路径：org.apache.ibatis.plugin
```java
public interface Interceptor {
    Object intercept(Invocation var1) throws Throwable;

    default Object plugin(Object target) {
        return Plugin.wrap(target, this);
    }

    default void setProperties(Properties properties) {
    }
}
```
这个接口只声明了三个方法：

1. `setProperties`方法是在Mybatis进行配置插件的时候可以配置自定义相关属性，即：接口实现对象的参数配置；
2. `plugin`方法是插件用于封装目标对象的，通过该方法可以返回目标对象本身，也可以返回一个它的代理，可以决定是否要进行拦截进而决定要返回一个什么样的目标对象，官方提供了示例：`return Plugin.wrap(target, this)`；
3. `intercept`方法就是要进行拦截的时候要执行的方法；
<a name="xLk72"></a>
## 编写简单的MyBatis插件
**注：MyBatis默认没有一个拦截器接口的实现类，开发者可以实现符合自己需求的拦截器**
```java
@Intercepts({@Signature(type= Executor.class, method = "update", args = {MappedStatement.class,Object.class})})
public class ExamplePlugin implements Interceptor {
    public Object intercept(Invocation invocation) throws Throwable {
        return invocation.proceed();
    }
    public Object plugin(Object target) {
        return Plugin.wrap(target, this);
    }
    public void setProperties(Properties properties) {
    }
}
```
**全局xml配置（实例化bean）**
```xml
<plugins>
	<plugin interceptor="org.format.mybatis.cache.interceptor.ExamplePlugin"></plugin>
</plugins>
```
**这个拦截器拦截**`**Executor**`**接口的**`**update**`**方法（其实也就是SqlSession的新增，删除，修改操作），所有执行**`**executor**`**的**`**update**`**方法都会被该拦截器拦截到，就在里边做相对应的逻辑处理就可以了。**
<a name="UJoc1"></a>
## 总结
分析了什么是插件首先需要了解执行流程，然后回顾JDBC流程来推导出MyBatis的执行流程，通过初始化的`org.apache.ibatis.session.Configuration`为切入点，跟踪到`interceptorChain`的`pluginAll`方法；最后通过一个简单的插件来实际操作了一波。
