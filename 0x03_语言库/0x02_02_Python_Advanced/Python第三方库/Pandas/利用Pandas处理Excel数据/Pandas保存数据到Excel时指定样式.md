Pandas Excel
<a name="m8JNG"></a>
## 准备数据
```python
import pandas as pd
from datetime import datetime, date
df = pd.DataFrame({'Date and time': [datetime(2015, 1, 1, 11, 30, 55),
                                     datetime(2015, 1, 2, 1, 20, 33),
                                     datetime(2015, 1, 3, 11, 10),
                                     datetime(2015, 1, 4, 16, 45, 35),
                                     datetime(2015, 1, 5, 12, 10, 15)],
                   'Dates only': [date(2015, 2, 1),
                                  date(2015, 2, 2),
                                  date(2015, 2, 3),
                                  date(2015, 2, 4),
                                  date(2015, 2, 5)],
                   'Numbers': [1010, 2020, 3030, 2020, 1515],
                   'Percentage': [.1, .2, .33, .25, .5],
                   })
df['final'] = [f"=C{i}*D{i}" for i in range(2, df.shape[0]+2)]
df
```
结果：

| <br /> | Date and time | Dates only | Numbers | Percentage | final |
| --- | --- | --- | --- | --- | --- |
| 0 | 2015/1/1 11:30 | 2015/2/1 | 1010 | 0.1 | =C2*D2 |
| 1 | 2015/1/2 1:20 | 2015/2/2 | 2020 | 0.2 | =C3*D3 |
| 2 | 2015/1/3 11:10 | 2015/2/3 | 3030 | 0.33 | =C4*D4 |
| 3 | 2015/1/4 16:45 | 2015/2/4 | 2020 | 0.25 | =C5*D5 |
| 4 | 2015/1/5 12:10 | 2015/2/5 | 1515 | 0.5 | =C6*D6 |

<a name="jozOp"></a>
## Pandas直接保存数据
对于这个pandas对象，如果需要将其保存为excel，最简单的，直接保存：
```python
df.to_excel("demo1.xlsx", sheet_name='Sheet1', index=False)
```
效果如下：<br />但如果想要给这个excel在保存时，同时指定一些特殊的自定义格式又该怎么做呢？这时就可以使用ExcelWriter进行操作，查看API文档发现两个重要参数：

- `date_format : str, default None`<br />Format string for dates written into Excel files (e.g. 'YYYY-MM-DD').
- `datetime_format : str, default None`<br />Format string for datetime objects written into Excel files. (e.g. 'YYYY-MM-DD HH:MM:SS').

这说明对于日期类型数据，都可以通过这两个参数指定特定的显示格式，那么采用以下方式才创建ExcelWriter，并保存结果：
```python
writer = pd.ExcelWriter("demo1.xlsx",
                        datetime_format='mmm d yyyy hh:mm:ss',
                        date_format='mmmm dd yyyy')
df.to_excel(writer, sheet_name='Sheet1', index=False)
writer.save()
```
可以看到excel保存的结果中，格式已经确实的发生了改变。
<a name="xqGnQ"></a>
## Pandas的Styler对表格着色输出
如果想对指定的列的数据设置文字颜色或背景色，可以直接`pandas.io.formats.style`工具，该工具可以直接对指定列用指定的规则着色：
```python
df_style = df.style.applymap(lambda x: 'color:red', subset=["Date and time"]) \
    .applymap(lambda x: 'color:green', subset=["Dates only"]) \
    .applymap(lambda x: 'background-color:#ADD8E6', subset=["Numbers"]) \
    .background_gradient(cmap="PuBu", low=0, high=0.5, subset=["Percentage"])
writer = pd.ExcelWriter("demo_style.xlsx",
                        datetime_format='mmm d yyyy hh:mm:ss',
                        date_format='mmmm dd yyyy')
df_style.to_excel(writer, sheet_name='Sheet1', index=False)
writer.save()
```

