压缩可以降低占用空间，顾名思义，内存压缩就是压缩内存，节省内存空间。就目前的技术而言， I/O 的速度远远慢于这 RAM 操作速度。因此，如果频繁地做 I/O 操作，不仅影响 flash 使用寿命，还严重影响系统性能。内存压缩是一种让 I/O 过程平滑过渡的做法，即尽量减少由于内存紧张导致的 I/O，提升性能。
<a name="Qfsh5"></a>
## 介绍 zSwap 技术
zSwap 是 Linux 内核的一个功能，它为交换页提供了一个压缩的回写缓存，作为一种虚拟内存压缩形式。当内存页要被换出时，zSwap 不会把它们移到交换设备上，而是对它们进行压缩，然后把它们存储到系统 RAM 中动态分配的内存池中。后来，向实际的交换设备的回写被推迟了，甚至完全避免了，从而大大减少了需要交换的 Linux 系统的 I/O，其代价是需要额外的 CPU 周期来执行压缩。<br />zSwap 允许 Linux 更有效地利用 RAM，因为它实际上增加了内存容量，而不是在压缩/解压缩交换页时稍微增加 CPU 的使用。zSwap 存在于内核中，但默认并没有开启，要使用它必须通过修改配置文件开启。
<a name="zMHx7"></a>
## 主流内存压缩技术
<a name="F6XW3"></a>
### zSwap
zSwap 是在 memory 与 flash 之间的一层缓存，当内存需要 swap 出去磁盘的时候，先通过压缩放到 zSwap 中去，zSwap 空间按需增长。达到一定程度后则会按照 LRU 的顺序(前提是使用的内存分配方法需要支持 LRU)将就最旧的 page 解压写入磁盘 swap device，之后将当前的 page 压缩写入 zSwap。<br />zSwap 本身存在一些缺陷或问题：如果开启当 zSwap 满交换出 backing store 的功能，由于需要将 zSwap 里的内存按 LRU 顺序解压再 swap out，这就要求内存分配器支持 LRU 功能；如果不开启当 zSwap 满交换出 backing store 的功能，和 zRam 是类似的。
<a name="KLxgz"></a>
### zRAM
zRram 即压缩的内存，使用内存模拟 block device 的做法。实际不会写到块设备中去，只会压缩后写到模拟的块设备中，其实也就是还是在 RAM 中，只是通过压缩了。由于压缩和解压缩的速度远比读写 I/O 好，因此在移动终端设备广泛被应用。<br />zRram 本身存在一些缺陷或问题：zRam 大小是可灵活配置的，配置多少成为了一个问题；使用 zRam 可能会在低内存场景由于频繁的内存压缩导致 kswapd 进程占 CPU 高；增大了 zRam 配置，对系统内存碎片是否有影响
<a name="udc8h"></a>
### zCache
zCache 是 oracle 提出的一种实现文件页压缩技术，也是 memory 与 block dev 之间的一层存储，与 zSwap 比较接近，但 zCache 目前压缩的是文件页，而 zSwap 和 zRAM 压缩是匿名页。
<a name="GLr8Z"></a>
## 介绍 zstd 压缩算法
Zstandard 是一种实时压缩算法，提供高压缩率。它提供了非常广泛的压缩/速度权衡，同时有一个非常快的解码器支持。它还为小数据提供了一种特殊的模式，称为字典压缩，并可以从任何样本集中创建字典。Zstandard 库是作为使用 BSD 许可证的开源软件提供的。
<a name="nMQN3"></a>
## 开启 zSwap 的方法
```bash
# edit grub as root
$ sudo vim /etc/default/grub
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash zswap.enabled=1 zswap.compressor=zstd zswap.zpool=z3fold"

# update grub
$ sudo update-grub
```
```bash
# install zstd and Z3fold
$ sudo apt install checkinstall build-essential
$ sudo apt install zstd # not the latest version

# edit the modules file
$ sudo vim /etc/initramfs-tools/modules
...
zstd
zstd_compress
z3fold

# update initramfs
$ sudo update-initramfs -u -k all

# restart PC and verify ZSWAP if enabled
$ cat /sys/module/zswap/parameters/enabled
Y

# verify ZSWAP if its using zstd and z3fold
$ sudo dmesg | grep -i zswap:
[    1.059997] zswap: loaded using pool zstd/z3fold
```
