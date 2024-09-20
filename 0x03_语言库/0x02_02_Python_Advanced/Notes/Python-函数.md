Python 函数
<a name="IpZkN"></a>
### 函数的概念
函数是将具有独立功能的代码块组织成为一个整体，使其具有特殊功能的代码集。它是编辑好的、可直接调用的、可重复使用的，用来实现单一或相关联功能的代码段<br />**优点：**

- 提高程序的模块性；<br />
- 使程序变得可扩展；<br />
- 减少重复代码；<br />
- 使程序变得可维护；<br />
<a name="4V8pb"></a>
### 函数的定义
函数必须先创建才可以使用，该过程称为函数定义，使用函数的过程称为函数调用。<br />函数可以根据自己的需求来定义，但有一些规则需要注意：

- 函数代码块必须以def开头，后接函数标识符名称和圆括号()；<br />
- 任何传入参数和自变量必须放在圆括号中间，圆括号之间可以用于定义参数；<br />
- 函数的第一行语句可以选择性的使用文档字符-用于存放函数说明；<br />
- 函数内容以冒号起始，并且缩进；<br />
- 函数名称的命名应符合标识符的命名规则；（可由字母、下划线和数字组成；不能以数字开头；不能与关键字重名）<br />
- return[表达式]结束函数，选择性地返回一个值给调用方。不带表达式的return相当于返回None

**函数定义的一般格式：**
```python
#函数定义的一般格式：
def 函数名 (参数列表)：
   函数体
#实例1
def hello():
   print('Hello World')
   
hello()
#Hello World
```
**实例2-计算圆的面积,带参数**
```python
#实例2-计算圆的面积,带参数
def area(r):
   pi=3.412
   return pi*(r^2)
r=5
print('圆的面积为:',area(r))
#圆的面积为: 23.884
```
<a name="3yHsH"></a>
### 函数调用
即使用函数的过程<br />**简单的调用应用**
```python
#函数功能：打印该字符串
def println(str):
   print(str)
   return
#调用函数
println('调用上述函数')
println('打印这段字符串')
println('多次调用函数')
#调用上述函数
#打印这段字符串
#多次调用函数
```
<a name="AfwTO"></a>
### 参数调用
参数分为形参、实参

- 形参：函数定义时括号内的参数<br />
- 实参：函数调用时括号内的参数<br />
- 形参相当于变量，实参相当于变量的值<br />

在实际代码中表示如下：
```python
#定义时：
def func(a,b,c):
   pass
其中 a,b,c 为形参
#调用时：
func(1,2,3)
其中1，2，3为实参
```
<a name="6QjbW"></a>
#### 形参
只在被调用时，才分配存储单元。调用结束，立刻释放所分配的内存。<br />形参只在函数内部有效
<a name="MFch9"></a>
#### 实参
可以是：常量、变量、表达式、函数<br />进行函数调用时，实参必须是确定的值
<a name="3XycE"></a>
#### 位置参数

- 位置形参：函数定义时，从左往右写的参数，比如上面的 a, b , c<br />
- 位置实参：函数调用时，从左往右写的参数,  比如上面的 1，2，3<br />
- 位置形参定义多少个，调用时位置实参必须写上多少个，多一个少一个都不行。<br />
- 位置参数必须以正确的顺序传入函数。调用时的数量必须和声明时的一样。
```python
def func(a,b,c):
   return a+b+c
#调用
print(func(2,3,4))  
#9
```
错误演示：
```python
def func(a,b,c):
   return a+b+c
#调用
print(func(2,3)) #实参数量不对应，报错
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-21-38b482e25fef> in <module>
     3
     4 #调用
----> 5 print(func(2,3))
TypeError: func() missing 1 required positional argument: 'c'
```
<a name="THYhQ"></a>
#### 关键参数

- 正常情况下，给函数传参数，要按顺序。如果不想按顺序，就用关键参数。<br />
- 指定参数名的参数，就叫做关键参数。<br />
- 函数调用时：func(a=1, b=2),  这种指定了参数名的参数，就是关键参数。<br />
- 调用函数时，关键参数可以和位置参数一起用，但是关键参数必须在位置参数的后面。不然会报错。
```python
def func(a,b,c):
   return a+b+c
#调用
print(func(4,c=3,b=2))  
#9
```
<a name="0eNGU"></a>
#### 默认参数
> - 函数定义时，默认参数必须在位置形参的后面。<br />
> - 函数调用时，指定参数名的参数，叫关键参数。<br />
> - 而在函数定义时，给参数名指定值的时候，这个参数叫做默认参数。

