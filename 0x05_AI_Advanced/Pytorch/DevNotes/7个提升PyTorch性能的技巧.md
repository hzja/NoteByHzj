PyTorch
<a name="SBaIC"></a>
## 在DataLoaders中使用workers
```python
DataLoader(dataset, num_workers = 8)
```
第一个错误很容易纠正。PyTorch允许同时在多个进程上加载数据。<br />在这种情况下，PyTorch可以通过处理8个批次绕过GIL锁，每个批次在一个单独的进程上。应该使用多少workers？一个好的经验法则是:
```python
num_worker = 4 * num_GPU
```
:::warning
警告：缺点是内存使用也会增加
:::
<a name="fD0pp"></a>
## Pin memory
```python
DataLoader(dataset, pin_memory = True)
```
有时候GPU内存显示它是满的但很确定的是模型没有使用那么多？这种开销称为pinned memory。这个内存被保留为一种“working allocation”类型。<br />当在一个DataLoader中启用pinned_memory时，它“自动将获取的数据张量放在pinned memory中，并使数据更快地传输到CUDA-enabled的gpu”<br />![2021-05-07-20-59-03-257920.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620392638707-3d09c070-a274-4dc9-b675-195021c9255c.png#clientId=u1cdb06b7-427f-4&from=ui&id=u8e14c1c3&originHeight=455&originWidth=875&originalType=binary&size=1196834&status=done&style=shadow&taskId=u5ba7b595-e593-4f3c-9983-695baf14c8c)<br />这意味着不应该不必要的去调用：
```python
torch.cuda.empty_cache()
```
<a name="L6QuJ"></a>
## 避免CPU到GPU的传输，反之亦然
```python
# bad.cpu()
.item()
.numpy()
```
大量使用`.item()`或`.cpu()`或`.numpy()`调用。这对于性能来说是非常糟糕的，因为每个调用都将数据从GPU传输到CPU，从而极大地降低了性能。<br />如果试图清除附加的计算图，请使用`.detach()`。
```python
# good.detach()
```
这不会将内存转移到GPU，它会删除任何附加到该变量的计算图。
<a name="nRB9m"></a>
## 直接在GPUs上构建张量
大多数人都是这样在GPUs上创建张量的
```python
t = tensor.rand(2,2).cuda()
```
然而，这首先创建CPU张量，然后将其转移到GPU……这真的很慢。相反，直接在想要的设备上创建张量。
```python
t = tensor.rand(2,2, device=torch.device('cuda:0'))
```
如果正在使用Lightning，会自动把模型和批处理放到正确的GPU上。但是，如果在代码的某个地方创建了一个新的张量(例如：为一个VAE采样随机噪声，或类似的东西)，那么必须自己放置张量。
```python
t = tensor.rand(2,2, device=self.device)
```
每个LightningModule都有一个方便的`self.device`调用，无论是在CPU上，多 GPUs上，还是在TPUs上，lightning会为那个张量选择正确的设备。
<a name="EBezm"></a>
## 使用DistributedDataParallel不要使用DataParallel
PyTorch有两个主要的模式用于在多 GPUs训练。第一种是DataParallel，它将一批数据分割到多个GPUs上。但这也意味着模型必须复制到每个GPU上，一旦在GPU 0上计算出梯度，它们必须同步到其他GPU。<br />这需要大量昂贵的GPU传输！相反，DistributedDataParallel在每个GPU(在它自己的进程中)上创建模型副本，并且只让数据的一部分对该GPU可用。这就像是让N个独立的模型进行训练，除了一旦每个模型都计算出梯度，它们就会在模型之间同步梯度……这意味着<br />在每批处理中只在GPUs之间传输一次数据。<br />在Lightning中，可以在两者之间轻松切换
```python
Trainer(distributed_backend='ddp', gpus=8)
Trainer(distributed_backend='dp', gpus=8)
```
请注意，PyTorch和Lightning都不鼓励使用DP。
<a name="DeQf7"></a>
## 使用16-bit精度
这是另一种加快训练速度的方法，没有看到很多人使用这种方法。在模型进行16bit训练的部分，数据从32位变到到16位。这有几个优点:

1. 使用了一半的内存(这意味着可以将batch大小翻倍，并将训练时间减半)。
2. 某些GPU(V100, 2080Ti)可以自动加速(3 -8倍)，因为它们针对16位计算进行了优化。

在Lightning中，这很简单:
```python
Trainer(precision=16)
```
注意：在PyTorch 1.6之前，还必须安装Nvidia Apex，现在16位是PyTorch的原生版本。但如果使用的是Lightning，它同时支持这两种功能，并根据检测到的PyTorch版本自动切换。
<a name="V7mzg"></a>
## 对代码进行Profile
如果没有Lightning，最后一条建议可能很难实现，可以使用cprofiler这样的工具来实现。然而，在Lightning中，可以通过两种方式获得所有在训练期间所做的调用的总结：<br />首先，内置的basic profiler
```python
Trainer(profile=True)
```
可以给出这样的输出：<br />![2021-05-07-20-59-03-381689.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620392778921-0737f6a8-71ba-4140-96a4-d1f7ab17796f.png#clientId=u1cdb06b7-427f-4&from=ui&id=u89ab16d5&originHeight=464&originWidth=875&originalType=binary&size=1220509&status=done&style=none&taskId=ub81d8da8-ab11-4550-9c72-12fac217df5)<br />或者是高级的profiler:
```python
profiler = AdvancedProfiler()
trainer = Trainer(profiler=profiler)
```
得到更小粒度的结果：<br />![2021-05-07-20-59-03-572943.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620392809020-b71a9de6-ddb6-42d8-b6bf-cc134e746ae6.png#clientId=u1cdb06b7-427f-4&from=ui&id=ucdede820&originHeight=485&originWidth=875&originalType=binary&size=1275744&status=done&style=none&taskId=u2da667cd-48f4-45aa-bbb8-15f5d7bfa09)
