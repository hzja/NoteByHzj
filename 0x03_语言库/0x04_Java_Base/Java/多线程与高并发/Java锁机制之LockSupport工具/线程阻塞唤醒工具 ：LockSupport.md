Java
<a name="zA9ze"></a>
## LockSupport 简介
LockSupport 是 Java 并发编程中一个非常重要的组件，熟知的并发组件 Lock、线程池、CountDownLatch 等都是基于 AQS 实现的，而 AQS 内部控制线程阻塞和唤醒又是通过 LockSupport 来实现的。<br />从该类的注释上也可以发现，它是一个控制线程阻塞和唤醒的工具，与以往的不同是它解决了曾经 `wait()`、`notify()`、`await()`、`signal()` 的局限。
<a name="JytVq"></a>
## 回顾 synchronized 和 Lock
Java 中实现并发安全通常会通过这两种加锁的方式，对于 `synchronized` 加锁的方式，如果想要控制线程的阻塞和唤醒是通过锁对象的 `wait()` 和 `notify()` 方法，以下面循环交替打印 AB 为例
```java
int status = 2;
public static void main(String[] args) throws InterruptedException {
    TestSync obj = new TestSync();

     new Thread(() -> {
        synchronized (obj){
            while (true){
                if(obj.status == 1){
                    obj.wait();
                }
                System.out.println("A");
                obj.status = 1;
                TimeUnit.SECONDS.sleep(1);
                obj.notify();
            }
        }
     }).start();
     
    new Thread(() -> {
       synchronized (obj){
          while (true){
              if(obj.status == 2){
                  obj.wait();
              }
              System.out.println("B");
              obj.status = 2;
              TimeUnit.SECONDS.sleep(1);
              obj.notify();
          }
       }
    }).start();

}
```
如果使用 Lock 实现类，上述代码几乎是一样的，只是先获取 Condition 对象
```java
Condition condition = lock.newCondition();
```
把 `obj.wait()` 换成 `condition.await()`， `obj.notify()` 换成 `condition.signal()` 即可。
<a name="crRN0"></a>
## LockSupport 和 synchronized 和 Lock 的阻塞方式对比
| 技术 | 阻塞唤醒方式 | 局限 |
| --- | --- | --- |
| synchronized | 使用锁对象的 `wait()`、`notify()` | <br />1. 只能用在 synchronized 包裹的同步代码块中 <br />2. 必须先 `wait()` 才能 `notify()`<br /> |
| Lock | 使用 `condition` 的 `await()`、`signal()` | <br />1. 只能用在 lock 锁住的代码块中 <br />2. 必须先 `await()` 才能 `signal()`<br /> |
| LockSupport | `park()`、`unpark(Thread t)` | 没有限制 |

<a name="pF4ca"></a>
## LockSupport 的使用
下面代码中，使用 LockSupport 去阻塞和唤醒线程，可以多次尝试，LockSupport 的 `park()` 和 `unpark()` 方法没有先后顺序的限制，也不需要捕获异常，也没有限制要在什么代码块中才能使用。
```java
public static void main(String[] args) throws InterruptedException {
    Thread t1 = new Thread(() -> {
        System.out.println("A");
        LockSupport.park();
        System.out.println("被唤醒");
    });

    t1.start();

    TimeUnit.SECONDS.sleep(2);
    new Thread(() -> {
        System.out.println("B");
        LockSupport.unpark(t1);
    }).start();
}
```
<a name="vhaTG"></a>
## LockSupport 注意事项
<a name="QHm47"></a>
### 许可证提前发放
从该类的注释中可以看到这个类存储了使用它的线程的一个许可证，当调用 `park()` 方法的时候会判断当前线程的许可证是否存在，如果存在将直接放行，否则就阻塞。
```java
public static void main(String[] args) throws InterruptedException {
    Thread t1 = new Thread(() -> {
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        System.out.println("A");
        LockSupport.park();//不会阻塞
        System.out.println("被唤醒");
    });

    t1.start();

    TimeUnit.SECONDS.sleep(2);
    new Thread(() -> {
        System.out.println("B");
        System.out.println("先调用 unpark()");
        LockSupport.unpark(t1);
    },"t2").start();
}
```
看这个代码示例，这里在 t2 中先让线程 `t1 unpark()`，然后在 t1 中调用 `park()`，结果并不会阻塞 t1 线程。因为在 t2 中调用 `LockSupport.unpark(t1);` 的时候相当于给 t1 提前准备好了许可证。
<a name="XyCZ6"></a>
### 许可证不会累计
`LockSupport.unpark(t1);` 无论调用多少次，t1 的通行证只有一个，当在 t1 中调用两次 `park()` 方法时线程依然会被阻塞。
```java
public static void main(String[] args) throws InterruptedException {
    Thread t1 = new Thread(() -> {
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        System.out.println("A");
        LockSupport.park();
        System.out.println("被唤醒");
    });

    t1.start();

    TimeUnit.SECONDS.sleep(2);
    new Thread(() -> {
        System.out.println("B");
        System.out.println("先调用 unpark()");
        LockSupport.unpark(t1);
    },"t2").start();
}
```
以上述代码为例，t1 将被阻塞。
<a name="cSKgh"></a>
## LockSupport 底层实现
观察源码发现 `park()` 和 `unpark()` 最底下调用的是 `native()` 方法，源码在 C++ 中实现
```java
@IntrinsicCandidate
public native void park(boolean isAbsolute, long time);

@IntrinsicCandidate
public native void unpark(Object thread);
```
<a name="SfpH3"></a>
## 结语
LockSupport 是 Java 并发编程中非常重要的组件，这是下一步阅读 AQS(AbstractQueuedSynchronizer) 源码的基础。总之只要记住它是控制线程阻塞和唤醒的工具，并且知道它与其他阻塞唤醒方式的区别即可。
