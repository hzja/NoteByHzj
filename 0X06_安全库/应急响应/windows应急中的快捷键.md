应急的时候，快捷键很重要，记录一下windows主机排查需要用到的快捷键
<a name="m85nZ"></a>
## windows快捷键
<a name="QZJn7"></a>
### appwiz.cpl 是打开安装面板  
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651737613109-0090da94-c09c-423b-8783-e45fee8a2662.png#clientId=u17a24c9e-bdad-4&from=paste&height=437&id=u5fcccb49&originHeight=546&originWidth=722&originalType=binary&ratio=1&rotation=0&showTitle=false&size=309604&status=done&style=none&taskId=uc33c2cbf-4526-4cbb-bf28-030483afffa&title=&width=577.6)

<a name="FvqKA"></a>
### msconfig是打开启动项
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651737860276-eb6b5c33-7871-4965-93c3-196822ad8efa.png#clientId=u17a24c9e-bdad-4&from=paste&height=288&id=u8616cd8d&originHeight=360&originWidth=531&originalType=binary&ratio=1&rotation=0&showTitle=false&size=126954&status=done&style=none&taskId=udc46ce79-cddc-4718-b0e6-e90906fb113&title=&width=424.8)
<a name="QgCdB"></a>
### eventvwr.msc事件查看器
Win+R打开运行，输入“eventvwr.msc”，回车运行，打开“事件查看器”。  比如常见的安全事件ID,_4624_代表登录成功、4625代表登录失败、4634代表用户注销<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651765992458-471295d3-f882-4b6c-a90d-fe2dbe435ad7.png#clientId=u49784d14-75ff-4&from=paste&height=495&id=ufd966530&originHeight=619&originWidth=1001&originalType=binary&ratio=1&rotation=0&showTitle=false&size=160564&status=done&style=none&taskId=ue8070dfc-b5a9-422a-9f48-ea190681b1c&title=&width=800.8)
<a name="LAhQW"></a>
### gpedit.msc 本地组策略
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651765368554-2cc5c00f-b02c-432a-a771-bbd92c540de1.png#clientId=u49784d14-75ff-4&from=paste&height=537&id=u847aa56a&originHeight=671&originWidth=960&originalType=binary&ratio=1&rotation=0&showTitle=false&size=175844&status=done&style=none&taskId=ubce124cc-3553-44e7-ba8d-ccc6105f031&title=&width=768)
<a name="WPHrK"></a>
### secpol.msc本地安全策略
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651766189190-2ddf6ee0-6729-4ac5-a449-54cf07e2c40e.png#clientId=u49784d14-75ff-4&from=paste&height=572&id=ue4c11818&originHeight=715&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=191929&status=done&style=none&taskId=u001bf9d9-7bbd-45b0-9e2c-96d4f7996a9&title=&width=800)
<a name="PjkX3"></a>
### services.msc服务自启动
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651765952352-1f2ba52f-e99f-4889-b6ea-16dd73fca1bf.png#clientId=u49784d14-75ff-4&from=paste&height=778&id=u736deefd&originHeight=972&originWidth=1304&originalType=binary&ratio=1&rotation=0&showTitle=false&size=624776&status=done&style=none&taskId=u9951809d-74f7-4003-96fd-b73ab4094af&title=&width=1043.2)

同时自启动还有以下路径<br />C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup<br />C:\Users\System\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
<a name="JswxD"></a>
###  regedit   是打开注册表
检查右侧是否有启动异常的项 目，如有请删除，并建议安装杀毒软件进行病毒查杀，清除残留病毒或木马。  <br />HKEY_CURRENT_USER\software\micorsoft\windows\currentversion\run HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Runonce 

<a name="oGK0J"></a>
#### 注册表查看隐藏用户
需要先开启权限，然后重新进入注册表<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651738082253-b3c8ce6e-829c-46fc-a281-340449e2a120.png#clientId=u17a24c9e-bdad-4&from=paste&height=446&id=necEN&originHeight=557&originWidth=743&originalType=binary&ratio=1&rotation=0&showTitle=false&size=377051&status=done&style=none&taskId=uc9bcd7eb-e1ab-4d8a-97df-34ebf2929a7&title=&width=594.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651736366511-9ce61087-1db4-4a71-9717-6b5e47398c43.png#clientId=u17a24c9e-bdad-4&from=paste&height=1058&id=RkZod&originHeight=1322&originWidth=1722&originalType=binary&ratio=1&rotation=0&showTitle=false&size=948615&status=done&style=none&taskId=u2f67b9b2-eaef-41c4-97d9-37884fbd173&title=&width=1377.6)
<a name="IRTt2"></a>
#### 通过注册表查看登陆过的服务器
HKEY_CURRENT_USER\SOFTWARE\Microsoft\Terminal Server Client\Servers 注册表可以查看服务器登陆过哪台服务器<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651740123651-20948a93-02b6-46e0-96c6-c5d976bdce6b.png#clientId=u17a24c9e-bdad-4&from=paste&height=603&id=u9f35b21d&originHeight=754&originWidth=758&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75410&status=done&style=none&taskId=ubdbe6124-4d3a-4ddb-a0cc-485fbcd15bc&title=&width=606.4)

