<a name="QH0fl"></a>
## 魔术常量
```
<?php
echo '这是第 " '  . __LINE__ . ' " 行';
?>

这是第 “ 2 ” 行
```

```
<?php
echo '该文件位于 " '  . __FILE__ . ' " ';
?>

该文件位于 “ E:\wamp\www\test\index.php ”
```
```

<?php
echo '该文件位于 " '  . __DIR__ . ' " ';
?>
该文件位于 “ E:\wamp\www\test ”
```
<a name="xJTd7"></a>
## 命令执行
shell_exec<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682433784203-20284b3b-da23-49e8-b28a-ae7d80f035e1.png#averageHue=%23fbfbf9&clientId=u00170d8a-a649-4&from=paste&height=424&id=uf45fe303&originHeight=530&originWidth=598&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=28754&status=done&style=none&taskId=u4f0609f9-6124-4700-ba73-3a127b4ef5e&title=&width=478.4)

<a name="ryF0L"></a>
## SQL注入
SELECT<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682433913121-ea4a7666-132a-4a1a-b09e-d2f8e5bc546d.png#averageHue=%23fcfcfc&clientId=u00170d8a-a649-4&from=paste&height=606&id=u47d98833&originHeight=757&originWidth=902&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=38452&status=done&style=none&taskId=ubcecafd8-7825-45f9-a6fe-5655240372a&title=&width=721.6)

<a name="iedRh"></a>
## 文件上传
$_FILES<br />move_uploaded_file	<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682433942323-0ef6f182-9963-4687-9937-230cc1c1dede.png#averageHue=%23fdfcfa&clientId=u00170d8a-a649-4&from=paste&height=400&id=u8f300040&originHeight=500&originWidth=805&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=28865&status=done&style=none&taskId=ue3f7ef58-fe81-4825-9edd-fb2967582b7&title=&width=644)

 Laravel框架  文件上传<br />Storage::disk<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682520508789-d37b9a90-2819-45b1-8065-13c777516b60.png#averageHue=%23f7f5f3&clientId=uc0a34ac8-b040-4&from=paste&height=84&id=u3b1d6524&originHeight=105&originWidth=703&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=10110&status=done&style=none&taskId=uc6c1e258-486b-48c1-8b5c-6cfb3910432&title=&width=562.4)
<a name="cx0TS"></a>
## 文件读取代码
file_get_contents
```php
<?php
// The page we wish to display
$file = $_GET[ 'page' ];
// Read the contents of the file
$content = file_get_contents($file);
// Output the contents
echo $content;
?>
```

```php
<?php
$myfile = fopen("webdictionary.txt", "r") or die("Unable to open file!");
echo fread($myfile,filesize("webdictionary.txt"));
fclose($myfile);
?>
```
<a name="bMwuq"></a>
## 文件包含代码
include():   执行到include()函数时才包含文件，当找不到文件时会产生告警，然后继续执行后续脚本。<br />require(): 与include()的区别在于当找不到文件时，会产生致命错误，并停止脚本。<br />include_once()：和Include()函数相同的作用，只不过若文件已经被包含，则不会再次包含。<br />require_once(): 和require文件相同的作用，若文件已经被包含，则不会再次包含。



include
```php
<?php


// The page we wish to include
$file = $_GET[ 'page' ];


// Include the contents of the file
include $file;


?>
```

**远程文件包含：**<br />与本地文件包含的区别在于能够包含我们的远程服务器上的文件。该漏洞的存在具有一定的条件限制：

- PHP.ini中配置allow_url_fopen=On（默认off）
- PHP.ini中配置allow_url_include=On（默认为off）

