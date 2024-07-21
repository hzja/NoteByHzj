网络 DNS
<a name="iJFqp"></a>
## DNS简介
DNS（Domain Name System）， 也叫网域名称系统，是互联网的一项服务。它实质上是一个 域名 和 IP 相互映射的分布式数据库，有了它，就可以通过域名更方便的访问互联网。
<a name="LHVlJ"></a>
### DNS 有以下特点

- 分布式的
- 协议支持 TCP 和 UDP，常用端口是 53
- 每一级域名的长度限制是 63
- 域名总长度限制是 253
<a name="mzqAF"></a>
### 那么，什么情况下使用 TCP，什么情况下使用 UDP 呢?
最早的时候，DNS 的 UDP 报文上限大小是 512 字节， 所以当某个 response 大小超过512 (返回信息太多)，DNS 服务就会使用 TCP 协议来传输。后来 DNS 协议扩展了自己的UDP 协议，DNS client 发出查询请求时，可以指定自己能接收超过512字节的 UDP 包， 这种情况下，DNS 还是会使用 UDP 协议。
<a name="RkPAc"></a>
### 分层的数据库结构
DNS 的结构跟 Linux 文件系统很相似，像一棵倒立的树。下面用站长之家的域名举例：<br />![2021-06-02-10-03-18-873247.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599673940-5c9d7341-0a9e-49a3-be0d-a877808899a2.jpeg#clientId=ucb833c61-1632-4&from=ui&id=ub45099ff&originHeight=554&originWidth=523&originalType=binary&size=11689&status=done&style=shadow&taskId=ud8abf9fe-69f6-4ea0-b126-a772a76475a)<br />最上面的.是根域名，接着是顶级域名com，再下来是站长之家域名 chinaz 依次类推。使用域名时，从下而上。s.tool.chinaz.com. 就是一个完整的域名，www.chinaz.com. 也是。<br />之所以设计这样复杂的树形结构， 是为了防止名称冲突。这样一棵树结构，当然可以存储在一台机器上，但现实世界中完整的域名非常多，并且每天都在新增、删除大量的域名，存在一台机器上，对单机器的存储性能就是不小的挑战。另外，集中管理还有一个缺点就是管理不够灵活。可以想象一下，每次新增、删除域名都需要向中央数据库申请是多么麻烦。所以现实中的 DNS 都是分布式存储的。<br />根域名服务器只管理顶级域，同时把每个顶级域的管理委派给各个顶级域，所以当你想要申请com下的二级域名时，找 com 域名注册中心就好了。例如申请了上图的 chinaz.com 二级域名，chinaz.com 再向下的域名就归你管理了。当你管理 chinaz.com 的子域名时，可以搭建自己的 nameserver，在 .com 注册中心把 chinaz.com 的管理权委派给自己搭建的nameserver。自建nameserver 和不自建的结构图如下:<br />![2021-06-02-10-03-18-984946.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599685176-b7b073c2-82e2-410d-abb9-537e97d70798.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u1cb1b424&originHeight=296&originWidth=640&originalType=binary&size=17026&status=done&style=shadow&taskId=u97f5fdf4-cce2-4de1-90e1-f166ae7b27c)<br />一般情况下，能不自建就不要自建，因为维护一个高可用的 DNS 也并非容易。有两种情况需要搭建自己的 nameserver：

1. 搭建对内的 DNS。公司内部机器众多，通过 IP 相互访问太过凌乱，这时可以搭建对内的 nameserver，允许内部服务器通过域名互通
2. 公司对域名厂商提供的 nameserver 性能不满意。虽然顶级域名注册商都有自己的nameserver，但注册商提供的 nameserver 并不专业，在性能和稳定性上无法满足企业需求，这时就需要企业搭建自己的高性能 nameserver，比如增加智能解析功能，让不同地域的用户访问最近的 IP，以此来提高服务质量

概括一下 DNS 的分布式管理， 当把一个域委派给一个nameserver后，这个域下的管理权都交由此nameserver处理。这种设计一方面解决了存储压力，另一方面提高了域名管理的灵活性 (这种结构像极了Linux File System, 可以把任何一个子目录挂载到另一个磁盘，还可以把它下面的子目录继续挂载出去)
<a name="B4LJI"></a>
### 顶级域名
像 com 这样的顶级域名，由 ICANN 严格控制，是不允许随便创建的。顶级域名分两类:

- 通用顶级域名
- 国家顶级域名

通用顶级域名常见的如.com、.org、.edu等， 国家顶级域名如我国的.cn， 美国的.us。一般公司申请公网域名时，如果是跨国产品，应该选择通用顶级域名；如果没有跨国业务，看自己喜好（可以对比各家顶级域的服务、稳定性等再做选择）。这里说一下几个比较热的顶级域，完整的顶级域参见维基百科。
<a name="zXYxG"></a>
#### me
me顶级域其实是国家域名， 是黑山共和国的国家域名，只不过它对个人开发申请，所以很多个人博主就用它作为自己的博客域名（本博客也是这么来的~)
<a name="E4tIQ"></a>
#### io
很多开源项目常用io做顶级域名，它也是国家域名。因为io 与计算机中的 input/output 缩写相同，和计算机的二机制10也很像，给人一种geek的感觉。相较于.com域名，.io下的资源很多，更多选择。
<a name="vwH6A"></a>
## DNS 解析流程
聊完了 DNS 的基本概念，再来聊一聊 DNS 的解析流程。当通过浏览器或者应用程序访问互联网时，都会先执行一遍 DNS 解析流程。标准 glibc 提供了 libresolv.so.2 动态库，应用程序就是用它进行域名解析（也叫 resolving）的， 它还提供了一个配置文件/etc/nsswitch.conf 来控制 resolving 行为，配置文件中最关键的是这行：<br />hosts:      files dns myhostname<br />它决定了 resolving 的顺序，默认是先查找 hosts 文件，如果没有匹配到，再进行 DNS 解析。默认的解析流程如下图：<br />![2021-06-02-10-03-19-057751.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599693967-4b988b56-62f2-40aa-a6eb-ef36f1d7e24f.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u290bc833&originHeight=537&originWidth=615&originalType=binary&size=22040&status=done&style=shadow&taskId=ud9cc8be8-9725-4242-8a43-c90d713f307)<br />上图主要描述了 client 端的解析流程，可以看到最主要的是第四步请求本地 DNS 服务器去执行 resolving，它会根据本地 DNS 服务器配置，发送解析请求到递归解析服务器（稍后介绍什么是递归解析服务器)， 本地 DNS 服务器在 /etc/resolv.conf 中配置。下面再来看看服务端的 resolving 流程：<br />![2021-06-02-10-03-19-147511.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599702761-03796d11-66c7-4fef-a848-44db3ac4e32c.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u49b7930d&originHeight=365&originWidth=640&originalType=binary&size=20661&status=done&style=shadow&taskId=uc9924fcb-c186-4565-94c5-76740f45da0)<br />分析一下解析流程：

