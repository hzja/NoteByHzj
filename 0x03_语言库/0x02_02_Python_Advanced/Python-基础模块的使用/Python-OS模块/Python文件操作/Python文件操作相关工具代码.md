Python
<a name="AAmLb"></a>
## OS模块相关
<a name="QxdeN"></a>
### 一、遍历文件夹
批量操作的前提就是对文件夹进行遍历，使用`os`模块可以轻松的遍历文件夹，`os.walk` 遍历后产生三个参数：

1. 当前文件夹路径
2. 包含文件夹名称[列表形式]
3. 包含文件名称[列表形式]

代码如下，大家可以根据自己的路径进行修改
```python
import os
    
for dirpath, dirnames, filenames in os.walk(r'C:\\Program Files (x86)'):
    print(f'打开文件夹{dirpath}') # 当前文件夹路径 
    if dirnames:
        print(dirnames) # 包含文件夹名称[列表形式] 
    if filenames:
        print(filenames) # 包含文件名称[列表形式]
    print('-' * 10)
```
当手上的需求明确有获取给定路径各级文件夹下所有符合条件的文件，并进行相应的批处理操作时，即可使用 `os.walk`
<a name="4b57c2d1"></a>
### 二、目标路径是否是文件
有时需要判断一个目录下是否存在文件也可以使用`os`模块。<br />给定一个目标路径 `path` ，通过一行代码就能够判断这是文件还是文件夹路径
```python
import os
path = 'xxx'
print(os.path.isfile(path))
```
<a name="K8MX1"></a>
### 三、获取路径中的文件名
`os.path.basename` 可以直接从绝对路径中获取最后的文件名，当然如果用传统的字符串切割方式也可以，即 `path.split('\\')[-1]`
```python
import os
path = 'xxx'
print(os.path.basename)
```
<a name="5fd9df62"></a>
### 四、创建文件夹
创建文件夹的代码非常常用，因为往往生成的新文件都希望有个新的文件夹存储，代码如下：
```python
import os
dirpath = 'xxx'
os.mkdir(dirpath)
```
但是，如果希望创建的文件夹已经存在，再运行 `os.mkdir()` 则会报错而终止代码。为了避免这一情况的发生，可以在创建文件夹之前先判断文件夹是否存在。<br />用到的代码是 `os.path.exists`，只有当路径不存在（即  `os.path.exists` 返回的结果是 `False` 时），才会创建：
```python
import os
dirpath = 'xxx'
if not os.path.exists(dirpath):
    os.mkdir(dirpath)
```
<a name="HQQcr"></a>
### 五、获取桌面路径
获取桌面路径也是非常常用的操作，可以使用`os.path.join(os.path.expanduser("~"), 'Desktop')` 获取桌面的绝对路径。<br />这样做的好处是可以把数据放在桌面上，在不同的电脑上都能调用代码对数据进行处理。如果是在一条电脑上把桌面路径固定在字符串中，则换一台电脑就必须修改桌面路径。代码如下：
```python
import os
desktop_path = os.path.join(os.path.expanduser("~"), 'Desktop')
print(desktop_path)
```
当然把上面的代码包装成一个函数 `GetDesktopPath()` 需要时调用它会更加方便
```python
import os
def GetDesktopPath():
    return os.path.join(os.path.expanduser("~"), 'Desktop')
```
<a name="foTU9"></a>
### 六、重命名文件/文件夹
需要用到 `os.rename()` 方法，下面的代码示例中分别演示如何重命名文件和文件夹
```python
import os
os.rename('practice.txt', 'practice_rename.txt')  # 重命名文件
os.rename('文件夹1', '文件夹2') # 重命名文件夹
```
<a name="9336e840"></a>
### 七、批处理文件 - 1
除了前面的 `os.walk` 之外，有其他的 `os` 模块下方法可完成获取指定路径的全部或符合条件的文件（非遍历各级文件夹的需求），还可以使用下面两个代码 第一种用到的方法是`os.scandir()`，使用如下：
```python
import os
path = 'xxx'
for file in os.scandir(path): 
    print(file.name, file.path)
```
<a name="fc41ee3e"></a>
### 八、批处理文件 - 2
上面代码最后输出的是 **给定路径下各内容的名字、绝对路径**第二种方法使用 `os.listdir()`，它比 `os.scandir()` 简单一些，可直接调用输出名称而非路径：
```python
import os 
path = 'xxx'
for file in os.listdir(path):
    print(file)
```
<a name="GrjvD"></a>
## shutil模块相关
<a name="yafnG"></a>
### 九、移动文件/文件夹
`shutil`也是经常出现在办公自动化场景中的模块，常用的就是**移动文件/文件夹**。<br />需要用到`shutil.move` 方法，下面的代码示例中分别演示如何移动文件和文件夹：
```python
import shutil 
shutil.move(r'.\practice.txt', r'.\文件夹1/') 
shutil.move(r'.\practice.txt', r'.\文件夹1/new.txt')
```
注意到上面后两行代码的区别吗？前一行是将目标文件移动到目标文件夹里，而后一行，在将目标文件移动到目标文件夹里的同时，能够对其进行重命名<br />也就是说，如果需要移动某个或某些文件到新的文件夹，并且需重命名文件，并不需要用 `os.rename` 先命名文件再用 `shutil.move` 将其移动的指定文件夹，而是可以用 `shutil.move` 一步到位
<a name="JXWVj"></a>
## glob模块相关
<a name="jvZ5k"></a>
### 十、批处理文件 - 3
最后要介绍的是`glob`模块，也是办公自动化必须要掌握的一个模块，同样可以用于批处理文件。<br />`glob` 最重要的功能就是搜索获取同一级或者各子级下符合条件的文件（绝对路径），非常适合写批处理的代码。<br />有时候需要对大量文件进行相同操作，在写完针对一份文件的操作后，只需要加上几行代码，就可以完成批处理全部文件的工作。大致代码框架如下：
```python
import glob
    
for file in glob.glob('**/*', recursive=True): 
    print(file)
```
`glob.glob()` 是一个非常重要的方法，能够获取给定路径下文件的绝对路径，并且接受「通配符」搜索，大大拓宽了灵活程度，`*` 表示任意字符长度，`**/*` 的使用表示用通配符指代给定路径下的任何一层，`recursive` 参数允许遍历搜索。
