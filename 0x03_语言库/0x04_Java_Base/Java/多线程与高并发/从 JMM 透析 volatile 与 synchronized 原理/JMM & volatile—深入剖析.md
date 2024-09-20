JMM volatile
<a name="OpdyS"></a>
## 一、现代计算机理论模型与工作原理
<a name="Nc2tv"></a>
### 1.1 冯诺依曼计算机模型
来一起回顾一下大学计算机基础，现代计算机模型——冯诺依曼计算机模型，是一种将程序指令存储器和数据存储器合并在一起的计算机设计概念结构。依据冯·诺伊曼结构设计出的计算机称做冯.诺依曼计算机，又称存储程序计算机。<br />计算机在运行指令时，会从存储器中一条条指令取出，通过译码（控制器），从存储器中取出数据，然后进行指定的运算和逻辑等操作，然后再按地址把运算结果返回内存中去。<br />接下来，再取出下一条指令，在控制器模块中按照规定操作。依此进行下去。直至遇到停止指令。<br />程序与数据一样存贮，按程序编排的顺序，一步一步地取出指令，自动地完成指令规定的操作是计算机最基本的工作模型。这一原理最初是由美籍匈牙利数学家冯.诺依曼于1945年提出来的，故称为冯.诺依曼计算机模型。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800371-63d06bbe-0d7c-492b-a02a-5ae8f10f2bef.webp#clientId=u075bc741-49d6-4&from=paste&id=u4ccbfd8d&originHeight=449&originWidth=861&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue760c858-8133-4f15-acbd-367ad650af1&title=)

- **五大核心组成部分：**
1. **运算器：**顾名思义，主要进行计算，算术运算、逻辑运算等都由它来完成。
2. **存储器：**这里存储器只是内存，不包括内存，用于存储数据、指令信息。实际就是计算机中内存（RAM）
3. **控制器：**控制器是是所有设备的调度中心，系统的正常运行都是有它来调配。CPU包含控制器和运算器。
4. **输入设备：**负责向计算机中输入数据，如鼠标、键盘等。
5. **输出设备：**负责输出计算机指令执行后的数据，如显示器、打印机等。
- **现代计算机硬件结构：**

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800940-37206d8c-fa38-4406-9b06-b2a5a1b96900.webp#clientId=u075bc741-49d6-4&from=paste&id=u996878d3&originHeight=484&originWidth=871&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f1e55e0-db35-45fd-bea5-2732646fa32&title=)<br />图中结构可以关注两个重点：<br />**I/O总线：**所有的输入输出设备都与I/O总线对接，保存内存条、USB、显卡等等，就好比一条公路，所有的车都在上面行驶，但是毕竟容量有限，IO频繁或者数据较大时就会引起“堵车”<br />**CPU：**当CPU运行时最直接也最快的获取存储的是寄存器，然后会通过CPU缓存从L1->L2->L3寻找，如果缓存都没有则通过I/O总线到内存中获取，内存中获取到之后会依次刷入L3->L2->L1->寄存器中。现代计算机上CPU一般都是 1.xG、2.xG的赫兹，而内存的速度只有每秒几百M，所以为了为了不让内存拖后腿也为了尽量减少I/O总线的交互，才有了CPU缓存的存在，CPU型号的不同有的是两级缓存，有的是三级缓存，运行速度对比：**寄存器 > L1 > L2 > L3 > 内存条**。
<a name="y8mgD"></a>
### 1.2 CPU多级缓存和内存
CPU缓存即高速缓冲存储器，是位于CPU与主内存之间容量很小但速度很高的存储器。CPU直接从内存中存取数据后会保存到缓存中，当CPU再次使用时可以直接从缓存中调取。如果有数据修改，也是先修改缓存中的数据，然后经过一段时间之后才会重新写回主内存中。<br />CPU缓存最小单元是缓存行（cache line），目前主流计算机的缓存行大小为64Byte，CPU缓存也会有LRU、Random等缓存淘汰策略。CPU的三级缓存为多个CPU共享的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800644-1c563e90-5ddb-4476-a647-c06730f099c6.webp#clientId=u075bc741-49d6-4&from=paste&id=ubf9b9b3b&originHeight=441&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u39e7abdf-a08b-4e7a-80e8-cdf701c472d&title=)

