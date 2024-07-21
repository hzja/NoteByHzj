Python Pandas
<a name="HrQoE"></a>
## 构造数据集
这里先构造一个数据集，用于演示这20个函数。
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
效果图：<br />![2021-08-26-15-16-01-401295.png](./img/1629962331251-1c9515ee-9ad2-42ae-830c-8abeaf37d100.png)
<a name="KMUwG"></a>
## 1、`cat`函数
这个函数主要用于字符串的拼接；
```python
df["姓名"].str.cat(df["家庭住址"],sep='-'*3)
```
效果图：<br />![2021-08-26-15-16-01-471289.png](./img/1629962331287-61e081ec-1404-4dcb-8b60-09716df011b7.png)
<a name="JvtML"></a>
## 2、`contains`函数
这个函数主要用于判断某个字符串是否包含给定字符；
```python
df["家庭住址"].str.contains("广")
```
效果图：<br />![2021-08-26-15-16-01-539289.png](./img/1629962331275-531e52ff-516e-40ae-9fc5-61eb28a5ea4a.png)
<a name="qaPF0"></a>
## 3、`startswith`、`endswith`函数
这个函数主要用于判断某个字符串是否以...开头/结尾；
```python
# 第一个行的“ 黄伟”是以空格开头的
df["姓名"].str.startswith("黄") 
df["英文名"].str.endswith("e")
```
效果图：<br />![2021-08-26-15-16-01-612297.png](./img/1629962360931-2768d03d-f6ac-4cc7-9a03-ea27b017234d.png)
<a name="PURsp"></a>
## 4、`count`函数
这个函数主要用于计算给定字符在字符串中出现的次数；
```python
df["电话号码"].str.count("3")
```
效果图：<br />![2021-08-26-15-16-01-720291.png](./img/1629962360924-839362bc-90b1-48e4-9382-ce906579f335.png)
<a name="nB4XT"></a>
## 5、`get`函数
这个函数主要用于获取指定位置的字符串；
```python
df["姓名"].str.get(-1)
df["身高"].str.split(":")
df["身高"].str.split(":").str.get(0)
```
效果图：<br />![2021-08-26-15-16-01-784289.png](./img/1629962378491-a3b38c97-0ccd-482b-b505-d268fdb04150.png)
<a name="DN7pB"></a>
## 6、`len`函数
这个函数主要用于计算字符串长度；
```python
df["性别"].str.len()
```
效果图：<br />![2021-08-26-15-16-01-853291.png](./img/1629962378489-8d2b8ef4-6fa9-4fc5-8250-f3e8b61a6de1.png)
<a name="wvBeL"></a>
## 7、`upper`、`lower`函数
这个函数主要用于英文大小写转换；
```python
df["英文名"].str.upper()
df["英文名"].str.lower()
```
效果图：<br />![2021-08-26-15-16-01-960291.png](./img/1629962427854-cc150c84-0068-4f05-9705-65c722806421.png)
<a name="JCzMz"></a>
## 8、`pad`+`side`参数/`center`函数
这个函数主要用于在字符串的左边、右边或左右两边添加给定字符；
```python
df["家庭住址"].str.pad(10,fillchar="*")      # 相当于ljust()
df["家庭住址"].str.pad(10,side="right",fillchar="*")    # 相当于rjust()
df["家庭住址"].str.center(10,fillchar="*")
```
效果图：<br />![2021-08-26-15-16-02-044289.png](./img/1629962427948-05454d8a-f698-4137-a384-3d00f36c3664.png)
<a name="kZMNA"></a>
## 9、`repeat`函数
这个函数主要用于重复字符串几次；
```python
df["性别"].str.repeat(3)
```
效果图：<br />![2021-08-26-15-16-02-114290.png](./img/1629962427821-7f92e3f1-7e74-4777-a22b-0a10ad0d198f.png)
<a name="Hj55b"></a>
## 10、`slice_replace`函数
这个函数主要用于使用给定的字符串，替换指定的位置的字符；
```python
df["电话号码"].str.slice_replace(4,8,"*"*4)
```
效果图：<br />![2021-08-26-15-16-02-183291.png](./img/1629962457636-8ca641b4-bc1d-4de3-91e0-d793649ae2a5.png)
<a name="aUxVb"></a>
## 11、`replace`函数
这个函数主要用于将指定位置的字符，替换为给定的字符串；
```python
df["身高"].str.replace(":","-")
```
效果图：<br />![2021-08-26-15-16-02-259291.png](./img/1629962457644-58b0c6b3-9898-4cb1-943d-b1a0391028d5.png)<br />这个函数还接受正则表达式，将指定位置的字符，替换为给定的字符串。
```python
df["收入"].str.replace("\d+\.\d+","正则")
```
效果图：<br />![2021-08-26-15-16-02-320288.png](./img/1629962457641-a92d6f16-514f-41bd-9444-108f331b175d.png)
<a name="k2IGr"></a>
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
效果图：<br />![2021-08-26-15-16-02-466303.png](./img/1629962481480-7d77d32e-22e6-41a8-92aa-b2efb88dc7a6.png)
<a name="k1rcB"></a>
## 13、`strip`、`rstrip`、`lstrip`函数
这个函数主要用于去除空白符、换行符；
```python
df["姓名"].str.len()
df["姓名"] = df["姓名"].str.strip()
df["姓名"].str.len()
```
效果图：<br />![2021-08-26-15-16-02-567289.png](./img/1629962481501-aee7ee54-c784-4d1f-a85c-5aa7a6ed3974.png)
<a name="Kxtbh"></a>
## 14、`findall`函数
这个函数主要用于利用正则表达式，去字符串中匹配，返回查找结果的列表；
```python
df["身高"]
df["身高"].str.findall("[a-zA-Z]+")
```
效果图：<br />![2021-08-26-15-16-02-668004.png](./img/1629962481493-a4bb282f-2d5e-46f9-97d7-240d31b419fe.png)
<a name="qVytv"></a>
## 15、`extract`、`extractall`函数
这个函数主要用于接受正则表达式，抽取匹配的字符串(一定要加上括号)；
```python
df["身高"].str.extract("([a-zA-Z]+)")
# extractall提取得到复合索引
df["身高"].str.extractall("([a-zA-Z]+)")
# extract搭配expand参数
df["身高"].str.extract("([a-zA-Z]+).*?([a-zA-Z]+)",expand=True)
```
效果图：<br />![2021-08-26-15-16-02-772143.png](./img/1629962481507-a17482fb-33a8-4326-9fde-ba637f7344c1.png)
