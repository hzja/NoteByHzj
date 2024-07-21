Python CSV文件
<a name="dInDC"></a>
## 简介
<a name="q2k2D"></a>
### CSV
`CSV` 全称 `Comma-Separated Values`，中文叫逗号分隔值或字符分隔值，它**以纯文本形式存储表格数据（数字和文本）**，其本质就是一个**字符序列**，可以由任意数目的记录组成，记录之间以某种换行符分隔，每条记录由字段组成，通常所有记录具有完全相同的字段序列，字段间常用逗号或制表符进行分隔。CSV 文件格式简单、通用，在现实中有着广泛的应用，其中使用最多的是**在程序之间转移表格数据**。
<a name="kWTqw"></a>
### CSV 与 Excel
因为 CSV 文件与 Excel 文件默认都是用 Excel 工具打开，那他们有什么区别呢？通过下表简单了解一下。

| CSV | Excel |
| --- | --- |
| 文件后缀为 .csv | 文件后缀为 .xls 或 .xlsx |
| 纯文本文件 | 二进制文件 |
| 存储数据不包含格式、公式等 | 不仅可以存储数据，还可以对数据进行操作 |
| 可以通过 Excel 工具打开，也可以通过文本编辑器打开 | 只能通过 Excel 工具打开 |
| 只能编写一次列标题 | 每一行中的每一列都有一个开始标记和结束标记 |
| 导入数据时消耗内存较少 | 数据时消耗内存较多 |

<a name="z5qIM"></a>
## 基本使用
Python 通过 `**csv 模块**`来实现 CSV 格式文件中数据的读写，该模块提供了兼容 Excel 方式输出、读取数据文件的功能，这样无需知道 Excel 所采用 CSV 格式的细节，同样的它还可以定义其他应用程序可用的或特定需求的 CSV 格式。<br />csv 模块中使用 `reader 类`和 `writer 类`读写序列化的数据，使用 `DictReader 类`和 `DictWriter 类`以字典的形式读写数据，下面来详细看一下相应功能。首先来看一下 csv 模块常量信息，如下所示：

| 属性 | 说明 |
| --- | --- |
| QUOTE_ALL | 指示 writer 对象给所有字段加上引号 |
| QUOTE_MINIMAL | 指示 writer 对象仅为包含特殊字符（如：定界符、引号字符、行结束符等）的字段加上引号 |
| QUOTE_NONNUMERIC | 指示 writer 对象为所有非数字字段加上引号 |
| QUOTE_NONE | 指示 writer 对象不使用引号引出字段 |

<a name="4BqTZ"></a>
### `writer(csvfile, dialect='excel', **fmtparams)`
返回一个 `writer 对象`，该对象负责**将用户的数据在给定的文件类对象上转换为带分隔符的字符串**。

- `csvfile` 可以是具有 `write()` 方法的任何对象，如果 csvfile 是文件对象，则使用 `newline=''`  打开；
- 可选参数 `dialect` 是用于不同的 CSV 变种的特定参数组；
- 可选关键字参数 `fmtparams` 可以覆写当前变种格式中的单个格式设置。

看下示例：
```python
import csv
with open('test.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['id', 'name', 'age'])
    # 写入多行
    data = [('1001', '张三', '21'), ('1002', '李四', '31')]
    writer.writerows(data)
```
<a name="hUZXG"></a>
### `reader(csvfile, dialect='excel', **fmtparams)`
返回一个 `reader 对象`，该对象将逐行遍历 csvfile，csvfile 可以是文件对象和列表对象，如果是文件对象要使用 `newline=''` 打开。看下示例：
```python
import csv
with open('test.csv', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=' ')
    for row in reader:
        print(', '.join(row))
```
<a name="vxMVv"></a>
### Sniffer 类
用于**推断 CSV 文件的格式**，该类提供了如下两个方法：
<a name="JTGwk"></a>
#### `sniff(sample, delimiters=None)`
分析给定的 sample，如果给出可选的 delimiters 参数，则该参数会被解释为字符串，该字符串包含了可能的有效定界符。
<a name="rLLsU"></a>
#### `has_header(sample)`
分析示例文本（假定为 CSV 格式），如果第一行很可能是一系列列标题，则返回 True。<br />该类及方法使用较少，了解即可，下面通过一个示例简单了解一下。
```python
import csv
with open('test.csv', newline='') as csvfile:
     dialect = csv.Sniffer().sniff(csvfile.read(1024))
     csvfile.seek(0)
     reader = csv.reader(csvfile, dialect)
     for row in reader:
         print(row)
```
<a name="3reua"></a>
### Reader 对象
Reader 对象指 DictReader 实例和 `reader()` 函数返回的对象，下面看一下其公开属性和方法。
<a name="3voZe"></a>
#### `next()`
返回 reader 的可迭代对象的下一行，返回值可能是列表或字典。
<a name="uEkmD"></a>
#### `dialect`
dialect 描述，只读，供解析器使用。
<a name="LVwcZ"></a>
#### `line_num`
源迭代器已经读取了的行数。
<a name="MJiZK"></a>
#### `fieldnames`
字段名称，该属性为 DictReader 对象属性。
<a name="gY11Q"></a>
### Writer 对象
Writer 对象指 DictWriter 实例和 `writer()` 函数返回的对象，下面看一下其公开属性和方法。
<a name="gGYv4"></a>
#### `writerow(row)`
将参数 row 写入 writer 的文件对象。
<a name="kKyk5"></a>
#### `writerows(rows)`
将 rows_（即能迭代出多个上述_ row 对象的迭代器）中的所有元素写入 writer 的文件对象。
<a name="oUlLh"></a>
#### `writeheader()`
在 writer 的文件对象中，写入一行字段名称，该方法为 DictWriter 对象方法。
<a name="RWOOj"></a>
#### `dialect`
dialect 描述，只读，供 writer 使用。
<a name="LVqlZ"></a>
### 写读追加状态
```python
'r'：读
'w'：写
'a'：追加
'r+' == r+w（可读可写，文件若不存在就报错(IOError)）
'w+' == w+r（可读可写，文件若不存在就创建）
'a+' ==a+r（可追加可写，文件若不存在就创建）
对应的，如果是二进制文件，就都加一个b就好了：
'rb'  'wb'  'ab'  'rb+'  'wb+'  'ab+
```