- **CPU读取数据时的流程：**

（1）先读取寄存器的值，如果存在则直接读取<br />（2）再读取L1，如果存在则先把cache行锁住，把数据读取出来，然后解锁<br />（3）如果L1没有则读取L2，如果存在则先将L2中的cache行加锁，然后将数据拷贝到L1，再执行读L1的过程，最后解锁<br />（4）如果L2没有则读取L3，同上先加锁，再往上层依次拷贝、加锁，读取到之后依次解锁<br />（5）如果L3也没有数据则通知内存控制器占用总线带宽，通知内存加锁，发起内存读请求，等待回应，回应数据保存到L3（如果没有就到L2），再从L3/2到L1，再从L1到CPU，之后解除总线锁定。

- **缓存一致性问题：**

在多处理器系统中，每个处理器都有自己的缓存，于是也引入了新的问题：缓存一致性。当多个处理器的运算任务都涉及同一块主内存区域时，将可能导致各自的缓存数据不一致的情况。为了解决一致性的问题，需要各个处理器访问缓存时都遵循一些协议，在读写时要根据协议来进行操作，这类协议有MSI、MESI、MOSI等等。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800291-d187fcea-c653-4029-9a58-b653ae0970a3.webp#clientId=u075bc741-49d6-4&from=paste&id=u21a12eb4&originHeight=542&originWidth=542&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9506b13e-cf76-4a77-a8e8-6d84e4977b9&title=)
<a name="y04Iy"></a>
### 1.3 MESI缓存一致性协议
缓存一致性协议中应用最广泛的就是MESI协议。主要原理是 CPU 通过总线**嗅探机制**（监听）可以感知数据的变化从而将自己的缓存里的数据失效，缓存行中具体的几种状态如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800338-099f3b45-2509-47b1-a741-ce1182283478.webp#clientId=u075bc741-49d6-4&from=paste&id=ue351a108&originHeight=322&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue5f1c06c-ef4a-4227-8e8f-8c31495c105&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801091-49018a13-0b42-4243-8a16-eb1dc753b4f7.webp#clientId=u075bc741-49d6-4&from=paste&id=u9dcea505&originHeight=581&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2d0557b8-b2fc-4bfc-8396-80995ee2b28&title=)<br />以上图为例，假设主内存中有一个变量x=1，CPU1和CPU2中都会读写，MESI的工作流程为：<br />（1）假设CPU1需要读取x的值，此时CPU1从主内存中读取到缓存行后的状态为E，代表只有当前缓存中独占数据，并利用CPU嗅探机制监听总线中是否有其他缓存读取x的操作。<br />（2）此时如果CPU2也需要读取x的值到缓存行，则在CPU2中缓存行的状态为S，表示多个缓存中共享，同时CPU1由于嗅探到CPU2也缓存了x所以状态也变成了S。并且CPU1和CPU2会同时嗅探是否有另缓存失效获取独占缓存的操作。<br />（3）当CPU1有写入操作需要修改x的值时，CPU1中缓存行的状态变成了M。<br />（4）CPU2由于嗅探到了CPU1的修改操作，则会将CPU2中缓存的状态变成 I 无效状态。<br />（5）此时CPU1中缓存行的状态重新变回独占E的状态，CPU2要想读取x的值的话需要重新从主内存中读取。
<a name="d9iGa"></a>
## 二、JMM模型
<a name="Oal5p"></a>
### 2.1  Java 线程与系统内核的关系
Java线程在JDK1.2之前，是基于称为“绿色线程”（Green Threads）的用户线程实现的，而在JDK1.2中，线程模型替换为基于操作系统原生线程模型来实现。因此，在目前的JDK版本中，操作系统支持怎样的线程模型，在很大程度上决定了Java虚拟机的线程是怎样映射的，这点在不同的平台上没有办法达成一致，虚拟机规范中也并未限定Java线程需要使用哪种线程模型来实现。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800808-4de7ee9a-3267-4003-94b4-96e786561893.webp#clientId=u075bc741-49d6-4&from=paste&id=u65312a96&originHeight=511&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue7fc7f9f-aa84-472e-9036-7279755e13b&title=)<br />**用户线程：**指不需要内核支持而在用户程序中实现的线程，其不依赖于操作系统核心，应用进程利用线程库提供创建、同步、调度和管理线程的函数来控制用户线程。另外，用户线程是由应用进程利用线程库创建和管理，不依赖于操作系统核心。不需要用户态/核心态切换，速度快。操作系统内核不知道多线程的存在，因此一个线程阻塞将使得整个进程（包括它的所有线程）阻塞。由于这里的处理器时间片分配是以进程为基本单位，所以每个线程执行的时间相对减少。<br />**内核线程： **线程的所有管理操作都是由操作系统内核完成的。内核保存线程的状态和上下文信息，当一个线程执行了引起阻塞的系统调用时，内核可以调度该进程的其他线程执行。在多处理器系统上，内核可以分派属于同一进程的多个线程在多个处理器上运行，提高进程执行的并行度。由于需要内核完成线程的创建、调度和管理，所以和用户级线程相比这些操作要慢得多，但是仍然比进程的创建和管理操作要快。<br />基于线程的区别，可以引出java内存模型的结构。
<a name="PxNYc"></a>
### 2.2  什么是 JMM 模型
Java内存模型(Java Memory Model简称JMM)是一种抽象的概念，并不真实存在，它描述的是一组规则或规范，通过这组规范定义了程序中各个变量（包括实例字段，静态字段和构成数组对象的元素）的访问方式。<br />为了屏蔽掉各种硬件和操作系统的内存访问差异，以实现让Java程序在各种平台下都能达到一致的并发效果，JMM规范了Java虚拟机与计算机内存是如何协同工作的：JVM运行程序的实体是线程，而每个线程创建时JVM都会为其创建一个工作内存(有些地方称为栈空间)，用于存储线程私有的数据，而Java内存模型中规定所有变量都存储在主内存，主内存是共享内存区域，所有线程都可以访问，但线程对变量的操作(读取赋值等)必须在工作内存中进行，首先要将变量从主内存拷贝的自己的工作内存空间，然后对变量进行操作，操作完成后再将变量写回主内存，不能直接操作主内存中的变量，工作内存中存储着主内存中的变量副本拷贝。工作内存是每个线程的私有数据区域，因此不同的线程间无法访问对方的工作内存，线程间的通信(传值)必须通过主内存来完成。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800915-953c7d01-1181-43ae-bf42-867d0a438e13.webp#clientId=u075bc741-49d6-4&from=paste&id=udfe9f201&originHeight=441&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f8f9588-6564-4ab1-ae59-b33f8812141&title=)<br />**主内存**<br />主要存储的是Java实例对象，所有线程创建的实例对象都存放在主内存中，不管该实例对象是成员变量还是方法中的本地变量(也称局部变量)，当然也包括了共享的类信息、常量、静态变量。由于是共享数据区域，从某个程度上讲应该包括了JVM中的堆和方法区。多条线程对同一个变量进行访问可能会发生线程安全问题。<br />**工作内存**<br />主要存储当前方法的所有本地变量信息(工作内存中存储着主内存中的变量副本拷贝)，每个线程只能访问自己的工作内存，即线程中的本地变量对其它线程是不可见的，就算是两个线程执行的是同一段代码，它们也会各自在自己的工作内存中创建属于当前线程的本地变量，当然也包括了字节码行号指示器、相关Native方法的信息。所以则应该包括JVM中的程序计数器、虚拟机栈以及本地方法栈。注意由于工作内存是每个线程的私有数据，线程间无法相互访问工作内存，因此存储在工作内存的数据不存在线程安全问题。
<a name="VBk5n"></a>
### 2.3 JMM 详解
需要注意的是JMM只是一种抽象的概念，一组规范，并不实际存在。对于真正的计算机硬件来说，计算机内存只有寄存器、缓存内存、主内存的概念。不管是工作内存的数据还是主内存的数据，对于计算机硬件来说都会存储在计算机主内存中，当然也有可能存储到CPU缓存或者寄存器中，因此总体上来说，Java内存模型和计算机硬件内存架构是一个相互交叉的关系，是一种抽象概念划分与真实物理硬件的交叉。<br />工作内存同步到主内存之间的实现细节，JMM定义了以下八种操作：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541800976-02eab851-4469-4e4d-9e3f-76a40a4d787a.webp#clientId=u075bc741-49d6-4&from=paste&id=u99ac7dde&originHeight=496&originWidth=452&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uabad1e04-0677-4c5e-8ea7-ecea8f26055&title=)<br />如果要把一个变量从主内存中复制到工作内存中，就需要按顺序地执行read和load操作，如果把变量从工作内存中同步到主内存中，就需要按顺序地执行store和write操作。但Java内存模型只要求上述操作必须按顺序执行，而没有保证必须是连续执行。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801461-bc193fa0-24df-4ebb-99fc-8866340720ad.webp#clientId=u075bc741-49d6-4&from=paste&id=uf3d42fbb&originHeight=420&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0695bf01-4cdd-4b30-a1d9-c5b3227d053&title=)

