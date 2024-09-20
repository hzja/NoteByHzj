机器学习CatBoost<br />**CatBoost（categorical boosting）是一种能够很好地处理类别型特征的梯度提升算法库。本文中，对 CatBoost 基本原理及应用实例做个详细介绍。后面小猴子还将针对其中几个重要特性做专门介绍，如 CatBoost 对类别型特征处理、特征选择、文本特征处理、超参数调整以及多标签目标处理！**
<a name="VPVvX"></a>
## 梯度提升概述
要理解 boosting，首先理解集成学习，为了获得更好的预测性能，集成学习结合多个模型（弱学习器）的预测结果。它的策略就是大力出奇迹，因为弱学习器的有效组合可以生成更准确和更鲁棒的模型。集成学习方法分为三大类：

- **Bagging**：该技术使用随机数据子集并行构建不同的模型，并聚合所有预测变量的预测结果。
- **Boosting**：这种技术是可迭代的、顺序进行的和自适应的，因为每个预测器都是针对上一个模型的错误进行修正。
- **Stacking**：这是一种元学习技术，涉及结合来自多种机器学习算法的预测，例如 bagging 和 boosting。
<a name="nKqX2"></a>
## 什么是 CatBoost
CatBoost（categorical boosting）是 Yandex 开源的机器学习算法。它可以与深度学习框架轻松集成。它可以处理多种数据类型，以帮助解决企业今天面临的各种问题。CatBoost 和 XGBoost、LightGBM 并称为 GBDT 的三大主流神器，都是在 GBDT 算法框架下的一种改进实现。XGBoost 被广泛的应用于工业界，LightGBM 有效的提升了 GBDT 的计算效率，而 Yandex 的 CatBoost 号称是比 XGBoost 和 LightGBM 在算法准确率等方面表现更为优秀的算法。<br />CatBoost 是一种基于 **对称决策树**（oblivious trees）为基学习器实现的参数较少、支持类别型变量和高准确性的GBDT框架，主要解决的痛点是高效合理地处理类别型特征，这一点从它的名字中可以看出来，CatBoost 是由 Categorical 和 Boosting 组成。此外，CatBoost 还解决了梯度偏差（Gradient Bias）以及预测偏移（Prediction shift）的问题，从而减少过拟合的发生，进而提高算法的准确性和泛化能力。<br />此外，CatBoost 梯度提升算法库中的学习算法基于 GPU 实现，打分算法基于 CPU 实现。
<a name="GE2z3"></a>
## CatBoost 的主要特点
CatBoost 优于同类产品的一些关键特性：
<a name="aeign"></a>
### 1、对称树
与 XGBoost 和 LightGBM 不同，CatBoost 构建对称（平衡）树。在每一步中，前一棵树的叶子都使用相同的条件进行拆分。选择损失最低的特征分割对并将其用于所有级别的节点。这种平衡的树结构有助于高效的 CPU 实现，减少预测时间，模型结构可作为正则化以防止过度拟合。<br />在对称决策树中，只使用一个特性来构建每个树级别上的所有分支。使用图示方法更加清晰地观察三种类型的拆分："FloatFeature"、"OneHotFeature" 和 "OnlineCtr"。
<a name="ukpON"></a>
#### **FloatFeature**
模型没有类别型特征时，在可视化的树中只有 "FloatFeature" 节点。"FloatFeature" 拆分对应的节点包含特征索引和边界值，用于拆分对象。
```python
boston = load_boston()
y = boston['target']
X = boston['data']
pool = catboost.Pool(data=X, label=y)
model = CatBoostRegressor(depth=2, verbose=False, iterations=1).fit(X, y)
model.plot_tree(tree_idx=0,
              # pool=pool, )
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866527158-88c47072-5991-42fc-ae6f-37082ab3e4ae.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u927876e8&originHeight=471&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucb401e30-d7ef-42ce-9f8e-abff0c81478&title=)<br />在这个例子中，深度为0的节点表示对象被它们的第0个带边界值的特征分割。类似地，深度1的节点通过其具有边界值的第二个特征来分割对象。
<a name="GvyLj"></a>
#### **OneHotFeature**
```python
titanic_df = titanic()

X = titanic_df[0].drop('Survived',axis=1)
y = titanic_df[0].Survived
# 分类变量的缺失值用"NAN"填充，代码略
pool = Pool(X, y, cat_features=cat_features_index, feature_names=list(X.columns))
model = CatBoostClassifier(
    max_depth=2, verbose=False, max_ctr_complexity=1, random_seed=42, iterations=2).fit(pool)
