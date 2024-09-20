Python
<a name="uE5FW"></a>
### `%`占位符的使用
先来看一下下面的这个例子，
```python
country_ = "France"
currency_ = "Euro"

print("%s is the currency of %s" % (currency_, country_))
```
output
```python
Euro is the currency of France
```
当然除了`%s`之外，代表的是字符串，还有`%f`代表的是浮点数以及`%d`代表的是整数，来看下面的这个例子，
```python
name = '欣一'
age = 24
height = 1.88

print('我是：%s, 年龄：%d, 身高：%fm' % (name,age,height))
```
output
```python
我是：欣一, 年龄：24, 身高：1.880000m
```
可以依据保留小数点后面固定的位数，默认的精度为6位，即是小数点后6位，当然也可以保留例如2位小数，代码如下
```python
print('我是：%s, 年龄：%d, 身高：%.2fm' % (name,age,height))
```
output
```python
我是：欣一, 年龄：24, 身高：1.88m
```
当然有时候指定了字符串的最终长度，但是现有的字符串远远没有那么长，因为就需要添加空格的方式来填充，可以填充在左边当然也可以填充在右边，代码如下
```python
## 左填充
display('%10s' % ('欣一'))

## 右填充
display('%-10s' % ('欣一'))
```
output
```python
'        欣一'
'欣一        '
```
<a name="iZjDd"></a>
### f-string格式化
将上面的案例稍稍做一个修改，代码如下
```python
country_ = "France"
currency_ = "Euro"

print(f"{currency_} is the currency of {country_}")
```
output
```python
Euro is the currency of France
```
使用f-string格式化的方法好就好在可以直接使用变量名来填充句子当中的内容，当然也可以拿它直接来进行数值运算，代码如下
```python
print(f'计算结果是：{2*10 + 3*15}')
```
output
```python
65
```
或者直接和Python当中的内置函数以及`lambda`方法联用，代码如下
```python
string_test = 'Python is awesome'
print(f'我想说 {string_test.lower()}')
```
output
```python
我想说 python is awesome
```
以及
```python
a = 100
b = 10
print(f'计算的结果是：{(lambda x,y:x+y)(a,b)}')
```
output
```python
计算的结果是：110
```
<a name="m0KnC"></a>
### `format`关键字
`format`关键字来格式化输出字符串有多种方式。
<a name="iXOdV"></a>
#### 不指定位置
看一下下面的这个例子，代码如下
```python
print('我是：{}, 年龄：{}, 身高：{}m'.format(name,age,height))
```
output
```python
我是：欣一, 年龄：24, 身高：1.88m
```
位置默认开始从0计算，然后对应位置填入数据
<a name="wzSzC"></a>
#### 指定位置
也可以是指定位置的填入数据，代码如下
```python
print('我是：{0}, 年龄：{1}, 身高：{2}m'.format(name,age,height))
print('我是：{0}, 年龄：{1}, 身高：{1}m'.format(name,age,height))
print('我是：{0}, 年龄：{2}, 身高：{1}m'.format(name,age,height))
```
output
```python
我是：欣一, 年龄：24, 身高：1.88m
我是：欣一, 年龄：24, 身高：24m
我是：欣一, 年龄：1.88, 身高：24m
```
<a name="UShwM"></a>
#### 关键字配对
也可以按照关键字配对的方式来进行内容的填充，代码如下
```python
print('我是：{name}, 年龄：{age}, 身高：{height}m'.format(name='欣一', age=25, height=1.88))
```
output
```python
我是：欣一, 年龄：25, 身高：1.88m
```
<a name="KgZvl"></a>
### 字典参数
当数据是以字典的形式来展现的时候，就可以这么来做
```python
dic = {'name':'欣一','age':24,'height':1.88}
print('我是：{name}, 年龄：{age}, 身高：{height}m'.format(**dic))
```
output
```python
我是：欣一, 年龄：24, 身高：1.88m
```
<a name="QePPk"></a>
### 列表参数
同样的，当数据是以列表的形式来展现的时候，就可以这么来做
```python
foods = ['fish', 'beef', 'fruit']
s = 'i like eat {} and {} and {}'.format(*foods)
print(s)
```
output
```python
i like eat fish and beef and fruit
```
或者是有根据位置来进行填充，代码如下
```python
foods = ['fish', 'beef', 'fruit']
s = 'i like eat {2} and {0} and {1}'.format(*foods)
print(s)
```
output
```python
i like eat fruit and fish and beef
```
<a name="xAE3A"></a>
### 精度
有时候碰到需要对小数点后面的数字保留几位小数，代码上可以这么来操作
```python
pi = 3.1415926
print("{:.2f}".format(pi)) # 保留两位小数
print("{:+.3f}".format(pi)) # 带符号保留3位小数
print("{:.2%}".format(pi)) # 百分比保留两位小数
```
output
```python
3.14
+3.142
314.16%
```
<a name="YReH5"></a>
### 千分位分隔符
主要是用于货币数据的格式化输出，例如是将“100000”变化成“100,000”，代码如下
```python
print('{:,}'.format(100000000))
```
output
```python
100,000,000
```
有时候还需要在前面添加上货币符号，代码如下
```python
print('${:,}'.format(100000000))
```
output
```python
$100,000,000
```
