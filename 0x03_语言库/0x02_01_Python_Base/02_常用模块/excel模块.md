# excel模块

## openpyxl官方手册

### 创建excel文件

开始使用openpyxl时无需文件系统中创建文件，只要导入`workbook`类就可：

~~~ python
>>> from openpyxl import Workbook
>>> wb = Workbook()
~~~



至少一个工作表在工作簿创建后可通过`Workbook.active`属性定位到工作表：

```ruby
>>> ws = wb.active
```



> Note：
>
> 该工作簿默认索引从0开始，除非索引值被修改，否则使用这个方法总获取第一个工作表



可使用`Workbook.create_sheet()`方法创建新工作表

~~~ python
>>> ws1 = wb.create_sheet("Mysheet") # 插入到最后 (默认)

>>> ws2 = wb.create_sheet("Mysheet", 0) # 插入到最前  

>>> ws3 = wb.create_sheet("Mysheet", -1) # 插入到倒数第二
~~~



工作表在创建时按数字序列自动命名(如Sheet，Sheet1，Sheet2，……)可在任何时候通过`Worksheet.title`属性修改工作表名：

~~~ python
>>>ws.title = "New Title"
~~~



创建工作表的标签背景色默认白色,可通过`Worksheet.sheet_properties.tabColor`对象中设置`RRGGBB`格式颜色代码修改：

~~~ python
>>>ws.sheet_properties.tabColor = "1072BA"
~~~



设置了worksheet名称，可将名称作为工作表索引：

```ruby
>>> ws3 = wb["New Title"]
```



可通过`Workbook.sheetname`对象查看工作簿中所有工作表名称

```python
>>> print(wb.sheetnames)
['Sheet2', 'New Title', 'Sheet1']
```



可遍历整个工作簿：

```python
>>> for sheet in wb:
...     print(sheet.title)
```



可使用`Workbook.copy_worksheet()`方法创建一个工作簿中所有表副本：

```ruby
>>> source = wb.active
>>> target = wb.copy_worksheet(source)
```



> <b>Note：</b>
>
> + 只有单元格(包括值、样式、超链接、备注)和一些工作表对象(包括尺寸、格式和参数)会被复制，其他属性不会被复制如图片或图表；
> + 无法在两个工作簿中复制工作表。当工作簿处于只读或只写状态时也无法复制工作表；



### 数据操作

#### 访问一个单元格

现在知道如何获取一个工作表，可开始修改单元格内容，单元格可通过工作表中索引直接访问：

```ruby
>>> c = ws['A4']
```

这将返回“A4”单元格内容，如不存在则创建一个，可直接对单元格赋值：

```ruby
>>> ws['A4'] = 4
```

是`Worksheet.cell()`方法，工具支持通过行列号访问单元格：

~~~ python
>>> d = ws.cell(row=4, column=2, value=10)
~~~

> <b>Note</b>
>  当内存中创建工作表但表中不包含任何单元格。单元格将在第一次访问时创建；
>
> <b>Warning</b>
>
> 因为这种特性，遍历而不是访问这些单元格将在内存中全部创建它们，即使并没有给它们赋值。比如说

~~~ python 
>>>for x in range(1,101):  

...     for y in range(1,101):  
 
...         ws.cell(row=x, column=y)
~~~



#### 访问多个单元格

可通过切片访问一个范围内单元格：

~~~ python
>>> cell_range = ws['A1':'C2']
~~~



行或列单元格也可通过类似的方法访问：

~~~ python
>>> colC = ws['C']
>>> col_range = ws['C:D']
>>> row10 = ws[10]
>>> row_range = ws[5:10]
~~~



也可用`Worksheet.iter_rows()`方法：

~~~ python
>>> for row in ws.iter_rows(min_row=1, max_col=3, max_row=2):
...    for cell in row:
...        print(cell)
<Cell Sheet1.A1>
<Cell Sheet1.B1>
<Cell Sheet1.C1>
<Cell Sheet1.A2>
<Cell Sheet1.B2>
<Cell Sheet1.C2>
~~~



