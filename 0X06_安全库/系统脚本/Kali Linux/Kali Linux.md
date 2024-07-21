---
title: Kali Linux
tags: []
---

# Kali Linux Learning 

优质博主:DeeL Mind     
视频学习网站:https://www.youtube.com/playlist?list=PLgZqc0esdeS9mbMWvAi5I7TWUWgaZLtpQ


## 攻击流程

    攻击流程->信息收集->判断是否存在漏洞->不存在漏洞->漏洞挖掘->判断是否存在漏洞->存在漏洞->攻击->权限维持->内网渗透->报告输出
                                     ->存在漏洞->攻击

### 信息收集

#### 网站目录扫描

    工具:Burp suit
    拿到IP和一个端口时，先进行端口扫描，再进行IP爆破
    课程思路: 拿到一个网站IP和端口时,第一步是网站端口扫描,第二步是子域名扫描，第三步是网站目录扫描
    
    网站目录
        登录入口
        后台入口
        数据库下载地址
        敏感文件下载地址
        网站目录架构
        GIT/SVN泄露
        代码泄露
        诸如此类
        
    如何获取网站目录
        爬虫爬取
        暴力扫描
        搜索引擎（Google等)
        ......
        
    IP/端口扫描
        IP开放端口
        域名,子域名
        ip:80 ip:81
        
    前端代码
        代码框架
        代码组件
        代码库
        代码注释（这个很关键)
        已知漏洞
        
    后端代码
        代码框架
        代码组件
        已知漏洞
        
    数据库
        数据库分类
        已知漏洞
        
    服务器
        IP端口
        服务信息
        已知漏洞
        
    漏洞扫描
        TOP XX
        
    开始攻击

#### Nmap

![Nmap攻击流程](nmap-lcga05g9.png)

    作用：活跃IP,端口探测;端口，服务，操作系统探测
    活跃IP:指IP是否在线;若IP的某个端口是Open,则该IP肯定是活跃的;
    保存文件、常用端口扫描、更换扫描方式、脚本扫描
    性能控制、主机发现、简单原理学习、规避防火墙
    MAC欺骗、空闲扫描、脚本扫描、简单总结
    
    端口状态:
        返回的端口号如果是open，则端口开;
        端口号如果是filtered，则端口无法知道是否开启;
        端口号如果是closed，则端口关闭;
    
    可用情况:
        拿到一个IP，但并不知道端口,可用Nmap扫描端口;
        没有IP,盲目扫描端口;
        
    命令:
        nmap 可直接查看帮助文档(帮助文档中都有)
        nmap 网站名(如bilibili.com) -p(端口号) 80(端口号80) -sS(扫描方式:类似的还有sT/sA/sW/sM) -sV(探测服务版本) -O(探测操作系统版本)
        nmap 网站名(如bilibili.com) -A(包括系统探测、服务版本探测、脚本扫描等全面探测)
        nmap -iR(随机扫描) 10(随机扫描的个数) -p(端口号) 80(端口号) -sS(扫描方式)
        nmap -n(不做DNS解析) -r(总是做DNS解析) -F(快速扫描,扫描比平时更少端口) 
        nmap 网站名(如bilibili.com) -p(端口号) 端口号 -A -sS
        nmap 8.8.8.8(IP,或者域名) -A -F(快速扫描) -sS/sT/sA/sW/sM(主机发现) -PS(主机发现,类似的还有PA/P/PY/PE/PP/PM,其后要跟端口号) -sL(扫描列表,单纯列出扫描目标但不扫描)
        nmap -Pn(该主机肯定在线,跳过主机发现 其后跟IP) 
        nmap -F -D(其后跟一些IP地址,冒充其后的ip地址,迷惑对方防火墙) -g(指定端口号,攻击特定端口号,也可理解为伪造端口)



#### 子域名

#### <b>子域名</b>(或者称<b>子域</b>;英语：Subdomain)是域名系统等级中，属于更高一层域的域。比如,mail.example.com和calendar.example.com是example.com的两个子域,而example.com则是顶级域.com的子域

+ 想要爆破某个网站，想知道其子域名

​	a.deelmind.com 对应于 ip1;

​	mail.deelmind.com 对应于 ip2;

​	......



#### 找子域名的方式

+ 1、爆破；可利用的工具网站:https://developers.facebook.com/tool/ct/;;https://crt.sh
+ 2、域传输漏洞；
+ 3、证书子域名
+ 4、robot.txt
+ 5、HTTP头跨域策略文件
+ 6、搜索引擎 site:*.baidu.com -www -baike
+ 7、其他搜索引擎
+ 8、公开数据
+ 9......



<b>找子域名的网站</b>

+ https://developers.facebook.com/tool/ct/;

+ https://crt.sh;

+ https://virustotal.com; 病毒相关的网站

+ 或者用 google.com搜索site://这里跟网站

  

  

  蜜罐原理以及工具讲解HFish

    蜜罐原理:对方设置的陷阱,让攻击者自主进去，同时记录攻击者攻击手段;
    
    工具:社区型免费蜜罐:https://github.com/hacklcx/HFish 蜜罐配置网页:https://hfish.net/#/2-3-windows



现在看到了：[第十一课](https://www.youtube.com/watch?v=4Yp-LQbGhQ4&list=PLgZqc0esdeS9mbMWvAi5I7TWUWgaZLtpQ&index=11)
