很多用了Win10版本系统的人都会发现，Windows对程序的运行权限是控制得更加严格了，即使你将UAC控制放至最低，如果没有特别赋予外来程序管理员运行权限的话，很多程序都会运行出错，包括很多用于系统维护的批处理程序由于运行权限不够都会导致出错，最简单的方法莫过于用右键点击程序，然后选择“以管理员身份运行”，对于那些需要经常使用的程序，我们可以进入程序的属性里面勾选上“以管理员身份运行”，这样每次点击它都会直接以管理员身份运行了。以上这些都是对单个程序的临时解决措施。那么对于众多的批处理程序，每次都用右键觉得很麻烦，又不可能到每个程序的属性里面去设置一下，效率低下，那有没有办法让批处理程序能够自动获取管理员运行权限呢，让其直接双击即可实现正常运行，答案当然是有的。下面是我从网上找到的几种提升批处理运行权限的方法，另外也有一些本人自己摸索出来的方法。

**方法1：借用VBS脚本**

~~~ vbscript
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
~~~



**方法2：借用VBS脚本**

~~~ vbscript
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
~~~



**方法3：借用VBS脚本**

~~~ vbscript
echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"
"%temp%\getadmin.vbs"
del /f /q "%temp%\getadmin.vbs" >nul
exit /B
~~~



**方法4：借用第三方程序**

~~~ bat
nircmd elevate cmd /c "%0"
exit /B
~~~



**方法5：runas命令**

~~~ bash
runas /noprofile /user:Administrator "%0"
exit /B
~~~

​       以上是批处理获取管理员权限的5种方法，只要把这几个命令放进批处理优先执行即可，当然还要加上判断是否已经获取管理员权限的语句，否则批处理就会无限循环下去。

​      判断批处理是否已获取管理员权限的方法，网上用的最多的是下面这句命令：



方法1为例：

~~~ bash
\>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
:UACAdmin
echo 已获取管理员权限
~~~

​       用cacls命令来判断管理员权限固然不错，但经过实践我发现cacls返回的errorlevel值不够稳定，经常会变换出其他不可预知的错误码，造成判断有误，导致批处理进入死循环。当然也有人采用能否往系统目录（比如system32）写入文件的方法来判断，我觉得这些都不够好，后来我发现使用bcdedit命令来判断比较准确，一般情况下它只会返回两个错误码0和1，很少见到有其他值出现，目前它是我觉得用来判断管理员权限最好的方法。

​       前3种方法是使用了VBS脚本，方法1最简洁，一行代码就搞定了，但我发现Win10系统似乎不支持短文件名，一旦文件路径出现空格，该命令就会出错；方法2也不赖，只有两行代码，在Win10系统能稳定运行；方法3比较复杂，还会产生临时文件，但也能在Win10系统稳定运行；第4种方法使用了第三方程序，不会产生临时文件，代码也简单，但你得先下载nircmd.exe这个工具；第5种方法采用了系统自带的命令runas来实现，理论上是没问题的，但实际上比较难实现，首先要求Administrator管理员账户必须已经启用，其次还必须设置有管理员密码，否则都会得到拒绝的提示。

​       以上5种方法都可以提升批处理的运行权限，可根据具体情况选择最合适的方法，我比较喜欢方法2，代码简洁，运行稳定，不会产生临时文件，其次方法4也不错。如果您有更好的批处理提权方法，也可以跟大家一起分享！共同进步！

 附：批处理获取管理员权限完整代码

~~~ bat
@echo off
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0
bcdedit >nul
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
cd /d "%~dp0"
echo 当前运行路径是：%CD%
echo 已获取管理员权限
pause
~~~

