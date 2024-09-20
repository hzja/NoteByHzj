Linux Shell  
<a name="TBizO"></a>
### 语法检查
利用工具对脚本进行检查，能最大程度地发现shell脚本中存在的语法错误。
<a name="EBi8E"></a>
### 脚本失败时即退出
可以在脚本的开头设置如下：
```bash
set -e
```
举个例子：
```bash
#！/bin/bash
set -e
lp    #这里运行会出错
date
```
这种情况下，运行一旦出错就会退出，不放过一个问题：
```bash
$ ./test.sh
lp: Error - no default destination available.
```
这也有不好的地方，有时候命令执行本来就可能是失败的，还是希望它继续，可以临时加上|| true：
```bash
#！/bin/bash
set -e
lp  || true  
date
```
很多时候就是需要处理不同的错误情况，而这样只能要么遇到错误退出，要么认为正确，导致无法走到异常分支。<br />通过set +e设置回来：
```bash
set -e
#command
set +e
#other command
```
<a name="0xX7D"></a>
### 打印脚本执行过程
调试阶段，想知道整个过程是执行了哪些命令，每条命令都具体执行了什么，可以利用下面的方式执行：
```bash
sh -x test.sh
```
或者，和上面类似，在开头加上set -x：
```bash
#！/bin/bash
set -x
if [ $# -lt 1 ]
then
   echo  "no para"
else
   echo "para 1 $1"
fi
```
执行时，输出如下：
```bash
+ [ 0 -le 1 ]
+ echo no para
no para
```
前面带+的内容就是命令实际执行的，可以看到比较条件是什么，变量被展开成了具体内容，走到了哪个分支，非常清楚。
<a name="K7Nlb"></a>
### 显示未定义的变量
shell中变量没有定义，仍然是可以使用的，但是它的结果可能不是所预期的。举个例子：
```bash
if [ "$var" = "abc" ]
then
   echo  " not abc"
else
   echo " abc "
fi
```
这里本来想判断var的内容是否为abc，实际上var并没有定义，但是在这里使用并没有报错，如果想早点发现这类问题，避免在复杂的脚本中问题被掩盖，那么可以在开头加上：
```bash
set -u
```
再次运行就会提示：
```bash
test.sh: 5: test.sh: num: parameter not set
```
再想象一下，本来想删除：
```bash
rm -rf $dir/*
```
然后dir是空的时候，变成了什么？是不是有种后背发凉的感觉？😏😏😏
<a name="sMWuw"></a>
### 管道命令一个失败时整个失败
有时候可能会执行类似这样的命令：
```bash
cat test.sh |grep if | cut -d ';' -f 2
```
三条命令一行执行，如果希望在其中一条失败，整个命令就失败，而避免执行后面无意义的命令，那么可以在开始设置：
```bash
set -o pipefail
```
不设置的情况下，cat test.sh即使执行失败了，后面的grep实际上还会继续执行，可能会导致一些意想不到的情况发生，如果不想这样的情况发生，那么这样设置是有帮助的。
<a name="WplQt"></a>
### 对于静态变量使用readonly
通常会在脚本开头定义一些静态变量：
```bash
MY_PATH=/usr/bin
```
而为了避免MY_PATH被意外修改，可以这样：
```bash
readonly MY_PATH=/usr/bin
```
这样的话，一旦后面有命令尝试修改，就会报错。
```bash
#!/bin/bash
readonly MY_PATH=/usr/bin
MY_PATH=/usr/local/bin
```
运行一下试试：
```bash
$ ./test.sh
test.sh: 3: test.sh: MY_PATH: is read only
```
<a name="ec31d6cb"></a>
### 给变量设置可选的初始值
例如：
```bash
name=${1:-Fcant}
echo "${name}"
```
这里让name为$1，即第一个参数，而当它为空时，令name为Fcant。
<a name="8h4IM"></a>
### 多条命令执行使用&&
例如：
```bash
cmd0;cmd1;cmd1
```
这里如果cmd0失败了，后面的命令仍然会执行，而如果不希望后面的命令执行，可以使用：
```bash
cmd0 && cmd1 && cmd1
```
<a name="jUFBt"></a>
### 使用函数
脚本本身比较短还好，而脚本一旦变长，不使用函数，将使得脚本很难维护，可读性也很差。
