JVM
<a name="WPPUn"></a>
## Java对象行为
不重启JVM，替换掉已经加载的类本质上是动态改变内存中已存在对象的行为问题。<br />所以，得先弄清楚JVM中和对象行为有关的地方在哪里，有没有更改的可能性。<br />对象使用两种东西来描述事物：行为和属性。<br />举个例子：
```java
publicclass Person{

  privateint age;
  private String name;

  public void speak(String str) {
    System.out.println(str);
 }

  public Person(int age, String name) {
    this.age = age;
    this.name = name;
  }
}
```
上面Person类中age和name是属性，speak是行为。对象是类的实例，每个对象的属性都属于对象本身，但是每个对象的行为却是公共的。举个例子，比如现在基于Person类创建了两个对象，personA和personB：
```java
Person personA = new Person(43, "lixunhuan");
personA.speak("我是李寻欢");
Person personB = new Person(23, "afei");
personB.speak("我是阿飞");
```
personA和personB有各自的姓名和年龄，但是有共同的行为：speak。想象一下，如果是Java语言的设计者，会怎么存储对象的行为和属性呢？<br />“很简单，属性跟着对象走，每个对象都存一份。行为是公共的东西，抽离出来，单独放到一个地方。”<br />“咦？抽离出公共的部分，跟代码复用好像啊。”<br />“大道至简，很多东西本来都是殊途同归。”<br />也就是说，第一步首先得找到存储对象行为的这个公共的地方。一番搜索之后，发现这样一段描述：
> Method area is created on virtual machine startup, shared among all Java virtual machine threads and it is logically part of heap area. It stores per-class structures such as the run-time constant pool, field and method data, and the code for methods and constructors.

Java的对象行为（方法、函数）是存储在方法区的。<br />“方法区中的数据从哪来？”<br />“方法区中的数据是类加载时从class文件中提取出来的。”<br />“class文件从哪来？”<br />“从Java或者其他符合JVM规范的源代码中编译而来。”<br />“源代码从哪来？”<br />“废话，当然是手写！”<br />“倒着推，手写没问题，编译没问题，至于加载……有没有办法加载一个已经加载过的类呢？如果有的话，就能修改字节码中目标方法所在的区域，然后重新加载这个类，这样方法区中的对象行为（方法）就被改变了，而且不改变对象的属性，也不影响已经存在对象的状态，那么就可以搞定这个问题了。<br />可是，这岂不是违背了JVM的类加载原理？毕竟不想改变`ClassLoader`。”<br />“少年，可以去看看java.lang.instrument.Instrumentation。”
<a name="dmcMg"></a>
## java.lang.instrument.Instrumentation
看完文档之后，可以发现这么两个接口：`redefineClasses`和`retransformClasses`。一个是重新定义class，一个是修改class。这两个大同小异，看`redefineClasses`的说明：
> This method is used to replace the definition of a class without reference to the existing class file bytes, as one might do when recompiling from source for fix-and-continue debugging. Where the existing class file bytes are to be transformed (for example in bytecode instrumentation) retransformClasses should be used.

都是替换已经存在的class文件，redefineClasses是自己提供字节码文件替换掉已存在的class文件，retransformClasses是在已存在的字节码文件上修改后再替换之。<br />当然，运行时直接替换类很不安全。比如新的class文件引用了一个不存在的类，或者把某个类的一个field给删除了等等，这些情况都会引发异常。所以如文档中所言，instrument存在诸多的限制：
> The redefinition may change method bodies, the constant pool and attributes. The redefinition must not add, remove or rename fields or methods, change the signatures of methods, or change inheritance. These restrictions maybe be lifted in future versions. The class file bytes are not checked, verified and installed until after the transformations have been applied, if the resultant bytes are in error this method will throw an exception.

