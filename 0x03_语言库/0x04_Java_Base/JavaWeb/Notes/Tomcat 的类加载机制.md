Java Tomcat 类加载机制
<a name="rnIRb"></a>
## JVM 类加载器
<a name="pO00Y"></a>
### 1、JVM类加载器
说起 Tomcat 类加载器，就不得不先简单说一下 JVM 类加载器，如下图所示：<br />![2021-05-07-21-29-49-124583.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620394310648-95730ac2-4526-4cdc-b657-a7a02d458511.png#clientId=u80ca43a9-3178-4&from=ui&id=uf4863602&originHeight=443&originWidth=452&originalType=binary&size=22399&status=done&style=shadow&taskId=u0dfea8fe-36c0-4461-961e-7265b40d4f8)

- 启动类加载器：Bootstrap ClassLoader，用于加载JVM提供的基础运行类，即位于%JAVA_HOME%/jre/lib目录下的核心类库；
- 扩展类加载器：Extension ClassLoader， Java提供的一个标准的扩展机制用于加载除核心类库外的Jar包，即只要复制 到指定的扩展目录（可以多个）下的Jar, JVM会自动加载（不需要通过-classpath指定）。默认的扩展目录是％JAVA_HOME%加e/lib/ext。典型的应用场景就是，Java使用该类加载 器加载JVM默认提供的但是不属于核心类库的Jar。不推荐将应用程序依赖的 类库放置到扩展目录下，因为该目录下的类库对所有基于该JVM运行的应用程序可见；
- 应用程序类加载器：Application ClassLoader ，用于加载环境变量CLASSPATH （不推荐使用）指定目录下的或者-classpath运行 参数指定的Jar包。System类加载器通常用于加载应用程序Jar包及其启动入口类（Tomcat 的Bootstrap类即由System类加载器加载）。

这些类加载器的工作原理是一样的，区别是它们的加载路径不同，也就是说 findClass 这个方法查找的路径不同。<br />双亲委托机制是为了保证一个 Java 类在 JVM 中是唯一的，假如你不小心写了一个与 JRE 核心类同名的类，比如 Object 类，双亲委托机制能保证加载的是 JRE 里的那个 Object 类，而不是你写的 Object 类。<br />这是因为 AppClassLoader 在加载 Object 类时，会委托给 ExtClassLoader 去加载，而 ExtClassLoader 又会委托给 BootstrapClassLoader，BootstrapClassLoader 发现自己已经加载过了 Object 类，会直接返回，不会去加载你写的 Object 类。<br />这里请注意，类加载器的父子关系不是通过继承来实现的，比如 AppClassLoader 并不是 ExtClassLoader 的子类，而是说 AppClassLoader 的 parent 成员变量指向 ExtClassLoader 对象。同样的道理，如果你要自定义类加载器，不去继承 AppClassLoader，而是继承 ClassLoader 抽象类，再重写 findClass 和 loadClass 方法即可，Tomcat 就是通过自定义类加载器来实现自己的类加载逻辑。如果要打破双亲委托机制，就需要重写 loadClass 方法，因为 loadClass 的默认实现就是双亲委托机制。
<a name="mFhuB"></a>
### 2、类加载器的源码
```java
public abstract class ClassLoader {
  //  每个类加载器都有一个父加载器
  private final ClassLoader parent;
  public Class<?> loadClass(String name) throws ClassNotFoundException {
        return loadClass(name, false);
    }
     protected Class<?> loadClass(String name, boolean resolve)
        throws ClassNotFoundException
    {
            // First, check if the class has already been loaded
            Class<?> c = findLoadedClass(name);
           // 如果没有加载过
            if (c == null) {
                if (parent != null) {
                  //  先委托给父加载器去加载，注意这是个递归调用
                 c = parent.loadClass(name, false);
                } else {
                 // 如果父加载器为空，查找 Bootstrap 加载器是不是加载过了
                   c = findBootstrapClassOrNull(name);
                }
              
            // 如果父加载器没加载成功，调用自己的 findClass 去加载
                if (c == null) {        
                    c = findClass(name);
                }
            } 
        
            return c;
        }
        
    }
    //ClassLoader 中findClass方式需要被子类覆盖，下面这段代码就是对应代码
      protected Class<?> findClass(String name){
       //1. 根据传入的类名 name，到在特定目录下去寻找类文件，把.class 文件读入内存
          ...
       //2. 调用 defineClass 将字节数组转成 Class 对象
       return defineClass(buf, off, len)；
    }
      // 将字节码数组解析成一个 Class 对象，用 native 方法实现
    protected final Class<?> defineClass(byte[] b, int off, int len){
    
    }
    
}
```
自定义类加载器就需要重写ClassLoader的loadClass方法。

<a name="LV5qN"></a>
## Tomcat 的类加载机制 
<a name="OnB0L"></a>
### 1、加载机制的特点

- 隔离性：Web应用类库相互隔离，避免依赖库或者应用包相互影响。设想一下，如果我们 有两个Web应用，一个釆用了Spring 2.5, 一个采用了Spring 4.0,而应用服务器使用一个 类加载器加载,那么Web应用将会由于Jar包覆盖而导致无法启动成功；
- 灵活性：既然Web应用之间的类加载器相互独立，那么我们就能只针对一个Web应用进行 重新部署，此时该Web应用的类加载器将会重新创建，而且不会影响其他Web应用。如果 釆用一个类加载器，显然无法实现，因为只有一个类加载器的时候，类之间的依赖是杂 乱无章的，无法完整地移除某个Web应用的类；
- 性能：由于每个Web应用都有一个类加载器，因此Web应用在加载类时，不会搜索其他 Web应用包含的Jar包，性能自然高于应用服务器只有一个类加载器的情况。
<a name="TGEVr"></a>
### 2、Tomcat 的类加载方案
![2021-05-07-21-29-49-376908.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620394332966-e42a366f-c5b9-40ac-907e-f10fc9b4bae3.png#clientId=u80ca43a9-3178-4&from=ui&id=u8429837d&originHeight=1170&originWidth=1080&originalType=binary&size=88380&status=done&style=shadow&taskId=ufebfdee3-9ce5-4200-b217-0a76a2f2345)

- 引导类加载器和扩展类加载器 的作用不变；
- 系统类加载器正常情况下加载的是 CLASSPATH 下的类，但是 Tomcat 的启动脚本并未使用该变量，而是加载tomcat启动的类，比如bootstrap.jar，通常在catalina.bat或者catalina.sh中指定。位于CATALINA_HOME/bin下；
- Common 通用类加载器加载Tomcat使用以及应用通用的⼀些类，位于CATALINA_HOME/lib下，比如servlet-api.jar；
- Catalina ClassLoader 用于加载服务器内部可见类，这些类应⽤程序不能访问；
- SharedClassLoader 用于加载应⽤程序共享类，这些类服务器不会依赖；
- WebappClassLoader，每个应⽤程序都会有⼀个独一无二的Webapp ClassLoader，他用来加载本应用程序 /WEB-INF/classes 和 /WEB-INF/lib 下的类。

tomcat 8.5 默认改变了严格的双亲委派机制：

- 从缓存中加载；
- 如果缓存中没有，会先调用ExtClassLoader进行加载， 扩展类加载器是遵循双亲委派的，他会调用bootstrap，查看对应的lib有没有，然后回退给ExtClassLoader对扩展包下的数据进行加载；
- 如果未加载到，则从 /WEB-INF/classes加载；
- 如果未加载到，则从 /WEB-INF/lib/*.jar 加载如果未加载到，WebAppclassLoader 会委派给SharedClassLoader,SharedClassLoad会委派给CommonClassLoader.....,依次委派给BootstrapClassLoader, 然后BootstrapClassLoader 在自己目录中查找对应的类如果有则进行加载，如果没有他会委派给下一级ExtClassLoader,ExtClassLoader再查找自己目录下的类，如果有则加载如果没有则委派给下一级……遵循双亲委派原则。
<a name="H9TjE"></a>
### 3、分析应用类加载器的加载过程
应用类加载器为WebappClassLoader ，他的loadClass在他的父类WebappClassLoaderBase中。
```java
public Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
    synchronized (getClassLoadingLock(name)) {
        if (log.isDebugEnabled())
            log.debug("loadClass(" + name + ", " + resolve + ")");
        Class<?> clazz = null;
        // Log access to stopped class loader
        checkStateForClassLoading(name);    
        //从当前ClassLoader的本地缓存中加载类，如果找到则返回
        clazz = findLoadedClass0(name);
        if (clazz != null) {
            if (log.isDebugEnabled())
                log.debug("  Returning class from cache");
            if (resolve)
                resolveClass(clazz);
            return clazz;
        }
        // 本地缓存没有的情况下，调用ClassLoader的findLoadedClass方法查看jvm是否已经加载过此类，如果已经加载则直接返回。
        clazz = findLoadedClass(name);
        if (clazz != null) {
            if (log.isDebugEnabled())
                log.debug("  Returning class from cache");
            if (resolve)
                resolveClass(clazz);
            return clazz;
        }
        String resourceName = binaryNameToPath(name, false);
        //此时的javaseClassLoader是扩展类加载器  是把扩展类加载器赋值给了javaseClassLoader
        ClassLoader javaseLoader = getJavaseClassLoader();
        boolean tryLoadingFromJavaseLoader;
        try {
            .....
                //如果可以用getResource得到
                //如果能用扩展类加载器的getResource得到就证明可以被扩展类加载器加载到接下来安排扩展类加载器加载
                if (tryLoadingFromJavaseLoader) {
                    try {
                        //使用扩展类加载器进行加载
                        clazz = javaseLoader.loadClass(name);
                        if (clazz != null) {
                            if (resolve)
                                resolveClass(clazz);
                            return clazz;
                        }
                    } catch (ClassNotFoundException e) {
                        // Ignore
                    }
                }
            // (0.5) Permission to access this class when using a SecurityManager
            if (securityManager != null) {
                int i = name.lastIndexOf('.');
                if (i >= 0) {
                    try {
                        securityManager.checkPackageAccess(name.substring(0,i));
                    } catch (SecurityException se) {
                        String error = "Security Violation, attempt to use " +
                            "Restricted Class: " + name;
                        log.info(error, se);
                        throw new ClassNotFoundException(error, se);
                    }
                }
            }
            boolean delegateLoad = delegate || filter(name, true);
            // (1) Delegate to our parent if requested
            //如果是true就是用父类加载器进行加载
            if (delegateLoad) {
                if (log.isDebugEnabled())
                    log.debug("  Delegating to parent classloader1 " + parent);
                try {
                    clazz = Class.forName(name, false, parent);
                    if (clazz != null) {
                        if (log.isDebugEnabled())
                            log.debug("  Loading class from parent");
                        if (resolve)
                            resolveClass(clazz);
                        return clazz;
                    }
                } catch (ClassNotFoundException e) {
                    // Ignore
                }
            }
            // (2) Search local repositories
            if (log.isDebugEnabled())
                log.debug("  Searching local repositories");
            try {
                // 本地进行加载
                clazz = findClass(name);
                if (clazz != null) {
                    if (log.isDebugEnabled())
                        log.debug("  Loading class from local repository");
                    if (resolve)
                        resolveClass(clazz);
                    return clazz;
                }
            } catch (ClassNotFoundException e) {
                // Ignore
            }
            // (3) Delegate to parent unconditionally
            //到这里还是没有加载上再次尝试使用父类加载器进行加载
            if (!delegateLoad) {
                if (log.isDebugEnabled())
                    log.debug("  Delegating to parent classloader at end: " + parent);
                try {
                    clazz = Class.forName(name, false, parent);
                    if (clazz != null) {
                        if (log.isDebugEnabled())
                            log.debug("  Loading class from parent");
                        if (resolve)
                            resolveClass(clazz);
                        return clazz;
                    }
                } catch (ClassNotFoundException e) {
                    // Ignore
                }
            }
        }
        throw new ClassNotFoundException(name);
    }
```
注：在37行英文注释中标注获取的是系统类加载器，debug的时候会发现他是扩展类加载器，实际中可以推断出他应该是扩展类加载器，因为如果加载的类在扩展类加载器路径下已经存在的话，那直接调用系统类加载器是就是错误的了。
:::success
总结：tomcat打破了双亲委派的原则，实际是在应用类加载器中打破了双亲委派，其他类加载器还是遵循双亲委派的。
:::
