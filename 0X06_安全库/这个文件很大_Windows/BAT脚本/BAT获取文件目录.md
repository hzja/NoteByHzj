# 获取文件目录

Q：在Win7/Win10中以管理员身份运行在[cmd](https://so.csdn.net/so/search?q=cmd&spm=1001.2101.3001.7020)/bat脚本时，如何获取当前脚本文件所在目录？

  当我们在Win7/Win10中使用鼠标右键的“以管理员身份运行”以管理员身份运行cmd/bat脚本时，系统默认进入的目录是C:\Windows\System32目录，因此cmd脚本文件中的相应命令因路径不对提示找不到要执行的程序文件等提示。

~~~ bat
netpay_Service.exe -install
netpay_Monitor.exe -install
~~~

A：解决办法：使用cd /d %~dp0，先进入cmd/bat脚本所在的目录，再去执行其他命令。
~~~ bat
netpay_Service.exe -install
netpay_Monitor.exe -install
~~~

  这个脚本在xp、2000、2003等系统中都可以正常双击运行。在win7/Win10系统中双击运行时，会以普通用户身份运行，此时所获取的文件路径的确是当前路径，而不是C:\Windows\System32。但是运行到安装netpay_Service.exe -install 的系统服务时，普通用户显然权限是不够的。

  于是在InstllSvc_En.cmd右键选择“以管理员身份运行”，此时又会出问题，win7/win10可能出于安全问题考虑，此时获得的目录是C:\Windows\System32，于是提示netpay_Service.exe命令无效或程序文件不存在，执行出错。

  此时在脚本开始尝试加入命令cd %cd%，来获取当前路径，实验得知，这行语句在xp等系统中有效，但是在win7/win10中依然无效。得到的目录依然是C:\Windows\System32。
  百度一下才知道要使用cd /d %~dp0命令来获取脚本所在的目录。在脚本最开始添加cd /d %~dp0即可，如下：

~~~ bash
cd /d %~dp0
netpay_Service.exe -install
netpay_Monitor.exe -install
~~~


  在Windows XP~Windows 10系统上运行此脚本，确认都没有问题。下面对命令中涉及到的参数做一解释。



一、关于cd的/d参数

关于cd 的/d参数，在cmd中敲入cd /?
可以看到/d参数的解释如下：

**使用 /d 命令行开关，除了改变驱动器的当前目录之外，还可改变当前驱动器**。这句话不太好理解，我做个试验：
通常我们在xp系统中打开cmd窗口时，会显示

~~~ bash
C:\users\Administrator>
~~~

如果我们执行“cd e:\xxx”命令，发现目录依然还是在C:\users\Administrator

此时，我们键入e:，不但会切换到e盘，而且会会发现E盘上的当前目录为e:/xxx。

~~~ bash
C:\users\Administrator>cd e:\xxx
C:\users\Administrator>e:
e:\xxx>
~~~

下面语句我们就能看到/d参数的作用了。发现加了/d参数之后直接切换到e:\xxx目录了。

~~~ bash
C:\users\Administrator>cd /d e:\xxx
e:\xxx>
~~~

结论：不加/d参数只能在同一驱动器的目录之间切换，加上/d参数则能在不同驱动器之间的目录之间切换。



二、关于%~dp0的批处理命令的详细解释

%~dp0 “d”为Drive的缩写，即为驱动器，磁盘、“p”为Path缩写，即为路径，目录
cd是转到这个目录，不过我觉得cd /d %~dp0 还好些

选项语法:

~~~ bash
~0 - 删除任何引号(\")，扩充 %0
%~f0 - 将 %0 扩充到一个完全合格的路径名(“f”是file，即文件)
%~d0 - 仅将 %0 扩充到一个驱动器号
%~p0 - 仅将 %0 扩充到一个路径
%~n0 - 仅将 %0 扩充到一个文件名(“n”是name 文件名)
%~x0 - 仅将 %0 扩充到一个文件扩展名
%~s0 - 扩充的路径只含有短名(“s”为Short，短的)
%~a0 - 将 %0 扩充到文件的文件属性(“a”为attribute，即属性)
%~t0 - 将 %0 扩充到文件的日期/时间(“t”time)
%~z0 - 将 %0 扩充到文件的大小(Size 大小)

%~$PATH:0 - 查找列在路径环境变量的目录，并将 %0 扩充到找到的第一个完全合格的名称。如果环境变量名未被定义，或者没有找到文件，此组合键会扩充到空字符串，可以组合修饰符来得到多重结果：

%~dp0 - 仅将 %0 扩充到一个驱动器号和路径
%~nx0 - 仅将 %0 扩充到一个文件名和扩展名
%~fs0 - 仅将 %0 扩充到一个带有短名的完整路径名
%~dp$PATH:0 - 查找列在路径环境变量的目录，并将 %I 扩充到找到的第一个驱动器号和路径。
%~ftza0 - 将 %0 扩充到类似输出线路的 DIR
%0为当前批处理文件，如果0换成1为第一个文件，2为第2个

%0       代指批处理文件自身
%~d0    是指批处理所在的盘符
%~dp0   是盘符加路径
cd %~dp0 就是进入批处理所在目录了
~~~