类似用`Worksheet.iter_cols()`方法将返回列：

~~~  python
>>> for col in ws.iter_cols(min_row=1, max_col=3, max_row=2):
...     for cell in col:
...         print(cell)
<Cell Sheet1.A1>
<Cell Sheet1.A2>
<Cell Sheet1.B1>
<Cell Sheet1.B2>
<Cell Sheet1.C1>
<Cell Sheet1.C2>
~~~



> <b>Note：</b> 
>  出于性能考虑，`Worksheet.iter_cols()`方法不支持在只读模式使用



若需遍历文件内所有行和列，可使用`Worksheet.rows`属性：

~~~ python
>>> ws = wb.active
>>> ws['C9'] = 'hello world'
>>> tuple(ws.rows)
((<Cell Sheet.A1>, <Cell Sheet.B1>, <Cell Sheet.C1>),
(<Cell Sheet.A2>, <Cell Sheet.B2>, <Cell Sheet.C2>),
(<Cell Sheet.A3>, <Cell Sheet.B3>, <Cell Sheet.C3>),
(<Cell Sheet.A4>, <Cell Sheet.B4>, <Cell Sheet.C4>),
(<Cell Sheet.A5>, <Cell Sheet.B5>, <Cell Sheet.C5>),
(<Cell Sheet.A6>, <Cell Sheet.B6>, <Cell Sheet.C6>),
(<Cell Sheet.A7>, <Cell Sheet.B7>, <Cell Sheet.C7>),
(<Cell Sheet.A8>, <Cell Sheet.B8>, <Cell Sheet.C8>),
(<Cell Sheet.A9>, <Cell Sheet.B9>, <Cell Sheet.C9>))
~~~



或`Worksheet.columns`属性

```python
>>> tuple(ws.columns)
((<Cell Sheet.A1>,
<Cell Sheet.A2>,
<Cell Sheet.A3>,
<Cell Sheet.A4>,
<Cell Sheet.A5>,
<Cell Sheet.A6>,
...
<Cell Sheet.B7>,
<Cell Sheet.B8>,
<Cell Sheet.B9>),
(<Cell Sheet.C1>,
<Cell Sheet.C2>,
<Cell Sheet.C3>,
<Cell Sheet.C4>,
<Cell Sheet.C5>,
<Cell Sheet.C6>,
<Cell Sheet.C7>,
<Cell Sheet.C8>,
<Cell Sheet.C9>))
```

> <b>Note：</b>
>  处于性能原因，`Worksheet.columns`属性不支持只读模式



### 取值

 如果只需要从工作表中获取值，可以使用`Worksheet.values`属性。这将遍历工作表中所有行，但只返回单元格值：

~~~ python
for row in ws.values:
   for value in row:
     print(value)
~~~



`Worksheet.iter_rows()`和`Worksheet.iter_cols()`可只返回单元格值：

~~~ python
>>> for row in ws.iter_rows(min_row=1, max_col=3, max_row=2, values_only=True):
...   print(row)
(None, None, None)
(None, None, None)
~~~



### 赋值

 当创建一单元格对象则可对其赋值：

```ruby
>>> c.value = 'hello, world'
>>> print(c.value)
'hello, world'

>>> d.value = 3.14
>>> print(d.value)
3.14
```



### 保存

 对`Workbook`对象用`Workbook.save()` 方法可简单安全保存工作簿：

```ruby
>>> wb = Workbook()
>>> wb.save('balances.xlsx')
```



> <b>Warning：</b>
> 该操作将覆盖同名文件，而不会有任何警告
>
> 
>
> <b>Note：</b>
>  文件扩展名不强制为xlsx或xlsm，如果你没有使用常用的扩展名，在使用其他应用打开该文件时可能存在一些异常。
>
> 
>
> 因为OOXML文件是基于zip文件，你也可以使用常用的解压软件打开。



### 流方式存储

若需通过流方式存储文件，如用web应用如Pyramid,Flask或Django则使用`NamedTemporaryFile()`方法：

