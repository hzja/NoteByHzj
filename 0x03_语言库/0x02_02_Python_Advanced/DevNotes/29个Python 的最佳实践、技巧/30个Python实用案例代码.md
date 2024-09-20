Python
<a name="dUtnE"></a>
## 1、转置矩阵
```python
old_list = [[1, 2, 3], [3, 4, 6], [5, 6, 7]]
list(list(x) for x in zip(*old_list))

# [[1, 3, 5], [2, 4, 6], [3, 6, 7]]
```
<a name="JRn3f"></a>
## 2、二进制转十进制
```python
decimal = int('1010', 2)
print(decimal) #10

# 10
```
<a name="A0fnK"></a>
## 3、字符串大写转小写
```python
# 方法一 lower()
"Hi my name is Allwin".lower()
# 'hi my name is allwin'
# 方法二 casefold()
"Hi my name is Allwin".casefold()
# 'hi my name is allwin'
```
<a name="Qj5aB"></a>
## 4、字符串小写转大写
```python
"hi my name is Allwin".upper()
# 'HI MY NAME IS ALLWIN'
```
<a name="vntKA"></a>
## 5、将字符串转换为字节
```python
"convert string to bytes using encode method".encode()
# b'convert string to bytes using encode method'
```
<a name="jENDn"></a>
## 6、复制文件内容
```python
import shutil; shutil.copyfile('source.txt', 'dest.txt')

# 'dest.txt'
```
<a name="rUuud"></a>
## 7、快速排序
```python
qsort = lambda l : l if len(l)<=1 else qsort([x for x in l[1:] if x < l[0]]) + [l[0]] + qsort([x for x in l[1:] if x >= l[0]])
qsort([1,3,2])

# [1, 2, 3]
```
<a name="RLwQA"></a>
## 8、n个连续数之和
```python
n = 3
sum(range(0, n+1))

# 6
```
<a name="biocS"></a>
## 9、交换两个变量
```python
a=1
b=2
a,b = b,a
```
<a name="oPbeE"></a>
## 10、斐波那契数列
```python
fib = lambda x: x if x<=1 else fib(x-1) + fib(x-2)
fib(10)

# 55
```
<a name="JHDcX"></a>
## 11、将嵌套列表合并为一个列表
```python
main_list = [[1,2],[3,4],[5,6,7]]
[item for sublist in main_list for item in sublist]

# [1, 2, 3, 4, 5, 6, 7]
```
<a name="KPFsY"></a>
## 12、运行 HTTP 服务器
```python
python3 -m http.server 8000
```
<a name="sE1zo"></a>
## 13、反转列表
```python
numbers = 'I Love China'
numbers[::-1]

# 'anihC evoL I'
```
<a name="O8z6w"></a>
## 14、返回阶乘
```python
import math; fact_5 = math.factorial(5)
fact_5

# 120
```
<a name="sWKyV"></a>
## 15、判断列表推导式
```python
even_list = [number for number in [1, 2, 3, 4] if number % 2 == 0]
even_list

# [2, 4]
```
<a name="AHDAm"></a>
## 16、取最长字符串
```python
words = ['This', 'is', 'a', 'list', 'of', 'words']
max(words, key=len) 

# 'words'
```
<a name="omhQ8"></a>
## 17、列表推导式
```python
li = [num for num in range(0,100)]
# this will create a list of numbers from 0 to 99
```
<a name="yMgYR"></a>
## 18、集合推导式
```python
num_set = { num for num in range(0,100)}
# this will create a set of numbers from 0 to 99
```
<a name="tXJj2"></a>
## 19、字典推导式
```python
dict_numbers = {x:x*x for x in range(1,5) }
# {1: 1, 2: 4, 3: 9, 4: 16}
```
<a name="vCZ2q"></a>
## 20、if-else
```python
print("even") if 4%2==0 else print("odd")

# even
```
<a name="HX6Jh"></a>
## 21、无限循环
```python
while 1:0
```
<a name="cCatf"></a>
## 22、检查数据类型
```python
isinstance(2, int)
isinstance("allwin", str)
isinstance([3,4,1997], list)
```
<a name="VDdcb"></a>
## 23、`while`循环
```python
a=5
while a > 0: a = a - 1; print(a)
```
<a name="JPRvB"></a>
## 24、使用`print`语句写入到文件里
```python
print("Hello, World!", file=open('source.txt', 'w'))
```
<a name="VRJ0I"></a>
## 25、统计字频
```python
print("umbrella".count('l'))

# 2
```
<a name="OWa3g"></a>
## 26、合并两个列表
```python
list1.extend(list2)
# contents of list 2 will be added to the list1
```
<a name="GEHtv"></a>
## 27、合并两个字典
```python
dict1.update(dict2)
# contents of dictionary 2 will be added to the dictionary 1
```
<a name="w02bD"></a>
## 28、合并两个集合
```python
set1.update(set2)
# contents of set2 will be copied to the set1
```
<a name="cTld1"></a>
## 29、时间戳
```python
import time; print(time.time())

# 1632146103.8406303
```
<a name="APMDp"></a>
## 30、统计最多的元素
```python
test_list = [9, 4, 5, 4, 4, 5, 9, 5, 4]
most_frequent_element = max(set(test_list), key=test_list.count)
most_frequent_element

# 4
```
