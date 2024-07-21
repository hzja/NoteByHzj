PyTorch<br />Pytorch 保存和加载模型后缀：**.pt 和.pth**
<a name="GcXlN"></a>
## 1、`torch.save() `
保存一个**序列化**（serialized）的目标到磁盘。函数使用了Python的**pickle**程序用于序列化。<br />模型（models），张量（tensors）和文件夹（dictionaries）都是可以用这个函数保存的目标类型。
```python
torch.save(obj, f, pickle_module=<module '...'>, pickle_protocol=2)
```
<a name="rQWGU"></a>
### 参数描述：

- **obj**：保存对象
- **f**：类文件对象 (必须实现写和刷新)或一个保存文件名的字符串
- **pickle_module**：用于 pickling 元数据和对象的模块
- **pickle_protocol**：指定 pickle protocol 可以覆盖默认参数
<a name="QE1hL"></a>
### 案例：

- **保存整个模型**：
```python
torch.save(model,'save.pt')
```

- **只保存训练好的权重**：
```python
torch.save(model.state_dict(), 'save.pt')
```
<a name="Tqlpj"></a>
## 2、`torch.load()`
用来**加载模型**。`torch.load()` 使用 Python 的 **解压工具（unpickling）来反序列化 pickled object 到对应存储设备上**。<br />首先**在 CPU 上对压缩对象进行反序列化**并且**移动到它们保存的存储设备上**，如果**失败**了（如：由于系统中没有相应的存储设备），就会**抛出一个异常**。用户可以通过 register_package 进行扩展，使用自己定义的标记和反序列化方法。
```python
torch.load(f, map_location=None, pickle_module=<module 'pickle' from '...'>)
```
<a name="PQ0xZ"></a>
### 参数描述：

- `**f**`：类文件对象 (返回文件描述符)或一个保存文件名的字符串
- `**map_location**`：一个函数或字典规定如何映射存储设备
- `**pickle_module**`：用于 unpickling 元数据和对象的模块 (必须匹配序列化文件时的 pickle_module )
<a name="rtDk3"></a>
### 案例：
```python
torch.load('tensors.pt')

# Load all tensors onto the CPU
torch.load('tensors.pt', map_location=torch.device('cpu'))

# Load all tensors onto the CPU, using a function
torch.load('tensors.pt', map_location=lambda storage, loc: storage)

# Load all tensors onto GPU 1
torch.load('tensors.pt', map_location=lambda storage, loc: storage.cuda(1))

# Map tensors from GPU 1 to GPU 0
torch.load('tensors.pt', map_location={'cuda:1':'cuda:0'})

# Load tensor from io.BytesIO object
with open('tensor.pt') as f:
    buffer = io.BytesIO(f.read())
torch.load(buffer)
```
<a name="Krtoy"></a>
## 3、`torch.nn.Module.load_state_dict(state_dict)`
**使用 state_dict 反序列化模型参数字典**。用来加载模型参数。将 state_dict 中的 parameters 和 buffers **复制到此 module 及其子节点中**。
```python
torch.nn.Module.load_state_dict(state_dict, strict=True)
```
<a name="isGhG"></a>
### 参数描述：

- `**state_dict**`：保存 parameters 和 persistent buffers 的字典
- `**strict**`：可选，bool型。state_dict 中的 key 是否和 `model.state_dict()` 返回的 key 一致。
<a name="rixVM"></a>
### 案例：
```python
torch.save(model,'save.pt')
model.load_state_dict(torch.load("save.pt"))  
#model.load_state_dict()函数把加载的权重复制到模型的权重中去
```
<a name="B5jg1"></a>
### 什么是state_dict？
在PyTorch中，一个`torch.nn.Module`模型中的**可学习参数**（比如weights和biases），模型的参数通过`**model.parameters()**`获取。而state_dict就是一个简单的**Python dictionary**，其功能是**将层与层的参数张量一一映射**。<br />注意，**只包含了可学习参数**（卷积层、线性层等）**的层和已注册的命令**（registered buffers，比如batchnorm的running_mean）才有模型的state_dict入口。<br />优化方法目标（**torch.optim**）也有state_dict，其中包含的是关于优化器状态的信息和使用到的超参数。<br />因为**state_dict**目标是**Python dictionaries**，所以它们可以很轻松地实现保存、更新、变化和再存储，从而给PyTorch模型和优化器增加了大量的模块化（modularity）。
<a name="sn23T"></a>
#### 1) `torch.nn.Module.state_dict`
```python
torch.nn.Module.state_dict(destination=None, prefix='', keep_vars=False)
```
返回一个**包含模型状态信息的字典**。包含**参数**（weighs and biases）和持续的**缓冲值**（如：观测值的平均值）。只有**具有可更新参数的层**才会被保存在模型的 state_dict 数据结构中。<br />**案例：**
```python
module.state_dict().keys()
# ['bias', 'weight']
```
<a name="sEZ03"></a>
#### 2) `torch.optim.Optimizer.state_dict`
```python
torch.optim.Optimizer.state_dict()
```
返回一个**包含优化器状态信息的字典**。包含两个 key：

