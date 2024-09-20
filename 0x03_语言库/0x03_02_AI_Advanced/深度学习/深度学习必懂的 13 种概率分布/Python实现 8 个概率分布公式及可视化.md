Python<br />介绍一些常见的分布并通过Python 代码进行可视化以直观地显示它们。<br />概率和统计知识是数据科学和机器学习的核心； 需要统计和概率知识来有效地收集、审查、分析数据。<br />现实世界中有几个现象实例被认为是统计性质的（即天气数据、销售数据、财务数据等）。这意味着在某些情况下，已经能够开发出方法来帮助我们通过可以描述数据特征的数学函数来模拟自然。<br />“概率分布是一个数学函数，它给出了实验中不同可能结果的发生概率。”<br />了解数据的分布有助于更好地模拟周围的世界。它可以帮助我们确定各种结果的可能性，或估计事件的可变性。所有这些都使得了解不同的概率分布在数据科学和机器学习中非常有价值。
<a name="xF8BD"></a>
## 均匀分布
最直接的分布是均匀分布。均匀分布是一种概率分布，其中所有结果的可能性均等。例如，如果掷一个公平的骰子，落在任何数字上的概率是 1/6。这是一个离散的均匀分布。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422204706-7ff89cc6-56f9-411b-aa31-969dd3143384.png#clientId=ufb6bcabe-964a-4&from=paste&height=52&id=QwE7R&originHeight=130&originWidth=361&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3926&status=done&style=shadow&taskId=ufef47c85-4360-45c7-91ff-4101beca7ff&title=&width=144.4)<br />但是并不是所有的均匀分布都是离散的——它们也可以是连续的。它们可以在指定范围内取任何实际值。a 和 b 之间连续均匀分布的概率密度函数 (PDF) 如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422227012-87976229-4d23-4686-b8f3-93e6a715fb52.png#clientId=ufb6bcabe-964a-4&from=paste&height=61&id=ue0516918&originHeight=152&originWidth=426&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5819&status=done&style=shadow&taskId=u5ab93b3d-da04-4759-8647-dc0cb62ec01&title=&width=170.4)<br />看看如何在 Python 中对它们进行编码：
```python
import numpy as np  
import matplotlib.pyplot as plt 
from scipy import stats 
 
# for continuous  
a = 0 
b = 50 
size = 5000 
 
X_continuous = np.linspace(a, b, size) 
continuous_uniform = stats.uniform(loc=a, scale=b) 
continuous_uniform_pdf = continuous_uniform.pdf(X_continuous) 
 
# for discrete 
X_discrete = np.arange(1, 7) 
discrete_uniform = stats.randint(1, 7) 
discrete_uniform_pmf = discrete_uniform.pmf(X_discrete)  
 
# plot both tables 
fig, ax = plt.subplots(nrows=1, ncols=2, figsize=(15,5)) 
# discrete plot 
ax[0].bar(X_discrete, discrete_uniform_pmf) 
ax[0].set_xlabel("X") 
ax[0].set_ylabel("Probability") 
ax[0].set_title("Discrete Uniform Distribution") 
# continuous plot 
ax[1].plot(X_continuous, continuous_uniform_pdf) 
ax[1].set_xlabel("X") 
ax[1].set_ylabel("Probability") 
ax[1].set_title("Continuous Uniform Distribution") 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979029-1dc4a1d4-5a90-4cb5-b943-1dc60cb2e549.png#clientId=ufb6bcabe-964a-4&from=paste&id=u4fd38f89&originHeight=300&originWidth=803&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud7fb9cef-ce00-4393-b3de-7f58ffe09f4&title=)
<a name="pYzkl"></a>
## 高斯分布
高斯分布可能是最常听到也熟悉的分布。它有几个名字：有人称它为钟形曲线，因为它的概率图看起来像一个钟形，有人称它为高斯分布，因为首先描述它的德国数学家卡尔·高斯命名，还有一些人称它为正态分布，因为早期的统计学家 注意到它一遍又一遍地再次发生。<br />正态分布的概率密度函数如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422190856-5989b766-8f2a-4f05-baba-29919ada9df5.png#clientId=ufb6bcabe-964a-4&from=paste&height=72&id=ud9f58b56&originHeight=181&originWidth=674&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15017&status=done&style=shadow&taskId=u546ac53b-6f18-475a-b6b1-68939191902&title=&width=269.6)<br />σ 是标准偏差，μ 是分布的平均值。要注意的是，在正态分布中，均值、众数和中位数都是相等的。<br />当绘制正态分布的随机变量时，曲线围绕均值对称——一半的值在中心的左侧，一半在中心的右侧。并且，曲线下的总面积为 1。
```python
mu = 0 
variance = 1 
sigma = np.sqrt(variance) 
x = np.linspace(mu - 3*sigma, mu + 3*sigma, 100) 
 
