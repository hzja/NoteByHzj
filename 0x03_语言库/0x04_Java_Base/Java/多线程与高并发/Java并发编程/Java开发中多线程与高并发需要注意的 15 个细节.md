Java

1. Class文件加载到同一个ClassLoader空间中是单例的，如果是不同的classloader则不是单例。
2. synchronized如果加在静态方法中，则锁住的是类（一般方法中锁住的是this），等同于`synchronize(T.class)`。
3. synchronized锁住的任何对象，看对象头顶的两位01，如果是已经标记过的，则是偏向锁。
4. synchronized锁对象底层jvm虚拟机规范没有要求，但是hostpot  markword中是用头顶两位用来标记，组合分别是不同锁的类型。
5. synchronized(this)表示锁定当前对象，与synchronized方法是等值的，如果静态方法则是锁定的T.class。
6. synchronized既保证了原子性又保证了可见性。
7. 程序中如果出现异常，则锁会被释放，其他程序会乱入，可能导致数据不一致。
8. syn（obj）  markdown 记录线程id（偏向锁），如果线程过来发现ID是自己的  那就直接用，偏向锁如果线程争用，则会升级为自旋锁（默认旋10次），十次以后还没得到这把锁则升级为重量级锁（-OS）， 去操作系统申请资源，变为等待状态则不占用CPU，锁只可以升级，没法降级。（由于java jvm没有规定synchronized如何实现，所以理论上调整jvm也是可以降级锁的），争用大部分是自旋锁，占CPU但是不访问操作系统，所以是用户态，不是内核态，加锁解锁比经过内核态效率要高。
9. 执行时间长或线程数量多用系统锁（-OS），时间短（指加锁代码时间短）用自旋锁（但是线程不能太多）。
10. 锁定的对象不能是String常量 Integer（特殊处理过）   Long等基本数据类型，比如String在常量池中，相同字面量是同一个锁，会出现一个线程死循环，其他线程没法进来，的当使用不可变类对象(final Class)作为对象锁时，使用synchronized同样会有并发问题 比如Integer类，由于不可变特性，作为锁但同步块内部仍然有计算操作，会生成一个新的锁对象（虽然String也是final Class，但它的原因却是字面量常量池），经过反编译之后对于锁定发Integer 变量i，执行i++操作相当于执行了`i = Integer.valueOf(i.intValue()+1)`，通过查看Integer的valueOf方法实现可知，其每次都new了一个新的Integer对象，锁变了.
11. Lock与synchronized区别在于，Lock中用的是CAS（很多实现用的是自旋）操作，占用CPU资源，而synchronized自旋（占用CPU时间）之后会进入wait队列，即不占用CPU时间。
12. volatile（大厂默认必会的知识点），意思：可变的、易变的，
- 保证线程可见性，不是实时监控。每一个线程有自己的局部内存，当需要使用共享内存中的变量时，会copy一份到自己的局部内存，修改之后会写回共享内存，但是其他需要使用这个变量的线程并不能及时读到自己的内存，这叫线程之间不可见，加了volatile之后可以保证一个线程做出改变之后其他线程能及时感受到。本质是使用了CPU的缓存一致性协议（MESI）

代码演示：

- 禁止指令重排序，跟CPU有关系，现代的CPU为了提高效率会并发执行命令，这种新的架构之上，要求编译器对指令进行重排序，而`volatile`会禁止指令重排序（JVM级别），举例子：单例模式的双重检查时，要不要加`volatile`？要！
```java
public class Mgr06 {
    private static volatile Mgr06 INSTANCE; //JIT
    private Mgr06() {
    }
    public static Mgr06 getInstance() {
        if (INSTANCE == null) {
            //双重检查
            synchronized (Mgr06.class) {
                if(INSTANCE == null) {
                    try {
                        Thread.sleep(1);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    INSTANCE = new Mgr06();
                }
            }
        }
        return INSTANCE;
    }
    public void m() {
        System.out.println("m");
    }
    public static void main(String[] args) {
        for(int i=0; i<100; i++) {
            new Thread(()->{
                System.out.println(Mgr06.getInstance().hashCode());
            }).start();
        }
    }
}
```
一个内存的赋值分为三个步骤，第一步是在堆内存中`new`，申请地址空间（如果是基本数据类型默认是0），第二步是赋值，第三步是赋值给栈内存中的变量，但是如果有指令         重排序会有第一步之后栈内存的变量就有值（0），即初始化一般后变量就有值了，那么双重检查时就会发现变量已经有值了，可以直接使用。超高并发时会有这种情况。加了volatile之后就不会有指令重排序的情况。则可以避免。没法测试，除非ASM编辑二进制在第二三步之间加入睡觉代码。

13. 锁应尽可能细化，但是如果一段代码块中很多很多锁，则粗化。
14. 锁定方法和非锁定方法可以同时执行。
15. CAS(compare and set缩写，无锁优化，自旋锁，也叫乐观锁，CPU原语级别的支持所有中间不可以被打断)，java.util.concurrent.atomic.AtomicInteger，凡是类加了Atomic的类都是用了CAS优化，线程安全。
