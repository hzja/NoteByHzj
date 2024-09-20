机器学习算法<br />主要介绍基于集成学习的决策树，其主要通过不同学习框架生产基学习器，并综合所有基学习器的预测结果来改善单个基学习器的识别率和泛化性。
<a name="kBXtR"></a>
## 1、集成学习
常见的集成学习框架有三种：Bagging，Boosting 和 Stacking。三种集成学习框架在基学习器的产生和综合结果的方式上会有些区别，先做些简单的介绍。
<a name="hqHQR"></a>
### **1.1 Bagging**
Bagging 全称叫 Bootstrap aggregating，看到 Bootstrap 立刻想到著名的开源前端框架（抖个机灵，是 Bootstrap 抽样方法） ，每个基学习器都会对训练集进行有放回抽样得到子训练集，比较著名的采样法为 0.632 自助法。每个基学习器基于不同子训练集进行训练，并综合所有基学习器的预测值得到最终的预测结果。Bagging 常用的综合方法是投票法，票数最多的类别为预测类别。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657160518431-bec2d913-e3c0-46eb-95d4-2e9ded9e8023.jpeg#clientId=uee3aef42-04d0-4&from=paste&id=uf0cbaed1&originHeight=390&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc7350cca-9f4e-4e30-aeaa-c2fba3c6ebb&title=)
<a name="YyAxJ"></a>
### **1.2 Boosting**
Boosting 训练过程为阶梯状，基模型的训练是有顺序的，每个基模型都会在前一个基模型学习的基础上进行学习，最终综合所有基模型的预测值产生最终的预测结果，用的比较多的综合方式为加权法。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657160518331-be8aaa85-8842-4142-a6fc-e8d2661f316e.jpeg#clientId=uee3aef42-04d0-4&from=paste&id=u426ddca3&originHeight=703&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3d38a3d8-7ef9-40a5-bc84-cb993e62e57&title=)
<a name="EOMGi"></a>
### **1.3 Stacking**
Stacking 是先用全部数据训练好基模型，然后每个基模型都对每个训练样本进行的预测，其预测值将作为训练样本的特征值，最终会得到新的训练样本，然后基于新的训练样本进行训练得到模型，然后得到最终预测结果。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657160518342-3fd49d96-193f-46a3-a063-ec244c245466.jpeg#clientId=uee3aef42-04d0-4&from=paste&id=u4ece5a45&originHeight=851&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue8d2306d-faae-4f62-aab8-9d75c4bae16&title=)<br />那么，为什么集成学习会好于单个学习器呢？原因可能有三：

