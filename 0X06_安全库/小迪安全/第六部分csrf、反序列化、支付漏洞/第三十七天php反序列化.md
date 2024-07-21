
<a name="Z3lVq"></a>
### 一、思维导图
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629513452361-1d1b230d-7fad-4387-ae68-76a3da18a08c.png#clientId=ud79b99db-0a31-4&from=paste&id=ua7bb86a6&originHeight=780&originWidth=1136&originalType=url&ratio=1&status=done&style=none&taskId=u74be46eb-2814-4c75-8c52-130ecd71b6c)<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629513462778-724f3c0d-a1f6-412c-8cfa-4f4cac6d4586.png#clientId=ud79b99db-0a31-4&from=paste&id=u5155ba91&originHeight=443&originWidth=808&originalType=url&ratio=1&status=done&style=none&taskId=uf1fabeba-a926-4874-b612-8a6d908cfe7)
<a name="dju9d"></a>
### 二、PHP反序列化
```bash
#PHP 反序列化
原理：未对用户输入的序列化字符串进行检测，导致攻击者可以控制反序列化过程，从而导致代码
执行，SQL 注入，目录遍历等不可控后果。在反序列化的过程中自动触发了某些魔术方法。当进行
反序列化的时候就有可能会触发对象中的一些魔术方法。

serialize() //将一个对象转换成一个字符串
unserialize() //将字符串还原成一个对象

触发：unserialize 函数的变量可控，文件中存在可利用的类，类中有魔术方法：

参考：https://www.cnblogs.com/20175211lyz/p/11403397.html

__construct()	//创建对象时触发
__destruct() 	//对象被销毁时触发
__call() 			//在对象上下文中调用不可访问的方法时触发
__callStatic() //在静态上下文中调用不可访问的方法时触发
__get() 			//用于从不可访问的属性读取数据
__set() 			//用于将数据写入不可访问的属性
__isset() 		//在不可访问的属性上调用 isset()或 empty()触发
__unset() 		//在不可访问的属性上使用 unset()时触发
__invoke() 		//当脚本尝试将对象调用为函数时触发

```
**无类测试**

- serialize

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629702598464-5fc46fbc-165e-4fe4-94ac-903265291b3f.png#clientId=u2df87704-8cdc-4&from=paste&height=183&id=uf6ce5ff2&originHeight=366&originWidth=1681&originalType=binary&ratio=1&size=61014&status=done&style=none&taskId=u20920bb8-9421-4d0c-aa77-dca3d83e212&width=840.5)

- unserialize

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629702950773-dd685602-5640-4632-9b1b-ad2eb8b95701.png#clientId=u2df87704-8cdc-4&from=paste&height=176&id=u01028557&originHeight=351&originWidth=1671&originalType=binary&ratio=1&size=57518&status=done&style=none&taskId=ufe4d5f23-4130-4cec-b0f6-df9d3160dc8&width=835.5)<br />**有类测试**
```bash
<?php
	class ABC{
    public $test;
    function __construct(){
        $test = 1;
        echo '调用了构造函数<br>';
    }
    function __destruct(){
        echo '调用了析构函数<br>';
    }
    function __wakeup(){
        echo '调用了苏醒函数<br>';
    }
}
echo '创建对象a<br>';
$a = new ABC;
echo '序列化<br>';
$a_ser=serialize($a);
echo '反序列化<br>';
$a_unser=unserialize($a_ser);
echo '对象快要死了！';
?>

运行结果
创建对象a<br>调用了构造函数<br>序列化<br>反序列化<br>调用了苏醒函数<br>对象快要死了！调用了析构函数<br>调用了析构函数<br>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629706689599-8340d273-a326-4ca3-81f0-1464a9e6c3dc.png#clientId=u2df87704-8cdc-4&from=paste&height=300&id=uf64b87e1&originHeight=600&originWidth=1920&originalType=binary&ratio=1&size=103725&status=done&style=none&taskId=u9a50ce60-40a9-49bd-ae05-61325ce84b2&width=960)
<a name="mE3E3"></a>
### 三、ctf真题bugku
[https://ctf.bugku.com/challenges#flag.php](https://ctf.bugku.com/challenges#flag.php)<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629704253019-9ac23b61-ebf7-46e0-80fa-03f8871f78df.png#clientId=u2df87704-8cdc-4&from=paste&id=ud7cf47bf&originHeight=372&originWidth=780&originalType=url&ratio=1&status=done&style=none&taskId=ud85c4732-de9a-4fad-b971-daa9750b1f6)<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629704287765-4a0d78dc-5d8b-443e-bbc3-a80b916afc8b.png#clientId=u2df87704-8cdc-4&from=paste&id=ud59ce7f6&originHeight=451&originWidth=689&originalType=url&ratio=1&status=done&style=none&taskId=u0ac882ca-edf1-4906-8490-0b9f587e463)<br />但是flag没有显示，原因是上面的请求了Hint。看源代码中的if和elseif，是这里的原因。<br />但是删除了?hint=1111之后，再请求，发现还是不对。原因是代码执行顺序的问题。这又是一个坑。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629704316818-2b4f73ca-1d3e-4c09-a484-25dd7c79b0aa.png#clientId=u2df87704-8cdc-4&from=paste&id=ufe7f2742&originHeight=633&originWidth=748&originalType=url&ratio=1&status=done&style=none&taskId=uc5648a49-4a4c-4977-abc4-58529a401f9)](https://gitee.com/darkerg/article-images/raw/master/typora/20210502120317.png)<br />所以我们传入Cookie的值应该为：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629704317436-5ca697f0-0e98-4e3e-923d-10c3a52c368a.png#clientId=u2df87704-8cdc-4&from=paste&id=ub6274a5c&originHeight=527&originWidth=544&originalType=url&ratio=1&status=done&style=none&taskId=ua1d3c9c5-cd22-4959-8c0c-c9dbf965891)](https://gitee.com/darkerg/article-images/raw/master/typora/20210502120442.png)<br />修改cookie<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629704317590-0203cfb1-4cd5-4f09-8668-af89495a543c.png#clientId=u2df87704-8cdc-4&from=paste&id=u1c6e1335&originHeight=136&originWidth=555&originalType=url&ratio=1&status=done&style=none&taskId=ud71b8afd-9fd4-43de-bc4e-e92da66be28)](https://gitee.com/darkerg/article-images/raw/master/typora/20210502120525.png)<br />得到flag<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629704317740-29d6a29f-7381-4c42-9f83-3d7915bd6c8c.png#clientId=u2df87704-8cdc-4&from=paste&id=u28d4677b&originHeight=168&originWidth=348&originalType=url&ratio=1&status=done&style=none&taskId=ub888b76f-356d-4699-8d15-97c66a4d538)](https://gitee.com/darkerg/article-images/raw/master/typora/20210502120504.png)