model.plot_tree(
    tree_idx=0,
    pool=pool # 对于一个需要使用独热编码的特征，"pool" 是一个必须的参数
)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866527185-3c3f35f3-6080-4618-998d-ecb54683b199.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u0f7ae94b&originHeight=454&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u41c27cef-2977-4c89-a7ae-f12cb54f043&title=)<br />第一棵树只包含一个由"OneHotFeature"特征产生的分裂。这种分割将"Sex=female"的对象放在左边，而"other"的对象放在右边。
<a name="aLoYH"></a>
#### **OnlineCtr**
```python
model.plot_tree(tree_idx=1, pool=pool)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866527097-861f5aa5-d649-4336-b706-c7e9bd362fa5.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u96ac45d8&originHeight=475&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u24ea12ae-7fd9-48f3-befd-8a7d9a14b22&title=)
<a name="JPR1X"></a>
### 2、Ordered Boosting
经典提升算法存在预测偏移的问题，容易在小的/嘈杂的数据集上过度拟合。在计算数据实例的梯度估计时，这些算法使用与构建模型相同的数据实例，因此没有机会遇到看不见的数据。<br />另一方面，CatBoost 使用排序提升的概念，这是一种置换驱动的方法，在数据子集上训练模型，同时在另一个子集上计算残差，从而防止目标泄漏和过度拟合。
<a name="DxNRN"></a>
### 3、鲁棒性
它减少了对大量超参数调整的需求，并降低了过度拟合的机会，这也导致了更通用的模型。虽然，CatBoost 有多个参数需要调整，它包含树的数量、学习率、正则化、树深度、折叠大小、装袋温度等参数。您可以在此处阅读所有这些参数。
<a name="CU2nN"></a>
### 4、原生特征支持,易于使用
CatBoost 支持数字、分类或文本的各种特征，节省了预处理的时间和精力。可以从命令行使用 CatBoost，使用 Python 和 R 的用户友好 API。
<a name="yrx9D"></a>
## CatBoost 的基本使用
<a name="soYzR"></a>
### 导入基本数据
使用 CatBoost 库里自带的经典数据集 titanic。
```python
from catboost.datasets import titanic
import numpy as np
train_df, test_df = titanic()
# 用一些超出分布范围的数字来填充缺失值
train_df.fillna(-999, inplace=True)
test_df.fillna(-999, inplace=True)
# 拆分特征变量及标签变量
X = train_df.drop('Survived', axis=1)
y = train_df.Survived
# 划分训练集和测试集
from sklearn.model_selection import train_test_split
X_train, X_validation, y_train, y_validation = train_test_split(X, y, train_size=0.75, random_state=42)
X_test = test_df
```
Titanic 数据集中的特征变量具有不同的类型—有些是数值类型，有些是分类类型，甚至有些只是字符串类型，通常应该以某种特定的方式进行处理(例如使用词袋表示进行编码)。但这里可以将这些字符串特征视为分类特征——所有繁重的工作都是在 CatBoost 中完成的。
<a name="Pqpnp"></a>
### 创建一个baseline模型
首先从一个基础baseline模型开始，认识如何使用catboost进行预测目标。
```python
# !pip install catboost
from catboost import CatBoostClassifier, Pool, metrics, cv
from sklearn.metrics import accuracy_score

model = CatBoostClassifier(
    custom_loss=[metrics.Accuracy()], # 该指标可以计算logloss，并且在该规模的数据集上更加光滑
    random_seed=42,
    logging_level='Silent'
)
# 模型训练
model.fit(
    X_train, y_train,
    cat_features=categorical_features_indices,
    eval_set=(X_validation, y_validation),
#     logging_level='Verbose',  # you can uncomment this for text output
    plot=True
);
```
可以通过详细输出或使用漂亮的绘图来观察模型学习，使用CatBoost自带的可交互模型过程可视化，查看模型的学习过程。<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866527201-b879e614-ee05-453e-87ce-0235828d15e4.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u4c3a84de&originHeight=492&originWidth=1006&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua7f73e05-8670-4ae6-ba41-1dd301f225f&title=)
<a name="b1Nec"></a>
### 特征变量统计
<a name="zKSUB"></a>
#### **Float feature**
```python
feature = 'Fare'
res = model.calc_feature_statistics(
      X_train, y_train, feature, plot=True)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866527129-07cbb0e1-0fc6-467c-8342-059974225ef7.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u683d2f21&originHeight=452&originWidth=1008&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2e274ba0-70bf-41d8-8c06-11a016cef4f&title=)
