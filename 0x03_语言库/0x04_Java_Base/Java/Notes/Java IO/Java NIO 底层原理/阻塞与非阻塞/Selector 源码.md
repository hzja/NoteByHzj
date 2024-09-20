深入了解 Selector 的源码，主要讲三个最常用的方法 `open()`，`register()` 和 `selector()`。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1700620038884-b0d3ffd0-bc03-4d2d-b017-981692fa294f.jpeg)
<a name="g1Zpv"></a>
## `open()`
调用 `Selector.open()` 方法会创建一个 Selector 实例。
```java
public static Selector open() throws IOException {
    return SelectorProvider.provider().openSelector();
}
```
`SelectorProvider` 是一个抽象类，它提供了创建 `Selector` 、`ServerSocketChannel`、`SocketChannel` 的方法，采用 Java 的 SPI 方式实现。
```java
public static SelectorProvider provider() {
    synchronized (lock) {
        // 这里保证了只有一个 SelectorProvider
        if (provider != null)
            return provider;
        return AccessController.doPrivileged(
            new PrivilegedAction<SelectorProvider>() {
                public SelectorProvider run() {
                    // 通过配置的 java.nio.channels.spi.SelectorProvider 值注入自定义的SelectorProvider
                    if (loadProviderFromProperty())
                        return provider;
                    // 通过 ServiceLoad 注入，然后获取配置的第一个服务
                    if (loadProviderAsService())
                        return provider;
                    // 默认
                    provider = sun.nio.ch.DefaultSelectorProvider.create();
                    return provider;
                }
            });
    }
}
```
如果没有特殊配置，一般都是使用默认方法 `sun.nio.ch.DefaultSelectorProvider.create()`。平台的不同 `DefaultSelectorProvider` 的实现就不同，这里是 MacBook，实现如下：
```java
public class DefaultSelectorProvider {
    private DefaultSelectorProvider() {
    }

    public static SelectorProvider create() {
        return new KQueueSelectorProvider();
    }
}
```
从这里就可以看出，MacOS 使用的多路复用是 kqueue。对于 Windows 和 Linux 也找到了相对应的实现。
```java
// Windows
public class DefaultSelectorProvider {
    private DefaultSelectorProvider() { }
    public static SelectorProvider create() {
        return new sun.nio.ch.WindowsSelectorProvider();
    }
}
// Linux
public class DefaultSelectorProvider {

    private DefaultSelectorProvider() { }

    @SuppressWarnings("unchecked")
    private static SelectorProvider createProvider(String cn) {
        Class<SelectorProvider> c;
        try {
            c = (Class<SelectorProvider>)Class.forName(cn);
        } catch (ClassNotFoundException x) {
            throw new AssertionError(x);
        }
        try {
            return c.newInstance();
        } catch (IllegalAccessException | InstantiationException x) {
            throw new AssertionError(x);
        }
    }

    public static SelectorProvider create() {
        String osname = AccessController
        .doPrivileged(new GetPropertyAction("os.name"));
        if (osname.equals("SunOS"))
            return createProvider("sun.nio.ch.DevPollSelectorProvider");
        if (osname.equals("Linux"))
            return createProvider("sun.nio.ch.EPollSelectorProvider");
        return new sun.nio.ch.PollSelectorProvider();
    }

}
```
创建完 `SelectorProvider` 后，调用 `openSelector()` 方法就创建了一个 `Selector` 实例，该方法是一个抽象方法，由具体的 `SelectorProvider` 类来实现，如下：
```java
// macos
public class KQueueSelectorProvider extends SelectorProviderImpl {
    public AbstractSelector openSelector() throws IOException {
        return new KQueueSelectorImpl(this);
    }
}

//windows
public class WindowsSelectorProvider extends SelectorProviderImpl {

    public AbstractSelector openSelector() throws IOException {
        return new WindowsSelectorImpl(this);
    }
}

//linux
public class EPollSelectorProvider extends SelectorProviderImpl {
    public AbstractSelector openSelector() throws IOException {
        return new EPollSelectorImpl(this);
    }
    ...
}
```
MacOS 下创建 `KQueueSelectorImpl `，Windows下创建 `WindowsSelectorImpl `， Linux 下创建 `EPollSelectorImpl `。<br />三个系统的 XxxSelectorImpl 都继承 SelectorImpl。在创建实体类时，都会调用 SelectorImpl 的构造方法：
```java
protected SelectorImpl(SelectorProvider sp) {
    super(sp);
    // key 的初始化
    keys = new HashSet<SelectionKey>();
    selectedKeys = new HashSet<SelectionKey>();
    if (Util.atBugLevel("1.4")) {
        publicKeys = keys;
        publicSelectedKeys = selectedKeys;
    } else {
        // 创建一个不可修改的集合
        publicKeys = Collections.unmodifiableSet(keys);
        // 创建一个只能删除不能添加的集合
        publicSelectedKeys = Util.ungrowableSet(selectedKeys);
    }
}
```
关于`Util.atBugLevel`找到一篇文章有提到该方法。似乎是和EPoll的一个空指针异常相关。这个 bug 在 nio bugLevel= 1.4 版本引入，这个 bug 在jdk1.5 中存在，直到 jdk1.7 才修复。<br />在 SelectorImpl 里面会发现有四个 Set 集合
```java
// The set of keys with data ready for an operation
protected Set<SelectionKey> selectedKeys;

// The set of keys registered with this Selector
protected HashSet<SelectionKey> keys;

// Public views of the key sets
private Set<SelectionKey> publicKeys;             // Immutable
private Set<SelectionKey> publicSelectedKeys;     // Removal allowed, but not addition
```

