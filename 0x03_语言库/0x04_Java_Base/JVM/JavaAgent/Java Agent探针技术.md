Java
<a name="L1non"></a>
## 前言
所谓Java Agent，其功能都是基于**「java.lang.instrument」**中的类去完成。Instrument提供了允许Java编程语言代理检测JVM上运行的程序的功能，**「而检测的机制就是修改字节码」**。Instrument位于**「rt.jar」**中，**「java.lang.instrument」**包下，使用Instrument可以用来**「检测」**或**「协助」**运行在 JVM中的程序；甚至对已加载class进行替换修改，这也就是常说的**「热部署、热加载」**。**「一句话总结Instrument：检测类的加载行为对其进行干扰（修改替换）」**<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694843034266-0e43dd26-1789-42b2-adf3-402f61867b22.png#averageHue=%23fdfde1&clientId=u8cd7bc76-bd03-4&from=paste&height=237&id=u857e5d91&originHeight=405&originWidth=820&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=37645&status=done&style=none&taskId=u6eef8895-7f1d-4ba5-9cf1-40ae55b0706&title=&width=480)<br />`Instrument`的实现基于JVMTI(Java Virtual Machine Tool Interface)的，所谓JVMTI就是一套由 Java 虚拟机提供的，为JVM 相关的工具提供的本地编程接口集合。JVMTI**「基于事件驱动」**，简单点讲就是在JVM运行层面添加一些**「钩子」**可以供开发者去自定义实现相关功能。<br />**「有哪些开源软件使用了该技术?」**<br />[https://github.com/alibaba/arthashttps://github.com/apache/skywalking](https://github.com/alibaba/arthashttps://github.com/apache/skywalking)<br />等等。。。
<a name="kcIsU"></a>
## 相关API初探
<a name="e3jjC"></a>
### `Instrumentation`
java.lang.instrument包下关键的类为：java.lang.instrument.Instrumentation。该接口提供一系列替换转化class定义的方法。接下来看一下该接口的主要方法进行以下说明：
<a name="SVA1C"></a>
### `addTransformer`
用于注册`transformer`。除了任何已注册的转换器所依赖的类的定义外，所有未来的类定义都将可以被transformer看到。当类被加载或被重新定义（redefine，可以是下方的`redefineClasses`触发）时，`transformer`将被调用。如果`canRetransform`为true，则表示当它们被`retransform`时（通过下方的`retransformClasses`），该transformer也会被调用。`addTransformer`共有如下两种重载方法：
```java
void addTransformer(ClassFileTransformer transformer,boolean canRetransform)

void addTransformer(ClassFileTransformer transformer)
```
<a name="Jkpf4"></a>
### `redefineClasses`
```java
void redefineClasses(ClassDefinition... definitions)
              throws ClassNotFoundException,
                     UnmodifiableClassException
```
此方法用于替换**「不引用现有」**类文件字节的类定义，就像从源代码重新编译以进行修复并继续调试时所做的那样。该方法对一系列`ClassDefinition`进行操作，以便允许同时对多个类进行相互依赖的更改(类a的重新定义可能需要类B的重新定义)。**「假如在redifine时，目标类正在执行中，那么执行中的行为还是按照原来字节码的定义执行，当对该类行为发起新的调用时，将会使用redefine之后的新行为。」**<br />**「注意：此redefine不会触发类的初始化行为」**<br />当然redefine时，并不是随心所欲，可以重新定义方法体、常量池、属性、**「但是不可以添加、移除、重命名方法和方法和入参，不能更改方法签名或更改继承」**。当然，在未来的版本中，这些限制可能不复存在。<br />在转换之前，不会检查、验证和安装类文件字节，如果结果字节出现错误，此方法将抛出异常。而抛出异常将不会有类被重新定义
<a name="vsGmT"></a>
### retransformClasses
针对JVM**「已经加载的类」**进行转换，当类初始加载或重新定义类（redefineClass）时，可以被注册的`ClassFileTransformer`进行转化；但是针对那些已经加载完毕之后的类不会触发这个`transform`行为进而导致这些类无法被agent进行监听，所以可以通过`retransformClasses`触发一个事件，而这个事件可以被`ClassFileTransformer`捕获进而对这些类进行`transform`。<br />此方法将针对每一个通过`addTransformer`注册的且`canRetransform`是true的，进行调用其`transform`方法，转换后的类文件字节被安装成为类的新定义，从而拥有新的行为。
<a name="wAzPm"></a>
### `redefineClasses`和`retransformClasses`区别
通过上面的定义可以看得出，貌似`redefineClasses`是在为JVM启动前未加载完成的class服务，而`retransformClasses`是针对JVM启动之后，那些已经完成加载初始化的class服务。
<a name="Omn5k"></a>
### `ClassFileTransformer`
在agent中，需要提供该接口的实现，以便在JVM定义类之前转换class字节码文件，该接口中就提供了一个方法,此方法的实现可以转换提供的类文件并返回一个新的替换类文件：
```java
byte[] transform(ClassLoader loader,
                 String className,
                 Class<?> classBeingRedefined,
                 ProtectionDomain protectionDomain,
                 byte[] classfileBuffer)
          throws IllegalClassFormatException
```
<a name="ybLIP"></a>
## Java Agent的两种实现
java agent其实就是一个jar文件，通过在该jar文件中的manifest中通过相关属性指定要加载的agent实现类。对于agent的实现有两种方式：一种实现是通过命令行方式在JVM启动之前进行代理设置；另一种则是在JVM启动之后通过attach机制去设置。
<a name="PFPec"></a>
### JVM启动前的agent实现
Instrument是JDK5开始引入，在JDK5中Instrument要求在目标JVM程序运行之前通过命令行参数**「javaagent」**来设置代理类，在JVM初始化之前，Instrument启动在JVM中设置回调函数，检测特点类加载情况完成实际增强工作。
```bash
-javaagent: jarpath[ =options]
```
这里jarpath就是agent jar的路径，agent jar必须符合jar文件规范。代理JAR文件的manifest（META-INF/MANIFEST.MF）必须包含属性Premain-Class。此属性的值是代理类的类名。代理类必须实现一个公共静态premain方法，该方法原则上与主应用程序入口点类似。在JVM初始化之后，将按照指定代理的顺序调用每个主方法（premain），然后将调用实际应用程序的主方法(main)。每个premain方法必须按照启动顺序返回。<br />premain方法可以有如下两种重载方法，**「如果两者同时存在，则优先调用多参数的方法」**：
```java
public static void premain(String agentArgs, Instrumentation inst);

public static void premain(String agentArgs);
```
代理类将被`SystemClassLoader`进行加载，premain方法将在和主应用程序main方法同等的安全和类加载器规则下执行，主应用程序main方法可以干的，premain都可以去干。**「如果agent无法被解析，这包括agent class无法被加载、或agent class没有premain方法、agent class的方法出现异常等都会导致JVM启动终止！」**
<a name="qbm3b"></a>
### JVM启动后的agent实现
JDK6开始为`Instrument`增加很多强大的功能，其中要指出的就是在JDK5中如果想要完成增强处理，必须是在目标JVM程序启动前通过命令行指定Instrument，然后在实际应用中，目标程序可能是已经运行中，针对这种场景下如果要保证 JVM不重启得以完成工作，这不是我们想要的，于是JDK6中Instrument提供了在JVM启动之后指定设置java agent达到Instrument的目的。<br />该实现需要确保以下3点：