- `**state**`：字典，保存当前优化器的状态信息。不同优化器内容不同。
- `**param_groups**`：字典，包含所有参数组（如：超参数）。

**案例：**
```python
from __future__ import print_function, division

import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import lr_scheduler
import numpy as np
import torchvision
from torchvision import datasets, models, transforms
import matplotlib.pyplot as plt
import time
import os
import copy


# 定义模型
class TheModelClass(nn.Module):
    def __init__(self):
        super(TheModelClass, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = x.view(-1, 16 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

# 初始化模型
model = TheModelClass()

# 初始化优化器
optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)

# 打印模型的 state_dict
print("Model's state_dict:")
for param_tensor in model.state_dict():
    print(param_tensor, "\t", model.state_dict()[param_tensor].size())

# 打印优化器的 state_dict
print("Optimizer's state_dict:")
for var_name in optimizer.state_dict():
    print(var_name, "\t", optimizer.state_dict()[var_name]) 
```
**输出：**
```python
Model's state_dict:
conv1.weight     torch.Size([6, 3, 5, 5])
conv1.bias   torch.Size([6])
conv2.weight     torch.Size([16, 6, 5, 5])
conv2.bias   torch.Size([16])
fc1.weight   torch.Size([120, 400])
fc1.bias     torch.Size([120])
fc2.weight   torch.Size([84, 120])
fc2.bias     torch.Size([84])
fc3.weight   torch.Size([10, 84])
fc3.bias     torch.Size([10])

Optimizer's state_dict:
state    {}
param_groups     [{'lr': 0.001, 'momentum': 0.9, 'dampening': 0, 'weight_decay': 0, 'nesterov': False, 'params': [4675713712, 4675713784, 4675714000, 4675714072, 4675714216, 4675714288, 4675714432, 4675714504, 4675714648, 4675714720]}]
```
<a name="hVTN1"></a>
## 4、保存/加载
<a name="Y24Kv"></a>
### **4.1 state_dict（推荐）**
```python
# 保存
torch.save(model.state_dict(), PATH)


# 加载
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH))
model.eval()
```
<a name="qfEb9"></a>
### **4.2 整个模型**
```python
# 保存
torch.save(model, PATH)


# 加载
# 模型类必须在别的地方定义
model = torch.load(PATH)
model.eval()
```
这种保存/加载模型的过程使用了**最直观**的语法，所用**代码量少**。这使用Python的pickle保存所有模块。<br />这种方法的**缺点**是，保存模型的时候，**序列化的数据被绑定到了特定的类和确切的目录**。这是因为pickle**不保存模型类本身**，而是**保存这个类的路径**，并且在加载的时候会使用。因此，当在其他项目里使用或者重构的时候，加载模型的时候**会出错**。<br />一般来说，PyTorch的模型以**.pt**或者**.pth**文件格式保存。<br />一定要记住在评估模式的时候调用`**model.eval()**`来固定dropout和批次归一化。否则会产生不一致的推理结果。
<a name="q6at9"></a>
### **4.3 保存加载用于推理的常规Checkpoint/或继续训练**
```python
# 保存
torch.save({
            'epoch': epoch,
            'model_state_dict': model.state_dict(),
            'optimizer_state_dict': optimizer.state_dict(),
            'loss': loss,
            ...
            }, PATH)

            
# 加载
model = TheModelClass(*args, **kwargs)
optimizer = TheOptimizerClass(*args, **kwargs)

checkpoint = torch.load(PATH)
model.load_state_dict(checkpoint['model_state_dict'])
optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
epoch = checkpoint['epoch']
loss = checkpoint['loss']

model.eval()
# - 或者 -
model.train()
```
在保存用于推理或者继续训练的常规检查点的时候**，除了模型的**`**state_dict**`之外，还**必须保存其他参数**。**保存优化器的state_dict**也非常重要，因为它包含了模型在训练时候优化器的缓存和参数。除此之外，还可以**保存停止训练时epoch数**，最新的模型损失，额外的`torch.nn.Embedding`层等。<br />要保存多个组件，则将它们**放到一个字典中**，然后**使用**`**torch.save()**`**序列化**这个字典。一般来说，**使用.tar文件格式来保存这些检查点**。<br />加载各个组件，首先初始化模型和优化器，然后使用`**torch.load()**`**加载**保存的字典，然后可以**直接查询字典中的值来获取保存的组件**。<br />同样，评估模型的时候一定不要忘了调用`**model.eval()**`**。**
<a name="XUy8h"></a>
### **4.4 保存多个模型到一个文件**
```python
# 保存
torch.save({
            'modelA_state_dict': modelA.state_dict(),
            'modelB_state_dict': modelB.state_dict(),
            'optimizerA_state_dict': optimizerA.state_dict(),
            'optimizerB_state_dict': optimizerB.state_dict(),
            ...
            }, PATH)


# 加载
modelA = TheModelAClass(*args, **kwargs)
modelB = TheModelBClass(*args, **kwargs)
optimizerA = TheOptimizerAClass(*args, **kwargs)
optimizerB = TheOptimizerBClass(*args, **kwargs)

checkpoint = torch.load(PATH)
modelA.load_state_dict(checkpoint['modelA_state_dict'])
modelB.load_state_dict(checkpoint['modelB_state_dict'])
optimizerA.load_state_dict(checkpoint['optimizerA_state_dict'])
optimizerB.load_state_dict(checkpoint['optimizerB_state_dict'])

modelA.eval()
modelB.eval()
# - 或者 -
modelA.train()
modelB.train()
```
<a name="Gv9Ig"></a>
### **4.5 使用其他模型来预热当前模型**
```python
# 保存
torch.save(modelA.state_dict(), PATH)


# 加载
modelB = TheModelBClass(*args, **kwargs)
modelB.load_state_dict(torch.load(PATH), strict=False)
```
在**迁移学习或者训练新的复杂模型**时，**加载部分模型**是很常见的。利用经过训练的参数，即使**只有少数参数可用**，也将**有助于预热训练过程**，并且使模型**更快收敛**。<br />在加载部分模型参数进行预训练的时候，很可能会**碰到键不匹配的情况**（模型权重都是按键值对的形式保存并加载回来的）。因此，无论是缺少键还是多出键的情况，都可以通过**在load_state_dict()函数中设定strict参数为False来忽略不匹配的键**。<br />如果想**将某一层的参数加载到其他层**，但是有些**键不匹配**，那么**修改state_dict中参数的key**可以解决这个问题。
<a name="NdrIS"></a>
### 4.6 跨设备保存与加载模型
<a name="jSYPC"></a>
#### 1) GPU上保存，CPU上加载
```python
# 保存
torch.save(model.state_dict(), PATH)


# 加载
device = torch.device('cpu')
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH, map_location=device))
```
当在CPU上加载一个GPU上训练的模型时，在`torch.load()`中指定`**map_location=torch.device('cpu')**`，此时，map_location动态地将tensors的底层存储重新映射到CPU设备上。<br />上述代码**只有在模型是在一块GPU上训练时才有效**，如果模型**在多个GPU上训练**，那么在CPU上加载时，会得到类似如下**错误**：
> KeyError: ‘unexpected key “module.conv1.weight” in state_dict’

