Linux<br />Nigel's Monitor (nmon) 是一个系统性能监控工具，其最初由 IBM 为 AIX 操作系统研发，后来被移植到 Linux 中（在多个 CPU 架构上运行）。<br />nmon 允许在一个简洁的界面中来监控系统的各个方面，如 CPU 利用率、内存、磁盘繁忙度、网络利用率等。这是它的一个主要的好处。假如没有 nmon，必须使用专门的监控工具，如 top（监控进程）、iostat（监控磁盘）和 ifstat（监控网络）来监控各种资源。这其中的每一个呈现数据的方式都是截然不同的。<br />大约 10 年前，作为 AIX 系统管理员第一次使用 nmon。通过使用 nmon，可以解决一些难以发现的问题，包括最初的那个看起来像似网络瓶颈的问题。通过使用 nmon 来同时检查多个资源，之后发现这是由于打开太多文件导致的一个应用程序问题。<br />除了交互式监控系统外，还可以在批处理模式下使用 nmon 来收集和存储性能数据以供分析。有关用于 Linux 的 nmon 的更多信息，请参阅该项目的官方网站。<br />本文着眼于这个强大的监控工具，从安装它开始。
<a name="XtJPJ"></a>
## 安装 nmon
nmon 在许多 Linux 发行版的默认存储库中都是可用的。例如，可以在 Fedora35 上使用 DNF 来安装它：
```bash
$ sudo dnf -y install nmon
```
对于红帽企业版 Linux 或 CentOS，nmon 在 EPEL(Extra Packages for Enterprise Linux) 的存储库中可用。<br />要安装 nmon，首先需启用 EPEL：
```bash
$ sudo dnf -y install \
https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm

$ sudo dnf repolist
```
接下来使用 dnf 安装 nmon：
```bash
$ sudo dnf -y install nmon
```
现在已经安装了 nmon，使用它来监控系统吧！
<a name="iLtqA"></a>
## 以交互的方式使用 nmon
安装完应用程序后，可以运行 nmon。之后会看到一个包含系统信息和一个小帮助菜单的初始屏幕，它显示了可以按哪些键来切换不同的指标：
```bash
$ nmon

┌─16k──[H for help]──Hostname=f35ks01─Refresh= 2secs ─18:04:27────┐
│                                                                 │
│------------------------------                                   │
│ _ __  _ __ ___   ___  _ __    For help type H or ...            │
│| '_ \| '_ ` _ \ / _ \| '_ \    nmon -?  - hint                  │
│| | | | | | | | | (_) | | | |   nmon -h  - full details          │
│|_| |_|_| |_| |_|\___/|_| |_|                                    │
│                               To stop nmon type q to Quit       │
│------------------------------                                   │
│                                                                 │
│Fedora release 35 (Thirty Five) VERSION="35 (Thirty Five)"       │
│Vendor=AuthenticAMD Model=AMD EPYC-Milan Processor               │
│MHz=1911.004 bogomips=3822.00     lscpu:CPU=4 Little Endian      │
│ProcessorChips=4 PhysicalCores=1        Sockets=4 Cores=1 Thrds=1│
│                 VirtualCPUs =4         MHz=0 max=0 min=0        │
│                                                                 │
│Use these keys to toggle statistics on/off:                      │
│  c = CPU         l = CPU Long-term     - = Faster screen updates│
│  C = " WideView  U = Utilisation       + = Slower screen updates│
│  m = Memory      V = Virtual memory    j = File Systems         │
│  d = Disks       n = Network           . = only busy disks/procs│
│  r = Resource    N = NFS               h = more options         │
│  k = Kernel      t = Top-processes     q = Quit                 │
│─────────────────────────────────────────────────────────────────│
```
例如，按 **C** 键会切换为 CPU 利用率显式：
```bash
┌─16k──[H for help]──Hostname=f35ks01─Refresh= 2secs ─18:05:58─┐
│ CPU Utilisation ─────────────────────────────────────────────│
│---------------------------+---------------------------------+│
│CPU User%  Sys% Wait%  Idle|0          |25   |50 |75   100|   │
│  1   3.6  17.2   0.0  79.2|Ussssssss>                    |   │
│  2   2.1   6.2   0.0  91.7|Usss    >                     |   │
│  3   0.0   1.1   0.0  98.9| >                            |   │
│  4   6.1  17.2   0.0   0.5|UUUssssssss>                  |   │
│---------------------------+---------------------------------+│
│Avg   3.0  10.3   0.0  86.7|Usssss>                       |   │
│---------------------------+---------------------------------+│
│──────────────────────────────────────────────────────────────│
```
按 **N** 键会切换为网络显示，并自动将其堆叠在 CPU 显示的下方：
```bash
┌─16k──[H for help]──Hostname=f35ks01─Refresh= 2secs ─18:17:01─┐
│ CPU Utilisation ─────────────────────────────────────────────│
│---------------------------+---------------------------------+│
│CPU User%  Sys% Wait%  Idle|0         |25    |50  |75  100|   │
│  1   2.6  10.3   0.0  87.1|Usssss     >                  |   │
│  2   0.0   2.2   0.0  97.8|s           >                 |   │
│  3   6.5  11.0   0.0  82.5|UUUsssss>                     |   │
│  4   0.0   0.5   0.0  99.5|               >              |   │
│---------------------------+---------------------------------+│
│Avg   2.4   5.9   0.0  91.6|Uss     >                     |   │
│---------------------------+---------------------------------+│
│ Network I/O ─────────────────────────────────────────────────│
│I/F Name Recv=KB/s Trans=KB/s p_in p_out in_sz out_sz Peak->..│
│      lo      0.0       0.0       0.0    0.0     0.0    0.0...│
│  enp1s0      0.0       0.0       0.5    0.0    52.0    0.0...│
│  enp7s0    218.6    5923.4    3335.5 9989.68   67.1  607.2...│
│ Network Error Counters ──────────────────────────────────────│
│I/F Name iErrors iDrop iOverrun iFrame oErrors   oDrop oOver..│
│      lo       0       0       0       0       0       0   ...│
│  enp1s0       0   34419       0       0       0       0   ...│
│  enp7s0       0   34419       0       0       0       0   ...│
│──────────────────────────────────────────────────────────────│
```
继续按下其他按键可在同一个视图中堆叠其他指标。例如，可使用 **D** 键来显式**磁盘 I/O**，可使用 **M** 键来显式**内存**信息，可使用 **T** 键来显式**Top 进程**。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650848969757-86abf221-c435-41ac-bd48-510a58b0e861.png#clientId=udd3f1cea-f16b-4&from=paste&id=ufa909347&originHeight=1956&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufc847f25-5b01-49f0-accc-b8f7352d43e&title=)<br />除了堆叠新显示外，还可以按**点（.）**键仅显示繁忙的 CPU 和磁盘，也可以 **+** 和 **-** 键来增加或减少屏幕刷新时间。使用 **H** 键显示帮助。<br />由于可以随时**启用**和**禁用**不同的监视器和指标，所以 nmon 是一个可以了解系统行为的高度灵活的工具。它还可以通过同时显示不同的指标来帮助解决问题，通过提供系统的整体视图以提高理解问题的能力。<br />完成后，按 **Q** 键退出并结束监控会话。
<a name="EVmEQ"></a>
## 收集性能数据
除了提供系统的整体交互式视图外，还可以在批处理模式下运行 nmon，这样就可以从系统中收集长时间的性能数据以进行后验分析。<br />除了交互模式提供的实时指标外，此功能还允许了解系统在较长时间内的行为。这是一项强大的功能，可以通过它进行容量规划，并有机会解决难以实时发现的零星问题。<br />要在批处理模式下运行 nmon，请在启动应用程序时使用标志 `-f` 参数（它必须是提供的第一个参数）。默认情况下，在一天内，nmon 每五分钟收集一次数据。可以用参数 `-s` 来指定收集间隔和参数 `-c` 来指定快照数量。<br />例如，开启 nmon 批处理模式，想在一小时内，每分钟收集数据一次，同时包括捕获顶级进程 `-t`，执行以下命令：
```bash
$ nmon -f -s60 -c60 -t
```
**终端提示符**将会立即返回，因为当执行此命令时应用程序正在后台运行。可以接着使用 ps 命令，然后就可以看到它正在运行了：
```bash
$ ps -ef | grep nmon
ricardo 1849 1 0 12:27 pts/0 00:00:00 nmon -f -s60 -c60 -t
```
在批处理模式下，nmon 使用**逗号分隔值** (Comma-Separated Value, CSV) 格式将收集的数据保存在当前目录中，其扩展名为 .nmon：
```bash
$ ls -l
total 36
-rw-r--r--. 1 ricardo ricardo 36395 Feb 13 12:28 f35ks01_220213_1227.nmon
```
由于这是一个 CSV 文件，因此可以使用其他 Linux 文本操作（命令行）工具对其进行处理，或者使用电子表格软件将其打开，以进行进一步分析。
```bash
$ head f35ks01_220213_1227.nmon
AAA,progname,nmon
AAA,command,nmon -f -s60 -c60 -t
AAA,version,16k
AAA,disks_per_line,150
AAA,max_disks,256,set by -d option
AAA,disks,10,
AAA,host,f35ks01
AAA,user,ricardo
AAA,OS,Linux,5.16.8-200.fc35.x86_64,#1 SMP PREEMPT Tue Feb 8 20:58:59 UTC 2022,x86_64
AAA,runname,f35ks01
```
除了这些方法之外，nmon 开发团队还提供了一个脚本，该脚本可将这些数据转换为 Web 浏览器能可视化查看的图表。接下来看看如何做到这一点。
<a name="EyoZz"></a>
## 生成图表
CSV 是一种灵活的格式，它允许使用电子表格处理软件等不同的应用程序导出、分析和查看这些数据。为了使这个过程更加容易，nmon 的开发人员创造了 nmonchart。此 shell 脚本使用 Google Charts（JavaScript）将这些数据转换为 HTML，以使其在图形环境中可视化数据并与之交互。<br />若要使用 nmoncart，请从 nmon 主页下载：
```bash
$ curl -LO http://sourceforge.net/projects/nmon/files/nmonchart40.tar
```
然后使用 tar 来提取包的内容：
```bash
$ mkdir nmonchart

