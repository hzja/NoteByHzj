![image.png](./img/1685086731249-edd512de-a1d6-41c2-a7e3-6788c6bfd5f5.png)<br />架构各种探索主要分为Encoder-Decoder、Encoder-Only和Decoder-Only，如上图所示。现在大语言模型的主流架构大致分为三种类型，即因果解码器(Causal Decoder)、前缀解码器(Prefix Decoder)、编码器-解码器(Encoder-Decoder)：

- 因果解码器(Causal Decoder)：因果解码器架构采用单向注意力掩码，以确保每个输入标记只能关注过去的标记和它本身。输入和输出标记通过解码器以相同的方式进行处理。
- 前缀解码器(Prefix Decoder)：前缀解码器结构修正了因果编码器的掩码机制，以使其能可对前缀标记执行双向注意力，并仅对生成的标记执行单向注意力。这样，与encoder-decoder类似，可以双向编码前缀序列并自回归低逐个预测输出标记，其中在编码和解码阶段共享相同的参数。
- 编码器-解码器(encoder-decoder)：传统 Transformer 模型是建立在编码器-解码器架构上的，由两个 Transformer 块分别作为编码器和解码器。编码器采用堆叠的多头自注意层对输入序列进行编码以生成其潜在表示，而解码器对这些表示进行交叉注意并自回归地生成目标序列。

