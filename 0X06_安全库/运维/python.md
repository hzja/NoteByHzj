<a name="UKWtO"></a>
# day1基础知识
<a name="nJHAB"></a>
## 1、导学内容
```python
print("--------我爱鱼C工作室-------")
temp=input("不妨猜一下我现在心里想的这个数字:")		 	#这个是一个输入函数类似于C语言中的scanf函数
guess=int(temp)										 #将temp变量中的值取为整数
if guess==8:										 #if条件判断后面":"结尾，一个等号是赋值两个等号是判断是等于
    print("卧槽你好牛逼啊")
else:
    print("猜错了我现在想的这个不是这个数字哦")
print("游戏结束")
```
<a name="EiKdd"></a>
## 2、字符串与变量
```c
teacher = "小甲鱼"
print(teacher)	#会打印出小甲鱼
teacher = "老甲鱼"
print(teacher)	#会打印出老甲鱼		#说明：要是变量名重复的话会将之前的变量给替换掉
first = 3
second = 8
third =first + second
print(third)		#会打印出11			#相当于字符串的运算
myteacher = "小甲鱼"
yourteacher = "黑夜"
outtecher = myteacher + yourteacher
print(outtecher)	#打印出小甲鱼黑夜，是字符串的拼接
```
**注意：**在使用变量的时候，需要对其赋值。变量名可以包括字母、数字、下划线、变量名不能是以数字开头
```python
import  random
secret = random.randint(1,10)
print("-----随机字符猜测-------")
temp = input("不妨猜一下小甲鱼现在心里想的这个数字：")
guess=int(temp)
while guess != secret:
    temp=input("哎呀，猜错了请重新输入吧:")
    guess=int(temp)
    if guess == secret:
        print("wocao，牛逼")
    else:
        if guess > secret:
            print("哥，猜大了")
        else:
            print("哥，猜小了")
print("游戏结束")
```
<a name="UWHqE"></a>
## 3、print语句
```c
# print(value, ..., sep=' ', end='\n', file=sys.stdout, flush=False)
print("I\'m","jiang","jingwei",sep="-----")
```
I'm-----jiang-----jingwei

**2、示例2**
```c
print("I\'m","jiang","jingwei",sep="-----",end="^^^^^^^^^^")
print("I\'m","jiang","jingwei")
```
I'm-----jiang-----jingwei^^^^^^^^^^I'm jiang jingwei
<a name="iHQCc"></a>
## 4、input语句
```c
age = input("请输入你的年龄:")
print(type(age))			#input函数的默认类型为字符串不能进行四则的运算
请输入你的年龄:10
<class 'str'>
Traceback (most recent call last):
  File "D:/python/day1.py", line 3, in <module>
    print(age+1)
TypeError: can only concatenate str (not "int") to str
#将字符串格式的数据转换为数字就可以了
age = input("请输入你的年龄:")
print(type(age))
age =  int(age)
print(age+1)
请输入你的年龄:10
<class 'str'>
11

Process finished with exit code 0

```
<a name="GtPno"></a>
## 5、进制的表达方式

