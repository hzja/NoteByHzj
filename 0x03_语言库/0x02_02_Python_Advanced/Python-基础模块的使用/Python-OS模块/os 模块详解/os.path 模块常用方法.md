Python<br />`os.path` 在Python中也算是一个常用的模块，特别是和文件系统打交道时，涉及到文件、目录操作经常会用到，去看一些框架的源代码的时候也经常会使用到这些方法。<br />如果去看官方文档，`os.path` 大概提供了近20个方法，其实比较常用的9个方法，可以跟着操作下，很容易就记住了。其实看方法名都知道具体是做什么的。<br />![2021-09-22-21-13-45-828079.png](./img/1632316437864-67a606ff-7e64-4424-8e8b-8b8d517cfbc0.png)
<a name="t7Hd6"></a>
### 1、`os.path.abspath`
获取文件的绝对路径
```python
path = "test.py"
print(os.path.abspath(path)) # C:\Users\fcant\workspace\python_scripts\test.py
```
<a name="tjeGn"></a>
### 2、`os.path.basename`
获取文件名，不包含目录部分， 如果该路径本身就是个目录，那么返回的就是空
```python
path = "C:/Users/fcant/workspace/python_scripts/test_path.py"
print(os.path.basename(path))  # test_path.py

path = "../test/"
print(os.path.basename(path)) # 空字符串
```
<a name="jvy3k"></a>
### 3、`os.path.dirname`
获取文件的目录部分， dirname + basename 就可以构成文件的完整路径名
```python
path = "C:/Users/fcant/workspace/python_scripts/test_path.py"
print(os.path.dirname(path))  # C:/Users/fcant/workspace/python_scripts
```
<a name="U1t5K"></a>
### 4、`os.path.exists`
判断路径是否存在，这里的路径包括目录和文件（在linux系统，一切皆文件）， 如果直接复制代码，可能返回的结果是False，因为系统可能没有这些目录。
```python
path = "C:/Users/fcant/workspace/python_scripts/test_path.py"
print(os.path.exists(path)) # True

path = "C:/Users/fcant/workspace/python_scripts"
print(os.path.exists(path)) True
```
<a name="BB5x9"></a>
### 5、`os.path.getsize`
获取文件大小，也可以获取目录（所有文件的）大小，取决于传的路径参数是文件还是目录。单位是字节
```python
path = "C:/Users/fcant/workspace/python_scripts/test_path.py"
print(os.path.getsize(path)) # 177 
```
<a name="cK7us"></a>
### 6、`os.path.split`
`split` 方法会将路径切割成两部分，以最后一个斜杠作为切割点，第一部分是文件所在的目录， 第二部分文件名本身。如果传入的path是以“/”结尾，那么第二部就是空字符串
```python
path = "C:/Users/fcant/workspace/python_scripts/test_path.py"
print(os.path.split(path))  # ('C:/Users/fcant/workspace/python_scripts', 'test_path.py')

path = "C:/Users/fcant/workspace/python_scripts/"
print(os.path.split(path))  # ('C:/Users/fcant/workspace/python_scripts', '')
```
<a name="l8MQq"></a>
### 7、`os.path.join`
`join`是与`split`对应的方法，用于拼接文件路径，一般用于已知a文件的完整路径，在与a同目录下创建个b文件就可以用到`join`方法。
```python
a_path = "C:/Users/fcant/workspace/python_scripts/a.py"
dir = os.path.split(a_path)[0]
print(os.path.join(dir, "b.py")) # C:/Users/fcant/workspace/python_scripts\b.py
```
<a name="EvH5K"></a>
### 8、`os.path.isdir`
判断路径是否为目录，注意，如果该目录不存在也不会报错，而是直接返回False
```python
path = "C:/Users/fcant/workspace/python_scripts/"
print(os.path.isdir(path)) # True
```
<a name="aX688"></a>
### 9、`os.path.isfile`
判断路径是否为文件，注意，如果该文件不存在也不会报错，而是直接返回False
```python
print(os.path.isfile(__file__))  # True
a_path = "C:/Users/fcant/workspace/python_scripts/a.py"
print(os.path.isfile(a_path)) # False (文件压根不存在)
```
官方文档：[https://docs.python.org/3/library/os.path.html](https://docs.python.org/3/library/os.path.html)
