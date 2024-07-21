# 原创
：  [译] APT分析报告：03.OpBlueRaven揭露APT组织Fin7/Carbanak（上）Tirion恶意软件

# [译] APT分析报告：03.OpBlueRaven揭露APT组织Fin7/Carbanak（上）Tirion恶意软件

这是作者新开的一个专栏，主要翻译国外知名的安全厂商APT报告文章，了解它们的安全技术，学习它们溯源APT组织的方法，希望对您有所帮助。前文分享了钓鱼邮件网址混淆URL逃避检测，这篇文章将介绍APT组织Fin7 / Carbanak的Tirion恶意软件，包括OpBlueRaven行动。

#### 文章目录

---


## 一.Fin7简介

FIN7组织的攻击目标是金融机构（尤其美国金融公司），以钓鱼邮件为主要攻击渠道。常见攻击手法包括：

常规攻击流程如下图所示，攻击者以钓鱼邮件为进入渠道，在恶意文档中嵌入vbs脚本，vbs脚本运行后解密后门程序写入注册表中，同时将调用后门程序的脚本以ads隐藏在磁盘文件中。在后门运行后，使用DNS TXT做为C&amp;C通信方式。

在邮件附件文档中，使用了恶意宏代码，不同钓鱼文档的界面大体如下，值的一提的是使用汉堡图片做为诱饵文档时的攻击目标是一家国外的食品公司。遭到FIN7入侵的公司包括Chipotle、Chilli’s和Arby’s等知名品牌，该组织被认为已入侵数千个业务据点，并且已窃取数百万个信用卡号码。

随着技术深入，该组织部署了新的战术（**类似于BadUSB**）。Securityaffairs网站发现FIN7通过美国邮政服务（USPS）给目标企业的人力资源、信息技术或执行管理部门的员工邮寄包裹，包含USB设备、礼品卡等。当员工将USB设备插入计算机时，会注入命令以下载并执行以GRIFFON跟踪的JavaScript后门。

这样的包裹被发送给多家企业，包括零售业、餐饮、酒店。武器化的USB设备模仿用户击键特征，启动PowerShell命令从远程服务器检索恶意软件。专家们观察到恶意代码联络域名与IP地址位于俄罗斯。

该USB设备使用Arduino微控制器ATMEGA32U4，并编程模拟USB键盘。由于PC默认情况下信任键盘USB设备，一旦插入，键盘模拟器就会自动插入恶意命令。然后Powershell脚本运行第三阶段JavaScript，收集系统信息并删除其他恶意软件。根据FBI的警告，一旦收集到目标的信息，FIN7组织就开始横向移动以获取管理权限。在收集到的信息发送到C&amp;C服务器之后。主JS代码会进入一个无限循环，在每个循环迭代中睡眠2分钟，然后从命令和控件获取一个新命令。

总之，一旦USB控制器芯片被重新编程用于其他用途（如模拟USB键盘），这些设备就可以被用来发动攻击，并在用户不知情的情况下感染他们的计算机。再加上这些设备非常便宜，任何人都可以随时使用，这也意味着攻击者更深入地利用这些技术和设备只是时间问题。

**参考资料：**

---


## 二.关于Fin7和Carbanak

<mark>本文旨在为读者提供有关PRODAFT（瑞士）＆INVICTUS（欧洲）威胁情报（PTI）团队针对不同威胁参与者的最新操作的详细信息，并发现与臭名昭著的Fin7 APT组织相关。在文章中，所有信息都源自威胁参与者的一次OPSEC故障，我们将尝试逐步扩展主题，在不断发现的基础上扩大范围。</mark>

在2020年5月至7月之间，PRODAFT威胁情报团队的四名成员进行了BlueRaven行动。案例研究源于发现一组看似不重要的轻微OpSec故障。当然，后来发现这些威胁因素与臭名昭著的Fin7 / Carbanak威胁因素有关联。

PTI的OP（PTI’s OP）源于攻击者一方的OPSEC故障。与先前发现和发布的数据不同，使此OP如此与众不同的是，我们设法发现了大量有关攻击者工具集的未发布信息，这些信息揭示了攻击者的TTP。

