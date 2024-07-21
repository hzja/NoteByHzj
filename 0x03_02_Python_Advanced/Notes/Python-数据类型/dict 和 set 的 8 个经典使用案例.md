Python
<a name="UrGJL"></a>
## 1、字典更新
使用字典的时候需要给现有的字典插入数据使用`update()`方法
```python
# 初始信息
info_dict = {'姓名': '张三', '性别': '男', '住址': '北京'}

# 添加信息
info_dict.update({'学校': '清华', '学号': '007'})

'''
{'姓名': '张三', '性别': '男', '住址': '北京', '学校': '清华', '学号': '007'}
'''
```
<a name="IAMSy"></a>
## 2、字典合并
合并字典可以使用`{**d1,**d2}` 实现合并 d1 和 d2，返回一个新字典：
```python
# 字典1
dict_1 = {'a': 'aa', 'b': 'bb', 'c': 'cc'}

# 字典2
dict_2 = {'1': 11, '2': 22, '3': 33}

# 合并
new_dict = {**dict_1, **dict_2}

'''
{'a': 'aa', 'b': 'bb', 'c': 'cc', '1': 11, '2': 22, '3': 33}
'''
```
<a name="ikq0a"></a>
## 3、字典排序key
给字典排序这里使用的是`sort()`方法按照键进行排序
```python
# 初始字典
my_dict = {'1': 11, '8': 88, '6': 66, '7': 77, '5': 55, '2': 22, '9': 99}

# 按照key对字典排序
sort_dict = sorted(my_dict.items(),key=lambda x: x[0])
print(sort_dict)


'''
[('1', 11), ('2', 22), ('5', 55), ('6', 66), ('7', 77), ('8', 88), ('9', 99)]
'''
```
<a name="jL755"></a>
## 4、字典排序key
给字典排序这里使用的是`sort()`方法按照值进行排序<br />`my_dict.items()` 返回元素为 (key, value) 的可迭代类型（Iterable），key 函数的参数 x 便是元素 `(key, value)`，所以 x[1] 取到字典的值。
```python
# 初始字典
my_dict = {'1': 11, '8': 18, '6': 16, '7': 17, '5': 15, '2': 12, '9': 19}

# 按照value对字典排序
sort_dict = sorted(my_dict.items(),key=lambda x: x[1])
print(sort_dict)


'''
[('1', 11), ('2', 12), ('5', 15), ('6', 16), ('7', 17), ('8', 18), ('9', 19)]
'''
```
<a name="OU9Cr"></a>
## 5、字典差集
字典求差有两种方式，使用的方法是一样的：
```python
# 初始字典
my_dict1 = {'1': 11, '8': 88, '6': 66, '7': 77, '5': 55, '2': 22, '9': 99}
my_dict2 = {'1': 11, '88': 888, '6': 66, '7': 77, '5': 55, '22': 222, '9': 99}

# 差集(字典1有字典2无)
diff_dict = dict([(k,v) for k,v in my_dict1.items() if k not in my_dict2])
print('1-2', diff_dict)

# 差集(字典2有字典1无)
diff_dict = dict([(k,v) for k,v in my_dict2.items() if k not in my_dict1])
print('2-1', diff_dict)


'''
1-2 {'8': 88, '2': 22}
2-1 {'88': 888, '22': 222}
'''
```
<a name="hfjLf"></a>
## 6、获取最大键
通过 `keys` 拿到所有键，获取最大键，返回 (最大键,值) 的元组
```python
# 求最大key函数
def max_keys(dict):
    if len(dict) == 0:
        return []
    max_key = max(dict.keys())
    return max_key, dict[max_key]

# 初始字典
my_dict = {'1': 11, '8': 18, '6': 16, '7': 17, '5': 15, '2': 12, '9': 19}
print(max_keys(my_dict))

'''
('9', 19)
'''
```
<a name="kfSnq"></a>
## 7、获取最大键值
通过 keys 拿到所有键，获取最大键，返回 (最大键,值) 的元组
```python
# 求最大values函数
def max_values(dict):
    if len(dict) == 0:
        return []
    max_val = max(dict.values())
    # 最大值可能有多个相同的数值
    return [(key, max_val) for key in dict if dict[key]==max_val]

# 初始字典
my_dict = {'1': 20, '8': 18, '6': 16, '7': 17, '5': 15, '2': 12, '9': 19, '3':20, '4':20}
print(max_values(my_dict))


'''
[('1', 20), ('3', 20), ('4', 20)]
'''
```
<a name="MzJap"></a>
## 8、求极值
找出集合中的最大、最小值
```python
# 求最大最小值函数
def max_min(set):
    return max(set), min(set)

# 初始集合
my_set = {456, -81, 75, 64, 15, 55, 141, 155, 100, 512}
print(max_min(my_set))


'''
(512, -81)
'''
```
