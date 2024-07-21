零拷贝 Zero-Copy
<a name="pGKI5"></a>
## 1、数据拷贝基础过程
在Linux系统内部缓存和内存容量都是有限的，更多的数据都是存储在磁盘中。对于Web服务器来说，经常需要从磁盘中读取数据到内存，然后再通过网卡传输给用户：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422436-76dc47c3-1bc0-445f-8ef9-61d530d52a61.png#align=left&display=inline&height=145&originHeight=145&originWidth=563&size=0&status=done&style=shadow&width=563)<br />上述数据流转只是大框，接下来看看几种模式。
<a name="5aXFT"></a>
### 1.1 仅CPU方式

- 当应用程序需要读取磁盘数据时，调用read()从用户态陷入内核态，read()这个系统调用最终由CPU来完成；
- CPU向磁盘发起I/O请求，磁盘收到之后开始准备数据；
- 磁盘将数据放到磁盘缓冲区之后，向CPU发起I/O中断，报告CPU数据已经Ready了；
- CPU收到磁盘控制器的I/O中断之后，开始拷贝数据，完成之后read()返回，再从内核态切换到用户态；

![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422524-720408cd-3a32-465d-a5f1-8b4dd29339c2.png#align=left&display=inline&height=587&originHeight=587&originWidth=604&size=0&status=done&style=shadow&width=604)
<a name="IR8tc"></a>
### 1.2 CPU&DMA方式
CPU的时间宝贵，让它做杂活就是浪费资源。<br />直接内存访问（Direct Memory Access），是一种硬件设备绕开CPU独立直接访问内存的机制。所以DMA在一定程度上解放了CPU，把之前CPU的杂活让硬件直接自己做了，提高了CPU效率。<br />目前支持DMA的硬件包括：网卡、声卡、显卡、磁盘控制器等。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422523-71696468-06ff-41f2-9ce1-87de64ebb6f7.png#align=left&display=inline&height=396&originHeight=396&originWidth=890&size=0&status=done&style=shadow&width=890)<br />有了DMA的参与之后的流程发生了一些变化：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422544-a3ff1017-2ac9-4f11-924d-dc223cdf7c29.png#align=left&display=inline&height=576&originHeight=576&originWidth=865&size=0&status=done&style=shadow&width=865)<br />最主要的变化是，CPU不再和磁盘直接交互，而是DMA和磁盘交互并且将数据从磁盘缓冲区拷贝到内核缓冲区，之后的过程类似。
> 无论从仅CPU方式和DMA&CPU方式，都存在多次冗余数据拷贝和内核态&用户态的切换。

继续思考Web服务器读取本地磁盘文件数据再通过网络传输给用户的详细过程。
<a name="1WyNm"></a>
## 2、普通模式数据交互
一次完成的数据交互包括几个部分：系统调用syscall、CPU、DMA、网卡、磁盘等。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422491-fbb6be99-6b4b-4f80-b950-00120c9f99fb.png#align=left&display=inline&height=249&originHeight=249&originWidth=429&size=0&status=done&style=shadow&width=429)<br />系统调用syscall是应用程序和内核交互的桥梁，每次进行调用/返回就会产生两次切换：

- 调用syscall 从用户态切换到内核态
- syscall返回 从内核态切换到用户态

![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422552-f8729b3d-e615-41b1-9c30-b231b5a6f25a.png#align=left&display=inline&height=284&originHeight=284&originWidth=654&size=0&status=done&style=shadow&width=654)<br />来看下完整的数据拷贝过程简图：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422577-247458e1-bfab-4a2b-a6b7-c1031b0c7c7f.png#align=left&display=inline&height=488&originHeight=488&originWidth=738&size=0&status=done&style=shadow&width=738)<br />**读数据过程：**

- 应用程序要读取磁盘数据，调用read()函数从而实现用户态切换内核态，这是第1次状态切换；
- DMA控制器将数据从磁盘拷贝到内核缓冲区，这是第1次DMA拷贝；
- CPU将数据从内核缓冲区复制到用户缓冲区，这是第1次CPU拷贝；
- CPU完成拷贝之后，read()函数返回实现用户态切换用户态，这是第2次状态切换；

**写数据过程：**

- 应用程序要向网卡写数据，调用write()函数实现用户态切换内核态，这是第1次切换；
- CPU将用户缓冲区数据拷贝到内核缓冲区，这是第1次CPU拷贝；
- DMA控制器将数据从内核缓冲区复制到socket缓冲区，这是第1次DMA拷贝；
- 完成拷贝之后，write()函数返回实现内核态切换用户态，这是第2次切换；

**综上所述：**

- 读过程涉及2次空间切换、1次DMA拷贝、1次CPU拷贝；
- 写过程涉及2次空间切换、1次DMA拷贝、1次CPU拷贝；

可见传统模式下，涉及多次空间切换和数据冗余拷贝，效率并不高，接下来就该零拷贝技术出场了。
<a name="TpK9g"></a>
## 3、零拷贝技术
<a name="ti2gR"></a>
### 3.1 出现原因
可以看到，如果应用程序不对数据做修改，从内核缓冲区到用户缓冲区，再从用户缓冲区到内核缓冲区。两次数据拷贝都需要CPU的参与，并且涉及用户态与内核态的多次切换，加重了CPU负担。<br />需要降低冗余数据拷贝、解放CPU，这也就是零拷贝Zero-Copy技术。
<a name="oOWpy"></a>
### 3.2 解决思路
目前来看，零拷贝技术的几个实现手段包括：mmap+write、sendfile、sendfile+DMA收集、splice等。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422568-688b8522-2c84-473d-bbb7-7a4a7d34031d.png#align=left&display=inline&height=274&originHeight=274&originWidth=468&size=0&status=done&style=shadow&width=468)
<a name="zUuAW"></a>
#### 3.2.1 mmap方式
mmap是Linux提供的一种内存映射文件的机制，它实现了将内核中读缓冲区地址与用户空间缓冲区地址进行映射，从而实现内核缓冲区与用户缓冲区的共享。<br />这样就减少了一次用户态和内核态的CPU拷贝，但是在内核空间内仍然有一次CPU拷贝。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422605-ace47549-f865-4936-b4df-7c8391669166.png#align=left&display=inline&height=516&originHeight=516&originWidth=725&size=0&status=done&style=shadow&width=725)<br />mmap对大文件传输有一定优势，但是小文件可能出现碎片，并且在多个进程同时操作文件时可能产生引发coredump的signal。
<a name="OotUe"></a>
#### 3.2.2 sendfile方式
mmap+write方式有一定改进，但是由系统调用引起的状态切换并没有减少。<br />sendfile系统调用是在 Linux 内核2.1版本中被引入，它建立了两个文件之间的传输通道。<br />sendfile方式只使用一个函数就可以完成之前的read+write 和 mmap+write的功能，这样就少了2次状态切换，由于数据不经过用户缓冲区，因此该数据无法被修改。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422590-76877042-ca9a-4964-8d64-d7f114f068a6.png#align=left&display=inline&height=395&originHeight=395&originWidth=764&size=0&status=done&style=shadow&width=764)![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422619-bed01b2d-0a26-4fb1-ad11-b542f5fa258b.png#align=left&display=inline&height=456&originHeight=456&originWidth=724&size=0&status=done&style=shadow&width=724)<br />从图中可以看到，应用程序只需要调用sendfile函数即可完成，只有2次状态切换、1次CPU拷贝、2次DMA拷贝。<br />但是sendfile在内核缓冲区和socket缓冲区仍然存在一次CPU拷贝，或许这个还可以优化。
<a name="waM3h"></a>
#### 3.2.3 sendfile+DMA收集
Linux 2.4 内核对 sendfile 系统调用进行优化，但是需要硬件DMA控制器的配合。<br />升级后的sendfile将内核空间缓冲区中对应的数据描述信息（文件描述符、地址偏移量等信息）记录到socket缓冲区中。<br />DMA控制器根据socket缓冲区中的地址和偏移量将数据从内核缓冲区拷贝到网卡中，从而省去了内核空间中仅剩1次CPU拷贝。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422615-2421f7ba-fe52-4fa6-9ef0-ef68de958f5a.png#align=left&display=inline&height=467&originHeight=467&originWidth=730&size=0&status=done&style=shadow&width=730)<br />这种方式有2次状态切换、0次CPU拷贝、2次DMA拷贝，但是仍然无法对数据进行修改，并且需要硬件层面DMA的支持，并且sendfile只能将文件数据拷贝到socket描述符上，有一定的局限性。
<a name="BuQ08"></a>
#### 3.2.4 splice方式
splice系统调用是Linux 在 2.6 版本引入的，其不需要硬件支持，并且不再限定于socket上，实现两个普通文件之间的数据零拷贝。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422612-b169af1a-bdff-4bf6-8e18-bea773cabcc6.png#align=left&display=inline&height=395&originHeight=395&originWidth=751&size=0&status=done&style=shadow&width=751)<br />splice 系统调用可以在内核缓冲区和socket缓冲区之间建立管道来传输数据，避免了两者之间的 CPU 拷贝操作。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603609422590-7ad08907-305b-46cc-a84e-6beb809944ff.png#align=left&display=inline&height=467&originHeight=467&originWidth=704&size=0&status=done&style=shadow&width=704)<br />splice也有一些局限，它的两个文件描述符参数中有一个必须是管道设备。

