Tensorflow2
<a name="AebBG"></a>
## 创建数据
详细讲解一下 TensorFlow 创建数据的集中方法.
<a name="KtIvF"></a>
### 创建常量
`tf.constant()` 格式为:
```python
tf.constant(value，dtype，shape，name)
```
参数:

- `value`: 常量值
- `dtype`: 数据类型
- `shape`: 表示生成常量数的维度
- `name`: 数据名称

例子:
```python
# 创建常量1
c1 = tf.constant(1)
print(c1)

# 创建常量, 类型为bool
c2 = tf.constant([True, False])
print(c2)

# 创建常量1, 类型为float32, 大小为3*3
c3 = tf.constant(0.1, shape=[2, 2])
print(c3)

# 创建常量, 类型为string字符串
c4 = tf.constant("Hello World!")
print(c4)
```
输出结果:
```python
tf.Tensor(1, shape=(), dtype=int32)
tf.Tensor([ True False], shape=(2,), dtype=bool)
tf.Tensor(
[[0.1 0.1]
 [0.1 0.1]], shape=(2, 2), dtype=float32)
tf.Tensor(b'Hello World!', shape=(), dtype=string)
```
<a name="iThZy"></a>
### 创建数据序列
格式:
```python
range(start, limit, delta=1, dtype=None, name='range')
```
参数:

- `start`: 开始位置
- `limit`: 序列的上限
- `delta`: 相当于 Numpy 的 step, 步长
- `detype`: 数据类型
- `name`: 数据名称, 默认为 “`range`”

例子:
```python
# 创建数字序列
r1 = tf.range(4)
print(r1)
```
输出结果:
```python
tf.Tensor([0 1 2 3], shape=(4,), dtype=int32)
```
<a name="ilLUv"></a>
### 创建图变量
格式:
```python
tf.Variable.init(initial_value, trainable=True, collections=None, validate_shape=True, name=None)
```
参数:

| 参数名称 | 参数类型 | 参数含义 |
| --- | --- | --- |
| initial_value | 所有可以转换为 Tensor 的类型 | 变量的初始值 |
| trainable | bool | 如果为 True, 会把它加入到 `GraphKeys.TRAINABLE_VARIABLES`, 才能对它使用 Optimizer &#124;collections&#124;list&#124;指定该图变量的类型, 默认为 [`GraphKeys.GLOBAL_VARIABLES`] |
| validate_shape | bool | 如果为 False, 则不进行类型和维度检查 |
| name | string | 数据名称 |

例子:
```python
# 创建图变量
v1 = tf.Variable(tf.range(6))
print(v1)
print(isinstance(v1, tf.Tensor))  # False
print(isinstance(v1, tf.Variable))  # True
print(tf.is_tensor(v1))  # True
```
输出结果:
```python
False
True
True
```
<a name="jRzFJ"></a>
### `tf.zeros`
`tf.zeros` 可以创建一个所有参数为 0 的 tensor 对象, 类似于 `np.zeros`.<br />格式:
```python
tf.zeros(shape, dtype=tf.dtypes.float32, name=None)
```
参数:

- `shape`: 数组的形状
- `dype`: 数据类型, 默认为 float32
- `name`: 数据名称

例子:
```python
# 创建参数为0的tensor
z1 = tf.zeros([1])
print(z1)

z2 = tf.zeros([3, 3])
print(z2)
```
输出结果:
```python
tf.Tensor([0.], shape=(1,), dtype=float32)
tf.Tensor(
[[0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]], shape=(3, 3), dtype=float32)
```
<a name="F5oqo"></a>
### `tf.ones`
`tf.ones` 用法和 `tf.zeros` 一样, 可以创建一个所有参数为 1 的 tensor 对象.
```python
tf.ones(shape, dtype=tf.dtypes.float32, name=None)
```
参数:

- `shape`: 数组的形状
- `dype`: 数据类型, 默认为 float32
- `name`: 数据名称

例子:
```python
# 创建参数为1的tensor
o1 = tf.ones([1])
print(o1)

o2 = tf.ones([3, 3])
print(o2)
```
输出结果:
```python
tf.Tensor([1.], shape=(1,), dtype=float32)
tf.Tensor(
[[1. 1. 1.]
 [1. 1. 1.]
 [1. 1. 1.]], shape=(3, 3), dtype=float32)
```
<a name="f05iD"></a>
### `tf.zeros_like`
`tf.zeros_like` 可以创建一个与给定 tensor 类型大小一致的 tensor, 类似 `np.zeros_like`.<br />格式:
```python
tf.zeros_like(tensor, dype=None, name=None)
```
参数:

