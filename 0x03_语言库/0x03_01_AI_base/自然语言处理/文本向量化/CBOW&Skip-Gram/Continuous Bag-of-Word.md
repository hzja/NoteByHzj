Continuous Bag-of-Word(CBOW)顾名思义，即连续词袋模型，即文本以单个词为最小单位，像“support vector machine”词组也会被当做三个独立的词考虑，且是连续词袋，即目标词的前后词也作为因素考虑。

<a name="d9612389"></a>
## One-word context

<a name="ab12ccc5"></a>
### 模型结构

下图为网络模型例子，词汇表大小为![](./img/5206560a306a2e085a437fd258eb57ce.svg)；隐藏层宽度为![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)(即我们想要的词向量维度)，各层连接为全连接方式；输入为one-hot编码的向量，即词汇表出现的![](./img/5206560a306a2e085a437fd258eb57ce.svg)个非重复词，一个词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的向量![](./img/d8c597021da2b34e2cc11f608e126ef8.svg)为对应![](./img/5a7fa14dac5c9d494aa75b18e1ad9a51.svg)的位置为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，其他位置都为![](./img/cfcd208495d565ef66e7dff9f98764da.svg)；真实的![](./img/415290769594460e2e485922904f345d.svg)为文本中输入词的下一个词的one-hot编码的向量。

![CBOW1.png](./img/1599029174771-1b5aa965-3085-4705-bf88-0cd89a799ab6.png)

输入层和隐藏层间的权重可由一个![](./img/60379337220a1788e57a9adf8e298281.svg)的矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)表示。![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的每一行是一个![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)维向量，表示输入层对应的词向量![](./img/8fc1c18c04aaa694c161d1c1bf61078f.svg)。

![](./img/4ea271606c586e0e8a53a9101ae3c901.svg)

![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)行是![](./img/9e55c5987341b7a0a7ea96022d84333c.svg)，给定一个词![](./img/c484d836ee7975e70a23e6529666f928.svg)且![](./img/c125598eb91915fa5e951dbbb869b498.svg)对于![](./img/1bef32dd034e10ea4d574d07a4f92acd.svg)(即这个词的one-hot向量只有![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)位置为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg))，可得：

![](./img/def8c815fb4c6944e4cf018514f5155f.svg)

其实就是将![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的第![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)行复制给了![](./img/2510c39011c5be704182423e3a695e91.svg)，因为![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)只有在第![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)位置是![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)（因为输入是one-hot，经过矩阵相乘其实就是把权重![](./img/61e9c06ea9a85a5088a499df6458d276.svg)对应行的值传递给下一层）。![](./img/5e6cac270d83c3ef95112fffe478a189.svg)即是输入词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)的向量表示。（这就意味着隐藏层的激活函数是线性的即可，不需要使用ReLU之类的对它们进行非线性变换。比如Multi-word context model中直接把这层的输入进行加权求和传给下层）

隐藏层到输出层的权重可用一个![](./img/1a0a5c611c9432c8e0140ea809eb8165.svg)的矩阵![](./img/4461c102803f90ac3229cd072fea19a3.svg)表示：

![](./img/8155ae9f2802e5e56418a70a8000e225.svg)

基于权重，我们对于每一个词汇表里的词可计算一个分数![](./img/7f2a13fc30f16b860d8d8b3e2ba033cd.svg)：

![](./img/8d3df38cde033b50288653c566be72eb.svg)

其中![](./img/5abeb80c246d9269baa19d1af1c6ebc9.svg)是![](./img/a88c849719f260199e49946541fd97b5.svg)第![](./img/363b122c528f54df4a0446b6bab05515.svg)列。然后我们用softmax去获得这个词的后验分布，是一个多项式分布：

![](./img/27967511137d1dad3c106704b786594d.svg)

其中![](./img/5a9ad302713f7739f121f71a8b263bab.svg)是输出层第![](./img/363b122c528f54df4a0446b6bab05515.svg)个单元的输出。结合输入层到隐藏层![](./img/9407d0b129b8f41695ce60e1bbf4d6cc.svg)和隐藏层到输出层![](./img/5e3fafe4a08cc6df1df412ac80007510.svg)公式代入softmax，我们得到：

![](./img/a2ab16708d0c623b44ea837029994f25.svg)

这里![](./img/8fc1c18c04aaa694c161d1c1bf61078f.svg)和![](./img/df6b98a48d1f040c46cbb1630347b570.svg)是词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的两种表达形式。![](./img/8fc1c18c04aaa694c161d1c1bf61078f.svg)源自输入层到隐藏层权重矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的行，![](./img/df6b98a48d1f040c46cbb1630347b570.svg)源自隐藏层到输出层权重矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)的列。我们将![](./img/8fc1c18c04aaa694c161d1c1bf61078f.svg)和![](./img/c41c256d4fa8a78877e1fd8b21ddf80e.svg)分别称为“输入向量”和“输出向量”。

