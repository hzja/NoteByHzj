<a name="iRYhO"></a>
### 1、WEB源码拓展
```bash
	前言:WEB源码在安全测试中是非常重要的信息来源，可以用来代码审计漏洞也可以用来做信息突破口，其中WEB源码有很多技术需要简明分析。比如:获取某ASP源码后可以采用默认数据库下载为突破，获取某其他脚本源码漏洞可以进行代码审计挖掘或分析其业务逻辑等，总之源码的获取将为后期的安全测试提供了更多的思路。

```
1、关于WEB源码目录结构<br />2、关于WEB源码脚本类型<br />3、关于WEB源码应用分类<br />4、关于WEB源码其他说明
```bash
#数据库配置文件，后台目录，模版目录，数据库目录等

#ASP,PHP,ASPX,JSP,JAVAWEB等脚本类型源码安全问题

#社交，论坛，门户，第三方，博客等不同的代码机制对应漏洞

#开源，未开源问题，框架非框架问题，关于CMS识别问题及后续等

#关于源码获取的相关途径:搜索，咸鱼淘宝，第三方源码站，各种行业对应

#总结:

关注应用分类及脚本类型估摸出可能存在的漏洞（其中框架类例外），在获取源码后可进行本地安全测试或代码审计，也可以分析其目录工作原理（数据库备份，bak文件等），未获取到的源码采用各种方法想办法获取!
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1622711788547-6d6af02d-1d54-4110-b24e-c132e1746624.png#clientId=ua300d2b1-bc54-4&from=paste&height=435&id=u0db118ae&originHeight=870&originWidth=706&originalType=binary&ratio=1&size=63833&status=done&style=none&taskId=u45f0e1f3-7b8a-47a3-8428-83dfc1b1c51&width=353)<br />参考文档：[https://websec.readthedocs.io/zh/latest/intranet/windows.html](https://websec.readthedocs.io/zh/latest/intranet/windows.html)

第三方源码获取：[https://www.cnyma.com/](https://www.cnyma.com/)
<a name="PStz9"></a>
### 2、演示案例

- **1、asp、php等源码测试**

- **2、平台识别-某CMS无漏洞-默认数据库平台**![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1622732862040-c435ae7a-2e38-47f8-9dde-533b41cb99e8.png#clientId=ua300d2b1-bc54-4&from=paste&height=234&id=ud171caab&originHeight=468&originWidth=1295&originalType=binary&ratio=1&size=35860&status=done&style=none&taskId=uf3deb8db-b397-4697-ad9d-4eba5231c02&width=647.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1622732885347-80f81979-2209-405c-996b-3d0b79027f34.png#clientId=ua300d2b1-bc54-4&from=paste&height=387&id=u3bb35a52&originHeight=774&originWidth=1390&originalType=binary&ratio=1&size=101373&status=done&style=none&taskId=u93c78c9e-4fcd-4c8d-b50a-cd7ecdc01ed&width=695)

**在线cms识别网站**

| 网站 | 优点 | 缺点 |
| --- | --- | --- |
| [https://www.yunsee.cn/](https://www.yunsee.cn/) | 相应的速度快 | 企业版收费 |
| [http://whatweb.bugscaner.com/](http://whatweb.bugscaner.com/) | 可以在线和多个cms识别 | 广告 |


- **3、源码应用针对漏洞**
```bash
niushop电商类关注漏洞点-业务逻辑
参考地址：https://www.niushop.com.cn/download.html
电商类网站的主要漏洞就是网站的支付问题。
```

- **4、简要目标识别源码获取**
```bash
本地演示个人博客-手工发现其CMS-漏洞搜索或下载分析
http://weipan.1016sangshen.cn/ 内部搭建的靶场人
工爆框架-搜索特定url-获取其他相符站点-漏洞测试
借助特定行业源码或咸鱼进行搜索获取-本地搭建-代码审计或其他

```
