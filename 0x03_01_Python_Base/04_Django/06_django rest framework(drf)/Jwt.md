# jwt

# 1.jwt认证流程

![1582197606797-8abd593c-9886-49d1-b1ca-04e22cdf63cd](D:\Note\python\Django\图片\1582197606797-8abd593c-9886-49d1-b1ca-04e22cdf63cd.png)

在项目开发中，一般会按照上图所示的过程进行认证，即：用户登录成功之后，服务端给用户浏览器返回一个token，以后用户浏览器要携带token再去向服务端发送请求，服务端校验token的合法性，合法则给用户看数据，否则，返回一些错误信息。



## 传统token方式和jwt在认证方面有什么差异？

- **传统token方式（空间换时间）**

1. a. 用户登录成功后，服务端生成一个随机token给用户，并且在服务端(数据库或缓存)中保存一份token，以后用户再来访问时需携带token，服务端接收到token之后，去数据库或缓存中进行校验token的是否超时、是否合法。

- **jwt方式（时间换空间）**

1. a. 用户登录成功后，服务端通过jwt生成一个随机token给用户（服务端无需保留token），以后用户再来访问时需携带token，服务端接收到token之后，通过jwt对token进行校验是否超时、是否合法。



 **优势：**

- token只在前端保存，后端只负责校验。
- 内部集成了超时时间，后端可以根据时间进行校验是否超时。
- 由于内部存在hash256加密，所以用户不可以修改token，只要一修改就认证失败。



## token被盗怎么办？

1. 最笨的方法就是把token的过期时间弄短些。
2. token和用户设备指纹信息绑定，但是这样会影响到用户的一些体验，如果恶意APP装载到了你的手机里，此方法无用。
3. 使用https，这样很难被盗。话又说回来，连token都能被盗的用户说明他肯定乱点不该点的东西了。



## JWT 和Session Cookies 的不同

JWT 和 Session Cookies 都提供安全的用户身份验证，但是它们有以下几点不同

- **密码签名**

JWT 具有加密签名，而 Session Cookies 则没有。

- **JSON 是无状态的**

JWT 是`无状态`的，因为声明被存储在`客户端`，而不是服务端内存中。

身份验证可以在`本地`进行，而不是在请求必须通过服务器数据库或类似位置中进行。这意味着可以对用户进行多次身份验证，而无需与站点或应用程序的数据库进行通信，也无需在此过程中消耗大量资源。

- **可扩展性**

Session Cookies 是存储在服务器内存中，这就意味着如果网站或者应用很大的情况下会耗费大量的资源。由于 JWT 是无状态的，在许多情况下，它们可以节省服务器资源。因此 JWT 要比 Session Cookies 具有更强的`可扩展性`。

- **JWT 支持跨域认证**

Session Cookies 只能用在`单个节点的域`或者它的`子域`中有效。如果它们尝试通过第三个节点访问，就会被禁止。如果你希望自己的网站和其他站点建立安全连接时，这是一个问题。

使用 JWT 可以解决这个问题，使用 JWT 能够通过`多个节点`进行用户认证，也就是我们常说的`跨域认证`



## JWT 和Session Cookies 的选型

对于只需要登录用户并访问存储在站点数据库中的一些信息的中小型网站来说，Session Cookies通常就能满足。

如果你有企业级站点，应用程序或附近的站点，并且需要处理大量的请求，尤其是第三方或很多第三方（包括位于不同域的API），则 JWT 显然更适合。



# 2. jwt实现过程

## 2.1 加密

用户提交用户名和密码给服务端，如果登录成功，使用jwt创建一个token，并给用户返回，jwt的生成token格式如下，即：由 `.` 连接的三段字符串组成。

![1586268155105-a09861db-0f4b-4176-b631-6ef3e5f6d31b](D:\Note\python\Django\django rest framework(drf)\图片\1586268155105-a09861db-0f4b-4176-b631-6ef3e5f6d31b.png)

~~~ json
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwi
aWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c
~~~



生成规则如下：

- 第一段HEADER部分，固定包含**签名算法**和**token类型**，对此json进行base64url加密，这就是token的第一段。

~~~ json
{
  "alg": "HS256",
  "typ": "JWT"
}
~~~



- 第二段PAYLOAD部分，包含一些数据，对此json进行base64url加密，这就是token的第二段。

~~~ json
{
  "sub": "1234567890", 
  "name": "John Doe",
  "iat": 1516239022 # 时间戳
}
~~~



- 第三段SIGNATURE部分，把前两段的base密文通过`.`拼接起来，然后对其进行`HS256`加盐再加密，再然后对`HS256`密文进行base64url加密，最终得到token的第三段。

