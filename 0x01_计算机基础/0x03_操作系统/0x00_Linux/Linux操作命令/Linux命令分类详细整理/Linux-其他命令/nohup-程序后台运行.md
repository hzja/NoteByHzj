Linux nohup 
<a name="wfYyr"></a>
# nohup介绍
nohup命令的全称为“no hang up”，该命令可以将程序以忽略挂起信号的方式运行起来，被运行的程序的输出信息将不会显示到终端。<br />无论是否将 nohup 命令的输出重定向到终端，输出都将附加到当前目录的 nohup.out 文件中。如果当前目录的 nohup.out 文件不可写，输出重定向到$HOME/nohup.out文件中。如果没有文件能创建或打开以用于追加，那么command 参数指定的命令不可调用。如果标准错误是一个终端，那么把指定的命令写给标准错误的所有输出作为标准输出重定向到相同的文件描述符。
<a name="2HhcC"></a>
#  `nohup` 命令的具体用法
<a name="blNKd"></a>
## nohup命令基本语法
nohup 命令的基本语法如下：
```bash
$ nohup command arguments
```
或者：
```bash
$ nohup options
```
<a name="wdf2g"></a>
### 命令格式
```bash
nohup Command [ Arg … ] [ & ]
```
<a name="I8YFF"></a>
### 参数说明

- `Command`：要执行的命令。
- `Arg`：一些参数，可以指定输出文件。
- `&`：让命令在后台执行，终端退出后命令仍旧执行。
<a name="JpnT1"></a>
### 命令后台执行,正常输出和错误输出到a.log文件
```bash
> nohup command > a.log 2>&1 &
```
<a name="J4cxj"></a>
#### 2>&1 解释
将标准错误 2 重定向到标准输出 &1 ，标准输出 &1 再被重定向输入到 runoob.log 文件中。

- 0 – stdin (standard input，标准输入)
- 1 – stdout (standard output，标准输出)
- 2 – stderr (standard error，标准错误输出)

如果想要得到更多关于 nohup 的用法介绍，可以查看它的帮助页面：
```bash
$ nohup --help
```
如果需要查看它的版本号，可以使用 `--version` 选项。
```bash
$ nohup --version
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595912794186-7c241021-b0fa-4d6b-a5c3-2ffee395f8b6.png#height=180&id=fLHcp&originHeight=540&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=602671&status=done&style=none&title=&width=1107.6666666666667)
<a name="DSa1L"></a>
## 使用nohup命令启动一个程序
如果需要运行一个程序，即使对应的 Shell 被退出后依然保持运行，可以这样使用 nohup 运行这个程序：
```bash
$ nohup command
```
当这个程序进行起来之后，这个程序对应的 log 输出及其错误日志都将被记录在 `nohup.out` 文件里，这个文件一般位于家目录或者当前目录。
<a name="tBA49"></a>
### 后台下载文件
```bash
> nohup wget https://repo.huaweicloud.com/java/jdk/8u172-b11/jdk-8u172-linux-x64.tar.gz &
// 查看后台的进程
> jobs
[1]+  Running                 nohup wget https://repo.huaweicloud.com/java/jdk/8u172-b11/jdk-8u172-linux-x64.tar.gz &
```
下载过程中，输出在nohup.out文件中，可以使用`tail -f nohup.out`动态查看
<a name="ySx8Y"></a>
### 后台运行SpringBoot项目
```bash
> nohup java -jar rumenz.jar &
```
<a name="ymWUB"></a>
### 后台执行bash命令
```bash
> nohup bash -c 'cal && ls' > output.txt
```
<a name="dALNb"></a>
## 重定向程序日志的输出
如果不想把程序的输出保存在家目录或者当前目录，想保存在指定的路径，并且自定义文件名，这时就可以使用重定向操作 `>` 。<br />比如，现在有个脚本 `myScript.sh` 想把它的输出保存在家目录下的 `output` 目录下，文件名为 `Output.log` ，可以这样运行：<br />新建脚本myScript.sh
```shell
#!/bin/bash
echo 'Start processt'
```
为脚本授权（否则会报没权限的错误）
```bash
[root@iZuligp6e1dyzfZ bash]# chmod +x myScript.sh
```
运行
```bash
$ nohup ./myScript.sh > ./Output.log
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595914321816-2d713bff-1960-4b42-a22a-ec15294411ae.png#height=343&id=fx5YN&originHeight=1028&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1314418&status=done&style=none&title=&width=1107.6666666666667)
<a name="Zj8Pg"></a>
## 使用nohup命令后台启动一个程序
如果想让程序在后台运行，可以加上 `&` 符号。但这样运行之后，程序就无影无踪了。想要让程序重新回到终端，可以使用 `fg` 命令。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595914475636-95b07894-0729-4ea2-b821-7c5e95d7dc3a.png#height=133&id=xdeJX&originHeight=399&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=464003&status=done&style=none&title=&width=1107.6666666666667)<br />这个命令的输出 log 将保存在 nohup.out 文件里，可以使用 cat 或其它命令查看。第二行里 8699 这个数字代表这个命令对应的进程号，也就是 pid 。可以使用 `ps` 命令来找到这个进程。
<a name="C3HWO"></a>
## 使用nohup同时运行多个程序
如果需要同时跑多个程序，没必要一个个运行，直接使用 `&&` 符号即可。比如，想同时跑 mkdir ，ping，ls 三个命令，可以这样运行：
```bash
$ nohup bash -c 'mkdir files && ping -c 1 baidu.com && ls'> output.txt
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595914650488-e6ce3276-0c5d-4b32-bd67-3f64cad357ff.png#height=342&id=frdZU&originHeight=1025&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1232721&status=done&style=none&title=&width=1107.6666666666667)
<a name="NowfS"></a>
## 终止跑在后台的进程
上面有提到，`nohup` 命令结合 `&` 符号可以使进程在后台运行，即使关闭了终端依然不受影响。这时，如果想要终止这个进程，要怎么操作呢？<br />最简单的当属 `kill` 命令
```bash
$ kill -9 PID
```
使用 `ps` 命令找到进程对应的 pid 
```bash
$ ps aux | grep myScript.sh
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595914804311-94d3fb77-811f-4758-9b3b-3a19b3c742e2.png#height=72&id=i1WBw&originHeight=217&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=191399&status=done&style=none&title=&width=1107.6666666666667)<br />或者用 `pgrep` 命令也行。
```bash
$ pgrep -a myScript.sh
```
接下来，再使用 `kill` 命令就可以终止该进程了。
```bash
$ kill -9 14942
```
<a name="AxIx1"></a>
# fg–将作业放到前台运行（可以用于查看nohup后台执行的程序）
fg命令用于将后台作业（在后台运行的或者在后台挂起的作业）放到前台终端运行。全称为“ForeGround”，与bg命令一样，若后台任务中只有一个，则使用该命令时，可以省略任务号。<br />语法格式：fg [参数]<br />常用参数：

| 作业标识 | 指定要放到前台的作业标识号 |
| --- | --- |

<a name="81c7j"></a>
# bg–将作业放到后台运行
bg命令用于将作业放到后台运行，使前台可以执行其他任务。该命令的运行效果与在指令后面添加符号&的效果是相同的，都是将其放到系统后台执行。<br />语法格式：bg [参数]<br />常用参数：

| 作业标识 | 指定需要放到后台的作业标识号 |
| --- | --- |

