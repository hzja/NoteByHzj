Python 
<a name="HO5Cj"></a>
## Python Excel库对比
先来看一下python中能操作Excel的库对比（一共九个库）：<br />![2021-11-03-21-42-15-697571.jpeg](./img/1635947018854-e6be9c87-74bd-42cc-8c63-ce2f20494a54.jpeg)
<a name="AN3ob"></a>
## 1、Python xlrd 读取 操作Excel
<a name="ayFW4"></a>
### 1.1 xlrd模块介绍
（1）什么是xlrd模块？

- python操作excel主要用到xlrd和xlwt这两个库，即xlrd是读excel，xlwt是写excel的库。

（2）为什么使用xlrd模块？

- 在UI自动化或者接口自动化中数据维护是一个核心，所以此模块非常实用。

xlrd模块可以用于读取Excel的数据，速度非常快，推荐使用！<br />官方文档：[https://xlrd.readthedocs.io/en/latest/](https://xlrd.readthedocs.io/en/latest/)
<a name="jAElP"></a>
### 1.2 安装xlrd模块

- 到python官网下载[http://pypi.python.org/pypi/xlrd](http://pypi.python.org/pypi/xlrd)模块安装，前提是已经安装了python 环境。
- 或者在cmd窗口 `pip install xlrd`
```bash
pip install xlrd
```
这里是anaconda自带有xlrd，所以提示已经安装：<br />![](./img/1635944853175-b7309bfc-2379-470e-b755-ee54303639cd.webp)
<a name="MV82M"></a>
### 1.3 使用介绍
<a name="gtH7S"></a>
#### 常用单元格的数据类型

- empty（空的）
- string（text）
- number
- date
- boolean
- error
- blank（空白表格）
<a name="Gvx9U"></a>
#### 导入模块
```python
import xlrd
```
<a name="e1pI2"></a>
#### 打开Excel文件读取数据
```python
data = xlrd.open_workbook(filename)#文件名以及路径，如果路径或者文件名有中文给前面加一个 r
```
<a name="gsKvl"></a>
#### 常用的函数

- excel中最重要的方法就是book和sheet的操作
<a name="xeQOK"></a>
##### （1）获取book（excel文件）中一个工作表
```python
table = data.sheets()[0]             #通过索引顺序获取
table = data.sheet_by_index(sheet_indx)  #通过索引顺序获取
table = data.sheet_by_name(sheet_name)  #通过名称获取

# 以上三个函数都会返回一个xlrd.sheet.Sheet()对象

names = data.sheet_names()        #返回book中所有工作表的名字
data.sheet_loaded(sheet_name or indx)    # 检查某个sheet是否导入完毕
```
<a name="hIzqv"></a>
##### （2） 行的操作
```python
nrows = table.nrows
# 获取该sheet中的行数，注，这里table.nrows后面不带().

table.row(rowx)
# 返回由该行中所有的单元格对象组成的列表,这与tabel.raw()方法并没有区别。

table.row_slice(rowx)
# 返回由该行中所有的单元格对象组成的列表

table.row_types(rowx, start_colx=0, end_colx=None)
# 返回由该行中所有单元格的数据类型组成的列表；    
# 返回值为逻辑值列表，若类型为empy则为0，否则为1

table.row_values(rowx, start_colx=0, end_colx=None)
# 返回由该行中所有单元格的数据组成的列表

table.row_len(rowx)
# 返回该行的有效单元格长度，即这一行有多少个数据
```
![](./img/1635944853093-ee67f9af-671a-4081-b21c-1608536438c6.webp)
<a name="EBfLT"></a>
##### （3）列（colnum）的操作
```python
ncols = table.ncols
# 获取列表的有效列数

table.col(colx, start_rowx=0, end_rowx=None)
# 返回由该列中所有的单元格对象组成的列表

table.col_slice(colx, start_rowx=0, end_rowx=None)
# 返回由该列中所有的单元格对象组成的列表

table.col_types(colx, start_rowx=0, end_rowx=None)
# 返回由该列中所有单元格的数据类型组成的列表

table.col_values(colx, start_rowx=0, end_rowx=None)
# 返回由该列中所有单元格的数据组成的列表
```
![](./img/1635944853068-fb51510b-becb-41f9-b4b1-84ec906662d2.webp)
<a name="i8Br3"></a>
##### （4）单元格的操作
```python
table.cell(rowx,colx)
# 返回单元格对象

table.cell_type(rowx,colx)
# 返回对应位置单元格中的数据类型

table.cell_value(rowx,colx)
# 返回对应位置单元格中的数据
```
<a name="qiOUe"></a>
### 1.4 实战训练
先在表格放入以下数据，点击保存：<br />![](./img/1635944853175-84ee5da7-d698-480a-9ef1-56bf9f634333.webp)<br />使用xlrd模块进行读取：
```python
import xlrd

xlsx = xlrd.open_workbook('./3_1 xlrd 读取 操作练习.xlsx')

# 通过sheet名查找：xlsx.sheet_by_name("sheet1")
# 通过索引查找：xlsx.sheet_by_index(3)
table = xlsx.sheet_by_index(0)

# 获取单个表格值 (2,1)表示获取第3行第2列单元格的值
value = table.cell_value(2, 1)
print("第3行2列值为",value)

# 获取表格行数
nrows = table.nrows
print("表格一共有",nrows,"行")

# 获取第4列所有值（列表生成式）
name_list = [str(table.cell_value(i, 3)) for i in range(1, nrows)]
print("第4列所有的值：",name_list)
```
打印结果：<br />![](./img/1635944853466-67c59b1f-bceb-49e1-b42b-9ad481c8b8ae.webp)<br />列表生成式介绍：<br />![](./img/1635944853569-97907287-f55b-4997-97ee-df975a516637.webp)
<a name="IUTMe"></a>
## 2、Python xlwt 写入 操作Excel（仅限xls格式！）
xlwt可以用于写入新的Excel表格或者在原表格基础上进行修改，速度也很快，推荐使用！<br />官方文档：[https://xlwt.readthedocs.io/en/latest/](https://xlwt.readthedocs.io/en/latest/)
<a name="bY5ks"></a>
### 2.1 pip安装xlwt
```bash
pip install xlwt
```
这里是anaconda自带有xlwt，所以提示已经安装：<br />![](./img/1635944853558-0f8e6ed7-d62c-4300-ace2-34e4ef913474.webp)
<a name="BivhJ"></a>
### 2.2 使用xlwt创建新表格并写入
一开始目录下只有这两个文件：<br />![](./img/1635944853739-7f603d82-d398-453d-bac3-24306d36a0b4.webp)<br />编写xlwt新表格写入程序:
```python
# 3.2.2 使用xlwt创建新表格并写入
def fun3_2_2():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")
    worksheet.write(2,1, "内容2")

    # 保存
    workbook.save("新创建的表格.xls")
```
![](./img/1635944853745-3a3d3abd-850b-431c-9f15-aeda0ced7321.webp)<br />生成的表格内容如下：<br />![](./img/1635944853881-0fe54faf-41a8-4410-836c-20de9db15d22.webp)
<a name="TOBXt"></a>
### 2.3 xlwt 设置字体格式
程序示例：
```python
# 3.2.3 xlwt设置字体格式
def fun3_2_3():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 初始化样式
    style = xlwt.XFStyle()

    # 为样式创建字体
    font = xlwt.Font()
    font.name = 'Times New Roman'   #字体
    font.bold = True                #加粗
    font.underline = True           #下划线
    font.italic = True              #斜体

    # 设置样式
    style.font = font

    # 往表格写入内容
    worksheet.write(0,0, "内容1")
    worksheet.write(2,1, "内容2",style)

    # 保存
    workbook.save("新创建的表格.xls")
```
效果如下：<br />![](./img/1635944853958-79c0b2dd-5156-4145-9ed3-05670a4ecfa6.webp)
<a name="KqvyB"></a>
### 2.4 xlwt 设置列宽
xlwt中列宽的值表示方法：默认字体0的1/256为衡量单位。<br />xlwt创建时使用的默认宽度为2960，既11个字符0的宽度<br />所以在设置列宽时可以用如下方法：<br />width = 256 * 20 256为衡量单位，20表示20个字符宽度<br />程序示例：
```python
# 3.2.4 设置列宽
def fun3_2_4():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")
    worksheet.write(2,1, "内容2")

    # 设置列宽
    worksheet.col(0).width = 256*20

    # 保存
    workbook.save("新创建的表格.xls")
```
效果如下：<br />![](./img/1635944853958-0855e382-8cd3-41d6-ad56-e36c1a7f6e30.webp)
<a name="LhjaD"></a>
### 2.5 xlwt 设置行高
在xlwt中没有特定的函数来设置默认的列宽及行高<br />行高是在单元格的样式中设置的，可以通过自动换行通过输入文字的多少来确定行高<br />程序示例：
```python
# 3.2.5 设置行高
def fun3_2_5():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")
    worksheet.write(2,1, "内容2")

    # 设置行高
    style = xlwt.easyxf('font:height 360;')  # 18pt,类型小初的字号
    row = worksheet.row(0)
    row.set_style(style)

    # 保存
    workbook.save("新创建的表格.xls")
```
效果如下：<br />![](./img/1635944854100-62cdb895-1bd8-43b1-a627-4032328a051b.webp)
<a name="W7p2t"></a>
### 2.6 xlwt 合并列和行
程序示例：
```python
# 3.2.6 合并列和行
def fun3_2_6():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")
    
    # 合并 第1行到第2行 的 第0列到第3列
    worksheet.write_merge(1, 2, 0, 3, 'Merge Test')

    # 保存
    workbook.save("新创建的表格.xls")
```
效果如下：<br />![](./img/1635944854340-3887ab16-7026-4b7d-8663-7f24cc626461.webp)
<a name="RksDO"></a>
### 2.7 xlwt 添加边框
程序示例：
```python
# 3.2.7 添加边框
def fun3_2_7():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")
    
    # 设置边框样式
    borders = xlwt.Borders()  # Create Borders
    
    # May be:   NO_LINE, THIN, MEDIUM, DASHED, DOTTED, THICK, DOUBLE, HAIR,
    #           MEDIUM_DASHED, THIN_DASH_DOTTED, MEDIUM_DASH_DOTTED, THIN_DASH_DOT_DOTTED,
    #           MEDIUM_DASH_DOT_DOTTED, SLANTED_MEDIUM_DASH_DOTTED, or 0x00 through 0x0D.
    # DASHED虚线
    # NO_LINE没有
    # THIN实线
    
    borders.left = xlwt.Borders.DASHED
    borders.right = xlwt.Borders.DASHED
    borders.top = xlwt.Borders.DASHED
    borders.bottom = xlwt.Borders.DASHED
    borders.left_colour = 0x40
    borders.right_colour = 0x40
    borders.top_colour = 0x40
    borders.bottom_colour = 0x40
    
    style = xlwt.XFStyle()  # Create Style
    style.borders = borders  # Add Borders to Style
    
    worksheet.write(0, 0, '内容1', style)

    worksheet.write(2,1, "内容2")

    # 保存
    workbook.save("新创建的表格.xls")
```
效果如下：<br />![](./img/1635944854321-02febd5d-21dc-48ea-830f-31a1d3738e7c.webp)
<a name="eB5wJ"></a>
### 2.8 xlwt为单元格设置背景色
程序示例：
```python
# 设置单元格背景色
def fun3_2_8():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")

    # 创建样式
    pattern = xlwt.Pattern()
    
    # May be: NO_PATTERN, SOLID_PATTERN, or 0x00 through 0x12
    pattern.pattern = xlwt.Pattern.SOLID_PATTERN
    
    # May be: 8 through 63. 0 = Black, 1 = White, 2 = Red, 3 = Green, 4 = Blue, 5 = Yellow,
    # 6 = Magenta, 7 = Cyan, 16 = Maroon, 17 = Dark Green, 18 = Dark Blue, 19 = Dark Yellow ,
    # almost brown), 20 = Dark Magenta, 21 = Teal, 22 = Light Gray, 23 = Dark Gray, the list goes on...
    pattern.pattern_fore_colour = 5
    style = xlwt.XFStyle()
    style.pattern = pattern

    # 使用样式
    worksheet.write(2,1, "内容2",style)
```
效果如下：<br />![](./img/1635944854378-f271f7f6-3763-46c6-9c8c-82a81b6cc9a1.webp)
<a name="mF1gi"></a>
### 2.9 xlwt设置单元格对齐
使用xlwt中的Alignment来设置单元格的对齐方式，其中horz代表水平对齐方式，vert代表垂直对齐方式。<br />VERT_TOP = 0x00 上端对齐<br />VERT_CENTER = 0x01 居中对齐（垂直方向上）<br />VERT_BOTTOM = 0x02 低端对齐<br />HORZ_LEFT = 0x01 左端对齐<br />HORZ_CENTER = 0x02 居中对齐（水平方向上）<br />HORZ_RIGHT = 0x03 右端对齐<br />程序示例：
```python
# 设置单元格对齐
def fun3_2_9():
    # 创建新的workbook（其实就是创建新的excel）
    workbook = xlwt.Workbook(encoding= 'ascii')

    # 创建新的sheet表
    worksheet = workbook.add_sheet("My new Sheet")

    # 往表格写入内容
    worksheet.write(0,0, "内容1")

    # 设置样式
    style = xlwt.XFStyle()
    al = xlwt.Alignment()
    # VERT_TOP = 0x00       上端对齐
    # VERT_CENTER = 0x01    居中对齐（垂直方向上）
    # VERT_BOTTOM = 0x02    低端对齐
    # HORZ_LEFT = 0x01      左端对齐
    # HORZ_CENTER = 0x02    居中对齐（水平方向上）
    # HORZ_RIGHT = 0x03     右端对齐
    al.horz = 0x02  # 设置水平居中
    al.vert = 0x01  # 设置垂直居中
    style.alignment = al

    # 对齐写入
    worksheet.write(2,1, "内容2",style)

    # 保存
    workbook.save("新创建的表格.xls")
```
效果如下：<br />![](./img/1635944854467-6f49fcf8-b7ce-44bb-9731-5eddab82458b.webp)
<a name="gUMIF"></a>
## 3、Python xlutils 修改 操作Excel
xlutils可用于拷贝原excel或者在原excel基础上进行修改，并保存；<br />官方文档：[https://xlutils.readthedocs.io/en/latest/](https://xlutils.readthedocs.io/en/latest/)
<a name="gPF6f"></a>
### 3.1 pip安装xlutils
```bash
pip install xlutils
```
安装过程：<br />![](./img/1635944854638-fa33b262-0043-46bd-b274-f94426fc5ed9.webp)
<a name="wEOdE"></a>
### 3.2 xlutils拷贝源文件（需配合xlrd使用）
![](./img/1635944854636-3466eda0-655e-4448-b11e-32f22a98c7a2.webp)<br />表格内容如下：<br />![](./img/1635944854790-9e5b906c-ca34-47f3-b07b-4b60405713ba.webp)<br />程序示例：
```python
# 3.3.2 拷贝源文件
def fun3_3_2():
    workbook = xlrd.open_workbook('3_3 xlutils 修改操作练习.xlsx')  # 打开工作簿
    new_workbook = copy(workbook)  # 将xlrd对象拷贝转化为xlwt对象
    new_workbook.save("new_test.xls")  # 保存工作簿
```
效果如下：<br />![](./img/1635944854684-881ed71e-f2db-44b6-a74f-a2d0e7610039.webp)<br />内容为：<br />![](./img/1635944854843-83b6648f-03d5-49d8-8b2d-917c1535389c.webp)<br />不过表格的样式全部消失了。
<a name="GeXql"></a>
### 3.3 xlutils 读取 写入 （也就是修改）Excel 表格信息
程序示例：
```python
# 3.3.3 xlutils读取 写入 Excel 表格信息
def fun3_3_3():
    # file_path：文件路径，包含文件的全名称
    # formatting_info=True：保留Excel的原格式（使用与xlsx文件）
    workbook = xlrd.open_workbook('3_3 xlutils 修改操作练习.xlsx')
    
    new_workbook = copy(workbook)  # 将xlrd对象拷贝转化为xlwt对象

    # 读取表格信息
    sheet = workbook.sheet_by_index(0)
    col2 = sheet.col_values(1)  # 取出第二列
    cel_value = sheet.cell_value(1, 1)
    print(col2)
    print(cel_value)

    # 写入表格信息
    write_save = new_workbook.get_sheet(0)
    write_save.write(0, 0, "xlutils写入！")

    new_workbook.save("new_test.xls")  # 保存工作簿
```
效果如下：<br />![](./img/1635944854947-022b1a38-2cef-41e5-aa9e-917ec001d736.webp)<br />复制了源文件信息，并进行了追加：<br />![](./img/1635944855004-1b824d8b-9d94-44a3-bfe9-2d7721f2f9fc.webp)
<a name="WSZfq"></a>
## 4、Python xlwings 读取 写入 修改 操作Excel
xlwings比起xlrd、xlwt和xlutils，xlwings可豪华多了，它具备以下特点：

- xlwings能够非常方便的读写Excel文件中的数据，并且能够进行单元格格式的修改
- 可以和matplotlib以及pandas无缝连接，支持读写numpy、pandas数据类型，将matplotlib可视化图表导入到excel中。
- 可以调用Excel文件中VBA写好的程序，也可以让VBA调用用Python写的程序。
- 开源免费，一直在更新

官网地址：[https://www.xlwings.org/](https://www.xlwings.org/)<br />官方文档：[https://docs.xlwings.org/en/stable/api.html](https://docs.xlwings.org/en/stable/api.html)<br />![](./img/1635944855025-d63aa497-026f-45d6-8dc1-1a851075d690.webp)<br />![](./img/1635944855159-a4502992-60e9-4342-bd31-28e18ca9fc18.webp)
<a name="j6RcE"></a>
### 4.1 pip安装xlwings
```bash
pip install xlwings
```
<a name="aWQgi"></a>
### 4.2 基本操作
引入库
```python
import xlwings as xw 
```
打开Excel程序，默认设置：程序可见，只打开不新建工作薄
```python
app = xw.App(visible=True,add_book=False)
#新建工作簿 (如果不接下一条代码的话，Excel只会一闪而过，卖个萌就走了）
wb = app.books.add()
```
打开已有工作簿（支持绝对路径和相对路径）
```python
wb = app.books.open('example.xlsx')
#练习的时候建议直接用下面这条
#wb = xw.Book('example.xlsx')
#这样的话就不会频繁打开新的Excel
```
保存工作簿
```python
wb.save('example.xlsx')
```
退出工作簿（可省略）
```python
wb.close()
```
退出Excel
```python
app.quit()
```
三个例子：<br />（1）打开已存在的Excel文档
```python
# 导入xlwings模块
import xlwings as xw

# 打开Excel程序，默认设置：程序可见，只打开不新建工作薄，屏幕更新关闭
app=xw.App(visible=True,add_book=False)
app.display_alerts=False
app.screen_updating=False

# 文件位置：filepath，打开test文档，然后保存，关闭，结束程序
filepath=r'g:\Python Scripts\test.xlsx'
wb=app.books.open(filepath)
wb.save()
wb.close()
app.quit()
```
（2）新建Excel文档，命名为test.xlsx，并保存在D盘
```python
import xlwings as xw

app=xw.App(visible=True,add_book=False)
wb=app.books.add()
wb.save(r'd:\test.xlsx')
wb.close()
app.quit()
```
（3）在单元格输入值<br />新建test.xlsx，在sheet1的第一个单元格输入 “人生” ，然后保存关闭，退出Excel程序。
```python
import xlwings as xw

app=xw.App(visible=True,add_book=False)
wb=app.books.add()

# wb就是新建的工作簿(workbook)，下面则对wb的sheet1的A1单元格赋值
wb.sheets['sheet1'].range('A1').value='人生'
wb.save(r'd:\test.xlsx')
wb.close()
app.quit()
```
打开已保存的test.xlsx，在sheet2的第二个单元格输入“苦短”，然后保存关闭，退出Excel程序
```python
import xlwings as xw

app=xw.App(visible=True,add_book=False)
wb=app.books.open(r'd:\test.xlsx')

# wb就是新建的工作簿(workbook)，下面则对wb的sheet1的A1单元格赋值
wb.sheets['sheet1'].range('A1').value='苦短'
wb.save()
wb.close()
app.quit()
```
掌握以上代码，已经完全可以把Excel当作一个txt文本进行数据储存了，也可以读取Excel文件的数据，进行计算后，并将结果保存在Excel中。
<a name="ircm2"></a>
### 4.3 引用工作薄、工作表和单元格
（1）按名字引用工作簿，注意工作簿应该首先被打开
```python
wb=xw.books['工作簿的名字']
```
（2）引用活动的工作薄
```python
wb=xw.books.active
```
（3）引用工作簿中的sheet
```python
sht=xw.books['工作簿的名字‘].sheets['sheet的名字']
# 或者
wb=xw.books['工作簿的名字']
sht=wb.sheets[sheet的名字]
```
（4）引用活动sheet
```python
sht=xw.sheets.active
```
（5）引用A1单元格
```python
rng=xw.books['工作簿的名字‘].sheets['sheet的名字']
# 或者
sht=xw.books['工作簿的名字‘].sheets['sheet的名字']
rng=sht.range('A1')
```
（6）引用活动sheet上的单元格
```python
# 注意Range首字母大写
rng=xw.Range('A1')

#其中需要注意的是单元格的完全引用路径是：
# 第一个Excel程序的第一个工作薄的第一张sheet的第一个单元格
xw.apps[0].books[0].sheets[0].range('A1')
迅速引用单元格的方式是
sht=xw.books['名字'].sheets['名字']

# A1单元格
rng=sht[’A1']
        
# A1:B5单元格
rng=sht['A1:B5']
        
# 在第i+1行，第j+1列的单元格
# B1单元格
rng=sht[0,1]
        
# A1:J10
rng=sht[:10,:10]
        
#PS： 对于单元格也可以用表示行列的tuple进行引用
# A1单元格的引用
xw.Range(1,1)
        
#A1:C3单元格的引用
xw.Range((1,1),(3,3))
```
引用单元格：
```python
rng = sht.range('a1')
#rng = sht['a1']
#rng = sht[0,0] 第一行的第一列即a1,相当于pandas的切片
```
引用区域：
```python
rng = sht.range('a1:a5')
#rng = sht['a1:a5']
#rng = sht[:5,0]
```
<a name="cmozM"></a>
### 4.4 写入&读取数据
<a name="Dyzdq"></a>
#### 1.写入数据
（1）选择起始单元格A1,写入字符串‘Hello’
```python
sht.range('a1').value = 'Hello'
```
（2）写入列表
```python
# 行存储：将列表[1,2,3]储存在A1：C1中
sht.range('A1').value=[1,2,3]
# 列存储：将列表[1,2,3]储存在A1:A3中
sht.range('A1').options(transpose=True).value=[1,2,3]
# 将2x2表格，即二维数组，储存在A1:B2中，如第一行1，2，第二行3，4
sht.range('A1').options(expand='table')=[[1,2],[3,4]]
```

- 默认按行插入：A1:D1分别写入1,2,3,4
```python
sht.range('a1').value = [1,2,3,4]
```
等同于
```python
sht.range('a1:d1').value = [1,2,3,4]
```

- 按列插入：A2:A5分别写入5,6,7,8

可能会想：
```python
sht.range('a2:a5').value = [5,6,7,8]
```
但是会发现xlwings还是会按行处理的，上面一行等同于：
```python
sht.range('a2').value = [5,6,7,8]
```
正确语法:
```python
sht.range('a2').options(transpose=True).value = [5,6,7,8]
```
既然默认的是按行写入，就把它倒过来嘛（transpose），单词要打对，如果打错单词，它不会报错，而会按默认的行来写入

- 多行输入就要用二维列表了：
```python
sht.range('a6').expand('table').value = [['a','b','c'],['d','e','f'],['g','h','i']]
```
<a name="u7QPZ"></a>
#### 2.读取数据
（1）读取单个值
```python
# 将A1的值，读取到a变量中
a=sht.range('A1').value
```
（2）将值读取到列表中
```python
#将A1到A2的值，读取到a列表中
a=sht.range('A1:A2').value
# 将第一行和第二行的数据按二维数组的方式读取
a=sht.range('A1:B2').value
```

- 选取一列的数据

先计算单元格的行数(前提是连续的单元格)
```python
rng = sht.range('a1').expand('table')
nrows = rng.rows.count
```
接着就可以按准确范围读取了
```python
a = sht.range(f'a1:a{nrows}').value
```

- 选取一行的数据
```python
ncols = rng.columns.count
#用切片
fst_col = sht[0,:ncols].value
```
<a name="TPxct"></a>
### 4.5 常用函数和方法
<a name="cpOVA"></a>
#### 1.Book工作薄常用的api
```python
wb=xw.books['工作簿名称']
```

- `wb.activate()` 激活为当前工作簿
- `wb.fullname` 返回工作簿的绝对路径
- `wb.name` 返回工作簿的名称
- `wb.save(path=None)` 保存工作簿，默认路径为工作簿原路径，若未保存则为脚本所在的路径
- `wb. close()` 关闭工作簿

代码示例：
```python
# 引用Excel程序中，当前的工作簿
wb=xw.books.acitve
# 返回工作簿的绝对路径
x=wb.fullname
# 返回工作簿的名称
x=wb.name
# 保存工作簿，默认路径为工作簿原路径，若未保存则为脚本所在的路径
x=wb.save(path=None)
# 关闭工作簿
x=wb.close()
```
<a name="xqPwh"></a>
#### 2.sheet常用的api
```python
# 引用某指定sheet
sht=xw.books['工作簿名称'].sheets['sheet的名称']
# 激活sheet为活动工作表
sht.activate()
# 清除sheet的内容和格式
sht.clear()
# 清除sheet的内容
sht.contents()
# 获取sheet的名称
sht.name
# 删除sheet
sht.delete
```
<a name="y5aDf"></a>
#### 3.range常用的api
```python
# 引用当前活动工作表的单元格
rng=xw.Range('A1')
# 加入超链接
# rng.add_hyperlink(r'www.baidu.com','百度',‘提示：点击即链接到百度')
# 取得当前range的地址
rng.address
rng.get_address()
# 清除range的内容
rng.clear_contents()
# 清除格式和内容
rng.clear()
# 取得range的背景色,以元组形式返回RGB值
rng.color
# 设置range的颜色
rng.color=(255,255,255)
# 清除range的背景色
rng.color=None
# 获得range的第一列列标
rng.column
# 返回range中单元格的数据
rng.count
# 返回current_region
rng.current_region
# 返回ctrl + 方向
rng.end('down')
# 获取公式或者输入公式
rng.formula='=SUM(B1:B5)'
# 数组公式
rng.formula_array
# 获得单元格的绝对地址
rng.get_address(row_absolute=True, column_absolute=True,include_sheetname=False, external=False)
# 获得列宽
rng.column_width
# 返回range的总宽度
rng.width
# 获得range的超链接
rng.hyperlink
# 获得range中右下角最后一个单元格
rng.last_cell
# range平移
rng.offset(row_offset=0,column_offset=0)
#range进行resize改变range的大小
rng.resize(row_size=None,column_size=None)
# range的第一行行标
rng.row
# 行的高度，所有行一样高返回行高，不一样返回None
rng.row_height
# 返回range的总高度
rng.height
# 返回range的行数和列数
rng.shape
# 返回range所在的sheet
rng.sheet
#返回range的所有行
rng.rows
# range的第一行
rng.rows[0]
# range的总行数
rng.rows.count
# 返回range的所有列
rng.columns
# 返回range的第一列
rng.columns[0]
# 返回range的列数
rng.columns.count
# 所有range的大小自适应
rng.autofit()
# 所有列宽度自适应
rng.columns.autofit()
# 所有行宽度自适应
rng.rows.autofit()
```
<a name="qWvq1"></a>
#### 4.books 工作簿集合的api
```python
# 新建工作簿
xw.books.add()
# 引用当前活动工作簿
xw.books.active
```
<a name="RG3sR"></a>
#### 5.sheets 工作表的集合
```python
# 新建工作表
xw.sheets.add(name=None,before=None,after=None)
# 引用当前活动sheet
xw.sheets.active
```
<a name="F0ZtP"></a>
### 4.6 数据结构
<a name="ESQ7A"></a>
#### 1.一维数据
python的列表，可以和Excel中的行列进行数据交换，python中的一维列表，在Excel中默认为一行数据。
```python
import xlwings as xw

sht=xw.sheets.active

# 将1，2，3分别写入了A1，B1，C1单元格中
sht.range('A1').value=[1,2,3]

# 将A1，B1，C1单元格的值存入list1列表中
list1=sht.range('A1:C1').value

# 将1，2，3分别写入了A1，A2，A3单元格中
sht.range('A1').options(transpose=True).value=[1,2,3]

# 将A1，A2，A3单元格中值存入list1列表中
list1=sht.range('A1:A3').value
```
<a name="MqMe8"></a>
#### 2.二维数据
python的二维列表，可以转换为Excel中的行列。二维列表，即列表中的元素还是列表。在Excel中，二维列表中的列表元素，代表Excel表格中的一列。例如：
```python
# 将a1,a2,a3输入第一列，b1,b2,b3输入第二列
list1=[[‘a1’,'a2','a3'],['b1','b2','b3']]
sht.range('A1').value=list1
```
![](./img/1635944855253-f01d1e99-ab60-4e9c-8045-7cc1ff71b3f0.webp)
```python
# 将A1：B3的值赋给二维列表list1
list1=sht.range('A1:B3').value
```
<a name="nr8ZM"></a>
#### 3.Excel中区域的选取表格
```python
# 选取第一列
rng=sht. range('A1').expand('down')
rng.value=['a1','a2','a3']
```
![](./img/1635944855245-74ff9818-8a60-459f-9f9d-e0fd9e4aec29.webp)
```python
# 选取第一行
rng=sht.range('A1').expand('right')
rng=['a1','b1']
```
![](./img/1635944855484-569a7bd3-4c76-4516-9824-9e0f91788ab1.webp)
```python
# 选取表格
rng.sht.range('A1').expand('table')
rng.value=[[‘a1’,'a2','a3'],['b1','b2','b3']]
```
![](./img/1635944855498-c49d2216-efb4-46a6-a32e-e086ec1215c7.webp)
<a name="mtQht"></a>
### 4.7 xlwings生成图表
```python
import xlwings as xw
app = xw.App()
wb = app.books.active
sht = wb.sheets.active

chart = sht.charts.add(100, 10)  # 100, 10 为图表放置的位置坐标。以像素为单位。
chart.set_source_data(sht.range('A1').expand())  # 参数为表格中的数据区域。
# chart.chart_type = i               # 用来设置图表类型，具体参数件下面详细说明。
chart.api[1].ChartTitle.Text = i          # 用来设置图表的标题。
```
示例代码：
```python
import xlwings as xw
app = xw.App()
wb = app.books.active
sht = wb.sheets.active
# 生成图表的数据
sht.range('A1').value = [['时间', '数量'], ['1日', 2], ['2日', 1], ['3日', 3]
                         , ['4日', 4], ['5日', 5], ['6日', 6]]
"""图表类型参数，被注释的那几个，无法生成对应的图表"""
dic = {
    '3d_area': -4098,
    '3d_area_stacked': 78,
    '3d_area_stacked_100': 79,
    '3d_bar_clustered': 60,
    '3d_bar_stacked': 61,
    '3d_bar_stacked_100': 62,
    '3d_column': -4100,
    '3d_column_clustered': 54,
    '3d_column_stacked': 55,
    '3d_column_stacked_100': 56,
    '3d_line': -4101,
    '3d_pie': -4102,
    '3d_pie_exploded': 70,
    'area': 1,
    'area_stacked': 76,
    'area_stacked_100': 77,
    'bar_clustered': 57,
    'bar_of_pie': 71,
    'bar_stacked': 58,
    'bar_stacked_100': 59,
    'bubble': 15,
    'bubble_3d_effect': 87,
    'column_clustered': 51,
    'column_stacked': 52,
    'column_stacked_100': 53,
    'cone_bar_clustered': 102,
    'cone_bar_stacked': 103,
    'cone_bar_stacked_100': 104,
    'cone_col': 105,
    'cone_col_clustered': 99,
    'cone_col_stacked': 100,
    'cone_col_stacked_100': 101,
    'cylinder_bar_clustered': 95,
    'cylinder_bar_stacked': 96,
    'cylinder_bar_stacked_100': 97,
    'cylinder_col': 98,
    'cylinder_col_clustered': 92,
    'cylinder_col_stacked': 93,
    'cylinder_col_stacked_100': 94,
    'doughnut': -4120,
    'doughnut_exploded': 80,
    'line': 4,
    'line_markers': 65,
    'line_markers_stacked': 66,
    'line_markers_stacked_100': 67,
    'line_stacked': 63,
    'line_stacked_100': 64,
    'pie': 5,
    'pie_exploded': 69,
    'pie_of_pie': 68,
    'pyramid_bar_clustered': 109,
    'pyramid_bar_stacked': 110,
    'pyramid_bar_stacked_100': 111,
    'pyramid_col': 112,
    'pyramid_col_clustered': 106,
    'pyramid_col_stacked': 107,
    'pyramid_col_stacked_100': 108,
    'radar': -4151,
    'radar_filled': 82,
    'radar_markers': 81,
    # 'stock_hlc': 88,
    # 'stock_ohlc': 89,
    # 'stock_vhlc': 90,
    # 'stock_vohlc': 91,
    # 'surface': 83,
    # 'surface_top_view': 85,
    # 'surface_top_view_wireframe': 86,
    # 'surface_wireframe': 84,
    'xy_scatter': -4169,
    'xy_scatter_lines': 74,
    'xy_scatter_lines_no_markers': 75,
    'xy_scatter_smooth': 72,
    'xy_scatter_smooth_no_markers': 73
}
w = 385
h = 241
n = 0
x = 100
y = 10
for i in dic.keys():
    xx = x + n % 3*w  # 用来生成图表放置的x坐标。
    yy = y + n//3*h   # 用来生成图表放置的y坐标。
    chart = sht.charts.add(xx, yy)
    chart.set_source_data(sht.range('A1').expand())
    chart.chart_type = i
    chart.api[1].ChartTitle.Text = i
    n += 1
    wb.save('chart_图表')
    wb.close()
    app.quit()
```
效果如下：<br />![](./img/1635944855564-5f559846-927a-4ff8-905e-5b153f5d0d51.webp)<br />![](./img/1635944855553-33404459-2a69-4e83-8fce-28cf264b7f30.webp)<br />![](./img/1635944855620-3f677709-981c-4999-b3ae-bfff973a0d97.webp)<br />![](./img/1635944855779-931b0b9a-73a6-4e94-9413-c33c0a2c65e4.webp)<br />![](./img/1635944855925-d31105a1-1da6-4cee-ae98-95f85f07561b.webp)
<a name="OW853"></a>
### 4.8 实战训练
<a name="wBSG7"></a>
#### 1. xlwings 新建 Excel 文档
![](./img/1635944855904-d25382eb-ec7b-4282-88db-572c47da44ea.webp)<br />程序示例：
```python
# 3.4.2 xlwings 新建 Excle 文档
def fun3_4_2():
    """
    visible
    Ture：可见excel
    False：不可见excel

    add_book
    True:打开excel并且新建工作簿
    False：不新建工作簿
    """
    app = xw.App(visible=True, add_book=False)

    # 新建工作簿 (如果不接下一条代码的话，Excel只会一闪而过，卖个萌就走了）
    wb = app.books.add()

    # 保存工作簿
    wb.save('example.xlsx')

    # 退出工作簿
    wb.close()

    # 退出Excel
    app.quit()
```
执行程序后文件夹增加了“example.xlsx”:<br />![](./img/1635944856006-01d37151-f14e-405d-aef3-a473e19409f8.webp)<br />此时表格是空的：<br />![](./img/1635944856041-947664ee-170b-48f5-b135-bb212f4de0a7.webp)
<a name="qtaCY"></a>
#### 2. xlwings 打开已存在的 Excel 文档
现有表格长这样：<br />![](./img/1635944856103-b0a73a75-84cd-4648-9d01-ad12225fd37c.webp)<br />运行程序：
```python
# 3.4.3 xlwings 打开已存在的Excel文件
def fun3_4_3():
    # 新建Excle 默认设置：程序可见，只打开不新建工作薄，屏幕更新关闭
    app = xw.App(visible=True, add_book=False)
    app.display_alerts = False
    app.screen_updating = False

    # 打开已存在的Excel文件
    wb=app.books.open('./3_4 xlwings 修改操作练习.xlsx')

    # 保存工作簿
    wb.save('example_2.xlsx')

    # 退出工作簿
    wb.close()

    # 退出Excel
    app.quit()
```
生成新的表格：<br />![](./img/1635944856288-33770a5d-1c72-47c3-a85d-87335d4af6da.webp)<br />内容如下：<br />![](./img/1635944856536-a7fa476b-b7f6-423e-a72f-67a9dfdfe24d.webp)
<a name="VWyFX"></a>
#### 3. xlwings 读写 Excel
程序示例：
```python
#  3.4.4 xlwings读写 Excel
def fun3_4_4():
    # 新建Excle 默认设置：程序可见，只打开不新建工作薄，屏幕更新关闭
    app = xw.App(visible=True, add_book=False)
    app.display_alerts = False
    app.screen_updating = False

    # 打开已存在的Excel文件
    wb=app.books.open('./3_4 xlwings 修改操作练习.xlsx')

    # 获取sheet对象
    print(wb.sheets)
    sheet = wb.sheets[0]
    # sheet = wb.sheets["sheet1"]

    # 读取Excel信息
    cellB1_value = sheet.range('B1').value
    print("单元格B1内容为：",cellB1_value)

    # 清除单元格内容和格式
    sheet.range('A1').clear()

    # 写入单元格
    sheet.range('A1').value = "xlwings写入"

    # 保存工作簿
    wb.save('example_3.xlsx')

    # 退出工作簿
    wb.close()

    # 退出Excel
    app.quit()
```
执行效果：<br />![](./img/1635944856366-e934c72b-cacf-4e11-b7c7-08694334d4ba.webp)<br />![](./img/1635944856433-97608f37-515a-43ca-a190-276a01facefa.webp)
<a name="yR8bJ"></a>
## 5、Python openpyxl 读取 写入 修改 操作Excel
在openpyxl中，主要用到三个概念：Workbooks，Sheets，Cells。

- Workbook就是一个excel工作表；
- Sheet是工作表中的一张表页；
- Cell就是简单的一个格。

openpyxl就是围绕着这三个概念进行的，不管读写都是“三板斧”：打开Workbook，定位Sheet，操作Cell。<br />官方文档：[https://openpyxl.readthedocs.io/en/stable/](https://openpyxl.readthedocs.io/en/stable/)<br />官方示例：
```python
from openpyxl import Workbook
wb = Workbook()

# grab the active worksheet
ws = wb.active

# Data can be assigned directly to cells
ws['A1'] = 42

# Rows can also be appended
ws.append([1, 2, 3])

# Python types will automatically be converted
import datetime
ws['A2'] = datetime.datetime.now()

# Save the file
wb.save("sample.xlsx")
```
<a name="Fq7nn"></a>
### 5.1 openpyxl 基本操作
<a name="gH21A"></a>
#### 1.安装
```bash
pip install openpyxl
```
因为已经安装，所以提示如下信息：<br />![](./img/1635944856554-ff25fc8c-fca1-4513-aea9-b5552023042a.webp)
<a name="NmlW4"></a>
#### 2.打开文件
（1）新建
```python
from  openpyxl import  Workbook 
# 实例化
wb = Workbook()
# 激活 worksheet
ws = wb.active
```
（2）打开已有
```python
from openpyxl  import load_workbook

wb = load_workbook('文件名称.xlsx')
```
<a name="UIBZS"></a>
#### 3.写入数据
```python
# 方式一：数据可以直接分配到单元格中(可以输入公式)
ws['A1'] = 42
# 方式二：可以附加行，从第一列开始附加(从最下方空白处，最左开始)(可以输入多行)
ws.append([1, 2, 3])
# 方式三：Python 类型会被自动转换
ws['A3'] = datetime.datetime.now().strftime("%Y-%m-%d")
```
<a name="hRXDO"></a>
#### 4.创建表（sheet）
```python
# 方式一：插入到最后(default)
ws1 = wb.create_sheet("Mysheet") 
# 方式二：插入到最开始的位置
ws2 = wb.create_sheet("Mysheet", 0)
```
<a name="X5mEf"></a>
#### 5.选择表（sheet）
```python
# sheet 名称可以作为 key 进行索引
>>> ws3 = wb["New Title"]
>>> ws4 = wb.get_sheet_by_name("New Title")
>>> ws is ws3 is ws4
True
```
<a name="qLPpq"></a>
#### 6.查看表名（sheet）
```python
# 显示所有表名
>>> print(wb.sheetnames)
['Sheet2', 'New Title',  'Sheet1']
# 遍历所有表
>>> for sheet in  wb:
...     print(sheet.title)
```
<a name="kE9F7"></a>
#### 7.访问单元格（cell）
（1）单个单元格访问
```python
# 方法一
>>> c = ws['A4']
# 方法二：row 行；column 列
>>> d = ws.cell(row=4, column=2, value=10)
# 方法三：只要访问就创建
>>> for i in  range(1,101):
...         for j in range(1,101):
...            ws.cell(row=i, column=j)
```
（2）多个单元格访问
```python
# 通过切片
>>> cell_range = ws['A1':'C2']
# 通过行(列)
>>> colC = ws['C']
>>> col_range = ws['C:D']
>>> row10 = ws[10]
>>> row_range = ws[5:10]
# 通过指定范围(行 → 行)
>>> for row in  ws.iter_rows(min_row=1, max_col=3, max_row=2):
...    for cell in  row:
...        print(cell)
<Cell Sheet1.A1>
<Cell Sheet1.B1>
<Cell Sheet1.C1>
<Cell Sheet1.A2>
<Cell Sheet1.B2>
<Cell Sheet1.C2> 
# 通过指定范围(列 → 列)
>>> for row in  ws.iter_rows(min_row=1, max_col=3, max_row=2):
...    for cell in  row:
...        print(cell)
<Cell Sheet1.A1>
<Cell Sheet1.B1>
<Cell Sheet1.C1>
<Cell Sheet1.A2>
<Cell Sheet1.B2>
<Cell Sheet1.C2>
# 遍历所有 方法一
>>> ws = wb.active
>>> ws['C9'] = 'hello world'
>>> tuple(ws.rows)
((<Cell Sheet.A1>, <Cell Sheet.B1>, <Cell Sheet.C1>),
(<Cell Sheet.A2>, <Cell Sheet.B2>, <Cell Sheet.C2>),
...
(<Cell Sheet.A8>, <Cell Sheet.B8>, <Cell Sheet.C8>),
(<Cell Sheet.A9>, <Cell Sheet.B9>, <Cell Sheet.C9>))
# 遍历所有 方法二
>>> tuple(ws.columns)
((<Cell Sheet.A1>,
<Cell Sheet.A2>,
<Cell Sheet.A3>,
...
<Cell Sheet.B7>,
<Cell Sheet.B8>,
<Cell Sheet.B9>),
(<Cell Sheet.C1>,
...
<Cell Sheet.C8>,
<Cell Sheet.C9>))
```
<a name="JXsye"></a>
#### 8.保存数据
```python
wb.save('文件名称.xlsx')
```
<a name="cNYTL"></a>
#### 9.其它
（1）改变sheet标签按钮颜色
```python
ws.sheet_properties.tabColor = "1072BA" # 色值为RGB16进制值
```
（2）获取最大行，最大列
```python
# 获得最大列和最大行
print(sheet.max_row)
print(sheet.max_column)
```
（3）获取每一行每一列

- `sheet.rows`为生成器, 里面是每一行的数据，每一行又由一个tuple包裹。
- `sheet.columns`类似，不过里面是每个tuple是每一列的单元格。
```python
# 因为按行，所以返回A1, B1, C1这样的顺序
for row in sheet.rows:
    for cell in row:
        print(cell.value)

# A1, A2, A3这样的顺序
for column in sheet.columns:
    for cell in column:
        print(cell.value)
```
（4）根据数字得到字母，根据字母得到数字
```python
from openpyxl.utils import get_column_letter, column_index_from_string

# 根据列的数字返回字母
print(get_column_letter(2))  # B
# 根据字母返回列的数字
print(column_index_from_string('D'))  # 4
```
（5）删除工作表
```python
# 方式一
wb.remove(sheet)
# 方式二
del wb[sheet]
```
（6）矩阵置换
```python
rows = [
    ['Number', 'data1', 'data2'],
    [2, 40, 30],
    [3, 40, 25],
    [4, 50, 30],
    [5, 30, 10],
    [6, 25, 5],
    [7, 50, 10]]

list(zip(*rows))

# out
[('Number', 2, 3, 4, 5, 6, 7),
 ('data1', 40, 40, 50, 30, 25, 50),
 ('data2', 30, 25, 30, 10, 5, 10)]

# 注意 方法会舍弃缺少数据的列(行)
rows = [
    ['Number', 'data1', 'data2'],
    [2, 40      ],    # 这里少一个数据
    [3, 40, 25],
    [4, 50, 30],
    [5, 30, 10],
    [6, 25, 5],
    [7, 50, 10],
]
# out
[('Number', 2, 3, 4, 5, 6, 7), ('data1', 40, 40, 50, 30, 25, 50)]
```
<a name="Z1Lm7"></a>
#### 10.设置单元格风格
（1）需要导入的类
```python
from openpyxl.styles import Font, colors, Alignment
```
（2）字体

- 下面的代码指定了等线24号，加粗斜体，字体颜色红色。直接使用cell的font属性，将Font对象赋值给它。
```python
bold_itatic_24_font = Font(name='等线', size=24, italic=True, color=colors.RED, bold=True)

sheet['A1'].font = bold_itatic_24_font
```
（3）对齐方式

- 也是直接使用cell的属性aligment，这里指定垂直居中和水平居中。除了center，还可以使用right、left等等参数
```python
# 设置B1中的数据垂直居中和水平居中
sheet['B1'].alignment = Alignment(horizontal='center', vertical='center')
```
（4）设置行高和列宽
```python
# 第2行行高
sheet.row_dimensions[2].height = 40
# C列列宽
sheet.column_dimensions['C'].width = 30
```
（5）合并和拆分单元格

- 所谓合并单元格，即以合并区域的左上角的那个单元格为基准，覆盖其他单元格使之称为一个大的单元格。
- 相反，拆分单元格后将这个大单元格的值返回到原来的左上角位置。
```python
# 合并单元格， 往左上角写入数据即可
sheet.merge_cells('B1:G1') # 合并一行中的几个单元格
sheet.merge_cells('A1:C3') # 合并一个矩形区域中的单元格
```

- 合并后只可以往左上角写入数据，也就是区间中:左边的坐标。
- 如果这些要合并的单元格都有数据，只会保留左上角的数据，其他则丢弃。换句话说若合并前不是在左上角写入数据，合并后单元格中不会有数据。
- 以下是拆分单元格的代码。拆分后，值回到A1位置
```python
sheet.unmerge_cells('A1:C3')
```
<a name="mntV6"></a>
#### 11.示例代码
```python
import datetime
from random import choice
from time import time
from openpyxl import load_workbook
from openpyxl.utils import get_column_letter

# 设置文件 mingc
addr = "openpyxl.xlsx"
# 打开文件
wb = load_workbook(addr)
# 创建一张新表
ws = wb.create_sheet()
# 第一行输入
ws.append(['TIME', 'TITLE', 'A-Z'])

# 输入内容（500行数据）
for i in range(500):
    TIME = datetime.datetime.now().strftime("%H:%M:%S")
    TITLE = str(time())
    A_Z = get_column_letter(choice(range(1, 50)))
    ws.append([TIME, TITLE, A_Z])

# 获取最大行
row_max = ws.max_row
# 获取最大列
con_max = ws.max_column
# 把上面写入内容打印在控制台
for j in ws.rows:    # we.rows 获取每一行数据
    for n in j:
        print(n.value, end="\t")   # n.value 获取单元格的值
    print()
# 保存，save（必须要写文件名（绝对地址）默认 py 同级目录下，只支持 xlsx 格式）
wb.save(addr)
```
<a name="gZB12"></a>
### 5.2 openpyxl生成2D图表
示例代码：
```python
from openpyxl import Workbook
from openpyxl.chart import BarChart, Series, Reference

wb = Workbook(write_only=True)
ws = wb.create_sheet()

rows = [
    ('Number', 'Batch 1', 'Batch 2'),
    (2, 10, 30),
    (3, 40, 60),
    (4, 50, 70),
    (5, 20, 10),
    (6, 10, 40),
    (7, 50, 30),
]

for row in rows:
    ws.append(row)

chart1 = BarChart()
chart1.type = "col"
chart1.style = 10
chart1.title = "Bar Chart"
chart1.y_axis.title = 'Test number'
chart1.x_axis.title = 'Sample length (mm)'

data = Reference(ws, min_col=2, min_row=1, max_row=7, max_col=3)
cats = Reference(ws, min_col=1, min_row=2, max_row=7)
chart1.add_data(data, titles_from_data=True)
chart1.set_categories(cats)
chart1.shape = 4
ws.add_chart(chart1, "A10")

from copy import deepcopy

chart2 = deepcopy(chart1)
chart2.style = 11
chart2.type = "bar"
chart2.title = "Horizontal Bar Chart"
ws.add_chart(chart2, "G10")

chart3 = deepcopy(chart1)
chart3.type = "col"
chart3.style = 12
chart3.grouping = "stacked"
chart3.overlap = 100
chart3.title = 'Stacked Chart'
ws.add_chart(chart3, "A27")

chart4 = deepcopy(chart1)
chart4.type = "bar"
chart4.style = 13
chart4.grouping = "percentStacked"
chart4.overlap = 100
chart4.title = 'Percent Stacked Chart'
ws.add_chart(chart4, "G27")

wb.save("bar.xlsx")
```
效果如下：<br />![](./img/1635944856663-6d6a1f27-331b-4e01-a6db-f70db83417af.webp)
<a name="LRs6g"></a>
### 5.3 openpyxl生成3D图表
示例代码：
```python
from openpyxl import Workbook
from openpyxl.chart import (
    Reference,
    Series,
    BarChart3D,
)

wb = Workbook()
ws = wb.active

rows = [
    (None, 2013, 2014),
    ("Apples", 5, 4),
    ("Oranges", 6, 2),
    ("Pears", 8, 3)
]

for row in rows:
    ws.append(row)

data = Reference(ws, min_col=2, min_row=1, max_col=3, max_row=4)
titles = Reference(ws, min_col=1, min_row=2, max_row=4)
chart = BarChart3D()
chart.title = "3D Bar Chart"
chart.add_data(data=data, titles_from_data=True)
chart.set_categories(titles)

ws.add_chart(chart, "E5")
wb.save("bar3d.xlsx")
```
效果如下：<br />![](./img/1635944856660-52f7cf23-e7d5-4601-938f-de05a5ac23d3.webp)
<a name="MBe19"></a>
### 5.4 实战训练
<a name="ftloD"></a>
#### 1.openpyxl 新建Excel
程序示例：
```python
# 3.5.2 openpyxl 新建Excel
def fun3_5_2():
    wb = Workbook()

    # 注意：该函数调用工作表的索引(_active_sheet_index)，默认是0。
    # 除非修改了这个值，否则使用该函数一直是在对第一张工作表进行操作。
    ws = wb.active

    # 设置sheet名称
    ws.title = "New Title"

    # 设置sheet颜色
    ws.sheet_properties.tabColor = "1072BA"

    # 保存表格
    wb.save('保存一个新的excel.xlsx')
```
执行效果：<br />![](./img/1635944856853-f8b96f18-413c-4f37-9893-e767e71e9df9.webp)<br />并对sheet设置了标题和背景颜色：<br />![](./img/1635944856873-254a7904-d846-4cfa-83e0-b5f992df96e7.webp)
<a name="NQ3f7"></a>
#### 2.openpyxl 打开已存在Excel
程序示例：
```python
# 3.5.3 openpyxl 打开已存在Excel
def fun3_5_3():
    wb = load_workbook("./3_5 openpyxl 修改操作练习.xlsx")

    # 注意：该函数调用工作表的索引(_active_sheet_index)，默认是0。
    # 除非修改了这个值，否则使用该函数一直是在对第一张工作表进行操作。
    ws = wb.active

    # 保存表格
    wb.save('copy.xlsx')
```
效果如下：<br />![](./img/1635944856930-78642478-0778-4c87-bcb4-1568b7d9d6fe.webp)
<a name="GjyWg"></a>
#### 3.openpyxl 读写Excel
程序示例：
```python
# 3.5.4 openpyxl 读写Excel
def fun3_5_4():
    wb = load_workbook("./3_5 openpyxl 修改操作练习.xlsx")

    # 注意：该函数调用工作表的索引(_active_sheet_index)，默认是0。
    # 除非修改了这个值，否则使用该函数一直是在对第一张工作表进行操作。
    ws = wb.active

    # 读取单元格信息
    cellB2_value = ws['B2'].value
    print("单元格B2内容为：",cellB2_value)

    # 写入单元格
    ws['A1'].value = "OPENPYXL"

    # 保存表格
    wb.save('copy.xlsx')
```
执行结果：<br />![](./img/1635944856963-d986a43d-ae6c-4745-bdeb-0d87df64f2e8.webp)<br />![](./img/1635944856998-ba77a1f4-ce73-48b6-9227-167f381c4bec.webp)
<a name="u5zaA"></a>
## 6、Python xlswriter 写入 操作Excel

- XlsxWriter是一个用来写Excel2007和xlsx文件格式的python模块。它可以用来写文本、数字、公式并支持单元格格式化、图片、图表、文档配置、自动过滤等特性
- 优点：功能更多、文档高保真、扩展格式类型、更快并可配置 缺点：不能用来读取和修改excel文件

官方文档：[https://xlsxwriter.readthedocs.io/](https://xlsxwriter.readthedocs.io/)
<a name="k6Cnr"></a>
### 6.1 xlswriter基本操作
<a name="xxS9B"></a>
#### 1.安装 xlswriter 模块
```bash
pip install XlsxWriter
```
由于已经安装过了，所以提示已经安装：<br />![](./img/1635944857204-011dff07-2bf2-48d0-9b5d-3fbf23e5fe33.webp)
<a name="wbr7Y"></a>
#### 2.创建excel文件
```python
# 创建文件
workbook = xlsxwriter.Workbook("new_excel.xlsx") 
```
<a name="CvmzF"></a>
#### 3.创建sheet
```python
# 创建sheet
worksheet = workbook.add_worksheet("first_sheet") 
```
<a name="YQDa8"></a>
#### 4.写入数据
（1）写入文本
```python
# 法一：
worksheet.write('A1', 'write something')
# 法二：
worksheet.write(1, 0, 'hello world')
```
（2）写入数字
```python
# 写入数字
worksheet.write(0, 1, 32)
worksheet.write(1, 1, 32.3)
```
（3）写入函数
```python
worksheet.write(2, 1, '=sum(B1:B2)')
```
（4）写入图片
```python
# 插入图片
worksheet.insert_image(0, 5, 'test.png')
worksheet.insert_image(0, 5, 'test.png', {'url': 'http://httpbin.org/'})
```
（5）写入日期
```python
# 写入日期
d = workbook.add_format({'num_format': 'yyyy-mm-dd'})
worksheet.write(0, 2, datetime.datetime.strptime('2017-09-13', '%Y-%m-%d'), d)
```
（6）设置行、列属性
```python
# 设置行属性，行高设置为40
worksheet.set_row(0, 40)

# 设置列属性，把A到B列宽设置为20
worksheet.set_column('A:B', 20)
```
<a name="X0aST"></a>
#### 5.自定义格式
常用格式：

- 字体颜色：color
- 字体加粗：bold
- 字体大小：font_site
- 日期格式：num_format
- 超链接：url
- 下划线设置：underline
- 单元格颜色：bg_color
- 边框：border
- 对齐方式：align
```python
# 自定义格式
f = workbook.add_format({'border': 1, 'font_size': 13, 'bold': True, 'align': 'center','bg_color': 'cccccc'})
worksheet.write('A3', "python excel", f)
worksheet.set_row(0, 40, f)
worksheet.set_column('A:E', 20, f)
```
<a name="Hvb3L"></a>
#### 6.批量往单元格写入数据
```python
# 批量往单元格写入数据
worksheet.write_column('A15', [1, 2, 3, 4, 5])  # 列写入，从A15开始
worksheet.write_row('A12', [6, 7, 8, 9])        # 行写入，从A12开始
```
<a name="bhP3G"></a>
#### 7.合并单元格写入
```python
# 合并单元格写入
worksheet.merge_range(7,5, 11, 8, 'merge_range')
```
<a name="fjeyU"></a>
#### 8.关闭文件
```python
workbook.close()
```
<a name="xBo9D"></a>
### 6.3 xlswriter 生成折线图
示例代码：
```python
# -*- coding:utf-8 -*-

import xlsxwriter

# 创建一个excel
workbook = xlsxwriter.Workbook("chart_line.xlsx")
# 创建一个sheet
worksheet = workbook.add_worksheet()
# worksheet = workbook.add_worksheet("bug_analysis")

# 自定义样式，加粗
bold = workbook.add_format({'bold': 1})

# --------1、准备数据并写入excel---------------
# 向excel中写入数据，建立图标时要用到
headings = ['Number', 'testA', 'testB']
data = [
    ['2017-9-1', '2017-9-2', '2017-9-3', '2017-9-4', '2017-9-5', '2017-9-6'],
    [10, 40, 50, 20, 10, 50],
    [30, 60, 70, 50, 40, 30],
]

# 写入表头
worksheet.write_row('A1', headings, bold)

# 写入数据
worksheet.write_column('A2', data[0])
worksheet.write_column('B2', data[1])
worksheet.write_column('C2', data[2])

# --------2、生成图表并插入到excel---------------
# 创建一个柱状图(line chart)
chart_col = workbook.add_chart({'type': 'line'})

# 配置第一个系列数据
chart_col.add_series({
    # 这里的sheet1是默认的值，因为在新建sheet时没有指定sheet名
    # 如果新建sheet时设置了sheet名，这里就要设置成相应的值
    'name': '=Sheet1!$B$1',
    'categories': '=Sheet1!$A$2:$A$7',
    'values':   '=Sheet1!$B$2:$B$7',
    'line': {'color': 'red'},
})

# 配置第二个系列数据
chart_col.add_series({
    'name': '=Sheet1!$C$1',
    'categories':  '=Sheet1!$A$2:$A$7',
    'values':   '=Sheet1!$C$2:$C$7',
    'line': {'color': 'yellow'},
})

# 配置第二个系列数据(用了另一种语法)
# chart_col.add_series({
#     'name': ['Sheet1', 0, 2],
#     'categories': ['Sheet1', 1, 0, 6, 0],
#     'values': ['Sheet1', 1, 2, 6, 2],
#     'line': {'color': 'yellow'},
# })

# 设置图表的title 和 x，y轴信息
chart_col.set_title({'name': 'The xxx site Bug Analysis'})
chart_col.set_x_axis({'name': 'Test number'})
chart_col.set_y_axis({'name':  'Sample length (mm)'})

# 设置图表的风格
chart_col.set_style(1)

# 把图表插入到worksheet并设置偏移
worksheet.insert_chart('A10', chart_col, {'x_offset': 25, 'y_offset': 10})

workbook.close()
```
效果如下：<br />![](./img/1635944857165-d7d2dbc5-07aa-4dbe-bbb9-a773285ffd3e.webp)
<a name="csls1"></a>
### 6.4 xlswriter 生成柱状图
示例代码：
```python
# -*- coding:utf-8 -*-

import xlsxwriter

# 创建一个excel
workbook = xlsxwriter.Workbook("chart_column.xlsx")
# 创建一个sheet
worksheet = workbook.add_worksheet()
# worksheet = workbook.add_worksheet("bug_analysis")

# 自定义样式，加粗
bold = workbook.add_format({'bold': 1})

# --------1、准备数据并写入excel---------------
# 向excel中写入数据，建立图标时要用到
headings = ['Number', 'testA', 'testB']
data = [
    ['2017-9-1', '2017-9-2', '2017-9-3', '2017-9-4', '2017-9-5', '2017-9-6'],
    [10, 40, 50, 20, 10, 50],
    [30, 60, 70, 50, 40, 30],
]

# 写入表头
worksheet.write_row('A1', headings, bold)

# 写入数据
worksheet.write_column('A2', data[0])
worksheet.write_column('B2', data[1])
worksheet.write_column('C2', data[2])

# --------2、生成图表并插入到excel---------------
# 创建一个柱状图(column chart)
chart_col = workbook.add_chart({'type': 'column'})

# 配置第一个系列数据
chart_col.add_series({
    # 这里的sheet1是默认的值，因为在新建sheet时没有指定sheet名
    # 如果新建sheet时设置了sheet名，这里就要设置成相应的值
    'name': '=Sheet1!$B$1',
    'categories': '=Sheet1!$A$2:$A$7',
    'values':   '=Sheet1!$B$2:$B$7',
    'line': {'color': 'red'},
})

# 配置第二个系列数据(用了另一种语法)
chart_col.add_series({
    'name': '=Sheet1!$C$1',
    'categories':  '=Sheet1!$A$2:$A$7',
    'values':   '=Sheet1!$C$2:$C$7',
    'line': {'color': 'yellow'},
})

# 配置第二个系列数据(用了另一种语法)
# chart_col.add_series({
#     'name': ['Sheet1', 0, 2],
#     'categories': ['Sheet1', 1, 0, 6, 0],
#     'values': ['Sheet1', 1, 2, 6, 2],
#     'line': {'color': 'yellow'},
# })

# 设置图表的title 和 x，y轴信息
chart_col.set_title({'name': 'The xxx site Bug Analysis'})
chart_col.set_x_axis({'name': 'Test number'})
chart_col.set_y_axis({'name':  'Sample length (mm)'})

# 设置图表的风格
chart_col.set_style(1)

# 把图表插入到worksheet以及偏移
worksheet.insert_chart('A10', chart_col, {'x_offset': 25, 'y_offset': 10})

workbook.close()
```
效果如下：<br />![](./img/1635944857307-d110de00-87c5-4d13-92a4-c3539977735e.webp)
<a name="S57j1"></a>
### 6.5 xlswriter 生成饼图
示例代码：
```python
# -*- coding:utf-8 -*-

import xlsxwriter

# 创建一个excel
workbook = xlsxwriter.Workbook("chart_pie.xlsx")
# 创建一个sheet
worksheet = workbook.add_worksheet()

# 自定义样式，加粗
bold = workbook.add_format({'bold': 1})

# --------1、准备数据并写入excel---------------
# 向excel中写入数据，建立图标时要用到
data = [
    ['closed', 'active', 'reopen', 'NT'],
    [1012, 109, 123, 131],
]

# 写入数据
worksheet.write_row('A1', data[0], bold)
worksheet.write_row('A2', data[1])

# --------2、生成图表并插入到excel---------------
# 创建一个柱状图(pie chart)
chart_col = workbook.add_chart({'type': 'pie'})

# 配置第一个系列数据
chart_col.add_series({
    'name': 'Bug Analysis',
    'categories': '=Sheet1!$A$1:$D$1',
    'values': '=Sheet1!$A$2:$D$2',
    'points': [
        {'fill': {'color': '#00CD00'}},
        {'fill': {'color': 'red'}},
        {'fill': {'color': 'yellow'}},
        {'fill': {'color': 'gray'}},
    ],

})

# 设置图表的title 和 x，y轴信息
chart_col.set_title({'name': 'Bug Analysis'})

# 设置图表的风格
chart_col.set_style(10)

# 把图表插入到worksheet以及偏移
worksheet.insert_chart('B10', chart_col, {'x_offset': 25, 'y_offset': 10})
workbook.close()
```
效果如下：<br />![](./img/1635944857383-b8742e25-e710-4b95-bdfb-073b255ef4b7.webp)
<a name="qIJoH"></a>
### 6.6 实战训练
<a name="zyiQ1"></a>
#### 1.xlswriter新建并写入Excel
程序示例：
```python
# 3.6.2 xlswriter新建并写入Excel
def fun3_6_2():
    # 创建Exce并添加sheet
    workbook = xlsxwriter.Workbook('demo.xlsx')
    worksheet = workbook.add_worksheet()

    # 设置列宽
    worksheet.set_column('A:A', 20)

    # 设置格式
    bold = workbook.add_format({'bold': True})

    # 添加文字内容
    worksheet.write('A1', 'Hello')

    # 按格式添加内容
    worksheet.write('A2', 'World', bold)

    # 写一些数字
    worksheet.write(2, 0, 123)
    worksheet.write(3, 0, 123.456)

    # 添加图片
    worksheet.insert_image('B5', 'demo.png')

    workbook.close()
```
效果如下：<br />![](./img/1635944857441-0f0db81d-ebfd-4238-b423-6a210d95cabf.webp)
<a name="oYUr1"></a>
## 7、Python win32com 读取 写入 修改 操作Excel
python可以使用一个第三方库叫做win32com达到操作com的目的，win32com功能强大，可以操作word、调用宏等等等。
<a name="Qkab6"></a>
### 7.1 pip安装win32com
```bash
pip install pypiwin32
```
由于已经安装过了，所以提示已经安装：<br />![](./img/1635944857566-daee3b03-727d-4db9-acb4-523cb93b4689.webp)
<a name="wXfPK"></a>
### 7.2 Python使用win32com读写Excel
程序示例：
```python
import win32com
from win32com.client import Dispatch, constants
import os

# 获取当前脚本路径
def getScriptPath():
    nowpath = os.path.split(os.path.realpath(__file__))[0]
    print(nowpath)
    return nowpath

# 3.7.2 Python使用win32com读写Excel
def fun3_7_2():
    app = win32com.client.Dispatch('Excel.Application')

    # 后台运行，不显示，不警告
    app.Visible = 0
    app.DisplayAlerts = 0

    # 创建新的Excel
    # WorkBook = app.Workbooks.Add()
    # 新建sheet
    # sheet = WorkBook.Worksheets.Add()

    # 打开已存在表格，注意这里要用绝对路径
    WorkBook = app.Workbooks.Open(getScriptPath() + "\\3_7 win32com 修改操作练习.xlsx")
    sheet = WorkBook.Worksheets('Sheet1')

    # 获取单元格信息 第n行n列，不用-1
    cell01_value = sheet.Cells(1,2).Value
    print("cell01的内容为：",cell01_value)

    # 写入表格信息
    sheet.Cells(2, 1).Value = "win32com"

    # 保存表格
    #WorkBook.Save()

    # 另存为实现拷贝
    WorkBook.SaveAs(getScriptPath() + "\\new.xlsx")

    # 关闭表格
    WorkBook.Close()
    app.Quit()


if __name__ == '__main__':
    fun3_7_2()
```
效果如下：<br />![](./img/1635944857477-3282a7bf-8aa4-4129-be93-ded0107ee9ac.webp)<br />内容为：<br />![](./img/1635944857651-7a1b4507-a048-4522-b475-0abffed734de.webp)
<a name="jCDSv"></a>
## 8、Python pandas 读取 写入 操作Excel
简介：<br />pandas 是基于NumPy 的一种工具，该工具是为了解决数据分析任务而创建的。Pandas 纳入了大量库和一些标准的数据模型，提供了高效地操作大型数据集所需的工具。pandas提供了大量能快速便捷地处理数据的函数和方法。它是使Python成为强大而高效的数据分析环境的重要因素之一。<br />官方网站：[https://pandas.pydata.org/](https://pandas.pydata.org/)<br />官方文档：[https://pandas.pydata.org/pandas-docs/stable/](https://pandas.pydata.org/pandas-docs/stable/)
<a name="ANHbM"></a>
### 8.1 pip安装pandas
```bash
pip install pandas
```
<a name="uWHBR"></a>
### 8.2 pandas 读写 Excel
表格内容如下：<br />![](./img/1635944857691-5095d4b8-9130-47cc-a556-72436b58c2d6.webp)<br />程序示例：
```python
import pandas as pd
from pandas import DataFrame

# 3.8.2 pandas读写Excel
def fun3_8_2():
    data = pd.read_excel('3_8 pandas 修改操作练习.xlsx', sheet_name='Sheet1')
    print(data)

    # 增加行数据，在第5行新增
    data.loc[4] = ['4', 'john', 'pandas']

    # 增加列数据，给定默认值None
    data['new_col'] = None

    # 保存数据
    DataFrame(data).to_excel('new.xlsx', sheet_name='Sheet1', index=False, header=True)


if __name__ == '__main__':
    fun3_8_2()
```
效果如下：<br />![](./img/1635944857739-1dafc46c-0bee-4c0f-8a4b-e7b12e5e080b.webp)<br />生成的excel如下：<br />![](./img/1635944857825-ee336051-f39c-43ef-875f-71720c605c92.webp)<br />pandas功能非常强大，这里只是做了又给很简单的示例，还有很多其它操作，可参考官方文档或快速入门进行学习。
