# 原创
：  [译] APT分析报告：04.Kraken - 新型无文件APT攻击利用Windows错误报告服务逃避检测

# [译] APT分析报告：04.Kraken - 新型无文件APT攻击利用Windows错误报告服务逃避检测

这是作者新开的一个专栏，主要翻译国外知名的安全厂商APT报告文章，了解它们的安全技术，学习它们溯源APT组织的方法，希望对您有所帮助。前文分享了APT组织Fin7 / Carbanak的Tirion恶意软件，包括OpBlueRaven行动。这篇文章将介绍一种新型无文件APT攻击Kraken，它会利用Windows错误报告服务逃避检测。其中，DllMain函数反分析检查，以确保它不在分析/沙箱环境或调试器中运行非常值得我们学习。

> 
Malwarebytes研究人员发现了一种名为Kraken的新攻击，该攻击利用Windows错误报告（WER）服务以逃避检测。攻击始于一个包含“Compensation manual.doc”的ZIP文件。该文档包含一个恶意宏，该宏使用CactusTorch VBA模块的修改版，通过使用VBScript将.Net编译的二进制文件加载到内存中来执行，以进行无文件攻击。该二进制文件通过将嵌入式Shellcode注入Windows错误报告服务（WerFault.exe）来推进了感染链，此策略用于尝试逃避检测。


#### 文章目录

---


2020年9月17日，我们发现了一种名为Kraken的新攻击，该攻击将其有效载荷注入到Windows错误报告（Windows Error Reporting，WER）服务中，作为一种防御规避机制。

这个报告服务是WerFault.exe，通常发生在与操作系统、Windows函数或应用程序相关的错误时调用。当受害者看到他们的计算机上运行WerFault.exe时，他们可能认为发生了一些错误，而在这种情况下，他们实际上已成为攻击的目标。

尽管这项技术不是什么新技术，但这次行动很可能是一个APT组织发动的，该组织先前曾使用网络钓鱼攻击，诱使受害者提出工人赔偿要求。威胁攻击者入侵了一个网站以托管其有效载荷，然后使用CactusTorch框架执行无文件攻击（fileless attack），随后采用多种反分析技术（anti-analysis ）。

在撰写本文时，尽管有一些因素让我们认为其是越南APT32组织，但目前仍然不能明确指出这次攻击的幕后发动者。

> 
WerFault.exe是一个Windows系统自带的程序，用于错误报告显示。在应用程序崩溃时，它仍然会执行未处理的异常处理程序，但是该处理程序会向WER服务发送消息，并且服务会启动WER错误报告进程以显示错误报告对话框。



## 恶意诱饵：“您的赔偿”

9月17日，我们发现了一种新型攻击，该攻击从一个包含恶意文档的zip文件开始，该文档很可能是通过鱼叉式网络钓鱼攻击传播的。文档名叫“薪酬手册”（Compensation manual.doc），伪装成包含有关工人补偿权利的信息，恶意文档如下图所示。

该文件包含一个图像标签（“ INCLDEPICTURE ”），该图像标签连接到如下网址，然后下载一张图片作为文档模板。

下图为嵌入在文档中的图片标签（Image tag）及对应的“您的补偿”网站。

该域名于2020年6月5日注册，而文档创建时间为2020年6月12日，这很可能表明它们属于同一攻击。在其内部，我们看到一个恶意宏，该宏使用CactusTorch VBA模块的修改版来执行其Shellcode。CactusTorch正在利用DotNetToJscript技术将.Net编译的二进制文件加载到内存中，并从vbscript中执行。

下图显示了该威胁攻击者所使用的宏内容。它具有自动打开和自动关闭功能。AutoOpen只是显示一条错误消息，而AutoClose是执行函数的主体。

如上图所示，已经定义了一个十六进制格式的序列化对象，它包含一个正在加载到内存中的.Net有效负载（Payload）。然后，宏使用“ Kraken.Kraken”作为值定义了一个入口类。这个值有两个部分，用一个点分隔.net加载器的名称和它目标类的名称。

在下一步中，它将创建一个序列化的BinaryFormatter对象，并使用BinaryFormatter的deseralize函数反序列化该对象。最后，通过调用DynamicInvoke函数，从内存中加载并执行.Net有效负载（Payload）。

与CactusTorch VBA不同，它指定了目标进程在宏中注入Payload，该元素更改了宏并在.Net有效负载中指定目标进程。

---


## Kraken Loader

加载的Payload是一个名叫“ Kraken.dll”的.Net DLL，该文件编译于2020年06月12日。

这个DLL是一个加载器，它将嵌入的shellcode注入到WerFault.exe中。需要说明的是，这并不是此类技术的第一个例子。以前在使用NetWire RAT和Cerber勒索软件时就观察到了这种情况。下图展示了Kraken.dll，加载器包括两个主要的类：

<mark>**(1) Kraken类**</mark><br/> Kraken类包含了shellcode，这些代码将被注入到这个类中定义为“WerFault.exe”的目标进程中。它只有一个函数调用Loader类的Load函数，其shellcode和目标进程作为参数。

<mark>**(2) Loader类**</mark><br/> Loader类负责通过调用Windows API将shell代码注入到目标进程中。下图展示了load函数。

下面是它执行注入过程的步骤：

---


## ShellCode分析

