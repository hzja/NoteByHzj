# 重用APP

在开发Django项目的过程中，有一些app是经常需要用到的，比如用户注册和登录app。你每次开发一个新项目，就重新写一个用户系统？不需要的，直接重用先前写好的就行了。

Django的子系统重用是基于app级别的。也就是一个项目可以包含多个互相独立的app，不同项目之间没有关系。但是，一个app可以属于多个项目，可以在任何地点、任何时间和任何项目中被重用。

一个app要能被重用，首先在设计和开发它的阶段，就要考虑后期重用的问题。你需要将该app运行时所必须的全部文件、资源、配置、数据等等都封装在一个整体内。如果有任何一部分重要的内容，放置在app之外，比如最初项目的其它目录下，都将引起重用失败。Django需要使用setuptools和pip来打包我们的app。

## 一、打包app

打包的本质，就是封装你的源代码和文件成为一种新的数据包装格式，有利于传输、分发和安装。在Django中打包一个app只需要下面八个步骤：



### 1. 文件准备

在你的Django项目目录外面，为我们的login应用，准备一个父目录，这里取名`django-login-register`。

**额外提醒：**

为你的app选择一个合适的名字：在取名前，去PyPi搜索一下是否有重名或冲突的app（包）已经存在。建议给app的名字加上“django-”的前缀。名字不能和任何Django的`contrib packages`中的app重名，例如auth、admin、messages等等。



### 2. 拷贝文件

将mysite/login目录中的所有内容拷贝到`django-login-register`目录内。将login/migrations目录中，除了`__init__.py`外的文件全部删除，这些被删除的文件是我们先前在本地数据库的操作记录，不应该打包到里面。



### 3. 创建说明文档

创建一个说明文档`django-login-register/README.rst`，写入下面的内容：

~~~python
=====
登录和注册系统
=====
## 简单的使用步骤：
1. 创建虚拟环境
2. 使用pip安装第三方依赖
3. 添加相应的路由
4. 配置settings
5. 运行migrate命令，创建数据库和数据表
6. 链接你的index页面
7. 运行python manage.py runserver启动服务器

## 路由设置：
from django.contrib import admin
from django.urls import path, include
from login import views
urlpatterns = [
    path('admin/', admin.site.urls),
    path('index/', views.index),
    path('login/', views.login),
    path('register/', views.register),
    path('logout/', views.logout),
    path('confirm/', views.user_confirm),
    path('captcha/', include('captcha.urls'))   
]
## settings配置：
1. 在INSTALLED_APPS中添加‘login’，'captcha'
2. 默认使用Sqlite数据库
3. LANGUAGE_CODE = 'zh-hans'
4. TIME_ZONE = 'Asia/Shanghai'
5. USE_TZ = False
# 邮件服务设置
6. EMAIL_BACKEND = 'django.core.mail.backends.smtp.EmailBackend'
7. EMAIL_HOST = 'smtp.sina.com'
8. EMAIL_PORT = 25
9. EMAIL_HOST_USER = 'xxxx@sina.com'
10. EMAIL_HOST_PASSWORD = 'xxxxx'
# 注册有效期天数
11. CONFIRM_DAYS = 7
这其实是一个纯文本文件，内容和格式完全自由，但核心要点是注明你的app功能和简单的使用方法。
~~~



### 4. 添加授权声明

创建一个`django-login-register/LICENSE`版权申明文件。大多数Django相关的app都基于BSD版权。如果不是发布到正式场合，可以不写。



### 5. 创建setup.py脚本

创建一个`django-login-register/setup.py`文件，包含了编译和安装app的配置细节。这种配置脚本的具体语法，请前往setuptools的官方文档获取详细的教程。下面是一个范例，大多数情况下，你在此基础上改改就可以了：

~~~ python
import os
from setuptools import find_packages, setup
with open(os.path.join(os.path.dirname(__file__), 'README.rst'), encoding='utf-8') as readme:
    README = readme.read()
# allow setup.py to be run from any path
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))
setup(
    name='django-login-register',
    version='1.0',
    packages=find_packages(),
    include_package_data=True,
    license='BSD License',  # example license
    description='一个通用的用户注册和登录系统',
    long_description=README,
    url='https://www.baidu.com/',
    author='liujiang',
    author_email='yourname@example.com',
    classifiers=[
        'Environment :: Web Environment',
        'Framework :: Django',
        'Framework :: Django :: 2.2',  # replace "X.Y" as appropriate
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',  # example license
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        # Replace these appropriately if you are stuck on Python 2.
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Topic :: Internet :: WWW/HTTP',
        'Topic :: Internet :: WWW/HTTP :: Dynamic Content',
    ],
)
~~~

