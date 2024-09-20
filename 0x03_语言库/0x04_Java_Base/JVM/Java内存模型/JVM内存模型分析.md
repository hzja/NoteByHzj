JVM 内存模型<br />JVM（Java虚拟机）主要包括五块区域，分别是程序计数器、虚拟机栈、本地方法栈、堆、方法区。在JVM当中堆和方法区各有一个，一条线程有一个栈和一个程序计数器。五块区域中当中变化最频繁是栈，最先有数据的是方法区，垃圾回收器主要针对的是堆。
<a name="q9M6k"></a>
### 1、程序计数器（PC寄存器）
由于在JVM中多线程是通过线程轮流切换来换取CPU执行时间的，在任何一个确定的时刻，一个CPU只会执行一条线程的指令。因此，为了线程切换后能恢复到正确的执行位置，每条线程都需要一个独立的程序计数器。<br />程序计数器占用内存空间非常小，是线程私有的，每个线程运行的时候都会有一个独立的计数器，是Java虚拟机规范中唯一一个没有规定任何`OutOfMemoryError`情况的区域。如果程序执行的时候线程执行的是一个Java方法，计数器是有值的，其记录的是程序正在执行的字节码指令的地址；如果执行的是`Natvie`方法，计数器的值则为空（Undefined）。
<a name="hlHMP"></a>
### 2、虚拟机栈
Java虚拟机栈是线程私有的，生命周期和线程一致，存储的是一个个栈帧，每个栈帧对应着一个被调用的方法。方法调用的时候，该方法所需的内存空间在栈内存中分配，称为压栈。方法执行结束之后，该方法所属的内存空间释放，称为弹栈（栈内存遵循先进后出，后进先出的原则）。虚拟机栈中主要存储的是局部变量、引用、操作数栈、动态链接、方法的出口等信息。<br />Java虚拟机规范中，对这个区域规定了两个异常情况：

- `StackOverflowError`：线程请求的栈深度大于虚拟机所允许的深度。
- `OutOfMemoryError`：如果虚拟机栈可以动态扩展，而扩展时无法申请到足够的内存。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650897103469-98589ff4-778a-4332-b450-573f5c2b2bb5.png#clientId=u6b3508a1-6b91-4&from=paste&id=uab50e9d0&originHeight=436&originWidth=527&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1b25418e-c35c-4855-86c9-3ea732449bb&title=)
<a name="hSXHd"></a>
### 3、本地方法栈
本地方法栈与Java虚拟机栈的的作用和原理非常相似，其区别只不过是虚拟机栈为虚拟机执行Java方法（也就是字节码）服务，而本地方法栈则是为虚拟机使用到的`Native`方法服务。<br />本地方法栈区域也会抛出`StackOverflowError`和`OutOfMemoryError`异常。
<a name="cVVXv"></a>
### 4、堆
对于大多数应用来说，这块区域是JVM所管理的内存中最大的一块。堆是被所有线程共享的一块区域，在虚拟机创建时启动。堆当中主要存储的是对象实例和数组，在程序执行过程中使用new运算符创建的Java对象，存储在堆内存当中。对象内部有实例变量，所以实例变量存储在堆内存当中。是垃圾回收机制主要管理的区域。<br />Java虚拟机规范中，对这个区域规定了一个异常情况：

- `OutOfMemoryError`：如果堆中没有内存完成实例分配，并且堆也无法再扩展时，抛出该异常。
<a name="nMeMY"></a>
### 5、方法区
方法区与堆一样是各个线程共享的区域，主要存储了类信息（类的名称、方法信息、字段信息）、静态变量、常量以及编译后的代码片段（在类被加载的时候代码片段会载入）等。<br />在Class文件中除了类的字段、方法、接口等描述信息外，还有一项信息是常量池，用来存储编译期间生成的字面量和符号引用。<br />在方法区中有一个非常重要的部分就是运行时常量池，它是每一个类或接口的常量池的运行时表示形式，在类和接口被加载到JVM后，对应的运行时常量池就被创建出来。当然并非Class文件常量池中的内容才能进入运行时常量池，在运行期间也可将新的常量放入运行时常量池中，比如String的`intern`方法。内存有限，无法申请时抛出 `OutOfMemoryError`异常。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650897103460-83c9da53-7b2a-4120-ae3d-5ce8c3d980b8.png#clientId=u6b3508a1-6b91-4&from=paste&id=u80851f0c&originHeight=595&originWidth=859&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u74a78012-0df7-419c-8425-42b75ed78c7&title=)
<a name="hzJ5Z"></a>
### 6、垃圾回收器【自动垃圾回收机制、GC机制】什么时候会考虑将某个Java对象的内存回收呢？

- 当堆中的Java对象成为垃圾数据的时候，会被垃圾回收器回收。
- 什么时候堆内存中Java对象会变成垃圾呢？

 没有更多的引用指向它的时候。<br />这个对象无法被访问，因为访问对象只能通过引用的方式访问。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650897103380-573cc32f-c788-44cd-862d-8830f55d598a.png#clientId=u6b3508a1-6b91-4&from=paste&id=u039bf29f&originHeight=642&originWidth=685&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u93a7d25b-0655-43a0-a681-63ab3f7331a&title=)
