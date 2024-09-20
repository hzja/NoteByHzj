Java 线程
<a name="TM5z7"></a>
## 线程状态
先来个开场四连问

- Java线程状态有几个？
- Java线程状态是如何转换？
- Java线程状态转换什么情况会进入锁的等待队列？
- Java线程状态转换什么情况会进入锁的同步队列？

![2021-08-02-16-37-35-325327.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893489219-8581676e-991d-4414-898c-4ce79a6ec185.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u901b3725&originHeight=470&originWidth=687&originalType=binary&ratio=1&size=182522&status=done&style=shadow&taskId=u48a1bad0-54d4-4ae7-820e-ca9d45898fe)<br />首先线程的状态可以分为6态或7态，具体状态如下<br />6态

- `New`：新建状态
- `Runnable`：可运行状态
- `Terminated`：终止状态
- `Waiting`：等待状态
- `TimedWaiting`：超时等待状态
- `Blocked`：阻塞状态

7态

- `New`：新建状态
- `Ready`：就绪状态
- `Running`：运行状态
- `Terminated`：终止状态
- `Waiting`：等待状态
- `TimedWaiting`：超时等待状态
- `Blocked`：阻塞状态

其实6态与7态差别不大，只不过7态把`Runnable`可运行状态，拆解成了`Ready`就绪状态与`Running`运行状态。<br />以7态为例，来逐步讲解它们之间是如何转换的。
<a name="wHi94"></a>
### 新建状态（New）
可以通过实现`Runnable`接口或继承`Thread`声明一个线程类，`new`一个实例后，线程就进入了新建状态。<br />![2021-08-02-16-37-35-422855.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893515383-dd0fda0b-e043-49c5-9bee-adea6a7c5f02.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u992809d2&originHeight=361&originWidth=611&originalType=binary&ratio=1&size=32347&status=done&style=shadow&taskId=ub30ed095-4829-4d8e-adb6-6394459a72a)<br />一个刚诞生的线程，处于新建状态。
<a name="QdV0A"></a>
### 就绪状态（Ready）
线程对象创建成功后，调用该线程的`start()`函数，线程进入就绪状态，该状态的线程进入可运行线程池中，等待获取C P U的使用权。<br />![2021-08-02-16-37-35-558925.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893525635-fb7cbb76-5539-4557-9dbc-1bf401a82e96.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=uc5ddc3c1&originHeight=426&originWidth=1069&originalType=binary&ratio=1&size=128466&status=done&style=shadow&taskId=u6faaeb9c-f8f9-4688-9922-1eefdcb7cf1)<br />线程表示，已经准备好了，此时是就绪状态，快选我吧~
<a name="kMbqB"></a>
### 运行状态（Running)
此时线程调度程序正在从可运行线程池中选择一个线程，该线程进入运行状态。<br />换句话说，线程获取到了C P U时间片。<br />![2021-08-02-16-37-35-691923.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893535652-53ce9bd0-449f-46ec-82ef-001067ba48d8.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u004a0aa2&originHeight=352&originWidth=1080&originalType=binary&ratio=1&size=133138&status=done&style=shadow&taskId=u704ac585-639a-4a20-82f8-4419af7e9f6)<br />还没完呢，当线程时间片用完或调用的`yield()`函数，该线程回到就绪状态。<br />![2021-08-02-16-37-35-838911.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893545025-590a81d6-f675-469a-8b97-d00378095159.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u4ae599c2&originHeight=402&originWidth=1080&originalType=binary&ratio=1&size=156995&status=done&style=none&taskId=uf8092738-b70f-4b72-834d-b112b2b31fb)<br />作为一名运气好的线程，进入了运行状态，但是运气用完了，还得回到就绪状态。
<a name="FSzvk"></a>
### 终止状态（Terminated）
线程继续运行，直到执行结束或执行过程中因异常意外终止都会使线程进入终止状态。<br />线程一旦终止，就不能复生，这是不可逆的过程。<br />![2021-08-02-16-37-36-003509.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893581001-9c38f2ef-ee8c-47bc-ad34-8dd98f9d8171.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=ud3eceb05&originHeight=318&originWidth=1080&originalType=binary&ratio=1&size=120484&status=done&style=shadow&taskId=ud8753901-cfc4-4bb2-8e01-412b3260e06)<br />线程的人生迎来了终点，可能一帆风顺过完一生，也可能英年早逝令人惋惜。
<a name="xMXMu"></a>
### 等待状态（Waiting）
运行状态的线程执行`wait()`、`join()`、`LockSupport.park()`任意函数，该线程进入等待状态。<br />其中`wait()`与`join()`函数会让J V M把该线程放入锁等待队列。<br />处于这种状态的线程不会被分配C P U执行时间，它们要等待被主动唤醒，否则会一直处于等待状态。<br />![2021-08-02-16-37-36-147470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893594388-bafff251-7a68-49b2-8a80-68945b131df6.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u27b71163&originHeight=659&originWidth=1080&originalType=binary&ratio=1&size=168813&status=done&style=shadow&taskId=udd382608-6f88-471c-be24-6bd2d000165)<br />如果要唤醒线程怎么办呢？<br />执行`LockSupport.unpark(t)`函数唤醒指定线程，该线程回到就绪状态。<br />而通过`notify()`、`notifyAll()`、join线程执行完毕方式，会唤醒锁等待队列的线程，出队的线程回到就绪状态。<br />![2021-08-02-16-37-36-402000.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893608379-747adeb1-57a6-48a6-adef-2c99534d35e6.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u9d07c0ea&originHeight=700&originWidth=1080&originalType=binary&ratio=1&size=189187&status=done&style=shadow&taskId=u04fe9393-41f9-4e77-a115-8e6c1ade246)
<a name="dDTdi"></a>
### 超时等待状态（Timed waiting）
超时等待与等待状态一样，唯一的区别就是多了超时机制，不会一直等待被其他线程主动唤醒，而是到达指定时间后会自动唤醒。<br />以下函数会触发进入超时等待状态

