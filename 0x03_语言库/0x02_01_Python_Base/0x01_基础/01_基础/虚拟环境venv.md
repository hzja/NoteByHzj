## 什么是虚拟环境

+ Python 3.3 新特性：[Python 虚拟环境](https://peps.python.org/pep-0405/)

+ 假设自己电脑主机 Python 环境称系统环境，默认情况下虚拟环境和系统环境完全隔离

+ 每个系统环境下可有多个虚拟环境

+ 每个虚拟环境有自己的 Python 二进制文件及单独的包安装目录，可在虚拟环境中安装属于自己的 Python 包，而系统环境、其

  他虚拟环境不会拥有这些 Python 包

### 一句话总结

+ Python 虚拟环境主要是为不同 Python 项目创建一个隔离的环境，每个项目都可以拥有独立的依赖包环境，而项目间的依赖包互不影响



## 为什么要虚拟环境

### 背景

+ 假设一个电脑包含多个 Python 项目
+ 每个项目所需的 Python 版本不一致，比如：Python 2.X、Python 3.X
+ 每个项目所需的依赖包不一致，比如 A 项目只要 Selenium 相关依赖包，B 项目只要 Appium 相关依赖包
+ 不同项目用同一个依赖包，但依赖包版本可能不一致



### 假设不用虚拟环境有什么问题？

#### 了解第三方库的安装目录

+ 默认情况下，Python 第三方库都是装在系统固定目录的，比如 Mac 电脑

~~~ python 
import site
site.getsitepackages()
['/usr/local/Cellar/python@3.9/3.9.5/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages']
~~~

+ 电脑上所有项目都会从这个目录去存储、查找第三方库(站点库)



#### 带来的问题

+ 不同项目使用同一个依赖包，但使用不同版本的依赖包时，意味着当开发 A 项目的时候，要装一次依赖包 A 版本，转开发 B 项目时，先要卸载依赖包 A 版本，再重装 B 版本...
+ 一个环境下安装所有项目需要的依赖包，所以某项目需生成 requirement.txt 时会带上很多用不上的依赖包，要删得手工一个个删....



## venv 操作虚拟环境

+ Python 3.5 后推荐使用 venv 来创建虚拟环境



### 创建虚拟环境

~~~ python
python3 -m venv fastapi_test
~~~

fastapi_test 是虚拟环境名字，可以自定义



### 激活虚拟环境

~~~ python
source <环境名称>/bin/activate 
~~~

可看到虚拟环境中 Package 只有最基础的 pip、setuptools

![6f383acb2119ab0a64984c7a8daff742](.\图片\6f383acb2119ab0a64984c7a8daff742.png)



### 关闭虚拟环境

~~~ python
deactivate
~~~



## Pycharm 项目关联新创建的虚拟环境

![44ae84a89394626895bc0e3efd8e88e2](.\图片\44ae84a89394626895bc0e3efd8e88e2.png)

先在项目目录下创建好虚拟环境



### Python Interpreter 选中虚拟环境

![adda2115729837b6f977e826dafbc80c](.\图片\adda2115729837b6f977e826dafbc80c.png)



### 安装项目所需要的库

![b158492df0c5882bd44b09c50428faa7](.\图片\b158492df0c5882bd44b09c50428faa7.png)



#### 安装requirements.txt的包

~~~ shell
pip install -r requirements.txt
~~~



#### pip生成requirements.txt

~~~ shell
pip freeze -> requirements.txt
~~~



### Pycharm 创建虚拟环境

![231c13fda321ca2a2082b0a358c422f3](.\图片\231c13fda321ca2a2082b0a358c422f3.png)

1. 选中 Python Interpreter
2. 查看更多 Interpreters
3. 添加一个新的 Interpreters
4. 选择 New environment，默认选项即可，确定

![e44d3a179f532ec08dd4f7a67a1630a0](.\图片\e44d3a179f532ec08dd4f7a67a1630a0.png)



### 查看虚拟环境的目录

![7e93a5c66a0b7f940220747f1ab396a5](.\图片\7e93a5c66a0b7f940220747f1ab396a5.png)

刚刚创建的两个虚拟环境的目录其实是一样的



#### bin

与虚拟环境交互的文件

~~~ tex
>> ll
total 72
-rw-r--r--  1 polo  staff   8.6K Sep 14 09:26 Activate.ps1
-rw-r--r--  1 polo  staff   1.9K Sep 14 09:26 activate
-rw-r--r--  1 polo  staff   882B Sep 14 09:26 activate.csh
-rw-r--r--  1 polo  staff   2.0K Sep 14 09:26 activate.fish
-rwxr-xr-x  1 polo  staff   269B Sep 14 08:40 pip
-rwxr-xr-x  1 polo  staff   269B Sep 14 08:40 pip3
-rwxr-xr-x  1 polo  staff   269B Sep 14 08:40 pip3.9
lrwxr-xr-x  1 polo  staff     9B Sep 14 08:40 python -> python3.9
lrwxr-xr-x  1 polo  staff     9B Sep 14 08:40 python3 -> python3.9
lrwxr-xr-x  1 polo  staff    39B Sep 14 08:40 python3.9 -> /usr/local/opt/python@3.9/bin/python3.9
~~~



#### include

编译 Python 包的 C 头文件



#### lib

包含Python 版本的副本(python3.9)，以及安装每个依赖包的 site-packages 文件夹

~~~ tex
>> pwd
/Users/polo/Downloads/FastAPI_project/fastapi_test/lib/python3.9/site-packages
 
>> ll
total 224
drwxr-xr-x   3 polo  staff    96B Sep 14 09:27 __pycache__
drwxr-xr-x   5 polo  staff   160B Sep 14 08:40 _distutils_hack
-rw-r--r--   1 polo  staff   152B Sep 14 08:40 distutils-precedence.pth
drwxr-xr-x  28 polo  staff   896B Sep 14 09:27 fastapi
drwxr-xr-x   8 polo  staff   256B Sep 14 09:27 fastapi-0.68.1.dist-info
drwxr-xr-x   8 polo  staff   256B Sep 14 08:40 pip
drwxr-xr-x  10 polo  staff   320B Sep 14 08:40 pip-21.1.1.dist-info
drwxr-xr-x   7 polo  staff   224B Sep 14 08:40 pkg_resources
drwxr-xr-x  53 polo  staff   1.7K Sep 14 09:27 pydantic
drwxr-xr-x   9 polo  staff   288B Sep 14 09:27 pydantic-1.8.2.dist-info
drwxr-xr-x  41 polo  staff   1.3K Sep 14 08:40 setuptools
drwxr-xr-x  11 polo  staff   352B Sep 14 08:40 setuptools-56.0.0.dist-info
drwxr-xr-x  27 polo  staff   864B Sep 14 09:27 starlette
drwxr-xr-x   8 polo  staff   256B Sep 14 09:27 starlette-0.14.2.dist-info
drwxr-xr-x   8 polo  staff   256B Sep 14 09:27 typing_extensions-3.10.0.2.dist-info
-rw-r--r--   1 polo  staff   107K Sep 14 09:27 typing_extensions.py
~~~



### 从虚拟环境生成 requirement.txt

#### 先看看有哪些包

~~~ tex
> pip list
Package           Version
----------------- --------
fastapi           0.68.1
pip               21.1.1
pydantic          1.8.2
setuptools        56.0.0
starlette         0.14.2
typing-extensions 3.10.0.2
~~~



#### pip freeze

在激活虚拟环境下敲

~~~ tex
pip freeze > requirement.txt  
~~~

![7d3997519d93bac4afacd27894a63360](.\图片\7d3997519d93bac4afacd27894a63360.png)

生成的 requirement.txt 文件只包含虚拟环境中已安装的依赖包



### 包管理利器 popety

TODO