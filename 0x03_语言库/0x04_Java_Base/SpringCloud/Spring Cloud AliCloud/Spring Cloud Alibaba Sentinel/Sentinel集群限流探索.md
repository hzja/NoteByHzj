Java SpringCloudSentinel<br />最近看了下关于分布式限流的部分，看到Sentinel的分布式限流，也就是集群限流的部分，想搭个环境看看，结果发现网上关于这方面的内容基本可以说没有，甚至很难跑起来他的demo，就算能跑起来，估计也得自己研究半天，麻烦的要死。<br />很重要的原因可能就是Sentinel关于这块做的并不完善，而且从官方的Issue中能看出来，其实官方对于这块后续并没有计划去做的更好。<br />在此之前，肯定要先说下关于Sentinel集群限流方面的原理，没有原理一切都是空中楼阁。
<a name="cODuo"></a>
## 集群限流原理
原理这方面比较好解释，就是在原本的限流规则中加了一个clusterMode参数，如果是true的话，那么会走集群限流的模式，反之就是单机限流。<br />如果是集群限流，判断身份是限流客户端还是限流服务端，客户端则和服务端建立通信，所有的限流都通过和服务端的交互来达到效果。<br />对于Sentinel集群限流，包含两种模式，内嵌式和独立式。
<a name="G17lF"></a>
### 内嵌式
什么是内嵌式呢，简单来说，要限流那么必然要有个服务端去处理多个客户端的限流请求，对于内嵌式来说呢，就是整个微服务集群内部选择一台机器节点作为限流服务端（Sentinel把这个叫做token-server），其他的微服务机器节点作为限流的客户端（token-client），这样的做法有缺点也有优点。<br />![限流-嵌入式](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055331581-eb4e3eae-7877-40b9-a87d-33e0bc81e33d.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=u7787878b&originHeight=807&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u0d925de8-4fa1-496d-baff-a9870dfeccf&title=%E9%99%90%E6%B5%81-%E5%B5%8C%E5%85%A5%E5%BC%8F "限流-嵌入式")<br />首先说优点：这种方式部署**不需要独立部署限流服务端**，**节省独立部署服务端产生的额外服务器开支**，**降低部署和维护复杂度**。<br />再说缺点，缺点的话也可以说是整个Sentinel在集群限流这方面做得不够好的问题。<br />先说第一个缺点：**无自动故障转移机制**。<br />无论是内嵌式还是独立式的部署方案，都无法做到自动的故障转移。<br />所有的server和client都需要事先知道IP的请求下做出配置，如果server挂了，需要手动的修改配置，否则集群限流会退化成单机限流。<br />比如交易服务有3台机器A\B\C，其中A被手动设置为server，B\C则是作为client，当A服务器宕机之后，需要手动修改B\C中一台作为server，否则整个集群的机器都将退化回单机限流的模式。<br />但是，如果client挂了，则是不会影响到整个集群限流的，比如B挂了，那么A和C将会继续组成集群限流。<br />如果B再次重启成功，那么又会重新加入到整个集群限流当中来，因为会有一个自动重连的机制，默认的时间是N*2秒，逐渐递增的一个时间。<br />这是想用Sentinel做集群限流并且使用内嵌式需要考虑的问题，要自己去实现自动故障转移的机制，当然，server节点选举也要自己实现了。<br />对于这个问题，官方提供了可以修改server/client的API接口，另外一个就是可以基于动态的数据源配置方式，这个后面再谈。<br />第二个缺点：**适用于单微服务集群内部限流**。<br />这个其实也是显而易见的道理，都内部选举一台作为server去限流了，如果还跨多个微服务的话，显然是不太合理的行为，现实中这种情况肯定也是非常少见的了，当然非要想跨多个微服务集群也不是不可以。<br />第三个缺点：**server节点的机器性能会受到一定程度的影响**。<br />这个肯定也比较好理解的，作为server去限流，那么其他的客户端肯定要和server去通信才能做到集群限流，对不对，所以一定程度上肯定会影响到server节点本身服务的性能，但是应该问题不大，就当server节点多了一个流量比较大的接口好了。<br />具体上会有多大的影响，没有实际对这块做出实际的测试，如果真的流量非常大，需要实际测试一下这方面的问题。<br />影响还是可控的，本身server和client基于netty通信，通信的内容其实也非常的小。
<a name="a8g9f"></a>
### 独立式
说完内嵌式的这些点，然后再说独立式，也非常好理解，就是单独部署一台机器作为限流服务端server，就不在本身微服务集群内部选一台作为server了。<br />![限流-独立式](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055331682-0efad4c6-99ff-472d-914b-f9b061e9ed29.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=u71c96a36&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5710a295-36c4-4670-a052-817aee3456e&title=%E9%99%90%E6%B5%81-%E7%8B%AC%E7%AB%8B%E5%BC%8F "限流-独立式")<br />很明显，优点就是解决了上面的缺点。

