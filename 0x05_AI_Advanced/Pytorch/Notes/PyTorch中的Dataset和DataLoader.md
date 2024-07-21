PyTorch<br />PyTorch提供的两个Dataset和`DataLoader`类分别负责可被Pytorch使用的数据集的创建以及向训练传递数据的任务。如果想个性化自己的数据集或者数据传递方式，也可以自己重写子类。<br />**Dataset是**`**DataLoader**`**实例化的一个参数。**
<a name="U8Eoc"></a>
## 什么时候使用Dataset
CIFAR10是CV训练中经常使用到的一个数据集，在PyTorch中CIFAR10是一个写好的Dataset，使用时只需以下代码：
```python
data = datasets.CIFAR10("./data/", transform=transform, train=True, download=True)
```
datasets.CIFAR10就是一个Datasets子类，data是这个类的一个实例。<br />有的时候需要用自己在一个文件夹中的数据作为数据集，这个时候，可以使用`ImageFolder`这个方便的API。
```python
FaceDataset = datasets.ImageFolder('./data', transform=img_transform)
```
<a name="KZxHl"></a>
## 如何自定义一个数据集？
`**torch.utils.data.Dataset**` 是一个**表示数据集的抽象类**。任何自定义的数据集都需要**继承这个类并覆写相关方法**。<br />所谓**数据集**，其实就是一个**负责处理索引(index)到样本(sample)映射的一个类**(class)。<br />Pytorch提供两种数据集：Map式数据集；Iterable式数据集。
<a name="uFsTG"></a>
### Map式数据集
一个Map式的数据集必须要重写`**getitem(self, index),len(self)**`** 两个内建方法**，用来**表示从索引到样本的映射(Map)**。<br />这样一个数据集dataset，举个例子，当使用`dataset[idx]`命令时，可以在硬盘中读取数据集中第idx张图片以及其标签（如果有的话）；`len(dataset)`则会返回这个数据集的容量。<br />自定义类大致是这样的：
```python
class CustomDataset(data.Dataset):#需要继承data.Dataset
    def __init__(self):
        # TODO
        # 1. Initialize file path or list of file names.
        pass
    def __getitem__(self, index):
        # TODO
        # 1. Read one data from file (e.g. using numpy.fromfile, PIL.Image.open).
        # 2. Preprocess the data (e.g. torchvision.Transform).
        # 3. Return a data pair (e.g. image and label).
        #这里需要注意的是，第一步：read one data，是一个data
        pass
    def __len__(self):
        # You should change 0 to the total size of your dataset.
        return 0
```
例子-1：自己实验中写的一个例子：这里图片文件储存在“./data/faces/”文件夹下，图片的名字并不是从1开始，而是从final_train_tag_dict.txt这个文件保存的字典中读取，label信息也是用这个文件中读取。大家可以照着上面的注释阅读这段代码。
```python
from torch.utils import data
import numpy as np
from PIL import Image


class face_dataset(data.Dataset):
    def __init__(self):
        self.file_path = './data/faces/'
        f=open("final_train_tag_dict.txt","r")
        self.label_dict=eval(f.read())
        f.close()

    def __getitem__(self,index):
        label = list(self.label_dict.values())[index-1]
        img_id = list(self.label_dict.keys())[index-1]
        img_path = self.file_path+str(img_id)+".jpg"
        img = np.array(Image.open(img_path))
        return img,label

    def __len__(self):
        return len(self.label_dict)
```
下面看一下官方MNIST数据集的例子：
```python

class MNIST(data.Dataset):
    """`MNIST <http://yann.lecun.com/exdb/mnist/>`_ Dataset.
    Args:
        root (string): Root directory of dataset where ``processed/training.pt``
            and  ``processed/test.pt`` exist.
        train (bool, optional): If True, creates dataset from ``training.pt``,
            otherwise from ``test.pt``.
        download (bool, optional): If true, downloads the dataset from the internet and
            puts it in root directory. If dataset is already downloaded, it is not
            downloaded again.
        transform (callable, optional): A function/transform that  takes in an PIL image
            and returns a transformed version. E.g, ``transforms.RandomCrop``
        target_transform (callable, optional): A function/transform that takes in the
            target and transforms it.
    """
    urls = [
        'http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz',
        'http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz',
        'http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz',
        'http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz',
    ]
    raw_folder = 'raw'
    processed_folder = 'processed'
    training_file = 'training.pt'
    test_file = 'test.pt'
    classes = ['0 - zero', '1 - one', '2 - two', '3 - three', '4 - four',
               '5 - five', '6 - six', '7 - seven', '8 - eight', '9 - nine']
    class_to_idx = {_class: i for i, _class in enumerate(classes)}

    @property
    def targets(self):
        if self.train:
            return self.train_labels
        else:
            return self.test_labels

    def __init__(self, root, train=True, transform=None, target_transform=None, download=False):
        self.root = os.path.expanduser(root)
        self.transform = transform
        self.target_transform = target_transform
        self.train = train  # training set or test set

        if download:
            self.download()

        if not self._check_exists():
            raise RuntimeError('Dataset not found.' +
                               ' You can use download=True to download it')

        if self.train:
            self.train_data, self.train_labels = torch.load(
                os.path.join(self.root, self.processed_folder, self.training_file))
        else:
            self.test_data, self.test_labels = torch.load(
                os.path.join(self.root, self.processed_folder, self.test_file))

    def __getitem__(self, index):
        """
        Args:
            index (int): Index
        Returns:
            tuple: (image, target) where target is index of the target class.
        """
        if self.train:
            img, target = self.train_data[index], self.train_labels[index]
        else:
            img, target = self.test_data[index], self.test_labels[index]

        # doing this so that it is consistent with all other datasets
        # to return a PIL Image
        img = Image.fromarray(img.numpy(), mode='L')

        if self.transform is not None:
            img = self.transform(img)

        if self.target_transform is not None:
            target = self.target_transform(target)

        return img, target

    def __len__(self):
        if self.train:
            return len(self.train_data)
        else:
            return len(self.test_data)

    def _check_exists(self):
        return os.path.exists(os.path.join(self.root, self.processed_folder, self.training_file)) and \
            os.path.exists(os.path.join(self.root, self.processed_folder, self.test_file))

    def download(self):
        """Download the MNIST data if it doesn't exist in processed_folder already."""
        from six.moves import urllib
        import gzip

        if self._check_exists():
            return

        # download files
        try:
            os.makedirs(os.path.join(self.root, self.raw_folder))
            os.makedirs(os.path.join(self.root, self.processed_folder))
        except OSError as e:
            if e.errno == errno.EEXIST:
                pass
            else:
                raise

        for url in self.urls:
            print('Downloading ' + url)
            data = urllib.request.urlopen(url)
            filename = url.rpartition('/')[2]
            file_path = os.path.join(self.root, self.raw_folder, filename)
            with open(file_path, 'wb') as f:
                f.write(data.read())
            with open(file_path.replace('.gz', ''), 'wb') as out_f, \
                    gzip.GzipFile(file_path) as zip_f:
                out_f.write(zip_f.read())
            os.unlink(file_path)

        # process and save as torch files
        print('Processing...')

        training_set = (
            read_image_file(os.path.join(self.root, self.raw_folder, 'train-images-idx3-ubyte')),
            read_label_file(os.path.join(self.root, self.raw_folder, 'train-labels-idx1-ubyte'))
        )
        test_set = (
            read_image_file(os.path.join(self.root, self.raw_folder, 't10k-images-idx3-ubyte')),
            read_label_file(os.path.join(self.root, self.raw_folder, 't10k-labels-idx1-ubyte'))
        )
        with open(os.path.join(self.root, self.processed_folder, self.training_file), 'wb') as f:
            torch.save(training_set, f)
        with open(os.path.join(self.root, self.processed_folder, self.test_file), 'wb') as f:
            torch.save(test_set, f)

        print('Done!')

    def __repr__(self):
        fmt_str = 'Dataset ' + self.__class__.__name__ + '\n'
        fmt_str += '    Number of datapoints: {}\n'.format(self.__len__())
        tmp = 'train' if self.train is True else 'test'
        fmt_str += '    Split: {}\n'.format(tmp)
        fmt_str += '    Root Location: {}\n'.format(self.root)
        tmp = '    Transforms (if any): '
        fmt_str += '{0}{1}\n'.format(tmp, self.transform.__repr__().replace('\n', '\n' + ' ' * len(tmp)))
        tmp = '    Target Transforms (if any): '
        fmt_str += '{0}{1}'.format(tmp, self.target_transform.__repr__().replace('\n', '\n' + ' ' * len(tmp)))
        return fmt_str
