Python 字典
<a name="p6ppg"></a>
## 通过序列生成字典
将下面的序列转换为dict类型。
```python
lst = [('a', 1), ('b', 2), ('c', 3)]
```
<a name="B205s"></a>
### 普通的写法
```python
for k, v in lst:
    dic[k] = v
```
<a name="XE6RB"></a>
### 更pythonic的写法
利用字典推导式快速生成字典。
```python
{k: v for k, v in lst}
```
<a name="Nu3Xb"></a>
## key的默认值
当指定key不存在时，将value设置为 0。
<a name="rPeIy"></a>
### 普通的写法
```python
if key not in dct:
    dct[key] = 0
```
<a name="jGOQL"></a>
### pythonic的写法
```python
dct[key] = dct.get(key, 0)
```
<a name="TVQCH"></a>
## 交换key与value
<a name="oAncc"></a>
### 普通的写法
```python
dic = {'Python': 1, 'Java': 2}
new_dic = {}
for k, v in dic.items():
    new_dic[v] = k
```
<a name="PNo6o"></a>
### 更pythonic的写法
```python
dic = {'Python': 1, 'Java': 2}
new_dic = {v: k for k, v in dic.items()}
```
<a name="v0Jf0"></a>
## 序列修改和初始化
示例数据
```python
lst = [('a', 1), ('b', 2), ('c', 3)]
dic = {'a': [0]}
```
如果需要根据lst来更新dic中的数据，当key存在，则将value添加到原序列末尾，否则初始化value并用序列保存。
<a name="HZFXO"></a>
### 普通的写法
```python
for key, value in lst:
    if key in dic:
        dic[key].append(value)
    else:
        dic[key] = [value]
```
<a name="lovuS"></a>
### 更pythonic的写法
```python
for (key, value) in lst:
    group = dic.setdefault(key, [])
    group.append(value)
# dic：{'a': [0, 1], 'b': [2], 'c': [3]}
```
`setdefault(key, default)`会先判断`key`是否存在，存在则返回`dct[key]`，不存在则把`dct[key]`设为 `[]` 并返回。
<a name="HdiSy"></a>
## key，items的集合运算
如果现在需要获取两个字典的key相交的部分的映射信息。
<a name="QoPWr"></a>
### 普通的写法
```python
dic1 = {'Python': 1, 'Java': 2, 'C': 3}
dic2 = {'Python': 3, 'Java': 2, 'C++': 1}

new_dic = {}
for k, v in dic1.items():
    if k in dic2.keys():
        new_dic[k] = v
print(new_dic)

# {'Python': 1, 'Java': 2}
```
<a name="UoJIi"></a>
### 更pythonic的写法
```python
dic1 = {'Python': 1, 'Java': 2, 'C': 3}
dic2 = {'Python': 3, 'Java': 2, 'C++': 1}

print({k: dic1[k] for k in dic1.keys() & dic2.keys()})

# {'Python': 1, 'Java': 2}
```
这里的`dic1.keys() & dic2.keys()`用到的就是 `keys()`进行集合运算，`items()`同样可以进行集合运算。<br />如果现在要获取两个字典中 key，value 完全相同的部分。
```python
dic1 = {'Python': 1, 'Java': 2, 'C': 3}
dic2 = {'Python': 3, 'Java': 2, 'C++': 1}

print(dic1.items() & dic2.items())

# {('Java', 2)}
```
灵活运用 keys，items() 集合运算的特性，可以快速提取我们想要的内容。
<a name="vRwmr"></a>
## 按key或value对字典排序
使用`sorted()`函数快速实现对key或value的排序。
```python
dic = {'a': 2, 'b': 1, 'c': 3, 'd': 0}
lst1 = sorted(dic.items(), key=lambda x: x[0], reverse=False)
# [('a', 2), ('b', 1), ('c', 3), ('d', 0)]
lst2 = sorted(dic.items(), key=lambda x: x[1], reverse=False)
# [('d', 0), ('b', 1), ('a', 2), ('c', 3)]
print('按照键降序：', {key: value for key, value in lst1})
print('按照值降序：', {key: value for key, value in lst2})

# 按照键降序： {'a': 2, 'b': 1, 'c': 3, 'd': 0}
# 按照值降序： {'d': 0, 'b': 1, 'a': 2, 'c': 3}
```
<a name="bLLr9"></a>
## 多个字典排序
如果一个序列中包含多个字典，现在要根据条件对这些字典继续排序。同样可以使用`sorted()`函数来实现。
```python
dict_list = [
    {'letter': 'B', 'number': '2'},
    {'letter': 'A', 'number': '3'},
    {'letter': 'B', 'number': '1'}
]

# 按 letter 排序
print(sorted(dict_list,
             key=lambda dic: dic['letter']))
# 按 letter, number 排序
print(sorted(dict_list,
             key=lambda dic: (dic['letter'], dic['number'])))
             
# [{'letter': 'A', 'number': '3'}, {'letter': 'B', 'number': '2'}, {'letter': 'B', 'number': '1'}]
# [{'letter': 'A', 'number': '3'}, {'letter': 'B', 'number': '1'}, {'letter': 'B', 'number': '2'}]
```
当然，如果知道`itemgetter()`的话，上面的代码就可以改变一下，执行速度会更快。
```python
from operator import itemgetter

print(sorted(dict_list,
             key=itemgetter('letter')))
print(sorted(dict_list,
             key=itemgetter('letter', 'number')))
```
`itemgetter()`获取的不是值，而是定义了一个函数，通过该函数作用到目标对象上。
