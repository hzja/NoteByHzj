JMESPath是一种用于在JSON数据中进行查询的查询语言，它可以让你以一种简洁的方式从复杂的JSON结构中提取所需的数据。<br />XML结构数据有Xpath，JSON结构数据有JMESPath。
<a name="u3mor"></a>
## JMESPath的入门用法
<a name="ZoNa2"></a>
### 取键"a"取对应值：
```python
data = {"a": "foo", "b": "bar", "c": "baz"}
```
普通方式：
```python
data = {"a": "foo", "b": "bar", "c": "baz"}

result = data.get("a")
print(result) # 结果：foo
```
JMESPath方式：
```python
import jmespath

data = {"a": "foo", "b": "bar", "c": "baz"}

query = "a"
result = jmespath.search(query, data)
print(result) # 结果：foo
```
**再复杂点！**
<a name="N2n5H"></a>
### 取嵌套字典中"d"对应的值：
```python
data = {"a": {"b": {"c": {"d": "value"}}}}
```
普通方式：
```python
data = {"a": {"b": {"c": {"d": "value"}}}}

result = data.get("a").get("b").get("c").get("d")
print(result) # 结果：value
```
一串get相连。<br />JMESPath方式：
```python
import jmespath
data = {"a": {"b": {"c": {"d": "value"}}}}

query = "a.b.c.d"
result = jmespath.search(query, data)
print(result)
```
像JavaScript那样的方式取得第三层的"d"值。<br />再猛一点：
<a name="dIS0x"></a>
### 取嵌套字典中第一个"c"中第一个字典中的"d"中第二个列表元素的第一个值：
目标是取得[0, [1, 2]]中的1。
```python
data = {"a": {
    "b": {
        "c": [
            {"d": [0, [1, 2]]},
            {"d": [3, 4]}
        ]
    }
}}
```
普通方式：
```python
data = {"a": {
    "b": {
        "c": [
            {"d": [0, [1, 2]]},
            {"d": [3, 4]}
        ]
    }
}}

result = data.get("a").get("b").get("c")[0].get("d")[1][0]
print(result) # 结果：1
```
JMESPath方式：
```python
import jmespath

data = {"a": {
    "b": {
        "c": [
            {"d": [0, [1, 2]]},
            {"d": [3, 4]}
        ]
    }
}}

# result = data.get("a").get("b").get("c")[0].get("d")[1][0]
# print(result) # 结果：1

query = "a.b.c[0].d[1][0]"
result = jmespath.search(query, data)
print(result) # 结果：1
```
<a name="z9oBY"></a>
## JMESPath的高阶用法
如果仅仅只能支持到索引的话，还挺一般的，毕竟xpath还支持**条件定位目标元素**。但JMESPath也是支持的！
<a name="V6wWA"></a>
### 获取JSON中所有的用户的名
```python
data = {
    "people": [
        {"first": "James", "last": "d"},
        {"first": "Jacob", "last": "e"},
        {"first": "Jayden", "last": "f"},
        {"missing": "different"}
    ],
    "foo": {"bar": "baz"}
}
```
目标是获取："James"，"Jacob"，"Jayden"。<br />**见证奇迹的时候到了！**
```python
import jmespath

data = {
    "people": [
        {"first": "James", "last": "d"},
        {"first": "Jacob", "last": "e"},
        {"first": "Jayden", "last": "f"},
        {"missing": "different"}
    ],
    "foo": {"bar": "baz"}
}

query = "people[*].first"
result = jmespath.search(query, data)
print(result) # ['James', 'Jacob', 'Jayden']
```
解析下`people[*].first`的含义：

1. `people`：这是查询的起点，表示希望从 data 数据中的 people 键对应的值开始查询。
2. `[*]`：这是通配符操作符，它告诉 JMESPath 查询在 people 数组中的所有元素上执行后续操作。
3. `.first`：这是字段投影操作符，它用于从每个 people 数组中的元素中提取 first 字段的值。

**再来个烧脑的！**
<a name="ifLFC"></a>
### 获取当前状态是runing的机器名
```python
data = {
    "machines": [
        {"name": "a", "state": "running"},
        {"name": "b", "state": "stopped"},
        {"name": "c", "state": "running"}
    ]
}
```
肉眼迅速识别出是"a"和"c"。<br />普通方式：
```python
data = {
    "machines": [
        {"name": "a", "state": "running"},
        {"name": "b", "state": "stopped"},
        {"name": "c", "state": "running"}
    ]
}

result = [machine["name"] for machine in data["machines"] if machine["state"] == "running"]
print(result) # 结果：['a', 'c']
```
这个**三元表达式**+**列表推导式**是不是有点绕？<br />JMESPath方式：
```python
import jmespath

data = {
    "machines": [
        {"name": "a", "state": "running"},
        {"name": "b", "state": "stopped"},
        {"name": "c", "state": "running"}
    ]
}
query = "machines[?state=='running'].name"
result = jmespath.search(query, data)
print(result) # 结果：['a', 'c']
```
`machines[?state == 'running'].name`表达式的含义：

1. `machines`：这是查询的起点，表示希望从 data 数据中的 machines 键对应的值开始查询。
2. `[?state == 'running']`：这是一个过滤器操作符，它用于筛选满足条件的元素。在这里，使用过滤器来选择 state 字段值等于 "running" 的元素。
