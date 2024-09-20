Java JavaAgent<br />熟悉Spring的小伙伴们应该都对aop比较了解，面向切面编程允许在目标方法的前后织入想要执行的逻辑，而今天要给大家介绍的Java Agent技术，在思想上与aop比较类似，翻译过来可以被称为Java代理、Java探针技术。<br />Java Agent出现在JDK1.5版本以后，它允许程序员利用agent技术构建一个独立于应用程序的代理程序，用途也非常广泛，可以协助监测、运行、甚至替换其他JVM上的程序，先从下面这张图直观的看一下它都被应用在哪些场景：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826212-162f5aeb-7810-4748-854a-2074b802743a.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u3bc5f4d1&originHeight=292&originWidth=530&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6607aedb-0268-4382-816f-b24c90c5258&title=)<br />看到这里是不是也很好奇，究竟是什么神仙技术，能够应用在这么多场景下，那今天就来挖掘一下，看看神奇的Java Agent是如何工作在底层，默默支撑了这么多优秀的应用。<br />回到文章开头的类比，还是用和aop比较的方式，来先对Java Agent有一个大致的了解：

- 作用级别：aop运行于应用程序内的方法级别，而agent能够作用于虚拟机级别
- 组成部分：aop的实现需要目标方法和逻辑增强部分的方法，而Java Agent要生效需要两个工程，一个是agent代理，另一个是需要被代理的主程序
- 执行场合：aop可以运行在切面的前后或环绕等场合，而Java Agent的执行只有两种方式，jdk1.5提供的preMain模式在主程序运行前执行，jdk1.6提供的agentMain在主程序运行后执行

下面就分别看一下在两种模式下，如何动手实现一个agent代理程序。
<a name="G1uNC"></a>
## Premain模式
Premain模式允许在主程序执行前执行一个agent代理，实现起来非常简单，下面分别实现两个组成部分。
<a name="sbm3I"></a>
### agent
先写一个简单的功能，在主程序执行前打印一句话，并打印传递给代理的参数：
```java
public class MyPreMainAgent {
    public static void premain(String agentArgs, Instrumentation inst) {
        System.out.println("premain start");
        System.out.println("args:"+agentArgs);
    }
}
```
在写完了agent的逻辑后，需要把它打包成jar文件，这里直接使用maven插件打包的方式，在打包前进行一些配置。
```xml
<build>
    <plugins>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-jar-plugin</artifactId>
            <version>3.1.0</version>
            <configuration>
                <archive>
                    <manifest>
                        <addClasspath>true</addClasspath>
                    </manifest>
                    <manifestEntries>
                        <Premain-Class>com.cn.agent.MyPreMainAgent</Premain-Class>                            
                        <Can-Redefine-Classes>true</Can-Redefine-Classes>
                        <Can-Retransform-Classes>true</Can-Retransform-Classes>
                        <Can-Set-Native-Method-Prefix>true</Can-Set-Native-Method-Prefix>
                    </manifestEntries>
                </archive>
            </configuration>
        </plugin>
    </plugins>
</build>
```
配置的打包参数中，通过manifestEntries的方式添加属性到MANIFEST.MF文件中，解释一下里面的几个参数：

- Premain-Class：包含premain方法的类，需要配置为类的全路径
- Can-Redefine-Classes：为true时表示能够重新定义class
- Can-Retransform-Classes：为true时表示能够重新转换class，实现字节码替换
- Can-Set-Native-Method-Prefix：为true时表示能够设置native方法的前缀

