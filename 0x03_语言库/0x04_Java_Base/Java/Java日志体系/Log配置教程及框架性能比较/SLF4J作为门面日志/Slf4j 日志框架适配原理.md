Java 日志 Slf4j
<a name="u0BhB"></a>
## 概要
slf4j只是为日志的输出提供了统一接口，并没有具体的实现，就好像JDBC一样。那么，大家会不会好奇slf4j是怎么绑定/适配/桥接到log4j或者logback其他日志实现组件的呢？
<a name="dTmVk"></a>
## 适配过程原理
统计API接口，说明slf4j使用的是门面模式（Facade），然后就很容易猜测到大致的调用过程是，slf4j是通过自己的api去调用实现组件的api，这样来完成适配的。重点看看是怎么做到适配的。<br />源码基于slf4j-api.1.7.25
<a name="lBiP5"></a>
### slf4j通用门面的实现
调用slf4j时都是使用它的api，首先需要获取它的logger<br />一般使用slf4j都是这样子的
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

private Logger logger = LoggerFactory.getLogger(LogTest.class);
```
<a name="R4gOh"></a>
#### `getLogger`
对`getLogger()`方法源码跟踪下去
```java
public static Logger getLogger(Class<?> clazz) {
    Logger logger = getLogger(clazz.getName());
    if (DETECT_LOGGER_NAME_MISMATCH) {
        Class<?> autoComputedCallingClass = Util.getCallingClass();
        if (autoComputedCallingClass != null && nonMatchingClasses(clazz, autoComputedCallingClass)) {
            Util.report(String.format("Detected logger name mismatch. Given name: \"%s\"; computed name: \"%s\".", logger.getName(),
                                      autoComputedCallingClass.getName()));
            Util.report("See " + LOGGER_NAME_MISMATCH_URL + " for an explanation");
        }
    }
    return logger;
}

public static Logger getLogger(String name) {
    //获取logger的工厂来生成logger
    ILoggerFactory iLoggerFactory = getILoggerFactory();
    return iLoggerFactory.getLogger(name);
}
```
从`ILoggerFactory`的名字上来看，这是一个接口，而它又可以生成到具体实际的logger，那应该猜测到这个`ILoggerFactory`会跟其他日志实现相关，但是例如log4j，自己的实现肯定不会关心slf4j的呀，所以应该由适配jar包，即slf4j-log4j12.jar来实现。<br />继续看代码
```java
public static ILoggerFactory getILoggerFactory() {
    //从ILoggerFactory的状态可以看出，ILoggerFactory只会一次初始化
    if (INITIALIZATION_STATE == UNINITIALIZED) {
        synchronized (LoggerFactory.class) {
            //同步语句 + 双重判断，防止多次初始化
            //如果还没初始化，则进行初始化
            if (INITIALIZATION_STATE == UNINITIALIZED) {
                INITIALIZATION_STATE = ONGOING_INITIALIZATION;
                performInitialization();
            }
        }
    }
    switch (INITIALIZATION_STATE) {
    //初始化成功，即绑定成功，则从StaticLoggerBinder获取ILoggerFactory并返回
    case SUCCESSFUL_INITIALIZATION:
        return StaticLoggerBinder.getSingleton().getLoggerFactory();
    case NOP_FALLBACK_INITIALIZATION:
        return NOP_FALLBACK_FACTORY;
    case FAILED_INITIALIZATION:
        throw new IllegalStateException(UNSUCCESSFUL_INIT_MSG);
    case ONGOING_INITIALIZATION:
        return SUBST_FACTORY;
    }
    throw new IllegalStateException("Unreachable code");
}

