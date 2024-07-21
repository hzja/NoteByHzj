<a name="ngsYD"></a>
# re库函数
<a name="lGOXQ"></a>
## match
re.match 尝试从字符串的起始位置匹配一个模式，如果不是起始位置匹配成功的话，match() 就返回 none。

```python
#!/usr/bin/python
# -*- coding: UTF-8 -*- 
 
import re

# re.match(pattern, string, flags=0)

print(re.match('www', 'www.runoob.com').span())  # 在起始位置匹配
print(re.match('com', 'www.runoob.com'))         # 不在起始位置匹配

#结果为
(0, 3)
None
```
<a name="IFf0Y"></a>
## search
<a name="i2tcy"></a>
## sub
<a name="Wae5L"></a>
## compile
<a name="pHxrR"></a>
# 正则表达式

<a name="fSI5Q"></a>
# Source
[Python 正则表达式 | 菜鸟教程](https://www.runoob.com/python/python-reg-expressions.html)
