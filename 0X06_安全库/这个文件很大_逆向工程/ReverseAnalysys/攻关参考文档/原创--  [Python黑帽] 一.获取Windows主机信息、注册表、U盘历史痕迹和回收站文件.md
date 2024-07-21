# 原创
：  [Python黑帽] 一.获取Windows主机信息、注册表、U盘历史痕迹和回收站文件

# [Python黑帽] 一.获取Windows主机信息、注册表、U盘历史痕迹和回收站文件

最近开始学习网络安全和系统安全，接触到了很多新术语、新方法和新工具，作为一名初学者，感觉安全领域涉及的知识好广、好杂，但同时也非常有意思。这系列文章是作者学习安全过程中的总结和探索，我们一起去躺过那些坑、跨过那些洞、守住那些站，真心希望文章对您有所帮助，感谢您的阅读和关注。

Python黑帽第一篇文章将分享获取Windows主机信息，利用注册表获取主机名及USB历史痕迹、回收站文件等，这些知识广泛应用于电子取证、Web渗透和攻击溯源领域，其中USB获取是亮点。希望这篇基础文章对您有所帮助，更希望大家提高安全意识，学会相关防范，也欢迎大家讨论。娜璋AI安全之家于2020年8月18日开通，将专注于Python和安全技术，主要分享Web渗透、系统安全、CVE复现、威胁情报分析、人工智能、大数据分析、恶意代码检测等文章。真心想把自己近十年的所学所做所感分享出来，与大家一起进步。

> 
声明：本人坚决反对利用教学方法进行恶意攻击的行为，一切错误的行为必将受到严惩，绿色网络需要我们共同维护，更推荐大家了解技术背后的原理，更好地进行安全防护。虽然作者是一名安全小白，但会保证每一篇文章都会很用心地撰写，希望这些基础性文章对你有所帮助，在安全路上一起前行。


#### 文章目录

---


## 一.获取Windows主机信息

WMI(Windows Management Instrumentation) 是一项核心的Windows管理技术，WMI模块可用于获取Windows内部信息。WMI作为一种规范和基础结构，通过它可以访问、配置、管理和监视几乎所有的Windows资源，比如用户可以在远程计算机器上启动一个进程；设定一个在特定日期和时间运行的进程；远程启动计算机；获得本地或远程计算机的已安装程序列表；查询本地或远程计算机的Windows事件日志等等。

本文使用Python获取Windows系统上相关的信息可以使用WMI接口，安装调用PIP工具即可。

下面的代码是获取Windows主机相关信息。

```
import wmi
import os
import socket

w = wmi.WMI()

#获取电脑使用者信息
for CS in w.Win32_ComputerSystem():
    #print(CS)
    print("电脑名称: %s" %CS.Caption)
    print("使用者: %s" %CS.UserName)
    print("制造商: %s" %CS.Manufacturer)
    print("系统信息: %s" %CS.SystemFamily)
    print("工作组: %s" %CS.Workgroup)
    print("机器型号: %s" %CS.model)
    print("")

#获取操作系统信息
for OS in w.Win32_OperatingSystem():
    #print(OS)
    print("操作系统: %s" %OS.Caption)
    print("语言版本: %s" %OS.MUILanguages)
    print("系统位数: %s" %OS.OSArchitecture)
    print("注册人: %s" %OS.RegisteredUser)
    print("系统驱动: %s" %OS.SystemDevice)
    print("系统目录: %s" %OS.SystemDirectory)
    print("")

#获取电脑IP和MAC信息
for address in w.Win32_NetworkAdapterConfiguration(ServiceName = "e1dexpress"):
    #print(address)
    print("IP地址: %s" % address.IPAddress)
    print("MAC地址: %s" % address.MACAddress)
    print("网络描述: %s" % address.Description)
    print("")

#获取电脑CPU信息
for processor in w.Win32_Processor():
    #print(processor)
    print("CPU型号: %s" % processor.Name.strip())
    print("CPU核数: %s" % processor.NumberOfCores)
    print("")

#获取BIOS信息
for BIOS in w.Win32_BIOS():
    #print(BIOS)
    print("使用日期: %s" %BIOS.Description)
    print("主板型号: %s" %BIOS.SerialNumber)
    print("当前语言: %s" %BIOS.CurrentLanguage)
    print("")

#获取内存信息
for memModule in w.Win32_PhysicalMemory():
    totalMemSize = int(memModule.Capacity)
    print("内存厂商: %s" %memModule.Manufacturer)
    print("内存型号: %s" %memModule.PartNumber)
    print("内存大小: %.2fGB" %(totalMemSize/1024**3))
    print("")

#获取磁盘信息
for disk in w.Win32_DiskDrive():
    diskSize = int(disk.size)
    print("磁盘名称: %s" %disk.Caption)
    print("硬盘型号: %s" %disk.Model)
    print("磁盘大小: %.2fGB" %(diskSize/1024**3))

#获取显卡信息
for xk in w.Win32_VideoController():
    print("显卡名称: %s" %xk.name)
    print("")
       
#获取计算机名称和IP
hostname = socket.gethostname()
ip = socket.gethostbyname(hostname)
print("计算机名称: %s" %hostname)
print("IP地址: %s" %ip)

```

