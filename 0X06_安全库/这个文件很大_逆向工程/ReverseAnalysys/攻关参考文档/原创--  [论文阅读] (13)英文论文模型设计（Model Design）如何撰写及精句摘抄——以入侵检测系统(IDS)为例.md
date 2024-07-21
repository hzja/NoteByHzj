# 原创
：  [论文阅读] (13)英文论文模型设计（Model Design）如何撰写及精句摘抄——以入侵检测系统(IDS)为例

# [论文阅读] (13)英文论文模型设计（Model Design）如何撰写及精句摘抄——以入侵检测系统(IDS)为例

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇从个人角度介绍英文论文引言如何撰写。这篇文章将从个人角度介绍英文论文模型设计（Model Design）如何撰写，并以入侵检测系统为例（Intrusion Detection System）。一方面自己英文太差，只能通过最土的办法慢慢提升，另一方面是自己的个人学习笔记，并分享出来希望大家批评和指正。希望这篇文章对您有所帮助，这些大佬是真的值得我们去学习，献上小弟的膝盖~fighting！**</font>

这里选择的论文多数为近三年的CCF A和二区以上为主，尤其是顶会顶刊。当然，作者能力有限，只能结合自己的实力和实际阅读情况出发，也希望自己能不断进步，每个部分都会持续补充。可能五年十年后，也会详细分享一篇英文论文如何撰写，目前主要以学习和笔记为主。大佬还请飘过O(∩_∩)O

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

**以入侵检测（intrusion detection）为例，简单介绍几篇经典论文的模型设计写法及组成，通常都是论文框架或算法如何实现的，包括框架图、算法、公式、表格、约束等。**

<mark>(1) Chuanpu Fu, et al. Realtime Robust Malicious Traffic Detection via Frequency Domain Analysis. CCS.</mark>

> 
4 DESIGN DETAILS



<mark>(2) Sunwoo Ahn, et al. Hawkware: Network Intrusion Detection based on Behavior Analysis with ANNs on an IoT Device. DAC</mark>

> 
III. HAWKWARE DESIGN



<mark>(3) Ning Wang, et al. MANDA: On Adversarial Example Detection for Network Intrusion Detection System. IEEE Infocom.</mark>

> 
III. SYSTEM MODEL AND THREAT MODEL

IV. THE MANDA SYSTEM



<mark>(4) Mohammed A. Ambusaidi, et al. Building an Intrusion Detection System Using a Filter-Based Feature Selection Algorithm. IEEE TRANSACTIONS ON COMPUTERS</mark>

> 
4 INTRUSION DETECTION FRAMEWORK BASED ON LEAST SQUARE SUPPORT VECTOR MACHINE



<mark>(5) Jun Zeng, et al. WATSON: Abstracting Behaviors from Audit Logs via Aggregation of Contextual Semantics. NDSS.</mark>

> 
III. WATSON DESIGN



<mark>(6) Ron Bitton, et al. A Machine Learning-Based Intrusion Detection System for Securing Remote Desktop Connections to Electronic Flight Bag Servers. IEEE TDSC.</mark>

> 
3 AN OVERVIEW OF THE PROPOSED NIDS FOR SECURING RDP CONNECTIONS

4 ADETAILED DESCRIPTION OF THE PROPOSED NIDS FOR SECURING RDP CONNECTIONS



---


### 4.整体结构撰写补充

同时，模型设计整体结构和写作细节补充几点：（引用周老师博士课程，受益匪浅）

实验部分同样有很多细节，下一篇文章我们再详细介绍。

---


## 二.入侵检测系统论文引言句子

个人习惯将模型设计结合框架图进行描述，也欢迎大家批评指正。下面主要以CCF A会议和期刊论文为主进行介绍，重点以入侵检测系统（Intrusion Detection System，IDS）领域为主。

### 第0部分：引入

该部分主要是模型设计板块的引入，介绍这部分的核心内容组成。

<mark>In this section, we present the design details of Whisper, i.e., the design of three main modules in Whisper.</mark>

<mark>We will first describe the design goals of our provenance-based intrusion detection system, then we elaborate (详细描述) the details on design and implementation of this system.</mark>