1、Python里的数据类型: 整型(int)浮点型(fLoat）复数(complex) 字符串(str)布尔(bool)列表(List)元组(tuple)字典(dict)集合(set)<br />2、二进制、八进制、十六进制十进制在Python里都能够表示<br />3、整型就是整数。计算机其实只能保存二进制0和1,为了方便数据的表示，同时计算机也支持几进制和十六进制<br />备注：默认表示的是10进制数据例如 num =10表示的是十进制的数字10

**2进制数的表达方式**
```c
num=0b1001011
print(num)		#输出的结果是75使用print语句打印一个数字的时候、默认使用的是10进制的方式打印输出
```

**8进制的表达方式**
```c
num=0o1234
print(num)		#输出的结果是668使用print语句打印一个数字的时候、默认使用的是10进制的方式打印输出
```

**16进制的表达方式**
```c
num=0x23
print(num)		#输出的结果是35使用print语句打印一个数字的时候、默认使用的是10进制的方式打印输出
```

<a name="ckKOt"></a>
## 6、进制的转换

**10进制转换为2进制**
```c
num=1023
print(bin(num))			#0b1111111111
```
**10进转换为8进制**
```c
num=1023
print(oct(num)) 		#0o1777
```
**10进制转换为16进制**
```c
num=1023
print(hex(num))		#0x3ff
```

<a name="cAZrV"></a>
## 7、数据类型的转换

**示例1**
```c
age = input("请输入你的年龄：")
print(type(age))
new_age =int(age)
print(type(new_age))
print(new_age+1)
```
**运行结果**
```c
C:\Users\jiang\project\Scripts\python.exe D:/python/day1.py
请输入你的年龄：10
<class 'str'>
<class 'int'>
11

Process finished with exit code 0
```
备注：要是不进行数据类型的转换input输入的默认是字符类型是不能进行加减、必须进行数据类型的转换才能进行数据类型的转换。

**示例2**
```c
str="hello"
new_str=int(str)		#这样就会报错因为str不是一个数字不能进行数字的转换
```
**运行结果**
```c
Traceback (most recent call last):
  File "D:/python/day1.py", line 8, in <module>
    new_str=int(str)
ValueError: invalid literal for int() with base 10: 'hello'
```
**示例3**<br />**
```c
num = "1a2c"
new_num=int(num,16)		#以16进制的方式进行转换
print(new_num)			#输出的结果是6700
num ="abc"
new_num= int(num,16)	#以16进制的方式进行转换，但是不能超过F因为16进制的数是没得F的
print(new_num)			输出的结果是2748
```

**示例4**
```c
num ="123"
new_num= int(num,8)
print(new_num)			#输出的结果是83
```

<a name="fts9T"></a>
## 8、浮点数的转换

```c
number="12.34"
number_1=float(number)
print(type(number_1))   #<class 'float'>
print(number_1+1)       #13.34
c="abc"
c=float(c)              #字符串不能转换为报错
print(type(c))
n = float("12")     #将字符串转换为浮点型
m = float(12)       #将数字转换为浮点型
print(type(n))      #数据的类型是浮点型
print(type(m))      #数据的类型是浮点型
print(n)            #12.0
print(m)            #12.0 
```
<a name="AFpgi"></a>
## 9、转换为字符串
```python
a = 12
b = str(a)
print(type(a))  #<class 'int'>
print(type(b))  # <class 'str'>
print(a+1)      #结果为13
print(b+1)      #结果为报错,因为字符串的运算规则和数字的运算规则不完全相同
```
<a name="6wDvz"></a>
## 10、转换为bool值
```c
print(bool(100))	#转换为True
print(bool(-1))		#转换为True
print(bool("jisnah"))	#转换为True
print(bool("false")) #转换为True
print(bool(0))		#转换为false
print(bool(False))	#转换为false
print(bool(None))   #转换为false
print(bool(""))		#转换为false
print(bool(()))		#转换为false
print(bool([]))		#转换为false
print(bool({}))		#转换为false
```
**总结：**非0转换为True，0转换为false，空字典、元组、列表转换为false

<a name="fkyK1"></a>
## 11、算数运算符

以下假设变量a为10，变量b为21：

| 运算符 | 描述 | 实例 |
| :--- | :--- | :--- |
| + | 加 - 两个对象相加 | a + b 输出结果 31 |
| - | 减 - 得到负数或是一个数减去另一个数 | a - b 输出结果 -11 |
| * | 乘 - 两个数相乘或是返回一个被重复若干次的字符串 | a * b 输出结果 210 |
| / | 除 - x 除以 y | b / a 输出结果 2.1 |
| % | 取模 - 返回除法的余数 | b % a 输出结果 1 |
| ** | 幂 - 返回x的y次幂 | a**b 为10的21次方 |
| // | 取整除 - 向下取接近商的整数 | >>> 9//2
4
>>> -9//2
-5 |


**加法运算符**
```c
print("hello" + "world")    #将两个字符串拼接到一起输出helloword
print("12" + 1)             #报错字符串与数字不能进行运算
print("hello" * 2)  		#输出hellohello

```

<a name="RMZGR"></a>
## 12、赋值运算符
```c
a=b=c="python"
print(a,b,c)	#打印出来的就是 python python
m,n = (3,5)
print(m,n)		#打印出来的是3，5
y,z=1,2,3,4,5
print(y,z)		#报错
o,*p,q=1,2,3,4,5
print(o,p,q)	#1 [2, 3, 4] 5
o,p,*q=1,2,3,4,5
print(o,p,q)	#1 2 [3, 4, 5]
```

<a name="oZcaW"></a>
## 13、比较运算符
```c
print("a" > "b")	#打印出的是false，字符串的比较是按照ASILL进行比较
print("a" < "b")	#打印出的是True
print("abc" > "b")	#字符串是根据首字母进行一一比较
```

数字和字符串之间做==运算的结果是false，!=运算的结果是True，不支持其他的比较运算
```c
print("a" > 97)	#报错
print("a" == 97)	#false
print("a" != 97)	#true
```
<a name="adFQF"></a>
## 14、逻辑运算符
and 全部为True才返回True<br />or 有一个为True就返回True<br />not 取反 True变成false，false变成True
```c
print(2 > 1 and 5 > 3)		#true
print(2 > 1 and 5 > 3 or 7 >8)	#True
print(not 5 > 6)			#返回True
```

**逻辑运算符的结果不一定只是True和false**
```c
print( 0 or [] or 'lisi' or 'ok')		#lisi
print("good" and "yes" and "ok" and "100")	#100
```
<a name="6V8DX"></a>
# day2循环、分支
python不支持switch语句
<a name="HE68O"></a>
## 1、if单分支
**1、判断一个数字是否能被3和7整除**
```c
num = int(input("请输入一个数字："))
if num % 3 == 0 and  num % 7 ==0:
    print("这个数字能被3和7整除:",num)
else:
    print("这个数字不能被3和7整除",num)
```
**2、判断一个数字是否能被3和7整除但不能同时被3和7整除**
```c
num = int(input("请输入一个数字："))
if num % 3 == 0 and  num % 7 ==0 and num % 21 != 0:
    print("这个数字能被3和7整除:",num)
else:
    print("这个数字不能被3和7整除",num)

```
**3、判断一个年份是否是闰年、判断是否是闰年的标准是能被4整除但不能被100整除或者能够被400整除**
```c
num = int(input("请输入一个年份："))
if num % 4 ==0 and num % 100 !=0 or num % 400 == 0:
    print(num,"这个是闰年")
else:
    print("这个不是闰年")

```
**4、假设今天上课的时间是15678秒计算今天的上课时间是多少小时多少分钟多少秒**
```c
x = 15678
hour = x // 3600
minute = x % 3600 // 60
second = x % 60
print(hour,"小时",minute,"分钟",second,"秒")
```
<a name="LGdYB"></a>
## 2、if多分支结构
```c
score = float(input("请输入你的成绩："))
if  60 > score >= 0:
    print("你个垃圾")
elif 80 > score >= 60:
    print("及格")
elif 90 > score >= 80:
    print("一般般")
elif 100>= score > 90:
    print("牛逼铁汁")
else:
    print("请重新输入你的成绩")
```
<a name="0kqmw"></a>
## 3、if语句的嵌套
```c
tciket = input("你是否买票了：Y/N:")
if tciket == "Y":
    print("您已经买票了,可以通过")
    safe = input("您是否通过了安检Y／Ｎ:")
    if safe == "Y":
        print("您已经通过了安检可以进行乘车")
    else:
        print("您的安检信息没有通过")
else:
    print("请购买您的车票")
```
<a name="TDdPf"></a>
## 4、pass关键字的使用

在python里面没有实际的意义就是单纯的占位、保证语句的完整性
```c
age = int(input("请输入你的年龄："))
if age > 18:
    pass		#没有实际的意义就是单纯的为了占位保证程序能够正常的执行。
print("over")
```

<a name="AWQom"></a>
## 5、循环语句while和for
**1、使用循环计算出****1****到****100****求和的结果**
```c
num =1
sum =0
while num <= 100:
    sum = sum +num
    num = num + 1
print(sum)

r=0
for j in range(1, 101):
    r = r +j
print(r)
```
**2、****统计****100****以内个位数是****2****并且能够被****3****整除的数的个数**
```c
count = 0
for i in range(1,101):
    if i % 10 == 2 and i % 3 == 0:
        print("这个数字有：",i)
        count += 1
print(count)
```
**3、输入任意一个正整数，求它是几位数**
```c
num = int(input("请输入一个数字："))
count = 0
while True:
    count += 1
    num //= 10
    if num == 0:
        break
print('您输入的数字是', count, '位数')
```

**4、打印所有的水仙花数**
```c
for i in range(100, 1000):  # 456除以10,商是45,余数是6
    ge = i % 10
    shi = i // 10 % 10
    bai = i // 100
    if ge ** 3 + shi ** 3 + bai ** 3 == i:
        print(i)
```

**5、写一个程序可以不断的输入内容，如果输入的内容是****exit****，打印****`****程序结束****`****后结束该程序**

```c
while True:
    content = input('请输入内容:')
    if content == 'exit':
        print('程序结束')
        break
```

**6、****求****2****到****100****的合数****(1****既不是质数，也不是合数****;2****是质数****)-1**
```c
for i in range(2, 101):  # i=105
    for j in range(2, int(i ** 0.5)+1):  # range(2,105)  从2取到104  2  3
        if i % j == 0:  # i 除以某一个数字，除尽了,i是合数
            # print(i, '是合数')
            break  # break放在内循环里，用来结束内循环
    else:
        # for...else语句:当循环里的break没有被执行的时候，就会执行else
        print(i, '是质数')

```
**7、求2到100的合数(1既不是质数，也不是合****数;2****是质数****)-2**
```c
for i in range(2, 101):
    # i = 11
    flag = True  # 每次都假设 i 是一个质数
    for j in range(2, int(i ** 0.5) + 1):
        if i % j == 0:
            # 除尽了，说明 i 是个合数
            flag = False
            break
    if flag:  # if flag == True:
        print(i, '是质数')
```
**8、求2到100的合数(1既不是质数，也不是合****数;2****是质数****)-3**
```c
for i in range(2, 101):  # 12
    count = 0  # 假设这个数能被0个数字整除
    for j in range(2, i):  # 2  3   4  6
        if i % j == 0:
            # 除尽了，是合数
            count += 1
    if count == 0:
        print(i, '是一个质数')
    else:
        print(i, '是一个合数，它能被', count, '个数字整除')
```
**9、9*9 乘法表**
```c
j = 0
while j < 9:
    j += 1
    i = 0
    while i < j:
        i += 1
        print(i, '*', j, '=', i * j, end='\t', sep='')
    print()

for i in range(1, 10):
    for j in range(1, i + 1):
        print(j, '*', i, '=', i * j, end='\t', sep='')
    print()

```
**10、****"****百马百担****"****问题：一匹大马能驮****3****担货，一匹中马能驮****2****担货，两匹小马能驮****1****担货，如果用一百匹马驮一百担货，问有大、中、小马各几匹？**

```c
# 假设大马有x,假设中马有y,小马  100 - x - y
for x in range(0, 100 // 3 + 1):
    for y in range(0, 100 // 2 + 1):
        if 3 * x + 2 * y + (100 - x - y) * 0.5 == 100:
            print(x, y, (100 - x - y))
```

**11、****一张纸的厚度大约是****0.08mm****，对折多少次之后能达到珠穆朗玛峰的高度（****8848.13****米）**
```c
height = 0.08 / 1000
count = 0
while True:
    height *= 2
    count += 1
    if height >= 8848.13:
        break

print(count)
```
**12、求斐波拉契的第N个数的值n为正整数**
```c
n =int(input("请输入第N个斐波拉契数列："))
num1 = 1
num2 = 1

for i in range(0,n -2):
    a =num1
    num1 =num2
    num2 = a +num2
    print(num2)
```
<a name="NBWOz"></a>
## 6、字符串的操作

<a name="2s2PD"></a>
### a、字符串的下标与切片
```c
# 下标我们又称之为索引，表示第几个数据。

# 可迭代对象: str  list  tuple  dict  set  range  都可以便利
# str list tuple 可以通过下标来获取或者操作数据

# 在计算机里，下标都是从 0 开始的。
word = 'zhangsan'  # 字符串:一个一个的字符串在一起
# 可以通过下标来获取或者修改指定位置的数据
print(word[4])

# 字符串是不可变的数据类型
# 对于字符串的任何操作，都不会改变原有的字符串!!!
# word[4] = 'x'

# 切片就是从字符串里复制一段指定的内容，生成一个新的字符串
m = 'abcdefghijklmnopqrstuvwxyz'
print(m[5])  # m[index] ==> 获取指定下标上的数据

# 切片语法  m[start:end:step]
# 包含start,不包含end
# step 指的是步长,理解为间隔。每隔 step-1 个取一次
# step 为负数，表示从右往左获取

print(m[2:9])  # 包含start,不包含end
print(m[2:])  # 如果只设置了start,会"截取"到最后
print(m[:9])  # 如果值设置了end,会从头开始"截取"

# 步长默认为 1
print(m[3:15:2])  # dfhjln
print(m[3:15:1])  # defghijklmno
# print(m[3:15:0])  # 步长不能为0
print('------------------')
# print(m[3:15:-1])  # 没有数据
print(m[15:3:-1])  # ponmlkjihgfe
print(m[::])  # abcdefghijklmnopqrstuvwxyz 从头到尾复制
print(m[::-1])  # zyxwvutsrqponmlkjihgfedcba

# start和end如果是负数，表示从右边数
print(m[-9:-5])  # rstu
```
<a name="avXKd"></a>
### b、字符串的的常见操作
```c
x = 'abcdefghijklmndsfasdfsadfadl'

# 使用内置函数 len 可以获取字符串的长度
print(len(x))

# 查找内容相关的方法  find/index/rfind/rindex  可以获取指定字符的下标
print(x.find('l'))  # 11
print(x.index('l'))  # 11

print(x.find('p'))  # -1 如果字符在字符串里不存在，结果是 -1
# print(x.index('p'))  # 使用index,如果字符不存在，会报错

print(x.find('l', 4, 9))

print(x.rfind('l'))
print(x.rindex('l'))

# startswith,endswith,isalpha,isdigit,isalnum,isspace
# is开头的是判断，结果是一个布尔类型
print('hello'.startswith('he'))  # True
print('hello'.endswith('o'))  # True
print('he45llo'.isalpha())  # False  alpha字母
print('good'.isdigit())  # False
print('123'.isdigit())  # True
print('3.14'.isdigit())  # False

# alnum 判断是否由数字和字母组成
print('ab12hello'.isalnum())  # True
print('hello'.isalnum())  # True
print('1234'.isalnum())  # True
print('4 - 1'.isalnum())  # False

print('h    o'.isspace())  # False

# num = input('请输入一个数字:')
# if num.isdigit():
#     num = int(num)
# else:
#     print('您输入的不是一个数字')


# replace方法:用来替换字符串
word = 'hello'
m = word.replace('l', 'x')  # replace 将字符串里 l 替换成 x
print(word)  # hello  字符串是不可变数据类型!!!
print(m)  # hexxo   原来的字符串不会改变，而是生成一个新的字符串来保存替换后的结果
```
<a name="XoGec"></a>
### c、大小写的问题
```sql
site = r"I'm a \teacher"		#在字符串中天剑r(R)将被表示为原生的字符串将原样的输出
print(site)

print('hello'.index('h'))

# 修改大小写

# capitalize 让第一个单词的首字母大写
print('hello world.good morning\nyes'.capitalize())

# upper 全大写
print('hello'.upper())  # HELLO

# lower 全小写
print('WoRLd'.lower())  # 面向对象里，我们称之为方法

# title 每个单词的首字母大写
print('good morning'.title())

# while True:
#     content = input('请输入内容，输入exit退出:')
#     print('您输入的内容是', content)
#     # if content.lower() == 'exit':
#     if content.upper() == 'EXIT':
#         break

# ljust(width,fillchar)
# width 长度   fillchar 填充字符，默认是空格
# 让字符串以指定长度显示，如果长度不够，默认在右边使用空格补齐
print('Monday'.ljust(10, '+'))
print('Tuesday'.rjust(12, '-'))
print('Wednesday'.center(20, '*'))

print('+++++apple+++'.lstrip('+'))
print('    pear     '.rstrip())
print('    banana     '.strip())

# 以某种固定格式显示的字符串，我们可以将它切割成为一个列表
x = 'zhangsan+lisi+wangwu+jack+tony+henry+chris'
names = x.split('+')
print(names)

# 将列表转换成为字符串
fruits = ['apple', 'pear', 'peach', 'banana', 'orange', 'grape']
print('-'.join(fruits))
print('*'.join('hello'))  # iterable可迭代对象
print('+'.join(('yes', 'ok')))

# print('*'.join({'name': 'zhangsan'}))

# 字符串的运算符
# 字符串和字符串之间可以使用加法运算，作用是拼接两个字符串
# 字符串和数字之间可以使用乘法运算，目的是将指定的字符串重复多次
# 字符串和数字之间做 == 运算结果是False,做 ！= 运算，结果是True
# 字符串之间做比较运算，会逐个比较字符串的编码值
# 不支持其他的运算符
```
<a name="Rprmo"></a>
# day3字典、列表、元组
```c
list = ['red', 'green', 'blue', 'yellow', 'white', 'black']		#[]定义的是列表
print( list[0] )												#red
print( list[1] )												#green
print( list[2] )												#yellow
print( list[0:2])    											#['red', 'green']
print( list[-1])												#black
print( list[-3:-2])												#yellow
nums = (9,4,3,1,6,7,7,4,2,2,2,1)								#定义一个元祖
print(nums.index(7))											#打印出第一个出现的数字7的索引
print(nums.count(2))											#统计出出现2的数字的次数

#如何表示元祖中只有一个元素的元祖
ages = (18)
print(type(ages))   #<class 'int'>	要是只有一个对象则在后面添加一个逗号
ages = (18,)
print(type(ages))   #<class 'tuple'>
print(tuple("hello"))	#('h', 'e', 'l', 'l', 'o')
    
#把转元祖转换为列表 list tuple set之间是可以相互转换的
words = ["hello","good","hi"]
print(tuple(words))			#('hello', 'good', 'hi')
    
name = ("I","am","jiang")
print(list(name))    		#['I', 'am', 'jiang']

height=("180","181","182")
print("".join(height))		#180181182

print("".join(height))
print("_".join(('I','a','m','j','w')))		#相当于是用_将字母给连接起来

#元组的遍历
name = ("I","am","jiang")
for i in name:
    print(i)
j =0
while j < len(name):
    print(name[j])
    j += 1
        
        
#字典
#字典是使用的是一对{}进行表示、多个键值对之间使用的是逗号分隔，key与value之间使用的是:进行分隔
persion={"name":"zhangsan",
         "age":"18",
         "chinese":"98",
         "English":"80",
         ("are","you"):"ok",    #因为元组是不可变的数据类型
         ["no","worse"]:'hi'    #列表是一个可变的数据类型所以会报错
         }
#注意字典的key唯一的要是重复后一个的key将覆盖前面一个,value可以是任意的数据类型
print(persion)

```
<a name="XK0fq"></a>
## 1、字典的增删改查
```c
#字典是不能通过索引的方式进行查找   
persion={"name":"zhangsan",
         "age":"18",
         "chinese":"98",
         "English":"80"
         }
print(persion['name'])		#zhangsan
print(persion['height'])	#KeyError: 'height'
print(persion.get("height"))	#如果没有找到对应的key则返回默认的值为none
print(persion.get('gender', 'male'))	#修改默认值为male，要是在字典中没有找到对应的值则将gender取得值为male
```
<a name="CPn5z"></a>
##  2、修改和添加数据
```c
persion={"name":"zhangsan",
         "age":"18",
         "chinese":"98",
         "English":"80"
         }
#直接使用key可以修改对应的value
persion['name']='lisi'				#lisi

#要是对应的key不存在则会添加
persion['gender']='female'			#female
print(persion)
```
<a name="erEuy"></a>
## 3、字典的删除
```c
persion={"name":"zhangsan",
         "age":"18",
         "chinese":"98",
         "English":"80"
}
persion.pop('name')     #将字典中的键name删除
print(persion)          #{'age': '18', 'chinese': '98', 'English': '80'}
persion.popitem()       #删除一个键值对
print(persion)          #{'age': '18', 'chinese': '98'}
persion.clear()         #清空字典
print(persion)          #{}

```




































