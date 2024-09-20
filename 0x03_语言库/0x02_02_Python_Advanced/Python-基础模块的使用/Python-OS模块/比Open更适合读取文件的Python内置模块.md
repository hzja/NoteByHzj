Python <br />有时需要把数据永久存储起来，随时使用随时读取。例如，通过程序建立的列表、字典等数据，当程序结束时，需要把这些数据存储到文件中，当程序再次启动时，可以把这些数据读入到程序中，避免这些数据的重新录入。<br />在Python语言中，负责文件操作的称为文件对象，文件对象不仅可以访问存储在磁盘中的文件，也可以访问网络文件。文件对象通过`open`函数得到，获取文件对象后，就可以使用文件对象提供的方法来读写文件。<br />但`open`函数在处理某些问题是并不是很理想，有没有其他比`open`函数更加适合读取某些特定文件呢？下面就一起来看看！
<a name="xHsQh"></a>
### 回顾open函数
对文件操作使用最频繁对函数，`open()`打开一个文件对象，使用Python内置的`open()`函数，传入文件名和模式。
```python
file_object = open(name [, mode][, buffering])
```
name: 要读取的文件名称。<br />mode: 打开文件的模式，选填。r, r+, w, w+, a, a+使用最多。<br />buffering: 文件所需的缓冲区大小, 选填。0表示无缓冲, 1表示线路缓冲。有四种打开文件的不同方法（模式）

- "r" - 读取 - 默认值。打开文件进行读取，如果文件不存在则报错。
- "a" - 追加 - 打开供追加的文件，如果不存在则创建该文件。
- "w" - 写入 - 打开文件进行写入，如果文件不存在则创建该文件。
- "x" - 创建 - 创建指定的文件，如果文件存在则返回错误。

此外，可以指定文件是应该作为二进制还是文本模式进行处理。

- "t" - 文本 - 默认值。文本模式。
- "b" - 二进制 - 二进制模式（例如图像）。

使用时常通过`with open()`方法读写文件。
```python
with open("文件名.txt", "r") as fin: # fin为 别名(文件句柄对象)
    file =  fin.read() # 会一次性读取文件的全部内容
    file_line = fin.readline() # 可以每次读取一行内容
    file_lines = fin.readlines() # 一次读取所有内容并按行返回list
```
<a name="rmNav"></a>
## pathlib

---

