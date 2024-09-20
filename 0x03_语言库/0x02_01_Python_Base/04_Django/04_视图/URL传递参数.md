# URL传递函数

## 1.无参数情况

> 配置URL及其视图如下：
>
> (r’^hello/$’, hello)

~~~ python
def hello(request):
    return HttpResponse("Hello World")
~~~

> 访问http://127.0.0.1:8000/hello，输出结果为“Hello World”



## 2.传递一个参数

> 配置URL及其视图如下,URL中通过正则指定一个参数：
>
> (r’^plist/(.+)/$’, helloParam）

~~~ python
def helloParam(request，param1):
    return HttpResponse("The param is : " + param1)
~~~

> 访问http://127.0.0.1:8000/plist/china，输出结果为”The param is : china”



## 3.传递多个参数

> 参照第二种情况，以传递两个参数为例，配置URL及其视图如下,URL中通过正则指定两个参数：
>
> (r’^plist/p1(\w+)p2(.+)/$’, helloParams）

~~~ python
def helloParams(request，param1,param2):
    return HttpResponse("p1 = " + param1 + "; p2 = " + param2)
~~~

> 访问http://127.0.0.1:8000/plist/p1chinap22012/
>
> 输出为”p1 = china; p2 = 2012″
>
> 从这里可以看出，视图的参数是根据URL的正则式，按顺序匹配并自动赋值的。虽然这样可以实现任意多个参数的传递，但是却不够灵活，URL看起来很混乱，而且由于是正则匹配，有些情况下容易出错。



## 4.通过传统的“？”传递参数

> 例如，http://127.0.0.1:8000/plist/?p1=china&p2=2012，url中‘?’之后表示传递的参数，这里传递了p1和p2两个参数。
>
> 通过这样的方式传递参数，就不会出现因为正则匹配错误而导致的问题了。在Django中，此类参数的解析是通过request.GET.get方法获取的。
>
> 配置URL及其视图如下：
>
> (r’^plist/$’, helloParams1）

~~~ python
def helloParams(request):
    p1 = request.GET.get('p1')
    p2 = request.GET.get('p2')
    return HttpResponse("p1 = " + p1 + "; p2 = " + p2)
~~~

> 输出结果为”p1 = china; p2 = 2012″



## 5.使用关键字参数而非位置参数

> 在 Python 正则表达式中，命名的正则表达式组的语法是 (?Ppattern) ，这里 name 是组的名字，而
>
> pattern 是匹配的某个模式。

下面是一个使用无名组的 URLconf 的例子:

~~~ python
from django.conf.urls.defaults import *
from mysite import views

urlpatterns = patterns('',
	(r'^articles/(\d{4})/$', views.year_archive),
	(r'^articles/(\d{4})/(\d{2})/$', views.month_archive),
)
~~~



下面是相同的 URLconf，使用命名组进行了重写:

~~~ python
from django.conf.urls.defaults import *
from mysite import views

urlpatterns = patterns('',
    (r'^articles/(?P<year>\d{4})/$', views.year_archive),
    (r'^articles/(?P<year>\d{4})/(?P<month>\d{2})/$', views.month_archive),
)
~~~

> 这段代码和前面的功能完全一样，只有一个细微的差别： 取的值是以关键字参数的方式而不是以位置参数的方
>
> 式传递给视图函数的。



例如，如果不带命名组，请求 /articles/2006/03/ 将会等同于这样的函数调用：

~~~ python
month_archive(request, '2006', '03')
~~~

而带命名组，同样的请求就会变成这样的函数调用：

~~~ python
month_archive(request, year='2006', month='03')
~~~



> 使用命名组可让URLconfs更清晰减少搞混参数次序的潜在BUG，还可函数定义中对参数重新排序；
>
> 接上面这例子若想修改URL把月份放到年份前而不用命名组就须修改视图month_archive参数次序， 若使用命名组修改URL里提取参数次序对视图没影响；
>
> 而命名组代价就是失去简洁性： 一些开发者觉得命名组语法丑陋和冗余，但命名组另一好处是可读性强；
>
> 需**注意**若URLconf中用命名组则命名组和非命名组不能同时存于同一URLconf模式中；若这样做Django不抛出任何错误，但可能URL并没有预想那样匹配正确；



具体地，以下是URLconf解释器有关正则表达式中命名组和 非命名组所遵循的算法:

+ 如果有任何命名的组，Django会忽略非命名组而直接使用命名组;
+ 否则，Django会把所有非命名组以位置参数的形式传递;
+ 在以上的两种情况，Django同时会以关键字参数的方式传递一些额外参数;