网络 TCP
<a name="CbigV"></a>
### 滑动窗口
滑动窗口动图体验的网站地址：<br />[https://www2.tkn.tu-berlin.de/teaching/rn/animations/gbn_sr/](https://www2.tkn.tu-berlin.de/teaching/rn/animations/gbn_sr/)<br />下图是从网站录制的一张动图：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1635900633194-b3c8cd49-b316-45e4-b6f0-44ba5650c5ca.gif#clientId=udab74d7f-676e-4&from=paste&id=u736c5be8&originHeight=426&originWidth=802&originalType=url&ratio=1&status=done&style=shadow&taskId=ud6fb92a6-3436-4702-a547-2346afe3f8b)<br />每个长方块代表一个数据，上面的是发送方，下面的是接收方，长方块的不同的颜色代表着不同的意义，而且发送发和接收方滑动窗口的也是不同的。<br />先来说说，「发送方」的长方块颜色：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635900633173-cb984597-0d4d-4d54-bd15-cd41b6d65fdf.webp#clientId=udab74d7f-676e-4&from=paste&id=ueb7c1655&originHeight=105&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u87f57734-1ab9-4d97-b474-672b49a06e7)

- 黄色：代表已发送并且已被确认的数据；
- 蓝色：代表已发送但未被确认或者还未发送的数据；
- 灰色背景：代表滑动窗口的大小，比如这个动图滑动窗口大小是 5。
- 紫色圈圈：代表超时重传时间；

再来说说，「接收方」的长方块颜色：

- 深蓝色：代表数据已经被接收；
- 白色：代表还未接收到的数据；
- 绿色：代表对发送方数据的ack确认包；

这个网站，可以自行设置一些参数。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635900633138-83349b6c-5aa1-4a64-bdd5-c8dcad939343.webp#clientId=udab74d7f-676e-4&from=paste&id=u4415d7b8&originHeight=324&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ub48f5d5f-8a39-488f-85a5-2f203cc476a)<br />比如，可以设置滑动窗口的大小、超时重传时间、发送速率、确认模式（选择性确认模式or普通确认模式）等等。<br />这里设置几个参数，看看效果。<br />把超时重传时间时间缩短，并且确认模式是普通确认模式，动图效果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1635900633228-dd3d094d-76b1-445a-8042-800eaadbd3d4.gif#clientId=udab74d7f-676e-4&from=paste&id=ub4615206&originHeight=426&originWidth=802&originalType=url&ratio=1&status=done&style=shadow&taskId=u3292bee8-0f2b-4594-a677-34185b417fb)<br />可以看到，这种确认模式在发生超时重传的时候，会把发送窗口内已发送的数据都会重传一遍。<br />接着，把确认模式改成「选择性确认」。其动图效果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1635900633618-64c3faff-d1eb-4689-94e2-7d8b48267048.gif#clientId=udab74d7f-676e-4&from=paste&id=uad12669d&originHeight=426&originWidth=802&originalType=url&ratio=1&status=done&style=none&taskId=u0410ea97-23ff-4979-b88d-6af2ad1fa32)<br />可以看到，改用「选择性确认」的方式，只会重传丢失的那一个数据。
<a name="uxUCk"></a>
### 流量控制
流量控制动图体验的网站地址：<br />[https://www2.tkn.tu-berlin.de/teaching/rn/animations/flow/](https://www2.tkn.tu-berlin.de/teaching/rn/animations/flow/)<br />这个网站的可以设置参数就比较少，理解起来也很容易。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635900633713-cdc2d98d-5ab6-4e33-a975-a89792ba5aa8.webp#clientId=udab74d7f-676e-4&from=paste&id=u57d9977d&originHeight=396&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ua20bb23a-2bd3-42af-b348-4f37fec563c)<br />这里简单介绍下这些参数：

- file size，发送的文件总大小；
- buffer size，发送方和接收方的内核缓冲区的大小；
- propagation time，发送方和接收方之间的传播时间。

下图是从网站录制的一张动图：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1635900633735-d28ce9fe-c11b-448a-ab80-a5ea9fcb7010.gif#clientId=udab74d7f-676e-4&from=paste&id=uaadb9019&originHeight=620&originWidth=1002&originalType=url&ratio=1&status=done&style=shadow&taskId=u2d96df5f-995a-4156-a94f-f59bcde0a2d)<br />可以看到，发送方和接收方共有两个缓冲区，一个是 buffer，指的是内核缓冲区，另外一个是 application，指的是应用层的缓冲区。<br />对于发送发来说，当要发送数据的时候，需要从 application 拷贝数据到 buffer，而对于接收方来说，当接收到数据的时候，会先缓存到 buffer，然后再由应用程序从 buffer 读取数据。<br />当接收方的 application 来不及从 buffer 读取数据，而导致 buffer 堆积满了，这时接收方就会给发送方通报一个接收窗口为0的报文，表示已经不能再接收数据啦，比如下面这个情况。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635900633691-065f6ee6-62f9-418f-ac31-68f939ba3d41.webp#clientId=udab74d7f-676e-4&from=paste&id=uf27f3268&originHeight=646&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u5f5c7d2c-c475-4b52-a859-306dd53b7e7)