模型目标是最大化![](./img/f576ad7e7d39974eec8756d73069e0eb.svg)，即模型输入![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)，模型输出![](./img/56b9717e224ac3315a16be96179614de.svg)(表示它的index在输出层为![](./img/cfac03e5dc007708c36c43216248aa1c.svg)) 与真实![](./img/415290769594460e2e485922904f345d.svg)(输入词的下一个词的one-hot向量)一致。即![](./img/415290769594460e2e485922904f345d.svg)向量第![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)位为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，其他为![](./img/cfcd208495d565ef66e7dff9f98764da.svg)，我们期望的最佳模型是输出层第![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)个单元为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，其他为![](./img/cfcd208495d565ef66e7dff9f98764da.svg)。模型使用反向传播进行训练。

<a name="28151a39"></a>
### 模型训练

<a name="98d2cd97"></a>
#### 隐藏层到输出层权重更新

训练目标即最大化![](./img/f576ad7e7d39974eec8756d73069e0eb.svg)， 公式（4）代表的就是给定上下文信息（这里为一个单词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)）以及其权重矩阵的情况下，预测其实际输出单词（即上下文信息的中心词![](./img/56b9717e224ac3315a16be96179614de.svg)）的条件概率

![](./img/27a34273a8ed5e30f8d589afba3d42f7.svg)<br />![](./img/8cd1070e2c7fc2f2ea0e7dc0a8f30ef6.svg)<br />![](./img/4054b7d5e43d5a08f9632b2dc56e4257.svg)

上式给了损失函数的定义，即![](./img/1b2f7cf4ccc80ad3dd5f4d7d05815ed2.svg)，我们旨在最小化![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)。![](./img/bd4c2017026988d1a9d25bdec900f8e7.svg)的表示方式由公式（2）而来，![](./img/cfac03e5dc007708c36c43216248aa1c.svg)则为实际输出单词的索引下标。我们注意到该损失函数可以理解为一种特殊情形下的交叉熵计算。

首先我们对损失函数![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)求关于![](./img/7f2a13fc30f16b860d8d8b3e2ba033cd.svg)的偏导数，我们可得

![](./img/ba2bec614585991f04e0c7a45d127279.svg)

上式给出了![](./img/b6f19bd9cacf5cb9b2c03d5615325e07.svg)的定义，其中![](./img/e81cfd62d282334e315e27a783b578cd.svg)只有在第![](./img/363b122c528f54df4a0446b6bab05515.svg)个单元是所期待的输出词（即真实的![](./img/415290769594460e2e485922904f345d.svg)）时才为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，其他情况下为![](./img/cfcd208495d565ef66e7dff9f98764da.svg)。这个导偏数其实就是表示在输出层的预测误差![](./img/b6f19bd9cacf5cb9b2c03d5615325e07.svg)。

我们根据链式法则求出损失函数![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)关于矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)元素![](./img/317e7c078454a337774f2b9c245c1ae7.svg)的偏导数以获得隐藏层到输出层权重的梯度

![](./img/b7f04fd7fc060ca08bbaf568084f60e5.svg)

因此，用随机梯度下降法，我们可以得到隐藏层到输出层的权重更新公式：

![](./img/def5784710781b7dd29c7cc0cb146435.svg)

或者

![](./img/3fc90ea8902eeea4897f159437d70cbe.svg)

其中![](./img/41800d853f240062dd0ce10d03c9561c.svg)是学习率，![](./img/d5e3dbc8fe4af7cf0ab879f35f32b36d.svg)，![](./img/57339b77b3af15427b7154f4daf8a223.svg)是隐藏层第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个单元；![](./img/5abeb80c246d9269baa19d1af1c6ebc9.svg)是![](./img/e8100be07fa5419af6c6738b934dfca0.svg)的输出向量。这个更新公式其实就表明了我们需要查看词汇表中每一个可能的词，比较网络的输出![](./img/5a9ad302713f7739f121f71a8b263bab.svg)与期望的输出(实际值) ![](./img/e81cfd62d282334e315e27a783b578cd.svg)：

