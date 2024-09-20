Python Pandas
<a name="tAoKj"></a>
## Pandas的下载
使用命令下载：
```bash
pip install pandas 
```
![](./img/1637984401347-e919a4a3-c75a-47d4-9787-7a3c5769aa18.gif)<br />或者自行下载whl文件安装：[https://www.lfd.uci.edu/~gohlke/pythonlibs/](https://www.lfd.uci.edu/~gohlke/pythonlibs/)<br />![](./img/1637984401440-23638936-0bfc-4793-bcb1-366dd7f2c75b.webp)
<a name="eKK58"></a>
## 创建DataFrame数据
```python
pd_data = pd.DataFrame({
    "name":["小明","小红","小孙","王小","关宇","刘蓓","张菲"],
    "age":[20,18,27,20,28,18,25],
    "sex":["男","女","男","男","男","女","女"],
    "score":[669,570,642,590,601,619,701],
    "address":["北京","深圳","广州","武汉","深圳","广州","长沙"]
})

print(pd_data)
```
![](./img/1637984401313-835737e2-68ea-4cc9-a775-b88c20a6615f.webp)
<a name="slzQo"></a>
## 读取本地文件
```python
pd_data = pd.read_excel('./测试.xlsx')

pd.set_option('display.max_columns', None)   # 显示完整的列
pd.set_option('display.max_rows', None)  # 显示完整的行
pd.set_option('display.expand_frame_repr', False)  # 设置不折叠数据

print(pd_data)
```
![](./img/1637984401647-ea6a61d5-9ffd-486a-85bd-b8324436dc1a.webp)
<a name="yXBCc"></a>
## 查看数据是否有缺失
```python
# 如果缺失显示为True，否则显示False
isnull = pd_data.isnull()        
print(isnull)
```
![](./img/1637984401580-62dadee0-6bdf-4a19-8d7f-820c6a4806da.webp)
<a name="qLUQu"></a>
## 统计缺失值个数
```python
# 统计缺失值个数
null_count = pd_data.isnull().sum()
print(null_count)
```
![](./img/1637984401775-196b2d4b-c436-4639-acbb-4ee1fe8617f1.webp)
<a name="dhYKm"></a>
## 缺失值填充
```python
# 填充数据 我选择了8.888,你随意
pd_data.fillna(8.888, inplace=True)
print(pd_data)
```
![](./img/1637984401951-31916b24-9300-4401-a466-8f5cb6bd5ade.webp)
<a name="vYYmG"></a>
## 缺失值删除
```python
# 如果有缺失值，删除此行
exist_col = pd_data.dropna()
print(exist_col)
```
![](./img/1637984402222-56010562-f8b6-40d5-952f-76279c216074.webp)
<a name="P2Qte"></a>
## 查看头尾文件
```python
# 查看头尾文件
print('头文件:', pd_data.head())
print('尾文件:', pd_data.tail())
```
![](./img/1637984402391-1c100c8e-f9f6-4bba-b21b-ece68270ab7f.webp)
<a name="p3YOU"></a>
## 取单列值
```python
# 单列值
pd_data = pd.read_excel('./测试.xlsx')
print(pd_data['全款价'])
```
![](./img/1637984402768-b905e266-739c-4a48-853d-fbab3b54ef20.webp)
<a name="Z7GGB"></a>
## 取多列值
```python
# 多列值
pd_data = pd.read_excel('./测试.xlsx')
print(pd_data[['车辆概况', '全款价']])
```
![](./img/1637984403123-a490fc97-4956-4c65-8ec9-b067f4d9be35.webp)
<a name="HEX08"></a>
## 单条件取值
```python
pd_data = pd.read_excel('./测试.xlsx')
print(pd_data[pd_data['全款价'] == 4])
print('-'*100)
print(pd_data[pd_data['汽车排量'] == '2.0T'])
```
![](./img/1637984403332-3f077d85-1c25-4af7-a180-57851399f630.webp)
<a name="SwlGZ"></a>
## 多条件取值-与
```python
# 多条件筛选数据
print(pd_data[(pd_data['车龄'] == '2018年') & (pd_data['变速箱'] == '自动')])
```
![](./img/1637984403730-7ee7eeda-d0fa-4550-9d9c-690c4d1403f3.webp)
<a name="ClnlS"></a>
## 多条件取值-或
```python
# 多条件筛选数据
print(pd_data[(pd_data['车龄'] == '2018年') | (pd_data['变速箱'] == '自动')])
```
![](./img/1637984403894-606f15b9-e977-4140-9344-6afe99267726.webp)
<a name="c0Njg"></a>
## 字符串的开始函数
```python
# 找出在 车辆概况 中以'大众'开头的
cars = pd_data[pd_data['车辆概况'].str.startswith('大众')]
print(cars)
```
![](./img/1637984404199-8f579066-2114-4b82-9be8-2d962c96439b.webp)
<a name="Kwwai"></a>
## 字符串的结尾函数
```python
# 找出在 车辆概况 中以'豪华型'结尾的
cars = pd_data[pd_data['车辆概况'].str.endswith('豪华型')]
print(cars)
```
![](./img/1637984404357-b744c89c-a68d-454b-a1c3-9394cea492a6.webp)
<a name="TVzUY"></a>
## 字符串的包含函数‍‍‍‍‍‍‍‍‍‍‍‍‍‍‍
```python
# 找出在 车辆概况 中包含'进口'的
cars = pd_data[pd_data['车辆概况'].str.contains('进口')]
print(cars)
```
![](./img/1637984404581-93b8fbd6-b154-4473-9ac8-fff9b764720f.webp)
<a name="FXWTB"></a>
## 统计元素个数
```python
# 统计 过户分类 以及对应次数
trans_count = pd_data['过户情况'].value_counts()
print(trans_count)
```
![](./img/1637984404588-70bdc556-cee4-4591-818e-93a818d7a7c1.webp)<br />为了便于进一步的数据分析，将它们置于不同的数组之中，可以采用如下方法：
```python
# 统计 过户分类 以及对应次数
trans_count = pd_data['过户情况'].value_counts()
# 针对于过户情况的分类
x1_data = trans_count.index.tolist()    
# 分类后各组数据的统计
x2_data = trans_count.tolist()      

print(x1_data)
print(x2_data)
```
![](./img/1637984404792-b440dc27-efc3-4f68-9412-751c6bf811a8.webp)<br />这种格式的数据才是最适合做可视化分析的！这里再多介绍两种方法，条条大路通罗马，都能轻松实现目标。
```python
# 统计 过户分类 以及对应次数
trans_count = pd_data['过户情况'].value_counts()
# 针对于过户情况的分类
x1_data = trans_count.index.tolist()
x11_data = trans_count.index
x12_data = trans_count.index.values
# 类后各组数据的统计
x2_data = trans_count.tolist()
print('index.tolist():', x1_data)
print('index:', x11_data)
print('index.values:', x12_data)
print('x2:', x2_data)
```
![](./img/1637984404668-e6508a65-0b37-4092-a464-5b072bdfdb40.webp)
<a name="X2ED8"></a>
## 分割字符串
这个功能也很实用，大家可以看看汽车名称数据这一列，目标仅仅是车名而已，后面的车型、车龄、排列、变速箱信息都是冗余信息。非常不利于后续数据可视化，所以字符串分割在这里就显得尤为重要。<br />![](./img/1637984404974-86ae8e74-0858-41bd-8c04-a86ce980c839.webp)
```python
# 对 汽车名称 这一列按照空格分割 并取第一个字符
pd_data['汽车名称'] = pd_data['车辆概况'].map(lambda x: x.split(" ")[0])
name = pd_data['汽车名称'].value_counts()
# 汽车名称分类
name1 = name.index.tolist()  
# 汽车名称对应数量
name2 = name.tolist()  
print(name1)
print(name2)
```
![](./img/1637984404894-942d8b06-3e62-43f5-a0b7-fbea84eb64af.webp)<br />看到取出来数据的样子了吗，要的就是这个！
<a name="Fusq9"></a>
## 清理数据
当相对汽车里程做进一步的分析时会发现数据后面都有一个'万公里'，这种数据要做可视化必须先对数据进行处理，<br />就是先要去除数字后面的字符<br />![](./img/1637984405237-f2ee8c80-e7e2-4057-9225-cd8c84d04f5d.webp)<br />可以使用字符串的`replace()`方法，使用空格替换字符
```python
pd_data.loc[:, '表显里程new'] = pd_data['表显里程'].str.replace('万公里', '').astype('float32')  # 去除 30 ’万公里‘
# 保存数据
pd_data.to_excel('测试1.xlsx')
```
黄色一列是处理之前的数据，绿色一列是处理之后的数据，已经达到了想要的效果<br />![](./img/1637984405346-6ca1a223-2e9a-4130-86fa-591b249d8bb9.webp)
<a name="VZc55"></a>
## 划分区间
现在有这么一个需求，想要按照汽车的行驶里程分类，基本上每个车的行驶里程都是不一样的，如果将每个数据都反映在图标上就会看起来很冗余，<br />也就失去了作图的意义，所以可以按照区间来划分，例如5w-10w公里、10w-15w公里这样图表展示展示出来的效果就会很好了。<br />![](./img/1637984405659-fb926609-588f-43d1-8278-1f6e9f3490fe.webp)
```python
pd_data.loc[:, '表显里程new'] = pd_data['表显里程'].str.replace('万公里', '').astype('float32')  # 去除 30 ’万公里‘
# 划分区间
pd_data['里程区间'] = pd.cut(pd_data['表显里程new'], [0, 2, 4, 6, 8, 10, 20],
                             labels=['0-2', '2-4', '4-6', '6-8', '8-10', '>10'])
mile = pd_data['里程区间'].value_counts()
mile1 = mile.index.tolist()         # 里程区间分类
mile2 = mile.tolist()               # 里程区间分类对应数量
print(mile1)
print(mile2)
```
![](./img/1637984405861-1731763e-9fc9-4417-8597-4ef9fb59f8f7.webp)
<a name="Ry8yR"></a>
## 重置索引
其实在上面案例的演示中已经发现了，根据条件取出来的数据的索引都是处理数据之前的索引，<br />现在要重置索引的话要怎么办呢？可以使用`reset_index()`来索引重置，重置索引前：
```python
# 找出在 过户情况 中所有'0次'的汽车
cars = pd_data[pd_data['过户情况'].str.contains('0次')]
print(cars.reset_index())
```
![](./img/1637984406096-9a579bb5-14d3-4a3a-adaa-12b23719775e.webp)<br />重置索引后：<br />![](./img/1637984406478-10d732b8-314d-47d1-91dd-d717ef73604e.webp)<br />很好，但是不完美。多了一列colm name叫做 index的先前序列号。不想看到它，有办法吗？<br />`drop = True`
```python
# 找出在 过户情况 中所有'0次'的汽车
cars = pd_data[pd_data['过户情况'].str.contains('0次')]
print(cars.reset_index(drop=True))
```
![](./img/1637984406281-06c602ec-1249-47f3-a525-888aea3b7bce.webp)
<a name="VqPEz"></a>
## column重命名
```python
# 重命名
pd_data = pd_data.rename(columns = {'车辆概况':'车辆详情'})
print(pd_data)
```
![](./img/1637984406715-bf20cab2-b73a-48f5-a5e3-fc1fcf9fe1e2.webp)
<a name="u7jOF"></a>
## 分组统计`groupby`-单条件
```python
# 统计不同变速箱总里程
pd_data.loc[:, '表显里程new'] = pd_data['表显里程'].str.replace('万公里', '').astype('float32')    # 去除 30 ’万公里‘
trans_mile = pd_data.groupby('变速箱')['表显里程new'].sum()
print(trans_mile)
```
![](./img/1637984406698-6f6d1236-c5b9-468e-8a13-a15eb4d1993a.webp)
<a name="a6gIu"></a>
## 分组统计`groupby`-多条件
```python
# 统计不同变速箱和过户情况总里程
pd_data.loc[:, '表显里程new'] = pd_data['表显里程'].str.replace('万公里', '').astype('float32')    # 去除 30 ’万公里‘
trans_mile = pd_data.groupby(['变速箱','过户情况'])['表显里程new'].sum()
print(trans_mile)
```
![](./img/1637984406848-aa3c97b4-96a2-41be-9ada-8e838016deac.webp)<br />如果再加上一个重置索引 `trans_mile.reset_index()`<br />![](./img/1637984406903-92bb5df1-3281-4e10-9b11-cb86d39f3f8d.webp)
<a name="Se0Hx"></a>
## 求平均
```python
# 统计不同过户次数车辆平均里程
pd_data.loc[:, '表显里程new'] = pd_data['表显里程'].str.replace('万公里', '').astype('float32')    # 去除 30 ’万公里‘
trans_mile = pd_data.groupby('过户情况')['表显里程new'].mean()
print(trans_mile.reset_index())
```
![](./img/1637984407212-63a64aaf-ccf4-4923-8949-6a9c3a43f7bf.webp)
<a name="XfrNJ"></a>
## `apply`函数
爬出来的大学的数据如果是985或者是211显示为1，如果非985或者211，显示为2<br />![](./img/1637984407233-c0a2bc69-e5ff-46c8-983a-df375629f6a7.webp)<br />现在不要1和2了，因为看不懂它是什么意思？如果是985或者211，就显示是，如果不是，就显示否！
```python
pd_data = pd.read_excel('./全国高校数据.xlsx')
print(pd_data)
pd_data1 = pd_data.copy()  # 生成一个副本, 防止数据损坏
pd_data['f985'] = pd_data['f985'].apply(lambda x: '是' if x == 1 else '否')       # 通过匿名函数解决
pd_data['f211'] = pd_data['f985'].apply(lambda x: '是' if x == 1 else '否')       # 通过匿名函数解决
print(pd_data)
```
![](./img/1637984407402-c88d594a-a0f9-4934-a29f-0b88f80ac338.webp)<br />同理利用`lambda`函数还可以给省份这一列后面加个'省'
```python
pd_data = pd.read_excel('./全国高校数据.xlsx')
print(pd_data)
pd_data1 = pd_data.copy()  # 生成一个副本, 防止数据损坏

pd_data['province_name'] = pd_data['province_name'].apply(lambda x: x+'省')       # 通过匿名函数解决
print(pd_data)
'''
```
![](./img/1637984407458-04240256-335f-4a8a-a018-36932cdff4a2.webp)<br />同理利用`lambda`函数还可以给人气值`view_total`这一列最后面的'w'
```python
pd_data['view_total'] = pd_data['view_total'].apply(lambda x: x[:-1])       # 通过匿名函数解决
print(pd_data)
```
![](./img/1637984407693-2c481765-f5c5-49ab-8861-5a63357f9ae6.webp)
<a name="sxSlq"></a>
## 求最大最小值
```python
max_view_total = pd_data[pd_data['view_total'] == pd_data['view_total'].max()]
print(max_view_total)
```
![](./img/1637984407590-05210960-b118-4197-844a-39d7392ae247.webp)
```python
min_view_total = pd_data[pd_data['view_total'] == pd_data['view_total'].min()]
print(min_view_total)
```
![](./img/1637984407888-01fe9236-5092-4172-9aff-71caacb4e074.webp)
<a name="QseWW"></a>
## 时间提取
为了便于演示，加上了一列 Date 选项，如下：<br />![](./img/1637984408246-38c6f5f6-71e8-4ce6-ad1f-80692e9f9348.webp)<br />现在想提取其中的年份或者月份，可以使用 '`DatetimeIndex`'这个方法来实现。
```python
pd_data = pd.read_excel('./全国高校数据.xlsx')
pd_data['year'] = pd.DatetimeIndex(pd_data['Date']).year
pd_data['month'] = pd.DatetimeIndex(pd_data['Date']).month
pd_data['day'] = pd.DatetimeIndex(pd_data['Date']).day
print(pd_data)
```
![](./img/1637984408108-9ffd2f09-fe63-4886-b310-20425bc177aa.webp)
<a name="hhciS"></a>
## 增加列
把刚才的生成的年+月+日方法到前三列，可以使用`insert()`方法来实现
```python
Year = pd.DatetimeIndex(pd_data['Date']).year
Month = pd.DatetimeIndex(pd_data['Date']).month
day = pd.DatetimeIndex(pd_data['Date']).day
pd_data.insert(0, 'Year', Year)
pd_data.insert(1, 'Month', Month)
pd_data.insert(2, 'day', day)
print(pd_data)
```
![](./img/1637984408308-c34caac5-f7b5-490a-b153-148ae4793fa0.webp)
