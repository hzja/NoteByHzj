Python 循环 判断<br />流程控制是Python语法很重要的一个分支，主要包括经常用到的判断语句、循环语句以及各种表达式，这也是上一篇文章没有介绍表达式的原因，在这篇文章中会更加系统全面的讲解这三方面的基础知识。
<a name="LGLxc"></a>
## 判断语句(`if`)
判断语句中最有名的应该就是if-else的组合，并且很多语言都通用这种格式，但是对于`elif`而言，不同语言表达形式可能会不同：
```typescript
In [1]: x = 5
In [2]: if x>0:
   ...:     print('正整数')
   ...: elif x<0:
   ...:     print('负整数')
   ...: else:
   ...:     print('零')
   ...:
正整数
```
一组判断语句可能会有零到多个 elif 部分，else 是可选的。关键字 `elif`  是 `else if` 的缩写，由于Python对缩进的要求很严格，而这种写法恰巧可以有效地避免过深的缩进。`if ... elif ... elif ... `序列用于替代其它语言中的 switch 或 case 语句。
<a name="kMLms"></a>
## 循环语句
<a name="GfaA7"></a>
### 1、for循环
要注意Python和C语言中的for语句表达形式完全不同。Python 的 for 语句依据任意序列(字符串、列表)中的子项，按照它们在序列中的顺序来进行迭代。
```typescript
In [3]: str1 = 'mao'

In [4]: for i in str1:
   ...:     print(i)
   ...:
m
a
o
```
对列表的循环与字符串几乎一致，只是子项不同。
```typescript
In [5]: list1 = ['a','aa','aaa']
In [6]: for i in list1:
   ...:     print(i,len(i))
   ...:
a 1
aa 2
aaa 3
```
`for`语句与`range`函数结合是一种很常见的组合，`range`函数主要提供一个数值序列。
```typescript
In [8]: for j in range(len(list1)):
   ...:     print(j,list1[j])
   ...:
0 a
1 aa
2 aaa
```
`range`提供的数值索引方式可以参考列表的切片，同样默认以0开始，且不包括最后一位，上面这种场合利用`enumerate()`函数表达会更加简单。
```typescript
In [9]: for i,v in enumerate(list1):
   ...:     print(i,v)
   ...:
0 a
1 aa
2 aaa
```
<a name="ZBT4K"></a>
### 2、`while`语句
```typescript
In [10]: i = 1
In [11]: list2 = []
In [12]: while i<=5:
    ...:     list2.append(i)
    ...:     i+=1
In [14]: list2
Out[14]: [1, 2, 3, 4, 5]
```
<a name="tsf7n"></a>
### 3、`break`、`continue`、`pass`
`break`语句的作用就是用来跳出一个`for`或`while`循环。
```typescript
In [15]: for i in range(0,5):
    ...:     if i == 3:
    ...:         break
    ...:     else:
    ...:         print(i)
0
1
2
```
可以看到在`for`循环中加了一个判断语句，当i等于3时会跳出`for`循环，不再继续执行，输出与语义符合。<br />`continue`语句的作用就是表示继续执行下一次迭代，可以结合判断语句使用，在什么条件下继续执行，或者什么条件下跳出循环。
```typescript
In [20]: for i in range(2,7):
    ...:     if i%2==0:
    ...:         print('An even number',i)
    ...:         continue
    ...:     if i>4:
    ...:         print('beyond 4')
    ...:         break
    ...:
An even number 2
An even number 4
beyond 4
```
`pass`语句主要起到一个占位的作用，而有的语句后面是不能为空的，比如`if`、`for`、`while`，可以利用`pass`占位，不会发生报错。
```typescript
In [1]: while True:
   ...:     pass
```
<a name="ybj8Y"></a>
### 迭代器、生成器
通常接触到的迭代器有序列，比如字符串、列表等等，利用`iter()`方法可以构建一个迭代器，与之匹配的`next()`方法可以迭代返回迭代器内的值，并将返回值弹出迭代器。
```typescript
In [1]: it = iter('python')
In [2]: print(it)
<str_iterator object at 0x00000187ADA75A00>

In [3]: next(it)
Out[3]: 'p'
In [4]: next(it)
Out[4]: 'y'
```
当`next`方法迭代到最后一个值时，继续调用`next`方法会弹出报错。当然也可以利用`for`循环迭代输出：
```typescript
In [6]: for i in it:
   ...:     print(i)
t
h
o
n
```
如果深入了解Python之后会发现生成器用起来会更加方便，结合函数即可。生成器的关键字为`yield`，生成器也具有`next()`方法，并且可以利用for循环迭代输出。
```typescript
In [8]: def reverse(data):
   ...:     for i in range(len(data)-1,-1,-1):
   ...:         yield data[i]
In [9]: gen = reverse('mao')
In [10]: print(gen)
<generator object reverse at 0x00000187AD99FDD0>

In [11]: next(gen)
Out[11]: 'o'
In [12]: for i in gen:
    ...:     print(i)
a
m
```
用小括号括起来的一个表达式可以用来创建一个生成器，下面将讲述如何来书写各种的表达式。
```typescript
In [14]: gen2 = (i for i in range(0,5))

In [15]: print(gen2)
<generator object <genexpr> at 0x00000187ADA18D60>
```
<a name="fZvZ3"></a>
## 表达式
<a name="t0qiQ"></a>
### 列表表达式
各种表达式创建相关数据类型提供了一种更加简单的方法，首先是列表表达式，普通的方法是通过将一些操作应用于序列的每个成员并通过返回的元素创建列表，或者通过满足特定条件的元素创建子序列。比如想获取10以内的平方数，利用普通方法如下：
```typescript
In [16]: squares = []
In [17]: for i in range(10):
    ...:     squares.append(i ** 2)

In [18]: squares
Out[18]: [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```
但一行语句就足以囊括上面的内容，可见列表表达式的简洁性。
```typescript
In [20]: squares = [x**2 for x in range(10)]

In [21]: squares
Out[21]: [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```
上面这个列表表达式应该是比较简单的，还可以在表达式中加入判断语句作为筛选条件。
```typescript
In [22]: list2 = [x*2 for x in range(10) if x*2>3]

In [23]: list2#0和2被过滤掉了
Out[23]: [4, 6, 8, 10, 12, 14, 16, 18]
```
进一步可以结合两个列表的元素组成一个列表表达式:
```typescript
In [24]: list3 = [(x,y) for x in [1,2] for y in [2,3]if x!=y]

In [25]: list3
Out[25]: [(1, 2), (1, 3), (2, 3)]
```
在复杂一点的列表表达式就是嵌套类型的，但是这样的表达式缺点就是可读性不太好，因为需要去思考他的一些细节，比如将一个23的矩阵转变为32的矩阵。
```typescript
In [28]: mat = [[1,2,3],[4,5,6]]
In [29]: list3 = [[row[i] for row in mat] for i in range(3)]

In [31]: list3
Out[31]: [[1, 4], [2, 5], [3, 6]]
```
<a name="Mlkuw"></a>
### 集合表达式
集合表达式与列表表达式有两处不同：

