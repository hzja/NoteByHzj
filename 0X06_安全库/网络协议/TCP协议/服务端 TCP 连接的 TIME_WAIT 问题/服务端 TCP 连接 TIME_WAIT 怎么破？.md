TCP
<a name="r9aKZ"></a>
### 问题描述
模拟高并发的场景，会出现批量的 TIME_WAIT 的 TCP 连接：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993933170-003044d1-5ba1-49c0-b40a-4ea94d307696.png#clientId=ucf1a97b6-81f7-4&from=paste&id=ueec9cb5a&originHeight=977&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9241a8b-f439-4a7f-87ca-f1479e0f404&title=)<br />短时间后，所有的 TIME_WAIT 全都消失，被回收，端口包括服务，均正常。即，在高并发的场景下，TIME_WAIT 连接存在，属于正常现象。<br />**线上场景中，持续的高并发场景：**

- 一部分 TIME_WAIT 连接被回收，但新的 TIME_WAIT 连接产生；
- 一些极端情况下，会出现大量的 TIME_WAIT 连接。

Think：上述大量的 TIME_WAIT 状态 TCP 连接，有什么业务上的影响吗？<br />Nginx 作为反向代理时，大量的短链接，可能导致 Nginx 上的 TCP 连接处于 time_wait 状态：

- 每一个 time_wait 状态，都会占用一个「本地端口」，上限为 65535(16 bit，2 Byte)；
- 当大量的连接处于 time_wait 时，新建立 TCP 连接会出错，address already in use : connect 异常

**统计 TCP 连接的状态：**
```bash
// 统计：各种连接的数量
$ netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'

ESTABLISHED 1154

TIME_WAIT 1645
```
Tips：TCP 本地端口数量，上限为 65535（6.5w），这是因为 TCP 头部使用 16 bit，存储「端口号」，因此约束上限为 65535。
<a name="RHEa8"></a>
### 问题分析
大量的 TIME_WAIT 状态 TCP 连接存在，其本质原因是什么？

- 大量的短连接存在
- 特别是 HTTP 请求中，如果 connection 头部取值被设置为 close 时，基本都由「服务端」发起主动关闭连接
- 而 TCP 四次挥手关闭连接机制中，为了保证 ACK 重发和丢弃延迟数据，设置 time_wait 为 2 倍的 MSL（报文最大存活时间）

**TIME_WAIT 状态：**

- TCP 连接中，主动关闭连接的一方出现的状态；（收到 FIN 命令，进入 TIME_WAIT 状态，并返回 ACK 命令）
- 保持 2 个 MSL 时间，即，4 分钟；（MSL 为 2 分钟）
<a name="EGPoV"></a>
### 解决办法
解决上述 time_wait 状态大量存在，导致新连接创建失败的问题，一般解决办法：

- 客户端，HTTP 请求的头部，connection 设置为 keep-alive，保持存活一段时间：现在的浏览器，一般都这么进行了
- 服务器端，允许 time_wait 状态的 socket 被重用、缩减 time_wait 时间，设置为 1 MSL（即，2 mins）

结论：几个核心要点<br />**time_wait 状态的影响：**

- TCP 连接中，「主动发起关闭连接」的一端，会进入 time_wait 状态
- time_wait 状态，默认会持续 2 MSL（报文的最大生存时间），一般是 2x2 mins
- time_wait 状态下，TCP 连接占用的端口，无法被再次使用
- TCP 端口数量，上限是 6.5w（65535，16 bit）
- 大量 time_wait 状态存在，会导致新建 TCP 连接会出错，address already in use : connect 异常

**现实场景：**

- 服务器端，一般设置：不允许「主动关闭连接」
- 但 HTTP 请求中，http 头部 connection 参数，可能设置为 close，则，服务端处理完请求会主动关闭 TCP 连接
- 现在浏览器中， HTTP 请求 connection 参数，一般都设置为 keep-alive
- Nginx 反向代理场景中，可能出现大量短链接，服务器端，可能存在。

**解决办法：**

- 服务器端允许 time_wait 状态的 socket 被重用
- 缩减 time_wait 时间，设置为 1 MSL（即，2 mins）
<a name="MgS3f"></a>
### 附录
几个方面：

- TCP 连接状态的查询
- MSL 时间
- TCP 三次握手和四次握手
<a name="JvmAf"></a>
#### 附录 A：查询 TCP 连接状态
Mac 下，查询 TCP 连接状态的具体命令：
```bash
// Mac 下，查询 TCP 连接状态
$ netstat -nat |grep TIME_WAIT

// Mac 下，查询 TCP 连接状态，其中 -E 表示 grep 或的匹配逻辑
$ netstat -nat | grep -E "TIME_WAIT|Local Address"
Proto Recv-Q Send-Q Local Address Foreign Address (state)
tcp4 0 0 127.0.0.1.1080 127.0.0.1.59061 TIME_WAIT

// 统计：各种连接的数量
$ netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
ESTABLISHED 1154
TIME_WAIT 1645
```
<a name="KlKnL"></a>
#### 附录 B：MSL 时间
MSL，Maximum Segment Lifetime，“报文最大生存时间”

- 任何报文在网络上存在的最长时间，超过这个时间报文将被丢弃。（IP 报文）
- TCP报文 （segment）是ip数据报（datagram）的数据部分。

Tips：RFC 793中规定MSL为2分钟，实际应用中常用的是30秒，1分钟和2分钟等。<br />2MSL，TCP 的 TIME_WAIT 状态，也称为2MSL等待状态：

- 当TCP的一端发起主动关闭（收到 FIN 请求），在发出最后一个ACK 响应后，即第3次握 手完成后，发送了第四次握手的ACK包后，就进入了TIME_WAIT状态。
- 必须在此状态上停留两倍的MSL时间，等待2MSL时间主要目的是怕最后一个 ACK包对方没收到，那么对方在超时后将重发第三次握手的FIN包，主动关闭端接到重发的FIN包后，可以再发一个ACK应答包。
- 在 TIME_WAIT 状态时，两端的端口不能使用，要等到2MSL时间结束，才可继续使用。（IP 层）
- 当连接处于2MSL等待阶段时，任何迟到的报文段都将被丢弃。不过在实际应用中，可以通过设置 「SO_REUSEADDR选项」，达到不必等待2MSL时间结束，即可使用被占用的端口。
<a name="SoC1C"></a>
#### 附录 C：TCP 三次握手和四次握手
具体示意图：

- 三次握手，建立连接过程
- 四次挥手，释放连接过程

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993933090-5bf7fd30-b9e5-4a3a-9733-e0edaca3fba8.png#clientId=ucf1a97b6-81f7-4&from=paste&id=u3da78146&originHeight=937&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6a47839e-2fee-49fb-b1ab-c2a2827c661&title=)
> 转自：高效运维 作者：NingG
> [ningg.top/computer-basic-theory-tcp-time-wait/](https://ningg.top/computer-basic-theory-tcp-time-wait/)