![image.png](./img/1685688912428-78df734d-082b-4e9f-867a-62fded4f4dba.png)<br />Decoder-only都是指因果解码器(Causal Decoder)，主要优点是没有降秩问题。Prefix Decoder只有U-PaLM和GLM，U-PaLM是在Causal Decoder PaLM的基础上微调成Prefix，纯正的PLM Prefix Decoder只有清华GLM，因为双向，就会降秩。Causal Decoder是绝对主流，Prefix Decoder和Encoder-Decoder也有不错表现。Encoder-only近年来没有更新进展主要是和主流方向不太相符。
<a name="nJW1p"></a>
# Encoder-Decoder
Encoder-Decoder结构是RNN最重要的一个变种，有多种称呼，比如Seq2Seq，Transfoer，Encoder-Decoder，Nvs.M的RNN等，具体可参见我之前的[深度学习-RNN-循环神经网络](https://www.yuque.com/angsweet/machine-learning/shen-du-xue-xi_shen-du-xue-xi_xun-huan-he-di-gui-wang-luo_xun-huan-shen-jing-wang-luo)章节。Encoder和Decoder结构是在[Attention Is All You Need](https://arxiv.org/abs/1706.03762)论文中被阐述，后续被广泛应用。<br />![Attention Is All You Need中描述](./img/1685947362178-900955cd-77de-47bf-b4af-be829fd1a12d.png "Attention Is All You Need中描述")<br />![Attention Is All You need中详细结构](./img/1685947488413-32b92aa8-c38a-4b92-b985-78f7376aca4b.png "Attention Is All You need中详细结构")<br />![简化版](./img/1685947880614-02a10466-adbf-4d68-90b9-87b01a11feb7.png "简化版")
<a name="qYkUV"></a>
## Attention
我在[深度学习-解析-Attention](https://www.yuque.com/angsweet/machine-learning/shen-du-xue-xi_shen-du-xue-xi_ji-shu-fen-xi_attention)章节有详细全面的介绍Attention，想全面掌握的可以跳转学习，这里仅再一次论述selft-attention相关。下图是论文中 Transformer 的内部结构图，左侧为 Encoder block，右侧为 Decoder block。红色圈中的部分为 Multi-Head Attention，是由多个 Self-Attention组成的，可以看到 Encoder block 包含一个 Multi-Head Attention，而 Decoder block 包含两个 Multi-Head Attention (其中有一个用到 Masked)。Multi-Head Attention 上方还包括一个 Add & Norm 层，Add 表示残差连接 (Residual Connection) 用于防止网络退化，Norm 表示 Layer Normalization，用于对每一层的激活值进行归一化。<br />![image.png](./img/1686017772892-509de24e-5039-49d8-a450-03b9fa008ce7.png)<br />因为 Self-Attention是 Transformer 的重点，所以我们重点关注 Multi-Head Attention 以及 Self-Attention，首先详细了解一下 Self-Attention 的内部逻辑。<br />![image.png](./img/1686017901544-891d9564-d878-4277-96c8-83e5cbb4d72c.png)
<a name="kcbsM"></a>
### Self-Attention
下图是 Self-Attention 的结构，在计算的时候需要用到矩阵![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)(查询)，![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)(键值)，![](./img/9f493997c33913987175caf4a4849955.svg)(值)。在实际中，Self-Attention 接收的是输入(单词的表示向量![](./img/712ecf7894348e92d8779c3ee87eeeb0.svg)组成的矩阵![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)) 或者上一个 Encoder block 的输出。而![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)，![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)正是通过 Self-Attention 的输入进行线性变换得到的。<br />![image.png](./img/1687673907122-6d12eac4-fe8e-4435-8aec-815db99ef169.png)<br />Self Attention也是在Scaled Dot-Product Attention单元里面实现的，如上图左图所示，首先把输入Input经过线性变换分别得到![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)，![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)，注意，![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)，![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)都来自于Input，只不过是线性变换的矩阵的权值不同而已。然后把![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)和![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)做dot Product相乘，得到输入Input词与词之间的依赖关系，然后经过尺度变换(scale)、掩码(mask)和softmax操作，得到最终的Self Attention矩阵。尺度变换是为了防止输入值过大导致训练不稳定，mask则是为了保证时间的先后关系。最后，把encoder端self Attention计算的结果加入到decoder做为![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)和![](./img/9f493997c33913987175caf4a4849955.svg)，结合decoder自身的输出做为![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)，得到encoder端的attention与decoder端attention之间的依赖关系

下面细讲Self Attention，![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)(Query)，![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)(Key)，![](./img/9f493997c33913987175caf4a4849955.svg)(Value)三个矩阵均来自同一输入，首先我们要计算![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)和![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)之间的点乘，然后为了防止其结果过大，会除以一个尺度标度![](./img/0e14005a4a886bbc4a99a987e2509e82.svg)，其中![](./img/7516b96678349ed002f1931a294f577c.svg)为一个query和key向量的维度。再利用Softmax操作将其结果归一化为概率分布，然后再乘以矩阵V就得到权重求和的表示。该操作可以表示为<br />![](./img/b81aacc017aea12665d9f0e25f4a080f.svg)<br />这里可能比较抽象，我们来看一个具体的例子（图片来源于[The Illustrated Transformer](https://jalammar.github.io/illustrated-transformer/)，，该博客讲解的极其清晰，强烈推荐）假如我们要翻译一个词组Thinking Machines，其中Thinking的输入的embedding vector用![](./img/aa687da0086c1ea060a8838e24611319.svg)表示，Machines的embedding vector用![](./img/8732099f74d777a67257cb2f04ead3d8.svg)表示。<br />![image.png](./img/1687663798808-6cca1f76-c1de-4757-842d-2d81fa42c1d4.png)<br />当我们处理Thinking这个词时，我们需要计算句子中所有词与它的Attention Score，这就像将当前词作为搜索的query，去和句子中所有词（包含该词本身）的key去匹配，看看相关度有多高。我们用![](./img/28dc930e7c69157c9b0876863eee407c.svg)代表Thinking对应的query vector，![](./img/5816a3210bbbab1d8dc57a0850a78101.svg)及![](./img/598f2d188e13c994d3ca15cec7cb87bc.svg)分别代表Thinking以及Machines对应的key vector，则计算Thinking的attention score的时候我们需要计算![](./img/28dc930e7c69157c9b0876863eee407c.svg)与![](./img/5816a3210bbbab1d8dc57a0850a78101.svg)，![](./img/598f2d188e13c994d3ca15cec7cb87bc.svg)的点乘，同理，我们计算Machines的attention score的时候需要计算![](./img/fa043c065dd111d926a3d140b618b05e.svg)与![](./img/5816a3210bbbab1d8dc57a0850a78101.svg)，![](./img/598f2d188e13c994d3ca15cec7cb87bc.svg)的点乘。如上图中所示我们分别得到了![](./img/28dc930e7c69157c9b0876863eee407c.svg)与![](./img/5816a3210bbbab1d8dc57a0850a78101.svg)，![](./img/598f2d188e13c994d3ca15cec7cb87bc.svg)的点乘积，然后我们进行尺度缩放与softmax归一化，如下图所示：<br />![image.png](./img/1687663839534-e9026787-3f1d-42ff-86e8-ac941ef2f456.png)<br />显然，当前单词与其自身的attention score一般最大，其他单词根据与当前单词重要程度有相应的score。然后我们在用这些attention score与value vector相乘，得到加权的向量。<br />![image.png](./img/1687663945143-14beff08-5bd9-4e7b-ab7f-007d090bd2b9.png)<br />如果将输入的所有向量合并为矩阵形式，则所有query, key, value向量也可以合并为矩阵形式表示<br />![image.png](./img/1687663986489-b7e09878-71e6-4f72-bf86-9a1292f13d7e.png)<br />![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)乘以![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)的转置后，得到的矩阵行列数都为![](./img/df378375e7693bdcf9535661c023c02e.svg)，![](./img/df378375e7693bdcf9535661c023c02e.svg)为句子单词数，这个矩阵可以表示单词之间的 attention 强度。下图为![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)乘以![](./img/703d6d1833d8fc879a427eb0f3da358e.svg)，![](./img/5cb207c577714753648ca727e6e092a1.svg)表示的是句子中的单词<br />![Q乘以K的转置的计算](./img/1687673467781-e725a139-2de6-4fcb-9b6b-00fe35b6b70b.png "Q乘以K的转置的计算")<br />得到![](./img/51bf47529d60d164b2759c3b8ae89b75.svg)之后，使用 Softmax 计算每一个单词对于其他单词的 attention 系数，公式中的 Softmax 是对矩阵的每一行进行 Softmax，即每一行的和都变为1<br />![对矩阵的每一行进行 Softmax](./img/1687673579931-7c6cef75-4b1b-413f-a4e2-a9b3e9994665.png "对矩阵的每一行进行 Softmax")<br />得到 Softmax 矩阵之后可以和![](./img/9f493997c33913987175caf4a4849955.svg)相乘，得到最终的输出![](./img/ca9972fb0e732f9a939604a4b2e0f4c0.svg)<br />![image.png](./img/1687673615648-2b8b5ccc-b45b-4d0a-a777-f82d8fbb3000.png)<br />上图中 Softmax 矩阵的第![](./img/53072c2388d69edc65c2377681e4e87c.svg)行表示单词![](./img/53072c2388d69edc65c2377681e4e87c.svg)与其他所有单词的 attention 系数，最终单词![](./img/53072c2388d69edc65c2377681e4e87c.svg)的输出![](./img/4774d09df340b823f29e88c62209f69e.svg)等于所有单词![](./img/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)的值![](./img/588455b04479646f6b7a4a2886b01dba.svg)根据 attention 系数的比例加在一起得到，如下图所示<br />![Z的计算方法](./img/1687673701149-5b2067f4-614b-4e17-a5d8-698c6129db8f.png "Z的计算方法")
<a name="GD4hE"></a>
### Multi-Head Attention
在上一步，我们已经知道怎么通过 Self-Attention 计算得到输出矩阵 Z，而 Multi-Head Attention 是由多个 Self-Attention 组合形成的，下图是论文中 Multi-Head Attention 的结构图。<br />![image.png](./img/1687673872779-82d011ed-516d-4ffa-a163-66d4d56cce8b.png)<br />其中![](./img/d7b6a2a0be5292d8ac0c334b71e840de.svg)，![](./img/826d7f5527d7ff305fa332da776b9016.svg)，![](./img/a410d5f91e526e902825e332cb8a0e6b.svg)是我们模型训练过程学习到的合适的参数。上述操作即可简化为矩阵形式<br />![image.png](./img/1687664010234-09ddb24b-83e2-4570-ac74-138587264153.png)<br />依此类推，multihead就是我们可以有不同的![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)，![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)表示，最后再将其结果结合起来，如下图所示：<br />![attention16.jpg](./img/1596095285925-cb2d168c-9fbc-4b2b-9690-7ae3854a658c.jpeg)

<a name="gs4CT"></a>
## Encoder
![image.png](./img/1687674182875-e539f4cd-9f4a-4823-bc91-7c409914cefa.png)<br />上图红色部分是 Transformer 的 Encoder block 结构，可以看到是由 Multi-Head Attention, Add & Norm, Feed Forward, Add & Norm 组成的。刚刚已经了解了 Multi-Head Attention 的计算过程，现在了解一下 Add & Norm 和 Feed Forward 部分
<a name="PK94t"></a>
### Add&Norm
Add & Norm 层由 Add 和 Norm 两部分组成，其计算公式如下：<br />![image.png](./img/1687674228975-f8b8deef-86db-4da5-94a4-b4686ed449fd.png)<br />其中 X表示 Multi-Head Attention 或者 Feed Forward 的输入，Multi-Head Attention(![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)) 和 Feed Forward(![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)) 表示输出 (输出与输入![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)维度是一样的，所以可以相加)

Add指 ![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)+ Multi-Head Attention(![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg))，是一种残差连接，通常用于解决多层网络训练的问题，可以让网络只关注当前差异的部分，在 ResNet 中经常用到：<br />![image.png](./img/1687674270253-5fed75f8-ceb4-4eac-9c44-b80f21374b8e.png)<br />Norm指 Layer Normalization，通常用于 RNN 结构，Layer Normalization 会将每一层神经元的输入都转成均值方差都一样的，这样可以加快收敛。
<a name="ETxeC"></a>
### Feed Forward
Feed Forward 层比较简单，是一个两层的全连接层，第一层的激活函数为 Relu，第二层不使用激活函数，对应的公式如下。<br />![image.png](./img/1687674293683-0a0b22dc-f866-4784-b28c-d8941e38a3a0.png)<br />![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)是输入，Feed Forward 最终得到的输出矩阵的维度与![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)一致。
<a name="POPLu"></a>
### 组成Encoder
通过上面描述的 Multi-Head Attention, Feed Forward, Add & Norm 就可以构造出一个 Encoder block，Encoder block 接收输入矩阵![](./img/dbd3c2199813e0e416ece2e9bc6aba81.svg)，并输出一个矩阵![](./img/0746f6abb71a55732e8262ad981a7d70.svg)。通过多个 Encoder block 叠加就可以组成 Encoder。

第一个 Encoder block 的输入为句子单词的表示向量矩阵，后续 Encoder block 的输入是前一个 Encoder block 的输出，最后一个 Encoder block 输出的矩阵就是编码信息矩阵![](./img/a42a4fc28b384cc408de066beed57485.svg)，这一矩阵后续会用到 Decoder 中<br />![image.png](./img/1687674385723-c19e8f1f-b61d-4c23-b49e-1a0ad8d083a6.png)
<a name="pRLDE"></a>
## Decoder
![image.png](./img/1687674398197-a2e7d6e4-7b11-41f4-a427-fb42ebe29933.png)<br />上图红色部分为 Transformer 的 Decoder block 结构，与 Encoder block 相似，但是存在一些区别：

- 包含两个 Multi-Head Attention 层
- 第一个 Multi-Head Attention 层采用了 Masked 操作
- 第二个 Multi-Head Attention 层的![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)矩阵使用 Encoder 的编码信息矩阵![](./img/a42a4fc28b384cc408de066beed57485.svg)进行计算，而![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)使用上一个 Decoder block 的输出计算
- 最后有一个 Softmax 层计算下一个翻译单词的概率
<a name="rqaV6"></a>
### 第一个Multi-Head Attention
Decoder block 的第一个 Multi-Head Attention 采用了 Masked 操作，因为在翻译的过程中是顺序翻译的，即翻译完第![](./img/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)个单词，才可以翻译第![](./img/ef641a858a2cbe77d7656beb54d1874f.svg)个单词。通过 Masked 操作可以防止第![](./img/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)个单词知道![](./img/ef641a858a2cbe77d7656beb54d1874f.svg)个单词之后的信息。下面以 "我有一只猫" 翻译成 "I have a cat" 为例，了解一下 Masked 操作。

下面的描述中使用了类似 Teacher Forcing 的概念，不熟悉 Teacher Forcing 的童鞋可以参考以下上一篇文章Seq2Seq 模型详解。在 Decoder 的时候，是需要根据之前的翻译，求解当前最有可能的翻译，如下图所示。首先根据输入 "<Begin>" 预测出第一个单词为 "I"，然后根据输入 "<Begin> I" 预测下一个单词 "have"。<br />![Decoder 预测](./img/1687674505395-2be063f4-f3fc-4996-b697-f2809fee96ca.png "Decoder 预测")<br />Decoder 可以在训练的过程中使用 Teacher Forcing 并且并行化训练，即将正确的单词序列 (<Begin> I have a cat) 和对应输出 (I have a cat <end>) 传递到 Decoder。那么在预测第![](./img/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)个输出时，就要将第![](./img/ef641a858a2cbe77d7656beb54d1874f.svg)之后的单词掩盖住，注意 Mask 操作是在 Self-Attention 的 Softmax 之前使用的，下面用![](./img/2393e86201de1afdcca3decd8f78668f.svg)分别表示 "<Begin> I have a cat <end>"。

**第一步：**是 Decoder 的输入矩阵和 Mask 矩阵，输入矩阵包含 "<Begin> I have a cat" (![](./img/2393e86201de1afdcca3decd8f78668f.svg)) 五个单词的表示向量，Mask 是一个![](./img/0fb0607e399002aea83e132439ab88f1.svg)的矩阵。在 Mask 可以发现单词![](./img/22d0feea96d3bb2fc273f7598ce748c1.svg)只能使用单词![](./img/22d0feea96d3bb2fc273f7598ce748c1.svg)的信息，而单词![](./img/53072c2388d69edc65c2377681e4e87c.svg)可以使用单词![](./img/e72b4c7598282e91bc1c568ad3eca8cb.svg)的信息，即只能使用之前的信息<br />![输入矩阵与 Mask 矩阵](./img/1687674543464-6d2f72db-cbc7-48c0-99a2-315651e6853d.png "输入矩阵与 Mask 矩阵")<br />**第二步：**接下来的操作和之前的 Self-Attention 一样，通过输入矩阵X计算得到![](./img/e528f6f55941fd46d7c56836d4749c25.svg)矩阵。然后计算![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)和![](./img/703d6d1833d8fc879a427eb0f3da358e.svg)的乘积![](./img/51bf47529d60d164b2759c3b8ae89b75.svg)<br />![Q乘以K的转置](./img/1687674623356-b102823a-0d87-4489-a0dc-05721c95389a.png "Q乘以K的转置")<br />**第三步：**在得到![](./img/51bf47529d60d164b2759c3b8ae89b75.svg)之后需要进行 Softmax，计算 attention score，我们在 Softmax 之前需要使用Mask矩阵遮挡住每一个单词之后的信息，遮挡操作如下：<br />![Softmax 之前 Mask](./img/1687674657464-1387910e-cff8-4fa8-8e89-a4cc5022347f.png "Softmax 之前 Mask")<br />得到 Mask ![](./img/51bf47529d60d164b2759c3b8ae89b75.svg)之后在 Mask ![](./img/51bf47529d60d164b2759c3b8ae89b75.svg)上进行 Softmax，每一行的和都为![](./img/53072c2388d69edc65c2377681e4e87c.svg)。但是单词![](./img/22d0feea96d3bb2fc273f7598ce748c1.svg)在单词![](./img/466a336aaac31c937faa74af9cf2559a.svg)上的 attention score 都为![](./img/22d0feea96d3bb2fc273f7598ce748c1.svg)<br />**第四步：**使用 Mask ![](./img/51bf47529d60d164b2759c3b8ae89b75.svg)与矩阵![](./img/9f493997c33913987175caf4a4849955.svg)相乘，得到输出![](./img/ca9972fb0e732f9a939604a4b2e0f4c0.svg)，则单词![](./img/53072c2388d69edc65c2377681e4e87c.svg)的输出向量 ![](./img/4774d09df340b823f29e88c62209f69e.svg)是只包含单词![](./img/53072c2388d69edc65c2377681e4e87c.svg)信息<br />![Mask 之后的输出](./img/1687674756832-457c1360-a485-43c2-bedb-86a95d576b0d.png "Mask 之后的输出")<br />第五步：通过上述步骤就可以得到一个 Mask Self-Attention 的输出矩阵![](./img/78d3f7d0db361689b1b4c483cd594e5e.svg)，然后和 Encoder 类似，通过 Multi-Head Attention 拼接多个输出![](./img/78d3f7d0db361689b1b4c483cd594e5e.svg)然后计算得到第一个 Multi-Head Attention 的输出![](./img/ca9972fb0e732f9a939604a4b2e0f4c0.svg)，![](./img/ca9972fb0e732f9a939604a4b2e0f4c0.svg)与输入![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)维度一样
<a name="vETTj"></a>
### 第二个Multi-Head Attention
Decoder block 第二个 Multi-Head Attention 变化不大， 主要的区别在于其中 Self-Attention 的![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)矩阵不是使用 上一个 Decoder block 的输出计算的，而是使用 Encoder 的编码信息矩阵![](./img/a42a4fc28b384cc408de066beed57485.svg)计算的。

根据 Encoder 的输出![](./img/a42a4fc28b384cc408de066beed57485.svg)计算得到![](./img/38a3f4d664b7a723d138f9d57be0c783.svg)，![](./img/9f493997c33913987175caf4a4849955.svg)，根据上一个 Decoder block 的输出![](./img/ca9972fb0e732f9a939604a4b2e0f4c0.svg)计算![](./img/4ef7132d0df72d9e3db76f6391960a3d.svg)(如果是第一个 Decoder block 则使用输入矩阵![](./img/94e79ad0c1aabeafef9e2fc4af6adf66.svg)进行计算)，后续的计算方法与之前描述的一致。

这样做的好处是在 Decoder 的时候，每一位单词都可以利用到 Encoder 所有单词的信息 (这些信息无需 Mask)。
<a name="xRQ5Q"></a>
### Softmax预测输出单词
Decoder block 最后的部分是利用 Softmax 预测下一个单词，在之前的网络层我们可以得到一个最终的输出![](./img/ca9972fb0e732f9a939604a4b2e0f4c0.svg)，因为 Mask 的存在，使得单词![](./img/22d0feea96d3bb2fc273f7598ce748c1.svg)的输出![](./img/20ca829f5d16d26b02abbdbc6485d883.svg)只包含单词![](./img/22d0feea96d3bb2fc273f7598ce748c1.svg)的信息，如下：<br />![Decoder Softmax 之前的 Z](./img/1687674872323-a0ac7e7d-3480-4371-b4b2-1e039293588a.png "Decoder Softmax 之前的 Z")<br />Softmax 根据输出矩阵的每一行预测下一个单词：<br />![Decoder Softmax 预测](./img/1687674904836-59d1a99f-4f8c-4fc7-9b8a-8f55b75394ba.png "Decoder Softmax 预测")<br />这就是 Decoder block 的定义，与 Encoder 一样，Decoder 是由多个 Decoder block 组合而成。
<a name="aezQ4"></a>
# BERT-style语言模型: Encoder-Decoder or Encoder-only
| 模型 | 论文链接 |
| --- | --- |
| BERT | [BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding. 2018](https://aclanthology.org/N19-1423.pdf) |
| RoBERTa | [RoBERTa: A Robustly Optimized BERT Pretraining Approach. 2019](https://arxiv.org/abs/1907.11692) |
| DistilBERT | [DistilBERT, a distilled version of BERT: smaller, faster, cheaper and lighter. 2019](https://arxiv.org/abs/1910.01108) |
| ALBERT | [ALBERT: A Lite BERT for Self-supervised Learning of Language Representations. 2019](https://arxiv.org/abs/1909.11942) |
| UniLM | [Unified Language Model Pre-training for Natural Language Understanding and Generation. 2019](https://arxiv.org/abs/1905.03197) |
| ELECTRA | [ELECTRA: PRE-TRAINING TEXT ENCODERS AS DISCRIMINATORS RATHER THAN GENERATORS. 2020](https://openreview.net/pdf?id=r1xMH1BtvB) |
| T5 | [Exploring the Limits of Transfer Learning with a Unified Text-to-Text Transformer. 2019](https://arxiv.org/abs/1910.10683) |
| GLM | [GLM-130B: An Open Bilingual Pre-trained Model. 2022](https://arxiv.org/abs/2210.02414) |
| AlexaTM | [AlexaTM 20B: Few-Shot Learning Using a Large-Scale Multilingual Seq2Seq Model. 2022](https://arxiv.org/abs/2208.01448) |
| ST-MoE | [ST-MoE: Designing Stable and Transferable Sparse Expert Models. 2022](https://arxiv.org/abs/2202.08906) |

<a name="CrLy5"></a>
# GPT-style语言模型: Decoder-only
| 模型 | 论文链接 |
| --- | --- |
| GPT | [Improving Language Understanding by Generative Pre-Training. 2018](https://s3-us-west-2.amazonaws.com/openai-assets/research-covers/language-unsupervised/language_understanding_paper.pdf) |
| GPT-2 | [Language Models are Unsupervised Multitask Learners. 2018](https://d4mucfpksywv.cloudfront.net/better-language-models/language_models_are_unsupervised_multitask_learners.pdf) |
| GPT-3 | [Language Models are Few-Shot Learners. 2020](https://arxiv.org/abs/2005.14165) |
| OPT | [OPT: Open Pre-trained Transformer Language Models](https://arxiv.org/abs/2205.01068) |
| PaLM | [PaLM: Scaling Language Modeling with Pathways. 2022](https://arxiv.org/abs/2204.02311) |
| BLOOM | [BLOOM: A 176B-Parameter Open-Access Multilingual Language Model. 2022](https://arxiv.org/abs/2211.05100) |
| MT-NLG | [Using DeepSpeed and Megatron to Train Megatron-Turing NLG 530B, A Large-Scale Generative Language Model. 2021](https://arxiv.org/abs/2201.11990) |
| GLaM | [GLaM: Efficient Scaling of Language Models with Mixture-of-Experts. 2022](https://arxiv.org/abs/2112.06905) |
| Gopher | [Scaling Language Models: Methods, Analysis & Insights from Training Gopher. 2021](http://arxiv.org/abs/2112.11446v2) |
| chinchilla | [Training Compute-Optimal Large Language Models. 2022](https://arxiv.org/abs/2203.15556) |
| LaMDA | [LaMDA: Language Models for Dialog Applications. 2021](https://arxiv.org/abs/2201.08239) |
| LLaMA | [LLaMA: Open and Efficient Foundation Language Models. 2023](https://arxiv.org/abs/2302.13971v1) |
| GPT-4 | [GPT-4 Technical Report. 2023](http://arxiv.org/abs/2303.08774v2) |
| BloombergGPT | [BloombergGPT: A Large Language Model for Finance. 2023](https://arxiv.org/abs/2303.17564) |
| GPT-NeoX-20B | [GPT-NeoX-20B: An Open-Source Autoregressive Language Model. 2022](https://arxiv.org/abs/2204.06745) |

<a name="syW4E"></a>
# Source
[The Illustrated Transformer](https://jalammar.github.io/illustrated-transformer/)<br />[完全图解RNN、RNN变体、Seq2Seq、Attention机制](https://zhuanlan.zhihu.com/p/28054589)<br />[Transformer模型详解（图解最完整版）](https://zhuanlan.zhihu.com/p/338817680)<br />[GitHub - Mooler0410/LLMsPracticalGuide: A curated list of practical guide resources of LLMs (LLMs Tree, Examples, Papers)](https://github.com/Mooler0410/LLMsPracticalGuide)<br />[GitHub - RUCAIBox/LLMSurvey: The official GitHub page for the survey paper “A Survey of Large Language Models”.](https://github.com/RUCAIBox/LLMSurvey)<br />[Attention Is All You Need](https://arxiv.org/abs/1706.03762)<br />[https://arxiv.org/pdf/2304.13712.pdf](https://arxiv.org/pdf/2304.13712.pdf)<br />[https://arxiv.org/pdf/2303.18223.pdf](https://arxiv.org/pdf/2303.18223.pdf)<br />[https://arxiv.org/pdf/2204.05832.pdf](https://arxiv.org/pdf/2204.05832.pdf)<br />[从Word Embedding到Bert模型—自然语言处理中的预训练技术发展史](https://zhuanlan.zhihu.com/p/49271699)<br />[乘风破浪的PTM：两年来预训练模型的技术进展](https://zhuanlan.zhihu.com/p/254821426)<br />[通向AGI之路：大型语言模型（LLM）技术精要](https://zhuanlan.zhihu.com/p/597586623)

