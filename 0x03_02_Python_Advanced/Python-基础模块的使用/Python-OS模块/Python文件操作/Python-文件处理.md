Python 文件处理
<a name="dzv84"></a>
## 1、打开文件
<a name="fZuuP"></a>
### `open()` 方法
Python `open()` 方法用于打开一个文件，并返回文件对象，在对文件进行处理过程都需要使用到这个函数，如果该文件无法被打开，会抛出 OSError。
:::tips
注意：使用 `open()` 方法一定要保证关闭文件对象，即调用 `close()` 方法。
:::
`open()` 函数常用形式是接收两个参数：**文件名(file)和模式(mode)**<br />完整的语法格式如下：
```python
open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)
```
```python
f1 = open(r'd:\测试文件.txt', mode='r', encoding='utf-8')
content = f1.read()
print(content)
f1.close()
```
```python
with open(r'd:\测试文件.txt', mode='r', encoding='utf-8') as f1:
    content = f1.read()
    print(content)
```

1. `open()`内置函数，open底层调用的是操作系统的接口。
2. f1变量，又叫文件句柄，通常文件句柄命名有f1，fh，file_handler，f_h，对文件进行的任何操作，都得通过文件句柄.方法的形式。
3. `encoding`：可以不写。不写参数，默认的编码本是操作系统默认的编码本。windows默认gbk，linux默认utf-8，mac默认utf-8。
4. `mode`：可以不写。默认`mode='r'`。
5. `f1.close()`关闭文件句柄。
6. 另外使用`with open()`的好处：
```python
#优点1：不用手动关闭文件句柄。
with open('C:\\Users\\Fcant\\test.txt', encoding='utf-8') as f1:
    print(f1.read())
    
#优点2：一个语句可以操作多个文件句柄。
with open('C:\\Users\\Fcant\\test.txt', encoding='utf-8') as f1, \
        open('文件操作的写', encoding='utf-8', mode='w') as f2:
    print(f1.read())
    f2.write('hahaha')
```
<a name="1b076efb"></a>
#### 绝对路径和相对路径

1. 绝对路径：指的是绝对位置，完整地描述了目标的所在地，所有目录层级关系是一目了然的。比如C:/Users/Python37/python.exe
2. 相对路径：是从当前文件所在的文件夹开始的路径。
   - 2.1 test.txt：是在当前文件夹查找 test.txt 文件。
   - 2.2 ./test.txt：也是在当前文件夹里查找test.txt文件， ./ 表示的是当前文件夹，可以省略。
   - 2.3 ../test.txt：从当前文件夹的上一级文件夹里查找 test.txt 文件。../ 表示的是上一级文件夹。
   - 2.4 demo/test.txt，在当前文件夹里查找demo这个文件夹，并在这个文件夹里查找 test.txt文件。
3. 路径书写的三种方法
- 3.1：\\
   - `file = open('C:\\Users\\Python基础\\xxx.txt')`
- 3.2：r'\'
   - `file = open(r'C:\Users\Python基础\xxx.txt')`
- 3.3：'/'(推荐)
   - `file = open('C:/Users/Python基础/xxx.txt')`
<a name="dWYlr"></a>
#### 常用文件的访问模式

1. 打开文件的模式有(默认为文本模式)：
```
r 只读模式【默认模式，文件必须存在，不存在则抛出异常】
w 只写模式【不可读；不存在则创建；存在则清空内容在写入】
a 只追加写模式【不可读；不存在则创建；存在则只追加内容】
```

2. 对于非文本文件，只能使用b模式。注：以b方式打开时，读取到的内容是字节类型，写入时也需要提供字节类型，不能指定编码。
```
rb 以二进制读取
wb 以二进制写入
ab 以二进制追加
```

3. `'+'`模式（就是增加了一个功能）
```
r+b 读写【可读，可写】
w+b 写读【可写，可读】
a+b 写读【可写，可读】
```

4. 以bytes类型操作的读写，写读，写读模式
```
r+b 读写【可读，可写】
w+b 写读【可写，可读】
a+b 写读【可写，可读】
```

