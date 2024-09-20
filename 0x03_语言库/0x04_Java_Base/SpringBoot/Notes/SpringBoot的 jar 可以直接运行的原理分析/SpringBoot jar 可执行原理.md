Java SpringBoot<br />涉及的知识点主要包括Maven的生命周期以及自定义插件，JDK提供关于jar包的工具类以及Springboot如何扩展，最后是自定义类加载器。
<a name="WcCle"></a>
## spring-boot-maven-plugin
SpringBoot 的可执行jar包又称fat jar ，是包含所有第三方依赖的 jar 包，jar 包中嵌入了除 java 虚拟机以外的所有依赖，是一个 all-in-one jar 包。<br />普通插件maven-jar-plugin生成的包和spring-boot-maven-plugin生成的包之间的直接区别，是fat jar中主要增加了两部分，第一部分是lib目录，存放的是Maven依赖的jar包文件，第二部分是spring boot loader相关的类。<br />fat jar //目录结构  
```bash
├─BOOT-INF  
│  ├─classes  
│  └─lib  
├─META-INF  
│  ├─maven  
│  ├─app.properties  
│  ├─MANIFEST.MF        
└─org  
    └─springframework  
        └─boot  
            └─loader  
                ├─archive  
                ├─data  
                ├─jar  
                └─util 
```
也就是说想要知道fat jar是如何生成的，就必须知道spring-boot-maven-plugin工作机制，而spring-boot-maven-plugin属于自定义插件，因此又必须知道，Maven的自定义插件是如何工作的
<a name="wehZX"></a>
## Maven的自定义插件
Maven 拥有三套相互独立的生命周期：clean、default 和 site，而每个生命周期包含一些phase阶段，阶段是有顺序的，并且后面的阶段依赖于前面的阶段。生命周期的阶段phase与插件的目标goal相互绑定，用以完成实际的构建任务。
```xml
<plugin>  
	<groupId>org.springframework.boot</groupId>  
	<artifactId>spring-boot-maven-plugin</artifactId>  
	<executions>  
		<execution>  
			<goals>  
				<goal>repackage</goal>  
			</goals>  
		</execution>  
	</executions>  
</plugin>  
```
repackage目标对应的将执行到`org.springframework.boot.maven.RepackageMojo#execute`，该方法的主要逻辑是调用了`org.springframework.boot.maven.RepackageMojo#repackage`
```java
private void repackage() throws MojoExecutionException {  
	//获取使用maven-jar-plugin生成的jar，最终的命名将加上.orignal后缀  
	Artifact source = getSourceArtifact();  
	//最终文件，即Fat jar  
	File target = getTargetFile();  
	//获取重新打包器，将重新打包成可执行jar文件  
	Repackager repackager = getRepackager(source.getFile());  
	//查找并过滤项目运行时依赖的jar  
	Set<Artifact> artifacts = filterDependencies(this.project.getArtifacts(),  
												 getFilters(getAdditionalFilters()));  
	//将artifacts转换成libraries  
	Libraries libraries = new ArtifactsLibraries(artifacts, this.requiresUnpack,  
												 getLog());  
	try {  
		//提供Spring Boot启动脚本  
		LaunchScript launchScript = getLaunchScript();  
		//执行重新打包逻辑，生成最后fat jar  
		repackager.repackage(target, libraries, launchScript);  
	}  
	catch (IOException ex) {  
		throw new MojoExecutionException(ex.getMessage(), ex);  
	}  
	//将source更新成 xxx.jar.orignal文件  
	updateArtifact(source, target, repackager.getBackupFile());  
}
```
关心一下`org.springframework.boot.maven.RepackageMojo#getRepackager`这个方法，知道Repackager是如何生成的，也就大致能够推测出内在的打包逻辑。
```java
private Repackager getRepackager(File source) {  
	Repackager repackager = new Repackager(source, this.layoutFactory);  
	repackager.addMainClassTimeoutWarningListener(  
		new LoggingMainClassTimeoutWarningListener());  
	//设置main class的名称，如果不指定的话则会查找第一个包含main方法的类，repacke最后将会设置org.springframework.boot.loader.JarLauncher  
	repackager.setMainClass(this.mainClass);  
	if (this.layout != null) {  
		getLog().info("Layout: " + this.layout);  
		//重点关心下layout 最终返回了 org.springframework.boot.loader.tools.Layouts.Jar  
		repackager.setLayout(this.layout.layout());  
	}  
	return repackager;  
}
```
```java
/**  
 * Executable JAR layout.  
 */  
public static class Jar implements RepackagingLayout {  
   @Override  
   public String getLauncherClassName() {  
      return "org.springframework.boot.loader.JarLauncher";  
   }  
   @Override  
   public String getLibraryDestination(String libraryName, LibraryScope scope) {  
      return "BOOT-INF/lib/";  
   }  
   @Override  
   public String getClassesLocation() {  
      return "";  
   }  
   @Override  
   public String getRepackagedClassesLocation() {  
      return "BOOT-INF/classes/";  
   }  
   @Override  
   public boolean isExecutable() {  
      return true;  
   }  
}  
```
layout可以将之翻译为文件布局，或者目录布局，代码一看清晰明了，同时需要关注，也是下一个重点关注对象org.springframework.boot.loader.JarLauncher，从名字推断，这很可能是返回可执行jar文件的启动类。
<a name="wMaUh"></a>
## MANIFEST.MF文件内容
```
Manifest-Version: 1.0  
Implementation-Title: oneday-auth-server  
Implementation-Version: 1.0.0-SNAPSHOT  
Archiver-Version: Plexus Archiver  
Built-By: oneday  
Implementation-Vendor-Id: com.oneday  
Spring-Boot-Version: 2.1.3.RELEASE  
Main-Class: org.springframework.boot.loader.JarLauncher  
Start-Class: com.oneday.auth.Application  
Spring-Boot-Classes: BOOT-INF/classes/  
Spring-Boot-Lib: BOOT-INF/lib/  
Created-By: Apache Maven 3.3.9  
Build-Jdk: 1.8.0_171
```
repackager生成的MANIFEST.MF文件为以上信息，可以看到两个关键信息Main-Class和Start-Class。可以进一步，程序的启动入口并不是SpringBoot中定义的main，而是`JarLauncher#main`，而再在其中利用反射调用定义好的Start-Class的main方法
<a name="PAXhy"></a>
## JarLauncher
<a name="Rim1Y"></a>
### 重点类介绍

