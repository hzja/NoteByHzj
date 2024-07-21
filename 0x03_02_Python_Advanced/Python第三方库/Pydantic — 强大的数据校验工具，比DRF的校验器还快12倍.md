Python<br />Pydantic 是一个使用Python类型注解进行数据验证和管理的模块。安装方法非常简单，打开终端输入：
```bash
pip install pydantic
```
它类似于 Django DRF 序列化器的数据校验功能，不同的是，Django里的序列化器的Field是有限制的，如果想要使用自己的Field还需要继承并重写它的基类：
```python
# Django 序列化器的一个使用例子，你可以和下面Pydantic的使用作对比
class Book(models.Model):
    id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=32)
    price = models.DecimalField(max_digits=5, decimal_places=2)
    author = models.CharField(max_length=32)
    publish = models.CharField(max_length=32)
```
而 Pydantic 基于Python3.7以上的类型注解特性，实现了可以对任何类做数据校验的功能：
```python
# Pydantic 数据校验功能
from datetime import datetime
from typing import List, Optional
from pydantic import BaseModel


class User(BaseModel):
    id: int
    name = 'John Doe'
    signup_ts: Optional[datetime] = None
    friends: List[int] = []


external_data = {
    'id': '123',
    'signup_ts': '2019-06-01 12:22',
    'friends': [1, 2, '3'],
}
user = User(**external_data)
print(user.id)
print(type(user.id))
#> 123
#> <class 'int'>
print(repr(user.signup_ts))
#> datetime.datetime(2019, 6, 1, 12, 22)
print(user.friends)
#> [1, 2, 3]
print(user.dict())
"""
{
    'id': 123,
    'signup_ts': datetime.datetime(2019, 6, 1, 12, 22),
    'friends': [1, 2, 3],
    'name': 'John Doe',
}
"""
```
从上面的基本使用可以看到，它甚至能自动做数据类型的转换，比如代码中的 user.id，在字典中是字符串，但经过Pydantic校验器后，它自动变成了int型，因为User类里的注解就是int型。<br />当数据和定义的注解类型不一致时会报这样的Error：
```python
from datetime import datetime
from typing import List, Optional
from pydantic import BaseModel


class User(BaseModel):
    id: int
    name = 'John Doe'
    signup_ts: Optional[datetime] = None
    friends: List[int] = []


external_data = {
    'id': '123',
    'signup_ts': '2019-06-01 12:222',
    'friends': [1, 2, '3'],
}
user = User(**external_data)
"""
Traceback (most recent call last):
  File "1.py", line 18, in <module>
    user = User(**external_data)
  File "pydantic\main.py", line 331, in pydantic.main.BaseModel.__init__
pydantic.error_wrappers.ValidationError: 1 validation error for User
signup_ts
  invalid datetime format (type=value_error.datetime)
"""
```
即 "invalid datetime format"，因为传入的 signup_ts 不是标准的时间格式（多了个2）。
<a name="rIpAd"></a>
## 1、Pydantic模型数据导出
通过Pydantic模型中自带的 json 属性方法，能让经过校验后的数据一行命令直接转成 json 字符串，如前文中的 user 对象：
```python
print(user.dict()) # 转为字典
"""
{
    'id': 123,
    'signup_ts': datetime.datetime(2019, 6, 1, 12, 22),
    'friends': [1, 2, 3],
    'name': 'John Doe',
}
"""
print(user.json()) # 转为json
"""
{"id": 123, "signup_ts": "2019-06-01T12:22:00", "friends": [1, 2, 3], "name": "John Doe"}
"""
```
非常方便。它还支持将整个数据结构导出为 schema json，它能完整地描述整个对象的数据结构类型：
```python
print(user.schema_json(indent=2))
"""
{
  "title": "User",
  "type": "object",
  "properties": {
    "id": {
      "title": "Id",
      "type": "integer"
    },
    "signup_ts": {
      "title": "Signup Ts",
      "type": "string",
      "format": "date-time"
    },
    "friends": {
      "title": "Friends",
      "default": [],
      "type": "array",
      "items": {
        "type": "integer"
      }
    },
    "name": {
      "title": "Name",
      "default": "John Doe",
      "type": "string"
    }
  },
  "required": [
    "id"
  ]
}
"""
```
<a name="RrvQC"></a>
## 2、数据导入
除了直接定义数据校验模型，它还能通过ORM、字符串、文件导入到数据校验模型：<br />比如字符串（raw）：
```python
from datetime import datetime
from pydantic import BaseModel


class User(BaseModel):
    id: int
    name = 'John Doe'
    signup_ts: datetime = None
      
m = User.parse_raw('{"id": 123, "name": "James"}')
print(m)
#> id=123 signup_ts=None name='James'
```
此外，它能直接将ORM的对象输入，转为Pydantic的对象，比如从Sqlalchemy ORM：
```python
from typing import List
from sqlalchemy import Column, Integer, String
from sqlalchemy.dialects.postgresql import ARRAY
from sqlalchemy.ext.declarative import declarative_base
from pydantic import BaseModel, constr

Base = declarative_base()


class CompanyOrm(Base):
    __tablename__ = 'companies'
    id = Column(Integer, primary_key=True, nullable=False)
    public_key = Column(String(20), index=True, nullable=False, unique=True)
    name = Column(String(63), unique=True)
    domains = Column(ARRAY(String(255)))


class CompanyModel(BaseModel):
    id: int
    public_key: constr(max_length=20)
    name: constr(max_length=63)
    domains: List[constr(max_length=255)]

    class Config:
        orm_mode = True


co_orm = CompanyOrm(
    id=123,
    public_key='foobar',
    name='Testing',
    domains=['example.com', 'foobar.com'],
)
print(co_orm)
#> <models_orm_mode.CompanyOrm object at 0x7f0bdac44850>
co_model = CompanyModel.from_orm(co_orm)
print(co_model)
#> id=123 public_key='foobar' name='Testing' domains=['example.com',
#> 'foobar.com']
```
从Json文件导入：
```python
from datetime import datetime
from pathlib import Path
from pydantic import BaseModel


class User(BaseModel):
    id: int
    name = 'John Doe'
    signup_ts: datetime = None
      
path = Path('data.json')
path.write_text('{"id": 123, "name": "James"}')
m = User.parse_file(path)
print(m)
```
从pickle导入：
```python
import pickle
from datetime import datetime
from pydantic import BaseModel

pickle_data = pickle.dumps({
    'id': 123,
    'name': 'James',
    'signup_ts': datetime(2017, 7, 14)
})
m = User.parse_raw(
    pickle_data, content_type='application/pickle', allow_pickle=True
)
print(m)
#> id=123 signup_ts=datetime.datetime(2017, 7, 14, 0, 0) name='James'
```
<a name="MG0RJ"></a>
## 3、自定义数据校验
还能给它增加 validator 装饰器，增加需要的校验逻辑：
```python
from sklearn.metrics import confusion_matrix
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# 1.导入数据集
dataset = pd.read_csv('Social_Network_Ads.csv')
X = dataset.iloc[:, [1, 2, 3]].values
Y = dataset.iloc[:, 4].values

# 性别转化为数字
labelencoder_X = LabelEncoder()
X[:, 0] = labelencoder_X.fit_transform(X[:, 0])

# 2.将数据集分成训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(
    X, Y, test_size=0.25, random_state=0)

# 3.特征缩放
sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)

# 4.训练
classifier = LogisticRegression()
classifier.fit(X_train, y_train)

# 5.预测
y_pred = classifier.predict(X_test)

# 6.评估预测

# 生成混淆矩阵
from sklearn.metrics import confusion_matrix
cm = confusion_matrix(y_test, y_pred)
print(cm)
```
上面，增加了三种自定义校验逻辑：

