# 原创
：  [论文阅读] (06) 万字详解什么是生成对抗网络GAN？经典论文及案例普及

# [论文阅读] (06) 万字详解什么是生成对抗网络GAN？经典论文及案例普及

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇文章分享了Pvop老师的分享，题目为《高手是怎样学习NLP》。这篇文章将详细介绍生成对抗网络GAN的基础知识，包括什么是GAN、常用算法（CGAN、DCGAN、infoGAN、WGAN）、发展历程、预备知识，并通过Keras搭建最简答的手写数字图片生成案例。本文主要学习小象学院老师的视频，并结合论文介绍，希望对您有所帮助！不服GAN，让我们开始吧~**</font>

> 
注意，本文代码采用GPU+Pycharm实现，如果你的电脑是CPU实现，将相关GPU操作注释即可。这里仅做简单的对比实验，不进行参数优化、实验原因分析及详细的效果提升，后面文章会介绍优化、参数选择、实验评估等。


**前文赏析：**

#### 文章目录

---


## 一.GAN简介

### 1.GAN背景知识

**Ian Goodfellow** 因提出了生成对抗网络（GANs，Generative Adversarial Networks）而闻名， GAN最早由Ian Goodfellow于2014年提出，以其优越的性能，在不到两年时间里，迅速成为一大研究热点。他也被誉为“GANs之父”，甚至被推举为人工智能领域的顶级专家。

实验运行结果如下图所示，生成了对应的图像。

或许，你对这个名字还有些陌生，但如果你对深度学习有过了解，你就会知道他。最畅销的这本《深度学习》作者正是Ian Goodfellow大佬。

在2016年，Ian Goodfellow大佬又通过50多页的论文详细介绍了GAN，这篇文章也推荐大家去学习。

Yann LeCun称GAN为“过去十年机器学习界最有趣的idea”。GAN在github上的火热程度如下图所示，呈指数增涨，出现各种变形。当然，其中也存在很多比较水的文章，推荐大家尽量学习比较经典的模型。

---


### 2.GAN原理解析

<font color="red">**首先，什么是GAN？**</font><br/> GANs（Generativeadversarial networks，对抗式生成网络）可以把这三个单词拆分理解。

> 
正如shunliz大佬总结：<br/> GANs是一类生成模型，从字面意思不难猜到它会涉及两个“对手”，一个称为**Generator**（生成者），一个称为**Discriminator**（判别者）。Goodfellow最初arxiv上挂出的GAN tutorial文章中将它们分别比喻为伪造者（Generator）和警察（Discriminator）。伪造者总想着制造出能够以假乱真的钞票，而警察则试图用更先进的技术甄别真假。两者在博弈过程中不断升级自己的技术。<br/><br/> 从博弈论的角度来看，如果是零和博弈（zero-sum game），两者最终会达到纳什均衡（Nash equilibrium），即存在一组策略(g, d)，如果Generator不选择策略g，那么对于Discriminator来说，总存在一种策略使得Generator输得更惨；同样地，将Generator换成Discriminator也成立。<br/><br/> 如果GANs定义的lossfunction满足零和博弈，并且有足够多的样本，双方都有充足的学习能力情况，在这种情况下，Generator和Discriminator的最优策略即为纳什均衡点，也即：Generator产生的都是“真钞”（材料、工艺技术与真钞一样，只是没有得到授权），Discriminator会把任何一张钞票以1/2的概率判定为真钞。


<font color="red">**那么，GAN究竟能做什么呢？**</font><br/> 如下图所示，这是一张非常有意思的图，最左边是真实的图，我们希望去预测视频后几帧的模样，中间这张图是用MSE做的，最右边的图是生成对抗网络做的。通过细节分析，我们可以看到中间这张图的耳朵和眼睛都是模糊的，而GAN生成的效果明显更好。

接着我们在看一个**超分辨率**的实例。首先给出一张超分辨率的图，最左边的图像是原始高分辨率图像（original），然后要对其进行下采样，得到低分辨率图像，接着采用不同的方法对低分辨率图像进行恢复，具体工作如下：

