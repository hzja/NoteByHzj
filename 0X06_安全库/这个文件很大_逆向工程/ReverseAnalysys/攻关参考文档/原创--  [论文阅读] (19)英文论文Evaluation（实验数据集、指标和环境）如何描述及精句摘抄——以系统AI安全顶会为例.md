# 原创
：  [论文阅读] (19)英文论文Evaluation（实验数据集、指标和环境）如何描述及精句摘抄——以系统AI安全顶会为例

# [论文阅读] (19)英文论文Evaluation（实验数据集、指标和环境）如何描述及精句摘抄——以系统AI安全顶会为例

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇介绍了英文论文模型设计（Model Design）和概述（Overview）如何撰写，并摘抄系统AI安全顶会论文的精句。这篇文章将从个人角度介绍英文论文实验评估（Evaluation）的数据集、评价指标和环境设置如何撰写，继续以系统AI安全的顶会论文为例。一方面自己英文太差，只能通过最土的办法慢慢提升，另一方面是自己的个人学习笔记，并分享出来希望大家批评和指正。希望这篇文章对您有所帮助，这些大佬是真的值得我们去学习，献上小弟的膝盖~fighting！**</font>

由于作者之前做NLP和AI，现在转安全方向，因此本文选择的论文主要为近四年篇AI安全和系统安全的四大顶会（S&amp;P、USENIX Sec、CCS、NDSS）。同时，作者能力有限，只能结合自己的实力和实际阅读情况出发，也希望自己能不断进步，每个部分都会持续补充。可能五年十年后，也会详细分享一篇英文论文如何撰写，目前主要以学习和笔记为主。大佬还请飘过O(∩_∩)O

#### 文章目录

**前文赏析：**

---


## 一.Evaluation如何撰写

论文如何撰写因人而异，作者仅分享自己的观点，欢迎大家提出意见。然而，坚持阅读所研究领域最新和经典论文，这个大家应该会赞成，如果能做到相关领域文献如数家珍，就离你撰写第一篇英文论文更近一步了，甚至高质量论文。

在实验设计中，重点是如何通过实验说服审稿老师，赞同你的创新点，体现你论文的价值。好的图表能更好地表达你论文的idea，因此我们需要学习优秀论文，一个惊喜的实验更是论文成功的关键。注意，安全论文已经不再是对比PRF的阶段了，一定要让实验支撑你整个论文的框架。同时，多读多写是基操，共勉！

### 1.论文总体框架及实验撰写

该部分回顾和参考周老师的博士课程内容，感谢老师的分享。典型的论文框架包括两种（The typical “anatomy” of a paper），如下所示：

<mark>第一种格式：理论研究</mark>

<mark>第二种格式：系统研究</mark>

<mark>实验评估介绍（Evaluation）</mark>

---


### 2.实验评估撰写

该部分主要是学习易莉老师书籍《学术写作原来是这样》，后面我也会分享我的想法，具体如下：

结果与方法一种是相对容易写作的部分，其内容其实就是你对收集来的数据做了什么样的分析。对于相对简单的结果（3个分析以内），按部就班地写就好。有专业文献的积累，相信难度不大。写起来比较困难的是复杂数据的结果，比如包括10个分析，图片就有七八张。这时候对结果的组织就非常重要了。老师推荐《10条简单规则》一文中推荐的 <mark>结论驱动（conclusion-driven）</mark> 方法。

> 
<font color="red">**个人感觉：**<br/> 实验部分同样重要，但更重要是如何通过实验结果、对比实验、图表描述来支撑你的创新点，让审稿老师觉得，就应该这么做，amazing的工作。作为初学者，我们可能还不能做到非常完美的实验，但一定要让文章的实验足够详细，力争像该领域的顶级期刊或会议一样，并且能够很好的和论文主题相契合，最终文章的价值也体现出来了。</font>


**在数据处理的过程中，梳理、总结自己的主要发现，以这些发现为大纲（小标题），来组织结果的写作**（而不是传统上按照自己数据处理的顺序来组织）。以作者发表论文为例，他们使用了这种方法来组织结果部分，分为四个小标题，每个小标题下列出相应的分析及结果。

如果还有其他结果不能归入任何一个结论，那就说明这个结果并不重要，<mark>没有对形成文章的结论做出什么贡献，这时候果断舍弃（或放到补充材料中）是明智的选择</mark>。

另外，<mark>同一种结果可能有不同的呈现方式，可以依据你的研究目的来采用不同的方式</mark>。我在修改学生文章时遇到比较多的一个问题是采用奇怪的方式，突出了不重要的结果。举例：

> 
示例句子：<br/> The two groups were similar at the 2nd, 4th, and 8th trials; They differed from each other in the remaining trials. <br/><br/> 这句话有两个比较明显的问题：(1)相似的试次并不是重点，重点是大部分的试次是有差异的，但是这个重点没有被突出，反而仅有的三个一样的试次突出了。(2)语言的模糊，differ的使用来来的模糊性（不知道是更好还是更差）。<br/> 修改如下：<br/> Four-year-olds outperformed 3-year-olds in most trials, except the 2nd, 4th, and 8th trials, in which they performed similarly.


