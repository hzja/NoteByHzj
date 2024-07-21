# drf认证流程

Django认证与三种方法，此处使用第三种自定义方法

~~~ plain text
方法一
系统：session认证
rest_framework.authentication.SessionAuthentication
ajax请求通过认证：
cookie中要携带 sessionid、csrftoken，请求头中要携带 x-csrftoken

方法二
第三方：jwt认证 
rest_framework_jwt.authentication.JSONWebTokenAuthentication
ajax请求通过认证：
请求头中要携带 authorization，值为 jwt空格token

方法三（常用）
自定义：基于jwt、其它
1）自定义认证类，继承BaseAuthentication(或其子类)，重写authenticate
2）authenticate中完成
    拿到认证标识 auth
    反解析出用户 user
    前两步操作失败 返回None => 游客
    前两步操作成功 返回user，auth => 登录用户
    注：如果在某个分支抛出异常，直接定义失败 => 非法用户
~~~



## 流程

1. 当用户进行登录的时候，运行了登录类的as_view()方法，进入了APIView类的dispatch方法
2. 执行self.initialize_request这个方法，里面封装了request和认证对象列表等其他参数
3. 执行self.initial方法中的self.perform_authentication，里面运行了user方法
4. 再执行了user方法里面的self._authenticate()方法

 \#以上是drf源码内容，下面为自定义方法

1. 然后执行了自己定义的类中的authenticate方法，自己定义的类继承了BaseAuthentication类，里面有  authenticate方法，如果自己定义的类中没有authenticate方法会报错
2. 把从authenticate方法得到的user和auth赋值给user和auth方法
3. 这两个方法把user和auth的值分别赋值给request.user：是登录用户的对象；request.auth：是认证的信息字典



## 返回值

- 没有携带认证信息，直接返回None => 游客
- 有认证信息，校验成功，返回一个元组，第一个参数赋值给request.user，第二个赋值给request.auth
- 有认证信息，校验失败，抛异常 => 非法用户



## 封装request的源码

~~~ python
# APIView的dispatch中有个self.initialize_request，它返回了一个Request类，
    它封装了django的request和认证对象列表等其他参数

class APIView(View):
    # 1.进入了APIView类的dispatch方法
	def dispatch(self, request, *args, **kwargs):

		self.args = args
		self.kwargs = kwargs
		# 2.执行了self.initialize_request这个方法，封装了request和认证对象列表等其他参数
		request = self.initialize_request(request, *args, **kwargs)
		self.request = request
        self.headers = self.default_response_headers  # deprecate?

        try:
            # 3.执行self.initial方法中的self.perform_authentication，里面运行了user方法
            self.initial(request, *args, **kwargs)

            # Get the appropriate handler method
            if request.method.lower() in self.http_method_names:
                handler = getattr(self, request.method.lower(),
                                  self.http_method_not_allowed)
            else:
                handler = self.http_method_not_allowed

            response = handler(request, *args, **kwargs)
            
     # 2.执行了self.initialize_request这个方法，封装了request和认证对象列表等其他参数
     '''
     def initialize_request(self, request, *args, **kwargs):

         parser_context = self.get_parser_context(request)
 
         return Request(
             request,
             parsers=self.get_parsers(),
             authenticators=self.get_authenticators(), # [MyAuthentication(),]
             negotiator=self.get_content_negotiator(),
             parser_context=parser_context
         )
         '''
        
    # 3.执行self.initial方法中的self.perform_authentication，里面运行了user方法
    '''
    def initial(self, request, *args, **kwargs):

        self.format_kwarg = self.get_format_suffix(**kwargs)

        neg = self.perform_content_negotiation(request)
        request.accepted_renderer, request.accepted_media_type = neg
        
		# 版本方法在这里
        version, scheme = self.determine_version(request, *args, **kwargs)
        request.version, request.versioning_scheme = version, scheme

        # 认证 权限 节流三兄弟
        self.perform_authentication(request)
        self.check_permissions(request)
        self.check_throttles(request)
        '''
    
    # 3.执行self.initial方法中的self.perform_authentication，里面运行了user方法
	def perform_authentication(self, request):
        request.user
        
        
    # 4、再执行了user方法里面的self._authenticate()方法    
    @property
    def user(self):
        """
        Returns the user associated with the current request, as authenticated
        by the authentication classes provided to the request.
        """
        if not hasattr(self, '_user'):
            with wrap_attributeerrors():
                self._authenticate()
        return self._user
~~~



## 示例

### 重写原生方法BaseAuthtication.authenticate

创建一个认证类，继承BaseAuthentication，实现authenticate方法



util/auth.py

~~~ python
from rest_framework import exceptions
from rest_framework.authentication import BaseAuthentication
from rest_framework.exceptions import AuthenticationFailed
from api import models


class Authtication(BaseAuthentication):
    # 常用
    def authenticate(self,request):
        token = request._request.GET.get('token')
        token_obj = models.UserToken.objects.filter(token=token).fitst()
        if not token_obj:
            raise exceptions.AuthenticationFailed('用户认证失败')
            
        # 在rest framework内部将会将整个两个字段赋值给request，以供后续操作使用
        return (token_obj.user, token_obj)

	# 这个方法一般用不到
    def authenticate_header(self, request):
        pass
~~~



局部使用

~~~ python
from rest_framework.views import APIView
from apps.api.auth import Authtication

class UserInfoView(APIView):
    
    # 单独只对这个视图认证
    authentication_classes = [Authtication,]
    def get(self, request, *args, **kwargs):
        print(request.user)
        return HttpResponse('用户相关信息')
~~~



全局使用setting.py

~~~ python
REST_FRAMEWORK = {
    # 全局配置
    # 'DEFAULT_AUTHENTICATION_CLASSES': ['apps.api.utils.auth.FirstAuthtication', 'apps.api.utils.auth.Authtication'],
    'DEFAULT_AUTHENTICATION_CLASSES': ['apps.api.utils.auth.Authtication'],
    # 匿名，request.user = None
    'UNAUTHENTICATED_USER': None,
    # 匿名，request.auth = None
    'UNAUTHENTICATED_TOKEN': None,
}
~~~