输出结果如下图所示：

---


## 二.获取Windows注册表信息

### 1.注册表基本结构

注册表（Registry）是Windows系统中一个重要的数据库，它用于存储有关应用程序、用户和系统信息。注册表的结构就像一颗树，树的顶级节点(hive)不能添加、修改和删除，如下图所示是Windows注册表的顶级节点。

在C#中对注册表进行操作，需要引用命名空间using Microsoft.Win32。

注册表中常用的数据类型有：

---


### 2.注册表基本操作

Python注册表操作主要调用winreg扩展包。官方文档如下：

基本操作函数如下：

<mark>**(1) 创建操作**</mark>

例如在HKEY_CURRENT_USER下创建键Eastmount，其中我们最常用的是在\Software这个键下创建程序产品键，保存一些程序的配置在注册表中。如果Software中没有Eastmount键，则会先创建这个键及其子键，如果存在就不会重写。

```
import winreg as wg

#创建键
subDir = r"Software\Microsoft\Windows\CurrentVersion\Explorer"
key_test = wg.OpenKey(wg.HKEY_CURRENT_USER, subDir)
wg.CreateKey(key_test, 'Eastmount')
wg.CloseKey(key_test)

```

运行结果如下:

<mark>**(2) 检索键值操作**</mark>

在Eastmount下面新建一个值yxz，内容为“hello na”，然后编写代码读取相关的内容。

```
#获取键值数据项值
subDir = r"Software\Microsoft\Windows\CurrentVersion\Explorer\Eastmount"
key_test = wg.OpenKey(wg.HKEY_CURRENT_USER, subDir)
value,type_ = wg.QueryValueEx(key_test,'yxz')
print(value)
print(type_)

```

输出结果如下图所示：

<mark>**(3) 创建键值操作**</mark>

创建键值代码如下，但会提示PermissionError: [WinError 5] 拒绝访问错误。

```
#创建键值数据项
subDir = r"Software\Microsoft\Windows\CurrentVersion\Explorer\Eastmount"
key_test = wg.OpenKey(wg.HKEY_CURRENT_USER, subDir)
print(key_test)
#PermissionError: [WinError 5] 拒绝访问
wg.SetValueEx(key_test,'data','',wg.REG_SZ,'0') 
wg.CloseKey(key_test)

```

<mark>**(4) 删除键值操作**</mark>

```
#删除键值数据项
subDir = r"Software\Microsoft\Windows\CurrentVersion\Explorer\Eastmount"
key_test = wg.OpenKey(wg.HKEY_CURRENT_USER,subDir,0,wg.KEY_WRITE)
wg.DeleteValue(key_test,'yxz')
wg.CloseKey(key_test)

```

成功删除键值，如下图所示。

<mark>**(5) 其他操作**</mark>

---


### 3.获取用户账户信息

获取用户名称的代码如下：

