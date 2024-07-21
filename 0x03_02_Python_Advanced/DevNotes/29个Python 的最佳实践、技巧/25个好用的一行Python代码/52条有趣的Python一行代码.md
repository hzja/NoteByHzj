Python
<a name="Z1c68"></a>
## ▍1、字母异位词
两个单词如果包含相同的字母，次序不同，则称为字母易位词(anagram)。<br />例如，“silent”和“listen”是字母易位词，而“apple”和“aplee”不是易位词。
```python
from collections import Counter

s1 = 'below'
s2 = 'elbow'

print('anagram') if Counter(s1) == Counter(s2) else print('not an anagram')
```
使用一行Python代码，就能判断出来了。
<a name="o2yt9"></a>
## ▍2、二进制转十进制
```python
decimal = int('1010', 2)
print(decimal) #10
```
<a name="cc8rs"></a>
## ▍3、将字符串转换为小写
```python
print("Hi my name is XiaoF".lower())
# 'hi my name is xiaof'

print("Hi my name is XiaoF".casefold())
# 'hi my name is xiaof'
```
<a name="DC0wO"></a>
## ▍4、将字符串转换为大写
```python
print("hi my name is XiaoF".upper())
# 'HI MY NAME IS XIAOF'
```
<a name="slxh2"></a>
## ▍5、将字符串转换为字节
```python
print("convert string to bytes using encode method".encode())
# b'convert string to bytes using encode method'
```
<a name="UFKui"></a>
## ▍6、拷贝文件
```python
import shutil

shutil.copyfile('source.txt', 'dest.txt')
```
<a name="Kwnwc"></a>
## ▍7、快速排序
```python
qsort = lambda l: l if len(l) <= 1 else qsort([x for x in l[1:] if x < l[0]]) + [l[0]] + qsort([x for x in l[1:] if x >= l[0]])

print(qsort([17, 29, 11, 97, 103, 5]))
# [5, 11, 17, 29, 97, 103]
```
<a name="nRXlS"></a>
## ▍8、n个连续数的和
```python
n = 10

print(sum(range(0, n+1)))
# 55
```
<a name="wYV7r"></a>
## ▍9、交换两个变量的值
```python
a,b = b,a
```
<a name="m3WnP"></a>
## ▍10、斐波纳契数列
```python
fib = lambda x: x if x<=1 else fib(x-1) + fib(x-2)

print(fib(20))
# 6765
```
<a name="DoD4O"></a>
## ▍11、将嵌套列表合并为一个列表
```python
main_list = [[0, 1, 2], [11, 12, 13], [52, 53, 54]]

result = [item for sublist in main_list for item in sublist]
print(result)

>
[0, 1, 2, 11, 12, 13, 52, 53, 54]
```
<a name="fl1EY"></a>
## ▍12、运行一个HTTP服务器
```python
python3 -m http.server 8000
python2 -m SimpleHTTPServer
```
<a name="Aadz0"></a>
## ▍13、反转列表
```python
numbers = [0, 1, 2, 11, 12, 13, 52, 53, 54]

print(numbers[::-1])
# [54, 53, 52, 13, 12, 11, 2, 1, 0]
```
<a name="RgTz4"></a>
## ▍14、阶乘
```python
import math

fact_5 = math.factorial(5)
print(fact_5)
# 120
```
<a name="SP8pQ"></a>
## ▍15、在列表推导式中使用for和if
```python
even_list = [number for number in [1, 2, 3, 4] if number % 2 == 0]

print(even_list)
# [2, 4]
```
<a name="Zj8lc"></a>
## ▍16、列表中最长的字符串
```python
words = ['This', 'is', 'a', 'list', 'of', 'words']

result = max(words, key=len)
print(result)
# 'words'
```
<a name="svU8t"></a>
## ▍17、列表推导式
```python
li = [num for num in range(0, 10)]

print(li)
# [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```
<a name="Xunj2"></a>
## ▍18、集合推导式
```python
num_set = {num for num in range(0, 10)}

print(num_set)
# {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
```
<a name="u8Nxo"></a>
## ▍19、字典推导式
```python
dict_numbers = {x: x*x for x in range(1, 5)}

print(dict_numbers)
# {1: 1, 2: 4, 3: 9, 4: 16}
```
<a name="MbtMU"></a>
## ▍20、if-else
```python
print("even") if 4 % 2==0 else print("odd")
```
<a name="qkZFd"></a>
## ▍21、无限循环
```python
while 1:0
```
<a name="TrlqA"></a>
## ▍22、检查数据类型
```python
print(isinstance(2, int))
# True

print(isinstance("allwin", str))
# True

print(isinstance([3, 4, 1997], list))
# True
```
<a name="iapKx"></a>
## ▍23、While循环
```python
a = 5

while a > 0:
    a = a - 1

print(a)
# 0
```
<a name="WJZrB"></a>
## ▍24、使用`print`语句写入文件
```python
print("Hello, World!", file=open('file.txt', 'w'))
```
既可打印出信息，还能将信息保存文件。
<a name="yS7HH"></a>
## ▍25、计算一个字符在字符串中出现的频率
```python
print("umbrella".count('l'))
# 2
```
<a name="q20oG"></a>
## ▍26、合并列表
```python
list1 = [1, 2, 4]
list2 = ['XiaoF']
list1.extend(list2)

print(list1)
# [1, 2, 4, 'XiaoF']
```
<a name="Gtzqu"></a>
## ▍27、合并字典
```python
dict1 = {'name': 'weiwei', 'age': 23}
dict2 = {'city': 'Beijing'}
dict1.update(dict2)

print(dict1)
# {'name': 'weiwei', 'age': 23, 'city': 'Beijing'}

```
<a name="WBZuZ"></a>
## ▍28、合并集合
```python
set1 = {0, 1, 2}
set2 = {11, 12, 13}
set1.update(set2)

print(set1)
# {0, 1, 2, 11, 12, 13}
```
<a name="psoIz"></a>
## ▍29、时间戳
```python
import time

print(time.time())
```
<a name="KH64J"></a>
## ▍30、列表中出现次数最多的元素
```python
test_list = [9, 4, 5, 4, 4, 5, 9, 5, 4]
most_frequent_element = max(set(test_list), key=test_list.count)

print(most_frequent_element)
# 4
```
<a name="vLcir"></a>
## ▍31、嵌套列表
```python
numbers = [[num] for num in range(10)]

print(numbers)
# [[0], [1], [2], [3], [4], [5], [6], [7], [8], [9]]
```
<a name="JVk7O"></a>
## ▍32、八进制转十进制
```python
print(int('30', 8)) 
# 24
```
<a name="LR0XT"></a>
## ▍33、将键值对转换为字典
```python
result = dict(name='XiaoF', age=23)

print(result)
# {'name': 'XiaoF', 'age': 23}
```
<a name="bc6Zb"></a>
## ▍34、求商和余数
```python
quotient, remainder = divmod(4, 5)

print(quotient, remainder)
# 0 4
```
`divmod()`函数返回当参数1除以参数2时，包含商和余数的元组。
<a name="ZuxHR"></a>
## ▍35、删除列表中的重复项
```python
print(list(set([4, 4, 5, 5, 6])))
# [4, 5, 6]
```
<a name="lig8S"></a>
## ▍36、按升序排序列表
```python
print(sorted([5, 2, 9, 1]))
# [1, 2, 5, 9]
```
<a name="O9wcf"></a>
## ▍37、按降序排序列表
```python
print(sorted([5, 2, 9, 1], reverse=True))
# [9, 5, 2, 1]
```
<a name="SFpcq"></a>
## ▍38、获取小写字母表
```python
import string

print(string.ascii_lowercase)
# abcdefghijklmnopqrstuvwxyz
```
<a name="msrco"></a>
## ▍39、获取大写字母表
```python
import string

print(string.ascii_uppercase)
# ABCDEFGHIJKLMNOPQRSTUVWXYZ
```
<a name="U5v3r"></a>
## ▍40、获取0到9字符串
```python
import string

print(string.digits)
# 0123456789
```
<a name="NXl7S"></a>
## ▍41、十六进制转十进制
```python
print(int('da9', 16))
# 3497
```
<a name="vziyp"></a>
## ▍42、日期时间
```python
import time

print(time.ctime())
# Thu Aug 13 20:00:00 2021
```
<a name="hQnKX"></a>
## ▍43、将列表中的字符串转换为整数
```python
print(list(map(int, ['1', '2', '3'])))
# [1, 2, 3]
```
<a name="nrOES"></a>
## ▍44、用键对字典进行排序
```python
d = {'one': 1, 'four': 4, 'eight': 8}
result = {key: d[key] for key in sorted(d.keys())}

print(result)
# {'eight': 8, 'four': 4, 'one': 1}
```
<a name="LIyNr"></a>
## ▍45、用键值对字典进行排序
```python
x = {1: 2, 3: 4, 4: 3, 2: 1, 0: 0}
result = {k: v for k, v in sorted(x.items(), key=lambda item: item[1])}

print(result)
# {0: 0, 2: 1, 1: 2, 4: 3, 3: 4}
```
<a name="RVkEK"></a>
## ▍46、列表旋转
```python
li = [1, 2, 3, 4, 5]

# li[n:] + li[:n], 右变左
print(li[2:] + li[:2])
# [3, 4, 5, 1, 2]

# li[-n:] + li[:-n], 左变右
print(li[-1:] + li[:-1])
# [5, 1, 2, 3, 4]
```
<a name="c0PjG"></a>
## ▍47、将字符串中的数字移除
```python
message = ''.join(list(filter(lambda x: x.isalpha(), 'abc123def4fg56vcg2')))

print(message)
# abcdeffgvcg
```
<a name="GOaJ8"></a>
## ▍48、矩阵变换
```python
old_list = [[1, 2, 3], [3, 4, 6], [5, 6, 7]]
result = list(list(x) for x in zip(*old_list))

print(result)
# [[1, 3, 5], [2, 4, 6], [3, 6, 7]]
```
<a name="srZc9"></a>
## ▍49、列表过滤
```python
result = list(filter(lambda x: x % 2 == 0, [1, 2, 3, 4, 5, 6]))

print(result)
# [2, 4, 6]
```
<a name="lJYKz"></a>
## ▍50、解包
```python
a, *b, c = [1, 2, 3, 4, 5]

print(a) # 1
print(b) # [2, 3, 4]
print(c) # 5
```
<a name="Tjcxh"></a>
## ▍51、打印九九乘法表
```python
print '\n'.join([' '.join(['%s*%s=%-2s' % (y,x,x*y) for y in range(1,x+1)]) 
for x in range(1,10)])
```
<a name="aG0Lf"></a>
## ▍52、打印一个心星
```python
print'\n'.join([''.join([('HiPython'[(x-y)%len('HiPython')]
if((x*0.05)**2+(y*0.1)**2-1)**3-(x*0.05)**2*(y*0.1)**3<=0 else' ')
for x in range(-32,32)])for y in range(17,-17,-1)]) >>
```
