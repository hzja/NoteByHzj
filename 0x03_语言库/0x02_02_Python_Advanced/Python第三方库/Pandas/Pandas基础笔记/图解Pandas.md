PythonPandas<br />Pandas是数据挖掘常见的工具，掌握使用过程中的函数是非常重要的。这里将借助可视化的过程，讲解Pandas的各种操作。
<a name="bQ1ao"></a>
## sort_values
```python
(dogs[dogs['size'] == 'medium']
 .sort_values('type')
 .groupby('type').median()
)
```
执行步骤：

- **size列筛选出部分行**
- **然后将行的类型进行转换**
- **按照type列进行分组，计算中位数**

![](./img/1653829368625-49d9076a-29c4-48ae-bca5-34e9822c37e1.png)<br />![](./img/1653829368635-05d660ea-0675-48be-9461-0ac0c572b3f9.png)<br />![](./img/1653829368634-d4d70296-cf47-405e-b75e-eadc26c91bb2.png)<br />![](./img/1653829368632-a450dd33-41e1-46a0-97ba-05fd45d95f43.png)
<a name="Suiv2"></a>
## selecting a column
```python
dogs['longevity']
```
![](./img/1653829368548-746f55e2-acfa-4f2b-8b62-2c59cfae723c.png)
<a name="bsP3D"></a>
## groupby + mean
```python
dogs.groupby('size').mean()
```
执行步骤：

- **将数据按照size进行分组**
- **在分组内进行聚合操作**

![](./img/1653829368949-ef4be53a-3714-45cf-9e0c-edea7d16edd4.png)<br />![](./img/1653829369040-6b11cae6-98eb-46ab-88d4-5aa71d7da3ad.png)
<a name="stFrk"></a>
## grouping multiple columns
```python
dogs.groupby(['type', 'size'])
```
![](./img/1653829369103-61060a41-1958-4762-8435-a55ecbe69ead.png)
<a name="O7yxh"></a>
## groupby + multi aggregation
```python
(dogs
  .sort_values('size')
  .groupby('size')['height']
  .agg(['sum', 'mean', 'std'])
)
```
执行步骤

- **按照size列对数据进行排序**
- **按照size进行分组**
- **对分组内的height进行计算**

![](./img/1653829369113-eb972e5c-71ae-425e-806d-2769658d8695.png)<br />![](./img/1653829369318-6f940f85-ffd5-4db4-95c6-b7edc36ff336.png)<br />![](./img/1653829369378-40ef825c-d30d-452c-8fe4-7e7afa141a69.png)<br />![](./img/1653829369444-3f637510-2589-4ca0-9251-bdffdf64fe3f.png)
<a name="nxey3"></a>
## filtering for columns
```python
df.loc[:, df.loc['two'] <= 20]
```
![](./img/1653829369463-33325307-adab-45e5-8b38-ccceaabc80f4.png)
<a name="t1UyC"></a>
## filtering for rows
```python
dogs.loc[(dogs['size'] == 'medium') & (dogs['longevity'] > 12), 'breed']
```
![](./img/1653829369678-ba9ffe56-4308-4e16-9fcf-c201cd730096.png)
<a name="t5NKT"></a>
## dropping columns
```python
dogs.drop(columns=['type'])
```
![](./img/1653829369851-2e5667c9-3aad-42ce-9918-161e0adb420a.png)
<a name="rNzll"></a>
## joining
```python
ppl.join(dogs)
```
![](./img/1653829369841-3a2ee6e2-e708-452a-8328-601d63ab6010.png)
<a name="Nzt0u"></a>
## merging
```python
ppl.merge(dogs, left_on='likes', right_on='breed', how='left')
```
![](./img/1653829370026-49d5be16-8fba-41fe-a778-d589bd46b2af.png)
<a name="dmEVW"></a>
## pivot table
```python
dogs.pivot_table(index='size', columns='kids', values='price')
```
![](./img/1653829370124-9653e234-17dd-44e8-a05d-6982ac88c63e.png)
<a name="wX5HU"></a>
## melting
```python
dogs.melt()
```
![](./img/1653829370192-129d5ce5-b051-4347-a1c9-6c5df0ddd42d.png)
<a name="UwThu"></a>
## pivoting
```python
dogs.pivot(index='size', columns='kids')
```
![](./img/1653829370296-ac6dd25a-4e51-449e-b422-97aba3119056.png)
<a name="i8pvy"></a>
## stacking column index
```python
dogs.stack()
```
![](./img/1653829370378-585490c4-4e64-44ed-9da0-0f52cb95ad0a.png)
<a name="ejVPU"></a>
## unstacking row index
```python
dogs.unstack()
```
![](./img/1653829370434-73a13629-8876-4704-87de-170ee11b75a9.png)
<a name="LuwJR"></a>
## resetting index
```python
dogs.reset_index()
```
![](./img/1653829370526-e36db9b3-e3e0-4e4a-b32a-489ed0d72131.png)
<a name="fI8M5"></a>
## setting index
```python
dogs.set_index('breed')
```
![](./img/1653829370645-54adbba3-cd5a-4ff3-963f-a6000d7e2531.png)
<a name="JT0by"></a>
## 原文地址
[https://pandastutor.com/index.html](https://pandastutor.com/index.html)
