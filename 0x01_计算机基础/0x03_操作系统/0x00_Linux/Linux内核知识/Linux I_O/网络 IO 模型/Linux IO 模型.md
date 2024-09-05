先抛出一个问题，基于此问题引出文章的主题：1999 年 Dan Kegel 在其个人站点提出了 C10K问题，首字母 C 是 Client 的缩写，C10K 即单机同时处理 1 万个连接的问题。C10K 表示处理 10000 个并发连接，注意这里的并发连接和每秒请求数不同，虽然它们是相似的，每秒处理许多请求需要很高的吞吐量（快速处理它们），但是更大数量的并发连接需要高效的连接调度，即 I/O 模型的问题。<br />道听途说：早期 QQ 也面临 C10K 问题，所以他们使用 UDP 绕开了这个问题。UDP 协议是无连接方式的协议，它的效率高，速度快，占资源少，但是其传输机制为不可靠传送，必须依靠辅助的算法来完成传输控制。
<a name="Ex8xu"></a>
## BIO
BIO（Blocking I/O）是最传统的 I/O 模型，它是阻塞的，即在进行 I/O 操作时，转到内核空间处理，应用程序会被阻塞，直到操作完成。对于每个连接或文件，通常需要创建一个单独的线程或进程来处理，这会导致资源消耗很高。BIO 不适用并发量大的应用，因为每个 I/O 请求都会阻塞进程，所以，需要为每请求分配一个处理进程（线程）以及时响应，系统开销大。<br />基于 BIO 去解决 C10K 问题，就需要创建 1 万个进程（线程），这显然是不现实的，系统无法支撑如此庞大的开销。而且进程（线程）间的频繁切换，也会导致性能的严重下降，系统的绝大部分资源都用来做进程（线程）的上下文切换。<br />下面是一段服务端基于 BIO 的实现，每一个 Client 都会创建一个线程来处理该连接：
```java
public void serverStartBIO() {
    // 创建服务端Socket, 监听8080 端口
    ServerSocket server = new ServerSocket(8080);
    while (true) {
        // 阻塞等待 Client 连接
        Socket client = server.accept(); 
        // 退出阻塞代表有客户端连接进来
        new Thread(new Runnable(){
            public void run() {
                InputStream in = client.getInputStream();
                BufferedReader reader = new BufferedReader(new InputStreamReader(in));
                while(true){
                    // 阻塞等待客户端发送消息
                    String dataline = reader.readLine(); 
                    if(null != dataline){
                        // 读取数据成功
                    }else{
                        // 关闭客户端
                        client.close();
                    }
                }
                System.out.println("客户端断开");
            }
        }).start();
    }
}
```
<a name="XvYcJ"></a>
## NIO
NIO 是一种相对于 BIO 更高效的 I/O 模型，它引入了通道（Channel）和缓冲区（Buffer）的概念，允许应用程序进行非阻塞 I/O 操作。NIO 解决了 BIO 需要大量进程（线程）造成资源浪费的问题，它允许程序在进行 I/O 操作时继续执行其他任务，而不必等待操作完成。它基于以下几个关键概念：

- **非阻塞调用：**程序可以使用非阻塞的系统调用来执行 I/O 操作，这些调用不会阻塞程序的执行，即使数据尚未准备好。
- **轮询：**程序会周期性地轮询或检查 I/O 操作的状态，以确定它们是否已经完成。这可以通过使用系统调用如 select、poll、epoll（Linux系统）或异步事件处理来实现。
- **回调函数：**另一种方法是使用回调函数，程序发起一个 I/O 操作，然后指定一个回调函数，以便在操作完成时被调用。这样，程序可以继续执行其他任务，而无需轮询。

NIO 具有多种优势和应用场景：

