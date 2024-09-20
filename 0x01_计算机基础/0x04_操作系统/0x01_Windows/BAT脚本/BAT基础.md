## 学习网站

1. [批处理之家](http://www.bathome.net/index.php)：推荐常用
2. [Windows 脚本技术 ](http://bbs.bathome.net/include/js/script56/html/vtorimicrosoftwindowsscripttechnologies.htm)：推荐常用
3. [中国DOS联盟论坛](http://www.cn-dos.net/forum/)：高手论坛，推荐
4. [Microsoft官方文档](https://learn.microsoft.com/zh-cn/windows-server/administration/windows-commands/windows-commands)：Microsoft的官方文档提供了详细的关于BAT脚本编程的指南、示例和参考资料。你可以访问Microsoft的官方网站，搜索"Windows Batch Scripting"或相关主题以获取更多信息。
5. [SS64.com](http://ss64.com/)：这是一个非常有用的网站，提供了大量BAT文件编程的命令和函数的详细介绍。你可以在该网站上找到具体命令的语法、示例和用法说明。
6. [DosTips.com](http://dostips.com/)：这个网站是一个活跃的BAT文件编程社区，你可以在其论坛上与其他BAT脚本编程爱好者交流并寻求帮助。此外，该网站还提供了一些教程和示例代码供学习和参考。



## bat文件与cmd文件的区别

+ 本质上没有区别，都是简单的文本编码方式，都可以用记事本创建、编辑和查看。

+ 两者所用的命令行代码也是共用的，只是cmd文件中允许使用的命令要比bat文件多。

+ cmd文件只有在windows2000以上的系统中才能运行，而bat文件则没有这个限制。

+ cmd文件的描述是“windows nt命令脚本” bat文件的描述是“ms dos批处理文件”

+ cmd是Win32命令，只能在32位系统中的命令行窗口中使用仅仅是基于windows环境下的假DOS，而bat是DOS命令，在任何dos环境下都可以使用



## 基础语法

1. 注释：使用REM关键字来添加注释。注释行以REM开头，可以在任何位置添加。
2. 命令执行方式：每个命令都可以单独占据一行，或者多个命令可以在同一行上，用"&&"符号分隔。
3. 变量：使用SET关键字定义变量，例如：`SET variable_name=value`。然后可以通过%variable_name%来引用变量值。
4. 控制结构：可以使用IF、FOR、GOTO等关键字来实现条件判断、循环和跳转。
5. 输入输出重定向：可以使用">"将命令的输出重定向到文件，使用"<"将文件内容作为命令的输入。
6. 执行其他脚本或程序：通过CALL关键字调用其他脚本或程序。



~~~ tex
@echo off #从本行开始关闭回显。一般批处理第一行都是这个
REM 这是一个示例的BAT文件

REM 定义变量
SET name=John
SET age=25

REM 输出变量值
echo My name is %name%
echo I am %age% years old.

REM 执行命令并将输出重定向到文件
dir > file.txt

REM 条件判断
IF %age% GTR 18 (
   echo I am an adult.
) ELSE (
   echo I am a minor.
)

REM 循环
FOR /L %%i IN (1,1,5) DO (
   echo Loop iteration: %%i
)

REM 调用其他脚本或程序
CALL another_script.bat

上述示例展示了一些常见的BAT文件语法，包括注释、变量定义、输出、重定向、条件判断、循环和调用其他脚本等。
需要注意的是，在BAT文件中，每个命令都应该占据一行，如果想在同一行上编写多个命令，需要使用"&&"符号进行分隔。
~~~



## 教程

### **什么是批处理？**

- 批处理(Batch)，也称为批处理脚本。顾名思义，批处理就是对某对象进行批量的处理。批处理文件的扩展
  名为bat

- **目前比较常见 的批处理包含两类：**

- - DOS批处理和PS批处理。
  - PS批处理是基于强大的图片编辑软件
  - Photoshop的，用来批量处理图片的脚本；
  - 而DOS批处理则是基于DOS命令的，用来自动地批量地执行
  - DOS命令以实现特定操作的脚本。这里要讲的就是DOS批处理



- 批处理是一种简化的脚本语言，它应用于DOS和Windows系统中，它是由DOS或者Windows系统内嵌的
  命令解释器（通常是[http://COMMAND.COM](https://link.zhihu.com/?target=http%3A//COMMAND.COM)或者CMD.EXE）解释运行
- 批处理类似于Unix中的Shell脚本。批处理文件具有.bat或者.cmd的扩展名，其最简单的例子，是逐行书写在命令行中会用到的各种命令。更复杂的情况，需要使用if，for，goto等命令控制程序的运行过程，如同C，Basic等中高级语言一样。如果需要实现更复杂的应用，利用外部程序是必要的，这包括系统本身提供的外部命令和第三方提供的工具或者软件



### **怎么创建批处理文件？**

- 批处理文件，或称为批处理程序，是由一条条的DOS命令组成的普通文本文件，可以用记事本直接编辑或用DOS命令创建，也可以用DOS下的文本编辑器Edit.exe来编辑。在“命令提示”下键入批处理文件的名称，或者双击该批处理文件，系统就会调用Cmd.exe运行该批处理程序
- 一般情况下，每条命令占据一行
- 当然也可以将多条命令用特定符号（如：&、&&、|、||等）分隔后写入同一行中
- 还有的情况就是像if、for等较高级的命令则要占据几行甚至几十几百行的空间。系统在解释运行批处理程序时，首先扫描整个批处理程序，然后从第一行代码开始向下逐句执行所有的命令，直至程序结尾或遇见exit命令或出错意外退出



### 常用命令

#### 文件夹管理

- - cd 显示当前目录名或改变当前目录。
  - md 创建目录。
  - rd 删除一个目录。
  - dir 显示目录中的文件和子目录列表。
  - tree 以图形显示驱动器或路径的文件夹结构。
  - path 为可执行文件显示或设置一个搜索路径。
  - xcopy 复制文件和目录树。

#### 文件管理

- - type 显示文本文件的内容。
  - copy 将一份或多份文件复制到另一个位置。
  - del 删除一个或数个文件。
  - move 移动文件并重命名文件和目录。(Windows XP Home Edition中没有)
  - ren 重命名文件。
  - replace 替换文件。
  - attrib 显示或更改文件属性。
  - find 搜索字符串。
  - fc 比较两个文件或两个文件集并显示它们之间的不同

#### 网络命令

- - ping 进行网络连接测试、名称解析
  - ftp 文件传输
  - net 网络命令集及用户管理
  - telnet 远程登陆
  - ipconfig显示、修改TCP/IP设置
  - msg 给用户发送消息
  - arp 显示、修改局域网的IP地址-物理地址映射列表

#### 系统管理

- - at 安排在特定日期和时间运行命令和程序
  - shutdown立即或定时关机或重启
  - tskill 结束进程
  - taskkill结束进程(比tskill高级，但WinXPHome版中无该命令)
  - tasklist显示进程列表(Windows XP Home Edition中没有)
  - sc 系统服务设置与控制
  - reg 注册表控制台工具
  - powercfg控制系统上的电源设置



### 命令解析

echo 和 @   回显命令

~~~ shell
回显命令
@  #关闭单行回显
echo off  #从下一行开始关闭回显
@echo off  #从本行开始关闭回显。一般批处理第一行都是这个
echo on #从下一行开始打开回显
echo #显示当前是 echo off 状态还是 echo on 状态
echo. #输出一个”回车换行”，空白行
#(同echo, echo; echo+ echo[ echo] echo/ echo)
~~~



errorlevel   查看返回码

~~~ shell
echo %errorlevel%
每个命令运行结束，可以用这个命令行格式查看返回码
默认值为0，一般命令执行出错会设 errorlevel 为1
~~~



dir   显示文件夹内容

~~~ shell
显示文件夹内容
dir #显示当前目录中的文件和子目录
dir /a #显示当前目录中的文件和子目录，包括隐藏文件和系统文件
dir c: /a:d #显示 C 盘当前目录中的目录
dir c: /a:-d #显示 C 盘根目录中的文件
dir c: /b/p #/b只显示文件名，/p分页显示
dir *.exe /s #显示当前目录和子目录里所有的.exe文件
~~~



cd  切换目录

~~~ shell
切换目录
cd #进入根目录
cd #显示当前目录
cd /d d:sdk #可以同时更改盘符和目录
~~~



md  创建目录

~~~ shell
创建目录
md d:abc #如果 d:a 不存在，将会自动创建中级目录
#如果命令扩展名被停用，则需要键入 mkdir abc
~~~



rd  删除目录

~~~ shell
删除目录
rd abc #删除当前目录里的 abc 子目录，要求为空目录
rd /s/q d:temp #删除 d:temp 文件夹及其子文件夹和文件，/q安静模式
~~~



del 删除文件

~~~ shell
删除文件
del d:test.txt #删除指定文件，不能是隐藏、系统、只读文件
del /q/a/f d:temp*.*
删除 d:temp 文件夹里面的所有文件，包括隐藏、只读、系统文件，不包括子目录
del /q/a/f/s d:temp*.*
删除 d:temp 及子文件夹里面的所有文件，包括隐藏、只读、系统文件，不包括子目录
~~~



ren  重命名

~~~ shell
重命名命令
ren d:temp tmp #支持对文件夹的重命名
~~~



cls 清屏



type 显示文件内容

~~~ shell
type c:boot.ini #显示指定文件的内容，程序文件一般会显示乱码
type *.txt #显示当前目录里所有.txt文件的内容
~~~



copy   拷贝文件

~~~ shell
拷贝文件
copy c:test.txt d:test.bak
复制 c:test.txt 文件到 d: ，并重命名为 test.bak
copy con test.txt
从屏幕上等待输入，按 Ctrl+Z 结束输入，输入内容存为test.txt文件
con代表屏幕，prn代表打印机，nul代表空设备
copy 1.txt + 2.txt 3.txt
合并 1.txt 和 2.txt 的内容，保存为 3.txt 文件
如果不指定 3.txt ，则保存到 1.txt
copy test.txt +
复制文件到自己，实际上是修改了文件日期
~~~



title   设置标题

~~~ shell
设置cmd窗口的标题
title 新标题 #可以看到cmd窗口的标题栏变了
~~~



ver 显示系统版本



pause 暂停命令 



rem 和 ::    注释命令

注释行不执行操作



date 和 time  日期和时间

~~~ shell
date #显示当前日期，并提示输入新日期，按"回车"略过输入
date/t #只显示当前日期，不提示输入新日期
time #显示当前时间，并提示输入新时间，按"回车"略过输入
time/t #只显示当前时间，不提示输入新时间
~~~



goto 和 :   跳转命令

~~~ shell
:label #行首为:表示该行是标签行，标签行不执行操作
goto label #跳转到指定的标签那一行
~~~



find (外部命令)   查找命令

~~~ shell
查找命令
find "abc" c:test.txt
在 c:test.txt 文件里查找含 abc 字符串的行
如果找不到，将设 errorlevel 返回码为1
find /i “abc” c:test.txt
查找含 abc 的行，忽略大小写
find /c "abc" c:test.txt
显示含 abc 的行的行数
~~~



more (外部命令)  逐屏显示

~~~ shell
more c:test.txt #逐屏显示 c:test.txt 的文件内容
~~~



tree  显示目录结构

~~~ shell
tree d: #显示D盘的文件目录结构
~~~



&  顺序执行

> 顺序执行多条命令，而不管命令是否执行成功



&& 顺序执行

~~~ shell
顺序执行多条命令，当碰到执行出错的命令后将不执行后面的命令
find "ok" c:test.txt && echo 成功
如果找到了"ok"字样，就显示"成功"，找不到就不显示
~~~



||   顺序执行

~~~ shell
顺序执行多条命令，当碰到执行正确的命令后将不执行后面的命令
find "ok" c:test.txt || echo 不成功
如果找不到"ok"字样，就显示"不成功"，找到了就不显示
~~~



| 管道命令

~~~ shell
dir *.* /s/a | find /c ".exe"
管道命令表示先执行 dir 命令，对其输出的结果执行后面的 find 命令
该命令行结果：输出当前文件夹及所有子文件夹里的.exe文件的个数
type c:test.txt|more
这个和 more c:test.txt 的效果是一样的
~~~



\>和 >>  输出重定向命令

~~~ shell
> 清除文件中原有的内容后再写入
>> 追加内容到文件末尾，而不会清除原有的内容
主要将本来显示在屏幕上的内容输出到指定文件中
指定文件如果不存在，则自动生成该文件
type c:test.txt >prn
屏幕上不显示文件内容，转向输出到打印机
echo hello world>con
在屏幕上显示hello world，实际上所有输出都是默认 >con 的
copy c:test.txt f: >nul
拷贝文件，并且不显示"文件复制成功"的提示信息，但如果f盘不存在，还是会显示出错信息
copy c:test.txt f: >nul 2>nul
不显示”文件复制成功”的提示信息，并且f盘不存在的话，也不显示错误提示信息
echo ^^W ^> ^W>c:test.txt
生成的文件内容为 ^W > W
^ 和 > 是控制命令，要把它们输出到文件，必须在前面加个 ^ 符号
~~~



< 从文件中获得输入信息

~~~ shell
从文件中获得输入信息，而不是从屏幕上
一般用于 date time label 等需要等待输入的命令
@echo off
echo 2005-05-01>temp.txt
date <temp.txt
del temp.txt
这样就可以不等待输入直接修改当前日期
~~~



%0 %1 %2 %3 %4 %5 %6 %7 %8 %9 %*  命令行传递给批处理的参数

~~~ shell
命令行传递给批处理的参数
%0 批处理文件本身
%1 第一个参数
%9 第九个参数
%* 从第一个参数开始的所有参数
批参数(%n)的替代已被增强。您可以使用以下语法:
%~1 - 删除引号(" )， 扩充 %1
%~f1 - 将 %1 扩充到一个完全合格的路径名
%~d1 - 仅将 %1 扩充到一个驱动器号
%~p1 - 仅将 %1 扩充到一个路径
%~n1 - 仅将 %1 扩充到一个文件名
%~x1 - 仅将 %1 扩充到一个文件扩展名
%~s1 - 扩充的路径指含有短名
%~a1 - 将 %1 扩充到文件属性
%~t1 - 将 %1 扩充到文件的日期/时间
%~z1 - 将 %1 扩充到文件的大小
%~$PATH : 1 - 查找列在 PATH 环境变量的目录，并将 %1
扩充到找到的第一个完全合格的名称。如果环境
变量名未被定义，或者没有找到文件，此组合键会
扩充到空字符串
可以组合修定符来取得多重结果:
%~dp1 - 只将 %1 扩展到驱动器号和路径
%~nx1 - 只将 %1 扩展到文件名和扩展名
%~dp$PATH:1 - 在列在 PATH 环境变量中的目录里查找 %1，
并扩展到找到的第一个文件的驱动器号和路径。
%~ftza1 - 将 %1 扩展到类似 DIR 的输出行。
可以参照 call/? 或 for/? 看出每个参数的含意
echo load "%%1" "%%2">c:test.txt
生成的文件内容为 load "%1"  "%2"
批处理文件里，用这个格式把命令行参数输出到文件
~~~



if   判断命令 

~~~ shell
if "%1"=="/a" echo 第一个参数是/a
if /i "%1" equ "/a" echo 第一个参数是/a
/i 表示不区分大小写，equ 和 == 是一样的，其它运算符参见 if/?
if exist c:test.bat echo 存在c:test.bat文件
if not exist c:windows (
echo 不存在c:windows文件夹
)
if exist c:test.bat (
echo 存在c:test.bat
) else (
echo 不存在c:test.bat
)
~~~



setlocal 和 endlocal   设置”命令扩展名”和”延缓环境变量扩充”

~~~ shell
设置”命令扩展名”和”延缓环境变量扩充”
SETLOCAL ENABLEEXTENSIONS #启用"命令扩展名"
SETLOCAL DISABLEEXTENSIONS #停用"命令扩展名"
SETLOCAL ENABLEDELAYEDEXPANSION #启用"延缓环境变量扩充"
SETLOCAL DISABLEDELAYEDEXPANSION #停用"延缓环境变量扩充"
ENDLOCAL #恢复到使用SETLOCAL语句以前的状态
“命令扩展名”默认为启用
“延缓环境变量扩充”默认为停用
批处理结束系统会自动恢复默认值
可以修改注册表以禁用"命令扩展名"，详见 cmd /? 。所以用到"命令扩展名"的程
序，建议在开头和结尾加上 SETLOCAL ENABLEEXTENSIONS 和 ENDLOCAL 语句，以确
保程序能在其它系统上正确运行
"延缓环境变量扩充"主要用于 if 和 for 的符合语句，在 set 的说明里有其实用例程
~~~



set   设置变量

~~~ shell
设置变量
引用变量可在变量名前后加 % ，即 %变量名%
set #显示目前所有可用的变量，包括系统变量和自定义的变量
echo %SystemDrive% #显示系统盘盘符。系统变量可以直接引用
set p #显示所有以p开头的变量，要是一个也没有就设errorlevel=1
set p=aa1bb1aa2bb2 #设置变量p，并赋值为 = 后面的字符串，即aa1bb1aa2bb2
echo %p% #显示变量p代表的字符串，即aa1bb1aa2bb2
echo %p:~6% #显示变量p中第6个字符以后的所有字符，即aa2bb2
echo %p:~6,3% #显示第6个字符以后的3个字符，即aa2
echo %p:~0,3% #显示前3个字符，即aa1
echo %p:~-2% #显示最后面的2个字符，即b2
echo %p:~0,-2% #显示除了最后2个字符以外的其它字符，即aa1bb1aa2b
echo %p:aa=c% #用c替换变量p中所有的aa，即显示c1bb1c2bb2
echo %p:aa=% #将变量p中的所有aa字符串置换为空，即显示1bb12bb2
echo %p:*bb=c% #第一个bb及其之前的所有字符被替换为c，即显示c1aa2bb2
set p=%p:*bb=c% #设置变量p，赋值为 %p:*bb=c% ，即c1aa2bb2
set /a p=39 #设置p为数值型变量，值为39
set /a p=39/10 #支持运算符，有小数时用去尾法，39/10=3.9，去尾得3，p=3
set /a p=p/10 #用 /a 参数时，在 = 后面的变量可以不加%直接引用
set /a p=”1&0″ #”与”运算，要加引号。其它支持的运算符参见set/?
set p= #取消p变量
set /p p=请输入
屏幕上显示”请输入”，并会将输入的字符串赋值给变量p
注意这条可以用来取代 choice 命令
注意变量在 if 和 for 的复合语句里是一次性全部替换的，如
@echo off
set p=aaa
if %p%==aaa (
echo %p%
set p=bbb
echo %p%
)
结果将显示
aaa
aaa
因为在读取 if 语句时已经将所有 %p% 替换为aaa
这里的"替换"，在 /? 帮助里就是指"扩充"、"环境变量扩充"
可以启用”延缓环境变量扩充”，用 ! 来引用变量，即 !变量名!
@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
set p=aaa
if %p%==aaa (
echo %p%
set p=bbb
echo !p!
)
ENDLOCAL
结果将显示
aaa
bbb
还有几个动态变量，运行 set 看不到
%CD% #代表当前目录的字符串
%DATE% #当前日期
%TIME% #当前时间
%RANDOM% #随机整数，介于0~32767
%ERRORLEVEL% #当前 ERRORLEVEL 值
%CMDEXTVERSION% #当前命令处理器扩展名版本号
%CMDCMDLINE% #调用命令处理器的原始命令行
可以用echo命令查看每个变量值，如 echo %time%
注意 %time% 精确到毫秒，在批处理需要延时处理时可以用到
~~~



start  调用外部程序

~~~ shell
批处理中调用外部程序的命令，否则等外部程序完成后才继续执行剩下的指令
启动单独的“命令提示符”窗口来运行指定程序或命令。如果在没有参数的情况下使用，start 将打开第二个命令提示符窗口。
~~~

语法

~~~ bash
start ["title"] [/dPath] [/i] [/min] [/max] [{/separate | /shared}] [{/low | /normal | /high | /realtime | /abovenormal | belownormal}] [/wait] [/b] [FileName] [parameters]
~~~

参数

~~~ tex\
"title" 指定在“命令提示符”窗口标题栏中显示的标题。 
/dpatch 指定启动目录。 
/i 将 Cmd.exe 启动环境传送到新的“命令提示符”窗口。 
/min 启动新的最小化窗口。 
/max 启动新的最大化窗口。 
/separate 在单独的内存空间启动 16 位程序。 
/shared 在共享的内存空间启动 16 位程序。 
/low 以空闲优先级启动应用程序。 
/normal 以一般优先级启动应用程序。 
/high 以高优先级启动应用程序。 
/realtime 以实时优先级启动应用程序。 
/abovenormal 以超出常规优先级的方式启动应用程序。 
/belownormal 以低出常规优先级的方式启动应用程序。 
/wait 启动应用程序，并等待其结束。 
/b 启动应用程序时不必打开新的“命令提示符”窗口。除非应用程序启用 CTRL+C，否则将忽略 CTRL+C 操作。使用 CTRL+BREAK 中断应用程序。 
非执行文件只要将文件名作为命令键入，即可通过其文件关联运行该文件。有关使用 assoc 和 ftype 在命令脚本中创建这些关联的详细信息，请参阅“”。 
在运行的命令的第一个标记为“CMD”字符串但不包括扩展名或路径限定符时，“CMD”将被 COMSPEC 变量的值取代。这样可以防止用户从当前目录选取 cmd。 
当您运行 32 位图形用户界面 (GUI) 应用程序时，cmd 不会在返回到命令提示符之前等待应用程序退出。如果从命令脚本运行应用程序，则不会发生这种新情况。在运行的命令中第一个符号不包括扩展名的情况下，Cmd.exe 使用 PATHEXT 环境变量的值确定要查找的扩展名以及查找顺序。PATHEXT 变量的默认值为：COM;.EXE;.BAT;.CMD（语法与 PATH 变量相同，使用分号分开不同元素）。当您搜索可执行文件且在任何扩展名上都没有匹配项时，start 将搜索目录名。 
~~~



**具体例子：** 
说明：如果你所在程序的路径中带有空格，那么必须用“”把路径括起来，否则系统会提示找不到XX文件，另外在运行某些程序时，需在路径的前面加一对空白的“”，表示创建一个空白的窗口，它指向的程序是XXXXXXXX。还有就是别忘了空格。 

当我想运行位于“D:/draw/”的“photoshop.exe”时，应该使用以下命令：

~~~ shell
当我想运行位于“D:/draw/”的“photoshop.exe”使，应该使用以下命令：
start “”“D:/draw/photoshop.exe” 表示以常规窗口运行程序
~~~

~~~ bash
如果想让程序以最大化窗口运行，则使用以下命令：
start /max“”“D:/draw/photoshop.exe” 表示以最大化窗口运行程序
~~~

~~~ bash
最小化这是这样：
start /min "" "D:/draw/photoshop.exe" 表示以最小化窗口运行程序
~~~

~~~ bash
等待某个程序允许完毕，也就是窗口关闭后，再打开下一个程序这可以这样：
start /w "" "D:/draw/photoshop.exe"
start "" cmd.exe
~~~



call  调用另外一个批处理

~~~ shell
批处理中调用另外一个批处理的命令，否则剩下的批处理指令将不会被执行
有时有的应用程序用start调用出错的，也可以call调用
~~~

> call与start的比较：
>
> 1.call是从批处理程序调用另一个批处理程序，并且不终止父批处理程序（如果不用call而直接调用别的批处理文件，那么执行完那个批处理文件后将无法返回当前文件并执行当前文件的后续命令）；而start是启动一个单独的窗口运行指定的程序或命令。
>
> 2.call命令能够调用标签，与goto命令的区别是：采用 call：label的形式（goto采用的形式是goto label，没有冒号），并且采用call调用完标签之后，会继续顺序执行后续命令，而goto则是直接跳转至标签进行执行，并顺序执行标签后的内容；start不能够调用标签。
>
> 3.调用范围不同：call主要用来进行批处理的内部调用，如call :pend 和一些dos命令的调用如call set test=2，但也可以调用其他可执行文件，而start则不能进行内部调用，但其可以执行基本上所有的外部程序，还可以执行shell，如打开文件夹start "" "%WINDIR%"，安装网络打印机start "" "\\IP\Printer"等等。
>
> 4.调用方式不同：call是严格意义上而言的调用，在call另一个批处理时是在同一窗体进程里执行，而start则是执行，所以在执行另外一个批处理时是在不同的窗体进程里进行，也就是说在新开启的进程里执行，虽然start可以加入b参数，但其结果却完全不同。如我们使用call set test=2 和 start /b set test=2 看似执行的结果相同，但是我们发现后者有两个进程，而且在窗体里要执行两次exit才能退出，所以当我们使用start来执行一个批处理后最好在被调用的批处理中也加一个exit，否则无法退出被调用的批处理的dos窗体，但是使用call调用时如果在被调用的批处理中存在exit则会直接结束原始和被调用的批处理程序，这是一个非常严重的问题。建议在被调用的批处理中使用goto :eof来取代exit。
>
> 5.调用结果不同：call调用不仅可以把参数或变量传给被调用的批处理，而且被调用的批处理也可以设置参数或变量以便回传，而start只能把参数或变量传给被调用的批处理，却不能回传参数或变量，这一点实际上是第二点的延伸。另外，我们还必须注意一点：使用call调用其他批处理时，在被调用的批处理中若我们使用goto命令的时候，建议要使用与原批处理中不同的标签名来跳转，否则可能会跳转到原批处理中而不能保证完整的执行被调用批处理中的所有语句。



choice (外部命令)  选择

~~~ shell
选择命令
让用户输入一个字符，从而选择运行不同的命令，返回码errorlevel为1234……
win98里是choice.com
win2000pro里没有，可以从win98里拷过来
win2003里是choice.exe
choice /N /C y /T 5 /D y>nul
延时5秒
~~~



assoc 和 ftype   文件关联

~~~ shell
assoc 设置'文件扩展名'关联，关联到'文件类型'
ftype 设置'文件类型'关联，关联到'执行程序和参数'
当你双击一个.txt文件时，windows并不是根据.txt直接判断用 notepad.exe 打开
而是先判断.txt属于 txtfile '文件类型'
再调用 txtfile 关联的命令行 txtfile=%SystemRoot%system32NOTEPAD.EXE %1
可以在"文件夹选项"→"文件类型"里修改这2种关联
assoc #显示所有'文件扩展名'关联
assoc .txt #显示.txt代表的'文件类型'，结果显示 .txt=txtfile
assoc .doc #显示.doc代表的'文件类型'，结果显示 .doc=Word.Document.8
assoc .exe #显示.exe代表的'文件类型'，结果显示 .exe=exefile
ftype #显示所有'文件类型'关联
ftype exefile #显示exefile类型关联的命令行，结果显示 exefile="%1" %*
assoc .txt=Word.Document.8
设置.txt为word类型的文档，可以看到.txt文件的图标都变了
assoc .txt=txtfile
恢复.txt的正确关联
ftype exefile="%1" %*
恢复 exefile 的正确关联
如果该关联已经被破坏，可以运行 command.com ，再输入这条命令
~~~



pushd 和 popd  切换当前目录

~~~ shell
切换当前目录
@echo off
c: & cd & md mp3 #在 C: 建立 mp3 文件夹
md d:mp4 #在 D: 建立 mp4 文件夹
cd /d d:mp4 #更改当前目录为 d:mp4
pushd c:mp3 #保存当前目录，并切换当前目录为 c:mp3
popd #恢复当前目录为刚才保存的 d:mp4
~~~



for  循环命令

~~~ shell
循环命令
这个比较复杂，请对照 for/? 来看
for %%i in (c: d: e: f:) do echo %%i
依次调用小括号里的每个字符串，执行 do 后面的命令
注意%%i，在批处理中 for 语句调用参数用2个%
默认的字符串分隔符是"空格键"，"Tab键"，"回车键"
for %%i in (*.txt) do find "abc" %%i
对当前目录里所有的txt文件执行 find 命令
for /r . %%i in (*.txt) do find "abc" %%i
在当前目录和子目录里所有的.txt文件中搜索包含 abc 字符串的行
for /r . %%i in (.) do echo %%~pni
显示当前目录名和所有子目录名，包括路径，不包括盘符
for /r d:mp3 %%i in (*.mp3) do echo %%i>>d:mp3.txt
把 d:mp3 及其子目录里的mp3文件的文件名都存到 d:mp3.txt 里去
for /l %%i in (2,1,8) do echo %%i
生成2345678的一串数字，2是数字序列的开头，8是结尾，1表示每次加1
for /f %%i in ('set') do echo %%i
对 set 命令的输出结果循环调用，每行一个
for /f "eol=P" %%i in ('set') do echo %%i
取 set 命令的输出结果，忽略以 P 开头的那几行
for /f %%i in (d:mp3.txt) do echo %%i
显示 d:mp3.txt 里的每个文件名，每行一个，不支持带空格的名称
for /f "delims=" %%i in (d:mp3.txt) do echo %%i
显示 d:mp3.txt 里的每个文件名，每行一个，支持带空格的名称
for /f "skip=5 tokens=4" %%a in ('dir') do echo %%a
对 dir 命令的结果，跳过前面5行，余下的每行取第4列
每列之间的分隔符为默认的"空格"
可以注意到 dir 命令输出的前5行是没有文件名的
for /f "tokens=1,2,3 delims=- " %%a in ('date /t') do (
echo %%a
echo %%b
echo %%c
)
对 date /t 的输出结果，每行取1、2、3列
第一列对应指定的 %%a ，后面的 %%b 和 %%c 是派生出来的，对应其它列
分隔符指定为 - 和"空格"，注意 delims=- 后面有个"空格"
其中 tokens=1,2,3 若用 tokens=1-3 替换，效果是一样的
for /f "tokens=2* delims=- " %%a in ('date /t') do echo %%b
取第2列给 %%a ，其后的列都给 %%b
~~~



subst (外部命令)  映射磁盘

~~~ shell
subst z: serverd #这样输入z:就可以访问serverd了
subst z: /d #取消该映射
subst #显示目前所有的映时
~~~



xcopy (外部命令)  文件拷贝

~~~ shell
xcopy d:mp3 e:mp3 /s/e/i/y
复制 d:mp3 文件夹、所有子文件夹和文件到 e: ，覆盖已有文件
加 /i 表示如果 e: 没有 mp3 文件夹就自动新建一个，否则会有询问
~~~



### 脚本批量打开文件

#### 脚本内容

~~~ shell
@echo off

start "" "E:\Records\WebDesignLearning\laoduVue\records\record1.md"
start "" "E:\Records\WebDesignLearning\laoduVUe\records\Vue.pdf"
start D:\MicrosoftVSCode\Code.exe "E:\Records\WebDesignLearning\laoduVue\code"	
start https://www.bilibili.com/video/BV17h41137i4/
~~~



#### 脚本解释

① 默认方式打开应用

~~~ shell
start "" "文件路径"
~~~

实例：

~~~ shell
start "" "E:\Records\WebDesignLearning\laoduVue\records\record1.md"
~~~



② 指定应用打开文件夹或文件

~~~ shell
start 应用路径 "文件路径"
~~~

实例： 用vscode 打开指定文件夹

~~~ shell
start D:\MicrosoftVSCode\Code.exe "E:\Records\WebDesignLearning\laoduVue\code"
~~~



③ 默认浏览器打开指定网页

~~~ shell
start 网页地址
~~~

实例：浏览器打开学习视频

~~~ shell
start https://www.bilibili.com/video/BV17h41137i4/
~~~



#### 脚本文件编写

先创建一个txt文件编写脚本内容，编写成功后另存为 xxx.bat 文件，双击即可运行

另存为bat文件后，可以右击该文件，选项里有`编辑`，点击编辑，即可改写脚本内容

> **注意： 路径和文件名称需要是英文，中文会出错，除此以外，路径文件名不能有空格，如有上述情况，需要自行修改文件或者文件夹名**



### 自动登录163邮箱

~~~ bash
@echo off
set "var=163邮箱的用户名"
set "num=163邮箱的密码"
start "163邮箱登录中" "https://reg.163.com/logins.jsp?username=%var%&password=%num%&url=http://fm163.163.com/coremail/fcg/ntesdoor2"
exit
~~~

把上面这段代码复制到文件的txt文件中，把 163邮箱的用户名 和 163邮箱的密码 改成你自己的内容，另存为 ***.bat, 类型为 全部

双击这个批处理即可自动登录你的163邮箱



### 常见问题

- 1.如果你自己编写的.bat文件，双击打开，出现闪退

- - 原因：执行速度很快，执行完之后，自行关闭
  - 解决办法：在最后面一行加上 pause



- 2.批处理.bat 文件中输出中文乱码

- - 原因：记事本新建的文件，编码不是 utf-8
  - 解决方法：用【记事本】打开>【另存为】>【修改编码为：utf-8】