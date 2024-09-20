JavaSynchronized
<a name="Msswx"></a>
## 一、`synchronized`介绍
`synchronized`中文意思是同步，也称之为”同步锁“。<br />`synchronized`的作用是保证在同一时刻， 被修饰的代码块或方法只会有一个线程执行，以达到保证并发安全的效果。<br />`synchronized`是Java中解决并发问题的一种最常用的方法，也是最简单的一种方法。<br />在JDK1.5之前`synchronized`是一个重量级锁，相对于j.u.c.Lock，它会显得那么笨重，随着Javs SE 1.6对`synchronized`进行的各种优化后，`synchronized`并不会显得那么重了。<br />`synchronized`的作用主要有三个：

- **原子性：** 确保线程互斥地访问同步代码；
- **可见性：** 保证共享变量的修改能够及时可见，其实是通过Java内存模型中的 “ 对一个变量unlock操作之前，必须要同步到主内存中；如果对一个变量进行lock操作，则将会清空工作内存中此变量的值，在执行引擎使用此变量前，需要重新从主内存中load操作或assign操作初始化变量值 ” 来保证的；
- **有序性：** 有效解决重排序问题，即 “一个unlock操作先行发生(happen-before)于后面对同一个锁的lock操作”；
<a name="ittEd"></a>
## 二、`synchronized`的使用
synchronized的3种使用方式：

- **修饰实例方法：** 作用于当前实例加锁
- **修饰静态方法：** 作用于当前类对象加锁
- **修饰代码块：** 指定加锁对象，对给定对象加锁
<a name="uVmWr"></a>
### 1、修饰方法
Synchronized修饰一个方法很简单，就是在方法的前面加`synchronized`，`synchronized`修饰方法和修饰一个代码块类似，只是作用范围不一样，修饰代码块是大括号括起来的范围，而修饰方法范围是整个函数。
<a name="joGuo"></a>
#### 方法一：修饰的是一个方法
```java
public synchronized void method(){
    // todo
}
```
<a name="XEH5r"></a>
#### 方法二：修饰的是一个代码块
```java
public void method(){
    synchronized(this) {
        // todo
    }
}
```
方法一与方法二是等价的，都是锁定了整个方法时的内容。<br />`synchronized`关键字不能继承。虽然可以使用`synchronized`来定义方法，但`synchronized`并不属于方法定义的一部分，因此，`synchronized`关键字不能被继承。<br />在子类方法中加上`synchronized`关键字
```java
class Parent {
    public synchronized void method() { }
}
class Child extends Parent {
    public synchronized void method() { }
}
```
在子类方法中调用父类的同步方法
```java
class Parent {
    public synchronized void method() {   }
}
class Child extends Parent {
    public void method() { super.method();   }
}
```
注意：

- 在定义接口方法时不能使用`synchronized`关键字。
- 构造方法不能使用`synchronized`关键字，但可以使用`synchronized`代码块来进行同步。

当有一个明确的对象作为锁时，就可以用类似下面这样的方式写程序：
```java
public void method3(SomeObject obj){
   //obj 锁定的对象
   synchronized(obj){
      // todo
   }
}
```
当没有明确的对象作为锁，只是想让一段代码同步时，可以创建一个特殊的对象来充当锁：
```java
class Test implements Runnable{
    private byte[] lock = new byte[0];  // 特殊的instance变量
    public void method()
    {
        synchronized(lock) {
            // todo 同步代码块
        }
    }

    public void run() {

    }
}
```
<a name="M4148"></a>
### 2、修饰一个静态方法
`synchronized`也可修饰一个静态方法，用法如下：
```java
public synchronized static void method() {
    // todo
}
```
<a name="l10Hd"></a>
### 3、修饰一个类
Synchronized还可作用于一个类，用法如下：
```java
class ClassName {
    public void method() {
        synchronized(ClassName.class) {
            // todo
        }
    }
}
```
使用总结

- 无论`synchronized`关键字加在方法上还是对象上，如果它作用的对象是非静态的，则它取得的锁是对象；如果`synchronized`作用的对象是一个静态方法或一个类，则它取得的锁是对类，该类所有的对象同一把锁。
- 每个对象只有一个锁（lock）与之相关联，谁拿到这个锁谁就可以运行它所控制的那段代码。
- 实现同步是要很大的系统开销作为代价的，甚至可能造成死锁，所以尽量避免无谓的同步控制。
<a name="F9CXa"></a>
## 三、`synchronized`的底层实现
谈`synchronized`的底层实现，就不得不谈数据在JVM内存的存储：Java对象头，以及Monitor对象监视器。
<a name="euxjc"></a>
### 对象头
在JVM中，对象在内存中的布局分为三块区域：对象头、实例数据和对齐填充。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680833960384-131faf86-a55b-45b7-a89f-1a1dd191fd3c.jpeg)

