<a name="1c4fc339"></a>
## XGBoost简介

在数据建模中，经常采用Boosting方法，该方法将成百上千个分类准确率较低的树模型组合起来，成为一个准确率很高的预测模型。这个模型会不断地迭代，每次迭代就生成一颗新的树。但在数据集较复杂的时候，可能需要几千次迭代运算，这将造成巨大的计算瓶颈。

针对这个问题。华盛顿大学的陈天奇博士开发的XGBoost（eXtreme Gradient Boosting）基于C++通过多线程实现了回归树的并行构建，并在原有Gradient Boosting算法基础上加以改进，从而极大地提升了模型训练速度和预测精度。在Kaggle的比赛中，多次有队伍借助XGBoost在比赛中夺得第一。其次，因为它的效果好，计算复杂度不高，在工业界中也有大量的应用。

<a name="ae6c8bcd"></a>
## 监督学习的三要素

因为Boosting Tree本身是一种有监督学习算法，要讲Boosting Tree，先从监督学习讲起。在监督学习中有几个逻辑上的重要组成部件，粗略地可以分为：模型、参数、目标函数和优化算法。

<a name="8000f187"></a>
### 模型

模型指的是给定输入![](./img/1ba8aaab47179b3d3e24b0ccea9f4e30.svg)如何去预测输出![](./img/8d62e469fb30ed435a668eb5c035b1f6.svg)。我们比较常见的模型如线性模型（包括线性回归和Logistic Regression）采用线性加和的方式进行预测

![](./img/d42c12aa647f537c4e87cc56d067b1a2.svg)

这里的预测值![](./img/415290769594460e2e485922904f345d.svg)可以有不同的解释，比如我们可以把它作为回归目标的输出，或者进行sigmoid变换得到概率（即用![](./img/8c5eb0151c49faa60d083456445d6f56.svg)来预测正例的概率），或者作为排序的指标等。一个线性模型根据![](./img/415290769594460e2e485922904f345d.svg)的解释不同（以及设计对应的目标函数），分别用到回归、分类或者排序等场景。

<a name="3d0a2df9"></a>
### 参数

参数就是我们根据模型要从数据里头学习的东西，比如线性模型中的线性系数：

![](./img/4f46c4bef3657afe5c034a9c59a1d06a.svg)

<a name="d8bc27e0"></a>
### 目标函数：误差函数+正则化项

模型和参数本身指定了给定输入我们如何预测，但是没有告诉我们如何去寻找一个比较好的参数，这个时候就需要目标函数函数登场了。一般地目标函数包含两项：一项是损失函数，它说明了我们的模型有多拟合数据；另一项是正则化项，它惩罚了复杂模型。

![](./img/167fa22b77acdfa99e7e8adfbc9d4efd.svg)

1、![](./img/ced2828448a99eaa8bd379e2279cad12.svg)：损失函数![](./img/b71fa745cacce061dbdc0b2d3fc97b60.svg)，常见的损失函数有：

- 平方损失：![](./img/b02a81bec7ce2ed3933f2d4a1da09329.svg)
- Logistic损失：![](./img/7606e36062026a4ddff64daa9c52c2e8.svg)

2、![](./img/5fb80bc534dce73274966e0ed0bebc58.svg)：正则化项，之所以要引入它是因为我们的目标是希望生成的模型能准确地预测新的样本（即应用于测试数据集），而不是简单地拟合训练集的结果（这样会导致过拟合）。所以需要在保证模型“简单”的基础上最小化训练误差，这样得到的参数才具有好的泛化性能。而正则化项就是用于惩罚复杂模型，避免模型过分拟合训练数据。常用的正则有![](./img/1922b8e22bae729ffa04ea46d29ac5b8.svg)正则与![](./img/11cc37d6dae6b905a0a0eb2ff087ae24.svg)正则：

- ![](./img/1922b8e22bae729ffa04ea46d29ac5b8.svg)正则(Lasso)：![](./img/cae51e0955bb2b0b02b3bae5c5e0748b.svg)
- ![](./img/11cc37d6dae6b905a0a0eb2ff087ae24.svg)正则：![](./img/19de0e921fb80fc7ceb383278a89f7fe.svg)

上述目标函数的设计来自于统计学习里面的一个重要概念叫做Bias-variance tradeoff（偏差-方差权衡），比较感性的理解，Bias可以理解为假设我们有无限多数据的时候，可以训练出最好的模型所拿到的误差。而Variance是因为我们只有有限数据，其中随机性带来的误差。目标中误差函数鼓励我们的模型尽量去拟合训练数据，这样相对来说最后的模型会有比较少的Bias。而正则化项则鼓励更加简单的模型。因为当模型简单之后，有限数据拟合出来结果的随机性比较小，不容易过拟合，使得最后模型的预测更加稳定。

<a name="34472ed9"></a>
## XGBoost原理

<a name="91e98b6e"></a>
### 目标函数及二阶泰勒展开

在XGBoost中，选择树模型为基学习器，我们需要正则的对象，或者说需要控制复杂度的对象就是这![](./img/a5f3c6a11b03839d46af9fb43c97c188.svg)颗树，通常树的参数有树的深度，叶子节点的个数，叶子节点值的取值（XGBoost里称为权重weight)。所以，我们的目标函数形式如下：

![](./img/b6b6532f09ae899ee572b48b32175bdb.svg)

其中第一部分是训练损失，如上面所述的平方损失或者Logistic Loss等，第二部分是每棵树的复杂度的和。因为现在我们的参数可以认为是在一个函数空间里面，我们不能采用传统的如SGD之类的算法来学习我们的模型，因此我们会采用一种叫做additive training的方式。即每次迭代生成一棵新的回归树，从而使预测值不断逼近真实值（即进一步最小化目标函数）。每一次保留原来的模型不变，加入一个新的函数![](./img/8fa14cdd754f91cc6554c9e71929cce7.svg)到模型里面：

![](./img/5e6f19ce7f691c8d3a1d5966cece54d9.svg)

所以，对于第![](./img/a5f3c6a11b03839d46af9fb43c97c188.svg)次的目标函数为：

![](./img/d2cb5d4f8fdd3eea5e888111d3e0635c.svg)

其中![](./img/4a8a08f09d37b73795649038408b5f33.svg)为前![](./img/bf20193137085b07680e64a4ed4a7666.svg)棵树的正则化项和，是一个常数。

根据![](./img/41b016b9bd8fb4174d391da6e5c52d65.svg)，进一步为：

![](./img/9cc00939150c0a110cc3cf6167c11ce1.svg)

上面的式子意思很明显，只需要寻找一棵合适的树![](./img/f316c7ab2d727b1e1d8a197b73a49e63.svg)使得目标函数最小。然后不断地迭代![](./img/a5f3c6a11b03839d46af9fb43c97c188.svg)次就可以完成![](./img/a5f3c6a11b03839d46af9fb43c97c188.svg)个学习器的训练。那么我们这棵树到底怎么找呢？在GBDT里面（当然GBDT没有正则），我们的树是通过拟合上一棵树的负梯度值，建树的时候采用的启发式准则，如MSE等。然而，在XGBoost里面，它是通过对损失函数进行二阶泰勒展开：

![](./img/9bf15217ebe9acc16ecb00c9b10b4a01.svg)

对损失函数二阶泰勒展开(前![](./img/bf20193137085b07680e64a4ed4a7666.svg)棵树即![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)，第![](./img/a5f3c6a11b03839d46af9fb43c97c188.svg)棵树即![](./img/b56546a86ab832a9b2a5b15f96519319.svg))：

