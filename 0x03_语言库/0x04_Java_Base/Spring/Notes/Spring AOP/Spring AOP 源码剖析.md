JavaSpringAOP<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087162-d0b2e6ac-0b70-4ad5-be3f-8fe0e4aa7d7a.png#clientId=udabac078-7e79-4&from=paste&id=u26c70881&originHeight=1202&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81a18670-cf83-49d7-8fa5-bfcd63aa3d7&title=)
<a name="qM5uw"></a>
## 1、基础知识
<a name="zg4yg"></a>
### 1.1 什么是 AOP ？
AOP 的全称是 “Aspect Oriented Programming”，即**面向切面编程**。<br />在 AOP 的思想里面，周边功能（比如性能统计，日志，事务管理等）被定义为**切面**，核心功能和切面功能分别独立进行开发，然后**把核心功能和切面功能“编织”在一起，这就叫 AOP。**<br />AOP 能够将那些与业务无关，却为业务模块所共同调用的逻辑封装起来，便于减少系统的重复代码，降低模块间的耦合度，并有利于未来的可拓展性和可维护性。
<a name="FdMUG"></a>
### 1.2 AOP 基础概念

- **连接点(Join point)**：能够被拦截的地方，Spring AOP 是基于动态代理的，所以是方法拦截的，每个成员方法都可以称之为连接点；
- **切点(Poincut)**：每个方法都可以称之为连接点，具体定位到某一个方法就成为切点；
- **增强/通知(Advice)**：表示添加到切点的一段逻辑代码，并定位连接点的方位信息，简单来说就定义了是干什么的，具体是在哪干；
- **织入(Weaving)**：将增强/通知添加到目标类的具体连接点上的过程；
- **引入/引介(Introduction)**：允许向现有的类添加新方法或属性，是一种特殊的增强；
- **切面(Aspect)**：切面由切点和增强/通知组成，它既包括了横切逻辑的定义、也包括了连接点的定义。

上面的解释偏官方，下面用“方言”再给大家解释一遍。

- 切入点（Pointcut）：在哪些类，哪些方法上切入（**where**）；
- 通知（Advice）：在方法执行的什么时机（**when**：方法前/方法后/方法前后）做什么（**what**：增强的功能）；
- 切面（Aspect）：切面 = 切入点 + 通知，通俗点就是在什么时机，什么地方，做什么增强；
- 织入（Weaving）：把切面加入到对象，并创建出代理对象的过程，这个由 Spring 来完成。

5 种通知的分类：

- **前置通知(Before Advice)**：在目标方法被调用前调用通知功能；
- **后置通知(After Advice)**：在目标方法被调用之后调用通知功能；
- **返回通知(After-returning)**：在目标方法成功执行之后调用通知功能；
- **异常通知(After-throwing)**：在目标方法抛出异常之后调用通知功能；
- **环绕通知(Around)**：把整个目标方法包裹起来，在被调用前和调用之后分别调用通知功能。
<a name="keGwi"></a>
### 1.3 AOP 简单示例
新建 Louzai 类：
```java
@Data
@Service
public class Louzai {

    public void everyDay() {
        System.out.println("睡觉");
    }
}
```
添加 LouzaiAspect 切面：
```java
@Aspect
@Component
public class LouzaiAspect {
    
    @Pointcut("execution(* com.java.Louzai.everyDay())")
    private void myPointCut() {
    }

    // 前置通知
    @Before("myPointCut()")
    public void myBefore() {
        System.out.println("吃饭");
    }

    // 后置通知
    @AfterReturning(value = "myPointCut()")
    public void myAfterReturning() {
        System.out.println("打豆豆。。。");
    }
}
```
applicationContext.xml 添加：
```xml
<!--启用@Autowired等注解-->
<context:annotation-config/>
<context:component-scan base-package="com" />
<aop:aspectj-autoproxy proxy-target-class="true"/>
```
程序入口：
```java
public class MyTest {
    public static void main(String[] args) {
        ApplicationContext context =new ClassPathXmlApplicationContext("classpath:applicationContext.xml");
        Louzai louzai = (Louzai) context.getBean("louzai");
        louzai.everyDay();
    }
}
```
输出：
```
吃饭
睡觉
打豆豆。。。
```
这个示例非常简单，“睡觉” 加了前置和后置通知，但是 Spring 在内部是如何工作的呢？
<a name="iktzv"></a>
### 1.4 Spring AOP 工作流程
为了方便大家能更好看懂后面的源码，先整体介绍一下源码的执行流程，让大家有一个整体的认识，否则容易被绕进去。<br />整个 Spring AOP 源码，其实分为 3 块，会结合上面的示例，给大家进行讲解。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087076-0e5b7881-75ac-4db0-a94c-4be8d17af4af.png#clientId=udabac078-7e79-4&from=paste&id=uee249f4d&originHeight=1366&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6cc5c223-e543-4e33-b895-9fab7b3d94c&title=)<br />第一块就是**前置处理**，在创建 Louzai Bean 的前置处理中，会遍历程序所有的切面信息，然后**将切面信息保存在缓存中**，比如示例中 LouzaiAspect 的所有切面信息。<br />第二块就是**后置处理**，在创建 Louzai Bean 的后置处理器中，里面会做两件事情：

