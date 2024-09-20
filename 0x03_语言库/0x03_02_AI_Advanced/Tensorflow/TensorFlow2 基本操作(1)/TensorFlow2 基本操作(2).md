Tensorflow2
<a name="dOQIS"></a>
## 索引操作
<a name="ae8YY"></a>
### 简单索引
通过索引 (index) 可以快速的找到张量中的特定信息.<br />例子:
```python
a = tf.reshape(tf.range(12), [2, 2, 3])
print(a)

print(a[0])
print(a[0][0])
```
输出结果:
```python
tf.Tensor(
[[[ 0  1  2]
  [ 3  4  5]]

 [[ 6  7  8]
  [ 9 10 11]]], shape=(2, 2, 3), dtype=int32)
tf.Tensor(
[[0 1 2]
 [3 4 5]], shape=(2, 3), dtype=int32)
tf.Tensor([0 1 2], shape=(3,), dtype=int32)
```
<a name="oMfPt"></a>
### Numpy 式索引
也可以按照 numpy 的写法来操作索引.<br />例子:
```python
a = tf.reshape(tf.range(12), [2, 2, 3])
print(a)

print(a[0])
print(a[0, 0])
```
输出结果:
```python
tf.Tensor(
[[[ 0  1  2]
  [ 3  4  5]]

 [[ 6  7  8]
  [ 9 10 11]]], shape=(2, 2, 3), dtype=int32)
tf.Tensor(
[[0 1 2]
 [3 4 5]], shape=(2, 3), dtype=int32)
tf.Tensor([0 1 2], shape=(3,), dtype=int32)
```
<a name="GLmlt"></a>
### 使用 : 进行索引
例子:
```python
c = tf.ones([4, 14, 14, 4])
print(c[0, :, :, :].shape)
print(c[0, 1, :, :].shape)
```
输出结果:
```python
(14, 14, 4)
(14, 4)
```
<a name="fwnGE"></a>
### `tf.gather`
假设一个有 3 个餐馆, 每个餐馆有 8 种菜系, 128 道菜`data: [resturants, cuisines, dishes]`.例子:
```python
data = tf.zeros([3, 8, 128])

g1 = tf.gather(data, axis=0, indices=[0, 2])
print(g1.shape)

g2 = tf.gather(data, axis=1, indices=[0, 1, 2, 3])
print(g2.shape)
```
输出结果:
```python
(2, 8, 128)
(3, 4, 128)
```
<a name="Ujenz"></a>
### `tf.gather_nd`
例子:
```python
g1 = tf.gather_nd(data, [0])
print(g1.shape)

g2 = tf.gather_nd(data, [0, 1])
print(g2.shape)

g3 = tf.gather_nd(data, [0, 1, 2])
print(g3.shape)
```
输出结果:
```python
(8, 128)
(128,)
()
```
<a name="tLLzn"></a>
### `tf.boolean_mask`
格式:
```python
tf.boolean_mask(
    tensor, mask, axis=None, name='boolean_mask'
)
```
例子:
```python
data = tf.zeros([3, 8, 128])

b1 = tf.boolean_mask(data, mask=[True, True, False])
print(b1.shape)

b2 = tf.boolean_mask(data, mask=[True, False, True, False, True, False, True, False], axis=1)
print(b2.shape)
```
输出结果:
```python
(2, 8, 128)
(3, 4, 128)
```
<a name="TGYg2"></a>
## 切片操作
借助切片技术, 可以灵活的处理张量对象.
<a name="LEHm5"></a>
### 简单切片
格式:
```python
tensor[start : end]
```
其中 start 为开始索引, end 为结束索引 (不包括)<br />例子:
```python
tf.Tensor([0 1 2], shape=(3,), dtype=int32)
tf.Tensor([9], shape=(1,), dtype=int32)
tf.Tensor([0 1 2 3 4 5 6 7 8], shape=(9,), dtype=int32)
```
<a name="u5h7x"></a>
### step 切片
格式:
```python
tensor[start : end: step]
```
例子:
```python
d = tf.range(6)
print(d[::-1])  # 实现倒序
print(d[::2])  # 步长为2
```
输出结果:
```python
tf.Tensor([5 4 3 2 1 0], shape=(6,), dtype=int32)
tf.Tensor([0 2 4], shape=(3,), dtype=int32)
```
<a name="VPlfx"></a>
## 维度变换
<a name="YCcmh"></a>
### `tf.reshape`
`tf.reshape` 可以进行维度转换.<br />格式:
```python
tf.reshape(
    tensor, shape, name=None
)
```
参数:

