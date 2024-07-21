
在讲基于Negative Sampling的word2vec模型前，我们先看看Hierarchical Softmax的的缺点。的确，使用霍夫曼树来代替传统的神经网络，可以提高模型训练的效率。但是如果我们的训练样本里的中心词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)是一个很生僻的词，那么就是在霍夫曼是中辛苦向下走了很久。能不能不用搞这么复杂的一颗霍夫曼树，将模型变的更加简单呢

Negative Sampling就是这么一种求解word2vec模型的方法，它摒弃了霍夫曼树，采用了Negative Sampling（负采样）的方法来求解，下面我们就来看看Negative Sampling的求解思路。

<a name="ZdoWY"></a>
# 基于Negative Sampling的模型概述

既然名字叫Negative Sampling（负采样），那么肯定使用了采样的方法。

比如我们有一个训练样本，中心词是![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)，它周围上下文共有![](./img/073169ca5cb4d45b1d0e56246c004216.svg)个词，记为Context(w)。由于这个中心词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的确和Context(w)相关存在，因此它是一个真实的正例。通过Negative Sampling采样，我们可以得到![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个和![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)不同的中心词![](./img/027bd626ae33c9c03dd6690f347dabcb.svg)，这样Context(w)和![](./img/aa38f107289d4d73d516190581397349.svg)就组成了![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个并不真实存在的负例。利用这一个正例和![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个负例，我们进行二元逻辑回归，得到负采样对应每个词![](./img/aa38f107289d4d73d516190581397349.svg)对应的模型参数![](./img/9fe4f6a929e86b5c5a7d19d4a18fc304.svg)，和每个词的词向量。

从上面的描述可以看出，Negative Sampling由于没有采用霍夫曼树，每次只是通过采样![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个不同的中心词做负例，就可以训练模型，因此整个过程要比Hierarchical Softmax简单。我们只需要弄明白两个问题：

1. 如果通过一个正例和neg个负例进行二元逻辑回归
2. 如何进行负采样
<a name="muJwu"></a>
# 基于Negative Sampling的模型梯度计算

Negative Sampling采用了二元逻辑回归来求模型参数，负采样得到![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个负例![](./img/f6874796f28c8f2757d58b2eb3371b9c.svg)，为了统一描述，我们将正例定义为![](./img/ac1052c8c41fa0e8d67714e0723a068b.svg)

在逻辑回归中，我们的正例应该期望满足：

![](./img/38b692e0d2e730308ba333f86d2aa192.svg)

我们的负例期望满足：

![](./img/efbdf8fc3bcf700b66324cce793c191f.svg)

我们期望可以最大化下式：

![](./img/455268d2cc497fa82509edd9df2359f3.svg)

利用逻辑回归和上一章的内容，我们容易写出此时模型的似然函数（因为只有1个正例）：

![](./img/5c30a1c62860f7ffb3c14f9e171e7b2d.svg)

此时对应的对数似然函数为

![](./img/e7aad8abe8de5259e3e9c0b459805a36.svg)

和Hierarchical Softmax类似，我们采用随机梯度上升法，仅仅每次只用一个样本更新梯度，来进行迭代更新得到我们需要的![](./img/237cd38f3ce17afef45db91d0dac43a7.svg)，这里我们需要求出![](./img/237cd38f3ce17afef45db91d0dac43a7.svg)的梯度：

首先我们计算![](./img/e75d2d175b8e12362d9ae9cc638b4542.svg)的梯度：

![](./img/49e0582576f2ab724e5534006d9b29a4.svg)

同样的方法，我们可以求出![](./img/22282ea4c1ee9726c7e87c052bbb20ae.svg)的梯度如下：

![](./img/b662b3e6fb0271c2c9e8249e9c85f266.svg)

有了梯度表达式，我们就可以用梯度上升法进行迭代来一步步的求解我们需要的![](./img/237cd38f3ce17afef45db91d0dac43a7.svg)

<a name="6Urpc"></a>
# Negative Sampling负采样方法

word2vec采样的方法并不复杂，如果词汇表的大小为![](./img/5206560a306a2e085a437fd258eb57ce.svg)，那么我们就将一段长度为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)的线段分成![](./img/5206560a306a2e085a437fd258eb57ce.svg)份，每份对应词汇表中的一个词。当然每个词对应的线段长度是不一致的，高频词对应的线段长，低频次对应的线段短。每个词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的线段长度由下式决定：

![](./img/589bd2fde75ee80618757f5942095fb5.svg)

在word2vec中，分子和分母都取了![](./img/9df743fb4a026d67e85ab08111c4aedd.svg)次幂：

![](./img/3611d2cfed4a65def5b455f800ce82bb.svg)

在采样前，我们将这段长度为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)的线段划分成![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)等份，这里![](./img/e034810efc2c5b19a7412f9b65815cfa.svg)，这样可以保证每个词对应的线段都会划分成对应的小块。而![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)份中的每一份都会落在某个词对应的线段上。在采样的时候，我们只需要从![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)个位置中采样出![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个位置就行，此时采样到的每一个位置对应到的线段所属的词就是我们的负例

![负采样.png](./img/1599205053074-176cc0fb-58a4-4b5a-a179-0980bcc02af0.png)

在word2vec中，![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)取值默认为![](./img/4fe556ec9c8619ad69394e7f975f35bd.svg)

<a name="OwvwK"></a>
# 基于Negative Sampling的CBOW模型

输入：基于CBOW语料，词向量的维度大小![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)，CBOW的上下文大小![](./img/073169ca5cb4d45b1d0e56246c004216.svg)，步长![](./img/ffe9f913124f345732e9f00fa258552e.svg)，负采样个数![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)

输出：所有的词向量![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)

1. 随机初始化所有的模型参数![](./img/2554a2bb846cffd697389e5dc8912759.svg)，所有的词向量![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)
2. 对于每个训练样本![](./img/f7ecbb166e80d7b99288c3e07f553976.svg)，负采样出![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个负例中心词![](./img/027bd626ae33c9c03dd6690f347dabcb.svg)
3. 进行梯度上升迭代过程，对于训练集中的每一个样本![](./img/8f75f5aab8572683dd93bc4fc50305f2.svg)：
   1. ![](./img/98ccd35dffe5ffbd05fc54274b6facef.svg)，计算![](./img/53095f11ce210c05dd6b9bbb50e8d4a0.svg)
   2. for  ![](./img/fc5d9d43c5d58df1cf63bcf7e5f83321.svg)  to ![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg) ：
      1. ![](./img/4baa6f57e9756fd5c8a961d5d71eccf7.svg)
      2. ![](./img/4ed20aaf4d56bdd40ea47bf92d7c2cb2.svg)
      3. ![](./img/9f6f2c54db903ad55ca1656cd0565ed6.svg)
      4. ![](./img/20a201373b2cbf44dda5e00c845ad347.svg)
   3. 对于![](./img/d3095c55e6654807fe19e88dc39c0d01.svg)中每一个词向量![](./img/83b08453f4197d78025b7af0f4b71186.svg)进行更新：
      1. ![](./img/d25ccbdb0dce5a279a06e172966e398e.svg)
   4. 如果梯度收敛，则结束梯度迭代，否则回到步骤3.继续迭代
<a name="gcWxo"></a>
# 基于Negative Sampling的Skip-Gram模型

输入：基于CBOW语料，词向量的维度大小![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)，CBOW的上下文大小![](./img/073169ca5cb4d45b1d0e56246c004216.svg)，步长![](./img/ffe9f913124f345732e9f00fa258552e.svg)，负采样个数![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)

输出：所有的词向量![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)

1. 随机初始化所有的模型参数![](./img/2554a2bb846cffd697389e5dc8912759.svg)，所有的词向量![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)
2. 对于每个训练样本![](./img/f7ecbb166e80d7b99288c3e07f553976.svg)，负采样出![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个负例中心词![](./img/027bd626ae33c9c03dd6690f347dabcb.svg)
3. 进行梯度上升迭代过程，对于训练集中的每一个样本![](./img/8f75f5aab8572683dd93bc4fc50305f2.svg) ：
   1. for  ![](./img/fc5d9d43c5d58df1cf63bcf7e5f83321.svg)  to  ![](./img/073169ca5cb4d45b1d0e56246c004216.svg)：
      1. ![](./img/98ccd35dffe5ffbd05fc54274b6facef.svg)
      2. for  ![](./img/fc5d9d43c5d58df1cf63bcf7e5f83321.svg)  to ![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)：
         1. ![](./img/4baa6f57e9756fd5c8a961d5d71eccf7.svg)
         2. ![](./img/4ed20aaf4d56bdd40ea47bf92d7c2cb2.svg)
         3. ![](./img/9f6f2c54db903ad55ca1656cd0565ed6.svg)
         4. ![](./img/20a201373b2cbf44dda5e00c845ad347.svg)
      3. 对于![](./img/d3095c55e6654807fe19e88dc39c0d01.svg)中每一个词向量![](./img/83b08453f4197d78025b7af0f4b71186.svg)进行更新：
         1. ![](./img/d25ccbdb0dce5a279a06e172966e398e.svg)
   2. 如果梯度收敛，则结束梯度迭代，否则回到步骤a继续迭代
<a name="sfBSN"></a>
# Source

[https://arxiv.org/pdf/1411.2738.pdf](https://arxiv.org/pdf/1411.2738.pdf)<br />[https://blog.csdn.net/lanyu_01/article/details/80097350](https://blog.csdn.net/lanyu_01/article/details/80097350)<br />[https://www.cnblogs.com/pinard/p/7160330.html](https://www.cnblogs.com/pinard/p/7160330.html)<br />[https://blog.csdn.net/anshuai_aw1/article/details/84241279](https://blog.csdn.net/anshuai_aw1/article/details/84241279)<br />[https://zhuanlan.zhihu.com/p/53194407](https://zhuanlan.zhihu.com/p/53194407)
