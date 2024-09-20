Python3.9新特性<br />以下内容参考Python3.9中文文档：[https://docs.python.org/zh-cn/3.9/whatsnew/3.9.html#optimizations](https://docs.python.org/zh-cn/3.9/whatsnew/3.9.html#optimizations)
<a name="TXoVu"></a>
## 新的语法特性

- [**PEP 584**](https://www.python.org/dev/peps/pep-0584)，为 `dict` 增加合并运算符；<br />
- [**PEP 585**](https://www.python.org/dev/peps/pep-0585)，标准多项集中的类型标注泛型。<br />
- [**PEP 614**](https://www.python.org/dev/peps/pep-0614)，放宽对装饰器的语法限制。
<a name="7AeEt"></a>
### 字典合并与更新运算符
合并 (`|`) 与更新 (`|=`) 运算符已被加入内置的 [`dict`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#dict) 类。 它们为现有的 `dict.update` 和 `{**d1, **d2}` 字典合并方法提供了补充。<br />示例:
```python
>>> x = {"key1": "value1 from x", "key2": "value2 from x"}
>>> y = {"key2": "value2 from y", "key3": "value3 from y"}
>>> x | y
{'key1': 'value1 from x', 'key2': 'value2 from y', 'key3': 'value3 from y'}
>>> y | x
{'key2': 'value2 from x', 'key3': 'value3 from y', 'key1': 'value1 from x'}
```
<a name="HJbCI"></a>
### 新增用于移除前缀和后缀的字符串方法
增加了 [`str.removeprefix(prefix)`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#str.removeprefix) 和 [`str.removesuffix(suffix)`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#str.removesuffix) 用于方便地从字符串移除不需要的前缀或后缀。 也增加了 `bytes`, `bytearray` 以及 `collections.UserString` 的对应方法。
<a name="LgJMh"></a>
### 标准多项集中的类型标注泛型
在类型标注中现在可以使用内置多项集类型例如 `list` 和 `dict` 作为通用类型而不必从 `typing` 导入对应的大写形式类型名 (例如 `List` 和 `Dict`)。 标准库中的其他一些类型现在同样也是通用的，例如 `queue.Queue`。<br />示例:
```python
def greet_all(names: list[str]) -> None:
    for name in names:
        print("Hello", name)
```
<a name="7r88g"></a>
## 新的内置特性

- [**PEP 616**](https://www.python.org/dev/peps/pep-0616)，移除前缀和后缀的字符串方法。<br />
<a name="BcAaY"></a>
## 标准库中的新特性

- [**PEP 593**](https://www.python.org/dev/peps/pep-0593)，灵活的函数和变量标注；<br />
- 添加了 [`os.pidfd_open()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.pidfd_open) 以允许不带竞争和信号的进程管理。<br />
<a name="ifukg"></a>
## 解释器的改进

- [**PEP 573**](https://www.python.org/dev/peps/pep-0573)，从 C 扩展类型的方法快速访问模块状态；<br />
- [**PEP 617**](https://www.python.org/dev/peps/pep-0617)，CPython 现在使用基于 PEG 的新解析器；<br />
- 许多 Python 内置类型（range, tuple, set, frozenset, list, dict）现在通过使用 [**PEP 590**](https://www.python.org/dev/peps/pep-0590) vectorcall 获得了加速；<br />
- 垃圾回收不会因恢复的对象而阻塞；<br />
- 许多 Python 模块 (`_abc`, [`audioop`](https://docs.python.org/zh-cn/3.9/library/audioop.html#module-audioop), `_bz2`, `_codecs`, `_contextvars`, `_crypt`, `_functools`, `_json`, `_locale`, [`math`](https://docs.python.org/zh-cn/3.9/library/math.html#module-math), [`operator`](https://docs.python.org/zh-cn/3.9/library/operator.html#module-operator), [`resource`](https://docs.python.org/zh-cn/3.9/library/resource.html#module-resource), [`time`](https://docs.python.org/zh-cn/3.9/library/time.html#module-time), `_weakref`) 现在会使用 PEP 489 所定义的多阶段初始化；<br />
- 许多标准库模块 ([`audioop`](https://docs.python.org/zh-cn/3.9/library/audioop.html#module-audioop), [`ast`](https://docs.python.org/zh-cn/3.9/library/ast.html#module-ast), [`grp`](https://docs.python.org/zh-cn/3.9/library/grp.html#module-grp), `_hashlib`, [`pwd`](https://docs.python.org/zh-cn/3.9/library/pwd.html#module-pwd), `_posixsubprocess`, [`random`](https://docs.python.org/zh-cn/3.9/library/random.html#module-random), [`select`](https://docs.python.org/zh-cn/3.9/library/select.html#module-select), [`struct`](https://docs.python.org/zh-cn/3.9/library/struct.html#module-struct), [`termios`](https://docs.python.org/zh-cn/3.9/library/termios.html#module-termios), [`zlib`](https://docs.python.org/zh-cn/3.9/library/zlib.html#module-zlib)) 现在会使用 PEP 384 所定义的稳定 ABI。

Python 3.9 使用于基于 [PEG](https://en.wikipedia.org/wiki/Parsing_expression_grammar) 的新解析器替代 [LL(1)](https://en.wikipedia.org/wiki/LL_parser)。 新解析器的性能与旧解析器大致相当，但 PEG 在设计新语言特性时的形式化比 LL(1) 更灵活。 我们将在 Python 3.10 及之后版本中开始使用这种灵活性。<br />[`ast`](https://docs.python.org/zh-cn/3.9/library/ast.html#module-ast) 模块会使用新解析器并会生成与旧解析器一致的 AST。<br />在 Python 3.10 中，旧解析器将被移除，依赖于它的所有功能也将被移除（主要是 [`parser`](https://docs.python.org/zh-cn/3.9/library/parser.html#module-parser) 模块，它早已被弃用）。 _只有_ 在 Python 3.9 中，可以使用命令行开关 (`-X oldparser`) 或环境变量 (`PYTHONOLDPARSER=1`) 切换回 LL(1) 解析器。
<a name="Lz4iO"></a>
## 其他语言特性修改

- [`__import__()`](https://docs.python.org/zh-cn/3.9/library/functions.html#__import__) 现在会引发 [`ImportError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ImportError) 而不是 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)，后者曾经会在相对导入超出其最高层级包时发生。 <br />
- Python 现在会获取命令行中指定的脚本文件名 (例如: `python3 script.py`) 的绝对路径: [`__main__`](https://docs.python.org/zh-cn/3.9/library/__main__.html#module-__main__) 模块的 `__file__` 属性将是一个绝对路径，而不是相对路径。 现在此路径在当前目录通过 [`os.chdir()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.chdir) 被改变后仍将保持有效。 作为附带效果，回溯信息也将在此情况下为 [`__main__`](https://docs.python.org/zh-cn/3.9/library/__main__.html#module-__main__) 模块帧显示绝对路径。<br />
- 在 [Python 开发模式](https://docs.python.org/zh-cn/3.9/library/devmode.html#devmode) 以及调试编译版本中，现在会针对字符串编码和解码操作检查 _encoding_ 和 _errors_ 参数。 例如: [`open()`](https://docs.python.org/zh-cn/3.9/library/functions.html#open), [`str.encode()`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#str.encode) 和 [`bytes.decode()`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#bytes.decode)。<br />默认设置下，为保证性能，_errors_ 参数只会在第一次发生编码/解码错误时被检查，并且对于空字符串 _encoding_ 参数有时会被忽略。<br />
- `"".replace("", s, n)` 对于所有非零的 `n` 都将返回 `s` 而不是空字符串。 现在此方法会与 `"".replace("", s)` 保持一致。 对于 [`bytes`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#bytes) 和 [`bytearray`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#bytearray) 对象也有类似的修改。
- 任何有效的表达式现在都可被用作 [decorator](https://docs.python.org/zh-cn/3.9/glossary.html#term-decorator)。 在之前版本中，相关语法则更为严格。 <br />
- 改进了 [`typing`](https://docs.python.org/zh-cn/3.9/library/typing.html#module-typing) 模块的帮助信息。 现在将为所有特殊形式和特殊通用别名 (例如 `Union` 和 `List`) 显示文档字符串。 使用 [`help()`](https://docs.python.org/zh-cn/3.9/library/functions.html#help) 时传入通用别名例如 `List[int]` 将显示对应实体类型 (这里对应的是 `list`) 的帮助信息。<br />
- [`aclose()`](https://docs.python.org/zh-cn/3.9/reference/expressions.html#agen.aclose) / [`asend()`](https://docs.python.org/zh-cn/3.9/reference/expressions.html#agen.asend) / [`athrow()`](https://docs.python.org/zh-cn/3.9/reference/expressions.html#agen.athrow) 的并行运行现在已被禁止，且 `ag_running` 现在会反映异步生成器的实际运行状态。<br />
- 调用 `__iter__` 方法时发生的非预期错误不会再被 [`in`](https://docs.python.org/zh-cn/3.9/reference/expressions.html#in) 运算符以及 [`operator`](https://docs.python.org/zh-cn/3.9/library/operator.html#module-operator) 的 [`contains()`](https://docs.python.org/zh-cn/3.9/library/operator.html#operator.contains), [`indexOf()`](https://docs.python.org/zh-cn/3.9/library/operator.html#operator.indexOf) 和 [`countOf()`](https://docs.python.org/zh-cn/3.9/library/operator.html#operator.countOf) 中的 `TypeError` 所掩盖。<br />
<a name="YQaNn"></a>
## 新的库模块

- [**PEP 615**](https://www.python.org/dev/peps/pep-0615)，IANA 时区数据库现在已包含于标准库的 [`zoneinfo`](https://docs.python.org/zh-cn/3.9/library/zoneinfo.html#module-zoneinfo) 模块；<br />
- 图的拓扑排序实现现在已由新的 [`graphlib`](https://docs.python.org/zh-cn/3.9/library/graphlib.html#module-graphlib) 模块提供。
<a name="WgR7j"></a>
### zoneinfo
[`zoneinfo`](https://docs.python.org/zh-cn/3.9/library/zoneinfo.html#module-zoneinfo) 模块为标准库引入了 IANA 时区数据库。 它添加了 [`zoneinfo.ZoneInfo`](https://docs.python.org/zh-cn/3.9/library/zoneinfo.html#zoneinfo.ZoneInfo)，这是一个基于系统时区数据的实体 [`datetime.tzinfo`](https://docs.python.org/zh-cn/3.9/library/datetime.html#datetime.tzinfo) 实现。<br />示例:
```python
>>> from zoneinfo import ZoneInfo
>>> from datetime import datetime, timedelta
>>> # Daylight saving time
>>> dt = datetime(2020, 10, 31, 12, tzinfo=ZoneInfo("America/Los_Angeles"))
>>> print(dt)
2020-10-31 12:00:00-07:00
>>> dt.tzname()
'PDT'
>>> # Standard time
>>> dt += timedelta(days=7)
>>> print(dt)
2020-11-07 12:00:00-08:00
>>> print(dt.tzname())
PST
```
作为不包含 IANA 数据库的平台的一个回退数据源，还以第一方软件包的形式发布了 [`tzdata`](https://pypi.org/project/tzdata/) 模块 -- 通过 PyPI 发行并由 CPython 核心团队维护。
:::tips
参见<br />[**PEP 615**](https://www.python.org/dev/peps/pep-0615) -- 在标准库中支持 IANA 时区数据库<br />PEP 由 Paul Ganssle 撰写并实现
:::
<a name="fGb5h"></a>
### graphlib
添加了新的 [`graphlib`](https://docs.python.org/zh-cn/3.9/library/graphlib.html#module-graphlib) 模块，其中包含 [`graphlib.TopologicalSorter`](https://docs.python.org/zh-cn/3.9/library/graphlib.html#graphlib.TopologicalSorter) 类来提供图的拓扑排序功能。
<a name="3ZBrb"></a>
## 改进的模块
<a name="VsbkT"></a>
### ast
将 _indent_ 选项添加到 [`dump()`](https://docs.python.org/zh-cn/3.9/library/ast.html#ast.dump)，这允许它产生多行缩进的输出。<br />添加了 [`ast.unparse()`](https://docs.python.org/zh-cn/3.9/library/ast.html#ast.unparse) 作为 [`ast`](https://docs.python.org/zh-cn/3.9/library/ast.html#module-ast) 模块中的一个函数，它可被用来反解析 [`ast.AST`](https://docs.python.org/zh-cn/3.9/library/ast.html#ast.AST) 对象并产生相应的代码字符串，当它被解析时将会产生一个等价的 [`ast.AST`](https://docs.python.org/zh-cn/3.9/library/ast.html#ast.AST) 对象。<br />为 AST 节点添加了文档字符串，其中包含 ASDL 签名，可被用来构造对应的节点。
<a name="Z7Z5O"></a>
### asyncio
出于重要的安全性考量，[`asyncio.loop.create_datagram_endpoint()`](https://docs.python.org/zh-cn/3.9/library/asyncio-eventloop.html#asyncio.loop.create_datagram_endpoint) 的 _reuse_address_ 形参不再被支持。 这是由 UDP 中的套接字选项 `SO_REUSEADDR` 的行为导致的。 更多细节请参阅 `loop.create_datagram_endpoint()` 的文档。<br />添加了新的 [coroutine](https://docs.python.org/zh-cn/3.9/glossary.html#term-coroutine) [`shutdown_default_executor()`](https://docs.python.org/zh-cn/3.9/library/asyncio-eventloop.html#asyncio.loop.shutdown_default_executor)，它可为等待 [`ThreadPoolExecutor`](https://docs.python.org/zh-cn/3.9/library/concurrent.futures.html#concurrent.futures.ThreadPoolExecutor) 结束关闭的默认执行器安排关闭日程操作。 此外，[`asyncio.run()`](https://docs.python.org/zh-cn/3.9/library/asyncio-task.html#asyncio.run) 已被更新以使用新的 [coroutine](https://docs.python.org/zh-cn/3.9/glossary.html#term-coroutine)。<br />添加了 [`asyncio.PidfdChildWatcher`](https://docs.python.org/zh-cn/3.9/library/asyncio-policy.html#asyncio.PidfdChildWatcher)，这是一个 Linux 专属的子监视器实现，它负责轮询进程的文件描述符。 ([bpo-38692](https://bugs.python.org/issue38692))<br />添加了新的 [coroutine](https://docs.python.org/zh-cn/3.9/glossary.html#term-coroutine) [`asyncio.to_thread()`](https://docs.python.org/zh-cn/3.9/library/asyncio-task.html#asyncio.to_thread)。 它主要被用于在单独线程中运行 IO 密集型函数以避免阻塞事件循环，实质上就相当于是 [`run_in_executor()`](https://docs.python.org/zh-cn/3.9/library/asyncio-eventloop.html#asyncio.loop.run_in_executor) 的高层级版本，可直接接受关键字参数。<br />当由于超时而取消任务时，[`asyncio.wait_for()`](https://docs.python.org/zh-cn/3.9/library/asyncio-task.html#asyncio.wait_for) 现在将会等待直到也在 _timeout_ 值 <= 0 的情况下完成取消。 就像 timeout 值为正数时一样。<br />当附带 [`ssl.SSLSocket`](https://docs.python.org/zh-cn/3.9/library/ssl.html#ssl.SSLSocket) 套接字调用不兼容的方法时 [`asyncio`](https://docs.python.org/zh-cn/3.9/library/asyncio.html#module-asyncio) 现在会引发 `TyperError`。
<a name="Y8nQi"></a>
### compileall
为重复的 `.pyc` 文件添加了使用硬软件的可能性: _hardlink_dupes_ 形参以及 --hardlink-dupes 命令行选项。<br />新增了一些用于在结果 `.pyc` 文件中操纵路径的选项: _stripdir_, _prependdir_, _limit_sl_dest_ 形参以及 -s, -p, -e 命令行选项。 并使得为优化等级多次指定选项成为可能。 
<a name="v8gHV"></a>
### concurrent.futures
将新的 _cancel_futures_ 形参添加到 [`concurrent.futures.Executor.shutdown()`](https://docs.python.org/zh-cn/3.9/library/concurrent.futures.html#concurrent.futures.Executor.shutdown)，可以取消尚未开始运行的所有挂起的 Future，而不必等待它们完成运行再关闭执行器。<br />从 [`ThreadPoolExecutor`](https://docs.python.org/zh-cn/3.9/library/concurrent.futures.html#concurrent.futures.ThreadPoolExecutor) 和 [`ProcessPoolExecutor`](https://docs.python.org/zh-cn/3.9/library/concurrent.futures.html#concurrent.futures.ProcessPoolExecutor) 中移除了守护线程。 这改善与与子解释器的兼容性及它们在关闭进程时的可预测性。<br />现在 [`ProcessPoolExecutor`](https://docs.python.org/zh-cn/3.9/library/concurrent.futures.html#concurrent.futures.ProcessPoolExecutor) 中的工作进程仅会在没有可重用的空闲工作进程时按需产生。 这优化了启动开销并减少了由空闲工作进程导致的 CPU 时间损失。
<a name="zqXna"></a>
### curses
增加了 [`curses.get_escdelay()`](https://docs.python.org/zh-cn/3.9/library/curses.html#curses.get_escdelay), [`curses.set_escdelay()`](https://docs.python.org/zh-cn/3.9/library/curses.html#curses.set_escdelay), [`curses.get_tabsize()`](https://docs.python.org/zh-cn/3.9/library/curses.html#curses.get_tabsize) 以及 [`curses.set_tabsize()`](https://docs.python.org/zh-cn/3.9/library/curses.html#curses.set_tabsize) 函数。<br />[`datetime.date`](https://docs.python.org/zh-cn/3.9/library/datetime.html#datetime.date) 的 [`isocalendar()`](https://docs.python.org/zh-cn/3.9/library/datetime.html#datetime.date.isocalendar) 以及 [`datetime.datetime`](https://docs.python.org/zh-cn/3.9/library/datetime.html#datetime.datetime) 的 [`isocalendar()`](https://docs.python.org/zh-cn/3.9/library/datetime.html#datetime.datetime.isocalendar) 等方法现在将返回 [`namedtuple()`](https://docs.python.org/zh-cn/3.9/library/collections.html#collections.namedtuple) 而不是 [`tuple`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#tuple)。
<a name="Eb6it"></a>
### distutils
**upload** 命令现在会创建 SHA2-256 和 Blake2b-256 哈希摘要。 它会在禁用 MD5 摘要的平台上跳过 MD5。
<a name="NRpaL"></a>
### fcntl
增加了 `F_OFD_GETLK`, `F_OFD_SETLK` 和 `F_OFD_SETLKW` 等常量。
<a name="Y76Lm"></a>
### ftplib
现在 [`FTP`](https://docs.python.org/zh-cn/3.9/library/ftplib.html#ftplib.FTP) 和 [`FTP_TLS`](https://docs.python.org/zh-cn/3.9/library/ftplib.html#ftplib.FTP_TLS) 当它们的构造器所给定的超时参数为零以防止创建非阻塞套接字时会引发 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。 
<a name="T8Za5"></a>
### gc
当垃圾回收器进行某些复活对象的收集时（在终结器被执行之后这些对象可以在隔离周期之外被访问），不会阻止对所有仍然无法访问的对象的收集。<br />增加了一个新的函数 [`gc.is_finalized()`](https://docs.python.org/zh-cn/3.9/library/gc.html#gc.is_finalized) 用来检测一个对象是否已被垃圾回收器所终结。
<a name="oybWU"></a>
### hashlib
[`hashlib`](https://docs.python.org/zh-cn/3.9/library/hashlib.html#module-hashlib) 模块现在会在可能的情况下使用 OpenSSL 中的 SHA3 哈希和 SHAKE XOF。内置的哈希模块现在可通过 `./configure --without-builtin-hashlib-hashes` 禁用或通过 `./configure --with-builtin-hashlib-hashes=sha3,blake2` 这样的形式有选择地启用以强制使用基于 OpenSSL 的实现。 
<a name="a7QCM"></a>
### http
添加 HTTP 状态码 `103 EARLY_HINTS`, `418 IM_A_TEAPOT` 和 `425 TOO_EARLY` 到 [`http.HTTPStatus`](https://docs.python.org/zh-cn/3.9/library/http.html#http.HTTPStatus)。
<a name="kjdSZ"></a>
### IDLE 与 idlelib
添加了切换光标闪烁停止的选项。<br />Esc 键现在会关闭 IDLE 补全提示窗口。<br />添加关键字到模块名称补全列表。<br />上述修改已被反向移植到 3.8 维护发行版中。
<a name="FJqdE"></a>
### imaplib
现在 [`IMAP4`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4) 和 [`IMAP4_SSL`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4_SSL) 的构造器具有可选的 _timeout_ 形参。 并且，现在 [`open()`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4.open) 方法也具有可选的 _timeout_ 形参提供同样的修改。 [`IMAP4_SSL`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4_SSL) 和 [`IMAP4_stream`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4_stream) 中被重载的方法也应用了这个修改。<br />增加了 [`imaplib.IMAP4.unselect()`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4.unselect)。 [`imaplib.IMAP4.unselect()`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4.unselect) 会释放关联到选定邮箱的服务器资源并将服务器返回到已认证状态。 此命令会执行与 [`imaplib.IMAP4.close()`](https://docs.python.org/zh-cn/3.9/library/imaplib.html#imaplib.IMAP4.close) 相同的动作，区别在于它不会从当前选定邮箱中永久性地移除消息。
<a name="fIbPZ"></a>
### importlib
为提升与 import 语句的一致性，现在 [`importlib.util.resolve_name()`](https://docs.python.org/zh-cn/3.9/library/importlib.html#importlib.util.resolve_name) 对于无效的相对导入尝试会引发 [`ImportError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ImportError) 而不是 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。<br />发布不可变模块对象的导入加载器除了发布单独模块以外现在也可以发布不可变包。<br />添加了带有对包数据中子目录支持的 [`importlib.resources.files()`](https://docs.python.org/zh-cn/3.9/library/importlib.html#importlib.resources.files) 函数，与 `importlib_resources` 1.5 版的反向端口相匹配。<br />来自 `importlib_metadata` 1.6.1 版的已更新 `importlib.metadata`。
<a name="LBjZo"></a>
### inspect
[`inspect.BoundArguments.arguments`](https://docs.python.org/zh-cn/3.9/library/inspect.html#inspect.BoundArguments.arguments) 已从 `OrderedDict` 改为常规字典。
<a name="keGPl"></a>
### ipaddress
[`ipaddress`](https://docs.python.org/zh-cn/3.9/library/ipaddress.html#module-ipaddress) 现在支持 IPv6 作用域地址（即带有 `%<scope_id>` 前缀的 IPv6 地址）。<br />IPv6 作用域地址可使用 [`ipaddress.IPv6Address`](https://docs.python.org/zh-cn/3.9/library/ipaddress.html#ipaddress.IPv6Address) 来解析。 作用域的区 ID 如果存在，可通过 [`scope_id`](https://docs.python.org/zh-cn/3.9/library/ipaddress.html#ipaddress.IPv6Address.scope_id) 属性来获取。
<a name="pV9Yt"></a>
### math
对 [`math.gcd()`](https://docs.python.org/zh-cn/3.9/library/math.html#math.gcd) 函数进行了扩展以处理多个参数。 在之前版本中，它只支持两个参数。<br />增加了 [`math.lcm()`](https://docs.python.org/zh-cn/3.9/library/math.html#math.lcm): 返回指定参数的最小公倍数。<br />增加了 [`math.nextafter()`](https://docs.python.org/zh-cn/3.9/library/math.html#math.nextafter): 返回从 _x_ 往 _y_ 方向的下一个浮点数值。<br />增加了 [`math.ulp()`](https://docs.python.org/zh-cn/3.9/library/math.html#math.ulp): 返回一个浮点数的最小有效比特位。
<a name="ZWQA7"></a>
### multiprocessing
[`multiprocessing.SimpleQueue`](https://docs.python.org/zh-cn/3.9/library/multiprocessing.html#multiprocessing.SimpleQueue) 类新增了 [`close()`](https://docs.python.org/zh-cn/3.9/library/multiprocessing.html#multiprocessing.SimpleQueue.close) 方法用来显式地关闭队列。
<a name="tLRSC"></a>
### nntplib
现在 [`NNTP`](https://docs.python.org/zh-cn/3.9/library/nntplib.html#nntplib.NNTP) 和 [`NNTP_SSL`](https://docs.python.org/zh-cn/3.9/library/nntplib.html#nntplib.NNTP_SSL) 当它们的构造器所给定的超时参数为零以防止创建非阻塞套接字时会引发 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。
<a name="i2X58"></a>
### os
增加了 [`CLD_KILLED`](https://docs.python.org/zh-cn/3.9/library/os.html#os.CLD_KILLED) 和 [`CLD_STOPPED`](https://docs.python.org/zh-cn/3.9/library/os.html#os.CLD_STOPPED) 作为 `si_code`。<br />对外公开了 Linux 专属的 [`os.pidfd_open()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.pidfd_open) ([bpo-38692](https://bugs.python.org/issue38692)) 和 [`os.P_PIDFD`](https://docs.python.org/zh-cn/3.9/library/os.html#os.P_PIDFD) ([bpo-38713](https://bugs.python.org/issue38713)) 用于文件描述符的进程管理。<br />现在 [`os.unsetenv()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.unsetenv) 函数在 Windows 上也已可用。<br />现在 [`os.putenv()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.putenv) 和 [`os.unsetenv()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.unsetenv) 函数将总是可用。<br />增加了 [`os.waitstatus_to_exitcode()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.waitstatus_to_exitcode) 函数：将等待状态转换为退出码。
<a name="k7nJk"></a>
### pathlib
增加了 [`pathlib.Path.readlink()`](https://docs.python.org/zh-cn/3.9/library/pathlib.html#pathlib.Path.readlink)，其行为类似于 [`os.readlink()`](https://docs.python.org/zh-cn/3.9/library/os.html#os.readlink)。
<a name="f96xg"></a>
### pdb
在 Windows 上 [`Pdb`](https://docs.python.org/zh-cn/3.9/library/pdb.html#pdb.Pdb) 现在支持 `~/.pdbrc`。
<a name="chiod"></a>
### poplib
现在 [`POP3`](https://docs.python.org/zh-cn/3.9/library/poplib.html#poplib.POP3) 和 [`POP3_SSL`](https://docs.python.org/zh-cn/3.9/library/poplib.html#poplib.POP3_SSL) 当它们的构造器所给定的超时参数为零以防止创建非阻塞套接字时会引发 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。
<a name="2CET7"></a>
### pprint
现在 [`pprint`](https://docs.python.org/zh-cn/3.9/library/pprint.html#module-pprint) 能美化打印 [`types.SimpleNamespace`](https://docs.python.org/zh-cn/3.9/library/types.html#types.SimpleNamespace)。 
<a name="A27ay"></a>
### pydoc
文档字符串的显示现在不仅针对类、函数、方法等，也针对任何具有自己的 `__doc__` 属性的对象。
<a name="yHRN0"></a>
### random
增加了新的 `random.Random.randbytes` 方法：生成随机字节串。
<a name="1rbDM"></a>
### signal
对外公开了 Linux 专属的 [`signal.pidfd_send_signal()`](https://docs.python.org/zh-cn/3.9/library/signal.html#signal.pidfd_send_signal) 用于向使用文件描述符而非 pid 的进程发送信号。 
<a name="hAOY8"></a>
### smtplib
现在 [`SMTP`](https://docs.python.org/zh-cn/3.9/library/smtplib.html#smtplib.SMTP) 和 [`SMTP_SSL`](https://docs.python.org/zh-cn/3.9/library/smtplib.html#smtplib.SMTP_SSL) 当它们的构造器所给定的超时参数为零以防止创建非阻塞套接字时会引发 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。<br />现在 `LMTP` 构造器具有可选的 _timeout_ 形参。
<a name="D0CUQ"></a>
### socket
[`socket`](https://docs.python.org/zh-cn/3.9/library/socket.html#module-socket) 模块现在会在 Linux 4.1 或更高版本上导出 [`CAN_RAW_JOIN_FILTERS`](https://docs.python.org/zh-cn/3.9/library/socket.html#socket.CAN_RAW_JOIN_FILTERS) 常量。<br />现在 socket 模块会在支持的平台上支持 [`CAN_J1939`](https://docs.python.org/zh-cn/3.9/library/socket.html#socket.CAN_J1939) 协议。<br />现在 socket 模块具有 `socket.send_fds()` 和 `socket.recv.fds()` 方法。
<a name="zBbze"></a>
### time
在 AIX 上，现在 [`thread_time()`](https://docs.python.org/zh-cn/3.9/library/time.html#time.thread_time) 是使用具有纳秒级精度的 `thread_cputime()` 实现，而不再是只有 10 毫秒精度的 `clock_gettime(CLOCK_THREAD_CPUTIME_ID)`。 
<a name="a4pPf"></a>
### sys
增加了新的 [`sys.platlibdir`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.platlibdir) 属性：平台专属库目录的名称。 它被用于构建标准库的路径以及已安装扩展模块的路径。 它在大多数平台上等于 `"lib"`。 在 Fedora 和 SuSE 上，它等于 64 位平台上的 `"lib64"`。<br />之前的版本中，[`sys.stderr`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.stderr) 在非交互模式时是带块缓冲的。 现在 `stderr` 默认总是为行缓冲的。
<a name="TbFpv"></a>
### tracemalloc
增加了 [`tracemalloc.reset_peak()`](https://docs.python.org/zh-cn/3.9/library/tracemalloc.html#tracemalloc.reset_peak) 用于将跟踪的内存块峰值大小设为当前大小，以测量特定代码段的峰值。 
<a name="W0fKA"></a>
### typing
[**PEP 593**](https://www.python.org/dev/peps/pep-0593) 引入了一种 [`typing.Annotated`](https://docs.python.org/zh-cn/3.9/library/typing.html#typing.Annotated) 类型以使用上下文专属的元数据来装饰现有类型，并将新的 `include_extras` 形参添加到 [`typing.get_type_hints()`](https://docs.python.org/zh-cn/3.9/library/typing.html#typing.get_type_hints) 以在运行时访问元数据。
<a name="6SdLv"></a>
### unicodedata
Unicode 数据库已更新到 13.0.0 版。
<a name="jbbON"></a>
### venv
由 [`venv`](https://docs.python.org/zh-cn/3.9/library/venv.html#module-venv) 所提供的激活脚本现在总是会使用 `__VENV_PROMPT__` 设置的值来一致地指明它们的自定义提示符。 在之前版本中某些脚本会无条件地使用 `__VENV_PROMPT__`，而另一些脚本只在其恰好被设置时（这是默认情况）才会使用，还有的脚本会改用 `__VENV_NAME__`。
<a name="Aipqc"></a>
### xml
当把 [`xml.etree.ElementTree`](https://docs.python.org/zh-cn/3.9/library/xml.etree.elementtree.html#module-xml.etree.ElementTree) 序列化为 XML 文件时属性内部的空白字符现在将被保留。 不同的行结束符不会再被正规化为 "n"。 这是对于如何解读 XML 规范 2.11 节的相关讨论的最终结果。 
<a name="BnbiM"></a>
## 性能优化

- 优化了在推导式中为临时变量赋值的惯用方式。 现在推导式中的 `for y in [expr]` 会与简单赋值语句 `y = expr` 一样快速。 例如:
> sums = [s for s in [0] for x in data for s in [s + x]]

- 不同于 `:=` 运算符，这个惯用方式不会使变量泄露到外部作用域中。<br />
- 优化了多线程应用中的信号处理。 如果一个线程不是获得信号的主线程，字节码求值循环不会在每条字节码指令上被打断以检查无法被处理的挂起信号。 只有主解释器的主线程能够处理信号。<br />在之前版本中，字节码求值循环会在每条指令上被打断直到主线程处理了信号。<br />
- 在 FreeBSD 上使用 `closefrom()` 优化了 [`subprocess`](https://docs.python.org/zh-cn/3.9/library/subprocess.html#module-subprocess) 模块。<br />
- [`PyLong_FromDouble()`](https://docs.python.org/zh-cn/3.9/c-api/long.html#c.PyLong_FromDouble) 对于匹配 `long` 的值执行速度现在加快了 1.87 倍。<br />
- 多个 Python 内置类型 ([`range`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#range), [`tuple`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#tuple), [`set`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#set), [`frozenset`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#frozenset), [`list`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#list), [`dict`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#dict)) 现在通过使用 [**PEP 590**](https://www.python.org/dev/peps/pep-0590) 向量调用协议得到加速。<br />
- 当另一集合远大于基础集合的情况下优化了 `difference_update()` 的性能。<br />
- Python 的小对象分配器 (`obmalloc.c`) 现在允许（至多）一个空位可用于立即重用，而不必将其返回给 OS。 这可以防止简单循环中的多余消耗，在每次迭代中可以创建和销毁全新的空位。<br />
- 浮点数运算中的 [floor division](https://docs.python.org/zh-cn/3.9/glossary.html#term-floor-division) 现在会有更好的性能。 并且此运算的 [`ZeroDivisionError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ZeroDivisionError) 的消息也已更新。 <br />
- 使用 UTF-8 和 ascii 编解码器解码短 ASCII 字符串现在会加快大约 15%。<br />

以下是对从 Python 3.4 到 Python 3.9 的提升提升情况的总结:
```
Python version                       3.4     3.5     3.6     3.7     3.8    3.9
--------------                       ---     ---     ---     ---     ---    ---
Variable and attribute read access:
    read_local                       7.1     7.1     5.4     5.1     3.9    4.0
    read_nonlocal                    7.1     8.1     5.8     5.4     4.4    4.8
    read_global                     15.5    19.0    14.3    13.6     7.6    7.7
    read_builtin                    21.1    21.6    18.5    19.0     7.5    7.7
    read_classvar_from_class        25.6    26.5    20.7    19.5    18.4   18.6
    read_classvar_from_instance     22.8    23.5    18.8    17.1    16.4   20.1
    read_instancevar                32.4    33.1    28.0    26.3    25.4   27.7
    read_instancevar_slots          27.8    31.3    20.8    20.8    20.2   24.5
    read_namedtuple                 73.8    57.5    45.0    46.8    18.4   23.2
    read_boundmethod                37.6    37.9    29.6    26.9    27.7   45.9
Variable and attribute write access:
    write_local                      8.7     9.3     5.5     5.3     4.3    4.2
    write_nonlocal                  10.5    11.1     5.6     5.5     4.7    4.9
    write_global                    19.7    21.2    18.0    18.0    15.8   17.2
    write_classvar                  92.9    96.0   104.6   102.1    39.2   43.2
    write_instancevar               44.6    45.8    40.0    38.9    35.5   40.7
    write_instancevar_slots         35.6    36.1    27.3    26.6    25.7   27.7
Data structure read access:
    read_list                       24.2    24.5    20.8    20.8    19.0   21.1
    read_deque                      24.7    25.5    20.2    20.6    19.8   21.6
    read_dict                       24.3    25.7    22.3    23.0    21.0   22.5
    read_strdict                    22.6    24.3    19.5    21.2    18.9   21.6
Data structure write access:
    write_list                      27.1    28.5    22.5    21.6    20.0   21.6
    write_deque                     28.7    30.1    22.7    21.8    23.5   23.2
    write_dict                      31.4    33.3    29.3    29.2    24.7   27.8
    write_strdict                   28.4    29.9    27.5    25.2    23.1   29.8
Stack (or queue) operations:
    list_append_pop                 93.4   112.7    75.4    74.2    50.8   53.9
    deque_append_pop                43.5    57.0    49.4    49.2    42.5   45.5
    deque_append_popleft            43.7    57.3    49.7    49.7    42.8   45.5
Timing loop:
    loop_overhead                    0.5     0.6     0.4     0.3     0.3    0.3
```
以上结果是由以下变量访问基准测试脚本所生成的: `Tools/scripts/var_access_benchmark.py`。 该基准测试脚本以纳秒为单位显示时间。 基准测试数据是在一块 [Intel® Core™ i7-4960HQ 处理器](https://ark.intel.com/content/www/us/en/ark/products/76088/intel-core-i7-4960hq-processor-6m-cache-up-to-3-80-ghz.html) 运行从 [python.org](https://www.python.org/downloads/mac-osx/) 获取的 macOS 64 位编译版本所得到的。
<a name="Jn6oL"></a>
## 弃用

- distutils 的 `bdist_msi` 命令现在已被弃用，请改用 `bdist_wheel` (wheel 包)。<br />
- 目前 [`math.factorial()`](https://docs.python.org/zh-cn/3.9/library/math.html#math.factorial) 接受具有非负整数值的 [`float`](https://docs.python.org/zh-cn/3.9/library/functions.html#float) 实例 (如 `5.0`)。 对于非整数和负浮点数它会引发 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。 此行为现在已被弃用。 在未来的 Python 版本中对所有浮点数都将引发 [`TypeError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#TypeError)。<br />
- [`parser`](https://docs.python.org/zh-cn/3.9/library/parser.html#module-parser) 和 [`symbol`](https://docs.python.org/zh-cn/3.9/library/symbol.html#module-symbol) 模块已被弃用并将在未来的 Python 版本中移除。 对于大多数用例，用户都可以使用 [`ast`](https://docs.python.org/zh-cn/3.9/library/ast.html#module-ast) 模块来控制抽象语法树 (AST) 的生成和编译阶段。<br />
- 公有 C API 函数 [`PyParser_SimpleParseStringFlags()`](https://docs.python.org/zh-cn/3.9/c-api/veryhigh.html#c.PyParser_SimpleParseStringFlags), [`PyParser_SimpleParseStringFlagsFilename()`](https://docs.python.org/zh-cn/3.9/c-api/veryhigh.html#c.PyParser_SimpleParseStringFlagsFilename), [`PyParser_SimpleParseFileFlags()`](https://docs.python.org/zh-cn/3.9/c-api/veryhigh.html#c.PyParser_SimpleParseFileFlags) 和 `PyNode_Compile()` 已被弃用并将在 Python 3.10 版与旧解析器一起被移除。<br />
- 在布尔运算中使用 [`NotImplemented`](https://docs.python.org/zh-cn/3.9/library/constants.html#NotImplemented) 已被弃用，因为它几乎必定是不正确的富比较运算符实现的结果。 它将在未来的 Python 版本中引发 [`TypeError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#TypeError)。 <br />
- [`random`](https://docs.python.org/zh-cn/3.9/library/random.html#module-random) 模块目前接受任何可哈希类型作为可能的种子值。 不幸的是，某些这样的类型并不保证具有确定性的哈希值。 在 Python 3.9 之后，该模块将限定其种子值为 [`None`](https://docs.python.org/zh-cn/3.9/library/constants.html#None), [`int`](https://docs.python.org/zh-cn/3.9/library/functions.html#int), [`float`](https://docs.python.org/zh-cn/3.9/library/functions.html#float), [`str`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#str), [`bytes`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#bytes) 以及 [`bytearray`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#bytearray)。<br />
- 打开 [`GzipFile`](https://docs.python.org/zh-cn/3.9/library/gzip.html#gzip.GzipFile) 文件用于写入而不指定 _mode_ 参数的特性已被弃用。 在未来的 Python 版本中将总是默认打开用于读取。 在打开文件用于写入时请指定 _mode_ 参数以静默相关警告信息。<br />
- 弃用了 `_tkinter.TkappType` 的 `split()` 方法而改用 `splitlist()` 方法，此方法具有更稳定且可预测的行为。<br />
- 将协程对象显式传递给 [`asyncio.wait()`](https://docs.python.org/zh-cn/3.9/library/asyncio-task.html#asyncio.wait) 的做法已被弃用并且将在 3.11 版中被移除。<br />
- binhex4 和 hexbin4 标准现已被弃用。[`binhex`](https://docs.python.org/zh-cn/3.9/library/binhex.html#module-binhex)模块和下列[`binascii`](https://docs.python.org/zh-cn/3.9/library/binascii.html#module-binascii)函数现已被弃用:
   - [`b2a_hqx()`](https://docs.python.org/zh-cn/3.9/library/binascii.html#binascii.b2a_hqx), [`a2b_hqx()`](https://docs.python.org/zh-cn/3.9/library/binascii.html#binascii.a2b_hqx)<br />
   - [`rlecode_hqx()`](https://docs.python.org/zh-cn/3.9/library/binascii.html#binascii.rlecode_hqx), [`rledecode_hqx()`](https://docs.python.org/zh-cn/3.9/library/binascii.html#binascii.rledecode_hqx)<br />
- [`ast`](https://docs.python.org/zh-cn/3.9/library/ast.html#module-ast) 类 `slice`, `Index` 和 `ExtSlice` 被视为已弃用并将在未来的 Python 版本中被移除。 应当使用 `value` 本身而不再是 `Index(value)`。 应当使用 `Tuple(slices, Load())` 而不再是 `ExtSlice(slices)`。<br />
- [`ast`](https://docs.python.org/zh-cn/3.9/library/ast.html#module-ast) 类 `Suite`, `Param`, `AugLoad` 和 `AugStore` 被视为已弃用并将在未来的 Python 版本中被移除。 它们不会被解析器所生成且不会被 Python 3 中的代码生成器所接受。<br />
- [`PyEval_InitThreads()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyEval_InitThreads) 和 [`PyEval_ThreadsInitialized()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyEval_ThreadsInitialized) 函数现已被弃用并将在 Python 3.11 中被移除。 调用 [`PyEval_InitThreads()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyEval_InitThreads) 现在没有任何效果。 自 Python 3.7 起 [GIL](https://docs.python.org/zh-cn/3.9/glossary.html#term-gil) 会由 [`Py_Initialize()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.Py_Initialize) 初始化。<br />
- 传入 `None` 作为 [`shlex.split()`](https://docs.python.org/zh-cn/3.9/library/shlex.html#shlex.split) 函数的第一个参数的做法已被弃用。<br />
- [`smtpd.MailmanProxy()`](https://docs.python.org/zh-cn/3.9/library/smtpd.html#smtpd.MailmanProxy) 现在已被弃用，因为它在没有外部模块 `mailman` 的情况下无法使用。<br />
- 现在 [`lib2to3`](https://docs.python.org/zh-cn/3.9/library/2to3.html#module-lib2to3) 模块将发出 [`PendingDeprecationWarning`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#PendingDeprecationWarning)。 Python 3.9 已切换到 PEG 解析器 (参见 [**PEP 617**](https://www.python.org/dev/peps/pep-0617))，Python 3.10 可以会包含 lib2to3 的 LL(1) 解析器所不能解析的新语法。 `lib2to3` 模块可能会在未来的 Python 版本中被移出标准库。 请考虑使用第三方替换例如 [LibCST](https://libcst.readthedocs.io/) 或 [parso](https://parso.readthedocs.io/)。<br />
- [`random.shuffle()`](https://docs.python.org/zh-cn/3.9/library/random.html#random.shuffle) 的 _random_ 形参已被弃用。<br />


<a name="VP639"></a>
## 移除

- `unittest.mock.__version__` 上的错误版本已经被移除。<br />
- [`nntplib.NNTP`](https://docs.python.org/zh-cn/3.9/library/nntplib.html#nntplib.NNTP): `xpath()` 和 `xgtitle()` 方法已被移除。 这些方法自 Python 3.3 起已被弃用。 一般来说，这些扩展都不再为 NNTP 服务管理员所支持或启用。 对于 `xgtitle()`，请改用 [`nntplib.NNTP.descriptions()`](https://docs.python.org/zh-cn/3.9/library/nntplib.html#nntplib.NNTP.descriptions) 或 [`nntplib.NNTP.description()`](https://docs.python.org/zh-cn/3.9/library/nntplib.html#nntplib.NNTP.description)。<br />
- [`array.array`](https://docs.python.org/zh-cn/3.9/library/array.html#array.array): `tostring()` 和 `fromstring()` 方法已被移除。 它们分别是 `tobytes()` 和 `frombytes()` 的别名，自 Python 3.2 起已被弃用。<br />
- 未写入文档的 `sys.callstats()` 函数已被移除。 自 Python 3.7 起它就已被弃用并且总是会返回 [`None`](https://docs.python.org/zh-cn/3.9/library/constants.html#None)。 它需要一个特殊的构建选项 `CALL_PROFILE` 而该选项在 Python 3.7 中已被移除。<br />
- `sys.getcheckinterval()` 和 `sys.setcheckinterval()` 函数已被移除。 它们自 Python 3.2 起已被弃用。 请改用 [`sys.getswitchinterval()`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.getswitchinterval) 和 [`sys.setswitchinterval()`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.setswitchinterval)。<br />
- C 函数 `PyImport_Cleanup()` 已被移除。 它原本的文档为: "清空模块表。 仅限内部使用。"<br />
- `_dummy_thread` 和 `dummy_threading` 模块已被移除。 这些模块自 Python 3.7 起已被弃用，它们需要线程支持。<br />
- `aifc.open()` 的别名 `aifc.openfp()`，`sunau.open()` 的别名 `sunau.openfp()`，以及 [`wave.open()`](https://docs.python.org/zh-cn/3.9/library/wave.html#wave.open) 的别名 `wave.openfp()` 已被移除。 它们自 Python 3.7 起已被弃用。<br />
- [`threading.Thread`](https://docs.python.org/zh-cn/3.9/library/threading.html#threading.Thread) 的 `isAlive()` 方法已被移除。 它自 Python 3.8 起已被弃用。 请改用 [`is_alive()`](https://docs.python.org/zh-cn/3.9/library/threading.html#threading.Thread.is_alive)。 <br />
- [`ElementTree`](https://docs.python.org/zh-cn/3.9/library/xml.etree.elementtree.html#module-xml.etree.ElementTree) 模块中 [`ElementTree`](https://docs.python.org/zh-cn/3.9/library/xml.etree.elementtree.html#xml.etree.ElementTree.ElementTree) 和 [`Element`](https://docs.python.org/zh-cn/3.9/library/xml.etree.elementtree.html#xml.etree.ElementTree.Element) 等类的 `getchildren()` 和 `getiterator()` 方法已被移除。 它们在 Python 3.2 中已被弃用。 请使用 `iter(x)` 或 `list(x)` 替代 `x.getchildren()` 并用 `x.iter()` 或 `list(x.iter())` 替代 `x.getiterator()`。<br />
- 旧的 [`plistlib`](https://docs.python.org/zh-cn/3.9/library/plistlib.html#module-plistlib) API 已被移除，它自 Python 3.4 起已被弃用。 请使用 [`load()`](https://docs.python.org/zh-cn/3.9/library/plistlib.html#plistlib.load), [`loads()`](https://docs.python.org/zh-cn/3.9/library/plistlib.html#plistlib.loads), [`dump()`](https://docs.python.org/zh-cn/3.9/library/plistlib.html#plistlib.dump) 和 [`dumps()`](https://docs.python.org/zh-cn/3.9/library/plistlib.html#plistlib.dumps) 等函数。 此外，_use_builtin_types_ 形参已被移除而总是会使用 [`bytes`](https://docs.python.org/zh-cn/3.9/library/stdtypes.html#bytes) 对象。<br />
- C 函数 `PyGen_NeedsFinalizing` 已被移除。 它未被写入文档、未经测试，且自 [**PEP 442**](https://www.python.org/dev/peps/pep-0442) 实现之后未在 CPython 的任何地方被使用。 由 Joannah Nanjekye 提供补丁。<br />
- 自 Python 3.1 起被弃用的别名 `base64.encodestring()` 和 `base64.decodestring()` 已被移除：请改用 [`base64.encodebytes()`](https://docs.python.org/zh-cn/3.9/library/base64.html#base64.encodebytes) 和 [`base64.decodebytes()`](https://docs.python.org/zh-cn/3.9/library/base64.html#base64.decodebytes)。<br />
- `fractions.gcd()` 函数已被移除，它自 Python 3.5 起被弃用 ([bpo-22486](https://bugs.python.org/issue22486))：请改用 [`math.gcd()`](https://docs.python.org/zh-cn/3.9/library/math.html#math.gcd)。<br />
- [`bz2.BZ2File`](https://docs.python.org/zh-cn/3.9/library/bz2.html#bz2.BZ2File) 的 _buffering_ 形参已被移除。 它自 Python 3.0 起即被忽略，使用它将会引发 [`DeprecationWarning`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#DeprecationWarning)。 请传入一个打开文件对象来控制文件的打开方式。<br />
- [`json.loads()`](https://docs.python.org/zh-cn/3.9/library/json.html#json.loads) 的 _encoding_ 形参已被移除。 它在 Python 3.1 中已被弃用和忽略；自 Python 3.8 起使用它将会引发 [`DeprecationWarning`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#DeprecationWarning)。<br />
- `with (await asyncio.lock):` 和 `with (yield from asyncio.lock):` 语句已不再受支持，请改用 `async with lock`。 `asyncio.Condition` 和 `asyncio.Semaphore` 也同样如此。<br />
- `sys.getcounts()` 函数，`-X showalloccount` 命令行选项以及 C 结构体 [`PyConfig`](https://docs.python.org/zh-cn/3.9/c-api/init_config.html#c.PyConfig) 的 `show_alloc_count` 字段已被移除。 它们需要使用定义了 `COUNT_ALLOCS` 宏的特殊 Python 编译版本。<br />
- [`typing.NamedTuple`](https://docs.python.org/zh-cn/3.9/library/typing.html#typing.NamedTuple) 类的 `_field_types` 属性已被移除。 它自 Python 3.8 起已被弃用。 请改用 `__annotations__` 属性。<br />
- `symtable.SymbolTable.has_exec()` 方法已被移除。 它自 2006 年起已被弃用，当被调用时仅会返回 `False`。 <br />
- `asyncio.Task.current_task()` 和 `asyncio.Task.all_tasks()` 已被移除。 它们自 Python 3.7 起已被弃用，可以改用 [`asyncio.current_task()`](https://docs.python.org/zh-cn/3.9/library/asyncio-task.html#asyncio.current_task) 和 [`asyncio.all_tasks()`](https://docs.python.org/zh-cn/3.9/library/asyncio-task.html#asyncio.all_tasks)。
- [`html.parser.HTMLParser`](https://docs.python.org/zh-cn/3.9/library/html.parser.html#html.parser.HTMLParser) 类的 `unescape()` 方法已被移除（它自 Python 3.4 起已被弃用）。 应当使用 [`html.unescape()`](https://docs.python.org/zh-cn/3.9/library/html.html#html.unescape) 来将字符引用转换为对应的 unicode 字符。<br />
<a name="L5Bdy"></a>
## 移植到 Python 3.9
本节列出了先前描述的更改以及可能需要更改代码的其他错误修正.
<a name="9gohN"></a>
### Python API 的变化

- [`__import__()`](https://docs.python.org/zh-cn/3.9/library/functions.html#__import__) 和 [`importlib.util.resolve_name()`](https://docs.python.org/zh-cn/3.9/library/importlib.html#importlib.util.resolve_name) 现在会引发 [`ImportError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ImportError) 取代之前所引发的 [`ValueError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#ValueError)。 捕获特定异常类型并同时支持 Python 3.9 和更早版本的调用者将需要使用 `except (ImportError, ValueError):` 来同时捕获两者。<br />
- [`venv`](https://docs.python.org/zh-cn/3.9/library/venv.html#module-venv) 激活脚本不再将 `__VENV_PROMPT__` 被设为 `""` 的情况作为特例处理。<br />
- [`select.epoll.unregister()`](https://docs.python.org/zh-cn/3.9/library/select.html#select.epoll.unregister) 方法不会再忽略 [`EBADF`](https://docs.python.org/zh-cn/3.9/library/errno.html#errno.EBADF) 错误。<br />
- [`bz2.BZ2File`](https://docs.python.org/zh-cn/3.9/library/bz2.html#bz2.BZ2File) 的 _compresslevel_ 形参已成为仅限关键字形参，因为 _buffering_ 形参已被移除。<br />
- 简化了 AST 的抽取操作。 简单索引将以它们的值来代表，扩展切片将以元组形式来代表。 `Index(value)` 将返回 `value` 本身，`ExtSlice(slices)` 将返回 `Tuple(slices, Load())`。<br />
- 当使用了 [`-E`](https://docs.python.org/zh-cn/3.9/using/cmdline.html#cmdoption-e) 或 [`-I`](https://docs.python.org/zh-cn/3.9/using/cmdline.html#id2) 命令行参数时 [`importlib`](https://docs.python.org/zh-cn/3.9/library/importlib.html#module-importlib) 模块现在会忽略 [`PYTHONCASEOK`](https://docs.python.org/zh-cn/3.9/using/cmdline.html#envvar-PYTHONCASEOK) 环境变量。<br />
- _encoding_ 形参已作为仅限关键字形参被添加到 [`ftplib.FTP`](https://docs.python.org/zh-cn/3.9/library/ftplib.html#ftplib.FTP) 和 [`ftplib.FTP_TLS`](https://docs.python.org/zh-cn/3.9/library/ftplib.html#ftplib.FTP_TLS) 类，并且默认编码格式由 Latin-1 改为 UTF-8 以遵循 [**RFC 2640**](https://tools.ietf.org/html/rfc2640.html)。<br />
- [`asyncio.loop.shutdown_default_executor()`](https://docs.python.org/zh-cn/3.9/library/asyncio-eventloop.html#asyncio.loop.shutdown_default_executor) 已被添加到 [`AbstractEventLoop`](https://docs.python.org/zh-cn/3.9/library/asyncio-eventloop.html#asyncio.AbstractEventLoop)，这意味着继承自它的替代事件循环应当定义此方法。<br />
- 更新了 [`__future__`](https://docs.python.org/zh-cn/3.9/library/__future__.html#module-__future__) 模块中未来特性旗标的常量值以防止与编译器旗标相冲突。 在之前版本中 `PyCF_ALLOW_TOP_LEVEL_AWAIT` 会与 `CO_FUTURE_DIVISION` 发生冲突。 <br />
- `array('u')` 现在使用 `wchar_t` 作为 C 类型而不是 `Py_UNICODE`。 这个改变不会影响其行为，因为自 Python 3.3 起 `Py_UNICODE` 是 `wchar_t` 的别名。<br />
- 现在 [`logging.getLogger()`](https://docs.python.org/zh-cn/3.9/library/logging.html#logging.getLogger) API 当传入名称 `'root'` 时将返回根日志记录器，而在之前它则返回一个名为 `'root'` 的非根日志记录器。 这可能会影响到用户代码明确希望使用一个名为 `'root'` 的非根日志记录器，或在某个名为 `'root.py'` 的最高层级模块中使用 `logging.getLogger(__name__)` 来实例化日志记录器的情况。<br />
- 现在 [`PurePath`](https://docs.python.org/zh-cn/3.9/library/pathlib.html#pathlib.PurePath) 的拆分处理当传入 `str` 或 [`PurePath`](https://docs.python.org/zh-cn/3.9/library/pathlib.html#pathlib.PurePath) 的实例以外的对象时会返回 `NotImplemented` 而不是引发 [`TypeError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#TypeError)。 这将允许创建不继承自上述类型的兼容类。<br />
<a name="IQmlp"></a>
### C API 的变化

- 堆分配类型的实例（例如使用 [`PyType_FromSpec()`](https://docs.python.org/zh-cn/3.9/c-api/type.html#c.PyType_FromSpec) 和类似 API 创建的实例）自 Python 3.8 起会带有一个对其类型对象的引用。 正如 Python 3.8 的 "C API 中的改变" 部分所述，对于大部分情况来说，这应当不会有任何副作用，但对于具有自定义 [`tp_traverse`](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#c.PyTypeObject.tp_traverse) 函数的类型来说，则要确保所有堆分配类型的自定义 `tp_traverse` 函数可访问对象的类型。
> 示例:

```c
int
foo_traverse(foo_struct *self, visitproc visit, void *arg) {
// Rest of the traverse function
#if PY_VERSION_HEX >= 0x03090000
    // This was not needed before Python 3.9 (Python issue 35810 and 40217)
    Py_VISIT(Py_TYPE(self));
#endif
}
```

- 如果遍历函数委托给了基类（或其他类）的 `tp_traverse`，则要确保 `Py_TYPE(self)` 只被访问一次。 请注意应当只有堆类型可访问 `tp_traverse` 中的类型。
> 举例来说，如果 `tp_traverse` 函数包括以下内容:

base->tp_traverse(self, visit, arg)
> 则要添加:

```c
#if PY_VERSION_HEX >= 0x03090000
    // This was not needed before Python 3.9 (Python issue 35810 and 40217)
    if (base->tp_flags & Py_TPFLAGS_HEAPTYPE) {
        // a heap type's tp_traverse already visited Py_TYPE(self)
    } else {
        Py_VISIT(Py_TYPE(self));
    }
#else
```

- `PyEval_CallObject`, `PyEval_CallFunction`, `PyEval_CallMethod` 和 `PyEval_CallObjectWithKeywords` 函数已被弃用。 请改用 [`PyObject_Call()`](https://docs.python.org/zh-cn/3.9/c-api/call.html#c.PyObject_Call) 及其变化形式。 <br />
<a name="vqyCD"></a>
### CPython 字节码的改变

- 添加了 [`LOAD_ASSERTION_ERROR`](https://docs.python.org/zh-cn/3.9/library/dis.html#opcode-LOAD_ASSERTION_ERROR) 操作码用于处理 [`assert`](https://docs.python.org/zh-cn/3.9/reference/simple_stmts.html#assert) 语句。 在之前的版本中，如果 [`AssertionError`](https://docs.python.org/zh-cn/3.9/library/exceptions.html#AssertionError) 异常被屏蔽则 assert 语句将不能正常运作。<br />
- [`COMPARE_OP`](https://docs.python.org/zh-cn/3.9/library/dis.html#opcode-COMPARE_OP)操作码已被拆分为四个单独指令:
   - `COMPARE_OP` 用于富比较<br />
   - `IS_OP` 用于 'is' 和 'is not' 检测<br />
   - `CONTAINS_OP` 用于 'in' 和 'not in' 检测<br />
   - `JUMP_IF_NOT_EXC_MATCH` 用于检查 'try-except' 语句中的异常。<br />
<a name="D24Ii"></a>
## 构建的改变

- 将 `--with-platlibdir` 选项添加到 `configure` 脚本：平台专属库目录的名称，保存在新的 [`sys.platlibdir`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.platlibdir) 属性中。 请参阅 [`sys.platlibdir`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.platlibdir) 属性了解详情。<br />
- `COUNT_ALLOCS` 特殊构建宏已被移除。<br />
- 在非 Windows 平台上，现在需要用 `setenv()` 和 `unsetenv()` 函数来构建 Python。<br />
- 在非 Windows 平台上，创建 `bdist_wininst` 安装器现在已不受官方支持。<br />
- 当在 macOS 上用源代码编译 Python 时，`_tkinter` 现在会链接到非系统的 Tcl 和 Tk 框架，如果它们被安装到 `/Library/Frameworks` 的话，就如在较旧的 macOS 发行版上的情况一样。 如果通过使用 `--enable-universalsdk=` 或 `-isysroot` 明确地配置了 macOS SDK，则只会搜索 SDK 本身。 默认行为仍然可以通过 `--with-tcltk-includes` 和 `--with-tcltk-libs` 来覆盖。<br />
- Python 现在可以针对 Windows 10 ARM64 进行编译。<br />
- 现在当使用 `--pgo` 时一些单独的测试会被跳过。 这些测试显著增加了 PGO 任务的时间并且可能无助于提升最终可执行文件的优化程度。 这样能使任务加速大约 15 倍。 运行完整的单元测试是很慢的。 这个改变可能导致优化程序稍差的构建，因为将被执行的代码分支不够多。 如果愿意等待更缓慢的构建，则可以使用 `./configure [..] PROFILE_TASK="-m test --pgo-extended"` 来恢复旧版本的行为。 我们不保证哪个 PGO 任务集能产生更快的构建。 关心此问题的用户应当自行运行相关基准测试，因为结果可能取决于具体环境、工作负载以及编译工具链。
<a name="isCB7"></a>
## C API 的改变
<a name="guWX1"></a>
### 新的特性

- [**PEP 573**](https://www.python.org/dev/peps/pep-0573): 添加了 [`PyType_FromModuleAndSpec()`](https://docs.python.org/zh-cn/3.9/c-api/type.html#c.PyType_FromModuleAndSpec) 用于通过类来关联一个模块；[`PyType_GetModule()`](https://docs.python.org/zh-cn/3.9/c-api/type.html#c.PyType_GetModule) 和 [`PyType_GetModuleState()`](https://docs.python.org/zh-cn/3.9/c-api/type.html#c.PyType_GetModuleState) 用于获取模块及其状态；以及 [`PyCMethod`](https://docs.python.org/zh-cn/3.9/c-api/structures.html#c.PyCMethod) 和 `METH_METHOD` 用于允许一个方法访问其定义所在的类。<br />
- 增加了 [`PyFrame_GetCode()`](https://docs.python.org/zh-cn/3.9/c-api/reflection.html#c.PyFrame_GetCode) 函数：获取帧代码。 增加了 [`PyFrame_GetBack()`](https://docs.python.org/zh-cn/3.9/c-api/reflection.html#c.PyFrame_GetBack) 函数：获取帧的下一个外部帧。<br />
- 将 [`PyFrame_GetLineNumber()`](https://docs.python.org/zh-cn/3.9/c-api/reflection.html#c.PyFrame_GetLineNumber) 添加到受限的 C API。<br />
- 增加了 [`PyThreadState_GetInterpreter()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyThreadState_GetInterpreter) 和 [`PyInterpreterState_Get()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyInterpreterState_Get) 函数用于获取解释器。 增加了 [`PyThreadState_GetFrame()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyThreadState_GetFrame) 函数用于获取 Python 线程状态的当前帧。 增加了 [`PyThreadState_GetID()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.PyThreadState_GetID) 函数：获取 Python 线程状态的唯一标识符。<br />
- 将新的公有 [`PyObject_CallNoArgs()`](https://docs.python.org/zh-cn/3.9/c-api/call.html#c.PyObject_CallNoArgs) 函数添加到 C API，该函数可不带任何参数调用一个 Python 可调用对象。 它是不带参数调用 Python 可调用对象最有效率的方式。 <br />
- 受限 C API 中的改变（如果定义了`Py_LIMITED_API`宏）:
   - 提供 [`Py_EnterRecursiveCall()`](https://docs.python.org/zh-cn/3.9/c-api/exceptions.html#c.Py_EnterRecursiveCall) 和 [`Py_LeaveRecursiveCall()`](https://docs.python.org/zh-cn/3.9/c-api/exceptions.html#c.Py_LeaveRecursiveCall) 作为常规函数用于受限 API。 在之前版本中是使用宏定义，但这些宏不能与无法访问 `PyThreadState.recursion_depth` 字段的受限 C API 一同编译（该结构体在受限 C API 中是不透明的）。<br />
   - `PyObject_INIT()` 和 `PyObject_INIT_VAR()` 已成为常规“不透明”函数以隐藏实现细节。<br />
- 增加了 [`PyModule_AddType()`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModule_AddType) 函数以协助将类型加入到模块中。<br />
- 将 [`PyObject_GC_IsTracked()`](https://docs.python.org/zh-cn/3.9/c-api/gcsupport.html#c.PyObject_GC_IsTracked) 和 [`PyObject_GC_IsFinalized()`](https://docs.python.org/zh-cn/3.9/c-api/gcsupport.html#c.PyObject_GC_IsFinalized) 函数添加到公有 API 以允许分别查询 Python 对象当前是正在被追踪还是已经被垃圾回收器所终结。<br />
- 增加了 `_PyObject_FunctionStr()` 以获取函数类对象的用户友好的表示形式。<br />
<a name="sDVYe"></a>
### 移植到 Python 3.9

- `PyInterpreterState.eval_frame` ([**PEP 523**](https://www.python.org/dev/peps/pep-0523)) 现在需要有新的强制性形参 _tstate_ (`PyThreadState*`)。
- 扩展模块: [`PyModuleDef`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModuleDef) 的 [`m_traverse`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModuleDef.m_traverse), [`m_clear`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModuleDef.m_clear) 和 [`m_free`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModuleDef.m_free) 等函数在模块状态被请求但尚未被分配时将不会再被调用。 这种情况出现在模块被创建之后且模块被执行 ([`Py_mod_exec`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.Py_mod_exec) 函数) 之前的时刻。 更准确地说，这些函数在 [`m_size`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModuleDef.m_size) 大于 0 并且模块状态（即 [`PyModule_GetState()`](https://docs.python.org/zh-cn/3.9/c-api/module.html#c.PyModule_GetState) 的返回值）为 `NULL` 时将不会被调用。<br />没有模块状态的扩展模块 (`m_size <= 0`) 不会受到影响。<br />
- 现在如果 [`Py_AddPendingCall()`](https://docs.python.org/zh-cn/3.9/c-api/init.html#c.Py_AddPendingCall) 是在子解释器内部被调用，该函数会被排入子解释器的调用日程，而不是由主解释器调用。 每个子解释器现在都拥有它们自己的调用日程列表。<br />
- 当 `-E` 选项被使用 (如果 [`PyConfig.use_environment`](https://docs.python.org/zh-cn/3.9/c-api/init_config.html#c.PyConfig.use_environment) 设为 `0`) 时将不再使用 Windows 注册表来初始化 [`sys.path`](https://docs.python.org/zh-cn/3.9/library/sys.html#sys.path)。 这会影响在 Windows 上嵌入 Python 的操作。 <br />
- 全局变量 [`PyStructSequence_UnnamedField`](https://docs.python.org/zh-cn/3.9/c-api/tuple.html#c.PyStructSequence_UnnamedField) 现在为常量并且指向一个字符串常量。<br />
- 现在 `PyGC_Head` 结构是不透明的。 它只在内部 C API (`pycore_gc.h`) 中定义。<br />
- `Py_UNICODE_COPY`, `Py_UNICODE_FILL`, `PyUnicode_WSTR_LENGTH`, [`PyUnicode_FromUnicode()`](https://docs.python.org/zh-cn/3.9/c-api/unicode.html#c.PyUnicode_FromUnicode), [`PyUnicode_AsUnicode()`](https://docs.python.org/zh-cn/3.9/c-api/unicode.html#c.PyUnicode_AsUnicode), `_PyUnicode_AsUnicode` 以及 [`PyUnicode_AsUnicodeAndSize()`](https://docs.python.org/zh-cn/3.9/c-api/unicode.html#c.PyUnicode_AsUnicodeAndSize) 在 C 中被标记为已弃用。 它们自 Python 3.3 起就已被 [**PEP 393**](https://www.python.org/dev/peps/pep-0393) 弃用。<br />
- [`Py_FatalError()`](https://docs.python.org/zh-cn/3.9/c-api/sys.html#c.Py_FatalError) 函数会被一个自动记录当前函数名称的宏所替代，除非已定义了 `Py_LIMITED_API` 宏。 <br />
- vectorcall 协议现在要求调用者只传入字符串作为键名。<br />
- 多个宏和函数的实现细节现在已被隐藏:
   - [`PyObject_IS_GC()`](https://docs.python.org/zh-cn/3.9/c-api/gcsupport.html#c.PyObject_IS_GC) 宏已被转换为函数。<br />
   - `PyObject_NEW()` 宏已成为 [`PyObject_New()`](https://docs.python.org/zh-cn/3.9/c-api/allocation.html#c.PyObject_New) 宏的别名，而 `PyObject_NEW_VAR()` 宏已成为 [`PyObject_NewVar()`](https://docs.python.org/zh-cn/3.9/c-api/allocation.html#c.PyObject_NewVar) 宏的别名。 它们将不再直接访问 [`PyTypeObject.tp_basicsize`](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#c.PyTypeObject.tp_basicsize) 成员。<br />
   - [`PyType_HasFeature()`](https://docs.python.org/zh-cn/3.9/c-api/type.html#c.PyType_HasFeature) 现在总是会调用 [`PyType_GetFlags()`](https://docs.python.org/zh-cn/3.9/c-api/type.html#c.PyType_GetFlags)。 在之前版本中，当受限的 C API 未被使用时它会直接访问 [`PyTypeObject.tp_flags`](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#c.PyTypeObject.tp_flags) 成员。<br />
   - `PyObject_GET_WEAKREFS_LISTPTR()` 宏已被转换为函数：该宏会直接访问 [`PyTypeObject.tp_weaklistoffset`](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#c.PyTypeObject.tp_weaklistoffset) 成员。<br />
   - [`PyObject_CheckBuffer()`](https://docs.python.org/zh-cn/3.9/c-api/buffer.html#c.PyObject_CheckBuffer) 宏已被转换为函数：该宏会直接访问 [`PyTypeObject.tp_as_buffer`](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#c.PyTypeObject.tp_as_buffer) 成员。<br />
   - 现在 [`PyIndex_Check()`](https://docs.python.org/zh-cn/3.9/c-api/number.html#c.PyIndex_Check) 总是被声明为不透明函数以隐藏实现细节；`PyIndex_Check()` 宏已被移除。 该宏会直接访问 [`PyTypeObject.tp_as_number`](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#c.PyTypeObject.tp_as_number) 成员。<br />
<a name="qnhHA"></a>
### 移除

- `pyfpe.h` 的 `PyFPE_START_PROTECT()` 和 `PyFPE_END_PROTECT()` 宏已从受限的 C API 中被排除。<br />
- [PyTypeObject](https://docs.python.org/zh-cn/3.9/c-api/typeobj.html#type-structs) 的 `tp_print` 空位已被移除。 它在 Python 2.7 及之前的版本中被用来将对象打印到文件。 自 Python 3.0 起，它已被忽略并且不再使用。<br />
- 受限 C API 中的改变（如果定义了`Py_LIMITED_API`宏）:
   - 以下函数已从受限 C API 中排除:
      - `PyThreadState_DeleteCurrent()` <br />
      - `_Py_CheckRecursionLimit`<br />
      - `_Py_NewReference()`<br />
      - `_Py_ForgetReference()`<br />
      - `_PyTraceMalloc_NewReference()`<br />
      - `_Py_GetRefTotal()`<br />
      - 在受限 C API 中从未使用的垃圾箱机制。<br />
      - `PyTrash_UNWIND_LEVEL`<br />
      - `Py_TRASHCAN_BEGIN_CONDITION`<br />
      - `Py_TRASHCAN_BEGIN`<br />
      - `Py_TRASHCAN_END`<br />
      - `Py_TRASHCAN_SAFE_BEGIN`<br />
      - `Py_TRASHCAN_SAFE_END`<br />
   - 已将下列函数和定义移至内部 C API:
      - `_PyDebug_PrintTotalRefs()`<br />
      - `_Py_PrintReferences()`<br />
      - `_Py_PrintReferenceAddresses()`<br />
      - `_Py_tracemalloc_config`<br />
      - `_Py_AddToAllObjects()` (`Py_TRACE_REFS` 构建专属)<br />
- 移除了 `_PyRuntime.getframe` 钩子并移除了 `_PyThreadState_GetFrame` 宏，该宏是 `_PyRuntime.getframe` 的一个别名。 它们仅由内部 C API 对外公开。 同样地移除了 `PyThreadFrameGetter` 类型。 <br />
- 从 C API 移除了下列函数。 请显式地调用`PyGC_Collect()`来清空所有自由列表。
   - `PyAsyncGen_ClearFreeLists()`<br />
   - `PyContext_ClearFreeList()`<br />
   - `PyDict_ClearFreeList()`<br />
   - `PyFloat_ClearFreeList()`<br />
   - `PyFrame_ClearFreeList()`<br />
   - `PyList_ClearFreeList()`<br />
   - `PyMethod_ClearFreeList()` 和 `PyCFunction_ClearFreeList()`: 绑定方法对象的自由列表已被移除。<br />
   - `PySet_ClearFreeList()`: 集合自由列表已在 Python 3.4 中被移除。<br />
   - `PyTuple_ClearFreeList()`<br />
   - `PyUnicode_ClearFreeList()`: Unicode 自由列表已在 Python 3.3 中被移除。<br />
- 移除了 `_PyUnicode_ClearStaticStrings()` 函数。<br />
- 移除了 `Py_UNICODE_MATCH`。 它已被 [**PEP 393**](https://www.python.org/dev/peps/pep-0393) 所弃用，并自 Python 3.3 起不再可用。 可以改用 [`PyUnicode_Tailmatch()`](https://docs.python.org/zh-cn/3.9/c-api/unicode.html#c.PyUnicode_Tailmatch) 函数。<br />
- 清除了已定义但未实现的接口的头文件。 被移除了公共 API 符号有: `_PyBytes_InsertThousandsGroupingLocale`, `_PyBytes_InsertThousandsGrouping`, `_Py_InitializeFromArgs`, `_Py_InitializeFromWideArgs`, `_PyFloat_Repr`, `_PyFloat_Digits`, `_PyFloat_DigitsInit`, `PyFrame_ExtendStack`, `_PyAIterWrapper_Type`, `PyNullImporter_Type`, `PyCmpWrapper_Type`, `PySortWrapper_Type`, `PyNoArgsFunction`。<br />
<a name="AmKET"></a>
## 发布进程的变化

- [**PEP 602**](https://www.python.org/dev/peps/pep-0602)，CPython 采用年度发布周期。<br />