- `tensor`: 传入的张量
- `shape`: 张量的形状
- `name`: 数据名称

例子:
```python
a = tf.random.normal([3, 8, 128])
print(a.shape)

b = tf.reshape(a, [3, 1024])
print(b.shape)

c = tf.reshape(a, [3, -1])
print(c.shape)
```
输出结果:
```python
(3, 8, 128)
(3, 1024)
(3, 1024)
```
<a name="Nn5r4"></a>
### `tf.transpose`
格式:
```python
tf.transpose(
    a, perm=None, conjugate=False, name='transpose'
)
```
例子:
```python
a = tf.random.normal([4, 3, 2, 1])
print(a.shape)

b = tf.transpose(a)
print(b.shape)

c = tf.transpose(a, perm=[0, 1, 3, 2])
print(c.shape)
```
输出结果:
```python
(4, 3, 2, 1)
(1, 2, 3, 4)
(4, 3, 1, 2)
```
<a name="PnQEM"></a>
### `tf.expand_dims`
格式:
```python
tf.expand_dims(
    input, axis, name=None
)
```
参数:

- `input`: 输入
- `axis`: 操作的维度
- `name`: 数据名称

例子:
```python
a = tf.random.normal([4, 3, 2, 1])
print(a.shape)

b = tf.expand_dims(a, axis=0)
print(b.shape)

c = tf.expand_dims(a, axis=1)
print(c.shape)

d = tf.expand_dims(a, axis=-1)
print(d.shape)
```
输出结果:
```python
(4, 3, 2, 1)
(1, 4, 3, 2, 1)
(4, 1, 3, 2, 1)
(4, 3, 2, 1, 1)
```
<a name="DD9gw"></a>
### `tf.squeeze`
`tf.squeeze` 可以删去所有维度为1 的维度.<br />格式:
```python
tf.squeeze(
    input, axis=None, name=None
)
```
参数:

- `input`: 输入
- `axis`: 操作的维度
- `name`: 数据名称

