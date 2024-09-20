Python探针<br />探针的实现主要涉及以下几个知识点:

- sys.meta_path
- sitecustomize.py
<a name="eGzGx"></a>
## sys.meta_path
`sys.meta_path` 这个简单的来说就是可以实现 `import hook` 的功能， 当执行 `import` 相关的操作时，会触发 `sys.meta_path` 列表中定义的对象。关于 `sys.meta_path` 更详细的资料请查阅 python 文档中 sys.meta_path 相关内容以及 PEP 0302 。<br />`sys.meta_path` 中的对象需要实现一个 `find_module` 方法， 这个 `find_module` 方法返回 `None` 或一个实现了 `load_module` 方法的对象 :
```python
import sys
class MetaPathFinder:
    def find_module(self, fullname, path=None):
        print('find_module {}'.format(fullname))
        return MetaPathLoader()
class MetaPathLoader:
    def load_module(self, fullname):
        print('load_module {}'.format(fullname))
        sys.modules[fullname] = sys
        return sys
sys.meta_path.insert(0, MetaPathFinder())
if __name__ == '__main__':
    import http
    print(http)
    print(http.version_info)
```
`load_module` 方法返回一个 `module` 对象，这个对象就是 import 的 module 对象了。比如上面那样就把 `http` 替换为 `sys` 这个 module 了。
```bash
$ python meta_path1.py
find_module http
load_module http
<module 'sys' (built-in)>
sys.version_info(major=3, minor=5, micro=1, releaselevel='final', serial=0)
```
通过 `sys.meta_path` 就可以实现 import hook 的功能：当 import 预定的 module 时，对这个 module 里的对象进行替换， 从而实现获取函数或方法的执行时间等探测信息。<br />上面说到了替换，那么怎么对一个对象进行替换操作呢？对于函数对象，可以使用装饰器的方式来替换函数对象:
```python
import functools
import time
def func_wrapper(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print('start func')
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print('spent {}s'.format(end - start))
        return result
    return wrapper
def sleep(n):
    time.sleep(n)
    return n
if __name__ == '__main__':
    func = func_wrapper(sleep)
    print(func(3))
```
执行结果:
```bash
$ python func_wrapper.py
start func
spent 3.004966974258423s
3
```
下面来实现一个计算指定模块的指定函数的执行时间的功能。<br />假设模块文件是 hello.py:
```python
import time
def sleep(n):
    time.sleep(n)
    return n
```
 import hook 是 hook.py:
