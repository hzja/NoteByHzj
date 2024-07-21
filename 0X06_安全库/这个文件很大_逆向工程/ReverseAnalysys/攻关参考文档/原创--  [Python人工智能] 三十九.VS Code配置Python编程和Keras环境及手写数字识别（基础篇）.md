# 原创
：  [Python人工智能] 三十九.VS Code配置Python编程和Keras环境及手写数字识别（基础篇）

# [Python人工智能] 三十九.VS Code配置Python编程和Keras环境及手写数字识别（基础篇）

<font color="red">**从本专栏开始，作者正式研究Python深度学习、神经网络及人工智能相关知识。前一篇文章利用Keras构建无监督学习Autoencoder模型并实现聚类分析。这篇文章将介绍基础知识，因为很多读者咨询我如何用VS Code配置Keras深度学习环境，并对比常用的深度学习框架，最后普及手写数字识别案例。基础性文章，希望对您有所帮助！**</font>

#### 文章目录

本专栏主要结合作者之前的博客、AI经验和相关视频及论文介绍，后面随着深入会讲解更多的Python人工智能案例及应用。基础性文章，希望对您有所帮助，如果文章中存在错误或不足之处，还请海涵！作者作为人工智能的菜鸟，希望大家能与我在这一笔一划的博客中成长起来。写了这么多年博客，尝试第一个付费专栏，为小宝赚点奶粉钱，但更多博客尤其基础性文章，还是会继续免费分享，该专栏也会用心撰写，望对得起读者。如果有问题随时私聊我，只望您能从这个系列中学到知识，一起加油喔~

**前文赏析：**

---


## 一.VS Code安装Python

在介绍代码之前，先讲解Python常用的开发工具。建议大家选择一款适合的工具即可，更需要去关注的是Python本身和实际程序应用，而不要被工具所困扰。作者最早学习Python所使用的工具是官网的IDLE，其轻巧便捷，可以更直观地撰写代码，提高代码的把控能力。图1展示了Python官网，在官网中点击“Downloads”页面，选择适合自己电脑的版本。

安装过程比较简单，点击软件并选择合适的路径即可安装，如图2所示。作者选择的版本为Windows系统的“Python 3.7.8”，也是经典版本，同时点击添加至路径（环境变量）。

安装成功后如图3所示，找到Python软件，包括命令行和IDLE两种模式的编辑器。

运行Python集成开发环境（Python Integrated Development Environment，IDLE），输入第一行Python代码“hello world”，输出结果如图4所示，接下来可以利用该工具学习Python。

结合文本挖掘和知识发现的任务的复杂性和易用性（如“代码高亮”或“代码补齐”），为更好地帮助读者专注于文本挖掘的学习，本文推荐三款工具，分别是：

三款工具均可以从对应的官网进行下载，其安装过程比较简单。以VS Code为例，从如图5的官网下载，并且官网有较为详细的使用介绍，推荐读者学习。

安装成功后运行如图6所示。VS Code是一款非常棒的文本编辑器，具有配置简单、功能丰富、轻量简洁的特点，并且极其适合处理中小规模的代码，可以让读者将精力集中于语言本身和文本挖掘任务，本书将使用它来编写Python代码。

首先，选择“Extensions（扩展）”可以安装对应的插件，推荐初学者安装四个常见。

点击“Install”安装即可，安装成功后，点击左下角设置图标重启VS Code编辑器。

在“文件”选项中“新建文件夹”，然后选择对应的存储位置，该文件夹后续Python代码的工程目录，如图8所示。

新建的文件夹如图9所示，右键新建“StartPython”文件夹。

其次，在对应的工程文件夹中新建“hello.py”文件并编写代码，如图10所示。

点击右下角选择对应的编译器，如之前已安装的“Python 3.7.8”。

最后运行代码，输出结果“Hello World”如图12所示。写到这里，Python运行环境配置成功，接下来将介绍Python语言的基础语法和基本操作。

---


## 二.TensorFlow、Keras和PyTorch框架

深度学习（Deep Learning）旨在构建多层神经网络从大量数据中学习其内在规律和表示层次，最终获取所需信息或完成指定任务。深度学习掀起了人工智能的浪潮，三个最著名的框架是TensorFlow、Keras和PyTorch。

### 1.TensorFlow

