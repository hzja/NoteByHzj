<a name="SDeUJ"></a>
# 一、思维导图
![](https://cdn.nlark.com/yuque/0/2022/webp/2476579/1647844675027-e63ddf0c-8481-4a12-b906-5874e16a3b16.webp#clientId=u53a3a109-98b7-4&from=paste&id=u06e06a00&originHeight=1707&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b021c32-3891-4ab0-bda5-6c3f9ba3074&title=)<br />1.文件上传漏洞挖掘：

- (1)关键字搜索（函数、键字、全局变量等）：比如$_FILES，move_uploades_file等
- (2)应该功能抓包：寻找任何可能存在上传的应用功能点，比如前台会员中心，后台新闻添加等。
- (3)漏洞举例：逻辑漏洞-先上传文件再判断后缀名，通过MIME类型来判断文件类型、前端校验文件类型而服务端未校验。

2.MVC开发框架类:：[https://www.cnblogs.com/wsybky/p/8638876.html](https://www.cnblogs.com/wsybky/p/8638876.html)<br />3.Thinkphp框架：[http://sites.thinkphp.cn/1556331](http://sites.thinkphp.cn/1556331)<br />4.phpstorm+xdebug调试：[https://blog.csdn.net/yinhangbbbbb/article/details/79247331](https://blog.csdn.net/yinhangbbbbb/article/details/79247331)<br />![](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647844721797-33df25f4-0e4c-4825-bbe4-a4025b84cd54.png#clientId=u53a3a109-98b7-4&from=paste&id=uce7e8625&originHeight=491&originWidth=1137&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc52b4f6d-bca3-4007-81eb-cfb710924f8&title=)
<a name="IzhPB"></a>
# 二、演示案例
演示案例

- PHP文件上传全局变量$_FILES
- PHPStorm+xdebug断点调试演示
- Beescms无框架后台任意文件上传
- Finecms基于前台MVC任意文件上传
- Cltphp基于前台TP5框架任意文件上传

漏洞挖掘过程

- 搜索$_FILES-->后台中心-->上传图像-->跟踪代码-->逻辑判断
- 业务功能分析-->会员中心-->上传图像-->跟踪代码-->逻辑判断
- 搜索文件上传-->会员中心-->上传图像-->跟踪代码-->逻辑判断
<a name="ybGJ6"></a>
# 三、beescms
<a name="EeHlA"></a>
## 没有测试到漏洞
1、官方下载Beescms v4.0,下载地址: [http://beescms.com/cxxz.html](http://beescms.com/cxxz.html)<br />2、解压压缩文件,然后把文件放到phpstudy的网站根目录<br />3、浏览器访问[http://localhost/beescms/install,](http://192.168.10.171/beescms/install,)开始安装

备注：笔者在安装安装的环境是windows+phpstudy2018，在安装的过程中莫名其妙的出现访问`403`<br />解决方案：**打开phpStudy，点击按键“其他选项菜单”=>找到phpStudy配置=>点击“允许目录列表”。最后重启phpStudy。**<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647857448459-5ab64a39-ce4a-4752-8408-d6ee5ea5baaa.png#clientId=u11e2dacf-3b06-4&from=paste&height=389&id=u99f74c83&originHeight=312&originWidth=554&originalType=url&ratio=1&rotation=0&showTitle=false&size=23558&status=done&style=none&taskId=u6640f11a-87ea-44d4-a36b-faa3ee4553c&title=&width=691)<br />正常安装后<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647857590416-2e93e588-bf46-46e1-80fb-7dd55d71efff.png#clientId=u11e2dacf-3b06-4&from=paste&height=834&id=u30e0c7c9&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=116208&status=done&style=none&taskId=u890486b3-8843-4663-8d18-30158a4bd60&title=&width=1536)<br />seasy代码审计，全局搜索-->`$_FILES`查找<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647858299998-6da1dbfa-5059-4ee9-96a5-eda59e7900ba.png#clientId=u11e2dacf-3b06-4&from=paste&height=834&id=u03694a2a&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=398676&status=done&style=none&taskId=u4c1af469-547c-4c0c-a6b4-8d546afd6cc&title=&width=1536)<br />`[http://127.0.0.1/beescms/admin/admin_file_upload.php](http://127.0.0.1/beescms/admin/admin_file_upload.php)`默认的用户名和密码都是`admin`<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647858436937-3c0b1f40-d5da-4cb7-bbf7-d4b0dcf4cddd.png#clientId=u11e2dacf-3b06-4&from=paste&height=311&id=u2f2f28c3&originHeight=389&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=67147&status=done&style=none&taskId=u6ff7e141-4abf-4d73-8208-7b479539fd4&title=&width=1536)<br />上传一张图片，配合burpsuit抓包查看数据包内容<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647858782908-c117f5a0-30f8-4fe0-b926-139497b3afc7.png#clientId=u11e2dacf-3b06-4&from=paste&height=648&id=u4c3e9b3e&originHeight=810&originWidth=1917&originalType=binary&ratio=1&rotation=0&showTitle=false&size=148970&status=done&style=none&taskId=u1ec89e2e-4ea8-47c1-861b-bd8e37ed489&title=&width=1533.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647858814451-2f8f6a50-d90f-4698-9c62-1fdb267d20a6.png#clientId=u11e2dacf-3b06-4&from=paste&height=414&id=u23888a40&originHeight=517&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=117800&status=done&style=none&taskId=u8a8ec0a7-f24c-4a1a-8dbf-852e6df93ba&title=&width=1536)<br />注意观察到有两个数据包名称`file_info`和`uppic`在seasy中查找这两个关键字<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647859042610-353782b9-fb61-421c-997e-72d91506a96f.png#clientId=u11e2dacf-3b06-4&from=paste&height=616&id=ubc10cf50&originHeight=770&originWidth=1418&originalType=binary&ratio=1&rotation=0&showTitle=false&size=191468&status=done&style=none&taskId=u27e97637-091f-43fc-b32a-7ac98d26840&title=&width=1134.4)<br />在文件中看到一个`up_file`函数这个是自定义的函数全局搜索可以看到函数的详细内容<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647860889916-8fd5c1e8-c8f4-41db-a60d-ecddfad51bd3.png#clientId=u11e2dacf-3b06-4&from=paste&height=834&id=u6104c96d&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=414494&status=done&style=none&taskId=ufe0dbcec-2102-4678-920a-db8865c5c44&title=&width=1536)<br />后面发现一个`pathinfo`函数，是一个PHP只带的系统函数
```vue
<?php
$path_parts = pathinfo('/www/htdocs/inc/lib.inc.php');

echo $path_parts['dirname'], "\n";
echo $path_parts['basename'], "\n";
echo $path_parts['extension'], "\n";
echo $path_parts['filename'], "\n";
?> 
以上例程会输出：

/www/htdocs/inc
lib.inc.php
php
lib.inc

```
发现%00上传绕过失败<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647863398355-3e8b5a23-161e-4137-99c5-8938604c30fe.png#clientId=u11e2dacf-3b06-4&from=paste&height=834&id=u02e6aeb4&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=256440&status=done&style=none&taskId=u18a34cec-a90f-41d7-bd70-d148726a69d&title=&width=1536)
<a name="OVikc"></a>
## 测试到有漏洞
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647863461312-59b807e3-24b3-48d4-af9f-8bbb5d83fc6d.png#clientId=u11e2dacf-3b06-4&from=paste&height=394&id=ufef2330e&originHeight=492&originWidth=1697&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115987&status=done&style=none&taskId=u2cca5e32-ce3e-413c-a138-a0a0095b880&title=&width=1357.6)<br />看到这里只是限制了文件的类型，没有其他的限制，也就说这里可能存在MIME类型绕过。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647863865365-636499ce-dfda-4281-adcd-dd264f3a5c40.png#clientId=u11e2dacf-3b06-4&from=paste&height=834&id=ude1e97b8&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=466901&status=done&style=none&taskId=ucfffccde-bc3d-48e8-80f0-2f3f25bcc8b&title=&width=1536)<br />可以看到图片马已经上传成功<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647864171642-312e5055-adde-4e29-bcdb-cc5c92f0da98.png#clientId=u11e2dacf-3b06-4&from=paste&height=834&id=u4b0aa8e9&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=294359&status=done&style=none&taskId=u3b5adda9-07f9-4c1f-a653-3752b18d1fe&title=&width=1536)


