在利用网络空间搜索引擎获取资产并进行漏洞检测时，会发现很多 IP 站存在漏洞，但是又无法直接证明 IP 的归属，通常会通过 IP 反查域名，然后通过域名的备案记录来确定归属，那么，如何通过 IP 反查域名呢？<br />从域名到 IP 的转换，通过 DNS 服务可以直接查询到，属于正向解析，但是从 IP 到域名，DNS 服务器并没有提供这项服务，属于不可逆查询，所以才有了今天的分享。
<a name="ToRsq"></a>
## 0x01、第三方平台反查
互联网上有很多平台通过积累自己的 DNS 记录库来提供反查域名的服务，有付费平台也有免费平台，这里分享几个已经集成到自己脚本中的几个平台：
<a name="TNiX7"></a>
### 1、webscan.cc
[http://api.webscan.cc/?action=query&ip=47.94.225.171](http://api.webscan.cc/?action=query&ip=47.94.225.171)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138332767-ac6dd2e5-14e2-4ec5-b3da-afa701363489.png#averageHue=%231a1a1a&clientId=u05592554-4249-4&from=paste&id=ub0ccd427&originHeight=393&originWidth=1036&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue0b86faa-cfff-4e4a-828f-3fda9494154&title=)
<a name="kXKIS"></a>
### 2、ip138
[https://site.ip138.com/47.94.225.171/](https://site.ip138.com/47.94.225.171/)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138332738-21158738-c46c-4db4-85e9-86d79de58741.png#averageHue=%23f9f8f7&clientId=u05592554-4249-4&from=paste&id=u3e0e61e5&originHeight=595&originWidth=682&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2919fab9-a39f-4e59-885a-ca348813e42&title=)
<a name="nS6DP"></a>
### 3、rappiddns
[https://rapiddns.io/sameip/](https://rapiddns.io/sameip/)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138332724-9714d2ef-255c-4fbe-8d6f-d005c63a354a.png#averageHue=%23cee6f0&clientId=u05592554-4249-4&from=paste&id=u0f72ed7a&originHeight=345&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua8e93818-eb05-4927-968c-a07b70a3dd8&title=)
<a name="w3lYk"></a>
### 4、fofa
[https://fofa.info/result?qbase64=aXA9IjQ3Ljk0LjIyNS4xNzEiICYmIGRvbWFpbiAhPSAiIg%3D%3D](https://fofa.info/result?qbase64=aXA9IjQ3Ljk0LjIyNS4xNzEiICYmIGRvbWFpbiAhPSAiIg%3D%3D)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138332812-ef08ffb4-88b8-4d13-8796-e53a70f6c8ad.png#averageHue=%23081824&clientId=u05592554-4249-4&from=paste&id=u96202114&originHeight=646&originWidth=1010&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud11dfd11-dae7-4741-af2c-7f5163b9c42&title=)
<a name="LRZBY"></a>
### 5、hunter
[https://hunter.qianxin.com/list?search=ip%3D%2247.94.225.171%22%26%26domain!%3D%22%22&conditions=](https://hunter.qianxin.com/list?search=ip%3D%2247.94.225.171%22%26%26domain!%3D%22%22&conditions=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138333471-7093976e-b2af-4c0e-93f3-f7966470c14b.png#averageHue=%23f5f6f8&clientId=u05592554-4249-4&from=paste&id=ub0b00965&originHeight=275&originWidth=951&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec5d392b-59ce-4e2d-acef-87fd52c0991&title=)<br />当然，还有很多其他的平台，这里推荐一个集成大量第三方平台的开源工具，模仿 subfinder 开发：<br />[https://github.com/Lengso/iplookup](https://github.com/Lengso/iplookup)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138333570-f9c6a08f-b91c-4ac6-8a85-d6a30f8af969.png#averageHue=%230f141a&clientId=u05592554-4249-4&from=paste&id=u65827cec&originHeight=645&originWidth=668&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4f7e64e9-ff22-462b-8887-d6e20185924&title=)
<a name="NsSOc"></a>
## 0x02、基于备案号
备案号？哪里来的备案号？国内网站对外提供服务均需要备案，且需要在页面中明确写出备案号，所以当发现一个存在漏洞的 IP 站时，不妨试试，查看主页下是否存在备案号，不过这个不一定准确，因为站长伪造是一件非常容易的事儿。比如：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138333784-e85083d1-e8b8-4859-8173-f64cd1661ae8.png#averageHue=%23f5f5f5&clientId=u05592554-4249-4&from=paste&id=udde64148&originHeight=127&originWidth=587&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue56dad25-2812-4d60-a369-053aa1774e9&title=)<br />有了备案号之后，还要进一步确认，可以通过备案号进行反查注册的域名，这里可以使用多个平台，比如：<br />[https://icplishi.com/](https://icplishi.com/)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138334053-54f2e35a-cabd-4872-9ada-53e5472c3d18.png#averageHue=%23e6dc95&clientId=u05592554-4249-4&from=paste&id=u708958f9&originHeight=731&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4fa3826f-a2bc-4859-93d0-9968bb93d65&title=)<br />有了主域名，可以组合一些常见字典，进行 dns 枚举，尝试发现目标 IP 所对应的域名是什么。
<a name="riG2t"></a>
## 0x03、证书信息
你所发现的 IP 站不一定是 443 端口，可能并没有绑定证书，但是该 IP 可能不仅仅只开放一个 80 端口，所以可以通过端口扫描的方式，探测常见的证书端口，如果存在，则获取该证书信息，从而提取证书中绑定的域名，也是一种证明归属的方式。比如：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138334022-2c6fb93c-f872-4e98-b42e-c28e1ef6a923.png#averageHue=%23f5f5f5&clientId=u05592554-4249-4&from=paste&id=u6d734e36&originHeight=407&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u441f0b1d-dd98-4055-8bdd-f99cf4a2995&title=)
<a name="QGx1I"></a>
## 0x04、数据汇总
将之前提到的几种方式获取到的主域名和子域名进行整合，先对子域名进行解析比对，是否有包含目标 IP 的域名存在，如果不存在，则需要将所有主域名进行整合，结合自定义的子域名字典，进行 DNS 枚举，从而发现该 IP 的真实域名。
<a name="kAWZc"></a>
## 0x05、总结
基于以上技术，已经实现了一键查询脚本，在查询到数据之后会进行入库，下次再次查询将不再重新获取，也可以通过域名枚举的方式扩充本地数据库，从而让 IP 反查域名的效率翻倍，最终成果如图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701138334449-354551d9-8b7c-4cf8-9a27-8bb1f05c170a.png#averageHue=%23161413&clientId=u05592554-4249-4&from=paste&id=u2120f9d4&originHeight=461&originWidth=935&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf6ab9ba-9796-4e93-ab6d-fc0dcffe3ce&title=)