- **实例数据：** 存放类的属性数据信息，包括父类的属性信息；
- **对齐填充：** 由于虚拟机要求 对象起始地址必须是8字节的整数倍。填充数据不是必须存在的，仅仅是为了字节对齐；
- **对象头：** Java对象头一般占有2个机器码（在32位虚拟机中，1个机器码等于4字节，也就是32bit，在64位虚拟机中，1个机器码是8个字节，也就是64bit），但是如果对象是数组类型，则需要3个机器码，因为JVM虚拟机可以通过Java对象的元数据信息确定Java对象的大小，但是无法从数组的元数据来确认数组的大小，所以用一块来记录数组长度。

`synchronized`用的锁就是存在Java对象头里的，那么什么是Java对象头呢？Hotspot虚拟机的对象头主要包括两部分数据：Mark Word（标记字段）、Class Pointer（类型指针）。<br />其中 Class Pointer是对象指向它的类元数据的指针，虚拟机通过这个指针来确定这个对象是哪个类的实例，Mark Word用于存储对象自身的运行时数据，它是实现轻量级锁和偏向锁的关键。
<a name="fruYM"></a>
### 监视器（`Monitor`）
任何一个对象都有一个`Monitor`与之关联，当且一个`Monitor`被持有后，它将处于锁定状态。<br />`synchronized`在JVM里的实现都是 基于进入和退出`Monitor`对象来实现方法同步和代码块同步，虽然具体实现细节不一样，但是都可以通过成对的`MonitorEnter`和`MonitorExit`指令来实现。

- `**MonitorEnter**`**指令：** 插入在同步代码块的开始位置，当代码执行到该指令时，将会尝试获取该对象`Monitor`的所有权，即尝试获得该对象的锁；
- `**MonitorExit**`**指令：** 插入在方法结束处和异常处，JVM保证每个`MonitorEnter`必须有对应的`MonitorExit`；

那什么是`Monitor`？可以把它理解为 一个同步工具，也可以描述为 一种同步机制，它通常被描述为一个对象。<br />与一切皆对象一样，所有的Java对象是天生的`Monitor`，每一个Java对象都有成为`Monitor`的潜质，因为在Java的设计中 ，每一个Java对象自打娘胎里出来就带了一把看不见的锁，它叫做内部锁或者`Monitor`锁。<br />也就是通常说`Synchronized`的对象锁，`MarkWord`锁标识位为10，其中指针指向的是`Monitor`对象的起始地址。在Java虚拟机（HotSpot）中，`Monitor`是由`ObjectMonitor`实现的。
<a name="rW0gR"></a>
## 四、synchronized 锁的升级顺序
锁解决了数据的安全性，但是同样带来了性能的下降。hotspot 虚拟机的作者经过调查发现，大部分情况下，加锁的代码不仅仅不存在多线程竞争，而且总是由同一个线程多次获得。所以基于这样一个概率。<br />synchronized 在JDK1.6 之后做了一些优化，为了减少获得锁和释放锁来的性能开销，引入了偏向锁、轻量级锁、自旋锁、重量级锁，锁的状态根据竞争激烈的程度从低到高不断升级。<br />锁主要存在四种状态，依次是：无锁状态、偏向锁状态、轻量级锁状态、重量级锁状态，锁可以从偏向锁升级到轻量级锁，再升级的重量级锁。但是锁的升级是单向的，也就是说只能从低到高升级，不会出现锁的降级。而且这个过程就是开销逐渐加大的过程。

| 锁 | 优点 | 缺点 | 适用场景 |
| --- | --- | --- | --- |
| 偏向锁 | 加锁和解锁不需要额外的消耗，和执行非同步方法比仅存在纳秒级的差距。 | 如果线程间存在锁竞争，会带来额外的锁撤销的消耗。 | 适用于只有一个线程访问同步块场景。 |
| 轻量级锁 | 竞争的线程不会阻塞，提高了程序的响应速度。 | 如果始终得不到锁竞争的线程使用自旋会消耗CPU。 | 追求响应时间。同步块执行速度非常快。 |
| 重量级锁 | 线程竞争不使用自旋，不会消耗CPU。 | 线程阻塞，响应时间缓慢。 | 追求吞吐量。同步块执行速度较长。 |

