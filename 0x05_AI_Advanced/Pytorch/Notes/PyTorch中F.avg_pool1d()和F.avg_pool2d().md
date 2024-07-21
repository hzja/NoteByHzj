PyTorch
<a name="BUxna"></a>
## `F.avg_pool1d()`
`F.avg_pool1d()`数据是三维输入：

- input维度：（batch_size,channels,width）channel可以看成高度
- kenerl维度：（一维:表示width的跨度）channel和输入的channel一致可以认为是矩阵的高度

假设`kernel_size=2`，则每俩列相加求平均，stride默认和kernel_size保持一致，越界则丢弃（下面表示1，2列和3，4列相加求平均）：
```python
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input)
m = F.avg_pool1d(input,kernel_size=2)
m

tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[1.0000, 1.0000],
         [1.0000, 1.0000],
         [0.0000, 0.5000],
         [1.0000, 1.0000],
         [1.0000, 1.0000]]])
```
假设`kenerl_size=3`，表示前3列相加求平均，后面的不足3列丢弃：
```python
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input)
m = F.avg_pool1d(input,kernel_size=3)
m
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[1.],
         [1.],
         [0.],
         [1.],
         [1.]]])

input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input)
m = F.avg_pool1d(input,kernel_size=4)
m
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[1.0000],
         [1.0000],
         [0.2500],
         [1.0000],
         [1.0000]]])
```
假设stride=1每次移动一个步伐：
```python
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input)
m = F.avg_pool1d(input,kernel_size=2,stride=1)
m
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[1.0000, 1.0000, 1.0000, 1.0000],
         [1.0000, 1.0000, 1.0000, 1.0000],
         [0.0000, 0.0000, 0.5000, 1.0000],
         [1.0000, 1.0000, 1.0000, 1.0000],
         [1.0000, 1.0000, 1.0000, 1.0000]]])
 
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input)
m = F.avg_pool1d(input,kernel_size=4,stride=1)
m
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[1.0000, 1.0000],
         [1.0000, 1.0000],
         [0.2500, 0.5000],
         [1.0000, 1.0000],
         [1.0000, 1.0000]]])
```
<a name="TOH4k"></a>
## `F.avg_pool2d()`
`F.avg_pool2d()`数据是四维输入：

- input维度：(batch_size,channels,height,width）
- kenerl维度：（二维：表示width的跨度）channel和输入的channle一致，如果数据是三维，则channel为1.(如果只写一个数n，`kenerl=(n,n)`)

stride默认和kenerl一致，这是个二维的，所以在height和width上均和kenerl一致，越界同样丢弃。<br />跟cnn卷积一致。
```python
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input.size())
print(input)
m = F.avg_pool2d(input,kernel_size=(4,4))
m
torch.Size([1, 5, 5])
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[0.8125]]])

input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input.size())
print(input)
m = F.avg_pool2d(input,kernel_size=(4,4),stride=1)
m
torch.Size([1, 5, 5])
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[0.8125, 0.8750],
         [0.8125, 0.8750]]])
```
如果求列的平均`kenerl=(1,5)`，此时默认`stride=(1,5)`：
```python
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input.size())
print(input)
m = F.avg_pool2d(input,kernel_size=(1,5))
m
torch.Size([1, 5, 5])
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[1.0000],
         [1.0000],
         [0.4000],
         [1.0000],
         [1.0000]]])
```
如果求行的平均`kenerl=(5,1)`，此时默认`stride=(5,1)`，用卷积的概念取思考：
```python
input = torch.tensor([[1,1,1,1,1],[1,1,1,1,1],[0,0,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]).unsqueeze(0).float()
print(input.size())
print(input)
m = F.avg_pool2d(input,kernel_size=(5,1))
m
torch.Size([1, 5, 5])
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [0., 0., 0., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
tensor([[[0.8000, 0.8000, 0.8000, 1.0000, 1.0000]]])
```
对于四维的数据，channel默认和输入一致：
```python
input=torch.randn(10,3,4,4)
m=F.avg_pool2d(input,(4,4))
print(m.size())

torch.Size([10, 3, 1, 1])
```
