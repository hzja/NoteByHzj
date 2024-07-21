
<a name="Vbwlq"></a>
## 一、思维导图
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630153353093-df38faa7-35f1-4078-9431-92271562f68a.png#clientId=u347fd9e2-d8c1-4&from=paste&id=u7e9806df&originHeight=928&originWidth=773&originalType=url&ratio=1&status=done&style=none&taskId=u34fb67b9-9f38-4347-8741-74958846a10)
<a name="h1EOR"></a>
## 二、站点判断
```bash
#已知 CMS
如常见的 dedecms.discuz,wordpress 等源码结构，这种一般采用非框架类开发，但也有少部分采用的
是框架类开发，针对此类源码程序的安全检测，我们要利用公开的漏洞进行测试，如不存在可采用
白盒代码审计自行挖掘。


#开发框架
如常见的 thinkphp，spring,flask 等开发的源码程序，这种源码程序正常的安全测试思路：先获取对
应的开发框架信息(名字，版本)，通过公开的框架类安全问题进行测试，如不存在可采用白盒代码审
计自行挖掘。


#未知 CMS
如常见的企业或个人内部程序源码，也可以是某 CMS 二次开发的源码结构，针对此类的源码程序测
试思路：能识别二次开发就按已知 CMS 思路进行，不能确定二次开发的话可以采用常规综合类扫描
工具或脚本进行探针，也可以采用人工探针（功能点，参数，盲猜），同样在有源码的情况下也可以
进行代码审计自行挖掘。

```
<a name="ncFWF"></a>
## 

<a name="hjOvC"></a>
## 三、涉及资源
```bash
涉及资源：
https://vulhub.org/
https://wpvulndb.com/users/sign_up
https://github.com/wpscanteam/wpscan
https://github.com/ajinabraham/CMSScan
https://pan.baidu.com/s/1KCa-5gU8R8jPXYY19vyvZA 提取码：xiao
https://www.mozhe.cn/bug/detail/S0JTL0F4RE1sY2hGdHdwcUJ6aUFCQT09bW96aGUmozhe
```
<a name="hpmEr"></a>
## 四、CVE-2018-1273
```bash
$ cd spring/CVE-2018-1273
$ docker-compose up -d
```
[https://vulhub.org/#/environments/spring/CVE-2018-1273/](https://vulhub.org/#/environments/spring/CVE-2018-1273/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630157772687-7f3a9933-ccda-40fd-9c66-f0a4c19db885.png#clientId=u5c24766b-c44d-4&from=paste&height=376&id=u482ec027&originHeight=751&originWidth=1920&originalType=binary&ratio=1&size=155170&status=done&style=none&taskId=u1f79443e-039f-4e81-8d1f-991f096b2d0&width=960)
```bash
root@a644ddd591a3:/# ls /tmp/success
/tmp/success
```
<a name="hgP9j"></a>
## 五、演示案例
 开发框架类源码渗透测试报告-资讯-thinkphp,spring<br /> 已知 CMS 非框架类渗透测试报告-工具脚本-wordpress<br /> 已知 CMS 非框架类渗透测试报告-代码审计-qqyewu_php<br /> 未知 CMS 非框架类渗透测试报告-人工-你我都爱的 wg 哦~
<a name="dopTG"></a>
### 1、已知CMS非框架类--WordPress
墨者靶场：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630158817638-b42d9ac1-a77b-4468-a1bb-237e8bfae346.png#clientId=u5c24766b-c44d-4&from=paste&id=ub85d0a76&originHeight=302&originWidth=688&originalType=url&ratio=1&status=done&style=none&taskId=u79bea406-f578-4768-8eb3-baa0fd18838)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512111841.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630158817208-661663fc-5281-4e84-8b4e-93a8bba5822c.png#clientId=u5c24766b-c44d-4&from=paste&id=u7b2dc2da&originHeight=447&originWidth=854&originalType=url&ratio=1&status=done&style=none&taskId=ua23201ff-bbbf-499d-8e7e-52f4323e57e)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512111931.png)<br />首先上来，不知道是啥cms，识别。但是这里已经提示。或者也可以从数据包中来找一些线索。<br />可以用wpscan工具，或者对应的cms的工具来测试使用。<br />网址：<br />wpscan.org https://github.com/wpscanteam/wpscan <br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630158817633-459355a6-7330-4609-bc6a-d6accb2fa2dc.png#clientId=u5c24766b-c44d-4&from=paste&id=u9bcc4fac&originHeight=82&originWidth=427&originalType=url&ratio=1&status=done&style=none&taskId=u27573a77-e0ca-401d-88cf-58c8e619eb4)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512112430.png)<br />需要在官网弄个账号，获取token，配合上才能用他的漏洞库。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630158817635-44eb97e8-97da-431a-898e-1a86cf73b282.png#clientId=u5c24766b-c44d-4&from=paste&id=u3fc69167&originHeight=105&originWidth=598&originalType=url&ratio=1&status=done&style=none&taskId=u849e19f9-6735-4c87-897b-9b898a088fd)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512112520.png)<br />重新来<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630158817636-93a591d7-8b67-47a3-86cd-fd134ab07ad1.png#clientId=u5c24766b-c44d-4&from=paste&id=u3fd9b291&originHeight=78&originWidth=604&originalType=url&ratio=1&status=done&style=none&taskId=uc458363d-0fb8-4a31-b4c8-67d9caebfbf)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512112637.png)<br />扫出漏洞：下好exp漏洞利用就行了。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630158818030-293ea503-5dc9-4cf6-9ab8-3fa8eafc4465.png#clientId=u5c24766b-c44d-4&from=paste&id=u4f207749&originHeight=209&originWidth=576&originalType=url&ratio=1&status=done&style=none&taskId=ud3e83268-cdac-4685-bafd-1454af3a85c)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512112706.png)
<a name="MgEr9"></a>
### 2、已知CMS非框架类---代码审计---qqyewu_php
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630160176235-69395486-dd32-498c-ac87-322eec48db74.png#clientId=u5c24766b-c44d-4&from=paste&id=u9bdb695d&originHeight=575&originWidth=1096&originalType=url&ratio=1&status=done&style=none&taskId=u14476372-2660-4be1-8a4f-4c1adcd85fb)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512113033.png)<br />代码审计<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630160176219-bd659261-7fd4-4e16-9e4f-ab5d0e585a83.png#clientId=u5c24766b-c44d-4&from=paste&id=ue14c6ec5&originHeight=497&originWidth=932&originalType=url&ratio=1&status=done&style=none&taskId=u511b4212-0a54-451f-9cbf-96ca46cc1b6)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512115110.png)
<a name="ztZ2T"></a>
### 3、真实案例
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630160176283-78eff06e-5964-4ec1-b908-9ef984ec76ec.png#clientId=u5c24766b-c44d-4&from=paste&id=u296cdc70&originHeight=680&originWidth=1476&originalType=url&ratio=1&status=done&style=none&taskId=ucc5aa697-3228-488c-a919-c97075e6ee4)](https://gitee.com/darkerg/article-images/raw/master/typora/20210512120753.png)<br />[https://www.855km.cn/e/admin/](https://www.855km.cn/e/admin/)
