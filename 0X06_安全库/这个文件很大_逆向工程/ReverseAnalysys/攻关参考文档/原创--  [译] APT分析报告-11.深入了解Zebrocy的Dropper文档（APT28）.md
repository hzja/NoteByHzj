# 原创
：  [译] APT分析报告：11.深入了解Zebrocy的Dropper文档（APT28）

# [译] APT分析报告：11.深入了解Zebrocy的Dropper文档（APT28）

这是作者新开的一个专栏，主要翻译国外知名安全厂商的APT报告，了解它们的安全技术，学习它们溯源APT组织和恶意代码分析的方法，希望对您有所帮助。当然，由于作者英语有限，会借助机翻进行校验，还请包涵！

<mark>前文分享了Lazarus攻击工业事件，主要采用ThreatNeedle高级恶意软件集群。这篇文章将详细讲解俄罗斯的APT组织Sofacy（APT28）中Zebrocy的Dropper文档。简短的文章读起来很轻松，希望对您有所帮助。</mark>

#### 文章目录

---


## 一.APT28介绍

`Sofacy` 是一个从2008年就一直处于活跃状态的APT组织，并在2016年美国总统选举黑客事件中崭露头角。该组织更广为人知的名字是 `FancyBear` 或 `APT28`，其攻击目标包括政府、J队、安全机构和私人组织等，并以参与黑客攻击和泄M行动而闻名。在过去的几年里，Sofacy进行了彻底的重组，在很大程度上避开了分析师的关注。其中一个比较典型的变体称为 `Zebrocy`，其目标主要集中在前苏联和亚洲周边国家。

> 
2018年，网络安全公司Palo AltoNetworks的研究人员就发现，与俄罗斯政府关系密切的SofacyAPT（又名APT28、Fancy Bear或Sednit）威胁组织使用Go编程语言编写了他们常用的Zebrocy工具的新版本。Zebrocy主要通过网络钓鱼攻击来实现，其中包含有宏和可执行文件附件的诱饵Microsoft Office文档。<br/><br/> 该恶意软件的幕后操纵者于2015年首次被发现与GreyEnergy具有高度关联性。GreyEnergy是一个威胁组织，被认为是BlackEnergy （又名Sandworm）的继任者，这表明它是一个与Sofacy和GreyEnergy关联的子组织。<mark>它实际上充当了一个后门和下载器，能够收集系统信息、操作文件、捕获屏幕截图并执行恶意命令，然后将这些恶意命令传输到攻击者控制的服务器。</mark><br/><br/> 在2020年，Sofacy利用COVID-19作为诱饵发送Zebrocy恶意软件发起在野攻击。虽然Zebrocy最初是用Delphi（称为Delphocy）编写的，但此后已被用六种语言实现，包括AutoIT，C ++，C＃，Go，Python和VB.NET。这篇文章将分享其Dropper的一个攻击案例。<br/><br/> <img alt="在这里插入图片描述" height="240" src="https://img-blog.csdnimg.cn/20210507153059249.png#pic_center" width="650"/>


在2021年3月，我们观察到了一系列针对哈萨克斯坦的攻击活动，它们使用Delphi编写的恶意软件 `Delphocy` 发起攻击，该恶意软件与以前的 `Zebrocy` 有关。被发现的Word文档据称来自哈萨克斯坦一家名为Kazchrome的公司，该公司是一家采矿和金属公司，并且是全球最大的铬矿和铁合金生产商之一。

**总体来说，我们发现了6个与这个集群相关的Delphocy Word文档，它们都包含相同的删除PE文件的VBA脚本**。在这六份Word文件中，有两份似乎是来自哈萨克斯坦的受害者上传到VirusTotal的真实文件。上传的文件包含了原始文件名：

<font color="red">在这篇文章中，我们将深入研究这些样本，并分享一些其他分析师用来逆向Delphocy dropper文档的技术。我们展示了研究人员如何绕过密码保护宏，并描述了如何使用IDR（交互式Delphi重构器）反编译Delphi，以及如何使用dhrake的插件将保存的IDC文件导入Ghidra。</font>

我们的分析结果引导发现了更多的Zebrocy簇，提供了一组IOC和YARA检测规则，使威胁搜索程序能够在其环境中搜索和这些相关的工件。

---


## 二.绕过VBA宏密码保护

在使用VBA宏分析Office文档时，攻击者有许多不同的工具和技术来完成这项工作，但我已经养成了一个习惯。当我第一次开始逆向恶意软件时，在手动绕过受密码保护的宏中，我就使用了这个习惯。