//对ILoggerFactory的状态做说明
static final int UNINITIALIZED = 0; //没初始化
static final int ONGOING_INITIALIZATION = 1; //正在初始化
static final int FAILED_INITIALIZATION = 2; //初始化失败
static final int SUCCESSFUL_INITIALIZATION = 3; //初始化成功
static final int NOP_FALLBACK_INITIALIZATION = 4; //无日志实现
```
<a name="WzdQU"></a>
#### bind
`performInitialization()`方法看来是重点
```java
private final static void performInitialization() {
    bind();
    if (INITIALIZATION_STATE == SUCCESSFUL_INITIALIZATION) {
        versionSanityCheck();
    }
}
```
`bind()`方法
```java
private final static void bind() {
    try {
        Set<URL> staticLoggerBinderPathSet = null;
        if (!isAndroid()) {
            //找出可能绑定的日志的path，其实即StaticLoggerBinder.class文件
            staticLoggerBinderPathSet = findPossibleStaticLoggerBinderPathSet();
            //如果找出多个的话则打印错误信息。（等下会演示）
            reportMultipleBindingAmbiguity(staticLoggerBinderPathSet);
        }
        //通过获取单例来做初始化          
        StaticLoggerBinder.getSingleton();
        INITIALIZATION_STATE = SUCCESSFUL_INITIALIZATION;
        //打印实际绑定的那个日志实现。（等下会演示）
        reportActualBinding(staticLoggerBinderPathSet);
        fixSubstituteLoggers();
        replayEvents();
        // release all resources in SUBST_FACTORY
        SUBST_FACTORY.clear();
    } catch (NoClassDefFoundError ncde) {
        String msg = ncde.getMessage();
        if (messageContainsOrgSlf4jImplStaticLoggerBinder(msg)) {
            INITIALIZATION_STATE = NOP_FALLBACK_INITIALIZATION;
            Util.report("Failed to load class \"org.slf4j.impl.StaticLoggerBinder\".");
            Util.report("Defaulting to no-operation (NOP) logger implementation");
            Util.report("See " + NO_STATICLOGGERBINDER_URL + " for further details.");
        } else {
            failedBinding(ncde);
            throw ncde;
        }
    } catch (java.lang.NoSuchMethodError nsme) {
        String msg = nsme.getMessage();
        if (msg != null && msg.contains("org.slf4j.impl.StaticLoggerBinder.getSingleton()")) {
            INITIALIZATION_STATE = FAILED_INITIALIZATION;
            Util.report("slf4j-api 1.6.x (or later) is incompatible with this binding.");
            Util.report("Your binding is version 1.5.5 or earlier.");
            Util.report("Upgrade your binding to version 1.6.x.");
        }
        throw nsme;
    } catch (Exception e) {
        failedBinding(e);
        throw new IllegalStateException("Unexpected initialization failure", e);
    }
}
```
<a name="D3ejx"></a>
#### `StaticLoggerBinder`类
`findPossibleStaticLoggerBinderPathSet()`方法<br />从hard code看重要性，`org/slf4j/impl/StaticLoggerBinder.class`就是slf4j日志适配的关键
```java
//hard code
private static String STATIC_LOGGER_BINDER_PATH = "org/slf4j/impl/StaticLoggerBinder.class";

static Set<URL> findPossibleStaticLoggerBinderPathSet() {

    Set<URL> staticLoggerBinderPathSet = new LinkedHashSet<URL>();
    try {
        //获取LoggerFactory，即slf4j-apoi的类加载器
        ClassLoader loggerFactoryClassLoader = LoggerFactory.class.getClassLoader();
        Enumeration<URL> paths;
        //为null说明是由Bootstrap Classloader加载的，则转为App Classloader去加载
        if (loggerFactoryClassLoader == null) {
            paths = ClassLoader.getSystemResources(STATIC_LOGGER_BINDER_PATH);
        } else {
            //用跟slf4j一样的Classloader去加载
            paths = loggerFactoryClassLoader.getResources(STATIC_LOGGER_BINDER_PATH);
        }
        while (paths.hasMoreElements()) {
            URL path = paths.nextElement();
            staticLoggerBinderPathSet.add(path);
        }
    } catch (IOException ioe) {
        Util.report("Error getting resources from path", ioe);
    }
    return staticLoggerBinderPathSet;
}
```
从类加载器的用法说明，org/slf4j/impl/StaticLoggerBinder.class要跟slf4j-api.jar包在同一个类加载器中，一般来说即要求放在同一路径下比较稳妥，当然也可以通过-classpath来指定。<br />前面已经猜测org/slf4j/impl/StaticLoggerBinder应该是由各种适配器来实现的，来看看<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633498222336-cb194f97-695f-4aa6-a88f-6ee2921519f4.webp#clientId=u1422842b-cb8b-4&from=paste&id=u04811ab2&originHeight=211&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u7d2e13fa-80f3-40e2-896f-997fd350b7d)<br />在IDE的类搜索，可以找到两个StaticLoggerBinder<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633498222368-193de92b-0c03-4625-a559-b26c2bd1c25f.webp#clientId=u1422842b-cb8b-4&from=paste&id=u88de5da3&originHeight=293&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u716d4416-4c30-4003-8807-b9aad703d70)<br />调试刚刚的源码，可以看到找到了两个`StaticLoggerBinder.class`文件<br />那是因为这里依赖了
```xml
<dependency>
    <groupId>ch.qos.logback</groupId>
    <artifactId>logback-classic</artifactId>
    <version>${logback.version}</version>