- **高并发处理：**在服务器应用中，NIO 可以帮助服务器同时处理大量并发连接，而不会因等待 I/O 操作而陷入停滞状态。
- **实时应用：**对于需要快速响应的实时应用，如在线游戏或聊天应用，非阻塞 I/O 可以确保系统不会因等待 I/O 操作而引起延迟。
- **节约资源：**NIO 可以节省系统资源，因为程序不需要创建大量的线程或进程来处理并发连接。
- **异步编程：**NIO 是异步编程的核心，允许程序执行其他任务而不必等待 I/O 完成。这在事件驱动编程中非常有用。
<a name="OO02u"></a>
### 非阻塞调用
早期的NIO 只是 read、write、recv 和 send 的调用不会阻塞，每次调用可以通过返回值来判断是否有数据下面是非阻塞调用的实现：
```java
public void serverStartNIO() {
    // list 用户维护所有的 Client 连接
    LinkedList<SocketChannel> clients = new LinkedList<>();
    // 创建服务端
    ServerSocketChannel ss = ServerSocketChannel.open();  //服务端开启监听：接受客户端
    ss.bind(new InetSocketAddress(8080));
    // 客服务端监听 connect 请求为非阻塞, 注意: 仅仅是服务端监听客户端连接请求非阻塞
    ss.configureBlocking(false); 
    while (true) {
        // 因为接受客户端连接 accept 是非阻塞, 为了避免没有连接时死循环, CPU跑满需要 sleep 一下
        Thread.sleep(1000);
        // 接受客户端的连接
        // accept  调用内核了：
        //   1，没有客户端连接进来，在BIO 的时候一直卡着，但是在NIO不卡着，返回-1，NULL
        //   2. 如果来客户端的连接，accept 返回的是这个客户端的fd
        SocketChannel client = ss.accept(); 

        // NON BLOCKING 就是代码能往下走了，只不过有不同的情况
        if (client == null) {
            //  没有客户端连接进来，在BIO 的时候一直卡着，但是在NIO不卡着，返回-1，NULL
        } else {
            // 设置该连接 read, write 为非阻塞
            client.configureBlocking(false);
            // 把连接加入列表, 统一管理, 循环遍历
            clients.add(client);
        }

        // 遍历已经链接进来的客户端能不能读写数据
        for (SocketChannel c : clients) {
            // client.configureBlocking(false) 设置的效果就是 下面 read 操作不会阻塞 
            int num = c.read(buffer);
            if (num > 0) {
                // num > 0 表示读取到了数据
            }
        }
    }
}
```
从上面的程序中不难发现：非阻塞调用模式下，所有客户端的链接需要在程序中自己管理，进程(现成)需要不停的循环遍历每个链接是否可读、 可写、关闭连接，大大增加了网络编程的难度。<br />这种模式存在以下的问题：

- 程序需要管理每个链接，增加了应用程序的复杂度；
- 每次循环需要遍历每个链接，即使该连接没有数据达到，造成资源浪费；
- 遍历每个链接，调用 read 、write 都需要转到内核空间处理，频繁的内核态和用户态的切换。
<a name="VQCLE"></a>
### select
select 解决了非阻塞调用的部分问题，select 是一种 I/O 多路复用模型，它具有以下优势：

1. **高效的并发处理：**select 允许一个进程同时管理多个文件描述符（sockets、文件、标准输入等），它可以监视多个文件描述符，并在它们中的任何一个变为可读、可写时通知应用程序。这使得一个进程可以有效地处理大量并发连接，而无需为每个连接创建一个新线程或进程，减少了资源消耗。
2. **简单的编程模型：**使用 select 可以让开发者避免复杂的多线程或多进程编程，开发者可以在一个单一的事件循环中处理多个连接，而不必担心线程同步和互斥。
3. **跨平台性：**select 是一个标准的 POSIX 系统调用，因此它在大多数 UNIX-like 操作系统上可用，包括 Linux、macOS 和各种 BSD 变种。它还在 Windows 平台上有对应的实现，如 select()、WSASelect()。
4. **支持非阻塞 I/O：**select 允许将文件描述符设置为非阻塞模式，从而实现非阻塞 I/O。这意味着应用程序可以继续执行其他任务，而不必等待数据准备好。
5. **可定制性：**select 允许应用程序监视不同类型的事件，如可读、可写、异常等，这使得开发者能够自定义事件处理逻辑，以适应各种应用场景。

先看下 select 函数的定义
```c
// 返回：若有就绪描述符则为其数目，若超时则为 0，若出错则为 -1
int select(int maxfd, 
           fd_set *readset, 
           fd_set *writeset, 
           fd_set *exceptset, 
           const struct timeval *timeout);
```
select 参数的含义如下：

- **maxfd：** 待监控的描述符基数，为待测试的最大描述符加 1。
- **readset/writeset/exceptset：** 分别为读描述符集合、写描述符集合、异常描述符集合，这三个分别通知内核，在哪些描述描述符上检测数据可以读、可写、有异常事件发生。
- **timeout：** 可以设置阻塞时间，如果为 null 代表一直阻塞。

