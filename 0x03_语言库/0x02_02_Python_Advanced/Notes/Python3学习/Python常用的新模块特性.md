Python<br />随着每个 Python 版本的发布，都会添加新模块，并引入新的更好的做事方式，虽然都习惯了使用好的旧 Python 库和某些做事方式，但现在也时候升级并利用新的和改进的模块及其特性了。
<a name="RjLUN"></a>
### Pathlib
pathlib 绝对是 Python 标准库中最近添加的更大的内容之一，自 Python 3.4 以来，它一直是标准库的一部分，但很多人仍然使用 os 模块进行文件系统操作。<br />然而，pathlib 与旧的 os.path 相比具有许多优点 - 虽然 os 模块以原始字符串格式表示路径，但 pathlib 使用面向对象的样式，这使得它更具可读性和编写自然：
```python
from pathlib import Path
import os.path

# 老方式
two_dirs_up = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# 新方式，可读性强
two_dirs_up = Path(__file__).resolve().parent.parent
```
路径被视为对象而不是字符串这一事实也使得可以创建一次对象，然后查找其属性或对其进行操作：
```python
readme = Path("README.md").resolve()

print(f"Absolute path: {readme.absolute()}")
# Absolute path: /home/martin/some/path/README.md
print(f"File name: {readme.name}")
# File name: README.md
print(f"Path root: {readme.root}")
# Path root: /
print(f"Parent directory: {readme.parent}")
# Parent directory: /home/martin/some/path
print(f"File extension: {readme.suffix}")
# File extension: .md
print(f"Is it absolute: {readme.is_absolute()}")
# Is it absolute: True
```
最喜欢 pathlib 的一个特性是可以使用 /（“除法”）运算符来连接路径：
```python
# Operators:
etc = Path('/etc')

joined = etc / "cron.d" / "anacron"
print(f"Exists? - {joined.exists()}")
# Exists? - True
```
重要的是要注意 pathlib 只是替代 `os.path` 而不是整个 os 模块， 它还包括 glob 模块的功能，因此如果习惯于将 `os.path` 与 `glob.glob` 结合使用，那么可以完全用pathlib替代它们。<br />在上面的片段中，展示了一些方便的路径操作和对象属性，但 pathlib 还包括习惯于 `os.path` 的所有方法，例如：
```python
print(f"Working directory: {Path.cwd()}")  # same as os.getcwd()
# Working directory: /home/martin/some/path
Path.mkdir(Path.cwd() / "new_dir", exist_ok=True)  # same as os.makedirs()
print(Path("README.md").resolve())  # same as os.path.abspath()
# /home/martin/some/path/README.md
print(Path.home())  # same as os.path.expanduser()
# /home/martin
```
有关 `os.path` 函数到 pathlib 中新函数的完整映射，请参阅 官方文档。
<a name="wAKDt"></a>
### Secrets
说到 os 模块，应该停止使用的另一部分是 `os.urandom`。相反，应该使用自 Python 3.6 以来可用的新秘密模块：
```python
# 老方式:
import os

length = 64

value = os.urandom(length)
print(f"Bytes: {value}")
# Bytes: b'\xfa\xf3...\xf2\x1b\xf5\xb6'
print(f"Hex: {value.hex()}")
# Hex: faf3cc656370e31a938e7...33d9b023c3c24f1bf5

# 新方式:
import secrets

value = secrets.token_bytes(length)
print(f"Bytes: {value}")
# Bytes: b'U\xe9n\x87...\x85>\x04j:\xb0'
value = secrets.token_hex(length)
print(f"Hex: {value}")
# Hex: fb5dd85e7d73f7a08b8e3...4fd9f95beb08d77391
```
使用 `os.urandom` 实际上并不是这里的问题，引入secrets模块的原因是因为人们使用随机模块来生成密码等，即使随机模块不产生密码安全令牌。<br />根据文档，随机模块不应用于安全目的，应该使用 secrets 或 `os.urandom`，但 secrets 模块绝对更可取，因为它比较新，并且包含一些用于十六进制令牌的实用程序/便利方法以及 URL 安全令牌。
<a name="G2w1X"></a>
### Zoneinfo
在 Python 3.9 之前，没有用于时区操作的内置库，所以每个人都在使用 pytz，但现在在标准库中有 zoneinfo，所以是时候切换了。
```python
from datetime import datetime
import pytz  # pip install pytz

dt = datetime(2022, 6, 4)
nyc = pytz.timezone("America/New_York")

localized = nyc.localize(dt)
print(f"Datetime: {localized}, Timezone: {localized.tzname()}, TZ Info: {localized.tzinfo}")

# 新方式:
from zoneinfo import ZoneInfo

nyc = ZoneInfo("America/New_York")
localized = datetime(2022, 6, 4, tzinfo=nyc)
print(f"Datetime: {localized}, Timezone: {localized.tzname()}, TZ Info: {localized.tzinfo}")
# Datetime: 2022-06-04 00:00:00-04:00, Timezone: EDT, TZ Info: America/New_York
```
datetime 模块将所有时区操作委托给抽象基类 `datetime.tzinfo`， 这个抽象基类需要一个具体的实现——在引入这个很可能来自 pytz 的模块之前。现在在标准库中有 `zoneinfo`，可以使用它。<br />然而，使用 zoneinfo 有一个警告——它假定系统上有可用的时区数据，UNIX 系统就是这种情况， 如果系统没有时区数据，那么应该使用 tzdata 包，它是由 CPython 核心开发人员维护的第一方库，其中包含 IANA 时区数据库。
<a name="T85eX"></a>
### Dataclasses
Python 3.7 的一个重要补充是 dataclasses 包，它是 namedtuple 的替代品。<br />你可能想知道为什么需要替换 namedtuple？以下是应该考虑切换到数据类的一些原因：

