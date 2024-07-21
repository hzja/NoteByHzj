一起来看看PromQL查询解析。虽然PromQL有操作符、函数、选择器等，但无需被这些细节所困扰。来看看查询的总体性质：PromQL查询是如何结构化和类型化的？随着时间的推移它们是如何评估的？<br />这里将假设大家对PromQL查询和Prometheus数据模型有大概粗略的了解。
<a name="VpT2l"></a>
## 1、PromQL是一种嵌套的函数式语言
不像SQL或其他一些更倾向于命令式的查询语言(`SELECT * FROM...`)，PromQL是一种嵌套的函数式语言。这意味着寻找的数据描述为一组嵌套表达式，每个表达式都计算出一个中间值(没有副作用)。每个中间值都用作它周围表达式的参数或操作数，而查询的最外层表达式表示您在表、图或类似用例中看到的最终返回值。<br />查询示例如下所示：
```
# Root of the query, final result, approximates a quantile.
histogram_quantile(
  # 1st argument to histogram_quantile(), the target quantile.
  0.9,
  # 2nd argument to histogram_quantile(), an aggregated histogram.
  sum by(le, method, path) (
    # Argument to sum(), the per-second increase of a histogram over 5m.
    rate(
      # Argument to rate(), the raw histogram series over the last 5m.
      demo_api_request_duration_seconds_bucket{job="demo"}[5m]
    )
  )
)
```
PromQL表达式不仅是整个查询，而且是查询的任何嵌套部分(如上面的`rate(…)`部分)，可以单独作为查询运行。在上面的例子中，每个注释行代表一个表达式。<br />当在PromLabs的PromLens查询可视化工具中分析相同的查询时，包含子表达式的嵌套表达式结构会变得特别清晰：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680221980557-fa9176c6-a9a4-44b7-8b86-1a3eb6ba5103.png#averageHue=%2385be8a&clientId=u39e127b6-5beb-4&from=paste&id=u12891aab&originHeight=361&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udfd2d22c-f7b1-4abe-8cf7-ea561819cda&title=)<br />在PromLens中，可以单击每个树节点来分别评估其子表达式，以了解在嵌套表达式树的每个部分中处理的数据。
<a name="aDGhz"></a>
## 2、输入的表达式，可能不像你想的那样
Prometheus中有两个“类型”的概念：

- **由抓取目标所报告的度量类型：计数器、仪表、直方图、摘要或无类型。**
- **PromQL表达式的类型：字符串、标量、瞬时向量或范围向量。**

PromQL完全忽略度量类型，只关心表达式类型：每个表达式都有一个类型，每个函数、操作符或其他类型的操作都要求其参数是某种表达式类型。例如，rate()函数要求其参数是一个范围向量，但rate()本身的计算结果是一个瞬时向量。因此，rate()的结果只能用于期望瞬时向量的地方。<br />PromQL中可能的表达式类型有：

- **字符串：类似 "I am a string!". 这些只作为某些函数(如**`**label_join()**`**)的参数出现，在PromQL中并不常用。**
- **标量：没有标注尺寸的单个数值，如 1.234。您将看到这些函数的数值参数，如**`**histogram_quantile(0.9，…)**`**或**`**topk(3，…)**`**，以及算术运算。**
- **瞬时向量：一组带标签的时间序列，每个序列有一个样本，所有样本都有相同的时间戳。瞬时向量可以直接来自TSDB时间序列选择器，如**`**node_cpu_seconds_total**`**，也可以来自返回它们的任何函数或其他转换。**
```
node_cpu_seconds_total{cpu="0", mode="idle"}   → 19165078.75 @ timestamp_1
node_cpu_seconds_total{cpu="0", mode="system"} →   381598.72 @ timestamp_1
node_cpu_seconds_total{cpu="0", mode="user"}   → 23211630.97 @ timestamp_1
```

- **范围向量：一组带标签的时间序列，每个序列都有一段时间的样本范围。在PromQL中只有两种方法产生范围向量:在查询中使用文字范围向量选择器(如node_cpu_seconds_total[5m])，或者使用子查询(如[5m:10s])。当想要聚合指定时间窗口内的一系列行为时，范围向量非常有用，就像使用**`**rate(node_cpu_seconds_total[5m])**`**计算**`**node_cpu_seconds_total**`**在过去5分钟内的平均每秒增长率一样。**
```
node_cpu_seconds_total{cpu="0", mode="idle"}   → 19165078.75 @ timestamp_1,  19165136.3 @ timestamp_2, 19165167.72 @ timestamp_3
node_cpu_seconds_total{cpu="0", mode="system"} → 381598.72   @ timestamp_1,   381599.98 @ timestamp_2,   381600.58 @ timestamp_3
node_cpu_seconds_total{cpu="0", mode="user"}   → 23211630.97 @ timestamp_1, 23211711.34 @ timestamp_2, 23211748.64 @ timestamp_3
```
但是指标类型呢？如果已经使用过PromQL，可能知道某些函数只适用于特定类型的指标！例如，`histogram_quantile()`函数只适用于直方图指标，`rate()`只适用于计数器指标，而`deriv()`只适用于仪表。但是PromQL实际上并不检查您是否传入了正确类型的指标——这些函数通常会顺利地运行，并为错误类型的输入指标返回一些无意义的东西，并且由用户决定是否传入符合某些假设的时间序列(比如在直方图的情况下有一个敏感的le标签，或者在计数器的情况下单调增加)。然而，将来很可能会看到像PromLens这样的用户界面试图警告你，如果把一个不兼容的指标类型传递给一个函数。
<a name="DrwWJ"></a>
## 3、时间是怎么进来的？范围和即时查询！
可能已经注意到，PromQL查询中对时间的唯一引用是相对引用(例如[5m]，向后看5分钟)。那么，如何指定绝对图形时间范围，或者在表中显示查询结果的时间戳呢？在PromQL中，这样的时间参数与表达式分开发送给Prometheus查询API，确切的时间参数取决于您发送的查询类型。Prometheus有两种类型的PromQL查询：即时查询和范围查询。
<a name="aWOzY"></a>
### 3.1即时查询
即时查询用于类似表格的视图，在这些视图中，您希望显示PromQL查询在单个时间点的结果。<br />即时查询的参数包括：

