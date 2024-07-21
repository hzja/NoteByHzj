PyTorch<br />PyTorch是一个基于Python的科学包，用于使用一种称为张量的特殊数据类型执行高级操作。张量是具有规则形状和相同数据类型的数字、向量、矩阵或多维数组。PyTorch是NumPy包的另一种选择，它可以在GPU下使用。它也被用作进行深度学习研究的框架。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658457511532-60569b4f-9461-4603-a35f-dd8d86edb11b.jpeg#clientId=u01d222b2-a9fe-4&from=paste&id=u1f29219a&originHeight=705&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue06f7a68-8eca-462c-8bbf-f22b3ef3544&title=)<br />这5个操作是：

- `expand()`
- `permute()`
- `tolist()`
- `narrow()`
- `where()`
<a name="LaY3Y"></a>
## 1、`expand()`
将现有张量沿着值为1的维度扩展到新的维度。张量可以同时沿着任意一维或多维展开。如果不想沿着一个特定的维度展开张量，可以设置它的参数值为-1。<br />注意：只能扩展单个维度
```python
# Example 1 - working 
a=torch.tensor([[[1,2,3],[4,5,6]]])
a.size()
>>torch.Size([1, 2, 3])

a.expand(2,2,3)
>>tensor([[[1, 2, 3],
         [4, 5, 6]],

        [[1, 2, 3],
         [4, 5, 6]]])
```
在这个例子中，张量的原始维数是[1,2,3]。它被扩展到[2,2,3]。
<a name="zpMh1"></a>
## 2、`permute()`
这个函数返回一个张量的视图，原始张量的维数根据选择而改变。例如，如果原来的维数是[1,2,3]，可以将它改为[3,2,1]。该函数以所需的维数顺序作为参数。
```python
# Example 1 - working
a=torch.tensor([[[1,2,3],[4,5,6]]])
a.size()
>>torch.Size([1, 2, 3])

a.permute(2,1,0).size()
>>torch.Size([3, 2, 1])

a.permute(2,1,0)
>>tensor([[[1],
         [4]],

        [[2],
         [5]],

        [[3],
         [6]]])
```
在这个例子中，原始张量的维度是[1,2,3]。使用permuting，将顺序设置为(2,1,0)，这意味着新的维度应该是[3,2,1]。如图所示，张量的新视图重新排列了数字，使得张量的维度为[3,2,1]。<br />当想要对不同维数的张量进行重新排序，或者用不同阶数的矩阵进行矩阵乘法时，可以使用这个函数。
<a name="qHDZq"></a>
## 3、`tolist()`
这个函数以Python数字、列表或嵌套列表的形式返回张量。在此之后，可以对它执行任何python逻辑和操作。
```python
# Example 1 - working
a=torch.tensor([[1,2,3],[4,5,6]])
a.tolist()
>> [[1, 2, 3], [4, 5, 6]]
```
在这个例子中，张量以嵌套列表的形式返回。
<a name="TXCiH"></a>
## 4、`narrow()`
这个函数返回一个新的张量，这个张量是原来张量的缩小版。这个函数的参数是输入张量、要缩小的维数、起始索引和新张量沿该维数的长度。它返回从索引start到索引(start+length-1)中的元素。
```python
# Example 1 - working
a=torch.tensor([[1,2,3,4],[5,6,7,8],[9,10,11,12],[14,15,16,17]])
torch.narrow(a,1,2,2)
>> tensor([[ 3,  4],
        [ 7,  8],
        [11, 12],
        [16, 17]])
```
在这个例子中，张量要沿着第2维，也就是最里面的维度缩小。它接受列表中的元素，从索引2开始，到索引3(=2+2 -1，即start+length-1)。<br />`Narrow()`的工作原理类似于高级索引。例如，在一个2D张量中，使用[:，0:5]选择列0到5中的所有行。同样的，可以使用`torch.narrow(1,0,5)`。然而，在高维张量中，对于每个维度都使用range操作是很麻烦的。使用`narrow()`可以更快更方便地实现这一点。
<a name="Qd9aH"></a>
## 5、`where()`
这个函数返回一个新的张量，其值在每个索引处都根据给定条件改变。这个函数的参数有：条件，第一个张量和第二个张量。在每个张量的值上检查条件(在条件中使用)，如果为真，就用第一个张量中相同位置的值代替，如果为假，就用第二个张量中相同位置的值代替。
```python
# Example 1 - working
a=torch.tensor([[[1,2,3],[4,5,6]]]).to(torch.float32)
b=torch.zeros(1,2,3)
torch.where(a%2==0,b,a)
>>tensor([[[1., 0., 3.],
         [0., 5., 0.]]])
```
这里，它检查张量a的值是否是偶数。如果是，则用张量b中的值替换，b中的值都是0，否则还是和原来一样。<br />此函数可用于设定阈值。如果张量中的值大于或小于某一数值，它们可以很容易地被替换。
