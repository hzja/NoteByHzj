服务上线后由于 bug 修复、扩容、或者发现了更好的方法进行了重构等原因，总免不了需要发布新版本，进行系统变更升级。服务变更过程本身也是引起服务不可用的重要原因。为了尽量降低可能出现故障而造成的损失，比较流行的思路是采用灰度发布策略，逐步增加流量导入新版本服务实例上，直至将所有流量切到新版本，下线旧版本。由于，spring cloud gateway 作为整个系统的入口，在 spring cloud gateway 上实施流量管控策略，也是顺利成章。本文就尝试介绍基于 spring cloud gateway 的灰度发布方法。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1700990231985-c3006ef1-f59f-4398-9626-ae5f368301b7.jpeg)
<a name="U38tW"></a>
## 部署和发布
在大家的一般印象中，服务的部署和发布是一体的，服务部署后就发布上线使用了，服务发布就是通过部署实现的。在考虑服务变更安全的时候，则可以严格将部署和发布分成两个不同的阶段。部署是指将服务程序和配置拷贝到目标机器，并启动程序运行。发布则是服务正式接入线上流量，处理用户请求。<br />常见的发布方式主要有：
<a name="m44CM"></a>
## 滚动发布（Rolling Update）
逐个（或小批量）上线服务实例，每上线一个（或小批量）实例，则观察是否正常，正常后再上线下线一个（或小批量）实例，异常则进行回滚。这种方式着眼于逐步增加服务实例，而不是着眼于接入流量。如果系统总共N个实例，已上线M个实例，则新版本接入的流量为 M/N。
<a name="mOai2"></a>
## 灰度发布
灰度发布（Gray Release或Dark Launch），又名金丝雀发布（Canary Deployment）。<br />灰度发布与滚动发布相比，区别在于它更加考虑的是逐步将流量引入到新版本实例中，而不是发布了多少个新版本实例，下线了多少个旧版本实例。比如先将10%的流量引入新版本，然后增加到 20%，然后 50%， 80%，100% 等等，直到所有流量到引入到新版本中。
<a name="cAi5b"></a>
## 蓝绿发布（Blue Green Deployment）
蓝绿发布则是同时部署两套系统，当确保新版本系统已正常工作时，则将流量切到新版本系统（听上去有点不太靠谱，实际也有企业应用）。
<a name="WaVGD"></a>
## 不同服务发布场景的灰度发布方式
服务需要进行变更（发布）的几个主要原因：

