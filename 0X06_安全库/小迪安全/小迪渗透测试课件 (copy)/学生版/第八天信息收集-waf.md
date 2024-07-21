# 第八天信息收集waf

## 一、前言说明

​		在安全测试中，信息收集是非常重要的一个环节，此环节的信息将影响到后续的成功几率，掌握信息的多少将决定发现漏洞机会大小，换言之决定着是否能完成目标的测试任务。也可以很直接的跟大家说:渗透测试的思路就是从信息收集这里开始，你与大牛的差距也是从这里开始的!

![信息收集1](D:\Note\网络安全\图片\信息收集1.png)



## 二、站点搭建分析

### 1、搭建习惯-目录型站点

~~~ bash
简单的理解就是主站上面存在其他的cms程序
例如：
	学生网站的上面通过后台扫描探针发现有一个bbs的目录一点击发现是一个bbs的论坛网站如：www.xxx.com/bbs
 我们把这个成为目录型网站、可以有两种找到漏洞的思路一个是主站的漏洞另外的一个是bbs上面的漏洞
~~~



### 2、搭建习惯-端口类站点

![image46](D:\Note\网络安全\图片\image46.png)

![image47](D:\Note\网络安全\图片\image47.png)

有的站点不是使用的是默认的站点80而是其他的端口、可以使用shodan这种工具去收集端口



### 3、搭建习惯-子域名站点

![image48](D:\Note\网络安全\图片\image48.png)

![image49](D:\Note\网络安全\图片\image49.png)

**备注：**现在的主流网站都是采用的这种模式且子域名和网站之间很有可能是不在同一台的服务器上面。



### 4、搭建习惯-类似域名站点

有些公司由于业务的发展将原来的域名弃用、选择了其他的域名但是我们访问他的旧的域名还是能够访问、有的是二级域名的更换而有的是顶级域名的更换，可以通过他的旧的域名找到一些突破口

![image50](D:\Note\网络安全\图片\image50.png)

![image51](D:\Note\网络安全\图片\image51.png)

例如：京东的网站是jd.com 那么他就有可能是采用了jd.net jd.cn等域名我们采用社工的方式去尝试获取他的相关域名信息



### 5、搭建习惯-旁注,c段站点

旁注：同一个服务器上面存在多个站点、但是你要攻击的是A网站由于各种原因不能完成安全测试。就通过测试B网站进入服务器然后在攻击A网站最终实现目的。

C段：不同的服务器上面存在不同的网站，通过扫描发现与你渗透测试的是同一个网段最终拿下服务器、然后通过内网渗透的方式拿下渗透服务器。

在线工具：https://www.webscan.cc/

![image52](D:\Note\网络安全\图片\image52.png)

![image53](D:\Note\网络安全\图片\image53.png)

![image54](D:\Note\网络安全\图片\image54.png)

​		旁注查询可以发现同一个服务器上面有两个站点可以通过对另外的一个站点进行突破。



### 6、搭建习惯-搭建软件特征站点

有的网站是借助于第三方的集成搭建工具实现例如：PHPstudy、宝塔等环境这样的集成环境搭建的危害就是泄露了详细的版本信息。

![image55](D:\Note\网络安全\图片\image55.png)

![image56](D:\Note\网络安全\图片\image56.png)

​		phpstudy搭建了之后在默认的站点安装了phpmyadmin有的网站没有做安全性直接可以通过用户名：root密码：root 登录进入



## 三、WAF防护分析

### 1、什么是wAF应用?

​		Web应用防护系统（也称为：网站应用级入侵防御系统。英文：Web Application Firewall，简称： WAF）。利用国际上公认的一种说法：Web应用防火墙是通过执行一系列针对HTTP/HTTPS的安全策略来专门为Web应用提供保护的一款产品。



### 2、如何快速识别WAF?

1、采用工具wafwoof

​		获取地址：https://codeload.github.com/EnableSecurity/wafw00f/zip/refs/heads/master

​		安装之前一定要有python的环境不然安装不上

~~~ bash
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
~~~

wafw00f的缺点判断的不是特别的准确存在误报或识别不出的情况。



2、在有些网站的请求信息当中有的网站没有做安全信息上面留下了waf的相关信息![image57](D:\Note\网络安全\图片\image57.png)



3、使用nmap指纹检测

~~~ bash
nmap --script==http-waf-fingerprint
nmap --script=http-waf-detec
~~~



4、identYwaf
		参考地址：https://github.com/stamparm/identywaf

~~~ bash
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
~~~

与wafwoof相比运行速度慢，比较稳定**推荐还是使用这一款工具。**



### 3、识别wAF对于安全测试的意义?

对于一个网站要是使用了waf而渗透人员没有识别直接使用工具进行扫描有可能会导致waf将你的ip地址拉入黑名单而不能访问。而识别waf在于有针对性行的绕过各个厂商的waf可能存在着不同的绕过思路。