JVM
<a name="a3yBn"></a>
## 1、简述JVM 内存模型
![2021-06-25-23-06-23-066905.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633632414-2f2e4588-b577-440d-98d6-7d6c9cfd17bf.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u3f327057&originHeight=546&originWidth=647&originalType=binary&ratio=3&size=46265&status=done&style=shadow&taskId=uf0803f9a-28cf-463f-991b-efba047c19a)<br />从宏观上来说JVM 内存区域 分为三部分线程共享区域、线程私有区域、直接内存区域(上图static变量该在堆区)。
<a name="gHM17"></a>
### 1.1、线程共享区域
<a name="hD2i0"></a>
#### 1.1.1、堆区
堆区Heap是JVM中最大的一块内存区域，基本上所有的对象实例都是在堆上分配空间。堆区细分为年轻代和老年代，其中年轻代又分为Eden、S0、S1 三个部分，他们默认的比例是8:1:1的大小。
<a name="zbnVj"></a>
#### 1.1.1、元空间
方法区：
> 1. 在 《Java虚拟机规范》中只是规定了有方法区这么个概念跟它的作用。HotSpot在JDK8之前 搞了个永久代把这个概念实现了。用来主要存储类信息、常量池、静态变量、JIT编译后的代码等数据。
> 2. PermGen(永久代)中类的元数据信息在每次FullGC的时候可能会被收集，但成绩很难令人满意。而且为PermGen分配多大的空间因为存储上述多种数据很难确定大小。因此官方在JDK8剔除移除永久代。

官方解释移除永久代：
> 1. This is part of the JRockit and Hotspot convergence effort. JRockit customers do not need to configure the permanent generation (since JRockit does not have a permanent generation) and are accustomed to not configuring the permanent generation.
> 2. 即：移除永久代是为融合HotSpot JVM与 JRockit VM而做出的努力，因为JRockit没有永久代，不需要配置永久代。

元空间：<br />在Java中用永久代来存储类信息，常量，静态变量等数据不是好办法，因为这样很容易造成内存溢出。同时对永久代的性能调优也很困难，因此在JDK8中 把永久代去除了，引入了元空间metaspace，原先的class、field等变量放入到metaspace。<br />总结：<br />元空间的本质和永久代类似，都是对JVM规范中方法区的实现。不过元空间与永久代之间最大的区别在于：元空间并不在虚拟机中，而是使用本地内存。因此，默认情况下，元空间的大小仅受本地内存限制，但可以通过参数来指定元空间的大小。
<a name="j1DyF"></a>
### 1.2、直接内存区域
直接内存：<br />一般使用Native函数操作C++代码来实现直接分配堆外内存，不是虚拟机运行时数据区的一部分，也不是Java虚拟机规范中定义的内存区域。这块内存不受Java堆空间大小的限制，但是受本机总内存大小限制所以也会出现OOM异常。分配空间后避免了在Java堆区跟Native堆中来回复制数据，可以有效提高读写效率，但它的创建、销毁却比普通Buffer慢。<br />PS：如果使用了NIO，本地内存区域会被频繁的使用，此时 jvm内存 ≈ 方法区 ＋ 堆 ＋ 栈＋ 直接内存
<a name="aClIU"></a>
### 1.3、线程私有区域
程序计数器、虚拟机栈、本地方法栈跟线程的声明周期是一样的。
<a name="f2ZP7"></a>
#### 1.3.1、程序计数器
课堂上比如你正在看小说《诛仙》，看到1412章节时，老师喊你回答问题，这个时候你肯定要先应付老师的问题，回答完毕后继续接着看，这个时候可以用书签也可以凭借记忆记住自己在看的位置，通过这样实现继续阅读。<br />落实到代码运行时候同样道理，程序计数器用于记录当前线程下虚拟机正在执行的字节码的指令地址。它具有如下特性：

1. 线程私有

多线程情况下，在同一时刻所以为了让线程切换后依然能恢复到原位，每条线程都需要有各自独立的程序计数器。

1. 没有规定OutOfMemoryError

程序计数器存储的是字节码文件的行号，而这个范围是可知晓的，在一开始分配内存时就可以分配一个绝对不会溢出的内存。

1. 执行Native方法时值为空

Native方法大多是通过C实现并未编译成需要执行的字节码指令，也就不需要去存储字节码文件的行号了。
<a name="Hiikx"></a>
#### 1.3.2、虚拟机栈
方法的出入栈：调用的方法会被打包成栈桢，一个栈桢至少需要包含一个局部变量表、操作数栈、桢数据区、动态链接。<br />![2021-06-25-23-06-23-138715.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633647262-2c7588e8-6614-48cb-9326-8632deaa5f27.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u4f5c6e46&originHeight=170&originWidth=484&originalType=binary&ratio=3&size=16280&status=done&style=shadow&taskId=uffdca979-a0f8-4637-82b6-c030b9588d7)<br />动态链接：<br />当栈帧内部包含一个指向运行时常量池引用前提下，类加载时候会进行符号引用到直接引用的解析跟链接替换。<br />局部变量表：<br />局部变量表是栈帧重要组中部分之一。他主要保存函数的参数以及局部的变量信息。局部变量表中的变量作用域是当前调用的函数。函数调用结束后，随着函数栈帧的销毁。局部变量表也会随之销毁，释放空间。<br />操作数栈：<br />保存着Java虚拟机执行过程中数据<br />方法返回地址：<br />方法被调用的位置，当方法退出时候实际上等同于当前栈帧出栈。<br />比如执行简单加减法：
```java
public class ShowByteCode {
    private String xx;
    private static final int TEST = 1;
    public ShowByteCode() {
    }
    public int calc() {
        int a = 100;
        int b = 200;
        int c = 300;
        return (a + b) * c;
    }
}
```
执行`javap -c *.class`：<br />![2021-06-25-23-06-23-260210.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633659334-cc780094-5edd-4397-8f60-eb4e3e6d4b16.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=uc7cab8b9&originHeight=463&originWidth=1080&originalType=binary&ratio=3&size=152453&status=done&style=none&taskId=ub634c560-f38a-4b97-91db-e6c153c6ef4)
<a name="rWqXe"></a>
##### 1.3.3、本地方法栈
跟虚拟机栈类似，只是为使用到的Native方法服务而已。
<a name="HRdbY"></a>
## 2、判断对象是否存活
JVM空间不够就需要Garbage Collection了，一般共享区的都要被回收比如堆区以及方法区。在进行内存回收之前要做的事情就是判断那些对象是死的，哪些是活的。常用方法有两种 引用计数法 跟 可达性分析。
<a name="NbaSF"></a>
### 2.1、引用计数法
思路是给 Java 对象添加一个引用计数器，每当有一个地方引用它时，计数器 +1；引用失效则 -1，当计数器不为 0 时，判断该对象存活；否则判断为死亡(计数器 = 0)。优点：<br />实现简单，判断高效。<br />缺点：<br />无法解决 对象间 相互循环引用 的问题
```java
class GcObject {
    public Object instance = null;
}
public class GcDemo {
    public static void main(String[] args) {
        GcObject object1 = new GcObject(); // step 1 
        GcObject object2 = new GcObject(); // step 2
        
        object1.instance = object2 ;//step 3
        object2.instance = object1; //step 4
        
        object1 = null; //step 5
        object2 = null; // step 6
    }
}
```
```java
step1: GcObject实例1的引用计数+1，实例1引用数 = 1 
step2: GcObject实例2的引用计数+1，实例2引用数 = 1 
step3: GcObject实例2的引用计数+1，实例2引用数 = 2 
step4: GcObject实例1的引用计数+1，实例1引用数 = 2 
step5: GcObject实例1的引用计数-1，结果为 1 
step6: GcObject实例2的引用计数-1，结果为 1
```
如上分析发现实例1跟实例2的引用数都不为0而又相互引用，这两个实例所占有的内存则无法释放。
<a name="cJZtP"></a>
### 2.2、可达性分析
很多主流商用语言（如Java、C#）都采用引用链法判断对象是否存活，大致的思路就是将一系列的 GC Roots 对象作为起点，从这些起点开始向下搜索。在Java语言中，可作为 GC Roots 的对象包含以下几种：

