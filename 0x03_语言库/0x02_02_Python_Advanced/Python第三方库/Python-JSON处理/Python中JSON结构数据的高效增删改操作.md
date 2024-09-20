Python
<a name="CQUwS"></a>
## 1、简介
Python中使用jsonpath库对JSON格式数据结构进行常规的节点条件查询，可以满足日常许多的数据处理需求。
<a name="BbMXG"></a>
## 2、基于jsonpath-ng的进阶JSON数据处理方法
jsonpath-ng是一个功能强大的Python库，它整合了jsonpath-rw、jsonpath-rw-ext等第三方JSONPath拓展库的实用功能，可以基于JSONPath语法，实现更多操纵JSON数据的功能，而不只是查询数据而已，使用`pip install jsonpath-ng`进行安装：
<a name="FvAko"></a>
### 2.1 JSON数据的增删改
jsonpath-ng中设计了一些方法，可以实现对现有JSON数据的增删改操作，首先来学习jsonpath-ng中如何定义JSONPath模式，并将其运用到对数据的匹配上，示例：
```python
import json
from jsonpath_ng import parse

# 读入示例json数据
with open('json示例.json', encoding='utf-8') as j:
    demo_json = json.loads(j.read())
    
# 构造指定JSONPath模式对应的解析器
parser = parse('$..paths..steps[*].duration')

# 利用解析器的find方法找到目标数据中所有满足条件的节点
matches = parser.find(demo_json)

# 利用value属性取得对应匹配结果的值
matches[0].value
```
而基于上面产生的一些对象就可以实现对JSON数据的增删改：
<a name="eSqCo"></a>
#### 2.1.1 对JSON数据进行增操作
在jsonpath-ng中对JSON数据添加节点，思想是先构造对**「原先不存在」**的节点进行匹配的解析器对象，利用`find_or_create`方法处理原始JSON数据：
```python
# 构造示例数据
demo_json = {
    'level1': [
        {
            'level2': {}
        },
        {
            'level2': {
                'level3': 12
            }
        }
    ]
}

# 构造规则解释器，所有除去最后一层节点规则外可以匹配到的节点
# 都属于合法匹配结果，会在匹配结果列表中出现
parser = parse('level1[*].level2.level3')

matches = parser.find_or_create(demo_json)

demo_json
```
在`find_or_create`操作之后，demo_json就被修改成下面的结果：<br />接下来的事情就很简单了，只需要在`matches`结果中进行遍历，遇到value属性为`{}`的，就运用`full_path.update_or_create()`方法对原始JSON数据进行更新即可，比如这里填充999：
```python
for match in matches:
    if match.value == {}:
        # 更新原始输入的JSON数据
        match.full_path.update_or_create(demo_json, 999)

demo_json
```
<a name="D4I1T"></a>
#### 2.1.2  对JSON数据进行删操作
当希望对JSON数据中指定JSONPath规则的节点予以删除时，可以使用到`parse`对象的`filter()`方法传入lambda函数，在lambda函数中进行条件判断，返回的即为删除指定节点之后的输入数据。<br />以上一步**「增」**操作后得到的demo_json为例，来对其`level1[*].level2.level3`值为999的予以过滤：
```python
parser = parse('level1[*].level2.level3')

# 过滤 level1[*].level2.level3 规则下值为 999 的节点
parser.filter(lambda x: x == 999, demo_json)
demo_json
```
可以看到结果正是预期的：
<a name="NnAEN"></a>
#### 2.1.3 对JSON数据进行改操作
对JSON数据中的指定节点进行改操作非常的简单，只需要使用`parse`对象的`update`或`update_or_create`方法即可，使用效果的区别如下所示，轻轻松松就可以完成两种策略下的节点更新操作😋：jsonpath-ng中还有一些丰富的功能，这里就不再赘述，可以前往[https://github.com/h2non/jsonpath-ng](https://github.com/h2non/jsonpath-ng)查看。
