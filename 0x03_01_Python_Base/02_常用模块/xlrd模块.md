# xlrd

+ 官方文档：[XLRD文档](https://xlrd.readthedocs.io/en/latest/index.html)



## 简介

xlrd 是一个用于从 Excel 读取数据和格式化信息的库`.xls`历史格式的文件



以下内容也不受支持，但将安全可靠地忽略：

- 图表、宏、图片、任何其他嵌入对象，**包括**嵌入的工作表。
- VBA 模块
- 公式，但提取公式计算的结果。
- 评论
- 超链接
- 自动筛选器、高级筛选器、数据透视表、条件格式、数据验证

+ 不支持受密码保护的文件，并且此库无法读取这些文件。



## 快速入门

~~~ shell
pip install xlrd
~~~

```python

import xlrd
book = xlrd.open_workbook("myfile.xls")
print("The number of worksheets is {0}".format(book.nsheets))
print("Worksheet name(s): {0}".format(book.sheet_names()))
sh = book.sheet_by_index(0)
print("{0} {1} {2}".format(sh.name, sh.nrows, sh.ncols))
print("Cell D30 is {0}".format(sh.cell_value(rowx=29, colx=3)))
for rx in range(sh.nrows):
    print(sh.row(rx))
```

在命令行中，这将显示每个文件中每个工作表的第一、第二和最后一行：

```shell
python PYDIR/scripts/runxlrd.py 3rows *blah*.xls
```



## 少量功能

+ 索引获取
  table = data.sheet()[0]
  table = data.sheet_by_index(0)



+ 通过名称获取
  table = data.sheet_by_name(u’sheet1’)



+ 获取整行和整列的值（数组)
  table.row_values(i)
  table.col_values(i)



+ 获取行数和列数
  nrows = table.nrows
  ncols = table.ncols



+ 循环行列表数据
  for i in range(nrows):
  print table.row_values(i)



+ 单元格
  cell_A1 = table.cell(0,0).value



+ 使用行列索引
  cell_A1 = table.cell(0,0).value



~~~ tex
# 打开文件
x1 = xlrd.open_workbook(filename)

# 获取sheet对象
x1.sheet_names()           # 获取所有sheet名字
x1.nsheets                     # 获取sheet数量
x1.sheets()                     # 获取所有sheet对象
x1.sheet_by_name("test")      # 通过sheet名查找
ds = x1.sheet_by_index(3)      # 通过索引查找 

# 获取sheet的汇总数据
ds.name           #获取sheet名
ds.nrows          #获取总行数
ds.ncols           #获取总列数

# 行操作
ds.row_values(0)      #获取第一行所有内容
ds.row(0)             #获取单元格值类型和内容
ds.row_types(0)        #获取单元格数据类型

# 表操作
ds.row_values(0,6,10)      #取第一行，第6-9列
ds.col_values(0,0,5)         #取第一列，第0-4行
ds.row_slice(2,0,2)           #获取单元格值类型和内容，同sheet1.row()
sheet1.row_types(1,0,2)   #获取单元格数据类型

# 特定单元格读取或获取单元类型
ds.cell_value(1,2)
ds.cell(1,2).value
ds.row(1)[2].value
ds.cell(1,2).ctype
ds.cell_type(1,2)
ds.row(1)[2].ctype

# 单元格位置表达方式
xlrd.cellname(0,0)          #将（0，0）转化为A1
xlrd.cellnameabs(0,0)   #将（0，0）转化为SAS1
xlrd.colname(30)          #将列由数字转换为字母表示

数据类型：
空：0
字符串：1
数字：2
日期：3
布尔：4
error：5
~~~



关于其他更深层次可查看官方文档

