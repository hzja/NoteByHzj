数据分析 Python <br />dython是python中的一款数据建模库。尽管已经有了scikit-learn、statsmodels、seaborn等非常优秀的数据建模库，但实际数据分析过程中常用到的一些功能场景仍然需要编写数十行以上的代码才能实现。<br />而今天推荐的dython就是一款集成了诸多实用功能的数据建模工具库，更加高效地完成数据分析过程中的诸多任务：<br />![2021-08-20-22-51-02-213550.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471284909-26f8e044-bf44-499f-96b7-07d590981438.png#clientId=u599a1709-e3da-4&from=ui&id=u94f8c03a&originHeight=360&originWidth=1080&originalType=binary&ratio=1&size=2261&status=done&style=none&taskId=u2f857e2d-8a98-4857-a3f5-5c5bad85d01)<br />通过下面两种方式均可完成对dython的安装：
```python
pip install dython
```
或：
```python
conda install -c conda-forge dython
```
dython中目前根据功能分类划分为以下几个子模块：
<a name="fuA4D"></a>
## 「data_utils」
data_utils子模块集成了一些基础性的数据探索性分析相关的API，如`identify_columns_with_na()`可用于快速检查数据集中的缺失值情况：
```python
>> df = pd.DataFrame({'col1': ['a', np.nan, 'a', 'a'], 'col2': [3, np.nan, 2, np.nan], 'col3': [1., 2., 3., 4.]})
>> identify_columns_with_na(df)
  column  na_count
1   col2         2
0   col1         1
```
`identify_columns_by_type()`可快速选择数据集中具有指定数据类型的字段：
```python
>> df = pd.DataFrame({'col1': ['a', 'b', 'c', 'a'], 'col2': [3, 4, 2, 1], 'col3': [1., 2., 3., 4.]})
>> identify_columns_by_type(df, include=['int64', 'float64'])
['col2', 'col3']
```
`one_hot_encode()`可快速对数组进行「独热编码」：
```python
>> one_hot_encode([1,0,5])
[[0. 1. 0. 0. 0. 0.]
 [1. 0. 0. 0. 0. 0.]
 [0. 0. 0. 0. 0. 1.]]
```
`split_hist()`则可以快速绘制分组直方图，帮助用户快速探索数据集特征分布：
```python
import pandas as pd
from sklearn import datasets
from dython.data_utils import split_hist

# Load data and convert to DataFrame
data = datasets.load_breast_cancer()
df = pd.DataFrame(data=data.data, columns=data.feature_names)
df['malignant'] = [not bool(x) for x in data.target]

# Plot histogram
split_hist(df, 'mean radius', split_by='malignant', bins=20, figsize=(15,7))
```
![2021-08-20-22-51-02-317519.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471299439-45dee114-f6a4-40d3-ae5a-4114fe71229e.png#clientId=u599a1709-e3da-4&from=ui&id=u12d6ffa2&originHeight=439&originWidth=885&originalType=binary&ratio=1&size=11465&status=done&style=shadow&taskId=u5708b986-e97c-431b-8f92-079d3f53b79)
<a name="bkKt7"></a>
## 「nominal」
nominal子模块包含了一些进阶的特征相关性度量功能，例如其中的associations()可以自适应由连续型和类别型特征混合的数据集，并自动计算出相应的Pearson、Cramer's V、Theil's U、条件熵等多样化的系数；`cluster_correlations()`可以绘制出基于层次聚类的相关系数矩阵图等实用功能：<br />![2021-08-20-22-51-02-498654.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471299463-8aa78925-c5ac-4b1b-85cb-0552a52f8de4.png#clientId=u599a1709-e3da-4&from=ui&id=QZOnu&originHeight=473&originWidth=630&originalType=binary&ratio=1&size=47987&status=done&style=shadow&taskId=u5af944f4-1610-47e2-a000-4074fc2e28f)<br />![2021-08-20-22-51-02-668656.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471299579-66f70419-5d0c-4b55-b721-140fe98472d1.png#clientId=u599a1709-e3da-4&from=ui&id=lYQ3K&originHeight=878&originWidth=878&originalType=binary&ratio=1&size=225910&status=done&style=shadow&taskId=u0b30e36d-47f8-4275-a8d3-2820edec549)
<a name="SaHAR"></a>
## 「model_utils」
`model_utils`子模块包含了诸多对机器学习模型进行性能评估的工具，如`ks_abc()`：
```python
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from dython.model_utils import ks_abc

# Load and split data
data = datasets.load_breast_cancer()
X_train, X_test, y_train, y_test = train_test_split(data.data, data.target, test_size=.5, random_state=0)

# Train model and predict
model = LogisticRegression(solver='liblinear')
model.fit(X_train, y_train)
y_pred = model.predict_proba(X_test)

# Perform KS test and compute area between curves
ks_abc(y_test, y_pred[:,1])
```
![2021-08-20-22-51-02-801681.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471374196-b0ac39e4-90c1-4c23-bb1f-026ae3f7afc6.png#clientId=u599a1709-e3da-4&from=ui&id=uc5ae9c49&originHeight=1094&originWidth=1080&originalType=binary&ratio=1&size=127962&status=done&style=shadow&taskId=u48acc8fc-4aea-4805-9c0d-1522bec1255)<br />`metric_graph()`：
```python
import numpy as np
from sklearn import svm, datasets
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import label_binarize
from sklearn.multiclass import OneVsRestClassifier
from dython.model_utils import metric_graph

# Load data
iris = datasets.load_iris()
X = iris.data
y = label_binarize(iris.target, classes=[0, 1, 2])

# Add noisy features
random_state = np.random.RandomState(4)
n_samples, n_features = X.shape
X = np.c_[X, random_state.randn(n_samples, 200 * n_features)]

# Train a model
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=.5, random_state=0)
classifier = OneVsRestClassifier(svm.SVC(kernel='linear', probability=True, random_state=0))

# Predict
y_score = classifier.fit(X_train, y_train).predict_proba(X_test)

# Plot ROC graphs
metric_graph(y_test, y_score, 'pr', class_names=iris.target_names)
```
![2021-08-20-22-51-02-916654.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471388956-020ac179-0120-4227-9e33-c524ce53adc3.png#clientId=u599a1709-e3da-4&from=ui&id=uce1dcc2e&originHeight=483&originWidth=638&originalType=binary&ratio=1&size=65793&status=done&style=shadow&taskId=u41f648aa-e616-42c2-8914-002749fcd29)
```python
import numpy as np
from sklearn import svm, datasets
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import label_binarize
from sklearn.multiclass import OneVsRestClassifier
from dython.model_utils import metric_graph

# Load data
iris = datasets.load_iris()
X = iris.data
y = label_binarize(iris.target, classes=[0, 1, 2])

# Add noisy features
random_state = np.random.RandomState(4)
n_samples, n_features = X.shape
X = np.c_[X, random_state.randn(n_samples, 200 * n_features)]

# Train a model
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=.5, random_state=0)
classifier = OneVsRestClassifier(svm.SVC(kernel='linear', probability=True, random_state=0))

# Predict
y_score = classifier.fit(X_train, y_train).predict_proba(X_test)

# Plot ROC graphs
metric_graph(y_test, y_score, 'roc', class_names=iris.target_names)
```
![2021-08-20-22-51-03-043660.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471406527-cb13e1c9-b2e6-4144-8c5d-03d66d30d51e.png#clientId=u599a1709-e3da-4&from=ui&id=u44cc0248&originHeight=476&originWidth=638&originalType=binary&ratio=1&size=71550&status=done&style=shadow&taskId=u4a1babd6-bd1a-447d-9330-a59029690f1)
<a name="QMi6w"></a>
## 「sampling」
sampling子模块则包含了`boltzmann_sampling()`和`weighted_sampling()`两种数据采样方法，简化数据建模流程。<br />![2021-08-20-22-51-03-187653.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629471418297-cefca897-8703-4bf2-a098-70adff4dec56.png#clientId=u599a1709-e3da-4&from=ui&id=ud56d8f22&originHeight=1005&originWidth=686&originalType=binary&ratio=1&size=101458&status=done&style=shadow&taskId=u15daf70b-a95c-49dd-a755-f0366553b21)<br />dython作为一个处于快速开发迭代过程的Python库，陆续会有更多的实用功能引入，感兴趣的朋友们可以前往[https://github.com/shakedzy/dython](https://github.com/shakedzy/dython)查看更多内容或对此项目保持关注。
