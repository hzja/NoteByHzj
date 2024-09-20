Linux
<a name="bjt3P"></a>
### 一、inode是什么？
文件储存在硬盘上，硬盘的最小存储单位叫做"扇区"（Sector）。每个扇区储存512字节（相当于0.5KB）。<br />操作系统读取硬盘的时候，不会一个个扇区地读取，这样效率太低，而是一次性连续读取多个扇区，即一次性读取一个"块"（block）。这种由多个扇区组成的"块"，是文件存取的最小单位。"块"的大小，最常见的是4KB，即连续八个 sector组成一个 block。<br />文件数据都储存在"块"中，那么很显然，还必须找到一个地方储存文件的元信息，比如文件的创建者、文件的创建日期、文件的大小等等。这种储存文件元信息的区域就叫做inode，中文译名为"索引节点"。<br />每一个文件都有对应的inode，里面包含了与该文件有关的一些信息。
<a name="lrsiW"></a>
### 二、inode的内容
inode包含文件的元信息，具体来说有以下内容：

- 文件的字节数
- 文件拥有者的User ID
- 文件的Group ID
- 文件的读、写、执行权限
- 文件的时间戳，共有三个：ctime指inode上一次变动的时间，mtime指文件内容上一次变动的时间，atime指文件上一次打开的时间。
- 链接数，即有多少文件名指向这个inode
- 文件数据block的位置

