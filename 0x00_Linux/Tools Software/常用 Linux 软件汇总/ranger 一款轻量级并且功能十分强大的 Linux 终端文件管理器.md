Linux
<a name="Xw0tS"></a>
## **1、什么是 ranger**
ranger 是一款轻量级并且功能十分强大的 Linux 终端文件管理器，它跟 Vim 深度绑定。它不仅可以在终端直接查看文件内容，还可以快速编辑文件。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675148082554-469784ca-a0ae-48eb-b54d-00675933ec14.jpeg#averageHue=%23313128&clientId=ub98d1aaf-4f7e-4&from=paste&id=u778d3ac6&originHeight=507&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uae49aaaa-a1aa-415b-a982-37ea9655e1d&title=)<br />ranger 的界面十分清爽，可以显示目录的层次结构，可以允许探索文件系统、搜索特定文件、切换目录等。<br />它一共由三列布局组成：

- **左栏：上一级目录**
- **中栏：当前目录内容**
- **右栏：所选文件或文件夹的预览**

除此之外， ranger 有哪些强大的功能呢？主要有以下：

- **常用文件操作（创建/修改权限/复制/删除）；**
- **类 Vim 控制台和快捷键；**
- **切换目录和浏览文件系统的快速方法；**
- **标签，书签，鼠标支持；**
- **视频缩略图预览；**

接下来详细介绍 ranger 的用法。
<a name="I8BtG"></a>
## **2、安装 ranger**
由于 ranger 与 make、Git、Vim 所以在安装 ranger 之前，需要先安装好以上三个组件。
```
$ sudo apt install make git vim
```
不过这三个工具作为程序员基本上都已经安装好了。<br />ranger 几乎已经包含在所有 Linux 发行版，所以使用对应的包管理器基本上可以直接安装。

- **Debian/Ubuntu**
```
$ sudo apt install ranger
```

- **CentOS/Fedora/RHEL**
```
sudo yum install ranger
```
<a name="FvJTK"></a>
## **3、ranger 的基本用法**
安装完成之后，只需要直接运行 ranger 这个命令就可以进入到 ranger 界面了。<br />进入到 ranger 界面之后，可以看到有一个对象被高亮显示。如果需要移动高亮块，除了使用箭头外，还可以使用 hjkl ，没错，跟 Vim 移动光标一样！<br />其实它的很多操作都跟 Vim 很类似，比如：
```
上页    ctrl + b/u
下页    ctrl + f/d

首行    gg
尾行    G

前进    L 
后退    H
上级目录    h
下级目录    l

查找    f
搜索    /
查找下一个  n
查找上一个  N
```
文件管理：
```
新建目录/文件       :touch xxx
删除目录/文件       dD
修改目录/文件       cw/I/A
复制目录/文件       yy
粘贴目录/文件       pp
剪切目录/文件       dd
刷新目录/文件       R
```
如果嫌键盘麻烦，它可以直接使用鼠标点击操作！如果想打开某个文件，直接按下回车就行，想要退出 ranger ，敲下 q 就行。<br />更多操作可以查看文末官方 GitHub 介绍。
<a name="zkFdV"></a>
## **4、ranger 的配置文件**
跟其它 Linux 程序一样，ranger 也是有很多配置文件，但需要执行以下命令生成本地配置文件：
```
$ ranger --copy-config=all
```
然后，就会在 ~/.config/ranger 里生成四个主要的配置文件：

- **commands.py：定义了 ranger 的控制台命令；**
- **rifle.py：定义了使用什么程序来打开文件；**
- **rc.conf：ranger 按键绑定及设置；**
- **scope.sh：定义了如何处理文件预览。**