1. 训练样本可能无法选择出最好的单个学习器，由于没法选择出最好的学习器，所以干脆结合起来一起用；
2. 假设能找到最好的学习器，但由于算法运算的限制无法找到最优解，只能找到次优解，采用集成学习可以弥补算法的不足；
3. 可能算法无法得到最优解，而集成学习能够得到近似解。比如说最优解是一条对角线，而单个决策树得到的结果只能是平行于坐标轴的，但是集成学习可以去拟合这条对角线。
<a name="GqSeo"></a>
## 2、偏差与方差
上节介绍了集成学习的基本概念，这节主要介绍下如何从偏差和方差的角度来理解集成学习。
<a name="i26cu"></a>
### 2.1 集成学习的偏差与方差
偏差（Bias）描述的是预测值和真实值之差；方差（Variance）描述的是预测值作为随机变量的离散程度。放一场很经典的图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657160519297-b4828dc1-3669-4723-af05-756997abab5a.jpeg#clientId=uee3aef42-04d0-4&from=paste&id=ub3fcaf03&originHeight=567&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u15117781-51b1-47ae-a441-a1a7dae9a19&title=)
<a name="XckIh"></a>
#### **模型的偏差与方差**
**偏差**：描述样本拟合出的模型的预测结果的期望与样本真实结果的差距，要想偏差表现的好，就需要复杂化模型，增加模型的参数，但这样容易过拟合，过拟合对应上图的 High Variance，点会很分散。低偏差对应的点都打在靶心附近，所以喵的很准，但不一定很稳；<br />**方差**：描述样本上训练出来的模型在测试集上的表现，要想方差表现的好，需要简化模型，减少模型的复杂度，但这样容易欠拟合，欠拟合对应上图 High Bias，点偏离中心。低方差对应就是点都打的很集中，但不一定是靶心附近，手很稳，但不一定瞄的准。<br />常说集成学习中的基模型是弱模型，通常来说弱模型是偏差高（在训练集上准确度低）方差小（防止过拟合能力强）的模型。但是，并不是所有集成学习框架中的基模型都是弱模型。Bagging 和 Stacking 中的基模型为强模型（偏差低方差高），Boosting 中的基模型为弱模型。<br />在 Bagging 和 Boosting 框架中，通过计算基模型的期望和方差可以得到模型整体的期望和方差。为了简化模型，假设基模型的权重 、方差 及两两间的相关系数 相等。由于 Bagging 和 Boosting 的基模型都是线性组成的，那么有：<br />模型总体期望：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161436695-4cfe79f8-5f40-4ea9-98c6-60404b5162ee.png#clientId=uee3aef42-04d0-4&from=paste&height=186&id=u14eff143&originHeight=464&originWidth=869&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46034&status=done&style=shadow&taskId=u5457c993-516d-4829-8743-97c328d7ae0&title=&width=347.6)<br />模型总体方差：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161423739-5bd01211-ccaf-4467-9e68-a6994ae30dd0.png#clientId=uee3aef42-04d0-4&from=paste&height=222&id=u8ccddd7d&originHeight=554&originWidth=1547&originalType=binary&ratio=1&rotation=0&showTitle=false&size=119796&status=done&style=shadow&taskId=u5df6a380-858e-4309-a8e7-1cf4eb6da97&title=&width=618.8)
<a name="caRHk"></a>
### 2.2 Bagging 的偏差与方差
对于 Bagging 来说，每个基模型的权重等于 1/m 且期望近似相等（子训练集都是从原训练集中进行子抽样），故可以进一步化简得到：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161408246-09879f28-d4a5-4990-93e0-4e1f40ad5414.png#clientId=uee3aef42-04d0-4&from=paste&height=276&id=u234095c5&originHeight=689&originWidth=1194&originalType=binary&ratio=1&rotation=0&showTitle=false&size=88944&status=done&style=shadow&taskId=uc58d3f05-1b50-46ba-b8ba-1cee1ffece0&title=&width=477.6)
<a name="tcG7k"></a>
#### **通过上式可以看到**
整体模型的期望等于基模型的期望，这也就意味着整体模型的偏差和基模型的偏差近似。<br />整体模型的方差小于等于基模型的方差，当且仅当相关性为 1 时取等号，随着基模型数量增多，整体模型的方差减少，从而防止过拟合的能力增强，模型的准确度得到提高。但是，模型的准确度一定会无限逼近于 1 吗？并不一定，当基模型数增加到一定程度时，方差公式第一项的改变对整体方差的作用很小，防止过拟合的能力达到极限，这便是准确度的极限了。<br />在此知道了为什么 Bagging 中的基模型一定要为强模型，如果 Bagging 使用弱模型则会导致整体模型的偏差提高，而准确度降低。Random Forest 是经典的基于 Bagging 框架的模型，并在此基础上通过引入特征采样和样本采样来降低基模型间的相关性，在公式中显著降低方差公式中的第二项，略微升高第一项，从而使得整体降低模型整体方差。
<a name="GqGZ0"></a>
### 2.3 Boosting 的偏差与方差
对于 Boosting 来说，基模型的训练集抽样是强相关的，那么模型的相关系数近似等于 1，故也可以针对 Boosting 化简公式为：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161388461-6c7f6f89-6ae5-455f-a5de-05c5a82125c5.png#clientId=uee3aef42-04d0-4&from=paste&height=192&id=u2330f7ff&originHeight=479&originWidth=1027&originalType=binary&ratio=1&rotation=0&showTitle=false&size=73203&status=done&style=shadow&taskId=uc061fb43-348c-4483-b271-0b89f6772c5&title=&width=410.8)
<a name="hDicE"></a>
#### **通过观察整体方差的表达式容易发现：**

