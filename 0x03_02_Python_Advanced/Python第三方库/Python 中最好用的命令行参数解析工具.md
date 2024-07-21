Python<br />Python 做为一个脚本语言，可以很方便地写各种工具。当在服务端要运行一个工具或服务时，输入参数似乎是一种硬需（当然也可以通过配置文件来实现）。<br />如果要以命令行执行，那需要一个命令行参数解析的模块来做这个苦力活。<br />Python 本身就提供了三个命令行参数解析模块，这里罗列一下它们的大致情况供了解。

- `getopt`，只能简单的处理命令行参数
- `optparse`，功能强大，易于使用，可以方便地生成标准的、符合Unix/Posix 规范的命令行说明。（Python2.7以后弃用，不会继续发展）
- `argparse`，使其更加容易的编写用户友好的命令行接口。它所需的程序进程了参数定义，argparse将更好的解析sys.argv。同时argparse模块还能自动生成帮助及用户输入错误参数时的提示信息。

很多初学者可能会使用`getopt`，上手简单功能也简单。比如说`optget`无法解析一个参数多个值的情况，如 `--file file1 file2 file3`，考虑到它在Python2.7后已经不再维护，通常也不会使用它。<br />接下来只剩下argparse 这一神器，它几乎能满足对命令解析器的所有需求。它支持解析一参数多值，可以自动生成help命令和帮助文档，支持子解析器，支持限制参数取值范围等等功能。
<a name="e0AZy"></a>
## HelloWorld
不管学习什么东西，首先第一步都应该是掌握它的大体框架。而使用 argparse 前，框架很简单，只需要记住这三行。
```python
# mytest.py
import argparse
parser = argparse.ArgumentParser(description="used for test")

args = parser.parse_args()
```
现在可以尝试一下
```bash
[root@localhost ~]# python mytest.py -h
usage: mytest.py [-h]

used for test

optional arguments:
  -h, --help  show this help message and exit
[root@localhost ~]# 
[root@localhost ~]# 
[root@localhost ~]# python mytest.py
[root@localhost ~]# 
```
已经可以使用了。
<a name="Ndy8M"></a>
## 1、入门配置
这里先讲一下，比较常用的参数配置。

- 调试：debug
- 版本号：version
```python
import argparse
parser = argparse.ArgumentParser()

parser.add_argument('--version', '-v', action='version',
                    version='%(prog)s version : v 0.01',
                    help='show the version')

parser.add_argument('--debug', '-d',
                    action='store_true',
                    help='show the version',
                    default=False)

args = parser.parse_args()
print("=== end ===")
```
上面debug处的配置，需要讲一下的是 `action='store_true'` 和 `default = False` 的作用和区别

- `store_true`：一旦指定了 -d 或者 --debug ，其值就为 True，store_false则相反
- `default=False`：未指定 -d 或者 --debug，其值就默认为False

当执行 `python mytest.py -v`，就会打印 version 里的内容。
```bash
[root@localhost ~]# python mytest.py -v
mytest.py version : v 0.01
[root@localhost ~]# 
```
一旦执行时，指定了参数 `-v` ，执行到 `parser.parse_args()` 就会退出程序，不会打印最后的`=== end ===`
<a name="u0731"></a>
## 2、参数种类
参数可分为 必选参数（positional arguments） 和 可选参数（optional arguments）。<br />在argsparse 里如何实现呢？
<a name="uzNWp"></a>
### 必选参数
用单词做参数，默认就为必选参数
```python
# mytest.py
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("name")

args = parser.parse_args()

print(args.name)
```
不指定name参数运行一下：`python mytest.py`
```bash
[root@localhost ~]# python mytest.py 
usage: mytest.py [-h] name
mytest.py: error: too few arguments
[root@localhost ~]#
```
如预期一样，报错了，说缺少参数。那指定一下：`python mytest.py name wangbm`
```bash
[root@localhost ~]# python mytest.py wangbm
wangbm
[root@localhost ~]# 
```
<a name="vwnFu"></a>
### 可选参数
有两种方式：

