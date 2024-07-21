# 原创
：  [论文阅读] (16)Powershell恶意代码检测论文总结及抽象语法树（AST）提取

# [论文阅读] (16)Powershell恶意代码检测论文总结及抽象语法树（AST）提取

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇介绍分享英文论文审稿意见及应对策略的学习笔记。这篇文章将简单总结Powershell恶意代码检测相关研究，并结合开源工具分享抽象语法树提取过程。希望自己能在科研路上不断前行，不断分享更高阶的科研心得，发表更高质量的文章。一方面自己英文太差，只能通过最土的办法慢慢提升，另一方面是自己的个人学习笔记，并分享出来希望大家批评和指正。希望这篇文章对您有所帮助，这些大佬是真的值得我们去学习，献上小弟的膝盖~fighting！**</font>

#### 文章目录

当然，作者能力有限，只能结合自己的实力和实际阅读情况出发，也希望自己能不断进步，每个部分都会持续补充。可能五年十年后，也会详细分享一篇英文论文如何撰写，目前主要以学习和笔记为主。大佬还请飘过O(∩_∩)O

**前文赏析：**

---


## 一.Powershell恶意代码检测

<mark>近年来，Powershell 由于其易用性强、隐蔽性高的特点被广泛应用于 APT 攻击中，传统的基于人工特征提取和机器学习方法的恶意代码检测技术在 Powershell 恶意代码检测中越来越难以有效。该部分以学术界论文为主，介绍Powershell恶意代码检测相关现状。</mark>

当你打开DBLP或知网，会发现Powershell相关的文章很少，主要以那篇CCS解混淆为主。这里，大家不妨也读读JavaScript恶意代码相关的论文。下面介绍比较相关的几个工作（排名不分先后）。

---


### 1.Light-Weight Deobfuscation and Semantic-Aware Attack Detection（CCS19 反混淆）

> 
<mark>Zhenyuan Li, et al. Effective and Light-Weight Deobfuscation and Semantic-Aware Attack Detection for PowerShell Scripts. CCS, 2019: 1831-1847.</mark>



近年来，PowerShell 越来越多地出现在各种网络攻击中，从高级持续威胁、勒索软件、网络钓鱼电子邮件、加密劫持、金融威胁到无文件攻击。但是，由于 PowerShell 语言在设计上是动态的，并且可以在不同级别构建脚本片段，因此基于最新静态分析的 PowerShell 攻击检测方法本质上容易受到混淆。

为了克服这一挑战，**本文为 PowerShell 脚本设计了第一个有效且轻量级的反混淆方法**。为了解决精确识别可恢复脚本片段的挑战，设计了一种新颖的基于子树的反混淆方法。该方法在 PowerShell 脚本的抽象语法树（`Abstract Syntax Tree`）中的子树级别执行混淆检测和基于仿真的恢复。

**基于新的反混淆方法，我们进一步设计了第一个语义感知的 PowerShell 攻击检测系统**。为了实现基于语义的检测，我们利用了经典的面向目标的关联挖掘算法，并为 PowerShell 攻击新识别了 31 个语义签名。我们对 2342 个良性样本和 4141 个恶意样本的集合进行了评估，发现我们的去混淆方法平均耗时不到 0.5 秒，同时将混淆后的脚本与原始脚本之间的相似度从仅 0.5% 提高到 80% 左右，这因此既有效又轻便。

此外，应用我们的去混淆处理后，**Windows Defender 和 VirusTotal 的攻击检测率分别从 0.3% 和 2.65% 大幅提高到 75.0% 和 90.0%**。同时，当应用我们的去混淆处理时，我们的语义感知攻击检测系统以 92.3% 的平均真阳性率和 0% 的假阳性率优于 Windows Defender 和 VirusTotal。

> 
**评价**：这篇文章写得非常棒，值得大家学习。之后，由于去混淆做得非常好，关于Powershell的恶意检测文章很少。同时，文章的摘要非常棒，推荐给大家。


---


