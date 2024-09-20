JVM
<a name="QNVLD"></a>
## 什么是jvm
jvm是java虚拟机 运行在用户态、通过应用程序实现java代码跨平台、与平台无关、实际上是"一次编译，到处执行"

1. 从微观来说编译出来的是字节码！去到哪个平台都能用，只要有那个平台的JDK就可以运行！字码好比是一个人，平台好比为国家，JDK好比这个国家的语言！只要这个人（字节码）有了这个国家的语言（JDK）就可以在这个国家（平台）生活下去。
2. JDK 是整个Java的核心，包括了Java运行环境（Java Runtime Envirnment），一堆Java工具和Java基础的类库(rt.jar)。
3. Java虚拟机（JVM）一种用于计算机设备的规范，可用不同的方式（软件或硬件）加以实现。编译虚拟机的指令集与编译微处理器的指令集非常类似。Java虚拟机包括一套字节码指令集、一组寄存器、一个栈、一个垃圾回收堆和一个存储方法域。
4. java编译出来的是一种“java字节码”，由虚拟机去解释执行。而c和c++则编译成了二进制，直接交由操作系统执行。
5. 所谓的一次编译、到处执行，即只需在一个地方编译，在其他各个平台下都可以执行。
6. 与平台无关指的是JAVA只运行在自己的JVM上，不需要依赖任何其他的底层类，所以和操作系统没有任何联系，平台是说运行的系统
<a name="FjfgM"></a>
## **内存结构图**
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127341391-a4cbb893-277c-44ba-b181-9fd783cff772.png#averageHue=%23c4d7de&height=446&id=AMoMP&originHeight=446&originWidth=634&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=634)
<a name="HjS5X"></a>
## class文件 
class文件径打破了C或者C++等语言所遵循的传统，使用这些传统语言写的程序通常首先被编译，然后被连接成单独的、专门支持特定硬件平台和操作系统的二进制文件。通常情况下，一个平台上的二进制可执行文件不能在其他平台上工作。而Java class文件是可以运行在任何支持Java虚拟机的硬件平台和操作系统上的二进制文件。
<a name="uVxRi"></a>
### **执行过程**
<a name="qID4A"></a>
#### **执行过程简介**
当编译和连接一个C++程序时，所获得的可执行二进制文件只能在指定的硬件平台和操作系统上运行，因为这个二进制文件包含了对目标处理器的机器语言。而Java编译器把Java源文件的指令翻译成字节码，这种字节码就是Java虚拟机的“机器语言”。<br />与普通程序不同的是，Java程序（class文件）并不是本地的可执行程序。当运行Java程序时，首先运行JVM（Java虚拟机），然后再把Java class加载到JVM里头运行，负责加载Java class的这部分就叫做Class Loader。
<a name="LYGGb"></a>
#### **JVM中的ClassLoader**
JVM本身包含了一个ClassLoader称为Bootstrap ClassLoader，和JVM一样，BootstrapClassLoader是用本地代码实现的，它负责加载核心JavaClass（即所有java.*开头的类）。<br />另外JVM还会提供两个ClassLoader，它们都是用Java语言编写的，由BootstrapClassLoader加载；其中Extension ClassLoader负责加载扩展的Javaclass（例如所有javax.*开头的类和存放在JRE的ext目录下的类）ApplicationClassLoader负责加载应用程序自身的类。<br />当运行一个程序的时候，JVM启动，运行bootstrapclassloader，该ClassLoader加载java核心API（ExtClassLoader和AppClassLoader也在此时被加载），然后调用ExtClassLoader加载扩展API，最后AppClassLoader加载CLASSPATH目录下定义的Class，这就是一个类最基本的加载流程。<br />第一个Class文件、通过javac编译成字节码、字节码之后有个ClassLoader叫类加载器，因为java.class文件到JVM内部运行起来需要有个装载过程、从物理的文件到内存的结构、比如加载、连接、初始化。<br />linux应用程序有个进程地址空间，对进程地址空间的解释：<br />linux采用虚拟内存管理技术，每一个进程都有一个3G大小的独立的进程地址空间，这个地址空间就是用户空间。每个进程的用户空间都是完全独立、互不相干的。进程访问内核空间的方式：系统调用和中断。<br />创建进程等进程相关操作都需要分配内存给进程。这时进程申请和获得的不是物理地址，仅仅是虚拟地址。 <br />实际的物理内存只有当进程真的去访问新获取的虚拟地址时，才会由“请页机制”产生“缺页”异常，从而进入分配实际页框的程序。该异常是虚拟内存机制赖以存在的基本保证，它会告诉内核去为进程分配物理页，并建立对应的页表，这之后虚拟地址才实实在在的映射到了物理地址上。<br />Linux操作系统采用虚拟内存技术，所有进程之间以虚拟方式共享内存。进程地址空间由每个进程中的线性地址区组成，而且更为重要的特点是内核允许进程使用该空间中的地址。通常情况况下，每个进程都有唯一的地址空间，而且进程地址空间之间彼此互不相干。但是进程之间也可以选择共享地址空间，这样的进程就叫做线程。<br />基本上所有linux应用程序都会遵循这个规泛、有栈、有堆、对于JVM来说、也是遵循这个规则、只不过在这个规则上做了一些改进<br />通过类加载器把Class文件装载进内存空间、装进来以后只是字节码，然后需要去运行、怎么去运行呢 ？图中类加载器子系统下面都是运行区<br />**内存空间里有：**

