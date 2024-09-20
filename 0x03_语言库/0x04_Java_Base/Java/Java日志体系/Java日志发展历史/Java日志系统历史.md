Java
<a name="sZaJ2"></a>
## 发展历史
<a name="R5x0n"></a>
### `System.out`和`System.err`
这应该是最早的日志记录方式，但是不灵活也不可配置，要么就是全部打印，要么就是全部不打印，没有一个统一的日志级别
<a name="k72ZF"></a>
### Log4j
在1996年初，E.U.SEMPER（欧洲安全电子市场）项目决定编写自己的跟踪API，最后该API演变为Log4j，Log4j日志软件包一经推出就备受欢迎，当然这里必须要提到一个人，就是Log4j的主要贡献者，这个大佬：
<a name="f7xQy"></a>
### Ceki Gülcü
后来Log4j成为了Apache基金会项目中的一员，同时Log4j的火爆，让Log4j一度成为业内日志标杆。（据说Apache基金会还曾经建议Sun引入Log4j到java的标准库中，但是sun拒绝了）
<a name="QaQ43"></a>
### JUL（Java Util Logging）
果然Sun有自己的考虑，2002年2月Java1.4发布，Sun竟然推出了自己的日志库Java Util Logging，其实很多日志的实现思想也都是仿照Log4j，毕竟Log4j先出来很多年了，已经很成熟了此时，这两个日志工具打架，显然Log4j是更胜一筹<br />它们打架感觉就是互相竞争，Sun心里可能在想，不就是做个日志工具嘛，谁不会！当然好景不长
<a name="QgiF4"></a>
### JCL（Jakarta Commons Logging）
2002年8月Apache又推出了日志接口Jakarta Commons Logging，也就是日志抽象层，当然也提供了一个默认实现Simple Log，这野心很大，想一统日志抽象（就像以前的JDBC一统数据库访问层），让日志产品去实现它的抽象，这样只要日志代码依赖的是JCL的接口，就可以很方便的在Log4j和JUL之间做切换，当时日志领域大概是这样的结构，当然也还是方便理解的，也很优雅<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422673579-ef3cdf1f-bd4f-4cb2-9416-57f094bef037.webp#clientId=u5b063754-cb57-4&from=paste&id=u78efdebe&originHeight=462&originWidth=414&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue0f9d58b-c384-4758-9e44-34b57419b0f&title=)<br />但是好景不长，在使用过程中，虽然现在日志系统在JCL的统一下很优雅，很美好，但大家发现了JCL还不够好，有些人甚至认为JCL造成的问题比解决的问题还多...emmm<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422673613-8954266d-3556-47bb-a4b8-1c28b370fa54.webp#clientId=u5b063754-cb57-4&from=paste&id=u9be9b1cb&originHeight=169&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f398c25-d314-48e1-bbf2-7152f4ac838&title=)
<a name="sqkS8"></a>
### Slf4j（Simple Logging Facade for Java）
**Ceki Gülcü**（也就是Log4j的作者）由于一些原因离开了Apache，之后觉得JCL不好，于是于2005年自己撸出一个新工程，也就是一套新日志接口（有得也叫日志门面）：Slf4j（Simple Logging Facade for Java）。<br />但是由于Slf4j出来的较晚，而且还只是一个日志接口，所以之前已经出现的日志产品，如JUL和Log4j都是没有实现这个接口的，所以尴尬的是光有一个接口，没有实现的产品也是很憋屈，就算开发者想用Slf4j也是用不了，这时候，大佬发话了**Ceki Gülcü**：别急，早想好了，要让Sun或者Apache这两个庞然大物来实现接口太难了，但我帮你们实现，不就完了么。<br />于是大佬**Ceki Gülcü**撸出了之前提到的桥接包，也就是这种类似适配器模式<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422673599-204a4e4e-1c8d-4669-98d4-a0df641965d8.webp#clientId=u5b063754-cb57-4&from=paste&id=udf4677b5&originHeight=476&originWidth=297&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc1809173-2b6c-4d54-8ffa-3216ed15de2&title=)<br />好了，大佬提供了桥接包，于是日志系统现在有了这样的结构<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422673780-00eee7de-12d7-4b8a-8cb5-552255b8f234.webp#clientId=u5b063754-cb57-4&from=paste&id=u0d12ef56&originHeight=331&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udcb3cd59-345c-4644-b294-8d7780f7c42&title=)<br />但是其实之前很多Java应用应该依赖的JCL，所以光有日志产品桥接包，好像还不够**Ceki Gülcü**：没问题，不就是不够桥接包么，我写，我来证明Slf4j是最完美的 于是有了JCL的桥接包<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422673916-7ef608a9-8ca9-4ba4-92d3-8a2129cfaab1.webp#clientId=u5b063754-cb57-4&from=paste&id=u4a978d5f&originHeight=448&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9b6ca9a4-a4d5-4c96-98e6-a6177b205a3&title=)<br />相当于此时的桥接包就是分了两种场景

1. 之前Java应用用的日志接口（如JCL）
2. 之前Java应用用的日志产品（如Log4j）

