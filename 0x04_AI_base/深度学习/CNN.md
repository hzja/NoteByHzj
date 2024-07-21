<a name="ee22c7b6"></a>
## 基本组成单元

卷积(Convolution)、激活函数(Active Function)、池化(Pooling)、全连接(Softmax)

<a name="8b27df5e"></a>
### 卷积(Convolution) 
有padding，strides，dilation，transposed四种(对应下图前四个)。第五个为padding+strides，第六个为padding+strides+transposed。<br />![CNN2.png](./img/1594025687296-acdff1a5-6e05-4a39-85f1-a282c9fd7625.png)![CNN3.png](./img/1594025701084-ed5ff57e-4917-4ae8-8c34-e73c62d90f17.png)

<a name="08ec0a88"></a>
### 激活函数(Active Function) 

模仿人类神经元、非线性变换增强特征的表达能力、同时考虑优化时梯度消失问题 (轻轻抚摸一个人可能感觉不到，但重击一个人会有明显感觉，激活函数作相似事情，增加特征表达能力)

![](./img/1592023501820-2ee1c5df-4e4e-4554-9e29-34b1ac1f9ca5.png)

![](./img/1592023501873-c0a4036e-c459-4f58-b586-00c4a1b0cb35.png)

![](./img/1592023501919-8adafc0d-b870-46ff-be72-a8d39cb4aaf2.png)

<a name="4bb90152"></a>
### 池化(Pooling) 

降低计算复杂度，增强特征的空间变换不变性

![](./img/1592023501982-6bcc9cb6-5d12-414d-9851-ae15e7feff5e.png)

<a name="e05e567d"></a>
### 全连接(Softmax) 

全连接操作的特点：需要固定维度、参数多，计算量大，占整个网络的参数量、计算量的一半以上

![](./img/b522265f890a584b217582e10835dfc7.svg)

![](./img/1592023502048-7d7fbd1f-ad85-4253-ac4a-1619bbdb9d33.png)

<a name="0d053b6e"></a>
## Code实现

[https://github.com/jiqizhixin/ML-Tutorial-Experiment/blob/master/Experiments/tf_Keras_CNN.ipynb](https://github.com/jiqizhixin/ML-Tutorial-Experiment/blob/master/Experiments/tf_Keras_CNN.ipynb)
