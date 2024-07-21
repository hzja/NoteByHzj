![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205510-6ffa7cc3-c0b8-4c15-9e2e-f43765580d4b.png#averageHue=%23f6f5f4&clientId=u485f6e98-67cf-4&from=paste&id=u829b4c10&originHeight=662&originWidth=648&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62f67be9-08b6-43dc-9d37-f029ddb7977&title=)
<a name="R3u1J"></a>
## 1、Chrome浏览器原理
还记得面试过程中被问了千百遍的"输入URL后发生了什么"这个经典问题吗？因为这个问题覆盖了太多的知识点，其中包括计算机网络，操作系统，数据结构等一些列问题，对于面试官和面试者来说都更方便后续面试的进展。想必很多小伙伴都做过web开发，或多或少都会和各种浏览器联系在一起，最终做测试的时候也会使用多种浏览器测试以保证能很好地兼容。那么现在先从Chrome浏览器说起。<br />先想想一个问题，打开一个微信或者一个XX音乐，一个网页，到底会开几个进程。<br />实验看看，打开一个网页到底开了几个进程，又分别有什么作用<br />![打开浏览器使用的 进程数](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205524-d97dfdf8-67de-4d9d-9d03-5a9d52da26cb.png#averageHue=%23fefdfd&clientId=u485f6e98-67cf-4&from=paste&id=uca07cd38&originHeight=155&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u964127c8-37d3-49cf-9af5-d76c3635acb&title=%E6%89%93%E5%BC%80%E6%B5%8F%E8%A7%88%E5%99%A8%E4%BD%BF%E7%94%A8%E7%9A%84%20%E8%BF%9B%E7%A8%8B%E6%95%B0 "打开浏览器使用的 进程数")<br />从上图可以发现，打开一个网页，使用了四个进程，分别为GPU进程，Network Service进程，当前网页进程和浏览器。到此，先复习进程与线程。<br />假设现在有这样几行伪代码，看看应该怎么去执行，可能分为四步<br />![示例伪代码](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205535-6884ccbd-8af0-4c2e-a9e5-e0dba916e4a4.png#averageHue=%2325ab38&clientId=u485f6e98-67cf-4&from=paste&id=u8dc55e8a&originHeight=482&originWidth=924&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u0e20419d-6cd1-4e33-b0cf-59e89f82eaa&title=%E7%A4%BA%E4%BE%8B%E4%BC%AA%E4%BB%A3%E7%A0%81 "示例伪代码")

- 计算X=5+2
- 计算y=8/4
- 计算z=2*5
- 显示出最后的结果

这也是采用串行的方式运行，也可说为单线程方式执行了四个任务，其好处是不用考虑诸如多线程的同步等问题。但是如果采用多线程

- 启动三个线程分别处理前面三个任务
- 最后一个线程显示结果

从上面这个小实验，可以知道使用多线程只需要两步就完成，但是单线程却使用了四步，可知使用多线程大大的提升了性能，记住：并不是多线程就一定会比单线程好，还需要从CPU使用率，IO磁盘等多个因素考虑。<br />_进程_<br />进程是一个程序的运行实体，在上面比较直观的感受到了多线程并行处理提高性能的优点。一个进程可以包含多个线程，但是一个线程只能归属于一个进程，那么一个进程到底是什么样子呢(ps 下面是在Linux中执行的代码，道理差不多)<br />_创建进程_<br />在Linux中使用fork创建进程，返回进程id。通过id的不同让父子进程各干其事，然后使用execvp执行具体任务<br />![创建进程](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205553-67c97a59-37cc-4bce-bfdc-b8e13a1fbd06.png#averageHue=%234aac5a&clientId=u485f6e98-67cf-4&from=paste&id=ub361a12d&originHeight=912&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u00c56bb6-d91f-4293-b57e-2a950844a61&title=%E5%88%9B%E5%BB%BA%E8%BF%9B%E7%A8%8B "创建进程")<br />创建主函数来使用上面的函数，看看会出现什么情况<br />![主函数](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205504-4aaeb6c0-179e-4950-a7bf-63533c558451.png#averageHue=%23326f3f&clientId=u485f6e98-67cf-4&from=paste&id=ua7eca077&originHeight=867&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufcd3a261-4b6f-405c-9d57-47ce8f8ccb1&title=%E4%B8%BB%E5%87%BD%E6%95%B0 "主函数")<br />好了，现在主函数和执行函数都写完了，但是这还只是文本文件，对于计算机而言只喜欢"01"组合，cpu执行的命令需要是二进制，所以需要进行「编译」，但是二进制的组合也得有一定的格式，不然定会乱套，在Linux中这种格式是"ELF"Executeableand Linkable Format），后续会详细介绍。程序编译到进程的过程如下图所示<br />![文本文件到二进制](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205894-befc5ba0-23c0-4734-a5ac-bbd2336be3fe.png#averageHue=%23f8f7f5&clientId=u485f6e98-67cf-4&from=paste&id=u2cf165f0&originHeight=1262&originWidth=662&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubdc6712a-7b80-4a14-9b17-14fd9453710&title=%E6%96%87%E6%9C%AC%E6%96%87%E4%BB%B6%E5%88%B0%E4%BA%8C%E8%BF%9B%E5%88%B6 "文本文件到二进制")<br />现在带参数编译两个程序<br />![编译](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891205974-0969a88d-32e9-4b6d-9b06-e4d90da422a6.png#averageHue=%2324b63a&clientId=u485f6e98-67cf-4&from=paste&id=ucaaa57a4&originHeight=446&originWidth=858&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufad272b8-8ad1-45bd-a06a-4582429115c&title=%E7%BC%96%E8%AF%91 "编译")<br />在编译的过程中，第一步预处理，将头文件直接嵌入到文件正文中，将定义的相关宏展开，最终编译为.o文件(可重定文件)，那么ELF是什么样子呢<br />![ELF头部](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891206025-c05806f5-2c06-49d9-9478-f839b90beee0.png#averageHue=%23f5efdf&clientId=u485f6e98-67cf-4&from=paste&id=ud50a2d45&originHeight=1086&originWidth=755&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u21fbbdfd-c002-4607-8710-ddb1287fd4a&title=ELF%E5%A4%B4%E9%83%A8 "ELF头部")<br />那么在Linux中如何查看呢(readelf)<br />可重定位什么意思呢？<br />字面意思是可以随时放在其他位置。对的，目前只是编译了文件，将来会被加载到内存里面，也就是加在某一个位置。可惜现在还是.o文件(代码片段)，不具备可执行的权限，它以后想变为函数库，哪里需要就在哪里去完成任务，搬到了哪里就重新定位了位置。要让它可重用，就得成为库文件，这个文件分为静态链接库(.a)和动态链接库，它能将一系列的.o文件归档为文件。怎么创建呢<br />![ar](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891206016-f9d39b53-8a47-42c3-884b-8678999cddef.png#averageHue=%2324b63a&clientId=u485f6e98-67cf-4&from=paste&id=u023be09a&originHeight=410&originWidth=924&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub0f791bb-5fbe-4a37-a353-668e38f0f14&title=ar "ar")<br />这个时候其他开发人员准备使用这个功能，加上参数连接过去就好了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891206145-514f6b9c-3357-416b-bc5f-072927d3bd00.png#averageHue=%2347694c&clientId=u485f6e98-67cf-4&from=paste&id=ud0d90347&originHeight=329&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc6187f94-6b61-4a47-943d-e1f2c3215c9&title=)<br />上面命令中"-L"代表默认在当前目录寻找.a文件，然后取出.o文件和creteprocess.o做连接形成二进制执行文件 staticcreateprocess。<br />一旦静态链接库连接出去，它的代码和变量的section合并，一次程序运行不再依赖这个库。这就可能出问题了，如果同样的代码段被多个程序使用，就会导致在内存中出现多份的情况，而且代码一旦更新，二进制文件也需要重新编译才能及时的更新。所以出现了_动态链接库，_使用这种方式的时候，程序并不在一开始就完成动态链接，而是需要到真正调用动态代码时，载入程序才会计算动态代码的逻辑地址。这种方式让程序初始化时间短，但是运行期间性能比不上静态连接的程序<br />说的有点远了，回来回来。刚才说了多线程并行计算的优势，画个图对比加深印象下<br />![单线程与多线程](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891206469-17177603-f68a-4dfd-bbc6-f55744e38432.png#averageHue=%23eceae4&clientId=u485f6e98-67cf-4&from=paste&id=u3db863e6&originHeight=752&originWidth=1025&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud27557f4-7849-4a37-a3ef-dfca7b5588d&title=%E5%8D%95%E7%BA%BF%E7%A8%8B%E4%B8%8E%E5%A4%9A%E7%BA%BF%E7%A8%8B "单线程与多线程")<br />总结下进程线程有哪些特点(面试跑不脱)

- 进程中的任意一个线程出错，将导致整个进程崩溃

假设将之前的伪代码修改为<br />X=5+2<br />Y=8/0<br />Z=5*2<br />此时Y很明显就是错的，当线程执行到Y的时候就会报错，进程崩溃大致其他两个线程也没有结果

- 当一个进程关闭后，操作系统会回收进程占用的资源

比如会使用很多不错的Chrome插件，当启动浏览器并打开这些插件的时候，都会占用内存，当关闭进程Chrome浏览器，这些内存就会被收回

- 进程之间内容相互隔离

这个机制是防止多个进程读写混乱，所以进程之间通信需要IPC(消息队列，共享内存等)。

- 线程之间共享进程数据

![线程共享数据](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891206643-7ff271d4-d322-487a-862c-796c65c8458f.png#averageHue=%23ebe9e7&clientId=u485f6e98-67cf-4&from=paste&id=ub9f90e5d&originHeight=752&originWidth=812&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud163abc6-eb7a-4069-a590-3f21ef29bd7&title=%E7%BA%BF%E7%A8%8B%E5%85%B1%E4%BA%AB%E6%95%B0%E6%8D%AE "线程共享数据")<br />从上图可以知道线程1，2和线程3分别将数据写入ABC，线程2在负责处理ABC三种读取数据并显示<br />现在基本上了解了线程和进程。想象一下，某宝级别的系统架构一开始就能抵抗这么大的流量吗，当然不是，最开始小黄页的单体架构，随着需求的复杂和多样化主键演变而来。那么浏览器依然如此。看看最开始的Chrome单进程样子。<br />最初的浏览器单进程，意味着无论是网络，页面渲染引擎还是js环境都在一个进程中，如下图所示。<br />![浏览器单进程](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891222759-328dedde-0fb9-4f87-815d-0a01c9cbd285.png#averageHue=%23ece9e1&clientId=u485f6e98-67cf-4&from=paste&id=u4939f57d&originHeight=752&originWidth=470&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud0225347-2517-4012-b9fe-1adee52fde3&title=%E6%B5%8F%E8%A7%88%E5%99%A8%E5%8D%95%E8%BF%9B%E7%A8%8B "浏览器单进程")<br />那个时候单体结构都有什么问题？

- 不稳定/不流畅

以前页面中的视频等元素需要使用插件才能观看，插件在页面进程中，插件出问题很容易导致浏览器崩溃。页面中如此多模块都运行在该线程中，一旦其中一个模块独占线程，其他的就只能当观众(ps 能不能完成了就走，别蹲着不X)，所以也就出现卡顿现象

- 安全性很难保障

当时很多插件能够比较轻松的拿到操作系统的shell，如果是页面脚本，可以通过浏览器爆出的漏铜来到shell，拿了shell就无法想象能干啥了<br />_如何解决上述问题_

- 不稳定和不流畅

原因是页面模块都在一个进程，采用进程分离，这样即使某个插件崩溃也只是影响某一部分，不会导致整个浏览器挂。

- 安全性问题

使用一个箱子(安全沙箱)，箱子里面程序可以运行且把箱子上锁，但是无法读取外部任何程序。这样的话，我把容易出错且关键的两个进程插件进程与渲染进程装进去，这样的话，即使两者之一被执行恶意程序也只是在这个箱子里瞎摆弄，无法翻越出去拿到更高的权限干坏事。<br />_当前架构_<br />最初的时候，发现使用chrome浏览器打开一个网页的有四个进程，下面我来看看这些都有什么功能<br />一共是四个进程，分别为网络进程，GPU进程，渲染进程和浏览器主进程。<br />网络进程<br />作为一个单独进程，负责页面网络资源的加载。<br />插件进程<br />由于插件容易崩溃，单独进程对其进行管理<br />GPU进程<br />Chrome中UI界面绘制和3DCSS等需要GPU计算密集性的帮助，从而引入GPU进程<br />浏览器进程<br />浏览器进程负责用户交互，各个子进程等功能<br />乍一看全是优点，通常事物都会有两面性，进程多了，开销当然也大也就是更高的资源占用和更加复杂的体系结构。
<a name="AuOqk"></a>
## 2、DNS简介
上面之所以介绍浏览器，因为DNS很多时候是在浏览器敲下回车时开始兴奋，这也是为什么从浏览器说起的原因。现在看看DNS到底是个啥玩意<br />mac地址诞生，可是太不容易记忆了，出现了简化了IP形式，它被直接暴露给外网不说，还让人类还是觉得比较麻烦，干脆用几个字母算了，也就是域名了。域名不仅仅能够代替IP，还有很多其他的用途比如在web应用中用来标识虚拟主机。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891222748-5b796615-c8f5-41c3-bcf7-73f20ffc20ff.png#averageHue=%23cffab8&clientId=u485f6e98-67cf-4&from=paste&id=ucab148a1&originHeight=332&originWidth=497&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7de9c51a-1e64-43d4-ac6c-8d08029364b&title=)
<a name="cCdaF"></a>
## 3、DNS报文结构
说了这么多，协议头部，到底有哪些字段，其含义是什么都还不知道，那怎么去分析报文，下面一起再看看报文什么样子<br />![DNS报文结构](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891222764-91c3bb14-2ead-49f3-87f2-0e8fe80b680a.png#averageHue=%23e4f1be&clientId=u485f6e98-67cf-4&from=paste&id=u352545a6&originHeight=441&originWidth=601&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u654de482-0a43-4f0a-ad3a-f76e879abee&title=DNS%E6%8A%A5%E6%96%87%E7%BB%93%E6%9E%84 "DNS报文结构")
<a name="TsVZJ"></a>
### 基础结构部分
DNS报文基础部分为DNS首部。其中包含了事务ID，标志，问题计数，回答资源计数，回答计数，权威名称服务器计数和附加资源记录数。

- 事务ID:报文标识，用来区分DNS应答报文是对哪个请求进行响应
- 标志:DNS报文中标志字段
- 问题计数：DNS查询请求了多少次
- 回答资源记录数：DNS响应了多少次
- 权威名称服务器计数: 权威名称服务器数目
- 附加资源记录数: 权威名称服务器对应IP地址的数目

基础结构中的标志字段细分如下：<br />![标志字段](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891222720-838bbcad-7871-4a15-9c0b-f572f20704a2.png#averageHue=%23cbf3b1&clientId=u485f6e98-67cf-4&from=paste&id=u55e29f60&originHeight=69&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf07c28c7-4063-467d-8c70-33865418462&title=%E6%A0%87%E5%BF%97%E5%AD%97%E6%AE%B5 "标志字段")

- QR(Response)：查询请求，值为0；响应为1
- Opcode:操作码。0表示标准查询；1表示反向查询；2服务器状态请求
- AA（Authoritative）：授权应答，该字段在响应报文中有效。通过0,1区分是否为权威服务器。如果值为 1 时，表示名称服务器是权威服务器；值为 0 时，表示不是权威服务器。
- TC（Truncated）：表示是否被截断。当值为1的时候时，说明响应超过了 512字节并已被截断，此时只返回前512个字节。
- RD（Recursion Desired）：期望递归。该字段能在一个查询中设置，并在响应中返回。该标志告诉名称服务器必须处理这个查询，这种方式被称为一个递归查询。如果该位为 0，且被请求的名称服务器没有一个授权回答，它将返回一个能解答该查询的其他名称服务器列表。这种方式被称为迭代查询。
- RA（Recursion Available）：可用递归。该字段只出现在响应报文中。当值为 1 时，表示服务器支持递归查询。
- Z：保留字段，在所有的请求和应答报文中，它的值必须为 0。
- rcode（Reply code）：通过返回值判断相应的状态。

当值为0时，表示没有错误；<br />当值为1时，表示报文格式错误（Format error），服务器不能理解请求的报文；<br />当值为2时，表示域名服务器失败（Server failure），因为服务器的原因导致没办法处理这个请求；<br />当值为3时，表示名字错误（Name Error），只有对授权域名解析服务器有意义，指出解析的域名不存在；<br />当值为4时，表示查询类型不支持（Not Implemented），即域名服务器不支持查询类型；<br />当值为5时，表示拒绝（Refused），一般是服务器由于设置的策略拒绝给出应答，如服务器不希望对某些请求者给出应答。
<a name="HtD9Z"></a>
### 问题部分
该部分是用来显示DNS查询请求的问题，其中包含正在进行的查询信息，包含查询名（被查询主机名字）、查询类型、查询类。

- 查询名:一般为查询的域名，也可能是通过IP地址进行反向查询
- 查询类型：查询请求的资源类型。常见的如果为A类型，表示通过域名获取IP。具体如下图所示

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891222725-c2a58ca0-0bef-49a5-8447-44d2dbe72148.png#averageHue=%23e9e8e8&clientId=u485f6e98-67cf-4&from=paste&id=u101a4a9e&originHeight=947&originWidth=737&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u271abdf6-2085-4ecc-b531-76e7d440af4&title=)

- 查询类：地址类型，通常为互联网地址为1
<a name="UvgwR"></a>
### 资源记录部分
资源记录部分包含回答问题区域，权威名称服务器区域字段、附加信息区域字段，格式如下<br />![资源记录部分](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891223072-c7bf7361-0abe-45bf-a918-31b65e752e07.png#averageHue=%23f2eddd&clientId=u485f6e98-67cf-4&from=paste&id=u56154b01&originHeight=362&originWidth=1007&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6654c23e-fb3b-422b-be20-7eba5210397&title=%E8%B5%84%E6%BA%90%E8%AE%B0%E5%BD%95%E9%83%A8%E5%88%86 "资源记录部分")

- 域名：所请求的域名
- 类型：与问题部分查询类型值一直
- 类：地址类型，和问题部分查询类值一样
- 生存时间：以秒为单位，表示资源记录的生命周期
- 资源数据长度：资源数据的长度
- 资源数据：按照查询要求返回的相关资源数据
<a name="otbdr"></a>
## 4、DNS解析详解
知道了DNS大概是什么，它的域名结构和报文结构，是时候看看到底怎么解析的以及如何保证域名的解析比较稳定和可靠<br />_DNS核心系统_

- 根域名服务器(Root DNS Server)管理顶级域名服务并放回顶级域名服务器IP，比如"com","cn"
- 顶级域名服务器(Top-level DNS Server)每个顶级域名服务器管理各自下属，比如com可以返回baidu.com域名服务器的IP
- 权威域名服务器(Authoritative DNS Server)管理当前域名下的IP地址，比如Tencent.com可以返回www.tencent.com的IP地址

![核心系统](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891223087-97206bf6-0c3f-408e-af3f-5868ba88e5f5.png#averageHue=%23fbfbfb&clientId=u485f6e98-67cf-4&from=paste&id=u94f071ad&originHeight=722&originWidth=836&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uad148a1d-aaab-4323-8b24-a3530be6b35&title=%E6%A0%B8%E5%BF%83%E7%B3%BB%E7%BB%9F "核心系统")<br />举个例子，假设访问"www.google.com"

- 访问根域名服务器，这样就会知道"com"顶级域名的地址
- 访问"com"顶级域名服务器，可知道"google.com"域名服务器的地址
- 最后方位"google.com"域名服务器，就可知道"www.google.com"的IP地址

嘿嘿，目前全世界13组根域名服务器还有上百太镜像，但是为了让它能力更强，处理任务效率更高，尽量减少域名解析的压力，通常会加一层"缓存"，意思是如果访问过了，就缓存，下一次再访问就直接取出，也就是咱么经常配置的"8.8.8.8"等<br />操作系统中同样也对DND解析做缓存，比如说曾访问过"www.google.com"，<br />其次，还有熟知的hosts文件，当在操作系统中没有命中则会在hosts中寻找。<br />这样依赖，相当于有了DNS服务器，操作系统的缓存和hosts文件，能就近(缓存)完成解析就好，不用每次都跑到很远的地方去解析，这样大大减轻的DNS服务器的压力。画了一个图，加深印象<br />![DNS解析过程](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891245969-7105e02d-bad8-4c29-9ba2-483ea3e3372f.png#averageHue=%23f3f3eb&clientId=u485f6e98-67cf-4&from=paste&id=u51b13391&originHeight=543&originWidth=828&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u3b3e7ce9-3bc7-4d17-9249-d01c1cdec1e&title=DNS%E8%A7%A3%E6%9E%90%E8%BF%87%E7%A8%8B "DNS解析过程")<br />想必大家应该知道这个过程了，再举个例子，假设访问www.qq.com

- 客户端发送一个DNS请求，请问qq你的IP的什么啊，同时会在本地域名服务器(一般是网络服务是临近机房)打声招呼
- 本地收到请求以后，服务器会有个域名与IP的映射表。如果存在，则会告诉你，如果想访问qq，那么你就访问XX地址。不存在则会去问上级(根域服务器):"老铁，你能告诉我www.qq.com"的IP么
- 根DNS收到本地DNS请求后，发现是.com，"www.qq.com哟，这个由.com大哥管理，马上给你它的顶级域名地址，你去问问它就好了"
- 这个时候，本地DNS跑去问顶级域名服务器，"老哥，能告诉下www.qq.com"的ip地址码"，这些顶级域名负责二级域名比如qq.com
- 顶级域名回复："小本本记好，我给你www.qq.com区域的权威DNS服务器地址"，它会告诉你
- 本地DNS问权威DNS服务器："兄弟，能不能告诉我www.qq.com对应IP是啥"
- 权威DNS服务器查询后将响应的IP地址告诉了本地DNS，本地服务器将IP地址返回给客户端，从而建立连接。
<a name="fGUKh"></a>
## 5、DNS进阶之新玩法
这里主要分享DNS(GSLB)的全局负载均衡。不是所有的互联网服务都适用于GSLB。<br />全局负载均衡采用的主要技术是智能DNS，它综合多种不同的策略(比如根据地理位置或者根据繁忙程度的权重)将客户访问的域名解析到不同的线路上。开启介绍之前，再一次复习下DNS中A记录和NS记录

- A记录

A记录是名称解析的重要记录，它用于将特定的主机名映射到对应主机的IP地址上。你可以在DNS服务器中手动创建或通过DNS客户端动态更新来创建

- NS记录

NS记录此记录指定负责此DNS区域的权威名称服务器。

- 两者区别

A记录直接给出目的IP，NS记录将DNS解析任务交给特定的服务器，NS记录中记录的IP即为该特定服务器的IP地址<br />在全局负载均衡解决方案中，NS记录指向具有智能DNS解析功能的GSLB设备，通过GSLB设备进行A记录解析。为了保证高可用，如果为多地部署GSLB，则均配置记录。另外，GSLB设备还会对所在的后端服务器公网IP进行健康检查，其结果通过自有协议在不同的的GLSB设备间同步。解析的步骤如下图<br />![智能DNS解析](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891245936-bf2952a1-0570-4f72-bec2-f0c9496b2102.png#averageHue=%23f6f3ef&clientId=u485f6e98-67cf-4&from=paste&id=u6b5e8db0&originHeight=798&originWidth=866&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9023cb2a-6d63-4ef4-bc25-556ed30c591&title=%E6%99%BA%E8%83%BDDNS%E8%A7%A3%E6%9E%90 "智能DNS解析")

- 用户给本地DNS服务器发送查询请求，如果本地有缓存直接返回给用户，否则通过递归查询获得名服务商商处的授权DNS服务器
- 授权服务器返回NS记录给本地DNS服务器。其中NS记录指向一个GSLB设备接口地址
- GSLB设备决策最优解析结果并返回A记录给本地DNS服务器。
- 本地服务器将查询结果通过一条A记录返回给用户，并缓存这条记录。
<a name="NW7mY"></a>
## 6、DNS实战(wireshark)
使用工具为wireshark，访问www.baidu.com<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891245959-e1426de6-8c98-482c-af59-9699214148a0.png#averageHue=%23cce4f7&clientId=u485f6e98-67cf-4&from=paste&id=u5460d66b&originHeight=146&originWidth=994&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6ed0ef18-f1f7-4dd1-9086-5d086a99e0d&title=)<br />分析DNS请求帧，如下图所示<br />![DNS请求帧](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891246000-690f8c5a-045a-4a25-9def-cdce502d5736.png#averageHue=%23f9f4f3&clientId=u485f6e98-67cf-4&from=paste&id=ucfe141ca&originHeight=313&originWidth=787&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u8ea1920c-acf1-4efe-b5aa-01b2ec1c61d&title=DNS%E8%AF%B7%E6%B1%82%E5%B8%A7 "DNS请求帧")<br />从上图可知道请求计数为1，请求的域名为dss0.bdstatic.com
<a name="ZLv9m"></a>
### 分析DNS响应帧
![DNS响应](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891245981-04183c7f-2450-4bde-a0ef-8001ab267a06.png#averageHue=%23f8f3f2&clientId=u485f6e98-67cf-4&from=paste&id=uc7570eb8&originHeight=304&originWidth=715&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub50b51c2-5598-4528-a530-974992e4a9b&title=DNS%E5%93%8D%E5%BA%94 "DNS响应")<br />从响应头可以知道，问题计数为1，正好对应请求帧1个问题。回应了2个问题。分别为<br />![answers](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891246221-72f89e7e-a818-4c0c-87eb-3bd938995204.png#averageHue=%23faf6f5&clientId=u485f6e98-67cf-4&from=paste&id=u6e8e9df1&originHeight=242&originWidth=788&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u1d4d236b-e377-45d5-8354-5101156a3cd&title=answers "answers")<br />![权威域名服务器](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891246374-b70942bd-8c97-41b6-aa22-cbb5c3f6e5b4.png#averageHue=%23f3eeea&clientId=u485f6e98-67cf-4&from=paste&id=u576c521e&originHeight=258&originWidth=256&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u40db58dc-1566-41ac-b9c4-1eddbad9ae7&title=%E6%9D%83%E5%A8%81%E5%9F%9F%E5%90%8D%E6%9C%8D%E5%8A%A1%E5%99%A8 "权威域名服务器")<br />从上图可以得出当前共有13个权威域名服务器，当然每一个的服务器地址不同，其中类型为NS代表权威域名服务器服务器<br />两个相似面试题
<a name="iUfb2"></a>
## 7、使用IP地址访问浏览器的原理

- 打开chrome浏览器，输入IP
- 三次握手建立连接
- 建立连接以后HTTP开始工作，通过TCP发送一个"GET / HTTP/1.1"，服务端给予回应
- 解析请求，根据HTTP协议规定解析，看看那浏览器想干啥
- 哦，原来你想获取我的视频呀，那我读出来拼接为HTTP格式给你，回复"HTTP/1.1 200 OK"
- 作为浏览器回复一个TCP的ACK表示确认
- 浏览器收到响应数据后，需要使用相应的引擎进行渲染，将更好的页面展现给用户
<a name="P4O4x"></a>
## 8、使用域名访问浏览器的原理
这一次从浏览器角度回答，相信大家已经了解一部分浏览器知识了，先看看URL到网页展示的完整流程是什么样子<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891246388-660db031-d1b2-4794-b93a-2b33bb736c3c.png#averageHue=%23f8f7f7&clientId=u485f6e98-67cf-4&from=paste&id=u4d592e5f&originHeight=276&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u54ec9abd-99b3-471c-acba-c9a33d08720&title=)

- 用户输入

在地址栏输入相应的内容，如果为关键字，如果直接输入搜索内容，浏览器默认引擎会合成为URL，如果符合URL规则，加上协议合成完整URL，回车就会出现加载页面，也就是等待提交文档的阶段

- URL请求过程

此时浏览器进程将URL通过进程间通信的方式发送给网络进程，开启真正的请求流程。注意了，网络进程这里也有缓存，它会现在本地缓存查看是否缓存了资源，如果有则直接返回。如果没有，那就需要DNS解析获取服务器IP地址(HTTPS还少不了TLS连接)<br />此时使用IP和服务器建立三次握手。连接成功开始构造请求头等信息。<br />服务器收到请求信，根据请求信息生成响应信息给网络进程。然后开始解析响应头内容。如果返回值为302/301，说明需要跳转到其他URL，如果为200则继续处理该请求。<br />URL的请求数据类型多种，对于浏览器而言是怎么区分的呢<br />这个时候就必须强调下Content-type了，因为他明确服务器返回响应体数据属于什么类型，此时的浏览器也会根据Content-type对决定响应体是什么内容

- 进入渲染阶段

通常情况下，当前多进程架构的浏览器对于每一个页面都有一个渲染进程，前提是如果从X页面打开Y页面，x和y属于同一个"站点"(使用相同的协议和根域名)，此时y页面会复用x页面，否则y页面会单独对应一个渲染进程。

- 提交阶段

渲染进程收到浏览器进程的"提交文档"后，通过和网络进程使用"管道"的方式通信。一旦这些文档数据传输完成，渲染进程就会告诉浏览器进程"确认提交"，此时浏览器进程收到"确认提交"就会更新地址栏的URL，历史状态等，这就是为什么当在地址栏输入地址信息后需要加载一小会儿到另一个页面。over

- 渲染阶段

文档提交以后，此时就需要使用js，css等美化页面，并通过构建DOM树等让用户有更好的使用体验。
<a name="Xbwce"></a>
## 9、DNS劫持
到这里至少知道了DNS可以将域名映射为IP，并且知道了使用了多种缓存方案来减少DNS访问的压力。那么DNS一旦出错，很可能将域名解析到其他IP地址，这样也就无法正确访问网页(PS是不是有的时候发现开启不了网页但是qq等可以使用，很可能就是DNS搞鬼了)<br />![DNS劫持](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891246440-6d7bb5f8-b8a1-4a12-8ae4-b2d6fb787612.png#averageHue=%23f8f6f1&clientId=u485f6e98-67cf-4&from=paste&id=u143d6682&originHeight=514&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5f960bd2-83e7-43b0-9e9e-f2be8af9d1a&title=DNS%E5%8A%AB%E6%8C%81 "DNS劫持")

- 利用DNS服务器进行DDOS攻击

什么是DDOS，应该知道SYN Flood，是一种DoS(拒绝服务攻击)与DDOS(分布式拒绝服务攻击的方式)，利用大量的伪造TCP请求让被攻击方资源榨干。<br />![DDOS](https://cdn.nlark.com/yuque/0/2023/png/396745/1686891246420-7572559c-dcc8-40be-a79b-edb5e9fa9ac6.png#averageHue=%23f1ece7&clientId=u485f6e98-67cf-4&from=paste&id=u87ee0e9b&originHeight=513&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc45c638e-9ff9-482f-880d-aea594ac017&title=DDOS "DDOS")<br />假设攻击者已经知道了攻击者IP(如果需要了解这一部分内容，可以去搜索主动被动信息搜集/sodan等关键字)，此时攻击者使用此地址作为解析命令的源地址，当DNS请求的时候返回恰巧也是被攻击者。此时如果足够多的请求(群肉鸡)就很容易使网络崩溃。

- 缓存感染

已经知道了在DNS查询过程中，会经过操作系统的缓存，hosts文件等，如果将数据放入有漏洞的服务器缓存中，当进行DNS请求的时候，就会将缓存信息返回给用户，这样用户就会莫名访问入侵者所设置的陷阱页面中。

- DNS信息劫持

看到这里的小伙伴，先思考一个问题，在TCP/IP协议栈中，三次握手中的序列号到底什么意思？<br />其功能之一就是避免伪装数据的插入。如果知道DNS报文中的ID，就可以知道这个ID请球员位置。作为攻击者，会通过旁路监听客户端和服务端的会话，拿到DNS中的ID，此时相当于在DNS服务器之前拿到ID，伪装DNS服务器回复客户端，引导客户端访问恶意的网站<br />_电脑小故障_<br />比如qq可用但是浏览器就是不好使

- 输入：http://192.168.1.1(可能是http://192.168.0.1)，输入路由器用户名密码
- DHCP服务器-----DHCP服务-，修改DNS为更加可靠的DNS服务器IP.保存即可

方法2:修改路由器password

- 地址栏输入"http://192.168.1.1"，登录并进入路由器页面
- 系统工具--修改登录口令页面

_保护域名/尽量避免攻击_

- 备份策略。一般至少会使用两个域名，一旦其中一个被攻击，用户可以通过另一个访问
- 随时留意域名注册中的电子邮件
- 保存好所有权信息(比如账单记录，注册信息等)
- 随时关注安全补丁
<a name="aY0j7"></a>
## 10、涉及高频面试题

- 讲讲DNS原理
- 进程与线程
- 递归查询和递归查询区别
- DNS解析流程
- chrome架构演变
- ELF是什么，数据段，代码段，全局变量等分别存放在哪儿
- DNS劫持
- 描述下DDOS与DOS攻击
- 使用IP地址访问web服务器
- 使用域名访问web服务器过程
- 可重定位什么意思？
- 静态库与动态库的区别
- 进程与线程间共享数据
