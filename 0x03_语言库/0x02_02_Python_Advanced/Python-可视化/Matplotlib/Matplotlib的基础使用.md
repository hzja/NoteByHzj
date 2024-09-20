Python Matplotlib<br />Matplotlib 是一个 Python 的 2D绘图库，它以各种硬拷贝格式和跨平台的交互式环境生成出版质量级别的图形。通过 Matplotlib，开发者可以仅需要几行代码，便可以生成绘图，直方图，功率谱，条形图，错误图，散点图等。
> 环境配置：
> - numpy:1.18.5
> - pandas:1.0.5
> - matplotlib:3.2.1

<a name="2pT8t"></a>
# 1、简单的折线图
对于图表来说，最简单的莫过于作出一个单一函数的图像。使用下面的代码将需要的包载入到 notebook 中：
```python
%matplotlib inline
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')
import numpy as np
```
对于所有的 Matplotlib 图表来说，都需要从创建图形和维度开始。图形和维度可以使用下面代码进行最简形式的创建：
```python
fig = plt.figure()
ax = plt.axes()
```
![](./img/1600391695385-fbe7df15-3f8f-4dde-bdb0-a194d5fca7b4.png)<br />在 Matplotlib 中，图形（类`plt.Figure`的一个实例）可以被认为是一个包括所有维度、图像、文本和标签对象的容器。维度（类`plt.Axes`的一个实例）就是上面看到的图像，一个有边界的格子包括刻度和标签，最终还有画在上面的图表元素。使用变量名`fig`来指代图形对象，以及变量名`ax`来指代维度变量。<br />一旦创建了维度，可以使用`ax.plot`方法将数据绘制在图表上。下面是一个简单的正弦函数图形：
```python
fig = plt.figure()
ax = plt.axes()
x = np.linspace(0, 10, 1000)
ax.plot(x, np.sin(x));
```
![](./img/1600391695398-dac8cf89-0342-40d7-8516-953499da00c8.png)<br />同样的，可以使用 pylab 接口（MATLAB 风格的接口）在后台自动创建这两个对象：
```python
plt.plot(x, np.sin(x));
```
![](./img/1600391695422-d968ce33-117d-4807-a888-9da949ef1f77.png)<br />如果需要在同一幅图形中绘制多根线条，只需要多次调用`plot`函数即可：
```python
plt.plot(x, np.sin(x))
plt.plot(x, np.cos(x));
```
![](./img/1600391695426-cb57db60-c391-411f-842d-17b038419036.png)<br />这就是在 Matplotlib 中绘制简单函数图像的所有接口了。深入了解一下控制坐标轴和线条外观的细节。
<a name="aATtu"></a>
## 调整折线图：线条颜色和风格
可能第一个想到需要进行调整的部分就是线条的颜色和风格。`plt.plot()`函数接受额外的参数可以用来指定它们。通过指定`color`关键字参数可以调整颜色，这个字符串类型参数基本上能用来代表任何能想到的颜色。<br />更多颜色代码参考：[https://www.w3schools.com/colors/colors_names.asp](https://www.w3schools.com/colors/colors_names.asp)<br />可以通过多种方式指定颜色参数：
```python
plt.plot(x, np.sin(x - 0), color='blue')        # 通过颜色名称指定
plt.plot(x, np.sin(x - 1), color='g')           # 通过颜色简写名称指定(rgbcmyk)
plt.plot(x, np.sin(x - 2), color='0.75')        # 介于0-1之间的灰阶值
plt.plot(x, np.sin(x - 3), color='#FFDD44')     # 16进制的RRGGBB值
plt.plot(x, np.sin(x - 4), color=(1.0,0.2,0.3)) # RGB元组的颜色值，每个值介于0-1
plt.plot(x, np.sin(x - 5), color='chartreuse'); # 能支持所有HTML颜色名称值
```
![](./img/1600391695465-540a63bc-b1a9-48e5-851b-fe8102c07509.png)<br />如果没有指定颜色，Matplotlib 会在一组默认颜色值中循环使用来绘制每一条线条。<br />类似的，通过`linestyle`关键字参数可以指定线条的风格：
```python
plt.plot(x, x + 0, linestyle='solid')
plt.plot(x, x + 1, linestyle='dashed')
plt.plot(x, x + 2, linestyle='dashdot')
plt.plot(x, x + 3, linestyle='dotted');
# 还可以用形象的符号代表线条风格
plt.plot(x, x + 4, linestyle='-')  # 实线
plt.plot(x, x + 5, linestyle='--') # 虚线
plt.plot(x, x + 6, linestyle='-.') # 长短点虚线
plt.plot(x, x + 7, linestyle=':');  # 点线
```
![](./img/1600391695433-e9e46448-ed4d-47bd-9523-c46570c557a4.png)<br />如果喜欢更简洁的代码，这些`linestyle`和`color`参数能够合并成一个非关键字参数，传递给`plt.plot()`函数：
```python
plt.plot(x, x + 0, '-g')  # 绿色实线
plt.plot(x, x + 1, '--c') # 天青色虚线
plt.plot(x, x + 2, '-.k') # 黑色长短点虚线
plt.plot(x, x + 3, ':r');  # 红色点线
```
![image.gif](./img/1600391701732-ab1a52a6-c39e-4627-aa40-14cd0a228792.gif)上面的单字母颜色码是 RGB 颜色系统以及 CMYK 颜色系统的缩写，被广泛应用在数字化图像的颜色系统中。<br />还有很多其他的关键字参数可以对折线图的外观进行精细调整；可以通过在 IPython 中使用帮助工具查看`plt.plot()`函数的文档来获得更多细节内容。
<a name="LCCDe"></a>
## 调整折线图：坐标轴范围
Matplotlib 会自动选择非常合适的坐标轴范围来绘制图像，但是有些情况下也需要自己进行相关调整。使用`plt.xlim()`和`plt.ylim()`函数可以调整坐标轴的范围：
```python
plt.plot(x, np.sin(x))
plt.xlim(-1, 11)
plt.ylim(-1.5, 1.5);
```
![](./img/1600391695495-4dace7bb-4b5c-496c-8c2d-26348d5ecc19.png)<br />如果某些情况下希望将坐标轴反向，可以通过上面的函数实现，将参数顺序颠倒即可：
```python
plt.plot(x, np.sin(x))
plt.xlim(10, 0)
plt.ylim(1.2, -1.2);
```
![](./img/1600391695469-74d9d33c-78db-4c41-8c81-ea09cd26f17c.png)<br />相关的函数还有`plt.axis()`（注意：这不是`plt.axes()`函数，函数名称是 i 而不是 e）。这个函数可以在一个函数调用中就完成 x 轴和 y 轴范围的设置，传递一个`[xmin, xmax, ymin, ymax]`的列表参数即可：
```python
plt.plot(x, np.sin(x))
plt.axis([-1, 11, -1.5, 1.5]);
```
![](./img/1600391695501-b0d96b3d-3af0-4efd-8be1-d3c28b218745.png)<br />当然`plt.axis()`函数不仅能设置范围，还能像下面代码一样将坐标轴压缩到刚好足够绘制折线图像的大小：
```python
plt.plot(x, np.sin(x))
plt.axis('tight');
```
![](./img/1600391695492-a83cb067-32b9-480c-befa-eeae166a2e45.png)<br />还可以通过设置`'equal'`参数设置`x`轴与`y`轴使用相同的长度单位：
```python
plt.plot(x, np.sin(x))
plt.axis('equal');
```
![](./img/1600391695483-71be03a8-c644-413a-a0f1-e8e30d28697b.webp)<br />更多关于设置 axis 属性的内容请查阅`plt.axis`函数的文档字符串。
<a name="rVSQG"></a>
## 折线图标签
本节最后介绍一下在折线图上绘制标签：标题、坐标轴标签和简单的图例。<br />标题和坐标轴标签是最简单的这类标签，Matplotlib 提供了函数用来方便的设置它们：
```python
plt.plot(x, np.sin(x))
plt.title("A Sine Curve")
plt.xlabel("x")
plt.ylabel("sin(x)");
```
![](./img/1600391695574-f5161622-ce3c-4b12-b7a5-92f23c01e110.png)<br />这些标签的位置、大小和风格可以通过上面函数的可选参数进行设置。参阅 Matplotlib 在线文档和这些函数的文档字符串可以获得更多的信息。<br />当一幅图中绘制了多条折线时，如果能够绘制一个线条对应的图例能让图表更加清晰。Matplotlib 也内建了函数来快速创建图例。通过`plt.legend()`函数可以实现这个需求。虽然有很多种正确的方法来指定图例，最简单的方法是通过在绘制每条线条时指定对应的`label`关键字参数来使用这个函数：
```python
plt.plot(x, np.sin(x), '-g', label='sin(x)')
plt.plot(x, np.cos(x), ':b', label='cos(x)')
plt.axis('equal')
plt.legend();
```
![image.gif](./img/1600391701738-0e04209a-d2c4-4bc4-a95f-d92e11ef0c28.gif)上图可见，`plt.legend()`函数绘制的图例线条与图中的折线无论风格和颜色都保持一致。查阅`plt.legend`文档字符串可以获得更多相关信息；在[自定义图表图例]一节中也会讨论更高级的图例应用。
<a name="modXd"></a>
## 额外内容：Matplotlib 的坑
虽然大多数的`plt`函数都可以直接转换为`ax`的方法进行调用（例如`plt.plot()` → `ax.plot()`，`plt.legend()` → `ax.legend()`等），但是并不是所有的命令都能应用这种情况。特别是用于设置极值、标签和标题的函数都有一定的改变。下表列出了将 MATLAB 风格的函数转换为面向对象的方法的区别：

- `plt.xlabel()` → `ax.set_xlabel()`
- `plt.ylabel()` → `ax.set_ylabel()`
- `plt.xlim()` → `ax.set_xlim()`
- `plt.ylim()` → `ax.set_ylim()`
- `plt.title()` → `ax.set_title()`

在面向对象接口中，与其逐个调用上面的方法来设置属性，更常见的使用`ax.set()`方法来一次性设置所有的属性：
```python
ax = plt.axes()
ax.plot(x, np.sin(x))
ax.set(xlim=(0, 10), ylim=(-2, 2),
       xlabel='x', ylabel='sin(x)',
       title='A Simple Plot');
```
![](./img/1600391695575-58ff00a6-e8dd-4f5e-ab1a-a9c030b4b213.png)
<a name="8eWqG"></a>
# 2、简单散点图
另一种常用的图表类型是简单散点图，它是折线图的近亲。不像折线图，图中的点连接起来组成连线，散点图中的点都是独立分布的点状、圆圈或其他形状。首先将需要用到的图表工具和函数导入到 notebook 中：
```python
%matplotlib inline
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')
import numpy as np
```
<a name="OMTvT"></a>
## 使用 `plt.plot` 绘制散点图
在上面介绍了`plt.plot`/`ax.plot`方法绘制折线图。这两个方法也可以同样用来绘制散点图：
```python
x = np.linspace(0, 10, 30)
y = np.sin(x)
plt.plot(x, y, 'o', color='black');
```
![](./img/1600391695554-0d772a7b-d5c3-4c44-9d84-91745ec055f7.png)<br />传递给函数的第三个参数是使用一个字符代表的图表绘制点的类型。就像可以使用`'-'`或`'--'`来控制线条的风格那样，点的类型风格也可以使用短字符串代码来表示。所有可用的符号可以通过`plt.plot`文档或 Matplotlib 在线文档进行查阅。大多数的代码都是非常直观的，使用下面的例子可以展示那些最通用的符号：
```python
rng = np.random.RandomState(0)
for marker in ['o', '.', ',', 'x', '+', 'v', '^', '<', '>', 's', 'd']:
    plt.plot(rng.rand(5), rng.rand(5), marker,
             label="marker='{0}'".format(marker))
plt.legend(numpoints=1)
plt.xlim(0, 1.8);
```
![](./img/1600391695588-a019acbb-2c14-4b25-a951-295cc504a12e.png)<br />而且这些符号代码可以和线条、颜色代码一起使用，这会在折线图的基础上绘制出散点：
```python
plt.plot(x, y, '-ok');
```
![](./img/1600391695563-f8d2f72a-3848-41e9-b143-08d6c86a2bf7.png)<br />`plt.plot`还有很多额外的关键字参数用来指定广泛的线条和点的属性：
```python
plt.plot(x, y, '-p', color='gray',
         markersize=15, linewidth=4,
         markerfacecolor='white',
         markeredgecolor='gray',
         markeredgewidth=2)
plt.ylim(-1.2, 1.2);
```
![](./img/1600391695590-2034fb5e-fcc6-438a-8117-8214c7492644.png)<br />`plt.plot`函数的这种灵活性提供了很多的可视化选择。查阅`plt.plot`帮助文档获得完整的选项说明。
<a name="i66wa"></a>
## 使用`plt.scatter`绘制散点图
第二种更强大的绘制散点图的方法是使用`plt.scatter`函数，它的使用方法和`plt.plot`类似：
```python
plt.scatter(x, y, marker='o');
```
![](./img/1600391695614-107e6e3c-bf46-4b33-bfef-14a92606449f.png)<br />`plt.scatter`和`plt.plot`的主要区别在于，`plt.scatter`可以针对每个点设置不同属性（大小、填充颜色、边缘颜色等），还可以通过数据集合对这些属性进行设置。<br />通过一个随机值数据集绘制不同颜色和大小的散点图来说明。为了更好的查看重叠的结果，还使用了`alpha`关键字参数对点的透明度进行了调整：
```python
rng = np.random.RandomState(0)
x = rng.randn(100)
y = rng.randn(100)
colors = rng.rand(100)
sizes = 1000 * rng.rand(100)
plt.scatter(x, y, c=colors, s=sizes, alpha=0.3,
            cmap='viridis')
plt.colorbar();  # 显示颜色对比条
```
![](./img/1600391695585-bf4e6130-9c65-448a-94a1-f3f560ffd30c.png)<br />注意图表右边有一个颜色对比条（这里通过`colormap()`函数输出），图表中的点大小的单位是像素。使用这种方法，散点的颜色和大小都能用来展示数据信息，在希望展示多个维度数据集合的情况下很直观。<br />例如，当使用 Scikit-learn 中的鸢尾花数据集，里面的每个样本都是三种鸢尾花中的其中一种，并带有仔细测量的花瓣和花萼的尺寸数据：
```python
from sklearn.datasets import load_iris
iris = load_iris()
features = iris.data.T
plt.scatter(features[0], features[1], alpha=0.2,
            s=100*features[3], c=iris.target, cmap='viridis')
plt.xlabel(iris.feature_names[0])
plt.ylabel(iris.feature_names[1]);
```
![](./img/1600391695595-465218aa-4e70-4c7f-8570-6e99f93bf632.png)<br />可以从上图中看出，可以通过散点图同时展示该数据集的四个不同维度：图中的(x, y)位置代表每个样本的花萼的长度和宽度，散点的大小代表每个样本的花瓣的宽度，而散点的颜色代表一种特定的鸢尾花类型。如上图的多种颜色和多种属性的散点图对于分析和展示数据集时都非常有帮助。
<a name="cD2Qw"></a>
## `plot` 和 `scatter` 对比：性能提醒
除了上面说的`plt.plot`和`plt.scatter`对于每个散点不同属性的支持不同之外，还有别的因素影响对这两个函数的选择吗？对于小的数据集来说，两者并无差别，当数据集增长到几千个点时，`plt.plot`会明显比`plt.scatter`的性能要高。造成这个差异的原因是`plt.scatter`支持每个点使用不同的大小和颜色，因此渲染每个点时需要完成更多额外的工作。而`plt.plot`来说，每个点都是简单的复制另一个点产生，因此对于整个数据集来说，确定每个点的展示属性的工作仅需要进行一次即可。对于很大的数据集来说，这个差异会导致两者性能的巨大区别，因此，对于大数据集应该优先使用`plt.plot`函数。
<a name="9JCl8"></a>
# 3、误差可视化
对于任何的科学测量来说，精确计算误差与精确报告测量值基本上同等重要，如果不是更加重要的话。例如，设想正在使用一些天文物理学观测值来估算哈勃常数，即本地观测的宇宙膨胀系数。从一些文献中知道这个值大概是 71 (km/s)/Mpc，而测量得到的值是 74 (km/s)/Mpc,。这两个值是否一致？在仅给定这些数据的情况下，这个问题的答案是，无法回答。<br />Mpc（百万秒差距）<br />如果将信息增加一些，给出不确定性：最新的文献表示哈勃常数的值大约是 71  2.5 (km/s)/Mpc，测量值是 74  5 (km/s)/Mpc。这两个值是一致的吗？这就是一个可以准确回答的问题了。<br />在数据和结果的可视化中，有效地展示这些误差能使图表涵盖和提供更加完整的信息。
<a name="9oCT8"></a>
## 基础误差条
调用一个 Matplotlib 函数就能创建一个基础的误差条：
```python
%matplotlib inline
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')
import numpy as np
```
```python
x = np.linspace(0, 10, 50)
dy = 0.8
y = np.sin(x) + dy * np.random.randn(50)
plt.errorbar(x, y, yerr=dy, fmt='.k');
```
![](./img/1600391695637-f8c23c22-f463-41e3-80ac-c864da7330bb.png)<br />这里的`fmt`参数是用来控制线条和点风格的代码，与`plt.plot`有着相同的语法，参见[简单的折线图]和[简单的散点图]。<br />除了上面的基本参数，`errorbar`函数还有很多参数可以用来精细调节图表输出。使用这些参数可以很容易的个性化调整误差条的样式。通常将误差线条颜色调整为浅色会更加清晰，特别是在数据点比较密集的情况下：
```python
plt.errorbar(x, y, yerr=dy, fmt='o', color='black',
             ecolor='lightgray', elinewidth=3, capsize=0);
```
![](./img/1600391695706-85340ad8-a63b-4e03-9700-38d0ee87e82c.png)<br />除了上面介绍的参数，还可以指定水平方向的误差条（`xerr`），单边误差条和其他很多的参数。参阅`plt.errorbar`的帮助文档获得更多信息。
<a name="auHfh"></a>
## 连续误差
在某些情况下可能需要对连续值展示误差条。虽然 Matplotlib 没有內建的函数能直接完成这个任务，但是可以通过简单将`plt.plot`和`plt.fill_between`函数结合起来达到目标。<br />这里会采用简单的高斯过程回归方法，Scikit-Learn 提供了 API。这个方法非常适合在非参数化的函数中获得连续误差。在这里不会详细介绍高斯过程回归，仅仅聚焦在如何绘制连续误差本身：
:::warning
译者注：新版的 sklearn 修改了高斯过程回归实现方法，下面代码做了相应修改。
:::
```python
from sklearn.gaussian_process import GaussianProcessRegressor
# 定义模型和一些符合模型的点
model = lambda x: x * np.sin(x)
xdata = np.array([1, 3, 5, 6, 8])
ydata = model(xdata)
# 计算高斯过程回归，使其符合 fit 数据点
gp = GaussianProcessRegressor()
gp.fit(xdata[:, np.newaxis], ydata)
xfit = np.linspace(0, 10, 1000)
yfit, std = gp.predict(xfit[:, np.newaxis], return_std=True)
dyfit = 2 * std  # 两倍sigma ~ 95% 确定区域
```
现在有了`xfit`、`yfit`和`dyfit`，作为对数据的连续拟合值以及误差限。当然也可以像上面一样使用`plt.errorbar`绘制误差条，但是事实上不希望在图标上绘制 1000 个点的误差条。于是可以使用`plt.fill_between`函数在误差限区域内填充一道浅色的误差带来展示连续误差：
```python
# 可视化结果
plt.plot(xdata, ydata, 'or')
plt.plot(xfit, yfit, '-', color='gray')
plt.fill_between(xfit, yfit - dyfit, yfit + dyfit,
                 color='gray', alpha=0.2)
plt.xlim(0, 10);
```
![](./img/1600391695709-77ff224a-fca2-4fec-9c2e-6f8c10115d97.png)<br />注意上面调用`fill_between`函数：传递了的参数包括 x 值，y 值的低限，然后是 y 值的高限，结果是图表中介于低限和高限之间的区域会被填充。<br />上图提供了一个非常直观的高斯过程回归展示：在观测点的附近，模型会被限制在一个很小的区域内，反映了这些数据的误差比较小。在远离观测点的区域，模型开始发散，反映了这时的数据误差比较大。<br />如果需要获得`plt.fill_between`（以及类似的`plt.fill`函数）更多参数的信息，请查阅函数的帮助文档或 Matplotlib 在线文档。
<a name="30Qog"></a>
# 4、密度和轮廓图
有些情况下，需要在二维图表中使用轮廓或颜色区域来展示三维的数据（可以设想等高线地图或温度分布图）。Matplotlib 提供了三个有用的函数来处理这项任务：`plt.contour`绘制轮廓图，`plt.contourf`来绘制填充区域颜色的图表以及`plt.imshow`来展示图像。本节会介绍几个使用它们的例子。当然还是首先从将需要使用的包导入 notebook 和初始化工作开始：
```python
%matplotlib inline
import matplotlib.pyplot as plt
plt.style.use('seaborn-white')
import numpy as np
```
<a name="IKQVg"></a>
## 三维可视化函数
首先使用一个简单的函数  绘制一个轮廓图来进行说明，用来作为数组广播运算的例子：
```python
def f(x, y):
    return np.sin(x) ** 10 + np.cos(10 + y * x) * np.cos(x)
```
轮廓图可以使用`plt.contour`函数进行创建。它接收三个参数：x参数代表三维网格的平面横轴坐标，y参数代表三维网格的平面纵轴坐标，而z参数代表三维网格的高度坐标。最容易用来准备这种网格数据的是`np.meshgrid`函数，可以将两个一维的数组构造成一个二维的网格：
```python
x = np.linspace(0, 5, 50)
y = np.linspace(0, 5, 40)
X, Y = np.meshgrid(x, y)
Z = f(X, Y)
```
下面可以绘制标准的轮廓线图表：
```python
plt.contour(X, Y, Z, colors='black');
```
![](./img/1600391695673-e704dac6-9518-4aab-865b-65bc513cec2d.png)<br />图中值得注意的是，当使用单色绘制轮廓图时，虚线代表的是负数的数值，而实线代表的是正数。而轮廓线可以通过指定`cmap`参数来设置线条的色图。下例中展示了使用色图且绘制了更多的轮廓线的例子，会在整个数据范围区域内等距分布有 20 条轮廓线：
```python
plt.contour(X, Y, Z, 20, cmap='RdGy');
```
![](./img/1600391695745-6f15efa5-14ef-470e-b135-76ac07ec1057.png)<br />上例中选择了`RdGy`（Red-Gray的缩写）色图，这对于聚集的数据来说是一个不错的选择。Matplotlib 有大量的颜色图可供使用，可以通过在 IPython 中对`plt.cm`模块使用 TAB 自动补全方法就可以看到：
```python
plt.cm.<TAB>
```
上面的图看起来比第一幅图好多了，但是线条之间的空隙还是有点让人混淆。可以将上面的图改为填充轮廓图来解决这个问题，使用`plt.contourf()`函数（注意函数名最后有个 f，代表填充 fill），这个函数的语法基本上与`plt.contour()`保持一致。<br />并且加上了`plt.colorbar()`函数，这个函数会在图表边上创建一个颜色图例用以展示颜色所表示的数值区域：
```python
plt.contourf(X, Y, Z, 20, cmap='RdGy')
plt.colorbar();
```
![](./img/1600391695711-543af860-7909-4110-bd0e-7333b82ab561.png)<br />有了图例，很容易可以看出黑色区域代表着“峰”，而红色区域代表这“谷”。<br />上图有一个缺点，那就是图中颜色的阶梯是离散的而不是连续的，这通常不是想要的。可以通过设置很高的轮廓线数量来改善，但是这会导致绘制图表的性能降低：Matplotlib 必须在每个颜色阶梯上绘制一条新的轮廓多边形。更好的办法是使用`plt.imshow()`函数，它会将一个二维的网格图表转换为一张图像。<br />下面的例子展示了该方法：
```python
plt.imshow(Z, extent=[0, 5, 0, 5], origin='lower',
           cmap='RdGy')
plt.colorbar()
plt.axis(aspect='image');
```
```python
C:\Users\gdc\Anaconda3\lib\site-packages\ipykernel_launcher.py:4: MatplotlibDeprecationWarning: Passing unsupported keyword arguments to axis() will raise a TypeError in 3.3.
  after removing the cwd from sys.path.
```
![](./img/1600391695696-d52437f4-d261-460c-8ab4-bac8d8dbafd0.png)<br />然而，在使用`imshow()`的时候也有一些坑：

- `plt.imshow()`不接受 x 和 y 网格值作为参数，因此需要手动指定extent参数[xmin, xmax, ymin, ymax]来设置图表的数据范围。
- `plt.imshow()`使用的是默认的图像坐标，即左上角坐标点是原点，而不是通常图表的左下角坐标点。这可以通过设置`origin`参数来设置。
- `plt.imshow()`会自动根据输入数据调整坐标轴的比例；这可以通过参数来设置，例如，`plt.axis(aspect='image')`能让 x 和 y 轴的单位一致。

最后，有时可能需要将轮廓图和图像结合起来。例如，下例中使用了半透明的背景图像（通过`alpha`参数设置透明度），然后在背景图层之上绘制了轮廓图，并带有每个轮廓的数值标签（使用`plt.clabel()`函数绘制标签）：
```python
contours = plt.contour(X, Y, Z, 3, colors='black')
plt.clabel(contours, inline=True, fontsize=8)
plt.imshow(Z, extent=[0, 5, 0, 5], origin='lower',
           cmap='RdGy', alpha=0.5)
plt.colorbar();
```
![](./img/1600391695719-591276ff-3eea-42b5-b0f5-2e399ae850cb.png)<br />通过组合使用`plt.contour`、`plt.contourf`和`plt.imshow`这三个函数，基本可以满足绘制所有这种在二维图标上的三维数据的需求。需要了解更多函数的参数信息，参考它们的文档字符串。如果对于使用三维图表展示这种数据感兴趣，参见[在 matplotlib 中创建三维图表]。
<a name="WWoMH"></a>
# 5、直方图：分桶和密度
一个简单的直方图可以是开始理解数据集的第一步。前面看到了 Matplotlib 的直方图函数，可以用一行代码绘制基础的直方图，当然首先需要将需要用的包导入 notebook：
```python
%matplotlib inline
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('seaborn-white')
data = np.random.randn(1000)
```
```python
plt.hist(data);
```
![](./img/1600391695736-e3ee3cc5-b226-470d-8202-514590adc619.png)<br />`hist()`函数有很多的参数可以用来调整运算和展示；下面又一个更加个性化的直方图展示：
:::warning
译者注：normed 参数已经过时，此处对代码进行了相应修改，使用了替代的 density 参数。
:::
```python
plt.hist(data, bins=30, density=True, alpha=0.5,
         histtype='stepfilled', color='steelblue',
         edgecolor='none');
```
![](./img/1600391695805-f8dec205-7b2d-4ea9-970a-67d6d6875aaf.png)<br />`plt.hist`文档中有更多关于个性化参数的信息。联合使用`histtype='stepfilled'`和`alpha`参数设置透明度在对不同分布的数据集进行比较展示时很有用：
```python
x1 = np.random.normal(0, 0.8, 1000)
x2 = np.random.normal(-2, 1, 1000)
x3 = np.random.normal(3, 2, 1000)
kwargs = dict(histtype='stepfilled', alpha=0.3, density=True, bins=40)
plt.hist(x1, **kwargs)
plt.hist(x2, **kwargs)
plt.hist(x3, **kwargs);
```
![](./img/1600391695854-7d563fdb-798c-4dc2-8058-9a1611711912.png)<br />如果只是需要计算直方图的数值（即每个桶的数据点数量）而不是展示图像，`np.histogram()`函数可以完成这个目标：
```python
counts, bin_edges = np.histogram(data, bins=5)
print(counts)
```
```python
[ 49 273 471 183  24]
```
<a name="lbmNL"></a>
## 二维直方图和分桶
正如前面可以在一维上使用数值对应的直线划分桶一样，也可以在二维上使用数据对应的点来划分桶。首先定义数据集，从多元高斯分布中获得`x`和`y`数组：
```python
mean = [0, 0]
cov = [[1, 1], [1, 2]]
x, y = np.random.multivariate_normal(mean, cov, 10000).T
```
<a name="ZC3XP"></a>
### `plt.hist2d`：二维直方图
绘制二维直方图最直接的方法是使用 Matplotlib 的`plt.hist2d`函数：
```python
plt.hist2d(x, y, bins=30, cmap='Blues')
cb = plt.colorbar()
cb.set_label('counts in bin')
```
![](./img/1600391695779-80367fa1-8afe-4505-a586-d05d453c76b1.png)<br />类似`plt.hist`，`plt.hist2d`有许多额外的参数来调整分桶计算和图表展示，可以通过文档了解更多信息。而且，`plt.hist`有`np.histogram`，`plt.hist2d`也有其对应的函数`np.histogram2d`。如下例：
```python
counts, xedges, yedges = np.histogram2d(x, y, bins=30)
```
如果要获得更高维度的分桶结果，参见`np.histogramdd`函数文档。
<a name="Mu3m0"></a>
### `plt.hexbin`：六角形分桶
刚才的二维分桶是沿着坐标轴将每个桶分为正方形。另一个很自然的分桶形状就是正六边形。对于这个需求，Matplotlib 提供了`plt.hexbin`函数，它也是在二维平面上分桶展示，不过每个桶（即图表上的每个数据格）将会是六边形：
```python
plt.hexbin(x, y, gridsize=30, cmap='Blues')
cb = plt.colorbar(label='count in bin')
```
![](./img/1600391695828-cfa9185c-090f-42ff-883d-7e3846c09733.png)<br />`plt.hexbin`有许多有趣的参数，包括能对每个点设置权重和将每个桶的输出数据结果改为任意的 NumPy 聚合结果（带权重的平均值，带权重的标准差等）。
<a name="EAlkR"></a>
### 核密度估计
另外一个常用来统计多维数据密度的工具是核密度估计（KDE）。这目前只需要知道 KDE 被认为是一种可以用来填补数据的空隙并补充上平滑变化数据的方法就足够了。快速和简单的 KDE 算法已经在`scipy.stats`模块中有了成熟的实现。下面就一个简单的例子来说明如何使用 KDE 和绘制相应的二维直方图：
```python
from scipy.stats import gaussian_kde
# 产生和处理数据，初始化KDE
data = np.vstack([x, y])
kde = gaussian_kde(data)
# 在通用的网格中计算得到Z的值
xgrid = np.linspace(-3.5, 3.5, 40)
ygrid = np.linspace(-6, 6, 40)
Xgrid, Ygrid = np.meshgrid(xgrid, ygrid)
Z = kde.evaluate(np.vstack([Xgrid.ravel(), Ygrid.ravel()]))
# 将图表绘制成一张图像
plt.imshow(Z.reshape(Xgrid.shape),
           origin='lower', aspect='auto',
           extent=[-3.5, 3.5, -6, 6],
           cmap='Blues')
cb = plt.colorbar()
cb.set_label("density")
```
![](./img/1600391695804-1d93da93-6bf2-4b1b-a68a-a15d46473215.png)<br />KDE 有着光滑的长度，可以在细节和光滑度中有效的进行调节（一个例子是方差偏差权衡）。这方面有大量的文献介绍：高斯核密度估计`gaussian_kde`使用了经验法则来寻找输入数据附近的优化光滑长度值。<br />其他的 KDE 实现也可以在 SciPy 中找到，每一种都有它的优点和缺点；参见`sklearn.neighbors.KernelDensity`和`statsmodels.nonparametric.kernel_density.KDEMultivariate`。要绘制基于 KDE 进行可视化的图表，Matplotlib 写出的代码会比较冗长。
<a name="AkkKU"></a>
# 6、自定义图标图例
图例可以为可视化赋予实际含义，为不同的图标元素附上明确说明。前面看到了一些简单的图例创建例子；看一下在 Matplotlib 中自定义图例的位置和进行美化的方法。<br />可以使用`plt.legend()`函数来创建最简单的图例，这个函数能自动创建任何带有标签属性的图表元素的图例：
```python
import matplotlib.pyplot as plt
plt.style.use('classic')
```
```python
%matplotlib inline
import numpy as np
```
```python
x = np.linspace(0, 10, 1000)
fig, ax = plt.subplots()
ax.plot(x, np.sin(x), '-b', label='Sine')
ax.plot(x, np.cos(x), '--r', label='Cosine')
ax.axis('equal')
leg = ax.legend();
```
![](./img/1600391695888-7f3bfcbe-449f-406a-87df-747fa93ccf01.png)<br />但除此之外还有很多能自定义图例的方法。例如，可以指定图例位置并且去除边框：
```python
ax.legend(loc='upper left', frameon=False)
fig
```
![](./img/1600391695823-d31f14fc-911a-43ac-b1e6-0cb79b892422.png)<br />可以使用`ncol`属性设置图例中每行的列数：
```python
ax.legend(frameon=False, loc='lower center', ncol=2)
fig
```
![](./img/1600391695865-576ba7f0-f695-4e25-9861-12f1afee4996.png)<br />还可以使用圆角方框（`fancybox`）或者增加阴影，设置方框的透明度（alpha 值）或修改文字的边距：
```python
ax.legend(fancybox=True, framealpha=1, shadow=True, borderpad=1)
fig
```
![](./img/1600391695859-b455a9c8-c6b5-4a74-9049-2f3ba57bde74.png)<br />要获取更多 legend 函数的可用选项信息，请参考`plt.legend`的文档字符串。
<a name="I8NGZ"></a>
## 选择设置图例的元素
正如前面例子所示，绘制的图例默认包括所有带标签的元素。如果这不是想要的效果，可以调整哪些元素和标签会出现在图例当中，这可以通过设置 plot 函数或方法返回的对象实现。`plt.plot`函数能够同时产生多条折线，然后将这些线条的实例列表返回。将其中的部分实例传递到`plt.legend()`函数就能设置哪些线条会出现在图例中，再通过一个标签的列表指定图例的名称：
```python
y = np.sin(x[:, np.newaxis] + np.pi * np.arange(0, 2, 0.5))
lines = plt.plot(x, y)
# lines是一个线条实例的列表
plt.legend(lines[:2], ['first', 'second']);
```
![](./img/1600391695886-23158f77-3561-4341-bfa8-4a1150d7796d.png)<br />第一种方式可能更加清晰。通过将标签应用在图表元素上，然后绘制到图例中：
```
plt.plot(x, y[:, 0], label='first')
plt.plot(x, y[:, 1], label='second')
plt.plot(x, y[:, 2:])
plt.legend(framealpha=1, frameon=True);
```
![](./img/1600391696009-0976df2d-9073-4f8b-a415-02ae532477dd.png)<br />请注意默认情况下，legend 会忽略所有不带标签的元素。
<a name="mvRIP"></a>
## 散点大小的图例
某些情况下默认的图例不足以满足特定的可视化需求。例如，在使用散点的大小来标记数据的某个特征，然后希望创建一个相应的图例。下面的例子是加州城市人口的散点图，使用散点的大小表现该城市的面积，散点的颜色来表现城市的人口数量（自然对数值）。希望使用一个图例来指明散点尺寸的比例，同时用一个颜色条来说明人口数量，可以通过自定义绘制一些标签数据来实现尺寸图例：
:::warning
译者注：新版 Matplotlib 已经取消 aspect 参数，此处改为使用新的`'scaled'`参数调用 axis 函数。
:::
```python
import pandas as pd
cities = pd.read_csv(r'D:\python\Github学习材料\Python数据科学手册\data\california_cities.csv')
# 提取感兴趣的数据
lat, lon = cities['latd'], cities['longd']
population, area = cities['population_total'], cities['area_total_km2']
# 绘制散点图，使用尺寸代表面积，颜色代表人口，不带标签
plt.scatter(lon, lat, label=None,
            c=np.log10(population), cmap='viridis',
            s=area, linewidth=0, alpha=0.5)
plt.axis('scaled')
plt.xlabel('longitude')
plt.ylabel('latitude')
plt.colorbar(label='log$_{10}$(population)')
plt.clim(3, 7)
# 下面创建图例：
# 使用空列表绘制图例中的散点，使用不同面积和标签，带透明度
for area in [100, 300, 500]:
    plt.scatter([], [], c='k', alpha=0.3, s=area,
                label=str(area) + ' km$^2$')
plt.legend(scatterpoints=1, frameon=False, labelspacing=1, title='City Area')
plt.title('California Cities: Area and Population');
```
![](./img/1600391695885-64281aea-b18e-4c93-a5b9-9ffa472d45a3.png)<br />之前的图例都关联着图表上的一些对象，因此如果需要展示图例的话首先需要绘制图表元素。在上例中，需要的图例对象（灰色圆圈）不在图表上，因此采用绘制空列表的方式将它们仿造在图表上（实际上图上没有点），但是还是需要注意，只有那些带标签的元素才会出现在图例中。<br />通过绘制空列表，创建了三个带标签的对象，然后就可以出现在图例当中，这个图例就能表示出有关城市面积的相关信息。这个策略在很多复杂可视化图表构建过程中都被用到。<br />最后注意到这个图表实际上是一个地理位置图表，如果能在上面绘制州界线或其他地图相关的元素的话，会更加清晰。Matplotlib 提供了一个 Basemap 额外工具集来实现这个目标。
<a name="AxC2N"></a>
## 多重图例
有时候可能需要在同一个图表维度中设计多个图例。不幸的是，Matplotlib 并没有提供很简单的方式实现：通过标准的`legend`接口，只能在整张图表上创建一个图例。如果试图使用`plt.legend()`或`ax.legend()`创建第二个图例，那么第二条语句创建的图例会覆盖第一条语句创建的。只能通过从底层开始来创建一个新的图例 artist 这种方法来解决这个问题，然后使用`ax.add_artist()`的底层方法手动将第二个作者加到图表上：
```python
fig, ax = plt.subplots()
lines = []
styles = ['-', '--', '-.', ':']
x = np.linspace(0, 10, 1000)
for i in range(4):
    lines += ax.plot(x, np.sin(x - i * np.pi / 2),
                     styles[i], color='black')
ax.axis('equal')
# 指定第一个图例的线条和标签
ax.legend(lines[:2], ['line A', 'line B'],
          loc='upper right', frameon=False)
# 手动创建第二个图例，并将作者添加到图表中
from matplotlib.legend import Legend
leg = Legend(ax, lines[2:], ['line C', 'line D'],
             loc='lower right', frameon=False)
ax.add_artist(leg);
```
![](./img/1600391695914-e72b1473-2872-4de9-843f-590fd2ea24f3.png)<br />上例展示了用来组成任何 Matplotlib 图表的底层 artist 对象的简单说明。如果去查看`ax.legend()`的源代码（可以通过 IPython 的`ax.legend??`帮助工具做到），可以看到这个方法包含了用来构建合适`Legend`的 artist 对象的逻辑，构建的对象被保存在`legend_`属性当中，当绘制时被添加到图表上进行展示。
<a name="MNykO"></a>
# 7、个性化颜色条
图例可以将离散的点标示为离散的标签。对于建立在不同颜色之上的连续的值（点线面）来说，标注了的颜色条是非常方便的工具。Matplotlib 的颜色条是独立于图表之外的一个类似于比色卡的图形，用来展示图表中不同颜色的数值含义。因为本书是使用黑白打印的，本节内容中的所有带色彩的图都可以在(https://github.com/wangyingsm/Python-Data-Science-Handbook)中找到。首先导入本节需要的包和模块：
```python
import matplotlib.pyplot as plt
plt.style.use('classic')
```
```python
%matplotlib inline
import numpy as np
```
通过`plt.colorbar`函数可以创建最简单的颜色条：
```python
x = np.linspace(0, 10, 1000)
I = np.sin(x) * np.cos(x[:, np.newaxis])
plt.imshow(I)
plt.colorbar();
```
![](./img/1600391695964-4a9bd0c9-ea9e-4add-b8ee-fd27ee476504.png)<br />个性化颜色条以及在不同的场合高效的使用它们。
<a name="b5klX"></a>
## 自定义颜色条
颜色条可以通过`cmap`参数指定使用的色谱系统（或叫色图）：
```python
plt.imshow(I, cmap='gray');
```
![](./img/1600391695904-2dfdb5f2-abaf-412a-909a-6598b136e339.png)<br />所有可用的色图都可以在`plt.cm`模块中找到；在 IPython 中使用 Tab 自动补全功能能列出所有的色图列表：
```python
plt.cm.<TAB>
```
但是知道在哪里选择色图只是第一步：更重要的是在各种选项中选出合适的色图。这个选择比预料的要微妙的多。
<a name="kvTN2"></a>
### 选择色图
通常来说，应该注意以下三种不同类型的色图：

- 序列色图：这类型的色谱只包括一个连续序列的色系（例如`binary`或`viridis`）。
- 分化色图：这类型的色谱包括两种独立的色系，这两种颜色有着非常大的对比度（例如`RdBu`或`PuOr`）。
- 定性色图：这类型的色图混合了非特定连续序列的颜色（例如`rainbow`或`jet`）。

`jet`色图，在 Matplotlib 2.0 版本之前都是默认的色图，是定性色图的一个例子。`jet`作为默认色图的位置其实有点尴尬，因为定性图通常都不是对定量数据进行展示的好选择。原因是定性图通常都不能在范围增加时提供亮度的均匀增长。<br />可以通过将`jet`颜色条转换为黑白来看到这点：
```python
from matplotlib.colors import LinearSegmentedColormap
def grayscale_cmap(cmap):
    """返回给定色图的灰度版本"""
    cmap = plt.cm.get_cmap(cmap) # 使用名称获取色图对象
    colors = cmap(np.arange(cmap.N)) # 将色图对象转为RGBA矩阵，形状为N×4
    # 将RGBA颜色转换为灰度
    # 参考 http://alienryderflex.com/hsp.html
    RGB_weight = [0.299, 0.587, 0.114] # RGB三色的权重值
    luminance = np.sqrt(np.dot(colors[:, :3] ** 2, RGB_weight)) # RGB平方值和权重的点积开平方根
    colors[:, :3] = luminance[:, np.newaxis] # 得到灰度值矩阵
    # 返回相应的灰度值色图
    return LinearSegmentedColormap.from_list(cmap.name + "_gray", colors, cmap.N)
def view_colormap(cmap):
    """将色图对应的灰度版本绘制出来"""
    cmap = plt.cm.get_cmap(cmap)
    colors = cmap(np.arange(cmap.N))
    cmap = grayscale_cmap(cmap)
    grayscale = cmap(np.arange(cmap.N))
    fig, ax = plt.subplots(2, figsize=(6, 2),
                           subplot_kw=dict(xticks=[], yticks=[]))
    ax[0].imshow([colors], extent=[0, 10, 0, 1])
    ax[1].imshow([grayscale], extent=[0, 10, 0, 1])
```
```python
view_colormap('jet')
```
![](./img/1600391696001-61c029a8-1ba5-4e0c-b929-87f6a9f6e682.png)<br />注意一下上面的灰度图中亮条纹的位置。即使在上述彩色图中，也出现了这种不规则的亮条纹，这会导致眼睛被区域中亮条纹所吸引，这很可能造成阅读者被不重要的数据集部分干扰了。更好的选择是使用类似`viridis`这样的色图（Matplotlib 2.0 后默认色图），它们被设计为有着均匀的亮度变化。因此它们无论是在彩色图中还是在灰度图中都有着同样的亮度变化：
```python
view_colormap('viridis')
```
![](./img/1600391696094-5369c6ee-7732-40f2-85a7-2aa61831959d.png)<br />如果更喜欢彩虹方案，另一个好的选择是使用`cubehelix`色图：
```python
view_colormap('cubehelix')
```
![](./img/1600391696047-cc3db3a2-15dd-4e36-9e3f-5d62f5f406e6.png)<br />对于其他的情况，例如某种正负分布的数据集，双色颜色条如`RdBu`（Red-Blue）会很常用。然而正如从下面例子看到的，如果将双色颜色条转化为灰度的话，正负或两级的信息就会丢失：
```
view_colormap('RdBu')
```
![](./img/1600391696045-ae593b14-6ba2-4d6f-857e-64d96aa22bb6.png)<br />后面会看到更多使用这些色图的例子。<br />Matplotlib 中有大量可用的色图；要看到它们的列表，可以使用 IPython 来探索`plt.cm`模块。要在 Python 中更加正规的使用颜色，可以查看 Seaborn 库的工具和文档。
<a name="IfDA3"></a>
### 颜色限制和扩展
Matplotlib 允许对颜色条进行大量的自定义。颜色条本身就是一个`plt.Axes`对象，因此所有轴和刻度定制的技巧都可以应用在上面。颜色条也有着一些有趣的自定义行为：例如，可以缩小颜色的范围并且通过设置`extend`参数将超出范围之外的数值展示为顶部和底部的三角箭头形状。这对于展示一些受到噪声干扰的数据时非常方便：
```python
# 在I数组中人为生成不超过1%的噪声
speckles = (np.random.random(I.shape) < 0.01)
I[speckles] = np.random.normal(0, 3, np.count_nonzero(speckles))
plt.figure(figsize=(10, 3.5))
# 不考虑去除噪声时的颜色分布
plt.subplot(1, 2, 1)
plt.imshow(I, cmap='RdBu')
plt.colorbar()
# 设置去除噪声时的颜色分布
plt.subplot(1, 2, 2)
plt.imshow(I, cmap='RdBu')
plt.colorbar(extend='both')
plt.clim(-1, 1);
```
![](./img/1600391696068-a7df5b19-67d5-4e02-b802-f580b1e803d6.png)<br />注意到在左边的图表中，默认的颜色阈值是包括了噪声的，因此整体的条纹形状都被噪声数据冲刷淡化了。而右边的图表，手动设置了颜色的阈值，并在绘制颜色条是加上了`extend`参数来表示超出阈值的数据。对于数据来说，右图比左图要好的多。
<a name="7xUEz"></a>
### 离散颜色条
色图默认是连续的，但是在某些情况下可能需要展示离散值。最简单的方法是使用`plt.cm.get_cmap()`函数，在传递某个色图名称的同时，还额外传递一个颜色分桶的数量值参数给该函数：
```python
plt.imshow(I, cmap=plt.cm.get_cmap('Blues', 6))
plt.colorbar()
plt.clim(-1, 1);
```
![](./img/1600391696044-f445451f-d599-4232-bfeb-57a8569e3b7c.png)<br />离散色图的使用方式和其他色图没有任何区别。
<a name="R5hhF"></a>
## 例子：手写数字
最后看一个很有实用价值的例子，让实现对一些手写数字图像数据的可视化分析。这个数据包含在 Sciki-Learn 中，以供包含有将近 2,000 张  大小的不同笔迹的手写数字缩略图。<br />首先，下载这个数据集，然后使用`plt.imshow()`将其中部分数据展示出来：
```python
# 读取数字0-5的手写图像，然后使用Matplotlib展示头64张缩略图
from sklearn.datasets import load_digits
digits = load_digits(n_class=6)
fig, ax = plt.subplots(8, 8, figsize=(6, 6))
for i, axi in enumerate(ax.flat):
    axi.imshow(digits.images[i], cmap='binary')
    axi.set(xticks=[], yticks=[])
```
![](./img/1600391696058-d5cff5f0-a415-421f-9e7f-de7700c53d3c.png)<br />因为每个数字都是使用 64 个像素点渲染出来的，可以认为每个数字是一个 64 维空间中的点：每个维度代表这其中一个像素的灰度值。但是要在图表中将这么高维度空间的联系可视化出来是非常困难的。有一种做法是使用降维技术，比方说使用流形学习来减少数据的维度然而不会丢失数据中有效的信息。<br />看一下将这些手写数字图像数据映射到二维流形学习当中：
```python
# 使用Isomap将手写数字图像映射到二维流形学习中
from sklearn.manifold import Isomap
iso = Isomap(n_components=2)
projection = iso.fit_transform(digits.data)
```
使用离散颜色条来展示结果，设置`ticks`和`clim`来进一步美化结果的颜色条：
```python
# 绘制图表结果
plt.scatter(projection[:, 0], projection[:, 1], lw=0.1,
            c=digits.target, cmap=plt.cm.get_cmap('cubehelix', 6))
plt.colorbar(ticks=range(6), label='digit value')
plt.clim(-0.5, 5.5)
```
![](./img/1600391696117-7135ef65-246e-43ce-bc60-efb5ccbbc906.png)<br />从流形学习中的映射中可以观察到一些有趣现象：例如，图表中 5 和 3 有一些重叠的部分，这表示一些手写体中 5 和 3 是比较难以辨别的，因此对于自动识别算法来说这是比较容易混淆的部分。而 0 和 1，它们在图表中距离很远，这表示两者比较容易辨别，不太可能造成混淆。这个图表分析与直觉一致，因为 5 和 3 显然比 0 和 1 看起来更加接近。
<a name="wWntm"></a>
# 8、多个子图表
在一些情况中，如果能将不同的数据图表并列展示，对于进行数据分析和比较会很有帮助。Matplotlib 提供了子图表的概念来实现这一点：单个图表中可以包括一组小的 axes 用来展示多个子图表。这些子图表可以是插图，网格状分布或其他更复杂的布局。Matplotlib 中用来构建子图表的四个函数。
```python
%matplotlib inline
import matplotlib.pyplot as plt
plt.style.use('seaborn-white')
import numpy as np
```
<a name="oP4xA"></a>
## `plt.axes`：手动构建子图表
构建 axes 作为子图表的最基础方法就是使用`plt.axes`函数。正如前面已经看到，默认情况下，这个函数够创建一个标准的 axes 对象填满整个图表区域。`plt.axes`函数也可以接收一个可选的列表参数用来指定在 axes 在整个图表中的坐标点位置。列表中有四个数值分别为`[left, bottom, width, height]`（取值都是 0-1），代表着子图表的左边、底部、宽度、高度在整个图表中左边、底部、宽度、高度所占的比例值。<br />例如，可以在距离左边和底部 65%的位置，以插图的形式放置一个宽度和高度都是 20%子图表，上述数值应该为`[0.65, 0.65, 0.2, 0.2]`：
```python
ax1 = plt.axes()  # 标准图表
ax2 = plt.axes([0.65, 0.65, 0.2, 0.2]) #子图表
```
![](./img/1600391696129-aeb12782-4c20-41b8-b5d0-325d05339e5b.png)<br />与上述等价的面向对象接口的语法是`fig.add_axes()`。使用这个方法来创建两个垂直堆叠的子图表：
```python
fig = plt.figure() # 获得figure对象
ax1 = fig.add_axes([0.1, 0.5, 0.8, 0.4],
                   xticklabels=[], ylim=(-1.2, 1.2)) # 左边10% 底部50% 宽80% 高40%
ax2 = fig.add_axes([0.1, 0.1, 0.8, 0.4],
                   ylim=(-1.2, 1.2)) # 左边10% 底部10% 宽80% 高40%
x = np.linspace(0, 10)
ax1.plot(np.sin(x))
ax2.plot(np.cos(x));
```
![](./img/1600391696135-9181cc60-2a35-49d2-8a6f-3fc97f5bb411.png)<br />这样就有两个子图表（上面的子图表没有 x 轴刻度），这两个子图表正好吻合：上面图表的底部是整个图表高度 50%位置，而下面图表的顶部也是整个图表的 50%位置（0.1+0.4）。
<a name="0y7wt"></a>
## `plt.subplot`：简单网格的子图表
将子图表的行与列对齐是一个很常见的需求，因此 Matplotlib 提供了一些简单的函数来实现它们。这些函数当中最底层的是`plt.subplot()`，它会在网格中创建一个子图表。函数接受三个整数参数，网格行数，网格列数以及该网格子图表的序号（从左上角向右下角递增）：
```python
for i in range(1, 7):
    plt.subplot(2, 3, i)
    plt.text(0.5, 0.5, str((2, 3, i)),
             fontsize=18, ha='center')
```
![](./img/1600391696163-d699fc86-8fad-4f84-a00a-d841513a8d92.png)<br />`plt.subplots_adjust`函数用来调整这些子图表之间的距离。下面的代码使用了与`plt.subplot()`等价的面向对象接口方法`fig.add_subplot()`：
```python
fig = plt.figure()
fig.subplots_adjust(hspace=0.4, wspace=0.4)
for i in range(1, 7):
    ax = fig.add_subplot(2, 3, i)
    ax.text(0.5, 0.5, str((2, 3, i)),
           fontsize=18, ha='center')
```
![](./img/1600391696135-5dc86226-7f33-4c31-9fc4-9531cabaabc2.png)<br />上例中指定了`plt.subplots_adjust`函数的`hspace`和`wspace`参数，它们代表这沿着高度和宽度方向子图表之间的距离，单位是子图表的大小（在本例中，距离是子图表宽度和高度的 40%）。
<a name="HWqBp"></a>
## `plt.subplots`：一句代码设置所有网格子图表
上面的方法当需要创建大量的子图表网格时会变得非常冗长乏味，特别是如果需要将内部图表 x 轴和 y 轴标签隐藏的情况下。因此，`plt.subplots`在这种情况下是一个合适的工具（注意末尾有个 s）。这个函数会一次性创建所有的网格子图表，而不是单个网格，并将它们在一个 NumPy 数组中返回。参数是行数和列数，还有两个可选的关键字参数`sharex`和`sharey`，可以指定不同子图表之间的关联。<br />下面来创建一个  网格的子图表，其中每一行的子图表共享它们的 y 轴，而每一列的子图表共享它们的 x 轴：
```python
fig, ax = plt.subplots(2, 3, sharex='col', sharey='row')
```
![](./img/1600391696112-381bf853-a676-4f40-8a74-59be7d81a20d.png)<br />注意上面设置了`sharex`和`sharey`之后，内部子图表的 x 轴和 y 轴的标签就自动被去掉了。返回值中 ax 是一个 NumPy 数组，里面含有每一个子图表的实例，可以使用 NumPy 索引的语法很简单的获得它们：
```python
# axes是一个2×3的数组，可以通过[row, col]进行索引访问
for i in range(2):
    for j in range(3):
        ax[i, j].text(0.5, 0.5, str((i, j)),
                      fontsize=18, ha='center')
fig
```
![](./img/1600391696147-bfe3d524-e7a7-4a7f-aecf-5b1cac5013f3.png)<br />并且相对于`plt.subplot`，`plt.subplots()`更复合 Python 从 0 开始进行索引的习惯。
<a name="P0pv3"></a>
## `plt.GridSpec`：更复杂的排列
当需要子图表在网格中占据多行或多列时，`plt.GridSpec()`正是所需要的。`plt.GridSpec()`对象并不自己创建图表；它只是一个可以被传递给`plt.subplot()`的参数。例如，一个两行三列并带有指定的宽度高度间隔的 gridspec 可以如下创建：
```python
grid = plt.GridSpec(2, 3, wspace=0.4, hspace=0.3)
```
使用这个对象可以指定子图表的位置和占据的网格，仅需要使用熟悉的 Python 切片语法即可：
```python
plt.subplot(grid[0, 0])
plt.subplot(grid[0, 1:])
plt.subplot(grid[1, :2])
plt.subplot(grid[1, 2]);
```
![](./img/1600391696135-6bd1623c-64b5-47b8-884d-8be36bba10e1.png)<br />这种灵活的网格对齐控制方式有着广泛的应用。在需要创建多个直方图的联合图表中使用这种方法，如下例：
```python
# 构建二维正态分布数据
mean = [0, 0]
cov = [[1, 1], [1, 2]]
x, y = np.random.multivariate_normal(mean, cov, 3000).T
# 使用GridSpec创建网格并加入子图表
fig = plt.figure(figsize=(6, 6))
grid = plt.GridSpec(4, 4, hspace=0.2, wspace=0.2)
main_ax = fig.add_subplot(grid[:-1, 1:])
y_hist = fig.add_subplot(grid[:-1, 0], xticklabels=[], sharey=main_ax)
x_hist = fig.add_subplot(grid[-1, 1:], yticklabels=[], sharex=main_ax)
# 在主图表中绘制散点图
main_ax.plot(x, y, 'ok', markersize=3, alpha=0.2)
# 分别在x轴和y轴方向绘制直方图
x_hist.hist(x, 40, histtype='stepfilled',
            orientation='vertical', color='gray')
x_hist.invert_yaxis() # x轴方向（右下）直方图倒转y轴方向
y_hist.hist(y, 40, histtype='stepfilled',
            orientation='horizontal', color='gray')
y_hist.invert_xaxis() # y轴方向（左上）直方图倒转x轴方向
```
![](./img/1600391696147-01225b3d-7a2a-4cae-bd2d-18af69c48094.png)<br />这种沿着数据各自方向分布并绘制相应图表的需求是很通用的，因此在 Seaborn 包中它们有专门的 API 来实现
<a name="27Hz7"></a>
# 9、文本和标注
创建一个优秀的可视化图表的关键在于引导读者，让他们能理解图表所讲述的故事。在一些情况下，这个故事可以通过纯图像的方式表达，不需要额外添加文字，但是在另外一些情况中，图表需要文字的提示和标签才能将故事讲好。也许标注最基本的类型就是图表的标签和标题，但是其中的选项参数却有很多。使用一些数据来创建可视化图表并标注这些图表来表达这些有趣的信息。首先还是需要将要用到的模块和包导入 notebook：
```python
%matplotlib inline
import matplotlib.pyplot as plt
import matplotlib as mpl
plt.style.use('seaborn-whitegrid')
import numpy as np
import pandas as pd
```
<a name="DYbey"></a>
## 例子：节假日对美国出生率的影响
本例中的数据可以在 [https://raw.githubusercontent.com/jakevdp/data-CDCbirths/master/births.csv](https://raw.githubusercontent.com/jakevdp/data-CDCbirths/master/births.csv) 下载。<br />先按照前面的方式进行同样的数据清洗程序，然后以图表展示这个结果：
```python
births = pd.read_csv(r'D:\python\Github学习材料\Python数据科学手册\data\births.csv')
quartiles = np.percentile(births['births'], [25, 50, 75])
mu, sig = quartiles[1], 0.74 * (quartiles[2] - quartiles[0])
births = births.query('(births > @mu - 5 * @sig) & (births < @mu + 5 * @sig)')
births['day'] = births['day'].astype(int)
births.index = pd.to_datetime(10000 * births.year +
                              100 * births.month +
                              births.day, format='%Y%m%d')
births_by_date = births.pivot_table('births',
                                    [births.index.month, births.index.day])
births_by_date.index = [pd.datetime(2012, month, day)
                        for (month, day) in births_by_date.index]
```
```python
C:\Users\gdc\Anaconda3\lib\site-packages\ipykernel_launcher.py:15: FutureWarning: The pandas.datetime class is deprecated and will be removed from pandas in a future version. Import from datetime module instead.
  from ipykernel import kernelapp as app
```
```python
fig, ax = plt.subplots(figsize=(12, 4))
births_by_date.plot(ax=ax);
```
![](./img/1600391696177-e7096c3a-2099-4185-aed2-c4a65d5f57d0.png)<br />绘制了这样的图表来表达数据时，如果能对一些图表的特性作出标注来吸引注意力通常是非常有帮助的。这可以通过调用`plt.text`或`ax.text`函数来实现，它们可以在某个特定的 x，y 轴位置输出一段文字：
```python
fig, ax = plt.subplots(figsize=(12, 4))
births_by_date.plot(ax=ax)
# 在折线的特殊位置标注文字
style = dict(size=10, color='gray')
ax.text('2012-1-1', 3950, "New Year's Day", **style)
ax.text('2012-7-4', 4250, "Independence Day", ha='center', **style)
ax.text('2012-9-4', 4850, "Labor Day", ha='center', **style)
ax.text('2012-10-31', 4600, "Halloween", ha='right', **style)
ax.text('2012-11-25', 4450, "Thanksgiving", ha='center', **style)
ax.text('2012-12-25', 3850, "Christmas ", ha='right', **style)
# 设置标题和y轴标签
ax.set(title='USA births by day of year (1969-1988)',
       ylabel='average daily births')
# 设置x轴标签月份居中
ax.xaxis.set_major_locator(mpl.dates.MonthLocator())
ax.xaxis.set_minor_locator(mpl.dates.MonthLocator(bymonthday=15))
ax.xaxis.set_major_formatter(plt.NullFormatter())
ax.xaxis.set_minor_formatter(mpl.dates.DateFormatter('%h'));
```
![](./img/1600391696205-522b21b2-7ce8-44b8-bc04-e3e42be4b062.png)<br />`ax.text`方法接收 x 位置、y 位置、一个字符串和额外可选的关键字参数可以用来设置颜色、大小、样式、对齐等文本格式。上面使用了`ha='right'`和`ha='center'`，这里的`ha`是*hirizonal alignment（水平对齐）*的缩写。要查阅更多的可用参数，请查看`plt.text()`和`mpl.text.Text()`的文档字符串内容。
<a name="5IA08"></a>
## 转换和文本位置
在刚才的例子中，将文字标注根据数据位置进行了定位。有些时候需要将文字标注独立于数据位置而根据图表位置进行定位。Matplotlib 通过转换完成这项工作。<br />任何的图形显示框架都需要在坐标系统之间进行转换的机制。例如，一个数据点位于  被转换为图表中的某个位置，进而转换为屏幕上显示的像素。这样的坐标转换在数学上都相对来说比较直接，，而且 Matplotlib 提供了一系列的工具实现了转换（这些工具可以在`matplotlib.transforms`模块中找到）。<br />一般来说，用户很少需要关注这些转换的细节，但是当考虑将文本在图表上展示时，这些知识却比较有用。在这种情况中，下面三种定义好的转换是比较有用的：

- `ax.transData`：与数据坐标相关的转换
- `ax.tranAxes`：与 Axes 尺寸相关的转换（单位是 axes 的宽和高）
- `ax.tranFigure`：与 figure 尺寸相关的转换（单位是 figure 的宽和高）

下面来看看使用这些转换将文字写在图表中不同位置的例子：
```python
fig, ax = plt.subplots(facecolor='lightgray')
ax.axis([0, 10, 0, 10])
# transform=ax.transData是默认的，这里写出来是为了明确对比
ax.text(1, 5, ". Data: (1, 5)", transform=ax.transData)
ax.text(0.5, 0.1, ". Axes: (0.5, 0.1)", transform=ax.transAxes)
ax.text(0.2, 0.2, ". Figure: (0.2, 0.2)", transform=fig.transFigure);
```
![](./img/1600391696187-25efff4d-ecd4-43b0-a016-028a1cbf9d1c.png)<br />注意默认情况下，文字是在指定坐标位置靠左对齐的：这里每个字符串开始的"."的位置就是每种转换的坐标位置。<br />`transData`坐标给定的是通常使用的 x 和 y 轴坐标位置。`transAxes`坐标给定的是从 axes 左下角开始算起（白色区域）的坐标位置，使用的是宽度和长度的占比。`transFigure`坐标类似，给定的是从 figure 左下角开始算起（灰色区域）的坐标位置，使用的也是宽度和长度的占比。<br />因此如果改变了轴的最大长度，只有`transData`坐标会收到影响，其他两个还是保持在相同位置：
```python
ax.set_xlim(0, 2)
ax.set_ylim(-6, 6)
fig
```
![](./img/1600391696220-4eb16a2c-cbde-4006-893c-613a535e9e0e.png)<br />这个变化可以通过动态改变轴的最大长度看的更加清楚：如果在 notebook 执行这段代码，可以将`%matplotlib inline`改为`%matplotlib notebook`，然后使用图表的菜单来交互式的改变图表。
<a name="SLJI0"></a>
## 箭头和标注
除了刻度标签和文字标签，另一种常用的标注是箭头。<br />在 Matplotlib 中绘制箭头通常比想象的难得多。虽然有`plt.arrow()`函数，但是不建议使用它：这个函数绘制的箭头是一个 SVG 对象，因此在图表使用不同的比例的情况会产生问题，结果通常不能让用户满意。因此，建议使用`plt.annotate()`函数。这个函数会绘制一些文字以及一个箭头，并且箭头可以非常灵活的进行配置。<br />下面提供一些参数来使用`annotate`函数：
```python
%matplotlib inline
fig, ax = plt.subplots()
x = np.linspace(0, 20, 1000)
ax.plot(x, np.cos(x))
ax.axis('equal')
ax.annotate('local maximum', xy=(6.28, 1), xytext=(10, 4),
            arrowprops=dict(facecolor='black', shrink=0.05))
ax.annotate('local minimum', xy=(5 * np.pi, -1), xytext=(2, -6),
            arrowprops=dict(arrowstyle="->",
                            connectionstyle="angle3,angleA=0,angleB=-90"));
```
![](./img/1600391696206-6facf96d-8b7d-445a-b146-78d68ed04dd5.png)<br />箭头的样式是使用`箭头属性`字典值进行控制的，里面有很多可用的参数。这些参数在 Matplotlib 的在线文档中已经有了很详细的说明，因此在这里就不将这部分内容重复介绍一遍了。在前面出生率图上再使用一些参数进行更多的说明：
```python
fig, ax = plt.subplots(figsize=(12, 4))
births_by_date.plot(ax=ax)
# 为图表添加标注
ax.annotate("New Year's Day", xy=('2012-1-1', 4100),  xycoords='data',
            xytext=(50, -30), textcoords='offset points',
            arrowprops=dict(arrowstyle="->",
                            connectionstyle="arc3,rad=-0.2"))
ax.annotate("Independence Day", xy=('2012-7-4', 4250),  xycoords='data',
            bbox=dict(boxstyle="round", fc="none", ec="gray"),
            xytext=(10, -40), textcoords='offset points', ha='center',
            arrowprops=dict(arrowstyle="->"))
ax.annotate('Labor Day', xy=('2012-9-4', 4850), xycoords='data', ha='center',
            xytext=(0, -20), textcoords='offset points')
ax.annotate('', xy=('2012-9-1', 4850), xytext=('2012-9-7', 4850),
            xycoords='data', textcoords='data',
            arrowprops={'arrowstyle': '|-|,widthA=0.2,widthB=0.2', })
ax.annotate('Halloween', xy=('2012-10-31', 4600),  xycoords='data',
            xytext=(-80, -40), textcoords='offset points',
            arrowprops=dict(arrowstyle="fancy",
                            fc="0.6", ec="none",
                            connectionstyle="angle3,angleA=0,angleB=-90"))
ax.annotate('Thanksgiving', xy=('2012-11-25', 4500),  xycoords='data',
            xytext=(-120, -60), textcoords='offset points',
            bbox=dict(boxstyle="round4,pad=.5", fc="0.9"),
            arrowprops=dict(arrowstyle="->",
                            connectionstyle="angle,angleA=0,angleB=80,rad=20"))
ax.annotate('Christmas', xy=('2012-12-25', 3850),  xycoords='data',
             xytext=(-30, 0), textcoords='offset points',
             size=13, ha='right', va="center",
             bbox=dict(boxstyle="round", alpha=0.1),
             arrowprops=dict(arrowstyle="wedge,tail_width=0.5", alpha=0.1));
# 设置图表标题和坐标轴标记
ax.set(title='USA births by day of year (1969-1988)',
       ylabel='average daily births')
# 设置月份坐标居中显示
ax.xaxis.set_major_locator(mpl.dates.MonthLocator())
ax.xaxis.set_minor_locator(mpl.dates.MonthLocator(bymonthday=15))
ax.xaxis.set_major_formatter(plt.NullFormatter())
ax.xaxis.set_minor_formatter(mpl.dates.DateFormatter('%h'));
ax.set_ylim(3600, 5400);
```
![](./img/1600391696301-a7bc6e8e-b44f-4a6b-bb0f-2637d8c1f8af.png)<br />上图中箭头和文字框都非常详尽了：可以看出几乎可以使用`plt.annotate`创建任何想要的箭头样式。不幸的是，这意味着这种特性都需要手工进行调整，因此如果需要获得印刷质量的图像，这将是一个非常耗费时间的工作。最后，必须指出，上述这种多种样式混合的方式来展现数据肯定不是最佳实践，这里只是为了尽可能多的介绍可用的参数。<br />更多关于 Matplotlib 的箭头和标注样式的讨论和例子可以访问 Matplotlib gallery。
<a name="blbH7"></a>
# 10、自定义刻度
Matplotlib 默认的刻度标志和格式被设计成能满足许多通用场景的需求，但是不会是所有图表的最佳选择。本节会介绍一些调整刻度位置和格式的例子来说明自定义刻度的使用。<br />在介绍例子之前，应该加深对 Matplotlib 图表的对象层次的理解。Matplotlib 的设计目标是展示在图表中的所有内容都会表达成为 Python 的对象：例如，回忆前面介绍过`figure`指的是用来展示图表所有内容的方框。每个 Matplotlib 对象也被设计为其子对象的一个容器：例如`figure`对象中可以包含一个或多个`axes`对象，每个`axes`对象都依次包含着其他用来展示图表的内容对象。<br />刻度也不例外。每个`axes`对象都有着属性`xaxis`和`yaxis`，表示 x 和 y 轴，其中包含着所有的属性用来指代轴的线、刻度和标签。
<a name="ptDq6"></a>
## 主要的和次要的刻度
在每个坐标轴上，都有主要的刻度和次要的刻度概念。正如名字指代的，主要刻度通常是大的和更多用到的，而次要刻度通常是小的。默认 Matplotlib 很少使用次要刻度，但是在对数图表中可能会看到它们：<br />在 Matplotlib 2.0 之后，当 axis 的跨度过大时，默认次要刻度将会不再展示，因此，下面的代码经过了修改，加上了 xlim 和 ylim 参数。
```python
import matplotlib.pyplot as plt
plt.style.use('classic')
%matplotlib inline
import numpy as np
```
```python
ax = plt.axes(xscale='log', yscale='log', xlim=[10e-5, 10e5], ylim=[10e-5, 10e5])
ax.grid();
```
![](./img/1600391696227-66362296-bb73-40ea-bf80-fb89925f7b84.png)<br />看到每个主要刻度显示了一个大的标志和标签，而每个次要刻度显示了一个小的刻度标志没有标签。<br />这些刻度属性，位置和标签，都可以使用每个轴的`formatter`和`locator`对象进行个性化设置。下面查看一下 x 轴的相应对象：
```python
print(ax.xaxis.get_major_locator())
print(ax.xaxis.get_minor_locator())
```
```python
<matplotlib.ticker.LogLocator object at 0x000001E8074AF108>
<matplotlib.ticker.LogLocator object at 0x000001E8074AD908>
```
```python
print(ax.xaxis.get_major_formatter())
print(ax.xaxis.get_minor_formatter())
```
```python
<matplotlib.ticker.LogFormatterSciNotation object at 0x000001E8074AEB88>
<matplotlib.ticker.LogFormatterSciNotation object at 0x000001E8074ADB48>
```
看到主要和次要刻度的位置都是使用`LogLocator`来设置的（对于对数图表来说那是理所当然的）。然而次要刻度的标签的格式是`NullFormatter`：这表示次要刻度不会显示标签。
:::warning
译者注：新版 Matplotlib 已经修改，可以看到 Formatter 都统一成为了 LogFormatterSciNotation，再根据图表实际情况选择是否展示标签。
:::
下面就可以开始介绍一些设置这些 locator 和 formatter 的例子了。
<a name="NNm8T"></a>
## 隐藏刻度和标签
也许最常见的刻度/标签格式设置的操作是隐藏刻度或标签。这可以通过使用`plt.NullLocator()`和`plt.NullFormatter()`来设置，如下例：
```python
ax = plt.axes()
ax.plot(np.random.rand(50))
ax.yaxis.set_major_locator(plt.NullLocator())
ax.xaxis.set_major_formatter(plt.NullFormatter())
```
![](./img/1600391696236-a0e52738-6167-4801-962c-9c9a07f00775.png)<br />注意上图中去除了 x 轴的标签（但是保留了刻度或网格线），y 轴的刻度和标签都被去除了。图表中没有刻度和标签在很多情况下很有用，例如，希望展示一个图像的网格。比方说，考虑下面的图表，包含着不同的头像，一个很常见的有监督机器学习问题：
```python
fig, ax = plt.subplots(5, 5, figsize=(5, 5))
fig.subplots_adjust(hspace=0, wspace=0)
# 从scikit-learn载入头像数据集
from sklearn.datasets import fetch_olivetti_faces
faces = fetch_olivetti_faces().images
for i in range(5):
    for j in range(5):
        ax[i, j].xaxis.set_major_locator(plt.NullLocator())
        ax[i, j].yaxis.set_major_locator(plt.NullLocator())
        ax[i, j].imshow(faces[10 * i + j], cmap="bone")
```
```python
downloading Olivetti faces from
https://ndownloader.figshare.com/files/5976027
to C:\Users\gdc\scikit_learn_data
```
![](./img/1600391696248-d8c080c8-cac4-47b3-95f8-421862bbca89.png)<br />注意上图中每张图像都有它自己的 axes，将每一个 axes 的 locator 都设置为 null 因为这些刻度值（像素值）在这里并没有任何实际意义。
<a name="YJtSd"></a>
## 减少或增加刻度的数量
默认设置的一个常见问题是当子图表较小时，刻度标签可能会粘在一起。可以从下面例子看到：
```python
fig, ax = plt.subplots(4, 4, sharex=True, sharey=True)
```
![](./img/1600391696271-c02e012c-130e-479e-a5a7-5c4e93181cdd.png)<br />特别是 x 轴，标签的数字就快重叠在一起了，这让这些标签难以认清。可以通过`plt.MaxNLocator()`来修正这点，用它可以设置最大展示刻度的数量。Matplotlib 会自己计算按照这个最大数量计算的刻度位置：
```python
# 对x和y轴设置刻度最大数量
for axi in ax.flat:
    axi.xaxis.set_major_locator(plt.MaxNLocator(3))
    axi.yaxis.set_major_locator(plt.MaxNLocator(3))
fig
```
![](./img/1600391696360-95920306-84fc-4930-8633-d801efa7d102.png)<br />上图就清晰多了。如果希望对于刻度位置进行更加精细的控制，可以使用`plt.MultipleLocator` 。
<a name="mIcrb"></a>
## 复杂的刻度格式
Matplotlib 的默认刻度格式只能在很多常见情况下工作良好，但是在特殊情况下会希望能够更多的进行个性化。考虑下面的正弦和余弦图表：
```python
# 绘制正弦和余弦图表
fig, ax = plt.subplots()
x = np.linspace(0, 3 * np.pi, 1000)
ax.plot(x, np.sin(x), lw=3, label='Sine')
ax.plot(x, np.cos(x), lw=3, label='Cosine')
# 设置网格、图例和轴极限
ax.grid(True)
ax.legend(frameon=False)
ax.axis('equal')
ax.set_xlim(0, 3 * np.pi);
```
![](./img/1600391696395-3716ac91-3699-4567-a5b7-a7ef546930f8.png)<br />这里有几个希望进行的改变。首先，如果刻度的间距和网格线是 π 的倍数会显得更加自然。可以通过`MultipleLocator`来设置它，这个对象用来设置刻度的配置。为了更直观，设置主要刻度为π/2位置，设置次要刻度为 π/4 位置：
```python
ax.xaxis.set_major_locator(plt.MultipleLocator(np.pi / 2))
ax.xaxis.set_minor_locator(plt.MultipleLocator(np.pi / 4))
fig
```
![](./img/1600391696359-ac0d1daa-ce55-4106-94cb-11767e1c5b83.png)<br />但是上图看起来有点傻：可以看出刻度确实是 π 的倍数，但是使用了小数的展示让它们看起来很奇怪。要修正这些标签，需要修改刻度的 formatter。在这种情况中，没有内建的 formatter 可以使用，因此使用`plt.FuncFormatter`，这个对象能够接受一个用户自定义的函数来提供对于刻度标签的精细控制：
```python
def format_func(value, tick_number):
    # N是pi/2的倍数
    N = int(np.round(2 * value / np.pi))
    if N == 0:
        return "0" # 0点
    elif N == 1:
        return r"$\frac{\pi}{2}$" # pi/2
    elif N == 2:
        return r"$\pi$" # pi
    elif N % 2 > 0:
        return r"$\frac{{%d}\pi}{2}$" %N # n*pi/2 n是奇数
    else:
        return r"${0}\pi$".format(N // 2) # n*pi n是整数
ax.xaxis.set_major_formatter(plt.FuncFormatter(format_func))
fig
```
![](./img/1600391696387-a31ff9a3-af4f-4a5a-9b2e-e27800350d5b.png)<br />上图看起来好多了。注意到使用到了 Matplotlib 的 LaTeX 支持，使用美元符号将 LaTeX 字符串括起来。这是用来展示数学符号和公式的简便方法：在这个例子中`"$\pi$"`被渲染成希腊字母 。<br />`plt.FuncFomatter()`提供了对于图表刻度最高级的自定义和精细控制，并且当需要创建需要印刷或出版的图表时非常方便。
<a name="VeU8x"></a>
## Formatter 和 Locator 总结
已经介绍了一些 formatter 和 locator。在最后通过将內建的 locator 和 formatter 参数列出来对本节做一个总结。要获得更多相关内容，请参阅文档或 Matplotlib 的在线文档。下表中列出的对象在`plt`命名空间中都是有效的：

| Locator 对象 | 描述 |
| --- | --- |
| `NullLocator` | 无刻度 |
| `FixedLocator` | 固定刻度位置 |
| `IndexLocator` | 序号图表刻度 (例如 x = range(len(y))) |
| `LinearLocator` | 从最小到最大值的均匀分割刻度 |
| `LogLocator` | 从最小到最大值的对数分割刻度 |
| `MultipleLocator` | 某个基数的倍数刻度 |
| `MaxNLocator` | 刻度数量最大值 |
| `AutoLocator` | 默认的刻度数量最大值 |
| `AutoMinorLocator` | 默认的次要刻度 |

| Formatter 对象 | 描述 |
| --- | --- |
| `NullFormatter` | 无标签 |
| `IndexFormatter` | 从一个列表获得标签 |
| `FixedFormatter` | 从固定的字符串设置标签 |
| `FuncFormatter` | 使用自定义函数设置标签 |
| `FormatStrFormatter` | 使用一个格式化字符串设置标签 |
| `ScalarFormatter` | 默认的标量标签 |
| `LogFormatter` | 默认的对数标签 |

<a name="mA367"></a>
# 11、在 matplotlib 中创建三维图表
Matplotlib 最开始被设计为仅支持二维的图表。到 1.0 版本发布左右，一些三维图表的工具在二维展示的基础上被创建了出来，结果就是 Matplotlib 提供了一个方便的（同时也是有限的）的可用于三维数据可视化的一套工具。三维图表可以使用载入`mplot3d`工具包来激活，这个包会随着 Matplotlib 自动安装：
```python
from mpl_toolkits import mplot3d
```
一旦模块被导入，三维 axes 就可以像其他普通 axes 一样通过关键字参数`projection='3d'`来创建：
```python
%matplotlib inline
import numpy as np
import matplotlib.pyplot as plt
```
```python
fig = plt.figure()
ax = plt.axes(projection='3d')
```
![](./img/1600391696328-5581d08f-79b4-447d-a2a0-4340ca05d22e.png)<br />三维 axes 激活后，可以在上面绘制不同的三维图表类型。三维图表在 notebook 中使用交互式图表展示会优于使用静态展示；回忆前面介绍过，可以使用`%matplotlib notebook`而不是`%matplotlib inline`来激活交互式展示模式。
<a name="WDEEq"></a>
## 三维的点和线
三维图表中最基础的是使用(x, y, z)坐标定义的一根线或散点的集合。前面介绍过普通的二维图表，作为类比，使用`ax.plot3D`和`ax.scatter3D`函数可以创建三维折线和散点图。这两个函数的签名与二维的版本基本一致，可以参考[简单折线图]和[简单散点图]来复习一下这部分的内容。下面绘制一个三维中的三角螺旋，在线的附近在绘制一些随机的点：
```python
ax = plt.axes(projection='3d')
# 三维螺旋线的数据
zline = np.linspace(0, 15, 1000)
xline = np.sin(zline)
yline = np.cos(zline)
ax.plot3D(xline, yline, zline, 'gray')
# 三维散点的数据
zdata = 15 * np.random.random(100)
xdata = np.sin(zdata) + 0.1 * np.random.randn(100)
ydata = np.cos(zdata) + 0.1 * np.random.randn(100)
ax.scatter3D(xdata, ydata, zdata, c=zdata, cmap='Greens');
```
![](./img/1600391696402-3341154c-1ca4-43a4-8f35-f24e10803cd5.png)<br />注意默认情况下，图中的散点会有透明度的区别，用于体现在图中散点的深度。虽然三维效果在静态图像中难以显示，可以使用交互式的视图来获得更佳的三维直观效果。
<a name="4v3ZY"></a>
## 三维轮廓图
类似于在[密度和轮廓图]中介绍的内容，`mplot3d`也包含着能够创建三维浮雕图像的工具。就像二维的`ax.contour`图表，`ax.contour3D`要求输入数据的格式是二维普通网格上计算得到的 Z 轴的数据值。下面展示一个三维的正弦函数轮廓图：
```python
def f(x, y):
    return np.sin(np.sqrt(x ** 2 + y ** 2))
x = np.linspace(-6, 6, 30)
y = np.linspace(-6, 6, 30)
X, Y = np.meshgrid(x, y)
Z = f(X, Y)
```
```python
fig = plt.figure()
ax = plt.axes(projection='3d')
ax.contour3D(X, Y, Z, 50, cmap='binary')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z');
```
![](./img/1600391696411-1b4e0d58-bceb-45df-8fc7-c7294c70c476.png)<br />有时候默认的视角角度不是最理想的，在这种情况下可以使用`view_init`函数来设置水平角和方位角。在下面的例子中，使用的是 60° 的水平角（即以 60° 俯视 x-y 平面）和 35° 的方位角（即将 z 轴逆时针旋转 35°）：
```python
ax.view_init(60, 35)
fig
```
![](./img/1600391696395-7ee5342b-e0c4-4e74-acdb-14effe70dd6a.png)<br />同样，注意到当使用 Matplotlib 交互式展示是，这样的旋转可以通过鼠标点击和拖拽来实现。
<a name="J8pDS"></a>
## 框线图和表面图
使用网格数据生成的三维图表还有框线图和表面图。这两种图表将网格数据投射到特定的三维表面，能够使得结果图像非常直观和具有说服力。下面是一个框线图的例子：
```python
fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_wireframe(X, Y, Z, color='black')
ax.set_title('wireframe');
```
![](./img/1600391696388-c342d098-2f4b-4054-8170-1e6df6b74bb9.png)<br />表面图类似框线图，区别在于每个框线构成的多边形都使用颜色进行了填充。添加色图用于填充多边形能够让图形表面展示出来：
```python
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                cmap='viridis', edgecolor='none')
ax.set_title('surface');
```
![](./img/1600391696372-2d65a9ad-9f25-4f2b-a49b-a49b2e24a02c.png)<br />注意虽然每个颜色填充的表面都是二维的，但是表面的边缘不需要是直线构成的。下面的例子使用`surface3D`绘制了一个部分极坐标网格，能够切入到函数内部观察效果：
```python
r = np.linspace(0, 6, 20)
theta = np.linspace(-0.9 * np.pi, 0.8 * np.pi, 40)
r, theta = np.meshgrid(r, theta)
X = r * np.sin(theta)
Y = r * np.cos(theta)
Z = f(X, Y)
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                cmap='viridis', edgecolor='none');
```
![](./img/1600391696388-11c11422-66ed-44e4-ad0f-972be632f441.png)
<a name="TK0Sy"></a>
## 表面三角剖分
在一些应用场合中，上面的这种均匀网格绘制的图表方式太过于局限和不方便。在这些情况下，三角剖分的图表可以派上用场。如果并不是使用笛卡尔坐标系或极坐标系的网格来绘制三维图表，而是使用一组随机的点来绘制三维图表呢？
```python
theta = 2 * np.pi * np.random.random(1000)
r = 6 * np.random.random(1000)
x = np.ravel(r * np.sin(theta))
y = np.ravel(r * np.cos(theta))
z = f(x, y)
```
有了上面的数据之后，可以使用它们来绘制一张散点图表现出样本所在表面的情况：
```python
ax = plt.axes(projection='3d')
ax.scatter(x, y, z, c=z, cmap='viridis', linewidth=0.5);
```
![](./img/1600391696395-61a272c4-bdee-408d-90b3-f7c3ceb9bcec.png)<br />上图并未形象的表示出表面情况。这种情况下可以使用`ax.plot_trisurf`函数，它能首先根据数据输入找到各点内在的三角函数形式，然后绘制表面（注意的是这里的 x，y，z 是一维的数组）：
```python
ax = plt.axes(projection='3d')
ax.plot_trisurf(x, y, z, cmap='viridis', edgecolor='none');
```
![](./img/1600391696408-209a15b7-bc8c-4fc3-9d9b-0f76e14cfe00.png)<br />上图的结果很显然没有使用网格绘制表面图那么清晰，但是对于并不是使用函数构建数据样本（数据样本通常来自真实世界的采样）的情况下，这能提供很大的帮助。例如下面会看到，能使用这种方法绘制一条三维的莫比乌斯环。
<a name="5gvzq"></a>
### 例子：绘制莫比乌斯环
莫比乌斯环是使用一条纸条，一端翻折后与另一端粘起来形成的环形。在拓扑学中这是非常有趣的一个形状，因为它只有一个面。下面使用 Matplotlib 的三维工具绘制莫比乌斯环。创建莫比乌斯环的关键在于能参数化它：莫比乌斯环是一个二维的环状结构，因此需要两个特定的维度。一个称为θ ，取值范围是0→2π表示整个环状，还有一个称为ω ，取值范围-1→1是表示纸带的宽度：
```python
theta = np.linspace(0, 2 * np.pi, 30)
w = np.linspace(-0.25, 0.25, 8)
w, theta = np.meshgrid(w, theta)
```
有了这两个参数之后，需要确定莫比乌斯环上*(x, y, z)*坐标的位置。<br />仔细思考一下，会发现在莫比乌斯环上有两个自转发生：一个是纸带绕环形中央位置的旋转（称为 θ），另一个纸带绕着中间轴线的旋转（称为 Φ）。纸带中央位置旋转一整圈 2π 时，纸带绕中间轴线旋转刚好半圈 ，将整个旋转均匀分布在纸带上时，就会有ΔΦ=Δθ/2 。
```python
phi = 0.5 * theta
```
现在已经有了所有需要获得三维坐标值的参数了。定义  为每个坐标点距离环形中间的位置，使用它来计算最终三维坐标系的坐标值：
```python
# r是坐标点距离环形中心的距离值
r = 1 + w * np.cos(phi)
# 利用简单的三角函数知识算得x，y，z坐标值
x = np.ravel(r * np.cos(theta))
y = np.ravel(r * np.sin(theta))
z = np.ravel(w * np.sin(phi))
```
最后，为了绘制对象，必须保证三角剖分是正确的。实现这个最好的方法是在底层的参数上面实现三角剖分，最后让 Matplotlib 将这个三角剖分投射到三维空间中形成莫比乌斯环。下面的代码最终绘制图形：
```python
# 在底层参数的基础上进行三角剖分
from matplotlib.tri import Triangulation
tri = Triangulation(np.ravel(w), np.ravel(theta))
ax = plt.axes(projection='3d')
ax.plot_trisurf(x, y, z, triangles=tri.triangles,
                cmap='viridis', linewidths=0.2);
ax.set_xlim(-1, 1); ax.set_ylim(-1, 1); ax.set_zlim(-1, 1);
```
![](./img/1600391696492-e0075e16-1d01-4b81-8ca8-047a60531aed.png)
