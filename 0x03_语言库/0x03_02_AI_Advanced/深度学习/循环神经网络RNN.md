神经网络 RNN<br />神经网络是深度学习的载体，而神经网络模型中，最经典非RNN模型所属，尽管它不完美，但它具有学习历史信息的能力。后面不管是Encoder-Decoder 框架，还是注意力模型，以及自注意力模型，以及更加强大的Bert模型家族，都是站在RNN的肩上，不断演化、变强的。<br />这篇文章，阐述了RNN的方方面面，包括模型结构，优缺点，RNN模型的几种应用，RNN常使用的激活函数，RNN的缺陷，以及GRU，LSTM是如何试图解决这些问题，RNN变体等。
<a name="J20FJ"></a>
## 概述
传统RNN的体系结构。Recurrent neural networks，也称为RNNs，是一类允许先前的输出用作输入，同时具有隐藏状态的神经网络。它们通常如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443812270-e484e4e1-9b1b-4fcb-a265-f854a902caee.webp#clientId=u68d214c5-6527-4&from=paste&id=u8f3d16a6&originHeight=291&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua9889f2e-8d9e-45c8-90fd-b96a233188f&title=)<br />对于每一时步![](https://cdn.nlark.com/yuque/__latex/e358efa489f58062f10dd7316b65649e.svg#card=math&code=t&id=JNXIO)，激活函数![](https://cdn.nlark.com/yuque/__latex/f12cb4a3b7201454edce380882db8c26.svg#card=math&code=a%5E%7B%3Ct%3E%7D&id=N7XGK)，输出![](https://cdn.nlark.com/yuque/__latex/204c79e6610ba606e86881efd1b18996.svg#card=math&code=y%5E%7B%3Ct%3E%7D&id=ZtQi3)被表达为:<br />这里  是时间维度网络的共享权重系数<br />![](https://cdn.nlark.com/yuque/__latex/d43cf9d6f8d2b2dfb5cff72ba7a6535d.svg#card=math&code=g1%2Cg2&id=aJUF4)是激活函数<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443812180-8e51d404-fbff-4d7d-ad1e-05f2f4c60247.webp#clientId=u68d214c5-6527-4&from=paste&id=ufa6995c5&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub25d5dbf-caa5-4405-b5a8-6d42250e6dc&title=)<br />下表总结了典型RNN架构的优缺点：

| 优点 | 缺点 |
| --- | --- |
| 处理任意长度的输入 | 计算速度慢 |
| 模型形状不随输入长度增加 | 难以获取很久以前的信息 |
| 计算考虑了历史信息 | 无法考虑当前状态的任何未来输入 |
| 权重随时间共享 | <br /> |

<a name="Rxncc"></a>
## RNNs应用
RNN模型主要应用于自然语言处理和语音识别领域。下表总结了不同的应用：