<a name="N6Gc7"></a>
#### 查看攻击者运行过的程序
计算机\HKEY_CURRENT_USER\SOFTWARE\Classes\Local Settings\Software\Microsoft\Windows\Shell\MuiCache

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1651740234860-500e9815-43ed-4de8-a337-a9f31d7ca076.png#clientId=u17a24c9e-bdad-4&from=paste&height=598&id=u0148ad49&originHeight=747&originWidth=1105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=150322&status=done&style=none&taskId=udc4c2449-56be-45fa-b4f6-963ad08856f&title=&width=884)
<a name="dbc9G"></a>
### taskschd.msc  打开任务计划程序
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1652627490246-609fb4c8-0c54-47f7-9643-7b3ef5eed505.png#clientId=ud0ace94c-8f54-4&from=paste&height=441&id=u1d6cd6b0&originHeight=551&originWidth=1088&originalType=binary&ratio=1&rotation=0&showTitle=false&size=109538&status=done&style=none&taskId=u1961aca8-a558-43ce-9cba-9e285eab7a8&title=&width=870.4)

<a name="EsoZ2"></a>
## 直观查看进程外联
process hacker可以查看所有进程的外联<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640879119228-be58e9d4-ed2a-4f01-a173-0375d25b83bc.png#clientId=ua5da9900-4a61-4&from=paste&height=362&id=aKZLT&originHeight=724&originWidth=1189&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78898&status=done&style=none&taskId=u0ab12565-9128-4e0e-8a91-cf4408d33f6&title=&width=594.5)<br />powertool也可以通过查看外联，定位到资源位置卸载流氓软件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640879365247-c44c8edc-2f66-4725-95df-767bd4f27aac.png#clientId=ua5da9900-4a61-4&from=paste&height=425&id=pQOpa&originHeight=691&originWidth=874&originalType=binary&ratio=1&rotation=0&showTitle=false&size=95664&status=done&style=none&taskId=u7d0f1e13-08fb-4756-bf2f-4021b20eacb&title=&width=538)<br />记录一下tools上luckyeast师傅发的一个脑图<br />![tools.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620181715079-f2b83cd7-fd3c-49ec-9bc6-e8733ab47364.png#height=1904&id=xDNOw&originHeight=1904&originWidth=2420&originalType=binary&ratio=1&rotation=0&showTitle=false&size=793811&status=done&style=none&title=&width=2420)


