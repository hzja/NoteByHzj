Python Excel文件
<a name="3e90c511"></a>
### 常用工具
数据处理是 Python 的一大应用场景，Python 处理 Excel 已有很多现成的轮子，比如 `xlrd` 、 `xlwt` 、 `xlutils` 、 `XlsxWriter` 、 `OpenPyXL` ，而在 Windows 平台上可以直接调用 Microsoft Excel 的开放接口，这些都是比较常用的工具，接下来通过一个表格展示各工具之间的特点：

| 类型 | xlrd&xlwt&xlutils | XlsxWriter | OpenPyXL | Excel开放接口 |
| :--- | :--- | :--- | :--- | :--- |
| 读取 | 支持 | 不支持 | 支持 | 支持 |
| 写入 | 支持 | 支持 | 支持 | 支持 |
| 修改 | 支持 | 不支持 | 支持 | 支持 |
| xls | 支持 | 不支持 | 不支持 | 支持 |
| xlsx | 高版本 | 支持 | 支持 | 支持 |
| 大文件 | 不支持 | 支持 | 支持 | 不支持 |
| 效率 | 快 | 快 | 快 | 超慢 |
| 功能 | 较弱 | 强大 | 一般 | 超强大 |

以上可以根据需求不同，选择合适的工具，现在为大家主要介绍下最常用的 xlrd & xlwt & xlutils 系列工具的使用。
<a name="tHoKK"></a>
### xlrd & xlwt & xlutils 介绍
xlrd&xlwt&xlutils 是由以下三个库组成：

- `xlrd`：用于读取 Excel 文件；
- `xlwt`：用于写入 Excel 文件；
- `xlutils`：用于操作 Excel 文件的实用工具，比如复制、分割、筛选等；
<a name="LHwuJ"></a>
### 安装库
安装比较简单，直接用 pip 工具安装三个库即可，安装命令如下：
```bash
$ pip install xlrd xlwt xlutils
```
<a name="rZu1W"></a>
### 写入 Excel
接下来从写入 Excel 开始，看代码：
```python
# 导入 xlwt 库
import xlwt
# 创建 xls 文件对象
wb = xlwt.Workbook()
# 新增两个表单页
sh1 = wb.add_sheet('成绩')
sh2 = wb.add_sheet('汇总')
# 然后按照位置来添加数据,第一个参数是行，第二个参数是列
# 写入第一个sheet
sh1.write(0, 0, '姓名')
sh1.write(0, 1, '专业')
sh1.write(0, 2, '科目')
sh1.write(0, 3, '成绩')
sh1.write(1, 0, '张三')
sh1.write(1, 1, '信息与通信工程')
sh1.write(1, 2, '数值分析')
sh1.write(1, 3, 88)
sh1.write(2, 0, '李四')
sh1.write(2, 1, '物联网工程')
sh1.write(2, 2, '数字信号处理分析')
sh1.write(2, 3, 95)
sh1.write(3, 0, '王华')
sh1.write(3, 1, '电子与通信工程')
sh1.write(3, 2, '模糊数学')
sh1.write(3, 3, 90)
# 写入第二个sheet
sh2.write(0, 0, '总分')
sh2.write(1, 0, 273)
# 最后保存文件即可
wb.save('test.xls')
```
运行代码，结果会看到生成名为 test.xls 的 Excel 文件
<a name="H3ldf"></a>
### 读取 Excel
读取 Excel 的操作看如下代码：
```python
# 导入 xlrd 库
import xlrd
# 打开刚才我们写入的 test_w.xls 文件
wb = xlrd.open_workbook("test_w.xls")
# 获取并打印 sheet 数量
print( "sheet 数量:", wb.nsheets)
# 获取并打印 sheet 名称
print( "sheet 名称:", wb.sheet_names())
# 根据 sheet 索引获取内容
sh1 = wb.sheet_by_index(0)
# 或者
# 也可根据 sheet 名称获取内容
# sh = wb.sheet_by_name('成绩')
# 获取并打印该 sheet 行数和列数
print( u"sheet %s 共 %d 行 %d 列" % (sh1.name, sh1.nrows, sh1.ncols))
# 获取并打印某个单元格的值
print( "第一行第二列的值为:", sh1.cell_value(0, 1))
# 获取整行或整列的值
rows = sh1.row_values(0) # 获取第一行内容
cols = sh1.col_values(1) # 获取第二列内容
# 打印获取的行列值
print( "第一行的值为:", rows)
print( "第二列的值为:", cols)
# 获取单元格内容的数据类型
print( "第二行第一列的值类型为:", sh1.cell(1, 0).ctype)
# 遍历所有表单内容
for sh in wb.sheets():
    for r in range(sh.nrows):
        # 输出指定行
        print( sh.row(r))
```
这里可以获取到**单元格的类型**，上面读取类型时获取的是`数字1`，那1表示什么类型，又都有什么类型呢？通用类型如下：