- **同步规则分析**

（1）不允许一个线程无原因地（没有发生过任何assign操作）把数据从工作内存同步回主内存中。<br />（2）一个新的变量只能在主内存中诞生，不允许在工作内存中直接使用一个未被初始化（load或者assign）的变量。即就是对一个变量实施use和store操作之前，必须先自行assign和load操作。<br />（3）一个变量在同一时刻只允许一条线程对其进行lock操作，但lock操作可以被同一线程重复执行多次，多次执行lock后，只有执行相同次数的unlock操作，变量才会被解锁。lock和unlock必须成对出现。<br />（4）如果对一个变量执行lock操作，将会清空工作内存中此变量的值，在执行引擎使用这个变量之前需要重新执行load或assign操作初始化变量的值。<br />（5）如果一个变量事先没有被lock操作锁定，则不允许对它执行unlock操作；也不允许去unlock一个被其他线程锁定的变量。<br />（6）对一个变量执行unlock操作之前，必须先把此变量同步到主内存中（执行store和write操作）。
<a name="YfPRB"></a>
### 2.4 JMM 如何解决多线程并发引起的问题
多线程并发下存在：**原子性、可见性、有序性**三种问题。

- **原子性：**

**问题：**原子性指的是一个操作是不可中断的，即使是在多线程环境下，一个操作一旦开始就不会被其他线程影响。但是当线程运行的过程中，由于CPU上下文的切换，则线程内的多个操作并不能保证是保持原子执行。<br />**解决：**除了JVM自身提供的对基本数据类型读写操作的原子性外，可以通过 synchronized和Lock实现原子性。因为synchronized和Lock能够保证任一时刻只有一个线程访问该代码块。

