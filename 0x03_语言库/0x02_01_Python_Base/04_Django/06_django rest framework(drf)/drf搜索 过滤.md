# drf 搜索 过滤



## 搜索

 

REST framework有提供了一个比较简洁的搜索功能给我们使用。

~~~ python
from rest_framework import filters

class UserViewSet(viewsets.ModelViewSet):
    queryset = UserInfo.objects.all()
    # 序列化
    serializer_class = UserSerializer
    # 分页
    pagination_class = publicViewSetPagination
    # 配置搜索功能
    filter_backends = (filters.SearchFilter, filters.OrderingFilter,)
    #设置搜索的关键字
    search_fields = ('username', 'password')
    
'''
        ^ ：搜索关键字开头的数据
        = ：完全匹配搜索      =name :则是完全匹配，不能模糊搜索
        @ ：全文搜索（目前只支持MySQL）
        $ ：正则表达式搜索
'''
~~~



配置filter_backends之后：会出现一个过滤器

![image9](D:\Note\python\Django\django rest framework(drf)\图片\image9.png)

这将允许客户端通过执行以下查询来过滤列表中的项目：

~~~ python
http://127.0.0.1/api/users/?search=xiaoming
~~~



## 过滤

### 自定义过滤

~~~  python
from rest_framework import filters

class UserViewSet(viewsets.ModelViewSet):
    queryset = UserInfo.objects.all()
    # 序列化
    serializer_class = UserSerializer
    # 分页
    pagination_class = publicViewSetPagination
    # 配置搜索功能
    filter_backends = (filters.SearchFilter)
    #设置搜索的关键字
    search_fields = ('username', 'password')

    def get_queryset(self):
        queryset = UserInfo.objects.all()  # 只有for时才会执行
        price_min = self.request.query_params.get("project")
        if price_min:
            queryset = queryset.filter(project=price_min)
        return queryset
~~~

这将允许客户端通过执行以下查询来过滤列表中的项目：

~~~ python
http://127.0.0.1/assets/system/?project=125
~~~



### 使用django-filter搜索

~~~ python
# 首先安装django-filter
pip install django-filter

# 然后注册到app
INSTALLED_APPS = [
    'django_filters',
]
~~~

自定义过滤类

~~~ python 
class GoodsFilter(django_filters.rest_framework.FilterSet):
    """
    商品的过滤类
    """
    pricemin = django_filters.NumberFilter(field_name='shop_price', help_text="最低价格", lookup_expr='gte')
    pricemax = django_filters.NumberFilter(field_name='shop_price', lookup_expr='lte')
    top_category = django_filters.NumberFilter(method='top_category_filter')
 
    def top_category_filter(self, queryset, name, value):
        return queryset.filter(Q(category_id=value) | Q(category__parent_category_id=value) | Q(
            category__parent_category__parent_category_id=value))
 
    class Meta:
        model = Goods
        fields = ['pricemin', 'pricemax', 'is_hot', 'is_new']
~~~

~~~ python 
class GoodsListViewSet(viewsets.ModelViewSet):
    # 数据
    queryset = Goods.objects.all()
    # 指定序列化的类
    serializer_class = GoodsSerializer
    # 配置搜索功能
    filter_backends = (DjangoFilterBackend, filters.SearchFilter)
    # 设置搜索的关键字
    # filter_fields = ('name',)  # 默认是全匹配，可自定义
    filter_class = GoodsFilter  # 自定义过滤
    search_fields = ('name', 'goods_brief', 'goods_desc')  # 搜索
~~~

效果如下：

![image11](D:\Note\python\Django\django rest framework(drf)\图片\image11.png)



> https://blog.csdn.net/weixin_42142216/article/details/91376274