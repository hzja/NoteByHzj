AI<br />假设需要执行一项回归机器学习项目。已经分析了数据，进行了一些数据清洗，创建了一些虚拟变量，现在，是时候运行机器学习回归模型了。想到的十大模型有哪些？大多数人可能都不知道有“十大回归模型”。如果不知道，也不必担心，因为在本文的最后，不仅可以运行 10 个机器学习回归模型，而且能运行 40 多个机器学习回归模型。<br />现在要在一个回归项目测试 Lazy Predict。因此，使用典型的西雅图房价数据集，在 Kaggle 上就能找到。
<a name="rMK2B"></a>
## Lazy Predict 是什么？
不需要很多代码，Lazy Predict 就能帮助构建几十个模型，并帮助了解哪些模型在不经过任何参数调整的情况下工作得更好。说明其工作原理的最好方法就是使用一个小项目，现在就开始吧。
<a name="dqQgX"></a>
## 回归项目使用 Lazy Predict
首先，要安装 Lazy Predict，可以`pip install lazypredict`回归项目到终端。接下来，导入一些用于本项目的库。
```python
# Importing important libraries
import pyforest
from lazypredict.Supervised import LazyRegressor
from pandas.plotting import scatter_matrix
# Scikit-learn packages
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import ExtraTreesRegressor
from sklearn import metrics
from sklearn.metrics import mean_squared_error
# Hide warnings
import warnings
warnings.filterwarnings(“ignore”)
# Setting up max columns displayed to 100
pd.options.display.max_columns = 100
```
可以看到导入了pyforest而非 Pandas 和 Numpy。在 Notebook 中，PyForest 可以非常快速地导入所有重要的库。
```python
# Import dataset
df = pd.read_csv('../data/kc_house_data_train.csv', index_col=0)
```
看看这个数据集是什么样子。<br />![2021-05-15-14-06-34-146766.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621059834274-897270d2-ed8d-46db-acc7-598663bf2b8b.jpeg#clientId=ubc301229-15b5-4&from=ui&id=u0bb96a21&originHeight=190&originWidth=1080&originalType=binary&size=31557&status=done&style=shadow&taskId=u4d2b5d9e-8517-4d03-95d3-098645a550c)<br />下面来检查一下数据类型。
```python
# Checking datatimes and null values
df.info()
```
![2021-05-15-14-06-34-244109.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621059841677-a212b7f5-fe2f-42a5-8f18-7094e6a75f10.jpeg#clientId=ubc301229-15b5-4&from=ui&id=ueb851815&originHeight=536&originWidth=1080&originalType=binary&size=65592&status=done&style=shadow&taskId=uf1656027-2716-479b-bc37-95dafd8982f)<br />下面是吸引注意力的几件事情。第一件是id列与这个小项目没有任何关联。但是，如果想更深入地研究这个项目，应该检查是否存在重复项。另外，date列是一个对象类型。应将其改为 DateTime 类型。这些列中的zipcode，lat和long可能与价格几乎或者根本没有关联。然而，因为本项目的目标是演示lazy predict，所以要保留它们。<br />接下来，在运行第一个模型之前，检查一些统计数据，以找出需要修改的地方。<br />![2021-05-15-14-06-34-339126.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621059873129-f797c8c8-acf0-4964-8892-30022b0a5649.jpeg#clientId=ubc301229-15b5-4&from=ui&id=u26822cb4&originHeight=274&originWidth=1080&originalType=binary&size=49423&status=done&style=shadow&taskId=ub822cd50-56a0-4872-82ed-6c040aea4f2)<br />看到了一些有趣的事情。首先，有一所房子有 33 间卧室，那不可能是真的。所以在网上查了一下，结果发现用它的id找到了这套房子，它实际上有 3 间卧室。此外，有些房子看上去没有卫生间。会包括至少 1 个卫生间，这样就可以完成数据清理了。
```python
# Fixing house with 33 bedrooms
df[df['bedrooms'] == 33] = df[df['bedrooms'] == 3]
# This will add 1 bathroom to houses without any bathroom
df['bathrooms'] = df.bedrooms.apply(lambda x: 1 if x < 1 else x)
```
<a name="YCpkT"></a>
## 拆分训练集和测试集
现在可以拆分训练集和测试集了。但是在此之前，确保代码不会出现nan或infinite的值。
```python
# Removing nan and infinite values
df.replace([np.inf, -np.inf], np.nan, inplace=True)
df.dropna(inplace=True)
```
现在将数据集分为 X 和 Y 两个变量。给训练集分配 75% 的数据集，给测试集 25%。
```python
# Creating train test split
X = df.drop(columns=['price])
y = df.price
# Call train_test_split on the data and capture the results
X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=3,test_size=0.25)
```
下面的代码将运行 40 多个模型，并显示每个模型的 R-Squared 和 RMSE。做好准备，开始！
```python
reg = LazyRegressor(ignore_warnings=False, custom_metric=None)
models, predictions = reg.fit(X_train, X_test, y_train, y_test)
print(models)
```
![2021-05-15-14-06-34-466068.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621059916497-9e5851c0-d78c-495b-839a-6b91875a8b9d.jpeg#clientId=ubc301229-15b5-4&from=ui&id=u0058b72c&originHeight=1040&originWidth=1080&originalType=binary&size=153379&status=done&style=shadow&taskId=ua07dab9d-f976-43fe-84db-c0d432887c5)<br />对于花费在上面的工作来说，这些结果非常好。对普通模型而言，这些都是非常好的 R-Squared 和 RMSE。运行了 41 个普通模型，并且得到了需要的指标，可以看到每个模型所花费的时间。一点也不差。那么，如何确定这些结果是否正确呢？通过运行一个模型，可以查看结果，看它是否和得到的结果相近。要不要测试一下基于直方图的梯度提升回归树。
<a name="T2CxZ"></a>
## 复核结果
首先，用 scikit-learn 导入这个模型。
```python
# Explicitly require this experimental feature
from sklearn.experimental import enable_hist_gradient_boosting
# Now you can import normally from ensemble
from sklearn.ensemble import HistGradientBoostingRegressor
```
此外，还创建了一个函数来检查模型的度量。
```python
# Evaluation Functions
def rmse(model, y_test, y_pred, X_train, y_train):
r_squared = model.score(X_test, y_test)
mse = mean_squared_error(y_test, y_pred)
rmse = np.sqrt(mse)
print(‘R-squared: ‘ + str(r_squared))
print(‘Mean Squared Error: ‘+ str(rmse))
# Create model line scatter plot
def scatter_plot(y_test, y_pred, model_name):
plt.figure(figsize=(10,6))
sns.residplot(y_test, y_pred, lowess=True, color='#4682b4',
line_kws={'lw': 2, 'color': 'r'})
plt.title(str('Price vs Residuals for '+ model_name))
plt.xlabel('Price',fontsize=16)
plt.xticks(fontsize=13)
plt.yticks(fontsize=13)
plt.show()
```
最后，来运行这个模型并查看结果。
```python
# Histogram-based Gradient Boosting Regression Tree
hist = HistGradientBoostingRegressor()
hist.fit(X_train, y_train)
y_pred = hist.predict(X_test)
```
![2021-05-15-14-06-34-582955.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621059978295-419b7918-1b40-4907-9290-0c7fa9fedfc1.jpeg#clientId=ubc301229-15b5-4&from=ui&id=ua339a43d&originHeight=623&originWidth=1080&originalType=binary&size=51651&status=done&style=shadow&taskId=u160a3b35-5583-49c2-9604-26f090ec600)<br />用 Lazy Predict 得到的结果和这个结果非常接近。看来这确实很管用。
<a name="IOVok"></a>
## 最后想法
Lazy Predict 是一个神奇的库，易于使用，并且非常快速，只需要很少的代码就可以运行普通模型。可以使用 2 到 3 行的代码来手动设置，而不需要手工设置多个普通模型。切记，不要把结果作为最终的模型，应该始终对结果进行复核，以确保库工作正常。数据科学是一个复杂的领域，Lazy Predict 并不能取代那些优化模型的专业人员的专业知识。