<a name="DNvRs"></a>
#### **One-hot feature**
```python
feature = 'Sex'
res = model.calc_feature_statistics(X_train, y_train, feature, plot=True)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866527455-0208e850-4062-412e-9e60-3b3f51ca7a7a.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u5e01af4b&originHeight=452&originWidth=1008&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udbbe1ef3-b50c-4918-8e69-cfc5807acc6&title=)
<a name="tAFAi"></a>
### 模型交叉验证
验证模型是否是性能最佳的，使用交叉验证，可能甚至会得到更好的模型。
```python
cv_params = model.get_params()
cv_params.update({
    'loss_function': metrics.Logloss()
})
cv_data = cv(
    Pool(X, y, cat_features=categorical_features_indices),
    cv_params,
    plot=True
)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866527613-6423c59b-5ad2-4835-8aa5-3f40b2297fdd.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u7edc8c6a&originHeight=500&originWidth=1008&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u297b06b8-814c-459a-9c36-45182161484&title=)<br />现在得到了每个 boosting 步骤 3folds 平均的损失函数值，这提供了更准确的模型性能估计。
```python
print('Best validation accuracy score: {:.2f}±{:.2f} on step {}'.format(
    np.max(cv_data['test-Accuracy-mean']),
    cv_data['test-Accuracy-std'][np.argmax(cv_data['test-Accuracy-mean'])],
    np.argmax(cv_data['test-Accuracy-mean'])
))

print('Precise validation accuracy score: {}'.format(np.max(cv_data['test-Accuracy-mean'])))
```
```python
Best validation accuracy score: 
0.83±0.02 on step 355
Precise validation accuracy score: 
0.8294051627384961
```
如上所示，最初对单次验证的性能估计并不是特别理想，而经过交叉验证后会有所提升。
<a name="rJ9GH"></a>
### 模型应用
模型训练结束后，可以保存模型，以供预测使用。
```python
predictions = model.predict(X_test)
predictions_probs = model.predict_proba(X_test)
print(predictions[:10])
print(predictions_probs[:10])
```
```python
[0 0 0 0 1 0 1 0 1 0]
[[0.85473931 0.14526069]
 [0.76313031 0.23686969]
 [0.88972889 0.11027111]
 [0.87876173 0.12123827]
 [0.3611047  0.6388953 ]
 [0.90513381 0.09486619]
 [0.33434185 0.66565815]
 [0.78468564 0.21531436]
 [0.39429048 0.60570952]
 [0.94047549 0.05952451]]
```
<a name="qQxqm"></a>
## CatBoost 应用案例
出于演示目的，同样使用 catboost 自带的 amazon 数据集。
```python
import pandas as pd
import os
import numpy as np
np.set_printoptions(precision=4)
import catboost
from catboost import *
from catboost import datasets

(train_df, test_df) = catboost.datasets.amazon()
train_df.head()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866527697-12724fff-7b81-4c82-992f-e6a450d77619.png#clientId=u0f7e6450-c0a1-4&from=paste&id=ueb4ae06f&originHeight=192&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5bcc6267-e4a9-48cb-9ce7-3fcb055c000&title=)
<a name="lSZp4"></a>
### 数据预处理
<a name="GZHrk"></a>
#### 数据标签提取
```python
y = train_df.ACTION
X = train_df.drop('ACTION', axis=1)
```
<a name="Fs6YQ"></a>
#### 检查数据集中标签平衡性
```python
print('Labels: {}'.format(set(y)))
print('Zero count = {}, One count = {}'.format(len(y) - sum(y), sum(y)))
```
```python
abels: {0, 1}
Zero count = 1897, One count = 30872
```
<a name="g65zk"></a>
#### 保存数据
```python
dataset_dir = './amazon'
if not os.path.exists(dataset_dir):
    os.makedirs(dataset_dir)
    
train_df.to_csv(
    os.path.join(dataset_dir, 'train.csv'),
    index=False, sep=',', header=True
)
test_df.to_csv(
    os.path.join(dataset_dir, 'test.csv'),
    index=False, sep=',', header=True
)
```
<a name="WuHbx"></a>
#### 创建 Pool 类
```python
from catboost.utils import create_cd
feature_names = dict()
for column, name in enumerate(train_df):
    if column == 0:
        continue
    feature_names[column - 1] = name
    
create_cd(
    label=0, 
    cat_features=list(range(1, train_df.columns.shape[0])),
    feature_names=feature_names,
    output_path=os.path.join(dataset_dir, 'train.cd')
)
```
```python
!cat amazon/train.cd

0	Label	
1	Categ	RESOURCE
2	Categ	MGR_ID
3	Categ	ROLE_ROLLUP_1
4	Categ	ROLE_ROLLUP_2
5	Categ	ROLE_DEPTNAME
6	Categ	ROLE_TITLE
7	Categ	ROLE_FAMILY_DESC
8	Categ	ROLE_FAMILY
9	Categ	ROLE_CODE
```
这里展示了几种创建Pool的不同方法，实际中选中其中一种创建方法即可。
```python
pool1 = Pool(data=X, label=y, cat_features=cat_features)
pool2 = Pool(
    data=os.path.join(dataset_dir, 'train.csv'), 
    delimiter=',', 
    column_description=os.path.join(dataset_dir, 'train.cd'),
    has_header=True
)
pool3 = Pool(data=X, cat_features=cat_features)

