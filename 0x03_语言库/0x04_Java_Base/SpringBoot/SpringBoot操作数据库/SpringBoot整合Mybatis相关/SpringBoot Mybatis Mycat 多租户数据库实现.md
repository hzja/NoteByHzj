Java SpringBoot Mybatis Mycat
<a name="J40ed"></a>
## 1、Mycat
<a name="j1Mru"></a>
### 下载
```bash
wget http://dl.mycat.io/1.6.7.3/20190927161129/Mycat-server-1.6.7.3-release-20190927161129-linux.tar.gz
```
<a name="WeLal"></a>
### 配置
server.xml，Mycat服务器配置，默认端口8066
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mycat:server SYSTEM "server.dtd">
<mycat:server xmlns:mycat="http://io.mycat/">
    <system>
        <property name="useSqlStat">0</property>
        <property name="useGlobleTableCheck">0</property>
        <property name="sequnceHandlerType">2</property>
        <property name="handleDistributedTransactions">0</property>
        <property name="useOffHeapForMerge">1</property>
        <property name="memoryPageSize">1m</property>
        <property name="spillsFileBufferSize">1k</property>
        <property name="useStreamOutput">0</property>
        <property name="systemReserveMemorySize">384m</property>
        <property name="useZKSwitch">true</property>
    </system>

    <!--Mycat用户名-->
    <user name="root">
        <!--Mycat密码-->
        <property name="password">root</property>
        <!--Mycat数据库名-->
        <property name="schemas">mycat_test</property>
    </user>

</mycat:server>
```
schema.xml，Mycat和Mysql节点映射配置
```xml
<?xml version="1.0"?>
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">
<mycat:schema xmlns:mycat="http://io.mycat/">
    <!--Mycat数据库名-->
    <schema name="mycat_test" checkSQLschema="true" sqlMaxLimit="100">
        <!--Mycat表名，节点名称列表-->
        <table name="mycat_test_student" dataNode="dn1,dn2"/>
    </schema>
    <!--Mycat节点名称、节点地址、mysql数据库名-->
    <dataNode name="dn1" dataHost="localhost1" database="db01" />
    <dataNode name="dn2" dataHost="localhost1" database="db02" />

    <!--Mycat节点地址-->
    <dataHost name="localhost1" maxCon="1000" minCon="10" balance="0"
              writeType="0" dbType="mysql" dbDriver="native" switchType="1"  slaveThreshold="100">
        <heartbeat>select user()</heartbeat>
        <!-- mysql数据库服务器、账户、密码 -->
        <writeHost host="hostM1" url="192.168.1.71:3306" user="test"
                   password="test@1234">
        </writeHost>
    </dataHost>
</mycat:schema>
```
<a name="dhqng"></a>
## 2、Spring Boot
<a name="RU7JI"></a>
### 数据源配置
```
#mycat连接信息
spring.datasource.url=jdbc:mysql://localhost:8066/mycat_test?serverTimezone=GMT
spring.datasource.username=root
spring.datasource.password=root
spring.datasource.driver-class-name=com.mysql.jdbc.Driver

#mybatis拦截器配置
mybatis.config-location=classpath:mybatis.xml
```
<a name="vNX5w"></a>
### Mybatis
mybatis.xml插件配置
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">

<configuration>
    <settings>
        <!-- 打印查询语句 -->
        <setting name="logImpl" value="STDOUT_LOGGING" />
    </settings>

    <typeAliases>
        <typeAlias alias="TestPojo" type="xx.xx.TestPojo"/>
    </typeAliases>

        <!-- 拦截器插件，改写sql -->
    <plugins>
        <plugin interceptor="xx.interceptor.MyInterceptor">
        </plugin>
    </plugins>

</configuration>
```
<a name="PcN8Q"></a>
### 拦截器
```java
//拦截StatementHandler的prepare方法
@Intercepts(value = {
    @Signature(type = StatementHandler.class,
               method = "prepare",
               args = {Connection.class,Integer.class})})
public class MyInterceptor implements Interceptor {
    // 修改sql，添加前后缀
    private static final String preState="/*!mycat:datanode=";
    private static final String afterState="*/";

    @Override
    public Object intercept(Invocation invocation) throws Throwable {
        StatementHandler statementHandler=(StatementHandler)invocation.getTarget();
        MetaObject metaStatementHandler=SystemMetaObject.forObject(statementHandler);
        Object object=null;
        //分离代理对象链
        while(metaStatementHandler.hasGetter("h")){
            object=metaStatementHandler.getValue("h");
            metaStatementHandler=SystemMetaObject.forObject(object);
        }

        //获取sql
        String sql=(String)metaStatementHandler.getValue("delegate.boundSql.sql");

        //根据会话上下文，获取节点标识
        String node=(String) SessionUtil.getSession().getAttribute("appId");
        if(node!=null) {
            //重写sql，适配mycat
            sql = preState + node + afterState + sql;
        }

        System.out.println("sql is "+sql);
        metaStatementHandler.setValue("delegate.boundSql.sql",sql);
        Object result = invocation.proceed();
        System.out.println("Invocation.proceed()");
        return result;
    }

    // 返回当前拦截的对象(StatementHandler)的动态代理
    // 当拦截对象的方法被执行时, 动态代理中执行拦截器intercept方法.
    @Override
    public Object plugin(Object target) {
        return Plugin.wrap(target, this);
    }

    @Override
    public void setProperties(Properties properties) {
        String prop1 = properties.getProperty("prop1");
        String prop2 = properties.getProperty("prop2");
        System.out.println(prop1 + "------" + prop2);
    }
}
```
<a name="L0PfM"></a>
## 总结
以上为关键实现，主要过程如下：

- 搭建 Mycat 服务，设置提供的数据库信息
- 配置Mycat动态映射的两个节点，Mycat根据sql语句中的`/*!mycat:datanode=dn1*/`进行动态切换数据源后执行sql
- 配置 SpringBoot 的 Mycat 数据源连接
- 配置 Mybatis 的拦截器插件
- 配置 Mybatis 拦截器实现，根据上下文节点，改写 sql
<a name="yvPhY"></a>
## 注意

- 生产可采用Mycat集群，集群用ZK管理，以动态实例化数据源
