Java Netty
<a name="o062T"></a>
## 前言
数据序列化存储，或者数据通过网络传输时，会遇到不可避免将数据转成字节数组的场景。字节数组的读写不会太难，但又有点繁琐，为了避免重复造轮子，jdk推出了`ByteBuffer`来帮助操作字节数组；而Netty是一款当前流行的java网络IO框架，它内部定义了一个ByteBuf来管理字节数组，和`ByteBuffer`大同小异

- `ByteBuffer`
- 零拷贝之`MappedByteBuffer`
- `DirectByteBuffer`堆外内存回收机制
- netty之ByteBuf
<a name="k4swi"></a>
## Buffer结构
```java
public abstract class Buffer {
    //关系: mark <= position <= limit <= capacity
    private int mark = -1;
    private int position = 0;
    private int limit;
    private int capacity;
    long address; // Used only by direct buffers，直接内存的地址
}
```

- `mark`：调用`mark()`方法的话，`mark`值将存储当前`position`的值，等下次调用`reset()`方法时，会设定`position`的值为之前的标记值
- `position`：是下一个要被读写的byte元素的下标索引
- `limit`：是缓冲区中第一个不能读写的元素的数组下标索引，也可以认为是缓冲区中实际元素的数量
- `capacity`：是缓冲区能够容纳元素的最大数量，这个值在缓冲区创建时被设定，而且不能够改变
<a name="YD1sJ"></a>
## Buffer.API
```java
Buffer(int mark, int pos, int lim, int cap)
//Buffer创建时设置的最大数组容量值
public final int capacity()
//当前指针的位置
public final int position() 
//限制可读写大小
public final Buffer limit(int newLimit)
//标记当前position的位置
public final Buffer mark()
//配合mark使用，position成之前mark()标志的位置。先前没调用mark则报错
public final Buffer reset()
//写->读模式翻转，单向的
//position变成了初值位置0，而limit变成了写模式下position位置
public final Buffer flip()
//重置position指针位置为0，mark为-1；相对flip方法是limit不变
public final Buffer rewind() //复位
//和rewind一样，多出一步是limit会被设置成capacity
public final Buffer clear() 
//返回剩余未读字节数
public final int remaining()
```
<a name="AnVkp"></a>
## ByteBuffer结构
```java
public abstract class ByteBuffer extends Buffer implements Comparable<ByteBuffer>{
    final byte[] hb;  //仅限堆内内存使用
    final int offset;
    boolean isReadOnly; 
}
```
<a name="oN1p2"></a>
## ByteBuffer.API
```java
//申请堆外内存
public static ByteBuffer allocateDirect(int capacity)
//申请堆内内存
public static ByteBuffer allocate(int capacity) 
//原始字节包装成ByteBuffer
public static ByteBuffer wrap(byte[] array, int offset, int length)
//原始字节包装成ByteBuffer
public static ByteBuffer wrap(byte[] array)
//创建共享此缓冲区内容的新字节缓冲区
public abstract ByteBuffer duplicate()
//分片，创建一个新的字节缓冲区
//新ByteBuffer的开始位置是此缓冲区的当前位置position
public abstract ByteBuffer slice()
//获取字节内容
public abstract byte get()
//从ByteBuffer偏移offset的位置，获取length长的字节数组，然后返回当前ByteBuffer对象
public ByteBuffer get(byte[] dst, int offset, int length)
//设置byte内存
public abstract ByteBuffer put(byte b);
//以offset为起始位置设置length长src的内容，并返回当前ByteBuffer对象
public ByteBuffer put(byte[] src, int offset, int length长)
//将没有读完的数据移到到缓冲区的初始位置，position设置为最后一没读字节数据的下个索引，limit重置为为capacity
//读->写模式，相当于flip的反向操作
public abstract ByteBuffer compact()
//是否是直接内存
public abstract boolean isDirect()
```

- `ByteBuffer bf = ByteBuffer.allocate(10);`，创建大小为10的`ByteBuffer`对象

