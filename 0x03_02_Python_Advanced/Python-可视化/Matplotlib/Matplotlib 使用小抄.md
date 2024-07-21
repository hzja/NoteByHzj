Python Matplotlib<br />![](./img/1698734280889-437499d2-e499-4028-a36e-c8520806cc8b.jpeg)<br />Matplotlib是一个python 2D绘图库，利用它可以画出许多高质量的图像。在matlablib中画出同一个图，通常可以有好多种不同的代码实现。所以，如果不把以下这几个概念搞明白，在网上找代码的时候会很懵，一会儿plt、一会ax，云里雾里一般。
<a name="i3Btg"></a>
## 整体结构
Figure（画板） 画一幅图，首先要确定画在哪里。所以，最先需要做的就是创建一个画板，对应图中红色的整体框架。<br />Axes（Subplot）（画纸、子图） 有了画板，拿来两张纸，开始画，其中一个就叫Axes1，另一个就是Axes2，对应绿色部分。<br />Axis（坐标轴） 横轴叫`x`坐标轴`label`，纵轴叫`y`坐标轴`ylabel`<br />![image.png](./img/1626230047588-2ae981f6-2810-42a5-8a45-ba09deb80b1c.png)<br />生成这个图的代码如下：
```python
import matplotlib.pyplot as plt
import numpy as np
%matplotlib inline

# 第一步创建画布
fig = plt.figure()  # 如果不传入参数默认画板1

# 第2步创建画纸，并选择画纸1
ax1=plt.subplot(2,1,1)   

# 在画纸1上绘图
plt.plot()

# 选择画纸2
ax2=plt.subplot(2,1,2)
# 在画纸2上绘图
plt.plot()
# 显示图像
plt.show()
```
Matplotlib可用于Python脚本，Python和IPython shell，Jupyter笔记本，Web应用程序服务器和图形用户界面工具包。<br />这里是在jupyter笔记本中使用Matplotlib。<br />对于中文乱码和数字为负数乱码的情况，可添加以下的代码：
```python
plt.rcParams['font.sans-serif']=['SimHei'] # 防止中文乱码
plt.rcParams['axes.unicode_minus']=False # 如果需要将数字设为负数，也可能出现乱码的情况
```
<a name="dIAS8"></a>
##### 看看一张图的细节
拿出一个Axes（subplot），看看细节。<br />当没有子图的时候，一个Figure就对应一个Axes，一套坐标轴。<br />![image.png](./img/1626229490493-6a222109-e021-422f-84d8-c3c2e8228d87.png)<br />最上方是标题Title<br />图例Legend 代表图形里的内容<br />网格Grid，`True`显示网格（图中没开网格）<br />坐标轴Axis，横轴叫`x`坐标轴`label`，纵轴叫`y`坐标轴`ylabel`<br />上图的代码如下：
```python
plt.figure(figsize=(6, 3))

x = np.linspace(0, 10, 1000)
y1 = np.sin(x)
y2 = np.cos(x)

plt.plot(x, y1, color="blue", linewidth=1, label="sin")
plt.plot(x, y2, color="red", linewidth=1, label="cos")

plt.ylim(-1.1,1.1)

plt.xlabel('X') #x轴标签
plt.ylabel('Y') #y轴标签

plt.legend(loc='best') #图列位置，可选best，center等

plt.title("三角函数曲线图")
plt.show()
```
<a name="x8Lcr"></a>
##### 画图的整个流程
Matplotlib画图可以总结为3个步骤：**获取数据——画出基本图形——设置细节。**<br />按照这个维度（步骤），如果想按照子图分别显示两个曲线（Sinx 和Cosx），可以这样来分析。<br />第一部分、获取数据<br />横坐标的数据是用`numpy`模块生成0-10内等间隔的1000个整数，纵坐标为`sin`函数和`cos`函数。
```python
import numpy as np
x = np.linspace(0, 10, 1000)
y1 = np.sin(x)
y2 = np.cos(x)
```
第二部分、画出基本图形<br />先创建画布，设置画布尺寸，然后在画纸1上绘制`sinx`，并设置线条宽度为1，颜色为蓝色；在画纸2上绘制`cosx`，设置线条宽度为1，颜色为红色。
```python
plt.figure(figsize=(12, 2))
ax1 = plt.subplot(1,2,1)
plt.plot(x, y1, color="blue", linewidth=1, label="sin")
ax2 = plt.subplot(1,2,2)
plt.plot(x, y2, color="red", linewidth=1, label="cos")
```
第三部分、完善细节<br />添加横纵坐标标签、图例、标题等细节。
```python
plt.xlabel('X') #x轴标签
plt.ylabel('Y') #y轴标签
plt.legend(loc='best') #图列位置，可选best，center等
plt.title("sin函数曲线图")
plt.ylim(-1.1,1.1)
plt.show()
```
最终效果如图：<br />![outbox_legend.png](./img/1626229667968-d0dda9c5-7175-4de0-9f3a-3394f1db216e.png)
<a name="Zg3fl"></a>
##### 图表分类
按照数据之间的关系，可以把可视化视图划分为 4 类。