<mark>In this section, we outline our SDN-based system to enforce MUD policies and dynamically inspect exception traffic which is a small fraction of total packets to/from IoT devices. Our system uses as input MUD profiles of 28 consumer IoT devices that we have automatically generated by the MUDgee tool [9] using packet traces collected over several months. We next begin with the architecture of our system.</mark>

<mark>In this section, we present the design of MANDA, the proposed AE detector for ML-based IDS, and explain the rationale behind each design choice. The valid input to an IDS system is real network traffic flows in the problem-space. Therefore, the generated AE should also lie in the same problem-space of IDS. We adapt existing feature-space AE generation algorithms to problem-space algorithms in order to generate AEs that can map back to valid real network events. The key insight for detecting AEs is to identify the discrepancy between true benign samples and AEs. Such an intuition motivates us to investigate AE’s position to the decision-boundary of the IDS model and its position in the traffic manifolds formed by training samples.</mark>

<mark>In this section, the sensor data are analyzed in the frequency domain based on the NPGF. First, the second-order NPGF is introduced in Section III-A to reconstruct the IoT sensor data. To conduct the frequency-domain analysis for the sensor data reconstruction, GFT and its inverse transform are presented in Section III-B, and the frequency-domain analysis for the second-order NPGF is presented in Section III-C.</mark>

<mark>In this section, we describe how the disagreement-based semi-supervised learning works, and introduce how to use it to construct a false alarm filter (simply called DASSL false alarm filter). Then, we present the framework of DAS-CIDS and show how to combine semi-supervised learning with CIDSs.</mark>

<mark>In this section, we first present the intuition of a conventional LSTM architecture. Then, we explain in detail the bidirectional LSTM (BiDLSTM) architecture. We further describe the NSL-KDD dataset used to train our model.</mark>

<mark>In this section, we give a technical description of the features in the data sets used in our experiments. We then explain the details of the techniques employed for adversarial example generation. This leads to the layout of our computational setting.</mark>

---


### 第1部分：System Model / Overview

该部分主要介绍模型的总体框架和主要实现方法，有些论文也将Overview放入模型设计的第一部分，当然很多论文将Overview置于模型设计前单独叙述。

<mark>The overall architecture of Hawkware is depicted in Figure 1. Hawkware is divided into two main modules: the monitor module (MM) and the detection module (DM). MM monitors both the network and device behaviors and extracts the relevant features for the ANN named as Hawknet. DM detects any suspicious behaviors indicating network intrusions by utilizing Hawknet.</mark>

<mark>Hawkware consists of five components and their functions are summarized as follows: (1) the packet analyzer (PA) analyzes network packet headers and extracts relevant features; (2) the system call logger (SCL) records the device behavior and extracts features related to incoming/outgoing network packets; (3) the feature preprocessor (FP) aggregates both extracted features and transfer them as inputs to Hawknet; (4) the Hawknet controller (HC) examines the Hawknet’s outputs and determines the existence of intrusions; (5) the Hawknet quantifies the degree of anomaly.</mark>

<mark>The proposed anomaly detection method is designed to prevent malicious entities from exploiting vulnerabilities in the remote desktop server. The proposed solution specifically focuses on the remote framebuffer protocol [1], which is one of the common protocols used for connecting and interacting with computers remotely.</mark>

<mark>The framework of the proposed intrusion detection system is depicted in Fig. 1. The detection framework is comprised of four main phases: (1) data collection, where sequences of network packets are collected, (2) data preprocessing, where training and test data are preprocessed and important features that can distinguish one class from the others are selected, (3) classifier training, where the model for classification is trained using LS-SVM, and (4) attack recognition, where the trained classifier is used to detect intrusions on the test data.</mark>

<mark>Support Vector Machine is a supervised learning method [38]. It studies a given labeled dataset and constructs an optimal hyperplane in the corresponding data space to separate the data into different classes. Instead of solving the classification problem by quadratic programming, Suykens and Vandewalle [39] suggested re-framing the task of classification into a linear programming problem. They named this new formulation the Least Squares SVM (LS-SVM). LS-SVM is a generalized scheme for classification and also incurs low computation complexity in comparison with the ordinary SVM scheme [40]. One can find more details about calculating LS-SVM in Appendix B, available in the online supplemental material. The following sections explain each phase in detail.</mark>