TensorFlow是当前人工智能主流的开发框架，由谷歌人工智能团队Google Brain开发和维护，其前身是谷歌神经网络算法库DistBelief。TensorFlow拥有TensorFlow Hub、TensorFlow Lite、TensorFlow Research Cloud等项目以及各类应用程序接口（API），被广泛应用于机器学习和深度学习任务中，并且拥有多层级结构，是神经网络实现的重要工具。

TensorFlow使用数据流图进行数值计算，具有良好的扩展性，可部署于各类服务器、PC终端和网页，支持GPU和TPU高性能计算，被广泛应用于学术界的科学研究和产业界的AI产品。Tensor（张量）是TensorFlow框架使用的基本数据结构，类似于多维数组。同时，TensorFlow良好的社区资源和生态系统，可以让研究人员推动机器学习领域的先进技术发展，并让开发者轻松地构建和部署由机器学习提供支持的应用，如图14所示。

---


### 2.Keras

Keras是一个由Python编写的开源人工神经网络库，旨在提供深度神经网络的快速实验。它可以作为Tensorflow、Microsoft-CNTK和Theano的高阶应用程序接口，进行深度学习模型的设计、调试、评估、应用和可视化。

Keras主要开发者是谷歌工程师François Chollet，其在代码结构上由面向对象方法编写，完全模块化并具有可扩展性。Keras简洁高效的代码简化了算法复杂度，能够快速搭建神经网络，具有良好的兼容性，并且适用于初学者学习。

Keras支持人工智能领域的主流算法，包括前馈结构和递归结构的神经网络，也能通过封装参与构建统计学习模型。在硬件和开发环境方面，Keras支持多操作系统的多GPU并行计算，可以根据后台设置转化为TensorFlow、Microsoft-CNTK等系统下的组件。然而，由于Keras是在TensorFlow框架上再次封装的，其运行速度低于TensorFlow。

---


### 3.PyTorch

PyTorch是由Facebook人工智能研究院基于Torch所开发的开源深度学习框架，提供了各种张量操作并通过自动求导可以自动进行梯度计算，方便构建各种动态神经网络模型。PyTorch支持使用GPU和TPU加速计算，拥有两个高级功能：

PyTorch代码很简洁、易于使用、支持计算过程中的动态图且内存使用高效。需要说明，Torch是一个与Numpy类似的张量操作库，与Numpy不同的是Torch对GPU支持的很好，Lua是Torch的上层包装。PyTorch是基于Torch的Python开源机器学习库，可以用于自然语言处理、机器学习、深度学习等任务。

---


## 三.VS Code安装Keras

Keras安装也是调用pip工具，在Anaconda中可以调用conda安装。在安装之前，我们可以通过“pip list”查看已安装的扩展包，如图15所示。

安装命令如下，需要先安装TensorFlow，再安装Keras，并且选择指定的版本。

安装过程如图16和图17所示。

---


## 四.Keras基础知识

Keras扩展包通常包括模型、网络层、网络配置和数据预处理，底层是由TensorFlow和Theano框架支撑，如图18所示。

Keras构建神经网络的流程如图19所示，其核心步骤包括：

Keras的核心数据结构是模型，即一种组织网络层的方式，通过Sequential实现，它是一系列网络层按顺序构成的栈，具体代码如下：

```
from keras.models import Sequential
model = Sequential()

```

Keras添加网络层通过add()函数实现，通过神经层的叠加构建模型，比如下列代码添加全连接层（Dense）和激活函数（Activation），是不是非常简洁和容易理解。用户可以根据自己的实际需求定制模型和网络层，甚至修改源代码。

```
from keras.layers import Dense, Activation
model.add(Dense(units=64, input_dim=100))
model.add(Activation("relu"))
model.add(Dense(units=10))
model.add(Activation("softmax"))

```

神经网络模型构建成功后，需要使用compile()函数来编译模型，且需要指定损失函数和优化器。

```
model.compile(loss='categorical_crossentropy', optimizer='sgd', metrics=['accuracy'])

```

模型编译后，需要分批次训练网络，调用fit()函数实现，也可以手动将一个个batch数据输入至网络中训练，关键代码如下：

```
model.fit(x_train, y_train, epochs=5, batch_size=32)
model.train_on_batch(x_batch, y_batch)

```

接着调用函数评估模型或预测新的数据。