前文已经说过，ranger 与 Vim 已经深度绑定，如果不喜欢 Vim 的操作方式，可以通过修改 rc.config 来修改配置。
<a name="XhN5i"></a>
## **5、使用 ranger 预览文件**
在正常情况下，需要查看一个文件的内容时，需要先打开才能查看。<br />有了 ranger 这个神器，不需要打开文件，选中某个文件，立刻就能直接预览！<br />常用的文本文件，比如：.txt，.py，.docx，.epub，.pdf，完全不在话下，甚至还可以直接预览图片及视频！<br />在 scope.sh 这个文件里，可以指定什么程序去预览这些文件。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675148082724-443a6040-26a4-4976-badb-e916f974ae1b.jpeg#averageHue=%2327271d&clientId=ub98d1aaf-4f7e-4&from=paste&id=u54ddb8e2&originHeight=813&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5c84f9e5-d8e5-416a-8fa1-302daaf2608&title=)
<a name="Mxvxn"></a>
## **6、使用 ranger 预览图片**
在一些终端工具，比如 urxvt, iTerm2, Kitty 和 Terminology ，可以支持图片预览。<br />（实测 MobaXterm 和 Xshell 都暂时还不支持直接图片预览，而是打开一个新窗口）<br />可以在 rc.conf 这个配置文件里，将 preview_images_method 这个变量设置为 true 。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675148082712-f27059ac-eae8-46a0-b771-4124c65f5732.jpeg#averageHue=%23151d23&clientId=ub98d1aaf-4f7e-4&from=paste&id=u56b1f85c&originHeight=556&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7b2193b8-f5a8-42ef-bd24-aa854631193&title=)
<a name="ycOAk"></a>
## **7、使用 ranger 预览 PDF 文件**
默认情况下，ranger 是将 PDF 文件转为图片再显示。如果想原汁原味显示 PDF 文件，可以打开 scope. sh 这个文件，然后把下面这段代码的注释打开：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675148082701-e3df8b35-814e-41a2-a962-338611c6718b.jpeg#averageHue=%2312181e&clientId=ub98d1aaf-4f7e-4&from=paste&id=uac7c6057&originHeight=198&originWidth=613&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc88c3892-6453-40f0-8e3c-7a850b98da4&title=)<br />接下来，就可以正常预览 PDF 文件了。（但是在MobaXterm 和 Xshell 实测都暂时还不支持直接预览，而是打开一个新窗口）<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675148082654-20d220a5-1da7-4c8c-a2f3-59e4a52ca5a6.jpeg#averageHue=%238b908e&clientId=ub98d1aaf-4f7e-4&from=paste&id=ucc3f80f8&originHeight=555&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u198a1c9a-d07e-4ee6-bf8f-7920eae7a84&title=)
<a name="uNHSq"></a>
## **8、控制台命令**
就像 Vim 一样，ranger 也有控制台命令选项（就是界面最下面的那一行）。可以通过键入`:` 然后跟上对应的命令来使用它们。<br />下面列出了一些有用的命令。

- `**:bulkrename**`** – 批量重命名**
- `**:openwith**`** – 使用您选择的程序打开选择的文件**
- `**:touch 文件名**`** – 创建一个文件**
- `**:mkdir 文件名**`** – 创建目录**
- `**:shell <命令>**`** – 在 shell 中运行命令 (bash/zsh 等）**
- `**:delete**`** – 删除文件**

这些命令同样也可以配合占位符和选项一起使用，实现现强大的功能。<br />**占位符**：

- `**％F**`** – 替换突出显示的文件**
- `**%d**`** – 替换当前目录**
- `**%s**`** – 替换当前选择的文件**
- `**%t**`** – 替换当前标记的文件**

比如—— `**:delete %s**`** ** 删除所有当前选择的文件。<br />**选项**：

- `**w**`** – 查看外部命令的输出，像这样 **`**:shell -w free**`
- `**t**`** - fork 控制台程序（在新的终端窗口中运行），像这样 **`**:shell -t nano**`
- `**f**`** – fork GUI 程序，像这样 **`**:shell -f firefox**`
- `**r**`** – 以 root 权限运行程序。**
<a name="pQ5FA"></a>
## **9、小结**
介绍了怎样安装、设置和使用终端文件管理器 ranger。它的工作与普通的文件管理器很类似，但由于它在终端中工作并支持各种脚本、按键绑定和命令，因此它比使用图形用户界面浏览文件更强大。<br />ranger 官方 GitHub 链接：[https://github.com/ranger/ranger](https://github.com/ranger/ranger)
