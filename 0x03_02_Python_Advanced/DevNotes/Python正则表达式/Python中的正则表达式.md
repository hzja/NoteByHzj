Python<br />正则表达式使用单个字符串来描述、匹配一系列符合某个句法规则的字符串. Python 自1.5版本起增加了re 模块，它提供 Perl 风格的正则表达式模式。
<a name="v0YY1"></a>
### re 模块
re 模块使 Python 语言拥有全部的正则表达式功能。re 模块也提供了与这些方法功能完全一致的函数，这些函数使用一个模式字符串做为它们的第一个参数。
<a name="pmYRC"></a>
### match 基本使用
```python
"""
    match(匹配的规则字符串, 目标子符串)
"""
from re import match, search

if __name__ == '__main__':
    pattern = "hello"
    str1 = "hell1word hello"
    # result = match(pattern, str1)
    result = search(pattern, str1)
    print(type(result))
    print(result)
    # print(result.span())  # 返回查找到字符串的索引
    # print(result.group())  # 返回查找到字符串的列表
```
<a name="rMtsV"></a>
### 表示字符
| 字符 | 功能 |
| --- | --- |
| . | 匹配任意一个字符 |
| [] | 匹配列表中的出现的任一字符 |
| \\d | 匹配数字，即0-9 |
| \\D | 匹配非数字 |
| \\s | 匹配空白，即空格(\\n\\t) |
| \\S | 匹配非空格 |
| \\w | 匹配单词字符，即a-z,A-Z,0-9,_等 |
| \\W | 匹配非单词字符 |

```python
import re

'''
 . 不能匹配换行 \n, \s 可以匹配换行 \n

'''
v = re.match('.','a')
v = re.match('.','1')
v = re.match('.','_')
#返回None
v = re.match('.','\n')
print(v)
v = re.match('\d','1')
print(v)
v = re.match('\D','a1')
print(v)
v = re.match('\s',' ')
v = re.match('\s','\n')
v = re.match('\s','\t')
print(v)
#非空格
v = re.match('\S',' ')
print(v)
v = re.match('\w','a')
v = re.match('\w','A')
v = re.match('\w','1')
v = re.match('\w','_')
print(v)

v = re.match('\W','a')
v = re.match('\W','A')
v = re.match('\W','1')
v = re.match('\W','_')
print(v)

v = re.match('\w\W','1a')
print(v)
```
<a name="bPbtg"></a>
### 表示数量
| 数量 | 说明 |
| --- | --- |
| * | 匹配前0个或多个（可有可无） |
| + | 匹配前1个或多个 |
| ? | 匹配前1个(可有(有只出现一次)可无) |
| {n} | 匹配前字符出现n次 |
| {n,} | 匹配前字符至少出现n次 |
| {n,m} | 匹配前字符出现m次到n次，限定字符的位数 |

```python
"""
    使用正则表达式表示数量：
        *匹配前0个或多个（可有可无）
        +匹配前1个或多个
        ？匹配前1个（可有(有只出现一次)可无）
        {n}匹配前字符出现 n 次
        {n,}匹配前字符至少出现  n 次
        {n,m}匹配前字符出现 m 次到 n 次，限定字符的位数
"""

import re

if __name__ == '__main__':
    # pattern = "\d*"
    # pattern = "\d+"
    # pattern = "\d?"
    pattern = "\d{2,4}"
    s = "123123ab"
    # s = ""
    s = "21abc"
    # v = re.match(pattern, s)
    # print(v)

    # 匹配手机号
    pattern = "1[348]\d{9}"
    s = "13345678901"
    # print(re.match(pattern, s))

    # 匹配变量名 (大写，小写，数字，下划线，只能以字母和下划线开头)
    pattern = "[a-zA-Z_]\w*"
    var = "H_hello_123"
    # print(re.match(pattern, var))

    # 匹配数字 1-99
    pattern = "[1-9][0-9]?"
    num = "0"
    num = "9"
    num = "99"
    print(re.match(pattern, num))

    # 用户名由字母、数字、下划线组成，位数 5,10位之间
    pattern = "[a-zA-Z_]{5,10}"
    s = "_abcaa1111"
    print(re.match(pattern, s))
```
<a name="JjOGq"></a>
### 原始字符串
Python 中字符串前边加 r 表示原生字符串
```python
"""
    原始字符串
"""
import re

if __name__ == '__main__':
    s = '\n123'
    print(s)
    s = r'\n123'
    print(s)
    s = '\\n123'
    print(s)
    # pattern = '\\\\n\d{3,}'
    pattern = r'\\n\d{3,}'
    v = re.match(pattern, s)
    print(v)
```
<a name="zTjsJ"></a>
### 边界
| 字符 | 功能 |
| --- | --- |
| ^ | 匹配字符串开头 |
| $ | 匹配字符串结尾 |
| \\b | 匹配一个单词的边界 |
| \\B | 匹配非单词的边界 |