1. 第一种是虚拟机栈中的引用的对象，在程序中正常创建一个对象，对象会在堆上开辟一块空间，同时会将这块空间的地址作为引用保存到虚拟机栈中，如果对象生命周期结束了，那么引用就会从虚拟机栈中出栈，因此如果在虚拟机栈中有引用，就说明这个对象还是有用的，这种情况是最常见的。
2. 第二种是我们在类中定义了全局的静态的对象，也就是使用了static关键字，由于虚拟机栈是线程私有的，所以这种对象的引用会保存在共有的方法区中，显然将方法区中的静态引用作为GC Roots是必须的。
3. 第三种便是常量引用，就是使用了static final关键字，由于这种引用初始化之后不会修改，所以方法区常量池里的引用的对象也应该作为GC Roots。
4. 第四种是在使用JNI技术时，有时候单纯的Java代码并不能满足我们的需求，我们可能需要在Java中调用C或C++的代码，因此会使用Native方法，JVM内存中专门有一块本地方法栈，用来保存这些对象的引用，所以本地方法栈中引用的对象也会被作为GC Roots。

GC Root步骤主要包含如下三步：
<a name="ynNKM"></a>
#### 2.1.1 可达性分析
![2021-06-25-23-06-23-334034.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633669847-64aca589-0e46-4bd3-8c55-20a3c8a47749.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u9585f823&originHeight=370&originWidth=534&originalType=binary&ratio=3&size=20592&status=done&style=shadow&taskId=u8a39e537-90c8-40d9-8185-c95afa4a6e6)<br />当一个对象到 GC Roots 没有任何引用链相连时，则判断该对象不可达。注意: 可达性分析仅仅只是判断对象是否可达，但还不足以判断对象是否存活 / 死亡。

<a name="UFrzI"></a>
#### 2.1.2 第一次标记 & 筛选
筛选的条件对象 如果没有重写`finalize`或者调用过finalize 则将该对象加入到F-Queue中
<a name="tNwHI"></a>
#### 2.1.3 第二次标记 & 筛选
当对象经过了第一次的标记 & 筛选，会被进行第二次标记 & 准备被进行筛选。经过F-Queue筛选后如果对象还没有跟GC Root建立引用关系则被回收，属于给个二次机会。<br />![2021-06-25-23-06-23-417867.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633680545-01a617c0-065a-475b-a76a-bf992697603e.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=uce7ff79b&originHeight=182&originWidth=1080&originalType=binary&ratio=3&size=52057&status=done&style=shadow&taskId=ucc627bb8-a4ad-4778-ab49-dbba86dbec8)
<a name="MSwS6"></a>
### 2.3、四大引用类型
<a name="MKtRm"></a>
#### 2.3.1 强引用
强引用(StrongReference)是使用最普遍的引用。垃圾回收器绝对不会回收它，内存不足时宁愿抛出OOM导致程序异常，平常的new 对象就是。
<a name="c5Ds3"></a>
#### 2.3.2 软引用
垃圾回收器在内存充足时不会回收软引用(SoftReference)对象，不足时会回收它，特别适合用于创建缓存。
<a name="H8r7o"></a>
#### 2.3.3 弱引用
弱引用(WeakReference)是在扫描到该对象时无论内存是否充足都会回收该对象。ThreadLocal 的Key就是弱引用。
<a name="MTch2"></a>
#### 2.3.4 虚引用
如果一个对象只具有虚引用(PhantomReference)那么跟没有任何引用一样，任何适合都可以被回收。主要用跟踪对象跟垃圾回收器回收的活动。
<a name="XIGsC"></a>
## 3、垃圾回收算法
为了挥手回收垃圾操作系统一般会使用标记清除、复制算法、标记整理三种算法，这三种各有优劣，简单介绍下：
<a name="fo47F"></a>
### 3.1、标记清除
![2021-06-25-23-06-23-495635.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633694539-ac655e9a-6951-4243-baef-005ba6517401.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u3ad40e97&originHeight=381&originWidth=556&originalType=binary&ratio=3&size=15369&status=done&style=shadow&taskId=u447e5da8-01b9-404b-b1de-ef5c82db919)<br />原理：<br />算法分为标记和清除两个阶段：首先标记出所有需要回收的对象，在标记完成后统一回收所有被标记的对象。<br />缺点：<br />标记清除之后会产生大量不连续的内存碎片，导致触发GC。
<a name="MYD3L"></a>
### 3.2、标记复制
![2021-06-25-23-06-23-583399.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633710256-25c37a70-48df-4467-bd98-b597a3485c9f.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u1566a38d&originHeight=344&originWidth=574&originalType=binary&ratio=3&size=5013&status=done&style=shadow&taskId=u18fbb82d-ec71-4649-a842-b85c3b41cbf)<br />原理：<br />将可用内存按容量划分为大小相等的两块，每次只使用其中的一块。当这一块的内存用完了，就将还存活着的对象复制到另外一块上面，然后再把已使用过的内存空间一次清理掉。<br />缺点：<br />这种算法的代价是将内存缩小为了原来的一半，还要来回移动数据。
<a name="q2lAi"></a>
### 3.3、标记整理
![2021-06-25-23-06-23-695614.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633721383-d73a8488-b1d4-416c-8cff-a2bbfe9da186.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u85833948&originHeight=397&originWidth=546&originalType=binary&ratio=3&size=5622&status=done&style=shadow&taskId=uae560224-fbbc-47e9-9144-95a4803ac74)<br />原理：<br />首先标记出所有需要回收的对象，在标记完成后，后续步骤是让所有存活的对象都向一端移动，然后直接清理掉端边界以外的内存。<br />缺点：<br />涉及到移动大量对象，效率不高。<br />总结：