例子中的配置项看起来有点复杂，实际简单得不要不要的。耐心点，就完全不是问题。

需要注意的是，如果你的readme文件中有中文，那么在setup.py文件的open方法中要指定`encoding='utf-8'`，否则会出现编码错误。



### 6. 创建MANIFEST文件

默认情况下，只有Python的模块和包会被打包进我们的app内。为了包含一些其它的文件，比如静态文件、templates模板等非Python语言编写的文件，需要创建一个`django-login-register/MANIFEST.in`文件，并写入下面的内容：

~~~ python
include LICENSE
include README.rst
recursive-include login/static *
recursive-include login/templates *
recursive-include docs *
~~~



### 7. 添加doc目录

该步骤可选，但是强烈推荐将详细的说明文档一起打包。创建一个空的目录`django-login-register/docs`，用于放置app相关的所有文档。同时确认在`django-login-register/MANIFEST.in`文件内有一行`recursive-include docs *`。需要注意的是，如果docs目录是空的，那么它不会被打包进去。



### 8. 执行打包动作

在`django-login-register`目录内，运行`python setup.py sdist`命令。这将会创建一个dist目录，并生成`django-login-register-1.0.tar.gz`文件。同时生成一个`django_login_register.egg-info`文件夹。

八个步骤完成了，我们的app也就打包好了。



## 二、使用打包好的app

实际使用时，我们只需要使用`django-login-register-1.0.tar.gz`这个文件就可以了。

在安装包的时候，最好是以个人用户的身份安装，而不是全系统范围的身份。这样可以有效减少给别的用户带去的影响或被别的用户影响。当然，最好的方式是在virtualenv环境，这种类似隔离的沙盒环境中使用（此时，不需要`--user`选项）。

**安装：**

~~~ shell
pip install --user django-login-register-1.0.tar.gz
~~~



~~~ shell
(venv) D:\work\for_test\dj_test\venv\Scripts>pip install d:\django-login-register-1.0.tar.gz
Processing d:\django-login-register-1.0.tar.gz
Installing collected packages: django-login-register
  Running setup.py install for django-login-register ... done
Successfully installed django-login-register-1.0
(venv) D:\work\for_test\dj_test\venv\Scripts>pip list
Package               Version
--------------------- ---------
certifi               2018.8.13
chardet               3.0.4
Django                2.0.7
django-login-register 1.0
idna                  2.7
Pillow                5.4.1
pip                   10.0.1
pytz                  2018.5
requests              2.19.1
setuptools            39.1.0
urllib3               1.23
~~~

在windows中使用`--user`选项会将文件安装到你的用户目录，而不是python目录中，所以建议不使用这个选项。另外，windows下使用cmd命令行时候，记得使用管理员权限打开。

在虚拟环境中使用pip安装的时候，一定要注意pip和Python的对应关系，所有的重点都是，你必须确保包被安装在了正确的位置。

最后需要提醒的是，这种方式安装后，app的文件会放在Python环境的site-packages中，而不是以源代码的形式放在我们认为的项目中。我们可以import login，可以在settings中注册‘login’，但要修改login中的源码，则需要去site-packages中。

一定要注意，pip安装的时候，使用的名字是django-login-register-1.0.tar.gz，而不是`pip install login`。实际使用中import的时候是‘import login’，而不是‘import django-login-register’。

安装成功后，再安装依赖包django-simple-captcha等等，然后在新Django项目的`INSTALLED_APPS`设置中注册`login`和captcha，按照使用说明，添加路由，修改settings配置，创建数据表，链接新的index页面，然后启动服务器，就可以使用这个app了。

**卸载方法：**

~~~ shell
pip uninstall django-login-register
~~~

一定要清楚的是：**以使用一个第三方库的形式来重用这个app**

重用的过程有些细节可能这里没有仔细说明，但都能见招拆招，解决起来并不麻烦，而且都是基本知识。



## 三、发布你的app

可以通过下面的方式发布你的app：

- 通过邮件的形式将app发送给朋友
- 将app上传到你的网站
- 将app推送到一个公开的仓库，例如PyPI，github等。

在https://packaging.python.org/distributing/#uploading-your-project-to-pypi中有如何上传到PyPI的教程。