- `java.util.jar.JarFile JDK`工具类提供的读取jar文件
- `org.springframework.boot.loader.jar.JarFileSpringboot-loader` 继承JDK提供JarFile类
- `java.util.jar.JarEntryDK`工具类提供的jar文件条目
- `org.springframework.boot.loader.jar.JarEntry Springboot-loader` 继承JDK提供JarEntry类
- `org.springframework.boot.loader.archive.Archive` Springboot抽象出来的统一访问资源的层
   - `JarFileArchivejar`包文件的抽象
   - `ExplodedArchive`文件目录

这里重点描述一下JarFile的作用，每个`JarFileArchive`都会对应一个JarFile。在构造的时候会解析内部结构，去获取jar包里的各个文件或文件夹类。可以看一下该类的注释。
```java
/* Extended variant of {@link java.util.jar.JarFile} that behaves in the same way but  
* offers the following additional functionality.  
* <ul>  
* <li>A nested {@link JarFile} can be {@link #getNestedJarFile(ZipEntry) obtained} based  
* on any directory entry.</li>  
* <li>A nested {@link JarFile} can be {@link #getNestedJarFile(ZipEntry) obtained} for  
* embedded JAR files (as long as their entry is not compressed).</li>  
</ul>  
**/
```
jar里的资源分隔符是!/，在JDK提供的JarFile URL只支持一个'`!/`'，而Spring boot扩展了这个协议，让它支持多个'`!/`'，就可以表示jar in jar、jar in directory、fat jar的资源了。
<a name="fNace"></a>
### 自定义类加载机制

- 最基础：Bootstrap ClassLoader（加载JDK的/lib目录下的类）
- 次基础：Extension ClassLoader（加载JDK的/lib/ext目录下的类）
- 普通：Application ClassLoader（程序自己classpath下的类）