1. name 必须带有空格
2. password2 必须和 password1 相同
3. username 必须为字母

试试这三个校验是否有效：
```python
user = UserModel(
    name='samuel colvin',
    username='scolvin',
    password1='zxcvbn',
    password2='zxcvbn',
)
print(user)
#> name='Samuel Colvin' username='scolvin' password1='zxcvbn' password2='zxcvbn'

try:
    UserModel(
        name='samuel',
        username='scolvin',
        password1='zxcvbn',
        password2='zxcvbn2',
    )
except ValidationError as e:
    print(e)
    """
    2 validation errors for UserModel
    name
      must contain a space (type=value_error)
    password2
      passwords do not match (type=value_error)
    """
```
可以看到，第一个UserModel里的数据完全没有问题，通过校验。<br />第二个UserModel里的数据，由于name存在空格，password2和password1不一致，无法通过校验。因此定义的自定义校验器完全有效。
<a name="HiXos"></a>
## 4、性能表现
Pydantic 比 Django-rest-framework 的校验器还快了12.3倍：

| Package | 版本 | 相对表现 | 平均耗时 |
| --- | --- | --- | --- |
| pydantic | 1.7.3 | 

 | 93.7μs |
| attrs + cattrs | 20.3 | 1.5x slower | 143.6μs |
| valideer | 0.4.2 | 1.9x slower | 175.9μs |
| marshmallow | 3.10 | 2.4x slower | 227.6μs |
| voluptuous | 0.12 | 2.7x slower | 257.5μs |
| trafaret | 2.1.0 | 3.2x slower | 296.7μs |
| schematics | 2.1.0 | 10.2x slower | 955.5μs |
| django-rest-framework | 3.12 | 12.3x slower | 1148.4μs |
| cerberus | 1.3.2 | 25.9x slower | 2427.6μs |

而且他们的所有基准测试代码都是开源的，可以在下面这个Github链接找到：<br />[https://github.com/samuelcolvin/pydantic/tree/master/benchmarks](https://github.com/samuelcolvin/pydantic/tree/master/benchmarks)
