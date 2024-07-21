Python 
<a name="TYlQ8"></a>
### 1、交换两个变量
```python
# a = 4 b = 5
a,b = b,a
# print(a,b) >> 5,4
```
此方法是最简单、最直观的方法之一，无需使用临时变量或应用算术操作即可编写。
<a name="quL28"></a>
### 2、多个变量赋值
```python
a,b,c = 4,5.5,'Hello'
#print(a,b,c) >> 4,5.5,hello
```
可以使用逗号和变量一次性将多个值分配给变量。使用此技术，可以一次分配多个数据类型。 <br />可以使用列表将值分配给变量。下面是将列表中的多个值分配给变量的示例。
```python
a,b,*c = [1,2,3,4,5]
print(a,b,c)
> 1 2 [3,4,5]
```
<a name="4iXSJ"></a>
### 3、列表中偶数的和<br />
有很多方法可以做到这一点，但最好和最简单的方法是使用列表索引和`sum`函数。
```python
a = [1,2,3,4,5,6]
s = sum([num for num in a if num%2 == 0])
print(s)
>> 12
```
<a name="14eb1ef4"></a>
### 4、从列表中删除多个元素<br />
`del`是Python中用于从列表中删除值的关键字。
```python
#### Deleting all even
a = [1,2,3,4,5]
del a[1::2]
print(a)
>[1, 3, 5]
a
```
<a name="a8QNg"></a>
### 5、读取文件
```python
lst = [line.strip() for line in open('data.txt')]
print(lst)
```
这里使用列表来处理。<br />首先打开一个文本文件，并使用`for`循环，逐行读取。 最后，使用`strip`删除所有不必要的空间。 <br />通过使用列表功能，使得代码更简单，更短。
```python
list(open('data.txt'))
##Using with will also close the file after use
with open("data.txt") as f:
    lst=[line.strip() for line in f]
print(lst)
```
<a name="WF9bu"></a>
### 6、将数据写入文件
```python
with open("data.txt",'a',newline='\n') as f: 
    f.write("Python is awsome")
```
上面的代码首先创建一个文件data.txt(如果没有的话)，然后它会在文件中写Python is awesome。
<a name="d9a48a69"></a>
### 7、创建列表
```python
lst = [i for i in range(0,10)]
print(lst)
> [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
or
lst = list(range(0,10))
print(lst)
```
也可以使用相同的方法创建一个字符串列表。
```python
lst = [("Hello "+i) for i in ['Karl','Abhay','Zen']]
print(lst)
> ['Hello Karl', 'Hello Abhay', 'Hello Zen']
```
<a name="BgLcS"></a>
### 8、映射列表或类型转换整个列表
有时在项目中，需要更改列表中所有元素的数据类型。第一个方法可能是使用循环，然后访问列表中的所有元素，然后一个接一个地更改元素的数据类型。这个方法是老派的，在Python中有一个映射函数，可以做这些工作。
```python
list(map(int,['1','2','3']))
> [1, 2, 3]
list(map(float,[1,2,3]))
> [1.0, 2.0, 3.0]
[float(i) for i in [1,2,3]]
> [1.0, 2.0, 3.0]
```
<a name="TzWmj"></a>
### 9、创建集合
用于创建列表的方法也可以用于创建集合。使用包含范围内所有偶数的平方根方法来创建一个集合。
```python
#### Square of all even numbers in an range
{x**2 for x in range(10) if x%2==0}
> {0, 4, 16, 36, 64}
```
<a name="8gy3g"></a>
### 10、Fizz Buzz<br />
在这个测试中，需要编写一个程序来打印从1到20的数字。但如果是3的倍数，打印Fizz，如果是5的倍数，打印Buzz，如果同时是3和5的倍数，打印FizzBuzz，否则打印数字。<br />看起来必须使用循环和多个`if-else`语句。如果尝试用其他语言来做，可能需要写10行代码，但是使用Python，可以只用一行代码就实现FizzBuzz。
```python
['FizzBuzz' if i%3==0 and i%5==0
    else 'Fizz' if i%3==0
    else 'Buzz' if i%5==0
    else i  for i in range(1,20)]
```
在上面的代码中，使用列表理解来运行一个从1到20的循环，然后在循环的每次迭代中，检查数字是否能被3或5整除。如果是，那么用Fizz或Buzz替换数值，或者用FizzBuzz数值。
<a name="7e61930b"></a>
### 11、回文
回文是一个数字或字符串，当它被反转时看起来是一样的。
```python
text = 'level'
ispalindrome = text == text[::-1]
ispalindrome
> True
```
<a name="6bacc285"></a>
### 12、用空格分隔的整数到一个列表
```python
lis = list(map(int, input().split()))
print(lis)
> 1 2 3 4 5 6 7 8
[1, 2, 3, 4, 5, 6, 7, 8]
```
<a name="b19a5dc5"></a>
### 13、Lambda函数<br />
`lambda`函数是一个小型匿名函数。`lambda`函数可以接受任意数量的参数，但只能有一个表达式。
```python
# Function that returns square of any number
sqr = lambda x: x * x
sqr(10)
> 100
```
<a name="387e2ed0"></a>
### 14、检查列表中数字的存在
```python
num = 5
if num in [1,2,3,4,5]:
     print('present')
> present
```
<a name="21QRF"></a>
### 15、打印图案<br />
在Python中，只需要一行代码就可以绘制出令人惊叹的图案。
```python
n = 5
print('\n'.join('😀' * i for i in range(1, n + 1)))
>
😀
😀😀
😀😀😀
😀😀😀😀
😀😀😀😀😀
```
<a name="aAGjA"></a>
### 16、查找阶乘<br />
阶乘是一个整数和它下面所有整数的乘积。
```python
import math
n = 6
math.factorial(n)
> 720
```
<a name="MjsWP"></a>
### 17、斐波纳契数列
一组数字，其中每个数字(斐波那契数)是前面两个数字的和。最简单的斐波那契数列1，1，2，3，5，8，13等等。可以使用列表推导式和`for`循环在一个范围内创建斐波那契数列。
```python
fibo = [0,1]
[fibo.append(fibo[-2]+fibo[-1]) for i in range(5)]
fibo
> [0, 1, 1, 2, 3, 5, 8]
```
<a name="daz0U"></a>
### 18、质数<br />
质数是一个只能被自身和1整除的数。例如:2、3、5、7等。为了在一个范围内生成质数，可以使用带有`filter`和`lambda`的`list`函数来生成质数。
```python
list(filter(lambda x:all(x % y != 0 for y in range(2, x)), range(2, 13)))
> [2, 3, 5, 7, 11]
```
<a name="REjsn"></a>
### 19、查找最大数值
```python
findmax = lambda x,y: x if x > y else y 
findmax(5,14)
> 14
or
max(5,14)
```
在上面的代码中，使用`lambda`函数检查比较条件，并根据返回的最大值。<br />或者使用`max()`内置函数。
<a name="1L66H"></a>
### 20、线性代数<br />
有时候需要将列表中的元素缩放2到5倍。下面的代码解释了如何做到这一点。
```python
def scale(lst, x):
    return [i*x for i in lst] 
scale([2,3,4], 2)
> [4,6,8]
```
<a name="Qtp0h"></a>
### 21、矩阵转置<br />
如果需要将所有行转换为列，反之亦然。在Python中，可以使用`zip`函数在一行代码中置换一个矩阵。
```python
a=[[1,2,3],
   [4,5,6],
   [7,8,9]] 
transpose = [list(i) for i in zip(*a)] 
transpose
> [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
```
<a name="72QAB"></a>
### 22、计数<br />
当需要知道某个值在文本中出现的次数时，这是一个重要而有用的用例。在Python中，有`re`库可以来完成这项工作。
```python
import re
len(re.findall('python','python is a programming language. python is python.'))
> 3
```
<a name="9dec63f4"></a>
### 23、用其他文本替换文本
```python
"python is a programming language.python is python".replace("python",'Java')
> Java is a programming language. Java is Java
```
<a name="O6YD0"></a>
### 24、模拟抛硬币<br />
这可能不是那么重要，但当需要从一组给定的选择中生成一些随机选择时，它会非常有用。
```python
import random
random.choice(['Head',"Tail"])
> Head
```
<a name="o8XcJ"></a>
### 25、生成组
```python
groups = [(a, b) for a in ['a', 'b'] for b in [1, 2, 3]] 
groups
> [('a', 1), ('a', 2), ('a', 3), ('b', 1), ('b', 2), ('b', 3)]
```

