Python
<a name="GUU5u"></a>
### 1. 两个字典的合并
```python
x = {'a': 1, 'b': 2}
y = {'c': 3, 'd': 4}
```
将两个字典合并起来，代码如下
```python
x.update(y)
print(x)
```
output
```python
{'a': 1, 'b': 2, 'c': 3, 'd': 4}
```
<a name="ff9Mf"></a>
### 2. 两个列表的合并
```python
x = ['a', 'b']
y = ['c', 'd', 'e']
```
将上面两个列表合并起来，代码如下
```python
x.extend(y)
print(x)
```
output
```python
['a', 'b', 'c', 'd', 'e']
```
当然除此之外，还有其他的方法来将两个列表合并起来，例如
```python
x += y
print(x)
```
output
```python
['a', 'b', 'c', 'd', 'e']
```
<a name="dK8IO"></a>
### 3. 计算列表中元素出现的频率
主要是通过调用python当中的`collections`模块来实现，例如
```python
from collections import Counter
my_list = ['a', 'b', 'b', 'a', 'a', 'a', 'c', 'c', 'b', 'd']
print(Counter(my_list).most_common())
```
output
```python
[('a', 4), ('b', 3), ('c', 2), ('d', 1)]
```
若是想要出现频率最多的一个，也就是在上面代码的基础之上添加筛选第一个元素的操作即可
```python
print(Counter(my_list).most_common()[0])
```
output
```python
('a', 4)
```
出现频率最多的是元素a，总共出现了4次<br />当然要是在后面再添加一个`[0]`，意思就是筛选出出现频率最多的元素
```python
print(Counter(my_list).most_common()[0][0])
```
output
```python
a
```
<a name="A7Il6"></a>
### 4. 计算获得除法中的商和余数
一般若想取得除法当中的商和余数，一般是Python运算符号当中的`//`和`/`，而`divmod`方法则可以同时获得除法运算当中的商和余数，代码如下
```python
quotient, remainder = divmod(37, 5)
print(quotient, remainder)
```
output
```python
7 2
```
<a name="ESKEU"></a>
### 5. 计算得到列表当中长度最长的字符串
```python
words = ['Python', 'is', 'awesome']
print(max(words, key=len))
```
output
```python
awesome
```
<a name="sa7l7"></a>
### 6. 将列表中的顺序倒转
```python
words = ['Python', 'is', 'awesome']
reverse_words = words[::-1]
print(reverse_words)
```
output
```python
['awesome', 'is', 'Python']
```
<a name="EjSgq"></a>
### 7. 文件的读与写
先将数据写入到文件当中
```python
data = 'Python is awesome'
with open('file.txt', 'a', newline='\n') as f: f.write(data)
```
从刚生成的文件当中读取刚写入的数据，代码就是这么来写
```python
data = [line.strip() for line in open("file.txt")]
print(data)
```
output
```python
['Python is awesome']
```
<a name="Whk49"></a>
### 8. 将字典当中的键值对位置调换
```python
staff = {'Data Scientist': 'Mike', 'Django Developer': 'Dylan'}
staff = {i:j for j, i in staff.items()}
print(staff)
```
output
```python
{'Mike': 'Data Scientist', 'Dylan': 'Django Developer'}
```
<a name="KmZLc"></a>
### 9. 将嵌套列表合并为一个列表
假设有这样的一个列表
```python
l = [[1, 2, 3], [4, 5], [6], [7, 8], [9]]
```
而最终希望列表能够是
```python
[1, 2, 3, 4, 5, 6, 7, 8, 9]
```
可以这么来做
```python
flattened_list = [item for sublist in l for item in sublist
print(flattened_list)
```
output
```python
[1, 2, 3, 4, 5, 6, 7, 8, 9]
```
<a name="eNmeV"></a>
### 10. 列表当中数据类型的转换
例如有下面的列表
```python
['1', '2', '3']
```
要将其转换成整数类型，代码如下
```python
print(list(map(int, ['1', '2', '3'])))
```
output
```python
[1, 2, 3]
```
当也可以将可以尝试转换成浮点数的类型，代码如下
```python
print(list(map(float, ['1', 2, '3.0', 4.0, '5', 6])))
```
output
```python
[1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
```
<a name="k35yV"></a>
### 11. 将列表转化成字典
```python
cars = ['Audi', 'BMW', 'Ford', 'Tesla', 'Volvo']
cars_dict = dict(enumerate(cars))
print(cars_dict)
```
output
```python
{0: 'Audi', 1: 'BMW', 2: 'Ford', 3: 'Tesla', 4: 'Volvo'}
```
<a name="l9IAh"></a>
### 12. 将列表当中的重复元素去除
```python
list(set(['a', 'a', 'b', 'a', 'c']))
```
output
```python
['a', 'b', 'c']
```
<a name="prtUE"></a>
### 13. 从列表中筛选出特定元素
```python
cars = ['Audi', 'BMW', 'Ford', 'Tesla', 'Volvo']
car_1 = [car for car in cars if car[0] == "A"]
print(car_1)
```
output
```python
['Audi']
```
当然还可以通过调用Python当中的`filter`方法来实现，代码如下
```python
car_1 = list(filter(lambda c: c[0] == 'A', cars))
```
得到的结果也和上述的一样
<a name="Xqhez"></a>
### 14. 列表中的元素排序
```python
numbers = [55, -30, 28, -36, 48, 20]
numbers.sort()
print(numbers)
```
output
```python
[-36, -30, 20, 28, 48, 55]
```
当然也可以从大到小，这样的方式来排序，代码如下
```python
numbers.sort(reverse=True)
numbers
```
output
```python
[55, 48, 28, 20, -30, -36]
```
而对于字符串而言，可以根据首字母的字母表顺序来排序
```python
cars = ['Ford', 'Tesla', 'BMW', 'Volvo', 'Audi']
cars.sort()
print(cars)
```
output
```python
['Audi', 'BMW', 'Ford', 'Tesla', 'Volvo']
```
<a name="taaoX"></a>
### 15. 合并集合
```python
set1 = {"1", "2", "5"}
set2 = {"4", "6", "7"}

set1.update(set2)
print(set1)
```
output
```python
{'7', '6', '5', '2', '1', '4'}
```
<a name="pw41S"></a>
### 16. 根据键来对字典进行排序
```python
d = {'one': 1, 'three': 4, 'five': 8, 'six': 10}
result = {key: d[key] for key in sorted(d.keys())}
print(result)
```
output
```python
{'five': 8, 'one': 1, 'six': 10, 'three': 4}
```
<a name="TqjN1"></a>
### 17. 根据键值来对字典进行排序
```python
d = {'one': 15, 'three': 12, 'five': 8, 'six': 30}
result = {key: value for key, value in sorted(d.items(), key=lambda item: item[1])}
print(result)
```
output
```python
{'five': 8, 'three': 12, 'one': 15, 'six': 30}
```
<a name="aGMWl"></a>
### 18. 替换字符串
```python
"Python is a programming language. Python is awesome".replace("Python",'Java')
```
output
```python
Java is a programming language. Java is awesome
```
<a name="NlA4L"></a>
### 19. 计算指定字符串出现的次数
```python
a = 'python is a programming language. python is python.'
result = a.count('python')
print(result)
```
output
```python
3
```
<a name="zsqFO"></a>
### 20. 将自定义方法作用在列表中的每个元素
```python
from functools import reduce

reduce(lambda x, y: x*y, [2, 2, 3, 4])
```
output
```python
48
```
<a name="z7f5L"></a>
### 21. 找到最大的那个数
```python
find_max = lambda x,y: x if x > y else y
result = find_max(5,20)
```
output
```python
20
```
<a name="w2vlQ"></a>
### 22. 将矩阵转置
```python
a = [[1, 2, 3],
     [4, 5, 6],
     [7, 8, 9]]

transposed = [list(i) for i in zip(*a)]
print(transposed)
```
output
```python
[[1, 4, 7], [2, 5, 8], [3, 6, 9]]
```
<a name="secKs"></a>
### 23. 生成斐波纳契数列
斐波纳契数列指的是列表当中元素的值是由前两个元素的值的总和而来的，例如像是1, 1, 2, 3, 5, 8,13，要生成它的代码如下
```python
fibo = [0, 1]
[fibo.append(fibo[-2]+fibo[-1]) for i in range(10)]
print(fibo)
```
output
```python
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89]
```
<a name="QWv75"></a>
### 24. 删除列表中的多个元素
```python
mylist = [100, 200, 300, 400, 500]
del mylist[:3]
print(mylist)
```
output
```python
[400, 500]
```
<a name="gr65d"></a>
### 25. 多个if-else组合
目标是将下面多个if-else的组合，写在一行上面
```python
x = 200

if x < 20:
    print("小于20")
elif x == 200:
    print("等于200")
else:
    print("大于20且不等于200")
```
也可以将多个if-else组合放在一行上面写
```python
x = 200
print("小于20") if x < 20 else print("等于200") if x == 200 else print("大于20且不等于200")
```
<a name="AywwE"></a>
### 26. 进制转换
平时的编码过程中，进制转换是非常常用的一个功能，尤其是涉及到一些算法的时候更是频繁。事实上 Python 已经内置了各个进制转换的 Api，咱们直接调用即可。
```python
In [1]: int('1100', 2)
Out[1]: 12

In [2]: int('30', 8)
Out[2]: 24

In [3]: int('ac9', 16)
Out[3]: 2761
```
<a name="QnSYz"></a>
### 27. 斐波纳契数列
斐波纳契数列是一个很经典的数列，其通项公式为第一项和第二项都为 1，从第三项开始，每一项都等于前两项之和。
```python
In [4]: fibonacci = lambda x: x if x <= 1 else fibonacci(x - 1) + fibonacci(x - 2)

In [5]: fibonacci(15)
Out[5]: 610
```
<a name="EM5w0"></a>
### 28. 快速排序
快速排序是初级工程师常考的一个算法题，整个算法写下来的话基本都需要八九行，来看看 Python 是如何一行代码搞定快速排序的。
```python
In [6]: quick_sort = lambda l: l if len(l) <= 1 else quick_sort([x for x in l[1:] if x < l[0]]) + [l[0]] + quick_sort([x for x in l[1:] if x >= l[0]])

In [7]: quick_sort([18, 20, 12, 99, 200, 59, 66, 34, 22])
Out[7]: [12, 18, 20, 22, 34, 59, 66, 99, 200]
```
<a name="ZmMxR"></a>
### 29. 写入文件
文件操作也是常用的操作之一，见过用 print 函数来写入文件的么。
```python
print("Hello, Python!", file=open('file.txt', 'w'))
```
<a name="NMLvx"></a>
### 30. 字母异位词
顾名思义，字母异位词就是通过交换单词中字母的顺序，两个单词最终是一样的。
```python
In [9]: from collections import Counter

In [10]: s1, s2 = 'apple', 'orange'

In [11]: 'anagram' if Counter(s1) == Counter(s2) else 'not an anagram'
Out[11]: 'not an anagram'
```
<a name="uWJey"></a>
### 31. 矩阵转换
对于数据分析工作者，经常会接触到矩阵，那么就需要熟悉对矩阵的各种操作。而矩阵转换就是常规操作之一。
```python
In [12]: num_list = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

In [13]: result = list(list(x) for x in zip(*num_list))

In [14]: result
Out[14]: [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
```
<a name="qKPtJ"></a>
### 32. 字典数
虽然现在很多常用的算法都被封装成 Api 直接调用就好了，但并不意味着工作就不需要写算法了。在写算法的过程中会用到一些常见的字典数，比如大写字母、小写字母、数字等。而这些 Python 都考虑到了，直接调用即可。
```python
In [15]: import string

In [16]: string.ascii_lowercase
Out[16]: 'abcdefghijklmnopqrstuvwxyz'

In [17]: string.ascii_uppercase
Out[17]: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

In [18]: string.digits
Out[18]: '0123456789'
```
<a name="tCkak"></a>
### 33. 合并列表
在对接外部接口或者数据处理时，嵌套列表是非常常见的数据结构，但显然整合成一个列表更容易处理。
```python
In [19]: num_list = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

In [20]: result = [item for sublist in num_list for item in sublist]

In [21]: result
Out[21]: [1, 2, 3, 4, 5, 6, 7, 8, 9]
```
<a name="Pzp9Q"></a>
### 34. 推导式
推导式是 Python 的精华所在，极大的方便了创建列表和字典。
```python
In [22]: num_list = [num for num in range(0, 10)]

In [23]: num_list
Out[23]: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

In [24]: num_set = {num for num in range(0, 10)}

In [25]: num_set
Out[25]: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

In [26]: num_dict = {x: x * x for x in range(1, 5)}

In [27]: num_dict
Out[27]: {1: 1, 2: 4, 3: 9, 4: 16}
```
