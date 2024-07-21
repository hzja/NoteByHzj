# 原创
：  [译] APT分析报告：06.猖獗的小猫——针对伊朗的APT攻击活动详解

# [译] APT分析报告：06.猖獗的小猫——针对伊朗的APT攻击活动详解

这是作者新开的一个专栏，主要翻译国外知名的安全厂商APT报告文章，了解它们的安全技术，学习它们溯源APT组织的方法，希望对您有所帮助。前文分享了Turla新型水坑攻击后门（NetFlash和PyFlash），通过伪造Adobe Flash更新，欺骗受害者下载两个新型恶意软件NetFlash和PyFlash，从而实现恶意攻击。这篇文章将详细介绍Rampant Kitten攻击活动，包括Windows信息窃取程序、Android后门和电报网络钓鱼页面，这是Check Point Research机构的分析报告，非常值得大家学习。

#### 文章目录

---


Check Point Research揭露了伊朗实体多年来一直针对伊朗侨民和持不同政见者多年进行监视行动。尽管其他研究人员和记者先前曾报道过一些针对个人的攻击事件，但我们的调查能够将不同的攻击行动联系起来，并将它们归因于同一攻击者。

我们发现了不同的攻击载体，包括：

上述工具和方法似乎主要用于对付伊朗少数MZ、FZQ组织和抵抗YD，例如:

## 一.初次感染

我们首先遇到的文档名称为 “وحشت_رژیم_از_گسترش_کانونهای_شورشی.docx”，大致翻译为“当局担心GMDP的扩散”.docx。该文件的标题实际上指的是伊朗ZQ与GMDP运动之间的持续斗争。

Mujahedin-e Khalq是一个FZF组织，其目标是使伊朗摆脱目前的地位。1986年，MEK开始建立他们的新总部，后来在Iraqi小镇哈里斯附近被称为Ashraf营地。但是，Iraqi多年的紧张局势最终导致该营地的居民被转移到一个新的、偏远的目的地——阿尔巴尼亚。

上面的文档利用了外部模板技术，允许它从远程服务器加载文档模板，在本例中是 <mark>afalr-sharepoint[.]com</mark>。远程模块如下图所示：

出于对这个网站的好奇，我们开始探索更多关于它的信息。我们发现一些反对伊朗ZQ的账户发布推文，提到了一个非常类似的SharePoint网站，该文档中的网站很可能冒充了以下网站：

AFALR的官方网站如下图所示：

---


## 二.感染链

当受害者打开文档并下载了远程模板后，模板中的恶意宏代码将执行一个批处理脚本，该脚本尝试从afalr-sharepoint[.]com下载并执行下一阶段的有效负载。

然后，有效负载会检查是否在受感染的计算机上安装了Telegram，如果检查有效，它将继续从其资源中提取三个额外的可执行文件。

---


## 三.有效载荷分析

该恶意软件的主要功能包括：

### 1.电报结构基础

首先，让我们回顾一下Telegram Desktop如何组织其文件。以下是一个普通的电报文件结构，通常可以在 <mark>%APPDATA%\Roaming\Telegram Desktop</mark> 路径找到。

如上所述，在感染链期间，有几个文件被拖放到Telegram工作目录中。删除的文件位于一个名为03A4B68E98C17164s的目录中，由于是一个定制的 <mark>Desktop.ini</mark>文件，所以第一眼看上去像一个文件，但它实际上是一个目录。受感染的Telegram Desktop目录如下图所示。

---


### 2.配置

有效负载的一个资源包含编码的配置数据。编码方案使用常规的Base64算法，但是使用自定义索引表为：

对该资源进行解码可为我们提供以下配置信息：

---


### 3.C＆C沟通

该恶意软件使用SOAP进行通信。SOAP是用于Web服务通信的基于XML的简单数据结构。SOAP Web服务的API是公开的，可以从以下浏览器访问网站来观察。C＆C网站中的SOAP API如下图所示：

消息（命令）可以分为以下几类：

**(1) 身份认证**<br/> 有效通信隧道的第一条消息应该是HelloWorld，它实现了简单的用户名/密码身份验证。凭证在示例中进行了硬编码，并且该消息的SOAP响应包含一个会话ID，该会话ID必须用于其余的会话。

