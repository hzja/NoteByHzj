Java ThreadLocal
<a name="ryU9m"></a>
## 1、对`ThreadLocal`的理解
`ThreadLocal`，很多地方叫做线程本地变量，也有些地方叫做线程本地存储，其实意思差不多。可能很多朋友都知道`ThreadLocal`为变量在每个线程中都创建了一个副本，那么每个线程可以访问自己内部的副本变量。 这句话从字面上看起来很容易理解，但是真正理解并不是那么容易。<br />`ThreadLocal`的官方API解释为：<br />"该类提供了线程局部 (thread-local) 变量。这些变量不同于它们的普通对应物，因为访问某个变量（通过其 get 或 set 方法）的每个线程都有自己的局部变量，它独立于变量的初始化副本。`ThreadLocal` 实例通常是类中的 `private static` 字段，它们希望将状态与某一个线程（例如，用户 ID 或事务 ID）相关联。"<br />大概的意思有两点：

- 1、`ThreadLocal`提供了一种访问某个变量的特殊方式：访问到的变量属于当前线程，即保证每个线程的变量不一样，而同一个线程在任何地方拿到的变量都是一致的，这就是所谓的线程隔离。
   1. 如果要使用`ThreadLocal`，通常定义为`private static`类型，最好是定义为`private static final`类型。

