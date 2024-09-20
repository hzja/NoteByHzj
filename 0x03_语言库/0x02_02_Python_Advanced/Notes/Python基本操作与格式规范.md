Python 命名规范 注释 格式化
<a name="LL5t3"></a>
### 命名规范
变量和标识符，变量和标识符在命名规则上都遵循以下原则，

- 只能由字母、数字和下划线组成，且不能以数字开头
- 命名要做到见名知意：一般来说，只要是自定义的名字都可以被称为标识符，包括变量、函数名、类名、模块名、包名，所以变量其实只是标识符的一个子类。

变量常见的命名风格有三种：

- 单词全部纯小写，单词之间使用下划线隔开，例如：data_science
- 小驼峰式命名，第一个单词字母全小写，其他单词首字母大写，例如：dataScience
- 大驼峰式命名，每个单词的首字母均大写，例如：DataScience
- 以上这些命名规范不仅仅适用于变量，同时也适用于函数、类等；
- 在Python中，方法和变量名一般采用第一种方式，类名采用第三种方式，第二种方式在Java声明函数时比较常见。
- 切记，标识符的名字不能和Python的关键字冲突(可以使用`keyword.kwlist`查看Python所有的关键字)
<a name="ee656aa1"></a>
### 注释
<a name="i8wRG"></a>
#### Python2-3输出区别
> Python3 使用 print 必须要以小括号包裹打印内容，比如 `print('hi')`
> Python2 既可以使用带小括号的方式，也可以使用一个空格来分隔打印内容，比如 `print 'hi'`
> **现在都是以Python3为主**

<a name="WUfON"></a>
#### 单行注释
Python中**单行注释**以 _#_ 开头，实例如下：
```python
#第一行代码
print('hello world') #第二个注释
hello world
```
<a name="ZanP9"></a>
#### 多行注释
**多行注释**可以用多个 _#_ 号，还有 _'''_ 和 _"""_：
```python
'''
我真的
可以
注释
很多行
'''
"""
我也能
注释
很多行
"""
print('hello world')
hello world
```
<a name="kqSEw"></a>
### 格式化输出
**拼接符 '+'**
```python
print('变量名：'+变量)
```
注意：变量必须为字符串类型<br />**拼接符','**
```python
print('变量名：'，变量)
```
<a name="SMnb9"></a>
#### 格式化符号
整数输出：<br />%o —— oct 八进制<br />%d —— dec 十进制<br />%x —— hex 十六进制
```python
print('%o' % 20)   #以八进制输出20
print('%d' % 20)   #以十进制输出20
print('%x' % 20)   #以十六进制输出20
 
24
20
14
```

- 浮点数输出<br />%f ——保留小数点后面六位有效数字<br />　　%.3f，保留3位小数位<br />%e ——保留小数点后面六位有效数字，指数形式输出<br />　　%.3e，保留3位小数位，使用科学计数法<br />%g ——在保证六位有效数字的前提下，使用小数方式，否则使用科学计数法<br />　　%.3g，保留3位有效数字，使用小数或科学计数法

下面做一系列的演示：
```python
print('%f' % 1.11)  # 默认保留6位小数
print('%.1f' % 1.11)  # 取1位小数
print('%e' % 1.11)  # 默认6位小数，用科学计数法
print('%.3e' % 1.11)  # 取3位小数，用科学计数法
print('%g' % 1111.1111)  # 默认6位有效数字
print('%.7g' % 1111.1111)  # 取7位有效数字
print('%.2g' % 1111.1111)  # 取2位有效数字，自动转换为科学计数法
1.110000
1.1
1.110000e+00
1.110e+00
1111.11
1111.111
1.1e+03
```
<a name="QJgh6"></a>
#### format的用法
format的用法很常见，并且实用，大家可以根据案例多进行模仿与演示。<br />format就是变量之间的映射，它有三种形式

- （1）不带编号，即“{}”
- （2）带数字编号，可调换顺序，即“{1}”、“{2}”
- （3）带关键字，即“{a}”、“{tom}”

映射案例如下：
```python
print('{} {}'.format('hello','world'))  # 不带字段
print('{0} {1}'.format('hello','world'))  # 带数字编号
print('{0} {1} {0}'.format('hello','world'))  # 打乱顺序
print('{1} {1} {0}'.format('hello','world'))
print('{a} {tom} {a}'.format(tom='hello',a='world'))  # 带关键字
 
hello world
hello world
hello world hello
world world hello
world hello world
```
<a name="0aDul"></a>
#### 格式化字符串

- f-string，亦称为格式化字符串常量（formatted string literals），是Python3.6新引入的一种字符串格式化方法。
- f-string在形式上是以 f 或 F 修饰符引领的字符串（f'xxx' 或 F'xxx'），以大括号 {} 标明被替换的字段；
- f-string在本质上并不是字符串常量，而是一个在运行时运算求值的表达式。

先尝试一下str、int类型的变量
```python
name = "小风"
age = 18
print(f"姓名：{name}，年龄：{age}")
姓名：小风，年龄：18
```
再试一下字典
```python
one_dict = {"name": "小风", "age": 18, "hobby": ["running", "singing"]}
print(f"姓名：{one_dict['name']},爱好：{one_dict['hobby']}")

姓名：小风,爱好：['running', 'singing']
```
<a name="tc5RK"></a>
#### 格式规范
代码块快捷键:CTRL+ALT+L
<a name="WayIy"></a>
### 转义字符
转义字符 '\' 可以转义很多字符

- '\n' 表示换行
- '\t' 表示制表符
- 字符 '\' 本身也要转义，所以 '\' 表示的字符就是 '\'

下面为 '\' 在输出过程中可能出现的情况;
```python
print('I\'m ok.')
I'm ok.
print('I\'m learning\nPython.')
I'm learning
Python.
print('\\\n\\')
```