- 整体模型的方差等于基模型的方差，如果基模型不是弱模型，其方差相对较大，这将导致整体模型的方差很大，即无法达到防止过拟合的效果。因此，Boosting 框架中的基模型必须为弱模型。
- 此外 Boosting 框架中采用基于贪心策略的前向加法，整体模型的期望由基模型的期望累加而成，所以随着基模型数的增多，整体模型的期望值增加，整体模型的准确度提高。

基于 Boosting 框架的 Gradient Boosting Decision Tree 模型中基模型也为树模型，同 Random Forrest，也可以对特征进行随机抽样来使基模型间的相关性降低，从而达到减少方差的效果。
<a name="qzLnf"></a>
### 2.4 小结

- 可以使用模型的偏差和方差来近似描述模型的准确度；
- 对于 Bagging 来说，整体模型的偏差与基模型近似，而随着模型的增加可以降低整体模型的方差，故其基模型需要为强模型；
- 对于 Boosting 来说，整体模型的方差近似等于基模型的方差，而整体模型的偏差由基模型累加而成，故基模型需要为弱模型。
<a name="HDHPB"></a>
## 3、随机森林
Random Forest（随机森林），用随机的方式建立一个森林。RF 算法由很多决策树组成，每一棵决策树之间没有关联。建立完森林后，当有新样本进入时，每棵决策树都会分别进行判断，然后基于投票法给出分类结果。
<a name="mMNTT"></a>
### 3.1 思想
Random Forest（随机森林）是 Bagging 的扩展变体，它在以决策树为基学习器构建 Bagging 集成的基础上，进一步在决策树的训练过程中引入了随机特征选择，因此可以概括 RF 包括四个部分：

1. 随机选择样本（放回抽样）；
2. 随机选择特征；
3. 构建决策树；
4. 随机森林投票（平均）。

随机选择样本和 Bagging 相同，采用的是 Bootstrap 自助采样法；随机选择特征是指在每个节点在分裂过程中都是随机选择特征的（区别与每棵树随机选择一批特征）。<br />这种随机性导致随机森林的偏差会有稍微的增加（相比于单棵不随机树），但是由于随机森林的“平均”特性，会使得它的方差减小，而且方差的减小补偿了偏差的增大，因此总体而言是更好的模型。<br />随机采样由于引入了两种采样方法保证了随机性，所以每棵树都是最大可能的进行生长就算不剪枝也不会出现过拟合。
<a name="EfmKp"></a>
### 3.2 优缺点

1. 在数据集上表现良好，相对于其他算法有较大的优势；
2. 易于并行化，在大数据集上有很大的优势；
3. 能够处理高维度数据，不用做特征选择。
<a name="hjEAP"></a>
## 4、AdaBoost
AdaBoost（Adaptive Boosting，自适应增强），其自适应在于：前一个基本分类器分错的样本会得到加强，加权后的全体样本再次被用来训练下一个基本分类器。同时，在每一轮中加入一个新的弱分类器，直到达到某个预定的足够小的错误率或达到预先指定的最大迭代次数。
<a name="IlX42"></a>
### 4.1 思想
<a name="pdLYP"></a>
#### **Adaboost 迭代算法有三步：**

