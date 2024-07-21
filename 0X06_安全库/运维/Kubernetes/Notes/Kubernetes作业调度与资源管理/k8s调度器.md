KubernetesKubernetes调度器
> 导语 | kubernetes调度器，通过watch机制来发现集群中新创建且未调度的pod，通过过滤node列表，打分策略，以及各个时机的插件调用机制，选择合适的node与之绑定。

<a name="Lydaw"></a>
## 一、调度队列
同一时刻会有多个pod等待调度，会把等待调度的pod放到activeQ中（PriorityQueue），然后周期性（1s）的进行调度，对于调度超时（ DefaultPodMaxInUnschedulablePodsDuration 5m）会放入队列中，再次重新调度。
<a name="DO5ee"></a>
## 二、单次调度
用下图来说明单个调度的流转逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662207314937-e895f289-f348-40ce-a8da-e2841e15fc7e.png#clientId=u458a7eba-058d-4&from=paste&id=u23e1fb89&originHeight=833&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u31d648f3-75cb-4aa5-93fa-dc688604bd7&title=)<br />**注意：一个集群中可以有多个调度器，所以首先需要根据pod中的spec参数获取调度器名称**<br />跳过`pod：skipPodSchedule`， 过滤调不需要调度的pod，比如正在删除中的pod，上个调度周期正在处理中的pod<br />筛选`pod：SchedulePod`，计算并预选出适合的node<br />如果筛选失败，则调用`RunPostFilterPlugins`; 如果筛选成功，则调用插件：`RunXXXPlugins`，开始调用配置的插件列表，从Reserve插件到`MultiPoint`依次按照埋点调用。<br />对于大规模集群，单此调度要遍历所有的node么？这是一个值得思考的问题。默认调度器给出的答案是根据集群规模自适应调度数量。

- 对于小规模集群，node数小于100， 遍历所有node。
- 对于大规模集群，node数大于100，且配置的百分比小于100%时：按照node数量的一定百分比遍历，区间范围是[5，100]。
- 计算公式是

prePercent :=50-numAllNodes/125<br />percent :=max(5，prePercent)
<a name="cmt9e"></a>
## 三、调度过程
调度过程分为3个步骤：过滤，打分，筛选，代码步骤如下：<br />省略非必要代码
```go
// node快照
if err := sched.Cache.UpdateSnapshot(sched.nodeInfoSnapshot); err != nil {
  return result, err
}

// 过滤
feasibleNodes, diagnosis, err := sched.findNodesThatFitPod(ctx, fwk, state, pod)
if err != nil {
  return result, err
}

// 打分
priorityList, err := prioritizeNodes(ctx, sched.Extenders, fwk, state, pod, feasibleNodes)
if err != nil {
  return result, err
}

// 随机筛选
host, err := selectHost(priorityList)
```
<a name="hTX1C"></a>
### **（一）过滤**
可用的node列表：

- 插件过滤：`RunPreFilterPlugins`如果插件执行失败，那么返回所有node可用，如果插件返回不可调度，则返回失败，终止本次调度。
- 获取`node allNodes, err := sched.nodeInfoSnapshot.NodeInfos().列表()`
- 抢占式pod的status字段中`NominatedNodeName`设置后，会优先抢占同名的node。
- `allNode`和`preFilter`返回的node求交集。
<a name="xFuhd"></a>
### **（二）打分**
根据优先级选择合适的node列表：`prioritizeNodes`

