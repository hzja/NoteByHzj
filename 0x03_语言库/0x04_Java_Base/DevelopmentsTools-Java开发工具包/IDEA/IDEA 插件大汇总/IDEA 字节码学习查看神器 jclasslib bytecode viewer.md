介绍一个 IDEA 的字节码学习神器 jclasslib bytecode viewer 插件。
<a name="KwFPM"></a>
## 下载安装
首先在 IDEA 的插件市场中搜索  jclasslib bytecode viewer 和进行下载安装，安装过后根据 IDEA 版本的不同，可能需要重启才能使用。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710066-05d60098-af0b-467c-bb25-af4429fbdf0f.png#averageHue=%2324272c&clientId=u4b2aeb04-5a24-4&from=paste&id=u52a820f3&originHeight=973&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u140344d8-6884-4334-af3b-1dfbf29a270&title=)
<a name="Okeqa"></a>
## 使用
下载安装过后，在项目中，首先先将整个项目进行编译，编译的作用是将 .java 文件编译成 .class 文件，对应菜单栏为 build => build project。然后找到需要查询字节码的文件，在菜单栏中点击 view => show bytecode with Jclasslib 即可打开字节码窗口。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710122-a09d22fc-479f-41e1-901b-0f8080937043.png#averageHue=%23212328&clientId=u4b2aeb04-5a24-4&from=paste&id=u86e5fa07&originHeight=883&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u27bb810b-c80d-410d-81ed-697efe68ba0&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710023-612c90f6-3da8-4c3b-9c6e-a33f9466ab34.png#averageHue=%2335383f&clientId=u4b2aeb04-5a24-4&from=paste&id=u8e708c7b&originHeight=787&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc56c5687-bb69-470f-b7e6-e69cd967c59&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710034-2056bfe4-6e2f-4f16-b33a-997d654dbb47.png#averageHue=%231e2022&clientId=u4b2aeb04-5a24-4&from=paste&id=u45c1e0fc&originHeight=568&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9ab2e05b-6991-4aab-80ab-7ed50c1c753&title=)
<a name="FAHAx"></a>
## jclasslib
通过上面打开的字节码窗口，可以看到很多信息，下面来依次看一下，
<a name="mxfsw"></a>
### 通用信息 General Information
在通用信息里面主要有一些类的通用信息，如下所示

1. 版本号（Version Number）: 这部分由次版本号和主版本号组成，用于确定编译进字节码的 Java 版本；
2. 常量池大小
3. 类的访问权限：此部分指定了与类或接口关联的访问权限（例如 public，private）和其他属性（例如，是类或接口，是否定义为 abstract，如果是类，是否被标记为 final）；
4. 此类与超类（This Class & Super Class）: 分别表示这个类本身和它的父类；
5. 字段表（Fields Table）: 描述类或接口声明的变量部分。
6. 方法表（Methods Table）: 包含该类或接口的所有方法的描述。
7. 属性表（Attributes Table）: 描述有关字段，方法和类的更多信息，例如代码实际的字节码，常量字段的值等。
<a name="QaYEV"></a>
## 常量池
在通用信息下面的是常量池，常量池是 Java Class 文件、运行时常量池在 JVM 规范中非常关键的一部分。用更简单地话来说，常量池就是一个存储常量的地方，包含了一个 Java 类或接口所需要的字面量和符号引用。字面量包括各种基本类型的常量和字符串常量，符号引用包括类、字段和方法的引用。<br />这些常量包括：

- 类和接口的全限定名
- 字段的名称和描述符
- 方法的名称和描述符
- 常量值，如文字字符串

所有的字面量和符号引用在 Class 文件中都是以常量池的形式存储的。常量池就像是 Class 文件的资源仓库，比如说 Java 类中定义的很多字符串、数字和引用类型都是作为常量项存储到常量池中。其实，常量池的主要目标是为JVM在运行期间提供所需的信息。字节码文件被加载到 JVM 时，会将这些符号引用解析为直接引用（地址）。<br />举个例子，当编译器遇到一个方法调用时，如 `obj.method()`，JVM 规范要求在常量池中创建一个 Methodref 常量。这个 Methodref会包含对当前类的符号引用和这个方法的名字及描述符。当 JVM 在运行时解析这个 Methodref 常量时，如果 obj 是一个符号引用，那么JVM 会第一次通过符号引用使用其 Fully Qualified Class Name 查找类。<br />在 JVM 中，每一个加载的类型（类或接口）都有一个常量池，当这个类型被加载到 JVM 级别后，这个常量池将成为运行时常量池，在执行类中的代码时会使用到这个常量池。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710034-e9e57cc9-d812-4c69-bf96-b87c71482e5d.png#averageHue=%23202224&clientId=u4b2aeb04-5a24-4&from=paste&id=u03634a5f&originHeight=500&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u80f60a92-2c03-4263-830e-5250d823085&title=)
<a name="rc6SR"></a>
## 字段 Fields
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710580-1d2e58f4-dcc2-48fd-89e5-b602058401bd.png#averageHue=%231e2023&clientId=u4b2aeb04-5a24-4&from=paste&id=ud11b83f4&originHeight=555&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub8938b2f-262f-427c-bdea-bf4bd83cc02&title=)<br />在字段表下面会有所有的类属性字段，在右侧会有引用地址和访问权限。
<a name="W9nRU"></a>
## 方法表 Methods
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975710917-23828c4d-14f0-4634-8303-7a5000915292.png#averageHue=%23212225&clientId=u4b2aeb04-5a24-4&from=paste&id=uc30a6539&originHeight=545&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub793d73c-ab9e-49eb-8fb9-27cec3ce85b&title=)<br />在方法表这里会列出所有的方法，包括类的无参构造方法以及字段的初始化方法，这里可以着重看一下 main 方法的字节码，在源代码中的 main 函数里面有一个循环，在右侧字节码中可以看到 28 行有一个 goto 2 可以猜测到就是进入下一次循环。<br />再依次对应源代码和指令可以大概的猜测到具体的指令的含义，比如 istore 存储，iload 加载，bipush 压栈，iadd 相加，iinc 累加 等。对于每个指令可以点击查看对应的手册，通过手册可以知道对应指令的含义。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975711041-2dd9c95a-7816-4738-b632-648474ba8d38.png#averageHue=%23202225&clientId=u4b2aeb04-5a24-4&from=paste&id=u7af71115&originHeight=631&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4a2da836-3f6d-4b01-8f97-fd4ffa1d0aa&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975711136-6c70a0be-5a66-49b7-8d2b-9b81b39733c1.png#averageHue=%23f5f5f5&clientId=u4b2aeb04-5a24-4&from=paste&id=u16764e3e&originHeight=536&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufcde5ec1-be95-4e69-83c8-51784065aeb&title=)<br />通过字节码可以看到在 for 循环底层有一个 if 比较的指令，在每个循环中进行条件判断。
<a name="nh124"></a>
## 属性 Attributes
这里的属性是一些描述有关字段，方法和类的更多信息，例如代码实际的字节码，常量字段的值等，不是类的字段属性。这里因为测试用例比较简单，所以只是有源代码一个属性。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691975711226-818ad310-02d4-4b69-ba80-05f84259e223.png#averageHue=%231f2023&clientId=u4b2aeb04-5a24-4&from=paste&id=u47e80ad2&originHeight=505&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u88ba7f03-45e8-4acc-985e-f21bac89860&title=)