- **可见性**

**问题：**之前分析过，程序运行的过程中是分工作内存和主内存，工作内存将主内存中的变量拷贝到副本中缓存，假如两个线程同时拷贝一个变量，但是当其中一个线程修改该值，另一个线程是不可见的，这种工作内存和主内存之间的数据同步延迟就会造成可见性问题。另外由于指令重排也会造成可见性的问题。<br />**解决：**volatile关键字保证可见性。当一个共享变量被volatile修饰时，它会保证修改的值立即被其他的线程看到，即修改的值立即更新到主存中，当其他线程需要读取时，它会去内存中读取新值。synchronized和Lock也可以保证可见性，因为它们可以保证任一时刻只有一个线程能访问共享资源，并在其释放锁之前将修改的变量刷新到内存中。<br />**有序性**<br />**问题：**在单线程下认为程序是顺序执行的，但是多线程环境下程序被编译成机器码的后可能会出现指令重排的现象，重排后的指令与原指令未必一致，则可能会造成程序结果与预期的不同。<br />**解决：**在Java里面，可以通过volatile关键字来保证一定的有序性。另外可以通过synchronized和Lock来保证有序性，很显然，synchronized和Lock保证每个时刻是有一个线程执行同步代码，相当于是让线程顺序执行同步代码，自然就保证了有序性。
<a name="v6bJr"></a>
## 三、volatile关键字
<a name="twEXL"></a>
### 3.1 volatile 的作用
volatile是 Java 虚拟机提供的轻量级的同步机制。volatile关键字有如下**两个作用：**

