Pandas<br />介绍一个简单的综合使用，主要分为：

- 如何自行模拟数据
- 多种数据处理方式
- 数据统计与可视化
- 用户RFM模型
- 用户复购周期

![](./img/1658278396654-b00923ca-fd3e-4033-99b7-927e8b6c266f.jpeg)
<a name="zefCx"></a>
## 构建数据
本案例中用的数据是自行模拟的，主要包含两个数据：订单数据和水果信息数据，并且会将两份数据合并
```python
import pandas as pd
import numpy as np
import random
from datetime import *
import time

import plotly.express as px
import plotly.graph_objects as go
import plotly as py

# 绘制子图
from plotly.subplots import make_subplots
```
<a name="bAWzy"></a>
### 1、时间字段
![](./img/1658278396614-7ee088e3-631b-449e-a9d5-b95fdecd751b.jpeg)
<a name="DOJqy"></a>
### 2、水果和用户
![](./img/1658278396681-d5dbeb15-1200-4715-baf3-270108c270a5.jpeg)
<a name="WHLTW"></a>
### 3、生成订单数据
```python
order = pd.DataFrame({
    "time":time_range,  # 下单时间
    "fruit":fruit_list,  # 水果名称
    "name":name_list,  # 顾客名
    # 购买量
    "kilogram":np.random.choice(list(range(50,100)), size=len(time_range),replace=True) 
})

order
```
![](./img/1658278396609-40117c82-a7d6-4982-8869-5701e5b72ff5.jpeg)
<a name="X18hn"></a>
### 4、生成水果的信息数据
```python
infortmation = pd.DataFrame({
    "fruit":fruits,
    "price":[3.8, 8.9, 12.8, 6.8, 15.8, 4.9, 5.8, 7],
    "region":["华南","华北","西北","华中","西北","华南","华北","华中"]
})

infortmation
```
![](./img/1658278396703-0f2a48c1-1982-4c9f-9edb-e160cfa0de80.jpeg)
<a name="FE15M"></a>
### 5、数据合并
将订单信息和水果信息直接合并成一个完整的DataFrame，这个df就是接下来处理的数据<br />![](./img/1658278397029-fc7e9803-830f-4bf5-908e-4ee10e6dcdd3.jpeg)
<a name="X6nYT"></a>
### 6、生成新的字段：订单金额
![](./img/1658278397168-42986f40-0905-4bb8-a155-5a639bb3769d.jpeg)<br />到这里可以学到：