- **获取 Louzai 的切面方法**：首先会从缓存中拿到所有的切面信息，和 Louzai 的所有方法进行匹配，然后找到 Louzai 所有需要进行 AOP 的方法。
- **创建 AOP 代理对象**：结合 Louzai 需要进行 AOP 的方法，选择 Cglib 或 JDK，创建 AOP 代理对象。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087061-592bdfef-f018-4e2e-a1d5-3f7266141f4e.png#clientId=udabac078-7e79-4&from=paste&id=uc59170f6&originHeight=546&originWidth=1074&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50f6c14f-c9e1-487b-befd-5b7f38a7324&title=)<br />第三块就是**执行切面**，通过“责任链 + 递归”，去执行切面。
<a name="G00Xe"></a>
## 2、源码解读
注意：Spring 的版本是 5.2.15.RELEASE！<br />除了原理部分，上面的知识都不难，下面才是重头戏，一起来走一遍代码流程。
<a name="lCVRS"></a>
### 2.1 代码入口
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087020-d9a73524-1892-45bc-97c5-2e92fcdc164b.png#clientId=udabac078-7e79-4&from=paste&id=u91936164&originHeight=874&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u09828ecc-3d46-4196-857b-5f803bf44d5&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087025-43d4f320-ee31-400a-9bdd-65a0c8db1d66.png#clientId=udabac078-7e79-4&from=paste&id=u6aab76ea&originHeight=666&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13a7ef9f-c4af-40f8-93b8-d2ebf2571ff&title=)<br />这里需要多跑几次，把前面的 beanName 跳过去，只看 louzai。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087713-37dd7ac8-a4a6-493a-a54b-4dbbc0a8e823.png#clientId=udabac078-7e79-4&from=paste&id=ucfcba7ac&originHeight=800&originWidth=996&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8592e074-a521-4a30-8126-da15eedbeea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087941-2440e374-31f7-48e6-a46c-65e3703eeb03.png#clientId=udabac078-7e79-4&from=paste&id=ud7ead8e9&originHeight=101&originWidth=860&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4119c5a9-0564-4638-a717-33808361343&title=)<br />进入 `doGetBean()`，进入创建 Bean 的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142087949-29208296-5ab9-46a1-a8e3-9d2359d842bf.png#clientId=udabac078-7e79-4&from=paste&id=u6560453e&originHeight=322&originWidth=950&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf87eee74-bd73-48d0-9361-713bea074c5&title=)
<a name="xmv8L"></a>
### 2.2 前置处理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142088260-3aa2ef6e-8975-452f-b5e3-b7e377634a02.png#clientId=udabac078-7e79-4&from=paste&id=u8ffd39f0&originHeight=463&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u074e0dd9-5252-429b-815c-bc5e3862ef8&title=)<br />主要就是遍历切面，放入缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142088528-9fa5edb3-e6e2-47be-aef5-b8380bfaab83.png#clientId=udabac078-7e79-4&from=paste&id=u7958800b&originHeight=887&originWidth=1042&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u88d11d42-4b2a-4ad1-88e9-4a4633fae14&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142088551-25ef9b7f-9089-4aec-a644-7b126f1a9484.png#clientId=udabac078-7e79-4&from=paste&id=u7d3f7ded&originHeight=400&originWidth=957&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56e05671-3b66-499d-ac57-1d1bc0d7028&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142088673-a2aab721-b8f8-4718-bb8a-ae2f3d0c740d.png#clientId=udabac078-7e79-4&from=paste&id=u706ea037&originHeight=682&originWidth=975&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8bcb7b7-afb2-46cc-8842-6862b370366&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142088622-fbde9bac-73d8-4736-a83e-9a5ed7e38da3.png#clientId=udabac078-7e79-4&from=paste&id=u36ceca53&originHeight=300&originWidth=955&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9da1c6c9-9c8e-4f93-8256-e35c93a7480&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142088957-6f981b6e-24c8-4c87-bf08-2d5dcb7acf91.png#clientId=udabac078-7e79-4&from=paste&id=u09652da7&originHeight=271&originWidth=917&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3b72eafc-2860-414e-aacc-875c1164d58&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142089263-659d4aeb-188f-443c-82ad-4cb04203d084.png#clientId=udabac078-7e79-4&from=paste&id=u63d22a28&originHeight=229&originWidth=893&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaf3a7c95-b808-4dcf-9506-e8198e6f1bc&title=)<br />**这里是重点！敲黑板！！！**