### 2.字符级CNN（AsiaCCS18）

> 
Danny Hendler, et al. Detecting Malicious PowerShell Commands using Deep Neural Networks. AsiaCCS, 2018: 187-197.



Microsoft 的 PowerShell 是一种命令行 shell 和脚本语言，默认安装在 Windows 机器上。它基于微软的.NET 框架，包括一个允许程序员访问操作系统服务的接口。虽然管理员可以配置 PowerShell 以限制访问和减少漏洞，但可以绕过这些限制。此外，PowerShell 命令可以轻松地动态生成、从内存中执行、编码和混淆，从而使 PowerShell 执行的代码的日志记录和取证分析具有挑战性。

由于这些原因，PowerShell 越来越多地被网络犯罪分子用作其攻击工具链的一部分，**主要用于下载恶意内容和横向移动**。事实上，赛门铁克最近一份关于 PowerShell 被网络犯罪分子滥用的综合技术报告报告称，他们收到的恶意 PowerShell 样本数量以及使用 PowerShell 的渗透工具和框架的数量急剧增加。这凸显了开发检测恶意 PowerShell 命令的有效方法的迫切需要。

在这项工作中，我们通过实施几个新颖的恶意 PowerShell 命令检测器并评估它们的性能来应对这一挑战。**我们实现了基于“传统”自然语言处理 (NLP) 的检测器和基于字符级卷积神经网络 (CNN) 的检测器**。使用大型真实数据集评估检测器的性能。

评估结果表明，尽管我们的检测器（尤其是传统的基于 NLP 的检测器）单独产生了高性能，但将基于 NLP 的分类器与基于 CNN 的分类器相结合的集成检测器提供了最佳性能，因为后者的分类器是能够检测成功规避前者的恶意命令。通过对这些规避命令的分析表明，CNN 分类器自动检测到的一些混淆模式本质上很难使用我们应用的 NLP 技术检测到。我们的检测器提供高召回值，同时保持非常低的误报率，使我们谨慎乐观地认为它们具有实用价值。

> 
**评价**：分别利用NLP和4层CNN实现Powershell检测，方法比较简单，但由于其2018年完成，做得比较早，仍能为我们提供一种思路。


---


### 3.AMSI-Based Detection Using Contextual Embeddings（Token-Char架构 AsiaCCS20）

> 
<mark>Danny Hendler, et al. AMSI-Based Detection of Malicious PowerShell Code Using Contextual Embeddings. AsiaCCS, 2020: 679-693</mark>



**继上一个工作后，这个团队在2020年提出了一种融合字符级和Token级的深度学习方法，并运用于AMSI较真实的数据集中。**

PowerShell 是一个命令行 shell，支持脚本语言。它在组织中广泛用于配置管理和任务自动化，但也越来越多地用于对组织发起网络攻击，主要是因为它预装在 Windows 机器上并暴露出可能被攻击者利用的强大功能。这使得检测恶意 PowerShell 代码的问题既紧迫又具有挑战性。 `Microsoft反恶意软件扫描接口 (Microsoft’s Antimalware Scan Interface， AMSI)` 内置于 Windows 10 中，允许防御系统在执行之前扫描传递给脚本引擎（如 PowerShell）的所有代码。在这项工作中，我们使用 AMSI 提供的信息对恶意 PowerShell 代码检测进行了首次研究。

**本文提出几种新颖的基于深度学习的恶意 PowerShell 代码检测器，它们使用来自 PowerShell 语言的单词预训练上下文嵌入（Contextual Embeddings）。上下文词嵌入能够将语义相似的词投影到嵌入空间中的近似向量**。网络安全领域的一个已知问题是，与未标记数据相比，标记数据相对稀缺，因此难以设计对多种类型的恶意活动的有效监督检测。 PowerShell 代码也是如此。我们的工作表明，可以通过学习基于未标记数据的预训练上下文嵌入来缓解这个问题。

