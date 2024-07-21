Python
<a name="n7bta"></a>
## 字符串的3种创建方式
<a name="bkE23"></a>
#### ① 单引号(`''`)，创建字符串
```python
a = 'I am a student'
print(a)
```
结果如下：<br />![](./img/1606574819281-534115ed-a7ec-4d4a-985f-93767bc0c6d8.webp)
<a name="WJFd8"></a>
#### ② 双引号(`""`)，创建字符串
```python
b = "I am a teacher"
print(b)
```
结果如下：<br />![](./img/1606574819269-cd6a9c30-ae97-4b2c-b699-50bbb4dde1cf.webp)
<a name="hL7Nu"></a>
#### ③ 连续3个单引号或者3个单引号，创建多行字符串
```python
c = '''
I am a student
My name is黄伟
I am a teacher
My name is陈丽
'''
print(c)
```
结果如下：<br />![](./img/1606574819251-7057bdf0-1ff8-441b-aff7-4a14d472d3ae.webp)
<a name="86Gi2"></a>
## 列表的5种创建方式
<a name="B2bWF"></a>
#### ① 用`[]`创建列表
```python
a = [1,2,3]
print(a)
```
结果如下：<br />![](./img/1606574819302-fc36f12c-8eaf-4dfa-a4bf-e3f9963ab950.webp)
<a name="xOZN6"></a>
#### ② 用`list`创建列表
```python
b = list('abc')
print(b)
c = list((1,2,3))
print(c)
d = list({"aa":1,"bb":3}) #对于字典，生成的是key列表。
print(d)
```
结果如下：<br />![](./img/1606574819304-ea6773ea-93a9-419c-9ae3-4d369c5c5403.webp)
<a name="SWnjx"></a>
#### ③ 用`range`创建整数列表
```python
e = list(range(10))
print(e)
```
结果如下：<br />![](./img/1606574819324-78579a9a-ce23-4c60-902c-8e8671df780c.webp)
<a name="ijE5s"></a>
#### ④ 用列表推导式创建列表
```python
f = [i for i in range(5)]
print(f)
```
结果如下：<br />![](./img/1606574819327-20cce372-1c08-44c3-8c7d-b13627fdbaf8.webp)
<a name="4xF13"></a>
#### ⑤ 用`list`和`[]`创建空列表
```python
g = list()
print(g)
h = []
print(h)
```
结果如下：<br />![](./img/1606574819325-4f1cfe02-29d3-4b8a-b1fd-0c21e2327f60.webp)
<a name="yxSGx"></a>
## 字典的5种创建方式
<a name="1t1T2"></a>
#### ① 用`{}`创建字典
```python
a = {'name':'陈丽','age':18,'job':'teacher'}
print(a)
b = {'name':'陈丽','age':18,'job':['teacher','wife']}
print(b)
```
结果如下：<br />![](./img/1606574819348-ce6157d0-f87f-40c8-89bb-10bd70da16e5.webp)
<a name="Z54uo"></a>
#### ② 用`dict`创建字典
```python
c = dict(name='张伟',age=19)
print(c)
d = dict([('name','李丽'),('age',18)])
print(d)
```
结果如下：<br />![](./img/1606574819349-910e848a-43a7-4454-bcea-8d999ead6a13.webp)
<a name="uJeL1"></a>
#### ③ 用`zip`函数创建字典
```python
x = ['name','age','job']
y = ['陈丽','18','teacher']
e = dict(zip(x,y))
print(e)
```
结果如下：<br />![](./img/1606574819365-7c091d35-774f-4b56-b55a-db8b3db9ea50.webp)
<a name="3scxa"></a>
#### ④ 用`{}`,`dict`创建空字典
```python
f = {}
print(f)
g = dict()
print(g)
```
结果如下：<br />![](./img/1606574819378-d8e2d00d-e835-49d3-9cbd-34fe940c00c8.webp)
<a name="v2UDZ"></a>
#### ⑤ 用`fromkeys`创建'值为空'的字典
```python
h =dict.fromkeys(['name','age','job'])
print(h)
```
结果如下：<br />![](./img/1606574819368-b40f770d-922e-479c-9aa3-96b1bd3228f4.webp)
