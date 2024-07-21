Python

- 文本文件
- CSV文件
- Excel文件
- 压缩文件
- XML文件
- JSON文件
- PDF文件
<a name="idYu1"></a>
## 创建文本文件
文本文件本质上是以.txt后缀结尾的文件，在Python当中创建文本文件是非常的容易和简单的，代码如下
```python
# 通过Python来创建文本文件

# 选择文件的名称
file_name = "my_text_file.txt"

# 将内容写入至文件中
texts = "Hi, How do you doing?"

# 打开文件，编辑模式
with open(file_name, "w") as file:
 file.write(texts)

# 打印创建文本文件成功的信息
print(f"{file_name} 创建成功！")
```
当然了要是想要来读取该文件当中的内容，代码也非常的简单，如下所示
```python

# 用Python读取文本文件

# 文件的名称
file_name = "my_text_file.txt"

# 打开文件，读取当中的内容
with open(file_name, "r") as file:
 texts = file.read()

# 打印文本文件中的内容
print(texts)
```
<a name="tyIRm"></a>
## 创建CSV文件
CSV文件在日常生活学习当中也是非常常用的，要想创建CSV文件，这里就需要用到Python当中的CSV模块，代码也是十分的简单，如下所示
```python
import csv

# 文件的名称
file_name = "my_csv_file.csv"

# 打开文件，并且是编辑的模式
with open(file_name, "w") as file:
    writer = csv.writer(file)
    # 编辑内容
    writer.writerow(["Number", "Name", "Company"])
    writer.writerow([1, "Elon Musk", "Tesla"])
    writer.writerow([2, "Bill Gates", "Microsoft"])
    writer.writerow([3, "Jeff Bezos", "Amazon"])
    writer.writerow([4, "Steve Jobs", "Apple"])

# 打印编辑成功的消息
print(f"{file_name} 创建成功！")
```
当然了这里还可以调用`writerows()`方法，代码如下
```python
# CSV文件的名称
file_name = "my_csv_file.csv"

# 写入的内容
content = [
    ["Number", "Name", "Company"],
    [1, "Elon Musk", "Tesla"],
    [2, "Bill Gates", "Microsoft"],
    [3, "Jeff Bezos", "Amazon"],
    [4, "Steve Jobs", "Apple"]
]

# 打开文件并且开启编辑的模式
with open(file_name, "w") as file:
    # 创建编辑的工具
    writer = csv.writer(file)
    writer.writerows(content)

# 打印出文件创建成功的消息
print(f"{file_name} 创建成功！")
```
那么当要来读取该CSV文件的时候，就需要下面的这个代码，如下所示
```python
# CSV文件的名称
file_name = "my_csv_file.csv"

# 打开文件，读取文件的模式
with open(file_name, "r") as file:
    # 创建读取文件的工具
    reader = csv.reader(file)
    for record in reader:
        print(record)
```
output
```
['Number', 'Name', 'Company']
['1', 'Elon Musk', 'Tesla']
['2', 'Bill Gates', 'Microsoft']
['3', 'Jeff Bezos', 'Amazon']
['4', 'Steve Jobs', 'Apple']
```
<a name="pnubV"></a>
## 创建Excel文件
这里需要用到openpyxl模块，要是你还没有下载该模块，这里就需要用到pip命令来实现模块的安装了，如下
```bash
pip install openpyxl
```
创建Excel文件的方式也特别简单，代码如下
```python
# 导入模块
from openpyxl import Workbook

# Excel文件的文件名
file_name = "my_excel_file.xlsx"

# 创建文件的工具
workbook = Workbook()
sheet_1 = workbook.active
```
编辑Excel文件的方式有三种，这里一一来介绍一下，第一种是在指定的单元格当中放入需要填充的内容，代码如下
```python
# 通过指定单元格的位置来填充内容
# 第一条记录
sheet_1["A1"] = "S. no"
sheet_1["B1"] = "Name"
sheet_1["C1"] = "Company"

# 第二条记录
sheet_1["A2"] = "1"
sheet_1["B2"] = "Elon Musk"
sheet_1["C2"] = "Tesla"

# 第三条记录
sheet_1["A3"] = "2"
sheet_1["B3"] = "Bill Gates"
sheet_1["C3"] = "Microsoft"

# 第四条记录
sheet_1["A4"] = "3"
sheet_1["B4"] = "Jeff Bezos"
sheet_1["C4"] = "Amazon"

# 保存文件
workbook.save(file_name)
```
方法二的话和方法一是十分相类似的，代码如下
```python
# 通过指定单元格的位置来填充内容
# 第一条记录
sheet_1.cell(row=1, column=1).value = "Number"
sheet_1.cell(row=1, column=1).value = "Name"
sheet_1.cell(row=1, column=1).value = "Company"

# 第二条记录
sheet_1.cell(row=1, column=1).value = "1"
sheet_1.cell(row=1, column=1).value = "Elon Musk"
sheet_1.cell(row=1, column=1).value = "Tesla"

# 第三条记录
sheet_1.cell(row=1, column=1).value = "2"
sheet_1.cell(row=1, column=1).value = "Bill Gates"
sheet_1.cell(row=1, column=1).value = "Microsoft"

# 第四条记录
sheet_1.cell(row=1, column=1).value = "3"
sheet_1.cell(row=1, column=1).value = "Jeff Bezos"
sheet_1.cell(row=1, column=1).value = "Amazon"

# 保存文件
workbook.save(file_name)
```
方法三，通过调用`append()`方法来实现往Excel文件当中填充内容，代码如下
```python
# 一系列的数据内容
records = [
    ["Number", "Name", "Company"],
    [1, "Elon Musk", "Tesla"],
    [2, "Bill Gates", "Microsoft"],
    [3, "Jeff Bezos", "Amazon"]
]

# append()方法填充内容
for record in records:
    sheet_1.append(record)

# 保存文件
workbook.save(file_name)
```
而要是来读取一下文件当中的内容，代码如下，这里需要用到`iter_rows()`
```python
# 导入模块
import openpyxl

# 文件名的名称
file_name = "my_excel_file.xlsx"

# 加载Excel文件
workbook = openpyxl.load_workbook(file_name)
sheet_1 = workbook.active

# 开始读取的行的位置
min_row = sheet_1.min_row
# 读取行的位置读取到哪儿
max_row = sheet_1.max_row

# 开始读取单元格的位置
min_col = sheet_1.min_column
# 单元格读取的最终的位置
max_col = sheet_1.max_column

for record in sheet_1.iter_rows(min_row, max_row, min_col, max_col):
    for cell in record:
        print(cell.value, end=" ")
    print()
```
output
```python
Number Name Company 
1 Elon Musk Tesla 
2 Bill Gates Microsoft 
3 Jeff Bezos Amazon
```
<a name="iHVuv"></a>
## 创建压缩文件
在Python当中来创建压缩文件是非常简单的，这里需要用到tarfile模块，代码如下
```python
# 导入模块
import tarfile

# 打开压缩文件，编辑模式
with tarfile.open("my_tar_file.tar", "w") as file:
    # 添加txt文本文件，并且指定文件的名称
    file.add("text_file_1.txt")
    file.add("text_file_2.txt")

# 打印创建成功的消息
print("压缩包创建成功！")
```
要是想要解压缩 压缩包当中的文件，这里就可以用到`extractall()`方法，代码如下
```python
# 导入模块
import tarfile

# 解压缩文件的名称
filename = "my_tar_file.tar"

# 查看文件是否是压缩文件的格式
if tarfile.is_tarfile(filename):
    
    # 打开文件
    with tarfile.open(filename, "r") as file:
        # 解压缩全部的文件
        file.extractall()
        
    # 打印解压缩成功的消息
    print(f"{filename} 解压缩成功！")
```
那么要是想要打印出来解压缩文件当中的文件名，代码也非常的简单，如下所示
```python
# 导入模块
import tarfile

# 解压缩包的名称
filename = "my_tar_file.tar"

# 检查文件是否是压缩包的格式
if tarfile.is_tarfile(filename):
    
    # 打开文件
    with tarfile.open(filename, "r") as file:
        # 解压缩文件当中的文件名称
        files = file.getnames()
        # 遍历循环文件名
        for file in files:
            print(file)
```
从压缩包中，解压缩出指定的文本文件，代码如下
```python
# 导入模块
import tarfile

# 解压缩包的名称
filename = "my_tar_file.tar"

# 检查文件是否是压缩包的格式
if tarfile.is_tarfile(filename):
    
    # 打开文件
    with tarfile.open(filename, "r") as file:
        # 获取解压缩包中文件的名称
        files = file.getnames()
        
        # 输入你想要解压缩的文件名
        fname_to_extract = "text_file_1.txt"
        # 检查文本文件是否存在
        if fname_to_extract in files:
            # 解压缩指定的文件出来
            file.extract(fname_to_extract)   
        
    # 打印成功
    print("File extracted successfully")
```
<a name="wzqmB"></a>
## 创建XML文件
在Python当中创建XML文件就需要用到xml模块，这里定义一个函数来创建XML文件，代码如下
```python
# 导入模块
import xml.etree.ElementTree as xml

# 函数方法来创建XML文件
def create_XML_file(filename):
    # Root element -> Mobile_shop
    root = xml.Element("Mobile_shop")
    
    # Root element -> mobile_names
    mobile = xml.Element("mobile_names")
    root.append(mobile)
    
    # Sub Elements -> mobile names
    mobile_1 = xml.SubElement(mobile, "mobile_1")
    mobile_1.text = "Vivo"
    
    mobile_2 = xml.SubElement(mobile, "mobile_2")
    mobile_2.text = "Oppo"
    
    mobile_3 = xml.SubElement(mobile, "mobile_3")
    mobile_3.text = "Realme"
    
    mobile_4 = xml.SubElement(mobile, "mobile_4")
    mobile_4.text = "Google Pixel"
    
    mobile_5 = xml.SubElement(mobile, "mobile_5")
    mobile_5.text = "IPhone"
    
    mobile_6 = xml.SubElement(mobile, "mobile_6")
    mobile_6.text = "Sony"
    
    tree = xml.ElementTree(root)
    
    # 将内容写入至XML文件中
    with open(filename, "wb") as file:
        tree.write(file)
        
    # 打印创建成功的信息
    print("创建成功！")
    
# 调用上述定义的函数
create_XML_file("my_xml_file.xml")
```
output
```xml
<Mobile_shop><mobile_names><mobile_1>Vivo</mobile_1><mobile_2>Oppo</mobile_2><mobile_3>Realme</mobile_3><mobile_4>Google Pixel</mobile_4><mobile_5>Iphone</mobile_5><mobile_6>Sony</mobile_6></mobile_names></Mobile_shop>
```
<a name="rB9Qy"></a>
## 创建JSON文件
创建JSON文件的模块在Python当中也叫json，那么往JSON文件中写入内容这里需要用到`dump()`方法，代码如下
```python
# 导入模块
import json

# 键值对来写入至JSON文件中
data = {
    "name": "junxin",
    "profession": "Blogging",
    "programming_language": "Python",
}

# 创建文件，并且以编辑的模式打开
with open("my_json_file.json", "w") as file:
    # 这里调用到dump()方法来写入内容
    json.dump(data, file)
    
# 打印创建成功的信息
print("创建成功！")
```
而当想要来查看JSON文件当中的内容，这里就需要调用`load()`方法，代码如下
```python
# 导入模块
import json

# 打开文件
with open("my_json_file.json", "r") as file:
    # 调用load()方法
    content = json.load(file)

# 打印内容
print(content)
```
output
```json
{'name': 'Gowtham', 'profession': 'Blogging', 'programming_lang_known': 'Python'}
```
<a name="wnLI6"></a>
## 创建PDF文件
如果是要创建PDF文件的话，这里就需要用到fpdf模块，这里用到pip命令来进行该模块的下载，
```bash
pip install fpdf
```
尝试往PDF文件中写入内容，代码如下
```python
# 导入模块
import fpdf

# 创建实例
pdf = fpdf.FPDF()

# 添加一页内容
pdf.add_page()

# 设置字体的格式
pdf.set_font("arial")

# 设置字体的大小
pdf.set_font_size(18)

# 写入内容
pdf.cell(w=0, txt="Hello World!!", ln=0)

# 保存内容
pdf.output("my_pdf.pdf")
```
