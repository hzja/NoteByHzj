Python 数据特征 <br />![2021-05-12-19-05-20-813987.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620817868565-bea49d95-a54f-4c5c-9087-d650d610f85b.png#clientId=u1e03d214-7e12-4&from=ui&id=u85a3c035&originHeight=613&originWidth=1080&originalType=binary&size=118742&status=done&style=shadow&taskId=u3768e804-a01b-4053-b153-0da3c210344)
<a name="EPV3z"></a>
## 准备
数据可视化是指以图形或表格的形式显示信息。成功的可视化需要将数据或信息转换成可视的形式，以便能够借此分析或报告数据的特征和数据项或属性之间的关系。可视化的目标是形成可视化信息的人工解释和信息的意境模型。
<a name="d3IJY"></a>
### 导入相关模块
```python
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import yfinance as yf
yf.pdr_override()
```
<a name="efogg"></a>
### 导入数据
```python
symbol = 'TCEHY'
start = '2015-01-01'
end = '2021-03-31'
# 获取数据
dataset = yf.download(symbol,start,end)
# 查看列
dataset.head()
```
![2021-05-12-19-05-20-873793.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620817921822-55fe5443-221c-490e-bff2-ed7aeebfa171.png#clientId=u1e03d214-7e12-4&from=ui&id=ua084b77c&originHeight=390&originWidth=808&originalType=binary&size=28959&status=done&style=shadow&taskId=u8ea67c5a-0fc0-4579-9a41-801772b25f9)
<a name="eoWWd"></a>
### 特征构造
```python
dataset['Increase/Decrease'] = np.where(dataset['Volume'].shift(-1) > dataset['Volume'],1,0)
dataset['Buy_Sell_on_Open'] = np.where(dataset['Open'].shift(-1) > dataset['Open'],1,-1)
dataset['Buy_Sell'] = np.where(dataset['Adj Close'].shift(-1) > dataset['Adj Close'],1,-1)
dataset['Return'] = dataset['Adj Close'].pct_change()
dataset = dataset.dropna()
dataset['Class'] = np.where(dataset['Return'].shift(-1) > dataset['Return'],'Increase','Decrease')
```
`pct_change()`表示当前元素与先前元素的相差百分比，当指定`periods=n`，表示当前元素与先前n个元素的相差百分比。
<a name="tch63"></a>
### 探索性数分析几种方法
```python
print("Exploratory Data Analysis")
print("Dataset information") 
print(dataset.info(memory_usage='deep',verbose=False))
print(dataset.info())
print("Data type:")
print(dataset.dtypes)
print("Check unique values without NaN")
print(dataset.nunique())
print("Data shape:")
print(dataset.shape)
print("Data columns Names:")
print(dataset.columns)
print("Check for NaNs:")
print(dataset.isnull().values.any())
print("How many NaN it has in each columns?")
print(dataset.isnull().sum())
print("Data Statistics Summary:")
print(dataset.describe())
```
<a name="RaEbV"></a>
### 描述性统计
```python
>>> dataset.describe().T
```
![2021-05-12-19-05-20-998461.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820343014-2a9307a7-ec75-4e25-9638-0a065a1c07d0.png#clientId=u1e03d214-7e12-4&from=ui&id=ub36c678b&originHeight=385&originWidth=1080&originalType=binary&size=84182&status=done&style=none&taskId=ufb95200d-6aee-4830-a3f7-bb93257905c)
<a name="XmeKo"></a>
## 可视化探索数据特征
<a name="KE6YQ"></a>
### 获取特征与标签
```python
X = dataset[['Open','High','Low','Adj Close']]
Y = dataset.index
```
<a name="ZcRzV"></a>
### 折线图探索趋势
```python
plt.figure(figsize=(10,8))
plt.plot(X)
plt.title('Stock Line Chart')
plt.legend(X)
```
![2021-05-12-19-05-21-065285.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820417031-cc676ac4-9a37-404e-810c-40173c29f595.png#clientId=u1e03d214-7e12-4&from=ui&id=u092d9c3d&originHeight=482&originWidth=877&originalType=binary&size=46690&status=done&style=shadow&taskId=u47dee7fd-b8b3-49ef-b6c8-34fabaa422f)
<a name="P6jLn"></a>
### 散点图探索相关性
散点图使用数据对象两个属性对值作为x和y坐标轴，每个数据对象都作为平面上对一个点绘制。
```python
plt.figure(figsize=(10,8))
plt.scatter(dataset['High'], dataset['Low'])
plt.title("High vs. Low", fontsize=20)
plt.xlabel("High", fontsize=20)
plt.ylabel("Low", fontsize=20)
```
![2021-05-12-19-05-21-126155.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820427511-55ae983a-aea8-4b09-a578-ce7cb2cdaf5d.png#clientId=u1e03d214-7e12-4&from=ui&id=u848f1215&originHeight=510&originWidth=900&originalType=binary&size=17618&status=done&style=shadow&taskId=u60e54629-84f4-4baa-bfe6-d8786c8be2f)
<a name="GY9b8"></a>
### 直方图探索分布
直方图是数值数据分布的精确图形表示。直方图通过将可能的值分散到箱中，并显示落入每个箱中到对象数，显示属性值到分布。<br />对于分类属性，每个值在一个箱中，如果值过多，则使用某种方法将值合并。对于连续属性，将值域划分成箱（通常是等宽）并对每个箱中对值计数。<br />一旦有了每个箱对计数，就可以构造条形图，每个箱用一个条形表示，并且每个条形对面积正比于落在对应区间对值对个数。如果所有对区间都是等宽对，则所有对条形对宽度相同，并且条形对高度正比于落在对应箱中值对个数。
<a name="Q9Wed"></a>
#### matplotlib直方图
```python
plt.figure(figsize=(10,8))
plt.hist(dataset['Adj Close'], bins = 50)
plt.xlabel("Adj Close")
plt.ylabel("Frequency")
```
![2021-05-12-19-05-21-194936.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820438249-0ba009a4-8870-47bf-95cc-d993d3e3fc41.png#clientId=u1e03d214-7e12-4&from=ui&id=uf72487f6&originHeight=480&originWidth=891&originalType=binary&size=6149&status=done&style=shadow&taskId=u6bcf3b65-82ca-47a0-911e-37f71aae80c)
<a name="zc1bn"></a>
#### Seaborn直方图
```python
sns.distplot(X['Return'],
          # 不显示核密度估计图
             kde = False) 
```
![2021-05-12-19-05-21-253781.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820471838-04763cbb-b4c0-462a-aae3-955ef968ae51.png#clientId=u1e03d214-7e12-4&from=ui&id=udd8ffbd9&originHeight=532&originWidth=1080&originalType=binary&size=24564&status=done&style=shadow&taskId=uf8867eb5-2be4-4f41-baaf-d886eac46de)
<a name="ylbu1"></a>
### 条形图探索大小
条形图（bar chart），也称为柱状图，是一种以长方形的长度为变量的统计图表，长方形的长度与它所对应的变量数值呈一定比例。
```python
plt.figure(figsize=(10,8))
labels = ['Open','High','Low','Adj Close']
y_positions = range(len(labels))
plt.bar(y_positions, X.iloc[0])
plt.xticks(y_positions, labels)
plt.ylabel("Price")
plt.title("Prices Columns")
```
![2021-05-12-19-05-21-945379.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820486224-d4575f2c-21c0-4e46-9255-dbfc50bea159.png#clientId=u1e03d214-7e12-4&from=ui&id=u3d7d69d7&originHeight=477&originWidth=1080&originalType=binary&size=23792&status=done&style=shadow&taskId=uf462762d-acd9-456f-a803-5ef0a1b8d2c)
<a name="R7lMZ"></a>
### 饼图探索比例
饼图直接以图形的方式直观形象地显示各个组成部分所占比例。<br />饼图类似于直方图，但通常用于具有相对较少的值的分类属性。饼图使用圆但相对面积显示不同值对相对频率，而不是像直方图那样使用条形图对面积或高度。
```python
sizes = X.iloc[0]
labels = ['Open','High','Low','Adj Close']
plt.figure(figsize=(10,8))
patches,l_text,p_text=plt.pie(sizes, 
            labels = labels, 
            autopct = "%.2f")
#改变文本的大小
#方法是把每一个text遍历。调用set_size方法设置它的属性
for t in l_text:
    t.set_size(30)
for t in p_text:
    t.set_size(20)
plt.axes().set_aspect("equal")
```
![2021-05-12-19-05-22-223676.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820496589-ba81cc2d-be35-4458-86b9-91b52281662b.png#clientId=u1e03d214-7e12-4&from=ui&id=u746ef716&originHeight=459&originWidth=748&originalType=binary&size=28206&status=done&style=shadow&taskId=u5ae91d69-8835-4bda-9705-afb933f2434)
<a name="YfLYL"></a>
### 计数直方图探索类别间计数差
```python
>>> sns.countplot(x="Class", 
                  data=dataset)
>>> dataset.loc[:,'Class'].value_counts()
Decrease    790
Increase    781
Name: Class, dtype: int64
```
![2021-05-12-19-05-22-286498.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820508745-01b1baf6-acac-4c25-a300-5a27a80f7f14.png#clientId=u1e03d214-7e12-4&from=ui&id=u391b6759&originHeight=504&originWidth=1080&originalType=binary&size=25604&status=done&style=shadow&taskId=u8a6290b3-f70d-4821-998d-0eee6ac9678)
<a name="kMjIW"></a>
### 热力图探索相关性
数据分析中常用热力图做相关性分析。使用热力图表达会更加的明显地看出数据表里多个特征两两的相似度。下图表达相关性颜色越红的相关性越大，颜色越青的相关性越小。
```python
sns.set_style('white')
cmap = sns.diverging_palette(220, 10, as_cmap=True)
plt.figure(figsize=(8, 6))
sns.heatmap(dataset.iloc[:dataset.shape[0]].corr(), 
            annot = True, 
            cmap = cmap)
```
![2021-05-12-19-05-22-349355.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820523160-2bcdbfbb-e556-489e-bf6d-916acde56105.png#clientId=u1e03d214-7e12-4&from=ui&id=uc0004569&originHeight=437&originWidth=530&originalType=binary&size=45877&status=done&style=shadow&taskId=u7d71d909-5418-4663-9781-bebf17c7fd2)
<a name="NZeJ5"></a>
## 几种多子图图形
<a name="OZPL1"></a>
### 密度图探索分布
密度图是可视化连续型随机变量分布的利器，分布曲线上的每一个点都是概率密度，分布曲线下的每一段面积都是特定情况的概率。
```python
dataset.plot(kind='density', 
             subplots=True, 
             layout=(5,5), 
             sharex=False, 
             figsize=(15,10))
```
![2021-05-12-19-05-22-424128.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820618251-6647d24f-d91a-4f5a-bd71-1b325debdc3f.png#clientId=u1e03d214-7e12-4&from=ui&id=uebf271ce&originHeight=246&originWidth=897&originalType=binary&size=48652&status=done&style=shadow&taskId=u675eab0b-55ff-481d-9291-1229e18b5f7)
<a name="cj2KQ"></a>
### 盒须图探索离散分布
箱形图Box plot又称为盒须图、盒式图、盒状图或箱线图，是一种用作显示一组数据分散情况资料的统计图。因型状如箱子而得名。<br />箱形图最大的优点就是不受异常值的影响，可以以一种相对稳定的方式描述数据的离散分布情况。
```python
# 盒须图
dataset.plot(kind='box', 
             subplots=True, 
             layout=(5,5), 
             sharex=False, 
             sharey=False, 
             figsize=(10,10))
```
![2021-05-12-19-05-22-479947.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820629456-818ec23c-72f5-44c6-9749-5018edd5797c.png#clientId=u1e03d214-7e12-4&from=ui&id=u9044660f&originHeight=237&originWidth=594&originalType=binary&size=11223&status=done&style=shadow&taskId=udac2f424-ee6e-43c7-93fc-7771aa63a5e)
<a name="TfI42"></a>
### 直方图
直方图是一种对数据分布情况的图形表示，是一种二维统计图表，它的两个坐标分别是统计样本和该样本对应的某个属性的度量，以长条图（bar）的形式具体表现。因为直方图的长度及宽度很适合用来表现数量上的变化，所以较容易解读差异小的数值。<br />分组数据字段（统计结果）映射到横轴的位置，频数字段（统计结果）映射到矩形的高度，分类数据可以设置颜色增强分类的区分度。
<a name="rLYN3"></a>
#### 方法一
使用DataFrame的plot方法绘制图像会按照数据的每一列绘制一条曲线，默认按照列columns的名称在适当的位置展示图例，比matplotlib绘制节省时间，且DataFrame格式的数据更规范，方便向量化及计算。
```python
dataset.plot(kind='hist', 
             subplots=True, 
             layout=(4,4), 
             sharex=False, 
             sharey=False,
             figsize=(10,10))
```
![2021-05-12-19-05-22-744515.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820640649-0c6d069a-8169-4e06-8605-58cc62ed9332.png#clientId=u1e03d214-7e12-4&from=ui&id=uf32006af&originHeight=442&originWidth=611&originalType=binary&size=20383&status=done&style=shadow&taskId=u2350615a-48ed-4ea7-b828-ddb95b8e147)
<a name="AsGAS"></a>
#### 方法二
`DataFrame.hist`函数在DataFrame中的每个系列上调用`matplotlib.pyplot.hist()`，每列产生一个直方图。
```python
dataset.hist(figsize=(10,10))
plt.show()
```
![2021-05-12-19-05-25-892390.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820652363-fb2d6fe9-91bd-4b82-a79b-4e3243d1a480.png#clientId=u1e03d214-7e12-4&from=ui&id=u76eef787&originHeight=587&originWidth=597&originalType=binary&size=19032&status=done&style=shadow&taskId=u47db740f-0b0a-4ea7-a554-4bb5833d239)
<a name="vjXpr"></a>
### 折线图
```python
dataset.plot(kind='line', 
             subplots=True, 
             layout=(5,5), 
             sharex=False, 
             sharey=False, 
             figsize=(15,10))
plt.show()
```
![2021-05-12-19-05-26-193615.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820682124-d00ef506-4cf9-4ac4-9a7f-7471330fa608.png#clientId=u1e03d214-7e12-4&from=ui&id=u72cd1995&originHeight=248&originWidth=873&originalType=binary&size=39053&status=done&style=shadow&taskId=u1f12a89a-bfa1-4aab-8beb-61f80d8ccc7)
<a name="hSYEw"></a>
### pairplot探索特征间的关系
当你需要对多维数据集进行可视化时，最终都要使用矩阵图pair plot。如果想画出所有变量中任意两个变量之间的图形，用矩阵图探索多维数据不同维度间的相关性非常有效。
```python
sns.pairplot(dataset, 
             diag_kind='kde', 
             size=2.4)
```
![2021-05-12-19-05-27-133072.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820696372-0de4f01a-ae5a-4733-a2c5-1921b4f3d458.png#clientId=u1e03d214-7e12-4&from=ui&id=uee48c893&originHeight=1080&originWidth=1080&originalType=binary&size=437181&status=done&style=shadow&taskId=uecac5a58-6423-47d2-9706-86bbfcedda4)
<a name="GnLpj"></a>
## 几种可交互图形
<a name="pMzrp"></a>
### plotly二维可交互图
plotly和经典Matplotlib最大的不同是plotly可以生成交互式的数据图表。Matplotlib生成的图示静态的图，而plotly是动态的图，具体方式是plotly可以生成一个html网页，该网页基于js支持数据交互（点击、缩放、拖拽）等等交互操作。
```python
import plotly
import chart_studio
import chart_studio.plotly as py
import plotly.graph_objs as go
chart_studio.tools.set_credentials_file(
        username='QuantPython',
        api_key='bWmf0mKJlNViBrOjDQbE')
trace = go.Candlestick(x=dataset.index,
                       open=dataset.Open,
                       high=dataset.High,
                       low=dataset.Low,
                       close=dataset.Close)
data = [trace]
py.iplot(data, filename='simple_candlestick')
```
![2021-05-12-19-05-28-197689.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620820730515-d8e634e4-96d4-42f4-8653-8e81c345e052.gif#clientId=u1e03d214-7e12-4&from=ui&id=uaa3bebfa&originHeight=614&originWidth=1079&originalType=binary&size=1004652&status=done&style=shadow&taskId=u5229ddc1-4440-4353-8704-af84f11e165)
<a name="hYjls"></a>
### plotly三维可交互图
```python
x = dataset['Low']
y = dataset['High']
z = dataset['Adj Close']
trace1 = go.Scatter3d(
    x=x,
    y=y,
    z=z,
    mode='markers',
    marker=dict(
        size=12,
        color=z,                # 将颜色设置为所需值的数组/列表
        colorscale='YlGnBu',   
        opacity=0.8))

data = [trace1]
layout = go.Layout(margin=dict(
            l=0,r=0,
            b=0,t=0))
fig = go.Figure(data=data, layout=layout)
py.iplot(fig, filename='3d-scatter-colorscale')
```
![2021-05-12-19-05-32-039155.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620820766459-2ad8464b-65a3-43f6-9731-252281ade7b4.gif#clientId=u1e03d214-7e12-4&from=ui&id=u95e3d397&originHeight=627&originWidth=1079&originalType=binary&size=2027147&status=done&style=shadow&taskId=ub5ccd9f9-ec9b-459a-ae90-bba63946555)
<a name="yfEbQ"></a>
### bokeh二维可交互图
bokeh是一款针对现代Web浏览器呈现功能的交互式可视化库。bokeh通过Python(或其他语言)以快速简单的方式为超大型或流式数据集提供高性能交互的漂亮而且结构简单的多功能图形。
```python
from bokeh.io import show, output_notebook
from bokeh.plotting import figure
output_notebook()
# 创建一个带有标题和轴标签的新折线图
p = figure(plot_width=400, plot_height=400)
p.line(dataset.index, dataset['Adj Close'])
# show出结果
show(p)
```
![2021-05-12-19-05-32-366565.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620820783928-78abd6a8-16c0-405e-a273-9b0a9031958b.gif#clientId=u1e03d214-7e12-4&from=ui&id=u8416b752&originHeight=519&originWidth=516&originalType=binary&size=454892&status=done&style=shadow&taskId=uf711d13f-5ac4-412a-ad2d-05b44dd9a4f)
```python
# 创建一个带有标签的空白图形
p = figure(plot_width = 600, plot_height = 600, 
           title = 'Stock Prices',
           x_axis_label = 'X', y_axis_label = 'Y')

# 示例数据
squares_x = dataset['Open']
squares_y = dataset['Adj Close']
circles_x = dataset['Low']
circles_y = dataset['High']

# 添加正方形
p.square(squares_x, squares_y, size = 12, color = 'navy', alpha = 0.6)
# 添加环形
p.circle(circles_x, circles_y, size = 12, color = 'red')

# 设置输出方式
output_notebook()
show(p)
```
![2021-05-12-19-05-32-627873.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820805584-1d55ed85-9337-4caf-bb0a-a71a697ace54.png#clientId=u1e03d214-7e12-4&from=ui&id=l7gLN&originHeight=774&originWidth=1080&originalType=binary&size=75167&status=done&style=shadow&taskId=u618f7e39-03cd-4f8b-8d5e-c0b9ae27661)
<a name="mzgDT"></a>
## 几种三维绘图方法
Matplotlib创建Axes3D主要有两种方式，一种是利用关键字projection='3d'来实现，另一种则是通过从`mpl_toolkits.mplot3d`导入对象Axes3D来实现，目的都是生成具有三维格式的对象Axes3D。
```python
#方法一，利用关键字
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
#定义坐标轴
fig = plt.figure()
ax1 = plt.axes(projection='3d')
#ax = fig.add_subplot(111,projection='3d')
#这种方法也可以画多个子图
#方法二，利用三维轴方法
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
#定义图像和三维格式坐标轴
fig=plt.figure()
ax2 = Axes3D(fig)
```
使用`%matplotlib notebook`使得3d图变得可交互。
```python
# 三维散乱点数据
zdata = dataset['Adj Close']
xdata = dataset['Low']
ydata = dataset['High']
# 可以使用ax.plot3D和ax.scatter3D函数创建3d图
ax.scatter3D(xdata, ydata, zdata, c=zdata, cmap='Greens')
```
![2021-05-12-19-05-32-756526.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820865439-5dccd21c-1575-477e-9eaf-3f6afc606cf9.png#clientId=u1e03d214-7e12-4&from=ui&id=u13cc0664&originHeight=449&originWidth=456&originalType=binary&size=45307&status=done&style=shadow&taskId=u067c2d6d-bf75-42a9-ad24-bed26b0ca69)
<a name="wWVL4"></a>
### 为三维图设置样式
```python
x = dataset['Low']
y = dataset['High']
z = dataset['Adj Close']

# 创建画布
fig = plt.figure(figsize=(10,8))
ax = fig.add_subplot(111, projection='3d')
# 绘制3d图
pnt3d=ax.scatter(x,y,z,c=z)
cbar=plt.colorbar(pnt3d)

ax.set_title('3d Plot')
ax.set_xlabel('Low')
ax.set_ylabel('High')
ax.set_zlabel('Adj Close')
plt.show()
```
![2021-05-12-19-05-32-894664.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820882326-60e54b7e-8ced-44b7-97f7-024d5ae20486.png#clientId=u1e03d214-7e12-4&from=ui&id=ucd802636&originHeight=463&originWidth=517&originalType=binary&size=53066&status=done&style=shadow&taskId=udb33cae0-ef72-4403-bcc9-7517480f84d)
<a name="eSwIi"></a>
### 多个三维散点图
```python
# 创建3d画布
fig =plt.figure(figsize=(10,8))
ax = fig.add_subplot(111, projection='3d')
n = 100

# 绘制散点图
for c, m, zl, zh in [('r', 'o', -50, -25), ('b', '^', -30, -5)]:
    x = dataset['Low']
    y = dataset['High']
    z = dataset['Adj Close']
    ax.scatter(x, y, z, c=c, marker=m)

ax.set_xlabel('Low')
ax.set_ylabel('High')
ax.set_zlabel('Adj Close')

plt.show()
```
![2021-05-12-19-05-33-039248.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820893934-ffa64b48-4b42-42da-aa9a-0e3170a26886.png#clientId=u1e03d214-7e12-4&from=ui&id=u0ab1f1c5&originHeight=449&originWidth=471&originalType=binary&size=48334&status=done&style=shadow&taskId=u94fc4918-5fc8-42ac-a662-1500d85ed61)
<a name="KXx0o"></a>
### plot_trisurf绘制3d三角形图
```python
x = dataset['Low']
y = dataset['High']
z = dataset['Adj Close']
# 创建画布
fig =plt.figure(figsize=(10,8))
ax = fig.gca(projection='3d')

# 绘制3d三角形图
ax.plot_trisurf(x, y, z, cmap='autumn', linewidth=0.2)
plt.show()

ax.set_xlabel('Low')
ax.set_ylabel('High')
ax.set_zlabel('Adj Close')

plt.show()
```
![2021-05-12-19-05-33-295060.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820907461-fbf27559-c7ba-4943-af25-ee0920416f15.png#clientId=u1e03d214-7e12-4&from=ui&id=u2c120e42&originHeight=449&originWidth=452&originalType=binary&size=58877&status=done&style=shadow&taskId=u72facd45-6e73-4cbd-9998-c0468b96244)
```python
# Class 是一个分类型变量
# 将其转换为数字
df_dummies = pd.get_dummies(dataset['Class'])
del df_dummies[df_dummies.columns[-1]]
df_new = pd.concat([dataset, df_dummies], axis=1)
del df_new['Class']
df_new = df_new.rename(columns={"Decrease":"Increase_Decrease"})
df_new.head()
```
![2021-05-12-19-05-33-450612.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820920937-a1e66d2d-5ae8-4cda-a431-57a370a1661e.png#clientId=u1e03d214-7e12-4&from=ui&id=uca378284&originHeight=392&originWidth=1080&originalType=binary&size=35025&status=done&style=shadow&taskId=u68090fa1-c901-47dd-9512-4b11d0b60c2)
<a name="TpxP0"></a>
## 探索特征顺序
```python
features = dataset[['Open', 'High', 'Low', 'Volume', 
                    'Increase/Decrease', 'Buy_Sell_on_Open', 
                    'Buy_Sell', 'Return']]

X = features
y = dataset['Adj Close']
```
本文使用Yellowbrick来做特征排序。Yellowbrick是一个机器学习可视化库，主要依赖于sklearn机器学习库，能够提供多种机器学习算法的可视化，主要包括特征可视化，分类可视化，回归可视化，回归可视化，聚类可视化，模型选择可视化，目标可视化，文字可视化。<br />Yellowbrick API是专门为与scikit-learn配合使用而专门设计的。因此只需使用与scikit-learn模型相同的工作流程，导入可视化工具，实例化它，调用可视化工具的`fit()`方法，然后为了渲染可视化效果，调用可视化工具的`show()`方法。
<a name="fvDc0"></a>
### 一维显示
```python
from yellowbrick.features import Rank1D

# 用Sharpiro排序算法实例化1D可视化器
visualizer = Rank1D(algorithm='shapiro')
visualizer.fit(X, y)
# 可视化器训练数据
visualizer.transform(X)
# Draw/show/poof数据
visualizer.poof()
```
![2021-05-12-19-05-33-761050.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820935738-7fe6c10c-28ac-481f-9dc2-a4bd5d954747.png#clientId=u1e03d214-7e12-4&from=ui&id=u7a3614a4&originHeight=368&originWidth=1080&originalType=binary&size=27636&status=done&style=shadow&taskId=u7fc31c8c-b47d-400a-a839-d13a8f289f1)
<a name="Ut8Ur"></a>
### 二维显示
作为特征选择的一部分，希望识别彼此具有线性关系的特征，可能会在模型中引入协方差并破坏`OLS`（指导移除特征或使用正则化）。可以使用Rank Features visualizer计算所有特征对之间的Pearson相关性。
```python
from yellowbrick.features import Rank2D
# 用协方差排序算法实例化可视化工具
visualizer = Rank2D(algorithm='covariance')
visualizer.fit(X, y)
visualizer.transform(X)        
visualizer.poof()        
```
![2021-05-12-19-05-33-861813.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820948525-82c382a1-04e7-4f99-87d1-866c6c82eb0a.png#clientId=u1e03d214-7e12-4&from=ui&id=u10346e1d&originHeight=420&originWidth=473&originalType=binary&size=21489&status=done&style=shadow&taskId=u2d8cb645-e133-4501-9f3d-227e38b5517)<br />以上结果显示了特征对之间的皮尔逊相关性，这样网格中的每个像元都代表了两个特征，这些特征在x和y轴上按顺序标识，并且颜色显示了相关性的大小。皮尔逊相关系数为1.0表示变量对之间存在强的正线性关系，值-1.0表示强的负线性关系（零值表示无关系）。因此，可以寻找深红色和深蓝色框以进一步识别。<br />在此图表中，我们看到这些特征High与Volume具有很强的相关性。使用直接数据可视化JointPlotVisualizer检查这些关系。
```python
from yellowbrick.features import JointPlotVisualizer
visualizer = JointPlotVisualizer(columns=['High', 'Volume'])
visualizer.fit_transform(X, y)
visualizer.show()
```
![2021-05-12-19-05-34-096617.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820960435-6a483bf1-b820-4e64-85f7-0d5ec4700247.png#clientId=u1e03d214-7e12-4&from=ui&id=u149081e8&originHeight=595&originWidth=1080&originalType=binary&size=108149&status=done&style=shadow&taskId=u48cf3785-c113-4c7a-8f28-6c8a539840b)
<a name="mGSCH"></a>
## 探索特征重要性
<a name="jRGUl"></a>
### 基于Lasso回归模型
```python
from sklearn.linear_model import Lasso
from yellowbrick.features import FeatureImportances

# 创建一个新的画布
fig = plt.figure()
ax = fig.add_subplot()
# 特征标题大小写，用于更好地显示和创建可视化工具的特性
labels = list(map(lambda s: s.title(), features))
viz = FeatureImportances(
          Lasso(), 
          ax=ax, 
          labels=labels, 
          relative=False)
viz.fit(X, y)
viz.poof()
```
![2021-05-12-19-05-34-225276.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820978305-14487751-aec1-4695-90ae-2927eb9f9793.png#clientId=u1e03d214-7e12-4&from=ui&id=uf01048cc&originHeight=444&originWidth=1080&originalType=binary&size=40989&status=done&style=shadow&taskId=ufd0ac125-d6b1-4ca4-afa4-71428593f25)
<a name="QRXQM"></a>
### 基于梯度提升分类器
```python
from sklearn.ensemble import GradientBoostingClassifier
from yellowbrick.features import FeatureImportances
# 数据不能是连续型
X = dataset[['Increase/Decrease', 'Buy_Sell_on_Open', 'Buy_Sell']]
y = dataset['Buy_Sell']
# 创建一个新的画布
fig = plt.figure()
ax = fig.add_subplot()
# 特征重要性
viz = FeatureImportances(GradientBoostingClassifier(), ax=ax)
viz.fit(X, y)
viz.poof()
```
![2021-05-12-19-05-34-347945.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620820995859-a52d7542-bbc7-4e53-972b-5a613e24d9ea.png#clientId=u1e03d214-7e12-4&from=ui&id=ue212c183&originHeight=465&originWidth=1080&originalType=binary&size=15510&status=done&style=shadow&taskId=ueeb5ccad-fa55-4919-844a-300a2f71a4a)
<a name="mVjK6"></a>
## 探索分类变量
```python
X = dataset[['Open', 'High', 'Low', 'Volume', 'Increase/Decrease', 'Buy_Sell_on_Open', 'Buy_Sell', 'Return']]
Y = dataset['Adj Close']
X.dtypes
```
```python
Open                 float64
High                 float64
Low                  float64
Volume                 int64
Increase/Decrease      int32
Buy_Sell_on_Open       int32
Buy_Sell               int32
Return               float64
dtype: object
```
<a name="y4aJj"></a>
### 选择float数值型变量
```python
dataset_float = dataset.select_dtypes(include=['float']).copy()
dataset_float.head()
```
![2021-05-12-19-05-34-487577.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821008844-e4d8bdd2-1081-496f-90ab-7e78bd782000.png#clientId=u1e03d214-7e12-4&from=ui&id=ufb59d1bb&originHeight=390&originWidth=944&originalType=binary&size=32459&status=done&style=shadow&taskId=u10c03437-c96d-49b7-ae8d-0bf9ee55349)
<a name="qIPPq"></a>
### 选择int型变量
```python
dataset_int = dataset.select_dtypes(include=['int']).copy()
dataset_int.head()
```
![2021-05-12-19-05-34-592757.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821022154-0fde543f-e3a2-4461-b35f-16ed9b1bbedc.png#clientId=u1e03d214-7e12-4&from=ui&id=u23cd84f5&originHeight=396&originWidth=834&originalType=binary&size=16618&status=done&style=shadow&taskId=u808afa76-c170-42a9-86bd-37f4e229b3a)
<a name="mf3vM"></a>
### 选择object型变量
```python
dataset_object = dataset.select_dtypes(include=['object']).copy()
dataset_object.head()
```
|  | Class |
| --- | --- |
| Date |  |
| 2015-01-02 | Increase |
| 2015-01-05 | Increase |
| 2015-01-06 | Increase |
| 2015-01-07 | Decrease |
| 2015-01-08 | Decrease |

<a name="pKxDj"></a>
### 几种饼图
```python
Increase = dataset_object[dataset_object=='Increase'].count().sum().astype(float)
Decrease = dataset_object[dataset_object=='Decrease'].count().sum().astype(float)

new_dataset_object = [Increase, Decrease]
labels = ['Increase', 'Decrease']
colors = ['g', 'r']
# 只"explode"第二个饼片
explode = (0, 0.1)  
fig1, ax1 = plt.subplots()
ax1.pie(new_dataset_object, 
        explode=explode, 
        labels=labels, 
        autopct='%1.1f%%',
        shadow=True, 
        startangle=90)

# 相等的长宽比确保饼图以圆的形式绘制
ax1.axis('equal')  
plt.tight_layout()
plt.show()
```
![2021-05-12-19-05-34-691526.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821035523-f4d7d3f5-4c77-4006-9bbe-fa62d8d9fd08.png#clientId=u1e03d214-7e12-4&from=ui&id=ua5add095&originHeight=384&originWidth=564&originalType=binary&size=18978&status=done&style=shadow&taskId=u1a74206d-85a3-4608-a0ff-f70add72e54)
<a name="SgtES"></a>
#### 饼图设置样式
```python
new_dataset_object = [Increase, Decrease]
labels = ['Increase', 'Decrease']
colors = ['#99ff99','#ff9999']

fig1, ax1 = plt.subplots()
patches, texts, autotexts = ax1.pie(
      new_dataset_object,
      colors = colors,
      labels=labels,
      autopct='%1.1f%%',
      startangle=90)
for text in texts:
    text.set_color('grey')
for autotext in autotexts:
    autotext.set_color('grey')
ax1.axis('equal')  
plt.tight_layout()
plt.show()
```
![2021-05-12-19-05-34-868405.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821048046-a5877968-76db-4ce0-9ed4-bf6056253d75.png#clientId=u1e03d214-7e12-4&from=ui&id=u40270057&originHeight=384&originWidth=564&originalType=binary&size=12105&status=done&style=shadow&taskId=u168179a4-b1e7-48ab-80c7-86d2da3a321)
<a name="NxXom"></a>
#### 环状饼图
```python
new_dataset_object = [Increase, Decrease]
labels = ['Increase', 'Decrease']
colors = ['#99ff99','#ff9999']
explode = (0.05,0.05)
plt.pie(new_dataset_object, 
        colors = colors, 
        labels=labels, 
        autopct='%1.1f%%', 
        startangle=90, 
        pctdistance=0.85, 
        explode = explode)
# 画环状图
centre_circle = plt.Circle((0,0),0.70,fc='white')
fig = plt.gcf()
fig.gca().add_artist(centre_circle)
ax1.axis('equal')  
plt.tight_layout()
plt.show()
```
![2021-05-12-19-05-34-990075.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821060824-3fb4121f-66b1-4296-9125-2b56bc71ec1a.png#clientId=u1e03d214-7e12-4&from=ui&id=u9d7b027c&originHeight=384&originWidth=435&originalType=binary&size=13997&status=done&style=shadow&taskId=uf0463e8e-5925-4928-85ca-9624796c3bd)
