# 第十一天web漏洞

​		前言:本章节将讲解各种WEB层面上的有那些漏洞类型,俱体漏洞的危害等级，以简要的影响范围测试进行实例分析，思维导图中的漏洞也是后面我们将要学习到的各个知识点，其中针对漏洞的形成原理，如何发现，如何利用将是本章节学习的重点内容!

![web-漏洞小迪安全](D:\Note\网络安全\图片\web-漏洞小迪安全.png)

## 简要知识点

~~~ bash
CTE,SRC，红蓝对抗，实战等

#简要说明以上漏洞危害情况
#简要说明以上漏洞等级划分
#简要说明以上漏洞重点内容
#简要说明以上漏洞形势问题
~~~



## 一、pikachu环境搭建

**靶场搭建：https://github.com/zhuifengshaonianhanlu/pikachu ,docker环境**

~~~ bash
[root@oldjiang ~]# docker pull area39/pikachu
[root@oldjiang ~]# docker run -d -p8080:80 area39/pikachu
72ddd9a05d31fdb921765519c413f3f97dbb34560c9c14d9aa59de73e5d6b3eb
~~~



![image70](D:\Note\网络安全\图片\image70.png)

![image71](D:\Note\网络安全\图片\image71.png)

![image72](D:\Note\网络安全\图片\image72.png)



## 二、sql注入之数字注入

![image73](D:\Note\网络安全\图片\image73.png)

![image74](D:\Note\网络安全\图片\image74.png)



在数据库中查看信息

~~~ bash
mysql> select * from member;
+----+----------+----------------------------------+------+-------------+-----------------------+-------------------+
| id | username | pw                               | sex  | phonenum    | address               | email             |
+----+----------+----------------------------------+------+-------------+-----------------------+-------------------+
|  1 | vince    | e10adc3949ba59abbe56e057f20f883e | boy  | 18626545453 | chain                 | vince@pikachu.com |
|  2 | allen    | e10adc3949ba59abbe56e057f20f883e | boy  | 13676767767 | nba 76                | allen@pikachu.com |
|  3 | kobe     | e10adc3949ba59abbe56e057f20f883e | boy  | 15988767673 | nba lakes             | kobe@pikachu.com  |
|  4 | grady    | e10adc3949ba59abbe56e057f20f883e | boy  | 13676765545 | nba hs                | grady@pikachu.com |
|  5 | kevin    | e10adc3949ba59abbe56e057f20f883e | boy  | 13677676754 | Oklahoma City Thunder | kevin@pikachu.com |
|  6 | lucy     | e10adc3949ba59abbe56e057f20f883e | girl | 12345678922 | usa                   | lucy@pikachu.com  |
|  7 | lili     | e10adc3949ba59abbe56e057f20f883e | girl | 18656565545 | usa                   | lili@pikachu.com  |
+----+----------+----------------------------------+------+-------------+-----------------------+-------------------+
~~~



**操作方法**

- 在文件/app/vul/sqli/sqli_id.php第27行下面增加一行 echo $query; 然后保存退出

~~~ bash
	$query="select username,email from member where id=$id";
		echo $query;
~~~



![image75](D:\Note\网络安全\图片\image75.png)



![image76](D:\Note\网络安全\图片\image76.png)



- 打开burp修改数据包

![image77](D:\Note\网络安全\图片\image77.png)



- 获取到数据库信息

![image78](D:\Note\网络安全\图片\image78.png)



## 三、文件遍历漏洞

创建文件dir.php

~~~ bash
root@eb8d8fc8a3e7:/app# pwd
/app
root@eb8d8fc8a3e7:/app# vim dir.php
root@eb8d8fc8a3e7:/app# pwd
/app
root@eb8d8fc8a3e7:/app# cat dir.php
<?php

function my_dir($dir) {
        $files = [];
        if(@$handle = opendir($dir)) {
                while(($file = readdir($handle)) !== false) {
                        if($file != ".." && $file != ".") {
                                if(is_dir($dir . "/" . $file)) { //如果是子文件夹，进行递归
                                        $files[$file] = my_dir($dir . "/" . $file);
                                } else {
                                        $files[] = $file;
                                }
                        }
                }
        closedir($handle);
    }
        return $files;
}

echo "<pre>";
print_r(my_dir("../app"));
echo "</pre>";
root@eb8d8fc8a3e7:/app# chmod +x dir.php
~~~



![image79](D:\Note\网络安全\图片\image79.png)



**备注：**目录遍历漏洞一般由其他的漏洞配合才能实现漏洞的作用。



## 四、文件上传漏洞

![image80](D:\Note\网络安全\图片\image80.png)

~~~ bash
┌──(root💀kali)-[~/桌面]
└─# cat phpinfo.jpg                                                                                                                                                                     2 ⚙
<?php
phpinfo();
?>
~~~

![image81](D:\Note\网络安全\图片\image81.png)

![image82](D:\Note\网络安全\图片\image82.png)

**注意：**文件上传一般是高危漏洞，因为要是上传的是木马文件可以直接拿下服务器。



## 五、文件下载漏洞

右击复制下载地址：http://10.1.1.133:8080/vul/unsafedownload/execdownload.php?filename=kb.png

![image83](D:\Note\网络安全\图片\image83.png)



在网站目录中查看

~~~ bash
root@eb8d8fc8a3e7:/app/vul/unsafedownload/download# pwd
/app/vul/unsafedownload/download
root@eb8d8fc8a3e7:/app/vul/unsafedownload/download# ls
ai.png  bigben.png  camby.png  kb.png  lmx.png  mbl.png  ns.png  oldfish.png  pj.png  rayal.png  sks.png  smallane.png
~~~



修改下载文件

~~~ bash
http://10.1.1.133:8080/vul/unsafedownload/execdownload.php?filename=../unsafedownload.php
~~~

![image84](D:\Note\网络安全\图片\image84.png)