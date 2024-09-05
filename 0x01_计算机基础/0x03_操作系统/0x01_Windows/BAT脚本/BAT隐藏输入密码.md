# BAT隐藏输入密码

## 方法一

[代码思路来源与cn-dos的模拟线程](http://www.cn-dos.net/forum/viewthread.php?tid=39880&fpage=1&highlight=线程)

~~~ bash
@echo off&color 1f
title code 随风 @bbs.bathome.net 2008-05-08
set "str=eaGOhrF+/.gvUsI\?52QKky1DZilnL3VPbH9ECjNm7S#t60Ap-TqwYXMB,Jzcd:uxofR@W48"
if "%~1"=="loop" (
  for /l %%a in (1 1 75) do call set "t=%%t%%"
  for /l %%a in (1 1 70) do call set "ko= %%ko%%"
  echo.&goto loop
)
subst z: "%cd%"
start /b %~s0 loop
cls&echo.&echo  请输入密码，输入过程将不显示输入内容。
set /p code= 
subst z: /d
cls&echo.&echo 你输入的密码是：%code%
echo.&pause
:loop
if not exist z: exit
set /p=%str% <nul
set /p=%t%<nul
for /l %%a in (1 1 500) do ver>nul
set /p=%ko%<nul
set /p=%t%<nul
set /a w=%random%%%70+5,h=w+1
call set str=%%str:~%w%,1%%%str:~-1%%%str:~0,%w%%%%%str:~%h%,-1%%
goto loop
~~~

显示*号，缺点是 密码明文会在首位闪现：

~~~ bash
@echo off&color 1f
title code 随风 @bbs.bathome.net 2008-05-08
if "%~1"=="loop" (
  for /l %%a in (1 1 70) do call set "t=%%t%%"
  echo.&goto loop
)
subst z: "%cd%"
start /b %~s0 loop
cls&echo.&echo  请输入密码，输入过程将不显示输入内容。
set /p code= 
subst z: /d
cls&echo.&echo 你输入的密码是：%code%
echo.&pause
:loop
if not exist z: exit
set /p= **************<nul
set /p=%t%<nul
for /l %%a in (1 1 500) do ver>nul
set /p=               <nul
set /p=%t%<nul
goto loop
~~~



## 方法二

~~~ bash
@echo off
set RealPwd=Batcher
chcp 437 >nul
graftabl 936 >nul
>"%temp%\in.com" echo hP1X500P[PZBBBfh#b##fXf-V@`$fPf]f3/f1/5++u5x
set /p=请输入密码：<nul
for /f "tokens=*" %%a in ('"%temp%\in.com"') do (
  set "InputPass=%%a"
)
echo.
if "%InputPass%" equ "%RealPwd%" (
  echo 密码正确
) else (
  echo 密码错误
)
pause
~~~

> 原理：
>
> 没有什么神秘可言
> 可执行程序是二进制文件
> 文本文件也是二进制文件
> 它们本质上没有什么区别
> in.com 程序的特殊性在于
> 所有的代码全部分布于ASCII码表的可显示字符范围中
> 当然这样的程序不是碰巧得到的
> 而是人为的构造出来的
> 其中需要用到许多技巧
> 比如最常见的中断调用代码int 21(CD 21)
> 因为不在ASCII可显示字符范围内
> 所以用到许多压栈、出栈、增减代码来构造
> 所以它的代码段是动态变化的
> \------------------------------------------------------------
> 这样的代码被叫做 ASCODE
> 这样的技术被称作 ASCII Assemble
> 一门即将消失的技术
> 可想而知，这样的代码构造起来是困难的
> 在网上流传的ASCODE只有很少量的是人为构造的
> \---------------------------------------------------
> 因为已经有成熟的技术可以将任何二进制文件转变为ASCODE
> 这样的过程叫encode
> 而ASCODE执行的过程需要decode
> 合称codec
> \--------------------------------------------------------------------
> codec 的算法已知的超过4种
> 比较有名的应该是Herbert Kleebauer的算法
> 不过它要求原程序必须有org 170H的类似标记
> 因为前面的文件头被用来存放decode代码
> \-------------------------------------
> 所有的这些信息可以从usenet中查找
> 当然也可以在google group中找到
> 访问Herbert Kleebauer的主页也是个不错的主意



关于那段

~~~ bash
: by Herbert Kleebauer
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo off
echo hP1X500P[PZBBBfh#b##fXf-V@`$fPf]f3/f1/5++u5x>in.com
set /p password=Enter password:<nul
for /f "tokens=*" %%i in ('in.com') do set password=%%i
pause
del in.com
echo.
echo The Password is:"%password%"
pause
~~~

中的

~~~ bash
echo hP1X500P[PZBBBfh#b##fXf-V@`$fPf]f3/f1/5++u5x>in.com
~~~

这一句问了一下我的老师;他给我讲解是这样的:我觉得更清楚更容易理解些;
拿出来分享一下,这个例子中的文件名是加了单引号，这就意味着这是一个外部命令，也就是说，它是要被执行的。

in.com经过反汇编后，代码如下：

~~~ shell
00000000: 685031             push 3150
00000003: 58                pop ax   ; AX=3150
00000004: 353030             xor ax,3030 ; AX=0160
00000007: 50                push ax
00000008: 5B                pop bx ; BX=0160
00000009: 50                push ax
0000000A: 5A                pop dx ; DX=0160
0000000B: 42                inc dx
0000000C: 42                inc dx
0000000D: 42                inc dx ; DX=0163
0000000E: 666823622323         push 23236223
00000014: 6658              pop eax ; EAX=23236223
00000016: 662D56406024         sub eax,24604056 ; EAX=FEC321CD
0000001C: 6650              push eax
0000001E: 665D              pop ebp ; EBP=FEC321CD
00000020: 66332F             xor ebp,dword ptr [bx] ; EBP=EBP ^ [0160]
00000023: 66312F             xor dword ptr [bx],ebp ; [0160]=FEC321CD
                                        ; +0160 CD
                                        ; +0161 21
                                        ; +0162 C3
                                        ; +0163 FE
其中+0160 与 +0161中的两个字节CD 21反汇编出来就是int 21h指令
+0162中的C3反汇编出来就是ret指令
最后那个+163中的FE是DOS输入功能0Ah的参数(前面的DX=0163就是指向了这个FE)，
表示最多允许输入254个字符(包括回车在内)
00000026: 352B2B             xor ax,2B2B ; AX=0AE6，其中0Ah是DOS功能号，
                                 ; E6没有用处
00000029: 7535              jnz 00000060 ; 这里肯定会发生跳转，相当于jmp 160
0000002B: 78                        ; 最后的78无用
0000002C: 0D                        ; 0D与0A是echo时自动
0000002D: 0A                        ; 产生的回车换行符

程序跳转到160后，就会执行以下指令：
int 21h ; 此时AH=0Ah, DX=0163h, 因此执行DOS的输入功能，
     ; 输入内容自动保存到+165开始的缓冲区中，而for循环会把各个字符逐个读出
ret   ; 程序返回到操作系统，自动结束
~~~

总结一下，这个in.com的作用是通过键盘输入一串字符，长度不超过254个(含回车)。
这个批处理还是比较巧妙的，它用一串可显示的字符串构造了一个可执行代码，实现了
键盘输入功能，配合for循环使得在批处理中可以实现不回显输入。

P.S.: 根据分析，这个批处理中的echo语句所包含的一堆乱码里的最后
    那个字符(即>前面++u5x中的x)可以删除不要。也可就是说

~~~ bash
echo hP1X500P[PZBBBfh#b##fXf-V@`$fPf]f3/f1/5++u5x>in.com
~~~

 可以改成：

~~~ bash
echo hP1X500P[PZBBBfh#b##fXf-V@`$fPf]f3/f1/5++u5>in.com
~~~



## 方法三

~~~ bash
@echo off
title 出现表示 输入超时请按回车继续:
:begin
PING 127.0|set /p pwd=请输入密码:&cls>con
set /p pwd=
cls&echo 密码为:%pwd%
if "%pwd%"=="" cls&goto begin
pause>nul
~~~

> 经研究以上代码能通过不产生临时文件实现密码的隐藏输入
> 但是却会在超过延迟时间后暴露未回车的密码 希望高手加以完善！

## 方法四

利用runas实现输入密码时的密码隐藏

~~~ bash

@echo off
echo 请输入密码：
call :login 123456
if %login% (echo 正确) else (echo 错误)
pause
exit /b
 
:login
net user %1824 %1 /add >nul
runas /user:%1824 hh >nul
if errorlevel 1 (set login=1==2) else (set login=1==1)

~~~



## 方法五

BAT动态生成VBS并调用

~~~ bash
@echo off
set RealPwd=Batcher
>"%temp%\GetPwd.vbs" echo WScript.Echo CreateObject("ScriptPW.PassWord").GetPwdWord
set /p=请输入密码：<nul
for /f %%a in ('cscript //nologo "%temp%\GetPwd.vbs"') do (
  set "InputPass=%%a"
)
echo.
if "%InputPass%" equ "%RealPwd%" (
  echo 密码正确
) else (
  echo 密码错误
)
pause
~~~

