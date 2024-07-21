# 原创
：  [Python从零到壹] 六十七.图像识别及经典案例篇之基于卷积神经网络的MNIST图像分类

# [Python从零到壹] 六十七.图像识别及经典案例篇之基于卷积神经网络的MNIST图像分类

<font color="red">**七月太忙，还是写一篇吧！**</font><br/> 欢迎大家来到“Python从零到壹”，在这里我将分享约200篇Python系列文章，带大家一起去学习和玩耍，看看Python这个有趣的世界。所有文章都将结合案例、代码和作者的经验讲解，真心想把自己近十年的编程经验分享给大家，希望对您有所帮助，文章中不足之处也请海涵。Python系列整体框架包括基础语法10篇、网络爬虫30篇、可视化分析10篇、机器学习20篇、大数据分析20篇、图像识别30篇、人工智能40篇、Python安全20篇、其他技巧10篇。您的关注、点赞和转发就是对秀璋最大的支持，知识无价人有情，希望我们都能在人生路上开心快乐、共同成长。

该系列文章主要讲解Python OpenCV图像处理和图像识别知识，前期主要讲解图像处理基础知识、OpenCV基础用法、常用图像绘制方法、图像几何变换等，中期讲解图像处理的各种运算，包括图像点运算、形态学处理、图像锐化、图像增强、图像平滑等，后期研究图像识别、图像分割、图像分类、图像特效处理以及图像处理相关应用。

<mark>**第一部分作者介绍了图像处理基础知识，第二部分介绍了图像运算和图像增强，接下来第三部分我们将详细讲解图像识别及图像处理经典案例，该部分属于高阶图像处理知识，能进一步加深我们的理解和实践能力。图像分类是根据图像信息中所反映的不同特征，把不同类别的目标区分开来的图像处理方法。上一篇文章主要讲解常见的图像分类算法，并介绍了Python环境下的贝叶斯图像分类算法和基于KNN算法的图像分类等案例。这篇文章将利用卷积神经网络实现MNIST（手写数字）图像分类，这也是经典的图像分类案例。希望文章对您有所帮助，如果有不足之处，还请海涵。**</mark>

#### 文章目录

**下载地址：记得点赞喔 O(∩_∩)O**

**前文赏析：（尽管该部分占大量篇幅，但我舍不得删除，哈哈！）**

<mark>第一部分 基础语法</mark>

<mark>第二部分 网络爬虫</mark>

<mark>第三部分 数据分析和机器学习</mark>

<mark>第四部分 Python图像处理基础</mark>

<mark>第五部分 Python图像运算和图像增强</mark>

<mark>第六部分 Python图像识别和图像高阶案例</mark>

第七部分 NLP与文本挖掘

第八部分 人工智能入门知识

第九部分 网络攻防与AI安全

第十部分 知识图谱构建实战

扩展部分 人工智能高级案例

> 
作者新开的“娜璋AI安全之家”将专注于Python和安全技术，主要分享Web渗透、系统安全、人工智能、大数据分析、图像识别、恶意代码检测、CVE复现、威胁情报分析等文章。虽然作者是一名技术小白，但会保证每一篇文章都会很用心地撰写，希望这些基础性文章对你有所帮助，在Python和安全路上与大家一起进步。


---


## 一.图像分类

图像分类（Image Classification）是对图像内容进行分类的问题，它利用计算机对图像进行定量分析，把图像或图像中的区域划分为若干个类别，以代替人的视觉判断。图像分类的传统方法是特征描述及检测，这类传统方法可能对于一些简单的图像分类是有效的，但由于实际情况非常复杂，传统的分类方法不堪重负。现在，广泛使用机器学习和深度学习的方法来处理图像分类问题，其主要任务是给定一堆输入图片，将其指派到一个已知的混合类别中的某个标签。

在图1中，图像分类模型将获取单个图像，并将为4个标签{cat，dog，hat，mug}分配对应的概率{0.6, 0.3, 0.05, 0.05}，其中0.6表示图像标签为猫的概率，其余类比。如图所示，该图像被表示为一个三维数组。在这个例子中，猫的图像宽度为248像素，高度为400像素，并具有红绿蓝三个颜色通道（通常称为RGB）。因此，图像由248×400×3个数字组成或总共297600个数字，每个数字是一个从0（黑色）到255（白色）的整数。图像分类的任务是将这接近30万个数字变成一个单一的标签，如“猫（cat）”。