<mark>Figure 1 summarises the MAGPIE architecture. Its collection phase captures and decodes the data coming from cyber (computation, communication) or physical feeds (e.g., audio, signal strength). It can dynamically activate or deactivate interfaces and decode the corresponding raw feeds, such as sensor readings or network datagrams.</mark>

<mark>Smart homes generate large volumes of usually encrypted data [24] that may differ considerably between different environments. In the transcription phase, MAGPIE considers only meta-data that are consistent across different smart homes. … Moreover, by reading only smart home network communication flow meta-data, MAGPIE is better positioned to preserve privacy. MAGPIE extracts meta-data streams (MDS) based on specific interface datastream parsing logic (e.g., communication/application/sensor protocol) (Figure 2).</mark>

<mark>Fig. 1 shows the architecture of Pagoda. It consists of six components, namely, Provenance collection, Provenance pruning, Provenance storage and maintenance, Rule building and deduplication, Detection process andForensic analysis. The Provenance collection component （部分） is responsible for monitoring the behaviors of the normal/intrusion applications, intercepting the system calls invoked by them and translate these system calls to causality-based provenance records. Then the provenance pruning module omits the provenance records that are not related to intrusion detection to improve the detection accuracy and save the storage space simultaneously. The Provenance storage and maintenance component uses key-value memory database (e.g., Redis [37]) to store rule database and run the provenance-based intrusion detection algorithm to make real-time detection. The Rule building and deduplication module constructs the rule sets for intrusion detection and removes the duplicated strings to make the rule database as small as possible. The Detection process component judges whether the intrusion has happened according to the rule sets and also updates the rule sets according to the detection results. At last, the Forensic analysis module looks for the system vulnerability and intrusion sources by making forward and backward queries.</mark>

<mark>Fig. 3 illustrates the overall process of applying the proposed fine-grained algorithm for evaluating an incoming packet. As can be seen, the process starts with a new incoming packet. First, two types of features, contextual and content-based, are extracted from the packet (step 1). The contextual features are extracted from the header of the packet, and the content-based features are extracted from the packet’s payload. Next, for dimensionality reduction, as well as for improving detection rate, a ‘message type classification’ PCA model is applied on the content-based features (step 2). The resulting PCA features, along with the contextual features, are used as the input vector to a decision tree model.</mark>

<mark>The decision tree model is applied on the packet’s feature vector to classify the packet based on its message type (step 3). Note that in the example provided in Fig. 3 the packet is classified by the decision tree model as a ‘Pointer Event’ packet.</mark>

<mark>Based on the derived message type (‘Pointer Event’), in the next step (step 4) a second PCA model which was initially created for each specific message type, is applied on the original content-based features of the packet.</mark>

<mark>Finally, the resulting PCA features are used by a k-means algorithm and CBLOF model of the specific message type (‘Pointer Event’) in order to assign an anomaly score for the packet.</mark>

<mark>A typical architecture of a ML-based IDS is shown in Fig. 1. Usually, IDS is a passive infrastructure which rarely interferes with the network traffic under monitoring. An IDS sniffs the internal interface of the firewall in a read-only mode and sends alerts to an IDS management server via a read-and-write network interface [33], [34]. As Fig. 1 shows, a ML-based IDS is composed of the following modules [3]:</mark>

---


### 第2部分：数据预处理

<mark>The data obtained during the phase of data collection are first processed to generate the basic features such as the ones in KDD Cup 99 dataset [41]. This phase contains three main stages shown as follows.</mark>

<mark>**Data Transferring.** The trained classifier requires each record in the input data to be represented as a vector of real number. Thus, every symbolic feature in a dataset is first converted into a numerical value. For example, the KDD CUP 99 dataset contains numerical as well as symbolic features. These symbolic features include the type of protocol (i.e., TCP, UDP and ICMP), service type (e.g., HTTP, FTP, Telnet and so on) and TCP status flag (e.g., SF, REJ and so on). The method simply replaces the values of the categorical attributes with numeric values.</mark>