1. agent jar中manifest必须包含属性Agent-Class，其值为agent类名。
2. agent类中必须包含公有静态方法**「agentmain」**
3. system classload必须支持可以将agent jar添加到system class path。

agent jar将被添加到system class path，这个路径就是SystemClassLoader加载主应用程序的地方，agent class被加载后，JVM将会尝试执行它的agentmain方法，同样的，如果以下两个方法都存在，则优先执行多参数方法：
```java
public static void agentmain(String agentArgs, Instrumentation inst);

public static void agentmain(String agentArgs);
```
看到这里，结合JVM前启动前agent的实现和JVM启动后agent的实现，可能想问是否可以在一个agent class中同时包含premain、agentmain呢，答案是可以的，只不过在JVM启动前不会执行agentmain，同样的，JVM启动后不会执行premain。<br />**「如果agent无法启动（agent class无法被加载、agentmain出异常、agent class没有合法的agentmain方法等），JVM将不会终止！」**
<a name="a9dB7"></a>
### Manifest
<a name="lgCME"></a>
### 属性构成
通过上述可以知道，有一个关键文件**「META-INF/MANIFEST.MF」**，需要在这个文件中指定agent class，结下来看下相关属性：

| 属性名称 | 描述 |
| --- | --- |
| Premain-Class | 这个属性用来指定JVM启动时的代理agent，它必须包含premain方法，如果这个属性不存在，则JVM将终止。注意是类的全路径 |
| Agent-Class | 如果agent实现支持在JVM启动后某个时间启动代理的机制，那么该属性则指定该代理类。如果该属性不存在，代理将不会启动。 |
| Boot-Class-Path | 该属性可以指定BootStrapClassLoad加载的路径(路径需要带上指定哪个文件)，多个路径用空格分开 |
| Can-Redefine-Classes | 可选值就是true false，大小写无所谓，默认false。该属性用来指定该agent是否针对redefineClass产生作用 |
| Can-Retransform-Classes | 可选值就是true false，大小写无所谓，默认false。该属性用来指定该agent是否针对retransformClass产生作用 |