那么，如何编写一个图像分类的算法呢？又怎么从众多图像中识别出猫呢？这里所采取的方法和教育小孩看图识物类似，给出很多图像数据，让模型不断去学习每个类的特征。在训练之前，首先需要对训练集的图像进行分类标注，如图2所示，包括cat、dog、mug和hat四类。在实际工程中，可能有成千上万类别的物体，每个类别都会有上百万张图像。

图像分类是输入一堆图像的像素值数组，然后给它分配一个分类标签，通过训练学习来建立算法模型，接着使用该模型进行图像分类预测。基于神经网络的图像分类流程如图35-3所示，参考网易云课程“莫烦”老师分享。

如下图所示，通常来说，计算机处理的东西和人类有所不同，无论是声音、图片还是文字，它们都只能以数字0或1出现在计算机神经网络里。神经网络看到的图片其实都是一堆数字，对数字的加工处理最终生成另一堆数字，并且具有一定认知上的意义，通过一点点的处理能够得知计算机到底判断这张图片是猫还是狗。

分类（Classification）属于有监督学习中的一类，它是数据挖掘、机器学习和数据科学中一个重要的研究领域。分类模型类似于人类学习的方式，通过对历史数据或训练集的学习得到一个目标函数，再用该目标函数预测新数据集的未知属性。分类模型主要包括两个步骤：

---


## 二.神经网络

神经网络（Neural Network）是对非线性可分数据的分类方法，通常包括输入层、隐藏层和输出层。其中，与输入直接相连的称为隐藏层（Hidden Layer），与输出直接相连的称为输出层（Output Layer）。神经网络算法的特点是有比较多的局部最优值，可通过多次随机设定初始值并运行梯度下降算法获得最优值。图像分类中使用最广泛的是BP神经网络和CNN神经网络。

BP神经网络是一种多层的前馈神经网络，其主要的特点为：信号是前向传播的，而误差是反向传播的。BP神经网络的过程主要分为两个阶段，第一阶段是信号的前向传播，从输入层经过隐含层，最后到达输出层；第二阶段是误差的反向传播，从输出层到隐含层，最后到输入层，依次调节隐含层到输出层的权重和偏置，输入层到隐含层的权重和偏置，具体结构如图4所示。

神经网络的基本组成单元是神经元。神经元的通用模型如图5所示，其中常用的激活函数有阈值函数、Sigmoid函数和双曲正切函数等。

神经元的输出为：

---


## 三.卷积神经网络

卷积神经网络（Convolutional Neural Networks）是一类包含卷积计算且具有深度结构的前馈神经网络，是深度学习的代表算法之一。卷积神经网络的研究始于二十世纪80至90年代，时间延迟网络和LeNet-5是最早出现的卷积神经网络。在二十一世纪后，随着深度学习理论的提出和数值计算设备的改进，卷积神经网络得到了快速发展，并被大量应用于计算机视觉、自然语言处理等领域。

图6是一个识别的CNN模型。最左边的图片是输入层二维矩阵，然后是卷积层，卷积层的激活函数使用ReLU，即。在卷积层之后是池化层，它和卷积层是CNN特有的，池化层中没有激活函数。卷积层和池化层的组合可以在隐藏层出现很多次，上图中循环出现了两次，而实际上这个次数是根据模型的需要而定。常见的CNN都是若干卷积层加池化层的组合，在若干卷积层和池化层后面是全连接层，最后输出层使用了Softmax激活函数来做图像识别的分类。

神经网络是由很多神经层组成，每一层神经层中存在很多神经元，这些神经元是识别事物的关键，当输入是图片时，其实就是一堆数字。卷积是指不在对每个像素做处理，而是对图片区域进行处理，这种做法加强了图片的连续性，看到的是一个图形而不是一个点，也加深了神经网络对图片的理解。<br/> 下面结合Google推荐视频详细介绍CNN的原理知识。

假设你有一张小猫咪的照片，如下图所示，它可以被表示为一个薄饼，它有宽度（width）和高度（height），并且由于天然存在红绿蓝三色，它还拥有RGB厚度（depth），此时你的输入深度为3。

假设我们现在拿出图片的一小块，运行一个具有K个输出的小神经网络，像图中一样把输出表示为垂直的一小列。

在不改变权重的情况下，通过小神经网络滑动扫遍整个图片，就像我们拿着刷子刷墙一样水平垂直的滑动。

此时，输出端画出了另一幅图像，如下图中红色区域所示。它与之前的宽度和高度不同，更重要的是它跟之前的深度不同，而不是仅仅只有红绿蓝，现在你得到了K个颜色通道，这种操作称为——<mark>卷积</mark>。

