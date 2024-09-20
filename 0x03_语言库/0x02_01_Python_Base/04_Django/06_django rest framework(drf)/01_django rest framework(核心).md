# django rest framework(核心)

## 为什么用drf?

1. 提供了可视化的API调试界面，开发者可以在线测试接口
2. 可以根据需求来选择常规视图功能或更高级的功能
3. 不用自己写大量的CRUD接口了，简单配置即可
4. 支持ORM（对象映射关系）和非ORM的数据序列化



~~~ python
# 安装drf
pip install  djangorestframework

# 具体功能在具体模块下
from rest_framework.request import Request
from rest_framework.response import Response
from rest_framework.exceptions import APIException
from rest_framework.filters import OrderingFilter
from rest_framework.views import APIView
from rest_framework.pagination import PageNumberPagination
from rest_framework.settings import APISettings

# 注册drfapp
INSTALLED_APPS = [
    ...
    'rest_framework',
]
~~~

as_view: 就干了一件事，禁用csrf认证



## 三大认证任务分析

- **认证模块：**校验用户是是否登陆

~~~ python
self.perform_authentication(request)
~~~



- **权限模块：**校验用户是否拥有权限

~~~ python
self.check_permissionsn(request)
~~~



- **节流模块：**访问接口的次数在设定的时间范围内是否过快(配置访问频率、缓存计次、超次后需要等待的时间)

~~~python
self.check_throttles(request)
~~~