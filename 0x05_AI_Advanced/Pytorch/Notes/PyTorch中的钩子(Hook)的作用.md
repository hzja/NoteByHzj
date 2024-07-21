PyTorch<br />首先明确一点，**有哪些hook？**<br />看到的有3个：

- `torch.autograd.Variable.register_hook` (Python method, in Automatic differentiation package)
- `torch.nn.Module.register_backward_hook` (Python method, in torch.nn)
- `torch.nn.Module.register_forward_hook`

第一个是register_hook，是**针对Variable对象**的，后面的两个：register_backward_hook和register_forward_hook是**针对**`**nn.Module**`这个对象的。<br />其次，明确一下，**为什么需要用hook?**<br />打个比方，有这么个函数：<br />![](https://cdn.nlark.com/yuque/__latex/04b3ac4e9f5b3d819832c8b4881bb00e.svg#card=math&code=x%20%5Cin%20%5Cmathbb%7BR%7D%5E%7B2%7D%2C%20%5Cquad%20y%3Dx%2B2%2C%20%5Cquad%20z%3D%5Cfrac%7B1%7D%7B2%7D%5Cleft%28y_%7B1%7D%5E%7B2%7D%2By_%7B2%7D%5E%7B2%7D%5Cright%29&id=leGgY)<br />![2022-06-28-08-24-30-525983.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656375902447-af0745cb-ec14-4ffe-8bfb-445fd52edceb.png#clientId=uc8213cbf-fda1-4&from=ui&id=u7dd72563&originHeight=104&originWidth=500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=156441&status=done&style=none&taskId=u72925570-8264-4ec4-9567-1326ce1b609&title=)<br />想通过梯度下降法求最小值。在PyTorch里面很容易实现，只需要：
```python
import torch
from torch.autograd import Variable

x = Variable(torch.randn(2, 1), requires_grad=True)
y = x+2
z = torch.mean(torch.pow(y, 2))
lr = 1e-3
z.backward()
x.data -= lr*x.grad.data
```
但问题是，如果想要求中间变量 y 的梯度，系统会返回错误。<br />事实上，如果输入：
```python
type(y.grad)
```
系统会提示：NoneType<br />这个问题在PyTorch的论坛上有人提问过，开发者说是因为当初开发时设计的是，**对于中间变量**，**一旦它们完成了自身反传的使命，就会被释放掉**。<br />因此，hook就派上用场了。简而言之，**register_hook的作用**是，**当反传时，除了完成原有的反传，额外多完成一些任务**。可以**定义一个中间变量的hook**，将它的**grad值打印**出来，当然也可以定义一个全局列表，将每次的**grad值添加**到里面去。
```python
import torch
from torch.autograd import Variable

grad_list = []

def print_grad(grad):
    grad_list.append(grad)

x = Variable(torch.randn(2, 1), requires_grad=True)
y = x+2
z = torch.mean(torch.pow(y, 2))
lr = 1e-3
y.register_hook(print_grad)
z.backward()
x.data -= lr*x.grad.data
```
需要注意的是，**register_hook函数接收的是一个函数**，这个函数有如下的形式：
```python
hook(grad) -> Variable or None
```
也就是说，这个函数是拥有改变梯度值的威力的！<br />至于`register_forward_hook`和`register_backward_hook`的用法和这个大同小异。只不过对象从Variable改成了自己定义的`nn.Module`。<br />当训练一个网络，想要提取中间层的参数、或者特征图的时候，使用hook就能派上用场了。<br />参考资料：

- [https://discuss.pytorch.org/t/extract-feature-maps-from-intermediate-layers-without-modifying-forward/1390](https://discuss.pytorch.org/t/extract-feature-maps-from-intermediate-layers-without-modifying-forward/1390)
- [https://discuss.pytorch.org/t/why-cant-i-see-grad-of-an-intermediate-variable/94](https://discuss.pytorch.org/t/why-cant-i-see-grad-of-an-intermediate-variable/94)
