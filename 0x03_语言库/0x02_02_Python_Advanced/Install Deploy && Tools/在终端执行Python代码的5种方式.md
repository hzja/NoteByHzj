Python<br />通常运行程序都是通过IDE里执行运行，有时候也会在命令行通过类似
```python
python hello.py
```
运行代码。实际上，在命令行中运行python程序还有一些其他的方法。这些方法主要运用在服务器环境上。
<a name="TQAsc"></a>
### 1、通过标准输入和管道
因为如何用管道传东西给一个进程是属于 shell 的内容，毋庸置疑，可以将代码传递到 Python 中。
```bash
# 管道传内容给 python
echo "print('hi')" | python
```
如果将文件重定向到 Python，这显然也可以。
```bash
# 重定向一个文件给 python
python < spam.py
```
归功于 Python 的 UNIX 传统，这些都不太令人感到意外。
<a name="pDvwd"></a>
### 2、通过 `-c` 指定的字符串
如果只需要快速地检查某些内容，则可以在命令行中将代码作为字符串传递。
```bash
# 使用 python 的 -c 参数
python -c "print('hi')"
```
当需要检查仅一行或两行代码时，而不是启动 REPL（译注：Read Eval Print Loop，即交互式解释器，例如在 windows 控制台中输入python， 就会进入交互式解释器。`-c` 参数用法可以省去进入解释器界面的过程）
<a name="eRkNS"></a>
### 3、文件的路径
最众所周知的传代码给 python 的方法很可能是通过文件路径。
```bash
# 指定 python 的文件路径
python spam.py
```
要实现这一点的关键是将包含该文件的目录放到sys.path里。这样所有导入都可以继续使用。但这也是为什么不能/不应该传入包含在一个包里的模块路径。<br />因为sys.path可能不包含该包的目录，因此所有的导入将相对于与预期的包不同的目录。（如果是在代码所在文件目录下，可以直接使用此命令，也就是通常在命令行下执行文件的情况）
<a name="skYYW"></a>
### 4、对包使用 `-m`
执行 Python 包的正确方法是使用 -m 并指定要运行的包名。
```python
python -m spam
```
它在底层使用了runpy。要在项目中做到这点，只需要在包里指定一个__main__.py文件，它将被当成__main__执行。而且子模块可以像任何其它模块一样导入，因此可以对其进行各种测试。<br />有些人喜欢在一个包里写一个main子模块，然后将其__main__.py写成：
```python
from . import main

if __name__ == "__main__":
    main.main()
```
就个人而言，不感冒于单独的main模块，而是直接将所有相关的代码放入__main__.py，因为这些模块名是多余的。（译注：即作者不关心作为入口文件的"main"或者“main”模块，因为执行时只需用它们的包名即可。这也暗示了入口模块不该再被其它模块 `import`）
<a name="EoHP3"></a>
### 5、目录
定义`__main__.py`也可以扩展到目录。python news可执行，就是因为 news 目录有一个 main.py 文件。该目录就像一个文件路径被 Python 执行了。<br />现在可能会问：“为什么不直接指定文件路径呢？”坦白说，关于文件路径，有件事得说清楚。😄在发布过程中，可以简单地写上说明，让运行python news/announce.py，但是并没有确切的理由说明这种机制何时存在。<br />再加上以后可以更改文件名，而且没人会注意到。再加上代码会带有辅助文件，因此将其放在目录中而不是单独作为单个文件是有意义的。<br />当然，也可以将它变为一个使用 -m 的包，但是没必要，因为 announce 脚本很简单，它要保持成为一个单独的自足的文件（少于 200 行，并且测试模块也大约是相同的长度） 况且，main.py文件非常简单。
```python
import runpy
# Change 'announce' to whatever module you want to run.
runpy.run_module('announce', run_name='__main__', alter_sys=True)
```
现在显然必须要处理依赖关系，但是如果脚本仅使用标准库或将依赖模块放在`__main__.py`旁边（译注：即同级目录），那么就足够了！
