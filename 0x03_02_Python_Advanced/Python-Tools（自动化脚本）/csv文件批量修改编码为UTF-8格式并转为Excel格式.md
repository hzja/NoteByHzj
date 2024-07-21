Python csv 编码 <br />有时候用excel打开一个csv文件，中文全部显示乱码。然后手动用notepad++打开，修改编码为utf-8并保存后，再用excel打开显示正常。<br />使用Python通过很少代码就能将上面过程自动化。
<a name="i8gxv"></a>
### 导入3个模块
```python
# coding: utf-8
# @author: Fcant
# @date: 2020-12-16
# @describe: functions about automatic file processing
import pandas as pd  
import os 
import chardet
```
chardet 模块用于得到文件的编码格式，pandas 按照这个格式读取，然后保存为xlsx格式。
<a name="rpfRB"></a>
### 获取filename文件的编码格式
```python
def get_encoding(filename):
    """
    返回文件编码格式
    """
    with open(filename,'rb') as f:
        return chardet.detect(f.read())['encoding']
```
<a name="8Wn36"></a>
### 文件转码处理
保存为utf-8编码xlsx格式文件，支持csv, xls, xlsx 格式的文件乱码处理。需要注意，如果读入文件为csv格式，保存时要使用xlsx格式：
```python
def to_utf8(filename):
    """
    保存为 to_utf-8
    """
    encoding = get_encoding(filename)
    ext = os.path.splitext(filename)
    if ext[1] =='.csv':
        if 'gb' in encoding or 'GB' in encoding:
            df = pd.read_csv(filename,engine='python',encoding='GBK')
        else:
            df = pd.read_csv(filename,engine='python',encoding='utf-8')
        df.to_excel(ext[0]+'.xlsx')
    elif ext[1]=='.xls' or ext[1] == '.xlsx':
        if 'gb' in encoding or 'GB' in encoding:
            df = pd.read_excel(filename,encoding='GBK')
        else:
            df = pd.read_excel(filename,encoding='utf-8')
        df.to_excel(filename)
    else:
        print('only support csv, xls, xlsx format')
```
<a name="JR1qc"></a>
### 批量转换
上面函数实现单个文件转化，下面batch_to_utf8 实现目录 path 下所有后缀为ext_name文件的批量乱码转化：
```python
def batch_to_utf8(path,ext_name='csv'):
    """
    path下，后缀为 ext_name的乱码文件，批量转化为可读文件
    """
    for file in os.listdir(path):
        if os.path.splitext(file)[1]=='.'+ext_name:
            to_utf8(os.path.join(path,file))
```
<a name="fSQHj"></a>
### 使用
```python
if __name__ == '__main__':
  batch_to_utf8('.') # 对当前目录下的所有csv文件保存为xlsx格式,utf-8编码的文件
```