如果你的块大小是整张图片，那它跟普通的神经网络层没有任何区别，正是由于我们使用了小块，我们有很多小块在空间中共享较少的权重。卷积不在对每个像素做处理，而是对图片区域进行处理，这种做法加强了图片的连续性，也加深了神经网络对图片的理解。

一个卷积网络是组成深度网络的基础，我们将使用数层卷积而不是数层的矩阵相乘。如下图所示，让它形成金字塔形状，金字塔底是一个非常大而浅的图片，仅包括红绿蓝，通过卷积操作逐渐挤压空间的维度，同时不断增加深度，使深度信息基本上可以表示出复杂的语义。同时，你可以在金字塔的顶端实现一个分类器，所有空间信息都被压缩成一个标识，只有把图片映射到不同类的信息保留，这就是CNN的总体思想。

上图的具体流程如下：

如果你想实现它，必须还要正确实现很多细节。此时，你已经接触到了块和深度的概念，块（PATCH）有时也叫做核（KERNEL），如下图所示，你堆栈的每个薄饼都被叫做特征图（Feature Map），这里把三个特性映射到K个特征图中，PATCH/KERNEL的功能是从图片中抽离一小部分进行分析，每次抽离的小部分都会变成一个长度、一个宽度、K个厚度的数列。

另一个你需要知道的概念是——步幅（STRIDE）。它是当你移动滤波器或抽离时平移的像素的数量，每一次跨多少步去抽离图片中的像素点。

如果步幅STRIDE等于1，表示每跨1个像素点抽离一次，得到的尺寸基本上和输入相同。

如果步幅STRIDE等于2，表示每次跨2个像素点抽离，意味着变为一半的尺寸。它收集到的信息就会被缩减，图片的长度和宽度被压缩了，压缩合并成更小的一块立方体。

压缩完之后再合并成一个立方体，它就是更小的一块立方体，包含了图片中的所有信息。

抽离图片信息的方式称为PADDING（填充），一般分为两种：

研究发现，卷积过程会丢失一些信息，比如现在想跨2步去抽离原始图片的重要信息，形成长宽更小的图片，该过程中可能会丢失重要的图片信息。为了解决这个问题，通过POOLING（持化）可以避免。其方法是：卷积时不再压缩长宽，尽量保证更多信息，压缩工作交给POOLING。经过图片到卷积，持化处理卷积信息，再卷积再持化，将结果传入两层全连接神经层，最终通过分类器识别猫或狗。

总结：整个CNN从下往上依次经历“图片-&gt;卷积-&gt;持化-&gt;卷积-&gt;持化-&gt;结果传入两层全连接神经层-&gt;分类器”的过程，最终实现一个CNN的分类处理。

写到这里，CNN的基本原理讲解完毕，希望大家对CNN有一个初步的理解。同时建议大家处理神经网络时，先用一般的神经网络去训练它，如果得到的结果非常好，就没必要去使用CNN，因为CNN结构比较复杂。

---


## 四.MNIST数据集

MNIST是手写体识别数据集，它是非常经典的一个神经网络示例。MNIST图片数据集包含了大量的数字手写体图片，如下图所示，我么可以尝试用它进行分类实验。

MNIST数据集是含标注信息的，上图分别表示数字5、0、4和1。该数据集共包含三部分：

通常，训练数据集用来训练模型，验证数据集用来检验所训练出来的模型的正确性和是否过拟合，测试集是不可见的（相当于一个黑盒），但我们最终的目的是使得所训练出来的模型在测试集上的效果（这里是准确性）达到最佳。

如图20所示，数据是以该形式被计算机所读取，比如28*28=784个像素点，白色的地方都是0，黑色的地方表示有数字的，总共有55000张图片。

MNIST数据集中的一个样本数据包含两部分内容：手写体图片和对应的label。这里我们用xs和ys分别代表图片和对应的label，训练数据集和测试数据集都有xs和ys，使用mnist.train.images和mnist.train.labels表示训练数据集中图片数据和对应的label数据。

如图21所示，它表示由28x28的像素点矩阵组成的一张图片，这里的数字784（28x28）如果放在我们的神经网络中，它就是x输入的大小，其对应的矩阵如下图所示，类标label为1。

最终MNIST的训练数据集形成了一个形状为55000*784位的tensor，也就是一个多维数组，第一维表示图片的索引，第二维表示图片中像素的索引（tensor中的像素值在0到1之间）。