- `selectedKeys`：已经就绪的 key
- `keys`：key 集合
- `publicKeys`：key 集合的代理，不可变
- `publicSelectedKeys`：已经就绪的 key 的代理，可删除，但不可新增

对于 Java 程序员而言，最熟悉的莫过于 Linux 系统，所以以下部分的实现都是基于 `EPollSelectorImpl `。
<a name="LZTXj"></a>
## `register()`
调用 `open()` 方法创建 Selector 后，需要告诉它监控哪些 `Channel`，`SelectableChannel` 提供的 `register()` 方法可以将某个可选择的 `Channel` 注册到 `Selector` 中。方法的实现是在 `AbstractSelectableChannel` 中实现的。
```java
public final SelectionKey register(Selector sel, int ops,Object att)
throws ClosedChannelException
{
    synchronized (regLock) {
        if (!isOpen())
            throw new ClosedChannelException();
        // 是否支持该操作
        if ((ops & ~validOps()) != 0)
            throw new IllegalArgumentException();
        // 阻塞不支持
        if (blocking)
            throw new IllegalBlockingModeException();
        // 根据 Selector 找到 SelectionKey
        SelectionKey k = findKey(sel);
        if (k != null) {
            // 存在
            // 注册相对应的操作
            k.interestOps(ops);
            // 保存参数
            k.attach(att);
        }
        if (k == null) {
            // New registration
            synchronized (keyLock) {
                if (!isOpen())
                    throw new ClosedChannelException();
                // 注册
                k = ((AbstractSelector)sel).register(this, ops, att);
                // 添加到缓存中
                addKey(k);
            }
        }
        return k;
    }
}
```