伪协议
```
file:// — 访问本地文件系统
http:// — 访问 HTTP(s) 网址
ftp:// — 访问 FTP(s) URLs
php:// — 访问各个输入/输出流（I/O streams）
zlib:// — 压缩流
data:// — 数据（RFC 2397）
glob:// — 查找匹配的文件路径模式
phar:// — PHP 归档
ssh2:// — Secure Shell 2
rar:// — RAR
ogg:// — 音频流
expect:// — 处理交互式的流
```
<a name="fhECA"></a>
## 文件下载
readfile
```php
$file_url = 'http://example.com/file.php';
header('Content-Type: application/octet-stream');
header("Content-Transfer-Encoding: Binary"); 
header("Content-disposition: attachment; filename=\"" . basename($file_url) . "\""); 
readfile($file_url);
```
<a name="rADCs"></a>
## 文件写入
```php
$file = fopen("path/to/file.txt", "w");
fwrite($file, "Hello World!");
fclose($file);
```
<a name="Zx03y"></a>
## 代码执行
eval执行代码assert()，当然还有回调函数比如call_user_func() array_map()
```php
<?
$id = $_GET['x'];
eval($id);
?>


test.php?x=phpinfo();
```
<a name="hYDif"></a>
## 任意文件删除
unlink<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682437236271-cbbbd7b9-5800-4e5c-8226-5fa052d0471b.png#averageHue=%232a2b24&clientId=uc0a34ac8-b040-4&from=paste&height=358&id=u028f75ad&originHeight=448&originWidth=898&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=120965&status=done&style=none&taskId=u3bb64275-7578-45b3-acf3-dfda1ce76a8&title=&width=718.4)


<a name="rIIOf"></a>
## php框架类型
<a name="Nzoy9"></a>
### laravel
[https://learnku.com/docs/laravel/8.5/structure/10361](https://learnku.com/docs/laravel/8.5/structure/10361)<br />路由架构<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682441198360-a8c81705-c184-4e1d-b448-94bd0502fdc3.png#averageHue=%23f6f6f5&clientId=uc0a34ac8-b040-4&from=paste&height=532&id=u7803198d&originHeight=665&originWidth=646&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=33179&status=done&style=none&taskId=u26f20de9-0bd2-4cb7-9476-5d5bfbdfbd2&title=&width=516.8)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682441522252-837cb65c-dc9f-46b5-9f15-ad6499c1aa97.png#averageHue=%2338362a&clientId=uc0a34ac8-b040-4&from=paste&height=370&id=u4eabd99c&originHeight=463&originWidth=1335&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=242313&status=done&style=none&taskId=u43f5ad6c-8eac-4a89-a045-5caa268171f&title=&width=1068)


<a name="H43H2"></a>
### thinkphp5
[http://www.360doc.com/content/22/0429/11/39821762_1028857092.shtml](http://www.360doc.com/content/22/0429/11/39821762_1028857092.shtml)<br />一般开发中，代码都是写在application这个文件夹中开发一个项目的时候，通常分为前台和后台，前台一般放在index模块中，后台一般放在admin模块中。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682521653453-ac13b5e3-506a-4666-b517-aa4662f7d86b.png#averageHue=%23aaba99&clientId=uc0a34ac8-b040-4&from=paste&height=726&id=u3e0712f9&originHeight=908&originWidth=860&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=225678&status=done&style=none&taskId=ud5022990-94ab-40a4-b86f-5fca6044d69&title=&width=688)



1. PATH_INFO模式 : http://127.0.0.1/public/index.php/模块/控制器/操作/(参数名)/(参数值)...
2. 兼容模式 : http://127.0.0.1/public/index.php?s=/模块/控制器/操作&(参数名)=(参数值)...

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682522565470-eaf2e4b3-0664-40b2-bf83-a2a6282f73e9.png#averageHue=%23faf5e9&clientId=uc0a34ac8-b040-4&from=paste&height=753&id=u004789bf&originHeight=941&originWidth=1148&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=130838&status=done&style=none&taskId=u2ea72a33-1797-4f53-ab2b-7560960d15f&title=&width=918.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1682521892623-de7a49d7-cfb9-4887-abcb-a2cd8cfac420.png#averageHue=%23a9a8a8&clientId=uc0a34ac8-b040-4&from=paste&height=376&id=ub52f46d8&originHeight=470&originWidth=862&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=42491&status=done&style=none&taskId=u39639aee-35f1-4fec-8687-356930ed5f8&title=&width=689.6)



[https://www.hacking8.com/MiscSecNotes/php/framework.html](https://www.hacking8.com/MiscSecNotes/php/framework.html) <br />[https://www.tmxsaidno.com/posts/95642275.html](https://www.tmxsaidno.com/posts/95642275.html)
