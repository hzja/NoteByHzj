Python
<a name="UUjbC"></a>
## 1、交换变量
```python
>>>a=3

>>>b=6
```
这个情况如果要交换变量在c++中，肯定需要一个空变量。但是Python不需要，只需一行，大家看清楚了
```python
>>>a,b=b,a

>>>print(a)>>>6

>>>ptint(b)>>>5
```
<a name="THppW"></a>
## 2、字典推导(Dictionary comprehensions)和集合推导(Set comprehensions)
大多数的Python程序员都知道且使用过列表推导(list comprehensions)。如果对list comprehensions概念不是很熟悉——一个list comprehension就是一个更简短、简洁的创建一个list的方法。
```python
>>> some_list = [1, 2, 3, 4, 5]

>>> another_list = [ x + 1 for x in some_list ]

>>> another_list
[2, 3, 4, 5, 6]
```
自从Python 3.1 起，可以用同样的语法来创建集合和字典表：
```python
>>> # Set Comprehensions
>>> some_list = [1, 2, 3, 4, 5, 2, 5, 1, 4, 8]

>>> even_set = { x for x in some_list if x % 2 == 0 }

>>> even_set
set([8, 2, 4])

>>> # Dict Comprehensions

>>> d = { x: x % 2 == 0 for x in range(1, 11) }

>>> d
{1: False, 2: True, 3: False, 4: True, 5: False, 6: True, 7: False, 8: True, 9: False, 10: True}
```
在第一个例子里，以some_list为基础，创建了一个具有不重复元素的集合，而且集合里只包含偶数。而在字典表的例子里，创建了一个key是不重复的1到10之间的整数，value是布尔型，用来指示key是否是偶数。<br />这里另外一个值得注意的事情是集合的字面量表示法。可以简单的用这种方法创建一个集合：
```python
>>> my_set = {1, 2, 1, 2, 3, 4}

>>> my_set
set([1, 2, 3, 4])
```
而不需要使用内置函数`set()`。
<a name="QWAO1"></a>
## 3、计数时使用`Counter`计数对象。
这听起来显而易见，但经常被人忘记。对于大多数程序员来说，数一个东西是一项很常见的任务，而且在大多数情况下并不是很有挑战性的事情——这里有几种方法能更简单的完成这种任务。<br />Python的`collections`类库里有个内置的`dict`类的子类，是专门来干这种事情的：
```python
>>> from collections import Counter
>>> c = Counter( hello world )

>>> c
Counter({ l : 3,  o : 2,    : 1,  e : 1,  d : 1,  h : 1,  r : 1,  w : 1})

>>> c.most_common(2)
[( l , 3), ( o , 2)]
```
<a name="TkXFc"></a>
## 4、漂亮的打印出JSON
JSON是一种非常好的数据序列化的形式，被如今的各种API和web service大量的使用。使用Python内置的json处理，可以使JSON串具有一定的可读性，但当遇到大型数据时，它表现成一个很长的、连续的一行时，人的肉眼就很难观看了。<br />为了能让JSON数据表现的更友好，可以使用`indent`参数来输出漂亮的JSON。当在控制台交互式编程或做日志时，这尤其有用：
```python
>>> import json

>>> print(json.dumps(data))  # No indention
{"status": "OK", "count": 2, "results": [{"age": 27, "name": "Oz", "lactose_intolerant": true}, {"age": 29, "name": "Joe", "lactose_intolerant": false}]}

>>> print(json.dumps(data, indent=2))  # With indention

{
  "status": "OK",
  "count": 2,
  "results": [

    {
      "age": 27,
      "name": "Oz",

      "lactose_intolerant": true
    },
    {
      "age": 29,

      "name": "Joe",
      "lactose_intolerant": false
    }
  ]

}
```
同样，使用内置的`pprint`模块，也可以让其它任何东西打印输出的更漂亮。
<a name="cTbID"></a>
## 5、解决FizzBuzz
写一个程序，打印数字1到100，3的倍数打印“Fizz”来替换这个数，5的倍数打印“Buzz”，对于既是3的倍数又是5的倍数的数字打印“FizzBuzz”。<br />这里就是一个简短的，有意思的方法解决这个问题：
```python
for x in range(1,101):
    print"fizz"[x%3*len( fizz )::]+"buzz"[x%5*len( buzz )::] or x
```
<a name="FDqzk"></a>
## 6、if 语句在行内
```python
print "Hello" if True else "World"
>>> Hello
```
<a name="rMjhT"></a>
## 7、连接
下面的最后一种方式在绑定两个不同类型的对象时显得很cool。
```python
nfc = ["Packers", "49ers"]
afc = ["Ravens", "Patriots"]
print nfc + afc
>>> [ Packers ,  49ers ,  Ravens ,  Patriots ]

print str(1) + " world"
>>> 1 world

print `1` + " world"
>>> 1 world

print 1, "world"
>>> 1 world
print nfc, 1
>>> [ Packers ,  49ers ] 1
```
<a name="yLMam"></a>
## 8、数值比较
这是见过诸多语言中很少有的如此棒的简便法
```python
x = 2
if 3 > x > 1:
   print x
>>> 2
if 1 < x > 0:
   print x
>>> 2
```
<a name="qWlZC"></a>
## 9、同时迭代两个列表
```python
nfc = ["Packers", "49ers"]
afc = ["Ravens", "Patriots"]
for teama, teamb in zip(nfc, afc):
     print teama + " vs. " + teamb
>>> Packers vs. Ravens
>>> 49ers vs. Patriots
```
<a name="Ah8uZ"></a>
## 10、带索引的列表迭代
```python
teams = ["Packers", "49ers", "Ravens", "Patriots"]
for index, team in enumerate(teams):
    print index, team
>>> 0 Packers
>>> 1 49ers
>>> 2 Ravens
>>> 3 Patriots
```
<a name="KLXMU"></a>
## 11、列表推导式
已知一个列表，可以刷选出偶数列表方法：
```python
numbers = [1,2,3,4,5,6]
even = []
for number in numbers:
    if number%2 == 0:
        even.append(number)
```
转变成如下：
```python
numbers = [1,2,3,4,5,6]
even = [number for number in numbers if number%2 == 0]
```
<a name="yqaRx"></a>
## 12、字典推导
和列表推导类似，字典可以做同样的工作：
```python
teams = ["Packers", "49ers", "Ravens", "Patriots"]
print {key: value for value, key in enumerate(teams)}
>>> { 49ers : 1,  Ravens : 2,  Patriots : 3,  Packers : 0}
```
<a name="ry6wh"></a>
## 13、初始化列表的值
```python
items = [0]*3
print items
>>> [0,0,0]
```
<a name="FGEY8"></a>
## 14、列表转换为字符串
```python
teams = ["Packers", "49ers", "Ravens", "Patriots"]
print ", ".join(teams)
>>>  Packers, 49ers, Ravens, Patriots
```
<a name="koqrq"></a>
## 15、从字典中获取元素
try/except代码并不雅致，不过这里有一种简单方法，尝试在字典中找key，如果没有找到对应的alue将用第二个参数设为其变量值。
```python
data = { user : 1,  name :  Max ,  three : 4}
try:
   is_admin = data[ admin ]
except KeyError:
   is_admin = False
```
替换成这样
```python
data = { user : 1,  name :  Max ,  three : 4}
is_admin = data.get( admin , False)
```
<a name="FS1KG"></a>
## 16、获取列表的子集
有时，只需要列表中的部分元素，这里是一些获取列表子集的方法。
```python
x = [1,2,3,4,5,6]
#前3个
print x[:3]
>>> [1,2,3]
#中间4个
print x[1:5]
>>> [2,3,4,5]
#最后3个
print x[3:]
>>> [4,5,6]
#奇数项
print x[::2]
>>> [1,3,5]
#偶数项
print x[1::2]
>>> [2,4,6]
```
除了Python内置的数据类型外，在`collection`模块同样还包括一些特别的用例，在有些场合`Counter`非常实用。
```python
from collections import Counter
print Counter("hello")
>>> Counter({ l : 2,  h : 1,  e : 1,  o : 1})
```
<a name="mBCrz"></a>
## 17、迭代工具
和`collections`库一样，还有一个库叫`itertools`，对某些问题真能高效地解决。其中一个用例是查找所有组合，他能告诉你在一个组中元素的所有不能的组合方式
```python
from itertools import combinations
teams = ["Packers", "49ers", "Ravens", "Patriots"]
for game in combinations(teams, 2):
    print game
>>> ( Packers ,  49ers )
>>> ( Packers ,  Ravens )
>>> ( Packers ,  Patriots )
>>> ( 49ers ,  Ravens )
>>> ( 49ers ,  Patriots )
>>> ( Ravens ,  Patriots )
```
<a name="OLMFk"></a>
## 18、`False == True`
比起实用技术来说这是一个很有趣的事，在Python中，True和False是全局变量，因此：
```python
False = True
if False:
   print "Hello"
else:
   print "World"
>>> Hello
```
