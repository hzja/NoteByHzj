Python
<a name="rIgwI"></a>
## Matplotlib
Matplotlib 是 Python 的一个绘图库，可以绘制出高质量的折线图、散点图、柱状图、条形图等等。它也是许多其他可视化库的基础。
```python
import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 10, 100)
y = np.sin(x)

plt.plot(x, y)
plt.show()
```
<a name="Cxy4Z"></a>
## Seaborn
Seaborn 是基于 Matplotlib 的 Python 数据可视化库，专门用于绘制统计图形，如热图、小提琴图、带误差线的折线图等等。
```python
import seaborn as sns
import pandas as pd

df = pd.read_csv('data.csv')

sns.boxplot(x='day', y='total_bill', data=df)
```
<a name="xngOZ"></a>
## Plotly
Plotly 是一个交互式数据可视化库，可以绘制出高质量的折线图、散点图、3D 图形等等。它支持多种编程语言，如 Python、R、JavaScript 等等。
```python
import plotly.graph_objs as go
import numpy as np

x = np.linspace(0, 10, 100)
y = np.sin(x)

fig = go.Figure(data=go.Scatter(x=x, y=y))
fig.show()
```
<a name="FXp6x"></a>
## Bokeh
Bokeh 是一个交互式数据可视化库，也支持多种编程语言，如 Python、R、JavaScript 等等。它可以绘制出高质量的折线图、散点图、柱状图、条形图等等。
```python
from bokeh.plotting import figure, show
import numpy as np

x = np.linspace(0, 10, 100)
y = np.sin(x)

p = figure(title='Sine Wave')
p.line(x, y, legend_label='Sine')
show(p)
```
<a name="EzeO8"></a>
## Altair
Altair 是一个基于 Vega-Lite 的 Python 可视化库，可以快速轻松地绘制出高质量的折线图、散点图、柱状图等等。
```python
import altair as alt
import pandas as pd

df = pd.read_csv('data.csv')

alt.Chart(df).mark_bar().encode(
    x='year',
    y='sales',
    color='region'
)
```
<a name="GdgLI"></a>
## ggplot
ggplot 是一个基于 R 语言中的 ggplot2 库的 Python 可视化库，可以绘制出高质量的散点图、柱状图、箱线图等等。
```python
from ggplot import *
import pandas as pd

df = pd.read_csv('data.csv')

ggplot(df, aes(x='date', y='value', color='variable')) + \
    geom_line() + \
    theme_bw()
```
<a name="PXVhS"></a>
## Holoviews
Holoviews 是一个 Python 可视化库，可以创建交互式的数据可视化，支持多种类型的可视化图形，如折线图、散点图、柱状图、热力图等等。
```python
import holoviews as hv
import numpy as np

x = np.linspace(0, 10, 100)
y = np.sin(x)

hv.extension('bokeh')
hv.Curve((x, y))
```
<a name="sMvcG"></a>
## Plotnine
Plotnine 是一个基于 Python 的 ggplot2 库的可视化库，它可以创建高质量的数据可视化图形，如散点图、柱状图、线图等等。
```python
from plotnine import *
import pandas as pd

df = pd.read_csv('data.csv')

(ggplot(df, aes(x='year', y='sales', fill='region')) +
 geom_bar(stat='identity', position='dodge'))
```
<a name="pTY4A"></a>
## Wordcloud
Wordcloud 是一个用于生成词云的 Python 库，可以将文本中出现频率高的词汇以图形的方式展示出来。
```python
from wordcloud import WordCloud
import matplotlib.pyplot as plt

text = "Python is a high-level programming language"

wordcloud = WordCloud().generate(text)

plt.imshow(wordcloud, interpolation='bilinear')
plt.axis('off')
plt.show()
```
<a name="Wya1w"></a>
## Networkx
Networkx 是一个用于创建、操作和可视化复杂网络的 Python 库。它支持创建多种类型的网络结构，如有向图、无向图、加权图等等。
```python
import networkx as nx
import matplotlib.pyplot as plt

G = nx.DiGraph()

G.add_edge('A', 'B')
G.add_edge('B', 'C')
G.add_edge('C', 'D')
G.add_edge('D', 'A')

pos = nx.spring_layout(G)

nx.draw_networkx_nodes(G, pos, node_size=500)
nx.draw_networkx_edges(G, pos)
nx.draw_networkx_labels(G, pos)

plt.axis('off')
plt.show()
```