- 如何生成时间相关的数据
- 如何从列表（可迭代对象）中生成随机数据
- Pandas的DataFrame自行创建，包含生成新字段
- Pandas数据合并
<a name="ctj6z"></a>
## 分析维度1：时间
<a name="dhoXY"></a>
### 2019-2021年每月销量走势
<a name="Ia3Bs"></a>
#### 1、先把年份和月份提取出来：
```python
df["year"] = df["time"].dt.year
df["month"] = df["time"].dt.month
# 同时提取年份和月份
df["year_month"] = df["time"].dt.strftime('%Y%m')

df
```
![](./img/1658278397124-c2373b22-d765-4236-b29d-38fff5ca54f2.jpeg)
<a name="cHNBq"></a>
#### 2、查看字段类型：
![](./img/1658278397211-b2a40d63-bd1b-4c95-9a47-00040232c6b7.jpeg)
<a name="vK6IC"></a>
#### 3、分年月统计并展示：
```python
# 分年月统计销量
df1 = df.groupby(["year_month"])["kilogram"].sum().reset_index()

fig = px.bar(df1,x="year_month",y="kilogram",color="kilogram")
fig.update_layout(xaxis_tickangle=45)   # 倾斜角度

fig.show()
```
![](./img/1658278397242-ec3bf179-a3dd-4be3-a10d-797fcaffcf38.jpeg)
<a name="cdkXp"></a>
### 2019-2021销售额走势
```python
df2 = df.groupby(["year_month"])["amount"].sum().reset_index()

df2["amount"] = df2["amount"].apply(lambda x:round(x,2))

fig = go.Figure()
fig.add_trace(go.Scatter(  #
    x=df2["year_month"],
    y=df2["amount"],
    mode='lines+markers', # mode模式选择
    name='lines')) # 名字

fig.update_layout(xaxis_tickangle=45)   # 倾斜角度

fig.show()
```
![](./img/1658278397472-7f7cbf7e-53ba-4610-a30b-6725320ae510.jpeg)
<a name="ZvV6o"></a>
### 年度销量、销售额和平均销售额
![](./img/1658278397550-d2bd0e05-54a8-44c7-8a5e-473a39104fca.jpeg)
<a name="qh4XC"></a>
## 分析维度2：商品
<a name="dRJqu"></a>
### 水果年度销量占比
```python
df4 = df.groupby(["year","fruit"]).agg({"kilogram":"sum","amount":"sum"}).reset_index()
df4["year"] = df4["year"].astype(str)
df4["amount"] = df4["amount"].apply(lambda x: round(x,2))

from plotly.subplots import make_subplots
import plotly.graph_objects as go

fig = make_subplots(
	rows=1, 
	cols=3,
	subplot_titles=["2019年","2020年","2021年"],
	specs=[[{"type": "domain"},   # 通过type来指定类型
			{"type": "domain"},
			{"type": "domain"}]]
)  

years = df4["year"].unique().tolist()

for i, year in enumerate(years):
	name = df4[df4["year"] == year].fruit
	value = df4[df4["year"] == year].kilogram
	
	fig.add_traces(go.Pie(labels=name,
						  values=value
						 ),
				   rows=1,cols=i+1
				  )
	
	fig.update_traces(
		textposition='inside',   # 'inside','outside','auto','none'
		textinfo='percent+label',
		insidetextorientation='radial',   # horizontal、radial、tangential
		hole=.3,
		hoverinfo="label+percent+name"
	)
	
fig.show()
```
![](./img/1658278397584-490c19f4-c8d4-402f-8429-641197182e01.jpeg)
<a name="PLZUh"></a>
### 各水果年度销售金额对比
```python
years = df4["year"].unique().tolist()

for _, year in enumerate(years):
    
    df5 = df4[df4["year"]==year]
    fig = go.Figure(go.Treemap( 
        labels = df5["fruit"].tolist(),
        parents = df5["year"].tolist(),
        values = df5["amount"].tolist(),
        textinfo = "label+value+percent root"
    ))
    
    fig.show()
```
![](./img/1658278397632-a98ce153-af3c-4111-a4d8-5ade729a887d.jpeg)<br />![](./img/1658278397761-e360801d-7b6e-4b2c-821b-eb1a330e556f.jpeg)<br />![](./img/1658278397810-fafe5824-f7aa-41e8-9edf-427450062cf5.jpeg)
<a name="hGxlT"></a>
### 商品月度销量变化
![](./img/1658278397955-19a3b7ee-003c-40ec-9ef4-926668b9a01b.jpeg)
```python
fig = px.bar(df5,x="year_month",y="amount",color="fruit")
fig.update_layout(xaxis_tickangle=45)   # 倾斜角度
fig.show()
```
![](./img/1658278397990-641c358f-2b79-4e94-8c19-0b344be61cb4.jpeg)<br />折线图展示的变化：<br />![](./img/1658278398060-a015962e-c785-468e-a376-adbf22d1eb16.jpeg)
<a name="aLlIA"></a>
## 分析维度3：地区
<a name="sx8Md"></a>
### 不同地区的销量
![](./img/1658278398149-ff875622-1162-4d96-aaaa-36947a4939ad.jpeg)<br />![](./img/1658278398182-327c5868-be50-45c7-b020-a37231260a50.jpeg)
<a name="FL7kX"></a>
### 不同地区年度平均销售额
```python
df7 = df.groupby(["year","region"])["amount"].mean().reset_index()
```
![](./img/1658278398354-16d597e8-a5c8-4857-9713-295823180811.jpeg)
<a name="s5ace"></a>
## 分析维度4：用户
<a name="Wi9PT"></a>
### 用户订单量、金额对比
```python
df8 = df.groupby(["name"]).agg({"time":"count","amount":"sum"}).reset_index().rename(columns={"time":"order_number"})

df8.style.background_gradient(cmap="Spectral_r")
```
![](./img/1658278398353-e06e17f8-bb63-4bb3-85d8-8f2273b20635.jpeg)
<a name="xzgKv"></a>
### 用户水果喜好
根据每个用户对每种水果的订单量和订单金额来分析：
```python
df9 = df.groupby(["name","fruit"]).agg({"time":"count","amount":"sum"}).reset_index().rename(columns={"time":"number"})

df10 = df9.sort_values(["name","number","amount"],ascending=[True,False,False])

df10.style.bar(subset=["number","amount"],color="#a97fcf")
```
![](./img/1658278398463-0d456b62-e45f-403e-98a7-82a554cd5005.jpeg)
```python
px.bar(df10,
       x="fruit",
       y="amount",
#            color="number",
       facet_col="name"
      )
```
![](./img/1658278398568-e0abc742-3c67-4ca0-93fc-6547521ffb13.jpeg)
<a name="iAIDz"></a>
## 用户分层—RFM模型
RFM模型是衡量客户价值和创利能力的重要工具和手段。<br />通过这个模型能够反映一个用户的交期交易行为、交易的总体频率和总交易金额3项指标，通过3个指标来描述该客户的价值状况；同时依据这三项指标将客户划分为8类客户价值：

