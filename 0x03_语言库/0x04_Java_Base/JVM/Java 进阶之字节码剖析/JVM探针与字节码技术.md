JVM 字节码<br />JVM探针是自jdk1.5以来，由虚拟机提供的一套监控类加载器和符合虚拟机规范的代理接口，结合字节码指令能够让开发者实现无侵入的监控功能。如：监控生产环境中的函数调用情况或动态增加日志输出等等。虽然在常规的业务中不会有太多用武之地，但是作为一项高级的技术手段也应该是资深开发人员的必备技能之一。同时，它也是企业级开发和生产环境部署不可或缺的技术方案，是对当下流行的APM的一种补充，因为使用探针技术能够实现比常规APM平台更细粒度的监控。<br />哪些方面适合使用探针技术：<br />(1) 如果发现生产环境上有些问题无法在测试或开发环境中复现<br />(2) 如果希望在不修改源码的情况下为应用添加一些输出日志<br />(3) 如果在刚发布的生产包中发现了一个bug，而又不希望被它阻断，希望有一个临时的补救措施
<a name="sirt9"></a>
## 一、JVM探针：`Instrumentation`
使用探针只需要一条附加选项：`-javaagent:<jar 路径>[=<选项>]`，作为探针（代理）的`jar`包必须满足两个条件：