```python
"""
    边界
"""

import re

if __name__ == '__main__':
    # $ 表示匹配结尾
    # 匹配 163 邮箱
    pattern = "1[389]\w{9}@163.com$"
    s = "13856123451@163.com"
    print(re.match(pattern, s))

    # \b 匹配单词结尾
    pattern = r"\w*er\b"
    pattern = r"\bHello_\w*"
    pattern = r"\bhttp://www.\w*.com"
    s = "zwzwer"
    s = "Hello_1232ljj"
    s = "https://www.baidu.com"
    print(re.match(pattern, s))
```
<a name="rwEAW"></a>
### 分组
| 字符 | 功能 |
| --- | --- |
| `&#124;` | 匹配左右任意一个表达式 |
| (ab) | 将括号中的字符作为一个分组 |
| \\num | 引用分组num匹配到的字符串 |
| `(?p<name>)` | 分别起组名 |
| `(?p=name)` | 引用别名为name分组匹配到的字符串 |

<a name="bziAo"></a>
#### 匹配 [0,100] 之间的数
```python
pattern = r"[1,9]?\d$|100$"  # 注意： | 前后不能有空格，否则当做匹配的规则
s = "0"
s = "99"
s = "100"
v = re.match(pattern, s)
print(v)
```
<a name="J4WkM"></a>
#### 匹配数字 0001-11233
```python
pattern = r"(\d{4})-([1-9]\d{4})$"
s = "0001-11111"
v = re.match(pattern, s)
print(v)
print(v.group())
print(v.group(1))
print(v.group(2))
print(v.groups())
```
<a name="GZyY1"></a>
#### 匹配 <html><title>我是标题哈</title></html>
```python
# pattern = r"<.+><.+>.+</.+></.+>"
pattern = r"<(.+)><(.+)>.+</\2></\1>"
# s = "<html><title>我是标题哈</head></body>"
s = "<html><title>我是标题哈</title></html>"
v = re.match(pattern, s)
print(v)
print(v.group())
print(v.group(1))
print(v.group(2))
print(v.groups())
```
<a name="HP0gT"></a>
#### 匹配 <h2><font>我是2级标题</font></h2>
```python
pattern = "<(?P<h>.+)><(?P<font>.+)>.+</(?P=font)></(?P=h)>"
s = "<h2><p>我是2级标题</p></h2>"
v = re.match(pattern, s)
print(v)
print(v.group())
print(v.groups())
print(v.group('h'))
print(v.group('font'))
```
<a name="DMhxc"></a>
## 正则表达式高级用法
| 方法 | 作用 |
| --- | --- |
| match | 从目标字符串的开头进行匹配 |
| search | 遍历目标字符串，查找**第一个**符合匹配规则的子串 |
| findall | 遍历目标字符串，查找**所有**符合匹配规则的子串 |
| sub | 遍历目标字符串，查找所有符合匹配规则的子串并 |
| split | 按照匹配规则，分割目标字符串为列表 |

```python
"""
    match：从目标字符串的开头，进行匹配
    search: 遍历目标字符串，查找第一个符合匹配规则的子符串
    findall: 遍历目标字符串，查找所有符合匹配规则的子符串
    sub: 遍历目标字符串，查找所有符合匹配规则的子符串并替换
    split: 按照匹配规则，分割目标字符串为列表
"""

import re

if __name__ == '__main__':
    s = "点赞量为：99次,转发量为:10次，评论数为:9次"

    pattern = r"\d+"

    v = re.search(pattern, s)
    print(v)
    list1 = re.findall(pattern, s)
    print(list1)
    print("======================s")

    """
        字符串替换
    """


    def replace(result):
        """ 替换匹配成功字串内容 """
        # print(result)
        # print(result.group())
        return str(int(result.group()) + 10)


    # count=0 表示替换所有符合条件的子串
    # count=1 表示替换第一次匹配成功后，替换该子串，后面符合条件的字串不替换
    # count=n(n>=1) 表示替换前n次匹配成功后，替换该子串，后面符合条件的字串不替换
    # new_str = re.sub(pattern, '100', s, count=2)
    new_str = re.sub(pattern, lambda x: str(int(x.group()) + 10), s, count=2)
    print(new_str)

    print("======================")

    """
        根据正则表达式拆分字符串
    """
    test = "HelloWorld!Python:大法好;优秀 哈"
    list_split = re.split(r'\s|!|:|;', test)
    print(list_split)
```
<a name="OmPWG"></a>
## 贪婪模式与非贪婪模式

- 什么是贪婪模式？

Python 里数量词默认是贪婪的， 总是尝试匹配尽可能多的字符

- 什么是非贪婪

与贪婪相反，总是尝试匹配尽可能少的字符，可以使用"`*`","`?`","`+`","`{m,n}`"后面加上`?`，使贪婪变成非贪婪
```python
"""
    贪婪模式： 默认是贪婪模式，在满足条件的前提下，尽可能多的匹配子串
    非贪婪模式:  增加 ? ，在满足条件的前提下，尽可能少的匹配子串
"""
from re import match

if __name__ == '__main__':

    # 匹配 My telephone is 011-1232-1231
    s = "My telephone is 011-1232-1231"

    pattern = "(.+?)(\d+)-(\d+)-(\d+?)"
    v = match(pattern, s)
    print(v.group())
    print(v.groups())


    s = "aaabb123"
    pattern = r"(\w+?)([1-9]+)"
    v = match(pattern, s)
    print(v.group())
    print(v.groups())
```
