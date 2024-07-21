Python Web
<a name="Y3a0c"></a>
## 速度为先
基于 async/await 语法的异步 Web 框架也有很多，在 github 上找一找比比皆是是，那究竟应该选哪一款呢？在 github 上有一个专门测试各种语言各种 Web 框架速度的项目，来看一看简单的数据：<br />这是所有的 Python Web 框架速度测试，有人可能会问为什么不是从 1 开始排序的，因为这个项目的测试还包含 golang、java、php 等众多语言的 Web 框架，共有 226 款。这里只用 Python 来做对比。<br />可以明显的看到，flask、django、tornado 等老牌的 Python Web 框架已经快要垫底了。<br />看一下测试源码：
```python
# Disable all logging features
import logging

logging.disable()


from flask import Flask
from meinheld import patch

patch.patch_all()

app = Flask(__name__)


@app.route("/")
def index():
    return ""


@app.route("/user/<int:id>", methods=["GET"])
def user_info(id):
    return str(id)


@app.route("/user", methods=["POST"])
def user():
    return ""
```
```python
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt


def index(request):
    return HttpResponse(status=200)


def get_user(request, id):
    return HttpResponse(id)


@csrf_exempt
def create_user(request):
    return HttpResponse(status=200)
```
```python
# Disable all logging features
import logging

logging.disable()


import tornado.httpserver
import tornado.ioloop
import tornado.web


class MainHandler(tornado.web.RequestHandler):
    def get(self):
        pass


class UserHandler(tornado.web.RequestHandler):
    def post(self):
        pass


class UserInfoHandler(tornado.web.RequestHandler):
    def get(self, id):
        self.write(id)


app = tornado.web.Application(
    handlers=[
        (r"/", MainHandler),
        (r"/user", UserHandler),
        (r"/user/(\d+)", UserInfoHandler),
    ]
)
```
```python
# Disable all logging features
import logging

logging.disable()

import multiprocessing

from sanic import Sanic
from sanic.response import text


app = Sanic("benchmark")


@app.route("/")
async def index(request):
    return text("")


@app.route("/user/<id:int>", methods=["GET"])
async def user_info(request, id):
    return text(str(id))


@app.route("/user", methods=["POST"])
async def user(request):
    return text("")


if __name__ == "__main__":
    workers = multiprocessing.cpu_count()
    app.run(host="0.0.0.0", port=3000, workers=workers, debug=False, access_log=False)
```
就是简单的不做任何操作，只返回响应，虽然这样测试没有任何实际意义，在正常的生产环境中不可能什么都不做，但是如果所有的框架都如此测试，也算是从一定程度上在同一起跑线上了吧。
<a name="FsTmE"></a>
## 为什么要用异步 Web 框架？
这可能是众多小伙伴最先想到的问题了吧？用 Django、Flask 用的好好的，能够完成正常的任务，为什么还要用异步 Web 框架呢？<br />说到这里，首先问一个问题，在 Web 开发过程中最大的敌人是谁？<br />在 Web 开发的过程中，最大的敌人不是用户，而是阻塞！<br />是的，而异步可以有效的解决 网络 I/O 阻塞，文件 I/O 阻塞。Python 异步编程。由于异步可以提升效率，所以对于 Python 来说，异步是最好的提升性能的方式之一。这也是为什么要选择 异步 Web 框架的原因。
<a name="Mjld0"></a>
## 生态环境
为什么不推荐 falcon 而是推荐 Sanic ？明明它的速度非常快，要比 Sanic 快了那么多，那看一下下面的代码：
```python
from wsgiref.simple_server import make_server
import falcon


class ThingsResource:
    def on_get(self, req, resp):
        """Handles GET requests"""
        resp.status = falcon.HTTP_200  # This is the default status
        resp.content_type = falcon.MEDIA_TEXT  # Default is JSON, so override
        resp.text = ('\nTwo things awe me most, the starry sky '
                     'above me and the moral law within me.\n'
                     '\n'
                     '    ~ Immanuel Kant\n\n')

app = falcon.App()

things = ThingsResource()

app.add_route('/things', things)

if __name__ == '__main__':
    with make_server('', 8000, app) as httpd:
        print('Serving on port 8000...')

        httpd.serve_forever()
```
一个状态码都要自己定义和填写的框架，它的速度快是值得肯定的，但是对于开发者来说，又有多少的实用价值呢？所以选择框架并不是要选最快的，而是要又快又好用的。<br />而大多数框架并不具备这样的生态环境，这应该也是为什么大多数 Python 的 Web 开发者愿意选择 Django 、 Flask 、 tornado 的原因。就是因为它们的生态相对于其他框架要丰富太多太多。<br />Sanic 框架， 从 2016 年 5 月开始 发布了第一版异步 Web 框架雏形，至今已经走过了 5 个年头，这 5 年，经过不断地技术积累，Sanic 已经由一个步履蹒跚的小框架变成了一个健步如飞的稳重框架。<br />在 awesome-sanic 项目中，记录了大量的第三方库，可以找到任何常用的工具：从 API 到 Authentication，从 Development 到 Frontend，从 Monitoring 到 ORM，从 Caching 到 Queue… 只有想不到的，没有它没有的第三方拓展。
<a name="aoriA"></a>
## 生产环境
以前在国内的社区中看到过一些小伙伴在问 2020 年了，Sanic 可以用于生产环境了吗？<br />答案是肯定的，Sanic 在创建之初目标就是创建一个可以用于生产环境的 Web 框架。可能有些框架会明确的说明框架中自带的 Run 方法仅用于测试环境，不要使用自带的 Run 方法用于部署环境。但是 Sanic 所创建的不止是一个用于测试环境的应用，更是可以直接用在生产环境中的应用。省去了使用 unicorn 等部署的烦恼！
<a name="bgEgs"></a>
## 文档完善
想必大多数 Python 的 Web 开发者 学的第一个框架就是 Flask 或者 Django 吧，尤其是 Django 的文档，大多数小伙伴看了都会心塞。因为旧的版本有中文，但是新的版本，尤其是新特性，完全没有任何中文文档了！这对于关注 Django 发展但英文又不是强项的同学来说，简直苦不堪言。<br />但 Sanic 拥有完善的 中文用户指南 和 API 文档，这些都是由贡献者自主发起的，官方承认的文档，由翻译者进行翻译贡献，由 Sanic 官方团队进行发布。或许有的小伙伴会说 Flask 也有完善的中文文档，但是那是在不同的站点上的，Sanic 的所有文档都有 Sanic 官方进行发布支持。且目前 Sanic 还在持续支持 韩语、葡萄牙语等更多的语种。
<a name="Qw26J"></a>
## 社区指导
和其他框架不同，或许能够在百度上找到论坛、频道等，但这些都是经过本地汉化的，运营者往往并不是官方，且其中夹杂了很多广告。很显然，如果是官方运营的不可能允许这种情况出现。<br />Sanic 不同于其他的社区，所有的论坛、频道完全由官方运营，在这里，可以向核心开发者提问问题，Sanic 的官方发布经理也非常乐意回答各种问题。也可以和志同道合的使用者分享自己的使用经验。这是一个完全开放的环境….<br />Sanic 目前常用的有 论坛、Discord、github issues、twitter、Stackoverflow<br />大家可以在以上的方式中关注 Sanic 的发展以及寻求社区帮助。