原因是在使用多GPU训练并保存模型时，**模型的参数名都带上了module前缀**，因此可以在加载模型时，**把key中的这个前缀去掉**：
```python
# 原始通过DataParallel保存的文件
state_dict = torch.load('myfile.pth.tar')
# 创建一个不包含`module.`的新OrderedDict
from collections import OrderedDict
new_state_dict = OrderedDict()
for k, v in state_dict.items():
    name = k[7:] # 去掉 `module.`
    new_state_dict[name] = v
# 加载参数
model.load_state_dict(new_state_dict)
```
<a name="Qdacs"></a>
#### 2) GPU上保存，GPU上加载
```python
# 保存
torch.save(model.state_dict(), PATH)


# 加载
device = torch.device("cuda")
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH, map_location="cuda:0"))  # 选择希望使用的GPU
model.to(device)
```
在把GPU上训练的模型加载到GPU上时，只需要使用`**model.to(torch.devie('cuda'))**`将初始化的模型转换为CUDA优化模型。同时确保在模型所有的输入上使用`**.to(torch.device('cuda'))**`。<br />注意，调用`my_tensor.to(device)`会**返回一份在GPU上的my_tensor的拷贝**。**不会覆盖原本的my_tensor**，因此要记得**手动将tensor重写**：`my_tensor = my_tensor.to(torch.device('cuda'))`。
<a name="qfgfp"></a>
#### 3) CPU上保存，GPU上加载
```python
# 保存
torch.save(model.state_dict(), PATH)


# 加载
device = torch.device("cuda")
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH, map_location="cuda:0"))  
# Choose whatever GPU device number you want

model.to(device)
# Make sure to call input = input.to(device) on any input tensors that you feed to the model
```
在 GPU 上加载 CPU 训练保存的模型时，将 `torch.load()` 函数的 map_location 参数设置为 `**cuda:device_id**`。这种方式将模型加载到指定设备。下一步，确保调用 `**model.to(torch.device('cuda'))**`** **将模型参数 tensor 转换为 cuda tensor。最后，**确保模型输入使用 **`**.to(torch.device('cuda'))**`** **为 cuda 优化模型准备数据。<br />注意：调用 `my_tensor.to(device)` 会在 GPU 上返回 my_tensor 的新副本，不会覆盖 my_tensor。因此，使用 `my_tensor = my_tensor.to(torch.device('cuda'))` **手动覆盖**。
<a name="dst2z"></a>
### 4.7 保存`torch.nn.DataParallel`模型
```python
# 保存
torch.save(model.state_dict(), PATH)

# 加载
# Load to whatever device you want
```
`torch.nn.DataParallel` 是支持模型使用 GPU 并行的封装器。要保存一个一般的 DataParallel 模型， 请保存** **`**model.module.state_dict()**`。这种方式，可以灵活地以任何方式加载模型到任何设备上。