1. 不会和内嵌式一样，影响到server节点的本身性能
2. 可以适用于跨多个微服务之间的集群限流

优点可以说就是解决了内嵌式的两个缺点，那么缺点也来了，这同样也是Sentinel本身并没有去解决的问题。<br />缺点一：**需要独立部署，会产生额外的资源（钱）和运维复杂度**<br />缺点二：**server默认是单机，需要自己实现高可用方案**<br />缺点二很致命，官方的server实现默认就是单机的，单点问题大家懂的都懂，需要自己实现高可用。<br />这么说Sentinel这个集群限流就是简单的实现了一下，真正复杂的部分他都没管，可以这么理解。
<a name="Ecgak"></a>
## run起来
那基本原理大概了解之后，还是要真正跑起来看看效果的，毕竟开头就说了，网上这方面真的是感觉啥也搜不到，下面以嵌入式集群的方式举例。<br />无论集群限流还是单机限流的方式，官方都支持写死配置和动态数据源的配置方式，写的话下面的代码中也都有，被注释掉了，至于动态数据源的配置，会基于Apollo来实现。<br />理解一下动态数据源的配置方式，基于这个可以实现限流规则的动态刷新，还有重点的一点可以做到基于修改配置方式的半自动故障转移。<br />动态数据源支持推和拉两种方式，比如文件系统和Eureka就是拉取的方式，定时读取文件内容的变更，Eureka则是建立HTTP连接，定时获取元数据的变更。<br />推送的方式主要是基于事件监听机制，比如Apollo和Nacos，Redis官方则是基于Pub/Sub来实现，默认的实现方式是基于Lettuce，如果想用其他的客户端要自己实现。<br />![限流-集群工作模式](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055331881-c5c1080b-ca10-4f25-b6bd-6c76db57dd91.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=u714ce21d&originHeight=452&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u327131b8-09ff-441f-9793-b64f739244d&title=%E9%99%90%E6%B5%81-%E9%9B%86%E7%BE%A4%E5%B7%A5%E4%BD%9C%E6%A8%A1%E5%BC%8F "限流-集群工作模式")<br />首先，该引入的包还是引入。
```xml
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-annotation-aspectj</artifactId>
	<version>1.8.4</version>
</dependency>

<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-transport-simple-http</artifactId>
	<version>1.8.4</version>
</dependency>

<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-cluster-client-default</artifactId>
	<version>1.8.4</version>
</dependency>
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-cluster-server-default</artifactId>
	<version>1.8.4</version>
</dependency>
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-datasource-apollo</artifactId>
	<version>1.8.4</version>
</dependency>
```
实现SPI，在resources目录的META-INF/services下新增名为`com.alibaba.csp.sentinel.init.InitFunc`的文件，内容写上自己实现的类名，比如com.fcant.demo.init.DemoClusterInitFunc。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055331798-f6050e26-54bd-4d76-af0f-2ba7537294ea.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=u5d27241a&originHeight=200&originWidth=1018&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f960c7c-e820-458d-a440-9ab9d174bc5&title=)<br />实现`InitFunc`接口，重写`init`方法，代码直接贴出来，这里整体依赖的是Apollo的配置方式，注释的部分是在测试的时候写死代码的配置方式，也是可以用的。
```java
public class DemoClusterInitFunc implements InitFunc {
    private final String namespace = "application";
    private final String ruleKey = "demo_sentinel";
    private final String ruleServerKey = "demo_cluster";
    private final String defaultRuleValue = "[]";

    @Override
    public void init() throws Exception {
        // 初始化 限流规则
        initDynamicRuleProperty();
        //初始化 客户端配置
        initClientConfigProperty();
        // 初始化 服务端配置信息
        initClientServerAssignProperty();
        registerClusterRuleSupplier();
        // token-server的传输规则
        initServerTransportConfigProperty();
        // 初始化 客户端和服务端状态
        initStateProperty();
    }

    /**
     * 限流规则和热点限流规则配置
     */
    private void initDynamicRuleProperty() {
        ReadableDataSource<String, List<FlowRule>> ruleSource = new ApolloDataSource<>(namespace, ruleKey,
                defaultRuleValue, source -> JSON.parseObject(source, new TypeReference<List<FlowRule>>() {
        }));
        FlowRuleManager.register2Property(ruleSource.getProperty());

        ReadableDataSource<String, List<ParamFlowRule>> paramRuleSource = new ApolloDataSource<>(namespace, ruleKey,
                defaultRuleValue, source -> JSON.parseObject(source, new TypeReference<List<ParamFlowRule>>() {
        }));
        ParamFlowRuleManager.register2Property(paramRuleSource.getProperty());
    }

    /**
     * 客户端配置，注释的部分是通过Apollo配置，只有一个配置就省略了
     */
    private void initClientConfigProperty() {
//        ReadableDataSource<String, ClusterClientConfig> clientConfigDs = new ApolloDataSource<>(namespace, ruleKey,
//                defaultRuleValue, source -> JSON.parseObject(source, new TypeReference<ClusterClientConfig>() {
//        }));
//        ClusterClientConfigManager.registerClientConfigProperty(clientConfigDs.getProperty());

        ClusterClientConfig clientConfig = new ClusterClientConfig();
        clientConfig.setRequestTimeout(1000);
        ClusterClientConfigManager.applyNewConfig(clientConfig);
    }

    /**
     * client->server 传输配置，设置端口号，注释的部分是写死的配置方式
     */
    private void initServerTransportConfigProperty() {
        ReadableDataSource<String, ServerTransportConfig> serverTransportDs = new ApolloDataSource<>(namespace, ruleServerKey,
                defaultRuleValue, source -> {
            List<ClusterGroupEntity> groupList = JSON.parseObject(source, new TypeReference<List<ClusterGroupEntity>>() {
            });
            ServerTransportConfig serverTransportConfig = Optional.ofNullable(groupList)
                    .flatMap(this::extractServerTransportConfig)
                    .orElse(null);
            return serverTransportConfig;
        });
        ClusterServerConfigManager.registerServerTransportProperty(serverTransportDs.getProperty());
//        ClusterServerConfigManager.loadGlobalTransportConfig(new ServerTransportConfig().setIdleSeconds(600).setPort(transPort));
    }

    private void registerClusterRuleSupplier() {
        ClusterFlowRuleManager.setPropertySupplier(namespace -> {
            ReadableDataSource<String, List<FlowRule>> ds = new ApolloDataSource<>(this.namespace, ruleKey,
                    defaultRuleValue, source -> JSON.parseObject(source, new TypeReference<List<FlowRule>>() {
            }));
            return ds.getProperty();
        });
        ClusterParamFlowRuleManager.setPropertySupplier(namespace -> {
            ReadableDataSource<String, List<ParamFlowRule>> ds = new ApolloDataSource<>(this.namespace, ruleKey,
                    defaultRuleValue, source -> JSON.parseObject(source, new TypeReference<List<ParamFlowRule>>() {
            }));
            return ds.getProperty();
        });
    }

    /**
     * 服务端配置，设置server端口和IP，注释的配置是写死的方式，这个在服务端是不用配置的，只有客户端需要配置用来连接服务端
     */
    private void initClientServerAssignProperty() {
        ReadableDataSource<String, ClusterClientAssignConfig> clientAssignDs = new ApolloDataSource<>(namespace, ruleServerKey,
                defaultRuleValue, source -> {
            List<ClusterGroupEntity> groupList = JSON.parseObject(source, new TypeReference<List<ClusterGroupEntity>>() {
            });

            ClusterClientAssignConfig clusterClientAssignConfig = Optional.ofNullable(groupList)
                    .flatMap(this::extractClientAssignment)
                    .orElse(null);
            return clusterClientAssignConfig;
        });
        ClusterClientConfigManager.registerServerAssignProperty(clientAssignDs.getProperty());

//        ClusterClientAssignConfig serverConfig = new ClusterClientAssignConfig();
//        serverConfig.setServerHost("127.0.0.1");
//        serverConfig.setServerPort(transPort);
//        ConfigSupplierRegistry.setNamespaceSupplier(() -> "trade-center");
//        ClusterClientConfigManager.applyNewAssignConfig(serverConfig);
    }

    private Optional<ClusterClientAssignConfig> extractClientAssignment(List<ClusterGroupEntity> groupList) {
        ClusterGroupEntity tokenServer = groupList.stream().filter(x -> x.getState().equals(ClusterStateManager.CLUSTER_SERVER)).findFirst().get();
        Integer currentMachineState = Optional.ofNullable(groupList).map(s -> groupList.stream().filter(this::machineEqual).findFirst().get().getState()).orElse(ClusterStateManager.CLUSTER_NOT_STARTED);
        if (currentMachineState.equals(ClusterStateManager.CLUSTER_CLIENT)) {
            String ip = tokenServer.getIp();
            Integer port = tokenServer.getPort();
            return Optional.of(new ClusterClientAssignConfig(ip, port));
        }
        return Optional.empty();
    }

    /**
     * 初始化客户端和服务端状态，注释的也是写死的配置方式
     */
    private void initStateProperty() {
        ReadableDataSource<String, Integer> clusterModeDs = new ApolloDataSource<>(namespace, ruleServerKey,
                defaultRuleValue, source -> {
            List<ClusterGroupEntity> groupList = JSON.parseObject(source, new TypeReference<List<ClusterGroupEntity>>() {
            });
            Integer state = Optional.ofNullable(groupList).map(s -> groupList.stream().filter(this::machineEqual).findFirst().get().getState()).orElse(ClusterStateManager.CLUSTER_NOT_STARTED);
            return state;
        });
        ClusterStateManager.registerProperty(clusterModeDs.getProperty());

//            ClusterStateManager.applyState(ClusterStateManager.CLUSTER_SERVER);

    }

    private Optional<ServerTransportConfig> extractServerTransportConfig(List<ClusterGroupEntity> groupList) {
        return groupList.stream()
                .filter(x -> x.getMachineId().equalsIgnoreCase(getCurrentMachineId()) && x.getState().equals(ClusterStateManager.CLUSTER_SERVER))
                .findAny()
                .map(e -> new ServerTransportConfig().setPort(e.getPort()).setIdleSeconds(600));
    }

    private boolean machineEqual(/*@Valid*/ ClusterGroupEntity group) {
        return getCurrentMachineId().equals(group.getMachineId());
    }

    private String getCurrentMachineId() {
        // 通过-Dcsp.sentinel.api.port=8719 配置， 默认8719，随后递增
        return HostNameUtil.getIp() + SEPARATOR + TransportConfig.getPort();
    }
  
    private static final String SEPARATOR = "@";
}
```
基础类，定义配置的基础信息。
```java
@Data
public class ClusterGroupEntity {
    private String machineId;
    private String ip;
    private Integer port;
    private Integer state;
}
```
然后是Apollo中的限流规则的配置和server/client集群关系的配置。<br />需要说明一下的就是flowId，这个是区分限流规则的全局唯一ID，必须要有，否则集群限流会有问题。<br />thresholdType代表限流模式，默认是0，代表单机均摊，比如这里count限流QPS=20，有3台机器，那么集群限流阈值就是60，如果是1代表全局阈值，也就是count配置的值就是集群限流的上限。
```xml
demo_sentinel=[
    {
        "resource": "test_res", //限流资源名
        "count": 20, //集群限流QPS
        "clusterMode": true, //true为集群限流模式
        "clusterConfig": {
            "flowId": 111, //这个必须得有，否则会有问题
            "thresholdType": 1 //限流模式，默认为0单机均摊，1是全局阈值
        }
    }
]
demo_cluster=[
    {
        "ip": "192.168.3.20",
        "machineId": "192.168.3.20@8720",
        "port": 9999, //server和client通信接口
        "state": 1 //指定为server
    },
    {
        "ip": "192.168.3.20",
        "machineId": "192.168.3.20@8721",
        "state": 0
    },
    {
        "ip": "192.168.3.20",
        "machineId": "192.168.3.20@8722",
        "state": 0
    }
]
```
OK，到这里代码和配置都已经OK，还需要跑起来Sentinel控制台，这个不用教，还有启动参数。<br />本地可以直接跑多个客户端，注意修改端口号：`-Dserver.port=9100 -Dcsp.sentinel.api.port=8720`这两个一块改，至于怎么连Apollo这块就省略了，不行的话用代码里的写死的方式也可以用。
```bash
-Dserver.port=9100 -Dcsp.sentinel.api.port=8720 -Dcsp.sentinel.dashboard.server=localhost:8080 -Dcsp.sentinel.log.use.pid=true 
```
因为有流量之后控制台才能看到限流的情况，所以用官方给的限流测试代码修改一下，放到Springboot启动类中，触发限流规则的初始化。
```java
@SpringBootApplication
public class DemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
        new FlowQpsDemo();
    }
}
```
测试限流代码：
```java
public class FlowQpsDemo {

    private static final String KEY = "test_res";

    private static AtomicInteger pass = new AtomicInteger();
    private static AtomicInteger block = new AtomicInteger();
    private static AtomicInteger total = new AtomicInteger();

    private static volatile boolean stop = false;

    private static final int threadCount = 32;

    private static int seconds = 60 + 40;

    public FlowQpsDemo() {
        tick();
        simulateTraffic();
    }

    private static void simulateTraffic() {
        for (int i = 0; i < threadCount; i++) {
            Thread t = new Thread(new RunTask());
            t.setName("simulate-traffic-Task");
            t.start();
        }
    }

    private static void tick() {
        Thread timer = new Thread(new TimerTask());
        timer.setName("sentinel-timer-task");
        timer.start();
    }

    static class TimerTask implements Runnable {

        @Override
        public void run() {
            long start = System.currentTimeMillis();
            System.out.println("begin to statistic!!!");

            long oldTotal = 0;
            long oldPass = 0;
            long oldBlock = 0;
            while (!stop) {
                try {
                    TimeUnit.SECONDS.sleep(1);
                } catch (InterruptedException e) {
                }
                long globalTotal = total.get();
                long oneSecondTotal = globalTotal - oldTotal;
                oldTotal = globalTotal;

                long globalPass = pass.get();
                long oneSecondPass = globalPass - oldPass;
                oldPass = globalPass;

                long globalBlock = block.get();
                long oneSecondBlock = globalBlock - oldBlock;
                oldBlock = globalBlock;

                System.out.println(seconds + " send qps is: " + oneSecondTotal);
                System.out.println(TimeUtil.currentTimeMillis() + ", total:" + oneSecondTotal
                        + ", pass:" + oneSecondPass
                        + ", block:" + oneSecondBlock);

                if (seconds-- <= 0) {
//                    stop = true;
                }
            }

            long cost = System.currentTimeMillis() - start;
            System.out.println("time cost: " + cost + " ms");
            System.out.println("total:" + total.get() + ", pass:" + pass.get()
                    + ", block:" + block.get());
            System.exit(0);
        }
    }

    static class RunTask implements Runnable {
        @Override
        public void run() {
            while (!stop) {
                Entry entry = null;

                try {
                    entry = SphU.entry(KEY);
                    // token acquired, means pass
                    pass.addAndGet(1);
                } catch (BlockException e1) {
                    block.incrementAndGet();
                } catch (Exception e2) {
                    // biz exception
                } finally {
                    total.incrementAndGet();
                    if (entry != null) {
                        entry.exit();
                    }
                }

                Random random2 = new Random();
                try {
                    TimeUnit.MILLISECONDS.sleep(random2.nextInt(50));
                } catch (InterruptedException e) {
                    // ignore
                }
            }
        }
    }
}
```
启动之后查看控制台，可以看到嵌入式的集群服务端已经启动好。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055331927-610ad95c-123a-4670-9316-8cf2afa7a7ca.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=ub696cd09&originHeight=144&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u70d17349-dd5d-4309-873d-3dd20c42843&title=)<br />查看限流的情况：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055331910-c882e1b1-50c7-42df-9786-48b1411950ce.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=u110e21bc&originHeight=293&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u05b10e5e-a91a-4d92-a836-4980a630792&title=)<br />最后为了测试效果，再启动一个客户端，修改端口号为9200和8721，可以看到新的客户端已经连接到了服务端，不过这里显示的总QPS 30000和配置的不符，这个不用管他。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055332189-8166ce99-9954-42a6-a4e9-eec7dbba7026.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=u87fa989b&originHeight=144&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1d641e20-7b44-420c-a99f-e4d72434219&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652055332345-e459aea9-e463-4612-b248-c25dd19ac07e.jpeg#clientId=u739ca4ae-dc9a-4&from=paste&id=ua7546e67&originHeight=504&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a995cc6-775f-4d19-8f62-c81f4b88961&title=)<br />好了，这个就是集群限流原理和使用配置方式，当然了，可以启动多台服务，然后手动修改Apollo中的`state`参数修改服务端，验证修改配置的方式是否能实现故障转移机制，另外就是关闭client或者server验证是否回退到单机限流的情况，这里就不一一测试了。<br />对于独立式的部署方式基本也是一样的，只是单独启动一个服务端的服务，需要手动配置server，而嵌入式的则不需要，`loadServerNamespaceSet`配置为自己的服务名称即可。
```java
ClusterTokenServer tokenServer = new SentinelDefaultTokenServer();

ClusterServerConfigManager.loadGlobalTransportConfig(new ServerTransportConfig()
													 .setIdleSeconds(600)
													 .setPort(11111));
ClusterServerConfigManager.loadServerNamespaceSet(Collections.singleton(DemoConstants.APP_NAME));

tokenServer.start();
```