- Recency（R）是客户最近一次购买日期距离现在的天数，这个指标与分析的时间点有关，因此是变动的。理论上客户越是在近期发生购买行为，就越有可能复购
- Frequency（F）指的是客户发生购买行为的次数--最常购买的消费者，忠诚度也就较高。增加顾客购买的次数意味着能占有更多的时长份额。
- Monetary value（M）是客户购买花费的总金额。![](./img/1658278398521-284daf2d-48f1-47bc-90b4-6e07dd211214.jpeg)

下面通过Pandas的多个方法来分别求解这个3个指标，首先是F和M：每位客户的订单次数和总金额<br />![](./img/1658278398792-8c6fe58a-d1a0-42cb-bbd5-f335bcb3acd6.jpeg)<br />如何求解R指标呢？<br />1、先求解每个订单和当前时间的差值<br />![](./img/1658278398807-a48a1eb2-ed2b-45c5-9888-1e726221a746.jpeg)<br />2、根据每个用户的这个差值R来进行升序排列，排在第一位的那条数据就是他最近购买记录：以xiaoming用户为例，最近一次是12月15号，和当前时间的差值是25天<br />![](./img/1658278398927-2c64dbbf-7ff3-487a-a8de-a721a119b74c.jpeg)<br />3、根据用户去重，保留第一条数据，这样便得到每个用户的R指标：<br />![](./img/1658278398883-93bbd945-21e4-4739-baf2-0b4fa432132c.jpeg)<br />4、数据合并得到3个指标：<br />![](./img/1658278399006-9345ea3e-147a-489c-b645-68ee3c092679.jpeg)<br />![](./img/1658278399137-8e0b72cb-c2df-4ede-ba90-9929cc8d227a.jpeg)<br />当数据量足够大，用户足够多的时候，就可以只用RFM模型来将用户分成8个类型
<a name="FZwPW"></a>
## 用户复购周期分析
复购周期是用户每两次购买之间的时间间隔：以xiaoming用户为例，前2次的复购周期分别是4天和22天<br />![](./img/1658278399307-bdff0c4a-24ad-4834-93da-a55532e554c5.jpeg)<br />下面是求解每个用户复购周期的过程：
<a name="aCNYA"></a>
### 1、每个用户的购买时间升序
![](./img/1658278399269-99f31173-e6d4-4d45-85d1-001d1cb9c1f7.jpeg)
<a name="S9Lgo"></a>
### 2、将时间移动一个单位：
![](./img/1658278399361-d15662bb-6e79-4638-87af-358766035d24.jpeg)
<a name="xINvs"></a>
### 3、合并后的差值：
出现空值是每个用户的第一条记录之前是没有数据，后面直接删除了空值部分<br />![](./img/1658278399363-2d18d5ad-d8d6-4e1d-a5d7-b79d70865890.jpeg)<br />![](./img/1658278399476-1410583d-67c8-4918-92e1-d65a37a7faa0.jpeg)<br />直接取出天数的数值部分：<br />![](./img/1658278399666-21197f51-3f51-41ef-a8c9-f63349c1b8f8.jpeg)
<a name="vMHTE"></a>
### 5、复购周期对比
```python
px.bar(df16,
       x="day",
       y="name",
       orientation="h",
       color="day",
       color_continuous_scale="spectral"   # purples
      )
```
![](./img/1658278399639-f95ad3ca-3269-4851-80d5-5556694a4b64.jpeg)<br />上图中矩形越窄表示间隔越小；每个用户整个复购周期由整个矩形长度决定。查看每个用户的整体复购周期之和与平均复购周期：<br />![](./img/1658278399725-357d9401-4e70-4aae-ae3f-d10209c8ab9d.jpeg)<br />得到一个结论：Michk和Mike两个用户整体的复购周期是比较长的，长期来看是忠诚的用户；而且从平均复购周期来看，相对较低，说明在短时间内复购活跃。<br />从下面的小提琴中同样可以观察到，Michk和Mike的复购周期分布最为集中。<br />![](./img/1658278399775-97a222d0-8280-4cee-822c-e4ca3d8867c9.jpeg)
