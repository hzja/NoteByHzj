Python Pandas<br />重复值处理主要涉及两个部分，一个是找出重复值，第二个是删除重复值，也就是根据自己设定的条件进行删除操作。
<a name="VYBtb"></a>
## 定位重复值
对于重复值，首先需要查看这些重复值是什么样的形式，然后确定删除的范围，而查询重复值需要用到`duplicated`函数。<br />`duplicated`的返回值是布尔值，返回`True`和`False`，默认情况下会按照一行的所有内容进行查重。<br />主要参数：

- `subset`：如果不按照全部内容查重，那么需要指定按照哪些列进行查重。比如按照姓名进行查重`subset=['name']`，那么具有相同名字的人就只会保留一个，但很可能只是重名的原因，而并非真正同一个人，所以可以按照姓名和出生日期两列查重，`subset=['name','birthday']`，同理还可以再添加列，这样就可以基本保证去重效果了。
- `keep`：用来确定要标记的重复值，可以设置为`first`、`last`、`False`。
   - `first`：除第一次出现的重复值，其他都标记为`True`
   - `last`：除最后一次出现的重复值，其他都标记为`True`
   - `False`：所有重复值都标记为`True`

实例：
```python
import pandas as pd
import numpy as np

data = {
    'user' : ['zszxz','zszxz','rose'],
    'price' : [100, 200, -300],
    'hobby' : ['reading','reading','hiking']
}
frame  = pd.DataFrame(data)
print(frame)
------------------------
    user  price    hobby
0  zszxz    100  reading
1  zszxz    200  reading
2   rose   -300   hiking
------------------------

frame.duplicated()
----------
0    False
1    False
2    False
dtype: bool
-----------
```
上面提到`duplicated`返回布尔值，所以如果要想输出这些重复值，还需要和查询的方法配合使用`df[df.duplicated()]`，比如：
```python
# 1、按user变量筛选重复值
frame[frame.duplicated(subset=['user'])]
-------------------
  user price hobby
1 zszxz 200 reading
-------------------
```
上面按user一个变量进行查重，但没有设置`keep`参数，所以默认筛选出除了第一个以外的其它重复值。
```python
# 2、按user变量筛选重复值,保留全部重复值
frame[frame.duplicated(subset=['user'], keep=False)]
-------------------
  user price hobby
0 zszxz 100 reading
1 zszxz 200 reading
-------------------
```
上面按user一个变量进行查重，并设置keep参数为False，所以保留了全部的重复值。
```python
# 3、按user和hobby变量筛选重复值,筛选出除最后一个重复值以外的其它重复值
frame[frame.duplicated(subset=['user','hobby'], keep='last')]
-------------------
  user price hobby
0 zszxz 100 reading
-------------------
```
上面按user和hobby两个变量进行查重，并设置keep参数为last，所以筛选出了除最后一个重复值以外的其它重复值。<br />通过两个参数的设置就可以查看自己想要的重复值了，以此判断要删除哪个，保留哪个。
<a name="R6l3U"></a>
## 删除重复值
当确定好需要删除的重复值后，就进行进行删除的操作了。<br />删除重复值会用到`drop_duplicates`函数。<br />和`duplicated()`函数参数类似，主要有3个参数：

- `subset`：同`duplicated()`，设置去重的字段
- `keep`：这里稍有不同，`duplicated()`中是将除设置值以外重复值都返回True，而这里是保留的意思。同样可以设置first、last、False
   - `first`：保留第一次出现的重复行，删除其他重复行
   - `last`：保留最后一次出现的重复行，删除其他重复行
   - `False`：删除所有重复行
- `inplace`：布尔值，默认为False，是否直接在原数据上删除重复项或删除重复项后返回副本。

实例：
<a name="GCXVU"></a>
### 1、全部去重
```python
# 按全部字段删除，在原数据frame上生效
frame.drop_duplicates(inplace=True)
print(frame)
------------------------
    user  price    hobby
0  zszxz    100  reading
1  zszxz    200  reading
2   rose   -300   hiking
------------------------
```
因为上面数据中没有全部重复的，因此没有可删除行。
<a name="OyTn5"></a>
### 2、指定列去重
```python
# 按user字段删除，在原数据frame上生效
frame.drop_duplicates(subset=['user'],inplace=True)
print(frame)
------------------------
    user  price    hobby
0  zszxz    100  reading
2   rose   -300   hiking
------------------------
```
上面按user字段删除重复行，保留第一个重复行，因此第二行被删除了。但这里大家注意下，**执行删除重复行操作后，表的索引也会被删掉。**<br />如需要重置可以加上`reset_index()`，设置`drop=True`，用索引替代被打乱的索引。
```python
frame.drop_duplicates(subset=['user'],inplace=True)
frame.reset_index(drop=True)
------------------------
    user  price    hobby
0  zszxz    100  reading
1   rose   -300   hiking
------------------------
```
keep默认为first，下面手动设置为last，只保留最后一个重复行。
```python
# 按全部字段删除，在原数据frame上生效
frame.drop_duplicates(subset=['user','hobby'],keep='last',inplace=True)
print(frame)
------------------------
    user  price    hobby
1  zszxz    200  reading
2   rose   -300   hiking
------------------------
```
keep手动设置为False，全部删除，这种一般很少用。
```python
# 按全部字段删除，在原数据frame上生效
frame.drop_duplicates(subset=['user','hobby'],keep=False,inplace=True)
print(frame)
------------------------
    user  price    hobby
2   rose   -300   hiking
------------------------
```
以上就是重复值相关的所有操作。
<a name="HNIw4"></a>
## 注意事项
在删除重复值时，要注意下删除的逻辑。<br />因为很多时候需要把这些离线的清洗操作在线上复现。<br />如果随机地删除重复行，没有明确的逻辑，那么对于这种随机性线上是无法复现的，即无法保证清洗后的数据一致性。<br />**所以在删除重复行前，可以把重复判断字段进行排序处理。**<br />比如上面例子中，如果要对user和price去重，那么比较严谨的做法是按照user和price进行排序。
```python
frame.sort_values(by=['user','price'],ascending=True).reset_index(drop=True)
--------------------
user price hobby
0 rose -300 hiking
1 zszxz 100 reading
2 zszxz 200 reading
--------------------
```
因为有了排序性，只要按这个逻辑它的顺序是固定的，而不是随机的。所以无论设置keep为first还是last，都没有任何影响。