- `wait(long)`
- `join(long)`
- `LockSupport.parkNanos(long)`
- `LockSupport.parkUtil(long)`
- `sleep(long)`

其中`wait(long)`、`join(long)`函数会让J V M把线程放入锁等待队列。<br />![2021-08-02-16-37-36-819438.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893633168-fefd81c9-d729-4ede-970f-b5bb80f76560.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u20e887c6&originHeight=778&originWidth=1080&originalType=binary&ratio=1&size=214766&status=done&style=shadow&taskId=u687d2526-0b5c-4300-9e2f-54c2256ec07)<br />后面的唤醒剧情就和等待状态如出一辙，就多了超时时间到了，自动唤醒的动作。<br />![2021-08-02-16-37-37-317949.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893648946-baa4f4f3-5d02-4210-95d5-f5ab9ee89960.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=ufc4c9f5b&originHeight=802&originWidth=1080&originalType=binary&ratio=1&size=248799&status=done&style=shadow&taskId=u6c2cea94-b501-423f-b071-a64efb8c49f)
<a name="FfoPX"></a>
### 阻塞状态（Blocked）
运行状态的线程获取同步锁失败或发出I/O请求，该线程进入阻塞状态。<br />如果是获取同步锁失败J V M还会把该线程放入锁的同步队列。<br />![2021-08-02-16-37-37-538081.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893660458-9b89674e-0ec9-4244-b819-5c2085b4e79c.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u8c04cd58&originHeight=805&originWidth=1080&originalType=binary&ratio=1&size=301510&status=done&style=shadow&taskId=ua0cfbc76-ce65-4751-a446-37ed3597582)<br />同步锁被释放时，锁的同步队列会出队所有线程，进入就绪状态。<br />I/O处理完毕时，该线程重新回到就绪状态。<br />![2021-08-02-16-37-37-795180.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893673855-ece8c3b6-710f-4634-bd18-932651f5bb67.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=ua20d540b&originHeight=867&originWidth=1080&originalType=binary&ratio=1&size=318744&status=done&style=shadow&taskId=u2054f1a7-9d32-40f4-af6b-a7e49f12493)
<a name="pxNkT"></a>
## 小结
最后放一张简化的线程转换图。<br />![2021-08-02-16-37-37-951441.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627893683226-c8e78e02-2e65-47c1-8868-c093e3d517d7.png#clientId=u3b9ab9f3-19fb-4&from=ui&id=u91c99eff&originHeight=445&originWidth=971&originalType=binary&ratio=1&size=156034&status=done&style=shadow&taskId=u54f2a1a4-0168-474d-bb25-fed3122c48d)
