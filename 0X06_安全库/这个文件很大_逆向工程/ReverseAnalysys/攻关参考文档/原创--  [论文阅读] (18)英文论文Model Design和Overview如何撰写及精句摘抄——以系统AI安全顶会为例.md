# 原创
：  [论文阅读] (18)英文论文Model Design和Overview如何撰写及精句摘抄——以系统AI安全顶会为例

# [论文阅读] (18)英文论文Model Design和Overview如何撰写及精句摘抄——以系统AI安全顶会为例

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇介绍CCS2019的Powershell去混淆工作，这篇文章质量非常高，来自于浙江大学的李振源老师。这篇文章将从个人角度介绍英文论文模型设计（Model Design）和概述（Overview）如何撰写，并以系统AI安全的顶会论文为例。一方面自己英文太差，只能通过最土的办法慢慢提升，另一方面是自己的个人学习笔记，并分享出来希望大家批评和指正。希望这篇文章对您有所帮助，这些大佬是真的值得我们去学习，献上小弟的膝盖~fighting！**</font>

由于作者之前做NLP和AI，现在转安全方向，因此本文选择的论文主要为近四年篇AI安全和系统安全的四大顶会（S&amp;P、USENIX Sec、CCS、NDSS）。同时，作者能力有限，只能结合自己的实力和实际阅读情况出发，也希望自己能不断进步，每个部分都会持续补充。可能五年十年后，也会详细分享一篇英文论文如何撰写，目前主要以学习和笔记为主。大佬还请飘过O(∩_∩)O

#### 文章目录

**前文赏析：**

---


## 一.模型设计或方法如何撰写

论文如何撰写因人而异，作者仅分享自己的观点，欢迎大家提出意见。然而，坚持阅读所研究领域最新和经典论文，这个大家应该会赞成，如果能做到相关领域文献如数家珍，就离你撰写第一篇英文论文更近一步了。在模型设计中，重点是模型如何与需要解决的问题结合，让人觉得确实该模型能解决类似的问题，一个好的故事是论文成功的关键。同时，多读多写是基操，共勉！

### 1.论文总体框架及方法撰写

该部分回顾和参考周老师的博士课程内容，感谢老师的分享。典型的论文框架包括两种（The typical “anatomy” of a paper），如下所示：

<mark>第一种格式：理论研究</mark>

<mark>第二种格式：系统研究</mark>

<mark>System Model（系统模型）</mark>

<mark>Mathematics and algorithms（算法）</mark>

注意，阅读理解该部分是前提，在阅读算法实现前，我们需要注意以下几点：

---


### 2.方法或模型设计撰写

该部分主要是学习易莉老师书籍《学术写作原来是这样》，具体如下：

“通常，我会指导学生从方法和结果部分开始一篇论文的写作。方法和结果相比于论文其他部分来说比较好写，学生参照文献‘依葫芦画瓢’，也能快速掌握。一般就是按部就班地写，实验是怎么做的，方法部分就怎么写；发现了什么，结果部分就写什么。”

> 
<mark>个人而言，感觉不太赞同这个观点。私以为方法或系统设计非常重要，至少在计算机领域，尤其方法对应的框架图，一定程度上能决定您论文的观点、创新及贡献，也决定论文最终的层次。其次，如果想投顶会或顶刊，一个好的故事，或者一个与您贡献吻合的方法描述至关重要。当然，初学者写作从模型设计开始是可以的。</mark>


在方法部分的写作中，需要注意：

该部分有一个重要的问题——<font color="red">**谋篇布局：怎么讲好一个故事**</font>

<mark>第一个问题：什么样的故事是好故事？</mark>

在评估一个科研成果的科学价值时，最重要的是创新性和研究意义。创新性是指研究者不是单纯地跟随或重复别人的研究，而是有自己的独到的新贡献。据说，研究要经历三个阶段：“me too”、“me better”、“me only”。同样，创新性也可以套用三个阶段描述。

**同时，创新不是天马行空。科研中的创新都是有边界的，创新是有研究意义的。总之，在开始写论文之前，我们需要考虑这篇文章大概要投稿给什么期刊，是小领域的专业期刊，还是一般的心理学期刊，这个决定会影响你的文章思路以及讲故事要达到的层次。**

