Python Pandas<br />首先先建立要用到的数据集，代码如下
```python
import pandas as pd
df = pd.DataFrame({
    "name": ["John","Jane","Emily","Lisa","Matt"],
    "note": [92,94,87,82,90],
    "profession":["Electrical engineer","Mechanical engineer",
                  "Data scientist","Accountant","Athlete"],
    "date_of_birth":["1998-11-01","2002-08-14","1996-01-12",
                     "2002-10-24","2004-04-05"],
    "group":["A","B","B","A","C"]
})
```
output
```python
    name  note           profession date_of_birth group
0   John    92  Electrical engineer    1998-11-01     A
1   Jane    94  Mechanical engineer    2002-08-14     B
2  Emily    87       Data scientist    1996-01-12     B
3   Lisa    82           Accountant    2002-10-24     A
4   Matt    90              Athlete    2004-04-05     C
```
<a name="pYTnF"></a>
### 筛选表格中的若干列
代码如下
```python
df[["name","note"]]
```
output
```python
    name  note
0   John    92
1   Jane    94
2  Emily    87
3   Lisa    82
4   Matt    90
```
<a name="KcuNf"></a>
### 再筛选出若干行
基于上面搜索出的结果之上，再筛选出若干行，代码如下
```python
df.loc[:3, ["name","note"]]
```
output
```python
    name  note
0   John    92
1   Jane    94
2  Emily    87
3   Lisa    82
```
<a name="H0QTx"></a>
### 根据索引来过滤数据
这里用到的是`iloc`方法，代码如下
```python
df.iloc[:3, 2]
```
output
```python
0    Electrical engineer
1    Mechanical engineer
2         Data scientist
```
<a name="r3QWz"></a>
### 通过比较运算符来筛选数据
```python
df[df.note > 90]
```
output
```python
   name  note           profession date_of_birth group
0  John    92  Electrical engineer    1998-11-01     A
1  Jane    94  Mechanical engineer    2002-08-14     B
```
<a name="MeAq9"></a>
### `dt`属性接口
`dt`属性接口是用于处理时间类型的数据的，当然首先需要将字符串类型的数据，或者其他类型的数据转换成事件类型的数据，然后再处理，代码如下
```python
df.date_of_birth = df.date_of_birth.astype("datetime64[ns]")
df[df.date_of_birth.dt.month==11]
```
output
```python
   name  note           profession date_of_birth group
0  John    92  Electrical engineer    1998-11-01     A
```
或者也可以
```python
df[df.date_of_birth.dt.year > 2000]
```
output
```python
   name  note           profession date_of_birth group
1  Jane    94  Mechanical engineer    2002-08-14     B
3  Lisa    82           Accountant    2002-10-24     A
4  Matt    90              Athlete    2004-04-05     C
```
<a name="J02L4"></a>
### 多个条件交集过滤数据
当遇上多个条件，并且是交集的情况下过滤数据时，代码应该这么来写
```python
df[(df.date_of_birth.dt.year > 2000) &  
   (df.profession.str.contains("engineer"))]
```
output
```python
   name  note           profession date_of_birth group
1  Jane    94  Mechanical engineer    2002-08-14     B
```
<a name="iPKPq"></a>
### 多个条件并集筛选数据
当多个条件是以并集的方式来过滤数据的时候，代码如下
```python
df[(df.note > 90) | (df.profession=="Data scientist")]
```
output
```python
    name  note           profession date_of_birth group
0   John    92  Electrical engineer    1998-11-01     A
1   Jane    94  Mechanical engineer    2002-08-14     B
2  Emily    87       Data scientist    1996-01-12     B
```
<a name="weohF"></a>
### Query方法过滤数据
Pandas当中的`query`方法也可以对数据进行过滤，将过滤的条件输入
```python
df.query("note > 90")
```
output
```python
   name  note           profession date_of_birth group
0  John    92  Electrical engineer    1998-11-01     A
1  Jane    94  Mechanical engineer    2002-08-14     B
```
又或者是
```python
df.query("group=='A' and note > 89")
```
output
```python
   name  note           profession date_of_birth group
0  John    92  Electrical engineer    1998-11-01     A
```
<a name="KEMbS"></a>
### `nsmallest`方法过滤数据
pandas当中的`nsmallest`以及`nlargest`方法是用来找到数据集当中最大、最小的若干数据，代码如下
```python
df.nsmallest(2, "note")
```
output
```python
    name  note      profession date_of_birth group
3   Lisa    82      Accountant    2002-10-24     A
2  Emily    87  Data scientist    1996-01-12     B

df.nlargest(2, "note")
```
output
```python
   name  note           profession date_of_birth group
1  Jane    94  Mechanical engineer    2002-08-14     B
0  John    92  Electrical engineer    1998-11-01     A
```
<a name="PUZnV"></a>
### `isna()`方法
`isna()`方法功能在于过滤出那些是空值的数据，首先将表格当中的某些数据设置成空值
```python
df.loc[0, "profession"] = np.nan
df[df.profession.isna()]
```
output
```python
   name  note profession date_of_birth group
0  John    92        NaN    1998-11-01     A
```
<a name="KgTYg"></a>
### `notna()`方法
`notna()`方法上面的`isna()`方法正好相反的功能在于过滤出那些不是空值的数据，代码如下
```python
df[df.profession.notna()]
```
output
```python
    name  note           profession date_of_birth group
1   Jane    94  Mechanical engineer    2002-08-14     B
2  Emily    87       Data scientist    1996-01-12     B
3   Lisa    82           Accountant    2002-10-24     A
4   Matt    90              Athlete    2004-04-05     C
```
<a name="U9XbS"></a>
### `assign`方法
pandas当中的`assign`方法作用是直接向数据集当中来添加一列
```python
df_1 = df.assign(score=np.random.randint(0,100,size=5))
df_1
```
output
```python
    name  note           profession date_of_birth group  score
0   John    92  Electrical engineer    1998-11-01     A     19
1   Jane    94  Mechanical engineer    2002-08-14     B     84
2  Emily    87       Data scientist    1996-01-12     B     68
3   Lisa    82           Accountant    2002-10-24     A     70
4   Matt    90              Athlete    2004-04-05     C     39
```
<a name="lOVrM"></a>
### `explode`方法
`explode()`方法直译的话，是爆炸的意思，经常会遇到这样的数据集
```python
  Name            Hobby
0   吕布  [打篮球, 玩游戏, 喝奶茶]
1   貂蝉       [敲代码, 看电影]
2   赵云        [听音乐, 健身]
```
Hobby列当中的每行数据都以列表的形式集中到了一起，而`explode()`方法则是将这些集中到一起的数据拆开来，代码如下
```python
 Name Hobby
0   吕布   打篮球
0   吕布   玩游戏
0   吕布   喝奶茶
1   貂蝉   敲代码
1   貂蝉   看电影
2   赵云   听音乐
2   赵云    健身
```
当然会展开来之后，数据会存在重复的情况，
```python
df.explode('Hobby').drop_duplicates().reset_index(drop=True)
```
output
```python
Name Hobby
0   吕布   打篮球
1   吕布   玩游戏
2   吕布   喝奶茶
3   貂蝉   敲代码
4   貂蝉   看电影
5   赵云   听音乐
6   赵云    健身
```
