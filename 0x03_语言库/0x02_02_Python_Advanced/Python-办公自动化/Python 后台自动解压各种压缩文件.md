Python 
<a name="n4qb9"></a>
### 一、需求描述
每次下载压缩包形式的文件后，自动将内部文件解压到当前文件夹后将压缩包删除，需要用到的模块有：

- `os` 模块
- `glob` 模块
- 利用 `gzip` `zipfile` `rarfile` `tarfile` 模块解压文件
<a name="AvDpD"></a>
### 二、步骤分析和前置知识
实现的逻辑为：

1. 定时检测某个文件夹（如 download 文件夹）是否存在压缩文件
2. 如果有新出现的压缩文件则将其解压到当前文件夹内的新文件夹中删除压缩文件

压缩文件就要分不同压缩格式进行分析，主要有以下4种：
> `.gz`: 即gzip，通常只能压缩一个文件
> `.tar`: 本质上不是压缩工具而是打包工具，可以跟.gz配合形成.tar.gz的打包压缩格式
> `.zip`: 和.tar.gz很类似但可以打包压缩多个文件
> `.rar`: 打包压缩文件，最初用于DOS

因此，文件是否为压缩文件的判断逻辑可以如下：

1. 建立一个含压缩包后缀的列表 `compressed_lst = ['gz', 'tar', 'zip', 'rar']`
2. 目标文件夹下的所有文件名运行`filename.split(.)[-1]`获取后缀名
3. 判断后缀名是否在`compressed_lst`中，如果存在则运行后续的解压代码
4. 如果是以`gz`结尾的文件，解压后需要再次判断是否是以`.tar`结尾，并进行相应处理

不同压缩文件的解压代码各不相同，在后续代码操作中具体展开。
<a name="4c100f22"></a>
### 三、代码实现
首先是获取 `download` 文件夹下所有文件的文件名
```python
import glob
import os
path = r'C:\xxxx\download'
file_lst = glob.glob(path + '/*')
# 列表推导式
filename_lst = [os.path.basename(i) for i in file_lst]
print(filename_lst)
```
接下来根据文件的后缀名判断是否需要压缩，先看一下代码框架
```python
for i in filename_lst:
    if '.' in i:
        # 获取后缀名
        suffix = i.split('.')[-1]
        # 逐个将后缀名和各格式的压缩后缀匹配
        if suffix == 'gz':
            pass
        if ...: # 再次判断新产生的文件名是否以 .tar 结尾
            pass
        if suffix == 'tar':
            pass
        if suffix == 'zip':
            pass
        if suffix == 'rar':
            pass
```
这里有2个注意点：

1. 有些文件没有后缀名，因此可能不存在 `.`，这时用 `filename.split('.')[-1]`会报错，因此需要先判断
2. 以`.gz` 结尾的文件，解压后需要再次判断是否是以 `.tar` 结尾，然后就可以分别将四种压缩文件的解压代码写成函数，符合时候单独调用即可
<a name="uTMj0"></a>
#### 处理.gz 文件
```python
import gzip
def ungz(filename):
    filename = filename[:-3] # gz文件的单文件解压就是去掉 filename 后面的 .gz
    gz_file = gzip.GzipFile(filename)
    with open(filename, "w+") as file:
        file.write(gz_file.read())
        return filename  # 这个gzip的函数需要返回值以进一步配合untar函数
```
前面反复提及的gz文件有可能跟`tar`文件配合，因此解压gz文件后需要再判断是否有必要解开`tar`文件<br />这里就可以先把tar文件的函数写出来👇
<a name="dGgOY"></a>
#### 处理.tar文件
```python
import tarfile
def untar(filename):
    tar = tarfile.open(filename)
    names = tar.getnames()
    # tar本身是将文件打包，解除打包会产生很多文件，因此需要建立文件夹存放
    if not os.path.isdir(filename + "_dir"):
        os.mkdir(filename + "_dir")
    for name in names:
        tar.extract(name, filename + "_dir/")
    tar.close()
```
<a name="yV21C"></a>
#### 处理.zip文件
```python
import zipfile
def unzip(filename):
    zip_file = zipfile.ZipFile(filename)
    # 类似tar解除打包，建立文件夹存放解压的多个文件
    if not os.path.isdir(filename + "_dir"):
        os.mkdir(filename + "_dir")
    for names in zip_file.namelist():
        zip_file.extract(names, filename + "_dir/")
    zip_file.close()
```
<a name="S7OaP"></a>
#### 处理.rar文件
```python
import rarfile
def unrar(filename):
    rar = rarfile.RarFile(filename)
    if not os.path.isdir(filename + "_dir"):
        os.mkdir(filename + "_dir")
    os.chdir(filename + "_dir")
    rar.extractall()
    rar.close()
```
可以看到，四种解压代码略有不同，解压后配合 `os.remove()` 就可以删除压缩包，现在看看逻辑框架补上解压函数后的内容：
```python
for filename in filename_lst:
    if '.' in filename:
        suffix = filename.split('.')[-1]
        if suffix == 'gz':
            new_filename = ungz(filename)
            os.remove(filename)
            if new_filename.split('.')[-1] == 'tar':
                untar(new_filename)
                os.remove(new_filename)  
        if suffix == 'tar':
            untar(filename)
            os.remove(filename)
        if suffix == 'zip':
            unzip(filename)
            os.remove(filename)
        if suffix == 'rar':
            unrar(filename)
            os.remove(filename)
```
<a name="M5E2A"></a>
#### 实现实时检测
比较简单的方法就是建立 `while True` 死循环配合 `time.sleep()`睡眠，框架如下：
```python
import time
while True:
    func()
    time.sleep(5) # 睡眠秒数可以设置大一点避免过度占用资源
```
最后将第二步的实现代码改为函数并放在循环框架中就可以完成本次需求，完整代码如下
```python
import glob
import os
import gzip
import tarfile
import zipfile
import rarfile
import time
path = r'C:\xxxx\download'
file_lst = glob.glob(path + '/*')
filename_lst = [os.path.basename(i) for i in file_lst]
def ungz(filename):
    filename = filename[:-3]
    gz_file = gzip.GzipFile(filename)
    with open(filename, "w+") as file:
        file.write(gz_file.read())
    return filename
def untar(filename):
    tar = tarfile.open(filename)
    names = tar.getnames()
    if not os.path.isdir(filename + "_dir"):
        os.mkdir(filename + "_dir")
    for name in names:
        tar.extract(name, filename + "_dir/")
    tar.close()
def unzip(filename):
    zip_file = zipfile.ZipFile(filename)
    if not os.path.isdir(filename + "_dir"):
        os.mkdir(filename + "_dir")
    for names in zip_file.namelist():
        zip_file.extract(names, filename + "_dir/")
    zip_file.close()
def unrar(filename):
    rar = rarfile.RarFile(filename)
    if not os.path.isdir(filename + "_dir"):
        os.mkdir(filename + "_dir")
    os.chdir(filename + "_dir")
    rar.extractall()
    rar.close()
def unzip_files():
    for filename in filename_lst:
        if '.' in filename:
            suffix = filename.split('.')[-1]
            if suffix == 'gz':
                new_filename = ungz(filename)
                os.remove(filename)
                if new_filename.split('.')[-1] == 'tar':
                    untar(new_filename)
                    os.remove(new_filename)
            if suffix == 'tar':
                untar(filename)
                os.remove(filename)
            if suffix == 'zip':
                unzip(filename)
                os.remove(filename)
            if suffix == 'rar':
                unrar(filename)
                os.remove(filename)
while True:
    unzip_files()
    time.sleep(5)
```
