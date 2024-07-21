Tensorflow2
<a name="Suym6"></a>
## 合并与分割
<a name="HJbK8"></a>
### `tf.concat`
`tf.concat`可以实现拼接操作.<br />格式:
```python
tf.concat(
    values, axis, name='concat'
)
```
参数:

- `values`: 一个 tensor 或 tensor list
- `axis`: 操作的维度
- `name`: 数据名称, 默认为 “`concat`”

例子:
```python
part_1 = tf.zeros([5, 3])
print(part_1)

part_2 = tf.ones([5, 3])
print(part_2)

# 竖向拼接
result_1 = tf.concat([part_1, part_2], axis=0)
print(result_1)

# 横向拼接
result_2 = tf.concat([part_1, part_2], axis=1)
print(result_2)
```
输出结果:
```python
tf.Tensor(
[[0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]], shape=(5, 3), dtype=float32)
tf.Tensor(
[[1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]], shape=(5, 3), dtype=float32)
tf.Tensor(
[[0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]], shape=(10, 3), dtype=float32)
tf.Tensor(
[[0. 0. 0. 1. 1. 1.]
 [0. 0. 0. 1. 1. 1.]
 [0. 0. 0. 1. 1. 1.]
 [0. 0. 0. 1. 1. 1.]
 [0. 0. 0. 1. 1. 1.]], shape=(5, 6), dtype=float32)
```
<a name="FFcXD"></a>
### `tf.stack`
`tf.stack`可以创建一个新的维度来合并两个张量.<br />格式:
```python
tf.stack(
    values, axis=0, name='stack'
)
```
参数:

- `values`: 一个 tensor list
- `axis`: 操作的维度
- `name`: 数据名称, 默认为 “`stack`”

例子:
```python
part_1 = tf.zeros([5, 3])
print(part_1)

part_2 = tf.ones([5, 3])
print(part_2)

# 头拼接
result_1 = tf.stack([part_1, part_2], axis=0)
print(result_1)

# 尾拼接
result_2 = tf.stack([part_1, part_2], axis=2)
print(result_2)
```
输出结果:
```python
tf.Tensor(
[[0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]], shape=(5, 3), dtype=float32)
tf.Tensor(
[[1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]], shape=(5, 3), dtype=float32)
tf.Tensor(
[[[0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]]

 [[1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]]], shape=(2, 5, 3), dtype=float32)
tf.Tensor(
[[[0. 1.]
  [0. 1.]
  [0. 1.]]

 [[0. 1.]
  [0. 1.]
  [0. 1.]]

 [[0. 1.]
  [0. 1.]
  [0. 1.]]

 [[0. 1.]
  [0. 1.]
  [0. 1.]]

 [[0. 1.]
  [0. 1.]
  [0. 1.]]], shape=(5, 3, 2), dtype=float32)
```
<a name="RiVb0"></a>
### `tf.unstack`
`tf.unstack`是一个矩阵分解函数.格式:
```python
# unstack
tf.unstack(
    value, num=None, axis=0, name='unstack'
)
```
参数:

- `values`: 一个 tensor, 维度大于 0
- `num`: 轴的长度
- `axis`: 操作的维度
- `name`: 数据名称, 默认为 “unstack”

例子:
```python
a = tf.stack([tf.zeros([5, 3]), tf.ones([5, 3])], axis=0)
print(a)

b = tf.unstack(a, axis=0)
print(b)
```
输出结果:
```python
tf.Tensor(
[[[0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]]

 [[1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]]], shape=(2, 5, 3), dtype=float32)
[&lt;tf.Tensor: shape=(5, 3), dtype=float32, numpy=
array([[0., 0., 0.],
       [0., 0., 0.],
       [0., 0., 0.],
       [0., 0., 0.],
       [0., 0., 0.]], dtype=float32)&gt;, &lt;tf.Tensor: shape=(5, 3), dtype=float32, numpy=
array([[1., 1., 1.],
       [1., 1., 1.],
       [1., 1., 1.],
       [1., 1., 1.],
       [1., 1., 1.]], dtype=float32)&gt;]
```
<a name="KYfj1"></a>
### `tf.split`
`tf.split()`可以把一个张量划分为几个子张量.<br />格式:
```python
tf.split(
    value, num_or_size_splits, axis=0, num=None, name='split'
)
```
参数:

- `value`: 待切分的张量
- `num_or_size_splits`: 切成几份
- `axis`: 操作的维度
- `num`: `num_or_size_splits` 不能实现的情况下使用
- `name`: 数据名称, 默认为 “`split`”

例子:
```python
# split
a = tf.stack([tf.zeros([5, 3]), tf.ones([5, 3])], axis=0)
print(a)

b = tf.split(a, 2)
print(b)
```
输出结果:
```python
tf.Tensor(
[[[0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]]

 [[1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]
  [1. 1. 1.]]], shape=(2, 5, 3), dtype=float32)
[&lt;tf.Tensor: shape=(1, 5, 3), dtype=float32, numpy=
array([[[0., 0., 0.],
        [0., 0., 0.],
        [0., 0., 0.],
        [0., 0., 0.],
        [0., 0., 0.]]], dtype=float32)&gt;, &lt;tf.Tensor: shape=(1, 5, 3), dtype=float32, numpy=
array([[[1., 1., 1.],
        [1., 1., 1.],
        [1., 1., 1.],
        [1., 1., 1.],
        [1., 1., 1.]]], dtype=float32)&gt;]
```
<a name="Yf0V6"></a>
## 数据统计
<a name="Jsqyu"></a>
### `tf.norm`
`tf.norm`可以计算向量, 矩阵, 张量的范数.<br />格式:
```python
tf.norm(
    tensor, ord='euclidean', axis=None, keepdims=None, name=None
)
```
参数:

