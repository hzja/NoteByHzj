Python<br />Addit 是一个Python模块，除了提供标准的字典语法外，Addit 生成的字典的值既可以使用属性来获取，也可以使用属性进行设置。<br />这意味着**不用**再写这样的字典了：
```python
body = {
    'query': {
        'filtered': {
            'query': {
                'match': {'description': 'addictive'}
            },
            'filter': {
                'term': {'created_by': 'Mats'}
            }
        }
    }
}
```
相反，只需编写以下三行代码就能完成目的：
```python
body = Dict()
body.query.filtered.query.match.description = 'addictive'
body.query.filtered.filter.term.created_by = 'Mats'
```
<a name="ghXIe"></a>
## 1、安装
可以通过** pip **安装：
```bash
pip install addict
```
或通过** conda ：**
```bash
conda install addict -c conda-forge
```
Addit 在Python2.7+和Python3上都可以运行。
<a name="WrbIs"></a>
## 2、用法
Addict 继承自字典，但在访问和设置其值方面更加灵活。使用 Addict 的字典是一种乐趣！<br />设置嵌套词典的项是极其舒服的：
```python
>>> from addict import Dict
>>> mapping = Dict()
>>> mapping.a.b.c.d.e = 2
>>> mapping
{'a': {'b': {'c': {'d': {'e': 2}}}}}
```
如果Dict是用任何可迭代值实例化的，它将遍历并克隆这些值，然后写入到对应的属性及值中，比如：
```python
>>> mapping = {'a': [{'b': 3}, {'b': 3}]}
>>> dictionary = Dict(mapping)
>>> dictionary.a[0].b
3
```
但** **`**mapping['a']**`** **不再与** **`**dictionary['a']**`** **相同。
```python
>>> mapping['a'] is dictionary['a']
False
```
当然，此特点仅限于构造函数，而不是在使用属性或设置值时：
```python
>>> a = Dict()
>>> b = [1, 2, 3]
>>> a.b = b
>>> a.b is b
True
```
<a name="XNBlu"></a>
## 3、要牢记的事情
记住，** int **不是有效的属性名，因此必须使用 get/setitem 语法 设置/获取 非字符串的 dict 键：
```python
>>> addicted = Dict()
>>> addicted.a.b.c.d.e = 2
>>> addicted[2] = [1, 2, 3]
{2: [1, 2, 3], 'a': {'b': {'c': {'d': {'e': 2}}}}}
```
不过，可以随意混合使用这两种语法：
```python
>>> addicted.a.b['c'].d.e
2
```
<a name="MsGwr"></a>
## 4、属性，如键、item等
Addit 不会覆盖** **`**dict**`** **的属性，因此以下操作将不起作用：
```python
>>> mapping = Dict()
>>> mapping.keys = 2
Traceback (most recent call last):
File "<stdin>", line 1, in <module>
 File "addict/addict.py", line 53, in __setattr__
 raise AttributeError("'Dict' object attribute '%s' is read-only" % name)
AttributeError: 'Dict' object attribute 'keys' is read-only
```
不过，使用下面这种方式就可以：
```python
>>> a = Dict()
>>> a['keys'] = 2
>>> a
{'keys': 2}
>>> a['keys']
2
```
<a name="Vez9A"></a>
## 5、默认值
对于不在字典中的键，Addit的行为如** **`**defaultdict(Dict)**`** **，因此丢失的键返回一个空的** Dict **而不是抛出** KeyError **如果此行为不是所需的，则可以使用以下方式恢复抛出KeyError：
```python
>>> class DictNoDefault(Dict):
>>>     def __missing__(self, key):
>>>         raise KeyError(key)
```
但请注意，这样会失去速记赋值功能(`**addicted.a.b.c.d.e = 2**`** **)
<a name="YG2gB"></a>
## 6、转化为普通字典
如果觉得将 Addict 传递到其他函数或模块并不安全，请使用** **`**to_dict()**`** **方法，它返回会把 Addict 转化为普通字典。
```python
>>> regular_dict = my_addict.to_dict()
>>> regular_dict.a = 2
Traceback (most recent call last):
File "<stdin>", line 1, in <module>
 AttributeError: 'dict' object has no attribute 'a'
```
当希望在几行代码中创建嵌套的字典，然后将其发送到不同的函数或模块时，这非常适合：
```python
body = Dict()
body.query.filtered.query.match.description = 'addictive'
body.query.filtered.filter.term.created_by = 'Mats'
third_party_module.search(query=body.to_dict())
```
<a name="awNuA"></a>
## 7、计数
**Dict **轻松访问和修改深度嵌套属性的能力使其成为计数的理想选择。使用Addict，还可以容易允许按多个级别计数，内部使用的原理是`**collections.Counter**`** **。<br />比如以下数据：
```python
data = [
    {'born': 1980, 'gender': 'M', 'eyes': 'green'},
    {'born': 1980, 'gender': 'F', 'eyes': 'green'},
    {'born': 1980, 'gender': 'M', 'eyes': 'blue'},
    {'born': 1980, 'gender': 'M', 'eyes': 'green'},
    {'born': 1980, 'gender': 'M', 'eyes': 'green'},
    {'born': 1980, 'gender': 'F', 'eyes': 'blue'},
    {'born': 1981, 'gender': 'M', 'eyes': 'blue'},
    {'born': 1981, 'gender': 'F', 'eyes': 'green'},
    {'born': 1981, 'gender': 'M', 'eyes': 'blue'},
    {'born': 1981, 'gender': 'F', 'eyes': 'blue'},
    {'born': 1981, 'gender': 'M', 'eyes': 'green'},
    {'born': 1981, 'gender': 'F', 'eyes': 'blue'}
]
```
如果想计算有多少人出生在born性别的gender使用eyes眼睛，可以很容易地计算出这些信息：
```python
counter = Dict()

for row in data:
    born = row['born']
    gender = row['gender']
    eyes = row['eyes']

    counter[born][gender][eyes] += 1 print(counter)

# 结果：{1980: {'M': {'blue': 1, 'green': 3}, 'F': {'blue': 1, 'green': 1}}, 1981: {'M': {'blue': 2, 'green': 1}, 'F': {'blue': 2, 'green': 1}}}
```
<a name="CtqHC"></a>
## 8、更新
普通字典的更新方式如下：
```python
>>> d = {'a': {'b': 3}}
>>> d.update({'a': {'c': 4}})
>>> print(d)
{'a': {'c': 4}}
```
**addict **的更新方式如下，它会递归并实际更新嵌套的字典：
```python
>>> D = Dict({'a': {'b': 3}})
>>> D.update({'a': {'c': 4}})
>>> print(D)
{'a': {'b': 3, 'c': 4}}
```
<a name="PIt4Y"></a>
## 9、Addict 是怎么来的？
这个模块完全是从用Python创建Elasticsearch查询的繁琐过程中发展而来的。每当发现自己在写了很复杂的字典逻辑时，只要记住没有必要这样做，使用 Addict 就行。
