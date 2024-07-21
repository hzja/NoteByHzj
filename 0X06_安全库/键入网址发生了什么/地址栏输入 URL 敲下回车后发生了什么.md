![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417642715-7af5e427-570c-4190-a0dd-64267579e105.webp#clientId=u0fcebbae-8c73-4&from=paste&id=u1f668cdd&originHeight=517&originWidth=577&originalType=url&ratio=3&status=done&style=shadow&taskId=u920050fd-60a4-4f83-9ae5-d2604aac9bf)
<a name="dBJUn"></a>
## 一、简单分析
简单的分析，从输入 URL到回车后发生的行为如下：

- URL解析
- DNS 查询
- TCP 连接
- HTTP 请求
- 响应请求
- 页面渲染
<a name="Kwoh8"></a>
## 二、详细分析
<a name="qnPZF"></a>
### URL解析
首先判断输入的是一个合法的URL 还是一个待搜索的关键词，并且根据输入的内容进行对应操作<br />一个url的结构解析如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417642756-c7d388dc-3a6b-4d8a-97d8-2ede557c2380.webp#clientId=u0fcebbae-8c73-4&from=paste&id=u4bb7f92b&originHeight=254&originWidth=1020&originalType=url&ratio=3&status=done&style=shadow&taskId=u199e93a9-8319-4bce-b1c1-5b82fcb668a)
<a name="g4kN0"></a>
### DNS查询
整个查询过程如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417642965-d251223a-9617-4f64-a1e0-2cd4cd545976.webp#clientId=u0fcebbae-8c73-4&from=paste&id=u78abe30d&originHeight=503&originWidth=691&originalType=url&ratio=3&status=done&style=shadow&taskId=uac6f4c2e-eca6-4a35-bf83-706c282846b)<br />最终，获取到了域名对应的目标服务器IP地址
<a name="u3U4S"></a>
### TCP连接
tcp是一种面向有连接的传输层协议<br />在确定目标服务器服务器的IP地址后，则经历三次握手建立TCP连接，流程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417642729-4a3b3e92-3b88-4a29-8150-5f24fd1e2d67.webp#clientId=u0fcebbae-8c73-4&from=paste&id=ue0cd2217&originHeight=399&originWidth=739&originalType=url&ratio=3&status=done&style=shadow&taskId=ub73d1fe7-7985-490b-9f95-4882b6e2d2a)
<a name="cl5FC"></a>
### 发送 http 请求
当建立`tcp`连接之后，就可以在这基础上进行通信，浏览器发送 http 请求到目标服务器<br />请求的内容包括：

- 请求行
- 请求头
- 请求主体

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417642787-f429df7a-5a9c-4974-b6f0-132f5b07f34b.webp#clientId=u0fcebbae-8c73-4&from=paste&id=u5bfa8a21&originHeight=461&originWidth=874&originalType=url&ratio=3&status=done&style=shadow&taskId=uf8b25c11-0660-4dca-83fe-9a623181cc2)
<a name="nBump"></a>
### 响应请求
当服务器接收到浏览器的请求之后，就会进行逻辑操作，处理完成之后返回一个HTTP响应消息，包括：

- 状态行
- 响应头
- 响应正文

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417643211-82949c80-286f-41c1-b5cf-8b901f81df58.webp#clientId=u0fcebbae-8c73-4&from=paste&id=ua0aee270&originHeight=270&originWidth=544&originalType=url&ratio=3&status=done&style=shadow&taskId=ud4d7c8e7-27de-45a2-9f1d-cb49d248985)<br />在服务器响应之后，由于现在`http`默认开始长连接`keep-alive`，当页面关闭之后，`tcp`链接则会经过四次挥手完成断开
<a name="d4C71"></a>
### 页面渲染
当浏览器接收到服务器响应的资源后，首先会对资源进行解析：

- 查看响应头的信息，根据不同的指示做对应处理，比如重定向，存储cookie，解压gzip，缓存资源等等
- 查看响应头的 Content-Type的值，根据不同的资源类型采用不同的解析方式

关于页面的渲染过程如下：

- 解析HTML，构建 DOM 树
- 解析 CSS ，生成 CSS 规则树
- 合并 DOM 树和 CSS 规则，生成 render 树
- 布局 render 树（ Layout / reflow ），负责各元素尺寸、位置的计算
- 绘制 render 树（ paint ），绘制页面像素信息
- 浏览器会将各层的信息发送给 GPU，GPU 会将各层合成（ composite ），显示在屏幕上

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624417643283-0de9044d-6417-4271-8ae9-84b99d82227b.webp#clientId=u0fcebbae-8c73-4&from=paste&id=u6c683498&originHeight=607&originWidth=597&originalType=url&ratio=3&status=done&style=shadow&taskId=u0034ef6c-9b98-41e7-aa8f-388a38b7eea)