1. 比较：比较数据间的各类别

柱状图、条形图、雷达图

2. 联系：变量之间的关系

散点图、桑葚图

3. 构成：每个部分占整体的比例

饼图、环形图、旭日图、堆积图

4. 分布：关注变量的分布情况

散点图、直方图、地图、热力图、面积图<br />有了上文中的逻辑结构，虽然每种图形显示的意义以及绘图的参数不尽相同，但框架都是一样的，万变不离其宗。<br />剩下的就是调细节！<br />**原图附件**<br />[Matplotlib.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1618795736729-a00d1ae9-eb7c-4032-b8fc-41d165e8f5f6.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1618795736729-a00d1ae9-eb7c-4032-b8fc-41d165e8f5f6.zip%22%2C%22name%22%3A%22Matplotlib.zip%22%2C%22size%22%3A1813966%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22taskId%22%3A%22u767a38f5-15bd-48fc-b222-0316de989e4%22%2C%22taskType%22%3A%22transfer%22%2C%22mode%22%3A%22title%22%2C%22id%22%3A%22JFyQ3%22%2C%22card%22%3A%22file%22%7D)
<a name="jLjqA"></a>
## 1、图形类型
![1.webp](./img/1618795323847-276a472a-965b-43cf-9efc-20d54df6fb10.webp)
<a name="bX5lV"></a>
## 2、图例配置
![2.png](./img/1618795606475-b5c74f7d-c1ca-4974-842d-ebdecf9c7ebe.png)
<a name="Zdk6b"></a>
## 3、颜色选项
![3.jpg](./img/1618795618296-db7d2ab7-1a0b-44bd-93fe-d0ba7c13a7fb.jpeg)
<a name="P0Rb0"></a>
## 4、线条相关
![4.webp](./img/1618795628662-90f08fdb-1d22-4f6d-bca2-9e372d0e4061.webp)
<a name="Z3FnV"></a>
## 5、标记项配置
![5.jpg](./img/1618795639575-e5bafc94-fb77-4647-8d98-eac08adabdc1.jpeg)
<a name="Ah7aI"></a>
## 6、坐标轴相关配置
![6.png](./img/1618795653032-a707668a-bb1b-4e04-afaa-9e4e3f0e7b36.png)
<a name="tZDSY"></a>
## 7、更多的图形
![7.webp](./img/1618795666154-2c15a0b7-7b26-458e-b58f-714a8ff330db.webp)
<a name="aLChT"></a>
## 8、其他配置
![8.jpg](./img/1618795682261-3d8fb15b-e16c-49bd-8a99-42f08aa555ae.jpeg)![9.png](./img/1618795682119-717132df-7088-4772-9595-794addb49a57.png)
<a name="lDvcM"></a>
## 五张Matplotlib小抄汇总～
![10.png](./img/1618795545263-e59e6be6-904c-450c-8209-aac4fc605fd6.png)![11.webp](./img/1618795545022-d24c6792-85ab-42e9-a5ef-dd4e97e3ff74.webp)![12.webp](./img/1618795545010-7c08dc12-1ed8-41c9-8152-c54c0debb8f7.webp)![13.png](./img/1618795545200-7eeea23e-0775-49e9-a6e3-592b97dde100.png)![14.webp](./img/1618795545028-0376818f-20f9-4e1f-80b1-f5dbf5be132d.webp)
