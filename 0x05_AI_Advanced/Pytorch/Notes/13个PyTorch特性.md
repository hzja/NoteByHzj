PyTorch<br />PyTorch在学术界和工业界的应用研究中都获得了很多关注。它是一个具有很大灵活性的深度学习框架，使用了大量的实用工具和函数来加快工作速度。
<a name="HJHNB"></a>
## 1、DatasetFolder
当学习PyTorch时，人们首先要做的事情之一是实现自己的某种`Dataset` 。这是一个低级错误，没有必要浪费时间写这样的东西。通常，数据集要么是数据列表(或者是numpy数组)，要么磁盘上的文件。所以，把数据在磁盘上组织好，要比写一个自定义的`Dataset`来加载某种奇怪的格式更好。<br />分类器最常见的数据格式之一，是有一个带有子文件夹的目录，子文件夹表示类，子文件夹中的文件表示样本，如下所示。
```
folder/class_0/file1.txt
folder/class_0/file2.txt
folder/class_0/...
folder/class_1/file3.txt
folder/class_1/file4.txt
folder/class_2/file5.txt
folder/class_2/...
```
有一个内置的方式来加载这类数据集，不管数据是图像，文本文件或其他什么，只要使用'`DatasetFolder`就可以了。令人惊讶的是，这个类是`torchvision`包的一部分，而不是核心PyTorch。这个类非常全面，可以从文件夹中过滤文件，使用自定义代码加载它们，并动态转换原始文件。例子:
```python
from torchvision.datasets import DatasetFolder
from pathlib import Path
# I have text files in this folder
ds = DatasetFolder("/Users/marcin/Dev/tmp/my_text_dataset", 
    loader=lambda path: Path(path).read_text(),
    extensions=(".txt",), #only load .txt files
    transform=lambda text: text[:100], # only take first 100 characters
)
# Everything you need is already there
len(ds), ds.classes, ds.class_to_idx
(20, ['novels', 'thrillers'], {'novels': 0, 'thrillers': 1})
```
如果在处理图像，还有一个`torchvision.datasets.ImageFolder`类，它基于`DatasetLoader`，它被预先配置为加载图像。
<a name="UeuvW"></a>
## 2、尽量少用 `.to(device)` ，用 `zeros_like` / `ones_like` 之类的代替
很多来自GitHub仓库的PyTorch代码。几乎在每个repo中都有许多`*.to(device)`行，它们将数据从CPU或GPU转移到其他地方。这样的语句通常会出现在大量的repos或初学者教程中。强烈建议尽可能少地实现这类操作，并依赖内置的PyTorch功能自动实现这类操作。到处使用`.to(device)`通常会导致性能下降，还会出现异常：
```
Expected object of device type cuda but got device type cpu
```
显然，有些情况下无法回避它，但大多数情况(如果不是全部)都在这里。其中一种情况是初始化一个全0或全1的张量，这在深度神经网络计算损失的的时候是经常发生的，模型的输出已经在cuda上了，需要另外的tensor也是在cuda上，这时，可以使用`*_like`操作符：
```python
my_output # on any device, if it's cuda then my_zeros will also be on cuda
my_zeros = torch.zeros_like(my_output_from_model)
```
在内部，PyTorch所做的是调用以下操作：
```python
my_zeros = torch.zeros(my_output.size(), dtype=my_output.dtype, layout=my_output.layout, device=my_output.device)
```
所以所有的设置都是正确的，这样就减少了代码中出现错误的概率。类似的操作包括：
```python
torch.zeros_like()
torch.ones_like()
torch.rand_like()
torch.randn_like()
torch.randint_like()
torch.empty_like()
torch.full_like()
```
<a name="tENBY"></a>
## 3、Register Buffer (`nn.Module.register_buffer`)
这将是劝开发者不要到处使用 `.to(device)` 的下一步。有时，模型或损失函数需要有预先设置的参数，并在调用`forward`时使用，例如，它可以是一个“权重”参数，它可以缩放损失或一些固定张量，它不会改变，但每次都使用。对于这种情况，请使用`nn.Module.register_buffer` 方法，它告诉PyTorch将传递给它的值存储在模块中，并将这些值随模块一起移动。如果初始化模块，然后将它移动到GPU，这些值也会自动移动。此外，如果保存模块的状态，buffers也会被保存！<br />一旦注册，这些值就可以在forward函数中访问，就像其他模块的属性一样。
```python
from torch import nn
import torch
class ModuleWithCustomValues(nn.Module):
    def __init__(self, weights, alpha):
        super().__init__()
        self.register_buffer("weights", torch.tensor(weights))
        self.register_buffer("alpha", torch.tensor(alpha))
    
    def forward(self, x):
        return x * self.weights + self.alpha
m = ModuleWithCustomValues(
    weights=[1.0, 2.0], alpha=1e-4
)
m(torch.tensor([1.23, 4.56]))
tensor([1.2301, 9.1201])
```
<a name="2d5At"></a>
## 4、Built-in `Identity()`
有时候，使用迁移学习时，需要用1:1的映射替换一些层，可以用`nn.Module`来实现这个目的，只返回输入值。PyTorch内置了这个类。<br />例子，想要在分类层之前从一个预训练过的`ResNet50`获取图像表示。以下是如何做到这一点：
```python
from torchvision.models import resnet50
model = resnet50(pretrained=True)
model.fc = nn.Identity()
last_layer_output = model(torch.rand((1, 3, 224, 224)))
last_layer_output.shape
torch.Size([1, 2048])
```
<a name="CACoP"></a>
## 5、Pairwise distances: `torch.cdist`
下次遇到计算两个张量之间的欧几里得距离(或者一般来说:p范数)的问题时，请记住`torch.cdist`。它确实做到了这一点，并且在使用欧几里得距离时还自动使用矩阵乘法，从而提高了性能。
```python
points1 = torch.tensor([[0.0, 0.0], [1.0, 1.0], [2.0, 2.0]])
points2 = torch.tensor([[0.0, 0.0], [-1.0, -1.0], [-2.0, -2.0], [-3.0, -3.0]]) # batches don't have to be equal
torch.cdist(points1, points2, p=2.0)
tensor([[0.0000, 1.4142, 2.8284, 4.2426],
        [1.4142, 2.8284, 4.2426, 5.6569],
        [2.8284, 4.2426, 5.6569, 7.0711]])
```
没有矩阵乘法或有矩阵乘法的性能，在机器上使用mm时，速度快了2倍以上。
```python
%%timeit
points1 = torch.rand((512, 2))
points2 = torch.rand((512, 2))
torch.cdist(points1, points2, p=2.0, compute_mode="donot_use_mm_for_euclid_dist")
```
_867µs±142µs per loop (mean±std. dev. of 7 run, 1000 loop each)_
```python
%%timeit
points1 = torch.rand((512, 2))
points2 = torch.rand((512, 2))
torch.cdist(points1, points2, p=2.0)
```
_417µs±52.9µs per loop (mean±std. dev. of 7 run, 1000 loop each)_
<a name="KC9fM"></a>
## 6、Cosine similarity: `F.cosine_similarity`
与上一点相同，计算欧几里得距离并不总是需要的东西。当处理向量时，通常余弦相似度是选择的度量。PyTorch也有一个内置的余弦相似度实现。
```python
import torch.nn.functional as F
vector1 = torch.tensor([0.0, 1.0])
vector2 = torch.tensor([0.05, 1.0])
print(F.cosine_similarity(vector1, vector2, dim=0))
vector3 = torch.tensor([0.0, -1.0])
print(F.cosine_similarity(vector1, vector3, dim=0))
tensor(0.9988)
tensor(-1.)
```

