<a name="601bfdb8"></a>
## LeNet-5简介

LeNet-5是1998年LeCun发表的用于手写字符识别的经典卷积神经网络模型。当年美国很多银行就采用了这种模型来识别支票上的手写数字，能够达到如此高的商业程度，可见其识别精准和在深度学习发展史上的重要地位。

LeNet-5在论文中被用于识别MNIST数据集提供的0~9共10个手写数字。MNIST是当时Google实验室的Corinna Cortes和纽约大学柯朗研究所的Yann LeCun共同建立的手写数字数据库，包含训练集60000张即测试集10000张手写数字图片。该数据集提供的图片尺寸统一为![](./img/388a0f04894f59cac6e3438ab14527f0.svg)，图中包含字符的最大尺寸为![](./img/7620433c0819f9e5149f48b22d079812.svg)。

231n中对LeNet的评价：

**LeNet**. The first successful applications of Convolutional Networks were developed by Yann LeCun in 1990’s. Of these, the best known is the [LeNet](http://yann.lecun.com/exdb/publis/pdf/lecun-98.pdf) architecture that was used to read zip codes, digits, etc.

<a name="ba210197"></a>
## LeNet-5详解

LeNet-5网络除输入层外包含7层，每层都含有可学习的参数。如下图所示，卷积层标记为![](./img/0ca9b50c4f630610a4851c789b270a9a.svg)，下采样层标记为![](./img/090614c64a0b7bb53db9ff6a62933770.svg)，全连接层标记为![](./img/a850ddbcd70880670c3c5b8e4390d4f8.svg)，其中![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)表示层下标（这个下标是指的在网络中的顺序，不是第几个这样的层）。

![LeNet-5.png](./img/1596531426616-bf44eae3-ea84-4284-82bc-5a00c8289cc1.png)

**输入：![](./img/43563acd97469d7a12d27c9043c982bf.svg)**的手写字体图片，这些手写字体包含0~9数字，也就是相当于10个类别的图片。网络输入是![](./img/43563acd97469d7a12d27c9043c982bf.svg)像素的图片，这一尺寸明显大于MNIST提供的字符尺度（![](./img/388a0f04894f59cac6e3438ab14527f0.svg)）。这样设计的原因是希望如笔画末端或尖端等潜在的有效特征能够出现在最高层特征检测子感受野的中心位置，换句话说，网络高层的各个特征都集成了笔画和尖角边缘信息。在LeNet-5中，最后一个卷积层（![](./img/349411fb9ab0071ae0963d9e3e113a1e.svg)）的所有感受野中心会在输入的![](./img/6364d3f0f495b6ab9dcf8d3b5c6e0b01.svg)像素![](./img/487fe5fa0b65f918964f25ba2d44c85f.svg)像素图片中形成一个![](./img/7620433c0819f9e5149f48b22d079812.svg)的区域。此外，还需对输入像素的取值进行标准化：白色背景取值![](./img/b4132a25aecd27236a53a2b1aabb7a75.svg)，黑色前景取值![](./img/787ef7c2ac72bd7ff5ea5eed1da8b7e3.svg)，这样使得在MNIST数据集上像素点取值均值近似为![](./img/cfcd208495d565ef66e7dff9f98764da.svg)，方差近似为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)。数据标准化能够有效地加速训练过程。

**输出：**分类结果，0~9之间的一个数。因此我们可以知道，这是一个多分类问题，总共有十个类，因此神经网络的最后输出层必然是SoftMax问题，然后神经元的个数是10个。

<a name="44225dfa"></a>
### 输入层

**输入层：![](./img/43563acd97469d7a12d27c9043c982bf.svg)**的图片（因为是灰度图，所以其实是![](./img/78b7ce544c2b6d52026fd6caac563e8a.svg)），也就是相当于1024个神经元。

<a name="c3f1d3bd"></a>
### 第一层(卷积)

![](./img/4fa71d007c094ac3c858919aec515277.svg)**层：**是网络的第一个卷积层，paper这里选择6个卷积核，卷积核大小选择![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)，这样我们可以得到6个特征图，每个特征图大小为![](./img/388a0f04894f59cac6e3438ab14527f0.svg)（![](./img/fa0ce1c9e3b53dd49add625b3ec81fb3.svg)），也就是神经元的个数为![](./img/5caf1ae6962059fd2bb0b5f549291cb9.svg)。所以现在我们的维度从![](./img/78b7ce544c2b6d52026fd6caac563e8a.svg)变成了![](./img/cb824dde43f2877f17cd924b24a7fa57.svg)

![第一层.jpg](./img/1596531569657-b696e634-b2f4-4894-9c13-bec207ff328e.jpeg)

