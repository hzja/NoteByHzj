TensorFlow2
<a name="W2w88"></a>
## 张量排序
<a name="bJ7S0"></a>
### `tf.sort`
`tf.sort`函数可以对张量进行排序.<br />格式:
```python
tf.sort(
    values, axis=-1, direction='ASCENDING', name=None
)
```
参数:

- `values`: 要进行排序的张量
- `axis`: 操作维度
- `direction`: 正序或者倒序
- `name`: 数据名称

例子:
```python
# 创建张量0~9, 并打乱顺序
a = tf.random.shuffle(tf.range(10))
print(a)

# 从小到大
b = tf.sort(a)  # direction="ASCENDING"
print(b)

# 从大到小
c = tf.sort(a, direction="DESCENDING")
print(c)
```
输出结果:
```python
tf.Tensor([6 3 7 5 4 0 2 9 8 1], shape=(10,), dtype=int32)
tf.Tensor([0 1 2 3 4 5 6 7 8 9], shape=(10,), dtype=int32)
tf.Tensor([9 8 7 6 5 4 3 2 1 0], shape=(10,), dtype=int32)
```
<a name="xGhhi"></a>
### `tf.argsort`
`tf.argsort`返回张量的索引排序, 沿给的轴排序.<br />格式:
```python
tf.argsort(
    values, axis=-1, direction='ASCENDING', stable=False, name=None
)
```
参数:

- 要进行排序的张量
- `axis`: 操作维度
- `direction`: 正序或者倒序
- `stable`: 如果为 True, 则原始张量中的相等元素将不会按返回的顺序重新排序
- `name`: 数据名称

例子:
```python
# 创建张量0~9, 并打乱顺序
a = tf.random.shuffle(tf.range(10))
print(a)

# 从小到大
b = tf.argsort (a)
print(b)

# 从大到小
c = tf.argsort (a, direction="DESCENDING")
print(c)
```
输出结果:
```python
tf.Tensor([9 4 3 1 2 6 0 5 7 8], shape=(10,), dtype=int32)
tf.Tensor([6 3 4 2 1 7 5 8 9 0], shape=(10,), dtype=int32)
tf.Tensor([0 9 8 5 7 1 2 4 3 6], shape=(10,), dtype=int32)
```
<a name="WUkxK"></a>
### `tf.math.top_k`
`tf.math.top_k`可以查找最后一个维度的 k 个最大条目的值和索引.<br />格式:
```python
tf.math.top_k(
    input, k=1, sorted=True, name=None
)
```
参数:

- `input`: 传入张量
- `k=1`: 前 k 位
- `sorted`: 是否排序
- `name`: 数据名称