1. MANIFEST.MF文件需要增加`Premain-Class`项，说明启动类。
2. 启动类必须声明一个静态函数，它的入参是: `String`和`Instrumentation`。因此，一个常见的启动类可能像这样：
```java
package aa.bb.cc;

public class PremainAgent {
    public static void premain(String agentArgs, Instrumentation inst) {
        // TODO
    }
}
```
MANIFEST.MF
```
premain-class: aa.bb.cc.PremainAgent
```
如果使用maven作为构建工具，需要在pom文件中添加构建插件
```xml
<plugin>  
    <groupId>org.apache.maven.plugins</groupId>  
    <artifactId>maven-jar-plugin</artifactId>  
    <version>3.2.0</version>  
    <configuration>  
        <archive>  
            <manifestEntries>  
                <premain-class>aa.bb.cc.PremainAgent</premain-class>  
            </manifestEntries>  
        </archive>  
    </configuration>  
</plugin>
```
如果还引入了其它依赖希望同时打包，那么应该使用assembly插件替代
```xml
<plugin>  
  <groupId>org.apache.maven.plugins</groupId>  
  <artifactId>maven-assembly-plugin</artifactId>  
  <version>2.4</version>  
  <configuration>  
    <descriptorRefs>  
      <descriptorRef>jar-with-dependencies</descriptorRef>  
    </descriptorRefs>  
    <archive>  
      <manifestEntries>  
        <Premain-Class>
          <package>
            .PremainAgent
          </package>
        </Premain-Class>
        <Can-Redefine-Classes>true</Can-Redefine-Classes>  
        <Can-Retransform-Classes>true</Can-Retransform-Classes>  
      </manifestEntries>  
    </archive>  
  </configuration>  
  <executions>  
    <execution>  
      <phase>package</phase>  
      <goals>  
        <goal>single</goal>  
      </goals>  
    </execution>  
  </executions>
</plugin>
```
两个重要的类<br />`Instrumentation`: 由JDK提供的一个探针类，它会负责加载用户自定义的`ClassFileTransformer`<br />`ClassFileTransformer`: 字节码转换类，jvm在加载class文件前会先调用它，对所有类加载器有效<br />具体用法稍后会做详细介绍。<br />总结：JVM探针只是提供了一种让开发人员能够在类加载加载class文件前主动介入的一种方法，具体如何操作需要开发人员了解Java虚拟机规范以及字节码的相关知识。
<a name="UacXN"></a>
## 二、栈帧与指令集
栈帧（Stack Frame）是用于支持虚拟机进行方法调用和方法执行的数据结构。它是虚拟机运行时数据区中的虚拟机栈的栈元素。栈帧存储了方法的局部变量表、操作数栈、动态连接和方法返回地址等信息。每一个方法从调用开始至执行完成的过程，都对应着一个栈帧在虚拟机里面从入栈到出栈的过程。<br />在编译程序代码的时候，栈帧中需要多大的局部变量表，多深的操作数栈都已经完全确定了。因此一个栈帧需要分配多少内存，不会受到程序运行期变量数据的影响，而仅仅取决于具体的虚拟机实现。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633529397518-14461a5c-1aae-47ab-8a0a-9da1fc2b149f.webp#clientId=u6344e9ba-b08a-4&from=paste&id=u99e19eac&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=shadow&taskId=u278a6c28-b5ad-4d77-83b2-16d5be4849f)<br />局部变量表（Local Variable Table）是一组变量值存储空间，用于存放方法参数和方法内部定义的局部变量。并且在Java编译为Class文件时，就已经确定了该方法所需要分配的局部变量表的最大容量。局部变量表类似一个数组结构，虚拟机在访问局部变量表的时候会使用下标作为引用，普通方法的局部变量表中第0位索引默认是用于传递方法所属对象实例的引用this。<br />操作数栈（Operand Stack）和局部变量表一样，在编译时期就已经确定了该方法所需要分配的局部变量表的最大容量。当一个方法刚刚开始执行的时候，这个方法的操作数栈是空的，在方法执行的过程中，会有各种字节码指令往操作数栈中写入和提取内容，也就是出栈/入栈操作。例如，在做算术运算的时候是通过操作数栈来进行的，又或者在调用其它方法的时候是通过操作数栈来进行参数传递的。<br />动态链接（Dynamic Linking）每个栈帧都包含一个指向运行时常量池中该栈帧所属方法的引用，持有这个引用是为了支付方法调用过程中的动态连接。在类加载阶段中的解析阶段会将符号引用转为直接引用，这种转化也称为静态解析。另外的一部分将在每一次运行时期转化为直接引用，这部分称为动态连接。<br />返回地址：当一个方法开始执行后，只有2种方式可以退出这个方法，方法返回指令和异常退出。无论采用任何退出方式，在方法退出之后，都需要返回到方法被调用的位置，程序才能继续执行，方法返回时可能需要在栈帧中保存一些信息。一般来说，方法正常退出时，调用者的PC计数器的值可以作为返回地址，栈帧中会保存这个计数器值。而方法异常退出时，返回地址是要通过异常处理器表来确定的，栈帧中一般不会保存这部分信息。<br />JVM指令集并非是对Java语句的直接翻译，由于指令只使用1个字节表示，所以指令集最多只能包含256种指令。因此，一条Java语句一般会对应多条底层指令。每一条指令都有与之对应的助记符，可以通过官方资料查看它们对应关系：[https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html](https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html)。为了帮助大家更加直观的理解字节码指令，通过三个用例分别解释。<br />从一个简单的加法函数开始，可以使用`javac`将.java文件编译成.class，再通过`javap -c`查看它的字节码文件
```java
public int add(int x, int y) {  
    return x + y;  
}
```
```java
public add(II)I
    ILOAD 1 // 将局部变量表中#1变量入栈
    ILOAD 2 // 将局部变量表中#2变量入栈
    IADD // 调用整型数相加（两个数出栈，再将结果入栈)
    IRETURN // 返回栈顶的结果
    MAXSTACK = 2 // 最大栈数2
    MAXLOCALS = 3 // 最大本地变量数3
```
第一行是它的函数签名，2~7行的注释分别是对指令的解释。`ILOAD`，`IADD`，`IRETURN`分别是整型数的入栈，加法和返回操作。大家可以将`add`方法修改为静态函数后重新编译，看看`MAXLOCALS`是否有变化。<br />接下来把函数变得复杂一些，尝试对函数的执行时间做一个计算并输出
```java
public int add(int x, int y) {  
    long t = System.nanoTime();  
    int ret = x + y;  
    t = System.nanoTime() - t;  
    System.out.println(t);  
    return ret;  
}
```
```java
public add(II)I
    INVOKESTATIC java/lang/System.nanoTime ()J // 调用静态函数，结果long入栈
    LSTORE 3 // 将栈顶的long保存到局部变量#3
    ILOAD 1
    ILOAD 2
    IADD
    ISTORE 5 // 将栈顶的int保存到局部变量#5
    INVOKESTATIC java/lang/System.nanoTime ()J
    LLOAD 3 // 局部变量#3入栈
    LSUB // 从栈顶弹出两个long相减
    LSTORE 3 // 结果保存到变量#3
    GETSTATIC java/lang/System.out : Ljava/io/PrintStream; // 获取静态引用
    LLOAD 3 // 局部变量#3入栈
    INVOKEVIRTUAL java/io/PrintStream.println (J)V // 调用函数
    ILOAD 5 //  局部变量#5入栈
    IRETURN
    MAXSTACK = 4
    MAXLOCALS = 6
```
第2行结尾的J表示函数返回值是long类型。第14行结尾的V表示`println`函数的返回值是void。第12行到第14行的指令对应代码的`System.out.println(t)`，特别需要注意的是`INVOKEVIRTUAL`指令实际上需要从操作数栈获取两个数，第一个数是在执行了`GETSTATIC`后入栈的对象引用。<br />再次修改函数，这一次引入比较和循环语句，尽管代码的逻辑不太正常，但这并不妨碍理解
```java
public int add(int x, int y) {  
    if(x > 1) {  
        return x + y;  
    }  
    for(int i = 0; i < y; i++) {  
        x ++;  
    }  
    return x - y;  
}
```
```java
public add(II)I
    ILOAD 1
    ICONST_1 // 将一个常整型数1入栈
    IF_ICMPLE L0 // 比较如果操两个操作数是小于等于的关系则成立，否则跳转到L0的位置继续
    ILOAD 1
    ILOAD 2
    IADD
    IRETURN
   L0
    ICONST_0 // 将常整型数0入栈
    ISTORE 3 // 栈顶数保存到局部变量#3
   L1
    ILOAD 3
    ILOAD 2
    IF_ICMPGE L2 // 比较栈顶的两个操作数是否是大于等于的关系，如果不成立则跳转到L2
    IINC 1 1 // 局部变量#1 自增1
    IINC 3 1 // 局部变量#3 自增1
    GOTO L1 // 跳转到L1执行
   L2
    ILOAD 1
    ILOAD 2
    ISUB
    IRETURN
    MAXSTACK = 2
    MAXLOCALS = 4
```
当使用字节码直接操作虚拟机中的底层代码的时候，基本上就是通过改变局部变量表和操作数栈来改变程序的逻辑。还记得根据Java虚拟机规范，`MAXSTACK`和`MAXLOCALS`是在.java文件被编译成.class就被确定下来的吗，如果要对方法做出修改势必会引入新的局部变量，这时就难免需要对`MAXSTACK`和`MAXLOCALS`做重新计算。好在目前流行的字节码框架已经可以自动完成这项任务。
<a name="IbOuF"></a>
## 三、ASM框架 
ASM是一个比较硬核的字节码框架，也是转换效率最高的工具。下面是常用类的介绍：
<a name="hSMLI"></a>
#### 1. `ClassReader`
按照Java虚拟机规范(JVMS)中定义的方式来解析class文件中的内容，在遇到合适的字段时调用`ClassVisitor`中相对应的方法。
```java
ClassReader(final byte[] classFile)
```
构造方法，通过class字节码数据加载
```java
ClassReader(final String className) throws IOException
```
通过class全路径名从`ClassLoader`加载
<a name="UWKNQ"></a>
#### 2. `ClassVisitor`
java中类的访问者，提供一系列方法由`ClassReader`调用。调用的顺序如下：`visit` -> `visitSource` -> `visitModule` -> `visitNestHost` -> `visitOuterClass` -> `visitAnnotation` -> `visitTypeAnnotation` -> `visitAttribute` -> `visitNestMember` -> `visitPermittedSubclass` -> `visitInnerClass` -> `visitRecordComponent` -> `visitField` -> `visitMethod` -> `visitEnd`
<a name="hAmYF"></a>
#### 3. `ClassWriter`
`ClassVisitor`的子类，通过它生成最后的字节码。并且它可以帮助重新计算`MAXSTACK`和`MAXLOCALS`
<a name="ogx0Y"></a>
#### 4. `ModuleVisitor`
Java中模块的访问者，作为`ClassVisitor.visitModule`方法的返回值
<a name="d1Vvf"></a>
#### 5. `AnnotationVisitor`
Java中注解的访问者，作为ClassVisito中`visitTypeAnnotation`和`visitTypeAnnotation`的返回值
<a name="cI23j"></a>
#### 6. `FieldVisitor`
Java中字段的访问者，作为`ClassVisito.visitField`的返回值
<a name="OeC5X"></a>
#### 7. `MethodVisitor`
Java中方法的访问者，作为`ClassVisito.visitMethod`的返回值

