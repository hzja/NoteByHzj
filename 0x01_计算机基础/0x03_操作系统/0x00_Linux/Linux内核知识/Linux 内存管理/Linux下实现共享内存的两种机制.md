Linux<br />来看一下Linux进程通信中重要的通信方式：**共享内存**作为Linux软件开发攻城狮，进程间通信是必须熟练掌握的重要技能，而**共享内存**是在程序开发中常用的也是重要的一种进程间通信方式。<br />下面就来聊一聊Linux下进程间如何实现**共享内存**通信，有哪些方式？
<a name="Vw8Fx"></a>
## 1、基本概念
<a name="PNRA5"></a>
### UNIX 和 Linux
UNIX和Linux是两种不同的操作系统，它们的主要区别在以下几个方面：

- 历史：UNIX是最早的商业化操作系统之一，最初由贝尔实验室开发，而Linux则是由Linus Torvalds于1991年开发的开源操作系统。
- 源代码：UNIX的源代码是私有的，需要购买授权才能使用和修改，而Linux是开源的，任何人都可以自由地访问、使用和修改其源代码。
- 可移植性：由于UNIX的代码是私有的，因此它们在不同的硬件平台之间的可移植性较差。而Linux的源代码是开放的，因此它可以在多种硬件平台上运行。
- 发行版：UNIX有多个商业和非商业版本，如Solaris、AIX、HP-UX等，每个版本都有自己的特点和功能。而Linux则有许多不同的发行版，如Ubuntu、Debian、Red Hat、Fedora等。
- 命令行工具：UNIX和Linux有许多相同的命令行工具和命令，如ls、grep、awk等，但也有一些不同之处。

总的来说，UNIX和Linux都是基于UNIX哲学的操作系统，但它们在源代码、可移植性、发行版和命令行工具等方面有所不同。
<a name="Y5R2B"></a>
### System V 和 POSIX
System V和POSIX是两种不同的操作系统标准，它们的区别在以下几个方面：

- 历史背景：System V最初是由AT&T开发的UNIX版本，而POSIX是IEEE为了保证不同UNIX系统的兼容性而开发的标准。
- 体系结构：System V是一种具体的操作系统，而POSIX则是一种操作系统接口标准。因此，System V具有更多的操作系统特定功能，而POSIX的接口更为通用，适用于多种不同类型的UNIX系统。
- 文件系统：System V和POSIX的文件系统不同。System V使用名为“inode”的数据结构来描述文件和目录，而POSIX则使用名为“文件描述符”的整数来表示打开的文件。
- Shell：System V和POSIX的Shell也不同。System V的Shell是Bourne Shell，而POSIX的Shell是Bourne-Again Shell（bash）。
- 网络支持：System V和POSIX的网络支持也不同。System V使用TCP/IP协议栈，而POSIX使用套接字（socket）接口。
<a name="ukiL3"></a>
## 2、System V IPC 和 POSIX IPC
System V IPC（Interprocess Communication）和POSIX IPC都是用于在不同进程间进行通信的机制，但它们之间有几个区别：

- 编程接口：System V IPC和POSIX IPC的编程接口不同。System V IPC使用IPC对象（如信号量、共享内存和消息队列）来实现进程间通信，而POSIX IPC使用命名对象（如命名信号量、命名共享内存和命名管道）。
- 可移植性：POSIX IPC是由IEEE POSIX标准定义的，因此POSIX IPC是可移植的，可在不同的操作系统上使用。而System V IPC是由System V操作系统提供的，因此不同的操作系统可能实现不同，因此在跨平台时可能会有问题。
- 接口和实现：System V IPC的接口和实现是紧密耦合的，而POSIX IPC的接口和实现是松散耦合的。这意味着在POSIX IPC中，接口和实现是独立的，因此可以在实现中进行更改，而不影响接口，这使得在不同的系统上实现相同的接口变得更容易。
- 特性：System V IPC提供的特性比POSIX IPC多，例如，System V IPC提供了消息队列，而POSIX IPC则没有。另一方面，POSIX IPC提供了诸如命名管道之类的特性，而System V IPC则没有。