例子:
```python
# 创建张量0~9, 并打乱顺序, 形状为 3*3
a = tf.reshape(tf.random.shuffle(tf.range(9)), [3, 3])
print(a)

# 取top2
b = tf.math.top_k(a, 2)
print(b)
```
输出结果:
```python
tf.Tensor(
[[2 1 4]
 [5 7 0]
 [8 6 3]], shape=(3, 3), dtype=int32)
TopKV2(values=&lt;tf.Tensor: shape=(3, 2), dtype=int32, numpy=
array([[4, 2],
       [7, 5],
       [8, 6]])&gt;, indices=&lt;tf.Tensor: shape=(3, 2), dtype=int32, numpy=
array([[2, 0],
       [1, 0],
       [0, 1]])&gt;)
```
<a name="qQE7e"></a>
## 填充与复制
<a name="niuXQ"></a>
### `tf.pad`
`tf.pad`可以对一个 tensor 四周进行填充.<br />![2021-12-19-18-12-40-942041.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639909518170-4aa3229f-93b7-4e60-a48a-1fcd016fd28a.png#clientId=u3eab3bc0-eb49-4&from=ui&id=u08f37a48&originHeight=268&originWidth=597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=481111&status=done&style=shadow&taskId=ufb6bb241-6905-425c-b542-37be49f7f7d&title=)<br />格式:
```python
tf.pad(
    tensor, paddings, mode='CONSTANT', constant_values=0, name=None
)

```
参数:

- `tensor`: 传入的张量
- `paddings`: 要扩展的维度
- `mode`: 模式, 默认为 “CONSTANT”
- `constant_value`: 在 “CONSTANT” 模式下, 要使用的标量填充值 (必须与张量类型相同)
- `name`: 数据名称

例子:
```python
# pad
a = tf.reshape(tf.range(9), [3, 3])
print(a)

# 上下左右填充一圈0
b = tf.pad(a, [[1, 1], [1, 1]])
print(b)
```
输出结果:
```python
tf.Tensor(
[[0 1 2]
 [3 4 5]
 [6 7 8]], shape=(3, 3), dtype=int32)
tf.Tensor(
[[0 0 0 0 0]
 [0 0 1 2 0]
 [0 3 4 5 0]
 [0 6 7 8 0]
 [0 0 0 0 0]], shape=(5, 5), dtype=int32)
```
<a name="ZFqtD"></a>
### `tf.tile`
`tf.tile`可以实现 tensor 的复制.<br />格式:
```python
tf.tile(
    input, multiples, name=None
)
```
参数:

- `input`: 传入的张量
- `multiples`: 复制的次数
- `name`: 数据名称

例子:
```python
# tile
a = tf.reshape(tf.range(9), [3, 3])
print(a)

b = tf.tile(a, [2, 2])
print(b)
```
输出结果:
```python
tf.Tensor(
[[0 1 2]
 [3 4 5]
 [6 7 8]], shape=(3, 3), dtype=int32)
tf.Tensor(
[[0 1 2 0 1 2]
 [3 4 5 3 4 5]
 [6 7 8 6 7 8]
 [0 1 2 0 1 2]
 [3 4 5 3 4 5]
 [6 7 8 6 7 8]], shape=(6, 6), dtype=int32)
```
<a name="H9hOy"></a>
## 查找与替换
<a name="WPgdS"></a>
### `tf.where` (第一种)
返回元素为 True 的位置.<br />格式:
```python
tf.where(
    condition, name=None
)
```
参数:

- `condition`: 判断条件
- `name`: 数据名称

例子:
```python
# 第一种用法(单参数)
mask = tf.constant([[True, True, True], [False, True, True], [True, False, False]])
print(mask)

indices = tf.where(mask)
print(indices)
```
输出结果:
```python
tf.Tensor(
[[ True  True  True]
 [False  True  True]
 [ True False False]], shape=(3, 3), dtype=bool)
tf.Tensor(
[[0 0]
 [0 1]
 [0 2]
 [1 1]
 [1 2]
 [2 0]], shape=(6, 2), dtype=int64)
```
<a name="qp5zh"></a>
### `tf.where` (第二种)
类似三元运算符的用法.<br />格式:
```python
tf.where(
    condition, x=None, y=None, name=None
)
```
参数:

- `condition`: 判断条件
- `x`: 如果条件为 True 赋值
- `y`: 如果条件为 False 赋值
- `name`: 数据名称

例子:
```python
# 第二种用法(三个参数)
zeros = tf.zeros([3, 3])
print(zeros)

ones = tf.ones([3, 3])
print(ones)

mask = tf.constant([[True, True, True], [False, True, True], [True, False, False]])
print(mask)

result = tf.where(mask, zeros, ones)
print(result)
```
输出结果:
```python
tf.Tensor(
[[0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]], shape=(3, 3), dtype=float32)
tf.Tensor(
[[1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]], shape=(3, 3), dtype=float32)
tf.Tensor(
[[ True  True  True]
 [False  True  True]
 [ True False False]], shape=(3, 3), dtype=bool)
tf.Tensor(
[[0. 0. 0.]
 [1. 0. 0.]
 [0. 1. 1.]], shape=(3, 3), dtype=float32)
```
<a name="V8Mu4"></a>
### `tf.scatter_nd`
使用索引更新张量.<br />![2021-12-19-18-12-41-090598.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639909077994-93481163-d89a-4946-ac20-5aef2bd76365.png#clientId=u3eab3bc0-eb49-4&from=ui&id=uba5048f3&originHeight=110&originWidth=457&originalType=binary&ratio=1&rotation=0&showTitle=false&size=151245&status=done&style=shadow&taskId=u977d530f-39ed-4ea6-b37b-ea2965485e1&title=)<br />格式:
```python
tf.scatter_nd(
    indices, updates, shape, name=None
)
```
参数:

- `indices`: 索引
- `updates`: 更新的值
- `shape`: 形状
- `name`: 数据名称

例子:
```python
# scatter_nd
indices = tf.constant([[4], [3], [1], [7]])
print(indices)

updates = tf.constant([9, 10, 11, 12])
print(updates)

shape = tf.constant([8])
print(shape)

result = tf.scatter_nd(indices, updates, shape)
print(result)
```
输出结果:
```python
tf.Tensor(
[[4]
 [3]
 [1]
 [7]], shape=(4, 1), dtype=int32)
tf.Tensor([ 9 10 11 12], shape=(4,), dtype=int32)
tf.Tensor([8], shape=(1,), dtype=int32)
tf.Tensor([ 0 11  0 10  9  0  0 12], shape=(8,), dtype=int32)
```
