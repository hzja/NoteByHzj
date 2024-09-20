Python<br />在日常工作中，除了会涉及到使用Python处理文本文件，有时候还会涉及对压缩文件的处理。<br />通常会涉及到的压缩文件格式有：

- rar：Windows 环境下用的比较多的压缩，比较著名的GUI工具是winrar
- tar：Linux系统下的打包工具，只打包，不压缩
- gz：即gzip，通常只能压缩一个文件。与tar结合起来就可以实现先打包，再压缩。
- tgz：即gz。先用tar打包，然后再用gz压缩得到的文件
- zip：不同于gzip，虽然使用相似的算法，可以打包压缩多个文件，不过分别压缩文件，压缩率低于tar
- 7z：7zip压缩软件支持的格式，压缩效率较高。

当然除了使用Python外，还可以选择使用压缩解压缩软件或命令手动进行处理。
<a name="Vvf6V"></a>
## zip文件
zipfile是Python里用来做zip格式编码的压缩和解压缩的module，zipfile里有两个非常重要的class：`ZipFile`和`ZipInfo`。`ZipFile`是主要的类，用来创建和读取zip文件，而`ZipInfo`是存储的zip文件的每个文件的信息。<br />示例代码：
```python
import os
import zipfile


# 压缩
def make_zip(source_dir, output_filename):
    zipf = zipfile.ZipFile(output_filename, 'w')
    pre_len = len(os.path.dirname(source_dir))
for parent, dirnames, filenames in os.walk(source_dir):
for filename in filenames:
            print(filename)
            pathfile = os.path.join(parent, filename)
            arcname = pathfile[pre_len:].strip(os.path.sep)  # 相对路径
            zipf.write(pathfile, arcname)
        print()
    zipf.close()


# 解压缩
def un_zip(file_name):
"""unzip zip file"""
    zip_file = zipfile.ZipFile(file_name)
if os.path.isdir(file_name + "_files"):
pass
else:
        os.mkdir(file_name + "_files")
for names in zip_file.namelist():
        zip_file.extract(names, file_name + "_files/")
    zip_file.close()


if __name__ == '__main__':
    make_zip(r"E:python_samplelibstest_tar_fileslibs", "test.zip")
    un_zip("test.zip")
```
<a name="PN3EK"></a>
## tar.gz文件
tarfile 模块可以用来读写 tar 归档，包括使用 gzip, bz2 和 lzma 压缩的归档。在使用tarfile是必须了解模式：<br />mode 必须是 `'filemode[:compression]'` 形式的字符串，其默认值为 `'r'`。以下是模式组合的完整列表:

| 模式 | 动作 |
| --- | --- |
| ‘r’ or ‘r:*’ | 打开和读取使用透明压缩（推荐）。 |
| ‘r:’ | 打开和读取不使用压缩。 |
| ‘r:gz’ | 打开和读取使用gzip 压缩。 |
| ‘r:bz2’ | 打开和读取使用bzip2 压缩。 |
| ‘r:xz’ | 打开和读取使用lzma 压缩。 |
| ‘x’ 或 ‘x:’ | 创建tarfile不进行压缩。如果文件已经存在，则抛出 FileExistsError 异常。 |
| ‘x:gz’ | 使用gzip压缩创建tarfile。如果文件已经存在，则抛出 FileExistsError 异常。 |
| ‘x:bz2’ | 使用bzip2 压缩创建tarfile。如果文件已经存在，则抛出 FileExistsError 异常。 |
| ‘x:xz’ | 使用lzma 压缩创建tarfile。如果文件已经存在，则抛出 FileExistsError 异常。 |
| ‘a’ or ‘a:’ | 打开以便在没有压缩的情况下追加。如果文件不存在，则创建该文件。 |
| ‘w’ or ‘w:’ | 打开用于未压缩的写入。 |
| ‘w:gz’ | 打开用于 gzip 压缩的写入。 |
| ‘w:bz2’ | 打开用于 bzip2 压缩的写入。 |
| ‘w:xz’ | 打开用于 lzma 压缩的写入。 |

针对特殊的目的，还存在第二种 mode 格式：`'filemode|[compression]'`。`tarfile.open()` 将返回一个将其数据作为数据块流来处理的 TarFile 对象：

| 模式 | 动作 |
| --- | --- |
| ‘r&#124;*’ | 打开 tar 块的 流 以进行透明压缩读取。 |
| ‘r&#124;’ | 打开一个未压缩的 tar 块的 stream 用于读取。 |
| ‘r&#124;gz’ | 打开一个 gzip 压缩的 stream 用于读取。 |
| ‘r&#124;bz2’ | 打开一个 bzip2 压缩的 stream 用于读取。 |
| ‘r&#124;xz’ | 打开一个 lzma 压缩 stream 用于读取。 |
| ‘w&#124;’ | 打开一个未压缩的 stream 用于写入。 |
| ‘w&#124;gz’ | 打开一个 gzip 压缩的 stream 用于写入。 |
| ‘w&#124;bz2’ | 打开一个 bzip2 压缩的 stream 用于写入。 |
| ‘w&#124;xz’ | 打开一个 lzma 压缩的 stream 用于写入。 |

