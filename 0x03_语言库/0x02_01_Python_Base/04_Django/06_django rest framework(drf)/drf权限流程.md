# drf权限流程

权限验证有两种方法，此处用第二种自定义方法

~~~ plain text
方法一
系统：
1）AllowAny：允许所有用户，校验方法直接返回True
2）IsAuthenticated：只允许登录用户
    必须request.user和request.user.is_authenticated都通过
3）IsAuthenticatedOrReadOnly：游客只读，登录用户无限制
    get、option、head 请求无限制
    前台请求必须校验 request.user和request.user.is_authenticated
4）IsAdminUser：是否是后台用户
    校验 request.user和request.user.is_staff    is_staff(可以登录后台管理系统的用户)
    
方法二
自定义：基于auth的Group与Permission表
1）自定义权限类，继承BasePermission，重写has_permission
2）has_permission中完成
    拿到登录用户 user <= request.user
    校验user的分组或是权限
    前两步操作失败 返回False => 无权限
    前两步操作成功 返回True => 有权限
~~~



# 流程

1. 当用户执行一个业务的时候，运行了as_view方法，进入了APIView类的dispatch方法
2. （此处不需要封装request了，因为在认证过程中封装了）
3. 进入self.initial方法中的self.check_permissions(request)方法
4. 里面执行了for循环，把每个权限类实例化对象

 \# 以上是drf源码内容，下面为自定义方法

1. 执行自己定义的权限类里面的has_permission方法，里面会判断request.user是否存在
2. 不存在就返回False，存在就返回True
3. 之后执行self.permission_denied报错方法，返回的是False就报错，可以自定义报错信息，在has_permission方法中写message = {"status": False, "error": "登录成功之后才能评论"}，就实现了自定义报错
4. 如果返回的是True就让他进入功能



# 返回值

- True, 有权限，进行下一步认证(频率认证)
- False, 无权限，将信息返回给前台



# 示例

## 重写原生方法 BasePermission.has_permission

必须继承BasePermission，必须实现has_permission



## utils/permission.py

~~~ python
from rest_framework.permissions import BasePermission

class MyPermission(BasePermission):
    message = '必须是管理员才能访问'
    def has_permission(self, request, view):
        if request.user.user_type != 1:
            return False
        return True
~~~



## models.py

~~~ python
from django.db import models

class UserInfo(models.Model):
    user_type_choices = [
        (1, '普通用户'),
        (2, 'VIP用户'),
        (3, 'SVIP用户'),
    ]
    username = models.CharField(max_length=32)
    password = models.CharField(max_length=32)
    user_type = models.IntegerField(choices=user_type_choices, default=0)
~~~



1. 全局

~~~ python
# settings.py
'DEFAULT_PERMISSION_CLASSES': ['apps.api.utils.permisson.MyPermission']
~~~



2. 局部

~~~ python
class UserInfoView(APIView):
    """
    用户信息
    """
    permission_classes = [MyPermisson, ]
    def get(self, request, *args, **kwargs):
        print(request.user)
        return HttpResponse('用户相关信息')
~~~

