Python<br />如何将一个嵌套的大列表展开形成一个列表，这里提供了7种方法供大家学习参考：

- for循环
- 列表推导式
- 使用第三方库itertools
- 使用sum函数
- python自加（+=）
- 使用extend函数
- 使用递归函数
<a name="to4Ok"></a>
## 模拟数据
```python
data = [[1,2,3],[4],[5,6,7],[8,9],[10]]  # 模拟数据

data
```
```python
[[1, 2, 3], [4], [5, 6, 7], [8, 9], [10]]
```
这份模拟的数据有2个特点：

- 嵌套列表只有两层
- 里面的元素也全部是列表类型
<a name="cHOp8"></a>
## 方式1：for循环
最容易想到的方式就是for循环：
```python
for i in data:
    print(type(i))
```
```
<class 'list'>
<class 'list'>
<class 'list'>
<class 'list'>
<class 'list'>
```
```python
# 导入Iterable 模块
from collections import Iterable 

sum_data = []

for i in data:
    if isinstance(i,Iterable):  # 如果可迭代（比如列表形式）
        for j in i:  # 再次循环追加元素
            sum_data.append(j)  
    else:
        sum_data.append(i)  # 否则直接追加
        
sum_data
```
```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="O5C62"></a>
## 方式2：列表推导式
for循环能够实现，那么列表推导式肯定也可以：
```python
sum_data = [i for j in data  if isinstance(j,Iterable) for i in j]  # 循环一定是从大到小

sum_data
```
```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="H1Jd1"></a>
## 方式3：使用itertools库
借助第三方的库itertools：
```python
import itertools

# 通过chain方法从可迭代对象中生成；最后展开成列表

sum_data = list(itertools.chain.from_iterable(data))
sum_data
```
```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="jtTMR"></a>
## 方式4：使用sum函数
```python
sum_data = sum(data, [])
sum_data
```
```python
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="pxThs"></a>
## 方式5：Python自加
```python
sum_data = []

for i in data:
    sum_data += i  # 实现自加
    
sum_data
```
```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="B6d4f"></a>
## 方式6：`extend`函数
如何快速理解python的`extend`函数，给个案例
```python
# 如何理解python的extend函数

list1 = [1,2,3,4]
list1.extend([5,6])  # 追加功能extend；就地修改

list1  # list1已经发生了变化
```
```
[1, 2, 3, 4, 5, 6]
```
```python
sum_data = [] 

for i in data:
    sum_data.extend(i)   # 对空列表逐步追加
    
sum_data
```
```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="O6uRP"></a>
## 方式7：使用递归函数
```
data
```
```
[[1, 2, 3], [4], [5, 6, 7], [8, 9], [10]]
```
```python
def flatten(data):  # 定义递归函数
    sum_data = []
    for i in data:
        if isinstance(i, Iterable):  # 如果i是可迭代的对象（列表等），调用函数本身；直到执行else语句
            sum_data.extend(flatten(i))
        else:
            sum_data.append(i)
    
    return sum_data
```
```python
sum_data = flatten(data)  # 调用递归函数
sum_data
```
```
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="CgKYb"></a>
## 后续
这里的数据有特殊点：嵌套只有两层，且里面的元素全部是列表；后面会考虑非全部列表的数据（比如列表和数字组合的嵌套列表等）和多层嵌套。
