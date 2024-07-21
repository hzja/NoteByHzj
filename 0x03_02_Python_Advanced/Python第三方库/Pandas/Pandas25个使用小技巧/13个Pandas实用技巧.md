Python Pandas
<a name="ztjvn"></a>
## Pandas技巧总结
<a name="F4Yfo"></a>
## 1、计算变量缺失率
```python
df=pd.read_csv('titanic_train.csv')
def missing_cal(df):
    """
    df :数据集
    
    return：每个变量的缺失率
    """
    missing_series = df.isnull().sum()/df.shape[0]
    missing_df = pd.DataFrame(missing_series).reset_index()
    missing_df = missing_df.rename(columns={'index':'col',
                                            0:'missing_pct'})
    missing_df = missing_df.sort_values('missing_pct',ascending=False).reset_index(drop=True)
    return missing_df
missing_cal(df)
```
如果需要计算样本的缺失率分布，只要加上参数`axis=1`
<a name="tqLhm"></a>
## 2、获取分组里最大值所在的行方法
分为分组中有重复值和无重复值两种。无重复值的情况。
```python
df = pd.DataFrame({'Sp':['a','b','c','d','e','f'], 'Mt':['s1', 's1', 's2','s2','s2','s3'], 'Value':[1,2,3,4,5,6], 'Count':[3,2,5,10,10,6]})
df

df.iloc[df.groupby(['Mt']).apply(lambda x: x['Count'].idxmax())]
```
先按Mt列进行分组，然后对分组之后的数据框使用idxmax函数取出Count最大值所在的列，再用iloc位置索引将行取出。有重复值的情况
```python
df["rank"] = df.groupby("ID")["score"].rank(method="min", ascending=False).astype(np.int64)
df[df["rank"] == 1][["ID", "class"]]
```
对ID进行分组之后再对分数应用rank函数，分数相同的情况会赋予相同的排名，然后取出排名为1的数据。
<a name="EcpYv"></a>
## 3、多列合并为一行
```python
df = pd.DataFrame({'id_part':['a','b','c','d'], 'pred':[0.1,0.2,0.3,0.4], 'pred_class':['women','man','cat','dog'], 'v_id':['d1','d2','d3','d1']})

df.groupby(['v_id']).agg({'pred_class': [', '.join],'pred': lambda x: list(x),
'id_part': 'first'}).reset_index()
```
<a name="ADJLI"></a>
## 4、删除包含特定字符串所在的行
```python
df = pd.DataFrame({'a':[1,2,3,4], 'b':['s1', 'exp_s2', 's3','exps4'], 'c':[5,6,7,8], 'd':[3,2,5,10]})
df[df['b'].str.contains('exp')]
```
<a name="ewSDK"></a>
## 5、组内排序
```python
df = pd.DataFrame([['A',1],['A',3],['A',2],['B',5],['B',9]], columns = ['name','score'])
```
介绍两种高效地组内排序的方法。
```python
df.sort_values(['name','score'], ascending = [True,False])
df.groupby('name').apply(lambda x: x.sort_values('score', ascending=False)).reset_index(drop=True)
```
<a name="WWay1"></a>
## 6、选择特定类型的列
```python
drinks = pd.read_csv('data/drinks.csv')
# 选择所有数值型的列
drinks.select_dtypes(include=['number']).head()
# 选择所有字符型的列
drinks.select_dtypes(include=['object']).head()
drinks.select_dtypes(include=['number','object','category','datetime']).head()
# 用 exclude 关键字排除指定的数据类型
drinks.select_dtypes(exclude=['number']).head()
```
<a name="jOyK8"></a>
## 7、字符串转换为数值
```python
df = pd.DataFrame({'列1':['1.1','2.2','3.3'],
                  '列2':['4.4','5.5','6.6'],
                  '列3':['7.7','8.8','-']})
df
df.astype({'列1':'float','列2':'float'}).dtypes
```
用这种方式转换第三列会出错，因为这列里包含一个代表 0 的下划线，pandas 无法自动判断这个下划线。为了解决这个问题，可以使用 `to_numeric()` 函数来处理第三列，让 pandas 把任意无效输入转为 NaN。
```python
df = df.apply(pd.to_numeric, errors='coerce').fillna(0)
```
<a name="kA8JI"></a>
## 8、优化 DataFrame 对内存的占用
方法一：只读取切实所需的列，使用`usecols`参数
```python
cols = ['beer_servings','continent']
small_drinks = pd.read_csv('data/drinks.csv', usecols=cols)
```
方法二：把包含类别型数据的 object 列转换为 Category 数据类型，通过指定 dtype 参数实现。
```python
dtypes ={'continent':'category'}
smaller_drinks = pd.read_csv('data/drinks.csv',usecols=cols, dtype=dtypes)
```
<a name="nvUb1"></a>
## 9、根据最大的类别筛选 DataFrame
```python
movies = pd.read_csv('data/imdb_1000.csv')
counts = movies.genre.value_counts()
movies[movies.genre.isin(counts.nlargest(3).index)].head()
```
<a name="N33DM"></a>
## 10、把字符串分割为多列
```python
df = pd.DataFrame({'姓名':['张 三','李 四','王 五'],
                   '所在地':['北京-东城区','上海-黄浦区','广州-白云区']})
df
df.姓名.str.split(' ', expand=True)
```
<a name="wSSkX"></a>
## 11、把 Series 里的列表转换为 DataFrame
```python
df = pd.DataFrame({'列1':['a','b','c'],'列2':[[10,20], [20,30], [30,40]]})
df

df_new = df.列2.apply(pd.Series)
pd.concat([df,df_new], axis='columns')
```
<a name="hlK7J"></a>
## 12、用多个函数聚合
```python
orders = pd.read_csv('data/chipotle.tsv', sep='\t')
orders.groupby('order_id').item_price.agg(['sum','count']).head()
```
<a name="mpBJ1"></a>
## 13、分组聚合
```python
import pandas as pd
df = pd.DataFrame({'key1':['a', 'a', 'b', 'b', 'a'],
    'key2':['one', 'two', 'one', 'two', 'one'],
    'data1':np.random.randn(5),
     'data2':np.random.randn(5)})
df

for name, group in df.groupby('key1'):
    print(name)
    print(group)

dict(list(df.groupby('key1')))
```
通过字典或Series进行分组
```python
people = pd.DataFrame(np.random.randn(5, 5),
     columns=['a', 'b', 'c', 'd', 'e'],
     index=['Joe', 'Steve', 'Wes', 'Jim', 'Travis'])
mapping = {'a':'red', 'b':'red', 'c':'blue',
     'd':'blue', 'e':'red', 'f':'orange'}
by_column = people.groupby(mapping, axis=1)
by_column.sum()
```