1. 方法区：被装载的class的信息存储在Methodarea的内存中。当虚拟机装载某个类型时，它使用类装载器定位相应的class文件，然后读入这个class文件内容并把它传输到虚拟机中。
2. Heap(堆)：一个Java虚拟实例中只存在一个堆空间。
3. JavaStack(java的栈)：虚拟机只会直接对栈执行两种操作：以帧为单位的压栈或出栈，java栈有个核心的数据、先进后出
4. Nativemethodstack(本地方法栈)：通过字面意思、基本是调用系统本地的一些方法、一般在底层封装好了、直接调用
5. 地址：在这里边是一个指针的概念、比如从变量到对象怎么做引用、就是地址
6. 计数器：主要做字节码解析的时候要记住它的位置、可以理解为一个标记
7. 执行引擎：数据、字节码做一些业务处理、最终达到想要的结果
8. 本地方法接口：基本是底层系统、比如IO网络、调用操作系统本身
9. 本地方法库：为了兼容、实现跨平台有不同的库 、兼容平台性<br />额外数据信息指的是本地方法接口和本地方法库
<a name="ZvEIk"></a>
### JMM
java的内存模型<br />大家可能听过一个词、叫线程安全、在写高并发的时候就会有线程安全问题、java里边为什么会出现线程安全问题呢、因为有JMM的存在、它会把内存分为两个区域（一个主内存、一个是工作内存）工作内存是每个java栈所私有的<br />因为要运行速度快、需要把主内存的数据放到本地内存中、然后进行计算、计算完以后再把数据回显回去![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127341542-ebb3c610-a03f-4288-bfa0-b0c1a218bc96.png#averageHue=%23eeeee9&height=293&id=JMTKw&originHeight=293&originWidth=608&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=608)<br />JMM有两个区域、主内存和栈内存、<br />java线程可能不止一个、可能有多个栈、现在需要三个线程同时做个运算、主内存初始值x=0 需要把x=0都要装载在自己的内存里边去、相当于有一个<br />副本、现在初始值和三个栈都是x=0<br />现在需要做运算 <br />x=x+1<br />x=x-1<br />x=0<br />期望值是x=0，如果是单个线程跑没问题 、取回x=0、运算x=+1、回显进来主内存就是1 、栈1是1，运算x=-1、回显进来主内存就是0、栈1是0<br />如果多个线程同时执行、结果是不可预期的、正因为有这种结构的存在、当执行x=+1、栈1是x=1  、栈2来不及执行、栈1就已经把x=1写到主内存了 、栈2跟栈3拿过去之后初始值就不是0、可能就是1了 、这样程序就写乱了 <br />所以在java中就出现了很多锁、来确保线程安全 
<a name="8rQg7"></a>
## **运行时数据区**
<a name="cIgN8"></a>
### PC寄存器----线程私有
PC寄存器也叫程序计数器（Program Counter Register）是一块较小的内存空间，它的作用可以看做是当前线程所执行的字节码的信号指示器。<br />每一条JVM线程都有自己的PC寄存器<br />在任意时刻，一条JVM线程只会执行一个方法的代码。该方法称为该线程的当前方法（Current Method）<br />如果该方法是java方法，那PC寄存器保存JVM正在执行的字节码指令的地址<br />如果该方法是native，那PC寄存器的值是undefined。<br />此内存区域是唯一一个在Java虚拟机规范中没有规定任何OutOfMemoryError情况的区域。
<a name="mXZFG"></a>
### Java虚拟机栈 ----线程私有
与PC寄存器一样，java虚拟机栈（Java Virtual Machine Stack）也是线程私有的。每一个JVM线程都有自己的java虚拟机栈，这个栈与线程同时创建，它的生命周期与线程相同。<br />虚拟机栈描述的是Java方法执行的内存模型：每个方法被执行的时候都会同时创建一个栈帧（Stack Frame）用于存储局部变量表、操作数栈、动态链接、方法出口等信息。每一个方法被调用直至执行完成的过程就对应着一个栈帧在虚拟机栈中从入栈到出栈的过程。<br />JVM stack 可以被实现成固定大小，也可以根据计算动态扩展。<br />如果采用固定大小的JVM stack设计，那么每一条线程的JVM Stack容量应该在线程创建时独立地选定。JVM实现应该提供调节JVM Stack初始容量的手段。<br />如果采用动态扩展和收缩的JVM Stack方式，应该提供调节最大、最小容量的手段。<br />JVM Stack 异常情况：<br />StackOverflowError：当线程请求分配的栈容量超过JVM允许的最大容量时抛出<br />OutOfMemoryError：如果JVM Stack可以动态扩展，但是在尝试扩展时无法申请到足够的内存去完成扩展，或者在建立新的线程时没有足够的内存去创建对应的虚拟机栈时抛出。
<a name="TNom9"></a>
### 本地方法栈----线程私有
Java虚拟机可能会使用到传统的栈来支持native方法（使用Java语言以外的其它语言编写的方法）的执行，这个栈就是本地方法栈（Native Method Stack）<br />如果JVM不支持native方法，也不依赖与传统方法栈的话，可以无需支持本地方法栈。<br />如果支持本地方法栈，则这个栈一般会在线程创建的时候按线程分配。<br />异常情况：<br />StackOverflowError：如果线程请求分配的栈容量超过本地方法栈允许的最大容量时抛出<br />OutOfMemoryError：如果本地方法栈可以动态扩展，并且扩展的动作已经尝试过，但是目前无法申请到足够的内存去完成扩展，或者在建立新的线程时没有足够的内存去创建对应的本地方法栈，那Java虚拟机将会抛出一个OutOfMemoryError异常。
<a name="hvsje"></a>
### Jave堆----线程公用
平时所说的java调优就是它<br />在JVM中，堆（heap）是可供各条线程共享的运行时内存区域，也是供所有类实例和数据对象分配内存的区域。<br />Java堆载虚拟机启动的时候就被创建，堆中储存了各种对象，这些对象被自动管理内存系统（Automatic Storage Management System，也即是常说的“Garbage Collector（垃圾回收器）”）所管理。这些对象无需、也无法显示地被销毁。<br />Java堆的容量可以是固定大小，也可以随着需求动态扩展，并在不需要过多空间时自动收缩。<br />Java堆所使用的内存不需要保证是物理连续的，只要逻辑上是连续的即可。<br />JVM实现应当提供给程序员调节Java 堆初始容量的手段，对于可动态扩展和收缩的堆来说，则应当提供调节其最大和最小容量的手段。<br />Java 堆异常：<br />OutOfMemoryError：如果实际所需的堆超过了自动内存管理系统能提供的最大容量时抛出。
<a name="ex9ne"></a>
### 方法区----线程公用
方法区是可供各条线程共享的运行时内存区域。存储了每一个类的结构信息，例如运行时常量池（Runtime Constant Pool）、字段和方法数据、构造函数和普通方法的字节码内容、还包括一些在类、实例、接口初始化时用到的特殊方法<br />方法区在虚拟机启动的时候创建。<br />方法区的容量可以是固定大小的，也可以随着程序执行的需求动态扩展，并在不需要过多空间时自动收缩。<br />方法区在实际内存空间中可以是不连续的。<br />Java虚拟机实现应当提供给程序员或者最终用户调节方法区初始容量的手段，对于可以动态扩展和收缩方法区来说，则应当提供调节其最大、最小容量的手段。<br />Java 方法区异常：<br />OutOfMemoryError： 如果方法区的内存空间不能满足内存分配请求，那Java虚拟机将抛出一个OutOfMemoryError异常。
<a name="1bNhz"></a>
### JVM内存分配
内存分配其实真正来讲是有三种的、但对于JVM来说只有两种

- 栈内存分配：

大家在调优的过程中会发现有个参数是-Xss 默认是1m，这个内存是栈内存分配, 在工作中会发现栈OutOfMemory Error内存溢出、就是因为它的内存空间不够了 一般情况下没有那么大的栈、除非一个方法里边有几十万行代码、一直往那压、不出，所以导致栈的溢出、栈的内存分配直接决定了线程数 、比如默认情况下是1m 、系统一共给了512m、那最高可以分配512个线程，再多系统分配不了啦、因为没有那么多的内存 、像tomcat、resin、jboss等、有个最大线程数、要根据这个来调、调个100万没有意义、分配不了那么大、调太少整个性能发挥不出来 ，调这个 、跟cpu有关系、需要找一个折中位置 、根据应用 、是IO密集型的还是CPU密集型的来调-Xss的值、它这里边主要保存了一些参数 、还有局部变量 、就比如说写代码、有开始有结束、这里边肯定定义了很多变量、比如：int x=1 y=0 只要在这方法内的都属于局部变量 、因为要做运算、要把这东西存住、只有等程序结束的时候才能销毁，对于这种参数是不会产生线程安全问题、因为线程是私有的

-  堆内存分配：

Java的堆是一个运行时数据区,类的(对象从中分配空间。这些对象通过new、newarray、anewarray和multianewarray等指令建立，它们不需要程序代码来显式的释放。堆是由垃圾回收来负责的，堆的优势是可以动态地分配内存大小，生存期也不必事先告诉编译器，因为它是在运行时动态分配内存的，Java的垃圾收集器会自动收走这些不再使用的数据。但缺点是，由于要在运行时动态分配内存，存取速度较慢<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127341475-b7c40591-51a7-4fd1-9ecf-c945179c24bd.png#averageHue=%23fefefe&height=373&id=TOCXL&originHeight=373&originWidth=631&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=631)
<a name="nAtQV"></a>
### jvm堆结构
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127341836-1e66f9eb-74d2-4b28-83c8-5b029fe57361.png#averageHue=%23dfdfe2&height=298&id=HIrBV&originHeight=298&originWidth=633&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=633)<br />　　　　　　　　　　　　　　　　　　　　　（图一）
<a name="EQJLs"></a>
#### 1.Young（年轻代）
年轻代分三个区。一个Eden区，两个Survivor区。大部分对象在Eden区中生成。当Eden区满时，还存活的对象将被复制到Survivor区（两个中的一个），当这个Survivor区满时，此区的存活对象将被复制到另外一个Survivor区，当这个Survivor区也满了的时候，从第一个Survivor区复制过来的并且此时还存活的对象，将被复制年老区(Old。需要注意，Survivor的两个区是对称的，没先后关系，所以同一个区中可能同时存在从Eden复制过来对象，和从前一个Survivor复制过来的对象，而复制到年老区的只有从第一个Survivor区过来的对象。而且，Survivor区总有一个是空的。
<a name="djCHM"></a>
#### 2.Old（年老代）
年老代存放从年轻代存活的对象。一般来说年老代存放的都是生命期较长的对象。
<a name="AlpbI"></a>
#### 3.Permanent：（持久代）
 也叫方法区、用于存放静态文件，如Java类、方法等。持久代对垃圾回收没有显著影响，但是有些应用可能动态生成或者调用一些class，例如hibernate等，在这种时候需要设置一个比较大的持久代空间来存放这些运行过程中新增的类。持久代大小通过-XX:MaxPermSize=进行设置。<br />举个例子：当在程序中生成对象时，正常对象会在年轻代中分配空间，如果是过大的对象也可能会直接在年老代生成（据观测在运行某程序时候每次会生成一个十兆的空间用收发消息，这部分内存就会直接在年老代分配）。年轻代在空间被分配完的时候就会发起内存回收，大部分内存会被回收，一部分幸存的内存会被拷贝至Survivor的from区，经过多次回收以后如果from区内存也分配完毕，就会也发生内存回收然后将剩余的对象拷贝至to区。等到to区也满的时候，就会再次发生内存回收然后把幸存的对象拷贝至年老区。<br />通常说的JVM内存回收总是在指堆内存回收，确实只有堆中的内容是动态申请分配的，所以以上对象的年轻代和年老代都是指的JVM的Heap空间，而持久代则是值指MethodArea，不属于Heap。
<a name="EM7xY"></a>
### **java堆结构和垃圾回收**
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127342028-2dd4a738-c659-42e4-94ad-416a344a1e23.png#averageHue=%237ea1b0&height=364&id=x9HbX&originHeight=364&originWidth=657&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=657)<br />　　　　　　　　　　　　　　　　　　　　图（二）<br />Direct Momery 严格意义来说也算堆，它是一块物理内存、可以分为操作系统内存、是比较快的、不会走JVM 在java里边实现了内存映射、这样速度更快<br />CodeCache 放一些字节码、类的信息会放在里边<br />Permanent Generation space 方法区、严格意义来说也属于堆<br />Eden Space 区<br />Survivor Space区<br />Tenured Generation Old区（年老代）<br />JVM GC 管理<br />调优大部分调优的是怎么回收，Minor GC 回收Eden Space和 Survivor Space ， Full GC回收所有区域 <br />不管什么GC，回收过程中会出现暂停、回收过程中用户线程是不会工作的、这样就造成程序卡了 这是无法改变不了的事实、避免不了、不过可以优化暂停时间的长短<br />原则上不能出现Full GC 、所有区域都要跑一遍 、出现Full GC 应用就不可用
<a name="qeD4Z"></a>
### JVM堆配置参数
1、-Xms初始堆大小<br />默认物理内存的64/1（<1GB），建议小于1G、可根据应用业务调节<br />2、-Xmx最大堆大小<br />默认物理内存的4/1（<1GB）、建议小于1G、实际中建议不大于4GB（否则会出现很多问题）<br />3、一般建议设置 -Xms= -Xmx<br />好处是避免每次在gc后、调整堆的大小、减少系统内存分配开销<br />4、整个堆大小=年轻代大小+年老代大小+持久代大小（Permanent Generation space区、也会被Full GC回收）
<a name="7qTVz"></a>
### jvm新生代（young generation)
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127341925-cf42b989-c8be-436a-802e-1823d8971d24.png#averageHue=%23b9d0d8&height=320&id=tCQCR&originHeight=361&originWidth=649&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=575)<br />                                              图（三）<br />1、新生代=1个eden区和2个Survivor区<br />2、-Xmn 年轻代大小<br />设置年轻代大小、比如-Xmn=100m那么新生代就是100m，然后共享<br />3、-XX：NewRatio<br />年轻代（包括Eden和两个Survivor区）与年老代的比值（除去持久代）Xms=Xmx并且设置了Xmn的情况下，该参数不需要进行设置。<br />4、-XX：SurvivorRatio<br />Eden区与Survivor区的大小比值，设置为8（默认是8） ，则两个Survivor区与一个Eden区的比值为2:8，一个Survivor区占整个年轻代的1/10<br />比如新生代=100m，设置-XX：SurvivorRatio为8，那E =80m S0 =10m S1=10m（1/10）<br />5、用来存放JVM刚分配的Java对象
<a name="bhP5g"></a>
### java老年代（tenured generation）
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127342210-9b6c3b2a-83ea-46eb-a01f-041cfa164aa0.png#averageHue=%23b8edc6&height=177&id=sb6dY&originHeight=177&originWidth=360&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=360)<br />　　　　　　　　　　　图（四）<br />1、老年代=整个堆-年轻代大小-持久代大小<br />年轻代就是上面讲的-xmn配置的参数、持久代参数默认是0<br />2、年轻代中经过垃圾回收没有回收掉的对象被复制到年老代。<br />就是这个对象收集完一次、发现被引用了、某个地方使用了、回收不掉才放进去，一般是多次回收、从E区回收过程中、先进S0或者S1、S0或者S1再回收一次、回收不掉再放到年老区<br />3、老年代存储对象比年轻代年龄大的多，而且不乏大对象。<br />对互联网企业来说、最常用的是"缓存"的对象比较多、缓存一般会用弱引用、但弱引用也不会轻易被回收的、除非是在整个堆的内存不够的情况下、防止内存宕机、强引用是和垃圾回收机制相关的。一般的，如果一个对象可以通过一系列的强引用引用到，那么就 说明它是不会被垃圾回收机制（Garbage Collection）回收的，<br />刚才说了缓存对象一般是弱引用、有些数据丢了是没关系的、只是提高系统性能才放到缓存里边去、但是如果有一天内存不够了 、缓存占了很大一部分对象、不回收的话、整个系统都不可用了、整个服务都不能用了、如果回收掉、可以从数据库去取、可 能速 度慢点、但是服务可用性不会降低<br />比如说刚开始分配的对象 、这个对象暂定是OLD区、刚开始一部分内存区域被缓存占据了、一般情况下对于一个缓存的设计都有初始值、对于java来说、比较通用的缓存是可以自动伸缩的、<br />如图（四）整个OLD区50M有45M是被缓存占据了、不会被回收掉、那整个OLD区只有5M可以用了 、假如E区有40M 、S0 分配10M 、S1分配也是10M 、理想情况下、经过E区到S0、S1到老年代的大小不到1M、 那5M就够了、不会出现FULL GC 、也不会出现       内存溢出、一旦对象大于5M、比如10M的数据、 放不进去了、就会出现FULL gc 、FULL gc会把整个缓存全都收掉、瞬间缓存数据就没了、然后把10M的数据放进去、这就是弱引用、可以理解为这是一种服务降级、如果是强引用那就直接挂了<br />4、新建的对象也有可能直接进入老年代<br />　　4.1、大对象，可通过启动参数设置-XX:PretenureSizeThreshold=1024（单位为字节，默认为0、也就是说所有的默认都在新生代）来代表超过多大时就不再新生代分配，而是直接在老年代分配<br />　　4.2、大的数组对象，切数组中无引用外部对象。<br />5、老年代大小无配置参数 
<a name="Lophd"></a>
### java持久代（perm generation）
1、持久代=整个堆-年轻代大小-老年代大小<br />2、-XX:PermSize 最小 -XX:MaxPermSize 最大<br />设置持久代的大小，一般情况推荐把-XX:PermSize设置成 -XX:MaxPermSize的值为相同的值，因为永久代大小的调整也会导致堆内存需要触发fgc。<br />3、存放Class、Method元信息，其大小与项目的规模、类、方法的数量有关。一般设置为128M就足够，设置原则是预留30%的空间<br />刚开始设置了128M、随着程序的运行、java有一个叫lib的地方放了很多类库、这个类库并不是所有的都加载的、只有在用的时候或者系统初始化的时候会加载一部分、比如已经占了100M了、但是随着业务的运行会动态去类<br />库里加、把一些Class文件通过反射的方式装进去、这样内存不断增大、达到128M以后就挂了、就会报方法区溢出、怎么做？调大到256M、然后监控、超过阈值再调大、简单方式是调大、另外JDK里边有一个GC可以回收<br />如果能接受停机、就调大，简单、快速、已解决问题为主<br />4、永久代的回收方式<br />4.1、常量池中的常量，无用的类信息，常量的回收很简单，没有引用了就可以被回收<br />比如一个常量=5 它的意义就是个值、如果回收、发现它没被引用就被回收了<br />4.2、对于无用的类进行回收，必须保证3点：<br />类跟常量不一样、一个类里边可能有好多东西、比如这个类引用那个类、

-  类的所有实例都已经被回收
-  加载类的ClassLoader已经被回收
-  类对象的Class对象没有被引用（即没有通过反射引用该类的地方）
<a name="sSJjJ"></a>
## jvm垃圾收集算法 
<a name="Yih9Z"></a>
### 1、引用计数算法
每个对象有一个引用计数属性，新增一个引用时计数加1，引用释放时计数减1，计数为0时可以回收。此方法简单，无法解决对象相互循环引用的问题。还有一个问题是如何解决精准计数。<br />这种方法现在已经不用了 
<a name="FjOpb"></a>
### 2、根搜索算法
从GC Roots开始向下搜索，搜索所走过的路径称为引用链。当一个对象到GC Roots没有任何引用链相连时，则证明此对象是不可用的。不可达对象。<br />在java语言中，GC Roots包括：<br />虚拟机栈中引用的对象。<br />方法区中类静态属性实体引用的对象。<br />方法区中常量引用的对象。<br />本地方法栈中JNI引用的对象。
<a name="xaxHd"></a>
## jvm垃圾回收算法
<a name="c4de7367"></a>
### 1、复制算法（Copying）
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127342302-1f7a4014-3a7a-4614-9f7d-62a64e4760b1.png#averageHue=%23f3f3f3&height=267&id=hFeQR&originHeight=267&originWidth=519&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=519)

- 复制算法采用从根集合扫描，并将存活对象复制到一块新的，没有使用过的空间中，这种算法当控件存活的对象比较少时，极为高效，但是带来的成本是需要一块内存交换空间用于进行对象的移动。
- 此算法用于新生代内存回收，从E区回收到S0或者S1

 从根集合扫描、就是刚才说的GC-Roots 收集算法、从它开始查引用、如果没有被引用、开始执行算法、并将存活对象复制到一块新的、（S0或者S1） 
<a name="3QMuQ"></a>
### 2、标记清除算法
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127342507-26b8c40b-3191-47da-ac77-24ba82dc9bb8.png#averageHue=%23f5f5f5&height=410&id=l4sLY&originHeight=410&originWidth=512&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=512)

- 标记-清除算法采用从根集合进行扫描，对存活的对象标记，标记完毕后，再扫描整个空间中未被标记的对象，进行回收，如图所示。
- 标记-清除算法不需要进行对象的移动，并且仅对不存活的对象进行处理，在存活对象比较多的情况下极为高效，但由于标记-清除算法直接回收不存活的对象，因此会造成内存碎片！

适合老生代去回收<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127342664-5d54608e-1def-41cd-81e6-a32f1417031e.png#averageHue=%23f0f0f0&height=383&id=vyNU6&originHeight=383&originWidth=360&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=360)<br />标记-整理算法采用标记-清除算法一样的方式进行对象的标记，但在清除时不同，在回收不存活的对象占用的空间后，会将所有的存活对象往左端空闲空间移动，并更新对应的指针。<br />标记-整理算法是在标记清除算法的基础上，又进行了对象的移动，因此成本更高，但是却解决了内存碎片的问题。
<a name="OOhv9"></a>
## **名词解释**
<a name="ZtK4F"></a>
### 1、串行回收
gc单线程内存回收、会暂停使有用户线程
<a name="82286bf2"></a>
### 2、并行回收
收集是指多个GC线程并行工作，但此时用户线程是暂停的；所以，Seral是串行的，Paralle收集器是并行的，而CMS收集器是并发的。
<a name="kiU33"></a>
### 3、并发回收
是指用户线程与GC线程同时执行（不一定是并行，可能交替，但总体上是在同时执行的），不需要停顿用户线程（其实在CMS中用户线程还是需要停顿的，只是非常短，GC线程在另一个CPU上执行）<br />串行回收要区分好并行回收和并发回收的区别，这地方非常关键、在选择GC的过程中根据应用场景来选择
<a name="jmzT5"></a>
## JVM常见垃圾回收器
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127342815-c274793e-fc21-4369-8bdc-3085bf1bba08.png#averageHue=%23dfded8&height=371&id=IQpUN&originHeight=371&originWidth=536&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=536)<br />上图是HotSpot里的收集器，中间的横线表示分代，有连线表示可以组合使用。<br />年轻代区域有<br />Serial 串行 <br />ParNew 并发 <br />Parallel Scavenge 并行<br />年老代区域有<br />CMS<br />Serial Old <br />Parallel Old<br />G1目前还不成熟 、适合年轻代和年老代
<a name="CqyZA"></a>
### Serial 回收器（串行回收器）
 <br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127343075-2f8c530b-2efa-47f2-ae98-aaba5ca86aa4.png#averageHue=%23e7e7e7&height=124&id=SDgTE&originHeight=124&originWidth=679&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=679)<br />是一个单线程的收集器，只能使用一个CPU或一条线程区完成垃圾收集；在进行垃圾收集时，必须暂停所有其它工作线程，直到收集完成。<br />缺点：Stop-The-World<br />优势：简单。对于单CPU的情况，由于没有多线程交互开销，反而可以更高效。是Client模式下默认的新生代收集器。
<a name="SsNfG"></a>
#### 新生代Serial回收器
1、通过-XX:+UseSerialGC来开启<br />Serial New+Serial Old的收集器组合进行内存回收<br />2、使用复制算法。<br />3、独占式的垃圾回收。<br />一个线程进行GC，串行。其它工作线程暂停。
<a name="xfUmp"></a>
#### 老年代Serial回收器
1、-XX：UseSerialGC来开启<br />Serial New+Serial Old的收集器组合进行内存回收<br />2、使用标记-压缩算法<br />3、串行的、独占式的垃圾回收器。<br />因为内存比较大的原因，回收比新生代慢
<a name="jl3Gl"></a>
### ParNew回收器（并行回收器）
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127343108-8308f73a-0617-4da7-9cd8-c3ef3bf64243.png#averageHue=%23dfdfdf&height=125&id=NhpjW&originHeight=125&originWidth=657&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=657)<br />并行回收器也是独占式的回收器，在收集过程中，应用程序会全部暂停。但由于并行回收器使用多线程进行垃圾回收，因此，在并发能力比较强的CPU上，它产生的停顿时间要短<br />于串行回收器，而在单CPU或者并发能力较弱的系统中，并行回收器的效果不会比串行回收器好，由于多线程的压力，它的实际表现很可能比串行回收器差。
<a name="OqSFp"></a>
### 新生代ParNew回收器
1、-XX:+UseParNewGC开启<br />新生代使用并行回收收集器，老年代使用串行收集器<br />2、-XX:ParallelGCThreads 指定线程数<br />默认最好与CPU数理相当，避免过多的线程数影响垃圾收集性能<br />3、使用复制算法。<br />4、并行的、独占式的垃圾回收器。
<a name="RWVwe"></a>
### 新生代Parallel Scavenge回收器
1、吞吐量优先回收器<br />关注CPU吞吐量，即运行用户代码的时间/总时间。比如：JVM运行100分钟，其中运行用户代码99分钟，垃圾回收1分钟。则吞吐量是99%，这种收集器能最高效率的利用CPU，适合运行后台运算<br />2、-XX:+UseParallelGC开启<br />使用Parallel Scavenge+Serial Old收集器组合回收垃圾，这也是Server模式下的默认值<br />3、-XX:GCTimeRation<br />来设置用户执行时间占总时间的比例，默认99，即1%的时间用来进行垃圾回收<br />4、-XX:MaxGCPauseMillis<br />设置GC的最大停顿时间<br />5、使用复制算法
<a name="ex4GC"></a>
### 老生代Parallel Old回收器
1、-XX:+UseParallelOldGC开启<br />使用Parallel Scavenge +Parallel Old组合收集器进行收集<br />2、使用标记整理算法。<br />3、并行的、独占式的垃圾回收器。
<a name="Qztqv"></a>
### CMS(并发标记清除)回收器
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618127343131-909a8dd0-b6c9-4435-8aad-d3faf7055aac.png#averageHue=%23e4e4e4&height=139&id=PawsQ&originHeight=139&originWidth=641&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=641)<br />运作过程分为4个阶段：<br />初始标记（CMS inital mark）：值标记GC Roots能直接关联到的对象。<br />并发标记（CMS concurrent mark）：进行GC RootsTracing的过程。<br />重新标记（CMS remark）：修正并发标记期间用户程序继续运行而导致标记发生改变的那一部分对象的标记.<br />并发清除（CMS concurrent sweep）：<br />其中标记和重新标记两个阶段仍然需要Stop-The-World,整个过程中耗时最长的并发标记和并发清除过程中收集器都可以和用户线程一起工作
<a name="CATRz"></a>
### CMS（并发标记清除）回收器
<a name="FjuZQ"></a>
#### 1、标记-清除算法
同时它又是一个使用多线程并发回收的垃圾收集器
<a name="d0gq6"></a>
#### 2、-XX:ParalleCMSThreads
手工设定CMS的线程数量，CMS默认启动的线程数是（ParallelGCTherads+3）+3/4）<br />这是它的公式，一般情况下、对于IO密集型的 cpu的核数乘以2+1 ，CPU密集型的一般CPU的核数+1
<a name="bgQGP"></a>
#### 3、-XX+UseConcMarkSweepGC开启
使用ParNew+CMS+Serial Old的收集器组合进行内存回收，Serial Old作为CMS出现“Concurrent Mode Failure” 失败后的后备收集器使用.<br />失败以后就会触发Full GC 、位了避免这种情况发生、就要对它进行配置、触发Full GC有两种情况、promotion failed和concurrent mode failure<br />对于采用CMS进行老年代GC的程序而言，尤其要注意GC日志中是否有promotion failed和concurrent mode failure两种状况，当这两种状况出现时可能<br />会触发Full GC。<br />promotion failed是在进行Minor GC时，survivor space放不下、对象只能放入老年代，而此时老年代也放不下造成的；concurrent mode failure是在<br />执行CMS GC的过程中同时有对象要放入老年代，而此时老年代空间不足造成的（有时候“空间不足”是CMS GC时当前的浮动垃圾过多导致暂时性的空间不足触发Full GC）。<br />对应措施为：增大survivor space、老年代空间或调低触发并发GC的比率。
<a name="rPhBz"></a>
#### 4、-XX:CMSInitiatingOccupancyFraction
设置CMS收集器在老年代空间被使用多少后触发垃圾回收器，默认值为68%,仅在CMS收集器时有效，-XX:CMSInitiatingOccupancyFraction=70<br />（一般情况为70%，设太高了可能会出现失败，设太低了、频繁， 只能去找一个比值、可以分析GC log、看是否符合要求 ）
<a name="O7RSl"></a>
#### 5、-XX:+UseCMSCompactAtFullCollection 
由于CMS收集器会产生碎片，此参数设置在垃圾收集器后是否需要一次内存碎片整理过程，仅在CMS收集器时有效
<a name="9Onjf"></a>
#### 6、-XX:+CMSFullGCBeforeCompaction
设置CMS收集器在进行若干次垃圾收集后再进行一次内存碎片整理过程,通常与UseCMSCompactAtFullCollection参数一起使用
<a name="zeFTN"></a>
#### 7、-XX:CMSInitiatingPermOccupancyFraction
设置Perm Gen使用到达多少比率时触发,默认92%