- 1、集合表达式需要用`{}`
- 2、集合表达式返回的是集合，所以会对元素去重
```typescript
In [32]: set1 = {i for i in 'abcdddddd' if i not in 'abc'}

In [33]: set1
Out[33]: {'d'}
```
可以看到`for`语句中的字符串原本有很多d，经过条件过滤和集合去重最后只剩下了一个。<br />当然也可以通过`set()`方法将一个列表表达式转换为集合的形式。
```typescript
In [34]: set2 = set([(x,y) for x in range(2) for y in range(2)])

In [35]: set2
Out[35]: {(0, 0), (0, 1), (1, 0), (1, 1)}
```
<a name="aSbPz"></a>
### 字典表达式
字典是Python中很常用的一种数据类型，所以字典表达式也显得尤为重要。字典表达式同样也用`{}`构建，只不过表达式还要用(`key:value`对)的形式。
```typescript
In [36]: dict1 = {x:x**2 for x in range(1,4)}
In [37]: dict1
Out[37]: {1: 1, 2: 4, 3: 9}
```
字典表达式也可以结合一些方法、语句实现更多的功能。
```typescript
In [38]: dict2 = {x:len(x) for x in ['aa','bb','ccc']}
In [39]: dict2
Out[39]: {'aa': 2, 'bb': 2, 'ccc': 3}
```
利用`items()`方法很容易实现键值对转换，不过需要注意字典的键需要唯一。
```typescript
In [40]: dict3 = {v:k for k,v in dict2.items()}
In [41]: dict3
Out[41]: {2: 'bb', 3: 'ccc'}
```
通过上面几个例子可以看到，表达式是由包含一个表达式的括号组成，表达式后面会跟一个`for`语句，之后还可以跟零或者多个`for`或`if`语句，结果最终会返回一个指定的数据类型，其中的元素是通过表达式依据后面的`for`和`if`语句计算而得出的。
