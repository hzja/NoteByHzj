PyTorch<br />主要是围绕 PyTorch 中的 tensor 展开的，讨论了张量的求导机制，在不同设备之间的转换，神经网络中权重的更新等内容。文中的代码例子基于 Python 3 和 PyTorch 1.1。
<a name="Dr2qn"></a>
## 1、requires_grad

1. 当**创建一个张量** (tensor) 的时候，如果没有特殊指定的话，那么这个张量是**默认是不需要求导**的。可以通过 **tensor.requires_grad** 来**检查一个张量是否需要求导**。

在张量间的计算过程中，如果**在所有输入中**，**有一个输入需要求导**，那么**输出一定会需要求导**；相反，**只有当所有输入都不需要求导**的时候，**输出才会不需要**。<br />举一个比较简单的例子，比如在训练一个网络的时候，从 DataLoader 中读取出来的一个 **mini-batch 的数据**，这些输入**默认是不需要求导**的，其次，网络的输出没有特意指明需要求导，Ground Truth 也没有特意设置需要求导。这么一想，那之前的那些** loss 怎么还能自动求导**？<br />其实原因就是上边那条规则，虽然**输入的训练数据是默认不求导**的，但是， **model 中的所有参数**，它**默认是求导**的，这么一来，其中**只要有一个需要求导**，那么**输出的网络结果必定也会需要求的**。来看个实例：
```python
input = torch.randn(8, 3, 50, 100)
print(input.requires_grad)
# False

net = nn.Sequential(nn.Conv2d(3, 16, 3, 1),
                    nn.Conv2d(16, 32, 3, 1))
for param in net.named_parameters():
    print(param[0], param[1].requires_grad)
# 0.weight True
# 0.bias True
# 1.weight True
# 1.bias True

output = net(input)
print(output.requires_grad)
# True
```
大家请注意前面只是举个例子来说明。在写代码的过程中，**不要把网络的输入和 Ground Truth 的 requires_grad 设置为 True**。虽然这样设置**不会影响反向传播**，但是**需要额外计算**网络的输入和 Ground Truth 的导数，**增大了计算量和内存**占用不说，这些计算出来的导数结果也没啥用。因为**只需要神经网络中的参数的导数**，用来更新网络，其余的导数都不需要。<br />好了，有个这个例子做铺垫，那么试试把网络参数的 requires_grad 设置为 False 会怎么样，同样的网络：
```python
input = torch.randn(8, 3, 50, 100)
print(input.requires_grad)
# False

net = nn.Sequential(nn.Conv2d(3, 16, 3, 1),
                    nn.Conv2d(16, 32, 3, 1))
for param in net.named_parameters():
    param[1].requires_grad = False
    print(param[0], param[1].requires_grad)
# 0.weight False
# 0.bias False
# 1.weight False
# 1.bias False

output = net(input)
print(output.requires_grad)
# False
```
这样有什么用处？用处大了。可以通过这种方法，**在训练的过程中冻结部分网络**，让这些层的参数不再更新，这在**迁移学习**中很有用处。来看一个例子：
```python
# https://pytorch.org/tutorials/beginner/finetuning_torchvision_models_tutorial.html#initialize-and-reshape-the-networks
model = torchvision.models.resnet18(pretrained=True)
for param in model.parameters():
    param.requires_grad = False

# 用一个新的 fc 层来取代之前的全连接层
# 因为新构建的 fc 层的参数默认 requires_grad=True
model.fc = nn.Linear(512, 100)

# 只更新 fc 层的参数
optimizer = optim.SGD(model.fc.parameters(), lr=1e-2, momentum=0.9)

# 通过这样，就冻结了 resnet 前边的所有层，
# 在训练过程中只更新最后的 fc 层中的参数。
```
<a name="CJUiJ"></a>
## 2、`torch.no_grad()`
当在做 **evaluating** 的时候（不需要计算导数），可以**将推断（inference）的代码包裹在 **`**with torch.no_grad():**`** 之中**，以达到**暂时不追踪网络参数中的导数**的目的，总之是为了减少可能存在的计算和内存消耗。看官方Tutorial 给出的例子：
```python
# https://link.zhihu.com/?target=https%3A//pytorch.org/tutorials/beginner/blitz/autograd_tutorial.html%23gradients
x = torch.randn(3, requires_grad = True)
print(x.requires_grad)
# True
print((x ** 2).requires_grad)
# True

with torch.no_grad():
    print((x ** 2).requires_grad)
    # False

print((x ** 2).requires_grad)
# True
```
<a name="b0Oxk"></a>
## 3、反向传播及网络的更新
这部分比较简单地讲一讲，有了**网络输出之后**，**怎么根据这个结果来更新网络参数**呢。以一个非常简单的自定义网络来讲解这个问题，这个网络包含**2个卷积层，1个全连接层**，输出的结果是**20维**的，类似分类问题中一共有**20个类别**，网络如下：
```python
class Simple(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(3, 16, 3, 1, padding=1, bias=False)
        self.conv2 = nn.Conv2d(16, 32, 3, 1, padding=1, bias=False)
        self.linear = nn.Linear(32*10*10, 20, bias=False)

    def forward(self, x):
        x = self.conv1(x)
        x = self.conv2(x)
        x = self.linear(x.view(x.size(0), -1))
        return x
```
接下来用这个网络，来研究一下**整个网络更新的流程**：
```python
# 创建一个很简单的网络：两个卷积层，一个全连接层
model = Simple()
# 为了方便观察数据变化，把所有网络参数都初始化为 0.1
for m in model.parameters():
    m.data.fill_(0.1)

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(), lr=1.0)

model.train()
# 模拟输入8个 sample，每个的大小是 10x10，
# 值都初始化为1，让每次输出结果都固定，方便观察
images = torch.ones(8, 3, 10, 10)
targets = torch.ones(8, dtype=torch.long)

output = model(images)
print(output.shape)
# torch.Size([8, 20])

loss = criterion(output, targets)

print(model.conv1.weight.grad)
# None
loss.backward()
print(model.conv1.weight.grad[0][0][0])
# tensor([-0.0782, -0.0842, -0.0782])
# 通过一次反向传播，计算出网络参数的导数，
# 因为篇幅原因，只观察一小部分结果

print(model.conv1.weight[0][0][0])
# tensor([0.1000, 0.1000, 0.1000], grad_fn=<SelectBackward>)
# 网络参数的值一开始都初始化为 0.1 的

optimizer.step()
print(model.conv1.weight[0][0][0])
# tensor([0.1782, 0.1842, 0.1782], grad_fn=<SelectBackward>)
# 回想刚才设置 learning rate 为 1，这样，
# 更新后的结果，正好是 (原始权重 - 求导结果) ！

# https://blog.csdn.net/scut_salmon/article/details/82414730
optimizer.zero_grad()
print(model.conv1.weight.grad[0][0][0])
# tensor([0., 0., 0.])
# 每次更新完权重之后，记得要把导数清零啊，
# 不然下次会得到一个和上次计算一起累加的结果。
# 当然，zero_grad() 的位置，可以放到前边去，
# 只要保证在计算导数前，参数的导数是清零的就好。
```
这里，多提一句，把**整个网络参数的值都传到 optimizer 里面了**，这种情况下**调用 **`**model.zero_grad()**`**效果是和 **`**optimizer.zero_grad()**`** 一样**的。这个知道就好，建议大家坚持用 `optimizer.zero_grad()`。现在来看一下如果没有调用 `zero_grad()`，会怎么样吧：
```python
# ...
# 代码和之前一样
model.train()

# 第一轮
images = torch.ones(8, 3, 10, 10)
targets = torch.ones(8, dtype=torch.long)

output = model(images)
loss = criterion(output, targets)
loss.backward()
print(model.conv1.weight.grad[0][0][0])
# tensor([-0.0782, -0.0842, -0.0782])

# 第二轮
output = model(images)
loss = criterion(output, targets)
loss.backward()
print(model.conv1.weight.grad[0][0][0])
# tensor([-0.1564, -0.1684, -0.1564])
```
可以看到，**第二次的结果正好是第一次的2倍**。第一次结束之后，因为**没有更新网络权重**，所以第二次反向传播的求导结果和第一次结果一样，加上上次没有将梯度清零，所以结果正好是2倍。
<a name="HXCUP"></a>
## 4、`tensor.detach()`
接下来探讨两个 0.4.0 版本更新产生的遗留问题。第一个，`tensor.data` 和 `tensor.detach()`。<br />在 0.4.0 版本以前，.data 是用来取 Variable 中的 tensor 的，但是之后 Variable 被取消，.data 却留了下来。现在调用 tensor.data，可以得到 **tensor的数据 + requires_grad=False 的版本**，而且**二者共享储存空间**，也就是如果修改其中一个，另一个也会变。因为 PyTorch 的**自动求导系统不会追踪 tensor.data 的变化**，所以使用它的话**可能会导致求导结果出错**。官方建议使**用 tensor.detach() 来替代它**，二者作用相似，但是 **detach 会被自动求导系统追踪**，使用起来很安全。多说无益，来看个例子：
```python
a = torch.tensor([7., 0, 0], requires_grad=True)
b = a + 2
print(b)
# tensor([9., 2., 2.], grad_fn=<AddBackward0>)

loss = torch.mean(b * b)

b_ = b.detach()
b_.zero_()
print(b)
# tensor([0., 0., 0.], grad_fn=<AddBackward0>)
# 储存空间共享，修改 b_ , b 的值也变了

loss.backward()
# RuntimeError: one of the variables needed for gradient computation has been modified by an inplace operation
```
这个例子中，b 是用来计算 loss 的一个变量，在计算完 loss 之后，进行反向传播之前，修改 b 的值。这么做会导致相关的导数的计算结果错误，因为在计算导数的过程中还会用到 b 的值，但是它已经变了（和正向传播过程中的值不一样了）。在这种情况下，PyTorch 选择报错来提示。但是，如果使用 `tensor.data` 的时候，结果是这样的：
```python
# https://github.com/pytorch/pytorch/issues/6990
a = torch.tensor([7., 0, 0], requires_grad=True)
b = a + 2
print(b)
# tensor([9., 2., 2.], grad_fn=<AddBackward0>)

loss = torch.mean(b * b)

b_ = b.data
b_.zero_()
print(b)
# tensor([0., 0., 0.], grad_fn=<AddBackward0>)

loss.backward()

print(a.grad)
# tensor([0., 0., 0.])

# 其实正确的结果应该是：
# tensor([6.0000, 1.3333, 1.3333])
```
这个**导数计算的结果明显是错的**，但**没有任何提醒**，之后再 Debug 会非常痛苦。所以，建议大家都用 `tensor.detach()`。
<a name="LN9U9"></a>
## 5、CPU and GPU
接下来来说另一个问题，是关于 `tensor.cuda()` 和 `tensor.to(device)` 的。后者是 0.4.0 版本之后后添加的，当 device 是 GPU 的时候，这两者并没有区别。那为什么要在新版本增加后者这个表达呢，是因为有了它，直接在代码最上边加一句话指定 device ，后面的代码直接用`to(device)` 就可以了：
```python
device = torch.device("cuda") if torch.cuda.is_available() else torch.device("cpu")

a = torch.rand([3,3]).to(device)
# 干其他的活
b = torch.rand([3,3]).to(device)
# 干其他的活
c = torch.rand([3,3]).to(device)
```
而之前版本的话，当每次在不同设备之间切换的时候，每次都要用 if cuda.is_available() 判断能否使用 GPU，很麻烦。
```python
# https://stackoverflow.com/questions/53331247/pytorch-0-4-0-there-are-three-ways-to-create-tensors-on-cuda-device-is-there-s
if torch.cuda.is_available():
    a = torch.rand([3,3]).cuda()
# 干其他的活
if  torch.cuda.is_available():
    b = torch.rand([3,3]).cuda()
# 干其他的活
if  torch.cuda.is_available():
    c = torch.rand([3,3]).cuda()
```
关于使用 GPU 还有一个点，想**把 GPU tensor 转换成 Numpy** 变量的时候，需要**先将 tensor 转换到 CPU 中**去，因为 **Numpy 是 CPU-only**的。其次，如果 **tensor 需要求导**的话，还**需要加一步 detach**，再转成 Numpy 。例子如下：
```python
x  = torch.rand([3,3], device='cuda')
x_ = x.cpu().numpy()

y  = torch.rand([3,3], requires_grad=True, device='cuda').
y_ = y.cpu().detach().numpy()
# y_ = y.detach().cpu().numpy() 也可以
# 二者好像差别不大？来比比时间：
start_t = time.time()
for i in range(10000):
    y_ = y.cpu().detach().numpy()
print(time.time() - start_t)
# 1.1049120426177979

start_t = time.time()
for i in range(10000):
    y_ = y.detach().cpu().numpy()
print(time.time() - start_t)
# 1.115112543106079
# 时间差别不是很大，当然，这个速度差别可能和电脑配置
# （比如 GPU 很贵，CPU 却很烂）有关。
```
<a name="WyCz3"></a>
## 6、`tensor.item()`
在提取 loss 的纯数值的时候，常常会用到 `**loss.item()**`，其**返回值是一个 Python 数值** (python number)。不像从 tensor 转到 numpy (需要考虑 tensor 是在 cpu，还是 gpu，需不需要求导)，无论什么情况，都直接使用 `item()` 就完事了。如果需要**从 gpu 转到 cpu** 的话，PyTorch 会**自动**处理。<br />但注意 `item()` **只适用于 tensor 只包含一个元素**的时候。因为**大多数情况下的 loss 就只有一个元素**，所以就经常会用到 `loss.item()`。如果想把**含多个元素**的 tensor 转换成 Python list 的话，要使用 `**tensor.tolist()**`。
```python
x  = torch.randn(1, requires_grad=True, device='cuda')
print(x)
# tensor([-0.4717], device='cuda:0', requires_grad=True)

y = x.item()
print(y, type(y))
# -0.4717346727848053 <class 'float'>

x = torch.randn([2, 2])
y = x.tolist()
print(y)
# [[-1.3069953918457031, -0.2710231840610504], [-1.26217520236969, 0.5559719800949097]]
```
<a name="tNgEf"></a>
## 结语
以上内容就是平时在写代码的时候，觉得需要注意的地方。文章中用了一些简单的代码作为例子，旨在帮助大家理解。
