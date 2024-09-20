Java Synchronized<br />在并发编程中`Synchronized`一直都是元老级的角色，`Jdk 1.6`以前大家都称呼它为重量级锁，相对于`J U C`包提供的`Lock`，它会显得笨重，不过随着`Jdk 1.6`对`Synchronized`进行各种优化后，`Synchronized`性能已经非常快了。
<a name="Fuw8J"></a>
# 内容大纲
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406253951-3954e521-2318-42a2-9a74-28d62cae75f3.png#align=left&display=inline&height=517&originHeight=517&originWidth=633&size=0&status=done&style=shadow&width=633)
<a name="2l7XG"></a>
# Synchronized使用方式
`Synchronized`是`Java`提供的**同步关键字**，在多线程场景下，对共享资源代码段进行读写操作（**必须包含写操作，光读不会有线程安全问题，因为读操作天然具备线程安全特性**），可能会出现线程安全问题，可以使用`Synchronized`锁定共享资源代码段，达到**互斥**（`mutualexclusion`）效果，保证线程安全。<br />共享资源代码段又称为**临界区**（`critical section`），保证**临界区互斥**，是指执行**临界区**（`critical section`）的只能有一个线程执行，其他线程阻塞等待，达到排队效果。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406253995-76fcb5f1-634c-471a-8e26-2e4314778926.png#align=left&display=inline&height=542&originHeight=542&originWidth=861&size=0&status=done&style=shadow&width=861)<br />`Synchronized`的食用方式有三种

- **修饰普通函数，监视器锁（`monitor`）便是对象实例（`this`）**
- **修饰静态静态函数，视器锁（`monitor`）便是对象的`Class`实例（每个对象只有一个`Class`实例）**
- **修饰代码块，监视器锁（`monitor`）是指定对象实例**
<a name="qM9QK"></a>
## 普通函数
普通函数使用`Synchronized`的方式很简单，在**访问权限修饰符**与**函数返回类型**间加上`Synchronized`。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406253991-07955dda-cdca-4eb3-91d6-0ba5ba6b8965.png#align=left&display=inline&height=447&originHeight=447&originWidth=654&size=0&status=done&style=shadow&width=654)<br />多线程场景下，`thread`与`threadTwo`两个线程执行`incr`函数，`incr`函数作为**共享资源代码段**被多线程**读写操作**，将它称为**临界区**，为了保证**临界区互斥**，使用`Synchronized`修饰`incr`函数即可。
```java
public class SyncTest {
    private int j = 0;
    
    /**
     * 自增方法
     */
    public synchronized void incr(){
        //临界区代码--start
        for (int i = 0; i < 10000; i++) {
            j++;
        }
        //临界区代码--end
    }
    public int getJ() {
        return j;
    }
}
public class SyncMain {
    public static void main(String[] agrs) throws InterruptedException {
        SyncTest syncTest = new SyncTest();
        Thread thread = new Thread(() -> syncTest.incr());
        Thread threadTwo = new Thread(() -> syncTest.incr());
        thread.start();
        threadTwo.start();
        thread.join();
        threadTwo.join();
        //最终打印结果是20000，如果不使用synchronized修饰，就会导致线程安全问题，输出不确定结果
        System.out.println(syncTest.getJ());
    }
}
```
代码十分简单，`incr`函数被`synchronized`修饰，函数逻辑是对`j`进行`10000`次累加，两个线程执行`incr`函数，最后输出`j`结果。<br />被`synchronized`修饰函数简称**同步函数**，线程执行称**同步函数**前，需要先获取监视器锁，简称锁，获取锁成功才能执行**同步函数**，**同步函数**执行完后，**线程会释放锁并通知唤醒其他线程获取锁**，获取锁失败「**则阻塞并等待通知唤醒该线程重新获取锁**」，**同步函数**会以`this`作为锁，即当前对象，以上面的代码段为例就是`syncTest`对象。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254327-bd1027aa-16b8-40ba-a1d9-c849bc25bbfe.png#align=left&display=inline&height=1039&originHeight=1039&originWidth=803&size=0&status=done&style=shadow&width=803)