---

<a name="h8fIW"></a>
### PyTorch中批量计算余弦距离
```python
import torch.nn.functional as F
batch_of_vectors = torch.rand((4, 64))
similarity_matrix = F.cosine_similarity(batch_of_vectors.unsqueeze(1), batch_of_vectors.unsqueeze(0), dim=2)
similarity_matrix
tensor([[1.0000, 0.6922, 0.6480, 0.6789],
        [0.6922, 1.0000, 0.7143, 0.7172],
        [0.6480, 0.7143, 1.0000, 0.7312],
        [0.6789, 0.7172, 0.7312, 1.0000]])
```
<a name="352e2"></a>
## 7、归一化向量: `F.normalize`
最后一点仍然与向量和距离有松散的联系，那就是归一化：通常是通过改变向量的大小来提高计算的稳定性。最常用的归一化是L2，可以在PyTorch中按如下方式应用:
```python
vector = torch.tensor([99.0, -512.0, 123.0, 0.1, 6.66])
normalized_vector = F.normalize(vector, p=2.0, dim=0)
normalized_vector
tensor([ 1.8476e-01, -9.5552e-01,  2.2955e-01,  1.8662e-04,  1.2429e-02])
```
在PyTorch中执行归一化的旧方法是：
```python
vector = torch.tensor([99.0, -512.0, 123.0, 0.1, 6.66])
normalized_vector = vector / torch.norm(vector, p=2.0)
normalized_vector
tensor([ 1.8476e-01, -9.5552e-01,  2.2955e-01,  1.8662e-04,  1.2429e-02])
```
在PyTorch中批量进行L2归一化
```python
batch_of_vectors = torch.rand((4, 64))
normalized_batch_of_vectors = F.normalize(batch_of_vectors, p=2.0, dim=1)
normalized_batch_of_vectors.shape, torch.norm(normalized_batch_of_vectors, dim=1) # all vectors will have length of 1.0
(torch.Size([4, 64]), tensor([1.0000, 1.0000, 1.0000, 1.0000]))
```
<a name="NxXev"></a>
## 8、线性层 + 分块技巧 (`torch.chunk`)
假设把输入映射到N个不同的线性投影中。可以通过创建**N**个`nn.Linear`来做到这一点。或者也可以创建一个单一的线性层，做一个向前传递，然后将输出分成**N**块。这种方法通常会带来更高的性能，所以这是一个值得记住的技巧。
```python
d = 1024
batch = torch.rand((8, d))
layers = nn.Linear(d, 128, bias=False), nn.Linear(d, 128, bias=False), nn.Linear(d, 128, bias=False)
one_layer = nn.Linear(d, 128 * 3, bias=False)
%%timeit
o1 = layers[0](batch)
o2 = layers[1](batch)
o3 = layers[2](batch)
```
_289 µs ± 30.8 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)_
```python
%%timeit
o1, o2, o3 = torch.chunk(one_layer(batch), 3, dim=1)
```
_202 µs ± 8.09 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)_
<a name="jDjV9"></a>
## 9、Masked select (`torch.masked_select`)
有时只需要对输入张量的一部分进行计算。给一个例子：想计算的损失只在满足某些条件的张量上。为了做到这一点，可以使用`torch.masked_select`，注意，当需要梯度时也可以使用这个操作。
```python
data = torch.rand((3, 3)).requires_grad_()
print(data)
mask = data > data.mean()
print(mask)
torch.masked_select(data, mask)
tensor([[0.0582, 0.7170, 0.7713],
        [0.9458, 0.2597, 0.6711],
        [0.2828, 0.2232, 0.1981]], requires_grad=True)
tensor([[False,  True,  True],
        [ True, False,  True],
        [False, False, False]])
tensor([0.7170, 0.7713, 0.9458, 0.6711], grad_fn=<MaskedSelectBackward>)
```
<a name="cAC4l"></a>
### 直接在tensor上应用mask
类似的行为可以通过使用mask作为输入张量的 “indexer”来实现。
```python
data[mask]
tensor([0.7170, 0.7713, 0.9458, 0.6711], grad_fn=<IndexBackward>)
```
有时，一个理想的解决方案是用0填充mask中所有的`False`值，可以这样做:
```python
data * mask
tensor([[0.0000, 0.7170, 0.7713],
        [0.9458, 0.0000, 0.6711],
        [0.0000, 0.0000, 0.0000]], grad_fn=<MulBackward0>)
```
<a name="wKpXq"></a>
## 10、使用 `torch.where`来对tensors加条件
把两个张量结合在一个条件下这个函数很有用，如果条件是真，那么从第一个张量中取元素，如果条件是假，从第二个张量中取元素。
```python
x = torch.tensor([1.0, 2.0, 3.0, 4.0, 5.0], requires_grad=True)
y = -x
condition_or_mask = x <= 3.0
torch.where(condition_or_mask, x, y)
tensor([ 1.,  2.,  3., -4., -5.], grad_fn=<SWhereBackward>)
```
<a name="6Qx05"></a>
## 11、在给定的位置给张量填入值(`Tensor.scatter`)
这个函数的用例如下，想用给定位置下另一个张量的值填充一个张量。一维张量更容易理解，所以将先展示它，然后继续更高级的例子。
```python
data = torch.tensor([1, 2, 3, 4, 5])
index = torch.tensor([0, 1])
values = torch.tensor([-1, -2, -3, -4, -5])
data.scatter(0, index, values)
tensor([-1, -2,  3,  4,  5])
```
上面的例子很简单，但是现在看看如果将index改为`index = torch.tensor([0, 1, 4])`会发生什么：
```javascript
data = torch.tensor([1, 2, 3, 4, 5])
index = torch.tensor([0, 1, 4])
values = torch.tensor([-1, -2, -3, -4, -5])
data.scatter(0, index, values)
tensor([-1, -2,  3,  4, -3])
```
为什么最后一个值是-3，这是反直觉的，对吧？这是PyTorch `scatter`函数的中心思想。`index`变量表示`data`张量的第i个值应该放在`values`张量的哪个位置。希望下面的简单python版的这个操作能让你更明白：
```javascript
data_orig = torch.tensor([1, 2, 3, 4, 5])
index = torch.tensor([0, 1, 4])
values = torch.tensor([-1, -2, -3, -4, -5])
scattered = data_orig.scatter(0, index, values)
data = data_orig.clone()
for idx_in_values, where_to_put_the_value in enumerate(index):
    what_value_to_put = values[idx_in_values]
    data[where_to_put_the_value] = what_value_to_put
data, scattered
(tensor([-1, -2,  3,  4, -3]), tensor([-1, -2,  3,  4, -3]))
```