<font color="red">**对于结果的呈现，作图是特别重要的，一张好图胜过千言万语**。</font> 但我不是作图方面的专家，如果你需要这方面的指导，建议你阅读《10个简单规则，创造更优图形》，文中为怎么做出一张好图提供了非常全面而有用的指导。

---


### 3.讨论撰写

该部分主要是学习易莉老师书籍《学术写作原来是这样》，后面我也会分享我的想法，具体如下：

讨论是一个非常头疼的部分。先来讲讲讨论的写法，在前面强调了从大纲开始写的好处，从大纲开始写是一种自上而下的写法，在写大纲的过程中确定主题句，然后再确定其他内容。还有一种方法是自下而上地写，就是先随心所以地写第一稿，从笔记开始写，然后对这些笔记进行梳理和归纳，提炼主题句。<mark>老师通常混合两种写法，先从零星的点进行归纳（写前言时对文献观点做笔记，写讨论时对结果的发现做笔记），之后通过梳理，整理出大纲，再从大纲开始写作。</mark>

比如我对某篇文章的讨论部分做过相关笔记，然后对这些点进行梳理和归纳，再结合前沿提出来的三个研究问题形成讨论的大纲，如下：

在(1)到(4)段的讨论中，要先总结自己最重要的发现，不要忘记回顾前言中提出的实验预期，说明结果是否符合自己的预期。然后回顾前人研究与自己的研究发现是否一致，如果不一致，就可以讨论可能的原因（取样、实验方法的不同等）。

此外还需要注意，很多学生把讨论的重点放在了与前人研究不一致的结果和自己的局限性上，这些是需要写的，<font color="red">**但是最重要的是突出自己研究的贡献**</font>。

讨论中最常出现的问题就是把结果里的话换个说法再说一遍。其实讨论部分给了我们一个从更高层面梳理和解读研究结果的机会。更重要的是，需要明确提出自己的研究贡献，进一步强调研究的重要性、意义以及创新性。因此，不要停留在就事论事的结果描述上。<mark>读者读完结果后，很容易产生“so what”的问题——“是的，你发现了这些，那又怎么样呢？”。</mark>

这时候，最重要的是告诉读者研究的启示（implication）——你的发现说明了什么，加深了对什么问题的理解，对未解决的问题提供了什么新的解决方法，揭示了什么新的机制。这也是影响稿件录用的最重要部分，所以一定要花最多时间和精力来写这个部分。

用前文提到的“机器人”文章的结论作为例子，说明如何总结和升华自己的结论。

> 
Overall, our study contributes several promising preliminary findings on the potential involvement of humanoid robots in social rules training for children with ASD. Our results also shed light for the direction of future research, which should address whether social learning from robots can be generalized to a universal case (e.g., whether distrusting/deceiving the robot contributes to an equivalent effect on distrusting/deceiving a real person); a validation test would be required in future work to test whether children with ASD who manage to distrust and deceive a robot are capable of doing the same to a real person.


---


### 4.实验评估撰写之个人理解

首先我们要清楚实验写作的目的，通过详细准确的数据集、环境、实验描述，仿佛能让别人模仿出整个实验的过程，更让读者或审稿老师信服研究方法的科学性，增加结果数据的准确性和有效性。

如果我们的实验能发现某些有趣的结论会非常棒；如果我们的论文就是新问题并有对应的解决方法（创新性强），则实验需要支撑对应的贡献或系统，说服审稿老师；如果上述都不能实现，我们尽量保证实验详细，并通过对比实验（baseline对比）来巩固我们的观点和方法。

切勿只是简单地对准确率、召回率比较，每个实验结果都应该结合研究背景和论文主旨进行说明，有开源数据集的更好，没有的数据集建议开源，重要的是说服审稿老师认可你的工作。同时，实验步骤的描述也非常重要，包括实验的图表、研究结论、简明扼要的描述（给出精读）等。

在时态方面，由于是描述已经发生的实验过程，一般用过去时态，也有现在时。大部分期刊建议用被动句描述实验过程，但是也有一些期刊鼓励用主动句，因此，在投稿前，可以在期刊主页上查看“Instructions to Authors”等投稿指导性文档来明确要求。一起加油喔~

下面结合周老师的博士英语课程，总结实验部分我们应该怎么表达。

<font color="red">**图/表的十个关键点(10 key points)**</font>

说明部分要尽量把相应图表的内容表达清楚

图的说明一般在图的下边；表的说明一般在表的上边；表示整体数据的分布趋势的图不需太大；表示不同方法间细微差别的图不能太小。

几个图并排放在一起，如果有可比性，并排图的x/y轴的取值范围最好一致，利于比<br/> 较。

实验结果跟baseline在绝对数值上差别不大，用列表加黑体字；实验结果跟baseline在绝对数值上差别较大，用柱状图/折线图视觉表现力更好。

折线图要选择适当的颜色和图标，颜色选择要考虑黑白打印的效果；折线图的图标选择要有针对性，比如对比A, A+，B, B+四种方法。

---


### 5.整体结构撰写补充

同时，模型设计整体结构和写作细节补充几点：（引用周老师博士课程，受益匪浅）

---


## 二.Evaluation撰写及精句

