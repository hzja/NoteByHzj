Window 系统信息
<a name="20h1M"></a>
### 一、本机信息
<a name="G22lf"></a>
#### 1、系统程序
收集系统信息，包括操作系统版本，已安装补丁列表等。通常使用以下方法收集：
```bash
net config workstation/server                                 # 查询简易系统信息
systeminfo                                                    # 查询全部内容
wmic qfe get Caption,Description,HotFixID,InstalledOn         # 查询已安装的补丁列表
```
注：其中systeminfo命令查询内容最全，但如果系统更新的补丁较多，可能会导致反应反应时间过长；使用 webshell 执行此命令可能会因为超时导致无法正常回显，或回显内容长度过长，无法全部显示。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598679265689-5f5384a6-9637-464f-b805-6ef18e6ef154.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2027627&status=done&style=none&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598679286253-1e5fb1bd-360d-41f1-88db-a04d6e2c0b41.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2097088&status=done&style=none&width=1107.6666666666667)
<a name="F1yFQ"></a>
#### 2、进程服务
```bash
tasklist /v                              # 查询正在运行的进程
wmic product get name,version            # 查询所有安装过的软件及版本
powershell "Get-WmiObject -class Win32_Product |Select-Object -Property name,version"
# 使用 powershell 查询所有安装过的软件及版本，效果和 wmic 相同
wmic service list brief                  # 查询当前机器的服务信息
wmic startup get command,caption         # 查看启动项
schtasks  /query  /fo  LIST /v           # 查看任务计划
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598679335791-b27e6a98-d821-41ef-a75a-f071a530a830.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2084646&status=done&style=none&width=1107.6666666666667)<br />windows 自带防火墙及特殊过滤规则等网络访问均可使用netsh及相关命令查看。
```bash
netsh firewall show config                      # 查看防火墙信息，但 firewall 命令已弃用，可使用 advfirewall 命令代替
netsh advfirewall firewall show rule name=all   # 查看配置规则
netsh advfirewall set allprofiles state off\on  # 关闭防火墙\开启防火墙
netsh advfirewall export\import xx.pol          # 导出\导入配置文件
netsh advfirewall firewall add rule name=”deny tcp 139″ dir=in protocol=tcp localport=139 action=block         # 新建规则阻止TCP协议139端口
netsh advfirewall firewall add rule name="Remote Desktop" protocol=TCP dir=in localport=3389 action=allow              # 新建规则允许3389通过防火墙
netsh advfirewall firewall delete rule name=Remote Desktop  # 删除名为Remote Desktop的规则
netsh interface                                 # 连接安全规则配置，很少配置。
```
<a name="UTimZ"></a>
#### 3、用户信息
```bash
whoami                                   # 当前用户
quser                                    # 查询登录用户，同 query user
qwinsta                                  # 查询登录用户，同 query user
query user                               # 查询登录用户
query session                            # 查询会话
query termserver                         # 查询远程桌面主机列表。
net accounts                             # 查询域密码策略
net user                                 # 查询本地用户列表
net user "$username"                     # 查询指定用户
net localgroup                           # 查询本地用户组列表
net localgroup "$groupname"              # 查询指定用户组成员
net group                                # 仅域控可执行，查询用户组列表
net group "$groupname"                   # 仅域控可执行，查询用户组成员
```
注：quser、qwinsta和query命令只存在于允许安装 RDP 服务的主机上，官方描述其仅存在于 server 2012 及以上版本存在。其中query termserver命令存在问题，本地测试时与描述严重不符，无法列出信息。
<a name="scr0e"></a>
#### 4、操作记录
cmd 和 powershell v3 以下的操作记录无法长时间保存，仅限当前窗口。
```bash
Get-History | Format-List -Property *         # 查询 powershell 当前窗口历史操作记录
Clear-History                                 # 删除 powershell 当前窗口历史操作记录
Clear-History -Id 3                           # 删除 powershell 当前窗口指定 ID 的历史操作记录
doskey /h                                     # 查看 cmd 的历史操作记录
doskey /reinstall                             # 删除 cmd 的历史操作记录
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598679428192-775fddc3-3414-4771-ba9a-abcfaba5eed7.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2062506&status=done&style=none&width=1107.6666666666667)<br />可以通过向进程发送键盘内容的方式将运行中的窗口历史操作记录导出。<br />powershell v5 以上的操作历史记录会直接保存在指定文件中。直接查看即可。
```bash
# 查看 powershell 历史操作记录
type %appdata%\Microsoft\Windows\PowerShell\PSReadline\ConsoleHost_history.txt
```
powershell v3、v4 版本需要安装Get-PSReadlineOption。
> msiexec /q /i PackageManagement_x64.msi

