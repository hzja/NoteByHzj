LinuxVim
<a name="wii2R"></a>
## 一、前言
相信学习过Linux的朋友们对于Vim编辑器都不会陌生吧！Vim是许多开发人员和高级用户的首选编辑器。它是基于 Bill Joy 在 1970 年代为 UNIX 版本编写的 Vi 编辑器。它继承了 Vi 的键绑定，但也增加了大量原始 Vi 所缺少的功能和可扩展性。<br />尽管 Vim 不像最初标准 GUI 文本编辑器（如 Gedit）或文字处理器（如 OpenOffice.org 的 Writer）那样容易上手，但从长远来看，使用 Vim 可以提高工作效率。如果你是一个盲打的程序员，你会发现你的速度在 Vim 上会提高得更多，因为你的手不需要离开键盘，也不用操作鼠标。<br />这里从 Vim 编辑器的初级使用以及高级使用来进行讲解，可以根据自己的情况选择阅读。
<a name="vtMjO"></a>
## 二、Vim 编辑器的简单介绍
Vi 其实是一个终端应用程序，可以通过终端来启动它。Vim 编辑器有三种模式，分别是：**命令模式**（Command mode），**输入模式**（Insert mode）和**底线命令模式**（Last line mode）
<a name="MwJGY"></a>
### 2.1 命令模式
当使用 Vim编辑器打开一个文件后会自动进入到命令模式，在这种模式下用户是不能对文件内容进行更改的，可以理解为是以只读的方式打开了该文本文件，可以通过鼠标滚轮或键盘上下左右键来浏览文本文件。<br />在命令模式下用户敲击键盘会被识别为命令而不是直接在文本中插入内容，例如可以通过输入 `i` 命令来切换到输入模式下，输入 x 会删除当前光标所在位置的字符，当然命令行模式下所支持的命令还有很多，后面会给大家列举出来！<br />另外大家需要注意的命令模式和底部命令行模式是有区别的，这些区别主要体现在两者能够识别的命令是不一样的，命令模式下的命令更多的是用来提高效率的，例如可以通过命令跳转到某行、删除某行、跳转到行首、跳转到行尾等，这些命令都会使我们的使用更加高效。<br />下面列举出一些常用到的命令，大家可以在使用过程中慢慢的去记住这些指令，这些指令即使记不住也不会影响对于 Vim 的使用，但是如果会用了这些命令可以大大提高 Vim 使用效率。
```
h 将光标向左移动一个字符。
j 将光标向下移动一行。
k 将光标向上移动一行。
l 将光标向右移动一个字符。
0 将光标移动到行首。
$ 将光标移动到行尾。
w 向前移动一个单词。
b 向后移动一个单词。
G 移动到文件末尾。
gg 移动到文件的开头。
`.移至上次编辑。
6k 在移动命令前面加上一个数字将多次执行该移动
```
对于上面的命令大家需要多加练习，在使用中慢慢去记住这些小技巧，**另外除了最初打开文件会进入到命令模式外还可以通过点击 Esc 键来从其他模式进入到命令模式下！**
<a name="JFnx2"></a>
### 2.2 输入模式
讲解完命令模式后一起来看一下 Vim 编辑器的输入模式，要想了解输入模式首先需要先学习如何从其他模式进入到输入模式。<br />要想进入到输入模式首先需要进入到命令模式下，即在其他模式下首先点击 Esc 按键，然后在命令模式下输入 i 即可进入到输入模式下，退出输入模式也可以通过点击 Esc 按键进入到命令模式。<br />进入到输入模式后就可以对文本内容进行更改了，当然键盘上的一些特定的按键也是可以用的例如以下按键：
```
字符按键以及Shift组合，输入字符
ENTER，回车键，换行
BACK SPACE，退格键，删除光标前一个字符
DEL，删除键，删除光标后一个字符
方向键，在文本中移动光标
HOME/END，移动光标到行首/行尾
Page Up/Page Down，上/下翻页
Insert，切换光标为输入/替换模式，光标将变成竖线/下划线
ESC，退出输入模式，切换到命令模式
```
除了通过输入 `i` 进入输入模式外还可以通过以下命令来进入到对应的输入模式：
```
i 在当前光标所在位置插入随后输入的文本，光标后的文本相应向右移动
I 在光标所在行的行首插入随后输入的文本，行首是该行的第一个非空白字符，相当于光标移动到行首执行 i 命令
o 在光标所在行的下面插入新的一行。光标停在空行首，等待输入文本
O 在光标所在行的上面插入新的一行。光标停在空行的行首，等待输入文本
a 在当前光标所在位置之后插入随后输入的文本
A 在光标所在行的行尾插入随后输入的文本，相当于光标移动到行尾再执行a命令
```
在该模式下，用户输入的任何字符都被 Vim 当做文件内容保存起来，并将其显示在屏幕上。在文本输入过程中，若想回到命令模式下，按下 Esc 键即可。但是如果想对输入的内容进行保存，可以还需要了解底部命令行模式，下面就开始介绍以下底部命令行模式。
<a name="P5FBq"></a>
### 2.3 底部命令模式
在命令模式下，用户按 : 键即可进入底部命令模式下，此时 Vim 会在显示窗口的最后一行（通常也是屏幕的最后一行）显示一个 `:` 作为底部命令模式的说明符，等待用户输入命令。多数文件管理命令都是在此模式下执行的（如把编辑缓冲区的内容写到文件中等）。<br />知道了如何进入到底部命令模式后就需要学习一下底部命令模式模式都支持哪些命令，下面就来一起介绍一下底部命令模式支持的命令：
```
:q 退出 vim 编辑器
:q! 不保存文件，直接退出 vim 编辑器
:w 只保存文件，但不退出 vim 编辑器
:wq 保存文件且退出 vim 编辑器
ZZ 保存文件且退出 vim 编辑器
:start,endd 从第 start 行开始到 end 行进行删除操作 最后一个 d 表示删除，如 :3,4d
:%d 清空文件内容(跳到行首 dG 执行也可删除)
:!command 暂时离开vim编辑器去到指令模式下执行 command 命令显示结果，如 :!ls
:/string 从文件首部开始查找到尾部；按 n 键可以跳到下一个，N 上一个，另外按 / 键后，按上下键可以找到以前查找的记录，同样的也有记录
:?string 同上，从当前光标向文件首部查找
:%s/word1/word2/g 从文件首到尾替换每一行的 word1为 word2
:set nu 显示行号 也可以输入 set number
:set nonu 取消行号
:set ai/noai 开启/关闭自动缩进
```
在底部命令行模式下除了上面这些还有很多，这里仅仅列出了一些常用的指令，感兴趣的可以看一下官方文档。
<a name="bLZwC"></a>
### 2.4 三种模式的切换
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527423565-bff63cb8-de58-4e0a-a1ae-ba87e27dc8ae.png#averageHue=%23edecea&clientId=u71ada780-8558-4&from=paste&id=u2cca3a80&originHeight=531&originWidth=787&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2fed128e-9660-43e7-9884-4eb620dabb4&title=)![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527423572-b56a1cde-baca-488e-8794-0f3e3646735f.png#averageHue=%23e4e1ce&clientId=u71ada780-8558-4&from=paste&id=u40e20e97&originHeight=724&originWidth=1024&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud2de2114-f108-474c-82a3-8e72c498ebf&title=)
<a name="e5oVg"></a>
## 三、Vim 编辑器常用操作
通过前面的介绍相信大家已经对 Vim 编辑器有了一定的了解，但是如果之前重来没有用过 Vim 的话相信现在还是很迷惑，为什么一个好好的编辑器要弄的这么麻烦呢？编辑器难道不是就是输入和删除吗？<br />下面一起来看一下一些常用的 Vim 操作，相信这部分会让你知道 Vim是如何使用的：
<a name="vdU2T"></a>
### 3.1 Vim 最基本的操作
这部分的操作需要确定电脑上已经安装了 Vim，否者将无法进行操作，如果想要安装 Vim 编辑器可以使用以下指令：
```bash
sudo apt-get install vim
```
安装完 Vim 编辑器后就可以使用 Vim 编辑器对文本文件进行操作了，下面一起来看一下：<br />如果想打开或新建一个名为 test.txt 的文本文件可以进行以下操作：
```bash
$ vim test.txt
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527423550-5bea7e6d-4dea-41a6-b5bd-dc9cfa3a0095.png#averageHue=%231c1c1c&clientId=u71ada780-8558-4&from=paste&id=u06e62c67&originHeight=636&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u634c7971-9d2d-4662-9b5f-5c66a43f8d8&title=)此时就打开或新建了一个名为 test.txt 的文本文件，并进入了命令模式模式下，此时就可以使用前面介绍的操作了，例如想要在该文件中输入 111 就需要先输入 i 进入到输入模式下。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527423596-37047a22-4589-4020-9c0a-037f6748e6d5.png#averageHue=%231c1c1c&clientId=u71ada780-8558-4&from=paste&id=ubb1cd5d7&originHeight=636&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8127b852-04c3-4a15-9ba0-b351d1a0feb&title=)此时再敲击键盘就会发现所在键盘上点击的按键都输入到了文本中，例如输入 111<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527423537-5c92381b-70f3-4d61-864d-3dfb9b0358d9.png#averageHue=%231c1c1c&clientId=u71ada780-8558-4&from=paste&id=u53fc83c2&originHeight=632&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4284de97-9795-4e65-963f-4b87043d532&title=)但是此时所进行的操作还只是在控制台进行的操作，文本文件的内容还没有修改，如果想要将输入的内容保存下来还需要进行以下操作，即 **保存文件 --> 退出文件**<br />需要首先点击 Esc 进入到命令模式，在命令模式下输入 `:` 进入底部命令模式，并在底部命令模式下输入 `wq` 来保存并退出该文本文件，这样就将写入的内容保存到了文本文档中，上面的操作过程是 Vim 最基本的操作，也是使用 Vim 必须要会的操作。
<a name="rkFdS"></a>
### 3.2 Vim 的光标移动操作
首先在 Vim 编辑器的最长用的操作就是光标的移动了，此时可能会 有疑问，光标移动不就用键盘上的方向键不就行了吗？为什么还要学其他的，有这个疑问的可以想象一下有一个几百行的文本文件，要上下左右一个一个的移动什么时候能移动到行尾，所以 Vim 编辑器准备了更加高效的光标移动操作。<br />虽然上下左右移动光标很多人依然用光标键，但 Vim 推荐用 hjkl，能够更高效移动光标。<br />下面给大家列举一些常用的操作，大家可以自己尝试一下：
```
基本操作：
 h:左
 l:右
 j:下
 k:上

高级操作：
 gg:文档顶部
 G:文件底部或行号
 w:下一个单词
 e:单词尾
 0:行首
 $:行尾
 H:将光标移动到当前可见页面的顶部（High）
 M:将光标移到当前可见页面的中间（Middle）
 L:将光标移动到当前可见页面的底部（Low）

快捷键操作：
 CTRL-D:向下翻页（Down）
 CTRL-U:向上翻页（Up）
 CTRL-E:向下滚动一行
 CTRL-Y:向上滚动一行
 ):移至下一个句子的开头
 (:移至上一句的开头
 }:移至下一个段落的开头（以空白行分隔）
 {:移至下一段的开头（以空白行分隔）
```
上面的这些操作大家要多加练习，相信你也会用的很6
<a name="L8ave"></a>
### 3.3 多窗口同时打开多个文件
下面再看几个比较秀的操作 -- **多窗口打开多文件**，Vim 的多窗口是动态的，可以开始编辑时就打开多窗口，也可以工作时随时增加新窗口，或者删除一个窗口。
```bash
$ vim -o file1 file2  打开一个编辑会话，显示为水平分割成两半的窗口，一个文件占用一个窗口。
$ vim -O file1 file2  和上面相同，不过是垂直分割两半的窗口
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527423865-ecb08816-6df2-4433-9c01-0b0fa3d91777.png#averageHue=%233a1831&clientId=u71ada780-8558-4&from=paste&id=u24a2e36a&originHeight=736&originWidth=762&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55530aa3-8058-4a1b-9f40-63351e5ed2e&title=)如果画面分割后不足以容纳下所有窗口，那么第一个文件会占用大部分窗口，余下的窗口则载入缓冲区，暂时不能看到，但仍可以取用。<br />`-o` 后面加一个数字 `n` ，是预分配窗口的形式打开：
```bash
$ vim -O file1 file2 将左右切分窗口并同时打开文本文件
$ vim -0 file1 file2 将上下切分窗口并同时打开文本文件
```
效果如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424020-1f3e7046-5ee4-495e-ab13-3d12a81e5d66.png#averageHue=%23202020&clientId=u71ada780-8558-4&from=paste&id=u2b3c21e6&originHeight=635&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u54965d34-10fe-4f32-aaeb-8e9d8df936d&title=)![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424099-d278c920-0cec-45d4-934e-1e2dccd614f4.png#averageHue=%23232323&clientId=u71ada780-8558-4&from=paste&id=u8c48389a&originHeight=634&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u75eb362d-ed11-4ab3-ac41-a5c2f367db5&title=)这时候可能会有疑问，这同时打开了两个文件怎么确定现在操作的是那个文件呢？可以通过光标来进行确定当前所操作的是哪个文件。<br />例如现在在操作file1，但是突然又需要操作file2 ，应该如何进行操作呢？<br />可以通过在命令模式下使用以下快捷键来进行窗口的切换，其中最常用的是：
```
Ctrl+ww 依次向后切换到下一个窗格中
Ctrl+6  两文件间的切换
:bn     下一个文件
:bp     上一个文件
:ls     列出打开的文件，带编号
:b1~n   切换至第n个文件
```
上面这几个操作都可以完成窗体间的切换，但是由于切换的原理和现象不同，下面给大家演示一下几种快捷键或命令的不同。<br />例如当前是在操作文件 file1 首先演示使用 Ctrl+ww 快捷键来切换窗口：<br />操作 Ctrl+ww 快捷键前：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424189-08b244df-5b6d-473d-9ba4-c19567be9a29.png#averageHue=%23202020&clientId=u71ada780-8558-4&from=paste&id=uc9ca4685&originHeight=636&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5d25d046-d83d-4cd3-a75d-1d3f4c802d6&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424351-cf5b191f-e7a2-4b26-8911-d9cf81a2d62b.png#averageHue=%23202020&clientId=u71ada780-8558-4&from=paste&id=u4072d9d9&originHeight=637&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7a6b9bc0-0103-4e8e-b928-625407cb2df&title=)这样 Vim 会在不更改窗口布局的情况下仅仅切换操作的串口，这个快捷键也是最常用的。<br />除了使用Ctrl+ww 快捷键进行操作外还可以使用下面的指令进行窗口的切换，但是大家需要注意的是下面的这些切换才是真正意义上的窗口切换，而上面讲的快捷键只是工作区的切换，来一起看一下窗体的切换指令效果：<br />初始状态下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424503-38a66bb5-5b7c-463d-8605-5e67ac338fe3.png#averageHue=%23202020&clientId=u71ada780-8558-4&from=paste&id=u8af4aeb8&originHeight=637&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1729f8b9-723d-4cad-9e34-f13d23e75fb&title=)在命令模式下执行完命令后：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424582-d851c0b9-b4fc-4e6e-aed8-b57b095e8701.png#averageHue=%23202020&clientId=u71ada780-8558-4&from=paste&id=ud0944ba6&originHeight=633&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua828a27a-f7d8-4c74-ba59-6f718c145a5&title=)![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676527424663-6fef4934-e8c0-4240-90b9-966ba8a22f2f.png#averageHue=%23202020&clientId=u71ada780-8558-4&from=paste&id=u9998113d&originHeight=632&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8394c002-20ee-46d0-b85b-94017e68f33&title=)可以看到这个命令是把下一个窗口移动到了当前最左侧的窗口内，其他窗口保持不变，可以发现此时两个窗口打开的都是文件2.<br />除了上面这几个指令以及快捷键外 Vim 的多窗口操作还有很多，感兴趣的大家可以执行百度一下。
```
:sp:   将当前窗口水平一分为二。最初，每个窗口将显示相同的缓冲区。（split）在“ sp”之前加上数字以设置新窗口的高度。
:vs:   垂直拆分当前窗口。最初，每个窗口将显示相同的缓冲区。（vertical split）在“ vs”之前加上数字以设置新的窗口宽度。
CTRL-ww: 将光标切换到下一个窗口（window）
CTRL-w: 将焦点沿指示的方向（h，j，k，l）更改到指定窗口
CTRL-wc: 关闭当前窗口（close）
CTRL-w+: 增加当前窗口的大小
CTRL-w-: 减小当前窗口的大小
CTRL-w=: 将所有窗口设置为相等大小
＃CTRL-w_: 将高度设置为前面的“＃”指示的大小
:only:   关闭当前窗口以外的所有窗口
CTRL-wn: 使用新缓冲区打开新窗口（new）
```
<a name="NGvPI"></a>
## 四、一些其他的 Vim 小技巧
说到小技巧肯定要说一下复制、粘贴，下面来看一下如何在 Vim 中进行复制和粘贴的。
<a name="uOV87"></a>
### 复制操作

1. 复制光标所在行 按键：yy，粘贴：在想要粘贴的地方按下 p 键
2. 以光标所在行为准（包含当前行），向下复制指定的行数 按键：数字yy，粘贴：p
3. 可视化复制 按键：ctrl+v 然后按上下左右方向键来选中需要复制的区块，按下yy键进行复制，最后按下p键粘贴
<a name="xGfVU"></a>
### 剪切/删除

1. 剪切/删除光标所在行 按键：dd （删除之后下一行上移）**注意：**dd 严格来说是剪切命令，但是如果剪切了不粘贴就是删除的效果
2. 剪切/删除光标所在行为准（包含当前行），向下剪切/删除指定的行 按键：数字 dd（删除之后下一行上移）
3. 剪切/删除光标所在的当前行，删除之后下一行不上移 按键：D
<a name="aVVQy"></a>
### 撤销/恢复

1. 撤销：输入:u （u即undo）
2. 恢复：ctrl+r （取消之前的撤销 ）

**注意：** 上面的复制粘贴是限制在本文件内进行的操作，也就是说复制的内容只能在该文件内进行复制粘贴，在外部粘贴是没有效果的。
