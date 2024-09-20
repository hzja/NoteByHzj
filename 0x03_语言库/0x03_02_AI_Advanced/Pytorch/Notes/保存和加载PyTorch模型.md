PyTorch
<a name="XqWHU"></a>
## 1、需要掌握3个重要的函数
1) `torch.save`： 将一个序列化的对象保存到磁盘。这个函数使用 Python 的 `pickle` 工具进行序列化。模型 (`model`)、张量 (`tensor`) 和各种对象的字典 (`dict`) 都可以用这个函数保存。<br />2) `torch.load`： 将 `pickled` 对象文件反序列化到内存，也便于将数据加载到设备中。<br />3) `torch.nn.Module.load_state_dict()`： 加载模型的参数。
<a name="NXx19"></a>
## 2、`state_dict`
<a name="NGSlp"></a>
### 2.1 `state_dict` 介绍
PyTorch 中，`torch.nn.Module`里面的可学习的参数 (weights 和 biases) 都放在`model.parameters()`里面。而 `state_dict` 是一个 Python dictionary object，将每一层映射到它的 parameter tensor 上。注意：只有含有可学习参数的层 (convolutional layers, linear layers)，或者含有 registered buffers 的层 (batchnorm's running_mean) 才有 `state_dict`。优化器的对象 (`torch.optim`) 也有 `state_dict`，存储了优化器的状态和它的超参数。<br />因为 `state_dict` 是一个 Python dictionary object，所以保存，加载，更新它比较容易。<br />下面通过一个例子直观感受下 `state_dict` 的用法：
```python
# Define model
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

# Initialize model
model = TheModelClass()

# Initialize optimizer
optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)

# Print model's state_dict
print("Model's state_dict:")
for param_tensor in model.state_dict():
    print(param_tensor, "\t", model.state_dict()[param_tensor].size())

# Print optimizer's state_dict
print("Optimizer's state_dict:")
for var_name in optimizer.state_dict():
    print(var_name, "\t", optimizer.state_dict()[var_name])
```
输出：
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
<a name="nPG3y"></a>
### 2.2 保存和加载 `state_dict` (已经训练完，无需继续训练)
保存：
```python
torch.save(model.state_dict(), PATH)
```
加载：
```python
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH))
model.eval()
```
一般保存为`.pt`或`.pth` 格式的文件。<br />注意：

1. 可以使用`model.eval()`将 `dropout` 和 batch normalization 层设置成 evaluation 模式。
2. `load_state_dict()`函数需要一个 dict 类型的输入，而不是保存模型的 PATH。所以这样 `model.load_state_dict(PATH)`是错误的，而应该`model.load_state_dict(torch.load(PATH))`。
3. 如果想保存验证机上表现最好的模型，那么这样`best_model_state=model.state_dict()`是错误的。因为这属于浅复制，也就是说此时这个 `best_model_state` 会随着后续的训练过程而不断被更新，最后保存的其实是个 `overfit` 的模型。所以正确的做法应该是`best_model_state=deepcopy(model.state_dict())`。
<a name="o0X3g"></a>
### 2.3 保存和加载整个模型 (已经训练完，无需继续训练)
保存：
```python
torch.save(model, PATH)
```
加载：
```python
# Model class must be defined somewhere
model = torch.load(PATH)
model.eval()
```
一般保存为`.pt`或`.pth`格式的文件。<br />注意：

1. 可以使用`model.eval()`将 `dropout` 和 batch normalization 层设置成 evaluation 模式。
<a name="emdIP"></a>
### 2.4 保存和加载 `state_dict` (没有训练完，还会继续训练)
保存：
```python
torch.save({
    'epoch': epoch,
    'model_state_dict': model.state_dict(),
    'optimizer_state_dict': optimizer.state_dict(),
    'loss': loss,
    ...
}, PATH)
```
与2.2的不同是除了保存 `model_state_dict` 之外，还需要保存：`optimizer_state_dict`，`epoch` 和 `loss`，因为继续训练时要知道优化器的状态，`epoch` 等等。<br />加载：
```python
model = TheModelClass(*args, **kwargs)
optimizer = TheOptimizerClass(*args, **kwargs)

checkpoint = torch.load(PATH)
model.load_state_dict(checkpoint['model_state_dict'])
optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
epoch = checkpoint['epoch']
loss = checkpoint['loss']

model.eval()
# - or -
model.train()
```
与2.2的不同是除了加载 `model_state_dict` 之外，还需要加载：`optimizer_state_dict`，`epoch` 和 `loss`。
<a name="m9Av7"></a>
### 2.5 把多个模型存进一个文件
保存：
```python
torch.save({
    'modelA_state_dict': modelA.state_dict(),
    'modelB_state_dict': modelB.state_dict(),
    'optimizerA_state_dict': optimizerA.state_dict(),
    'optimizerB_state_dict': optimizerB.state_dict(),
    ...
}, PATH)
```
把模型 A 和 B 的 `state_dict` 和 `optimizer` 都存进一个文件中。<br />加载：
```python
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
# - or -
modelA.train()
modelB.train()
```
<a name="ur4PW"></a>
### 2.6 使用其他模型的参数暖启动自己的模型
有时候训练一个新的复杂模型时，需要加载它的一部分预训练的权重。即使只有几个可用的参数，也会有助于 warmstart 训练过程，帮助模型更快达到收敛。<br />如果手里有的这个 `state_dict` 缺乏一些 keys，或者多了一些 keys，只要设置`strict`参数为 `False`，就能够把 `state_dict` 能够匹配的 keys 加载进去，而忽略掉那些 non-matching keys。<br />保存模型 A 的 state_dict ：
```python
torch.save(modelA.state_dict(), PATH)
```
加载到模型 B：
```python
modelB = TheModelBClass(*args, **kwargs)
modelB.load_state_dict(torch.load(PATH), strict=False)
```
<a name="jFw0z"></a>
### 2.7 保存在 GPU, 加载到 CPU
保存：
```python
torch.save(model.state_dict(), PATH)
```
加载：
```python
device = torch.device('cpu')
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH, map_location=device))
```
这种情况 `model.state_dict()` 保存之后在 GPU，直接 `torch.load(PATH)` 会加载进 GPU 中。所以若想加载到 CPU 中，需要加 `map_location=torch.device('cpu')`。
<a name="nNMwm"></a>
### 2.8 保存在 GPU, 加载到 GPU
保存：
```python
torch.save(model.state_dict(), PATH)
```
加载：
```python
map_location="cuda:0"device = torch.device("cuda")
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH))
model.to(device)
# Make sure to call input = input.to(device) on any input tensors that you feed to the model
```
这种情况 `model.state_dict()` 保存之后在 GPU，直接 `torch.load(PATH)` 会加载进 GPU 中。所以若想加载到 GPU 中，不需要加 `map_location=device`。因为最后要加载到 GPU 里面，model 是重新初始化的 (在 CPU 里面)，所以要 `model.to(device)`。
<a name="k9Q6W"></a>
### 2.9 保存在 CPU, 加载到 GPU
保存：
```python
torch.save(model.state_dict(), PATH)
```
加载：
```python
device = torch.device("cuda")
model = TheModelClass(*args, **kwargs)
model.load_state_dict(torch.load(PATH, map_location="cuda:0"))  # Choose whatever GPU device number you want
model.to(device)
# Make sure to call input = input.to(device) on any input tensors that you feed to the model
```
这种情况 `model.state_dict()` 保存之后在 CPU，直接 `torch.load(PATH)` 会加载进 CPU 中。所以若想加载到 GPU 中，需要加 `map_location="cuda:0"` 。因为最后要加载到 GPU 里面，model 是重新初始化的 (在 CPU 里面)，所以要 `model.to(device)`。