5. 关于`r+`模式：打开一个文件用于读写，文件指针默认将会放在文件的开头。
:::tips
注意：如果在读写模式下，先写后读，那么文件就会出问题，因为默认光标是在文件的最开始，要是先写，则写入的内容会将原内容覆盖掉，直到覆盖到写完的内容，然后在从后面开始读取。
:::
```python
文件'test.txt'中的内容是：Hello World

#1. 先读后写
f1 = open('C:\\Users\\Fcant\\test.txt', encoding='utf-8', mode='r+')
content = f1.read()
print(content)
f1.write('Python开发者')
f1.close()

#2. 先写后读（错误实例）
f1 = open('C:\\Users\\Fcant\\test.txt', encoding='utf-8', mode='r+')
f1.write('Python开发者')
content = f1.read()
print(content) 
f1.close()
```
<a name="osnch"></a>
## 2、关闭文件
调用`close()`方法关闭文件<br />文件使用完毕后必须关闭，因为文件对象会占用操作系统的资源，并且操作系统同一时间能打开的文件数量也是有限的：
```python
f.close()
```
<a name="oLYTl"></a>
## 3、文件读写
<a name="r6MkH"></a>
### A.文件读取的四种方式
文件读取的方式有三种:
<a name="kHdrX"></a>
#### 1)`read()`
read：如果指定了参数 size，就按照该指定长度从文件中读取内容，否则，就读取全文。被读出来的内容，全部塞到一个字符串里面。这样有好处，就是东西都到内存里面了，随时取用；但如果文件内容太多了，内存会吃不消。注意换行符也为占用一个内存，缺点需要知道光标的位置，以及每一行的大小SIZE。
```python
f = open('C:\\Users\\Fcant\\test.txt', 'r',encoding='gbk')   #这里的文本文件，大家可以自己写一个，没有硬性要求。
# f.read(10)
f.read(5)  
'DataS'
```
<a name="aTGBF"></a>
#### 2)`readline()`
`readline()` 方法用于从文件读取整行，包括 "`\n`" 字符。从字面意思可以看出，该方法每次读出一行内容。所以，读取时占用内存小，比较适合大文件，该方法返回一个字符串对象。如果指定了一个非负数的参数，则返回指定大小的字节数，包括 "`\n`" 字符。<br />**语法：**`**fileObject.readline()**`**;**
```python
f = open('C:\\Users\\Fcant\\test.txt', 'r',encoding='gbk')
while True:
   line=f.read()
   if not line:#到 EOF，返回空字符串，则终止循环
            break
   print(line)
```
<a name="Xh6fK"></a>
#### 3)`readlines()`
`readlines()` 方法用于读取所有行(直到结束符 EOF)并返回列表，该列表可以由 Python 的 `for... in ...` 结构进行处理。当文件太大时，内存可能不够用。如果碰到结束符 EOF 则返回空字符串。<br />如果碰到结束符 EOF 则返回空字符串。<br />**语法：**`**fileObject.readlines()**`**;**
```python
f = open('C:\\Users\\Fcant\\test.txt', 'r',encoding='gbk')
print(f.readlines())

#遍历每行打印
f = open('C:\\Users\\Fcant\\test.txt', 'r',encoding='gbk')
for line in f.readlines():
    print(line)
```
<a name="gDqGp"></a>
#### 4)for循环读取文件内容
`for` 循环读取。文件句柄是一个迭代器。特点是每次循环只在内存中占一行的数据，非常节省内存。
```python
f1 = open('C:\\Users\\Fcant\\test.txt', encoding='utf-8')
for line in f1:    
    print(line.strip())
f1.close()
```
<a name="WyE1S"></a>
### B.文件写入
<a name="oMB1t"></a>
#### `w`模式
没有文件，则创建文件，写入内容；如果文件存在，先清空原文件内容，在写入新内容。
```python
f1 = open('C:\\Users\\Fcant\\test.txt', encoding='utf-8', mode='w')
f1.write('Hello World')
f1.close()
```
<a name="MOQiN"></a>
#### `wb`模式
```python
f1 = open(r'编码进阶.png', mode='rb')
content = f1.read()
f1.close()

f2 = open('图片.jpg', mode='wb')
f2.write(content)
f2.close()
```
<a name="Y4UFk"></a>
#### 关于清空
关闭文件句柄，再次以w模式打开此文件时，才会清空。
<a name="cmjdj"></a>
### C.指针定位
<a name="UxOkM"></a>
#### `tell()` 方法用来显示当前指针的位置
```python
f = open('test.txt')
print(f.read(10))  # read 指定读取的字节数
print(f.tell())    # tell()方法显示当前文件指针所在的文字
f.close()
```
<a name="hNMz8"></a>
#### `seek(offset, whence)` 方法用来重新设定指针的位置。

- `offset`:表示偏移量
- `whence`:只能传入012中的一个数字。 
   - 0表示从文件头开始
   - 1表示从当前位置开始
   - 2 表示从文件的末尾开始
