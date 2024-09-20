介绍10个Sklearn方法，比较小众但非常好用。
<a name="aX6lh"></a>
## 1️、`FunctionTransformer`
虽然Sklearn中有很多内置的预处理操作可以放进pipeline管道，但很多时候并不能满足需求。<br />如果是比较简单并且通过一个函数可以实现需求的情况，可以将函数通过`FunctionTransformer`进行包装生成可与Sklearn兼容的转换器，然后装进pipeline。
```python
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import FunctionTransformer

def reduce_memory(X: pd.DataFrame, y=None):
    """将数值型列的类型转换为float32类型，以减少内存占用
    """

    num_cols = X.select_dtypes(incluce=np.number).columns
    for col in num_cols:
        X[col] = X.astype("float32")

    return X, y

ReduceMemoryTransformer = FunctionTransformer(reduce_memory)

# 装进一个pipeline
>>> make_pipeline(SimpleImputer(), ReduceMemoryTransformer)

Pipeline(steps=[('simpleimputer', SimpleImputer()),
                ('functiontransformer', ReduceMemoryTransformer()])
```
<a name="PTaAO"></a>
## 2、自定义`transformers`
但有更复杂需求的时候，可能一个简单函数也是无法完成功能实现的，这时就需要自己实实在在地创建一个转换器了。<br />比如，数据清洗中比较常见的操作缩放特征变量并使其呈正态分布。通常会使用对数变换器如`PowerTransformer`或np.log，但默认的方法会有一点问题，即如果某个特征包含零值，那么底层的对数函数无法处理会提出报错。<br />因此，一般的应对方法是将特征向量加上1，然后再执行转换，以避免报错。如果想要还原原始向量，直接调用指数函数然后再减去1，就可以了。非常的丝滑。<br />当然，这个操作并未内置于Sklearn中，并且也不是一个简单函数能搞定的。下面看看如何自定义一个转换器解决这个问题。
```python
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.preprocessing import PowerTransformer

class CustomLogTransformer(BaseEstimator, TransformerMixin):
    def __init__(self):
        self._estimator = PowerTransformer()  # 初始化一个转换器

    def fit(self, X, y=None):
        X_copy = np.copy(X) + 1  # 防止零值出现报错，进行加一操作
        self._estimator.fit(X_copy)

        return self

    def transform(self, X):
        X_copy = np.copy(X) + 1

        return self._estimator.transform(X_copy)  # 执行转换

    def inverse_transform(self, X):
        X_reversed = self._estimator.inverse_transform(np.copy(X))

        return X_reversed - 1  # 指数函数后减一
```
上面创建了一个类，继承了`BaseEstimator`并使其`TransformerMixin`能够插入pipeline管道的类。
<a name="Wb8OS"></a>
## 3、`TransformedTargetRegressor`
有些时候，不仅仅是特征X需要处理，目标变量y也需要预处理操作。一个典型的场景就是上面提到的缩放数据使其呈现正态分布。通常会在pipeline以外做额外的处理，但 Sklearn 有一个方法可以同时在管道中处理。<br />`TransformedTargetRegressor`是一个专门针对regressor回归器进行转换的类，通过它可以同时将特征X和目标变量y在管道pipeline中做处理。比如下面的lgb回归的例子，它使用`CustomLogTransformer`对目标y进行对数缩放，然后拟合回归模型。
```python
from sklearn.compose import TransformedTargetRegressor

reg_lgbm = lgbm.LGBMRegressor()

final_estimator = TransformedTargetRegressor(
    regressor=reg_lgbm, transformer=CustomLogTransformer()
)

final_estimator.fit(X_train, y_train)
TransformedTargetRegressor(regressor=LGBMRegressor(),
                           transformer=CustomLogTransformer())
```
如果转换器是一个函数如np.log，可以将其传递给func参数。
<a name="aZ7bz"></a>
## 4、管道流程图
如果管道由多个步骤或子管道组成，代码上可能会比较混乱。Sklearn提供了估计器的HTML表示形式，让整理处理流程更直观清晰：
```python
>>> giant_pipeline

Pipeline(steps=[('columntransformer',
                 ColumnTransformer(transformers=[('cat_pipe',
                                                  Pipeline(steps=[('impute',
                                                                   SimpleImputer(strategy='most_frequent')),
                                                                  ('oh',
                                                                   OneHotEncoder())]),
                                                  <sklearn.compose._column_transformer.make_column_selector object at 0x000001B6D8BD9310>),
                                                 ('num_pipe',
                                                  Pipeline(steps=[('impute',
                                                                   SimpleImputer(strategy='median')),
                                                                  ('transform',
                                                                   QuantileTransformer())]),
                                                  <sklearn.compose._column_transformer.make_column_selector object at 0x000001B6D8BD9160>)])),
                ('lgbmregressor',
                 LGBMRegressor(device_type='gpu', learning_rate=0.01,
                               n_estimators=10000))])
```
```python
from sklearn import set_config

set_config(display="diagram")

>>> giant_pipeline
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699882658601-3b8db5a5-034d-4fe8-bea6-be6f34b40db2.png#averageHue=%23c1c7bf&clientId=u43092785-386d-4&from=paste&id=u0a6d3637&originHeight=327&originWidth=798&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u435b8559-124e-40c4-84b9-426a6f0511e&title=)<br />将dispaly参数设置为diagram，就可以获得管道的HTML的交互式形式。
<a name="C67o2"></a>
## 5、`QuadraticDiscriminantAnalysis`
QDA为`QuadraticDiscriminantAnalysis`的简称，是**二次判别分析**的意思。在Kaggle竞赛中，即使没有超参数调整，二次判别分析分类器也可以获得AUC0.965这样高的分数，超过了大多数树模型，包括XGBoost和LightGBM。<br />那为什么之前很少听说过该算法呢？因为它的使用有严格的限制条件，它**要求训练特征严格的正态分布**，这样QDA就可以轻松计算并拟合分布周围的椭球形状了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699882658725-aea76ccc-a663-4147-b32e-0046cbf82bf2.png#averageHue=%23fafafa&clientId=u43092785-386d-4&from=paste&id=u60cb4318&originHeight=435&originWidth=582&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u99b30f6a-26d4-45f0-82aa-e990476e6fe&title=)<br />QDA 的另一个优点是它的速度非常快，在百万行数据集上训练它只需要几秒钟。以下是QDA在Sklearn中的执行速度。
```python
%%time

