# 登录界面demo

## 一、使用原生HTML页面

删除原来的login.html文件内容，写入下面代码：

~~~ html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>登录</title>
</head>
<body>
    <div style="margin: 15% 40%;">
        <h1>欢迎登录！</h1>
       <form action="/login/" method="post">
            <p>
                <label for="id_username">用户名：</label>
                <input type="text" id="id_username" name="username" placeholder="用户名" autofocus required />
            </p>
            <p>
                <label for="id_password">密码：</label>
                <input type="password" id="id_password" placeholder="密码" name="password" required >
            </p>
            <input type="submit" value="确定">
        </form>
    </div>
</body>
</html>
~~~

简单解释一下：

- form标签主要确定目的地url和发送方法；
- p标签将各个输入框分行；
- label标签为每个输入框提供一个前导提示，还有助于触屏使用；
- placeholder属性为输入框提供占位符；
- autofocus属性为用户名输入框自动聚焦
- required表示该输入框必须填写
- passowrd类型的input标签不会显示明文密码

以上功能都是HTML5原生提供的，可以减少你大量的验证和JS代码，更详细的用法，请自行学习。

**特别声明：所有前端的验证和安全机制都是不可信的，恶意分子完全可以脱离浏览器伪造请求数据！**

启动服务器，访问`http://127.0.0.1:8000/login/`，可以看到如下图的页面：

![1585736378570-40684c45-0d34-4835-a1d9-bbd984fa26f8](D:\Note\python\Django\图片\1585736378570-40684c45-0d34-4835-a1d9-bbd984fa26f8.png)

## 二、引入Bootstrap 4

如果你的实际项目真的使用上面的那个页面外观，妥妥的被老板打死。代码虽然简单，速度虽然快，但没有CSS和JS，样子真的令人无法接受，在颜值即正义的年代，就是错误。

然而，大多数使用Django的人都不具备多高的前端水平，通常也没有专业的前端工程师配合，自己写的CSS和JS却又往往惨不忍睹。怎么办？没关系，我们有现成的开源前端CSS框架！Bootstrap4就是最好的CSS框架之一！

想要在HTML页面中使用Bootstrap4，最方便的方法就是使用国内外的免费CDN（如果app的使用环境不可以使用外部网络，也可以使用内部的CDN，或者静态文件）。

这里推荐BootCDN：https://www.bootcdn.cn/，速度比较快，有大量的不同版本的CDN。

![1585736476590-c9c35443-b8e2-4c89-9be4-c0c03b0dcec1](D:\Note\python\Django\图片\1585736476590-c9c35443-b8e2-4c89-9be4-c0c03b0dcec1.png)

这里直接给出HTML标签，复制粘贴即可：

~~~ html
CSS：
<link href="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet">
以及JS：
<script src="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/js/bootstrap.min.js"></script>
~~~

由于Bootstrap依赖JQuery，所以我们也需要使用CDN引用JQuery 3.3.1:

~~~ html
<script src="https://cdn.bootcss.com/jquery/3.3.1/jquery.js"></script>
~~~

另外，从Bootstrap4开始，额外需要popper.js的支持，依旧使用CDN的方式引入:

~~~ html
<script src="https://cdn.bootcss.com/popper.js/1.15.0/umd/popper.js"></script>
~~~

下面，我们就可以创建一个漂亮美观的登录页面了，具体代码如下：

~~~ html
<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <!-- 上述meta标签*必须*放在最前面，任何其他内容都*必须*跟随其后！ -->
    <!-- Bootstrap CSS -->
    <link href="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet">
    <title>登录</title>
  </head>
  <body>
    <div class="container">
            <div class="col">
              <form class="form-login" action="/login/" method="post">
                  <h3 class="text-center">欢迎登录</h3>
                  <div class="form-group">
                    <label for="id_username">用户名：</label>
                    <input type="text" name='username' class="form-control" id="id_username" placeholder="Username" autofocus required>
                  </div>
                  <div class="form-group">
                    <label for="id_password">密码：</label>
                    <input type="password" name='password' class="form-control" id="id_password" placeholder="Password" required>
                  </div>
                <div>
                  <a href="/register/" class="text-success "><ins>新用户注册</ins></a>
                  <button type="submit" class="btn btn-primary float-right">登录</button>
                </div>
              </form>
            </div>
    </div> <!-- /container -->

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <!-- 以下三者的引用顺序是固定的 -->
    <script src="https://cdn.bootcss.com/jquery/3.3.1/jquery.js"></script>
    <script src="https://cdn.bootcss.com/popper.js/1.15.0/umd/popper.js"></script>
    <script src="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/js/bootstrap.min.js"></script>

  </body>
</html>
~~~

访问一下login页面，看起来如下：

![1585737775740-91bf711a-8e76-43e6-b732-57c68e453bd1](D:\Note\python\Django\图片\1585737775740-91bf711a-8e76-43e6-b732-57c68e453bd1.png)



## 三、添加静态文件

然而，上面的登录页面在宽度上依然不太合适，背景也单调，所以一般我们会写一些CSS代码，同时使用背景图片。

在工程根目录下的login目录下，新建一个static目录，再到static目录里创建一个login目录，这种目录的创建方式和模板文件templates的创建方式都是一样的思维，也就是让重用app变得可能，并且不和其它的app发生文件路径和名称上的冲突。

继续在`/login/static/login`目录下创建一个css和一个image目录，css中添加我们为登录视图写的css文件，这里是`login.css`，image目录中，拷贝进来你想要的背景图片，这里是`bg.jpg`。最终目录结构如下：

![1585737775790-2fe9d661-db6d-4157-948d-97e12f558b15](D:\Note\python\Django\图片\1585737775790-2fe9d661-db6d-4157-948d-97e12f558b15.png)

下面我们修改一下login.html的代码，主要是引入了login.css文件，注意最开头的`{% load static %}`，表示我们要加载静态文件。

~~~ html
{% load static %}
<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <!-- 上述meta标签*必须*放在最前面，任何其他内容都*必须*跟随其后！ -->
    <!-- Bootstrap CSS -->
    <link href="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet">
    <link href="{% static 'login/css/login.css' %}" rel="stylesheet"/>
    <title>登录</title>
  </head>
  <body>
    <div class="container">
            <div class="col">
              <form class="form-login" action="/login/" method="post">               
                {% csrf_token %}
                  <h3 class="text-center">欢迎登录</h3>
                  <div class="form-group">
                    <label for="id_username">用户名：</label>
                    <input type="text" name='username' class="form-control" id="id_username" placeholder="Username" autofocus required>
                  </div>
                  <div class="form-group">
                    <label for="id_password">密码：</label>
                    <input type="password" name='password' class="form-control" id="id_password" placeholder="Password" required>
                  </div>
                  <div>
                  <a href="/register/" class="text-success "><ins>新用户注册</ins></a>
                  <button type="submit" class="btn btn-primary float-right">登录</button>
                  </div>
              </form>
            </div>
    </div> <!-- /container -->

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <!-- 以下三者的引用顺序是固定的 -->
    <script src="https://cdn.bootcss.com/jquery/3.3.1/jquery.js"></script>
    <script src="https://cdn.bootcss.com/popper.js/1.15.0/umd/popper.js"></script>
    <script src="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/js/bootstrap.min.js"></script>

  </body>
</html>
~~~

而login.css文件的代码如下，注意其中背景图片bg.jpg的引用方式：

~~~ css
body {
  height: 100%;
  background-image: url('../image/bg.jpg');
}
.form-login {
  width: 100%;
  max-width: 330px;
  padding: 15px;
  margin: 0 auto;
}
.form-login{
  margin-top:80px;
  font-weight: 400;
}
.form-login .form-control {
  position: relative;
  box-sizing: border-box;
  height: auto;
  padding: 10px;
  font-size: 16px;

}
.form-login .form-control:focus {
  z-index: 2;
}
.form-login input[type="text"] {
  margin-bottom: -1px;
  border-bottom-right-radius: 0;
  border-bottom-left-radius: 0;
}
.form-login input[type="password"] {
  margin-bottom: 10px;
  border-top-left-radius: 0;
  border-top-right-radius: 0;
}
form a{
  display: inline-block;
  margin-top:25px;
  font-size: 12px;
  line-height: 10px;
}
~~~

好了，现在可以重启服务器，刷新登录页面，看看效果了：

![1585737836340-c8fa9a82-6f76-43dc-9b78-5065fc451f57](D:\Note\python\Django\图片\1585737836340-c8fa9a82-6f76-43dc-9b78-5065fc451f57.png)



## 四、接收前端请求

~~~ python
from django.shortcuts import render
from django.shortcuts import redirect
from . import models


def index(request):
    pass
    return render(request, 'login/index.html')


def login(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        message = '请检查填写的内容！'
        if username.strip() and password:
            # 用户名字符合法性验证
            # 密码长度验证
            # 更多的其它验证.....
            try:
                user = models.User.objects.get(name=username)
            except :
                message = '用户不存在！'
                return render(request, 'login/login.html', {'message': message})

            if user.password == password:
                print(username, password)
                return redirect('/index/')
            else:
                message = '密码不正确！'
                return render(request, 'login/login.html', {'message': message})
        else:
            return render(request, 'login/login.html', {'message': message})
    return render(request, 'login/login.html')


def register(request):
    pass
    return render(request, 'login/register.html')


def logout(request):
    pass
    return redirect("/login/")
~~~

这里增加了message变量，用于保存提示信息。当有错误信息的时候，将错误信息打包成一个字典，然后作为第三个参数提供给render方法。这个数据字典在渲染模板的时候会传递到模板里供你调用。

为了在前端页面显示信息，还需要对`login.html`进行修改：

~~~ html
<form class="form-login" action="/login/" method="post">
    {% if message %}
    <div class="alert alert-warning">{{ message }}</div>
    {% endif %}
    {% csrf_token %}
    <h3 class="text-center">欢迎登录</h3>
    <div class="form-group">
        <label for="id_username">用户名：</label>
        <input type="text" name='username' class="form-control" id="id_username" placeholder="Username" autofocus required>
    </div>
    <div class="form-group">
        <label for="id_password">密码：</label>
        <input type="password" name='password' class="form-control" id="id_password" placeholder="Password" required>
    </div>
    <div>
        <a href="/register/" class="text-success " ><ins>新用户注册</ins></a>
        <button type="submit" class="btn btn-primary float-right">登录</button>
    </div>
</form>
~~~

Django的模板语言`{% if xxx %}{% endif %}`非常类似Python的if语句，也可以添加`{% else %}`分句。例子中，通过判断message变量是否为空，也就是是否有错误提示信息，如果有，就显示出来！这里使用了Bootstrap的警示信息类alert，你也可以自定义CSS或者JS。

![1585826998497-46951002-bd50-400a-9e50-0f1d467dd0d6](D:\Note\python\Django\图片\1585826998497-46951002-bd50-400a-9e50-0f1d467dd0d6.png)