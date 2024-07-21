Python 模块重载
<a name="ivFaH"></a>
### 环境准备
新建一个 foo 文件夹，其下包含一个 bar.py 文件
```bash
$ tree foo
foo
└── bar.py
0 directories, 1 file
```
bar.py 的内容非常简单，只写了个 print 语句
```python
print("successful to be imported")
```
只要 bar.py 被导入一次，就被执行一次 print
<a name="ttU3V"></a>
### 禁止重复导入
由于有 sys.modules 的存在，当导入一个已导入的模块时，实际上是没有效果的。
```python
>>> from foo import bar
successful to be imported
>>> from foo import bar
>>>
```
<a name="e2a5afc7"></a>
### 重载模块方法一
如果使用的 python2（记得前面在 foo 文件夹下加一个 `__init__.py`），有一个 reload 的方法可以直接使用
```python
>>> from foo import bar
successful to be imported
>>> from foo import bar
>>>
>>> reload(bar)
successful to be imported
<module 'foo.bar' from 'foo/bar.pyc'>
```
如果使用的 python3 那方法就多了，详细请看下面
<a name="b129edd6"></a>
### 重载模块方法二
如果使用 Python3.0 -> 3.3，那么可以使用 `imp.reload` 方法
```python
>>> from foo import bar
successful to be imported
>>> from foo import bar
>>>
>>> import imp
>>> imp.reload(bar)
successful to be imported
<module 'foo.bar' from '/Users/MING/Code/Python/foo/bar.py'>
```
但是这个方法在 Python 3.4+，就不推荐使用了
```
<stdin>:1: DeprecationWarning: the imp module is deprecated in favour of importlib; see the module's documentation for alternative uses
```
<a name="XYYHz"></a>
### 重载模块方法三
如果使用的 Python 3.4+，使用 `importlib.reload` 方法
```python
>>> from foo import bar
successful to be imported
>>> from foo import bar
>>>
>>> import importlib
>>> importlib.reload(bar)
successful to be imported
<module 'foo.bar' from '/Users/MING/Code/Python/foo/bar.py'>
```
<a name="a0AhV"></a>
## 重载模块方法四
如果对包的加载器有所了解，还可以使用下面的方法
```python
>>> from foo import bar
successful to be imported
>>> from foo import bar
>>>
>>> bar.__spec__.loader.load_module()
successful to be imported
<module 'foo.bar' from '/Users/MING/Code/Python/foo/bar.py'>
```
<a name="b0coU"></a>
### 重载模块方法五
既然影响重复导入的是 sys.modules，那只要将已导入的包从其中移除是不是就好了呢？
```python
>>> import foo.bar
successful to be imported
>>>
>>> import foo.bar
>>>
>>> import sys
>>> sys.modules['foo.bar']
<module 'foo.bar' from '/Users/MING/Code/Python/foo/bar.py'>
>>> del sys.modules['foo.bar']
>>>
>>> import foo.bar
successful to be imported
```
有没有发现在前面的例子里使用的都是 `from foo import bar`，在这个例子里，却使用 `import foo.bar`，这是为什么呢？<br />这是因为如果使用 `from foo import bar` 这种方式，想使用移除 sys.modules 来重载模块这种方法是失效的。<br />这应该算是一个小坑，不知道的人，会掉入坑中爬不出来。
```python
>>> import foo.bar
successful to be imported
>>>
>>> import foo.bar
>>>
>>> import sys
>>> del sys.modules['foo.bar']
>>> from foo import bar
>>>
```