Wireshark抓包后保存为pcap网卡，然后筛选出centos.uno域名<br />tshark -r 789.pcap -Y dns|findstr "centos.uno"<br /> <br />清楚过过滤规则、 file→capture Events、process Mon 先抓进程、再dnsSQuery 开始捕捉DNS请求，抓到后停止所有捕抓，Filter设置过滤规则、有进程id 去process<br /> <br /> <br />awk -F ' ' '{print $1}' > 1.txt<br /> <br />1.恶意域名-》端口（进程）-》恶意进程->存储位置（缺失）-》kill<br />2.恶意域名-》端口（进程）-》系统进程-》分析进程所关联的服务（模块）-》判断签名（逆向分析/病毒分析）-》<br /> 
<a name="160f1471"></a>
## Logparser使用
 <br />[https://www.cnblogs.com/luoyong0201/p/Dynamics_365_Log_Parser.html](https://www.cnblogs.com/luoyong0201/p/Dynamics_365_Log_Parser.html)<br /> <br />LogParser.exe -i:EVT -o:DATAGRID "SELECT TimeGenerated as LoginTime,EXTRACT_TOKEN(Strings,5,'|') as username,EXTRACT_TOKEN(Strings, 8, '|') as LogonType,EXTRACT_TOKEN(Strings, 17, '|') AS ProcessName,EXTRACT_TOKEN(Strings, 18, '|') AS SourceIP FROM 日志位置 where EventID=4624"<br /> <br />LogParser.exe -i:EVT -o:csv "SELECT TimeGenerated as LoginTime,EXTRACT_TOKEN(Strings,5,'|') as username,EXTRACT_TOKEN(Strings, 8, '|') as LogonType,EXTRACT_TOKEN(Strings, 17, '|') AS ProcessName,EXTRACT_TOKEN(Strings, 18, '|') AS SourceIP FROM 日志位置  where EventID=4624 " > c:/1.csv<br /> 
<a name="ffaf9284"></a>
## 后门判断

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620182070152-51272a0a-29cd-4439-874a-09a8dbcd1907.png#height=429&id=dGWpc&originHeight=571&originWidth=446&originalType=binary&ratio=1&rotation=0&showTitle=false&size=103453&status=done&style=none&title=&width=335)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620182078369-2baa240b-3e7a-475d-8727-cb01f6015702.png#height=441&id=PfwPS&originHeight=587&originWidth=461&originalType=binary&ratio=1&rotation=0&showTitle=false&size=104210&status=done&style=none&title=&width=346)

<a name="H7A6y"></a>
## 关于勒索病毒
碰到勒索病毒， 需要和客户人员确认该系统有什么业务，例如该oa是内网还是外网，是商业oa还是自己开发的oa，确认下服务是不是对外开放，如果不是对外开放，可能就是内网传播的。

<a name="cGK9z"></a>
## windows日志查看
[https://mp.weixin.qq.com/s/-aBO3xVqLYPEbkcIgLgkCA](https://mp.weixin.qq.com/s/-aBO3xVqLYPEbkcIgLgkCA)<br />[https://mp.weixin.qq.com/s/Zpca-OH5HSb8UaUdXEGSWg](https://mp.weixin.qq.com/s/Zpca-OH5HSb8UaUdXEGSWg)

在开始菜单输入msconfig点击启动项栏目，查看是否有异常启动进程。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629558548168-5f583ddd-8606-4f11-a221-b2408ce33dde.png#height=437&id=EQFoT&originHeight=583&originWidth=868&originalType=binary&ratio=1&rotation=0&showTitle=false&size=81306&status=done&style=none&title=&width=651)

<a name="PL6wD"></a>
## powertool工具
可以查看所有启动项的内容<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639446342632-26ecdf7b-6b37-44c1-bab7-882dc987a923.png#clientId=u9fc3581e-8a93-4&from=paste&height=492&id=ud67b2b3a&originHeight=983&originWidth=1777&originalType=binary&ratio=1&rotation=0&showTitle=false&size=285288&status=done&style=none&taskId=ub1c4d15b-b213-404c-b648-41dc32bf4c0&title=&width=888.5)
<a name="Mm4gd"></a>
## windows的所有启动项
可以用powertool进行注册表定位
```go
%appdata%\Microsoft\Windows\Start Menu\Programs\Startup
C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup 

%appdata%\Microsoft\Windows\Start Menu\Programs\Startup
C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup 
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce
HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run
HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\RunOnce
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnceEx\0001 HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnceEx\0001\Depend

```

<a name="LnRQu"></a>
## windows隐藏文件
使用这个命令就看不到文件了<br />attrib +s +a +h +r D:\test\project\test.txt

如果应急需要排查，可以勾掉下面的推荐框，就可以看到隐藏的文件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639448142776-35038ec0-c68a-4d30-965f-d63dcf1f0f27.png#clientId=u9fc3581e-8a93-4&from=paste&height=484&id=ua48e14e0&originHeight=726&originWidth=588&originalType=binary&ratio=1&rotation=0&showTitle=false&size=66445&status=done&style=none&taskId=u7ab54487-36e4-468e-b307-f431d81a323&title=&width=392)


<a name="OR5Qc"></a>
## cs进程注入排查
首先查看外联，看看哪个系统服务有外联的情况<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1665801605878-2067f515-9992-4641-bc53-4d0011448676.png#clientId=uffd83054-3c81-4&from=paste&height=378&id=u0c45f4e5&originHeight=472&originWidth=1486&originalType=binary&ratio=1&rotation=0&showTitle=false&size=277428&status=done&style=none&taskId=uf9d9505c-f205-44a6-8957-281735ff6b1&title=&width=1188.8)

然后根据对应的进程id去查看对应服务的dll文件，看是不是有非系统文件<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1665801791729-264c6568-8770-4c78-b843-1bbc8b765f5b.png#clientId=uffd83054-3c81-4&from=paste&height=784&id=uf963166f&originHeight=980&originWidth=1799&originalType=binary&ratio=1&rotation=0&showTitle=false&size=606215&status=done&style=none&taskId=u2c0a3378-e145-4ca9-aff7-3d9c8ab5985&title=&width=1439.2)