| 指标 | 标记清理 | 标记整理 | 标记复制 |
| --- | --- | --- | --- |
| 速度 | 中等 | 最慢 | 快 |
| 空间开销 | 少（但会堆积碎片） | 少（不堆积碎片） | 通常需要活对象的2倍大小（不堆积碎片） |
| 移动对象 | 否 | 是 | 是 |

<a name="WjTYJ"></a>
### 3.4 、三色标记跟读写屏障
前面说的三种回收算法都说到了先标记，问题是如何标记的呢？<br />接下来的知识点面试应该问不到那么深了，但是为了深入必须Mark下！CMS、G1 标记时候一般用的是三色标记法，根据可达性分析从GC Roots开始进行遍历访问，可达的则为存活对象，而最终不可达说明就是需要被GC对象。大致流程是把遍历对象图过程中遇到的对象，按是否访问过这个条件标记成以下三种颜色：<br />白色：尚未访问过。<br />黑色：本对象已访问过，而且本对象 引用到 的其他对象 也全部访问过了。灰色：本对象已访问过，但是本对象 引用到 的其他对象 尚未全部访问完。全部访问后会转换为黑色。<br />![2021-06-25-23-06-24-896692.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633759154-5401afc1-0d27-436a-bc05-ac397bb320c8.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u441b603e&originHeight=641&originWidth=1080&originalType=binary&ratio=3&size=151983&status=done&style=shadow&taskId=u5cae442b-8324-4f31-9ac8-ff3da2dcb27)<br />假设现在有白、灰、黑三个集合(表示当前对象的颜色)，遍历访问过程：<br />1、初始时所有对象都在白色集合中。<br />2、将GC Roots 直接引用到的对象挪到灰色集合中。 <br />3、从灰色集合中获取对象：第一步将本对象 引用到的 其他对象 全部挪到灰色集合中，第二步将本对象 挪到黑色集合里面。 <br />4、重复步骤3，直至灰色集合为空时结束。 <br />5、结束后仍在白色集合的对象即为GC Roots 不可达，可以尝试进行回收。<br />当STW时对象间的引用是不会发生变化的，可以轻松完成标记。当支持并发标记时，对象间的引用可能发生变化，多标和漏标的情况就有可能发生。
<a name="yMDlp"></a>
#### 3.4 .1、浮动垃圾
状况：GC线程遍历到E(E是灰色)，一个业务线程执行了D.E = null，此时E应该被回收的。但是GC线程已经认为E是灰色了会继续遍历，导致E没有被回收。<br />![2021-06-25-23-06-24-971509.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633770073-742a33fd-a4f0-471e-b171-b4d9775697db.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u88162bf0&originHeight=312&originWidth=433&originalType=binary&ratio=3&size=20587&status=done&style=shadow&taskId=u1feb9384-a55f-476a-85d7-b4469679720)
<a name="PtHrl"></a>
#### 3.4 .2、漏标
![2021-06-25-23-06-25-046292.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633780675-51f5c579-cb9a-425d-9bc8-22c5606e4525.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u2a17424a&originHeight=310&originWidth=448&originalType=binary&ratio=3&size=22166&status=done&style=shadow&taskId=udc487290-7a55-43a0-8e84-8f13281d693)<br />GC线程遍历到E(灰色了)。业务线程执行了E-->G断开，D-->G链接的操作。GC线程发现E无法到达G，因为是黑色不会再遍历标记了。最终导致漏标G。漏标的必备两个条件：灰到白断开，黑到白建立。
```java
Object G = E.G;    // 第一步 ：读
Object E.G = null; // 第二步：写
Object D.G = G;   // 第三步：写
```
漏标解决方法：<br />将对象G存储到特定集合中，等并发标记遍历完毕后再对集合中对象进行重新标记。
<a name="yM2lI"></a>
##### 3.4.2.1、CMS方案
这里比如开始B指向C，但是后来B不指向C，A指向D，最简单的方法是将A变成灰色，等待下次进行再次遍历。CMS中可能引发ABA问题：<br />![2021-06-25-23-06-25-123088.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633794426-0200df5b-1e48-45ab-815f-2c84f268a659.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=uddcffe98&originHeight=289&originWidth=226&originalType=binary&ratio=3&size=8912&status=done&style=shadow&taskId=u87692253-e620-49c6-8a03-94eccfc23e6)<br />1、回收线程 m1 正在标记A，属性A.1标记完毕，正在标记属性A.2。<br />2、业务线程 m2 把属性1指向了C，由于CMS方案此时回收线程 m3 把A标记位灰色。 <br />3、回收线程 m1 认为所有属性标记完毕，将A设置为黑色，结果C漏标。所以CMS阶段需要重新标记。<br />![2021-06-25-23-06-25-189908.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633810863-86b86457-b209-4f0f-a193-8c93b41bbe83.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u5aec13c9&originHeight=311&originWidth=296&originalType=binary&ratio=3&size=4896&status=done&style=shadow&taskId=u932fec07-beb0-4d6b-8a3c-6e213868ce6)
<a name="jc8VA"></a>
##### 3.4.2.2、读写屏障
漏标的实现是有三步的，JVM加入了读写屏障，其中读屏障则是拦截第一步，写屏障用于拦截第二和第三步。<br />写屏障 + SATB(原始快照) 来破坏 灰到白断开。 <br />写屏障 + 增量更新 来破坏 黑到白建立。 <br />读屏障 一种保守方式来破坏灰到白断开后白的存储，此时用读屏障OK的。<br />现代使用可达性分析的垃圾回收器几乎都借鉴了三色标记的算法思想，尽管实现的方式不尽相同。对于读写屏障，以Java HotSpot VM为例，其并发标记时对漏标的处理方案如下：<br />CMS：写屏障 + 增量更新<br />G1：写屏障 + SATB<br />ZGC：读屏障<br />CMS中使用的增量更新，在重新标记阶段除了需要遍历 写屏障的记录，还需要重新扫描遍历GC Roots(标记过的不用再标记)，这是由于CMS对于astore_x等指令不添加写屏障的原因。
<a name="qeLuA"></a>
## 4、GC流程
核心思想就是根据各个年代的特点不同选用不同到垃圾收集算法。