</dependency>

<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>slf4j-log4j12</artifactId>
    <version>${slf4j.version}</version>
</dependency>
```
所以只是看到logback和log4j的适配器包。slf4j是对每一种日志实现都有对应的一个适配实现。适配器包的具体内容等下再看。（PS：这不是一个好的依赖配置，等下会说）<br />到这里已经找到了`StaticLoggerBinder`类了，`StaticLoggerBinder`是由各自的slf4j适配器包提供的。<br />这里有个trick，既然`StaticLoggerBinder`在slf4j-api有，也在其他logback-classic或slf4j-log4j12有，那么怎么确保JVM只加载到适配器包中的`StaticLoggerBinder`？其实看看slf4j代码的pom.xml就发现，答案是打包时是没有`StaticLoggerBinder`打进去的，这样slf4j-api.jar包是没有`StaticLoggerBinder`类的，JVM在找类时只会找到其他jar包的`StaticLoggerBinder`。<br />刚刚的源码到`bind()`方法的这一句
```java
StaticLoggerBinder.getSingleton();
```
这一句其实已经是调用适配包的代码，将会看到logback和log4j对应`StaticLoggerBinder`类的代码。
<a name="EVCVH"></a>
### 对logback适配实现
从上面的依赖可以看出，为什么slf4j对logback的适配是在logback-classic.jar包呢？logback-classic应该是logback的核心包才对，不应该关心slf4j的。那是因为slf4j和logback是同一个作者，所以才说logback是天然集成slf4j的。<br />来看看logback-classic.jar中的`StaticLoggerBinder`
```java
static {
    SINGLETON.init();
}
public static StaticLoggerBinder getSingleton() {
    return SINGLETON;
}
void init() {
    try {
        try {
            new ContextInitializer(defaultLoggerContext).autoConfig();
        } catch (JoranException je) {
            Util.report("Failed to auto configure default logger context", je);
        }
        // logback-292
        if (!StatusUtil.contextHasStatusListener(defaultLoggerContext)) {
            StatusPrinter.printInCaseOfErrorsOrWarnings(defaultLoggerContext);
        }
        contextSelectorBinder.init(defaultLoggerContext, KEY);
        initialized = true;
    } catch (Exception t) { // see LOGBACK-1159
        Util.report("Failed to instantiate [" + LoggerContext.class.getName() + "]", t);
    }
}
```
上面的就是logback的初始化了。
```java
public ILoggerFactory getLoggerFactory() {
    if (!initialized) {
        return defaultLoggerContext;
    }

    if (contextSelectorBinder.getContextSelector() == null) {
        throw new IllegalStateException("contextSelector cannot be null. See also " + NULL_CS_URL);
    }
    return contextSelectorBinder.getContextSelector().getLoggerContext();
}
```
`getLoggerFactory()`方法会返回`logback`的`LoggerContext`，而`LoggerContext`是继承slf4j的`ILoggerFactory`的，这样就适配到slf4j。<br />`Logger`是从`LoggerFactory`取出的。<br />看看`LoggerContext`的`getLogger()`方法
```java
public final Logger getLogger(final Class<?> clazz) {
    return getLogger(clazz.getName());
}