比起其他的写作技能，讲故事的能力更难掌握，因为这是一种“只可意会不可言传”的神秘技能，甚至有不少人觉得这是写作的天花板。有些研究者在国外老板的大实验室里能做出非常漂亮的工作，回到柜内自己做PI时成果却平平无奇，多是因为国外大老板的研究视野（vision）和品味决定了文章的层次，而他们自己却没有学会提有前瞻性和创新性的科学问题。

> 
个人想对上面的观点进行补充。确实国外的大老板能带给我们很大的视野，提升论文的层次。然而，我们大部分的科研工作者，尤其是博士生，很少能接触到这么厉害的科学家，甚至很多都需要独立的搞论文。此时，怎么办呢？<br/><br/> <font color="red">我觉得一方面我们需要多读论文，多看国外大佬们的分享，通过学习和对比他们的论文和方法来提升自己；另一方面我们要多写论文，多做实验，善于关注问题，当我们的论文故事叙述能写得像顶会、顶刊时，我们至少会中一些SCI二区、B会，如果再持之以恒的提升质量、想idea，最后肯定也能写出高质量论文。芸芸众生，虽然我们没有这些大佬的帮助，但我相信通过努力肯定能学会科研，提升自己。这也是我写这个专栏的目的，从零开始学习英文写作，用最土的方法学习写作。博士路上一起前行，相信自己，加油~</font>


<mark>第二个问题：怎么将一个故事讲好？</mark><br/> 有了故事内核，我们还需要把这个故事完美地呈现出来。为此，我们需要学习如何利用大纲（outline）来帮助文章布局。大纲是关于文章组织架构的写作计划，它可以帮助你思考文章的主要故事和框架，抓住核心科学问题，把文章的故事有顺序、有逻辑、有重点地呈现出来。

---


### 3.模型设计撰写之个人理解

个人感觉：模型设计非常重要，通常我的写法如下。

**下面简单介绍几篇经典论文的模型设计写法及组成，通常都是论文框架或算法如何实现的，包括框架图、算法、公式、表格、约束等。**

<mark>(1) Chuanpu Fu, et al. Realtime Robust Malicious Traffic Detection via Frequency Domain Analysis. CCS21. （频域恶意流量入侵检测）</mark>

> 
4 DESIGN DETAILS



<mark>(2) Yuankun Zhu, et al. Hermes Attack: Steal DNN Models with Lossless Inference Accuracy. USENIX Sec21. （DNN模型攻击&amp;推理信息）</mark>

> 
3 Attack Design



<mark>(3) Xuezixiang Li, et al. PalmTree: Learning an Assembly Language Model for Instruction Embedding, CCS21 （Bert预训练指令向量）</mark>

> 
3 DESIGN OF PALMTREE



<mark>(4) Jinfeng Li, et al. TextShield: Robust Text Classification Based on Multimodal Embedding and Neural Machine Translation, USENIX Sec20. （文本对抗样本，多模态和实验值得我学习）</mark>

> 
3 Design of TEXTSHIELD



<mark>(5) Xueyuan Han, et al. UNICORN: Runtime Provenance-Based Detector for Advanced Persistent Threats, NDSS20. （溯源图检测APT）</mark>

> 
IV. DESIGN



<mark>(6) Zhenyuan Li, et al. Effective and Light-Weight Deobfuscation and Semantic-Aware Attack Detection for PowerShell Scripts, CCS19. （Powershell解混淆）</mark>

> 
3 OVERVIEW<br/> 4 POWERSHELL DEOBFUSCATION



---


### 4.整体结构撰写补充

同时，模型设计整体结构和写作细节补充几点：（引用周老师博士课程，受益匪浅）

实验部分同样有很多细节，下一篇文章我们再详细介绍。

---


## 二.Model Design撰写及精句

个人习惯将模型设计结合框架图进行描述，也欢迎大家批评指正。下面主要以四大顶会论文为主进行介绍，重点以系统安全和AI安全领域为主。从这些论文中，我们能学习到什么呢？具体如下：