能做的基本上也就是简单修改方法内的一些行为，这对于开头的问题，打印一段日志来说，已经足够了。当然，除了通过`retransform`来打印日志，还能做很多其他非常有用的事情，这个下文会进行介绍。<br />那怎么得到需要的class文件呢？一个最简单的方法，是把修改后的Java文件重新编译一遍得到class文件，然后调用redefineClasses替换。但是对于没有（或者拿不到，或者不方便修改）源码的文件应该怎么办呢？其实对于JVM来说，不管是Java也好，Scala也好，任何一种符合JVM规范的语言的源代码，都可以编译成class文件。<br />JVM的操作对象是class文件，而不是源码。所以，从这种意义上来讲，可以说“JVM跟语言无关”。既然如此，不管有没有源码，其实只需要修改class文件就行了。
<a name="dQUkf"></a>
## 直接操作字节码
Java是软件开发人员能读懂的语言，class字节码是JVM能读懂的语言，class字节码最终会被JVM解释成机器能读懂的语言。无论哪种语言，都是人创造的。<br />所以，理论上（实际上也确实如此）人能读懂上述任何一种语言，既然能读懂，自然能修改。只们愿意，完全可以跳过Java编译器，直接写字节码文件，只不过这并不符合时代的发展罢了，毕竟高级语言设计之始就是为人类所服务，其开发效率也比机器语言高很多。<br />对于人类来说，字节码文件的可读性远远没有Java代码高。尽管如此，还是有一些杰出的程序员们创造出了可以用来直接编辑字节码的框架，提供接口可以让开发者方便地操作字节码文件，进行注入修改类的方法，动态创造一个新的类等等操作。其中最著名的框架应该就是ASM了，cglib、Spring等框架中对于字节码的操作就建立在ASM之上。<br />Spring的AOP是基于动态代理实现的，Spring会在运行时动态创建代理类，代理类中引用被代理类，在被代理的方法执行前后进行一些神秘的操作。<br />那么，Spring是怎么在运行时创建代理类的呢？动态代理的美妙之处，就在于开发者不必手动为每个需要被代理的类写代理类代码，Spring在运行时会根据需要动态地创造出一个类。这里创造的过程并非通过字符串写Java文件，然后编译成class文件，然后加载。Spring会直接“创造”一个class文件，然后加载，创造class文件的工具，就是ASM了。<br />到这里，知道了用ASM框架直接操作class文件，在类中加一段打印日志的代码，然后`retransform`就可以了。
<a name="W8XbJ"></a>
## BTrace
截止到目前，都是停留在理论描述的层面。那么如何进行实现呢？先来看几个问题：

1. 在工程中，谁来做这个寻找字节码，修改字节码，然后retransform的动作呢？不可能知道未来有没有可能遇到开头的这种问题。考虑到性价比，也不可能在每个工程中都开发一段专门做这些修改字节码、重新加载字节码的代码。
2. 如果JVM不在本地，在远程呢？
3. 如果连ASM都不会用呢？能不能更通用一些，更“傻瓜”一些。

幸运的是，因为有BTrace的存在，不必自己写一套这样的工具了。什么是BTrace呢？BTrace已经开源，项目描述极其简短：<br />A safe, dynamic tracing tool for the Java platform.<br />BTrace是基于Java语言的一个安全的、可提供动态追踪服务的工具。BTrace基于ASM、Java Attach API、Instrument开发，为用户提供了很多注解。依靠这些注解，可以编写BTrace脚本（简单的Java代码）达到想要的效果，而不必深陷于ASM对字节码的操作中不可自拔。<br />看BTrace官方提供的一个简单例子：拦截所有java.io包中所有类中以read开头的方法，打印类名、方法名和参数名。当程序IO负载比较高的时候，就可以从输出的信息中看到是哪些类所引起，是不是很方便？
```java
package com.sun.btrace.samples;

import com.sun.btrace.annotations.*;
import com.sun.btrace.AnyType;
importstatic com.sun.btrace.BTraceUtils.*;

/**
 * This sample demonstrates regular expression
 * probe matching and getting input arguments
 * as an array - so that any overload variant
 * can be traced in "one place". This example
 * traces any "readXX" method on any class in
 * java.io package. Probed class, method and arg
 * array is printed in the action.
 */
@BTracepublicclass ArgArray {
    @OnMethod(
        clazz="/java\\.io\\..*/",
        method="/read.*/"
    )
    public static void anyRead(@ProbeClassName String pcn, @ProbeMethodName String pmn, AnyType[] args) {
        println(pcn);
        println(pmn);
        printArray(args);
    }
}
```
再来看另一个例子：每隔2秒打印截止到当前创建过的线程数。
```java
package com.sun.btrace.samples;

import com.sun.btrace.annotations.*;
importstatic com.sun.btrace.BTraceUtils.*;
import com.sun.btrace.annotations.Export;

/**
 * This sample creates a jvmstat counter and
 * increments it everytime Thread.start() is
 * called. This thread count may be accessed
 * from outside the process. The @Export annotated
 * fields are mapped to jvmstat counters. The counter
 * name is "btrace." + <className> + "." + <fieldName>
 */
@BTracepublicclass ThreadCounter {

    // create a jvmstat counter using @Export
    @Exportprivatestaticlong count;

    @OnMethod(
        clazz="java.lang.Thread",
        method="start"
    )
    public static void onnewThread(@Self Thread t) {
        // updating counter is easy. Just assign to
        // the static field!
        count++;
    }

    @OnTimer(2000)
    public static void ontimer() {
        // we can access counter as "count" as well
        // as from jvmstat counter directly.
        println(count);
        // or equivalently ...
        println(Counters.perfLong("btrace.com.sun.btrace.samples.ThreadCounter.count"));
    }
}
```
看了上面的用法是不是有所启发？忍不住冒出来许多想法。比如查看HashMap什么时候会触发rehash，以及此时容器中有多少元素等等。<br />有了BTrace，文章开头的问题可以得到完美的解决。至于BTrace具体有哪些功能，脚本怎么写，这些Git上BTrace工程中有大量的说明和举例，网上介绍BTrace用法的文章更是恒河沙数，这里就不再赘述了。<br />明白了原理，又有好用的工具支持，剩下的就是发挥自己的创造力了，只需在合适的场景下合理地进行使用即可。<br />既然BTrace能解决上面提到的所有问题，那么BTrace的架构是怎样的呢？<br />BTrace主要有下面几个模块：