<a name="1d2a4170"></a>
### 第二层(下采样)

![](./img/a3de00c1597600a387128a7add5b354f.svg)**层：**是下采样层，也就数使用最大池化进行下采样，池化的大小选择(2,2)，也就相当于对![](./img/4fa71d007c094ac3c858919aec515277.svg)层![](./img/388a0f04894f59cac6e3438ab14527f0.svg)的图片，进行分块（即邻域），每个块的大小为![](./img/6d92cd358862cf6ada4d640f248f6ac0.svg)，这样我们可以得到![](./img/5c29f9eccdbfe168b03140dbfe0b7c01.svg)个块，我们统计每个块中最大的值作为下采样的新像素，因此我们可以得到![](./img/5c29f9eccdbfe168b03140dbfe0b7c01.svg)大小的图片，共有6个这样的图片。至此我们的维度从![](./img/cb824dde43f2877f17cd924b24a7fa57.svg)变成了![](./img/45f489ff3975dd44df86ae335f330db0.svg)。在最初的论文中，会将邻域里的4个值求和乘系数并加上一个偏置，所得结果再通过Sigmoid激活函数来产生![](./img/a3de00c1597600a387128a7add5b354f.svg)层的特征图。这里系数和偏置是本层需要学习的参数。在后来的实现中，常用最大池化取代上述过程。

![第二层.jpg](./img/1596532019843-d75946e0-45ab-4400-88eb-246a0f947445.jpeg)

<a name="ca74e47f"></a>
### 第三层(卷积-关键)

![](./img/349411fb9ab0071ae0963d9e3e113a1e.svg)**层：**同样是卷积层，这一层卷积核大小仍未![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)，据此我们可以得到新的图片大小为![](./img/ba251f7115d14d1535beda426db20ede.svg)（![](./img/fd8cc0ed51a1e88ac51d68b4456305d7.svg)），然后我们希望可以得到16张特征图，即我们的维度从![](./img/45f489ff3975dd44df86ae335f330db0.svg)变成![](./img/97dfd8f7b249e173fc678da8722a378c.svg)。

![第三层.jpg](./img/1596532086544-a1b41c30-00bd-4eb4-bde4-780a889405de.jpeg)

那么问题来了，这一层是最难理解的，我们知道![](./img/a3de00c1597600a387128a7add5b354f.svg)包含：6张![](./img/5c29f9eccdbfe168b03140dbfe0b7c01.svg)大小的图片，我们希望这一层得到的结果是：16张![](./img/ba251f7115d14d1535beda426db20ede.svg)的图片。这16张图片的每一张，是通过![](./img/a3de00c1597600a387128a7add5b354f.svg)的6张图片进行加权组合得到的，具体怎么组合如下面所示：

![第三层1.png](./img/1596532158719-a6afc0f7-83fe-453a-b347-c0fd8d4f3177.png)

前6个特征图与![](./img/a3de00c1597600a387128a7add5b354f.svg)层相连的3个特征图相连接，后面6个特征图与![](./img/a3de00c1597600a387128a7add5b354f.svg)层相连的4个特征图相连接，后面3个特征图与![](./img/a3de00c1597600a387128a7add5b354f.svg)层部分不相连的4个特征图相连接，最后一个与![](./img/a3de00c1597600a387128a7add5b354f.svg)层的所有特征图相连。卷积核大小仍然为![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)，所以共有 个参数，而不是![](./img/5a1513725f92f04d6202e6b51f45211b.svg)个参数，而不是![](./img/ca44b919627c74bac5f779859b1866f0.svg)个参数。因为图像大小是![](./img/ba251f7115d14d1535beda426db20ede.svg)，所以共有![](./img/e2b654239f344e8bed059443c27f9df6.svg)个连接而不是![](./img/70b9bb361eda4af635c1d540f6ddf5dc.svg)个。每个![](./img/349411fb9ab0071ae0963d9e3e113a1e.svg)层的特征图只与部分![](./img/a3de00c1597600a387128a7add5b354f.svg)层的特征图进行连接，这样做有两个原因：

- 部分不完全的连接能够将连接数控制在一个比较合理的范围内。
- 更重要的是，它强制地打破了网络的对称性，不同的特征图由于输入不同而能够表达出不同的特征。

<a name="200fb8c6"></a>
### 第四层(下采样)

![](./img/982f1877cdcf00febf7dbf67cc7a1229.svg)**层：**也是下采样层，采用同样的下采样方式，得到16个![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)的特征图。我们的维度从![](./img/97dfd8f7b249e173fc678da8722a378c.svg)变成![](./img/2cd61e2f98504ef99141eea28e593ad9.svg)。