<mark>我们注意观察图像头部雕饰的细节，发现GAN恢复的轮廓更清晰。该实验显示了使用经过训练的生成模型从多模态分布生成真实样本的优势。</mark>

**在这里，我们也科普下超分辨率——SRCNN。**<br/> 它最早是在论文《Learning a Deep Convolutional Network for Image Super-Resolution》中提出，这篇文章的四位作者分别为董超，Chen Change Loy，何凯明，汤晓欧，也都是妥妥的大神。从CV角度来看，这篇论文是真的厉害。

现假设要解决一个问题：<mark>能不能解决超分辨率，从一个低分辨率的图像恢复成一个高分辨率的图像，那怎么做呢？</mark> 他们通过增加两个卷积层的网络就解决了一个实际问题，并且这篇文章发了一个顶会。

更详细的介绍参考知乎oneTaken大佬的分享。

> 
这是第一篇将端到端的深度学习训练来进行超分的论文，整篇论文的的过程现在看起来还是比较简单的，先将低分辨率图片双三次插值上采样到高分辨率图片，然后再使用两层卷积来进行特征映射，最后使用MSE来作为重建损失函数进行训练。从现在来看很多东西还是比较粗糙的，但这篇论文也成为很多超分论文的baseline。<br/><br/> 整篇论文的创新点有：<br/> (1) 使用了一个卷积神经网络来进行超分，端到端的学习低分辨率与超分辨率之间的映射。<br/> (2) 将提出的神经网络模型与传统的稀疏编码方法之间建立联系，这种联系还指导用来设计神经网络模型。<br/> (3) 实验结果表明深度学习方法可以用于超分中，可以获得较好的质量和较快的速度。<br/><br/> 整个的模型架构非常的简单，先是对于输入图片进行双三次插值采样到高分辨空间，然后使用一层卷积进行特征提取，再用ReLU进行非线性映射，最后使用一个卷积来进行重建，使用MSE来作为重建损失。中间一个插曲是将传统用于超分的稀疏编码算法进行了延伸，可以看作是一种具有不同非线性映射的卷积神经网络模型。


---


### 3.GAN经典案例

<font color="red">**GNN究竟能做什么呢？**</font><br/> 下面来看看一些比较有趣的GAN案例。

首先是一个视频，这篇文章中介绍了Zhu等人开发了交互式（interactive）生成对抗网络（iGAN），用户可以绘制图像的粗略草图，就使用GAN生成相似的真实图像。在这个例子中，用户潦草地画了几条绿线，就把它变成一块草地，用户再花了一条黑色的三角形，就创建了一个山包。

另一个比较经典的案例是左侧输入的皮包简图最终生成接近真实包的图像，或者将卫星照片转换成地图，将阈值车辆图像转换为现实中逼真的图像。

再比如通过GAN去预测视频中下一帧动画会发生什么，比如右下角给了一张火车的静态图片，会生成一段火车跑动的动态视频。

Wu等在NIPS 2016中通过GAN实现了用噪声去生成一张3D椅子模型。

下图是starGAN。左侧输入的是一张人脸，然后GAN会生成对应的喜怒哀乐表情，这篇文章的创新不是说GAN能做这件事，而是提出一个方案，**所有的核心功能都在一起，只训练一个生成器**，即不是生成多对多的生成器，而只训练一个生成器就能实现这些功能。

starGAN转移从RaFD数据集中学到的知识，在CelebA数据集上的多域图像转换结果。第一和第六列显示输入图像，其余列是由starGAN生成的图像。请注意，这些图像是由一个单一的生成器网络生成的，而愤怒、快乐和恐惧等面部表情标签都来自RaFD，而不是CelebA。

---


## 二.GAN预备知识

<font color="red">**为什么要讲预备知识呢？**</font><br/> 通过学习神经网络的基础知识，能进一步加深我们对GAN的理解。当然，看到这篇文章的读者可能很多已经对深度学习有过了解或者是大佬级别，这里也照顾下初学者，普及下GAN相关基础知识。这里推荐初学者去阅读作者该系列文章，介绍了很多基础原理。

### 1.什么是神经网络