1. 年轻代：使用复制算法
2. 老年代：使用标记整理或者标记清除算法。

为什么要有年轻代：<br />分代的好处就是优化GC性能，如果没有分代每次扫描所有区域能累死GC。因为很多对象几乎就是朝生夕死的，如果分代的话，我们把新创建的对象放到某一地方，当GC的时候先把这块存朝生夕死(80%以上)对象的区域进行回收，这样就会腾出很大的空间出来。
<a name="K58WE"></a>
### 4.1、 年轻代
HotSpot JVM把年轻代分为了三部分：1个Eden区和2个Survivor区(分别叫from和to)。默认比例为8:1:1。一般情况下，新创建的对象都会被分配到Eden区(一些大对象特殊处理)，这些对象经过第一次Minor GC后，如果仍然存活，将会被移到Survivor区。对象在Survivor区中每熬过一次Minor GC年龄就会增加1岁，当它的年龄增加到一定次数(默认15次)时，就会被移动到年老代中。年轻代的垃圾回收算法使用的是复制算法。<br />![2021-06-25-23-06-25-292633.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633824782-1bc6d8e3-daa6-4f5a-8e1e-6ccb32060df2.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u9c160e37&originHeight=460&originWidth=985&originalType=binary&ratio=3&size=47048&status=done&style=shadow&taskId=ubaabe2e5-03e5-4e58-a328-f3ac31276fb)<br />年轻代GC过程：GC开始前，年轻代对象只会存在于Eden区和名为From的Survivor区，名为To的Survivor区永远是空的。如果新分配对象在Eden申请空间发现不足就会导致GC。<br />yang GC：Eden区中所有存活的对象都会被复制到To，而在From区中，仍存活的对象会根据他们的年龄值来决定去向。年龄达到一定值(年龄阈值可以通过-XX:MaxTenuringThreshold来设置)的对象会被移动到年老代中，没有达到阈值的对象会被复制到To区域。经过这次GC后，Eden区和From区已经被清空。这个时候，From和To会交换他们的角色，也就是新的To就是上次GC前的From，新的From就是上次GC前的To。不管怎样都会保证名为To的Survivor区域是空的。Minor GC会一直重复这样的过程，直到To区被填满，To区被填满之后，会将所有对象移动到年老代中。这里注意如果yang GC 后空间还是不够用则会 空间担保 机制将数据送到Old区<br />卡表 Card Table：

1. 为了支持高频率的新生代回收，虚拟机使用一种叫做卡表(Card Table)的数据结构，卡表作为一个比特位的集合，每一个比特位可以用来表示年老代的某一区域中的所有对象是否持有新生代对象的引用。
2. 新生代GC时不用花大量的时间扫描所有年老代对象，来确定每一个对象的引用关系，先扫描卡表，只有卡表的标记位为1时，才需要扫描给定区域的年老代对象。而卡表位为0的所在区域的年老代对象，一定不包含有对新生代的引用。
<a name="pAhVb"></a>
### 4.2、 老年代
老年代GC过程：<br />老年代中存放的对象是存活了很久的，年龄大于15的对象 或者 触发了老年代的分配担保机制存储的大对象。在老年代触发的gc叫major gc也叫full gc。full gc会包含年轻代的gc。full gc采用的是 标记-清除 或 标记整理。在执行full gc的情况下，会阻塞程序的正常运行。老年代的gc比年轻代的gc效率上慢10倍以上。对效率有很大的影响。所以一定要尽量避免老年代GC！
<a name="dihZY"></a>
### 4.3、 元空间
永久代的回收会随着full gc进行移动，消耗性能。每种类型的垃圾回收都需要特殊处理 元数据。将元数据剥离出来，简化了垃圾收集，提高了效率。<br />-XX:MetaspaceSize 初始空间的大小。达到该值就会触发垃圾收集进行类型卸载，同时GC会对该值进行调整：<br />如果释放了大量的空间，就适当降低该值；如果释放了很少的空间，那么在不超过MaxMetaspaceSize时，适当提高该值。<br />`-XX:MaxMetaspaceSize`：<br />最大空间，默认是没有限制的。
<a name="bsdey"></a>
### 4.4 、垃圾回收流程总结
![2021-06-25-23-06-25-395363.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633935262-2ea57807-3bf4-4a21-af32-70627b3d2f13.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u8eac81a3&originHeight=369&originWidth=1080&originalType=binary&ratio=3&size=104627&status=done&style=shadow&taskId=u42f8d95a-6363-4a28-8718-6aea9cc2b33)<br />大致的GC回收流程如上图，还有一种设置就是 大对象直接进入老年代：

1. 如果在新生代分配失败且对象是一个不含任何对象引用的大数组，可被直接分配到老年代。通过在老年代的分配避免新生代的一次垃圾回收。
2. 设置了-XX:PretenureSizeThreshold 值，任何比这个值大的对象都不会尝试在新生代分配，将在老年代分配内存。

内存回收跟分配策略

1. 优先在Eden上分配对象，此区域垃圾回收频繁速度还快。
2. 大对象直接进入老生代。
3. 年长者(长期存活对象默认15次)跟 进入老生代。
4. 在Survivor空间中相同年龄所有对象大小的总和大于Survivor空间的一半，年龄大于或等于该年龄的对象会群体进入老生代。
5. 空间分配担保(担保minorGC)，如果Minor GC后 Survivor区放不下新生代仍存活的对象，把Suvivor 无法容纳的对象直接进人老年代。
<a name="q6A1X"></a>
## 5、垃圾收集器
<a name="EmvdO"></a>
### 5.1、 垃圾收集器
堆heap是垃圾回收机制的重点区域。我们知道垃圾回收机制有三种minor gc、major gc 和full gc。针对于堆的就是前两种。年轻代的叫 minor gc，老年代的叫major gc。

1. JDK7、JDK8 默认垃圾收集器 Parallel Scavenge（新生代）+ Parallel Old（老年代）
2. JDK9 默认垃圾收集器G1
3. 服务端开发常见组合就是 ParNew + CMS