这里的y值其实是一个矩阵，这个矩阵有10个位置，如果它是1的话，它在1的位置（第2个数字）上写1，其他地方写0；如果它是2的话，它在2的位置（第3个数字）上写1，其他位置为0。通过这种方式对不同位置的数字进行分类，例如用[0,0,0,1,0,0,0,0,0,0]来表示数字3，如下图所示。

mnist.train.labels是一个55000*10的二维数组，如下图所示。它表示55000个数据点，第一个数据y表示5，第二个数据y表示0，第三个数据y表示4，第四个数据y表示1。

知道MNIST数据集的组成，以及x和y具体的含义，我们就开始编写代码。

---


## 五.基于神经网络的图像分类

本文通过Keras搭建一个分类神经网络，再训练MNIST数据集。其中X表示图片，28x28，y对应的是图像的标签。

<mark>第一步，导入扩展包。</mark>

```
import numpy as np
from keras.datasets import mnist
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.optimizers import RMSprop

```

<mark>第二步，载入MNIST数据及预处理。</mark><br/> 该步骤的核心代码如下：

由于MNIST数据集是Keras或TensorFlow的示例数据，所以我们只需要下面一行代码，即可实现数据集的读取工作。如果数据集不存在它会在线下载，如果数据集已经被下载，它会被直接调用。

```
# 下载MNIST数据 
# X shape(60000, 28*28) y shape(10000, )
(X_train, y_train), (X_test, y_test) = mnist.load_data()

# 数据预处理
X_train = X_train.reshape(X_train.shape[0], -1) / 255  # normalize
X_test = X_test.reshape(X_test.shape[0], -1) / 255     # normalize

# 将类向量转化为类矩阵  数字 5 转换为 0 0 0 0 0 1 0 0 0 0 矩阵
y_train = np_utils.to_categorical(y_train, num_classes=10)
y_test = np_utils.to_categorical(y_test, num_classes=10)

```

<mark>第三步，创建神经网络层。</mark><br/> 前面介绍创建神经网络层的方法是定义之后，利用add()添加神经层。

而这里采用另一种方法，在Sequential()定义的时候通过列表添加神经层。同时需要注意，这里增加了神经网络激励函数并调用RMSprop加速神经网络。

该神经网络层为：

对应代码如下：

```
# Another way to build your neural net
model = Sequential([
        Dense(32, input_dim=784),  # 输入值784(28*28) =&gt; 输出值32
        Activation('relu'),        # 激励函数 转换成非线性数据
        Dense(10),                 # 输出为10个单位的结果
        Activation('softmax')      # 激励函数 调用softmax进行分类
        ])

# Another way to define your optimizer
rmsprop = RMSprop(lr=0.001, rho=0.9, epsilon=1e-08, decay=0.0) #学习率lr

# We add metrics to get more results you want to see
# 激活神经网络
model.compile(
        optimizer = rmsprop,                 # 加速神经网络
        loss = 'categorical_crossentropy',   # 损失函数
        metrics = ['accuracy'],               # 计算误差或准确率
        )

```

<mark>第四步，神经网络训练及预测。</mark>

```
print("Training")
model.fit(X_train, y_train, nb_epoch=2, batch_size=32)    # 训练次数及每批训练大小

print("Testing")
loss, accuracy = model.evaluate(X_test, y_test)

print("loss:", loss)
print("accuracy:", accuracy)

```

最终的完整代码如下：

```
# -*- coding: utf-8 -*-
"""
Created on Fri Feb 14 16:43:21 2020 
@author: Eastmount YXZ
"""
import numpy as np
from keras.datasets import mnist
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.optimizers import RMSprop

#---------------------------载入数据及预处理---------------------------
# 下载MNIST数据 
# X shape(60000, 28*28) y shape(10000, )
(X_train, y_train), (X_test, y_test) = mnist.load_data()

# 数据预处理
X_train = X_train.reshape(X_train.shape[0], -1) / 255  # normalize
X_test = X_test.reshape(X_test.shape[0], -1) / 255     # normalize

# 将类向量转化为类矩阵  数字 5 转换为 0 0 0 0 0 1 0 0 0 0 矩阵
y_train = np_utils.to_categorical(y_train, num_classes=10)
y_test = np_utils.to_categorical(y_test, num_classes=10)

#---------------------------创建神经网络层---------------------------
# Another way to build your neural net
model = Sequential([
        Dense(32, input_dim=784),  # 输入值784(28*28) =&gt; 输出值32
        Activation('relu'),        # 激励函数 转换成非线性数据
        Dense(10),                 # 输出为10个单位的结果
        Activation('softmax')      # 激励函数 调用softmax进行分类
        ])

# Another way to define your optimizer
rmsprop = RMSprop(lr=0.001, rho=0.9, epsilon=1e-08, decay=0.0) #学习率lr

# We add metrics to get more results you want to see
# 激活神经网络
model.compile(
        optimizer = rmsprop,                 # 加速神经网络
        loss = 'categorical_crossentropy',   # 损失函数
        metrics = ['accuracy'],               # 计算误差或准确率
        )

#------------------------------训练及预测------------------------------
print("Training")
model.fit(X_train, y_train, nb_epoch=2, batch_size=32)    # 训练次数及每批训练大小
print("Testing")
loss, accuracy = model.evaluate(X_test, y_test)

print("loss:", loss)
print("accuracy:", accuracy)

```

