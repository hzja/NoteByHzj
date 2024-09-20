PythonPandas<br />在数据分析和数据建模的过程中需要对数据进行清洗和整理等工作，有时需要对数据增删字段。下面为大家介绍Pandas对数据的修改、数据迭代以及函数的使用。
<a name="JhacS"></a>
## 添加修改
数据的修改、增加和删除在数据整理过程中时常发生。修改的情况一般是修改错误、格式转换，数据的类型修改等。
<a name="KUq3Z"></a>
### **1、修改数值**
```python
df.iloc[0,0] # 查询值
# 'Liver'
df.iloc[0,0] = 'Lily' # 修改值
df.iloc[0,0] # 查看结果
# 'Lily'

# 将小于60分的成绩修改为60
df[df.Q1 < 60] = 60
# 查看
df.Q1

# 生成一个长度为100的列表
v = [1, 3, 5, 7, 9] * 20
```
<a name="j5VWp"></a>
### **2、替换数据**
```python
s.replace(0, 5) # 将列数据中的0换为5
df.replace(0, 5) # 将数据中的所有0换为5
df.replace([0, 1, 2, 3], 4) # 将0～3全换成4
df.replace([0, 1, 2, 3], [4, 3, 2, 1]) # 对应修改
s.replace([1, 2], method='bfill') # 向下填充
df.replace({0: 10, 1: 100}) # 字典对应修改
df.replace({'Q1': 0, 'Q2': 5}, 100) # 将指定字段的指定值修改为100
df.replace({'Q1': {0: 100, 4: 400}}) # 将指定列里的指定值替换为另一个指定的值
```
<a name="fcHHr"></a>
### **3、填充空值**
```python
df.fillna(0) # 将空值全修改为0
# {'backfill', 'bfill', 'pad', 'ffill',None}, 默认为None
df.fillna(method='ffill') # 将空值都修改为其前一个值
values = {'A': 0, 'B': 1, 'C': 2, 'D': 3}
df.fillna(value=values) # 为各列填充不同的值
df.fillna(value=values, limit=1) # 只替换第一个
```
<a name="wER5Z"></a>
### **4、修改索引名**
```python
df.rename(columns={'team':'class'})
```
常用方法如下：
```python
df.rename(columns={"Q1":"a", "Q2": "b"}) # 对表头进行修改
df.rename(index={0: "x", 1:"y", 2: "z"}) # 对索引进行修改
df.rename(index=str) # 对类型进行修改
df.rename(str.lower, axis='columns') # 传索引类型
df.rename({1: 2, 2: 4}, axis='index')

# 对索引名进行修改
s.rename_axis("animal")
df.rename_axis("animal") # 默认是列索引
df.rename_axis("limbs",axis="columns") # 指定行索引

# 索引为多层索引时可以将type修改为class
df.rename_axis(index={'type': 'class'})

# 可以用set_axis进行设置修改
s.set_axis(['a', 'b', 'c'], axis=0)
df.set_axis(['I', 'II'], axis='columns')
df.set_axis(['i', 'ii'], axis='columns',inplace=True)
```
<a name="ciMDW"></a>
### **5、增加列**
```python
df['foo'] = 100 # 增加一列foo，所有值都是100
df['foo'] = df.Q1 + df.Q2 # 新列为两列相加
df['foo'] = df['Q1'] + df['Q2'] # 同上
# 把所有为数字的值加起来
df['total'] =df.select_dtypes(include=['int']).sum(1)df['total'] =
df.loc[:,'Q1':'Q4'].apply(lambda x: sum(x), axis='columns')
df.loc[:, 'Q10'] = '我是新来的' # 也可以
# 增加一列并赋值，不满足条件的为NaN
df.loc[df.num >= 60, '成绩'] = '合格'
df.loc[df.num < 60, '成绩'] = '不合格'
```
<a name="v1oMA"></a>
### **6、插入列**`**df.insert()**`
```python
# 在第三列的位置上插入新列total列，值为每行的总成绩
df.insert(2, 'total', df.sum(1))
```
<a name="gC1GM"></a>
### **7、指定列**`**df.assign()**`
```python
# 增加total列
df.assign(total=df.sum(1))
# 增加两列
df.assign(total=df.sum(1), Q=100)
df.assign(total=df.sum(1)).assign(Q=100)
其他使用示例：
df.assign(Q5=[100]*100) # 新增加一列Q5
df = df.assign(Q5=[100]*100) # 赋值生效
df.assign(Q6=df.Q2/df.Q1) # 计算并增加Q6
df.assign(Q7=lambda d: d.Q1 * 9 / 5 + 32) # 使用lambda# 添加一列，值为表达式结果：True或False
df.assign(tag=df.Q1>df.Q2)
# 比较计算，True为1，False为0
df.assign(tag=(df.Q1>df.Q2).astype(int))
# 映射文案
df.assign(tag=(df.Q1>60).map({True:'及格',False:'不及格'}))
# 增加多个
df.assign(Q8=lambda d: d.Q1*5,
          Q9=lambda d: d.Q8+1) # Q8没有生效，不能直接用df.Q8
```
<a name="Lq2mP"></a>
### **8、执行表达式**`**df.eval()**`
```python
# 传入求总分表达式
df.eval('total = Q1+Q3+Q3+Q4')
```
其他方法：
```python
df['C1'] = df.eval('Q2 + Q3')
df.eval('C2 = Q2 + Q3') # 计算
a = df.Q1.mean()df.eval("C3 =`Q3`+@a") # 使用变量
df.eval("C3 = Q2 > (`Q3`+@a)") #加一个布尔值
df.eval('C4 = name + team', inplace=True) # 立即生效
```
<a name="fwsDh"></a>
### **9、增加行**
```python
# 新增索引为100的数据
df.loc[100] = ['tom', 'A', 88, 88, 88, 88]
```
其他方法：
```python
df.loc[101]={'Q1':88,'Q2':99} # 指定列，无数据列值为NaN
df.loc[df.shape[0]+1] = {'Q1':88,'Q2':99} # 自动增加索引
df.loc[len(df)+1] = {'Q1':88,'Q2':99}
# 批量操作，可以使用迭代
rows = [[1,2],[3,4],[5,6]]
for row in rows:
    df.loc[len(df)] = row
```
<a name="KxDIq"></a>
### **10、追加合并**
```python
df = pd.DataFrame([[1, 2], [3, 4]],columns=list('AB'))
df2 = pd.DataFrame([[5, 6], [7, 8]],columns=list('AB'))
df.append(df2)
```
<a name="LwExo"></a>
### **11、删除**
```python
# 删除索引为3的数据
s.pop(3)
# 93s
s
```
<a name="SxAmQ"></a>
### **12、删除空值**
```python
df.dropna() # 一行中有一个缺失值就删除
df.dropna(axis='columns') # 只保留全有值的列
df.dropna(how='all') # 行或列全没值才删除
df.dropna(thresh=2) # 至少有两个空值时才删除
df.dropna(inplace=True) # 删除并使替换生效
```
<a name="ICgg9"></a>
## 高级过滤
介绍几个非常好用的复杂数据处理的数据过滤输出方法。
<a name="JGcE6"></a>
### **1、**`**df.where()**`
```python
# 数值大于70
df.where(df > 70)
```
<a name="Mjf8d"></a>
### **2、**`**np.where()**`
```python
# 小于60分为不及格
np.where(df>=60, '合格', '不合格')
```
<a name="N3aQ1"></a>
### **3、**`**df.mask()**`
```python
# 符合条件的为NaN
df.mask(s > 80)
```
<a name="FONss"></a>
### **4、**`**df.lookup()**`
```python
# 行列相同数量，返回一个array
df.lookup([1,3,4], ['Q1','Q2','Q3']) # array([36, 96, 61])
df.lookup([1], ['Q1']) # array([36])
```
<a name="Q81gU"></a>
## **数据迭代**
<a name="PDA9o"></a>
### **1、迭代Series**
```python
# 迭代指定的列
for i in df.name:
    print(i)
# 迭代索引和指定的两列
for i,n,q in zip(df.index, df.name,df.Q1):
    print(i, n, q)
```
<a name="yrIge"></a>
### **2、**`**df.iterrows()**`
```python
# 迭代，使用name、Q1数据
for index, row in df.iterrows():
    print(index, row['name'], row.Q1)
```
<a name="xz7xG"></a>
### **3、**`**df.itertuples()**`
```python
for row in df.itertuples():
    print(row)
```
<a name="z2mBh"></a>
### **4、**`**df.items()**`
```python
# Series取前三个
for label, ser in df.items():
    print(label)
    print(ser[:3], end='\n\n')
```
<a name="IHq7t"></a>
### **5、按列迭代**
```python
# 直接对DataFrame迭代
for column in df:
    print(column)
```
<a name="ztj8G"></a>
## 函数应用
<a name="suO7k"></a>
### **1、**`**pipe()**`
应用在整个DataFrame或Series上。
```python
# 对df多重应用多个函数
f(g(h(df), arg1=a), arg2=b, arg3=c)
# 用pipe可以把它们连接起来
(df.pipe(h)
    .pipe(g, arg1=a)
    .pipe(f, arg2=b, arg3=c)
)
```
<a name="O2jyl"></a>
### **2、**`**apply()**`
应用在DataFrame的行或列中，默认为列。
```python
# 将name全部变为小写
df.name.apply(lambda x: x.lower())
```
<a name="drO9s"></a>
### **3、**`**applymap()**`
应用在DataFrame的每个元素中。
```python
# 计算数据的长度
def mylen(x):
    return len(str(x))
df.applymap(lambda x:mylen(x)) # 应用函数
df.applymap(mylen) # 效果同上
```
<a name="SQBpS"></a>
### **4、**`**map()**`
应用在Series或DataFrame的一列的每个元素中。
```python
df.team.map({'A':'一班', 'B':'二班','C':'三班', 'D':'四班',})# 枚举替换
df['name'].map(f)
```
<a name="tZPRP"></a>
### **5、**`**agg()**`
```python
# 每列的最大值
df.agg('max')
# 将所有列聚合产生sum和min两行
df.agg(['sum', 'min'])
# 序列多个聚合
df.agg({'Q1' : ['sum', 'min'], 'Q2' : ['min','max']})
# 分组后聚合
df.groupby('team').agg('max')
df.Q1.agg(['sum', 'mean'])
```
<a name="Z0whs"></a>
### **6、**`**transform()**`
```python
df.transform(lambda x: x*2) # 应用匿名函数
df.transform([np.sqrt, np.exp]) # 调用多个函数
```
<a name="TGvif"></a>
### **7、**`**copy()**`
```python
s = pd.Series([1, 2], index=["a","b"])
s_1 = s
s_copy = s.copy()
s_1 is s # True
s_copy is s # False
```

