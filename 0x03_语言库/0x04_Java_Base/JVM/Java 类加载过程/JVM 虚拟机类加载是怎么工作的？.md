<a name="XUj9E"></a>
## 引言
在程序世界的大海洋中，**「类」**就像是构建一切的基石。它们是构建 Java 应用的原材料，**「类加载器」**则是这个世界的建筑工人。他们负责将构建城市所需的材料搬运到工地（**「JVM」**）。了解类加载器的工作原理，就像了解城市建设的过程，能够更好地理解和控制程序的运行。现在，深入探索 JVM 的类加载器，解析它的奥秘，开启这趟神奇的旅程吧！
<a name="twmWh"></a>
## 城市建设过程 | 类加载器工作原理
首先，编写了一个类:
```java
public class Building {
    public Building() {
        System.out.println("建筑蓝图已被创建! 我们可以添砖Java了");
    }
    private static int constructionYear = 2023;
    private int floorCount;

    public Building(int floors) {
        this.floorCount = floors;
    }

    public void setFloorCount(int floors) {
        this.floorCount = floors;
    }

    public int getFloorCount() {
        return this.floorCount;
    }

    public static int getConstructionYear() {
        return constructionYear;
    }

    public static void main(String[] args) {
        new Building();
    }
}
```
类中有一个`main()`方法的程序入口点。运行一下：
```bash
Connected to the target VM, address: '127.0.0.1:9888', transport: 'socket'
建筑蓝图已被创建! 我们可以添砖Java了
Disconnected from the target VM, address: '127.0.0.1:9888', transport: 'socket'

Process finished with exit code 0
```
好，没什么问题。是否好奇当在IDE绿色的小箭头点击run ‘Building.main()’之后，底层到底发什么什么？嗯...有问必答，接着看。
<a name="cf386"></a>
## 类加载器加载类前过程
还是结合上面的例子，仔细思考它们的对应关系。开始吧~
<a name="xXnGA"></a>
### 建筑工程立项 | JVM进程启动
当在IDE绿色的小箭头点击run ‘Building.main()’ 其实IDE会进行两个步骤：**「编译」**：IDE会先使用javac编译器，将.java源文件编译成.class字节码文件。这一步骤通常是在后台进行的，通常不会看到任何有关编译的消息，除非出现编译错误。（往往这个时候程序员就要挠掉不少头发） **「运行」**：编译完成后，IDE会使用java命令启动JVM进程，然后载入并执行相应的.class文件中的main方法。<br />搞懂这两个步骤，接着往下说。<br />首先，在运行java Building这个指令时，就好比发出了开工命令。JVM就像一位总承包商，控制着一位特殊的工人，也就是**「Bootstrap类加载器」**。这位工人的工作是从核心材料库（$JAVA_HOME/jre/lib）中取出构建这座大楼所需的**「基本原材料」**，这些基本材料包括了Java的核心类库。
<a name="S2Pnl"></a>
### Bootstrap类加载器
高级工程师的两位得力助手 | Bootstrap类加载器创建扩展和应用类加载器。<br />Bootstrap类加载器，像一位高级工程师，接下来派遣了另外两位工人，他们是**「扩展（ext）类加载器」**和**「应用（app）类加载器」**。扩展类加载器的任务是从扩展材料库$JAVA_HOME/jre/lib/ext获取扩展材料。应用类加载器的任务是从建筑工地周围（系统类路径CLASSPATH）收集所需的特定材料。<br />至此，类加载器加载类前过程已经完成了，接着往后看。
<a name="Ml9G5"></a>
## 类加载器加载类后过程
<a name="Dt3Yw"></a>
### 应用类加载器加载Building
当你（雇主）告诉高级工程师（Bootstrap类加载器）你需要一个名为Building 的设计蓝图，这个时候高级工程师就可以派出它的得力助手**「扩展类加载器」**了，但是**「扩展类加载器」**发现Building不是它的职责范围，于是把活交给**「应用类加载器」**，他刚好知道在哪里可以找到Building.class这个特定的建筑蓝图。他会沿着系统类路径，寻找到这个类文件，并将其内容（类的字节码）搬运到JVM中。这个过程就好像是将建筑蓝图放到了JVM的工地上。
<a name="mwUvY"></a>
### 链接过程（验证，准备，解析）
当Building类的字节码被搬运到JVM后，总承包商会委托工人们对这些原材料进行处理。他们会检查材料（验证），然后对`constructionYear `材料进行预处理先把它设置为0（准备），你看：
```java
private static int constructionYear = 0;
```
最后将它们组合在一起（**「解析」**），把JVM将常量池中的符号引用替换为直接引用。这个过程就好比按照蓝图的要求，将砖块、水泥等材料准备好并组装起来。
<a name="WfhEm"></a>
### 初始化过程
紧接着就开始真正的施工了。工人们按照`Building`类的main方法（也就是建筑的蓝图）开始构建大楼。在这个例子中，它会创建一个新的Building对象。并且静态变量`constructionYear`在初始化阶段会被初始化为2023,你看：
```java
private static int constructionYear = 2023;
```
这就好比工人们按照蓝图上的指示，开始把砖块、水泥等材料搭建起来。
<a name="ZVASI"></a>
### 使用过程
一旦建筑物（也就是Building对象）被创建出来，就可以开始使用了。在这个例子中，当Building对象被创建时，它的构造函数会被调用，打印出”建筑蓝图已被创建! 我们可以添砖Java了“。
<a name="o0R2K"></a>
### 卸载过程
当大楼（也就是Building对象）不再被使用，或者建筑工地（也就是JVM）需要关闭时，这座大楼就会被拆除。这个过程由JVM的**「垃圾回收器」**负责，它会清理掉不再需要的Building对象。当没有任何类加载器引用这个Building类时，这个类也将被卸载。这就好比当大楼不再需要，或者工地需要关闭时，大楼会被拆除，蓝图（也就是Building.class文件）也会被收回。
<a name="rBFFC"></a>
## 解决建筑过程中出现的问题
不知道你有没有发现，在描述初始化的过程中并没有提到floorCount变量，那这个材料就不初始化了吗？还有，为什么一开始高级工程师不直接把活派给**「应用类加载器」**而是先给**「扩展类加载器」**？还有，为什么写了`main()`方法，程序就可以运行了？一个一个来解答
<a name="GbTuY"></a>
### floorCount为什么没被初始化？
它们会在创建对象的时候（也就是新建Building对象时）被初始化。**「实例变量」**floorCount是属于对象的，每个对象都有一份独立的副本，它们的生命周期随着对象的创建和销毁而开始和结束。
<a name="ra9sC"></a>
### 高级工程师为什么不直接把活派给应用类加载器而是先给扩展类加载器？
因为高级工程师很聪明，他知道有一种双亲委派机制可以提高效率，怎么提高效率？
<a name="qsws7"></a>
### 高级工程师的决定他人不能改变 | 保证Java核心API不被篡改
例如：自己写的java.lang.String.class类不会被加载，这样便可以防止核心 API库被随意篡改
<a name="ypTrb"></a>
### 避免出现重复的工作量 | 防止类的重复加载
当父类加载器已经加载了某个类时，子加载器就不会再加载，避免了重复加载。<br />当然还有不少优点：防止Java类库的冲突，节省内存空间... 这里就不赘述了。
<a name="xIJ0U"></a>
### 为什么写了main()方法，程序就可以运行了？
这是由类加载器内部运行机制决定的，可以看下流程：在初始化完成后，JVM会查找类中的 main 方法。main 方法的标准声明应为：`public static void main(String[] args)`。这个方法是静态的（即与类关联，而不是与对象关联），因此JVM可以在不创建类的实例的情况下调用它。一旦找到 main 方法，JVM就会执行它。程序的执行流程就从 main 方法开始。
<a name="eGiA4"></a>
## 有哪些建筑工人 | 类加载器
上面已经提到，除了引导类加载器（BootStrap）之外，还有扩展类加载器(Ext) 和应用类加载器（App），在这里着重再介绍下吧~
<a name="Q8ou0"></a>
### 引导类加载器（Bootstrap ClassLoader）
引导类加载器是最顶级的类加载器，它主要负责加载Java的核心类库，例如java.lang.*，java.util.*等。这些类库的位置通常在JDK的jre/lib/rt.jar中。引导类加载器是由C++编写的，在Java中是无法获取它的引用的。**「引导类加载器是其他类加载器的父加载器」**。
<a name="jSMxY"></a>
### 扩展类加载器（Extension ClassLoader）
扩展类加载器是引导类加载器的子类，它负责加载JDK的扩展类库，这些类库通常位于JDK的jre/lib/ext/目录下或者由系统变量java.ext.dirs指定的目录下。扩展类加载器是由Java编写的，其具体实现类是sun.misc.Launcher$ExtClassLoader。
<a name="qaLoM"></a>
### 应用类加载器（Application ClassLoader）
应用类加载器是扩展类加载器的子类，也是通常接触到的默认的类加载器。它负责加载用户路径（ClassPath）上所指定的类库。这个路径可以通过环境变量CLASSPATH设置，也可以通过java命令的-classpath或者-cp参数设置。应用类加载器的实现类也是sun.misc.Launcher$AppClassLoader。<br />每当子类加载器需要加载类时，首先会委托父类加载器进行加载，直到最顶层的引导类加载器。如果父类加载器无法加载该类，才会由子类加载器自己进行加载。<br />为了你加深对它们的印象，画了一张关于这三个的类加载器树图，可以看一下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1702559813414-c683afa7-38ab-4b97-a8af-77e9f5b408a9.jpeg)
