# 原创
：  [论文阅读] (27) AAAI20 Order Matters: 基于图神经网络的二进制代码相似性检测（腾讯科恩实验室）

# [论文阅读] (27) AAAI20 Order Matters: 基于图神经网络的二进制代码相似性检测（腾讯科恩实验室）

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇文章介绍Excel论文可视化分析基础知识。这篇文章将带来AAAI20腾讯科恩实验室的经典工作——Order Matters，提出语义感知（Semantic-Aware）神经网络来实现二进制代码相似性检测，希望这篇文章对您有所帮助。一方面自己英文太差，只能通过最土的办法慢慢提升，另一方面是自己的个人学习笔记，并分享出来希望大家批评和指正。这些大佬是真的值得我们去学习，献上小弟的膝盖~fighting！**</font>

**原文作者**：Zeping Yu, Rui Cao, Qiyi Tang, Sen Nie, Junzhou Huang, Shi Wu<br/> **原文标题**：Order Matters: Semantic-Aware Neural Networks for Binary Code Similarity Detection<br/> **原文链接**：[https://ojs.aaai.org/index.php/AAAI/article/view/5466](https://ojs.aaai.org/index.php/AAAI/article/view/5466)<br/> **发表会议**：AAAI 2020<br/> **官方博客**：[科恩 - AAAI-20论文解读：基于图神经网络的二进制代码分析](https://keenlab.tencent.com/zh/2019/12/10/Tencent-Keen-Security-Lab-Order-Matters/)

**除了原文和作者的理解，本文还参考了下面两位老师的博客，再次感谢，向老师和好友们学习。**

> 
<mark>**作者感受：**</mark><br/> <mark>**这篇论文的框架风格和我的很像，非常值得我学习，尤其是安全和AI的结合。同时，文中的英文表述、创新点、模型设计以及实验评估也值得学习。**</mark>


腾讯科恩实验室官方博客：

#### 文章目录

**前文赏析：**

---


## 一.摘要

<mark>二进制代码相似性检测是计算机安全领域中的一项重要（essential）任务，其目标是检测二进制函数的相似性。</mark>

<font color="blue">**传统方法通常使用图匹配（graph matching）算法，但速度缓慢且不准确（inaccurate）。近年来，基于神经网络的研究方法已取得巨大的成就（have made great achievements）。首先将一个二进制函数表示为一个具有手动选择块特征的控制流图（CFG），然后采用图神经网络（GNN）来计算图嵌入（graph embedding）。虽然这些方法非常有效（effective and efficient），但它们不能足够地捕获二进制代码的语义信息。**</font>

<font color="red">**本文提出一种语义感知（semantic-aware）神经网络来提取二进制代码的语义信息，其输入是以基本块为单位的CFG图。具体而言，我们使用BERT在一个令牌级任务（token-level）、一个块级任务（block-level）和两个图级任务（graph-level）上对二进制代码进行预训练。此外，我们发现CFG节点的顺序对于图的相似度检测很重要，因此我们在邻接矩阵上采用卷积神经网络（CNN）来提取顺序信息。**</font>

<mark>我们用四个数据集在两个任务上进行了实验。结果表明，我们的方法优于最先进（state-of-art）的模型。</mark>

---


## 二.引言

> 
由于每篇论文的引言都非常重要，会告诉大家为什么有这个工作，以及这个工作做了什么，有什么贡献。因此该部分作者会全文翻译，后续章节则介绍重点内容。


<mark>**二进制代码相似性检测（Binary code similarity detection）**</mark> 旨在检测两个给定的二进制函数是否相似 [不访问源代码]。二进制代码分析被广泛应用于计算机安全领域，譬如代码克隆检测（code clone detection）、漏洞发现（vulnerability discovery）、恶意软件检测等。

传统方法采用 **图匹配算法**（Liu et al. 2006）来计算两个函数的相似度。然而，这些基于图匹配的方法速度缓慢，并且可能很难适应不同的应用程序。随着近年来深度学习算法的发展，研究者尝试在控制流图（CFG）上使用图神经网络算法，并取得不错的效果。

**文献[1]提出一种基于神经网络的方法 `Gemine`（Xu et al.2017）**，它的输入是两个二进制函数的pair，输出是这两个二进制函数的相似度得分。该工作证明 `Gemine` 的准确率和速度优于现有方法。

> 
[1] Xu X.; Liu C.; Feng Q.; et al. 2017. Neural network-based graph embedding for crossplatform binary code similarity detection. In Proceedings of the 2017 ACM SIGSAC Conference on Computer and Communications Security (CCS), 363–376. ACM.


尽管基于神经网络的模型已经取得了很大的进步，但仍存在一些未被考虑的问题。

为了解决这两个问题，我们提出一个包含三个组件的整体框架：

> 
**原文（优美句子）**：<br/> Even though neural network-based models have achieved a lot, there are several important things that have not been taken into consideration. <mark>Firstly, as shown in Figure 1, each block is represented as a low-dimensional embedding with manually selected features, which will cause the loss of much semantic information</mark>. Secondly, <mark>the order of the nodes plays an important role in representing binary functions, while previous approaches did not design methods to extract it</mark>. To solve these two problems, we propose an overall framework with three components: semantic-aware modeling, structural-aware modeling, and order-aware modeling.


<font color="green">**(1) 在语义感知模块：**</font><br/> **我们使用NLP模型来提取二进制代码的语义信息。CFG块中的token被视为单词，CFG块被视为句子。** 在先前的工作中：

在本文中，我们采用BERT（Devlin et al. 2018）对tokens和blocks进行预训练。与BERT相同，我们对MLM（masked language model）任务的标记进行预训练，并提取所有相邻块对邻接节点预测任务（ANP）进行预训练。与分别学习token向量和block向量不同，**本文方法能够同时学习token向量和block向量**。此外，因为我们的最终目标是生成完整的图表示，所以我们添加了两个图级任务。

我们发现，额外的任务可以帮助提取更多的语义信息，更好地学习块表示。在对块嵌入进行预训练之后，我们将在图级任务上对它们进行微调。

<font color="green">**(2) 在结构感知模块：**</font><br/> 我们使用MPNN（Gilmer等人2017）和GRU（Cho等人2014）更新函数。（Xu et al. 2018）已经证明了图神经网络可以具有像Weisfeiler-Lehman测试一样的区分能力。我们发现，在每个步骤中使用GRU比只使用tanh函数可以存储更多的信息。

<font color="green">**(3) 在顺序感知模块：**</font><br/> 我们尝试设计一种体系结构来提取CFG的节点顺序信息。图2显示函数“_freading”在不同平台x86-64和ARM上编译出的二进制代码的控制流图及邻接矩阵。这两个控制流图的节点顺序是非常相似的，例如node1都与node2和node3相连，node2都与node4和node5相连，而这种相似性可以体现在它们的邻接矩阵上。

通过探索了许多跨平台函数对，我们发现节点顺序的变化很小。在此基础上，我们提出了一种简单的捕获顺序信息的方法，即在邻接矩阵上使用CNN。**我们发现只有一个三层的CNN表现良好。** 我们进一步探索了其他CNN模型，如Resnet（He et al. 2016），并讨论了CNN模型可以从邻接矩阵中学到什么。

> 
O(∩_∩)O<br/> 哈哈，学习如何证明三层CNN更好，想到自己的论文。


<font color="red">**本文的贡献如下：**</font>

---


## 三.相关工作

**1.Graph Neural Networks**<br/> 图神经网络提出来学习节点表示和图表示。典型方法包括：

**2.BERT**<br/> BERT是自然语言处理中最先进的预训练模型，通过Transformer实现。

**3.Binary Code Similarity Detection**<br/> 二进制代码相似度检测是计算机安全研究中的一项重要任务。传统的方法使用图匹配算法来计算图的相似度。然而，这些方法是缓慢和低效的。现有方法缺陷：

---


## 四.本文方法

### 1.Overall Structure

本文模型的输入是二进制代码的控制流图，其中每个块都是一个带有中间表示的令牌序列。模型的总体结构如图3所示，包含semantic-aware 模块、structural-aware模块、order-aware模块。

最后，对两个向量使用concat和MLP得到最终的graph embedding，如公式1所示。

---


### 2.Semantic-aware Modeling

<mark>在语义感知模块中，我们提出了一个包含4个任务的BERT预训练模型来处理CFG</mark>。这个模型有几个优点。

<font color="blue">**本文方法的灵感来自于NLP中的句子嵌入任务，CFG中的块可以看作句子，块中的token可以看做单词。**</font> 这个任务是提取一个句子的embedding，完成这个任务主要有两种方法。

我们使用基于BERT的改进模型来提取CFG上的块向量。如图4所示，在我们的训练前过程中有四个任务：

对于节点内的token序列，我们使用MLM来提取块内的语义信息。MLM是一个令牌级任务，它屏蔽输入层上的令牌，并在输出层上预测它们，和语言模型的方式相同。

邻接节点预测任务（ANP）是一个块级的任务。在图中，块的信息不仅与块本身的信息有关，还与块本身的邻居信息有关，我们希望模型能够学习这些信息。在ANP任务中，在一个图上提取所有相邻的块，并在同一个图中随机抽取几个块，以预测两个块是否相邻。这两个任务（MLM &amp; ANP）类似于原始BERT论文中的MLM &amp; NSP任务（Devlin et al. 2018）。

为了更好地利用graph-level的信息，我们添加了两个辅助监督任务BIG和GC。

---


### 3.Structural-aware Modeling

在从BERT预训练中获得块向量后，我们使用MPNN来计算每个CFG的graph semantic &amp; structural embedding。MPNN有三个步骤：message function（M），update function（U）以及readout function（R）。具体步骤如公式2-公式4所示。

其中，G表示整个图，v表示节点，N(v)表示v的相邻节点。在本文的场景中，节点即是控制流图中的block，图即是经过预训练后表示成block向量的控制流图。本文在message步骤使用MLP，update步骤使用GRU，readout步骤使用sum，如公式5-公式7所示。

---


### 4.Order-aware Modeling

在这个模块中，我们的目标是提取CFG节点的顺序信息，本文使用CNN模型来观察能学到哪些信息。图5显示三个图（块中没有语义信息）和它们的邻接矩阵，它们可以通过添加几个小变化来相互传递。这三个图非常相似，每个图中都有一个三角形特征（图a的节点123，图b的节点234，图c的节点134），这个特征体现在它们的邻接矩阵中。

首先对比5(a)和5(b)，与图5(a)相比，图5(b)加入了节点1，节点顺序依次后移一位，但三角形特征中三个节点的顺序还是连续的，这个特征在邻接矩阵中可以看到，这个1-1-0-1的2x2矩阵仍然存在。

<font color="blue">**CNN能捕捉这些信息：** CNN在训练集中看过很多这种样例后，可以学习到这种平移不变性。</font>

再看图5(c），其加入的节点2打破了原有三角形的节点顺序，然而，在邻接矩阵中我们可以看到它实际上是把原来的2x2矩阵放大成了3x3矩阵，当我们移除第二行和第二列时，仍然可以得到一个1-1-0-1的2x2矩阵。

<font color="blue">**CNN能学习伸缩不变性的信息：** 这种这与图像中的image scaling类似，CNN在训练集中包含足够多样例的情况下，也是可以学到这种伸缩不变性的。</font>

我们讨论了CNN的平移不变性和尺度不变性可以学习节点顺序的小变化。在二进制代码相似性检测任务中，当在不同的平台上编译相同的函数时，节点的顺序通常没有很大的变化。大多数节点顺序的变化都是添加一个节点、删除一个节点或交换几个节点，所以CNN在我们的任务中很有用。除了提高学习节点顺序信息的准确性外，CNN还有几个额外的优点。

> 
**原文：**<br/> Most node order changes are adding a node, deleting a node, or exchanging several nodes, so CNN is useful on our task. <mark>Except the accuracy improvement on learning node order information, CNN has several additional advantages.</mark> First, comparing with traditional graph feature extracting algorithms, using CNN directly on adjacency matrices is much faster. Second, CNN could be added on inputs with different sizes, so it could model different-size graphs without pre-processing such as padding and clipping.


我们在任务中使用Resnet（He et al. 2016），使用一个11层的Resnet，包含3个residual block，所有的feature map大小均为3x3。之后用一个global max pooling层，得到graph order embedding。在此之前不用pooling层，因为输入的图的大小不同。具体如公式8所示。

---


## 五.实验分析

### 1.Datasets

本文在两个任务上进行实验。

请注意，我们的方法对于检测不同编译器（如clang &amp; gcc）上的二进制代码也很有用，在本文中，我们不选择它作为数据集。数据集的基本统计数据如表1所示。任务1是排序问题，因此使用MRR10和Rank1作为评价指标。任务2是分类问题，因此使用准确率作为评价指标。

---


### 2.Compared Methods

因为我们的模型有三个组成部分，所以进行了不同的实验来找出每个部分的影响。

---


### 3.Results

**总体性能（Overall performance）**<br/> 表2和表3显示了不同模型在两个任务上的总体性能。表中第一个分块是整体模型，包括graph kernel，Gemini以及MPNN模型。第二个分块是semantic-aware模块的对比实验，分别使用了word2vec，skip thought，以及BERT，其中BERT2是指原始BERT论文中的两个task（即MLM和ANP），BERT4是指在此基础上加入两个graph-level task（BIG和GC）。第三个分块是对order-aware模块的对比实验，基础CNN模型使用3层CNN以及7、11层的Resnet，CNN_random是对训练集中控制流图的节点顺序随机打乱再进行训练，MPNN_ws是去除控制流图节点中的语义信息（所有block向量设为相同的值）再用MPNN训练。最后是本文的最终模型，即BERT+MPNN+Resnet。

本文提出的模型与Gemini模型相比，在任务1和任务2上的评价指标分数均大幅提升。semantic-aware模块使用NLP模型（word2vec，BERT等）均优于使用人工提取的特征。只使用order-aware时模型也取得了不错的效果。与其它所有模型相比，本文提出的模型均取得了更优的效果。

**语义感知（Model variants for semantic-aware modeling）**<br/> <mark>只看表中第二个分块，BERT的结果优于word2vec和skip thought，因为BERT能在预训练过程中提取更多的信息。</mark> 为了验证BERT预训练是否必要和有效，我们研究了几个变体。首先，基于NLP的训练前块特征（word2vec、skip thought、BERT 2和4）比手动特征具有更好的性能，这表明为CFG块构建复杂的模型是必不可少的。与word2vec和skip thought相比，使用MLM和ANP任务的BERT不仅考虑块级预测，还考虑令牌级预测，并且双向转换器更具有提取有用信息的能力。

BIG任务和GC任务也很有用，其结果增加了1% - 2%。在这两个任务中，块嵌入可以学习图信息，这可能有助于处理图任务。在图6中显示了块嵌入，四个cfg及其块嵌入被设置在四个方向上。我们采用K-means将这些块嵌入聚为四类，不同的集群有不同的颜色（红色、蓝色、绿色和紫色）。我们可以观察到，同一图中的块的趋势是有相同的颜色，而不同的图有不同的主色。

**顺序感知（Model variants for order-aware modeling）**<br/> 只有使用基于cnn的模型才能在这两个任务上都能取得良好的效果。11层Resnet比3层CNN和7层Resnet稍微好一些。与mpnws相比，基于cnn的模型获得了更好的性能。当随机变换节点时，CNN什么也学不到。这意味着CNN模型可以学习节点的顺序

观察表中第三个分块，CNN模型在两个任务上都取得了不错的效果。Resnet11优于Resnet7和CNN3。与MPNN_ws相比，CNN效果更优。随机打乱节点顺序后，CNN模型效果大幅下降，这表示CNN模型确实可以学到节点顺序信息。图7是控制流图pair的例子，这个函数为“ZN12libfwbuilder15RuleElementRGtw13validateC-hildEPNS8FWObjectE“。

<mark>可以看到，左图的节点3在右图中被拆成节点3和节点4，除此之外其它节点的顺序与边的连接方式均相同。经过CNN模型的计算，这两个图的cosine相似度为0.971，排序rank的排名为1。这表明CNN模型可以从邻接矩阵中学到控制流图的节点顺序。</mark>

---


## 六.结论

本文提出了一个新的模型，用于解决二进制代码分析的问题。本文的模型中包含semantic-aware模块，structural-aware模块以及order-aware模块。我们观察到语义信息和节点顺序信息都是控制流图重要的特征。我们使用BERT预训练模型提取语义信息，并使用CNN模型提取节点顺序信息。实验结果表明，本文提出的模型与之前最优的模型相比，取得了更好的效果。

---


## 七.个人感受

<font color="red">**这篇文章就写到这里，希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，继续加油！感恩遇见，且看且珍惜。**</font>

(By:Eastmount 2023-04-06 周四夜于武汉 http://blog.csdn.net/eastmount/ )

---

