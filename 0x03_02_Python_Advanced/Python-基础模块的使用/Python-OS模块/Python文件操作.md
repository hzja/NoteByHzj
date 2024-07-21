Python 文件操作
<a name="mZlaZ"></a>
### 1、显示当前目录
当想知道当前的工作目录是什么的时候，可以简单地使用`os`模块的`getcwd()`功能，或者使用`pathlib`的`cwd()`，如下所示。
```python
>>> # 第一种方法：显示当前目录
... import os
... print("当前工作目录:", os.getcwd())
... 
Current Work Directory: /Users/ycui1/PycharmProjects/Medium_Python_Tutorials
>>> # 第二种方法：或者也可以使用 pathlib
... from pathlib import Path
... print("当前工作目录:", Path.cwd())
... 
Current Work Directory: /Users/ycui1/PycharmProjects/Medium_Python_Tutorials
```
> 如果使用的是旧版本的Python（<3.4），则必须使用该os模块。

<a name="0DXzl"></a>
### 2、建立一个新目录
要创建目录，可以使用`os`模块的`mkdir()`功能。该函数将在指定的路径下创建目录，如果仅使用目录名称，则将在当前目录中创建文件夹，即绝对路径和相对路径的概念。
```python
>>> # 在当前文件夹创建新目录
... os.mkdir("test_folder")
... print("目录是否存在:", os.path.exists("test_folder"))
... 
目录是否存在: True
>>> # 在特定文件夹创建新目录
... os.mkdir('/Users/ycui1/PycharmProjects/tmp_folder')
... print("目录是否存在:", os.path.exists('/Users/ycui1/PycharmProjects/tmp_folder'))
... 
目录是否存在: True
```
但是，如果想要建立一个多层级的目录，比如文件夹中下的文件夹），则需要使用该`makedirs()`功能。
```python
>>> # 创建包含子目录的目录
... os.makedirs('tmp_level0/tmp_level1')
... print("目录是否存在:", os.path.exists("tmp_level0/tmp_level1"))
... 
Is the directory there: True
```
如果使用最新版本的Python（≥3.4），则可以考虑利用`pathlib`模块创建新目录。它不仅可以创建子目录，而且可以处理路径中所有丢失的目录。
```python
# 使用 pathlib
from pathlib import Path
Path("test_folder").mkdir(parents=True, exist_ok=True)
```
需要注意一个问题，如果尝试多次运行上述某些代码，可能会遇到问题“无法创建已经存在的新目录”。可以将`exist_ok`参设置为`True`来处理此问题（默认值False值将阻止创建目录）。
```python
>>> # 使用 pathlib
... from pathlib import Path
... Path("test_folder").mkdir(parents=True, exist_ok=False)
... 
Traceback (most recent call last):
  File "<input>", line 3, in <module>
  File "/Users/ycui1/.conda/envs/Medium/lib/python3.8/pathlib.py", line 1284, in mkdir
    self._accessor.mkdir(self, mode)
FileExistsError: [Errno 17] File exists: 'test_folder'
```
<a name="g36DL"></a>
### 3、删除目录和文件
完成对某些文件或文件夹的操作后，可能希望删除它。为此可以使用`os`模块中的`remove()`函数来删除文件。如果要删除文件夹，应该改用`rmdir()`。
```python
>>> # 删除一个文件
... print(f"* 删除文件前 {os.path.isfile('tmp.txt')}")
... os.remove('tmp.txt')
... print(f"* 删除文件后 {os.path.exists('tmp.txt')}")
... 
* 删除文件前 True
* 删除文件后 False
>>> # 删除一个文件夹
... print(f"* 删除文件夹前 {os.path.isdir('tmp_folder')}")
... os.rmdir('tmp_folder')
... print(f"* 删除文件夹后 {os.path.exists('tmp_folder')}")
... 
* 删除文件夹前 True
* 删除文件夹后 False
```
如果使用`pathlib`模块，可以使用`unlink()`方法，而删除目录可以使用`rmdir()`方法。
<a name="dzvVd"></a>
### 4、获取文件列表
当分析某个工作或机器学习项目进行数据处理时，需要获取特定目录中的文件列表。<br />通常，文件名具有匹配的模式。假设要查找目录中的所有.txt文件，可使用Path对象的方法`glob()`来实现。`glob()`方法创建了一个生成器，允许进行迭代。
```python
>>> txt_files = list(Path('.').glob("*.txt"))
... print("Txt files:", txt_files)
... 
Txt files: [PosixPath('hello_world.txt'), PosixPath('hello.txt')]
```
另外，直接使用`glob模块`也很方便，如下所示，通过创建可以使用的文件名列表，它具有相似的功能。在大多数情况下，例如文件读取和写入，两者都可以使用。
```python
>>> from glob import glob
... files = list(glob('h*'))
... print("以h开头的文件:", files)
... 
Files starting with h: ['hello_world.txt', 'hello.txt']
```
<a name="1bVSm"></a>
### 5、移动和复制文件
<a name="r1rjM"></a>
#### 移动文件
常规文件管理任务之一是移动和复制文件。在Python中，这些工作可以非常轻松地完成。要移动文件，只需将其旧目录替换为目标目录即可重命名该文件。假设需要将所有.txt文件移动到另一个文件夹，下面用`Path`来实现。
```python
>>> target_folder = Path("目标文件")
... target_folder.mkdir(parents=True,exist_ok=True)
... source_folder = Path('.')
... 
... txt_files = source_folder.glob('*.txt')
... for txt_file in txt_files:
...     filename = txt_file.name
...     target_path = target_folder.joinpath(filename)
...     print(f"** 移动文件 {filename}")
...     print("目标文件存在:", target_path.exists())
...     txt_file.rename(target_path)
...     print("目标文件存在:", target_path.exists(), '\n')
... 
** 移动文件 hello_world.txt
目标文件存在: False
目标文件存在: True 
** 移动文件 hello.txt
目标文件存在: False
目标文件存在: True
```
<a name="91r90"></a>
#### 复制文件
可以利用`_shutil_`模块中可用的功能，`_shutil_`模块是标准库中另一个用于文件操作的有用模块。可以`copy()`通过将源文件和目标文件指定为字符串来在模块中使用该函数。一个简单的例子如下所示。当然，也可以将`copy()`函数与`glob()`函数结合使用，以处理具有相同模式的一堆文件。
```python
>>> import shutil
... 
... source_file = "target_folder/hello.txt"
... target_file = "hello2.txt"
... target_file_path = Path(target_file)
... print("* 复制前，文件存在:", target_file_path.exists())
... shutil.copy(source_file, target_file)
... print("* 复制后，文件存在:", target_file_path.exists())
... 
* 复制前，文件存在: False
* 复制后，文件存在: True
```
<a name="NCAMb"></a>
### 6、检查目录/文件
上面的示例中一直在使用`exists()`方法来检查是否存在特定路径。如果存在，返回True；如果不存在，则返回False。此功能在`os`和`pathlib`模块中均可用，各自的用法如下。
```python
# os 模块中 exists() 用法
os.path.exists('path_to_check')
# pathlib 模块中 exists() 用法
Path('directory_path').exists()
```
使用`pathlib`，还可以检查路径是目录还是文件。
```python
# 检查路径是否是目录
os.path.isdir('需要检查的路径')
Path('需要检查的路径').is_dir()
# 检查路径是否是文件
os.path.isfile('需要检查的路径')
Path('需要检查的路径').is_file()
```
<a name="eL4hC"></a>
### 7、获取文件信息
<a name="xijhU"></a>
#### 文件名称
处理文件时，许多情况下都需要提取文件名。使用Path非常简单，可以在Path对象上查看name属性`path.name`。如果不想带后缀，可以查看stem属性`path.stem`。
```python
for py_file in Path().glob('c*.py'):
...     print('Name with extension:', py_file.name)
...     print('Name only:', py_file.stem)
... 
带文件后缀: closures.py
只有文件名: closures
带文件后缀: counter.py
只有文件名: counter
带文件后缀: context_management.py
只有文件名: context_management
```
<a name="6X22A"></a>
#### 文件后缀
如果想单独提取文件的后缀，可查看Path对象的`suffix`属性。
```python
>>> file_path = Path('closures.py')
... print("文件后缀:", file_path.suffix)
... 
File Extension: .py
```
<a name="tPXHH"></a>
#### 文件更多信息
如果要获取有关文件的更多信息，例如文件大小和修改时间，则可以使用该`stat()`方法，该方法和`os.stat()`一样。
```python
>>> # 路径 path 对象
... current_file_path = Path('iterable_usages.py')
... file_stat = current_file_path.stat()
... 
>>> # 获取文件大小:
... print("文件大小（Bytes）:", file_stat.st_size)
文件大小（Bytes）: 3531
>>> # 获取最近访问时间
... print("最近访问时间:", file_stat.st_atime)
最近访问时间: 1595435202.310935
>>> # 获取最近修改时间
... print("最近修改时间:", file_stat.st_mtime)
最近修改时间: 1594127561.3204417
```
<a name="PZJNN"></a>
### 8、读取文件
最重要的文件操作之一就是从文件中读取数据。读取文件，最常规的方法是使用内置`open()`函数创建文件对象。默认情况下，该函数将以读取模式打开文件，并将文件中的数据视为文本。
```python
>>> # 读取所有的文本
... with open("hello2.txt", 'r') as file:
...     print(file.read())
... 
Hello World!
Hello Python!
>>> # 逐行的读取
... with open("hello2.txt", 'r') as file:
...     for i, line in enumerate(file, 1):
...         print(f"* 读取行 #{i}: {line}") 
... 
* 读取行 #1: Hello World!
* 读取行 #2: Hello Python!
```
如果文件中没有太多数据，则可以使用该`read()`方法一次读取所有内容。但如果文件很大，则应考虑使用生成器，生成器可以逐行处理数据。<br />默认将文件内容视为文本。如果要使用二进制文件，则应明确指定用`r`还是`rb`。<br />另一个棘手的问题是**文件的编码**。在正常情况下，`open()`处理编码使用`utf-8`编码，如果要使用其他编码处理文件，应设置`encoding`参数。
<a name="e5c13829"></a>
### 9、写入文件
仍然使用`open()`函数，将模式改为`w`或`a`打开文件来创建文件对象。`w`模式下会覆盖旧数据写入新数据，`a`模式下可在原有数据基础上增加新数据。
```python
>>> # 向文件中写入新数据
... with open("hello3.txt", 'w') as file:
...     text_to_write = "Hello Files From Writing"
...     file.write(text_to_write)
... 
>>> # 增加一些数据
... with open("hello3.txt", 'a') as file:
...     text_to_write = "\nHello Files From Appending"
...     file.write(text_to_write)
... 
>>> # 检查文件数据是否正确
... with open("hello3.txt") as file:
...     print(file.read())
... 
Hello Files From Writing
Hello Files From Appending
```
上面每次打开文件时都使用`with`语句。<br />`with`语句创建了一个处理文件的上下文，当完成文件操作后，它可以关闭文件对象。这点很重要，如果不及时关闭打开的文件对象，它很有可能会被损坏。
<a name="6XhFU"></a>
### 10、压缩和解压缩文件
<a name="P7G1R"></a>
#### 压缩文件
`zipfile`模块提供了文件压缩的功能。使用`ZipFile()`函数创建一个`zip`文件对象，类似于对open()函数所做的操作，两者都涉及创建由上下文管理器管理的文件对象。
```python
>>> from zipfile import ZipFile
... 
... # 创建压缩文件
... with ZipFile('text_files.zip', 'w') as file:
...     for txt_file in Path().glob('*.txt'):
...         print(f"*添加文件: {txt_file.name} 到压缩文件")
...         file.write(txt_file)
... 
*添加文件: hello3.txt 到压缩文件
*添加文件: hello2.txt 到压缩文件
```
<a name="ygHkq"></a>
#### 解压缩文件
```python
>>> # 解压缩文件
... with ZipFile('text_files.zip') as zip_file:
...     zip_file.printdir()
...     zip_file.extractall()
... 
File Name                                             Modified             Size
hello3.txt                                     2020-07-30 20:29:50           51
hello2.txt                                     2020-07-30 18:29:52           26
```