```python
>>> wb = load_workbook('document.xlsx')
>>> wb.template = True
>>> wb.save('document_template.xltx')
```

或设这对象为False：

```ruby
>>> wb = load_workbook('document_template.xltx')
>>> wb.template = False
>>> wb.save('document.xlsx', as_template=False)
```

> **Warning**
> 监控文档模板中保存文档的数据属性和文档扩展名反之也是，否则结果表引擎将无法打开文档

------



> <b>Note</b>
>  The following will fail:

```ruby
>>> wb = load_workbook('document.xlsx')
>>> # Need to save with the extension *.xlsx
>>> wb.save('new_document.xlsm')
>>> # MS Excel can't open the document
>>>
>>> # or
>>>
>>> # Need specify attribute keep_vba=True
>>> wb = load_workbook('document.xlsm')
>>> wb.save('new_document.xlsm')
>>> # MS Excel will not open the document
>>>
>>> # or
>>>
>>> wb = load_workbook('document.xltm', keep_vba=True)
>>> # If we need a template document, then we must specify extension as *.xltm.
>>> wb.save('new_document.xlsm')
>>> # MS Excel will not open the document
```



### 读取文件

和写操作一样可使用`openpyxl.load_workbook()`打开存在的工作簿：

```python
>>> from openpyxl import load_workbook
>>> wb2 = load_workbook('test.xlsx')
>>> print wb2.sheetnames
['Sheet2', 'New Title', 'Sheet1']
```



### Excel 读写及追加写入

首先贴出四种方法适用范围比较：

|        | XlsxWriter     |                xlrd                |                 xlwt                  |       openpyxl       |
| :----- | :------------- | :--------------------------------: | :-----------------------------------: | :------------------: |
| 介绍   | 可创建XLSX文件 | 用于读取xls文件，是excel的三大模块 | 用于写xls文件，是python-excel三大模块 | 可读写XLSX、XLSM文件 |
| 读     | ×              |                 √                  |                   ×                   |          √           |
| 写     | √              |                 ×                  |                   √                   |          √           |
| xls    | ×              |                 √                  |                   √                   |          ×           |
| xlsx   | √              |                 ×                  |                   ×                   |          √           |
| 大文件 | √              |                 ×                  |                   ×                   |          √           |

> 注释：Excel 2003 即XLS文件有大小限制即65536行256列，所以不支持大文件，而Excel 2007以上即XLSX文件的限制则为1048576行16384列



下面是几种模块的使用：

#### 1. xlwt 写入xls文件内容

~~~ python
import xlwt
 
book = xlwt.Workbook() # 新建工作簿
table = book.add_sheet('Over',cell_overwrite_ok=True) # 如果对同一单元格重复操作会发生overwrite Exception，cell_overwrite_ok为可覆盖
sheet = book.add_sheet('Test') # 添加工作页
sheet.write(1,1,'A') # 行，列，属性值 (1,1)为B2元素，从0开始计数
style = xlwt.XFStyle() # 新建样式
font = xlwt.Font() #新建字体
font.name = 'Times New Roman'
font.bold = True
style.font = font # 将style的字体设置为font
table.write(0,0,'Test',style)
book.save(filename_or_stream='excel_test.xls') # 一定要保存
~~~



#### 2. xlrd读取xls文件内容

~~~ python
import xlrd 
data = xlrd.open_workbook('excel_test.xls')
print(data.sheet_names()) # 输出所有页的名称
table = data.sheets()[0] # 获取第一页
table = data.sheet_by_index(0) # 通过索引获得第一页
table = data.sheet_by_name('Over') # 通过名称来获取指定页
nrows = table.nrows # 为行数，整形
ncolumns = table.ncols # 为列数，整形
print(type(nrows))
print(table.row_values(0))# 输出第一行值 为一个列表
# 遍历输出所有行值
for row in range(nrows):
    print(table.row_values(row))
# 输出某一个单元格值
print(table.cell(0,0).value)
print(table.row(0)[0].value)
~~~



