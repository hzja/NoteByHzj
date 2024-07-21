Python Pandas 数据清洗
<a name="TIgn2"></a>
## 1、处理数据中的空值
在处理真实的数据时，往往会有很多缺少的的特征数据，就是所谓的空值，必须要进行处理才能进行下一步分析<br />空值的处理方式有很多种，一般是删除或者填充<br />Excel通过“查找和替换”功能实现空值的统一替换：<br />![image.png](./img/1620361359444-458b8025-243f-4476-919c-365f77f89163.png)<br />通过“定位”删除空值：<br />![image.png](./img/1620361405336-6fa71858-0d5d-471f-b927-7513151fbeba.png)<br />pandas处理空值的方式比较灵活，可以使用`dropna`函数删除空值
```python
import pandas as pd
data=pd.read_csv('成绩表.csv',encoding='gbk')
data.dropna(how='any')
```
![2021-05-07-12-19-09-602965.png](./img/1620361425762-440ead66-05b4-4a45-8d14-1522a337d20c.png)<br />![2021-05-07-12-19-09-692184.png](./img/1620361425782-0bec410b-06b7-43b6-9455-5bd18aaab4f7.png)<br />用`fillna`函数实现空值的填充
<a name="oL6MJ"></a>
### ①使用数字0填充数据表中的空值
```python
data.fillna(value=0)  
```
![2021-05-07-12-19-09-758258.png](./img/1620361452342-4e3a4092-fd43-49fd-829a-760f43f45ba2.png)
<a name="FxPcB"></a>
### ②使用平均值填充数据表中的空值
```python
data['语文'].fillna(data['语文'].mean())
```
![2021-05-07-12-19-09-985650.png](./img/1620361465668-3a3b5904-c712-48fb-a375-692777f487d8.png)
<a name="UtqXl"></a>
## 2、删除空格
excel中清理空格很简单，直接替换即可<br />pandas删除空格也很方便，主要使用`map`函数
```python
data['姓名']=data['姓名'].map(str.strip)
data
```
![2021-05-07-12-19-10-114300.png](./img/1620361482109-0538f16a-190f-48dd-8ccd-837312ada28c.png)
<a name="vIiIE"></a>
## 3、大小写转换
excel中大小写转换函数分别为`upper()`和`lower()`<br />pandas中转换函数也为`upper()`和`lower()`
```python
data['拼音']=data['拼音'].str.upper()
data
```
![2021-05-07-12-19-10-228669.png](./img/1620361512596-78d3a51b-10e8-4e8e-8ca6-1ba56947db53.png)
```python
data['拼音']=data['拼音'].str.lower()
data
```
![2021-05-07-12-19-10-379258.png](./img/1620361532439-1826d810-3a1c-49b3-94ac-02c6a66d6136.png)
<a name="cpVAJ"></a>
## 4、更改数据格式
excel中更改数据格式通过快捷键“ctrl+1”打开“设置单元格格式”：<br />![image.png](./img/1620361561430-148083ae-d767-41ad-a4cf-bbbec12dbd42.png)<br />pandas使用`astype`来修改数据格式，以将“语文”列改成整数为例
```python
data['语文'].dropna(how='any').astype('int')
```
![2021-05-07-12-19-10-913552.png](./img/1620361579510-399c6ba4-1876-4eb9-a4bd-8ccd2f80b2a2.png)
<a name="pquSi"></a>
## 5、更改列名称
excel中更改列名称就不说了，大家都会<br />pandas使用`rename`函数更改列名称，代码如下：
```python
data.rename(columns={'语文':'语文成绩'})
```
![2021-05-07-12-19-11-072127.png](./img/1620361595645-af5162d4-bbd4-45b6-94ab-34d9f4dcdd31.png)
<a name="zlffr"></a>
## 6、删除重复值
excel的功能区“数据”下有“删除重复项”，可以用来删除表中的重复值，默认保留最第一个重复值，把后面的删除：<br />![image.png](./img/1620361633040-e13d5416-f0a2-4397-9c05-475abbeeedcd.png)<br />pandas使用`drop_duplicates`函数删除重复值：
```python
data['数学'].drop_duplicates()  #默认删除后面的重复值

data['数学'].drop_duplicates(keep='last')  #删除第一项重复值
```
<a name="V9oEB"></a>
## 7、修改及替换数据
excel中使用“查找和替换”功能实现数值的替换<br />pandas中使用`replace`函数实现数据替换
```python
data['姓名'].replace('成  功','失  败')
```