- 它可以是可变的
- 默认提供 `repr`、`eq`、`init`、`hash` 魔术方法，
- 允许指定默认值，
- 支持继承。此外，数据类还支持 `frozen` 和 `slots`（从 3.10 开始）属性以提供与命名元组的特征奇偶校验。

切换真的不应该太难，因为只需要更改定义：
```python
# 老方式:
# from collections import namedtuple
from typing import NamedTuple
import sys

User = NamedTuple("User", [("name", str), ("surname", str), ("password", bytes)])

u = User("John", "Doe", b'tfeL+uD...\xd2')
print(f"Size: {sys.getsizeof(u)}")
# Size: 64

# 新方式:
from dataclasses import dataclass

@dataclass()
class User:
   name: str
   surname: str
   password: bytes

u = User("John", "Doe", b'tfeL+uD...\xd2')

print(u)
# User(name='John', surname='Doe', password=b'tfeL+uD...\xd2')

print(f"Size: {sys.getsizeof(u)}, {sys.getsizeof(u) + sys.getsizeof(vars(u))}")
# Size: 48, 152
```
在上面的代码中，还包含了大小比较，因为这是 namedtuple 和数据类之间的较大差异之一，如上所见，命名元组的大小要小得多，这是由于数据类使用 dict 来表示属性。<br />至于速度比较，除非计划创建数百万个实例，否则属性的访问时间应该基本相同，或者不够重要：
```python
import timeit

setup = '''
from typing import NamedTuple
User = NamedTuple("User", [("name", str), ("surname", str), ("password", bytes)])
u = User("John", "Doe", b'')
'''

print(f"Access speed: {min(timeit.repeat('u.name', setup=setup, number=10000000))}")
# Access speed: 0.16838401100540068

setup = '''
from dataclasses import dataclass

@dataclass(slots=True)
class User:
  name: str
  surname: str
  password: bytes

u = User("John", "Doe", b'')
'''

print(f"Access speed: {min(timeit.repeat('u.name', setup=setup, number=10000000))}")
# Access speed: 0.17728697300481144
```
如果以上内容说服了你打算切换到数据类，请尽快尝试吧<br />相反，如果不想切换并且出于某种原因真的想使用命名元组，那么至少应该使用键入模块而不是collections中的 NamedTuple：
```python
# 不好方式的:
from collections import namedtuple
Point = namedtuple("Point", ["x", "y"])

# 更好的方式:
from typing import NamedTuple
class Point(NamedTuple):
    x: float
    y: float
```
最后，如果既不使用 namedtuple 也不使用数据类，可能需要考虑直接使用 Pydantic。
<a name="jALtU"></a>
### Proper Logging
这不是标准库的最新添加，但值得使用 - 应该使用正确的日志记录而不是打印语句， 如果在本地调试问题，则可以使用 print，但对于任何无需用户干预即可运行的生产就绪程序，正确的日志记录是必须的。<br />特别是考虑到设置 Python 日志记录非常简单：
```python
import logging
logging.basicConfig(
    filename='application.log',
    level=logging.WARNING,
    format='[%(asctime)s] {%(pathname)s:%(lineno)d} %(levelname)s - %(message)s',
    datefmt='%H:%M:%S'
)

logging.error("Some serious error occurred.")
# [12:52:35] {<stdin>:1} ERROR - Some serious error occurred.
logging.warning('Some warning.')
# [12:52:35] {<stdin>:1} WARNING - Some warning.
```
与打印语句相比，上面的简单配置将提供卓越的调试体验， 最重要的是，可以进一步自定义日志库以记录到不同的位置、更改日志级别、自动轮换日志等。
<a name="a74uj"></a>
### f-strings
Python 包含很多格式化字符串的方法，包括 C 样式格式化、f 字符串、模板字符串或 .format 函数， 不过，其中之一 - f-strings - 格式化的字符串文字 ， 它们写起来更自然，可读性更强，并且是前面提到的选项中最快的。<br />因此，没有必要争论或解释为什么要使用它们，然而，在某些情况下不能使用 f 字符串：<br />使用 % 格式的唯一原因是用于记录：
```python
import logging

things = "something happened..."

logger = logging.getLogger(__name__)
logger.error("Message: %s", things)  # 评估内部记录器方法
logger.error(f"Message: {things}")  # 立即评估
```
在上面的示例中，如果使用 f 字符串，则表达式将立即计算，而使用 C 样式格式，替换将被推迟到实际需要时，这对于消息分组很重要，其中具有相同模板的所有消息都可以记录为一个， 这不适用于 f 字符串，因为模板在传递给记录器之前填充了数据。<br />此外，有些事情是 f-strings 根本无法做到的， 例如在运行时填充模板 - 即动态格式 - 这就是 f-strings 被称为文字字符串格式的原因：
```python
# 动态设置模板及其参数
def func(tpl: str, param1: str, param2: str) -> str:
    return tpl.format(param=param1, param2=param2)

some_template = "First template: {param1}, {param2}"
another_template = "Other template: {param1} and {param2}"

print(func(some_template, "Hello", "World"))
print(func(another_template, "Hello", "Python"))

# 动态重用具有不同参数的相同模板.
inputs = ["Hello", "World", "!"]
template = "Here's some dynamic value: {value}"

for value in inputs:
    print(template.format(value=value))
```
最重要的是，尽可能使用 f 字符串，因为它们更具可读性和更高性能，但请注意，在某些情况下仍然首选和/或需要其他格式样式。
<a name="nzyDP"></a>
### Tomllib
TOML 是一种广泛使用的配置格式，对于 Python 的工具和生态系统尤其重要，因为它用于 pyproject.toml 配置文件， 到目前为止，必须使用外部库来管理 TOML 文件，但是从 Python 3.11 开始，将有一个名为 tomllib 的内置库，它基于 toml 包。<br />所以，一旦切换到 Python 3.11，应该养成使用 `import tomllib` 而不是 `import tomli` 的习惯。少了一种需要担心的依赖！
```python
# import tomli as tomllib
import tomllib

with open("pyproject.toml", "rb") as f:
    config = tomllib.load(f)
    print(config)
    # {'project': {'authors': [{'email': 'contact@martinheinz.dev',
    #                           'name': 'Martin Heinz'}],
    #              'dependencies': ['flask', 'requests'],
    #              'description': 'Example Package',
    #              'name': 'some-app',
    #              'version': '0.1.0'}}

toml_string = """
[project]
name = "another-app"
description = "Example Package"
version = "0.1.1"
"""

config = tomllib.loads(toml_string)
print(config)
# {'project': {'name': 'another-app', 'description': 'Example Package', 'version': '0.1.1'}}
```
<a name="qcA5W"></a>
### Setuptools
最后一个更像是弃用通知：
> 由于 Distutils 已弃用，因此同样不鼓励使用任何来自 distutils 的函数或对象，Setuptools 旨在替换或弃用所有此类用途。

是时候告别 distutils 包并切换到 setuptools 了，setuptools 文档提供了有关如何替换 distutils 用法的指导， 除此之外，PEP 632 还为 setuptools 未涵盖的部分 distutils 提供迁移建议。
<a name="XmYY9"></a>
### 总结
每个新的 Python 版本都会带来新的特性，因此建议查看 Python 发行说明中的“新模块”、“不推荐使用的模块”和“已删除的模块”部分，这是了解 Python 标准重大变化的好方法 ， 通过这种方式，可以不断地将新功能和最佳实践整合到项目中。
