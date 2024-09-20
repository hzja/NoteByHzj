Python 迭代器<br />Python官方文档用"高效的循环"来形容`itertools`模块，有些tools会带来性能提升，而另外一些tools并不快，只是会节省一些开发时间而已，如果滥用还会导致代码可读性变差。
<a name="e891d86b"></a>
### 1. 数列累加
给定一个列表`An`，返回数列累加和`Sn`。举例说明：<br />输入: `[1, 2, 3, 4, 5]`<br />返回: `[1, 3, 6, 10, 15]`<br />使用`accumulate`，性能提升了2.5倍
```python
from itertools import accumulate
def _accumulate_list(arr):
    tot = 0
    for x in arr:
        tot += x
        yield tot
def accumulate_list(arr):
    return list(_accumulate_list(arr))
def fast_accumulate_list(arr):
    return list(accumulate(arr))
arr = list(range(1000))
%timeit accumulate_list(arr)
61 µs ± 2.91 µs per loop (mean ± std. dev. of 7 runs, 10000 loops each)
%timeit fast_accumulate_list(arr)
21.3 µs ± 811 ns per loop (mean ± std. dev. of 7 runs, 10000 loops each)
```
<a name="a2714f3e"></a>
### 2. 选择数据
给定一个列表`data`，一个用0/1表示的列表`selectors`，返回被选择的数据。举例说明：<br />输入: `[1, 2, 3, 4, 5], [0, 1, 0, 1, 0]`<br />返回: `[2, 4]`<br />使用`compress`，性能提升了2.8倍
```python
from itertools import compress
from random import randint
def select_data(data, selectors):
    return [x for x, y in zip(data, selectors) if y]
def fast_select_data(data, selectors):
    return list(compress(data, selectors))
data = list(range(10000))
selectors = [randint(0, 1) for _ in range(10000)]
%timeit select_data(data, selectors)
341 µs ± 17.8 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
%timeit fast_select_data(data, selectors)
130 µs ± 3.19 µs per loop (mean ± std. dev. of 7 runs, 10000 loops each)
```
<a name="6W3Y8"></a>
### 3. 组合
给定一个列表`arr`和一个数字`k`，返回从`arr`中选择`k`个元素的所有情况。举例说明：<br />输入: `[1, 2, 3], 2`<br />返回: `[(1, 2), (1, 3), (2, 1), (2, 3), (3, 1), (3, 2)]`<br />使用`permutations`，性能提升了10倍
```python
from itertools import permutations
def _get_permutations(arr, k, i):
    if i == k:
        return [arr[:k]]
    res = []
    for j in range(i, len(arr)):
        arr_cpy = arr.copy()
        arr_cpy[i], arr_cpy[j] = arr_cpy[j], arr_cpy[i]
        res += _get_permutations(arr_cpy, k, i + 1)
    return res
def get_permutations(arr, k):
    return _get_permutations(arr, k, 0)
def fast_get_permutations(arr, k):
    return list(permutations(arr, k))
arr = list(range(10))
k = 5
%timeit -n 1 get_permutations(arr, k)
15.5 ms ± 1.96 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)
%timeit -n 1 fast_get_permutations(arr, k)
1.56 ms ± 284 µs per loop (mean ± std. dev. of 7 runs, 1 loop each)
```
<a name="UKjAL"></a>
### 4. 筛选数据
给定一个列表`arr`，筛选出所有的偶数。举例说明：<br />输入: `[3, 1, 4, 5, 9, 2]`<br />返回: `[(4, 2]`<br />使用`filterfalse`，性能反而会变慢，所以不要迷信`itertools`。
```python
from itertools import filterfalse
def get_even_nums(arr):
    return [x for x in arr if x % 2 == 0]
def fast_get_even_nums(arr):
    return list(filterfalse(lambda x: x % 2, arr))
arr = list(range(10000))
%timeit get_even_nums(arr)
417 µs ± 18.8 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
%timeit fast_get_even_nums(arr)
823 µs ± 22.6 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
```
<a name="uB0Dp"></a>
### 5. 条件终止
给定一个列表arr，依次对列表的所有数字进行求和，若遇到某个元素大于target之后则终止求和，返回这个和。举例说明：<br />输入: `[1, 2, 3, 4, 5], 3`<br />返回: 6 (4 > 3，终止)<br />使用`takewhile`，性能反而会变慢，所以不要迷信`itertools`。
```python
from itertools import takewhile
def cond_sum(arr, target):
    res = 0
    for x in arr:
        if x > target:
            break
        res += x
    return res
def fast_cond_sum(arr, target):
    return sum(takewhile(lambda x: x <= target, arr))
arr = list(range(10000))
target = 5000
%timeit cond_sum(arr, target)
245 µs ± 11.8 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
%timeit fast_cond_sum(arr, target)
404 µs ± 13.3 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
```
<a name="pxFDr"></a>
### 6. 循环嵌套
给定列表`arr1`，`arr2`，返回两个列表的所有元素两两相加的和。举例说明：<br />输入: `[1, 2], [4, 5]`<br />返回: `[1 + 4， 1 + 5， 2 + 4， 2 + 5]`<br />使用`product`，性能提升了1.25倍。
```python
from itertools import product
def _cross_sum(arr1, arr2):
    for x in arr1:
        for y in arr2:
            yield x + y
def cross_sum(arr1, arr2):
    return list(_cross_sum(arr1, arr2))
def fast_cross_sum(arr1, arr2):
    return [x + y for x, y in product(arr1, arr2)]
arr1 = list(range(100))
arr2 = list(range(100))
%timeit cross_sum(arr1, arr2)
484 µs ± 16.6 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
%timeit fast_cross_sum(arr1, arr2)
373 µs ± 11.4 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
```
<a name="TdTaJ"></a>
### 7. 二维列表转一维列表
给定二维列表`arr`，转为一维列表 举例说明：<br />输入: `[[1, 2], [3, 4]]`<br />返回: `[1, 2, 3, 4]`<br />使用`chain`，性能提升了6倍。
```python
from itertools import chain
def _flatten(arr2d):
    for arr in arr2d:
        for x in arr:
            yield x
def flatten(arr2d):
    return list(_flatten(arr2d))
def fast_flatten(arr2d):
    return list(chain(*arr2d))
arr2d = [[x + y * 100 for x in range(100)] for y in range(100)]
%timeit flatten(arr2d)
379 µs ± 15.4 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
%timeit fast_flatten(arr2d)
66.9 µs ± 3.43 µs per loop (mean ± std. dev. of 7 runs, 10000 loops each)
```