from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis

# Generate 1M samples and 100 features
X, y = make_classification(n_samples=1000000, n_features=100)
qda = QuadraticDiscriminantAnalysis().fit(X, y)

Wall time: 13.4 s
```
<a name="AFYLC"></a>
## 6、Voting Classifier/Regressor
在模型训练中，经常会遇到几个模型效果相似的情况，想要进一步提升效果，这时可以使用**投票方法**，是一种简单的模型集成方法。<br />投票方法效果提升原因在于**概率论**，简单来说就是少数服从多数。具体的就是，投票分类器会将多个分类器的多数票作为最终预测，而如果类别是概率或预测是连续的，则对预测进行平均。<br />Sklearn提供了两个方法`VotingClassifier`和`VotingRegressor`，只需要传递一个分类器或回归器的列表，将它们组合起来就可以了。下面是VotingClassifier的示例。
```python
from sklearn.ensemble import VotingClassifier

X, y = make_classification(n_samples=1000)

ensemble = VotingClassifier(
    estimators=[
        ("xgb", xgb.XGBClassifier(eval_metric="auc")),
        ("lgbm", lgbm.LGBMClassifier()),
        ("cb", cb.CatBoostClassifier(verbose=False)),
    ],
    voting="soft",
    # n_jobs=-1,
)

_ = ensemble.fit(X, y)
```
以上设置voting参数为soft，代表预测是概率。此外，还可以为不同的模型分配weights权重系数进行更精准的预测。
<a name="NbXai"></a>
## 7、Stacking Classifier/Regressor
另一种比投票更强大的集成方法是**stacking**。<br />stacking背后的思想是，子模型应该尽可能多样化，因为不同的模型从不同的角度学习训练集的信息，可以覆盖整个信息空间。<br />换句话说，各种模型（如树、线性模型、表面拟合器、近邻模型、贝叶斯模型和高斯模型）最大化了训练潜力，它们的组合输出减少了偏差并防止了过拟合。<br />Kaggle竞赛中，stacking是一个提分的神器，很多获胜方案中都有提及。示例代码如下。
```python
from sklearn.ensemble import StackingClassifier, StackingRegressor
from sklearn.linear_model import LogisticRegression

X, y = make_classification(n_samples=1000)

ensemble = StackingClassifier(
    estimators=[
        ("xgb", xgb.XGBClassifier(eval_metric="auc")),
        ("lgbm", lgbm.LGBMClassifier()),
        ("cb", cb.CatBoostClassifier(verbose=False)),
    ],
    final_estimator=LogisticRegression(),
    cv=5,
    passthrough=False
    # n_jobs=-1,
)

