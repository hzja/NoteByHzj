JVM 调优<br />![2021-10-06-13-39-49-942190.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498873059-c294bd8d-b010-4e15-9819-22daa29d9e8b.png#clientId=u55ea4475-fdcb-4&from=ui&id=ua6edecc1&originHeight=716&originWidth=1080&originalType=binary&ratio=1&size=113733&status=done&style=shadow&taskId=uc382293c-3d2f-47e4-89a7-7dae91d3e50)
<a name="bja7l"></a>
## JVM的分类
这里，先来说说什么是VM吧，VM的中文含义为：虚拟机，指的是使用软件的方式模拟具有完整硬件系统功能、运行在一个完全隔离环境中的完整计算机系统，是物理机的软件实现。<br />常用的虚拟机有：VMWare、Virtual Box，Java Virtual Machine（JVM，Java虚拟机）。<br />这里，重点聊的就是JVM，Java虚拟机。看下图。<br />![2021-10-06-13-39-50-527192.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498872895-1305579d-a679-4918-b04c-19964c3caf6a.png#clientId=u55ea4475-fdcb-4&from=ui&id=ffZs5&originHeight=419&originWidth=525&originalType=binary&ratio=1&size=19150&status=done&style=shadow&taskId=uc327097e-ca5d-41d8-85ed-46bb40e5c74)<br />这张图看起来还是比较简单的，JVM运行于操作系统之上，操作系统是运行在计算机硬件上的。<br />关于JVM，其实有很多大厂开发了不同版本的JVM，比较知名的有：Sun HotSpot VM、BEA JRockit VM、IBM J9 VM、 Azul VM、 Apache Harmony、 Google Dalvik VM、 Microsoft JVM等等。<br />现在使用的比较多的JDK8版本就是Sun HotSpot VM与BEA JRockit VM合并之后开发出的JDK版本。
<a name="uAsT8"></a>
## JVM的构成
JVM主要由三个子系统构成，分别为：类加载器子系统、运行时数据区（内存结构）和字节码执行引擎。<br />![2021-10-06-13-39-51-029223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498872973-c27f54dd-82f0-4ae9-bb91-b6fd1ada3a36.png#clientId=u55ea4475-fdcb-4&from=ui&id=knPp7&originHeight=279&originWidth=652&originalType=binary&ratio=1&size=20571&status=done&style=shadow&taskId=udc1544bf-ba54-44e8-be6d-e2dd2df1546)<br />为了更好的理解JVM，来看一下JVM的全貌图。<br />![2021-10-06-13-39-51-148222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498899320-8ddf3b82-8037-4b95-b9b2-35d3a974b54d.png#clientId=u55ea4475-fdcb-4&from=ui&id=ub9ba6cee&originHeight=729&originWidth=798&originalType=binary&ratio=1&size=67783&status=done&style=shadow&taskId=u14586218-0c13-4aee-a189-e8aad4c711a)<br />当开发Java程序时，首先会编写.java文件，之后，会将.java文件编译成.class文件。<br />JVM中，会通过类装载子系统将.class文件的内容装载到JVM的运行时数据区，而JVM的运行时数据区又会分为：方法区、堆、栈、本地方法栈和程序计数器 几个部分。<br />在装载class文件的内容时，会将class文件的内容拆分为几个部分，分别装载到JVM运行时数据区的几个部分。其中，值得注意的是：程序计数器的作用是：记录程序执行的下一条指令的地址。<br />方法区也叫作元空间，主要包含了：运行时常量池、类型信息、字段信息、方法信息、类加载器的引用、对应的Class实例的引用等信息。<br />在JVM中，程序的执行是通过执行引擎进行的，执行引擎会调用本地方法的接口来执行本地方法库，进而完成整个程序逻辑的执行。<br />常说的垃圾收集器是包含在执行引擎中的，在程序的运行过程中，执行引擎会开启垃圾收集器，并在后台运行，垃圾收集器会不断监控程序运行过程中产生的内存垃圾信息，并根据相应的策略对垃圾信息进行清理。<br />这里，大家需要注意的是：栈、本地方法栈和程序计数器是每个线程运行时独占的，而方法区和堆是所有线程共享的。所以，栈、本地方法栈和程序计数器不会涉及线程安全问题，而方法区和堆会涉及线程安全问题。
<a name="M0HvQ"></a>
### 方法区（元空间）
很多小伙伴一看到方法区三个字，脑海中的第一印象可能是存储方法的地方吧。<br />实则不然，方法区的另一个名字叫作元空间，相信不少小伙伴或多或少的听说过元空间。这个区域是JDK1.8中划分出来的。主要包含：运行时常量池、类型信息、字段信息、方法信息、类加载器的引用、对应的Class实例的引用等信息。方法区中的信息能够被多个线程共享。<br />例如，在程序中声明的常量、静态变量和有关于类的信息等的引用，都会存放在方法区，而这些引用所指向的具体对象 一般都会在堆中开辟单独的空间进行存储，也可能会在直接内存中进行存储。<br />![2021-10-06-13-39-51-296223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498899325-9634a824-8e23-48a9-bdae-bdad705d9998.png#clientId=u55ea4475-fdcb-4&from=ui&id=tCyDM&originHeight=256&originWidth=542&originalType=binary&ratio=1&size=21047&status=done&style=shadow&taskId=u4a6b173c-bc9c-4976-888a-0cf1165f592)
<a name="UgH23"></a>
### 堆
堆中主要存储的是实际创建的对象，也就是会存储通过new关键字创建的对象，堆中的对象能够被多个线程共享。堆中的数据不需要事先明确生存期，可以动态的分配内存，不再使用的数据和对象由JVM中的GC机制自动回收。对JVM的性能调优一般就是对堆内存的调优。<br />Java中基本类型的包装类：Byte、Short、Integer、Long、Float、Double、Boolean、Character类型的数据是存储在堆中的。<br />堆一般会被分成年轻代和老年代。而年轻代又会被进一步分为1个Eden区和2个Survivor区。在内存分配上，如果保持默认配置的话，年轻代和老年代的内存大小比例为1 : 2，年轻代中的1个Eden区和2个Survivor区的内存大小比例为：8 : 1 : 1。<br />![2021-10-06-13-39-51-390222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498899341-b1cd16b7-c875-462a-a7d5-1b9dff0cc900.png#clientId=u55ea4475-fdcb-4&from=ui&id=Sxz5D&originHeight=382&originWidth=867&originalType=binary&ratio=1&size=33924&status=done&style=shadow&taskId=uc8956fc7-c1c1-4c4a-b0b7-f9da02781ba)
<a name="BuONY"></a>
### 栈
栈一般又叫作线程栈或虚拟机栈，一般存储的是局部变量。在Java中，每个线程都会有一个单独的栈区，每个栈中的元素都是私有的，不会被其他的栈所访问。栈中的数据大小和生存期都是确定的，存取速度比较快。<br />在Java中，所有的基本数据类型（byte、short、int、long、float、double、boolean、char）和引用变量（对象引用）都是在栈中的。一般情况下，线程退出或者方法退出时，栈中的数据会被自动清除。<br />程序在执行过程中，会在栈中为不同的方法创建不同的栈帧，在栈帧中又包含了：局部变量表、操作数栈、动态链接和方法出口。<br />![2021-10-06-13-39-51-524223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498938783-87aa3877-75d4-4303-a376-4577fc3c2e68.png#clientId=u55ea4475-fdcb-4&from=ui&id=u31cd22df&originHeight=755&originWidth=699&originalType=binary&ratio=1&size=43037&status=done&style=shadow&taskId=ubf3b7e36-1d3e-4e9e-8af4-f66b289dbe0)<br />栈中一般会存储对象的引用，这些引用所指向的具体对象一般都会在堆中开辟单独的地址空间进行存储，也有可能存储在直接内存中。<br />![2021-10-06-13-39-51-631223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498938839-ecf23bca-e303-4585-b139-22e6574d5b3d.png#clientId=u55ea4475-fdcb-4&from=ui&id=INvpH&originHeight=268&originWidth=555&originalType=binary&ratio=1&size=19681&status=done&style=shadow&taskId=u4394a969-dc86-49dc-ab93-5880baba65e)<br />注意：这里说的是这些引用所指向的具体对象一般都会在堆中开辟单独的地址空间进行存储，也有可能存储在直接内存中。<br />因为在JVM中，如果开启了逃逸分析和标量替换，则可能不会再在堆上创建对象，可能会将对象直接分配到栈上，也可能不再创建对象，而是进一步分解对象中的成员变量，将其直接在栈上分配空间并赋值。
<a name="y92ZA"></a>
### 本地方法栈
本地方法栈相对来说比较简单，就是保存`native`方法进入区域的地址。<br />例如，在Java中创建线程，调用Thread对象的`start()`方法时，会通过本地方法`start0()`调用操作系统创建线程的方法。此时，本地方法栈就会保存`start0()`方法进入区域的内存地址。
<a name="Bzb7q"></a>
### 程序计数器
程序计数器也叫作PC计数器，主要存储的是下一条将要执行的命令的地址。
<a name="KhVvG"></a>
## JVM调优参数
在JVM中，主要是对堆（新生代）、方法区和栈进行性能调优。各个区域的调优参数如下所示。

- 堆：`-Xms`、`-Xmx`
- 新生代：`-Xmn`
- 方法区（元空间）：`-XX:MetaspaceSize`、`-XX:MaxMetaspaceSize`
- 栈（线程）：`-Xss`

为了更加直观的表述，可以将JVM的内存区域和对应的调优参数总结成下图所示。<br />![2021-10-06-13-39-51-755225.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498938835-544698f8-8ce3-4c30-8e77-6d696276c797.png#clientId=u55ea4475-fdcb-4&from=ui&id=EyuQA&originHeight=403&originWidth=752&originalType=binary&ratio=1&size=35124&status=done&style=shadow&taskId=ue7095663-5b5d-4494-a8f2-7970c55a4f9)<br />在设置JVM启动参数时，需要特别注意方法区（元空间）的参数设置。<br />关于方法区（元空间）的JVM参数主要有两个：`-XX:MetaspaceSize`和`-XX:MaxMetaspaceSize`。<br />`-XX:MetaspaceSize`： 指的是方法区（元空间）触发Full GC的初始内存大小（方法区没有固定的初始内存大小），以字节为单位，默认为21M。达到设置的值时，会触发Full GC，同时垃圾收集器会对这个值进行修改。<br />如果在发生Full GC时，回收了大量内存空间，则垃圾收集器会适当降低此值的大小；如果在发生Full GC时，释放的空间比较少，则在不超过设置的`-XX:MetaspaceSize`值或者在没设置`-XX:MetaspaceSize`的值时不超过21M，适当提高此值。<br />`-XX:MaxMetaspaceSize`： 指的是方法区（元空间）的最大值，默认值为-1，不受堆内存大小限制，此时，只会受限于本地内存大小。<br />最后需要注意的是： 调整方法区（元空间）的大小会发生Full GC，这种操作的代价是非常昂贵的。如果发现应用在启动的时候发生了Full GC，则很有可能是方法区（元空间）的大小被动态调整了。<br />所以，为了尽量不让JVM动态调整方法区（元空间）的大小造成频繁的Full GC，一般将`-XX:MetaspaceSize`和`-XX:MaxMetaspaceSize`设置成一样的值。例如，物理内存8G，可以将这两个值设置为256M<br />最后一起看下在物理内存8G的情况下，启动应用程序时，可以设置的JVM参数。当然，这里给出的是一些经验值，实际部署到生产环境时，需要经过压测找到最佳的参数值。

- 启动SpringBoot
```bash
java ‐Xms2048M ‐Xmx2048M ‐Xmn1024M ‐Xss512K ‐XX:MetaspaceSize=256M ‐XX:MaxMetaspaceSize=256M ‐jar xxx.jar
```

- 启动Tomcat（Linux）

在Tomcat bin目录下catalina.sh文件里配置。
```bash
‐Xms2048M ‐Xmx2048M ‐Xmn1024M ‐Xss512K ‐XX:MetaspaceSize=256M ‐XX:MaxMetaspaceSize=256M
```

- 启动Tomcat（Windows）

在Tomcat bin目录下catalina.bat文件里配置。
```bash
‐Xms2048M ‐Xmx2048M ‐Xmn1024M ‐Xss512K ‐XX:MetaspaceSize=256M ‐XX:MaxMetaspaceSize=256M
```
