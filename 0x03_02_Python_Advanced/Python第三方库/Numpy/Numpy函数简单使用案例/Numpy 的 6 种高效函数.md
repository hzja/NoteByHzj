Python Numpy
<a name="sLCYU"></a>
### `argpartition()`
借助于 `argpartition()`，Numpy 可以找出 N 个最大数值的索引，也会将找到的这些索引输出。然后根据需要对数值进行排序。
```python
x = np.array([12, 10, 12, 0, 6, 8, 9, 1, 16, 4, 6, 0])
index_val = np.argpartition(x, -4)[-4:]
index_val
array([1, 8, 2, 0], dtype=int64)
np.sort(x[index_val])
array([10, 12, 12, 16])
```
<a name="33f51127"></a>
### `allclose()`
`allclose()` 用于匹配两个数组，并得到布尔值表示的输出。如果在一个公差范围内（within a tolerance）两个数组不等同，则 `allclose()` 返回 False。该函数对于检查两个数组是否相似非常有用。
```python
array1 = np.array([0.12,0.17,0.24,0.29])
array2 = np.array([0.13,0.19,0.26,0.31])
# with a tolerance of 0.1, it should return False:
np.allclose(array1,array2,0.1)
False
# with a tolerance of 0.2, it should return True:
np.allclose(array1,array2,0.2)
True
```
<a name="0KAI6"></a>
### `clip()`
`clip()` 使得一个数组中的数值保持在一个区间内。有时需要保证数值在上下限范围内。为此，可以借助 Numpy 的 `clip()` 函数实现该目的。给定一个区间，则区间外的数值被剪切至区间上下限（interval edge）。
```python
x = np.array([3, 17, 14, 23, 2, 2, 6, 8, 1, 2, 16, 0])
np.clip(x,2,5)
array([3, 5, 5, 5, 2, 2, 5, 5, 2, 2, 5, 2])
```
<a name="9f8d53f6"></a>
### `extract()`
顾名思义，`extract()` 是在特定条件下从一个数组中提取特定元素。借助于 `extract()`，还可以使用 and 和 or 等条件。
```python
# Random integers
array = np.random.randint(20, size=12)
array
array([ 0,  1,  8, 19, 16, 18, 10, 11,  2, 13, 14,  3])#  Divide by 2 and check if remainder is 1
cond = np.mod(array, 2)==1
cond
array([False,  True, False,  True, False, False, False,  True, False, True, False,  True])# Use extract to get the values
np.extract(cond, array)
array([ 1, 19, 11, 13,  3])# Apply condition on extract directly
np.extract(((array < 3) | (array > 15)), array)
array([ 0,  1, 19, 16, 18,  2])
```
<a name="yH6wK"></a>
### `where()`
`Where()` 用于从一个数组中返回满足特定条件的元素。比如，它会返回满足特定条件的数值的索引位置。`Where()` 与 SQL 中使用的 where condition 类似，如以下示例所示：
```python
y = np.array([1,5,6,8,1,7,3,6,9])
# Where y is greater than 5, returns index position
np.where(y>5)
array([2, 3, 5, 7, 8], dtype=int64),)
# First will replace the values that match the condition, 
# second will replace the values that does not
np.where(y>5, "Hit", "Miss")
array([ Miss ,  Miss ,  Hit ,  Hit ,  Miss ,  Hit ,  Miss ,  Hit ,  Hit ],dtype= <U4 )
```
<a name="797602fe"></a>
### `percentile()`
`Percentile()` 用于计算特定轴方向上数组元素的第 n 个百分位数。
```python
a = np.array([1,5,6,8,1,7,3,6,9])
print("50th Percentile of a, axis = 0 : ",  np.percentile(a, 50, axis =0))
# 50th Percentile of a, axis = 0 :  6.0
b = np.array([[10, 7, 4], [3, 2, 1]])
print("30th Percentile of b, axis = 0 : ",  np.percentile(b, 30, axis =0))
# 30th Percentile of b, axis = 0 :  [5.1 3.5 1.9]
```