### 第0部分：引入和关联

引入主要是各个部分（Section）详细介绍前，会说明该部分的相关分布情况。其描述大同小异，通常介绍各个部分的内容即可“In this section, we present…”。此外，部分论文也会通过“To tackle the challenges mentioned above”引入。下面给出12个示例，来源于模型设计或相关工作部分。

---


### 第1部分：Overview（系统框架如何描述）

#### Hermes Attack（USENIX Sec21）

> 
[1] Yuankun Zhu, et al. Hermes Attack: Steal DNN Models with Lossless Inference Accuracy. USENIX Sec21.


**Attack Overview**. <mark>The methodology of our attack can be divided into two phases: offline phase and online phase</mark>. During the offline phase, we use white-box models to build a database with the identified command headers, the mappings between GPU kernel (binaries) and DNN layer types, and the mappings between GPU kernels and offsets of hyperparameters. **Specifically**, <mark>the traffic processing module ( ① in Figure 5) sorts the out-of-order PCIe packets intercepted by PCIe snooping device. The extraction module ( ② ) has two sub-modules: header extraction module and command extraction module</mark>. The header extraction module extracts command headers from the sorted PCIe packets (Section 3.3.1). The extracted command headers will be stored in the database, accelerating command extraction in the online phase. The command extraction module in the offline phase helps get the kernel binaries (Section 3.3.2). **The semantic reconstruction module** within the reconstruction module ( ③ ) takes the inputs from the command extraction module and the GPU profiler to create the mappings between the kernel (binary) and the layer type, **as well as** the mappings between the kernel and the offset of hyper-parameters, **facilitating the module reconstruction in the online phase (Section 3.4.1)**.

**During the online phase**, the original (victim) model is used for inference on a single image. The victim model is a black-box model and thoroughly different from the white-box models used in the offline phase. PCIe traffics are intercepted and sorted by the traffic processing module. The command extraction module ( ② ) extracts K (kernel launch related) and D (data movement related) commands as well as the GPU kernel binaries, <mark>using the header information profiled from the offline phase (Section 3.3.2)</mark>. The entire database are feed to the model reconstruction module ( ③ ) to fully reconstruct architecture, hyper-parameters, and parameters (Section 3.4.2). <mark>All these steps need massive efforts of reverse engineering.</mark>

---


#### PalmTree（CCS21）

> 
[2] Xuezixiang Li, et al. PalmTree: Learning an Assembly Language Model for Instruction Embedding, CCS21.


<font color="red">**To meet the challenges summarized in Section 2, we propose PalmTree, a novel instruction embedding scheme that automatically learns a language model for assembly code. PalmTree is based on BERT [9], and incorporates the following important design considerations.**</font>

<mark>**First of all, to capture the complex internal formats of instructions, we use a fine-grained strategy to decompose instructions**</mark>: we consider each instruction as a sentence and decompose it into basic tokens. **Then**, in order to train the deep neural network to understand the internal structures of instructions, <mark>we make use of a recently proposed training task in NLP to train the model: Masked Language Model (MLM) [9]</mark>. This task trains a language model to predict the masked (missing) tokens within instructions.

<font color="red">**Moreover, we would like to train this language model to capture the relationships between instructions. To do so, we design a training task, inspired by word2vec [28] and Asm2Vec [10], which attempts to infer the word/instruction semantics by predicting two instructions’ co-occurrence within a sliding window in control flow.**</font> We call this training task Context Window Prediction (CWP), which is based on Next Sentence Prediction (NSP) [9] in BERT. **Essentially**, if two instructions i and j fall within a sliding window in control flow and i appears before j, we say i and j have a contextual relation. **Note that** this relation is more relaxed than NSP, where two sentences have to be next to each other. <mark>We make this design decision based on our observation described in Section 2.2.2</mark>: instructions may be reordered by compiler optimizations, so adjacent instructions might not be semantically related.

