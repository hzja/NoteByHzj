JavaSpringBoot
<a name="TrBnw"></a>
## 目的
1、熟悉SpringBoot多环境配置<br />2、熟悉SpringBoot使用jar包启动时候的参数含义
<a name="gWxJG"></a>
## 测试代码
<a name="E5i3h"></a>
### 配置文件配置
application.yml
```yaml
# springboot多环境配置
# springboot多环境配置
#端口，项目上下文
server:
  port: 8080
  servlet:
    context-path: /springboot-params-demo

# 默认启动的是测试环境配置
spring:
  profiles:
    active: test

# 日志输出配置
logging:
  level:
    root: INFO
    org:
      springframework:
        security: WARN
        web: ERROR
  file:
    path: ./logs
    name: './logs/springboot-params-demo.log'
  pattern:
    file: '%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}:%L - %msg%n'
    console: '%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}:%L - %msg%n'
```
application-test.yml
```yaml
# 自定义的参数
myParam: 'on'
```
application-prod.yml
```yaml
# 自定义的参数
myParam: 'close'
```
<a name="rUP4L"></a>
### 获取自定义参数
```java
package com.demo.config;

import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import java.util.Arrays;

@Component
@Slf4j
public class MyParamRunner implements CommandLineRunner {

    /**
     * 自定义测试参数
     */
    @Value("${myParam}")
    String myParam;

    /**
     * 获取环境变量参数
     */
    @Autowired
    Environment environment;

    @Override
    public void run(String... args) throws Exception {
        log.info("本地设置的参数myParam为：{}", myParam);
        log.info(environment.toString());
        log.info(String.valueOf(Arrays.asList(environment.getActiveProfiles())));
    }
}
```
<a name="UdYUP"></a>
### 项目打包发布
打包后springboot-jvm-params-1.0-SNAPSHOT.jar文件存放目录如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661005370720-848d3a9d-41a1-457d-99b8-1dcac310c95e.png#clientId=uc0d55629-2211-4&from=paste&id=ubd1ea984&originHeight=185&originWidth=893&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2de00b90-dd87-46bf-ac0a-85e768d5927&title=)启动jar包进行测试：
```bash
java -jar /usr/local/springboot_demos/springboot-port8001/springboot-jvm-params-1.0-SNAPSHOT.jar
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661005370892-0396dbec-ac97-4af6-a93a-40cc127ccf19.png#clientId=uc0d55629-2211-4&from=paste&id=ue13ff1b3&originHeight=115&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u57681112-6e46-4154-ae85-ddd91beeb9f&title=)
<a name="IRNhl"></a>
## 修改启动配置
<a name="xvv6L"></a>
### 方式一：系统变量
```bash
java -jar -Dspring.profiles.active=prod /usr/local/springboot_demos/springboot-port8001/springboot-jvm-params-1.0-SNAPSHOT.jar
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661005370758-aba2aaaa-0d00-4625-bb1f-d5c6bebf2c3f.png#clientId=uc0d55629-2211-4&from=paste&id=ufbd244eb&originHeight=113&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8d2b48dd-8986-411e-96b0-bc9f6125528&title=)
<a name="KruTl"></a>
### 方式二：命令行参数
```bash
java -jar /usr/local/springboot_demos/springboot-port8001/springboot-jvm-params-1.0-SNAPSHOT.jar --spring.profiles.active=prod --myParam='test'
```
注意以上两种方式的区别，可以在启动参数里修改配置文件中配置的参数。这里是将`myParam='close'`修改为`myParam='test'`<br />如果是下面的配置结果myParam会是什么？
```bash
java -jar -DmyParam='test1' /usr/local/springboot_demos/springboot-port8001/springboot-jvm-params-1.0-SNAPSHOT.jar ---spring.profiles.active=prod --myParam='test2'
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661005370719-19a7b7a1-a077-47a8-a8e5-76378e003655.png#clientId=uc0d55629-2211-4&from=paste&id=u3c40c1cf&originHeight=133&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uecf8d1c9-4ecf-49c3-b168-b080a28810e&title=)由上可以得出，SpringBoot加载参数的优先级：配置文件变量 < JVM系统变量 < 命令行参数（注意：优先级由低到高，非常多的启动命令中传参也是这个道理）
<a name="hLvx3"></a>
## SpringBoot启动参数解释
测试配置的的参数如下：
```bash
/usr/local/jdk/jdk1.8.0_261/bin/java -jar -server \          ## 服务模式，linux默认是server模式，window默认是client参数 
-XX:+HeapDumpOnOutOfMemoryError \              ## 当OOM发生时自动生成Heap Dump文件
-XX:HeapDumpPath=/usr/local/springboot_demos/springboot-port8001/dump/heap/oom.hprof \ ## 指定发生OOM时生成Dump文件存储位置
-Djava.io.tmpdir=/usr/local/springboot_demos/springboot-port8001/tmp/ \     ## 指定操作系统缓存的临时目录
-Dserver.port=8001 \                 ## web服务使用端口
-Dcom.sun.management.jmxremote \              ## 是否支持远程JMX访问，默认true
-Dcom.sun.management.jmxremote.port=5103 \            ## 配置jmx远程connection的端口号，要确认这个端口没有被占用
-Dcom.sun.management.jmxremote.rmi.port=6103 \           ## JMX在远程连接时，会随机开启一个RMI端口作为连接的数据端口
-Dcom.sun.management.jmxremote.authenticate=false \          ## 是否需要开启用户认证，默认开启
-Dcom.sun.management.jmxremote.ssl=false \            ## 是否连接开启SSL加密，默认开启
-Dcom.sun.management.jmxremote.access.file=/usr/local/jdk/jdk1.8.0_261/jre/lib/management/jmxremote.access \ ## 对访问用户的权限授权的文件的路径，默认路径是${JRE_HOME}/lib/management/jmxremote.access
-Xmx256m \                    ## 设置堆最大空间为256m
-Xms256m \                    ## 设置堆最小空间为256m
-XX:+DisableExplicitGC \                ## 禁止手动的system.gc
-Xloggc:/usr/local/springboot_demos/springboot-port8001/logs/springboot-jvm-params_gc.%t.log \ ## gc日志存放的位置
-XX:+PrintHeapAtGC \                 ## HotSpot在GC前后都会将GC堆的概要状况输出到log中
-XX:+PrintTenuringDistribution \              ## 打印Survivor对象年龄分布
-XX:+PrintGCApplicationStoppedTime \             ## 预估垃圾收集"Stop the world"暂停所阻塞的时间
-XX:+PrintGCTaskTimeStamps \               ## 打印gc线程的时间戳
-XX:+PrintGCDetails \                 ## 打印gc详情
-XX:+PrintGCDateStamps \                ## 日志开头显示日期以及时间
-Dserver.connection-timeout=60000 \              ## HTTP请求超时时间
-Dserver.tomcat.accept-count=1000 \              ## 所有可能的请求处理线程正在使用时，传入连接请求的最大队列长度
-Dserver.tomcat.max-threads=300 \              ## 最大工作线程数
-Dserver.tomcat.min-spare-threads=65 \             ## 最小工作线程数
-Dserver.tomcat.accesslog.enabled=false \            ## 启用访问你日志
-Dserver.tomcat.accesslog.directory=/usr/local/springboot_demos/springboot-port8001/logs/ \ ## 日志文件路径
-Dserver.tomcat.accesslog.prefix=access_log \           ## 日志文件名前缀
-Dserver.tomcat.accesslog.pattern=combined \           ## 日志格式
-Dserver.tomcat.accesslog.suffix=.log \             ## 日志文件后缀
-Dserver.tomcat.accesslog.file-date-format=.yyyy-MM-dd          ## 放在日志文件名中的日期格式 
-Dserver.tomcat.accesslog.rotate=true \             ## 是否启用访问日志分割
-Dserver.tomcat.accesslog.rename-on-rotate=true \          ## 推迟在文件名中加入日期表示，直到日志分割时
-Dserver.tomcat.accesslog.request-attributes-enabled=true \        ## 为请求使用的IP地址、主机名、协议和端口设置请求属性
-Dserver.tomcat.accesslog.buffered=true \            ## 缓存日志定期刷新输出（建议设置为true，否则当有请求立即打印日志对服务的响应会有影响）
-XX:NewRatio=4 \                  ## 设置Yang和Old的比例，设置4则Old是Yang的4倍，即Yang占1/5
-XX:SurvivorRatio=8 \                 ## 设置Eden和Suivior的比例，Eden:S0:S1=8:1:1
-XX:MaxTenuringThreshold=15 \               ## 在新生代对象存活次数(经过Minor GC的次数)超过n后，就会晋升到老年代
-XX:TargetSurvivorRatio=90 \               ## 在新生代的对象不一定要满足存活年龄达到MaxTenuringThreshold才能去老年代，当Survivor空间中相同年龄所有对象大小总和大于[Desired survivor size]时，年龄大于或等于该年龄的对象直接进入老年代。[Desired survivor size]=单个survivor大小*TargetSurvivorRatio百分比
-XX:+UseCMSInitiatingOccupancyOnly \                                                    ## 指在使用CMS收集器的情况下，老年代使用了指定阈值的内存时，触发FullGC
-XX:CMSInitiatingOccupancyFraction=70 \             ## 指在使用CMS收集器的情况下，老年代使用达到70%，出发CMS垃圾回收
-XX:ParallelGCThreads=8 \                ## parallel回收的时候可以设置年轻代的并行线程数,取决于cpu核数
-XX:ConcGCThreads=2 \                 ## 设置并行标记的线程数。将n设置为并行垃圾回收线程数(ParallelGCThreads)的 1/4 左右。
-XX:-UseGCOverheadLimit \                ## jvm gc行为中超过98%以上的时间去释放小于2%的堆空间时会报“GC overhead limit exceeded”错误，此参数避免此报错
-XX:+UseParNewGC \                  ## 开启此参数使用ParNew & serial old搜集器（不推荐）使用这个参数后会在新生代进行并行回收
-XX:+UseConcMarkSweepGC \                ## 开启此参数使用ParNew & CMS（serial old为替补）搜集器
-XX:CMSFullGCsBeforeCompaction=1 \              ## 设置在几次CMS垃圾收集后，触发一次内存整理
-XX:+CMSParallelRemarkEnabled \               ## 降低标记停顿
-XX:+CMSScavengeBeforeRemark \               ## 开启或关闭在 CMS-remark 阶段之前的清除（Young GC）尝试
-XX:+ParallelRefProcEnabled \               ## 并行处理Reference，加快处理速度，缩短耗时
-XX:+UseCMSCompactAtFullCollection \             ## 年老代使用CMS，默认是不会整理堆碎片的。设置此配置打开对年老代的压缩，即执行Full GC后对内存进行整理压缩，免得产生内存碎片，但有可能会影响性能。
-XX:CMSMaxAbortablePrecleanTime=6000 \               ## 指定CMS-concurrent-abortable-preclean阶段执行的时间，该阶段主要是执行一些预清理，减少应用暂停的时间
-XX:CompileThreshold=10 \                ## 超过10此进行JTI即时编译
-XX:MaxInlineSize=1024 \                ## 方法体的大小阈值。通过 -XX:CompileThreshold 来设置热点方法的阈值。但要强调一点，热点方法不一定会被 JVM 做内联优化，如果这个方法体太大了，JVM 将不执行内联操作
-Dsun.net.client.defaultConnectTimeout=60000 \           ## socket连接超时时间
-Dsun.net.client.defaultReadTimeout=60000 \            ## socket读取超时时间
-Dnetworkaddress.cache.ttl=300 \              ## JVM的DNS缓存有效期，单位秒
-Dsun.net.inetaddr.ttl=300 \               ## 缓存失败结果，如果在缓存时效内再次lookup时直接返回错误（减轻DNS服务压力）             
-Djsse.enableCBCProtection=false \              ## 关闭jvm中的java修复程序
-Djava.security.egd=file:/dev/./urandom \            ## 加快随机数产生过程
-Dfile.encoding=UTF-8 \                 ## 指定web应用编码
-Dlog.path=/usr/local/springboot_demos/springboot-port8001/logs/ \      ## 指定项目日志文件路径
-Dspring.profiles.active=prod \               ## 指定运行的环境配置
/usr/local/springboot_demos/springboot-port8001/springboot-jvm-params-1.0-SNAPSHOT.jar jvmparams
```
