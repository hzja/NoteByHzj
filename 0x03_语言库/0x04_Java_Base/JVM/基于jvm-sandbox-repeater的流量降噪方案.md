<a name="DytWh"></a>
## 一、背景
jvm-sandbox-repeater 是阿里开源的一款可基于 jvm-sandbox (阿里另一开源项目)可对应用目标 jvm 进行动态增强同时对目标服务的指定流量进行录制及回放的工具，使用过程中遇到如下问题：

- 目前流量的录制仅能通过采样率来降低录制的频次，比如采样率为 10% ，那么一天之中将有 10% 的时间在录制流量，但这 10% 的时间内录制的流量也可能存在很多的重复流量或者很多相似的流量，从业务场景的角度去分析，这些重复或者相似的流量很大程度上业务场景是一致的，而对于功能测试或者接口测试而言，需要测试的是场景，重复的场景流量重复录制、重复回放是一种极大的浪费，而且对流量用例的分类及回放结果的分析及其不友好。所以我们需要对流量的录入进行降噪处理。
- 目前回放结果的对比是全量对比，即对录制的主调用结果和回放的主调用的结果进行全面(返回对象的所有的 key 和 value )对比，通常情况下，全量对比是非常有必要的，但存在一些场景：比如流量回放不支持 mock 的 native 方法(像时间戳、时间、随机数等等)，比如业务改造新增的一些返回字段或者较少的一些返回字段，这些场景会对我们的结果对比产生干扰，所以需要在结果对比时对特定的纬度进行降噪处理。
<a name="eWXLP"></a>
## 二、需求明细
基于上述背景，打算对流量回放进行如下改造：

- 结果对比降噪：新增配置，支持降噪纬度灵活配置；结果对比时根据降噪纬度进行对比去噪音处理
- 流量录制降噪并自动转用例
<a name="Gvk8u"></a>
## 三、设计方案
<a name="pytcq"></a>
### 3.1 结果对比降噪
配置对象 `RepeaterConfig` 新增属性 `List ignoreFiled` 支持用户配置降噪字段。
```java
/**
 * 需要忽略对比的列信息
 */
private List<String> ignoreFiled = Lists.newArrayList();
```
对比器接口 `com.alibaba.jvm.sandbox.repeater.aide.compare.Comparable#compare` 新增入参 `List ignoreFiled`，并对参数完成传递
```java
/**
 * compare to object
 *
 * @param left  left object to be compare
 * @param right right object to be compare
 * @param ignoreFiled compare ignore some filed
 * @return compare result
 */
CompareResult compare(Object left, Object right,List<String> ignoreFiled);
```
对比器 Map 对比器实现 `com.alibaba.jvm.sandbox.repeater.aide.compare.comparator.MapComparator` 进行改造，根据降噪字段进行忽略对比。
```java
if (ignoreFiled != null && key instanceof String && ignoreFiled.contains(key)) {
    continue;
}
```
结果对比，再保存回放结果处 `com.alibaba.repeater.console.service.impl.ReplayServiceImpl#saveRepeat` 进行改造。
```java
List<String> ignoreFiledLists = null;
if (moduleConfigBORepeaterResult.getData() != null) {
    ignoreFiledLists = moduleConfigBORepeaterResult.getData().getConfigModel().getIgnoreFiled();
}
CompareResult result = comparable.compare(actual, expect, ignoreFiledLists);
```
其他关联影响：由于对比器入参新增，导致所有调用对比器处都需要进行入参添加，不使用降噪配置的地方传 null 即可,比如
<a name="Sr372"></a>
### 3.2 流量录入降噪
<a name="nie8P"></a>
#### 3.2.1 调研
jvm-sandbox-repeater 根据协议、 dubbo 接口全路径、 http 接口后缀、入参长度、出参长度、入参相似度、出参相似度做一套流量去重、分类的方案，通过以下七种参数的组合，可以对请求进行细化的分类和去重，从而更加准确地识别和过滤出重复或异常请求

- 可以使用请求的出参相似度来进行分类和去重。出参相似度指请求出参之间的相似程度，可以使用一些相似度算法（如余弦相似度、编辑距离等）来计算。例如，可以设置一个阈值，将请求出参相似度在该阈值以上的请求归为一类。
- 可以使用请求的入参相似度来进行分类和去重。入参相似度指请求入参之间的相似程度，可以使用一些相似度算法（如余弦相似度、编辑距离等）来计算。例如，可以设置一个阈值，将请求入参相似度在该阈值以上的请求归为一类。
- 可以使用请求的出参长度来进行分类和去重。例如，可以设置一个阈值，将请求出参长度在该阈值以下的请求归为一类。
- 可以使用请求的入参长度来进行分类和去重。例如，可以设置一个阈值，将请求入参长度在该阈值以下的请求归为一类。
- 对于 HTTP 接口，可以使用其 URL 的后缀来进行分类和去重。例如，对于 http://example.com/user/getUserById ，其后缀为 getUserById 。
- 对于 Dubbo 接口，可以使用其全路径来进行分类和去重。Dubbo 接口的全路径包括接口的接口名和版本号。例如，对于 com.example.service.UserService 接口的版本为 1.0.0 的方法 getUserById ，其全路径为 com.example.service.UserService:1.0.0:getUserById 。
- 协议
- 入参长度
- 出参长度
- 入参相似度
- 出参相似度