```
<a name="KN9cy"></a>
### Iterable式数据集
一个Iterable（迭代）式数据集是**抽象类**`**data.IterableDataset**`**的子类**，并且**覆写了iter方法**成为一个迭代器。这种数据集主要**用于数据大小未知**，或者以流的形式的输入，本地文件不固定的情况，需要**以迭代的方式来获取样本索引**。
<a name="ea9Pj"></a>
## DataLoader
> Data loader. Combines a dataset and a sampler, and provides an iterable over the given dataset. --PyTorch Documents

一般来说PyTorch中深度学习训练的**流程**是这样的：

1. **创建Dateset** 
2. Dataset**传递给DataLoader **
3. DataLoader**迭代产生训练数据提供给模型**

对应的一般都会有这三部分代码：
```python
# 创建Dateset(可以自定义)
    dataset = face_dataset # Dataset部分自定义过的face_dataset
# Dataset传递给DataLoader
    dataloader = torch.utils.data.DataLoader(dataset,batch_size=64,shuffle=False,num_workers=8)
# DataLoader迭代产生训练数据提供给模型
    for i in range(epoch):
        for index,(img,label) in enumerate(dataloader):
            pass
```
到这里应该就PyTorch的数据集和数据传递机制应该就比较清晰明了了。**Dataset负责建立索引到样本的映射**，`**DataLoader**`**负责以特定的方式从数据集中迭代的产生 一个个batch的样本集合**。在enumerate过程中实际上是`dataloader`按照其参数sampler规定的策略调用了其dataset的getitem方法。
<a name="ftGWi"></a>
## 参数介绍
先看一下实例化一个`DataLoader`所需的参数，只关注几个重点即可。
```python
DataLoader(dataset, batch_size=1, shuffle=False, sampler=None,
           batch_sampler=None, num_workers=0, collate_fn=None,
           pin_memory=False, drop_last=False, timeout=0,
           worker_init_fn=None)
