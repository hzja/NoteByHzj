Java JMM
<a name="rhlbL"></a>
## 1、`volitale`是Java虚拟机提供的一种轻量级的同步机制
三大特性

- 保证可见性
- 不保证原子性
- 禁止指令重排

首先保证可见性
<a name="wf2z9"></a>
### 1.1 可见性
概念：当多个线程访问同一个变量时，一个线程修改了这个变量的值，其他线程能够立即看到修改的值
```java
/**
 * 1验证volatile的可见性
 * 1.1 如果int num = 0，number变量没有添加volatile关键字修饰
 * 1.2 添加了volatile，可以解决可见性
 */
class VolatileDemo1 {

    //自定义的类
    public static class MyTest{
        //类的内部成员变量num
        public int num = 0;
        //numTo60 方法，让num值为60
        public void numTo60(){
            num = 60;
        }
    }

    public static void main(String[] args) {

        MyTest myTest = new MyTest();
        //第一个线程
        new Thread(() -> {
            try {
                System.out.println(Thread.currentThread().getName() + "\t come in");
                Thread.sleep(3000);
                myTest.numTo60();
                System.out.println(Thread.currentThread().getName() + "\t update value:" + myTest.num);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } ,"thread1").start();;

       //主线程判断num值
        while (myTest.num == 0){
            //如果myData的num一直为零，main线程一直在这里循环
        }
        System.out.println(Thread.currentThread().getName() + "\t mission is over, num value is " + myTest.num);
    }
}
```
如上代码是没有保证可见性的，可见性存在于JMM当中即java内存模型当中的，可见性主要是指当一个线程改变其内部的工作内存当中的变量后，其他线程是否可以观察到，因为不同的线程件无法访问对方的工作内存，线程间的通信（传值）必须通过主内存来完成，因为此处没有添加volatile指令，导致其中thread1对num值变量进行更改时，main线程无法感知到num值发生更改，导致在while处无限循环，读不到新的num值，会发生死循环<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496902766-1facd778-5f79-4123-8261-93f6dbbde74e.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=u84e9cfd6&originHeight=130&originWidth=718&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u025dcbed-f7b3-4adb-8db8-8e72c844762&title=)<br />此时修改类中代码为
```java
/**
* volatile可以保证可见性，及时通知其他线程，主物理内存的值已经被修改
*/
public static class MyTest{
  //类的内部成员变量num
  public volatile int num = 0;
  //numTo60 方法，让num值为60
  public void numTo60(){
    num = 60;
  }
}
```
此时`volatile`就可以保证内存的可见性，此时运行代码就可以发现<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496902737-ededf389-ffdf-4be2-8636-1096bb5544eb.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=u9b16f74d&originHeight=158&originWidth=731&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc4f06481-dfe9-469a-aff8-ae544124649&title=)
<a name="bBqUx"></a>
### 1.2 不保证原子性
原子性概念：不可分割、完整性，即某个线程正在做某个具体业务时，中间不可以被加塞或者被分割，需要整体完整，要么同时成功，要么同时失败<br />类代码为：
```java
//自定义的类
public static class MyTest {
  //类的内部成员变量num
  public volatile int num = 0;

  public void numPlusPlus() {
    num++;
  }
}
```
主方法为
```java
public static void main(String[] args) {
    MyTest myTest = new MyTest();
    /**
         * 10个线程创建出来，每个线程执行2000次num++操作
         * 在字节码及底层，i++被抽象为三个操作
         * 即先取值，再自加，再赋值操作
         */
    for (int i = 1; i <= 10; i++) {
        new Thread(() -> {
            for (int j = 0; j < 2000; j++) {
                myTest.numPlusPlus();
            }
        }, "Thread" + i).start();
    }
    //这里规定线程数大于2，一般有GC线程以及main主线程
    while (Thread.activeCount() > 2) {
        Thread.yield();
    }
    System.out.println(Thread.currentThread().getName() + "\t finally num value is " + myTest.num);
}
```
代码如上所示，如果`volatile`保证原子性，那么10个线程分别执行自加2000次操作，那么最终结果一定是20000，但是执行三次结果如下
```java
//第一次
main  finally num value is 19003
//第二次
main  finally num value is 18694
//第三次
main  finally num value is 19552
```
可以发现，num的值每次都不相同，且最后的值都没有达到20000，这是为什么呢？
<a name="IcNyF"></a>
#### 为什么会出现这种情况？
首先，要考虑到这种情况，假如线程A执行到第11行即`myTest.numPlusPlus();`方法时<br />线程进入方法执行numPlusPlus方法后，num的值不管是多少，线程A将num的值首先初始化为0（假如主存中num的值为0），之后num的值自增为1，之后线程A挂起，线程B此时也将主存中的num值读到自己的工作内存中值为0，之后num的值自增1，之后线程B挂起，线程A继续运行将num的值写回主存，但是因为volatile关键字保证可见性，但是在很短的时间内，线程B也将num的值写回主存，此时num的值就少加了一次，所以最后总数基本上少于20000
<a name="sOITi"></a>
#### 如何解决？
但是JUC有线程的原子类为`AtomicInteger`类，此时，将类代码更改为
```java
public static class MyTest {
    //类的内部成员变量num
    public volatile int num = 0;
    AtomicInteger atomicInteger = new AtomicInteger();

    //numTo60 方法，让num值为60
    public void numTo60() {
        num = 60;
    }

    public void numPlusPlus() {
        num++;
    }
    public void myAtomPlus(){
        atomicInteger.getAndIncrement();
    }
}
```
共同测试num和`atomicInteger`，此时执行主函数，三次结果为
```java
//第一次
main  finally num value is 19217
main  finally atomicInteger value is 20000
//第二次
main  finally num value is 19605
main  finally atomicInteger value is 20000
//第三次
main  finally num value is 18614
main  finally atomicInteger value is 20000
```
可以发现`volatile`关键字并没有保证变量的原子性，但是JUC内部的`AtomicInteger`类保证了变量相关的原子性，`AtomicInteger`底层用到了CAS。
<a name="yS42V"></a>
### 1.3 禁止指令重排
有序性的概念：在计算机执行程序时，为了提高性能，编译器和处理器常常会对指令做重排，一般分以下三种<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496902758-680a8236-c960-4bfa-a13d-5d17f5ae88e1.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=u2877f893&originHeight=91&originWidth=813&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u174531ab-6a3a-48dd-8a44-d7746d80426&title=)<br />单线程环境里面确保程序最终执行结果和代码顺序执行的结果一致。<br />处理器在进行重排顺序是必须要考虑指令之间的数据依赖性<br />多线程环境中线程交替执行，由于编译器优化重排的存在，两个线程中使用的变量能否保证一致性时无法确定的，结果无法预测<br />重排代码实例：声明变量：`int a,b,x,y=0`<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496902737-749c3f60-aebc-41a5-b344-7878c06b021b.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=u39350feb&originHeight=162&originWidth=898&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uba4d4638-f5df-478f-bbd8-6fc419fd29b&title=)<br />如果编译器对这段程序代码执行重排优化后，可能出现如下情况：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496902778-2ea79a78-9f74-4da2-92b2-c9f308a9dccc.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=ue99d57e2&originHeight=161&originWidth=901&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9f238e0-eb7c-4822-b357-8b02cce0f06&title=)<br />这个结果说明在多线程环境下，由于编译器优化重排的存在，两个线程中使用的变量能否保证一致性是无法确定的<br />volatile实现禁止指令重排，从而避免了多线程环境下程序出现乱序执行的现象<br />内存屏障（Memory Barrier）又称内存栅栏，是一个CPU指令，他的作用有两个：