例子:
```python
a = tf.zeros([2, 1, 1, 3, 5])

s1 = tf.squeeze(a)
print(s1.shape)

s2 = tf.squeeze(a, axis=1)
print(s2.shape)

s3 = tf.squeeze(a, axis=2)
print(s3.shape)
```
输出结果:
```python
(2, 3, 5)
(2, 1, 3, 5)
(2, 1, 3, 5)
```
<a name="eJLon"></a>
## Boardcasting
广播机制 (Boardcasting) 是一种张量复制的手段, Boardcasting 可以扩张张量的形状但无需实际复制数据.<br />![2021-12-18-11-11-18-750827.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639797133790-ecbe7320-a2fc-4974-a3ca-a8989045bfc3.png#clientId=u6eb1f95b-c499-4&from=ui&id=u0740b414&originHeight=646&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2097145&status=done&style=shadow&taskId=u77081e74-2928-48e1-a0a6-5c5ee917cf8&title=)<br />广播机制允许在隐式情况下进行填充, 从而使得代码更加简洁, 更有效率地使用内存.
<a name="olj1E"></a>
### `tf.boardcast_to`
boardcast_to:
```python
tf.broadcast_to(
    input, shape, name=None
)
```
参数:

- `input`: 输入
- `shape`: 数据形状
- `name`: 数据名称

例子:
```python
a = tf.broadcast_to(tf.random.normal([4, 1, 1, 1]), [4, 32, 32, 3])
print(a.shape)

b = tf.broadcast_to(tf.zeros([128, 1, 1, 1]), [128, 32, 32, 3])
print(b.shape)
```
输出结果:
```python
(4, 32, 32, 3)
(128, 32, 32, 3)
```
<a name="c9YTi"></a>
### `tf.tile`
格式:
```python
tf.tile(
    input, multiples, name=None
)
```
参数:

- `input`: 输入
- `multiples`: 同一纬度上复制的次数
- `name`: 数据名称

例子:
```python
a = tf.zeros([4, 1, 1, 1])
print(a.shape)

b = tf.tile(a, [1, 32, 32, 3])
print(b.shape)
```
输出结果:
```python
(4, 1, 1, 1)
(4, 32, 32, 3)
```
:::tips
注: boardcast_to 和 tile 的区别在于 boardcast_to 可以在不复制内存的情况下自动扩张 tensor。
:::
<a name="ANpQ4"></a>
## 数学运算
<a name="jP6Xg"></a>
### 加减乘除
例子:
```python
# 定义张量
t1 = tf.ones([3, 3])
t2 = tf.fill([3, 3], 3.0)

# 加
add = t1 + t2
print(add)

# 减
minus = t1 - t2
print(minus)

# 乘
multiply = t1 * t2
print(multiply)

# 除
divide = t1 / t2
print(divide)
```
输出结果:
```python
tf.Tensor(
[[4. 4. 4.]
 [4. 4. 4.]
 [4. 4. 4.]], shape=(3, 3), dtype=float32)
tf.Tensor(
[[-2. -2. -2.]
 [-2. -2. -2.]
 [-2. -2. -2.]], shape=(3, 3), dtype=float32)
tf.Tensor(
[[3. 3. 3.]
 [3. 3. 3.]
 [3. 3. 3.]], shape=(3, 3), dtype=float32)
tf.Tensor(
[[0.33333334 0.33333334 0.33333334]
 [0.33333334 0.33333334 0.33333334]
 [0.33333334 0.33333334 0.33333334]], shape=(3, 3), dtype=float32)
```
<a name="l6tCP"></a>
### `log` & `exp`
例子:
```python
# log
a = tf.fill([2], 100.0)
print(a)

b = tf.math.log(a)  # 以e为底
print(b)

# exp
c = tf.ones([2])
print(c)

d = tf.exp(c)
print(d)
```
输出结果:
```python
tf.Tensor([100. 100.], shape=(2,), dtype=float32)
tf.Tensor([4.6051702 4.6051702], shape=(2,), dtype=float32)
tf.Tensor([1. 1.], shape=(2,), dtype=float32)
tf.Tensor([2.7182817 2.7182817], shape=(2,), dtype=float32)
```
<a name="O4ynK"></a>
### `pow` & `sqrt`
例子:
```python
# 定义张量
a = tf.fill([2], 4.0)
print(a)

# pow
b = tf.pow(a, 2)
print(b)

# sqrt
c = tf.sqrt(a, 2)
print(c)
```
输出结果:
```python
tf.Tensor([4. 4.], shape=(2,), dtype=float32)
tf.Tensor([16. 16.], shape=(2,), dtype=float32)
tf.Tensor([2. 2.], shape=(2,), dtype=float32)
```
<a name="y8yjj"></a>
### 矩阵相乘 `@`
可以使用`tf.matmul`或`@`来实现矩阵相乘.<br />例子:
```python
# 定义张量
a = tf.fill([2, 2], 2)
b = tf.fill([2, 2], 3)

# matmul
c = tf.matmul(a, b)
print(c)

# @
d = a@b
print(d)
```
输出结果:
```python
tf.Tensor(
[[12 12]
 [12 12]], shape=(2, 2), dtype=int32)
tf.Tensor(
[[12 12]
 [12 12]], shape=(2, 2), dtype=int32)
```