1. 客户端向本地DNS服务器(递归解析服务器) 发出解析tool.chinaz.com域名的请求
2. 本地dns服务器查看缓存，是否有缓存过tool.chinaz.com域名，如果有直接返回给客户端；如果没有执行下一步
3. 本地dns服务器向根域名服务器发送请求，查询com顶级域的nameserver 地址
4. 拿到com域名的IP后，再向com nameserver发送请求，获取chinaz域名的nameserver地址
5. 继续请求 chinaz 的nameserver，获取 tool 域名的地址，最终得到了tool.chinaz.com 的 IP，本地 dns 服务器把这个结果缓存起来，以供下次查询快速返回
6. 本地dns服务器把把结果返回给客户端
<a name="kcx0C"></a>
### 递归解析服务器 vs 权威域名服务器
在解析流程中发现两类 DNS 服务器，客户端直接访问的是 递归解析服务器， 它在整个解析过程中也最忙。它的查询步骤是递归的，从根域名服务器开始，一直询问到目标域名。<br />递归解析服务器通过请求一级一级的权威域名服务器，获得下一目标的地址，直到找到目标域名的权威域名服务器<br />简单来说：递归解析服务器是负责解析域名的，权威域名服务器是负责存储域名记录的<br />递归解析服务器一般由 ISP 提供，除此之外也有一些比较出名的公共递归解析服务器， 如谷歌的 8.8.8.8，联通的 114，BAT 也都有推出公共递归解析服务器，但性能最好的应该还是你的ISP提供的，只是可能会有 DNS劫持的问题
<a name="RAHmC"></a>
### 缓存
由于整个解析过程非常复杂，所以 DNS 通过缓存技术来实现服务的鲁棒性。当递归nameserver 解析过 tool.chianaz.com 域名后，再次收到 tool.chinaz.com 查询时，它不会再走一遍递归解析流程，而是把上一次解析结果的缓存直接返回。并且它是分级缓存的，也就是说，当下次收到的是 www.chinaz.com 的查询时， 由于这台递归解析服务器已经知道 chinaz.com 的权威 nameserver，所以它只需要再向 chinaz.com nameserver 发送一个查询 www 的请求就可以了。<br />根域名服务器递归解析服务器是怎么知道根域名服务器的地址的呢？根域名服务器的地址是固定的，目前全球有13个根域名解析服务器，这13条记录持久化在递归解析服务器中：<br />![2021-06-02-10-03-19-227298.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599713762-757ea23a-497f-405b-86f7-e86c091f2c1a.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u5d5727bf&originHeight=671&originWidth=610&originalType=binary&size=67249&status=done&style=shadow&taskId=u0c80d8d7-49ad-43e8-a6e5-b071f088c05)<br />为什么只有 13 个根域名服务器呢，不是应该越多越好来做负载均衡吗？之前说过 DNS 协议使用了 UDP 查询， 由于 UDP 查询中能保证性能的最大长度是 512 字节，要让所有根域名服务器数据能包含在512字节的UDP包中， 根服务器只能限制在13个， 而且每个服务器要使用字母表中单字母名<br />智能解析<br />智能解析，就是当一个域名对应多个 IP 时，当你查询这个域名的 IP，会返回离你最近的 IP。<br />由于国内不同运营商之间的带宽很低，所以电信用户访问联通的IP就是一个灾难，而智能 DNS 解析就能解决这个问题。<br />智能解析依赖 EDNS 协议，这是 google 起草的 DNS 扩展协议， 修改比较简单，就是在 DNS 包里面添加 origin client IP, 这样 nameserver 就能根据 client IP 返回距离 client 比较近的 server IP 了<br />国内最新支持 EDNS 的就是 DNSPod 了，DNSPod 是国内比较流行的域名解析厂商，很多公司会把域名利用DNSPod 加速， 它已经被鹅厂收购
<a name="hQpzz"></a>
## 域名注册商
一般要注册域名，都要需要找域名注册商，比如说想注册 hello.com，那么需要找com域名注册商注册hello域名。com的域名注册商不止一家， 这些域名注册商也是从ICANN 拿到的注册权， 参见如何申请成为.com域名注册商<br />那么，域名注册商 和 权威域名解析服务器  有什么关系呢？<br />域名注册商都会自建权威域名解析服务器，比如你在狗爹上申请一个.com下的二级域名，并不需要搭建nameserver， 直接在godaddy控制中心里管理域名指向就可以了， 原因就是你新域名的权威域名服务器默认由域名注册商提供。当然也可以更换，比如从godaddy申请的境外域名，把权威域名服务器改成DNSPod，一方面加快国内解析速度，另一方面还能享受DNSPod 提供的智能解析功能
<a name="CwvLp"></a>
## 用 bind 搭建域名解析服务器
这里主要介绍一下bind 的配置文件：<br />bind 的配置文件分两部分: bind配置文件 和 zone配置文件
<a name="CLHJp"></a>
### bind 配置文件
bind 配置文件位于 /etc/named.conf，它主要负责 bind 功能配置，如 zone 路径、日志、安全、主从等配置<br />![2021-06-02-10-03-19-344783.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599726481-43c358dc-ab8e-4636-a2da-b16f87efd2aa.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u596cc7ea&originHeight=336&originWidth=640&originalType=binary&size=28150&status=done&style=shadow&taskId=ud9fd9472-41d9-4ad2-a590-aa861b157bc)<br />其中最主要的是添加zone的配置以及指定zone配置文件。recursion 开启递归解析功能， 这个如果是no， 那么此bind服务只能做权威解析服务，当你的bind服务对外时，打开它会有安全风险，如何防御不当，会让你的nameserver 被hacker 用来做肉鸡
<a name="MQ3fh"></a>
### zone 配置文件
zone 的配置文件在 bind 配置文件中指定，下图是一份简单的 zone 配置：<br />![2021-06-02-10-03-19-409339.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599735496-636ba7d9-e149-4933-80ae-b2a5f5d6f0b0.jpeg#clientId=ucb833c61-1632-4&from=ui&id=ufbcd3d97&originHeight=249&originWidth=640&originalType=binary&size=20813&status=done&style=shadow&taskId=uf7a45ab3-fb0b-4021-bedd-b575acdbcd3)<br />zone的配置是 nameserver 的核心配置， 它指定了 DNS 资源记录，如 SOA、A、CNAME、AAAA 等记录，各种记录的概念网上资料太多，这里就不重复了。其中主要讲一下 SOA 和 CNAME 的作用。<br />SOA记录<br />SOA 记录表示此域名的权威解析服务器地址。上文讲了权威解析服务器和递归解析服务器的差别， 当所有递归解析服务器中有没你域名解析的缓存时，它们就会回源来请求此域名的SOA记录，也叫权威解析记录<br />CNAME<br />CNAME 的概念很像别名，它的处理逻辑也如此。一个server 执行resloving 时，发现 name 是一个 CNAME， 它会转而查询这个 CNAME 的A记录。一般来说，能使用CNAME的地方都可以用A记录代替， 那么为什么还要发明 CNAME 这样一个东西呢？它是让多个域名指向同一个 IP 的一种快捷手段， 这样当最低层的 CNAME 对应的IP换了之后，上层的 CNAME 不用做任何改动。就像代码中的硬编码，总会去掉这些硬编码，用一个变量来表示，这样当这个变量变化时，只需要修改一处<br />配置完之后可以用 named-checkconf 和 named-checkzone 两个命令来check配置文件有没有问题， 之后就可以启动 bind 服务了：
```bash
$> service named startRedirecting to /bin/systemctl restart  named.service
```
用 `netstat -ntlp` 来检查一下服务是否启动:<br />![2021-06-02-10-03-19-485135.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599745034-df83815a-520f-481f-b585-c92eed772087.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u3a8bbb33&originHeight=330&originWidth=640&originalType=binary&size=27850&status=done&style=shadow&taskId=ubc201655-db8d-468a-94d3-ca752b63d49)<br />53 端口已启动，那么测试一下效果， 用 dig 解析一下 www.hello.com 域名，使用127.0.0.1 作为递归解析服务器<br />![2021-06-02-10-03-19-594846.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599762689-6c846249-e307-420f-ad43-0e78068986b9.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u89a97c7d&originHeight=508&originWidth=640&originalType=binary&size=37139&status=done&style=shadow&taskId=ue598cd64-d26d-4f82-81ee-8dee03ceb58)<br />可以看到 dig 的结果跟配置文件中配置的一样是 1.2.3.4，DNS 完成了它的使命，根据域名获取到 IP，但这里用来做示范的IP明显是个假IP:)
<a name="s1ycn"></a>
### 用 DNS 实现负载均衡
一个域名添加多条A记录，解析时使用轮询的方式返回随机一条，流量将会均匀分类到多个A记录。
```bash
www     IN      A       1.2.3.4www     IN      A       1.2.3.5
```
复制代码上面的配置中，给 www 域添加了两条A记录， 这种做法叫 multi-homed hosts， 它的效果是：当请求 nameserver 解析 www.hello.com 域名时，返回的IP会在两个IP中轮转（默认行为，有些智能解析 DNS 会根据 IP 判断，返回一个离client近的IP，距离 请搜索 DNS 智能解析）。<br />其实每次DNS解析请求时，nameserver都会返回全部IP，如上面配置，它会把1.2.3.4 和1.2.3.5 都返回给client端。那么它是怎么实现RR的呢？nameserver 只是每次返回的IP排序不同，客户端会把response里的第一个IP用来发请求。
<a name="VjdyW"></a>
#### DNS负载均衡 vs LVS专业负载均衡
和 LVS 这种专业负载均衡工具相比，在DNS层做负载均衡有以下特点:

1. 实现非常简单
2. 默认只能通过RR方式调度
3. DNS 对后端服务不具备健康检查
4. DNS 故障恢复时间比较长（DNS 服务之间有缓存）
5. 可负载的 rs 数量有限（受 DNS response 包大小限制)

真实场景中，还需要根据需求选择相应的负载均衡策略
<a name="nhtbR"></a>
### 子域授权
从 .com 域下申请一个二级域名 hello.com 后， 发展到某一天公司扩大了，需要拆分两个事业部A和B， 并且公司给他们都分配了三级域名  a.hello.com 和 b.hello.com， 域名结构如下图：<br />![2021-06-02-10-03-19-653684.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599785937-ca19e6cc-791d-46c6-8a40-3136af6904f7.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u231aa304&originHeight=463&originWidth=465&originalType=binary&size=16902&status=done&style=shadow&taskId=ubc22a190-45de-4e6d-b7ac-02cdfe45315)<br />再发展一段时间，A部门和B部门内部业务太多，需要频繁的为新产品申请域名， 这个时候他们就想搭建自己的 namserver，并且需要上一级把相应的域名管理权交给自己，他们期望的结构如下：<br />![2021-06-02-10-03-19-751393.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599795800-25c636a3-a114-4c89-8c0f-caaa988c8b32.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u96655bc6&originHeight=708&originWidth=594&originalType=binary&size=25493&status=done&style=shadow&taskId=uba33d1a6-398c-4f9b-a6e0-8c615e5d7c3)<br />注意第一阶段和第二阶段的区别：第一阶段，A 部门想申请 a.hello.com 下的子域名，需要向上级申请，整个 a.hello.com 域的管理都在总公司；第二阶段，A 部门先自己搭建 nameserver，然后总公司把 a.hello.com 域管理权转交给自建的 nameserver， 这个转交管理权的行为，就叫子域授权<br />子域授权分两部操作:

1. A部门自建 nameserver，并且在 zone 配置文件中指定 a.hello.com 的权威解析服务器为自己的 nameserver 地址
2. 总公司在 nameserver 上增加一条 NS 记录， 把 a.hello.com 域授权给 A 部门的 nameserver

第一步在用 bind 搭建域名解析服务器里讲过， 只要在 zone 配置文件里指定SOA记录就好:
```bash
@       IN     SOA      ns.a.hello.com    admin.a.hello.com. (……)
```
第二步，在 hello.com 域的 nameserver 上添加一条NS记录:
```bash
a.hello.com      IN       NS       ns.a.hello.comns.a.hello.com      IN      A        xx.xx.xx.xx (自建nameserver的IP)
```
这样当解析 xx.a.hello.com 域名时，hello.com nameserver 发现配置中有 NS 记录，就会继续递归向下解析
<a name="eQht0"></a>
## DNS 调试工具
OPS 常用的 DNS 调试工具有：host，nslookup，dig<br />这三个命令都属于 bind-utils 包， 也就是 bind 工具集，它们的使用复杂度、功能 依次递增。关于它们的使用， man 手册和网上有太多教程，这里简单分析一下dig命令的输出吧：<br />![2021-06-02-10-03-19-873060.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622599808914-4a422897-285a-4799-b97a-7f5e8c3052ce.jpeg#clientId=ucb833c61-1632-4&from=ui&id=u1e163703&originHeight=630&originWidth=640&originalType=binary&size=67162&status=done&style=shadow&taskId=u569bacf6-344a-4633-a101-83b7204efb3)<br />dig 的参数非常多， 功能也很多，详细使用方法大家自行man吧
<a name="xpKNQ"></a>
## 其他
<a name="sZTcr"></a>
### DNS 放大攻击
DNS 放大攻击属于DoS攻击的一种，是通过大量流量占满目标机带宽， 使得目标机对正常用户的请求拒绝连接从而挂掉。
<a name="ZCrNW"></a>
#### 思路
正常的流量攻击，hack 机向目标机建立大量 request-response，但这样存在的问题是需要大量的 hack 机器。因为服务器一般的带宽远大于家用网络， 如果自己的家用机用来做 hack 机器，还没等目标机的带宽占满，我们的带宽早超载了。
<a name="zyWSD"></a>
#### 原理
DNS 递归解析的流程比较特殊， 可以通过几个字节的 query 请求，换来几百甚至几千字节的 resolving 应答（流量放大）， 并且大部分服务器不会对DNS服务器做防御。那么 hacker 们只要可以伪装 DNS query 包的 source IP， 从而让 DNS 服务器发送大量的 response 到目标机，就可以实现 DoS 攻击。<br />但一般常用的 DNS 服务器都会对攻击请求做过滤，所以找 DNS 服务器漏洞也是一个问题。
