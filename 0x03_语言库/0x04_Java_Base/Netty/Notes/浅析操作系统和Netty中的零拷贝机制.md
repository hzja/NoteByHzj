Netty 零拷贝<br />零拷贝机制(Zero-Copy)是在操作数据时不需要将数据从一块内存区域复制到另一块内存区域的技术，这样就避免了内存的拷贝，使得可以提高CPU的。零拷贝机制是一种操作数据的优化方案，通过避免数据在内存中拷贝达到的提高CPU性能的方案。
<a name="UEA1A"></a>
## 1、操作系统的零拷贝机制
操作系统的存储空间包含硬盘和内存，而内存又分成用户空间和内核空间。以从文件服务器下载文件为例，服务器需要将硬盘中的数据通过网络通信发送给客户端，大致流程如下:<br />第一步：操作系统通过DMA传输将硬盘中的数据复制到内核缓冲区<br />第二步：操作系统执行`read`方法将内核缓冲区的数据复制到用户空间<br />第三步：操作系统执行`write`方法将用户空间的数据复制到内核`socket`缓冲区<br />第四步：操作系统通过DMA传输将内核`socket`缓冲区数据复制给网卡发送数据<br />流程如下图示：<br />![2021-05-05-17-56-59-718752.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620208666324-965fcbe9-7a96-4684-bae6-aac64a3ec58c.jpeg#clientId=u9b5756bf-8879-4&from=ui&id=u07aecdda&originHeight=727&originWidth=1080&originalType=binary&size=46379&status=done&style=shadow&taskId=u307b6fc1-ba9f-4f24-81bf-122f45a4427)<br />整个流程中：DMA拷贝2次、CPU拷贝2次、用户空间和内核空间切换4次<br />整个流程从内核空间和硬件之间数据拷贝是DMA复制传输，内核空间和用户空间之间数据拷贝是通过CPU复制。另外CPU除了需要参与拷贝任务，还需要多次从内核空间和用户空间之间来回切换，无疑都额外增加了很多的CPU工作负担。<br />所以操作系统为了减少CPU拷贝数据带来的性能消耗，提供了几种解决方案来减少CPU拷贝次数
<a name="KxkXK"></a>
### 1.1.使用`mmap`函数
`mmap`函数的作用相当于是内存共享，将内核空间的内存区域和用户空间共享，这样就避免了将内核空间的数据拷贝到用户空间的步骤，通过`mmap`函数发送数据时上述的步骤如下:<br />第一步：操作系统通过DMA传输将硬盘中的数据复制到内核缓冲区，执行了`mmap`函数之后，拷贝到内核缓冲区的数据会和用户空间进行共享，所以不需要进行拷贝<br />第二步：CPU将内核缓冲区的数据拷贝到内核空间`socket`缓冲区<br />第三步：操作系统通过DMA传输将内核`socket`缓冲区数据拷贝给网卡发送数据<br />流程如下图示：<br />![2021-05-05-17-57-00-006832.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620208678824-d0db4255-ab8e-4c3a-92ac-18bbc790bf98.jpeg#clientId=u9b5756bf-8879-4&from=ui&id=ud0b11f96&originHeight=733&originWidth=1080&originalType=binary&size=48446&status=done&style=shadow&taskId=ue73eca70-8dd9-48da-8a6b-55802f3273c)<br />整个流程中：DMA拷贝2次、CPU拷贝1次、用户空间和内核空间切换4次<br />可以发现此种方案避免了内核空间和用户空间之间数据的拷贝工作，但是在内核空间内部还是会有一次数据拷贝过程，而且CPU还是会有从内核空间和用户空间的切换过程
<a name="VeDWZ"></a>
### 1.2.使用`sendfile`函数
`sendfile`函数的作用是将一个文件描述符的内容发送给另一个文件描述符。而用户空间是不需要关心文件描述符的，所以整个的拷贝过程只会在内核空间操作，相当于减少了内核空间和用户空间之间数据的拷贝过程，而且还避免了CPU在内核空间和用户空间之间的来回切换过程。整体流程如下：<br />第一步：通过DMA传输将硬盘中的数据复制到内核页缓冲区<br />第二步：通过`sendfile`函数将页缓冲区的数据通过CPU拷贝给socket缓冲区<br />第三步：网卡通过DMA传输将socket缓冲区的数据拷贝走并发送数据<br />流程如下图示：<br />![2021-05-05-17-57-03-384507.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620208693169-c03e148b-99b9-4357-923b-51a2caf580f2.jpeg#clientId=u9b5756bf-8879-4&from=ui&id=u359b3147&originHeight=719&originWidth=1080&originalType=binary&size=43371&status=done&style=shadow&taskId=u8e5cf620-3d0c-4045-991a-e1d58b0b07b)<br />整个过程中：DMA拷贝2次、CPU拷贝1次、内核空间和用户空间切换0次<br />可以看出通过`sendfile`函数时只会有一次CPU拷贝过程，而且全程都是在内核空间实现的，所以整个过程都不会使得CPU在内核空间和用户空间进行来回切换的操作，性能相比于`mmap`而言要更好<br />另外如果硬件支持的话，`sendfile`函数还可以直接将文件描述符和数据长度发送给`socket`缓冲区，然后直接通过DMA传输将页缓冲区的数据拷贝给网卡进行发送即可，这样就避免了CPU在内核空间内的拷贝过程，流程如下：<br />第一步：通过DMA传输将硬盘中的数据复制到内核页缓冲区<br />第二步：通过`sendfile`函数将页缓冲区数据的文件描述符和数据长度发送给`socket`缓冲区<br />第三步：网卡通过DMA传输根据文件描述符和文件长度直接从页缓冲区拷贝数据<br />如下图示：<br />![2021-05-05-17-57-03-692367.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620208703018-9ca67e49-e371-4dd7-8745-8327999e9eb9.jpeg#clientId=u9b5756bf-8879-4&from=ui&id=u0619cd4a&originHeight=717&originWidth=1080&originalType=binary&size=47070&status=done&style=shadow&taskId=ub7946130-8d94-4731-b2dd-468f64555d1)<br />整个过程中：DMA拷贝2次、CPU拷贝0次、内核空间和用户空间切换0次<br />所以整个过程都是没有CPU拷贝的过程的，实现了真正的CPU零拷贝机制
<a name="nzouC"></a>
### 1.3.使用`splice`函数
`splice`函数的作用是将两个文件描述符之间建立一个管道，然后将文件描述符的引用传递过去，这样在使用到数据的时候就可以直接通过引用指针访问到具体数据。过程如下:<br />第一步：通过DMA传输将文件复制到内核页缓冲区<br />第二步：通过`splice`函数在页缓冲区和socket缓冲区之间建立管道，并将文件描述符的引用指针发送给`socket`缓冲区<br />第三步：网卡通过DMA传输根据文件描述符的指针直接访问数据<br />如下图示：<br />![2021-05-05-17-57-04-003224.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620208717946-c514c932-2936-4dcd-a18e-95fa8b211785.jpeg#clientId=u9b5756bf-8879-4&from=ui&id=u70d3a70c&originHeight=732&originWidth=1080&originalType=binary&size=49956&status=done&style=shadow&taskId=u934a6679-cc3d-48b2-82ae-d9d4886a252)<br />整个过程中：DMA拷贝2次、CPU拷贝0次、内核空间和用户空间切换0次可以看出通过`splice`函数传输数据时同样可以实现CPU的零拷贝，且不需要CPU在内核空间和用户空间之间来回切换
:::info
总结：实际上操作系统的零拷贝机制只是针对于CPU的零拷贝，而内核空间和硬件之间还是会存在数据拷贝的过程，只不过通过DMA传输，而不需要CPU来参与数据的拷贝过程可以看出通过`mmap`函数可以减少一次CPU拷贝，但是还会有一个CPU拷贝。而使用`sendfile`和`splice`函数都已经实现了CPU零拷贝而实现了数据传输过程。
:::
<a name="MRMnw"></a>
## 2、Java中的零拷贝机制
Java的应用程序经常会遇到数据传输的场景，在Java NIO包中就提供了零拷贝机制的实现，主要是通过NIO包中的`FileChannel`实现`FileChannel`提供了`transferTo`和`transferFrom`方法，都是采用了调用底层操作系统的`sendfile`函数来实现的CPU零拷贝机制。<br />kafka服务器就是采用了`FileChannel`的`transfer`方法实现了高性能的IO传输操作。<br />Netty中的零拷贝机制Netty作为NIO的高性能网络通信框架，同样也实现了零拷贝机制，不过和操作系统的零拷贝机制则不是一个概念。<br />Netty中的零拷贝机制体现在多个场景：

1. 使用直接内存，在进行IO数据传输时避免了`ByteBuf`从堆外内存拷贝到堆内内存的步骤，而如果使用堆内内存分配`ByteBuf`的话，那么发送数据时需要将IO数据从堆内内存拷贝到堆外内存才能通过`Socket`发送
2. Netty的文件传输使用了`FileChannel`的`transferTo`方法，底层使用到`sendfile`函数来实现了CPU零拷贝
3. Netty中提供`CompositeByteBuf`类，用于将多个`ByteBuf`合并成逻辑上的`ByteBuf`，避免了将多个`ByteBuf`拷贝成一个`ByteBuf`的过程
4. ByteBuf支持`splice`方法可以将`ByteBuf`分解成多个共享内存区域的`ByteBuf`，避免了内存拷贝
