Linux ncdu 
<a name="s9sM7"></a>
## 1、Ncdu的官网
[ncdu的官网](https://dev.yorhel.nl/ncdu)<br />[https://dev.yorhel.nl/ncdu](https://dev.yorhel.nl/ncdu)
<a name="IQP69"></a>
## 2、Ncdu的安装
```bash
[root@iZuligp6e1dyzfZ bin]# yum install ncdu
```
<a name="skh2k"></a>
## 3、Ncdu简介
NCurses 磁盘利用率（ncdu）工具提供了与 du 类似的结果，但是在一个基于 curses 的交互式界面中，它重点关注那些占用磁盘空间比较多的目录。<br />在根目录下执行扫描容易崩溃，使用需注意。
<a name="p20KH"></a>
## 4、Ncdu的使用
ncdu 会花一些时间分析磁盘，然后根据最常用的目录或文件显示结果，如下所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596446756851-4e2a4585-7b0d-4fa5-ab9a-62dbdcdeb76d.png#height=583&id=KdEOw&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1846873&status=done&style=none&title=&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596521850151-2d490e86-eda5-4178-bd56-5ee0def0681b.png#height=583&id=xNnGv&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1815546&status=done&style=none&title=&width=1107.6666666666667)
```bash
ncdu 1.15.1 ~ Use the arrow keys to navigate, press ? for help                                                          --- /local -------------------------------------------------------------------------------------------------------------
    2.5 GiB [##########] /mysql                                                                                           743.8 MiB [##        ] /redis
  552.0 MiB [##        ] /jdk
  499.5 MiB [#         ] /redis-6
  181.1 MiB [          ] /ftp
  108.3 MiB [          ] /maven
   75.2 MiB [          ] /zookeeper
   40.0 KiB [          ] /tools-software
   16.0 KiB [          ] /bash





 Total disk usage:   4.6 GiB  Apparent size:   4.6 GiB  Items: 11024                                                    
```
使用方向键导航到每个条目。如果在一个目录条目上按下 **Enter** 键，ncdu 将显示该目录的内容：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596521900693-26bdf075-ad76-4f3f-aa0c-c8c386cdea50.png#height=583&id=bPnXC&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1783808&status=done&style=none&title=&width=1107.6666666666667)<br />可以使用它下级目录并查找哪些文件占用的磁盘空间最多，使用左箭头键可以返回上一级目录。在默认情况下，可以按下 d 键使用 ncdu 删除文件，在删除之前，它会确认删除请求。如果想禁用此行为以防止发生意外，请使用 `-r` 选项进行只读访问：`ncdu -r`。