简单来说，select 可以让程序不用再创建一个列表来管理所有的连接， 而是将所有的加入 select 的对应集合中，由 select 去内核查询每个连接对应的事件。下面是 select 的实现：
```java
public void serverStartSelect(){
    // 启动服务端监听
    ServerSocketChannel server = ServerSocketChannel.open();
    server.configureBlocking(false);
    server.bind(new InetSocketAddress(8080));

    // register select
    Selector selector = Selector.open(); 
    server.register(selector, SelectionKey.OP_ACCEPT);

    while (true) {  //死循环
        Set<SelectionKey> keys = selector.keys();
        while (selector.select() > 0) {
            // 返回的有状态的 fd 集合
            Set<SelectionKey> selectionKeys = selector.selectedKeys();  
            Iterator<SelectionKey> iter = selectionKeys.iterator();
            // 如果 selectionKeys 不为空,表示存在连接 可读, 可写
            while (iter.hasNext()) {
                SelectionKey key = iter.next();
                iter.remove(); 
                if (key.isAcceptable()) {
                    // 有 client 连接进来了, accept 建立连接
                    ServerSocketChannel ssc = (ServerSocketChannel) key.channel();
                    SocketChannel client = ssc.accept(); 
                    client.configureBlocking(false);
                    // 将新建立的链接加入 select 管理
                    client.register(selector, SelectionKey.OP_READ, buffer);
                } else if (key.isReadable()) {
                    // 存在连接可读, 可以 read 读取该连接的数据
                    read = client.read();
                }
            }
        }
    }
}
```
在 select 模式下，应用程序将所有的连接交给 select 管理，程序中只需要关注 select 的返回数据，处理对应的事件。这大大降低了程序的复杂度，看起来 select 很好，但在生产上用处依然不多。主要是因为 select 有以下劣势：

1. 每次调用 select，都需要把 fdset 从用户态拷贝到内核态，在高并发下是个巨大的性能开销（可优化为不拷贝）；
2. 调用 select 阻塞后，用户进程虽然没有轮询，但在内核还是通过遍历的方式来检查 fd 的就绪状态（可通过异步 IO 唤醒的方式）；
3. select 只返回已就绪 fd 的数量，用户线程还得再遍历所有的 fd 查看哪些 fd 已准备好了事件（可优化为直接返回给用户进程数据已就绪的 fd 列表）。
```c
/linux/posix_types.h:

# define __FD_SETSIZE         1024
```
此外在 Linux 内核中，select 所用到的 FD_SET 是有限的，即内核中有个参数 __FD_SETSIZE 定义了每个 FD_SET 的句柄个数。所以 select 限制了 maxfd 的大小为 1024，表示只能监听 1024 个 fd 的事件，select 适用于小规模并发连接的应用，如聊天服务器、网络游戏服务器等，而不需要复杂的线程或进程管理，这离 C10k 显然还是有距离的。
<a name="qEcHM"></a>
### poll
poll 的机制其实和 select 一样，唯一比较大的区别其实是把 1024 这个限制给放开了，虽然通过放开限制可以使内核监听上万 socket，但由于以上说的两点劣势，它的性能依然不高。 同 select 一样 poll 适用于小规模的并发连接，通常对于处理百甚至千级并发连接足够。<br />看下 poll 的函数定义：
```c
int poll(struct pollfd *fds, nfds_t nfds, int timeout)
```

- fds：是一个指向 pollfd 结构数组的指针;
- nfds：是数组中的文件描述符数量;
- timeout：是超时时间（以毫秒为单位）
   - timeout = -1：poll 将一直阻塞直到有文件描述符准备好；
   - timeout = 0：poll 立即返回，不等待；
   - timeout > 0：poll 将等待指定的时间。
- **返回值：**是就绪文件描述符的数量，或者在超时或出错时返回 0。
<a name="ZVzfk"></a>
### epoll
epoll 是 Linux 操作系统中的一种高效 I/O 多路复用机制，用于监视多个文件描述符的状态，以便进行非阻塞I/O操作。它是对 select 和 poll 的改进，提供了更好的性能和扩展性，特别适用于处理大规模并发连接，如 Web 服务器、聊天服务器和网络游戏服务器等，因为它具有出色的性能和扩展性。它主要在 Linux 操作系统上可用，而不是跨平台的，因此不适用于需要跨平台支持的应用程序。<br />在 Linux 2.4 内核前主要是 select 和 poll，自 Linux 2.6 内核正式引入 epoll 以来，epoll 已经成为了目前实现高性能网络服务器的必备技术。<br />epoll 使用了3个方法来实现 select 方法要做的事：

