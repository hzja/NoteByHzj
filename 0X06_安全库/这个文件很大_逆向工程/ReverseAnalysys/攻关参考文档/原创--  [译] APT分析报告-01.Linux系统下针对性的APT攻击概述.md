# 原创
：  [译] APT分析报告：01.Linux系统下针对性的APT攻击概述

# [译] APT分析报告：01.Linux系统下针对性的APT攻击概述

这是作者新开的一个专栏，主要翻译国外知名的安全厂商APT报告文章，了解它们的安全技术，学习它们溯源APT组织的方法，希望对您有所帮助。第一篇文章主要分享Linux系统下针对性的APT攻击及技术要点，并简单总结溯源部分APT组织的方法。

关于Windows系统的APT攻击已经写了很多文章，这也许并不奇怪。由于Windows的流行，我们发现它是最适合攻击的平台。与此同时，人们普遍认为Linux是一种默认安全的操作系统，不会受到恶意代码的影响。毫无疑问，Linux并没有面临Windows系统多年来所遭遇的病毒、蠕虫和木马。<mark>但这并不意味着不存在，Linux确实也存在恶意软件——包括PHP后门（backdoors）、rootkit和exp代码（exploit code）。</mark>

此外，数据可能会误导人。运行Linux服务器的战略重要性使它们成为各种攻击者的目标。如果攻击者能够攻击运行Linux的服务器，他们不仅可以访问存储在服务器上的数据，还可以访问连接到服务器上运行Windows或macOS的端点。同时，Linux计算机更有可能不受保护，因此这种情况很可能不会引起注意。当2014年 **“心脏出血”（Heartbleed）和“Shellshock”漏洞** 首次被报道时，有两个主要的担忧是，受感染的Linux服务器可能成为攻击者进入公司网络的通道，并且可以使攻击者访问敏感的公司数据。

卡巴斯基全球研究与分析团队（GReAT）定期发布有关高级持续威胁（APT）活动的摘要，该摘要基于我们在APT私人报告中详细讨论的威胁情报研究。 在本报告中，我们重点介绍APT威胁参与者针对Linux资源的目标。

#### 文章目录

---


## Barium

卡巴斯基安全团队在2013年首次撰写了有关 **Winnti APT集团（又名APT41或Barium）** 的文章，当时他们主要针对游戏公司以获取其财务利润。与此同时，他们扩大了自己的业务，开发了大量的新工具，并致力于更复杂的目标。

最近，我们发现了另一种怀疑的Barium / APT41工具，它是用编程语言Go(也被称为Golang)编写的，该工具为Linux机器实现了动态的，受C2控制的数据包破坏/网络攻击工具。虽然它是否是为系统管理任务而开发的工具或者是否是APT41工具集的一部分还不清楚，但是它提供的功能也可以通过其他系统管理工具来实现，这一事实表明其目的不好的。而且，它在磁盘上的名称相当普通，与它的功能无关，这再次表明它可能是一个用于执行某些类型的破坏性攻击的工具。更多关于这个工具的细节可以在我们的报告中找到。

> 
**APT41溯源**<br/> C＆C、数字证书、域名、电子邮件，主要针对电子游戏公司。通过分析受感染PC的样本和行为，发现恶意代码将辅助程序ff.exe 下载到受感染计算机上的Config.Msi文件夹中，此代码在硬盘上搜索 HTML、MS Excel、MS Word、Adobe、PowerPoint和MS Works文档以及文本文件（.txt）。在ff.exe_中发现了调试行为，从而进行溯源。它们并没有立即引起注意，因为在编辑器中看起来像这样，但在进行详细分析后发现，该文本为中文简体GBK编码。同时，在恶意程序CmdPlus.dll插件的组件之一中也发现了ZW。 <br/><br/> <img alt="在这里插入图片描述" height="200" src="https://img-blog.csdnimg.cn/20200914161635357.png#pic_centerpic_center" width="450"/><br/> <img alt="在这里插入图片描述" height="250" src="https://img-blog.csdnimg.cn/20200914161627991.png#pic_centerpic_center" width="200"/><br/> <img alt="在这里插入图片描述" height="150" src="https://img-blog.csdnimg.cn/20200914162059499.png#pic_centerpic_center" width="500"/>


---


## Cloud Snooper

2020年2月，Sophos发布了一份报告，描述了一组恶意工具，该报告将其归因于一种之前未知的威胁行为——**云窥探者(Cloud Snooper)**。其核心是一个面向服务器的Linux内核rootkit，该内核钩子连接netfilter流量控制功能，以支持穿越防火墙的隐蔽C2（命令和控制）通信。我们分析并描述了rootkit的用户区同伴后门，称为“**Snoopy**”，并能够设计检测和扫描方法来大规模识别rootkit。同时，还发现了更多的样本以及亚洲的目标服务器。我们相信这个进化的工具集可能至少从2016年就已经在开发中了。

