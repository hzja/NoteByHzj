Pandas 
<a name="PIvUJ"></a>
## 构造数据集
这里先构造一个数据集，用于为大家演示这20个函数。
```python
import pandas as pd
df ={'姓名':[' 黄同学','黄至尊','黄老邪 ','陈大美','孙尚香'],
     '英文名':['Huang tong_xue','huang zhi_zun','Huang Lao_xie','Chen Da_mei','sun shang_xiang'],
     '性别':['男','women','men','女','男'],
     '身份证':['463895200003128433','429475199912122345','420934199110102311','431085200005230122','420953199509082345'],
     '身高':['mid:175_good','low:165_bad','low:159_bad','high:180_verygood','low:172_bad'],
     '家庭住址':['湖北广水','河南信阳','广西桂林','湖北孝感','广东广州'],
     '电话号码':['13434813546','19748672895','16728613064','14561586431','19384683910'],
     '收入':['1.1万','8.5千','0.9万','6.5千','2.0万']}
df = pd.DataFrame(df)
df
```
效果图：<br />![2021-09-15-14-12-08-300723.png](./img/1631686344150-a03ac062-da2e-450f-80ca-33d6bfbd259b.png)
<a name="ENLFE"></a>
## 1、`cat`函数
这个函数主要用于字符串的拼接；
```python
df["姓名"].str.cat(df["家庭住址"],sep='-'*3)
```
效果图：<br />![2021-09-15-14-12-08-425703.png](./img/1631686344168-e5931023-d7b3-47f4-ae74-aa4036e5bee4.png)
<a name="paYKe"></a>
## 2、`contains`函数
这个函数主要用于判断某个字符串是否包含给定字符；
```python
df["家庭住址"].str.contains("广")
```
效果图：<br />![](./img/1631686023570-0d8ad2a7-e8ce-4155-a75d-bbcf8cc88b1e.png)
<a name="j0crB"></a>
## 3、`startswith`、`endswith`函数
这个函数主要用于判断某个字符串是否以...开头/结尾；
```python
# 第一个行的“ 黄伟”是以空格开头的
df["姓名"].str.startswith("黄") 
df["英文名"].str.endswith("e")
```
效果图：<br />![](./img/1631686023628-b83fd759-0695-46e5-9a0c-09ab36c62287.webp)
<a name="U0ynO"></a>
## 4、`count`函数
这个函数主要用于计算给定字符在字符串中出现的次数；
```python
df["电话号码"].str.count("3")
```
效果图：<br />![](./img/1631686023574-0b63fd75-222f-4d55-ab4b-73838aa81add.png)
<a name="aMrHh"></a>
## 5、`get`函数
这个函数主要用于获取指定位置的字符串；
```python
df["姓名"].str.get(-1)
df["身高"].str.split(":")
df["身高"].str.split(":").str.get(0)
```
效果图：<br />![](./img/1631686023907-feb0efc5-e9d8-431d-81e3-fed58a5e7b98.webp)
<a name="ri5vB"></a>
## 6、`len`函数
这个函数主要用于计算字符串长度；
```python
df["性别"].str.len()
```
效果图：<br />![](./img/1631686024081-5ae23f3c-951f-4692-a3eb-61be8e3aaa5a.webp)
<a name="rhGiJ"></a>
## 7、`upper`、`lower`函数
这个函数主要用于英文大小写转换；
```python
df["英文名"].str.upper()
df["英文名"].str.lower()
```
效果图：<br />![2021-09-15-14-12-08-937700.png](./img/1631686388819-2e62238d-fffc-4ca7-9ac3-7ef2e7cf6483.png)
<a name="WdFqn"></a>
## 8、`pad`+`side`参数/`center`函数
这个函数主要用于在字符串的左边、右边或左右两边添加给定字符；
```python
df["家庭住址"].str.pad(10,fillchar="*")      # 相当于ljust()
df["家庭住址"].str.pad(10,side="right",fillchar="*")    # 相当于rjust()
df["家庭住址"].str.center(10,fillchar="*")
```
效果图：<br />![](./img/1631686024054-0f81d6ab-ecf5-40e8-a58d-02c5f38662e6.png)
<a name="BtVO5"></a>
## 9、`repeat`函数
这个函数主要用于重复字符串几次；
```python
df["性别"].str.repeat(3)
```
效果图：<br />![2021-09-15-14-12-09-135703.png](./img/1631686402484-6af8d56d-84cd-4b76-8211-4fa12b37d995.png)
<a name="u2Amd"></a>
## 10、`slice_replace`函数
这个函数主要用于使用给定的字符串，替换指定的位置的字符；
```python
df["电话号码"].str.slice_replace(4,8,"*"*4)
```
效果图：<br />![2021-09-15-14-12-09-267705.png](./img/1631686402501-509e0e2b-ccbc-4475-9bdd-50063d96f39f.png)
<a name="OYyrO"></a>
## 11. `replace`函数
这个函数主要用于将指定位置的字符，替换为给定的字符串；
```python
df["身高"].str.replace(":","-")
```
效果图：<br />![](./img/1631686024515-7d21f375-6c57-41b4-858d-6fa9fa039811.webp)<br />这个函数还接受正则表达式，将指定位置的字符，替换为给定的字符串。
```python
df["收入"].str.replace("\d+\.\d+","正则")
```
效果图：<br />![2021-09-15-14-12-09-475346.png](./img/1631686420703-78d3408b-5028-4db4-b0e9-315472a1b15c.png)
<a name="PSrr6"></a>
## 12、`split`方法+`expand`参数
这个函数主要用于将一列扩展为好几列；
```python
# 普通用法
df["身高"].str.split(":")
# split方法，搭配expand参数
df[["身高描述","final身高"]] = df["身高"].str.split(":",expand=True)
df
# split方法搭配join方法
df["身高"].str.split(":").str.join("?"*5)
```
效果图：<br />![](./img/1631686024630-44dee568-3b62-4973-b2f9-b24d1e0d8e4e.webp)
<a name="gdMGn"></a>
## 13、`strip`、`rstrip`、`lstrip`函数
这个函数主要用于去除空白符、换行符；
```python
df["姓名"].str.len()
df["姓名"] = df["姓名"].str.strip()
df["姓名"].str.len()
```
效果图：<br />![](./img/1631686024823-855d05ed-e8af-45a5-85f7-d77111f793c4.webp)
<a name="XpHdj"></a>
## 14、`findall`函数
这个函数主要用于利用正则表达式，去字符串中匹配，返回查找结果的列表；
```python
df["身高"]
df["身高"].str.findall("[a-zA-Z]+")
```
效果图：<br />![](./img/1631686024791-eac21197-30b6-4a2e-861c-ba75cb2c725c.webp)
<a name="mBgex"></a>
## 15、`extract`、`extractall`函数
这个函数主要用于接受正则表达式，抽取匹配的字符串(一定要加上括号)；
```python
df["身高"].str.extract("([a-zA-Z]+)")
# extractall提取得到复合索引
df["身高"].str.extractall("([a-zA-Z]+)")
# extract搭配expand参数
df["身高"].str.extract("([a-zA-Z]+).*?([a-zA-Z]+)",expand=True)
```
效果图：<br />![2021-09-15-14-12-09-837882.png](./img/1631686435093-bdbbf945-f450-4a30-bd8e-043b9b356b80.png)
