Java
<a name="ZlZiW"></a>
## 1、Netty
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771601672-f346b0ce-7acc-4731-9751-7c63118aeddc.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=ue8c51adb&originHeight=158&originWidth=318&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue5e3907e-8caa-4aac-96f0-0b25f1aed57&title=)<br />Netty已经成为了目前最流行的Java网络编程框架。有了它，Java才有了与golang等语言相提并论的权利。
<a name="ozzmm"></a>
## 2、SpringBoot
在很多中小公司，Java企业级开发首选SpringBoot，这是没得说的。由于它的应用实在是太广泛。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771601588-1b1b65ef-51a4-4394-9801-b2ae106a9c71.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=ucdd8632d&originHeight=801&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e30b2e2-5d89-4adf-898d-2746595e773&title=)<br />Spring当年的目标，是为了干掉J2EE。屠龙的少年如今变成了龙，现在在干着J2EE当年做的事情。<br />不过，尽量的做的轻量级，一直是它的主旋律，这一点从来没有变过。因为错的不是Spring，错的是这个世界。
<a name="wut45"></a>
## 3、工具类库
由于历史原因，apache-commons工具类库，几乎遍布了项目中的每个角落，比如lang3、beanutils、collections、codec等。个人觉得，apache-commons的很多工具，已经是过度设计了，在一些新项目里，已经很少使用了。<br />取而代之的，是大量使用guava工具类库，以及国产的hutool工具类库。有了这些工具，就不用再在项目里写一些拼拼凑凑，让人难受的小工具类了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771601604-c1e71026-f521-4f63-8c7f-75b9d32a10f0.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=uc0a1ec56&originHeight=300&originWidth=580&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8ff17203-5567-42ea-aa99-e81473753f5&title=)<br />说点有意思的事情，guava这个词朗朗上口，它的中文意思是一种热带水果：番石榴，长的就像是小西瓜一样。
<a name="ggt3S"></a>
## 4、单元测试
JUnit几乎成了Java单元测试的标配。JUnit5更是对整体模块进行了重构。现在，它包含3个主要的模块：JUnit Jupiter、JUnit Platform、JUnit Vintage等。<br />Vintage是为了兼容比较老的版本而存在的，Platform是为了兼容其他测试引擎，这样在JUnit上使用Mockito也成为了可能。如果项目比较新，建议直接上Jupiter。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771601520-54db9c77-cdfb-4d55-8355-f987c537460e.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=u73b05b79&originHeight=581&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc25ee2e5-9b16-4a8a-bb0c-66fb78d0550&title=)<br />很多国外系统对单元测试看的比较重，这和国内的开发模式是不太一样的。虽然说，单元测试是一个程序员的基本功，但离着普及还有一段距离。不过现在也有很多公司强制要求写单元测试，以任务指标的形式存在。
<a name="jY5Dz"></a>
## 5、JMH
JMH是最装逼，最牛逼的基准测试工具套件，主要用在性能测试方面。如果定位到了热点代码，要测试它的性能数据，评估改善情况，就可以交给 JMH。它的测量精度非常高，可达纳秒级别。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771601703-45e6ede7-2590-4224-823e-8a8c13b9a200.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=ub789b874&originHeight=300&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2970d46a-feb4-409c-9cf9-4252b72af9b&title=)<br />它能够消除JIT的差异，测试出真正的代码性能。JMH 已经在 JDK 12中被包含，其他版本的需要自行引入 maven。
<a name="JnK4G"></a>
## 6、OkHTTP
最新的JDK已经内置了HTTP的功能，但这种协议层面的东西，放在基础类库里老感觉不是那么妥当。<br />很长一段时间里，Apache 的HttpClient统治了世界，甚至重构后在不同版本之间不能够做到兼容。更喜欢轻量级的OkHTTP多一些，第一次见它还是在Android的应用代码里，现在用在应用代码里也很香。OKHTTP的透明压缩，显得也更加智能一些。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602103-2f9b0f37-2847-4157-adb2-36a4cf631d12.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=u006fad31&originHeight=280&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubbb4991e-694d-40ec-9ae8-13be4a8c127&title=)<br />像SpringCloud这样的组件，在底层是可以选择切换成HttpClient还是OkHTTP的。<br />应用层的协议变动都比较大，更新也比较快。比如HTTP2，Quic等支持，显然要对整个类库做很多的修改才能适配。
<a name="fU072"></a>
## 7、数据库连接池
SpringBoot2默认选择了hikaricp作为连接池，据说是速度最快的连接池，而且代码量非常的精简。c3p0、dbcp等老旧的数据库连接池已经慢慢退位。<br />hikaricp非常的卷，它采用无锁化的思想，核心类库只有ConcurrentBag一个，甚至使用Javassist修改字节码来增加执行速度。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771601964-e9922dd6-723b-48bc-8b79-9cd9dafc86a5.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=u7be6e484&originHeight=265&originWidth=520&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3bb55e95-343e-4091-9a0d-3af100f1c86&title=)<br />在国内，还有一个使用较为广泛的连接池是Druid，它有着完整的监控功能。通过开启后台可以直接在web端查看整个连接池的状态。
<a name="jduKe"></a>
## 8、caffeine
堆内缓存，首选caffeine。<br />它结合了LRU和LFU，两者合体之后，变成了新的W-TinyLFU算法，命中率非常高，内存占用也更加的小。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602093-4d393a52-4b8a-421c-a718-742a0a9cb8c3.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=u1b6413e5&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u26893c95-72b8-4f7e-ae88-b29128679ab&title=)<br />在SpringBoot里，通过提供一个`CacheManager`的Bean，即可与Springboot-cache进行集成，可以说是很方便了。
<a name="K8T84"></a>
## 9、hazelcast
说完了堆内的，就再说个堆外的。<br />除了有redis这种选择之外，还可以选择hazelcast。hazelcast采用raft算法进行分布式协调，在一致性方面强于redis。据hazelcast测试，在达到一定规模的时候，速度竟然比redis还快，hazelcast和redis甚至有过激烈的口水战。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602068-97a180d9-9db9-47ca-9180-55625a15e04b.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=uf5736978&originHeight=138&originWidth=417&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uef60670b-2071-43ef-a223-10cac69aaa6&title=)<br />hazelcast可以以jar包的方式集成在Java应用中，自身同时作为客户端和服务端，组件多节点的集群。它提供了类似redisson一样的功能齐全的分布式数据结构，比如Queue，只需要直接在Java代码中调用相应的API，它就能把数据同步到所有的节点上去。
<a name="mlyeY"></a>
## 10、日志组件
日志组件是常用的类库，最早得到广泛使用的是log4j，而现阶段比较流行的是slf4j+logback。由于这些类库非常的好用，JDK自带的JUL反而无人问津。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602235-ef524432-d1ef-48aa-858d-559e9a064878.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=ua1f8e4b7&originHeight=575&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua18fd2d7-0566-41df-bdab-0f5fd4e740f&title=)<br />slf4j是门面模式的典型应用，它本身是一套接口，背后可以使用logback和log4j。这些关系，能够从上面的图很容易的看出来。
<a name="nflEW"></a>
## 11、JSON、XML、YAML解析
由于Fastjson频频爆出漏洞，现在在Java中使用最广泛的JSON解析类库，就是jackson库。它最主要的入口工具类是`ObjectMapper`，解析方法是线程安全的，可以在整个项目中共享一个解析对象。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602357-fae53801-d33c-48cb-9a61-5dd128a666c0.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=uf338b118&originHeight=599&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1d0623f4-64e9-4460-9d53-fa607f7130b&title=)<br />jackson库不仅仅可以解析JSON，它还可以解析XML、YAML、TOML等。事实上，它的抽象接口，可以实现Avro、Protobuf、CSV等格式数据的加载，但最长使用的，就是它的JSON解析。
<a name="R28vp"></a>
## 12、Jolokia
J2EE当年的目的肯定是好的，但总是做一些又大又笨重的东西。JMS算一个，JMX也算一个。<br />JMX其实是非常不好用的，因为它把功能隔离在了Java体系之内。现在的Promethus监控系统就做的比较好，一切都是HTTP交互的文本格式。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602436-607da1f5-e913-4d29-9b8c-4be7978185b8.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=u6aa24994&originHeight=262&originWidth=610&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u61e2abb9-77c6-49a6-8467-9e573fff736&title=)<br />Jolokia就可以提供JMX的远程访问方法，把JMX接口给强制转化成HTTP接口，以便于其他的监控系统进行功能对接。从这个描述上来看，Jolokia就是一个适配器。不过，通过Agent或者Jar包的方式，它能让ActiveMQ，JBoss等这些老掉牙的系统接入到现代化的监控系统中来，也算是大功一件。
<a name="GKQuY"></a>
## 13、hibernate-validator
hibernate留下了一个很棒的遗产，那就是它的验证框架，它是Bean Validation 的参考实现，被广泛的应用于数据库模型校验、参数校验等领域。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602403-b1c8cdf2-2184-4a70-b0aa-eaeed45b1b79.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=u8b64ed91&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue3015b2b-3013-4c85-ba68-8d28c3470d8&title=)<br />在JPA中，`@Table`的字段可以根据这些验证，自动生成数据库约束。在Spring的Controller中，也可以使用`@Valid`注解，来自动对传入的对象进行参数验证。
<a name="JmeKy"></a>
## 14、freemarker
FreeMarker 是一款模板引擎，在很早之前，主要用它来生成Web页面。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638771602440-41158e88-983c-48d0-8f22-718d71573c90.webp#clientId=u9ec0561c-fc2f-4&from=paste&id=ub2230b66&originHeight=180&originWidth=435&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc9452b8b-09dc-473d-9fda-0f57d689a4b&title=)<br />不过，随着前后端分离的流行，现在主要在一些代码生成器工具中用到它。它的应用比较广泛，比如做一个邮件模板，短信模板之类的。如果写过JSP的话，一定会知道它做的功能是类似的。<br />在Spring中，还有一个更加轻量级的模板引擎，叫做SPEL--一个表达式语言。在`@Value`注解中所使用的语法就是它，它们都是类似的技术。
