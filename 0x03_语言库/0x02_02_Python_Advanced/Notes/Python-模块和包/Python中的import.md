Python import
<a name="zArUj"></a>
## 1、使用 `__all__`  控制可被导入的变量
使用 `from module import *`  默认情况下会导入 module 里的所有变量，若只想从模块中导入其中几个变量，可以在 module 中使用 `__all__` 来控制想要被其他模块导入的变量。
```python
# profile.py
name='小明'
age=18
__all__=['name']
```
打开 Python console 验证一下
```python
>>> from profile import *
>>> print(name)
小明
>>> print(age)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'age' is not defined
```
`__all__` 仅对于使用`from module import *`  这种情况适用。<br />它经常在一个包的 `__init__.py` 中出现。
<a name="6oNDp"></a>
## 2、命名空间包的神奇之处
命名空间包，与熟悉的常规包不同的是，它没有 `__init__.py` 文件。<br />更为特殊的是，它可以跨空间地将两个不相邻的子包，合并成一个虚拟机的包，将其称之为 `命名空间包`。<br />例如，一个项目的部分代码布局如下
```
foo-package/
    spam/
        blah.py
bar-package/
    spam/
        grok.py
```
在这2个目录里，都有着共同的命名空间spam。在任何一个目录里都没有`__init__.py`文件。<br />如果将foo-package和bar-package都加到Python模块路径并尝试导入会发生什么？
```python
>>> import sys
>>> sys.path.extend(['foo-package', 'bar-package'])
>>> import spam.blah
>>> import spam.grok
>>>
```
当一个包为命名空间包时，他就不再和常规包一样具有 `__file_` 属性，取而代之的是 `__path__`
```python
>>> import sys
>>> sys.path.extend(['foo-package', 'bar-package'])
>>> import spam.blah
>>> import spam.grok
>>> spam.__path__
_NamespacePath(['foo-package/spam', 'bar-package/spam'])
>>> spam.__file__
Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
AttributeError: 'module' object has no attribute '__file__'
```
<a name="iP88A"></a>
## 3、模块重载中的一个坑
由于有 `sys.modules` 的存在，当导入一个已导入的模块时，实际上是没有效果的。<br />为了达到模块的重载，有的人会将已导入的包从 `sys.modules` 中移除后再导入<br />就像下面这样子
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
上面的例子里使用的是`import foo.bar` ，如果使用的是  `from foo import bar` 这种导入形式，可以发现重载是同样是无效的。<br />这应该算是一个小坑。
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
因此，在生产环境中可能需要避免重新加载模块。而在调试模式中，它会提供一定的便利。