- ![](./img/f0e675ced08800d98bce2ce8b483c133.svg)，那么就从向量![](./img/5abeb80c246d9269baa19d1af1c6ebc9.svg)中减去隐藏向量![](./img/2510c39011c5be704182423e3a695e91.svg)的一部分（例如![](./img/5e6cac270d83c3ef95112fffe478a189.svg)），这样向量![](./img/5abeb80c246d9269baa19d1af1c6ebc9.svg)就会与向量![](./img/5e6cac270d83c3ef95112fffe478a189.svg)相差更远
- ![](./img/48bc0c5c59fc56ea625407776f99a0de.svg)（这种情况只有在![](./img/711265fa4a8ec3391f297bcf5387d2f8.svg)时，才会发生，此时![](./img/c38083de2a443c2104d1bccba412d980.svg)），则将隐藏向量![](./img/2510c39011c5be704182423e3a695e91.svg)的一部分加入![](./img/cb303a28624850e37ccab51f886d09e8.svg)，使得![](./img/cb303a28624850e37ccab51f886d09e8.svg)与![](./img/5e6cac270d83c3ef95112fffe478a189.svg)更接近
- ![](./img/5a9ad302713f7739f121f71a8b263bab.svg)与![](./img/e81cfd62d282334e315e27a783b578cd.svg)非常接近，则此时![](./img/8307a91a9208ac824fdbba938c08d2a1.svg)由于公式（8）非常接近于![](./img/cfcd208495d565ef66e7dff9f98764da.svg)，故更新参数基本没什么变化

<a name="b1e28030"></a>
#### 输入层到隐藏层权重更新

我们继续对损失函数![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)求关于隐藏层![](./img/57339b77b3af15427b7154f4daf8a223.svg)的偏导数，得：

![](./img/69a74d25b89decd713c73bd63a259976.svg)

其中![](./img/57339b77b3af15427b7154f4daf8a223.svg)为隐藏层第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个神经单元的输出；![](./img/7f2a13fc30f16b860d8d8b3e2ba033cd.svg)在公式（2）中已经定义，表示输出层第![](./img/363b122c528f54df4a0446b6bab05515.svg)个神经单元的输入；![](./img/8307a91a9208ac824fdbba938c08d2a1.svg)为输出层第![](./img/363b122c528f54df4a0446b6bab05515.svg)个单元的预测误差。因此![](./img/088a2013906137902c9832d2f5a3a940.svg)应该是一个![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)维度向量，它的每个元素代表的是词汇表中的每个单词的预测误差![](./img/b6f19bd9cacf5cb9b2c03d5615325e07.svg)与其输出向量![](./img/317e7c078454a337774f2b9c245c1ae7.svg)在![](./img/027770abe9a99f31d66cb33a30e4494c.svg)到![](./img/5206560a306a2e085a437fd258eb57ce.svg)上的乘积之和。

接下来，我们需要求出损失函数![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)关于权重矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的偏导数。首先，分解公式（1），我们知道隐藏层激活单元的输出![](./img/57339b77b3af15427b7154f4daf8a223.svg)是输入层![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)与权重的线性组合，即

![](./img/3cc770425989e8af85af244cbdb2f396.svg)

因此对于权重矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的每一个元素，我们求关于![](./img/3a3ea00cfc35332cedf6e5e9a32e94da.svg)的偏导数，得到

![](./img/cc8c4d1b14c37e8e67fd1ef25e44af8a.svg)

因此我们利用张量乘积的方式，便可得到

![](./img/9491c6e4dc480fc6a77de79ffd0f32c0.svg)

我们再次得到了一个![](./img/1a0a5c611c9432c8e0140ea809eb8165.svg)的矩阵。由于![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)向量只有一个非![](./img/cfcd208495d565ef66e7dff9f98764da.svg)元素，因此![](./img/c489ebc0d3c58fc8172ab94c30949925.svg)只有一行是![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)维非![](./img/cfcd208495d565ef66e7dff9f98764da.svg)向量![](./img/2572f5a03c85e2b468a789b1b7e4bde5.svg)，因此矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的更新公式为

![](./img/d68d73382983990d50161135573a7c43.svg)

其中![](./img/5e6cac270d83c3ef95112fffe478a189.svg)是矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的其中一行，是唯一的上下文单词的“输入向量”，也是矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)唯一的导数非![](./img/cfcd208495d565ef66e7dff9f98764da.svg)的行向量。除了![](./img/5e6cac270d83c3ef95112fffe478a189.svg)以外，矩阵![](./img/61e9c06ea9a85a5088a499df6458d276.svg)的其他行向量在参数更新迭代过程中都会保持不变（因为其导数为![](./img/cfcd208495d565ef66e7dff9f98764da.svg))

与矩阵![](./img/a88c849719f260199e49946541fd97b5.svg)的更新过程相似，对于公式（16），我们分析如下：