**Furthermore, unlike natural language, instruction semantics are clearly documented.** For instance, the source and destination operands for each instruction are clearly stated. **Therefore**, the data dependency (or def-use relation) between instructions is clearly specified and will not be tampered by compiler optimizations. <font color="red">**Based on these facts, we design another training task called Def-Use Prediction (DUP) to further improve our assembly language model.**</font> Essentially, we train this language model to predict if two instructions have a def-use relation.

Figure 1 presents the design of PalmTree. <mark>It consists of three components</mark>: Instruction Pair Sampling, Tokenization, and Language Model Training. The main component (Assembly Language Model) of the system is based on the BERT model [9]. After the training process, we use mean pooling of the hidden states of the second last layer of the BERT model as instruction embedding. The Instruction Pair Sampling component is responsible for sampling instruction pairs from binaries based on control flow and def-use relations.

In Section 3.2, we introduce how we construct two kinds of instruction pairs. In Section 3.3, we introduce our tokenization process. Then, <mark>we introduce how we design different training tasks to pre-train a comprehensive assembly language model for instruction embedding in Section 3.4.</mark>

---


#### TextShield（USENIX Sec20）

> 
[3] Jinfeng Li, et al. TextShield: Robust Text Classification Based on Multimodal Embedding and Neural Machine Translation, USENIX Sec20.


<mark>We present the framework overview of TEXTSHIELD in Fig.1, which is built upon multimodal embedding, multimodal fusion and NMT.</mark> **Generally**, we first feed each text into an NMT model trained with a plenty of adversarial–benign text pairs for adversarial correction. <font color="red">**Then, we input the corrected text into the DLTC model for multimodal embedding to extract features from semantic-level, glyph-level and phoneticlevel.**</font> Finally, we use a multimodal fusion scheme to fuse the extracted features for the following regular classifications. <mark>Below, we will elaborate on each of the backbone techniques.</mark>

<mark><font color="red">Since the variation strategies adopted by malicious users in the real scenarios are mainly concentrated on glyph-based and phonetic-based perturbations [47], we therefore dedicatedly propose three embedding methods across different modalities to handle the corresponding variation types, i.e., semantic embedding, glyph embedding and phonetic embedding.</font></mark> They are also dedicatedly designed to deal with the sparseness and diversity unique to Chinese adversarial perturbations.

Since multiple modalities can provide more valuable information than a single one by describing the same content in various ways, it is highly expected to learn <mark>effective joint representation</mark> by fusing the features of different modalities. **Therefore**, after multimodal embedding, **we first fuse the features extracted from different modalities by multimodal fusion and then feed the fused features into a classification model for regular classification**. In this paper, we experiment with two different fusion strategies, i.e., early multimodal fusion and intermediate multimodal fusion as shown in Fig. 10 in Appendix A.

---


#### UNICORN（NDSS20）

> 
[4] Xueyuan Han, et al. UNICORN: Runtime Provenance-Based Detector for Advanced Persistent Threats, NDSS20.


UNICORN is a host-based intrusion detection system capable of simultaneously (同时) detecting intrusions on a collection of networked hosts. <mark>We begin with a brief overview of UNICORN and then follow with a detailed discussion of each system component in the following sections. Fig.1 illustrates UNICORN’s general pipeline.</mark>

<font color="red">**①Takes as input a labeled, streaming provenance graph.**</font><br/> UNICORN accepts a stream of attributed edges produced by a provenance capture system running on one or more networked hosts. <mark>Provenance systems construct a single, whole-system provenance DAG with a partial-order guarantee, which allows for efficient streaming computation (§ IV-B) and fully contextualized analysis (L2).</mark> **We present UNICORN using CamFlow [100], although it can obtain provenance from other systems, such as LPM [16] and Spade [44], the latter of which interoperates with commodity audit systems such as Linux Audit and Windows ETW.**

<font color="red">**② Builds at runtime an in-memory histogram.**</font><br/> <mark>**UNICORN efficiently constructs a streaming graph histogram (直方图) that represents the entire history of system execution, updating the counts of histogram elements as new edges arrive in the graph data stream.**</mark> By iteratively exploring larger graph neighborhoods, it discovers causal relationships between system entities providing execution context. This is UNICORN’s first step in building an efficient data structure that facilitates contextualized graph analysis (L2). **Specifically**, each element in the histogram describes a unique substructure of the graph, <mark>taking into consideration the heterogeneous label(s) attached to the vertices and edges within the substructure, as well as the temporal order of those edges.</mark>

