Linux 内存 
<a name="zFKHx"></a>
### 一、linux的内存机制
直接从物理内存读写数据要比从硬盘读写数据要快的多，因此，希望所有数据的读取和写入都在内存完成，而内存是有限的，这样就引出了物理内存与虚拟内存的概念。<br />物理内存就是系统硬件提供的内存大小，是真正的内存，相对于物理内存，在linux下还有一个虚拟内存的概念，虚拟内存就是为了满足物理内存的不足而提出的策略，它是利用磁盘空间虚拟出的一块逻辑内存，用作虚拟内存的磁盘空间被称为交换空间（Swap Space）。<br />作为物理内存的扩展，linux会在物理内存不足时，使用交换分区的虚拟内存，更详细的说，就是内核会将暂时不用的内存块信息写到交换空间，这样以来，物理内存得到了释放，这块内存就可以用于其它目的，当需要用到原始的内容时，这些信息会被重新从交换空间读入物理内存。<br />Linux的内存管理采取的是分页存取机制，为了保证物理内存能得到充分的利用，内核会在适当的时候将物理内存中不经常使用的数据块自动交换到虚拟内存中，而将经常使用的信息保留到物理内存。<br />要深入了解linux内存运行机制，需要知道下面提到的几个方面：<br />Linux系统会不时的进行页面交换操作，以保持尽可能多的空闲物理内存，即使并没有什么事情需要内存，Linux也会交换出暂时不用的内存页面。这可以避免等待交换所需的时间。<br />Linux 进行页面交换是有条件的，不是所有页面在不用时都交换到虚拟内存，linux内核根据“最近最经常使用”算法，仅仅将一些不经常使用的页面文件交换到虚拟内存，有时会看到这么一个现象：linux物理内存还有很多，但是交换空间也使用了很多。其实，这并不奇怪，例如，一个占用很大内存的进程运行时，需要耗费很多内存资源，此时就会有一些不常用页面文件被交换到虚拟内存中，但后来这个占用很多内存资源的进程结束并释放了很多内存时，刚才被交换出去的页面 文件并不会自动的交换进物理内存，除非有这个必要，那么此刻系统物理内存就会空闲很多，同时交换空间也在被使用，就出现了刚才所说的现象了。<br />交换空间的页面在使用时会首先被交换到物理内存，如果此时没有足够的物理内存来容纳这些页面，它们又会被马上交换出去，如此以来，虚拟内存中可能没有足够空间来存储这些交换页面，最终会导致linux出现假死机、服务异常等问题，linux虽然可以在一段时间内自行恢复，但是恢复后的系统已经基本不可用了。<br />因此，合理规划和设计Linux内存的使用，是非常重要的.<br />在Linux 操作系统中，当应用程序需要读取文件中的数据时，操作系统先分配一些内存，将数据从磁盘读入到这些内存中，然后再将数据分发给应用程序；当需要往文件中写数据时，操作系统先分配内存接收用户数据，然后再将数据从内存写到磁盘上。然而，如果有大量数据需要从磁盘读取到内存或者由内存写入磁盘时，系统的读写性能就变得非常低下，因为无论是从磁盘读数据，还是写数据到磁盘，都是一个很消耗时间和资源的过程，在这种情况下，Linux引入了buffers和 cached机制。<br />buffers与cached都是内存操作，用来保存系统曾经打开过的文件以及文件属性信息，这样当操作系统需要读取某些文件时，会首先在buffers 与cached内存区查找，如果找到，直接读出传送给应用程序，如果没有找到需要数据，才从磁盘读取，这就是操作系统的缓存机制，通过缓存，大大提高了操作系统的性能。但buffers与cached缓冲的内容却是不同的。<br />buffers是用来缓冲块设备做的，它只记录文件系统的元数据（metadata）以及 tracking in-flight pages，而cached是用来给文件做缓冲。更通俗一点说：buffers主要用来存放目录里面有什么内容，文件的属性以及权限等等。而cached直接用来记忆打开过的文件和程序。<br />为了验证结论是否正确，可以通过vi打开一个非常大的文件，看看cached的变化，然后再次vi这个文件，感觉一下两次打开的速度有何异同，是不是第二次打开的速度明显快于第一次呢？接着执行下面的命令：<br />find / -name .conf看看buffers的值是否变化，然后重复执行find命令，看看两次显示速度有何不同。
<a name="2wPlD"></a>
### 二、linux使用虚拟内存（swap)
```bash
[root@iZuligp6e1dyzfZ tools 15:49]# cat /proc/sys/vm/swappiness
60
```
上面这个60代表物理内存在使用40%的时候才会使用swap（当剩余物理内存低于40%（40=100-60）时，开始使用交换空间）swappiness=0的时候表示最大限度使用物理内存，然后才是 swap空间，swappiness＝100的时候表示积极的使用swap分区，并且把内存上的数据及时的搬运到swap空间里面。<br />**值越大表示越倾向于使用swap。可以设为0，这样做并不会禁止对swap的使用，只是最大限度地降低了使用swap的可能性。**<br />通常情况下：swap分区设置建议是内存的两倍 （内存小于等于4G时），如果内存大于4G，swap只要比内存大就行。另外尽量的将swappiness调低，这样系统的性能会更好。<br />B.**修改swappiness参数**
```bash
临时性修改：
[root@iZuligp6e1dyzfZ tools 16:08]# sysctl vm.swappiness=10
vm.swappiness = 10
[root@iZuligp6e1dyzfZ tools 16:15]# cat /proc/sys/vm/swappiness
10
```
**永久性修改：**
```bash
[root@iZuligp6e1dyzfZ tools 16:15]# vim /etc/sysctl.conf
```
修改参数vm.swappiness = 35的值<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597393119260-a067c0d5-f68d-4efb-92a2-10d618258617.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2060135&status=done&style=none&width=1107.6666666666667)<br />应用配置
```bash
[root@iZuligp6e1dyzfZ tools 16:19]# sysctl -p /etc/sysctl.conf
```
查看生效的配置的值
```bash
[root@iZuligp6e1dyzfZ tools 16:19]# cat /proc/sys/vm/swappiness
35
```
立即生效，重启也可以生效。
<a name="yYCW5"></a>
### 三、释放内存
一般系统是不会自动释放内存的关键的配置文件/proc/sys/vm/drop_caches。这个文件中记录了缓存释放的参数，默认值为0，也就是不释放缓存。他的值可以为0~3之间的任意数字，代表着不同的含义：<br />0 – 不释放1 – 释放页缓存2 – 释放dentries和inodes3 – 释放所有缓存<br />实操：
```bash
[root@iZuligp6e1dyzfZ tools 16:19]# free -m
              total        used        free      shared  buff/cache   available
Mem:           1839         547          92          16        1199        1107
Swap:             0           0           0
[root@iZuligp6e1dyzfZ tools 16:21]# echo '3' > /proc/sys/vm/drop_caches
[root@iZuligp6e1dyzfZ tools 16:22]# free -m
              total        used        free      shared  buff/cache   available
Mem:           1839         548        1208          16          82        1163
Swap:             0           0           0
[root@iZuligp6e1dyzfZ tools 16:22]#
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597393418907-62ab4efd-be24-4fc3-abb7-7bc8deededb5.png#align=left&display=inline&height=187&originHeight=561&originWidth=3323&size=659908&status=done&style=none&width=1107.6666666666667)<br />很明显多出来很多空闲的内存
<a name="IQCx9"></a>
### 四、释放swap
前提：首先要保证内存剩余要大于等于swap使用量，否则会宕机！根据内存机制，swap分区一旦释放，所有存放在swap分区的文件都会转存到物理内存上。通常通过重新挂载swap分区完成释放swap。<br />a.查看当前swap分区挂载在哪<br />b.关停这个分区<br />c.查看状态<br />d.查看swap分区是否关停，最下面一行显示全<br />e.将swap挂载到/dev/sda5上<br />f.查看挂载是否成功
```bash
[root@iZuligp6e1dyzfZ tools 16:22]# swapon -s
[root@iZuligp6e1dyzfZ tools 16:22]# swapoff /dev/dm-1
```

