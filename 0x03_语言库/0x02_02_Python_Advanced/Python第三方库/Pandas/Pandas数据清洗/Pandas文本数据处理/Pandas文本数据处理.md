Python Pandas
<a name="Denj9"></a>
#### 创建文本内容的数据
先来创建一个包含文本数据的DataFrame，供后面使用
```python
import pandas as pd
df = pd.DataFrame({
   "姓": ["李","王","戴", "李", "张"],
   "名": ["华","硕","建业", "四", "三"],
   "户籍地址": [" 浙江省·宁波市 ", "   浙江省·杭州市    ", "  浙江省·丽水市  ", "  浙江省·衢州市 ", "  浙江省·湖州市           "],
   "微信ID": ["Tomoplplplut1248781", "Smopopo857", "Adahuhuifhhjfj", "Tull1945121", "ZPWERERTFD599557"],
   "邮箱地址": ["tom02159@163.com", "smitt7821@163.com", "adams623@163.com", "tull0305@163.com", "five7532@163.com"]
})

df
```
output
```
   姓   名                  户籍地址                 微信ID               邮箱地址
0  李   华              浙江省·宁波市   Tomoplplplut1248781   tom02159@163.com
1  王   硕           浙江省·杭州市               Smopopo857  smitt7821@163.com
2  戴  建业             浙江省·丽水市         Adahuhuifhhjfj   adams623@163.com
3  李   四              浙江省·衢州市           Tull1945121   tull0305@163.com
4  张   三    浙江省·湖州市                ZPWERERTFD599557   five7532@163.com

```
<a name="YPtud"></a>
#### 常用处理文本数据的方法总结
Python当中用来处理字符串数据的方法有很多，基本上都可以在DataFrame内部拿来使用，例如`lower()`方法和`upper()`方法，给字母大小写
```python
df["微信ID"].str.lower()
```
output
```
0    tomoplplplut1248781
1             smopopo857
2         adahuhuifhhjfj
3            tull1945121
4       zpwerertfd599557
Name: 微信ID, dtype: object
```
又譬如
```python
df["微信ID"].str.len()
```
output
```
0    19
1    10
2    14
3    11
4    16
Name: 微信ID, dtype: int64
```
可以看到户籍地址这一列中的数据有很多的空格
```python
df["户籍地址"]
```
output
```
0                浙江省·宁波市 
1             浙江省·杭州市    
2               浙江省·丽水市  
3                浙江省·衢州市 
4      浙江省·湖州市           
Name: 户籍地址, dtype: object
```
可以使用处理字符串时的`strip()`方法
```python
df["户籍地址"].str.strip()
```
output
```
0    浙江省·宁波市
1    浙江省·杭州市
2    浙江省·丽水市
3    浙江省·衢州市
4    浙江省·湖州市
Name: 户籍地址, dtype: object
```
与之相类似的还有`lstrip()`方法以及`rstrip()`方法，这里就不做赘述。<br />在字符串的处理过程当中，`startswith()`方法和`endswith()`方法也是用的非常的频繁，例如想要挑选出户籍地址是“宁波市”的数据，
```python
df["户籍地址"] = df["户籍地址"].str.strip()
df[df["户籍地址"].str.endswith("宁波市")]
```
output
```
   姓  名     户籍地址                 微信ID              邮箱地址
0  李  华  浙江省·宁波市  Tomoplplplut1248781  tom02159@163.com
```
另外还可以使用`replace()`方法来实现当中的字符串的替换
```python
df["户籍地址"].str.replace("·", "--")
```
output
```
0    浙江省--宁波市
1    浙江省--杭州市
2    浙江省--丽水市
3    浙江省--衢州市
4    浙江省--湖州市
Name: 户籍地址, dtype: object
```
那既然用到了`replace()`方法，那么`split()`方法也可以尝试一下
```python
df["户籍地址"].str.split("·")
```
output
```
0    [浙江省, 宁波市]
1    [浙江省, 杭州市]
2    [浙江省, 丽水市]
3    [浙江省, 衢州市]
4    [浙江省, 湖州市]
Name: 户籍地址, dtype: object
```
在经过`spilit()`方法的切割过之后就变成了列表的形式，然后可以通过`get()`方法或者`[]`来获取里面的元素，例如
```python
df["户籍地址"].str.split("·").str.get(0)
```
或者
```python
df["户籍地址"].str.split("·").str[0]
```
output
```
0    浙江省
1    浙江省
2    浙江省
3    浙江省
4    浙江省
Name: 户籍地址, dtype: object
```
那么获取列表当中的第二个元素也是同样的道理，当然也可以在`split()`方法当中添加`expand=True`这个参数，来将上面列表形式的数据转化成DataFrame格式
```python
df["户籍地址"].str.split("·", expand=True)
```
output
```
     0    1
0  浙江省  宁波市
1  浙江省  杭州市
2  浙江省  丽水市
3  浙江省  衢州市
4  浙江省  湖州市
```
同样地，可以在后面添加`[]`来获取想要的元素
```python
df["户籍地址"].str.split("·", expand=True)[1]
```
output
```
0    宁波市
1    杭州市
2    丽水市
3    衢州市
4    湖州市
Name: 1, dtype: object
```
<a name="Eode3"></a>
#### 正则表达式与DataFrame内部方法的结合
假如想要提取文本数据内部的一部分数据，可以结合正则表达式来使用，例如想要提取“微信ID”这一列当中的字母和数字，并且将两者分开来
```python
two_groups = "([a-zA-Z]+)([0-9]+)"
df["微信ID"].str.extract(two_groups, expand=True)
```
output
```
              0        1
0  Tomoplplplut  1248781
1       Smopopo      857
2           NaN      NaN
3          Tull  1945121
4    ZPWERERTFD   599557
```
当然了，如果想是要提取文本数据中的部分数据，可以直接在`str`方法后面添加索引，例如
```python
df["邮箱地址"].str[-8:]
```
output
```
0    @163.com
1    @163.com
2    @163.com
3    @163.com
4    @163.com
Name: 邮箱地址, dtype: object
```
当然，从另外一个角度讲，正则表达式也可以确认文本数据是否符合某种规律，
```python
two_groups = "([a-zA-Z]+)([0-9]+)"
df["微信ID"].str.match(two_groups)
```
output
```
0     True
1     True
2    False
3     True
4     True
Name: 微信ID, dtype: bool
```
当中有一个为`False`，不满足字母+数字的规律，再进一步，将满足条件的数据提取出来
```python
df[df["微信ID"].str.match(two_groups)]
```
output
```
   姓  名     户籍地址                 微信ID               邮箱地址
0  李  华  浙江省·宁波市  Tomoplplplut1248781   tom02159@163.com
1  王  硕  浙江省·杭州市           Smopopo857  smitt7821@163.com
3  李  四  浙江省·衢州市          Tull1945121   tull0305@163.com
4  张  三  浙江省·湖州市     ZPWERERTFD599557   five7532@163.com
```
针对文本数据而言，`contains()`方法也能够派上用场，例如下面的数据
```
   姓   名                  户籍地址                 微信ID               邮箱地址
0  李   华              浙江省·宁波市   Tomoplplplut1248781   tom02159@163.com
1  王   硕           浙江省·杭州市               Smopopo857  smitt7821@163.com
2  戴  建业             浙江省·丽水市         Adahuhuifhhjfj   adams623@163.com
3  李   四              浙江省·衢州市           Tull1945121   tull0305@163.com
4  张   三    浙江省·湖州市                ZPWERERTFD599557   five7532@163.com
5  黄   五               浙江省·宁波市        hunhunhu45652  1erdcvf127@16.com
```
用`contains()`来提取出户籍地址为“宁波市”的内容，可以这么做
```python
df[df["户籍地址"].str.contains("宁波市")]
```
output
```
   姓  名     户籍地址                 微信ID               邮箱地址
0  李  华  浙江省·宁波市  Tomoplplplut1248781   tom02159@163.com
5  黄  五  浙江省·宁波市        hunhunhu45652  1erdcvf127@16.com
```
