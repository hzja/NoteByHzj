Netty
<a name="unwGL"></a>
## Netty 的异步模型
说起 Netty 的异步模型，相信大多数人，只要是写过服务端的话，都是耳熟能详的，bossGroup 和 workerGroup 被 ServerBootstrap 所驱动，用起来简直是如虎添翼。<br />再加上各种配置化的 handler 加持，组装起来也是行云流水，俯拾即是。但是，任何一个好的架构，都不是一蹴而就实现的，那她经历了怎样的心路历程呢？
<a name="XWjPX"></a>
### ①经典的多线程模型
![2022-08-29-14-00-50.209065500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753768623-f19d7b54-1681-48d7-995b-3f80baa7cf5b.png#averageHue=%23f6f2ed&clientId=u690e85de-ff38-4&from=ui&id=u19ba216e&originHeight=237&originWidth=874&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7389&status=done&style=shadow&taskId=udfbae9d3-6489-4bf5-b093-231453b0493&title=)<br />此模型中，服务端起来后，客户端连接到服务端，服务端会为每个客户端开启一个线程来进行后续的读写操作。<br />客户端少的时候，整体性能和功能还是可以的，但是如果客户端非常多的时候，线程的创建将会导致内存的急剧飙升从而导致服务端的性能下降，严重者会导致新客户端连接不上来，更有甚者，服务器直接宕机。<br />此模型虽然简单，但是由于其简单粗暴，所以难堪大用，建议在写服务端的时候，要彻底的避免此种写法。
<a name="NCjLC"></a>
### ②经典的 Reactor 模型
由于多线程模型难堪大用，所以更好的模型一直在研究之中，Reactor 模型，作为天选之子，也被引入了进来，由于其强大的基于事件处理的特性，使得其成为异步模型的不二之选。<br />Reactor 模型由于是基于事件处理的，所以一旦有事件被触发，将会派发到对应的 event handler 中进行处理。<br />所以在此模型中，有两个最重要的参与者，列举如下：

- **Reactor：**主要用来将 IO 事件派发到相对应的 handler 中，可以将其想象为打电话时候的分发总机，先打电话到总机号码，然后通过总机，可以分拨到各个分机号码。
- **Handlers：**主要用来处理 IO 事件相关的具体业务，可以将其想象为拨通分机号码后，实际上处理事件的员工。

![2022-08-29-14-00-50.232713000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753742683-d9a82d66-bf7b-44f3-a813-60a853e0589d.png#averageHue=%23ebd9d7&clientId=u690e85de-ff38-4&from=ui&id=u7ac1f0b9&originHeight=474&originWidth=753&originalType=binary&ratio=1&rotation=0&showTitle=false&size=92790&status=done&style=shadow&taskId=u6c531faf-16c1-40e1-b112-ed45981e1ee&title=)<br />上图为 Reactor 模型的描述图，具体来说一下：<br />Initiation Dispatcher 其实扮演的就是 Reactor 的角色，主要进行 Event Demultiplexer，即事件派发。<br />而其内部一般都有一个 Acceptor，用于通过对系统资源的操纵来获取资源句柄，然后交由 Reactor，通过 handle_events 方法派发至具体的 EventHandler 的。<br />Synchronous Event Demultiplexer 其实就是 Acceptor 的角色，此角色内部通过调用系统的方法来进行资源操作。<br />比如说，假如客户端连接上来，那么将会获得当前连接，假如需要删除文件，那么将会获得当前待操作的文件句柄等等。<br />这些句柄实际上是要返回给 Reactor 的，然后经由 Reactor 派发下放给具体的 EventHandler。<br />Event Handler 这里，其实就是具体的事件操作了。其内部针对不同的业务逻辑，拥有不同的操作方法。<br />比如说，鉴权 EventHandler 会检测传入的连接，验证其是否在白名单，心跳包 EventHanler 会检测管道是否空闲。<br />业务 EventHandler 会进行具体的业务处理，编解码 EventHandler 会对当前连接传输的内容进行编码解码操作等等。<br />由于 Netty 是 Reactor 模型的具体实现，所以在编码的时候，可以非常清楚明白的理解 Reactor 的具体使用方式，这里暂时不讲，后面会提到。<br />由于 Doug Lea 写过一篇关于 NIO 的文章，整体总结的极好，所以这里就结合他的文章来详细分析一下 Reactor 模型的演化过程。<br />![2022-08-29-14-00-50.252496000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753686988-6573fce7-f5a4-475a-b58d-f21643319ecb.png#averageHue=%23f3e9bc&clientId=u690e85de-ff38-4&from=ui&id=pFQHh&originHeight=379&originWidth=959&originalType=binary&ratio=1&rotation=0&showTitle=false&size=81477&status=done&style=shadow&taskId=u59cffac1-d3f9-4422-adb4-a1c446700e5&title=)<br />上图模型为单线程 Reator 模型，Reactor 模型会利用给定的 selectionKeys 进行派发操作，派发到给定的 handler。<br />之后当有客户端连接上来的时候，acceptor 会进行 accept 接收操作，之后将接收到的连接和之前派发的 handler 进行组合并启动。<br />![2022-08-29-14-00-50.276026800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753687014-4310c4cb-21cc-4395-a03c-bb2160ccfe5b.png#averageHue=%23f7f1d9&clientId=u690e85de-ff38-4&from=ui&id=C8KS7&originHeight=695&originWidth=992&originalType=binary&ratio=1&rotation=0&showTitle=false&size=109941&status=done&style=shadow&taskId=uf792684a-a939-4ba5-b462-341bbb60236&title=)<br />上图模型为池化 Reactor 模型，此模型将读操作和写操作解耦了出来，当有数据过来的时候，将 handler 的系列操作扔到线程池中来进行，极大的提到了整体的吞吐量和处理速度。<br />![2022-08-29-14-00-50.307389100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753687012-58de396f-b549-4168-8784-6769eefe5e1b.png#averageHue=%23f6f2dc&clientId=u690e85de-ff38-4&from=ui&id=u875ea203&originHeight=689&originWidth=996&originalType=binary&ratio=1&rotation=0&showTitle=false&size=116314&status=done&style=shadow&taskId=u4ae3ed47-1c70-4e2d-8b0b-f57c8f9789c&title=)<br />上图模型为多 Reactor 模型，此模型中，将原本单个 Reactor 一分为二，分别为 mainReactor 和 subReactor。<br />其中 mainReactor 主要进行客户端连接方面的处理，客户端 accept 后发送给 subReactor 进行后续处理处理。<br />这种模型的好处就是整体职责更加明确，同时对于多 CPU 的机器，系统资源的利用更加高一些。<br />![2022-08-29-14-00-50.338286400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753666130-adfb89f8-b054-4258-85c4-96685db3f518.png#averageHue=%23212121&clientId=u690e85de-ff38-4&from=ui&id=u53f6a198&originHeight=670&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=false&size=282275&status=done&style=shadow&taskId=u5d090117-2849-445e-8c13-8c94529e55c&title=)<br />从 Netty 写的 server 端，就可以看出，boss worker group 对应的正是主副 Reactor。<br />之后 ServerBootstrap 进行 Reactor 的创建操作，里面的 group，channel，option 等进行初始化操作。<br />而设置的 childHandler 则是具体的业务操作，其底层的事件分发器则通过调用 Linux 系统级接口 epoll 来实现连接并将其传给 Reactor。
<a name="eD6Dq"></a>
## Netty 强悍的原理（JNI）
Netty 之剑之所以锋利，不仅仅因为其纯异步的编排模型，避免了各种阻塞式的操作，同时其内部各种设计精良的组件，终成一统。<br />且不说让人眼前一亮的缓冲池设计，读写标随心而动，摒弃了繁冗复杂的边界检测，用起来着实舒服之极。<br />原生的流控和高低水位设计，让流速控制真的是随心所欲，铸就了一道相当坚固的护城河。<br />齐全的粘包拆包处理方式，让每一笔数据都能够清晰明了；而高效的空闲检测机制，则让心跳包和断线重连等设计方案变得如此俯拾即是。<br />上层的设计如此优秀，其性能又怎能甘居下风。由于底层通讯方式完全是 C 语言编写，然后利用 JNI 机制进行处理，所以整体的性能可以说是达到了原生 C 语言性能的强悍程度。<br />说到 JNI，这里有必要详细说一下，他是利用 Java 直接调用 C 语言原生代码的关键。<br />JNI，全称为Java Native Interface，翻译过来就是 Java 本地接口，他是 Java 调用 C 语言的一套规范。具体来看看怎么做的吧。<br />步骤一，先来写一个简单的 Java 调用函数：
```java
/**
 * @Description: 数据同步器
 */
public class DataSynchronizer {
    /**
     * 加载本地底层C实现库
     */
    static {
        System.loadLibrary("synchronizer");
    }
    /**
     * 底层数据同步方法
     */
    private native String syncData(String status);
    /**
     * 程序启动，调用底层数据同步方法
     *
     * @param args
     */
    public static void main(String... args) {
        String rst = new DataSynchronizer().syncData("ProcessStep2");
        System.out.println("The execute result from C is : " + rst);
    }
}
```
可以看出，是一个非常简单的 Java 类，此类中，syncData 方法前面带了 native 修饰，代表此方法最终将会调用底层 C 语言实现。main 方法是启动类，将 C 语言执行的结果接收并打印出来。<br />然后，打开 Linux 环境，这里由于用的是 linux mint，依次执行如下命令来设置环境：<br />执行`apt install default-jdk` 安装java环境，安装完毕。<br />通过`update-alternatives --list java` 获取java安装路径，这里为：/usr/lib/jvm/java-11-openjdk-amd64 <br />设置java环境变量 `export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64`<br />环境设置完毕之后，就可以开始进行下一步了。<br />步骤二，编译，首先，进入到代码 DataSynchronizer.c 所在的目录，然后运行如下命令来编译 Java 源码：
```bash
javac -h . DataSynchronizer.java
```
编译完毕之后，可以看到当前目录出现了如下几个文件：<br />![2022-08-29-14-00-50.391040600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753500253-ebffdbcc-f0ec-401b-99af-417799f05d77.png#averageHue=%23fbf9f8&clientId=u690e85de-ff38-4&from=ui&id=ziGZ6&originHeight=119&originWidth=366&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6957&status=done&style=shadow&taskId=ud41a5812-bb75-427a-964b-f49b42e3c51&title=)<br />其中 DataSynchronizer.h 是生成的头文件，这个文件尽量不要修改，整体内容如下：
```c
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class DataSynchronizer */
#ifndef _Included_DataSynchronizer
#define _Included_DataSynchronizer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     DataSynchronizer
 * Method:    syncData
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_DataSynchronizer_syncData
  (JNIEnv *, jobject, jstring);
#ifdef __cplusplus
}
#endif
#endif
```
其中 JNIEXPORT jstring JNICALL Java_DataSynchronizer_syncData 方法，就是生成的本地 C 语言方法，这里只需要创建一个 C 语言文件，名称为 DataSynchronizer.c。<br />将此头文件加载进来，实现此方法即可：
```c
#include <jni.h>
#include <stdio.h>
#include "DataSynchronizer.h"

JNIEXPORT jstring JNICALL Java_DataSynchronizer_syncData(JNIEnv *env, jobject obj, jstring str) {
    // Step 1: Convert the JNI String (jstring) into C-String (char*)
    const char *inCStr = (*env)->GetStringUTFChars(env, str, NULL);
    if (NULL == inCStr) {
       return NULL;
   }

    // Step 2: Perform its intended operations
    printf("In C, the received string is: %s\n", inCStr);
    (*env)->ReleaseStringUTFChars(env, str, inCStr);  // release resources

    // Prompt user for a C-string
    char outCStr[128];
    printf("Enter a String: ");
    scanf("%s", outCStr);

    // Step 3: Convert the C-string (char*) into JNI String (jstring) and return
    return (*env)->NewStringUTF(env, outCStr);
}
```
其中需要注意的是，JNIEnv* 变量，实际上指的是当前的 JNI 环境。而 jobject 变量则类似 Java 中的 this 关键字。<br />jstring 则是 C 语言层面上的字符串，相当于 Java 中的 String。整体对应如下：<br />![2022-08-29-14-00-50.398870100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753500280-3d04b082-658f-41f8-b456-ed77efff0911.png#averageHue=%23f7f7f7&clientId=u690e85de-ff38-4&from=ui&id=uf18e5a8d&originHeight=717&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=false&size=164645&status=done&style=shadow&taskId=u24e1edae-8f8e-4e10-973b-cc13ff5e1ba&title=)<br />最后，来编译一下：
```bash
gcc -fPIC -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" -shared -o libsynchronizer.so DataSynchronizer.c
```
编译完毕后，可以看到当前目录下又多了一个 libsynchronizer.so 文件（这个文件类似 Windows 上编译后生成的 .dll 类库文件）：<br />![2022-08-29-14-00-50.438745900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753435168-571bc2dd-9390-4310-bace-573f55d6ef07.png#averageHue=%23faf9f8&clientId=u690e85de-ff38-4&from=ui&id=ymAc5&originHeight=120&originWidth=614&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10072&status=done&style=shadow&taskId=u82d7d4d0-cdd1-451d-80a1-6a4d729df01&title=)<br />此时可以运行了，运行如下命令进行运行：
```bash
java -Djava.library.path=. DataSynchronizer
```
得到结果如下：
```bash
java -Djava.library.path=. DataSynchronizer
In C, the received string is: ProcessStep2
Enter a String: sdfsdf
The execute result from C is : sdfsdf
```
从这里看到，正确的通过 java jni 技术，调用了 C 语言底层的逻辑，然后获取到结果，打印了出来。<br />在 Netty 中，也是利用了 jni 的技术，然后通过调用底层的 C 语言逻辑实现，来实现高效的网络通讯的。<br />感兴趣的同学可以扒拉下 Netty 源码，在 transport-native-epoll 模块中，就可以见到具体的实现方法了。<br />![2022-08-29-14-00-50.453918900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753320288-083e7871-3e1c-4d9a-9818-30192e5580d7.png#averageHue=%23ecede0&clientId=u690e85de-ff38-4&from=ui&id=ucbb7b9d9&originHeight=218&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60869&status=done&style=shadow&taskId=u489f557c-9477-4acb-b2f2-f7b064c7266&title=)
<a name="wRrJa"></a>
## IO 多路复用模型
石中剑，之所以能荡平英格兰全境，自然有其最强悍的地方。<br />相应的，Netty，则也是不遑多让，之所以能够被各大知名的组件所采用，自然也有其最强悍的地方，而本章节的 IO 多路复用模型，则是其强悍的理由之一。<br />在说 IO 多路复用模型之前，先来大致了解下 Linux 文件系统。<br />在 Linux 系统中，不论是鼠标，键盘，还是打印机，甚至于连接到本机的 socket client 端，都是以文件描述符的形式存在于系统中，诸如此类，等等等等。<br />所以可以这么说，一切皆文件。来看一下系统定义的文件描述符说明：<br />![2022-08-29-14-00-50.473641800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753307470-0026cfe7-da3c-405c-a48e-43e982badddf.png#averageHue=%23e9e8e6&clientId=u690e85de-ff38-4&from=ui&id=u149ee284&originHeight=149&originWidth=631&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16481&status=done&style=shadow&taskId=ub92c1906-a98a-490d-b0e8-b5d2aff6874&title=)<br />从上面的列表可以看到，文件描述符 0，1，2 都已经被系统占用了，当系统启动的时候，这三个描述符就存在了。<br />其中 0 代表标准输入，1 代表标准输出，2 代表错误输出。当创建新的文件描述符的时候，就会在 2 的基础上进行递增。<br />可以这么说，文件描述符是为了管理被打开的文件而创建的系统索引，他代表了文件的身份 ID。对标 Windows 的话，可以认为和句柄类似，这样就更容易理解一些。<br />由于网上对 Linux 文件这块的原理描述的文章已经非常多了，所以这里不再做过多的赘述，感兴趣的同学可以从 Wikipedia 翻阅一下。<br />由于这块内容比较复杂，不属于本文普及的内容，建议另行自研。
<a name="ALP6K"></a>
## select 模型
此模型是 IO 多路复用的最早期使用的模型之一，距今已经几十年了，但是现在依旧有不少应用还在采用此种方式，可见其长生不老。<br />首先来看下其具体的定义（来源于 man 二类文档）：
```c
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);
```
这里解释下其具体参数：

- **参数一：nfds，**也即 maxfd，最大的文件描述符递增一。这里之所以传最大描述符，为的就是在遍历 fd_set 的时候，限定遍历范围。
- **参数二：readfds，**可读文件描述符集合。
- **参数三：writefds，**可写文件描述符集合。
- **参数四：errorfds，**异常文件描述符集合。
- **参数五：timeout，**超时时间。在这段时间内没有检测到描述符被触发，则返回。

下面的宏处理，可以对 fd_set 集合（准确的说是 bitmap，一个描述符有变更，则会在描述符对应的索引处置 1）进行操作：

- `**FD_CLR(inr fd,fd_set* set)**`**：**用来清除描述词组 set 中相关 fd 的位，即 bitmap 结构中索引值为 fd 的值置为 0。
- `**FD_ISSET(int fd,fd_set *set)**`**：**用来测试描述词组 set 中相关 fd 的位是否为真，即 bitmap 结构中某一位是否为 1。
- `**FD_SET(int fd,fd_set*set)**`**：**用来设置描述词组 set 中相关 fd 的位，即将 bitmap 结构中某一位设置为 1，索引值为 fd。
- `**FD_ZERO(fd_set *set)**`**：**用来清除描述词组 set 的全部位，即将 bitmap 结构全部清零。

首先来看一段服务端采用了 select 模型的示例代码：
```c
//创建server端套接字，获取文件描述符
int listenfd = socket(PF_INET,SOCK_STREAM,0);
if(listenfd < 0) return -1;

//绑定服务器
bind(listenfd,(struct sockaddr*)&address,sizeof(address));
//监听服务器
listen(listenfd,5); 

struct sockaddr_in client;
socklen_t addr_len = sizeof(client);

//接收客户端连接
int connfd = accept(listenfd,(struct sockaddr*)&client,&addr_len);

//读缓冲区
char buff[1024]; 

//读文件操作符
fd_set read_fds;  

while(1)
{
    memset(buff,0,sizeof(buff));

    //注意：每次调用select之前都要重新设置文件描述符connfd，因为文件描述符表会在内核中被修改
    FD_ZERO(&read_fds);
    FD_SET(connfd,&read_fds);

    //注意：select会将用户态中的文件描述符表放到内核中进行修改，内核修改完毕后再返回给用户态，开销较大
    ret = select(connfd+1,&read_fds,NULL,NULL,NULL);
    if(ret < 0)
    {
        printf("Fail to select!\n");
        return -1;
    }

    //检测文件描述符表中相关请求是否可读
    if(FD_ISSET(connfd, &read_fds))
    {
        ret = recv(connfd,buff,sizeof(buff)-1,0);
        printf("receive %d bytes from client: %s \n",ret,buff);
    }
}
```
上面的代码加了比较详细的注释了，大家应该很容易看明白，说白了大概流程其实如下：

- 首先，创建 socket 套接字，创建完毕后，会获取到此套接字的文件描述符。
- 然后，bind 到指定的地址进行监听 listen。这样，服务端就在特定的端口启动起来并进行监听了。
- 之后，利用开启 accept 方法来监听客户端的连接请求。一旦有客户端连接，则将获取到当前客户端连接的 connection 文件描述符。

双方建立连接之后，就可以进行数据互传了。需要注意的是，在循环开始的时候，务必每次都要重新设置当前 connection 的文件描述符，是因为文件描描述符表在内核中被修改过，如果不重置，将会导致异常的情况。<br />重新设置文件描述符后，就可以利用 select 函数从文件描述符表中，来轮询哪些文件描述符就绪了。<br />此时系统会将用户态的文件描述符表发送到内核态进行调整，即将准备就绪的文件描述符进行置位，然后再发送给用户态的应用中来。<br />用户通过 FD_ISSET 方法来轮询文件描述符，如果数据可读，则读取数据即可。<br />举个例子，假设此时连接上来了 3 个客户端，connection 的文件描述符分别为 4，8，12。<br />那么其 read_fds 文件描述符表（bitmap 结构）的大致结构为  00010001000100000....0。<br />由于 read_fds 文件描述符的长度为 1024 位，所以最多允许 1024 个连接。<br />![2022-08-29-14-00-50.487178000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753233444-b84f15aa-d252-4a38-b2f5-f3f2356334ff.png#averageHue=%23eeeeee&clientId=u690e85de-ff38-4&from=ui&id=ue3a9c7ef&originHeight=106&originWidth=676&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2067&status=done&style=shadow&taskId=u61f57e89-e397-4cdc-9c55-e0d1eebcab7&title=)<br />而在 select 的时候，涉及到用户态和内核态的转换，所以整体转换方式如下：<br />![2022-08-29-14-00-50.495424500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753233472-a6e20733-9331-4e50-9bc1-ae15dea5e793.png#averageHue=%23f7f7f7&clientId=u690e85de-ff38-4&from=ui&id=BGBiy&originHeight=532&originWidth=990&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29797&status=done&style=shadow&taskId=u641449e4-dc21-43f5-aa4c-c3cbde68fe2&title=)<br />所以，综合起来，select 整体还是比较高效和稳定的，但是呈现出来的问题也不少。<br />这些问题进一步限制了其性能发挥：

- 文件描述符表为 bitmap 结构，且有长度为 1024 的限制。
- fdset 无法做到重用，每次循环必须重新创建。
- 频繁的用户态和内核态拷贝，性能开销较大。
- 需要对文件描述符表进行遍历，O(n) 的轮询时间复杂度。
<a name="GC2s7"></a>
## poll 模型
考虑到 select 模型的几个限制，后来进行了改进，这也就是 poll 模型，既然是 select 模型的改进版，那么肯定有其亮眼的地方，一起来看看吧。<br />当然，这次依旧是先翻阅 linux man 二类文档，因为这是官方的文档，对其有着最为精准的定义。
```c
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```
其实，从运行机制上说来，poll 所做的功能和 select 是基本上一样的，都是等待并检测一组文件描述符就绪，然后在进行后续的 IO 处理工作。<br />只不过不同的是，select 中，采用的是 bitmap 结构，长度限定在 1024 位的文件描述符表，而 poll 模型则采用的是 pollfd 结构的数组 fds。<br />也正是由于 poll 模型采用了数组结构，则不会有 1024 长度限制，使其能够承受更高的并发。<br />pollfd 结构内容如下：
```c
struct pollfd {
    int   fd;         /* 文件描述符 */
    short events;     /* 关心的事件 */
    short revents;    /* 实际返回的事件 */
};
```
从上面的结构可以看出，fd 很明显就是指文件描述符，也就是当客户端连接上来后，fd 会将生成的文件描述符保存到这里。<br />而 events 则是指用户想关注的事件；revents 则是指实际返回的事件，是由系统内核填充并返回，如果当前的 fd 文件描述符有状态变化，则 revents 的值就会有相应的变化。<br />events 事件列表如下：<br />![2022-08-29-14-00-50.508839900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753069402-5663fef0-bb4a-427f-b7ed-46bf09d6f844.png#averageHue=%23f5f4f3&clientId=u690e85de-ff38-4&from=ui&id=ua0366604&originHeight=351&originWidth=405&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27231&status=done&style=shadow&taskId=u545e4205-4592-4a8f-ab28-ee9851d0fa6&title=)<br />revents 事件列表如下：<br />![2022-08-29-14-00-50.518451000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753069416-ed3c17ab-8d33-4140-8214-6447ef8d8de2.png#averageHue=%23f6f5f4&clientId=u690e85de-ff38-4&from=ui&id=n2b5s&originHeight=474&originWidth=406&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36060&status=done&style=shadow&taskId=u0787932b-8fd0-4bbd-9174-9f858f747f1&title=)<br />从列表中可以看出，revents 是包含 events 的。接下来结合示例来看一下：
```c
//创建server端套接字，获取文件描述符
int listenfd = socket(PF_INET,SOCK_STREAM,0);
if(listenfd < 0) return -1;

//绑定服务器
bind(listenfd,(struct sockaddr*)&address,sizeof(address));
//监听服务器
listen(listenfd,5); 

struct pollfd pollfds[1];
socklen_t addr_len = sizeof(client);

//接收客户端连接
int connfd = accept(listenfd,(struct sockaddr*)&client,&addr_len);

//放入fd数组
pollfds[0].fd = connfd;
pollfds[0].events = POLLIN;

//读缓冲区
char buff[1024]; 

//读文件操作符
fd_set read_fds;  

while(1)
{
    memset(buff,0,sizeof(buff));
    
    /**
    ** SELECT模型专用
    ** 注意：每次调用select之前都要重新设置文件描述符connfd，因为文件描述符表会在内核中被修改
    ** FD_ZERO(&read_fds);
    ** FD_SET(connfd,&read_fds);
    ** 注意：select会将用户态中的文件描述符表放到内核中进行修改，内核修改完毕后再返回给用户态，开销较大
    ** ret = select(connfd+1,&read_fds,NULL,NULL,NULL);
    **/
    
    ret = poll(pollfds, 1, 1000);
    if(ret < 0)
    {
        printf("Fail to poll!\n");
        return -1;
    }
    
    /**
    ** SELECT模型专用
    ** 检测文件描述符表中相关请求是否可读
    ** if(FD_ISSET(connfd, &read_fds))
    ** {
    **   ret = recv(connfd,buff,sizeof(buff)-1,0);
    **   printf("receive %d bytes from client: %s \n",ret,buff);
    ** }
    **/
    //检测文件描述符数组中相关请求
    if(pollfds[0].revents & POLLIN){
        pollfds[0].revents = 0;
        ret = recv(connfd,buff,sizeof(buff)-1,0);
        printf("receive %d bytes from client: %s \n",ret,buff);
    }
}
```
由于源码中，做了比较详细的注释，同时将和 select 模型不一样的地方都列了出来，这里就不再详细解释了。<br />总体说来，poll 模型比 select 模型要好用一些，去掉了一些限制，但是仍然避免不了如下的问题：

- 用户态和内核态仍需要频繁切换，因为 revents 的赋值是在内核态进行的，然后再推送到用户态，和 select 类似，整体开销较大。
- 仍需要遍历数组，时间复杂度为 O（N）。
<a name="s9aE1"></a>
## epoll 模型
如果说 select 模型和 poll 模型是早期的产物，在性能上有诸多不尽人意之处，那么自 Linux 2.6 之后新增的 epoll 模型，则彻底解决了性能问题，一举使得单机承受百万并发的课题变得极为容易。<br />现在可以这么说，只需要一些简单的设置更改，然后配合上 epoll 的性能，实现单机百万并发轻而易举。<br />同时，由于 epoll 整体的优化，使得之前的几个比较耗费性能的问题不再成为羁绊，所以也成为了 Linux 平台上进行网络通讯的首选模型。<br />讲解之前，还是 linux man 文档镇楼：linux man epoll 4 类文档 linux man epoll 7 类文档，俩文档结合着读，会对 epoll 有个大概的了解。<br />和之前提到的 select 和 poll 不同的是，此二者皆属于系统调用函数，但是 epoll 则不然，他是存在于内核中的数据结构。<br />可以通过 epoll_create，epoll_ctl 及 epoll_wait 三个函数结合来对此数据结构进行操控。<br />说到 epoll_create 函数，其作用是在内核中创建一个 epoll 数据结构实例，然后将返回此实例在系统中的文件描述符。<br />此 epoll 数据结构的组成其实是一个链表结构，称之为 interest list，里面会注册连接上来的 client 的文件描述符。<br />其简化工作机制如下：<br />![2022-08-29-14-00-50.539666600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753037079-01c2ffd0-ffb8-4a67-8aae-24f53a6a4a38.png#averageHue=%23f3f3f3&clientId=u690e85de-ff38-4&from=ui&id=CXVak&originHeight=502&originWidth=646&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31851&status=done&style=shadow&taskId=u24cde318-a292-4d10-b921-d2273e49df2&title=)<br />说到 epoll_ctl 函数，其作用则是对 epoll 实例进行增删改查操作。有些类似常用的 CRUD 操作。<br />这个函数操作的对象其实就是 epoll 数据结构，当有新的 client 连接上来的时候，他会将此 client 注册到 epoll 中的 interest list 中，此操作通过附加 EPOLL_CTL_ADD 标记来实现。<br />当已有的 client 掉线或者主动下线的时候，他会将下线的 client从epoll 的 interest list 中移除，此操作通过附加 EPOLL_CTL_DEL 标记来实现。<br />当有 client 的文件描述符有变更的时候，他会将 events 中的对应的文件描述符进行更新，此操作通过附加 EPOLL_CTL_MOD 来实现。<br />当 interest list 中有 client 已经准备好了，可以进行 IO 操作的时候，他会将这些 clients 拿出来，然后放到一个新的 ready list 里面。<br />其简化工作机制如下：<br />![2022-08-29-14-00-50.551245000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753037067-9929f35a-26ee-40c8-9416-f5b337b7caa0.png#averageHue=%23f4f3f3&clientId=u690e85de-ff38-4&from=ui&id=u20572514&originHeight=549&originWidth=644&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36070&status=done&style=shadow&taskId=u0645a4a1-bf5d-4d88-ac55-81fb80307e2&title=)<br />说到 epoll_wait 函数，其作用就是扫描 ready list，处理准备就绪的 client IO，其返回结果即为准备好进行 IO 的 client 的个数。通过遍历这些准备好的 client，就可以轻松进行 IO 处理了。<br />上面这三个函数是 epoll 操作的基本函数，但是，想要彻底理解 epoll，则需要先了解这三块内容，即：inode，链表，红黑树。<br />在 Linux 内核中，针对当前打开的文件，有一个 open file table，里面记录的是所有打开的文件描述符信息；同时也有一个 inode table，里面则记录的是底层的文件描述符信息。<br />这里假如文件描述符 B fork 了文件描述符 A，虽然在 open file table 中，看新增了一个文件描述符 B，但是实际上，在 inode table 中，A 和 B 的底层是一模一样的。<br />这里，将 inode table 中的内容理解为 Windows 中的文件属性，会更加贴切和易懂。<br />这样存储的好处就是，无论上层文件描述符怎么变化，由于 epoll 监控的数据永远是 inode table 的底层数据，那么就可以一直能够监控到文件的各种变化信息，这也是 epoll 高效的基础。<br />简化流程如下：<br />![2022-08-29-14-00-50.566795700.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661753019126-865ac245-86aa-45db-84e2-69ed80d1e339.jpeg#averageHue=%231e2325&clientId=u690e85de-ff38-4&from=ui&id=u462f5701&originHeight=307&originWidth=391&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24034&status=done&style=shadow&taskId=u14260f65-83ae-47ae-844c-c9ae7db30a8&title=)<br />数据存储这块解决了，那么针对连接上来的客户端 socket，该用什么数据结构保存进来呢？<br />这里用到了红黑树，由于客户端 socket 会有频繁的新增和删除操作，而红黑树这块时间复杂度仅仅为 O(logN)，还是挺高效的。<br />有人会问为啥不用哈希表呢？当大量的连接频繁的进行接入或者断开的时候，扩容或者其他行为将会产生不少的 rehash 操作，而且还要考虑哈希冲突的情况。<br />虽然查询速度的确可以达到 o(1)，但是 rehash 或者哈希冲突是不可控的，所以基于这些考量，认为红黑树占优一些。<br />客户端 socket 怎么管理这块解决了，接下来，当有 socket 有数据需要进行读写事件处理的时候，系统会将已经就绪的 socket 添加到双向链表中，然后通过 epoll_wait 方法检测的时候。<br />其实检查的就是这个双向链表，由于链表中都是就绪的数据，所以避免了针对整个客户端 socket 列表进行遍历的情况，使得整体效率大大提升。<br />整体的操作流程为：

- 首先，利用 epoll_create 在内核中创建一个 epoll 对象。其实这个 epoll 对象，就是一个可以存储客户端连接的数据结构。
- 然后，客户端 socket 连接上来，会通过 epoll_ctl 操作将结果添加到 epoll 对象的红黑树数据结构中。
- 然后，一旦有 socket 有事件发生，则会通过回调函数将其添加到 ready list 双向链表中。
- 最后，epoll_wait 会遍历链表来处理已经准备好的 socket，然后通过预先设置的水平触发或者边缘触发来进行数据的感知操作。

从上面的细节可以看出，由于 epoll 内部监控的是底层的文件描述符信息，可以将变更的描述符直接加入到 ready list，无需用户将所有的描述符再进行传入。<br />同时由于 epoll_wait 扫描的是已经就绪的文件描述符，避免了很多无效的遍历查询，使得 epoll 的整体性能大大提升，可以说现在只要谈论 Linux 平台的 IO 多路复用，epoll 已经成为了不二之选。
<a name="XRILu"></a>
## 水平触发和边缘触发
上面说到了 epoll，主要讲解了 client 端怎么连进来，但是并未详细的讲解 epoll_wait 怎么被唤醒的，这里将来详细的讲解一下。<br />水平触发，意即 Level Trigger，边缘触发，意即 Edge Trigger，如果单从字面意思上理解，则不太容易，但是如果将硬件设计中的水平沿，上升沿，下降沿的概念引进来，则理解起来就容易多了。<br />比如可以这样认为：<br />![2022-08-29-14-00-50.576415900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661753000732-297d4318-29c2-4fe0-8aac-0dabbb5a0a27.png#averageHue=%23fafafa&clientId=u690e85de-ff38-4&from=ui&id=uada234ca&originHeight=248&originWidth=643&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13576&status=done&style=shadow&taskId=u90d4f4fd-2d9c-4a58-bb50-72089c81e7f&title=)<br />如果将上图中的方块看做是 buffer 的话，那么理解起来则就更加容易了，比如针对水平触发，buffer 只要是一直有数据，则一直通知；而边缘触发，则 buffer 容量发生变化的时候，才会通知。<br />虽然可以这样简单的理解，但是实际上，其细节处理部分，比图示中展现的更加精细，这里来详细的说一下。
<a name="zWniG"></a>
### ①边缘触发
针对读操作，也就是当前 fd 处于 EPOLLIN 模式下，即可读。此时意味着有新的数据到来，接收缓冲区可读，以下 buffer 都指接收缓冲区：<br />![2022-08-29-14-00-50.597088900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752984612-c633dca6-f628-45c6-8b72-f61de40e7e9c.png#averageHue=%23f8f7f5&clientId=u690e85de-ff38-4&from=ui&id=u05442ee5&originHeight=161&originWidth=332&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2508&status=done&style=shadow&taskId=u66e5fbdd-9e08-409b-a719-06aa98566f0&title=)<br />buffer 由空变为非空，意即有数据进来的时候，此过程会触发通知：<br />![2022-08-29-14-00-50.603142900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752943963-04e5bba0-1ef0-4a4a-9b25-f1be6ae4a448.png#averageHue=%23f8f3ed&clientId=u690e85de-ff38-4&from=ui&id=u17ec4594&originHeight=157&originWidth=326&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2915&status=done&style=shadow&taskId=u1e58204e-163f-4008-843f-b55d0e36fe0&title=)<br />buffer 原本有些数据，这时候又有新数据进来的时候，数据变多，此过程会触发通知：<br />![2022-08-29-14-00-50.612007200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752943963-55fe7b15-4920-4d32-b0a6-96b900f4e91b.png#averageHue=%23f9f6f3&clientId=u690e85de-ff38-4&from=ui&id=vhGvv&originHeight=198&originWidth=280&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3390&status=done&style=shadow&taskId=uce951b88-783a-4595-b5df-8ad1929beef&title=)<br />buffer 中有数据，此时用户对操作的 fd 注册 EPOLL_CTL_MOD 事件的时候，会触发通知<br />针对写操作，也就是当前 fd 处于 EPOLLOUT 模式下，即可写。此时意味着缓冲区可以写了，以下 buffer 都指发送缓冲区：<br />buffer 满了，这时候发送出去一些数据，数据变少，此过程会触发通知：<br />![2022-08-29-14-00-50.620178200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752916594-7dc7f876-ba84-49b6-a148-5a80014c9b7a.png#averageHue=%23f9f3ea&clientId=u690e85de-ff38-4&from=ui&id=ub7ae64d5&originHeight=158&originWidth=329&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2774&status=done&style=shadow&taskId=uc6bafdb1-7594-4aa9-bd6e-10b040166af&title=)<br />buffer 原本有些数据，这时候又发送出去一些数据，数据变少，此过程会触发通知：<br />![2022-08-29-14-00-50.627500700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752916601-2dc75054-ac6c-49ef-8b54-f9b197622fca.png#averageHue=%23f8f3ee&clientId=u690e85de-ff38-4&from=ui&id=LuKcP&originHeight=155&originWidth=324&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2841&status=done&style=shadow&taskId=ufa6c5525-34d0-4c50-83ef-58b2645f1d3&title=)<br />这里就是 ET 这种模式触发的几种情形，可以看出，基本上都是围绕着接收缓冲区或者发送缓冲区的状态变化来进行的。<br />晦涩难懂？不存在的，举个栗子：<br />在服务端，开启边缘触发模式，然后将 buffer size 设为 10 个字节，来看看具体的表现形式。<br />服务端开启，客户端连接，发送单字符 A 到服务端，输出结果如下：
```
-->ET Mode: it was triggered once

get 1 bytes of content: A

-->wait to read!
```
可以看到，由于 buffer 从空到非空，边缘触发通知产生，之后在 epoll_wait 处阻塞，继续等待后续事件。<br />这里变一下，输入 ABCDEFGHIJKLMNOPQ，可以看到，客户端发送的字符长度超过了服务端 buffer size，那么输出结果将是怎么样的呢？
```
-->ET Mode: it was triggered once

get 9 bytes of content: ABCDEFGHI

get 8 bytes of content: JKLMNOPQ

-->wait to read!
```
可以看到，这次发送，由于发送的长度大于 buffer size，所以内容被折成两段进行接收，由于用了边缘触发方式，buffer 的情况是从空到非空，所以只会产生一次通知。
<a name="lG2r3"></a>
### ②水平触发
水平触发则简单多了，他包含了边缘触发的所有场景，简而言之如下：<br />当接收缓冲区不为空的时候，有数据可读，则读事件会一直触发：<br />![2022-08-29-14-00-50.635394800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752893730-95c7ba7f-06af-4e63-8dae-93416896921f.png#averageHue=%23f8f6f3&clientId=u690e85de-ff38-4&from=ui&id=uc1a108f6&originHeight=122&originWidth=280&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1790&status=done&style=shadow&taskId=ub7d83ae0-6ae7-451c-8f08-693b78c76b4&title=)<br />当发送缓冲区未满的时候，可以继续写入数据，则写事件一直会触发：<br />![2022-08-29-14-00-50.643030300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661752893752-ea183c29-bc0a-4105-8753-c467f6c8cc4f.png#averageHue=%23f8f6f3&clientId=u690e85de-ff38-4&from=ui&id=mSL8i&originHeight=117&originWidth=287&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1743&status=done&style=shadow&taskId=uc7dcc252-9dcd-4df5-af69-e95efbd7cf6&title=)<br />同样的，为了使表达更清晰，也来举个栗子，按照上述入输入方式来进行。<br />服务端开启，客户端连接并发送单字符 A，可以看到服务端输出情况如下：
```
-->LT Mode: it was triggered once!
get 1 bytes of content: A
```
这个输出结果，毋庸置疑，由于 buffer 中有数据，所以水平模式触发，输出了结果。<br />服务端开启，客户端连接并发送 ABCDEFGHIJKLMNOPQ，可以看到服务端输出情况如下：
```
-->LT Mode: it was triggered once!
get 9 bytes of content: ABCDEFGHI
-->LT Mode: it was triggered once!
get 8 bytes of content: JKLMNOPQ
```
从结果中，可以看出，由于 buffer 中数据读取完毕后，还有未读完的数据，所以水平模式会一直触发，这也是为啥这里水平模式被触发了两次的原因。<br />有了这两个栗子的比对，不知道聪明的你，get 到二者的区别了吗？<br />在实际开发过程中，实际上 LT 更易用一些，毕竟系统做了大部分校验通知工作，之前提到的 SELECT 和 POLL，默认采用的也都是这个。<br />但是需要注意的是，当有成千上万个客户端连接上来开始进行数据发送，由于 LT 的特性，内核会频繁的处理通知操作，导致其相对于 ET 来说，比较的耗费系统资源，所以，随着客户端的增多，其性能也就越差。<br />而边缘触发，由于监控的是 FD 的状态变化，所以整体的系统通知并没有那么频繁，高并发下整体的性能表现也要好很多。<br />但是由于此模式下，用户需要积极的处理好每一笔数据，带来的维护代价也是相当大的，稍微不注意就有可能出错。所以使用起来须要非常小心才行。<br />至于二者如何抉择，诸位就仁者见仁智者见智吧。<br />行文到这里，关于 epoll 的讲解基本上完毕了，大家从中是不是学到了很多干货呢？<br />由于从 Netty 研究到 linux epoll 底层，其难度非常大，可以用曲高和寡来形容，所以在这块探索的文章是比较少的，很多东西需要自己照着 man 文档和源码一点一点的琢磨（linux 源码详见 eventpoll.c 等）。<br />这里来纠正一下搜索引擎上，说 epoll 高性能是因为利用 mmap 技术实现了用户态和内核态的内存共享，所以性能好。<br />前期被这个观点误导了好久，后来下来了 Linux 源码，翻了一下，并没有在 epoll 中翻到 mmap 的技术点，所以这个观点是错误的。<br />这些错误观点的文章，国内不少，国外也不少，希望大家能审慎抉择，避免被错误带偏。<br />所以，epoll 高性能的根本就是，其高效的文件描述符处理方式加上颇具特性边的缘触发处理模式，以极少的内核态和用户态的切换，实现了真正意义上的高并发。
<a name="FZ5OH"></a>
## 手写 epoll 服务端
实践是最好的老师，现在就不妨尝试着写一下看看。<br />手写 epoll 服务器，具体细节如下（非 C 语言 coder，代码有参考）：
```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#define MAX_EVENT_NUMBER 1024   //事件总数量
#define BUFFER_SIZE 10          //缓冲区大小，这里为10个字节
#define ENABLE_ET 0             //ET模式
/* 文件描述符设为非阻塞状态
* 注意：这个设置很重要，否则体现不出高性能
*/
int SetNonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}
/* 将文件描述符fd放入到内核中的epoll数据结构中并将fd设置为EPOLLIN可读，同时根据ET开关来决定使用水平触发还是边缘触发模式 
* 注意：默认为水平触发，或上EPOLLET则为边缘触发
*/
void AddFd(int epoll_fd, int fd, bool enable_et)
{
    struct epoll_event event;  //为当前fd设置事件
    event.data.fd = fd;        //指向当前fd
    event.events = EPOLLIN;    //使得fd可读
    if(enable_et)
    {
        event.events |= EPOLLET; //设置为边缘触发
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);  //将fd添加到内核中的epoll实例中
    SetNonblocking(fd);  //设为非阻塞模式                      
}
/*  LT水平触发 
*  注意：水平触发简单易用，性能不高，适合低并发场合
*        一旦缓冲区有数据，则会重复不停的进行通知，直至缓冲区数据读写完毕
*/
void lt_process(struct epoll_event* events, int number, int epoll_fd, int listen_fd)
{
    char buf[BUFFER_SIZE];
    int i;
    for(i = 0; i < number; i++) //已经就绪的事件，这些时间可读或者可写
    {
        int sockfd = events[i].data.fd; //获取描述符
        if(sockfd == listen_fd)  //如果监听类型的描述符，则代表有新的client接入，则将其添加到内核中的epoll结构中
        {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listen_fd, (struct sockaddr*)&client_address, &client_addrlength); //创建连接并返回文件描述符（实际进行的三次握手过程）
            AddFd(epoll_fd, connfd, false);  //添加到epoll结构中并初始化为LT模式
        }
        else if(events[i].events & EPOLLIN) //如果客户端有数据过来
        {
            printf("-->LT Mode: it was triggered once!\n");
            memset(buf, 0, BUFFER_SIZE); 
            int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
            if(ret <= 0)  //读取数据完毕后，关闭当前描述符
            {
                close(sockfd);
                continue;
            }
            printf("get %d bytes of content: %s\n", ret, buf);
        }
        else
        {
            printf("something unexpected happened!\n");
        }
    }
}
  /*  ET Work mode features: efficient but potentially dangerous */
  /*  ET边缘触发
*  注意：边缘触发由于内核不会频繁通知，所以高效，适合高并发场合，但是处理不当将会导致严重事故
其通知机制和触发方式参见之前讲解，由于不会重复触发，所以需要处理好缓冲区中的数据，避免脏读脏写或者数据丢失等
*/
  void et_process(struct epoll_event* events, int number, int epoll_fd, int listen_fd)
{
    char buf[BUFFER_SIZE];
    int i;
    for(i = 0; i < number; i++)
    {
        int sockfd = events[i].data.fd;
        if(sockfd == listen_fd) //如果有新客户端请求过来，将其添加到内核中的epoll结构中并默认置为ET模式
        {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listen_fd, (struct sockaddr*)&client_address, &client_addrlength);
            AddFd(epoll_fd, connfd, true); 
        }
        else if(events[i].events & EPOLLIN) //如果客户端有数据过来
        {
            printf("-->ET Mode: it was triggered once\n");
            while(1) //循环等待
            {
                memset(buf, 0, BUFFER_SIZE);
                int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
                if(ret < 0)
                {
                    if(errno == EAGAIN || errno == EWOULDBLOCK) //通过EAGAIN检测，确认数据读取完毕
                    {
                        printf("-->wait to read!\n");
                        break;
                    }
                    close(sockfd);
                    break;
                }
                else if(ret == 0) //数据读取完毕，关闭描述符
                {
                    close(sockfd);
                }
                else //数据未读取完毕，继续读取
                {
                    printf("get %d bytes of content: %s\n", ret, buf);
                }
            }
        }
        else
        {
            printf("something unexpected happened!\n");
        }
    }
}
  int main(int argc, char* argv[])
{
    const char* ip = "10.0.76.135";
    int port = 9999;

    //套接字设置这块，参见https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
    int ret = -1;
    struct sockaddr_in address; 
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);    //创建套接字并返回描述符
    if(listen_fd < 0)
    {
        printf("fail to create socket!\n");
        return -1;
    }
    ret = bind(listen_fd, (struct sockaddr*)&address, sizeof(address)); //绑定本机
    if(ret == -1)
    {
        printf("fail to bind socket!\n");
        return -1;
    }
    ret = listen(listen_fd, 5); //在端口上监听
    if(ret == -1)
    {
        printf("fail to listen socket!\n");
        return -1;
    }
    struct epoll_event events[MAX_EVENT_NUMBER];
    int epoll_fd = epoll_create(5);  //在内核中创建epoll实例，flag为5只是为了分配空间用，实际可以不用带
    if(epoll_fd == -1)
    {
        printf("fail to create epoll!\n");
        return -1;
    }
    AddFd(epoll_fd, listen_fd, true); //添加文件描述符到epoll对象中
    while(1)
    {
        int ret = epoll_wait(epoll_fd, events, MAX_EVENT_NUMBER, -1); //拿出就绪的文件描述符并进行处理
        if(ret < 0)
        {
            printf("epoll failure!\n");
            break;
        }
        if(ENABLE_ET) //ET处理方式
        {
            et_process(events, ret, epoll_fd, listen_fd);
        }
        else  //LT处理方式
        {
            lt_process(events, ret, epoll_fd, listen_fd);
        }
    }
    close(listen_fd); //退出监听
    return 0;
}
```
详细的注释都已经写上去了，这就是整个 epoll server 端全部源码了，仅仅只有 200 行左右，是不是很惊讶。