- 如果没有开启打分插件，返回所有node list。
- 打分插件逐次调用`RunPreScorePlugins`-->`RunScorePlugins`
<a name="cHGM6"></a>
### **（三）筛选**
相同优先级列表下，获取score最大值的node，如果存在多个相同分数，则随机一个。
<a name="WdSy3"></a>
## 四、插件机制
插件分为了调度和绑定两大类，划分成了多个时机调用，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662207314862-5c808355-6cef-4cfa-94d4-c444599a8081.png#clientId=u458a7eba-058d-4&from=paste&id=u9820e402&originHeight=541&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c8dd1b0-1962-463d-972a-d80b6d17e17&title=)
<a name="dqtyK"></a>
### **（一）插件类型**
对于pod的调度过程，划分了多个点，每个点调用对应的插件列表，目前支持如下多种类型插件：
```go
// QueueSort is a list of plugins that should be invoked when sorting pods in the scheduling queue.
QueueSort PluginSet `json:"queueSort,omitempty"`

// PreFilter is a list of plugins that should be invoked at "PreFilter" extension point of the scheduling framework.
PreFilter PluginSet `json:"preFilter,omitempty"`

// Filter is a list of plugins that should be invoked when filtering out nodes that cannot run the Pod.
Filter PluginSet `json:"filter,omitempty"`

// PostFilter is a list of plugins that are invoked after filtering phase, but only when no feasible nodes were found for the pod.
PostFilter PluginSet `json:"postFilter,omitempty"`

// PreScore is a list of plugins that are invoked before scoring.
PreScore PluginSet `json:"preScore,omitempty"`

// Score is a list of plugins that should be invoked when ranking nodes that have passed the filtering phase.
Score PluginSet `json:"score,omitempty"`

// Reserve is a list of plugins invoked when reserving/unreserving resources
// after a node is assigned to run the pod.
Reserve PluginSet `json:"reserve,omitempty"`

// Permit is a list of plugins that control binding of a Pod. These plugins can prevent or delay binding of a Pod.
Permit PluginSet `json:"permit,omitempty"`

// PreBind is a list of plugins that should be invoked before a pod is bound.
PreBind PluginSet `json:"preBind,omitempty"`

// Bind is a list of plugins that should be invoked at "Bind" extension point of the scheduling framework.
// The scheduler call these plugins in order. Scheduler skips the rest of these plugins as soon as one returns success.
Bind PluginSet `json:"bind,omitempty"`

// PostBind is a list of plugins that should be invoked after a pod is successfully bound.
PostBind PluginSet `json:"postBind,omitempty"`

// MultiPoint is a simplified config section to enable plugins for all valid extension points.
MultiPoint PluginSet `json:"multiPoint,omitempty"`
```
<a name="kxUjI"></a>
### **（二）插件列表**
默认调度器，实现了多种插件不用特性的插件，目前支持的列表如下，下面举几个例子说明。
```go
"PrioritySort" : 
"DefaultBinder"
"DefaultPreemption"
"ImageLocality"
"InterPodAffinity"
"NodeAffinity"
"NodeName"
"NodePorts"
"NodeResourcesBalancedAllocation"
"NodeResourcesFit"
"NodeUnschedulable"
"NodeVolumeLimits"
"AzureDiskLimits"
"CinderLimits"
"EBSLimits"
"GCEPDLimits"
"PodTopologySpread"
"SelectorSpread"
"ServiceAffinity"
"TaintToleration"
"VolumeBinding"
"VolumeRestrictions"
"VolumeZone"
```
对于打分插件，必须实现如下接口，且每个插件打分范围是[0, 100]
```go
type ScorePlugin interface {
    Plugin
    // Score is called on each filtered node. It must return success and an integer
    // indicating the rank of the node. All scoring plugins must return success or
    // the pod will be rejected.
    Score(ctx context.Context, state *CycleState, p *v1.Pod, nodeName string) (int64, *Status)

    // ScoreExtensions returns a ScoreExtensions interface if it implements one, or nil if does not.
    ScoreExtensions() ScoreExtensions
}
```
<a name="IOOhQ"></a>
### **（三）插件特性**
<a name="nREde"></a>
#### **图像定位**
`ImageLocality`：本地镜像打分插件，计算分数规则如下：<br />sumScore :=(拥有镜像的node数/node总数)*镜像大小<br />得分 := (总和分数容器数- 23mb)/(1000mb 3-23mb)<br />**注意**：这里不是指单个containner，而是一个pod中的所有container的打分之和。为什么范围是23mb到1000mb？可以想一想。
<a name="lqUbx"></a>
#### **节点关联**
`NodeAffinity`：node亲和性和反亲和性，提供了**两种策略配置**。

- 对于必选策略`RequiredDuringSchedulingIgnoredDuringExecution`，如果匹配未成功，则在`PreFilter`阶段返回失败，终止调度。
- 对于首先策略`PreferredDuringSchedulingIgnoredDuringExecution`，如果匹配未成功，则尝试其他node也失败，调度器仍然会调度改pod。亲和性，反亲和性是用来影响打分数值（正负分）weight