1. 会先遍历所有的类；
2. 判断是否切面，只有切面才会进入后面逻辑；
3. 获取每个 Aspect 的切面列表；
4. 保存 Aspect 的切面列表到缓存 advisorsCache 中。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142089470-11537f7a-53c9-4425-9e8a-5fd2b9259b5d.png#clientId=udabac078-7e79-4&from=paste&id=ucd39a00f&originHeight=774&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue85e8383-5b61-4491-a26f-268b2682126&title=)<br />到这里，获取切面信息的流程就结束了，**因为后续对切面数据的获取，都是从缓存 advisorsCache 中拿到。**<br />下面就对上面的流程，再深入解读一下。
<a name="G3YYV"></a>
#### 2.2.1 判断是否是切面
上图的第 2 步，逻辑如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142089880-fdaca8f0-0fde-409d-8fa5-3f437144ef80.png#clientId=udabac078-7e79-4&from=paste&id=u56924070&originHeight=189&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u68f56728-496b-4625-a907-1d45c1f2f95&title=)
<a name="uRaND"></a>
#### 2.2.2 获取切面列表
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142090068-02c783c0-8f3d-4919-9a5d-cf064490b081.png#clientId=udabac078-7e79-4&from=paste&id=u93f7cffd&originHeight=884&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u70cf8a5b-4b03-4470-9af2-b88996fa2f2&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142090061-6c7798f6-e02b-474f-aad7-5387c3d10082.png#clientId=udabac078-7e79-4&from=paste&id=u0e8cb0d3&originHeight=354&originWidth=984&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9a162363-4164-47c7-ae9c-4a7d951f231&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142090342-54f696ff-5d2d-4f36-922a-b7f0505dcd1d.png#clientId=udabac078-7e79-4&from=paste&id=u2e06dfc7&originHeight=740&originWidth=943&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u224863a7-b1d5-402f-bcc3-39a5f2066d9&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142090823-c48c4bbc-682c-4d8c-8fa4-98a17fd7dbd0.png#clientId=udabac078-7e79-4&from=paste&id=u6b600d64&originHeight=137&originWidth=980&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u05d32638-7dc1-4235-ad91-b45f947898f&title=)<br />进入到 `getAdvice()`，生成切面信息。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142090848-f0d24abe-7b82-4255-ac5e-d59966d65438.png#clientId=udabac078-7e79-4&from=paste&id=ue61f911e&originHeight=712&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8ba80e6-a172-4fce-b1f3-09d0989b6ba&title=)
<a name="NziJN"></a>
### 2.3 后置处理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142090949-d228605f-c684-4a3c-9d58-b9b136d0a36b.png#clientId=udabac078-7e79-4&from=paste&id=uc3f4f94c&originHeight=807&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7a2b9658-1d14-4bf4-b6bb-3245c31094a&title=)<br />主要就是从缓存拿切面，和 louzai 的方法匹配，并创建 AOP 代理对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142091043-7dc7371a-21b7-4dcd-ad33-aa648bee5e58.png#clientId=udabac078-7e79-4&from=paste&id=uec28df11&originHeight=651&originWidth=1045&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u79b74780-a9bd-4868-9aec-43db99939a4&title=)<br />进入 `doCreateBean()`，走下面逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142091281-84845723-df02-48cb-9758-50f1da4bdff2.png#clientId=udabac078-7e79-4&from=paste&id=uc4aab9e3&originHeight=390&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0e67d021-1bd6-46b3-bf45-34a6eb9bdfe&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142091716-2fe945e6-1d6b-4b67-8536-49de80d64f5d.png#clientId=udabac078-7e79-4&from=paste&id=u655983cb&originHeight=619&originWidth=941&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uec375640-ea96-4270-bae3-f56a4d21a99&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142092022-965e6ae0-31b2-4b88-80ce-6cdca38b7650.png#clientId=udabac078-7e79-4&from=paste&id=uc53f0fa6&originHeight=577&originWidth=1003&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udfeaf51e-dbe0-4a28-9ae0-4d3cd3a87f1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142092774-5ac14661-8b6b-4083-b142-1aeb167e61f6.png#clientId=udabac078-7e79-4&from=paste&id=u76a7a386&originHeight=227&originWidth=981&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7300a610-d344-4c45-a15b-59e251ab11e&title=)<br />**这里是重点！敲黑板！！！**