运行代码，首先会下载MNIT数据集。

```
Using TensorFlow backend.
Downloading data from https://s3.amazonaws.com/img-datasets/mnist.npz
11493376/11490434 [==============================] - 18s 2us/step

```

接着输出两次训练的结果，可以看到误差不断减小、正确率不断增大。最终测试输出的误差loss为“0.185575”，正确率为“0.94690”。

如果读者想更直观地查看我们数字分类的图形，可以定义函数并显示。

此时的完整代码如下所示：

```
# -*- coding: utf-8 -*-
"""
Created on Fri Feb 14 16:43:21 2020 
@author: Eastmount YXZ
"""
import numpy as np
from keras.datasets import mnist
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.optimizers import RMSprop
import matplotlib.pyplot as plt
from PIL import Image

#---------------------------载入数据及预处理---------------------------
# 下载MNIST数据 
# X shape(60000, 28*28) y shape(10000, )
(X_train, y_train), (X_test, y_test) = mnist.load_data()

#------------------------------显示图片------------------------------
def show_mnist(train_image, train_labels):
    n = 6
    m = 6
    fig = plt.figure()
    for i in range(n):
        for j in range(m):
            plt.subplot(n,m,i*n+j+1)
            index = i * n + j #当前图片的标号
            img_array = train_image[index]
            img = Image.fromarray(img_array)
            plt.title(train_labels[index])
            plt.imshow(img, cmap='Greys')
    plt.show()

show_mnist(X_train, y_train)

# 数据预处理
X_train = X_train.reshape(X_train.shape[0], -1) / 255  # normalize
X_test = X_test.reshape(X_test.shape[0], -1) / 255     # normalize

# 将类向量转化为类矩阵  数字 5 转换为 0 0 0 0 0 1 0 0 0 0 矩阵
y_train = np_utils.to_categorical(y_train, num_classes=10)
y_test = np_utils.to_categorical(y_test, num_classes=10)

#---------------------------创建神经网络层---------------------------
# Another way to build your neural net
model = Sequential([
        Dense(32, input_dim=784),  # 输入值784(28*28) =&gt; 输出值32
        Activation('relu'),        # 激励函数 转换成非线性数据
        Dense(10),                 # 输出为10个单位的结果
        Activation('softmax')      # 激励函数 调用softmax进行分类
        ])

# Another way to define your optimizer
rmsprop = RMSprop(lr=0.001, rho=0.9, epsilon=1e-08, decay=0.0) #学习率lr

# We add metrics to get more results you want to see
# 激活神经网络
model.compile(
        optimizer = rmsprop,                 # 加速神经网络
        loss = 'categorical_crossentropy',   # 损失函数
        metrics = ['accuracy'],               # 计算误差或准确率
        )

#------------------------------训练及预测------------------------------
print("Training")
model.fit(X_train, y_train, nb_epoch=2, batch_size=32)    # 训练次数及每批训练大小
print("Testing")
loss, accuracy = model.evaluate(X_test, y_test)

print("loss:", loss)
print("accuracy:", accuracy)

```

---


## 六.总结

写到这里，这篇文章就结束了。本文主要通过Keras实现了一个分类学习的案例，并详细介绍了MNIST手写体识别数据集。最后，希望这篇基础性文章对您有所帮助，如果文章中存在错误或不足之处，还请海涵。

感谢在求学路上的同行者，不负遇见，勿忘初心。图像处理系列主要包括三部分，分别是：

> 
忙碌的七月，忙碌的2023。转眼四年过去，我和她都不容易，两人每次看“致谢”都会泪目，青春变了，唯有情感不变，希望一家人健康快乐。刚到寝室，要战斗了！ <br/><br/> <img alt="在这里插入图片描述" height="560" src="https://img-blog.csdnimg.cn/139d1b6497224e069e1449a76536d36b.png#pic_center" width="460"/>


**参考文献：**
