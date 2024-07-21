CDN
<a name="Ae715"></a>
## CDN简介
CDN的全称是Content Delivery Network，即内容分发网络。CDN是构建在现有网络基础之上的智能虚拟网络，依靠部署在各地的边缘服务器，通过中心平台的负载均衡、内容分发、调度等功能模块，使用户就近获取所需内容，降低网络拥塞，提高用户访问响应速度和命中率。CDN的关键技术主要有内容存储和分发技术。<br />简而言之，就是将数据部署在各地的服务器中，通过负载均衡技术，让用户就近获取服务器中的数据。
<a name="P2DHU"></a>
## CDN原理
CDN的基本原理是广泛采用各种缓存服务器，将这些缓存服务器分布到用户访问相对集中的地区或网络中，在用户访问网站时，利用全局负载技术将用户的访问指向距离最近的工作正常的缓存服务器上，由缓存服务器直接响应用户请求。<br />全局负载均衡主要用于在多个区域拥有自己服务器的站点，为了使全球用户只以一个IP地址或域名就能访问到离自己最近的服务器，从而获得最快的访问速度。<br />CDN的基本思路是尽可能避开互联网上有可能影响数据传输速度和稳定性的瓶颈和环节，使内容传输的更快、更稳定。通过在网络各处放置节点服务器所构成的在现有的互联网基础之上的一层智能虚拟网络，CDN系统能够实时地根据网络流量和各节点的连接、负载状况以及到用户的距离和响应时间等综合信息将用户的请求重新导向离用户最近的服务节点上。其目的是使用户可就近取得所需内容，解决 Internet网络拥挤的状况，提高用户访问网站的响应速度。 <br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671977436581-d4ce7c3f-cdcb-403a-bf9b-1ee565319e11.png#averageHue=%23ebe5c3&clientId=u70612f55-3c4d-4&from=paste&id=u7d144df9&originHeight=685&originWidth=726&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uce7b8eac-9068-4ff3-8311-88f64ce89e7&title=)<br />CDN功能 归纳起来，CDN具有以下主要功能：

