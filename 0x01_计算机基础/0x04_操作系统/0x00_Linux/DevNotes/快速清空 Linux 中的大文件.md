Linux
<a name="cZv5y"></a>
### 1、通过重定向到null清空文件内容
使用 shell 重定向`null`(不存在的对象)到文件的最简单方法来清空文件内容
```bash
# > access.log  
```
<a name="QKj3N"></a>
### 2、使用`true`命令重定向清空文件
这里使用一个符号，`:`是一个shell内置命令，本质上等同于`true`命令<br />另一种方法是将`:`或`true`内置命令的输出重定向到文件
```bash
# : > access.log  
OR   
# true > access.log
```
<a name="NI1Rk"></a>
### 3、使用带有 `/dev/null` 的 `cat/cp/dd` 应用程序清空文件
在 Linux 中，`null`设备主要用于丢弃进程不需要的输出流，或者作为输入流。这通常是通过重定向机制完成的。<br />`/dev/null`设备文件是一个特殊文件，它会吞噬发送给它的任何输入，或者它的输出与空文件的输出相同。<br />此外，可以通过`/dev/null`使用cat 命令将输出重定向到文件作为输入来清空文件的内容
```bash
# cat /dev/null > access.log
```
也可以使用cp 命令将文件内容清空
```bash
# cp /dev/null access.log  
```
在下面的命令中，`if`指的是输入文件，`of`指的是输出文件。
```bash
# dd if=/dev/null of=access.log  
```
<a name="kckXz"></a>
### 4、使用 `echo` 命令清空文件
在这里，可以使用带有空字符串的echo 命令并将其重定向到文件
```bash
# echo "" > access.log  
OR  
# echo > access.log  
```
空字符串与 null 不同。一个字符串是一个对象，它可能是空的。而null意味着一个不存在的对象。<br />因此将上面的`echo` 命令的输出重定向到文件中，并使用`cat` 命令查看文件内容时，会打印一个空行<br />要发送一个空输出到文件，使用该标志`-n`告诉`echo` 不输出换行符。
```bash
# echo -n "" > access.log  
```
<a name="wJljC"></a>
### 5、使用 `truncate` 命令清空文件
`truncate`命令将文件的大小缩小或扩展到定义的大小。<br />可以将它与`-s`指定文件大小的选项一起使用。要清空文件内容使用`-s 0`参数。
```bash
# truncate -s 0 access.log
```
