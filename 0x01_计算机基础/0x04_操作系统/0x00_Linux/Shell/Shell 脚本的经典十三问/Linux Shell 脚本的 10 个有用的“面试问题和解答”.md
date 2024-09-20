Linux<br />Linux 的浩瀚无垠，使人总能每次都提交与众不同的内容。这些内容不仅对他们的职业生涯很有用，同时也让他们增长知识。<br />在此，作为 shell 脚本的附加内容，在本文中将从面试的角度解读与 Linux Shell 相关的问题。
<a name="UZ5T3"></a>
### 1、在 shell 脚本成功执行前，如何中断脚本执行？
解答：需要使用`exit`命令来实现以上描述的情境。exit命令被强制输出非0值时，脚本会报错并退出。在 Unix 环境下的 shell 脚本中，0值表示成功执行。因此，在脚本终止前执行一个不带引号的exit -1命令将使脚本中止。
```bash
#!/bin/bash
echo "Hello"
exit -1
echo "bye"
```
保存文件并执行。
```bash
# sh linuxmi.sh
Hello
linuxmi.sh:行3: exit-1: 未找到命令
bye
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679033811117-30cc7eef-c0c4-4db2-9eba-ff84eba1fc55.png#averageHue=%23272c2e&clientId=u8f4fee36-2264-4&from=paste&id=ub49f70fa&originHeight=147&originWidth=734&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u384268dd-1aa5-440a-8c17-58a2825d7c1&title=)<br />从上面的脚本中可以清楚地看到，在 exit -1 命令前，脚本执行得很好。
<a name="HI89G"></a>
### 2、如何使用 Linux 命令来移除文件头？
解答：当需要删除文件中的指定行时，sed命令可以用来解决该问题。<br />这个是用来删除文件头（文件的首行）的正确命令。
```bash
sed '1 d' file.txt
```
好吧，其实sed命令内建的`-i`开关就可以干这活，就不需要重定向符了吧。
```bash
sed -i '1 d' file.txt
```
<a name="ZHlkL"></a>
### 3、怎么检查一个文本文件中某一行的长度？
解答：sed命令也可以用来查找文本文件中的某一行或者检查其长度。<br />`sed -n 'n p' file.txt`可以解决，这里n表示行号，p打印出匹配内容（到标准输出），该命令通常与`-n`命令行选项连用。那么，怎样来获取长度计数呢？很明显，需要通过管道输出给wc命令来计算。
```bash
sed –n 'n p' file.txt | wc –c
```
要得到文本文件‘linuxmi.txt’的第五行的长度，运行如下命令：
```bash
sed -n '5 p' linuxmi.txt | wc -c
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679033811112-51985faf-b60c-4753-8299-25decbe15401.png#averageHue=%23282c2f&clientId=u8f4fee36-2264-4&from=paste&id=u09b5ef69&originHeight=129&originWidth=734&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uef933694-d8fa-4415-ac04-72124edd7ed&title=)
<a name="UmZ8g"></a>
### 4、可以在 Linux 系统上查看到所有非打印字符吗？怎么做到？
解答：可以。可以在 Linux 中查看所有的非打印字符。要实现上面所讲的方案，需要vi编辑器的帮助。怎样在vi编辑器中显示非打印字符？<br />打开vi编辑器。<br />先按[esc]键，然后按：进入到vi编辑器的命令模式。<br />最后，从 vi 编辑器的命令界面输入set list命令并执行。<br />注：这种方式可以查看文本文件中的所有非打印字符，包括 ctrl+m（^M）。
<a name="dLU14"></a>
### 5、假如你是一个员工组的团队领导，为xyz公司工作。公司要求你创建一个dir_xyz目录，让该组成员都能在该目录下创建或访问文件，但是除了文件创建者之外的其他人不能删除文件，你会怎么做？
解答：这真是个有趣的工作方案。好吧，上面所讲的方案，需要通过下面的步骤来实施，这简直就是小菜一碟。
```bash
mkdir dir_xyz
chmod g+wx dir_xyz
chmod +t dir_xyz
```
第一行命令创建了一个目录（dir_xyz），上面的第二行命令让组（g）具有‘写’和‘执行’的权限，而上面的最后一行命令——权限位最后的‘+t’是‘粘滞位’，它用来替换‘x’，表明在这个目录中，文件只能被它们的拥有者、目录的拥有者或者是超级用户root删除。
<a name="pG18w"></a>
### 6、一个Linux进程经历的各个阶段吗？
解答：一个 Linux 进程在它的一生中，通常经历了四个主要阶段。<br />这里是Linux进程要经历的四个阶段。

- 等待：Linux进程等待资源。
- 运行：Linux进程当前正在执行中。
- 停止：Linux进程在成功执行后或收到杀死进程信号后停止。
- 僵尸：如果该进程已经结束，但仍然留在进程表中，被称为‘僵尸’。
<a name="cZwjS"></a>
### 7、Linux中cut命令怎么用？
解答：cut是一个很有用的 Linux 命令，当要截取文件的指定部分并打印到标准输出，当文本区域以及文件本身很大时，这个命令很有用。<br />例如，截取txt_linuxmi文件的前10列。
```bash
cut -c1-10 txt_linuxmi
```
要截取该文件中的第二，第五和第七列。
```bash
cut -d;-f2 -f5 -f7 txt_linuxmi
```
<a name="hsl4R"></a>
### 8、cmp和diff命令的区别是什么？
解答：cmp和diff命令用来获取相同的东西，但各有侧重。<br />diff命令输出为了使两个文件一样而应该做的修改。而‘cmp’命令则将两个文件逐字节对比，并报告第一个不匹配的项。
<a name="W0ZxH"></a>
### 9、可以用echo命令来替换ls命令吗？
解答：可以的。`ls`命令可以用`echo`命令来替代。`ls`命令列出目录内容，从替换上述命令的角度讲，可以使用`echo *`，两个命令的输出完全一样。
<a name="hPO4O"></a>
### 10、能简要描述一下 inode 吗？
解答：inode 是一个数据结构，在 Linux 上用于文件标识。每个文件在 Unix 系统上有一个独立的 inode 和一个唯一的 inode 号。
