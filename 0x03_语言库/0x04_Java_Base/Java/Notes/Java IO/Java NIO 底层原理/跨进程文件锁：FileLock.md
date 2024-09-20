Java<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582269601-611c184c-6651-4b76-a85e-9285f1ab858e.png#clientId=u5945ab88-375f-4&from=paste&id=u9ab880ce&originHeight=199&originWidth=639&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u958b4243-ae2b-4ca8-a1d2-c8847075784&title=)<br />一个场景有多个进程同时操作某一个文件，并行往文件中写数据，请问如何保证写入文件的内容是正确的？可能有人说加分布式锁，可以解决问题，但是有点儿重了。<br />有没有更加轻量级的方案呢？**多进程文件锁：FileLock**。
<a name="tbRjW"></a>
## FileLock
FileLock是文件锁，它能保证同一时间只有一个进程（程序）能够修改它，或者都只可以读，这样就解决了多进程间的同步文件，保证了安全性。但是需要注意的是，它进程级别的，不是线程级别的，他可以解决多个进程并发访问同一个文件的问题，但是它不适用于控制同一个进程中多个线程对一个文件的访问。这也是为什么它叫做 **多进程文件锁**，而不是 **多线程文件锁**。<br />FileLock一般都是从FileChannel 中获取，FileChannel 提供了三个方法用以获取 FileLock。
```java
public abstract FileLock lock(long position, long size, boolean shared) throws IOException;

public final FileLock lock() throws IOException;

public abstract FileLock tryLock(long position, long size, boolean shared) throws IOException;

public final FileLock tryLock() throws IOException；
```

- **lock()** 是阻塞式的，它要阻塞进程直到锁可以获得，或调用`lock()`的线程中断，或调用`lock()`的通道关闭。
- `tryLock()`是非阻塞式的，它设法获取锁，但如果不能获得，例如因为其他一些进程已经持有相同的锁，而且不共享时，它将直接从方法调用返回。

`lock()`和`tryLock()`方法有三个参数，如下：

- `position`：锁定文件中的开始位置
- `size`：锁定文件中的内容长度
- `shared`：是否使用共享锁。true为共享锁；false为独占锁。
<a name="tCvt1"></a>
### 示例
<a name="jygqW"></a>
#### 不使用文件锁来读写文件
首先不使用文件锁来进行多进程间文件读写，进程1往文件中写数据，进程2读取文件的大小。

- 进程1
```java
RandomAccessFile randomAccessFile = new RandomAccessFile("/Users/chenssy/Downloads/filelock.txt","rw");
FileChannel fileChannel = randomAccessFile.getChannel();
// 这里是独占锁
//FileLock fileLock = fileChannel.lock();
System.out.println("进程 1 开始写内容：" + LocalTime.now());
for(int i = 1 ; i <= 10 ; i++) {
    randomAccessFile.writeChars("chenssy_" + i);
    // 等待两秒
    TimeUnit.SECONDS.sleep(2);
}
System.out.println("进程 1 完成写内容：" + LocalTime.now());
// 完成后要释放掉锁
//fileLock.release();
fileChannel.close();
randomAccessFile.close();
```

- 进程2
```java
RandomAccessFile randomAccessFile = new RandomAccessFile("/Users/chenssy/Downloads/filelock.txt","rw");
FileChannel fileChannel = randomAccessFile.getChannel();
// 这里是独占锁
//FileLock fileLock = fileChannel.lock();
System.out.println("开始读文件的时间：" + LocalTime.now());

for(int i = 0 ; i < 10 ; i++) {
    // 这里直接读文件的大小
    System.out.println("文件大小为：" + randomAccessFile.length());
    // 这里等待 1 秒
    TimeUnit.SECONDS.sleep(1);
}

System.out.println("结束读文件的时间：" + LocalTime.now());
// 完成后要释放掉锁
//fileLock.release();
fileChannel.close();
randomAccessFile.close();
```
运行结果

- 进程1

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582269540-b10ad2ae-af4a-43bc-b493-dca9eff5d2e4.png#clientId=u5945ab88-375f-4&from=paste&id=ub58763ee&originHeight=213&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue11e3266-b297-4c37-8a43-65381c4bd6f&title=)

