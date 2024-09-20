Java Spring Spring Native<br />写一段简单的 Java 程序。<br />通常想运行它要这样。但运行起来需要 jre。
```java
public class Hello {
    public static void main(String[] args) {
         System.out.println("hello world");
    }
}
```
```bash
[root@flash ~]# javac Hello.java
[root@flash ~]# java Hello
hello world
```
换一种方式来编译这个程序，首先下载一个 GraalVM 的 native-image 工具，然后。
```bash
[root@flash ~]# native-image Hello
[hello:11725]    classlist:   1,031.19 ms,  0.96 GB
[hello:11725]        (cap):   2,624.14 ms,  0.96 GB
[hello:11725]        setup:   3,960.95 ms,  0.96 GB
[hello:11725]     (clinit):     288.49 ms,  1.72 GB
[hello:11725]   (typeflow):   2,642.38 ms,  1.72 GB
[hello:11725]    (objects):   3,803.54 ms,  1.72 GB
[hello:11725]   (features):   1,176.79 ms,  1.72 GB
[hello:11725]     analysis:   8,288.82 ms,  1.72 GB
[hello:11725]     universe:     909.14 ms,  1.75 GB
[hello:11725]      (parse):     801.67 ms,  1.75 GB
[hello:11725]     (inline):   1,096.07 ms,  2.32 GB
[hello:11725]    (compile):   7,352.50 ms,  2.37 GB
[hello:11725]      compile:  10,146.59 ms,  2.37 GB
[hello:11725]        image:   1,639.93 ms,  2.37 GB
[hello:11725]        write:     682.24 ms,  2.37 GB
[hello:11725]      [total]:  26,855.67 ms,  2.37 GB
# Printing build artifacts to: .../hello.build_artifacts.txt
```
执行完这个命令后，发现当前目录多了个 hello 文件。<br />直接执行它，可以成功！
```bash
[root@flash ~]# ./hello
hello world
```
而且注意，这个是可以直接以二进制形式运行的，不依赖 jre。<br />也就是说，一个 Java 程序，被这个 **native-image** 编译成了本地代码！<br />这项技术来自于 GraalVM 的一个特性，在其官网的文档中可以了解到，GraalVM 主要有三大特性：

1. 通过新的 JIT 技术使 Java 程序更快运行
2. 多语言支持
3. 构建 JVM 无关的本地镜像

这个 native-image 技术就是其中的第三点，**即将 Java 代码编译成 JVM 无关的本地镜像**，使其可以直接以二进制的方式运行起来。<br />除了运行方便之外，对比一下这俩的**文件大小**和**启动时间**。
```bash
[root@flash ~]# ll
-rw-r--r--  1 flash  staff   415B 10 27 15:50 Hello.class
-rwxr-xr-x  1 flash  staff    10M 10 27 15:51 hello

[root@flash ~]# time java Hello
hello world
java Hello  0.09s user 0.03s system 113% cpu 0.106 total

[root@flash ~]# time ./hello
hello world
./hello  0.00s user 0.01s system 34% cpu 0.032 total
```
总结个表格。

| 可执行文件 | 文件大小 | 启动时间 |
| --- | --- | --- |
| Hello.class | 415B | 0.12s |
| hello | 10M | 0.01s |