1. 节省骨干网带宽，减少带宽需求量；
2. 提供服务器端加速，解决由于用户访问量大造成的服务器过载问题；
3. 服务商能使用Web Cache技术在本地缓存用户访问过的Web页面和对象，实现相同对象的访问无须占用主干的出口带宽，并提高用户访问因特网页面的相应时间的需求；
4. 能克服网站分布不均的问题，并且能降低网站自身建设和维护成本；
5. 降低“通信风暴”的影响，提高网络访问的稳定性。
<a name="U4Jb3"></a>
## CDN服务模式
简单地说，内容分发网络（CDN）是一个经策略性部署的整体系统，包括分布式存储、负载均衡、网络请求的重定向和内容管理4个要件，而内容管理和全局的网络流量管理（Traffic Management）是CDN的核心所在。通过用户就近性和服务器负载的判断，CDN确保内容以一种极为高效的方式为用户的请求提供服务。总的来说，内容服务基于缓存服务器，也称作代理缓存（Surrogate），它位于网络的边缘，距用户仅有"一跳"（Single Hop）之遥。<br />同时，代理缓存是内容提供商源服务器（通常位于CDN服务提供商的数据中心）的一个透明镜像。这样的架构使得CDN服务提供商能够代表他们客户，即内容供应商，向最终用户提供尽可能好的体验，而这些用户是不能容忍请求响应时间有任何延迟的。
<a name="pa6VR"></a>
## 全局负载均衡
全局负载均衡（Global Server Load Balance, GSLB），全局负载均衡是指对分别放置在不同的地理位置的服务器群间作负载均衡。服务器负载均衡是指对本地的服务器群做负载均衡。主要用于在多个区域拥有自己服务器的站点，为了使全球用户只以一个IP地址或域名就能访问到离自己最近的服务器，从而获得最快的访问速度。
<a name="NZQsv"></a>
### 服务器群选择
对于全局负载均衡而言，其核心就是服务器群的选择。对于某个特定的客户，应该将其定向到哪一个服务群？应该使用什么标准来进行这种选择？一般情况下，主要考虑两个因素：临近程度和负载大小。<br />临近机制主要考察服务器群与用户之间的物理距离。选择地理位置最接近用户的服务器集群，可以减少服务响应到达用户所经过的中转次数，从而降低中转节点对服务质量的影响。常见的有两种方式，一种是静态配置，例如根据静态的IP地址配置表进行IP地址到服务器群的映射。另一种方式是动态的检测，例如实时地探测到目标IP的距离（可以采用到达目标IP经过的跳数作为度量单位），然后比较探测结果进行选择。<br />负载机制比较各个服务器群的负载，确定由哪一个服务器群来响应请求。在全局负载均衡中，考察的是服务器群的负载，而不是单个服务器的负载，因此，需要更多地考虑普遍的问题，比如，需要考虑站点的最大连接数、站点的平均响应时间、服务质量等。<br />常见的GSLB实现方式有三种：DNS轮询、HTTP重定向、IP欺骗（又称三角传输）。这三种实现方式都是在用户通过域名来访问目标服务器时，由GSLB设备进行智能决策，将用户引导到一个最佳的服务IP。
<a name="ETbvi"></a>
### 基于DNS的GSLB
用户访问某个网站时，需要首先通过域名解析服务（DNS）获得网站的IP。域名解析通常不是一次性完成的，常常需要查询若干不同的域名服务器才能找到对应的IP。如下图所示，用户首先在本地配置一个本地DNS服务器地址，本地DNS服务器收到DNS请求后若不能解析，会将请求转发给更高一级的DNS服务器直到找到域名对应的IP或确定域名不存在。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671977436588-dfe10355-3ade-437a-918c-c64f6504d1a4.png#averageHue=%23f6f3f2&clientId=u70612f55-3c4d-4&from=paste&id=u9a502dc1&originHeight=581&originWidth=768&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5c183a1a-f338-4899-adf8-5d8f5088b30&title=)<br />对于加入了GSLB的情况，一个GSLB设备（可能是一个4层交换机）会最终代替DNS服务器完成域名解析。下图展示两种流程的不同。 <br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671977436620-9e358748-70d4-472f-806d-1e09707da4bb.png#averageHue=%23f8f6f5&clientId=u70612f55-3c4d-4&from=paste&id=u4a61fecf&originHeight=322&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub66a0704-099f-42d9-ace9-8f43f7a03b3&title=)
<a name="ye4ti"></a>
### 基于DNS的GSLB优缺点
优点是：实现简单、实施容易、成本低。<br />缺点是：当GSLB设备采用“用户就近访问”的原则作为选择最优服务器的策略时，会存在判断不准的现象。原因是在这种策略下，GSLB设备是根据用户IP地址和内容服务器IP地址比较来判断其就近性的，但由于DNS响应是通过本地DNS服务器到达用户的，GSLB设备实际上只能得到用户的本地DNS服务器地址，若用户指定的DNS服务器IP不能正确代表用户的实际位置，就会出现判断不准的现象。
<a name="AEBc2"></a>
### 基于HTTP重定向的GSLB
为了解决基于DNS实现方式判断不准的问题，又出现了基于HTTP重定向的GSLB。这种方案中GSLB使用HTTP重定向技术，将用户访问重定向到最合适的服务器上。 <br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671977436611-1a6fe4e4-823f-49fd-bb3b-8b455db9a5c6.png#averageHue=%23f2f0ee&clientId=u70612f55-3c4d-4&from=paste&id=uf3ef3684&originHeight=472&originWidth=684&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud8807aaa-943c-4cca-b312-45113724282&title=)<br />使用基于HTTP重定向方案，首先在DNS中将GSLB设备的IP地址登记为域名的A记录（既域名对应的IP）。如上图所示，用户首先通过DNS得到GSLB设备的IP地址，此时用户以为这就是站点服务器的IP，并向其发送HTTP请求。GSLB设备收到HTTP请求后使用一定策略选择一个最合适的服务器，然后GSLB设备向用户发送一个HTTP重定向指令（HTTP302），并附上选出的服务器的IP地址。最后，用户根据重定向IP访问站点的服务器。
<a name="JMJeK"></a>
### 基于HTTP重定向的GSLB优缺点
优点：由于直接向用户发送HTTP重定向指令，可以得到用户的真实IP，从而解决了判断不准确的问题。<br />缺点是只能为HTTP访问重定向。
<a name="WDbS6"></a>
### 基于IP欺骗的GSLB
HTTP重定向方案解决了判断不准确的问题，但只能针对HTTP协议应用使用。对于HTTP协议以外的访问，就需要使用基于IP欺骗（又称三角传输）的GSLB。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671977436709-e7874b08-1d0a-416c-ac2d-c831bca9354c.png#averageHue=%23f4f2f1&clientId=u70612f55-3c4d-4&from=paste&id=u6ddc71e6&originHeight=526&originWidth=705&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u75832d77-b473-4e16-bad2-c051488a3ff&title=)<br />基于IP欺骗的方案同样需要首先将GSLB设备的IP地址在DNS中登记为域名的A记录，这样用户对该域名的请求包都会先发送到GSLB设备。如上图所示，GSLB设备首次收到服务请求包后，会选择一个最合适的服务器，并将服务请求包发送到该服务器。服务器在向用户发送响应包时，将其源IP地址字段改为GSLB设备的IP，发送给用户。<br />这样，整个过程对用户来说，感觉到的只是GSLB设备在为其提供服务，并不知道其中经历这样一个三角传输的过程。而且这种方案可以对所有类型的访问如HTTP、FTP等进行重定向，但其速度和效率相对比前两种方案要差一点，因为用户所有的访问请求都通过三个点才能响应，经历了更多的路径和处理，所以其主要作为HTTP重定向方案的补充方案在同一GSLB设备中实现。
<a name="hehvx"></a>
### 服务器群选择策略
上文中介绍的三种方案，解决了如何将用户引导到指定服务器群的问题，而在此之前首先需要使用某种方式选出最适合用户的服务器群，也就是GSLB在选择服务器群时所采用的策略。接下来介绍一些常用的GSLB策略。<br />1)地理区域或用户自定义区域：将若干条IP地址前缀划分一个区域为。根据用户本地DNS的IP地址，将特定IP范围的用户优先分配到某个通过健康检查的站点。<br />2)IP地址权重：可以为DNS应答中的每个IP地址分配权重，权重决定与其他候选IP相比分配到该IP的流量比例。<br />3)往返时间（Round Trip Time, RTT）：RTT策略是基于区域之外最常用的策略。有两种模式的RTT测量：Active RTT测量与Passive RTT测量。在实际部署中，由于网络限制和性能原因，Active RTT往往无法使用，Passive RTT更实用一些。<br />a) Active RTT 测量：<br />当GSLB Controller收到来自LDNS的DNS请求时，GSLB Controller会通知所有站点负载均衡设备对该LDNS进行RTT测量。根据采集到的RTT值，GSLB Controller会选择RTT值最小的站点的VIP返回给LDNS。<br />由于Active RTT采用DNS Query或ICMP进行RTT测量，在有些网络中可能会被安全策略所过滤而无法工作。<br />Active RTT测量会产生额外的DNS Query或ICMP流量，在有些网络中用户不希望有太多类似的非用户流量。<br />b) Passive RTT测量：<br />Passive RTT测量指从内容站点收到一个用户发出连接请求(发送TCN SYN)到接收到用户的确认(收到TCP ACK)所经历的时间。而不是简单的PING的响应时间，可以更精确的衡量访问最快的站点。<br />Passive RTT测量不会主动去进行测量，也不会产生额外的数据流量，而是在用户向返回的VIP建立连接时进行采集。<br />Passive RTT的测量值真正反映了用户的上网感受，在运营商网络中也不会产生额外流量。也不会受到其他运营商或网络的安全策略的影响。
