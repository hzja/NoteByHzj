来扒一扒轻量级的分布式任务调度平台Xxl-Job背后的架构原理
<a name="DR850"></a>
## 核心概念
<a name="sRBO1"></a>
### 1、调度中心
调度中心是一个单独的Web服务，主要是用来触发定时任务的执行<br />它提供了一些页面操作，可以很方便地去管理这些定时任务的触发逻辑<br />调度中心依赖数据库，所以数据都是存在数据库中的<br />调度中心也支持集群模式，但是它们所依赖的数据库必须是同一个<br />所以同一个集群中的调度中心实例之间是没有任何通信的，数据都是通过数据库共享的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573169627-d78f1b06-a652-486e-aedb-e5d767cfa33a.png#averageHue=%23f3eee0&clientId=u3b3088f5-2ee6-4&from=paste&id=u547b1fc7&originHeight=171&originWidth=421&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u47e2241b-d2c7-4ab5-8b18-40c68c85dbf&title=)
<a name="wY7ec"></a>
### 2、执行器
执行器是用来执行具体的任务逻辑的<br />执行器可以理解为就是平时开发的服务，一个服务实例对应一个执行器实例<br />每个执行器有自己的名字，为了方便，可以将执行器的名字设置成服务名
<a name="L0Hia"></a>
### 3、任务
任务什么意思就不用多说了<br />一个执行器中也是可以有多个任务的<br />总的来说，调用中心是用来控制定时任务的触发逻辑，而执行器是具体执行任务的，这是一种任务和触发逻辑分离的设计思想，这种方式的好处就是使任务更加灵活，可以随时被调用，还可以被不同的调度规则触发。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573169653-4e2eb261-4acd-4e65-b4ea-21841b253885.png#averageHue=%23f3ece8&clientId=u3b3088f5-2ee6-4&from=paste&id=ub328d7a1&originHeight=231&originWidth=151&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uedd371cc-163e-44c7-b2a3-8c8bdddcaa0&title=)
<a name="Z6qlF"></a>
## 来个Demo
<a name="Y1eZy"></a>
### 1、搭建调度中心
调度中心搭建很简单，先下载源码：[https://github.com/xuxueli/xxl-job.git](https://github.com/xuxueli/xxl-job.git)<br />然后改一下数据库连接信息，执行一下在项目源码中的/doc/db下的sql文件<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573169723-e1fa5210-5804-44fc-9511-70cdf15b7c46.png#averageHue=%23929393&clientId=u3b3088f5-2ee6-4&from=paste&id=u645665ed&originHeight=588&originWidth=736&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u497e025f-cc18-41a1-82c5-20382d1dfae&title=)<br />启动可以打成一个jar包，或者本地启动就是可以的<br />启动完成之后，访问下面这个地址就可以访问到控制台页面了<br />http://localhost:8080/xxl-job-admin/toLogin<br />用户名密码默认是 admin/123456
<a name="IboXs"></a>
### 2、执行器和任务添加
添加一个名为sanyou-xxljob-demo执行器<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573169643-90369a6f-7b62-4a4f-93e4-efdca5419358.png#averageHue=%23397348&clientId=u3b3088f5-2ee6-4&from=paste&id=u465a4706&originHeight=506&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue70205d2-5f76-45c5-8fb7-a26c1d87dae&title=)<br />任务添加<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573169642-a14dac40-657d-4b80-9b0f-30851915279f.png#averageHue=%236b5330&clientId=u3b3088f5-2ee6-4&from=paste&id=u5461e51c&originHeight=509&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud6c8100c-811f-4463-abcb-8802ecf34a9&title=)<br />执行器选择刚刚添加的，指定任务名称为TestJob，corn表达式的意思是每秒执行一次<br />创建完之后需要启动一下任务，默认是关闭状态，也就不会执行<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170226-b30055d9-4b4d-407e-8e55-0ecc81290fee.png#averageHue=%23f2f1f3&clientId=u3b3088f5-2ee6-4&from=paste&id=uf6f3fc1d&originHeight=730&originWidth=698&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7b0d0a55-d68c-4983-8c75-40a29510e79&title=)<br />创建执行器和任务其实就是CRUD，并没有复杂的业务逻辑<br />按照如上配置的整个Demo的意思就是<br />**每隔1s，执行一次sanyou-xxljob-demo这个执行器中的TestJob任务**
<a name="Hczsc"></a>
### 3、创建执行器和任务
引入依赖
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <version>2.2.5.RELEASE</version>
  </dependency>
  <dependency>
    <groupId>com.xuxueli</groupId>
    <artifactId>xxl-job-core</artifactId>
    <version>2.4.0</version>
  </dependency>
</dependencies>
```
配置`XxlJobSpringExecutor`这个Bean
```java
@Configuration
public class XxlJobConfiguration {

    @Bean
    public XxlJobSpringExecutor xxlJobExecutor() {
        XxlJobSpringExecutor xxlJobSpringExecutor = new XxlJobSpringExecutor();
        //设置调用中心的连接地址
        xxlJobSpringExecutor.setAdminAddresses("http://localhost:8080/xxl-job-admin");
        //设置执行器的名称
        xxlJobSpringExecutor.setAppname("sanyou-xxljob-demo");
        //设置一个端口，后面会讲作用
        xxlJobSpringExecutor.setPort(9999);
        //这个token是保证访问安全的，默认是这个，当然可以自定义，
        // 但需要保证调度中心配置的xxl.job.accessToken属性跟这个token是一样的
        xxlJobSpringExecutor.setAccessToken("default_token");
        //任务执行日志存放的目录
        xxlJobSpringExecutor.setLogPath("./");
        return xxlJobSpringExecutor;
    }

}
```
`XxlJobSpringExecutor`这个类的作用，后面会着重讲<br />通过`@XxlJob`指定一个名为TestJob的任务，这个任务名需要跟前面页面配置的对应上
```java
@Component
public class TestJob {

    private static final Logger logger = LoggerFactory.getLogger(TestJob.class);

    @XxlJob("TestJob")
    public void testJob() {
        logger.info("TestJob任务执行了。。。");
    }

}
```
所以如果顺利的话，每隔1s钟就会打印一句TestJob任务执行了。。。<br />启动项目，注意修改一下端口，因为调用中心默认也是8080，本地起会端口冲突<br />最终执行结果如下，符合预期<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170147-75c3cf9c-097d-4d19-8658-a01206149f0b.png#averageHue=%23373635&clientId=u3b3088f5-2ee6-4&from=paste&id=u4fbe0dec&originHeight=206&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1cfe72f3-9e0b-4582-94fd-6a6652ae256&title=)<br />讲完概念和使用部分，接下来就来好好讲一讲Xxl-Job核心的实现原理
<a name="TN6of"></a>
## 从执行器启动说起
前面Demo中使用到了一个很重要的一个类`XxlJobSpringExecutor`，这个类就是整个执行器启动的入口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170215-c8f211f2-d02a-47cf-a108-3029b0a433d0.png#averageHue=%232e2c2b&clientId=u3b3088f5-2ee6-4&from=paste&id=u0f62a56f&originHeight=526&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u559f3290-5b44-4c56-a94a-f19a75ec308&title=)<br />这个类实现了`SmartInitializingSingleton`接口<br />所以经过Bean的生命周期，一定会调用`afterSingletonsInstantiated`这个方法的实现<br />这个方法干了很多初始化的事，这里挑三个重要的讲，其余的等到具体的功能的时候再提
<a name="RqoNQ"></a>
### 1、初始化JobHandler
`JobHandler`是个什么？<br />所谓的`JobHandler`其实就是一个定时任务的封装<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170233-1557f7c9-83e6-4c18-99f3-06143a910555.png#averageHue=%232f2f2d&clientId=u3b3088f5-2ee6-4&from=paste&id=ufa3457a1&originHeight=883&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uad8a25ff-1b05-4e05-8755-657fe2bfd11&title=)<br />一个定时任务会对应一个`JobHandler`对象<br />当执行器执行任务的时候，就会调用`JobHandler`的`execute`方法<br />`JobHandler`有三种实现：

- `MethodJobHandler`
- `GlueJobHandler`
- `ScriptJobHandler`

`**MethodJobHandler**`是通过反射来调用方法执行任务<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170248-1be04962-f07d-4a00-873d-2309f4de094c.png#averageHue=%232e2d2b&clientId=u3b3088f5-2ee6-4&from=paste&id=ud51e10ad&originHeight=775&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5138dee7-dab2-45fe-b1ed-a2abd2ea13f&title=)<br />所以`MethodJobHandler`的任务的实现就是一个方法，刚好demo中的例子任务其实就是一个方法<br />所以Demo中的任务最终被封装成一个`MethodJobHandler`<br />`**GlueJobHandler**`比较有意思，它支持动态修改任务执行的代码<br />当在创建任务的时候，需要指定运行模式为GLUE(Java)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170710-0d71c695-d1f9-4dcf-9f3c-5759f28728c9.png#averageHue=%23ede9eb&clientId=u3b3088f5-2ee6-4&from=paste&id=ua57b0c50&originHeight=432&originWidth=1030&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uffc8d551-83e4-4999-b83f-ea3e655aa2c&title=)<br />之后需要在操作按钮点击GLUE IDE编写Java代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170686-e5092788-a464-4287-a1be-fdcb6476b46b.png#averageHue=%23fdfefd&clientId=u3b3088f5-2ee6-4&from=paste&id=u435a60ac&originHeight=746&originWidth=858&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf9b1cd25-6b4e-4fd5-97ff-141418c6ea8&title=)<br />代码必须得实现`IJobHandler`接口，之后任务执行的时候就会执行`execute`方法的实现<br />如果需要修改任务的逻辑，只需要重新编辑即可，不需要重启服务<br />`**ScriptJobHandler**`，通过名字也可以看出，是专门处理一些脚本的<br />运行模式除了BEAN和GLUE(Java)之外，其余都是脚本模式<br />而本节的主旨，所谓的初始化`JobHandler`就是指，执行器启动的时候会去Spring容器中找到加了`@XxlJob`注解的Bean<br />解析注解，然后封装成一个`MethodJobHandler`对象，最终存到`XxlJobSpringExecutor`成员变量的一个本地的Map缓存中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170810-758969ee-47a4-42b2-b33b-a83841ad5d5b.png#averageHue=%23352e2c&clientId=u3b3088f5-2ee6-4&from=paste&id=u0c632be3&originHeight=400&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ude627fa4-ee7e-45da-9502-8f1e29adb7e&title=)<br />缓存key就是任务的名字<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170904-b25f5b17-60e7-4951-896d-631c45541c8f.png#averageHue=%23f8f7f4&clientId=u3b3088f5-2ee6-4&from=paste&id=u6097cc51&originHeight=321&originWidth=341&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3c3017bb-474a-46b9-80ee-e106c8b52ef&title=)<br />至于`GlueJobHandler`和`ScriptJobHandler`都是任务触发时才会创建<br />除了上面这几种，也自己实现`JobHandler`，手动注册到`JobHandler`的缓存中，也是可以通过调度中心触发的
<a name="kRPO4"></a>
### 2、创建一个Http服务器
除了初始化`JobHandler`之外，执行器还会创建一个Http服务器<br />这个服务器端口号就是通过`XxlJobSpringExecutor`配置的端口，demo中就是设置的是9999，底层是基于Netty实现的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573170955-ac648695-5035-48ed-9df6-812aaa106208.png#averageHue=%2353503a&clientId=u3b3088f5-2ee6-4&from=paste&id=u673f47ee&originHeight=597&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u922e846e-5b2e-4c6f-8b34-af98440e7cb&title=)<br />这个Http服务端会接收来自调度中心的请求<br />**当执行器接收到调度中心的请求时，会把请求交给**`**ExecutorBizImpl**`**来处理**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171235-0e8f2d47-be7a-494a-a77d-2184fb348ea5.png#averageHue=%232d2c2b&clientId=u3b3088f5-2ee6-4&from=paste&id=u4479e59a&originHeight=810&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub4d0cd38-6e86-4649-803f-b23f4be0969&title=)<br />这个类非常重要，所有调度中心的请求都是这里处理的<br />`ExecutorBizImpl`实现了`ExecutorBiz`接口<br />当翻源码的时候会发现，`ExecutorBiz`还有一个`ExecutorBizClient`实现<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171213-5f3259b5-9b76-4684-aff3-b59e59ad8481.png#averageHue=%232d2c2b&clientId=u3b3088f5-2ee6-4&from=paste&id=uea94dcce&originHeight=495&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u24a5cc00-f25a-41d9-a66e-e5e593b6604&title=)<br />`ExecutorBizClient`的实现就是发送http请求，所以这个实现类是在调度中心使用的，用来访问执行器提供的http接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171344-f0d1450b-eb59-4ca6-96f3-b6dedbc3e3ca.png#averageHue=%234c4433&clientId=u3b3088f5-2ee6-4&from=paste&id=uf982ac7c&originHeight=151&originWidth=521&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8d506722-8c62-408f-9ad3-60aaaa93bfc&title=)
<a name="mMtKI"></a>
### 3、注册到调度中心
当执行器启动的时候，会启动一个注册线程，这个线程会往调度中心注册当前执行器的信息，包括两部分数据

- 执行器的名字，也就是设置的appname
- 执行器所在机器的ip和端口，这样调度中心就可以访问到这个执行器提供的Http接口

前面提到每个服务实例都会对应一个执行器实例，所以调用中心会保存每个执行器实例的地址<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171399-c9523e35-69d1-45de-b083-1bdc8265fdb6.png#averageHue=%23524741&clientId=u3b3088f5-2ee6-4&from=paste&id=ua9e50f11&originHeight=241&originWidth=321&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5137f63c-0317-49b1-8358-c0216a93f74&title=)<br />这里可以把调度中心的功能类比成注册中心
<a name="v1tWb"></a>
## 任务触发原理
弄明白执行器启动时干了哪些事，接下来讲一讲Xxl-Job最最核心的功能，那就是任务触发的原理<br />任务触发原理会分下面5个小点来讲解

- 任务如何触发？
- 快慢线程池的异步触发任务优化
- 如何选择执行器实例？
- 执行器如何去执行任务？
- 任务执行结果的回调
<a name="dkuiI"></a>
### 1、任务如何触发？
调度中心在启动的时候，会开启一个线程，这个线程的作用就是来计算任务触发时机，这里把这个线程称为**调度线程**<br />这个调度线程会去查询xxl_job_info这张表<br />这张表存了任务的一些基本信息和任务下一次执行的时间<br />调度线程会去查询**下一次执行的时间 <= 当前时间 + 5s**的任务<br />这个5s是XxlJob写死的，被称为预读时间，提前读出来，保证任务能准时触发<br />举个例子，假设当前时间是2023-11-29 08:00:10，这里的查询就会查出下一次任务执行时间在2023-11-29 08:00:15之前执行的任务<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171498-2c6e4cd6-111a-4913-a623-c2d0e5833d13.png#averageHue=%23f6e9d3&clientId=u3b3088f5-2ee6-4&from=paste&id=u1abcb39b&originHeight=61&originWidth=361&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8119a40c-185d-406b-be46-7657f4c584f&title=)<br />查询到任务之后，调度线程会去将这些任务根据执行时间划分为三个部分：

- 当前时间已经超过任务下一次执行时间5s以上，也就是需要在2023-11-29 08:00:05（不包括05s）之前的执行的任务
- 当前时间已经超过任务下一次执行时间，但是但不足5s，也就是在2023-11-29 08:00:05和2023-11-29 08:00:10（不包括10s）之间执行的任务
- 还未到触发时间，但是一定是5s内就会触发执行的

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171733-6a3ef8a2-c022-4e06-a5c9-c08b5751a73f.png#averageHue=%230d0b0a&clientId=u3b3088f5-2ee6-4&from=paste&id=ubcb6ded8&originHeight=161&originWidth=383&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4476cffd-6a45-4b17-a827-da459d7c824&title=)<br />对于第一部分的已经超过5s以上时间的任务，会根据任务配置的**调度过期策略**来选择要不要执行<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171829-86d5228c-9d04-4ac9-ac45-948a8ef1c372.png#averageHue=%23f8f6f6&clientId=u3b3088f5-2ee6-4&from=paste&id=u1e217c6c&originHeight=335&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf70f7809-6efc-44f8-85ce-0dc65566f33&title=)<br />调度过期策略就两种，就是字面意思

- 直接忽略这个已经过期的任务
- 立马执行一次这个过期的任务

对于第二部分的超时时间在5s以内的任务，就直接立马执行一次，之后如果判断任务下一次执行时间就在5s内，会直接放到一个时间轮里面，等待下一次触发执行<br />对于第三部分任务，由于还没到执行时间，所以不会立马执行，也是直接放到时间轮里面，等待触发执行<br />当这批任务处理完成之后，不论是前面是什么情况，调度线程都会去重新计算每个任务的下一次触发时间，然后更新xxl_job_info这张表的下一次执行时间<br />到此，一次调度的计算就算完成了<br />之后调度线程还会继续重复上面的步骤，查任务，调度任务，更新任务下次执行时间，一直死循环下去，这就实现了任务到了执行时间就会触发的功能<br />这里在任务触发的时候还有一个很有意思的细节<br />由于调度中心可以是集群的形式，每个调度中心实例都有调度线程，那么如何保证任务在同一时间只会被其中的一个调度中心触发一次？<br />第一时间肯定想到分布式锁，但是怎么加呢？<br />XxlJob实现就比较有意思了，它是基于八股文中常说的通过数据库来实现的分布式锁的<br />在调度之前，调度线程会尝试执行下面这句sql<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573171811-aa446efc-8f29-4b85-ad05-7957924f46b6.png#averageHue=%23302c2b&clientId=u3b3088f5-2ee6-4&from=paste&id=u72bb7e1c&originHeight=276&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaac1a342-acc6-41de-96ee-10f2efdd048&title=)<br />就是这个sql
```plsql
select * from xxl_job_lock where lock_name = 'schedule_lock' for update
```
一旦执行成功，说明当前调度中心成功抢到了锁，接下来就可以执行调度任务了<br />当调度任务执行完之后再去关闭连接，从而释放锁<br />由于每次执行之前都需要去获取锁，这样就保证在调度中心集群中，同时只有一个调度中心执行调度任务<br />最后画一张图来总结一下这一小节<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172040-e9665979-ce1b-4c5a-8817-5d0d83b110e7.png#averageHue=%231d1b19&clientId=u3b3088f5-2ee6-4&from=paste&id=ud0c2d438&originHeight=601&originWidth=451&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u248289ba-c188-42ff-839f-7a8b68e8fa7&title=)
<a name="j4TmO"></a>
### 2、快慢线程池的异步触发任务优化
当任务达到了触发条件，并不是由调度线程直接去触发执行器的任务执行<br />调度线程会将这个触发的任务交给线程池去执行<br />所以上图中的最后一部分触发任务执行其实是线程池异步去执行的<br />那么，为什么要使用线程池异步呢？<br />主要是因为触发任务，需要通过Http接口调用具体的执行器实例去触发任务<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172124-6b4df093-a669-47b3-80db-51cb03decbd7.png#averageHue=%236d676d&clientId=u3b3088f5-2ee6-4&from=paste&id=u3263c711&originHeight=181&originWidth=121&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0c729e2e-c8c7-4c06-8ee6-79897015b51&title=)<br />这一过程必然会耗费时间，如果调度线程去做，就会耽误调度的效率<br />所以就通过异步线程去做，调度线程只负责判断任务是否需要执行<br />并且，Xxl-Job为了进一步优化任务的触发，将这个触发任务执行的线程池划分成**快线程池**和**慢线程池**两个线程池<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172213-bacac256-28ea-4a71-a932-b9f42be70338.png#averageHue=%232c2b2b&clientId=u3b3088f5-2ee6-4&from=paste&id=u58dd75bf&originHeight=873&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7052b120-b218-4511-b03f-d3588679019&title=)<br />在调用执行器的Http接口触发任务执行的时候，Xxl-Job会去记录每个任务的触发所耗费的时间<br />注意并不是任务执行时间，只是整个Http请求耗时时间，这是因为执行器执行任务是异步执行的，所以整个时间不包括任务执行时间，这个后面会详细说<br />当任务一次触发的时间超过500ms，那么这个任务的慢次数就会加1<br />如果这个任务**一分钟内触发的慢次数超过10次**，接下来就会将触发任务交给慢线程池去执行<br />所以快慢线程池就是避免那种频繁触发并且每次触发时间还很长的任务阻塞其它任务的触发的情况发生
<a name="XxLsN"></a>
### 3、如何选择执行器实例？
上一节说到，当任务需要触发的时候，调度中心会向执行器发送Http请求，执行器去执行具体的任务<br />那么问题来了<br />由于一个执行器会有很多实例，那么应该向哪个实例请求？<br />这其实就跟任务配置时设置的**路由策略**有关了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172282-080554c6-fa07-47f6-9b2b-9b0433e6f93c.png#averageHue=%23989897&clientId=u3b3088f5-2ee6-4&from=paste&id=u53c01d68&originHeight=337&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9df77ddc-358b-4b81-b7a5-52f0f34094d&title=)<br />从图上可以看出xxljob支持多种路由策略<br />除了分片广播，其余的具体的算法实现都是通过`ExecutorRouter`的实现类来实现的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172295-3d8be615-0842-45c0-915a-bb480fe4b986.png#averageHue=%235e7076&clientId=u3b3088f5-2ee6-4&from=paste&id=u87e4b59e&originHeight=551&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u954c02a0-d0e2-41a5-90d8-53df9435316&title=)<br />这里简单讲一讲各种算法的原理，有兴趣的小伙伴可以去看看内部的实现细节<br />第一个、最后一个、轮询、随机都很简单，没什么好说的<br />**最不经常使用**（LFU：Least Frequently Used）：Xxl-Job内部会有一个缓存，统计每个任务每个地址的使用次数，每次都选择使用次数最少的地址，这个缓存每隔24小时重置一次<br />**最近最久未使用**（LRU：Least Recently Used）：将地址存到`LinkedHashMap`中，它利用`LinkedHashMap`可以根据元素访问（get/put）顺序来给元素排序的特性，快速找到最近最久未使用（未访问）的节点<br />**故障转移**：调度中心都会去请求每个执行器，只要能接收到响应，说明执行器正常，那么任务就会交给这个执行器去执行<br />**忙碌转移**：调度中心也会去请求每个执行器，判断执行器是不是正在执行当前需要执行的任务（任务执行时间过长，导致上一次任务还没执行完，下一次又触发了），如果在执行，说明忙碌，不能用，否则就可以用<br />**分片广播**：XxlJob给每个执行器分配一个编号，从0开始递增，然后向所有执行器触发任务，告诉每个执行器自己的编号和总共执行器的数据<br />可以通过`XxlJobHelper#getShardIndex`获取到编号，`XxlJobHelper#getShardTotal`获取到执行器的总数据量<br />分片广播就是将任务量分散到各个执行器，每个执行器只执行一部分任务，加快任务的处理<br />举个例子，比如现在需要处理30w条数据，有3个执行器，此时使用分片广播，那么此时可将任务分成3分，每份10w条数据，执行器根据自己的编号选择对应的那份10w数据处理<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172570-ad55fcc1-a59a-4cfb-a39d-3989ef8e01d7.png#averageHue=%23f6f3ee&clientId=u3b3088f5-2ee6-4&from=paste&id=uf11ecd07&originHeight=361&originWidth=271&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u487d91cf-9323-45eb-abd0-44e239c93e4&title=)<br />当选择好了具体的执行器实例之后，调用中心就会携带一些触发的参数，发送Http请求，触发任务
<a name="kjbE8"></a>
### 4、执行器如何去执行任务？
相信你一定记得前面在说执行器启动是会创建一个Http服务器的时候提到这么一句<br />当执行器接收到调度中心的请求时，会把请求交给`ExecutorBizImpl`来处理<br />所以前面提到的故障转移和忙碌转移请求执行器进行判断，最终执行器也是交给`ExecutorBizImpl`处理的<br />执行器处理触发请求是这个`ExecutorBizImpl`的`run`方法实现的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172574-3ad0b8ba-76d6-4caa-9c17-75930b06480d.png#averageHue=%232f2c2b&clientId=u3b3088f5-2ee6-4&from=paste&id=u2923dc6f&originHeight=729&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u96c3e45b-8f34-44ca-8e4f-2d1a3553424&title=)<br />当执行器接收到请求，在**正常情况下**，执行器会去为这个任务创建一个单独的线程，这个线程被称为`JobThread`<br />每个任务在触发的时候都有单独的线程去执行，保证不同的任务执行互不影响<br />之后任务并不是直接交给线程处理的，而是直接放到一个内存队列中，线程直接从队列中获取任务<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172704-c28c406d-9626-4e71-a8ec-15b30bd512b5.png#averageHue=%23faf8f8&clientId=u3b3088f5-2ee6-4&from=paste&id=u8b436ded&originHeight=154&originWidth=641&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u304e8a3b-5ac4-4b91-aa2e-dd46e5c8406&title=)<br />这里一定有个疑惑<br />为什么不直接处理，而是交给队列，从队列中获取任务呢？<br />那就得讲讲不正常的情况了<br />如果调度中心选择的**执行器实例正在处理定时任务**，那么此时该怎么处理呢？**<br />这时就跟**阻塞处理策略**有关了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172851-0f3b1ed0-9ea4-408a-88be-ee8dd591b37d.png#averageHue=%23f4f2f1&clientId=u3b3088f5-2ee6-4&from=paste&id=ud5ae5276&originHeight=366&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue040e9c3-3d76-4458-8465-8c9bab132c3&title=)<br />阻塞处理策略总共有三种：

- 单机串行
- 丢弃后续调度
- 覆盖之前调度

**单机串行**的实现就是将任务放到队列中，由于队列是先进先出的，所以就实现串行，这也是为什么放在队列的原因<br />**丢弃调度**的实现就是执行器什么事都不用干就可以了，自然而然任务就丢了<br />**覆盖之前调度**的实现就很暴力了，他是直接重新创建一个JobThread来执行任务，并且尝试打断之前的正在处理任务的JobThread，丢弃之前队列中的任务<br />打断是通过`Thread#interrupt`方法实现的，所以正在处理的任务还是有可能继续运行，并不是说一打断正在运行的任务就终止了<br />这里需要注意的一点就是，**阻塞处理策略是对于单个执行器上的任务来生效的，不同执行器实例上的同一个任务是互不影响的**<br />比如说，有一个任务有两个执行器A和B，路由策略是轮询<br />任务第一次触发的时候选择了执行器实例A，由于任务执行时间长，任务第二次触发的时候，执行器的路由到了B，此时A的任务还在执行，但是B感知不到A的任务在执行，所以此时B就直接执行了任务<br />所以此时配置的什么阻塞处理策略就没什么用了<br />如果业务中需要保证定时任务同一时间只有一个能运行，需要把任务路由到同一个执行器上，比如路由策略就选择第一个
<a name="ivpN0"></a>
### 5、任务执行结果的回调
当任务处理完成之后，执行器会将任务执行的结果发送给调度中心<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573172953-20cb97ef-721e-46ba-adf7-d770be9f1422.png#averageHue=%23f6f4f3&clientId=u3b3088f5-2ee6-4&from=paste&id=u7f82dacb&originHeight=126&originWidth=661&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua1c74dcf-0696-4bd3-9ee4-c9e5a192844&title=)<br />如上图所示，这整个过程也是异步化的

- JobThread会将任务执行的结果发送到一个内存队列中
- 执行器启动的时候会开启一个处发送任务执行结果的线程：`TriggerCallbackThread`
- 这个线程会不停地从队列中获取所有的执行结果，将执行结果批量发送给调度中心
- 调用中心接收到请求时，会根据执行的结果修改这次任务的执行状态和进行一些后续的事，比如失败了是否需要重试，是否有子任务需要触发等等

到此，一次任务的就算真正处理完成了
<a name="b9sf0"></a>
## 最后
最后从官网捞了一张Xxl-Job架构图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701573173132-ce38e463-3a24-408c-9a48-2f9cb98a416d.png#averageHue=%23a8cc99&clientId=u3b3088f5-2ee6-4&from=paste&id=u785cdff3&originHeight=573&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub882ce4f-ae15-4b24-95e7-4f641a72ac9&title=)<br />奈何作者不更新，导致这个图稍微有点老了，有点跟现有的架构对不上<br />比如说图中的自研RPC（xxl-rpc）部分已经替换成了Http协议，这主要是拥抱生态，方便跨语言接入<br />但是不要紧，大体还是符合现在的整个的架构<br />从架构图中也可以看出来，本文除了日志部分的内容没有提到，其它的整个核心逻辑基本上都讲到了<br />而日志部分其实是个辅助的作用，更方便查看任务的运行情况，对任务的触发逻辑是没有影响的，所以就没讲了<br />所以从本文的讲解再到官方架构图，会发现整个Xxl-Job不论是使用还是实现都是比较简单的，非常的轻量级。