![2021-05-27-17-25-35-180286.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622109117157-416eb018-1532-4367-8911-198bb340037f.png#clientId=u8ec88217-67aa-4&from=ui&id=u60a03f7e&originHeight=301&originWidth=638&originalType=binary&size=577424&status=done&style=shadow&taskId=u4f413485-224d-44e0-a9e5-65d6add62b0)

- 写入数据
```java
ByteBuffer buf ByteBuffer.allocate(10);
buf.put("csc".getBytes());
```
![2021-05-27-17-25-35-312934.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622109123569-788ac534-083e-4ff3-909a-032f8358d88a.png#clientId=u8ec88217-67aa-4&from=ui&id=u71262304&originHeight=294&originWidth=566&originalType=binary&size=500385&status=done&style=shadow&taskId=u32f24be9-97b5-4ccd-8965-57454ea9065)

- 调用flip转换缓冲区为读模式; `buf.flip();`

![2021-05-27-17-25-35-445577.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622109129242-c85b902c-7db3-4c51-af4b-3f7626cedf13.png#clientId=u8ec88217-67aa-4&from=ui&id=u9c897fc1&originHeight=306&originWidth=610&originalType=binary&size=561276&status=done&style=shadow&taskId=ue2e2d854-8b4c-4547-885b-c597f47dc0a)

- 读取缓冲区中到内容：`get();` `System.out.println((char) buf.get());`

![2021-05-27-17-25-35-611133.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622109106469-656d8b18-5869-49d4-aef4-b7b8fb57e194.png#clientId=u8ec88217-67aa-4&from=ui&id=u71c9ee50&originHeight=482&originWidth=1080&originalType=binary&size=1564761&status=done&style=none&taskId=u8fd80873-76d2-4eea-bc55-7b17ec9a6d2)![2021-05-27-17-25-35-754749.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622109105926-ecffb936-3256-44fe-ae93-0d0da098e316.png#clientId=u8ec88217-67aa-4&from=ui&id=u7c12a41c&originHeight=293&originWidth=568&originalType=binary&size=500444&status=done&style=shadow&taskId=u0a582f7a-2942-42a5-bece-7456e7b73cb)
<a name="oxo0F"></a>
## 零拷贝之MappedByteBuffer

- 共享内存映射文件，对应的`ByteBuffer`子操作类，`MappedByteBuffer`是基于`mmap`实现的。`MappedByteBuffer`需要`FileChannel`调用本地map函数映射。C++代码可以查阅下FileChannelImpl.c-Java_sun_nio_ch_FileChannelImpl_map0方法
- 使用MappedByteBuffer和文件映射，其读写可以减少内存拷贝次数
```java
FileChannel readChannel = FileChannel.open(Paths.get("./cscw.txt"), StandardOpenOption.READ);
MappedByteBuffer data = readChannel.map(FileChannel.MapMode.READ_ONLY, 0, 1024 * 1024 * 40);
```
<a name="gbhMa"></a>
## `DirectByteBuffer`堆外内存回收机制Cleaner
下面看看直接内存的回收机制（java8）；`DirectByteBuffer`内部存在一个`Cleaner`对象，并且委托内部类`Deallocator`对象进行内存回收
```java
class DirectByteBuffer extends MappedByteBuffer implements DirectBuffer
{
 //构造函数
    DirectByteBuffer(int cap) { 
  .... //内存分配
        cleaner = Cleaner.create(this, new Deallocator(base, size, cap));
     ...
    }    
    private static class Deallocator implements Runnable{
     ...
     public void run() {
            if (address == 0) {
                // Paranoia
                return;
            }
            unsafe.freeMemory(address); //回收内存
            address = 0;
            Bits.unreserveMemory(size, capacity);
        }
}
```
细看下`Cleaner`，继承于`PhantomReference`，并且在`public void clean()`方法会调用`Deallocator`进行清除操作
```java
public class Cleaner extends PhantomReference<Object> {
    //如果DirectByteBuffer对象被回收，相应的Cleaner会被放入dummyQueue队列
    private static final ReferenceQueue<Object> dummyQueue = new ReferenceQueue();
    //构造函数
    public static Cleaner create(Object var0, Runnable var1) {
        return var1 == null ? null : add(new Cleaner(var0, var1));
    }
    private Cleaner(Object var1, Runnable var2) {
        super(var1, dummyQueue);
        this.thunk = var2;
    }
    private final Runnable thunk;
    public void clean() {
            if (remove(this)) {
                try {
                    this.thunk.run();
                } catch (final Throwable var2) {
                ....
```
在`Reference`内部存在一个守护线程，循环获取`Reference`，并判断是否`Cleaner`对象，如果是则调用其`clean`方法
```java
public abstract class Reference<T> 
    static {
        ThreadGroup tg = Thread.currentThread().getThreadGroup();
        for (ThreadGroup tgn = tg; tgn != null; g = tgn, tgn = tg.getParent());
        Thread handler = new ReferenceHandler(tg, "Reference Handler");
        ...
        handler.setDaemon(true);
        handler.start();
        ...
    }
 ...
    //内部类调用 tryHandlePending
    private static class ReferenceHandler extends Thread {
        public void run() {
                    while (true) {
                        tryHandlePending(true);
                    }
                }
   ... 
    static boolean tryHandlePending(boolean waitForNotify) {
        Cleaner c;
        .... //从链表获取对象被回收的引用
        // 判断Reference是否Cleaner，如果是则调用其clean方法
        if (c != null) {
            c.clean(); //调用Cleaner的clean方法
            return true;
        }
        ReferenceQueue<? super Object> q = r.queue;
        if (q != ReferenceQueue.NULL) q.enqueue(r);
        return true;

```
<a name="ifzL3"></a>
## Netty之`ByteBuf`
<a name="ky6Ls"></a>
### `ByteBuf`原理

- `Bytebuf`通过两个位置指针来协助缓冲区的读写操作，分别是`readIndex`和`writerIndex`
```java
 *      +-------------------+------------------+------------------+
 *      | discardable bytes |  readable bytes  |  writable bytes  |
 *      |                   |     (CONTENT)    |                  |
 *      +-------------------+------------------+------------------+
 *      |                   |                  |                  |
 *      0 <= readerIndex <= writerIndex <= capacity
```
<a name="JR6Fb"></a>
### ByteBuf.API
```java
//获取ByteBuf分配器
public abstract ByteBufAllocator alloc()
//丢弃可读字节
public abstract ByteBuf discardReadBytes()
//返回读指针
public abstract int readerIndex()
//设置读指针
public abstract ByteBuf readerIndex(int readerIndex);
//标志当前读指针位置，配合resetReaderIndex使用
public abstract ByteBuf markReaderIndex()
public abstract ByteBuf resetReaderIndex()
//返回可读字节数
public abstract int readableBytes()
//返回写指针
public abstract int writerIndex()
//设置写指针
public abstract ByteBuf writerIndex(int writerIndex);
//标志当前写指针位置，配合resetWriterIndex使用
public abstract ByteBuf markWriterIndex()
public abstract ByteBuf resetWriterIndex()
//返回可写字节数
public abstract int writableBytes()
public abstract ByteBuf clear();
//设置读写指针
public abstract ByteBuf setIndex(int readerIndex, int writerIndex)
//指针跳过length
public abstract ByteBuf skipBytes(int length)
//以当前位置切分ByteBuf todo
public abstract ByteBuf slice();
//切割起始位置为index，长度为length的ByteBuf todo
public abstract ByteBuf slice(int index, int length);
//Returns a copy of this buffer's readable bytes. //复制ByteBuf todo
public abstract ByteBuf copy()
//是否可读
public abstract boolean isReadable()
//是否可写
public abstract boolean isWritable()
//字节编码顺序
public abstract ByteOrder order()
//是否在直接内存申请的ByteBuf
public abstract boolean isDirect()
//转为jdk.NIO的ByteBuffer类
public abstract ByteBuffer nioBuffer()
```
<a name="uPqbd"></a>
### 使用示例
```java
public static void main(String[] args) {
    //分配大小为10的内存
    ByteBuf buf = Unpooled.buffer(10);
    //写入
    buf.writeBytes("csc".getBytes());
    //读取
    byte[] b =  new byte[3];
    buf.readBytes(b);
    System.out.println(new String(b));
    System.out.println(buf.writerIndex());
    System.out.println(buf.readerIndex());
}
```
－－－－result－－－－
```java
csc
3
3
```

- `ByteBuf`初始化时，`readIndex`和`writerIndex`等于`0`，调用`writeXXX()`方法写入数据，`writerIndex`会增加(`setXXX`方法无作用)；调用`readXXX()`方法读取数据，则会使`readIndex`增加(`getXXX`方法无作用)，但不会超过`writerIndex`
- 在读取数据之后，0-`readIndex`之间的byte数据被视为`discard`，调用`discardReadBytes()`，释放这部分空间，作用类似于`ByteBuffer`的`compact`方法