由于作者偏向于AI系统安全领域，因此会先介绍数据集、评价指标和实验环境（含Baseline），然后才是具体的对比实验和性能比较及讨论（后续博客分享），所有实验都应层层递进证明本文的贡献和Insight。下面主要以四大顶会论文为主进行介绍，重点以系统安全和AI安全领域为主。

从这些论文中，我们能学习到什么呢？具体如下：

### 第0部分：引入和关联

该部分在实验评估环节主要作为引入，通常是介绍实验模块由哪几部分组成。同时，有些论文会直接给出实验的各个小标题，这时会省略该部分。主要包括两种类型描述（个人划分，欢迎指正）：

#### (1) 直接描述型

<font color="red">**第一种方法：介绍该部分所包含的内容，通常“In this section”并按顺序介绍。**</font>

In this section, <mark>we employ four datasets and experimentally evaluate four aspects of WATSON</mark>: 1) the explicability of inferred event semantics; 2) the accuracy of behavior abstraction; 3) the overall experience and manual workload reduction in attack investigation; and 4) <mark>the performance overhead (性能开销)</mark>.

> 
Jun Zeng, et al. WATSON: Abstracting Behaviors from Audit Logs via Aggregation of Contextual Semantics. NDSS 2021.


---


#### (2) 目的导向型

<font color="red">**第二种方法：结合研究背景、解决问题和本文方法及创新来描述，包括设计的实验内容。**</font>

<mark>Previous binary analysis studies</mark> usually evaluate their approaches by designing specific experiments in an end-to-end manner, since their instruction embeddings are only for individual tasks. <font color="blue">**In this paper, we focus on evaluating different instruction embedding schemes. To this end, we have designed and implemented an extensive evaluation framework to evaluate PalmTree and the baseline approaches.**</font> Evaluations can be classified into two categories: intrinsic evaluation and extrinsic evaluation. <mark>In the remainder of this section, we first introduce our evaluation framework and experimental configurations, then report and discuss the experimental results.</mark>

> 
Xuezixiang Li, et al. PalmTree: Learning an Assembly Language Model for Instruction Embedding, CCS21.


In this section, <mark>we prototype Whisper and evaluate its performance by using 42 real-world attacks.</mark> **In particular**, the experiments will answer the three questions:

> 
Chuanpu Fu, et al. Realtime Robust Malicious Traffic Detection via Frequency Domain Analysis. CCS 2021.


In this section, we evaluate our approach with the following major goals:

> 
Hongda Li, et al. vNIDS: Towards Elastic Security with Safe and Efficient Virtualization of Network Intrusion Detection Systems. CCS 2018.


In this section, we present our evaluation of DEEPREFLECT. <mark>First, we **outline** our objectives for each evaluation experiment and list which research goals (§2.4) are achieved by the experiment.</mark> We evaluate DEEPREFLECT’s (1) reliability by running it on three **real-world malware** samples we compiled and <mark>compared it to a machine learning classifier, a signature-based solution, and a function similarity tool</mark>, (2) cohesiveness by tasking malware analysts to **randomly sample** and label functions identified in in-the-wild samples and <mark>compare how DEEPREFLECT clustered these functions together</mark>, (3) **focus** by computing the number of functions an analyst has to reverse engineer given <mark>an entire malware binary</mark>, (4) <font color="blue">**insight** by observing different malware families sharing the same functionality and how DEEPREFLECT handles new incoming malware families, and (5) robustness by obfuscating and modifying a malware’s source code to attempt to evade DEEPREFLECT.</font>

> 
Evan Downing, et al. DeepReflect: Discovering Malicious Functionality through Binary Reconstruction, USENIX Sec 2021.


<font color="blue">**In this section, we evaluate Slimium on a 64-bit Ubuntu 16.04 system equipped with Intel® Xeon® E5-2658 v3 CPU (with 48 2.20 G cores) and 128 GB RAM. In particular, we assess Slimium from the following three perspectives:**</font>

<font color="blue">**IWe then highlight security benefits along with the number of CVEs discarded accordingly (Section 6.4.3).**</font>

> 
Chenxiong Qian, et al. Slimium: Debloating the Chromium Browser with Feature Subsetting, CCS 2020.


<mark>In this section, we evaluate DEEPBINDIFF with respect to its effectiveness and efficiency for **two different diffing scenarios (场景)**</mark>: cross-version and cross-optimization-level. <font color="blue">**To our best knowledge, this is the first research work that comprehensively (全面地) examines the effectiveness of program-wide binary diffing tools under the cross-version setting.**</font> <mark>**Furthermore**, we **conduct** a case study to demonstrate the usefulness of DEEPBINDIFF **in real-world vulnerability analysis**.</mark>

> 
Yue Duan, et al. DEEPBINDIFF: Learning Program-Wide Code Representations for Binary Diffing, NDSS 2020.


---


### 第1部分：数据集（bench-marked）

**Datasets**. <mark>To pre-train PalmTree and evaluate its transferability and generalizability, and evaluate baseline schemes in different downstream applications, we used different binaries from different compilers.</mark> The pre-training dataset contains different versions of Binutils4, Coreutils5, Diffutils6, and Findutils7 on x86-64 platform and compiled with Clang8 and GCC9 with different optimization levels. **The whole pre-training dataset contains 3,266 binaries and 2.25 billion instructions in total.** There are about 2.36 billion positive and negative sample pairs during training. <font color="blue">**To make sure that training and testing datasets do not have much code in common in extrinsic evaluations, we selected completely different testing dataset from different binary families and compiled by different compilers. Please refer to the following sections for more details about dataset settings.**</font>