虽然Pandas的Styler样式还包括设置显示格式、条形图等功能，但写入到excel却无效，所以只能借助Pandas的Styler实现作色的功能，而且只能对数据着色，不能对表头作色。
<a name="fvPjT"></a>
## Pandas使用xlsxwriter引擎保存数据
进一步的，需要将数值等其他类型的数据也修改一下显示格式，这时就需要从ExcelWriter拿出其中的workbook进行操作：
```python
writer = pd.ExcelWriter("demo1.xlsx")
workbook = writer.book
workbook
```
结果：
```
<xlsxwriter.workbook.Workbook at 0x52fde10>
```
从返回的结果可以看到这是一个xlsxwriter对象，说明pandas默认的excel写出引擎是xlsxwriter，即上面的ExcelWriter创建代码其实等价于：
```python
pd.ExcelWriter("demo1.xlsx", engine='xlsxwriter')
```
关于xlsxwriter可以参考官方文档：[https://xlsxwriter.readthedocs.org/](https://xlsxwriter.readthedocs.org/)<br />下面的代码即可给数值列设置特定的格式：
```python
writer = pd.ExcelWriter("demo1.xlsx",
                        engine='xlsxwriter',
                        datetime_format='mmm d yyyy hh:mm:ss',
                        date_format='mmmm dd yyyy')
df.to_excel(writer, sheet_name='Sheet1', index=False)
workbook = writer.book
worksheet = writer.sheets['Sheet1']
worksheet.set_column('A:A', 19)
worksheet.set_column('B:B', 17)
format1 = workbook.add_format({'num_format': '#,##0.00'})
format2 = workbook.add_format({'num_format': '0%'})
worksheet.set_column('C:C', 8, format1)
worksheet.set_column('D:D', 11, format2)
worksheet.set_column('E:E', 6, format1)
writer.save()
```
关于set_column方法：<br />`worksheet.set_column('C:C', 8, format1)` 表示将C列的列宽设置为8个字符，并采用format1的样式，当然 `format1 = workbook.add_format({'num_format': '#,##0.00'})` 就是表示生成一个指定的格式对象。
<a name="iKiam"></a>
### xlsxwriter按照指定样式写出Pandas对象的数据
假如现在希望能够定制excel表头的样式，并给数据添加边框。xlsxwriter的API文档没有一个可以修改指定范围样式的API，要修改样式只能通过set_column修改列，或者通过set_row修改行，这种形式的修改都是针对整行和整列，对于显示格式还能满足条件，但对于背景色和边框之类的样式就不行了，这点上确实不如openpyxl方便，但xlsxwriter还有个优势，就是写出数据时可以直接指定样式。<br />下面看看如何直接通过xlsxwriter保存指定样式的数据：
```python
import xlsxwriter
workbook = xlsxwriter.Workbook('demo2.xlsx')
worksheet = workbook.add_worksheet('sheet1')
# 创建列名的样式
header_format = workbook.add_format({
    'bold': True,
    'text_wrap': True,
    'valign': 'top',
    'fg_color': '#D7E4BC',
    'border': 1})
# 从A1单元格开始写出一行数据，指定样式为header_format
worksheet.write_row(0, 0,  df.columns, header_format)
# 创建一批样式对象
format1 = workbook.add_format({'border': 1, 'num_format': 'mmm d yyyy hh:mm:ss'})
format2 = workbook.add_format({'border': 1, 'num_format': 'mmmm dd yyyy'})
format3 = workbook.add_format({'border': 1, 'num_format': '#,##0.00'})
format4 = workbook.add_format({'border': 1, 'num_format': '0%'})
# 从第2行（角标从0开始）开始，分别写出每列的数据，并指定特定的样式
worksheet.write_column(1, 0, df.iloc[:, 0], format1)
worksheet.write_column(1, 1, df.iloc[:, 1], format2)
worksheet.write_column(1, 2, df.iloc[:, 2], format3)
worksheet.write_column(1, 3, df.iloc[:, 3], format4)
worksheet.write_column(1, 4, df.iloc[:, 4], format3)
# 设置对应列的列宽，单位是字符长度
worksheet.set_column('A:A', 19)
worksheet.set_column('B:B', 17)
worksheet.set_column('C:C', 8)
worksheet.set_column('D:D', 12)
worksheet.set_column('E:E', 6)
workbook.close()
```
上面的代码应该都比较好理解，`header_format`和`formatN`是创建的样式对象，`write_row`用于按行写出数据，`write_column`用于按列写出数据，`set_column`则是用于设置整列的列宽和样式（没传入的不设置）。
<a name="e8as1"></a>
## Pandas使用openpyxl引擎保存数据
pandas的默认写出引擎是xlsxwriter，那么是不是可以修改为其他引擎呢？答案是可以，下面使用 openpyxl 实现同样的效果。<br />关于openpyxl可参考：[https://openpyxl.readthedocs.org/](https://openpyxl.readthedocs.org/)
```python
writer = pd.ExcelWriter("demo3.xlsx",
                        engine='openpyxl',
                        datetime_format='mmm d yyyy hh:mm:ss',
                        date_format='mmmm dd yyyy')
df.to_excel(writer, sheet_name='Sheet1', index=False)
workbook = writer.book
worksheet = writer.sheets['Sheet1']
workbook
```
结果：
```
<openpyxl.workbook.workbook.Workbook at 0x16c6bb70>
```
从上述打印结果可以看到，从engine指定为'openpyxl'后，workbook已经是 openpyxl 对象了。<br />与 xlsxwriter 不同的是 openpyxl 只能对逐个单元格设置样式，而xlsxwriter只能指定行或指定列或写入数据指定样式。<br />下面首先修改表头的样式：
```python
import itertools
from openpyxl.styles import Alignment, Font, PatternFill, Border, Side, PatternFill
font = Font(name="微软雅黑", bold=True)
alignment = Alignment(vertical="top", wrap_text=True)
pattern_fill = PatternFill(fill_type="solid", fgColor="D7E4BC")
side = Side(style="thin")
border = Border(left=side, right=side, top=side, bottom=side)
for cell in itertools.chain(*worksheet["A1:E1"]):
    cell.font = font
    cell.alignment = alignment
    cell.fill = pattern_fill
    cell.border = border
```
上述代码引入的了itertools.chain方便迭代出每个单元格，而不用写多重for循环。<br />下面再修改数值列的格式：
```python
for cell in itertools.chain(*worksheet["A2:E6"]):
    cell.border = border
for cell in itertools.chain(*worksheet["C2:C6"], *worksheet["E2:E6"]):
    cell.number_format = '#,##0.00'
for cell in itertools.chain(*worksheet["D2:D6"]):
    cell.number_format = '0%'
```
最后给各列设置一下列宽：
```python
worksheet.column_dimensions["A"].width = 20
worksheet.column_dimensions["B"].width = 17
worksheet.column_dimensions["C"].width = 10
worksheet.column_dimensions["D"].width = 12
worksheet.column_dimensions["E"].width = 8
```
最后保存即可：
```python
writer.save()
```
整体完整代码：
```python
from openpyxl.styles import Alignment, Font, PatternFill, Border, Side, PatternFill
import itertools
writer = pd.ExcelWriter("demo3.xlsx",
                        engine='openpyxl',
                        datetime_format='mmm d yyyy hh:mm:ss',
                        date_format='mmmm dd yyyy')
df.to_excel(writer, sheet_name='Sheet1', index=False)
workbook = writer.book
worksheet = writer.sheets['Sheet1']
font = Font(name="微软雅黑", bold=True)
alignment = Alignment(vertical="top", wrap_text=True)
pattern_fill = PatternFill(fill_type="solid", fgColor="D7E4BC")
side = Side(style="thin")
border = Border(left=side, right=side, top=side, bottom=side)
for cell in itertools.chain(*worksheet["A1:E1"]):
    cell.font = font
    cell.alignment = alignment
    cell.fill = pattern_fill
    cell.border = border
for cell in itertools.chain(*worksheet["A2:E6"]):
    cell.border = border
for cell in itertools.chain(*worksheet["C2:C6"], *worksheet["E2:E6"]):
    cell.number_format = '#,##0.00'
for cell in itertools.chain(*worksheet["D2:D6"]):
    cell.number_format = '0%'
worksheet.column_dimensions["A"].width = 20
worksheet.column_dimensions["B"].width = 17
worksheet.column_dimensions["C"].width = 10
worksheet.column_dimensions["D"].width = 12
worksheet.column_dimensions["E"].width = 8
writer.save()
```
从上述代码可以看到，openpyxl 修改样式时相对xlsxwriter来说麻烦了太多，特别是修改一个表头就需要创建5个对象。
<a name="2584a35d"></a>
### openpyxl加载数据模板写出Pandas对象的数据
虽然 openpyxl 直接写出数据指定样式相对xlsxwriter麻烦，但 openpyxl 还有个巨大的优势就是可以读取已有的excel文件，在其基础上修改。<br />那就完全可以先将模板数据写入到一个excel，然后加载这个模板文件进行修改，所以上面那个固定不变的表头，就可以事先创建好：然后加载模板，再写入：<br />（经测试ExcelWriter无法对已经存在的工作表进行操作，会创建新的工作表，所以这里直接使用openpyxl自己的API）
```python
from openpyxl import load_workbook
workbook = load_workbook('template.xlsx')
worksheet = workbook["Sheet1"]
# 添加数据列，i表示当前的行号，用于后续格式设置
for i, row in enumerate(df.values, 2):
    worksheet.append(row.tolist())
# 批量修改给写入的数据的单元格范围加边框
side = Side(style="thin")
border = Border(left=side, right=side, top=side, bottom=side)
for cell in itertools.chain(*worksheet[f"A2:E{i}"]):
    cell.border = border
    
# 批量给各列设置指定的自定义格式
for cell in itertools.chain(*worksheet[f"A2:A{i}"]):
    cell.number_format = 'mmm d yyyy hh:mm:ss'
for cell in itertools.chain(*worksheet[f"B2:B{i}"]):
    cell.number_format = 'mmmm dd yyyy'
for cell in itertools.chain(*worksheet[f"C2:C{i}"], *worksheet[f"E2:E{i}"]):
    cell.number_format = '#,##0.00'
for cell in itertools.chain(*worksheet[f"D2:D{i}"]):
    cell.number_format = '0%'
workbook.save(filename="demo4.xlsx")
```
可以明显的看到openpyxl在加载模板后，可以省掉表头设置和列宽设置的代码。
<a name="wvFKv"></a>
## Pandas自适应列宽保存数据
大多数时候并不需要设置自定义样式，也不需要写出公式字符串，而是直接写出最终的结果文本，这时就可以使用pandas计算一下各列的列宽再保存excel数据。<br />例如有如下数据：
```python
df = pd.DataFrame({
    'Region': ['East', 'East', 'South', 'North', 'West', 'South', 'North', 'West', 'West', 'South', 'West', 'South'],
    'Item': ['Apple', 'Apple', 'Orange', 'Apple', 'Apple', 'Pear', 'Pear', 'Orange', 'Grape', 'Pear', 'Grape',
             'Orange'],
    'Volume': [9000, 5000, 9000, 2000, 9000, 7000, 9000, 1000, 1000, 10000, 6000, 3000],
    'Month': ['July', 'July', 'September', 'November', 'November', 'October', 'August', 'December', 'November', 'April',
              'January', 'May']
})
df
```
结果：

| <br /> | Region | Item | Volume | Month |
| --- | --- | --- | --- | --- |
| 0 | East | Apple | 9000 | July |
| 1 | East | Apple | 5000 | July |
| 2 | South | Orange | 9000 | September |
| 3 | North | Apple | 2000 | November |
| 4 | West | Apple | 9000 | November |
| 5 | South | Pear | 7000 | October |
| 6 | North | Pear | 9000 | August |
| 7 | West | Orange | 1000 | December |
| 8 | West | Grape | 1000 | November |
| 9 | South | Pear | 10000 | April |
| 10 | West | Grape | 6000 | January |
| 11 | South | Orange | 3000 | May |

使用pandas来进行计算各列列宽，思路是计算出每列的字符串gbk编码（Windows下的Excel软件默认使用gbk编码）后的最大字节长度：
```python
#  计算表头的字符宽度
column_widths = (
    df.columns.to_series()
    .apply(lambda x: len(x.encode('gbk'))).values
)
#  计算每列的最大字符宽度
max_widths = (
    df.astype(str)
    .applymap(lambda x: len(x.encode('gbk')))
    .agg(max).values
)
# 计算整体最大宽度
widths = np.max([column_widths, max_widths], axis=0)
widths
```
结果：
```python
array([6, 6, 6, 9], dtype=int64)
```
下面将改造一下前面的代码。<br />首先，使用xlsxwriter引擎自适应列宽保存数据：
```python
writer = pd.ExcelWriter("auto_column_width1.xlsx", engine='xlsxwriter')
df.to_excel(writer, sheet_name='Sheet1', index=False)
worksheet = writer.sheets['Sheet1']
for i, width in enumerate(widths):
    worksheet.set_column(i, i, width)
writer.save()
```
然后，使用openpyxl引擎自适应列宽保存数据(openpyxl引擎设置字符宽度时会缩水0.5左右个字符，所以干脆+1)：
```python
from openpyxl.utils import get_column_letter
writer = pd.ExcelWriter("auto_column_width2.xlsx", engine='openpyxl')
df.to_excel(writer, sheet_name='Sheet1', index=False)
worksheet = writer.sheets['Sheet1']
for i, width in enumerate(widths, 1):
    worksheet.column_dimensions[get_column_letter(i)].width = width+1
writer.save()
```
可以看到列宽设置的都比较准确。