- 进程2

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582269591-929190b9-4d8e-48b0-b810-5c844034a8f7.png#clientId=u5945ab88-375f-4&from=paste&id=u43b591ea&originHeight=436&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua927dcc4-b15f-4b48-a3a1-3f417a71e84&title=)<br />从这个结果可以非常清晰看到，进程1和进程2是同时执行的。进程1一边往文件中写，进程2是一边在读的
<a name="VdQ4V"></a>
#### 使用文件锁读写文件
这里使用文件锁来进行多进程间文件读写，依然使用上面的程序，只需要将对应的注释放开即可。执行结果

- 进程1

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582269522-93686ab1-2a72-4156-9bfa-a790fef75fd8.png#clientId=u5945ab88-375f-4&from=paste&id=uddd1cb5e&originHeight=164&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u421e34e3-41e1-49da-9800-80563c3146d&title=)

- 进程2

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582269535-6d9e6d1e-3027-455e-86e7-643e0e4744f4.png#clientId=u5945ab88-375f-4&from=paste&id=ue247ff05&originHeight=440&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2cc62ebc-cdf3-406d-9ddf-bef6ae002f9&title=)<br />从这里可以看到，进程2是等进程1释放掉锁后才开始执行的。同时由于进程1已经将数据全部写入文件了，所以进程2读取文件的大小是一样的。从这里可以看出 **FileLock确实是可以解决多进程访问同一个文件的并发安全问题。**
<a name="dUpOA"></a>
#### 同进程不同线程进行文件读写
在开始就说到，FileLock是不适用同一进程不同线程之间文件的访问。因为根本无法在一个进程中不同线程同时对一个文件进行加锁操作，如果线程1对文件进行了加锁操作，这时线程2也来进行加锁操作的话，则会直接抛出异常：`java.nio.channels.OverlappingFileLockException`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582269910-9be51003-7039-481a-a208-a4bcdeb93713.png#clientId=u5945ab88-375f-4&from=paste&id=u9bc708f8&originHeight=331&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud649a8ec-bed4-4f06-8227-c17f2ebbaef&title=)<br />当然可以通过另外一种方式来规避，如下：
```java
FileLock fileLock;
while (true){
    try{
        fileLock = fileChannel.tryLock();
        break;
    } catch (Exception e) {
        System.out.println("其他线程已经获取该文件锁了，当前线程休眠 2 秒再获取");
        TimeUnit.SECONDS.sleep(2);
    }
}
```
将上面获取锁的部分用这段代码替换，执行结果又如下两种：

- 线程1先获取文件锁

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582270018-6f396e6f-41b1-4d6e-901e-57e8710113b4.png#clientId=u5945ab88-375f-4&from=paste&id=u57148c1c&originHeight=773&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u98444563-ea6a-49bc-b954-5064db4e8ca&title=)