$ tar xvf nmonchart40.tar -C nmonchart
```
nmonchart 需要使用 ksh（Korn SHell），因此要创建图表，首先应使用 dnf 在系统中安装 ksh：
```bash
$ sudo dnf install -y ksh
```
接下来，使用以下命令转换 .nmon 文件（该文件是在批处理模式下执行 nmon 时创建的）：
```bash
$ ./nmonchart/nmonchart f35ks01_220213_1227.nmon 
```
该脚本创建一个与源 .nmon 文件同名的 HTML 文件。或者，可以将目标文件名作为第二个参数传递给 nmonchart。这是一个例子：
```bash
$ ls -lg
total 4164
-rw-r--r--. 1 ricardo 113822 Feb 14 18:52 f35ks01_220213_1227.html
-rw-r--r--. 1 ricardo 93436 Feb 13 13:26 f35ks01_220213_1227.nmon
drwx------. 2 ricardo 4096 Aug 28 2020 nmonchart
```
系统图表现已准备就绪。下一步是查看它们。
<a name="Vaa0I"></a>
## 可视化性能图表
nmonchart 提供的每个 .nmon 源文件创建一个 HTML 文件，其中嵌入了所需的图表 JavaScript 库。如果本地计算机上有 HTML 文件，请直接在 Web 浏览器中查看它。<br />还可以使用 Web 服务器提供这些文件。最喜欢的方法之一是在 Podman 容器中使用 Nginx，正如在将 20 个单行 Linux 命令添加到工具箱中所建议的那样。<br />首先，创建一个目录并将 HTML 文件移到那里：
```bash
$ mkdir html