`ThreadLocal`可以总结为一句话：`ThreadLocal`的作用是提供线程内的局部变量，这种变量在线程的生命周期内起作用，减少同一个线程内多个函数或者组件之间一些公共变量的传递的复杂度。<br />还是先来看一个例子：
```java
class ConnectionManager {
     
    private static Connection connect = null;
     
    public static Connection openConnection() {
        if(connect == null){
            connect = DriverManager.getConnection();
        }
        return connect;
    }
     
    public static void closeConnection() {
        if(connect!=null)
            connect.close();
    }
}
```
假设有这样一个数据库链接管理类，这段代码在单线程中使用是没有任何问题的，但是如果在多线程中使用呢？很显然，在多线程中使用会存在线程安全问题：第一，这里面的2个方法都没有进行同步，很可能在`openConnection`方法中会多次创建`connect`；第二，由于`connect`是共享变量，那么必然在调用`connect`的地方需要使用到同步来保障线程安全，因为很可能一个线程在使用`connect`进行数据库操作，而另外一个线程调用`closeConnection`关闭链接。<br />所以出于线程安全的考虑，必须将这段代码的两个方法进行同步处理，并且在调用`connect`的地方需要进行同步处理。这样将会大大影响程序执行效率，因为一个线程在使用`connect`进行数据库操作的时候，其他线程只有等待。<br />那么大家来仔细分析一下这个问题，这地方到底需不需要将`connect`变量进行共享？事实上，是不需要的。假如每个线程中都有一个`connect`变量，各个线程之间对`connect`变量的访问实际上是没有依赖关系的，即一个线程不需要关心其他线程是否对这个`connect`进行了修改的。<br />到这里，可能会有朋友想到，既然不需要在线程之间共享这个变量，可以直接这样处理，在每个需要使用数据库连接的方法中具体使用时才创建数据库链接，然后在方法调用完毕再释放这个连接。比如下面这样：
```java
class ConnectionManager {
     
    private  Connection connect = null;
     
    public Connection openConnection() {
        if(connect == null){
            connect = DriverManager.getConnection();
        }
        return connect;
    }
     
    public void closeConnection() {
        if(connect!=null)
            connect.close();
    }
}
 
 
class Dao{
    public void insert() {
        ConnectionManager connectionManager = new ConnectionManager();
        Connection connection = connectionManager.openConnection();
         
        //使用connection进行操作
         
        connectionManager.closeConnection();
    }
}
```
这样处理确实也没有任何问题，由于每次都是在方法内部创建的连接，那么线程之间自然不存在线程安全问题。但是这样会有一个致命的影响：导致服务器压力非常大，并且严重影响程序执行性能。由于在方法中需要频繁地开启和关闭数据库连接，这样不尽严重影响程序执行效率，还可能导致服务器压力巨大。<br />那么这种情况下使用`ThreadLocal`是再适合不过的了，因为`ThreadLocal`在每个线程中对该变量会创建一个副本，即每个线程内部都会有一个该变量，且在线程内部任何地方都可以使用，线程之间互不影响，这样一来就不存在线程安全问题，也不会严重影响程序执行性能。<br />但是要注意，虽然`ThreadLocal`能够解决上面说的问题，但是由于在每个线程中都创建了副本，所以要考虑它对资源的消耗，比如内存的占用会比不使用`ThreadLocal`要大。
<a name="dLmcF"></a>
## 2、深入解析`ThreadLocal`类
在上面谈到了对`ThreadLocal`的一些理解，那下面来看一下具体`ThreadLocal`是如何实现的。先了解一下`ThreadLocal`类提供的几个方法：
```java
public T get() { }
public void set(T value) { }
public void remove() { }
protected T initialValue() { }
```
`get()`方法是用来获取`ThreadLocal`在当前线程中保存的变量副本，`set()`用来设置当前线程中变量的副本，`remove()`用来移除当前线程中变量的副本，`initialValue()`是一个`protected`方法，用来返回此线程局部变量的当前线程的初始值，一般是在使用时进行重写的，它是一个延迟加载方法， 下面会详细说明。<br />首先来看一下`ThreadLocal`类是如何为每个线程创建一个变量的副本的。先看下`get`方法的实现：
```java
public T get() {
    //1.首先获取当前线程
    Thread t = Thread.currentThread();
    //2.获取线程的map对象
    ThreadLocalMap map = getMap(t);
    //3.如果map不为空，以threadlocal实例为key获取到对应Entry，然后从Entry中取出对象即可。
    if (map != null) {
        ThreadLocalMap.Entry e = map.getEntry(this);
        if (e != null)
            return (T)e.value;
    }
    //如果map为空，也就是第一次没有调用set直接get（或者调用过set，又调用了remove）时，为其设定初始值
    return setInitialValue();
}
```
第3行是取得当前线程，然后通过`getMap(t)`方法获取到一个`map`，`map`的类型为`ThreadLocalMap`。然后接着下面获取到`<key,value>`键值对，注意这里获取键值对传进去的是  this，而不是当前线程t。如果获取成功，则返回`value`值。如果`map`为空，则调用`setInitialValue`方法返回`value`。<br />下面对上面的每一句来仔细分析。首先看一下`getMap`方法中做了什么：
```java
ThreadLocalMap getMap(Thread t) {
    return t.threadLocals;
}
```
可能大家没有想到的是，在`getMap`中，是调用当期线程t，返回当前线程t中的一个成员变量`threadLocals`。那么继续取Thread类中取看一下成员变量`threadLocals`是什么：
```java
/* ThreadLocal values pertaining to this thread. This map is maintained
 * by the ThreadLocal class. */
ThreadLocal.ThreadLocalMap threadLocals = null;
```
实际上就是一个`ThreadLocalMap`，这个类型是`ThreadLocal`类的一个内部类，继续取看`ThreadLocalMap`的实现：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624021591541-17febdc6-610f-452f-8a78-aa269061bba3.webp#clientId=uf7dea3be-2321-4&from=paste&id=u02a0adc8&originHeight=424&originWidth=635&originalType=url&ratio=3&status=done&style=shadow&taskId=u44958929-4ccf-406d-83fe-f61a98b4d6f)<br />可以看到`ThreadLocalMap`的`Entry`继承了`WeakReference`，并且使用`ThreadLocal`作为键值。
<a name="DKQ77"></a>
### 总结
`get()`方法的第3和第5行很明显是获取属于当前线程的`ThreadLocalMap`，如果这个map不为空，就以当前的`ThreadLocal`为键，去获取相应的`Entry`，`Entry`是`ThreadLocalMap`的静态内部类，它继承于弱引用，所以在`get()`方法里面如第10行一样调用`e.value`方法就可以获取实际的资源副本值。但是如果有一个为空，说明属于该线程的资源副本还不存在，则需要去创建资源副本，从代码中可以看到是调用`setInitialValue()`方法，其定义如下：
```java
/**
 * Variant of set() to establish initialValue. Used instead
 * of set() in case user has overridden the set() method.
 *
 * @return the initial value
 */
private T setInitialValue() {
    T value = initialValue();
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null)
        map.set(this, value);
    else
        createMap(t, value);
    return value;
}
```
第8行调用`initialValue()`方法初始化一个值。接下来是判断线程的`ThreadLocalMap`是否为空，不为空就直接设置值（键为`this`，值为`value`），为空则创建一个Map，调用方法为`createMap()`，其定义如下：
```java
void createMap(Thread t, T firstValue) {
    t.threadLocals = new ThreadLocalMap(this, firstValue);
}
```
简单明了，而`ThreadLocalMap`的这个构造方法的实现如下：
```java
/**
 * Construct a new map initially containing (firstKey, firstValue).
 * ThreadLocalMaps are constructed lazily, so we only create
 * one when we have at least one entry to put in it.
 */
ThreadLocalMap(ThreadLocal firstKey, Object firstValue) {
    table = new Entry[INITIAL_CAPACITY];
    int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
    table[i] = new Entry(firstKey, firstValue);
    size = 1;
    setThreshold(INITIAL_CAPACITY);
}
```
实例化table数组用于存储键值对，然后通过映射将键值对存储进入相应的位置。<br />下面再来看set方法。
```java
/** 
 * Sets the current thread's copy of this thread-local variable 
 * to the specified value.  Most subclasses will have no need to 
 * override this method, relying solely on the {@link #initialValue} 
 * method to set the values of thread-locals. 
 * 
 * @param value the value to be stored in the current thread's copy of 
 *        this thread-local. 
 */  
public void set(T value) {  
    // 获取当前线程对象  
    Thread t = Thread.currentThread();  
    // 获取当前线程本地变量Map  
    ThreadLocalMap map = getMap(t);  
    // map不为空  
    if (map != null)  
        // 存值  
        map.set(this, value);  
    else  
        // 创建一个当前线程本地变量Map  
        createMap(t, value);  
}
```
在这个方法内部看到，首先通过`getMap(Thread t)`方法获取一个和当前线程相关的`ThreadLocalMap`，然后将变量的值设置到这个`ThreadLocalMap`对象中，当然如果获取到的`ThreadLocalMap`对象为空，就通过`createMap`方法创建。<br />至此，可能大部分朋友已经明白了`ThreadLocal`是如何为每个线程创建变量的副本的：<br />首先，在每个线程`Thread`内部有一个`ThreadLocal.ThreadLocalMap`类型的成员变量`threadLocals`，这个`threadLocals`就是用来存储实际的变量副本的，键值为当前`ThreadLocal`变量，`value`为变量副本（即T类型的变量）。<br />初始时，在`Thread`里面，`threadLocals`为空，当通过`ThreadLocal`变量调用`get()`方法或者`set()`方法，就会对`Thread`类中的`threadLocals`进行初始化，并且以当前`ThreadLocal`变量为键值，以`ThreadLocal`要保存的副本变量为`value`，存到`threadLocals`。<br />然后在当前线程里面，如果要使用副本变量，就可以通过`get`方法在`threadLocals`里面查找。
<a name="vc1Ru"></a>
## 3、示例
<a name="AmaH5"></a>
### 示例1：
下面通过一个例子来证明通过`ThreadLocal`能达到在每个线程中创建变量副本的效果：
```java
package com.demo.test;

public class TestThreadLocal {

    ThreadLocal<Long> longLocal = new ThreadLocal<Long>();
    ThreadLocal<String> stringLocal = new ThreadLocal<String>();
 
    public void set() {
        longLocal.set(Thread.currentThread().getId());
        stringLocal.set(Thread.currentThread().getName());
    }
     
    public long getLong() {
        return longLocal.get();
    }
     
    public String getString() {
        return stringLocal.get();
    }
     
    public static void main(String[] args) throws InterruptedException {
        final TestThreadLocal test = new TestThreadLocal();
         
        test.set();
        System.out.println(test.getLong());
        System.out.println(test.getString());
         
        Thread thread1 = new Thread(){
            public void run() {
                test.set();
                System.out.println(test.getLong());
                System.out.println(test.getString());
            };
        };
        thread1.start();
        thread1.join();
         
        System.out.println(test.getLong());
        System.out.println(test.getString());
    }
}
```
<a name="UXNej"></a>
### 这段代码的输出结果为：
```java
1
main
8
Thread-0
1
```
main 从这段代码的输出结果可以看出，在main线程中和thread1线程中，`longLocal`保存的副本值和`stringLocal`保存的副本值都不一样。最后一次在main线程再次打印副本值是为了证明在main线程中和thread1线程中的副本值确实是不同的。
<a name="EzDJE"></a>
### 总结一下：

