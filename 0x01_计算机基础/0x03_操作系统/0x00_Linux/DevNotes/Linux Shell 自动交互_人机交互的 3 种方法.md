Linux Shell 
<a name="23OhQ"></a>
### 一、背景
Shell脚本在处理自动循环或大的任务方面可节省大量的时间，通过创建一个处理任务的命令清单，使用变量、条件、算术和循环等方法快速创建脚本以完成相应工作，这比在命令行下一个个敲入命令要省时省力得多。<br />但是有时候可能会需要实现和交互程序如ftp,telnet服务器等进行交互的功能，这时候需要用到shell的自动交互功能。
<a name="rd3lU"></a>
### 二、需求
**需求1：**从一台Linux机器ftp登陆到另一台Linux机器，进行系列操作后关闭，避免每次都手动输入密码。<br />**需求2：**改变登录用户密码，避免每次都输入新旧密码。<br />**需求3：**希望su自动登录到root账户，避免每次都输入root密码。
<a name="x7c67"></a>
### 三、调试环境
终端：SecureCRT<br />系统：WinXP, CentOS 4.4(VmWare)<br />Shell:  bash
:::tips
注：shell有很多种，B类SHELL（sh, bash, ksh）之间行为相近；C类SHELL（csh, tcsh）之间行为相近，还有zsh和rc等shell，当前使用的调试环境是bash。
:::
<a name="0TEXH"></a>
#### 自动交互方法一
自动交互最关键的就是交互信息的自动输入，首先联想到文件重定向，在shell编程中有这样一种用法(参考Linux与UNIX SHELL编程指南 chapt 5.7)："`command << delimiter`  从标准输入中读入，直至遇到delimiter分界符。"<br />重定向操作符`command << delimiter`是一种非常有用的命令，shell将分界符delimiter之后直至下一个同样的分界符之前的所有内容都作为输入，遇到下一个分界符， shell就知道输入结束了。最常见的delimiter分界符是EOF，当然完全可以自定为其他字符。<br />对于**需求1**要求的自动登陆ftp，并作系列操作，则可以用这种方法进行自动交互。代码如下：
```bash
#!/bin/bash
ftp -i -n 192.168.167.187 << EOF
user hzc 123456
pwd
cd test
pwd
close
bye
EOF
```
测试可以发现，如上代码使用帐号名hzc，密码123456成功登陆了ftp服务器，并进入目录，打印出了pwd。
<a name="AI4hF"></a>
#### 自动交互方法二
**需求2**中要求采用非交互的方式改变登录用户密码，尝试用方法1，无法实现。<br />这时候联想到交互信息的另一个自动输入方法，管道，通过`echo + sleep + |` 可以实现这个需求。
```bash
#!/bin/bash
(echo "curpassword"
sleep 1
echo "newpassword"
sleep 1
echo "newpassword")|passwd
```
测试通过，运行这个脚本，直接把当前用户的 curpassword 改成 newpassword。
<a name="qgoqh"></a>
#### 自动交互方法三
需求3中要求自动登录root账号，尝试方法1和方法2，都出现错误提示standard in must be a tty。<br />这时候尝试寻找外部帮助，一个shell工具`expect`可以实现这个功能，其实expect就是一个专门用来实现自动交互功能的工具，`expect`的语法可以参考相关资料，代码如下：
```bash
#!/usr/bin/expect
spawn su root
expect "password: "
send "123456\r"
expect eof
exit
```
测试通过，运行这个脚本，直接从当前用户登录到root用户。
<a name="ge52U"></a>
### 四、方法总结
方法一(重定向)简单直观，也经常有实际应用，但是在自动交互领域功能有限。<br />方法二(管道)也很简单直观，有时甚至不用`sleep`配合就能展现强大的自动交互实力，但是在某些时候也束手无策。<br />方法三(`expect`)在功能上是最为强大的，`expect`本来就是为实现自动交互功能而生，但是缺点是需要安装`expect`包，在嵌入式等环境下难以安装。<br />三个方法各有优劣，应用的好，都可以完成Linux Shell自动交互。