**(2) 模块下载器**<br/> 该程序尝试获取其当前模块的更新，并下载几个其他模块。一些额外的确实模块无法获取，包括：

**(3) 数据渗漏**<br/> 该恶意软件的核心功能是从目标设备中窃取尽可能多的信息。有效负载针对两个主要应用程序：Telegram Desktop和著名的密码管理器KeePass。

一旦相关的Telegram桌面和KeePass的文件已被上传，恶意软件就会列举在受害者电脑上找到具有以下扩展名的相关文件:

对于每个这样的文件，恶意软件在将其编码为base64后将其上传。

---


### 4.持久性

该恶意软件使用一种独特的持久性方法，该方法与电报更新过程相关联。它将定期将Telegram主可执行文件复制到中 <mark>Telegram Desktop\tupdates</mark>，一旦电报应用程序启动，就会触发一个更新程序。该恶意软件的持久性方法的一个隐藏技巧是更改默认的Telegram更新器文件 <mark>Telegram Desktop\Updater.exe</mark>，其中包含已删除的有效载荷之一（具体的是CO79B3A985C5C7D30）。该更新程序最显着的更新是再次运行有效负载，其中Telegram更新程序运行主要有效负载如下图所示。

---


## 四.基础设施和连接

在分析了有效载荷之后，我们能够追溯到2014年的多种变体，这表明这种攻击已经酝酿了多年。由相同攻击者开发的恶意软件变体之间常常会有细微的差异，尤其是在相同的时间范围内使用它们时。但是，在这种情况下，我们注意到，虽然某些变体是同时使用的，但它们是用不同的编程语言编写的，利用了多种通信协议，并不总是窃取相同类型的信息。

在下表中，我们列出了发现的变体并突出了它们的独特特征。请参阅下面的技术附录，了解关于每种变体的详细信息。

相关样本还揭示了更多的C＆C服务器，通过查找它们的被动DNS信息和其他元数据，我们可以找到由同一攻击者操作的相似域。事实证明，某些域名出现在恶意Android应用程序和网络钓鱼页面中，从而暴露了该操作的更多层次信息。下图为恶意基础架构的Maltego图。

---


## 五.Android后门

在我们的调查过程中，还发现了与相同威胁参与者有关联的恶意Android应用程序。该应用程序伪装成一种服务，以帮助在瑞典说波斯语的人获得驾照的服务。我们已经找到了同一应用程序的两个不同变体，一个似乎是为了测试而编译的，另一个是要在目标设备上部署的发行版。Android应用程序的主界面如下图所示。

此Android后门包含以下功能：

有关此应用程序的详细信息，请参阅下面的技术附录。

---


## 六.电报网络钓鱼

这些后门并不是攻击者试图窃取有关Telegram帐户信息的唯一方法。与该恶意活动相关的某些网站还托管了仿冒Telegram的网络钓鱼页面，如下图所示。

令人惊讶的是，几个伊朗电报频道实际上已经发出了针对这些钓鱼网站的警告，并声称伊朗ZQ是他们的幕后黑手。针对网络钓鱼尝试的翻译后的消息警告如下图所示。

根据这些频道，钓鱼信息是由电报机器人发送的。这些信息警告收件人，他们正在不当使用Telegram的服务，如果他们不进入钓鱼链接，他们的账户将被封掉。

另一个Telegram频道提供了钓鱼企图的截图，显示攻击者建立了一个冒充官方Telegram的账户。最初，攻击者在新的Telegram更新中发送了有关这些功能的消息，使其显得合法。网络钓鱼消息仅在5天后发送，并指向https://telegramreport[.]me/active（与上面图相同的域）。下图展示了从假电报帐户发送的网络钓鱼消息。

---


## 七.有效载荷传递

尽管在某些情况下我们无法确定恶意文件如何到达受害者，但我们收集了一些有关攻击者分发恶意软件方式的潜在线索。例如，访问 mailgoogle[.]info 表明它模拟 ozvdarozv[.]com 并推广了一种软件，以增加Telegram频道中的成员数量。mailgoogle[.]info下载页面如下图所示。

但是，在单击“下载”后，一个名为 <mark>Ozvdarozv-Windows.rar</mark> 的有密码保护的文档就会被下载，其包含一种恶意软件变种。

