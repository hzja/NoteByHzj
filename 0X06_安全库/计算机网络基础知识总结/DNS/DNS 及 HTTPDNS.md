DNS
<a name="yly9z"></a>
## 说明
移动场景下DNS解析开销是整个网络请求中不可忽略的一部分。在弱网环境下，基于UDP的LocalDNS解析非常容易出现解析超时的问题，并且即使解析成功会消耗数百毫秒乃至更甚，对整个业务请求而言是非常不利的，它直接影响了客户的体验。<br />对于一个比较大众的应用而言，DNS的优化对整个应用的网络优化所占的权重是很大的。接下来从以下几个方面全面理解DNS，对开发中的网络优化会有不小的帮助。
<a name="EJjen"></a>
## 1、DNS
<a name="F2HdK"></a>
### 1.1 认识DNS
DNS(Domain Name System)是域名“系统”的英文缩写，是一种组织成域层次结构的计算机和网络服务命名系统，它用于TCP/IP网络，它所提供的服务是用来将主机名和域名转换为IP地址的工作。<br />作为网络通信的最靠前的一个环节，其在整个网络通信的过程中的重要性不言而喻。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360628314-ecd1be56-4a61-466b-965f-fa4314643882.webp#clientId=u2994236a-76ea-4&from=paste&id=uc88a2ecf&originHeight=326&originWidth=515&originalType=url&ratio=1&status=done&style=shadow&taskId=u89af25e4-4b0b-4420-8abe-d7bf3a2c28a)<br />ios10之后，apple提供的原生http请求方法能返回http请求各个阶段的时间指标，其中就包含DNS解析时间。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360628293-c2c562a1-f80a-4ec1-afa2-ea99104641d5.webp#clientId=u2994236a-76ea-4&from=paste&id=u06fb9b27&originHeight=266&originWidth=876&originalType=url&ratio=1&status=done&style=shadow&taskId=ubfd0bc00-0b25-4d9e-b3d8-3ad46911182)
<a name="SHg1T"></a>
### 1.2 DNS解析相关概念
<a name="EMIcY"></a>
#### 1.2.1 DNS域名层次结构
DNS是一种分层结构，在整个互联网中组成一个树状系统，顶层是系统的根域名，下层为TLD以及二级域名，叶子就构成了所谓的FQDN（Fully Qualified Domain Names），根域名通常使用"."来表示，其实际上也是由域名组成，全世界目前有13组域名根节点，由少数几个国家进行管理，而国内仅有几台根节点镜像。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360628311-063d8bdb-db24-49ef-9fab-db4d2c1ddfea.webp#clientId=u2994236a-76ea-4&from=paste&id=u713cb53c&originHeight=311&originWidth=864&originalType=url&ratio=1&status=done&style=shadow&taskId=u60a0a15d-a325-411b-845e-c3c7e8a6279)
<a name="A0Rvb"></a>
#### 1.2.2 权威DNS
权威DNS是经过上一级授权对域名进行解析的服务器，同时它可以把解析授权转授给其他人，如COM顶级服务器可以授权xxorg.com这个域名的的权威服务器为NS.ABC.COM，同时NS.ABC.COM还可以把授权转授给NS.DDD.COM，这样NS.DDD.COM就成了ABC.COM实际上的权威服务器了。平时解析域名的结果都源自权威DNS。eg: 阿里云云解析  [[https://wanwang.aliyun.com/domain/dns](https://wanwang.aliyun.com/domain/dns)]([https://wanwang.aliyun.com/domain/dns](https://wanwang.aliyun.com/domain/dns))
<a name="TY4PT"></a>
#### 1.2.3 递归DNS
递归DNS又称为Local DNS，它没有域名解析结果的决定权，但代理了用户向权威DNS获取域名解析结果的过程。递归DNS上有缓存模块，当目标域名存在缓存解析结果并且TTL未过期时（每个域名都有TTL时间，即有效生存时间，若域名解析结果缓存的时间超过TTL，需要重新向权威DNS获取解析结果），递归DNS会返回缓存结果，否则，递归DNS会一级一级地查询各个层级域名的权威DNS直至获取最终完整域名的解析结果。eg: 自己的电脑，运营商提供的dns服务器等等。
<a name="zohQP"></a>
#### 1.2.4 公共DNS
公共DNS是递归DNS的一种特例，它是一种全网开放的递归DNS服务，而传统的递归DNS信息一般由运营商分发给用户。<br />在 DNS 的解析过程中，用户向递归 DNS 发起请求，递归 DNS 向权威 DNS 请求解析结果，可以说递归 DNS 起到一种转发的作用。ISP DNS 就是递归 DNS；同时一些个人或互联网服务提供商也会架设自己的递归 DNS 开放给所有人使用，称为公共 DNS。

| 运营商 | anycast | 官网 | DNS |
| --- | --- | --- | --- |
| 南京信风公共DNS | 南京、济南、芝加哥 | www.114dns.com | 114.114.114..114  114.114.115.115 |
| 阿里云公共DNS | 成都、深圳、杭州 | http://www.alidns.com | 233.5.5.5  233.6.6.6 |
| Google Public DNS | Google 36个数据中心 | developers.google.com/speed/publi… | 8.8.8.8  8.8.4.4 |

全国DNS汇总:  www.114dns.com/DNS_List.ht…  ipip: tools.ipip.net/dns.php<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360628493-f428ad34-da73-45e1-abd8-37b1041ad802.webp#clientId=u2994236a-76ea-4&from=paste&id=ubdf67223&originHeight=341&originWidth=917&originalType=url&ratio=1&status=done&style=shadow&taskId=ua1241861-dc02-4814-86ff-aebdb148381)<br />图片
<a name="fkyTZ"></a>
#### 1.2.5 转发DNS
可以理解为递归DNS和用户之间的一个中转站，它不提供直接解析域名的服务，它将请求转发给递归DNS，然后将递归DNS的结果转发一下，也提供缓存作用。比如，日常家用的路由器，它的DNS服务器一般都是192.168.1.1，只是转发给递归DNS。
<a name="EEdKG"></a>
### 1.3 域名解析记录方式
域名解析记录主要分为A记录、MIX记录、CNAME记录、NS记录和TXT记录：

- A记录：A代表Address,用来指定域名对应的ip,例如将www.hello.com指定到 113.112.3.xxx, A记录可以将多个域名解析到一个ip地址，但是不能讲一个域名解析到多个ip地址
- MIX记录：Mail Exchange，就是可以将某个域名下的邮件服务器直线给自己的Mail Server
- CNAME记录：Canonical Name，即别名解析。所谓别名解析就是可以为一个域名设置一个或者多个别名，如将aaa.com解析到bbb.net、将ccc.com也解析到bbb.net，其中bbb.net分别是aaa.com和ccc.com的别名
- NS记录：为某个域名指定DNS解析服务器，也就是这个域名由指定的IP地址的DNS服务器解析
- TXT记录：为某个主机名或域名设置说明，如可以为ucloud.cn是指TXT记录为“中立安全科信赖”这样的说明
<a name="GLBkC"></a>
### 1.4 域名解析过程
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360628561-3aaa9b5f-b858-4ff2-8a53-52b403621e09.webp#clientId=u2994236a-76ea-4&from=paste&id=u43f2a60b&originHeight=789&originWidth=1017&originalType=url&ratio=1&status=done&style=none&taskId=u94885e83-45fd-4fb6-8d23-60fa125bec7)<br />以下是在终端中dig百度显示的具体过程：
```bash
macdeiMac:~ ethan$ dig +trace www.baidu.com

; <<>> DiG 9.10.6 <<>> +trace www.baidu.com
;; global options: +cmd
.   1615 IN NS a.root-servers.net.
.   1615 IN NS g.root-servers.net.
.   1615 IN NS f.root-servers.net.
.   1615 IN NS l.root-servers.net.
.   1615 IN NS m.root-servers.net.
.   1615 IN NS j.root-servers.net.
.   1615 IN NS k.root-servers.net.
.   1615 IN NS c.root-servers.net.
.   1615 IN NS b.root-servers.net.
.   1615 IN NS d.root-servers.net.
.   1615 IN NS i.root-servers.net.
.   1615 IN NS e.root-servers.net.
.   1615 IN NS h.root-servers.net.
;; Received 239 bytes from 114.114.114.114#53(114.114.114.114) in 10 ms

com.   172800 IN NS a.gtld-servers.net.
com.   172800 IN NS b.gtld-servers.net.
com.   172800 IN NS c.gtld-servers.net.
com.   172800 IN NS d.gtld-servers.net.
com.   172800 IN NS e.gtld-servers.net.
com.   172800 IN NS f.gtld-servers.net.
com.   172800 IN NS g.gtld-servers.net.
com.   172800 IN NS h.gtld-servers.net.
com.   172800 IN NS i.gtld-servers.net.
com.   172800 IN NS j.gtld-servers.net.
com.   172800 IN NS k.gtld-servers.net.
com.   172800 IN NS l.gtld-servers.net.
com.   172800 IN NS m.gtld-servers.net.
com.   86400 IN DS 30909 8 2 E2D3C916F6DEEAC73294E8268FB5885044A833FC5459588F4A9184CF C41A5766
com.   86400 IN RRSIG DS 8 1 86400 20191112050000 20191030040000 22545 . Sr1g7h+DSqi+ekBQQS2ZBc/jt0zL+IR+Od/R9TnMjcy8Mw9RxLrMY2pm 1VYNqL5cAME1stSAfRUKwjD/vixnCeVLoJ6idCFOZeB+t/tTFQF/jfk1 td66pW9V/WgLIvslAwEZjidVeUFYERc7hZXr10BgzryZthizHISimuiQ qBjoIQN/uYULTnmePkIW07mnJXc9/AVZrjeI1AmvYC7wE0uR7DWNg1Ig dL4DaLDOM30qN7FBAD7K091uEgctpdxd/8G5XoYclSqroN4G6RibvkWT /vPCFRUzoaxembT5tR7gIz7gxdhN1r8NBD468JTG180MNUvb16Z/87U6 7UkMrg==
;; Received 1173 bytes from 192.58.128.30#53(j.root-servers.net) in 77 ms

baidu.com.  172800 IN NS ns2.baidu.com.
baidu.com.  172800 IN NS ns3.baidu.com.
baidu.com.  172800 IN NS ns4.baidu.com.
baidu.com.  172800 IN NS ns1.baidu.com.
baidu.com.  172800 IN NS ns7.baidu.com.
CK0POJMG874LJREF7EFN8430QVIT8BSM.com. 86400 IN NSEC3 1 1 0 - CK0Q1GIN43N1ARRC9OSM6QPQR81H5M9A  NS SOA RRSIG DNSKEY NSEC3PARAM
CK0POJMG874LJREF7EFN8430QVIT8BSM.com. 86400 IN RRSIG NSEC3 8 2 86400 20191103044815 20191027033815 12163 com. U/FwNWeuKzR/uT2X/8Cf9TQmnaMdWf6XwBrFIIOCQ/kfKaOExEiT8LSQ 13OaEjtvFOOlIPK0XIbsL+dgGPYb/UV6sipBeQ1n8KuK18m3bYk47Ely oe+3VVp0zaiXt9DZrmRRenBB13o0DPqCbRHAHq1pj5zG5VkMufu9L/TT g80XlNukAMcu4GrV4VP8OimOQxz7HJbadci2oYn3beiHqQ==
HPVV2B5N85O7HJJRB7690IB5UVF9O9UA.com. 86400 IN NSEC3 1 1 0 - HPVVN3Q5E5GOQP2QFE2LEM4SVB9C0SJ6  NS DS RRSIG
HPVV2B5N85O7HJJRB7690IB5UVF9O9UA.com. 86400 IN RRSIG NSEC3 8 2 86400 20191105055359 20191029034359 12163 com. J5Dq0lGkcejjg1vPqWDBvNYaAhqFF3Ck8trKj4tgW5Z1bmoXsHGU6/Cl y3GlLfzb49xjiXzxVLCuAQJ9uLuKSX5cn+kesc8rwYqcVXU4nXbD5jzo u3CK2yHD3FqPDCOKlMSNy3KKkL03bB3DfmvAae/qQs7xSe6VTpCkR6v/ lo3UA/pMfTYBjSIOvR2KmpM9yFLmN5LXAQW3rNH8sW91BA==
;; Received 761 bytes from 192.26.92.30#53(c.gtld-servers.net) in 241 ms

www.baidu.com.  1200 IN CNAME www.a.shifen.com.
a.shifen.com.  1200 IN NS ns2.a.shifen.com.
a.shifen.com.  1200 IN NS ns3.a.shifen.com.
a.shifen.com.  1200 IN NS ns4.a.shifen.com.
a.shifen.com.  1200 IN NS ns5.a.shifen.com.
a.shifen.com.  1200 IN NS ns1.a.shifen.com.
```
;; Received 239 bytes from 180.76.76.92#53(ns7.baidu.com) in 10 ms<br />通过上面命令可以看到DNS解析的逐步过程，其中最后一步可以看到www.baidu.com 被CNAME到 www.a.shifen.com 所以再查一下 www.a.shifen.com 即可看到其ip.
```bash
;; Received 215 bytes from 220.181.33.31#53(ns2.baidu.com) in 28 ms

www.a.shifen.com. 300 IN A 180.101.49.12
www.a.shifen.com. 300 IN A 180.101.49.11
a.shifen.com.  1200 IN NS ns4.a.shifen.com.
a.shifen.com.  1200 IN NS ns5.a.shifen.com.
a.shifen.com.  1200 IN NS ns1.a.shifen.com.
a.shifen.com.  1200 IN NS ns2.a.shifen.com.
a.shifen.com.  1200 IN NS ns3.a.shifen.com.; Received 236 bytes from 180.76.76.95#53(ns5.a.shifen.com) in 9 ms
```
然后再用`nslookup`命令查看一下百度的ip是不是上面显示的两个:
```bash
macdeiMac:~ ethan$ nslookup www.baidu.com
Server:  114.114.114.114
Address: 114.114.114.114#53

Non-authoritative answer:
www.baidu.com canonical name = www.a.shifen.com.
Name: www.a.shifen.com
Address: 180.101.49.11
Name: www.a.shifen.com
Address: 180.101.49.12
```
图示DNS解析baidu的过程：

1. 终端向 Local DNS发起域名解析请求
2. Local DNS在获取到域名请求后，首先从Root hins获取根域名服务器的地址(Root hints包含了互联网DNS根服务器的地址信息)
3. 获取到了根域名服务器地址后，Local DNS向根域名服务器发起DNS解析请求，根域名服务器返回顶级域名服务器地址(com或者cn或者其它)
4. 随后Local DNS向com域名服务器发起解析请求，并得到baidu.com二级域名服务器地址
5. Local DNS向baidu.com二级域名服务器发起解析请求，并最终货到了www.baidu.com 的ip地址信息
6. Local DNS将递归查询获得的IP地址信息缓存并返回给客户端
<a name="yAdLq"></a>
## 2、全局负载均衡GSLB
GSLB是Global Server load Blance的缩写，即全局负载均衡。目的是实现互联网上不同地域的服务器间的流量调配，保证使用户的请求能被离用户最近或者服务质量更好的服务器来处理。从而确保服务质量。<br />能通过判断服务器的负载，包括CPU占用、带宽占用等数据，决定服务器的可用性，同时能判断用户（访问者）与服务器间的链路状况，选择链路状况最好的服务器。因此GSLB是对服务器和链路进行综合判断来决定由哪个地点的服务器来提供服务，实现异地服务器群服务质量的保证。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360630153-9a687289-ac1f-4b7e-8351-fecb9e695e55.webp#clientId=u2994236a-76ea-4&from=paste&id=uc966e7e1&originHeight=417&originWidth=511&originalType=url&ratio=1&status=done&style=shadow&taskId=u5f81e42e-e434-4e95-b1d2-585bf64f5a9)
<a name="uK1TY"></a>
### 2.1  智能DNS
智能DNS是GSLB的一种应用。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360630336-bcd32f0e-787c-49b9-8b41-63f7a1729843.webp#clientId=u2994236a-76ea-4&from=paste&id=u63a10517&originHeight=731&originWidth=816&originalType=url&ratio=1&status=done&style=shadow&taskId=u274233de-9992-45e4-bbf1-c916bb03f03)
<a name="lHcDv"></a>
## 3、DNS解析存在的问题
有时候在访问百度或者在应用中发出一个http请求时，如果DNS解析被劫持，可能最终访问到的不是想要访问的服务器。
<a name="Wbt0R"></a>
### 3.1 运营商劫持
DNS劫持 就是通过劫持了 DNS 服务器，通过某些手段取得某域名的解析记录控制权，进而修改此域名的解析结果，导致对该域名的访问由原 IP 地址转入到修改后的指定 IP，其结果就是对特定的网址不能访问或访问的是假网址，从而实现窃取资料或者破坏原有正常服务的目的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360630409-b1980c10-e496-494b-b340-9e0c65b2dd12.webp#clientId=u2994236a-76ea-4&from=paste&id=u3cb8ffe3&originHeight=439&originWidth=791&originalType=url&ratio=1&status=done&style=shadow&taskId=uf062fbe8-7f3c-4263-9243-5a40eec305f)
<a name="dIVXE"></a>
### 3.2 DNS解析域名时缓存解析结果
在开发中有时候会遇到这样的情况：一个联通用户，在手机浏览器中输入baidu.com，由一个LocalDNS服务器像百度权威服务器查应该访问哪一台服务器，权威把结果返回给LocalDNS服务器,localDNS服务器返回结果给用户。<br />如果当LocalDNS缓存的有baidu.com对应的结果，那么他就不会像百度的权威服务器查询其对应的ip，而是直接返回缓存中的结果。如果此时权威服务器中的baidu.com对应的ip发生了变化，LocalDNS没有及时更新，这样会导致用户访问不到服务器。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360630444-be7ab09a-db56-4ff4-8961-8592a0e6c9ca.webp#clientId=u2994236a-76ea-4&from=paste&id=ua7a6eed5&originHeight=291&originWidth=908&originalType=url&ratio=1&status=done&style=shadow&taskId=u8e94890d-392a-4532-aaed-1601f31db71)
<a name="YnfT7"></a>
### 3.3 转发解析
用手机访问baidu.com时，会到当前运营商的DNS服务器查，然后运营商的DNS服务再去百度权威服务器去查，最终把权威服务器中的正确ip返回。<br />上面是正常的情况，但是如果当前运营商(比如联通)的LocalDNS不访问百度权威DNS服务器，而是直接访问了其它运营商(比如电信)的LocalDNS服务器，有些小的运营商就是通过这样做来降低成本。如果电信的LocalDNS对非自家ip的访问限了速那么很明显会影响DNS解析时间。如果应用中的某些服务需要运营商信息isp(eg:只能dns, cdn等服务).<br />下面截图手机的网路环境(NetPing开源地址：github.com/mediaios/ne…)<br />![2021-07-15-22-50-41-759616.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1626360913161-ae714d4a-0b95-4176-a1a8-aa3fa03582c8.jpeg#clientId=u2994236a-76ea-4&from=ui&id=u04ca103d&originHeight=882&originWidth=437&originalType=binary&ratio=1&size=31917&status=done&style=shadow&taskId=u7c30d6ae-ec4e-4227-a001-fa46e61700f)<br />直接ping百度的地址，然后用wireshark抓包结果，正常结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1626360630893-719e6be3-0e54-432b-904a-135470bd4a6e.webp#clientId=u2994236a-76ea-4&from=paste&id=u70dbf27e&originHeight=534&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uc93c0aca-fa56-46e5-aa59-f6415116ccd)<br />如果发生了转发则逻辑如下：<br />![2021-07-15-22-50-41-949651.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1626360928830-4cbfa25a-841a-4ee9-997c-96c15545c9bf.jpeg#clientId=u2994236a-76ea-4&from=ui&id=u1dc62fa5&originHeight=167&originWidth=895&originalType=binary&ratio=1&size=29245&status=done&style=none&taskId=ud005e13f-b610-4d1d-bc27-21beeb4e1d9)
<a name="cIrAT"></a>
## 4、HTTPDNS
<a name="jyRC1"></a>
### 4.1 什么是HTTPDNS
HTTPDNS使用HTTP与DNS服务器交互，代替传统的基于UDP的DNS协议，域名解析请求直接发送到HTTPDNS服务端，从而绕过运营商的Local DNS<br />![2021-07-15-22-50-42-044618.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1626361119660-e82deded-79d7-4e33-91d6-2db644fb9084.jpeg#clientId=u2994236a-76ea-4&from=ui&id=u87c27d65&originHeight=273&originWidth=749&originalType=binary&ratio=1&size=30900&status=done&style=shadow&taskId=u399cbc0f-85ed-44f4-a2e5-a6559212671)![2021-07-15-22-50-42-174615.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1626361119696-0db3c8f7-02f7-4d81-97ea-f85511477180.jpeg#clientId=u2994236a-76ea-4&from=ui&id=ub9f10179&originHeight=649&originWidth=620&originalType=binary&ratio=1&size=58017&status=done&style=shadow&taskId=u1ec78dfe-cc7b-4308-8ce8-35e6612a679)
<a name="XmtfR"></a>
### 4.2 HTTPDNS的特性
<a name="rsDfU"></a>
#### 4.2.1 防止域名劫持
由于 HttpDns 是通过 IP 直接请求 HTTP 获取服务器 A 记录地址，不存在向本地运营商询问 domain 解析过程，所以从根本避免了劫持问题。
<a name="mOdbD"></a>
#### 4.2.2 精准调度
HTTPDNS能够直接获取到用户的IP地址，从而实现精确定位与导流
<a name="gcsgc"></a>
#### 4.2.3 用户连接失败率下降
通过算法降低以往失败率过高的服务器排序，通过时间近期访问过的数据提高服务器排序，通过历史访问成功记录提高服务器排序。<br />原来的请求地址为 “apis.juhe.cn/simpleWeath…”，通过HTTPDNS替换域名后最终的结果如下：<br />![2021-07-15-22-50-42-290616.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1626360948977-1213fb09-8fe1-4afc-b002-f5f4e9899ca1.jpeg#clientId=u2994236a-76ea-4&from=ui&id=uacbb1316&originHeight=921&originWidth=410&originalType=binary&ratio=1&size=58941&status=done&style=none&taskId=ufbb1faae-39ee-4bbb-9303-ce09b892b8c)![2021-07-15-22-50-42-417618.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1626360948996-c02bb4fc-217a-413e-8cbe-7a9c4aa2e3b6.jpeg#clientId=u2994236a-76ea-4&from=ui&id=u1ed8dabd&originHeight=925&originWidth=417&originalType=binary&ratio=1&size=61011&status=done&style=shadow&taskId=u489d57b8-c169-452b-9df7-c9c103b058e)
<a name="kINvg"></a>
### 4.2 HTTPS IP Content
发送HTTPS请求首先要进行SSL/TLS握手，握手过程大致如下：

1. 客户端发起握手请求，携带随机数、支持算法列表等参数。
2. 服务端收到请求，选择合适的算法，下发公钥证书和随机数。
3. 客户端对服务端证书进行校验，并发送随机数信息，该信息使用公钥加密。
4. 服务端通过私钥获取随机数信息。
5. 双方根据以上交互的信息生成session ticket，用作该连接后续数据传输的加密密钥。

上述过程中，和HTTPDNS有关的是第3步，客户端需要验证服务端下发的证书，验证过程有以下两个要点：

1. 客户端用本地保存的根证书解开证书链，确认服务端下发的证书是由可信任的机构颁发的。
2. 客户端需要检查证书的domain域和扩展域，看是否包含本次请求的host。

如果上述两点都校验通过，就证明当前的服务端是可信任的，否则就是不可信任，应当中断当前连接。<br />当客户端使用HTTPDNS解析域名时，请求URL中的host会被替换成HTTPDNS解析出来的IP，所以在证书验证的第2步，会出现domain不匹配的情况，导致SSL/TLS握手不成功。
<a name="BDJ1h"></a>
## 5、问题
<a name="Cg0lj"></a>
### 5.1 主机是如何知道DNS服务器地的IP地址的？
通过DHCP动态获得，或者手工配置的。<br />DHCP协议：DHCP（Dynamic Host Configuration Protocol，动态主机配置协议）通常被应用在大型的局域网络环境中，主要作用是集中的管理、分配IP地址，使网络环境中的主机动态的获得IP地址、Gateway地址、DNS服务器地址等信息，并能够提升地址的使用率。
<a name="D88f8"></a>
### 5.1 为什么DNS采用UDP协议 ？
TCP通信过程太复杂并且开销大，一次TCP交换需要9个包：三个连接包，四个断开包，一个request包，一个响应包。<br />UDP通信过程简单，只需要一个查询包和一个响应包。
