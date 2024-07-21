# xadmin

## 介绍

直接替换掉Django自带的admin系统，并提供了很多有用的东西；完全的可扩展的插件支持，基于Twitter Bootstrap的漂亮UI。



## 特点

- 完全替代Django admin
- 支持基于Twitter Bootstrap的UI主题
- 支持可扩展的插件
- 更好的过滤器，日期范围，数量范围等
- 内置xls，csv，xml和json格式数据导出
- 仪表板页面和小部件支持 （暂时用不到）
- 现场书签
- 完整的CURD方法（增删改查）



## 安装

1. 安装Xadmin

~~~ shell
pip install django-xadmin
~~~

2. 如果xadmin运行报错：卸载，换下面的安装方法，可能有点慢

~~~ shell
pip install git+git://github.com/sshwsfc/xadmin.git
~~~

3. 安装 Xadmin 的时候，需要注意 Django 的版本，Xadmin 在 [Github](https://github.com/sshwsfc/xadmin) 上的仓库 master 分支是支持 Django 1.9 版本的，假如是 Django 2.0 版本的，则需要从 Django2 分支安装

~~~ shell
pip install git+git://github.com/sshwsfc/xadmin.git@django2
~~~

4. 安装依赖项

~~~ shell
# 在工程项目根目录中执行一下命令
$ pip install -r requirements.txt


django-crispy-forms
django-import-export
django-reversion
django-formtools
future
httplib2
six
~~~



## 配置

在 settings.py 中注册相应库

~~~ python
# 添加以下代码
# Application definition
INSTALLED_APPS = [
    ...
    'xadmin',
    'crispy_forms',
    'reversion',
]
~~~



添加 url 规则

~~~ python
from django.urls import path
import xadmin

xadmin.autodiscover()
# version模块自动注册需要版本控制的 Model
from xadmin.plugins import xversion
xversion.register_models()


urlpatterns = [
    # 将原先的 admin.site.urls 替换为以下
    path('xadmin/', xadmin.site.urls),  # xadmin后台管理界面
]
~~~



## 登录

~~~ shell
$ python manage.py runserver
~~~

1. 登陆 [127.0.0.1:8000/admin](http://127.0.0.1:8000/admin/)

![1587017092549-f08244de-6c6e-495c-a92e-3592e735c2e1](D:\Note\python\Django\图片\1587017092549-f08244de-6c6e-495c-a92e-3592e735c2e1.webp)

2. 创建一个超级用户登录

~~~ shell
$ python manage.py createsuperuser
~~~

![1587017092705-7e647066-e416-4da9-84dd-fd4d4ded18f0](D:\Note\python\Django\图片\1587017092705-7e647066-e416-4da9-84dd-fd4d4ded18f0.webp)

## 简单定制

- 中文显示

~~~ python
# settings.py
...
LANGUAGE_CODE = 'zh-hans'
TIME_ZONE = 'Asia/Sahnghai'
...
~~~



- 注册 models

~~~ python
# 数据模型
# users/models.py
from django.db import models


class User(models.Model):
    nickname = models.CharField(max_length=50, blank=True)
    email = models.EmailField(('email address'), blank=True, unique=True)
    def __str__(self):
        return self.nickname
~~~



~~~ python
# users/admin.py
import xadmin
from .models import User


class UserAdmin(object):
    # 在列表中显示 nickname email 两个字段
    list_display = ['nickname', 'email']
xadmin.site.register(User, UserAdmin)
~~~



+ 修改左侧菜单栏中栏目

~~~ python
# users/apps.py
from django.apps import AppConfig


class UsersConfig(AppConfig):
    name = 'users'
    # verbose_name 为菜单栏目名字
    verbose_name = '用户'
~~~

~~~ python
# users/__init__.py
default_app_config='users.apps.UsersConfig'
~~~



- 修改页面 title footer header

~~~ python
# users/admin.py
import xadmin
from .models import User
from xadmin import views

# 增加文章表的功能
class UserAdmin(object):
    # 在主页显示以下字段
    list_display = ['nickname', 'email']
    
class GlobalSettings(object):
    # 修改logo
    site_title = '博客后台管理'
    site_footer = '浪子逐梦的博客'
    
class BaseSettings(object):
    # 添加修改后台主题功能
    enable_themes = True
    use_bootswatch = True
    
xadmin.site.register(User, UserAdmin)
xadmin.site.register(views.CommAdminView, GlobalSetting)
xadmin.site.register(views.BaseAdminView, BaseSettings)
~~~



- 添加搜索、过滤器

~~~python
# users/admin.py

# 增加文章表的功能
class UserAdmin(object):
    # 在主页显示以下字段
    list_display = ['nickname', 'email']
    # 添加检索框，可以检索以下字段内容
    search_fields = ['nickname']
    # 在主页就可以编辑一下子段
    list_editable= ['click_num','love_num']
    # 添加过滤器，可以过滤以下字段
    list_filter = ['nickname']
...
~~~



> 更多后台管理界面请访问 http://x.xuebingsi.com/