- `tensor`: 传入的 tensor
- `dype`: 数据类型, 默认为 float32
- `name`: 数据名称

例子:
```python
# tf.zeros_like
t1 = tf.range(6)
z1 = tf.zeros_like(t1)
print(z1)
```
输出结果:
```python
tf.Tensor([0 0 0 0 0 0], shape=(6,), dtype=int32)
```
<a name="fqoo8"></a>
### `tf.ones_like`
格式:
```python
tf.ones_like(tensor, dype=None, name=None)
```
参数:

- `tensor`: 传入的 tensor
- `dype`: 数据类型, 默认为 float32
- `name`: 数据名称

例子:
```python
# tf.ones_like
t1 = tf.range(6)
o1 = tf.ones_like(t1)
print(o1)
```
输出结果:
```python
tf.Tensor([1 1 1 1 1 1], shape=(6,), dtype=int32)
```
<a name="kP42o"></a>
### `tf.fill`
`tf.fill` 可以创建一个指定形状和内容的 tensor.<br />格式:
```python
tf.fill(shape, value, name=None)
```
参数:

- `shape`: 数组的形状
- `value`: 填充的值
- `name`: 数据名称

例子:
```python
# tf.fill
f1 = tf.fill([2, 2], 0)
print(f1)

f2 = tf.fill([3, 3], 6)
print(f2)
```
输出结果:
```python
[[0 0]
 [0 0]], shape=(2, 2), dtype=int32)
tf.Tensor(
[[6 6 6]
 [6 6 6]
 [6 6 6]], shape=(3, 3), dtype=int32)
```
<a name="sARtZ"></a>
### `tf.gather`
`tf.gather`: 根据索引从参数轴收集切片.<br />格式:
```python
tf.gather(
    params, indices, validate_indices=None, axis=None, batch_dims=0, name=None
)
```
参数:

- `params`: 传入的张量
- `indices`: A Tensor.types 必须是: int32, int64.里面的每一个元素大小必须在 `[0, params.shape[axis])` 范围内
- `axis`: 维度, 默认为 0 例子:
```python
input =[ [[[1, 1, 1], [2, 2, 2]],
         [[3, 3, 3], [4, 4, 4]],
         [[5, 5, 5], [6, 6, 6]]],
 
         [[[7, 7, 7], [8, 8, 8]],
         [[9, 9, 9], [10, 10, 10]],
         [[11, 11, 11], [12, 12, 12]]],
 
        [[[13, 13, 13], [14, 14, 14]],
         [[15, 15, 15], [16, 16, 16]],
         [[17, 17, 17], [18, 18, 18]]]
         ]
output=tf.gather(input, [0,2],axis=0)
```
输出结果:
```python
tf.Tensor(
[[[[ 1  1  1]
   [ 2  2  2]]

  [[ 3  3  3]
   [ 4  4  4]]

  [[ 5  5  5]
   [ 6  6  6]]]


 [[[13 13 13]
   [14 14 14]]

  [[15 15 15]
   [16 16 16]]

  [[17 17 17]
   [18 18 18]]]], shape=(2, 3, 2, 3), dtype=int32)
```
<a name="brSxE"></a>
### `tf.random`
<a name="Pfw3V"></a>
#### 正态分布
`tf.random.normal` 可以创建随机数服从正态分布.<br />格式:
```python
tf.random.normal(
    shape, mean=0.0, stddev=1.0, dtype=tf.dtypes.float32, seed=None, name=None
)
```
参数:

- `shape`: 张量的形状
- `mean`: 正态分布的均值, 默认为 0.0
- `stddev`: 正态分布的标准差, 默认为 1.0
- `dtype`: 数据类型, 默认为 float32
- `seed`: 随机数种子
- `name`: 数据名称

例子:
```python
# tf.normal
n1 = tf.random.normal([2, 2], mean = 1, stddev=1, seed=0)
print(n1)
```
输出结果:
```python
tf.Tensor(
[[0.60084236 3.1044393 ]
 [1.1710722  1.5465181 ]], shape=(2, 2), dtype=float32)
```
<a name="T9B3h"></a>
#### 均匀分布
`tf.random.uniform` 可以创建随机数服从均匀分布.格式:
```python
tf.random.uniform(
    shape, minval=0, maxval=None, dtype=tf.dtypes.float32, seed=None, name=None
)
```
参数:

- `shape`: 张量的形状
- `minval`: 均匀分布的最小值, 默认为 0
- `maxvak`: 均匀分布的最大值
- `dtype`: 数据类型, 默认为 float32
- `seed`: 随机数种子
- `name`: 数据名称

例子:
```python
# tf.uniform
u1 = tf.random.uniform([2, 2], minval=0, maxval=1)
print(u1)
```
输出结果:
```python
tf.Tensor(
[[0.7382153  0.6622821 ]
 [0.22840345 0.09706533]], shape=(2, 2), dtype=float32)
```
<a name="i8iHh"></a>
#### 打乱顺序
`tf.random.shuffle` 可以打乱张量的顺序.<br />格式:
```python
tf.random.shuffle(
    value, seed=None, name=None
)
```
参数:

- `value`: 要被打乱的张量
- `seed`: 随机数种子
- `name`: 数据名称

例子:
```python
# tf.shuffle
s1 = tf.random.shuffle(tf.range(10))
print(s1)
```
输出结果:
```python
tf.Tensor([1 7 3 9 2 6 8 5 4 0], shape=(10,), dtype=int32)
```
<a name="cqSax"></a>
## 获取数据信息
<a name="Z1luR"></a>
### 获取数据维度
`tf.rank` 的用法和 `np.ndim` 基本一样.<br />格式:
```python
rank(input, name=None)  # 类似np.ndim
```
参数:

- `input`: 传入的张量
- `name`: 数据名称

例子:
```python
# 获取张量维度
t = tf.constant([[[1, 1, 1], [2, 2, 2]], [[3, 3, 3], [4, 4, 4]]])
print(tf.rank(t))
```
输出结果:
```python
tf.Tensor(3, shape=(), dtype=int32)
```
<a name="qCyBC"></a>
### 数据是否为张量
格式:
```python
tf.is_tensor(input)
```
参数:

- `input`: 传入的张量

例子:
```python
# 判断是否为张量
a = tf.constant([1, 2, 3])
b = tf.constant([True, False, False])
c = tf.constant("Hello World")
d = np.arange(6)

print(a)
print(tf.is_tensor(a))

print(b)
print(tf.is_tensor(b))

print(c)
print(tf.is_tensor(c))

print(d)
print(tf.is_tensor(d))
```
输出结果:
```python
tf.Tensor([1 2 3], shape=(3,), dtype=int32)
True
tf.Tensor([ True False False], shape=(3,), dtype=bool)
True
tf.Tensor(b'Hello World', shape=(), dtype=string)
True
[0 1 2 3 4 5]
False
```
<a name="JxZ8w"></a>
## 数据转换
<a name="dud7z"></a>
### 转换成张量
格式:
```python
tf.convert_to_tensor(value, dtype=None, dtype_hint=None, name=None)
```
参数:

- `value`: 需要转换的值
- `dtype`: 数据类型
- `dtype_hint`: 当 `dtype` 为 None 时的备选方案
- `name`: 数据名称

例子:
```python
# 转换成张量
array = np.arange(6)
print(array.dtype)

array_tf = tf.convert_to_tensor(array)
print(array_tf)
```
输出结果:
```python
int32
tf.Tensor([0 1 2 3 4 5], shape=(6,), dtype=int32)
```
<a name="BWjfL"></a>
### 转换数据类型
格式:
```python
cast(x, dtype, name=None)
```
参数:

- `x`: 输入的值
- `dtype`: 数据类型
- `name`: 数据名称

例子:
```python
# 装换数据类型
array_tf = tf.constant(np.arange(6))
print(array_tf)

array_tf = tf.cast(array_tf, dtype=tf.float32)
print(array_tf)

tf_bool = tf.cast(tf.constant([False, True]), dtype=tf.int32)
print(tf_bool)
```
输出结果:
```python
tf.Tensor([0 1 2 3 4 5], shape=(6,), dtype=int32)
tf.Tensor([0. 1. 2. 3. 4. 5.], shape=(6,), dtype=float32)
tf.Tensor([0 1], shape=(2,), dtype=int32)
```
<a name="t2tsH"></a>
### 转换成 numpy
例子:
```python
# tensor转换成numpy
array_tf = tf.ones([2,2])
array_np = array_tf.numpy()
print(array_np)
```
输出结果:
```python
[[1. 1.]
 [1. 1.]]
```
