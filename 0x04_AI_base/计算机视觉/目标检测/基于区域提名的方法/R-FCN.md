<a name="341e5bb3"></a>
## R-FCN思路

前面的目标检测方法都可以细分为两个子网络：共享的全卷积网络和不共享计算的与RoI相关的子网络（比如全连接网络）。

R-FCN将最后的全连接层之类的换成了一个位置敏感的卷积网络，从而让所有计算都可以共享。具体来说，先把每个提名区域划分为![](./img/5b5f0112171324a6f9fc5df06f6019dd.svg)个网络，比如R-FCN原论文中![](./img/8ce4b16b22b58894aa86c421e8759df3.svg)的取值为3，则对应的9个网格分别表示左上、上中...右下，对应于figure3、4中的九宫格及figure2中的不同颜色的块，每个网络都有对应的编码，但预测时会有![](./img/face97c01fed4b642bf28ee802cc6978.svg)个输出，![](./img/0d61f8370cad1d412f80b84d143e1257.svg)表示类别数量，![](./img/7015440590904d38e35615ff0672d21c.svg)是因为有背景类别，全部的输出通道数量为![](./img/fe862f5292dc63b8adec3816b9251180.svg)。

![R-FCN1.png](./img/1598598449389-30f10d27-03e9-4be2-86dd-eee1de978979.png)<br />![R-FCN2.png](./img/1598598476296-91d2be9f-ae6d-4958-a2e8-f4bdecefafb7.png)<br />![R-FCN3.png](./img/1598598483184-95c547ca-9251-43f6-8edb-40bb664f7057.png)<br />![R-FCN4.png](./img/1598598488846-0af0f835-e0cf-4c9e-8ba5-afc2cd94f9df.png)

需要注意的是，figure3、4和figure2中的不同位置都存在一个九宫格，但是池化时只有一个起作用，比如bottom-right层只有右下角的小块起作用。那么问题来了，这一层其他的8个框有什么作用呢？答案是它们可以作为其他RoI（偏左或偏上一些的RoI）的右下角。

<a name="f2d7cc0d"></a>
## R-FCN步骤

（1）区域提名：使用RPN（Region Proposal Network，区域提名网络），RPN本身是全卷积网络结构的

（2）分类与回归：利用和RPN共享的特征进行分类。当进行bbox回归时，则将![](./img/0d61f8370cad1d412f80b84d143e1257.svg)设置为4。

<a name="Source"></a>
## Source

[https://arxiv.org/abs/1605.06409](https://arxiv.org/abs/1605.06409)