2018年一篇被删除的博客文章指控一名网络安全专家剽窃，当时他在接受阿拉比亚新闻频道采访，讨论伊朗的网络攻击。我们相信这个页面是针对此人或他的同伙的有针对性的攻击的一部分。这个博客包含了一个链接，可以下载一个有密码保护的档案，其中包含了来自 <mark>endpload [.]com</mark> 的抄袭证据。

endupload[.]com 通过几个被动DNS跃点连接到上述PC和Android操作，包括通过历史DNS服务器信息直接连接到 <mark>mailgoogle[.]info</mark> 域名。我们不仅没有发现在合法环境中使用过该域名的任何实例，而且还发现了该域名已被波斯语黑客注册的证据。请参见下面的“归因”部分，已删除的博客文章及其链接 endupload[.]com 如下图所示。

与2012年不同的博客文章讨论了伊朗人权活动家协会附属新闻机构HRANA的侵犯人权行为报告。再次，此博客引用了可以从endupload[.]com以下网站下载的文档：

2012年的另一篇博客文章则讨论了伊朗RQ活动人士协会（Iranian Association of human rights Activists）下属新闻机构HRANA发布的一篇报告。同样，这个博客引用了一个可以从 endpload [.]com 下载的文档。下图展示了博客中带有指向 endupload [.] com 的链接。

不幸的是，我们无法获得两个博客条目所指的文件，也无法确认它们是否是恶意的。但是，似乎 endupload[.]com 已经由攻击者控制了多年，因为与此攻击有关的一些恶意样本（可追溯到2014年）已与该网站进行了通信。

---


## 八.溯源

尽管我们发现多年来在此攻击中使用过的许多文件和网站，但它们并未归因于特定的威胁组织或实体。但是，威胁攻击者留在恶意事件中的一些指纹使我们可以更好地了解攻击可能来自何处。

### 1.攻击起源

首先，某些恶意网站的WHOIS信息表明，它们据称是由伊朗个人注册的。下图展示了 endupload[.]com 和 picfile[.]net 的WHOIS信息。

WHOIS记录了 endpload[.]com 还提到一个电子邮件地址，nobody.gu3st@gmail[.]com。显然，该网站的注册用户在网上非常活跃，因为查询的用户名是“nobody.gu3st ”，我们可以在伊朗黑客论坛上看到关于他的许多帖子。

### 2.目标

我们观察到的目标清单反映了伊朗内部的一些斗争和这次Xi击背后的动机。精心挑选的目标包括SZ组织和阿塞拜疆组织的支持者，这两个著名的抵抗运动提倡JF伊朗人民和伊朗境内的少数MZ。

这些运动与伊朗当局之间的意识形态CT使它们成为这种Xi击的自然目标，因为它们与该ZQ的目标相吻合。此外，后门的功能以及窃取敏感文件、访问KeePass和Telegram账户的重点表明，攻击者对收集受害者的情报、了解他们的更多活动很感兴趣。

---


## 九.结论

追踪这次攻击的踪迹可以发现，这次大规模行动在很大程度上成功地隐藏了至少6年。根据我们收集到的证据，这些似乎来自伊朗的攻击者利用多种攻击载体暗中监视他们的受害者及其个人电脑和移动设备。

由于我们确认的大多数目标都是伊朗人，这可能是伊朗攻击者正在收集关于该组织潜在对手的情报。SandBlast Mobile可提供实时威胁情报和对移动威胁的可见性，防止受到恶意软件、网络钓鱼、中间人攻击、操作系统攻击等的侵害。Check Point的反网络钓鱼解决方案包括针对网络钓鱼攻击的所有攻击媒介的产品，包括电子邮件、移动电话、端点和网络。

---


## 十.技术附录——PC后门变异分析

### 1.TelB变体

“ TelB”是我们遇到的最新变体，其分析如上所示。我们之所以这样命名是因为下一个调试字符串：

---


### 2.TelAndExt变体

此变体可能是“ TelB”的旧版本，主要在2019年和2020年期间活跃。它与新版本共享以下特性和技术：

我们将此变体命名为“TelAndExt”，因为下一个调试字符串为：

---


### 3.Python信息窃取变体分析

我们发现了一些使用以下方法的样本：

根据我们的分析，该变体在信息窃取方面，使用硬编码的凭据与FTP服务器通信，并窃取以下数据：