# 创建Pool的最快方法是从numpy矩阵创建它。
# 如果你想要快速的预测或者以最快的方式在python中加载数据，就应该使用这种方式。

X_prepared = X.values.astype(str).astype(object)
# 对于FeaturesData类，类别特性必须具有str类型

pool4 = Pool(
    data=FeaturesData(
        cat_feature_data=X_prepared,
        cat_feature_names=list(X)
    ),
    label=y.values
)

print('Dataset shape')
print('dataset 1:' + str(pool1.shape) +
      '\ndataset 2:' + str(pool2.shape) + 
      '\ndataset 3:' + str(pool3.shape) +
      '\ndataset 4:' + str(pool4.shape))

print('\n')
print('Column names')
print('dataset 1:')
print(pool1.get_feature_names()) 
print('\ndataset 2:')
print(pool2.get_feature_names())
print('\ndataset 3:')
print(pool3.get_feature_names())
print('\ndataset 4:')
print(pool4.get_feature_names())
```
```python
Dataset shape
dataset 1:(32769, 9)
dataset 2:(32769, 9)
dataset 3:(32769, 9)
dataset 4:(32769, 9)

Column names
dataset 1:
['RESOURCE', 'MGR_ID', 'ROLE_ROLLUP_1',
'ROLE_ROLLUP_2', 'ROLE_DEPTNAME', 
'ROLE_TITLE', 'ROLE_FAMILY_DESC',
'ROLE_FAMILY', 'ROLE_CODE']

dataset 2:
['RESOURCE', 'MGR_ID', 'ROLE_ROLLUP_1',
'ROLE_ROLLUP_2', 'ROLE_DEPTNAME', 
'ROLE_TITLE', 'ROLE_FAMILY_DESC',
'ROLE_FAMILY', 'ROLE_CODE']

dataset 3:
['RESOURCE', 'MGR_ID', 'ROLE_ROLLUP_1',
'ROLE_ROLLUP_2', 'ROLE_DEPTNAME', 
'ROLE_TITLE', 'ROLE_FAMILY_DESC',
'ROLE_FAMILY', 'ROLE_CODE']

dataset 4:
['RESOURCE', 'MGR_ID', 'ROLE_ROLLUP_1',
'ROLE_ROLLUP_2', 'ROLE_DEPTNAME', 
'ROLE_TITLE', 'ROLE_FAMILY_DESC',
'ROLE_FAMILY', 'ROLE_CODE']
```
<a name="skw2a"></a>
### 拆分训练集和测试集
这一步大家应该比较熟悉，就不做过多的介绍。
```python
from sklearn.model_selection import train_test_split
X_train, X_validation, y_train, y_validation = train_test_split(
     X, y, train_size=0.8, random_state=1234)
```
<a name="OdYFc"></a>
### 选择目标函数
对于二分类数据集，目标函数可以选择Logloss，如果要预测目标标签的概率值，推荐使用交叉熵CrossEntropy。
```python
from catboost import CatBoostClassifier
model = CatBoostClassifier(
    iterations=5,
    learning_rate=0.1,
    # loss_function='CrossEntropy'
)
model.fit(
    X_train, y_train,
    cat_features=cat_features,
    eval_set=(X_validation, y_validation),
    verbose=False
)
print('Model is fitted: ' + str(model.is_fitted()))
print('Model params:')
print(model.get_params())
```
```python
Model is fitted: True
Model params:
{'iterations': 5, 'learning_rate': 0.1}
```
<a name="AMFkd"></a>
### 训练模型
模型训练与通常sklearn模型训练差异不大，先实例化模型 model，然后直接 `fit` 训练即可。
```python
from catboost import CatBoostClassifier
model = CatBoostClassifier(
    iterations=15,
#     verbose=5,
)
model.fit(
    X_train, y_train,
    cat_features=cat_features,
    eval_set=(X_validation, y_validation),
)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866528393-c8cc0e8c-e73c-42b4-a147-f3f0e863a198.png#clientId=u0f7e6450-c0a1-4&from=paste&id=ufbb4cc50&originHeight=445&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubf6eaf3c-099b-40bc-a9ff-a6e1d08f34b&title=)
<a name="cTDX2"></a>
### 评估模型
Catboost 做模型评估时，同一般模型少有区别，该模型在 `model.fit()` 时，传递给参数 eval_set 相应的验证子集，设置参数 plot 为 True，即可在训练模型的同时，用验证集评估模型，并且输出过程可视化结果，可谓是非常方便与惊艳。
```python
from catboost import CatBoostClassifier
model = CatBoostClassifier(
    iterations=50,
    random_seed=63,
    learning_rate=0.5,
    custom_loss=['AUC', 'Accuracy']
)
model.fit(
    X_train, y_train,
    cat_features=cat_features,
    eval_set=(X_validation, y_validation),
    verbose=False,
    plot=True
)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866528467-dd9f2450-9381-44e0-91d5-397690187e57.png#clientId=u0f7e6450-c0a1-4&from=paste&id=uea4bd0d8&originHeight=524&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6f6d106f-deef-47fc-ac52-3b8680944f4&title=)
<a name="qPFZz"></a>
### 模型比较
与模型评估一样，使用相同 CatBoostClassifier 分类器，仅仅设置不同的 `learning_rate`，并设置`train_dir`分别为 `'learing_rate_0.7'` 及 `'learing_rate_0.01'`。
```python
model1 = CatBoostClassifier(
    learning_rate=0.7,
    iterations=100,
    random_seed=0,
    train_dir='learing_rate_0.7'
)

