Python
<a name="ybGkq"></a>
## 字典是否是无序的
关于这个概念，很多朋友不一定清楚。<br />在 Python 2.7 中，字典是无序结构。字典项目的顺序是混乱的。这意味着项目的顺序是确定性和可重复的。
```python
>>> # Python 2.7
>>> a_dict = {'color': 'blue', 'fruit': 'apple', 'pet': 'dog'}
>>> a_dict
{'color': 'blue', 'pet': 'dog', 'fruit': 'apple'}
>>> a_dict
{'color': 'blue', 'pet': 'dog', 'fruit': 'apple'}
```
在 Python 3.5 中，字典仍然是无序的，但这次是随机的数据结构。这意味着每次重新运行字典时，都会得到不同的项目顺序。
```python
>>> # Python 3.5
>>> a_dict = {'color': 'blue', 'fruit': 'apple', 'pet': 'dog'}
>>> a_dict
{'color': 'blue', 'pet': 'dog', 'fruit': 'apple'}
>>> a_dict
{'color': 'blue', 'pet': 'dog', 'fruit': 'apple'}
```
在 Python 3.6 及更高版本中，字典是有序的数据结构，这意味着它们保持元素的顺序与它们被引入时的顺序相同。
```python
>>> a_dict = {'color': 'blue', 'fruit': 'apple', 'pet': 'dog'}
>>> a_dict
{'color': 'blue', 'fruit': 'apple', 'pet': 'dog'}
>>> a_dict
{'color': 'blue', 'fruit': 'apple', 'pet': 'dog'}
```
<a name="bykT8"></a>
## 键值互换
假设有一本字典，由于某种原因需要将键转换为值，值转换为键，应该怎么做呢？
```python
>>> a_dict = {'one': 1, 'two': 2, 'three': 3, 'four': 4}
>>> new_dict = {}
>>> for key, value in a_dict.items():
...     new_dict[value] = key
...
>>> new_dict
{1: 'one', 2: 'two', 3: 'three', 4: 'four'}
# 另一种更快捷的实现方式
>>> new_dict = {v:k for k,v in a_dict.items()}
```
<a name="wDBdD"></a>
## 依据某种条件，过滤字典
有时候，需要根据某种条件来过滤字典。那么配合if条件语句，是一个很好的选择。
```python
>>> a_dict = {'one': 1, 'two': 2, 'thee': 3, 'four': 4}
>>> new_dict = {}  # Create a new empty dictionary
>>> for key, value in a_dict.items():
...     if value <= 2:
...         new_dict[key] = value
...
>>> new_dict
{'one': 1, 'two': 2}
```
<a name="soNeh"></a>
## 利用字典中的值，做一些计算
在Python中遍历字典时。需要进行一些计算也是很常见的。假设已将公司销售额的数据存储在字典中，现在想知道一年的总收入。
```python
>>> incomes = {'apple': 5600.00, 'orange': 3500.00, 'banana': 5000.00}
>>> total_income = 0.00
>>> for value in incomes.values():
...     total_income += value  # Accumulate the values in total_income
...
>>> total_income
14100.0
```
<a name="Fua13"></a>
## 字典推导式
字典推导式，是一个和列表推导式一样，具有很强大功能的知识点。因此，大家一定要掌握。<br />例如，假设有两个数据列表，需要根据它们创建一个新字典。
```python
>>> objects = ['blue', 'apple', 'dog']
>>> categories = ['color', 'fruit', 'pet']
>>> a_dict = {key: value for key, value in zip(categories, objects)}
>>> a_dict
{'color': 'blue', 'fruit': 'apple', 'pet': 'dog'}
```
<a name="eTuBq"></a>
## 利用字典推导式，实现键值转换
可以发现，使用字典推导式，是一个更简单、高效的操作。
```python
>>> a_dict = {'one': 1, 'two': 2, 'thee': 3, 'four': 4}
>>> new_dict = {value: key for key, value in a_dict.items()}
>>> new_dict
{1: 'one', 2: 'two', 3: 'thee', 4: 'four'}
```
<a name="QvCqb"></a>
## 利用字典推导式，过滤字典
```python
>>> a_dict = {'one': 1, 'two': 2, 'thee': 3, 'four': 4}
>>> new_dict = {k: v for k, v in a_dict.items() if v <= 2}
>>> new_dict
{'one': 1, 'two': 2}
```
<a name="FgJJx"></a>
## 利用字典推导式，做一些计算
```python
>>> incomes = {'apple': 5600.00, 'orange': 3500.00, 'banana': 5000.00}
>>> total_income = sum([value for value in incomes.values()])
>>> total_income
14100.0
```
<a name="KUH4i"></a>
## 字典排序
从 Python 3.6 开始，字典是有序的数据结构，因此如果使用 Python 3.6（及更高版本），能够通过使用`sorted()`并借助字典理解对任何字典的键，进行排序。
```python
>> incomes = {'apple': 5600.00, 'orange': 3500.00, 'banana': 5000.00}
>>> sorted_income = {k: incomes[k] for k in sorted(incomes)}
>>> sorted_income
{'apple': 5600.0, 'banana': 5000.0, 'orange': 3500.0}
```
<a name="f7gLq"></a>
## 内置函数，与字典配合使用
Python 提供了一些内置函数，这些函数在处理集合（如字典）时可能会很有用。
<a name="fSfD0"></a>
### `map()`函数
假设有一个包含一堆产品价格的字典，并且需要对它们应用折扣。
```python
>>> prices = {'apple': 0.40, 'orange': 0.35, 'banana': 0.25}
>>> def discount(current_price):
...     return (current_price[0], round(current_price[1] * 0.95, 2))
...
>>> new_prices = dict(map(discount, prices.items()))
>>> new_prices
{'apple': 0.38, 'orange': 0.33, 'banana': 0.24}
```
<a name="uIuxs"></a>
### `filter()`函数
假设要知道单价低于0.40的产品。
```python
>>> prices = {'apple': 0.40, 'orange': 0.35, 'banana': 0.25}
>>> def has_low_price(price):
...     return prices[price] < 0.4
...
>>> low_price = list(filter(has_low_price, prices.keys()))
>>> low_price
['orange', 'banana']
```
<a name="IsBKw"></a>
## 字典解包运算符
这是很多人不清楚的概念，Python 3.5 带来了一个有趣的新特性，因此大家需要着重学习。<br />可以使用字典解包运算符 (`**`) 将两个字典合并为一个新字典。
```python
>>> vegetable_prices = {'pepper': 0.20, 'onion': 0.55}
>>> fruit_prices = {'apple': 0.40, 'orange': 0.35, 'pepper': .25}
>>> {**vegetable_prices, **fruit_prices}
{'pepper': 0.25, 'onion': 0.55, 'apple': 0.4, 'orange': 0.35}
```
如果尝试合并的字典，具有重复或公共键，则最右侧字典的值将补充上。