相似度算法

- 优点：
- 缺点：
- 考虑词语重要性：余弦相似度可以使用词袋模型或 TF-IDF 等方法表示字符串，从而考虑词语的重要性和权重。
- 忽略字符串长度：余弦相似度计算的是向量之间的夹角，而不依赖于字符串的长度，因此可以比较不同长度的字符串。
- 考虑字符串的分布特征：余弦相似度考虑了字符串中词语的分布特征，可以捕捉到更多的语义和上下文信息。
- 无法处理字符顺序：余弦相似度将字符串表示为向量，忽略了字符的顺序信息。因此，在某些应用场景中，顺序关系可能很重要，而余弦相似度无法准确捕捉这种关系。
- 对于稀疏向量效果差：当字符串的表示向量是稀疏向量时，余弦相似度可能会受到干扰，因为它只考虑向量的夹角而不考虑向量的长度。
- 优点：
- 缺点：
- 直观简单：编辑距离易于理解和实现，通过计算字符串之间的插入、删除和替换操作次数来度量相似度。
- 适用性广泛：编辑距离可以应用于不同类型的字符串，包括文本、 DNA 序列等。
- 能够考虑字符串结构差异：编辑距离不仅考虑字符内容的差异，还考虑字符位置和结构的差异。
- 计算复杂度高：对于较长的字符串，计算编辑距离的复杂度较高，特别是当字符串长度差异较大时。
- 忽略语义关系：编辑距离只关注字符串之间的字符操作次数，而忽略了语义关系和上下文信息。
- 对于字符串相似度的度量有限：编辑距离度量的是字符串的差异程度，而不是相似程度。两个相似的字符串可能具有较大的编辑距离，而两个不相似的字符串可能具有较小的编辑距离。
- 编辑距离（ Edit Distance ）：编辑距离是指将一个字符串转换成另一个字符串所需的最少操作次数，可以通过插入、删除和替换字符来实现。编辑距离越小，表示两个字符串越相似。
- 余弦相似度（ Cosine Similarity ）：余弦相似度是通过计算两个向量之间的夹角的余弦值来度量相似度。在字符串相似度计算中，可以将字符串转换为向量表示，例如使用词袋模型或 TF-IDF 向量表示。然后，计算两个向量之间的余弦相似度，值越接近 1 表示越相似。
- 综上所述，编辑距离适用于简单的字符串比较和相似度度量，而余弦相似度适用于考虑语义和上下文信息的字符串相似度度量。
<a name="uFg6O"></a>
#### 3.2.2 最终方案
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695175402251-f6ed296d-3394-4817-aeba-57e948fa75ad.png#averageHue=%23b1d688&clientId=u312a9ae1-ad41-4&from=paste&id=u1add138d&originHeight=466&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74d7689b-0a18-4f44-8ad6-da382a2c115&title=)<br />关键信息图解：

- 降噪处理：
- 相似度判断：
- 接口路径预处理，为了降低因为域名等信息不同导致的干扰需要对接口进行预处理。拿到最原始的接口路径。
- 子调用判断：子调用不一致一定不属于同一业务场景。
- 入参、出参的编辑距离计算，根据编辑距离判断是否相似。
- 创建降噪定时任务：根据消费速度及发送数量适当调整间隔频次。
- 批量获取流量详细信息
- 发送消息队列：根据 akka 系统的 Actor 来发送和接受信息
- 消费消息队列：

下面是一些基础代码：
```java
public class AiDenoiseOfferJob extends QuartzJobBean {

    @Override
    protected void executeInternal(JobExecutionContext context) throws JobExecutionException {


    }
}
```

- akka 系统配置
- 在 executeInternal 根据 Actor 并发数量生产消息
- Actor 接受并消费消息
- 降噪核心逻辑
- Actor 发送消息、消费消息
```java
public class AkkaConfig {

    @Autowired
    private ApplicationContext applicationContext;

    @Autowired
    private ApolloConfig apolloConfig;

    @Bean
    public ActorSystem actorSystem() {
        return ActorSystem.create("my-actor-system");
    }

    @Bean
    public ActorRef myActor(ActorSystem actorSystem) {
        return actorSystem.actorOf(new RoundRobinPool(apolloConfig.getActorNum()).props(Props.create(MyActor.class,applicationContext)), "router");
    }
}
```

- 新建定时任务
<a name="hK1Zd"></a>
## 四、未来规划
现阶段针对结果对比提供了手动降噪的能力，并将录制的流量进行了结果降噪对比同时将其转为用例，对于测试同学很难分辨这些用例覆盖了哪些代码。
