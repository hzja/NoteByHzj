Python<br />推荐几个在自动化办公领域当中非常好用的Python模块，可以帮助大家在工作当中及大地提高效率，避免重复机械化地操作流程。
<a name="UhYSW"></a>
## **1、Pathlib**
涉及到文件系统操作的时候，相信不少人仍然在使用Python当中的OS模块，相比较之下，Pathlib模块有着不少优势，来看几个简单的案例。
<a name="CSdVZ"></a>
### 删除与创建目录
例如可以来创建与删除目录，代码如下：
```python
from pathlib import Path

currentPath = Path.cwd()
makePath = currentPath / 'pythonPractice'
makePath.mkdir()
```
那么同样的，删除目录的代码为：
```python
currentPath = Path.cwd()
delPath = currentPath / 'pythonPractice'
delPath.rmdir()
```
<a name="De8JC"></a>
### 获取目录
例如想要获取当前所在的目录的路径，代码如下：
```python
currentPath = Path.cwd()
print(currentPath)
```
以及电脑用户所在的目录：
```python
homePath = Path.home()
print(homePath) 
```
<a name="qnxKi"></a>
### 路径的拼接
例如想要拼接出桌面的绝对路径，代码如下：
```python
Path(Path.home(), "Desktop")
```
也可以是：
```python
Path.joinpath(Path.home(), "Desktop")
```
<a name="CnRTV"></a>
### 目录文件的判断
针对指定的路径，能够来判断是否是文件夹以及是否是一个文件，代码如下：
```python
input_path = r"指定的路径"

if Path(input_path ).exists():
	if Path(input_path ).is_file():
		print("是文件哦！")
	elif Path(input_path ).is_dir():
		print("是文件夹哦！")
	else:
		print("路径给的不对!")
```
<a name="kzPXU"></a>
## **2、glob**
Python当中的`glob`模块主要是用来查找符合特定规则的目录和文件，并将搜索到的结果返回到一个列表当中去。<br />由于该模块支持使用正则通配符来进行查找，因此使用起来也是特别的方便，来看一下简单的案例，
```python
path1 = r".\[0-9].jpg"
glob.glob(path1)
```
outut
```python
['.\\1.jpg', '.\\2.jpg', '.\\3.jpg', ......]
```
其中经常会用到的通配符有

- `*`：匹配0个或者多个字符
- `**`：匹配所有文件、目录，子目录和子目录里面的文件
- `[]`：匹配指定范围内的字符，例如`[1-9]`匹配的是1-9以内的字符
- `[!]`：匹配不在指定范围内的字符

再来看几个案例，代码如下
```python
for fname in glob.glob("./*.py"):
    print(fname)
```
上面的代码打印出了当前目录下的所有py后缀的文件，再来看一下案例
```python
for fname in glob.glob("./file[!0-9].py"):
    print(fname)
```
上面代码则是打印出了以filename开头的非数字符号的py文件。