- 保证被volatile修饰的共享变量对所有线程总数可见，也就是当一个线程修改了一个被volatile修饰共享变量的值，新值总是可以被其他线程立即得知
- 禁止指令重排序优化
<a name="Cu46y"></a>
### 3.2 volatile 保证可见性
以下是一段多线程场景下存在可见性问题的程序。
```java
public class VolatileTest extends Thread {
    private int index = 0;
    private boolean flag = false;
 
    @Override
    public void run() {
        while (!flag) {
            index++;
        }
    }
 
    public static void main(String[] args) throws Exception {
        VolatileTest volatileTest = new VolatileTest();
        volatileTest.start();
 
        Thread.sleep(1000);
 
        // 模拟多次写入，并触发JIT
        for (int i = 0; i < 10000000; i++) {
            volatileTest.flag = true;
        }
        System.out.println(volatileTest.index);
    }
}
```
运行可以发现，当 volatileTest.index 输出打印之后程序仍然未停止，表示线程依然处于运行状态，子线程读取到的flag的值仍为false。
```java
private volatile boolean flag = false;
```
尝试给flag增加volatile关键字后程序可以正常结束， 则表示子线程读取到的flag值为更新后的true。<br />那么为什么volatile可以保证可见性呢？<br />可以尝试在JDK中下载hsdis-amd64.dll后使用参数-XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly 运行程序，可以看到程序被翻译后的汇编指令，发现增加volatile关键字后给flag赋值时汇编指令多了一段 "lock addl $0x0,(%rsp)"<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801530-eb5d7478-d923-4e0f-bcd5-e6a11999b8a1.webp#clientId=u075bc741-49d6-4&from=paste&id=u8377051a&originHeight=250&originWidth=754&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u601eb5eb-6c80-480e-bf86-4a258b1d18e&title=)<br />说明volatile保证了可见性正是这段lock指令起到的作用，查阅IA-32手册，可以得知该指令的主要作用：

- 锁总线，其它CPU对内存的读写请求都会被阻塞，直到锁释放，不过实际后来的处理器都采用锁缓存替代锁总线，因为锁总线的开销比较大，锁总线期间其他CPU没法访问内存。
- lock后的写操作会回写已修改的数据，同时让其它CPU相关缓存行失效，从而重新从主存中加载最新的数据。
- 不是内存屏障却能完成类似内存屏障的功能，阻止屏障两遍的指令重排序。
<a name="LrxgW"></a>
### 3.3 volatile 禁止指令重排
Java 语言规范规定JVM线程内部维持顺序化语义。即只要程序的最终结果与它顺序化情况的结果相等，那么指令的执行顺序可以与代码顺序不一致，此过程叫指令的重排序。指令重排序的意义是什么？<br />JVM能根据处理器特性（CPU多级缓存系统、多核处理器等）适当的对机器指令进行重排序，使机器指令能更符合CPU的执行特性，最大限度的发挥机器性能。<br />以下是源代码到最终执行的指令集的示例图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801640-5a112cbd-41af-494b-90be-ee61a80982ca.webp#clientId=u075bc741-49d6-4&from=paste&id=ucf2e15fe&originHeight=51&originWidth=841&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua32d4a0b-516e-4847-a291-6b48c84c9d9&title=)<br />as-if-serial原则：不管怎么重排序，单线程程序下编译器和处理器不能对存在数据依赖关系的操作做重排序。但是，如果操作之间不存在数据依赖关系，这些操作就可能被编译器和处理器重排序。<br />下面是一段经典的发生指令重排导致结果预期不符的例子：
```java
public class VolatileTest {
 
    int a, b, x, y;
 
    public boolean test() throws InterruptedException {
        a = b = 0;
        x = y = 0;
        Thread t1 = new Thread(() -> {
            a = 1;
            x = b;
        });
        Thread t2 = new Thread(() -> {
            b = 1;
            y = a;
        });
        t1.start();
        t2.start();
        t1.join();
        t2.join();
 
        if (x == 0 && y == 0) {
            return true;
        } else {
            return false;
        }
    }
 
    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; ; i++) {
            VolatileTest volatileTest = new VolatileTest();
            if (volatileTest.test()) {
                System.out.println(i);
                break;
            }
        }
    }
}
```
按照正常的逻辑理解，在不出现指令重排的情况下，x、y永远只会有下面三种情况，不会出现都为0，即循环永远不会退出。

