JavaUnsafe<br />几乎每个使用 Java开发的工具、软件基础设施、高性能开发库都在底层使用了sun.misc.Unsafe，比如**Netty**、**Cassandra**、**Hadoop**、**Kafka** 等。<br />Unsafe类在提升Java运行效率，增强Java语言底层操作能力方面起了很大的作用。但Unsafe类在sun.misc包下，不属于Java标准。<br />很早之前，在阅读并发编程相关类的源码时，看到Unsafe类，产生了一个疑惑：既然是并发编程中用到的类，为什么命名为Unsafe呢？<br />深入了解之后才知道，这里的`Unsafe`并不是说线程安全与否，而是指：该类对于普通的程序员来说是”危险“的，一般应用开发者不会也不应该用到此类。<br />因为Unsafe类功能过于强大，提供了一些可以绕开JVM的更底层功能。它让Java拥有了像C语言的指针一样操作内存空间的能力，能够提升效率，但也带来了指针的问题。官方并不建议使用，也没提供文档支持，甚至计划在高版本中去掉该类。<br />但对于开发者来说，了解该类提供的功能更有助于学习CAS、并发编程等相关的知识，还是非常有必要学习和了解的。
<a name="n9sUm"></a>
## `Unsafe`的构造
`Unsafe`类是"final"的，不允许继承，且构造函数是`private`，使用了单例模式来通过一个静态方法`getUnsafe()`来获取。
```java
private Unsafe() {
}

@CallerSensitive
public static Unsafe getUnsafe() {
	Class var0 = Reflection.getCallerClass();
	if (!VM.isSystemDomainLoader(var0.getClassLoader())) {
		throw new SecurityException("Unsafe");
	} else {
		return theUnsafe;
	}
}
```
在`getUnsafe`方法中对单例模式中的对象创建做了限制，如果是普通的调用会抛出一个`SecurityException`异常。只有由主类加载器加载的类才能调用这个方法。<br />那么，如何获得`Unsafe`类的对象呢？通常采用反射机制：
```java
public static Unsafe getUnsafe() throws IllegalAccessException {
	Field unsafeField = Unsafe.class.getDeclaredFields()[0];
	unsafeField.setAccessible(true);
	return (Unsafe) unsafeField.get(null);
}
```
当获得Unsafe对象之后，就可以”为所欲为“了。下面就来看看，通过`Unsafe`方法，可以做些什么。
<a name="S6RIP"></a>
## Unsafe的主要功能
可先从根据下图从整体上了解一下Unsafe提供的功能：<br />![Unsafe功能概述](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654046523401-0fa343c7-e003-47f4-90ad-364167217f4b.jpeg#clientId=uaf2d38c1-b43b-4&from=paste&id=u476c9832&originHeight=375&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0fcdf8f9-9902-4fee-9754-9ea76915c7d&title=Unsafe%E5%8A%9F%E8%83%BD%E6%A6%82%E8%BF%B0 "Unsafe功能概述")<br />下面挑选重要的功能进行讲解。
<a name="Ugq3b"></a>
### 1、内存管理
Unsafe的内存管理功能主要包括：普通读写、volatile读写、有序写入、直接操作内存等分配内存与释放内存的功能。
<a name="tQyvc"></a>
#### 普通读写
Unsafe可以读写一个类的属性，即便这个属性是私有的，也可以对这个属性进行读写。
```java
// 获取内存地址指向的整数
public native int getInt(Object var1, long var2);

// 将整数写入指定内存地址
public native void putInt(Object var1, long var2, int var4);
```
`getInt`用于从对象的指定偏移地址处读取一个int。`putInt`用于在对象指定偏移地址处写入一个int。其他原始类型也提供有对应的方法。<br />另外，`Unsafe`的`getByte`、`putByte`方法提供了直接在一个地址上进行读写的功能。
<a name="F6iof"></a>
#### `volatile`读写
普通的读写无法保证可见性和有序性，而volatile读写就可以保证可见性和有序性。
```java
// 获取内存地址指向的整数，并支持volatile语义
public native int getIntVolatile(Object var1, long var2);

// 将整数写入指定内存地址，并支持volatile语义
public native void putIntVolatile(Object var1, long var2, int var4);
```
volatile读写要保证可见性和有序性，相对普通读写更加昂贵。
<a name="rDv61"></a>
#### 有序写入
有序写入只保证写入的有序性，不保证可见性，就是说一个线程的写入不保证其他线程立马可见。
```java
// 将整数写入指定内存地址、有序或者有延迟的方法
public native void putOrderedInt(Object var1, long var2, int var4);
```
而与`volatile`写入相比`putOrderedXX`写入代价相对较低，`putOrderedXX`写入不保证可见性，但是保证有序性，所谓有序性，就是保证指令不会重排序。
<a name="YYRFk"></a>
#### 直接操作内存
`Unsafe`提供了直接操作内存的能力：
```java
// 分配内存
public native long allocateMemory(long var1);
// 重新分配内存
public native long reallocateMemory(long var1, long var3);
// 内存初始化
public native void setMemory(long var1, long var3, byte var5);
// 内存复制
public native void copyMemory(Object var1, long var2, Object var4, long var5, long var7);
// 清除内存
public native void freeMemory(long var1);
```
对应操作内存，也提供了一些获取内存信息的方法：
```java
// 获取内存地址
public native long getAddress(long var1);

public native int addressSize();

public native int pageSize();
```
值得注意的是：利用`copyMemory`方法可以实现一个通用的对象拷贝方法，无需再对每一个对象都实现`clone`方法，但只能做到对象浅拷贝。
<a name="g5Jp5"></a>
### 2、非常规对象实例化
通常，通过new`或`反射来实例化对象，而`Unsafe`类提供的`allocateInstance`方法，可以直接生成对象实例，且无需调用构造方法和其他初始化方法。<br />这在对象反序列化的时候会很有用，能够重建和设置final字段，而不需要调用构造方法。
```java
// 直接生成对象实例，不会调用这个实例的构造方法
public native Object allocateInstance(Class<?> var1) throws InstantiationException;
```
<a name="Rhu8i"></a>
### 3、类加载java
通过以下方法，可以实现类的定义、创建等操作。
```java
// 方法定义一个类，用于动态地创建类
public native Class<?> defineClass(String var1, byte[] var2, int var3, int var4, ClassLoader var5, ProtectionDomain var6);

//  动态的创建一个匿名内部类
public native Class<?> defineAnonymousClass(Class<?> var1, byte[] var2, Object[] var3);

// 判断是否需要初始化一个类
public native boolean shouldBeInitialized(Class<?> var1);

// 保证已经初始化过一个类
public native void ensureClassInitialized(Class<?> var1);
```
<a name="KvDEw"></a>
### 4、偏移量相关
Unsafe提供以下方法获取对象的指针，通过对指针进行偏移，不仅可以直接修改指针指向的数据（即使它们是私有的），甚至可以找到JVM已经认定为垃圾、可以进行回收的对象。
```java
// 获取静态属性Field在对象中的偏移量，读写静态属性时必须获取其偏移量
public native long staticFieldOffset(Field var1);
// 获取非静态属性Field在对象实例中的偏移量，读写对象的非静态属性时会用到这个偏移量
public native long objectFieldOffset(Field var1);
// 返回Field所在的对象
public native Object staticFieldBase(Field var1);
// 返回数组中第一个元素实际地址相对整个数组对象的地址的偏移量
public native int arrayBaseOffset(Class<?> var1);
// 计算数组中第一个元素所占用的内存空间
public native int arrayIndexScale(Class<?> var1);
```
<a name="aYqWJ"></a>
### 5、数组操作
数组操作提供了以下方法：
```java
// 获取数组第一个元素的偏移地址
public native int arrayBaseOffset(Class<?> var1);
// 获取数组中元素的增量地址
public native int arrayIndexScale(Class<?> var1);
```
`arrayBaseOffset`与`arrayIndexScale`配合起来使用，就可以定位数组中每个元素在内存中的位置。<br />由于Java的数组最大值为`Integer.MAX_VALUE`，使用Unsafe类的内存分配方法可以实现超大数组。实际上这样的数据就可以认为是C数组，因此需要注意在合适的时间释放内存。
<a name="XNQ93"></a>
### 6、线程调度
线程调度相关方法如下：
```java
// 唤醒线程
public native void unpark(Object var1);
// 挂起线程
public native void park(boolean var1, long var2);
// 用于加锁，已废弃
public native void monitorEnter(Object var1);
// 用于加锁，已废弃
public native void monitorExit(Object var1);
// 用于加锁，已废弃
public native boolean tryMonitorEnter(Object var1);
```
通过`park`方法将线程进行挂起， 线程将一直阻塞到超时或中断条件出现。`unpark`方法可以终止一个挂起的线程，使其恢复正常。<br />整个并发框架中对线程的挂起操作被封装在`LockSupport`类中，`LockSupport`类中有各种版本pack方法，但最终都调用了`Unsafe.park()`方法。
<a name="zvPrO"></a>
### 7、CAS操作
Unsafe类的CAS操作可能是使用最多的方法。它为Java的锁机制提供了一种新的解决办法，比如`AtomicInteger`等类都是通过该方法来实现的。`compareAndSwap`方法是原子的，可以避免繁重的锁机制，提高代码效率。
```java
public final native boolean compareAndSwapObject(Object var1, long var2, Object var4, Object var5);

public final native boolean compareAndSwapInt(Object var1, long var2, int var4, int var5);

public final native boolean compareAndSwapLong(Object var1, long var2, long var4, long var6);
```
CAS一般用于乐观锁，它在Java中有广泛的应用，`ConcurrentHashMap`，`ConcurrentLinkedQueue`中都有用到CAS来实现乐观锁。
<a name="Reql1"></a>
### 8、内存屏障
JDK8新引入了用于定义内存屏障、避免代码重排的方法：
```java
// 保证在这个屏障之前的所有读操作都已经完成
public native void loadFence();

// 保证在这个屏障之前的所有写操作都已经完成
public native void storeFence();

// 保证在这个屏障之前的所有读写操作都已经完成
public native void fullFence();
```
<a name="clurE"></a>
### 9、其他
当然，`Unsafe`类中还提供了大量其他的方法，比如上面提到的CAS操作，以`AtomicInteger`为例，当调用`getAndIncrement`、`getAndDecrement`等方法时，本质上调用的就是`Unsafe`的`getAndAddInt`方法。
```java
public final int getAndIncrement() {
    return unsafe.getAndAddInt(this, valueOffset, 1);
}

public final int getAndDecrement() {
    return unsafe.getAndAddInt(this, valueOffset, -1);
}
```
在实践的过程中，如果阅读其他框架或类库实现，当发现用到Unsafe类，可对照该类的整体功能，结合应用场景进行分析，即可大概了解其功能。