可以用stat命令，查看某个文件的inode信息：
```bash
stat example.txt
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717237-95f89704-40d9-4c9c-88e0-4092a9c6b76a.png#clientId=u3f5d4715-52d8-4&from=paste&id=u7428233e&originHeight=154&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2dcf3175-fe58-402b-88bb-0d504967147&title=)<br />总之，除了文件名以外的所有文件信息，都存在inode之中。至于为什么没有文件名，下面会有详细解释。
<a name="B0P6d"></a>
### 三、inode的大小
inode也会消耗硬盘空间，所以硬盘格式化的时候，操作系统自动将硬盘分成两个区域。一个是数据区，存放文件数据；另一个是inode区（inode table），存放inode所包含的信息。<br />每个inode节点的大小，一般是128字节或256字节。inode节点的总数，在格式化时就给定，一般是每1KB或每2KB就设置一个inode。假定在一块1GB的硬盘中，每个inode节点的大小为128字节，每1KB就设置一个inode，那么inode table的大小就会达到128MB，占整块硬盘的12.8%。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717230-aa299767-0213-4b92-9dfe-5ff10f437f2b.png#clientId=u3f5d4715-52d8-4&from=paste&id=u83501165&originHeight=177&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6d38e746-12ac-4bc9-881a-dc8dd78ea40&title=)<br />查看每个硬盘分区的inode总数和已经使用的数量，可以使用`df`命令。
```bash
df -i
```
查看每个inode节点的大小，可以用如下命令：
```bash
sudo dumpe2fs -h /dev/hda | grep "Inode size"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717218-a8e7ee2e-8cd2-4878-92ae-3baca726104c.png#clientId=u3f5d4715-52d8-4&from=paste&id=ubee41358&originHeight=127&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5cdd931c-8bb4-42d2-bbe9-408a9c7aa42&title=)<br />由于每个文件都必须有一个inode，因此有可能发生inode已经用光，但是硬盘还未存满的情况。这时，就无法在硬盘上创建新文件。
<a name="PkDN1"></a>
### 四、inode号码
每个inode都有一个号码，操作系统用inode号码来识别不同的文件。<br />这里值得重复一遍，Unix/Linux系统内部不使用文件名，而使用inode号码来识别文件。对于系统来说，文件名只是inode号码便于识别的别称或者绰号。<br />表面上，用户通过文件名，打开文件。实际上，系统内部这个过程分成三步：首先，系统找到这个文件名对应的inode号码；其次，通过inode号码，获取inode信息；最后，根据inode信息，找到文件数据所在的block，读出数据。<br />使用`ls -i`命令，可以看到文件名对应的inode号码：
```bash
ls -i example.txt
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717216-0426fa91-5d54-4db0-86fd-b4cd3f12722e.png#clientId=u3f5d4715-52d8-4&from=paste&id=uc84abd35&originHeight=95&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u66daff7a-66f1-4af7-9804-aed4e4d0a9a&title=)
<a name="GOpBf"></a>
### 五、目录文件
Unix/Linux系统中，目录（directory）也是一种文件。打开目录，实际上就是打开目录文件。<br />目录文件的结构非常简单，就是一系列目录项（dirent）的列表。每个目录项，由两部分组成：所包含文件的文件名，以及该文件名对应的inode号码。<br />ls命令只列出目录文件中的所有文件名：
```bash
ls /etc
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717315-3cb2c23e-552e-4f04-8e9c-53a208f5bb41.png#clientId=u3f5d4715-52d8-4&from=paste&id=u01a62725&originHeight=94&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf2e49f1a-bbf2-4961-a78c-189421b73f8&title=)<br />ls -i命令列出整个目录文件，即文件名和inode号码：
```bash
ls -i /etc
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717631-d234a0f8-d874-4d64-b760-b937b3d4b506.png#clientId=u3f5d4715-52d8-4&from=paste&id=u1410629d&originHeight=107&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a2af1a4-478c-4b8f-9b71-ef23b651524&title=)<br />如果要查看文件的详细信息，就必须根据inode号码，访问inode节点，读取信息。`ls -l`命令列出文件的详细信息。
```bash
ls -l /etc
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717590-e0a7aeaa-1dd5-4f4a-b8bf-a68dcd4caa33.png#clientId=u3f5d4715-52d8-4&from=paste&id=u9efb3df1&originHeight=160&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ff87022-8186-4a56-a201-7c00db8ee16&title=)<br />理解了上面这些知识，就能理解目录的权限。目录文件的读权限（r）和写权限（w），都是针对目录文件本身。由于目录文件内只有文件名和inode号码，所以如果只有读权限，只能获取文件名，无法获取其他信息，因为其他信息都储存在inode节点中，而读取inode节点内的信息需要目录文件的执行权限（x）。
<a name="vNFUY"></a>
### 六、硬链接
一般情况下，文件名和inode号码是"一一对应"关系，每个inode号码对应一个文件名。但是，Unix/Linux系统允许，多个文件名指向同一个inode号码。<br />这意味着，可以用不同的文件名访问同样的内容；对文件内容进行修改，会影响到所有文件名；但是，删除一个文件名，不影响另一个文件名的访问。这种情况就被称为"硬链接"（hard link）。<br />ln命令可以创建硬链接：
```bash
ln 源文件 目标文件
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717657-2ab4e82c-8945-458c-a1b6-0aa0b69af8e2.png#clientId=u3f5d4715-52d8-4&from=paste&id=uac353d54&originHeight=207&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubf979ffe-bc40-4f37-b58a-56712feaabe&title=)<br />运行上面这条命令以后，源文件与目标文件的inode号码相同，都指向同一个inode。inode信息中有一项叫做"链接数"，记录指向该inode的文件名总数，这时就会增加1。<br />反过来，删除一个文件名，就会使得inode节点中的"链接数"减1。当这个值减到0，表明没有文件名指向这个inode，系统就会回收这个inode号码，以及其所对应block区域。<br />这里顺便说一下目录文件的"链接数"。创建目录时，默认会生成两个目录项："."和".."。前者的inode号码就是当前目录的inode号码，等同于当前目录的"硬链接"；后者的inode号码就是当前目录的父目录的inode号码，等同于父目录的"硬链接"。所以，任何一个目录的"硬链接"总数，总是等于2加上它的子目录总数（含隐藏目录）。
<a name="jmYcp"></a>
### 七、软链接
除了硬链接以外，还有一种特殊情况。<br />文件A和文件B的inode号码虽然不一样，但是文件A的内容是文件B的路径。读取文件A时，系统会自动将访问者导向文件B。因此，无论打开哪一个文件，最终读取的都是文件B。这时，文件A就称为文件B的"软链接"（soft link）或者"符号链接（symbolic link）。<br />这意味着，文件A依赖于文件B而存在，如果删除了文件B，打开文件A就会报错："No such file or directory"。这是软链接与硬链接最大的不同：文件A指向文件B的文件名，而不是文件B的inode号码，文件B的inode"链接数"不会因此发生变化。<br />ln -s命令可以创建软链接。
```bash
ln -s 源文文件或目录 目标文件或目录
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651191717640-08652822-b4ad-4c7c-9043-c6e9d76efd17.png#clientId=u3f5d4715-52d8-4&from=paste&id=u6b71b75e&originHeight=193&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7907da54-bc14-431f-b076-b697f833d0b&title=)
<a name="uVAH3"></a>
### 八、inode的特殊作用
由于inode号码与文件名分离，这种机制导致了一些Unix/Linux系统特有的现象。

- 有时，文件名包含特殊字符，无法正常删除。这时，直接删除inode节点，就能起到删除文件的作用。
- 移动文件或重命名文件，只是改变文件名，不影响inode号码。
- 打开一个文件以后，系统就以inode号码来识别这个文件，不再考虑文件名。因此，通常来说，系统无法从inode号码得知文件名。

第3点使得软件更新变得简单，可以在不关闭软件的情况下进行更新，不需要重启。因为系统通过inode号码，识别运行中的文件，不通过文件名。更新的时候，新版文件以同样的文件名，生成一个新的inode，不会影响到运行中的文件。等到下一次运行这个软件的时候，文件名就自动指向新版文件，旧版文件的inode则被回收。
