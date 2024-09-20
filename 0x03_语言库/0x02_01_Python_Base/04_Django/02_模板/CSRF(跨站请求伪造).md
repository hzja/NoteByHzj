# CSRF(跨站请求伪造)

## 攻击原理

XSS(跨站脚本)。XSS定义的主语是“脚本”，是一种跨站执行的脚本即javascript脚本，指网站上注入javascript脚本执行非法操作；

XSS攻击发生条件是可执行javascript脚本，一般站点中总有发表文章、留言等信息的表单，这种表单一般写入到数据库中后在某个页面展示。我们可在表单中直接编写javascript代码（<script>alert("哈哈哈哈，你被攻击了!");</script>）测试看是否可执行。如信息展示页面js代码可执行则XSS攻击成功；

CSRF跨站请求伪造也称为“One Click Attack”或者Session Riding，通常缩写为CSRF或者XSRF，是一种对网站的恶意利用。CSRF定义的主语是”请求“，一种跨站的伪造请求，指跨站伪造用户请求模拟用户操作；

CSRF攻击能成功是因黑客可完全伪造用户请求，而该请求中所有用户验证信息都存于cookie中，故黑客可在不知验证信息情况下直接利用用户cookie 通过安全验证。要抵御 CSRF关键在于请求中放入黑客不能伪造的信息且该信息不存于 cookie 中。可在 HTTP 请求中以参数形式加入一随机产生的 token并在服务器端建立一拦截器验证这 token，如请求中没token或token 内容不正确则认为可能是CSRF攻击而拒绝该请求。这种方法比检查 Referer安全，token 可在用户登陆后产生并放于session中，然后每次请求时把token 从 session 中拿出与请求中 token 比对；



## 攻击流程

![1580565134207-77699864-2b21-4e1b-8968-956179b7d214](D:\Note\python\Django\图片\1580565134207-77699864-2b21-4e1b-8968-956179b7d214.png)



### 自定义登录装饰器函数

~~~ python
def login_required(view_func):
    '''登录判断装饰器'''
    def wrapper(request, *view_args, **view_kwargs):
        # 判断用户是否登录
        if request.session.has_key('islogin'):
            # 用户已登录,调用对应的视图
            return view_func(request, *view_args, **view_kwargs)
        else:
            # 用户未登录,跳转到登录页
            return redirect('/login')
    return wrapper

# 其他需要验证的函数只需调用装饰器即可
@login_required
def change_pwd(request):
	...
    
@login_required
def change_pwd_action(request):
    ...
~~~



### 单独对视图禁用CSRF

~~~ python
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def get_data(request):
    return HttpResponse('成功')
~~~



## 防御原理

1. 渲染模板文件时在页面生成一个名字叫做 csrfmiddlewaretoken 的隐藏域。
2. 服务器交给浏览器保存一个名字为 csrftoken 的cookie信息。
3. 提交表单时，两个值都会发给服务器，服务器进行比对，如果一样，则csrf验证通过，否则失败。



## Form处理CSRF

~~~ html
<form action="" method="post" class="sidebar-form">
  {% csrf_token %}
	<div class="input-group">
  	<input type="text" name="asset_number" class="form-control" placeholder="Search...">
  		<span class="input-group-btn">
      	<button type="submit" name="search" id="search-btn" class="btn btn-flat"><i class="fa fa-search"></i>
      	</button>
    	</span>
	</div>
</form>
~~~



## Ajax处理CSRF

