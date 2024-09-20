JavaMybatis
<a name="UWPof"></a>
## 一、需求背景
出于安全合规的考虑，需要对明文存储在数据库中的部分字段进行加密，防止未经授权的访问以及个人信息泄漏。<br />由于项目已停止迭代，改造的成本太大，因此选用了MyBatis插件来实现数据库加解密，保证往数据库写入数据时能对指定字段加密，读取数据时能对指定字段解密。
<a name="d1mLi"></a>
## 二、思路解析
<a name="t8QNp"></a>
### 2.1 系统架构
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661050517527-432097ae-352b-40d0-b64d-2d2792993c87.png#clientId=u67b355b8-532c-4&from=paste&id=u005d81cd&originHeight=933&originWidth=1053&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u41d140ff-4a60-4fff-818f-09e40229dcd&title=)

1. 对每个需要加密的字段新增密文字段（**对业务有侵入**），修改数据库、mapper.xml以及DO对象，通过插件的方式把针对明文/密文字段的加解密进行收口。
2. 自定义Executor对SELECT/UPDATE/INSERT/DELETE等操作的明文字段进行加密并设置到密文字段。
3. 自定义插件ResultSetHandler负责针对查询结果进行解密，负责对SELECT等操作的密文字段进行解密并设置到明文字段。
<a name="r4AHu"></a>
### 2.2 系统流程
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661050517505-6555577d-88c4-4f21-9d4f-0a90ba37aa1f.png#clientId=u67b355b8-532c-4&from=paste&id=u746e06bd&originHeight=145&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c92d4b4-1581-4096-8d6a-a2655f94b33&title=)

1. 新增加解密流程控制开关，分别控制写入时是只写原字段/双写/只写加密后的字段，以及读取时是读原字段还是加密后的字段。
2. 新增历史数据加密任务，对历史数据批量进行加密，写入到加密后字段。
3. 出于安全上的考虑，流程里还会有一些校验/补偿的任务，这里不再赘述。
<a name="tlhXo"></a>
## 三、方案制定
<a name="STxYy"></a>
### 3.1 MyBatis插件简介
MyBatis 预留了 `org.apache.ibatis.plugin.Interceptor` 接口，通过实现该接口，能对MyBatis的执行流程进行拦截，接口的定义如下:
```java
public interface Interceptor {
    Object intercept(Invocation invocation) throws Throwable;
    Object plugin(Object target);
    void setProperties(Properties properties);
}
```
其中有三个方法：

- **【**`**intercept**`**】**：插件执行的具体流程，传入的`Invocation`是MyBatis对被代理的方法的封装。
- **【**`**plugin**`**】**：使用当前的`Interceptor`创建代理，通常的实现都是 `Plugin.wrap(target, this)`，`wrap`方法内使用 jdk 创建动态代理对象。
- **【**`**setProperties**`**】**：参考下方代码，在MyBatis配置文件中配置插件时可以设置参数，在`setProperties`函数中调用 `Properties.getProperty("param1")` 方法可以得到配置的值。
```xml
<plugins>
  <plugin interceptor="com.xx.xx.xxxInterceptor">
    <property name="param1" value="value1"/>
  </plugin>
</plugins>
```
在实现`intercept`函数对MyBatis的执行流程进行拦截前，需要使用`@Intercepts`注解指定拦截的方法。
```java
@Intercepts({ @Signature(type = Executor.class, method = "update", args = { MappedStatement.class, Object.class }),
        @Signature(type = Executor.class, method = "query", args = { MappedStatement.class, Object.class, RowBounds.class, ResultHandler.class }) })
```
参考上方代码，可以指定需要拦截的类和方法。当然不能对任意的对象做拦截，MyBatis件可拦截的类为以下四个。

1. `Executor`
2. `StatementHandler`
3. `ParameterHandler`
4. `ResultSetHandler`

回到数据库加密的需求，需要从上面四个类里选择能用来实现入参加密和出参解密的类。在介绍这四个类之前，需要对MyBatis的执行流程有一定的了解。
<a name="ZG7uZ"></a>
### 3.2 Spring-MyBatis执行流程 
（1）Spring通过`sqlSessionFactoryBean`创建`sqlSessionFactory`，在使用`sqlSessionFactoryBean`时，通常会指定`configLocation`和`mapperLocations`，来告诉`sqlSessionFactoryBean`去哪里读取配置文件以及去哪里读取mapper文件。<br />（2）得到配置文件和mapper文件的位置后，分别调用`XmlConfigBuilder.parse()`和`XmlMapperBuilder.parse()`创建`Configuration`和`MappedStatement`，`Configuration`类顾名思义，存放的是MyBatis所有的配置，而`MappedStatement`类存放的是每条SQL语句的封装，`MappedStatement`以map的形式存放到`Configuration`对象中，key为对应方法的全路径。<br />（3）Spring通过`ClassPathMapperScanner`扫描所有的`Mapper`接口，为其创建`BeanDefinition`对象，但由于他们本质上都是没有被实现的接口，所以Spring会将他们的`BeanDefinition`的`beanClass`属性修改为`MapperFactorybean`。<br />（4）`MapperFactoryBean`也实现了FactoryBean接口，Spring在创建Bean时会调用`FactoryBean.getObject()`方法获取Bean，最终是通过`mapperProxyFactory`的`newInstance`方法为mapper接口创建代理，创建代理的方式是JDK，最终生成的代理对象是`MapperProxy`。<br />（5）调用mapper的所有接口本质上调用的都是`MapperProxy.invoke`方法，内部调用`sqlSession`的insert/update/delete等各种方法。<br />MapperMethod.java
```java
public Object execute(SqlSession sqlSession, Object[] args) {
    Object result;
    if (SqlCommandType.INSERT == command.getType()) {
        Object param = method.convertArgsToSqlCommandParam(args);
        result = rowCountResult(sqlSession.insert(command.getName(), param));
    } else if (SqlCommandType.UPDATE == command.getType()) {
        Object param = method.convertArgsToSqlCommandParam(args);
        result = rowCountResult(sqlSession.update(command.getName(), param));
    } else if (SqlCommandType.DELETE == command.getType()) {
        Object param = method.convertArgsToSqlCommandParam(args);
        result = rowCountResult(sqlSession.delete(command.getName(), param));
    } else if (SqlCommandType.SELECT == command.getType()) {
        if (method.returnsVoid() && method.hasResultHandler()) {
            executeWithResultHandler(sqlSession, args);
            result = null;
        } else if (method.returnsMany()) {
            result = executeForMany(sqlSession, args);
        } else if (method.returnsMap()) {
            result = executeForMap(sqlSession, args);
        } else {
            Object param = method.convertArgsToSqlCommandParam(args);
            result = sqlSession.selectOne(command.getName(), param);
        }
    } else if (SqlCommandType.FLUSH == command.getType()) {
        result = sqlSession.flushStatements();
    } else {
        throw new BindingException("Unknown execution method for: " + command.getName());
    }
    if (result == null && method.getReturnType().isPrimitive() && !method.returnsVoid()) {
        throw new BindingException("Mapper method '" + command.getName()
                                   + " attempted to return null from a method with a primitive return type (" + method.getReturnType() + ").");
    }
    return result;
}
```
（6）`SqlSession`可以理解为一次会话，`SqlSession`会从`Configuration`中获取对应的`MappedStatement`，交给`Executor`执行。<br />DefaultSqlSession.java
```java
@Override
public <E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds) {
    try {
        // 从configuration对象中使用被调用方法的全路径，获取对应的MappedStatement
        MappedStatement ms = configuration.getMappedStatement(statement);
        return executor.query(ms, wrapCollection(parameter), rowBounds, Executor.NO_RESULT_HANDLER);
    } catch (Exception e) {
        throw ExceptionFactory.wrapException("Error querying database.  Cause: " + e, e);
    } finally {
        ErrorContext.instance().reset();
    }
}
```
（7）`Executor`会先创建`StatementHandler`，`StatementHandler`可以理解为是一次语句的执行。<br />（8）然后`Executor`会获取连接，具体获取连接的方式取决于`Datasource`的实现，可以使用连接池等方式获取连接。<br />（9）之后调用`StatementHandler.prepare`方法，对应到JDBC执行流程中的`Connection.prepareStatement`这一步。<br />（10）`Executor`再调用`StatementHandler`的`parameterize`方法，设置参数，对应到<br />JDBC执行流程的`StatementHandler.setXXX()`设置参数，内部会创建`ParameterHandler`方法。<br />SimpleExecutor.java
```java
@Override
public <E> List<E> doQuery(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException {
    Statement stmt = null;
    try {
        Configuration configuration = ms.getConfiguration();
        // 创建StatementHandler，对应第7步
        StatementHandler handler = configuration.newStatementHandler(wrapper, ms, parameter, rowBounds, resultHandler, boundSql);
        // 获取连接，再调用conncetion.prepareStatement创建prepareStatement，设置参数
        stmt = prepareStatement(handler, ms.getStatementLog());
        // 执行prepareStatement
        return handler.<E>query(stmt, resultHandler);
    } finally {
        closeStatement(stmt);
	}
}
```
（11）再由`ResultSetHandler`处理返回结果，处理JDBC的返回值，将其转换为Java的对象。
<a name="RMlyU"></a>
### 3.3  MyBatis插件的创建时机
在`Configuration`类中，能看到`newExecutor`、`newStatementHandler`、`newParameterHandler`、`newResultSetHandler`这四个方法，插件的代理类就是在这四个方法中创建的，以`StatementHandeler`的创建为例：<br />Configuration.java
```java
public StatementHandler newStatementHandler(Executor executor, MappedStatement mappedStatement, Object parameterObject, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) {
  StatementHandler statementHandler = new RoutingStatementHandler(executor, mappedStatement, parameterObject, rowBounds, resultHandler, boundSql);
  // 使用责任链的形式创建代理
  statementHandler = (StatementHandler) interceptorChain.pluginAll(statementHandler);
  return statementHandler;
}
 
InterceptorChain.java
public Object pluginAll(Object target) {
  for (Interceptor interceptor : interceptors) {
    target = interceptor.plugin(target);
  }
  return target;
}
```
`interceptor.plugin`对应到自己实现的`interceptor`里的方法，通常的实现是 `Plugin.wrap(target, this);` ，该方法内部创建代理的方式为JDK。
<a name="bnlP8"></a>
### 3.4 MyBatis插件可拦截类选择
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661050517537-4775797f-b07f-437c-8176-ce27bc626db3.png#clientId=u67b355b8-532c-4&from=paste&id=uacf2057e&originHeight=683&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c2fc5b9-f2ce-4a04-ad74-784a34e5b9d&title=)<br />Mybatis本质上是对JDBC执行流程的封装。结合上图简要概括下Mybatis这几个可被代理类的职能。

