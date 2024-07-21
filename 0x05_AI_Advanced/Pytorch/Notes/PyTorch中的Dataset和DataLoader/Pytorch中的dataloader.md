Pytorch<br />起初，最开始单独训练一个网络来完成landmark点回归任务和分类任务，训练的数据是txt格式，在训练之前对数据进行分析，发现分类任务中存在严重的数据样本不均衡的问题，那么事先针对性的进行数据采样均衡操作，重新得到训练和测试的txt数据和标签，保证了整个训练和测试数据的样本均衡性。由于整个项目是检测+点回归+分类，起初检测和点回归+分类是分两步实现的，检测是通过读取XML格式来进行训练，现在要统一整个项目的训练和测试过程，要将点回归+分类的训练测试过程也按照读取XML格式来进行，那么就遇到一个问题，如何针对性的去给样本偏少的样本进行均衡，由于在dataset类中，返回的图像和标签都是针对每个index返回一个结果，在dataset类中进行操作似乎不太可行，那么就想到在dataloader中进行操作，通过dataloader中的参数sample来完成针对性采样。<br />还有一个问题是关于num_workers的设置，因为有对比过，在单机RTX 2080Ti上和八卡服务器TITAN RTX上(仅使用单卡，其它卡有在跑其它任务)，使用相同的`num_workers`，在单机上的训练速度反而更快，于是猜想可能和CPU或者内存有关系，下面会具体分析。<br />首先来看下下dataloader中的各个参数的含义。<br />类的定义为：`torch.utils.data.DataLoader` ，其中包含的参数有：
```python
torch.utils.data.DataLoader(dataset, batch_size=1, shuffle=False, sampler=None, \
    batch_sampler=None, num_workers=0, collate_fn=None, pin_memory=False, \
    drop_last=False, timeout=0, worker_init_fn=None, multiprocessing_context=None)
```
`dataset`：定义的dataset类返回的结果。<br />`batchsize`：每个bacth要加载的样本数，默认为1。<br />`shuffle`：在每个epoch中对整个数据集data进行shuffle重排，默认为False。<br />`sample`：定义从数据集中加载数据所采用的策略，如果指定的话，shuffle必须为False；batch_sample类似，表示一次返回一个batch的index。<br />`num_workers`：表示开启多少个线程数去加载数据，默认为0，代表只使用主进程。<br />`collate_fn`：表示合并样本列表以形成小批量的Tensor对象。<br />`pin_memory`：表示要将load进来的数据是否要拷贝到pin_memory区中，其表示生成的Tensor数据是属于内存中的锁页内存区，这样将Tensor数据转义到GPU中速度就会快一些，默认为False。<br />drop_last：当整个数据长度不能够整除batchsize，选择是否要丢弃最后一个不完整的batch，默认为False。<br />注：这里简单科普下pin_memory，通常情况下，数据在内存中要么以锁页的方式存在，要么保存在虚拟内存(磁盘)中，设置为True后，数据直接保存在锁页内存中，后续直接传入cuda；否则需要先从虚拟内存中传入锁页内存中，再传入cuda，这样就比较耗时了，但是对于内存的大小要求比较高。<br />下面针对`num_workers`，`sample`和`collate_fn`分别进行说明：
<a name="ZnOCh"></a>
## 1、设置`num_workers`：
Pytorch中`dataloader`一次性创建`num_workers`个子线程，然后用`batch_sampler`将指定batch分配给指定worker，worker将它负责的batch加载进RAM，dataloader就可以直接从RAM中找本轮迭代要用的batch。如果`num_worker`设置得大，好处是寻batch速度快，因为下一轮迭代的batch很可能在上一轮/上上一轮...迭代时已经加载好了。坏处是内存开销大，也加重了CPU负担（worker加载数据到RAM的进程是进行CPU复制）。如果num_worker设为0，意味着每一轮迭代时，dataloader不再有自主加载数据到RAM这一步骤，只有当需要的时候再加载相应的batch，当然速度就更慢。`num_workers`的经验设置值是自己电脑/服务器的CPU核心数，如果CPU很强、RAM也很充足，就可以设置得更大些，对于单机来说，单跑一个任务的话，直接设置为CPU的核心数最好。
<a name="Dvdqe"></a>
## 2、定义sample：（假设dataset类返回的是：data, label）
```python
from torch.utils.data.sampler import WeightedRandomSampler
## 如果label为1，那么对应的该类别被取出来的概率是另外一个类别的2倍
weights = [2 if label == 1 else 1 for data, label in dataset]
sampler = WeightedRandomSampler(weights,num_samples=10, replacement=True)
dataloader = DataLoader(dataset, batch_size=16, sampler=sampler)
```
PyTorch中提供的这个`sampler`模块，用来对数据进行采样。默认采用`SequentialSampler`，它会按顺序一个一个进行采样。常用的有随机采样器：`RandomSampler`，当`dataloader`的`shuffle`参数为True时，系统会自动调用这个采样器，实现打乱数据。这里使用另外一个很有用的采样方法：`WeightedRandomSampler`，它会根据每个样本的权重选取数据，在样本比例不均衡的问题中，可用它来进行重采样。`replacement`用于指定是否可以重复选取某一个样本，默认为`True`，即允许在一个`epoch`中重复采样某一个数据。
<a name="bDaBg"></a>
## 3、定义`collate_fn`：
```python
def detection_collate(batch):
    """Custom collate fn for dealing with batches of images that have a different
    number of associated object annotations (bounding boxes).

    Arguments:
        batch: (tuple) A tuple of tensor images and lists of annotations

    Return:
        A tuple containing:
            1) (tensor) batch of images stacked on their 0 dim
            2) (list of tensors) annotations for a given image are stacked on
                                 0 dim
    """
    targets = []
    imgs = []
    for sample in batch:
        imgs.append(sample[0])
        targets.append(torch.FloatTensor(sample[1]))
    return torch.stack(imgs, 0), targets
```
使用`dataloader`时加入`collate_fn`参数，即可合并样本列表以形成小批量的Tensor对象，如果标签不止一个的话，还可以支持自定义，在上述方法中再额外添加对应的label即可。
```python
data_loader = torch.utils.data.DataLoader(dataset, args.batch_size,
    num_workers=args.num_workers, sampler=sampler, shuffle=False, 
    collate_fn=detection_collate, pin_memory=True, drop_last=True)
```
参考链接：<br />torch.utils.data - PyTorch master documentationpytorch.org（[https://pytorch.org/docs/stable/data.html?highlight=dataloader#torch.utils.data.DataLoader](https://pytorch.org/docs/stable/data.html?highlight=dataloader#torch.utils.data.DataLoader)）<br />Guidelines for assigning num_workers to DataLoaderdiscuss.pytorch.org（[https://discuss.pytorch.org/t/guidelines-for-assigning-num-workers-to-dataloader/813](https://discuss.pytorch.org/t/guidelines-for-assigning-num-workers-to-dataloader/813)）
