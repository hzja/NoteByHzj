Linux <br />在一行中运行两个或多个命令可以节省大量时间，并在Linux中提高效率。在Linux中，有三种方法可以在一行中运行多个命令。<br />`;` Command 1 ; Command 2 首先运行Command1，然后运行Command2<br />`&&` Command 1 && Command 2 当Command1运行成功并结束，然后运行Command2<br />`||` Command 1 || Command 2 当Command1运行失败时才运行Command2
<a name="FEZzn"></a>
### 使用 ; 符号运行多条命令
其中最简单的是分号(;)，使用方式如下：
```bash
cmd1; cmd2; cmd3
```
cmd1将首先运行，不管cmd1运行成功还是出现错误，cmd2都会在它之后运行，当cmd2命令完成时，cmd3将会运行
```bash
$ mkdir /root/new_folder; cd /root/new_folder; touch test_file; pwd ; ls -l
/root/new_folder
total 0
-rw-r--r--. 1 root root 0 Sep  2 22:01 test_file
[root@localhost new_folder]#
```
在上面命令中，首先mkdir创建了一个文件夹在/root目录中，cd 进入前面创建的目录，touch创建一个空文件，然后pwd显示当前位置，ls -l查看目录中的文件。
:::info
分号(;)后面的空格可带可不带，但空格使命令行更容易读懂。
:::
<a name="G3YAI"></a>
### 使用 && 符号运行多条命令
有时候希望确保Linux命令中，只有在前一个命令成功结束时，下一个命令才会运行。这就是逻辑和运算符`&&`出现的地方：`cmd1 && cmd2 && cmd3`<br />如果使用基于CentOS的发行版，一定遇到过这个使用&&概念的命令:
```bash
$ yum makecache && yum -y update
```
命令行汇总的前半部分时更新缓存，后半部分更新所有安装包。<br />举个例子，如果new_folder已经存在，那么mkdir命令将返回错误。
:::danger
; 和&& 的差异：<br />当第一个命令出现错误时，`&&`分隔的命令会停止执行后面的命令。
:::
<a name="324b4f1a"></a>
### 使用 || 符号运行多条命令
可以使用逻辑操作符(||)运行命令行，但是只有在前一个命令出现错误时才运行下一个命令：
```bash
cmd1 || cmd2 || cmd3
```
如果cmd1运行失败，则运行cmd2。如果cmd2运行成功，cmd3将不会运行。
```bash
$ mkdir /root/new_folder || cd / root/new_folder || pwd
```
在上面的命令中，创建文件夹的命令失败，因为该文件夹已经存在。因为这个命令失败了，所以下一个命令cd /root/new_folder被成功执行。这个命令已经成功运行，下一个命令pwd将不运行。
<a name="UYb03"></a>
### 合并&&和||运算符
例如，可以检查文件是否存在，并相应地打印消息。
```bash
$ [ -f file.txt ] && echo "File exists" || echo "File doesn't exist"
```
在创建file.txt文件之前和之后运行上面的命令，看看区别:<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603674999530-8ae2639c-f7c4-482a-8151-5126a80f7b36.png#align=left&display=inline&height=127&originHeight=380&originWidth=3323&size=405069&status=done&style=none&width=1107.6666666666667)