```
参数介绍： 

- `dataset` (Dataset) – 定义好的Map式或者Iterable式数据集。 
- `batch_size` (python:int, optional) – 一个batch含有多少样本 (default: 1)。 
- `shuffle` (bool, optional) – 每一个epoch的batch样本是相同还是随机 (default: False)。 
- `sampler` (Sampler, optional) – 决定数据集中采样的方法. 如果有，则shuffle参数必须为False。
- `batch_sampler` (Sampler, optional) – 和 sampler 类似，但是一次返回的是一个batch内所有样本的index。和 batch_size, shuffle, sampler, and drop_last 三个参数互斥。 
- `num_workers` (python:int, optional) – 多少个子程序同时工作来获取数据，多线程。(default: 0) 
- `collate_fn` (callable, optional) – 合并样本列表以形成小批量。 
- `pin_memory` (bool, optional) – 如果为True，数据加载器在返回前将张量复制到CUDA固定内存中。 
- `drop_last` (bool, optional) – 如果数据集大小不能被batch_size整除，设置为True可删除最后一个不完整的批处理。如果设为False并且数据集的大小不能被batch_size整除，则最后一个batch将更小。(default: False) 
- `timeout` (numeric, optional) – 如果是正数，表明等待从worker进程中收集一个batch等待的时间，若超出设定的时间还没有收集到，那就不收集这个内容了。这个numeric应总是大于等于0。(default: 0) 
- `worker_init_fn` (callable, optional*) – 每个worker初始化函数 (default: None)

dataset 没什么好说的，很重要，需要按照前面所说的两种dataset定义好，完成相关函数的重写。<br />batch_size 也没啥好说的，就是训练的一个批次的样本数。<br />shuffle 表示每一个epoch中训练样本的顺序是否相同，一般True。
<a name="e0DBY"></a>
### 采样器
**sampler** 重点参数，采样器，是一个**迭代器**。PyTorch提供了多种采样器，用户也可以自定义采样器。<br />所有sampler都是继承 `torch.utils.data.sampler.Sampler`这个抽象类。
```python
class Sampler(object):
    # """Base class for all Samplers.
    # Every Sampler subclass has to provide an __iter__ method, providing a way
    # to iterate over indices of dataset elements, and a __len__ method that
    # returns the length of the returned iterators.
    # """
    # 一个 迭代器 基类
    def __init__(self, data_source):
        pass

    def __iter__(self):
        raise NotImplementedError

    def __len__(self):
        raise NotImplementedError
```
<a name="IE4z9"></a>
### PyTorch自带的Sampler

- `SequentialSampler`
- `RandomSampler`
- `SubsetRandomSampler`
- `WeightedRandomSampler`

`**SequentialSampler**` 很好理解就是**顺序采样器**。<br />其原理是首先在初始化的时候拿到数据集`data_source`，之后在`__iter__`方法中首先得到一个和`data_source`一样长度的`range`可迭代器。**每次只会返回一个索引值**。
```python
class SequentialSampler(Sampler):
    # r"""Samples elements sequentially, always in the same order.
    # Arguments:
    #     data_source (Dataset): dataset to sample from
    # """
   # 产生顺序 迭代器
    def __init__(self, data_source):
        self.data_source = data_source

    def __iter__(self):
        return iter(range(len(self.data_source)))

    def __len__(self):
        return len(self.data_source)