步骤如下：

当打开Word文档并查看宏时，您可以看到脚本和表单。在分析函数时，最突出的地方是：

这表明UserForm1采用base64编码。

---


## 三.Wininition用户表单

当选择UserForm1时，文本框将显示一个 `base64` 编码的字符串，这正是我们上面讨论过的函数。下一步是将整个字符串复制到一个文件中，以便对其进行解码。

现在我们通过base64解码二进制文件并将其保存为wininition.exe。

接下来，使用HxD清理头部（headers），然后使用PE-Bear修改块头部（fix the sections headers），以进入下一阶段的分析。

当对一个二进制文件进行分类时，首要工具是Hiew，它可以调查并寻找线索以便进行更深层次的理解。通过 `winintion`，我注意到Embarcadero字符串，<mark>这意味着这个二进制文件是用Delphi编写的</mark>。在逆向Delphi二进制文件时，我总是使用IDR（交互式Delphi重构器）。其中，IDR是一个用Delphi编写的可执行文件和动态库（DLL）的反编译器。

---


## 四.用Ghidra和dhrake逆向Delphi二进制文件

在搜索IDR的最新进展时，我们遇到了一个很棒的 `Ghidra` 插件。它是一个用在Ghidra中的脚本集合，使用IDR输出到IDC的Delphi二进制文件进行逆向工程。虽然这是一年前发表的，但如果攻击者使用Ghidra，它就是一个宝藏。

dhrake允许您将IDC文件从IDR导入Ghidra。这将导入符号名、函数签名并为Delphi类创建结构。该插件从IDR生成的IDC文件中提取并应用Delphi符号，并且试图查找Ghidra错误确定函数入口点的情况。如果您从未将插件导入Ghidra，请阅读另一篇文章。

这那篇文章中，我已经把IDC保存到一个选定的文件夹，然后将插件安装在Ghidra中，并运行提示输入IDC文件的脚本将其加载！

在wininit二进制文件中，WinMain的第一个函数是 `SetWindowsHookExW`，这是一个钩子程序（hook procedures），**用来监视系统中某些类型的事件**。钩子进程的低级键盘输入事件是 `WH_KEYBOARD_LL`，其参数中的数字是13。<mark>该钩子是一种拦截按键事件的机制，然后将所有事件保存到一个日志文件中，并将其发送到C2。</mark>

C2使用十六进制进行模糊处理，十六进制可以转换为ascii。

注意：这些域名似乎是被盗用的域名。

---


## 五.结论

通过对这些文件的分析，我们找到了其他的Zebrocy集群。随着Zebrocy的范围不断发展，组织必须具有适当的可见性和检测能力来发现这个威胁行动者。我们希望在这篇文章中讨论的技术将有助于其他研究人员分析Delphocy droppper文档，特别是文档中有密码保护的宏。

此外，为了阻止此类攻击，CISA建议在使用可移动媒体打开来自未知发件人的电子邮件和附件，以及扫描可疑的电子邮件附件时，请确保谨慎行事，并确保扫描附件的扩展名与文件头匹配。

**前文分享：**

<font color="red">**2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、逆向分析、APT分析报告、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。**</font>