- 1）实际的通过`ThreadLocal`创建的副本是存储在每个线程自己的`threadLocals`中的；
- 2）为何`threadLocals`的类型`ThreadLocalMap`的键值为`ThreadLocal`对象，因为每个线程中可有多个`threadLocal`变量，就像上面代码中的`longLocal`和`stringLocal`；
- 3）在进行get之前，必须先set，否则会报空指针异常。如果想在get之前不需要调用set就能正常访问的话，必须重写`initialValue()`方法。

因为在上面的代码分析过程中，可以发现如果没有先set的话，即在map中查找不到对应的存储，则会通过调用`setInitialValue`方法返回i，而在`setInitialValue`方法中，有一个语句是`T value = initialValue()`， 而默认情况下，`initialValue`方法返回的是null。<br />注意 ：默认情况下 `initValue()`, 返回 null 。线程在没有调用 set 之前，第一次调用 get 的时候， get方法会默认去调用 `initValue` 这个方法。所以如果没有覆写这个方法，可能导致 get 返回的是 null 。当然如果调用过 set 就不会有这种情况了。但是往往在多线程情况下不能保证每个线程的在调用 `get` 之前都调用了set ，所以最好对 `initValue` 进行覆写，以免导致空指针异常。
<a name="yr3ha"></a>
### 看下面这个例子：
```java
public class TestThreadLocal {

    ThreadLocal<Long> longLocal = new ThreadLocal<Long>();
    ThreadLocal<String> stringLocal = new ThreadLocal<String>();
 
    public void set() {
        longLocal.set(Thread.currentThread().getId());
        stringLocal.set(Thread.currentThread().getName());
    }
     
    public long getLong() {
        return longLocal.get();
    }
     
    public String getString() {
        return stringLocal.get();
    }
     
    public static void main(String[] args) throws InterruptedException {
        final TestThreadLocal test = new TestThreadLocal();
         
        //test.set();
        System.out.println(test.getLong());
        System.out.println(test.getString());
         
        Thread thread1 = new Thread(){
            public void run() {
                test.set();
                System.out.println(test.getLong());
                System.out.println(test.getString());
            };
        };
        thread1.start();
        thread1.join();
         
        System.out.println(test.getLong());
        System.out.println(test.getString());
    }
}
```
在main线程中，没有先set，直接get的话，运行时会报空指针异常。
```java
Exception in thread "main" java.lang.NullPointerException
    at com.demo.test.TestThreadLocal.getLong(TestThreadLocal.java:14)
    at com.demo.test.TestThreadLocal.main(TestThreadLocal.java:25)
```
但是如果改成下面这段代码，即重写了`initialValue`方法：
```java
public class TestThreadLocal {

    ThreadLocal<Long> longLocal = new ThreadLocal<Long>(){
         protected Long initialValue() {
             return Thread.currentThread().getId();
         };
    };
    ThreadLocal<String> stringLocal = new ThreadLocal<String>(){
         protected String initialValue() {
             return Thread.currentThread().getName();
         };
    };
 
    public void set() {
        longLocal.set(Thread.currentThread().getId());
        stringLocal.set(Thread.currentThread().getName());
    }
     
    public long getLong() {
        return longLocal.get();
    }
     
    public String getString() {
        return stringLocal.get();
    }
     
    public static void main(String[] args) throws InterruptedException {
        final TestThreadLocal test = new TestThreadLocal();
         
        //test.set();
        System.out.println(test.getLong());
        System.out.println(test.getString());
         
        Thread thread1 = new Thread(){
            public void run() {
                test.set();
                System.out.println(test.getLong());
                System.out.println(test.getString());
            };
        };
        thread1.start();
        thread1.join();
         
        System.out.println(test.getLong());
        System.out.println(test.getString());
    }
}
```
就可以直接不用先set而直接调用get了。
<a name="uBlmQ"></a>
### 示例2：
```java
public class TestNum {

    // ①通过匿名内部类覆盖ThreadLocal的initialValue()方法，指定初始值  
    private static ThreadLocal<Integer> seqNum = new ThreadLocal<Integer>() {  
        public Integer initialValue() {  
            return 0;  
        }  
    };  
  
    // ②获取下一个序列值  
    public int getNextNum() {  
        seqNum.set(seqNum.get() + 1);  
        return seqNum.get();  
    }  
  
    public static void main(String[] args) {  
        TestNum sn = new TestNum();  
        // ③ 3个线程共享sn，各自产生序列号  
        TestClient t1 = new TestClient(sn);  
        TestClient t2 = new TestClient(sn);  
        TestClient t3 = new TestClient(sn);  
        t1.start();  
        t2.start();  
        t3.start();  
    }  
  
    private static class TestClient extends Thread {  
        private TestNum sn;  
  
        public TestClient(TestNum sn) {  
            this.sn = sn;  
        }  
  
        public void run() {  
            for (int i = 0; i < 3; i++) {  
                // ④每个线程打出3个序列值  
                System.out.println("thread[" + Thread.currentThread().getName() + "] --> sn["  
                         + sn.getNextNum() + "]");  
            }  
        }  
    }  
}
```
通常通过匿名内部类的方式定义`ThreadLocal`的子类，提供初始的变量值，如例子中①处所示。TestClient线程产生一组序列号，在③处，生成3个TestClient，它们共享同一个TestNum实例。运行以上代码，在控制台上输出以下的结果：
```java
thread[Thread-0] --> sn[1]
thread[Thread-1] --> sn[1]
thread[Thread-2] --> sn[1]
thread[Thread-1] --> sn[2]
thread[Thread-0] --> sn[2]
thread[Thread-1] --> sn[3]
thread[Thread-2] --> sn[2]
thread[Thread-0] --> sn[3]
thread[Thread-2] --> sn[3]
```
考察输出的结果信息，可以发现每个线程所产生的序号虽然都共享同一个TestNum实例，但它们并没有发生相互干扰的情况，而是各自产生独立的序列号，这是因为通过`ThreadLocal`为每一个线程提供了单独的副本。
<a name="bapGe"></a>
## 4、`ThreadLocal`的应用场景
最常见的`ThreadLocal`使用场景为 用来解决 数据库连接、Session管理等。
```java
private static ThreadLocal<Connection> connectionHolder = new ThreadLocal<Connection>() {
    public Connection initialValue() {
        return DriverManager.getConnection(DB_URL);
    }
};
 
public static Connection getConnection() {
    return connectionHolder.get();
}

private static final ThreadLocal threadSession = new ThreadLocal();
 
public static Session getSession() throws InfrastructureException {
    Session s = (Session) threadSession.get();
    try {
        if (s == null) {
            s = getSessionFactory().openSession();
            threadSession.set(s);
        }
    } catch (HibernateException ex) {
        throw new InfrastructureException(ex);
    }
    return s;
}
```
`ThreadLocal`对象通常用于防止对可变的单实例变量或全局变量进行共享。<br />当一个类中使用了`static`成员变量的时候，一定要多问问自己，这个`static`成员变量需要考虑线程安全吗？也就是说，多个线程需要独享自己的`static`成员变量吗？如果需要考虑，不妨使用`ThreadLocal`。<br />`ThreadLocal`的主要应用场景为多线程多实例（每个线程对应一个实例）的对象的访问，并且这个对象很多地方都要用到。例如：同一个网站登录用户，每个用户服务器会为其开一个线程，每个线程中创建一个`ThreadLocal`，里面存用户基本信息等，在很多页面跳转时，会显示用户信息或者得到用户的一些信息等频繁操作，这样多线程之间并没有联系而且当前线程也可以及时获取想要的数据。