| 数值 | 类型 | 说明 |
| :--- | :--- | :--- |
| 0 | empty | 空 |
| 1 | string | 字符串 |
| 2 | number | 数字 |
| 3 | date | 日期 |
| 4 | boolean | 布尔值 |
| 5 | error | 错误 |

通过上面表格，可以知道刚获取单元格类型返回的数字1对应的就是字符串类型。
<a name="4wUl4"></a>
### 修改 excel
修改时就需要用到 xlutils 中的方法。看下最简单的修改操作：
```python
# 导入相应模块
import xlrd
from xlutils.copy import copy
# 打开 excel 文件
readbook = xlrd.open_workbook("test_w.xls")
# 复制一份
wb = copy(readbook)
# 选取第一个表单
sh1 = wb.get_sheet(0)
# 在第五行新增写入数据
sh1.write(4, 0, '王欢')
sh1.write(4, 1, '通信工程')
sh1.write(4, 2, '机器学习')
sh1.write(4, 3, 89)
# 选取第二个表单
sh1 = wb.get_sheet(1)
# 替换总成绩数据
sh1.write(1, 0, 362)
# 保存
wb.save('test.xls')
```
从上面代码可以看出，这里的修改 Excel 是通过 xlutils 库的 copy 方法将原来的 Excel 整个复制一份，然后再做修改操作，最后再保存。
<a name="eM8qt"></a>
### 格式转换操作
在平时使用 Excel 时会对数据进行一下格式化，或者样式设置，在这里把上面介绍写入的代码简单修改下，使输出的格式稍微改变一下，代码如下：
```python
# 导入 xlwt 库
import xlwt
# 设置写出格式字体红色加粗
styleBR = xlwt.easyxf('font: name Times New Roman, color-index red, bold on')
# 设置数字型格式为小数点后保留两位
styleNum = xlwt.easyxf(num_format_str='#,##0.00')
# 设置日期型格式显示为YYYY-MM-DD
styleDate = xlwt.easyxf(num_format_str='YYYY-MM-DD')
# 创建 xls 文件对象
wb = xlwt.Workbook()
# 新增两个表单页
sh1 = wb.add_sheet('成绩')
sh2 = wb.add_sheet('汇总')
# 然后按照位置来添加数据,第一个参数是行，第二个参数是列
sh1.write(0, 0, '姓名', styleBR)   # 设置表头字体为红色加粗
sh1.write(0, 1, '日期', styleBR)   # 设置表头字体为红色加粗
sh1.write(0, 2, '成绩', styleBR)   # 设置表头字体为红色加粗
# 插入数据
sh1.write(1, 0, '张三',)
sh1.write(1, 1, '2020-07-01', styleDate)
sh1.write(1, 2, 90, styleNum)
sh1.write(2, 0, '李四')
sh1.write(2, 1, '2020-08-02')
sh1.write(2, 2, 95, styleNum)
# 设置单元格内容居中的格式
alignment = xlwt.Alignment()
alignment.horz = xlwt.Alignment.HORZ_CENTER
style = xlwt.XFStyle()
style.alignment = alignment
# 合并A4,B4单元格，并将内容设置为居中
sh1.write_merge(3, 3, 0, 1, '总分', style)
# 通过公式，计算C2+C3单元格的和
sh1.write(3, 2, xlwt.Formula("C2+C3"))
# 对 sheet2 写入数据
sh2.write(0, 0, '总分', styleBR)
sh2.write(1, 0, 185)
# 最后保存文件即可
wb.save('test.xls')
```
可以看出，使用代码可以对字体，颜色、对齐、合并等平时 Excel 的操作进行设置，也可以格式化日期和数字类型的数据。<br />想了解更多功能操作可以参考官网。
> python-excel官网：www.python-excel.org/

