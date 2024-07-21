Python 字符串
<a name="t6uBF"></a>
## 一、%s%d%f方法
简单而言在Python2.5版本之前，使用的都是最原始的格式化方法`%s`，而这种方法也是目前为止最常用的方法，简单又方便。来看一下他的具体用法：
<a name="uzEYy"></a>
### 1.标志性符号
当看到输出语句存在 **%d，%s，%f等百分号的时候** ，这便是第一种%方法。Python 支持字符串格式化输出，尽管这样可能遇到非常复杂的表达式，但最基本的是用法就是**将一个值插入到一个有字符串格式符%s的字符串中**。<br />**例1：** 输入：
```python
name = '是Dream呀'
print('Hello，大家好，我叫%s'%name)
```
输出：
```python
Hello，大家好，我叫是Dream呀
```
这里大家要注意，字符串和变量中间要用`%`连接，不可以用`','`进行连接`'Hello，大家好，我叫%s',name`，如果这样连接就直接相当于把变量在字符串之后进行输出。<br />**例2：** 输入：
```python
name = '是Dream呀'
print('Hello，大家好，我叫%s',name)
```
输出：
```python
Hello，大家好，我叫%s 是Dream呀
```
<a name="U7fpx"></a>
### 2.输出数据类型
在程序中，看到`%s`这样的操作符，这就是Python中字符串格式化的符号。除此之外，还可以用`%`符号对其他类型的数据进行格式化。常见的格式化符号如下：

- `%s` 通过`str()`字符串转换来格式化- `%f` 浮点实数- `%d` 有符号的十进制整数
<a name="YhgHJ"></a>
### 3.%10s %-10s %.10s用法
<a name="WbG3Q"></a>
#### （1）`%10s`
`%10s`代表输出宽度为10个字符，如果输出的字符串不超过10，则在左边补空格；宽度超过10，按原长度输出。<br />**例3：** 输入：
```python
string = '12345'
print("123456789ABCDEFGHI")
print("%10s" %string) 
```
输出：
```python
123456789ABCDEFGHI
     12345
```
<a name="J3pFC"></a>
#### （2）`%-10s`
和上面的`%10s`相反，如果输出的字符串宽度不超过10，则在右边补齐空格；宽度超过10，按原长度输出。<br />**例4：** 输入：
```python
string = '12345'
print("123456789ABCDEFGHI")
print("%-10s" %string + "aaaaa")  # 长度未超过10，右边补齐
print("%-3s" %string + "aaaaa")  # 长度超过3，原样输出
```
输出：
```python
123456789ABCDEFGHI
12345     aaaaa
12345aaaaa
```
<a name="gq7Dh"></a>
#### （3）`%.10s`
`%.10s`表示截取前10个字符，若原长度不够10，则原样输出。<br />**例5：** 输入：
```python
string = '12345'
print("123456789ABCDEFGHI")
print("%.3s" %string) #原长度超过3，截取前3个字符
print("%.10s" %string)#原长度不够10，原样输出
```
```python
123456789ABCDEFGHI
123
12345
```
<a name="SKkxj"></a>
#### （4）`%10.3s`
这种格式化字符串要分成两部分来看，先运行右边的".3"部分，即先截取3个字符；再运行左边的"10"部分，即上面“1”中提到的功能。具体见下面的例子：**例6：** 输入：
```python
string = '12345'
print("123456789ABCDEFGHI")
print("%10.3s" %string)      # 先截取3个字符("123")，由于长度不足10，左边补7个空格
print("%10.7s" %string)      # 先截取7个字符("12345")，由于长度不足10，左边补5个空格
print("%2.3s" %string)       # 先截取3个字符("123")，由于长度超过2，原样输出
```
```python
123456789ABCDEFGHI
       123
     12345
123
```
<a name="eMHVh"></a>
### 4.`%d` `%2d` `%02d` `%-2d%` `%.2d`的区别
`**%d**`就是普通的输出了整形数<br />`**%2d**`是将数字按宽度为2，采用右对齐方式输出，若数据位数不到2位，则左边补空格。`**%02d**`，和`%2d`差不多，只不过左边补0<br />`**%-2d**`将数字按宽度为2，采用左对齐方式输出，若数据位数不到2位，则右边补空格 `**%.2d**` 输出整形时最少输出2位，如不够前面以0占位。如输出2时变成02，200时只输出200；输出浮点型时（`%.2f`）小数点后强制2位输出<br /> **例7：** 输入：
```python
num = 1
print("%d" % (num))
print("%2d" % (num))
print("%02d" % (num))
print("%-2d" % (num))
print("%.2d" % (num))
print("%.2d" % (200))
```
```python
1
 1
01
1 
01
200
```
<a name="LIfWn"></a>
### 5.`%f`详解
<a name="lycty"></a>
#### （1）`%f`
`%f`时表示原值，默认是小数点后5位数 <br />**例8：** 输入：
```python
import math
print('%f'%math.pi)
```
```python
3.141593
```
<a name="wkpeM"></a>
#### （2）`%9f`
只是`%9f`时，表示打印长度9位数，小数点也占一位，不够左侧补空格 <br />**例9：** 输入：
```python
import math
print('%f'%math.pi)
print('%9f'%math.pi)
```
```python
3.141593
 3.141593
```
<a name="X0MNW"></a>
#### （3）`03.f`
只有`.`没有后面的数字时，表示去掉小数输出整数，03表示不够3位数左侧补0 <br />**例10：** 输入：
```python
import math
print('%f'%math.pi)
print('%.f'%math.pi)
print('%03.f'%math.pi)
```
```python
3.141593
3
003
```
<a name="WlmmY"></a>
#### （4）`%6.3f`
`%6.3f`表示小数点后面精确到3位，总长度6位数，包括小数点，不够左侧补空格 <br />**例11：** 输入：
```python
import math
print('%f'%math.pi)
print('%6.3f'%math.pi)
```
```python
3.141593
 3.142
```
<a name="yBkSm"></a>
#### （5）`%-6.3f`
`%-6.3f`表示小数点后面精确到3位，总长度6位数，包括小数点，不够右侧补空格 <br />**例12：** 输入：
```python
import math
print('%f'%math.pi)
print('%-6.3f'%math.pi)
```
```python
3.141593
3.142     
```
<a name="kpcRJ"></a>
## 二、`format()`方法
从Python3.0版本开始起(Python2.6同期发布)，Python中同时支持两个版本的格式化，多出来的一个新版本就是利用`format()`函数，进行格式化输出。
<a name="HwUeM"></a>
### 1.无参（1）
**例13：** 输入：
```python
print('{} {}'.format('hello','world'))
```
```python
hello world
```
<a name="KaJEE"></a>
### 2.无参（2）
**例14：** 输入：
```python
print('{0} {1}'.format('hello','world'))
```
```python
world hello
```
<a name="RkM0w"></a>
### 3.无参（3）
**例15：** 输入：
```python
print('{1} {0} {1}'.format('hello','world'))
```
```python
world hello world
```
<a name="KiCai"></a>
### 4.key value
**例16：** 输入：
```python
print('ID:{id},Name:{name}'.format(id='001',name='hello'))
```
```python
ID:001,Name:hello
```
<a name="u1eFT"></a>
### 5.列表
**例17：** 输入：
```python
list=['001','hello']
print('ID:{List[0]},Name:{List[1]}'.format(List = list))
print('ID:{0[0]},Name:{0[1]}'.format(list))
```
```python
ID:001,Name:hello
ID:001,Name:hello
```
<a name="fdzKg"></a>
### 6.字典
**例18：** 输入：
```python
dict={<!-- -->'id':'001,'name':'hello'}
print('ID:{Dict[0]},Name:{Dict[1]}'.format(Dict = dict))
print('ID:{id},Name:{name}'.format(**dict))
```
```python
ID:001,Name:hello
ID:001,Name:hello
```
<a name="rBzXE"></a>
### 7.类
**例19：** 输入：
```python
class value():
    id = '001'
    name = 'hello'
print('ID:{Value.id},Name{Value.name}'.format(Value = value))

```
```python
ID:001,Name:hello
```
<a name="Tds3u"></a>
### 8.数字格式化

