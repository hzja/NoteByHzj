DNS<br />DNS 是互联网的基石之一，从公共 DNS 的必要性、利弊来讲一讲选择公共 DNS 需要关注的事情，以及列举一些当前主流的公共 DNS。<br />2020 年更新：新增「是否支持 TCP 查询」；阿里公共 DNS 已支持 DoH/DoT；腾讯 Public DNS+ 开始公测 DoH、DoT
<a name="Fm1HS"></a>
## 是否需要公共 DNS
在选择公共 DNS 之前，需要考虑一个问题：是否真的需要公共 DNS 么？<br />无论是 PPPoE 拨号上网，还是 DHCP 连接光猫上网，互联网服务提供商（ISP）都会下发两个 DNS 给你。为了方便介绍，在下文中称这两个 DNS 为 ISP DNS。在 DNS 的解析过程中，用户向递归 DNS 发起请求，递归 DNS 向权威 DNS 请求解析结果，可以说递归 DNS 起到一种转发的作用。运营商的 ISP DNS 就是递归 DNS；同时一些个人或互联网服务提供商也会架设自己的递归 DNS 开放给所有人使用，称为公共 DNS。<br />对于绝大部分人来说，运营商下发的 ISP DNS 应该是最准确的和最合适的，响应时间短、CDN 解析结果也会最准确。<br />为什么说运营商下发的 DNS 更快？请看下文「选择公共 DNS 时应该考虑什么」章节中关于「DNS 出口」的描述。<br />但是运营商不是做公益的。运营商经常使用 DNS 投毒来引导用户去使用他们的 缓存服务器，从而降低运营商带宽负载；或者劫持解析将用户引导去他们已经插入了广告的镜像站点，从而获利；或者为了国家相关法律政策要求或者运营商自己的需求屏蔽一些网站的访问（如辽宁联通曾将工信部举报站点的域名解析至 127.0.0.1）；或者自行篡改 TTL（DNS 结果缓存时间）降低 DNS 的负载，结果就是解析结果不能尽快更新；或者对于不正确的域名给你返回一个满是广告的页面，等等。即使运营商非常良心不使用 DNS 做坏事，也有可能因为设备没有及时扩容或者维护不善而导致不佳的体验。<br />ISP DNS 固然有这么多弊端，但是公共 DNS 也不是没有问题。下面两篇文章是曾就职于 CloudXNS 的 Ephen 大佬写的关于使用公共 DNS 上网的弊端，主要围绕公共 DNS 出口导致的 CDN 解析结果优化问题展开的。

- 使用公共 DNS 上网的弊端（一）
- 使用公共 DNS 上网的弊端（二）

如果你阅读完上述文章后，为了追求更安全、更准确的 DNS 解析结果而决定继续使用公共 DNS（比如和我一样 雾），你就可以继续阅读下去了。
<a name="KEE4x"></a>
## 选择公共 DNS 时应该考虑什么
公共 DNS 服务有很多，有大公司搭建的，有非盈利组织搭建的，还有个人搭建的，令人眼花缭乱。在选择的时候需要考虑很多方面才能选出适合需求的 DNS。通常在选择对于上网起非常重要作用的 DNS 时，需要考虑以下方面：

- SLA 服务在线率。DNS 是上网冲浪事时非常重要的一个环节，DNS 的可靠性直接影响到上网的体验；如果 DNS 宕机，那么很大一部分网站将无法访问
- 响应速度 在访问一个新的网站时，DNS 对这个网站的响应速度会直接影响到当前网站的直观加载速度
- 准确性 即使不考虑 DNS 污染和投毒，DNS 对网站访问的结果是否准确是非常重要的
- EDNS 简单地说，EDNS（正确的简称应该是 ECS）有助于帮助你获取最准确的 CDN 解析结果
- 其它特性 一些个人搭建的 DNS 会提供一些去广告或者特殊上网的功能
- DNS 出口 通常情况下，公共 DNS 的入口和出口不同。你可以通过 dig 查看你的 DNS 出口 IP：
```bash
$ dig whoami.akamai.net
```
DNS 出口对于 CDN 非常重要。公共 DNS 的本质上就是把你的查询请求转发给上游 DNS；在没有 EDNS 的情况下，CDN 的权威 DNS 会根据公共 DNS 使用的请求 IP（也就是 DNS 出口）来判定你的运营商、你所在的位置，从而返回距离你最近的节点 IP。<br />所以说，理论上 ISP 给你分配的 DNS 应该是最快的、也是 CDN 友好的。本文接下来提到的 CDN 优化、CDN 友好，也是指的 DNS 出口的 IP 能否让你访问到最快的 CDN 节点。<br />受篇幅限制，本文只介绍一些大型的、有名的公共 DNS，而一些类似服务不稳定的 1.1.9.9（由牙木运营）公共 DNS 和速度慢还不遵守 RFC 规范 的 ~~IBM Quad9 DNS~~、Level 3 DNS，亦或是只有单线单点部署的 360 公共 DNS、Verisign DNS，还有实际上没什么卵用的“安全 DNS” 如诺顿 DNS 和 OneDNS，本文就不介绍了。
<a name="Zr4Hk"></a>
## 中国国内一些公共 DNS 服务
<a name="i45l9"></a>
### 腾讯 DNSPod

