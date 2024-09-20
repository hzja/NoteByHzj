Python<br />在自然语言处理界，「**模式匹配**」可以说是最常用的技术；甚至可以说，将 NLP 技术作为真实生产力的项目都少不了**模式匹配。**
<a name="goxx8"></a>
## 模式匹配
在计算机科学中，往往是检查给定的序列或字符串中是否有符合**某种模式**的片段，比如说：“啊，你的 **AK-47 **打得真准”，如果将 “啊，你的**_**打得真准 ” 作为一种模式，则会将 AK-47 匹配出来<br />实现模式匹配往往都是用正则表达式，但是如果想识别特别复杂的模式，编写正则表达式就会变得非常非常麻烦，<br />而 Pampy 这个项目能解决不少的烦恼。
<a name="TE2hS"></a>
## Pampy项目地址
[https://github.com/santinic/pampy](https://github.com/santinic/pampy)<br />该程序还有一个同名的 Pampy.js 的 JavaScript 版本库。
<a name="XJ8dC"></a>
## 安装Pampy
安装 Pampy 前，要确保 Python 和 pip 已经成功安装在电脑上<br />使用命令安装 Pampy
```bash
$ pip install pampy
```
看到 Successfully installed pampy-0.3.0 则说明安装成功
<a name="Ao3fX"></a>
## 使用Pampy
<a name="YooM0"></a>
### 特性1：匹配单个字符
可以用 `_` 来匹配单个字符：
```python
from pampy import _,match
 
a=['a',1,'b',2,'c',3,'d',4]
 
patter = ['a',1,'b',_,'c',3,'d',4]
 
action=lambda x: f'b is: {x}'
 
print(match(a,patter,action))
```
运行结果是：
```python
b is: 2
```
从上面例子可以看出，实际上只是用 `_` 充当一个占位符，当匹配的时候，找到这个占位符对应的元素即可。
<a name="htQiJ"></a>
### 特性2：HEAD 和 TAIL
HEAD 和 TAIL 能代表某个模式的前面部分或后面部分。<br />比如：将特定模式后的元素都变成元组：
```python
from pampy import match, HEAD, TAIL, _
x = [-1, -2, -3, 0, 1, 2, 3]
print(match(x, [-1, TAIL], lambda t: [-1, tuple(t)]))
# => [-1, (-2, -3, 0, 1, 2, 3)]
```
将特定模式前的元素设为集合，后面的元素设为元组：
```python
from pampy import match, HEAD, TAIL, _
x = [-1, -2, -3, 0, 1, 2, 3]
print(match(x, [HEAD, _, _, 0, TAIL], lambda h, a, b, t: (set([h, a, b]), tuple(t))))
# => ({-3, -1, -2}, (1, 2, 3))
```
用 HEAD 和 TAIL 这两个关键词来匹配开头和结尾，他们可以匹配任意多个字符：
```python
from pampy import _,match,HEAD,TAIL

a=['a',1,'b',2,'c',3,'d',4]
patter = [HEAD,_,'b',2,'c',3,TAIL]
action=lambda h,m,t: ({'head':h,'middle':m,'tail':t})
print(match(a,patter,action))
```
运行上面例子，结果是：
```python
{'head': 'a', 'middle': 1, 'tail': ['d', 4]}
```
可以从结果看到，HEAD 匹配了一个字符，TAIL 匹配了两个字符，输出的时候，如果是多个字符，结果会以数组的方式给出。
<a name="9a122a50"></a>
### 特性3：甚至能匹配字典中的键
可以匹配多层级的字典中的任意一个层级的 key 或者 value：
```python
from pampy import _, match

person = {
    'address': {'province': '湖北', 'city': '武汉', 'district': '东湖高新'},
    'name': '闲欢'
}
patter = {_: {_: '武汉'}}
action = lambda k1, k2: ({'k1': k1, 'k2': k2})
print(match(person, patter, action))
```
运行结果是：
```python
{'k1': 'address', 'k2': 'city'}
```
跟前一个例子类似，这里使用 `_` 这个占位符占位，然后在 action 里面定位占位符，即可输出结果。<br />在不知道哪个键下有某个值的时候，这招非常好用：
```python
from pampy import match, HEAD, TAIL, _
my_dict = {
    'global_setting': [1, 3, 3],
    'user_setting': {
        'face': ['beautiful', 'ugly'],
        'mind': ['smart', 'stupid']
    }
}
result = match(my_dict, { _: {'face': _}}, lambda key, son_value: (key, son_value))
print(result)
# => ('user_setting', ['beautiful', 'ugly'])
```
<a name="5e3b10cc"></a>
### 特性4：搭配正则
不仅如此，它还能搭配正则一起使用：
```python
import re
from pampy import match, HEAD, TAIL, _
def what_is(pet):
    return match(
        pet, re.compile('(\w+)，(\w)\w+鳕鱼$'), lambda mygod, you: you + "像鳕鱼"
    )
print(what_is('我的天，你长得真像鳕鱼'))
# => '你像鳕鱼'
```
