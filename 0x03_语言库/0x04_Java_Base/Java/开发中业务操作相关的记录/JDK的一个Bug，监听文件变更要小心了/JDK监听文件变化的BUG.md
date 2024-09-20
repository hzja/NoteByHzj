Java
<a name="vLh2R"></a>
### 从一次故障说起
还是从故障说起，这样更加贴近实际，也能让大家更快速理解背景。<br />有一个下发配置的服务，这个配置服务的实现有点特殊，服务端下发配置到各个服务的本地文件，当然中间经过了一个agent，如果没有agent也就无法写本地文件，然后由client端的程序监听这个配置文件，一旦文件有变更，就重新加载配置，画个架构图大概是这样：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749220-43bf2050-a2e6-4c40-9f1c-402668a09031.png#clientId=uf4d13edb-f163-4&from=paste&id=uc98261fd&originHeight=671&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1686aec4-858e-4732-abb3-6cc5ddadbaf&title=)<br />重点是文件的变更该如何监听（watch），当时的实现非常简单：

- 单独起个线程，定时去获取文件的最后更新时间戳（毫秒级）
- 记录每个文件的最后更新时间戳，根据这个时间戳是否变化来判断文件是否有变更

从上述简单的描述，能看出这样实现有一些缺点：

- 无法实时感知文件的变更，感知误差在于轮询文件最后更新时间的间隔
- 精确到毫秒级，如果同一毫秒内发生2次变更，且轮询时刚好落在这2次变更的中间时，后一次变更将无法感知，但这概率很小

还好，上述两个缺点几乎没有什么大的影响。<br />但后来还是发生了一次比较严重的线上故障，这是为什么呢？因为一个JDK的BUG，这里直接贴出罪魁祸首：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749196-bf5aa06d-f7c4-40fa-91f6-7f022c6aa56d.png#clientId=uf4d13edb-f163-4&from=paste&id=u07af3094&originHeight=625&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u232704fb-45d2-41cd-a337-8716b147397&title=)<br />BUG详见：[https://bugs.java.com/bugdatabase/view_bug.do?bug_id=8177809](https://bugs.java.com/bugdatabase/view_bug.do?bug_id=8177809)<br />在某些JDK版本下，获取文件的最后更新时间戳会丢失毫秒精度，总是返回整秒的时间戳，为了直观感受，写了个demo分别在**jdk1.8.0_261**和**jdk_11.0.6**测试（均为MacOs）：

- jdk_1.8.0_261

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749268-bdddf071-01d0-44f7-9723-e493be74623d.png#clientId=uf4d13edb-f163-4&from=paste&id=u6cb7b8cb&originHeight=861&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4da531ec-5697-4e55-8bc5-ddb6c422aae&title=)

