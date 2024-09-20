我们已经知道word2vec也使用了CBOW与Skip-Gram来训练模型与得到词向量，但是并没有使用传统的DNN模型。在Hierarchical Softmax中，使用的数据结构是用霍夫曼树来代替隐藏层和输出层的神经元，霍夫曼树的叶子节点起到输出层神经元的作用，叶子节点的个数即为词汇表的小大。 而内部节点则起到隐藏层神经元的作用。霍夫曼树建立过程如下：

输入：权值为![](./img/bd9c2bec39e4abced9dbebf0843c455f.svg)的![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个节点

输出：对应的霍夫曼树

1. 将![](./img/bd9c2bec39e4abced9dbebf0843c455f.svg)看作是有![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)颗树的森林，每个树仅有一个节点。
2. 在森林中选择根节点权值最小的两棵树进行合并，得到一个新的树，这两颗树分布作为新树的左右子树。新树的根节点权重为左右子树的根节点权重之和。
3. 将之前的根节点权值最小的两棵树从森林删除，并把新树加入森林。
4. 重复步骤（2）和（3）直到森林里只有一颗树为止。

我们有(a,b,c,d,e,f)共6个节点，节点的权值分布是(16,4,8,6,20,3)。

首先是最小的b和f合并，得到的新树根节点权重是7.此时森林里5棵树，根节点权重分别是16,8,6,20,7。此时根节点权重最小的6,7合并，得到新子树，依次类推，最终得到下面的霍夫曼树：

![Hierarchical Softmax1.png](./img/1599124103305-844f8691-d5c7-4a17-ac6f-3639fad4ca23.png)

霍夫曼树的好处是，一般得到霍夫曼树后我们会对叶子节点进行霍夫曼编码，由于权重高的叶子节点越靠近根节点，而权重低的叶子节点会远离根节点，这样我们的高权重节点编码值较短，而低权重值编码值较长。这保证的树的带权路径最短，也符合我们的信息论，即我们希望越常用的词拥有更短的编码。如何编码呢？一般对于一个霍夫曼树的节点（根节点除外），可以约定左子树编码为0，右子树编码为1.如上图，则可以得到c的编码是00。

在word2vec中，约定编码方式和上面的例子相反，即约定左子树的编码为1，右子树的编码为0。同时约定左子树的权重不小于右子树的权重。
<a name="JC9Za"></a>
# 基于Hierarchical Softmax的模型概述

我们先回顾下传统的神经网络词向量语言模型，里面一般有三层，输入层（词向量），隐藏层和输出层（softmax层）。里面最大的问题在于从隐藏层到输出的softmax层的计算量很大，因为要计算所有词的softmax概率，再去找概率最大的值。这个模型如下图所示。其中![](./img/5206560a306a2e085a437fd258eb57ce.svg)是词汇表的大小

![Hierarchical Softmax2.png](./img/1599124137071-80d9c411-d855-42d5-a5a4-2d9c36589e21.png)

word2vec对这个模型做了改进，首先，对于从输入层到隐藏层的映射，没有采取神经网络的线性变换加激活函数的方法，而是采用简单的对所有输入词向量求和并取平均的方法。比如输入的是三个4维词向量：![](./img/edd31d7ba37ddfa27a8546748800a1f4.svg)，![](./img/56eb3afb43c202a255b52ad19baff813.svg)，![](./img/48c3e3a6f6158ae4a2a075669c06d37e.svg)，那么我们word2vec映射后的词向量就是![](./img/022cd16488660412343efdbd4b45f0f1.svg)。由于这里是从多个词向量变成了一个词向量。

第二个改进就是从隐藏层到输出的softmax层这里的计算量改进。为了避免要计算所有词的softmax概率，word2vec采样了霍夫曼树来代替从隐藏层到输出softmax层的映射。理解如何映射就是理解word2vec的关键所在了。

首先我们根据语料库创建一棵霍夫曼树，比如用文本中每个词的词频当做权重，生成霍夫曼树。由于我们把之前所有都要计算的从输出softmax层的概率计算变成了一棵二叉霍夫曼树，那么我们的softmax概率计算只需要沿着树形结构进行就可以了。如下图所示，我们可以沿着霍夫曼树从根节点一直走到我们的叶子节点的词![](./img/c172e309535f6ff639b845bddf5e5319.svg)。

![Hierarchical Softmax3.png](./img/1599124153723-5052c46b-2f4a-4f84-b601-90c2cd26cfc7.png)

和之前的神经网络语言模型相比，我们的霍夫曼树的所有内部节点就类似之前神经网络隐藏层的神经元，其中，根节点的词向量对应我们的投影后的词向量，而所有叶子节点就类似于之前神经网络softmax输出层的神经元，叶子节点的个数就是词汇表的大小。在霍夫曼树中，隐藏层到输出层的softmax映射不是一下子完成的，而是沿着霍夫曼树一步步完成的，因此这种softmax取名为"Hierarchical Softmax"。

如何“沿着霍夫曼树一步步完成”呢？在word2vec中，我们采用了二元逻辑回归的方法，即规定沿着左子树走，那么就是负类(霍夫曼树编码1)，沿着右子树走，那么就是正类(霍夫曼树编码0)。判别正类和负类的方法是使用sigmoid函数，即：

![](./img/f2b31e3c82bf7710f2cfa02cc363a7fd.svg)

其中![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)是当前内部节点的词向量，而![](./img/2554a2bb846cffd697389e5dc8912759.svg)则是我们需要从训练样本求出的逻辑回归的模型参数。

使用霍夫曼树有什么好处呢？首先，由于是二叉树，之前计算量为![](./img/5206560a306a2e085a437fd258eb57ce.svg)现在变成了![](./img/d6dd3458a48c69a046f8eb7a8dc63b4c.svg)。第二，由于使用霍夫曼树是高频的词靠近树根，这样高频词需要更少的时间会被找到，这符合我们的贪心优化思想。

容易理解，被划分为左子树而成为负类的概率为![](./img/43e603683c93b716a26fecb730b4569e.svg)。在某一个内部节点，要判断是沿左子树还是右子树走的标准就是看![](./img/a1016c83f1c6e3b2343a1fe7fbdbbcbf.svg)和![](./img/c3ea71461df7eba137eac2b23075f0bb.svg)谁的概率值大。而控制![](./img/a1016c83f1c6e3b2343a1fe7fbdbbcbf.svg)和![](./img/c3ea71461df7eba137eac2b23075f0bb.svg)谁的概率值大的因素一个是当前节点的词向量，另一个是当前节点的模型参数![](./img/2554a2bb846cffd697389e5dc8912759.svg)。

对于上图中的![](./img/c172e309535f6ff639b845bddf5e5319.svg)，如果它是一个训练样本的输出，那么我们期望对于里面的隐藏节点![](./img/518efdbb7b92af0dbe591f4f1fd5b0b2.svg)的![](./img/a1016c83f1c6e3b2343a1fe7fbdbbcbf.svg)概率大，![](./img/e497136a6563f8ffda7d46fa20f45a8f.svg)的![](./img/a1016c83f1c6e3b2343a1fe7fbdbbcbf.svg)概率大，![](./img/4abdac3eb2e435bed89b9033394daa38.svg)的![](./img/c3ea71461df7eba137eac2b23075f0bb.svg)概率大。

回到基于Hierarchical Softmax的word2vec本身，我们的目标就是找到合适的所有节点的词向量和所有内部节点![](./img/2554a2bb846cffd697389e5dc8912759.svg)，使训练样本达到最大似然。如何达到最大似然，来看下一节的阐述。

<a name="ce22r"></a>
# 基于Hierarchical Softmax的模型梯度计算

接上节模型概述的例子，我们用最大似然法来寻找所有节点的词向量和所有内部节点![](./img/2554a2bb846cffd697389e5dc8912759.svg)，我们期望最大化下面的似然函数（我们期望对于里面的隐藏节点![](./img/518efdbb7b92af0dbe591f4f1fd5b0b2.svg)的![](./img/a1016c83f1c6e3b2343a1fe7fbdbbcbf.svg)概率大，![](./img/e497136a6563f8ffda7d46fa20f45a8f.svg)的![](./img/a1016c83f1c6e3b2343a1fe7fbdbbcbf.svg)概率大，![](./img/4abdac3eb2e435bed89b9033394daa38.svg)的![](./img/c3ea71461df7eba137eac2b23075f0bb.svg)概率大）：

![](./img/192cbe3f95643029b3c1830e992f9a6a.svg)

对于所有的训练样本，我们期望最大化所有样本的似然函数乘积。

为了便于我们后面一般化的描述，我们定义输入的词为![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)，其从输入层词向量求和平均后的霍夫曼树根节点词向量为![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)，从根节点到![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)所在的叶子节点，包含的节点总数为![](./img/9bfd282c92de420bfd85385de96f8554.svg)，![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)在霍夫曼树中从根节点开始，经过的第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个节点（不包括根节点）表示为![](./img/01697c60b81a1fecbee93b9fa842dbe1.svg)，对应的霍夫曼编码为![](./img/17301796069046609412cbb36eb1eced.svg)。而该节点对应的模型参数表示为![](./img/6a0f7715e821ad97126899a9263071c8.svg)，其中![](./img/ede41fb5c3c002b325c09926b4e5ff08.svg)，没有![](./img/403cf895517cfd72adfc42e5459d4285.svg)是因为模型参数仅仅针对霍夫曼树的内部节点。

定义![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)经过的霍夫曼树某一节点![](./img/363b122c528f54df4a0446b6bab05515.svg)的逻辑回归概率为![](./img/99a4c1d6abb41365b67c433bc7d4ba96.svg)，其表达式为

![](./img/6390aa1f84bb868b506fbf102c4592d3.svg)

那么对于某一个目标输出词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)，其最大似然为：

