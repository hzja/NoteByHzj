Python
<a name="giLGF"></a>
### 打开Excel文件
新建一个Excel文件
```python
>>> from openpyxl import Workbook
>>> wb = Workbook()
```
打开现有Excel文件
```python
>>> from openpyxl import load_workbook
>>> wb2 = load_workbook('test.xlsx')
```
打开大文件时，根据需求使用只读或只写模式减少内存消耗。
```python
wb = load_workbook(filename='large_file.xlsx', read_only=True)

wb = Workbook(write_only=True)
```
<a name="qXa8I"></a>
### 获取、创建工作表
获取当前活动工作表：
```python
>>> ws = wb.active
```
创建新的工作表：
```python
>>> ws1 = wb.create_sheet("Mysheet") # insert at the end (default)
# or
>>> ws2 = wb.create_sheet("Mysheet", 0) # insert at first position
# or
>>> ws3 = wb.create_sheet("Mysheet", -1) # insert at the penultimate position
```
使用工作表名字获取工作表：
```python
>>> ws3 = wb["New Title"]
```
获取所有的工作表名称：
```python
>>> print(wb.sheetnames)
['Sheet2', 'New Title', 'Sheet1']
```
使用for循环遍历所有的工作表：
```python
>>> for sheet in wb:
...     print(sheet.title)
```
<a name="barV8"></a>
### 保存
<a name="RgKOd"></a>
#### 保存到流中在网络中使用：
```python
>>> from tempfile import NamedTemporaryFile
>>> from openpyxl import Workbook
>>> wb = Workbook()
>>> with NamedTemporaryFile() as tmp:
    wb.save(tmp.name)
    tmp.seek(0)
    stream = tmp.read()
```
<a name="mMHxR"></a>
#### 保存到文件：
```python
>>> wb = Workbook()
>>> wb.save('balances.xlsx')
```
<a name="HI6DM"></a>
#### 保存为模板：
```python
>>> wb = load_workbook('document.xlsx')
>>> wb.template = True
>>> wb.save('document_template.xltx')
```
<a name="fIcgZ"></a>
### 单元格
单元格位置作为工作表的键直接读取：
```python
>>> c = ws['A4']
```
为单元格赋值：
```python
>>> ws['A4'] = 4
>>> c.value = 'hello, world'
```
多个单元格可以使用切片访问单元格区域：
```python
>>> cell_range = ws['A1':'C2']
```
使用数值格式：
```python
>>> # set date using a Python datetime
>>> ws['A1'] = datetime.datetime(2010, 7, 21)
>>>
>>> ws['A1'].number_format
'yyyy-mm-dd h:mm:ss'
```
使用公式：
```python
>>> # add a simple formula
>>> ws["A1"] = "=SUM(1, 1)"
```
合并单元格时，除左上角单元格外，所有单元格都将从工作表中删除：
```python
>>> ws.merge_cells('A2:D2')
>>> ws.unmerge_cells('A2:D2')
>>>
>>> # or equivalently
>>> ws.merge_cells(start_row=2, start_column=1, end_row=4, end_column=4)
>>> ws.unmerge_cells(start_row=2, start_column=1, end_row=4, end_column=4) 
```
<a name="e1kDV"></a>
### 行、列
可以单独指定行、列、或者行列的范围：
```python
>>> colC = ws['C']
>>> col_range = ws['C:D']
>>> row10 = ws[10]
>>> row_range = ws[5:10]
```
可以使用`Worksheet.iter_rows()`方法遍历行：
```python
>>> for row in ws.iter_rows(min_row=1, max_col=3, max_row=2):
...    for cell in row:
...        print(cell)
<Cell Sheet1.A1>
<Cell Sheet1.B1>
<Cell Sheet1.C1>
<Cell Sheet1.A2>
<Cell Sheet1.B2>
<Cell Sheet1.C2>
```
同样的`Worksheet.iter_cols()`方法将遍历列：
```python
>>> for col in ws.iter_cols(min_row=1, max_col=3, max_row=2):
...     for cell in col:
...         print(cell)
<Cell Sheet1.A1>
<Cell Sheet1.A2>
<Cell Sheet1.B1>
<Cell Sheet1.B2>
<Cell Sheet1.C1>
<Cell Sheet1.C2>
```
遍历文件的所有行或列，可以使用`Worksheet.rows`属性：
```python
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
```
或`Worksheet.columns`属性：
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
使用`Worksheet.append()`或者迭代使用`Worksheet.cell()`新增一行数据：
```python
>>> for row in range(1, 40):
...     ws1.append(range(600))

>>> for row in range(10, 20):
...     for col in range(27, 54):
...         _ = ws3.cell(column=col, row=row, value="{0}".format(get_column_letter(col)))
```
插入操作比较麻烦。可以使用`Worksheet.insert_rows()`插入一行或几行：
```python
>>> from openpyxl.utils import get_column_letter
>>> ws.insert_rows(7) 
>>> row7 = ws[7]
>>> for col in range(27, 54):
...         _ = ws3.cell(column=col, row=7, value="{0}".format(get_column_letter(col)))
```
`Worksheet.insert_cols()`操作类似。`Worksheet.delete_rows()`和`Worksheet.delete_cols()`用来批量删除行和列。
<a name="iMYcK"></a>
### 只读取值
使用`Worksheet.values`属性遍历工作表中的所有行，但只返回单元格值：
```python
for row in ws.values:
    for value in row:
        print(value)
```
`Worksheet.iter_rows()`和`Worksheet.iter_cols()`可以设置`values_only`参数来仅返回单元格的值：
```python
>>> for row in ws.iter_rows(min_row=1, max_col=3, max_row=2, values_only=True):
...   print(row)
(None, None, None)
(None, None, None)
```
