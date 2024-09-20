# HttpResponse

## HttpResponse

视图在接收请求并处理后，必须返回HttpResponse对象或子对象。在django.http模块中定义了HttpResponse对象的API。HttpRequest对象由Django创建，HttpResponse对象由开发人员创建。

~~~ python
class CBV_View(View):
    def get(self, request, *args, **kwargs):
        return HttpResponse("GET")
~~~



## JsonResponse

在浏览器中使用javascript发起ajax请求时，返回json格式的数据，此处以jquery的get()方法为例。类JsonResponse继承自HttpResponse对象，被定义在django.http模块中，创建对象时接收字典作为参数

> JsonResponse对象的content-type为'application/json'。

~~~ python
class CBV_View(View):
    def post(self, request, *args, **kwargs):
        message = {
            "INFO": "账号或密码为空",
            "rtn": "-1",
        return JsonResponse(message)
~~~



## Response

drf自己封装的方法，会有一个简单的web接口界面

与普通 HttpResponse 对象不同，您不会使用渲染的内容实例化 Response 对象。相反，您传递的是未渲染的数据，可能包含任何 Python 对象。

由于 Response 类使用的渲染器不能处理复杂的数据类型（比如 Django 的模型实例），所以需要在创建 Response 对象之前将数据序列化为基本的数据类型。

你可以使用 REST framework 的 Serializer  类来执行序列化的操作，也可以用自己的方式来序列化。![image](D:\Note\python\Django\图片\image.png)

~~~ python
from rest_framework.response import Response

class Personal_Assets_View(APIView):
    def get(self, request, *args, **kwargs):
        username = request.GET.get('username')
        if not username:
            context['msg'] = '请求失败'
            context['code'] = 400
            context['data'] = '请输入用户名'
            return Response(context)
~~~



## render

结合一个给定的模板和一个给定的上下文字典, 并返回一个渲染后的HttpResponse对象

~~~ python
from django.shortcuts import render
 
def my_view(request):
    # 视图代码写在这里
    return render(request, "myapp/index.html", {"foo": "bar"})
~~~



## 重定向HttpResponseRedirect

HttpResponseRedirect对象实现重定向功能，这个类继承自HttpResponse，被定义在django.http模块中，返回的状态码为302

~~~ python
from django.http import HttpResponseRedirect

# 定义重定义向视图，转向首页
def index(request):
    return HttpResponseRedirect('/login', {'h1': 'hello'})
~~~



## 重定向简写redirect

在django.shortcuts模块中为重定向类提供了简写函数redirect

~~~ python
from django.shortcuts import redirect

def index(request):
    return redirect('/login', {'h1': 'hello'})
~~~



## reverse

reverse方法的作用是对已命名的URL进行反向解析，还传递相应的参数（args或带key的参数kargs)。



~~~ python
from django.urls import reverse
from django.shortcuts import redirect

def my_view(request):
    ...
    return redirect(reverse('user:login', kwargs={'app_label': 'auth'}))
~~~

