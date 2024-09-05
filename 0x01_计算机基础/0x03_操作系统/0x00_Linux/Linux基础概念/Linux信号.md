Linux
<a name="k4q2w"></a>
## 1、信号是什么？
信号其实就是一个软件中断。<br />例：

1. 输入命令，在 Shell 下启动一个前台进程。
2. 用户按下 Ctrl-C，键盘输入产生一个硬件中断。
3. 如果 CPU 当前正在执行这个进程的代码，则该进程的用户空间代码暂停执行， CPU 从用户态切换到内核态处理硬件中断。
4. 终端驱动程序将 Ctrl-C 解释成一个 SIGINT 信号，记在该进程的 PCB 中（也可以说发送了一个 SIGINT 信号给该进程）。
5. 当某个时刻要从内核返回到该进程的用户空间代码继续执行之前，首先处理 PCB 中记录的信号，发现有一个 SIGINT 信号待处理，而这个信号的默认处理动作是终止进程，所以直接终止进程而不再返回它的用户空间代码执行。

在这个例子中，由 ctrl+c 产生的硬件中断就是一个信号。Ctrl+C 产生的信号只能发送给前台进程，命令后加 & 就可放到后台运行。Shell 可同时运行一个前台进程和任意多个后台进程，只有前台进程才能接受到像 CTRL+C 这种控制键产生的信号。
<a name="xJsFB"></a>
## 2、信号的种类
使用命令查看：
```bash
kill -l
```
非可靠信号：1~31 号信号，信号可能会丢失 可靠信号：34~64 号信号，信号不可能丢失<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668855479288-34dffa99-5ac3-4013-b05e-c39a428c661d.png#averageHue=%23330e29&clientId=ube44fc9d-6d07-4&from=paste&id=u5e58b047&originHeight=342&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc4486b6a-90f3-4fa4-81bb-f1bd2dcf961&title=)<br />SIGHUP：1 号信号，Hangup detected on controlling terminal or death of controlling process（在控制终端上挂起信号，或让进程结束），ation：term<br />SIGINT：2 号信号，Interrupt from keyboard（键盘输入中断，**「ctrl + c」** ），action：term<br />SIGQUIT：3 号信号，Quit from keyboard（键盘输入退出**「ctrl+ |」** ），action：core，产生 core dump 文件<br />SIGABRT：6 号信号，Abort signal from abort(3)（非正常终止，**「double free」**），action：core<br />SIGKILL：9 号信号，Kill signal（杀死进程信号），action：term，该信号不能被阻塞、忽略、自定义处理<br />SIGSEGV：11 号信号，Invalid memory reference（无效的内存引用，解引用空指针、内存越界访问），action：core<br />SIGPIPE：13 号信号，Broken pipe: write to pipe with no readers（管道中止：写入无人读取的管道，会导致管道破裂），action：term<br />SIGCHLD：17 号信号，Child stopped or terminated（子进程发送给父进程的信号，但该信号为忽略处理的）<br />SIGSTOP：19 号信号，Stop process（停止进程），action：stop<br />SIGTSTP：20 号信号，Stop typed at terminal（终端上发出的停止信号，**「ctrl + z」**），action：stop<br />具体的信号采取的动作和详细信息可查看：**「man 7 signal」**
<a name="jHvtt"></a>
## 3、信号的产生
<a name="w4auA"></a>
### 3.1 硬件产生
硬件产生即通过终端按键产生的信号：

1. ctrl + c：SIGINT(2)，发送给前台进程，& 进程放到后台运行，fg 把刚刚放到后台的进程，再放到前台来运行
2. ctrl + z：SIGTSTP(20)，一般不用，除非有特定场景
3. ctrl + | ：SIGQUIT(3)，产生 core dump 文件