> 
Xuezixiang Li, et al. PalmTree: Learning an Assembly Language Model for Instruction Embedding, CCS21.


<mark>Constructing a good benign dataset is **crucial (至关重要)** to our model’s performance</mark>. If we do not provide enough diverse behaviors of benign binaries, then everything within the malware binary will appear as unfamiliar. For example, if we do not train the autoencoder on binaries which perform network activities, then any network behaviors will be highlighted.

<mark>To collect our benign dataset, we **crawled** CNET [4] in 2018 for Portable Executable (PE) and Microsoft Installer (MSI) files from 22 different categories as defined by CNET to **ensure** a diversity of types of benign files.</mark> We collected **a total of** 60,261 binaries. After labeling our dataset, we ran our samples through Unipacker [11], a tool to extract unpacked executables. **Though not complete as compared to prior work** [21, 58], the tool produces a valid executable if it was successful (i.e., the malware sample was packed using one of several techniques Unipacker is designed to unpack). <mark>Since Unipacker covers most of the popular packers used by malware [67], it is reasonable to use this tool on our dataset.</mark> **By default**, if Unipacker cannot unpack a file successfully,it will not produce an output. Unipacker was able to unpack 34,929 samples. **However**, even after unpacking we found a few samples which still seemed partially packed or not complete (e.g., missing import symbols). We **further filtered** PE files which did not have a valid start address and whose import table size was zero (i.e., were likely not unpacked properly). We also deduplicatedthe unpacked binaries. Uniqueness was determined by taking the SHA-256 hash value of the contents of each file. <font color="blue">To improve the quality of our dataset, we only accepted benign samples which were classified as malicious by less than three antivirus companies (**according to VirusTotal**). In total, after filtering, we obtained 23,307 unique samples. The sizes of each category can be found in Table 1.</font>

<mark>To acquire our malicious dataset, we gathered 64,245 malware PE files from VirusTotal [12] during 2018.</mark> We then ran these samples through AVClass [62] to retrieve malware **family labels**. **Similar to the benign samples**, we unpacked, deduplicated, and filtered samples. Unipacker was able to unpack 47,878 samples. **In total**, we were left with 36,396 unique PE files from 4,407 families (3,301 of which were singleton families – i.e., only one sample belonged to that family). The sizes of the top-10 most populous families can be found in Table 2.

<mark>After collecting our datasets, we extracted our features from each sample using BinaryNinja, an industry-standard binary disassembler, and ordered each feature vector according to its basic block’s address location in a sample’s binary.</mark>

> 
Evan Downing, et al. DeepReflect: Discovering Malicious Functionality through Binary Reconstruction, USENIX Sec 2021.


We evaluate WATSON on four datasets: a benign dataset, a malicious dataset, a background dataset, and the DARPA TRACE dataset. **The first three datasets** are collected from ssh sessions on five enterprise servers running Ubuntu 16.04 (64-bit). The last dataset is collected on a network of hosts running Ubuntu 14.04 (64-bit). The audit log source is Linux Audit [9].

<mark>In the benign dataset, four users independently complete seven daily tasks, as described in Table I.</mark> Each user performs a task 150 times in 150 sessions. **In total**, we collect 17 (expected to be 4×7 = 28) classes of benign behaviors because different users may <mark>conduct the same operations to accomplish tasks</mark>. **Note that** there are user-specific artifacts, like launched commands, between each time the task is performed. For our benign dataset, there are 55,296,982 audit events, which make up 4,200 benign sessions.

<mark>In the malicious dataset, following the procedure found in previous works [2], [10], [30], [53], [57], [82], we **simulate** eight attacks from **real-world scenarios** as shown in Table II. Each attack is carefully performed ten times by two security engineers on the enterprise servers.</mark> <font color="blue">In order to incorporate (融合) the impact of typical noisy enterprise environments [53], [57], we continuously execute extensive ordinary user behaviors and underlying system activities in parallel to the attacks.</font> For our malicious dataset, there are 37,229,686 audit events, which make up 80 malicious sessions.

In the background dataset, we record behaviors of developers and administrators on the servers for two weeks. <font color="blue">**To ensure the correctness of evaluation, we manually analyze these sessions and only incorporate sessions without behaviors in Table I and Table II into the dataset. For our background dataset, there are 183,336,624 audit events, which make up 1,000 background sessions.**</font>

The DARPA TRACE dataset [13] is a **publicly available** APT attack dataset released by the TRACE team in the DARPA Transparent Computing (TC) program [4]. <mark>The dataset was derived from a network of hosts during a two-week-long red-team vs. blue-team adversarial Engagement 3 in April 2018.</mark> In the engagement, an enterprise is simulated with different security-critical services such as a web server, an SSH server, an email server, and an SMB server [63]. <mark>The red team **carries out** a series of nation-state and common attacks on the target hosts while simultaneously performing benign behaviors, such as ssh login, web browsing, and email checking.</mark> For the DARPA TRACE dataset, there are 726,072,596 audit events, which make up 211 graphs. **Note that we analyze only events that match our rules for triple translation in Section IV.**

