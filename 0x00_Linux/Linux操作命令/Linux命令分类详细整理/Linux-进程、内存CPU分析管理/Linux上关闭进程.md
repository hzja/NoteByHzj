Linux<br />想干掉Liunx某个进程怎么搞?<br />如果进程在前台运行，可以使用 Ctrl+C 终端快捷方式。但是，如果进程不可见(在后台运行)，可以使用专用命令杀死它。<br />终止进程是指在执行过程中停止进程。如果知道进程ID(PID)，则可以使用 kill 命令，如下所示：
```bash
> kill <signal> <PID>
```
在上面的语法中，signal指的是要发送终止的终止信号，并且PID指的是进程的 ID。<br />还有一个 killall 命令，主要讨论kill 和 killall 命令。
<a name="QFegJ"></a>
## Linux 中的终止信号
当一个进程被操作系统或用户终止时，即进程没有自行完成，它会被发送一个终端信号。<br />以下是可用的终止信号：

| 信号 | 数值 | 描述 |
| --- | --- | --- |
| SIGHUP | 1 | 信号挂断：当控制它的终端关闭时发送到进程。 |
| SIGINT | 2 | 信号中断：当用户终止进程时发送给进程的信号.(例如 Ctrl + X) |
| SIGKILL | 9 | Signal Kill：立即退出进程的信号，不允许它保存当前状态。 |
| SIGTERM | 15 | 信号终止：发送到信号以请求终止进程。该信号可以被进程忽略。但这是终止进程的首选方式，因为它可以在进程收到 SIGTERM 时释放资源。 |
| SIGSTOP | 19 (for x86, ARM and most others)<br />17 (for Alpha)<br />23 (for MIPS)<br />24 (for PARISC) | 停止信号：停止进程的信号，但稍后将恢复。 |

常用的信号 9 和 15
<a name="tmSzp"></a>
## 获取进程的PID
还需要了解要终止的进程的详细信息。使用 kill 命令，必须提供进程的 ID(PID)。可以从进程名称中获取 PID
```bash
> pidof exact_process_name
```
比如获取java进程的进程号
```bash
> pidof java
8075 1032
```
<a name="i7945"></a>
## 在 Linux 命令行中终止进程
<a name="bWoYT"></a>
### 使用 kill 命令
该kill命令要求知道要终止的进程的 ID，以及可选的终止信号。<br />要简单地终止命令，请使用以下语法：
```bash
kill [signal] <PID>
```
向 PID 发送终止信号是可选的，如果未提供任何信号，则kill默认发送 SIGTERM ( 15)，以正常终止所述进程结束。<br />启动了sleep 命令的后台进程（它给了一个 PID）。使用kill命令杀死它。<br />杀死这个特定的实例sleep如下所示：
```bash
$  sleep 120 &
[1] 125686

$ kill 125686
[1]  + terminated  sleep 120
```
如果想使用终止信号，可以使用数值或信号本身：
```bash
$ sleep 120 &
[1] 125746

$ kill -SIGKILL 125746
[1]  + killed     sleep 120

$ sleep 120 &
[1] 125759

$ kill -9 125759
[1]  + killed     sleep 120
```
如果 kill 命令没有指定信号，则默认使用 SIGTERM (15)。
<a name="OxRBj"></a>
### 使用 killall 命令
如果不知道某个进程的PID是多少，或者该进程有多个子进程，又想一次性杀死子进程和父进程，可以使用killall命令。
```bash
killall [signal] <process-name>
```
与kill命令类似，指定终止信号是可选的。当没有指定终止信号时，killall将发送 SIGTERM ( 15) 以优雅地关闭所述进程。<br />为了演示killall的使用，将杀死两个正在运行的 sleep 命令。
```
$ sleep 120 &
[1] 112351

$ sleep 2000 &
[2] 112362

$ killall sleep
[1]-  Terminated              sleep 120
[2]+  Terminated              sleep 2000
```
此外，可以使用该`-e`标志来查找进程名称的完全匹配。
<a name="IedLy"></a>
## 使用 pkill 命令
命令的替代命令kill是pkill命令。它是pgrep和kill命令的组合。<br />killall杀死所有具有匹配名称的进程。另一方面，pkill使用模式匹配来匹配进程并杀死它们。
```bash
pkill [options] pattern
```
命令中可用的一些有用选项pkill如下：

- `-u`：特定所有者拥有的进程
- `-x`：完全匹配模式的进程
- `-signal`：指定终止信号（默认为 SIGTERM）

结束nginx进程
```bash
> pkill nginx
```
