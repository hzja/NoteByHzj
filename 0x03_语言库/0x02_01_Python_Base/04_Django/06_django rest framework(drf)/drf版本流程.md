# drf版本

# 流程

1. 当前端来请求时，执行了as_views()方法，如果设置了全局版本或者进入了设置了版本的功能函数，则会先执行APIView类中的dispatch方法，之后再执行initial方法，然后进入了self.determine_version方法
2. 里面会先判断是否有versioning_class，如果没有就返回(None,None)，就代表没有版本，如果有就执行versioning_class(URLPathVersioning)类中的determine_version方法,它会返回版本
3. 里面会判断如果获取到的version为空则返回默认版本，并且还要判断版本是否存在允许出现的版本列表中，返回版本之后，再把版本号和版本类分别赋值给request.version和request.versioning_scheme



## 继承内置类determine_version

1. 定义一个版本类，重写determine_version方法，返回版本号

utils/versioning.py

~~~ python
class ParamVersion(object):
    
    def determine_version(self, request, *args, **kwargs):
        if self.request.version == 'v1':
            return AccountSerializerVersion1
        return AccountSerializer
~~~

~~~ python
urlpatterns = [
    url(r'^api/(?P<version>\w+)/', include('api.urls')),
]
~~~

~~~ python
urlpatterns = [
    url(r'^order/$', views.UsersView.as_view()),
]
~~~

2. 局部使用

~~~ python
from rest_framework.views import APIView
from rest_framework.versioning import QueryParameterVersioning, URLPathVersioning

class UsersView(APIView):
    versioning_class = ParamVersion
    # versioning_class = QueryParameterVersioning
    # versioning_class = URLPathVersioning
    
    def get(self, request, *args, **kwargs):
        # 获取用户veison
        print(version)
        return HttpResponse('用户列表')
~~~

3. 全局使用 

~~~ python
REST_FRAMEWORK = {
    # 全局应用
    'DEFAULT_VERSIONING_CLASS': 'rest_framework.versioning.URLPathVersioning',
    # 如果没有传版本，可以使用默认版本
    "DEFAULT_VERSION":'v1',
    # 允许出现的版本
    "ALLOWED_VERSION":['v1','v2'],
    # 设置url中获取版本的变量，默认是version
    "VERSION_PARAM":'version',

~~~

## 反向获取路由

~~~ python
from rest_framework.versioning import QueryParameterVersioning


class UsersView(APIView):
    versioning_class = QueryParameterVersioning
    
    def get(self, request, *args, **kwargs):
        # 1.获取版本
        print(request.version)
        
        # 2.获取版本处理的对象
        print(request.versioning_scheme)
        # 反向生成URL（drf中的方法）
        u1 = request.versioning_scheme.reverse('api:uuu', request=request)
        print("u1:", u1)
        # 反向生成URL（原生方法）
        u2 = reverse('api:uuu', kwargs={'version': 1})
        print("u2:", u2)
        return HttpResponse('用户列表')
~~~

request.versioning_scheme.reverse方法可以获取路由，是drf中的方法

django.urls.reverse 是django中的反向获取路由方法

1. 用正则匹配

~~~ python
re_path(r'^(?P<version>[v1|v2]+)/users/', views.UsersView.as_view(), name="uuu"),
~~~

~~~ python
u1: http://127.0.0.1:8000/api/v1/users/
u2: /api/1/users/
~~~

