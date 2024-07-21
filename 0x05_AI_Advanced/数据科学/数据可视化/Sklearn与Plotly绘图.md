Python 可视化 Plotly<br />![2021-05-29-14-10-28-223623.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269134454-e30b96a8-963b-4590-a1b1-4e15ac22de91.png#clientId=u3f1e8791-536d-4&from=ui&id=u45bb5be2&originHeight=427&originWidth=1080&originalType=binary&size=67567&status=done&style=shadow&taskId=u8b15e6d6-de01-4017-80cf-034043805f7)
<a name="rL4Us"></a>
## Plotly基本介绍
Plotly：协同 Python 和 matplotlib 工作的 web 绘图库<br />官网链接：[https://plot.ly/python/](https://plot.ly/python/)<br />Plotly 是一款用来做数据分析和可视化的在线平台，功能非常强大，可以在线绘制很多图形比如条形图、散点图、饼图、直方图等等。而且还是支持在线编辑，以及多种语言python、javascript、matlab、R等许多API。<br />Plotly在Python中使用也很简单，直接用`pip install plotly`就可以了。推荐最好在Jupyter notebook中使用，Pycharm操作不是很方便。<br />Plotly的图表多样化且专业化，可以绘制很多专业学科领域的图表。下面是官网的几种划分。
<a name="dvlWB"></a>
### 基本图表
![2021-05-29-14-10-28-332331.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269144594-74147914-61da-442e-9b54-77aae89d1298.png#clientId=u3f1e8791-536d-4&from=ui&id=u8f47e6e1&originHeight=307&originWidth=1080&originalType=binary&size=145138&status=done&style=shadow&taskId=u6769ca6f-fca3-4b01-96b6-52ddc3a0e6b)
<a name="iOAKs"></a>
### 基础图表
![2021-05-29-14-10-28-428073.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269154506-3af82528-4d1b-43de-aea6-955cfe65558c.png#clientId=u3f1e8791-536d-4&from=ui&id=ubfb274ac&originHeight=268&originWidth=1080&originalType=binary&size=161006&status=done&style=shadow&taskId=u78319acf-c01c-4784-91fd-5eb8f4f9549)
<a name="qheqc"></a>
### 统计图
![2021-05-29-14-10-28-520829.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269164517-9f8dcba0-86e8-45d7-bef7-dc7ee6b33eeb.png#clientId=u3f1e8791-536d-4&from=ui&id=u87e424c5&originHeight=262&originWidth=1048&originalType=binary&size=90597&status=done&style=shadow&taskId=u251e695a-eac6-4498-9391-1e21b70de51)
<a name="Nctod"></a>
### 科学图
![2021-05-29-14-10-28-662452.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269175767-afddd232-f609-420e-a0f1-cb231f159a46.png#clientId=u3f1e8791-536d-4&from=ui&id=u5156ec9a&originHeight=259&originWidth=1066&originalType=binary&size=217007&status=done&style=shadow&taskId=u04e6ed94-01ac-40ff-96c5-41ba58d7f5c)
<a name="QCmQX"></a>
### 金融图表
![2021-05-29-14-10-28-788245.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269186973-62f800fe-5d2e-4a90-a869-55093323a4bb.png#clientId=u3f1e8791-536d-4&from=ui&id=uc3c75b3f&originHeight=268&originWidth=1080&originalType=binary&size=161006&status=done&style=shadow&taskId=u3db913d7-e692-4493-84a3-1f6e37b438e)
<a name="v1jGK"></a>
### 地图
![2021-05-29-14-10-28-975168.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269196663-b670e9c5-dbc5-4ace-b64a-6109ad7cad4e.png#clientId=u3f1e8791-536d-4&from=ui&id=u7d889a14&originHeight=291&originWidth=1080&originalType=binary&size=244936&status=done&style=shadow&taskId=u580a6824-a2d6-417a-9506-e4100dbafbe)
<a name="GlewS"></a>
### 3D图表
![2021-05-29-14-10-29-156445.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269207139-652785eb-9807-49e4-a3c7-3f717ad3e962.png#clientId=u3f1e8791-536d-4&from=ui&id=uebc6a9d9&originHeight=264&originWidth=1080&originalType=binary&size=239419&status=done&style=shadow&taskId=u761d8f92-522d-48a0-bc0d-f1e40ea59bd)
<a name="UCAYu"></a>
### 多子图
![2021-05-29-14-10-29-378850.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269216239-d1e70ee1-a4c3-45ba-b806-4822962b4430.png#clientId=u3f1e8791-536d-4&from=ui&id=u9da52253&originHeight=357&originWidth=1080&originalType=binary&size=250727&status=done&style=shadow&taskId=ua3b921e0-626e-4ae2-b891-64be95e782f)
<a name="NZaLI"></a>
### 与Jupyter交互图
![2021-05-29-14-10-29-578316.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622269248708-1d3adc54-b593-4698-a05f-393e41bfdc4e.gif#clientId=u3f1e8791-536d-4&from=ui&id=u5ddb43f2&originHeight=314&originWidth=895&originalType=binary&size=73896&status=done&style=shadow&taskId=u2a51385f-78dc-48a2-b2b3-6c98c02abb4)
<a name="XOxD0"></a>
### 添加自定义控件
![2021-05-29-14-10-29-698233.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269261035-6c004653-a6ca-48b0-a749-493fef139a3a.png#clientId=u3f1e8791-536d-4&from=ui&id=u6eedd44d&originHeight=351&originWidth=1080&originalType=binary&size=151739&status=done&style=shadow&taskId=u8e42ca39-2bd9-4b76-9a5b-7628dc05c99)
<a name="xB71C"></a>
### 人工智能与机器学习图<br />![2021-05-29-14-10-29-853778.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269273165-703fcb54-ea8f-48fa-9ace-c79c2cb4961d.png#clientId=u3f1e8791-536d-4&from=ui&id=u54df6566&originHeight=294&originWidth=1080&originalType=binary&size=169619&status=done&style=shadow&taskId=udad6080b-b51d-428e-bb4f-7cf9f29c616)

---

主要深入探讨poltly与机器学习结合，绘制机器学习相关图。  <br />注意：正文中绘图代码仅展示部分核心代码！
<a name="UDFEr"></a>
## Plotly Express 回归
如何使用plotly图表来显示各种类型的回归模型，从简单的模型如线性回归，到其他机器学习模型如决策树和多项式回归。<br />重点学习plotly的各种功能，如使用不同参数对同一模型进行比较分析、Latex显示、3D表面图，以及使用plotly Express进行增强的预测误差分析。
<a name="iWXEr"></a>
### Plotly Express 简介
Plotly Express 是plotly的易于使用的高级界面，可处理多种类型的数据并生成易于样式化的图形。<br />通过Plotly Express 可以将普通最小二乘回归趋势线添加到带有`trendline`参数的散点图中。为此需要安装statsmodels及其依赖项。<br />基础图形: `scatter`, `line`, `area`, `bar`, `funnel`, `timeline`<br />部分到整体图表: `pie`, `sunburst`, `treemap`, `funnel_area`<br />一维分布图: `histogram`, `box`, `violin`, `strip`<br />二维分布图: `density_heatmap`, `density_contour`<br />矩阵的输入图: `imshow`<br />三维图: `scatter_3d`, `line_3d`<br />多维图: `scatter_matrix`, `parallel_coordinates`, `parallel_categories`<br />平铺地图: `scatter_mapbox`, `line_mapbox`, `choropleth_mapbox`, `density_mapbox`<br />离线地图: `scatter_geo`, `line_geo`, `choropleth`<br />极坐标图: `scatter_polar`, `line_polar`, `bar_polar`<br />三元图: `scatter_ternary`, `line_ternary`
<a name="wCmCD"></a>
### 普通最小二乘回归可视化
将线性普通最小二乘(OLS)回归趋势线或非线性局部加权散点图平滑(LOWESS)趋势线添加到Python中的散点图。将鼠标悬停在趋势线上将显示该线的方程式及其R平方值，非常方便。
<a name="r3oLl"></a>
#### 单线拟合
与seaborn类似，plotly图表主题不需要单独设置，使用默认参数即可满足正常情况下的使用，因此一行代码并设置参数`trendline="ols"`即可搞定散点图与拟合线的绘制，非常方便。
```python
import plotly.express as px
fig=px.scatter(df, x="open", y="close", 
                 trendline="ols")
fig.show()
```
![2021-05-29-14-10-30-039280.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269310360-61face36-db24-4e04-bb44-d4d653c448d2.png#clientId=u3f1e8791-536d-4&from=ui&id=u5a1379a9&originHeight=604&originWidth=1080&originalType=binary&size=73456&status=done&style=shadow&taskId=ub2c6f05c-3c6c-4b36-bf0d-6171f10ab5b)
<a name="YDWTN"></a>
#### 多线拟合
同样，在绘制多个变量及多个子图时，也不需要设置多画布，只要设置好参数 `'x'，'y'，'facet_col'，'color'` 即可。
```python
fig=px.scatter(df, x="open", y="close", 
              facet_col="Increase_Decrease", 
              color="Up_Down", trendline="ols")
fig.show()
```
![2021-05-29-14-10-30-148987.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269325561-75416253-c704-4200-b72d-2b835734e74e.png#clientId=u3f1e8791-536d-4&from=ui&id=u9b92cce9&originHeight=598&originWidth=1080&originalType=binary&size=105682&status=done&style=shadow&taskId=ucff1b4c8-2374-4399-9e58-7664ef5fdb2)
<a name="tAn4k"></a>
#### 查看拟合结果
绘图后，需要查看具体的各项统计学数据，可以通过`get_trendline_results`方法，具体代码与结果如下。
```python
results = px.get_trendline_results(fig)
results.query(
  "Up_Down == 'Up' and Increase_Decrease == '1'"
       ).px_fit_results.iloc[0].summary()
```
![2021-05-29-14-10-30-266672.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269337610-72a3d198-53f7-46ae-928d-94ec329c3a4d.png#clientId=u3f1e8791-536d-4&from=ui&id=u88aa35fc&originHeight=894&originWidth=808&originalType=binary&size=63823&status=done&style=shadow&taskId=ub6dd4156-293e-44d8-ab71-ee3dc634465)
<a name="kczvP"></a>
### 非线性回归可视化
非线性回归拟合是通过设置参数`trendline="lowess"`来实现，Lowess是指局部加权线性回归，它是一种非参数回归拟合的方式。
```python
fig = px.scatter(df2, x="date", y="open", 
                 color="Increase_Decrease", 
                 trendline="lowess")
fig.show()
```
![2021-05-29-14-10-30-418268.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269353680-b858e5ca-6691-4b11-978a-f77e96d252ea.png#clientId=u3f1e8791-536d-4&from=ui&id=ucc6906bc&originHeight=612&originWidth=1080&originalType=binary&size=73446&status=done&style=shadow&taskId=ubd8cfd46-a285-410c-88f1-aa3dd9eaf2e)
<a name="jZHPY"></a>
## Sklearn与Plotly组合
Scikit-learn是一个流行的机器学习(ML)库，它提供了各种工具，用于创建和训练机器学习算法、特征工程、数据清理以及评估和测试模型。<br />这里使用Scikit-learn来分割和预处理的数据，并训练各种回归模型。
<a name="QxqWR"></a>
### 线性回归可视化
可以使用Scikit-learn的线性回归执行相同的预测。与直接用`plotly.express`拟合普通最小二乘回归不同，这是通过散点图和拟合线组合的方式绘制图形，这会更加灵活，除了添加普通线性回归拟合曲线，还可以组合其他线性回归曲线，即将拟合结果很好地可视化出来。
```python
import plotly.graph_objects as go
from sklearn.linear_model import LinearRegression
X = df.open.values.reshape(-1, 1)
# 回归模型训练
model = LinearRegression()
model.fit(X, df.close)
# 生产预测点
x_range = np.linspace(X.min(), X.max(), 100)
y_range = model.predict(x_range.reshape(-1, 1))
# 图形绘制
fig = px.scatter(df, x='open', y='close', opacity=0.65)
fig.add_traces(go.Scatter(x=x_range, y=y_range, name='Regression Fit'))
fig.show()
```
![2021-05-29-14-10-30-560887.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269366198-4f855f08-5ee1-4df9-9b0e-145c31dd7193.png#clientId=u3f1e8791-536d-4&from=ui&id=ud8d8aa23&originHeight=586&originWidth=1080&originalType=binary&size=67955&status=done&style=shadow&taskId=ua0ba80db-85d7-453a-8261-622c8d53026)
<a name="E0aTi"></a>
#### 模型泛化能力可视化
利用plotly可视化查看模型泛化能力，即需要比较模型分别在训练集与测试集上的拟合状况。这里使用`Scatter`绘图，可以通过用不同的颜色着色训练和测试数据点，将训练集与测试集数据及拟合线绘制在同一张画布上，即可很容易地看到模型是否能很好地拟合测试数据。<br />![2021-05-29-14-10-30-725957.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269379570-2847c17a-a90a-4ff0-b746-423282510bd4.png#clientId=u3f1e8791-536d-4&from=ui&id=uce951d43&originHeight=531&originWidth=1080&originalType=binary&size=60755&status=done&style=shadow&taskId=ub6bd264c-5f44-4aaf-a354-3a548c93ba2)
<a name="xBLVU"></a>
### KNN回归可视化
KNN回归的原理是从训练样本中找到与新点在距离上最近的预定数量的几个点，并从这些点中预测标签。<br />KNN回归的一个简单的实现是计算最近邻K的数值目标的平均值。另一种方法是使用K近邻的逆距离加权平均值。
```python
from sklearn.neighbors import KNeighborsRegressor
# 数据准备
X = df2.open.values.reshape(-1, 1)
x_range = np.linspace(X.min(), X.max(), 100)
# 模型训练，weights='distance'及weights='uniform'
knn_dist = KNeighborsRegressor(10, weights='distance')
knn_dist.fit(X, df2.Returns)
y_dist = knn_dist.predict(x_range.reshape(-1, 1))
# 绘制散点图及拟合曲线
fig = px.scatter(df2, x='open', y='Returns', color='Up_Down', opacity=0.65)
fig.add_traces(go.Scatter(x=x_range, y=y_uni, name='Weights: Uniform'))
# 'Weights: Distance'
fig.show()
```
![2021-05-29-14-10-30-845636.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269392786-b84dff4d-17d2-4733-b724-0a50efcf327e.png#clientId=u3f1e8791-536d-4&from=ui&id=u0de88091&originHeight=569&originWidth=1080&originalType=binary&size=88690&status=done&style=shadow&taskId=u54e957fb-97f4-4ac1-a35a-cf128a88450)
<a name="ktr9l"></a>
### 多项式回归可视化
线性回归是如何拟合直线的，而KNN可以呈现非线性的形状。除此之外，还可以通过使用scikit-learn的多项式特征为特征的n次幂拟合一个斜率，将线性回归扩展到多项式回归。<br />使用Plotly，只需在方程前后添加$符号，就可以在图例和标题中使用`$\LaTeX$`显示拟合方程，即可以看到多项式回归拟合的系数。
```python
# 定义图例中多项式方程函数
def format_coefs(coefs):
    equation_list = [f"{coef}x^{i}" for i, 
        coef in enumerate(coefs)]
    equation = "$" +  " + ".join(equation_list) + "$"
    replace_map = {"x^0": "", "x^1": "x", '+ -': '- '}
    for old, new in replace_map.items():
        equation = equation.replace(old, new)
    return equation
# 绘制散点图
fig = px.scatter(df, x='open', y='High_Low', opacity=0.65)
# 利用循环方式绘制多项式拟合曲线
fig.add_traces(go.Scatter(x=x_range.squeeze(), y=y_poly, name=equation))
```
![2021-05-29-14-10-30-991248.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269422127-adcf66c3-906b-4739-9e77-d745fb041802.png#clientId=u3f1e8791-536d-4&from=ui&id=u6f826a48&originHeight=576&originWidth=1080&originalType=binary&size=71323&status=done&style=shadow&taskId=u01575d25-6569-4425-9778-74ce3e3a328)
<a name="cXOFb"></a>
### 3D图绘制支持向量机决策边界
二维平面中，当类标签给出时，可以使用散点图考察两个属性将类分开的程度。即用一条直线或者更复杂的曲线，将两个属性定义的平面分成区域，每个区域包含一个类的大部分对象，则可能基于这对指定的属性构造精确的分类器，如用于二分类的逻辑回归。<br />而在更高维度中，即当输入数据中有多个变量时，分类器可以是支持向量机(SVM)，其通过在高维空间中寻找决策边界以区分不同类别标签。如在三维空间中可以通3D图内的曲线来可视化模型的决策平面。<br />在Plotly中可以利用`px.scatter_3d` 和`go.Surface`绘制3D图。
```python
from sklearn.svm import SVR
# 建立模型
model = SVR(C=1.)
model.fit(X, y)
# 使用模型预测
pred = model.predict(np.c_[xx.ravel(), yy.ravel()])
pred = pred.reshape(xx.shape)
# 绘图
fig = px.scatter_3d(df, x='sepal_length', y='sepal_width', z='petal_width')
fig.update_traces(marker=dict(size=5))
fig.add_traces(go.Surface(x=xrange, y=yrange, 
                          z=pred, name='pred_surface'))
```
![2021-05-29-14-10-31-159797.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269436531-01d20acc-f8fb-416d-9503-f2a7e8953f4f.png#clientId=u3f1e8791-536d-4&from=ui&id=u6259b9ce&originHeight=613&originWidth=1080&originalType=binary&size=121350&status=done&style=shadow&taskId=u70e570af-3d01-4c7c-b991-d820c559bab)
<a name="fndyk"></a>
### 多元线性回归可视化
本节介绍用plotly可视化多元线性回归(MLR)的系数。<br />用一个或两个变量可视化回归是很简单的，因为可以分别用散点图和3D散点图来绘制它们。但如果有两个以上的特性，则需要找到其他方法来可视化数据。<br />一种方法是使用条形图。下面列子中每个条形图表示每个输入特征的线性回归模型的系数。柱状图等大小代表线性回归系数的大小，负相关与正相关分别用红色与蓝色区分，特别显目。
```python
X = df.loc[:,['open', 'close','volume', 'Increase_Decrease']]
X = pd.get_dummies(X, columns=['Increase_Decrease'], prefix_sep='=')
y = df['Returns']
# 模型训练
model = LinearRegression()
model.fit(X, y)
# 绘制柱状图
fig = px.bar(
    x=X.columns, y=model.coef_, color=colors,
    color_discrete_sequence=['red', 'blue'],
    labels=dict(x='Feature', y='Linear coefficient'),
    title='Weight of each feature for predicting Returns'
)
fig.show()
```
![2021-05-29-14-10-31-290578.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269462317-bf98b0a3-fcd1-4387-a6f3-d124d18f9d9d.png#clientId=u3f1e8791-536d-4&from=ui&id=u1372cd2e&originHeight=591&originWidth=1080&originalType=binary&size=73740&status=done&style=shadow&taskId=ueef8b125-2bd7-45c9-b5c6-c5f60c347ff)
<a name="Tq7XQ"></a>
#### 实际点与预测点的比较图
这介绍了比较预测输出与实际输出的最简单方法，即以真实值为x轴，以预测值为y值，绘制二维散点图。从图中看，若理论最优拟合(黑色斜线)附近有大部分的散点则说明模型拟合效果很好。
```python
y_pred = model.predict(X)
# 绘制散点图
fig = px.scatter(x=y, y=y_pred, 
                 labels={'x': 'ground truth',
                 'y': 'prediction'})
# 绘制理论最优拟合
fig.add_shape( 
    type="line", line=dict(dash='dash'),
    x0=y.min(), y0=y.min(),
    x1=y.max(), y1=y.max())
fig.show()
```
![2021-05-29-14-10-31-450160.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269476957-03c5a64f-2f8d-48c4-8e23-e4d96da8ba09.png#clientId=u3f1e8791-536d-4&from=ui&id=ufc6bf74a&originHeight=593&originWidth=1080&originalType=binary&size=58215&status=done&style=shadow&taskId=ucf1c68c6-b6e6-4449-8a13-81cdd3483e3)
<a name="EkM9w"></a>
### 增强的预测误差分析图
通过添加边缘直方图来快速诊断模型可能存在的任何预测误差。通过将模型与理论最优拟合(黑色虚线)进行比较，内置的OLS功能可以可视化模型的泛化程度。<br />边缘的直方图表示在某个区间内，模型与理论最优拟合之间的误差值，不同的颜色代表不同的数据集。
```python
model = LinearRegression()
model.fit(X_train, y_train)
df['prediction'] = model.predict(X)
# 散点图与拟合虚线
fig = px.scatter(
    df, x='open', y='prediction',
    marginal_x='histogram', marginal_y='histogram',
    color='split', trendline='ols')
# 边缘直方图
fig.update_traces(histnorm='probability', selector={'type':'histogram'})
# 理论最优拟合 黑色虚线
fig.add_shape(
    type="line", line=dict(dash='dash'),
    x0=y.min(), y0=y.min(),
    x1=y.max(), y1=y.max())
```
![2021-05-29-14-10-31-561818.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269491151-e0023511-3160-4558-974e-f47a2d59297d.png#clientId=u3f1e8791-536d-4&from=ui&id=u4c24e44b&originHeight=571&originWidth=1080&originalType=binary&size=51849&status=done&style=shadow&taskId=udb01abf8-6a3b-4e35-b010-47e70834c55)
<a name="LcRGi"></a>
### 残差图
就像预测误差图一样，使用plotly很容易在几行代码中可视化预测残差。即在常规的散点图中设置预测参数`trendline='ols'`及预测残差参数`marginal_y='violin'`，并以小提琴的图形展示出来。
```python
# 模型训练
model = LinearRegression()
model.fit(X_train, y_train)
# 模型预测
df['prediction'] = model.predict(X)
df['residual'] = df['prediction'] - df['close']
# 绘制散点图和拟合线
fig = px.scatter(
    df, x='prediction', y='residual',
    marginal_y='violin',   # 设置残差小提琴图
    color='split', trendline='ols')
fig.show()
```
![2021-05-29-14-10-31-659569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269504857-5cb6aa61-60fb-4381-8579-2d29a0aa84f4.png#clientId=u3f1e8791-536d-4&from=ui&id=ufb33c78f&originHeight=571&originWidth=1080&originalType=binary&size=64578&status=done&style=shadow&taskId=u80135b85-18da-4f0c-928c-2f3b8735a70)
<a name="QtyAn"></a>
### 交叉验证可视化
交叉验证是将训练数据再次分配，以5折为例，就是说将交叉数据分成五份，每次都选取不同的数据作为验证数据。每一组不同的验证数据都会得出一个准确度，求得五组准确度的平均值，就是某个参数情况下的准确度。<br />Plotly可以使用Scikit-learn的LassoCV绘制交叉验证结果中各种惩罚值的结果。
```python
from sklearn.linear_model import LassoCV
N_FOLD = 6
# 数据准备
X = df.loc[:,['open', 'close', 'Open_Close', 
              'High_Low', 'volume', 
              'Increase_Decrease']]
X = pd.get_dummies(X, columns=['Increase_Decrease'], 
                   prefix_sep='=')
y = df['Returns']
# 模型训练
model = LassoCV(cv=N_FOLD, normalize=True)
model.fit(X, y)
mean_alphas = model.mse_path_.mean(axis=-1)
# 绘制交叉验证均方误差曲线
fig = go.Figure([
    go.Scatter(
        x=model.alphas_, y=model.mse_path_[:, i],
        name=f"Fold: {i+1}", opacity=.5, 
       line=dict(dash='dash'),
        hovertemplate="alpha: %{x} <br>MSE: %{y}")
    for i in range(N_FOLD)])
# 添加交叉验证的平均均方误差
fig.add_traces(go.Scatter(
    x=model.alphas_, y=mean_alphas,
    name='Mean', line=dict(color='black', width=3),
    hovertemplate="alpha: %{x} <br>MSE: %{y}",))
fig.show()

```
![2021-05-29-14-10-31-773258.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269521375-f8df7ecc-374d-42ea-83bb-126bbbe3d28e.png#clientId=u3f1e8791-536d-4&from=ui&id=u08b72588&originHeight=528&originWidth=1080&originalType=binary&size=80992&status=done&style=shadow&taskId=uec8f604c-11c9-4bd7-a67c-cfd364b6d18)
<a name="FEbww"></a>
### 基于决策树的网格搜索可视化
Scikit-learn机器学习中的GridSearchCV，即GridSearch和CV，网格搜索和交叉验证。<br />网格搜索，搜索的是参数，即在指定的参数范围内，按步长依次调整参数，利用调整的参数训练学习器，从所有的参数中找到在验证集上精度最高的参数，这其实是一个训练和比较的过程。<br />Plotly中运用`px.density_heatmap` 和 `px.box`，在DecisionTreeRegressor上将网格搜索过程可视化。
<a name="K81aG"></a>
#### 网格搜索调参
```python
from sklearn.model_selection import GridSearchCV
from sklearn.tree import DecisionTreeRegressor

N_FOLD = 6
X = df[['open', 'volume']]
y = df['close']
# 定义与训练网格搜索
model = DecisionTreeRegressor()
param_grid = {
    'criterion': ['mse', 'friedman_mse', 'mae'],
    'max_depth': range(2, 5)}
grid = GridSearchCV(model, param_grid, cv=N_FOLD)
grid.fit(X, y)
grid_df = pd.DataFrame(grid.cv_results_)
# 将网格的宽格式转换为长格式
```
![2021-05-29-14-10-31-859023.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269532950-04f8a195-bdaf-453a-94d4-a1ad2d84178c.png#clientId=u3f1e8791-536d-4&from=ui&id=ub783b4c8&originHeight=293&originWidth=1080&originalType=binary&size=52075&status=done&style=shadow&taskId=u431c49bc-b70a-41a3-a1f5-3e859e86f1a)
<a name="Dt4mb"></a>
#### 单个函数调用来绘制每个图形
第一个图显示了如何在单个分割(使用facet分组)上可视化每个模型参数的分数。<br />每个大块代表不同数据分割下，不同网格参数的R方和。而其中每个小块代表相同数据分割下，网格参数：`'criterion'`与`'max_depth'`在不同取值组合下的R方和。
```python
fig_hmap = px.density_heatmap(
    melted, x="max_depth", y='criterion',
    histfunc="sum", z="r_squared",
    title='Grid search results on individual fold',
    hover_data=['mean_fit_time'],
    facet_col="cv_split", facet_col_wrap=3,
    labels={'mean_test_score': "mean_r_squared"})
fig_hmap.show()
```
![2021-05-29-14-10-31-984719.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269546416-e2d0dfea-37ca-4189-a36c-ee1849521892.png#clientId=u3f1e8791-536d-4&from=ui&id=u8b39f88e&originHeight=570&originWidth=1080&originalType=binary&size=66629&status=done&style=shadow&taskId=u60375794-59b0-4b61-8ae8-3cbacf109be)<br />第二个图汇总了所有分割的结果，每个盒子代表一个单一的模型。三组盒子代表三个不同的树深度`'max_depth'`，每组中不同颜色的盒子代表不同的评价标准`'criterion'`。
```python
fig_box = px.box(
    melted, x='max_depth', y='r_squared',
    title='Grid search results ',
    hover_data=['mean_fit_time'],
    points='all',
    color="criterion",
    hover_name='cv_split',
    labels={'mean_test_score': "mean_r_squared"})
fig_box.show()
```
![2021-05-29-14-10-32-061482.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269558741-84412aea-6c41-417e-af18-8fdbe174bf9c.png#clientId=u3f1e8791-536d-4&from=ui&id=u1576e906&originHeight=607&originWidth=1080&originalType=binary&size=49224&status=done&style=shadow&taskId=uc008849c-b406-400d-8230-c00e0172e40)
<a name="w0kGe"></a>
### KNN分类可视化
训练一个 K-Nearest Neighbors 分类器，首先模型记录每个训练样本的标签。然后每当给它一个新样本时，它就会从训练集中找k个最接近的样本来找到对应的标签，然后做投票，看看这个区域内，哪个类别标签数量多，以确定标签值并把它赋给新样本。<br />在图中，将所有负标签显示为正方形，正标签显示为圆形。通过在测试数据中心添加一个点来区分训练集和测试集。<br />![2021-05-29-14-10-32-240003.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269572135-393c0061-d658-4e9d-a7a1-9301241a2346.png#clientId=u3f1e8791-536d-4&from=ui&id=u121bc615&originHeight=436&originWidth=1080&originalType=binary&size=124836&status=done&style=shadow&taskId=u35cbacc5-2d16-4628-b4fd-b0d097601e8)<br />通过plotly中的`dash`还可以绘制交互图，不同参数下不同的决策边界，无疑给我们理解模型提供了一个很好的帮手。具体绘图过程可以到官网查看，这里不做过多的介绍。<br />![2021-05-29-14-10-32-526268.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622269584567-9983ffed-370b-447d-9beb-5b223bd8b139.gif#clientId=u3f1e8791-536d-4&from=ui&id=ua3216d0e&originHeight=635&originWidth=639&originalType=binary&size=247065&status=done&style=shadow&taskId=u4ffe28b3-3ccb-455a-8fe4-7e77cbea393)
<a name="I2Plp"></a>
### 模型评价可视化
这里的模型评价主要针对分类模型，回归模型用拟合误差、拟合残差等可以评价回归模型的优劣，前面已经介绍过了。此处主要是将模型的预测概率、模型效果可视化，如假正率真正率曲线图、绘制ROC曲线图等。
<a name="Ptv7v"></a>
#### 与真实标签相比的分数直方图
```python
from sklearn.metrics import roc_curve, auc
# 二分类逻辑回归建模
model = LogisticRegression()
model.fit(X, y)
# 模型预测概率
y_score = model.predict_proba(X)[:, 1]
# 绘制预测概率直方图
fig_hist = px.histogram(
    x=y_score, color=y, nbins=30,
    labels=dict(color='True Labels', x='Score'))
fig_hist.show()
```
![2021-05-29-14-10-32-610015.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269600504-ea618212-b71b-4d18-b9e4-70480e490424.png#clientId=u3f1e8791-536d-4&from=ui&id=uc49ec8f8&originHeight=595&originWidth=1080&originalType=binary&size=31175&status=done&style=shadow&taskId=uc2cedc8a-4727-4559-9602-1d234784957)
<a name="Kvp1l"></a>
#### 在不同的阈值下评估模型性能
```python
# 计算ROC曲线各个值
fpr, tpr, thresholds = roc_curve(y, y_score)
# 建立阈值数据框
df = pd.DataFrame({
    'False Positive Rate': fpr,
    'True Positive Rate': tpr}
      , index=thresholds)
df.index.name = "Thresholds"
df.columns.name = "Rate"
# 绘制折线图
fig_thresh = px.line(
    df, title='TPR and FPR at every threshold',
    width=500, height=500)
# 设置x/y轴
fig_thresh.update_yaxes(scaleanchor="x", scaleratio=1)
fig_thresh.update_xaxes(range=[0.2, 1], constrain='domain')
fig_thresh.show()
```
![2021-05-29-14-10-32-756065.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269614418-43e54d00-670b-473d-8950-26dbe201e482.png#clientId=u3f1e8791-536d-4&from=ui&id=uf4f692c7&originHeight=784&originWidth=1042&originalType=binary&size=53694&status=done&style=shadow&taskId=uf0390274-ab63-425a-9291-edfe8b218db)
```python
# 绘制面积图
fig = px.area(
    x=fpr, y=tpr,
    title=f'ROC Curve (AUC={auc(fpr, tpr):.4f})',
    labels=dict(x='False Positive Rate', 
                y='True Positive Rate'),
    width=700, height=500)
# 添加理论线 黑色虚线
fig.add_shape(
    type='line', line=dict(dash='dash'),
    x0=0, x1=1, y0=0, y1=1)
# 更新图表样式
fig.update_yaxes(scaleanchor="x", scaleratio=1)
fig.update_xaxes(constrain='domain')
fig.show()
```
![2021-05-29-14-10-32-931597.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622269638213-f5b85755-9d1f-466f-9dd4-2e8862ca01e4.png#clientId=u3f1e8791-536d-4&from=ui&id=ucc782bbf&originHeight=644&originWidth=1080&originalType=binary&size=66599&status=done&style=shadow&taskId=u10789b0a-a152-4dd2-a824-f7da33b19de)

