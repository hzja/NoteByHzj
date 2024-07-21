<a name="FK18q"></a>
## 1、介绍
使用 Prometheus Go client library 在 Go 代码中埋点，将指标数据上报到 Prometheus server，接下来使用 PromQL 查询指标数据。<br />PromQL 可以在 Prometheus 提供的 http://localhost:9090/graph 中执行，本文介绍怎么使用 PromQL 查询指标数据。
<a name="mq0dl"></a>
## 2、PromQL
PromQL 除了直接使用指标名称查询时序数据之外，还可以联合使用 Label 和函数等。<br />比如使用最多的 Counter 指标类型，它会统计自启动 Prometheus 以来的计数总和，但是并不关心总数。只想知道 Counter 随着时间的推移，计数增加的速度。<br />比如可以直接使用自定义指标名称 `http_request_count_total` 查询所有指标名称为 `http_request_count_total` 的指标数据（HTTP 请求数）。<br />实际上，并不是要查询 HTTP 请求数的总和，而是想查询单位时间内，请求数的增长速度。此时，可以使用函数 rate 完成。<br />使用 `rate(http_request_count_total[5m])` 可以查询 HTTP 每秒请求数，其中 [5m] 是指过去 5 分钟每秒请求数的平均值。<br />比如 HTTP 请求数指标中包含 PATH 标签，如果想要统计所有 HTTP 请求数指标的每秒请求数的总和，可以使用 sum 聚合运算符 和 without 子句，`sum without(path)(rate(http_request_count_total[5m]))`。<br />比如想要查询指定 PATH 的 HTTP 每秒请求数，可以使用 `rate(http_request_count_total{path="/ping"}[5m])` 查询，通过 {path="/ping"} 过滤指标数据。<br />实际上 `http_request_count_total{path="/ping"}` 中 {} 称为选择器，其中 path="/ping" 中的 = 称为匹配器，可以在 {} 中使用多个匹配器，使用，分隔。<br />匹配器有 4 种，path="/ping" 中的 = 是等式匹配器，此外，还有 != 不等、=~ 正则等、!~ 正则不等。
<a name="IdOxs"></a>
## 3、总结
本文主要介绍 PromQL 的一般使用方式，主要包括以下几个维度：

1. 指标名称
2. 标签
3. 聚合运算符
4. 子句
5. 函数
6. 选择器
7. 匹配器

除了指标名称和标签，在之前的文章介绍过之外，其它都属于新知识，聚合运算符共计 11 个，只使用了 sum，子句除了 without 之外，还有 by，函数也只是使用 rate 举例，建议读者朋友们阅读官方文档，了解所有聚合运算符和函数的使用方式。<br />需要注意的是，本文介绍的是 PromQL 的查询范畴，关于 PromQL 计算范畴的内容，都没有介绍， 比如，算术运算符、比较运算符、逻辑运算符，也是非常重要的内容，尤其在配置告警时会用到这些运算符。