```
loss_and_metrics = model.evaluate(x_test, y_test, batch_size=128)
classes = model.predict(x_test, batch_size=128)

```

---


## 五.Keras实现手写数字识别

最后通过神经网络识别手写数字图像案例加深读者印象。首先介绍Keras库中手写数字数据集mnist的导入方法，并显示四张灰度图像的示例。关键代码如下：

```
# -*- coding: utf-8 -*- 
from keras.datasets import mnist
import matplotlib.pyplot as plt

(X_train, y_train), (X_test, y_test) = mnist.load_data()
print(X_train.shape, y_train.shape)
print(X_test.shape, y_test.shape)
for i in range(4):
   plt.subplot(2,2,i+1)
   plt.imshow(X_train[i], cmap=plt.get_cmap('gray'), interpolation='none')
   plt.title("Class {}".format(y_train[i]))
plt.show()

```

通过mnist.load_data()函数导入数据集，并将结果复制给四个变量，对应训练数据、训练类标、测试数据和测试类标，共十类（类标0至9），输出的形状分别为“(60000, 28, 28) (60000,)”和“(10000, 28, 28) (10000,)”，即6万个训练样本、1万个测试样本。

接着将构建神经网络模型训练手写字体(X_train,y_train)，并在测试集上进行测试。完整代码如下所示：

```
# -*- coding: utf-8 -*- 
from keras.datasets import mnist
import matplotlib.pyplot as plt
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Dense, Activation, Dropout

#导入数据集
(X_train, y_train), (X_test, y_test) = mnist.load_data()
print(X_train.shape, y_train.shape)
print(X_test.shape, y_test.shape)
X_train = X_train.reshape(len(X_train), -1)
X_test = X_test.reshape(len(X_test), -1)
X_train = X_train.astype('float32') / 255
X_test = X_test.astype('float32') / 255

#One-hot编码
y_train = np_utils.to_categorical(y_train)
y_test = np_utils.to_categorical(y_test)

#构建神经网络模型
model = Sequential()
model.add(Dense(512, input_shape=(28*28,), activation='relu'))
model.add(Dropout(0.3))
model.add(Dense(10, activation='softmax')) #十分类

#训练和预测
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
model.fit(X_train, y_train, epochs=10, batch_size=64, verbose=1, validation_split=0.05)
loss, accuracy = model.evaluate(X_test, y_test)
print('Loss:', loss)
print('Accuracy:', accuracy)

```

输出结果如图21所示，通过十个批次的训练，最终误差为“0.06783”，准确率为“0.9805”。该示例更多是给大家展示下Keras构建神经网络的流程，第四章将详细介绍。

---


## 六.总结

为更好地帮助初学者学习系列内容，尤其是零基础朋友，本文主要介绍文本挖掘和知识发现的预备知识，包括Python基础语法、数据分析常见扩展包和深度学习Keras框架，并通过图文结合、代码和原理的普及来讲解相关知识，这将为后续的内容提供帮助。

最后，希望这篇基础性文章对您有所帮助，如果文章中存在错误或不足之处，还请海涵~作为人工智能的菜鸟，我希望自己能不断进步并深入，后续将它应用于图像识别、网络安全、对抗样本等领域，指导大家撰写简单的学术论文，一起加油！

> 
转眼就过年了，2022年简单总结：很多遗憾，很多不足，勉强算是分秒必争，只争朝夕，但愧对家人，陪伴太少，论文、科研、分享和家庭都做得不好，这一年勉强给个65分吧。最最感恩的永远是女神，回家的感觉真好，平平淡淡，温温馨馨，虽然这辈子科研、事业和职称上没有太大的追求，和大佬们的差距如鸿沟，但能做自己喜欢的事，爱自己喜欢的人，每天前进一小步（人生勿比），一家人健康幸福，足矣。提前祝大家新春快乐，阖家幸福。小珞珞是真的逗，陪伴的感觉真好，女神是真的好，爱你们喔，晚安娜继续加油！ <br/><br/> <img alt="在这里插入图片描述" height="500" src="https://img-blog.csdnimg.cn/15efe1ed83884d8c8c542b3feb57b9f4.jpeg#pic_center" width="400"/>


下载地址：

(By:Eastmount 2023-01-25 夜于贵阳 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


**参考文献：**