本文使用 AMSI 收集的真实数据来训练和评估我们的模型。上下文嵌入是使用从公共存储库收集的大量未标记的 PowerShell 脚本和模块来学习的。我们的性能分析表明，使用未标记的数据进行嵌入显着提高了检测器的性能。我们表现​​最佳的模型使用的架构能够处理来自字符和标记级别的文本信号，并获得接近 90% 的真阳性率，同时保持低于 0.1% 的低假阳性率。

> 
**评价**：本文的创新在于提出了Token-Char模型架构，并进行了详细的对比实验，证明了方法的有效性，推荐大家学习该论文的施压描述。


---


### 4.FC-PSDS检测方法（RF+DNN 信安学报）

> 
<mark>刘岳, 刘宝旭, 等. 基于特征组合的Powershell恶意代码检测方法[J]. 信息安全学报, 2021, 6(1): 40-53.</mark>



本文提出了一种基于随机森林特征组合和深度学习的Powershell 恶意代码检测方法。该方法使用随机森林生成更好表征原始数据的新特征组合，随后使用深度学习神经网络训练并进行分类识别。该方法可以弥补人工特征工程经验不足的问题，更好表征原始数据从而提高检测效果。

FC-PSDS 方法设计的目标是使用随机森林和深度神经网络检测恶意 Powershell 代码。通过混淆去除和标准化从 Powershell 代码中抽取特征组成原始特征集，根据集成学习思想使用原始特征集生成对原始数据表达更有效的新特征组合，并据此训练深度神经网络分类器模型，以此解决传统方法对于Powershell恶意代码的检测困难，以及冗余特征导致的模型检测能力较差的问题。

实验结果如下图所示：

> 
**评价**：这是来自中科院刘老师他们团队的工作，主要使用随机森林生成更好表征原始数据的新特征组合，再接深度学习完成恶意性识别，文章实验也比较充分，国内为数不多的几篇Powershell文章。


---


### 5.基于混合特征的检测模型（Neurocomputing21）

> 
<mark>Yong Fang, Xiangyu Zhou, Cheng Huang. Effective method for detecting malicious PowerShell scripts based on hybrid features. Neurocomputing, 448: 30-39 (2021).</mark>



目前，网络攻击在互联网世界猖獗，黑客的攻击方式也在不断变化。 PowerShell是一种基于命令行和.NET框架的编程语言，功能强大，兼容性好。因此，黑客经常使用 PowerShell 恶意脚本来攻击 APT 攻击中的受害者。当这些恶意 PowerShell 脚本被执行时，黑客可以控制受害者的计算机或在他们的计算机上留下后门。

**本文提出了一种基于混合特征的恶意PowerShell脚本检测模型，分析了恶意样本和良性样本在文本字符、函数、标记和抽象语法树节点方面的差异**。

在实验中，将恶意脚本插入到良性脚本中，以削弱恶意样本在抽象语法树节点和令牌层面的特征，使脚本更加复杂。即使在如此复杂的数据集中，所提出的基于混合特征的模型在五重交叉验证中仍能达到 97.76% 的准确率。此外，该模型在原始脚本上的准确率为 98.93%，这意味着该模型具有对复杂脚本进行分类的能力。

> 
**评价**：这是来自四川大学黄老师团队的工作，主要提出了一种基于混合特征的恶意PowerShell脚本检测模型，并且相关工作对比值得大家学习。


---


### 6.AA-PSFC检测方法（BiGRU 武大学报）

> 
<mark>基于深度学习的PowerShell恶意代码家族分类研究[J]. 武汉大学学报（理学版）, 2022(1)</mark>



由于PowerShell具备隐蔽性高、易用性好、运行环境简单等特点，近年来已被广泛应用于高级持续性威胁攻击中。对PowerShell恶意代码进行基于功能的家族分类是检测其新型变异代码的关键。

针对已有工作主要集中于PowerShell代码的恶意性检测，缺乏对其功能层面深入挖掘的问题，提出了一种基于功能类型的 PowerShell 恶意代码家族分类方法。该方法通过构建双向门控循环网络与注意力机制提取PowerShell恶意代码的上下文语义信息，利用 PowerShell恶意代码的语义特征实现家族分类。

