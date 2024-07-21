

![1582961576728-9c0295fb-9132-4b12-8337-8117684bbccd](图片\1582961576728-9c0295fb-9132-4b12-8337-8117684bbccd.png)

## 步骤

1. 数据库查询
2. 构建序列化器, 进行序列化操作, 返回数据



## drf视图类及其区别

### 第一种：APIView

`APIView`是REST framework提供的所有视图的基类，继承自Django的`View`父类。

`APIView`与`View`的不同之处在于：

- 传入到视图方法中的是REST framework的`Request`对象，而不是Django的`HttpRequeset`对象；
- 视图方法可以返回REST framework的`Response`对象，视图会为响应数据设置（render）符合前端要求的格式；
- 任何`APIException`异常都会被捕获到，并且处理成合适的响应信息；
- 在进行dispatch()分发前，会对请求进行身份认证、权限检查、流量控制。

遵循了CBV的模式，里面的功能比较多但是需要自己写的代码也有很多

提供了免除csrf认证，版本处理、认证、权限、节流、解析器、筛选器、分页、序列化、渲染器

#### 使用场景

直接继承API，适用于不需要数据库操作，如果操作数据库需要自己代码实现

~~~ python
class TestView(APIView):
    def get(self, request, *args, **kwargs):
        pass
~~~

 

### 第二种：ListAPIView、RetrieveAPIView、CreateAPIView、UpdateAPIView、DestroyAPIView

第二种则在第一种的基础上，封装了许多我们需要自己的写的代码，许多功能的实现只需要给专属的变量名赋值就可以实现该功能

1. CreateAPIView仅用于创建实例(等价于GenericAPIView+CreateModelMixin)提供 post 方法
2. ListAPIView仅用于读取模型实例列表(等价于GenericAPIView+ListModelMixin)提供 get 方法
3. RetrieveAPIView仅用于查询单个模型实例(等价于GenericAPIView+RetrieveModelMixin)提供 get 方法
4. DestoryAPIView仅用于删除单个模型实例(等价于GenericAPIView+DestoryModelMixin)提供 delete 方法
5. UpdateAPIView仅用于更新单个模型实例(等价于GenericAPIView+UpdateModelMixin)提供 put 和 patch 方法

![1586230181223-24c39ec6-a28f-4879-b9e8-c834ffce9c9e](图片\1586230181223-24c39ec6-a28f-4879-b9e8-c834ffce9c9e.webp)



**基于ListAPIView等实现对module表：单条数据展示、增加**

~~~ python
re_path(r'^api/(?P<version>\w+)/module/new/$', views.ModuleNewView.as_view()),
re_path(r'^api/(?P<version>\w+)/module/new/(?P<pk>\d+)/$', views.ModuleNewView.as_view()),
~~~

~~~ python
from rest_framework.generics import RetrieveAPIView, CreateAPIView

class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = UserInfo
        fields = "__all__"
                
class UserView(RetrieveAPIView, CreateAPIView):
    queryset = UserInfo.objects.all()
    serializer_class = UserSerializer
~~~



#### 使用场景

继承ListAPIView，DestroyAPIView，UpdateAPIView，RetrieveAPIView，CreateAPIView，项目中只要实现某几个接口时，而不是增删改查。



#### GenericAPIView

主要增加了操作序列化器和数据库查询的方法，为Mixin扩展类的执行提供方法支持，通常在使用时可搭配一个或多个Mixin扩展类

~~~ python
from rest_framework.generics import GenericAPIView
from api.utils.serializesers.pager import PagerSerialiser

class View1View(GenericAPIView):
    queryset = models.Role.objects.all()  
    serializer_class = PagerSerialiser
    pagination_class = PageNumberPagination

    def get(self, request, *args, **kwargs):
        # 返回查询到的数据集
        roles = self.get_queryset()
        # [1, 100]  [1, 10]
        pager_roles = self.paginate_queryset(roles)
        # 创建序列化器对象
        ser = self.get_serializer(instance=pager_roles, many=True)
        return Response(ser.data)
~~~



##### GenericAPIView的作用？

指定了接口执行的流程。

我们在使用drf内置类做增删改查时，就可以通过自定义静态字段（类变量）或重写方法（get_queryset、get_serializer_class）来进行更高级的定制，**更多高级用法参考我写drf重写**。



### 第三种：GenericViewSet

第三种继承自GenericAPIView，并且和它区别不大，重构了APIView中的as_view()方法，结合请求方法和不同Mixin类的方法名从而进行执行不同的功能。与前面两种最主要的区别是url路由中as_view()方法中需要传值。



#### 使用场景

您可能不需要使用完整的 ModelViewSet 操作集，或其他自定义行为，请从 GenericViewSet 继承，并混合（mixin ）所需的操作

主要目的是解决第二种的bug（查询全部数据的功能和查询单个数据的功能无法在一个类中实现），将其实现在一个类中！

~~~ python
re_path('^(?P<version>[v1|v2]+)/v1/', views.View1View.as_view({'get':'list'})),
~~~

~~~ python
from rest_framework.response import Response
from rest_framework.mixins import ListModelMixin, RetrieveModelMixin
from rest_framework import serializers


class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = UserInfo
        fields = "__all__"

class UserViewSet(viewsets.GenericViewSet, ListModelMixin, RetrieveModelMixin):
    """
    允许用户查看或编辑的API路径。
    """
    queryset = UserInfo.objects.all()
    serializer_class = UserSerializer
~~~



#### 效果

![image3](图片\image3.png)

![image4](图片\image4.png)



#### ModelViewSet(路由系统有所变化)最牛逼的

懒人用法：功能中需要实现对表的增删改查时

继承自GenericViewSet，同时包括了ListModelMixin、RetrieveModelMixin、CreateModelMixin、UpdateModelMixin、DestoryModelMixin，不需要手动继承了

![1582958526137-645839ff-89f4-46c6-85fa-7e1fd7b9d459](图片\1582958526137-645839ff-89f4-46c6-85fa-7e1fd7b9d459.png)



**ModelViewset实现对Module表进行增伤改查**

~~~ python
router = routers.DefaultRouter()
router.register(r'users', views.UserViewSet)
router.register(r'groups', views.GroupViewSet)
~~~

~~~ python
from rest_framework import viewsets
from api.utils.serializesers.pager import PagerSerialiser
from rest_framework.decorators import action
from rest_framework.response import Response

class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = UserInfo
        fields = "__all__"


class UserViewSet(viewsets.ModelViewSet):
    """
    允许用户查看或编辑的API路径。
    """
    queryset = UserInfo.objects.all()
    serializer_class = UserSerializer

    @action(methods=['get'], detail=False)
    def latest(self, request):
        # 返回最新创建的用户
        book = UserInfo.objects.latest('id')
        serializer = self.get_serializer(book)
        return Response(serializer.data)
~~~



#### 效果

![image5](图片\image5.png)