> - 关键参数，和默认参数两个参数写法一样，区别在于：<br />关键参数是在函数调用时，指定实参的参数名，也可以说指定值的参数名。<br />默认参数是在函数定义时，指定参数名的值。<br />
> - 写法：

```python
def (a, b=100):
   pass
```
> - 定义时，有默认参数的话，调用时，这个实参可以不写。如果实参不写的话，这个形参的参数值是他的默认值。

实例演示：
```python
def info(name,age=18):
   print('名字:',name)
   print('年龄:',age)
   return
#调用
info(age=22,name='Jack')
print('****************')
info(name='Jack')
'''
名字: Jack
年龄: 22
*****************
名字: Jack
年龄: 18
'''
```
<a name="FwaR1"></a>
#### 动态参数：* args    ** kwargs
当需要一个函数能处理很多参数，超过已声明的参数数量，这时就需要动态参数。与上述两中参数不同的是，该参数声明不需要命名。
<a name="fO7QO"></a>
##### * args
一个'* '的参数会以元组(tuple)的形式导入，存放未命名的变量参数
```python
#函数定义
def printinfo(arg1,*vartuple):
   print('输出：')
   print(arg1)
   print(vartuple)
   
#调用
printinfo(34,45,32,12)
#输出：
#34
#(45, 32, 12)
```
如果函数调用时没有指定参数，动态参数则表现为空元组。
```python
#函数定义
def printinfo(arg1,*vartuple):
   print('输出：')
   print(arg1)
   print(vartuple)
   
#调用
printinfo(34)
#输出：
#34
#()
```
<a name="DUdHT"></a>
##### ** kwargs
还有一种动态参数,加了两个星号则以字典的形式导入
```python
#函数定义
def printinfo(arg1,**vardict):
   print('输出：')
   print(arg1)
   print(vardict)
   
#调用
printinfo(34,a=45,b=32,c=12)
#输出：
#34
#{'a': 45, 'b': 32, 'c': 12}
```
> 声明函数时，参数中的* 可以单独出现。
> 如果* 单独出现，后面的参数必须以关键字传入

```python
def func(a,b,*,c):
   return a+b-c
func(3,2,4)   #非关键字传入报错
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-34-f69556d382a4> in <module>
     2     return a+b-c
     3
----> 4 func(3,2,4)
TypeError: func() takes 2 positional arguments but 3 were given
```
关键字传入正确
```python
func(3,4,c=5)  
#2
```
<a name="cQxGP"></a>
#### 参数传递
> python语言中，类型属于对象，变量是没有类型的,例如:
> a=[1,2,3]
> a='DataScience'
> 其中[1,2,3]是list类型，'DataScience'是string类型，变量a没有类型，它只是一个对象的引用（一个指针），可以指向list类型对象也可以指向string类型对象

<a name="icfH5"></a>
### 可更改(mutable)与不可更改(immutable)对象
> 在 python 中，strings, tuples, 和 numbers 是不可更改的对象，而 list,dict 等则是可以修改的对象。
> - 不可变类型：变量赋值 a=5 后再赋值 a=10，这里实际是新生成一个 int 值对象 10，再让 a 指向它，而 5 被丢弃，不是改变a的值，相当于新生成了a。<br />
> - 可变类型：变量赋值 la=[1,2,3,4] 后再赋值 la[2]=5 则是将 list la 的第三个元素值更改，本身la没有动，只是其内部的一部分值被修改了。<br />
> 
**python 函数的参数传递：**
> - 不可变类型：类似 c++ 的值传递，如 整数、字符串、元组。如fun（a），传递的只是a的值，没有影响a对象本身。比如在 fun（a）内部修改 a 的值，只是修改另一个复制的对象，不会影响 a 本身。<br />
> - 可变类型：类似 c++ 的引用传递，如 列表，字典。如 fun（la），则是将 la 真正的传过去，修改后fun外部的la也会受影响<br />
> 
python 中一切都是对象，严格意义不能说值传递还是引用传递，应该说传不可变对象和传可变对象。

