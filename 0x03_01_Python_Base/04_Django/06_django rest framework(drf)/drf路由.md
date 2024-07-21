# drf 路由

依赖于视图集，只有使用了视图集，才可以使用自动生成路由

路由系统可以自动完成增删改查，如果只是增删就不太需要引用



# 自定义路由（最low）

urls.py

~~~ python
from django.urls import path, include
from apps.user import views

urlpatterns = [
    path(r'test/', views.TestView.as_view()),
    path(r'test\.(?P<format>[a-z0-9]+)', views.TestView.as_view()),
    path(r'test/(?P<pk>[^/.]+)/', views.TestView.as_view()),
    path(r'test/(?P<pk>[^/.]+)\.(?P<format>[a-z0-9]+)', views.TestView.as_view())
]
~~~



views.py

~~~ python
from rest_framework.views import APIView
from rest_framework.response import Response


class TestView(APIView):
    def get(self, request, *args, **kwargs):
        print(kwargs)
        print(self.renderer_classes)
        return Response('...')
~~~



# 半自动路由（次low）

urls.py

~~~ python
from django.urls import path, include
from apps.user import views

urlpatterns = [
    path(r'test/', s10_generic.UserViewSet.as_view({'get': 'list', 'post': 'create'})),
    path(r'test/(?P<pk>\d+)/', s10_generic.UserViewSet.as_view(
        {'get': 'retrieve', 'put': 'update', 'patch': 'partial_update', 'delete': 'destroy'})),
]
~~~



views.py

~~~ python
from rest_framework.viewsets import ModelViewSet
from rest_framework import serializers
from .. import models


class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = models.UserInfo
        fields = "__all__"


class UserViewSet(ModelViewSet):
    queryset = models.UserInfo.objects.all()
    serializer_class = UserSerializer
~~~



# 全自动路由

对于视图集ViewSet，我们除了可以自己手动指明请求方式与动作action之间的对应关系外，还可以使用Routers来帮助我们快速实现路由信息。

urls.py

~~~ python
from django.urls import path, include
from rest_framework import routers
from apps.user import views

router = routers.DefaultRouter()
router.register(r'users', views.UserViewSet)
router.register(r'groups', views.GroupViewSet)


# 在路由中的使用,可以有两种方式
urlpatterns = [
    ...
    path(r'', include(router.urls)),
]

urlpatterns = [
    ...
]
urlpatterns += router.urls
~~~

register(prefix, viewset, base_name)

- prefix 该视图集的路由前缀
- viewset 视图集
- base_name 用来生成urls名字，您不需要指定 base_name 参数，但是如果您有一个视图集定义了自定义 get_queryset 方法，那么该视图集可能没有设置 .queryset 属性



如上述代码会形成的路由如下：

~~~ python
^groups/$    name: groups-list
^groups/{pk}/$   name: groups-detail
    
^users/$    name: users-list
^users/{pk}/$   name: users-detail
~~~



![image8](图片\image8.png)



在视图集中，如果想要让Router自动帮助我们为自定义的动作生成路由信息，需要使用`rest_framework.decorators.action`装饰器。

以action装饰器装饰的方法名会作为action动作名，与list、retrieve等同。

action装饰器可以接收两个参数：

- **methods**: 声明该action对应的请求方式，列表传递
- **detail**: 声明该action的路径是否与单一资源对应，及是否是`xxx/<pk>/action方法名/`

- - True 表示路径格式是`xxx/<pk>/action方法名/`
  - False 表示路径格式是`xxx/action方法名/`



views.py

~~~ python
from rest_framework import mixins
from rest_framework.viewsets import GenericViewSet
from rest_framework.decorators import action
from rest_framework.response import Response


class BookInfoViewSet(mixins.ListModelMixin, mixins.RetrieveModelMixin, GenericViewSet):
    queryset = BookInfo.objects.all()
    serializer_class = BookInfoSerializer
 
    @action(methods=['get'], detail=False)
    def latest(self, request):
        # 返回最新的书籍
        book = BookInfo.objects.latest('id')
        serializer = self.get_serializer(book)
        return Response(serializer.data)
 
    @action(methods=['put'], detail=True)
    def read(self, request, pk):     
        # 修改书记的阅读量
        book = self.get_object()
        serializer = self.get_serializer(instance=book, data=request.data)
        serializer.is_valid(raise_exception=True)
        serializer.save()
        return Response(serializer.data)
~~~

此视图集会形成的路由：

~~~ python
^books/latest/$    name: book-latest
^books/{pk}/read/$  name: book-read
    
    
^users/{pk}/change-password/$   name: user-change-password
~~~



## 路由router形成URL的方式

### SimpleRouter

### ![20180628143116131](图片\20180628143116131.png)

### DefaultRouter

![20180628143128366](D:\Note\python\Django\django rest framework(drf)\图片\20180628143128366.png)

DefaultRouter与SimpleRouter的区别是，DefaultRouter会多附带一个默认的API根视图，返回一个包含所有列表视图的超链接响应数据。



# 标记额外的路由行为

viewset只定义了list, update, retrieve, create, update, destory, partial_update方法,  

如果你想自定义一些额外的操作，可以使用@detail_route 或者 @list_route来实现

`@detail_route` 装饰器在其 URL 模式中包含 `pk`，用于支持需要获取单个实例的方法。`@list_route` 修饰器适用于在对象列表上操作的方法。

~~~ python
from django.contrib.auth.models import User
from rest_framework import status
from rest_framework import viewsets
from rest_framework.decorators import detail_route, list_route
from rest_framework.response import Response
from myapp.serializers import UserSerializer, PasswordSerializer
class UserViewSet(viewsets.ModelViewSet):
    """
    提供标准操作的视图集
    """
    queryset = User.objects.all()
    serializer_class = UserSerializer
    
    # 如果你不想使用默认的url，你可以通过设置url_path参数来改变url
    # 如果您不想使用生成的默认名称，则可以使用 url_name 参数对其进行自定义
    @detail_route(methods=['post'], url_path='change-password')
    def set_password(self, request, pk=None):
        user = self.get_object()
        serializer = PasswordSerializer(data=request.data)
        if serializer.is_valid():
            user.set_password(serializer.data['password'])
            user.save()
            return Response({'status': 'password set'})
        else:
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
        
    @list_route()
    def recent_users(self, request):
        recent_users = User.objects.all().order('-last_login')
        page = self.paginate_queryset(recent_users)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)
        serializer = self.get_serializer(recent_users, many=True)
        return Response(serializer.data)
~~~



参考链接 https://blog.csdn.net/weixin_34082177/article/details/87985065?utm_medium=distribute.pc_relevant.none-task-blog-searchFromBaidu-8.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-searchFromBaidu-8.channel_param



http://drf.jiuyou.info/#/drf/routers