```
参数作用：

- `data_source`：同上
- `num_samples`：指定采样的数量，默认是所有。
- `replacement`：若为True，则表示可以重复采样，即同一个样本可以重复采样，这样可能导致有的样本采样不到。所以此时可以设置`num_samples`来增加采样数量使得每个样本都可能被采样到。

**RandomSampler**
```python
class RandomSampler(Sampler):
    # r"""Samples elements randomly. If without replacement, then sample from a shuffled dataset.
    # If with replacement, then user can specify ``num_samples`` to draw.
    # Arguments:
    #     data_source (Dataset): dataset to sample from
    #     num_samples (int): number of samples to draw, default=len(dataset)
    #     replacement (bool): samples are drawn with replacement if ``True``, default=False
    # """

    def __init__(self, data_source, replacement=False, num_samples=None):
        self.data_source = data_source
        self.replacement = replacement
        self.num_samples = num_samples

        if self.num_samples is not None and replacement is False:
            raise ValueError("With replacement=False, num_samples should not be specified, "
                             "since a random permute will be performed.")

        if self.num_samples is None:
            self.num_samples = len(self.data_source)

        if not isinstance(self.num_samples, int) or self.num_samples <= 0:
            raise ValueError("num_samples should be a positive integeral "
                             "value, but got num_samples={}".format(self.num_samples))
        if not isinstance(self.replacement, bool):
            raise ValueError("replacement should be a boolean value, but got "
                             "replacement={}".format(self.replacement))

    def __iter__(self):
        n = len(self.data_source)
        if self.replacement:
            return iter(torch.randint(high=n, size=(self.num_samples,), dtype=torch.int64).tolist())
        return iter(torch.randperm(n).tolist())

    def __len__(self):
        return len(self.data_source)
```
这个采样器常见的使用场景是**将训练集划分成训练集和验证集**：
```python
class SubsetRandomSampler(Sampler):
    # r"""Samples elements randomly from a given list of indices, without replacement.
    # Arguments:
    #     indices (sequence): a sequence of indices
    # """

    def __init__(self, indices):
        self.indices = indices

    def __iter__(self):
        return (self.indices[i] for i in torch.randperm(len(self.indices)))

    def __len__(self):
        return len(self.indices)
```
**batch_sampler**<br />前面的采样器每次都只返回一个索引，但是在训练时是**对批量的数据进行训练**，而这个工作就需要`BatchSampler`来做。也就是说BatchSampler的作用就是将前面的Sampler采样得到的索引值进行合并，当数量等于一个batch大小后就将这一批的索引值返回。
```python
class BatchSampler(Sampler):
    #     Wraps another sampler to yield a mini-batch of indices.
    # Args:
    #     sampler (Sampler): Base sampler.
    #     batch_size (int): Size of mini-batch.
    #     drop_last (bool): If ``True``, the sampler will drop the last batch if
    #         its size would be less than ``batch_size``
    # Example:
    #     >>> list(BatchSampler(SequentialSampler(range(10)), batch_size=3, drop_last=False))
    #     [[0, 1, 2], [3, 4, 5], [6, 7, 8], [9]]
    #     >>> list(BatchSampler(SequentialSampler(range(10)), batch_size=3, drop_last=True))
    #     [[0, 1, 2], [3, 4, 5], [6, 7, 8]]

# 批次采样
    def __init__(self, sampler, batch_size, drop_last):
        if not isinstance(sampler, Sampler):
            raise ValueError("sampler should be an instance of "
                             "torch.utils.data.Sampler, but got sampler={}"
                             .format(sampler))
        if not isinstance(batch_size, _int_classes) or isinstance(batch_size, bool) or \
                batch_size <= 0:
            raise ValueError("batch_size should be a positive integeral value, "
                             "but got batch_size={}".format(batch_size))
        if not isinstance(drop_last, bool):
            raise ValueError("drop_last should be a boolean value, but got "
                             "drop_last={}".format(drop_last))
        self.sampler = sampler
        self.batch_size = batch_size
        self.drop_last = drop_last

    def __iter__(self):
        batch = []
        for idx in self.sampler:
            batch.append(idx)
            if len(batch) == self.batch_size:
                yield batch
                batch = []
        if len(batch) > 0 and not self.drop_last:
            yield batch

    def __len__(self):
        if self.drop_last:
            return len(self.sampler) // self.batch_size
        else:
            return (len(self.sampler) + self.batch_size - 1) // self.batch_size
```
<a name="OAnEE"></a>
### 多线程
`num_workers` 参数表示同时参与数据读取的线程数量，多线程技术可以加快数据读取，提供GPU/CPU利用率。
