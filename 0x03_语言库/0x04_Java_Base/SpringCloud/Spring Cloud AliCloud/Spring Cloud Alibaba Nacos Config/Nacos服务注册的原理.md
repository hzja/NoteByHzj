Java SpringCloud Nacos
<a name="CMNXm"></a>
### Nacos 服务注册需要具备的能力：

- 服务提供者把自己的协议地址注册到Nacos server
- 服务消费者需要从Nacos Server上去查询服务提供者的地址（根据服务名称）
- Nacos Server需要感知到服务提供者的上下线的变化
- 服务消费者需要动态感知到Nacos Server端服务地址的变化

作为注册中心所需要的能力大多如此，需要做的是理解各种注册中心的独有特性，总结他们的共性。
<a name="Kn1R6"></a>
### Nacos的实现原理：
下面先来了解一下 Nacos 注册中心的实现原理，通过下面这张图来说明。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635670417730-d95cd41a-b7a6-43ba-9a7a-d4417c1f4da8.webp#clientId=u52e5b218-0840-4&from=paste&id=u23d137fd&originHeight=490&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uac2fa47b-4852-48b4-81b9-c8f0ebffd4d)<br />Nacos 注册中心的实现原理<br />图中的流程是大家所熟悉的，不同的是在Nacos 中，服务注册时在服务端本地会通过轮询注册中心集群节点地址进行服务得注册，在注册中心上，即Nacos Server上采用了Map保存实例信息，当然配置了持久化的服务会被保存到数据库中，在服务的调用方，为了保证本地服务实例列表的动态感知，Nacos与其他注册中心不同的是，采用了 Pull/Push同时运作的方式。通过这些对Nacos注册中心的原理有了一定的了解。从源码层面去验证这些理论知识。
<a name="VOLpG"></a>
### Nacos的源码分析（结合spring-cloud-alibaba +dubbo +nacos 的整合）：
「服务注册的流程：」<br />在基于Dubbo服务发布的过程中， 自动装配是走的事件监听机制，在 `DubboServiceRegistrationNonWebApplicationAutoConfiguration` 这个类中，这个类会监听 `ApplicationStartedEvent` 事件，这个事件是spring boot在2.0新增的，就是当spring boot应用启动完成之后会发布这个时间。而此时监听到这个事件之后，会触发注册的动作。
```java
@EventListener(ApplicationStartedEvent.class)
public void onApplicationStarted() {
    setServerPort();
    register();
}

private void register() {
    if (registered) {
        return;
    }
    serviceRegistry.register(registration);
    registered = true;
}
```
`serviceRegistry`是 spring-cloud 提供的接口实现(`org.springframework.cloud.client.serviceregistry.ServiceRegistry`)，很显然注入的实例是：`NacosServiceRegistry`。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635670417559-64ed48a5-0357-431e-add4-8ce30984a0a0.webp#clientId=u52e5b218-0840-4&from=paste&id=u1481bfb3&originHeight=260&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u8059e742-7132-4286-9dfd-2e30e52b84e)<br />NacosServiceRegistry<br />然后进入到实现类的注册方法：
```java
@Override
public void register(Registration registration) {

    if (StringUtils.isEmpty(registration.getServiceId())) {
        log.warn("No service to register for nacos client...");
        return;
    }
    //对应当前应用的application.name
    String serviceId = registration.getServiceId();
    //表示nacos上的分组配置
    String group = nacosDiscoveryProperties.getGroup();
    //表示服务实例信息
    Instance instance = getNacosInstanceFromRegistration(registration);

    try {
        //通过命名服务进行注册
        namingService.registerInstance(serviceId, group, instance);
        log.info("nacos registry, {} {} {}:{} register finished", group, serviceId,
                 instance.getIp(), instance.getPort());
    }
    catch (Exception e) {
        log.error("nacos registry, {} register failed...{},", serviceId,
                  registration.toString(), e);
        // rethrow a RuntimeException if the registration is failed.
        // issue : https://github.com/alibaba/spring-cloud-alibaba/issues/1132
        rethrowRuntimeException(e);
    }
}    
```
接下去就是开始注册实例，主要做两个动作