![2021-06-25-23-06-25-508582.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633953412-a3a6332d-1e79-467c-a6de-2e402f3f084c.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u3d27ce42&originHeight=269&originWidth=379&originalType=binary&ratio=3&size=19599&status=done&style=shadow&taskId=u394b66c2-8d7b-49f8-b88a-c8d54dd75c1)<br />工程化使用的时候使用指定的垃圾收集器组合使用，讲解垃圾收集器前先普及几个重要知识点：<br />STW<br />java中Stop-The-World机制简称STW，是指执行垃圾收集算法时Java应用程序的其他所有线程都被挂起（除了垃圾收集帮助器之外）。是Java中一种全局暂停现象，全局停顿，所有Java代码停止，native代码虽然可以执行但不能与JVM交互，如果发生了STW 现象多半是由于gc引起。<br />吞吐量<br />吞吐量 = 运行用户代码时间 / ( 运行用户代码时间 + 垃圾收集时间 )。例如：虚拟机共运行100分钟，垃圾收集器花掉1分钟，那么吞吐量就是99%<br />垃圾收集时间<br />垃圾回收频率 * 单次垃圾回收时间<br />并行收集<br />指多条垃圾收集线程并行工作，但此时用户线程仍处于等待状态。<br />并发收集<br />指用户线程与垃圾收集线程同时工作（不一定是并行的可能会交替执行）。用户程序在继续运行，而垃圾收集程序运行在另一个CPU上。
<a name="kCJF8"></a>
### 5.2、 新生代
新生代有Serial、ParNew、Parallel Scavenge三种垃圾收集器。

| 名称 | 串行/并行/并发 | 回收算法 | 使用场景 | 可以跟CMS配合 |
| --- | --- | --- | --- | --- |
| Serial | 串行 | 复制 | 单CPU，Client模式下虚拟机 | 是 |
| ParNew | 并行(Serial的并行版) | 复制 | 多CPU，常在Server模式 | 是 |
| Parallel Scavenge | 并行 | 复制 | 多CPU且关注吞吐量 | 否 |

<a name="OpjvJ"></a>
### 5.3、 老年代
老年代有Serial Old、Parallel Old、CMS 三种垃圾收集器。

| 名称 | 串行/并行/并发 | 回收算法 | 使用场景 | 组合年轻代 |
| --- | --- | --- | --- | --- |
| Serial Old | 串行 | 标记整理 | 单CPU | Serial  、ParNew、Parallel Scavenge |
| Parallel Old | 并行 | 标记整理 | 多CPU | Parallel Scavenge |
| CMS | 并发 | 标记清除 | 多CPU且关注吞吐量，常用Server端 | Serial 、ParNew |

<a name="rsBM3"></a>
#### 5.3.1、CMS
CMS(Concurrent Mark Sweep)比较重要这里 重点说一下。<br />CMS的初衷和目的：<br />为了消除Throught收集器和Serial收集器在Full GC周期中的长时间停顿。是一种以获取最短回收停顿时间为目标的收集器，具有自适应调整策略，适合互联网站 跟B/S 服务应用。<br />CMS的适用场景：<br />如果你的应用需要更快的响应，不希望有长时间的停顿，同时你的CPU资源也比较丰富，就适合适用CMS收集器。比如常见的Server端任务。<br />优点：<br />并发收集、低停顿。<br />缺点：

1. CMS收集器对CPU资源非常敏感：在并发阶段，虽然不会导致用户线程停顿，但是会占用CPU资源而导致引用程序变慢，总吞吐量下降。
2. 无法处理浮动垃圾：由于CMS并发清理阶段用户线程还在运行，伴随程序的运行自热会有新的垃圾不断产生，这一部分垃圾出现在标记过程之后，CMS无法在本次收集中处理它们，只好留待下一次GC时将其清理掉。这一部分垃圾称为浮动垃圾。如果内存放不下浮动垃圾这时 JVM 启动 Serial Old 替代 CMS。
3. 空间碎片：CMS是基于标记-清除算法实现的收集器，使用标记-清除算法收集后，会产生大量碎片。

CMS回收流程：

1. 初始标记：引发STW， 仅仅只是标记出GC ROOTS能直接关联到的对象，速度很快。
2. 并发标记：不引发STW，正常运行 所有Old 对象是否可链到GC Roots
3. 重新标记：引发STW，为了修正并发标记期间，因用户程序继续运作而导致标记产生改变的标记。这个阶段的停顿时间会被初始标记阶段稍长，但比并发标记阶段要短。
4. 并发清除：不引发STW，正常运行，标记清除算法来清理删除掉标记阶段判断的已经死亡的对象。

总结：