- `visitMethodInsn` 方法调用指令
- `visitVarInsn` 局部变量调用指令
- `visitInsn(int)` 访问一个零参数要求的字节码指令，如LSUB
- `visitLdcInsn` 把一个常量放到栈顶
- `visitInvokeDynamicInsn` 动态方法调用
- `visitFieldInsn` 调用/访问某个字段
<a name="KI6YE"></a>
#### 8. `AnalyzerAdapter`
`MethodVisitor`的子类，使用它重新计算最大操作数栈(`MAXSTACK`)
<a name="s1iax"></a>
#### 9. `LocalVariablesSorter`
`MethodVisitor`的子类，使用它重新计算局部变量表(`MAXLOCALS`)的索引

- `newLocal` 创建局部变量

通过IDEA的Plugins安装ASM Bytecode Viewer Support Kotlin，可以借助这个插件来帮助生成大部分代码，具体用法这里就赘述了。<br />总结：有了以上知识基础，可以完成一个简单的demo来感受探针和字节码技术的强大。
<a name="N9k1y"></a>
### 一个计算函数执行时间的完整用例

1. 在IDEA中创建一个典型的maven工程
2. 编写pom文件
```xml
<?xml version="1.0" encoding="UTF-8"?>  
<project xmlns="http://maven.apache.org/POM/4.0.0"  
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
 xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
    <groupId>com.learnhow.study</groupId>
    <version>1.0</version>
      <packaging>jar</packaging>
    <artifactId>agent</artifactId>  
  
    <dependencies>  
        <dependency>  
            <groupId>org.ow2.asm</groupId>  
            <artifactId>asm</artifactId>  
            <version>9.2</version>
        </dependency>  
        <dependency>  
            <groupId>org.ow2.asm</groupId>  
            <artifactId>asm-commons</artifactId>
            <version>9.2</version>
        </dependency>  
    </dependencies>  
  
    <build>  
        <plugins>  
            <plugin>  
                <groupId>org.apache.maven.plugins</groupId>  
                <artifactId>maven-compiler-plugin</artifactId>  
                <version>3.8.1</version>  
                <configuration>  
                    <source>[your jdk version]</source>  
                    <target>[your jdk version]</target>  
                </configuration>  
            </plugin>  
            <plugin>  
                <groupId>org.apache.maven.plugins</groupId>  
                <artifactId>maven-assembly-plugin</artifactId>  
                <version>2.4</version>  
                <configuration>  
                    <descriptorRefs>  
                        <descriptorRef>jar-with-dependencies</descriptorRef>  
                    </descriptorRefs>  
                    <archive>  
                        <manifestEntries>  
                            <Premain-Class>[your package].PremainAgent</Premain-Class>
                            <Can-Redefine-Classes>true</Can-Redefine-Classes>  
                            <Can-Retransform-Classes>true</Can-Retransform-Classes>  
                        </manifestEntries>  
                    </archive>  
                </configuration>  
                <executions>  
                    <execution>  
                        <phase>package</phase>  
                        <goals>  
                            <goal>single</goal>  
                        </goals>  
                    </execution>  
                </executions>  
            </plugin>  
        </plugins>  
    </build>  
</project>
```
带`[]`的部分请换成本地环境。

