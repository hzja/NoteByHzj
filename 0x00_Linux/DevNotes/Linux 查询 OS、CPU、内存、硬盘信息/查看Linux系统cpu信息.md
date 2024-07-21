Linux<br />CPU也称为微处理器或简称为处理器。就像大脑如何控制人体一样，CPU 控制着计算机的所有部分。因此CPU被认为是计算机的大脑。那怎么在Linux系统中查看如 Intel Core i3、i5、AMD 等品牌信息和其他细节如处理器速度、内核数量、架构、缓存大小等呢?
<a name="n4rBz"></a>
### 方法 1- 检查 CPU 信息使用lscpu
lscpu命令行应用程序从 sysfs 和特定于架构的库(如/proc/cpuinfo。该lscpu命令收集 CPU 详细信息，例如 CPU、线程、内核、套接字和非统一内存访问(NUMA)节点的数量。它还检索有关 CPU 缓存和缓存共享、系列、型号、bogoMIPS、字节顺序和步进等的信息。lscpu 命令是util-linux包的一部分，所以不需要安装。<br />要使用命令在 Linux 中查找 CPU 信息lscpu
```bash
> lscpu

Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              4
On-line CPU(s) list: 0-3
Thread(s) per core:  2
Core(s) per socket:  2
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               85
Model name:          Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
Stepping:            7
CPU MHz:             2500.002
BogoMIPS:            5000.00
Hypervisor vendor:   KVM
Virtualization type: full
L1d cache:           32K
L1i cache:           32K
L2 cache:            1024K
L3 cache:            36608K
NUMA node0 CPU(s):   0-3
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss ht syscall nx pdpe1gb rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq monitor ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single pti fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx avx512f avx512dq rdseed adx smap clflushopt clwb avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves arat avx512_vnni
```
<a name="aCvWH"></a>
#### 显示lscpu在 Linux 中使用的特定 CPU 信息
lscpu会输出 CPU 的完整信息。可以用grep搜索指定的字段信息，例如model name
```bash
> lscpu | grep -i 'Model name'
Model name:          Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
```
如果只想显示处理器型号
```bash
> lscpu | grep -i "Model name:" | cut -d':' -f2- -
                      Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
```
还可以找到其他 CPU 详细信息，例如CPU 核心数，如下所示：
```bash
> lscpu | grep -i "CPU(s)"
CPU(s):              4
On-line CPU(s) list: 0-3
NUMA node0 CPU(s):   0-3
```
<a name="wj4dZ"></a>
### 方法 2 - 在 Linux 中使用/proc/cpuinfo文件查找 CPU 信息
lscpu从文件/proc/cpuinfo中收集处理器详细信息。因此，可以cat来检查 Linux 中的 CPU 详细信息。
```bash
cat /proc/cpuinfo
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662697746357-89000c9e-f857-490f-9bd9-0e9f896bfd23.png#clientId=ua6287719-453d-4&from=paste&height=629&id=u4f226633&originHeight=1573&originWidth=2167&originalType=binary&ratio=1&rotation=0&showTitle=false&size=192918&status=done&style=none&taskId=u8472eeb3-cc28-48d0-a405-e53b6175602&title=&width=866.8)<br />还可以使用命令从/proc/cpuinfo文件中过滤特定的 CPU 详细信息
```bash
cat /proc/cpuinfo | grep "cpu family"
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662697694518-7d154f45-2a27-4fd9-b930-8a578fca6a52.png#clientId=ua6287719-453d-4&from=paste&height=106&id=ub812ef86&originHeight=266&originWidth=1619&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23647&status=done&style=none&taskId=u45658cd1-b5f8-49a0-ae88-80a83434de1&title=&width=647.6)
<a name="iBquq"></a>
### 方法 3 - 查看处理器信息使用lshw
Lshw (Hardware Lister) 是一个功能齐全的 CLI 应用程序，可提供有关 Linux 系统硬件配置的详细信息。它可以获取硬件详细信息，例如确切的内存配置、固件版本、主板配置、CPU 版本和速度、缓存配置、总线速度等。<br />lshw 应用程序有命令行和图形界面两种操作方式。它预装在大多数 Linux 发行版上。如果需要安装可以执行下面的命令。
```bash
sudo apt install lshw lshw-gtk
```
要查看处理器信息lshw
```bash
sudo lshw -class processor
```
如果只想显示处理器型号，使用`-short`参数。
```bash
> sudo lshw -short -class processor
H/W path        Device     Class          Description
=====================================================
/0/400                     processor      Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
```
lshw还有一个图形界面。通过运行以下命令从应用程序菜单或终端启动lshw gui：
```bash
sudo lshw-gtk
```
双击便携式计算机 -> 主板 -> CPU 以获取 CPU 的完整详细信息。
<a name="aCiOy"></a>
### 方法 4 - 使用获取处理器详细信息dmidecode
dmidecode应用程序从 DMI表中获取系统硬件组件的详细信息。它将首先尝试从 中读取 DMI 表sysfs，然后如果sysfs访问失败则尝试直接从内存中读取。找到 DMI 表后，dmidecode 将解析该表并以人类可读的格式显示系统的硬件信息。Dmidecode 预装在大多数 Linux 操作系统中，可以直接使用。<br />要在 Linux 中使用dmidecode 获取CPU详细信息
```bash
sudo dmidecode --type processor
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662697628960-f291529f-3fc4-4aa3-b742-45e77a412ca2.png#clientId=ua6287719-453d-4&from=paste&height=630&id=u0755692a&originHeight=1575&originWidth=1856&originalType=binary&ratio=1&rotation=0&showTitle=false&size=153937&status=done&style=none&taskId=u79a9cabd-b899-4dbe-87d9-f9994240900&title=&width=742.4)
<a name="wj7hO"></a>
### 方法 5 - 查看 CPU 信息使用inxi
Inxi 是一个命令行系统信息工具，用于显示系统信息，例如 CPU、驱动程序、xorg、桌面环境、内核、GCC 版本、进程、RAM 使用情况以及许多其他有用的信息。<br />Inxi 在许多 Linux 和 Unix 操作系统的默认存储库中可用。需要安装的话可以执行下面的命令。
```bash
sudo apt install inxi
```
要使用inxi 显示处理器详细信息，使用`-C`或`--cpu`参数：`$ inxi -C` 或者`$ inxi --cpu`<br />显示的信息包括内核数量、CPU 型号、CPU 缓存、CPU 时钟速度、CPU 最小/最大速度等。
```bash
CPU:       Dual core Intel Xeon Platinum 8269CY (-MT-MCP-) cache: 36608 KB
           clock speeds: max: 2500 MHz 1: 2500 MHz 2: 2500 MHz 3: 2500 MHz 4: 2500 MHz
```
<a name="IQke1"></a>
### 方法 6 - 使用打印 CPU 信息hardinfo
HardInfo是一个图形应用程序，用于收集有关系统硬件和操作系统的信息，并生成HTML或纯文本格式的可打印报告。<br />Hardinfo 在默认存储库中可用，因此可以使用发行版的默认包管理器安装。在 Debian、Ubuntu 上，运行以下命令来安装 Hardinfo：
```bash
sudo apt install hardinfo
```
安装 hardinfo 后，从 dash 或 menu 启动它。点击Processor选项下的Devices左侧窗格中的部分以获取 CPU/处理器详细信息。
<a name="iMhK2"></a>
### 方法 7 - 使用hwinfo
Hwinfo 是另一个用于查找系统硬件信息的命令行应用程序。它探测 Linux 系统中存在的硬件并显示每个硬件设备的详细信息。<br />执行下面的命令进行安装
```bash
sudo apt install hwinfo
```
要使用hwinfo 查看CPU信息
```bash
sudo hwinfo --cpu
```
只打印 CPU 的简短摘要
```bash
sudo hwinfo --short --cpu
```
<a name="Cz2Gu"></a>
### 方法 8 - 使用cpuid
Cpuid转储从 CPUID 指令收集的有关 CPU 的详细信息，并根据该信息确定 CPU 的确切型号。<br />执行下面命令进行安装
```bash
sudo apt install cpuid
```
在 Fedora 等基于RPM的系统上：
```bash
sudo dnf install cpuid
```
安装 cpuid 后，使用命令获取处理器信息：
```bash
cpuid
```
<a name="iruio"></a>
### 方法 9 - 使用nproc
Nproc是 GNU coreutils 的一部分。它打印的CPU信息很少。它只打印当前进程可用的处理单元数。
```bash
> nproc 
4
```
处理单元的数量可能少于在线处理器的数量。要打印已安装处理器的数量，请使用`--all`标志：
```bash
nproc --all
```
<a name="lJjtT"></a>
### 方法 10 - 使用hwloc
hwloc 应用程序提供命令行工具和 C API接口来获取CPU信息

- NUMA 内存节点，
- 共享缓存，
- 处理器包，
- 处理器内核，
- 和处理器线程。

确保已在系统上安装 hwloc：
```bash
sudo apt install hwloc
```
要显示 Linux CPU 详细信息，请运行：
```bash
hwloc-ls
```