~~~ python
$.post({
        url: '/get_result/',
        data: {
              value0: $('#v1').val(),
              value1: $('#v2').val(),
              csrfmiddlewaretoken: '{{ csrf_token }}'
              },
              success: function (data) {
                    if (data) {
                    $('#v3').val(data);
                    swal("操作成功!", "成功算出结果","success");
              }
         })
~~~



## 总结

- 重要信息如金额、积分等，采用POST方式传递
- 启用CSRF中间件，默认启用



# 验证码

用户注册/登录页面，为防止暴力请求可加入验证码功能，如验证码错误则不需继续处理，可减轻业务服务器、数据库服务器压力。



## 手动实现验证码

1）安装包Pillow3.4.1。

~~~ shell
pip install Pillow==3.4.1
~~~

点击查看[PIL模块API](http://pillow.readthedocs.io/en/3.4.x/)，以下代码中用到了Image、ImageDraw、ImageFont对象及方法。



2）booktest/views.py文件中创建视图verify_code。

- 提示1：随机生成字符串后存入session用于后续判断;
- 提示2：视图返回mime-type为image/png;

~~~ python
from PIL import Image, ImageDraw, ImageFont
from django.utils.six import BytesIO

def verify_code(request):
    # 引入随机函数模块
    import random
    # 定义变量，用于画面的背景色、宽、高 RGB
    bgcolor = (random.randrange(20, 100), random.randrange(
        20, 100), 255)
    width = 100
    height = 25
    # 创建画面对象
    im = Image.new('RGB', (width, height), bgcolor)
    # 创建画笔对象
    draw = ImageDraw.Draw(im)
    # 调用画笔的point()函数绘制噪点
    for i in range(0, 100):
        xy = (random.randrange(0, width), random.randrange(0, height))
        fill = (random.randrange(0, 255), 255, random.randrange(0, 255))
        draw.point(xy, fill=fill)

    # 定义验证码的备选值
    str1 = 'ABCD123EFGHIJK456LMNOPQRS789TUVWXYZ0'
    # 随机选取4个值作为验证码
    rand_str = ''
    for i in range(0, 4):
        rand_str += str1[random.randrange(0, len(str1))]

    # 构造字体对象，ubuntu的字体路径为“/usr/share/fonts/truetype/freefont”
    font = ImageFont.truetype('FreeMono.ttf', 23)
    # 构造字体颜色
    fontcolor = (255, random.randrange(0, 255), random.randrange(0, 255))
    # 绘制4个字
    draw.text((5, 2), rand_str[0], font=font, fill=fontcolor)
    draw.text((25, 2), rand_str[1], font=font, fill=fontcolor)
    draw.text((50, 2), rand_str[2], font=font, fill=fontcolor)
    draw.text((75, 2), rand_str[3], font=font, fill=fontcolor)
    # 释放画笔
    del draw
    # 存入session，用于做进一步验证
    request.session['verifycode'] = rand_str
    # 内存文件操作
    buf = BytesIO()
    # 将图片保存在内存中，文件类型为png
    im.save(buf, 'png')
    # 将内存中的图片数据返回给客户端，MIME类型为图片png
    return HttpResponse(buf.getvalue(), 'image/png')
~~~



3）打开booktest/urls.py文件配置url

~~~ python
url(r'^verify_code/$', views.verify_code),
~~~



## 调用验证码

1）booktest/views.py文件中创建视图verify_show

~~~ python
def verify_show(request):
    return render(request,'booktest/verify_show.html')
~~~



2）打开booktest/urls.py文件配置url

~~~ python
url(r'^verify_show/$', views.verify_show),
~~~



3）templates/booktest/目录下创建verify_show.html

~~~ html
<html>
<head>
    <title>验证码</title>
</head>
<body>
<form method="post" action="/verify_yz/">
    {%csrf_token%}
    <input type="text" name="yzm">
    <img id="yzm" src="/verify_code/"/>
    <span id="change">看不清，换一个</span>
    <br>
    <input type="submit" value="提交">
</form>
</body>
</html>
~~~



4）运行服务器并在浏览器中输入如下网址

~~~ html
http://127.0.0.1:8000/verify_show/
~~~



5）浏览效果如下图：

![1580565846001-0e7f8426-9b0c-4e22-a76f-5e029fb2d665](D:\Note\python\Django\图片\1580565846001-0e7f8426-9b0c-4e22-a76f-5e029fb2d665.png)



## 验证

1）booktest/views.py文件中创建视图verify_yz

~~~ python
def verify_yz(request):
    yzm=request.POST.get('yzm')
    verifycode=request.session['verifycode']
    response=HttpResponse('no')
    if yzm == verifycode:
        response=HttpResponse('ok')
    return response
~~~



2）打开booktest/urls.py文件配置url

~~~ python
url(r'^verify_yz/$', views.verify_yz),
~~~



3）回到浏览器后刷新，在文本框中填写验证码后点击提交按钮![1580565846018-5ac60a30-6fb3-4209-8985-207cc1a2a731](D:\Note\python\Django\图片\1580565846018-5ac60a30-6fb3-4209-8985-207cc1a2a731.png)



4）浏览效果如下图

![1580565846014-522062a5-9632-45ff-88c7-254695e73d72](D:\Note\python\Django\图片\1580565846014-522062a5-9632-45ff-88c7-254695e73d72.png)