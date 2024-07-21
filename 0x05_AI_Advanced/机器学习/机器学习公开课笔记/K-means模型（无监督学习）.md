机器学习 无监督学习<br />建模过程：一个模型参数的调整过程。即：给模型输入x后，输出尽可能逼近已知目标y。<br />在回归模型、贝叶斯模型、决策树模型中，建模阶段都是调用`fit`函数，传递两个参数x：用来做预测的特征;y：已知的目标。<br />回归模型：RMSE、对数类似函数<br />决策树：基尼系数<br />贝叶斯模型：用已知目标的数据来提供似然性<br />这些方法都必须给出已知的Y，否则模型就无法建立。对于这一类方法有一个专门的名称叫有监督学习。
<a name="hxEZO"></a>
## 有监督学习
x：变量或独立变量、特征、输入等等;<br />Y：响应、目标、标签、依赖变量、输出等等;<br />真正工作时：调用`predict`函数。<br />对未知响应的新数据来进行回归或分类。<br />实质：通过对训练数据的分析，建立一种从输人特征到输出响应的联系。<br />【例】客户是否有支票账户、目前欠款金额，性别和婚姻状况，可以知道他是否是高风险客户。
<a name="ozBDN"></a>
### 有监督模型
☆模型本身<br />☆准确的标签<br />劣势：标签数据的缺乏，成为该类模型的瓶颈。
<a name="tMee9"></a>
### 无监督模型
无监督学习模型的建模过程中，并不需要给出目标或响应，而是完全根据输入特征，来找出相似的数据。<br />如：聚类     数据被分为几簇<br />评价标准：依靠数据科学家结合专业领域知识后的理解与解释。
<a name="c2dt3"></a>
#### 应用场合
没有明显响应变量（不明确要预测什么）<br />从无明显模式的数据中寻找模式（特征提取）
<a name="xAHkw"></a>
## 无监督模型——K-means模型
聚类任务的无监督学习模型。<br />簇( cluster )：一组相似的数据。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628174014142-00295d03-5588-44cc-ba96-a509ded4a2f4.png#clientId=ua71c4f6f-d54a-4&from=paste&height=332&id=u424ea659&originHeight=664&originWidth=695&originalType=binary&ratio=1&size=366781&status=done&style=none&taskId=u4388b4d8-cfdb-494a-93b1-9f265073b14&width=348)<br />若有n个特征，可构造出n维特征空间。每个样本数据都是空间中以其具体特征为坐标的点。<br />相似性衡量——点与点在特征空间中的距离
```python
import numpy as np
import pandas as pd
import random
from scipy import stats
from matplotlib import pyplot as plt
x1=[]
x2=[]
y1=[]
y2=[]
for n in range(40):
x1.append(random.gauss(-1,0.25))
x2.append(random.gauss(-1,0.25))
y1.append(random.gauss(1,0.25))
y2.append(random.gauss(1,0.25))
plt.figure(figsize=(8,6))
plt.plot(x1,y1,'og')
plt.plot(x1,x2,'og')
plt.plot(y1,x1,'og')
plt.plot(y1,y2,'og')
plt.xlim([-2.5,2.5])
plt.ylim([-2.5,2.5])
plt.savefig('cluster')
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628174447142-b44de09a-f955-4fc7-86da-729ade5b2f9a.png#clientId=ua71c4f6f-d54a-4&from=paste&height=395&id=u4771acdb&originHeight=1184&originWidth=1627&originalType=binary&ratio=1&size=113101&status=done&style=shadow&taskId=u867fc424-62d2-4ea5-96df-559c261270b&width=542.3333333333334)<br />图中的数据，也就是点形成了四个不同的簇，每个簇中的点虽相同却又不靠近。那么每个簇由谁来代表呢，由此引出簇中心的概念：簇中所有点的平均。
```python
plt.figure(figsize=(8,6))
plt.plot(x1,y1,'og')
plt.plot(x1,x2,'og')
plt.plot(y1,x1,'og')
plt.plot(y1,y2,'og')
plt.xlim([-2.5,2.5])
plt.ylim([-2.5,2.5])
plt.plot(np.mean(x1),np.mean(y1),'*r',markersize=15)
plt.plot(np.mean(x1),np.mean(x2),'*r',markersize=15)
plt.plot(np.mean(y1),np.mean(x1),'*r',markersize=15)
plt.plot(np.mean(y1),np.mean(y2),'*r',markersize=15)
plt.savefig('cluster2')
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628174475944-525c5e40-736a-4f05-9e44-288f83579680.png#clientId=ua71c4f6f-d54a-4&from=paste&height=419&id=ufd76085d&originHeight=1257&originWidth=1650&originalType=binary&ratio=1&size=120237&status=done&style=shadow&taskId=ub895b03e-7549-44b5-b1d1-63587f43cd0&width=550)
<a name="CH0OV"></a>
### K-means模型的迭代算法
第1步:选择k个初始的簇中心点;<br />第2步:遍历所有的点,把每个点分配到离它最近的那个簇;<br />第3步:重新计算簇中心;<br />第4步:重复前面的第2和第3步，直到达到停止准则。<br />Scikit-learn库中有现成的K-means对象可以实现K-means聚类<br />以一个电影数据为示例
```python
my_data=pd.read_csv('C:\Python\Scripts\my_data\\tmdb_5000_movies.csv')
my_data.describe()

from sklearn.cluster import KMeans
# 以'budget','popularity','revenue'三个特征来作为聚类分析的特征
X=my_data[['budget','popularity','revenue']]
km=KMeans(n_clusters=3,random_state=1)
# 这次的fit函数只传了一个参数
km.fit(X)
my_cl=pd.DataFrame(data=km.labels_,columns=['cluster'])
X=pd.concat([X,my_cl],axis=1)
X.head(5)
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628174689594-894bf5e5-c06f-4615-b0db-28bf51464212.png#clientId=ua71c4f6f-d54a-4&from=paste&height=133&id=ud9e0b906&originHeight=400&originWidth=1559&originalType=binary&ratio=1&size=155377&status=done&style=shadow&taskId=uadc462f8-f08d-4c1b-aaa7-b2be4119469&width=519.6666666666666)
```python
# 对聚类得到的结果进行分组统计
X.groupby('cluster').mean()
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628174783348-0be69b6f-31f1-4f5c-b30e-7eaec0b4c56a.png#clientId=ua71c4f6f-d54a-4&from=paste&height=118&id=uabe8475c&originHeight=355&originWidth=1657&originalType=binary&ratio=1&size=113737&status=done&style=shadow&taskId=u74d7dd84-7002-4524-894a-42b96ac7f80&width=552.3333333333334)
<a name="LxR1v"></a>
### 图形化展示聚类的结果
```python
x=X['budget']
y=X['popularity']
z=X['revenue']
colors=list()
palette={0:"red",1:"green",2:"blue"}
# 字典，给三种类别对应散点图中的三种 marker_color
for n,row in enumerate(X['cluster']): # 根据类别为每个样本设置绘图颜色
	colors.append(palette[X['cluster'][n]])#
fig = plt.figure(figsize=(12,10))
ax = fig.gca(projection='3d')
ax.scatter(x,y,z,color=colors)
ax.set_xlim(0,2e8)
ax.set_zlim(0,1e9)
ax.set_xlabel('budget',size=15)
ax.set_ylabel('popularity',size=15)
ax.set_zlabel('revenue',size=15)
fig.savefig('cluster3')
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628174884092-45774d7b-8da8-4de5-a709-ae39e264435d.png#clientId=ua71c4f6f-d54a-4&from=paste&height=401&id=u316b9836&originHeight=1202&originWidth=1614&originalType=binary&ratio=1&size=672955&status=done&style=shadow&taskId=ud9fbbc26-9c28-40f1-99c0-414e7de3805&width=538)<br />可以看到不同类别占据特征空间中的不同位置。