使用HollowHunter，我们将注入的shellcode转储到WerFault.exe中，以便进行进一步分析。这个DLL在多个线程中执行其恶意活动，使其分析更加困难。这个DLL通过调用“Main”函数来执行“DllEntryPoint”。

主函数调用DllMain来创建一个线程，在同一进程上下文中的新线程中执行它的函数。<br/> <img alt="在这里插入图片描述" height="250" src="https://img-blog.csdnimg.cn/20201008214126589.png#pic_center" width="550"/>

<mark>DllMain函数如上图所示，创建的线程首先执行一些反分析检查，以确保它不在分析/沙箱环境或调试器中运行，它通过以下操作来实现的。</mark>

**(1) 通过调用GetTickCount来检查调试器的存在**<br/> GetTickCount是一种计时函数，用于度量执行某些指令集所需要的时间。在此线程中，它在睡眠（Sleep）指令之前和之后被调用两次，然后计算差值。如果不等于2，则程序退出，因为标识着它正在被调试。创建线程代码如下图所示。

**(2) VM检测**<br/> 在此函数中，它将通过提取显示驱动程序注册表项的提供程序名称来检查其是否在VMWare或VirtualBox中运行。

然后检查它是否包含字符串VMware或Oracle。

**(3) IsProcessorFeaturePresent**<br/> 此API调用用于确定是否支持指定的处理器特性。从下图可以看出，“ 0x17”已作为参数传递给此API，这意味着它在立即终止之前检查剩余的__fastfail支持。

**(4) NtGlobalFlag**<br/> shellcode代码检查PEB结构中的NtGlobalFlag来确定它是否正在被调试。为了识别调试器，它将NtGlobalFlag值与0x70进行比较。

**(5) IsDebuggerPresent**<br/> 通过调用“IsDebuggerPresent”来检查调试器是否存在。下图展示了 NtGlobalFlag 和 IsDebuggerPresent 检查。

在执行所有这些反分析检查之后，它进入一个函数，在一个新线程中创建最终的shellcode。通过调用“ Resolve_Imports”函数，可以动态混淆并解析在此部分中使用的导入调用。此函数使用LoadLibraryEx获取“kernel32.dll”的地址，然后在循环中检索12个导入。

使用libpeconv库，我们能够获得已解析的API调用表。下面是导入表，我们可以预期它将执行一些进程注入。

在解析了所需的API调用之后，它使用VirtualAlloc创建一个内存区域，然后调用下面的函数来解密最终shellcode的内容，并将它们写入创建的内存中。

在下一个步骤中，将调用VirtualProtect来更改对已分配内存的保护以使其可执行。最后，CreateThread被调用来在一个新线程中执行最后的shellcode。

---


## 最终的Shellcode

最终的shellcode是一组指令，这些指令向硬编码域发出HTTP请求，以下载恶意有效负载并将其注入到进程中。

<mark>第一步，它通过调用LoadLibraryA加载Wininet API。</mark>

<mark>第二步，构建函数调用列表所需的HTTP请求，包括InternetOpenA、InternetConnectA、InternetOpenRequestA和InternetSetOptionsExA。其中，HttpOpenRequestA如下图所示。</mark>

<mark>第三步，在准备好构建HTTP请求的需求之后，它将创建一个HTTP请求，并通过调用HttpSendrequestExA发送该请求。请求的网址是：</mark>

在下一步中，它将检查HTTP请求是否成功。如果HTTP请求不成功，它将调用ExitProcess停止其进程。

如果HTTPSendRequestExA的返回值为true，则表示请求成功，并且代码继续执行下一步。在此步骤中，它调用VirtualAllocExA分配内存区域，然后调用InternetReadFile读取数据并将其写入分配的内存。InternetReadFile调用如下图所示。

最后，它跳转到已分配内存的开头以执行它。这很有可能是另一个受感染的“asia-kotoba.net”网站上托管的shellcode，并在其中植入了伪造的图标。由于在报告时目标URL已关闭，因此我们无法检索此Shellcode进行进一步分析。

---


## 究竟是哪个APT组织的攻击呢？

我们没有足够的证据来确定这次攻击的原因。但是，我们发现其与APT32的松散联系，并且仍在调查中。

Malwarebytes阻止访问托管有效负载的受感染站点：

最后给出IOCs：

```
诱饵文件：
31368f805417eb7c7c905d0ed729eb1bb0fea33f6e358f7a11988a0d2366e942

包含诱饵文件的文档：
d68f21564567926288b49812f1a89b8cd9ed0a3dbf9f670dbe65713d890ad1f4

文档模板图片：
yourrighttocompensation[.]com/ping

存档文件下载URL：
yourrighttocompensation[.]com/?rid=UNfxeHM
yourrighttocompensation[.]com/download/?key=
15a50bfe99cfe29da475bac45fd16c50c60c85bff6b06e530cc91db5c710ac30&amp;id=0
yourrighttocompensation[.]com/?rid=n6XThxD
yourrighttocompensation[.]com/?rid=AuCllLU

下载URL的最终Payload：
asia-kotoba[.]net/favicon32.ico

```

最后希望这篇文章对您有所帮助，感觉反分析和沙箱逃逸部分知识挺有意思的，后续不忙可以尝试复现相关的功能。中秋节和国庆节结束，虽然一直在忙，大家接着加油。某人照顾好自己喔！

**前文分享：**

<font color="red">**2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、逆向分析、APT分析报告、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。**</font>

(By:Eastmount 2020-10-08 星期四 晚上11点写于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )
