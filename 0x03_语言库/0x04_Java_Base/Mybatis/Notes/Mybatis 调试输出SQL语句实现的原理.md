JavaMybatis<br />Java 开发中常用的几款日志框架有很多种，并且这些日志框架来源于不同的开源组织，给用户暴露的接口也有很多不同之处，所以很多开源框架会自己定义一套统一的日志接口，兼容上述第三方日志框架，供上层使用。<br />一般实现的方式是使用适配器模式，将各个第三方日志框架接口转换为框架内部自定义的日志接口。MyBatis 也提供了类似的实现，这里就来简单了解一下。
<a name="vHBPr"></a>
## 适配器模式是什么？
简单来说，适配器模式主要解决的是由于接口不能兼容而导致类无法使用的问题，这在处理遗留代码以及集成第三方框架的时候用得比较多。其核心原理是：**通过组合的方式，将需要适配的类转换成使用者能够使用的接口。**
<a name="pRaPZ"></a>
## 日志模块
MyBatis 自定义的 Log 接口位于 `org.apache.ibatis.logging` 包中，相关的适配器也位于该包中。<br />首先是 LogFactory 工厂类，它负责创建 Log 对象，在 LogFactory 类中有一段静态代码块，其中会依次加载各个第三方日志框架的适配器。
```java
static {
    tryImplementation(LogFactory::useSlf4jLogging);
    tryImplementation(LogFactory::useCommonsLogging);
    tryImplementation(LogFactory::useLog4J2Logging);
    tryImplementation(LogFactory::useLog4JLogging);
    tryImplementation(LogFactory::useJdkLogging);
    tryImplementation(LogFactory::useNoLogging);
}
```
以 JDK Logging 的加载流程（`useJdkLogging()` 方法）为例，其具体代码实现和注释如下：
```java
/**
 * 首先会检测 logConstructor 字段是否为空，
 * 1.如果不为空，则表示已经成功确定当前使用的日志框架，直接返回；
 * 2.如果为空，则在当前线程中执行传入的 Runnable.run() 方法，尝试确定当前使用的日志框架
 */
private static void tryImplementation(Runnable runnable) {
    if (logConstructor == null) {
        try {
            runnable.run();
        } catch (Throwable t) {
            // ignore
        }
    }
}

public static synchronized void useJdkLogging() {
    setImplementation(org.apache.ibatis.logging.jdk14.Jdk14LoggingImpl.class);
}

private static void setImplementation(Class<? extends Log> implClass) {
    try {
        // 获取适配器的构造方法
        Constructor<? extends Log> candidate = implClass.getConstructor(String.class);
        // 尝试加载适配器，加载失败会抛出异常
        Log log = candidate.newInstance(LogFactory.class.getName());
        // 加载成功，则更新logConstructor字段，记录适配器的构造方法
        logConstructor = candidate;
    } catch (Throwable t) {
        throw new LogException("Error setting Log implementation.  Cause: " + t, t);
    }
}
```
<a name="kUDL9"></a>
### 打印SQL语句
<a name="Tcd2e"></a>
#### 如何开启打印
这里演示Mybatis在运行时怎么输出SQL语句，具体分析见原理章节。
<a name="bjm9K"></a>
#### 单独使用Mybatis
在mybatis.xml配置文件中添加如下配置：
```xml
<setting name="logImpl" value="STDOUT_LOGGING" />
```
<a name="BAjou"></a>
#### 和SpringBoot整合
有两种方式，第一种也是利用StdOutImpl实现类去实现打印，在application.yml配置文件填写如下：
```yaml
#mybatis配置
mybatis:
  # 控制台打印sql日志
  configuration:
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl
```
其次还可以通过指定日志级别来输出SQL语句：<br />SpringBoot默认使用的SL4J（日志门面）+Logback（具体实现）的日志组合
```yaml
logging:
  level:
    xx包名: debug
```
<a name="wEAAs"></a>
### 简单分析原理
这里直接看到`org.apache.ibatis.executor.BaseExecutor#getConnection`方法，了解Mybatis的应该都知道Mybatis在执行sql操作的时候会去获取数据库连接
```java
protected Connection getConnection(Log statementLog) throws SQLException {
    Connection connection = transaction.getConnection();
    // 判断日志级别是否为Debug，是的话返回代理对象
    if (statementLog.isDebugEnabled()) {
        return ConnectionLogger.newInstance(connection, statementLog, queryStack);
    } else {
        return connection;
    }
}
```
可以看到注释的那行，它通过判断日志级别来判断是否返回`ConnectionLogger`代理对象，那么前面提到 Log 接口的实现类中`StdOutImpl`它的`isDebugEnabled`其实是永远返回 true，代码如下：<br />并且它直接用的 `System.println`去输出的SQL信息
```java
public class StdOutImpl implements Log {

	// ...省略无关代码

	@Override
	public boolean isDebugEnabled() {
		return true;
	}

	@Override
	public boolean isTraceEnabled() {
		return true;
	}

	@Override
	public void error(String s, Throwable e) {
		System.err.println(s);
		e.printStackTrace(System.err);
	}

	@Override
	public void error(String s) {
		System.err.println(s);
	}
	// ...省略无关代码
}
```
到这里起码知道了为什么通过配置 MyBatis 所用日志的具体实现 `logImpl`就可以实现日志输出到控制台的效果了。<br />那么还可以深究一下 `statementLog` 是在什么时候变成 `StdOutImpl`的，在解析Mybatis配置文件的时候，会去读取配置的logImpl属性，然后通过`LogFactory.useCustomLogging`方法先指定好适配器的构造方法
```java
// org.apache.ibatis.builder.xml.XMLConfigBuilder#loadCustomLogImpl  
private void loadCustomLogImpl(Properties props) {
    Class<? extends Log> logImpl = resolveClass(props.getProperty("logImpl"));
    configuration.setLogImpl(logImpl);
}

public void setLogImpl(Class<? extends Log> logImpl) {
    if (logImpl != null) {
        this.logImpl = logImpl;
        LogFactory.useCustomLogging(this.logImpl);
    }
}
```
然后在构建`MappedStatement`的时候就已经将日志对象初始化好了<br />每个`MappedStatement`对应了自定义Mapper接口中的一个方法，它保存了开发人员编写的SQL语句、参数结构、返回值结构、Mybatis对它的处理方式的配置等细节要素，是对一个SQL命令是什么、执行方式的完整定义。
```java
public Builder(Configuration configuration, String id, SqlSource sqlSource, SqlCommandType sqlCommandType) {
	// ...省略无关代码
	mappedStatement.statementLog = LogFactory.getLog(logId);
	mappedStatement.lang = configuration.getDefaultScriptingLanguageInstance();
}

public static Log getLog(String logger) {
	try {
		return logConstructor.newInstance(logger);
	} catch (Throwable t) {
		throw new LogException("Error creating logger for logger " + logger + ".  Cause: " + t, t);
	}
}
```
最后SpringBoot的就不概述了

- 第一种方式其实也是同理
- 第二种方式是通过修改了日志级别，然后使 `isDebugEnabled` 返回true，去返回代理对象，然后去输出SQL语句。

感兴趣的还可以看看SQL语句的输出是怎么输出的，具体在 `ConnectionLogger`的`invoke`方法中，会发现熟悉的Preparing: "和"Parameters: "。
