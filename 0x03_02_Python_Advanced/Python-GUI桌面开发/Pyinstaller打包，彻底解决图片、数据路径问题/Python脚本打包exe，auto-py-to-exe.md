<a name="Ft2Qf"></a>
## 1、什么是auto-py-to-exe
auto-py-to-exe 是一个用于将Python程序打包成可执行文件的图形化工具。本文就是主要介绍如何使用 auto-py-to-exe 完成 Python 程序打包。auto-py-to-exe 基于 pyinstaller ，相比于 pyinstaller ，它多了 GUI 界面，用起来更为简单方便

## 2、安装 auto-py-to-exe
首先要确保 Python 环境要大于或等于 2.7 然后在 cmd 里面输入：`pip install auto-py-to-exe` ，输入完成之后，pip 就会安装 auto-py-to-exe 包了。安装完成之后，就可以在 cmd 输入：auto-py-to-exe，来启动 auto-py-to-exe 程序了。<br />![](./img/1698372434908-1afb8513-a6ec-4e47-bd32-f5b5cb1d9dfb.png)<br />出现上述图片，auto-py-to-exe 就安装成功了。
<a name="Tmizi"></a>
## 3、auto-py-to-exe 部分选项介绍
在使用 auto-py-to-exe 打包 Python 程序的时候，有许多配置选项需要去指定，能正确知道这些选项的作用是十分重要的。下面介绍其中一些重要的选项。
<a name="fsJMJ"></a>
### (1) Script Location
Script Location 主要是指定要打包的 Python 文件<br />![](./img/1698372435079-1eef1f75-7246-42a1-8c8d-00377812f36c.png)
<a name="u5A5m"></a>
### (2) Onefile
Onefile 下有两个选项，分别是：One Directory 和 One File

- 如果选择 One Directory ，那么程序打包完成后会是一个文件夹的形式展现
- 如果选择 One File ，那么程序打包完成后就一个 .exe 文件
<a name="UGecp"></a>
### (3) Console Window
Console Window 主要设置打包程序运行时，是否出现控制台

- Console Based：当打包的程序运行时会显示一个控制台界面
- Window Based (hide the console)：会隐藏控制台界面，主要用于带有 GUI 的 Python 程序打包
<a name="XyZFI"></a>
### (4) Icon
用于指定打包程序的图标
<a name="saFUV"></a>
## 4、auto-py-to-exe 实战
本节主要以一个计算器程序来介绍如何使用 auto-py-to-exe 来打包程序。<br />auto-py-to-exe 打包程序主要分 3 部分，分别是：

- 打开 auto-py-to-exe
- 配置打包选项
- 查看打包效果
<a name="kiJLi"></a>
### 1、打开 auto-py-to-exe
打开 cmd ，输入：auto-py-to-exe 打开 auto-py-to-exe 后，就要进行配置选择了。
<a name="lSu0f"></a>
### 2、配置打包选项
计算器程序，大家可以到 GitHub 去下载，地址是：[https://github.com/pythonprogrammingbook/simple_calculator](https://github.com/pythonprogrammingbook/simple_calculator)<br />在打包时，要进行的配置主要有：

- Script Location
- Onefile
- Console Window

Script Location 选择程序的主程序，在计算器项目里，选择的是 main.py<br />Onefile 选择 One File ，因为一个文件看起来比较简洁<br />由于计算器项目带有 GUI，所以 Console Window 选择 Window Based (hide the console)<br />Icon 选择一个 ico 文件，此处不是必须操作，可以不设置<br />![](./img/1698372434963-a23f54e7-ff46-496b-9db8-5dc5379d7d68.png)<br />如果程序里面有自己的模块，必须把模块的目录添加到 Additional Files 里面。不然会出现 Failed to execute script XXX 错误<br />![](./img/1698372434881-b373a9bc-0a24-4d3a-8e84-78be9349d717.png)<br />在计算器程序里面所有的模块都在 calculation 目录下，所有需要将 calculation 路径添加到 Additional Files 里面<br />![](./img/1698372435003-f1c241d6-b11b-423e-9cff-8eb03989f0a0.png)<br />配置完成之后点击 CONVERT .PY TO .EXE 按钮<br />这样就完成一个计算器项目的打包。
<a name="NKzYk"></a>
### 3、查看打包效果 
程序完成打包后，可以点击 OPEN OUTPUT FOLDER 按钮，然后就会打开打包文件的路径。<br />![](./img/1698372727551-b086c9e5-5c12-4c76-aac4-2763552f4a18.png)<br />在打包文件目录中，可以看到一个 main.exe 文件，这就是打包文件。<br />点击 main.exe ，就可以看到一个计算器程序了。<br />![](./img/1698372435791-806c49f3-27fc-4f31-8361-eb9985053cb0.png)<br />至此，打包工作圆满完成。
<a name="tCnVD"></a>
## 5、总结一下
主要介绍了如何使用 auto-py-to-exe 来对 Python 程序进行打包。但只是介绍最简单的 Python 程序打包，如果想对复杂的程序进行打包，上面的配置肯定是不行的。<br />如果想更加深入的了解 auto-py-to-exe ，建议大家去研究一下 pyinstaller 。auto-py-to-exe 是基于 pyinstaller 的，研究 pyinstaller ，将会对深入使用 auto-py-to-exe 有非常明显的效果。<br />想更加深入了解 pyinstaller 可以去阅读官方文档。