1. 如果当前注册的是临时节点，则构建心跳信息，通过beat反应堆来构建心跳任务
2. 调用`registerService`发起服务注册
```java
@Override
public void registerInstance(String serviceName, String groupName, Instance instance) throws NacosException {
    ////是否是临时节点，如果是临时节点，则构建心跳信息
    if (instance.isEphemeral()) {
        BeatInfo beatInfo = new BeatInfo();
        beatInfo.setServiceName(NamingUtils.getGroupedName(serviceName, groupName));
        beatInfo.setIp(instance.getIp());
        beatInfo.setPort(instance.getPort());
        beatInfo.setCluster(instance.getClusterName());
        beatInfo.setWeight(instance.getWeight());
        beatInfo.setMetadata(instance.getMetadata());
        beatInfo.setScheduled(false);

        //beatReactor， 添加心跳信息进行处理
        beatReactor.addBeatInfo(NamingUtils.getGroupedName(serviceName, groupName), beatInfo);
    }
    //调用服务代理类进行注册   
    serverProxy.registerService(NamingUtils.getGroupedName(serviceName, groupName), groupName, instance);
}
```
然后调用 NamingProxy  的注册方法进行注册，代码逻辑很简单，构建请求参数，发起请求。
```java
public void registerService(String serviceName, String groupName, Instance instance) throws NacosException {

    NAMING_LOGGER.info("[REGISTER-SERVICE] {} registering service {} with instance: {}",
                       namespaceId, serviceName, instance);

    final Map<String, String> params = new HashMap<String, String>(8);
    params.put(CommonParams.NAMESPACE_ID, namespaceId);
    params.put(CommonParams.SERVICE_NAME, serviceName);
    params.put(CommonParams.GROUP_NAME, groupName);
    params.put(CommonParams.CLUSTER_NAME, instance.getClusterName());
    params.put("ip", instance.getIp());
    params.put("port", String.valueOf(instance.getPort()));
    params.put("weight", String.valueOf(instance.getWeight()));
    params.put("enable", String.valueOf(instance.isEnabled()));
    params.put("healthy", String.valueOf(instance.isHealthy()));
    params.put("ephemeral", String.valueOf(instance.isEphemeral()));
    params.put("metadata", JSON.toJSONString(instance.getMetadata()));

    reqAPI(UtilAndComs.NACOS_URL_INSTANCE, params, HttpMethod.POST);

}
```
往下走就会发现上面提到的，服务在进行注册的时候会轮询配置好的注册中心的地址：
```java
public String reqAPI(String api, Map<String, String> params, List<String> servers, String method) {

    params.put(CommonParams.NAMESPACE_ID, getNamespaceId());

    if (CollectionUtils.isEmpty(servers) && StringUtils.isEmpty(nacosDomain)) {
        throw new IllegalArgumentException("no server available");
    }

    Exception exception = new Exception();
    //如果服务地址不为空
    if (servers != null && !servers.isEmpty()) {
        //随机获取一台服务器节点
        Random random = new Random(System.currentTimeMillis());
        int index = random.nextInt(servers.size());
        // 遍历服务列表
        for (int i = 0; i < servers.size(); i++) {
            String server = servers.get(index);//获得索引位置的服务节点
            try {//调用指定服务
                return callServer(api, params, server, method);
            } catch (NacosException e) {
                exception = e;
                NAMING_LOGGER.error("request {} failed.", server, e);
            } catch (Exception e) {
                exception = e;
                NAMING_LOGGER.error("request {} failed.", server, e);
            }
            //轮询
            index = (index + 1) % servers.size();
        }
        // ..........
    }
```
最后通过 `callServer(api, params, server, method)` 发起调用，这里通过 JSK自带的 HttpURLConnection 进行发起调用。可以通过断点的方式来看到这里的请求参数：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635670417638-8a5d134a-8362-4dbe-92ff-6828cb127bbb.webp#clientId=u52e5b218-0840-4&from=paste&id=u1d2fc323&originHeight=300&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u03b6899f-a92b-4870-b7c3-abd0b43526a)<br />HttpURLConnection 进行发起调用<br />期间可能会有多个 GET 的请求获取服务列表，是正常的，会发现有如上的一个请求，会调用 http://192.168.200.1:8848/nacos/v1/ns/instance 这个地址。那么接下去就是Nacos Server 接受到服务端的注册请求的处理流程。需要下载Nacos Server 源码，源码下载可以参考官方文档，本文不做过多阐述。<br />「Nacos服务端的处理：」<br />服务端提供了一个InstanceController类，在这个类中提供了服务注册相关的API，而服务端发起注册时，调用的接口是：[post]: /nacos/v1/ns/instance ，serviceName: 代表客户端的项目名称 ，namespace: nacos 的namespace。
```java
@CanDistro
@PostMapping
@Secured(parser = NamingResourceParser.class, action = ActionTypes.WRITE)
public String register(HttpServletRequest request) throws Exception {

    final String serviceName = WebUtils.required(request, CommonParams.SERVICE_NAME);
    final String namespaceId = WebUtils
        .optional(request, CommonParams.NAMESPACE_ID, Constants.DEFAULT_NAMESPACE_ID);
    // 从请求中解析出instance实例
    final Instance instance = parseInstance(request);

    serviceManager.registerInstance(namespaceId, serviceName, instance);
    return "ok";
}
```
然后调用 ServiceManager 进行服务的注册
```java
public void registerInstance(String namespaceId, String serviceName, Instance instance) throws NacosException {
    //创建一个空服务，在Nacos控制台服务列表展示的服务信息，实际上是初始化一个serviceMap，它是一个ConcurrentHashMap集合
    createEmptyService(namespaceId, serviceName, instance.isEphemeral());
    //从serviceMap中，根据namespaceId和serviceName得到一个服务对象
    Service service = getService(namespaceId, serviceName);

    if (service == null) {
        throw new NacosException(NacosException.INVALID_PARAM,
                                 "service not found, namespace: " + namespaceId + ", service: " + serviceName);
    }
    //调用addInstance创建一个服务实例
    addInstance(namespaceId, serviceName, instance.isEphemeral(), instance);
}
```
在创建空的服务实例的时候发现了存储实例的map：
```java
public void createServiceIfAbsent(String namespaceId, String serviceName, boolean local, Cluster cluster)
    throws NacosException {
    //从serviceMap中获取服务对象
    Service service = getService(namespaceId, serviceName);
    if (service == null) {//如果为空。则初始化
        Loggers.SRV\_LOG.info("creating empty service {}:{}", namespaceId, serviceName);
        service = new Service();
        service.setName(serviceName);
        service.setNamespaceId(namespaceId);
        service.setGroupName(NamingUtils.getGroupName(serviceName));
        // now validate the service. if failed, exception will be thrown
        service.setLastModifiedMillis(System.currentTimeMillis());
        service.recalculateChecksum();
        if (cluster != null) {
            cluster.setService(service);
            service.getClusterMap().put(cluster.getName(), cluster);
        }
        service.validate();
        putServiceAndInit(service);
        if (!local) {
            addOrReplaceService(service);
        }
    }
```
在 `getService` 方法中发现了Map：
```java
/*
* Map(namespace, Map(group::serviceName, Service)).
*/
private final Map<String, Map<String, Service>> serviceMap = new ConcurrentHashMap<>();
```
通过注释可以知道，Nacos是通过不同的 namespace 来维护服务的，而每个namespace下有不同的group，不同的group下才有对应的Service ，再通过这个 serviceName 来确定服务实例。<br />第一次进来则会进入初始化，初始化完会调用 `putServiceAndInit`
```java
private void putServiceAndInit(Service service) throws NacosException {
    putService(service);//把服务信息保存到serviceMap集合
    service.init();//建立心跳检测机制
    //实现数据一致性监听，ephemeral(标识服务是否为临时服务，默认是持久化的，也就是true)=true表示采用raft协议，false表示采用Distro
    consistencyService
        .listen(KeyBuilder.buildInstanceListKey(service.getNamespaceId(), service.getName(), true), service);
    consistencyService
        .listen(KeyBuilder.buildInstanceListKey(service.getNamespaceId(), service.getName(), false), service);
    Loggers.SRV_LOG.info("[NEW-SERVICE] {}", service.toJson());
}
```
获取到服务以后把服务实例添加到集合中，然后基于一致性协议进行数据的同步。然后调用 addInstance
```java
public void addInstance(String namespaceId, String serviceName, boolean ephemeral, Instance... ips)
    throws NacosException {
    // 组装key
    String key = KeyBuilder.buildInstanceListKey(namespaceId, serviceName, ephemeral);
    // 获取刚刚组装的服务
    Service service = getService(namespaceId, serviceName);

    synchronized (service) {
        List<Instance> instanceList = addIpAddresses(service, ephemeral, ips);

        Instances instances = new Instances();
        instances.setInstanceList(instanceList);
        // 也就是上一步实现监听的类里添加注册服务
        consistencyService.put(key, instances);
    }
}
```
然后给服务注册方发送注册成功的响应，结束服务注册流程。
