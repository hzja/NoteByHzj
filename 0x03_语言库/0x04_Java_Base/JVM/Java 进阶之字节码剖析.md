Java 字节码
<a name="xhzaE"></a>
### 前言
字节码 是 Java 能跨平台的根本原因，而且通过了解字节码也可以彻底揭开 JVM 运行程序的秘密，整体会用问答的形式来讲解
<a name="V12WC"></a>
#### 能否简单介绍一下 Java 的特性
Java 是一门**面向对象**，**静态类型的**语言，具有跨平台的特点，与 C,C++ 这些需要手动管理内存，编译型的语言不同，它是解释型的，具有跨平台和自动垃圾回收的特点，那么它的跨平台到底是怎么实现的呢？<br />都知道计算机只能识别二进制代码表示的机器语言，所以不管用的什么高级语言，最终都得翻译成机器语言才能被 CPU 识别并执行，对于 C++这些编译型语言来说是直接一步到位转为相应平台的可执行文件（即机器语言指令），而对 Java 来说，则首先由编译器将源文件编译成字节码，再在运行时由虚拟机（JVM）解释成机器指令来执行，可以看下下图<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346438-d66bdffc-9ac5-4f46-8831-d1866eadd362.webp#clientId=ub775fb79-871f-4&from=paste&id=ub0bba9a6&originHeight=1007&originWidth=962&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3e3d68aa-459a-4367-9279-10326d93f70&title=)<br />也就是说 Java 的跨平台其实是通过先生成字节码，再由针对各个平台实现的 JVM 来解释执行实现的，JVM 屏蔽了 OS 的差异，Java 工程都是以 Jar 包分发（一堆 class 文件的集合体）部署的，这就意味着 jar 包可以在各个平台上运行（由相应平台的 JVM 解释执行即可），这就是 Java 能实现跨平台的原因所在<br />这也是为什么 JVM 能运行 Scala、Groovy、Kotlin 这些语言的原因，并不是 JVM 直接来执行这些语言，而是这些语言最终都会生成符合 JVM 规范的字节码再由 JVM 执行，是否注意到，使用字节码也利用了计算机科学中的分层理念，通过加入字节码这样的中间层，有效屏蔽了与上层的交互差异。
<a name="qywdA"></a>
#### JVM 是怎么执行字节码的
在此之前先来看下 JVM 的整体内存结构，对其有一个宏观的认识，然后再来看 JVM 是如何执行字节码的<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346370-c06ad13e-73d8-4294-9b2e-4a259362d38c.webp#clientId=ub775fb79-871f-4&from=paste&id=ufa2493ae&originHeight=407&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u558e9b25-bae1-4401-9590-84f0c7eb546&title=)<br />JVM 内存结构<br />JVM 在内存中主要分为「栈」，「堆」，「非堆」以及 JVM 自身，堆主要用来分配类实例和数组，非堆包括「方法区」、「JVM内部处理或优化所需的内存(如JIT编译后的代码缓存)」、每个类结构(如运行时常数池、字段和方法数据)以及方法和构造方法的代码<br />主要关注栈，线程是 cpu 调度的最小单位，在 JVM 中一旦创建一个线程，就会为其分配一个线程栈，线程会调用一个个方法，每个方法都会对应一个个的栈帧压到线程栈里，JVM 中的栈内存结构如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346719-8aefbe3f-2847-4ca2-8cfb-1740bd2f6112.webp#clientId=ub775fb79-871f-4&from=paste&id=uff201c3c&originHeight=468&originWidth=707&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6049967f-8ca3-44bc-9ed0-70ac8f80ede&title=)<br />JVM 栈内存结构<br />至此总算接近 JVM 执行的真相了，JVM 是以栈帧为单位执行的，栈帧由以下四个部分组成

