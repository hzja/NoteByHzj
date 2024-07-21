虽然现代的开源代码编辑器已经在编程界掀起了风暴，但 Linux 的命令行仍然由一些选定的文本编辑器所统治。流行的命令行编辑器如 Vim 和 Emacs 等也因其奇怪的键盘快捷键而臭名昭著。<br />在编程领域，有几个关于那些奇怪的键盘快捷键的笑话 - **如何退出 Vim 可能是其中最常见的**。<br />**Micro** 是一个使用 Go 语言开发的现代的基于终端的开源文本编辑器，它试图摆脱键盘快捷键的痛苦，并提供流行的快捷键以及鼠标支持。<br />Micro 旨在通过易于安装和使用来成为 nano 编辑器的继承者。对于喜欢在终端中工作的人或经常通过 SSH 编辑文件的人来说，它努力成为一名全职编辑器。<br />你可能想知道这款软件有什么特别之处，因为还有很多其他基于终端的文本编辑器，为什么要选择使用 **Micro** 呢。答案是，**Micro 非常容易使用**，几乎没有什么学习曲线，不需要学习任何新东西，而且它有一些非常有趣的功能。<br />此外也不乏一些业界大佬对  **Micro** 表达了喜爱之情。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673915561666-ba32829b-b294-4fbe-a30d-d6c76afc4e11.png#averageHue=%23ababab&clientId=uef833fbf-cd50-4&from=paste&id=u435c8336&originHeight=1085&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub9bad058-3407-4d8c-a637-62e675852ea&title=)
<a name="XL4Vw"></a>
## 特性
![Micro 编辑器界面](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1673915561503-6bda36b0-4f44-4a5b-9eeb-0eabd3486966.jpeg#averageHue=%236e6f6e&clientId=uef833fbf-cd50-4&from=paste&id=u89373765&originHeight=651&originWidth=1065&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=udd72f2d4-1318-4135-a44a-b9fc44b151e&title=Micro%20%E7%BC%96%E8%BE%91%E5%99%A8%E7%95%8C%E9%9D%A2 "Micro 编辑器界面")<br />**Micro** 编辑器的一些主要亮点有：

- 易于使用和安装。
- 不需要依赖或外部文件。
- 多光标支持。
- 支持通用键盘快捷键（Ctrl-s, Ctrl-c, Ctrl-v, Ctrl-z, ...）。
- 智能的默认值。
- 拆分和选项卡。
- 类似 nano 的菜单，帮助你记住键盘快捷键。
- 极好的鼠标支持：这意味着鼠标拖动可以创建一个选择，双击可以按字选择，三击可以按行选择。
- 跨平台（它应该能在所有 Go 运行的平台上工作）。
- 插件系统（插件是用 Lua 编写的）：micro 有一个内置的插件管理器来自动安装、删除和更新插件。
- 内置 diff gutter。
- 简单的自动补全。
- 撤销持久化。
- 自动提示和错误通知。
- 超过 130 种语言的语法高亮。
- 颜色方案支持：默认情况下，micro 带有 16、256 和真彩色主题。
- 支持真彩色（将 **MICRO_TRUECOLOR** 环境变量设为 1 来启用它）。
- 用系统剪贴板进行复制和粘贴。
- 小而简单。
- 易于配置。
- 宏。
- 常见的编辑器功能，如撤销/重做、行号、Unicode 支持...
<a name="HP2db"></a>
## 安装
**Micro** 在所有主要发行版的存储库中都可用。在 Ubuntu 中，可以使用以下命令安装它：
```bash
$ sudo apt install micro
```
这将安装 **xclip** 作为剪贴板功能的依赖项。<br />此外也可以直接下载编译好的二进制文件，从 Release 页面 [https://github.com/zyedidia/micro/releases](https://github.com/zyedidia/micro/releases) 下载对应的安装包，解压缩文件，会在其中找到二进制文件。将此二进制文件复制到 PATH 路径下面即可。然后，就可以使用命令 **micro** 在终端中运行它了。
```bash
$ micro -version
Version: 2.0.10
Commit hash: b9763856
Compiled on August 07, 2021
```
要获得剪贴板支持，需要安装 **xclip** 和 **xsel** 包。在 Ubuntu 和其他基于 Ubuntu 的 Linux 发行版中，可以使用以下命令来安装它：
```bash
$ sudo apt install xclip
```
对于 Mac 系统，同样可以使用 Homebrew 进行安装：
```bash
$ brew install micro
```
如果使用的是 macOS，则应考虑使用 **iTerm2** 而不是默认终端 (Terminal.app)。**iTerm2** 终端具有更好的鼠标支持以及对关键事件的更好处理，为了获得最佳的按键绑定行为，请在 **Preferences->Profiles->Keys->Presets...** 下选择 xterm 默认值，然后在同一菜单中为 **Left Option Key** 选择 **Esc+**，最新版本还支持真彩色。
<a name="Bc0RD"></a>
## 使用
当安装完成后，就可以在终端通过运行 **micro path/to/file.txt** 或 **micro** 命令来打开一个空缓冲区启动它。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673915561540-51e1e17b-24b7-49f8-b8b2-508e8bebab17.png#averageHue=%23353535&clientId=uef833fbf-cd50-4&from=paste&id=ue73c8ebc&originHeight=674&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e8dc568-05e8-481d-895f-2c0cee687ec&title=)<br />编辑器在终端中打开后就可以使用鼠标键盘快捷键来操作文本了，只需要单击并拖动鼠标就可以选择文本，也可以双击选择单词，三击启用行选择。<br />可以使用快捷键 **Ctrl+g** 来打开 **Micro** 帮助信息，要打开编辑器的命令行工具，可以使用快捷键 **Ctrl+e**，这会在编辑器底部打开一个 **>** 等待命令输入的提示符。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673915561531-d8999abd-b261-41ca-9d3a-41b2b4744b47.png#averageHue=%235b5958&clientId=uef833fbf-cd50-4&from=paste&id=ueeb4b78a&originHeight=218&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u236c229b-4e52-4d5a-8499-f32e95d190e&title=)<br />比如现在要获取帮助信息，则可以输入 **Ctrl+e** 然后输入 **help** 回车即可执行该命令了。<br />要获取默认的快捷键和功能绑定信息，可以使用 `**help defaultkeys**` 命令获取。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673915561886-5ec5c978-bca2-4c58-8482-b902848bf92d.png#averageHue=%23353535&clientId=uef833fbf-cd50-4&from=paste&id=uaadbefb5&originHeight=335&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u66f97be7-f250-4ee0-a61e-441ef0cda0e&title=)<br />要在编辑器中进行水平拆分可以输入命令 `**hsplit path/to/file.txt**`，要进行垂直拆分则可以使用命令 `**vsplit path/to/file.txt**`。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673915561959-e1e0f5f0-cf45-46ba-9b5c-f755b8cd53b7.png#averageHue=%23323232&clientId=uef833fbf-cd50-4&from=paste&id=uca369c3e&originHeight=674&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4126b679-9504-431e-a3cf-f5d0e0fc591&title=)<br />下图是总结的 **Micro** 编辑器的速查表，可以对照来学习 **Micro** 的使用，还是非常简单的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673915561977-c8df8621-e0a7-4e6a-b933-c38fb20cc1cb.png#averageHue=%23d8e7ea&clientId=uef833fbf-cd50-4&from=paste&id=u6152ae58&originHeight=942&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u11f586a7-1b66-4c66-a41a-e9e48a98526&title=)<br />**Micro** 是一个非常好的文本编辑工具，虽然它不像 Vim 或其他成熟的文本编辑器那样功能丰富，但它可以轻松替换 Nano 等工具，用于偶尔在终端中进行文件编辑。如果经常需要在终端中编辑文件，但 Vim 对你来说太复杂，那么可以尝试一下 **Micro**。<br />Git 仓库：[https://github.com/zyedidia/micro](https://github.com/zyedidia/micro)
