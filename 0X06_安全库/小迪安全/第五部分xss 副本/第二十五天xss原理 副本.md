<a name="fjmcO"></a>
### 思维导图
![web漏洞-xss跨站.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628132918301-6d92440b-aa98-4275-a9bf-b7f7658b568a.png#clientId=uf78c347c-9bd6-4&from=paste&id=u5adbf0be&originHeight=811&originWidth=908&originalType=binary&ratio=1&size=70648&status=done&style=none&taskId=uce3e4037-8462-4b12-a5e5-f9756b89bb4)

```bash
#XSS跨站漏洞产生原理，危害，特点?
本质，产生层面，函数类，漏洞操作对应层，危害影响，浏览器内核版本等

#Xss跨站漏洞分类:反射，存储，DOM
从产生层面，具体区别，危害等级等讲解
# 'onclick="alert (2) ">

#xss常规攻击手法:平台，工具，结合其他等

```
**00x01 js**执行****<br />Js是浏览器执行的前端语言，用户在存在xss漏洞的站点url后者能输入数据的部分插入js语言，服务器接收到此数据，认为是js代码，从而返回的时候执行。因此，攻击者可利用这个漏洞对站点插入任意js代码进行窃取用户的信息。

**00x02 xss**攻击****<br />XSS是一种经常出现在web应用中的计算机安全漏洞，它允许恶意web用户将代码植入到提供给其它用户使用的页面中。比如这些代码包括HTML代码和客户端脚本。攻击者利用XSS漏洞旁路掉访问控制——例如[同源策略](http://baike.baidu.com/item/%E5%90%8C%E6%BA%90%E7%AD%96%E7%95%A5)(same origin policy)。这种类型的漏洞由于被黑客用来编写危害性更大的[网络钓鱼](http://baike.baidu.com/item/%E7%BD%91%E7%BB%9C%E9%92%93%E9%B1%BC)(Phishing)攻击而变得广为人知。<br />对于[跨站脚本攻击](http://baike.baidu.com/item/%E8%B7%A8%E7%AB%99%E8%84%9A%E6%9C%AC%E6%94%BB%E5%87%BB)，黑客界共识是：跨站脚本攻击是新型的“[缓冲区溢出攻击](http://baike.baidu.com/item/%E7%BC%93%E5%86%B2%E5%8C%BA%E6%BA%A2%E5%87%BA%E6%94%BB%E5%87%BB)“，而JavaScript是新型的“ShellCode”。<br />**00x03 XSS**攻击的危害****

- 1、盗取各类用户[帐号](http://baike.baidu.com/item/%E5%B8%90%E5%8F%B7)，如机器登录帐号、用户网银帐号、各类管理员帐号
- 2、控制企业数据，包括读取、篡改、添加、删除企业敏感数据的能力
- 3、盗窃企业重要的具有商业价值的资料
- 4、非法转账
- 5、强制发送电子邮件
- 6、网站挂马
- 7、控制受害者机器向其它网站发起攻击（重定向语句）
- 8、窃取cookie的sessionid，冒充登录。

XSS攻击利用到最大就需要自己有台服务器用于窃取信息，还要利用一点社工，骗取别人点击恶意的链接。

**00x04 XSS**漏洞的分类****<br />**1.DOM****型(不经过服务器，前端js代码的利用)：**<br />本地利用漏洞，这种漏洞存在于页面中客户端脚本自身。其攻击过程如下所示：<br />Alice给Bob发送一个恶意构造了Web的[URL](http://baike.baidu.com/item/URL)。<br />Bob点击并查看了这个URL。<br />恶意页面中的JavaScript打开一个具有漏洞的HTML页面并将其安装在Bob电脑上。<br />具有漏洞的HTML页面包含了在Bob电脑本地域执行的JavaScript。<br />Alice的[恶意脚本](http://baike.baidu.com/item/%E6%81%B6%E6%84%8F%E8%84%9A%E6%9C%AC)可以在Bob的电脑上执行Bob所持有的权限下的命令。

**2.反射型：**<br />这种漏洞和类型A有些类似，不同的是Web客户端使用Server端脚本生成页面为用户提供数据时，如果未经验证的用户数据被包含在页面中而未经[HTML实体](http://baike.baidu.com/item/HTML%E5%AE%9E%E4%BD%93)编码，客户端代码便能够注入到[动态页面](http://baike.baidu.com/item/%E5%8A%A8%E6%80%81%E9%A1%B5%E9%9D%A2)中。<br />其攻击过程如下：<br />Alice经常浏览某个网站，此网站为Bob所拥有。Bob的站点运行Alice使用用户名/密码进行登录，并存储敏感信息(比如银行帐户信息)。<br />Charly发现Bob的站点包含反射性的XSS漏洞。<br />Charly编写一个利用漏洞的URL，并将其冒充为来自Bob的邮件发送给Alice。<br />Alice在登录到Bob的站点后，浏览Charly提供的URL。<br />嵌入到URL中的[恶意脚本](http://baike.baidu.com/item/%E6%81%B6%E6%84%8F%E8%84%9A%E6%9C%AC)在Alice的浏览器中执行，就像它直接来自Bob的服务器一样。此脚本盗窃敏感信息(授权、信用卡、帐号信息等)然后在Alice完全不知情的情况下将这些信息发送到Charly的Web站点。

**3.存储型（长久型，危害最大）：**<br />该类型是应用最为广泛而且有可能影响到Web服务器自身安全的漏洞，[骇客](http://baike.baidu.com/item/%E9%AA%87%E5%AE%A2)将攻击脚本上传到Web服务器上，使得所有访问该页面的用户都面临信息泄漏的可能，其中也包括了Web服务器的管理员。其攻击过程如下：<br />Bob拥有一个Web站点，该站点允许用户发布信息/浏览已发布的信息。<br />Charly注意到Bob的站点具有类型C的XSS漏洞。<br />Charly发布一个热点信息，吸引其它用户纷纷阅读。<br />Bob或者是任何的其他人如Alice浏览该信息，其会话cookies或者其它信息将被Charly盗走。<br />类型A直接威胁用户个体，而类型B和类型C所威胁的对象都是企业级Web应用。

创建1.PHP然后访问传参

```bash
<?php
$xss = $_GET['x'];
echo $xss;
?>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628133622764-a9c03a16-a319-4515-a31f-36e05fcc4ace.png#clientId=uf78c347c-9bd6-4&from=paste&height=307&id=u23fb52dd&originHeight=614&originWidth=1920&originalType=binary&ratio=1&size=40224&status=done&style=none&taskId=udd57443f-e194-43b0-a37f-753b51b6e7b&width=960)
<a name="oPjkR"></a>
### 搭建靶场
```bash
$ docker run -d -p 80:80 --name pikachu area39/pikachu
```
反射型<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628135478892-17b78cf6-8c7b-4652-b895-360068073faa.png#clientId=uf78c347c-9bd6-4&from=paste&height=284&id=uc028ca57&originHeight=567&originWidth=1920&originalType=binary&ratio=1&size=81094&status=done&style=none&taskId=u34ed5e9f-598c-480f-a077-575aab59812&width=960)<br />存储型，每次刷新的时候都会弹框<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628135610154-50f608bd-fd6e-44cc-b355-0c7a2781c738.png#clientId=uf78c347c-9bd6-4&from=paste&height=253&id=u4a26b015&originHeight=505&originWidth=1918&originalType=binary&ratio=1&size=64279&status=done&style=none&taskId=u6b44a5b8-28af-46f8-baa5-432aa688e02&width=959)
<a name="V16wg"></a>
### 随意留言板搭建
```bash
<BODY onload="alert('xss')">
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628148000505-33510aed-0c30-4766-942a-ad8bc448a2bc.png#clientId=uf78c347c-9bd6-4&from=paste&id=u27319127&originHeight=622&originWidth=1180&originalType=binary&ratio=1&size=113932&status=done&style=none&taskId=u34db2dae-f4aa-4740-855f-0194744dab3)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628147925237-93e03f2a-9bc4-4e96-9ca2-67ed4a748990.png#clientId=uf78c347c-9bd6-4&from=paste&height=401&id=ue2afdbbc&originHeight=802&originWidth=1677&originalType=binary&ratio=1&size=132820&status=done&style=none&taskId=u8b6c9c41-6762-448c-9547-dfefe8de803&width=838.5)
<a name="SSQrf"></a>
### xss平台
[https://xss8.cc/bdstatic.com/](https://xss8.cc/bdstatic.com/)