逻辑代码如下：
```go
// 亲和性，反亲和性判定判定
if hasPreferredAffinityConstraints || hasPreferredAntiAffinityConstraints {
    for _, existingPod := range podsToProcess {
      pl.processExistingPod(state, existingPod, nodeInfo, pod, topoScore)
    }
    
  topoScores[atomic.AddInt32(&index, 1)] = topoScore
}
```
比如业务逻辑中的配置如下图，期望是一个node上只调度一个这种pod，但是配置了首选策略。所以当node数小于pod数时，是会出现一个node上有多个此类pod，会有一定的影响。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662207314923-677aedfd-6496-442d-b1af-6dbacacea0dd.png#clientId=u458a7eba-058d-4&from=paste&id=uad460751&originHeight=541&originWidth=796&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10dd5e89-ec60-41b5-8d35-5bd7aaa81f1&title=)
<a name="HcMjw"></a>
#### **污点耐受**
`TaintToleration`：污点插件，提供了过滤，预打分，打分，打分标准化（平行扩展到0到100）接口。<br />污点标记提供了3种类型
```go
// 尽可能不调度
TaintEffectPreferNoSchedule TaintEffect = "PreferNoSchedule"    

// 一定不调度
TaintEffectNoSchedule TaintEffect = "NoSchedule"

// 一定不调度且驱逐
TaintEffectNoExecute TaintEffect = "NoExecute"
```
比如业务中打了污点，那么一般pod是不会调度到此pod上的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662207314859-db12396b-6a54-441c-a6d4-92b44be87b29.png#clientId=u458a7eba-058d-4&from=paste&id=u45b4c523&originHeight=694&originWidth=1068&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ca499df-744a-44ab-9e6d-196863f7e88&title=)
<a name="b2aNL"></a>
## 五、调度器配置
一般情况下，scheduler会起多副本进行容灾。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662207314947-7a598359-c407-447c-a57b-f7957fff2998.png#clientId=u458a7eba-058d-4&from=paste&id=ucebf3dd3&originHeight=90&originWidth=749&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub4704c2f-c4c2-4466-bc55-705f55ca9e0&title=)
```json
{
  "name": "BalancedResourceAllocation",
  "weight": 1
},
{
  "name": "EvenPodsSpreadPriority",
  "weight": 1
},
{
  "name": "InterPodAffinityPriority",
  "weight": 1
},
{
  "name": "LeastRequestedPriority",
  "weight": 1
},
{
  "name": "NodeAffinityPriority",
  "weight": 1
},
{
  "name": "NodePreferAvoidPodsPriority",
  "weight": 10000
},
{
  "name": "SelectorSpreadPriority",
  "weight": 1
},
{
  "name": "TaintTolerationPriority",
  "weight": 1
}
```
<a name="rxOJ9"></a>
## 六、如何自定义pod调度
目前有2种常用方法：
<a name="odObo"></a>
### （一）扩展模式
实现t`ype Extender struct`接口，并且在策略文件scheduler-policy-config中配置扩展访问方式
```json
"extenders": [{
    "urlPrefix": "http://xxx/prefix",
    "filterVerb": "filter",
    "weight": 1,
    "bindVerb": "bind",
    "enableHttps": false
}]
```
<a name="w4GyX"></a>
### （二）多调度器
在需要自定义调度的pod中，指定pod的`spec.schedulerName` 为自定义的调度器名称。实现自定义调度器。部署自定义的调度器deployment。<br />在新版本1.19之后建议扩展自定义调度框架，如下例：
```go
import (
    scheduler "k8s.io/kubernetes/cmd/kube-scheduler/app"
)

func main() {
    command := scheduler.NewSchedulerCommand(
            scheduler.WithPlugin("my-plugin", MyPlugin))
    if err := command.Execute(); err != nil {
        fmt.Fprintf(os.Stderr, "%v\n", err)
    }
}
```
<a name="DDD9q"></a>
## 七、总结
在深入schedule源码之后，对于调度器有了剖丝抽茧的理解，了解背后的设计初衷。对于高性能，提供了自适应集群规模的调度策略。对于可靠性，kube-scheduler提供了多副本选主机制，由master提供调度功能。对于扩展性，它提供了丰富的扩展接口和时机用，且提供了灵活而实用插件策略配置。
