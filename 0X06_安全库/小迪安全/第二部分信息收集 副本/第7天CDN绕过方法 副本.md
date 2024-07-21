CDN的全称是Content Delivery Network，即内容分发网络。CDN是构建在现有网络基础之上的智能虚拟网络，依靠部署在各地的边缘服务器，通过中心平台的负载均衡、内容分发、调度等功能模块，使用户就近获取所需内容，降低网络拥塞，提高用户访问响应速度和命中率。但在安全测试过程中，若目标存在CDN服务，将会影响到后续的安全测试过程。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623216008707-d063e412-ff3d-4256-8b9d-8ef9ef41d219.png#clientId=u99aa0c77-5059-4&from=paste&height=280&id=ubfe488cf&originHeight=559&originWidth=965&originalType=binary&ratio=2&size=416936&status=done&style=none&taskId=u6bbbffc4-7f9e-412e-a50b-e53807cc361&width=482.5)
<a name="a9G4m"></a>
### 1、如何判断目标存在CDN服务?
```bash
利用多节点技术进行请求返回判断
	http://ping.chinaz.com/
 	http://ping.aizhan.com/
  http://ce.cloud.360.cn/
```
如果ping的结果只有一个那么就没有CDN、要是不止一个则可以判断为有CDN<br />没有使用CDN<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623217475468-cd530cb6-3c7d-45c6-8346-ec66dcc9993f.png#clientId=u99aa0c77-5059-4&from=paste&height=48&id=ude527105&originHeight=95&originWidth=930&originalType=binary&ratio=2&size=6947&status=done&style=none&taskId=u3bfbdcde-e8e4-4187-b305-b54f4a103e1&width=465)<br />使用CDN<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623217655187-bcd611f8-5ddc-4c9e-af74-05ab076501bb.png#clientId=u99aa0c77-5059-4&from=paste&height=62&id=u74dae834&originHeight=123&originWidth=914&originalType=binary&ratio=2&size=10233&status=done&style=none&taskId=u0828b899-46ce-4768-9643-d45b83f5eee&width=457)
<a name="YROV0"></a>
### 2、CDN对于安全测试有那些影响?
```bash
	cdn会隐藏服务器真实的ip地址，无法对目标网站的操作系统进行渗透，但cdn站点又可以理解为是目标站点的镜像站点（大多数都是静态cdn加速），拥有相同的网站架构，且cdn服务器可与站点服务器进行交互，因此sql注入，xss等漏洞的挖掘并不受太大影响。
```
<a name="cF4ez"></a>
### 3、目前常见的CDN绕过技术有哪些?
```bash
子域名查询：
					因为有些主站是做了CDN服务而子站是没有做CDN服务
邮件服务查询
					因为邮箱大部分都是内部人在访问、而且访问的量也不是很大，一般是没有做CDN。
国外地址请求
					因为很多的企业没有在国外部署CDN，要是用国外的地址请求、就容易找到他的真实地址。
遗留文件、扫描全网
					如PHPinfo信息当中会遗留出ip地址
黑暗引擎搜索
					fofa、傻蛋、谛听、zoomeye、censys
特定文件dns历史记录，以量打量

#CDN真实IP地址获取后绑定指向地址
更改本地HOSTS解析指向文件

奇淫技巧
	fackcdn w8fuckcdn	zmap
```
[<br />](https://tools.ipip.net/cdn.php)
<a name="WsZBa"></a>
### 四、案例演示获取真实ip
<a name="aorhn"></a>
#### 1、以学而思网站为例
`www.xueersi.com`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623221502624-1ad2b891-695a-4bb8-8aed-6d76a4af55d6.png#clientId=u99aa0c77-5059-4&from=paste&height=426&id=uab164cb9&originHeight=852&originWidth=1317&originalType=binary&ratio=2&size=200792&status=done&style=none&taskId=uaefcc56e-db22-4d83-8d66-2a3d83ab745&width=658.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623221515487-67a0826d-e508-431a-b414-1de25c938330.png#clientId=u99aa0c77-5059-4&from=paste&height=300&id=ud6ca20e8&originHeight=600&originWidth=1228&originalType=binary&ratio=2&size=64641&status=done&style=none&taskId=u06de8742-cc36-4ebf-82a1-c18f6c16d6c&width=614)<br />`xuersi.com`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623221535716-9ffd1037-e88f-4fd3-88f4-da54ae6b4e15.png#clientId=u99aa0c77-5059-4&from=paste&height=391&id=u391b9f29&originHeight=781&originWidth=1338&originalType=binary&ratio=2&size=170855&status=done&style=none&taskId=u1be28b45-fc4b-4ea2-a53d-6ea35c44c6c&width=669)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623221578304-dbf57d08-16b5-487c-9d68-7fb7a8473ee6.png#clientId=u99aa0c77-5059-4&from=paste&height=289&id=ue7e66976&originHeight=577&originWidth=1209&originalType=binary&ratio=2&size=66715&status=done&style=none&taskId=u93e1e740-00e7-4437-937d-6693270f08d&width=604.5)<br />备注：通过对比发现xueersi.com没有使用cdn直接获取到了真实的IP地址、而其中的IP地址71、72是负载均衡的访问入口。

验证获取到ip是否可信可以采用第三方的ip地址查询工具经行验证。<br />[https://get-site-ip.com/](https://get-site-ip.com/)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623221828776-c24014d7-9a2b-485a-ac5d-80e771185112.png#clientId=u99aa0c77-5059-4&from=paste&height=222&id=u753a7f0a&originHeight=443&originWidth=1325&originalType=binary&ratio=2&size=148334&status=done&style=none&taskId=u45ec08c6-cfd9-4828-b8ac-8274ffb3e4d&width=662.5)
<a name="Hs0v9"></a>
#### 2、www.sp910.com
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623222180759-dd3fc90b-e5da-469b-923d-78ad27dfd383.png#clientId=u99aa0c77-5059-4&from=paste&height=61&id=u61b06550&originHeight=122&originWidth=1210&originalType=binary&ratio=2&size=10350&status=done&style=none&taskId=udee4040c-26a4-4155-a0bc-3ceb50eb86b&width=605)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623222189256-f2153fc7-0a26-4df7-8190-1908ce782ece.png#clientId=u99aa0c77-5059-4&from=paste&height=298&id=u0a74c330&originHeight=595&originWidth=950&originalType=binary&ratio=2&size=53244&status=done&style=none&taskId=u9955e112-bebf-47ec-9260-7cac25a6cd9&width=475)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623222237005-e0383a26-4166-43c1-be1c-372a7ce60535.png#clientId=u99aa0c77-5059-4&from=paste&height=64&id=ud1301313&originHeight=127&originWidth=1014&originalType=binary&ratio=2&size=9308&status=done&style=none&taskId=u12dbe752-5d53-4f32-9d08-519f1d77b27&width=507)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623222252362-e5ddc8d7-0a32-4880-9c41-63085e82523a.png#clientId=u99aa0c77-5059-4&from=paste&height=186&id=u0db3c72b&originHeight=371&originWidth=891&originalType=binary&ratio=2&size=30912&status=done&style=none&taskId=ud44df6af-7195-4e9f-931d-aa8b7582ff5&width=445.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623222289371-e1764710-f9f8-4eeb-94c6-2eb7113e3a84.png#clientId=u99aa0c77-5059-4&from=paste&height=99&id=u751cd2b2&originHeight=198&originWidth=1301&originalType=binary&ratio=2&size=102895&status=done&style=none&taskId=u075efa3d-06ca-4198-9cfc-ce5a4ebef17&width=650.5)
<a name="ene4I"></a>
#### 3、cdn网站
[https://tools.ipip.net/cdn.php](https://tools.ipip.net/cdn.php)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623720533766-d5412aec-bdcb-47fe-acde-02807483daf0.png#clientId=u21c361b5-2c2f-4&from=paste&height=179&id=u656e2640&originHeight=357&originWidth=1525&originalType=binary&ratio=2&size=40365&status=done&style=none&taskId=u60642e6a-e3a7-403b-b4c6-0a0a2ea0780&width=762.5)


`总结:`网站的cdn的绕过方法第三方工具或通过邮件的方式不一定完全靠谱还是要结合一定的社会工程学才能找到真实的IP地址。