- 3.1415926 `**{:.2f}**` 3.14 保留小数点后两位
- 3.1415926 `**{:+.2f}**` +3.14 带符号保留小数点后两位
- -1 `**{:+.2f}**` -1.00 带符号保留小数点后两位
<a name="HKMbj"></a>
### 9.格式总结
第一种方式：固定形式的格式化填充，给的顺序和填充顺序完全一致。<br />**例20：** 输入：
```python
b1 = "四大天王：{}、{}、{}、{}".format("是Dream呀","郭富城","张学友","黎明")
print(b1)
```
```python
四大天王：是Dream呀、郭富城、张学友、黎明
```
第二种方式：大括号`{ }`中写的是`format()`传入值所对应的下标。<br />**例21：** 输入：
```python
b2 = "四大天王：{2}、{0}、{3}、{1}".format("刘德华","郭富城","张学友","黎明")
print(b2)
```
第三种方式：大括号`{ }`中的变量，和`format()`传入值是一一对应的。<br />**例22：** 输入：
```python
x1 = "刘德华"
x2 = "郭富城"
x3 = "张学友"
x4 = "黎明"
b3 = "四大天王：{p}、{q}、{r}、{s}".format(p=x4,q=x2,r=x1,s=x3)
print(b3)
```
<a name="FzRkH"></a>
## 三、f-string方法
Python3.6版本后，又引入了一种新的字符串格式化方式f-string。从`%s`格式化到`format`格式化再到f-string格式化，格式化的方式越来越直观，f-string的效率也较前两个高一些，使用起来也比前两个更简单一些。**f-string格式化：占位符**`**{}**`**，搭配f符号一起使用**。
<a name="PDV1L"></a>
### 简单使用
f-string用大括号 `{}` 表示被替换字段，其中直接填入替换内容：<br />**例23：** 输入：
```python
name = '是Dream呀'
print('Hello, my name is {name}'.format(name=name))
print(f'Hello, my name is {<!-- -->name}')

Hello, my name is 是Dream呀
Hello, my name is 是Dream呀
```
**例24：** 输入：
```python
import datetime
name = '是Dream呀'
age = 20
date = datetime.date(2022,2,5)
print(f'my name is {<!-- -->name}, this year is {<!-- -->date:%Y},Next year, I\'m {<!-- -->age+1}')
# my name is 是Dream呀, this year is 2022,Next year, I'm 21
```
其余的方法也都和`format()`函数方法一致，大家自行参考即可。
<a name="jPcta"></a>
## 总结
Python最先的格式化字符串方法是`%`，但他的致命缺点是支持的类型有限。`format()`比较全面，而`format()`中有的f-string基本都有，而且更简单，所以说**一般来说用f-string，除非特殊情况下**`**format()**`**。**