**To adapt to expected behavioral changes during the course of normal system execution**, UNICORN periodically discounts the influence of histogram elements that have no causal relationships with recent events (L3). Slowly “forgetting” irrelevant past events allows us to effectively model metastates (§ IV-D) throughout system uptime <mark>(e.g., system boot, initialization, serving requests, failure modes, etc.)</mark>. **However, it does not mean that** UNICORN forgets informative execution history; **rather**, UNICORN uses information flow dependencies in the graph to **keep up-to-date important**, relevant context information. Attackers can slowly penetrate the victim system in an APT, hoping that a time-based IDS eventually forgets this initial attack, <mark>but they cannot break the information flow dependencies that are essential to the success of the attack [87].</mark>

<font color="red">**③ Periodically, computes a fixed-size graph sketch.**</font><br/> In a pure streaming environment, the number of unique histogram elements can grow arbitrarily large as UNICORN summarizes the entire provenance graph. <mark>This variation in size makes it challenging to efficiently compute similarity between two histograms and impractical to design algorithms for later modeling and detection</mark>. **UNICORN employs a similarity-preserving hashing technique [132] to transform the histogram to a graph sketch [7].** The graph sketch is incrementally maintainable, meaning that UNICORN does not need to keep the entire provenance graph in memory; its size is constant (L4). **Additionally**, graph sketches preserve normalized Jaccard similarity [64] between two graph histograms. This distance-preserving property is particularly important to the clustering algorithm in our later analysis, which is based on the same graph similarity metric.

<font color="red">**④ Clusters sketches into a model.**</font><br/> UNICORN builds a normal system execution model and identifies abnormal activities without attack knowledge (L1). <font color="red">**However, unlike traditional clustering approaches, UNICORN takes advantage of its streaming capability to generate models that are evolutionary.**</font> <mark>The model captures behavioral changes within a single execution by clustering system activities at various stages of its execution</mark>, but UNICORN does not modify models dynamically during runtime when the attacker may be subverting the system (L3). It is therefore more suitable for long-running systems under potential APT attacks.

---


#### PowerShell Deobfuscation（CCS19）

> 
[5] Zhenyuan Li, et al. Effective and Light-Weight Deobfuscation and Semantic-Aware Attack Detection for PowerShell Scripts, CCS19.


As shown in §2.3, obfuscation is highly effective in bypassing today’s the PowerShell attack detection. <font color="red">**To combat such threat, it is thus highly desired to design a effective and light-weight deobfuscation mechanism for PowerShell scripts.**</font> <mark>In this paper, we are the first to design such a mechanism and use it as the key building block to develop the first semantic-aware PowerShell attack detection system. As shown in Figure 3, the detection process can be divided into three phases:</mark>

---


#### DeepReflect（USENIX Sec21）

> 
[6] Evan Downing, et al. DeepReflect: Discovering Malicious Functionality through Binary Reconstruction, USENIX Sec21.


<mark>The goal of DEEPREFLECT is to identify malicious functions within a malware binary.</mark> **In practice**, it identifies functions which are likely to be malicious by locating abnormal basic blocks (regions of interest - RoI). The analyst must then determine if these functions exhibit malicious or benign behaviors. <mark>There are two primary steps in our pipeline, illustrated in Figure 2: (1) RoI detection and (2) RoI annotation.</mark> RoI detection is performed using an autoencoder, while annotation is performed by clustering all of the RoIs per function and labeling those clusters.