```
# encoding:utf-8
from winreg import *
import sys

#连接注册表根键 以HKEY_LOCAL_MACHINE为例
regRoot = ConnectRegistry(None, HKEY_LOCAL_MACHINE)
subDir = r'SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList'

#获取指定目录下所有键的控制
keyHandle = OpenKey(regRoot, subDir)

#获取该目录下所有键的个数(0-下属键个数 1-当前键值个数)
count = QueryInfoKey(keyHandle)[0]
for i in range(count):
    #穷举键获取键名
    subKeyName = EnumKey(keyHandle, i)
    subDir_2 = r'%s\%s' % (subDir, subKeyName)
    
    #根据获取的键名拼接之前的路径作为参数 获取当前键下所属键的控制
    keyHandle_2 = OpenKey(regRoot, subDir_2)
    num = QueryInfoKey(keyHandle_2)[1]
    for j in range(num):
        name, value, type_ = EnumValue(keyHandle_2, j)
        if('ProfileImagePath' in name and 'Users' in value):
            print(value)
    #读写操作结束后关闭键
    CloseKey(keyHandle_2)
    
CloseKey(keyHandle)
CloseKey(regRoot)

```

执行结果如下，我们可以通过读取含有Users字段的数据，从而间接获取用户账户信息。

```
C:\Users\xiuzhang

```

---


## 三.获取回收站内容

为什么我们要去获取回收站文件呢？因为很多情况下调查取证需要获取远程目标的历史痕迹，回收站是重要的一个目标。在Windows操作系统中，回收站是一个专门用来存放被删除文件的特色文件夹。

在使用FAT文件系统的Windows98系统中，回收站目录通常是C:\Recycled；在Windows NT2000、Windows XP在内支持的NTFS操作系统中，C:\Recycler；在Windows Vista和Windows7中，回收站目录是C:\$Recycle.Bin。如下图所示，回收站中包含两个文件，分别位于桌面和D盘目录。

**第一步，检测回收站目录是否存在。**

```
import os

def returnDir():
    #定义回收站目录
    dirs = ['C:\\Recycler\\', 'C:\\Recycled', 'C:\\$Recycle.Bin\\']
    for recycleDir in dirs:
        if os.path.isdir(recycleDir):
            return recycleDir
    return None

#调用函数
res = returnDir()
print(res) #C:\$Recycle.Bin\

```

Windows10操作系统输出结果如下图所示：

**第二步，找到回收站之后，检测其中的内容，如下图所示，字符串SID与用户账户名是对应的，比如1001结尾的SID。**

**第三步，编写代码获取回收站文件夹所在目录。**

```
import os

#判断回收站目录是否存在
def returnDir():
    dirs = ['C:\\Recycler\\', 'C:\\Recycled', 'C:\\$Recycle.Bin\\']
    for recycleDir in dirs:
        if os.path.isdir(recycleDir):
            return recycleDir
    return None

#获取回收站内容
def findRecycle(recycleDir):
    dirList = os.listdir(recycleDir)
    for sid in dirList:
        print(sid)
        files = os.listdir(recycleDir+sid)
        print(files,"\n")

#主函数
def main():
    res = returnDir()
    print(res) #C:\$Recycle.Bin\
    findRecycle(res)
    
if __name__ == '__main__':
    main()

```

输出结果如下图所示：

**第四步，用python将用户的SID关联起来，使用Windows注册表将SID转化为一个准确的用户名。**

编写一个函数来将每一个SID转化为用户名，这个函数将打开注册便检查ProfileImagePath键值，找到其值并从中找到用户名。

如下图所示，用户名为“xiuzhang”。

**第五步，获取回收站所有内容。**<br/> <mark>完整代码：</mark>

```
import os
import winreg

#判断回收站目录是否存在
def returnDir():
    dirs = ['C:\\Recycler\\', 'C:\\Recycled', 'C:\\$Recycle.Bin\\']
    for recycleDir in dirs:
        if os.path.isdir(recycleDir):
            return recycleDir
    return None

#通过sid获取用户名信息
def sid2user(sid):
    try:
        key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,
                                "SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList\\" + sid)
        #获取该目录下所有键的个数: 0-下属键个数; 1-当前键值个数
        count = winreg.QueryInfoKey(key)[1] 
        for j in range(count):
            #FileNotFoundError: [WinError 2] 系统找不到指定的文件
            #(value, type) = winreg.QueryValue(key, 'ProfileImagePath')
            name, value, type = winreg.EnumValue(key, j)
            if('ProfileImagePath' in name):
                user = value.split('\\')[-1]
                #print(user)
        return user
    except:
        return sid

#获取回收站内容
def findRecycle(recycleDir):
    dirList = os.listdir(recycleDir)
    for sid in dirList:
        print(sid)
        files = os.listdir(recycleDir+sid)
        print(files)
        user = sid2user(sid)
        print('[*]Listing Files For User:' + str(user))
        for file in files:
            print('[+]Found File:' + str(file))
        print("")

#主函数
def main():
    res = returnDir()
    print(res)             #C:\$Recycle.Bin
    findRecycle(res)
    
if __name__ == '__main__':
    main()

```

