Java 日志<br />Java日志体系可以说是五花八门，眼花缭乱。<br />导致很多小伙伴因为日志标准库之间复杂的关系而感到烦恼，不知道统一系统的日志标准库需要依赖哪些jar包。
<a name="AolLV"></a>
# 发展史
要正确的配置好日志，让jar相互生效，就要先理清关系，理清关系就得从它的发展史下手。
<a name="sdYP6"></a>
## `System.out`和`System.err`
2001年以前，Java是没有日志库的，打印日志全凭`System.out`和`System.err`。<br />![2021-06-01-21-34-58-751788.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554630510-953c78e4-0b8a-4a01-bcc1-521e0289e7c1.png#clientId=u9f4613dd-e3eb-4&from=ui&id=uc03de523&originHeight=426&originWidth=817&originalType=binary&size=167259&status=done&style=shadow&taskId=uec35e84d-2c97-41ee-811f-1687aa79db9)<br />缺点如下：

- 产生大量的IO操作   同时在生产环境中 无法合理的控制是否需要输出
- 输出的内容不能保存到文件
- 只打印在控制台，打印完就过去了，也就是说除非一直盯着程序跑
- 无法定制化，且日志粒度不够细
<a name="iAUjs"></a>
## Log4j
此时名为Ceki的巨佬站出来，说这个不好用，接着在2001年掏出了Log4j，用起来也确实比`System`系列香，Log4j一度成为业内日志标准。<br />![2021-06-01-21-34-59-030050.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554663491-054520d5-bed1-43c9-9765-b63efeb4bb89.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u98215524&originHeight=372&originWidth=774&originalType=binary&size=151752&status=done&style=shadow&taskId=ub2b39e98-0bf7-4858-9a6f-49538d427ab)<br />后来Log4j成为Apache项目，Ceki也加入Apache组织（据说Apache还曾经建议Sun引入Log4j到Java的标准库中，但Sun拒绝了）。
<a name="loA2N"></a>
## JUL（Java Util Logging）
原来Sun自己也搞一个，2002年2月JDK1.4发布，Sun推出了自己的日志标准库JUL（Java Util Logging），其实是照着Log4j抄的，而且还没抄好，还是在JDK1.5以后性能和可用性才有所提升。<br />因为在JUL出来以前，Log4j就已经成为一项成熟的技术，使得Log4j在选择上占据了一定的优势。
<a name="HWdut"></a>
## JCL（Jakarta Commons Logging）
现在市面上有两款Java日志标准库，分别是Log4j与JUL，此时Apache组织十分有想法，想统一抽象日志标准接口规范（就像JDBC统一数据库访问层），让其他日志标准库去实现它的抽象接口，这样日志操作都是统一的接口。<br />![2021-06-01-21-34-59-279377.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554698075-aa64168d-a11b-4567-8aab-fae8b4899d4b.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u8e95b632&originHeight=555&originWidth=588&originalType=binary&size=137872&status=done&style=shadow&taskId=u75361f3a-e009-4c94-8892-27a76f67e0f)<br />于是JUL刚出来不久，2002年8月Apache推出了JCL（Jakarta Commons Logging），也就是日志抽象层，支持运行时动态加载日志组件的实现，当然也提供一个默认实现Simple Log（在ClassLoader中进行查找，如果能找到Log4j则默认使用log4j实现，如果没有则使用JUL 实现，再没有则使用JCL内部提供的Simple Log实现）。<br />![2021-06-01-21-34-59-576376.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554714999-7818d591-9b10-4e1a-94ae-e0b34a25f218.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u2dc0d718&originHeight=410&originWidth=894&originalType=binary&size=148869&status=done&style=shadow&taskId=u9ce2358e-bc98-4482-b1a9-c59cda833f4)<br />但是JUL有三个缺点

1. 效率较低
2. 容易引发混乱
3. 使用了自定义ClassLoader的程序中，使用JCL会引发内存泄露

总之就是问题也挺多~
<a name="EW0zx"></a>
## Slf4j（Simple Logging Facade for Java）
2006年巨佬Ceki（Log4j的作者）因为一些原因离开了Apache组织，之后Ceki觉得JCL不好用，自己撸了一套新的日志标准接口规范Slf4j（Simple Logging Facade for Java），也可以称为日志门面，很明显Slf4j是对标JCL，后面也证明了Slf4j比JCL更优秀。<br />![2021-06-01-21-35-01-514751.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554731685-01539e8a-109b-4ed6-85b4-bc092174b6bb.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u94c1b122&originHeight=372&originWidth=859&originalType=binary&size=179045&status=done&style=shadow&taskId=u449c31c2-e4d9-462b-9889-ed478bd1e70)<br />由于Slf4j出来的较晚，光有一个接口，没有实现的日志库也很蛋疼，如JUL和Log4j都是没有实现Slf4j，就算开发者想用Slf4j也用不了。<br />![2021-06-01-21-35-04-284899.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554745459-6009b64a-606a-4fe3-8f04-44ab17b3c377.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u8b34fc58&originHeight=582&originWidth=1080&originalType=binary&size=220092&status=done&style=none&taskId=u3e54055d-b64e-4088-8e98-0999ba9e515)<br />这时候巨佬Ceki发话了，Sum和Apache这两个组织不来实现我的接口没关系，我来实现就好了，只有魔法才能打败魔法。<br />后面巨佬Ceki提供了一系列的桥接包来帮助Slf4j接口与其他日志库建立关系，这种方式称为桥接设计模式。<br />![2021-06-01-21-35-04-662662.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554764930-660a49b9-3c6a-41c2-8035-acb1612de929.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u004e3613&originHeight=584&originWidth=1080&originalType=binary&size=221997&status=done&style=shadow&taskId=u895273bf-3d80-4f0a-bfe3-971b5f1e772)<br />有了桥接包配合，其他的问题都迎刃而解，先看看有那些问题吧~<br />![2021-06-01-21-35-05-219143.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554778187-4f570282-0779-40be-91f5-31609e90d5bf.png#clientId=u9f4613dd-e3eb-4&from=ui&id=ucdd5d361&originHeight=712&originWidth=944&originalType=binary&size=336780&status=done&style=shadow&taskId=ud7e27fbb-b371-44be-b198-9a7d6b22d36)<br />从上图可以看出，不同时期的项目使用的日志标准库不一样，以Slf4j接口作为划分线，考虑两个问题，一个是Slf4j之前的项目怎么统一日志标准，另一个是Slf4j之后的项目怎么统一日志标准。<br />先来看Slf4j之后的项目怎么统一日志标准，项目D、E都使用Slf4j接口，首先在代码层已经统一了，如果要做到日志标准统一也十分简单，直接替换日志标准库与对应的桥接包即可，就如下图所示<br />![2021-06-01-21-35-09-411035.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554803835-7ccedc59-8d71-4acb-9b6a-12cd0857de37.png#clientId=u9f4613dd-e3eb-4&from=ui&id=uc71f96c8&originHeight=499&originWidth=1080&originalType=binary&size=407889&status=done&style=shadow&taskId=u6a974b62-97b6-456b-a5a7-db3a12a62e0)<br />好家伙，Slf4j接口配合桥接包简直无敌了，灵活配置。。<br />再来看Slf4j之前的项目怎么统一日志标准，项目A、B、C都使用了不同的日志标准，所以它们的API不一样，如果要统一标准，首先就要改动代码，这样侵入太强了，难道就没有办法在不改代码的情况下，让A、B、C项目统一日志标准吗？<br />办法当然有，Slf4j接口能通过桥接包勾搭上具体的日志标准库，为什么日志标准库不能通过桥接包勾搭Slf4j接口呢？<br />想把A、B、C项目都统一成Log4j日志输出，只需要做如下调整<br />![2021-06-01-21-35-10-709593.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554817667-e6da73fd-8385-4193-aa09-34eedfc3d2cb.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u233009a4&originHeight=747&originWidth=1080&originalType=binary&size=601219&status=done&style=shadow&taskId=uff952262-944c-430a-8dc9-80414ac0526)<br />是不是很简单，引入Slf4j与相关的桥接包，再引入具体的日志标准库，比如Log4j，就完成了3个项目的统一日志标准，对代码层是零入侵。
<a name="H6tmU"></a>
## Logback
Ceki巨佬觉得市场上的日志标准库都是间接实现Slf4j接口，也就是说每次都需要配合桥接包，因此在2006年，Ceki巨佬基于Slf4j接口撸出了Logback日志标准库，做为Slf4j接口的默认实现，Logback也十分给力，在功能完整度和性能上超越了所有已有的日志标准库。<br />目前Java日志体系关系图如下<br />![2021-06-01-21-35-14-902514.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554834519-a16d704d-6853-4f42-bfc9-2825123679e5.png#clientId=u9f4613dd-e3eb-4&from=ui&id=u10c29eee&originHeight=937&originWidth=1080&originalType=binary&size=761652&status=done&style=shadow&taskId=u443d6e11-9a51-4095-ab7d-f82d824bcc2)
<a name="KW9FB"></a>
## Log4j2
自从Logback出来后，可以说Slf4j+Logback组合如日中天，很冲击JCL+Log4j组合，Apache眼看有被Logback反超的势头。<br />于2012年，Apache直接推出新项目Log4j2（不兼容Log4j），Log4j2全面借鉴Slf4j+Logback（十分明显的抄袭嫌疑）。<br />因为Log4j2不仅仅具有Logback的所有特性，还做了分离设计，分为log4j-api和log4j-core，log4j-api是日志接口，log4j-core是日志标准库，并且Apache也为Log4j2提供了各种桥接包。。。<br />到目前为止Java日志体系被划分为两大阵营，分别是Apache阵营和Cekij阵营，如下图所示<br />![2021-06-01-21-35-17-500600.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554856331-4c0ae234-0a5f-4c75-babb-25465c0abd66.png#clientId=u9f4613dd-e3eb-4&from=ui&id=ucce12300&originHeight=767&originWidth=1080&originalType=binary&size=299217&status=done&style=shadow&taskId=ub497b302-19a9-45fe-a13f-ccbd68be649)<br />至于系统中用那套体系，自行选择。
<a name="yDs4a"></a>
# Slf4j的桥接包介绍
相信大家都对桥接包都有了基本概念，这里罗列下与Slf4j配合使用的桥接包<br />Slf4j转向某个日志标准库

- slf4j-jdk14.jar
   - Slf4j到JUL的桥梁
- slf4j-log4j12.jar
   - Slf4j到Log4j的桥梁
- log4j-slf4j-impl.jar
   - Slf4j到Log4j2的桥梁
- slf4j-jcl.jar
   - Slf4j到JCL的桥梁

某个实际日志框架转向Slf4j

- jul-to-slf4j.jar
   - JUL到Slf4j的桥梁
- log4j-over-slf4j.jar
   - Log4j到Slf4j的桥梁
- jcl-over-slf4j.jar
   - JCL到Slf4j的桥梁
<a name="SrgFa"></a>
# 小小实践
Java开发的伙伴们都清楚，Spring框架内部使用JCL做日志输出标准，可是项目使用Slf4j + Logback做日志输出标准，问题来了，怎样才能让项目内的Spring保持统一日志输出标准呢？<br />其实非常简单，只需要引入正确的Slf4j桥接包，去除无用的日志组件即可。<br />![2021-06-01-21-35-17-921224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622554881200-10abc97f-2cef-4198-a077-972973ece407.png#clientId=u9f4613dd-e3eb-4&from=ui&id=uf4e2d37d&originHeight=730&originWidth=632&originalType=binary&size=287183&status=done&style=shadow&taskId=u570b01f9-e9ea-4682-b17e-a7024a8c960)<br />引入jcl-over-slf4j.jar或jul-to-slf4j.jar问题就解决了。