**Terminology**. First, we define what we mean by “malicious behaviors.” <font color="red">**We generate our ground-truth based on identifying core components of our malware’s source code (e.g., denial-of-service function, spam function, keylogger function, command-and-control (C&amp;C) function, exploiting remote services, etc.).**</font> These are easily described by the MITRE ATT&amp;CK framework [9], which aims to standardize these terminologies and descriptions of behaviors. **However**, when statically reverse engineering our evaluation malware binaries (i.e., in-the-wild malware binaries), <mark>we sometimes cannot for-certain attribute the observed low-level functions to these higher-level descriptions.</mark> For example, malware may modify registry keys for a number of different reasons (many of which can be described by MITRE), but sometimes determining which registry key is modified for what reason is difficult and thus can only be labeled loosely as “Defense Evasion: Modify Registry” in MITRE. <mark>Even modern tools like CAPA [3] identify these types of vague labels as well. **Thus in our evaluation**, we denote “malicious behaviors” as functions which can be described by the MITRE framework.</mark>

**RoI Detection**. <mark>The goal of detection is to automatically identify malicious regions within a malware binary.</mark> **For example, we would like to detect the location of the C&amp;C logic rather than detect the specific components of that logic (e.g, the network API calls connect(), send(), and recv()).** <font color="red">**The advantage of RoI detection is that an analyst can be quickly pointed to specific regions of code responsible for launching and operating its malicious actions. Prior work only focuses on creating ad hoc signatures that simply identify a binary as malware or some capability based on API calls alone. This is particularly helpful for analysts scaling their work (i.e., not relying on manual reverse engineering and domain expertise alone).**</font>

**RoI Annotation (标注)**. The goal of annotation is to automatically label the behavior of the functions containing the RoIs. <mark>In other words, this portion of our pipeline identifies what this malicious functionality is doing. Making this labeling nonintrusive to an analyst’s workflow and scalable is <font color="red">**crucial（至关重要）**</font>.</mark> The initial work performed by an analyst for labeling clusters is a **long-tail distribution**. That is, there is relatively significant work upfront but less work as they continue to label each cluster. The advantage of this process is simple: it gives the analyst a way to automatically generate reports and insights about an unseen sample. <mark>For example, if a variant of a malware sample contains similar logic as prior malware samples (but looks different enough to an analyst to be unfamiliar), our tool gives them a way to realize this more quickly.</mark>

---


#### Phishpedia（USENIX Sec21）

> 
[7] Yun Lin, et al. Phishpedia: A Hybrid Deep Learning Based Approach to Visually Identify Phishing Webpages, USENIX Sec21.


Figure 3 provides an overview of our proposed system, Phishpedia. <mark>Phishpedia takes as input a URL and a target brand list describing legitimate brand logos and their web domains; it then generates a phishing target (if the URL is considered as phishing) as output.</mark> We refer to the logo that identifies with the legitimate brand as the identity logo of that brand. **Moreover**, input boxes are the small forms where a user inputs credential information such as username and password.

Given a URL, we first capture its screenshot in a sandbox. <mark>Then, we decompose the phishing identification task into two: an object-detection task and an image recognition task.</mark> First, we detect important UI components, **specifically** identity logos and input boxes, in the screenshot with an object detection algorithm [57, 58] (Section 3.1). **As the next step**, we identify the phishing target by comparing the detected identity logo with the logos in the target brand list via a Siamese model [33]<br/> (Section 3.2). <mark>Once a logo in the target brand list (e.g., that of Paypal) is matched, we consider its corresponding domain (e.g., paypal.com) as the intended domain for the captured screenshot.</mark> **Subsequently（随后）**, we analyze the difference between the intended domain and the domain of the given URL to report the phishing result. Finally, we combine the reported identity logo, input box, and phishing target to synthesize a visual phishing explanation (as shown in Figure 2).

---


#### TextExerciser（S&amp;P21）

> 
[8] Yuyu He, et al. TextExerciser: Feedback-driven Text Input Exercising for Android Applications, S&amp;P21.



<mark>TextExerciser is a feedback-driven text exerciser that understands hints shown on user interfaces of Android apps and then extracts corresponding constraints (约束条件).</mark> The high-level idea of understanding these hints is based on an **observation** that these hints with similar semantics often have a similar syntax structure—and therefore TextExerciser can cluster these hints based on their syntax structures and then extract the constraints from the syntax structure. Now, let us give some details of TextExerciser’s workflow.