---

`ThreadLocal`通常用来共享数据，在多个方法中使用某个变量，这个变量是当前线程的状态，其它线程不依赖这个变量，第一时间想到的就是把变量定义在方法内部，然后再方法之间传递参数来使用，这个方法能解决问题，但是有个烦人的地方就是，每个方法都需要声明形参，多处声明，多处调用。影响代码的美观和维护。有没有一种方法能将变量像`private static`形式来访问呢？这样在类的任何一处地方就都能使用。这个时候`ThreadLocal`大显身手了。
<a name="ZgXsA"></a>
## 5、总结
<a name="xE4pE"></a>
### 1. 实现思想
`ThreadLocal` 的实现思想，在前面已经说了，每个线程维护一个 `ThreadLocalMap` 的映射表，映射表的 key 是 `ThreadLocal` 实例本身，`value` 是要存储的副本变量。`ThreadLocal` 实例本身并不存储值，它只是提供一个在当前线程中找到副本值的 key。如下图所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624021592846-a9adcd9e-2fdf-4a6d-ae83-7f6bfad41817.webp#clientId=uf7dea3be-2321-4&from=paste&id=u0c8f6bce&originHeight=403&originWidth=714&originalType=url&ratio=3&status=done&style=shadow&taskId=u5adf669b-ebed-47bb-9448-55853318515)
<a name="ft0eK"></a>
### 2. 线程隔离的秘密
线程隔离的秘密，就在于`ThreadLocalMap`这个类。`ThreadLocalMap`是`ThreadLocal`类的一个静态内部类，它实现了键值对的设置和获取（对比Map对象来理解），每个线程中都有一个独立的`ThreadLocalMap`副本，它所存储的值，只能被当前线程读取和修改。`ThreadLocal`类通过操作每一个线程特有的`ThreadLocalMap`副本，从而实现了变量访问在不同线程中的隔离。因为每个线程的变量都是自己特有的，完全不会有并发错误。还有一点就是，`ThreadLocalMap`存储的键值对中的键是this对象指向的`ThreadLocal`对象，而值就是所设置的对象了。
<a name="wA0jU"></a>
### 3. `ThreadLocalMap`
`ThreadLocalMap`并不是为了解决线程安全问题，而是提供了一种将实例绑定到当前线程的机制，类似于隔离的效果，实际上自己在方法中`new`出来变量也能达到类似的效果。`ThreadLocalMap`跟线程安全基本不搭边，绑定上去的实例也不是多线程公用的，而是每个线程new一份，这个实例肯定不是共用的，如果共用了，那就会引发线程安全问题。`ThreadLocalMap`最大的用处就是用来把实例变量共享成全局变量，在程序的任何方法中都可以访问到该实例变量而已。网上很多人说`ThreadLocalMap`是解决了线程安全问题，其实是望文生义，两者不是同类问题。
<a name="Mwk8u"></a>
### 4. 设计的初衷
`ThreadLocal`设计的初衷是为了解决多线程编程中的资源共享问题。提起这个，大家一般会想到`synchronized`，`synchronized`采取的是“以时间换空间”的策略，本质上是对关键资源上锁，让大家排队操作。而`ThreadLocal`采取的是“以空间换时间”的思路，为每个使用该变量的线程提供独立的变量副本，在本线程内部，它相当于一个“全局变量”，可以保证本线程任何时间操纵的都是同一个对象。
<a name="Feoz7"></a>
### 5. 最重要的概念
`ThreadLocal`类最重要的一个概念是，其原理是通过一个`ThreadLocal`的静态内部类`ThreadLocalMap`实现，但是实际中，`ThreadLocal`不保存`ThreadLocalMap`，而是有每个Thread内部维护`ThreadLocal.ThreadLocalMap threadLocals`一份数据结构。<br />这里画张图更容易理解，假如有如下的代码：
```java
class ThreadLocalDemo{
    ThreadLocal<Integer> localA = new ThreadLocal<Integer>();
    ThreadLocal<Integer> localB = new ThreadLocal<Integer>();
}
```
在多线程环境下，数据结构应该是如下图所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624021591690-3e1c0935-0338-4c42-add9-1a7db4f1355e.webp#clientId=uf7dea3be-2321-4&from=paste&id=u481bcc6c&originHeight=705&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u33b51585-e086-431b-8159-3d691aa7ff5)
<a name="M0jVN"></a>
### 6. 一般步骤

