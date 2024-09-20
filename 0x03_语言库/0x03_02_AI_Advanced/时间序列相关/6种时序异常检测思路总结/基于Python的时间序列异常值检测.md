Python<br />介绍一下使用python做时间序列数据分析和预测中异常值检测的方法，常用的异常值检测方法有以下几种：

- 3sigma：基于正太分布，当数据值超过±3个标准差(3sigma)时为异常值。
- z-score：z标准分数，它测量数据值到平均值的距离，当数据与平均值相差2个标准差时z-score为2，如果将z-score为3作为异常值判断标准时，便相当于3sigma。
- 箱体法(box)：它基于数据的四分位值来判断异常值。
- 多维度异常值判断法，通过数据特征的多个维度综合判断数据是否为异常值。

注：3sigma，z-score，箱体法(box)都是从数据值本身的单一维度去分析和判断异常值，从而有一定的局限性，然而多维度异常值判断法更注重从数据特征的各个维度去分析和判断异常值，显然多维度异常值判断法更为科学和精准。
<a name="YOpOH"></a>
## 导入时间序列数据
数据来自于某商业零售门店的每日客流量数据，客流量数据直接关系到门店销售业绩，所以有必要对客流量数据进行分析。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699508926-5fc9461d-8f61-4a12-afe7-ceb7eef612d0.png#averageHue=%23f4f1ef&clientId=uebe02699-24bd-4&from=paste&id=u34df96a6&originHeight=398&originWidth=164&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u503ed195-c884-47e1-9984-44f3e172d51&title=)<br />数据中的 y 列代表了客流量，这里数据的时间范围为2020.1至2023.1 ，接下来查看数据的趋势图。
```python
plt.figure(figsize=(10,4),dpi=100)
plt.plot(df)
plt.title("客流量趋势")
plt.show()
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699508939-0adf1dfc-420f-48f7-a3af-0ddb6afceb76.png#averageHue=%23fafafa&clientId=uebe02699-24bd-4&from=paste&id=ub220ad4c&originHeight=400&originWidth=897&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9460543a-7c26-4c3b-a828-4c96dfb0a7d&title=)<br />下面查看客流量数据的热力图分布：
```python
calplot.calplot(df.y,suptitle='客流量分布',cmap='YlGn');
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699508967-3cd025b4-c78e-4c4b-82f9-86fc2b8bfae0.png#averageHue=%23eff4e1&clientId=uebe02699-24bd-4&from=paste&id=u364705ad&originHeight=644&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b93ea3d-924a-474a-beda-141667acf46&title=)<br />从热力图的颜色深浅变化，也能发现客流量逐年在减少，这可能和疫情持续有关。
<a name="uJ41B"></a>
## 3sigma
依据正太分布异常值分布在3个标准差以外的位置，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699508988-8acb13e8-d4ac-461c-8dcc-a128b52cc64e.png#averageHue=%23e2e1d9&clientId=uebe02699-24bd-4&from=paste&id=ud71e557e&originHeight=336&originWidth=613&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93587083-897e-4cbe-a0d0-27a1b9501ef&title=)<br />下面来计算数据的±3个标准以外的位置，落在这两个位置内的数据点即为异常值：
```python
# 3sigma
def three_sigma(df):
    mean=df.y.mean()
    std=df.y.std()
    upper_limit=mean+3*std
    lower_limit=mean-3*std
    df['anomaly']=df.y.apply(lambda x: 1 if (x>upper_limit )
                             or (x<lower_limit) else 0)
    return df
df1 = three_sigma(df.copy())
df1[df1.anomaly==1]
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699508999-4d630388-beb3-4feb-a4d1-c6b074281193.png#averageHue=%23f3f0ee&clientId=uebe02699-24bd-4&from=paste&id=u192d4669&originHeight=514&originWidth=199&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubefaf461-ff3a-4698-8da1-5b0c0b3f05c&title=)
```python
fig, ax = plt.subplots(figsize=(10,4))
a = df1.loc[df1['anomaly'] == 1, ['y']] #anomaly
ax.plot(df.index, df['y'], color='blue', label='正常值')
ax.scatter(a.index,a['y'], color='red', label='异常值')
plt.title(f'3sigma')
plt.xlabel('date')
plt.ylabel('y')
plt.legend()
plt.show();
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699509898-28a8fcc5-e5e4-4aa0-a964-d6d879388b3e.png#averageHue=%23faf9f9&clientId=uebe02699-24bd-4&from=paste&id=u9cb49327&originHeight=399&originWidth=925&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf56ba72f-0d10-4ca5-934c-83e54d9e07c&title=)
<a name="zsUDt"></a>
## z-score
z-score测量数据值到平均值的距离，异常值的判断依据为给定的距离阈值，一般情况下阈值可以设置在大于2个标准差的任意位置(依据业务和经验来确定阈值)。如果将z-score为3作为异常值判定的阈值时，便相当于3sigma。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699509820-2b0aceea-4a8d-4025-a4cb-e130d6290f41.png#averageHue=%23eee1d9&clientId=uebe02699-24bd-4&from=paste&id=u70c0bfe8&originHeight=533&originWidth=632&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u25e33f45-9ca0-4a20-a99a-0e171ca6df3&title=)
```python
# Z-Score
def z_score(df,threshold):
    mean=df.y.mean()
    std=df.y.std()
    df['z_score']=df.y.apply(lambda x:abs(x-mean)/std)
    df['anomaly']=df.z_score.apply(lambda x: 1 if x>threshold else 0)
    return df

#设置阈值为2或3，当阈值为3时便相当于3sigma
threshold=2
df2 = z_score(df.copy(),threshold)
df2[df2.anomaly==1]
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699509781-6b42be66-3025-469f-a816-f32c3a795550.png#averageHue=%23f3efec&clientId=uebe02699-24bd-4&from=paste&id=uf35f3806&originHeight=519&originWidth=267&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u52e217be-44a0-4112-94bc-85e38de825b&title=)
```python
fig, ax = plt.subplots(figsize=(10,4))
a = df2.loc[df2['anomaly'] == 1, ['y']] 
ax.plot(df.index, df['y'], color='blue', label='正常值')
ax.scatter(a.index,a['y'], color='red', label='异常值')
plt.title(f'Z-score, {threshold=}')
plt.xlabel('date')
plt.ylabel('y')
plt.legend()
plt.show();
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699510341-5c9505e6-7cd2-48ac-ae88-5d3c8de95d19.png#averageHue=%23faf8f8&clientId=uebe02699-24bd-4&from=paste&id=ufddc2e22&originHeight=397&originWidth=916&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u592bb6aa-acbe-4f7c-8c11-dd8ec9a1e08&title=)
<a name="Hjn5B"></a>
## 箱体法(box)
箱体法(box)基于数据的四分位值来判断异常值。异常值>Q3+1.5*IQR 或者 异常值<Q1-1.5*IQR<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699510458-d41d11d8-9029-47eb-a1a0-a64c79f702f9.png#averageHue=%23f5f4f4&clientId=uebe02699-24bd-4&from=paste&id=ue566f690&originHeight=258&originWidth=726&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u68f8b9ee-81c7-4025-ba08-6e834353280&title=)
```python
def box_plot(df):
    q1=np.nanpercentile(df.y,25)
    q3=np.nanpercentile(df.y,75)
    iqr=q3-q1
    lower_limit=q1-1.5*iqr
    upper_limit=q3+1.5*iqr
    df['anomaly']=df.y.apply(lambda x: 1 if x<lower_limit or x>upper_limit  
                             else 0)
    return df
df3 = box_plot(df.copy())
df3[df3.anomaly==1]
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699510491-45f1aa92-1015-4d9f-8351-082e76f44711.png#averageHue=%23f3f1ee&clientId=uebe02699-24bd-4&from=paste&id=uc47ba948&originHeight=462&originWidth=200&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u71a690aa-7d98-43a6-8f7e-963777dcc9f&title=)
```python
fig, ax = plt.subplots(figsize=(10,4))
a = df3.loc[df3['anomaly'] == 1, ['y']] 
ax.plot(df.index, df['y'], color='blue', label='正常值')
ax.scatter(a.index,a['y'], color='red', label='异常值')
plt.title(f'Box-plot')
plt.xlabel('date')
plt.ylabel('y')
plt.legend()
plt.show();
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699510771-7435c0a3-1fed-4002-8be2-82b9083b1606.png#averageHue=%23f9f8f8&clientId=uebe02699-24bd-4&from=paste&id=uac205b84&originHeight=404&originWidth=910&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9bc0b531-2cb3-494d-b459-019709bc75c&title=)
<a name="q9Xh9"></a>
## 多维度异常检测法PyOD
异常检测算法工具库(PyOD) 可以从数据的多个特征维度来检测异常值，所以可以将时间序列数据的日期特征分解成多个和时间相关的其它特征，同时还需要设置一个异常值比例，一般情况下设置异常值比例在5%以下。这里使用的是Pycaret的异常值检测模型，该模型是对PyOD进行了再次包装，使之调用更为简单。这里首先将日期字段进行分解，从原始的日期字段中可以拆分出年，月，日，星期，季度等和时间相关的特征：
```python
from pycaret.anomaly import AnomalyExperiment
# 分解日期特征
def create_features(df):        
    df['year'] = df.index.year #年 
    df['month'] = df.index.month #月
    df['dayofmonth'] = df.index.day #日
    df['dayofweek'] = df.index.dayofweek #星期
    df['quarter'] = df.index.quarter #季度
    df['weekend'] = df.dayofweek.apply(lambda x: 1 if x > 5 else 0) #是否周末
    df['dayofyear'] = df.index.dayofyear   #年中第几天
    df['weekofyear'] = df.index.weekofyear #年中第几月
    df['is_month_start']=df.index.is_month_start
    df['is_month_end']=df.index.is_month_end
    return df
#创建特征
df4 = create_features(df.copy())
df4
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699511112-6bca5dd4-54d0-4e4d-87d2-835a5b7c52cf.png#averageHue=%23f6f5f4&clientId=uebe02699-24bd-4&from=paste&id=ue64af3de&originHeight=378&originWidth=902&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue99e676c-ac17-4447-9b96-484cde8d6e1&title=)<br />接下来使用Pycaret的anomaly模型对新数据集进行建模和预测，同时仍然需要设置一个异常值比例的阈值fraction：
```python
#异常值算法：'knn','cluster','iforest','svm'等。
alg='knn'#异常值算法
fraction=0.02 #异常值比例 0.02,0.03,0.04,0.05
#创建异常值模型
exp = AnomalyExperiment()
r = exp.setup(df4.copy(), session_id = 123,verbose=False)
model = exp.create_model(alg, fraction=fraction,verbose=False)
model_results = exp.assign_model(model,verbose=False)
#获取检测结果
df5 = pd.merge(df.reset_index(),model_results[['Anomaly']],
               left_index=True, right_index=True)