![](./img/0bb5b570b13d08d568a278375b513642.svg)

注意的是，这里的![](./img/8d62e469fb30ed435a668eb5c035b1f6.svg)是标签值，是个常数，而![](./img/28116e202e27d5b53513f2eef82ae9ff.svg)是第![](./img/bf20193137085b07680e64a4ed4a7666.svg)次学习的结果，也是个常数。所以只要把变化量![](./img/b56546a86ab832a9b2a5b15f96519319.svg)看成我们需要学习的模型![](./img/643cf74e18c3ec4fbd9b9fa67de44edd.svg)，把![](./img/28116e202e27d5b53513f2eef82ae9ff.svg)看作![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)就可以展成上面的样子。

这里，我们用![](./img/3b008c77d31e3eaedb59c6264f80b88f.svg)记为第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个样本损失函数的一阶导，![](./img/57339b77b3af15427b7154f4daf8a223.svg)记为第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个样本损失函数的二阶导：

![](./img/bf6d8cd8855036ba0d3027e8148420f5.svg)

上面两式非常重要，贯串整个树的创建（分裂，叶子节点值的计算）。同时二阶导式子是我们利用XGBoost做特征选择时的一个评价指标。所以我们可以得到我们进化后的目标函数：

![](./img/266372cfb13c4a5fbfdcdcc8854538e7.svg)

我们先回忆一下，一颗树用数学模型来描述是什么样，很简单其实就是一个分段函数。比如有下面一颗树。

![XGboost1.png](./img/1592810718356-fdf2a430-9e0f-4d16-87a9-7ef0f2ec8ba1.png)

![](./img/ff3223f718b57ba3e40af62883456862.svg)

也就是说，一棵树其实可以由一片区域以及若干个叶子节点来表达。而同时，构建一颗树也是为了找到每个节点的区域以及叶子节点的值。

也就说可以有如下映射的关系![](./img/f0c80a81ee0af031338df92e27d227ee.svg)。其中![](./img/9dbcf8f4523b910764da7f544a192a69.svg)叶子节点的编号（从左往右遍）。![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)是叶子节点的取值。即对于任意一个样本![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)，其最后会落在树的某个叶子节点上，其值为![](./img/e6c7ba3ff96d6c2dc96e2a0d0b308468.svg)。

既然一棵树可以用叶子节点来表达，我们可以对叶子节点进行惩罚（正则），比如取![](./img/11cc37d6dae6b905a0a0eb2ff087ae24.svg)正则，以及我们控制一下叶子节点的个数![](./img/b9ece18c950afbfa6b0fdbfa4ff731d3.svg)，那么正则项有：

![](./img/acc3d3d8af73f555574f6389f2cd867b.svg)

正则为什么可以控制模型复杂度呢？有很多角度可以看这个问题，最直观就是，我们为了使得目标函数最小，自然正则项也要小，正则项要小，叶子节点个数![](./img/b9ece18c950afbfa6b0fdbfa4ff731d3.svg)要小（叶子节点个数少，树就简单）。