We test WATSON’s **explicability and accuracy** on our first three datasets as we need the precise ground truth of the event semantics and high-level (both benign and malicious) behaviors for verification. <mark>We **further** explore WATSON’s efficacy in attack investigation against our malicious dataset and DARPA TRACE dataset because the ground truth of malicious behaviors related to attack cases is available to us.</mark>

**In general**, our experimental behaviors for abstraction are **comprehensive** as compared to behaviors in real-world systems. **Particularly**, the benign behaviors are designed based upon basic system activities [84] claimed to **have drawn attention in cybersecurity study**; the malicious behaviors are either selected from typical attack scenarios in previous work or generated by a red team with expertise in instrumenting and collecting data for attack investigation.

> 
Jun Zeng, et al. WATSON: Abstracting Behaviors from Audit Logs via Aggregation of Contextual Semantics, NDSS 2021.


We evaluate TEXTSHIELD on three datasets of which two are used for toxic content detection and one is used for adversarial NMT. <mark>Each dataset is divided into three parts, i.e., 80%, 10%, 10% as training, validation and testing, respectively [26].</mark>

> 
Jinfeng Li, et al. TextShield: Robust Text Classification Based on Multimodal Embedding and Neural Machine Translation, USENIX Sec 2020.


Datasets. **To thoroughly evaluate the effectiveness of DEEP- BINDIFF, we utilize three popular binary sets - Coreutils [2], Diffutils [3] and Findutils [4] with a total of 113 binaries.** Multiple different versions of the binaries (5 versions for Coreutils, 4 versions for Diffutils and 3 versions of Findutils) are collected with wide time spans between the oldest and newest versions (13, 15, and 7 years respectively). <mark>This setting ensures that each version has enough distinctions so that binary diffing results among them are meaningful and representative</mark>.

We **then compile** them using GCC v5.4 with 4 different compiler optimization levels (O0, O1, O2 and O3) in order to produce binaries equipped with different optimization techniques. This dataset is to show the **effectiveness** of DEEPBIN- DIFF in terms of cross-optimization-level diffing. We **randomly select** half of the binaries in our dataset for token embedding model training.

**To demonstrate the effectiveness with C++ programs**, we also collect 2 popular open-source C++ projects LSHBOX [8] and indicators [6], which contain plenty of virtual functions, from GitHub. The two projects include 4 and 6 binaries respectively. In LSHBOX, the 4 binaries are psdlsh, rbslsh, rhplsh and thlsh. And in indicators, there exist 6 binaries - blockprogressbar, multithreadedbar, progressbarsetprogress, progressbartick, progressspinner and timemeter. For each project, we select 3 major versions and compile them with the default optimization levels for testing.

<font color="blue">Finally, we **leverage** two different real-world vulnerabilities in a popular crypto library OpenSSL [9] for a case study to demonstrate the usefulness of DEEPBINDIFF in practice.</font>

> 
Yue Duan, et al. DEEPBINDIFF: Learning Program-Wide Code Representations for Binary Diffing, NDSS 2020.


To answer the above research questions, we collect relevant datasets. The details are as follows:

We publish all the above three datasets at [7] for the research community.

> 
Yun Lin, et al. Phishpedia: A Hybrid Deep Learning Based Approach to Visually Identify Phishing Webpages, USENIX Sec 2021.


---


### 第2部分：评估指标或方法

**Intrinsic Evaluation**. <mark>In NLP domain, intrinsic evaluation refers to the evaluations that compare the generated embeddings with human assessments [2]</mark>. **Hence**, for each intrinsic metric, manually organized datasets are needed. This kind of dataset could be collected either in laboratory on a limited number of examinees or through crowd-sourcing [25] by using web platforms or offline survey [2]. **Unlike the evaluations in NLP domain**, <mark>programming languages including assembly language (instructions) do not necessarily rely on human assessments</mark>. **Instead**, each opcode and operand in instructions has **clear semantic meanings**, which can be extracted from instruction reference manuals. **Furthermore**, <mark>debug information generated by different compilers and compiler options can also indicate whether two pieces of code are semantically equivalent</mark>. **More specifically**, we design two intrinsic evaluations: instruction outlier detection based on the knowledge of semantic meanings of opcodes and operands from instruction manuals, and basic block search by **leveraging** the debug information **associated with source code**.

**Extrinsic Evaluation**. <font color="blue">**Extrinsic evaluation aims to evaluate the quality of an embedding scheme along with a downstream machine learning model in an end-to-end manner [2].**</font> So if a downstream model is more accurate when integrated with instruction embedding scheme A than the one with scheme B, then A is considered better than B. <font color="blue">**In this paper, we choose three different binary analysis tasks for extrinsic evaluation, i.e., Gemini [40] for binary code similarity detection, EKLAVYA [5] for function type signatures inference, and DeepVSA [14] for value set analysis.**</font> We **obtained** the original implementations of these downstream tasks for this evaluation. <mark>All of the downstream applications are implemented based on TensorFlow</mark>. **Therefore we choose the first way of deploying PalmTree in extrinsic evaluations (see Section 3.4.6). We encoded all the instructions in the corresponding training and testing datasets and then fed the embeddings into downstream applications.**