df5.set_index('date',inplace=True)

fig, ax = plt.subplots(figsize=(10,4))
a = df5.loc[df5['Anomaly'] == 1, ['y']] 
ax.plot(df5.index, df5['y'], color='blue', label='正常值')
ax.scatter(a.index,a['y'], color='red', label='异常值')
plt.title(f'Pycaret.anomaly {fraction=}')
plt.xlabel('date')
plt.ylabel('y')
plt.legend()
plt.show();
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699511417-e47163dc-1f18-49d5-99ef-d0f77f0ed45f.png#averageHue=%23faf9f9&clientId=uebe02699-24bd-4&from=paste&id=u01ddd708&originHeight=401&originWidth=919&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b9ad560-a78b-46a4-8960-1feae42bc27&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699511557-977365d4-fbdb-4fcb-9a57-2142bc06d889.png#averageHue=%23faf9f9&clientId=uebe02699-24bd-4&from=paste&id=u565e1d95&originHeight=423&originWidth=922&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u03310e28-2271-430d-a23d-115163a7c2a&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699511480-b1dc6f70-946d-4bb6-9023-aa6abb37c3ca.png#averageHue=%23faf9f9&clientId=uebe02699-24bd-4&from=paste&id=udd3c2b90&originHeight=392&originWidth=916&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udcfc82c4-0e1b-45ed-b4de-4f00c6056a2&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699511744-95889556-0d82-4ac0-8ebf-e560deebaf4a.png#averageHue=%23faf9f9&clientId=uebe02699-24bd-4&from=paste&id=u40cdbdd3&originHeight=397&originWidth=911&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub010c63c-283d-456e-95b2-2f8ceaeddb1&title=)<br />这里还有多种异常值算法可以选择，有兴趣的朋友可以自己去尝试不同的异常值算法：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683699511720-3ec5c0b0-6c3f-4c1f-ab8d-cd491f2eae6d.png#averageHue=%23f7f4f2&clientId=uebe02699-24bd-4&from=paste&id=u3b7827b8&originHeight=423&originWidth=554&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u00ffc30d-2291-497c-8593-e4577936d54&title=)
<a name="NdD7I"></a>
## 总结
介绍几种常用的异常值检测方法，其中3sigma，z-score，箱体法(box)都是从数据值本身的单一维度去分析和判断异常值，从而有一定的局限性，而多维度异常值判断法更注重从数据特征的各个维度去分析和判断异常值，显然多维度异常值判断法更为科学和精准。
<a name="Z75zg"></a>
## 参考资料
Pycaret 文档：[https://pycaret.gitbook.io/docs/](https://pycaret.gitbook.io/docs/)<br />PyOD文档：[https://pyod.readthedocs.io/en/latest/](https://pyod.readthedocs.io/en/latest/)