1. 并发标记和并发清除的耗时最长但是不需要停止用户线程。初始标记和重新标记的耗时较短，但是需要停止用户线程，所以整个GC过程造成的停顿时间较短，大部分时候是可以和用户线程一起工作的。
<a name="Rh90k"></a>
### 5.4、G1
之前的GC收集器对Heap的划分：<br />![2021-06-25-23-06-25-608562.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624633972210-373b5f54-33c8-4576-97ef-a93fdfe11f02.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=u44121020&originHeight=204&originWidth=835&originalType=binary&ratio=3&size=12922&status=done&style=shadow&taskId=u2b2f8756-1042-4a53-9e79-ca349d80e8c)<br />以前垃圾回收器是 新生代 + 老年代，用了CMS效果也不是很好，为了减少STW对系统的影响引入了G1(Garbage-First Garbage Collector)，G1是一款面向服务端应用的垃圾收集器，具有如下特点：<br />1、并行与并发：G1能充分利用多CPU、多核环境下的硬件优势，可以通过并发的方式让Java程序继续执行。<br />2、分代收集：分代概念在G1中依然得以保留，它能够采用不同的方式去处理新创建的对象和已经存活了一段时间、熬过多次GC的旧对象来获得更好的收集效果。 <br />3、空间整合：G1从整体上看是基于标记-整理算法实现的，从局部(两个Region之间)上看是基于复制算法实现的，G1运行期间不会产生内存空间碎片。 <br />4、可预测停顿：G1比CMS牛在能建立可预测的停顿时间模型，能让使用者明确指定在一个长度为M毫秒的时间片段内，消耗在垃圾收集上的时间不得超过N毫秒。<br />G1作为JDK9之后的服务端默认收集器，不再区分年轻代和老年代进行垃圾回收，G1默认把堆内存分为N个分区，每个1~32M(总是2的幂次方)。并且提供了四种不同Region标签Eden、Survivor 、Old、 Humongous。H区可以认为是Old区中一种特列专门用来存储大数据的，关于H区数据存储类型一般符合下面条件：<br />当 0.5 Region <=  当对象大小 <= 1 Region 时候将数据存储到 H区 <br />当对象大小 > 1 Region 存储到连续的H区。<br />![2021-06-25-23-06-25-694332.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624634052948-cc8c8e90-36f2-4ef5-8fce-352a6abb23a2.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=ue91dbed4&originHeight=337&originWidth=681&originalType=binary&ratio=3&size=690025&status=done&style=shadow&taskId=u97754273-a478-4ccd-b9ca-2ed32de6be5)<br />同时G1中引入了RememberSets、CollectionSets帮助更好的执行GC 。<br />1、RememberSets： RSet 记录了其他Region中的对象引用本Region中对象的关系，属于points-into结构（谁引用了我的对象） <br />2、CollectionSets：Csets 是一次GC中需要被清理的regions集合，注意G1每次GC不是全部region都参与的，可能只清理少数几个，这几个就被叫做Csets。在GC的时候，对于old -> young 和old -> old的跨代对象引用，只要扫描对应的CSet中的RSet即可。<br />G1进行GC的时候一般分为Yang GC跟Mixed GC。<br />Young GC：CSet 就是所有年轻代里面的Region<br />Mixed GC：CSet 是所有年轻代里的Region加上在全局并发标记阶段标记出来的收益高的Region
<a name="fPd75"></a>
#### 5.4.1、Yang GC
![2021-06-25-23-06-25-806290.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624634092975-0e2a9964-0ffe-4f20-adea-c5224048b97b.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=uf9de7573&originHeight=432&originWidth=556&originalType=binary&ratio=3&size=722258&status=done&style=shadow&taskId=u4430e3cb-949f-4b89-a14c-947e46d5f0b)<br />标准的年轻代GC算法，整体思路跟CMS中类似。
<a name="Xbenw"></a>
#### 5.4.2、Mixed GC
G1中是没有Old GC的，有一个把老年代跟新生代同时GC的 Mixed GC，它的回收流程：<br />1、初始标记：是STW事件，其完成工作是标记GC ROOTS 直接可达的对象。标记位RootRegion。<br />2、根区域扫描 ：不是STW事件，拿来RootRegion，扫描整个Old区所有Region，看每个Region的Rset中是否有RootRegion。有则标识出来。 <br />3、并发标记 ：同CMS并发标记 不需要STW，遍历范围减少，在此只需要遍历 第二步 被标记到引用老年代的对象 RSet。 <br />4、最终标记 ：同 CMS 重新标记 会STW ，用的SATB操作，速度更快。<br />5、清除 ：STW操作，用 复制清理算法，清点出有存活对象的Region和没有存活对象的Region(Empty Region)，更新Rset。把Empty Region收集起来到可分配Region队列。<br />回收总结：<br />1、经过global concurrent marking，collector就知道哪些Region有存活的对象。并将那些完全可回收的Region(没有存活对象)收集起来加入到可分配Region队列，实现对该部分内存的回收。对于有存活对象的Region，G1会根据统计模型找处收益最高、开销不超过用户指定的上限的若干Region进行对象回收。这些选中被回收的Region组成的集合就叫做collection set 简称Cset！ <br />2、在MIX GC中的Cset = 所有年轻代里的region + 根据global concurrent marking统计得出收集收益高的若干old region。 <br />3、在YGC中的Cset = 所有年轻代里的region + 通过控制年轻代的region个数来控制young GC的开销。 <br />4、YGC 与 MIXGC 都是采用多线程复制清理，整个过程会STW。G1的低延迟原理在于其回收的区域变得精确并且范围变小了。<br />G1提速点：<br />1 重新标记时X区域直接删除。<br />2 Rset降低了扫描的范围，上题中两点。 <br />3 重新标记阶段使用SATB速度比CMS快。 <br />4 清理过程为选取部分存活率低的Region进行清理，不是全部，提高了清理的效率。<br />总结：<br />就像你妈让你把自己卧室打扫干净，你可能只把显眼而比较大的垃圾打扫了，犄角旮旯的你没打扫。关于G1 还有很多细节其实没看到也。<br />一句话总结G1思维：每次选择性的清理大部分垃圾来保证时效性跟系统的正常运行。
<a name="BasjD"></a>
## 6、New个对象
一个Java类从编码到最终完成执行，主要包括两个过程，编译、运行。<br />编译：将我们写好的.java文件通过Javac命令编译成.class文件。 <br />运行：把编译生成的.class文件交由JVM执行。<br />Jvm运行class类的时候，并不是一次性将所有的类都加载到内存中，而是用到哪个就加载哪个，并且只加载一次。
<a name="mV63q"></a>
### 6.1、类的生命周期
![2021-06-25-23-06-25-919986.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624634110102-55439c04-c420-4310-b422-d13c0cb9a783.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=ub32bfbbd&originHeight=247&originWidth=592&originalType=binary&ratio=3&size=13768&status=done&style=shadow&taskId=ub9b68bf3-6f48-401e-a8ee-1c1a9100f60)
<a name="wcUTa"></a>
#### 6.1.1、 加载
加载指的是把class字节码文件从各个来源通过类加载器装载入内存中，这里有两个重点：

1. 字节码来源：一般的加载来源包括从本地路径下编译生成的.class文件，从jar包中的.class文件，从远程网络，以及动态代理实时编译
2. 类加载器：一般包括启动类加载器，扩展类加载器，应用类加载器，以及用户的自定义类加载器(加密解密那种)。
<a name="t8MNF"></a>
#### 6.1.2、 验证
主要是为了保证加载进来的字节流符合虚拟机规范，不会造成安全错误。文件格式验证、元数据验证、字节码验证、符号引用验证。
<a name="YqSRM"></a>
#### 6.1.3、 准备
给类静态变量分配内存空间，仅仅是分配空间，比如 public static int age = 14，在准备后age = 0，在初始化阶段 age = 14，如果添加了final则在这个阶段直接赋值为14。
<a name="hefE2"></a>
#### 6.1.4、 解析
将常量池内的符号引用替换为直接引用。<br />![2021-06-25-23-06-26-074572.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624634120864-f8ee5787-7939-438d-af21-8216de96d999.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=ub538d9e7&originHeight=66&originWidth=584&originalType=binary&ratio=3&size=2811&status=done&style=shadow&taskId=u55fee022-735b-4529-bb2e-f2a5cfc1d89)
<a name="izrRX"></a>
#### 6.1.5、 初始化
前面在加载类阶段用户应用程序可以通过自定义类加载器参与之外 其余动作完全由虚拟机主导和控制。此时才是真正开始执行类中定义的代码 ：执行static代码块进行初始化，如果存在父类，先对父类进行初始化。
<a name="T7Ga7"></a>
#### 6.1.6、 使用
类加载完毕后紧接着就是为对象分配内存空间和初始化了：