- 首先会对注册的 Channel 进行校验，这里再次强调，Selector 不支持阻塞模式。
- 调用 `findKey()` 方法找到该 Selector 对应的 `SelectionKey`
- 如果不为空，则将该操作注册上就可以了
- 如果为空，则需要调用 `AbstractSelector.register()` 创建 `SelectionKey` 并注册，然后将其添加到缓存中去。
<a name="j90oB"></a>
### `findKey()`
`findKey()` 用于根据 Selector 查找对应的 `SelectionKey`。
```java
private SelectionKey findKey(Selector sel) {
    synchronized (keyLock) {
        if (keys == null)
            return null;
        for (int i = 0; i < keys.length; i++)
            if ((keys[i] != null) && (keys[i].selector() == sel))
                return keys[i];
        return null;
    }
}
```
逻辑就是从 keys 数组中获取。keys 数组定义如下：
```java
private SelectionKey[] keys = null;
```
该数组用于保存所有已创建的 `SelectionKey`，如果注册的 Channel 已关闭，它对应的 SelectionKey 需要从该数组中取消。
<a name="QULRX"></a>
### `AbstractSelector.register()`
如果从 keys 数组中没有找到对应的 `SelectionKey` ，就需要调用 `AbstractSelector.register()` 方法来创建相对应的 `SelectionKey` 。该方法在 `SelectorImpl` 中实现：
```java
protected final SelectionKey register(AbstractSelectableChannel ch,
                                      int ops,
                                      Object attachment)
{
    if (!(ch instanceof SelChImpl))
        throw new IllegalSelectorException();
    // 新建 SelectionKeyImpl 对象
    SelectionKeyImpl k = new SelectionKeyImpl((SelChImpl)ch, this);
    // 保存参数
    k.attach(attachment);
    // 注册到 Selector
    // 加锁 publicKeys 后面再介绍
    synchronized (publicKeys) {
        implRegister(k);
    }
    // 关注操作
    k.interestOps(ops);
    return k;
}
```
首先创建 `SelectionKeyImpl` 对象，该对象是 `SelectionKey` 的实现，然后调用 `implRegister()` 注册，该方法的实现每个操作系统实现是不一样的，以 Linux 操作胸系统为准。
```java
protected void implRegister(SelectionKeyImpl ski) {
    if (closed)
        throw new ClosedSelectorException();
    SelChImpl ch = ski.channel;
    // 获取 Channel 的句柄
    int fd = Integer.valueOf(ch.getFDVal());
    // 加入到缓存中
    fdToKey.put(fd, ski);
    
    pollWrapper.add(fd);
    // 加入到数组缓存
    keys.add(ski);
}
```
在 `implRegister()` 中，先获取对应 Channel 的文件描述符句柄 fd，然后将文件描述符句柄 fd 和 SelectionKey 保存加入到 fdToKey，fdToKey 是一个 HashMap（`Map<Integer,SelectionKeyImpl>`），它保存文件描述符句柄 fd 和的 SelectionKey 的映射关系，这样 Channel 与对应的 SelectionKey 就映射上了。<br />调用 `pollWrapper.add()`将文件描述符句柄 fd 加入到 `pollWrapper` 中，`pollWrapper` 是`EPollArrayWrapper` 对象， 它是调用底层 Epoll 算法的包装类，这里就不再延伸了。
<a name="fIXyo"></a>
### `addKey()`
获取到 `SelectionKey` 后，调用 `addKey()`方法将该 SelectionKey 添加到 keys 数组中，若空间不够则进行扩容处理（2 倍）。
```java
private void addKey(SelectionKey k) {
    assert Thread.holdsLock(keyLock);
    int i = 0;
    if ((keys != null) && (keyCount < keys.length)) {
        // 找第 i 个为 null 的位置
        for (i = 0; i < keys.length; i++)
            if (keys[i] == null)
                break;
    } else if (keys == null) {
        // 默认 3 
        keys =  new SelectionKey[3];
    } else {
        // 扩容 2 倍
        int n = keys.length * 2;
        SelectionKey[] ks =  new SelectionKey[n];
        for (i = 0; i < keys.length; i++)
            ks[i] = keys[i];
        keys = ks;
        i = keyCount;
    }
    // 保存 key
    keys[i] = k;
    keyCount++;
}
```
<a name="dPF9q"></a>
## `select()`
`select()`有三个重载方法：

- `select()`：阻塞到至少有一个通道在注册的事件上就绪了。
- `select(long timeout)`：阻塞，最长会阻塞 timeout 毫秒(参数)。需要注意的是该方法并不能提供精准的时间保证。关于 timeout 说明如下：
   - 如果 timeout 为正，则 `select(long timeout)` 在等待有通道被选择时至多会阻塞 timeout 毫秒
   - 如果 timeout 为零，则永远阻塞直到有至少一个通道准备就绪
   - timeout不能为负数，会抛出 IllegalArgumentException 异常
- `selectNow()`：不阻塞，立刻返回，如果没有 Channel 准备好，则返回 0。

以 `select()` 方法为入口进去看源码，该方法是一个抽象方法：
```java
public abstract int select() throws IOException;
```
在 SelectorImpl 中实现：
```java
public int select() throws IOException {
    return select(0);
}
```
这里传入的 timeout 为 0，表示一直阻塞到有至少一个通道准备就绪。
```java
public int select(long timeout) throws IOException {
    // timeout 不能为 0，
    if (timeout < 0)
        throw new IllegalArgumentException("Negative timeout");
    return lockAndDoSelect((timeout == 0) ? -1 : timeout);
}
```
调用 `lockAndDoSelect()` 方法：
```java
private int lockAndDoSelect(long timeout) throws IOException {
    synchronized (this) {
        if (!isOpen())
            throw new ClosedSelectorException();
        // 双重锁
        synchronized (publicKeys) {
            synchronized (publicSelectedKeys) {
                return doSelect(timeout);
            }
        }
    }
}
```
最终调用到 `lockAndDoSelect(long timeout)`，其实三个重载方法其实都是委托给 `lockAndDoSelect(long timeout)`，最终调用 `doSelect()` 方法。 `doSelect()` 方法由各个操作系统来实现：
```java
protected int doSelect(long timeout) throws IOException {
    if (closed)
        throw new ClosedSelectorException();
    // 删除取消的key
    processDeregisterQueue();
    try {
        begin();
        // 获取就绪文件描述符
        pollWrapper.poll(timeout);
    } finally {
        end();
    }
    // 再次删除取消的 key
    processDeregisterQueue();
    // 将已就绪的 key 加入到 selectedKeys 中
    int numKeysUpdated = updateSelectedKeys();

    if (pollWrapper.interrupted()) {
        // Clear the wakeup pipe
        pollWrapper.putEventOps(pollWrapper.interruptedIndex(), 0);
        synchronized (interruptLock) {
            pollWrapper.clearInterrupted();
            IOUtil.drain(fd0);
            interruptTriggered = false;
        }
    }
    return numKeysUpdated;
}
```
该方法就三个核心步骤

