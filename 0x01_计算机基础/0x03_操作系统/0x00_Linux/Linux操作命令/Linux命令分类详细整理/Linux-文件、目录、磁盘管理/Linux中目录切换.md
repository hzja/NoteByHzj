Linux cd dirs pushed popd<br />在命令行模式下，当需要切换不同目录中，很多时间都浪费在重复输入上如果这些目录不在同一个根目录中，有时候不得不在转换时输入完整的路径名，这难免让人有些难以忍受，可以用以下的一个或两个步骤来避免所有多余的输入：用命令行解释器中的历史记录，或者用命令行函数`pushd`。<br />用命令行解释器中的历史记录的好处是只需按很少的键在命令行中用向上的箭头来查找使用过的命令，直到找到目标，然后按回车键如果所切换的两个目录在整个驱动器的子目录结构中很接近，那用解释器中的历史记录可能是最好的选择。<br />如果在两个截然不同的路径间转换的话，可以很好的使用`pushd`这个函数，可以用它创建一个目录堆栈（在内存中的一个列表）。
:::tips
注释：缺省情况下，`pushd`函数可能不包括在Linux系统中；但它包涵在Red Hat和用Red Hat驱动的系统中如果系统中没有`pushd`函数，可以在ibiblio.org网站上下载相关的函数工具。
:::
`pushd`的使用：假设现在工作在/usr/share/fonts/目录下，需要对/usr/share/fonts/做一些改动，将频繁的在两个目录间切换开始在一个目录下，用`pushd`函数切换到另一个目录在例子中，开始在/usr/share/fonts/下，键入`pushd /home/yxy/`，然后按回车键现在，将在下一行看到堆栈中的内容：/home/yxy /usr/share/fonts。<br />正如所看到的，当键入`pushd`和一个路径名时，将自动产生一个堆栈，内容是键入的目录名和当前工作的目录名，在例子中，键入的路径（/opt/wonderword/fonts）在堆栈的顶部。<br />快速返回上一级目录，可以直接键入`pushd`，如果不跟路径名，将返回到堆栈中前一个目录的上一层目录。<br />如果需要从堆栈中删除一个目录，键入`popd`，然后是目录名称，再按回车键想查看堆栈中目录列表，键入`dirs`，然后按回车键`popd`和`dirs`命令也是常用函数中的一部分。
:::info
说明：pushd、popd、dirs 这几个命令是bash内建的命令，所以只要登录到了bash，这几个命令就可以使用了。
:::
<a name="8XG07"></a>
### `cd`
<a name="4luZr"></a>
#### `cd -`在两个目录之间切换
如果只是在两个目录之间切换，那么还不必使用pushd，这时只需使用 `cd -`就可以了。<br />`cd -` 的作用是可以回到前一个目录。<br />例子：
```bash
[root@localhost ~]# cd /usr/share/kde4/apps/kget/pics/
[root@localhost pics]# cd -
/root
[root@localhost ~]# cd -
/usr/share/kde4/apps/kget/pics
[root@localhost pics]#
```
在两个目录之间切换非常方便，无需再输入很长的路径。<br />说明：<br />`-` 在此处等同于 `$OLDPWD`变量，`$OLDPWD`变量就是bash所记录的前一个目录，也就是说：`$OLDPWD` 和 `-` 相等。<br />下面再看一个例子：
```bash
[root@localhost ~]# cd /usr/share/kde4/apps/kget/pics/
[root@localhost pics]# echo $OLDPWD;
/root
[root@localhost pics]# cd $OLDPWD
[root@localhost ~]# echo $OLDPWD;
/usr/share/kde4/apps/kget/pics
[root@localhost ~]# cd $OLDPWD;
[root@localhost pics]# echo $OLDPWD;
/root
```
:::success
结论：用`cd -`和`cd $OLDPWD`都可以在最近所操作的两个目录之间进行切换。
:::
<a name="TCbLO"></a>
### `pushed`
<a name="7jEte"></a>
#### `pushd`在多个目录之间切换
因为 `cd -`和 `cd $OLDPWD`都是只支持最近操作的两个目录，所以要在多个目录之间操作时，需要使用 `pushd`。

1. `pushd`：切换到作为参数的目录，并把原目录和当前目录压入到一个虚拟的堆栈中如果不指定参数，则会回到前一个目录，并把堆栈中最近的两个目录作交换。
2. `popd`：弹出堆栈中最近的目录。
3. `dirs`：列出当前堆栈中保存的目录列表。