3. `PremainAgent`类
```java
public class PremainAgent {  
    public static void premain(String agentArgs, Instrumentation inst) {  
        inst.addTransformer(new XClassFileTransformer());  
    }  
}
```

4. `XClassFileTransformer`类
```java
public class XClassFileTransformer implements ClassFileTransformer {
    @Override
    public byte[] transform(ClassLoader loader,
                            String className,
                            Class<?> classBeingRedefined,
                            ProtectionDomain protectionDomain,
                            byte[] classfileBuffer) throws IllegalClassFormatException {
        try {
            ClassReader cr = new ClassReader(classfileBuffer);
            ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
            cr.accept(new NanoTimerClassVisitor(cw), ClassReader.SKIP_DEBUG);
            byte[] cc = cw.toByteArray();
            return cc;
        } catch (IOException e) {

        }
        return null;
    }
}
```
`transform`方法返回null或者`new byte[0]`表示对当前字节码文件不进行修改。`ClassWriter.COMPUTE_MAXS`表示框架会自动计算`MAXSTACK`和`MAXLOCALS`，`ClassReader.SKIP_DEBUG`表示当字节码中包含调试信息的时候，会忽略不会触发回调。

5. `NanoTimerClassVisitor`类
```java
public class NanoTimerClassVisitor extends ClassVisitor {
    private String className;

    public NanoTimerClassVisitor(ClassVisitor classVisitor) {
        super(ASM9, classVisitor);
    }

    @Override
    public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
        this.className = name;
        super.visit(version, access, name, signature, superName, interfaces);
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String descriptor, String signature, String[] exceptions) {
        MethodVisitor mv = super.visitMethod(access, name, descriptor, signature, exceptions);
        if (Objects.nonNull(mv) && !name.equals("<init>") && !name.equals("<clinit>")) {
            NanoTimerMethodVisitor methodVisitor = new NanoTimerMethodVisitor(mv, className, access, name, descriptor);
            return methodVisitor.refactor();
        }
        return mv;
    }

    class NanoTimerMethodVisitor extends MethodVisitor {
        private AnalyzerAdapter analyzerAdapter;
        private LocalVariablesSorter localVariablesSorter;
        private int timeOpcode;
        private int outOpcode;
        private String className;
        private int methodAccess;
        private String methodName;
        private String methodDescriptor;

        public NanoTimerMethodVisitor(MethodVisitor methodVisitor, String className, int methodAccess,
                                 String methodName, String methodDescriptor) {
            super(ASM9, methodVisitor);
            this.className = className;
            this.methodAccess = methodAccess;
            this.methodName = methodName;
            this.methodDescriptor = methodDescriptor;
            // 使用AnalyzerAdapter计算最大操作数栈
            analyzerAdapter = new AnalyzerAdapter(className, methodAccess, methodName, methodDescriptor, this);
            // LocalVariablesSorter重新计算局部变量的索引并自动更新字节码中的索引引用
            localVariablesSorter = new LocalVariablesSorter(methodAccess, methodDescriptor, analyzerAdapter);
        }

        public MethodVisitor refactor() {
            return localVariablesSorter;
        }

        @Override
        public void visitCode() {
            super.visitCode();
            mv.visitMethodInsn(INVOKESTATIC, "java/lang/System", "nanoTime", "()J", false);
            timeOpcode = localVariablesSorter.newLocal(Type.LONG_TYPE);
            mv.visitVarInsn(LSTORE, timeOpcode);
        }

        @Override
        public void visitInsn(int opcode) {
            if ((opcode >= IRETURN && opcode <= RETURN) || opcode == ATHROW) {
                mv.visitMethodInsn(INVOKESTATIC, "java/lang/System", "nanoTime", "()J", false);
                mv.visitVarInsn(LLOAD, timeOpcode);
                mv.visitInsn(LSUB);
                mv.visitVarInsn(LSTORE, timeOpcode);

                mv.visitLdcInsn(className + "." + methodName + "(ns):");
                outOpcode = localVariablesSorter.newLocal(Type.getType(String.class));
                mv.visitVarInsn(ASTORE, outOpcode);

                mv.visitVarInsn(ALOAD, outOpcode);
                mv.visitVarInsn(LLOAD, timeOpcode);
                mv.visitInvokeDynamicInsn("makeConcatWithConstants", "(Ljava/lang/String;J)Ljava/lang/String;", new Handle(Opcodes.H_INVOKESTATIC, "java/lang/invoke/StringConcatFactory", "makeConcatWithConstants", "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;", false), new Object[]{"\u0001\u0001"});
                mv.visitVarInsn(ASTORE, outOpcode);

                mv.visitFieldInsn(GETSTATIC, "java/lang/System", "out", "Ljava/io/PrintStream;");
                mv.visitVarInsn(ALOAD, outOpcode);
                mv.visitMethodInsn(INVOKEVIRTUAL, "java/io/PrintStream", "println", "(Ljava/lang/String;)V", false);
            }
            super.visitInsn(opcode);
        }
    }
}
```

