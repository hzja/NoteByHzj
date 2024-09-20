Java线程池<br />Java并发编程中在使用到ThreadPoolExecutor时，对它的三个关闭方法（`shutdown()`、`shutdownNow()`、`awaitTermination()`）的异同点如下：
<a name="lSKMd"></a>
## `shutdown()`
将线程池状态置为`SHUTDOWN`，并不会立即停止：

1. 停止接收外部`submit`的任务
2. 内部正在跑的任务和队列里等待的任务，会执行完
3. 等到第二步完成后，才真正停止
<a name="zIveu"></a>
## `shutdownNow()`
将线程池状态置为STOP。企图立即停止，事实上不一定：

1. 跟`shutdown()`一样，先停止接收外部提交的任务
2. 忽略队列里等待的任务
3. 尝试将正在跑的任务`interrupt`中断
4. 返回未执行的任务列表

它试图终止线程的方法是通过调用`Thread.interrupt()`方法来实现的，但是大家知道，这种方法的作用有限，如果线程中没有sleep 、wait、Condition、定时锁等应用，`interrupt()`方法是无法中断当前的线程的。所以，`ShutdownNow()`并不代表线程池就一定立即就能退出，它也可能必须要等待所有正在执行的任务都执行完成了才能退出。<br />但是大多数时候是能立即退出的
<a name="CSY3C"></a>
## `awaitTermination(long timeOut, TimeUnit unit)`
当前线程阻塞，直到

1. 等所有已提交的任务（包括正在跑的和队列中等待的）执行完
2. 或者等超时时间到
3. 或者线程被中断，抛出`InterruptedException`<br />然后返回true（`shutdown`请求后所有任务执行完毕）或false（已超时）

`_shuntdown()_`_和_`_awaitTermination()_`_效果差不多，方法执行之后，都要等到提交的任务全部执行完才停。_
<a name="wNFxP"></a>
## `shutdown()` 和 `shutdownNow()`的区别
从字面意思就能理解，`shutdownNow()`能立即停止线程池，正在跑的和正在等待的任务都停下了。这样做立即生效，但是风险也比较大；<br />`shutdown()`只是关闭了提交通道，用`submit()`是无效的；而内部该怎么跑还是怎么跑，跑完再停。
> _Between client threads and thread pool there is a queue of tasks. When your application shuts down, you must take care of two things: what is happening with queued tasks and how already running tasks are behaving (more on that later). Surprisingly many developers are not shutting down thread pool properly or consciously. There are two techniques: either let all queued tasks to execute (shutdown()) or drop them (shutdownNow()) - it totally depends on your use case._

<a name="QZboX"></a>
## `shutdown()`和`awaitTermination()`的区别

1. `shutdown()`后，不能再提交新的任务进去；但是`awaitTermination()`后，可以继续提交。
2. `awaitTermination()`是阻塞的，返回结果是线程池是否已停止（true/false）；`shutdown()`不阻塞。
<a name="BqrtP"></a>
## 总结

1. 优雅的关闭，用`shutdown()`
2. 想立马关闭，并得到未执行任务列表，用`shutdownNow()`
3. 优雅的关闭，并允许关闭声明后新任务能提交，用`awaitTermination()`

_ps：关闭功能 【从强到弱】 依次是：_`_shuntdownNow()_`_ > _`_shutdown()_`_ > _`_awaitTermination()_`<br />`_awaitTermination_`_并不是用来关闭线程池，它只是用来检测timeout时间后线程池是否关闭。_<br />_一般在调用_`_shutdown()_`_方法后调用_