```python
f = open('test.txt','rb')  # 需要指定打开模式为rb,只读二进制模式
print(f.read(3))
print(f.tell())
f.seek(2,0)   # 从文件的开头开始，跳过两个字节
print(f.read())
f.seek(1,1) # 从当前位置开始，跳过一个字节
print(f.read())
f.seek(-4,2) # 从文件末尾开始，往前跳过四个字节
print(f.read())
f.close()
```
<a name="Cazf5"></a>
## 4、总结-文件的打开模式
其实关于文件的打开有很多种模式，这里用一个表格总结一下

| 模式 | 概述 |
| --- | --- |
| t | 文本模式 (默认)。 |
| x | 写模式，新建一个文件，如果该文件已存在则会报错。 |
| b | 二进制模式。 |
| + | 打开一个文件进行更新(可读可写)。 |
| U | 通用换行模式（不推荐）。 |
| r | 以只读方式打开文件。文件的指针将会放在文件的开头。这是默认模式。 |
| rb | 以二进制格式打开一个文件用于只读。文件指针将会放在文件的开头。这是默认模式。一般用于非文本文件如图片等。 |
| r+ | 打开一个文件用于读写。文件指针将会放在文件的开头。 |
| rb+ | 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。一般用于非文本文件如图片等。 |
| w | 打开一个文件只用于写入。如果该文件已存在则打开文件，并从开头开始编辑，即原有内容会被删除。如果该文件不存在，创建新文件。 |
| wb | 以二进制格式打开一个文件只用于写入。如果该文件已存在则打开文件，并从开头开始编辑，即原有内容会被删除。如果该文件不存在，创建新文件。一般用于非文本文件如图片等。 |
| w+ | 打开一个文件用于读写。如果该文件已存在则打开文件，并从开头开始编辑，即原有内容会被删除。如果该文件不存在，创建新文件。 |
| wb+ | 以二进制格式打开一个文件用于读写。如果该文件已存在则打开文件，并从开头开始编辑，即原有内容会被删除。如果该文件不存在，创建新文件。一般用于非文本文件如图片等。 |
| a | 打开一个文件用于追加。如果该文件已存在，文件指针将会放在文件的结尾。也就是说，新的内容将会被写入到已有内容之后。如果该文件不存在，创建新文件进行写入。 |
| ab | 以二进制格式打开一个文件用于追加。如果该文件已存在，文件指针将会放在文件的结尾。也就是说，新的内容将会被写入到已有内容之后。如果该文件不存在，创建新文件进行写入。 |
| a+ | 打开一个文件用于读写。如果该文件已存在，文件指针将会放在文件的结尾。文件打开时会是追加模式。如果该文件不存在，创建新文件用于读写。 |
| ab+ | 以二进制格式打开一个文件用于追加。如果该文件已存在，文件指针将会放在文件的结尾。如果该文件不存在，创建新文件用于读写。 |

以上为文件打开的所有方式，但实际应用中常用的就其中几种，不必硬记，根据自己的需求记住几个常用的就好。
<a name="bed7eecf"></a>
## 5、实现文件拷贝功能
```python
import os

file_name = input('请输入一个文件路径:')
if os.path.isfile(file_name):
    old_file = open(file_name, 'rb')  # 以二进制的形式读取文件

    names = os.path.splitext(file_name)
    new_file_name = names[0] + '.bak' + names[1]

    new_file = open(new_file_name, 'wb')  # 以二进制的形式写入文件

    while True:
        content = old_file.read(1024)  # 读取出来的内容是二进制
        new_file.write(content)
        if not content:
            break

    new_file.close()
    old_file.close()
else:
    print('您输入的文件不存在')
```
<a name="OPOqu"></a>
## 6、将数据写入内存
除了将数据写入到一个文件以外，还可以使用代码，将数据暂时写入到内存里，可以理解为数据缓冲区。Python中提供了`StringIO`和`BytesIO`这两个类将字符串数据和二进制数据写入到内存里。
<a name="PD5rn"></a>
### `StringIO`
`StringIO`可以将字符串写入到内存中，像操作文件一下操作字符串。
```python
from io import StringIO

# 创建一个StringIO对象
f = StringIO()
# 可以像操作文件一下，将字符串写入到内存中
f.write('hello\r\n')
f.write('good')

# 使用文件的 readline和readlines方法，无法读取到数据
# print(f.readline())
# print(f.readlines())

# 需要调用getvalue()方法才能获取到写入到内存中的数据
print(f.getvalue())

f.close()
```
<a name="EbiBa"></a>
### `BytesIO`
如果想要以二进制的形式写入数据，可以使用`BytesIO`类，它的用法和`StringIO`相似，只不过在调用`write`方法写入时，需要传入二进制数据。
```python
from io import BytesIO

f = BytesIO()
f.write('你好\r\n'.encode('utf-8'))
f.write('中国'.encode('utf-8'))

print(f.getvalue())
f.close()
```