代码示例：
```python
import os
import tarfile
import gzip


# 一次性打包整个根目录。空子目录会被打包。
# 如果只打包不压缩，将"w:gz"参数改为"w:"或"w"即可。
def make_targz(output_filename, source_dir):
with tarfile.open(output_filename, "w:gz") as tar:
        tar.add(source_dir, arcname=os.path.basename(source_dir))


# 逐个添加文件打包，未打包空子目录。可过滤文件。
# 如果只打包不压缩，将"w:gz"参数改为"w:"或"w"即可。
def make_targz_one_by_one(output_filename, source_dir):
    tar = tarfile.open(output_filename, "w:gz")
for root, dir, files in os.walk(source_dir):
for file in files:
            pathfile = os.path.join(root, file)
            tar.add(pathfile)
    tar.close()


def un_gz(file_name):
"""ungz zip file"""
    f_name = file_name.replace(".gz", "")
# 获取文件的名称，去掉
    g_file = gzip.GzipFile(file_name)
# 创建gzip对象
    open(f_name, "wb+").write(g_file.read())
# gzip对象用read()打开后，写入open()建立的文件里。
    g_file.close()  # 关闭gzip对象


def un_tar(file_name):
# untar zip file
    tar = tarfile.open(file_name)
    names = tar.getnames()
if os.path.isdir(file_name + "_files"):
pass
else:
        os.mkdir(file_name + "_files")
# 由于解压后是许多文件，预先建立同名文件夹
for name in names:
        tar.extract(name, file_name + "_files/")
    tar.close()
	

if __name__ == '__main__':
    make_targz('test.tar.gz', "E:python_samplelibs")
    make_targz_one_by_one('test01.tgz', "E:python_samplelibs")
    un_gz("test.tar.gz")
    un_tar("test.tar")
```
<a name="KIjVt"></a>
## rar文件
可以使用rarfile来解压.rar的文件，但是不支持用rarfile来压缩rar文件。rarfile以下unrar组件，但是使用`pip install unrar`安装后发现会报如下错误：
> Couldn’t find path to unrar library…

这是因为 Python下的 unrar 还依赖了RAR官方的库。<br />**Windows的安装**

- 到RARLab官方下载库文件，[https://www.rarlab.com/rar/UnRARDLL.exe](https://www.rarlab.com/rar/UnRARDLL.exe)，然后安装。
- 安装是最好选择默认路径，一般在 C:Program Files (x86)UnrarDLL 目录下。
- 添加环境变量，在系统变量中新建变量名UNRAR_LIB_PATH，如果是64位系统，就输入 C:Program Files (x86)UnrarDLLx64UnRAR64.dll，如果是32位系统就是 C:Program Files (x86)UnrarDLLUnRAR.dll。
- 确定保存环境变量后，在进行 `pip install unrar` 安装，这时代码再运行就不会报错了。

**Linux的安装**

- 下载rar源文件：[https://www.rarlab.com/rar/rarlinux-6.0.0.tar.gz](https://www.rarlab.com/rar/rarlinux-6.0.0.tar.gz)
- 解压安装包，进入安装包目录，编译安装，生成so 文件
- 配置环境变量，完成后，在进行 pip install unrar 安装
```bash
# cd /usr/local/src/
# wget https://www.rarlab.com/rar/unrarsrc-6.0.3.tar.gz
# tar zxvf unrarsrc-6.0.3.tar.gz
# cd unrar
# make lib
# make install-lib  //生成libunrar.so 文件
# vim /etc/profile
export UNRAR_LIB_PATH=/usr/lib/libunrar.so
# source /etc/profile
```
代码示例：
```python
import rarfile

def unrar(rar_file, dir_name):
	# rarfile需要unrar支持, linux下pip install unrar, windows下在winrar文件夹找到unrar,加到path里
	rarobj = rarfile.RarFile(rar_file.decode('utf-8'))
    rarobj.extractall(dir_name.decode('utf-8'))
```
<a name="NwmSI"></a>
## 7z文件
要压缩和解压缩.7z文件需要用到py7zr组件。代码示例：
```python
import py7zr

# 压缩
with py7zr.SevenZipFile("Archive.7z", 'r') as archive:
    archive.extractall(path="/tmp")

# 解压缩
with py7zr.SevenZipFile("Archive.7z", 'w') as archive:
    archive.writeall("target/")
```
