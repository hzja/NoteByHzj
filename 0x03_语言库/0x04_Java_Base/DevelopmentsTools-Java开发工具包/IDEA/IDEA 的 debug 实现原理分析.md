Java IDEA
<a name="W3r4J"></a>
### 对 Debug 的好奇
初学 Java 时，IDEA 的 Debug 非常强大，不止是它能查看断点的上下文环境，更神奇的是可以在断点处使用它的 Evaluate 功能直接执行某些命令，进行一些计算或改变当前变量。<br />刚开始语法不熟经常写错代码，重新打包部署一次代码耗时很长，直接面向 Debug 开发。在要编写的方法开始处打一个断点，在 Evaluate 框内一次次地执行方法函数不停地调整代码，没问题后再将代码复制出来放到 IDEA 里，再进行下一个方法的编写，这样就跟写 PHP 类似的解释性语言一样，写完即执行，非常方便。<br />![2021-05-06-21-18-13-863151.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620307219450-2eaf2150-f0dd-4772-acf6-106761429362.png#clientId=ue23ee3d8-487c-4&from=ui&id=u09b8b129&originHeight=353&originWidth=1080&originalType=binary&size=130613&status=done&style=none&taskId=u832cbdef-7132-4093-983a-d0292291831)<br />但 Java 是静态语言，运行之前是要先进行编译的，难道写的这些代码是被实时编译又”注入”到正在 Debug 的服务里了吗？<br />随着对 Java 的愈加熟悉，了解了反射、字节码等技术。
<a name="nGL5R"></a>
### ASM
实现 Evaluate 要解决的第一个问题就是怎么改变原有代码的行为，它的实现在 Java 里被称为动态字节码技术。
<a name="FNUt6"></a>
#### 动态生成字节码
编写的 Java 代码都是要被编译成字节码后才能放到 JVM 里执行的，而字节码一旦被加载到虚拟机中，就可以被解释执行。<br />字节码文件（.class）就是普通的二进制文件，它是通过 Java 编译器生成的。而只要是文件就可以被改变，如果用特定的规则解析了原有的字节码文件，对它进行修改或者干脆重新定义，这不就可以改变代码行为了么。<br />Java 生态里有很多可以动态生成字节码的技术，像 BCEL、Javassist、ASM、CGLib 等，它们各有自己的优势。有的使用复杂却功能强大、有的简单确也性能些差。
<a name="E5Brm"></a>
#### ASM 框架
ASM 是它们中最强大的一个，使用它可以动态修改类、方法，甚至可以重新定义类，连 CGLib 底层都是用 ASM 实现的。<br />当然，它的使用门槛也很高，使用它需要对 Java 的字节码文件有所了解，熟悉 JVM 的编译指令。虽然对 JVM 的字节码语法不熟，但有大神开发了可以在 IDEA 里查看字节码的插件：`ASM Bytecode Outline` ，在要查看的类文件里右键选择 `Show bytecode Outline` 即可以右侧的工具栏查看要生成的字节码。对照着示例，就可以很轻松地写出操作字节码的 Java 代码了。<br />而切到 `ASMified` 标签栏，甚至可以直接获取到 ASM 的使用代码。<br />![2021-05-06-21-18-17-298661.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620307195207-cb5fea1d-e290-4d68-a69a-61b12b7bc4b8.png#clientId=ue23ee3d8-487c-4&from=ui&id=uc8808d72&originHeight=497&originWidth=1080&originalType=binary&size=185526&status=done&style=none&taskId=u3365ad78-31a4-4f47-8922-bd7d67acc57)
<a name="cGvZt"></a>
#### 常用方法
在 ASM 的代码实现里，最明显的就是访问者模式，ASM 将对代码的读取和操作都包装成一个访问者，在解析 JVM 加载到的字节码时调用。<br />`ClassReader` 是 ASM 代码的入口，通过它解析二进制字节码，实例化时它时，需要传入一个 `ClassVisitor`，在这个 `Visitor` 里，可以实现 `visitMethod()`/`visitAnnotation()` 等方法，用以定义对类结构（如方法、字段、注解）的访问方法。<br />而 `ClassWriter` 接口继承了 `ClassVisitor` 接口，在实例化类访问器时，将 `ClassWriter` “注入” 到里面，以实现对类写入的声明。
<a name="HGgBu"></a>
### Instrument
<a name="yor50"></a>
#### 介绍
字节码是修改完了，可是 JVM 在执行时会使用自己的类加载器加载字节码文件，加载后并不会理会做出的修改，要想实现对现有类的修改，还需要搭配 Java 的另一个库 instrument。<br />instrument 是 JVM 提供的一个可以修改已加载类文件的类库。1.6以前，instrument 只能在 JVM 刚启动开始加载类时生效，之后，instrument 更是支持了在运行时对类定义的修改。
<a name="mKlra"></a>
#### 使用
要使用 `instrument` 的类修改功能，需要实现它的 `ClassFileTransformer` 接口定义一个类文件转换器。它唯一的一个 `transform()` 方法会在类文件被加载时调用，在 `transform` 方法里，可以对传入的二进制字节码进行改写或替换，生成新的字节码数组后返回，JVM 会使用 `transform` 方法返回的字节码数据进行类的加载。
<a name="It9FI"></a>
### JVM TI
定义完了字节码的修改和重定义方法，但怎么才能让 JVM 能够调用我们提供的类转换器呢？这里又要介绍到 JVM TI 了。
<a name="hlwUM"></a>
#### 介绍
JVM TI（JVM Tool Interface）JVM 工具接口是 JVM 提供的一个非常强大的对 JVM 操作的工具接口，通过这个接口，可以实现对 JVM 多种组件的操作，从JVMTM Tool Interface 这里可以认识到 JVM TI 的强大，它包括了对虚拟机堆内存、类、线程等各个方面的管理接口。<br />JVM TI 通过事件机制，通过接口注册各种事件勾子，在 JVM 事件触发时同时触发预定义的勾子，以实现对各个 JVM 事件的感知和反应。
<a name="mBKTq"></a>
#### Agent
Agent 是 JVM TI 实现的一种方式。在编译 C 项目里链接静态库，将静态库的功能注入到项目里，从而才可以在项目里引用库里的函数。可以将 agent 类比为 C 里的静态库，也可以用 C 或 C++ 来实现，将其编译为 dll 或 so 文件，在启动 JVM 时启动。<br />这时再来思考 Debug 的实现，在启动被 Debug 的 JVM 时，必须添加参数 `-agentlib:jdwp=transport=dt_socket,suspend=y,address=localhost:3333`，而 -agentlib 选项就指定了要加载的 Java Agent，jdwp 是 agent 的名字，在 linux 系统中，可以在 jre 目录下找到 jdwp.so 库文件。<br />Java 的调试体系 jdpa 组成，从高到低分别为 jdi->jdwp->jvmti，通过 JDI 接口发送调试指令，而 jdwp 就相当于一个通道，翻译 JDI 指令到 JVM TI，最底层的 JVM TI 最终实现对 JVM 的操作。
<a name="JCybF"></a>
#### 使用
JVM TI 的 agent 使用很简单，在启动 agent 时添加 `-agent` 参数指定要加载的 agent jar包即可。<br />而要实现代码的修改，需要实现一个 instrument agent，它可以通过在一个类里添加 `premain()` 或 `agentmain()` 方法来实现。而要实现 1.6 以上的动态 instrument 功能，实现 agentmain 方法即可。<br />在 `agentmain` 方法里，调用 `Instrumentation.retransformClasses()` 方法实现对目标类的重定义。<br />另外往一个正在运行的 JVM 里动态添加 agent，还需要用到 JVM 的 attach 功能，Sun 公司的 tools.jar 包里包含的 `VirtualMachine` 类提供了 attach 一个本地 JVM 的功能，它需要传入一个本地 JVM 的 pid， tools.jar 可以在 jre 目录下找到。
<a name="frkX2"></a>
#### agent生成
另外，还需要注意 agent 的打包，它需要指定一个 Agent-Class 参数指定包括 `agentmain` 方法的类，可以算是指定入口类。<br />此外，还需要配置 `MANIFEST.MF` 文件的一些参数，允许重新定义类。如果 agent 实现还需要引用一些其他类库时，还需要将这些类库都打包到此 jar 包中，下面是 pom 文件配置。
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-assembly-plugin</artifactId>
      <configuration>
        <archive>
          <manifestEntries>
            <Agent-Class>asm.TestAgent</Agent-Class>
            <Can-Redefine-Classes>true</Can-Redefine-Classes>
            <Can-Retransform-Classes>true</Can-Retransform-Classes>
            <Manifest-Version>1.0</Manifest-Version>
            <Permissions>all-permissions</Permissions>
          </manifestEntries>
        </archive>
        <descriptorRefs>
          <descriptorRef>jar-with-dependencies</descriptorRef>
        </descriptorRefs>
      </configuration>
    </plugin>
  </plugins>
</build>
```
另外在打包时需要使用 `mvn assembly:assembl` 命令生成 jar-with-dependencies 作为 agent。
<a name="Y6LEe"></a>
### 代码实现
在测试时写了一个用以上技术实现了一个简单的字节码动态修改的 Demo。
<a name="D05DL"></a>
#### 被修改的类
`TransformTarget` 是要被修改的目标类，正常执行时，它会三秒输出一次 “hello”。
```java
public class TransformTarget {
    public static void main(String[] args) {
        while (true) {
            try {
                Thread.sleep(3000L);
            } catch (Exception e) {
                break;
            }
            printSomething();
        }
    }

    public static void printSomething() {
        System.out.println("hello");
    }

}
```
<a name="zNu6O"></a>
#### Agent
Agent 是执行修改类的主体，它使用 ASM 修改 `TransformTarget` 类的方法，并使用 instrument 包将修改提交给 JVM。<br />入口类，也是代理的 Agent-Class。
```java
public class TestAgent {
    public static void agentmain(String args, Instrumentation inst) {
        inst.addTransformer(new TestTransformer(), true);
        try {
            inst.retransformClasses(TransformTarget.class);
            System.out.println("Agent Load Done.");
        } catch (Exception e) {
            System.out.println("agent load failed!");
        }
    }
}
```
执行字节码修改和转换的类。
```java
public class TestTransformer implements ClassFileTransformer {

    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined, ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {
        System.out.println("Transforming " + className);
        ClassReader reader = new ClassReader(classfileBuffer);
        ClassWriter classWriter = new ClassWriter(ClassWriter.COMPUTE_FRAMES);
        ClassVisitor classVisitor = new TestClassVisitor(Opcodes.ASM5, classWriter);
        reader.accept(classVisitor, ClassReader.SKIP_DEBUG);
        return classWriter.toByteArray();
    }

    class TestClassVisitor extends ClassVisitor implements Opcodes {
        TestClassVisitor(int api, ClassVisitor classVisitor) {
            super(api, classVisitor);
        }

        @Override
        public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
            MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
            if (name.equals("printSomething")) {
                mv.visitCode();
                Label l0 = new Label();
                mv.visitLabel(l0);
                mv.visitLineNumber(19, l0);
                mv.visitFieldInsn(Opcodes.GETSTATIC, "java/lang/System", "out", "Ljava/io/PrintStream;");
                mv.visitLdcInsn("bytecode replaced!");
                mv.visitMethodInsn(Opcodes.INVOKEVIRTUAL, "java/io/PrintStream", "println", "(Ljava/lang/String;)V", false);
                Label l1 = new Label();
                mv.visitLabel(l1);
                mv.visitLineNumber(20, l1);
                mv.visitInsn(Opcodes.RETURN);
                mv.visitMaxs(2, 0);
                mv.visitEnd();
                TransformTarget.printSomething();
            }
            return mv;
        }
    }
}
```
<a name="gH6ZY"></a>
#### Attacher
使用 tools.jar 里方法将 agent 动态加载到目标 JVM 的类。
```java
public class Attacher {
    public static void main(String[] args) throws AttachNotSupportedException, IOException, AgentLoadException, AgentInitializationException {
        VirtualMachine vm = VirtualMachine.attach("34242"); // 目标 JVM pid
        vm.loadAgent("/path/to/agent.jar");
    }
}
```
这样，先启动 `TransformTarget` 类，获取到 pid 后将其传入 Attacher 里，并指定 agent jar，将 agent attach 到 TransformTarget 中，原来输出的 “hello” 就变成想要修改的 “bytecode replaced!” 了。<br />![2021-05-06-21-18-17-686249.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620307187249-59cbd867-8083-4eed-9562-8edf697b66b8.png#clientId=ue23ee3d8-487c-4&from=ui&id=FhsSx&originHeight=408&originWidth=866&originalType=binary&size=68304&status=done&style=none&taskId=u23ca50ab-f3f6-4705-85e2-6fb435a4207)
<a name="odu3L"></a>
### 小结
掌握了字节码的动态修改技术后，再回头看 Btrace 的原理就更清晰了，稍微摸索一下也可以实现一个简版的。另外很多大牛实现的各种 Java 性能分析工具的技术栈也不外如此，了解了这些，未来也可以写出适合自己的工具，至少能对别人的工具进行修改~