首先需要关注双亲委派机制很重要的一点是，如果一个类可以被委派最基础的ClassLoader加载，就不能让高层的`ClassLoader`加载，这样是为了范围错误的引入了非JDK下但是类名一样的类。<br />其二，如果在这个机制下，由于fat jar中依赖的各个第三方jar文件，并不在程序自己classpath下，也就是说，如果采用双亲委派机制的话，根本获取不到所依赖的jar包，因此需要修改双亲委派机制的查找class的方法，自定义类加载机制。<br />先简单的介绍Springboot2中`LaunchedURLClassLoader`，该类继承了`java.net.URLClassLoader`，重写了`java.lang.ClassLoader#loadClass(java.lang.String, boolean)`，然后再探讨他是如何修改双亲委派机制。<br />在上面讲到Spring boot支持多个'`!/`'以表示多个jar，而问题在于，如何解决查找到这多个jar包。看一下`LaunchedURLClassLoader`的构造方法。
```java
public LaunchedURLClassLoader(URL[] urls, ClassLoader parent) {  
   super(urls, parent);  
} 
```
urls注释解释道the URLs from which to load classes and resources，即fat jar包依赖的所有类和资源，将该urls参数传递给父类`java.net.URLClassLoader`，由父类的`java.net.URLClassLoader#findClass`执行查找类方法，该类的查找来源即构造方法传递进来的urls参数。
```java
//LaunchedURLClassLoader的实现  
protected Class<?> loadClass(String name, boolean resolve)  
      throws ClassNotFoundException {  
   Handler.setUseFastConnectionExceptions(true);  
   try {  
      try {  
          //尝试根据类名去定义类所在的包，即java.lang.Package，确保jar in jar里匹配的manifest能够和关联               //的package关联起来  
         definePackageIfNecessary(name);  
      }  
      catch (IllegalArgumentException ex) {  
         // Tolerate race condition due to being parallel capable  
         if (getPackage(name) == null) {  
            // This should never happen as the IllegalArgumentException indicates  
            // that the package has already been defined and, therefore,  
            // getPackage(name) should not return null.  
  
            //这里异常表明，definePackageIfNecessary方法的作用实际上是预先过滤掉查找不到的包  
            throw new AssertionError("Package " + name + " has already been "  
                  + "defined but it could not be found");  
         }  
      }  
      return super.loadClass(name, resolve);  
   }  
   finally {  
      Handler.setUseFastConnectionExceptions(false);  
   }  
}
```
方法`super.loadClass(name, resolve)`实际上会回到了`java.lang.ClassLoader#loadClass(java.lang.String, boolean)`，遵循双亲委派机制进行查找类，而Bootstrap ClassLoader和Extension ClassLoader将会查找不到fat jar依赖的类，最终会来到`Application ClassLoader`，调用`java.net.URLClassLoader#findClass`
<a name="JwQEk"></a>
### 如何真正的启动
Springboot2和Springboot1的最大区别在于，Springboo1会新起一个线程，来执行相应的反射调用逻辑，而SpringBoot2则去掉了构建新的线程这一步。<br />方法是`org.springframework.boot.loader.Launcher#launch(java.lang.String[], java.lang.String, java.lang.ClassLoader)`反射调用逻辑比较简单，这里就不再分析，比较关键的一点是，在调用main方法之前，将当前线程的上下文类加载器设置成`LaunchedURLClassLoader`
```java
protected void launch(String[] args, String mainClass, ClassLoader classLoader)  
      throws Exception {  
   Thread.currentThread().setContextClassLoader(classLoader);  
   createMainMethodRunner(mainClass, args, classLoader).run();  
}
```
<a name="FR0DG"></a>
### Demo
```java
public static void main(String[] args) throws ClassNotFoundException, MalformedURLException {  
	JarFile.registerUrlProtocolHandler();  
	// 构造LaunchedURLClassLoader类加载器，这里使用了2个URL，分别对应jar包中依赖包spring-boot-loader和spring-boot，使用 "!/" 分开，需要org.springframework.boot.loader.jar.Handler处理器处理  
	LaunchedURLClassLoader classLoader = new LaunchedURLClassLoader(  
		new URL[] {  
			new URL("jar:file:/E:/IdeaProjects/oneday-auth/oneday-auth-server/target/oneday-auth-server-1.0.0-SNAPSHOT.jar!/BOOT-INF/lib/spring-boot-loader-1.2.3.RELEASE.jar!/")  
				, new URL("jar:file:/E:/IdeaProjects/oneday-auth/oneday-auth-server/target/oneday-auth-server-1.0.0-SNAPSHOT.jar!/BOOT-INF/lib/spring-boot-2.1.3.RELEASE.jar!/")  
			},  
		Application.class.getClassLoader());  
	// 加载类  
	// 这2个类都会在第二步本地查找中被找出(URLClassLoader的findClass方法)  
	classLoader.loadClass("org.springframework.boot.loader.JarLauncher");  
	classLoader.loadClass("org.springframework.boot.SpringApplication");  
	// 在第三步使用默认的加载顺序在ApplicationClassLoader中被找出  
	classLoader.loadClass("org.springframework.boot.autoconfigure.web.DispatcherServletAutoConfiguration");  

	//        SpringApplication.run(Application.class, args);  
}  
```
```xml
<!-- https://mvnrepository.com/artifact/org.springframework.boot/spring-boot-loader -->  
<dependency>  
    <groupId>org.springframework.boot</groupId>  
    <artifactId>spring-boot-loader</artifactId>  
    <version>2.1.3.RELEASE</version>  
</dependency>  
<dependency>  
    <groupId>org.springframework.boot</groupId>  
    <artifactId>spring-boot-maven-plugin</artifactId>  
    <version>2.1.3.RELEASE</version>  
</dependency>  
```
<a name="YLU8E"></a>
## 总结
从Maven的自定义插件开始进行追踪，巩固了对Maven的知识点，在这个过程中甚至了解到JDK对jar的读取是有提供对应的工具类。最后最重要的知识点则是自定义类加载器。