实验结果如下图所示：

> 
**评价**：这是来自武汉大学彭老师团队的工作，主要结合抽象语法树和BiGRU识别Powershell恶意家族，文章实验也比较充分，国内为数不多的几篇Powershell文章。


---


### 7.POSTER（AST+DNN CCS18）

> 
<mark>Gili Rusak, et al. AST-Based Deep Learning for Detecting Malicious PowerShell. CCS, 2018: 2276-2278.</mark>



随着深度学习的发展，出现了利用神经网络或字符级卷积神经网络检测混淆的Powershell恶意命令的方法，虽然这些表达能显示Powershell的属性，但本文假设静态程序分析的工具会更加有效。基于此，提出一种结合传统程序分析（抽象语法树）和深度学习混合得方法，下图展示了基本步骤：

> 
**评价**：提出一种基于语法树的Powershell检测思路，仅3页内容更像workshop。


---


### 8.PSDEM（反混淆 ISCC18）

> 
<mark>Chao Liu, et al. PSDEM: A Feasible De-Obfuscation Method for Malicious PowerShell Detection. ISCC, 2018: 825-831.</mark>



PowerShell 非常强大，我们已经看到攻击者最近在他们的攻击方法中越来越多地使用 PowerShell。在大多数情况下，PowerShell 恶意软件通过垃圾邮件传播，使用 Microsoft Word 文档组合以致命有效的负载感染受害者。如今，PowerShell的去混淆和分析仍然是基于人工分析。然而，随着恶意样本和混淆方法数量的快速增长，速度太慢，无法满足需求。

本文提出了一种称为 PSDEM 的 PowerShell 去混淆方法，该方法具有两层去混淆以获得原始 PowerShell 脚本。一种是从许多混淆的文档代码中提取 PowerShell 脚本。另一个是反混淆脚本，包括编码、字符串操作和代码逻辑混淆。同时，我们设计了一种基于PSDEM的Word文档中恶意PowerShell脚本的自动反混淆分析工具。

我们从去混淆的准确性和时间效率方面测试了该工具的性能，评估结果表明它具有令人满意的性能。 PSDEM 提高了分析 Word 文档中恶意 PowerShell 脚本的效率和准确率，并为安全专家提供了进一步分析以获取有关攻击的更多信息的途径。

> 
**评价**：设计了一种基于PSDEM的Word文档中恶意PowerShell脚本的自动反混淆分析工具，并与现有检测工具进行对比分析。


---


### 9.PowerDrive（动静态反混淆 DIMVA19）

> 
Denis Ugarte, et al. PowerDrive: Accurate De-obfuscation and Analysis of PowerShell Malware. DIMVA，2019: 240-259.



如今，PowerShell 是一种广泛使用的技术，用于管理和管理基于 Windows 的操作系统。但是，恶意软件向量也广泛使用它来执行有效负载或删除其他恶意内容。与恶意软件使用的其他脚本语言类似，PowerShell 攻击由于广泛使用多个混淆层而难以分析，这使得真正的恶意代码难以被发现。据我们所知，目前还缺少用于正确消除此类攻击的综合解决方案。

在本文中，我们介绍了 PowerDrive，**一种用于 PowerShell 攻击的开源、静态和动态多阶段去混淆器**。 PowerDrive 检测 PowerShell 代码，通过向分析师展示所采用的混淆步骤来逐步对其进行去混淆。

我们使用 PowerDrive 成功分析了从各种恶意软件向量和可执行文件中提取的数千个 PowerShell 攻击。获得的结果显示了攻击者用来设计恶意脚本的有趣模式。此外，我们提供了分析代码所采用的行为模型的分类以及分析期间联系的恶意域的综合列表。

> 
**评价**：提出一种用于 PowerShell 攻击的开源、静态和动态多阶段去混淆器，并评价了Powershell攻击各个阶段（常见六个）的性能，用VirusTotal score验证。


