<a name="x2hbR"></a>
## 1、概述
在使用Python 命令或者 Python的命令行工具的时候，一个痛点是没有补全。比如`python -m`后面输入包名字，就没有提示，每次想运行一个http server的时候，都需要搜索一下http服务的包名。另外，像pip，pipx等命令也没有提示，使用不太方便。<br />偶然看到[**argcomplete**](https://github.com/kislyuk/argcomplete)这个库，按tab键就可以给Python的命令行添加自动补全，简直是使用Python的一个神器。<br />具体来说，argcomplete有下面的特点：

- 官方支持支持bash和zsh两种shell，对tcsh和fish有第三方贡献者提供的支持（不好意思Windows用户这里又被当做二等公民了 ）
- 可以对python命令和pip命令进行补全
- 其他任何以argparse解析的第三方包的命令都可以用自动补全，添加argcomplete的几行代码就行

下面具体展开怎么对已有的工具启用自动补全，以及如何让自己的Python包支持argcomplete。
<a name="B4Yt8"></a>
## 2、对Python和pip启用自动补全
首先通过pip命令来安装argcomplete：
```bash
pip install argcomplete
```
然后执行下面的语句来启用对Python和pip的自动补全：
```bash
activate-global-python-argcomplete
```
重启Shell，试试输入pip然后按tab，发现就会列出所有的命令选项。
<a name="awsZ9"></a>
## 3、如何对别的第三方库启用自动补全
有些库的命令行程序是已经支持argcomplete补全，只需要用下面的命令来激活：
```
eval "$(register-python-argcomplete <python-app-name>)"
```
例如 pipx 包安装后会在系统安装一个命令行程序pipx，且pipx已经支持argcomplete，就可以用下面的命令来激活自动补全：
```
eval "$(register-python-argcomplete pipx)"
```
激活后输入pipx in 按tab键，就可以列出pipx所有以in开头的命令，再按tab键会在各个候选命令之间切换。<br />⚠️注意：这个激活命令是只对哪些代码中已经支持了argcomplete语句的程序才生效，如果代码中无这些语句，那是不生效的。
<a name="DwZUc"></a>
## 4、如何让自己的Python库支持自动补全
只需要增加下面几行代码，就能让你的库的命令行支持自动补全：
```python
# 在ArgumentParser对象初始化前增加这两行
# PYTHON_ARGCOMPLETE_OK
import argcomplete, argparse

# 原有代码
parser = argparse.ArgumentParser() 
...

# 在调用parse_args()函数前增加这一行
argcomplete.autocomplete(parser)

# 原有代码
args = parser.parse_args()
...
```
然后你的包安装后，对应的命令行程序就可以用`eval "$(register-python-argcomplete )"`来补全了。<br />⚠️注意：如果程序执行到`argcomplete.autocomplete()` 被调用的地方耗时很久的话，用户按tab就会有明显的延迟感。所以尽量将一些比较耗时的操作放在`argcomplete.autocomplete()` 语句后面，比如一些`import`语句，常常比较耗时，可以往后放。
