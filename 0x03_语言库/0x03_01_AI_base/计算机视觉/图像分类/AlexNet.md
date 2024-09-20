AlexNet可以说是一个具有里程碑意义的网络结构。在2012年的ILSVRC 2012中一举刷新了纪录，Top-5错误率比上一年的冠军下降了十多个百分点，而且远远超过当年的第二名ISI，从而奠定了深度学习在计算机视觉领域的霸主地位。

231n中对AlexNet的评价：

**AlexNet**. The first work that popularized Convolutional Networks in Computer Vision was the [AlexNet](http://papers.nips.cc/paper/4824-imagenet-classification-with-deep-convolutional-neural-networks), developed by Alex Krizhevsky, Ilya Sutskever and Geoff Hinton. The AlexNet was submitted to the [ImageNet ILSVRC challenge](http://www.image-net.org/challenges/LSVRC/2014/) in 2012 and significantly outperformed the second runner-up (top 5 error of 16% compared to runner-up with 26% error). The Network had a very similar architecture to LeNet, but was deeper, bigger, and featured Convolutional Layers stacked on top of each other (previously it was common to only have a single CONV layer always immediately followed by a POOL layer).

AlexNet网络包含8个学习层：5个卷积层和3个全连接层，最后的输出层为一个1000类的Softmax层。AlexNet模型中间层分为两路，明确显示了两块GPU之间的职责划分——一块GPU运行图中顶部模型部分，另一块GPU则运行图中底部模型部分。GPU之间仅在某些层相互通信。该网络的输入是150528维的，且该网络剩下的各层的神经元数分别为253440-186624-64896-64896-43264-4096-1000。

![AlexNet.png](./img/1596684242154-59f6280e-4e58-47a1-b539-8ffcfa605dad.png)

<a name="07ec1504"></a>
## AlexNet结构

结构如下图所示，5个卷积阶段（这里我不称为卷积层是因为这5部分不仅进行了卷积操作，还有其他运算），再接3个全连接层，最后1000维的Softmax输出。每层的维度及核数量如下下图所示。

![AlexNet1.png](./img/1596684269956-b4707292-60e0-41a5-9060-272e3bfe90ae.png)

![AlexNet2.png](./img/1596684292213-f6bcc918-45f2-4812-9174-ac458c2d4bff.png)

![AlexNet3.png](./img/1596684309552-f7e607e3-f2e1-4639-b1fb-c077c1a1773b.png)

<a name="c3f1d3bd"></a>
### 第一层(卷积)

![第一层.png](./img/1596684326053-a6b985fc-ef08-4ab4-b81f-94d1600eaded.png)

第一层输入数据为![](./img/2a2f1064a8264b36a464b4c8ad874f26.svg)（原始为![](./img/132e952037d4fd27735963da66d19851.svg)，加了padding，为了保证卷积之后图像尺寸依然是整数）的图像，这个图像被![](./img/32d647047774334e9575de31db88fb12.svg)的卷积核进行卷积运算，卷积核对原始图像的每次卷积都生成一个新的像素。卷积核沿原始图像的![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)轴方向和![](./img/415290769594460e2e485922904f345d.svg)轴方向两个方向移动，移动的步长是4个像素。因此，卷积核在移动的过程中会生成![](./img/f66dfabd3bd99ce168ae1c0a5e7c9ad7.svg)个像素(227个像素减去11再除以4，正好是54，即生成54个像素，再加上被减去的11也对应生成一个像素)，行和列的![](./img/08914710493ea53471484b3b8f10b658.svg)个像素形成对原始图像卷积之后的像素层。共有96个卷积核，会生成![](./img/bed3f6bbb6767733a06156f6e16a95c2.svg)个卷积后的像素层。96个卷积核分成2组，每组48个卷积核。对应生成2组![](./img/1bef14f7381f1d6d0d846c0d388014f2.svg)的卷积后的像素层数据。这些像素层经过ReLU单元的处理，生成激活像素层，尺寸仍为2组![](./img/1bef14f7381f1d6d0d846c0d388014f2.svg)的像素层数据。

这些像素层经过池化处理，池化的尺寸为![](./img/5793c51d91799e7fb044517074a59eb0.svg)，运算的步长为2，则池化有图像的尺寸为![](./img/d87263b9fa480ed9a1f66b5e043071fc.svg)。即池化后像素的规模为![](./img/9c96fa36c10a5114df9bcf765fbc8275.svg)；然后经过归一化处理，归一化运算的尺度为![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)；第一卷积层运算结束后形成的像素层的规模为![](./img/9c96fa36c10a5114df9bcf765fbc8275.svg)。分别对应96个卷积核所运算形成。这96层像素层分为2组，每组48个像素层，每组在一个独立的GPU上进行运算。

反向传播时，每个卷积核对应一个偏差值。即第一层的96个卷积核对应上层输入的96个偏差值。

<a name="3047515b"></a>
### 第二层(卷积)

![第二层.png](./img/1597125837293-3f8fcc81-6617-4a55-b4f5-3c4a0c04cc58.png)

第二层输入数据为第一层输出的![](./img/9c96fa36c10a5114df9bcf765fbc8275.svg)的像素层，为便于后续处理，每幅像素层的左右两边和上下两边都要填充2个像素；![](./img/9c96fa36c10a5114df9bcf765fbc8275.svg)的像素数据分成![](./img/351c173de4722c46e0e43c2dbc0ff780.svg)的两组像素数据，两组数据分别再两个不同的GPU中进行运算。每组像素数据被![](./img/ea7e6b77d85b481730799b6622a073ec.svg)的卷积核进行卷积运算，卷积核对每组数据的每次卷积都生成一个新的像素。卷积核沿原始图像的![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)轴方向和![](./img/415290769594460e2e485922904f345d.svg)轴方向两个方向移动，移动的步长是1个像素。因此，卷积核在移动的过程中会生成![](./img/eda03f44b92ac4551b39ec1da13b365d.svg)个像素(27个像素减去5，正好是22，在加上上下、左右各填充的2个像素，即生成26个像素，再加上被减去的5也对应生成一个像素)。行和列的![](./img/d0e263f923ca937d054463c8d399af72.svg)个像素形成对原始图像卷积之后的像素层。共有256个![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)卷积核；这256个卷积核分成2组，每组针对一个GPU中的![](./img/351c173de4722c46e0e43c2dbc0ff780.svg)的像素进行卷积运算。会生成![](./img/c0d45ab2f92b6b2fd795e5bf1fd1b447.svg)个卷积后的像素层。这些像素层经过ReLU单元的处理，生成激活像素层，尺寸仍为2组![](./img/3bec1a119954cb6a7eb8f03ee97ce291.svg)的像素层。

这些像素层经过池化运算，池化的尺寸为3*3，运算的步长为2，则池化后的图像尺寸为![](./img/0057215111514e103566f0b7fde9e64c.svg)。即池化后像素的规模为2组![](./img/3df1f0686524ca636c8b77d24f843875.svg)的像素层；然后经过归一化处理，归一化运算尺寸为![](./img/f57cbc4e8ef8235bc15328cedf283f97.svg)；第二卷积层运算结束后形成的像素层的规模为2组![](./img/3df1f0686524ca636c8b77d24f843875.svg)的像素层。分别对应2组128个卷积核所运算形成。每组在一个GPU上进行运算。即共256个卷积核，共2个GPU进行运算。

反向传播时，每个卷积核对应一个偏差值。即第一层的96个卷积核对应上层输入的256个偏差值。

<a name="feea8d44"></a>
### 第三层(卷积)

![第三层.png](./img/1597133319277-5a7b547d-f3e4-419c-a1d6-4a4db234daaf.png)

第三层输入数据为第二层输出的2组![](./img/3df1f0686524ca636c8b77d24f843875.svg)的像素层；为便于后续处理，每幅像素层的左右两边和上下两边都要填充1个像素；2组像素层数据都被送至2个不同的GPU中进行运算。每个GPU中都有192个卷积核，每个卷积核的尺寸是![](./img/51f6e747b261afb91c5500ef1708784c.svg)。因此，每个GPU中的卷积核都能对2组![](./img/3df1f0686524ca636c8b77d24f843875.svg)的像素层的所有数据进行卷积运算。卷积核对每组数据的每次卷积都生成一个新的像素。卷积核沿像素层数据的![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)轴方向和![](./img/415290769594460e2e485922904f345d.svg)轴方向两个方向移动，移动的步长是1个像素。因此，运算后的卷积核的尺寸为![](./img/ae5f221f7699c36fe8e5e197980a1c68.svg)（13个像素减去3，正好是10，在加上上下、左右各填充的1个像素，即生成12个像素，再加上被减去的3也对应生成一个像素），每个GPU中共![](./img/42ea828fb00373fc373b2ec6b064d297.svg)个卷积核。2个GPU中共![](./img/e7be89a1396a404979365ea95d630546.svg)个卷积后的像素层。这些像素层经过ReLU单元的处理，生成激活像素层，尺寸仍为2组![](./img/42ea828fb00373fc373b2ec6b064d297.svg)像素层，共![](./img/e7be89a1396a404979365ea95d630546.svg)个像素层。

<a name="48816849"></a>
### 第四层(卷积)

![第四层.png](./img/1597134126498-d0476ea6-7a04-4f93-b65f-9a7f657667e0.png)

第四层输入数据为第三层输出的2组13*13*192的像素层；为便于后续处理，每幅像素层的左右两边和上下两边都要填充1个像素；2组像素层数据都被送至2个不同的GPU中进行运算。每个GPU中都有192个卷积核，每个卷积核的尺寸是3*3*192。因此，每个GPU中的卷积核能对1组![](./img/42ea828fb00373fc373b2ec6b064d297.svg)的像素层的数据进行卷积运算。卷积核对每组数据的每次卷积都生成一个新的像素。卷积核沿像素层数据的![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)轴方向和![](./img/415290769594460e2e485922904f345d.svg)轴方向两个方向移动，移动的步长是1个像素。因此，运算后的卷积核的尺寸为![](./img/ae5f221f7699c36fe8e5e197980a1c68.svg)（13个像素减去3，正好是10，在加上上下、左右各填充的1个像素，即生成12个像素，再加上被减去的3也对应生成一个像素），每个GPU中共![](./img/42ea828fb00373fc373b2ec6b064d297.svg)个卷积核。2个GPU中共![](./img/e7be89a1396a404979365ea95d630546.svg)个卷积后的像素层。这些像素层经过ReLU单元的处理，生成激活像素层，尺寸仍为2组![](./img/42ea828fb00373fc373b2ec6b064d297.svg)像素层，共![](./img/e7be89a1396a404979365ea95d630546.svg)个像素层。

<a name="e06409a6"></a>
### 第五层(卷积)

![第五层.png](./img/1597748862444-1b5aa612-d06d-40c8-92f2-fe480cbaf757.png)

第五层输入数据为第四层输出的2组![](./img/42ea828fb00373fc373b2ec6b064d297.svg)的像素层；为便于后续处理，每幅像素层的左右两边和上下两边都要填充1个像素；2组像素层数据都被送至2个不同的GPU中进行运算。每个GPU中都有128个卷积核，每个卷积核的尺寸是![](./img/940686da710739e4f694ff4cd82f1418.svg)。因此，每个GPU中的卷积核能对1组![](./img/42ea828fb00373fc373b2ec6b064d297.svg)的像素层的数据进行卷积运算。卷积核对每组数据的每次卷积都生成一个新的像素。卷积核沿像素层数据的![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)轴方向和![](./img/415290769594460e2e485922904f345d.svg)轴方向两个方向移动，移动的步长是1个像素。因此，运算后的卷积核的尺寸为![](./img/ae5f221f7699c36fe8e5e197980a1c68.svg)（13个像素减去3，正好是10，在加上上下、左右各填充的1个像素，即生成12个像素，再加上被减去的3也对应生成一个像素），每个GPU中共![](./img/3df1f0686524ca636c8b77d24f843875.svg)个卷积核。2个GPU中共![](./img/9e272a38158f8b6668eb444e50a95e0f.svg)个卷积后的像素层。这些像素层经过ReLU单元的处理，生成激活像素层，尺寸仍为2组![](./img/3df1f0686524ca636c8b77d24f843875.svg)像素层，共![](./img/9e272a38158f8b6668eb444e50a95e0f.svg)个像素层。

2组![](./img/3df1f0686524ca636c8b77d24f843875.svg)像素层分别在2个不同GPU中进行池化运算。池化运算的尺寸为![](./img/5793c51d91799e7fb044517074a59eb0.svg)，运算步长为2，则池化后图像的尺寸为![](./img/b46adddc21f148d33d918a4f6f37fbae.svg)。即池化后像素的规模为2组![](./img/aafdf67dd3fe65f0ca0379cfced71b40.svg)的像素层数据，共![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)规模的像素层数据。

<a name="54ab53be"></a>
### 第六层(全连接)

![第六层.png](./img/1597749033711-f2cc89b7-df53-4cde-9e9b-15e3985a9470.png)

第六层输入数据的尺寸是![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)，采用![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)尺寸的滤波器对第六层的输入数据进行卷积运算；每个![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)尺寸的滤波器对第六层的输入数据进行卷积运算生成一个运算结果，通过一个神经元输出这个运算结果；共有4096个![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)尺寸的滤波器对输入数据进行卷积运算，通过4096个神经元输出运算结果；这4096个运算结果通过ReLU激活函数生成4096个值；并通过drop运算后输出4096个本层的输出结果值。

由于第六层的运算过程中，采用的滤波器的尺寸![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)与待处理的特征图的尺寸相同，即滤波器中的每个系数只与特征图中的一个像素值相乘；而其它卷积层中，每个滤波器的系数都会与多个特征图中像素值相乘；因此，将第六层称为全连接层。

第五层输出的![](./img/2aaf3b7ffd35f596c6f9030fd3bcf5c3.svg)规模的像素层数据与第六层的4096个神经元进行全连接，然后经由ReLU进行处理后生成4096个数据，再经过dropout处理后输出4096个数据。

<a name="58f9cd0a"></a>
### 第七层(全连接)

![第七层.png](./img/1597749062484-a44119a9-95d9-46f9-a87f-2410e2cbd2c5.png)

第六层输出的4096个数据与第七层的4096个神经元进行全连接，然后经由ReLU进行处理后生成4096个数据，再经过dropout处理后输出4096个数据。

<a name="0ed6a151"></a>
### 第八层(全连接)

![第八层.png](./img/1597749096445-b6e64075-8526-4cae-bcc8-1fe906e50374.png)

第七层输出的4096个数据与第八层的1000个神经元进行全连接，经过训练后输出被训练的数值。

<a name="45749311"></a>
## AlexNet特点

![AlexNet特点.png](./img/1597749146197-016ce551-23f3-4201-bba9-66baf36d5e17.png)
<a name="d430d32f"></a>
### ReLU非线性激活函数

通常神经元对输入![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)的激活函数使用![](./img/ba7dc904f816625cf206c28dbffbb7ed.svg)或![](./img/d2b5c320a304df7d1b0cf1c9f6f71cf9.svg)。无论是tanh还是sigmoid函数，都会在![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)取值很大或很小时进入饱和区，这时候神经元的梯度会接近于![](./img/cfcd208495d565ef66e7dff9f98764da.svg)。在使用梯度下降法的反向传播过程中，这些饱和的非线性函数容易造成梯度消失，导致网络很难学习。而非饱和的非线性函数ReLU（![](./img/9db6867e7cad45ba0853963a952a0fbc.svg) ）能够在一定程度上克服这一问题。通常认为使用ReLU的深度神经网络训练速度比同样使用tanh的网络有数倍提升。

![ReLU1.png](./img/1598494758514-b729d059-12ce-4dc5-b9e3-63fded237bd4.png) ![ReLU2.png](./img/1598494771837-e05d168f-c154-4206-96a0-a74f5663b898.png)

<a name="748b6ef8"></a>
### 百万数据和多GPU训练

AlexNet当时使用GTX 580 GPU进行实验。单块GTX 580 GPU只有3GB显存，这限制了在其上进行训练的最大网络规模。处理训练模型所需的128万张图片数据对于一块GPU而言太吃力，于是AlexNet被拆分到两块GPU上。GPU能够直接对其他GPU的显存进行读写，不需要通过主机内存，故而特别适合跨GPU并优化。

AlexNet的并行方案除将模型的神经元进行均分外，还采用了一种技巧：GPU之间的通信只在某些层进行。如下图所示。第三层卷积需要以第二层产生的所有特征图作为输入，而第四层卷积则只需要以第三层的特征图中处在同一块GPU的部分作为输入。选择层间特征图的连接关系是一个交叉验证问题，但这使得我们能够将通信量精确地调整到一个可接受的范围内。

![多GPU.png](./img/1598494823921-d08471b2-bb7d-49c0-9af6-e6b547c759d8.png)

与单块GPU上训练得到的每个卷积层核数减半的网络相比，上述方案使Top-1与Top-5误差率分别减少了1.7%和1.2%。（事实上，由于模型的大多数参数集中在第一个全连接层，而这个全连接层以最后一个卷积层作为输入，这里是用的单GPU模型既没有减半最后一个卷积层的核数，也没有减半全连接的尺寸。由此单GPU和双GPU网络具有大致相同的参数数量。这样的设置可能对单GPU网络更有利一些，虽然无法精确体现参数数量对模型精度的影响，但更有效地证明了使用双GPU网络的必要性。）训练双GPU网络的耗时比单GPU网络略少一些。

![多GPU2.png](./img/1598494842167-48ad08ed-e30b-4fe6-a630-286817606b81.png)

<a name="d8347f4a"></a>
### 局部响应归一化

采用ReLU一个有利的属性时不需要对输入进行特别的归一化来避免激活函数进入饱和区。对于一个ReLU单元，只要训练样本中至少有一部分数据能对其产生正值的输入，这个神经元便能够进行学习。不过，AlexNet同样提出使用局部归一化(Local Response Normalization, LRN)有助于模型泛化。

定义![](./img/9d788cb1f0d8c90831a6db6dd81c0361.svg)表示卷积核![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)在图像坐标![](./img/90cbc22edf225adf8a68974f51227f05.svg)处得到的神经元激活值，继而通过非线性ReLU，用![](./img/1cdfac04cfd1897d7315cbc3491edfae.svg)表示归一化后的激活值，表达式如下图：

![局部响应归一化.png](./img/1598494984109-5e767216-e10b-4d35-825b-bb1b45cedb12.png)

其中，求和部分设计了相同空间位置下![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)个“相邻”的特征图，![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)表示该层中特征图的总数量。特征图的顺序关系可以在训练开始前任意生成。应用这样的响应归一化，形成了一种特征图间的侧向抑制，使不同激活单元的输出值相互竞争。常数![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)，![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)，![](./img/7b7f9dbfea05c83784f8b85149852f08.svg)，![](./img/b0603860fcffe94e5b8eec59ed813421.svg)是超参数，通常由验证集决定，AlexNet当时使用的参数配置是![](./img/38c9abaca4c0e762d42281e967e45959.svg)，![](./img/44d21af66b0874d9b45905ea79807cb3.svg)，![](./img/656502039794cb0aeb7d637b72440e4d.svg)，![](./img/4cf8aa5a22bf6baa9f3ebd0a7ba42135.svg)。归一化应用于某些层的ReLU非线性激活之后。

LRN这一方案与Jarrett等人提出的局部对比度归一化策略有相似之处。不过，AlexNet的归一化策略准确说来是一种“亮度归一化”，它不需要减去平均激活值。采用这一策略，使得Top-1和Top-5错误率分别下降了1.4%和1.2%。此外，CIFAR-10数据集上的实验也同样证明了响应归一化的有效性：利用一个普通的四层卷积网络，测试错误率为13%，加入归一化后错误率下降到11%。

<a name="2c680dea"></a>
### 重叠池化

![重叠池化.png](./img/1598496028972-3df2beac-7de5-4aa7-8b25-8278954e4b20.png)

在卷积网络中池化层可以视为对同一特征图中相邻神经元输出的一种概括。传统上，相邻的池化单元是互不重叠的。更准确地说，一个池化层可以被想象成由间隔![](./img/03c7c0ace395d80182db07ae2c30f034.svg)像素的若干池化网格单元组成，每个网格单元将对以该单元位置为中心、尺寸![](./img/df0cb370269714cf4cc611f5a2ca4ab5.svg)的邻域进行处理。

![重叠池化2.png](./img/1598496060718-2d31193e-8673-4ab7-a298-c126a942fd64.png)

若设![](./img/9c3b18c20940bce03f4257d26370e01c.svg)，我们便得到了CNN中常见的传统池化层；若设![](./img/994a841d5e2a311ded343f760792df7a.svg)，得到的便是一个有重叠的池化层。在AlexNet中，取![](./img/1854056973826159fd101c7787ad422e.svg)，![](./img/4ff87b04e2a1483fa004459f8a3ca745.svg)时，模型Top-1和Top-5错误率比采用不重叠的![](./img/1854056973826159fd101c7787ad422e.svg)，![](./img/c6c6aa9b314f0b77c1e5fd9aeeafa6ed.svg)分别下降了0.4%和0.3%。

同时在AlexNet的训练过程中也观测到，有重叠的池化层相比传统池化层出现过拟合现象的问题也略有缓解

<a name="0787c4d6"></a>
### 整体网络结构

AlexNet网络包含8个学习层：5个卷积层和3个全连接层，最后的输出层为一个1000类的Softmax层，产生一个1000类标签的分布。网络优化目标是最大化多分类逻辑回归，也等价于在最大化预测分布下训练样本中正确标签的对数概率平均值。

第二、四、五个卷积层只连接到前一个卷积层中也位于同一块GPU上的那些特征图，第三个卷积层则连接着第二个卷积层中的所有特征图，全连接层中的神经元连接前一层所有的神经元。

![整体网络结构.png](./img/1598496145643-6ff79768-6028-4666-843d-7ef0b2f3c7bb.png)

如下图，在第一和第二卷积层后面各接了一个局部响应归一化层。在局部响应归一化层和第五个卷积层之后接的最大池化层采用了前文介绍的重叠池化方式。所有卷积层和全连接层都采用ReLU作为非线性激活函数。网络输入层仍采用224*224*3的输入图像（加padding变为227*227*3）。第一个卷积层包含96个尺寸为11*11*3、步长为4个像素的卷积核。第二层卷积层将第一个卷积层的输出作为输入（包含局部响应归一化和池化），该层包含256个5*5*48的卷积核。第三、四、五个卷积层顺序连接、其间不采用池化和归一化。第三个卷积层包含384个尺寸为3*3*256的卷积核，与第二个卷积层相连（归一化、池化）。第四个卷积层由384个尺寸为3*3*192的卷积核组成，而第五个卷积层则由256个尺寸为3*3*192的卷积核组成。每个全连接层都包含4096个神经元。

![整体网络结构2.png](./img/1598496167693-1303573a-5670-4246-a354-e0418d30794c.png)

<a name="6c0a965e"></a>
### 降低过拟合

AlexNet包含6000万个参数，650000个神经元。尽管ILSVRC的1000个类别使得每个训练样本都能够为从图像到标签提供10bit的约束，但是对学习如此数量庞大的参数模型还是会出现明显的过拟合现象。当时AlexNet采用了两种对抗过拟合的方法：数据增强，Dropout

<a name="e4fc36e3"></a>
#### 数据增强

很多时候，大规模神经网络的效果直接取决于训练数据的规模，通常充足、丰富的训练数据能够有效地提高网络精度。正因如此，克服过拟合最简单、直接的方法是对标注数据进行一系列变换，并将新产生的数据集作为补充加入训练数据中。其中，通用的图像数据变形包括裁剪、平移、尺度变化、水平翻转，以及增加一些随机的光照、色彩变换。

AlexNet采用了两种数据增强的变换方式：

（1）第一种变换方式由生成图像裁剪和水平翻转组成。在256*256的训练数据原图上随机裁剪尺寸为224*224的片段（及它们的水平翻转），并在这些数据上训练网络。这使得训练集规模扩大了2048倍，但显然这样产生的训练样本之间具有高度依赖性。如果不采用这样的策略，网络模型可能过拟合严重，从而迫使我们使用比较小的网络。实际中，这种数据增强的方法往往是比较有效的。

在测试阶段，在输入图片的四角和中心裁剪5个224*224的图像块及它们的水平翻转都送入网络进行预测，最后对10个由Softmax层产生的输出进行平均作为最终的预测结果。

![数据增强.png](./img/1598496211409-1fc9c6ad-df64-4006-97d6-413c6daa15ac.png)

（2）第二种变换方式是改变训练数据中RGB通道的强度。具体来说，就是对整个ImageNet训练数据的RGB像素值做主成分分析(PCA)。变换过程是，将一个训练数据的RGB叠加上PCA得到的主成分所对应的特征向量乘以一个零均值、标准差为0.1的高斯分布的随机变量。这样，RGB的每个像素值为：

![](./img/719e80e83cee8937fedbf18fb2a83b2a.svg)

其中，![](./img/eca91c83a74a2373ca5f796700e99fd3.svg)和![](./img/5614371f803f8a78b18b27391549a107.svg)表示RGB像素值对应的3*3协方差矩阵中第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个特征向量和特征值，![](./img/cd0f1069db14b3485b705eb04d3e58a4.svg)则是上文中提到的随机系数。采用这种变换能够使网络近似地学习到自然物体识别中一个很重要的属性，即物体识别应该对光照强度和颜色保持不变性。

![数据增强2.png](./img/1598496300535-b777e319-b022-4a8c-96c3-8daab951bee7.png)

<a name="Dropout"></a>
#### Dropout

结合多个模型的预测结果能有效地降低测试误差，但对于需要花费数天来训练的大型神经网络来说，这样做显得太过昂贵。然而，模型融合有一个非常高效的版本“Dropout”。“Dropout”以一定的概率将一个神经元输出设置为0.以这种被关闭的神经元在前向、后向传播过程中都不起作用。这样每次来一个输入，网络便采样出一个不同的结构，但这些结构共享一套参数。由于一个神经元不能只依赖有某些特定的神经元，这种技术降低了神经元间复杂的共适性。这样，强迫神经元学习更为鲁棒的特征，从而适应与其他神经元的不同随机子集相结合。

![Dropout.png](./img/1598496319947-9cbc6603-06cd-44b7-956d-627a907d2780.png)

<a name="0d053b6e"></a>
## Code实现

```python
def AlexNet():
    model = Sequential()
    model.add(Conv2D(96,(11,11),strides=(4,4),input_shape=(227,227,3),padding='valid',activation='relu',kernel_initializer='uniform'))
    model.add(MaxPooling2D(pool_size=(3,3),strides=(2,2)))
    model.add(Conv2D(256,(5,5),strides=(1,1),padding='same',activation='relu',kernel_initializer='uniform'))
    model.add(MaxPooling2D(pool_size=(3,3),strides=(2,2)))
    model.add(Conv2D(384,(3,3),strides=(1,1),padding='same',activation='relu',kernel_initializer='uniform'))
    model.add(Conv2D(384,(3,3),strides=(1,1),padding='same',activation='relu',kernel_initializer='uniform'))
    model.add(Conv2D(256,(3,3),strides=(1,1),padding='same',activation='relu',kernel_initializer='uniform'))
    model.add(MaxPooling2D(pool_size=(3,3),strides=(2,2)))
    model.add(Flatten())
    model.add(Dense(4096,activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(4096,activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(1000,activation='softmax'))
    return model
```

<a name="Source"></a>
## Source

[http://cs231n.github.io/convolutional-networks/#case](http://cs231n.github.io/convolutional-networks/#case)<br />[https://www.cnblogs.com/skyfsm/p/8451834.html](https://www.cnblogs.com/skyfsm/p/8451834.html)<br />[http://vision.stanford.edu/teaching/cs231b_spring1415/slides/alexnet_tugce_kyunghee.pdf](http://vision.stanford.edu/teaching/cs231b_spring1415/slides/alexnet_tugce_kyunghee.pdf)<br />[https://papers.nips.cc/paper/4824-imagenet-classification-with-deep-convolutional-neural-networks.pdf](https://papers.nips.cc/paper/4824-imagenet-classification-with-deep-convolutional-neural-networks.pdf)<br />[https://blog.csdn.net/zyqdragon/article/details/72353420](https://blog.csdn.net/zyqdragon/article/details/72353420)