PS：还有一些工作不再详细介绍，请读者搜索并阅读相关论文。

---


## 二.抽象语法树之deobshell

前面的论文提到抽象语法树，它能将Powershell抽象成树状结构，常见方法是使用接口或编写自定义程序实现，下面介绍其中一种方法。希望对大家有所帮助。

PowerShell 的抽象语法树作为代码的语义表达，以多叉树的形式表示脚本功能的逻辑结构，保<br/> 留了代码上下文的特征并剔除无关的参数干扰，是分析功能类似的PowerShell代码的有效方法 。Windows 为PowerShell提供了访问脚本AST的接口，使用内置接口获取的 AST 结构如图所示。

下面是Deobshell的介绍。

> 
<font color="red">DeobShell 是使用 Python 中的抽象语法树 (AST) 操作对 Powershell 进行反混淆的 PoC。通过调用System.Management.Automation.Language.Parser 相关节点并将相关节点写入 XML 文件，使用 Powershell 脚本提取 AST。</font>


AST 操作和优化基于一组规则（例如：concat 常量字符串、应用格式运算符…）。从去混淆的 AST 中，使用 Python 重建一个 ps1 脚本。见下图。

规则示例：

格式运算符的 BinaryExpressionAst 节点示例。

输入：

```
&lt;BinaryExpressionAst Operator="Format" StaticType="System.Object"&gt;
  &lt;StringConstantExpressionAst StringConstantType="DoubleQuoted" StaticType="string"&gt;{0}{1}&lt;/StringConstantExpressionAst&gt;
  &lt;ArrayLiteralAst StaticType="System.Object[]"&gt;
    &lt;Elements&gt;
      &lt;StringConstantExpressionAst StringConstantType="SingleQuoted" StaticType="string"&gt;c&lt;/StringConstantExpressionAst&gt;
      &lt;StringConstantExpressionAst StringConstantType="SingleQuoted" StaticType="string"&gt;AcA&lt;/StringConstantExpressionAst&gt;
    &lt;/Elements&gt;
  &lt;/ArrayLiteralAst&gt;
&lt;/BinaryExpressionAst&gt;

```

输出：

```
&lt;StringConstantExpressionAst StringConstantType="SingleQuoted" StaticType="string"&gt;cAcA&lt;/StringConstantExpressionAst&gt;

```

data.zip 中提供了更多示例，比如一个CTF去混淆的示例。

输入：

```
$mRSp73  =  [ChaR[] ]" ))43]raHc[]gNIRtS[,)38]raHc[+98]raHc[+611]raHc[((eCAlper.)421]raHc[]gNIRtS[,'5IP'(eCAlper.)'$',)09]raHc[+99]raHc[+701]raHc[((eCAlper.)93]raHc[]gNIRtS[,'vzW'(eCAlper.)'


2halB.tcejboZck tuptuO-etirW

7halB.tcejboZck +'+' 6halB.tcejboZck + halB.tc'+'ejboZck '+'= 2galFFT'+'C:'+'vneZck

SYt!eciNSYt = 1galFFTC:vneZck

SYt...aedi dab yre'+'v'+' ,yre'+'v a yllacipyt svzWtaht ,ton fI .ti gninnur erofeb siht detacsufbo-ed uoy epoh ISYt eulaV- 2halB emaN- '+'ytreporPetoN epy'+'TrebmeM- rebmeM-ddA 5IP tcejboZck

SYt'+'.uoy tresed dna dnuora nur annog reveNSYt eulaV- 9hal'+'B emaN- ytreporPetoN epyTrebmeM- rebmeM-ddA 5'+'IP tcejboZck

SYt.nwod uo'+'y tel annog '+'re'+'veN .'+'pu uoy evig annog reveNSYt eulaV- 8halB emaN- ytreporPetoN epyTrebm'+'eM- rebmeM-d'+'dA 5IP tcejboZck

SYt}f1j9kdSYt eulaV- 7halB emaN- y'+'treporPetoN ep'+'yTrebmeM- rebmeM-ddA 5IP tcejboZck

SYtg4lf_3ht_t0nSYt eulaV- 4halB emaN- yt'+'reporPetoN epyTrebmeM- rebmeM-ddA 5IP tcejboZck

SYt1#f!J{SYt eulaV- 6halB emaN- ytreporPetoN epyTrebmeM- rebmeM-'+'ddA 5IP tcejboZck

SYtgalF,ehT,toN,oslASYt eulaV- 5halB emaN- ytreporPetoN epyTrebmeM- rebmeM-ddA 5IP tcejboZck

SY'+'t}fdjfkslfdSYt eulaV- 3halB emaN- ytrepor'+'PetoN e'+'pyTrebmeM- rebmeM-ddA 5IP tcejboZ'+'ck

SYtgalfSYt eulaV- halB em'+'aN- ytreporPetoN e'+'pyTrebmeM- rebmeM-ddA 5IP tcej'+'boZck

tc'+'ejbO'+'SP tcejbO-weN = tc'+'ejboZck'( ()''nioJ-'x'+]3,1[)eCNERefErpESoBreV$]GniRTS[( (. " ;[aRRAy]::REVerse($MrSp73);. ( 'IeX') ( -JoiN$MrSp73)

```

