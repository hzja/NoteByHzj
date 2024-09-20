Java 内存区域<br />Java内存区域是指 JVM运行时将数据分区域存储 ，简单的说就是不同的数据放在不同的地方。通常又叫 运行时数据区域。<br />Java内存模型（JMM）定义了程序中各个变量的访问规则，即在虚拟机中将变量存储到内存和从内存中取出变量这样的底层细节。
<a name="n84Gq"></a>
## 1、Java内存区域
<a name="FsxkY"></a>
##### 1.8 之前：
![2021-09-16-22-28-39-952300.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631802557798-1c2e9e6f-5cd9-422d-92a3-0c080eaad610.png#clientId=u5ddab611-9000-4&from=ui&id=uf2a01099&originHeight=539&originWidth=887&originalType=binary&ratio=1&size=49015&status=done&style=shadow&taskId=u19d9c650-0258-4907-b13f-4016241f640)<br />Java内存区域 1.8之前
<a name="SZLjv"></a>
##### JDK1.8（含）之后：
![2021-09-16-22-28-40-096269.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631802557804-2035a69d-c1d7-4464-9c7a-51b9f2299219.png#clientId=u5ddab611-9000-4&from=ui&id=FOBvJ&originHeight=551&originWidth=891&originalType=binary&ratio=1&size=45968&status=done&style=shadow&taskId=u88664b0d-07fc-4d4b-9607-dad191e21f7)<br />Java内存区域 1.8<br />区别就是 1.8有一个元数据区替代方法区了。<br />JDK 1.7 其实是并没完全移除方法区，但是不会像1.6以前报 “java.lang.OutOfMemoryError: PermGen space”，而是报 java.lang.OutOfMemoryError: Java heap space<br />1.7部分内容（比如 常量池、静态变量有方法区转移到了堆）<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631802327195-4aae64a6-dd0b-434f-a592-68a1e6ad6e56.webp#clientId=u5ddab611-9000-4&from=paste&id=u41096139&originHeight=482&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uf1d58877-fca2-47f5-8fe6-d9c81f83070)<br />演变<br />那么，Java 8 中 PermGen 为什么被移出 HotSpot JVM 了？总结了两个主要原因：

1. 由于 PermGen 内存经常会溢出，引发恼人的 java.lang.OutOfMemoryError: PermGen，因此 JVM 的开发者希望这一块内存可以更灵活地被管理，不要再经常出现这样的 OOM
2. 移除 PermGen 可以促进 HotSpot JVM 与 JRockit VM 的融合，因为 JRockit 没有永久代。

根据上面的各种原因，PermGen 最终被移除，方法区移至 Metaspace，字符串常量移至 Java Heap。<br />下面逐一介绍一下jvm管辖的这几种内存区域。
<a name="CcM8A"></a>
## 2、程序计数器
程序计数器（Program Counter Register）是一块较小的内存空间，由于JVM可以并发执行线程，因此会存在线程之间的切换，而这个时候就程序计数器会记录下当前程序执行到的位置，以便在其他线程执行完毕后，恢复现场继续执行。<br />JVM会为每个线程分配一个程序计数器，与线程的生命周期相同。<br />如果线程正在执行的是应该Java方法，这个计数器记录的是正在执行虚拟机字节码指令的地址。<br />如果正在执行的是Native方法，计数器的值则为空（undefined）<br />程序计数器是唯一一个在 Java 虚拟机规范中没有规定任何 `OutOfMemoryError` 情况的区域。
<a name="n1AHC"></a>
## 3、Java虚拟机栈
虚拟机栈 描述的是 Java 方法执行的内存模型：<br />每个方法在执行的同时都会创建一个栈帧（Stack Frame，是方法运行时的基础数据结构）用于存储局部变量表、操作数栈、动态链接、方法出口等信息。每一个方法从调用直至执行完成的过程，就对应着一个栈帧在虚拟机栈中入栈到出栈的过程。<br />虚拟机栈是每个线程独有的，随着线程的创建而存在，线程结束而死亡。<br />在虚拟机栈内存不够的时候会OutOfMemoryError，在线程运行中需要更大的虚拟机栈时会出现StackOverFlowError。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631802327373-9b0b0b45-4357-473d-b02e-fe0686d9a49e.webp#clientId=u5ddab611-9000-4&from=paste&id=udd18b65c&originHeight=592&originWidth=895&originalType=url&ratio=1&status=done&style=shadow&taskId=u3370a12a-34a6-412b-b1ef-98230490705)<br />虚拟机栈包含很多栈帧，每个方法执行的同时会创建一个栈帧，栈帧又存储了方法的局部变量表、操作数栈、动态连接和方法返回地址等信息。<br />在活动线程中，只有位于栈顶的栈帧才是有效的，称为当前栈帧，与这个栈帧相关联的方法称为当前方法。
<a name="V3c1U"></a>
### 1）局部变量表
局部变量表是存放方法参数和局部变量的区域。<br />全局变量是放在堆的，有两次赋值的阶段，一次在类加载的准备阶段，赋予系统初始值；另外一次在类加载的初始化阶段，赋予代码定义的初始值。<br />而局部变量没有赋初始值是不能使用的。
<a name="Cpmzw"></a>
### 2）操作数栈
一个先入后出的栈。<br />当一个方法刚刚开始执行的时候，这个方法的操作数栈是空的，在方法的执行过程中，会有各种字节码指令往操作数栈中写入和提取内容，也就是出栈/入栈操作。
<a name="oQX2U"></a>
### 3） 动态连接
每个栈帧都包含一个指向运行时常量池中该栈帧所属方法的引用。持有这个引用是为了支持方法调用过程中的动态连接(Dynamic Linking)。<br />常量池可以便于指令的识别
```java
public void methodA(){

}
public void methodB(){
    methodA();//methodB()调用methodA(),先找到调用methodA()的版本符号，再变为直接引用
}
```
方法调用并不等同于方法执行，方法调用阶段唯一的任务就是确定被调用方法的版本(即调用哪一个方法)，这也是Java强大的扩展能力，在运行期间才能确定目标方法的直接引用。<br />所有方法调用中的目标方法在Class文件里面都是一个常量池中的符号引用，在类加载的解析阶段，会将其中的一部分符号引用转化为直接引用。
<a name="wco7W"></a>
### 4）方法返回地址（方法出口）
返回分为 正常返回 和 异常退出。<br />无论何种退出情况，都将返回至方法当前被调用的位置，这也程序才能继续执行。<br />一般来说，方法正常退出时，调用者的PC计数器的值可以作为返回地址，栈帧中会保存这个计数器值。<br />方法退出的过程相当于弹出当前栈帧。
<a name="QUXKS"></a>
## 4、本地方法栈
Java虚拟机栈是调用Java方法；本地方法栈是调用本地native方法，可以认为是通过 JNI (Java Native Interface) 直接调用本地 C/C++ 库，不受JVM控制。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631802327435-7a31c6b4-4fa2-4969-874c-257445edd691.webp#clientId=u5ddab611-9000-4&from=paste&id=u27bf4e70&originHeight=458&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ucea82d16-4af1-4bab-9fad-9a8b30d2443)<br />Native方法<br />![2021-09-16-22-28-40-808267.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631802605942-e6f299c5-6368-411e-99d2-b3d64d1acc6f.png#clientId=u5ddab611-9000-4&from=ui&id=u906b6d73&originHeight=657&originWidth=881&originalType=binary&ratio=1&size=39279&status=done&style=shadow&taskId=ue3b8f394-89b1-4efc-a981-d47603fe33f)<br />Java虚拟机栈与本地方法栈的调用过程<br />本地方法栈也会抛出 `StackOverflowError` 和 `OutOfMemoryError` 异常
<a name="IzhUp"></a>
## 5、Java堆
Java 堆是被所有线程共享的一块内存区域，在虚拟机启动时创建。此内存区域的唯一目的就是存放对象实例，几乎所有的对象实例都在这里分配内存。<br />堆是垃圾收集器管理的主要区域，又称为“GC堆”，可以说是Java虚拟机管理的内存中最大的一块。<br />现在的虚拟机（包括HotSpot VM）都是采用分代回收算法。在分代回收的思想中， 把堆分为：新生代+老年代+永久代（1.8没有了）；新生代 又分为 Eden + From Survivor + To Survivor区。<br />![2021-09-16-22-28-40-940266.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631802606001-6ef2d750-2e38-4c89-abf3-5281e072aca8.png#clientId=u5ddab611-9000-4&from=ui&id=vHR3p&originHeight=727&originWidth=1035&originalType=binary&ratio=1&size=60904&status=done&style=shadow&taskId=ud3f91fd9-0e72-4953-bac9-67e021b31bd)
<a name="BSw9R"></a>
## 6、方法区
方法区（Method Area）与 Java 堆一样，是所有线程共享的内存区域。<br />方法区用于存储已经被虚拟机加载的类信息（即加载类时需要加载的信息，包括版本、field、方法、接口等信息）、final常量、静态变量、编译器即时编译的代码等。<br />方法区逻辑上属于堆的一部分，但是为了与堆进行区分，通常又叫“非堆”。<br />方法区比较重要的一部分是运行时常量池（Runtime Constant Pool），为什么叫运行时常量池呢？是因为运行期间可能会把新的常量放入池中，比如说常见的String的intern()方法。
```java
String a = "I am HaC";
Integer b = 100;
```
在编译阶段就把所有的字符串文字放到一个常量池中，复用同一个（比如说上述的“I am HaC”），节省空间。<br />关于方法区和元空间的关系：
> 方法区是JVM规范概念，而永久代则是Hotspot虚拟机特有的概念，简单点理解：方法区和堆内存的永久代其实一个东西，但是方法区是包含了永久代。
> 只有 HotSpot 才有 “PermGen space”，而对于其他类型的虚拟机，如 JRockit（Oracle）、J9（IBM） 并没有“PermGen space”

<a name="enPlr"></a>
## 7、元空间
1.8就把方法区改用元空间了。类的元信息被存储在元空间中。元空间没有使用堆内存，而是与堆不相连的本地内存区域。所以，理论上系统可以使用的内存有多大，元空间就有多大，所以不会出现永久代存在时的内存溢出问题。<br />可以通过 `-XX:MetaspaceSize` 和 `-XX:MaxMetaspaceSize` 来指定元空间的大小。
<a name="rPyUa"></a>
## 8、总结
![2021-09-16-22-28-41-102264.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631802606027-b3021e03-fe63-4e10-9618-c4216a8efdfe.png#clientId=u5ddab611-9000-4&from=ui&id=uhoVu&originHeight=577&originWidth=1080&originalType=binary&ratio=1&size=97822&status=done&style=none&taskId=u1a992ca9-c735-4c2e-aa57-2a9d810a016)<br />Java内存区域