输出结果如下图所示：

对应的回收站内容如下，但非常可惜获取的值无法对应，why？后续作者会继续深入挖掘。

<img alt="在这里插入图片描述" height="180" src="https://img-blog.csdnimg.cn/20200905141822195.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="550"/><br/> <img alt="在这里插入图片描述" height="180" src="https://img-blog.csdnimg.cn/20200905142559913.png#pic_center" width="550"/>

---


**如果我们想把文件删除到回收站，又怎么解决呢？Python删除文件一般使用os.remove，但这样是直接删除文件，不删到回收站的，那么想删除文件到回收站怎么办？**

(1) 安装pypiwin32扩展包（含win32api）。

(2) 调用SHFileOperation函数实现删除文件至回收站。

> 
在Windows的shellapi文件中定义了一个名为SHFileOperation的外壳函数，用它可以实现各种文件操作，如文件的拷贝、删除、移动等，该函数使用起来非常简单，它只有一个指向SHFILEOPSTRUCT结构的参数。


```
from win32com.shell import shell,shellcon
import os

debug = False
def deltoreCyclebin(filename):
    #直接删除文件，不经过回收站
    #os.remove(filename) 
    if not debug:
        #删除文件到回收站
        res = shell.SHFileOperation((0,
                                     shellcon.FO_DELETE,
                                     filename,
                                     None,
                                     shellcon.FOF_SILENT | shellcon.FOF_ALLOWUNDO | shellcon.FOF_NOCONFIRMATION,
                                     None,
                                     None))
        print(res)
        if not res[1]:
            os.system('del '+ filename)

if __name__ == '__main__':
    filename = "C:\\Users\\xiuzhang\\Desktop\\require.rb"
    deltoreCyclebin(filename)

```

最终效果如下图所示，可以看到require.rb文件被成功删除。

可能会遇到“PermissionError: [WinError 5] 拒绝访问”问题，我们需要设置Python.exe用户名完全控制，并且用管理员方式打开即可解决。

---


## 四.获取U盘痕迹

在Windows系统中，当一个USB移动存储设备插入时，就会在注册表中留下痕迹。当移动设备插入计算机时，即插即用管理器PnP(Plug and Play)接受该事件，并且在USB设备的固件(Firewre information)中查询有关该设备的描述信息(厂商、型号、序列号等)。当设备被识别后，在注册表中创建一个新的键值：

在这个键值下，会看到类似下面的结构子键，该子键代表设备类标示符，用来标识设备的一个特定类。

其中，子键中"###"代表区域由PnP管理器依据在USB设备描述符中获取的数据填写。如下图所示：

注意需要判断Service值为disk，即为磁盘的子项，光盘为cdrom。

如果使用UVCView工具可以看见USB设备描述内容，其中的信息都是相互对应的。设备类ID一旦建立，就需要建立一个特定唯一的UID，它可以把具有同一设备类标识的多个存储设备区分。

**完整实现代码如下：**

