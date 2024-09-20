Python Python Web<br />Replit 使编码人员能够快速构建应用程序。除了浏览器，开发人员可以启动服务器，将数据存储在 Repl DB 中，并使用 Repl Auth 对用户进行身份验证。然而，直到今天，用户还必须将所有这些组件串在一起来构建应用程序。这就是为什么使用 replit.web，这是一个专注于快速制作应用程序的 Python 框架。构建具有用户身份验证和持久性的应用程序从未如此简单：
```python
import flask
from replit import db, web


app = flask.Flask(__name__)
users = web.UserStore()


@app.route("/")
@web.authenticated
def index():
    hits = users.current.get("hits", 0) + 1
    users.current["hits"] = hits
return f"You have visited this page {hits} times"


web.run(app)
```
<a name="cwJ89"></a>
### Repl Auth
Repl Auth 是每个 Replit 应用程序附带的内置身份验证系统。虽然 Auth 非常简单，但仍需要编写一些代码才能将其集成到您的应用程序中。由于 replit.web 扩展了流行的 Flask web 框架，现在验证路由就像使用 `web.authenticated` 装饰器要求验证一样简单。然后可以在 `web.auth.name` 下访问当前用户：
```python
@app.route("/")
@web.authenticated
def index():
return f"Hello, {web.auth.name}"
```
<a name="p8Dd3"></a>
### Repl DB
如果没有 Repl DB（每个 repl 随附的完全托管的云数据库）提供毫不费力的持久性，新框架的神奇体验就不可能实现。DB 可以像普通 Python 字典一样使用，它甚至支持嵌套设置功能：
```python
db["bob"] = {"score": 0}
db["bob"]["score"] += 100
db["bob"].get("friends", []).append("Alice")
print(db["bob"]) # => ObservedDict(value={'score': 100, 'friends': ObservedList(value=['Alice'])})
```
`replit.web` 结合了 DB 和 Auth 来提供 `web.UserStore`，这是一个由已认证用户的用户名键控的 db 实例，这使得存储用户数据变得简单：
```python
users = web.UserStore() # optionally, pass a prefix
users.current # shorthand  for users[web.auth.name]
```
最快的构筑体验<br />除了 DB 和 Auth 之外， replit.web 还尝试使 Web 应用程序开发的各个方面变得更容易，包括诸如速率限制和要求参数之类的实用程序。为了测试该框架，团队还构建 了ReplTweet，这是一个为 Replit 用户设计的有趣的仿Twitter功能 。用户可以登录，阅读推文，like他们，并发布自己的想法。整个应用程序有 150 行代码。<br />目标是让这个框架成为制作 Python 应用程序的最快方式。现在，它专注于小型应用程序和原型设计。在 Replit 所做的一切一样，这是一个早期版本，希望就如何改进提供反馈。<br />请访问文档以开始使用：<br />[https://replit-py.readthedocs.io/en/latest/index.html](https://replit-py.readthedocs.io/en/latest/index.html)