- **【**`**Executor**`**】**: 真正执行SQL语句的对象，调用`sqlSession`的方法时，本质上都是调用executor的方法，还负责获取connection，创建StatementHandler。
- **【**`**StatementHandler**`**】**: 创建并持有`ParameterHandler`和`ResultSetHandler`对象，操作JDBC的statement与进行数据库操作。
- **【**`**ParameterHandler**`**】**: 处理入参，将Java方法上的参数设置到被执行语句中。
- **【**`**ResultSetHandler**`**】**: 处理SQL语句的执行结果，将返回值转换为Java对象。

对于入参的加密，需要在`ParameterHandler`调用`prepareStatement.setXXX()`方法设置参数前，将参数值修改为加密后的参数，这样一看好像拦截`Executor`/`StatementHandler`/`ParameterHandler`都可以。<br />但实际上呢？由于并不是在原始字段上做加密，而是新增了一个加密后字段，这会带来什么问题？请看下面这条mapper.xml文件中加了加密后字段的动态SQL：
```xml
<select id="selectUserList" resultMap="BaseResultMap" parameterType="com.xxx.internet.demo.entity.UserInfo">
  SELECT
  *
  FROM
  `t_user_info`
  <where>
    <if test="phone != null">
      `phone` = #{phone}
    </if>
    <!--            明文字段-->
    <if test="secret != null">
      AND `secret` = #{secret}
    </if>
    <!--            加密后字段-->
    <if test="secretCiper != null">
      AND `secret_ciper` = #{secretCiper}
    </if>
    <if test="name">
      AND `name` = #{name}
    </if>
  </where>
  ORDER BY `update_time` DESC
</select>
```
可以看到这条语句带了动态标签，那肯定不能直接交给JDBC创建`prepareStatement`，需要先将其解析成静态SQL，而这一步是在`Executor`在调用`StatementHandler.parameterize()`前做的，由`MappedStatementHandler.getBoundSql(Object parameterObject)`函数解析动态标签，生成静态SQL语句，这里的`parameterObject`可以暂时先将其看成一个Map，键值分别为参数名和参数值。<br />那么来看下用`StatementHandler`和`ParameterHandler`做参数加密会有什么问题，在执行`MappedStatementHandler.getBoundSql`时，`parameterObject`中并没有写入加密后的参数，在判断标签时必定为否，最后生成的静态SQL必然不包含加密后的字段，后续不管在`StatementHandler`和`ParameterHandler`中怎么处理`parameterObject`，都无法实现入参的加密。<br />因此，在入参的加密上只能选择拦截`Executor`的`update`和`query`方法。<br />那么返回值的解密呢？参考流程图，能对`ResultSetHandler`和`Executor`做拦截，事实也确实如此，在处理返回值这一点上，这两者是等价的，`ResultSetHandler.handleResultSet()`的返回值直接透传给`Executor`，再由`Executor`透传给`SqlSession`，所以两者任选其一就可以。
<a name="GyeiL"></a>
## 四、方案实施
在知道需要拦截的对象后，就可以开始实现加解密插件了。首先定义一个方法维度的注解。
```java
/**
 * 通过注解来表明，我们需要对那个字段进行加密
 */
@Target({ ElementType.METHOD })
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
public @interface TEncrypt {
    /**
     * 加密时从srcKey到destKey
     * @return
     */
    String[] srcKey() default {};
 
    /**
     * 解密时从destKey到srcKey
     * @return
     */
    String[] destKey() default {};
}
```
将该注解打在需要加解密的DAO层方法上。<br />UserMapper.java
```java
public interface UserMapper {
    @TEncrypt(srcKey = {"secret"}, destKey = {"secretCiper"})
    List<UserInfo> selectUserList(UserInfo userInfo);
    }
}
```
修改xxxMapper.xml文件
```java

<mapper namespace="com.xxx.internet.demo.mapper.UserMapper">
    <resultMap id="BaseResultMap" type="com.xxx.internet.demo.entity.UserInfo">
        <id column="id" jdbcType="BIGINT" property="id" />
        <id column="phone" jdbcType="VARCHAR" property="phone"/>
        <id column="secret" jdbcType="VARCHAR" property="secret"/>
        <!--        加密后映射-->
        <id column="secret_ciper" jdbcType="VARCHAR" property="secretCiper"/>
        <id column="name" jdbcType="VARCHAR" property="name" />
    </resultMap>

    <select id="selectUserList" resultMap="BaseResultMap" parameterType="com.xxx.internet.demo.entity.UserInfo">
        SELECT
        *
        FROM
        `t_user_info`
        <where>
        <if test="phone != null">
        `phone` = #{phone}
        </if>
        <!--            明文字段-->
        <if test="secret != null">
        AND `secret` = #{secret}
        </if>
        <!--            加密后字段-->
        <if test="secretCiper != null">
        AND `secret_ciper` = #{secretCiper}
        </if>
        <if test="name">
        AND `name` = #{name}
        </if>
        </where>
        ORDER BY `update_time` DESCv
    </select>
</mapper>
```
做完上面的修改，就可以编写加密插件了
```java
@Intercepts({ @Signature(type = Executor.class, method = "update", args = { MappedStatement.class, Object.class }),
        @Signature(type = Executor.class, method = "query", args = { MappedStatement.class, Object.class, RowBounds.class, ResultHandler.class }) })
public class ExecutorEncryptInterceptor implements Interceptor {
    private static final ObjectFactory        DEFAULT_OBJECT_FACTORY         = new DefaultObjectFactory();
 
    private static final ObjectWrapperFactory DEFAULT_OBJECT_WRAPPER_FACTORY = new DefaultObjectWrapperFactory();
 
    private static final ReflectorFactory     REFLECTOR_FACTORY              = new DefaultReflectorFactory();
 
    private static final List<String>         COLLECTION_NAME  = Arrays.asList("list");
 
    private static final String               COUNT_SUFFIX                   = "_COUNT";
 
    @Override
    public Object intercept(Invocation invocation) throws Throwable {
 
        // 获取拦截器拦截的设置参数对象DefaultParameterHandler
        final Object[] args = invocation.getArgs();
        MappedStatement mappedStatement = (MappedStatement) args[0];
        Object parameterObject = args[1];
 
        // id字段对应执行的SQL的方法的全路径，包含类名和方法名
        String id = mappedStatement.getId();
        String className = id.substring(0, id.lastIndexOf("."));
        String methodName = id.substring(id.lastIndexOf(".") + 1);
 
        // 分页插件会生成一个count语句，这个语句的参数也要做处理
        if (methodName.endsWith(COUNT_SUFFIX)) {
            methodName = methodName.substring(0, methodName.lastIndexOf(COUNT_SUFFIX));
        }
 
        // 动态加载类并获取类中的方法
        final Method[] methods = Class.forName(className).getMethods();
 
        // 遍历类的所有方法并找到此次调用的方法
        for (Method method : methods) {
            if (method.getName().equalsIgnoreCase(methodName) && method.isAnnotationPresent(TEncrypt.class)) {
 
                // 获取方法上的注解以及注解对应的参数
                TEncrypt paramAnnotation = method.getAnnotation(TEncrypt.class);
 
                // 支持加密的操作，这里只修改参数
                if (parameterObject instanceof Map) {
                    List<String> paramAnnotations = findParams(method);
                    parameterMapHandler((Map) parameterObject, paramAnnotation, mappedStatement.getSqlCommandType(), paramAnnotations);
                } else {
                    encryptParam(parameterObject, paramAnnotation, mappedStatement.getSqlCommandType());
                }
            }
        }
 
        return invocation.proceed();
    }
}
```
加密的主体流程如下：