```
# encoding:utf-8
from winreg import *
import sys

usb_name = []
uid_flag = []
usb_path = []

#连接注册表根键 以HKEY_LOCAL_MACHINE为例
regRoot = ConnectRegistry(None, HKEY_LOCAL_MACHINE)

#检索子项
subDir = r"SYSTEM\CurrentControlSet\Enum\USBSTOR"

#获取指定目录下所有键的控制
keyHandle = OpenKey(regRoot, subDir)

#获取该目录下所有键的个数(0-下属键个数 1-当前键值个数)
count = QueryInfoKey(keyHandle)[0]
print(count)

#穷举USBSTOR键获取键名
for i in range(count):
    subKeyName = EnumKey(keyHandle, i)
    subDir_2 = r'%s\%s' % (subDir, subKeyName)
    #print(subDir_2)

    #根据获取的键名拼接之前的路径作为参数 获取当前键下所属键的控制
    keyHandle_2 = OpenKey(regRoot, subDir_2)
    num = QueryInfoKey(keyHandle_2)[0]
    #遍历子键内容
    for j in range(num):
        subKeyName_2 = EnumKey(keyHandle_2, j)
        #print(subKeyName_2)
        result_path = r'%s\%s' % (subDir_2, subKeyName_2)

        #获取具体键值内容并判断Service为disk
        keyHandle_3 = OpenKey(regRoot, result_path)
        numKey = QueryInfoKey(keyHandle_3)[1]
        for k in range(numKey):
            #获取USB名称
            name, value, type_ = EnumValue(keyHandle_3, k)
            if(('Service' in name) and ('disk'in value)):
                value,type_ = QueryValueEx(keyHandle_3,'FriendlyName')
                usb = value
                uid = subKeyName_2
                path = "USBSTOR" + "\\" + subKeyName + "\\" + subKeyName_2
                print(usb)
                print(uid)
                print(path)                
                print("")
    
#关闭键值
CloseKey(keyHandle)
CloseKey(regRoot)

```

输出的USB记录键名如下图所示：

其中对应的注册表信息如下图所示，FriendlyName即是输出的“USB名称 Kingston DataTraveler 2.0 USB Device”，UID序号为“C860008862F1EE501A0F0105&amp;0”，搜索的Service(服务)为disk(磁盘)的选项。

**简单总结：**

个人感觉这方面的资料真心很少，文章博客也少，所以看起来操作似乎很简单，但真正实现起来还是令人深思的。然后就是其实存储USB记录的还有很多键值，如

接下来我想要完成的就是如何把这些键值联系起来，似乎要通过Dictionary&lt;string, UInfo&gt;，同时怎样获取时间，怎样正确删除这些信息都值得深究。

---


## 五.总结

<font color="red">**这篇文章真的花费了一些精力，希望您喜欢。同时感觉自己要学习的知识好多，也有好多大神卧虎藏龙，开源分享。作为初学者，我们可能有差距，不论你之前是什么方向，是什么工作，是什么学历，是大学大专中专，亦或是高中初中，只要你喜欢安全，喜欢渗透，就朝着这个目标去努力吧！有差距不可怕，我们需要的是去缩小差距，去战斗，况且这个学习的历程真的很美，安全真的有意思。但切勿去做坏事，我们需要的是白帽子，是维护我们的网络，安全路上共勉。**</font>

最后，真诚地感谢您关注“娜璋之家”公众号，也希望我的文章能陪伴你成长，希望在技术路上不断前行。文章如果对你有帮助、有感悟，就是对我最好的回报，且看且珍惜！再次感谢您的关注，也请帮忙宣传下“娜璋之家”，哈哈~初来乍到，还请多多指教。

**独在异乡为异客，<br/> 每逢佳节倍思亲。**

(By:Eastmount 2020-09-04 夜于武汉 [https://blog.csdn.net/Eastmount](https://blog.csdn.net/Eastmount))

---


**参考文献：**<br/> [1]《Python绝技运用Python成为顶级黑客》TJ.O Connor<br/> [2] [Python wmi模块获取windows内部信息 - minger_lcm](https://www.cnblogs.com/mingerlcm/p/10498530.html)<br/> [3] [SHFileOperation的用法 - xiaodai0](https://www.cnblogs.com/xiaodai0/p/10174877.html)<br/> [4] [https://blog.csdn.net/Eastmount/article/details/108020041](https://blog.csdn.net/Eastmount/article/details/108020041)<br/> [5] [C# 系统应用之注册表使用详解 - Eastmount](https://blog.csdn.net/Eastmount/article/details/25428405)<br/> [6] [https://docs.python.org/2/library/_winreg.html](https://docs.python.org/2/library/_winreg.html)<br/> [7] 《计算机信息获取系统的研究与实现》论文