Carbanak Group / Fin7于2014年首次被发现，是世界上最著名的APT组之一，并且是最早曝光的APT组之一。该组织被认为在全球范围内造成超过9亿美元的损失。我们的OP结果发现了有关这些威胁参与者的以下关键信息：

撰写此报告旨在提高认识并协助网络安全专家进行分析。当然，PRODAFT的一些发现已被删除。因此，授权机构可以与PRODAFT或INVICTUS联系以进行进一步的披露。

每篇文章都将讨论操作的特定方面，不仅仅包括攻击方法、组织和攻击者的身份。我们的团队还设法窃听攻击者之间的各种对话，这些对话中的大多数也将在整个系列中发布。

---


## 三.Carbanak后门

<mark>Carbanak Backdoor是我们小组获得的第一批发现之一。当前版本的Carbanak后门程序（该团队中最知名的工具，即Carbanak组的名字）是引起我们团队关注的第一个工具。</mark> 根据PE文件标题在2019年11月编译的“3.7.5”版本是后门命令和控制服务器最新检测到的版本。下面的屏幕快照提供了“3.7.5”版Carbanak后门管理面板的屏幕截图。

我们将获得的最新版本与2017年Virustotal中的“命令管理器”版本进行了比较，并对此工具进行了评估。下图反映了上述两个版本反编译获得的源代码之间的差异。图中仅列出了两个版本之间的源代码，左列属于2017年上传到Virustotal的文件，右列属于我们团队获得的“3.7.5”版本。蓝线表示不同的文件，而绿线表示新文件。

通过对命令和控制服务器软件的检查，可以看出，GUI界面对插件进行了基本更改，以创建更详细的错误日志，并添加了新的语言编码。

确定了2019年编译的6个版本的恶意软件“命令管理器”工具。下图给出了检测到的版本的时间戳。

在旧版本的Bot.dll中，它是在受害设备上运行的恶意软件的组件，在反汇编中检测到981个函数，而在同一软件的新版本中检测到706个函数。使用Diaphora二进制比较工具，有607个函数获得最佳匹配分数，而有43个函数获得部分匹配。此外，与Virustotal中的旧版本相比，新的bot文件的文件大小小于50kb。在检查新的bot文件时，可以看到，旧版本中除基本函数以外的函数都是作为插件实现的。<mark>这些新插件可以执行诸如按键记录、过程监视之类的操作，并且可以通过反射加载方法无文件执行。</mark> 结果，恶意软件的文件大小减小了，从而为鉴识基于签名的安全软件解决方案留下了更少的痕迹。

在本小节中，将检查先前发现的文件中“not”的一些插件。由于这些是工具包中前所未有的函数，因此，我们认为以下内容对于进一步分析该组织的TTP至关重要。

---


### 1.键盘记录插件

“keylog.dll”插件使用RegisterRawInputDevices API捕获用户击键。为了确定在哪个上下文中使用了击键，前台进程的“可执行文件路径（Executable File Path）”、“ Windows文本（Windows Text）”和时间戳（Timestamp）信息将与击键一起记录下来。

**键盘记录插件使用Windows GDI + API将收集的数据转换为Bitmap，并将其写入用户％TEMP％目录，名为“ SA45E91.tmp”的文件夹中。下图显示了恶意软件用来存储数据的函数。**

下图给出了所获得日志示例的屏幕截图。

---


### 2.过程监控器插件

该插件可以跟踪在目标系统中运行的进程，并用于获取有关所需进程的开始和终止时间的信息。下图给出了收集有关正在运行进程的信息的函数屏幕快照。

---


## 四.Tirion Loader（Carbanak后门的未来）

<font color="red">**Fin7组织的新加载程序工具是名为Tirion的恶意软件，该软件被认为是替代Carbanak后门程序而开发的。它包含许多功能，用于信息收集、代码执行、侦察和横向移动。与上一节中检查过的最新版本的Carbanak后门程序一样，该恶意软件执行的许多功能已作为独立的插件开发，使用反射性加载方法将它们加载到目标系统中并以无文件方式执行。公开数据显示，Carbanak后门的开发目前已停止，并且同一团队正在Tirion Loader上进行开发和测试。攻击者之间的通信日志表明，此新工具旨在替代Carbanak后门。**</font>

Tirion恶意软件的功能如下：

检测到的最新Tirion Loader版本属于“2020年6月28日23:24:03”编译的版本“1.6.4”。下图显示了攻击者可以在机器人设备上执行的操作。“1.0”版本是最早检测到的版本，被认为是最早使用的版本，已在“2020年3月5日20:29:53”上进行了编译。

攻击者编写的“readme.txt”文件中清楚地说明了恶意软件的基本组件。

相关翻译如下，该系统包含3个组件：

这些组件的相关性如下，加载器定期连接到服务器，客户端通过永久连接连接到服务器。加载器执行来自服务器的命令并向服务器发送响应。通过客户端，用户通过服务器向加载器发出命令；接收到来自加载器的响应后，服务器将其传输到cl。

---


### 1.文件结构

恶意软件的文件组织如下：

---


### 2.Readme.txt

“readme.txt”文件中某些重要项目的英文翻译说明如下，该文件指示从恶意软件的第一版本到“1.6.3”版本的更改，并包含构建说明。（省略俄语原文）

---


### 3.Loader组件

该恶意软件的这一组件将在受害系统上运行，大小约为9kb，并从服务器运行命令。当攻击者想要在受害者的设备上运行某个功能时，包含该功能的相关插件文件会以反射方式加载到受害者的设备上，并以无文件方式执行。

服务器和加载器之间的网络流量使用在构建阶段确定的密钥进行加密。下图包含相关的加密算法。

---


### 4.PswInfoGrabber

它是一个DLL文件，负责从目标系统中窃取和报告敏感信息，尤其是浏览器和邮件密码。确定攻击者也独立于Tirion Loader使用了此工具。在下图中，包含由恶意软件收集的日志的屏幕截图。

---


## 五.OpBlueRaven | 第一部分结尾

在这些系列的第一版中，我们希望通过将PTI发现的最新Carbanak工具包与可公开访问的较旧版本进行比较，以介绍我们的分析报告。

在下一篇文章中，我们还将通过提供攻击者之间实际对话的参考来更深入地研究攻击者的TTP。除此之外，我们还将提供直接从威胁参与者的计算机获取的屏幕截图。

最后希望这篇文章对您有所帮助，中秋节和国庆节快乐。某人照顾好自己喔！

**前文分享：**

<font color="red">**2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、逆向分析、APT分析报告、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。**</font>

(By:Eastmount 2020-10-04 星期天 晚上12点写于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


**附录：YARA签名**

```
import "pe"
rule apt_Fin7_Tirion_plugins
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Tirion Loader's plugins. It is used by Fin7 group. Need manual verification"
        version = "1.0"
        date = "2020-07-22"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "fdc0ec0cc895f5b0440d942c0ab60eedeb6e6dca64a93cecb6f1685c0a7b99ae"
        
    strings:
        $a1 = "ReflectiveLoader" ascii
        $a2 = "plg.dll" fullword ascii
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 15000 and (pe.exports("?ReflectiveLoader@@YA_KPEAX@Z") or
            pe.exports("?ReflectiveLoader@@YGKPAX@Z"))
}

rule apt_Fin7_Tirion_PswInfoGrabber
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Tirion Loader's PswInfoGrabber plugin. It is used by Fin7 group."
        version = "1.0"
        date = "2020-07-22"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "e7d89d1f23c2c31e2cd188042436ce6d83dac571a5f30e76cbbcdfaf51e30ad9"
   
    strings:
        $a1 = "IE/Edge Grabber Begin" fullword ascii
        $a2 = "Mail Grabber Begin" fullword ascii
        $a3 = "PswInfoGrabber" ascii
        $a4 = "Chrome Login Profile: '"
        $a5 = "[LOGIN]:[HOST]:"
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 150KB
}

rule apt_Fin7_Tirion_loader
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Tirion Loader's loader component. It is used by Fin7 group."
        version = "1.0"
        date = "2020-07-22"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "e7d89d1f23c2c31e2cd188042436ce6d83dac571a5f30e76cbbcdfaf51e30ad9"
   
    strings:
        $a1 = "HOST_PORTS" fullword ascii
        $a2 = "KEY_PASSWORD" fullword ascii
        $a3 = "HOSTS_CONNECT" ascii
        $a4 = "SystemFunction036"
        $a5 = "ReflectiveLoader"
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 15KB
}

rule apt_Fin7_Carbanak_keylogplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's keylogger plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "db486e0cb94cf2bbe38173b7ce0eb02731ad9a435a04899a03d57b06cecddc4d"
   
    strings:
        $a1 = "SA45E91.tmp" fullword ascii
        $a2 = "%02d.%02d.%04d %02d:%02d" fullword ascii
        $a3 = "Event time:" fullword ascii
        $a4 = "MY_CLASS" fullword ascii
        $a5 = "RegisterRawInputDevices" fullword ascii 

    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 15000
}

rule apt_Fin7_Carbanak_procmonplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's process monitoring plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "3bf8610241a808e85e6ebaac2bb92ba4ae92c3ec1a6e56e21937efec71ea5425"
   
    strings:
        $a1 = "[%02d.%02d.%04d %02d:%02d:%02d]" fullword ascii
        $a2 = "%s open %s" fullword ascii
        $a3 = "added monitoring %s" fullword ascii
        $a4 = "pm.dll" fullword ascii
        $a5 = "CreateToolhelp32Snapshot" fullword ascii  

    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 10000
}

rule apt_Fin7_Carbanak_hdplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's hidden desktop plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "39b545c7cd26258a9e45923053a5a64c9461470c3d7bfce3be1c776b287e8a95"
   
    strings:
        $a1 = "hd%s%s" fullword ascii
        $a2 = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced" fullword ascii
        $a3 = "StartHDServer" fullword ascii
        $a4 = "SetThreadDesktop" fullword ascii
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 15000
}

rule apt_Fin7_Carbanak_hvncplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's hvnc plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "40ce820df679b59476f5d277350dca43e3b3f8cac7ec47ad638371aaa646c315"
   
    strings:
        $a1 = "VncStartServer" fullword ascii
        $a2 = "VncStopServer" fullword ascii
        $a3 = "RFB 003.008" fullword ascii
        $a4 = "-nomerge -noframemerging" fullword ascii
        $a5 = "--no-sandbox --allow-no-sandbox-job --disable-3d-apis --disable-gpu --disable-d3d11" fullword wide
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 300000
}

rule apt_Fin7_Carbanak_vncplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's vnc plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "ecf3679f659c5a1393b4a8b7d7cca615c33c21ab525952f8417c2a828697116a"
   
    strings:
        $a1 = "VncStartServer" fullword ascii
        $a2 = "VncStopServer" fullword ascii
        $a3 = "ReflectiveLoader" fullword ascii
        $a4 = "IDR_VNC_DLL" fullword ascii
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 400000
}

rule apt_Fin7_Carbanak_rdpplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's rdp plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "0d3f1696aae8472145400d6858b1c44ba7532362be5850dae2edbd4a40f36aa5"
   
    strings:
        $a1 = "sdbinst.exe" fullword ascii
        $a2 = "-q -n \"UAC\"" fullword ascii
        $a3 = "-q -u \"%s\"" fullword ascii
        $a4 = "test.txt" fullword ascii
        $a5 = "install" fullword ascii
        $a6 = "uninstall" fullword ascii
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 400000
}

rule apt_Fin7_Carbanak_switcherplugin  
{
    meta:
        author = "Yusuf A. POLAT"
        description = "Carbanak backdoor's switcher plugin. It is used by Fin7 group"
        version = "1.0"
        date = "2020-07-21"    
        reference = "https://threatintelligence.blog/"
        copyright = "PRODAFT"
        SHA256 = "d470da028679ca8038b062f9f629d89a994c79d1afc4862104611bb36326d0c8"
        
    strings:
        $a1 = "iiGI1E05.tmp" fullword ascii
        $a2 = "oCh4246.tmp" fullword ascii
        $a3 = "inf_start" fullword ascii
        $a4 = "Shell_TrayWnd" fullword ascii
        $a5 = "ReadDirectoryChangesW" fullword ascii
        $a6 = "CreateToolhelp32Snapshot" fullword ascii
    condition:
        uint16(0) == 0x5A4D  and (all of ($a*)) and filesize &lt; 15000
}

```
