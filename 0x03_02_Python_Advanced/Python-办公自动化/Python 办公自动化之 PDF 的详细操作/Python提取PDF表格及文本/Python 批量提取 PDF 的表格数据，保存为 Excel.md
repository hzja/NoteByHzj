Python
<a name="cxZUJ"></a>
### 一、pdfplumber 库
pdfplumber 是一个开源 Python 工具库，可以方便获取 PDF 的各种信息，包括文本、表格、图表、尺寸等。这里主要使用 pdfplumber 提取 PDF 表格数据。
<a name="E9q33"></a>
#### 安装命令
```bash
pip install pdfplumber
```
<a name="TMxie"></a>
### 二、代码实现
<a name="teL6r"></a>
#### 导入相关包
```python
import pdfplumber
import pandas as pd
```
<a name="tAG2r"></a>
#### 读取 PDF，并获取 PDF 的页数
```python
pdf = pdfplumber.open("/Users/fcant/Desktop/1.pdf")
pages = pdf.pages
```
<a name="hstg5"></a>
#### 提取单个 PDF 文件，保存成 Excel
```python
if len(pages) > 1:
    tables = []
    for each in pages:
        table = each.extract_table()
        tables.extend(table)
else:
    tables = each.extract_table()

data = pd.DataFrame(tables[1:], columns=tables[0])
data
data.to_excel("/Users/wangwangyuqing/Desktop/1.xlsx", index=False)
```
<a name="ZLSl3"></a>
#### 提取文件夹下多个 PDF 文件，保存成 Excel
```python
import os
import glob

path = r'/Users/wangwangyuqing/Desktop/pdf文件'
for f in glob.glob(os.path.join(path, "*.pdf")):
    res = save_pdf_to_excel(f)
    print(res)


def save_pdf_to_excel(path):
    #     print('文件名为：',path.split('/')[-1].split('.')[0] + '.xlsx')

    pdf = pdfplumber.open(path)
    pages = pdf.pages
    if len(pages) > 1:
        tables = []
        for each in pages:
            table = each.extract_table()
            tables.extend(table)
    else:
        tables = each.extract_table()
    data = pd.DataFrame(tables[1:], columns=tables[0])

    file_name = path.split('/')[-1].split('.')[0] + '.xlsx'
    data.to_excel("/Users/wangwangyuqing/Desktop/data/{}".format(file_name), index=False)
    return '保存成功！'
```
<a name="Lj51x"></a>
### 三、小结
Python 中还有很多库可以处理 pdf，比如 PyPDF2、pdfminer 等，本文选择 pdfplumber 的原因在于能轻松访问有关 PDF 的所有详细信息，包括作者、来源、日期等，并且用于提取文本和表格的方法灵活可定制。大家可以根据手头数据需求，再去解锁 pdfplumber 的更多用法。
