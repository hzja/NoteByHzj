ThreadLocal
<a name="0090055c02fec164ccfa2a9fb0e869f8"></a>
## 一、ThreadLocal简介
多线程访问同一个共享变量的时候容易出现并发问题，特别是多个线程对一个变量进行写入的时候，为了保证线程安全，一般使用者在访问共享变量的时候需要进行额外的同步措施才能保证线程安全性。`ThreadLocal`是除了加锁这种同步方式之外的一种保证一种规避多线程访问出现线程不安全的方法，在创建一个变量后，如果每个线程对其进行访问的时候访问的都是线程自己的变量这样就不会存在线程不安全问题。<br />`ThreadLocal`是JDK包提供的，它提供线程本地变量，如果创建一个`ThreadLocal`变量，那么访问这个变量的每个线程都会有这个变量的一个副本，在实际多线程操作的时候，操作的是自己本地内存中的变量，从而规避了线程安全问题，如下图所示<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452552-c9f65da1-ddbc-48a5-a4f3-620afad42268.png#height=305&id=TLRQJ&originHeight=305&originWidth=667&originalType=binary&size=0&status=done&style=shadow&width=667)
<a name="44edacadd63cdb76157ac9a28add087b"></a>
## 二、ThreadLocal简单使用
下面的例子中，开启两个线程，在每个线程内部设置了本地变量的值，然后调用`print`方法打印当前本地变量的值。如果在打印之后调用本地变量的`remove`方法会删除本地内存中的变量，代码如下所示
```java
public class ThreadLocalTest {
    static ThreadLocal<String> localVar = new ThreadLocal<>();
    static void print(String str) {
        //打印当前线程中本地内存中本地变量的值
        System.out.println(str + " :" + localVar.get());
        //清除本地内存中的本地变量
        localVar.remove();
    }
    public static void main(String[] args) {
        Thread t1  = new Thread(new Runnable() {
            @Override
            public void run() {
                //设置线程1中本地变量的值
                localVar.set("localVar1");
                //调用打印方法
                print("thread1");
                //打印本地变量
                System.out.println("after remove : " + localVar.get());
            }
        });
        Thread t2  = new Thread(new Runnable() {
            @Override
            public void run() {
                //设置线程1中本地变量的值
                localVar.set("localVar2");
                //调用打印方法
                print("thread2");
                //打印本地变量
                System.out.println("after remove : " + localVar.get());
            }
        });
        t1.start();
        t2.start();
    }
}
```
下面是运行后的结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614935553067-50dba2b5-cde0-424e-ae26-2b6a663acc40.png#height=250&id=OofK0&originHeight=749&originWidth=1947&originalType=binary&size=103916&status=done&style=shadow&width=649)
<a name="4567b958d416730e47fa2c7c1efdd621"></a>
## 三、ThreadLocal的实现原理
下面是`ThreadLocal`的类图结构，从图中可知：Thread类中有两个变量`threadLocals`和`inheritableThreadLocals`，二者都是`ThreadLocal`内部类`ThreadLocalMap`类型的变量，通过查看内部类`ThreadLocalMap`可以发现实际上它类似于一个`HashMap`。在默认情况下，每个线程中的这两个变量都为null<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452581-c8933ea5-3907-4c0d-b1a3-d3735435fd2f.png#height=31&id=ECPP2&originHeight=31&originWidth=440&originalType=binary&size=0&status=done&style=shadow&width=440)![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452496-de7eefe0-b23e-4944-9658-c74a829c99f4.png#height=32&id=IBCsU&originHeight=32&originWidth=542&originalType=binary&size=0&status=done&style=shadow&width=542)<br />只有当线程第一次调用`ThreadLocal`的set或者get方法的时候才会创建他们（后面会查看这两个方法的源码）。除此之外，不同的是，每个线程的本地变量不是存放在`ThreadLocal`实例中，而是放在调用线程的`ThreadLocals`变量里面（前面也说过，该变量是Thread类的变量）。也就是说，`ThreadLocal`类型的本地变量是存放在具体的线程空间上，其本身相当于一个装载本地变量的工具壳，通过set方法将value添加到调用线程的`threadLocals`中，当调用线程调用get方法时候能够从它的`threadLocals`中取出变量。如果调用线程一直不终止，那么这个本地变量将会一直存放在他的`threadLocals`中，所以不使用本地变量的时候需要调用`remove`方法将`threadLocals`中删除不用的本地变量。下面通过查看`ThreadLocal`的set、get以及remove方法来查看`ThreadLocal`具体实怎样工作的<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452486-ac4e4395-b9d8-49c1-a630-d6d472646c4f.png#height=582&id=EK4Bc&originHeight=582&originWidth=1027&originalType=binary&size=0&status=done&style=shadow&width=1027)
<a name="6bd9b42b588752b9478db9b61b22daa2"></a>
### 1、`set`方法源码
```java
public void set(T value) {
    //(1)获取当前线程（调用者线程）
    Thread t = Thread.currentThread();
    //(2)以当前线程作为key值，去查找对应的线程变量，找到对应的map
    ThreadLocalMap map = getMap(t);
    //(3)如果map不为null，就直接添加本地变量，key为当前线程，值为添加的本地变量值
    if (map != null)
        map.set(this, value);
    //(4)如果map为null，说明首次添加，需要首先创建出对应的map
    else
        createMap(t, value);
}
```
在上面的代码中，(2)处调用`getMap`方法获得当前线程对应的`threadLocals`(参照上面的图示和文字说明)，该方法代码如下
```java
ThreadLocalMap getMap(Thread t) {
    //获取线程自己的变量threadLocals，并绑定到当前调用线程的成员变量threadLocals上
    return t.threadLocals; 
}
```
如果调用`getMap`方法返回值不为null，就直接将`value`值设置到`threadLocals`中（`key`为当前线程引用，值为本地变量）；如果`getMap`方法返回null说明是第一次调用`set`方法（前面说到过，`threadLocals`默认值为null，只有调用`set`方法的时候才会创建`map`），这个时候就需要调用`createMap`方法创建`threadLocals`，该方法如下所示
```java
void createMap(Thread t, T firstValue) {
    t.threadLocals = new ThreadLocalMap(this, firstValue);
}
```
`createMap`方法不仅创建了`threadLocals`，同时也将要添加的本地变量值添加到了`threadLocals`中。
<a name="0a5eaa455e633e08f897e42cbd0f3884"></a>
### 2、`get`方法源码
在`get`方法的实现中，首先获取当前调用者线程，如果当前线程的`threadLocals`不为null，就直接返回当前线程绑定的本地变量值，否则执行`setInitialValue`方法初始化`threadLocals`变量。在`setInitialValue`方法中，类似于`set`方法的实现，都是判断当前线程的`threadLocals`变量是否为null，是则添加本地变量（这个时候由于是初始化，所以添加的值为null），否则创建`threadLocals`变量，同样添加的值为null。
```java
public T get() {
    //(1)获取当前线程
    Thread t = Thread.currentThread();
    //(2)获取当前线程的threadLocals变量
    ThreadLocalMap map = getMap(t);
    //(3)如果threadLocals变量不为null，就可以在map中查找到本地变量的值
    if (map != null) {
        ThreadLocalMap.Entry e = map.getEntry(this);
        if (e != null) {
            @SuppressWarnings("unchecked")
            T result = (T)e.value;
            return result;
        }
    }
    //(4)执行到此处，threadLocals为null，调用该更改初始化当前线程的threadLocals变量
    return setInitialValue();
}
private T setInitialValue() {
    //protected T initialValue() {return null;}
    T value = initialValue();
    //获取当前线程
    Thread t = Thread.currentThread();
    //以当前线程作为key值，去查找对应的线程变量，找到对应的map
    ThreadLocalMap map = getMap(t);
    //如果map不为null，就直接添加本地变量，key为当前线程，值为添加的本地变量值
    if (map != null)
        map.set(this, value);
    //如果map为null，说明首次添加，需要首先创建出对应的map
    else
        createMap(t, value);
    return value;
}
```
<a name="47f16b407e5d276c3478c5e7d91d92f2"></a>
### 3、`remove`方法的实现
`remove`方法判断该当前线程对应的`threadLocals`变量是否为null，不为null就直接删除当前线程中指定的`threadLocals`变量
```java
public void remove() {
    //获取当前线程绑定的threadLocals
    ThreadLocalMap m = getMap(Thread.currentThread());
    //如果map不为null，就移除当前线程中指定ThreadLocal实例的本地变量
    if (m != null)
        m.remove(this);
}
```
如下图所示：每个线程内部有一个名为`threadLocals`的成员变量，该变量的类型为`ThreadLocal.ThreadLocalMap`类型（类似于一个`HashMap`），其中的`key`为当前定义的`ThreadLocal`变量的this引用，value使用`set`方法设置的值。每个线程的本地变量存放在自己的本地内存变量`threadLocals`中，如果当前线程一直不消亡，那么这些本地变量就会一直存在（所以可能会导致内存溢出），因此使用完毕需要将其`remove`掉。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452495-f0258f7d-e4c9-49f4-8ed9-3c08e9cd72d9.png#height=438&id=KkY1v&originHeight=438&originWidth=899&originalType=binary&size=0&status=done&style=shadow&width=899)
<a name="74a19e61e09437462217001058debaa4"></a>
## 四、ThreadLocal不支持继承性
同一个`ThreadLocal`变量在父线程中被设置值后，在子线程中是获取不到的。（`threadLocals`中为当前调用线程对应的本地变量，所以二者自然是不能共享的）
```java
public class ThreadLocalTest2 {
    //(1)创建ThreadLocal变量
    public static ThreadLocal<String> threadLocal = new ThreadLocal<>();
    public static void main(String[] args) {
        //在main线程中添加main线程的本地变量
        threadLocal.set("mainVal");
        //新创建一个子线程
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("子线程中的本地变量值:"+threadLocal.get());
            }
        });
        thread.start();
        //输出main线程中的本地变量值
        System.out.println("mainx线程中的本地变量值:"+threadLocal.get());
    }
}
```
<a name="55b67aa1f8ff98173218542cbce4886a"></a>
## 五、InheritableThreadLocal类
在上面说到的`ThreadLocal`类是不能提供子线程访问父线程的本地变量的，而`InheritableThreadLocal`类则可以做到这个功能，下面是该类的源码
```java
public class InheritableThreadLocal<T> extends ThreadLocal<T> {
    protected T childValue(T parentValue) {
        return parentValue;
    }
    ThreadLocalMap getMap(Thread t) {
       return t.inheritableThreadLocals;
    }
    void createMap(Thread t, T firstValue) {
        t.inheritableThreadLocals = new ThreadLocalMap(this, firstValue);
    }
}
```
从上面代码可以看出，`InheritableThreadLocal`类继承了`ThreadLocal`类，并重写了`childValue`、`getMap`、`createMap`三个方法。其中`createMap`方法在被调用（当前线程调用`set`方法时得到的`map`为null的时候需要调用该方法）的时候，创建的是`inheritableThreadLocal`而不是`threadLocals`。同理，`getMap`方法在当前调用者线程调用`get`方法的时候返回的也不是`threadLocals`而是`inheritableThreadLocal`。<br />下面看看重写的`childValue`方法在什么时候执行，怎样让子线程访问父线程的本地变量值。首先从`Thread`类开始说起
```java
private void init(ThreadGroup g, Runnable target, String name,
                  long stackSize) {
    init(g, target, name, stackSize, null, true);
}
private void init(ThreadGroup g, Runnable target, String name,
                  long stackSize, AccessControlContext acc,
                  boolean inheritThreadLocals) {
    //判断名字的合法性
    if (name == null) {
        throw new NullPointerException("name cannot be null");
    }
    this.name = name;
    //(1)获取当前线程(父线程)
    Thread parent = currentThread();
    //安全校验
    SecurityManager security = System.getSecurityManager();
    if (g == null) { //g:当前线程组
        if (security != null) {
            g = security.getThreadGroup();
        }
        if (g == null) {
            g = parent.getThreadGroup();
        }
    }
    g.checkAccess();
    if (security != null) {
        if (isCCLOverridden(getClass())) {
            security.checkPermission(SUBCLASS_IMPLEMENTATION_PERMISSION);
        }
    }
    g.addUnstarted();
    this.group = g; //设置为当前线程组
    this.daemon = parent.isDaemon();//守护线程与否(同父线程)
    this.priority = parent.getPriority();//优先级同父线程
    if (security == null || isCCLOverridden(parent.getClass()))
        this.contextClassLoader = parent.getContextClassLoader();
    else
        this.contextClassLoader = parent.contextClassLoader;
    this.inheritedAccessControlContext =
        acc != null ? acc : AccessController.getContext();
    this.target = target;
    setPriority(priority);
    //(2)如果父线程的inheritableThreadLocal不为null
    if (inheritThreadLocals && parent.inheritableThreadLocals != null)
        //（3）设置子线程中的inheritableThreadLocals为父线程的inheritableThreadLocals
        this.inheritableThreadLocals =
        ThreadLocal.createInheritedMap(parent.inheritableThreadLocals);
    this.stackSize = stackSize;
    tid = nextThreadID();
}
```
在`init`方法中，首先(1)处获取了当前线程(父线程)，然后（2）处判断当前父线程的`inheritableThreadLocals`是否为null，然后调用`createInheritedMap`将父线程的`inheritableThreadLocals`作为构造函数参数创建了一个新的`ThreadLocalMap`变量，然后赋值给子线程。下面是`createInheritedMap`方法和`ThreadLocalMap`的构造方法
```java
static ThreadLocalMap createInheritedMap(ThreadLocalMap parentMap) {
    return new ThreadLocalMap(parentMap);
}
private ThreadLocalMap(ThreadLocalMap parentMap) {
    Entry[] parentTable = parentMap.table;
    int len = parentTable.length;
    setThreshold(len);
    table = new Entry[len];
    for (int j = 0; j < len; j++) {
        Entry e = parentTable[j];
        if (e != null) {
            @SuppressWarnings("unchecked")
            ThreadLocal<Object> key = (ThreadLocal<Object>) e.get();
            if (key != null) {
                //调用重写的方法
                Object value = key.childValue(e.value);
                Entry c = new Entry(key, value);
                int h = key.threadLocalHashCode & (len - 1);
                while (table[h] != null)
                    h = nextIndex(h, len);
                table[h] = c;
                size++;
            }
        }
    }
}
```
在构造函数中将父线程的`inheritableThreadLocals`成员变量的值赋值到新的`ThreadLocalMap`对象中。返回之后赋值给子线程的`inheritableThreadLocals`。总之，`InheritableThreadLocals`类通过重写`getMap`和`createMap`两个方法将本地变量保存到了具体线程的`inheritableThreadLocals`变量中，当线程通过`InheritableThreadLocals`实例的`set`或者`get`方法设置变量的时候，就会创建当前线程的`inheritableThreadLocals`变量。而父线程创建子线程的时候，`ThreadLocalMap`中的构造函数会将父线程的`inheritableThreadLocals`中的变量复制一份到子线程的`inheritableThreadLocals`变量中。
<a name="362110a123727779681dd4209a6e0f02"></a>
## 六、从ThreadLocalMap看ThreadLocal使用不当的内存泄漏问题
<a name="524130cf9ccf0b535f3b0853a04969d2"></a>
### 1、基础概念
首先看看`ThreadLocalMap`的类图，在前面的介绍中，可以知道`ThreadLocal`只是一个工具类，他为用户提供`get`、`set`、`remove`接口操作实际存放本地变量的`threadLocals`（调用线程的成员变量），也知道`threadLocals`是一个`ThreadLocalMap`类型的变量，下面来看看`ThreadLocalMap`这个类。在此之前，回忆一下Java中的四种引用类型。<br />①强引用：Java中默认的引用类型，一个对象如果具有强引用那么只要这种引用还存在就不会被GC。<br />②软引用：简言之，如果一个对象具有弱引用，在JVM发生OOM之前（即内存充足够使用），是不会GC这个对象的；只有到JVM内存不足的时候才会GC掉这个对象。软引用和一个引用队列联合使用，如果软引用所引用的对象被回收之后，该引用就会加入到与之关联的引用队列中<br />③弱引用（这里讨论`ThreadLocalMap`中的`Entry`类的重点）：如果一个对象只具有弱引用，那么这个对象就会被垃圾回收器GC掉(被弱引用所引用的对象只能生存到下一次GC之前，当发生GC时候，无论当前内存是否足够，弱引用所引用的对象都会被回收掉)。弱引用也是和一个引用队列联合使用，如果弱引用的对象被垃圾回收期回收掉，JVM会将这个引用加入到与之关联的引用队列中。若引用的对象可以通过弱引用的`get`方法得到，当引用的对象呗回收掉之后，再调用`get`方法就会返回null<br />④虚引用：虚引用是所有引用中最弱的一种引用，其存在就是为了将关联虚引用的对象在被GC掉之后收到一个通知。（不能通过`get`方法获得其指向的对象）<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452486-32a3ff5f-9210-4e5b-beb6-cb651957d678.png#height=371&id=OcoUC&originHeight=371&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)
<a name="1cca965538e6f7a4e55821595aee2446"></a>
### 2、分析ThreadLocalMap内部实现
从上面知道`ThreadLocalMap`内部实际上是一个Entry数组<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614872452489-ca1479d1-6540-4021-8fed-454720710a12.png#height=39&id=DZ6oK&originHeight=39&originWidth=247&originalType=binary&size=0&status=done&style=none&width=247)<br />，先看看Entry的这个内部类
```java
/**
 * 是继承自WeakReference的一个类，该类中实际存放的key是
 * 指向ThreadLocal的弱引用和与之对应的value值(该value值
 * 就是通过ThreadLocal的set方法传递过来的值)
 * 由于是弱引用，当get方法返回null的时候意味着坑能引用
 */
static class Entry extends WeakReference<ThreadLocal<?>> {
    /** value就是和ThreadLocal绑定的 */
    Object value;
    //k：ThreadLocal的引用，被传递给WeakReference的构造方法
    Entry(ThreadLocal<?> k, Object v) {
        super(k);
        value = v;
    }
}
//WeakReference构造方法(public class WeakReference<T> extends Reference<T> )
public WeakReference(T referent) {
    super(referent); //referent：ThreadLocal的引用
}
//Reference构造方法
Reference(T referent) {
    this(referent, null);//referent：ThreadLocal的引用
}
Reference(T referent, ReferenceQueue<? super T> queue) {
    this.referent = referent;
    this.queue = (queue == null) ? ReferenceQueue.NULL : queue;
}
```
在上面的代码中，可以看出，当前`ThreadLocal`的引用k被传递给`WeakReference`的构造函数，所以`ThreadLocalMap`中的`key`为`ThreadLocal`的弱引用。当一个线程调用`ThreadLocal`的set方法设置变量的时候，当前线程的`ThreadLocalMap`就会存放一个记录，这个记录的`key`值为`ThreadLocal`的弱引用，`value`就是通过set设置的值。如果当前线程一直存在且没有调用该`ThreadLocal`的`remove`方法，如果这个时候别的地方还有对`ThreadLocal`的引用，那么当前线程中的`ThreadLocalMap`中会存在对`ThreadLocal`变量的引用和value对象的引用，是不会释放的，就会造成内存泄漏。<br />考虑这个`ThreadLocal`变量没有其他强依赖，如果当前线程还存在，由于线程的`ThreadLocalMap`里面的`key`是弱引用，所以当前线程的`ThreadLocalMap`里面的`ThreadLocal`变量的弱引用在gc的时候就被回收，但是对应的`value`还是存在的这就可能造成内存泄漏(因为这个时候`ThreadLocalMap`会存在`key`为null但是`value`不为null的`entry`项)。<br />总结：`THreadLocalMap`中的Entry的key使用的是`ThreadLocal`对象的弱引用，在没有其他地方对`ThreadLocal`依赖，`ThreadLocalMap`中的`ThreadLocal`对象就会被回收掉，但是对应的不会被回收，这个时候`Map`中就可能存在`key`为null但是`value`不为null的项，这需要实际的时候使用完毕及时调用`remove`方法避免内存泄漏。
