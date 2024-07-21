LVSNginx

- Nginx特点
   - 正向代理与反向代理
   - 负载均衡
   - 动静分离
- Nginx的优势
   - 可操作性大
   - 网络依赖小
   - 安装简单
   - 支持健康检查以及请求重发
- LVS 的优势
   - 抗负载能力强
   - 配置性低
   - 工作稳定
   - 无流量

---

总结一下负载均衡中LVS与Nginx的区别，好几篇博文一开始就说LVS是单向的，Nginx是双向的，个人认为这是不准确的，LVS三种模式中，虽然DR模式以及TUN模式只有请求的报文经过Director，但是NAT模式，Real Server回复的报文也会经过Director Server地址重写：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819418535-d438ad84-472f-41a9-9f83-cbd91ab4282c.png#averageHue=%23bbc83f&clientId=ufead2517-43bf-4&from=paste&id=u13cd0a52&originHeight=427&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf7e1d7fd-bfcc-429e-ab00-2016d0ebf8e&title=)<br />首先要清楚的一点是，LVS是一个四层的负载均衡器，虽然是四层，但并没有TCP握手以及分手，只是偷窥了IP等信息，而Nginx是一个七层的负载均衡器，所以效率势必比四层的LVS低很多，但是可操作性比LVS高，后面所有的讨论都是基于这个区别。<br />为什么四层比七层效率高?<br />四层是TCP层，使用IP+端口四元组的方式。只是修改下IP地址，然后转发给后端服务器，TCP三次握手是直接和后端连接的。只不过在后端机器上看到的都是与代理机的IP的established而已，LVS中没有握手。<br />7层代理则必须要先和代理机三次握手后，才能得到7层（HTT层）的具体内容，然后再转发。意思就是代理机必须要与client和后端的机器都要建立连接。显然性能不行，但胜在于七层，人工可操作性高，能写更多的转发规则。
<a name="d14E7"></a>
## **Nginx特点**
Nginx 专为性能优化而开发，性能是其最重要的要求，十分注重效率，有报告 Nginx 能支持高达 50000 个并发连接数。
<a name="FTwBR"></a>
### 正向代理与反向代理
**正向代理** ：局域网中的电脑用户想要直接访问服务器是不可行的，服务器可能Hold不住，只能通过代理服务器来访问，这种代理服务就被称为正向代理，特点是客户端知道自己访问的是代理服务器。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819418539-fa182e0c-339c-4be9-bccd-e6706f131b33.png#averageHue=%23efd396&clientId=ufead2517-43bf-4&from=paste&id=u3f87bbd6&originHeight=661&originWidth=1028&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uda4d41ea-d454-4b83-8fe1-67cff96dca6&title=)<br />**反向代理** ：客户端无法感知代理，因为客户端访问网络不需要配置，只要把请求发送到反向代理服务器，由反向代理服务器去选择目标服务器获取数据，然后再返回到客户端。<br />此时反向代理服务器和目标服务器对外就是一个服务器，暴露的是代理服务器地址，隐藏了真实服务器 IP 地址。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819418582-27bd2f5e-4866-456a-b277-021ef2d9e648.png#averageHue=%23efd396&clientId=ufead2517-43bf-4&from=paste&id=u0d969731&originHeight=611&originWidth=941&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2204d0fa-38e2-4639-9527-4f2439a9cf3&title=)
<a name="be7SX"></a>
### 负载均衡
客户端发送多个请求到服务器，服务器处理请求，有一些可能要与数据库进行交互，服务器处理完毕之后，再将结果返回给客户端。<br />普通请求和响应过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819418564-af42492f-e282-40ab-8f8d-db832233202e.png#averageHue=%23f8f4f0&clientId=ufead2517-43bf-4&from=paste&id=u271a1ca8&originHeight=437&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueeac5fe9-9157-4d14-ac28-e01ea1a4f7a&title=)<br />但是随着信息数量增长，访问量和数据量增长，单台的Server以及Database就成了系统的瓶颈，这种架构无法满足日益增长的需求，这时候要么提升单机的性能，要么增加服务器的数量。<br />关于提升性能，这儿就不赘述，提提如何增加服务器的数量，构建集群，将请求分发到各个服务器上，将原来请求集中到单个服务器的情况改为请求分发到多个服务器，也就是负载均衡。<br />图解负载均衡：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819418620-15836796-8bbe-4b3e-9cad-9c43560b7a5d.png#averageHue=%23f7f1ee&clientId=ufead2517-43bf-4&from=paste&id=u88cd0179&originHeight=452&originWidth=931&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u909cbe3d-4690-4db7-90f2-2981c4c570f&title=)<br />关于服务器如何拆分组建集群，这儿主要讲讲负载均衡，也就是图上的Proxy，可以是LVS，也可以是Nginx。假设有 15 个请求发送到代理服务器，那么由代理服务器根据服务器数量，这儿假如是平均分配，那么每个服务器处理 5 个请求，这个过程就叫做负载均衡。
<a name="dXUEp"></a>
### 动静分离
为了加快网站的解析速度，可以把动态页面和静态页面交给不同的服务器来解析，加快解析的速度，降低由单个服务器的压力。
<a name="mfKi7"></a>
#### 动静分离之前的状态
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819419136-b85e021e-0ea1-4a9a-93b6-1aff88bdcaed.png#averageHue=%23f8f5f2&clientId=ufead2517-43bf-4&from=paste&id=u54509ddc&originHeight=404&originWidth=768&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u721e8e85-b798-49e8-adcf-3b38dc515d4&title=)
<a name="rVu9w"></a>
#### 动静分离之后
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674819419134-fbf1a719-dbfb-4604-84ad-0f99a07dc621.png#averageHue=%23f8f5f4&clientId=ufead2517-43bf-4&from=paste&id=ub78742cd&originHeight=518&originWidth=730&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u30751135-1781-43d5-83fd-0017819f9f6&title=)<br />光看两张图可能有人不理解这样做的意义是什么，在进行数据请求时，以淘宝购物为例，商品详情页有很多东西是动态的，随着登录人员的不同而改变，例如用户ID，用户头像，但是有些内容是静态的，例如商品详情页，那么可以通过CDN(全局负载均衡与CDN内容分发)将静态资源部署在用户较近的服务器中，用户数据信息安全性要更高，可以放在某处集中，这样相对于将说有数据放在一起，能分担主服务器的压力，也能加速商品详情页等内容传输速度。
<a name="JATo4"></a>
## **Nginx的优势**
<a name="e4oCq"></a>
### 可操作性大
Nginx是一个应用层的程序，所以用户可操作性的空间大得多，可以作为网页静态服务器，支持 Rewrite 重写规则；支持 GZIP 压缩，节省带宽；可以做缓存；可以针对 http 应用本身来做分流策略，静态分离，针对域名、目录结构等相比之下 LVS 并不具备这样的功能，所以 nginx 单凭这点可以利用的场合就远多于 LVS 了；但 nginx 有用的这些功能使其可调整度要高于 LVS，所以经常要去触碰，人为出现问题的几率也就大
<a name="fNsLZ"></a>
### 网络依赖小
nginx 对网络的依赖较小，理论上只要 ping 得通，网页访问正常，nginx 就能连得通，nginx 同时还能区分内外网，如果是同时拥有内外网的节点，就相当于单机拥有了备份线路；LVS 就比较依赖于网络环境，目前来看服务器在同一网段内并且 LVS 使用 direct 方式分流，效果较能得到保证。另外注意，LVS 需要向托管商至少申请多于一个 ip 来做 visual ip
<a name="Jbx9y"></a>
### 安装简单
nginx 安装和配置比较简单，测试起来也很方便，因为它基本能把错误用日志打印出来。LVS 的安装和配置、测试就要花比较长的时间，因为同上所述，LVS 对网络依赖性比较大，很多时候不能配置成功都是因为网络问题而不是配置问题，出了问题要解决也相应的会麻烦的多<br />nginx 也同样能承受很高负载且稳定，但负载度和稳定度差 LVS 还有几个等级：nginx 处理所有流量所以受限于机器 IO 和配置；本身的 bug 也还是难以避免的；nginx 没有现成的双机热备方案，所以跑在单机上还是风险比较大，单机上的事情全都很难说
<a name="H3KwY"></a>
### 支持健康检查以及请求重发
nginx 可以检测到服务器内部的故障（健康检查），比如根据服务器处理网页返回的状态码、超时等等，并且会把返回错误的请求重新提交到另一个节点。目前 LVS 中 ldirectd 也能支持针对服务器内部的情况来监控，但 LVS 的原理使其不能重发请求。比如用户正在上传一个文件，而处理该上传的节点刚好在上传过程中出现故障，nginx 会把上传切到另一台服务器重新处理，而 LVS 就直接断掉了。
<a name="hU5po"></a>
## **LVS 的优势**
<a name="CKTT9"></a>
### 抗负载能力强
因为 LVS 工作方式的逻辑是非常简单的，而且工作在网络的第 4 层，仅作请求分发用，没有流量，所以在效率上基本不需要太过考虑。LVS 一般很少出现故障，即使出现故障一般也是其他地方（如内存、CPU 等）出现问题导致 LVS 出现问题
<a name="NdLN3"></a>
### 配置性低
这通常是一大劣势同时也是一大优势，因为没有太多的可配置的选项，所以除了增减服务器，并不需要经常去触碰它，大大减少了人为出错的几率
<a name="JiPBs"></a>
### 工作稳定
因为其本身抗负载能力很强，所以稳定性高也是顺理成章的事，另外各种 LVS 都有完整的双机热备方案，所以一点不用担心均衡器本身会出什么问题，节点出现故障的话，LVS 会自动判别，所以系统整体是非常稳定的
<a name="HMwO3"></a>
### 无流量
LVS 仅仅分发请求，而流量并不从它本身出去，所以可以利用它这点来做一些线路分流之用。没有流量同时也保住了均衡器的 IO 性能不会受到大流量的影响<br />LVS 基本上能支持所有应用，因为 LVS 工作在第 4 层，所以它可以对几乎所有应用做负载均衡，包括 http、数据库、聊天室等。