The exercising has three phases, seven steps as shown in Figure 2. <mark>First, TextExerciser extracts all the texts in the app’s UI (Step 1) and then identifies static hints via a learning-based method and dynamic hints via a structure-based differential analysis (Step 2).</mark> Second, TextExerciser parses all the extracted hints via three steps: classifying hints into different categories (Step 3), generating syntax trees for each hint (Step 4), and interpreting the generated tree into a constraint representation form (Step 5). **Lastly**, TextExerciser generates a concrete input by feeding constraints into a solver (Step 6), e.g., Z3. <mark>Then, TextExerciser solves the problem, feeds generated inputs back to the target Android app and extracts feedbacks, such as success and another hint (Step 7).</mark> In the case of another hint, TextExerciser will iterate the entire procedure until TextExerciser finds a valid input.

Now let us look at our motivating example in §II again to explain TextExerciser’s workflow. <mark>We start from the sign-up page, which has three text input fields, i.e., “username”, “password” and “confirm password”.</mark> TextExerciser generates a random input to the username field: If the username is used in the database, Yippi returns a “username used” hint. TextExerciser will then parse the hint and generate a new username. The “password” and “confirm password” are handled together by TextExerciser: based on the hint that “Both password has to be the same”1, TextExerciser will convert the hint into a constraint that the value of both fields need to be the same and then generate corresponding inputs.

After TextExerciser generates inputs for the first sign-up page, Yippi asks the user to input a code that is sent to a phone number. TextExerciser will first extract hints related to the phone number page, understand that this is a phone number, and then input a pre-registered phone number to the field. **Next, TextExerciser will automatically extract the code from the SMS and solve the constraints by inputting the code to Yippi.** <mark>In order to find the aforementioned vulnerability in §II, TextExerciser also generates text inputs to the “Change Password” page.</mark> **Particularly**, TextExerciser extracts the password matching hint and another hint that distinguishes old and new passwords, converts them into constraints and then generates corresponding inputs so that existing dynamic analysis tools can find the vulnerability.

> 
<font color="blue">**段落优点**：<br/> 自动生成合适的程序输入是软件自动化测试和动态程序分析的关键环节，而现代软件中文本输入是很常见的一项功能，如何自动化生成有效的文本输入事件来驱动测试是影响动态测试的一大难题。现有方案一般根据用户界面信息和启发式规则生成文本输入，不能根据应用特点理解输入的内容和格式限制，因此产生的文本输入通常不满足程序运行需要。此外，近年来移动应用逐渐将数据处理逻辑移动至云端，对输入信息的审核也大部分位于云服务器中，从而导致传统通过程序分析求解输入文本的方法失效。<br/><br/> 有鉴于此，本文提出了一种面向移动应用的自动文本输入生成方法（TextExerciser）。其基于的insight是：只要文本输入不符合应用要求，应用软件都会将提示信息通过自然语言显示在人机交互界面上。本文通过结合自然语言处理和机器学习等技术，对应用提示信息进行解析，理解提示信息包含的输入限制，并据此自动生成输入文本。该过程是迭代进行，直到产生合适的文本输入。在实验过程中，本文将此文本生成方法与现有的动态测试和分析工具结合，验证了此方法不但能提高应用在测试过程中的代码覆盖，还能找到基于特定输入事件的程序漏洞和隐私泄露问题。相关研究成果发表在信息安全领域顶级会议S&amp;P 2020上。</font>


---


#### DeepBinDiff（NDSS20）

> 
[9] Yue Duan, et al. DEEPBINDIFF: Learning Program-Wide Code Representations for Binary Diffing, NDSS20.


Figure 1 delineates the system architecture of DEEP- BINDIFF. Red squares represent generated intermediate data during analysis. **As shown**, the system takes as input two binaries and outputs the basic block level diffing results. The system solves the two tasks mentioned in Section II-A by using two major techniques. <mark>First, to calculate sim(mi) that quantitatively measures basic block similarity, DEEPBINDIFF embraces an unsupervised learning approach to generate embeddings and <font color="red">**utilizes**</font> them to efficiently calculate the similarity scores between basic blocks.</mark> Second, our system uses a k-hop **greedy (贪心) matching algorithm** to generate the matching M(p1, p2).

