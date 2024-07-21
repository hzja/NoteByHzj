# Django基础

## 查看版本号

~~~ python
python -m django --version
~~~

通过以上命令查看Django的版本.



## 创建项目

到想创建项目的文件夹中，打开命令行，输入以下命令创建项目

~~~ python
django-admin startproject '网站名'
~~~

该命令行创建了以下Django项目：

~~~ python
mysite/
    manage.py
    mysite/
        __init__.py
        settings.py
        urls.py
        asgi.py
        wsgi.py
~~~

这些目录和文件的用处是：

- 最外层的 `mysite/` 根目录只是你项目的容器， 根目录名称对 Django 没有影响，你可以将它重命名为任何你喜欢的名称。
- `manage.py`: 一个让你用各种方式管理 Django 项目的命令行工具
- 里面一层的 `mysite/` 目录包含你的项目，它是一个纯 Python 包。它的名字就是当你引用它内部任何东西时需要用到的 Python 包名。 (比如 `mysite.urls`).
- `mysite/__init__.py`：一个空文件，告诉 Python 这个目录应该被认为是一个 Python 包
- `mysite/settings.py`：Django 项目的配置文件
- `mysite/urls.py`：Django 项目的 URL 声明，就像你网站的“目录"
- `mysite/asgi.py`：作为你的项目的运行在 ASGI 兼容的 Web 服务器上的入口
- `mysite/wsgi.py`：作为你的项目的运行在 WSGI 兼容的Web服务器上的入口



## 运行

输入以下命令运行Django项目：

~~~ python
python manage.py runserver
~~~

或者

~~~ python
python manage.py runserver 0.0.0.0:8000
~~~



## 创建应用

通过以下命令创建应用：

~~~ python
python manage.py startapp login
~~~

应用的目录结构：

~~~ python
login/
    __init__.py
    admin.py
    apps.py
    migrations/
        __init__.py
    models.py
    tests.py
    views.py
~~~



### 改变模型

- 编辑 `models.py` 文件，改变模型。



### 激活模型

因为 `LoginConfig` 类写在文件 `login/apps.py` 中，所以它的点式路径是 `'login.apps.LoginConfig'`。在文件 `网站名/settings.py` 中 [`INSTALLED_APPS`](https://docs.djangoproject.com/zh-hans/4.2/ref/settings/#std-setting-INSTALLED_APPS) 子项添加点式路径:

~~~ python
INSTALLED_APPS = [
    "login.apps.LoginConfig",
    "django.contrib.admin",
    "django.contrib.auth",
    "django.contrib.contenttypes",
    "django.contrib.sessions",
    "django.contrib.messages",
    "django.contrib.staticfiles",
]
~~~



### 应用中创建模型

~~~ python
 python manage.py makemigrations login
~~~

创建login应用中的数据结构



### 应用中迁移数据

~~~ python
python manage.py migrate
~~~



### 应用测试API

~~~ python
python manage.py shell
~~~



## 创建管理员账号

~~~ python
python manage.py createsuperuser
~~~



创建用户名

~~~ python
Username: admin
~~~



创建邮箱(可以随便写):

~~~ python 
Email address: admin@example.com
~~~



创建密码(要求输入两次):

~~~~ python
Password: **********
Password (again): *********
Superuser created successfully.
~~~~