而为什么要对叶子节点的值进行![](./img/11cc37d6dae6b905a0a0eb2ff087ae24.svg)正则，这个可以参考一下逻辑回归里面进行正则的原因，简单的说就是逻辑回归没有加正则，整个![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的参数空间是无限大的，只有加了正则之后，才会把![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的解规范在一个范围内。（对此困惑的话可以跑一个不带正则的逻辑回归，每次出来的权重![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)都不一样，但是Loss都是一样的，加了![](./img/11cc37d6dae6b905a0a0eb2ff087ae24.svg)正则后，每次得到的![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)都是一样的）。

我们的目标函数（移除常数项后，注意：![](./img/46a5a9d096f787d0a2b5f52cdbc39402.svg)也是常数）就可以改写成这样（用叶子节点表达）：

![](./img/10c284d56c94f78403a0965abff4955b.svg)

其实我们可以进一步化简，也是最重要的一步化简（上式展开之后按照叶子节点编号进行合并后可以得到下式）。那么最后可以化简成：

![](./img/a86080d9ad6cf17e6b81b9ce3fe2cbbc.svg)

下面，我们把![](./img/b1557f0ae52e468912a4c347a95277d8.svg)记为![](./img/e8153e65e22df0b3d23d1614471fd606.svg)，把![](./img/cf2ef7f5119b5ec974d2871a604d4213.svg)记为![](./img/fd416a281c753110630157443962219b.svg)，那么目标函数可以进一步简化为：

![](./img/1b17cd38a95d7bb22f5f411f646f3a61.svg)

我们做了这么多，其实一直都是在对二阶泰勒展开后的式子化简，其实刚展开的时候就已经是一个二次函数了，只不过化简到这里能够把问题看的更加清楚。所以对于这个目标函数，我们对![](./img/e8100be07fa5419af6c6738b934dfca0.svg)求导，然后代入极值点，可以得到一个极值：

![](./img/287eeb0abe2c88eabb8fe08a1504ed13.svg)<br />![](./img/485b51e5aaab99ba94ab86850a37a468.svg)

上面第一个式子即为叶子节点取值的表达式，第二个式子即为目标函数的值。

值得注意的是：在GBDT中，不同的损失函数有不同的叶子节点的取值，而在Xgboost里，叶子节点取值的表达式很简洁，推导起来也比GBDT的要简便许多。

<a name="4faf7e51"></a>
### 分裂准则

我们一直都是在围绕目标函数进行分析，这个到底是为什么呢？为了我们寻找![](./img/2fd2d8b9444762a6e12096c20301f6ab.svg)，也就是建树的过程。

具体来说，我们回忆一下建树的时候需要做什么，建树的时候最关键的一步就是选择一个分裂的准则，也就如何评价分裂的质量。比如在前面文章GBDT的介绍里，我们可以选择MSE，MAE来评价我们的分裂的质量，但是，我们所选择的分裂准则似乎不总是和我们的损失函数有关，因为这种选择是启发式的。比如，在分类任务里面，损失函数可以选择logloss，分裂准确选择MSE，这样看来，似乎分裂的好坏和我们的损失并没有直接挂钩。

但是，在Xgboost里面，我们的分裂准则是直接与损失函数挂钩的准则，这个也是Xgboost和GBDT一个很不一样的地方。具体来说，XGBoost选择![](./img/99d14ca0c09acd0973ed963bd60c4872.svg)：

![](./img/e8f48f4660b760aa19e585e8afdb3fc4.svg)

其实选择这个作为准则的原因很简单也很直观。我们这样考虑，由![](./img/485b51e5aaab99ba94ab86850a37a468.svg)知道，对于一个节点，假设我们不分裂的话，此时的损失为![](./img/3a9f93ffb195b57b43dd9b111d08b6e2.svg)。假设在这个节点分裂的话，分裂之后左右叶子节点的损失分别为：![](./img/c7a843ce92e6e87e18f9864309265f8e.svg)、![](./img/de6c35a86fc17a44a86f5f9cb645907a.svg)。

既然要分裂的时候，我们当然是选择分裂成左右子节点后，损失减少的最多。也就是找到一种分裂有：

![](./img/fb28330a944c4eba0141d3eb79b777aa.svg)

那么![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)的作用是什么呢？利用![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)可以控制树的复杂度，进一步来说，利用![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)来作为阈值，只有大于![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)时候才选择分裂。这个其实起到了预剪枝的作用。最后就是如何得到左右子节点的样本集合？这个和普通的树一样，都是通过遍历特征所有取值，逐个尝试。

至此，我们把Xgboost的基本原理阐述了一遍。我们总结一下，其实就是做了以下几件事情：

- 在损失函数的基础上加入了正则项
- 对目标函数进行二阶泰勒展开
- 利用推导得到的表达式作为分裂标准，来构建一棵树

<a name="4b354c7e"></a>
### 算法流程

[XGBoost核心部分的算法流程图如下：](https://arxiv.org/pdf/1603.02754.pdf)

![XGBoost2.png](./img/1592811649754-04e44ec6-bbea-423a-b536-c8e07f0ebcda.png)

<a name="718532cd"></a>
## 手动计算还原XGBoost过程

上面阐述了整个流程，有一些细节的地方可能都说的不太清楚。在这里，我以一个简单的UCI数据集，一步一步地和大家演算整个XGBoost的过程。

<a name="0b5a12c5"></a>
### 数据集、参数设置以及损失函数

数据集的样本条数只有15条，2个特征。具体如下：

![XGBoost3.png](./img/1592811714510-63cdc18e-5f67-4472-8bb2-b35f0f817277.png)

这里为了简单起见，树的深度设置为3(max_depth=3)，树的颗数设置为2(num_boost_round=2),学习率为0.1(eta=0.1)。另外再设置两个正则的参数，![](./img/0f95d2f2265095c8032bb38dd0790e74.svg)，![](./img/74153503dc81c057a8f2bc8111bba1bd.svg)。损失选择logloss。 由于后面需要用到logloss的一阶导数以及二阶导数，这里先简单推导一下：

![](./img/a26dcb1bc5989d5bc53265c26731c09b.svg)

两边对![](./img/c9e53cbdffe795c0913cd927f13ffb9b.svg)求一阶导数：

![](./img/2f4f159ee5ecae9294e62082df371eb0.svg)

![](./img/47e3a10e3e1d299b79a350c78abe6e70.svg)

![](./img/1f4e5eaa526629dea72c217b1d1f934a.svg)

![](./img/b19a9ca4c7274d7678b0839505e403fa.svg)

其中，![](./img/d09b1044dab41e6ee2cc57f7ffd8c8fa.svg)

在一阶导的基础上再求一次导（其实就是sigmoid函数求导）：

![](./img/6e82e7fd9f9371bfb0959f3461e08351.svg)

所以样本的一阶导数值

![](./img/f170107aa298a361e45b68e129cb89e2.svg)

样本的二阶导数值

![](./img/e45f3f6d75ce8f2ee2a2ae26328cd838.svg)

<a name="a9b66893"></a>
### 建立第一棵树（k=1）

建树的时候从根节点开始(Top-down greedy)，在根节点上的样本有ID1～ID15。那么回顾Xgboost的算法流程，我们需要在根节点处进行分裂，分裂的时候需要计算式子：

![](./img/e8f48f4660b760aa19e585e8afdb3fc4.svg)

式子表达的是：在节点处把样本分成左子节点和右子节点两集合。分别求两集合的![](./img/9723139430cafbabcff8120286d7074c.svg)，然后计算增益![](./img/5e34e943e39d1e419af1e193115efbca.svg)。

而这里，其实可以先计算每个样本的一阶导数值和二阶导数值，但是这里你可能碰到了一个问题，那就是第一颗树的时候每个样本的预测的概率是多少？这里和GBDT一样，应该说和所有的Boosting算法一样，都需要一个初始值。而在XGBoost里面，对于分类任务只需要初始化为![](./img/b6dbc33006b907f2db1855810abfce98.svg)中的任意一个数都可以。具体来说就是参数base_score。（其默认值是![](./img/d310cb367d993fb6fb584b198a2fd72c.svg)。值得注意的是base_score是一个经过sigmod映射的值，可以理解为一个概率值，提这个原因是后面建第二颗树会用到，需要注意这个细节）

这里我们也设base_score=![](./img/d310cb367d993fb6fb584b198a2fd72c.svg) 。然后我们可以计算每个样本的一阶导数值和二阶导数值了。具体如下表

![XGBoost4.png](./img/1592812231540-4585d35a-1ef2-46ab-8ba5-8cb52a2c09a6.png)

对于ID=1样本，![](./img/81071da0ddc16ced8c23c6ef8da3fca1.svg)，![](./img/af9d5b1dabf3e7376981d84ac0178906.svg)。那么把样本如何分成两个集合呢？这里就是上面说到选取一个最佳的特征以及分裂点使得![](./img/5e34e943e39d1e419af1e193115efbca.svg)最大。

比如说对于特征![](./img/aa687da0086c1ea060a8838e24611319.svg)，一共有![](./img/38ec8534b90184364e7c24ebacef8ba3.svg)这![](./img/c9f0f895fb98ab9159f51fd0297e236d.svg)种取值。可以得到![](./img/8f14e45fceea167a5a36dedd4bea2543.svg)种划分+不划分(分裂)方式。

以![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)为划分时（![](./img/e5934b63b9c94532038d47ec0e476d57.svg)）：

左子节点包含的样本：![](./img/2aa2ca2ee6d75b3624e95726d781cca3.svg)

右子节点包含的样本：![](./img/85a6e442fab14c64d1e1e266e886acae.svg)

那么左子节点为空，![](./img/79b6b42c0ea6a7ff5d8de536d49ae983.svg)和![](./img/29a2477f8c4c44d43854f818f7060e1b.svg)

右子节点的一阶导数和：![](./img/3ff61efee7a329f6976d52564241c090.svg)

右子节点的二阶导数和：![](./img/2d012a8fce0f85400b710cbcdcc5c82e.svg)

最后我们在计算以下增益 ![](./img/5e34e943e39d1e419af1e193115efbca.svg)，可以得到![](./img/05dc82e526a12ec5b2d50c2d76347516.svg)。计算出来发现![](./img/05dc82e526a12ec5b2d50c2d76347516.svg)其实不用惊讶， 因为左子结点为空，也就是这次分裂把全部样本都归到右子结点，这个和没分裂有啥区别？所以，分裂的时候每个结点至少有一个样本。

下面，再计算当以![](./img/c81e728d9d4c2f636f067f89cc14862c.svg)划分时的增益![](./img/5e34e943e39d1e419af1e193115efbca.svg)（以![](./img/c81e728d9d4c2f636f067f89cc14862c.svg)为划分时![](./img/a287d597cd85f6606f03b557a3b1b1a2.svg)）：

左子节点包含的样本：![](./img/0d6125469b25cebe0b73526744aac4e8.svg)

右子节点包含的样本：![](./img/c8e391adca37e105bd057586c55ce21a.svg)

左子节点的一阶导数和：![](./img/3037c446431fc9208e22167a19757fa8.svg)

左子节点的二阶导数和：![](./img/718f52b7ec93f2910403d28234d5c1e3.svg)

右子节点的一阶导数和：![](./img/878a814eb02476614a87e136ea9314ac.svg)

右子节点的二阶导数和：![](./img/e018ae4d6688553ff34b84d6fedd7f46.svg)

最后计算一下增益![](./img/5e34e943e39d1e419af1e193115efbca.svg)：![](./img/0a376c3ef3f1325a4f6509f6911c825c.svg)

其他的值![](./img/678763276b624298d3739c369c00d820.svg)类似，计算归总到下表：

![XGBoost5.png](./img/1592812828933-4ea23e08-6e46-46ce-a296-97c38af56909.png)

从上表我们可以看到，如果特征![](./img/aa687da0086c1ea060a8838e24611319.svg)以![](./img/de1253485f263daa1eb22fb297dab4b5.svg)分裂时可以得到最大的增益![](./img/ed30daf74bda4efbb940ea121dc4c76c.svg)。 按照算法的流程，这个时候需要遍历下一个特征![](./img/8732099f74d777a67257cb2f04ead3d8.svg)，对于特征![](./img/8732099f74d777a67257cb2f04ead3d8.svg)也是重复上面这些步骤，可以得到类似的表如下：

![XGBoost6.png](./img/1592812844120-4c9e7901-c6e7-445f-b15c-02184fb10a5d.png)

可以看到，以![](./img/8732099f74d777a67257cb2f04ead3d8.svg)特征来分裂时，最大的增益是![](./img/573018bc03b006a550eb10d4398a405b.svg)。所以在根节点处，以![](./img/de1253485f263daa1eb22fb297dab4b5.svg)来进行分裂。

由于设置的最大深度是![](./img/eccbc87e4b5ce2fe28308fd9f2a7baf3.svg)，此时只有![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)层，所以还需要继续往下分裂。

左子节点的样本集合：![](./img/5ed487c0531cef0f31c1985346928457.svg)

右子节点的样本集合：![](./img/1507a3395c9f37e75ecf8fd4f4b5faec.svg)

右子节点此时只剩一个样本，不需要分裂了，也就是已经是叶子结点。可以计算其对应的叶子结点值了，按照公式![](./img/287eeb0abe2c88eabb8fe08a1504ed13.svg)：

![](./img/7418daabe8b167564ea09dd6192f0cc6.svg)

下面就是对左子节点![](./img/112481210a0d644ea16d9650ae4b1498.svg)进行分裂。分裂的时候把此时的节点看成根节点，其实就是循环上面的过程，同时，同样也是需要遍历所有特征![](./img/aa687da0086c1ea060a8838e24611319.svg)，![](./img/8732099f74d777a67257cb2f04ead3d8.svg)的所有取值作为分裂点，选取增益最大的点。重复上面的过程，得到![](./img/aa687da0086c1ea060a8838e24611319.svg)和![](./img/8732099f74d777a67257cb2f04ead3d8.svg)特征分裂的增益分别为下两表：

![XGBoost7.png](./img/1592813384276-6550c9cd-39e7-4505-aa59-25c2e6862b48.png)<br />![XGBoost8.png](./img/1592813398764-f9479b97-c3f6-41ea-9701-547adb825c4b.png)

可以看到，![](./img/6fe0c9d1820b3c9db9a43608ea59c028.svg)时分裂可以获得最大的增益。就这样重复下去，直到到达规定深度。这里就不啰嗦了，第一个树结构如下图：

![XGBoost9.png](./img/1592813433543-81cf3c90-0c3a-4c01-b45c-11d007fc5996.png)

这里有的人可能对叶子结点取值感到困惑。为何我算出来的是![](./img/d1ab6103a7baf2fcf7a1611ac2299261.svg)，可图上却是![](./img/4eb9ea46659fbc5b46b9f902cdd383d5.svg)？

这里以最左边的一个叶子结点为例，展示一下计算的过程。落在最左边这个叶子节点的样本有![](./img/8e3d6e41bcaa715605fb6ff8d614fe34.svg)所以由公式![](./img/287eeb0abe2c88eabb8fe08a1504ed13.svg)：

![](./img/8cc97b3ed7d662557cb56b48c5e3c782.svg)

落在从左边数起第二个叶子结点上的样本有![](./img/e6e3f7bcf0c7cf213e013b6c45dfcf2a.svg)：

![](./img/2a5fc1d4b2daadd4062b7b902988dbb2.svg)

到这里完全没有问题，但是为什么和图上的不一样呢？这里其实和我们在GBDT中的处理一样，我们会以一个学习率来乘这个值，当完全取![](./img/d1ab6103a7baf2fcf7a1611ac2299261.svg)时说明学习率取![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，这个时候很容易过拟合。所以每次得到叶子结点的值后需要乘上学习率![](./img/ffe9f913124f345732e9f00fa258552e.svg)，在前面我们已经设置了学习率是![](./img/cb5ae17636e975f9bf71ddf5bc542075.svg)。这里也是GBDT和XGBoost一个共同点，大家都是通过学习率来进行Shrinkage，以减少过拟合的风险。

至此，我们学习完了第一颗树。

<a name="4662a2b6"></a>
### 建立第二颗树（k=2）

这里，我们开始拟合我们第二颗树。其实过程和第一颗树完全一样。只不过对![](./img/ec0960436669f19adb6164a1111b9e37.svg)需要进行更新，也就是拟合第二颗树是在第一颗树预测的结果基础上。这和GBDT一样，因为大家都是Boosting思想的算法。

由于在第一棵树构造时之前没有树，所以初始![](./img/049ed180b13e9ba33e18c6fd0b727645.svg)，即参数base_score为![](./img/d310cb367d993fb6fb584b198a2fd72c.svg)。

假设此时，模型只有这一颗树(![](./img/2f9cea51b81d35c6f0d3927791601985.svg))，那么模型对样例![](./img/1ba8aaab47179b3d3e24b0ccea9f4e30.svg)进行预测时，预测的结果表达是什么呢？

由加法模型：<br />![](./img/256c84cc1c3a5bae08f4d613372af198.svg)

![](./img/b7f11b387e2c7a4f5c3c0c74f5eb65f8.svg)

![](./img/8a54b6a7bf67da04473489d2cd59865a.svg)的值是样例![](./img/1ba8aaab47179b3d3e24b0ccea9f4e30.svg)落在第一棵树上的叶子节点值。那么![](./img/28bb7eb03c53b2a10ab1ef0a46b3f702.svg)是什么呢？ 这里就涉及前面提到一个问题base_score是一个经过sigmod映射后的值（因为选择使用log loss做损失函数，概率![](./img/1678f4c7518880403090e8509a34017c.svg)）

所以需要将![](./img/d310cb367d993fb6fb584b198a2fd72c.svg)逆运算![](./img/fbf9ce57c23ea5a08de27c8b04150a17.svg)后可以得到![](./img/6c83b497b965e4814eb47ad9a59ef1b6.svg)。

所以第一棵树预测的结果就是![](./img/312451587882febac919f85386c100b9.svg)（其实当训练次数K足够多的时候，初始化这个值几乎不起作用的，这个在官网文档上有说明）

所以，我们可以得到第一棵树预测的结果为下表（预测后将其映射成概率）

![XGBoost10.png](./img/1592813723069-42aefddc-9053-4981-9c8b-1736caeafb7f.png)

比如对于ID=1的样本，其落在![](./img/4eb9ea46659fbc5b46b9f902cdd383d5.svg)这个节点。经过sigmod映射后的值为![](./img/9014dbc8f0cc7b24be278a9e78c05138.svg)。有了这个之后，根据公式![](./img/f170107aa298a361e45b68e129cb89e2.svg)和![](./img/e45f3f6d75ce8f2ee2a2ae26328cd838.svg)我们就可以计算所有样本新的一阶导数和二阶导数的值了。具体如下：

![XGBoost11.png](./img/1592813976892-d015fd50-f8b8-4217-8502-b7a6ce9389c3.png)

之后，我们和第一棵树建立的时候一样以![](./img/e8f48f4660b760aa19e585e8afdb3fc4.svg)公式去建树。

拟合完后第二棵树如下图：

![XGBoost12.png](./img/1592814026122-7c60dafe-2415-4e4b-ab7b-ebbe185091c6.png)

后面的所有过程都是重复这个过程，这里就不再啰嗦了。

<a name="9b757d96"></a>
## XGBoost近似分位数算法

当数据量非常大难以被全部加载进内存时或者分布式环境下时，exact greedy算法将不再合适。因此作者提出近似算法（Approximate Algorithm）来寻找分割点。 [近似算法的大致流程见下面的算法](https://arxiv.org/pdf/1603.02754.pdf)<br />![XGBoost13.png](./img/1592814049805-c2f6a0d4-74f6-43d8-86f0-3eaa102cbf39.png)<br />对于某个特征![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)，算法首先根据特征分布的分位数找到特征切割点的候选集合![](./img/5c595da6fbc30d780f83c0ee40436a91.svg)；然后将特征![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)的值根据集合![](./img/85c5cb7c4e2ae43f3450a97e3e6a9a39.svg)划分到桶(bucket)中，接着对每个桶内的样本统计值![](./img/dfcf28d0734569a6a693bc8194de62bf.svg)、![](./img/c1d9f50f86825a1a2302ec2449c17196.svg)进行累加统计，最后在这些累计的统计量上寻找最佳分裂点。

不同于基本的穷举算法，paper指出两种近似算法：一种是全局算法，即在初始化tree的时候划分好候选分割点，并且在树的每一层都使用这些候选分割点；另一种是局部算法，即每一次划分的时候都重新计算候选分割点。这两者各有利弊，全局算法不需要多次计算候选节点，但需要一次获取较多的候选节点供后续树生长使用，而局部算法一次获取的候选节点较少，可以在分支过程中不断改善，即适用于生长更深的树，两者在effect和accuracy做trade off。

![XGBoost14.png](./img/1592814256377-2f10df16-b7f7-4ed2-a395-37304c9fda1b.png)

从算法伪代码可以看出近似算法的核心是如何根据分位数采样得到分割点的候选集合![](./img/5dbc98dcc983a70728bd082d1a47546e.svg)。XGBoost提出了Weighted Quantile Sketch来解决这个问题。

在讲述Weighted Quantile Sketch之前，必须先要介绍一下什么是Quantile。这是Weighted Quantile Sketch的关键，如果不理解Quantile，就不会理解，在当数据量非常大难以被全部加载进内存时或者分布式环境下时，XGBoost的近似算法是如何寻找分割点的候选集合![](./img/5dbc98dcc983a70728bd082d1a47546e.svg)的。

<a name="Quantile"></a>
### Quantile

<a name="vTdCg"></a>
#### ![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantile

输入数据： 14, 19, 3, 15, 4, 6, 1, 13, 13, 7, 11, 8, 4, 5, 15, 2

则排序后，该组数据为： 1, 2, 3, 4, 4, 5, 6, 7, 8, 11, 13, 13, 14, 15, 15, 19. 如下图所示：

![XGBoost15.png](./img/1592814333356-43c50263-853d-40ed-9a20-4072eec0350a.png)

在上面的序列中，

第1小的数是什么？ 很明显是：1 (rank=1)。 第4小的数是什么？ 答案是：4 (rank=4)。

第50%小的数是什么？ 50% * 16 = 8（rank=8)， 则答案为：7

什么是分位点呢？![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantile表示![](./img/fd28514d6c715a865c6bbeeb6826b9e8.svg)的元素，其中，![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)为序列中元素的个数。例如，在上面的例子中： 0.25-quantile是什么？ rank=0.25×16=4，所以答案为：4

<a name="e79d27b8"></a>
#### ![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate ![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantiles

![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate ![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantiles 的意思就是：在![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantiles误差![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate以内位置的取值。即近似分位点。

即 ![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantiles 是在区间![](./img/f213e89d19a5ca3f6cdb1650157d5a2e.svg)，而不是之前的精确的![](./img/6492c642d42b0ca96ace60dded5c52a2.svg)。还是上面的例子，令![](./img/3cdb52cebcb93f668b28112b927cde85.svg)，![](./img/3451b32543f53af70c6e6e81e1a62959.svg)，由数据可知![](./img/e46762870a73af188a52f301e68b738f.svg)，此时![](./img/f213e89d19a5ca3f6cdb1650157d5a2e.svg)为![](./img/a6ed548496f65309d7b3fe75bfb4d7d5.svg)，即rank为![](./img/6340240b70871521d64a0a591beb88e9.svg)，![](./img/cb5ae17636e975f9bf71ddf5bc542075.svg)-approximate ![](./img/d310cb367d993fb6fb584b198a2fd72c.svg)-quantiles 为![](./img/5be099e9ee787f2d4add9900ed8666a9.svg)。

物理含义是什么呢？就是说，如果我们允许![](./img/aa30ca75c94db5794ea98cb55116647b.svg)是![](./img/4341447b6ad78bf4d7eb59ebb5a53d63.svg)的误差的话，那么![](./img/d310cb367d993fb6fb584b198a2fd72c.svg)-quantiles 的值为![](./img/7c8c44d5f9bcc4398733c26180ea7bce.svg)都可以。

<a name="580ea3af"></a>
#### ![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg) -approximate quantile summary

我们已经可以看到，即便是求一个序列的![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate ![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantiles，也必须先对数据进行排序，而如果我们的内存不足以让全部数据排序时，应该怎么解决？早在2001年，M.Greenwald和S. Khanna提出了GK Summay分位点近似算法（![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate ![](./img/1ed346930917426bc46d41e22cc525ec.svg)-quantiles），直到到2007年被Q. Zhang和W. Wang提出的多层level的merge与compress/prune框架进行高度优化，而被称为A fast algorithm for approximate quantiles，目前XGBoost框架套用A fast algorithm算法结构。

GK Summay巧妙地设计了![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate quantile summary。![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate quantile summary 是一种数据结构，该数据结构能够以![](./img/aa30ca75c94db5794ea98cb55116647b.svg)的精度计算任意的分位查询。当一个序列无法全部加载到内存时，常常采用quantile suammary近似的计算分位点。

大致来讲下思路：![](./img/92e4da341fe8f4cd46192f21b6ff3aa7.svg)-approximate quantile summary 这个数据结构不需要一次存入所有的数据，它先用一些元组存入部分数据（当然在内部需要排序），这些元组记录的是现有的value值和一些位置信息，有了这些信息，就保证了能够以![](./img/aa30ca75c94db5794ea98cb55116647b.svg)的精度计算任意的分位查询。只要流式系统中每个时刻都维持这种summary结构，每次查询都能满足精度要求，但是流式数据实时更新，需要解决新增数据的summary更新问题。为此，算法提供了insert操作，insert操作可以保证现有的summary结构仍然可以保证![](./img/aa30ca75c94db5794ea98cb55116647b.svg)的精度。当然，每次数据插入都需要新增元组，summary结构不能持续增加而不删除，因此到达一定程度需要对summary进行delete。同时，delete操作也可以保证现有的summary结构仍然可以保证![](./img/aa30ca75c94db5794ea98cb55116647b.svg)的精度

<a name="ef7f0d1a"></a>
### Weighted Datasets

现在我们回到XGBoost中，在建立第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)棵树的时候已经知道数据集在前面![](./img/800ba394e806c049c98cf4b570d04c72.svg)棵树的误差，因此采样的时候是需要考虑误差，对于误差大的特征值采样粒度要加大，误差小的特征值采样粒度可以减小，也就是说采样的样本是需要权重的。重新审视目标函数

![](./img/df067a6a7050c81fbcad5eff6bf4c9ae.svg)

![](./img/0ceae771e1e15ebe24d69b420c139371.svg)

因此可以将该目标还是看作是关于标签的![](./img/a774f3b50ab1f5d8d2f51c1495fc6019.svg)和权重为![](./img/57339b77b3af15427b7154f4daf8a223.svg)的平方误差形式。![](./img/57339b77b3af15427b7154f4daf8a223.svg)为样本的二阶导数

<a name="d6867fd5"></a>
#### 二阶导数为权重的解释

如果损失函数是Square loss，即![](./img/4996ec57a95046305380f94fba3a294d.svg)，则![](./img/cc1dd08ac81b052b146e9df3b0d35c92.svg)，那么实际上式不带权（每个样本的权重一样）。如果损失函数是Log loss，则![](./img/ceaa77fef62b6bdee1e81064763ff7dd.svg)。 这是个开口朝下的一元二次函数，所以最大值在![](./img/9b9aac6a58881b85e187da1ab22afbb1.svg)。当pred在![](./img/d310cb367d993fb6fb584b198a2fd72c.svg)附近，值都比较大，也就是权重都比较大，在切直方图时，我们希望桶比较均匀，因此这部分就会被切分的更细。

![XGBoost16.png](./img/1592815148924-a4b6cf58-31a3-42a8-9ddd-45ab794b9f51.png)

<a name="1362d22e"></a>
#### 问题转换

![](./img/58dde6dea17f665e6119e6febd25212a.svg)表示每个训练样本的第![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)维特征值和对应二阶导数。接下来定义排序函数为![](./img/f055efc58764a3d9d23419c94a7fad89.svg)

![](./img/ad63b2af6e4b216336052187b3e7f24e.svg)

函数表示特征的值小于![](./img/fbade9e36a3f36d3d676c1b808451dd7.svg)的样本分布占比，其中二阶导数![](./img/2510c39011c5be704182423e3a695e91.svg)可以视为权重。

在这个排序函数下，我们找到一组点![](./img/f265209eb1e540c351693c9082cefdf5.svg)，满足

![](./img/9a8676ca3ac9903731b7dd15abc345ec.svg)

其中，![](./img/7ce76d13326ac65ff03034142c6f2380.svg)，![](./img/55be392a51d191d5a69fe237fcb43854.svg)，![](./img/f8b1c5a729a09649c275fca88976d8dd.svg)为采样率，直观上理解，我们最后会得到![](./img/2dd9375ac2d1a84151a550a2a3240be2.svg)个分界点。注意，这里的![](./img/f8b1c5a729a09649c275fca88976d8dd.svg)就是每个桶的比例大小。白话说就是让相邻两个候选分裂点相差不超过某个值![](./img/f8b1c5a729a09649c275fca88976d8dd.svg)因此，总会得到![](./img/2dd9375ac2d1a84151a550a2a3240be2.svg)个切分点。

![XGBoost17.png](./img/1592815417369-98547572-78dd-4a70-b0f3-1660faeb3a51.png)

要切分为3个，总和为1.8，因此第1个在0.6处，第2个在1.2处。

在XGBoost[官方文档](https://xgboost.readthedocs.io/en/latest/parameter.html)中设置tree_method=approx，然后设置sketch_eps，就可以确定有多少个候选分割点

<a name="29ce77d3"></a>
### Weighted Quantile Sketch

好了，如果我们的数据集比较小（特征的不同value不多），采用近似算法的话，我们只需要排序特征的所有特征值，然后根据公式![](./img/9a8676ca3ac9903731b7dd15abc345ec.svg)就可以求出候选的分割点。但是，如果这里的关键是：如果我们的数据量特别大（某些特征的不同value特别特别多），以致于无法排序找到分位点，我们应该怎么做？

对于这种weighted datasets的情况，陈天奇博士提出了Weighted Quantile Sketch算法。 我们可以求得一个大数据集每个特征的近似分位点，也就是求得了算法2中最核心的![](./img/993b304dd241ccd45a7beb568102d0dd.svg)。且这个近似分为点的精度是在![](./img/6b6a60f6cb6eb6736e6ea25b1519ab00.svg)之内的。![](./img/592cd4d6e399a729983a5a65dd7d532f.svg)就是第二部分的![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)的意思。

<a name="3512c1ba"></a>
## XGBoost一些细节

<a name="97d24153"></a>
### XGBoost如何画出树

对XGBoost使用了一定程度的读者，肯定会面临如何画出树这个问题，毕竟不画出树就是一个黑箱，黑箱总是难以让人放心。这篇[博客](https://blog.csdn.net/anshuai_aw1/article/details/82988494)完整地给出了如何画出XGBoost中的树的过程。

<a name="2b386dc8"></a>
### XGBoost与传统GBDT区别和联系

<a name="4fb95da6"></a>
#### 区别

XGBoost和GBDT的一个区别在于目标函数上。 在Xgboost中，损失函数+正则项。 GBDT中，只有损失函数

XGBoost中利用二阶导数的信息，而GBDT只利用了一阶导数。

XGBoost在建树的时候利用的准则来源于目标函数推导，而GBDT建树利用的是启发式准则。

XGBoost中可以自动处理空缺值，自动学习空缺值的分裂方向，GBDT(sklearn版本）不允许包含空缺值。

列抽样（column subsampling）：XGBoost借鉴了随机森林的做法，支持列抽样，不仅能降低过拟合，还能减少计算，这也是XGBoost异于传统GBDT的一个特性。

XGBoost工具支持并行：Boosting不是一种串行的结构吗?怎么并行的？注意XGBoost的并行不是tree粒度的并行，XGBoost也是一次迭代完才能进行下一次迭代的（第t次迭代的损失函数里包含了前面t−1次迭代的预测值）。XGBoost的并行是在特征粒度上的。我们知道，决策树的学习最耗时的一个步骤就是对特征的值进行排序（因为要确定最佳分割点），XGBoost在训练之前，预先对数据进行了排序，然后保存为block(块)结构，后面的迭代中重复地使用这个结构，大大减小计算量。这个block结构也使得并行成为了可能，在进行节点的分裂时，需要计算每个特征的增益，最终选增益最大的那个特征去做分裂，那么各个特征的增益计算就可以开多线程进行。

线程缓冲区存储：按照特征列方式存储能优化寻找最佳的分割点，但是当以行计算梯度数据时会导致内存的不连续访问，严重时会导致cache miss，降低算法效率。paper中提到，可先将数据收集到线程内部的buffer（缓冲区），主要是结合多线程、数据压缩、分片的方法，然后再计算，提高算法的效率。

可并行的近似直方图算法：树节点在进行分裂时，我们需要计算每个特征的每个分割点对应的增益，即用贪心法枚举所有可能的分割点。当数据无法一次载入内存或者在分布式情况下，贪心算法效率就会变得很低，所以XGBoost还提出了一种可并行的近似直方图算法，用于高效地生成候选的分割点。大致的思想是根据百分位法列举几个可能成为分割点的候选者，然后从候选者中根据上面求分割点的公式计算找出最佳的分割点。

<a name="d38a089e"></a>
#### 联系

XGBoost和GBDT的学习过程都是一样的，都是基于Boosting的思想，先学习前![](./img/47425da90092a0727c15c009fac9f866.svg)个学习器，然后基于前![](./img/47425da90092a0727c15c009fac9f866.svg)个学习器学习第![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个学习器。(Boosting)

建树过程都利用了损失函数的导数信息(Gradient)，只是大家利用的方式不一样而已。

都使用了学习率来进行Shrinkage，从前面我们能看到不管是GBDT还是XGBoost，我们都会利用学习率对拟合结果做缩减以减少过拟合的风险。

<a name="fc700a54"></a>
### [XGBoost构造新特征+LR融合](https://blog.csdn.net/anshuai_aw1/article/details/82983997)

为什么要使用LR模型进行融合呢？这是因为LR (逻辑回归) 算法简单有效，成为工业界最常用的算法之一。但 LR 算法是线性模型，不能捕捉到非线性信息，需要大量特征工程找到特征组合。为了发现有效的特征组合，Facebook 在 2014年的论文ractical Lessons from Predicting Clicks on Ads at Facebook介绍了通过 GBDT （Gradient Boost Decision Tree）+ LR 的方案 （XGBoost 是 GBDT 的后续发展）。在这篇论文中他们提出了一种将Xgboost作为feature transform的方法。随后在多个Kaggle 竞赛实践中，均证明了此思路的有效性。

大概的思想可以描述为如下：先用已有特征训练XGBoost模型，然后利用XGBoost模型学习到的树来构造新特征，最后把这些新特征加入原有特征一起训练模型。构造的新特征向量是取值![](./img/2260380a88f9b8759c0ccb1230f1498e.svg)的，向量的每个元素对应于XGBoost模型中树的叶子结点。当一个样本点通过某棵树最终落在这棵树的一个叶子结点上，那么在新特征向量中这个叶子结点对应的元素值为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，而这棵树的其他叶子结点对应的元素值为![](./img/cfcd208495d565ef66e7dff9f98764da.svg)。新特征向量的长度等于XGBoost模型里所有树包含的叶子结点数之和。最后将新的特征扔到LR模型进行训练。

<a name="61bbb5ef"></a>
### 如何理解训练过程的参数min_child_weight

在选择分裂的时候，我们是选取分裂点为一个最大的增益Gain。但是其实在Xgboost里面有一个参数叫min_child_weight。 先来看看官网对这个参数的解释：

![XGBoost18.png](./img/1592815625053-2699bc04-2f50-490d-990c-3669d93b4c78.png)

可能看完大概知道这个权重指的应该是二阶导数，但是具体是怎么一回事呢。 其实是这样的：

在前面建立第一颗的根结点的时候，我们得到特征![](./img/aa687da0086c1ea060a8838e24611319.svg)每个分裂点的这个表：

![XGBoost19.png](./img/1592815640291-9589366b-785d-42e7-9bc3-2dcc65deac12.png)

我们当时选取了![](./img/de1253485f263daa1eb22fb297dab4b5.svg)分裂，但是！ 这个是有前提的，那就是参数![](./img/e9f3089a882a801d3f6863d392840ef7.svg)。如果设置![](./img/1d467eab8ba82efd725535808afee775.svg)，分裂点就不是选择![](./img/d3d9446802a44259755d38e6d163e820.svg)，而是放弃这个最大增益，考虑次大增益。如果次大增益也不满足![](./img/e9f3089a882a801d3f6863d392840ef7.svg)，则继续往下找，如果没有找到一个满足的，则不进行分裂。 （在上面中min_child_weight取的0，所以只要是最大的增益就选为分裂点）

<a name="15808874"></a>
### 如何理解训练过程的参数![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)

在前面例子里，我们把![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)设成了0，如果我们设置成其他值比如1的话，在考虑最大增益的同时，也要考虑这个最大的增益是否比![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)大，如果小于![](./img/ae539dfcc999c28e25a0f3ae65c1de79.svg)则不进行分裂（预剪枝），由增益公式得到此结论

![](./img/e8f48f4660b760aa19e585e8afdb3fc4.svg)

<a name="ad7d6cae"></a>
### XGBoost如何处理缺失值

XGBoost对缺失值的处理思想很简单，具体看下面的算法流程：

![XGBoost20.png](./img/1592816004166-113fc2ab-2d1a-42ef-8e71-f6802285c9bd.png)

首先需要注意到两个集合一个是![](./img/dd7536794b63bf90eccfd37f9b147d7f.svg)，其包含所有的样本（包括含空缺值的样本）。

![](./img/bc6d0de85e84afdaf232791d9aafa398.svg)是不包含空缺值样本的集合。在计算总的![](./img/dfcf28d0734569a6a693bc8194de62bf.svg)和![](./img/c1d9f50f86825a1a2302ec2449c17196.svg)时用的是![](./img/dd7536794b63bf90eccfd37f9b147d7f.svg)，也就是说空缺值的一阶导数和二阶导数已经包含进去了。

可以看到内层循环里面有两个![](./img/dfae6ca823dcf10885bd7fbb8ffdb0ba.svg)，这两个![](./img/dfae6ca823dcf10885bd7fbb8ffdb0ba.svg)针对的集合就是![](./img/bc6d0de85e84afdaf232791d9aafa398.svg)。

第一个![](./img/dfae6ca823dcf10885bd7fbb8ffdb0ba.svg)是把特征取值从小到大的排序，然后从小到大进行扫描，这个时候在计算![](./img/3495c271d8ef03136cf53721657e2247.svg)的时候是用总的![](./img/dfcf28d0734569a6a693bc8194de62bf.svg)减去 ![](./img/2e512a81d467a1c1463701b487dbffa5.svg)，![](./img/cef6e94d4015931401f9e974b08137ad.svg)也是同样用总的![](./img/c1d9f50f86825a1a2302ec2449c17196.svg)减去![](./img/5a3b32f6ca61292ec7566b98f969368f.svg)，这意味着把空缺样本归到了右子节点。

第二个![](./img/dfae6ca823dcf10885bd7fbb8ffdb0ba.svg)相反过来，把空缺样本归到了左子节点。

所以，只要比较这两次最大增益出现在第一个![](./img/dfae6ca823dcf10885bd7fbb8ffdb0ba.svg)中还是第二个![](./img/dfae6ca823dcf10885bd7fbb8ffdb0ba.svg)中就可以知道对于空缺值的分裂方向，这就是XGBoost如何学习空缺值的思想。

<a name="a47b1f7e"></a>
### XGBoost如何用于特征选择

相信很多做过数据挖掘比赛的人都利用Xgboost来做特征选择。

一般我们调用xgb库的get_fscore()。但其实Xgboost里面有三个指标用于对特征进行评价，而get_fscore()只是其中一个指标weight。这个指标大部分玩家都很熟悉，其代表着某个特征被选作分裂的次数。

比如在前面举的例子里，我们得到这两颗树：

![XGBoost21.png](./img/1592816278034-50f3c44b-0492-45a3-b916-f6d80b8690ec.png)  ![XGBoost22.png](./img/1592816289491-8d3d7193-33d4-48a8-9763-56d37355a9c4.png)

可以看到特征![](./img/aa687da0086c1ea060a8838e24611319.svg)被选作分裂点的次数为![](./img/1679091c5a880faf6fb5e6087eb1b2dc.svg)，![](./img/8732099f74d777a67257cb2f04ead3d8.svg)被选做分裂点的次数为![](./img/c81e728d9d4c2f636f067f89cc14862c.svg)。 get_fscore()就是返回这个指标。

而XGBoost还提供了另外两个指标，一个叫gain，一个叫cover。可以利用get_score()来选择。那么gain是指什么呢？其代表着某个特征的平均增益。比如，特征![](./img/aa687da0086c1ea060a8838e24611319.svg)被选了![](./img/1679091c5a880faf6fb5e6087eb1b2dc.svg)次作为分裂的特征，每次的增益假如为![](./img/55c6d3f81efead1cbf2439dab3026e40.svg)，那么其平均增益为![](./img/b293e5c57fcf3d60887b48feea181fba.svg)。

最后一个cover指的是什么呢？其代表着每个特征在分裂时结点处的平均二阶导数。

这个为了加深理解，还是UCI数据集，不过此时只有max_depth=2，num_boost_round=1。

![XGBoost23.png](./img/1592816437656-94f645e7-dff8-4ebd-ad3d-c08b4f76c8bf.png)

建树完之后，其结构如上。 在第一个结点分裂时，![](./img/aa687da0086c1ea060a8838e24611319.svg)的特征增益表如下：

![XGBoost24.png](./img/1592816488831-e4dd1e86-a0c8-4cde-b4b7-e5e49dfc0adb.png)

第二个结点分裂时，![](./img/8732099f74d777a67257cb2f04ead3d8.svg)的特征增益表如下：

![XGBoost25.png](./img/1592816497497-ca7d688b-d84c-4e64-b258-1489f093f27f.png)

那么特征![](./img/aa687da0086c1ea060a8838e24611319.svg)的cover是如下计算的：

![](./img/aa687da0086c1ea060a8838e24611319.svg)在第一个节点处被选择为分裂特征，在特征值为![](./img/d3d9446802a44259755d38e6d163e820.svg)时进行分裂。此时节点处的总二阶导数![](./img/0af3bee7066ab18925f9322efe6cf9a8.svg)。故![](./img/aa687da0086c1ea060a8838e24611319.svg)的cover值为![](./img/7a3ffbb62732bd87727bc88ef0653e6b.svg)。这里![](./img/aa687da0086c1ea060a8838e24611319.svg)只是被分裂的了一次，如果后续还有就是求平均。

同样地，![](./img/8732099f74d777a67257cb2f04ead3d8.svg)的cover值为![](./img/49f89c56c734100db34a5107da8d08f7.svg)。

举这个例子就是先给大家说一下何为平均二阶导数。为什么需要选择二阶导数？这个二阶导数背后有什么意义吗？我们先看看官网对cover的定义：

‘cover’ - the average coverage of the feature when it is used in trees。大概的意义就是特征覆盖了多少个样本。 这里，我们不妨假设损失函数是mse，也就是![](./img/c132ccc790d82c45672ee4e559c4e599.svg)，我们求其二阶导数，很容易得到为常数![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)。也就是每个样本对应的二阶导数都是![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，那么这个cover指标不就是意味着，在某个特征在某个结点进行分裂时所覆盖的样本个数。

<a name="b33f5828"></a>
## XGBoot系统设计

<a name="c734bed7"></a>
### 分块并行 – Column Block for Parallel Learning

在建树的过程中，最耗时是找最优的切分点，而这个过程中，最耗时的部分是将数据排序。为了减少排序的时间，XGBoost采用Block结构存储数据。它有如下几点特征：

- Block中的数据以稀疏格式CSC进行存储。CSC格式请参考该[文章](https://www.cnblogs.com/rollenholt/p/5960523.html)。
- Block中的特征进行排序（不对缺失值排序），且只需要排序依次，以后的分裂树的过程可以复用。

对于exact greedy算法来说，Xgboost将所有的数据放到了一个Block中。在Block中，可以同时对所有叶子进行分裂点的计算，因此对Block进行一次扫描将可以得到所有叶子的分割特征点候选者的统计数据。（We do the split finding of all leaves collectively, so one scan over the block will collect the statistics of the split candidates in all leaf branches.） Block 中特征还需存储指向样本的索引，这样才能根据特征的值来取梯度。如下图所示：

![XGBoost26.png](./img/1592816664891-dac2f146-26bf-498c-8880-180b0d4e2f47.png)

对于approximate算法来说，Xgboost使用了多个Block，存在多个机器上或者磁盘中。每个Block对应原来数据的子集。不同的Block可以在不同的机器上计算。该方法对Local策略尤其有效，因为Local策略每次分支都重新生成候选切分点。

Block结构还有其它好处，数据按列存储，可以同时访问所有的列，很容易实现并行的寻找分裂点算法。缺点是空间消耗大了一倍。

<a name="99636040"></a>
### 缓存优化 – Cache-aware Access

使用Block结构的一个缺点是取梯度的时候，是通过索引来获取的，而这些梯度的获取顺序是按照特征的大小顺序的。这将导致非连续的内存访问，可能使得CPU cache缓存命中率低，从而影响算法效率。如下图所示：

![XGBoost27.png](./img/1592817872078-a781c90a-d10f-4355-8954-c66f13bed422.png)

因此，对于exact greedy算法中, 使用缓存预取（cache-aware prefetching）。具体来说，对每个线程分配一个连续的buffer，读取梯度信息并存入Buffer中（这样就实现了非连续到连续的转化），然后再统计梯度信息。该方式在训练样本数大的时候特别有用，见下图：

![XGBoost28.png](./img/1592817882328-01a4523c-2820-4e9e-bbed-efdd38329a5d.png)

可以看到，对于大规模数据，效果十分明显。

在approximate 算法中，对Block的大小进行了合理的设置。定义Block的大小为Block中最多的样本数。设置合适的大小是很重要的，设置过大则容易导致命中率低，过小则容易导致并行化效率不高。经过实验，发现![](./img/3ca2a55527f9b3df8d7e4b849a6c7b68.svg)比较好。如下图：

![XGBoost29.png](./img/1592817969879-e7f3cda8-238d-40b0-aba1-9926e6cfa160.png)  ![XGBoost30.png](./img/1592817976451-8881a8ae-9d1b-4903-b044-f7e54951a55d.png)

<a name="e31e3560"></a>
### Blocks for Out-of-core Computation

当数据量太大不能全部放入主内存的时候，为了使得out-of-core计算称为可能，将数据划分为多个Block并存放在磁盘上。计算的时候，使用独立的线程预先将Block放入主内存，因此可以在计算的同时读取磁盘。但是由于磁盘IO速度太慢，通常更不上计算的速度。因此，需要提升磁盘IO的销量。XGBoost采用了2个策略：

- Block压缩（Block Compression）：将Block按列压缩（LZ4压缩算法？），读取的时候用另外的线程解压。对于行索引，只保存第一个索引值，然后只保存该数据与第一个索引值之差(offset)，一共用16个bits来保存 offset，因此，一个block一般有2的16次方个样本。
- Block拆分（Block Sharding）：将数据划分到不同磁盘上，为每个磁盘分配一个预取（pre-fetcher）线程，并将数据提取到内存缓冲区中。然后，训练线程交替地从每个缓冲区读取数据。这有助于在多个磁盘可用时增加磁盘读取的吞吐量。

<a name="2db38e91"></a>
## 算法实现

[https://github.com/dmlc/xgboost](https://github.com/dmlc/xgboost)

<a name="Source"></a>
## Source

[https://arxiv.org/abs/1603.02754](https://arxiv.org/abs/1603.02754)<br />[https://blog.csdn.net/anshuai_aw1/article/details/82970489#_604](https://blog.csdn.net/anshuai_aw1/article/details/82970489#_604)<br />[https://blog.csdn.net/anshuai_aw1/article/details/85093106](https://blog.csdn.net/anshuai_aw1/article/details/85093106)
