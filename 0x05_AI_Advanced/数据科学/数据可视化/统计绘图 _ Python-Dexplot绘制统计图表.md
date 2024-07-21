Python 统计图表
<a name="p6kuh"></a>
## Python-Dexplot 简介
<a name="mA4kv"></a>
### 「官网」
Dexplot库的官网如下：[https://www.dexplo.org/dexplot/](https://www.dexplo.org/dexplot/)
<a name="uXWVR"></a>
### 「安装方式」
Dexplot库的安装方式也十分简单，如下：
```bash
pip install dexplot
```
<a name="ilBOQ"></a>
### 「绘图所需数据类型」
Dexplot库可同时支持“长数据”和“宽数据”类型，这一点和R-ggplot2绘图所需要的数据类型相似(ggplot2只支持长数据)，下面通过两幅图展示一下什么是长数据和宽数据：<br />![2021-05-19-09-01-52-709446.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386411863-e5ef94b9-c8f9-4555-96d8-f90c47856698.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u6d8b4a59&originHeight=742&originWidth=611&originalType=binary&size=40174&status=done&style=shadow&taskId=u63e30d24-155d-4cca-9a8c-a73cd107395)<br />long_data<br />![2021-05-19-09-01-52-848079.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386420361-a734ad06-9dda-4d44-aa89-6e6598167406.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u5b69afb6&originHeight=303&originWidth=661&originalType=binary&size=12554&status=done&style=shadow&taskId=u4f2c265a-4dba-442f-aecc-38916dcd923)<br />width_data
<a name="yknuR"></a>
### 「基本用法」
Dexplot库提供了用于绘制不同图表的绘图函数，但其基本使用语法如下：
```python
dxp.plotting_func(x, y, data, aggfunc, split, row, col, orientation, ...)
```
<a name="F4SyX"></a>
#### 各参数解释如下

- `x`:沿x轴的列名
- `y`:沿y轴的列名
- `data`：Pandas DataFrame数据类型。
- `aggfunc`：pandas常用聚类计算函数。
- `split`：将数据分到不同组的的数据列名。
- `row`：用于按行将数据拆分为不同的子图的数据列名。
- `col`：用于按列将数据拆分为不同的子图的数据列名。
- `orientation`：绘图方向，有垂直('v')或水平('h')。大多数图的默认设置为垂直。
<a name="Pn0XN"></a>
### 「主要绘图类型」
Dexplot绘图库主要提供聚类图和分布图两种数据类型， 聚合图采用一系列值，并使用提供给aggfunc的函数返回单个值，而分布图采用一系列值并以某种方式描述分布形状。接下来通过具体可视化图表展示Dexplot库绘图魅力。
<a name="WZtCf"></a>
## Python-Dexplot样例展示
<a name="rwYc1"></a>
### 「样例一」：柱形图系列
```python
import dexplot as dxp
import pandas as pd
import matplotlib.pyplot as plt
airbnb = dxp.load_dataset('airbnb')
#bar01
dxp.bar(x='neighborhood', y='price', data=airbnb, aggfunc='median',figsize=(5,3.5),
       bar_kwargs={"color":"#0073C2","ec":"black"})
```
![2021-05-19-09-01-52-999672.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386430616-bdae1c64-9465-44cd-bfc4-82d58f308bce.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u41b9713e&originHeight=738&originWidth=1080&originalType=binary&size=31589&status=done&style=shadow&taskId=u25ccf263-0b3b-4533-852c-c986ca6dc5b)<br />Example01 of dxp.bar
<a name="JNEeo"></a>
#### 排序
```python
dxp.bar(x='neighborhood', y='price', data=airbnb, aggfunc='median', 
                figsize=(5,3.5),sort_values='asc',bar_kwargs={"color":"#0073C2","ec":"black"})
```
![2021-05-19-09-01-53-179193.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386437921-452fca0d-b148-4841-a647-be3c7cadd9e6.png#clientId=u350c6b7d-e3eb-4&from=ui&id=ue4dbcfa5&originHeight=738&originWidth=1080&originalType=binary&size=31519&status=done&style=shadow&taskId=uae650641-a49d-4fc7-835f-8b773b6fb27)<br />Example02 of dxp.bar
<a name="momaj"></a>
#### 水平
```python
dxp.bar(x='price', y='neighborhood', data=airbnb, aggfunc='median', figsize=(3,4),
        orientation='h', sort_values='desc',bar_kwargs={"color":"#EFC000","ec":"black"})
```
![2021-05-19-09-01-53-301862.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386445282-14da0e32-0de9-4e3b-9c0f-8eb032ff6d9e.png#clientId=u350c6b7d-e3eb-4&from=ui&id=ue5f2ebb7&originHeight=1442&originWidth=1080&originalType=binary&size=57222&status=done&style=shadow&taskId=u4293aebe-8b3e-43b4-85b6-cbca2854e37)<br />Example03 of dxp.bar
<a name="Sco3n"></a>
#### 分组
```python
dxp.bar(x='neighborhood', y='price', data=airbnb, aggfunc='median', split='superhost',figsize=(4,2.5),
       cmap=["#0073C2","#EFC000"],bar_kwargs={"ec":"black"})
```
![2021-05-19-09-01-53-459440.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386453660-9e67fd19-6167-450e-823d-544c0d7ae847.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u85fd389c&originHeight=562&originWidth=1080&originalType=binary&size=40113&status=done&style=shadow&taskId=u284dbed7-40d9-44a4-a418-38939e125b4)<br />Example04 of dxp.bar
<a name="XIYoC"></a>
#### 分面
```python
dxp.bar(x='neighborhood', y='price', data=airbnb, aggfunc='median', split='superhost', 
        col='property_type', col_order=['House', 'Condominium', 'Apartment'],
        row='bedrooms', row_order=[1, 2, 3], sharey=False,cmap=["#0073C2","#EFC000"],
        bar_kwargs={"ec":"black"})
```
![2021-05-19-09-01-53-674863.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386464239-fac1ba1f-d69c-475c-8fd1-f826c1403350.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u1cee4ed3&originHeight=518&originWidth=1080&originalType=binary&size=65296&status=done&style=shadow&taskId=u21398f24-2d9b-47c0-88e8-6dd554b6380)<br />Example05 of dxp.bar
<a name="jM0ik"></a>
### 「样例二」：箱线图系列
```python
dxp.box(x='price', y='neighborhood', data=airbnb,cmap=["#CD534C"],figsize=(3,3))
```
![2021-05-19-09-01-53-848400.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386518161-32222ff5-2eb4-45eb-a694-d315d82485b4.png#clientId=u350c6b7d-e3eb-4&from=ui&id=uc6853d64&originHeight=1047&originWidth=1080&originalType=binary&size=126534&status=done&style=shadow&taskId=u2b61e4cd-e610-4983-aea2-4bea4ae2293)<br />Example01 of dxp.box
<a name="Ahgow"></a>
#### 分组
```python
dxp.box(x='price', y='neighborhood', data=airbnb,figsize=(3,3.5),
        cmap=["#0073C2","#EFC000","#868686","#CD534C"],
        split='property_type', split_order='desc')
```
![2021-05-19-09-01-54-099731.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386527586-3899eac1-f61e-4354-bbb8-62399a937f01.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u39079318&originHeight=912&originWidth=1080&originalType=binary&size=123892&status=done&style=shadow&taskId=u1a938bd9-e3ec-4f50-b25a-718b869fd9e)<br />Example02 of dxp.box
<a name="qOUJq"></a>
### 「样例三」：密度图(KDE)
```python
dxp.kde(x='price', data=airbnb, split='bedrooms', split_order=[1, 2, 3],
       cmap=["#0073C2","#EFC000","#868686"])
```
![2021-05-19-09-01-54-385961.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386537633-bd7d6f02-5535-4139-a375-b4ca990ab14a.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u9397e6a8&originHeight=677&originWidth=1080&originalType=binary&size=70130&status=done&style=shadow&taskId=u3844a9bc-6405-466b-b9b9-c7b6fa9369f)<br />Example of dxp.kde
<a name="oL2YJ"></a>
### 「样例四」：散点图
```python
dxp.scatter(x='longitude', y='latitude', data=airbnb, 
            split='neighborhood', col='bedrooms', col_order=[2, 3])
```
![2021-05-19-09-01-54-574457.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621386233700-e94d0c8c-f17b-4ea8-9bca-6ce0a2a2ccd2.png#clientId=u350c6b7d-e3eb-4&from=ui&id=u22470d82&originHeight=314&originWidth=1080&originalType=binary&size=123926&status=done&style=shadow&taskId=u002607ef-7e0b-4035-80cb-36c7391d97c)<br />Example of dxp.scatter