<img alt="在这里插入图片描述" height="350" src="https://img-blog.csdnimg.cn/202009141627042.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_centerpic_center" width="550"/><br/> <img alt="在这里插入图片描述" height="250" src="https://img-blog.csdnimg.cn/20200914163136899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_centerpic_center" width="650"/>

---


## Equation

我们在2015年发现了**方程式组织（Equation Group）**。这是一个非常复杂的威胁组织，可以追溯到2001年，也许早在1996年就参与了多次CNE（计算机网络利用）行动。多年来，该威胁组织与其他强大的APT团体进行了互动或合作。例如Stuxnet和Flame。

该小组拥有强大的武器库，我们发现的包括：

方程式组织的创新不限于Windows平台，该小组的POSIX兼容代​​码库允许在其他平台上进行并行开发。2015年，我们发现了针对Linux的早期DOUBLEFANTASY恶意软件。该植入程序收集系统信息和凭证，并提供对受感染计算机的通用访问。

> 
**溯源**<br/> Equation Group使用的其他C＆C似乎早在1996年就已注册，这可能表明该组已经活跃了将近20年。该小组本身为其工具和植入物有许多代号，包括SKYHOOKCHOW、UR、KS、SF、STEALTHFIGHTER、DRINKPARSLEY和GROK等。对于一个这样的精英团体来说，这似乎令人难以置信，一位开发人员犯了一个不可原谅的错误，即在其中一个恶意软件样本中保留其用户名“ RMGREE5 ”，如工作文件夹的路径“C:\users\rmgree5\”。同时， Fann蠕虫使用的LINK漏洞（Microsoft公告MS09-025修补的漏洞）如下： <br/><br/> <img alt="在这里插入图片描述" height="200" src="https://img-blog.csdnimg.cn/20200914165612325.png#pic_centerpic_center" width="550"/><br/> <img alt="在这里插入图片描述" height="380" src="https://img-blog.csdnimg.cn/2020091417021929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_centerpic_center" width="500"/>


---


## HackingTeam

HackingTeam是一家意大利信息技术公司，它向世界各地的政府、执法机构和企业出售入侵软件。不幸的是，他们在2015年遭到了黑客攻击，并遭受了一次严重的数据泄露，受害者是一位名叫菲尼亚斯·菲舍（Phineas Phisher）的活动人士。随后，包括源代码和客户信息在内的400GB被盗公司数据被泄露，使得这些工具被世界各地的攻击者获取、改造和使用，比如DancingSalome（又名Callisto）。泄露的工具包括：