- 线程2先获取文件锁

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647582270099-d1ad60db-e306-493f-92c1-6aaba0be57b3.png#clientId=u5945ab88-375f-4&from=paste&id=ue525aa37&originHeight=633&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf603c2d7-ecb2-4f0d-a674-456e9de4775&title=)<br />这种方式虽然也可以实现多线程访问同一个文件，但是不建议这样操作！！！
<a name="U3rbG"></a>
### 源码分析
下面以 `FileLock lock(long position, long size, boolean shared)`为例简单分析下文件锁的源码。`lock()`方法是由`FileChannel`的子类 `FileChannelImpl`来实现的。
```java
public FileLock lock(long position, long size, boolean shared) throws IOException {
	// 确认文件已经打开 , 即判断open标识位
	ensureOpen();
	if (shared && !readable)
		thrownew NonReadableChannelException();
	if (!shared && !writable)
		thrownew NonWritableChannelException();
	// 创建 FileLock 对象
	FileLockImpl fli = new FileLockImpl(this, position, size, shared);
	// 创建 FileLockTable 对象
	FileLockTable flt = fileLockTable();
	flt.add(fli);
	boolean completed = false;
	int ti = -1;
	try {
		// 标记开始IO操作 , 可能会导致阻塞
		begin();
		ti = threads.add();
		if (!isOpen())
			returnnull;
		int n;
		do {
			// 开始锁住文件
			n = nd.lock(fd, true, position, size, shared);
		} while ((n == FileDispatcher.INTERRUPTED) && isOpen());
		if (isOpen()) {
			// 如果返回结果为RET_EX_LOCK的话
			if (n == FileDispatcher.RET_EX_LOCK) {
				assert shared;
				FileLockImpl fli2 = new FileLockImpl(this, position, size,
													 false);
				flt.replace(fli, fli2);
				fli = fli2;
			}
			completed = true;
		}
	} finally {
		// 释放锁
		if (!completed)
			flt.remove(fli);
		threads.remove(ti);
		try {
			end(completed);
		} catch (ClosedByInterruptException e) {
			thrownew FileLockInterruptionException();
		}
	}
	return fli;
}
```
首先会判断文件是否已打开，然后创建FileLock和FileLockTable 对象，其中`FileLockTable`是用于存放 `FileLock`的table。

- 调用 `begin()`设置中断触发
```java
protected final void begin() {
    if (interruptor == null) {
        interruptor = new Interruptible() {
                public void interrupt(Thread target) {
                    synchronized (closeLock) {
                        if (!open)
                            return;
                        open = false;
                        interrupted = target;
                        try {
                            AbstractInterruptibleChannel.this.implCloseChannel();
                        } catch (IOException x) { }
                    }
                }};
    }
    blockedOn(interruptor);
    Thread me = Thread.currentThread();
    if (me.isInterrupted())
        interruptor.interrupt(me);
}
```

- 调用 `FileDispatcher.lock()`开始锁住文件
```java
int lock(FileDescriptor fd, boolean blocking, long pos, long size,
             boolean shared) throws IOException
    {
        BlockGuard.getThreadPolicy().onWriteToDisk();
        return lock0(fd, blocking, pos, size, shared);
    }

lock0()的实现是在 FileDispatcherImpl.c 中，源码如下：
JNIEXPORT jint JNICALL
FileDispatcherImpl_lock0(JNIEnv *env, jobject this, jobject fdo,
                                      jboolean block, jlong pos, jlong size,
                                      jboolean shared)
{
    // 通过fdval函数找到fd
    jint fd = fdval(env, fdo);
    jint lockResult = 0;
    int cmd = 0;
    // 创建flock对象
    struct flock64 fl;

    fl.l_whence = SEEK_SET;
    // 从position位置开始
    if (size == (jlong)java_lang_Long_MAX_VALUE) {
        fl.l_len = (off64_t)0;
    } else {
        fl.l_len = (off64_t)size;
    }
    fl.l_start = (off64_t)pos;
    // 如果是共享锁 , 则只读
    if (shared == JNI_TRUE) {
        fl.l_type = F_RDLCK;
    } else {
        // 否则可读写
        fl.l_type = F_WRLCK;
    }
    // 设置锁参数
    // F_SETLK : 给当前文件上锁（非阻塞）。
    // F_SETLKW : 给当前文件上锁（阻塞，若当前文件正在被锁住，该函数一直阻塞）。
    if (block == JNI_TRUE) {
        cmd = F_SETLKW64;
    } else {
        cmd = F_SETLK64;
    }
    // 调用fcntl锁住文件
    lockResult = fcntl(fd, cmd, &fl);
    if (lockResult < 0) {
        if ((cmd == F_SETLK64) && (errno == EAGAIN || errno == EACCES))
            // 如果出现错误 , 返回错误码
            return sun_nio_ch_FileDispatcherImpl_NO_LOCK;
        if (errno == EINTR)
            return sun_nio_ch_FileDispatcherImpl_INTERRUPTED;
        JNU_ThrowIOExceptionWithLastError(env, "Lock failed");
    }
    return0;
}
```
所以，其实文件锁的核心就是调用Linux的`fnctl`来从内核对文件进行加锁。
