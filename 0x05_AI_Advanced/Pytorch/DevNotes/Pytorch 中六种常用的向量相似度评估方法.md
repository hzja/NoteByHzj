PyTorch<br />提供几个pytorch中常用的向量相似度评估方法，并给出其源码实现，供大家参考。分别为以下六个。

1. CosineSimilarity
2. DotProductSimilarity 
3. ProjectedDotProductSimilarity 
4. BiLinearSimilarity 
5. TriLinearSimilarity 
6. MultiHeadedSimilarity
<a name="BCXDD"></a>
### 1、余弦相似度
余弦相似度用向量空间中两个向量夹角的余弦值作为衡量两个个体间差异的大小。余弦值越接近1，就表明夹角越接近0度，也就是两个向量越相似，称为"余弦相似性"
```python
import torch
import torch.nn as nn
import math


class CosineSimilarity(nn.Module):
 
    def forward(self, tensor_1, tensor_2):
        normalized_tensor_1 = tensor_1 / tensor_1.norm(dim=-1, keepdim=True)
        normalized_tensor_2 = tensor_2 / tensor_2.norm(dim=-1, keepdim=True)
        return (normalized_tensor_1 * normalized_tensor_2).sum(dim=-1)
```
<a name="Ql9Lf"></a>
### 2、DotProductSimilarity 
这个相似度函数简单地计算每对向量之间的点积，并使用可选的缩放来减少输出的方差。
```python
class DotProductSimilarity(nn.Module):
 
    def __init__(self, scale_output=False):
        super(DotProductSimilarity, self).__init__()
        self.scale_output = scale_output
 
    def forward(self, tensor_1, tensor_2):
        result = (tensor_1 * tensor_2).sum(dim=-1)
        if self.scale_output:
            # TODO why allennlp do multiplication at here ?
            result /= math.sqrt(tensor_1.size(-1))
        return result
```
<a name="UzPYC"></a>
### 3、ProjectedDotProductSimilarity 
这个相似度函数做一个投影，然后计算点积，计算公式为：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653190546505-41cf901c-15c4-473f-bc18-98455b6ef764.png#clientId=u7c6ba465-e96f-4&from=paste&id=u941c0673&originHeight=42&originWidth=120&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue20c4d37-cdca-42e1-a97a-3ab23c5538d&title=)<br />计算后的激活函数。默认为不激活。
```python
class ProjectedDotProductSimilarity(nn.Module):
   
    def __init__(self, tensor_1_dim, tensor_2_dim, projected_dim,
                 reuse_weight=False, bias=False, activation=None):
        super(ProjectedDotProductSimilarity, self).__init__()
        self.reuse_weight = reuse_weight
        self.projecting_weight_1 = nn.Parameter(torch.Tensor(tensor_1_dim, projected_dim))
        if self.reuse_weight:
            if tensor_1_dim != tensor_2_dim:
                raise ValueError('if reuse_weight=True, tensor_1_dim must equal tensor_2_dim')
        else:
            self.projecting_weight_2 = nn.Parameter(torch.Tensor(tensor_2_dim, projected_dim))
        self.bias = nn.Parameter(torch.Tensor(1)) if bias else None
        self.activation = activation
 
    def reset_parameters(self):
        nn.init.xavier_uniform_(self.projecting_weight_1)
        if not self.reuse_weight:
            nn.init.xavier_uniform_(self.projecting_weight_2)
        if self.bias is not None:
            self.bias.data.fill_(0)
 
    def forward(self, tensor_1, tensor_2):
        projected_tensor_1 = torch.matmul(tensor_1, self.projecting_weight_1)
        if self.reuse_weight:
            projected_tensor_2 = torch.matmul(tensor_2, self.projecting_weight_1)
        else:
            projected_tensor_2 = torch.matmul(tensor_2, self.projecting_weight_2)
        result = (projected_tensor_1 * projected_tensor_2).sum(dim=-1)
        if self.bias is not None:
            result = result + self.bias
        if self.activation is not None:
            result = self.activation(result)
        return result
```
<a name="NVg1Z"></a>
### 4、BiLinearSimilarity 
此相似度函数执行两个输入向量的双线性变换。这个函数有一个权重矩阵“W”和一个偏差“b”，以及两个向量之间的相似度，计算公式为：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653190546423-d083f409-73f9-4055-9cb5-16a3370688d1.png#clientId=u7c6ba465-e96f-4&from=paste&id=u4ecb1ae0&originHeight=35&originWidth=98&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f2c86de-4ce6-4420-bf4f-7c8f8b13e72&title=)<br />计算后的激活函数。 默认为不激活。
```python
class BiLinearSimilarity(nn.Module):
 
    def __init__(self, tensor_1_dim, tensor_2_dim, activation=None):
        super(BiLinearSimilarity, self).__init__()
        self.weight_matrix = nn.Parameter(torch.Tensor(tensor_1_dim, tensor_2_dim))
        self.bias = nn.Parameter(torch.Tensor(1))
        self.activation = activation
        self.reset_parameters()
 
    def reset_parameters(self):
        nn.init.xavier_uniform_(self.weight_matrix)
        self.bias.data.fill_(0)
 
    def forward(self, tensor_1, tensor_2):
        intermediate = torch.matmul(tensor_1, self.weight_matrix)
        result = (intermediate * tensor_2).sum(dim=-1) + self.bias
        if self.activation is not None:
            result = self.activation(result)
        return result
```
<a name="oUaav"></a>
### 5、TriLinearSimilarity 
此相似度函数执行两个输入向量的三线性变换，计算公式为：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653190546513-7b01290a-dce4-4128-b02e-7ac4b7da28bc.png#clientId=u7c6ba465-e96f-4&from=paste&id=u4f904753&originHeight=36&originWidth=148&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9e94ea5-b250-43ce-8d28-11537dd95f8&title=)<br />计算后的激活函数。 默认为不激活。
```python
class TriLinearSimilarity(nn.Module):
 
    def __init__(self, input_dim, activation=None):
        super(TriLinearSimilarity, self).__init__()
        self.weight_vector = nn.Parameter(torch.Tensor(3 * input_dim))
        self.bias = nn.Parameter(torch.Tensor(1))
        self.activation = activation
        self.reset_parameters()
 
    def reset_parameters(self):
        std = math.sqrt(6 / (self.weight_vector.size(0) + 1))
        self.weight_vector.data.uniform_(-std, std)
        self.bias.data.fill_(0)
 
    def forward(self, tensor_1, tensor_2):
        combined_tensors = torch.cat([tensor_1, tensor_2, tensor_1 * tensor_2], dim=-1)
        result = torch.matmul(combined_tensors, self.weight_vector) + self.bias
        if self.activation is not None:
            result = self.activation(result)
        return result
```
<a name="AgY1c"></a>
### 6、MultiHeadedSimilarity
这个相似度函数使用多个“头”来计算相似度。也就是说，将输入张量投影到多个新张量中，并分别计算每个投影张量的相似度。这里的结果比典型的相似度函数多一个维度。
```python
class MultiHeadedSimilarity(nn.Module):
 
    def __init__(self,
                 num_heads,
                 tensor_1_dim,
                 tensor_1_projected_dim=None,
                 tensor_2_dim=None,
                 tensor_2_projected_dim=None,
                 internal_similarity=DotProductSimilarity()):
        super(MultiHeadedSimilarity, self).__init__()
        self.num_heads = num_heads
        self.internal_similarity = internal_similarity
        tensor_1_projected_dim = tensor_1_projected_dim or tensor_1_dim
        tensor_2_dim = tensor_2_dim or tensor_1_dim
        tensor_2_projected_dim = tensor_2_projected_dim or tensor_2_dim
        if tensor_1_projected_dim % num_heads != 0:
            raise ValueError("Projected dimension not divisible by number of heads: %d, %d"
                             % (tensor_1_projected_dim, num_heads))
        if tensor_2_projected_dim % num_heads != 0:
            raise ValueError("Projected dimension not divisible by number of heads: %d, %d"
                             % (tensor_2_projected_dim, num_heads))
        self.tensor_1_projection = nn.Parameter(torch.Tensor(tensor_1_dim, tensor_1_projected_dim))
        self.tensor_2_projection = nn.Parameter(torch.Tensor(tensor_2_dim, tensor_2_projected_dim))
        self.reset_parameters()
 
    def reset_parameters(self):
        torch.nn.init.xavier_uniform_(self.tensor_1_projection)
        torch.nn.init.xavier_uniform_(self.tensor_2_projection)
 
    def forward(self, tensor_1, tensor_2):
        projected_tensor_1 = torch.matmul(tensor_1, self.tensor_1_projection)
        projected_tensor_2 = torch.matmul(tensor_2, self.tensor_2_projection)
 
        # Here we split the last dimension of the tensors from (..., projected_dim) to
        # (..., num_heads, projected_dim / num_heads), using tensor.view().
        last_dim_size = projected_tensor_1.size(-1) // self.num_heads
        new_shape = list(projected_tensor_1.size())[:-1] + [self.num_heads, last_dim_size]
        split_tensor_1 = projected_tensor_1.view(*new_shape)
        last_dim_size = projected_tensor_2.size(-1) // self.num_heads
        new_shape = list(projected_tensor_2.size())[:-1] + [self.num_heads, last_dim_size]
        split_tensor_2 = projected_tensor_2.view(*new_shape)
 
        # And then we pass this off to our internal similarity function. Because the similarity
        # functions don't care what dimension their input has, and only look at the last dimension,
        # we don't need to do anything special here. It will just compute similarity on the
        # projection dimension for each head, returning a tensor of shape (..., num_heads).
        return self.internal_similarity(split_tensor_1, split_tensor_2)
```
