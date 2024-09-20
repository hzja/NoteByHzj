# drf 序列化

## 作用

1. 进行数据的校验
2. 对数据对象进行转换

## 核心

1. 将数据库数据序列化为前端所需要的格式，并返回； 
2. 将前端发送的数据反序列化为模型类对象，并保存到数据库中。



**需要序列化字段**

- 在不做任何处理的情况下，我们定义的类里面的字段必须在model类中必须存在该字段
- 参与序列化的属性名必须与model类的属性相同
- 只出现在序列化中不出现在反序列化中我们要加只读属性read_only=True
- 如果我们Serializer类中定义字段类型为SerializerMethodField及自定义字段类型不用遵守类里面的字段必须在model类中必须存在该字段

**不需要序列化字段**

- 不需要序列化的属性字段在序列化类中不需要声明
- 不需要序列化的属性字段在序列化类中设置只写属性write_only=True



在开发REST API的视图中，虽然每个视图具体操作的数据不同，但增、删、改、查的实现流程基本套路化，所以这部分代码也是可以复用简化编写的：

增：校验请求数据 -> 执行反序列化过程 -> 保存数据库 -> 将保存的对象序列化并返回

删：判断要删除的数据是否存在 -> 执行数据库删除

改：判断要修改的数据是否存在 -> 校验请求的数据 -> 执行反序列化过程 -> 保存数据库 -> 将保存的对象序列化并返回

查：查询数据库 -> 将数据序列化并返回



**不同情况下用的序列化方法**

- 单表：最简单定义fileds字段
- ForeignKey：基于source参数链表操作
- ManyToManyField：基于SerializerMethodField+自定义一个钩子方法。

注意：choice显示中文可以使用source字段 "get_字段名_display"

![1586229358970-bbe91cef-ae60-41ef-baf4-aa071cbdeec1](D:\Note\python\Django\django rest framework(drf)\图片\1586229358970-bbe91cef-ae60-41ef-baf4-aa071cbdeec1.webp)



# 正向序列化

## 自定义字段 serializers.Serializer

方法一：单独取字段展示

~~~ python
from rest_framework import serializers

class UserInfoSerializer(serializers.Serializer):
   		'''
   		这里是声明序列化类,都是models.py已有的字段
   		'''
		username = serializers.CharField()
		password = serializers.CharField()
        
        # 可以查看主键的外键所在表中的任意值，但是需要写多行，如果全返回用下面的深度连表
        administrator = serializers.CharField(source='administrator.username')
~~~



## 基于Model自动生成字段 serializers.ModelSerializer

方法二：基于Model自动生成字段