- `tensor`: 输入的张量
- `ord`: 范数的顺序
- `axis`: 操作的维度
- `keep_dims`: 如果为 `True`, 则 `axis` 中指定的轴将保持为大小 1
- `name`: 数据名称

例子:
```python
a = tf.fill([2, 2], 2.0)
print(a)

# sqrt(2^2 * 4) = sqrt(16) = 4
b = tf.norm(a)
print(b)

# [2 + 2, 2 + 2] = [4, 4]
c = tf.norm(a, ord=1, axis= 0)
print(c)

# [sqrt(2^2 + 2^2), sqrt(2^2 + 2^2)] = [sqrt(8), sqrt(8)]
d = tf.norm(a, ord=2, axis= 0)
print(d)
```
输出结果:
```python
tf.Tensor(
[[2. 2.]
 [2. 2.]], shape=(2, 2), dtype=float32)
tf.Tensor(4.0, shape=(), dtype=float32)
tf.Tensor([4. 4.], shape=(2,), dtype=float32)
tf.Tensor([2.828427 2.828427], shape=(2,), dtype=float32)
```
<a name="HpEzs"></a>
### `reduce_min`/`max`/`mean`
计算一个张量各个维度上元素的最小值 / 最大值 / 平均值.<br />格式:
```python
tf.math.reduce_min / reduce_max / reduce_mean(
    input_tensor, axis=None, keepdims=False, name=None
)
```
参数:

- `input_tensor`: 传入的张量
- `axis`: 维度, 默认计算所有维度
- `keepdims`: 如果为真保留维度, 默认为 `False`
- `name`: 数据名称

例子:
```python
a = tf.reshape(tf.range(9), [3, 3])
print(a)

min = tf.reduce_min(a)
print(min)

max = tf.reduce_max(a)
print(max)
```
输出结果:
```python
tf.Tensor(
[[0 1 2]
 [3 4 5]
 [6 7 8]], shape=(3, 3), dtype=int32)
tf.Tensor(0, shape=(), dtype=int32)
tf.Tensor(8, shape=(), dtype=int32)
```
<a name="UCgbJ"></a>
### `argmax` / `argmin`
`tf.argmax`/`tf.argmin`可以找到最大 / 最小值所在的索引 (index).<br />格式:
```python
tf.math.argmax(
    input, axis=None, output_type=tf.dtypes.int64, name=None
)
```
参数:

- `input`: 输入
- `axis`: 操作的维度
- `output_type`: 输出数据类型, 默认为 int64
- `name`: 数据名称

例子:
```python
# argmax / argmin
a = tf.reshape(tf.range(9), [3, 3])
print(a)

max = tf.argmax(a)
print(max)

min = tf.argmin(a)
print(min)
```
输出结果:
```python
tf.Tensor(
[[0 1 2]
 [3 4 5]
 [6 7 8]], shape=(3, 3), dtype=int32)
tf.Tensor([2 2 2], shape=(3,), dtype=int64)
tf.Tensor([0 0 0], shape=(3,), dtype=int64)
```
<a name="bU20i"></a>
### `tf.equal`
`tf.equal`可以判断两个张量是否相等, 返回 True / False.<br />格式:
```python
tf.math.equal(
    x, y, name=None
)
```
例子:
```python
a = tf.zeros(5, dtype=tf.float32)
print(a)

b = tf.range(5, dtype=tf.float32)
print(b)

print(tf.equal(a, b))
```
输出结果:
```python
tf.Tensor([0. 0. 0. 0. 0.], shape=(5,), dtype=float32)
tf.Tensor([0. 1. 2. 3. 4.], shape=(5,), dtype=float32)
tf.Tensor([ True False False False False], shape=(5,), dtype=bool)
```
<a name="eGTvc"></a>
### `tf.unique`
`tf.unique`可以找出张量中不重复的值<br />格式:
```python
tf.unique(
    x, out_idx=tf.dtypes.int32, name=None
)
```
参数:

- `input`: 输入
- `output_type`: 输出数据类型, 默认为 int32
- `name`: 数据名称

例子:
```python
a = tf.range(5)
print(tf.unique(a))

b = tf.constant([4, 2, 2, 4, 3])
print(tf.unique(b))
```
输出结果:
```python
Unique(y=&lt;tf.Tensor: shape=(5,), dtype=int32, numpy=array([0, 1, 2, 3, 4])&gt;, idx=&lt;tf.Tensor: shape=(5,), dtype=int32, numpy=array([0, 1, 2, 3, 4])&gt;)
Unique(y=&lt;tf.Tensor: shape=(3,), dtype=int32, numpy=array([4, 2, 3])&gt;, idx=&lt;tf.Tensor: shape=(5,), dtype=int32, numpy=array([0, 1, 1, 0, 2])&gt;)
```
