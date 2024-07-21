正则表达式使用单个字符串来描述、 匹配一系列符合某个语法规则的字符串。 在很多文本编辑器里， 正则表达式通常被用来检索、 替换那些符合某个模式的文本。 在 Linux 中， grep，sed， awk 等文本处理工具都支持通过正则表达式进行模式匹配。
<a name="FZjow"></a>
## 常规匹配
例如下面的命，会对/etc/passwd的每行内容进行匹配，将含有root的行输出，如图中红色的部分即为匹配的内容
```bash
[root@test001 ~]# cat /etc/passwd | grep root
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693066217825-782d0f08-0eb1-400f-bfad-e591aaac1203.png#averageHue=%23404040&clientId=uc6fb52ca-b211-4&from=paste&height=88&id=ub275553e&originHeight=221&originWidth=1342&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=121793&status=done&style=none&taskId=u7d7dec0a-607f-424e-a178-92c34b812ab&title=&width=536.8)
<a name="NILIt"></a>
## 常用的特殊字符串
<a name="UVGgb"></a>
### 1）特殊字符：`^`
^匹配一行的开头，如下，会匹配/etc/passwd中以root开头的行
```bash
[root@test001 ~]# cat /etc/passwd | grep ^root
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693066177227-0fa8ae02-688d-4a1b-9320-9ffb517ff903.png#averageHue=%233d3d3d&clientId=uc6fb52ca-b211-4&from=paste&height=72&id=u2a3b0a29&originHeight=181&originWidth=1314&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=103345&status=done&style=none&taskId=ua257ed4a-dc85-4509-bd14-27f67d8ab27&title=&width=525.6)
<a name="z43nD"></a>
### 2）特殊字符：`$`
$匹配一行的结尾，如下，会匹配出/etc/passwd中以bash结尾的行
```bash
[root@test001 ~]# cat /etc/passwd | grep bash$
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693066153750-154b6712-ba6f-4fcd-b0aa-d1b1066a994a.png#averageHue=%23454545&clientId=uc6fb52ca-b211-4&from=paste&height=148&id=u2fa4b1ae&originHeight=369&originWidth=1317&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=189369&status=done&style=none&taskId=u6092e70d-8b01-42f1-a00b-db4abfcd4b4&title=&width=526.8)
<a name="NkFsM"></a>
### 3）特殊字符：`.`
.匹配一个任意字符，如下，可以匹配含有四个字符，其中第1个字符是r，第4个字符是t的行，中间两个可以是任意字符
```bash
[root@test001 ~]# cat /etc/passwd | grep r..t
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693066094067-2d9403d3-c40f-4384-80aa-03e8aa15b341.png#averageHue=%23414141&clientId=uc6fb52ca-b211-4&from=paste&height=110&id=udcf83500&originHeight=275&originWidth=1337&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=148913&status=done&style=none&taskId=u1243521c-52e9-4cdb-94ae-d72d1d97a99&title=&width=534.8)
<a name="Cnmpe"></a>
### 4）特殊字符：`*`
*不单独使用，他和上一个字符连用，表示匹配上一个字符0次或多次，如下，可以匹配第1个字符是r，最后1个字符是t，中间可以包含任意数量的o
```bash
[root@test001 ~]# cat /etc/passwd | grep ro*t
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693066031380-520b6ebb-d1b5-4aed-8099-86de42320863.png#averageHue=%233e3e3e&clientId=uc6fb52ca-b211-4&from=paste&height=94&id=u2bc1d50f&originHeight=236&originWidth=1366&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=131599&status=done&style=none&taskId=ua6db81ef-5cdb-4b85-adec-2d3152eb7b3&title=&width=546.4)
<a name="wif2R"></a>
### 5）字符区间（中括号）：`[]`
[]表示匹配某个范围内的一个字符，例如

| **[6,8]** | **匹配6或者8** |
| --- | --- |
| [0-9] | 匹配1个0-9的数字 |
| [0-9]* | 匹配任意长度的数字字符串 |
| [a-z] | 匹配1个a-z之间的字符 |
| [a-z]* | 匹配任意长度的字母字符串 |
| [a-ce-f] | 匹配a-z或者e-f之间的任意一个字符 |

```bash
[root@test001 ~]# cat /etc/passwd | grep "[a-z]*"
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693065980035-a30598ed-a12e-4293-b53e-bc2604908981.png#averageHue=%23353535&clientId=uc6fb52ca-b211-4&from=paste&height=177&id=ufbe1d623&originHeight=442&originWidth=1549&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=288232&status=done&style=none&taskId=u65c9d205-3c90-4649-8df6-817b2bca979&title=&width=619.6)
<a name="ie8wV"></a>
### 6）特殊字符：`\`
`\`表示转义，并不会单独使用，由于所有特殊字符都有其特定的匹配模式，当匹配某一特殊字符本身时（例如，我想找出所有包含‘$’的行），就会碰到困难，此时就要将转义字符和特殊字符连用，来表示特殊字符本身，例如
```bash
[root@test001 ~]# echo "abc$" | grep "\\$"
abc$
[root@test001 ~]# echo "abc$" | grep '\$'
abc$
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693065934088-831a3bc4-0dbd-4caa-bfe2-d337a1afba4c.png#averageHue=%233b3b3b&clientId=uc6fb52ca-b211-4&from=paste&height=110&id=u17f6d6e0&originHeight=274&originWidth=1285&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=137210&status=done&style=none&taskId=u5353c2fd-bc42-4dc1-9463-e6a62ac3933&title=&width=514)
<a name="Quo88"></a>
### 7）扩展的正则表达式
这里说一下，有些语言中支持正则表达式的扩展语法，如`{n,m}`等等，对于这种，使用grep匹配的时候，需要添加`-E`选项，否则无法识别这种语法。<br />比如下面2行，匹配a-z中的任意字符2或者3次
```bash
[root@test001 ~]# echo "ab,cdre" | grep "[a-z]{2,3}"
[root@test001 ~]# echo "ab,cdre" | grep -E "[a-z]{2,3}"
ab,cdre
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693065872709-b8d0cc69-8770-470d-af0b-b651726cbb3c.png#averageHue=%233d3d3d&clientId=uc6fb52ca-b211-4&from=paste&height=89&id=u11202ee7&originHeight=223&originWidth=1581&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=135605&status=done&style=none&taskId=u2c504284-36de-4305-9031-c1ddfbd6d3d&title=&width=632.4)