```python
#传不可变对象实例
#
#函数定义
def changeInt(a):
   a=10
   
#函数调用
b=5
changeInt(b)
print(b)
#5
```
实例中有 int 对象 2，指向它的变量是 b，在传递给 ChangeInt 函数时，按传值的方式复制了变量 b，a 和 b 都指向了同一个 Int 对象，在 a=10 时，则新生成一个 int 值对象 10，并让 a 指向它。
```python
#传可变对象实例
def changelist(List):
   #对传入的列表进行扩展
   List.append([1,2,3,4])
   print('函数内取值:',List)
   return
#函数调用
List=['datascience']
changelist(List)
print('函数外取值:',List)
#函数内取值: ['datascience', [1, 2, 3, 4]]
#函数外取值: ['datascience', [1, 2, 3, 4]]
```
可变对象在函数里修改了参数，那么在调用这个函数的函数里，原始的参数也被改变了。<br />传入函数的和在末尾添加新内容的对象用的是同一个引用。
<a name="4Q736"></a>
### 匿名函数

- python可以使用lambda来创建匿名函数<br />
- 所谓匿名，即不再使用def这样的标准语句来专门定义函数<br />
- lambda的主体是一个表达式，而不是一个代码块。仅仅能在lambda中封装有限的逻辑进去。<br />
- lambda函数拥有自己的命名空间，且不能访问自己参数列表之外或全局命名空间中的参数。<br />
- 有名函数的定义规则为【def+函数名+参数】；而匿名函数则是用【lambda+参数+表达式】的方式定义函数<br />

lambda的语法只包含一个语句，格式如下：
```python
lambda [arg1[,arg2,...,argN]]:expression
```
下面用几个例子对两种函数进行对比，以便理解。
<a name="PrTNb"></a>
#### 用def格式定义匿名函数
```python
#用def格式写
def func(x,y):
   return x*y
func(2,3)
#6
```
<a name="Rm5KM"></a>
#### 用匿名函数定义匿名函数
```python
#用匿名函数写
func=lambda x,y:x*y
func(3,4)
#12
```
从上面例子可以看出**匿名函数相较标准函数的优点有**：

- 不用取名称，因为给函数取名是比较头疼的一件事，特别是函数比较多的时候可以直接在使用的地方定义，如果需要修改，直接找到修改即可，方便以后代码的维护工作<br />
- 语法结构简单<br />
<a name="pWrif"></a>
### 函数的返回值
**return[表达式]语句用于退出函数，选择性的向调用方返回一个表达式。不带参数值的return语句返回None。**<br />下面演示一下return语句的用法：
```python
def sum(arg1,arg2):
   "返回两个参数的和"
   total=arg1+arg2
   print('函数内：',total)
   return total
#调用sum函数
total=sum(23,32)
print('函数外：',total)
#函数内：55
#函数外：55
```
<a name="bOZPT"></a>
### 函数的说明文档

- 定义：即对函数进行简单的解释说明（一般对参数和返回值进行说明）<br />
- 作用：方便他人理解和自己日后的复读<br />

**语法结构如下：**
```python
def 函数名（参数）:
   '函数的说明文档内容'
   函数体
   return 返回值
```
对于函数的内置函数，可以使用help()函数查看其内置文档。
<a name="HtzyH"></a>
### 函数的嵌套
在函数中再定义一个函数<br />**语法结构如下：**
```python
def outer():
   def inner():
       print('inner')
   print('outer')
   inner()
outer()
# inner() #该句会报错
#outer
#inner
```
**一个案例**
```python
#一个案例
def outer():
   str='Hello World'
   def inner():
       print(str)
   return inner
info=outer()
info()      
#Hello World
```
<a name="6J2oA"></a>
### 变量作用域
一个标识符的可见范围，就是标识符的作用域。一般常说的是变量的作用域。

- 函数中的局部作用域：local<br />
- 嵌套函数中父级函数的局部作用域：enclosing<br />
- 全局作用域：global<br />
- 系统内置的变量：如 int、str、list 等关键字<br />
- 局部变量：在某个函数内部定义，作用在函数内部。生命周期：从变量被创建开始到函数结束死亡。<br />
- 全局变量：定义在.py模块内部，作用在整个.py模块。生命周期：从变量被创造开始到.py模块结束死亡。<br />

**案例演示**
```python
def Demo1():
   num=1
   print(num)
   
Demo1()
print(num)    #该句报错。因为num是局部作用，所以在函数外面是找不到num这个变量的。
#1
---------------------------------------------------------------------------
NameError                                 Traceback (most recent call last)
<ipython-input-56-d3aaf0ba8dea> in <module>
     5
     6 Demo1()
----> 7 print(num)
NameError: name 'num' is not defined
```
对上例的解释
```python
num = 0
def Demo1():
   num = 1
   print("在demo1中的结果:",num)
def Demo2():
   print("在demo2中的结果:",num)
Demo1()
Demo2()
#在demo1中的结果: 1
#在demo2中的结果: 0
```
> 从结果可以看出，在demo1中num = 1，只在demo1内部起作用，而全局变量num仍然是0；因为在python中，在函数内部改变全局变量的值，会默认的在函数内部创建一个新的变量，全局变量并没有改变。
> 要想要改变全局变量的值，需要在函数内部用global声明。