1. x = 1、y = 1
2. x = 1、y = 0
3. x = 0、y = 1

但是当运行的时候会发现一段时间之后循环就会退出，即出现了x、y都为0的情况，则是因为出现了指令重排，时线程内的对象赋值顺序发生了变化。<br />而这个问题给参数增加volatile关键字即可以解决，此处是因为JMM针对重排序问题限制了规则表。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801387-53e99dd3-67e2-411a-a3c9-1d09dff13cb4.webp#clientId=u075bc741-49d6-4&from=paste&id=u6ecb87b3&originHeight=184&originWidth=438&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u468a579a-dcbe-4131-b57b-200eb8346b6&title=)<br />为了实现volatile的内存语义，编译器在生成字节码时，会在指令序列中插入内存屏障来禁止特定类型的处理器重排序。一个读的操作为load，写的操作为store。<br />对于编译器来说，发现一个最优布置来最小化插入屏障的总数几乎不可能。为此，JMM采取保守策略。下面是基于保守策略的JMM内存屏障插入策略。

- 在每个volatile写操作的前面插入一个StoreStore屏障。
- 在每个volatile写操作的后面插入一个StoreLoad屏障。
- 在每个volatile读操作的后面插入一个LoadLoad屏障。
- 在每个volatile读操作的后面插入一个LoadStore屏障。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801674-58815ed4-e2af-4557-9503-4478005fb8ad.webp#clientId=u075bc741-49d6-4&from=paste&id=u12d7cd1a&originHeight=426&originWidth=361&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u820f1e49-64cf-4b15-8203-8700bb28e33&title=)<br />以上图为例，普通写与volatile写之间会插入一个StoreStore屏障，另外有一点需要注意的是，volatile写后面可能有的volatile读/写操作重排序，因为编译器常常无法准确判断是否需要插入StoreLoad屏障。<br />则JMM采用了比较保守的策略：**在每个volatile写的后面插入一个StoreLoad屏障。**<br />那么存汇编指令的角度，CPU是怎么识别到不同的内存屏障的呢：<br />**(1）sfence：**实现Store Barrior 会将store buffer中缓存的修改刷入L1 cache中，使得其他cpu核可以观察到这些修改，而且之后的写操作不会被调度到之前，即sfence之前的写操作一定在sfence完成且全局可见。<br />**（2）lfence：**实现Load Barrior 会将invalidate queue失效，强制读取入L1 cache中，而且lfence之后的读操作不会被调度到之前，即lfence之前的读操作一定在lfence完成（并未规定全局可见性）。<br />**（3）mfence：**实现Full Barrior 同时刷新store buffer和invalidate queue，保证了mfence前后的读写操作的顺序，同时要求mfence之后写操作结果全局可见之前，mfence之前写操作结果全局可见。<br />**（4）lock**：用来修饰当前指令操作的内存只能由当前CPU使用，若指令不操作内存仍然由用，因为这个修饰会让指令操作本身原子化，而且自带Full Barrior效果。<br />所以可以发现上述分析到的"lock addl"指令也是可以实现内存屏障效果的。
<a name="MnHcN"></a>
## 四、volatile 拓展
<a name="XmuPT"></a>
### 4.1 滥用 volatile 的危害
经过上述的总结可以知道volatile的实现是根据MESI缓存一致性协议实现的，而这里会用到CPU的嗅探机制，需要不断对总线进行内存嗅探，大量的交互会导致总线带宽达到峰值。因此滥用volatile可能会引起**总线风暴**，除了volatile之外大量的CAS操作也可能会引发这个问题。所以使用过程中要视情况而定，适当的场景下可以加锁来保证线程安全。
<a name="HDFS7"></a>
### 4.2 如何不用 volatile 不加锁禁止指令重排？
指令重排的示例中既然已经知道了插入内存屏障可以解决重排问题，那么用什么方式可以手动插入内存屏障呢？<br />JDK1.8之后可以在Unsafe魔术类中发现新增了插入屏障的方法。
```java

/**
 * Ensures lack of reordering of loads before the fence
 * with loads or stores after the fence.
 * @since 1.8
 */
public native void loadFence();
 
/**
 * Ensures lack of reordering of stores before the fence
 * with loads or stores after the fence.
 * @since 1.8
 */
public native void storeFence();
 
/**
 * Ensures lack of reordering of loads or stores before the fence
 * with loads or stores after the fence.
 * @since 1.8
 */
public native void fullFence();
```
（1）`loadFence()`表示该方法之前的所有load操作在内存屏障之前完成。<br />（2）`storeFence()`表示该方法之前的所有store操作在内存屏障之前完成。<br />（3）`fullFence()`表示该方法之前的所有load、store操作在内存屏障之前完成。<br />可以看到这三个方法正式对应了CPU插入内存屏障的三个指令lfence、sfence、mfence。<br />因此如果想手动添加内存屏障的话，可以用Unsafe的这三个native方法完成，另外由于Unsafe必须由bootstrap类加载器加载，所以想使用的话需要用反射的方式拿到实例对象。
```java
/**
 * 反射获取到unsafe
 */
private Unsafe reflectGetUnsafe() throws NoSuchFieldException, IllegalAccessException {
    Field field = Unsafe.class.getDeclaredField("theUnsafe");
    field.setAccessible(true);
    return (Unsafe) field.get(null);
}
 
 
// 上述示例中手动插入内存屏障
Thread t1 = new Thread(() -> {
    a = 1;
    // 插入LoadStore()屏障
    reflectGetUnsafe().storeFence();
    x = b;
});
Thread t2 = new Thread(() -> {
    b = 1;
    // 插入LoadStore()屏障
    reflectGetUnsafe().storeFence();
    y = a;
});
```
<a name="mPKlL"></a>
### 4.3 单例模式的双重检查锁为什么需要用 `volatile`
以下是单例模式双重检查锁的初始化方式：
```java
private volatile static Singleton instance = null;
 
public static Singleton getInstance() {
    if (instance == null) {
        synchronized (Singleton.class) {
            if (instance == null) {
                instance = new Singleton();
            }
        }
    }
    return instance;
}
```
那么为什么初始化对象加了双重检查和synchronized加锁来保证原子性，可是实例对象instance还要用volatile修饰呢？<br />因为synchronized虽然加了锁，但是代码块内的程序是无法保证指令重排的，其中instance = new Singleton(); 方法其实是拆分成多个指令，用javap -c 查看字节码，可以发现这段对象初始化操作是分成了三步：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801915-15fae422-d00e-4b04-b748-09e1b1d2118a.webp#clientId=u075bc741-49d6-4&from=paste&id=u18ed5da4&originHeight=238&originWidth=644&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua38c7cba-d6c2-49bb-8f80-5124a7cfbc9&title=)<br />（1）`new`：创建对象实例，分配内存空间<br />（2）`invokespecial`：调用构造器方法，初始化对象<br />（3）`aload_0`：存入局部方法变量表<br />以上三步如果顺序执行的话是没问题的，但是如果2、3步发生指令重排，则极端并发情况下可能出现下面这种情况：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637541801828-6cfa8345-eeb6-4538-abe5-43f2ce049031.webp#clientId=u075bc741-49d6-4&from=paste&id=u86a4fc04&originHeight=257&originWidth=483&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u70a1bc21-580b-45b5-b9d7-e1d0675f556&title=)<br />所以，为了保证单例对象顺利的初始化完成，应该给对象加上volatile关键字禁止指令重排。
<a name="Et7cw"></a>
## 五、总结
随着计算机和CPU的逐步升级，CPU缓存大大提高了数据读写的性能，在高并发的场景下，CPU通过MESI缓存一致性协议针对缓存行的失效进行处理。基于JMM模型，将用户态和内核态进行了划分，通过java提供的关键字和方法可以解决原子性、可见性、有序性的问题。其中volatile关键字的使用最为广泛，通过添加内存屏障、lock汇编指令的方式保证了可见性和有序性，在开发高并发系统的过程中也要注意volatile关键字的使用，但是不能滥用，否则会导致总线风暴。