model2 = CatBoostClassifier(
    learning_rate=0.01,
    iterations=100,
    random_seed=0,
    train_dir='learing_rate_0.01'
)
model1.fit(
    X_train, y_train,
    eval_set=(X_validation, y_validation),
    cat_features=cat_features,
    verbose=False
)
model2.fit(
    X_train, y_train,
    eval_set=(X_validation, y_validation),
    cat_features=cat_features,
    verbose=False
)
```
然后使用catboost的`MetricVisualizer`方法比较两个模型。该方法在单个图表上绘制有关训练、指标评估或交叉验证运行的信息。根据输入信息，一个图表可以包含有关一次或多次运行的信息。图表既可以在训练进行时实时绘制，也可以在训练结束后绘制。
```python
from catboost import MetricVisualizer
MetricVisualizer(['learing_rate_0.01', 'learing_rate_0.7']).start()
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866530627-8ba0adad-b1aa-4e56-bc93-06255144cca2.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u337aa431&originHeight=498&originWidth=1004&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u175f490d-de36-44e1-958a-2205846b844&title=)
<a name="fzWWI"></a>
### 交叉验证
在前面已经提到，使用交叉验证可以得到性能更好的模型，进而得到更好的预测结果。相对使用sklearn 中的交叉验证方法，Catboost 模型自带的交叉验证方法简单、灵活，还可以直接显示可视化交叉验证过程及结果。下面小猴子录制了动画，展示交叉验证过程。
```python
from catboost import cv
# 设置参数空间
params = {}
params['loss_function'] = 'Logloss'
params['iterations'] = 80
params['custom_loss'] = 'AUC'
params['random_seed'] = 63
params['learning_rate'] = 0.5
# 直接使用catboost中自带的cv参数。
cv_data = cv(
    params = params,
    pool = Pool(X, label=y, cat_features=cat_features), # 设置Pool类。
    fold_count=5,
    shuffle=True,
    partition_random_seed=0,
    plot=True,   # 设置可视化过程
    stratified=False, 
    verbose=False
)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866530583-84ac6378-59f1-4483-b1cf-75a43147b188.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=ufbf41f7d&originHeight=841&originWidth=922&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue276377a-5365-4f4d-ad6b-3b57851ab51&title=)<br />交叉验证过程中所有数据都记录下来并以DataFrame格式返回，可以直接查看，或后续使用，非常方便！
```python
cv_data.head()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866530677-99c6aef8-f0b2-46e2-8b88-1a33255d2266.png#clientId=u0f7e6450-c0a1-4&from=paste&id=ufced6ecc&originHeight=250&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3f752470-43c8-42a4-b405-80957cdea04&title=)<br />其实，只关心最佳得分，使用如下方法可以轻松获得：
```python
best_value = np.min(cv_data['test-Logloss-mean'])
best_iter = np.argmin(cv_data['test-Logloss-mean'])

print('Best validation Logloss score, not stratified: {:.4f}±{:.4f} on step {}'.format(
    best_value,
    cv_data['test-Logloss-std'][best_iter],
    best_iter)
)
```
```python
Best validation Logloss score,
not stratified: 0.1581±0.0104 on step 52
```
<a name="MFQwi"></a>
### 过拟合检验
在创建CatBoostClassifier实例时，设置参数`early_stopping_rounds=20`（根据实际情况设置），模型可以在 `early_stopping_rounds` 所设置的迭代轮数内寻找模型效果最好的，这个模型效果评价指标可以通过`eval_metric`设置，默认 Logloss，也可以设置为"AUC"。还可以通过设置`custom_metric`参数，使用自定义评价指标函数。
```python
model_with_early_stop = CatBoostClassifier(
    eval_metric='AUC',
    iterations=200,
    random_seed=63,
    learning_rate=0.5,
    early_stopping_rounds=20
)
model_with_early_stop.fit(
    X_train, y_train,
    cat_features=cat_features,
    eval_set=(X_validation, y_validation),
    verbose=False,
    plot=True
)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866530924-1fffe92f-227c-4327-aad0-7a78528eb6f6.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u67082d7f&originHeight=490&originWidth=812&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7662f04e-7dab-4adc-aca8-9a3ad251ba5&title=)
```python
print(model_with_early_stop.tree_count_)
```
```python
30
```
可以使用`tree_count_`属性查看在何时停止的。
<a name="YzhKi"></a>
### 选择概率决策边界
<a name="k8TPM"></a>
#### **绘制 ROC 曲线**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866531110-cf0837ff-3379-41f4-ace8-4f29ccc10b82.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u4b7b2037&originHeight=458&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u694c8fef-01a8-4f88-8995-ba2bb46a03b&title=)<br />首先使用catboost的工具函数`get_roc_curve`获取到在验证池中的数据fpr 和  tpr 值，然后将其输入到 sklearn 中的 auc 函数中，计算得到 roc_auc 面积大小。为了更加直观，绘制如下曲线。
```python
from catboost.utils import get_roc_curve
import sklearn
from sklearn import metrics