- 返回值
- 局部变量表（Local Variables）：存储方法用到的本地变量
- 动态链接：在字节码中，所有的变量和方法都是以符号引用的形式保存在 class 文件的常量池中的，比如一个方法调用另外的方法，是通过常量池中指向方法的符号引用来表示的，动态链接的作用就是为了将这些符号引用转换为调用方法的直接引用，这么说可能有人还是不理解，所以先执行一下 `javap -verbose Demo.class`命令来查看一下字节码中的常量池是咋样的

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346423-267a1732-25ec-4b99-9b0f-9be50b719da8.webp#clientId=ub775fb79-871f-4&from=paste&id=u7b8916d6&originHeight=379&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucb5d0bcc-5e95-47eb-8e82-0d84f3f21e1&title=)<br />注意：以上只列出了常量池中的**部分符号引用**<br />可以看到 Object 的 init 方法是由 #4.#16 表示的，而 #4 又指向了 #19，#19 表示 Object，#16 又指向了 #7.#8，#7 指向了方法名，#8 指向了 ()V（表示方法的返回值为 void，且无方法参数），字节码加载后，会把类信息加载到元空间（Java 8 以后）中的方法区中，动态链接会把这些符号引用替换为调用方法的直接引用，如下图示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346358-172ae168-4381-4668-8bfd-a80294b45b0b.webp#clientId=ub775fb79-871f-4&from=paste&id=ued9b95ce&originHeight=936&originWidth=707&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua4bf2dcf-8044-4c3f-ab01-6d60bf1ce5d&title=)<br />那为什么要提供动态链接呢，通过上面这种方式绕了好几个弯才定位到具体的执行方法，效率不是低了很多吗，其实**主要是为了支持 Java 的多态**，比如声明一个 `Father f = new Son()`这样的变量，但执行 `f.method()` 的时候会绑定到 son 的 method（如果有的话），这就是用到了动态链接的技术，在运行时才能定位到具体该调用哪个方法，动态链接也称为后期绑定，与之相对的是静态链接（也称为前期绑定），即在编译期和运行期对象的方法都保持不变，静态链接发生在编译期，也就是说在程序执行前方法就已经被绑定，**java 当中的方法只有**`**final**`**、**`**static**`**、**`**private**`**和构造方法是前期绑定的**。而动态链接发生在运行时，**几乎所有的方法都是运行时绑定的**<br />举个例子来看看两者的区别，一目了解
```java
class Animal{
    public void eat(){
        System.out.println("动物进食");
    }
}

class Cat extends Animal{
    @Override
    public void eat() {
        super.eat();//表现为早期绑定（静态链接）
        System.out.println("猫进食");
    }
}
public class AnimalTest {
    public void showAnimal(Animal animal){
        animal.eat();//表现为晚期绑定（动态链接）
    }
}
```