> 
Xuezixiang Li, et al. PalmTree: Learning an Assembly Language Model for Instruction Embedding, CCS21.


<mark>To evaluate DEEPREFLECT’s reliability, we explore and contrast the models’ performance in localizing the malware components within binaries.</mark>

**Baseline Models**. To evaluate the localization capability of DEEPREFLECT’s autoencoder, we compare it to a general method and domain specific method for localizing concepts in samples: (1) SHAP, a classification model explanation tool [40], (2) CAPA [3], <mark>a signature based tool by FireEye for identifying malicious behaviors within binaries</mark>,4 and (3) FunctionSimSearch [5], a function similarity tool.

Given a trained classifier and the sample x, SHAP provides each feature x(i) in x a **contribution score** for the classifier’s prediction. For SHAP’s model, we trained a modified deep neural network VGG19 [64] to predict a sample’s malware family and **whether the sample is benign.** For this model, we could not use our features because the model would not converge. <mark>**Instead**, we used the classic ACFG features without the string or integer features.</mark> We call these features attributed basic block (ABB) features. We trained this model for classification (on both malicious and benign samples) and achieved a training accuracy of 90.03% and a testing accuracy of 83.91%. **In addition to SHAP**, we trained another autoencoder on ABB features to compare to our new features as explained in §3.2.1.

> 
Evan Downing, et al. DeepReflect: Discovering Malicious Functionality through Binary Reconstruction, USENIX Sec 2021.


<mark>In this paper, we first evaluate the performance of our subtree-based deobfuscation, which is divided into three parts.</mark> First, we evaluate whether we can find the minimum subtrees involved in obfuscation, which can directly determine the quality of the deobfuscation. **This is dependent on the classifier and thus we cross-validate the classifier with manually-labelled ground truth.** Second, we **verify** the quality of the entire obfuscation by comparing the similarity between the deobfuscated scripts and the original scripts. In this evaluation, we **modify** the AST-based similarity calculation algorithm provided by [39]. <mark>Third, we evaluate the efficiency of deobfuscation by calculating the average time required to deobfuscate scripts obfuscated by different obfuscation methods.</mark>

**Next**, we evaluate the benefit of our deobfuscation method on PowerShell attack detection. In §2, we find that obfuscation can **evade most of the existing anti-virus engine**. In this section, we compare the detection results for the same PowerShell scripts before and after applying our deobfuscation method. **In addition**, we also evaluate the effectiveness of the **semantic-based detection algorithm** in Section 5.

**6.1.1 PowerShell Sample Collection**<br/> <mark>To evaluate our system, we create a collection of malicious and benign, obfuscated and non-obfuscated PowerShell samples.</mark> We **attempt to cover all possible download sources that can have PowerShell scripts**, e.g., GitHub, security blogs, open-source PowerShell attack repositories, etc., instead of intentionally making selections among them.

**6.1.2 Script Similarity Comparison**<br/> Deobfuscation can **be regarded as** the reverse process of obfuscation. In the ideal case, deobfuscated scripts should be **exactly** the same as the original ones. <mark>However, in practice, it is difficult to achieve such perfect recovery for various reasons.</mark> However, the similarity between the recovered script and the original script is still a good indicator to evaluate the overall recovery effect.

**To measure the similarity of scripts**, we **adopt** the methods of code clone detection. This problem is widely studied in the past decades [50]. Different clone granularity levels apply to different intermediate source representations. Match detection algorithms are a critical issue in the clone detection process. <mark>After a source code representation is decided, a carefully selected match detection algorithm is applied to the units of source code representation.</mark> We employ suffix tree matching based on ASTs [40]. Both the suffix tree and AST are widely used in similarity calculation. **Moreover**, <mark>such combination can be used to distinguish three types of clones, namely, Type 1(Exact Clones), Type 2(Renamed Clones), Type 3(Near Miss Clones), which fits well for our situation.</mark>

**To this end, we parse each PowerShell script into an AST.** Most of the code clone detection algorithm is line-based. However, lines wrapping is not reliable after obfuscation. We **utilize** subtrees instead of lines. We **serialize** the subtree by pre-order traversal and apply suffix tree works on sequences. Therefore, each subtree in one script is compared to each subtree in the other script. The similarity between the two subtrees is computed by the following formula:

> 
Zhenyuan Li, et al. Effective and Light-Weight Deobfuscation and Semantic-Aware Attack Detection for PowerShell Scripts, CCS 2019.


**Baselines.**<br/> <mark>We implement and compare two state-of-the-art methods with TEXTSHIELD to evaluate their robustness against the extended TextBugger.</mark> **In total**, the two methods are:

**Evaluation Metrics.**<br/> **Translation Evaluation**. We use three metrics, i.e, word error rate, bilingual evaluation understudy and semantic similarity to evaluate the translation performance of our adversarial NMT model from word, feature and semantics levels.

> 
Jinfeng Li, et al. TextShield: Robust Text Classification Based on Multimodal Embedding and Neural Machine Translation, USENIX Sec 2020.