其中Premain-Class为必须配置，其余几项是非必须选项，默认情况下都为false，通常也建议加入，这几个功能会在后面具体介绍。在配置完成后，使用mvn命令打包：
```bash
mvn clean package
```
打包完成后生成myAgent-1.0.jar文件，可以解压jar文件，看一下生成的MANIFEST.MF文件：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826302-07d736b5-3457-460a-b08b-e1f061bdee45.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u9863e740&originHeight=207&originWidth=505&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5f6037c1-543e-426a-a98d-10b775f4f25&title=)<br />可以看到，添加的属性已经被加入到了文件中。到这里，agent代理部分就完成了，因为代理不能够直接运行，需要附着于其他程序，所以下面新建一个工程来实现主程序。
<a name="ghRcM"></a>
### 主程序
在主程序的工程中，只需要一个能够执行的`main`方法的入口就可以了。
```java
public class AgentTest {
    public static void main(String[] args) {
        System.out.println("main project start");
    }
}
```
在主程序完成后，要考虑的就是应该如何将主程序与agent工程连接起来。这里可以通过-javaagent参数来指定运行的代理，命令格式如下：
```bash
java -javaagent:myAgent.jar -jar AgentTest.jar
```
并且，可以指定的代理的数量是没有限制的，会根据指定的顺序先后依次执行各个代理，如果要同时运行两个代理，就可以按照下面的命令执行：
```bash
java -javaagent:myAgent1.jar -javaagent:myAgent2.jar  -jar AgentTest.jar
```
以在idea中执行程序为例，在VM options中加入添加启动参数：
```bash
-javaagent:F:\Workspace\MyAgent\target\myAgent-1.0.jar=Hydra
-javaagent:F:\Workspace\MyAgent\target\myAgent-1.0.jar=Trunks
```
执行main方法，查看输出结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826286-af0cbb0c-e795-4e9e-9d37-4b1e7042edbb.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u09112320&originHeight=253&originWidth=599&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7cdb19dd-2e92-46b1-88bf-08daad4a2a0&title=)<br />根据执行结果的打印语句可以看出，在执行主程序前，依次执行了两次agent代理。可以通过下面的图来表示执行代理与主程序的执行顺序。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826253-602e65ba-3bff-47ac-b934-56e9542ad396.webp#clientId=u0424cf57-ad4d-4&from=paste&id=uabf1ff26&originHeight=413&originWidth=598&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u43c6eae7-93bb-4881-a469-210e1830b77&title=)
<a name="GuVPr"></a>
### 缺陷
在提供便利的同时，premain模式也有一些缺陷，例如如果agent在运行过程中出现异常，那么也会导致主程序的启动失败。对上面例子中agent的代码进行一下改造，手动抛出一个异常。
```java
public static void premain(String agentArgs, Instrumentation inst) {
    System.out.println("premain start");
    System.out.println("args:"+agentArgs);
    throw new RuntimeException("error");
}
```
再次运行主程序：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826197-87f7851a-eaba-4e99-baac-ecde8587c60c.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u8de04394&originHeight=374&originWidth=999&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uabf0aea7-48f3-4ebd-a25d-ca28db8bedd&title=)<br />可以看到，在agent抛出异常后主程序也没有启动。针对premain模式的一些缺陷，在jdk1.6之后引入了agentmain模式。
<a name="W0zUc"></a>
## Agentmain模式
agentmain模式可以说是premain的升级版本，它允许代理的目标主程序的jvm先行启动，再通过attach机制连接两个jvm，下面分3个部分实现。
<a name="vs5uU"></a>
### agent
agent部分和上面一样，实现简单的打印功能：
```java
public class MyAgentMain {
    public static void agentmain(String agentArgs, Instrumentation instrumentation) {
        System.out.println("agent main start");
        System.out.println("args:"+agentArgs);
    }
}
```
修改maven插件配置，指定Agent-Class：
```xml
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-jar-plugin</artifactId>
    <version>3.1.0</version>
    <configuration>
        <archive>
            <manifest>
                <addClasspath>true</addClasspath>
            </manifest>
            <manifestEntries>
                <Agent-Class>com.cn.agent.MyAgentMain</Agent-Class>
                <Can-Redefine-Classes>true</Can-Redefine-Classes>
                <Can-Retransform-Classes>true</Can-Retransform-Classes>
            </manifestEntries>
        </archive>
    </configuration>
</plugin>
```
<a name="mjwFr"></a>
### 主程序
这里直接启动主程序等待代理被载入，在主程序中使用了`System.in`进行阻塞，防止主进程提前结束。
```java
public class AgentmainTest {
    public static void main(String[] args) throws IOException {
        System.in.read();
    }
}
```
<a name="eeh4C"></a>
### attach机制
和premain模式不同，不能再通过添加启动参数的方式来连接agent和主程序了，这里需要借助`com.sun.tools.attach`包下的`VirtualMachine`工具类，需要注意该类不是jvm标准规范，是由Sun公司自己实现的，使用前需要引入依赖：
```xml
<dependency>
    <groupId>com.sun</groupId>
    <artifactId>tools</artifactId>
    <version>1.8</version>
    <scope>system</scope>
    <systemPath>${JAVA_HOME}\lib\tools.jar</systemPath>
</dependency>
```
`VirtualMachine`代表了一个要被附着的java虚拟机，也就是程序中需要监控的目标虚拟机，外部进程可以使用`VirtualMachine`的实例将agent加载到目标虚拟机中。先看一下它的静态方法`attach`：
```java
public static VirtualMachine attach(String var0);
```
通过attach方法可以获取一个jvm的对象实例，这里传入的参数是目标虚拟机运行时的进程号pid。也就是说，在使用attach前，需要先获取刚才启动的主程序的pid，使用jps命令查看线程pid：
```bash
11140
16372 RemoteMavenServer36
16392 AgentmainTest
20204 Jps
2460 Launcher
```
获取到主程序`AgentmainTest`运行时pid是16392，将它应用于虚拟机的连接。
```java
public class AttachTest {
    public static void main(String[] args) {
        try {
            VirtualMachine  vm= VirtualMachine.attach("16392");
            vm.loadAgent("F:\\Workspace\\MyAgent\\target\\myAgent-1.0.jar","param");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
在获取到`VirtualMachine`实例后，就可以通过`loadAgent`方法可以实现注入agent代理类的操作，方法的第一个参数是代理的本地路径，第二个参数是传给代理的参数。执行`AttachTest`，再回到主程序`AgentmainTest`的控制台，可以看到执行了了agent中的代码：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826802-c124b84d-4c92-4471-bcee-00ff317950df.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u679d964f&originHeight=173&originWidth=628&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u75653ff5-c05e-4fc1-8f3c-eede1ab6823&title=)<br />这样，一个简单的agentMain模式代理就实现完成了，可以通过下面这张图再梳理一下三个模块之间的关系。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826598-c1ee23a9-a37c-445e-b58d-f77530906a16.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u4ec454b0&originHeight=197&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50ebd6c0-44ea-47f7-933d-90c64403dc0&title=)
<a name="Dd2s4"></a>
## 应用
到这里，就已经简单地了解了两种模式的实现方法，但是作为高质量程序员，肯定不能满足于只用代理单纯地打印语句，下面再来看看能怎么利用Java Agent搞点实用的东西。<br />在上面的两种模式中，agent部分的逻辑分别是在`premain`方法和`agentmain`方法中实现的，并且，这两个方法在签名上对参数有严格的要求，`premain`方法允许以下面两种方式定义：
```java
public static void premain(String agentArgs)
public static void premain(String agentArgs, Instrumentation inst)
```
`agentmain`方法允许以下面两种方式定义：
```java
public static void agentmain(String agentArgs)
public static void agentmain(String agentArgs, Instrumentation inst)
```
如果在agent中同时存在两种签名的方法，带有`Instrumentation`参数的方法优先级更高，会被jvm优先加载，它的实例`inst`会由jvm自动注入，下面就看看能通过`Instrumentation`实现什么功能。
<a name="zMQXA"></a>
### `Instrumentation`
先大体介绍一下`Instrumentation`接口，其中的方法允许在运行时操作java程序，提供了诸如改变字节码，新增jar包，替换class等功能，而通过这些功能使Java具有了更强的动态控制和解释能力。在编写agent代理的过程中，`Instrumentation`中下面3个方法比较重要和常用，来着重看一下。
<a name="gt1ee"></a>
#### `addTransformer`
`addTransformer`方法允许在类加载之前，重新定义Class，先看一下方法的定义：
```java
void addTransformer(ClassFileTransformer transformer);
```
`ClassFileTransformer`是一个接口，只有一个`transform`方法，它在主程序的`main`方法执行前，装载的每个类都要经过`transform`执行一次，可以将它称为转换器。可以实现这个方法来重新定义Class，下面就通过一个例子看看具体如何使用。<br />首先，在主程序工程创建一个`Fruit`类：
```java
public class Fruit {
    public void getFruit(){
        System.out.println("banana");
    }
}
```
编译完成后复制一份class文件，并将其重命名为`Fruit2.class`，再修改`Fruit`中的方法为：
```java
public void getFruit(){
    System.out.println("apple");
}
```
创建主程序，在主程序中创建了一个`Fruit`对象并调用了其`getFruit`方法：
```java
public class TransformMain {
    public static void main(String[] args) {
        new Fruit().getFruit();
    }
}
```
这时执行结果会打印`apple`，接下来开始实现premain代理部分。<br />在代理的`premain`方法中，使用`Instrumentation`的`addTransformer`方法拦截类的加载：
```java
public class TransformAgent {
    public static void premain(String agentArgs, Instrumentation inst) {
        inst.addTransformer(new FruitTransformer());
    }
}
```
`FruitTransformer`类实现了`ClassFileTransformer`接口，转换class部分的逻辑都在`transform`方法中：
```java
public class FruitTransformer implements ClassFileTransformer {
    @Override
    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined,
                            ProtectionDomain protectionDomain, byte[] classfileBuffer){
        if (!className.equals("com/cn/hydra/test/Fruit"))
            return classfileBuffer;

        String fileName="F:\\Workspace\\agent-test\\target\\classes\\com\\cn\\hydra\\test\\Fruit2.class";
        return getClassBytes(fileName);
    }

    public static byte[] getClassBytes(String fileName){
        File file = new File(fileName);
        try(InputStream is = new FileInputStream(file);
            ByteArrayOutputStream bs = new ByteArrayOutputStream()){
            long length = file.length();
            byte[] bytes = new byte[(int) length];

            int n;
            while ((n = is.read(bytes)) != -1) {
                bs.write(bytes, 0, n);
            }
            return bytes;
        }catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
```
在`transform`方法中，主要做了两件事：

- 因为`addTransformer`方法不能指明需要转换的类，所以需要通过`className`判断当前加载的class是否要拦截的目标class，对于非目标class直接返回原字节数组，注意`className`的格式，需要将类全限定名中的`.`替换为`/`
- 读取之前复制出来的class文件，读入二进制字符流，替换原有`classfileBuffer`字节数组并返回，完成class定义的替换

将agent部分打包完成后，在主程序添加启动参数：
```bash
-javaagent:F:\Workspace\MyAgent\target\transformAgent-1.0.jar
```
再次执行主程序，结果打印：
```java
banana
```
这样，就实现了在`main`方法执行前class的替换。
<a name="nwXeE"></a>
#### `redefineClasses`
可以直观地从方法的名字上来理解它的作用，重定义class，通俗点来讲的话就是实现指定类的替换。方法定义如下：
```java
void redefineClasses(ClassDefinition... definitions) throws  ClassNotFoundException, UnmodifiableClassException;
```
它的参数是可变长的`ClassDefinition`数组，再看一下`ClassDefinition`的构造方法：
```java
public ClassDefinition(Class<?> theClass,byte[] theClassFile) {...}
```
`ClassDefinition`中指定了的Class对象和修改后的字节码数组，简单来说，就是使用提供的类文件字节，替换了原有的类。并且，在`redefineClasses`方法重定义的过程中，传入的是`ClassDefinition`的数组，它会按照这个数组顺序进行加载，以便满足在类之间相互依赖的情况下进行更改。<br />下面通过一个例子来看一下它的生效过程，premain代理部分：
```java
public class RedefineAgent {
    public static void premain(String agentArgs, Instrumentation inst) 
            throws UnmodifiableClassException, ClassNotFoundException {
        String fileName="F:\\Workspace\\agent-test\\target\\classes\\com\\cn\\hydra\\test\\Fruit2.class";
        ClassDefinition def=new ClassDefinition(Fruit.class,
                FruitTransformer.getClassBytes(fileName));
        inst.redefineClasses(new ClassDefinition[]{def});
    }
}
```
主程序可以直接复用上面的，执行后打印：
```java
banana
```
可以看到，用指定的class文件的字节替换了原有类，即实现了指定类的替换。
<a name="Jh243"></a>
#### `retransformClasses`
`retransformClasses`应用于`agentmain`模式，可以在类加载之后重新定义`Class`，即触发类的重新加载。首先看一下该方法的定义：
```java
void retransformClasses(Class<?>... classes) throws UnmodifiableClassException;
```
它的参数`classes`是需要转换的类数组，可变长参数也说明了它和`redefineClasses`方法一样，也可以批量转换类的定义。<br />下面，通过例子来看看如何使用`retransformClasses`方法，agent代理部分代码如下：
```java
public class RetransformAgent {
    public static void agentmain(String agentArgs, Instrumentation inst)
            throws UnmodifiableClassException {
        inst.addTransformer(new FruitTransformer(),true);
        inst.retransformClasses(Fruit.class);
        System.out.println("retransform success");
    }
}
```
看一下这里调用的`addTransformer`方法的定义，与上面略有不同：
```java
void addTransformer(ClassFileTransformer transformer, boolean canRetransform);
```
`ClassFileTransformer`转换器依旧复用了上面的`FruitTransformer`，重点看一下新加的第二个参数，当`canRetransform`为true时，表示允许重新定义class。这时，相当于调用了转换器`ClassFileTransformer`中的`transform`方法，会将转换后class的字节作为新类定义进行加载。<br />主程序部分代码，在死循环中不断的执行打印语句，来监控类是否发生了改变：
```java
public class RetransformMain {
    public static void main(String[] args) throws InterruptedException {
        while(true){
            new Fruit().getFruit();
            TimeUnit.SECONDS.sleep(5);
        }
    }
}
```
最后，使用attach api注入agent代理到主程序中：
```java
public class AttachRetransform {
    public static void main(String[] args) throws Exception {
        VirtualMachine vm = VirtualMachine.attach("6380");
        vm.loadAgent("F:\\Workspace\\MyAgent\\target\\retransformAgent-1.0.jar");
    }
}
```
回到主程序控制台，查看运行结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826785-e5ea9e7e-ba79-4f58-b60f-a4019fae7b7e.webp#clientId=u0424cf57-ad4d-4&from=paste&id=uefb281a7&originHeight=291&originWidth=624&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue30fea8d-fc5d-443e-a52a-2bc2ee7fb50&title=)<br />可以看到在注入代理后，打印语句发生变化，说明类的定义已经被改变并进行了重新加载。
<a name="KOrg0"></a>
#### 其他
除了这几个主要的方法外，`Instrumentation`中还有一些其他方法，这里仅简单列举一下常用方法的功能：

- `removeTransformer`：删除一个`ClassFileTransformer`类转换器
- `getAllLoadedClasses`：获取当前已经被加载的Class
- `getInitiatedClasses`：获取由指定的`ClassLoader`加载的Class
- `getObjectSize`：获取一个对象占用空间的大小
- `appendToBootstrapClassLoaderSearch`：添加jar包到启动类加载器
- `appendToSystemClassLoaderSearch`：添加jar包到系统类加载器
- `isNativeMethodPrefixSupported`：判断是否能给`native`方法添加前缀，即是否能够拦截`native`方法
- `setNativeMethodPrefix`：设置`native`方法的前缀
<a name="UqIv5"></a>
### Javassist
在上面的几个例子中，都是直接读取的class文件中的字节来进行class的重定义或转换，但是在实际的工作环境中，可能更多的是去动态的修改class文件的字节码，这时候就可以借助javassist来更简单的修改字节码文件。<br />简单来说，javassist是一个分析、编辑和创建java字节码的类库，在使用时可以直接调用它提供的api，以编码的形式动态改变或生成class的结构。相对于ASM等其他要求了解底层虚拟机指令的字节码框架，javassist真的是非常简单和快捷。<br />下面，就通过一个简单的例子，看看如何将Java agent和Javassist结合在一起使用。首前先引入javassist的依赖：
```xml
<dependency>
    <groupId>org.javassist</groupId>
    <artifactId>javassist</artifactId>
    <version>3.20.0-GA</version>
</dependency>
```
要实现的功能是通过代理，来计算方法执行的时间。premain代理部分和之前基本一致，先添加一个转换器：
```java
public class Agent {
    public static void premain(String agentArgs, Instrumentation inst) {
        inst.addTransformer(new LogTransformer());
    }

    static class LogTransformer implements ClassFileTransformer {
        @Override
        public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined, 
                                ProtectionDomain protectionDomain, byte[] classfileBuffer) 
            throws IllegalClassFormatException {
            if (!className.equals("com/cn/hydra/test/Fruit"))
                return null;

            try {
                return calculate();
            } catch (Exception e) {
                e.printStackTrace();
                return null;
            }
        }
    }
}
```
在`calculate`方法中，使用`javassist`动态的改变了方法的定义：
```java
static byte[] calculate() throws Exception {
    ClassPool pool = ClassPool.getDefault();
    CtClass ctClass = pool.get("com.cn.hydra.test.Fruit");
    CtMethod ctMethod = ctClass.getDeclaredMethod("getFruit");
    CtMethod copyMethod = CtNewMethod.copy(ctMethod, ctClass, new ClassMap());
    ctMethod.setName("getFruit$agent");

    StringBuffer body = new StringBuffer("{\n")
            .append("long begin = System.nanoTime();\n")
            .append("getFruit$agent($$);\n")
            .append("System.out.println(\"use \"+(System.nanoTime() - begin) +\" ns\");\n")
            .append("}");
    copyMethod.setBody(body.toString());
    ctClass.addMethod(copyMethod);
    return ctClass.toBytecode();
}
```
在上面的代码中，主要实现了这些功能：

- 利用全限定名获取类`CtClass`
- 根据方法名获取方法`CtMethod`，并通过`CtNewMethod.copy`方法复制一个新的方法
- 修改旧方法的方法名为`getFruit$agent`
- 通过`setBody`方法修改复制出来方法的内容，在新方法中进行了逻辑增强并调用了旧方法，最后将新方法添加到类中

主程序仍然复用之前的代码，执行查看结果，完成了代理中的执行时间统计功能：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826875-af2d808a-7fd2-4457-bbc1-79281cf6f40c.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u59c1c3bd&originHeight=181&originWidth=607&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81dec62f-8cf9-48d2-a022-19d8be62ada&title=)<br />这时候可以再通过反射看一下：
```java
for (Method method : Fruit.class.getDeclaredMethods()) {
    System.out.println(method.getName());
    method.invoke(new Fruit());
    System.out.println("-------");
}
```
查看结果，可以看到类中确实已经新增了一个方法：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638498826648-8cd28d69-5697-4891-beb6-bac0dfc597ce.webp#clientId=u0424cf57-ad4d-4&from=paste&id=u88cdab88&originHeight=282&originWidth=590&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa22a971-cff9-49a0-ad3b-6f41ba06307&title=)<br />除此之外，javassist还有很多其他的功能，例如新建Class、设置父类、读取和写入字节码等等，大家可以在具体的场景中学习它的用法。
<a name="hffRz"></a>
## 总结
虽然在平常的工作中，直接用到Java Agent的场景可能并不是很多，但是在热部署、监控、性能分析等工具中，它们可能隐藏在业务系统的角落里，一直在默默发挥着巨大的作用。<br />本文从Java Agent的两种模式入手，手动实现并简要分析了它们的工作流程，虽然在这里只利用它们完成了一些简单的功能，但是不得不说，正是Java Agent的出现，让程序的运行不再循规蹈矩，也为代码提供了无限的可能性。