_ = ensemble.fit(X, y)
```
<a name="joqmm"></a>
## 8、`LocalOutlierFactor`
异常值会使模型的目标函数产生偏差，可能导致过于乐观或过于悲观的结果。<br />对于小数据集来说，查找异常值不成问题。如果特征超过50-100个时，就需要一种快速准确的算法来检测高维异常值了。<br />对于具有数百个特征和数百万行的数据集，原始算法可能也需要运行几个小时。这时可以将降维算法与异常值检测方法结合起来，一个比较好的组合是UMAP和`LocalOutlierFactor`。`LocalOutlierFactor`是一种基于近邻的算法，旨在快速处理大型数据集。
```python
%%time

import umap  # pip install umap
from sklearn.neighbors import LocalOutlierFactor

X, y = make_classification(n_samples=5000, n_classes=2, n_features=10)
X_reduced = umap.UMAP(n_components=2).fit_transform(X, y)

lof = LocalOutlierFactor()
labels = lof.fit_predict(X_reduced, y)

Wall time: 17.8 s

    >>> np.where(labels == -1)
    (array([ 119,  155,  303,  331,  333,  407,  418,  549,  599,  664,  795,
            3092, 3262, 3271, 3280, 3289, 3311, 3477, 3899, 3929, 3975, 4301,
            4358, 4442, 4522, 4561, 4621, 4631, 4989], dtype=int64),)
```
<a name="D7dag"></a>
## 9️、`QuantileTransformer`
某些时候得到的模型结果分布非常不规则，可能通过对数转换器或缩放器都无法强制转换为正态分布，比如双峰、三峰、或者n峰的分布。<br />这种情况下可以使用`QuantileTransformer`，它使用分位数的统计指标实现中心化和缩放分布。
```python
import pandas as pd
from sklearn.preprocessing import QuantileTransformer

qt = QuantileTransformer().fit(crazy_distributions)

crazy_feature_names = ["f18", "f31", "f61"]
crazy_distributions = pd.DataFrame(qt.transform(crazy_distributions), columns=crazy_feature_names)

fig, axes = plt.subplots(1, 3, figsize=(20, 6))

for ax, f_name in zip(axes.flatten(), crazy_feature_names):
    sns.kdeplot(crazy_distributions[f_name], ax=ax, color="#E50914")
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699882658552-73ee3daf-7b65-4faa-9679-0daf028de8c0.png#averageHue=%23b7b7b7&clientId=u43092785-386d-4&from=paste&id=u9abea931&originHeight=343&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u078b9c8a-e6c4-49e0-9372-730e4585402&title=)
<a name="kALdA"></a>
## 10、PCA + tSNE/UMAP
这个一个降维的组合使用方法。因为PCA主成分分析对于高维度处理速度是比较快的，因此通常作为第一阶段的处理方法，比如使用PCA缩减到30-50的维度，然后再用其他算法如tSNE或UMAP作为第二阶段的处理方法。<br />下面是 PCA 和 tSNE 的组合：
```python
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE

df = dt.fread("data/large.csv").to_pandas()

>>> df.shape
(1000000, 287)

X, y = df.drop("target", axis=1), df[["target"]].values.flatten()

%%time
manifold_pipe = make_pipeline(QuantileTransformer(), PCA(n_components=30), TSNE())

reduced_X = manifold_pipe.fit_transform(X, y)

------------------------------------------

Wall time: 4h 27min 46s
```
以上在100万行和约300个特征的数据集上，先通过PCA投影到前30个维度，然后再投影到二维，整个过程需要4.5小时，并且结果也不是很好。
```python
>>> plt.scatter(reduced_X[:, 0], reduced_X[:, 1], c=y, s=0.05);
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699882658594-d1a52ae4-e931-45e4-8ad1-6285dbe63fad.png#averageHue=%23a39e95&clientId=u43092785-386d-4&from=paste&id=u32d950f3&originHeight=272&originWidth=380&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0cce7729-3247-4c4d-8ea8-c810f6be93f&title=)<br />因此建议使用UMAP，它比tSNE快得多，并且可以更好地保留了数据的局部结构。
```python
%%time
manifold_pipe = make_pipeline(QuantileTransformer(), PCA(n_components=30))

X_pca = manifold_pipe.fit_transform(X, y)
embedding = umap.UMAP(n_components=2).fit(X_pca, y)

Wall time: 14min 27s

>>> plt.scatter(embedding.embedding_[:, 0], embedding.embedding_[:, 1], c=y, s=0.05);
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699882658564-0a7f5f2d-f3a5-4322-b29a-6509b718708f.png#averageHue=%23c5c4bb&clientId=u43092785-386d-4&from=paste&id=uf42aad88&originHeight=248&originWidth=370&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6a261287-006c-4ba1-8c61-00af10b759f&title=)<br />UMAP设法找到目标类别之间的明显区别，并且速度比tSNE快了20倍。