1. 修复 bug：这种情况服务的接口和功能都不变
2. 重构：原先的功能使用新的方式实现，这种情况服务的接口和功能也不变
3. 功能升级：这种情况服务实例提供了新的功能，往往新增了新的接口
4. 扩容或缩容：这种情况服务实例的接口和功能也都不变，只是新增或减少了实例，导致流量在服务实例间重新分配
<a name="bLI9Z"></a>
## 通过gateway的权重路由实现灰度发布
spring cloud gateway 提供了一个Weight Route Predicate，它支持将流量按照不同比例路由到不同的目的地。这种方式特别适合进行A/B测试。比如同一个服务有 A、B 两个版本，分别注册为服务 app-serer-a，app-server-b，那么可以如下配置 gateway 的 route：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: app-server-a
        uri: lb://app-server-a
        predicates:
        - Path=/app-server/**
        - Weight=app-server, 50
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
      - id: app-server-a
        uri: lb://app-server-b
        predicates:
        - Path=/app-server/**
        - Weight=app-server, 50
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
```
如上所示，A、B 两个版本的流量各占 50%，可以通过配置中心（如apollo、spring cloud config等）动态修改生效上述的权重配置，从而调整 A、B 两个版本的流量占比<br />如果 A、B 两个版本的接口可以通过接口的路径进行区分，则可以如下配置 gateway 的 route：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: app-server-a
        uri: lb://app-server/v1
        predicates:
        - Path=/app-server/**
        - Weight=app-server, 50
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
      - id: app-server-a
        uri: lb://app-server/v2
        predicates:
        - Path=/app-server/**
        - Weight=app-server, 50
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
```
<a name="Diw6y"></a>
## `weightedServiceInstanceListSupplier`
通过 loadbalancer 的 `weightedServiceInstanceListSupplier` 实现。<br />通过对不同的服务实例配置不同的权重值，loadbalancer 在做负载均衡时，按照权重所占比例将流量负载均衡到不同的服务实例。假设版本 A 的所有实例的权重之和为 80，版本 B 的所有实例的权重之和为 20，那么版本 A 承载的流量站 80%，版本 B 承载的流量占 20%。通过动态调整两个版本的权重占比，就能调整两个版本的流量占比了。这种方式比较适合新旧版本的接口和功能都不变的变更场景。<br />这种方式 gateway 的 route 配置进行普通的配置即可：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: app-server
        uri: lb://app-server
        predicates:
        - Path=/app-server/**
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
```
每个服务实例配置自己的 weight 值：
```yaml
eureka:
  instance:
    metadata-map:
      weight: 80
```
然后在 gatway 上配置服务的负载均衡策略为 `weightedServiceInstanceListSupplier`。
```yaml
spring:
  cloud:
    loadbalancer:
      clients:
        app-server:
          configurations: weighted
```
<a name="HCzi0"></a>
## `hintBasedServiceInstanceListSupplier`
通过 loadbalancer 的 `hintBasedServiceInstanceListSupplier` 实现。<br />loadbalancer 的 `hintBasedServiceInstanceListSupplier` 会检查请求的X-SC-LB-Hint头，将请求发送给metadata-map.hint值与请求X-SC-LB-Hint头值相同的服务实例。这样如果服务实例的`eureka.instance.metadata-map.hint`配置为实例的版本号，就可以通过在请求的X-SC-LB-Hint头中设置为相应的版本号，从而控制流量导入不同的版本服务实例。这种方式尤其适合于版本升级了新的功能，提供了新的接口，可以通过在请求头中指定版本号让新版本的请求只由新版本的实例处理。<br />这种方式 gateway 的 route 配置进行普通的配置即可：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: app-server
        uri: lb://app-server
        predicates:
        - Path=/app-server/**
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
```
每个服务实例配置自己的版本值：
```yaml
eureka:
  instance:
    metadata-map:
      hint: v1
```
同时在 gateway 上添加以下配置和代码：
```java
public class AppLoadBalancerClientConfiguration {
    @Bean
    public ServiceInstanceListSupplier hintBasedServiceInstanceListSupplier(ConfigurableApplicationContext context) {
        return ServiceInstanceListSupplier.builder().withBlockingDiscoveryClient().withCaching().withHints()
        .build(context);
    }
}

@LoadBalancerClients(value = {
    @LoadBalancerClient(name = "app-server", configuration = AppLoadBalancerClientConfiguration.class)
})
public class AppClient1Application {

    public static void main(String[] args) {
        SpringApplication.run(AppClient1Application.class, args);
    }
}
```
<a name="alXyk"></a>
## `ServiceInstanceListSupplier`
自定义 loadbalancer 的 `ServiceInstanceListSupplier`，根据服务版本控制流量分配。<br />上面的几种方法虽然可以实现特定情况下的流量在不同版本的服务实例间分配，但是都不太完美。灰度发布应该将新增接口（功能）调用全部导入新版本的服务实例，而保持不变的接口（功能）流量则在新旧版本实例间按设定的比例分配，然后逐渐调整新旧比例值，直到所有流量都迁移到新版本中。同时服务的名字需要保持不变，用户也不用感知到版本的变化（比如，不需要在请求中添加特殊的头部，用来标识需要调用的服务版本，实际调用哪个版本应该由系统对用户隐藏）。<br />为实现上述目的，可以在 gateway 中添加如下 route 配置：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: app-server-v2
        uri: lb://app-server
        predicates:
        - Path=/app-server/v2/**
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
        - AddRequestHeader=X-SC-LB-Hint,v2
      - id: app-server-v1
        uri: lb://app-server
        predicates:
        - Path=/app-server/**
        filters:
        - RewritePath=/app-server(?<segment>/?.*), $\{segment}
```
上面的 route 配置目的为：新增接口（假设全部的新增接口的路径前缀统一为 v2）匹配 app-server-v2 路由，匹配后添加请求头X-SC-LB-Hint，值为v2，目的是让`loadbalancer`基于`hintBasedServiceInstanceListSupplier`的思路将请求都转发给 v2 版本的服务实例。保持不变的接口的路由配置则也保持不变，自定义的 loadbalancer 策略会根据配置比例将请求分发给不同版本的服务实例。<br />然后实现自定义的`ServiceInstanceListSupplier`。它实现的功能为：如果请求中存在X-SC-LB-Hint头，则从服务实例列表中查找所有的`metadata-map.hint`值与请求X-SC-LB-Hint头值相同的实例，然后返回找到的实例列表。如果请求中不存在在X-SC-LB-Hint头，则根据配置的服务不同版本的流量分配比例，计算当前请求应该分配给哪个版本的服务实例，然后从服务实例列表中查找所有的`metadata-map.hint`值等于选中版本号的服务实例，然后返回找到的实例列表。如果上面两种方式找到的服务实例列表均为空，则返回所有的服务实例。代码如下：
```java
import com.google.common.base.Splitter;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.cloud.client.ServiceInstance;
import org.springframework.cloud.client.loadbalancer.HintRequestContext;
import org.springframework.cloud.client.loadbalancer.LoadBalancerProperties;
import org.springframework.cloud.client.loadbalancer.Request;
import org.springframework.cloud.client.loadbalancer.RequestDataContext;
import org.springframework.cloud.client.loadbalancer.reactive.ReactiveLoadBalancer;
import org.springframework.cloud.loadbalancer.core.DelegatingServiceInstanceListSupplier;
import org.springframework.cloud.loadbalancer.core.ServiceInstanceListSupplier;
import org.springframework.http.HttpHeaders;
import org.springframework.util.StringUtils;
import reactor.core.publisher.Flux;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

public class WeightedVersionServiceInstanceListSupplier extends DelegatingServiceInstanceListSupplier {

    private static final Log LOG = LogFactory.getLog(WeightedVersionServiceInstanceListSupplier.class);

    private static ThreadLocal<VersionWeightInfo> versionWeightInfoThreadLocal = new ThreadLocal<>();

    private final ReactiveLoadBalancer.Factory<ServiceInstance> factory;

    private final Splitter splitter = Splitter.on(",").omitEmptyStrings().trimResults();

    public WeightedVersionServiceInstanceListSupplier(ServiceInstanceListSupplier delegate,
                                                      ReactiveLoadBalancer.Factory<ServiceInstance> factory) {
        super(delegate);
        this.factory = factory;
    }

    @Override
    public Flux<List<ServiceInstance>> get() {
        return delegate.get();
    }

    @Override
    public Flux<List<ServiceInstance>> get(Request request) {
        return delegate.get(request).map(instances -> filteredByHintAndWeightedVersion(instances, getHint(request.getContext())));
    }

    /**
     * 从 LoadBalancerProperties 中提取并解析 version-weight配置
     */
    private void parseWeightedVersionConfig() {

        VersionWeightInfo versionWeightInfo = versionWeightInfoThreadLocal.get();

        LoadBalancerProperties properties = factory.getProperties(getServiceId());
        String versionWeights = properties.getHint().getOrDefault("version-weight", "");
        if (versionWeights == null || versionWeights.isEmpty()) {
            // 没有配置version-weight
            versionWeightInfo = new VersionWeightInfo();
            versionWeightInfoThreadLocal.set(versionWeightInfo);
            return;
        }

        if (versionWeightInfo != null && versionWeights.equals(versionWeightInfo.getVersionWeights())) {
            // version-weight配置没有变化
            return;
        }

        // 第一次或version-weight发生了变化，计算随机数区间

        LinkedHashMap<String, Integer> weights = new LinkedHashMap<>();
        LinkedHashMap<String, Double> normalizedWeights = new LinkedHashMap<>();

        // version-weight: v1,80,v2,20
        List<String> segments = splitter.splitToList(versionWeights);
        int len = segments.size() & 0xFFFF_FFFE;
        for (int i = 0; i < len; i += 2) {
            try {
                String version = segments.get(i);
                Integer weight = Integer.valueOf(segments.get(i + 1));
                weights.put(version, weight);
            } catch (NumberFormatException e) {
                // version配置错误
                LOG.error("invalid version-weight configuration. configured version-weight value: " + versionWeights);
                versionWeightInfo = new VersionWeightInfo(versionWeights, false,
                        new LinkedHashMap<>(), new ArrayList<>());
                versionWeightInfoThreadLocal.set(versionWeightInfo);
                return;
            }
        }

        // 权重总和
        int weightsSum = 0;
        for (Integer weight : weights.values()) {
            weightsSum += weight;
        }

        // 权重占比
        LinkedHashMap<Integer, String> rangeIndexes = new LinkedHashMap<>();
        int index = 0;
        for (Map.Entry<String, Integer> entry : weights.entrySet()) {
            String version = entry.getKey();
            Integer weight = entry.getValue();
            Double nomalizedWeight = weight / (double) weightsSum;
            normalizedWeights.put(version, nomalizedWeight);
            rangeIndexes.put(index, version);
            index = index + 1;
        }

        // version随机数区间
        List<Double> ranges = new ArrayList<>();
        ranges.add(0.0);
        List<Double> values = new ArrayList<>(normalizedWeights.values());
        for (int i = 0; i < values.size(); i++) {
            Double currentWeight = values.get(i);
            Double previousRange = ranges.get(i);
            Double range = previousRange + currentWeight;
            ranges.add(range);
        }

        // 保证最后一个值大于或等于1
        ranges.set(values.size(), 1.1D);

        versionWeightInfo = new VersionWeightInfo(versionWeights, true, rangeIndexes, ranges);
        versionWeightInfoThreadLocal.set(versionWeightInfo);
    }

    private String getHint(Object requestContext) {
        if (requestContext == null) {
            return null;
        }
        String hint = null;
        if (requestContext instanceof RequestDataContext) {
            hint = getHintFromHeader((RequestDataContext) requestContext);
        }
        if (!StringUtils.hasText(hint) && requestContext instanceof HintRequestContext) {
            hint = ((HintRequestContext) requestContext).getHint();
        }
        return hint;
    }

    private String getHintFromHeader(RequestDataContext context) {
        if (context.getClientRequest() != null) {
            HttpHeaders headers = context.getClientRequest().getHeaders();
            if (headers != null) {
                LoadBalancerProperties properties = factory.getProperties(getServiceId());
                return headers.getFirst(properties.getHintHeaderName());
            }
        }
        return null;
    }

    /**
     * 从实例列表中根据hint值过滤实例
     * @param instances instances
     * @param hint hint
     * @return
     */
    private List<ServiceInstance> filteredByHint(List<ServiceInstance> instances, String hint) {

        List<ServiceInstance> filteredInstances = new ArrayList<>();
        if (StringUtils.hasText(hint)) {
            for (ServiceInstance serviceInstance : instances) {
                if (serviceInstance.getMetadata().getOrDefault("hint", "").equals(hint)) {
                    filteredInstances.add(serviceInstance);
                }
            }
        }

        return filteredInstances;
    }

    private List<ServiceInstance> filteredByWeightedVersion(List<ServiceInstance> instances) {
        List<ServiceInstance> filteredInstances = new ArrayList<>();

        VersionWeightInfo versionWeightInfo = versionWeightInfoThreadLocal.get();
        if (versionWeightInfo.getValidVersionWeight()) {
            String hint = getHintFromWeightedVersion(versionWeightInfo);
            if (hint != null && !hint.isEmpty()) {
                filteredInstances = filteredByHint(instances, hint);
            }
        }

        return filteredInstances;
    }

    /**
     * 计算版本号
     * @return 版本号
     */
    private String getHintFromWeightedVersion(VersionWeightInfo versionWeightInfo) {

        double r = versionWeightInfo.getRandom().nextDouble();
        // 循环所有区间，看当前随机数落入哪个区间
        String selectedVersion = "";
        LinkedHashMap<Integer, String> rangeIndexes = versionWeightInfo.getRangeIndexes();
        for (int i = 0; i < rangeIndexes.size() - 1; i++) {
            // 确定落入区间
            List<Double> ranges = versionWeightInfo.getRanges();
            if (r >= ranges.get(i) && r < ranges.get(i + 1)) {
                selectedVersion = rangeIndexes.get(i);
                break;
            }
        }

        return selectedVersion;
    }

    private List<ServiceInstance> filteredByHintAndWeightedVersion(List<ServiceInstance> instances, String hint) {

        // 从header中获取hint并过滤
        List<ServiceInstance> filteredInstances = filteredByHint(instances, hint);
        if (filteredInstances.size() > 0) {
            return filteredInstances;
        }

        // 根据版本权重计算hint并过滤
        filteredInstances = filteredByWeightedVersion(instances);
        if (filteredInstances.size() > 0) {
            return filteredInstances;
        }

        // 没有符合条件的实例，返回所有的
        return instances;
    }

    private static class VersionWeightInfo {
        private final String versionWeights;
        private final boolean validVersionWeight;
        private final LinkedHashMap<Integer, String> rangeIndexes;
        private final List<Double> ranges;
        private final Random random = new Random();

        public VersionWeightInfo() {
            this.versionWeights = "";
            this.validVersionWeight = false;
            this.rangeIndexes = new LinkedHashMap<>();
            this.ranges = new ArrayList<>();
        }

        public VersionWeightInfo(String versionWeights,
                             boolean validVersionWeight,
                             LinkedHashMap<Integer, String> rangeIndexes,
                             List<Double> ranges) {
            this.versionWeights = versionWeights;
            this.validVersionWeight = validVersionWeight;
            this.rangeIndexes = rangeIndexes;
            this.ranges = ranges;
        }

        public String getVersionWeights() {
            return this.versionWeights;
        }

        public boolean getValidVersionWeight() {
            return this.validVersionWeight;
        }

        public LinkedHashMap<Integer, String> getRangeIndexes() {
            return this.rangeIndexes;
        }

        public List<Double> getRanges() {
            return this.ranges;
        }

        public Random getRandom() {
            return this.random;
        }
    }
}
```
然后将自定义`WeightedVersionServiceInstanceListSupplier`注册到 `loadbalancer` 中。
```java
public class AppLoadBalancerClientConfiguration {

    @Bean
    public ServiceInstanceListSupplier weightedVersionServiceInstanceListSupplier(ConfigurableApplicationContext context) {
        DelegateCreator creator = (context, delegate) -> {
            LoadBalancerClientFactory loadBalancerClientFactory = context.getBean(LoadBalancerClientFactory.class);
            return new WeightedVersionServiceInstanceListSupplier(delegate, loadBalancerClientFactory);
        };

        return ServiceInstanceListSupplier.builder().withBlockingDiscoveryClient().with(creator).withCaching()
        .build(context);
    }

}


@LoadBalancerClients(defaultConfiguration = AppLoadBalancerClientConfiguration.class)
@SpringBootApplication
public class AppClient1Application {

    public static void main(String[] args) {
        SpringApplication.run(AppClient1Application.class, args);
    }

}
```
在 gateway 上配置服务不同版本的流量分配比例：
```yaml
spring:
  cloud:
    loadbalancer:
      clients:
        app-server:
          hint:
            version-weight: v1,80,v2,20
```
最后，服务实例的`metadata-map.hint`配置相应的版本号：
```yaml
eureka:
  instance:
    metadata-map:
      hint: v2
```