- **线程`thread`执行`syncTest.incr()`前**
- **线程`thread`获取锁成功**
- **线程`threadTwo`执行`syncTest.incr()`前**
- **线程`threadTwo`获取锁失败**
- **线程`threadTwo`阻塞并等待唤醒**
- **线程`thread`执行完`syncTest.incr()`，`j`累积到`10000`**
- **线程`thread`释放锁，通知唤醒`threadTwo`线程获取锁**
- **线程`threadTwo`获取锁成功**
- **线程`threadTwo`执行完`syncTest.incr()`，`j`累积到`20000`**
- **线程`threadTwo`释放锁**
<a name="ie9Vy"></a>
## 静态函数
静态函数顾名思义，就是静态的函数，它使用`Synchronized`的方式与普通函数一致，唯一的区别是锁的对象不再是`this`，而是`Class`对象。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254350-e6944a98-2d87-485f-bf81-969ea82be01b.png#align=left&display=inline&height=306&originHeight=306&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />多线程执行`Synchronized`修饰静态函数代码段如下。
```java
public class SyncTest {
    private static int j = 0;
    
    /**
     * 自增方法
     */
    public static synchronized void incr(){
        //临界区代码--start
        for (int i = 0; i < 10000; i++) {
            j++;
        }
        //临界区代码--end
    }
    public static int getJ() {
        return j;
    }
}
public class SyncMain {
    public static void main(String[] agrs) throws InterruptedException {
        Thread thread = new Thread(() -> SyncTest.incr());
        Thread threadTwo = new Thread(() -> SyncTest.incr());
        thread.start();
        threadTwo.start();
        thread.join();
        threadTwo.join();
        //最终打印结果是20000，如果不使用synchronized修饰，就会导致线程安全问题，输出不确定结果
        System.out.println(SyncTest.getJ());
    }
}
```
`Java`的静态资源可以直接通过类名调用，静态资源不属于任何实例对象，它只属于`Class`对象，每个`Class`在`J V M`中只有唯一的一个`Class`对象，所以同步静态函数会以`Class`对象作为锁，后续获取锁、释放锁流程都一致。
<a name="GxS7Y"></a>
## 代码块
前面介绍的普通函数与静态函数粒度都比较大，以整个函数为范围锁定，现在想把范围缩小、灵活配置，就需要使用**代码块**了，使用`{}`符号定义范围给`Synchronized`修饰。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254470-393c16fe-b9d9-47e9-a1d7-867b4f5638ac.png#align=left&display=inline&height=124&originHeight=124&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />下面代码中定义了`syncDbData`函数，`syncDbData`是一个伪同步数据的函数，耗时`2`秒，并且逻辑不涉及**共享资源读写操作**（**非临界区**），另外还有两个函数`incr`与`incrTwo`，都是在自增逻辑前执行了`syncDbData`函数，只是使用`Synchronized`的姿势不同，一个是修饰在函数上，另一个是修饰在代码块上。
```java
public class SyncTest {
    private static int j = 0;
    /**
     * 同步库数据，比较耗时，代码资源不涉及共享资源读写操作。
     */
    public void syncDbData() {
        System.out.println("db数据开始同步------------");
        try {
            //同步时间需要2秒
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("db数据开始同步完成------------");
    }
    //自增方法
    public synchronized void incr() {
        //start--临界区代码
        //同步库数据
        syncDbData();
        for (int i = 0; i < 10000; i++) {
            j++;
        }
        //end--临界区代码
    }
    //自增方法
    public void incrTwo() {
        //同步库数据
        syncDbData();
        synchronized (this) {
            //start--临界区代码
            for (int i = 0; i < 10000; i++) {
                j++;
            }
            //end--临界区代码
        }
    }
    public int getJ() {
        return j;
    }
}
public class SyncMain {
    public static void main(String[] agrs) throws InterruptedException {
        //incr同步方法执行
        SyncTest syncTest = new SyncTest();
        Thread thread = new Thread(() -> syncTest.incr());
        Thread threadTwo = new Thread(() -> syncTest.incr());
        thread.start();
        threadTwo.start();
        thread.join();
        threadTwo.join();
        //最终打印结果是20000
        System.out.println(syncTest.getJ());
        //incrTwo同步块执行
        thread = new Thread(() -> syncTest.incrTwo());
        threadTwo = new Thread(() -> syncTest.incrTwo());
        thread.start();
        threadTwo.start();
        thread.join();
        threadTwo.join();
        //最终打印结果是40000
        System.out.println(syncTest.getJ());
    }
}
```
先看看`incr`同步方法执行，流程和前面没区别，只是`Synchronized`锁定的范围太大，把`syncDbData()`也纳入**临界区**中，多线程场景执行，会有性能上的浪费，因为`syncDbData()`完全可以让多线程**并行**或**并发**执行。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254503-ddd87fb9-78bb-4f8d-9a33-8867f35d66d5.png#align=left&display=inline&height=1108&originHeight=1108&originWidth=970&size=0&status=done&style=shadow&width=970)<br />通过代码块的方式，来缩小范围，定义正确的**临界区**，提升性能，目光转到`incrTwo`同步块执行，`incrTwo`函数使用修饰代码块的方式同步，只对自增代码段进行锁定。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254456-57fab70a-7fb5-48ab-8b11-d9dc732853c2.png#align=left&display=inline&height=987&originHeight=987&originWidth=966&size=0&status=done&style=shadow&width=966)<br />代码块同步方式除了灵活控制范围外，还能做线程间的协同工作，因为`Synchronized ()`括号中能接收任何对象作为锁，所以可以通过`Object`的`wait、notify、notifyAll`等函数，做多线程间的通信协同（这里不对线程通信协同做展开，主角是`Synchronized`，而且也不推荐去用这些方法，因为`LockSupport`工具类会是更好的选择）。