<mark>With the datasets and ground truth information, we evaluate the effectiveness of DEEPBINDIFF by performing diffing between binaries across different versions and optimization levels, and comparing the results with the baseline techniques.</mark>

**Evaluation Metrics**. We use precision and recall metrics to measure the effectiveness of the diffing results produced by diffing tools. The matching result M from DEEPBINDIFF can be presented as a set of basic block **matching pairs** with a length of x as Equation 5. Similarly, the ground truth information G for the two binaries can be presented as a set of basic block matching pairs with a length of y as Equation 6.

==We then introduce two subsets, Mc and Mu, which represent correct matching and unknown matching respectively. == Correct match Mc = M ∩ G is the intersection of our result M and ground truth G. It gives us the correct basic block matching pairs. <mark>Unknown matching result Mu represents the basic block matching pairs in which no basic block ever appears in ground truth.</mark> **Thus**, we have no idea whether these matching pairs are correct. This could happen because of the conservativeness of our ground truth collection process. **Consequently**, M − Mu − Mc portrays the matching pairs in M that are not in Mc nor in Mu, therefore, all pairs in M − Mu − Mc are confirmed to be incorrect matching pairs. Once M and G are formally presented, the precision metric presented in Equation 7 gives the percentage of correct matching pairs among all the known pairs (correct and incorrect).

The recall metric shown in Equation 8 is produced by dividing the size of intersection of M and G with the size of G. This metric shows the percentage of ground truth pairs that are confirmed to be correctly matched.

> 
Yue Duan, et al. DEEPBINDIFF: Learning Program-Wide Code Representations for Binary Diffing, NDSS 2020.


---


### 第3部分：环境设置（baseline）

**Baseline Schemes and PalmTree Configurations**. <mark>We choose Instruction2Vec, word2vec, and Asm2Vec as baseline schemes</mark>. <font color="blue">**For fair comparison, we set the embedding dimension as 128 for each model. We performed the same normalization method as PalmTree on word2vec and Asm2Vec.**</font> We did not set any limitation on the vocabulary size of Asm2Vec and word2vec. We **implemented** these baseline embedding models and PalmTree <mark>using PyTorch</mark> [30]. PalmTree is based on BERT but has fewer parameters. While in BERT #Layers = 12, Head = 12 and Hidden_dimension = 768, we set #Layers = 12, Head = 8, Hidden_dimension = 128 in PalmTree, **for the sake of efficiency and training costs**. The ratio between the positive and negative pairs in both CWP and DUP is 1:1. <font color="blue">**Furthermore, to evaluate the contributions of three training tasks of PalmTree, we set up three configurations:**</font>

**Hardware Configuration**. All the experiments <mark>were conducted</mark> on a dedicated server with a Ryzen 3900X CPU@3.80GHz×12, one GTX 2080Ti GPU, 64 GB memory, and 500 GB SSD.

> 
Xuezixiang Li, et al. PalmTree: Learning an Assembly Language Model for Instruction Embedding, CCS21.


<mark>In this section, we evaluate the effectiveness and efficiency of our approach using the collected PowerShell samples described earlier (§6.1.1).</mark> The experiment results are obtained using a PC with Intel Core i5-7400 Processor 3.5 GHz, 4 Cores, and 16 Gigabytes of memory, running Windows 10 64-bit Professional.

> 
Zhenyuan Li, et al. Effective and Light-Weight Deobfuscation and Semantic-Aware Attack Detection for PowerShell Scripts, CCS 2019.


We measure what semantics WATSON learns for audit events **both visually and quantitatively**: Visually, we use t-SNE to project the embedding space into a 2D-plane <mark>giving us an intuition of the embedding distribution</mark>=; quantitatively, for each triple, we compare the training loss in the TransE model against our knowledge of event semantics and their similarities.

**Embedding of system entities**. Each element’s semantics in events is represented as a 64-dimensional vector, <mark>where the spatial distance between vectors encodes their semantic similarity</mark>. **To visualize the distance,** we apply t-SNE to project high-dimensional embedding space into a two-dimensional (2D) space while largely preserving structural information among vectors. **To manage the complexity**, we **randomly sampled** 20 sessions from the first three datasets for visualization, obtaining a scatter plot of 2,142 points. Figure 5a shows the 2D visualization of the embedding space. Points in the space are distributed in clusters, suggesting that events are indeed grouped based on some metric of similarity.

<mark>We further select eight programs (git, scp, gcc, ssh, scp, vim, vi, and wget) to investigate process element embeddings.</mark> Figure 5b shows a zoom in view of Figure 5a containing 53 elements corresponding to the eight programs. **For clarity**, the elements are labeled with process names and partial arguments. **Note that** identity information is erased during one-hot encoding and thus does not contribute to semantics inference. <mark>**While** most elements of the same program are clustered together, there are a few interesting cases supporting the hypothesis that the embeddings are actually semantic-based.= For example, git has a few subcommands (push, add, commit).</mark> Git push is mapped closer to scp and wget instead of git commit and git add. This agrees with the **high-level behaviors** where git push uploads a local file to a remote repository while git add and git commit manipulate files locally. Another interesting example involves ssh where two different clusters can be identified, but both represent ssh connection to a remote host. Upon closer inspection, we notice that these two clusters, in effect, **correspond to** the usage of ssh with and without X-forwarding, <mark>reflecting the difference in semantics</mark>.

