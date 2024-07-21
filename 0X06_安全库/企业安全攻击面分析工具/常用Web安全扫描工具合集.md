Web<br />从扫描方式来说，最传统的一种方式就是基于爬虫的漏洞扫描，通过爬虫收集网站的所有链接及其参数请求，发送Payload进行漏洞探测。对于一些涉及逻辑判断，爬虫无法抓取的页面，则可以通过被动代理扫描，基于被动代理的漏洞扫描让扫描器成为了指哪打哪的利器。另外，但是有一些API或孤页，通过爬虫和人工点击是一一获取到的，这就需要基于日志/流量分析的漏洞扫描来解决这个问题。
<a name="KeK4N"></a>
## 1、AWVS
非常经典的Web扫描神器，入门必备。推荐指数：★★★★★<br />官方网站：[https://www.acunetix.com](https://www.acunetix.com)<br />AWS10.5 以前的版本，提供的是有客户端和Web界面，不到50M的安装文件，界面简洁，扫描速度快，资源占用率低、扫描策略设置简单，客户端的各种辅助工具更是提供了强大的单兵作战能力。<br />![2021-05-28-15-24-52-645068.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622186818040-143b4dd3-a1e9-40b6-b846-9f9bed159f02.png#clientId=u86cb84aa-3501-4&from=ui&id=u41b90477&originHeight=728&originWidth=1006&originalType=binary&size=2201467&status=done&style=none&taskId=u4e3443f4-3dbe-4631-9f0e-c223d9fd0ce)
<a name="iLeJS"></a>
## 2、IBM AppScan
一款可与AWVS比肩的Web安全扫描工具， 推荐指数：★★★★<br />官方网站：[https://www.hcltechsw.com/products/appscan](https://www.hcltechsw.com/products/appscan)<br />总体而言，扫描的效果还是不错的，准确度也相对高一些，扫描速度确实是有点慢。一般而言，通常可以对一个web站点同时使用AWVS和AppScan都进行检测，然后相互验证扫描效果，提高检测的准确率。<br />![2021-05-28-15-24-52-737820.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622186825536-d819993f-a2b2-4b6b-a000-d2d5c5bc1ba9.png#clientId=u86cb84aa-3501-4&from=ui&id=udb9e4ad2&originHeight=631&originWidth=911&originalType=binary&size=1728023&status=done&style=none&taskId=u7d7415d5-0702-49b7-8ddb-2f407696900)
<a name="pMYnj"></a>
## 3、Goby
一款攻击面分析工具，推荐指数：★★★★★<br />官方网站：[https://gobies.org](https://gobies.org)<br />安装过程非常简单，Windows解压缩直接双击EXE即可运行，可跨平台支持Windows、Linux、 Mac。功能上也挺全面的，提供最全面的资产识别，最快的扫描体验，内置可自定义的漏洞扫描框架。<br />![2021-05-28-15-24-52-864482.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622186836083-83628c8c-fb3d-4863-86bd-27f0cb935326.png#clientId=u86cb84aa-3501-4&from=ui&id=u3f541a27&originHeight=660&originWidth=1080&originalType=binary&size=2142601&status=done&style=none&taskId=u470814cd-a75b-42ff-a396-763bacc9ecb)
<a name="ehuxT"></a>
## 4、Xray
一款强大的安全评估工具。推荐指数：★★★★<br />官方网站：[https://xray.cool](https://xray.cool)<br />xray 最好用的就是它的被动扫描模式，与burp进行联动更是一项绝活，让流量从Burp转发到Xray，所有的数据包透明，堪称指哪打哪的利器。<br />![2021-05-28-15-24-52-926320.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622186806520-1a1c856b-4e81-4afc-9f00-e35db0d93040.png#clientId=u86cb84aa-3501-4&from=ui&id=ufebd3f23&originHeight=542&originWidth=765&originalType=binary&size=1246513&status=done&style=none&taskId=uc6d8ddda-bfc5-4843-aad6-b78b62ba6be)
<a name="QQFfx"></a>
## 5、开源漏洞检测框架
以POC-T、pocsuite、pocscan、Osprey等为代表的开源项目，提供了可自定义的漏洞检测框架，Poc数量和质量，决定了检测效果，漏洞库的积累就显得尤为重要。<br />github项目地址：<br />POC-T：[https://github.com/Xyntax/POC-T](https://github.com/Xyntax/POC-T)<br />pocsuite3：[https://github.com/knownsec/pocsuite3](https://github.com/knownsec/pocsuite3)<br />pocscan：[https://github.com/erevus-cn/pocscan](https://github.com/erevus-cn/pocscan)<br />Osprey：[https://github.com/TophantTechnology/osprey](https://github.com/TophantTechnology/osprey)<br />开源的扫描器不计其数，复造轮子的人甚多，而真正能够成为神器的工具其实不多。
<a name="EiQMl"></a>
## 6、IAST  灰盒扫描工具
如果定位是在SDL的安全测试过程中，相比起黑盒测试方案，IAST则是一种新的安全测试方案。一般会通过Agent插桩监测，无需重放请求、无脏数据，几乎达到0误报，无疑是实现自动化安全测试的最佳选择。
<a name="dkqbU"></a>
## 7、商业Web应用扫描器
一些安全厂商提供了漏扫产品，不过在细分领域其实还有是一定区别的，比如有专门做Web应用安全扫描的，也有综合性漏洞扫描的，还有做Web安全监测的扫描产品，都有提供了一定的Web应用漏洞扫描的能力。<br />部分安全厂商及扫描产品汇总：<br />绿盟       绿盟WEB应用漏洞扫描系统(WVSS)  绿盟远程安全评估系统(RSAS)启明       天镜脆弱性扫描与管理系统安恒       Web应用弱点扫描器（MatriXay    明鉴远程安全评估系统奇安信     网神SecVSS3600漏洞扫描系统盛邦安全    Web漏洞扫描系统（RayWVS）    远程安全评估系统（RayVAS)斗象科技    ARS 智能漏洞与风险检测长亭科技    洞鉴（X-Ray）安全评估系统四叶草安全  全时风险感知平台
