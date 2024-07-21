Nginx
<a name="fzHBJ"></a>
## 一、Nginx基础架构
Nginx启动后以daemon形式在后台运行，后台进程包含一个master进程和多个worker进程。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149307-886a64ad-74a4-430b-bd9d-5aef34675799.png#averageHue=%232f2a27&clientId=u869bc1a1-47b5-4&from=paste&id=u9682aff3&originHeight=196&originWidth=669&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uc1dc0e15-5b74-49f4-bd9b-22cd507f165&title=)<br />Nginx是由一个master管理进程，多个worker进程处理工作的多进程模型。基础架构设计，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149403-dd3def9b-4ea4-44f6-806a-1e17e9cf7033.png#averageHue=%23f1edeb&clientId=u869bc1a1-47b5-4&from=paste&id=ue0d4cd14&originHeight=547&originWidth=860&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34d417e7-c77e-446c-8673-248940473b6&title=)<br />Master负责管理worker进程，worker进程负责处理网络事件。整个框架被设计为一种依赖事件驱动、异步、非阻塞的模式。<br />如此设计的优点有：

- 可以充分利用多核机器，增强并发处理能力。
- 多worker间可以实现负载均衡。
- Master监控并统一管理worker行为。在worker异常后，可以主动拉起worker进程，从而提升了系统的可靠性。并且由Master进程控制服务运行中的程序升级、配置项修改等操作，从而增强了整体的动态可扩展与热更的能力。
<a name="BwrkP"></a>
## 二、Master进程
<a name="Ci0vC"></a>
### 1、核心逻辑
Master进程的主逻辑在`ngx_master_process_cycle`，核心关注源码：
```c
ngx_master_process_cycle(ngx_cycle_t *cycle)
{
    ...
    ngx_start_worker_processes(cycle, ccf->worker_processes,
                                        NGX_PROCESS_RESPAWN);
    ...
    for ( ;; ) {
        if (delay) {...}
        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "sigsuspend");
        sigsuspend(&set);
        ngx_time_update();
        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                             "wake up, sigio %i", sigio);
        if (ngx_reap) {
            ngx_reap = 0;
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "reap children");
            live = ngx_reap_children(cycle);
        }
        if (!live && (ngx_terminate || ngx_quit)) {...}
        if (ngx_terminate) {...}
        if (ngx_quit) {...}
        if (ngx_reconfigure) {...}
        if (ngx_restart) {...}
        if (ngx_reopen) {...}
        if (ngx_change_binary) {...}
        if (ngx_noaccept) {
            ngx_noaccept = 0;
            ngx_noaccepting = 1;
            ngx_signal_worker_processes(cycle, ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
        }
    }
 }
```
由上述代码，可以理解，master进程主要用来管理worker进程，具体包括如下4个主要功能：<br />1）**接受来自外界的信号。**其中master循环中的各项标志位就对应着各种信号，如：`ngx_quit`代表`QUIT`信号，表示优雅的关闭整个服务。<br />2）**向各个worker进程发送信。**比如`ngx_noaccept`代表`WINCH`信号，表示所有子进程不再接受处理新的连接，由master向所有的子进程发送QUIT信号量。<br />3）**监控worker进程的运行状态。**比如`ngx_reap`代表`CHILD`信号，表示有子进程意外结束，这时需要监控所有子进程的运行状态，主要由`ngx_reap_children`完成。<br />4）**当woker进程退出后（异常情况下），会自动重新启动新的woker进程。**主要也是在`ngx_reap_children`。
<a name="pwKnt"></a>
### 2、热更
<a name="Vi89i"></a>
#### 1）热重载-配置热更
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149380-738a31ff-bb2d-4aa3-b92b-8dd77e5b3c8e.png#averageHue=%23fbf9f5&clientId=u869bc1a1-47b5-4&from=paste&id=ucf481be5&originHeight=349&originWidth=652&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1acdd0cd-950c-43db-807c-7585d93a0de&title=)<br />Nginx热更配置时，可以保持运行中平滑更新配置，具体流程如下：

