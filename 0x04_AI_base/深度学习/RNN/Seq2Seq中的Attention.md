在Encoder-Decoder结构（即Seq2Seq，N vs. M）中，Encoder把所有的输入序列都编码成一个统一的语义特征![](./img/4a8a08f09d37b73795649038408b5f33.svg)再解码，因此，![](./img/4a8a08f09d37b73795649038408b5f33.svg) 中必须包含原始序列中的所有信息，它的长度就成了限制模型性能的瓶颈。如机器翻译问题，当要翻译的句子较长时，一个![](./img/4a8a08f09d37b73795649038408b5f33.svg)可能存不下那么多信息，就会造成翻译精度的下降。

![attention1.jpg](./img/1594262083428-ef2317f1-2a52-42e7-ad7a-543509b0ec19.jpeg)或者![attention2.jpg](./img/1594262101774-30d8846e-59b6-467a-9265-1d45ea69756a.jpeg)

Attention机制通过在每个时间输入不同的![](./img/4a8a08f09d37b73795649038408b5f33.svg)来解决这个问题，下图是带有Attention机制的Decoder：

![attention3.jpg](./img/1594262288085-f6eb092f-be85-4acf-9732-302c05957d6d.jpeg)

每一个![](./img/4a8a08f09d37b73795649038408b5f33.svg)会自动去选取与当前所要输出的![](./img/415290769594460e2e485922904f345d.svg)最合适的上下文信息。具体来说，我们用![](./img/9a59294d35ec9247478796ffb89359eb.svg)衡量Encoder中第![](./img/363b122c528f54df4a0446b6bab05515.svg)阶段的![](./img/7891fa1c2293f9c8b0796c28c083c500.svg)和解码时第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)阶段的相关性，最终Decoder中第i阶段的输入的上下文信息![](./img/96fafac0c054b9eb47d3f630ed02c289.svg)就来自于所有![](./img/7891fa1c2293f9c8b0796c28c083c500.svg)对![](./img/9a59294d35ec9247478796ffb89359eb.svg)的加权和。

以机器翻译为例（将中文翻译成英文）：

![attention4.jpg](./img/1594262471950-13a2dcaf-67b2-417a-b87b-25d6e09866a9.jpeg)

输入的序列是“我爱中国”，因此，Encoder中的![](./img/f429c22e302e299d92b05ee6c8c4a4c7.svg)就可以分别看做是“我”、“爱”、“中”、“国”所代表的信息。在翻译成英语时，第一个上下文![](./img/576f1dacd615219d9f8bea06b26d5fdc.svg)应该和“我”这个字最相关，因此对应的![](./img/b91902652a46b22e848b8a69e4265f92.svg)就比较大，而相应的![](./img/20493d24189b386010b737998fda0660.svg)就比较小。![](./img/71f0427a673c14326195285a092cc63a.svg)应该和“爱”最相关，因此对应的![](./img/6f0742941894eb318ac076c8ffb880b4.svg)就比较大。最后的![](./img/20b620923ab918a6f2b7a0eb419f8fc4.svg)和![](./img/40814808923c481689acfe57deb8e8c1.svg)最相关，因此![](./img/22c1adf27f15faf7e8304de4919f085e.svg)的值就比较大。

至此，关于Attention模型，我们就只剩最后一个问题了，那就是：这些权重![](./img/9a59294d35ec9247478796ffb89359eb.svg)是怎么来的？事实上，![](./img/9a59294d35ec9247478796ffb89359eb.svg)同样是从模型中学出的，它实际和Decoder的第![](./img/800ba394e806c049c98cf4b570d04c72.svg)阶段的隐状态、Encoder第![](./img/363b122c528f54df4a0446b6bab05515.svg)个阶段的隐状态有关。

同样还是拿上面的机器翻译举例，![](./img/9a59294d35ec9247478796ffb89359eb.svg)的计算（此时箭头就表示对![](./img/06d7d40c143e482eeadfda4d7ac69b32.svg)和![](./img/7891fa1c2293f9c8b0796c28c083c500.svg)同时做变换）：

![](./img/83ff5873c4d62c5d0407bf86014f7b6a.svg)计算：  ![attention5.jpg](./img/1594262703125-362f357c-5e56-46cf-9c28-6259c6fe1f0b.jpeg)

![](./img/653139ec2d5456201a5e5d2adbcf959d.svg)计算： ![attention6.jpg](./img/1594262752387-a4cd83a4-abd7-4aba-ad0b-df4c17ec53de.jpeg)

![](./img/3f4c0d781325c9777a75b7bb4838c0a2.svg)计算： ![attention7.jpg](./img/1594262764865-4ded44ac-bac4-4245-9b96-0420e47e16d1.jpeg)

以上就是带有Attention的Encoder-Decoder模型计算的全过程。

<a name="Source"></a>
## Source
[完全图解RNN、RNN变体、Seq2Seq、Attention机制](https://zhuanlan.zhihu.com/p/28054589)
