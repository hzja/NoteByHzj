JavaSpringCloud<br />Eureka 有个延迟注册的功能，也就是在服务启动成功之后不立刻注册到 Eureka Server，而是延迟一段时间再去注册，这样做的主要目的是因为虽然服务启动成功了，可能还有一些框架或者业务的代码没有初始化完成，可能会导致调用的报错，所以需要延迟注册。<br />但是发现，然并卵，好像这个延迟注册并没有生效，也是开始了排查之路。
<a name="COC6s"></a>
## **延迟注册**
首先，延迟注册的功能主要依赖这两个参数，`eureka.client.initial-instance-info-replication-interval-seconds`代表第一次初始化延迟注册的时间间隔，`eureka.client.instance-info-replication-interval-seconds`则代表后续同步注册的时间间隔。
```
eureka.client.initial-instance-info-replication-interval-seconds=40 //默认40秒
eureka.client.instance-info-replication-interval-seconds=30 //默认30秒
```
从源码先来看是怎么做到延迟注册的，先看 `DiscoveryClient` 的 `initScheduledTasks` ，这里创建了同步注册到 Eureka Server 的定时任务。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332069-d6f193d3-0379-4b7e-84ae-ab74cd47b09b.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u09c924d1&originHeight=326&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubf39ac4a-c7f8-4710-a443-d6faf36ee42&title=)<br />之后调用 `start` 方法创建定时任务，并且延迟 40 秒执行，也就是达到的延迟注册的效果。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332227-779c0c2d-d2a0-4982-9649-e93ad0a5789f.jpeg#clientId=u773c3112-daf5-4&from=paste&id=ud4d0bce9&originHeight=101&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucafc198a-a055-4b2e-9e9a-bd0162f8426&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332075-99205fb8-c609-47aa-baee-a485c9e98a28.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u1b6948cc&originHeight=267&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc53931c2-cd08-46f8-abdd-9c007d35bcb&title=)<br />默认的第一次注册，也就是延迟注册的时间是 40 秒，之后每 30 秒会同步注册信息。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332187-0aa06a5b-a389-433e-a1f8-625df8d72d9f.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u458af133&originHeight=475&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69b558bd-2a1c-4da3-ba63-5936bde73c3&title=)<br />但是，即便配置了这俩属性，发现好像没什么卵用，接下来要排查下到底是为啥？
<a name="XFlTq"></a>
## **第一个问题**
可以发现在 `InstanceInfoReplica` 中存在这样一段终止当前线程池任务，并且直接调用 run 方法的存在，猜测失效就是他直接调用导致延迟任务没有生效，因为这个方法的直接调用导致延迟注册压根就没效果嘛。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332075-ac77ff78-1c5d-4763-9978-5f92a6383d4d.jpeg#clientId=u773c3112-daf5-4&from=paste&id=uad07a5b9&originHeight=625&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud61581f9-d47b-4165-ae77-045d3299bdc&title=)<br />看起来他存在两个调用，第一个是`registerHealthCheck`，当存在这个健康检查什么玩意儿的时候就会去调用`onDemandUpdate`。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332640-70be787a-5213-44d7-b873-c05c0f34b570.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u802da73c&originHeight=431&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc288fdf-bd7f-4b8a-bdc4-cfdcfb6c0f0&title=)<br />经过排查可以发现，只要配置了`eureka.client.healthcheck.enabled=true`，就会创建 `HealthCheckHandler`的实例出来，默认情况下他是false的，所以应该是没有影响的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332792-d47c899a-1f04-475d-8356-45803e8eccd8.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u59d4346f&originHeight=530&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u06ea10b9-2473-4e41-bd9a-c1ef19b568f&title=)<br />这里需要特别说明一下 `eureka.client.healthcheck.enabled` 的作用，默认 Eureka 根据心跳来决定应用的状态，如果是这个属性配置成 true的话，则是会根据 Spring Boot Actuator 来决定，而不是心跳了。<br />比如可以实现 `HealthIndicator`接口，自己写一个Controller来动态改变服务的状态
```java
@RestController
public class ControllerTest {
	@Autowired
	private HealthChecker healthChecker;
	
	@RequestMapping("/change")
	public String test(Boolean flag) {
		healthChecker.setUp(new AtomicBoolean(flag));
		return "success";
	}
	
}
```
实现`HealthChecker`，这样会发现启动、下线服务 Eureka Server 的状态不会变成 Down，只有通过调用接口手动改变应用状态 Server 的状态才会发生改变，大家可以自行测试。
```java
@Component
public class HealthChecker extends EurekaHealthIndicator implements HealthIndicator {
	private AtomicBoolean up = new AtomicBoolean(true);
	
	public HealthChecker(EurekaClient eurekaClient, EurekaInstanceConfig instanceConfig, EurekaClientConfig clientConfig) {
		super(eurekaClient, instanceConfig, clientConfig);
	}
	
	@Override
	public Health health() {
		if(up.get()){
			return Health.up().build();
		}else{
			return Health.down().build();
		}
    }
```
<a name="NkQdA"></a>
## **第二个问题**
第一个问题找到了，发现他不是导致问题的根因，于是继续排查。<br />发现第二个调用，在`DiscoveryClient`注册了状态事件变更的监听，如果状态发生变更，也会去调用 `onDemandUpdate `，影响延迟注册的效果。<br />这里存在一个配置项`onDemandUpdateStatusChange`，默认是`true`，所以应该是他没错了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332720-4076c935-9b46-400c-9e03-22cb5be1a9b2.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u90895549&originHeight=556&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc07c6d05-08fe-4c64-bc31-09c736614db&title=)<br />进入`StatusChangeListener`，找到了一个调用。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332754-cec8b820-624d-4811-adba-b6ce5f3c8711.jpeg#clientId=u773c3112-daf5-4&from=paste&id=ueeefd429&originHeight=363&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u96852c59-8968-48b4-bf9c-9e7c26a60aa&title=)<br />就是通过`setInstanceStatus`方法触发的事件通知。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740332850-c715229c-5a52-4d3d-be29-db06712e6bc3.jpeg#clientId=u773c3112-daf5-4&from=paste&id=ueb4e3d6c&originHeight=666&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u21292695-cd36-4e29-bf28-10fe2da781c&title=)<br />这里存在 6 个调用，一一排查，通过源码找啊找，最终定位到服务启动自动装配的地方，在这里去修改服务状态为 UP，然后触发事件通知，启动 start 方法调用register方法。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333117-c72af940-b2f6-405c-9218-e180f3bb4928.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u8c89d1b6&originHeight=427&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8c50f977-d150-4eef-984c-e52782ee2fe&title=)<br />继续调用，修改应用为上线UP状态。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333196-4a6596f4-d953-4382-ba6f-49e36aed05ed.jpeg#clientId=u773c3112-daf5-4&from=paste&id=ua0ef7282&originHeight=303&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1de70d15-d14f-48f2-8e3b-be23ad3106a&title=)<br />由此可以知道，只要服务启动成功，就会触发事件通知，所以这个基本上是启动成功立刻就会去注册到 Eureka Server，这就会导致延迟注册的失效，从启动日志也能直观的看到这个效果。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333263-00b30a43-2f9b-4edf-a68a-1bce3ff137aa.jpeg#clientId=u773c3112-daf5-4&from=paste&id=ud17cc233&originHeight=169&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0c450c86-0ad7-430e-8cd7-b0fae279bf6&title=)
<a name="RKRUV"></a>
## **验证**
为了验证猜想，把这两个配置同时配置成false，并且把延迟注册的时间调整到非常大。
```
eureka.client.healthcheck.enabled=false
eureka.client.onDemandUpdateStatusChange=false
eureka.client.initial-instance-info-replication-interval-seconds=9999999 //默认40秒
eureka.client.instance-info-replication-interval-seconds=999999 //默认30秒
```
但是，但是！！！<br />发现过了几十秒之后，还是注册到 Server 了，真的是醉了。。。<br />那就继续看吧。<br />再看下注册方法，可能不止一个地方存在调用，发现果然如此，有 3 个地方都调用了注册方法。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333266-cdaab1aa-cb85-4829-8e06-18b0659ad348.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u25758f3f&originHeight=315&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u28a8ab6d-434b-4b66-8a05-d68c0d0e975&title=)<br />第一个调用在`DiscoveryClient`注入的时候，这个看了下，`clientConfig.shouldEnforceRegistrationAtInit()`默认是`false`，方法不会进来，不管他了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333284-77855f11-d408-4376-ac3f-614ae09700ce.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u97679187&originHeight=328&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua831fd53-e6de-4320-9252-2b8cf69bb34&title=)<br />那么继续看第二个调用，第二个调用看`renew`方法，这一看就知道了，这不就是心跳吗？！<br />发送心跳如果返回`NOT_FOUND`，就会去注册了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333645-2ab356e0-d732-4d3c-b8f7-094b7414bf2a.jpeg#clientId=u773c3112-daf5-4&from=paste&id=uc6c70924&originHeight=435&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc8b7dcf7-360a-48e2-be08-d27ceb08c11&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333585-d8a2ad37-d602-4ec8-b797-e2678956d1f3.jpeg#clientId=u773c3112-daf5-4&from=paste&id=u5be7a108&originHeight=441&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaf337725-582a-425a-9067-4a661a5f382&title=)<br />感觉已经接近真相了，去找下 Server 心跳的源码，根据调用的路径找到源码位于`InstanceResource`中。<br />可以看到第一次注册的时候从注册表拿到的实例信息是空的，所以直接返回了 false，就会返回 NOT FOUND 了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333659-6fd64dee-c83d-4b9f-ae00-bd407fd2eb9b.jpeg#clientId=u773c3112-daf5-4&from=paste&id=uc0f793aa&originHeight=516&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc81e1946-976b-4a92-9f87-498bd1f2f86&title=)<br />看`registry.renew`方法，最终会调用到`AbstractInstanceRegistry`中，初始化的时候注册表`registry`肯定没有当前实例的信息，所以拿到是空的，返回了false，最终就返回了`NOT_FOUND`。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658740333666-9a39efe9-3cdc-48e5-84ca-00aa1d6a6b53.jpeg#clientId=u773c3112-daf5-4&from=paste&id=ub4b1ab80&originHeight=313&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc2b84c98-c1c5-4364-bd53-665b3a7883a&title=)<br />因此，虽然把这两个参数都设置成了`false`，但是由于心跳默认 30 秒一次，所以最终发现配置的超级大的延迟注册的时间并没有完全生效。
<a name="cSgqX"></a>
## **总结**
延迟注册不生效的原因找到了，做一个总结。<br />默认情况下，配置了延迟注册的时间并不会生效，因为事件监听默认是true，服务启动之后就会立刻注册到 Eureka Server。<br />如果需要延迟注册生效，必须 `eureka.client.healthcheck.enabled` 、`eureka.client.onDemandUpdateStatusChange` 都为`false`。<br />即便把所有途径都封死了，但是发送心跳的线程仍然会去注册，所以这个延迟注册的时间最多也不会超过 30 秒，即便配置的延迟时间超过 30 秒。
