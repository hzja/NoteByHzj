# drf 分页

我们数据库有几千万条数据，这些数据需要展示，我们不可能直接从数据库把数据全部读取出来，这样会给内存造成特别大的压力，有可能还会内存溢出，所以希望一点一点的取，然后通过分页展示，但是在数据量特别大的时候，我们的分页会越往后读取速度越慢，怎么能让我的查询速度变快？drf给我们提供了三种分页方式

 步骤 

1 数据库查询

2调用分页器

3构建序列化器, 进行序列化操作, 返回数据



### 首先提取数据库字段

utils/serializers/pager.py

~~~ python
from rest_framework import serializers
from api import models

class PagerSerialiser(serializers.ModelSerializer):
    class Meta:
        model = models.Role
        fields = "__all__"
~~~



### 普通分页（最常用）PageNumberPagination

看第N页，每页显示N条数据

~~~ python
from api.utils.serializesers.pager import PagerSerialiser
from rest_framework.response import Response
from rest_framework.pagination import PageNumberPagination

# 重写原生方法
class MyPageNumberPagination(PageNumberPagination):

    # 默认一页显示3个
    page_size = 3
    # 获取URL参数中传入的页码key字段
    page_query_param = 'page'
    # 指定单页最大值的字段
    page_size_query_param = 'size'
    # 设置单次取的最大值
    max_page_size = None


class Pager1View(APIView):
    def get(self, request, *args, **kwargs):

        # 查库返回Queryset对象
        roles = models.Role.objects.all()
        # 创建分页对象
        pg = MyPageNumberPagination()
        # 在数据库中获取分页的数据
        pager_roles = pg.paginate_queryset(queryset=roles, request=request,view=self)
        # 对数据进行序序列化
        ser = PagerSerialiser(instance=pager_roles, many=True)
        
        # 方法一：使用django方法
        return Response(ser.data)
        
        # 方法二：使用drf分页方法，可以显示上一页和下一页链接
        # return pg.get_paginated_response(ser.data)
~~~

![1582443307712-38e5b54a-0e77-4ecd-95de-22fffe811e04](图片\1582443307712-38e5b54a-0e77-4ecd-95de-22fffe811e04.png)



### 偏移分页 LimitOffsetPagination

在第N个位置，向后查看N条数据

~~~ python
from api.utils.serializesers.pager import PagerSerialiser
from rest_framework.response import Response
from rest_framework.pagination import LimitOffsetPagination


class MyPageNumberPagination(LimitOffsetPagination):

    # 默认单页显示2个
    default_limit = 2
    # 指定单页显示个数的字段
    limit_query_param = 'limit' 
    # 指定从id为多少开始
    offset_query_param = 'offset'
    # 指定最大id
    max_limit = None


class Pager1View(APIView):
    def get(self, request, *args, **kwargs):

        # 查库返回Queryset对象
        roles = models.Role.objects.all()
        # 创建分页对象
        pg = MyPageNumberPagination()
        # 在数据库中获取分页的数据
        pager_roles = pg.paginate_queryset(queryset=roles, request=request,view=self)
        # 对数据进行序序列化
        ser = PagerSerialiser(instance=pager_roles, many=True)
        # 方法一：使用django方法
        return Response(ser.data)
        
        # 方法二：使用dr分页方法，可以显示上一页和下一页链接
        # return pg.get_paginated_response(ser.data)
~~~



![1582444791491-00ce0fd5-4b22-440c-a4fd-d4689721da83](图片\1582444791491-00ce0fd5-4b22-440c-a4fd-d4689721da83.png)



### 加密分页 CursorPagination

加密游标的分页，把上一页和下一页的id记住，不能直接通过数字跳转，只能获取上一页和下一页

~~~ python
from api.utils.serializesers.pager import PagerSerialiser
from rest_framework.response import Response
from rest_framework.pagination import PageNumberPagination,LimitOffsetPagination,CursorPagination


class MyPageNumberPagination(CursorPagination):

    # 页码的字段
    cursor_query_param = 'cursor'
    # 单页显示2个
    page_size = 2
    # 根据id进行排序
    ordering = 'id'


class Pager1View(APIView):
    def get(self, request, *args, **kwargs):

        # 查库返回Queryset对象
        roles = models.Role.objects.all()
        # 创建分页对象
        pg = MyPageNumberPagination()
        # 在数据库中获取分页的数据
        pager_roles = pg.paginate_queryset(queryset=roles, request=request, view=self)
        # 对数据进行序序列化
        ser = PagerSerialiser(instance=pager_roles, many=True)
        # 方法一：使用django方法
        # return Response(ser.data)
        
        # 方法二：使用dr分页方法，返回带超链接 需返回的时候用内置的响应方法
        return pg.get_paginated_response(ser.data)
~~~



![1582444700774-7678e1c7-75e3-45d2-a5b5-852b395a3777](图片\1582444700774-7678e1c7-75e3-45d2-a5b5-852b395a3777.png)