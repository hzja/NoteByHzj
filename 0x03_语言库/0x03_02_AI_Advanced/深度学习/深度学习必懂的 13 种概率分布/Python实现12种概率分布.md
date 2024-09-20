Python<br />机器学习有其独特的数学基础，用微积分来处理变化无限小的函数，并计算它们的变化；<br />使用线性代数来处理计算过程；还用概率论与统计学建模不确定性。<br />在这其中，概率论有其独特的地位，模型的预测结果、学习过程、学习目标都可以通过概率的角度来理解。<br />与此同时，从更细的角度来说，随机变量的概率分布也是必须理解的内容。项目作者介绍了所有需要了解的统计分布，他还提供了每一种分布的实现代码。<br />**项目地址：**[https://github.com/graykode/distribution-is-all-you-need](https://github.com/graykode/distribution-is-all-you-need)

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454485-4972dded-a975-4e62-a99d-c051ef60869e.png#clientId=ua7429062-4a99-4&from=paste&id=u0824ab48&originHeight=534&originWidth=974&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc3ad03b-a8b8-4747-83f7-431225bef8e&title=)<br />下面先看看总体上概率分布都有什么吧：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651544454540-b90972ce-fa64-4459-a856-6c6c41974d06.jpeg#clientId=ua7429062-4a99-4&from=paste&id=u0e9a4ccd&originHeight=610&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uadc7ce54-73dd-4997-904b-3b2104b98b3&title=)<br />非常有意思的是，上图每一种分布都是有联系的。比如说伯努利分布，它重复几次就是二项分布，如果再扩展到多类别，就成为了多项式分布。注意，其中共轭（conjugate）表示的是互为共轭的概率分布；Multi-Class 表示随机变量多于 2 个；N Times 表示还会考虑先验分布 P(X)。<br />在贝叶斯概念理论中，如果后验分布 p(θ | x) 与先验分布 p(θ) 是相同的概率分布族，那么后验分布可以称为共轭分布，先验分布可以称为似然函数的共轭先验。<br />为了学习概率分布，项目作者建议查看 Bishop 的模式识别与机器学习。当然，要是准备再过一遍《概率论与数理统计》，那也是极好的。
<a name="ZoYaZ"></a>
## 概率分布与特性
<a name="TPk3D"></a>
### 1、均匀分布（连续型）
均匀分布是指闭区间 [a, b] 内的随机变量，且每一个变量出现的概率是相同的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454553-6e5040ec-3fda-4adf-ac9a-b60faf4ee1b9.png#clientId=ua7429062-4a99-4&from=paste&id=u11ac8335&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc4ffde0-82a3-4133-969d-b89508705e6&title=)
<a name="pB4uh"></a>
### 2、伯努利分布（离散型）
伯努利分布并不考虑先验概率 P(X)，它是单个二值随机变量的分布。它由单个参数φ∈ [0, 1] 控制，φ 给出了随机变量等于 1 的概率。使用二元交叉熵函数实现二元分类，它的形式与对伯努利分布取负对数是一致的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454489-d3c2ee03-e254-46f6-a02b-4867f0750f81.png#clientId=ua7429062-4a99-4&from=paste&id=ufed403ac&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3e920615-fb57-40d9-b144-b7732e2e96b&title=)
<a name="rGlcF"></a>
### 3、二项分布（离散型）
二项分布是由伯努利提出的概念，指的是重复 n 次独立的伯努利试验。在每次试验中只有两种可能的结果，而且两种结果发生与否互相对立。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454492-a2f1d8aa-18a2-405f-9998-b46427f2d60a.png#clientId=ua7429062-4a99-4&from=paste&id=ud00d7126&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub77763f9-a59c-48e6-9892-f50507ca4c6&title=)
<a name="bs6Ha"></a>
### 4、Multi-Bernoulli 分布（离散型）
Multi-Bernoulli 分布又称为范畴分布（Categorical distribution），它的类别超过 2，交叉熵的形式与该分布的负对数形式是一致的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454838-a11ac00a-a2c7-4ae4-bd31-543d9a977bc5.png#clientId=ua7429062-4a99-4&from=paste&id=ud196b2c8&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u14133afb-5caf-45d6-bf8c-9df452363d8&title=)
<a name="P8Y7H"></a>
### 5、多项式分布（离散型）
范畴分布是多项式分布（Multinomial distribution）的一个特例，它与范畴分布的关系就像伯努利分布与二项分布之间的关系。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454886-3e98c902-cb63-4b0a-a520-0a409f632192.png#clientId=ua7429062-4a99-4&from=paste&id=ubb30c99f&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u054897fa-e2f0-476d-93af-4d75b949f11&title=)
<a name="UdF0f"></a>
### 6、Beta 分布（连续型）
贝塔分布（Beta Distribution) 是一个作为伯努利分布和二项式分布的共轭先验分布的密度函数，它指一组定义在 (0,1) 区间的连续概率分布。均匀分布是 Beta 分布的一个特例，即在 alpha=1、 beta=1 的分布。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454898-42eb94ba-7a1e-474e-ab6c-64bce5438d0f.png#clientId=ua7429062-4a99-4&from=paste&id=ua9c93ba3&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uea355c1d-54e0-4c40-bc00-e67dc6f2beb&title=)
<a name="kJzom"></a>
### 7、狄利克雷分布（连续型）
狄利克雷分布（Dirichlet distribution）是一类在实数域以正单纯形（standard simplex）为支撑集（support）的高维连续概率分布，是 Beta 分布在高维情形的推广。在贝叶斯推断中，狄利克雷分布作为多项式分布的共轭先验得到应用，在机器学习中被用于构建狄利克雷混合模型。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651544454946-fae31f68-4596-49ee-9abd-b94604aefdaf.jpeg#clientId=ua7429062-4a99-4&from=paste&id=u78dd74f3&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u05ceae9d-fb12-4a75-b7d0-8fae05e5d47&title=)
<a name="nmiyl"></a>
### 8、Gamma 分布（连续型）
Gamma 分布是统计学中的常见连续型分布，指数分布、卡方分布和 Erlang 分布都是它的特例。如果 Gamma(a,1) / Gamma(a,1) + Gamma(b,1)，那么 Gamma 分布就等价于 Beta(a, b) 分布。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544454954-ab719bdd-bbf1-4405-bdbd-56bf9651fdbe.png#clientId=ua7429062-4a99-4&from=paste&id=u1d3e6b9b&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0a097fdf-6389-40a5-93b6-685caf0b250&title=)
<a name="u4cze"></a>
### 9、指数分布（连续型）
指数分布可以用来表示独立随机事件发生的时间间隔，比如旅客进入机场的时间间隔、打进客服中心电话的时间间隔等等。当 alpha 等于 1 时，指数分布就是 Gamma 分布的特例。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544455224-af34c54a-e754-4eed-a376-484a99ed8dcc.png#clientId=ua7429062-4a99-4&from=paste&id=ue26362f1&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7e71095b-b4ed-47fd-bb8f-468e0a40aa3&title=)
<a name="hNd0F"></a>
### 10、高斯分布（连续型）
高斯分布或正态分布是最为重要的分布之一，它广泛应用于整个机器学习的模型中。例如，权重用高斯分布初始化、隐藏向量用高斯分布进行归一化等等。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544455299-3713430b-156d-48c0-a4ff-f2e77f7ebbc9.png#clientId=ua7429062-4a99-4&from=paste&id=u348bc0df&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9f46fc43-d40c-4de8-b05f-472a3ba593e&title=)<br />当正态分布的均值为 0、方差为 1 的时候，它就是标准正态分布，这也是最常用的分布。
<a name="AoitO"></a>
### 11、卡方分布（连续型）
简单而言，卡方分布（Chi-squared）可以理解为，k 个独立的标准正态分布变量的平方和服从自由度为 k 的卡方分布。卡方分布是一种特殊的伽玛分布，是统计推断中应用最为广泛的概率分布之一，例如假设检验和置信区间的计算。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544455335-f4b3ea92-f62e-49b7-8b8d-d6533fbd7e39.png#clientId=ua7429062-4a99-4&from=paste&id=ue5101dd8&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u30c099b6-8117-4816-a3bf-dbf13ef70c1&title=)
<a name="iCcqV"></a>
### 12、学生 t-分布
学生 t-分布（Student t-distribution）用于根据小样本来估计呈正态分布且变异数未知的总体，其平均值是多少。t 分布也是对称的倒钟型分布，就如同正态分布一样，但它的长尾占比更多，这意味着 t 分布更容易产生远离均值的样本。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651544455395-8cbf249c-79d0-4140-9ca1-576139b2d1e9.png#clientId=ua7429062-4a99-4&from=paste&id=u8e0b4e67&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0144a1b8-5dde-44b1-a76c-4f91eb4a22e&title=)
<a name="oH6Jh"></a>
## 分布的代码实现
上面多种分布的 NumPy 构建方式以及制图方式都提供了对应的代码，可在原项目中查阅。如下所示展示了指数分布的构建的制图方式，可以直接定义概率密度函数，再打印出来就好了。
```python
import numpy as np
from matplotlib import pyplot as plt

def exponential(x, lamb):
    y = lamb * np.exp(-lamb * x)
    return x, y, np.mean(y), np.std(y)

for lamb in [0.5, 1, 1.5]:

    x = np.arange(0, 20, 0.01, dtype=np.float)
    x, y, u, s = exponential(x, lamb=lamb)
    plt.plot(x, y, label=r'$mu=%.2f, sigma=%.2f,'
                         r' lambda=%d$' % (u, s, lamb))
plt.legend()
plt.savefig('graph/exponential.png')
plt.show()
```