1. `processDeregisterQueue()`：删除已取消的 key
2. `pollWrapper.poll(timeout)`：获取就绪文件描述符
3. `updateSelectedKeys()`：将已就绪的 key 加入到 selectedKeys 中
<a name="iOwSb"></a>
### `processDeregisterQueue()`
当通道 Channel 关闭后，对应的 key 就需要被取消掉，所有已取消的 key 都会添加到 `cancelledKeys` 中，该操作在 AbstractSelector 中完成。`processDeregisterQueue()` 会遍历 `cancelledKeys` 集合，然后依次删除：
```java
void processDeregisterQueue() throws IOException {
    // Precondition: Synchronized on this, keys, and selectedKeys
    Set<SelectionKey> cks = cancelledKeys();
    synchronized (cks) {
        if (!cks.isEmpty()) {
            Iterator<SelectionKey> i = cks.iterator();
            while (i.hasNext()) {
                SelectionKeyImpl ski = (SelectionKeyImpl)i.next();
                try {
                    implDereg(ski);
                } catch (SocketException se) {
                    throw new IOException("Error deregistering key", se);
                } finally {
                    i.remove();
                }
            }
        }
    }
}
```
就是遍历 `cancelledKeys` 集合，然后调用 `implDereg(ski)` 方法：
```java
protected void implDereg(SelectionKeyImpl ski) throws IOException {
    assert (ski.getIndex() >= 0);
    // channel
    SelChImpl ch = ski.channel;
    // 文件描述符
    int fd = ch.getFDVal();
    // 删除文件描述符和 SelectionKey 的对应关系
    fdToKey.remove(Integer.valueOf(fd));
    // 将句柄值设置为KILLED(-1)
    pollWrapper.remove(fd);
    ski.setIndex(-1);
    // 删除 key 的集合
    keys.remove(ski);
    selectedKeys.remove(ski);
    // 将key设置为无效
    deregister((AbstractSelectionKey)ski);
    SelectableChannel selch = ski.channel();
    if (!selch.isOpen() && !selch.isRegistered())
        ((SelChImpl)selch).kill();
}
```
<a name="MiWDk"></a>
### `pollWrapper.poll(timeout)`
该方法用于获取已经准备就绪的文件描述符。
```java
int poll(long timeout) throws IOException {
    updateRegistrations();
    //获取已就绪的文件句柄
    updated = epollWait(pollArrayAddress, NUM_EPOLLEVENTS, timeout, epfd);
    //如是唤醒文件句柄，则跳过，设置interrupted=true
    for (int i=0; i<updated; i++) {
        if (getDescriptor(i) == incomingInterruptFD) {
            interruptedIndex = i;
            interrupted = true;
            break;
        }
    }
    return updated;
}
```
核心方法是 `epollWait()`，该方法是用于获取已就绪的文件句柄，它是 `native` 方法：
```java
private native int epollWait(long pollAddress, int numfds, long timeout,
                             int epfd) throws IOException;
```
<a name="F8NRC"></a>
### `updateSelectedKeys()`
该方法是将已准备就绪的 key 加入到 selectedKeys 中：
```java
private int updateSelectedKeys() {
    // 
    int entries = pollWrapper.updated;
    int numKeysUpdated = 0;
    for (int i=0; i<entries; i++) {
        int nextFD = pollWrapper.getDescriptor(i);
        // 获取 SelectionKey
        SelectionKeyImpl ski = fdToKey.get(Integer.valueOf(nextFD));
        // ski is null in the case of an interrupt
        if (ski != null) {
            int rOps = pollWrapper.getEventOps(i);
            // 存在
            if (selectedKeys.contains(ski)) {
                if (ski.channel.translateAndSetReadyOps(rOps, ski)) {
                    numKeysUpdated++;
                }
            } else {
                // 不存在，加入
                ski.channel.translateAndSetReadyOps(rOps, ski);
                if ((ski.nioReadyOps() & ski.nioInterestOps()) != 0) {
                    // 加入到 selectedKeys 中
                    selectedKeys.add(ski);
                    numKeysUpdated++;
                }
            }
        }
    }
    return numKeysUpdated;
}
```