- **wait：当前线程暂停，释放锁**
- **notify：释放锁，唤醒调用了wait的线程（如果有多个随机唤醒一个）**
- **notifyAll：释放锁，唤醒调用了wait的所有线程**
<a name="FGSk8"></a>
# Synchronized原理
```java
public class SyncTest {
    private static int j = 0;
    /**
     * 同步库数据，比较耗时，代码资源不涉及共享资源读写操作。
     */
    public void syncDbData() {
        System.out.println("db数据开始同步------------");
        try {
            //同步时间需要2秒
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("db数据开始同步完成------------");
    }
    //自增方法
    public synchronized void incr() {
        //start--临界区代码
        //同步库数据
        syncDbData();
        for (int i = 0; i < 10000; i++) {
            j++;
        }
        //end--临界区代码
    }
    //自增方法
    public void incrTwo() {
        //同步库数据
        syncDbData();
        synchronized (this) {
            //start--临界区代码
            for (int i = 0; i < 10000; i++) {
                j++;
            }
            //end--临界区代码
        }
    }
    public int getJ() {
        return j;
    }
}
```
为了探究`Synchronized`原理，对上面的代码进行反编译，输出反编译后结果，看看底层是如何实现的（**环境Java 11、win 10系统**）。
```
只截取了incr与incrTwo函数内容
        
  public synchronized void incr();
    Code:
       0: aload_0                                         
       1: invokevirtual #11                 // Method syncDbData:()V 
       4: iconst_0                          
       5: istore_1                          
       6: iload_1                                     
       7: sipush        10000               
      10: if_icmpge     27
      13: getstatic     #12                 // Field j:I
      16: iconst_1
      17: iadd
      18: putstatic     #12                 // Field j:I
      21: iinc          1, 1
      24: goto          6
      27: return
  public void incrTwo();    
    Code:
       0: aload_0
       1: invokevirtual #11                 // Method syncDbData:()V
       4: aload_0
       5: dup
       6: astore_1
       7: monitorenter                     //获取锁
       8: iconst_0
       9: istore_2
      10: iload_2
      11: sipush        10000
      14: if_icmpge     31
      17: getstatic     #12                 // Field j:I
      20: iconst_1
      21: iadd
      22: putstatic     #12                 // Field j:I
      25: iinc          2, 1
      28: goto          10
      31: aload_1
      32: monitorexit                      //正常退出释放锁 
      33: goto          41
      36: astore_3
      37: aload_1
      38: monitorexit                      //异步退出释放锁    
      39: aload_3
      40: athrow
      41: return
```
**ps:对上面指令感兴趣的读者，可以百度或google一下“JVM 虚拟机字节码指令表”**<br />先看`incrTwo`函数，`incrTwo`是代码块方式同步，在反编译后的结果中，发现存在`monitorenter`与`monitorexit`指令（**获取锁、释放锁**）。<br />`monitorenter`指令插入到同步代码块的开始位置，`monitorexit`指令插入到同步代码块的结束位置，`J V M`需要保证每一个 `monitorenter`都有`monitorexit`与之对应。<br />**任何对象**都有一个监视器锁（`monitor`）关联，线程执行`monitorenter`指令时尝试获取`monitor`的所有权。