1. 判断本次调用的方法上是否注解了`@TEncrypt`。
2. 获取注解以及在注解上配置的参数。
3. 遍历`parameterObject`，找到需要加密的字段。
4. 调用加密方法，得到加密后的值。
5. 将加密后的字段和值写入`parameterObject`。

难点主要在`parameterObject`的解析，到了`Executor`这一层，`parameterObject`已经不再是简单的`Object[]`，而是由`MapperMethod.convertArgsToSqlCommandParam(Object[] args)`方法创建的一个对象，既然要对这个对象做处理，肯定得先知道它的创建过程。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661050517512-f2a5d379-a8c2-4b31-8c8b-07225679dcd4.png#clientId=u67b355b8-532c-4&from=paste&id=u2214eb2b&originHeight=624&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud2ab8cb7-413b-4159-a7ea-d0e4cd856f9&title=)<br />参考上图`parameterObject`的创建过程，加密插件对`parameterObject`的处理本质上是一个逆向的过程。如果是list，就遍历list里的每一个值，如果是map，就遍历map里的每一个值。<br />得到需要处理的Object后，再遍历Object里的每个属性，判断是否在`@TEncrypt`注解的`srcKeys`参数中，如果是，则加密再设置到Object中。<br />解密插件的逻辑和加密插件基本一致，这里不再赘述。
<a name="GiJYT"></a>
## 五、问题挑战
<a name="IGJhJ"></a>
### 5.1 分页插件自动生成`count`语句
业务代码里很多地方都用了 `com.github.pagehelper` 进行物理分页，参考下面的demo，在使用`PageRowBounds`时，pagehelper插件会帮我们获取符合条件的数据总数并设置到`rowBounds`对象的total属性中。
```java
PageRowBounds rowBounds = new PageRowBounds(0, 10);
List<User> list = userMapper.selectIf(1, rowBounds);
long total = rowBounds.getTotal();
```
那么问题来了，表面上看，只执行了`userMapper.selectIf(1, rowBounds)`这一条语句，而pagehelper是通过改写SQL增加limit、offset实现的物理分页，在整个语句的执行过程中没有从数据库里把所有符合条件的数据读出来，那么pagehelper是怎么得到数据的总数的呢？<br />答案是pagehelper会再执行一条count语句。先不说额外一条执行count语句的原理，先看看加了一条count语句会导致什么问题。<br />参考之前的selectUserList接口，假设想选择secret为某个值的数据，那么经过加密插件的处理后最终执行的大致是这样一条语句 "`select * from t_user_info where secret_ciper = ? order by update_time limit ?, ?`"。<br />但由于pagehelper还会再执行一条语句，而由于该语句并没有 @TEncrypt 注解，所以是不会被加密插件拦截的，最终执行的count语句是类似这样的: "`select count(*) from t_user_info where secret = ? order by update_time`"。<br />可以明显的看到第一条语句是使用secret_ciper作为查询条件，而count语句是使用secret作为查询条件，会导致最终得到的数据总量和实际的数据总量不一致。<br />因此在加密插件的代码里对count语句做了特殊处理，由于`pagehelper`新增的count语句对应的`mappedStatement`的id固定以"_COUNT"结尾，而这个id就是对应的mapper里的方法的全路径，举例来说原始语句的id是"`com.xxx.internet.demo.entity.UserInfo.selectUserList`"，那么count语句的id就是"`com.xxx.internet.demo.entity.UserInfo.selectUserList_COUNT`"，去掉"_COUNT"后再判断对应的方法上有没有注解就可以了。
<a name="MQTcN"></a>
## 六、总结
本文介绍了使用 MyBatis 插件实现数据库字段加解密的探索过程，实际开发过程中需要注意的细节比较多，整个流程下来对 MyBatis 的理解也加深了。总的来说，这个方案比较轻量，虽然对业务代码有侵入，但能把影响面控制到最小。