- `**epoll_create()**`：新建 epoll 描述符
- `epoll_ctrl()` ：添加或者删除所有待监控的连接
- `epoll_wait()`：返回的活跃连接

与 select 相比，epoll 分清了频繁调用和不频繁调用的操作。例如，epoll_ctrl 是不太频繁调用的，而 epoll_wait 是非常频繁调用的。这时，epoll_wait 却几乎没有入参，这比 select 的效率高出一大截。而且它也不会随着并发连接的增加使得入参越发多起来，导致内核执行效率下降。<br />除此之外，epoll 还增加了很多机制，用来提升性能：

- **mmap（共享内存）：**epoll 是通过内核与用户空间 mmap 同一块内存实现的。mmap 将用户空间的一块地址和内核空间的一块地址同时映射到相同的一块物理内存地址（不管是用户空间还是内核空间都是虚拟地址，最终要通过地址映射映射到物理地址），使得这块物理内存对内核和对用户均可见，减少用户态和内核态之间的数据交换。内核可以直接看到 epoll 监听的句柄，效率高。
- **红黑树：**红黑树将存储 epoll 所监听的套接字。上面 mmap 出来的内存如何保存 epoll 所监听的套接字，必然也得有一套数据结构，epoll 在实现上采用红黑树去存储所有套接字，当添加或者删除一个套接字时（epoll_ctl），都在红黑树上去处理，红黑树本身插入和删除性能比较好，时间复杂度 O(logN)。
- **链表：**链表用于存储所有的事件，当监听的事件发生后，内核会回调 ep_poll_callback 函数把这个事件添加到 rdllist 这个双向链表中。一旦有事件发生，epoll 就会将该事件添加到双向链表中。那么当我们调用 epoll_wait 时，epoll_wait 只需要检查 rdlist 双向链表中是否有存在注册的事件，效率非常可观。这里也需要将发生了的事件复制到用户态内存中即可。

下面看下 epoll 的相关函数
```c
#include <sys/epoll.h>
// 该 size 已经没有意义, epoll 早期实现中, 对于监控文件描述符的组织并不是使用红黑树，而是hash表
int epoll_create ( int size );

// epfd: epoll的描述符, epoll_create 的返回值
// fd：要操作的文件描述符
// op：指定操作类型
//     EPOLL_CTL_ADD：往事件表中注册fd上的事件
//     EPOLL_CTL_MOD：修改fd上的注册事件
//     EPOLL_CTL_DEL：删除fd上的注册事件
// event：指定事件 可读，可写及异常等事件
int epoll_ctl ( int epfd, int op, int fd, struct epoll_event *event );

// epfd: epoll的描述符, epoll_create 的返回值
// events：检测到事件，将所有就绪的事件从内核事件表中复制到它的第二个参数events指向的数组中。
// maxevents：指定最多监听多少个事件
// timeout：指定epoll的超时时间，单位是毫秒。当timeout为-1是，epoll_wait调用将永远阻塞，直到某个时间发生。当timeout为0时，epoll_wait调用将立即返回。
// 返回：成功时返回就绪的文件描述符的个数，失败时返回-1并设置errno  
int epoll_wait ( int epfd, struct epoll_event* events, int maxevents, int timeout );
```
因为 epoll 是 Linux 特有的，在 Java 中，要使用 epoll 机制，通常需要通过 JNI（Java Native Interface）来调用操作系统特定的 epoll 函数。以下为 基于 C++ 实现的 epoll 示例：
```c
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

int create_and_bind(int port) {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // 设置 socket 属性
    struct sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    // 绑定端口
    bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))
        return listen_fd;
}

int main() {
    // 创建服务端
    int listen_fd = create_and_bind(8080);
    listen(listen_fd, 10)

        // 创建 epoll 
        int epoll_fd = epoll_create1(0);

    // 初始化epoll结构并将服务端 fd 加入 epoll
    struct epoll_event event{};
    event.data.fd = listen_fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    struct epoll_event events[64];
    while (true) {
        // 等待 epoll 返回事件
        int num_events = epoll_wait(epoll_fd, events, 64, -1);
        // 处理事件
        for (int i = 0; i < num_events; ++i) {
            // 如果是服务端监听的 fd, 那么表示新的客户端连接进来
            if (events[i].data.fd == listen_fd) {
                // 调用 accept, 接受客户端连接
                int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
                // 将新的连接也加入到 epoll 中    
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            } else {
                // 如果不是服务端监听的 fd, 那么就是与客户端的连接有数据进来
                // read 方法读数据
                ssize_t n = read(client_fd, buffer, sizeof(buffer));
                if (n <= 0) {
                    // 客户端断开连接
                } else {
                    // 数据读取成功, 处理业务
                }
            }
        }
    }

    close(listen_fd);
    return 0;
}
```
<a name="Sl8jh"></a>
## AIO
AIO 是一种异步的 I/O 模型，它允许应用程序发起 I/O 请求后立即返回，而不需要等待 I/O 操作完成。操作系统负责在后台执行 I/O 操作，当操作完成时，应用程序会收到通知。AIO 适用于需要高度并发、高性能的应用，如网络服务器、数据库管理系统等，其中 I/O 操作可能是性能瓶颈之一。但 AIO 的编程模型相对复杂，通常不适用于所有类型的应用。在某些情况下，使用更简单的 I/O 多路复用模型（如epoll、kqueue）可能更为合适。AIO 的实现方式因操作系统而异。在 Linux 中，libaio 库提供了对 AIO 的支持，而在 Windows 上，IOCP（I/O Completion Port）是其异步 I/O 模型。<br />**AIO 的优势**：