那好，那如果再考虑一下这种场景呢？假设Java应用使用了Spring的第三方的框架，但是假设Spring默认用JCL，并且最终用的JUL打印的日志，但是系统使用了Slf4j作为日志接口，日志产品使用了Log4j，那不出意外的话将有两种日志输出，两种日志的打印方式不统一，到时候解决bug的时候就很恼火，而且配置日志的配置文件还需要两份。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422673942-03476fa0-d120-40b5-b6a2-04dea5a06ffa.webp#clientId=u5b063754-cb57-4&from=paste&id=u3ef327b6&originHeight=459&originWidth=562&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u395b6273-e2ae-482a-af38-0a569365123&title=)<br />所以为了方便统一应用中的所有日志，大佬发话了**Ceki Gülcü**：没事，大家都选择用Slf4j统一吧，我来帮大家统一，没有事是桥接包解决不了的，有的话，那就再来个<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422674010-80936596-6884-4ccf-b7f6-957c6972dced.webp#clientId=u5b063754-cb57-4&from=paste&id=u776a23f6&originHeight=427&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf0efb7f8-ef20-42d4-bbee-394cf33dba5&title=)<br />当然此时这种场景也是符合之前说的两种情况的，因此现在日志系统大体应该是这样的<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422674139-38c0972d-0288-4333-958e-a435938a473b.webp#clientId=u5b063754-cb57-4&from=paste&id=ue5fdcca9&originHeight=563&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6d714208-2b33-4292-8ab8-0c51ef2a641&title=)<br />但好景又不长，大佬毕竟是大佬，Log4j不就是自己写的么，所以最清楚Log4j缺点的人也正是他
<a name="bkyIq"></a>
## Logback
由于使用Slf4j，需要一次桥接包，也就是之前的日志产品都不是正统的Slf4j的实现，因此，2006年，出自**Ceki Gülcü**之手的日志产品Logback应运而生，而且大佬还专门写了一篇文章<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422674248-b079a940-bc1d-4bf7-8c6a-47114272ff94.webp#clientId=u5b063754-cb57-4&from=paste&id=u1ed588b3&originHeight=62&originWidth=539&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud0418190-8040-41c3-8c31-88268f2d3e0&title=)<br />Logback是完美实现了Slf4j，于是现在日志系统变成了<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422674348-0e076b7c-f004-47f5-91bb-2852e120a554.webp#clientId=u5b063754-cb57-4&from=paste&id=u14d06ef7&originHeight=635&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udc2f4018-0518-4f29-85d2-b860bc05715&title=)<br />现在有了2个日志接口，3个日志产品，大家也都看起来相安无事。但Slf4j+Logback的模式，显然很冲击JCL+Log4j，并且本身Logback确实比Log4j性能更优，设计更为合理，所以老东家Apache可就坐不住了
<a name="jppXT"></a>
## Log4j2
在2012年，Apache直接推出新项目，不是Log4j1.x升级，而是新项目Log4j2，因为Log4j2是完全不兼容Log4j1.x的<br />并且很微妙的，Log4j2几乎涵盖Logback所有的特性，更甚者的Log4j2也搞了分离的设计，分化成log4j-api和log4j-core，这个log4j-api也是日志接口，log4j-core才是日志产品。<br />现在有了3个日志接口，以及4个日志产品。当然Apache也知道该做啥，为了让大家可以接入自己的Log4j2，Apache也推出了它的桥接包。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642422674515-2ef84fe5-d8de-44c8-8ac2-afda11a118ae.webp#clientId=u5b063754-cb57-4&from=paste&id=u8c7c49ac&originHeight=636&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u85fdbb0a-a982-4041-a411-cce98ec7ac8&title=)<br />总结到这，可以吸取什么经验么？

1. 不写接口的坏处
2. 没有什么问题是加一个层适配器（接口）解决不了的，如果有，那就再加一层
<a name="HCwUw"></a>
## 最佳实践
了解了日志的发展历史，那现在再回过头来看看如果，系统在选择日志方案的时候，如何抉择呢？毕竟3个日志接口，以及4个日志产品

- 显然第一点是使用日志接口的API而不是直接使用日志产品的API这一条也是必须的，也是符合依赖倒置原则的，应该依赖日志的抽象，而不是日志的实现
- 日志产品的依赖只添加一个 当然也这个也是必须的，依赖多个日志产品，只会让自己的应用处理日志显得更复杂，不可统一控制
- 把日志产品的依赖设置为`Optional`和runtime scope其中Optional是为了依赖不会被传递，比如别的人引用这个jar，就会被迫使用不想用的日志依赖
```xml
<dependency>
  <groupId>org.apache.logging.log4j</groupId>
  <artifactId>log4j-core</artifactId>
  <version>${log4j.version}</version>
  <optional>true</optional>
</dependency>
```
而`scope`设置为`runtime`，是可以保证日志的产品的依赖只有在运行时需要，编译时不需要，这样，开发人员就不会在编写代码的过程中使用到日志产品的API了
```xml
<dependency>
  <groupId>org.apache.logging.log4j</groupId>
  <artifactId>log4j-slf4j-impl</artifactId>
  <version>${log4j.version}</version>
  <scope>runtime</scope>
</dependency>
```