![第四层.jpg](./img/1596532299616-a6ded278-071a-4c68-a77a-f18065543f38.jpeg)

<a name="e06409a6"></a>
### 第五层(卷积)

![](./img/81454c854daa5b2cc48d20abb6ad176d.svg)层：这一层是网络最后一个卷积层，具有120个特征图，每个特征图与![](./img/982f1877cdcf00febf7dbf67cc7a1229.svg)层所有的特征图相连接，卷积核大小是![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)，而![](./img/982f1877cdcf00febf7dbf67cc7a1229.svg)层的特征图大小也是![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)，所以![](./img/81454c854daa5b2cc48d20abb6ad176d.svg)层的特征图就变成了![](./img/4d6ba9b460a81712eacde7738ad52dcb.svg)大小的特征图。即维度从![](./img/2cd61e2f98504ef99141eea28e593ad9.svg)变成![](./img/28e31569831c290f4b36c5439d20bd6c.svg)。共有![](./img/4285dd007e8a3ce11a8762390fda02b4.svg)个参数。

![第五层.jpg](./img/1596683904024-d79ddce8-0d8a-440c-9ab0-86df13adfe4b.jpeg)

当特征图尺寸为![](./img/4d6ba9b460a81712eacde7738ad52dcb.svg)时，![](./img/81454c854daa5b2cc48d20abb6ad176d.svg)层等价于一个全连接层。但我们仍以卷积层来命名它，因为当输入图片尺寸增大时，网络结构不发生变化。

<a name="54ab53be"></a>
### 第六层(全连接)

![](./img/a67c78796049ddbb01efcfa3a71483e6.svg)层是与![](./img/81454c854daa5b2cc48d20abb6ad176d.svg)层相连，包含84个单元的全连接层，即有![](./img/b495b5c4fb2ea48bbaa6d16622ef7d61.svg)个参数 。这一层采用了正切函数，即![](./img/4abc8661722eec303da667073ca27357.svg)

![第六层.jpg](./img/1596683940301-ba2f0875-caa3-47af-a7b7-fb67ff1c3ae3.jpeg)

<a name="b8ba1121"></a>
### 输出层

最后，输出层由每个类别对应一个欧几里得RBF(Euclidean Radial Basis Function)单元构成。每个RBF单元的输入均为![](./img/a67c78796049ddbb01efcfa3a71483e6.svg)层的84个输出，输出计算公式如下：

![](./img/d856f5b983163fa365127f8539b4085b.svg)

换言之，每个RBF单元计算的输入向量![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)和参数向量![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)之间的欧式距离，输入向量与参数向量之间的距离越远，RBF单元的输出越大。对于一个特定的RBF单元，其输出可以理解成衡量输入向量和对应类别之间匹配程度的一个惩罚项。从概率的观点看，RBF核的输出也可以理解成在由![](./img/a67c78796049ddbb01efcfa3a71483e6.svg)层定义的高斯分布下一个未经归一化的负对数似然。由此，给定一个输入，损失函数将定义为使得![](./img/a67c78796049ddbb01efcfa3a71483e6.svg)层输出尽量接近期望类别的RBF参数向量。采用这种方式，RBF单元的参数向量需要手动给出，并保持固定（至少在初始的时候），参数向量的每个元素取-1或1。

事实上，在以后的实践中，RBF单元常常被Softmax所取代。

![输出层.jpg](./img/1596684021727-bf0a004e-cd69-43f8-bb8a-29a2316211fa.jpeg)

<a name="0d053b6e"></a>
## Code实现

```python
def LeNet():
    model = Sequential()
    model.add(Conv2D(32,(5,5),strides=(1,1),input_shape=(28,28,1),padding='valid',activation='relu',kernel_initializer='uniform'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(Conv2D(64,(5,5),strides=(1,1),padding='valid',activation='relu',kernel_initializer='uniform'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(Flatten())
    model.add(Dense(100,activation='relu'))
    model.add(Dense(10,activation='softmax'))
    return model
```

<a name="Source"></a>
## Source

[https://engmrk.com/lenet-5-a-classic-cnn-architecture/](https://engmrk.com/lenet-5-a-classic-cnn-architecture/)<br />[https://blog.csdn.net/xuanyuansen/article/details/41800721](https://blog.csdn.net/xuanyuansen/article/details/41800721)<br />[http://cs231n.github.io/convolutional-networks/#case](http://cs231n.github.io/convolutional-networks/#case)<br />[https://www.cnblogs.com/skyfsm/p/8451834.html](https://www.cnblogs.com/skyfsm/p/8451834.html)