- **非阻塞：**AIO 允许应用程序继续执行其他任务，而不需要等待 I/O 操作完成。这可以提高应用程序的并发性和响应性。
- **性能：**对于某些 I/O 密集型应用，使用 AIO 可以提高性能，因为它能够在后台并行处理多个 I/O 请求。
- **可扩展性：**AIO 适用于需要大规模并发连接的应用，如高性能的网络服务器，因为它不需要为每个 I/O 请求创建一个新的线程或进程。

Java提供了对异步 I/O（AIO）的支持，它使用 java.nio.channels.AsynchronousFileChannel 和java.nio.channels.AsynchronousSocketChannel 等类来执行异步 I/O 操作。这些类允许应用程序发起 I/O 请求后，不需要等待操作完成，而可以在后台执行其他任务。当 I/O 操作完成时，应用程序会收到通知。以下是一个简单的 Java AIO 示例
```java
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;

public class AioServer {
    public static void main(String[] args) throws IOException {
        AsynchronousServerSocketChannel serverChannel = AsynchronousServerSocketChannel.open().bind(new InetSocketAddress(8080));
        // 设置当有连接进来时, 回调方法
        serverChannel.accept(null, new CompletionHandler<AsynchronousSocketChannel, Void>() {
            @Override
            public void completed(AsynchronousSocketChannel clientChannel, Void attachment) {
                // accept 客户端连接
                serverChannel.accept(null, this);
                // 处理该 Client 的链接
                handleClient(clientChannel);
            }
            @Override
            public void failed(Throwable exc, Void attachment) {
                // 处理链接失败
            }
        });

        // 由于是异步执行, 执行完上面代码主线程就会退出
        // join 让主线程阻塞, 等待子线程终止后再退出
        try {
            Thread.currentThread().join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void handleClient(AsynchronousSocketChannel clientChannel) {
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        // 设置有数据时的回调
        clientChannel.read(buffer, null, new CompletionHandler<Integer, Void>() {
            @Override
            public void completed(Integer bytesRead, Void attachment) {
                if (bytesRead > 0) {
                    // 处理数据读取成功的业务逻辑
                } else if (bytesRead == -1) {
                    // 客户端断开连接
                    clientChannel.close();
                }
            }
            @Override
            public void failed(Throwable exc, Void attachment) {
                // Client 发送的消息读取失败
            }
        });
    }
}
```
<a name="HCQpz"></a>
## 总结
C10K 问题是一个优化网络套接字以同时处理大量客户端连接的问题。为了解决该问题，首先的研究方向就是 I/O 模型的优化，逐渐解决了 C10K 的问题。epoll、kqueue、IOCP 就是 I/O 模型优化的一些最佳实践，这几种技术实现分别对应于不同的系统平台。不过，随着摩尔定律带来的服务器性能提升，以及互联网的普及，新兴服务对性能提出了更高的要求。很快，原来的 C10K 已经不能满足需求，所以又有了 C100K 和 C1000K，也就是并发从原来的 1 万增加到 10 万、乃至 100 万。<br />性能的要求是无止境的，再进一步，有没有可能在单机中，同时处理 1000 万的请求呢？这也就是 C10M 问题。2013年 Robert Graham 提出解决 C10M 的关键：The kernel isn't the solution,The kernel is the problem（内核不是解决方案，内核就是问题所在）。