#### 3. 综合用python-excel三大模块完成Excel内容追加写入

```python
import xlwt,xlrd
from xlutils.copy import copy
data = xlrd.open_workbook('excel_test.xls',formatting_info=True)
excel = copy(wb=data) # 完成xlrd对象向xlwt对象转换
excel_table = excel.get_sheet(0) # 获得要操作的页
table = data.sheets()[0]
nrows = table.nrows # 获得行数
ncols = table.ncols # 获得列数
values = ["E","X","C","E","L"] # 需要写入的值
for value in values:
    excel_table.write(nrows,1,value) # 因为单元格从0开始算，所以row不需要加一
    nrows = nrows+1
excel.save('excel_test.xls')
```



#### 4. 用 openpyxl 写xlsx文件

~~~ python
import openpyxl
data = openpyxl.Workbook() # 新建工作簿
data.create_sheet('Sheet1') # 添加页
#table = data.get_sheet_by_name('Sheet1') # 获得指定名称页
table = data.active # 获得当前活跃的工作页，默认为第一个工作页
table.cell(1,1,'Test') # 行，列，值 这里是从1开始计数的
data.save('excel_test.xlsx') # 一定要保存
~~~



#### 5. 用openpyxl读取xlsx文件

~~~ python
import openpyxl
data = openpyxl.load_workbook('excel_test.xlsx') # 读取xlsx文件
table = data.get_sheet_by_name('Sheet') # 获得指定名称的页
nrows = table.rows # 获得行数 类型为迭代器
ncols = table.columns # 获得列数 类型为迭代器
print(type(nrows))
for row in nrows:
    print(row) # 包含了页名，cell，值
    line = [col.value for col in row] # 取值
    print(line)
# 读取单元格
print(table.cell(1,1).value)
~~~



#### 6. 用 openpyxl 对excel内容追加写入

~~~ python
import openpyxl
data = openpyxl.load_workbook('excel_test.xlsx')
print(data.get_named_ranges()) # 输出工作页索引范围
print(data.get_sheet_names()) # 输出所有工作页的名称
# 取第一张表
sheetnames = data.get_sheet_names()
table = data.get_sheet_by_name(sheetnames[0])
table = data.active
print(table.title) # 输出表名
nrows = table.max_row # 获得行数
ncolumns = table.max_column # 获得行数
values = ['E','X','C','E','L']
for value in values:
    table.cell(nrows+1,1).value = value
    nrows = nrows + 1
data.save('excel_test.xlsx')
~~~





#### 7. XlsxWriter

~~~ python
# !/usr/bin/python
# coding:utf-8
# xlsxwriter的基本用法
import xlsxwriter
 
# 1. 创建一个Excel文件
workbook = xlsxwriter.Workbook('demo1.xlsx')
 
# 2. 创建一个工作表sheet对象
worksheet = workbook.add_worksheet()
 
# 3. 设定第一列（A）宽度为20像素
worksheet.set_column('A:A',20)
 
# 4. 定义一个加粗的格式对象
bold = workbook.add_format({'bold':True})
 
# 5. 向单元格写入数据
# 5.1 向A1单元格写入'Hello'
worksheet.write('A1','Hello')
# 5.2 向A2单元格写入'World'并使用bold加粗格式
worksheet.write('A2','World',bold)
# 5.3 向B2单元格写入中文并使用加粗格式
worksheet.write('B2',u'中文字符',bold)
 
# 5.4 用行列表示法（行列索引都从0开始）向第2行、第0列（即A3单元格）和第3行、第0列（即A4单元格）写入数字
worksheet.write(2,0,10)
worksheet.write(3,0,20)
 
# 5.5 求A3、A4单元格的和并写入A5单元格，由此可见可以直接使用公式
worksheet.write(4,0,'=SUM(A3:A4)')
 
# 5.6 在B5单元格插入图片
worksheet.insert_image('B5','./demo.png')
 
# 5.7 关闭并保存文件
workbook.close()
~~~

