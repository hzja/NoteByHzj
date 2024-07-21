Python Pandas
<a name="L9LSo"></a>
## Pandas和Matplotlib绘图对比
使用pandas随机生成示例数据
```python
import pandas as pd
df= pd.DataFrame(np.random.rand(8, 4), columns=['A','B','C','D'])
```
![](./img/1606469127027-458a48b0-440e-4c31-960c-a7659d93b73c.png)<br />现在只要一行代码，即可完成柱状图的绘制`df.plot.bar()`![](./img/1606469127060-0ed8ece9-31c2-4a0d-8d11-29660e879612.png)<br />同样是这个图，如果使用Matplotlib制作的话👇
```python
import matplotlib.pyplot as plt
x = np.array(df.index.tolist())
y1 = df.A.tolist()
y2 = df.B.tolist()
y3 = df.C.tolist()
y4 = df.D.tolist()
y5 = df.E.tolist()
total_width, n = 0.8, 5
width = total_width / n
x = x - (total_width - width) / 2
plt.bar(x, y1,  width=width, label='A')
plt.bar(x + width, y2, width=width, label='B')
plt.bar(x + 2*width, y3, width=width, label='C')
plt.bar(x + 3*width, y4, width=width, label='D')
plt.bar(x + 4*width, y5, width=width, label='E')
plt.legend()
plt.show()
```
![](./img/1606469127149-04e53ae8-e31a-4577-9d0a-aee668b63879.png)<br />可以看到，虽然结果差不多，但是Matplotlib绘图需要的代码量比Pandas要多的多。
<a name="yttM6"></a>
## Pandas绘图示例
<a name="ItvB1"></a>
### 堆叠柱状图，添加一个参数即可`df.plot.barh(stacked=True)`![](./img/1606469127025-f244815b-43ee-448f-bcb1-b036288778a1.png)
<a name="iKzrV"></a>
### 堆叠面积图`df.plot.area(stacked=True,alpha = 0.9)`![](./img/1606469127343-d0265d91-6489-447d-806d-dc27c9f8b6fa.png)
<a name="6541eb2c"></a>
### 密度估计图`df.plot.kde()`
![](./img/1606469127395-533187a2-a54e-4a9d-98d2-693f010bab0f.png)
<a name="WckmY"></a>
### 直方图
需要换一组数据
```python
df = pd.DataFrame({'a': np.random.randn(1000) + 1,
                   'b': np.random.randn(1000),
                   'c': np.random.randn(1000) - 1},
                  columns=['a', 'b', 'c'])
```
下面，两行代码就能制作堆叠/不堆叠的直方图
```python
df.plot.hist(stacked=True, bins=20)
df.plot.hist(alpha=0.5)
```
![](./img/1606469127055-2666de0a-d2b5-4288-ba6a-68abe79749f9.png)![](./img/1606469127166-d0666ca1-f8bb-4f5e-b3db-5822f4682a86.png)<br />**箱线图、散点图等常用图形**，都可以通过一行代码轻松搞定👇
```python
df.plot.box()
df['value'].plot()
df.plot.scatter()
data.plot.hexbin(x='A',y='B')
```
![](./img/1606469127239-9314f5e6-7dd0-43c5-a1df-0887ccfbf819.png)
<a name="hizMd"></a>
### 制作子图
只需要设置`subplots=True`就行了，子图位置、大小调整方式和Matplotlib设置一样！
```python
data.plot(subplots=True,layout=(3, 2), figsize=(15, 8));
```
![](./img/1606469127147-0dd08542-ff2e-4ad1-a6b0-c944bc900188.png)<br />更多的图表可以从官方文档中可以看到，`**Pandas**`**一共支持14种常见图表的直接绘制**。
<a name="5brMU"></a>
## 修改主题，使用酷炫样式
如果觉得默认生成的图表不够好看，而它又是基于`Matoplotlib`制作的，所以**可以使用同样套着Matplotlib壳的**`**Seaborn**`**调色板**，从而调整图片主题。<br />还是上面的数据，下面换个主题重新绘制
```python
import seaborn as sns
sns.set_palette("pastel", 8)
```
![](./img/1606469127247-0a8a16a9-5baa-4ee7-90c6-0dc36bd637fd.png)
```python
import seaborn as sns
sns.set_palette("Blues_r", 8)
```
![](./img/1606469127142-613fc3d6-fa51-4256-90c2-af5fe569ed1f.png)
```python
import seaborn as sns
sns.set_palette("magma", 8)
```
![](./img/1606469127172-6d918f8c-b1d4-4b24-8b93-1211578502be.png)<br />更多的颜色搭配可以在seaborn相关文档中找到并使用！