- 单横划线 `-` 来指定的短参数，如`-h`；
- 双横划线 `--` 来指定的长参数，如`--help`
```python
# mytest.py
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbosity",
                    help="increase output verbosity")

args = parser.parse_args()

if args.verbosity:
    print("verbosity turned on")
else:
    print("verbosity turned off")
```
运行 `python mytest.py`，不会报错。
```bash
[root@localhost ~]# python mytest.py
verbosity turned off
[root@localhost ~]#
```
<a name="tfbsm"></a>
## 3、参数类型
有的参数，是字符串，有的参数，是数值。<br />为了对命令行中的参数进行有效的约束，可以事先对参数的类型进行声明。argparse 会对参数进行校验，不通过时，会直接抛出错误。
```python
# mytest.py
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("name")
parser.add_argument("age", type=int)

args = parser.parse_args()

print(args.name)
print(args.age)
```
测试一下。
```bash
[root@localhost ~]# python mytest.py wangbm eighteen
usage: mytest.py [-h] name age
mytest.py: error: argument age: invalid int value: 'eighteen'
[root@localhost ~]# 
[root@localhost ~]# python mytest.py wangbm 18
wangbm
18
[root@localhost ~]#
```
写 eighteen 就不行，提示类型不合法，只有写 18 才行。
<a name="RDqEh"></a>
## 4、互斥参数
有些参数，是互斥的，有你无我。比如，性别。<br />在 argparse 中如何实现？
```python
import argparse

parser = argparse.ArgumentParser()
group = parser.add_mutually_exclusive_group()
group.add_argument("-m", "--male", action="store_true")
group.add_argument("-f", "--female", action="store_true")
args = parser.parse_args()
```
如果同时指定了这两个参数，就会报错。
```bash
[root@localhost ~]# python mytest.py -f
[root@localhost ~]# python mytest.py -m
[root@localhost ~]# python mytest.py -m -f 
usage: mytest.py [-h] [-m | -f]
mytest.py: error: argument -f/--female: not allowed with argument -m/--male
[root@localhost ~]# 
```
<a name="Of1cx"></a>
## 5、可选值
如果是性别，可以像上面那样放在两个参数里然后用互斥组来约束，也可以放在一个参数里，在argparse里限制再在外层做判断。
```python
# mytest.py
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-g", "--gender", default='male',
                    choices=['male', 'female'])

args = parser.parse_args()
print(args.gender)
```
试着执行一下，发现性别只能是男或女，不能为人妖。
```bash
[root@localhost ~]# python mytest.py --gender male
male
[root@localhost ~]# python mytest.py --gender female
female
[root@localhost ~]# 
[root@localhost ~]# 
[root@localhost ~]# python mytest.py --gender other
usage: mytest.py [-h] [-g {male,female}]
mytest.py: error: argument -g/--gender: invalid choice: 'other' (choose from 'male', 'female')
[root@localhost ~]#
```
<a name="eXcCb"></a>
## 6、指定文件
经常会有那种要在脚本中指定配置文件或者其他文件的需求。可以使用下面的配置
```python
import argparse
parser = argparse.ArgumentParser()

parser.add_argument('--file', '-f', action='append',
                    dest='files',
                    help=('additional yaml configuration files to use'),
                    type=argparse.FileType('rb'))

args = parser.parse_args()
```
`dest=files`，是说将命令行中，`--file` 的参数值赋值给变量files，可以用`args.files`访问。<br />`action=append`，由于会有指定多个文件的需求，那就指定多次`--file` ，`argparse`会将其放在一个list里。<br />`type=argparse.FileType('rb')`，既然是指定文件，那么参数应该为路径，并指定打开模式为rb，如果如果要取得文件内容，可以用 `args.files[0].read()`
<a name="ymyhN"></a>
## 7、子解析器
如果对命令行，有过足够多的接触，就会知道有些情况下会有子解析器。<br />这里来举个例子。
```bash
cloud-init --debug single -name mymodule
```
其中 single 后面是一个子解析器。
```python
# cloud-init.py

def main_single(name, args):
    print("name: ", name)
    print("args: ", args)
    print("I am main_single")

# 添加一个子解析器
subparsers = parser.add_subparsers()

parser_single = subparsers.add_parser('single',
                      help='run a single module')

# 对single 子解析器添加 action 函数。
parser_single.set_defaults(action=('single', main_single))

# require=True: 命令行指定了single解析器，必须带上--name的参数。
parser_single.add_argument("--name", '-n', action="store",
                           help="module name to run",
                           required=True)

args = parser.parse_args()

(name, functor) = args.action
if name in ["single"]:
    functor(name, args)
```
执行命令`cloud-init single -name mymodule`，输出如下
```bash
name:  single
args:  Namespace(action=('single', <function main_single at 0x0000000003F161E0>), debug=False, file=None, name='mymodule')
I am main_single
```
以上，就是 argparse 的使用方法