~~~ json
base64url(
    HMACSHA256(
      base64UrlEncode(header) + "." + base64UrlEncode(payload),
      your-256-bit-secret (秘钥加盐)
    )
)
~~~

最后将三段字符串通过 `.`拼接起来就生成了jwt的token。

**注意**：base64url加密是先做base64加密，然后再将 `-` 替代 `+` 及 `_` 替代 `/` 。



## 2.2 解密

一般在认证成功后，把jwt生成的token返回给用户，以后用户再次访问时候需要携带token，此时jwt需要对token进行`超时`及`合法性`校验。

- 第一步: 获取token，将token分割成 `header_segment`、`payload_segment`、`crypto_segment` 三部分

~~~ json
jwt_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6Ikpv
aG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c"
signing_input, crypto_segment = jwt_token.rsplit(b'.', 1)
header_segment, payload_segment = signing_input.split(b'.', 1)
~~~



- 第二步: 对第二段进行base64url解密，并获取payload信息，检测token是否已经超时?

~~~ json
{
  "sub": "1234567890",
  "name": "John Doe",
  "iat": 1516239022 # 超时时间
}
~~~



- 第三步: 把第1,2端拼接，再次执行`HS256`加密

~~~ json
第一步: 第1,2部分密文拼接起来
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwi
aWF0IjoxNTE2MjM5MDIyfQ

第二步:对前2部分密文进行HS256加密 + 加盐
密文 = base64解密(SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c)
如果相等,表示token未被修改过.(认证通过)
~~~



## 2.3 代码实现

基于Python的pyjwt模块创建jwt的token。

- 安装

~~~ python
pip3 install pyjwt
~~~



- 实现

~~~ python
import jwt
import datetime
from jwt import exceptions
SALT = 'iv%x6xo7l7_u9bf_u!9#g#m*)*=ej@bek5)(@u3kh*72+unjv='

def create_token():
    # 构造header
    headers = {
        'typ': 'jwt',
        'alg': 'HS256'
    }
    
    # 构造payload
    payload = {
        'user_id': 1, # 自定义用户ID
        'username': 'wupeiqi', # 自定义用户名
        'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=5) # 超时时间
    }
    # 构造signature即token
    token = jwt.encode(payload=payload, key=SALT, algorithm="HS256", headers=headers).decode('utf-8')
    return token

if __name__ == '__main__':
    token = create_token()
    print(token)
~~~



- 验证

~~~ python
import jwt
import datetime
from jwt import exceptions
def get_payload(token):
    """
    根据token获取payload
    :param token:
    :return:
    """
    try:
        # 从token中获取payload【不校验合法性】
        # unverified_payload = jwt.decode(token, None, False)
        # print(unverified_payload)
        # 从token中获取payload【校验合法性】
        verified_payload = jwt.decode(token, SALT, True)
        return verified_payload
    except exceptions.ExpiredSignatureError:
        print('token已失效')
    except jwt.DecodeError:
        print('token认证失败')
    except jwt.InvalidTokenError:
        print('非法的token')
if __name__ == '__main__':
    token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE1NzM1NTU1NzksInVzZXJuYW1lIjoid3VwZWlxaSIsInVzZXJfaWQiOjF9.xj-7qSts6Yg5Ui55-aUOHJS4KSaeLq5weXMui2IIEJU"
    payload = get_payload(token)
~~~



# 3.drf实战应用

## url.py 

~~~ python
from django.conf.urls import url
from django.contrib import admin
from api import views

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^api/pro/login/$', views.ProLoginView.as_view()),
    url(r'^api/pro/order/$', views.ProOrderView.as_view()),
]
~~~



## view.py

~~~ python
from api.extensions.auth import JwtQueryParamsAuthentication
from api.utils.jwt_auth import create_token
from rest_framework.views import APIView
from rest_framework.response import Response
from api import models

# 登陆视图
class ProLoginView(APIView):
    # 登陆界面不用做验证
    authentication_classes = []

    def post(self,request,*args,**kwargs):
        user = request.data.get('username')
        pwd = request.data.get('password')

        # 查库返回Queryset类型对象
        user_object = models.UserInfo.objects.filter(username=user,password=pwd).first()
        if not user_object:
            return Response({'code':1000,'error':'用户名或密码错误'})
        
		# 根据用户信息生成token
        token = create_token({'id':user_object.id,'name':user_object.username})

        return Response({'code': 1001, 'data': token})

    
# 订单视图
class ProOrderView(APIView):
    def get(self, request, *args, **kwargs):
        print(request.user)
        return Response('订单列表')
~~~



## extensions/auth.py

~~~ python
from rest_framework.authentication import BaseAuthentication
from rest_framework.exceptions import AuthenticationFailed
from django.conf import settings
import jwt
from jwt import exceptions


# 自定义基于BaseAuthentication的认证方法
class JwtQueryParamsAuthentication(BaseAuthentication):
    """
    用户需要在url中通过参数进行传输token，例如：
    http://www.pythonav.com?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE1NzM1NTU1NzksInVzZXJuYW1lIjoid3VwZWlxaSIsInVzZXJfaWQiOjF9.xj-7qSts6Yg5Ui55-aUOHJS4KSaeLq5weXMui2IIEJU
    """
    def authenticate(self, request):
        # 获取token并判断token的合法性，query_params可以获取url中的参数，默认是Queryset对象
        token = request.query_params.get('token')
        salt = settings.SECRET_KEY

        # 1.切割
        # 2.解密第二段/判断过期
        # 3.验证第三段合法性

        try:
            # 解密token
            payload = jwt.decode(token, salt, True)
        except exceptions.ExpiredSignatureError:
            raise AuthenticationFailed({'code':1003,'error':"token已失效"})
        except jwt.DecodeError:
            raise AuthenticationFailed({'code':1004,'error':"token认证失败"})
        except jwt.InvalidTokenError:
            raise AuthenticationFailed({'code':1005,'error':"非法的token"})

        # 三种操作
        # 1.抛出异常，后续不再执行
        # 2.return一个元祖（1，2），认证通过；并将其分别赋值给request.user和request.auth
        # 3.None
        return(payload,token)
    
    
class JwtAuthorizationAuthentication(BaseAuthentication):
    """
    用户需要通过请求头的方式来进行传输token，例如：
    Authorization:jwt eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE1NzM1NTU1NzksInVzZXJuYW1lIjoid3VwZWlxaSIsInVzZXJfaWQiOjF9.xj-7qSts6Yg5Ui55-aUOHJS4KSaeLq5weXMui2IIEJU
    """
    def authenticate(self, request):

        # 非登录页面需要校验token
        authorization = request.META.get('HTTP_AUTHORIZATION', '')
        auth = authorization.split()
        if not auth:
            raise exceptions.AuthenticationFailed({'error': '未获取到Authorization请求头', 'status': False})
        if auth[0].lower() != 'jwt':
            raise exceptions.AuthenticationFailed({'error': 'Authorization请求头中认证方式错误', 'status': False})

        if len(auth) == 1:
            raise exceptions.AuthenticationFailed({'error': "非法Authorization请求头", 'status': False})
        elif len(auth) > 2:
            raise exceptions.AuthenticationFailed({'error': "非法Authorization请求头", 'status': False})

        token = auth[1]
        result = parse_payload(token)
        if not result['status']:
            raise exceptions.AuthenticationFailed(result)

        # 如果想要request.user等于用户对象，此处可以根据payload去数据库中获取用户对象。
        return (result, token)
~~~



## utils/jwt_auth.py

~~~ python
import jwt
import datetime
from django.conf import settings

# 创建生成的token方法
def create_token(payload, timeout):

    salt = settings.SECRET_KEY
    
    # 构造header
    headers = {
        'typ': 'jwt',
        'alg': 'HS256'
    }
    
    # 构造payload
    payload['exp'] = {datetime.datetime.utcnow() + datetime.timedelta(minutes=timeout)}
    
    # 构造signature即token
    token = jwt.encode(payload=payload, key=salt, algorithm="HS256", headers=headers).decode('utf-8')

    return token
~~~



## setting.py

~~~ python
# 全局使用
REST_FRAMEWORK = {
    "DEFAULT_AUTHENTICATION_CLASSES":['api.extensions.auth.JwtQueryParamsAuthentication',]
}
~~~



# 4.示例测试

## url传参

~~~ python
GET /something/ HTTP/1.1
Host: 127.0.0.1
~~~

![1586176225595-78a38755-c34c-4950-ab4e-132f38b1d149](D:\Note\python\Django\django rest framework(drf)\图片\1586176225595-78a38755-c34c-4950-ab4e-132f38b1d149.png)



## Authorization 请求头传参

~~~ python
GET /something/ HTTP/1.1
Host: 127.0.0.1
Authorization: JWT eyJhbGciOiAiSFMyNTYiLCAidHlwIj
~~~

![1586231382042-569300d4-941b-4cfb-8a8d-404ec6fb22ac](D:\Note\python\Django\django rest framework(drf)\图片\1586231382042-569300d4-941b-4cfb-8a8d-404ec6fb22ac.png)