$ mv f35ks01_220213_1227.html html/
```
然后，在 8080 端口上使用 Podman 去服务来自于 Ngnix 的 html 目录中的内容：
```bash
$ podman run --rm -v ./html:/usr/share/nginx/html:ro,Z -p 8080:80 -d nginx
```
如果需要的话，请在服务器防火墙上打开 8080 端口以允许流量通过：
```bash
$ sudo firewall-cmd --add-port 8080/tcp 
```
然后，为了查看图标，请将浏览器指向 URL http://<YOUR_SERVER_IP_ADDR>:8080/f35ks01_220213_1227.html：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650848969795-6d794208-12b8-4f75-be41-1be04c02c7b1.png#clientId=udd3f1cea-f16b-4&from=paste&id=ufa07b0b3&originHeight=468&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c259c9b-9fcc-4bc2-8d5e-f2b67dca9cf&title=)<br />单击屏幕顶部的任何按钮以查看相应的图表（图表是交互式的）。可以放大和缩小以查看更精细的数据，或将鼠标悬停在图表上以查看给定时间的详细数据。
<a name="pGXXc"></a>
## 总结
nmon 是一款灵活且功能强大的系统监控应用程序，更好地了解系统，并在出现问题时及时地对其进行故障排除。它结合了监控能力、整合来自不同系统资源的实时数据的能力，以及长期性能数据收集和分析的能力。这使其成为系统管理员的绝佳工具。