首先，深度学习就是模拟人的脑神经（生物神经网络），比如下图左上方①中的神经元，可以认为是神经网络的接收端，它有很多的树突接收信号，对应Neuron的公式如下：

 
      
       
        
        
          z 
         
        
          = 
         
         
         
           a 
          
         
           1 
          
         
         
         
           w 
          
         
           1 
          
         
        
          + 
         
        
          . 
         
        
          . 
         
        
          . 
         
        
          + 
         
         
         
           a 
          
         
           k 
          
         
         
         
           w 
          
         
           k 
          
         
        
          + 
         
        
          . 
         
        
          . 
         
        
          . 
         
        
          + 
         
         
         
           a 
          
         
           K 
          
         
         
         
           w 
          
         
           K 
          
         
        
          + 
         
        
          b 
         
        
       
         z=a_1w_1+...+a_kw_k+...+a_Kw_K+b 
        
       
     z=a1​w1​+...+ak​wk​+...+aK​wK​+b

其中，a表示信号（树突接收），w表示对应的权重，它们会进行加权求和组合且包含一个偏置b。通过激活函数判断能否给下一个神经元传递信号。

有了这个神经元之后，我们需要构建网络，如右下方②所示。经过一层、两层、三层神经网络，我们最后会有一个判断，如右上方③所示，经过Softmax函数判断，决策这幅图像是什么，比如猫或狗。

<font color="red">**其次，深度学习有哪些知识点呢？**</font><br/> 深度学习的网络设计如下图所示：

> 
补充：<br/> **深度学习的可解释性非常差**，很多时候不知道它为什么正确。NLP会议上也经常讨论这个可解释性到底重不重要。个人认为，如果用传统的方法效果能达到80%，而深度学习如果提升非常大，比如10%，个人感觉工业界还是会用的，因为能提升性能并解决问题。除非比如风控任务，美团检测异常刷单情况，此时需要准确的确认是否刷单。


---


### 2.全连接层

隐藏层的输入和输出都有关联，即全连接层的每一个结点都与上一层的所有结点相连，用来把前边提取到的特征综合起来。由于其全相连的特性，一般全连接层的参数也是最多的。

全连接层包括神经元的计算公式、维度（神经元个数）、激活函数、权值初始化方法（w、b）、正则项。

---


### 3.激活函数

激活函数（activation function）会让某一部分神经元先激活，然后把激活的信息传递给后面一层的神经系统中。比如，某些神经元看到猫的图片，它会对猫的眼睛特别感兴趣，那当神经元看到猫的眼睛时，它就被激励了，它的数值就会被提高。

激活函数相当于一个过滤器或激励器，它把特有的信息或特征激活，常见的激活函数包括softplus、sigmoid、relu、softmax、elu、tanh等。

激活函数可以参考作者前面的第三篇文章。

常用的激活函数Sigmoid、tanh、ReLU、Leaky ReLU曲线如下图所示：

---


### 4.反向传播

BP神经网络是非常经典的网络，这里通过知乎EdisonGzq大佬的两张图来解释神经网络的反向传播。对于一个神经元而言，就是计算最后的误差传回来对每个权重的影响，即计算每层反向传递的梯度变化。

对于多个神经元而言，它是两条线的输出反向传递，如下图所示Eo1和Eo2。

---


### 5.优化器选择

存在梯度变化后，会有一个迭代的方案，这种方案会有很多选择。优化器有很多种，但大体分两类：

class tf.train.Optimizer是优化器（optimizers）类的基类。优化器有很多不同的种类，最基本的一种是GradientsDescentOptimizer，它也是机器学习中最重要或最基础的线性优化。七种常见的优化器包括：