1. BTrace脚本：利用BTrace定义的注解，可以很方便地根据需要进行脚本的开发。
2. Compiler：将BTrace脚本编译成BTrace class文件。
3. Client：将class文件发送到Agent。
4. Agent：基于Java的Attach API，Agent可以动态附着到一个运行的JVM上，然后开启一个BTrace Server，接收client发过来的BTrace脚本；解析脚本，然后根据脚本中的规则找到要修改的类；修改字节码后，调用Java Instrument的retransform接口，完成对对象行为的修改并使之生效。

整个BTrace的架构大致如下：<br />![2021-05-14-08-21-46-328920.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620951919787-c731f611-a9ce-4bde-be36-93dca6ad77c2.png#clientId=ufa9e5f78-ddcb-4&from=ui&id=u4c05a390&originHeight=694&originWidth=900&originalType=binary&size=1877599&status=done&style=shadow&taskId=u26345588-6272-4efc-8ead-e312aefdc75)<br />btrace工作流程<br />BTrace最终借Instrument实现class的替换。如上文所说，出于安全考虑，Instrument在使用上存在诸多的限制，BTrace也不例外。BTrace对JVM来说是“只读的”，因此BTrace脚本的限制如下：

1. 不允许创建对象
2. 不允许创建数组
3. 不允许抛异常
4. 不允许catch异常
5. 不允许随意调用其他对象或者类的方法，只允许调用com.sun.btrace.BTraceUtils中提供的静态方法（一些数据处理和信息输出工具）
6. 不允许改变类的属性
7. 不允许有成员变量和方法，只允许存在`static public void`方法
8. 不允许有内部类、嵌套类
9. 不允许有同步方法和同步块
10. 不允许有循环
11. 不允许随意继承其他类（当然，java.lang.Object除外）
12. 不允许实现接口
13. 不允许使用assert
14. 不允许使用Class对象

如此多的限制，其实可以理解。BTrace要做的是，虽然修改了字节码，但是除了输出需要的信息外，对整个程序的正常运行并没有影响。
<a name="ts25C"></a>
## Arthas
BTrace脚本在使用上有一定的学习成本，如果能把一些常用的功能封装起来，对外直接提供简单的命令即可操作的话，那就再好不过了。阿里的工程师们早已想到这一点，就在去年，阿里巴巴开源了自己的Java诊断工具——Arthas<br />Arthas提供简单的命令行操作，功能强大。究其背后的技术原理，和文中提到的大致无二。<br />旨在说明Java动态追踪技术的来龙去脉，掌握技术背后的原理之后，只要愿意，也可以开发出自己的“冰封王座”出来。
<a name="pVUxI"></a>
## 三生万物
现在试着站在更高的地方“俯瞰”这些问题。<br />Java的Instrument给运行时的动态追踪留下了希望，Attach API则给运行时动态追踪提供了“出入口”，ASM则大大方便了“人类”操作Java字节码的操作。<br />基于Instrument和Attach API前辈们创造出了诸如JProfiler、Jvisualvm、BTrace这样的工具。以ASM为基础发展出了cglib、动态代理，继而是应用广泛的Spring AOP。<br />Java是静态语言，运行时不允许改变数据结构。然而，Java 5引入Instrument，Java 6引入Attach API之后，事情开始变得不一样了。虽然存在诸多限制，然而，在前辈们的努力下，仅仅是利用预留的近似于“只读”的这一点点狭小的空间，仍然创造出了各种大放异彩的技术，极大地提高了软件开发人员定位问题的效率。
