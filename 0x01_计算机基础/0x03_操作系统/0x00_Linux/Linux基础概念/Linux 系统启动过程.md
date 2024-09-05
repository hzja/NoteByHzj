Linux<br />每次打开 Linux PC 时，它都会经历一系列阶段，然后最终显示提示输入用户名或密码的登录屏幕。每个 Linux 发行版在典型的启动过程中都会经历 4 个不同的阶段。<br />**用户登录提示**<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671254056068-3ec8b634-d451-4c01-adda-8d6c81ee65cb.jpeg#averageHue=%23060606&clientId=u7e157960-20eb-4&from=paste&id=ue3f00f5a&originHeight=122&originWidth=439&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7249c831-08bb-4ad1-afef-a5b3952e23b&title=)<br />在本指南中，将重点介绍 Linux 操作系统从开机到登录的各个步骤。请注意，本指南仅考虑了当前使用的GRUB2引导加载程序和systemdinit 绝大多数现代 Linux 发行版 引导过程需要以下 4 个步骤，将更详细地讨论这些步骤：

- BIOS完整性检查 (POST)
- 加载引导加载程序 (GRUB2)
- 内核初始化
- 启动systemd，所有进程的父进程
<a name="JFx8U"></a>
### 1、BIOS 完整性检查 (POST)
引导过程通常在用户按下开机按钮（如果 PC 已经关闭）或使用 GUI 或命令行重新启动系统时初始化。<br />当 Linux 系统启动时，BIOS（基本输入输出系统）启动并执行开机自检(POST)。这是执行大量诊断检查的完整性检查。<br />该POST探测部件，如硬件操作性HDD或SSD，键盘，RAM，USB端口，和任何其他的硬件。如果未检测到某些硬件设备，或者任何设备出现故障，例如损坏的 HDD 或 SSD，屏幕上会出现一条错误消息，提示您进行干预。<br />在某些情况下，特别是在缺少 RAM 模块的情况下，会发出哔哔声。但是，如果预期的硬件存在并且按预期运行，则引导过程将进入下一阶段。
<a name="LmF3i"></a>
### 2、引导加载程序 (GRUB2)
一旦POST完成并且停止运行，BIOS 就会探测MBR（主引导记录）以获取引导加载程序和磁盘分区信息。<br />该MBR是位于硬盘驱动器通常是第一个扇区 512 字节的代码/dev/sda或/dev/hda根据您的硬盘驱动器架构。但是请注意，有时MBR可以位于Linux的Live USB或DVD安装中。<br />Linux 中有 3 种主要类型的引导加载程序：LILO、GRUB和GRUB2。该GRUB2引导装载程序是现代的 Linux 发行版的最新和主引导程序，另外两个已经过时。<br />GRUB2代表GRand Unified Bootloader版本 2。一旦BIOS找到 grub2 引导加载程序，它就会执行并将其加载到主内存 (RAM) 上。<br />该GRUB2菜单允许你做两件事情。它允许您选择要使用的 Linux 内核版本。如果您已经升级了几次系统，您可能会看到列出的不同内核版本。此外，它还使您能够通过按键盘组合键来编辑某些内核参数。
<a name="XnCXu"></a>
#### 选择内核版本
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671254056019-6261ee6b-34df-4be3-9913-9e7f3048a4ff.jpeg#averageHue=%23080808&clientId=u7e157960-20eb-4&from=paste&id=uf383e1f0&originHeight=400&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e198dd3-f1f4-4559-9a10-524b99cf035&title=)<br />此外，在有多个操作系统安装如何使用 FTP 网络源安装多个 CentOS/RHEL 服务器")的双引导设置中，grub 菜单允许您选择要引导到的操作系统。grub2 配置文件是/boot/grub2/grub2.cfg文件。GRUB 的主要目标是将 Linux 内核加载到主内存中。
<a name="KG9Hm"></a>
### 3、内核初始化
内核是任何 Linux 系统的核心。它将 PC 的硬件与底层进程连接起来。内核控制 Linux 系统上的所有进程。一旦引导加载程序加载了选定的 Linux 内核，它必须在执行任何任务之前从其压缩版本中自行解压缩。在自解压缩，所选择的内核安装根文件系统，并初始化/ sbin 目录 / 初始化通常被称为程序初始化。
<a name="dYWyK"></a>
#### 内核初始化过程
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671254056088-ee5dad1f-26fe-4aad-9967-aa9eb71a3eef.jpeg#averageHue=%23111111&clientId=u7e157960-20eb-4&from=paste&id=ued24714b&originHeight=600&originWidth=800&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a81f7ad-63b6-4d5e-8823-d767625825e&title=)<br />Init始终是第一个要执行的程序，并被分配进程 ID 或 PID 为 1。它是 init 进程，它产生各种守护进程并挂载/etc/fstab文件中指定的所有分区。<br />内核然后挂载初始 RAM 磁盘 (initrd)，它是一个临时的根文件系统，直到真正的根文件系统被挂载。所有内核都/boot与初始 RAM 磁盘映像一起位于该目录中。
<a name="ogYZd"></a>
### 4、启动 Systemd
内核最终加载了Systemd，它是旧SysVinit的替代品。Systemd是所有 Linux 进程的母体，并管理文件系统的挂载、启动和停止服务等等。<br />Systemd 使用/etc/systemd/system/default.target文件来确定 Linux 系统应该引导到的状态或目标。

- 对于桌面工作站（带有 GUI），默认目标值为 5，相当于旧 SystemV init 的运行级别 5。
- 对于服务器，默认目标是multi-user.target，它对应于 SysV init 中的运行级别 3。

以下是 systemd 目标的细分：

- `poweroff.target (runlevel 0)`：关闭或关闭系统。
- `rescue.target (runlevel 1)`：启动救援shell会话。
- `multi-user.target (runlevel 2,3,4)`：将系统配置为非图形（控制台）多用户系统。
- `graphics.target (runlevel 5)`：将系统设置为使用具有网络服务的图形多用户界面。
- `reboot.target (runlevel 6)`：重新启动系统。

要检查系统上的当前目标，请运行以下命令：
```bash
> systemctl get-default
```
<a name="ZosYu"></a>
#### 检查运行级别
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671254056031-1f0d4723-dcec-490e-adfc-0441029fbc0e.jpeg#averageHue=%23070606&clientId=u7e157960-20eb-4&from=paste&id=u5e9bd71e&originHeight=82&originWidth=356&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4da643e6-6022-4873-bcd4-72ede396c0c&title=)<br />可以通过在终端上运行以下命令从一个目标切换到另一个目标：
```bash
> init runlevel-value
```
例如，`init 3` 将系统配置为非图形状态。<br />在`init 6`命令重新启动系统和`init 0`关闭系统电源。当要切换到这两个目标时，请务必调用sudo 命令。<br />一旦systemd加载所有守护进程并设置目标或运行级别值，引导过程就会结束。此时，系统会提示输入用户名和密码，然后就可以进入 Linux 系统。
