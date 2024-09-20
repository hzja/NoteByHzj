JavaSocket
<a name="lbXij"></a>
## 1、QuickServer
一个免费的开源Java库，用于快速创建健壮的多线程、多客户端TCP服务器应用程序。使用QuickServer，用户可以只集中处理应用程序的逻辑/协议。
<a name="jMKRA"></a>
## 2、Netty 2
提供了一组基于事件的API来开发高性能，可管理的TCP/IP服务端或客户端应用程序，对于要求建立高性能的网络应用来说，Netty提供许多基本特性，像readiness selection，线程池，写缓冲DoS的预防，可重用的缓冲等。
<a name="V7VFs"></a>
## 3、ColoradoFTP
一个开源完全遵循RFC 959(文件传输协议)的Java FTP服务器。它具有快速，可靠，易于扩展的特点。
<a name="AcssL"></a>
## 4、Apache James
完全采用纯Java技术开发，实现了SMTP、POP3与NNTP等多种邮件相关协议。James也是一个邮件应用平台，可以通过Mailet扩充其功能，如Mail2SMS、Mail2Fax等。James提供了比较完善的配置方案，尤其是关于邮件内容存储和用户信息存储。
<a name="QJeH5"></a>
## 5、Java Email Server(JES)
支持SMTP和POP3协议纯Java mail服务器。
<a name="iCRux"></a>
## 6、Java Sockets
Java Sockets是一个Java类库模仿了C++ Sockets library的实现方式并基于SUN的java.nio.*非阻塞网络I/O类。
<a name="zVlXy"></a>
## 7、Raining Sockets
一个非阻塞的sockets框架.开发者可以利用它来构建高性能的可以发送与接收10000 socket连接的应用程序。
<a name="VpjM8"></a>
## 8、Cindy
基于java nio的，提供一个简单，高效的异步I/O框架，支持tcp/udp/pipe并易于测试。
<a name="R73bk"></a>
## 9、HP-Socket
HP-Socket：是一套通用的高性能 TCP/UDP 通信框架，包含服务端组件、客户端组件和Agent组件，广泛适用于各种不同应用场景的TCP/UDP通信系统，提供C/C++、C#、Delphi、E（易语言）、Java、Python等编程语言接口。HP-Socket对通信层实现完全封装，应用程序不必关注通信层的任何细节；HP-Socket提供基于事件驱动模型的API接口，能非常简单高效地整合到新旧应用程序中。<br />HP-Socket 目前运行在 Windows 平台，将来会实现跨平台支持。<br />Client 组件：<br />基于 Event Select 通信模型，在单独线程中执行通信操作，避免与主线程或其他线程相互干扰。每个组件对象管理一个Socket连接。<br />Server 组件：<br />基于 IOCP 通信模型，并结合缓存池、私有堆（Private Heap）等技术，支持超大规模连接，在高并发场景下实现高效内存管理。<br />Agent 组件：<br />对于代理服务器或中转服务器等应用场景，服务器自身也作为客户端向其它服务器发起大规模连接，一个 Agent组件对象同时可管理多个Socket连接；Agent组件与Server组件采用相同的技术架构，可以用作代理服务器或中转服务器的客户端部件。
<a name="krZGF"></a>
## 10、Grizzl
设计与一般的 nio 框架相比是比较不同的，主要不同点在于读和写都是采用 blocking 方式，并且使用临时 selector；线程模型高度可配置。性能据说比 MINA 还高，但是学习曲线很高。
<a name="lnxUo"></a>
## 11、xSocket
一个轻量级的解决方案，核心思想是屏蔽，简化 nio 方式的的开发，并不需要过多的学习。
