PyTorch
<a name="bsGE9"></a>
## 1、基本用法
```python
criterion = LossCriterion() #构造函数有自己的参数
loss = criterion(x, y) #调用标准时也有参数
```
<a name="AJHFY"></a>
## 2、损失函数
<a name="X1Y0G"></a>
### 2-1 L1范数损失 L1Loss
计算 output 和 target 之差的绝对值。
```python
torch.nn.L1Loss(reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认：mean。
<a name="Hnrj4"></a>
### 2-2 均方误差损失 MSELoss
计算 output 和 target 之差的均方差。
```python
torch.nn.MSELoss(reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认：mean。
<a name="Dq2iw"></a>
### 2-3 交叉熵损失 CrossEntropyLoss
当训练有 C 个类别的分类问题时很有效，可选参数 weight 必须是一个1维 Tensor，权重将被分配给各个类别，对于不平衡的训练集非常有效。<br />在多分类任务中，经常采用 softmax 激活函数+交叉熵损失函数，因为交叉熵描述了两个概率分布的差异，然而神经网络输出的是向量，并不是概率分布的形式。所以需要 softmax激活函数将一个向量进行“归一化”成概率分布的形式，再采用交叉熵损失函数计算 loss。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816580-e9cfc7fe-b46a-4c0c-919f-4bfb2ecdcd58.png#clientId=uf4835b3e-3679-4&from=paste&id=u563ebb16&originHeight=78&originWidth=558&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u218361a4-dbe4-4135-b950-6fe3b63e460&title=)
```python
torch.nn.CrossEntropyLoss(weight=None, ignore_index=-100, reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个类别的权重，必须是一个长度为 C 的 Tensor<br />`ignore_index (int, optional)` – 设置一个目标值，该目标值会被忽略，从而不会影响到 输入的梯度。<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认：mean。
<a name="BP9Aw"></a>
### 2-4 KL 散度损失 KLDivLoss
计算 input 和 target 之间的 KL 散度。KL 散度可用于衡量不同的连续分布之间的距离，在连续的输出分布的空间上(离散采样)上进行直接回归时很有效。
```python
torch.nn.KLDivLoss(reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认：mean。
<a name="pwEwF"></a>
### 2-5 二进制交叉熵损失 BCELoss
二分类任务时的交叉熵计算函数。用于测量重构的误差，例如自动编码机，注意目标的值 t[i] 的范围为0到1之间。
```python
torch.nn.BCELoss(weight=None, reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个 batch 元素的 loss 的权重，必须是一个长度为 “nbatch” 的 的 Tensor<br />`pos_weight(Tensor, optional)` – 自定义的每个正样本的 loss 的权重，必须是一个长度 为 “classes” 的 Tensor
<a name="YgCwD"></a>
### 2-6 BCEWithLogitsLoss
BCEWithLogitsLoss损失函数把 Sigmoid 层集成到了 BCELoss 类中。该版比用一个简单的 Sigmoid 层和 BCELoss 在数值上更稳定，因为把这两个操作合并为一个层之后，可以利用 log-sum-exp 的 技巧来实现数值稳定。
```python
torch.nn.BCEWithLogitsLoss(weight=None, reduction='mean', pos_weight=None)
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个 batch 元素的 loss 的权重，必须是一个长度 为 “nbatch” 的 Tensor<br />`pos_weight(Tensor, optional)` – 自定义的每个正样本的 loss 的权重，必须是一个长度 为 “classes” 的 Tensor
<a name="k29sI"></a>
### 2-7 MarginRankingLoss
```python
torch.nn.MarginRankingLoss(margin=0.0, reduction='mean')
```
对于 mini-batch(小批量) 中每个实例的损失函数如下:<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816584-40abc291-2d51-4498-911f-1559eecb53cd.png#clientId=uf4835b3e-3679-4&from=paste&id=ub2704488&originHeight=38&originWidth=398&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub93d38f8-905e-4581-b8f2-5440157020d&title=)<br />参数：<br />margin:默认值0
<a name="BSu0L"></a>
### 2-8 HingeEmbeddingLoss
```python
torch.nn.HingeEmbeddingLoss(margin=1.0,  reduction='mean')
```
对于 mini-batch(小批量) 中每个实例的损失函数如下:<br />参数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816592-8e2a830e-9265-476b-b002-774d1f85b77b.png#clientId=uf4835b3e-3679-4&from=paste&id=u730788e0&originHeight=73&originWidth=321&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46912fc7-3f62-471a-b40d-bf2d91cfb68&title=)<br />margin:默认值1
<a name="VZNcm"></a>
### 2-9 多标签分类损失 MultiLabelMarginLoss
```python
torch.nn.MultiLabelMarginLoss(reduction='mean')
```
对于mini-batch(小批量) 中的每个样本按如下公式计算损失:<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816628-331cff81-5fcb-4cd4-911e-8821f09f1f73.png#clientId=uf4835b3e-3679-4&from=paste&id=uf8451146&originHeight=67&originWidth=371&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ued9f969c-e0f3-4f35-b228-222bc08f829&title=)
<a name="ttFdb"></a>
### 2-10 平滑版L1损失 SmoothL1Loss
也被称为 Huber 损失函数。
```python
torch.nn.SmoothL1Loss(reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816585-39849efd-9373-4acd-989f-4776c8e768fd.png#clientId=uf4835b3e-3679-4&from=paste&id=ub1bf648b&originHeight=64&originWidth=216&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf3e33a74-0a30-4f60-a092-989c9919c25&title=)<br />其中<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816904-c4c5f1ad-03e9-4bba-9876-3a15d9419694.png#clientId=uf4835b3e-3679-4&from=paste&id=u47f53dd6&originHeight=65&originWidth=344&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u629c1669-1894-4f69-b1d7-1a56fd8d0ee&title=)
<a name="pSMtO"></a>
### 2-11 2分类的logistic损失 SoftMarginLoss
```python
torch.nn.SoftMarginLoss(reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816903-bd7cc1f5-5ffd-4686-83eb-d64b0563caf1.png#clientId=uf4835b3e-3679-4&from=paste&id=u6d336472&originHeight=70&originWidth=372&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u545837b8-e829-4ea1-9317-c5370eb12c8&title=)
<a name="LuWKL"></a>
### 2-12 多标签 one-versus-all 损失 MultiLabelSoftMarginLoss
```python
torch.nn.MultiLabelSoftMarginLoss(weight=None, reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816850-5b8bce1d-875d-4ea5-a4b0-d4d1409ac27a.png#clientId=uf4835b3e-3679-4&from=paste&id=u5aa3eb9c&originHeight=77&originWidth=751&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf6bba9d1-d8e5-4685-ba4f-22fd1936b34&title=)
<a name="zRra8"></a>
### 2-13 cosine 损失 CosineEmbeddingLoss
```python
torch.nn.CosineEmbeddingLoss(margin=0.0, reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816905-9e047056-6b09-42b6-9870-b12430dbb31e.png#clientId=uf4835b3e-3679-4&from=paste&id=u5ea631d8&originHeight=69&originWidth=496&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u892b6341-611f-441f-b088-1b9aa088760&title=)<br />参数：<br />margin:默认值0
<a name="ZZ6Lc"></a>
### 2-14 多类别分类的hinge损失 MultiMarginLoss
```python
torch.nn.MultiMarginLoss(p=1, margin=1.0, weight=None,  reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040816950-3a7f1acf-534e-44b0-b4bb-e9e672e44cfa.png#clientId=uf4835b3e-3679-4&from=paste&id=uda254d56&originHeight=62&originWidth=470&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucad813d9-6f7a-42d7-a837-3f553f93917&title=)<br />参数：<br />p=1或者2 默认值：1<br />margin:默认值1
<a name="c6DMr"></a>
### 2-15 三元组损失 TripletMarginLoss
```python
torch.nn.TripletMarginLoss(margin=1.0, p=2.0, eps=1e-06, swap=False, reduction='mean')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040817130-0ec1ed10-d220-4f27-8162-e903cb3378bd.png#clientId=uf4835b3e-3679-4&from=paste&id=u929cb0ee&originHeight=43&originWidth=432&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua622d832-5dda-4adb-b039-9373aa6a060&title=)<br />其中：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040817215-9547db87-81af-457f-b99e-b5a5beefa790.png#clientId=uf4835b3e-3679-4&from=paste&id=ufffb8d8d&originHeight=37&originWidth=205&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u85a6bc03-bdc6-42e7-bf2f-9ff03a2c013&title=)
<a name="kmsC4"></a>
### 2-16 连接时序分类损失 CTCLoss
CTC连接时序分类损失，可以对没有对齐的数据进行自动对齐，主要用在没有事先对齐的序列化数据训练上。比如语音识别、ocr识别等等。
```python
torch.nn.CTCLoss(blank=0, reduction='mean')
```
参数：<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认：mean。
<a name="WckBG"></a>
### 2-17 负对数似然损失 NLLLoss
负对数似然损失. 用于训练 C 个类别的分类问题.
```python
torch.nn.NLLLoss(weight=None, ignore_index=-100,  reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个类别的权重，必须是一个长度为 C 的 Tensor<br />`ignore_index (int, optional)` – 设置一个目标值, 该目标值会被忽略，从而不会影响到 输入的梯度。
<a name="nwnCM"></a>
### 2-18 NLLLoss2d
对于图片输入的负对数似然损失，它计算每个像素的负对数似然损失。
```python
torch.nn.NLLLoss2d(weight=None, ignore_index=-100, reduction='mean')
```
参数：<br />`weight (Tensor, optional)` – 自定义的每个类别的权重。必须是一个长度为 C 的 Tensor<br />reduction-三个值，none: 不使用约简；mean:返回loss和的平均值；sum:返回loss的和。默认：mean。
<a name="ecHQC"></a>
### 2-19 PoissonNLLLoss
目标值为泊松分布的负对数似然损失
```python
torch.nn.PoissonNLLLoss(log_input=True, full=False,  eps=1e-08,  reduction='mean')
```
参数：<br />`log_input (bool, optional)` – 如果设置为 True，loss 将会按照公 式 `exp(input) - target * input` 来计算，如果设置为 False，loss 将会按照 `input - target * log(input+eps)` 计算。<br />`full (bool, optional)` – 是否计算全部的 loss，i. e. 加上 Stirling 近似项 `target * log(target) - target + 0.5 * log(2 * pi * target)`。<br />`eps (float, optional)` – 默认值: 1e-8