<font color="red">**The whole system consists of three major components: 1) pre-processing; 2) embedding generation and 3) code diffing.**</font> <mark>Pre-processing, which can be further divided into two sub-components: CFG generation and feature vector generation, is responsible for generating two pieces of information: inter-procedural control-flow graphs (ICFGs) and feature vectors for basic blocks.</mark> **Once generated**, the two results are sent to embedding generation component that utilizes TADW technique [48] to learn the graph embeddings for each basic block. DEEPBINDIFF then makes use of the generated basic block embeddings and performs a k-hop greedy matching algorithm for code diffing at basic block level.

---


#### Slimium（CCS20）

> 
[10] Chenxiong Qian, Slimium: Debloating the Chromium Browser with Feature Subsetting, CCS20.


<mark>Figure 3 shows an overview of Slimium for debloating Chromium. Slimium consists of three main phases: i) feature-code mapp generation, ii) prompt website profiling based on page visits, and iii) binary instrumentation based on i) and ii).</mark>

**Feature-Code Mapping**. <mark>To build a set of unit features for debloating, we **investigate** source code [35] (Figure 2), previously-assigned CVEs pertaining to Chromium, and external resources [8, 47] for the Web specification standards (Step ① in Figure 3).</mark> **Table 1 summarizes 164 features with four different categories.** <mark>Once the features have been prepared, we generate a feature-code map that aids further debloating from the two sources (①’ and ②’).</mark> From the light-green box in Figure 3, consider the binary that contains two CUs to which three and four consecutive binary functions (i.e., { f0 − f2} and { f3 − f6}) belong, respectively. The initial mapping between a feature and source code relies on a manual discovery process that may miss some binary functions (i.e., from the source generated at compilation). Then, we apply a new means to **explore** such missing functions, <mark>followed by creating a call graph on the IR (Intermediate Representation) (Step ②, Section 4.2).</mark>

**Website Profiling**. The light-yellow box in Figure 3 enables us to <mark>**trace**</mark> exercised functions when running a Chromium process. Slimium <mark>**harnesses**</mark> a website profiling to collect non-deterministic code paths, <mark>which helps to avoid accidental code elimination</mark>. As a baseline, we perform differential analysis on exercised functions by visiting a set of websites (Top 1000 from Alexa [3]) multiple times (Step ③). For example, we mark any function non-deterministic if a certain function is not exercised for the first visit but is exercised for the next visit. <mark>Then, we gather exercised functions for target websites of our interest with a defined set of user activities (Step ④).</mark> **During this process**, profiling may identify a small number of exercised functions that belong to an unused feature (i.e., initialization). As a result, we **obtain** the final profiling results that assist binary instrumentation (③’ and ④’).

**Binary Rewriting**. The final process creates a debloated version of a Chromium binary with a feature subset (Step ⑤ in Figure 3). In this scenario, the feature in the green box has not been needed based on the feature-code mapping and profiling results, **erasing** the functions { f0, f1, f3} of the feature. <mark>As an end user, it is sufficient to take Step ④ and ⑤ for binary instrumentation where pre-computed feature-code mapping and profiling results are given as **supplementary information** (补充信息).</mark>

---


## 三.总结

这篇文章就写到这里了，希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，真心推荐原文，这些大佬真的值得我们学习，继续加油，且看且珍惜！

> 
忙碌的三月结束，时间流逝得真快，也很充实。虽然很忙，但三月还是挤时间回答了很多博友的问题，有咨询技术和代码问题的，有找工作和方向入门的，也有复试面试的。尽管自己科研和技术很菜，今年闭关也非常忙，但总忍不住。唉，性格使然，感恩遇见，问心无愧就好。但行好事，​莫问前程。继续奋斗，继续闭关，。小珞珞这嘴巴，哈哈，每日快乐源泉，晚安娜O(∩_∩)O <br/><br/> <img alt="在这里插入图片描述" height="500" src="https://img-blog.csdnimg.cn/bfdacca1afce411c90b76c40ca29d7ce.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARWFzdG1vdW50,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center" width="550"/>


(By:Eastmount 2022-04-01 晚上12点 http://blog.csdn.net/eastmount/ )