| RNN 类型 | 图解 | 例子 |
| --- | --- | --- |
| 1对1![](https://cdn.nlark.com/yuque/__latex/e9c62ea1cb9e8940897f854a98ac848a.svg#card=math&code=T_x%3DT_y%3D1&id=kWUsH) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443812635-6dfe7406-99be-4243-b67c-ad8be59727b2.webp#clientId=u68d214c5-6527-4&from=paste&id=ufcfede82&originHeight=724&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud90b6416-e1e9-4440-b962-6c09a430d61&title=) | 传统神经网络 |
| 1对多![](https://cdn.nlark.com/yuque/__latex/38ab5c451046f9847a139159b935a0af.svg#card=math&code=T_x%3D1%2CT_y%3E1&id=UysBW) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443812698-f3f97729-c20c-415e-89ec-bf964010b923.webp#clientId=u68d214c5-6527-4&from=paste&id=u81a2fb0f&originHeight=580&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucdab82c4-6df9-4f5f-997f-35dadeb8eb8&title=) | 音乐生成 |
| 多对1![](https://cdn.nlark.com/yuque/__latex/19e7fab2513b7351402bc875480868d3.svg#card=math&code=T_x%3E1%2CT_y%3D1&id=vC7St) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443812755-3926ce4d-9202-4136-8076-8b6a8a8c8c38.webp#clientId=u68d214c5-6527-4&from=paste&id=ua0f8d318&originHeight=563&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud07e68a7-967d-4b28-ac81-18f557e0ab7&title=) | 情感分类 |
| 多对多![](https://cdn.nlark.com/yuque/__latex/5e94734658a546765cb2bcb3c5c16294.svg#card=math&code=T_x%3DT_y&id=RfNWZ) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443812687-8df7dfa7-cad8-4ea2-952b-e8f5b958bf28.webp#clientId=u68d214c5-6527-4&from=paste&id=ucc3642e1&originHeight=587&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua05d1396-83a1-4311-aaa5-4d07be3b5c0&title=) | 命名实体识别 |
| 多对多![](https://cdn.nlark.com/yuque/__latex/5ac0522e209471a5f838d6ade2d04645.svg#card=math&code=T_x%5Cne%20T_y&id=Q8G7Z) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813211-fb2e9be1-234f-470b-9dc8-93ecdffcdbee.webp#clientId=u68d214c5-6527-4&from=paste&id=u27cfd36e&originHeight=476&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3bb8801d-9e31-4fc9-9fa2-2e4e4cd84d3&title=) | 机器翻译 |

<a name="wYQtj"></a>
### 损失函数
对于RNN网络，所有时间步的损失函数![](https://cdn.nlark.com/yuque/__latex/5dcbf640cdb155217ca0c533e1078ff7.svg#card=math&code=%5Cmathcal%7BL%7D&id=XYu84)是根据每个时间步的损失定义的，如下所示：
<a name="kox6Z"></a>
### 时间反向传播
在每个时间点进行反向传播。在时间步![](https://cdn.nlark.com/yuque/__latex/b9ece18c950afbfa6b0fdbfa4ff731d3.svg#card=math&code=T&id=usmx1)，损失![](https://cdn.nlark.com/yuque/__latex/5dcbf640cdb155217ca0c533e1078ff7.svg#card=math&code=%5Cmathcal%7BL%7D&id=S4puc)相对于权重矩阵![](https://cdn.nlark.com/yuque/__latex/61e9c06ea9a85a5088a499df6458d276.svg#card=math&code=W%0A&id=OSRxx)的偏导数表示如下：
<a name="Ywzjb"></a>
## 处理长短依赖
<a name="rG8ZN"></a>
### 常用激活函数
RNN模块中最常用的激活函数描述如下：

| Sigmoid | Tanh | RELU |
| --- | --- | --- |
| 

 | 

 | 

 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813033-da5b97b3-c8c9-4fa4-bd33-4555d1e2a848.webp#clientId=u68d214c5-6527-4&from=paste&id=uf9c2ddee&originHeight=966&originWidth=996&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub5601703-3f4a-4dcd-8fc0-0ea605ca4d3&title=) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813422-4ca6f40c-179c-497f-8e30-feade1c2357d.webp#clientId=u68d214c5-6527-4&from=paste&id=ufecd5313&originHeight=966&originWidth=988&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub2faea12-39ab-439e-86b6-049f3381651&title=) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813436-10089c51-6e13-4aba-b6ec-77fd98e17a9f.webp#clientId=u68d214c5-6527-4&from=paste&id=u79e145a4&originHeight=956&originWidth=982&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u31fa7994-1b00-4a52-af27-ae3a607a8d9&title=) |

<a name="lQSqU"></a>
### 梯度消失/爆炸
在RNN中经常遇到梯度消失和爆炸现象。之所以会发生这种情况，是因为很难捕捉到长期的依赖关系，因为乘法梯度可以随着层的数量呈指数递减/递增。
<a name="eG3x6"></a>
### 梯度修剪
梯度修剪是一种技术，用于执行反向传播时，有时遇到的梯度爆炸问题。通过限制梯度的最大值，这种现象在实践中得以控制。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813512-95ae0ae5-d9f1-47b1-b4ed-11c7c147c3a3.webp#clientId=u68d214c5-6527-4&from=paste&id=u0a511ba9&originHeight=466&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uce90382c-2a65-4cf5-8e00-3492ca98713&title=)
<a name="jzCzA"></a>
### 门的类型
为了解决消失梯度问题，在某些类型的RNN中使用特定的门，并且通常有明确的目的。它们通常标注为![](https://cdn.nlark.com/yuque/__latex/07710b5c43702a8bb7b9104eacc6ba71.svg#card=math&code=%5CGamma&id=Lrnxw)，等于：<br />![](https://cdn.nlark.com/yuque/__latex/7a120d57000dc67dcd18cf38d8ecbe2d.svg#card=math&code=%5CGamma%3D%5Csigma%5Cleft%28W%20x%5E%7B%3Ct%3E%7D%2BU%20a%5E%7B%3Ct-1%3E%7D%2Bb%5Cright%29&id=rNfmq)<br />其中，![](https://cdn.nlark.com/yuque/__latex/abbc2f464e886fd7ac047843aa7c83f8.svg#card=math&code=W%2CU%2Cb&id=DP3VH)是特定于门的系数，![](https://cdn.nlark.com/yuque/__latex/a2ab7d71a0f07f388ff823293c147d21.svg#card=math&code=%5Csigma&id=UgjFl)是sigmoid函数。主要内容总结如下表：

| 门的种类 | 作用 | 应用 |
| --- | --- | --- |
| 更新门  ![](https://cdn.nlark.com/yuque/__latex/159aec9bd0b30b8fa9cc3db72fea629c.svg#card=math&code=%5CGamma_u&id=pGtG5) | 过去对现在有多重要？ | GRU, LSTM |
| 关联门  ![](https://cdn.nlark.com/yuque/__latex/8923fdc376f23341a67e3e178da95693.svg#card=math&code=%5CGamma_r&id=a0IOd) | 丢弃过去信息? | GRU, LSTM |
| 遗忘门  ![](https://cdn.nlark.com/yuque/__latex/ec078fe5bbfad9d08d4f188a49668c0d.svg#card=math&code=%5CGamma_f&id=AsDjK) | 是不是擦除一个单元? | LSTM |
| 输出门  ![](https://cdn.nlark.com/yuque/__latex/8996584981f888698b87d0e765b95852.svg#card=math&code=%5CGamma_0&id=beqd6) | 暴露一个门的多少? | LSTM |

<a name="JpWnZ"></a>
### GRU/LSTM
Gated Recurrent Unit（GRU）和长-短期记忆单元（LSTM）处理传统RNNs遇到的消失梯度问题，LSTM是GRU的推广。下表总结了每种结构的特征方程：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813588-53c68da7-47e4-4410-86ab-a93d9f52dd59.webp#clientId=u68d214c5-6527-4&from=paste&id=uff9cb62c&originHeight=493&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8ae462f0-53b2-424f-964e-9d235395b9b&title=)<br />注：符号![](https://cdn.nlark.com/yuque/__latex/4b4efc2fbe82a047fc08c83ea081f1d9.svg#card=math&code=%5Cstar&id=ALbXs)表示两个向量之间按元素相乘。
<a name="FIpRR"></a>
### RNN的变体
下表总结了其他常用的RNN模型：

| Bidirectional (BRNN) | Deep (DRNN) |
| --- | --- |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813702-1b375b3b-389c-4147-8f21-ae7592e17afe.webp#clientId=u68d214c5-6527-4&from=paste&id=u024d33de&originHeight=867&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa645721-fa73-47b6-ba4f-fd4beed26cb&title=) | ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639443813788-9650a068-839b-4f77-a36a-1886e416dde7.webp#clientId=u68d214c5-6527-4&from=paste&id=u993487cc&originHeight=1151&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4ee70acc-d17e-4ebf-8b4b-aa5fdce4436&title=) |

