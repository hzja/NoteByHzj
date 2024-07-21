Python
<a name="uIqMT"></a>
## 1、前言
在日常编写 Python 脚本的过程中，经常需要结合命令行参数传入一些变量参数，使项目使用更加的灵活方便<br />这里罗列出构建 Python 命令行参数的 4 种常见方式<br />它们分别是：

- 内置 `sys.argv` 模块
- 内置 argparse 模块
- 内置 getopt 模块
- 第三方依赖库 click
<a name="h4ObN"></a>
## 2、`sys.argv`
构建命令行参数最简单、常见的方式是利用内置的「 `sys.argv` 」模块<br />它是将参数以一个有序的列表传入，所以在使用时传参顺序必须固定<br />因此，这种方式适用于参数个数少且场景固定的项目中
```python
import sys

if __name__ == '__main__':
    # 获取参数列表
    # 注意：sys.argv[0] 代表第一个参数，即：脚本名称「1_sys.argv.py」
    # 其他参数列表
    args = sys.argv[1:]

    # 参数个数
    args_length = len(sys.argv) if sys.argv else 0

    print("排除运行主文件参数，其他参数列表为:", args)

    print("参数总数：", args_length)

# 使用
# python3 1_sys.argv.py arg1 arg2
# 排除运行主文件参数，其他参数列表为: [arg1, arg1]
# 参数总数：3
```
需要注意的是，在脚本中通过「 sys.argv 」获取的参数列表中，第一个参数代表脚本主文件名称
<a name="dfFjO"></a>
## 3、argparse
内置的 argparse 模块可以非常轻松地编写出友好的命令行参数脚本，并且它能自动生成帮助使用手册，在用户给程序传入无效参数时能抛出清晰的错误信息<br />官方文档：[https://docs.python.org/zh-cn/3.10/library/argparse.html](https://docs.python.org/zh-cn/3.10/library/argparse.html)<br />使用它构建命令行参数包含 3 个步骤：

1. 构建参数解析对象添加参数
2. 添加参数
3. 解析参数
<a name="ec22p"></a>
### 3-1  构建命令行参数解析对象
```python
import argparse

...
# 构建一个命令行参数解析对象
parser = argparse.ArgumentParser(description='命令行参数')
...
```
<a name="Sk1rU"></a>
### 3-2  添加命令行参数
需要指出的是，`--arg1` 用于指定参数名称，`-a1` 代表 dest 目标变量值（简写）
```python
...
# 设置3个参数
# 参数1：arg1，整型，非必须传入参数
# 参数2：arg2，字符串，非必须传入参数，包含默认值「xag」
# 参数3：arg3，字符串类型，必须传入的参数
parser.add_argument('--arg1', '-a1', type=int, help='参数1，非必须参数')
parser.add_argument('--arg2', '-a2', type=str, help='参数2，非必须参数,包含默认值', default='xag')
parser.add_argument('--arg3', '-a3', type=str, help='参数3，必须参数', required=True)
...
```
<a name="tFBXR"></a>
### 3-3  解析出参数值
```python
...
# 解析参数,获取所有的命令行参数（Namespace），然后转为字典
args = vars(parser.parse_args())

# 获取所有参数
print("所有命令行参数为:")
for key in args:
    print(f"命令行参数名:{key}，参数值:{args[key]}")
...
```
使用的时候，就可以通过「 -h / --help 」命令参数查看帮助文档
```python
# 查看命令行参数帮助文档
python3 2_argparse.py --help
# 或
python3 2_argparse.py -h
```
上面添加的 3 个参数，参数 arg1、arg2 非必须传值，arg3 必须传值，另外参数 arg2 指定了默认值
```python
# 参数arg3必须，参数arg1和参数arg2非必须，其中参数arg2设置了默认值

# 传入参数arg3
# python3 2_argparse.py --arg3 123
# python3 2_argparse.py -a3 123 
xingag@xingagdeMacBook-Pro args % python3 2_argparse.py -a3 123  
所有命令行参数为:
命令行参数名:arg1，参数值:None
命令行参数名:arg2，参数值:xag
命令行参数名:arg3，参数值:123

# 传入参数arg1、arg3
xingag@xingagdeMacBook-Pro args % python3 2_argparse.py -a1 111  -a3 123  
所有命令行参数为:
命令行参数名:arg1，参数值:111
命令行参数名:arg2，参数值:xag
命令行参数名:arg3，参数值:123

# 传入参数arg1、arg2、arg3
xingag@xingagdeMacBook-Pro args % python3 2_argparse.py -a1 111 -a2 222  -a3 123
所有命令行参数为:
命令行参数名:arg1，参数值:111
命令行参数名:arg2，参数值:222
命令行参数名:arg3，参数值:123
```
<a name="Dn3Am"></a>
## 4、getopt
getopt 是 Python 中一个内置标准模块，可以结合 sys.argv 模块，直接解析脚本运行时参数<br />使用格式：`getopt(args,shortopts,longopts = [])`<br />其中

- args参数列表，可以通过 sys.argv 获取，可以参考上面
- shortopts 短参数组成的字符串定义短参数，如果参数有值，需要在短参数后追加一个「 : 」符号传参使用方式：`-n 23`
- longopts 长参数列表定义长参数列表时，如果某个参数有值，需要在短参数后追加一个「 = 」符号传参使用方式：`--port 8080`

这里以传入数据库连接参数为例（参数包含 ip、用户名、密码、数据库名称）进行讲解
```python
from getopt import getopt
import sys

# 获取参数
# sys.argv[1:]：获取除脚本文件名外的所有命令行参数
# opts：存有所有选项及其输入值的元组列表
# args：去除有用的输入以后剩余的部分
opts, args = getopt(sys.argv[1:], 'i:u:p:d:', ['ip=', 'user=', 'pwd=', 'db='])

# 获取参数值
# 短参数
# python3 4_getopt.py -i 127.0.0.1 -u root -p 123456 -d mysqldb
# 长参数
# python3 4_getopt.py --ip 127.0.0.1 -u root -p 123456 -d mysqldb
ip_pre = [item[1] for item in opts if item[0] in ('-i', '--ip')]
ip = ip_pre[0] if len(ip_pre) > 0 else None
print("参数ip：", ip)

user_pre = [item[1] for item in opts if item[0] in ('-u', '--user')]
user = user_pre[0] if len(user_pre) > 0 else None
print("参数user：", user)

pwd_pre = [item[1] for item in opts if item[0] in ('-p', '--pwd')]
pwd = pwd_pre[0] if len(pwd_pre) > 0 else None
print("参数pwd：", pwd)

db_pre = [item[1] for item in opts if item[0] in ('-d', '--db')]
db = db_pre[0] if len(db_pre) > 0 else None
print("参数db：", db)
```
在运行脚本时，可以利用「 短参数 」或「 长参数 」形式进行传递
```python
# 测试
# 短参数
python3 4_getopt.py -i 127.0.0.1 -u root -p 123456 -d mysqldb

# 长参数
python3 4_getopt.py --ip 127.0.0.1 --user root --pwd 123456 --db mysqldb
```
<a name="NE1Tb"></a>
## 5、click
click 作为第三方依赖库，它为命令行工具封装了大量的方法，可以非常方便实现命令行参数<br />项目地址：[https://pypi.org/project/click/](https://pypi.org/project/click/)<br />首先，需要安装依赖库 click
```python
import click

# 安装依赖
# pip3 install -U click

@click.command()
@click.option('--arg1', default='111', help='参数arg1，默认值为「111」')
@click.option('--arg2', type=int, help='参数arg2')
@click.option('--arg3', type=str, help='参数arg3')
def start(arg1, arg2, arg3):
    """
    基于参数arg1、参数arg2、参数arg3运行项目
    :param arg1:
    :param arg2:
    :param arg3:
    :return:
    """
    print("参数arg1值为:", arg1)
    print("参数arg2值为:", arg2)
    print("参数arg3值为:", arg3)


if __name__ == '__main__':
    start()
```
然后，在主运行函数上通过装饰器配置参数即可<br />比如，上面就定义了 3 个命令行参数 arg1、arg2、arg3，并设置了参数的类型及默认值<br />最后，在运行脚本时只需要按参数设定传递对应的值即可
```python
# 不包含参数
xingag@xingagdeMacBook-Pro args % python3 3_click.py 
参数arg1值为: 111
参数arg2值为: None
参数arg3值为: None

# 包含参数arg1
xingag@xingagdeMacBook-Pro args % python3 3_click.py --arg1 test1
参数arg1值为: test1
参数arg2值为: None
参数arg3值为: None

# 包含参数arg1、arg2
# 注意：arg2的参数类型为整形，传入的参数必须能转为整形，否则会报错
xingag@xingagdeMacBook-Pro args % python3 3_click.py --arg1 test1 --arg2 222  
参数arg1值为: test1
参数arg2值为: 222
参数arg3值为: None

# 包含参数arg1、arg2、arg3
xingag@xingagdeMacBook-Pro args % python3 3_click.py --arg1 test1 --arg2 222 --arg3 test3
参数arg1值为: test1
参数arg2值为: 222
参数arg3值为: test3
```
<a name="zwHvn"></a>
## 6、最后
上面通过实例讲解了实现 Python 命令行参数常见的 4 种方式<br />对于一些简单自用且不那么正规的场景，可以考虑用 `sys.argv` 模块快速构建命令行参数；而对一些参数多、面向用户的正式生产环境，建议利用 argparse 模块或 click 依赖来创建命令行参数
