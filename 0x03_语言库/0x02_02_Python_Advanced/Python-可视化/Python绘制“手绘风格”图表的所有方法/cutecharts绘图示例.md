Python<br />这是一个纯 Python 语言编写的可视化工具，就像其名字一样，这是一个可以产生 cute 版图表的工具。<br />![](./img/1676939159325-fd771096-174f-4086-9434-8b89a6276b35.png)
<a name="W1MEe"></a>
## 柱状图
<a name="TDg60"></a>
### 基本柱状图
```python
from cutecharts.charts import Bar
from cutecharts.components import Page
from cutecharts.faker import Faker


def bar_base() -> Bar:
    chart = Bar("Bar-基本示例")
    chart.set_options(labels=Faker.choose(), x_label="I'm xlabel", y_label="I'm ylabel")
    chart.add_series("series-A", Faker.values())
    return chart

bar_base().render()
```
Output:<br />![](./img/1676939159308-94bf5208-80c8-4064-981f-e658c6a97ac3.png)
<a name="YTh0P"></a>
### 调整颜色
```python
def bar_tickcount_colors():
    chart = Bar("Bar-调整颜色")
    chart.set_options(labels=Faker.choose(), y_tick_count=10, colors=Faker.colors)
    chart.add_series("series-A", Faker.values())
    return chart
```
Output：<br />![](./img/1676939159306-b09976a4-dec8-489c-bad4-29f4634aba77.png)
<a name="LdMJV"></a>
## 折线图
<a name="Tw8U8"></a>
### 基本折线图
```python
from cutecharts.charts import Line
from cutecharts.components import Page
from cutecharts.faker import Faker


def line_base() -> Line:
    chart = Line("Line-基本示例")
    chart.set_options(labels=Faker.choose(), x_label="I'm xlabel", y_label="I'm ylabel")
    chart.add_series("series-A", Faker.values())
    chart.add_series("series-B", Faker.values())
    return chart
line_base().render()
```
Output:<br />![](./img/1676939159311-97e6a2a6-21d6-4291-8c02-28b305c342a5.png)
<a name="LZd5Z"></a>
### Legend 位置
```python
def line_legend():
    chart = Line("Line-Legend 位置")
    chart.set_options(labels=Faker.choose(), legend_pos="upRight")
    chart.add_series("series-A", Faker.values())
    chart.add_series("series-B", Faker.values())
    return chart
```
Output：<br />![](./img/1676939159274-0971aa3e-482f-4c76-9ca3-8e4c2e711cac.png)
<a name="NBRDP"></a>
### 调整颜色
```python
def line_tickcount_colors():
    chart = Line("Line-调整颜色")
    chart.set_options(labels=Faker.choose(), colors=Faker.colors, y_tick_count=8)
    chart.add_series("series-A", Faker.values())
    chart.add_series("series-B", Faker.values())
    return chart
```
Output:<br />![](./img/1676939159709-70b969ac-2b74-439c-8fbe-b73083cf6f28.png)
<a name="D4yK0"></a>
## 饼图
<a name="pzdPs"></a>
### 基本饼图
```python
from cutecharts.charts import Pie
from cutecharts.components import Page
from cutecharts.faker import Faker


def pie_base() -> Pie:
    chart = Pie("Pie-基本示例")
    chart.set_options(labels=Faker.choose())
    chart.add_series(Faker.values())
    return chart


pie_base().render()
```
Output：<br />![](./img/1676939159724-90eb7fd6-d22c-4c6e-a743-6511d4abbb2d.png)
<a name="EhrpI"></a>
### Legend 位置
```python
def pie_legend_font():
    chart = Pie("Pie-Legend")
    chart.set_options(
        labels=Faker.choose(),
        legend_pos="downLeft",
        font_family='"Times New Roman",Georgia,Serif;',
    )
    chart.add_series(Faker.values())
    return chart
```
Output：<br />![](./img/1676939159840-62442a4c-c85c-489e-be70-d8c5a32d9984.png)
<a name="CAg0Y"></a>
### Radius 调整
```python
def pie_radius():
    chart = Pie("Pie-Radius")
    chart.set_options(
        labels=Faker.choose(),
        inner_radius=0,
    )
    chart.add_series(Faker.values())
    return chart
```
Output：<br />![](./img/1676939159877-c2011d35-802e-4cee-b1de-ab3d2640d8fe.png)
<a name="yNGXV"></a>
## 雷达图
<a name="O5jyu"></a>
### 基本雷达图
```python
from cutecharts.charts import Radar
from cutecharts.components import Page
from cutecharts.faker import Faker


def radar_base() -> Radar:
    chart = Radar("Radar-基本示例")
    chart.set_options(labels=Faker.choose())
    chart.add_series("series-A", Faker.values())
    chart.add_series("series-B", Faker.values())
    return chart


radar_base().render()
```
Output：<br />![](./img/1676939160034-d83cb5f6-99ba-470f-96ee-4563caced64d.png)
<a name="vD6ip"></a>
### 颜色调整
```python
def radar_legend_colors():
    chart = Radar("Radar-颜色调整")
    chart.set_options(labels=Faker.choose(), colors=Faker.colors, legend_pos="upRight")
    chart.add_series("series-A", Faker.values())
    chart.add_series("series-B", Faker.values())
    return chart
```
Output：<br />![](./img/1676939160246-496efc8c-01cd-44d8-90ba-79350b4927ae.png)
<a name="oLqop"></a>
## 散点图
<a name="nONv7"></a>
### 基本散点图
```python
from cutecharts.charts import Scatter
from cutecharts.components import Page
from cutecharts.faker import Faker


def scatter_base() -> Scatter:
    chart = Scatter("Scatter-基本示例")
    chart.set_options(x_label="I'm xlabel", y_label="I'm ylabel")
    chart.add_series(
        "series-A", [(z[0], z[1]) for z in zip(Faker.values(), Faker.values())]
    )
    chart.add_series(
        "series-B", [(z[0], z[1]) for z in zip(Faker.values(), Faker.values())]
    )
    return chart


scatter_base().render()
```
Output：<br />![](./img/1676939160306-7bb9406e-82db-404c-9c39-591d736bdc05.png)
<a name="gU5Oo"></a>
### 散点大小
```python
def scatter_dotsize_tickcount():
    chart = Scatter("Scatter-散点大小")
    chart.set_options(dot_size=2, y_tick_count=8)
    chart.add_series(
        "series-A", [(z[0], z[1]) for z in zip(Faker.values(), Faker.values())]
    )
    chart.add_series(
        "series-B", [(z[0], z[1]) for z in zip(Faker.values(), Faker.values())]
    )
    return chart
```
Output：<br />![](./img/1676939160450-58493fc0-579b-436b-ab40-c5732b9c5e00.png)
<a name="lOCF2"></a>
### 散点连成线
```python
def scatter_show_line():
    chart = Scatter("Scatter-散点连成线")
    chart.set_options(y_tick_count=8, is_show_line=True)
    chart.add_series(
        "series-A", [(z[0], z[1]) for z in zip(Faker.values(), Faker.values())]
    )
    chart.add_series(
        "series-B", [(z[0], z[1]) for z in zip(Faker.values(), Faker.values())]
    )
    return chart
```
Output：<br />![](./img/1676939160473-637071dc-ff9e-4aaa-abd1-263687da3f6f.png)
<a name="xXjrx"></a>
## 参考资料
cutecharts: [https://github.com/cutecharts/cutecharts.py](https://github.com/cutecharts/cutecharts.py)
