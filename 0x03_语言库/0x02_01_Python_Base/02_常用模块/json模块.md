# json 模块

+ json.loads(file)  从字符串中读取 JSON 数据（字典类型）

+ json.dumps(file)  将Python对象变成 JSON 对象(字符串)

~~~ python
import json
from pprint import pprint

info_string = """
{
    "name": "echo",
    "age": 24,
    "coding skills": ["python", "matlab", "java", "c", "c++", "ruby", "scala"],
    "ages for school": { 
        "primary school": 6,
        "middle school": 9,
        "high school": 15,
        "university": 18
    },
    "hobby": ["sports", "reading"],
    "married": false
}
"""

# json.loads()从字符串中读取 JSON 数据（字典类型）
info = json.loads(info_string)
pprint(info)
pprint(type(info))
{'age': 24,
 'ages for school': {'high school': 15,
                     'middle school': 9,
                     'primary school': 6,
                     'university': 18},
 'coding skills': ['python', 'matlab', 'java', 'c', 'c++', 'ruby', 'scala'],
 'hobby': ['sports', 'reading'],
 'married': False,
 'name': 'echo'}
<class 'dict'>


# json.dumps()将Python对象变成 JSON 对象(字符串)
info_json = json.dumps(info)
pprint(info_json)
pprint(type(info_json))
('{"name": "echo", "age": 24, "coding skills": ["python", "matlab", "java", '
 '"c", "c++", "ruby", "scala"], "ages for school": {"primary school": 6, '
 '"middle school": 9, "high school": 15, "university": 18}, "hobby": '
 '["sports", "reading"], "married": false}')
<class 'str'>

~~~