1. 先获取 louzai 类的所有切面列表；
2. 创建一个 AOP 的代理对象。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142093540-dd894505-2249-4521-93ec-fdc1f996207d.png#clientId=udabac078-7e79-4&from=paste&id=u39a9f63a&originHeight=746&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6f968aee-a4a8-4752-b241-9976eb18bfe&title=)
<a name="B9v6z"></a>
#### 2.3.1 获取切面
先进入第一步，看是如何获取 louzai 的切面列表。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142093750-5b69c4bd-0faf-4103-9740-b93938b3f19c.png#clientId=udabac078-7e79-4&from=paste&id=ub0c3596a&originHeight=255&originWidth=945&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uac11c592-9153-4586-8299-dacb4d8a599&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094245-78440be1-dc01-48d3-83e9-6230c2e20f36.png#clientId=udabac078-7e79-4&from=paste&id=ub8a89757&originHeight=212&originWidth=896&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua4c8b2eb-4bbb-4c4b-9abb-30f4142e2d0&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094347-18b600df-cb8b-402b-bc99-28570e885034.png#clientId=udabac078-7e79-4&from=paste&id=u8726a670&originHeight=232&originWidth=989&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9f64aef-387d-4e62-8198-f17ab400ddf&title=)<br />进入 `buildAspectJAdvisors()`，这个方法应该有印象，就是前面将切面信息放入缓存 `advisorsCache` 中，现在这里就是要获取缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094541-00f3d46e-2322-406d-b94e-d0a78752388b.png#clientId=udabac078-7e79-4&from=paste&id=uc479a9c9&originHeight=304&originWidth=923&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub2f0b9ea-7af7-4dfc-8ece-b8bb57887c1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094756-31876f6d-ea2f-4ad9-ad38-9c03b9c8e572.png#clientId=udabac078-7e79-4&from=paste&id=u470f92fb&originHeight=362&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1ab696ed-0283-493d-9b0c-f96022d57bf&title=)<br />再回到 `findEligibleAdvisors()`，从缓存拿到所有的切面信息后，继续往后执行。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094878-0ed6d36a-73b7-46fc-9c6d-1b33638cc6dc.png#clientId=udabac078-7e79-4&from=paste&id=u1f17e07c&originHeight=197&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uff6e4d66-a603-4dab-ae4c-58ced6badd2&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094935-22a3c87a-6e6b-4b55-9379-6d46c9ff7a8c.png#clientId=udabac078-7e79-4&from=paste&id=u65651cc6&originHeight=253&originWidth=993&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc57c08d5-afb3-4850-94cd-6150f6b691b&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142094952-2e23160a-a67d-4c8d-a71b-394505a34e54.png#clientId=udabac078-7e79-4&from=paste&id=u7d14cb04&originHeight=806&originWidth=990&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7a949f0c-05f2-4f45-a349-c307260c78e&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142095121-d3f19f07-9011-4224-af56-237b8d636403.png#clientId=udabac078-7e79-4&from=paste&id=uba4023c1&originHeight=296&originWidth=972&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uce2b45b7-c2d2-496f-8fb9-4f78f85d945&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142096048-b5c3bf63-1717-4c39-84ec-2aff2f56a6c4.png#clientId=udabac078-7e79-4&from=paste&id=u544c6de4&originHeight=626&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9621cb90-b6ae-40b7-9f06-8e1ff3c5d73&title=)
<a name="AmJIR"></a>
#### 2.3.2 创建代理对象
有了 louzai 的切面列表，后面就可以开始去创建 AOP 代理对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142096013-6c9f3700-4777-4f22-b328-663e5d1204d9.png#clientId=udabac078-7e79-4&from=paste&id=u5c2b5edf&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6631dccf-ea60-47cd-b6da-67f7089258a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142096160-00952196-1557-4709-ac99-63062d485b14.png#clientId=udabac078-7e79-4&from=paste&id=ub60a0517&originHeight=691&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u126c4c8a-2520-49df-b333-dd79e2fae16&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142096252-40724066-ea9a-4346-b829-2d328c5b82ba.png#clientId=udabac078-7e79-4&from=paste&id=u984e1d37&originHeight=93&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5198b0fa-fd56-4389-8e32-17723eac7d2&title=)<br />**这里是重点！敲黑板！！！**<br />这里有 2 种创建 AOP 代理对象的方式，是选用 Cglib 来创建。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142096614-5ff37b58-98b2-4cc1-a819-c808e74542cd.png#clientId=udabac078-7e79-4&from=paste&id=u52f709b5&originHeight=450&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u88f2f424-16fd-4d2b-8877-afbce275ca2&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142097007-567ae2da-3f3a-42e3-bf28-68598565b453.png#clientId=udabac078-7e79-4&from=paste&id=ua6a82ca5&originHeight=271&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u735f2282-5b14-4edf-befb-30cfdbc1487&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142097549-d0ce0cb2-c43d-4564-9438-dd1c842b15a4.png#clientId=udabac078-7e79-4&from=paste&id=uaab00155&originHeight=349&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9c195eff-f01d-4869-8cfc-0ffe45d6451&title=)<br />再回到创建代理对象的入口，看看创建的代理对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142097942-0469d33f-d10f-481f-874e-5db4ffad38fd.png#clientId=udabac078-7e79-4&from=paste&id=uc4bc631d&originHeight=626&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5dd9fd75-d19e-4264-8e2a-d2e7c5e53c9&title=)
<a name="hXvWm"></a>
### 2.4 切面执行
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142098080-aa82fac6-6ecd-45ab-97aa-1f484719de34.png#clientId=udabac078-7e79-4&from=paste&id=u00e6a4e4&originHeight=581&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u315e6379-2de4-4564-a0bc-3d3a55c1446&title=)<br />通过 “责任链 + 递归”，执行切面和方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142098032-30474ece-298d-4fbd-b10c-c716004a02f7.png#clientId=udabac078-7e79-4&from=paste&id=u9e0b1c45&originHeight=228&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6fc19832-855a-4cc2-8e8f-0e830ddddea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142098511-a4e4905a-fb8a-43da-96b2-6db425099c7c.png#clientId=udabac078-7e79-4&from=paste&id=u8e4cbb1b&originHeight=723&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda1ef1d9-c1e7-4d11-a10d-a0db9e83471&title=)<br />**前方高能！这块逻辑非常复杂！！！**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142098650-b4e93f5d-ab96-49ea-804a-eaadf6488235.png#clientId=udabac078-7e79-4&from=paste&id=u59488698&originHeight=674&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u152f60b3-6cbc-45aa-a001-150bcb3348b&title=)<br />下面就是“执行切面”最核心的逻辑，简单说一下设计思路：