plt.subplots(figsize=(8, 5)) 
plt.plot(x, stats.norm.pdf(x, mu, sigma)) 
plt.title("Normal Distribution") 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979080-b8e18965-56bd-4d8a-b14d-a88e778a0cef.png#clientId=ufb6bcabe-964a-4&from=paste&id=uab434d44&originHeight=286&originWidth=440&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u172e79b7-1b77-462e-9c0f-380aef8e683&title=)<br />对于正态分布来说。经验规则告诉我们数据的百分比落在平均值的一定数量的标准偏差内。这些百分比是：

- 68% 的数据落在平均值的一个标准差内。
- 95% 的数据落在平均值的两个标准差内。
- 99.7% 的数据落在平均值的三个标准差范围内。
<a name="lelMi"></a>
## 对数正态分布
对数正态分布是对数呈正态分布的随机变量的连续概率分布。因此，如果随机变量 X 是对数正态分布的，则 Y = ln(X) 具有正态分布。<br />这是对数正态分布的 PDF：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422179771-8ebaadd1-d23e-4fe9-8650-fb02245c2dc5.png#clientId=ufb6bcabe-964a-4&from=paste&height=68&id=ufc106d61&originHeight=170&originWidth=900&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27960&status=done&style=shadow&taskId=ua07bd812-6701-43f2-bd89-710f4587d79&title=&width=360)<br />对数正态分布的随机变量只取正实数值。因此，对数正态分布会创建右偏曲线。<br />在 Python 中绘制它：
```python
X = np.linspace(0, 6, 500) 
 
std = 1 
mean = 0 
lognorm_distribution = stats.lognorm([std], loc=mean) 
lognorm_distribution_pdf = lognorm_distribution.pdf(X) 
 
fig, ax = plt.subplots(figsize=(8, 5)) 
plt.plot(X, lognorm_distribution_pdf, label="μ=0, σ=1") 
ax.set_xticks(np.arange(min(X), max(X))) 
 
std = 0.5 
mean = 0 
lognorm_distribution = stats.lognorm([std], loc=mean) 
lognorm_distribution_pdf = lognorm_distribution.pdf(X) 
plt.plot(X, lognorm_distribution_pdf, label="μ=0, σ=0.5") 
 
std = 1.5 
mean = 1 
lognorm_distribution = stats.lognorm([std], loc=mean) 
lognorm_distribution_pdf = lognorm_distribution.pdf(X) 
plt.plot(X, lognorm_distribution_pdf, label="μ=1, σ=1.5") 
 
plt.title("Lognormal Distribution") 
plt.legend() 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979019-114eab13-8163-420b-b95a-fc57de416085.png#clientId=ufb6bcabe-964a-4&from=paste&id=u00b15dd9&originHeight=286&originWidth=431&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56059110-e247-46be-ade7-dd3cfdbe656&title=)
<a name="p383D"></a>
## 泊松分布
泊松分布以法国数学家西蒙·丹尼斯·泊松的名字命名。这是一个离散的概率分布，这意味着它计算具有有限结果的事件——换句话说，它是一个计数分布。因此，泊松分布用于显示事件在指定时期内可能发生的次数。<br />如果一个事件在时间上以固定的速率发生，那么及时观察到事件的数量（n）的概率可以用泊松分布来描述。例如，顾客可能以每分钟 3 次的平均速度到达咖啡馆。可以使用泊松分布来计算 9 个客户在 2 分钟内到达的概率。<br />下面是概率质量函数公式：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422164178-a7c015dc-67a3-4256-8cb9-ffc2b9c4708b.png#clientId=ufb6bcabe-964a-4&from=paste&height=76&id=u1413a86b&originHeight=190&originWidth=467&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7551&status=done&style=shadow&taskId=u971b2762-fd3f-49b9-a977-5994449b291&title=&width=186.8)<br />λ 是一个时间单位的事件率——在例子中，它是 3。k 是出现的次数——在我们的例子中，它是 9。这里可以使用 Scipy 来完成概率的计算。
```python
from scipy import stats 

print(stats.poisson.pmf(k=9, mu=3)) 

0.002700503931560479
```
泊松分布的曲线类似于正态分布，λ 表示峰值。
```python
X = stats.poisson.rvs(mu=3, size=500) 
 