- （1）在多线程的类（如ThreadDemo类）中，创建一个`ThreadLocal`对象threadXxx，用来保存线程间需要隔离处理的对象xxx。
- （2）在ThreadDemo类中，创建一个获取要隔离访问的数据的方法`getXxx()`，在方法中判断，若`ThreadLocal`对象为null时候，应该`new()`一个隔离访问类型的对象，并强制转换为要应用的类型。
- （3）在ThreadDemo类的`run()`方法中，通过`getXxx()`方法获取要操作的数据，这样可以保证每个线程对应一个数据对象，在任何时刻都操作的是这个对象。
<a name="UJHTT"></a>
### 7. `ThreadLocal` 与 `synchronized` 的对比

- （1）`ThreadLocal`和`synchonized`都用于解决多线程并发访问。但是`ThreadLocal`与`synchronized`有本质的区别。`synchronized`是利用锁的机制，使变量或代码块在某一时该只能被一个线程访问。而`ThreadLocal`为每一个线程都提供了变量的副本，使得每个线程在某一时间访问到的并不是同一个对象，这样就隔离了多个线程对数据的数据共享。而`synchronized`却正好相反，它用于在多个线程间通信时能够获得数据共享。
- （2）`synchronized`用于线程间的数据共享，而`ThreadLocal`则用于线程间的数据隔离。
<a name="J5DN5"></a>
### 8. 一句话理解`ThreadLocal`
向`ThreadLocal`里面存东西就是向它里面的Map存东西的，然后`ThreadLocal`把这个`Map`挂到当前的线程底下，这样`Map`就只属于这个线程了。