现修改上面代码如下：
```python
num = 0
def Demo1():
   global num
   num = 1
   print("在demo1中的结果:",num)
def Demo2():
   print("在demo2中的结果:",num)
Demo1()
Demo2()
print("在函数外面中的结果:",num)
#在demo1中的结果: 1
#在demo2中的结果: 1
#在函数外面中的结果: 1
```
> 还有一种情况是当全局变量是可变数据类形，可以通过修改可变数据类型的方法，修改可变类型的内容，从而实现修改全局变量。（这里因为可变数据类型改变时，会在内存中改变数据的值）

```python
num_list = [1,2,3]
def Demo1(demo1_list):
   demo1_list.append(4)
   print("在demo1中的结果:",demo1_list)
def Demo2():
   print("在demo2中的结果:",num_list)
Demo1(num_list)
Demo2()
print("在函数外面中的结果:",num_list)
#在demo1中的结果: [1, 2, 3, 4]
#在demo2中的结果: [1, 2, 3, 4]
#在函数外面中的结果: [1, 2, 3, 4]
```
<a name="yDgKr"></a>
#### 列表 += 与 + 的区别
<a name="1GQRV"></a>
##### #+：
```python
num_list = [1,2,3]
def Demo1(demo1_list):
   demo1_list = demo1_list + [4,5,6]
   print("在demo1中的结果:",demo1_list)
def Demo2():
   print("在demo2中的结果:",num_list)
Demo1(num_list)
Demo2()
print("在函数外面中的结果:",num_list)
#在demo1中的结果: [1, 2, 3, 4, 5, 6]
#在demo2中的结果: [1, 2, 3]
#在函数外面中的结果: [1, 2, 3]
```
<a name="JsCTp"></a>
##### #+=：
```python
num_list = [1,2,3]
def Demo1(demo1_list):
   demo1_list += [4,5,6]
   print("在demo1中的结果:",demo1_list)
def Demo2():
   print("在demo2中的结果:",num_list)
Demo1(num_list)
Demo2()
print("在函数外面中的结果:",num_list)
#在demo1中的结果: [1, 2, 3, 4, 5, 6]
#在demo2中的结果: [1, 2, 3, 4, 5, 6]
#在函数外面中的结果: [1, 2, 3, 4, 5, 6]
```
> 因为：

> 对于+号操作，可变对象和不可变对象调用的都是**add**操作
> 对于+=号操作，可变对象调用**add**，不可变对象调用的是**iadd**(不可变对象没有**iadd**) **iadd**是原地修改

<a name="oiI5A"></a>
### 函数递归

- 定义：在调用一个函数的过程中直接或间接的调用该函数本身，称之为递归调用。<br />
- 递归调用最多能调用999层。
```python
#基础模型一
def func():
   print('from func')
   func()    #直接调用自身
   
func()
   
#基础模型二
def func():
   print('from func')
   bar()    #间接调用自身
def bar():
   print("from bar")
   func()
func()
```
> 虽然以上两中方式为函数递归的基础模型，但往往不能直接这样使用。因为没有一个函数的结束条件，仅仅相当于一个死循环。

> <a name="e361da83"></a>
###### 递归分为两个重要的阶段: 递推+回溯
> - 递推：函数不断减少问题规模直至最终的终止条件。<br />
> - 回溯：拿到最终明确的值后，返回给上次调用进行处理，直至初始层。<br />

案例：解决年龄问题，求出Jack的年龄
```python
"""
Jack 他比小王 大两岁。4   age(3) + 2
小王 他比大枫 大两岁。3   age(2) + 2
大枫 他比美丽 大两岁。2   age(1) + 2
美丽：我今年18.         1   18
"""
def age(n):
   if n == 1:
       return 18
   else:
       return age(n-1) + 2
print(age(4))
#24
```
> 注意在Python：

> 　　　　1、递归调用必须有一个明确的结束条件
> 　　　　2、在python中没有尾递归优化,递归调用的效率不高
> 　　　　3、进入下一次递归时,问题的规模必须降低
> 再来一个例子输入任意一个数，让其除以2，直到不能再除

```python
def  cal(num):
   if  num%2==0:#先判断能不能整除
       num=num//2
       return cal(num)
   else:
       return num
print(cal(8))
#1
```

