<a name="ab12ccc5"></a>
## 模型结构

与CBOW模型正好相反，Skip-Gram模型是根据中心单词（target word）来预测其上上下文信息（context words）。如下图所示，为Skip-Gram模型的结构示意图

![Skip-Gram1.png](./img/1599032273405-d7f9db22-3ce7-4de4-9e3b-d68b9d4b2fab.png)

我们仍然使用![](./img/5e6cac270d83c3ef95112fffe478a189.svg)来表示输入层上唯一的那个单词的输入向量，因此，我们对于隐藏层的输出值![](./img/2510c39011c5be704182423e3a695e91.svg)的计算公式与CBOW相同，表示如下：

![](./img/5d0c7afda5696107f6ec86c37ff7abee.svg)

上式显示，![](./img/2510c39011c5be704182423e3a695e91.svg)向量其实就是输入层到隐藏层权重矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)某一行结合输入单词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)的向量拷贝。在输出层，与CBOW模型的输出为单个多项式分布不同的是，Skip-Gram模型在输出层输出了C个多项式分布。每个输出都使用相同的隐藏层到输出层矩阵计算：

![](./img/d12f6984f225556f24cb96114212c3cd.svg)

其中，![](./img/717a1de3acc3d4233286f74a92df375b.svg)表示输出层的第![](./img/4a8a08f09d37b73795649038408b5f33.svg)个panel的第j个单词（何为panel？就是输出层的表示每个上下文单词的神经元的组合，图中一共有![](./img/0d61f8370cad1d412f80b84d143e1257.svg)个context words，所以总共有![](./img/0d61f8370cad1d412f80b84d143e1257.svg)个panel）；![](./img/4a07dc5307a9d2983425161cffa5ecc9.svg)实际上表示的是输出上下文单词（output context words）的第![](./img/4a8a08f09d37b73795649038408b5f33.svg)个单词；![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)是唯一的输入单词；![](./img/6d955048c9d4a59e88b138db6d439589.svg)为输出层的第![](./img/4a8a08f09d37b73795649038408b5f33.svg)个panel上的第![](./img/363b122c528f54df4a0446b6bab05515.svg)个神经单元的概率输出值；![](./img/010e3c3c9a5c415e575bbc04c97dcea3.svg)表示的是输出层第![](./img/4a8a08f09d37b73795649038408b5f33.svg)个panel的第![](./img/363b122c528f54df4a0446b6bab05515.svg)个神经元的输入值；由于输出层的所有panels共享同一权重矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)，因此：

![](./img/ab6118a75dd868c50ad310cb39287e5b.svg)

其中，![](./img/5abeb80c246d9269baa19d1af1c6ebc9.svg)为词汇表第![](./img/363b122c528f54df4a0446b6bab05515.svg)个单词![](./img/e8100be07fa5419af6c6738b934dfca0.svg)的输出向量；同样，它也是取自于隐藏层到输出层权重矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)的一列。

<a name="28151a39"></a>
## 模型训练

Skip-Gram模型参数更新公式的推导过程与One-word context 模型的推导过程大体上一样。这里我们将损失函数变为：

![](./img/3c040153889bb64911ae98a4676628b0.svg)<br />![](./img/a96fbff88a63ba303f8ec46f52eb74d0.svg)<br />![](./img/8b5f8ae5a2228a37cfb6780d2da33095.svg)

其中，![](./img/190342fe91a5affe84e30e6236416397.svg)为第![](./img/4a8a08f09d37b73795649038408b5f33.svg)个输出层输出的上下文单词在词汇表中的真实索引。

在得到损失函数![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)之后，我们对输出层的每一个panel上的所有激活单元的输入值![](./img/010e3c3c9a5c415e575bbc04c97dcea3.svg)均求其关于![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)的偏导数：

![](./img/72fd5c856ca4f1be12627e3c5de01b22.svg)

其中![](./img/ee2ad8a6c9a7b6736e340155977b39b6.svg)为输出层神经元的预测误差。为了简化符号，我们定义一个![](./img/5206560a306a2e085a437fd258eb57ce.svg)维的向量![](./img/dec0aecc2aedb437a5273c489c0f575a.svg)作为所有上下文单词的预测误差之和，![](./img/9ad1d8bfb0391cabab19baecb67e4f55.svg)用公式定义如下：

![](./img/55b3981eeff47875763f26dcff290b97.svg)

接下来，我们计算隐藏层到输出层权重矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)关于![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)的偏导数为：

![](./img/211df4c07840a1cc9161005adbd7ff32.svg)

<a name="98d2cd97"></a>
### 隐藏层到输出层权重更新

这样，我们就得到了隐藏层到输出层权重矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)的参数更新公式为：

![](./img/6a3e68f8fe2c4f551a2f401bff94e4a9.svg)

或者表示为

![](./img/ceeaeaf1619ff678310a8d2bd848698a.svg)

上述参数更新公式的直观概念理解与CBOW无二，除了一点就是：输出层的预测误差的计算是基于多个上下文单词context words，而不是单个目标单词 target word；需注意的是对于每一个训练样本，我们都要利用该参数更新公式来更新隐藏层到输出层权重矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)的每个元素。

<a name="b1e28030"></a>
### 输入层到隐藏层权重更新

同样，对于输入层到隐藏层权重矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的参数更新公式的推导过程，除了考虑要将预测误差![](./img/b6f19bd9cacf5cb9b2c03d5615325e07.svg)替换为![](./img/9ad1d8bfb0391cabab19baecb67e4f55.svg)外，其他也与CBOW类似。这里我们直接给出更新公式：

![](./img/5c6cb1134232ef206280486fa91bca37.svg)

其中，![](./img/088a2013906137902c9832d2f5a3a940.svg)是一个![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)维向量，组成该向量的每一个元素可以用如下公式表示：

![](./img/4bf84715d0c4a865de87016329097b72.svg)

<a name="Source"></a>
## Source

[https://arxiv.org/pdf/1411.2738.pdf](https://arxiv.org/pdf/1411.2738.pdf)<br />[https://blog.csdn.net/lanyu_01/article/details/80097350](https://blog.csdn.net/lanyu_01/article/details/80097350)<br />[https://www.cnblogs.com/pinard/p/7160330.html](https://www.cnblogs.com/pinard/p/7160330.html)