例子：
```bash
[root@localhost ~]# pushd /usr/local/sbin/
/usr/local/sbin ~
[root@localhost sbin]# dirs
/usr/local/sbin ~
[root@localhost sbin]# dirs -p -v
0  /usr/local/sbin
1 ~
[root@localhost sbin]# pushd /usr/share/kde4/apps/kget/
/usr/share/kde4/apps/kget /usr/local/sbin ~
[root@localhost kget]# dirs -p -v
0  /usr/share/kde4/apps/kget
1  /usr/local/sbin
2  ~
```
说明：`dirs`的 `-p`参数可以每行一个目录的形式显示堆栈中的目录列表；<br />`-v` 参数可以在目录前加上编号；<br />注意：有 `-v`时，不添加 `-p`也可以每行一个目录的形式显示。<br />说明之二：可以看到，最近压入堆栈的目录位于最上面。
<a name="e5a9cde1"></a>
#### `pushd`在最近的两个目录之间切换
在最近的两个目录之间切换：用`pushd`不加参数即可
```bash
[root@localhost kget]# pushd /boot/grub/
/boot/grub /usr/share/kde4/apps/kget /usr/local/sbin ~
[root@localhost grub]# dirs -v
0  /boot/grub
1  /usr/share/kde4/apps/kget
2  /usr/local/sbin
3  ~
[root@localhost grub]# pushd
/usr/share/kde4/apps/kget /boot/grub /usr/local/sbin ~
[root@localhost kget]# dirs -v
0  /usr/share/kde4/apps/kget
1  /boot/grub
2  /usr/local/sbin
3  ~
[root@localhost kget]# pushd
/boot/grub /usr/share/kde4/apps/kget /usr/local/sbin ~
[root@localhost grub]# dirs -v
0  /boot/grub
1  /usr/share/kde4/apps/kget
2  /usr/local/sbin
3  ~
```
说明：可以看到，用pushd不加参数在最近的两个目录之间切换时，当前目录总是位于堆栈的最上面。
<a name="qAotl"></a>
#### `pushd +n`在多个目录之间切换
说明：n 是一个数字，有此参数时，是切换到堆栈中的第n个目录，并把此目录以堆栈循环的方式推到堆栈的顶部。<br />需要注意：堆栈从第0个开始数起。<br />例子：
```bash
[root@localhost grub]# dirs -v
0  /boot/grub
1  /usr/share/kde4/apps/kget
2  /usr/local/sbin
3  ~
[root@localhost grub]# pushd +2
/usr/local/sbin ~ /boot/grub /usr/share/kde4/apps/kget
[root@localhost sbin]# dirs -v
0  /usr/local/sbin
1  ~
2  /boot/grub
3  /usr/share/kde4/apps/kget
```
<a name="kfHVw"></a>
#### `popd`把目录从堆栈中删除
例子：
```bash
root@localhost sbin]# dirs -v
0  /usr/local/sbin
1  ~
2  /boot/grub
3  /usr/share/kde4/apps/kget
[root@localhost sbin]# popd
~ /boot/grub /usr/share/kde4/apps/kget
[root@localhost ~]# dirs -v
0  ~
1  /boot/grub
2  /usr/share/kde4/apps/kget
[root@localhost ~]# popd +1
 ~ /usr/share/kde4/apps/kget
[root@localhost ~]# dirs -v
0  ~
1  /usr/share/kde4/apps/kget
```
说明：可以看到popd不加参数的运行情况：<br />popd把堆栈顶端的目录从堆栈中删除，并切换于位于新的顶端的目录。<br />说明之二：opd 加有参数 +n时，n 是堆栈中的第n个目录，表示把堆栈中第n个目录从堆栈中删除。
<a name="kYY2C"></a>
#### `pushd`和`popd`都可以只影响堆栈而不切换目录
用 `-n` 参数即可。看例子：
```bash
[root@localhost ~]# dirs -v
0  ~
1  /usr/share/kde4/apps/kget
[root@localhost ~]# pushd -n /boot/grub
~ /boot/grub /usr/share/kde4/apps/kget
[root@localhost ~]# dirs -v
0  ~
1  /boot/grub
2  /usr/share/kde4/apps/kget
```
<a name="KvDwY"></a>
### `dirs`
<a name="Y7jre"></a>
#### `dirs`用 `-c`参数可以清空目录堆栈
例子：
```bash
[root@localhost ~]# dirs -v
0  ~
1  /boot/grub
2  /usr/share/kde4/apps/kget
[root@localhost ~]# dirs -c
[root@localhost ~]# dirs -v
0  ~
```
:::warning
说明：位于堆栈顶部的目录是当前目录，它不能被pop出去的。
:::