- Anycast：上海、天津、广州、香港
- DNS 出口：上述四点
- TCP 查询：不支持
- DoT、DoH：支持。域名 dns.pub 和 doh.pub，同时支持 DoH 和 DoT
- ECS：不完整支持

[https://www.dnspod.cn/Products/Public.DNS](https://www.dnspod.cn/Products/Public.DNS)
```
119.29.29.29 119.28.28.28
```
这是 DNSPod 建立的公共 DNS，之后 DNSPod 被腾讯收购以后由腾讯云负责运营。腾讯 DNSPod 公共 DNS 配置了 Anycast，节点囊括了腾讯云所有可用区的节点（包括海外），所以速度还是不错的，并且除了支持 ECS 以外还有一些关于 DNS 出口选择优化的加成，所以 CDN 解析结果相对准确很多。但是 SLA 却并不优秀—— 曾经 经常遭遇 DDoS 攻击导致无法解析。除此以外，由于相对出名、使用人数较多，是运营商重点劫持的对象。<br />值得一提的是，DNSPod 的公共 DNS 是免费提供 HTTPDNS 的，Demo 可以看 这里<br />需要注意，DNSPod 的公共 DNS 不支持 cookie 查询。如果你在使用最新版 Bind 内置的 dig 工具、通过 @119.29.29.29 查询时，需要额外添加参数 +nocookie。
<a name="zx5RN"></a>
### 阿里云公共 DNS

- Anycast：成都、深圳、杭州
- DNS 出口：上述三点
- TCP 查询：不支持
- DoT、DoH：均支持。除了可以直接通过 IP 使用，还可以使用域名 dns.alidns.com。
- ECS：部分支持（DNS JSON API 支持 edns_client_subnet）

[http://www.alidns.com/](http://www.alidns.com/)
```
223.5.5.5 223.6.6.6
```
阿里建立的公共 DNS。和腾讯一样，阿里公共 DNS 也是搭建在自家的云服务——阿里云上。虽然也配置了 Anycast，不过不包括海外节点，国内也就浙江阿里云和深圳阿里云两个节点， 而且华北地区都是隧道穿透回深圳，响应速度略逊于 DNSPod 提供的公共 DNS；阿里公共 DNS 不支持 ECS ，有 DNS 出口的优化 ~~（一般都是广东出口）~~，。关于阿里的公共 DNS 没有听说太多宕机、无法使用相关的报告。~~但是倒是听说阿里公共 DNS 时常有返回 NXDOMAIN 影响使用体验。~~ NSDOMAIN 问题已得到一定缓解。
<a name="FTfWH"></a>
### 南京信风公共 DNS

- Anycast：南京、济南、芝加哥
- DNS 出口：未测出
- TCP 查询：不支持
- DoT、DoH：不支持
- ECS：不支持

[https://www.114dns.com/](https://www.114dns.com/)
```
114.114.114.114 114.114.115.115
```
无疑是国内最著名的公共 DNS，但是显然不是最好的。有 Anycast，国内有南京和青岛济南两个节点、海外有芝加哥节点，响应速度不敢恭维。国内最著名的公共 DNS、使用的人很多，SLA 非常可靠，因此也是运营商重点劫持的对象。但是考虑到南京信风为运营商旁路广告劫持提供技术和硬件支持，对他们提供的公共 DNS 服务还是戴着有色眼镜来看吧。
<a name="dc2yX"></a>
### ~~清华 TUNA 协会 666DNS~~

- Anycast：无，仅清华大学单点。
- DNS 出口：北京教育网
- TCP 查询：支持
- DoT、DoH：均支持。直接通过 IP 使用，没有绑定域名。
- ECS：不支持

[https://tuna.moe/help/dns/](https://tuna.moe/help/dns/)
```
101.6.6.6
```
由清华大学 TUNA 协会运营的公共 DNS，看他们说 ECS 还在测试，并没有上线（不过应该不会上线了）；由于是为清华大学校内搭建，所以也没有做 DNS 出口选择之类的优化，所以只适合北方 少数运营商 教育网的用户使用。TUNA 公共 DNS 的特点是准确（但是依然会受到 DNS 污染）、不会篡改解析、可以使用非常规端口（5353）查询。<br />近期赛尔网络对 CERNET 进行审查，要求限制 CERNET 以外的网络向网内的 53 端口发送流量，常规 DNS 查询已经不可用，理论上非常规端口的 DoT、DoH 不会受到影响。<br />另外，如果你当前使用的是高校教育网则不会受到影响，如果无法使用可以按照 这里 的指南申请清华大学为你所在高校院校的出口 IP 加白。
<a name="Ga3vZ"></a>
### CNNIC 公共 DNS

- Anycast：北京、杭州、香港
- DNS 出口：CNNIC 和阿里云
- TCP 查询：不支持
- DoT、DoH：不支持
- ECS：不支持

[http://www.sdns.cn/](http://www.sdns.cn/)
```
1.2.4.8 210.2.4.8
```
CNNIC 名声并不好（CNNIC Root CA 的故事），因此有些许人相对都有些对 CNNIC 的抵触心理。CNNIC 的公共 DNS（SDNS）国内仅双点部署、Anycast 配得一塌糊涂、速度堪忧，解析结果没有 CDN 优化（DNS 出口都是阿里云）。至于撇开 CNNIC 来谈 SDNS 推不推荐使用？SLA 比 DNSPod 的公共 DNS 还惨，解析请求时不时超时，嗯。
<a name="e90gt"></a>
### 百度公共 DNS

- Anycast：北京、南京、深圳
- DNS 出口：上述三点
- DoT、DoH：不支持
- TCP 查询：不支持
- ECS：不支持

[https://dudns.baidu.com/](https://dudns.baidu.com/)
```
180.76.76.76
```
百度的名声现在怕是比 CNNIC 还要臭得多，他们的公共 DNS 于 2017 年上线，现在也不被太多人知道，不过还是简单提两笔：百度也为公共 DNS 也配置了 Anycast，国内是单点百度北京数据中心北京、南京、深圳三点，海外是单点百度香港数据中心。用的人少，也许不容易被运营商劫持，有兴趣的可以试一试看。
<a name="owc15"></a>
## 国外常用 DNS 服务
你注意到没有？国内的 DNS 几乎没有支持 TCP 查询的，反而国外的 DNS 全都按照 RFC 规范支持 TCP 查询。
<a name="lWFXG"></a>
### Google Public DNS

- Any cast：Google 的 36 个数据中心（不包括 Google Global Cache）
- DNS 出口：Google 全球边缘网络
- TCP 查询：支持
- DoT、DoH：均支持。除了可以直接通过 IP 使用，还可以使用域名 dns.google
- ECS：支持（DNS JSON API 支持 edns_client_subnet）
```
8.8.8.8 8.8.4.4
```
[https://developers.google.com/speed/public-dns/](https://developers.google.com/speed/public-dns/)<br />最著名的公共 DNS（即使在国内也是很有名的），得益于 Google 庞大的全球网络设施（不过 Google 公共 DNS 并未使用 Google Global Cache，并且在非洲和大洋洲也没有节点），速度虽然不能说是最快的，但是至少不慢；支持 ECS、DoH、DoT，SLA 无限接近 100（Google 搜索引擎的 SLA 是 99.9999%），海外 CDN 都有针对 Google DNS 做优化，解析海外站点时强烈推荐。
<a name="azH4I"></a>
### OpenDNS

- Anycast：OpenDNS 的 32 个数据中心
- DNS 出口：未测出
- TCP 查询：支持
- DoT、DoH：不支持
- ECS：支持
```
208.67.222.222 208.67.220.220
```
[https://www.opendns.com/](https://www.opendns.com/)<br />被 Cisco 收购的 OpenDNS 一度是世界上最快的公共 DNS——OpenDNS 在全球拥有 30 余节点并且 Anycast 配的很棒。支持 ECS 和 SLA 达到 100，而且 OpenDNS 开放非常规端口 5353 查询和 TCP 查询，即使从国内直接请求也不容易被污染和劫持。如果你在使用 ChinaDNS 这类工具同时又没有专门为其准备一条加密隧道，那么直连 OpenDNS 的 5353 就是一个不错的替代选择。
<a name="rpOHA"></a>
### Cloudflare DNS

- Anycast：Cloudflare 的 160+ 个数据中心
- DNS 出口：Cloudflare 的 160+ 个数据中心
- TCP 查询：支持
- DoT、DoH：均支持，可以使用域名 one.one.one.one 也可以直接使用 IP
- ECS：不支持（由于 ECS 需要基于用户 IP 查询，和 Cloudflare DNS 保护隐私相违背）
```
1.0.0.1 1.1.1.1
```
[https://1.1.1.1](https://1.1.1.1/) [https://1.0.0.1](https://1.0.0.1/)<br />当 Cloudflare 从 APNIC 手上接过 1.0.0.0/24 和 1.1.1.0/24 并架设了公共 DNS 以后，得益于 Cloudflare 全球 160+ 数据中心（Cloudflare 拥有 185+ 数据中心，但其公共 DNS 并没有部署在百度云加速的节点上）、BGP Anycast 和 Cloudflare Argo 等技术，成功超越 OpenDNS 成为了世界上最快的公共 DNS（数据来自 DNSPerf），还支持 DoT、DoH 等常见加密解析方案。由于其隐私政策，Cloudflare 公共 DNS 不记录用户 IP，意味着无法使用 ECS 等技术，不过仗着节点数量众多、DNS 出口覆盖全球各大区域，也适合作为主力 DNS。
<a name="E4PSV"></a>
### Freenom World

- Anycast：是
- DNS 出口：Freenom 的 Transit 的 IP
- TCP 查询：支持
- DoT、DoH：不支持
- ECS：支持
```
80.80.80.80 80.80.81.81
```
[https://www.freenom.world/zh/index.html?lang=zh](https://www.freenom.world/zh/index.html?lang=zh)<br />注册过后缀如 .cf .ga .ml 的免费域名的，对 Freenom 这个名字一定不会陌生。这家荷兰域名注册商借助自己旗下的云服务资源也运营了一家公共 DNS 服务，卖点是隐私和安全。支持 ECS，测到的 DNS 出口都是落地 IP。部署了 Anycast 但是节点不多，响应速度并不算拔尖。Freenom 的公共 DNS 的一个特点其实是电信走 163 去 HK，联通移动走 IIJ 去 JP，因此如果你所在的地区和 ISP 对国外递归 DNS 的污染并不严重的话可以尝试使用 Freenom 的 DNS。
<a name="xz4Hw"></a>
### DNS.sb

- Anycast：是
- DNS 出口：SB Network 的上游的 IP
- TCP 查询：支持
- DoT、DoH：支持，可以使用 IP 或 dns.sb 域名
- ECS：不支持
```
185.222.222.222 185.184.222.222
```
[https://dns.sb](https://dns.sb/)<br />和 Cloudflare 一样支持主流的 DoT、DoH 等加密 DNS 解析。启用了 Anycast，节点可能没有 Cloudflare 那么多，不过还是覆盖了大部分地区。
<a name="Hbgke"></a>
## 公共 DNS 最佳实践
综上所述，如果需要选择公共 DNS 的话同时需要获得尽可能友好的 CDN 解析结果，解析国内站点推荐使用阿里 DNS 和腾讯 DNSPod（注重稳定请用前者、注重准确请用后者）；解析海外网站推荐主 DNS 1.0.0.1 或 208.67.222.222，备 DNS 8.8.8.8 或 8.8.4.4。
