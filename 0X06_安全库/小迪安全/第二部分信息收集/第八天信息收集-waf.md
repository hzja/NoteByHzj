<a name="O1UiF"></a>
## 一、前言说明
在安全测试中，信息收集是非常重要的一个环节，此环节的信息将影响到后续的成功几率，掌握信息的多少将决定发现漏洞机会大小，换言之决定着是否能完成目标的测试任务。也可以很直接的跟大家说:渗透测试的思路就是从信息收集这里开始，你与大牛的差距也是从这里开始的!<br />![信息收集.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623307390451-79e7a71e-656c-4401-ae03-c817b38967cc.png#clientId=u77a71539-a0c0-4&from=paste&height=1869&id=u84a5a1db&originHeight=2493&originWidth=1251&originalType=binary&ratio=2&size=378727&status=done&style=none&taskId=u812b09b1-d8e0-4dab-ae93-2f386ae1b2e&width=938)
<a name="sbA3e"></a>
## 二、站点搭建分析
<a name="LVUS9"></a>
### 1、搭建习惯-目录型站点
```bash
简单的理解就是主站上面存在其他的cms程序
例如：
	学生网站的上面通过后台扫描探针发现有一个bbs的目录一点击发现是一个bbs的论坛网站如：www.xxx.com/bbs
 我们把这个成为目录型网站、可以有两种找到漏洞的思路一个是主站的漏洞另外的一个是bbs上面的漏洞
```
<a name="X7IzV"></a>
### 2、搭建习惯-端口类站点
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623309693194-f3e0696b-3a11-426e-95e7-275d4310f2ee.png#clientId=u77a71539-a0c0-4&from=paste&height=283&id=uf548b7cd&originHeight=565&originWidth=990&originalType=binary&ratio=2&size=361706&status=done&style=none&taskId=uc98e39b3-0eb8-4592-810c-6bf6b6becf3&width=495)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623309747376-8f0312da-c76e-4152-8ada-f6e477f08769.png#clientId=u77a71539-a0c0-4&from=paste&height=280&id=u5fd69cb1&originHeight=539&originWidth=987&originalType=binary&ratio=2&size=115322&status=done&style=none&taskId=u7d510d57-92e5-419a-b598-c15e59caee4&width=513.5)<br />有的站点不是使用的是默认的站点80而是其他的端口、可以使用shodan这种工具去收集端口

<a name="fBIGX"></a>
### 3、搭建习惯-子域名站点
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623310053639-425e1708-fef7-445f-8650-83ac695a97aa.png#clientId=u77a71539-a0c0-4&from=paste&height=258&id=ubc155f42&originHeight=516&originWidth=982&originalType=binary&ratio=2&size=521778&status=done&style=none&taskId=u8e5f49e8-5829-4c4c-b4ea-5a00a371b0e&width=491)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623310088509-4ae4f6f7-6cb9-492d-ac13-262ea278f1cf.png#clientId=u77a71539-a0c0-4&from=paste&height=255&id=ue11888bc&originHeight=510&originWidth=990&originalType=binary&ratio=2&size=431068&status=done&style=none&taskId=u29e77e12-4bb7-4479-bb7e-876a58a2663&width=495)<br />**备注：**现在的主流网站都是采用的这种模式且子域名和网站之间很有可能是不在同一台的服务器上面。

<a name="bpJcK"></a>
### 4、搭建习惯-类似域名站点
有些公司由于业务的发展将原来的域名弃用、选择了其他的域名但是我们访问他的旧的域名还是能够访问、有的是二级域名的更换而有的是顶级域名的更换<br />可以通过他的旧的域名找到一些突破口、<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623311217855-af50ba83-a4e7-4ed9-bc75-cebe73fc9df9.png#clientId=u77a71539-a0c0-4&from=paste&height=379&id=u0b28b0fc&originHeight=757&originWidth=1024&originalType=binary&ratio=2&size=624468&status=done&style=none&taskId=u2cba8a47-a43b-440d-9c2d-5d411b67ab6&width=512)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623311236392-dbfc998f-650a-4ce5-b3f8-eb77981a7d93.png#clientId=u77a71539-a0c0-4&from=paste&height=376&id=u9f781d82&originHeight=785&originWidth=1157&originalType=binary&ratio=2&size=985647&status=done&style=none&taskId=u2d8e6ed1-9a52-40e0-b18a-15084e008cd&width=554.5)<br />例如：京东的网站是jd.com 那么他就有可能是采用了jd.net jd.cn等域名我们采用社工的方式去尝试获取他的相关域名信息


<a name="cq8Dy"></a>
### 5、搭建习惯-旁注,c段站点
旁注：同一个服务器上面存在多个站点、但是你要攻击的是A网站由于各种原因不能完成安全测试。就通过测试B网站进入服务器然后在攻击A网站最终实现目的。<br />C段：不同的服务器上面存在不同的网站，通过扫描发现与你渗透测试的是同一个网段最终拿下服务器、然后通过内网渗透的方式拿下渗透服务器。<br />在线工具：[https://www.webscan.cc/](https://www.webscan.cc/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623312899032-2c09b7c9-3d83-418a-a391-cec2f9d4f5cb.png#clientId=u77a71539-a0c0-4&from=paste&height=196&id=u328ad83a&originHeight=391&originWidth=825&originalType=binary&ratio=2&size=138000&status=done&style=none&taskId=u1d68c1b8-1932-4388-a7e5-5894232cc67&width=412.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623312814056-d73ebf9e-0b3c-4fce-8a9a-a6849721fd22.png#clientId=u77a71539-a0c0-4&from=paste&height=249&id=uc1ffe796&originHeight=419&originWidth=829&originalType=binary&ratio=2&size=93215&status=done&style=none&taskId=ue661a356-0f4b-4d61-861f-036a345aff2&width=492.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623312778830-9aa0efd4-3f84-4f6a-a0b8-9446e7d95b32.png#clientId=u77a71539-a0c0-4&from=paste&height=250&id=u322520aa&originHeight=499&originWidth=766&originalType=binary&ratio=2&size=324951&status=done&style=none&taskId=u03949fc2-c3e3-43e9-b656-87ef799b08a&width=383)<br />通过旁注查询可以发现同一个服务器上面有两个站点可以通过对另外的一个站点进行突破。
<a name="z9JLf"></a>
### 6、搭建习惯-搭建软件特征站点
有的网站是借助于第三方的集成搭建工具实现例如：PHPstudy、宝塔等环境这样的集成环境搭建的危害就是泄露了详细的版本信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623315191745-28be7472-876e-4aaa-b0ba-1bb96cfa6860.png#clientId=u77a71539-a0c0-4&from=paste&height=174&id=u446f219a&originHeight=316&originWidth=772&originalType=binary&ratio=2&size=68439&status=done&style=none&taskId=uec1c4b96-86c3-45b0-96c2-487c963dc8b&width=424)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623315216297-f3eaa8b3-e65c-49c4-a45e-7ac63c582ba8.png#clientId=u77a71539-a0c0-4&from=paste&height=206&id=ubfec15fa&originHeight=411&originWidth=772&originalType=binary&ratio=2&size=37211&status=done&style=none&taskId=u31142c2f-bb9b-4efa-acd7-d0ccb34fef4&width=386)<br />phpstudy搭建了之后在默认的站点安装了phpmyadmin有的网站没有做安全性直接可以通过用户名：root密码：root 登录进入
<a name="sQUxe"></a>
## 三、WAF防护分析
<a name="VhfgI"></a>
### 1、什么是wAF应用?
Web应用防护系统（也称为：网站应用级入侵防御系统。英文：Web Application Firewall，简称： WAF）。利用国际上公认的一种说法：Web应用防火墙是通过执行一系列针对HTTP/HTTPS的安全策略来专门为Web应用提供保护的一款产品。
<a name="ovbBt"></a>
### 2、如何快速识别WAF?
1、采用工具wafwoof<br />获取地址：[https://codeload.github.com/EnableSecurity/wafw00f/zip/refs/heads/master](https://codeload.github.com/EnableSecurity/wafw00f/zip/refs/heads/master)<br />安装之前一定要有python的环境不然安装不上
```bash
# unzip wafw00f-master.zip
# python3.8 setup.py install
# ls -ld wafw00f
drwxr-xr-x 6 root root 4096  6月 10 17:22 wafw00f
# cd wafw00f 
#python main.py https://www.hlszsb.com/

[*] Checking https://www.hlszsb.com/
[+] Generic Detection results:
[-] No WAF detected by the generic detection
[~] Number of requests: 7
```
wafw00f的缺点判断的不是特别的准确存在误报或识别不出的情况。

2、在有些网站的请求信息当中有的网站没有做安全信息上面留下了waf的相关信息<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623321097508-bb59f182-b4cd-4fc3-9754-16552c7f8523.png#clientId=u77a71539-a0c0-4&from=paste&height=141&id=u761686ab&originHeight=282&originWidth=880&originalType=binary&ratio=2&size=58455&status=done&style=none&taskId=uc24ff997-8a01-4f0c-bd12-7b142a12265&width=440)

3、使用nmap指纹检测
```bash
nmap --script==http-waf-fingerprint

nmap --script=http-waf-detec
```

4、identYwaf<br />参考地址：[https://github.com/stamparm/identywaf](https://github.com/stamparm/identywaf)
```bash
C:\Users\admin\Desktop\security\软件\identYwaf-master>python identYwaf.py https://www.manjaro.cn/
                                    __ __
 ____  ___      ___  ____   ______ |  T  T __    __   ____  _____
l    j|   \    /  _]|    \ |      T|  |  ||  T__T  T /    T|   __|
 |  T |    \  /  [_ |  _  Yl_j  l_j|  ~  ||  |  |  |Y  o  ||  l_
 |  | |  D  YY    _]|  |  |  |  |  |___  ||  |  |  ||     ||   _|
 j  l |     ||   [_ |  |  |  |  |  |     ! \      / |  |  ||  ]
|____jl_____jl_____jl__j__j  l__j  l____/   \_/\_/  l__j__jl__j  (1.0.134)

[o] initializing handlers...
[i] checking hostname 'www.manjaro.cn'...
[i] running basic heuristic test...
[i] rejected summary: 200 ('<title>����ҳ��</title>')
[-] non-blind match: -
[i] running payload tests... (45/45)
[=] results: '.xx.xxxxx..xxxxxxxx.xxx.x...x.xxx......x..xxx'
[=] hardness: hard (60%)
[=] blocked categories: SQLi, XSS, XPATHi, XXE, PHPi, PT
[=] signature: '90fa:RVZXu261OEhCWapBYKcPk4JzWOpohM4JiUcMr2RXg1uQJbX3uhdOnthtOj+hX7AA16FcPhJOdLoXo2tKaK99n+i7c4RmkgI2FZjxtDtAeq+c3qA4chW1XaTD'
[+] blind match: 'Safedog' (100%)
```
`与wafwoof相比运行速度慢，比较稳定**推荐还是使用这一款工具。**`
<a name="WyBEE"></a>
### 3、识别wAF对于安全测试的意义?
对于一个网站要是使用了waf而渗透人员没有识别直接使用工具进行扫描有可能会导致waf将你的ip地址拉入黑名单而不能访问。而识别waf在于有针对性行的绕过各个厂商的waf可能存在着不同的绕过思路。