输出：

```
$object = New-Object PSObject;
$object | Add-Member  NoteProperty  Blah  "flag";
$object | Add-Member  NoteProperty  Blah3  "dflskfjdf}";
$object | Add-Member  NoteProperty  Blah5  "Also,Not,The,Flag";
$object | Add-Member  NoteProperty  Blah6  "{J!f`#1";
$object | Add-Member  NoteProperty  Blah4  "n0t_th3_fl4g";
$object | Add-Member  NoteProperty  Blah7  "dk9j1f}";
$object | Add-Member  NoteProperty  Blah8  "Never gonna give you up. Never gonna let you down.";
$object | Add-Member  NoteProperty  Blah9  "Never gonna run around and desert you.";
$object | Add-Member  NoteProperty  Blah2  "I hope you de-obfuscated this before running it. If not, that''s typically a very, very bad idea...";
$env:CTFFlag1 = "Nice!";
$env:CTFFlag2 = $object.Blah + $object.Blah6 + $object.Blah7;
Write-Output $object.Blah2;

```

---


## 三.AST抽取实例操作

下面简单复现DeobShell工具。

### 1.环境部署

首先是配置环境。

下载并解压Deobshell。

> 
注意，这里的代码有个新的规则，叫海象运算，是python 3.8正式版更新的，PEP572中的海象运算符获得正式python版本的支持。大家需要配置该版本以上的环境。<br/><br/> <img alt="在这里插入图片描述" src="https://img-blog.csdnimg.cn/1976dd1009e1410c9352d8d4a3a58d55.png#pic_center"/><br/> 海象运算的规则如下：<br/> <img alt="在这里插入图片描述" src="https://img-blog.csdnimg.cn/f21829f589c7473aa7eb5985caa6920d.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center"/>


接着，设置环境变量。

作者配置了最新的Python版本。

---


### 2.AST提取

接下来简单看看整个代码文件夹。

运行过程是在Powershell中执行main.py文件，并附带两个参数，其中“-i”表示输入，对应的为混淆原始PS文件。

它会首先生成抽象语法树AST，然后执行反混淆操作。如下图所示：

核心代码如下：

```
# coding=utf-8
import os
import subprocess
# noinspection PyPep8Naming
import xml.etree.ElementTree as ET

from modules.logger import log_info, log_debug, log_err


def read_ast_file(filename):
    log_info(f"Reading input AST: {filename}")
    try:
        ast = ET.parse(filename)
        return ast
    except IOError as e:
        log_err(e.args[1])
        return None
    except Exception as e:
        log_err(str(e))
        return None