综上所述，System V IPC和POSIX IPC都有其优点和缺点。在选择使用哪种IPC机制时，需要根据具体应用场景和需求进行权衡。
<a name="v1dM8"></a>
## 3、共享内存实现方式
在Linux下，共享内存可以使用System V IPC机制或POSIX IPC机制实现。

- 使用System V IPC机制：

使用`shmget()`函数创建共享内存区域并获取其标识符。使用`shmat()`函数将共享内存区域附加到进程地址空间中。使用`shmdt()`函数将共享内存区域从进程地址空间中分离。使用`shmctl()`函数控制共享内存区域的属性和状态。

- 使用POSIX IPC机制：

使用`shm_open()`函数创建共享内存区域并获取其文件描述符。使用`ftruncate()`函数调整共享内存区域的大小。使用`mmap()`函数将共享内存区域映射到进程地址空间中。使用`munmap()`函数解除共享内存区域与进程地址空间的映射关系。使用`shm_unlink()`函数删除共享内存区域的文件名并释放资源。
<a name="vvMfv"></a>
### System V IPC机制实现共享内存
以下是一个使用System V IPC机制实现共享内存的简单例程，它展示了如何创建、附加和分离共享内存区域。
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024 // 共享内存大小

int main() {
    int shmid;
    char *shmaddr;
    char s8ReadBuf[1024] = {0};
    key_t key = ftok(".", 's'); // 获取共享内存标识符

    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // 创建共享内存区域
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // 将共享内存区域附加到进程地址空间中
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    #if 1
    // 在共享内存中写入数据
    strncpy(shmaddr, "Hello, world!", SHM_SIZE);
    #else
    // 读数据
    // memcpy(s8ReadBuf, shmaddr, 1024);
    // printf("s8ReadBuf:%s\n", s8ReadBuf);
    #endif

    // 分离共享内存区域
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
```
在上面的例程中，首先使用`ftok()`函数生成一个key值作为共享内存的标识符。然后使用shmget()函数创建共享内存区域，shmaddr指向共享内存区域的起始地址。最后使用`shmdt()`函数分离共享内存区域。
<a name="Pd477"></a>
### POSIX IPC机制实现共享内存
以下是一个使用POSIX IPC机制实现共享内存的简单例程，它展示了如何创建、映射和解除映射共享内存区域。
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE 1024 // 共享内存大小
#define SHM_NAME "/myshm" // 共享内存名称

int main() {
    int fd;
    char *shmaddr;
    char s8ReadBuf[1024] = {0};
    const char *msg = "Hello, world!";

    // 创建共享内存区域
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // 调整共享内存区域的大小
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // 映射共享内存区域到进程地址空间中
    shmaddr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmaddr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    #if 1
    // 在共享内存中写入数据
    strncpy(shmaddr, msg, SHM_SIZE);
    #else
    // 读数据
    // memcpy(s8ReadBuf, shmaddr, 1024);
    // printf("s8ReadBuf:%s\n", s8ReadBuf);
    #endif
    // 解除共享内存区域与进程地址空间的映射关系
    if (munmap(shmaddr, SHM_SIZE) == -1) {
        perror("munmap");
        exit(1);
    }

    // 删除共享内存区域的文件名并释放资源
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
```
在上面的例程中，使用`shm_open()`函数创建一个共享内存区域，然后使用`ftruncate()`函数调整共享内存区域的大小。接着，使用`mmap()`函数将共享内存区域映射到进程地址空间中，并使用`strncpy()`函数在共享内存中写入数据。最后，使用`munmap()`函数解除共享内存区域与进程地址空间的映射关系，并使用`shm_unlink()`函数删除共享内存区域的文件名并释放资源。
