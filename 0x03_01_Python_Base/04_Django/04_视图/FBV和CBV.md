# FBV和CBV

> 写一django项目通常用CBV如按rest规范写一接口时CBV适用性比FBV强

<kbd>views.py</kbd>

~~~ python
from django.shortcuts import render
from django.views import View
from app import models
from django.http import JsonResponse, HttpResponse


def FBV_View(request):
    if request.method == 'GET':
        return HttpResponse("GET")
    if request.method == 'POST':
        return HttpResponse("POST")


class CBV_View(View):
    def get(self, request, *args, **kwargs):
        return HttpResponse("GET")

    def post(self, request, *args, **kwargs):
        message = {
            "INFO": "",
            "rtn": "-1",
        }

        username = request.POST.get('username')
        password = request.POST.get('password')
        if username and password:
            user = models.User.objects.get(username=username)
            if user.password == password:
                message["INFO"] = "认证成功"
                message["rtn"] = "0"
                return JsonResponse(message)
            else:
                message["INFO"] = "认证失败"
                message["rtn"] = "-1"
                return JsonResponse(message)
        else:
            message["INFO"] = "账号或密码为空"
            message["rtn"] = "-2"
        return JsonResponse(message)
~~~



<kbd>urls.py</kbd>

~~~ python
from django.urls import path
from app import views

urlpatterns = [
    path(r'fbv/', views.FBV_View),
    path(r'cbv/', views.CBV_View.as_view()),
]
~~~



>  **FBV和CBV的区别**
>
> ​		FBV顾名思义是函数处理请求但代码冗余多非面向对象编程；
>
> ​		CBV则用类中不同方法处理请求，迎合了python推崇的面向对象编程思想；
>
> **CBV优点：**
>
> 1. 提高了代码的复用性，可以使用面向对象的计算，比如Mixin(多继承)
>   1. 可以用不同的函数针对不同的http请求方法处理，而不是通过过多的if判断，提高了代码的可读性



[**URL前r的意思**](https://www.cnblogs.com/xiangxiaolin/p/11181113.html)

> 正则表达式字符串开头字母“r”其告诉Python此乃原始字符串不需处理反斜杠（转义字符）；
>
> 而普通Python字符串中反斜杠用于特殊字符转义，如n转义成一换行符当用r标它为一原始字符串后Python不再视反斜杠为转义字符，即“/n”是两字符串：“/”和“n”；
>
> 由于反斜杠在Python代码和正则表达式中有冲突，因此建议Python定义正则表达式时都使用原始字符串，即前面加"r"；