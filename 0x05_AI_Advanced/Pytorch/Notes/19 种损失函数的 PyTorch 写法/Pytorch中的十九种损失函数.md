PyTorch<br />这里汇总了十九种损失函数，介绍了其数学公式和代码实现方式。
<a name="WIDAF"></a>
## 1、基本用法
```python
criterion = LossCriterion() #构造函数有自己的参数
loss = criterion(x, y) #调用标准时也有参数
```
<a name="aq2tw"></a>
## 2、损失函数
<a name="l6VgD"></a>
### 2-1 L1范数损失 L1Loss
计算 output 和 target 之差的绝对值。
```python
torch.nn.L1Loss(reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean: 返回loss和的平均值；sum: 返回loss的和。默认: mean。
<a name="CYTq3"></a>
### 2-2 均方误差损失 MSELoss
计算 output 和 target 之差的均方差。
```python
torch.nn.MSELoss(reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean: 返回loss和的平均值；sum: 返回loss的和。默认: mean。
<a name="Q7wyP"></a>
### 2-3 交叉熵损失 CrossEntropyLoss
当训练有 C 个类别的分类问题时很有效. 可选参数 weight 必须是一个1维 Tensor，权重将被分配给各个类别. 对于不平衡的训练集非常有效。<br />在多分类任务中，经常采用 softmax 激活函数+交叉熵损失函数，因为交叉熵描述了两个概率分布的差异，然而神经网络输出的是向量，并不是概率分布的形式。所以需要 softmax激活函数将一个向量进行“归一化”成概率分布的形式，再采用交叉熵损失函数计算 loss。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772438984-78e5350a-98f3-4138-bb08-8a3a74949333.png#clientId=ua0783c72-665b-4&from=paste&id=u94c46781&originHeight=78&originWidth=558&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udc39eb4c-1c8f-4608-bbe7-e7790e5e4bb&title=)
```python
torch.nn.CrossEntropyLoss(weight=None, ignore_index=-100, reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个类别的权重. 必须是一个长度为 C 的 Tensor<br />`ignore_index (int, optional)` – 设置一个目标值，该目标值会被忽略，从而不会影响到 输入的梯度。<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认: mean。
<a name="jGjbr"></a>
### 2-4 KL 散度损失 KLDivLoss
计算 input 和 target 之间的 KL 散度。KL 散度可用于衡量不同的连续分布之间的距离，在连续的输出分布的空间上(离散采样)上进行直接回归时很有效.
```python
torch.nn.KLDivLoss(reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean: 返回loss和的平均值；sum: 返回loss的和。默认: mean。
<a name="iUIxZ"></a>
### 2-5 二进制交叉熵损失 BCELoss
二分类任务时的交叉熵计算函数。用于测量重构的误差，例如自动编码机. 注意目标的值 t[i] 的范围为0到1之间.
```python
torch.nn.BCELoss(weight=None, reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个 batch 元素的 loss 的权重，必须是一个长度为 “nbatch” 的 的 Tensor<br />`pos_weight(Tensor, optional)` – 自定义的每个正样本的 loss 的权重，必须是一个长度 为 “classes” 的 Tensor
<a name="jV8Cx"></a>
### 2-6 BCEWithLogitsLoss
BCEWithLogitsLoss损失函数把 Sigmoid 层集成到了 BCELoss 类中，该版比用一个简单的 Sigmoid 层和 BCELoss 在数值上更稳定，因为把这两个操作合并为一个层之后，可以利用 log-sum-exp 的 技巧来实现数值稳定.
```python
torch.nn.BCEWithLogitsLoss(weight=None, reduction='mean', pos_weight=None)
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个 batch 元素的 loss 的权重，必须是一个长度 为 “nbatch” 的 Tensor<br />`pos_weight(Tensor, optional)` – 自定义的每个正样本的 loss 的权重，必须是一个长度 为 “classes” 的 Tensor
<a name="yvb0u"></a>
### 2-7 MarginRankingLoss
```python
torch.nn.MarginRankingLoss(margin=0.0, reduction='mean')
```
对于 mini-batch(小批量) 中每个实例的损失函数如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772438996-9829c83e-b25a-483f-b08f-d36197fe6366.png#clientId=ua0783c72-665b-4&from=paste&id=uc4f0a089&originHeight=38&originWidth=398&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29ae6fed-e8bc-418c-98b0-cea0ce3ad73&title=)<br />参数：<br />margin: 默认值0
<a name="YOSp2"></a>
### 2-8 HingeEmbeddingLoss
```python
torch.nn.HingeEmbeddingLoss(margin=1.0,  reduction='mean')
```
对于 mini-batch(小批量) 中每个实例的损失函数如下：<br />参数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439000-b23c414c-c4b3-4f17-a982-91161adc9447.png#clientId=ua0783c72-665b-4&from=paste&id=u4486d941&originHeight=73&originWidth=321&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ued859d62-7d69-4b5c-aab1-e5f9d6a827c&title=)<br />margin: 默认值1
<a name="ShOch"></a>
### 2-9 多标签分类损失 MultiLabelMarginLoss
```python
torch.nn.MultiLabelMarginLoss(reduction='mean')
```
对于mini-batch(小批量) 中的每个样本按如下公式计算损失：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772438933-acb2a00a-689c-4c7b-8ed8-5b5ee8952f89.png#clientId=ua0783c72-665b-4&from=paste&id=uc396bb04&originHeight=67&originWidth=371&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2479c3e2-b73e-4e84-bc19-6bb9a4eb2f4&title=)
<a name="opkIe"></a>
### 2-10 平滑版L1损失 SmoothL1Loss
也被称为 Huber 损失函数。
```python
torch.nn.SmoothL1Loss(reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772438990-d65f33be-e934-495d-9e3b-773596d96dae.png#clientId=ua0783c72-665b-4&from=paste&id=u80ec4e60&originHeight=64&originWidth=216&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5c087324-f803-4de2-8fb8-cd105807e79&title=)<br />其中<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439195-3313cb88-348e-4522-812e-265f45a75779.png#clientId=ua0783c72-665b-4&from=paste&id=uf303a03e&originHeight=65&originWidth=344&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9b9898bf-b460-4603-b644-e3f8506c9b0&title=)
<a name="sBvns"></a>
### 2-11 2分类的logistic损失 SoftMarginLoss
```python
torch.nn.SoftMarginLoss(reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439252-5c78e14f-e9f6-4742-8ae0-f806eac90ad6.png#clientId=ua0783c72-665b-4&from=paste&id=u1a1ea373&originHeight=70&originWidth=372&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9d5829d9-ac6c-4d37-8603-20a0461461c&title=)
<a name="MrmaD"></a>
### 2-12 多标签 one-versus-all 损失 MultiLabelSoftMarginLoss
```python
torch.nn.MultiLabelSoftMarginLoss(weight=None, reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439476-0a7b7095-4969-4742-9ad7-c66e088ab773.png#clientId=ua0783c72-665b-4&from=paste&id=u1b7220ed&originHeight=77&originWidth=751&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4efcd4bf-6974-4e89-b01a-ff99caee129&title=)
<a name="dyTKQ"></a>
### 2-13 cosine 损失 CosineEmbeddingLoss
```python
torch.nn.CosineEmbeddingLoss(margin=0.0, reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439301-29aa66bf-c737-42a3-b0cc-8f8013a48129.png#clientId=ua0783c72-665b-4&from=paste&id=u2187d8f4&originHeight=69&originWidth=496&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u55f7ca24-b6a2-4c00-b64c-b2d9a09d600&title=)<br />参数：<br />margin:  默认值0
<a name="ZYrLQ"></a>
### 2-14 多类别分类的hinge损失 MultiMarginLoss
```python
torch.nn.MultiMarginLoss(p=1, margin=1.0, weight=None,  reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439314-082244a0-93c8-4677-aeed-443d47d4aa5d.png#clientId=ua0783c72-665b-4&from=paste&id=ud2da00e8&originHeight=62&originWidth=470&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56cf6f9f-331f-4039-b742-b2cd6112d58&title=)<br />参数：<br />p=1或者2 默认值: 1<br />margin: 默认值1
<a name="mSpfr"></a>
### 2-15 三元组损失 TripletMarginLoss
```python
torch.nn.TripletMarginLoss(margin=1.0, p=2.0, eps=1e-06, swap=False, reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439513-0f0805fa-5e32-4031-8571-df6744925fb5.png#clientId=ua0783c72-665b-4&from=paste&id=ubde13d4f&originHeight=43&originWidth=432&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u176e4261-90da-45b0-9a0d-8330232914c&title=)<br />其中：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1656772439535-3f44bec6-7248-41eb-b9ab-508d98b988fa.png#clientId=ua0783c72-665b-4&from=paste&id=u3462079b&originHeight=37&originWidth=205&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5e45e4a3-3d5f-4f0e-bac5-8574233b55d&title=)
<a name="fEney"></a>
### 2-16 连接时序分类损失 CTCLoss
CTC连接时序分类损失，可以对没有对齐的数据进行自动对齐，主要用在没有事先对齐的序列化数据训练上。比如语音识别、ocr识别等等。
```python
torch.nn.CTCLoss(blank=0, reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean: 返回loss和的平均值；sum: 返回loss的和。默认: mean。
<a name="PZV70"></a>
### 2-17 负对数似然损失 NLLLoss
负对数似然损失. 用于训练 C 个类别的分类问题.
```python
torch.nn.NLLLoss(weight=None, ignore_index=-100,  reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个类别的权重，必须是一个长度为 C 的 Tensor<br />`ignore_index (int, optional)` – 设置一个目标值，该目标值会被忽略，从而不会影响到输入的梯度。
<a name="GNFqM"></a>
### 2-18 NLLLoss2d
对于图片输入的负对数似然损失. 它计算每个像素的负对数似然损失。
```python
torch.nn.NLLLoss2d(weight=None, ignore_index=-100, reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个类别的权重. 必须是一个长度为 C 的 Tensor<br />reduction-三个值，none: 不使用约简；mean: 返回loss和的平均值；sum: 返回loss的和。默认: mean。
<a name="X7f76"></a>
### 2-19 PoissonNLLLoss
目标值为泊松分布的负对数似然损失
```python
torch.nn.PoissonNLLLoss(log_input=True, full=False,  eps=1e-08,  reduction='mean')
```
参数：<br />`log_input (bool, optional)` – 如果设置为 True，loss 将会按照公 式 `exp(input) - target * input` 来计算，如果设置为 False，loss 将会按照 `input - target * log(input+eps)` 计算。<br />`full (bool, optional)` – 是否计算全部的 loss，i. e. 加上 Stirling 近似项 `target * log(target) - target + 0.5 * log(2 * pi * target)`。<br />`eps (float, optional)` – 默认值: 1e-8