- **如果`monitor`的进入数为`0`，则该线程进入`monitor`，然后将进入数设置为`1`，该线程为`monitor`的所有者**
- **如果线程已经占有该`monitor`，重新进入，则`monitor`的进入数加`1`**
- **线程执行`monitorexit`，`monitor`的进入数-1，执行过多少次`monitorenter`，最终要执行对应次数的`monitorexit`**
- **如果其他线程已经占用`monitor`，则该线程进入阻塞状态，直到`monitor`的进入数为0，再重新尝试获取`monitor`的所有权**

回过头看`incr`函数，`incr`是普通函数方式同步，虽然在反编译后的结果中没有看到`monitorenter`与`monitorexit`指令，但是实际执行的流程与`incrTwo`函数一样，通过`monitor`来执行，只不过它是一种隐式的方式来实现，最后放一张流程图。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254553-ef4e965b-5168-4dc7-a259-5ed83646bcb3.png#align=left&display=inline&height=471&originHeight=471&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="tcjIO"></a>
# Synchronized优化
`Jdk 1.5`以后对`Synchronized`关键字做了各种的优化，经过优化后`Synchronized`已经变得原来越快了，这也是为什么官方建议使用`Synchronized`的原因，具体的优化点如下。

- **锁粗化**
- **锁消除**
- **锁升级**
<a name="cqDZA"></a>
## 锁粗化
**互斥的临界区**范围应该尽可能小，这样做的目的是为了使同步的操作数量尽可能缩小，缩短阻塞时间，如果存在锁竞争，那么等待锁的线程也能尽快拿到锁。<br />但是加锁解锁也需要消耗资源，如果存在一系列的连续加锁解锁操作，可能会导致不必要的性能损耗，**锁粗化**就是将「**多个连续的加锁、解锁操作连接在一起**」，扩展成一个范围更大的锁，避免频繁的加锁解锁操作。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254765-5b4952b5-6da6-4f4b-834d-15864100e084.png#align=left&display=inline&height=386&originHeight=386&originWidth=1050&size=0&status=done&style=shadow&width=1050)<br />`JVM`会检测到一连串的操作都对同一个对象加锁（`for`循环`10000`次执行`j++`，没有锁粗化就要进行`10000`次加锁/解锁），此时`J V M`就会将加锁的范围粗化到这一连串操作的外部（比如`for`循环体外），使得这一连串操作只需要加一次锁即可。
<a name="gNfkx"></a>
## 锁消除
`Java`虚拟机在`JIT`编译时(**可以简单理解为当某段代码即将第一次被执行时进行编译，又称即时编译**)，通过对运行上下文的扫描，经过逃逸分析（**对象在函数中被使用，也可能被外部函数所引用，称为函数逃逸**），去除不可能存在共享资源竞争的锁，通过这种方式消除没有必要的锁，可以节省毫无意义的时间消耗。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406255046-0049cbee-2274-4f56-8b23-fe29c40c1a50.png#align=left&display=inline&height=348&originHeight=348&originWidth=1008&size=0&status=done&style=shadow&width=1008)<br />代码中使用`Object`作为锁，但是`Object`对象的生命周期只在`incrFour()`函数中，并不会被其他线程所访问到，所以在`J I T`编译阶段就会被优化掉（**此处的`Object`属于没有逃逸的对象**）。
<a name="0A1hm"></a>
## 锁升级
`Java`中每个对象都拥有对象头，对象头由`Mark World` 、指向类的指针、以及数组长度三部分组成，只需要关心`Mark World` 即可，`Mark World`  记录了对象的`HashCode`、分代年龄和锁标志位信息。<br />**Mark World简化结构**

| 锁状态 | 存储内容 | 锁标记 |
| :--- | :--- | :--- |
| 无锁 | 对象的hashCode、对象分代年龄、是否是偏向锁（0） | 01 |
| 偏向锁 | 偏向线程ID、偏向时间戳、对象分代年龄、是否是偏向锁（1） | 01 |
| 轻量级锁 | 指向栈中锁记录的指针 | 00 |
| 重量级锁 | 指向互斥量（重量级锁）的指针 | 10 |