- 如果过高地估计了某个单词![](./img/e8100be07fa5419af6c6738b934dfca0.svg)作为最终输出单词的概率（即：![](./img/f0e675ced08800d98bce2ce8b483c133.svg)），则上下文单词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)(context word)的输入向量与单词![](./img/e8100be07fa5419af6c6738b934dfca0.svg)的输出向量在更新的过程中会相差越来越大。
- 如果相反，某个单词![](./img/e8100be07fa5419af6c6738b934dfca0.svg)作为最终输出单词的概率被低估（即：![](./img/48bc0c5c59fc56ea625407776f99a0de.svg)），则单词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)的输入向量与单词![](./img/e8100be07fa5419af6c6738b934dfca0.svg)的输出向量在更新过程中会越来越接近。
- 如果对于单词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)的概率预测是准确的，则对于单词的输入向量在更新过程中几乎保持不变。

因此，上下文单词![](./img/9c3f4479e7c733d4b12c5b62cdadb4a2.svg)（context word ）的输入向量的更新取决于词汇表中所有单词的预测误差。预测误差越大，则该单词对于上下文单词的输入向量的更新过程影响越大。

<a name="859b88f0"></a>
## Multi-word context

根据字面意思我们就可以看出，基于multi-word context的CBOW模型就是利用多个上下文单词来推测中心单词target word的一种模型。比如下面这段话，我们的上下文大小取值为4，特定的这个词是"Learning"，也就是我们需要的输出词向量,上下文对应的词有8个，前后各4个，这8个词是我们模型的输入。由于CBOW使用的是词袋模型，因此这8个词都是平等的，也就是不考虑他们和我们关注的词之间的距离大小，只要在我们上下文之内即可

![CBOW.png](./img/1599031740182-b48837fd-6c2b-4d1a-8afd-1404ea24c657.png)

<a name="ab12ccc5-1"></a>
### 模型结构

其隐藏层的输出值的计算过程为：首先将输入的上下文单词（context words）的向量叠加起来并取其平均值，接着与输入层到隐藏层的权重矩阵相乘，作为最终的结果，公式如下：

![](./img/bbd3780ccc3c68c5cf38cd9c233b7f67.svg)<br />![](./img/a1d92cd0debe24b1e73136ffdc35cd5b.svg)

其中![](./img/0d61f8370cad1d412f80b84d143e1257.svg)为上下文单词的个数，![](./img/08e007d041aae77ff6b548dd6566dbd4.svg)为上下文单词，![](./img/8fc1c18c04aaa694c161d1c1bf61078f.svg)为单词![](./img/f1290186a5d0b1ceab27f4e77c0c5d68.svg)的输入向量。损失函数为

![](./img/651789246e18ab04060d2a019f1a759d.svg)<br />![](./img/828fe261a9d84e01063c953fe294a744.svg)<br />![](./img/98dac185be7f15b1cc9f98807cc22831.svg)

网络结构如下：

![CBOW2.png](./img/1599029197091-762d2967-2390-41dd-af2d-577ffe35af41.png)

<a name="28151a39-1"></a>
### 模型训练

<a name="98d2cd97-1"></a>
#### 隐藏层到输出层权重更新

同样，由隐藏层到输出层的权重更新公式与One-word context模型下的一模一样，即类似于公式（11），我们直接写在下面：

![](./img/a09c2740962272cb8bef8bc560b49699.svg)

<a name="b1e28030-1"></a>
#### 输入层到隐藏层权重更新

由输入层到隐藏层的权重矩阵更新公式与公式（16）类似，只不过现在我们需要对每一个上下文单词![](./img/fc6216c1c1e2881f188265af7dfabbee.svg)都执行如下更新公式：

![](./img/2cf187ca5888912281ce1297661b7c12.svg)

其中![](./img/1c98d7053b9a89f745bacc5bfa5d258e.svg)为context中第![](./img/4a8a08f09d37b73795649038408b5f33.svg)个单词的输入向量；![](./img/ffe9f913124f345732e9f00fa258552e.svg)为正学习速率；![](./img/865411a0349948f268d5d03c77360290.svg)由公式（12）给出。

<a name="Source"></a>
## Source

[https://arxiv.org/pdf/1411.2738.pdf](https://arxiv.org/pdf/1411.2738.pdf)<br />[https://blog.csdn.net/lanyu_01/article/details/80097350](https://blog.csdn.net/lanyu_01/article/details/80097350)<br />[https://www.cnblogs.com/pinard/p/7160330.html](https://www.cnblogs.com/pinard/p/7160330.html)