以前在Python中操作文件路径，更多的时候是使用os模块。Python3的系统标准库pathlib模块的Path对路径的操作会更简单。<br />pathlib的一些基本操作，如文件名操作、路径操作、文件操作等等并不在本文讨论范围。本此介绍使用率非常高的文件操作，其文件读写方法。
<a name="lVl5G"></a>
### `Path.open`
在`pathlib`里如果要打开一个文件十分的简单。
```python
Path.open(mode='r', 
          buffering=-1, 
          encoding=None, 
          errors=None, 
          newline=None)
```
打开路径指向的文件，就像内置的`open()`函数所做的一样。
```python
from pathlib2 import Path
example_path = Path('./info.csv')
with example_path.open() as f:
   print(f.readline())
   print(f.read())
```
结果
```
"编号","性别","年龄","成绩"

961067,"男",23,97
969157,"男",38,98
969237,"男",27,120
970394,"男",27,118
```
<a name="BHuD7"></a>
### Path读与写
对于简单的文件读写，在`pathlib`模块中有几个简便的方法：<br />`Path.read_text()`: 以字符串形式返回路径指向的文件的解码后文本内容。<br />`Path.read_bytes()`: 以二进制/字节模式打开路径并以字节串的形式返回内容。<br />`Path.write_text()`: 打开路径并向其写入字符串数据。<br />`Path.write_bytes()`: 以二进制/字节模式打开路径并向其写入数据。
```python
>>> p = Path('my_binary_file')
>>> p.write_bytes(b'Binary file contents')
20
>>> p.read_bytes()
b'Binary file contents'

>>> p = Path('my_text_file')
>>> p.write_text('Text file contents')
18
>>> p.read_text()
'Text file contents'
```
更多详情可参见[pathlib模块](https://docs.python.org/zh-cn/3/library/pathlib.html)。
<a name="wikIx"></a>
## fileinput

---

如果只想读取一个文件，使用`open()`。如果需要实现文件列表的批量循环操作，不妨使用本模块。
<a name="y7Y2t"></a>
### `fileinput.input`
`input`是`fileinput`模块的初始接口，其使用也是较简单。
```python
fileinput.input(files=None, 
                inplace=False, 
                backup='', 
                *, 
                mode='r', 
                openhook=None)
```
files 需要读取的文件对象，可迭代对象。<br />inplace 标准输出重定向替换，表示是否将标准输出的结果写回文件，默认不取代。<br />backup 读取时同时备份文件，可以指定备份的后缀名，比如 `backup='.bak'`。<br />mode  文件读取模式，fileinput 有且仅有这两种读取模式r和 rb。

- 默认使用 `mode='r'`
- 如果文件是二进制的，可以使用mode='rb' 模式。

openhook 支持用户传入自定义的对象读取方法。fileinput 内置了两个勾子函数：

- `fileinput.hook_encoded(encoding, errors=None)`<br />使用 gzip 和 bz2模块透明地打开 gzip 和 bzip2 压缩的文件
- `fileinput.hook_compressed(filename, mode)`<br />使用给定的 encoding 和 errors 来读取文件。
<a name="IueL4"></a>
### 从标准输入中读取
若`input()`不传任何参数时，`fileinput` 默认会以 stdin 作为输入源。<br />运行stdinput.py后，在编译器中输入内容，程序会自动读取并再打印一次。
```python
# stdinput.py
import fileinput
for line in fileinput.input():
    print(line)

>>> python stdinput.py 
>>> hello
hello
>>> python
python
```
<a name="J9Q4C"></a>
### 从指定文件中读取
<a name="e1nBP"></a>
#### 读取批量文件
```python
import fileinput
with fileinput.input(files=('info1.csv', 'info2.csv')) as file:
    for line in file:
        print(f'{fileinput.filename()} 第{fileinput.lineno()}行: {line}', end='') 
```
输出
```
info1.csv 第1行: |编号| |性别| |年龄| |成绩|  
info1.csv 第2行: 961067 |男| 23 97  
info1.csv 第3行: 969157 |男| 38 98  
info2.csv 第4行: "编号","性别","年龄","成绩"  
info2.csv 第5行: 969237,"男",27,120  
info2.csv 第6行: 970394,"男",27,118  
```
由于 info.txt 和 info.txt 的内容被整合成一个文件对象 file ，因此 `fileinput.lineno()` 只有在读取一个文件时，才是原文件中真实的行号。如果想要在读取多个文件的时候，也能读取原文件的真实行号，可以使用 `fileinput.filelineno()` 方法。<br />以上几个常用对方法解释如下。<br />`fileinput.filename()`<br />返回当前被读取的文件名。在第一行被读取之前，返回 None。<br />`fileinput.lineno()`<br />返回已被读取的累计行号。在第一行被读取之前，返回 0。在最后一个文件的最后一行被读取之后，返回该行的行号。<br />`fileinput.filelineno()`<br />返回当前文件中的行号。在第一行被读取之前，返回 0。在最后一个文件的最后一行被读取之后，返回此文件中该行的行号。
<a name="UTajj"></a>
#### 读取单个文件
与批量读取文件一样，只需要在参数files中传人一个文件即可。
```python
import fileinput
with fileinput.input(files=('info2.csv')) as file:
    for line in file:
        print(f'{fileinput.filename()} 第{fileinput.lineno()}行: {line}', end='') 
```
输出
```
info2.csv 第1行: "编号","性别","年龄","成绩"
info2.csv 第2行: 969237,"男",27,120
info2.csv 第3行: 970394,"男",27,118
```
<a name="CUkML"></a>
### 与`glob`配合批量读取
<a name="ZGZWU"></a>
#### `glob`简介
`glob`是python自带的一个操作文件的相关模块，可以对文件夹下所有文件进行遍历，并将符合匹配模式的文件名保存为一个list列表。
```python
glob.glob(pathname, *, recursive=False)
```
返回匹配 pathname 的可能为空的路径名列表，其中的元素必须为包含路径信息的字符串。<br />pathname 可以是绝对路径 (如 /usr/src/Tools/sub/1.gif) 或相对路径 (如 ../../Tools/*/*.gif)，并且可包含 shell 风格的通配符。<br />实例<br />一个包含以下内容的目录：文件 1.gif, 2.txt, card.gif 以及一个子目录 sub 其中只包含一个文件 3.txt。glob()将产生如下结果。请注意路径的任何开头部分都将被保留。
```python
>>> import glob
>>> glob.glob('./[0-9].*')
['./1.gif', './2.txt']
>>> glob.glob('*.gif')
['1.gif', 'card.gif']
>>> glob.glob('?.gif')
['1.gif']
>>> glob.glob('**/*.txt', recursive=True)
['2.txt', 'sub/3.txt']
>>> glob.glob('./**/', recursive=True)
['./', './sub/']
```
<a name="yKGWE"></a>
#### fileinput与glob配合使用
```python
import fileinput
import glob

for line in fileinput.input(glob.glob("*.csv")):
    if fileinput.isfirstline():
        print(f'Reading {fileinput.filename()}...'.center(50,'-'))
    print(str(fileinput.filelineno()) + ': ' + line.upper(), end="")
```
输出
```
---------------Reading info1.csv...---------------
1: |编号| |性别| |年龄| |成绩|
2: 961067 |男| 23 97
3: 969157 |男| 38 98
---------------Reading info2.csv...---------------
1: "编号","性别","年龄","成绩"
2: 969237,"男",27,120
3: 970394,"男",27,118
```
更多使用方法请参见[fileinput模块](https://docs.python.org/zh-cn/3/library/fileinput.html)。
<a name="oYO5A"></a>
## codecs

---

常用`open`方法操作文件，写入str类型，不管字符串是什么编码方式，此时一般不会出现什么问题。但有时候爬虫或者其他方式得到一些数据写入文件时会有编码不统一的问题，或在自然语言处理过程中，使用`open`方法操作文件会经常出现报错，通常是编码错误。<br />此时如若想继续使用 open 方式打开，就需要先将输入文件`decode`，统一转为`unicode `，再`encode`到目标编码方式，如gbk、utf-8等等。即<br />input ---> encode ---> unicode ---> output<br />幸运的是，可以使用 `codecs.open()` 代替这一繁琐操作。这种方法可以指定一个编码打开文件，读取返回的将是`unicode`。
<a name="HQzhV"></a>
### `codecs.open()`
```python
codecs.open(filename, 
            mode='r', 
            encoding=None, 
            errors='strict', 
            buffering=-1)
```
使用给定的 mode 打开已编码的文件并返回一个 `StreamReaderWriter` 的实例，提供透明的编码/解码。<br />`mode`<br />参数可以是内置 `open()`函数所接受的任意二进制模式，默认的文件模式为 'r'，表示以读取模式打开文件。'b' 会被自动添加。<br />`encoding`<br />指定文件所要使用的编码格式。允许任何编码为字节串或从字节串解码的编码格式，而文件方法所支持的数据类型则取决于所使用的编解码器。<br />写入时，如果参数是unicode，则使用`open()`时指定的编码进行编码后写入；如果是str，则先根据源代码文件声明的字符编码，解码成`unicode`后再进行前述操作。<br />相对内置的`open()`来说，这个方法比较不容易在编码上出现问题，并且在速度不变的同时，还兼容`open()`函数所有操作命令。
<a name="cRXqk"></a>
#### 应用实例
将未知编码方式的csv文件转为utf-8格式文件。
```python
import codecs

src="......\\xxxx.csv"
dst="......\\xxx_utf8.csv"

def ReadFile(filePath):
    with codecs.open(filePath, "r") as f:
        return f.read()

def WriteFile(filePath, u, encoding="utf-8"):
    # with codecs.open(filePath,"w",encoding) as f:
    with codecs.open(filePath, "wb") as f:
        f.write(u.encode(encoding, errors="ignore"))

def CSV_2_UTF8(src, dst):
    content = ReadFile(src)
    WriteFile(dst, content, encoding="utf-8")
    
CSV_2_UTF8(src, dst)
```
更多相关方法可参见[codecs模块](https://docs.python.org/zh-cn/3/library/codecs.html)。
<a name="WL5rF"></a>
## CSV

---

[CSV (Comma Separated Values)](http://zh.wikipedia.org/zh-cn/逗号分隔值)，即逗号分隔值（也称字符分隔值，因为分隔符可以不是逗号），是一种常用的文本格式，用以存储表格数据，包括数字或者字符。很多程序在处理数据时都会碰到csv这种格式的文件。<br />python内置了csv模块。常用的有四个方法：
<a name="E0p2K"></a>
### `csv.reader`
```python
csv.reader(csvfile, 
           dialect='excel', 
           **fmtparams)
```
返回一个reader对象，该对象将逐行遍历csvfile 。<br />`svfile`<br />可以是任何对象，文件对象和列表对象均适用。如果 csvfile 是文件对象，则打开它时应使用 `newline=''`。<br />`dialect`<br />用于不同的 CSV 变种的特定参数组。<br />`fmtparams`<br />可以覆写当前变种格式中的单个格式设置。有关变种和格式设置参数的完整详细信息，请参见 [变种与格式参数](https://docs.python.org/zh-cn/3/library/csv.html#csv-fmt-params)。
<a name="egCwz"></a>
#### 应用实例
```python
import csv
with open('info.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile
                             , delimiter=' '
                             , quotechar='|'
                            ) 
    for row in spamreader:
        print(row)
        print(', '.join(row))
```
输出
```
['"编号","性别","年龄","成绩"']
"编号","性别","年龄","成绩"
['961067,"男","23","97"']
961067,"男","23","97"
['969157,"男","38","98"']
969157,"男","38","98"
```
以上用到的变种与格式参数解释如下。<br />`delimiter`<br />一个用于分隔字段的单字符，默认为 `','`。<br />`quotechar`<br />一个单字符，用于包住含有特殊字符的字段，特殊字符如 定界符 或 引号字符 或 换行符。默认为 `' " '`。<br />由于使用 `open()`来读取 CSV 文件，因此默认情况下，将使用系统默认编码来解码文件并转换为`unicode`，要使用其他编码来解码文件，可使用`open`的`encoding`参数:
```python
import csv
with open('sample.csv', newline='', encoding='utf-8') as f:
    reader = csv.reader(f)
    for row in reader:
        print(row)
```
<a name="wm9aX"></a>
### `csv.writer`
```python
csv.writer(csvfile, dialect='excel', **fmtparams)
```
返回一个 `writer` 对象，该对象将用户的数据在给定的文件类对象上转换为带分隔符的字符串。None 值会写入为空字符串。<br />写入前，所有非字符串数据都先用 `str()` 转化为字符串再写入。<br />csvfile 可以是具有 `write()` 方法的任何对象。
<a name="VdYwA"></a>
#### 应用案例
```python
import csv
with open('info.csv', 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=' ',
                            quotechar='|', quoting=csv.QUOTE_NONNUMERIC)
    spamwriter.writerow(["编号","性别","年龄","成绩"])
    spamwriter.writerow([961067,"男",23,97])
    spamwriter.writerow([969157,"男",38,98])
```
输出csv文件打开如下
```
|编号| |性别| |年龄| |成绩|
961067 |男| 23 97
969157 |男| 38 98
```
参数quoting说明：<br />控制 writer 何时生成引号，以及 reader 何时识别引号。该属性可以等于任何 `QUOTE_*` 常量，默认为`QUOTE_MINIMAL`。<br />`QUOTE_*` 常量包括：

- `csv.QUOTE_ALL`<br />指示 writer 对象给所有字段加上引号。
- `csv.QUOTE_MINIMAL`<br />指示 writer 对象仅为包含特殊字符（例如定界符、引号字符 或 行结束符 中的任何字符）的字段加上引号。
- `csv.QUOTE_NONNUMERIC`<br />指示 writer 对象为所有非数字字段加上引号。<br />指示 reader 将所有未用引号引出的字段转换为 float 类型。
- `csv.QUOTE_NONE`<br />指示 writer 对象不使用引号引出字段。当 定界符 出现在输出数据中时，其前面应该有 转义符。如果未设置 转义符，则遇到任何需要转义的字符时，writer 都会抛出 Error 异常。<br />指示 reader 不对引号字符进行特殊处理。
<a name="s3yeO"></a>
### `csv.DictReader`
```python
csv.DictReader(*f*, 
               *fieldnames=None*, 
               *restkey=None*, 
               *restval=None*, 
               *dialect='excel'*, **args*, ***kwds*)
```
创建一个对象，该对象在操作上类似于常规 reader，但是将每行中的信息映射到一个 dict，该 dict 的键由 fieldnames 可选参数给出。<br />`fieldnames`<br />参数是一个 `sequence`。如果省略 `fieldnames`，则文件 f 第一行中的值将用作字段名。无论字段名是如何确定的，字典都将保留其原始顺序。<br />如果某一行中的字段多于字段名，则剩余数据会被放入一个列表，并与 restkey 所指定的字段名 (默认为 None) 一起保存。如果某个非空白行的字段少于字段名，则缺失的值会使用 restval 的值来填充 (默认为 None)。
<a name="VRHNe"></a>
#### 应用实例
```python
import csv
with open('info.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        print(row)
```
注意：<br />在 3.6 版更改: 返回的行现在的类型是 `OrderedDict`。<br />在 3.8 版更改: 现在，返回的行是 dict类型。
```
# python 3.6/3.7输出：
OrderedDict([('编号', '961067'), ('性别', '男'), ('年龄', '23'), ('成绩', '97')])
OrderedDict([('编号', '969157'), ('性别', '男'), ('年龄', '38'), ('成绩', '98')])

# python 3.8输出
{'编号': '961067', '性别': '男', '年龄': '23', '成绩': '97'}
{'编号': '969157', '性别': '男', '年龄': '38', '成绩': '98'}
```
Reader 对象的一些方法<br />`csvreader.line_num`<br />源迭代器已经读取了的行数。它与返回的记录数不同，因为记录可能跨越多行。<br />`csvreader.fieldnames`<br />字段名称。如果在创建对象时未传入字段名称，则首次访问时或从文件中读取第一条记录时会初始化此属性。
```python
import csv
with open('info.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    print(f'文件字段名称：{reader.fieldnames}\n')
    for row in reader:
        print(f'读取的第{reader.line_num}行.')
        print(dict(row))
```
输出
```
文件字段名称：['编号', '性别', '年龄', '成绩']

读取的第2行.
{'编号': '961067', '性别': '男', '年龄': '23', '成绩': '97'}
读取的第3行.
{'编号': '969157', '性别': '男', '年龄': '38', '成绩': '98'}
读取的第4行.
{'编号': '969237', '性别': '男', '年龄': '27', '成绩': '120'}
读取的第5行.
{'编号': '970394', '性别': '男', '年龄': '27', '成绩': '118'}
```
<a name="FGNsM"></a>
### `csv.DictWriter`
```python
csv.DictWriter(f, fieldnames, restval='', extrasaction='raise', dialect='excel', *args, **kwds)
```
创建一个对象，该对象在操作上类似常规 writer，但会将字典映射到输出行。<br />`fieldnames` 参数是由键组成的 `sequence`，它指定字典中值的顺序，这些值会按指定顺序传递给 `writerow()` 方法并写入文件 f。<br />如果字典缺少 fieldnames 中的键，则可选参数 `restval` 用于指定要写入的值。<br />如果传递给 `writerow()` 方法的字典的某些键在 `fieldnames` 中找不到，则可选参数 `extrasaction` 用于指定要执行的操作。如果将其设置为默认值 `'raise'`，则会引发 `ValueError`。如果将其设置为 `'ignore'`，则字典中的其他键值将被忽略。
<a name="gwyvO"></a>
#### 应用案例
```python
with open('info.csv', 'w', newline='') as csvfile:
    fieldnames = ['编号', '性别', '年龄', '成绩']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames, quoting=csv.QUOTE_NONNUMERIC)

    writer.writeheader()
    writer.writerow({'编号': 961067, '性别': '男', '年龄': 23, '成绩': 97})
    writer.writerow({'编号': 969157, '性别': '男', '年龄': 38, '成绩': 98})
```
写入的csv文件打开结果如下
```
"编号","性别","年龄","成绩"
961067,"男",23,97
969157,"男",38,98
```
Writer 对象的一些方法
```python
DictWriter.writeheader()
```
在 writer 的文件对象中，写入一行字段名称（字段名称在构造函数中指定），并根据当前设置的变种进行格式化。
```python
csvwriter.writerow(row)
```
将参数 row 写入 writer 的文件对象。
```python
csvwriter.writerows(rows)
```
将 rows (即能迭代出多个上述 row 对象的迭代器）中的所有元素写入 writer 的文件对象<br />更多相关方法可参见[csv模块](https://docs.python.org/zh-cn/3/library/csv.html#index-0)。
<a name="sM8qm"></a>
## 第三方模块

---

另外还有专门针对Word文件、Excel文件及PDF文件的模块，这里只简单介绍下。
<a name="GQA0B"></a>
### word文件
<a name="tyM87"></a>
#### python-docx
```python
from docx import Document
# 初始化一个实例
document = Document()
# 增加一集标题
document.add_heading('Document Title', 0)
```
<a name="dajwa"></a>
### PDF文件
<a name="OzrrA"></a>
#### pdfplumber
```python
# 导入pdfplumber
import pdfplumber
# 读取pdf文件，保存为pdf实例
pdf =  pdfplumber.open("D:\\demo.pdf")
# 访问第一页
first_page = pdf.pages[0]
# 自动读取表格信息，返回列表
table = first_page.extract_table()
```
<a name="V19ev"></a>
### Excel文件
比较常用的Excel操作模块有如下三种。

- xlrd 读取 .xls 和 .xlsx 文件
- xlwings 读取 .xls 和 .xlsx 文件
- openpyxl 读取 .xlsx 文件
<a name="Vzcnk"></a>
#### xlrd
xlrd不能创建和保存Excel文件
```python
import xlrd
# 打开Excel文件
df = xlrd.open_workbook(name)
# 查看所有sheet
df.sheet_names()
```
<a name="VYUuN"></a>
#### xlwings
xlwings 直接对接的是 Excel 应用程序，然后才是工作簿 books 和工作表 sheets，xlwings 需要安装有 Excel 应用程序的环境xlwings 可以读取 .xls 和 .xlsx 文件。
```python
import xlwings as xw
# 程序可见，只打开不新建工作薄
app = xw.App(visible=True, add_book=False) 
app.display_alerts = False # 警告关闭
app.screen_updating = False # 屏幕更新关闭
# wb = app.books.open(xls_path)
wb = app.books.open(xlsx_path)
wb = app.books.add() # 创建文件
wb.save() # 保存文件
wb.close() # 关闭文件
app.quit() # 关闭程序
```
<a name="ZY2Iw"></a>
#### openpyxl
openpyxl 可以读取 .xlsx 文件，如果读取.xls 文件会报错。
```python
import openpyxl
# 读取excel文件
wb = openpyxl.load_workbook(xlsx_path)
# 获取工作表、新工作簿中指定即创建工作表
sheet = wb.active
# 保存文件
wb.save()
```
<a name="t4ThW"></a>
### 参考
pathlib模块: [https://docs.python.org/zh-cn/3/library/pathlib.html](https://docs.python.org/zh-cn/3/library/pathlib.html)<br />fileinput模块: [https://docs.python.org/zh-cn/3/library/fileinput.html](https://docs.python.org/zh-cn/3/library/fileinput.html)<br />codecs模块: [https://docs.python.org/zh-cn/3/library/codecs.html](https://docs.python.org/zh-cn/3/library/codecs.html)<br />CSV (Comma Separated Values: [http://zh.wikipedia.org/zh-cn/](http://zh.wikipedia.org/zh-cn/)逗号分隔值<br />变种与格式参数: [https://docs.python.org/zh-cn/3/library/csv.html#csv-fmt-params](https://docs.python.org/zh-cn/3/library/csv.html#csv-fmt-params)<br />csv模块: [https://docs.python.org/zh-cn/3/library/csv.html#index-0](https://docs.python.org/zh-cn/3/library/csv.html#index-0)