- 保证特定操作的执行顺序
- 保证某些变量的内存可见性（利用该特性实现volatile的内存可见性）

由于编译器和处理器都能执行指令重排优化。如果在之间插入一条Memory Barrier则会告诉编译器和CPU， 不管什么指令都不能和这条Memory Barrier指令重排顺序，也就是说通过插入内存屏障禁止在内存屏障前后的指令执行重排序优化。内存屏障另外一个作用是强制刷出各种CPU的缓存数据，因此任何CPU上的线程都能读 取到这些数据的最新版本<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496903297-07294449-ba22-48cc-bc24-8227d0ec0c09.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=u670f70cf&originHeight=625&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7586a39f-efc4-4f29-8775-3e488811aa4&title=)
<a name="nRcd1"></a>
## 2、JMM（java内存模型）
为什么提到JMM？JMM当中规定了可见性、原子性、以及有序性的问题，在多线程中只要保证了以上问题的正确性，那么基本上不会发生多线程当中存在数据安全问题<br />JMM（Java Memory Model）本身是一种抽象的概念，并不真实存在，他描述的时一组规则或规范，通过这组规范定义了程序中各个变量（包括实例字段，静态字段和构成数组对象的元素）的访问方式。<br />JMM关于同步的规定：

- 线程解锁前，必须把共享变量的值刷新回主内存
- 线程加锁前，必须读取主内存的最新值到自己的工作内存
- 加锁解锁时同一把锁