锁的升级变化，体现在锁对象的对象头`Mark World`部分，也就是说`Mark World`的内容会随着锁升级而改变。<br />`Java1.5`以后为了减少获取锁和释放锁带来的性能消耗，引入了**偏向锁**和**轻量级锁**，`Synchronized`的升级顺序是 「**无锁-->偏向锁-->轻量级锁-->重量级锁，只会升级不会降级**」
<a name="z2T2J"></a>
### 偏向锁
在大多数情况下，锁总是由同一线程多次获得，不存在多线程竞争，所以出现了偏向锁，其目标就是在只有一个线程执行同步代码块时，降低获取锁带来的消耗，提高性能（**可以通过J V M参数关闭偏向锁：-XX:-UseBiasedLocking=false，关闭之后程序默认会进入轻量级锁状态**）。<br />线程执行同步代码或方法前，线程只需要判断对象头的`Mark Word`中线程`ID`与当前线程`ID`是否一致，如果一致直接执行同步代码或方法，具体流程如下<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254975-1be7924d-d0dd-4db7-a8e2-6ef686797e36.png#align=left&display=inline&height=842&originHeight=842&originWidth=1080&size=0&status=done&style=shadow&width=1080)

- **无锁状态，存储内容「是否为偏向锁（`0`）」，锁标识位`01`**
   - **`CAS`设置当前线程ID到`Mark Word`存储内容中**
   - **是否为偏向锁`0` => 是否为偏向锁`1`**
   - **执行同步代码或方法**
- **偏向锁状态，存储内容「是否为偏向锁（`1`）、线程ID」，锁标识位`01`**
   - **对比线程`ID`是否一致，如果一致执行同步代码或方法，否则进入下面的流程**
   - **如果不一致，`CAS`将`Mark Word`的线程`ID`设置为当前线程`ID`，设置成功，执行同步代码或方法，否则进入下面的流程**
   - **`CAS`设置失败，证明存在多线程竞争情况，触发撤销偏向锁，当到达全局安全点，偏向锁的线程被挂起，偏向锁升级为轻量级锁，然后在安全点的位置恢复继续往下执行。**
<a name="cbFGJ"></a>
### 轻量级锁
轻量级锁考虑的是竞争锁对象的线程不多，持有锁时间也不长的场景。因为阻塞线程需要`C P U`从用户态转到内核态，代价较大，如果刚刚阻塞不久这个锁就被释放了，那这个代价就有点得不偿失，所以干脆不阻塞这个线程，让它自旋一段时间等待锁释放。<br />当前线程持有的锁是偏向锁的时候，被另外的线程所访问，偏向锁就会升级为轻量级锁，其他线程会通过自旋的形式尝试获取锁，不会阻塞，从而提高性能。轻量级锁的获取主要有两种情况：① 当关闭偏向锁功能时；② 多个线程竞争偏向锁导致偏向锁升级为轻量级锁。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406254849-8765a201-b5c7-4f30-9090-7e4c06dc2e85.png#align=left&display=inline&height=935&originHeight=935&originWidth=1080&size=0&status=done&style=shadow&width=1080)

- **无锁状态，存储内容「是否为偏向锁（`0`）」，锁标识位`01`**
   - **关闭偏向锁功能时**
   - **`CAS`设置当前线程栈中锁记录的指针到`Mark Word`存储内容**
   - **锁标识位设置为`00`**
   - **执行同步代码或方法**
   - **释放锁时，还原来`Mark Word`内容**
- **轻量级锁状态，存储内容「线程栈中锁记录的指针」，锁标识位`00`（存储内容的线程是指"持有轻量级锁的线程"）**
   - **`CAS`设置当前线程栈中锁记录的指针到`Mark Word`存储内容，设置成功获取轻量级锁，执行同步块代码或方法，否则执行下面的逻辑**
   - **设置失败，证明多线程存在一定竞争，线程自旋上一步的操作，自旋一定次数后还是失败，轻量级锁升级为重量级锁**
   - **`Mark Word`存储内容替换成重量级锁指针，锁标记位`10`**
<a name="woIY1"></a>
### 重量级锁
轻量级锁膨胀之后，就升级为重量级锁，重量级锁是依赖操作系统的`MutexLock`（**互斥锁**）来实现的，需要从用户态转到内核态，这个成本非常高，这就是为什么`Java1.6`之前`Synchronized`效率低的原因。<br />升级为重量级锁时，锁标志位的状态值变为`10`，此时`Mark Word`中存储内容的是重量级锁的指针，等待锁的线程都会进入阻塞状态，下面是简化版的锁升级过程。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618406255225-d2476023-3928-4ffb-bfbc-ede446a79cd4.png#align=left&display=inline&height=297&originHeight=297&originWidth=1080&size=0&status=done&style=shadow&width=1080)
