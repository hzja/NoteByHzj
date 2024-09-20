Python<br />“12-Factor” 是构建SaaS服务的一种方法论，这套理论适用于任意语言和后端服务（数据库、消息队列、缓存等）开发的应用程序。其中有一条很重要的原则是关于配置的，  12-Factor 要求代码和配置严格分离。为什么要这么做?<br />如果代码放在Github等外部网络，哪一天要是代码不小心泄露了，各种密码，密钥，等配置全都暴露于公网中，这是一件非常可怕的事。判断一个应用是否正确的将配置与代码分离开了，一个简单方法是代码是否可以立刻开源，而不用担心有任何敏感信息暴露。<br />将应用的配置存储于环境变量中是一种常规做法，例如在命令行中加入：
```bash
export PASSWORD=123456
```
Window
```bash
set PASSWORD=123456
```
业务代码中，通过环境变量来加载。
```python
import os
env = os.environ.get("PASSWORD")
print(env)
```
这样做就不会将敏感信息暴露于业务代码中了，同时也可以最大程度让开发人员接触正式环境的敏感信息。<br />不过，问题来了，将敏感信息设置成环境变量，如果这样的信息非常多，挨个设置也太麻烦了。<br />一定希望可以将这些敏感信息单独放在一个文件中，始终与代码分开管理，例如们在一个flask项目中，敏感信息专门放在一个叫.flaskenv 的文件中。<br />.flaskenv 文件
```
FLASK_DEBUG=1
FLASK_ENV=local
AAA=1234
```
可是这些配置如何加载到环境变量中去？
<a name="wxlMi"></a>
## python-dotenv
python-dotenv 就是专门干这事的，他能将配置文件的配置信息自动加入到环境变量。
<a name="l297N"></a>
### 安装 python-dotenv
```bash
pip install python-dotenv
```
<a name="H9WgX"></a>
### 加载配置文件
```python
from dotenv import load_dotenv

# 加载文件
load_dotenv(".flaskenv")


import os

flask_env = os.environ.get("FLASK_ENV")
print(flask_env) # local
```
加载文件后，就可以通过`os.environ`从环境变量中读取内容。
<a name="E7XXM"></a>
## flask配置最佳实践
在flask中，python-dotenv 可以无缝接入项目中，只要项目中存在 .env 或者 .flaskenv 文件，他就会提示是否安装 python-dotenv
```bash
$ flask run
 * Tip: There are .env files present. Do "pip install python-dotenv" to use them.
```
安装完后python-dotenv后，就会自动加载里面的配置文件到环境变量中。
```python
# config.py
class LocalConfig(BaseConfig):
    ENV = "development"
    FLASK_DEBUG = 1
    # 通过变量环境来加载数据库配置
    SQLALCHEMY_DATABASE_URI = os.getenv("SQLALCHEMY_DATABASE_URI")


# app.py
def create_app():
    app = Flask(__name__)
    app.config.from_object(LocalConfig)


# view.py
def hello():
    # 加载环境变量
    os.environ.get("AAA")
```