<mark>**Data Normalisation.** An essential step of data preprocessing after transferring all symbolic attributes into numerical values is normalisation. Data normalisation is a process of scaling the value of each attribute into a well-proportioned range, so that the bias in favor of features with greater values is eliminated from the dataset. Data used in Section 5 are standardised. Every feature within each record is normalised by the respective maximum value and falls into the same range of [0-1]. The transferring and normalisation process will also be applied to test data.</mark>

<mark>As mentioned in Section 4.3 above, the NSL-KDD dataset comes with 38 numeric and 3 non-numeric features. However, just as any RNN, the proposed BiDLSTM model only handles numerical data inputs. As a result, there is a need for us to convert all non-numeric features to numeric representations. The features (protocol type, service and flag) are the non-numeric features in the NSL-KDD dataset that require transformation into numeric form. These three features are encoded and assigned integer values unique to each of them. After successfully transforming these features into numeric form, the next appropriate thing is feature scaling. Feature scaling ensures that the dataset is in the normalized form. The values of some features in the NSL-KDD dataset (e.g., src_bytes and dst_bytes) have uneven distribution, so we scale every feature’s values within the range of (0, 1) using Min–Max scaling. By this, we ensure that our classifier does not produce biased outcomes. The expression for the Min–Max feature scaling is as follows:</mark>

<mark>Note that we apply PCA transformation twice in the overall proposed process. In the first task, namely the message type classification, we apply the PCA transformation on all packet payloads in order to bring out the patterns that are useful for differentiating packets by its message types, as well as for dimensionality reduction. In the second task, namely the fine-grained anomaly detection, we apply the PCA transformation on subsets of packets belonging to the same message type; this is performed separately for each message type.</mark>

---


### 第3部分：本文提出的核心算法（特征选择）

<mark>Thus, we need a measure capable of analysing the relation between two variables no matter whether they are linearly or nonlinearly dependent. For these reasons, this work intends to explore a means of selecting optimal features from a feature space regardless of the type of correlation between them.</mark>

<mark>Even though every connection in a dataset is represented by various features, not all of these features are needed to build an IDS. Therefore, it is important to identify the most informative features of traffic data to achieve higher performance. In the previous section using Algorithm 1, a flexible method for the problem of feature selection, FMIFS, is developed. However, the proposed feature selection algorithms can only rank features in terms of their relevance but they cannot reveal the best number of features that are needed to train a classifier. Therefore, this study applies the same technique proposed in [12] to determine the optimal number of required features. To do so, the technique first utilises the proposed feature selection algorithm to rank all features based on their importance to the classification processes.</mark>

<mark>Then, incrementally the technique adds features to the classifier one by one. The final decision of the optimal number of features in each method is taken once the highest classification accuracy in the training dataset is achieved.</mark>

<mark>The selected features for all datasets are depicted in Tables 1 [a, b, c], where each row lists the number and the indexes of the selected features with respect to the corresponding feature selection algorithm. In addition, for KDD Cup 99 and to make a comparison with those systems that have been evaluated on different types of attacks (discussed in Sections 5.5 and 5.6), we construct five classes. One of these classes contains purely the normal records and the other four hold different types of attacks (i.e., DoS, Probe, U2R, R2L), respectively. The proposed feature selection algorithm is applied to the aforementioned classes. The selected features are shown in Table 3.</mark>

---


### 第4部分：模型实现详解

<mark>The proposed solution (i.e., the fine-grained anomaly detection algorithm) for detecting anomalous TCP packets in RFB traffic combines supervised classification techniques, together with unsupervised cluster analysis and anomaly detection methods. The solution consists of two primary phases, the Model Construction Phase in which we utilize legitimate RFB traffic to construct a predictive model which represents the normal protocol behavior; and the Detection Phase in which we apply these models in order to detect anomalous protocol behavior.</mark>

<mark>Now we utilize the statistical clustering algorithm to learn the patterns of the frequency domain features obtained from the feature extraction module with the selected parameters. We train the statistical clustering algorithm with only benign traffic. In the training phase, this module calculates the clustering centers of the frequency domain features and the averaged training loss. In order to improve the robustness of Whisper and reduce false positive caused by the extreme values, we segment the frequency domain feature matrix R with a sampling window of length. We use … to denote the number of samples and … to denote the start points. We average the sampling window on the dimension of the feature sequence and use … to indicate the input of the clustering algorithm. We can obtain:</mark>

