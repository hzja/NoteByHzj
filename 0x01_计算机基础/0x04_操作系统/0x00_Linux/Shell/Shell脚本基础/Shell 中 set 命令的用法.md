Linux Shell <br />在使用操作系统的时候，会了解到系统中的set命令的主要作用或者说是大部分作用就是显示系统中已经存在的shell变量，以及设置shell变量的新变量值。使用set更改shell特性时，符号`+`和`-`的作用分别是关闭和打开指定的模式。set命令不能够定义新的shell变量，如果要定义新的变量，可以使用`declare`命令以变量名=值的格式进行定义即可。
<a name="IXMbR"></a>
### 命令选项
| 选项 | 含义解释 |
| --- | --- |
| -a | 标示已修改的变量，以供输出至环境变量 |
| -b | 使被中止的后台程序立刻回报执行状态 |
| -C | 转向所产生的文件无法覆盖已存在的文件 |
| -d | Shell预设会用杂凑表记忆使用过的指令，以加速指令的执行。使用-d 参数可取消 |
| -e | 若指令传回值不等于0，则立即退出shell |
| -f | 取消使用通配符 |
| -h | 自动记录函数的所在位置 |
| -H Shell | 可利用!加<指令编号>的方式来执行history中记录的指令 |
| -k | 指令所给的参数都会被视为此指令的环境变量 |
| -l | 记录for循环的变量名称 |
| -m | 使用监视模式 |
| -n | 只读取指令，而不实际执行 |
| -p | 启动优先顺序模式 |
| -P | 启动-P参数后，执行指令时，会以实际的文件或目录来取代符号连接 |
| -t | 执行完随后的指令，即退出shell |
| -u | 当执行时使用到未定义过的变量，则显示错误信息 |
| -v | 显示shell所读取的输入值 |
| -x | 执行指令后，会先显示该指令及所下的参数 |

<a name="cpVJ2"></a>
### 定义变量
使用`declare`命令定义一个新的环境变量tools。
```bash
# 定义新环境变量
$ declare tools='VScode'
```
再使用`set`命令将新定义的变量输出为环境变量。
```bash
# 设置为环境变量
$ set -a tools
```
执行该命令后，将会新添加对应的环境变量。用户可以使用`env`命令和`grep`命令分别显示和搜索环境变量`tools`。
```bash
# 显示环境变量值
$ env | grep tools
```
此时，该命令执行后，将输出查询到的环境变量值。
<a name="DztpZ"></a>
### 使用 `set`/`env`/`export` 的区别
| 选项 | 含义解释 |
| --- | --- |
| set | 显示/设置shell变量，包括的用户变量以及私有变量；不同类的shell有不同的私有变量，bash/ksh/csh每中shell私有变量都不一样 |
| env | 显示/设置用户变量 |
| export | 显示/设置当前导出成用户变量的shell变量 |

```bash
[escape@localhost ~]$ name="EscapeWen"
[escape@localhost ~]$ echo $name
EscapeWen

# 设置完当前用户变量并没有对应的值
[escape@localhost ~]$ env | grep name

# 设置完成后shell变量有对应的值
[escape@localhost ~]$ set | grep name
EscapeWen

# 对应的值，使用export也没导出
[escape@localhost ~]$ export| grep name

# 使用expor导出一下，在用户变量就可以找到了
[escape@localhost ~]$ export aaa
[escape@localhost ~]$ env | grep aaa
EscapeWen
```
<a name="fNyXX"></a>
### 使用注意事项
<a name="pZZx1"></a>
#### 使用`unset`命令来清除环境变量
注意`set`、`env`、`export`设置的变量，都可以用`unset`来清除的。
```bash
# 删除环境变量TEST
[escape@localhost ~]$ export TEST="Test"
[escape@localhost ~]$ unset $TEST
```
<a name="L16lr"></a>
#### 使用readonly命令，设置只读变量
```bash
# 将环境变量TEST设为只读
[escape@localhost ~]$ export TEST="Test"
[escape@localhost ~]$ readonly TEST
```
