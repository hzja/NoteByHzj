# 数字(Number) 1.0

~~~ javascript
# 加
print(3+8)
11

# 减
print(3-8)
-5

# 乘
print(3*8)
24

# 除
print(3/8)
0.375

# 取整除 - 返回商的整数部分（向下取整）
print(17//3)
5
      
# 取模 - 返回除法的余数
print(17%3)
2

# 幂，0.5次方就是开方
print(8**0.5)
2.8284271247461903

# 向下取整数
print(int(0.5))
0

# 求绝对值
print(abs(-1))
1

# 将一个整数转换为一个十六进制字符串
print(hex(13))
0xd

# 将一个整数转换为一个八进制字符串
print(oct(13))
0o15

# 将一个整数转换为一个二进制字符串
print(bin(13))
0b1101

######### 位运算 #########
print(bin(13))
001101
print(bin(60))
111100

# 与，如果两个相应位都为1，则该位的结果为1，否则为0，结果为001100
print(13&60)
12

# 或，只要两个相应位有一个为1，结果位就为1，结果111101
print(13|60)
61

# 异或，当同位相同时结果为0，不同结果为1，结果为110010
print(13^60)
49
# 作用：奇偶判断，a=4，他的二进制为1010，而1^0^1^0=0，因为结果为0，所以a是偶数
~~~

## <b>进制转换</b>

| 由右边的转下边的 | 2进制         | 8进制         | 10进制         | 16进制         |
| ---------------- | ------------- | ------------- | -------------- | -------------- |
| 2进制            | -             | bin(int(n,8)) | bin(int(n,10)) | bin(int(n,16)) |
| 8进制            | oct(int(n,2)) | -             | oct(int(n,10)) | oct(int(n,16)) |
| 10进制           | int(n,2)      | int(n,8)      | -              | int(n,16)      |
| 16进制           | hex(int(n,2)) | hex(int(n,8)) | hex(int(n,10)) | -              |

**原理：**Python内置函数进行进制转换的原理是将读入的一个字符串(Python默认读入的都是字符串形式)转为10进制，然后再用相关函数进行其他进制转换。

+ 在线转换： [https://tool.lu/hexconvert](https://tool.lu/hexconvert/)



# 数字(Number) 2.0

Python 数字数据类型用于存储数值。

数据类型是不允许改变的,这就意味着如果改变数字数据类型的值，将重新分配内存空间。



## 创建Number对象

以下实例在变量赋值时 Number 对象将被创建：

```python
var1 = 1
var2 = 10
```



## 删除Number对象

可使用del语句删除一些数字对象引用

del语句语法是

```
del var1[,var2[,var3[....,varN]]]
```

可使用del语句删除单个或多个对象引用如：

```python
del var
del var_a, var_b
```



## python支持的数值类型

Python 支持三种不同的数值类型：

- **整型(int)** - 通常被称为是整型或整数，是正或负整数，不带小数点。Python3 整型是没有限制大小的，可以当作 Long 类型使用，所以 Python3 没有 Python2 的 Long 类型。布尔(bool)是整型的子类型。
- **浮点型(float)** - 浮点型由整数部分与小数部分组成，浮点型也可以使用科学计数法表示（2.5e2 = 2.5 x 102 = 250）
- **复数( (complex))** - 复数由实数部分和虚数部分构成，可以用a + bj,或者complex(a,b)表示， 复数的实部a和虚部b都是浮点型。



使用十六进制和八进制来代表整数：

```python
>>> number = 0xA0F # 十六进制
>>> number
2575

>>> number=0o37 # 八进制
>>> number
31
```



| int    | float      | complex    |
| :----- | :--------- | :--------- |
| 10     | 0.0        | 3.14j      |
| 100    | 15.20      | 45.j       |
| -786   | -21.9      | 9.322e-36j |
| 080    | 32.3e+18   | .876j      |
| -0490  | -90.       | -.6545+0J  |
| -0x260 | -32.54e100 | 3e+26J     |
| 0x69   | 70.2E-12   | 4.53e-7j   |

- Python支持复数，复数由实数部分和虚数部分构成，可以用a + bj,或者complex(a,b)表示， 复数的实部a和虚部b都是浮点型。



## Python 数字类型转换

有时候需对数据内置类型转换，数据类型转换只需将数据类型作函数名即可。

- **int(x)** 将x转换为一个整数。
- **float(x)** 将x转换到一个浮点数。
- **complex(x)** 将x转换到一个复数，实数部分为 x，虚数部分为 0。
- **complex(x, y)** 将 x 和 y 转换到一个复数，实数部分为 x，虚数部分为 y。x 和 y 是数字表达式。

以下实例将浮点数变量 a 转换为整数：

```python
>>> a = 1.0
>>> int(a)
1
```



## Python 数字运算

Python 解释器可作一简单计算器，可在解释器里输入一表达式其将输出表达式值；

表达式语法很直白： **+**, **-**, ***** ,**/**和其它语言（如Pascal或C）里一样，如：

```python
>>> 2 + 2
4
>>> 50 - 5*6
20
>>> (50 - 5*6) / 4
5.0
>>> 8 / 5  # 总是返回一个浮点数
1.6
```

**注意：**在不同的机器上浮点运算的结果可能会不一样。



整数除法中**/** 总返回一浮点数，如果只想得整数结果丢弃可能分数部可使用运算符 **//** ：

```python
>>> 17 / 3  # 整数除法返回浮点型
5.666666666666667
>>>
>>> 17 // 3  # 整数除法返回向下取整后的结果
5
>>> 17 % 3  # ％操作符返回除法的余数
2
>>> 5 * 3 + 2 
17
```



**注意：** **//** 得到不一定是整数类型的数其与分母分子数据类型有关

```python
>>> 7//2
3
>>> 7.0//2
3.0
>>> 7//2.0
3.0
>>> 
```



等号 **=** 用于给变量赋值，赋值后除下一提示符解释器不显示任何结果

```python
>>> width = 20
>>> height = 5*9
>>> width * height
900
```



Python 可以使用 **\*\*** 操作来进行幂运算：

```python
>>> 5 ** 2  # 5 的平方
25
>>> 2 ** 7  # 2的7次方
128
```



变量使用前须"定义"（即赋予变量一个值）否则出现错误：

```
>>> n   # 尝试访问一个未定义的变量
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'n' is not defined
```



不同类型混合运算时将整数转为浮点数：

```
>>> 3 * 3.75 / 1.5
7.5
>>> 7.0 / 2
3.5
```



与解释器的交互模式中最后输出表达式结果赋值给变量 **_** ，如：

```python
>>> tax = 12.5 / 100
>>> price = 100.50
>>> price * tax ##这里的输出结果被赋值给变量_
12.5625
>>> price + _
113.0625
>>> round(_, 2) ##round函数是指保留多少位小数
113.06
```

此处 **_ ** 变量被用户视为只读变量



## 数学函数

| 函数                                                         | 返回值 ( 描述 )                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [abs(x)](https://www.runoob.com/python3/python3-func-number-abs.html) | 返回数字的绝对值，如abs(-10) 返回 10                         |
| [ceil(x)](https://www.runoob.com/python3/python3-func-number-ceil.html) | 返回数字的上入整数，如math.ceil(4.1) 返回 5                  |
| cmp(x, y)                                                    | 如果 x < y 返回 -1, 如果 x == y 返回 0, 如果 x > y 返回 1。 **Python 3 已废弃，使用 (x>y)-(x<y) 替换**。 |
| [exp(x)](https://www.runoob.com/python3/python3-func-number-exp.html) | 返回e的x次幂(ex),如math.exp(1) 返回2.718281828459045         |
| [fabs(x)](https://www.runoob.com/python3/python3-func-number-fabs.html) | 以浮点数形式返回数字的绝对值，如math.fabs(-10) 返回10.0      |
| [floor(x)](https://www.runoob.com/python3/python3-func-number-floor.html) | 返回数字的下舍整数，如math.floor(4.9)返回 4                  |
| [log(x)](https://www.runoob.com/python3/python3-func-number-log.html) | 如math.log(math.e)返回1.0,math.log(100,10)返回2.0            |
| [log10(x)](https://www.runoob.com/python3/python3-func-number-log10.html) | 返回以10为基数的x的对数，如math.log10(100)返回 2.0           |
| [max(x1, x2,...)](https://www.runoob.com/python3/python3-func-number-max.html) | 返回给定参数的最大值，参数可以为序列。                       |
| [min(x1, x2,...)](https://www.runoob.com/python3/python3-func-number-min.html) | 返回给定参数的最小值，参数可以为序列。                       |
| [modf(x)](https://www.runoob.com/python3/python3-func-number-modf.html) | 返回x的整数部分与小数部分，两部分的数值符号与x相同，整数部分以浮点型表示。 |
| [pow(x, y)](https://www.runoob.com/python3/python3-func-number-pow.html) | x**y 运算后的值。                                            |
| [round(x \[,n\])](https://www.runoob.com/python3/python3-func-number-round.html) | 返回浮点数 x 的四舍五入值，如给出 n 值，则代表舍入到小数点后的位数。**其实准确的说是保留值将保留到离上一位更近的一端。** |
| [sqrt(x)](https://www.runoob.com/python3/python3-func-number-sqrt.html) | 返回数字x的平方根。                                          |



## 随机数函数

随机数可以用于数学，游戏，安全等领域中，还经常被嵌入到算法中，用以提高算法效率，并提高程序的安全性。

Python包含以下常用随机数函数：

| 函数                                                         | 描述                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [choice(seq)](https://www.runoob.com/python3/python3-func-number-choice.html) | 从序列的元素中随机挑选一个元素，比如random.choice(range(10))，从0到9中随机挑选一个整数。 |
| [randrange ([start,\] stop [,step])](https://www.runoob.com/python3/python3-func-number-randrange.html) | 从指定范围内，按指定基数递增的集合中获取一个随机数，基数默认值为 1 |
| [random()](https://www.runoob.com/python3/python3-func-number-random.html) | 随机生成下一个实数，它在[0,1)范围内。                        |
| [seed([x\])](https://www.runoob.com/python3/python3-func-number-seed.html) | 改变随机数生成器的种子seed。如果你不了解其原理，你不必特别去设定seed，Python会帮你选择seed。 |
| [shuffle(lst)](https://www.runoob.com/python3/python3-func-number-shuffle.html) | 将序列的所有元素随机排序                                     |
| [uniform(x, y)](https://www.runoob.com/python3/python3-func-number-uniform.html) | 随机生成下一个实数，它在[x,y]范围内。                        |



## 三角函数

Python包括以下三角函数：

| 函数                                                         | 描述                                              |
| :----------------------------------------------------------- | :------------------------------------------------ |
| [acos(x)](https://www.runoob.com/python3/python3-func-number-acos.html) | 返回x的反余弦弧度值。                             |
| [asin(x)](https://www.runoob.com/python3/python3-func-number-asin.html) | 返回x的反正弦弧度值。                             |
| [atan(x)](https://www.runoob.com/python3/python3-func-number-atan.html) | 返回x的反正切弧度值。                             |
| [atan2(y, x)](https://www.runoob.com/python3/python3-func-number-atan2.html) | 返回给定的 X 及 Y 坐标值的反正切值。              |
| [cos(x)](https://www.runoob.com/python3/python3-func-number-cos.html) | 返回x的弧度的余弦值。                             |
| [hypot(x, y)](https://www.runoob.com/python3/python3-func-number-hypot.html) | 返回欧几里德范数 sqrt(x*x + y*y)。                |
| [sin(x)](https://www.runoob.com/python3/python3-func-number-sin.html) | 返回的x弧度的正弦值。                             |
| [tan(x)](https://www.runoob.com/python3/python3-func-number-tan.html) | 返回x弧度的正切值。                               |
| [degrees(x)](https://www.runoob.com/python3/python3-func-number-degrees.html) | 将弧度转换为角度,如degrees(math.pi/2) ， 返回90.0 |
| [radians(x)](https://www.runoob.com/python3/python3-func-number-radians.html) | 将角度转换为弧度                                  |



## 数学常量

| 常量 | 描述                                  |
| :--- | :------------------------------------ |
| pi   | 数学常量 pi（圆周率，一般以π来表示）  |
| e    | 数学常量 e，e即自然常数（自然常数）。 |