下面简单介绍其中四个常用的优化器：（推荐 [优化器总结](https://blog.csdn.net/weixin_40170902/article/details/80092628) ）

各种优化器用的是不同的优化算法（如Mmentum、SGD、Adam等），本质上都是梯度下降算法的拓展。下图通过可视化对各种优化器进行了对比分析，机器学习从目标学习到最优的过程，有不同的学习路径，由于Momentum考虑了上一步的学习（learning_rate），走的路径会很长；GradientDescent的学习时间会非常慢。建议如下：

---


### 6.卷积层

为什么会提出卷积层呢？因为全连接层存在一个**核心痛点**：

**利器一：局部感知野**<br/> 提出了一个卷积核的概念，局部感知信息。

**利器二：参数共享**<br/> 从图像的左上角按照3x3扫描至右下角，获得如右图所示的结果，通过卷积共享减少了参数个数。注意，这里的卷积核是如下：

 
      
       
        
        
          [ 
         
         
          
           
            
            
              1 
             
            
           
           
            
            
              0 
             
            
           
           
            
            
              1 
             
            
           
          
          
           
            
            
              0 
             
            
           
           
            
            
              1 
             
            
           
           
            
            
              0 
             
            
           
          
          
           
            
            
              1 
             
            
           
           
            
            
              0 
             
            
           
           
            
            
              1 
             
            
           
          
         
        
          ] 
         
        
       
         \left[ \begin{matrix} 1 &amp; 0 &amp; 1 \\ 0 &amp; 1 &amp; 0 \\ 1 &amp; 0 &amp; 1 \end{matrix} \right] 
        
       
     ⎣⎡​101​010​101​⎦⎤​

当前扫描的区域为如下，最终计算结果为2。

 
      
       
        
        
          [ 
         
         
          
           
            
            
              0 
             
            
           
           
            
            
              1 
             
            
           
           
            
            
              1 
             
            
           
          
          
           
            
            
              0 
             
            
           
           
            
            
              0 
             
            
           
           
            
            
              1 
             
            
           
          
          
           
            
            
              0 
             
            
           
           
            
            
              0 
             
            
           
           
            
            
              1 
             
            
           
          
         
        
          ] 
         
        
       
         \left[ \begin{matrix} 0 &amp; 1 &amp; 1 \\ 0 &amp; 0 &amp; 1 \\ 0 &amp; 0 &amp; 1 \end{matrix} \right] 
        
       
     ⎣⎡​000​100​111​⎦⎤​

卷积层的核心知识点如下：

下图展示了五层卷积层，每层输出的内容。它从最初简单的图形学习到后续的复杂图形。

---


### 7.池化层

池化层主要解决的问题是：

常用池化层包括：

基本知识点如下图所示：

---


### 8.图像问题基本思路

此时，我们通过介绍的全连接层、卷积层、池化层，就能解决实际的问题。如下图所示：

可以将图像问题基本思路简化为下图的模型。

<font color="red">**至此，预备知识介绍完毕！接下来我们进入GAN网络实战分析。**</font>

---


## 三.GAN网络实战分析

GANs（Generativeadversarial networks）对抗式生成网络

### 1.GAN模型解析

<font color="red">**首先，我们先说说GAN要做什么呢？**</font>

可能大家还比较蒙，下面我们再详细介绍一个思路。

GAN的整体思路是一个生成器，一个判别器，并且GoodFellow论文证明了GAN全局最小点的充分必要条件是：**生成器的概率分布和真实值的概率分布是一致的时候。**

 
      
       
        
        
          G 
         
        
          l 
         
        
          o 
         
        
          b 
         
        
          a 
         
        
          l 
         
        
          O 
         
        
          p 
         
        
          t 
         
        
          i 
         
        
          m 
         
        
          a 
         
        
          l 
         
        
          i 
         
        
          t 
         
        
          y 
         
        
          o 
         
        
          f 
         
         
         
           p 
          
         
           g 
          
         
        
          = 
         
         
         
           p 
          
          
          
            d 
           
          
            a 
           
          
            t 
           
          
            a 
           
          
         
        
       
         Global Optimality of p_g=p_{data} 
        
       
     GlobalOptimalityofpg​=pdata​

<font color="red">**其次，GAN还需要分析哪些问题呢？**</font>

---


#### (1) 目标函数

该目标函数如下所示，其中：

这也是一个博弈的过程。

整个公式的具体含义如下：

接着我们回到大神的原论文，看看其算法（**Algorithm 1**）流程。

---


#### (2) GAN图片生成

接着我们介绍训练方案，通过GAN生成图片。

GAN的核心就是这些，再简单总结下，即：

其伪代码如下：

```
for 迭代 in range(迭代总数):
    for batch in range(batch_size):
        新batch = input1的batch + input2的batch (batch加倍)
        for 轮数 in range(判别器中轮数):
           步骤一 训练D
        步骤二 训练G

```

---


### 2.生成手写数字demo分析

接下来我们通过手写数字图像生成代码来加深读者的印象。这是一个比较经典的共有数据集，包括图像分类各种案例较多，这里我们主要是生成手写数字图像。

**首先，我们看看生成器是如何生成一个图像（从噪音生成）？**<br/> 核心代码如下，它首先要随机生成一个噪音（noise），所有生成的图片都是靠噪音实现的。Keras参考代码：

**(1) 生成器G**<br/> 生成器总共包括：

**(2) 判别器D**<br/> 判别器就是做一个二分类的问题，要么真要么假。

**(3) 辅助函数**<br/> 如何把D固定去调整G的函数generator_containing_discriminator。

combine_images函数实现合并图像的操作。

**(4) GAN图片生成训练**<br/> GAN核心流程包括：

**(5) 生成**<br/> 模型训练好之后，我们想办法用GAN生成图片。

**完整代码如下：**<br/> 这段代码更像一个简单的GAN生成图片。

```
# -*- coding: utf-8 -*-
"""
Created on 2021-03-19
@author: xiuzhang Eastmount CSDN
参考：https://github.com/jacobgil/keras-dcgan
"""
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Reshape
from keras.layers.core import Activation
from keras.layers.normalization import BatchNormalization
from keras.layers.convolutional import UpSampling2D
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.core import Flatten
from keras.optimizers import SGD
from keras.datasets import mnist
import tensorflow as tf
import numpy as np
from PIL import Image
import argparse
import math
import os

## GPU处理 读者如果是CPU注释该部分代码即可
## 指定每个GPU进程中使用显存的上限 0.9表示可以使用GPU 90%的资源进行训练
os.environ["CUDA_DEVICES_ORDER"] = "PCI_BUS_IS"
os.environ["CUDA_VISIBLE_DEVICES"] = "0"
gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.8)
sess = tf.Session(config=tf.ConfigProto(gpu_options=gpu_options))

#----------------------------------------------------------------
#生成器
def generator_model():
    model = Sequential()
    model.add(Dense(input_dim=100, output_dim=1024))
    model.add(Activation('tanh'))
    model.add(Dense(128*7*7))        #7x7 128通道
    model.add(BatchNormalization())
    model.add(Activation('tanh'))
    model.add(Reshape((7, 7, 128), input_shape=(128*7*7,)))
    model.add(UpSampling2D(size=(2, 2)))
    model.add(Conv2D(64, (5, 5), padding='same'))
    model.add(Activation('tanh'))
    model.add(UpSampling2D(size=(2, 2)))
    model.add(Conv2D(1, (5, 5), padding='same'))
    model.add(Activation('tanh'))
    return model

#----------------------------------------------------------------
#判别器
def discriminator_model():
    model = Sequential()
    model.add(
            Conv2D(64, (5, 5),
            padding='same',
            input_shape=(28, 28, 1))
            )
    model.add(Activation('tanh'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(128, (5, 5)))
    model.add(Activation('tanh'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Flatten())
    model.add(Dense(1024))
    model.add(Activation('tanh'))
    model.add(Dense(1))
    model.add(Activation('sigmoid'))
    return model

#----------------------------------------------------------------
#辅助函数 固定D调整G
def generator_containing_discriminator(g, d):
    model = Sequential()
    model.add(g)
    d.trainable = False
    model.add(d)
    return model

#辅助函数 合并图像
def combine_images(generated_images):
    num = generated_images.shape[0]
    width = int(math.sqrt(num))
    height = int(math.ceil(float(num)/width))
    shape = generated_images.shape[1:3]
    image = np.zeros((height*shape[0], width*shape[1]),
                     dtype=generated_images.dtype)
    for index, img in enumerate(generated_images):
        i = int(index/width)
        j = index % width
        image[i*shape[0]:(i+1)*shape[0], j*shape[1]:(j+1)*shape[1]] = \
            img[:, :, 0]
    return image

#----------------------------------------------------------------
#训练
def train(BATCH_SIZE):
    (X_train, y_train), (X_test, y_test) = mnist.load_data()
    X_train = (X_train.astype(np.float32) - 127.5)/127.5
    X_train = X_train[:, :, :, None]
    X_test = X_test[:, :, :, None]
    #X_train = X_train.reshape((X_train.shape, 1) + X_train.shape[1:])
    d = discriminator_model()
    g = generator_model()
    d_on_g = generator_containing_discriminator(g, d)
    d_optim = SGD(lr=0.0005, momentum=0.9, nesterov=True)
    g_optim = SGD(lr=0.0005, momentum=0.9, nesterov=True)
    g.compile(loss='binary_crossentropy', optimizer="SGD")
    d_on_g.compile(loss='binary_crossentropy', optimizer=g_optim)
    d.trainable = True
    d.compile(loss='binary_crossentropy', optimizer=d_optim)
    for epoch in range(100):
        print("Epoch is", epoch)
        print("Number of batches", int(X_train.shape[0]/BATCH_SIZE))
        for index in range(int(X_train.shape[0]/BATCH_SIZE)):
            noise = np.random.uniform(-1, 1, size=(BATCH_SIZE, 100))
            image_batch = X_train[index*BATCH_SIZE:(index+1)*BATCH_SIZE]
            generated_images = g.predict(noise, verbose=0)
            if index % 20 == 0:
                image = combine_images(generated_images)
                image = image*127.5+127.5
                Image.fromarray(image.astype(np.uint8)).save(
                    str(epoch)+"_"+str(index)+".png")
            X = np.concatenate((image_batch, generated_images))
            y = [1] * BATCH_SIZE + [0] * BATCH_SIZE
            d_loss = d.train_on_batch(X, y)
            print("batch %d d_loss : %f" % (index, d_loss))
            noise = np.random.uniform(-1, 1, (BATCH_SIZE, 100))
            d.trainable = False
            g_loss = d_on_g.train_on_batch(noise, [1] * BATCH_SIZE)
            d.trainable = True
            print("batch %d g_loss : %f" % (index, g_loss))
            if index % 10 == 9:
                g.save_weights('generator', True)
                d.save_weights('discriminator', True)

#----------------------------------------------------------------
#GAN图片生成
def generate(BATCH_SIZE, nice=False):
    g = generator_model()
    g.compile(loss='binary_crossentropy', optimizer="SGD")
    g.load_weights('generator')
    if nice:
        d = discriminator_model()
        d.compile(loss='binary_crossentropy', optimizer="SGD")
        d.load_weights('discriminator')
        noise = np.random.uniform(-1, 1, (BATCH_SIZE*20, 100))
        generated_images = g.predict(noise, verbose=1)
        d_pret = d.predict(generated_images, verbose=1)
        index = np.arange(0, BATCH_SIZE*20)
        index.resize((BATCH_SIZE*20, 1))
        pre_with_index = list(np.append(d_pret, index, axis=1))
        pre_with_index.sort(key=lambda x: x[0], reverse=True)
        nice_images = np.zeros((BATCH_SIZE,) + generated_images.shape[1:3], dtype=np.float32)
        nice_images = nice_images[:, :, :, None]
        for i in range(BATCH_SIZE):
            idx = int(pre_with_index[i][1])
            nice_images[i, :, :, 0] = generated_images[idx, :, :, 0]
        image = combine_images(nice_images)
    else:
        noise = np.random.uniform(-1, 1, (BATCH_SIZE, 100))
        generated_images = g.predict(noise, verbose=1)
        image = combine_images(generated_images)
    image = image*127.5+127.5
    Image.fromarray(image.astype(np.uint8)).save(
        "generated_image.png")

#参数设置
def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--mode", type=str)
    parser.add_argument("--batch_size", type=int, default=128)
    parser.add_argument("--nice", dest="nice", action="store_true")
    parser.set_defaults(nice=False)
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    """
    args = get_args()
    if args.mode == "train":
        train(BATCH_SIZE=args.batch_size)
    elif args.mode == "generate":
        generate(BATCH_SIZE=args.batch_size, nice=args.nice)
    """
    mode = "train"
    if mode == "train":
        train(BATCH_SIZE=128)
    elif mode == "generate":
        generate(BATCH_SIZE=128)

```

代码执行参数：

```
Training:
python dcgan.py --mode train --batch_size &lt;batch_size&gt;
python dcgan.py --mode train --path ~/images --batch_size 128

Image generation:
python dcgan.py --mode generate --batch_size &lt;batch_size&gt;
python dcgan.py --mode generate --batch_size &lt;batch_size&gt; --nice : top 5% images according to discriminator
python dcgan.py --mode generate --batch_size 128

```

训练过程，首先手写数字MNIST图片数据集可以下载存储至该位置，也可以运行代码在线下载。

```
Epoch is 0
Number of batches 468
batch 0 d_loss : 0.648902
batch 0 g_loss : 0.672132
batch 1 d_loss : 0.649307
....
batch 466 g_loss : 1.305099
batch 467 d_loss : 0.375284
batch 467 g_loss : 1.298173

Epoch is 1
Number of batches 468
batch 0 d_loss : 0.461435
batch 0 g_loss : 1.231795
batch 1 d_loss : 0.412679
....

```

运行过程中会生成很多图像，随着训练次数增加图像会越来越清晰。

然后参数设置为“generate”，利用GAN最终生成图像，如下图所示。

---


### 3.其他常见GAN网络

#### (1) CGAN

**首先，GAN如何输出指定类的图像呢？**<br/> CGAN出场。这里简单介绍下GAN和CGAN的区别：GAN只能判断生成的东西是真的或假的，如果想指定生成图像如1、2、3呢？GAN会先生成100张图像，然后从中去挑选出1、2、3，这确实不方便。

在2014年提出GAN时，CGAN也被提出来了。CGAN除了生成以外，还要把条件带出去，即带着我们要生成一个什么样的图条件去混淆，如下右图：<mark>**噪声z向量+条件c向量去生成**</mark>。

所以整套流程大体不变，接着我们看看公式，它在D(x|y)和G(z|y)中增加了y。其中，y不一定是指定类的输出，可以是一些条件。

---


#### (2) DCGAN

**DCGAN（Deep Convolutional Generative Adversarial Networks）**<br/> 卷积神经网络和对抗神经网络结合起来的一篇经典论文，核心要素是：<mark>**在不改变GAN原理的情况下提出一些有助于增强稳定性的tricks**</mark>。注意，这一点很重要。因为GAN训练时并没有想象的稳定，生成器最后经常产生无意义的输出或奔溃，但是DCGAN按照tricks能生成较好的图像。

DCGAN论文使用的tricks包括：

---


#### (3) ACGAN

<mark>**ACGAN（既能生成图像又能进行分类）**</mark><br/> Conditional Image Synthesis with Auxiliary Classifier GANs，该判别器不仅要判断是真（real）或假（fake），还要判断其属于哪一类。

---


#### (4) infoGAN

InfoGAN：Interpretable Representation Learning by Information Maximizing Generative Adversarial Networks。这个号称是OpenAI在2016年的五大突破之一。

原文地址：[https://arxiv.org/abs/1606.03657](https://arxiv.org/abs/1606.03657)

其理论如下：

整个网络的训练在原目标函数的基础上，增加互信息下界L(G,Q)，因此InfoGAN的目标函数最终表示为：

实验结果如下图所示：

---


#### (5) LAPGAN

下面介绍一个比较有趣的网络拉普拉斯GAN。我们的目标是如何通过噪音生成一张图片，噪声本身生成图片比较困难，不可控量太多，所以我们逐层生成（生成从右往左看）。

那么，如何训练呢？对原来这个大图的鸟进行压缩，再生成一张图去判别，依次逐层训练即可。

---


#### (6) EBGAN

再来看一个EBGAN（Energy-based GAN），它抛弃了之前说的对和错的概念。它增加了一个叫**能量**的东西，经过自动编码器Enc（中间提取特征）和Dec解码器（输出），它希望生成一个跟真实图片的能量尽可能小，跟假的图片能量更大。

其生成器和判别器的损失函数计算公式如下（分段函数）：

下图展示了GAN、EBGAN、EBGAN-PT模型生成的图像。

---


### 4.GAN改进策略

**你以为解决了所有问题了吗？too young.**<br/> 如下图所示误差，我们无法判断GAN训练的好坏。

**GAN需要重视：稳定（训练不奔）、多样性（各种样本）、清晰度（质量好），现在很多工作也是解决这三个问题。**

为什么GAN存在这些问题，这是因为GAN原论文将GAN目标转换成了KL散度的问题，KL散度就是存在这些坑。

最终导致偏向于生成“稳妥”的样本，如下图所示，目标target是均匀分布的，但最终生成偏稳妥的样本。

**那么，有没有解决方法呢？**<br/> WGAN（Wasserstein GAN）在2017年被提出，也算是GAN中里程碑式的论文，它从原理上解决了GAN的问题。具体思路为：

后续接着改进，提出了WGAN-GP（WGAN with gradient penalty），不截断，只对梯度增加惩罚项生成质量更高的图像。它一度被称为“state of the art”。

接下来，做GAN的就会出来反驳“谁说GAN就不如WGAN，我们加上Gradient Penalty，大家效果都差不多”。

效果如下图所示：

**《Google Brain: Are GANs Created Equal? A Large-Scale Study》** 这篇论文详细对比了各GAN模型点心LOSS优化变种。

这篇文章比较的结论为：特定的数据集说特定的事情，没有哪一种碾压其他。好的算法还得看成本，时间短的效果某家强，但训练时间长了，反倒会变差。根据评价标准的不同，场景的不同，效果差的算法也可以逆袭。工业界更看重稳定性，比如WGAN。

> 
参考知乎苏剑林老师的回答<br/> 首先，从理论完备的角度来看，原始的GAN（SGAN）就是一个完整的GAN框架，只不过它可能存在梯度消失的风险。而论文比较的是 <mark>“大家都能稳定训练到收敛的情况下，谁的效果更好”</mark> 的问题，这答案是显然易见的：不管是SGAN还是WGAN，大家都是理论完备的，只是从不同角度看待概率分布的问题而已，所以效果差不多是正常的。<br/><br/> 甚至可以说，SGAN的理论更完备一些（因为WGAN需要L约束，而目前L约束的各种加法都有各自的缺点），所以通常来说SGAN的效果还比WGAN效果好一些。那么WGAN它们的贡献是什么呢？WGAN的特点就是基本上都能 <mark>“稳定训练到收敛”</mark>，而SGAN相对而言崩溃的概率更大。所以，如果在“大家都能稳定训练到收敛”的前提下比较效果，那对于WGAN这些模型本来就很不公平的，因为它们都是致力于怎么才能“稳定训练到收敛”，而这篇论文直接将它作为大前提，直接抹杀了WGAN所作的贡献了。


---


## 四.总结

写到这里，这篇文章就介绍结束了，希望对您有所帮助。首先非常感谢小象学院美图老师的介绍，文章虽然很冗余，但还是能学到知识，尤其是想学GAN的同学，这算一个非常不错的普及。当然，后续随着作者深入，会分享更简洁的介绍和案例，继续加油~

个人感觉GAN有一部分很大的应用是在做强化学习，同时在推荐领域、对抗样本、安全领域均有应用，希望随着作者深入能分享更多的实战性GAN论文。比如如果图片被修改，GAN能不能第一次时间反馈出来或优化判决器。最后给出各类GAN模型对比图。

希望您喜欢这篇文章，从看视频到撰写代码，我真的写了一周时间，再次感谢参考文献的老师们。真心希望这篇文章对您有所帮助，加油~

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享CCF、SCI、南核北核论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:Eastmount 2021-03-30 周二夜于武汉 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


**参考文献：**