- 操作数栈（Operand Stack）：程序主要由指令和操作数组成，指令用来说明这条操作做什么，比如是做加法还是乘法，操作数就是指令要执行的数据，那么指令怎么获取数据呢，指令集的架构模型分为**基于栈的指令集架构**和**基于寄存器的指令集架构**两种，JVM 中的指令集属于前者，也就是说任何操作都是用栈来管理，基于栈指令可以更好地实现跨平台，栈都是是在内存中分配的，而寄存器往往和硬件挂钩，不同的硬件架构是不一样的，不利于跨平台，当然基于栈的指令集架构缺点也很明显，基于栈的实现需要更多指令才能完成（因为栈只是一个FILO结构，需要频繁压栈出栈），而寄存器是在CPU的高速缓存区，相较而言，**基于栈的速度要慢不少**，这也是为了跨平台而做出的一点性能牺牲，毕竟鱼和熊掌不可兼得。
<a name="aX73t"></a>
### Java 字节码技术简介
注意线程中还有一个「PC 程序计数器」，是每个线程独有的，记录着当前线程所执行的字节码的行号指示器，也就是指向下一条指令的地址，也就是将执行的指令代码。由执行引擎读取下一条指令。先来看下看一下字节码长啥样。假设有以下 Java 代码
```java
public class Demo {
      private  int a = 1;
    public static void foo() {
        int a = 1;
        int b = 2;
        int c = (a + b) * 5;
    }
}
```
执行 `javac Demo.java` 后可以看到其字节码如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346706-f42278c9-ccc2-4dbb-9f11-ef3f29b928a5.webp#clientId=ub775fb79-871f-4&from=paste&id=u243d77eb&originHeight=290&originWidth=794&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50d4402f-88ca-46bc-a1ce-2b8ea06d1b0&title=)<br />字节码是给 JVM 看的，所以需要将其翻译成人能看懂的代码，好在 JDK 提供了反解析工具 javap ，可以根据字节码反解析出 code 区（汇编指令）、本地变量表、异常表和代码行偏移量映射表、常量池等信息。执行以下命令来看下根据字节码反解析的文件长啥样（更详细的信息可以执行 javap -verbose 命令，在本例中重点关注 Code 区是如何执行的，所以使用了 javap -c 来执行
```bash
javap -c Demo.class
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346867-38839c72-8147-41c9-912d-8fa36d01493e.webp#clientId=ub775fb79-871f-4&from=paste&id=uf80aa8a7&originHeight=764&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ea50cde-318d-4d5e-8591-09af8a73cbc&title=)<br />转换成这种形式可读性强了很多，那么aload_0，invokespecial 这些表示什么含义呢， javap 是怎么根据字节码来解析出这些指令出来的呢<br />首先需要明白什么是指令，**指令=操作码+操作数**，操作码表示这条指令要做什么，比如加减乘除，操作数即操作码操作的数，比如 1+ 2 这条指令，操作码其实是加法，1，2 为操作数，在 Java 中每个操作码都由一个字节表示，每个操作码都有对应类似 aload_0，invokespecial，iconst_1 这样的助记符，有些操作码本来就包含着操作数，比如字节码 0x04 对应的助记符为 iconst_1， 表示 将 int 型 1 推送至栈顶，这些操作码就相当于指令，而有些操作码需要配合操作数才能形成指令，如字节码 0x10 表示 bipush，后面需要跟着一个操作数，表示 将单字节的常量值(-128~127)推送至栈顶。以下为列出的几个字节码与助记符示例

| 字节码 | 助记符 | 表示含义 |
| --- | --- | --- |
| 0x04 | iconst_1 | 将int型1推送至栈顶 |
| 0xb7 | invokespecial | 调用超类构建方法, 实例初始化方法, 私有方法 |
| 0x1a | iload_0 | 将第一个int型本地变量推送至栈顶 |
| 0x10 | bipush | 将单字节的常量值(-128~127)推送至栈顶 |

至此不难明白 javap  的作用了，它主要就是找到字节码对应的的助记符然后再展示，简单看下上述的默认构造方法是如何根据字节码映射成助记符并最终呈现在开发者面前的：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346887-016b7c20-35eb-4c59-9a16-eb461080e617.webp#clientId=ub775fb79-871f-4&from=paste&id=ue33f69ff&originHeight=1243&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uffd37bcb-4cc4-41eb-9187-2c759e681d0&title=)<br />最左边的数字是 Code 区中每个字节的偏移量，这个是保存在 PC 的程序计数中的，比如如果当前指令指向 1，下一条就指向 4<br />另外大家不难发现，在源码中其实并没有定义默认构造函数，但在字节码中却生成了，而且会发现在源码中定义了`private int a = 1;`但这个变量赋值的操作却是在构造方法中执行的（下文会分析到），这就是理解字节码的意义：**它可以反映 JVM 执行程序的真正逻辑**，而源码只是表象，要深入分析还得看字节码!<br />接下来看看构造方法对应的指令是如何执行的，首先来看一下在 JVM 中指令是怎么执行的。

1. 首先 JVM 会为每个方法分配对应的局部变量表，可以认为它是一个数组，每个槽位（称为 slot）为方法中分配的变量，如果是实例方法，这些局部变量可以是 this，方法参数，方法里分配的局部变量，这些局部变量的类型即熟知的 int，long 等八大基本，还有引用，返回地址，每个 slot 为 4 个字节，所以像 Long，Double 这种 8 个字节的要占用 2 个 slot， 如果这个方法为实例方法，则第一个 slot 为 this 指针，如果是静态方法则没有 this 指针
2. 分配好局部变量表后，方法里如果涉及到赋值，加减乘除等操作，那么这些指令的运算就需要依赖于操作数栈了，将这些指令对应的操作数通过压栈，弹栈来完成指令的执行

比如有 `int i = 69` 这样的指令，对应的字码节指令如下
```java
0:bipush 69
2:istore_0
```
其在内存中的操作过程如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305346944-8516ed4e-84f5-4e10-b427-99a81b777f1f.webp#clientId=ub775fb79-871f-4&from=paste&id=uc9f56201&originHeight=302&originWidth=537&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u19fbad88-679f-437b-9206-b39b48f1545&title=)<br />可以看到主要分两步：第一步首先把 69 这个 int 值压栈，然后再弹栈，把 69 弹出放到局部变量表 i 对应的位置，istore_0 表示弹栈，将其从操作数栈中弹出整型数字存储到本地变量中，0 表示本地变量在局部变量表的第 0 个 slot<br />理解了上面这个操作，再来看一下默认构造函数对应的字节码指令是如何执行的<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305347163-65cc90c4-92ea-4437-bf00-2dda79eaaa83.webp#clientId=ub775fb79-871f-4&from=paste&id=u76362bb8&originHeight=342&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7c64426d-ded5-435b-848a-99c61564b53&title=)<br />首先需要先来理解一下上面几个指令

- aload_0：从局部变量表中加载第 0 个 slot 中的对象引用到操作数栈的栈顶，这里的 0 表示第 0 个位置，也就是 this
- invokespecial：用来调用构造函数，但也可以用于调用同一个类中的 `private` 方法，以及 可见的超类方法，在此例中表示调用父类的构造器（因为 #1 符号引用指向对应的 init 方法）
- iconst_1：将 int 型 1推送至栈顶
- putfield：它接受一个操作数，这个操作数引用的是运行时常量池里的一个字段，在这里这个字段是 a。赋给这个字段的值，以及包含这个字段的对象引用，在执行这条指令的时候，都会从操作数栈顶上 pop 出来。前面的 aload_0 指令已经把包含这个字段的对象（this）压到操作数栈上了，而后面的 iconst_1 又把 1 压到栈里。最后 putfield 指令会将这两个值从栈顶弹出。执行完的结果就是这个对象的 a 这个字段的值更新成了 1。

接下来详细解释以上以上助记符代表的含义

- 第一条命令 aload_0，表示从局部变量表中加载第 0 个 slot 中的对象引用到操作数栈的栈顶，也就是将 this 加载到栈顶，如下

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305347132-07c98317-4594-4a67-be17-f1d8ff7f00aa.webp#clientId=ub775fb79-871f-4&from=paste&id=ue5df4165&originHeight=408&originWidth=678&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc3ca339e-a3c1-4233-91b6-2567f49ee9e&title=)

- 第二步 invokespecial #1，表示弹栈并且执行 #1 对应的方法，#1 代表的含义可以从旁边的解释（# Method java/lang/Object."":()V）看出，即调用父类的初始化方法，这也印证了那句话：**子类初始化时会从初始化父类**
- 之后的命令  aload_0，iconst_1，putfied #2 图解如下

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305347383-67e48c70-7265-4643-be7d-dcd3a7687b05.webp#clientId=ub775fb79-871f-4&from=paste&id=u6c59aa02&originHeight=1113&originWidth=833&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub86ba437-df0a-4a07-aaea-d9f23cd99bd&title=)<br />可能有人有些奇怪，上述 6: putfield #2命令中的 #2 怎么就代表 Demo 的私有成员 a 了，这就涉及到字节码中的常量池概念了，执行 `javap -verbose path/Demo.class` 可以看到这些字面量代表的含义，#1，#2 这种数字形式的表示形式也被称为符号引用，程序运行期会将符号引用转换为直接引用<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305347352-d82e7b1b-bcaf-40d2-8260-e06232dd4692.webp#clientId=ub775fb79-871f-4&from=paste&id=u9d811128&originHeight=730&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf10ec6cb-642e-49e3-8117-9e8906e8b88&title=)<br />由此可知 #2 代表 Demo 类的 a 属性，如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305347390-8385a8ed-a8c6-470d-bc84-7662c6c71a7e.webp#clientId=ub775fb79-871f-4&from=paste&id=u3999b427&originHeight=408&originWidth=822&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u409370bd-3484-455c-be2b-88857c49f88&title=)<br />从最终的叶子节点可以看出 #2 最终代表的是 Demo 类中类型为 int（I 代表 int 代表 int 类型），名称为 a 的变量<br />再来用动图看一下 foo 的执行流程，相信能理解其含义了<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1639305347446-61a44f04-3785-47d2-9d16-d6bfe4c14d0f.gif#clientId=ub775fb79-871f-4&from=paste&id=u70574f5f&originHeight=365&originWidth=908&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua4490f81-bb84-4890-9a59-e808014b073&title=)<br />唯一需要注意的此例中的 foo 是个静态方法，所以局部变量区是没有 this 的。<br />不难发现 JVM 执行字节码的流程与 CPU 执行机器码步骤如出一辙，都经历了「取指令」，「译码」，「执行」，「存储计算结果」这四步，首先程序计数器指向下一条要执行的指令，然后 JVM 获取指令，由本地执行引擎将字节码操作数转成机器码（译码）执行，执行后将值存储到局部变量区（存储计算结果）中<br />最后关于字节码推荐两款工具

- 一个是 Hex Fiend，一款很好的十六进制编辑器，可以用来查看编辑字节码
- 一款是 Intellij Idea 的插件 `jclasslib Bytecode viewer`，能展示 `javap -verbose` 命令对应的常量池，接口，Code 等数据，非常的直观，对于分析字节码非常有帮忙，如下

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639305347903-d073e2f7-0ea0-4870-a84f-d963a1f77b27.webp#clientId=ub775fb79-871f-4&from=paste&id=u8fd9edbb&originHeight=425&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u85f9daec-3000-4376-bd3c-be0e2ad09db&title=)
