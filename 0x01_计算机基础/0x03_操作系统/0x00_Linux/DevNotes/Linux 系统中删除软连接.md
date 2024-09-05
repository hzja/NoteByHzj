Linux<br />在Linux中，符号链接（symbolic link，或者symlink）也称为软链接，是一种特殊类型的文件，用作指向另一个文件的快捷方式。<br />可以使用 ln 命令来创建软连接，那么怎样删除呢？<br />Linux 中没有用于删除符号链接的特殊命令，不过可以使用 rm 命令，该命令主要用于删除文件和目录，如下：
```bash
rm symbolic_link_name
```
当然也可以使用 `unlink` 命令，但是不要直接使用软链接的名称，因为它不仅仅用于删除链接，还会删除文件。
```bash
unlink symbolic_link_name
```
下面来详细看一下。
<a name="vyoh7"></a>
## 使用 rm 命令删除符号链接
在 rm 命令后面提供链接路径的名称：
```bash
rm name_or_path_of_link
```
来看个具体例子，如下命令中列出当前路径中的文件：
```bash
$ ls -l
total 4708
-rw-rw-r-- 1 abhishek abhishek 4794657 Sep 27 20:36 export.json
-rw-rw-r-- 1 abhishek abhishek     311 Sep 22 12:19 line.txt
lrwxrwxrwx 1 abhishek abhishek      26 Oct 17 11:24 mylink -> ./Documents/sample-mark.md
-rw-rw-r-- 1 abhishek abhishek     106 Sep 27 20:39 redirects.json
-rw-r--r-- 1 abhishek abhishek   12817 Sep 22 12:28 sample.txt
```
上述列表中 mylink 为符号链接，大家可以看到其是以 l （表示链接）开头，名称显示它指向的文件。<br />使用 rm 删除它，并验证：
```bash
$ rm mylink 
$ ls -l
total 4708
-rw-rw-r-- 1 abhishek abhishek 4794657 Sep 27 20:36 export.json
-rw-rw-r-- 1 abhishek abhishek     311 Sep 22 12:19 line.txt
-rw-rw-r-- 1 abhishek abhishek     106 Sep 27 20:39 redirects.json
-rw-r--r-- 1 abhishek abhishek   12817 Sep 22 12:28 sample.txt
```
如下图所示，源文件仍然存在：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668920975554-35b8131d-f832-4687-bed4-8fd115963834.png#averageHue=%23202427&clientId=u5b4b8a32-c914-4&from=paste&id=u238ce005&originHeight=329&originWidth=883&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e167c21-1994-4aae-86dc-475b4db8229&title=)
<a name="QwGD8"></a>
## 删除多个链接
可以使用 rm 命令一次删除多个符号链接：
```bash
rm symlink1 symlink2 symlink3
```
<a name="zc5Ff"></a>
### 使用 unlink 命令删除软链接
删除软链接的另一种方法是使用 unlink 命令。这个命令乍听起来好像只用于删除链接，但其实它也可以删除文件。使用方式如下：<br />使用上面的例子，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668920975450-e659accc-3c9f-4292-a10e-d185fe2d6542.png#averageHue=%23202327&clientId=u5b4b8a32-c914-4&from=paste&id=u650d7707&originHeight=295&originWidth=886&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u86e73926-ce85-4187-98c4-810c3c81b9d&title=)<br />unlink 不能一次删除多个链接。
<a name="iZA19"></a>
### 删除指向目录的软链接
可以创建指向文件的软连接，也可以创建指向目录的软链接。删除目录需要使用 `-r` 选项，但是删除指向目录的链接却不需要使用 `-r` 选项。<br />使用方法与删除文件链接的方法相同：
```bash
rm name_or_path_to_link_to_dir
```
删除指向目录的软连接的时候，在目录的末尾不要添加斜杠，否则会报错：<br />下图是一个删除目录软链接的示例：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668920975550-b24fad22-7ed4-4882-b3a7-31669c6fabb9.png#averageHue=%23202528&clientId=u5b4b8a32-c914-4&from=paste&id=u26e83056&originHeight=367&originWidth=786&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6a0079fe-dc75-404b-aa15-7686ccab0dd&title=)<br />注意，尽量不要使用 `-f` 选项强制删除指向目录的链接，因为它会删除目录的内容。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668920975573-185b690c-dbb9-48b9-aca2-50d1b5ce65e4.png#averageHue=%23202428&clientId=u5b4b8a32-c914-4&from=paste&id=ub240ff8e&originHeight=352&originWidth=821&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf57a30c4-9ea1-4ed5-bdd9-994250a2875&title=)
<a name="urUwV"></a>
## 删除硬链接
与软链接不同，硬链接与原始文件几乎无法区分，只能通过 inode number 注意到它。<br />看下面的例子：
```bash
$ ls -li
total 4716
 544057 -rw-rw-r-- 1 abhishek abhishek 4794657 Sep 27 20:36 export.json
 544884 -rw-rw-r-- 2 abhishek abhishek     311 Sep 22 12:19 hard_link
 544884 -rw-rw-r-- 2 abhishek abhishek     311 Sep 22 12:19 line.txt
1181365 drwxrwxr-x 2 abhishek abhishek    4096 Oct 17 12:33 my_dir
 546419 -rw-rw-r-- 1 abhishek abhishek     106 Sep 27 20:39 redirects.json
```
删除硬链接与删除文件相同：<br />删除链接文件会怎样呢？<br />实际上，几乎没有在删除软链接的同时，也要删除原始文件的场景和需求。当然，如果有的话，可以通过符号链接找到原始文件，然后删除它。<br />注意，如果删除源文件，但是没有删除软链接，那么这个链接将会成为断开的或悬空的链接。<br />虽然存在 unlink 命令，但是建议使用 rm 命令来删除符号链接。因为这个是都比较熟悉的命令，经常使用它来删除文件，当然也可以用来删除符号链接。