@Override
public final Logger getLogger(final String name) {

    if (name == null) {
        throw new IllegalArgumentException("name argument cannot be null");
    }

    // if we are asking for the root logger, then let us return it without
    // wasting time
    if (Logger.ROOT_LOGGER_NAME.equalsIgnoreCase(name)) {
        return root;
    }

    int i = 0;
    Logger logger = root;

    // check if the desired logger exists, if it does, return it
    // without further ado.
    Logger childLogger = (Logger) loggerCache.get(name);
    // if we have the child, then let us return it without wasting time
    if (childLogger != null) {
        return childLogger;
    }

    // if the desired logger does not exist, them create all the loggers
    // in between as well (if they don't already exist)
    String childName;
    while (true) {
        int h = LoggerNameUtil.getSeparatorIndexOf(name, i);
        if (h == -1) {
            childName = name;
        } else {
            childName = name.substring(0, h);
        }
        // move i left of the last point
        i = h + 1;
        synchronized (logger) {
            childLogger = logger.getChildByName(childName);
            if (childLogger == null) {
                childLogger = logger.createChildByName(childName);
                loggerCache.put(childName, childLogger);
                incSize();
            }
        }
        logger = childLogger;
        if (h == -1) {
            return childLogger;
        }
    }
}
```
这里涉及了logback很多逻辑，不太需要理会。这里主要看logback的`Logger`其实是继承了slf4j的`Logger`，这样就适配到slf4j。
<a name="BRp8m"></a>
### 对log4j配置实现
看了logback的适配，就猜到log4j的也差不多<br />slf4j-log4j12的`StaticLoggerBinder`
```java
private StaticLoggerBinder() {
    loggerFactory = new Log4jLoggerFactory();
    try {
        @SuppressWarnings("unused")
        Level level = Level.TRACE;
    } catch (NoSuchFieldError nsfe) {
        Util.report("This version of SLF4J requires log4j version 1.2.12 or later. See also http://www.slf4j.org/codes.html#log4j_version");
    }
}

public ILoggerFactory getLoggerFactory() {
    return loggerFactory;
}
```
`Log4jLoggerFactory()`是继承了slf4j的`ILoggerFactory`。继续看`getLogger`方法。
```java
public Logger getLogger(String name) {
    Logger slf4jLogger = loggerMap.get(name);
    if (slf4jLogger != null) {
        return slf4jLogger;
    } else {
        org.apache.log4j.Logger log4jLogger;
        if (name.equalsIgnoreCase(Logger.ROOT_LOGGER_NAME))
            log4jLogger = LogManager.getRootLogger();
        else
            log4jLogger = LogManager.getLogger(name);

        Logger newInstance = new Log4jLoggerAdapter(log4jLogger);
        Logger oldInstance = loggerMap.putIfAbsent(name, newInstance);
        return oldInstance == null ? newInstance : oldInstance;
    }
}
```
这里又是把log4j的`Logger`包装成slf4j的`Logger`，适配到slf4j。
<a name="WCwIM"></a>
### 图解
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633498222368-d17039de-dd52-4227-9cba-a388e6f22848.webp#clientId=u1422842b-cb8b-4&from=paste&id=u59c751ee&originHeight=578&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u5e94f3a8-87d1-4e5a-9a00-447ba5ffcf3)
<a name="QUu7h"></a>
## 总结
slf4j的适配原理是通过适配包的`org/slf4j/impl/StaticLoggerBinder`来做转承，适配包通过继承和使用slf4j-api的`ILoggerFactory`和`Logger`来完成适配。<br />在最新的版本（1.8.0）已经改为使用Java的SPI机制来实现，`StaticLoggerBinder`类已经不用了，改为`SLF4JServiceProvider`，这样就真正的面向接口编程了，不用打包时忽略`StaticLoggerBinder`。