eval_pool = Pool(X_validation, y_validation, cat_features=cat_features)
curve = get_roc_curve(model, eval_pool)
(fpr, tpr, thresholds) = curve
roc_auc = sklearn.metrics.auc(fpr, tpr)

lw = 2
plt.plot(fpr, tpr, color='darkorange',
         lw=lw, label='ROC curve (area = %0.2f)' % roc_auc, alpha=0.5)
plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--', alpha=0.5)
```
![2022-07-15-14-34-11.439726800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866961707-034bfae8-35bd-4787-8aeb-7efa1b21c519.png#clientId=u0f7e6450-c0a1-4&from=ui&id=uf120476c&originHeight=566&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1837441&status=done&style=shadow&taskId=u2ddbdfdf-f974-4542-9a26-46a641f4352&title=)<br />除了上面上面用于ROC曲线的FPR，TPR，另外还可以绘制FPR，FNR曲线。
```python
from catboost.utils import get_fpr_curve
from catboost.utils import get_fnr_curve

(thresholds, fpr) = get_fpr_curve(curve=curve)
(thresholds, fnr) = get_fnr_curve(curve=curve)
lw = 2
plt.plot(thresholds, fpr, color='blue', lw=lw, label='FPR', alpha=0.5)
plt.plot(thresholds, fnr, color='green', lw=lw, label='FNR', alpha=0.5)
```
![2022-07-15-14-34-11.542884100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866961677-fd6ce6bc-31e5-4aa6-b7d2-227815f2d653.png#clientId=u0f7e6450-c0a1-4&from=ui&id=JM088&originHeight=549&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1782267&status=done&style=shadow&taskId=udac6be3b-2b68-4815-8f39-9fc2214def6&title=)<br />返回实现指定FNR或FPR所需的概率边界。
```python
from catboost.utils import select_threshold

print(select_threshold(model=model, data=eval_pool, FNR=0.01))
print(select_threshold(model=model, data=eval_pool, FPR=0.01))
```
```python
0.48689529945049076
0.9899713850692811
```
<a name="f4Jks"></a>
### 模型预测
CatBoost预测有四种方式，`predict`、`staged_predict`、`predict_proba` 及 `staged_predict_prob`。看下他们之间的区别。<br />首先 predict 和 predict_proba，将模型应用于给定数据集，预测得到结果，predict 是直接得到计算后的结果，如果是二分类，就是0或1。predict_proba 结果是归属于哪种类别的概率值。
```python
print(model.predict_proba(X=X_validation))
```
```python
[[0.0608 0.9392]
 [0.0141 0.9859]
 [0.0126 0.9874]
 ...
 [0.0148 0.9852]
 [0.0215 0.9785]
 [0.0333 0.9667]]
```
```python
print(model.predict(data=X_validation))
```
```python
[1 1 1 ... 1 1 1]
```
与常规预测不同，`Predict()` 函数中有个 `prediction_type` 参数，支持的预测类型包含多种：

- Probability
- Class
- RawFormulaVal
- Exponent
- LogProbability
```python
raw_pred = model.predict(
    data=X_validation,
    prediction_type='RawFormulaVal'
)
print(raw_pred)
```
```python
[2.7374 4.2445 4.3614 ... 4.1992 3.8198 3.3681]
```
可以通过 Sigmoid 函数将上面结果转换为概率。
```python
from numpy import exp
sigmoid = lambda x: 1 / (1 + exp(-x))
probabilities = sigmoid(raw_pred)
print(probabilities)
```
```python
[0.9392 0.9859 0.9874 ... 0.9852 0.9785 0.9667]
```
另一个就是 staged_predict 及 staged_predict_prob，他是阶段预测，仅考虑 trees 在`range[0; i)` 内的计算结果值。这个范围是通过参数<br />`eval_period`控制的：<br />要在应用模型或计算指标时减少要使用的树的数量，将树索引的范围设置为`[ntree_start; ntree_end)`并将要使用的树的步长设置为`eval_period`。<br />此参数定义迭代范围的步骤`[ntree_start; ntree_end)`。例如，假设设置了以下参数值：

- `ntree_start`设置为 0
- `ntree_end`设置为 N（总树数）
- `eval_period`设置为 2

在这种情况下，将返回以下树范围的结果：`[0, 2), [0, 4), ... , [0, N)`。
```python
predictions_gen = model.staged_predict_proba(
    data=X_validation,
    ntree_start=0, 
    ntree_end=5, 
    eval_period=1
)
try:
    for iteration, predictions in enumerate(predictions_gen):
        print('Iteration ' + str(iteration) + ', predictions:')
        print(predictions)
except Exception:
    pass
```
```python
Iteration 0, predictions:
[[0.3726 0.6274]
 ...
 [0.3726 0.6274]]
...
Iteration 4, predictions:
[[0.1388 0.8612]
 ...
 [0.175  0.825 ]]
```
<a name="Bhw5Y"></a>
### 在未知数据集上评估模型
使用 `eval_metrics` 方法计算指定数据集的指定指标。
```python
metrics = model.eval_metrics(
    data=pool1,
    metrics=['Logloss','AUC'],
    ntree_start=0,
    ntree_end=0,
    eval_period=1,
    plot=True
)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866532110-6f418a56-b7eb-47c8-a3b5-01a39a45e7e2.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u6702728b&originHeight=497&originWidth=1010&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue4ae37fa-ce84-4cd6-ade3-fcb1b4cc38e&title=)<br />从可视化结果看，`eval_metrics` 只包含 **Eval** 结果曲线，设置了 `metrics=['Logloss','AUC']` ，因此包含'Logloss'和'AUC'两条评估曲线。
```python
print('AUC values:')
print(np.array(metrics['AUC']))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866534688-24533cf4-7d66-44d2-8a9e-aaa44064cc1a.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u7660eee7&originHeight=623&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubac4f45a-0d23-4470-a9fd-60fc10aa90c&title=)
<a name="qUYBb"></a>
### 特征重要性
使用模型自带的`get_feature_importance`方法。
```python
model.get_feature_importance(prettified=True)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866535127-79d396e0-ffc2-4021-832d-ddf2a633f056.png#clientId=u0f7e6450-c0a1-4&from=paste&id=uca1d6f45&originHeight=536&originWidth=676&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua52276dd-b153-4639-81ba-49664d71f59&title=)<br />使用第三方解释库 Shap。与一般模型直接使用 Shap 有所不同，使用 `model.get_feature_importance()` 方法，并设置参数 `type='ShapValues'`， 直接输出 `shap_values` 值，该值可直接用户输出结果值及绘制相应可视化图形。
```python
shap_values = model.get_feature_importance(
                  pool1, type='ShapValues')
expected_value = shap_values[0,-1]
shap_values = shap_values[:,:-1]
print(shap_values.shape)
```
```python
(32769, 9)
```
```python
import shap

shap.initjs()
shap.force_plot(expected_value, shap_values[3,:], X.iloc[3,:])
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866537197-ab3b6e41-7e0a-435e-b73a-4e7aed6ebc0c.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u0df48b24&originHeight=183&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50e2731c-932f-4520-91a1-80b0425569b&title=)
```python
shap.initjs()
shap.force_plot(expected_value, shap_values[91,:], X.iloc[91,:])
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866537904-835ff26c-d584-44fc-ace8-607ec831114e.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u72ccdaea&originHeight=171&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u92b805f3-e3fa-42aa-b4d8-6993bab5c33&title=)
```python
shap.summary_plot(shap_values, X)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866537893-0cb93a57-9dc7-425f-a797-621fb0986d37.png#clientId=u0f7e6450-c0a1-4&from=paste&id=uaea11124&originHeight=556&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc5934b4f-a471-41b5-bb91-6d00f9523ba&title=)
```python
X_small = X.iloc[0:200]
shap_small = shap_values[:200]
shap.force_plot(expected_value, shap_small, X_small)
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657866539295-a80f903e-8b22-4a1c-852f-d723475863f9.gif#clientId=u0f7e6450-c0a1-4&from=paste&id=u1844809b&originHeight=400&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc31a1cca-d8c5-4514-9dc0-b993ea86a20&title=)
<a name="L5Gvl"></a>
### 特征评估
CatBoost还有个很厉害的功能，就是对指定特征进行评估，给出评估结果，是好是坏
```python
from catboost.eval.catboost_evaluation import *
learn_params = {'iterations': 20, # 2000
                'learning_rate': 0.5, # we set big learning_rate,
                                      # because we have small
                                      # #iterations
                'random_seed': 0,
                'verbose': False,
                'loss_function' : 'Logloss',
                'boosting_type': 'Plain'}
evaluator = CatboostEvaluation(
                'amazon/train.tsv',
                fold_size=10000, # <= 50% of dataset
                fold_count=20,
                column_description='amazon/train.cd',
                partition_random_seed=0,
                #working_dir=... 
)
result = evaluator.eval_features(
               learn_config=learn_params,
               eval_metrics=['Logloss', 'Accuracy'],
               features_to_eval=[6, 7, 8])
```
以上设定用来评估的特征是[6, 7, 8]，从以下结果看到特征6得到正向结论，而特征8得到负向结论，特征7从各项指标中得不到确切的指标。
```python
from catboost.eval.evaluation_result import *
logloss_result = result.get_metric_results('Logloss')
logloss_result.get_baseline_comparison(
    ScoreConfig(ScoreType.Rel, overfit_iterations_info=False)
)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657866539553-709b4bcf-d1b4-42ac-acad-e0b10786f391.png#clientId=u0f7e6450-c0a1-4&from=paste&id=u522f2f36&originHeight=240&originWidth=948&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2bd6ecfb-e37a-427c-8581-87206b8b622&title=)
<a name="PKPoy"></a>
### 模型保存和导入
当得到一个较为理想的模型后，需要保存模型，以后期使用模型，因此，该步骤还是非常重要的。而CatBoost保存模型非常方便，无需借助第三方库如pickle等，直接使用其`save_model`方法，即可保存模型。<br />`save_model`保存模型，可以保存为各种格式：

- cbm — CatBoost 二进制格式。
- coreml — Apple CoreML 格式（目前仅支持没有分类特征的数据集）。
- json — JSON 格式。有关格式详细信息，请参阅[**CatBoost JSON 模型教程**](https://github.com/catboost/tutorials/blob/master/model_analysis/model_export_as_json_tutorial.ipynb)。
- python — 独立的 Python 代码（目前不支持多分类模型）。有关应用结果模型的详细信息，请参阅 [**Python**](https://catboost.ai/en/docs/concepts/python-reference_apply_catboost_model)部分。
- cpp — 独立 C++ 代码（当前不支持多分类模型）。有关应用结果模型的详细信息，请参阅 [**C++**](https://catboost.ai/en/docs/concepts/c-plus-plus-api_applycatboostmodel)部分。
- onnx — ONNX-ML 格式（目前仅支持没有分类特征的数据集）。详情请参阅 [https://onnx.ai/](https://onnx.ai/)。有关应用结果模型的详细信息，请参阅 [**ONNX**](https://catboost.ai/en/docs/concepts/apply-onnx-ml)部分。
- pmml — [**PMML 4.3 版**](http://dmg.org/pmml/pmml-v4-3.html)格式。如果训练数据集中存在分类特征，则必须在训练期间将其解释为 one-hot 编码。这可以通过将`--one-hot-max-size/one_hot_max_size`参数设置为大于数据集中所有分类特征中唯一分类特征值的最大数量的值来实现。有关应用结果模型的详细信息，请参阅[**PMML**](https://catboost.ai/en/docs/concepts/apply-pmml)部分。
```python
my_best_model.save_model('catboost_model.bin')
my_best_model.save_model('catboost_model.json', format='json')
```
当然，导入模型也是非常方便，直接使用load_model 方法
```python
my_best_model.load_model('catboost_model.bin')
print(my_best_model.get_params())
print(my_best_model.random_seed_)
```
<a name="SZo3C"></a>
### 参考资料
CatBoost JSON 模型教程: [https://github.com/catboost/tutorials/blob/master/model_analysis/model_export_as_json_tutorial.ipynb](https://github.com/catboost/tutorials/blob/master/model_analysis/model_export_as_json_tutorial.ipynb)<br />Python: [https://catboost.ai/en/docs/concepts/python-reference_apply_catboost_model](https://catboost.ai/en/docs/concepts/python-reference_apply_catboost_model)<br />C++: [https://catboost.ai/en/docs/concepts/c-plus-plus-api_applycatboostmodel](https://catboost.ai/en/docs/concepts/c-plus-plus-api_applycatboostmodel)<br />ONNX: [https://catboost.ai/en/docs/concepts/apply-onnx-ml](https://catboost.ai/en/docs/concepts/apply-onnx-ml)<br />PMML 4.3 版: [http://dmg.org/pmml/pmml-v4-3.html](http://dmg.org/pmml/pmml-v4-3.html)<br />PMML: [https://catboost.ai/en/docs/concepts/apply-pmml](https://catboost.ai/en/docs/concepts/apply-pmml)
