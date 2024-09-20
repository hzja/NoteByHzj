Python 
<a name="cAne7"></a>
### 函数连续调用
```python
def add(x):
class AddNum(int):
def __call__(self, x):
return AddNum(self.numerator + x)
return AddNum(x)
print add(2)(3)(5)
# 10
print add(2)(3)(4)(5)(6)(7)
# 27
# javascript 版
var add = function(x){
    var addNum = function(x){
return add(addNum + x);
    };
    addNum.toString = function(){
return x;
    }
return addNum;
}
add(2)(3)(5)//10
add(2)(3)(4)(5)(6)(7)//27
```
<a name="j3niZ"></a>
### 默认值陷阱
```python
>>> def evil(v=[]):
...     v.append(1)
...     print v
...
>>> evil()
[1]
>>> evil()
[1, 1]
```
<a name="9qSDa"></a>
### 读写csv文件
```python
import csv
with open('data.csv', 'rb') as f:
    reader = csv.reader(f)
for row in reader:
print row
# 向csv文件写入
import csv
with open( 'data.csv', 'wb') as f:
    writer = csv.writer(f)
    writer.writerow(['name', 'address', 'age'])  # 单行写入
    data = [
            ( 'xiaoming ','china','10'),
            ( 'Lily', 'USA', '12')]
    writer.writerows(data)  # 多行写入
```
<a name="wL43o"></a>
### 数制转换
```python
>>> int('1000', 2)
8
>>> int('A', 16)
10
```
<a name="Rn13o"></a>
### 格式化 json
```python
echo'{"k": "v"}' | python-m json.tool
```
<a name="X1u2w"></a>
### list 扁平化
```python
list_ = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
[k for i in list_ for k in i] #[1, 2, 3, 4, 5, 6, 7, 8, 9]
import numpy as np
print np.r_[[1, 2, 3], [4, 5, 6], [7, 8, 9]]
import itertools
print list(itertools.chain(*[[1, 2, 3], [4, 5, 6], [7, 8, 9]]))
sum(list_, [])
flatten = lambda x: [y for l in x for y in flatten(l)] if type(x) is list else [x]
flatten(list_)
```
<a name="D92hi"></a>
### list 合并
```python
>>> a = [1, 3, 5, 7, 9]
>>> b = [2, 3, 4, 5, 6]
>>> c = [5, 6, 7, 8, 9]
>>> list(set().union(a, b, c))
[1, 2, 3, 4, 5, 6, 7, 8, 9]
```
<a name="fCA4B"></a>
### 出现次数最多的 2 个字母
```python
from collections import Counter
c = Counter('hello world')
print(c.most_common(2)) #[('l', 3), ('o', 2)]
```
<a name="qC4o4"></a>
### 谨慎使用
```python
eval("__import__('os').system('rm -rf /')", {})
```
<a name="i9sKq"></a>
### 置换矩阵
```python
matrix = [[1, 2, 3],[4, 5, 6]]
res = zip( *matrix )   # res = [(1, 4), (2, 5), (3, 6)]
```
<a name="NIRdk"></a>
### 列表推导
```python
[item**2 for item in lst if item % 2]
map(lambda item: item ** 2, filter(lambda item: item % 2, lst))
>>> list(map(str, [1, 2, 3, 4, 5, 6, 7, 8, 9]))
['1', '2', '3', '4', '5', '6', '7', '8', '9']
```
<a name="lFV7h"></a>
### 排列组合
```python
>>> for p in itertools.permutations([1, 2, 3, 4]):
...     print ''.join(str(x) for x in p)
...
1234
1243
1324
1342
1423
1432
2134
2143
2314
2341
2413
2431
3124
3142
3214
3241
3412
3421
4123
4132
4213
4231
4312
4321
>>> for c in itertools.combinations([1, 2, 3, 4, 5], 3):
...     print ''.join(str(x) for x in c)
...
123
124
125
134
135
145
234
235
245
345
>>> for c in itertools.combinations_with_replacement([1, 2, 3], 2):
...     print ''.join(str(x) for x in c)
...
11
12
13
22
23
33
>>> for p in itertools.product([1, 2, 3], [4, 5]):
(1, 4)
(1, 5)
(2, 4)
(2, 5)
(3, 4)
(3, 5)
```
<a name="YdYC9"></a>
### 默认字典
```python
>>> m = dict()
>>> m['a']
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: 'a'
>>>
>>> m = collections.defaultdict(int)
>>> m['a']
0
>>> m['b']
0
>>> m = collections.defaultdict(str)
>>> m['a']
''
>>> m['b'] += 'a'
>>> m['b']
'a'
>>> m = collections.defaultdict(lambda: '[default value]')
>>> m['a']
'[default value]'
>>> m['b']
'[default value]'
```
<a name="KAjtQ"></a>
### 反转字典
```python
>>> m = {'a': 1, 'b': 2, 'c': 3, 'd': 4}
>>> m
{'d': 4, 'a': 1, 'b': 2, 'c': 3}
>>> {v: k for k, v in m.items()}
{1: 'a', 2: 'b', 3: 'c', 4: 'd'}
```
<a name="n4Fzf"></a>
### 1. List：all_equal功能实现：检验一个列表中的所有元素是否都一样。
**解读：**使用`[1:]` 和 `[:-1]` 来比较给定列表的所有元素。
```python
def all_equal(lst):
  return lst[1:] == lst[:-1]
```
**举例：**
```python
all_equal([1, 2, 3, 4, 5, 6]) # False
all_equal([1, 1, 1, 1]) # True
```
<a name="GiPXE"></a>
### 2. List：all_unique功能实现：如果列表所有值都是唯一的，返回 `True`，否则 False
**解读：**在给定列表上使用集合`set()`去重，比较它和原列表的长度。
```python
def all_unique(lst):
  return len(lst) == len(set(lst))
```
****举例：****
```python
x = [1,2,3,4,5,6]
y = [1,2,2,3,4,5]
all_unique(x) # True
all_unique(y) # False
```
<a name="150a4637"></a>
### 3. List：bifurcate功能实现：将列表值分组。如果在filter的元素是True，那么对应的元素属于第一个组；否则属于第二个组。
**解读：**使用列表推导式和`enumerate()`基于`filter`元素到各组。
```python
def bifurcate(lst, filter):
    return [
        [x for i,x in enumerate(lst) if filter[i] == True],
        [x for i,x in enumerate(lst) if filter[i] == False]
    ]
```
**举例：**
```python
bifurcate(['beep', 'boop', 'foo', 'bar'], [True, True, False, True])# [ ['beep', 'boop', 'bar'], ['foo'] ]
```
<a name="7vWHb"></a>
### 4. List：difference功能实现：返回两个iterables间的差异。
**解读：**创建b的集合，使用a的列表推导式保留不在_b中的元素。
```python
def difference(a, b):
    _b = set(b)
    return [item for item in a if item not in _b]
```
**举例：**
```python
difference([1, 2, 3], [1, 2, 4]) # [3]
```
<a name="jrudg"></a>
### 5. List：flatten功能实现：一次性的整合列表。
**解读：**使用嵌套的列表提取子列表的每个值。
```python
def flatten(lst):
    return [x for y in lst for x in y]
```
**举例：**<br />
```python
flatten([[1,2,3,4],[5,6,7,8]]) # [1, 2, 3, 4, 5, 6, 7, 8]
```
**6. Math：digitize功能实现：**将一个数分解转换为个位数字。<br />**解读：**将n字符化后使用`map()`函数结合`int`完成转化
```python
def digitize(n):
    return list(map(int, str(n)))
```
**举例：**<br />
```python
digitize(123) # [1, 2, 3]
```
<a name="QnApd"></a>
### 7. List：shuffle功能实现：将列表元素顺序随机打乱。
**解读：**使用Fisher-Yates算法重新排序列表元素。
```python
from copy import deepcopy
from random import randint
def shuffle(lst):
    temp_lst = deepcopy(lst)
    m = len(temp_lst)
    while (m):
        m -= 1
        i = randint(0, m)
        temp_lst[m], temp_lst[i] = temp_lst[i], temp_lst[m]
        return temp_lst
```
**举例：**<br />
```python
foo = [1,2,3]
shuffle(foo) # [2,3,1] , foo = [1,2,3]
```
<a name="c1JPf"></a>
### 8. Math：clamp_number功能实现：将数字num限制在由a和b边界值规定的范围中。
**解读：**如果num落在范围内，返回num；否则，返回范围内最接近的数字。
```python
def clamp_number(num,a,b):
    return max(min(num, max(a,b)),min(a,b))
```
**举例：**<br />
```python
clamp_number(2, 3, 5) # 3
clamp_number(1, -1, -5) # -1
```
<a name="F1esB"></a>
### 9. String：byte_size功能实现：返回字符串的字节数。
**解读：**使用`string.encode('utf-8')`解码给定字符串，返回长度。
```python
def byte_size(string):
  return len(string.encode('utf-8'))
```
**举例：**<br />
```python
byte_size('😀') # 4
byte_size('Hello World') # 11
```
<a name="DnVz2"></a>
### 10. Math：gcd功能实现：计算几个数的最大公因数。
**解读：**使用`reduce()`和`math.gcd`在给定列表上实现。
```python
from functools import reduce
import math
def gcd(numbers):
    return reduce(math.gcd, numbers)
```
**举例：**
```python
gcd([8,36,28]) # 4
```