参考资料：[Spyware. HackingTeam](https://securelist.com/spyware-hackingteam/37064/)

通过电子邮件发送的所有Mac OS X恶意文件都有指向名为“guido”的文件夹中文件链接，并且一个用户的linkedin.com资料表明他是HackingTeam的前开发人员，他的名字也叫Guido。

---


## Lazarus

在2018年底，我们发现了一个以前未知的恶意框架，并在内部将其命名为MATA。该框架用于定位韩国、印度、德国和波兰的商业公司。虽然我们找不到与任何其他已知角色重叠的代码，但卡巴斯基威胁归因引擎显示出与Manuscrypt代码相似之处，它是Lazarus（又名Hidden Cobra）使用过的复杂恶意软件。与Lazarus开发的早期恶意软件一样，此框架包括Windows后门。但是，我们还发现了一个Linux变体，我们认为它是为网络设备设计的。

2020年6月，我们分析了与拉撒路行动AppleJeus和TangoDaiwbo活动有关的macOS新样本，这些macOS被用于金融和间谍攻击。样本已经上传到VirusTotal，上传的文件还包括一个Linux恶意软件变种，其功能与macOS TangoDaiwbo恶意软件类似。这些样本证实了我们两年前强调的一个进展——该组织正在积极开发非Windows恶意软件。

参考文章：[MATA: Multi-platform targeted malware framework](https://securelist.com/mata-multi-platform-targeted-malware-framework/97746/)

> 
**溯源**<br/> MATA恶意框架与Lazarus的关联，MATA协调器使用两个唯一的文件名c_2910.cls和k_3872.cls，这些文件名以前仅在几种Manuscrypt变体中能看到，包括在US-CERT出版物中提到的样本。 <br/><br/> <img alt="在这里插入图片描述" height="150" src="https://img-blog.csdnimg.cn/20200914173724697.png#pic_centerpic_center" width="550"/><br/> 此外，MATA使用全局配置数据，包括随机生成的会话ID、基于日期的版本信息、休眠间隔以及多个C2和C2服务器地址。我们已经看到Manuscrypt变体与MATA框架共享相似的配置结构。这个旧的Manuscrypt变体是一个活动的后门，它具有类似的配置数据，例如会话ID、睡眠间隔、C2地址的数量、受感染的日期和C2地址，它们不相同，但是结构相似。 <br/><br/> <img alt="在这里插入图片描述" height="150" src="https://img-blog.csdnimg.cn/20200914181258163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_centerpic_center" width="550"/>


---


## Sofacy

Sofacy(又名APT28, Fancy Bear, STRONTIUM, Sednit和Tsar团队)是一个非常活跃和高产的APT威胁行动者。从它的大量零日部署到它的创新广泛的恶意软件集合，Sofacy是我们监控的顶级组织之一。在该组织的武器装备中，有一种工具是SPLM（也被称为CHOPSTICK和XAgent），这是一种第二阶段的工具，有选择地用于对付世界各地的目标。

多年来，Sofacy已经为多个平台开发了模块，包括在2016年被检测为“Fysbis”的Linux模块，并在Windows、macOS、iOS和Linux上看到的一致成果表明，相同的开发人员或小型核心团队正在修改和维护代码。

---


## The Dukes

The Dukes是一个复杂的威胁组织，最早由美国在2013年记录在案，但他们的工具被用于攻击可以追溯到2008年。该组织负责针对乌克兰、格鲁吉亚以及西方国家和目标，北约和个人。同时，该组织被认为是2016年美国MZD大会的幕后黑手。Dukes的工具集包括一套全面的恶意软件，这些恶意软件实现了相似的功能，但使用几种不同的编程语言进行了编码。该组织的恶意软件和活动包括：PinchDuke、GeminiDuke、CosmicDuke、MiniDuke、CozyDuke、OnionDuke、SeaDuke、HammerDuke和CloudDuke。其中至少有一个SeaDuke包含Linux变体。

**MiniDuke恶意软件**：将使用Twitter（用户不知道）并从预制帐户中查找特定的推文，这些帐户由MiniDuke的命令和控制（C2）操作员创建，并且这些推文还维护特定的标签。标签为后门标记了加密的URL，然后URL提供对C2的访问，C2提供潜在的命令以及通过GIF文件将其他后门加密传输到系统上。

---


## The Lamberts

Lamberts是一个高度复杂的威胁组织，据称拥有庞大的恶意软件库，包括被动的、网络驱动的后门，模块化后门，收集工具和用于进行破坏性攻击的wipers。我们创建了一种配色方案，以区分针对全球不同受害者的各种工具的时间表。

2017年，我们发布了Lamberts家族概述，进一步的更新（GoldLambert、SilverLambert、RedLambert、BrownLambert）将提供给我们威胁情报报告的客户。各种Lamberts变体的焦点肯定是Windows，但是，我们为Windows的Green Lambert创建的签名也触发了功能与Windows版本相似的macOS变体。此外，我们还确定了针对Windows和Linux编译的SilverLambert后门示例。

---


## Tsunami backdoor

海啸（又名Kaiten）是一个UNIX后门，自2002年首次在野外发现以来，被多个威胁者使用。其源代码几年前就公开了，现在有70多个变种。源代码可以在各种嵌入式设备上顺畅地编译;还有针对ARM、MIPS、Sparc和思科4500/PowerPC的版本。

海啸仍然是基于Linux的路由器、DVR和不断增加的物联网设备的威胁。2016年，Linux Mint黑客使用了Tsunami的变体，其中一个未知的威胁因素破坏了Linux Mint发行版ISO，使其包含后门。我们还观察到使用Tsunami后门以手术方式针对Linux上的许多加密货币用户。

---


## Turla

Turla（又名Uroboros、Venomous Bear和Waterbug）是一个多产的俄语组织，以其秘密的渗透策略而闻名，比如利用劫持的卫星连接、在政府网站上挖洞、秘密通道后门、rootkits和欺骗策略。像其他APT组织一样，该威胁组织多年来也对其工具集进行了重大更改。直到2014年，我们看到的Turla使用的每个恶意软件样本都是针对32位或64位版本的Windows设计的。

**然后在2014年12月，我们发布了有关Turla工具库中Linux组件Penguin Turla的报告。这是一个后门，不需要提升特权，即管理员或root权限。即使有人对系统的访问受到限制，后门也可以拦截传入的数据包并在保持隐身性的同时运行系统攻击者的命令。** 它也很难被发现，因此，如果将其安装在受感染的服务器上，则可能会长时间不被注意。对Penguin Turla的进一步研究表明，其起源可以追溯到1990年代中期的月光迷宫行动。今年5月，来自Leonardo的研究人员发表了有关Penguin_x64的报告，这是 **Penturin Turla Linux** 后门的先前未记录的变体。根据此报告，我们生成了可大规模检测Penquin_x64感染主机的网络探针，使我们能够在2020年7月之前在欧洲和美国发现几十个受感染服务器。我们相信，根据GNU/Linux工具的公开文档，Turla可能已经对Penguin进行了改造，使其能够执行传统情报收集之外的操作。

---


## Two-Sail Junk

2020年1月，一个水坑（watering hole）被发现，该水坑利用完整的远程iOS漏洞链部署了功能丰富的植入物LightSpy。根据登陆页面的内容，该网站似乎是为XG用户设计的。目前，在我们能把这个活动和一个已知的组织联系起来之前，我们已经把这个植入物背后的威胁者命名为Two-Sail Junk。然而，尽管我们的公开报告聚焦于iOS的植入程序，但这个项目比之前想象的更广泛，它也支持Android的植入，并可能也支持Windows、Linux和MacOS的植入。

LightSpy iOS植入组件的布局和通信如下图所示：

最初的水坑站点（hxxps://appledaily.googlephoto[.]vip/news[.]html）于2020年1月10日设计，旨在通过复制粘贴HTML来模仿XG知名报纸《苹果日报》。原始内容：

从2月18日开始，攻击者开始利用一系列不可见的iframe将潜在受害者从诱饵重定向到漏洞利用站点以及预期的合法新闻站点。

---


## WellMess

2020年3月，我们开始积极跟踪与恶意软件WellMess相关的新C2服务器，这意味着潜在的大规模新活动浪潮。该恶意软件最初是在2018年7月被JPCERT记录下来的，从那以后就偶尔活跃起来。有传言暗示可能与CozyDuke（又名APT29）有关，还有猜测称目前的活动主要集中在医疗保健行业，尽管我们无法证实这两种说法。

---


## WildNeutron

我们在2015年与Symantec的同事Morph或Butterfly一起首次发布了有关WildNeutron的信息。该群体在2012至2013年对Twitter、Microsoft、Apple和Facebook的攻击中脱颖而出，是我们所见到的最难以捉摸、最神秘和最活跃的群体之一。他们的武器库包括许多有趣且创新的工具，例如LSA后门或IIS插件，以及基于0day的部署和物理部署。毫不奇怪，在几种已知的攻击中，WildNeutron也使用了定制的Linux后门。

> 
**溯源**<br/> 在该组织的某些样本示例中，加密的配置包括罗马尼亚语字符串，该字符串用于标记C＆C通信的结束。有趣的是，“ La revedere”在罗马尼亚语中是“再见”。除此之外，我们发现了另一个非英语字符串，这是俄语单词Успешно（“ uspeshno”-&gt;“ successfully”）的拉丁语转录。执行C2命令后，将此字符串写入管道。 <br/><br/> <img alt="在这里插入图片描述" height="200" src="https://img-blog.csdnimg.cn/2020091419052767.png#pic_centerpic_center" width="550"/>


---


## Zebrocy

Zebrocy是自2015年以来一直在跟踪的自定义恶意软件。使用该恶意软件的组织最初是Sofacy的子集，但与其他APT组也有相似之处和重叠之处。该小组已经开发了多种语言的恶意软件，包括Delphi、AutoIT、.NET、C＃、PowerShell和Go。Zebrocy主要针对国内和偏远地区的中亚政府相关组织。该组织广泛使用鱼叉式网络钓鱼来破坏Windows端点。但是，它的后门配置为通过端口80与IP分配的Web服务器主机直接通信。并且该小组似乎更喜欢Linux作为其基础架构的一部分，特别是在Debian Linux上运行的Apache 2.4.10。

---


## 保护Linux系统的建议

Linux系统不受保护的主要原因之一是：对使用Linux而不是更流行（且更具针对性）的Windows会产生一种错误的安全感。尽管如此，我们希望上述所有观点足以说服您，让您开始认真地保护基于Linux的计算机。

我们把这些建议分成了合理的部分。请记住，除了应用我们提到的所有措施外，您还应该定期审计和检查所有生成的日志和任何其他消息。否则你可能会错过被打扰的迹象。最后一个想法，对于安全狂热者来说，是采取积极的措施——不时地提供系统渗透测试。

---


## 总结

写到这里，这篇文章就介绍完毕，主要翻译了卡巴斯基的Linux中APT攻击的报告，并融合了自己的理解及相关资料，希望这系列翻译文章对您有所帮助。学安全一年，认识了很多安全大佬和朋友，希望大家一起进步。这篇文章中如果存在一些不足，还请海涵。作者作为网络安全初学者的慢慢成长路吧！希望未来能更透彻撰写相关文章，深知自己很菜，得努力前行。

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:Eastmount 2020-09-14 星期一 晚上11点写于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )
