# 表单 form

前面都是手工在HTML文件中编写表单form元素，然后在views.py的视图函数中接收表单中的用户数据，再编写验证代码进行验证，最后使用ORM进行数据库的增删改查。这样费时费力，整个过程比较复杂，而且有可能写得不太恰当，数据验证也比较麻烦。设想一下，如果我们的表单拥有几十上百个数据字段，有不同的数据特点，如果也使用手工的方式，其效率和正确性都将无法得到保障。



## 作用

- 前端和后端都要校验
- 前端校验的目的：减少后台代码连接数据库的压力
- 用forms可以同时完成前端和后端同时校验且减少代码量



Django会处理涉及表单的三个不同部分：

1. 准备并重组数据，以便下一步的渲染
2. 为数据创建HTML 表单
3. 接收并处理客户端提交的表单及数据



## Form

Form比较灵活需要自己定义各个要验证的字段

~~~ python
from django import forms
from captcha.fields import CaptchaField


class UserForm(forms.Form):
    username = forms.CharField(label='用户名', max_length=128, widget=forms.TextInput(attrs={'class': 'form-control',
                                                                                          'placeholder': 'username',
                                                                                          'autofocus': ''}))
    password = forms.CharField(label='密码', max_length=256, widget=forms.PasswordInput(attrs={'class': 'form-control',
                                                                                          'placeholder': 'password'}))
    captcha = CaptchaField(label='验证码')
~~~



~~~  python
from . import forms


def login(request):
    if request.session.get('is_login', None):  # 不允许重复登录
        return redirect(reverse('social:index'))
    if request.method == 'POST':
        login_form = forms.UserForm(request.POST)
        message = '请检查填写的内容！'
        if login_form.is_valid():
            username = login_form.cleaned_data.get('username')
            password = login_form.cleaned_data.get('password')

            try:
                user = UserProfile.objects.get(username=username)
            except :
                message = '用户不存在！'
                return render(request, 'login/login.html', locals())

            if not user.has_confirmed:
                message = '该用户还未经过邮件确认！'
                return render(request, 'login/login.html', locals())

            if user.password == hash_code(password):
                request.session['is_login'] = True
                request.session['user_id'] = user.id
                request.session['user_name'] = user.username
                return redirect(reverse('social:index'))
            else:
                message = '密码不正确！'
                return render(request, 'login/login.html', locals())
        else:
            return render(request, 'login/login.html', locals())

    # 对于非POST方法发送数据时，比如GET方法请求页面，返回空的表单，让用户可以填入数据
    login_form = forms.UserForm()
    # 这里使用了一个小技巧，Python内置了一个locals()函数，它返回当前所有的本地变量字典，我们可以偷懒的将这作
    # 为render函数的数据字典参数值，就不用费劲去构造一个形如{'message':message, 'login_form':login_form}的字典了。
    return render(request, 'login/login.html', locals())
~~~



<kbd>login.html</kbd>

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
      <link href="{% static 'login/css/login.css' %}" rel="stylesheet" />

    <title>登录</title>
  </head>
  <body>

  <div class="container">
            <div class="col">
              <form class="form-login" action="{%url 'user:login'%}" method="post">

                  {% if login_form.captcha.errors %}
                    <div class="alert alert-warning">{{ login_form.captcha.errors }}</div>
                  {% elif message %}
                      <div class="alert alert-warning">{{ message }}</div>
                  {% endif %}

                  {% csrf_token %}

                  <h3 class="text-center">欢迎登录</h3>

                  <div class="form-group">
                  # 使用form表单
                      {{ login_form.username.label_tag }}
                      {{ login_form.username }}
                  </div>
                  <div class="form-group">
                      {{ login_form.password.label_tag }}
                      {{ login_form.password }}
                  </div>
                  <div class="form-group">
                      {{ login_form.captcha.label_tag }}
                      {{ login_form.captcha }}
                  </div>

                  <div>
                  <a href="{%url 'user:register'%}" class="text-success"><ins>新用户注册</ins></a>
                  <button type="submit" class="btn btn-primary float-right">登录</button>
                </div>
              </form>
            </div>
  </div> <!-- /container -->

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    {#    以下三者的引用顺序是固定的#}
    <script src="https://cdn.bootcss.com/jquery/3.3.1/jquery.min.js"></script>
    <script src="https://cdn.bootcss.com/popper.js/1.15.0/umd/popper.min.js"></script>
    <script src="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/js/bootstrap.min.js"></script>
  </body>
</html>
~~~

**注意事项：**

- 一个{{ login_form }}就直接完成了表单内容的生成工作！login_form这个名称来自你在视图函数中生成的form实例的变量名！
- 但是，它不会生成<form>...</form>标签，这个要自己写；
- 使用POST的方法时，必须添加{% csrf_token %}标签，用于处理csrf安全机制；
- Django自动为每个input元素设置了一个id名称，对应label的for参数
- 注册链接和登录按钮需要自己写，Django不会帮你生成！



**补充：**实际上除了通过{{ login_form }}简单地将表单渲染到HTML页面中了，还有下面几种方式：

- {{ login_form.as_table }} 将表单渲染成一个表格元素，每个输入框作为一个<tr>标签
- {{ login_form.as_p }} 将表单的每个输入框包裹在一个<p>标签内
- {{ login_form.as_ul }} 将表单渲染成一个列表元素，每个输入框作为一个<li>标签



![1587212889544-52ea34eb-75a8-47f4-bc20-08f8fa791c7a](D:\Note\python\Django\图片\1587212889544-52ea34eb-75a8-47f4-bc20-08f8fa791c7a.png)



## ModelForm

ModelForm是跟Model有关的，model的字段可以直接引用过来，既用于数据库操作，也可用于用户请求的验证

~~~ python
forms.py
from django.forms import ModelForm


# 使用ModelForm写一个注册模块
class RegisterForm(ModelForm):
    # 方法一：直接导入想要的表字段，会直接映射到页面
    class Meta:
        model = UserProfile
        fields = ['username','name_cn','password','email','phone']
        widgets = {
            'username': forms.TextInput(attrs={'class': 'form-control'}),
            'name_cn': forms.TextInput(attrs={'class': 'form-control'}),
            'password': forms.PasswordInput(attrs={'class': 'form-control'}),
            'email': forms.EmailInput(attrs={'class': ' form-control'}),
            'phone': forms.TextInput(attrs={'class': 'form-control'}),
        }

    # 方法二：导入全部，然后删掉不想要的
    class Meta:
        model = UserProfile
        fields = '__all__'
        exclude = ['first_name', 'date_joined', 'last_name']
        
        
    # 另外还可以定义一些指定字段的单独验证
    def clean_username(self):
        username = self.cleaned_data.get('username')
        result = re.match(r'[a-zA-Z]\w{8,}', username)
        if not result:
            raise ValidationError('用户名必须字母开头')
        return username
    
    def clean_password(self):
        password = self.cleaned_data.get('password')
        if len(password) < 6  :
            raise forms.ValidationError(u'密码必须大于6位/密码为空')
        return password
~~~

~~~ python 
views.py
# 用户注册视图
def user_register(request):
    if request.method == 'GET':
        return render(request, 'user/register.html')
    else:
        # 调用form表单去接受数据
        rform = RegisterForm(request.POST)
        
        # 判断用户是否提交了正确的数据，返回值为True
        if rform.is_valid():
            # cleaned_data以字典保存了用户输入的信息
            username = rform.cleaned_data.get('username')
            email = rform.cleaned_data.get('email')
            mobile = rform.cleaned_data.get('mobile')
            password = rform.cleaned_data.get('password')

            # 判断用户名和手机号是否已经存在
            # 不存在
            if not UserProfile.objects.filter(Q(username=username) | Q(mobile=mobile)).exists():
                # 密码加密
                password = make_password(password)
                # 注册到数据库
                user = UserProfile.objects.create(username=username, password=password, email=email, mobile=mobile)
                if user:
                    return HttpResponse('注册成功')
            # 存在
            else:
                return render(request, 'user/register.html', context={'msg': '用户名或者手机号码已经存在！'})
        print(rform.is_valid())
        return render(request, 'user/register.html', context={'msg': '注册失败，重新填写！'})
~~~

