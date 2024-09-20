Python random
<a name="Y7kf5"></a>
## random 模块
Python 提供的 random 模块实现了各种分布式的伪随机数生成器。该模块主要包含以下几种常用的函数。
<a name="eCa76"></a>
### `random()`函数
`random()`函数可以随机生成一个`[0,1)`的浮点数。例如：
```python
import random
print(random.random())

0.7258069853437111
```
<a name="LrYA5"></a>
### `randint(a,b)`函数
`randint(a,b)`函数可以在指定范围内随机生成一个整数，其中参数a是下限，参数 b 是上限，生成的随机数n：a <= n <= b。例如：
```python
import random  
print(random.randint(5,50))

37
```
<a name="OU4VX"></a>
### `uniform(a,b)`函数
`uniform(a,b)`函数可以在指定范围内随机生成一个浮点数，两个参数其中一个是上限，一个是下限。如果a > b，则生成的随机数n: b <= n <= a。如果 a <b， 则 a <= n <= b。
```python
import random
print(random.uniform(2,5))
print(random.uniform(5,2))

4.400555757352024
3.4262178434528874
```
<a name="RF96r"></a>
### `sample(sequence, k)`函数
`sample(sequence, k)`函数可以获取从总体序列或集合中选择的唯一元素的k长度列表。`sample()`函数不会修改原有序列，它主要用在无重复的随机抽样场景，实现从大量样本中快速进行抽样。例如：
```python
import random   
lst = [1,2,3,4,5]  
print(random.sample(lst,4))  
print(lst)

[5, 4, 3, 1]
[1, 2, 3, 4, 5]
```
<a name="fnN2C"></a>
### `randrange([start], stop[, step])`函数
`randrange()`函数主要用于返回一个随机数，它有三个参数，其中第三个参数`step`是可选参数。具体区别如下：

1. `randrange([start], stop)`可以在指定范围内随机生成一个整数，生成的随机数n：a <= n <= b。其中，参数start和参数stop是有要求的，必须小数在前大数在后，否则会报错。例如：
```python
import random
print(random.randrange(2,5))
print(random.randrange(5,2))

3
empty range for randrange()
```

1. `randrange([start], stop[, step])`函数可以先从1到10中产生一个间隔是2的等差数列[1,3,5,7,9]，再从中随机获取一个随机数。例如：
```python
import random
print(random.randrange(1,10,2))
9
```
<a name="rMJtH"></a>
### `choice(sequence)`函数
`choice(sequence)`函数可以从非空序列 sequence 中随机返回一个数，参数 sequence 表示一个有序类型，可以包含 list、tuple 等。例如：
```python
import random  
strlist = ['C++','C#','Java','Python']  
strtemp = ('Do you love python')  
print(random.choice(strlist))
print(random.choice(strtemp))

Python
v
```
<a name="quPFD"></a>
### `shuffle(x[, random])`函数
`shuffle(x[, random])`函数可以将一个有序列表中的元素打乱，再重新排序。例如：
```python
import random
lst = ['A' , 'B', 'C', 'D', 'E' ]
random.shuffle(lst)  
print (lst)

['C', 'B', 'A', 'E', 'D']
```