由于JVM运行程序的实体是线程，而每个线程创建时JVM都会为其创建一个工作内存（有的成为栈空间），工作内存是每个线程的私有数据区域，而java内存模型中规定所有变量都存储在主内存，主内存是贡献内存区域，所有线程都可以访问，但线程对变量的操作（读取赋值等）必须在工作内存中进行，首先概要将变量从主内存拷贝到自己的工作内存空间，然后对变量进行操作，操作完成后再将变量写回主内存，不能直接操作主内存中的变量，各个线程中的工作内存中存储着主内存的变量副本拷贝，因此不同的线程件无法访问对方的工作内存，线程间的通信（传值）必须通过主内存来完成，期间要访问过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496904201-8ceb5055-81ef-42bb-8bc5-e2f753c3899e.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=u5218ac14&originHeight=709&originWidth=848&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u466cc000-f9d4-4f7a-b961-503e0b0a39a&title=)
<a name="vGgBc"></a>
### JMM的三大特性

- 可见性
- 原子性
- 有序性

所以JMM当中的2.1和2.3在`volatile`当中都有很好的体现，`volatile`关键字并不能保证多线程当中的原子性，但是`volatile`是轻量级的同步机制，不想`synchronized`锁一样粒度太大
<a name="UlY8E"></a>
## 3、在那些地方用过`volatile`？结合实际谈论一下？
当普通单例模式在多线程情况下：
```java
/**
 * 普通单例模式
 * */
public class SingletonDemo {
    private static SingletonDemo instance = null;
    private SingletonDemo() {
        System.out.println(Thread.currentThread().getName() + "\t 构造方法 SingletonDemo（）");
    }
    public static SingletonDemo getInstance() {
        if (instance == null) {
            instance = new SingletonDemo();
        }
        return instance;
    }
    public static void main(String[] args) {
    //构造方法只会被执行一次
    // System.out.println(getInstance() == getInstance());
    // System.out.println(getInstance() == getInstance());
    // System.out.println(getInstance() == getInstance());
    //并发多线程后，构造方法会在一些情况下执行多次
        for (int i = 0; i < 10; i++) {
            new Thread(() -> {
                SingletonDemo.getInstance();
            }, "Thread " + i).start();
        }
    }
}
```
此时会出现两个线程运行了SingletonDemo的构造方法<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638496904185-dc11ad87-5784-4d39-96e9-d32d008e102e.webp#clientId=ub9a9b5e4-9288-4&from=paste&id=ub45f8c88&originHeight=124&originWidth=410&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uef7181ce-df6b-402e-94a8-34109699e0b&title=)<br />此时就违反了单例模式的规定，其构造方法在一些情况下会被执行多次<br />解决方式：<br />单例模式DCL代码<br />DCL （Double Check Lock双端检锁机制）在加锁前和加锁后都进行一次判断
```java
public static SingletonDemo getInstance() {
  if (instance == null) {
    synchronized (SingletonDemo.class) {
      if (instance == null) {
        instance = new SingletonDemo();
      }
    }
  }
  return instance;
}
```
不仅两次判空让程序执行更有效率，同时对代码块加锁，保证了线程的安全性<br />但是！还存在问题！<br />什么问题？<br />大部分运行结果构造方法只会被执行一次，但指令重排机制会让程序很小的几率出现构造方法被执行多次<br />DCL（双端检锁）机制不一定线程安全，原因时有指令重排的存在，加入`volatile`可以禁止指令重排<br />原因是在某一个线程执行到第一次检测，读取到`instance`不为null时，`instance`的引用对象可能没有完成初始化。`instance=new SingleDemo();`可以被分为一下三步（伪代码）：
```java
memory = allocate();//1.分配对象内存空间
instance(memory); //2.初始化对象
instance = memory; //3.设置instance执行刚分配的内存地址，此时instance!=null
```
步骤2和步骤3不存在数据依赖关系，而且无论重排前还是重排后程序的执行结果在单线程中并没有改变，因此这种重排优化时允许的<br />所以如果3步骤提前于步骤2，但是`instance`还没有初始化完成指令重排只会保证串行语义的执行的一致性（单线程），但并不关心多线程间的语义一致性。<br />所以当一条线程访问`instance`不为null时，由于`instance`示例未必已初始化完成，也就造成了线程安全问题。<br />此时加上`volatile`后就不会出现线程安全问题
```java
private static volatile SingletonDemo instance = null;
```
因为`volatile`禁止了指令重排序的问题