<a name="xtkIh"></a>
### 二、网络发现
内网网络发现是一个很重要的信息收集内容，一般情况下是不建议使用扫描器扫描，尤其不建议使用 nmap，当然，如果是靶场，或是甲方授权就另当别论。
<a name="xXemX"></a>
#### 1、基本信息收集
```bash
ipconfig /all                     # 列出当前主机详细网络信息
ipconfig  /displaydns             # 列出dns缓存信息
route print                       # 查询路由表
arp -a                            # 地址解析协议缓存表
netstat -ano                      # 端口使用情况
net share                         # 查看共享信息
net view                          # 查看共享资源列表
wmic share get name,path,status   # 查看共享信息
# 查看 host 文件
type  c:\Windows\system32\drivers\etc\hosts
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598679464272-0f13616f-82ce-4f5a-bb8f-3a4e7577c7c4.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2038953&status=done&style=none&width=1107.6666666666667)<br />使用 arp 命令收集最近建立过连接的 ip，但是此方法在某些时候无法收集到网段外的 ip 地址。
<a name="i2dIo"></a>
#### 2、SPN 信息收集
SPN：服务主体名称。使用 Kerberos 须为服务器注册 SPN，因此可以在内网中扫描 SPN，快速寻找内网中注册的服务，SPN 扫描可以规避像端口扫描的不确定性探测动作。<br />主要利用工具有：setspn、GetUserSPNs.vbs和Rubeus。<br />利用 Windows 自带的 setspn 工具<br />普通域用户权限执行即可
> setspn -T domain.com -Q /

利用 GetUserSPNs.vbs<br />使用 Rubeus 工具
<a name="ceCtv"></a>
#### 3、协议探测
<a name="tWktz"></a>
##### icmp 协议
直接回显
> for /l %i in (1,1,255) do @ ping 10.0.0.%i -w 1 -n 1|find /i "ttl="

输出文件
> @for /l %i in (1,1,255) do @ping -n 1 -w 40 192.168.0.%i & if errorlevel 1 (echo 192.168.0.%i >> ip.txt) else (echo 192.168.0.%i >> ip.txt)

<a name="Us8UB"></a>
##### Netbios 协议
msf 扫描
> msf > use auxiliary/scanner/netbios/nbname

nbtscan 扫描，下载：
> [http://www.unixwiz.net/tools/nbtscan.html#download](http://www.unixwiz.net/tools/nbtscan.html#download)

参数：
> -m 192.168.1.0/24

<a name="NYljO"></a>
##### UDP 协议
msf
> msf > use auxiliary/scanner/discovery/udp_probemsf > use auxiliary/scanner/discovery/udp_sweep

smb-version<br />python版：<br />[https://github.com/amitn322/smb-version](https://github.com/amitn322/smb-version)<br />c#版：<br />[https://www.zcgonvh.com/post/CSharp_smb_version_Detection.html](https://www.zcgonvh.com/post/CSharp_smb_version_Detection.html)
<a name="QxAgJ"></a>
##### SNMP 协议
msf
> msf > use auxiliary/scanner/snmp/snmp_enum

<a name="c1MdR"></a>
#### 4、系统日志
可以使用 wevtutil.exe psloglist.exe 或者 powershell 的Get-WinEvent模块进行日志导出，server 03 的域控可使用 eventquery.vbs 导出。<br />wevtutil 不需要 UAC， 支持很多过滤语法，若有需要请查看官方说明。
```bash
# 查询登录日志语法
wevtutil qe security /f:text /q:*[System[(EventID=4624)]]
# 查询所有登录、注销相关的日志语法
wevtutil qe security /rd:true /f:text /q:"*[system/eventid=4624 and 4623 and 4627]"
# 远程查询时在后面加入/r:computer /u:user /p:password 比如查询dc1上的登录注销日志
wevtutil qe security /rd:true /f:text /q:"*[system/eventid=4624 and 4623 and 4672]" /r:dc1 /u:administrator /p:password
```
本地使用 LogParser 日志分析工具整理导出的日志，然后去除重复数据、无效数据（以 '$' 结束的用户名）
> LogParser.exe -i:EVT -o txt "SELECT TO_UPPERCASE(EXTRACT_TOKEN(Strings,5,'|')) as USERNAME,TO_UPPERCASE(EXTRACT_TOKEN(Strings,18,'|')) as SOURCE_IP FROM C:\ProgramData\log.evtx" > C:\ProgramData\log.txt

导出域控登录日志到域控上:
> wevtutil epl Security C:\ProgramData\dc.evtx /q:"*[EventData[Data[@Name='LogonType']='3'] and System[(EventID=4624) and TimeCreated[timediff(@SystemTime) <= 2592000000]]]" /r:域控IP /u:域管 /p:域管密码

<a name="rTQ7e"></a>
#### 5、浏览器日志
收集浏览器访问记录。<br />chrome
> C:\Users$username\AppData\Local\Google\Chrome\User Data\Default\History

firefox
> C:\Users$username\AppData\Roaming\Mozilla\Firefox\Profiles$name.default\places.sqlite

IE
> reg query "HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\TypedURLs" 或C:\Users$user\AppData\Local\Microsoft\Windows\History

edge, v79+:
> C:\Users$user\AppData\Local\Microsoft\Edge\User Data\Default\History

v44+
> C:\Users$user\AppData\Local\Microsoft\Windows\WebCache\WebCacheV01.dat

浏览器的历史记录文件 history，是使用 sqlite 格式记录的，直接打开是一个二进制文件，可以下载到本地破解，或者直接替换查看。<br />需要注意的是，edge v44+ 版本的历史记录文件在用户登录状态下是锁定的。
<a name="FXdlX"></a>
#### 6、DNS 服务器记录
如果目标机器在域内，只需要按部就班收集域信息，准备域横向渗透即可。<br />但其中有一点单独提一下，一般在域环境中都会有一台 DNS 服务器，小企业或内网环境较小的可能会和域控为同一台，大一些的企业，多为独立的 DNS 服务器。<br />由于 DNS 服务器的特性，因此，在 DNS 服务器上会存在大量内网地址解析记录，如果可以获取，会极大的方便内网拓扑展开，同时很多其他主机无法到达的地址，DNS 服务器是可以到达的。<br />列出 DNS 区域中当前节点的资源记录
> dnscmd . /EnumZones

列出 test.com 的信息
> dnscmd . /ZoneInfo test.com

列出 test.com 中的详细记录
> dnscmd . /ZonePrint test.com

列出 test.com 中的记录
> dnscmd . /EnumRecords test.com

DNS 记录有正向解析和反向解析两种。<br />正向解析是从域名到IP，反向解析则是从 IP 到域名。<br />反向隔离和正向隔离相同，都会有一个对应的区域，因此，查询方式与正向记录相同。<br />DNS 服务器还存在域传送漏洞。可以直接获得 DNS 记录。
> nslookup -type=all_ldap._tcp.dc.greyfinger.com
> nslookup -type=ns domain.com
> cmd> nslookup server dns.domain.com ls domain.com

<a name="QjYpn"></a>
### 三、域信息查询
<a name="9rsJz"></a>
#### 1、基本信息收集
域信息收集，需要当前用户为域用户。本地用户无法执行域命令查询域内容。
```bash
net config workstation                    # 查询当前登录域
net time                                  # 同步时间，通常为域控服务器
net accounts /domain                      # 查询域密码策略
net user /domain                          # 列出当前域成员列表
net user username /domain                 # 列出域成员详细信息
net group /domain                         # 列出域内组列表
net group groupname /domain               # 列出域内组成员列表
net view                                  # 查询同一域内机器列表
net view /domain                          # 查询域列表
net view /domain:test                     # 查询 test 域中计算机列表
nltest /dclist:domain                     # 查询域内的所有DC
nltest /dsgetdc:domain                    # 拿到DC当前的认证信息
nltest /domain_trusts                     # 查询域信任信息
nltest /user:"username"                   # 得到用户信息
```
想要定位域管和域控机器，可以查询domain admin和domain controllers组，需要注意的是，组名会随系统语言变化而变化，如domain controllers会变化成domaine contrôleurs。<br />在域内其他机器上，查询组是net group /domain，而在域控上则是net group。
<a name="vF3zI"></a>
#### 2、dsquery
dsquery 命令很少使用，而且限制较大仅能在域控上执行，所以相对而言较为鸡肋，但是在一定条件下还是具有一定的使用价值。
```bash
dsquery computer                          # 查询目录中的计算机
dsquery contact                           # 查询目录中的联系人
dsquery subnet                            # 查询目录中的子网
dsquery group                             # 查询目录中的组
dsquery site                              # 查询目录中的站点
dsquery user                              # 查询目录中的用户
```
3、powershell
```bash
# 查询当前Domain信息
[System.DirectoryServices.ActiveDirectory.Domain]::GetCurrentDomain()
# 查询域信任关系
([System.DirectoryServices.ActiveDirectory.Domain]::GetCurrentDomain()).GetAllTrustRelationships()
# 查询当前林信息
[System.DirectoryServices.ActiveDirectory.Forest]::GetCurrentForest()
# 查询林信任信息
([System.DirectoryServices.ActiveDirectory.Forest]::GetForest((New-Object System.DirectoryServices.ActiveDirectory.DirectoryContext('Forest', 'forest-of-interest.local')))).GetAllTrustRelationships()
```
<a name="E4kt0"></a>
### 四、配置文件收集
<a name="u9762"></a>
#### 配置文件
IIS 配置文件路径为：
> %windir%\system32\inetsrv\config\applicationHost.config

使用 appcmd 的方式可以快速导出所需内容：
> %windir%\system32\inetsrv\appcmd list site /config %windir%\system32\inetsrv\appcmd list site /config /xml > c:\sites.xml

<a name="novm7"></a>
#### 密码保存
<a name="NLE5V"></a>
##### navicat
| **数据库** | **路径** |
| :---: | :---: |
| MySQL | HKEYCURRENTUSER\\Software\\PremiumSoft\\Navicat\\Servers\\ |
| MariaDB | HKEYCURRENTUSER\\Software\\PremiumSoft\\NavicatMARIADB\\Servers\\ |
| MongoDB | HKEYCURRENTUSER\\Software\\PremiumSoft\\NavicatMONGODB\\Servers\\ |
| Microsoft SQL | HKEYCURRENTUSER\\Software\\PremiumSoft\\NavicatMSSQL\\Servers\\ |
| Oracle | HKEYCURRENTUSER\\Software\\PremiumSoft\\NavicatOra\\Servers\\ |
| PostgreSQL | HKEYCURRENTUSER\\Software\\PremiumSoft\\NavicatPG\\Servers\\ |
| SQLite | HKEYCURRENTUSER\\Software\\PremiumSoft\\NavicatSQLite\\Servers\\ |

<a name="OYqCp"></a>
##### SecureCRT
| **系统版本** | **路径** |
| :---: | :---: |
| xp/win2003 | C:\\Documents and Settings\\USERNAME\\Application Data\\VanDyke\\Config\\Sessions |
| win7/win2008以上 | C:\\Users\\USERNAME\\AppData\\Roaming\\VanDyke\\Config\\Sessions |

<a name="HL0dB"></a>
##### Xshell
| **版本** | **路径** |
| :---: | :---: |
| Xshell 5 | %userprofile%\\Documents\\NetSarang\\Xshell\\Sessions |
| Xshell 6 | %userprofile%\\Documents\\NetSarang Computer\\6\\Xshell\\Sessions |

<a name="OiN5C"></a>
##### WinSCP
> HKCU\Software\Martin Prikryl\WinSCP 2\Sessions

<a name="YAyAa"></a>
##### VNC:
| **版本** | **路径** | **类型** |
| :---: | :---: | :---: |
| RealVNC | HKEYLOCALMACHINE\\SOFTWARE\\RealVNC\\vncserver | Password |
| TightVNC | HKEYCURRENTUSER\\Software\\TightVNC\\Server Value | Password or PasswordViewOnly |
| TigerVNC | HKEYLOCALUSER\\Software\\TigerVNC\\WinVNC4 | Password |
| UltraVNC | C:\\Program Files\\UltraVNC\\ultravnc.ini | passwd or passwd2 |


