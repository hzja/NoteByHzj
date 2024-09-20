Python
<a name="dnwbF"></a>
### 单行循环列表
```python
mylist = [11,24,56,45,60]
print([i*3 for i in mylist]) #[33, 72, 168, 135, 180]
print([i-10 for i in mylist])
[33, 72, 168, 135, 180] #[1, 14, 46, 35, 50]
```
这种写法可以减少代码行数，而且比较清晰。
<a name="rhVEq"></a>
### 获取数据类型
```python
mylist = [123, 'python', 34.23, False, [4,5]]
print([type(i) for i in mylist])
# [<class 'int'>, <class 'str'>, <class 'float'>, <class 'bool'>, <class 'list'>]
```
有时候需要知道数据的类型，做对应的转换或者操作。可以这样判断：
```python
print(type(123) == int) # True
```
<a name="UVJ7u"></a>
### 获取数字的整数值
```python
integer = 8938485
digitz = [int(i) for i in str(integer)]
print(digitz)
# [8, 9, 3, 8, 4, 8, 5]
```
这里的原理就是先将数字转换为字符串，然后再进行循环获取。
<a name="UQWeS"></a>
### 链式比较
```python
a =5 
print(1 == a < 2) # False
print(2 < 4 < 5 == a)  # True
```
看到第一个，是不是会这样理解：1 == a 为 False，False < 2 为 False，所以最后答案为 False。<br />答案是对的，但是过程是错的。在 Python 中，比较逻辑是这样的：分别比较 1 == a 和 a < 2，两个都为 True，结果才为 True。<br />按照这个逻辑，自己领会一下第二个的运行逻辑。
<a name="WBQA5"></a>
### 字符串乘法器
如果需要重复生成字符串，通常会这样：
```python
str = ''
for i in range(10):
    str += 'Python'
print(str)
# PythonPythonPythonPythonPythonPythonPythonPythonPythonPython
```
其实，优雅的方式只需一行：
```python
print('Python'*10)
# PythonPythonPythonPythonPythonPythonPythonPythonPythonPython
```
<a name="Stz0U"></a>
### 带分隔符打印
```python
print('123', '234', 100, sep='x')
# 123x234x100
```
还有另外一种方式：
```python
print('x'.join(['123', '234', '100']))
# # 123x234x100
```
<a name="ozAJo"></a>
### 变量交换
```python
d1 = 34
d2 = 23
d1, d2 = d2, d1
print(d1, d2)
# 23 34
```
<a name="HpUoc"></a>
### 字符串变小写
```python
mylist = ['ABCD', 'Python', 'I am Chinese']
print([data.lower() for data in mylist])
# ['abcd', 'python', 'i am chinese']
```
<a name="Ucwv8"></a>
### 统计列表中元素出现频率
```python
import collections
mylist = [10, 11, 12, 3, 3, 11, 11, 33]
print(dict(collections.Counter(mylist)))
# {10: 1, 11: 3, 12: 1, 3: 2, 33: 1}
```
<a name="CIOFR"></a>
### 更新字典
```python
mydict = {1: 'Python', 2: 'Javascript', 3: 'Java', 4: 'C#'}
mydict.update({2: 'C'})
print(mydict)
# {1: 'Python', 2: 'C', 3: 'Java', 4: 'C#'}
mydict.update({5: 'Svelte'})
print(mydict)
# {1: 'Python', 2: 'C', 3: 'Java', 4: 'C#', 5: 'Svelte'}
```
<a name="xBB3Y"></a>
### 非 Pandas 方式读取 csv 文件
```python
import csv 
with open('Test.csv', 'r') as f:
    read = csv.reader(f)
    print([r for r in read])
```