![](./img/19836286ec744f5d5f21fd2082c0450d.svg)

我们可以看到，每一个词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)都对应着自己的一套逻辑回归参数![](./img/fb8ded21e55845882b1147b7be342f77.svg)。

在word2vec中，由于使用的是随机梯度上升法，所以并没有把所有样本的似然乘起来得到真正的训练集最大似然，仅仅每次只用一个样本更新梯度，这样做的目的是减少梯度计算量。这样我们可以得到![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的对数似然函数![](./img/d20caec3b48a1eef164cb4ca81ba2587.svg)如下：

![](./img/534849d68fd01a2ad0ee12d7ec016c99.svg)

要得到模型中![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)词向量和内部节点的模型参数![](./img/2554a2bb846cffd697389e5dc8912759.svg)，我们使用梯度上升法即可。首先我们求模型参数![](./img/7fe6c52798ec22be47be446e55cf13f4.svg)的梯度：

![](./img/89d07962b67381eeaef3a2d9fe048275.svg)<br />![](./img/12a67082280e375ac707eb0d5d0b1c48.svg)<br />![](./img/1ab744e2a7dc2368c700321998a0ca77.svg)

在这里，用到了逻辑回归里的一个公式：![](./img/fbf266749fcff3acddad73bc6be4a66e.svg)

同样的方法，可以求出![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)的梯度表达式如下：

![](./img/7f955cfabb3ab7d28780fa8806f29d3d.svg)

我们的最终目的是求得词典中每个词的词向量，而这里的![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)表示的是![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的上下文Context各词词向量的均值，那么，如何利用![](./img/6258cf31878e7f8554a13c879d1f3cc6.svg)来对![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的上下文context中每个词向量![](./img/4b0ca67bd5957a08f1587cb222c00e3b.svg)进行更新呢？word2vec的做法很简答，直接取

![](./img/79c2c132f69ee650aa99d4f68f1b3cfa.svg)

即把![](./img/6f5d50c6486aef37f976f17149431150.svg)贡献到Context中每一个词的词向量中。这个很好理解，既然![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)本身就是Context中各个词向量的累加平均，求完梯度后当然也应该贡献到每个分量上去。

<a name="WIgRp"></a>
# 基于Hierarchical Softmax的CBOW模型

由于word2vec有两种模型：CBOW和Skip-Gram，我们先看看基于CBOW模型时， Hierarchical Softmax如何使用。首先我们要定义词向量的维度大小![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)，以及CBOW的上下文大小![](./img/073169ca5cb4d45b1d0e56246c004216.svg)，这样我们对于训练样本中的每一个词，其前面的![](./img/4a8a08f09d37b73795649038408b5f33.svg)个词和后面的![](./img/4a8a08f09d37b73795649038408b5f33.svg)个词作为CBOW模型的输入，该词本身作为样本的输出，期望softmax概率最大。

在做CBOW模型前，我们需要先将词汇表建立成一棵霍夫曼树。

对于从输入层到隐藏层，这一步比较简单，就是对![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)周围的![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个词向量求和取平均即可，即

![](./img/625dd3c3c88edf006bba7e82e2e50c33.svg)

第二步，通过梯度上升法来更新我们的![](./img/7fe6c52798ec22be47be446e55cf13f4.svg)和![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)，注意，这里的![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)是由![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个词向量相加平均而成，我们做梯度更新完毕后会用梯度项直接更新原始的各个![](./img/fba807cbe94421bea76d395e28a68bd7.svg)，即

![](./img/a914e5350e3322a4d99eeb0fd6ff102a.svg)<br />![](./img/4a03eda6304226a3c785a13d0fdf7a1f.svg)

其中![](./img/ffe9f913124f345732e9f00fa258552e.svg)为梯度上升法的步长。

算法流程：

输入：基于CBOW的语料训练样本，词向量的维度大小![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)，CBOW的上下文大小![](./img/073169ca5cb4d45b1d0e56246c004216.svg)，步长![](./img/ffe9f913124f345732e9f00fa258552e.svg)

输出：霍夫曼树的内部结点模型参数![](./img/2554a2bb846cffd697389e5dc8912759.svg)，所有的词向量![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)

1. 基于语料训练样本建立霍夫曼树
2. 随机初始化所有的参数模型![](./img/2554a2bb846cffd697389e5dc8912759.svg)，所有的词向量![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)
3. 进行梯度上升迭代过程，对于训练集中的每一个样本（Context(w)，w）做如下处理：
   1. ![](./img/98ccd35dffe5ffbd05fc54274b6facef.svg)，计算![](./img/625dd3c3c88edf006bba7e82e2e50c33.svg)
   2. for  ![](./img/68f30d6b3878080e01c9e44eeb1245f7.svg)  to  ![](./img/9bfd282c92de420bfd85385de96f8554.svg)，计算
      1. ![](./img/2f601fdf326be8790cc03356c91157b8.svg)
      2. ![](./img/6c608ba94f9cbcadee8982d364e71c7c.svg)
      3. ![](./img/cc5d74377eaca39da4a4a92e3f8e83e5.svg)
      4. ![](./img/40cb82ee9e581ec908ddf3182c144520.svg)
   3. 对于Context中的每一个词向量![](./img/1ba8aaab47179b3d3e24b0ccea9f4e30.svg)(共![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个)进行更新：
      1. ![](./img/b350f5449c479cce2bf86eee23dfc933.svg)
   4. 如果梯度收敛，则结束梯度迭代，否则回到步骤3.继续迭代
<a name="sw7Vf"></a>
# 基于Hierarchical Softmax的Skip-Gram模型

现在我们先看看基于Skip-Gram模型时， Hierarchical Softmax如何使用。此时输入的只有一个词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)，输出的为![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个词向量Context。

我们对于训练样本的每一个词，该词本身作为样本的输入，其前面的![](./img/4a8a08f09d37b73795649038408b5f33.svg)个词和后面的![](./img/4a8a08f09d37b73795649038408b5f33.svg)个词作为了Skip-Gram模型的输出，期望这些词的softmax概率比其他的词大。

第一步，在做Skip-Gram模型前，我们需要先将词汇表建立成一棵霍夫曼树。

对于从输入层到隐藏层，这一步比CBOW简单，只有一个词，即![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)就是词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)对应的输入词向量。

第二步，通过梯度上升法来更新我们的![](./img/7fe6c52798ec22be47be446e55cf13f4.svg)和![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)，注意这里的![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)周围有![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个词向量，此时如果我们期望![](./img/ab137ff1ac4728cfd493d3e529f93f49.svg)最大。此时我们注意到由于上下文是相互的，在期望![](./img/ab137ff1ac4728cfd493d3e529f93f49.svg)最大化的同事，反过来我们也期望![](./img/9902efa68644c747a35d8fe0dce5b7f1.svg)最大。那么是使用![](./img/14a5691a6b337798440b8be200b247a1.svg)好还是![](./img/fe87706af18f572172268409b03184b7.svg)好呢，word2vec使用了后者，这样做的好处就是在一个迭代窗口内，我们不是只更新![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)一个词，而是![](./img/413236853f97be0170dc852be11c9824.svg)共![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个词。这样整体的迭代会更加均衡。因为这个原因，Skip-Gram模型并没有和CBOW模型一样对输入进行迭代更新，而是对![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个输出进行迭代更新。

算法流程：

输入：基于Skip-Gram的语料训练样本，词向量的维度大小![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)，Skip-Gram的上下文大小![](./img/073169ca5cb4d45b1d0e56246c004216.svg)，步长![](./img/ffe9f913124f345732e9f00fa258552e.svg)

输出：霍夫曼树的内部结点模型参数![](./img/2554a2bb846cffd697389e5dc8912759.svg)，所有的词向量![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)

1. 基于语料训练样本建立霍夫曼树
2. 随机初始化所有的参数模型![](./img/2554a2bb846cffd697389e5dc8912759.svg)，所有的词向量![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)
3. 进行梯度上升迭代过程，对于训练集中的每一个样本（w，Context(w)）做如下处理：
   1. for  ![](./img/5905475576a21ecdafdaab879ff45aff.svg)  to ![](./img/073169ca5cb4d45b1d0e56246c004216.svg)：
      1. ![](./img/98ccd35dffe5ffbd05fc54274b6facef.svg)
      2. for  ![](./img/68f903b0d1c3568e8aeb18c7867a5ef0.svg)  to ![](./img/9bfd282c92de420bfd85385de96f8554.svg)，计算：
         1. ![](./img/41f0c59256fee27cca7abcffad57cff9.svg)
         2. ![](./img/f79263922212c72420f99b6c10eaad25.svg)
         3. ![](./img/cc5d74377eaca39da4a4a92e3f8e83e5.svg)
         4. ![](./img/7ab40dcf5d79e3e1e04c23dfdd3254b5.svg)
      3. ![](./img/b350f5449c479cce2bf86eee23dfc933.svg)
   2. 如果梯度收敛，则结束梯度迭代，算法结束，否则回到步骤a.继续迭代
<a name="EUdMA"></a>
# 基于Hierarchical Softmax模型缺点

![Hierarchical Softmax4.png](./img/1599124185455-042e6118-d43d-44c1-a0ca-f39f24c107d9.png)

（1）如上图，训练完![](./img/c172e309535f6ff639b845bddf5e5319.svg)后，![](./img/d003dbf70780f4259519d7435049d386.svg)结点中的参数进行了改变，保证了上下文(Context)和目标词(target word)对的最大似然，但是再训练![](./img/db007d6a923c2909d42c4292bffca5f0.svg)时，路径上的结点参数又进行了改变，此时不能保证还能满足训练![](./img/c172e309535f6ff639b845bddf5e5319.svg)时的最大似然，即，不能保证全局最优解，即每个结点的参数保证每个叶结点都可满足最大似然的。

（2）如果我们的训练样本里的中心词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)是一个很生僻的词，那么就是在霍夫曼是中辛苦向下走了很久。

<a name="DCvvg"></a>
# Source

[https://arxiv.org/pdf/1411.2738.pdf](https://arxiv.org/pdf/1411.2738.pdf)<br />[https://blog.csdn.net/lanyu_01/article/details/80097350](https://blog.csdn.net/lanyu_01/article/details/80097350)<br />[https://www.cnblogs.com/pinard/p/7160330.html](https://www.cnblogs.com/pinard/p/7160330.html)<br />[https://blog.csdn.net/anshuai_aw1/article/details/84241279](https://blog.csdn.net/anshuai_aw1/article/details/84241279)<br />[https://zhuanlan.zhihu.com/p/53194407](https://zhuanlan.zhihu.com/p/53194407)
