six是用于Python2与Python3兼容的库。它存在的目的是为了拥有无需修改即可在Python 2和Python 3上同时工作的代码。话虽这么说，但是这并不代表在Python 3中引用该库就可以轻松地跑Python 2的代码。

实际上，SIX是重定义了在python2和3中有差异的函数，例如dict的获取全部键值函数：

- 在Python2中是dictionary.iterkeys()
- 在Python3中是dictionary.keys()
- 在six中是six.iterkeys(dictionary)（当然对应版本的原函数也能够使用）
<a name="qajy9"></a>
# Source
[Six: Python 2 and 3 Compatibility Library — six 1.15.0 documentation](https://six.readthedocs.io/)