<mark>In this module, we extract the frequency domain features from high speed traffic. We acquire the per-packet features of 𝑁 packets from the same flow by polling the high speed packet parser module. We use the mathematical representation similar to Bartos et al. [4] to denote the features.</mark>

<mark>Once the optimal subset of features is selected, this subset is then taken into the classifier training phase where LS-SVM is employed. Since SVMs can only handle binary classification problems and because for KDD Cup 99 five optimal feature subsets are selected for all classes, five LS-SVM classifiers need to be employed. Each classifier distinguishes one class of records from the others. For example the classifier of Normal class distinguishes Normal data from nonNormal (all types of attacks). The DoS class distinguishes DoS traffic from non-DoS data (including Normal, Probe, R2L and U2R instances) and so on. The five LS-SVM classifiers are then combined to build the intrusion detection model to distinguish all different classes.</mark>

---


### 第5部分：公式引出表达

这里仅给出公式的引出句子。

**(1) Chuanpu Fu, et al. Realtime Robust Malicious Traffic Detection via Frequency Domain Analysis. CCS.**

**(2) Mohammed A. Ambusaidi, et al. Building an Intrusion Detection System Using a Filter-Based Feature Selection Algorithm. IEEE TRANSACTIONS ON COMPUTERS.**

**模型评估公式：**

**(3) Ning Wang, et al. MANDA: On Adversarial Example Detection for Network Intrusion Detection System. IEEE INFOCOM.**

**其他句子：**

---


## 三.模型框架图

好的框架图是非常重要的，下面给出IDS比较经典的几个总体框架。

(1) Chuanpu Fu, et al. Realtime Robust Malicious Traffic Detection via Frequency Domain Analysis. CCS.

(2) Ron Bitton, et al. A Machine Learning-Based Intrusion Detection System for Securing Remote Desktop Connections to Electronic Flight Bag Servers. IEEE TDSC.

(3) Tohid Shekari, et al. RFDIDS: Radio Frequency-based Distributed Intrusion Detection System for the Power Grid. NDSS.

(4) Ryan Heartfield, et al. Self-Configurable Cyber-Physical Intrusion Detection for Smart Homes Using Reinforcement Learning. IEEE TIFS.

(5) Congyuan Xu, et al. A Method of Few-Shot Network Intrusion Detection Based on Meta-Learning Framework. IEEE TIFS.

(6) Jun Zeng, et al. WATSON: Abstracting Behaviors from Audit Logs via Aggregation of Contextual Semantics. NDSS.

(7) Ning Wang, et al. MANDA: On Adversarial Example Detection for Network Intrusion Detection System. IEEE Infocom.

(8) Sunwoo Ahn, et al. Hawkware: Network Intrusion Detection based on Behavior Analysis with ANNs on an IoT Device. DAC

(9) Xinghua Li, et al. Sustainable Ensemble Learning Driving Intrusion Detection Model. IEEE TDSC.

(10) Neha Gupta, et al. LIO-IDS: Handling class imbalance using LSTM and improved one-vs-one technique in intrusion detection system. Computer Networks.

(11) Ron Bitton, et al. A Machine Learning-Based Intrusion Detection System for Securing Remote Desktop Connections to Electronic Flight Bag Servers. IEEE TDSC.

---


## 四.总结

这篇文章就写到这里了，希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，真心推荐原文。学安全两年，认识了很多安全大佬和朋友，希望大家一起进步。同时非常感谢参考文献中的大佬们，感谢老师、实验室小伙伴们的教导和交流，深知自己很菜，得努力前行。感恩遇见，且行且珍惜，小珞珞太可爱了，哈哈。

最后感谢CSDN和读者们十年的陪伴，不论外面如何评价CSDN，这里始终是我的家，在这里写文章很温馨，也认识了很多大佬和朋友。此外，个人感觉今年是我近十年文章质量最高的一年，每一篇都写得很用心，都是我的血肉，很多都要自己从零去学习再分享，也希望帮助更多初学者。总之，希望自己还能写二十年，五十年，一辈子。

(By:Eastmount 2021-12-07 晚上12点 http://blog.csdn.net/eastmount/ )