plt.subplots(figsize=(8, 5)) 
plt.hist(X, density=True, edgecolor="black") 
plt.title("Poisson Distribution") 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979072-86a7bb60-87cd-47fd-932e-ea0f47a0ea57.png#clientId=ufb6bcabe-964a-4&from=paste&id=u8d8b3742&originHeight=288&originWidth=440&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7752628b-9bfb-447d-b4d4-919cec9c588&title=)
<a name="xql7s"></a>
## 指数分布
指数分布是泊松点过程中事件之间时间的概率分布。指数分布的概率密度函数如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422147495-24ad80fd-0c58-4d83-a0e2-aaaceb58e8cd.png#clientId=ufb6bcabe-964a-4&from=paste&height=65&id=uaf003a38&originHeight=163&originWidth=559&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16958&status=done&style=shadow&taskId=u58ad970b-8174-4c2d-baf9-2ef70ba8572&title=&width=223.6)<br />λ 是速率参数，x 是随机变量。
```python
X = np.linspace(0, 5, 5000) 
 
exponetial_distribtuion = stats.expon.pdf(X, loc=0, scale=1) 
 
plt.subplots(figsize=(8,5)) 
plt.plot(X, exponetial_distribtuion) 
plt.title("Exponential Distribution") 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979051-9076c6f7-5ab7-4bef-9f03-407fcfdedf6a.png#clientId=ufb6bcabe-964a-4&from=paste&id=u2ed7c787&originHeight=281&originWidth=434&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1a4feb6b-094a-483d-b286-54d1d0af212&title=)
<a name="fSBiS"></a>
## 二项分布
可以将二项分布视为实验中成功或失败的概率。有些人也可能将其描述为抛硬币概率。<br />参数为 n 和 p 的二项式分布是在 n 个独立实验序列中成功次数的离散概率分布，每个实验都问一个是 - 否问题，每个实验都有自己的布尔值结果：成功或失败。<br />本质上，二项分布测量两个事件的概率。一个事件发生的概率为 p，另一事件发生的概率为 1-p。<br />这是二项分布的公式：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422129433-6af4ee8a-7eac-4584-9ec0-0e187e1bb7e8.png#clientId=ufb6bcabe-964a-4&from=paste&height=79&id=u432ae4d9&originHeight=197&originWidth=558&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8860&status=done&style=shadow&taskId=u140a7f98-dce2-4498-a12a-1d7dd284a73&title=&width=223.2)

- P = 二项分布概率
- = 组合数
- x = n次试验中特定结果的次数
- p = 单次实验中，成功的概率
- q = 单次实验中，失败的概率
- n = 实验的次数

可视化代码如下：
```python
X = np.random.binomial(n=1, p=0.5, size=1000) 

plt.subplots(figsize=(8, 5)) 
plt.hist(X) 
plt.title("Binomial Distribution") 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979406-5cebce38-2603-4889-b089-3c4a6ec11803.png#clientId=ufb6bcabe-964a-4&from=paste&id=ufd36104b&originHeight=283&originWidth=434&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6542a894-9e04-42ca-93e3-f7109370000&title=)
<a name="kbyZC"></a>
## 学生 t 分布
学生 t 分布（或简称 t 分布）是在样本量较小且总体标准差未知的情况下估计正态分布总体的均值时出现的连续概率分布族的任何成员。它是由英国统计学家威廉·西利·戈塞特（William Sealy Gosset）以笔名“student”开发的。<br />PDF如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422113197-b1564113-88a1-47fc-ab08-b96caaf8be74.png#clientId=ufb6bcabe-964a-4&from=paste&height=96&id=ua8a1ee1e&originHeight=241&originWidth=890&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26265&status=done&style=shadow&taskId=uf701463f-81e1-4dde-bd6d-79c668aeaff&title=&width=356)<br />n 是称为“自由度”的参数，有时可以看到它被称为“d.o.f.” 对于较高的 n 值，t 分布更接近正态分布。
```python
import seaborn as sns 
from scipy import stats 
 
X1 = stats.t.rvs(df=1, size=4) 
X2 = stats.t.rvs(df=3, size=4) 
X3 = stats.t.rvs(df=9, size=4) 
 
plt.subplots(figsize=(8,5)) 
sns.kdeplot(X1, label = "1 d.o.f") 
sns.kdeplot(X2, label = "3 d.o.f") 
sns.kdeplot(X3, label = "6 d.o.f") 
plt.title("Student's t distribution") 
plt.legend() 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979501-0caa6090-2203-4ae9-91b3-96e25d21494e.png#clientId=ufb6bcabe-964a-4&from=paste&id=ud6aa2cb6&originHeight=284&originWidth=451&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0c6e62c9-041b-4ffb-9676-ce395f362b9&title=)
<a name="zxTxC"></a>
## 卡方分布
卡方分布是伽马分布的一个特例；对于 k 个自由度，卡方分布是一些独立的标准正态随机变量的 k 的平方和。<br />PDF如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659422095179-b8384813-5d70-40d9-8677-90c37be34049.png#clientId=ufb6bcabe-964a-4&from=paste&height=71&id=ucaa35931&originHeight=177&originWidth=675&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18613&status=done&style=shadow&taskId=ua121b1c2-bbc7-45a3-9e23-75417dcf79d&title=&width=270)<br />这是一种流行的概率分布，常用于假设检验和置信区间的构建。<br />在 Python 中绘制一些示例图：
```python
X = np.arange(0, 6, 0.25) 
 
plt.subplots(figsize=(8, 5)) 
plt.plot(X, stats.chi2.pdf(X, df=1), label="1 d.o.f") 
plt.plot(X, stats.chi2.pdf(X, df=2), label="2 d.o.f") 
plt.plot(X, stats.chi2.pdf(X, df=3), label="3 d.o.f") 
plt.title("Chi-squared Distribution") 
plt.legend() 
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659421979438-8c140769-ac65-4d78-9ee6-6c7577f01ebb.png#clientId=ufb6bcabe-964a-4&from=paste&id=uee483a23&originHeight=284&originWidth=433&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2e038039-b5e9-4f3d-9c6b-b7ac3fa1a8a&title=)<br />掌握统计学和概率对于数据科学至关重要。