- **PromQL表达式。**
- **评估时间戳**

表达式在计算时间戳时进行计算，查询中的任何数据选择器都可以选择从该时间戳回溯到过去的数据(foo[1h]选择foo系列的最后一个小时的数据)，但不能选择未来的数据(foo[-1h]是无效的PromQL)。访问过去数据的窗口通常有助于计算一段时间内的比率或平均值等总量。<br />即时查询可以返回任何有效的PromQL表达式类型(字符串、标量、即时和范围瞬时和范围向量)。<br />例如即时查询：<br />看一个即时查询的例子，看看它是如何工作的。想象一下使用表达式http_requests_total查询给定的时间戳数据。http_requests_total是一个瞬时向量选择器，它使用指标名称http_requests_total为任何时间序列选择最新的样本。更具体地说，相对于评估时间戳，“latest”意味着“最多5分钟以前的并且不是陈旧的”。因此，该选择器将只为在评估时间戳之前最多5分钟有一个样本的序列产生一个结果，并且评估时间戳之前的最后一个样本不是一个陈旧的标记(一种将序列标记为在Prometheus TSDB中的某个时间终止的显式方法)。<br />如果在有最近样本的时间戳上运行此查询，结果将包含两个序列，每个序列只有一个样本：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680221980604-96eebfdf-e0eb-4409-b842-a15141b3adcf.png#averageHue=%23fdfdfd&clientId=u39e127b6-5beb-4&from=paste&id=uaddc9e8e&originHeight=708&originWidth=859&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe41ac73-98c3-4f7e-b3aa-f2646a18478&title=)<br />注意，每个返回样本的输出时间戳不再是原始样本时间戳，而是被设置为评估时间戳。<br />想象一下，在一个时间戳上执行同样的查询，在该时间戳之前有>5m的数据间隔:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680221980646-0488997e-4414-4276-bdec-ddd849336ada.png#averageHue=%23fdfdfd&clientId=u39e127b6-5beb-4&from=paste&id=u49541a03&originHeight=706&originWidth=856&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucbe865c7-944d-47bf-9ca9-e2bad2d47b9&title=)<br />在这种情况下，查询将返回一个空结果，因为所有匹配的样本都太旧而无法包含。
<a name="cbpWR"></a>
### 3.2范围查询
范围查询主要用于图形，其中您希望显示给定时间范围内的PromQL表达式。范围查询的工作方式完全类似于许多完全独立的即时查询，这些查询在给定时间范围内的后续时间步骤中进行评估。当然，这是经过高度优化的，在这种情况下，Prometheus实际上并没有运行很多独立的即时查询。<br />范围查询包含以下参数：

- PromQL表达式。
- 开始时间。
- 结束时间。
- 解析步骤。

在开始时间和结束时间之间的每个解析步骤对表达式求值后，单独求值的时间片被拼接到单个范围向量中。范围查询允许传入瞬时向量类型或标量类型的表达式，但总是返回一个范围向量(标量或瞬时向量在一段时间内计算的结果)。<br />范围查询示例：<br />如果将上面的示例表达式评估为一个范围查询，它将如下所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680221980661-2fa71c33-aadd-4c10-90d5-c620b884bd48.png#averageHue=%23f5f5f5&clientId=u39e127b6-5beb-4&from=paste&id=ud62b193c&originHeight=709&originWidth=856&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd9d0f95-0edf-4f8f-aa3e-2fd897e9911&title=)<br />请注意，每个评估步骤的行为完全类似于独立的即时查询，并且每个独立的即时查询对查询的整体范围没有概念。这种情况下的最终结果将是一个范围向量，它包含一个时间范围内两个选定序列的样本，但也包含某些时间步长的序列数据中的间隙。
<a name="CgcWP"></a>
## 最后
希望大家对PromQL查询的整体结构、正在进行的类型检查(或缺少的类型检查)以及查询的解析评估有了更好的了解。