- 更新nginx.conf配置文件，向master发送SIGHUP信号或执行`nginx -s reload`
- Master进程使用新配置，启动新的worker进程
- 使用旧配置的worker进程，不再接受新的连接请求，并在完成已存在的连接后退出
<a name="Iphkd"></a>
#### 2）热升级-程序热更
Nginx热升级过程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149435-ee1bcc9d-328a-438a-aa4f-98e89107dff1.png#averageHue=%23fbf8f0&clientId=u869bc1a1-47b5-4&from=paste&id=u721b518a&originHeight=247&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ed9443e-c6e9-4c09-ac42-ce6bfaa00ba&title=)

- 将旧Nginx文件换成新Nginx文件（注意备份）
- 向master进程发送USR2信号（平滑升级到新版本的Nginx程序）
- master进程修改pid文件号，加后缀.oldbin
- master进程用新Nginx文件启动新master进程，此时新老master/worker同时存在。
- 向老master发送WINCH信号，关闭旧worker进程，观察新worker进程工作情况。若升级成功，则向老master进程发送QUIT信号，关闭老master进程；若升级失败，则需要回滚，向老master发送HUP信号（重读配置文件），向新master发送QUIT信号，关闭新master及worker。
<a name="uxLIP"></a>
## 三、Worker进程
<a name="XH5MG"></a>
### 1、核心逻辑
Worker进程的主逻辑在ngx_worker_process_cycle，核心关注源码：
```basic
ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ngx_int_t worker = (intptr_t) data;
    ngx_process = NGX_PROCESS_WORKER;
    ngx_worker = worker;
    ngx_worker_process_init(cycle, worker);
    ngx_setproctitle("worker process");
    for ( ;; ) {
        if (ngx_exiting) {...}
        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "worker cycle");
        ngx_process_events_and_timers(cycle);
        if (ngx_terminate) {...}
        if (ngx_quit) {...}
        if (ngx_reopen) {...}
    }
}
```
由上述代码，可以理解，worker进程主要在处理网络事件，通过`ngx_process_events_and_timers`方法实现，其中事件主要包括：网络事件、定时器事件。
<a name="V7ZAJ"></a>
### 2、事件驱动-epoll
Worker进程在处理网络事件时，依靠epoll模型，来管理并发连接，实现了事件驱动、异步、非阻塞等特性。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149382-1f521a4d-a554-45b6-8ed8-f54dcb62325a.png#averageHue=%23505052&clientId=u869bc1a1-47b5-4&from=paste&id=u814447ee&originHeight=355&originWidth=376&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=udb4bbff9-42a7-492c-8812-fa5c5735ec9&title=)<br />通常海量并发连接过程中，每一时刻（相对较短的一段时间），往往只需要处理一小部分有事件的连接即活跃连接。基于以上现象，epoll通过将连接管理与活跃连接管理进行分离，实现了高效、稳定的网络IO处理能力。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149962-a23ecf19-e93a-4b66-b17d-50af2c9a44ae.png#averageHue=%23f5f5f4&clientId=u869bc1a1-47b5-4&from=paste&id=u5662963c&originHeight=415&originWidth=554&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc277071-fc3a-4854-9171-72e05bb2864&title=)<br />其中，epoll利用红黑树高效的增删查效率来管理连接，利用一个双向链表来维护活跃连接。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280149972-38c113cf-3d5d-413c-bce5-1f68ec8602d3.png#averageHue=%23f7f7f7&clientId=u869bc1a1-47b5-4&from=paste&id=u8088cbb2&originHeight=445&originWidth=467&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1027b3a7-606d-4417-9b3a-c19a9851ad5&title=)
<a name="KN0No"></a>
### 3、惊群
由于worker都是由master进程fork产生，所以worker都会监听相同端口。这样多个子进程在accept建立连接时会发生争抢，带来著名的“惊群”问题。<br />Worker核心处理逻辑`ngx_process_events_and_timers`核心代码如下：
```c
void ngx_process_events_and_timers(ngx_cycle_t *cycle){
    //这里面会对监听socket处理
    ...

    if (ngx_accept_disabled > 0) {
            ngx_accept_disabled--;
    } else {
        //获得锁则加入wait集合,
        if (ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {
            return;
        }
        ...
        //设置网络读写事件延迟处理标志，即在释放锁后处理
        if (ngx_accept_mutex_held) {
            flags |= NGX_POST_EVENTS;
        }
    }
    ...
    //这里面epollwait等待网络事件
    //网络连接事件，放入ngx_posted_accept_events队列
    //网络读写事件，放入ngx_posted_events队列
    (void) ngx_process_events(cycle, timer, flags);
    ...
    //先处理网络连接事件，只有获取到锁，这里才会有连接事件
    ngx_event_process_posted(cycle, &ngx_posted_accept_events);
    //释放锁，让其他进程也能够拿到
    if (ngx_accept_mutex_held) {
        ngx_shmtx_unlock(&ngx_accept_mutex);
    }
    //处理网络读写事件
    ngx_event_process_posted(cycle, &ngx_posted_events);
}
```
由上述代码可知，Nginx解决惊群的方法：<br />1）将连接事件与读写事件进行分离。连接事件存放为`ngx_posted_accept_events`，读写事件存放为`ngx_posted_events`。<br />2）设置`ngx_accept_mutex`锁，只有获得锁的进程，才可以处理连接事件。
<a name="bk4r6"></a>
### 4、负载均衡
Worker间的负载关键在于各自接入了多少连接，其中接入连接抢锁的前置条件是`ngx_accept_disabled > 0`，所以`ngx_accept_disabled`就是负载均衡机制实现的关键阈值。
```c
ngx_int_t             ngx_accept_disabled;
ngx_accept_disabled = ngx_cycle->connection_n / 8 - ngx_cycle->free_connection_n;
```
因此，在nginx启动时，`ngx_accept_disabled`的值就是一个负数，其值为连接总数的7/8。当该进程的连接数达到总连接数的7/8时，该进程就不会再处理新的连接了。<br />同时每次调用'`ngx_process_events_and_timers`'时，将`ngx_accept_disabled`减1，直到其值低于阈值时，才试图重新处理新的连接。<br />因此，nginx各worker子进程间的负载均衡仅在某个worker进程处理的连接数达到它最大处理总数的7/8时才会触发，其负载均衡并不是在任意条件都满足。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624280150120-988b4959-bb22-4370-a01e-5d3c43baf0ab.png#averageHue=%2315110e&clientId=u869bc1a1-47b5-4&from=paste&id=u73e5f77d&originHeight=553&originWidth=761&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uc19470bc-d62e-44fb-89d1-316c9920cca&title=)<br />其中'pid'为1211的进程为master进程，其余为worker进程。
<a name="l8WWG"></a>
## 四、思考
<a name="ODDWp"></a>
### 1、为什么不采用多线程模型管理连接？

- 无状态服务，无需共享进程内存
- 采用独立的进程，可以让互相之间不会影响。一个进程异常崩溃，其他进程的服务不会中断，提升了架构的可靠性。
- 进程之间不共享资源，不需要加锁，所以省掉了锁带来的开销。
<a name="dggjw"></a>
### 2、为什么不采用多线程处理逻辑业务？

- 进程数已经等于核心数，再新建线程处理任务，只会抢占现有进程，增加切换代价。
- 作为接入层，基本上都是数据转发业务，网络IO任务的等待耗时部分，已经被处理为非阻塞/全异步/事件驱动模式，在没有更多CPU的情况下，再利用多线程处理，意义不大。并且如果进程中有阻塞的处理逻辑，应该由各个业务进行解决，比如openResty中利用了Lua协程，对阻塞业务进行了优化。