def create_ast_file(ps1_file):
    log_info(f"Creating AST for: {ps1_file}")

    cmd = ["PowerShell", "-ExecutionPolicy", "Unrestricted", "-File",
           os.path.abspath(os.path.join("tools", "Get-AST.ps1")),
           "-ps1", os.path.abspath(ps1_file)]

    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)

    for line in result.stdout.splitlines():
        log_debug(line)

    return result.returncode == 0

```

---


### 3.代码解析及结果

接着我们看看主函数代码，主要通过类和多个文件、函数调用实现。开源代码真心值得大家学习。

```
# coding=utf-8
import os
import pathlib
import sys

from modules.ast import create_ast_file, read_ast_file
from modules.logger import set_log_level, LogLevel
from modules.optimize import Optimizer
from modules.rebuilder import Rebuilder
from modules.utils import welcome

OPTIONS = {}

def usage():
    print("Usage: " + os.path.basename(sys.argv[0]) + ' [options]')
    print()
    print("Command:")
    print("      deob            : Deobfuscate PowerShell script")
    print("      format          : Format PowerShell script")
    print()
    print("Options:")
    print("      -h, --help      : Show help")
    print("      -i, --in        : Input .ps1 file")
    print()
    sys.exit(0)


def parse_args():
    global OPTIONS
    i = 1
    while i &lt; len(sys.argv):
        if sys.argv[i] in ["-h", "--help"]:
            usage()
        elif sys.argv[i] in ["-i", "--in"]:
            OPTIONS["input"] = sys.argv[i + 1]
            i += 1
        else:
            OPTIONS["command"] = sys.argv[i]
        i += 1


def deob(ps1_file):
    p = pathlib.Path(ps1_file)

    if create_ast_file(p):

        if ast := read_ast_file(p.with_suffix(".xml")):

            o = Optimizer()
            o.optimize(ast)

            with open(p.with_suffix(".deob.xml"), "wb") as output:
                ast.write(output)

            r = Rebuilder(p.with_suffix(".deob.ps1"))
            r.rebuild(ast.getroot())


def format(ps1_file):
    p = pathlib.Path(ps1_file)

    if create_ast_file(p):

        if ast := read_ast_file(p.with_suffix(".xml")):

            r = Rebuilder(p.with_suffix(".formatted.ps1"))
            r.rebuild(ast.getroot())


def main():
    cmd = OPTIONS.setdefault("command", None)
    if cmd == "deob":
        deob(OPTIONS['input'])
    elif cmd == "format":
        format(OPTIONS['input'])
    else:
        usage()


if __name__ == '__main__':
    welcome()
    set_log_level(LogLevel.DEBUG)
    parse_args()
    print("main")
    main()

```

最后运行结果如下图所示：

<mark>**(1) 抽象语法树生成**</mark><br/> test001.ps1为输入文件，即原始PS文件。

test001.xml为生成的AST，为XML文件。

<mark>**(2) 反混淆**</mark><br/> 如果文件带混淆，会执行反混淆操作。

输出结果如下图所示：

> 
提醒：但作者尝试几个复杂的混淆PS文件，其效果较差，建议代价完善代码，遗憾的是CCS那篇未开源。同时，如果读者仅想得到AST，该方法还不错，另官方提供的接口也是一种不错的方法。建议读者深入研究。


---


## 四.总结

写到这里，这篇文章就介绍结束。首先，感谢参考文献中的各位老师，其次感恩大家在科研和读博路上的陪伴。希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，真心推荐原文。

学安全两年，认识了很多安全大佬和朋友，希望大家一起进步。同时非常感谢参考文献中的大佬们，感谢老师、实验室小伙伴们的教导和交流，深知自己很菜，得努力前行。感恩遇见，且行且珍惜，小珞珞太可爱了，哈哈。

**欢迎大家讨论，是否觉得这系列文章帮助到您！任何建议都可以评论告知读者，共勉。**

(By:Eastmount 2022-03-15 夜于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


**能看到这里的读者，我说声诚恳的感谢。作者再给一个福利，推荐几个Powershell宝藏网站。**
