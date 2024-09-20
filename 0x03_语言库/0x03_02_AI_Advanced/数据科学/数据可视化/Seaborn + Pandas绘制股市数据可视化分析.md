Python Seaborn Pandas<br />![2021-05-21-14-40-57-113725.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579600688-d421ac93-92e7-4a5d-a830-641f2340fc25.png#clientId=ub7787f62-84e7-4&from=ui&id=u6d7ddf5e&originHeight=472&originWidth=1080&originalType=binary&ratio=1&size=75091&status=done&style=shadow&taskId=u4d164ca2-8d3c-486a-bae7-acca9bbb539)<br />在日常生活中，可视化技术常常是优先选择的方法。尽管在大多数技术学科（包括数据挖掘）中通常强调算法或数学方法，但是可视化技术也能在数据分析方面起到关键性作用。
<a name="SABsn"></a>
## 导入相关模块
```python
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set(style="white", color_codes=True)
```
<a name="NavXJ"></a>
## 数据准备
![2021-05-21-14-40-57-259312.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579616294-3d754518-dd88-4172-8963-5be9035b75dd.png#clientId=ub7787f62-84e7-4&from=ui&id=u7198b26d&originHeight=369&originWidth=1080&originalType=binary&ratio=1&size=42095&status=done&style=shadow&taskId=u1e596266-5cc2-4a59-8403-14f8ec45444)
<a name="oztz8"></a>
### 折线图看趋势
折线图在股市中地位是不可撼动的，折线图即股票走势图也就是K线图，是股民们分析股市历史数据即走势的重要图形，通常分为，日、周、月、季、年K线图。
<a name="vlfH3"></a>
#### 单条折线
```python
fig, ax = plt.subplots()
fig.set_size_inches(12, 8)
sns.lineplot(dataset.index ,
             dataset['Adj Close'])
```
![2021-05-21-14-40-57-443422.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579635651-acb1937b-1c76-4218-b6b4-7b7fc012a9eb.png#clientId=ub7787f62-84e7-4&from=ui&id=uf7d6d042&originHeight=482&originWidth=725&originalType=binary&ratio=1&size=31065&status=done&style=shadow&taskId=ud309e49a-a48c-4445-9548-7fc3f4b08ee)
<a name="K0iGF"></a>
#### 双折线
```python
sns.lineplot(x=dataset.index, 
             y="Adj Close", 
             hue="Increase_Decrease", data=dataset)
```
![2021-05-21-14-40-57-608114.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579651562-09516dad-fbeb-4919-9675-965ea298ee31.png#clientId=ub7787f62-84e7-4&from=ui&id=u0bf78f01&originHeight=591&originWidth=837&originalType=binary&ratio=1&size=49581&status=done&style=shadow&taskId=ued7b402d-509f-4894-b956-048c6f7957a)
<a name="qIW0h"></a>
#### 多条折线
```python
# Plot Multi lines
sns.lineplot(data=dataset[
  ['Open', 'Low', 'High', 'Close']])
```
![2021-05-21-14-40-57-744748.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579657884-7e7dec17-372b-4e53-b4de-9cf70d3ed0b8.png#clientId=ub7787f62-84e7-4&from=ui&id=u44683743&originHeight=482&originWidth=711&originalType=binary&ratio=1&size=45719&status=done&style=shadow&taskId=u805ef796-dd6c-4f16-9ee7-b0e26a9017a)
<a name="DtREy"></a>
### 计数直方图做对比
```python
dataset['Increase_Decrease'].value_counts()

Decrease    812
Increase    759
Name: Increase_Decrease, dtype: int64
```
计数直方图会自动聚合求和。可以用以比较各个不同阶段成交量或成交金额的有力工具。
```python
sns.countplot(
        dataset['Increase_Decrease'],
        label="Count")   
```
![2021-05-21-14-40-57-860437.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579666697-dbc22581-4f61-4e10-98bb-9f08cd767085.png#clientId=ub7787f62-84e7-4&from=ui&id=ufd8db9c7&originHeight=374&originWidth=725&originalType=binary&ratio=1&size=4950&status=done&style=shadow&taskId=ue071ac2b-2ecf-4de1-ab96-4f83d9c92d9)
<a name="fZQya"></a>
### 散点图看相关性
散点图表示因变量（Y轴数值）随自变量（X轴数值）变化的大致趋势，从而选择合适的函数对数据点进行拟合；散点图中包含的数据越多，比较的效果也越好。<br />可以使用散点图提供关键信息：<br />1、变量之间是否存在数量关联趋势；<br />2、如果存在关联趋势，是线性还是曲线的；<br />3、如果有某一个点或者某几个点偏离大多数点，也就是离群值，通过散点图可以一目了然。从而可以进一步分析这些离群值是否可能在建模分析中对总体产生很大影响。
```python
# Scatterplot
dataset.plot(kind="scatter", 
             x="Open", 
             y="Close",
             figsize=(10,8))
```
![2021-05-21-14-40-57-994084.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579676773-2940b8e7-4193-4959-8ea4-a72590d161a7.png#clientId=ub7787f62-84e7-4&from=ui&id=u007d8e6e&originHeight=482&originWidth=614&originalType=binary&ratio=1&size=18119&status=done&style=shadow&taskId=u1ad852e0-4225-44b0-abd2-a1818de83e8)
<a name="tt4H8"></a>
### 二元散点图和一元直方图
用 `sns.jointplot` 可以同时看到两个变量的联合分布与单变量的独立分布。
```python
grid=sns.jointplot(x="Open", 
                   y="Close", 
                   data=dataset, 
                   size=5)
grid.fig.set_figwidth(8)
grid.fig.set_figheight(8)
```
![2021-05-21-14-40-58-136697.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579686011-33c0209b-989c-45b1-bbb6-c7347da297f9.png#clientId=ub7787f62-84e7-4&from=ui&id=ucab68f15&originHeight=528&originWidth=528&originalType=binary&ratio=1&size=24722&status=done&style=shadow&taskId=uca7fed83-2048-4c49-8d1b-4e300309e5c)<br />联合分布图也可以自动进行 KDE 和回归。
```python
sns.jointplot(dataset.loc[:,'Open'], 
              dataset.loc[:,'Close'], 
              kind="reg", 
              color="#ce1414")
```
![2021-05-21-14-40-58-290288.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579695247-4c98e659-a045-4595-9960-c01ecdd8d4bf.png#clientId=ub7787f62-84e7-4&from=ui&id=uf008ec9f&originHeight=544&originWidth=545&originalType=binary&ratio=1&size=32428&status=done&style=shadow&taskId=u335b6646-7081-4bb7-99cd-e762afabb8d)
<a name="vbCxB"></a>
### 结构化多绘图网格
当您想要在数据集的子集中分别可视化变量的分布或多个变量之间的关系时，FacetGrid类非常有用。一个FacetGrid可以与多达三个维度可以得出：`row`，`col`，`和hue`。前两个与得到的轴阵列有明显的对应关系; 将色调变量视为沿深度轴的第三个维度，其中不同的级别用不同的颜色绘制。<br />基本工作流程是FacetGrid使用数据集和用于构造网格的变量初始化对象。然后，可以通过调用`FacetGrid.map()`或将一个或多个绘图函数应用于每个子集 `FacetGrid.map_dataframe()`。最后，可以使用其他方法调整绘图，以执行更改轴标签，使用不同刻度或添加图例等操作。
```python
grid = sns.FacetGrid(dataset,
                      col='Buy_Sell', 
                      hue="Increase_Decrease",
                      size=5) \
   .map(plt.scatter, "Open", "Close") \
   .add_legend()
grid.fig.set_figwidth(15)
grid.fig.set_figheight(6)
```
![2021-05-21-14-40-58-415951.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579705761-1d105382-17e3-432e-9906-c0fbaeacc404.png#clientId=ub7787f62-84e7-4&from=ui&id=u664c9fa0&originHeight=410&originWidth=1051&originalType=binary&ratio=1&size=26088&status=done&style=shadow&taskId=u5d7f7bd0-7d56-4843-8dba-b48c1f8fb57)
```python
grid = sns.FacetGrid(dataset, 
                     col='Buy_Sell',
                     row='Buy_Sell_on_Open', 
                     hue="Increase_Decrease", 
                     size=6)
grid.map(sns.kdeplot, "Close") 
grid.add_legend()
```
![2021-05-21-14-40-58-561561.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579714321-d6f21349-2a7f-4c78-8dac-666004e0a2a2.png#clientId=ub7787f62-84e7-4&from=ui&id=ubd79ead9&originHeight=856&originWidth=968&originalType=binary&ratio=1&size=93849&status=done&style=shadow&taskId=uf53c6a54-25ae-40dc-893f-e24ab9ba2a1)
<a name="JSdJx"></a>
### 箱图可看离群值
```python
# Boxplot
sns.boxplot(x='Increase_Decrease', 
            y=dataset['Close'], 
            data=dataset)
```
![2021-05-21-14-40-58-667279.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579731839-5893d823-b310-4527-a5f5-e2ff8aaabe64.png#clientId=ub7787f62-84e7-4&from=ui&id=ubf9c045e&originHeight=265&originWidth=391&originalType=binary&ratio=1&size=3789&status=done&style=shadow&taskId=ud331c090-2a45-40d8-b47d-960baab42bc)
<a name="YqLT6"></a>
### 分类散点图
按照不同类别对样本数据进行分布散点图绘制。
```python
ax = sns.boxplot(x='Increase_Decrease', 
                  y=dataset['Close'], 
                  data=dataset)
ax = sns.stripplot(x='Increase_Decrease', 
# 按照x轴类别进行绘制
                   y=dataset['Close'], 
                   data=dataset, 
                   jitter=True, 
# 当数据重合较多时，用该参数做一些调整，
# 也可以设置间距如，jitter = 0.1
                   edgecolor="gray")
# 可以通过hue参数对散点图中的数值进行分类
```
![2021-05-21-14-40-58-799923.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579744713-cb8f97ba-cfe8-4c35-9c0c-23781415ad90.png#clientId=ub7787f62-84e7-4&from=ui&id=u90e7e562&originHeight=265&originWidth=391&originalType=binary&ratio=1&size=24544&status=done&style=shadow&taskId=u619ec50d-eddb-4ee2-9ce0-cecc9e60960)
<a name="Y8LW1"></a>
### 小提琴图
小提琴图是箱线图与核密度图的结合，箱线图展示了分位数的位置，核密度图则展示了任意位置的密度，通过小提琴图可以知道哪些位置的数据点聚集的较多，因其形似小提琴而得名。<br />其外围的曲线宽度代表数据点分布的密度，中间的箱线图则和普通箱线图表征的意义是一样的，代表着中位数、上下分位数、极差等。细线代表  置信区间。<br />当使用带有两种颜色的变量时，将`split`设置为 `True` 则会为每种颜色绘制对应半边小提琴。从而可以更容易直接的比较分布。
```python
sns.violinplot(x='Increase_Decrease', 
               y=dataset['Volume'], 
               hue='Buy_Sell', 
               split=True,
               data=dataset, 
               size=6)
```
![2021-05-21-14-40-58-922595.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579758115-0bd6197c-b809-49cc-90b4-39a447603247.png#clientId=ub7787f62-84e7-4&from=ui&id=u339aa5a8&originHeight=276&originWidth=388&originalType=binary&ratio=1&size=15076&status=done&style=shadow&taskId=uf2f3ee6c-117f-4f01-bfbd-afde2e80825)
<a name="u0esC"></a>
### 热力图
热力图在实际中常用于展示一组变量的相关系数矩阵，在展示列联表的数据分布上也有较大的用途，通过热力图可以非常直观地感受到数值大小的差异状况。
```python
sns.heatmap(dataset[['Open', 'High', 'Low', 'Adj Close',                      'Volume', 'Returns']].corr(), 
            annot=True, 
            linewidths=.5, 
            fmt= '.3f')
```
![2021-05-21-14-40-59-042277.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579766138-aa46af76-b9a5-4372-9700-d083c9a3101b.png#clientId=ub7787f62-84e7-4&from=ui&id=u4a5942fb&originHeight=1016&originWidth=961&originalType=binary&ratio=1&size=34759&status=done&style=shadow&taskId=ua63f20cf-b7ef-4741-bfd2-a9f8091caf3)<br />热力图的右侧是颜色带，上面代表了数值到颜色的映射，数值由小到大对应色彩由暗到亮。
<a name="kzYN3"></a>
### pairplot看特征间的关系
seaborn中pairplot函数可视化探索数据特征间的关系。<br />当需要对多维数据集进行可视化时，最终都要使用散布矩阵图**(pair plot)** 。如果想画出所有变量中任意两个变量之间的图形，用矩阵图探索多维数据不同维度间的相关性非常有效。<br />散布图有两个主要用途。其一，他们图形化地显示两个属性之间的关系。直接使用散布图，或使用变换后属性的散布图，也可以判断非线性关系。<br />其二，当类标号给出时，可以使用散布图考察两个属性将类分开的程度。意思是用一条直线或者更复杂的曲线，将两个属性定义的平面分成区域，每个区域包含一个类的大部分对象，则可能基于这对指定的属性构造精确的分类器。
```python
sns.pairplot(dataset.drop(
    ["Increase_Decrease", "Buy_Sell_on_Open", 
    "Buy_Sell"],axis=1), size=3,
             # diag_kind="kde"
            )
```
![2021-05-21-14-40-59-269669.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579779846-362b46c6-f5c8-42e3-b7e2-79b6d375ba0b.png#clientId=ub7787f62-84e7-4&from=ui&id=u0a6cf473&originHeight=1081&originWidth=1080&originalType=binary&ratio=1&size=534152&status=done&style=shadow&taskId=u7a6ab8d2-6b56-4e69-b2a2-151b6c7f14c)
<a name="EIYgK"></a>
### PairGrid 成对关系子图
子图网格，用于在数据集中绘制成对关系。<br />此类将数据集中的每个变量映射到多轴网格中的列和行。可以使用不同的`axes-level`绘图函数在上三角形和下三角形中绘制双变量图，并且每个变量的边际分布可以显示在对角线上。<br />它还可以使用`hue`参数表示条件化的附加级别，该参数以不同的颜色绘制不同的数据子集。这使用颜色来解析第三维上的元素，但仅在彼此之上绘制子集，而不会像`axes-level`函数接受色相那样为特定的可视化效果定制色相参数。
```python
sns.set(style="white")
df = dataset.loc[:,['Open','High','Low']]
g = sns.PairGrid(df, diag_sharey=False)
g.map_lower(sns.kdeplot, cmap="Blues_d")
g.map_upper(plt.scatter)
g.map_diag(sns.kdeplot, lw=3)
```
![2021-05-21-14-40-59-392342.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579793844-0758bfc4-e485-492e-b9e5-46751a030159.png#clientId=ub7787f62-84e7-4&from=ui&id=u48cb9f6f&originHeight=531&originWidth=526&originalType=binary&ratio=1&size=36461&status=done&style=shadow&taskId=u0cd9e07c-7ff6-4e05-9cbc-cdf4b45d74b)
```python
dataset.boxplot(by="Increase_Decrease", figsize=(12, 6))
```
![2021-05-21-14-40-59-512018.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579804017-8657eadc-f130-4547-b561-872b2bdb3d8d.png#clientId=ub7787f62-84e7-4&from=ui&id=ua49a8746&originHeight=406&originWidth=721&originalType=binary&ratio=1&size=9472&status=done&style=shadow&taskId=uad244f12-15e5-4bb9-bc00-bc95bd2da7e)
<a name="zsDai"></a>
## pandas绘图
<a name="mN9tT"></a>
### 条形图
```python
dataset['Volume'].resample('Y').mean().plot.bar()
```
![2021-05-21-14-40-59-699518.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579813348-0c98449d-2fbb-4e21-a989-90e4b6baab3e.png#clientId=ub7787f62-84e7-4&from=ui&id=u8ef2f6a2&originHeight=368&originWidth=364&originalType=binary&ratio=1&size=4485&status=done&style=shadow&taskId=ufabb4a45-5823-4efe-af3a-fda77e79911)<br />[pandas可视化](https://pandas.pydata.org/pandas-docs/stable/user_guide/visualization.html)中，可以使用Series和DataFrame上的`plot`方法，它只是一个简单的包装器 `plt.plot()`，另外还有一些有几个绘图功能在`pandas.plotting` 内。
<a name="WLsiI"></a>
### 安德鲁斯曲线
[安德鲁斯曲线](https://en.wikipedia.org/wiki/Andrews_plot)允许将多元数据绘制为大量曲线，这些曲线是使用样本的属性作为傅里叶级数的系数而创建的。通过为每个类别对这些曲线进行不同的着色，可以可视化数据聚类。属于同一类别的样本的曲线通常会更靠近在一起并形成较大的结构。
```python
from pandas.plotting import andrews_curves
andrews_curves(dataset[
    ['Open', 'Close', 'Increase_Decrease']],
        "Increase_Decrease")
```
![2021-05-21-14-41-00-020656.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579826785-a54c26e9-10c0-471a-911c-7e9a464ef201.png#clientId=ub7787f62-84e7-4&from=ui&id=u8d2d5fdf&originHeight=251&originWidth=377&originalType=binary&ratio=1&size=64755&status=done&style=shadow&taskId=ufb8638d6-f9e1-417a-bdb3-bd44301df7a)
<a name="MqLST"></a>
### 平行坐标
[平行坐标](https://en.wikipedia.org/wiki/Parallel_coordinates )是一种用于绘制多元数据的绘制技术 。平行坐标允许人们查看数据中的聚类，并直观地估计其他统计信息。使用平行坐标点表示为连接的线段。每条垂直线代表一个属性。一组连接的线段代表一个数据点。趋于聚集的点将显得更靠近。
```python
from pandas.plotting import parallel_coordinates
parallel_coordinates(dataset[
    ['Open', 'High', 'Low', 'Increase_Decrease']], 
        "Increase_Decrease")
```
![2021-05-21-14-41-00-268992.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579837222-3a8c4463-30cf-4957-8576-ae89c2c2e660.png#clientId=ub7787f62-84e7-4&from=ui&id=u6f903667&originHeight=251&originWidth=387&originalType=binary&ratio=1&size=43905&status=done&style=shadow&taskId=u1ebaf045-b21c-4ada-86c1-f1ac5fb736a)
<a name="F0lxG"></a>
### 径向坐标可视化
RadViz是一种可视化多变量数据的方法。它基于简单的弹簧张力最小化算法。基本上，在平面上设置了一堆点。在当前情况下，它们在单位圆上等距分布。每个点代表一个属性。然后，假设数据集中的每个样本都通过弹簧连接到这些点中的每个点，弹簧的刚度与该属性的数值成正比（将它们标准化为单位间隔）。样本在平面上的沉降点（作用在样本上的力处于平衡状态）是绘制代表样本的点的位置。根据样本所属的类别，其颜色会有所不同。
```python
from pandas.plotting import radviz

radviz(dataset[
    ['Open','High', 'Low', 'Close', 'Increase_Decrease']], 
      "Increase_Decrease")
```
![2021-05-21-14-41-00-377701.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579851187-185ca168-4704-4e19-92a6-224f2175e1d2.png#clientId=ub7787f62-84e7-4&from=ui&id=ufe666c96&originHeight=251&originWidth=380&originalType=binary&ratio=1&size=3874&status=done&style=shadow&taskId=u7f249d4c-c506-46f0-86b6-cb2b16c17b1)
<a name="HyCta"></a>
### 滞后图
滞后图用于检查数据集或时间序列是否随机。随机数据在滞后图中不应显示任何结构。非随机结构意味着基础数据不是随机的。该`lag`参数可以传递，而当`lag=1`时基本上是`data[:-1]`对 `data[1:]`。
```python
from pandas.plotting import lag_plot
lag_plot(dataset['Volume'].tail(250))
```
![2021-05-21-14-41-00-478430.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579860536-37369033-b16d-474b-af54-164d53b686f6.png#clientId=ub7787f62-84e7-4&from=ui&id=udc634cfe&originHeight=276&originWidth=388&originalType=binary&ratio=1&size=5408&status=done&style=shadow&taskId=u226e6f3d-af4f-4f6c-a0c5-950018990cc)
<a name="A2zx2"></a>
### 自相关图
自相关图通常用于检查时间序列中的随机性。通过在变化的时滞中计算数据值的自相关来完成此操作。如果时间序列是随机的，则对于任何和所有时滞间隔，此类自相关应接近零。如果时间序列不是随机的，则一个或多个自相关将明显为非零。图中显示的水平线对应于95％和99％的置信带。虚线是99％置信带。
```python
from pandas.plotting import autocorrelation_plot
autocorrelation_plot(dataset['Volume'])
```
![2021-05-21-14-41-00-590132.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621579868271-33165749-3db1-4652-88e8-265387cecf1c.png#clientId=ub7787f62-84e7-4&from=ui&id=uac04b4f2&originHeight=269&originWidth=400&originalType=binary&ratio=1&size=16096&status=done&style=shadow&taskId=ue175c926-ed2c-4448-b0f3-fb1d883ad04)
