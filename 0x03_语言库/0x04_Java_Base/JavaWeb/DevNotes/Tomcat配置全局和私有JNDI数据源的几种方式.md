JavaTomcatJNDI<br />参考官方文档：[https://tomcat.apache.org/tomcat-8.5-doc/jndi-resources-howto.html](https://tomcat.apache.org/tomcat-8.5-doc/jndi-resources-howto.html)
<a name="PITlI"></a>
## JNDI数据源配置
<a name="Ejgml"></a>
### 全局
<a name="B4tnK"></a>
#### 1、修改Tomcat的context.xml的`<Context>`标签
在`<Context>`标签内加入如下内容(以配置mysql数据源为例)：
```xml
<Resource name="jdbc/mysql"     //名称可以自定义
    auth="Container" 
    type="javax.sql.DataSource" 
    username=""   //改成自己的数据库用户名和密码
    password="" 
    maxIdle="30" 
    maxWait="10000" 
    maxActive="100"
    driverClassName="com.mysql.jdbc.Driver"
    url="jdbc:mysql://localhost:3306/数据库名" />
```
配置好后重启Tomcat。 可以选择在项目中引入MySQL驱动，或者直接将驱动jar包扔进Tomcat安装目录中的lib文件夹下。<br />最后在项目的web.xml中配置这段代码(其实可有可无，但最好加上，后文会解释原因)：
```xml
<resource-ref>
  <res-ref-name>jdbc/mysql</res-ref-name> //和上面的name属性值相同
  <res-type>javax.sql.DataSource</res-type>
  <res-auth>Container</res-auth>
  <res-sharing-scope>Shareable</res-sharing-scope>
</resource-ref>
```
使用下面这段代码测试(注意，不能在main中测试，否则会报错。在jsp页面中或者servlet中可以，需要web环境)：
```java
Context initial = new InitialContext(); //得到上下文引用
DataSource datasource = (DataSource) initial.lookup("java:comp/env/jdbc/mysql");
```
<a name="qZOEg"></a>
#### 2、修改Tomcat的server.xml的`<GlobalNamingResources>`
在`<GlobalNamingResources>`标签中加入下面代码：
```xml
<Resource name="globalResourceTest" 
  auth="Container" 
  type="javax.sql.DataSource" 
  username=""
  password="" 
  maxIdle="30" 
  maxWait="10000" 
  maxActive="100"
  driverClassName="com.mysql.jdbc.Driver"
  url="jdbc:mysql://localhost:3306/数据库名" /> 
```
打开`$tomcat_home\conf\context.xml`，在`<Context>`标签中加入下面代码：
```xml
<ResourceLink name="jdbc/mysql" global="globalResource" type="javax.sql.DataSource"/>
```
注意：`<Resource>`中的name属性要和`<ResourceLink>`中的global属性对应才能引用到，`<ResourceLink>`中的name才是数据源的名字，要通过这个名字找到数据源。<br />同样在web.xml中加入下面代码：
```xml
<resource-ref>
    <res-ref-name>jdbc/mysql</res-ref-name> //和上面的name属性值相同
    <res-type>javax.sql.DataSource</res-type>
    <res-auth>Container</res-auth>
    <res-sharing-scope>Shareable</res-sharing-scope>
</resource-ref>
```
<a name="O3AJa"></a>
### 私有
<a name="wqpxt"></a>
#### 1、修改Tomcat的server.xml的`<Host>`
找到`<Host appBase="webapps" autoDeploy="true" name="localhost" unpackWARs="true"></Host>`，在其中加入以下内容：
```xml
<Context path="/xxxx" docBase="项目的绝对路径" reloadable="true">
    <Resource name="jdbc/mysql" 
        auth="Container" 
        type="javax.sql.DataSource" 
        username="" 
        password="" 
        maxIdle="30" 
        maxWait="10000" 
        maxActive="100"
        driverClassName="com.mysql.jdbc.Driver"
        url="jdbc:mysql://127.0.0.1:3306/数据库名"/>
</Context>
```
注意：`<Context>`的path属性必须和项目访问路径一致且开头必须加“/”。 什么叫项目访问路径？如果用Eclipse+Tomcat启动项目，那path属性为“/项目名”。 若使用IDEA+Tomcat启动项目，这里的path必须等于下图圈起来的地方：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681389281072-4ad41f85-00aa-4b62-915c-a072212584d5.png#averageHue=%23f7f7f6&clientId=uad99e3ff-e09d-4&from=paste&height=810&id=u5ce3e938&originHeight=2024&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=119326&status=done&style=shadow&taskId=u10d74268-5e39-41a2-ba1e-d4e4e2cdc0e&title=&width=1042)<br />若在IDE中启动Tomcat，其实docBase这个属性设不设置都可以，设置了乱填也不影响。但为什么还是写上了，因为当使用cmd启动Tomcat时，若不设置该属性启动时会抛出“NamingException: No naming context bound to this class loader”。所以必须将docBase设置为项目的绝对路径。<br />然后在web.xml中加入：
```xml
<resource-ref>
    <res-ref-name>jdbc/mysql</res-ref-name> //和上面的name属性值相同
    <res-type>javax.sql.DataSource</res-type>
    <res-auth>Container</res-auth>
    <res-sharing-scope>Shareable</res-sharing-scope>
</resource-ref>
```
<a name="SX0W9"></a>
#### 2、在项目的META-INF文件夹下创建context.xml
内容如下:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Context reloadable="true">
    <Resource name="jdbc/mysql" 
        auth="Container" 
        type="javax.sql.DataSource" 
        username="" 
        password="" 
        maxIdle="30" 
        maxWait="10000" 
        maxActive="100"
        driverClassName="com.mysql.jdbc.Driver"
        url="jdbc:mysql://127.0.0.1:3306/项目名"/>
</Context>
```
<a name="iKvXD"></a>
### 同时存在多种配置方式且同名，会获取哪一个
首先，全局配置大于私有配置，也就是说若全局和私有同时存在，优先获取全局配置的数据源<br />全局：<br />在context.xml中，`<Resource>`和`<ResourceLink>`(引用server.xml中`<GlobalNamingResources>` )同时存在，哪个标签在前获取到哪一个，即按顺序获取。<br />私有：

1. 若只存在`<Host>`标签中的配置，不存在META-INF中的context.xml，则优先获取`<Host>`中配置的第一个数据源。
2. 若两种私有配置方式都存在，会获取META-INF中context.xml配置的数据源。
<a name="svQSf"></a>
### 为什么web.xml中的配置可有可无，却要加上呢？
详情见Tomcat的docs：jndi-resources-howto.html<br />其中有这么一句话：
> If a resource has been defined in a <Context> element it is not necessary for that resource to be defined in /WEB-INF/web.xml. However, it is recommended to keep the entry in /WEB-INF/web.xml to document the resource requirements for the web application.

也就是说，在web.xml加入的那段代码仅起到说明作用，说明这个Web应用引用了哪些资源！
<a name="ZOWgq"></a>
## 通过JNDI配置事务
在Tomcat的context.xml文件中添加以下代码
```xml
<Resource name="UserTransaction" auth="Container"
          type="javax.transaction.UserTransaction"
          factory="org.apache.naming.factory.TransactionFactory"
          pathname="java:comp/UserTransaction"/>
```
在web.xml文件中添加以下代码配置资源引用
```xml
<resource-ref>
  <description>User Transaction</description>
  <res-ref-name>java:comp/UserTransaction</res-ref-name>
  <res-type>javax.transaction.UserTransaction</res-type>
  <res-auth>Container</res-auth>
</resource-ref>
```
在Tomcat环境下，可以通过以下代码来获取事务：
```java
Context ctx = new InitialContext();
return (UserTransaction)ctx.lookup("java:comp/UserTransaction");
```
请注意，此代码需要在Tomcat的servlet或过滤器中使用，因为它需要访问`java:comp/UserTransaction`上下文。
<a name="zpjys"></a>
## Tomcat中使用JTA手动管理事务且不使用JNDI的操作步骤

1. 添加JTA实现库依赖，例如Atomikos或Bitronix。
2. 创建一个`UserTransaction`对象和`TransactionManager`对象。
3. 在事务开始时调用`UserTransaction`的`begin()`方法，然后将`TransactionManager`的当前事务设置为这个事务。
4. 在事务结束时，调用`UserTransaction`的`commit()`方法或`rollback()`方法，并将`TransactionManager`的当前事务设置为null。
5. 在应用程序中，使用`TransactionManager`来获取当前事务或创建新的事务。

以下是一个简单的示例代码：
```java
import javax.transaction.UserTransaction;
import javax.transaction.TransactionManager;
import com.atomikos.icatch.jta.UserTransactionManager;
import com.atomikos.icatch.jta.UserTransactionImp;
import com.atomikos.jdbc.AtomikosDataSourceBean;

public class MyTransactionManager {
    private static UserTransaction userTransaction;
    private static TransactionManager transactionManager;

    static {
        userTransaction = new UserTransactionImp();
        transactionManager = new UserTransactionManager();
    }

    public static void beginTransaction() {
        try {
            userTransaction.begin();
            transactionManager.begin();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void commitTransaction() {
        try {
            userTransaction.commit();
            transactionManager.commit();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void rollbackTransaction() {
        try {
            userTransaction.rollback();
            transactionManager.rollback();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
```
在业务逻辑中，可以使用以下代码来开始、提交或回滚事务：
```java
MyTransactionManager.beginTransaction();
try {
    // 执行数据库操作
    MyTransactionManager.commitTransaction();
} catch (Exception e) {
    MyTransactionManager.rollbackTransaction();
}
```