6. 通过`assembly`插件对项目进行打包生成：`agent-1.0-jar-with-dependencies.jar`
7. 运行一个目标项目，并添加虚拟机指令`-javaagent`，就可以看到执行效果
<a name="f1eAY"></a>
### 如何查看生成后的代码
计算函数执行时间是一个非常简单的功能，很容易一次性写正确。但是如果需要代理的逻辑比较复杂，而探针程序又不像普通程序一样方便做断点调试。如何才能够很方便知道生成的代码是否正确呢？这里告诉大家一个诀窍。回到`XClassFileTransformer`类，增加两行代码：
```java
public class XClassFileTransformer implements ClassFileTransformer {
    @Override
    public byte[] transform(ClassLoader loader,
                            String className,
                            Class<?> classBeingRedefined,
                            ProtectionDomain protectionDomain,
                            byte[] classfileBuffer) throws IllegalClassFormatException {
        try {
            ClassReader cr = new ClassReader(classfileBuffer);
            ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
            cr.accept(new NanoTimerClassVisitor(cw), ClassReader.SKIP_DEBUG);
            byte[] cc = cw.toByteArray();
            FileOutputStream fos = new FileOutputStream("./cc.class");
            fos.write(cc);
            return cc;
        } catch (IOException e) {

        }
        return null;
    }
}
```
第13、14行代码的功能是将生成的字节码输出到本地文件中，然后通过IDEA打开这个.class文件，看看新增加的代码是否如预期的那样。<br />总结：JVM代理发生在类加载器加载.class文件前，因此能够动态修改字节码。通过ASM这类字节码框架，使得开发人员即使对字节码指令不是很熟悉依然能够操作。当然，Java的探针技术除了和被代理的项目同时启动以外还提供了一种热部署的方案。