同时，在调查过程中，我们看到了几个Python信息窃取器，它们与同一个FTP服务器通信，但是将窃取的信息以不同的别名存储在不同的页面中。我们怀疑这是恶意软件作者的操作方式：

第二阶段有效负载HookInjEx：其核心功能之一是获取第二阶段的有效负载。如果指定的FTP文件夹包含名为 net-update.exe 的文件，则它将下载并执行该文件。我们分析了其中的几个net-update.exe样本，发现与下面的“HookInjEx”变体完全重叠，从而使其成为有针对性的高级有效负载。

---


### 4.HookInjEx变体

该变体自2014年以来一直在使用，并具有32位和64位版本。随着时间的流逝，该变体不断演变并添加了一些功能，同时还更改了其感染链中不同组件的名称。我们发现了两种主要的感染链：

SCR文件还将其他有效载荷作为其资源删除，并使用命令行运行主加载程序：

```
cmd.exe /C choice /C Y /N /D Y /T 3 &amp; 
"%APPDATA%\\Microsoft\\Windows\\Device\\DrvUpdt.exe" -pSDF32fsj8979_)$

```

恶意SCR打开诱饵JPG资源如下图所示：

该变体在挂钩和注入（Hooking and Injection）方面，主加载程序使用称为“HookInjEx”的挂钩和注入方法。该方法将DLL映射到explorer.exe，在其中将Start按钮子类化。在我们的示例中，加载的DLL是DrvUpdtd.dll（dhtmlh.dll在较早的版本中）。

在新版本中，该恶意软件还会以其他语言挂起“Start”按钮。不同语言的存在可能表明它的受害者来自世界各地的国家。不同的翻译是：

该恶意软件从一个名为Devinf.asd的文件接收其配置。asd(在旧版本中命名为file2.asd)。配置被解密并写入一个名为Drvcnf的新文件。asd(旧版本中命名为file3.asd)。加密方法为:

该恶意软件从一个名为的文件Devinf.asd（在较早的版本中名为file2.asd）接收配置。解密配置并将其写入名为Drvcnf.asd的新文件（在旧版本中名为file3.asd）。加密方法为：

```
def decode(content):
    dec_array = [0, 1, 2, 3, 4, 5, 6, 7, 8, 
    0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 
    0x16, 0x17, 0x18, 0x19, 0x1a,0x1b, 0x1c, 0x1d, 
    0x1e, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
    0x38, 0x39,0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 
    0x40, 0x41, 0x42, 0x43, 0x44]
    output_str = ''
    known_values = [9, 10, 13, 32]
    for j in range(len(content)):
        int_cur_content = ord(content[j])
        cur_byte = 0
        for i in range(62):
            if int_cur_content == dec_array[i]:
                if i &lt; 26:
                    cur_byte = i + 0x61
                elif 26 &lt;= i &lt; 52:
                    cur_byte = i + 0x27
                elif 52 &lt;= i &lt; 62:
                    cur_byte = i - 0x4
                output_str += (chr(cur_byte))
                break
        if cur_byte == 0:
            if int_cur_content in known_values:
                cur_byte = int_cur_content
            elif int_cur_content - 0x61 &lt;= 0xe:
                cur_byte = int_cur_content - 0x40
            elif int_cur_content - 0x70 &lt;= 0x6:
                cur_byte = int_cur_content - 0x36
            elif int_cur_content - 0x77 &lt;= 0x5:
                cur_byte = int_cur_content - 0x1c
            elif int_cur_content - 0x53 &lt;= 0x3:
                cur_byte = int_cur_content + 0x28
            output_str += (chr(cur_byte))
    return output_str

```

解密配置后，恶意软件会解析这些值并将其放入全局变量中。

