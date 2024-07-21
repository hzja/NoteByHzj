Python Excel
<a name="Vedf2"></a>
### 批量新建并保存工作簿
```python
import xlwings as xw     # 导入库
# 启动Excel程序，但不新建工作簿
app = xw.App(visible = True, add_book = False)
  
for i in range(6):
    workbook = app.books.add()   # 新建工作簿
    workbook.save(f'e:\\file\\test{i}.xlsx')    # 保存新建的多个工作簿
    workbook.close()     # 关闭当前工作簿
app.quit()     # 退出Excel程序
```
上面的代码是对工作簿最基本的操作，新建工作簿和保存工作簿，还有关闭当前工作簿。
<a name="vjkiS"></a>
### 列出文件夹下所有文件和子文件夹的名称
方便总结和查看文件的。
```python
import os
file_path = 'table'
file_list = os.listdir(file_path)
for i in file_list:
    print(i)
```
<a name="DfaNg"></a>
### 批量重命名一个工作簿中的所有工作表
这里面最重要的功能是对工作表`worksheets`进行操作。
```python
import xlwings as xw
app = xw.App(visible = False, add_book = False)
workbook = app.books.open('e:\\table\\统计表.xlsx')
worksheets = workbook.sheets     # 获取工作簿中所有的工作表
  
for i in range(len(worksheets)):   # 遍历获取到的工作表
    worksheets[i].name = worksheets[i].name.replace('销售', '')     # 重命名工作表
workbook.save('e:\\table\\统计表1.xlsx')       # 另存重命名工作表后的工作簿
app.quit()
```
<a name="PTKmu"></a>
### 批量打印工作表
这里面用到的是`workbook.api.PrintOut()`，也就是调用打印机
```python
import os
import xlwings as xw
  
file_path = 'e:\\table\\公司'
file_list = os.listdir(file_path)
app = xw.App(visible = False, add_book = False)
  
for i in file_list:
    if i.startswith('~$'):
        continue
         
    file_paths = os.path.join(file_path, i)   # 获取需要打印的工作簿的文件路径
    workbook = app.books.open(file_paths)    # 打开要打印的工作簿
     
    workbook.api.PrintOut()   # 打印工作簿
app.quit()
```
<a name="Nq5OG"></a>
### 拆分工作簿
原理比较简单，遍历原有工作簿中的工作表，然后把他们都`copy`到新的工作簿工作表中，然后再保存起来就行了。
```python
import xlwings as xw
workbook_name = 'e:\\table\\产品销售表.xlsx'        # 指定要拆分的来源工作簿
app = xw.App(visible = False, add_book = False)
  
header = None   
all_data = []  
workbook = app.books.open(workbook_name)
  
for i in workbook.sheets:                 # 遍历来源工作簿中的工作表
    workbook_split = app.books.add()      # 新建一个目标工作簿
    sheet_split = workbook_split.sheets[0]     # 选择目标工作簿中的第一个工作表
    i.api.Copy(Before = sheet_split.api)       # 将来源工作簿中的当前工作表复制到目标工作簿的第一个工作表之前
    workbook_split.save('{}'.format(i.name))   # 以当前工作表的名称作为文件名保存目标工作簿
app.quit()
```