产生 core dump 文件的条件：
:::danger
当前OS一定不要限制core dump文件的大小，ulimit -a<br />磁盘空间要足够<br />如何产生：<br />3.1 解引用空指针，收到11号信号，产生core dump文件<br />3.2 内存访问越界，程序一旦崩溃，就会收到11号信号，也就会产生core dump文件<br />3.3 double free，收到6号信号，并产生core dump。<br />3.4 free（NULL），不会崩溃
:::
<a name="FCsXM"></a>
### 3.2 软件产生
软件产生即调用系统函数向进程发信号

1. kill 函数
```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
参数解释：
pid：进程号
sig：要发送的信号值
返回值：成功返回0，失败返回-1，并设置错误
```

1. kill 命令：kill -[信号] pid，
2. abort：`void abort(void);`，收到 6 号信号，谁调用该函数，谁就收到信号
3. alarm：`unsigned int alarm(unsigned int seconds);`，收到 14 号信号，告诉内核在 seconds 秒后给进程发送 SIGALRM 信号，该信号默认处理动作为终止当前进程。
<a name="TBwT0"></a>
## 4、信号的注册
信号注册又分为可靠信号的注册和非可靠信号的注册。信号注册实际上是一个位图和一个 sigqueue 队列。<br />![2022-11-19-19-00-31.846132900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668855684292-adeb67af-7ca4-4a67-be7f-3f2774f8a18e.png#averageHue=%23f9f8f8&clientId=ube44fc9d-6d07-4&from=ui&id=u73c8d715&originHeight=446&originWidth=1331&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45895&status=done&style=none&taskId=u66d139ce-c2e8-458b-9195-18d1af3523e&title=)
<a name="sRGe7"></a>
### 4.1 非可靠信号的注册
当进程收到非可靠信号时：

1. 将非可靠信号对应的比特位置为 1
2. 添加 sigqueue 节点到 sigqueue 队列当中，但是，在添加 sigqueue 节点的时候，队列当中已然有了该信号的 sigqueue 节点，则不添加
<a name="L4TLJ"></a>
### 4.2 可靠信号的注册
当进程所受到可靠信号时：<br />在 sig 位图中更改信号对应的比特位为 1 不论之前 sigqueue 队列中是否存在该信号的 sigqueue 节点，都再次添加 sigqueue 节点到 sigqueue 队列当中去
<a name="QVz9E"></a>
## 5、信号的注销
<a name="hBqvB"></a>
### 5.1 非可靠信号的注销
信号对应的比特位从 1 置为 0 将该信号的 sigqueue 节点从 sigqueue 队列当中进行出队操作
<a name="zFd2L"></a>
### 5.2 可靠信号的注销
将该信号的 sigqueue 节点从 sigqueue 队列当中进行出队操作 需要判断 sigqueue 队列当中是否还有相同的 sigqueue 节点：①没有了：信号比特位从 1 置为 0 ②还有：不会更改 sig 位图中的比特位
<a name="OOstp"></a>
## 6、信号阻塞
<a name="gLIMz"></a>
### 6.1 信号是怎样阻塞的？
![2022-11-19-19-00-32.049567200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668855728350-9db83793-7b5a-4ac9-bb10-877bb531158c.png#averageHue=%23fefefe&clientId=ube44fc9d-6d07-4&from=ui&id=u5abf9992&originHeight=276&originWidth=1064&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14473&status=done&style=none&taskId=u3db92776-73a7-4f3b-b8a0-c15bfde9a34&title=)<br />信号的阻塞，并不会干扰信号的注册。信号能注册，但不能被立即处理， 将 block 位图中对应的信号比特位置为 1，表示阻塞该信号 进程收到该信号，还是一如既往的注册 当进程进入到内核空间，准备返回用户空间的时候，调用 do_signal 函数，就不会立即去处理该信号了 当该信号不被阻塞后，就可以进行处理了
<a name="fV7ay"></a>
### 6.2 sigprocmask
函数原型：`int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);` 参数解释：
```
how，该做什么样的操作
SIG_BLOCK：设置信号为阻塞
SIG_UNBLOCK：解除信号阻塞
SIG_SETMASK：替换阻塞位图
set：用来设置阻塞位图
SIG_BLOCK：设置某个信号为阻塞，block（new） = block（old） | set
SIG_UNBLOCK：解除某个信号阻塞，block（new）= block（old） & （~set）
SIG_SETMASK：替换阻塞位图，block（new）= set
oldset：原来的阻塞位图
```
例：下述例子，信号全部被阻塞，采用 `kill -9`，将该进程结束掉<br />![2022-11-19-19-00-32.170899700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668855936078-6c7b96b3-b848-4fc4-b067-a3789ef0a84e.png#averageHue=%23151414&clientId=ube44fc9d-6d07-4&from=ui&id=u00ae2005&originHeight=449&originWidth=756&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35678&status=done&style=none&taskId=u7523a52d-6a4b-41dc-bcdb-6968d071e67&title=)<br />结果：此时发送信号是不会有作用的，采用 `kill -9` 强杀掉<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668855479229-4356cc80-2aa0-4627-8b80-2b655e90aea5.png#averageHue=%23080707&clientId=ube44fc9d-6d07-4&from=paste&id=uda556f24&originHeight=115&originWidth=616&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucc786b48-31bb-4b87-9f4b-300903ef63d&title=)<br />代码：
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


void signcallback(int signumber)
{
    printf("change the signal %d\n",signumber);
}

int main()
{
    sigset_t set;
    sigset_t oldset;
    sigfillset(&set);//所有比特位全置为1，则信号全部会被阻塞
    sigprocmask(SIG_BLOCK,&set,&oldset);
    while(1)
  {
      sleep(1);
  }

    return 0;
}
```
<a name="RwXSm"></a>
## 7、信号未决
<a name="P9jW3"></a>
### 7.1 未决概念
实际执行信号的处理动作称为信号递达（Delivery），信号从产生到递达之间的状态，称为信号未决（Pending）。进程可以选择阻塞（Block）某个信号。被阻塞的信号产生时将保持在未决状态，直到进程解除对此信号的阻塞，才执行递达的动作。注意，阻塞和忽略是不同的，只要信号被阻塞就不会递达，而忽略是、在递达之后可选的一种处理动作。
<a name="cf5gJ"></a>
### 7.2 sigpending
函数原型：`int sigpending(sigset_t *set);` 读取当前进程的未决信号集，通过 set 参数传出。调用成功返回 0，出错返回 - 1。<br />例：<br />![2022-11-19-19-04-42.123737200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856036606-9ff0b88e-fbf1-47af-a2c8-9082c00d79cc.png#averageHue=%23141414&clientId=ube44fc9d-6d07-4&from=ui&id=u822cec02&originHeight=786&originWidth=1009&originalType=binary&ratio=1&rotation=0&showTitle=false&size=67259&status=done&style=none&taskId=u7219d27b-b2ae-40af-ba8d-a36a38be222&title=)<br />结果：<br />![2022-11-19-19-05-44.322414300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856054898-223a75af-4326-4324-99d6-aefc5e44b9fa.png#averageHue=%2320170e&clientId=ube44fc9d-6d07-4&from=ui&id=u8a408407&originHeight=396&originWidth=1607&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37065&status=done&style=none&taskId=uab343475-c69b-446a-b0b3-a81f9ad4e01&title=)<br />代码：
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalcallback(int signumber)
{
    printf("chang signumber %d\n",signumber);
}
void printsigset(sigset_t *set)
{
    int i = 0;
    for(;i < 32;i++)
  {
      if(sigismember(set,i))
    {
        putchar('1');
    }
      else{
        putchar('0');
    }
  }
}

int main()
{
    signal(2,signalcallback);
    signal(10,signalcallback);
    sigset_t set;
    sigset_t oldset;
    sigset_t pending;
    sigfillset(&set);//所有比特位全部置为1，则信号会全部被阻塞
    sigprocmask(SIG_BLOCK,&set,&oldset);
    while(1)
  {
      sigpending(&pending);
      printsigset(&pending);
      sleep(1);
  }

    return 0;
}
```
<a name="ScDTD"></a>
## 8、信号的处理方式
![2022-11-19-19-05-44.520488200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856084745-165d61dc-31ee-400b-a9d9-6a082361efe2.png#averageHue=%23f5f4f4&clientId=ube44fc9d-6d07-4&from=ui&id=uc2c13c66&originHeight=441&originWidth=1274&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72454&status=done&style=none&taskId=uca4bfa8e-beeb-4cb5-8360-b1a06c16d16&title=)
:::danger
每个信号都有两个标志位分别表示阻塞和未决，还有一个函数指针表示处理动作。
:::
在上述例子中：

1. SIGHUP 信号未阻塞也未产生过，当它递达时执行默认处理动作。
2. SIGINT 信号产生过，但正在被阻塞，所以暂时不能递达。虽然它的处理动作是忽略，但在没有解除阻塞之前不能忽略这个信号，因为进程仍有机会改变处理动作之后再解除阻塞。
3. SIGQUIT 信号未产生过，一旦产生 SIGQUIT 信号将被阻塞，它的处理动作是用户自定义函数 `sighandler`。
<a name="DZ6Aq"></a>
### 8.1 `signal` 函数
该函数可以更改信号的处理动作。
```c
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
参数解释：

signum：更改的信号值
handler：函数指针，要更改的动作是什么
```
实际上，该函数内部也调用了 `sigaction` 函数。
<a name="k5rwj"></a>
### 8.2 `sigaction` 函数
读取和修改与指定信号相关联的处理动作。
```c
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```
参数解释：
```
signum：待更改的信号值
```
`struct sigaction` 结构体：
```c
void     (*sa_handler)(int);//函数指针，保存了内核对信号的处理方式
void     (*sa_sigaction)(int, siginfo_t *, void *);//
sigset_t   sa_mask;//保存的是当进程在处理信号的时候，收到的信号
int        sa_flags;//SA_SIGINFO，OS在处理信号的时候，调用的就是sa_sigaction函数指针当中
//保存的值0，在处理信号的时候，调用sa_handler保存的函数
void     (*sa_restorer)(void);
```
例：<br />![2022-11-19-19-05-44.788998600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856146779-c5f1e62a-966c-4dab-97b6-02ae22074ea4.png#averageHue=%2311100f&clientId=ube44fc9d-6d07-4&from=ui&id=uf2207b27&originHeight=482&originWidth=721&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42600&status=done&style=none&taskId=u7de1cb03-01c7-48b3-91ca-2da5bdc23f8&title=)<br />结果：<br />![2022-11-19-19-05-44.986652900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856146775-97aa181f-ddf2-423d-833d-b4f5993a2d87.png#averageHue=%23060503&clientId=ube44fc9d-6d07-4&from=ui&id=u31e46c3f&originHeight=154&originWidth=631&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10667&status=done&style=none&taskId=ua89621c8-0ab0-46b2-ad31-f6d851cfe1e&title=)<br />代码：
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signcallback(int signumber)
{
    printf("change signumber %d\n",signumber);
}


int main()
{
    struct sigaction act;//act为入参
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = signcallback;

    struct sigaction oldact;//oldact为出参
    sigaction(3,&act,&oldact);
    while(1)
  {
      sleep(1);
  }
    return 0;
}
```
<a name="fj8bg"></a>
### 8.3 自定义信号处理的流程
![2022-11-19-19-05-45.196812900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856185829-ea0b9716-8e89-48cd-ab6f-9f4bce9145fe.png#averageHue=%23fbf9f7&clientId=ube44fc9d-6d07-4&from=ui&id=uc7e196b7&originHeight=642&originWidth=1436&originalType=binary&ratio=1&rotation=0&showTitle=false&size=67249&status=done&style=none&taskId=ua974e30c-f47e-4b0f-9244-fa1829a50f3&title=)

1. **「task_struct」** 结构体中有一个**「**`**struct sighand_struct**`**」** 结构体。
2. **「**`**struct sighand_struct**`**」** 结构体有一个 **「**`**struct k_sigaction action[_NSIG]**`**」** 结构体数组。
3. 该数组中，其中的 **「**`**_sighandler_t sa_handler**`**」** 保存的是信号的处理方式，通过改变其指向，可以实现我们对自定义信号的处理。
<a name="BmUvS"></a>
## 9、信号的捕捉
<a name="eySfg"></a>
### 9.1 信号捕捉的条件
如果信号的处理动作是用户自定义函数，在信号递达时就调用这个函数，这就称为信号捕捉。
<a name="wXNQf"></a>
### 9.2 信号捕捉流程
![2022-11-19-19-05-45.402218400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856234885-03808dac-0331-4a83-846d-1894b0f02148.png#averageHue=%23fefefe&clientId=ube44fc9d-6d07-4&from=ui&id=uff44eddd&originHeight=587&originWidth=786&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27608&status=done&style=none&taskId=u736e15ab-5c55-4786-8019-e993297a823&title=)<br />内核态返回用户态会调用 `do_signal` 函数，两种情况：

1. 无信号：`sys_return` 函数，返回用户态
2. 有信号：先处理信号，信号返回，再调用 `do_signal` 函数 例：
3. 程序注册了 SIGQUIT 信号的处理函数 `sighandler`。
4. 当前正在执行 main 函数，这时发生中断或异常切换到内核态。
5. 在中断处理完毕后要返回用户态的 main 函数之前检查到有信号 SIGQUIT 递达。
6. 内核决定返回用户态后不是恢复 main 函数的上下文继续执行，而是执行 `sighandler` 函数， `sighandler` 和 main 函数使用不同的堆栈空间，它们之间不存在调用和被调用的关系，是两个独立的控制流程。
7. `sighandler` 函数返回后自动执行特殊的系统调用 `sigreturn` 再次进入内核态。
8. 如果没有新的信号要递达，这次再返回用户态就是恢复 main 函数的上下文继续执行了。
<a name="MoME5"></a>
## 10、常用信号集操作函数
```c
int sigemptyset(sigset_t *set);：//将比特位图全置为0

int sigfillset(sigset_t *set);//将比特位图全置为1

int sigaddset(sigset_t *set, int signum);//将该set位图，多少号信号置为1

int sigdelset(sigset_t *set, int signum);//将该set位图，多少号信号置为0

int sigismember(const sigset_t *set, int signum);//信号signum是否是set位图中的信号
```
<a name="rNVuI"></a>
## 11、SIGCHLD 信号
该信号是子进程在结束是发送给父进程的信号，但是该信号的处理方式是默认处理的。父进程对子进程发送过来的 SIGCHLD 信号进行了忽略处理，就会导致子进程成为僵尸进程。<br />可以自定义该信号的处理方式：<br />![2022-11-19-19-05-45.709233400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856308054-2ccd294c-84e7-4d7f-9458-6c0a8bdddd4e.png#averageHue=%23060505&clientId=ube44fc9d-6d07-4&from=ui&id=u01623c77&originHeight=693&originWidth=894&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54564&status=done&style=none&taskId=u04c4b99d-0a7a-4dc3-a95f-f87f3f6dac5&title=)<br />指令查看后台：**「**`**ps aux | grep ./fork**`**」**<br />![2022-11-19-19-05-46.058241000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668856308055-0fc995a2-8462-4c3f-aeb3-c28c22722001.png#averageHue=%23080403&clientId=ube44fc9d-6d07-4&from=ui&id=ub55cf42c&originHeight=113&originWidth=967&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15379&status=done&style=none&taskId=u4b3e541c-a72c-4d92-9bb1-e1cb7511547&title=)<br />代码：
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

void signcallback(int signumber)
{
    printf("change signal %d\n",signumber);
    wait(NULL);
}

int main()
{
    signal(17,signcallback);
    pid_t pid = fork();
    if(pid < 0)
  {
      perror("fork");
      return -1;
  }
    else if(pid == 0)
  {
      printf("I am child\n");
      sleep(1);
      exit(12);
  }
    else{
      while(1)
    {
        sleep(1);
    }
  }
    return 0;
}
```
