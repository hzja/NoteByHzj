PyTorch<br />PyTorch 中有一些基础概念在构建网络的时候很重要，比如 `**nn.Module**`**, **`**nn.ModuleList**`**, **`**nn.Sequential**`，这些类称之为**容器** (containers)，因为**可以添加模块 (module)** 到它们之中。<br />这些容器之间很容易混淆，本文中主要学习一下 `nn.ModuleList` 和 `nn.Sequential`，并**判断在什么时候用哪一个比较合适**。<br />这里的例子使用的是 PyTorch 1.0 版本。
<a name="DPyn5"></a>
## `nn.ModuleList`
首先说说 `nn.ModuleList` 这个类，可以**把任意 **`**nn.Module**`** 的子类** (比如 `nn.Conv2d`, `nn.Linear` 之类的) **加到这个 list 里面**，方法和 Python 自带的 list 一样，无非是 `extend`，`append` 等操作。<br />但**不同于一般的 list**，加入到 `nn.ModuleList` 里面的 module 是**会自动注册到整个网络上**的，同时 **module 的 parameters 也会自动添加到整个网络**中。描述看起来很枯燥，来看几个例子。<br />**第一个网络**，先来看看**使用 **`**nn.ModuleList**`** 来构建一个小型网络**，包括3个全连接层：
```python
class net1(nn.Module):
    def __init__(self):
        super(net1, self).__init__()
        self.linears = nn.ModuleList([nn.Linear(10,10) for i in range(2)])
    def forward(self, x):
        for m in self.linears:
            x = m(x)
        return x

net = net1()
print(net)
# net1(
#   (modules): ModuleList(
#     (0): Linear(in_features=10, out_features=10, bias=True)
#     (1): Linear(in_features=10, out_features=10, bias=True)
#   )
# )

for param in net.parameters():
    print(type(param.data), param.size())
# <class 'torch.Tensor'> torch.Size([10, 10])
# <class 'torch.Tensor'> torch.Size([10])
# <class 'torch.Tensor'> torch.Size([10, 10])
# <class 'torch.Tensor'> torch.Size([10])
```
可以看到，这个网络包含两个全连接层，他们的**权重 (weithgs) 和偏置 (bias) 都在这个网络之内**。<br />接下来看看**第二个网络**，它**使用 Python 自带的 list**：
```python
class net2(nn.Module):
    def __init__(self):
        super(net2, self).__init__()
        self.linears = [nn.Linear(10,10) for i in range(2)]
    def forward(self, x):
        for m in self.linears:
            x = m(x)
        return x

net = net2()
print(net)
# net2()
print(list(net.parameters()))
# []
```
显然，**使用 Python 的 list** 添加的全连接层和它们的 parameters 并**没有自动注册到网络中**。当然，还是可以使用 `forward` 来计算输出结果。<br />但是如果**用 net2** 实例化的网络进行训练的时候，因为**这些层的 parameters 不在整个网络之中**，所以其**网络参数也不会被更新**，也就是**无法训练**。<br />好，看到这里，大致明白了 `**nn.ModuleList**` 是干什么的了：它**是一个储存不同 module**，**并自动将每个 module 的 parameters 添加到网络之中的容器**。<br />但是，需要注意到，nn.ModuleList **并没有定义一个网络**，它只是**将不同的模块储存在一起**，这些**模块之间**并**没有什么先后顺序**可言，比如：
```python
class net3(nn.Module):
    def __init__(self):
        super(net3, self).__init__()
        self.linears = nn.ModuleList([nn.Linear(10,20), nn.Linear(20,30), nn.Linear(5,10)])
    def forward(self, x):
        x = self.linears[2](x)
        x = self.linears[0](x)
        x = self.linears[1](x) 
        return x

net = net3()
print(net)
# net3(
#   (linears): ModuleList(
#     (0): Linear(in_features=10, out_features=20, bias=True)
#     (1): Linear(in_features=20, out_features=30, bias=True)
#     (2): Linear(in_features=5, out_features=10, bias=True)
#   )
# )
input = torch.randn(32, 5)
print(net(input).shape)
# torch.Size([32, 30])
```
根据 net3 的结果，可以看出来这个 **ModuleList 里面的顺序并不能决定什么**，网络的**执行顺序是根据 forward 函数来决定**的。<br />如果非要 ModuleList 和 forward 中的顺序不一样， PyTorch 表示它无所谓，但以后 review 代码的人可能会意见比较大。<br />再考虑另外一种情况，既然这个 ModuleList **可以根据序号来调用**，那么**一个模块是否可以在 forward 函数中被调用多次呢？**<br />答案当然是**可以**的，但是，被调用多次的模块，是**使用同一组 parameters** 的，也就是它们的**参数是共享的**，无论之后怎么更新。例子如下，虽然在 forward 中**用了 **`**nn.Linear(10,10)**`** 两次**，但是它们**只有一组参数**。这么做有什么用处呢，目前没有想到...
```python
class net4(nn.Module):
    def __init__(self):
        super(net4, self).__init__()
        self.linears = nn.ModuleList([nn.Linear(5, 10), nn.Linear(10, 10)])
    def forward(self, x):
        x = self.linears[0](x)
        x = self.linears[1](x)
        x = self.linears[1](x)
        return x

net = net4()
print(net)
# net4(
#   (linears): ModuleList(
#     (0): Linear(in_features=5, out_features=10, bias=True)
#     (1): Linear(in_features=10, out_features=10, bias=True)
#   )
# )
for name, param in net.named_parameters():
    print(name, param.size())
# linears.0.weight torch.Size([10, 5])
# linears.0.bias torch.Size([10])
# linears.1.weight torch.Size([10, 10])
# linears.1.bias torch.Size([10])
```
<a name="JyxZc"></a>
## `nn.Sequential`
现在来研究一下 `**nn.Sequential**`，**不同于 **`**nn.ModuleList**`，**它已经实现了内部的 forward 函数**，而且里面的模块**必须是按照顺序进行排列**的，所以**必须确保前一个模块的输出大小和下一个模块的输入大小是一致的**，如下面的例子所示：
```python
class net5(nn.Module):
    def __init__(self):
        super(net5, self).__init__()
        self.block = nn.Sequential(nn.Conv2d(1,20,5),
                                    nn.ReLU(),
                                    nn.Conv2d(20,64,5),
                                    nn.ReLU())
    def forward(self, x):
        x = self.block(x)
        return x

net = net5()
print(net)
# net5(
#   (block): Sequential(
#     (0): Conv2d(1, 20, kernel_size=(5, 5), stride=(1, 1))
#     (1): ReLU()
#     (2): Conv2d(20, 64, kernel_size=(5, 5), stride=(1, 1))
#     (3): ReLU()
#   )
# )
```
下面给出了两个 nn.Sequential 初始化的例子，来自于官网教程。在第二个初始化中**用到了 OrderedDict 来指定每个 module 的名字**，而不是采用默认的命名方式 (按序号 0,1,2,3...) 。
```python
# https://pytorch.org/docs/stable/nn.html#sequential
# Example of using Sequential
model1 = nn.Sequential(
          nn.Conv2d(1,20,5),
          nn.ReLU(),
          nn.Conv2d(20,64,5),
          nn.ReLU()
        )
print(model1)
# Sequential(
#   (0): Conv2d(1, 20, kernel_size=(5, 5), stride=(1, 1))
#   (1): ReLU()
#   (2): Conv2d(20, 64, kernel_size=(5, 5), stride=(1, 1))
#   (3): ReLU()
# )

# Example of using Sequential with OrderedDict
import collections
model2 = nn.Sequential(collections.OrderedDict([
          ('conv1', nn.Conv2d(1,20,5)),
          ('relu1', nn.ReLU()),
          ('conv2', nn.Conv2d(20,64,5)),
          ('relu2', nn.ReLU())
        ]))
print(model2)
# Sequential(
#   (conv1): Conv2d(1, 20, kernel_size=(5, 5), stride=(1, 1))
#   (relu1): ReLU()
#   (conv2): Conv2d(20, 64, kernel_size=(5, 5), stride=(1, 1))
#   (relu2): ReLU()
# )
```
有同学可能发现了，这个 model1 和 从类 net5 实例化来的 net 有什么区别吗？是没有的。**这两个网络是相同的**，因为 `nn.Sequential` 就是一个 `nn.Module` 的子类，也就是 `nn.Module` 所有的方法 (method) 它都有。并且**直接使用 **`**nn.Sequential**`** 不用写 forward 函数**，因为它内部已经写好了。<br />这时候有同学该说了，既然 `nn.Sequential` 这么好，以后都直接用它了。如果确定 `nn.Sequential` 里面的顺序是想要的，而且不需要再添加一些其他处理的函数，那么完全可以直接用 `nn.Sequential`。这么做的代价就是失去了部分灵活性，毕竟不能自己去定制 forward 函数里面的内容了。<br />一般情况下 `nn.Sequential` 的**用法是来组成卷积块 (block)**，然后像拼积木一样把不同的 block 拼成整个网络，让代码更简洁，更加结构化。
<a name="iU7MP"></a>
## `nn.ModuleList` 和 `nn.Sequential`：到底该用哪个
前边已经简单介绍了这两个类，现在来讨论一下**在两个不同的场景中，选择哪一个比较合适**。<br />**场景一**，有的时候**网络中有很多相似或者重复的层**，一般会考虑**用 for 循环来创建**它们，而不是一行一行地写，比如：
```python
layers = [nn.Linear(10, 10) for i in range(5)]
```
这个时候，很自然而然地，会想到**使用 ModuleList**，像这样：
```python
class net6(nn.Module):
    def __init__(self):
        super(net6, self).__init__()
        self.linears = nn.ModuleList([nn.Linear(10, 10) for i in range(3)])

    def forward(self, x):
        for layer in self.linears:
            x = layer(x)
        return x

net = net6()
print(net)
# net6(
#   (linears): ModuleList(
#     (0): Linear(in_features=10, out_features=10, bias=True)
#     (1): Linear(in_features=10, out_features=10, bias=True)
#     (2): Linear(in_features=10, out_features=10, bias=True)
#   )
# )
```
这个是比较一般的方法，但如果不想这么麻烦，也可以用 Sequential 来实现，如 net7 所示！**注意 * 这个操作符**，它可以**把一个 list 拆开成一个个独立的元素**。<br />但是，请注意这个 list 里面的**模块必须是按照想要的顺序来进行排列**的。在 **场景一** 中，个人觉得使用 net7 这种方法比较方便和整洁。
```python
class net7(nn.Module):
    def __init__(self):
        super(net7, self).__init__()
        self.linear_list = [nn.Linear(10, 10) for i in range(3)]
        self.linears = nn.Sequential(*self.linears_list)

    def forward(self, x):
        self.x = self.linears(x)
        return x

net = net7()
print(net)
# net7(
#   (linears): Sequential(
#     (0): Linear(in_features=10, out_features=10, bias=True)
#     (1): Linear(in_features=10, out_features=10, bias=True)
#     (2): Linear(in_features=10, out_features=10, bias=True)
#   )
# )
```
下面考虑 **场景二**，当**需要之前层的信息**的时候，比如 ResNets 中的 shortcut 结构，或者是像 FCN 中用到的 skip architecture 之类的，当前层的结果需要和之前层中的结果进行融合，**一般使用 ModuleList 比较方便**，一个非常简单的例子如下：
```python
class net8(nn.Module):
    def __init__(self):
        super(net8, self).__init__()
        self.linears = nn.ModuleList([nn.Linear(10, 20), nn.Linear(20, 30), nn.Linear(30, 50)])
        self.trace = []

    def forward(self, x):
        for layer in self.linears:
            x = layer(x)
            self.trace.append(x)
        return x

net = net8()
input  = torch.randn(32, 10) # input batch size: 32
output = net(input)
for each in net.trace:
    print(each.shape)
# torch.Size([32, 20])
# torch.Size([32, 30])
# torch.Size([32, 50])
```
使用了一个 trace 的列表来储存网络每层的输出结果，这样如果以后的层要用的话，就可以很方便地调用了。
<a name="JnsWJ"></a>
## 总结
通过一些实例学习了 ModuleList 和 Sequential 这两种 nn containers，**ModuleList 就是一个储存各种模块的 list**，这些**模块之间没有联系**，**没有实现 forward 功能**，但相比于普通的 Python list，ModuleList **可以把添加到其中的模块和参数自动注册到网络上**。<br />而**Sequential 内的模块需要按照顺序排列**，要**保证相邻层的输入输出大小相匹配**，**内部 forward 功能已经实现**，可以使代码更加整洁。在不同场景中，如果二者都适用，那就看个人偏好了。非常推荐大家看一下 PyTorch 官方的 TorchVision 下面模型实现的代码，能学到很多构建网络的技巧。
