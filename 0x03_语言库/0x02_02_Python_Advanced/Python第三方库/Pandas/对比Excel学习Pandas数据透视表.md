Pandas
<a name="Bq3lo"></a>
## Excel中做数据透视表
<a name="btOrO"></a>
### ① 选中整个数据源；
![2021-05-20-20-30-13-851109.png](./img/1621514072900-11d870b1-3174-4058-ba62-e7e5374229f4.png)
<a name="afvFZ"></a>
### ② 依次点击“插入”—“数据透视表”
![2021-05-20-20-30-13-972844.png](./img/1621514088452-21fcfc22-f421-4eaa-a044-99d47876a115.png)
<a name="YiYeF"></a>
### ③ 选择在Excel中的哪个位置，插入数据透视表
![2021-05-20-20-30-14-118066.png](./img/1621514097727-5bc359f3-47d4-4618-9069-40aece8ae3d0.png)
<a name="Ahgfm"></a>
### ④ 然后根据实际需求，从不同维度展示结果
![2021-05-20-20-30-14-274279.png](./img/1621514107045-1802df42-7706-4774-b085-e340dcd85d38.png)
<a name="Iwu0u"></a>
### ⑤ 结果如下
![2021-05-20-20-30-14-446113.png](./img/1621514120533-e532bd94-4781-4c35-8d1b-29b7ba09697e.png)
<a name="saV9g"></a>
## Pandas用`pivot_table()`做数据透视表
<a name="Lz5yT"></a>
### 1）语法格式
```python
pd.pivot_table(data,index=None,columns=None,
               values=None,aggfunc='mean',
               margins=False,margins_name='All',
               dropna=True,fill_value=None)
```
<a name="eYCW6"></a>
### 2）对比excel，说明上述参数的具体含义
![2021-05-20-20-30-14-571084.png](./img/1621514133175-53adb2e2-b62c-4cc5-9c64-c725ca111b70.png)<br />参数说明：

- `data` 相当于Excel中的"选中数据源"；
- `index` 相当于上述"数据透视表字段"中的行；
- `columns` 相当于上述"数据透视表字段"中的列；
- `values` 相当于上述"数据透视表字段"中的值；
- `aggfunc` 相当于上述"结果"中的计算类型；
- `margins` 相当于上述"结果"中的总计；
- `margins_name` 相当于修改"总计"名，为其它名称；

下面几个参数，用的较少，记住干嘛的，等以后需要就百度。

- `dropna` 表示是否删除缺失值，如果为True时，则把一整行全作为缺失值删除；
- `fill_value` 表示将缺失值，用某个指定值填充。
<a name="Z3y75"></a>
## 案例说明
<a name="KsoHo"></a>
### 1）求出不同品牌下，每个月份的销售数量之和
<a name="l9vqa"></a>
#### ① 在Excel中的操作结果如下
![2021-05-20-20-30-14-711679.png](./img/1621514144868-687f6c82-26a6-417f-923f-b6af068f4f06.png)
<a name="WJZaL"></a>
#### ② 在pandas中的操作如下
```python
df = pd.read_excel(r".\pivot_table.xlsx")
display(df.sample(5))

df.insert(1,"月份",df["销售日期"].apply(lambda x:x.month))
display(df.sample(5))

df1 = pd.pivot_table(df,index="品牌",columns="月份",
                     values="销售数量",aggfunc=np.sum)
display(df1)
```
结果如下：<br />![image.png](./img/1621514373237-be2a6b46-eee0-4294-b567-729e5f9ec725.png)
<a name="U2J0d"></a>
### 2）求出不同品牌下，每个地区、每个月份的销售数量之和
<a name="WWO1i"></a>
#### ① 在Excel中的操作结果如下
![2021-05-20-20-30-15-061131.png](./img/1621514303830-1b1d720f-1ca6-41dc-9668-a53fecd2ad68.png)
<a name="Z61Hu"></a>
#### ② 在pandas中的操作如下
```python
df = pd.read_excel(r".\pivot_table.xlsx")
display(df.sample(5))

df.insert(1,"月份",df["销售日期"].apply(lambda x:x.month))
display(df.sample(5))

df1 = pd.pivot_table(df,index="品牌",columns=["销售区域","月份"],
                     values="销售数量",aggfunc=np.sum)
display(df1)
```
结果如下：<br />![image.png](./img/1621514408284-6957cff5-9dd4-41b4-a3bc-2ffdc94629e8.png)
<a name="aVApn"></a>
### 3）求出不同品牌不同地区下，每个月份的销售数量之和
<a name="PodpD"></a>
#### ① 在Excel中的操作结果如下
![2021-05-20-20-30-15-370337.png](./img/1621514444603-df201803-61e3-464f-8436-813b553aca68.png)
<a name="jYDt3"></a>
#### ② 在pandas中的操作如下
```python
df = pd.read_excel(r"pivot_table.xlsx")
display(df.sample(5))

df.insert(1,"月份",df["销售日期"].apply(lambda x:x.month))
display(df.sample(5))

df1 = pd.pivot_table(df,index=["品牌","销售区域"],columns="月份",
                     values="销售数量",aggfunc=np.sum)
display(df1)
```
结果如下：<br />![image.png](./img/1621514474166-13dc16ee-aff9-46e8-afc0-1d21ebe53f0b.png)
<a name="TkfaX"></a>
### 4）求出不同品牌下的“销售数量之和”与“货号计数”
<a name="bKTLo"></a>
#### ① 在Excel中的操作结果如下
![2021-05-20-20-30-15-760873.png](./img/1621514488478-43ecffe4-1ae0-4c30-8d59-6e1a6f154940.png)
<a name="B79ym"></a>
#### ② 在pandas中的操作如下
```python
df = pd.read_excel(r"pivot_table.xlsx")
display(df.sample(5))

df.insert(1,"月份",df["销售日期"].apply(lambda x:x.month))
display(df.sample(5))

df1 = pd.pivot_table(df,index="品牌",columns="月份",
                     values=["销售数量","货号"],
                     aggfunc={"销售数量":"sum","货号":"count"},
                     margins=True,margins_name="总计")
display(df1)
```
结果如下：<br />![image.png](./img/1621514519163-ce2084b1-1b2f-4d71-9b1f-293b4e2a9092.png)