In summary, WATSON learns semantics that consistently mirrors our intuitive understanding of event contexts.

> 
Jun Zeng, et al. WATSON: Abstracting Behaviors from Audit Logs via Aggregation of Contextual Semantics, NDSS 2021.


<mark>To fairly study the performance and robustness of the baselines and TEXTSHIELD, our experiments have the following settings</mark>:

We **conducted** all the experiments on a server with two Intel Xeon E5-2682 v4 CPUs running at 2.50GHz, 120 GB memory, 2 TB HDD and two Tesla P100 GPU cards.

> 
Jinfeng Li, et al. TextShield: Robust Text Classification Based on Multimodal Embedding and Neural Machine Translation, USENIX Sec 2020.


<mark>Our experiments are performed on a moderate desktop computer running Ubuntu 18.04LTS operating system with Intel Core i7 CPU, 16GB memory and no GPU.</mark> The feature vector generation and basic block embedding generation components in DEEPBINDIFF are expected to be significantly faster if GPUs are utilized since they are built upon deep learning models.

**Baseline Techniques**. <mark>With the aforementioned datasets, we compare DEEPBINDIFF with two state-of-the-art baseline techniques (Asm2Vec [23] and BinDiff [10]). Note that Asm2Vec is designed only for function level similarity detection.</mark> **We leverage its algorithm to generate embeddings, and use the same k-hop greedy matching algorithm to perform diffing.** Therefore, we denote it as ASM2VEC+k-HOP. Also, to demonstrate the usefulness of the contextual information, we **modify** DEEPBINDIFF to exclude contextual information and only include semantics information for embedding generation, <mark>shown as DEEPBINDIFF-CTX</mark>. **As mentioned in Section I,** another state-of-the-art technique InnerEye [58] has scalability issue for binary diffing. **Hence**, we only compare it with DEEPBINDIFF using a set of small binaries in Coreutils. Note that we also apply the same k-hop greedy matching algorithm in InnerEye, and denote it as INNEREYE+k-HOP.

**Ground Truth Collection**. <mark>For the purpose of evaluation, we rely on source code level matching and debug symbol information to conservatively collect ground truth that indicates how basic blocks from two binaries should match.</mark>

**Particularly**, for two input binaries, <mark>we first extract source file names from the binaries and use Myers algorithm [46] to perform text based matching for the source code in order to get the line number matching</mark>. **To ensure the soundness of our extracted ground truth**, 1) we only collect identical lines of source code as matching but ignore the modified ones; 2) our ground truth collection conservatively removes the code statements that lead to multiple basic blocks. **Therefore**, although our source code matching is by no means complete, it is guaranteed to be sound. Once we have the line number mapping between the two binaries, we extract debug information to understand the mapping between line numbers and program addresses. **Eventually**, the ground truth is collected by examining the basic blocks of the two binaries containing program addresses that map to the matched line numbers.

> 
Yue Duan, et al. DEEPBINDIFF: Learning Program-Wide Code Representations for Binary Diffing, NDSS 2020.


---


### 第4部分：实验精美图片

这里先让大家学习顶会论文的优美作图，具体描述将在后续的博客中详细介绍。

<img alt="在这里插入图片描述" height="400" src="https://img-blog.csdnimg.cn/c94b2e7843d948feac7389fb884ae107.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center" width="550"/><br/> <img alt="在这里插入图片描述" height="360" src="https://img-blog.csdnimg.cn/a650cbfc43e34ebc87e9ffeefb107f45.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center" width="650"/>

<img alt="在这里插入图片描述" height="420" src="https://img-blog.csdnimg.cn/ab45bf3acab344e3bf1dd4c78590caa5.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_18,color_FFFFFF,t_70,g_se,x_16#pic_center" width="500"/><br/> <img alt="在这里插入图片描述" height="400" src="https://img-blog.csdnimg.cn/97e3259f379d404c94f668c441f357d1.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center" width="550"/>

---


### 第5部分：实验精美表格

---


## 三.总结

这篇文章就写到这里了，希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，真心推荐原文，这些大佬真的值得我们学习，继续加油，且看且珍惜！

> 
忙碌的三月结束，时间流逝得真快，也很充实。虽然很忙，但三月还是挤时间回答了很多博友的问题，有咨询技术和代码问题的，有找工作和方向入门的，也有复试面试的。尽管自己科研和技术很菜，今年闭关也非常忙，但总忍不住。唉，性格使然，感恩遇见，问心无愧就好。但行好事，​莫问前程。继续奋斗，继续闭关，。小珞珞这嘴巴，哈哈，每日快乐源泉，晚安娜O(∩_∩)O <br/><br/> <img alt="在这里插入图片描述" height="500" src="https://img-blog.csdnimg.cn/bfdacca1afce411c90b76c40ca29d7ce.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center" width="550"/>


<mark>**独在异乡为异客，每逢佳节倍思亲。思念亲人**</mark>

(By:Eastmount 2022-04-05 晚上12点 http://blog.csdn.net/eastmount/ )
