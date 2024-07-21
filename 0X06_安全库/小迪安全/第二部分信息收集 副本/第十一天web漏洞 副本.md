前言:本章节将讲解各种WEB层面上的有那些漏洞类型,俱体漏洞的危害等级，以简要的影响范围测试进行实例分析，思维导图中的漏洞也是后面我们将要学习到的各个知识点，其中针对漏洞的形成原理，如何发现，如何利用将是本章节学习的重点内容!<br />![web-漏洞小迪安全.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623721425978-e14463d7-40a4-4686-99e5-3ac03dfe8c33.png#clientId=u21f8c456-51a2-4&from=paste&id=u46878205&originHeight=901&originWidth=913&originalType=binary&ratio=2&size=110515&status=done&style=none&taskId=u61e034b7-fd69-4809-be97-2c6c850959b)

---

**简要知识点**
```bash
CTE,SRc，红蓝对抗，实战等

#简要说明以上漏洞危害情况
#简要说明以上漏洞等级划分
#简要说明以上漏洞重点内容
#简要说明以上漏洞形势问题
```
<a name="ftASS"></a>
### 一、pikachu环境搭建
**靶场搭建：**[https://github.com/zhuifengshaonianhanlu/pikachu](https://github.com/zhuifengshaonianhanlu/pikachu)<br />**docker环境**
```bash
[root@oldjiang ~]# docker pull area39/pikachu
[root@oldjiang ~]# docker run -d -p8080:80 area39/pikachu
72ddd9a05d31fdb921765519c413f3f97dbb34560c9c14d9aa59de73e5d6b3eb
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623725817931-b6ba0eae-866c-4410-ba2b-0e09edaa7483.png#clientId=u13944bd1-bcd7-4&from=paste&height=334&id=u93ceff76&originHeight=421&originWidth=931&originalType=binary&ratio=2&size=158884&status=done&style=none&taskId=u0fc5e381-96d3-4b7a-8697-ef9e46fe0bf&width=739.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623725840715-17108d47-0daf-4cf9-a4d9-a4b06c01e1e0.png#clientId=u13944bd1-bcd7-4&from=paste&height=210&id=ubbe4d6c6&originHeight=419&originWidth=1478&originalType=binary&ratio=2&size=48482&status=done&style=none&taskId=u3123bf68-92d0-47f5-ad2c-4546163fd82&width=739)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623725858994-fab78038-e51a-439c-98c5-c27fb0bdc951.png#clientId=u13944bd1-bcd7-4&from=paste&height=267&id=u83d3533f&originHeight=534&originWidth=1489&originalType=binary&ratio=2&size=67139&status=done&style=none&taskId=u57a443c7-91e4-4b2c-a61c-e23c070807c&width=744.5)
<a name="n3Ita"></a>
### 二、sql注入之数字注入
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623726082542-3c0ab2c3-ac9d-47d9-aeb7-5eb283e338d4.png#clientId=u13944bd1-bcd7-4&from=paste&height=244&id=ua8945fa4&originHeight=488&originWidth=1612&originalType=binary&ratio=2&size=39647&status=done&style=none&taskId=ub61d7e48-f1ed-43f3-bb4c-34c6f119002&width=806)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623726168437-2aea63ff-47d4-4bb6-b842-2eddfc326037.png#clientId=u13944bd1-bcd7-4&from=paste&height=126&id=u12d462b2&originHeight=251&originWidth=1520&originalType=binary&ratio=2&size=26810&status=done&style=none&taskId=u9ca742ad-cf0c-44dd-b8b6-600e0fec31b&width=760)

在数据库中查看信息
```bash
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

```
**操作方法**

- 在文件/app/vul/sqli/sqli_id.php第27行下面增加一行 echo $query; 然后保存退出
```bash
27 		$query="select username,email from member where id=$id";
28  			echo $query;
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623727035392-8bae692b-b5a8-4950-b78b-1b28a69f0307.png#clientId=u13944bd1-bcd7-4&from=paste&height=26&id=ub35b2099&originHeight=51&originWidth=707&originalType=binary&ratio=2&size=6608&status=done&style=none&taskId=u05a64f85-8589-4b33-a0c3-4e25522b096&width=353.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623727102875-ef393c8b-04ff-48cc-93d2-c1c85b87bd7d.png#clientId=u13944bd1-bcd7-4&from=paste&height=240&id=uf958373f&originHeight=349&originWidth=1089&originalType=binary&ratio=2&size=39642&status=done&style=none&taskId=u00e9e7c7-0e11-4ee0-abd5-cc1c633cf1d&width=749.5)

- 打开burp修改数据包

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623727977772-d60e637e-411b-47da-b0e5-1a83f7aad635.png#clientId=u13944bd1-bcd7-4&from=paste&height=189&id=u94073c4e&originHeight=280&originWidth=1063&originalType=binary&ratio=2&size=43235&status=done&style=none&taskId=uf1290480-e38f-4bf3-ada3-0b36c44af4b&width=718.5)

- 获取到数据库信息

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623727870910-067e4d6b-2a8a-4b38-8eaa-fc5a2de6ea8a.png#clientId=u13944bd1-bcd7-4&from=paste&height=246&id=nhXA9&originHeight=399&originWidth=1173&originalType=binary&ratio=2&size=58759&status=done&style=none&taskId=u809f9ae1-4eda-4ae7-bd50-ebe42838693&width=724.5)
<a name="ZH2D0"></a>
### 三、文件遍历漏洞
创建文件dir.php
```bash
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
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623732208906-1be70b3b-2f4f-49ef-9bd0-64603ddb3051.png#clientId=u13944bd1-bcd7-4&from=paste&height=456&id=u3917c384&originHeight=912&originWidth=669&originalType=binary&ratio=2&size=38929&status=done&style=none&taskId=ud0c6faab-8772-42cc-b0fb-39520a32f75&width=334.5)<br />**备注：**目录遍历漏洞一般由其他的漏洞配合才能实现漏洞的作用。
<a name="jtYs5"></a>
### 四、文件上传漏洞
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623738344797-a82edfa6-f1c3-451a-a8ca-6a2607601c5d.png#clientId=u13944bd1-bcd7-4&from=paste&id=u3ed76088&originHeight=606&originWidth=1191&originalType=binary&ratio=2&size=51358&status=done&style=none&taskId=u51a5d54b-66e0-41aa-aabf-62c0446c4ce)
```bash
┌──(root💀kali)-[~/桌面]
└─# cat phpinfo.jpg                                                                                                                                                                     2 ⚙
<?php
phpinfo();
?>
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623738441797-8fa3377f-832f-40f9-ad58-ad37e08b925e.png#clientId=u13944bd1-bcd7-4&from=paste&height=407&id=uce5ce7f1&originHeight=579&originWidth=1056&originalType=binary&ratio=2&size=72618&status=done&style=none&taskId=u4dd2ad4d-6369-41e6-9f4d-262d15d55b9&width=742)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623738490937-30de4bcb-4317-4c99-a52d-6dad4fe2fb9f.png#clientId=u13944bd1-bcd7-4&from=paste&height=404&id=u86d6b199&originHeight=654&originWidth=1179&originalType=binary&ratio=2&size=164037&status=done&style=none&taskId=ua77a5ff3-16c5-4680-b96f-a8977c712d8&width=727.5)<br />**注意：**文件上传一般是高危漏洞，因为要是上传的是木马文件可以直接拿下服务器。
<a name="HKtMk"></a>
### 五、文件下载漏洞
右击复制下载地址：[http://10.1.1.133:8080/vul/unsafedownload/execdownload.php?filename=kb.png](http://10.1.1.133:8080/vul/unsafedownload/execdownload.php?filename=kb.png)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623739441944-aab00923-c2a4-4830-9e35-24fa47e9548d.png#clientId=u13944bd1-bcd7-4&from=paste&height=437&id=ucb370b58&originHeight=669&originWidth=1115&originalType=binary&ratio=2&size=657558&status=done&style=none&taskId=u53c158ca-34bd-4933-800d-95bf13303d5&width=727.5)<br />在网站目录中查看
```bash
root@eb8d8fc8a3e7:/app/vul/unsafedownload/download# pwd
/app/vul/unsafedownload/download
root@eb8d8fc8a3e7:/app/vul/unsafedownload/download# ls
ai.png  bigben.png  camby.png  kb.png  lmx.png  mbl.png  ns.png  oldfish.png  pj.png  rayal.png  sks.png  smallane.png
```
修改下载文件
```bash
http://10.1.1.133:8080/vul/unsafedownload/execdownload.php?filename=../unsafedownload.php
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623739668728-e80bb7e0-a4c1-44cf-9ec2-cc9785b06bc5.png#clientId=u13944bd1-bcd7-4&from=paste&height=157&id=u59701882&originHeight=157&originWidth=449&originalType=binary&ratio=2&size=8634&status=done&style=none&taskId=ubf5e3a45-e7b1-41c6-aab3-1a028e8ff1f&width=449)