- jdk_11.0.6

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749282-0fb0f7d3-4f0b-4bd4-9a89-afc5d1f5650f.png#clientId=uf4d13edb-f163-4&from=paste&id=uabf29e3f&originHeight=908&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u351059cf-021b-4512-b958-9a405f0ab5c&title=)<br />如果是在这个BUG的影响下，只要同一秒内有2次变更，且读取文件最后时间戳位于这2次变更之间的时间，第2次变更就无法被程序感知了，同1秒这个概率比同一毫秒大的多的多，所以当然就被触发了，导致了一次线上故障。<br />这就好比之前是沧海一粟，现在变成了大海里摸到某条鱼的概率。这也能碰到，真是有点极限~
<a name="NywJJ"></a>
### WatchService—JDK内置的文件变更监听
当了解到之前的实现存在BUG后，就去搜了一下Java下如何监听文件变更，果然找到了`**WatchService**`。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749375-0b69e3e8-f380-4015-ae8a-98c80ba8b5fb.png#clientId=uf4d13edb-f163-4&from=paste&id=u615ca8f5&originHeight=440&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04b70fad-fc37-4e91-b390-8d35e0ffd8a&title=)<br />说是WatchService可以监听一个目录，对目录下的文件新增、变更、删除进行监听。于是很快就写了个demo进行测试：
```java
public static void watchDir(String dir) {
    Path path = Paths.get(dir);
    try (WatchService watchService = FileSystems.getDefault().newWatchService()) {
        path.register(watchService, StandardWatchEventKinds.ENTRY_CREATE, StandardWatchEventKinds.ENTRY_MODIFY, StandardWatchEventKinds.ENTRY_DELETE, StandardWatchEventKinds.OVERFLOW);
        while (true) {
            WatchKey key = watchService.take();
            for (WatchEvent<?> watchEvent : key.pollEvents()) {
                if (watchEvent.kind() == StandardWatchEventKinds.ENTRY_CREATE) {
                    System.out.println("create..." + System.currentTimeMillis());
                } else if (watchEvent.kind() == StandardWatchEventKinds.ENTRY_MODIFY) {
                    System.out.println("modify..." + System.currentTimeMillis());
                } else if (watchEvent.kind() == StandardWatchEventKinds.ENTRY_DELETE) {
                    System.out.println("delete..." + System.currentTimeMillis());
                } else if (watchEvent.kind() == StandardWatchEventKinds.OVERFLOW) {
                    System.out.println("overflow..." + System.currentTimeMillis());
                }
            }
            if (!key.reset()) {
                System.out.println("reset false");
                return;
            }
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749646-5483ae19-b9ba-4e11-8caa-6b948e8a5554.png#clientId=uf4d13edb-f163-4&from=paste&id=u50be67e5&originHeight=633&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf0ea6b66-a4b1-4801-8fdc-cdda4296257&title=)<br />先对**/tmp/file_test**目录进行监听，然后每隔5毫秒往文件写数据，理论上来说，应该能收到3次事件，但实际上很奇怪，仔细看接收到**modify**事件的时间大概是第一次文件修改后的**9.5s**左右，很奇怪，先记着，读一下WatchService源码
```java
>>> 1652076266609 - 1652076257097
9512
```
<a name="Ocp3G"></a>
### WatchService原理
```java
WatchService watchService = FileSystems.getDefault().newWatchService()
```
通过debug发现，这里的`watchService`实际上是`PollingWatchService`的实例，直接看`PollingWatchService`的实现：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749922-1276dcb8-08fd-4300-ba80-0c02ad9f2437.png#clientId=uf4d13edb-f163-4&from=paste&id=u6ac4cd49&originHeight=237&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7880425e-42f6-47c5-ad89-0b6d678684b&title=)<br />`PollingWatchService`上来就起了个线程。再找一下这个`scheduledExecutor`在哪里用到：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998749942-ecae0829-4dbe-4707-a1d1-aa691cc5ae47.png#clientId=uf4d13edb-f163-4&from=paste&id=uf0504ff6&originHeight=255&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u54e0ba13-be18-4baf-a694-184b4d8b283&title=)<br />每隔一段时间（默认为10s）去poll下，这个poll干了什么？代码太长，截出关键部分：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998750439-3da13f3c-0e3d-4ef3-a355-5fc92db0b577.png#clientId=uf4d13edb-f163-4&from=paste&id=u38bd3061&originHeight=713&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3b60c894-9b28-4575-bfba-992e4db00b0&title=)<br />果然，也是去读文件的最后更新时间，根据时间的变化来发出变更事件。<br />换句话说，在某些JDK版本下，他也是有BUG的！<br />这也就解释了上文提到的事件监听为什么是在第一个9.5s之后才发出，因为监听注册后，sleep了500ms后修改文件，10s轮询，刚好9.5s后拿到第一轮事件。
<a name="i9J59"></a>
### `inotify`—Linux内核提供的文件监听机制
至此，想起了linux上的**tail**命令，tail 是在文件有变更的情况下输出文件的末尾，理论上也是监听了文件变更，这块刚好在很久之前听过一个技术大佬分享如何自己实现**tail**命令，用到的底层技术就是**inotify**![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998750334-d4ab2ee7-38a1-47ce-946f-850a465bcd7a.png#clientId=uf4d13edb-f163-4&from=paste&id=u554ac225&originHeight=458&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u335c2bee-346c-4c5c-8caf-92095b1e9c3&title=)<br />简单来说，inotify是linux内核提供的一种监控文件变更事件的系统调用。如果基于此来实现，不就可以规避JDK的BUG了吗？<br />但奇怪的是为什么Java没有用这个来实现呢？于是又搜了搜，发现谷歌似乎有一个库，但被删了，看不到代码:![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998750351-f9dc65ee-46ef-4dd0-a329-01ed527ddd28.png#clientId=uf4d13edb-f163-4&from=paste&id=ucb890fb5&originHeight=606&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u290849a1-7734-48a8-9999-d332479b688&title=)<br />github上又搜到一个：[https://github.com/sunmingshi/Jinotify](https://github.com/sunmingshi/Jinotify)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998750392-ad4209ca-1ba5-4fa5-8380-eb93d5e71dde.png#clientId=uf4d13edb-f163-4&from=paste&id=ue3c0eae0&originHeight=791&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u11732a88-4966-4d53-9cc1-ba08aa3be4c&title=)<br />看起来是一个`native`的实现，需要自己编译.so文件，这样就比较蛋疼了。<br />记得上次这么蛋疼还是在折腾Java的unix domain socket，也是找到了一个google的库，测试没问题，放到线上就崩了~不得不说google还是厉害，JDK提供不了的库，自己来提供~<br />于是带着这个疑问去问了一个搞JVM开发的朋友，结果他说，Java也可以使用inotify！难道是测试的姿势不对？<br />又去翻了一遍Java文档，发现在角落隐藏了这么一段话：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998750963-47d7f129-116d-4bee-8e99-2df3652a362c.png#clientId=uf4d13edb-f163-4&from=paste&id=ub9f1b7cd&originHeight=156&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6fc0940a-6637-4aa0-ad78-58a4960fcff&title=)<br />也就是说，不同的平台下会使用不同的实现，`PollingWatchService`是在系统不支持`inotify`的情况下使用的兜底策略。<br />于是将`watchService`的类型打印出来，在Mac上打印为：
```java
class sun.nio.fs.PollingWatchService
```
在Linux上是：
```java
class sun.nio.fs.LinuxWatchService
```
`LinuxWatchService`在Mac上是找不到这个类，猜测应该是Mac版的JDK压根没把这块代码打包进来。<br />原来本地测试都走了兜底策略，看来是测了个寂寞。<br />于是写了个demo再测试一把：
```java
public static void main(String[] args) throws Exception {
    Thread thread = new Thread(() -> watchDir("/tmp/file_test"));
    thread.setDaemon(false);
    thread.start();

    Thread.sleep(500L);

    for (int i = 0; i < 3; i++) {
        String path = "/tmp/file_test/test";
        FileWriter fileWriter = new FileWriter(path);
        fileWriter.write(i);
        fileWriter.close();
        File file = new File(path);
        System.out.println(file.lastModified());
        Thread.sleep(5);
    }
}
```

- 本地Mac

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998751152-c24638e1-1e89-444d-89aa-8f9ab37e7cad.png#clientId=uf4d13edb-f163-4&from=paste&id=u88863e0d&originHeight=255&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46449cf9-d947-4c78-89d7-9d4be3e511d&title=)

- Linux

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998751366-84be2b85-2704-4c4e-add3-12a8b1d75b0f.png#clientId=uf4d13edb-f163-4&from=paste&id=uf5b3a0e5&originHeight=332&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uce109c94-e336-4100-a213-20e27de529e&title=)<br />可以看出，Linux上能收到的事件比本地多的多，而且接收事件的时间明显实时多了。<br />为了更加准确的验证是inotify，用**strace**抓一下系统调用，由于JVM fork出的子进程较多，所以要加`**-f**`命令，输出太多可以存入文件再分析：
```bash
strace -f -o s.txt java FileTime
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661998751425-ac3a638a-ef00-436d-b9e2-71aefe2028ce.png#clientId=uf4d13edb-f163-4&from=paste&id=u3ef37f52&originHeight=348&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u72905850-b7d0-4d68-96b0-26bcd14b082&title=)<br />果然是用到了inotify系统调用的，再次验证了猜想。
<a name="HPnCl"></a>
### 故障是如何修复的？
再次回到开头的故障，是如何修复的呢？由于下发的文件和读取文件的程序都是可控的，所以绕过了这个BUG，给每个文件写一个**version**，可以用文件内容md5值作为version，写入一个特殊文件，读取时先读version，当version变化时再重新载入文件。<br />可能要问了，为什么不用`WatchService`呢？<br />据说inotify在docker上运行的不是很好，经常会丢失事件，不是Java的问题，所有语言都存在这个问题，所以一直没有使用。不过这块找不到相关的资料，也无法证明，所以暂时搁置。