---

<a name="y9Ex5"></a>
### 2D数据的PyTorch scatter例子
始终记住，`index`的形状与`values`的形状相关，而`index`中的值对应于`data`中的位置。
```javascript
data = torch.zeros((4, 4)).float()
index = torch.tensor([
    [0, 1],
    [2, 3],
    [0, 3],
    [1, 2]
])
values = torch.arange(1, 9).float().view(4, 2)
values, data.scatter(1, index, values)
(tensor([[1., 2.],
        [3., 4.],
        [5., 6.],
        [7., 8.]]),
tensor([[1., 2., 0., 0.],
        [0., 0., 3., 4.],
        [5., 0., 0., 6.],
        [0., 7., 8., 0.]]))
```
<a name="15WnC"></a>
## 12、在网络中进行图像插值 (`F.interpolate`)
实际上可以在前向传递中调整图像(**或任何中间张量**)，并保持梯度流。这种方法在使用CNN和GANs时特别有用。
```javascript
# image from https://commons.wikimedia.org/wiki/File:A_female_British_Shorthair_at_the_age_of_20_months.jpg
img = Image.open("./cat.jpg")
img
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618389161765-f5911a0f-9f67-4a77-9178-ef1f3783823f.png#align=left&display=inline&height=107&originHeight=107&originWidth=128&size=0&status=done&style=none&width=128)
```javascript
to_pil_image(
    F.interpolate(to_tensor(img).unsqueeze(0),  # batch of size 1
                  mode="bilinear", 
                  scale_factor=2.0, 
                  align_corners=False).squeeze(0) # remove batch dimension
)
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1618389161887-4e6cc07e-65c0-41a9-8faa-d47b4807712c.webp#align=left&display=inline&height=214&originHeight=214&originWidth=256&size=0&status=done&style=none&width=256)<br />看看梯度流是如何保存的：
```javascript
F.interpolate(to_tensor(img).unsqueeze(0).requires_grad_(),
                  mode="bicubic", 
                  scale_factor=2.0, 
                  align_corners=False)
tensor([[[[0.9216, 0.9216, 0.9216,  ..., 0.8361, 0.8272, 0.8219],
    [0.9214, 0.9214, 0.9214,  ..., 0.8361, 0.8272, 0.8219],
    [0.9212, 0.9212, 0.9212,  ..., 0.8361, 0.8272, 0.8219],
    ...,
    [0.9098, 0.9098, 0.9098,  ..., 0.3592, 0.3486, 0.3421],
    [0.9098, 0.9098, 0.9098,  ..., 0.3566, 0.3463, 0.3400],
    [0.9098, 0.9098, 0.9098,  ..., 0.3550, 0.3449, 0.3387]],
    [[0.6627, 0.6627, 0.6627,  ..., 0.5380, 0.5292, 0.5238],
    [0.6626, 0.6626, 0.6626,  ..., 0.5380, 0.5292, 0.5238],
    [0.6623, 0.6623, 0.6623,  ..., 0.5380, 0.5292, 0.5238],
    ...,
    [0.6196, 0.6196, 0.6196,  ..., 0.3631, 0.3525, 0.3461],
    [0.6196, 0.6196, 0.6196,  ..., 0.3605, 0.3502, 0.3439],
    [0.6196, 0.6196, 0.6196,  ..., 0.3589, 0.3488, 0.3426]],
    [[0.4353, 0.4353, 0.4353,  ..., 0.1913, 0.1835, 0.1787],
    [0.4352, 0.4352, 0.4352,  ..., 0.1913, 0.1835, 0.1787],
    [0.4349, 0.4349, 0.4349,  ..., 0.1913, 0.1835, 0.1787],
    ...,
    [0.3333, 0.3333, 0.3333,  ..., 0.3827, 0.3721, 0.3657],
    [0.3333, 0.3333, 0.3333,  ..., 0.3801, 0.3698, 0.3635],
    [0.3333, 0.3333, 0.3333,  ..., 0.3785, 0.3684, 0.3622]]]],
grad_fn=<UpsampleBicubic2DBackward1>)
```
<a name="CWcKC"></a>
## 13、将图像做成网格 (`torchvision.utils.make_grid`)
当使用PyTorch和torchvision时，不需要使用matplotlib或一些外部库来复制粘贴代码来显示图像网格。只要使用`torchvision.utils.make_grid`就行了。
```json
from torchvision.utils import make_grid
from torchvision.transforms.functional import to_tensor, to_pil_image
from PIL import Image
img = Image.open("./cat.jpg")
to_pil_image(
    make_grid(
        [to_tensor(i) for i in [img, img, img]],
         nrow=2, # number of images in single row
         padding=5 # "frame" size
     )
)
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1618389161794-e54a3399-0361-410f-9aa2-2ad2c8bf26f0.webp#align=left&display=inline&height=229&originHeight=229&originWidth=271&size=0&status=done&style=none&width=271)