1. **设计思路**：采用递归 + 责任链的模式；
2. **递归**：反复执行 `CglibMethodInvocation` 的 `proceed()`；
3. **退出递归条件**：`interceptorsAndDynamicMethodMatchers` 数组中的对象，全部执行完毕；
4. **责任链**：示例中的责任链，是个长度为 3 的数组，每次取其中一个数组对象，然后去执行对象的 `invoke()`。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142099083-b702e463-43ef-4cfd-a686-d11cf2aa66ac.png#clientId=udabac078-7e79-4&from=paste&id=u85bfab72&originHeight=633&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5076e1b3-fc12-4409-bcea-b7e56965159&title=)<br />因为数组里面只有 3 个对象，所以只会递归 3 次，下面就看这 3 次是如何递归，责任链是如何执行的，设计得很巧妙！
<a name="wJQxO"></a>
#### 2.4.1 第一次递归
数组的第一个对象是 `ExposeInvocationInterceptor`，执行 `invoke()`，**注意入参是 **`**CglibMethodInvocation**`**。**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142099244-6b76f3db-836d-47ea-864c-a2a0fd29e9dd.png#clientId=udabac078-7e79-4&from=paste&id=u6edf97a0&originHeight=388&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u37297e79-078f-45d3-9c38-02a1a11c7d0&title=)<br />里面啥都没干，继续执行 `CglibMethodInvocation` 的 `process()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142099526-84c9cd69-7b1d-4601-83ed-6542e24121dc.png#clientId=udabac078-7e79-4&from=paste&id=u0bf6c518&originHeight=738&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u71f324c3-044e-4329-ad68-3b6c0605ef8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142099916-208a0c1b-a549-4eed-b4d9-c25d3bc7fbf0.png#clientId=udabac078-7e79-4&from=paste&id=u0b0cfd27&originHeight=606&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf3099115-e901-4550-9272-345751af16e&title=)
<a name="QUYqg"></a>
#### 2.4.2 第二次递归
数组的第二个对象是 `MethodBeforeAdviceInterceptor`，执行 `invoke()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142100124-dce56e3f-0025-4e0a-8ea6-6bce396a422c.png#clientId=udabac078-7e79-4&from=paste&id=ufe758db4&originHeight=531&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u375dafc6-023d-40ba-b4ff-59387b55b11&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142100692-ede0b84e-20a5-473a-b0e7-b78005371d63.png#clientId=udabac078-7e79-4&from=paste&id=uf50c60a6&originHeight=129&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u579598bc-5dfb-4218-af87-afbc3497e4c&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142100714-d4fef841-b4e0-4a2f-ab8a-ab8e978b9706.png#clientId=udabac078-7e79-4&from=paste&id=u52d4f2d6&originHeight=551&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u367e2cf5-d26c-4165-bd5f-5c579c27d45&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142101045-9f20ae55-007c-4f4b-988b-4c8187b90443.png#clientId=udabac078-7e79-4&from=paste&id=u8a00ef9f&originHeight=195&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9a281276-4b16-417d-8367-6982809f8f7&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142101322-5d5d8b6b-bc3b-42a6-878c-6b15c0057b24.png#clientId=udabac078-7e79-4&from=paste&id=u9d57e2f2&originHeight=513&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc05ea6b6-3fcc-4664-a62a-b522eba0526&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142102022-8e589e57-6d08-464e-bc55-cc00a7a61227.png#clientId=udabac078-7e79-4&from=paste&id=u2d46d2d5&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc750cb68-2b09-40c2-b8ea-66250851fda&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142101918-630b05a7-0cda-4bda-adb1-c0aaf081e19f.png#clientId=udabac078-7e79-4&from=paste&id=ub20abb3b&originHeight=520&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u19434217-715b-46f3-9623-584b545c686&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142102043-50538490-d1aa-46c0-862f-95804f9b73a1.png#clientId=udabac078-7e79-4&from=paste&id=u4122ba4a&originHeight=699&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61a833b0-e399-433b-bb19-9e038798fd9&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142102097-d77dac87-dbd7-4092-b78d-4c778ca4c599.png#clientId=udabac078-7e79-4&from=paste&id=ue74e3a32&originHeight=711&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub9044391-cee9-4079-a675-fb0d4b8bab0&title=)
<a name="ETUSV"></a>
#### 2.4.3 第三次递归
数组的第二个对象是 `AfterReturningAdviceInterceptor`，执行 `invoke()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142102268-8c9c5354-62a1-41d4-8685-9589f0890da7.png#clientId=udabac078-7e79-4&from=paste&id=u65c5fdf6&originHeight=548&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u833d5db2-e579-46ec-be55-e68f51901cf&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142102674-f123db62-2e2d-4b16-8602-78be8ab1626d.png#clientId=udabac078-7e79-4&from=paste&id=u3b64229f&originHeight=686&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ce9b062-8191-45cb-babc-5278b7f91eb&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142102945-5cef8a13-cb09-48c3-b6c0-87c4429d674d.png#clientId=udabac078-7e79-4&from=paste&id=uf8ccef82&originHeight=558&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3778a791-dfee-4e23-a34f-7d1225bb29d&title=)<br />执行完上面逻辑，就会退出递归，看看 `invokeJoinpoint()` 的执行逻辑，其实就是执行主方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142103074-488683c3-11e2-40c2-a37f-a0420735b311.png#clientId=udabac078-7e79-4&from=paste&id=ufb71f425&originHeight=295&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub7849146-37fd-42cb-a5db-659a2da499a&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142103885-28047a8d-dce8-49e1-ba8a-eeb81945a9a1.png#clientId=udabac078-7e79-4&from=paste&id=ud5d544fd&originHeight=479&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udcf5158f-be6b-430d-987e-07068d874e3&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142103996-0e808e6a-ede9-4f62-a5b2-6a2797f465b3.png#clientId=udabac078-7e79-4&from=paste&id=ue111c214&originHeight=230&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc6dfde1-9b0f-4533-8ba4-9b7a9264bf5&title=)<br />再回到第三次递归的入口，继续执行后面的切面。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142103937-16555574-d926-4b98-aced-6c549b56cbe1.png#clientId=udabac078-7e79-4&from=paste&id=uf66844f3&originHeight=518&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0de65f7e-fb3c-4d84-93cb-5b5b66f1762&title=)<br />切面执行逻辑，前面已经演示过，直接看执行方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142104121-af78a7b5-ddf8-4f1a-9aee-9f48d11bd800.png#clientId=udabac078-7e79-4&from=paste&id=u575d3cf2&originHeight=690&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc0889a92-4185-41d9-a3d0-8fe2d923cc6&title=)<br />后面就依次退出递归，整个流程结束。
<a name="ZI7pg"></a>
#### 2.4.4 设计思路
这块代码研究了大半天，因为这个不是纯粹的责任链模式。<br />纯粹的责任链模式，对象内部有一个自身的 next 对象，执行完当前对象的方法末尾，就会启动 next 对象的执行，直到最后一个 next 对象执行完毕，或者中途因为某些条件中断执行，责任链才会退出。<br />这里 `CglibMethodInvocation` 对象内部没有 `next` 对象，全程是通过 `interceptorsAndDynamicMethodMatchers` 长度为 3 的数组控制，依次去执行数组中的对象，直到最后一个对象执行完毕，责任链才会退出。<br />**这个也属于责任链，只是实现方式不一样，后面会详细剖析**，下面再讨论一下，这些类之间的关系。<br />主对象是 `CglibMethodInvocation`，继承于 `ReflectiveMethodInvocation`，然后 `process()` 的核心逻辑，其实都在 `ReflectiveMethodInvocation` 中。<br />`**ReflectiveMethodInvocation**`** 中的 **`**process()**`** 控制整个责任链的执行。**<br />`ReflectiveMethodInvocation` 中的 `process()` 方法，里面有个长度为 3 的数组 `interceptorsAndDynamicMethodMatchers`，里面存储了 3 个对象，分别为 `ExposeInvocationInterceptor`、`MethodBeforeAdviceInterceptor`、`AfterReturningAdviceInterceptor`。<br />**注意！！！这 3 个对象，都是继承 **`**MethodInterceptor**`** 接口。**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666142104222-fbf62520-ec76-400f-ae8f-03d2dfd96e39.png#clientId=udabac078-7e79-4&from=paste&id=uf95100d0&originHeight=628&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc91e2ee2-8a78-4cb8-bdb7-f038b61e774&title=)<br />然后每次执行 `invoke()` 时，里面都会去执行 `CglibMethodInvocation` 的 `process()`。<br />**是不是听得有些蒙圈？甭着急，重新再梳理一下。**<br />对象和方法的关系：

