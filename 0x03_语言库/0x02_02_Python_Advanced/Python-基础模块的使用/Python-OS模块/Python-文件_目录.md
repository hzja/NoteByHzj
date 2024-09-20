Python 文件 目录 
<a name="lxiQU"></a>
### OS模块
os模块提供了多数操作系统的功能接口函数。当os模块被导入后，它会自适应于不同的操作系统平台，根据不同的平台进行相应的操作，在python编程时，经常和文件、目录打交道，这时就离不了os模块。
<a name="y7Fu9"></a>
#### 模块导入
```python
import  os
```
<a name="QgCty"></a>
### 文件操作
<a name="f8MZ5"></a>
#### 文件重命名—`os.rename(原文件名，新文件名)`
```python
print(os.getcwd()) #先打印一下当前路径
#C:\Users\WJF   #每个人的不一样
print("目录为: %s"%os.listdir(os.getcwd()))   #该路径下的文件名
os.rename('oldname','newname')     #这里是创建了一个名为'oldname'的空文件夹，大家创文件也可以的。
print('修改目录名成功')
#修改目录名成功
print("目录为: %s"%os.listdir(os.getcwd()))  #修改成功后再打印一下当前的目录与之前的目录进行比对。
```
对比上面的目录列表可以发现已成功修改目录名
<a name="UaQqV"></a>
#### 删除文件—`os.remove(文件名)`
```python
os.remove('remove.txt')  #这里执行了两次，然后报错了，说明第一次的时候已经删掉了，这样可以省去在打印的目录中检查是否删除成功
---------------------------------------------------------------------------
FileNotFoundError                         Traceback (most recent call last)
<ipython-input-17-3113f0689db3> in <module>
----> 1 os.remove('remove.txt')
FileNotFoundError: [WinError 2] 系统找不到指定的文件。: 'remove.txt'
```
<a name="9neLL"></a>
### 文件夹操作
<a name="tw57V"></a>
#### 创建文件夹—`os.mkdir(文件夹名)`
```python
os.mkdir('newtest')
print("目录为: %s"%os.listdir(os.getcwd()))  #从下面的目录中可以看到新建的文件夹名称
```
<a name="B80zl"></a>
#### 删除文件夹—`os.rmdir(文件夹名)`
```python
os.rmdir('newtest')
print("目录为: %s"%os.listdir(os.getcwd()))  #从下面的目录中可以发现刚才的文件夹不见了
```
<a name="V45fO"></a>
#### 获取当前目录—`os.getcwd()`
```python
os.getcwd()
#'C:\\Users\\WJF'
```
<a name="E4831"></a>
#### 改变默认目录—`os.chdir(目录)`
```python
os.chdir('D:\Download')
os.getcwd()   #可以发现默认目录已改变
#'D:\\Download'
```
<a name="9fNgg"></a>
#### 获取目录列表—`os.listdir()`
`os.listdir(目录)`
```python
os.listdir()
os.listdir('D:\MD笔记\python学习路线')   #还可以列出指定文件夹下的目录
'''
['水印_python01.png',
'水印_python02.png',
'水印_python03.png',
'水印_Python05.png',
'水印_Python06.png',
'水印_Python07.png',
'水印_Python08.png']
'''
```
<a name="h0F8X"></a>
#### 重命名文件夹—`os.rename(原文件夹名，新文件夹名)`
```python
os.rename('Hello', 'World')
```
这个和修改文件名是一样的。
