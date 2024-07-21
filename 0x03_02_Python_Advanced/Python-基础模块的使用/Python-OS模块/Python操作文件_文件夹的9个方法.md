Python
<a name="KRySi"></a>
## 输出当前的路径
可以通过Python当中的`OS`库来获取当前文件所在的位置
```python
import os
os.getcwd()
```
<a name="Te36q"></a>
## 路径的拼接
通过`os.path.join()`方法来实现
```python
os.path.join('output', 'Book1.xlsx')
```
output
```python
output\Book1.xlsx
```
<a name="cWq54"></a>
## 确认某文件夹或者是文件是否存在
先来看如何来确认文件夹是否存在，同样也是通过`OS`模块
```python
os.path.exists('directory_name')
```
同样的道理，要是查看一下某个文件是否存在的时候，可以这么做
```python
os.path.exists('path/file_name')
```
<a name="KkDTW"></a>
## 建立文件夹目录
然后来看一下如何新建一个文件夹
```python
os.mkdir("文件夹名称")
```
当然要是文件夹目录已经提前存在的情况下，上述的代码自然会报错，所以通常会先检查一下是否已经存在
```python
if not os.path.exists('文件夹名称'):
    os.mkdir('文件夹名称')
```
当然有时候需要在已建的文件夹下创建子文件夹，例如下图<br />![](./img/1639795358260-18c5f72c-109c-4f14-87e8-ac07ec7518a3.webp)<br />这个时候要是引用`os.mkdir()`方法或许会稍显繁琐，这时可以通过`os.mkdirs()`方法来实现
```python
os.makedirs(os.path.join('test_dir', 'level_1', 'level_2', 'level_3'))
```
<a name="WdXcl"></a>
## 罗列出当前目录下所包含的所有文件
代码如下
```python
os.listdir('文件夹名称')
```
但是有时候可能想例如搜寻一下当前目录下的所有文件中以“py”结尾的文件，可以通过通配符来实现，代码如下
```python
list(glob(os.path.join('.', '*.py')))
```
上面提到的`glob`模块能够快速查找想要的目录和文件，它支持`*`、`**`、`?`、`[]`这四个通配符
<a name="lnNhW"></a>
## 移动文件
要是想在不同的目录文件夹之下移动文件，该怎么操作呢？这里介绍Python当中的`shutil`模块，假设要将当前目录下的几个csv文件移动到“test_dir”目录文件夹之下，代码如下
```python
import shutil

for file in list(glob(os.path.join('.', '*.csv'))):
    shutil.move(file, 'test_dir')
```
<a name="aturL"></a>
## 复制文件
当想要复制文件的时候，也可以使用`shutil`模块，例如要将几个“test_dir”目录文件夹下的csv文件复制到“output”目录文件夹之下，代码如下
```python
shutil.copy(os.path.join('test_dir', 'data.csv'), 'output')
```
除此之外，还能够对粘贴过去的文件重命名，代码如下
```python
shutil.copy(os.path.join('test_dir', 'data.csv'),
            os.path.join('output', 'data_2.csv'))
```
<a name="ckk9A"></a>
## 删除文件
`os.remove()`方法可以完成删除文件的功能，
```python
os.remove(os.path.join('output', 'data_2.csv'))
```
当想要删除一整个目录文件夹的时候，可以使用`os.rmdir()`方法，当然只仅限于是空的目录文件夹
```python
os.rmdir(os.path.join('test_dir', 'level_1', 'level_2', 'level_3'))
```
而对于目录文件夹下存在文件的，并不是空的目录文件夹，还是需要用到`shutil`模块，代码如下
```python
shutil.rmtree("test_delete")
```
或者是
```python
shutil.rmtree(os.path.join("test_delete", "test_1_delete"))
```
<a name="W0QWU"></a>
## 创建和解压压缩包
谈及到操作压缩包相关的内容，就不得不说`zipobj`这个模块了

- 创建一个压缩包

这里要用到的是`zipfile`模块当中的`write()`方法
```python
file_lists = list(glob(os.path.join('.', '*.xlsx')))

with zipfile.ZipFile(r"我创建的压缩包.zip", "w") as zipobj:
    for file in file_lists:
        zipobj.write(file)
```

- 读取压缩包当中的文件信息

通过`zipfile`模块当中的`namelist()`方法来实现
```python
with zipfile.ZipFile("我创建的压缩包.zip", "r") as zipobj:
    print(zipobj.namelist())
```
output
```python
['Book1.xlsx', 'supermarkt_sales.xlsx']
```
可以看到输出的就是上次打包的几个excel文件

- 将压缩包当中的单个文件，解压出来

通过`zipfile`模块当中的`extract()`方法来实现
```python
dst = "output"

with zipfile.ZipFile("我创建的压缩包.zip", "r") as zipobj:
    zipobj.extract("Book1.xlsx",dst)
```
上述代码的用意就是将压缩包当中的“Book1.xlsx”文件解压至“output”目录文件夹下

- 将压缩包中的所有文件，都解压出来

通过`zipfile`模块当中的`extractall()`方法来实现，代码如下
```python
dst = "output"

with zipfile.ZipFile("我创建的压缩包.zip", "r") as zipobj:
    zipobj.extractall(dst)
```