```python
import functools
import importlib
import sys
import time
_hook_modules = {'hello'}
class MetaPathFinder:
    def find_module(self, fullname, path=None):
        print('find_module {}'.format(fullname))
        if fullname in _hook_modules:
            return MetaPathLoader()
class MetaPathLoader:
    def load_module(self, fullname):
        print('load_module {}'.format(fullname))
        # ``sys.modules`` 中保存的是已经导入过的 module
        if fullname in sys.modules:
            return sys.modules[fullname]
        # 先从 sys.meta_path 中删除自定义的 finder
        # 防止下面执行 import_module 的时候再次触发此 finder
        # 从而出现递归调用的问题
        finder = sys.meta_path.pop(0)
        # 导入 module
        module = importlib.import_module(fullname)
        module_hook(fullname, module)
        sys.meta_path.insert(0, finder)
        return module
sys.meta_path.insert(0, MetaPathFinder())
def module_hook(fullname, module):
    if fullname == 'hello':
        module.sleep = func_wrapper(module.sleep)
def func_wrapper(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print('start func')
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print('spent {}s'.format(end - start))
        return result
    return wrapper
```
测试代码:
```python
>>> import hook
>>> import hello
find_module hello
load_module hello
>>>
>>> hello.sleep(3)
start func
spent 3.0029919147491455s
3
>>>
```
其实上面的代码已经实现了探针的基本功能。不过有一个问题就是上面的代码需要显式的执行 `import hook` 操作才会注册上定义的 hook。<br />那么有没有办法在启动 python 解释器的时候自动执行 `import hook` 的操作呢？答案就是可以通过定义 `sitecustomize.py` 的方式来实现这个功能。
<a name="BOhjH"></a>
## sitecustomize.py
简单的说就是，python 解释器初始化的时候会自动 `import PYTHONPATH` 下存在的 `sitecustomize` 和 `usercustomize` 模块:<br />实验项目的目录结构如下 :
```bash
$ tree
.
├── sitecustomize.py
└── usercustomize.py
```
sitecustomize.py:
```bash
$ cat sitecustomize.py
print('this is sitecustomize')
```
usercustomize.py:
```bash
$ cat usercustomize.py
print('this is usercustomize')
```
把当前目录加到 `PYTHONPATH` 中，然后看看效果:
```bash
$ export PYTHONPATH=.
$ python
this is sitecustomize       <----
this is usercustomize       <----
Python 3.5.1 (default, Dec 24 2015, 17:20:27)
[GCC 4.2.1 Compatible Apple LLVM 7.0.2 (clang-700.1.81)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>>
```
可以看到确实自动导入了。所以可以把之前的探测程序改为支持自动执行 import hook。<br />目录结构:
```bash
$ tree
.
├── hello.py
├── hook.py
├── sitecustomize.py
```
sitecustomize.py:
```bash
$ cat sitecustomize.py
import hook
```
结果:
```python
$ export PYTHONPATH=.
$ python
find_module usercustomize
Python 3.5.1 (default, Dec 24 2015, 17:20:27)
[GCC 4.2.1 Compatible Apple LLVM 7.0.2 (clang-700.1.81)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
find_module readline
find_module atexit
find_module rlcompleter
>>>
>>> import hello
find_module hello
load_module hello
>>>
>>> hello.sleep(3)
start func
spent 3.005002021789551s
3
```
不过上面的探测程序其实还有一个问题，那就是需要手动修改 `PYTHONPATH` 。用过探针的朋友应该会记得， 使用 newrelic 之类的探针只需要执行一条命令就 可以了：`newrelic-admin run-program python hello.py` 实际上修改 `PYTHONPATH` 的操作是在`newrelic-admin` 这个程序里完成的。<br />下面也要来实现一个类似的命令行程序，就叫 `agent.py` 。
<a name="TIXDZ"></a>
## agent
还是在上一个程序的基础上修改。先调整一个目录结构，把 `hook` 操作放到一个单独的目录下， 方便设置 `PYTHONPATH` 后不会有其他的干扰。
```bash
$ mkdir bootstrap
$ mv hook.py bootstrap/_hook.py
$ touch bootstrap/__init__.py
$ touch agent.py
$ tree
.
├── bootstrap
│   ├── __init__.py
│   ├── _hook.py
│   └── sitecustomize.py
├── hello.py
├── test.py
├── agent.py
```
`bootstrap/sitecustomize.py` 的内容修改为:
```bash
$ cat bootstrap/sitecustomize.py
import _hook
```
agent.py 的内容如下:
```python
import os
import sys
current_dir = os.path.dirname(os.path.realpath(__file__))
boot_dir = os.path.join(current_dir, 'bootstrap')
def main():
    args = sys.argv[1:]
    os.environ['PYTHONPATH'] = boot_dir
    # 执行后面的 python 程序命令
    # sys.executable 是 python 解释器程序的绝对路径 ``which python``
    # >>> sys.executable
    # '/usr/local/var/pyenv/versions/3.5.1/bin/python3.5'
    os.execl(sys.executable, sys.executable, *args)
if __name__ == '__main__':
    main()
```
`test.py` 的内容为:
```bash
$ cat test.py
import sys
import hello
print(sys.argv)
print(hello.sleep(3))
```
使用方法:
```bash
$ python agent.py test.py arg1 arg2
find_module usercustomize
find_module hello
load_module hello
['test.py', 'arg1', 'arg2']
start func
spent 3.005035161972046s
3
```
至此，就实现了一个简单的 python 探针程序。当然，跟实际使用的探针程序相比肯定是有很大的差距的，主要是探索一下探针背后的实现原理。