1. title = serializers.CharField(source="xxx.xx'')
2. title= serializers.SerializerMethodField()  # 自定义显示

~~~ python
from rest_framework import serializers

class UserInfoSerializers(serializers.ModelSerializer): 
    class Meta:
        model = models.UserInfo   # models类名
        # 方法一：生成所有数据库字段
        # fields = "__all__"
        
        # 方法二：自定义字段名
        fields = ['username', 'id', 'password', 'rls',]   
~~~



### 基于SerializerMethodField+自定义一个钩子方法

~~~ python
class TicketRecordSerializers(serializers.ModelSerializer):
    admin_username = serializers.SerializerMethodField()

    class Meta:
        model = TicketRecord
        fields = "__all__"
        
    '''
    下面是自定义序列化字段，序列化的属性值由方法来提供，
       方法的名字：固定为 get_属性名，
       方法的参数：self为序列化对象，obj为当前序列化的model的TicketRecord对象,名字随便写
       注意 : 建议自定义序列化字段名不要与model已有的属性名重名,否则会覆盖model已有字段的声明
       注意 : 自定义序列化字段要用SerializerMethodField()作为字段类型
    '''

    def get_admin_username(self, obj):
        ticket_id = obj.ticket_id
        tag_list = WorkflowAdmin.objects.filter(workflow__ticketrecord__ticket_id=ticket_id)
        for i in tag_list:
            admin_username = i.username
            return admin_username
~~~

**注意**：自定义方法可以跨表获取数据内容



## 调用序列化方法

~~~ python
class UserInfoView(APIView):
    def get(self, request, *args, **kwargs):
        # 查库，返回Queryset对象
        users = models.UserInfo.objects.all()       
        # 当返回多个对象时，需要写many=True
        ser = UserInfoSerializer(instance=users, many=True)
        # 或
        # users = models.UserInfo.objects.all().first()
        # ser = UserSerializer(instance=users, many=False)
        
        # ser.data已经是转换完成的结果
        # ensure_ascii=False表示返回值不会被编码成二进制
        ret = json.dumps(ser.data, ensure_ascii=False)
        return Response(ret)
    
    def post(self, request, *args, **kwargs):
        # 验证，对请求发来的数据进行验证
        ser = ModelUserSerializer(data=request.data)
        if ser.is_valid():
            print(ser.validated_data)
        else:
            print(ser.errors)
        return Response('POST请求，响应内容')
~~~

**注意：**在使用APIView时，数据展示的时候序列化多个数据对象的时候用many=True，序列化单个数据对象的时候用many=False



## 深度自动化连表

**UserInfo**类中有外键和多对多的键，直接序列化的结果是只显示id，如果想得到更多的信息可以用*depth*字段，可以理解为深度读取，默认为0，就是我们默认显示的结果。如果改为1，表示深度为1，就可以再往下读取一层所有的信息，在这里就可以读取到用户的角色和组别两个类中所有的信息。

~~~ python
from rest_framework import serializers

class UserInfoSerializers(serializers.ModelSerializer):
    class Meta:
        model = models.UserInfo
        fields = "__all__"
        # 深度为1
        depth = 1  # 0 ~ 10
~~~

效果

![1582380736139-f1919802-8673-423b-a6bc-752acc5f5126](D:\Note\python\Django\django rest framework(drf)\图片\1582380736139-f1919802-8673-423b-a6bc-752acc5f5126.png)



为了解释各种类型的关系字段，将为示例使用几个简单的模型。我们的模型将使用音乐专辑，以及每张专辑中列出的曲目。

~~~ python
class Album(models.Model):
    album_name = models.CharField(max_length=100)
    artist = models.CharField(max_length=100)
    
class Track(models.Model):
    album = models.ForeignKey(Album, related_name='tracks', on_delete=models.CASCADE)
    order = models.IntegerField()
    title = models.CharField(max_length=100)
    duration = models.IntegerField()
    
    class Meta:
        unique_together = ('album', 'order')
        ordering = ['order']
        
    def __unicode__(self):
        return '%d: %s' % (self.order, self.title)
~~~



# Serializer关系

关系字段用于表示模型关系。 它们可以应用于 `ForeignKey`，`ManyToManyField` 和 `OneToOneField` 关系，反向关系以及 `GenericForeignKey` 等自定义关系。

## StringRelatedField

StringRelatedField 用于使用 __unicode__ 方法表示关系。

例如，下面的序列化类。

~~~ python
class AlbumSerializer(serializers.ModelSerializer):
    tracks = serializers.StringRelatedField(many=True)
    
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
~~~

将序列化为以下形式。

~~~ python
{
    'album_name': 'Things We Lost In The Fire',
    'artist': 'Low',
    'tracks': [
        '1: Sunflower',
        '2: Whitetail',
        '3: Dinosaur Act',
        ...
    ]
}
~~~

该字段是只读的。

**参数**:

- `many` - 如果是一对多的关系，就将此参数设置为 `True`.



## PrimaryKeyRelatedField

`PrimaryKeyRelatedField` 用于使用其主键表示关系。

例如，以下序列化类：

~~~ python
class AlbumSerializer(serializers.ModelSerializer):
    tracks = serializers.PrimaryKeyRelatedField(many=True, read_only=True)
    
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
~~~

将序列化为这样的表示：

~~~ python
{
    'album_name': 'Undun',
    'artist': 'The Roots',
    'tracks': [
        89,
        90,
        91,
        ...
    ]
}
~~~

默认情况下，该字段是可读写的，但您可以使用 `read_only` 标志更改此行为。

**参数**:

- `queryset` - 验证字段输入时用于模型实例查询的查询集。必须显式地设置查询集，或设置 `read_only=True`。
- `many` - 如果应用于一对多关系，则应将此参数设置为 `True`。
- `allow_null` - 如果设置为 `True`，那么该字段将接受 `None` 值或可为空的关系的空字符串。默认为 `False`。
- `pk_field` - 设置为一个字段来控制主键值的序列化/反序列化。例如， `pk_field=UUIDField(format='hex')` 会将 UUID 主键序列化为其紧凑的十六进制表示形式。



## HyperlinkedRelatedField

`HyperlinkedRelatedField` 用于使用超链接来表示关系。

例如，以下序列化类：

~~~ python
class AlbumSerializer(serializers.ModelSerializer):
    tracks = serializers.HyperlinkedRelatedField(
        many=True,
        read_only=True,
        view_name='track-detail'
    )
    
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
~~~

将序列化为这样的表示：

~~~ python
{
    'album_name': 'Graceland',
    'artist': 'Paul Simon',
    'tracks': [
        'http://www.example.com/api/tracks/45/',
        'http://www.example.com/api/tracks/46/',
        'http://www.example.com/api/tracks/47/',
        ...
    ]
}
~~~

默认情况下，该字段是可读写的，但您可以使用 `read_only` 标志更改此行为。

**注意**：该字段是为映射到接受单个 URL 关键字参数的 URL 的对象而设计的，如使用 `lookup_field` 和 `lookup_url_kwarg` 参数设置的对象。

这适用于包含单个主键或 slug 参数作为 URL 一部分的 URL。

如果需要更复杂的超链接表示，你需要自定义该字段，稍后会详解。

**参数**：

- `view_name` - 用作关系目标的视图名称。如果你使用的是标准路由器类，则这将是一个格式为 `<modelname>-detail` 的字符串。**必填**.
- `queryset` - 验证字段输入时用于模型实例查询的查询集。必须显式地设置查询集，或设置 `read_only=True`。
- `many` - 如果应用于一对多关系，则应将此参数设置为 `True`。
- `allow_null` - 如果设置为 `True`，那么该字段将接受 `None` 值或可为空的关系的空字符串。默认为 `False`。
- `lookup_field` - 用于查找的目标字段。对应于引用视图上的 URL 关键字参数。默认是 `'pk'`.
- `lookup_url_kwarg` - 与查找字段对应的 URL conf 中定义的关键字参数的名称。默认使用与 `lookup_field` 相同的值。
- `format` - 如果使用格式后缀，则超链接字段将使用与目标相同的格式后缀，除非使用 `format` 参数进行覆盖。



## SlugRelatedField

`SlugRelatedField` 用于使用目标上的字段来表示关系。

例如，以下序列化类：

~~~ python
class AlbumSerializer(serializers.ModelSerializer):
    tracks = serializers.SlugRelatedField(
        many=True,
        read_only=True,
        slug_field='title'
     )
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
~~~

将序列化为这样的表示：

~~~ python
{
    'album_name': 'Dear John',
    'artist': 'Loney Dear',
    'tracks': [
        'Airport Surroundings',
        'Everything Turns to You',
        'I Was Only Going Out',
        ...
    ]
}
~~~

默认情况下，该字段是可读写的，但您可以使用 `read_only` 标志更改此行为。

将 `SlugRelatedField` 用作读写字段时，通常需要确保 slug 字段与 `unique=True` 的模型字段相对应。

**参数**：

- `slug_field` - 用来表示目标的字段。这应该是唯一标识给定实例的字段。例如， `username`。**必填**
- `queryset` - 验证字段输入时用于模型实例查询的查询集。必须显式地设置查询集，或设置 `read_only=True`。
- `many` - 如果应用于一对多关系，则应将此参数设置为 `True`。
- `allow_null` - 如果设置为 `True`，那么该字段将接受 `None` 值或可为空的关系的空字符串。默认为 `False`。



## HyperlinkedModelSerializer

HyperlinkedModelSerializer类类似于ModelSerializer类，不同之处在于它使用超链接来表示关联关系而不是主键。默认情况下序列化器将包含一个url字段而不是主键字段。

此字段可以作为身份关系应用，例如 `HyperlinkedModelSerializer` 上的 `'url'` 字段。它也可以用于对象的属性。例如，以下序列化类：

~~~ python
class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = UserInfo
        fields = "__all__"
        
class UserViewSet(viewsets.ModelViewSet):
    """
    允许用户查看或编辑的API路径。
    """
    queryset = UserInfo.objects.all()
    serializer_class = UserSerializer
~~~

~~~ python
router.register(r'users', views.UserViewSet)
~~~

将序列化为这样的表示：

![image13](D:\Note\python\Django\django rest framework(drf)\图片\image13.png)该字段始终为只读。

**参数**：

- `view_name` - 用作关系目标的视图名称。如果你使用的是标准路由器类，则这将是一个格式为 `<modelname>-detail` 的字符串。**必填**。
- `lookup_field` - 用于查找的目标字段。对应于引用视图上的 URL 关键字参数。默认是 `'pk'`。
- `lookup_url_kwarg` - 与查找字段对应的 URL conf 中定义的关键字参数的名称。默认使用与 `lookup_field` 相同的值。
- `format` - 如果使用格式后缀，则超链接字段将使用与目标相同的格式后缀，除非使用 `format` 参数进行覆盖。



# 嵌套序列化

前面的实例适用于处理只有简单数据类型的对象，但是有时候我们也需要表示更复杂的对象，其中对象的某些属性可能不是字符串、日期、整数这样简单的数据类型。

Serializer类本身也是一种Field，并且可以用来表示一个对象类型嵌套在另一个对象中的关系。

如果该字段用于表示一对多关系，则应将 `many=True` 标志添加到序列化字段。

例如，以下序列化类：

~~~ python
class TrackSerializer(serializers.ModelSerializer):
    class Meta:
        model = Track
        fields = ('order', 'title', 'duration')
        
class AlbumSerializer(serializers.ModelSerializer):
    tracks = TrackSerializer(many=True, read_only=True)
    
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
~~~



## 可写嵌套序列化类

默认情况下，嵌套序列化类是只读的。如果要支持对嵌套序列化字段的写操作，则需要创建 `create()` 和/或 `update()` 方法，以明确指定应如何保存子关系。

~~~ python
class TrackSerializer(serializers.ModelSerializer):
    class Meta:
        model = Track
        fields = ('order', 'title', 'duration')
        
class AlbumSerializer(serializers.ModelSerializer):
    tracks = TrackSerializer(many=True)
    
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
        
    def create(self, validated_data):
        tracks_data = validated_data.pop('tracks')
        album = Album.objects.create(**validated_data)
        for track_data in tracks_data:
            Track.objects.create(album=album, **track_data)
        return album
    
>>> data = {
    'album_name': 'The Grey Album',
    'artist': 'Danger Mouse',
    'tracks': [
        {'order': 1, 'title': 'Public Service Announcement', 'duration': 245},
        {'order': 2, 'title': 'What More Can I Say', 'duration': 264},
        {'order': 3, 'title': 'Encore', 'duration': 159},
    ],
}
>>> serializer = AlbumSerializer(data=data)
>>> serializer.is_valid()
True
>>> serializer.save()
<Album: Album object>
~~~



## 父类查询所有子类

首先有两个类

一级目录和二级目录

~~~ python
class Level1Model(BaseModel):
    id = models.BigAutoField(primary_key=True, verbose_name="一级目录id")
    name = models.CharField(max_length=50, verbose_name="一级目录名称", null=True, blank=True)
    level = models.CharField(max_length=10, verbose_name="等级", default=1)
    class Meta:
        managed = True
        db_table = 'menu_lv1'
        verbose_name = "一级目录"
        verbose_name_plural = verbose_name
        
# 二级目录模型
class Level2Model(BaseModel):
    id = models.BigAutoField(primary_key=True, verbose_name="level2_id")
    name = models.CharField(max_length=50, verbose_name="二级目录名称", null=True, blank=True)
    lv1_id = models.ForeignKey(Level1Model, on_delete=models.CASCADE, verbose_name="一级目录id外键", null=True, blank=True,
                              default=None, related_name='children')
    level = models.CharField(max_length=10, verbose_name="等级", default=2)
    class Meta:
        managed = True
        db_table = 'menu_lv2'
        verbose_name = "二级目录"
        verbose_name_plural = verbose_name
~~~

只需要在二级目录中关联父类的那一行，加一个**related_name**

再参考这个序列化器

~~~ python
# 二级目录的序列化器
class Level2ModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Level2Model
        fields = ['id', 'name', 'modify_datetime', 'lv1_id', 'level']
        
# 一级目录的序列化器
class Level1ModelSerializer(serializers.ModelSerializer):
    children = Level2ModelSerializer(many=True)  
    # ↑ 这里的变量名为related_name的
    class Meta:
        model = Level1Model
        fields = ['id', 'name', 'modify_datetime', 'level', 'children']
                                                               # ↑这里的也是
            
# view中调用            
class GroupViewSet(viewsets.ModelViewSet):
    queryset = Level1Model.objects.all()
    serializer_class = Level1ModelSerializer
~~~

~~~ python
"results": [
        {
            "id": 2,
            "name": "一级目录",
            "modify_datetime": "2020-08-20T17:36:10.317624+08:00",
            "level": "1",
            "children": [
                {
                    "id": 2,
                    "name": "二级目录1",
                    "modify_datetime": "2020-08-20T17:40:08.266023+08:00",
                    "lv1_id": 2,
                    "level": "2"
                },
                {
                    "id": 3,
                    "name": "二级目录2",
                    "modify_datetime": "2020-08-20T17:40:25.780470+08:00",
                    "lv1_id": 2,
                    "level": "2"
                }
            ]
        }]
~~~



# 反向序列化

**反向序列化的定义**：`从“前端接口”接收添加的数据并序列化后返回一个object对象给后端`，而且可以进行数据验证以及添加到数据库的操作。

~~~ python
class bookModelSerializers(serializers.ModelSerializer):
    class Meta():
        model = models.Books
        fields = '__all__'

       
class booksView(APIView):
    def get(self,request):
        params = request.query_params.get('id')
        if params:
            res=Books.objects.filter(pk=params)
            serializer = bookModelSerializers(res, many=True)
            return JsonResponse(serializer.data, safe=False)
        res_obj = Books.objects.all()
        serializer = bookModelSerializers(res_obj,many=True)
        return JsonResponse(serializer.data,safe=False)
    
    def post(self,request):
        params = request.data
        # 通过构造序列化器对象，并将要反序列化的数据传递给data构造参数，进而进行验证
        serializer = bookModelSerializers(data=params)
        # is_valid()方法还可以在验证失败时抛出异常serializers.ValidationError，
		# 可以通过传递**raise_exception=True**参数开启，REST framework接收到此异常，会向前端返回HTTP 400 Bad Request响应。
        if serializer.is_valid():
            serializer.save()
        return JsonResponse(serializer.data)
~~~

~~~ python
class bookModelSerializers(serializers.Serializer):
	#与上面不同的点在于,在这里需要向Forms组件一样,把显示在html页面上的信息写出来
	id = serializers.CharField(read_only=True)
    book_name = serializers.CharField(max_length=32)
    book_auther=serializers.CharField(max_length=32)
    book_price=serializers.DecimalField(max_digits=5)
    create_time = serializers.DateField(read_only=True)
    
    # 你可以通过向 Serializer 子类添加 .validate_<field_name> 方法来指定自定义字段级验证
    def validate_book_name(self, data):
        if data.startswith('sb') or data.endswith('sb'):
            raise ValidationError('不能以sb开头,不能以sb结尾')
        return data
    
    # 如果创建序列化器对象的时候，没有传递instance实例，则调用save()方法的时候，create()被调用，相反，如果传递了instance实例，则调用save()方法的时候，update()被调用。
    def create(self, validated_data):
    	# **validated_data代表打散写入
        return models.Books.objects.create(**validated_data)
    
    # 前面的验证数据成功后,我们可以使用序列化器来完成数据反序列化的过程.这个过程可以把数据转成模型类对象.
    def update(self, instance, validated_data):
        instance.book_name = validated_data.get('book_name')
        instance.book_auther = validated_data.get('book_auther')
        instance.book_price = validated_data.get('book_price')
        # 如果需要在返回数据对象的时候，也将数据保存到数据库中，加save
        instance.save()
        return instance



#另一点是返回值最好携带状态码和信息
class bookView(APIView):
    def get(self,request,pk):
        dic = {'code':100,'msg':'成功'}
        book = models.Books.objects.all()
        ser = serializers.BookeSerializer(instance=book,many=True)
        dic['data']=ser.data
        return Response(dic)
    def post(self,request,pk):
        dic = {'code': 100, 'msg': '成功'}
        ser = serializers.BookeSerializer(data=request.data)
        if ser.is_valid():
            ser.save()
            dic['data']=ser.data
            return Response(dic)
        else:
            dic['msg']='格式不正确'
            dic['data']=ser.errors
            return Response(dic)
~~~



## 保存实例

如果我们希望能够返回基于验证数据的完整对象实例，我们需要实现其中一个或全部实现.create()和update()方法。例如：

~~~ python
class bookModelSerializers(serializers.Serializer):
	id = serializers.CharField(read_only=True)
    book_name = serializers.CharField(max_length=32)
    book_auther=serializers.CharField(max_length=32)
    book_price=serializers.DecimalField(max_digits=5)
    create_time = serializers.DateField(read_only=True)
    
    # 如果创建序列化器对象的时候，没有传递instance实例，则调用save()方法的时候，create()被调用，相反，如果传递了instance实例，则调用save()方法的时候，update()被调用。
    def create(self, validated_data):
    	# **validated_data代表打散写入
        return models.Books.objects.create(**validated_data)
    
    # 前面的验证数据成功后,我们可以使用序列化器来完成数据反序列化的过程.这个过程可以把数据转成模型类对象.
    def update(self, instance, validated_data):
        instance.book_name = validated_data.get('book_name')
        instance.book_auther = validated_data.get('book_auther')
        instance.book_price = validated_data.get('book_price')
        # 如果需要在返回数据对象的时候，也将数据保存到数据库中，加save
        return instance
~~~

如果你的对象实例对应Django的模型，你还需要确保这些方法将对象保存到数据库。例如，如果Comment是一个Django模型的话，具体的方法可能如下所示：

~~~ python
    def create(self, validated_data):
    	# **validated_data代表打散写入
        return models.Books.objects.create(**validated_data)
    
    def update(self, instance, validated_data):
        instance.book_name = validated_data.get('book_name')
        instance.book_auther = validated_data.get('book_auther')
        instance.book_price = validated_data.get('book_price')
        # 如果需要在返回数据对象的时候，也将数据保存到数据库中，加save
        instance.save()
        return instance
~~~

现在当我们反序列化数据的时候，基于验证过的数据我们可以调用.save()方法返回一个对象实例。

~~~ python
comment = serializer.save()
~~~

调用.save()方法将创建新实例或者更新现有实例，具体取决于实例化序列化器类的时候是否传递了现有实例：

~~~ python
# .save() will create a new instance.
serializer = CommentSerializer(data=data)
# .save() will update the existing `comment` instance.
serializer = CommentSerializer(comment, data=data)
~~~

.create()和.update()方法都是可选的。你可以根据你序列化器类的用例不实现、实现它们之一或都实现。

#### 传递附加属性到.save()

有时你会希望你的视图代码能够在保存实例时注入额外的数据。此额外数据可能包括当前用户，当前时间或不是请求数据一部分的其他信息。

你可以通过在调用.save()时添加其他关键字参数来执行此操作。例如：

~~~ python
serializer.save(owner=request.user)
~~~

在.create()或.update()被调用时，任何其他关键字参数将被包含在validated_data参数中。

#### 直接重写.save()

在某些情况下.create()和.update()方法可能无意义。例如在contact form中，我们可能不会创建新的实例，而是发送电子邮件或其他消息。

在这些情况下，你可以选择直接重写.save()方法，因为那样更可读和有意义。

例如：

~~~ python
class ContactForm(serializers.Serializer):
    email = serializers.EmailField()
    message = serializers.CharField()
    def save(self):
        email = self.validated_data['email']
        message = self.validated_data['message']
        send_email(from=email, message=message)
~~~

请注意在上述情况下，我们现在不得不直接访问serializer的.validated_data属性。



## 验证

反序列化数据的时候，你始终需要先调用is_valid()方法，然后再尝试去访问经过验证的数据或保存对象实例。如果发生任何验证错误，.errors属性将包含表示生成的错误消息的字典。例如：

~~~ python
serializer = CommentSerializer(data={'email': 'foobar', 'content': 'baz'})
serializer.is_valid()
# False
serializer.errors
# {'email': [u'Enter a valid e-mail address.'], 'created': [u'This field is required.']}
~~~

字典里的每一个键都是字段名称，值是与该字段对应的任何错误消息的字符串列表。non_field_errors键可能存在，它将列出任何一般验证错误信息。non_field_errors的名称可以通过REST framework设置中的NON_FIELD_ERRORS_KEY来自定义。 当对对象列表进行序列化时，返回的错误是每个反序列化项的字典列表。

#### 抛出无效数据的异常

.is_valid()方法使用可选的raise_exception标志，如果存在验证错误将会抛出一个serializers.ValidationError异常。

这些异常由REST framework提供的默认异常处理程序自动处理，默认情况下将返回HTTP 400 Bad Request响应。

~~~ python
# 如果数据无效就返回400响应
serializer.is_valid(raise_exception=True)
~~~

#### 字段级别的验证

你可以通过向你的Serializer子类中添加.validate_<field_name>方法来指定自定义字段级别的验证。这些类似于Django表单中的.clean_<field_name>方法。

这些方法采用单个参数，即需要验证的字段值。

你的validate_<field_name>方法应该返回一个验证过的数据或者抛出一个serializers.ValidationError异常。例如：

~~~ python
from rest_framework import serializers
class BlogPostSerializer(serializers.Serializer):
    title = serializers.CharField(max_length=100)
    content = serializers.CharField()
    def validate_title(self, value):
        """
        Check that the blog post is about Django.
        """
        if 'django' not in value.lower():
            raise serializers.ValidationError("Blog post is not about Django")
        return value
~~~

注意： 如果你在序列化器中声明<field_name>的时候带有required=False参数，字段不被包含的时候这个验证步骤就不会执行。

#### 对象级别的验证

要执行需要访问多个字段的任何其他验证，请添加一个.validate()方法到你的Serializer子类中。这个方法采用字段值字典的单个参数，如果需要应该抛出一个 ValidationError异常，或者只是返回经过验证的值。例如：

~~~ python
from rest_framework import serializers
class EventSerializer(serializers.Serializer):
    description = serializers.CharField(max_length=100)
    start = serializers.DateTimeField()
    finish = serializers.DateTimeField()
    def validate(self, data):
        """
        Check that the start is before the stop.
        """
        if data['start'] > data['finish']:
            raise serializers.ValidationError("finish must occur after start")
        return data
~~~

#### 验证器

序列化器上的各个字段都可以包含验证器，通过在字段实例上声明，例如：

~~~ python
def multiple_of_ten(value):
    if value % 10 != 0:
        raise serializers.ValidationError('Not a multiple of ten')
class GameRecord(serializers.Serializer):
    score = IntegerField(validators=[multiple_of_ten])
    ...
~~~

序列化器类还可以包括应用于一组字段数据的可重用的验证器。这些验证器要在内部的Meta类中声明，如下所示：

~~~ python
class EventSerializer(serializers.Serializer):
    name = serializers.CharField()
    room_number = serializers.IntegerField(choices=[101, 102, 103, 201])
    date = serializers.DateField()
    class Meta:
        # 每间屋子每天只能有1个活动。
        validators = UniqueTogetherValidator(
            queryset=Event.objects.all(),
            fields=['room_number', 'date']
        )
~~~