1. 初始化训练样本的权值分布，每个样本具有相同权重；
2. 训练弱分类器，如果样本分类正确，则在构造下一个训练集中，它的权值就会被降低；反之提高。用更新过的样本集去训练下一个分类器；
3. 将所有弱分类组合成强分类器，各个弱分类器的训练过程结束后，加大分类误差率小的弱分类器的权重，降低分类误差率大的弱分类器的权重。
<a name="nYgEb"></a>
### 4.2 细节
<a name="I2ovX"></a>
#### **4.2.1 损失函数**
Adaboost 模型是加法模型，学习算法为前向分步学习算法，损失函数为指数函数的分类问题。<br />**加法模型：** 最终的强分类器是由若干个弱分类器加权平均得到的。<br />**前向分布学习算法：** 算法是通过一轮轮的弱学习器学习，利用前一个弱学习器的结果来更新后一个弱学习器的训练集权重。第 k 轮的强学习器为：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161222136-e18c5e1c-d7bc-472b-8ff8-fb66b3acde4f.png#clientId=uee3aef42-04d0-4&from=paste&height=75&id=u25b2d37f&originHeight=187&originWidth=1026&originalType=binary&ratio=1&rotation=0&showTitle=false&size=34288&status=done&style=shadow&taskId=u9601aa7c-4809-444e-a78d-de676f6ed46&title=&width=410.4)<br />损失函数：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161211117-d911e390-4078-4ca6-8011-7bf92de60fb7.png#clientId=uee3aef42-04d0-4&from=paste&height=67&id=udb18e539&originHeight=167&originWidth=720&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30969&status=done&style=shadow&taskId=ua1688c1c-ba19-4539-bfd1-30701099c1b&title=&width=288)<br />利用前向分布学习算法的关系可以得到损失函数为：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161197693-93cb110c-1d7e-44e3-bd08-8fe472f3eb53.png#clientId=uee3aef42-04d0-4&from=paste&height=71&id=u352c0505&originHeight=177&originWidth=1304&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51780&status=done&style=shadow&taskId=u16890d64-7925-4c7d-b45c-7e5b3307e8c&title=&width=521.6)<br />令 ![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161260378-ed1594d7-b420-4e69-bc7d-3e791876922a.png#clientId=uee3aef42-04d0-4&from=paste&height=32&id=uc1447dca&originHeight=81&originWidth=524&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17305&status=done&style=shadow&taskId=ua1fac9a3-5564-47d0-bfc5-e71483825fc&title=&width=209.6)，它的值不依赖于 ![](https://cdn.nlark.com/yuque/__latex/421e05ea7f35c0496a2802d995ab2e3f.svg#card=math&code=%5Calpha%2CG&id=EPSE1)，因此与最小化无关，仅仅依赖于 ![](https://cdn.nlark.com/yuque/__latex/5cd480f24dfb5a30f8a3a55b45618a38.svg#card=math&code=f_%7Bk-1%7D%28x%29&id=Wrpqw)，随着每一轮迭代而将这个式子带入损失函数，损失函数转化为：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161181833-95c9d246-77e6-408b-b27c-c1f64b661fa0.png#clientId=uee3aef42-04d0-4&from=paste&height=74&id=u8c8f1c20&originHeight=184&originWidth=1145&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46522&status=done&style=shadow&taskId=uabc13f72-c989-469c-935a-ab89d0926ac&title=&width=458)<br />通过求 ![](https://cdn.nlark.com/yuque/__latex/e092dd8cfa90769d37b50081b55ff2a5.svg#card=math&code=G_k%28x%29&id=qzZ25)，可以得到：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161163760-f40738bb-5b45-4521-806a-e1f513043aa3.png#clientId=uee3aef42-04d0-4&from=paste&height=72&id=u1b1ad392&originHeight=179&originWidth=969&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39483&status=done&style=shadow&taskId=u8fbce1fd-b516-4106-8e6a-47ed7e6912e&title=&width=387.6)<br />将 ![](https://cdn.nlark.com/yuque/__latex/e092dd8cfa90769d37b50081b55ff2a5.svg#card=math&code=G_k%28x%29&id=zhj37)带入损失函数，并对 ![](https://cdn.nlark.com/yuque/__latex/18d25ca4f77a9bbed9812e2bb0b350a5.svg#card=math&code=%5Calpha&id=rM7Jn) 求导，使其等于 0，则就得到了：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161105912-503d355a-7610-48ea-8d51-212f0fd2087a.png#clientId=uee3aef42-04d0-4&from=paste&height=50&id=uedc898d8&originHeight=124&originWidth=502&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8842&status=done&style=shadow&taskId=u2fcc0cb4-0cbb-49b1-a501-baa5da5076a&title=&width=200.8)<br />其中，![](https://cdn.nlark.com/yuque/__latex/dccbfd34e1a969b6afcc715f730885a2.svg#card=math&code=e_k&id=f4y1O) 即为前面的分类误差率。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161020544-4ea40b61-2f1e-47e3-9fd2-24e4ed90ffe9.png#clientId=uee3aef42-04d0-4&from=paste&height=113&id=u5aed61d3&originHeight=282&originWidth=1238&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55872&status=done&style=shadow&taskId=ubc3c13b0-ae48-42e8-95c9-06d2072a8bb&title=&width=495.2)<br />最后看样本权重的更新。利用 ![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161035377-ea76b05a-113f-4c7c-af1f-5161457b2b63.png#clientId=uee3aef42-04d0-4&from=paste&height=40&id=ub09acf71&originHeight=99&originWidth=658&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20550&status=done&style=shadow&taskId=uf8ca4a31-51c8-4310-a9fe-32ce78fda37&title=&width=263.2) 和 ![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161072505-b9013be9-fa75-4b58-ae2c-3d9a79e8dc2f.png#clientId=uee3aef42-04d0-4&from=paste&height=31&id=u66d58557&originHeight=78&originWidth=661&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22236&status=done&style=shadow&taskId=u43075388-a393-4aa2-b4a5-fee37f37e02&title=&width=264.4)，即可得：![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657161087243-fcaac869-486f-49f5-b8a5-194ff4067703.png#clientId=uee3aef42-04d0-4&from=paste&height=30&id=u4e5cef59&originHeight=75&originWidth=695&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22407&status=done&style=shadow&taskId=u0e4f3bdd-195c-46f4-82e3-e3297a9a3b4&title=&width=278)<br />这样就得到了样本权重更新公式。
<a name="FZH2X"></a>
#### **4.2.2 正则化**
为了防止 Adaboost 过拟合，通常也会加入正则化项，这个正则化项通常称为步长（learning rate）。定义为 ![](https://cdn.nlark.com/yuque/__latex/30d78d0c324bfbeb31c88c29415019fa.svg#card=math&code=%5Cnu&id=QT5Hm)，对于前面的弱学习器的迭代<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160816379-ffdfeabe-deeb-4560-865d-3bc83cb7412b.png#clientId=uee3aef42-04d0-4&from=paste&height=65&id=ud7283af5&originHeight=162&originWidth=749&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21407&status=done&style=shadow&taskId=uae592ecb-de0a-440c-acb4-cdfb854c834&title=&width=299.6)	<br />加上正则化有：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160825909-18a57a5e-732e-49b3-9809-996ea754b3c5.png#clientId=uee3aef42-04d0-4&from=paste&height=30&id=u6ac0717c&originHeight=74&originWidth=684&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20852&status=done&style=shadow&taskId=u8bd25b07-39a8-432a-bbde-851ce310508&title=&width=273.6)<br />![](https://cdn.nlark.com/yuque/__latex/30d78d0c324bfbeb31c88c29415019fa.svg#card=math&code=%5Cnu&id=VzyjS) 的取值范围为 ![](https://cdn.nlark.com/yuque/__latex/483a24cf3229c7722f080c4c92e27ad8.svg#card=math&code=0%3C%5Cnu%3C%3D1&id=zUEC7)。对于同样的训练集学习效果，较小的 ![](https://cdn.nlark.com/yuque/__latex/30d78d0c324bfbeb31c88c29415019fa.svg#card=math&code=%5Cnu&id=BsyKz) 意味着需要更多的弱学习器的迭代次数。通常用步长和迭代最大次数一起来决定算法的拟合效果。
<a name="k7hNw"></a>
### 4.3 优缺点
<a name="lwIYE"></a>
#### **优点**

1. 分类精度高；
2. 可以用各种回归分类模型来构建弱学习器，非常灵活；
3. 不容易发生过拟合。
<a name="PmpNR"></a>
#### **缺点**
对异常点敏感，异常点会获得较高权重。
<a name="mytOP"></a>
## 5、GBDT
GBDT（Gradient Boosting Decision Tree）是一种迭代的决策树算法，该算法由多棵决策树组成，从名字中可以看出来它是属于 Boosting 策略。GBDT 是被公认的泛化能力较强的算法。
<a name="GAhG1"></a>
### 5.1 思想
GBDT 由三个概念组成：Regression Decision Tree（即 DT）、Gradient Boosting（即 GB），和SHringkage（一个重要演变）
<a name="YIOL5"></a>
#### **5.1.1 回归树（Regression Decision Tree）**
如果认为 GBDT 由很多分类树那就大错特错了（虽然调整后也可以分类）。对于分类树而言，其值加减无意义（如性别），而对于回归树而言，其值加减才是有意义的（如说年龄）。GBDT 的核心在于累加所有树的结果作为最终结果，所以 GBDT 中的树都是回归树，不是分类树，这一点相当重要。<br />回归树在分枝时会穷举每一个特征的每个阈值以找到最好的分割点，衡量标准是最小化均方误差。
<a name="QrMv7"></a>
#### **5.1.2 梯度迭代（Gradient Boosting）**
上面说到 GBDT 的核心在于累加所有树的结果作为最终结果，GBDT 的每一棵树都是以之前树得到的残差来更新目标值，这样每一棵树的值加起来即为 GBDT 的预测值。模型的预测值可以表示为：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160795404-1b4149b7-ae4c-402f-a9c4-06c3859e5f9b.png#clientId=uee3aef42-04d0-4&from=paste&height=70&id=uca5eda0c&originHeight=175&originWidth=486&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9888&status=done&style=shadow&taskId=u73c7b8c0-8298-49c6-9f88-15f97b52e0b&title=&width=194.4)<br />![](https://cdn.nlark.com/yuque/__latex/ae31ccdda32c95885101d09cab2be309.svg#card=math&code=h_i%28x%29&id=ioR0T)为基模型与其权重的乘积，模型的训练目标是使预测值逼近真实值 ，也就是说要让每个基模型的预测值逼近各自要预测的部分真实值。由于要同时考虑所有基模型，导致了整体模型的训练变成了一个非常复杂的问题。所以研究者们想到了一个贪心的解决手段：每次只训练一个基模型。那么，现在改写整体模型为迭代式：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160764696-57d72df3-8288-4c35-934b-2296b50493a5.png#clientId=uee3aef42-04d0-4&from=paste&height=40&id=u7193c474&originHeight=99&originWidth=653&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10595&status=done&style=shadow&taskId=u41b94660-0c4f-486a-a07d-af8f5b4a7fc&title=&width=261.2)<br />这样一来，每一轮迭代中，只要集中解决一个基模型的训练问题：使 ![](https://cdn.nlark.com/yuque/__latex/5d8d0229ee6b475a0529d00f8eab6f6f.svg#card=math&code=F_i%28x%29&id=QIruj) 逼近真实值 ![](https://cdn.nlark.com/yuque/__latex/bf98c0ddcbe9c1e535f767c78c3aa813.svg#card=math&code=y&id=sFbug)。<br />举个例子：比如说 A 用户年龄 20 岁，第一棵树预测 12 岁，那么残差就是 8，第二棵树用 8 来学习，假设其预测为 5，那么其残差即为 3，如此继续学习即可。<br />那么 Gradient 从何体现？其实很简单，其残差其实是最小均方损失函数关于预测值的反向梯度：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160752547-a794b51d-06f9-413d-a994-2627f9fdbd6d.png#clientId=uee3aef42-04d0-4&from=paste&height=59&id=ue2842634&originHeight=147&originWidth=891&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30879&status=done&style=shadow&taskId=uc295def7-5ef4-4578-b0d4-5defd2440b3&title=&width=356.4)<br />也就是说，若 ![](https://cdn.nlark.com/yuque/__latex/8f8190475432d4878c0266cd5b827d2c.svg#card=math&code=F_%7Bi-1%7D%28x%29&id=IJIEn) 加上拟合了反向梯度的 ![](https://cdn.nlark.com/yuque/__latex/ae31ccdda32c95885101d09cab2be309.svg#card=math&code=h_i%28x%29&id=cwUPS) 得到 ![](https://cdn.nlark.com/yuque/__latex/5d8d0229ee6b475a0529d00f8eab6f6f.svg#card=math&code=F_i%28x%29&id=C0zxe)，该值可能将导致平方差损失函数降低，预测的准确度提高！<br />但要注意，基于残差 GBDT 容易对异常值敏感，举例：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657160521730-237e4637-7168-4bde-8d4d-0cea0eedd285.jpeg#clientId=uee3aef42-04d0-4&from=paste&id=u3f210430&originHeight=149&originWidth=753&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc4e3f30-00e8-47fd-a8d5-431ef1f316c&title=)<br />很明显后续的模型会对第 4 个值关注过多，这不是一种好的现象，所以一般回归类的损失函数会用绝对损失或者 Huber 损失函数来代替平方损失函数。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657160523340-5f0274ba-be1a-481e-855c-be5f24786567.jpeg#clientId=uee3aef42-04d0-4&from=paste&id=u35711762&originHeight=499&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u366c18e6-9dba-4563-8c9f-933594c1fa3&title=)<br />GBDT 的 Boosting 不同于 Adaboost 的 Boosting，GBDT 的每一步残差计算其实变相地增大了被分错样本的权重，而对与分对样本的权重趋于 0，这样后面的树就能专注于那些被分错的样本。
<a name="Znbsm"></a>
#### **5.1.13 缩减（Shrinkage）**
Shrinkage 的思想认为，每走一小步逐渐逼近结果的效果要比每次迈一大步很快逼近结果的方式更容易避免过拟合。即它并不是完全信任每一棵残差树。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657160662793-afec45b2-00bf-45f7-885e-7902f068bde0.png#clientId=uee3aef42-04d0-4&from=paste&height=48&id=u5f125fcf&originHeight=119&originWidth=821&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20695&status=done&style=shadow&taskId=ubcfaf475-4403-458f-aa07-8b1387cd311&title=&width=328.4)<br />Shrinkage 不直接用残差修复误差，而是只修复一点点，把大步切成小步。本质上 Shrinkage 为每棵树设置了一个 weight，累加时要乘以这个 weight，当 weight 降低时，基模型数会配合增大。
<a name="dJMgU"></a>
### 5.2 优缺点
<a name="pojP0"></a>
#### **优点**

1. 可以自动进行特征组合，拟合非线性数据；
2. 可以灵活处理各种类型的数据。
<a name="Zp9gJ"></a>
#### **缺点**
对异常点敏感。
<a name="TjnVE"></a>
### 5.3 与 Adaboost 的对比
<a name="A6zGs"></a>
#### **相同：**

1. 都是 Boosting 家族成员，使用弱分类器；
2. 都使用前向分布算法。
<a name="k5uFq"></a>
#### **不同：**

1. **迭代思路不同：** Adaboost 是通过提升错分数据点的权重来弥补模型的不足（利用错分样本），而 GBDT 是通过算梯度来弥补模型的不足（利用残差）；
2. **损失函数不同：** AdaBoost 采用的是指数损失，GBDT 使用的是绝对损失或者 Huber 损失函数。