**可以看出，启动时间大大缩短了！但文件大小却大大增加了。**<br />不过要知道，运行 Hello.class 要整个 jre 的支持，而运行二进制的 hello 却不需要，这部分文件大小的差距，在小代码上对比并不公平。<br />当然，启动时间也都是毫秒级的，差距也不足以说明问题。下面试着用这种方式，对比一个 Spring Boot 项目。<br />有一点要说明的是，GraalVM 的本地编译对 Java 代码有很多的限制，有的时候需要配合配置文件才能成功，比如不支持动态类加载、反射、序列化等，具体可以见这里：<br />[https://www.GraalVM.org/reference-manual/native-image/Limitations/](https://www.GraalVM.org/reference-manual/native-image/Limitations/)<br />可是 Spring 项目中可是大量充斥着这些，需要增加好多配置文件，才能成功本地编译一个 Spring Boot 项目。<br />好在，Spring 已经考虑好这些事情了，提供了一个专门为 native 而生的 Spring Boot 依赖项，最方便的是新建项目的时候可以直接从 **start.spring.io** 生成。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638153263023-382cb8d7-1112-49dd-ada2-d6f995101d6c.webp#clientId=u26332293-505f-4&from=paste&id=u755eb1ba&originHeight=849&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udc843eba-04c4-4225-9278-db8c1b1f9e7&title=)<br /> 然后可以直接用 `mvn` 命令来打包一个本地镜像。
```bash
[root@flash ~]# mvn package -Pnative
...
[INFO] Executing: ...native-image -cp ... -H:Name=demo-1
...
[demo-1:7725]    classlist:   1,695.81 ms,  0.94 GB
[demo-1:7725]        (cap):   1,932.48 ms,  0.94 GB
[demo-1:7725]        setup:   3,287.65 ms,  0.94 GB
[demo-1:7725]     (clinit):   2,256.61 ms,  5.68 GB
[demo-1:7725]   (typeflow):  18,462.41 ms,  5.68 GB
[demo-1:7725]    (objects):  17,848.47 ms,  5.68 GB
[demo-1:7725]   (features):   4,646.24 ms,  5.68 GB
[demo-1:7725]     analysis:  45,521.71 ms,  5.68 GB
[demo-1:7725]     universe:   2,624.03 ms,  5.68 GB
[demo-1:7725]      (parse):   1,917.71 ms,  5.68 GB
[demo-1:7725]     (inline):   6,021.71 ms,  5.93 GB
[demo-1:7725]    (compile):  30,497.99 ms,  6.06 GB
[demo-1:7725]      compile:  42,184.66 ms,  6.06 GB
[demo-1:7725]        image:   8,700.31 ms,  5.90 GB
[demo-1:7725]        write:   1,647.51 ms,  5.90 GB
[demo-1:7725]      [total]: 106,412.95 ms,  5.90 GB
# Printing build artifacts to: .../demo-1.build_artifacts.txt
```
同样，用传统的 jar 包方式打包一个 jar 文件，对比一下。
```bash
-rwxr-xr-x  1 flash  staff    66M Nov  2 16:11 demo-1
-rw-r--r--  1 flash  staff    17M Nov  2 16:09 demo-1-exec.jar
```
这回大小已经没差那么多了，但仍然是二进制的本地包大。不过这仅仅是几乎空的 Spring Boot 项目，随着项目依赖的包越来越多，二进制的文件大小会越来越有优势，这是后话了。<br />再来对比一下**启动速度**，首先是传统的 jar 包运行。
```bash
[root@flash ~]# java -jar demo-1-exec.jar 


  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::                (v2.5.6)


2021-11-02 16:36:11.192  INFO 9468 --- [main] com.example.demo1.Demo1Application       : Starting Demo1Application v0.0.1-SNAPSHOT using Java 11.0.12 on sunyiming07deMacBook-Pro.local with PID 9468 (/Users/sunyiming07/IdeaProjects/graalvm-demos/springboot/demo/demo-1/target/demo-1-0.0.1-SNAPSHOT-exec.jar started by sunyiming07 in /Users/sunyiming07/IdeaProjects/graalvm-demos/springboot/demo/demo-1/target)
2021-11-02 16:36:11.195  INFO 9468 --- [main] com.example.demo1.Demo1Application       : No active profile set, falling back to default profiles: default
2021-11-02 16:36:12.097  INFO 9468 --- [main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat initialized with port(s): 8080 (http)
2021-11-02 16:36:12.110  INFO 9468 --- [main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2021-11-02 16:36:12.110  INFO 9468 --- [main] org.apache.catalina.core.StandardEngine  : Starting Servlet engine: [Apache Tomcat/9.0.54]
2021-11-02 16:36:12.164  INFO 9468 --- [main] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2021-11-02 16:36:12.164  INFO 9468 --- [main] w.s.c.ServletWebServerApplicationContext : Root WebApplicationContext: initialization completed in 917 ms
2021-11-02 16:36:12.484  INFO 9468 --- [main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat started on port(s): 8080 (http) with context path ''
2021-11-02 16:36:12.494  INFO 9468 --- [main] com.example.demo1.Demo1Application       : Started Demo1Application in 2.033 seconds (JVM running for 2.504)
```
2.033 秒，已经慢下来了，不过正常的空 Spring Boot 项目也就这样。<br />再看看本地镜像启动速度。
```bash
[root@flash ~]# ./demo-1
2021-11-02 16:38:33.141  INFO 9724 --- [main] o.s.nativex.NativeListener               : This application is bootstrapped with code generated with Spring AOT


  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::                (v2.5.6)


2021-11-02 16:38:33.143  INFO 9724 --- [main] com.example.demo1.Demo1Application       : Starting Demo1Application v0.0.1-SNAPSHOT using Java 11.0.12 on sunyiming07deMacBook-Pro.local with PID 9724 (/Users/sunyiming07/IdeaProjects/graalvm-demos/springboot/demo/demo-1/target/demo-1 started by sunyiming07 in /Users/sunyiming07/IdeaProjects/graalvm-demos/springboot/demo/demo-1/target)
2021-11-02 16:38:33.143  INFO 9724 --- [main] com.example.demo1.Demo1Application       : No active profile set, falling back to default profiles: default
2021-11-02 16:38:33.178  INFO 9724 --- [main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat initialized with port(s): 8080 (http)
2021-11-02 16:38:33.178  INFO 9724 --- [main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2021-11-02 16:38:33.178  INFO 9724 --- [main] org.apache.catalina.core.StandardEngine  : Starting Servlet engine: [Apache Tomcat/9.0.54]
2021-11-02 16:38:33.184  INFO 9724 --- [main] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2021-11-02 16:38:33.184  INFO 9724 --- [main] w.s.c.ServletWebServerApplicationContext : Root WebApplicationContext: initialization completed in 41 ms
2021-11-02 16:38:33.204  INFO 9724 --- [main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat started on port(s): 8080 (http) with context path ''
2021-11-02 16:38:33.204  INFO 9724 --- [main] com.example.demo1.Demo1Application       : Started Demo1Application in 0.078 seconds (JVM running for 0.08)
```
**0.078 秒**！！！<br />从来没有启动 Spring Boot 项目体验过这么极速的状态呢！<br />看吧，前面的 hello world 项目看不出什么，现在的 Spring Boot 项目，优势就已经完全出来了，启动速度秒杀呀！<br />可想而知，原来启动可能要几分钟才成功的 Spring Boot 项目，会被这个 GraalVM 优化到多少呢？想想就激动！<br />刚刚也说了，想通过 GraalVM 的 native-image 功能编译一个 Java 程序，有很多限制，比如不支持动态类加载、反射、动态代理、JNI、序列化以及 invoke dynamic 指令等。<br />这是由于，**AOT **这种提前编译的技术，需要一个**封闭空间假设**，即在编译期就能够把运行期所有需要的东西都准备好，但 Java 的好多特性就是和这种封闭空间假设相冲突的。<br />Java 启动后随着程序不断运行，JVM 将一部分代码编译成本地代码，这个叫 **JIT **技术，它是在程序运行起来之后不断分析而做的编译，所以它不受封闭空间假设的限制。<br />说回 GraalVM 的 AOT，比如程序中有个反射，这就属于运行时才会知道有这样一个 Student 类被需要的情况。
```java
Class.forName("com.flash.Student")
```
当然，GraalVM 会通过扫描这些反射方法的调用，来尝试分析用到了哪些类。<br />如果分析不出来，就需要程序员手动配置，告诉 GraalVM 有哪些类要反射。
```java
[
    {
        name: "com.flash.Student",
        allDeclaredConstructors: true,
        allPublicMethods: true
    },
    {
        name: "com.flash.Teacher",
        fileds: [{name: "teach"}, {name: "talk"}],
        methods: [{
            name: "<init>",
            parameterTypes: ["char[]"]
        }]
    },
    // ……
]
```
但这样肯定是反人性的。<br />自己写的代码和依赖还好，但如果是使用第三方组建，比如人人都用的 Spring，肯定不能由程序员来去写这些配置文件。<br />那就只有让 Spring 官方提供这些配置，让程序员仍然是简单写一些 maven 依赖就能把项目跑起来，才能把这个技术推广出去，这也是刚刚 **Spring Native** 项目存在的意义。