- **接口继承**：数组中的 3 个对象，都是继承 `MethodInterceptor` 接口，实现里面的 `invoke()` 方法；
- **类继承**：主对象 `CglibMethodInvocation`，继承于 `ReflectiveMethodInvocation`，复用它的 `process()` 方法；
- **两者结合（策略模式）**：`invoke()` 的入参，就是 `CglibMethodInvocation`，执行 `invoke()` 时，内部会执行 `CglibMethodInvocation.process()`，这个其实就是个策略模式。

可能有同学会说，`invoke()` 的入参是 `MethodInvocation`，没错！但是 `CglibMethodInvocation` 也继承了 `MethodInvocation`，不信自己可以去看。<br />执行逻辑：

- **程序入口**：是 `CglibMethodInvocation` 的 `process()` 方法；
- **链式执行（衍生的责任链模式）**：`process()` 中有个包含 3 个对象的数组，依次去执行每个对象的 `invoke()` 方法。
- **递归（逻辑回退）**：`invoke()` 方法会执行切面逻辑，同时也会执行 `CglibMethodInvocation` 的 `process()` 方法，让逻辑再一次进入 `process()`。
- **递归退出**：当数字中的 3 个对象全部执行完毕，流程结束。

所以这里设计巧妙的地方，是因为纯粹责任链模式，里面的 next 对象，需要保证里面的对象类型完全相同。<br />但是数组里面的 3 个对象，里面没有 next 成员对象，所以不能直接用责任链模式，那怎么办呢？就单独搞了一个 `CglibMethodInvocation.process()`，通过去无限递归 `process()`，来实现这个责任链的逻辑。<br />**这就是为什么要看源码，学习里面优秀的设计思路！**
<a name="hRPHV"></a>
## 3、总结
介绍了什么是 AOP，以及 AOP 的原理和示例。<br />之后再剖析了 AOP 的源码，分为 3 块：

- 将所有的切面都保存在缓存中；
- 取出缓存中的切面列表，和 louzai 对象的所有方法匹配，拿到属于 louzai 的切面列表；
- 创建 AOP 代理对象；
- 通过“责任链 + 递归”，去执行切面逻辑。