<a name="icwHd"></a>
### 文件生成方式
有两种方式生成此文件：

1. 手动创建此文件
2. 通过Maven插件
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-jar-plugin</artifactId>
      <version>3.1.0</version>
      <configuration>
        <archive>
          <!--自动添加META-INF/MANIFEST.MF -->
          <manifest>
            <addClasspath>true</addClasspath>
          </manifest>
          <manifestEntries>
            <Premain-Class>xxx</Premain-Class>
            <Agent-Class>xxx</Agent-Class>
            <Can-Redefine-Classes>true</Can-Redefine-Classes>
            <Can-Retransform-Classes>true</Can-Retransform-Classes>
          </manifestEntries>
        </archive>
      </configuration>
    </plugin>

  </plugins>
</build>
```
<a name="s5pLb"></a>
## 实战
接下来通过实战近距离感受下java agent的魅力。目标是**「替换目标类的行为」**。
<a name="Kw7ko"></a>
### 准备工作
这里初始化一个SpringBoot工程，随便搞一个简单的Controller如下：
```java
@RestController
public class MainController {
    @RequestMapping("/index")
    public String index(){
        return "hello world";
    }
}
```
那么当访问这个地址时，浏览器将会展现hello world字样，接下来将通过java agent来改变这个controller的行为。
<a name="BrOEl"></a>
## JVM启动前替换实现
<a name="bDGGp"></a>
### 定义`ClassFileTransformer`实现
在自定义的`ClassFileTransformer`中，通过javassist动态修改字节码，来更改Controller输出的内容
```java
public class MyClassFileTransformer implements ClassFileTransformer{
    @SneakyThrows
    @Override
    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined,
                            ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {
        if (className.contains("MainController")){
            final ClassPool classPool = ClassPool.getDefault();
            final CtClass clazz = classPool.get("com.cf.springboot.controller.MainController");
            CtMethod convertToAbbr = clazz.getDeclaredMethod("index");
            String methodBody = "return \"hello world【version2]\";";
            convertToAbbr.setBody(methodBody);
            // 返回字节码，并且detachCtClass对象
            byte[] byteCode = clazz.toBytecode();
            //detach的意思是将内存中曾经被javassist加载过的Date对象移除，如果下次有需要在内存中找不到会重新走javassist加载
            clazz.detach();
            return byteCode;
        }
        // 如果返回null则字节码不会被修改
        return null;
    }
}
```
<a name="GbN2l"></a>
### 定义agent class实现
```java
public class BeforeJvmAgent {
    public static void premain(String agentArgs, Instrumentation inst) {
        System.out.println("premain invoke!");

        inst.addTransformer(new MyClassFileTransformer());
    }

    public static void main(String[] args) {
        System.out.println("main invoke!");
    }

}
```
<a name="s8oBo"></a>
### 打包，设置命令行参数启动SpringBoot
启动后，观察控制台输出premain invoke!<br />可以看到premain最新被执行了，这时候访问下试试，浏览器显示了hello world【version2]<br />可以看到，修改已经生效～
<a name="fIiuY"></a>
## JVM启动后替换实现
在这里，`ClassFileTransformer`的实现还是复用4.2节中的，所以这里只需要看新实现。此刻开始，应用属于一直启动之中了，要做的就是真正意义上的**「热替换」**。
<a name="hTd2K"></a>
### agent class实现
```java
public class AfterJvmAgent {
    public static void agentmain(String agentArgs, Instrumentation inst)
    throws ClassNotFoundException, UnmodifiableClassException {
        inst.addTransformer(new MyClassFileTransformer(), true);
        // 关键点
        inst.retransformClasses(Class.forName("com.cf.springboot.controller.MainController",false,ClassLoader.getSystemClassLoader()));
    }

    public static void main(String[] args) {

    }
}
```
这里关键的一点就是在agentmain中手动retransform一下需要增强的类。
<a name="U0my7"></a>
### 启动应用程序，并attach
这里需要获取目标JVM程序，并且进行attach加载agent
```java
public static void main(String[] args) throws Exception{
    List<VirtualMachineDescriptor> list = VirtualMachine.list();
    for (VirtualMachineDescriptor vmd : list) {
        //如果虚拟机的名称为 xxx 则 该虚拟机为目标虚拟机，获取该虚拟机的 pid
        //然后加载 agent.jar 发送给该虚拟机
        System.out.println(vmd.displayName());
        if (vmd.displayName().equals("com.cf.springboot.Application")) {
            VirtualMachine virtualMachine = VirtualMachine.attach(vmd.id());
            virtualMachine.loadAgent("/Users/cc/code/spring-boot/after_jvm_agent/target/after_jvm_agent-0.0.1-SNAPSHOT.jar");
            virtualMachine.detach();
        }
    }
}
```
这个时候看再访问一下请求，浏览器显示了hello world【version2]。