(By:Eastmount 2021-05-07 星期五 晚上7点写于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


## IOC指标

**Word文件**

```
SHA256
3b548a851fb889d3cc84243eb8ce9cbf8a857c7d725a24408934c0d8342d5811
1dd03c4ea4d630a59f73e053d705185e27e2e2545dd9caedb26a824ac5d11466
1e8261104cbe4e09c19af7910f83e9545fd435483f24f60ec70c3186b98603cc
c213b60a63da80f960e7a7344f478eb1b72cee89fd0145361a088478c51b2c0e
2bf088955007b4f47fe9187affe65fffea234ff16596313a74958a7c85129172
d9e7325f266eda94bfa8b8938de7b7957734041a055b49b94af0627bd119c51c

SHA1
fc0b7ad2ae9347d6d2ababe2947ffb9f7cc73030
71b4b9f105de94090fc36d9226faaa1db6d9f3d1
6a8f63c4491adcf2cf7f76cd1481c5647615a6c9
a3ecf1fdc1206e9d3061530fa91775cf3d97f788
ae01ca2cf0dc07abb3a7bef9930e38c9212975d5
66b39f4fd1dd51c2f548330e5818f732dad0aa28

```

**VBA**

```
SHA256
a442135c04dd2c9cbf26b2a85264d31a5ac4ec5d2069a7b63bc14b64a6dd82b7

SHA1
6ec4eb883752b70db134ac0f4e0d5b4a77196184

```

**Wininition**

```
SHA256
ee7cfc55a49b2e9825a393a94b0baad18ef5bfced67531382e572ef8a9ecda4b

SHA1
afbdb13d8f620d0a5599cbc7a7d9ce8001ee32f1

```

**网址**

```
hxxps://www.xbhp[.]com/dominargreatasianodyssey/wp-content/plugins/akismet/style.php
hxxps://www.c4csa[.]org/includes/sources/felims.php

```

**Yara Rules**

```
rule apt_RU_delphocy_encStrings {
  meta:
    desc = "Hex strings in Delphocy drops"
    author = "JAG-S @ SentinelLabs"
    version = "1.0"
    TLP = "White"
    last_modified = "04.09.2021"
    hash0 = "ee7cfc55a49b2e9825a393a94b0baad18ef5bfced67531382e572ef8a9ecda4b"
    hash1 = "07b2d21f4ef077ccf16935e44864b96fa039f2e88c73b518930b6048f6baad74"

  strings:
    $enc_keylogger2 = "5B4241434B53504143455D" ascii wide
    $enc_keylogger3 = "5B5441425D" ascii wide
    $enc_keylogger4 = "5B53484946545D" ascii wide
    $enc_keylogger5 = "5B434F4E54524F4C5D" ascii wide
    $enc_keylogger6 = "5B4553434150455D" ascii wide
    $enc_keylogger7 = "5B454E445D" ascii wide
    $enc_keylogger8 = "5B484F4D455D" ascii wide
    $enc_keylogger9 = "5B4C4546545D" ascii wide
    $enc_keylogger10 = "5B55505D" ascii wide
    $enc_keylogger11 = "5B52494748545D" ascii wide
    $enc_keylogger12 = "5B444F574E5D" ascii wide
    $enc_keylogger13 = "5B434150534C4F434B5D" ascii wide
    $cnc1 = "68747470733A2F2F7777772E786268702E636F6D2F646F6D696E61726772656174617369616E6F6479737365792F77702D636F6E74656E742F706C7567696E732F616B69736D65742F7374796C652E706870" ascii wide
    $cnc2 = "68747470733A2F2F7777772E63346373612E6F72672F696E636C756465732F736F75726365732F66656C696D732E706870" ascii wide

  condition:
    uint16(0) == 0x5a4d and (any of ($cnc*) or all of ($enc_keylogger*))
}

rule apt_RU_Delphocy_Maldocs {
  meta:
    desc = "Delphocy dropper docs"
    author = "JAG-S @ SentinelLabs"
    version = "1.0"
    TLP = "White"
    last_modified = "04.09.2021"
    hash1 = "3b548a851fb889d3cc84243eb8ce9cbf8a857c7d725a24408934c0d8342d5811"
    hash2 = "c213b60a63da80f960e7a7344f478eb1b72cee89fd0145361a088478c51b2c0e"
    hash3 = "d9e7325f266eda94bfa8b8938de7b7957734041a055b49b94af0627bd119c51c"
    hash4 = "1e8261104cbe4e09c19af7910f83e9545fd435483f24f60ec70c3186b98603cc"

  strings:
    $required1 = "_VBA_PROJECT" ascii wide
    $required2 = "Normal.dotm" ascii wide
    $required3 = "bin.base64" ascii wide
    $required4 = "ADODB.Stream$" ascii wide
    $author1 = "Dinara Tanmurzina" ascii wide
    $author2 = "Hewlett-Packard Company" ascii wide
    $specific = "Caption         =   \"\\wininition.exe\"" ascii wide
    $builder1 = "Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} UserForm1" ascii wide
    $builder2 = "{02330CFE-305D-431C-93AC-29735EB37575}{33D6B9D9-9757-485A-89F4-4F27E5959B10}" ascii wide
    $builder3 = "VersionCompatible32=\"393222000\"" ascii wide
    $builder4 = "CMG=\"1517B95BC9F7CDF7CDF3D1F3D1\"" ascii wide
    $builder5 = "DPB=\"ADAF01C301461E461EB9E2471E616F01D06093C59A7C4D30F64A51BDEDDA98EC1590C9B191FF\"" ascii wide
    $builder6 = "GC=\"4547E96B19021A021A02\"" ascii wide

  condition:
    uint32(0) == 0xE011CFD0 and all of ($required*) and (all of ($author*) or $specific or 5 of ($builder*))
}

```

---


参考文献：