```
&lt;Reg&gt;&lt;/Reg&gt; - Registry key for persistence
&lt;Pre&gt;&lt;/Pre&gt; - pre value for info files to send
&lt;Pas&gt;&lt;/Pas&gt; - extensions for info files to send
&lt;Path_Log&gt;&lt;/Path_Log&gt; - log path direcory
&lt;L_s&gt;&lt;/L_s&gt; - minimum size for file to send
&lt;S_n&gt;&lt;/S_n&gt; - FTP domain
&lt;F_k&gt;&lt;/F_k&gt; - FTP User value
&lt;F_R&gt;&lt;/F_R&gt; - FTP Password value
&lt;Ver&gt;&lt;/Ver&gt; - version
&lt;U_u2&gt;&lt;/U_u2&gt; - Downloads updates URL
&lt;U_u1&gt;&lt;/U_u1&gt; -  Downloads updates URL
&lt;F_f&gt;&lt;/F_f&gt; - Directory in ftp connection.
&lt;U_t&gt;&lt;/U_t&gt; - timer_1
&lt;S_t&gt;&lt;/S_t&gt; - timer_2
&lt;S_q&gt;&lt;/S_q&gt; - timer_3
&lt;U_u3&gt;&lt;/U_u3&gt; - Downloads updates URL
&lt;El&gt;&lt;/El&gt; - value for encryption method to files
&lt;Ez&gt;&lt;/Ez&gt; - value for encryption method to files
&lt;F_n&gt;&lt;/F_n&gt; - Fake name
&lt;E_dt&gt;&lt;/E_dt&gt;
&lt;E_dy&gt;&lt;/E_dy&gt;
&lt;Snd_P&gt;&lt;/Snd_P&gt; - Value to choose name template for info files to send.
&lt;Mlt&gt;&lt;/Mlt&gt; - flag for exeucting again from different place.
&lt;Ws1&gt;&lt;/Ws1&gt; - WHOIS first URL
&lt;Ws2&gt;&lt;/Ws2&gt; - WHOIS second URL
&lt;Ws3&gt;&lt;/Ws3&gt; - WHOIS third URL
&lt;S_li&gt;&lt;/S_li&gt;
&lt;RTL&gt;&lt;/RTL&gt;
&lt;SHttpR&gt;&lt;/SHttpR&gt; - value to download using computer-name and username
&lt;OPP&gt;&lt;/OPP&gt; - Opera gather data flag
&lt;FIP&gt;&lt;/FIP&gt; - Firefox gather data flag
&lt;CHP&gt;&lt;/CHP&gt; - Chrome gather data flag
&lt;WHP&gt;&lt;/WHP&gt; - WHOIS flag
&lt;TRP&gt;&lt;/TRP&gt; - tracert flag
&lt;FRC&gt;&lt;/FRC&gt; - number of tries
&lt;Clipfp&gt;&lt;/Clipfp&gt; - clipboard data flag of CF_HDROP (CLB-f.jpg)
&lt;Cliptp&gt;&lt;/Cliptp&gt; - clipboard data flag of text, unicode, oemtext and locale (clb-t.jpg)
&lt;Clipip&gt;&lt;/Clipip&gt; - clipboard data flag of bitmap and dib (clb-p.jpg)

```

<mark>**在持久化方面：**</mark><br/> 该恶意软件将&lt; Reg &gt;配置文件值中的注册表项（几乎总是注册表项RunOnce）设置为以下值，它将名称设置为SunJavaHtml或DevNicJava，其值为DrvUpdt.exe 11。这样恶意软件就知道它已经被执行了。在旧版本中，该恶意软件用来删除名为rreegg.exe或Capdev.exe的文件。该文件被添加到RunOnce中，然后依次执行DrvUpdt.exe 11。

<mark>**在信息窃取方面：**</mark><br/> 该恶意软件的主要功能是从受害者的计算机中窃取信息，然后使用FTP将其发送到C2。该恶意软件窃取了不同类型的数据：

下图为带有硬编码可移动驱动程序的调试代码。

<mark>**在C2通讯方面：**</mark><br/> 此变体使用FTP协议将文件上传到其C2域。FTP域位于&lt; S_n &gt;标记内的配置文件中。

该变体还与其他域联系以更新其不同组件。域被放置在配置文件内&lt; U_U1 &gt;，&lt; U_U2 &gt;和&lt; U_U3 &gt;标签。使用 URLDownloadToFileW 从给定的URL下载文件，这些文件user_id包含在URL中。

<mark>**在字符串混淆方面：**</mark><br/> 在较新版本（自2018年起）中，使用以下脚本对字符串进行加密：

```
buf_1 = 'qweyuip[];lkjhgdszcm,.&gt;&lt;MNBVCXZ|ASDFGHJK:}{POIUYTREWQ123456789-=+_)(*&amp;^%$#@!'
buf_2 = '!#$%&amp;()*+,-.123456789:;&lt;=&gt;@ABCDEFGHIJKMNOPQRSTUVWXYZ[]^_cdeghijklmpqsuwyz{|}'
input_str = "" # The encrypted string
output_str = ''
for i in range(len(input_str)):
      cur_byte = input_str[i]
      place = buf_2.find(cur_byte)
      if place == -1:
            output_str += cur_byte
            continue
      new_byte = buf_1[place]
      output_str += new_byte
print(output_str)

```

---


## 十一.技术附录——Android后门分析

第一个活动是MainActivity，它负责向用户提供诱饵内容并请求执行特权活动的权限。它还启动一个名为MainService的后台服务，并在服务器发送命令时启动第二个MainActivityFake (GmailActivity)。下图展示了要求用户允许一组权限。

### 1.数据采集

一旦GmailActivity启动MainService，它将负责以下任务：计时器注册、配置监视、显示虚假通知（如下所述）和敏感数据收集。在此初始数据收集过程中，将读取并准备以下信息：

从C＆C服务器收到命令后，其余信息将按需收集：

---


### 2.Google凭据盗窃

在收到正确的命令从C&amp;C服务器，谷歌登录页面将显示给受害者，通过激活MainActivityFake (GmailActivity)。Google登录页面如下图所示：

此时，用户会看到一个合法的 accounts.google.com 登录页面，位于Android的WebView中。为了窃取输入的凭证，使用了Android的JavascriptInterface，以及一个定时从用户名和密码输入字段中检索信息的计时器。定期检索Google帐户凭据代码如下图所示：

---


### 3.“已启用Google保护”

如前所述，它的核心功能之一是打开麦克风并记录周围环境。为了实时实现这一目标，应用程序需要在后台运行其服务。任何想要执行此类操作的Android应用程序都需要向用户发布一个正在进行的通知，提醒用户设备上有未启动的活动。为了规避这个问题，恶意软件开发者选择向用户显示一个“谷歌保护已启用”的假通知。下图展示了应用程序显示虚假通知。

结果是伪装成“ Google保护”的永远在线的诱饵通知。

---


### 4.C＆C

该恶意软件使用常规的HTTP与C&amp;C服务器通信。它将初始请求发送给alarabiye[.]net，并继续与gradleservice[.]info通信以获得配置、命令和状态更新信息。为了上传所有敏感信息，该恶意软件使用带有硬编码凭证的FTPS。

此外，在将敏感文件上传到FTP服务器之前，它会使用AES算法以及预先配置的密码对它们进行加密，AES加密代码如下。

### 5.短信渗透

<mark>此恶意应用程序的独特功能之一是将前缀为G-(谷歌双因素身份验证码的前缀)的短信转发到它从C&amp;C服务器接收到的电话号码。此外，所有来自Telegram等社交网络应用的短信也会自动发送到攻击者的电话号码。</mark>

在我们的分析过程中，很明显这个恶意应用程序仍在积极开发中，各种资产和功能要么是以前操作的遗留，要么是尚未使用。其中一个未使用的网络钓鱼资产甚至包含一个预先输入的用户名，可能是攻击者先前操作的目标。未使用的网络钓鱼HTML资产和位置跟踪代码如下图所示。

---


## 十二.相关指标及总结

**网络钓鱼**

**安卓C＆C服务器**

**文件的MD5**

**文件的SHA-1**

**文件的SHA-256**

**TelB变体C＆C服务器**

**后门的MD5**

**后门的SHA-1**

**后门的SHA-256**

最后希望这篇文章对您有所帮助，更多指标推荐大家阅读原文。非常感谢大家的祝福和关心，希望早些时候带思远去珞珈山看看，如果以后他能考上就更好了，哈哈。爸爸即将返校奋斗，希望宝宝和女神都健健康康、开开心心。敏而多思，宁静致远。无思远人，劳心忉忉。爱你们喔回去后得更加努力，脚踏实地，当然最重要的还有远方和你们，晚安娜~

**前文分享：**

<font color="red">**2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、逆向分析、APT分析报告、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。**</font>

(By:Eastmount 2020-11-09 星期一 晚上10点写于贵阳 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )
