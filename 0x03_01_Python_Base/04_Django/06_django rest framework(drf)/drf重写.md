# drf 重写

源代码：这里可以根据自己需求修改继承和重写

~~~ python
"""
Basic building blocks for generic class based views.

We don't bind behaviour to http method handlers yet,
which allows mixin classes to be composed in interesting ways.
"""
from rest_framework import status
from rest_framework.response import Response
from rest_framework.settings import api_settings


class CreateModelMixin:
    """
    Create a model instance.
    """
    def create(self, request, *args, **kwargs):
        serializer = self.get_serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        self.perform_create(serializer)
        headers = self.get_success_headers(serializer.data)
        return Response(serializer.data, status=status.HTTP_201_CREATED, headers=headers)

    def perform_create(self, serializer):
        serializer.save()

    def get_success_headers(self, data):
        try:
            return {'Location': str(data[api_settings.URL_FIELD_NAME])}
        except (TypeError, KeyError):
            return {}


class ListModelMixin:
    """
    List a queryset.
    """
    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())

        page = self.paginate_queryset(queryset)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(queryset, many=True)
        return Response(serializer.data)


class RetrieveModelMixin:
    """
    Retrieve a model instance.
    """
    def retrieve(self, request, *args, **kwargs):
        instance = self.get_object()
        serializer = self.get_serializer(instance)
        return Response(serializer.data)


class UpdateModelMixin:
    """
    Update a model instance.
    """
    def update(self, request, *args, **kwargs):
        partial = kwargs.pop('partial', False)
        instance = self.get_object()
        serializer = self.get_serializer(instance, data=request.data, partial=partial)
        serializer.is_valid(raise_exception=True)
        self.perform_update(serializer)

        if getattr(instance, '_prefetched_objects_cache', None):
            # If 'prefetch_related' has been applied to a queryset, we need to
            # forcibly invalidate the prefetch cache on the instance.
            instance._prefetched_objects_cache = {}

        return Response(serializer.data)

    def perform_update(self, serializer):
        serializer.save()

    def partial_update(self, request, *args, **kwargs):
        kwargs['partial'] = True
        return self.update(request, *args, **kwargs)


class DestroyModelMixin:
    """
    Destroy a model instance.
    """
    def destroy(self, request, *args, **kwargs):
        instance = self.get_object()
        self.perform_destroy(instance)
        return Response(status=status.HTTP_204_NO_CONTENT)

    def perform_destroy(self, instance):
        instance.delete()
~~~



##### create方法（CreateMinix）

> 一般要保存对象需要做额外操作时重写

~~~ python
    # 当需要接口支持批量创建时
    def create(self, request, *args, **kwargs):
        # 如果传的是列表，执行批量创建
        if isinstance(request.data, list):
            serializer = self.get_serializer(data=request.data, many=True)
        else:
            serializer = self.get_serializer(data=request.data)
        # is_valid()方法还可以在验证失败时抛出异常serializers.ValidationError，可以通过传递raise_exception=True参数开启，REST framework接收到此异常，会向前端返回HTTP 400 Bad Request响应
        serializer.is_valid(raise_exception=True)
        self.perform_create(serializer)
        header = self.get_success_headers(serializer.data)
        return Response(serializer.data, status=status.HTTP_201_CREATED, headers=header)
    
    # 使用JWT时
    def create(self, request, *args, **kwargs):
        serializer = self.get_serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        user = self.perform_create(serializer)

        # 这一步会序列化对象，所有fields中的字段都会被序列号，而code已经被删除了
        # ，所以code中加入了write_only=True，让他不要被序列化和返回前端
        # register的用户对象
        re_dict = serializer.data

        # 将token返回给前端
        payload = jwt_payload_handler(user)
        re_dict["token"] = jwt_encode_handler(payload)
        re_dict["name"] = user.name if user.name else user.username

        header = self.get_success_headers(serializer.data)
        return Response(re_dict, status=status.HTTP_201_CREATED, headers=header)
~~~



##### perform_create（CreateMinix）

> 执行保存对象的操作,被create地调用，一般和信号量配合使用

~~~ python
# 重写次方法，返回user
def perform_create(self, serializer):
     # 这里需要操作数据库，而密码需要加密，所以用到了信号量
     # 在每次post保存的时候，将密码加密
     return serializer.save()
~~~



 get_serializer_class 动态设置serializer_class 

> 栗子：本处，添加收藏时，序列化当前用户
> 获取收藏时，返回goods的序列化结果

~~~ python
def get_serializer_class(self):
    if self.action == "list":
        return UserFavDetailSerializer
    elif self.action == "create":
        return UserFavSerializer

    return UserFavSerializer
~~~



##### get_permissions 动态设置permissions

> 栗子：本处，创建用户时，不要求验证是否已经登录，其他操作，如更新，获取个人信息都需要验证

~~~ python 
def get_permissions(self):
    if self.action == "retrieve":
        return [permissions.IsAuthenticated()]
    elif self.action == "create":
        return []
    return []
~~~



##### get_queryset 根据需求过滤指定条件的查询结果集

~~~ python 
# 我最开始是重写了list方法，后来发现很垃圾，可以直接使用下面的过滤方法
def list(self, request, *args, **kwargs):
    try:
        project_id = request.GET.get('project')
        if not project_id:
            return Response({"msg": "project不存在", "code": 2, "data": {}})
        project_info = RtInfo.objects.filter(project=project_id).order_by('-create_time')
        count = RtInfo.objects.filter(project=project_id).count()
        pg = RtInfoViewSetPagination()
        pager_roles = pg.paginate_queryset(queryset=project_info, request=request, view=self)
        ser = RtInfoSerializer(instance=pager_roles, many=True)
        ret = ser.data
        return Response({'code': 0, "msg": "success", "data": {"count": count, "results": ret}})
    except Exception as e:
        print('发生错误：', e)
        return Response({"msg": "出现了无法预料的view视图错误：%s" % e, "code": 1, "data": {}})

    # 重写get_queryset
    def get_queryset(self):
        queryset = RtInfo.objects.all()
        price_min = self.request.query_params.get("project")
        if price_min:
            queryset = queryset.filter(project=price_min)
            return queryset
~~~



##### perform_destroy（DestroyMixin）执行删除操作

~~~ python 
def perform_destroy(self, instance):
    goods = instance.goods
    goods.goods_num += instance.nums
    goods.save()
    instance.delete()
~~~



##### perform_update(Update_Mixin) 执行更新操作

~~~ python
# 先继承原生update方法
def update(self, request, *args, **kwargs):
    partial = kwargs.pop('partial', False)
    instance = self.get_object()
    serializer = self.get_serializer(instance, data=request.data, partial=partial)
    serializer.is_valid(raise_exception=True)
    self.perform_update(serializer)

    if getattr(instance, '_prefetched_objects_cache', None):
        # If 'prefetch_related' has been applied to a queryset, we need to
        # forcibly invalidate the prefetch cache on the instance.
        instance._prefetched_objects_cache = {}

        return Response(serializer.data)

    # 保存结果的时候回调用第三方API
    def perform_update(self, serializer):
        serializer.save()
        existed_record = RtServerInfo.objects.get(id=serializer.instance.id)
        asset_number = existed_record.asset_number
        parameter = 'asset_number=='
        cmdb_data = CmdbAPI(url=CMDB_URL, username=CMDB_USER, password=CMDB_PASSWORD)
        cmdb_info_get = cmdb_data.get_data(q='manifest==rack_server and ' + parameter + asset_number).get('result')
        _id = cmdb_info_get[0].get('.id')
        data = {
            "id": _id,
            "value": {
                "mac_pxe": existed_record.mac_address,
            }
        }
        cmdb_info_update = cmdb_data.update_data(data)
        success = cmdb_info_update.get('success')
        print(success)


        def partial_update(self, request, *args, **kwargs):
            kwargs['partial'] = True
            return self.update(request, *args, **kwargs)
~~~



##### retrieve(RetrieveMixin)

> 栗子：每当前端使用get/:id访问Retrieve时，增加该商品的点击数量，get_object是拿到model单个纪录的实例化对象

~~~ python 
# 获取详细信息
def retrieve(self, request, *args, **kwargs):
    try:
        id = str(kwargs['pk'])
        rtinfo = RtServerInfo.objects.filter(rt_number=id)
        ser = RtServerInfoSerializer(instance=rtinfo, many=True)
        ret = ser.data
        return Response(ret)
    except Exception as e:
        print('发生错误：', e)
        return Response({"msg": "出现了无法预料的view视图错误：%s" % e, "code": 1, "data": {}})
~~~