1. 为对象分配合适大小的内存空间
2. 为实例变量赋默认值
3. 设置对象的头信息，对象hash码、GC分代年龄、元数据信息等
4. 执行构造函数(init)初始化。
<a name="V76lq"></a>
#### 6.1.7、 卸载
最终没啥说等，就是通过GC算法回收对象了。
<a name="N8Erw"></a>
### 6.2、 对象占据字节
关于对象头问题在 Synchronized 一文中已经详细写过了，一个对象头包含三部分对象头(MarkWord、classPointer)、实例数据Instance Data、对齐Padding，想看内存详细占用情况IDEA调用jol-core包即可。<br />问题一：new Object()占多少字节

1. markword 8字节 + classpointer 4字节(默认用calssPointer压缩) + padding 4字节  = 16字节
2. 如果没开启classpointer压缩：markword 8字节 + classpointer 8字节 = 16字节

问题二：User (int id,String name) User u = new User(1,"李四")<br />markword 8字节 + 开启classPointer压缩后classpointer 4字节 + instance data int 4字节 + 开启普通对象指针压缩后String4字节 + padding 4  = 24字节
<a name="ZM9XV"></a>
### 6.3、 对象访问方式
![2021-06-25-23-06-26-169973.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624634133187-8dbde6a7-a7d2-4c4d-b98e-d69b69a42c78.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=ue4d895bf&originHeight=275&originWidth=1065&originalType=binary&ratio=3&size=60241&status=done&style=shadow&taskId=u7aa90806-f3a1-4b1e-9992-d46a7c97fcb)<br />使用句柄：<br />使用句柄来访问的最大好处就是reference中存储的是稳定的句柄地址，在对象被移动（垃圾收集时移动对象是非常普遍的行为）时只会改变句柄中的实例数据指针，而reference本身不需要修改。<br />直接指针：<br />reference中存储的直接就是对象地址。最大好处就是速度更快，它节省了一次指针定位的时间开销，由于对象的访问在Java中非常频繁，因此这类开销积少成多后也是一项非常可观的执行成本。<br />Sun HotSpot 使用 直接指针访问方式 进行对象访问的。
<a name="t3k9s"></a>
## 7、对象一定创建在堆上吗
结论：不一定 看对象经过了逃逸分析后发现该变量只是用到方法区时，则JVM会自动优化，在栈上创建该对象。
<a name="zwDul"></a>
### 7.1、逃逸分析
逃逸分析(Escape Analysis)简单来讲就是：Java Hotspot 虚拟机可以分析新创建对象的使用范围，并决定是否在 Java 堆上分配内存。
<a name="vPHfK"></a>
### 7.2、标量替换
标量替换：JVM通过逃逸分析确定该对象不会被外部访问。那就通过将该对象标量替换分解在栈上分配内存，这样该对象所占用的内存空间就可以随栈帧出栈而销毁，就减轻了垃圾回收的压力。<br />标量：不可被进一步分解的量，而JAVA的基本数据类型就是标量 <br />聚合量：在JAVA中对象就是可以被进一步分解的聚合量。
<a name="RyNge"></a>
### 7.3、栈上分配
JVM对象分配在堆中，当对象没有被引用时，依靠GC进行回收内存，如果对象数量较多会给GC带来较大压力，也间接影响了应用的性能。<br />为了减少临时对象在堆内分配的数量，JVM通过逃逸分析确定该对象不会被外部访问。那就通过将该对象标量替换分解在栈上分配内存，这样该对象所占用的内存空间就可以随栈帧出栈而销毁，就减轻了垃圾回收的压力。
<a name="s2x70"></a>
### 7.4、同步消除
同步消除是java虚拟机提供的一种优化技术。通过逃逸分析，可以确定一个对象是否会被其他线程进行访问，如果对象没有出现线程逃逸，那该对象的读写就不会存在资源的竞争，不存在资源的竞争，则可以消除对该对象的同步锁。比如方法体内调用StringBuffer。<br />逃逸分析结论：<br />虽然经过逃逸分析可以做标量替换、栈上分配、和锁消除。但是逃逸分析自身也是需要进行一系列复杂的分析的，这其实也是一个相对耗时的过程。如果对象经过层层分析后发现 无法进行逃逸分析优化则反而耗时了，因此慎用。
<a name="KLj4q"></a>
## 8、类加载器
在连接阶段一般是无法干预的，大部分干预 类加载阶段，对于任意一个类，都需要由加载它的类加载器和这个类本身一同确立其在Java虚拟机中的唯一性，类加载时候重要三个方法：<br />1、loadClass() ：加载目标类的入口，它首先会查找当前 ClassLoader 以及它的双亲里面是否已经加载了目标类，找到直接返回 <br />2、findClass() ：如果没有找到就会让双亲尝试加载，如果双亲都加载不了，就会调用 findClass() 让自定义加载器自己来加载目标类 <br />3、defineClass() ：拿到这个字节码之后再调用 defineClass() 方法将字节码转换成 Class 对象。
<a name="FuXc3"></a>
### 8.1、双亲委派机制
![2021-06-25-23-06-26-242780.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624634148181-6b73b43e-f282-439a-920b-a9d8cda8ac2b.png#clientId=u80cbfcf6-6ab8-4&from=ui&id=ubd28eff0&originHeight=316&originWidth=438&originalType=binary&ratio=3&size=27235&status=done&style=shadow&taskId=ud7624646-1cef-47a5-a344-be217cad645)<br />定义：<br />当某个类加载器需要加载某个.class文件时，首先把这个任务委托给他的上级类加载器，递归这个操作，如果上级的类加载器没有加载，自己才会去加载这个类。<br />作用：<br />1、可以防止重复加载同一个.class。通过委托去向上面问一问，加载过了，就不用再加载一遍。保证数据安全。 <br />2、保证核心.class不能被篡改*，通过委托方式，不会去篡改核心.class。’<br />类加载器：<br />1、`BootstrapClassLoader`（启动类加载器）：c++编写，加载java核心库 java.*，JAVA_HOME/lib<br />2、`ExtClassLoader `（标准扩展类加载器）：java编写的加载扩展库，JAVA_HOME/lib/ext<br />3、`AppClassLoader`（系统类加载器）：加载程序所在的目录，如user.dir所在的位置的ClassPath<br />4、`CustomClassLoader`（用户自定义类加载器）：用户自定义的类加载器,可加载指定路径的class文件
<a name="tqlc0"></a>
### 8.2、关于加载机制
双亲委派机制只是Java类加载的一种常见模式，还有别的加载机制哦，比如Tomcat 总是先尝试去加载某个类，如果找不到再用上一级的加载器，跟双亲加载器顺序正好相反。再比如当使用第三方框架JDBC跟具体实现的时候，反而会引发错误，因为JDK自带的JDBC接口由启动类加载，而第三方实现接口由应用类加载。这样相互之间是不认识的，因此JDK引入了SPI机制 线程上下文加载器 来实现加载(跟Dubbo的SPI不一样哦)。
<a name="C005y"></a>
## 9、OOM 、CPU100%
系统性能分析常用指令：

| 工具 | 用途 |
| --- | --- |
| jps | 输出JVM中运行的进程状态信息 |
| jstack | 生成虚拟机当前时刻的线程快照 |
| jstat | 虚拟机统计信息监控工具 |
| jinfo | 实时地查看和调整虚拟机各项参数 |
| jmap | 生成虚拟机的内存转储快照，heapdump文件 |
| JConsole | 可视化管理工具，常用 |

<a name="Eyw7b"></a>
### 9.1、OOM
<a name="qgFYB"></a>
#### 9.1.1、为啥OOM
发生 OOM 简单来说可总结为两个原因：

1. 分配给 JVM的 内存不够用。
2. 分配内存够用，但代码写的不好，多余的内存 没有释放，导致内存不够用。
<a name="G4XEU"></a>
#### 9.1.2、三种类型OOM
<a name="zAbHG"></a>
###### 9.2.1、堆内存溢出：
此种情况最常见 Java heap space。一般是先通过内存映像工具对Dump出来的堆转储快照然后辨别到底是内存泄漏还是内存溢出。内存泄漏<br />通过工具查看泄漏对象到GC Roots的引用链。找到泄漏的对象是通过怎么样的路径与GC Roots相关联的导致垃圾回收机制无法将其回收，最终比较准确地定位泄漏代码的位置。<br />不存在泄漏<br />就是内存中的对象确实必须存活着，那么此时就需要通过虚拟机的堆参数，从代码上检查是否存在某些对象存活时间过长、持有时间过长的情况，尝试减少运行时内存的消耗。
<a name="Knacb"></a>
#### 9.2.2、虚拟机栈和本地方法栈溢出
在HotSpot虚拟机上不区分虚拟机栈和本地方法栈，因此栈容量只能由**-Xss**参数设定。在Java虚拟机规范中描述了两种异常：<br />StackOverflowError ：线程请求的栈深度超过了虚拟机所允许的最大深度，就会抛出该异常。OutOfMemoryError：虚拟机在拓展栈的时候无法申请到足够的空间，就会抛出该异常。<br />单线程环境下无论是由于栈帧太大还是虚拟机栈容量太小，当内存无法继续分配的时候，虚拟机抛出的都是StackOverflowError 异常。<br />多线程环境下为每个线程的栈分配的内存越大，每个线程获得空间大则可建立的线程数减少了反而越容易产生OOM异常，因此一般通过减少最大堆 和 减少栈容量 来换取更多的线程数量。
<a name="FtjBA"></a>
#### 9.2.3、永久代溢出：
PermGen space 即方法区溢出了。方法区用于存放Class的相关信息，如类名、访问修饰符、常量池、字段描述、方法描述等。当前的一些主流框架，如Spring、Hibernate，对于类进行增强的时候都会使用到CGLib这类字节码技术，增强的类越多，就需要越大的方法区来保证动态生成Class可以加载入内存，这样的情况下可能会造成方法区的OOM异常。
<a name="EwO83"></a>
#### 9.2.4、OOM查看指令

1. 通过命令查看对应的进程号  ：

比如：`jps`  或者  `ps -ef | grep` 需要的任务

1. 输入命令查看gc情况命令：

`jstat -gcutil` 进程号 刷新的毫秒数 展示的记录数 <br />比如：`jstat -gcutil 1412 1000 10`  （查看进程号1412，每隔1秒获取下，展示10条记录）

1. 查看具体占用情况：

命令：`jmap -histo 进程号 | more`  （默认展示到控制台） <br />比如：`jmap -histo 1412 | more`    查看具体的classname，是否有开发人员的类，也可以输出到具体文件分析
<a name="PscYH"></a>
### 9.3 CPU 100%
线上应用导致 CPU 占用 100%，  出现这样问题一般情况下是代码进入了死循环，分析步骤如下：

1. 找出对应服务进程id :

用 `ps -ef | grep` 运行的服务名字，直接top命令也可以看到各个进程CPU使用情况。

1. 查询目标进程下所有线程的运行情况 :

`top -Hp pid`， `-H`表示以线程的维度展示，默认以进程维度展示。

1. 对目标线程进行10进制到16进制转换：

`printf '%x\n' 线程pid`

1. 用jstack 进程id | grep 16进制线程id 找到线程信息，具体分析：

`jstack 进程ID | grep -A 20 16进制线程id`
<a name="bpuxh"></a>
## 10、GC调优
一般项目加个xms和xmx参数就够了。在没有全面监控、收集性能数据之前，调优就是瞎调。出现了问题先看自身代码或者参数是否不合理，毕竟不是谁都能写JVM底层代码的。一般要减少创建对象的数量， 减少使用全局变量和大对象， GC 优化是到最后不得已才采用的手段。日常 分析 GC 情况 优化代码比优化 GC 参数要多得多。一般如下情况不用调优的：<br />1、minor GC 单次耗时 < 50ms，频率10秒以上。说明年轻代OK。 <br />2、Full GC 单次耗时 < 1秒，频率10分钟以上，说明年老代OK。<br />GC调优目的：GC时间够少，GC次数够少。<br />调优建议：

1. `-Xms5m`设置JVM初始堆为5M，`-Xmx5m`设置JVM最大堆为5M。`-Xms`跟`-Xmx`值一样时可以避免每次垃圾回收完成后JVM重新分配内存。
2. `-Xmn2g`:设置年轻代大小为2G，一般默认为整个堆区的1/3 ~ 1/4。-Xss每个线程栈空间设置。
3. `-XX:SurvivorRatio`，设置年轻代中Eden区与Survivor区的比值，默认=8，比值为8:1:1。
4. `-XX:+HeapDumpOnOutOfMemoryError` 当JVM发生OOM时，自动生成DUMP文件。
5. `-XX:PretenureSizeThreshold` 当创建的对象超过指定大小时，直接把对象分配在老年代。
6. `-XX:MaxTenuringThreshold` 设定对象在Survivor区最大年龄阈值，超过阈值转移到老年代，默认15。
7. 开启GC日志对性能影响很小且能定位问题，`-XX:+PrintGCTimeStamps` `-XX:+PrintGCDetails` `-Xloggc:gc.log`
