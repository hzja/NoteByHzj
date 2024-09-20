Python Plotly Express<br />Plotly Express 是一个新的高级 Python 可视化库，它是 Plotly.py 的高级封装，为复杂图表提供简单的语法。最主要的是 Plotly 可以与 Pandas 数据类型 DataFrame 完美的结合，对于数据分析、可视化来说实在是太便捷了，而且是完全免费的，非常值得尝试<br />下面使用 Ployly 的几个内置数据集来进行相关图表绘制的演示
<a name="MVcW5"></a>
## 数据集
Plotly 内置的所有数据集都是 DataFrame 格式，也即是与 Pandas 深度契合的体现
<a name="kzsZK"></a>
### 不同国家历年GDP收入与人均寿命
包含字段：国家、洲、年份、平均寿命、人口数量、GDP、国家简称、国家编号
```python
gap = px.data.gapminder()
gap2007 = gap.query("year==2007")
gap2007
```
Output<br />![](./img/1641137790838-6a4a1014-8191-41ea-b6e5-85c71fc63438.webp)
<a name="V8zg3"></a>
### 餐馆的订单流水
包含字段：总账单、小费、性别、是否抽烟、星期几、就餐时间、人数
```python
tips = px.data.tips()
tips
```
Output<br />![](./img/1641137790874-e5e389b5-9831-42a6-952c-3d9c8ff1b326.webp)
<a name="STbi4"></a>
### 鸢尾花
包含字段：萼片长、萼片宽、花瓣长、花瓣宽、种类、种类编号
```python
iris = px.data.iris()  
iris
```
Output<br />![](./img/1641137790871-036613b2-55ce-44ef-97c7-a4181e5ff66f.webp)
<a name="a0t1E"></a>
### 风力数据
包含字段：方向、强度、数值
```python
wind = px.data.wind()  
wind
```
Output<br />![](./img/1641137791059-3da04d6e-4937-4412-903d-95c65ad4db56.webp)
<a name="LWeVr"></a>
### 2013年蒙特利尔市长选举投票结果
包括字段：区域、Coderre票数、Bergeron票数、Joly票数、总票数、胜者、结果(占比分类)
```python
election = px.data.election() 
election
```
Output<br />![](./img/1641137791717-00a940f9-c781-4452-b77f-bd89599585e6.webp)
<a name="MUC2y"></a>
### 蒙特利尔一个区域中心附近的汽车共享服务的可用性
包括字段：纬度、经度、汽车小时数、高峰小时
```python
carshare = px.data.carshare()
carshare
```
Output<br />![](./img/1641137791201-f9f4bc79-52f9-410c-9cad-532a243bad6d.webp)
<a name="t4Tzg"></a>
## 内置调色板
Plotly 还拥有众多色彩高级的调色板，在绘制图表的时候不再为颜色搭配而烦恼
<a name="Kibzi"></a>
### 卡通片的色彩和序列
```python
px.colors.carto.swatches()
```
Output<br />![](./img/1641137791691-888db05d-e7ea-402b-8cef-89d96595be8a.webp)
<a name="rRP8G"></a>
### CMOcean项目的色阶
```python
px.colors.cmocean.swatches()
```
Output<br />![](./img/1641137791657-0e120f59-8752-4e02-bed5-c0d7e357aae3.webp)<br />还有其他很多调色板供选择，就不一一展示了，下面只给出代码，具体颜色样式可以自行运行代码查看<br />ColorBrewer2项目的色阶
```python
px.colors.colorbrewer
```
周期性色标，适用于具有自然周期结构的连续数据
```python
px.colors.cyclical 
```
分散色标，适用于具有自然终点的连续数据
```python
px.colors.diverging
```
定性色标，适用于没有自然顺序的数据
```python
px.colors.qualitative
```
顺序色标，适用于大多数连续数据
```python
px.colors.sequential
```
<a name="crUQQ"></a>
## Plotly Express 基本绘图
<a name="tK6qm"></a>
### 散点图
Plotly 绘制散点图非常容易，一行代码就可以完成
```python
px.scatter(gap2007, x="gdpPercap", y="lifeExp")
```
Output<br />![](./img/1641137791818-3643e176-9281-4ab3-b25b-1c7d69765c8a.webp)<br />还可以通过参数 color 来区分不同的数据类别
```python
px.scatter(gap2007, x="gdpPercap", y="lifeExp", color="continent")
```
Output<br />![](./img/1641137791977-6eadecb4-44cb-4c87-9be9-8ab3bdcac820.webp)<br />这里每个点都代表一个国家，不同颜色则代表不同的大洲<br />可以使用参数 size 来体现数据的大小情况
```python
px.scatter(gap2007, x="gdpPercap", y="lifeExp", color="continent", size="pop", size_max=60)
```
Output<br />![](./img/1641137791983-f8bf40ac-2396-4588-8884-a88ed773d9d9.webp)<br />还可以通过参数 hover_name 来指定当鼠标悬浮的时候，展示的信息<br />![](./img/1641137792340-1d31a844-8176-49d9-9b93-fd7f1cf2beb6.gif)<br />还可以根据数据集中不同的数据类型进行图表的拆分
```python
px.scatter(gap2007, x="gdpPercap", y="lifeExp", color="continent", size="pop", 
           size_max=60, hover_name="country", facet_col="continent", log_x=True)
```
Output<br />![](./img/1641137792286-f076e9c8-f635-4358-a509-1d2a56b7b3ea.webp)<br />当然还可以查看不同年份的数据，生成自动切换的动态图表
```python
px.scatter(gap, x="gdpPercap", y="lifeExp", color="continent", size="pop", 
           size_max=60, hover_name="country", animation_frame="year", animation_group="country", log_x=True,
          range_x=[100, 100000], range_y=[25, 90], labels=dict(pop="Population", gdpPercap="GDP per Capa", lifeExp="Life Expectancy"))
```
Output<br />![](./img/1641137792483-24ab6ef5-0625-4ccb-ad09-5ffd5b8f49e8.gif)
<a name="WM8w9"></a>
### 地理信息图
Plotly 绘制动态的地理信息图表也是非常方便，通过这种地图的形式，也可以清楚的看到数据集中缺少前苏联的相关数据
```python
px.choropleth(gap, locations="iso_alpha", color="lifeExp", hover_name="country", animation_frame="year", 
              color_continuous_scale=px.colors.sequential.Plasma, projection="natural earth")
```
Output<br />![](./img/1641137792638-08e79b49-79e7-48f8-b133-c1f642657793.gif)
<a name="eRA31"></a>
### 矩阵散点图
```python
px.scatter_matrix(iris, dimensions=['sepal_width', 'sepal_length', 'petal_width', 'petal_length'], color='species', symbol='species')
```
Output<br />![](./img/1641137792327-920308d0-a7ce-49af-828c-7919808a5e70.webp)
<a name="yxQbm"></a>
### 平行坐标图
```python
px.parallel_coordinates(tips, color='size', color_continuous_scale=px.colors.sequential.Inferno)
```
Output<br />![](./img/1641137792954-d7208712-e194-44d5-a28d-9b06271eae4e.webp)
<a name="UHaXA"></a>
### 三元散点图
```python
px.scatter_ternary(election, a="Joly", b="Coderre", c="Bergeron", color="winner", size="total", hover_name="district",
                   size_max=15, color_discrete_map = {"Joly": "blue", 
                   "Bergeron": "green", "Coderre":"red"} )
```
Output<br />![](./img/1641137793014-8545cc5b-bf6d-436c-a89c-fc73f65d3f97.webp)
<a name="v14yQ"></a>
### 极坐标线条图
```python
px.line_polar(wind, r="frequency", theta="direction", color="strength", 
            line_close=True,color_discrete_sequence=px.colors.sequential.Plotly3[-2::-1])
```
Output<br />![](./img/1641137792912-c4eac58b-29d7-4017-bb4d-4dce8b5bcf8d.webp)
<a name="v92nr"></a>
### 小提琴图
```python
px.violin(tips, y="tip", x="sex", color="smoker", facet_col="day", facet_row="time",box=True, points="all", 
          category_orders={"day": ["Thur", "Fri", "Sat", "Sun"], "time": ["Lunch", "Dinner"]},
          hover_data=tips.columns)
```
Output<br />![](./img/1641137793595-241fa019-2150-4a4e-a3cf-0d44471b9564.webp)
<a name="PdQZN"></a>
### 极坐标条形图
```python
px.bar_polar(wind, r="frequency", theta="direction", color="strength",
            color_discrete_sequence= px.colors.sequential.Plotly3[-2::-1])
```
Output<br />![](./img/1641137793650-52a311ee-a1d6-4cd1-84cf-a1b6b7185fa1.webp)
<a name="FPYX4"></a>
### 并行类别图
```python
px.parallel_categories(tips, color="size", color_continuous_scale=px.
            colors.sequential.Inferno)
```
Output<br />![](./img/1641137793322-a9fa0d27-abf2-47c2-8498-03e5f5c24fc4.webp)
<a name="rofMU"></a>
### 直方图
```python
px.histogram(tips, x="total_bill", color="smoker",facet_row="day", facet_col="time")
```
Output<br />![](./img/1641137793335-613d2b13-3081-48f0-8027-67fb3aa5c2b7.webp)
<a name="tACxJ"></a>
### 三维散点图
```python
px.scatter_3d(election, x="Joly", y="Coderre", z="Bergeron", color="winner", 
              size="total", hover_name="district",symbol="result", 
              color_discrete_map = {"Joly": "blue", "Bergeron": "green", 
              "Coderre":"red"})
```
Output<br />![](./img/1641137793418-49ffe385-447f-4880-8af5-a2bf8756824a.webp)
<a name="HWzvS"></a>
### 密度等值线图
```python
px.density_contour(iris, x="sepal_width", y="sepal_length", color="species")
```
Output<br />![](./img/1641137793906-60899a08-cf81-4b1e-9973-1037e405b331.webp)
<a name="GvnzZ"></a>
### 箱形图
```python
px.box(tips, x="sex", y="tip", color="smoker", notched=True)
```
Output<br />![](./img/1641137793581-dafa7836-ed2a-4ba5-8fd5-0e239a14bcf5.webp)
<a name="mYexp"></a>
### 地理坐标线条图
```python
px.line_geo(gap.query("year==2007"), locations="iso_alpha", 
            color="continent", projection="orthographic")
```
Output<br />![](./img/1641137793985-09fae8e9-8ced-4fcc-b8ae-5beb9b3e6904.webp)
<a name="CokHN"></a>
### 条线图
```python
px.line(gap, x="year", y="lifeExp", color="continent", 
        line_group="country", hover_name="country",
        line_shape="spline", render_mode="svg")
```
Output<br />![](./img/1641137793903-a8676c4e-26e4-43c1-a8b5-879c28ee9f9a.webp)
<a name="C1ZOf"></a>
### 面积图
```python
px.area(gap, x="year", y="pop", color="continent", 
        line_group="country")
```
Output<br />![](./img/1641137793925-0283470a-3443-46ad-a7f2-988becdecd56.webp)
<a name="AvT4S"></a>
### 热力图
```python
px.density_heatmap(iris, x="sepal_width", y="sepal_length", 
                   marginal_x="rug", marginal_y="histogram")
```
Output<br />![](./img/1641137794313-0ad997d3-20f1-4b1a-8b10-1e8ddb3cd34a.webp)
<a name="ppjPZ"></a>
### 条形图
```python
px.bar(tips, x="sex", y="total_bill", color="smoker", barmode="group")
```
Output<br />![](./img/1641137794135-a2920ff2-c927-4021-a44c-aa7d3916b342.webp)<br />总体来说，Plotly/Plotly Express 还是非常强大绘图工具，值得细细研究~
