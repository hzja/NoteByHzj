机器学习 PyTorch<br />在大多数情况下，基本都是以类的形式实现神经网络。<br />大多数情况下创建一个继承自 Pytorch 中的 `nn.Module` 的类，这样可以使用 Pytorch 提供的许多高级 API，而无需自己实现。<br />下面展示了一个可以从`nn.Module`创建的最简单的神经网络类的示例。基于 `nn.Module`的类的最低要求是覆盖`__init__()`方法和`forward()`方法。<br />在这个类中，定义了一个简单的线性网络，具有两个输入和一个输出，并使用 `Sigmoid()`函数作为网络的激活函数。
```python
import torch
from torch import nn

class LinearRegression(nn.Module):
    def __init__(self):
        #继承父类构造函数
        super(LinearRegression, self).__init__() 
        #输入和输出的维度都是1
        self.linear = nn.Linear(1, 1) 
    def forward(self, x):
        out = self.linear(x)
        return out
```
现在测试一下模型。
```python
# 创建LinearRegression（）的实例
model = LinearRegression()
print(model) 
# 输出如下
LinearRegression(
  (linear): Linear(in_features=1, out_features=1, bias=True)
)
```
现在让定义一个损失函数和优化函数。
```python
model = LinearRegression()#实例化对象
num_epochs = 1000#迭代次数
learning_rate = 1e-2#学习率0.01
Loss = torch.nn.MSELoss()#损失函数
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)#优化函数
```
创建一个由方程产生的数据集，并通过函数制造噪音
```python
import torch 
from matplotlib import pyplot as plt
from torch.autograd import Variable
from torch import nn
# 创建数据集  unsqueeze 相当于
x = Variable(torch.unsqueeze(torch.linspace(-1, 1, 100), dim=1))
y = Variable(x * 2 + 0.2 + torch.rand(x.size()))
plt.scatter(x.data.numpy(),y.data.numpy())
plt.show()
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628423508534-565348fd-15bc-446a-9a4d-f8a576e2f6b0.png#clientId=ud3a82cc3-5faf-4&from=paste&id=u9584acc1&originHeight=251&originWidth=370&originalType=url&ratio=1&size=8333&status=done&style=shadow&taskId=ua78b1957-1b86-4242-87b7-a999ed8fa15)<br />关于`torch.unsqueeze`函数解读。
```python
>>> x = torch.tensor([1, 2, 3, 4])
>>> torch.unsqueeze(x, 0)
tensor([[ 1,  2,  3,  4]])
>>> torch.unsqueeze(x, 1)
tensor([[ 1],
        [ 2],
        [ 3],
        [ 4]])
```
遍历每次epoch，计算出loss，反向传播计算梯度，不断的更新梯度，使用梯度下降进行优化。
```python
for epoch in range(num_epochs):
    # 预测
    y_pred= model(x)
    # 计算loss
    loss = Loss(y_pred, y)
    #清空上一步参数值
    optimizer.zero_grad()
    #反向传播
    loss.backward()
    #更新参数
    optimizer.step()
    if epoch % 200 == 0:
        print("[{}/{}] loss:{:.4f}".format(epoch+1, num_epochs, loss))

plt.scatter(x.data.numpy(), y.data.numpy())
plt.plot(x.data.numpy(), y_pred.data.numpy(), 'r-',lw=5)
plt.text(0.5, 0,'Loss=%.4f' % loss.data.item(), fontdict={'size': 20, 'color':  'red'})
plt.show()
```
```python
####结果如下####
[1/1000] loss:4.2052
[201/1000] loss:0.2690
[401/1000] loss:0.0925
[601/1000] loss:0.0810
[801/1000] loss:0.0802
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628423706944-5cdb6df6-a398-4a00-b9b6-847cbb748802.png#clientId=ud3a82cc3-5faf-4&from=paste&id=uf03a93ae&originHeight=251&originWidth=410&originalType=url&ratio=1&size=15488&status=done&style=shadow&taskId=ud2065095-07f4-4a65-bcc6-ee8d9d32ff6)
```python
[w, b] = model.parameters()
print(w,b)
# Parameter containing:
tensor([[2.0036]], requires_grad=True) Parameter containing:
tensor([0.7006], requires_grad=True)
```
这里的b=0.7，等于`0.2 + torch.rand(x.size())`，经过大量的训练`torch.rand()`一般约等于0.5。
<a name="o1fVC"></a>
## 附录-代码文件
[PytorchSample.ipynb](https://www.yuque.com/attachments/yuque/0/2021/ipynb/396745/1628423795747-03ac487a-1186-40bc-8028-537f07383610.ipynb?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fipynb%2F396745%2F1628423795747-03ac487a-1186-40bc-8028-537f07383610.ipynb%22%2C%22name%22%3A%22PytorchSample.ipynb%22%2C%22size%22%3A31400%2C%22type%22%3A%22%22%2C%22ext%22%3A%22ipynb%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22u2ad95a7f-4bcf-4c18-a02c-67e44f808d0%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22ua786f0b5%22%2C%22card%22%3A%22file%22%7D)
