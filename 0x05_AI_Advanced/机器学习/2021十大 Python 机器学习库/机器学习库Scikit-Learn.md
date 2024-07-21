AI<br />Scikit-learn是一个非常知名的Python机器学习库，它广泛地用于统计分析和机器学习建模等数据科学领域。

- 建模无敌：用户通过scikit-learn能够实现各种监督和非监督学习的模型
- 功能多样：同时使用sklearn还能够进行数据的预处理、特征工程、数据集切分、模型评估等工作
- 数据丰富：内置丰富的数据集，比如：泰坦尼克、鸢尾花等，数据不再愁啦

这里通过简明快要的方式来介绍scikit-learn的使用，更多详细内容请参考官网：

1. 内置数据集使用
2. 数据集切分
3. 数据归一化和标准化
4. 类型编码
5. 建模6步曲

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491510-78ed9ccb-54bf-470d-b37e-82f69782c7f6.webp#clientId=ub1e48f70-300d-4&from=paste&id=ue8d5819f&originHeight=234&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3f895c47-1598-46e9-be14-293633ff7d1&title=)
<a name="Ehfde"></a>
## 一、Scikit-learn使用神图
下面这张图是官网提供的，从样本量的大小开始，分为回归、分类、聚类、数据降维共4个方面总结了scikit-learn的使用：[https://scikit-learn.org/stable/tutorial/machine_learning_map/index.html](https://scikit-learn.org/stable/tutorial/machine_learning_map/index.html)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491414-595dc58d-8612-44a0-9dcb-c067dd0f21b6.webp#clientId=ub1e48f70-300d-4&from=paste&id=u69aaf363&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uad3977d7-84eb-410f-871c-2732b66c62d&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491522-d3daea20-28eb-42f2-a69c-f93c84c87d34.webp#clientId=ub1e48f70-300d-4&from=paste&id=uf6dcc7cb&originHeight=823&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubdc3513b-3670-40f6-8cf1-2b3f47a29d4&title=)
<a name="rXdMp"></a>
## 二、安装
关于安装scikit-learn，建议通过使用anaconda来进行安装，不用担心各种配置和环境问题。当然也可以直接pip来安装：
```bash
pip install scikit-learn
```
<a name="zeAix"></a>
## 三、数据集生成
sklearn内置了一些优秀的数据集，比如：Iris数据、房价数据、泰坦尼克数据等。
```python
import pandas as pd
import numpy as np

import sklearn 
from sklearn import datasets  # 导入数据集
```
<a name="ODtAW"></a>
### 分类数据-iris数据
```python
# iris数据
iris = datasets.load_iris()
type(iris)

sklearn.utils.Bunch
```
iris数据到底是什么样子？每个内置的数据都存在很多的信息<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491494-ff4f4885-eab0-4b04-a979-e8a636802a57.webp#clientId=ub1e48f70-300d-4&from=paste&id=u3146b91c&originHeight=806&originWidth=994&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u35708b7e-38cf-4475-a2df-b33d2cb665b&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491494-b549e2c0-068d-4229-85bb-bb9707f5a182.webp#clientId=ub1e48f70-300d-4&from=paste&id=uc19bcc59&originHeight=487&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0a805fc0-ec3d-478f-bedb-f769c530f6f&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491870-c63ddeab-62a2-4d68-a171-b5cccc0a3f15.webp#clientId=ub1e48f70-300d-4&from=paste&id=ucefc2719&originHeight=189&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u390ad6c1-6eff-4afe-a732-a18e1f28f5d&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491957-88b131e7-50b5-4266-a45f-d1617611b985.webp#clientId=ub1e48f70-300d-4&from=paste&id=uaf0f8b81&originHeight=623&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9888d130-858e-4cfa-b665-718ef155ef1&title=)<br />可以将上面的数据生成想看到的DataFrame，还可以添加因变量：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491880-b35eff85-54eb-4e8d-bc67-fc23916adf27.webp#clientId=ub1e48f70-300d-4&from=paste&id=u79772c02&originHeight=780&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u541f8b02-d34f-425f-8ea1-48121602040&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603492053-6c63ddc5-12fb-405c-bf9b-c52714614a85.webp#clientId=ub1e48f70-300d-4&from=paste&id=u2ad76cf5&originHeight=674&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u132e9842-95d5-4a6a-87c5-d639aa4344f&title=)
<a name="hY4Kx"></a>
### 回归数据-波士顿房价
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603491983-c78e1ed7-29d6-4009-9649-b5540b3a7fe7.webp#clientId=ub1e48f70-300d-4&from=paste&id=u071917af&originHeight=597&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc58d6438-8e07-43bf-ae0f-82d66ab77dc&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603492176-c13979a7-2783-4f62-9050-88d21ed4ffd6.webp#clientId=ub1e48f70-300d-4&from=paste&id=u29d0ea5e&originHeight=470&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua771f270-66f1-4015-99d2-07f5fd9d8fb&title=)<br />重点关注的属性：

- data
- target、target_names
- feature_names
- filename

同样可以生成DataFrame：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603492231-da7baef3-6276-4226-a2dd-6a66a2fc4f68.webp#clientId=ub1e48f70-300d-4&from=paste&id=ud4a5106a&originHeight=594&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucb174970-c75c-4aa4-aabd-595dcc8a7b0&title=)
<a name="Uq4J0"></a>
### 三种方式生成数据
<a name="taIcM"></a>
#### 方式1
```python
#调用模块
from sklearn.datasets import load_iris
data = load_iris()

#导入数据和标签
data_X = data.data
data_y = data.target 
```
<a name="oHBja"></a>
#### 方式2
```python
from sklearn import datasets
loaded_data = datasets.load_iris()  # 导入数据集的属性

#导入样本数据
data_X = loaded_data.data
# 导入标签
data_y = loaded_data.target
```
<a name="Ykean"></a>
#### 方式3
```python
# 直接返回
data_X, data_y = load_iris(return_X_y=True)
```
<a name="R1hwF"></a>
### 数据集使用汇总
```python
from sklearn import datasets  # 导入库

boston = datasets.load_boston()  # 导入波士顿房价数据
print(boston.keys())  # 查看键(属性)     ['data','target','feature_names','DESCR', 'filename'] 
print(boston.data.shape,boston.target.shape)  # 查看数据的形状 
print(boston.feature_names)  # 查看有哪些特征 
print(boston.DESCR)  # described 数据集描述信息 
print(boston.filename)  # 文件路径 
```
<a name="K0zZm"></a>
## 四、数据切分
```python
# 导入模块
from sklearn.model_selection import train_test_split
# 划分为训练集和测试集数据
X_train, X_test, y_train, y_test = train_test_split(
  data_X, 
  data_y, 
  test_size=0.2,
  random_state=111
)

# 150*0.8=120
len(X_train)
```
<a name="jX42F"></a>
## 五、数据标准化和归一化
```python
from sklearn.preprocessing import StandardScaler  # 标准化
from sklearn.preprocessing import MinMaxScaler  # 归一化

# 标准化
ss = StandardScaler()
X_scaled = ss.fit_transform(X_train)  # 传入待标准化的数据

# 归一化
mm = MinMaxScaler()
X_scaled = mm.fit_transform(X_train)
```
<a name="lc4MJ"></a>
## 六、类型编码
来自官网案例：[https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html](https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html)
<a name="eOYXG"></a>
### 对数字编码
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603492432-404ed726-bbdc-4d29-ba7c-f21c48e37fe8.webp#clientId=ub1e48f70-300d-4&from=paste&id=u33ee0abd&originHeight=605&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9b0abc54-f40f-4cf8-ae9f-732dbeac5c1&title=)
<a name="dUPBf"></a>
### 对字符串编码
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642603492313-7a7c92ff-a568-460b-8e9d-b166b9116786.webp#clientId=ub1e48f70-300d-4&from=paste&id=u25cec61e&originHeight=441&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucf33b8f1-2883-4a35-be08-1a58d700a75&title=)
<a name="w3raI"></a>
## 七、建模案例
<a name="nzY9w"></a>
### 导入模块
```python
from sklearn.neighbors import KNeighborsClassifier, NeighborhoodComponentsAnalysis  # 模型
from sklearn.datasets import load_iris  # 导入数据
from sklearn.model_selection import train_test_split  # 切分数据
from sklearn.model_selection import GridSearchCV  # 网格搜索
from sklearn.pipeline import Pipeline  # 流水线管道操作

from sklearn.metrics import accuracy_score  # 得分验证
```
<a name="ATS0O"></a>
### 模型实例化
```python
# 模型实例化
knn = KNeighborsClassifier(n_neighbors=5)
```
<a name="IFReq"></a>
### 训练模型
```python
knn.fit(X_train, y_train)

KNeighborsClassifier()
```
<a name="ARDuI"></a>
### 测试集预测
```python
y_pred = knn.predict(X_test)
y_pred  # 基于模型的预测值

array([0, 0, 2, 2, 1, 0, 0, 2, 2, 1, 2, 0, 1, 2, 2, 0, 2, 1, 0, 2, 1, 2,
       1, 1, 2, 0, 0, 2, 0, 2])
```
<a name="WJPrh"></a>
### 得分验证
模型得分验证的两种方式：
```python
knn.score(X_test,y_test)

0.9333333333333333

accuracy_score(y_pred,y_test)

0.9333333333333333
```
<a name="O8cel"></a>
### 网格搜索
<a name="Ypp2Z"></a>
#### 如何搜索参数
```python
from sklearn.model_selection import GridSearchCV

# 搜索的参数
knn_paras = {"n_neighbors":[1,3,5,7]}
# 默认的模型
knn_grid = KNeighborsClassifier()

# 网格搜索的实例化对象
grid_search = GridSearchCV(
 knn_grid, 
 knn_paras, 
 cv=10  # 10折交叉验证
)
grid_search.fit(X_train, y_train)

GridSearchCV(cv=10, estimator=KNeighborsClassifier(),
             param_grid={'n_neighbors': [1, 3, 5, 7]})

# 通过搜索找到的最好参数值
grid_search.best_estimator_ 

KNeighborsClassifier(n_neighbors=7)

grid_search.best_params_

Out[42]:
{'n_neighbors': 7}

grid_search.best_score_

0.975
```
<a name="WQOc3"></a>
#### 基于搜索结果建模
```python
knn1 = KNeighborsClassifier(n_neighbors=7)

knn1.fit(X_train, y_train)

KNeighborsClassifier(n_neighbors=7)
```
通过下面的结果可以看到：网格搜索之后的建模效果是优于未使用网格搜索的模型：
```python
y_pred_1 = knn1.predict(X_test)

knn1.score(X_test,y_test)

1.0

accuracy_score(y_pred_1,y_test)

1.0
```
