PyTorch<br />`torch.roll(input, shifts, dims=None)`：

- `input` (Tensor) –输入tensor
- `shifts` (int or tuple of python:ints) – 变换的幅度，为整数或者元组。若为元组，其shape与dims保持一样
- `dims` (int or tuple of python:ints) – 维度。在dims维上进行大小为shift的变换。0 为行向，1为列向
```python
import torch

tensor = torch.tensor(torch.range(1, 9).reshape(3, 3))
print(tensor)
# tensor([[1., 2., 3.],
#         [4., 5., 6.],
#         [7., 8., 9.]])

# 沿着 dim 0 上移一行
torch.roll(tensor, -1, 0)
# tensor([[4., 5., 6.],
#         [7., 8., 9.],
#         [1., 2., 3.]])

# 沿着 dim 0 下移一行
torch.roll(tensor, 1, 0)
# tensor([[7., 8., 9.],
#         [1., 2., 3.],
#         [4., 5., 6.]])

# 沿着 dim 1 左移一行
torch.roll(tensor, -1, 1)
# tensor([[2., 3., 1.],
#         [5., 6., 4.],
#         [8., 9., 7.]])

# 沿着 dim 1 右移一行
torch.roll(tensor, 1, 1)
# tensor([[3., 1., 2.],
#         [6., 4., 5.],
#         [9., 7., 8.]])

# 沿着 dim 0 上移两行
torch.roll(tensor, -2, 0)

# 沿着 dim 0 上移一行， 再沿着 dim 0 上移一行
torch.roll(tensor, (-1, -1), (0, 0))
# tensor([[7., 8., 9.],
#         [1., 2., 3.],
#         [4., 5., 6.]])
```
