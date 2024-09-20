JavaAgent
<a name="OJKuO"></a>
## 1、什么是java agent
Java Agent又叫做Java探针，是在JDK1.5引入的一种可以动态修改Java字节码的技术，是依附于java应用程序（JVM）Instrumentation API与虚拟机交互。<br />Java类编译之后形成字节码被JVM执行，在JVM在执行这些字节码之前获取这些字节码信息，并且通过字节码转换器对这些字节码进行修改，来完成一些额外的功能<br />底层的具体由JVMTI机制实现。<br />JVM tool interface（JVMTI）是供工具使用的本机编程接口。它提供了一种检查状态和控制Java虚拟机（JVM）中运行的应用程序执行的方法。JVMTI支持需要访问JVM状态的各种工具，包括但不限于：分析、调试、监视、线程分析和覆盖率分析工具。
<a name="ZIPzI"></a>
## 2、使用场景
java agent技术的一些常用使用场景：

- 对class文件加密
- 实现应用性能监控(APM)
- JAVA程序的调试
- 热加载
- 启动方式

在java主程序中，通过-javaagent指定代理jar包，来实现程序代理。**-javaagent:jarpath[=**options]<br />jarpath是代理JAR文件的路径。options是代理选项。此开关可以在同一命令行上多次使用，从而创建多个代理。多个代理可以使用同一jarpath。代理JAR文件必须符合JAR文件规范。
<a name="ImSQj"></a>
## 3、启动方式
挂载agent的方式主要包含静态启动、动态启动两种方式。
<a name="itRdX"></a>
### 3.1 静态启动（Agent模式）
当在JVM启动时，来指定代理。需要实现的方法`premain()`
```java
//优先执行
public static void premain(String agentArgs, Instrumentation inst)
  
public static void premain(String agentArgs)
```
启动agent
```bash
-javaagent:/data/gitlab/test-demo/agent/target/agent.jar -jar example-1.0-SNAPSHOT.jar
```
<a name="HfJYE"></a>
### 3.2 动态启动（Attach模式）
当在JVM启动一段时间后，来指定代理。基于动态attach模式，需要实现方法 agentmain。
```java
//优先执行
public static void agentmain(String agentArgs, Instrumentation inst);
public static void agentmain(String agentArgs)
```
需要通过VirtualMachine将加载agent的jar，插装到需要代理的JVM进程中，来实现代理操作。
```java
public class AttachAgent {
    public static void main(String[] args) {
        String pid = args[0];
        String agentJar = args[1];
        VirtualMachine virtualMachine = VirtualMachine.attach(pid);
        virtualMachine.loadAgent(agentJar);
        virtualMachine.detach();
    }
}
```
启动agent命令，因为要依赖java toos.jar，需要显示指定classpath。具体做法如下所示：
```bash
java -classpath .:$CLASS_PATH:/data/gitlab/test-demo/agent/target/agent.jar com.vhicool.demo.AttachAgent  65887 /data/gitlab/test-demo/agent/target/agent.jar
```
命令的具体参数说明如下：

- agentArgs：java agent启动指定的传入的参数，如：`java -agentlib:/data/agent.jar=opt1,opt2`
- inst：它提供了向现有编译的Java类添加字节码的能力

编写完AttachAgent，还不能直接被jvm加载，首先需要在 在resources/META-INF/MANIFREST.MF文件加入如下配置
```
Manifest-Version: 1.0 
Premain-Class: com.vhicoo.HelloAgent
Agent-Class: com.vhicoo.HelloAgent
Can-Redefine-Classes: true 
Can-Retransform-Classes: true 
Build-Jdk-Spec: 1.8 
Created-By: Maven Jar Plugin 3.2.0
```
然后要在maven中定义agent配置:
```xml
<build> 
  <finalName>HelloAgent</finalName> 
  <plugins> 
    <plugin> 
      <groupId>org.apache.maven.plugins</groupId> 
      <artifactId>maven-jar-plugin</artifactId> 
      <version>3.2.0</version> 
      <configuration> 
        <archive> 
          <!--META-INF/MANIFEST.MF --> 
          <manifest> 
            <addClasspath>true</addClasspath> 
          </manifest> 
          <manifestEntries> 
            <Premain-Class>com.vhicoo.HelloAgent</Premain-Class> 
            <Agent-Class>com.vhicoo.HelloAgent</Agent-Class> 
            <Can-Redefine-Classes>true</Can-Redefine-Classes> 
            <Can-Retransform-Classes>true</Can-Retransform-Classes> 
          </manifestEntries> 
        </archive> 
      </configuration> 
    </plugin> 
  </plugins> 
</build>
```
对这些配置项说明如下：

- Premain-Class：包含premain方法的类名，当在JVM静态启动时指定代理时，需要此属性
- Agent-Class：包含agentmain方法的类。当在JVM动态启动时指定代理时，需要此属性
- Boot-Class-Path：引导类加载器要搜索的路径列表。路径表示目录或库（在许多平台上通常称为JAR或zip库）。在查找类的平台特定机制失败后，引导类加载器会搜索这些路径。列表中的路径由一个或多个空格分隔。路径采用层次URI的路径组件的语法。如果路径以斜杠字符（“/”）开头，则路径为绝对路径，否则路径为相对路径。根据代理JAR文件的绝对路径解析相对路径。忽略格式不正确和不存在的路径。当在VM启动后某个时间启动代理时，将忽略不表示JAR文件的路径。此属性是可选的。
- Can-Redefine-Classes：重新定义此代理所需的类的能力。返回：true/false，默认false
- Can-Retransform-Classes：是否能够重新转换此代理所需的类。返回：true/false，默认false
- Can-Set-Native-Method-Prefix：是否设置此代理所需的本机方法前缀的能力。返回：true/false，默认false

接下来再来看一下java agent技术中一个非常重要的类：Instrumentation，是实现动态修改字节码的主力军，对其中非常重要的方法一一介绍下作用。

- transformer 所有满足ClassFileTransformers的类，在初始化过程中，都会调用转换器（除了任何注册的转换器所依赖的类的定义）。转换器可以修改目标类的字节码，从而达到重新定义类功能的能力。对于已经加载的类，需要借助retransform来实现重新触发类转换
- retransform 注册的Transformer会在类初始化过程中，一个一个去将满足条件的类进行转换。retransfrom是JVM重复执行这个过程的能力。`Instrumentation`的`retransformClasses`方法只提供类，不通过类字节码，它是通过将已经注册的具有转换能力的`ClassFileTransformers`，提供实际字节码，来达到修改目标类的能力。使用场景是在目标类已经加载（如agent attach模式），对目标类进行修改。
- redefine 代理类可以在任何时候，通过指定目标类和类字节码，将来达到修改目标类，redefineClasses更改现有（和已加载）类的实际定义
<a name="b8Mfa"></a>
## 4、卸载Agent
在javaagent不管是通过agent模式还是通过attach模式，都将本身agent代码也加载到目标JVM中。<br />在不需要对目标JVM代理，想要使目标class恢复到代理之前应该如果去操作呢？<br />首先卸载javaagent包括如下途径。当然最快的卸载agent方式是直接将目标JVM进程重启，但是这种方式对于高可用业务场景，可能会带来风险。<br />通常有如下两种方式进行类的卸载。
<a name="gqCya"></a>
### 4.1 卸载`Transform`
第一种是卸载`Transform`。<br />如果是通过定义`Transformer`来实现javaagent，那么可以使用`retransform`将`Transformer`移除，并且将被代理class恢复。<br />具体的实施步骤如下：

1. 在premain或者`agentmain`中，注册`Transformer`
```java
HelloTransformer helloTransformer = new HelloTransformer();
inst.addTransformer(helloTransformer, true);
```

1. 触发retransform使类重新加载，从而修改目标class字节码，生成新的被代理class。其中clazz变量是被代理类对象
```java
inst.retransformClasses(clazz);
```

1. 通过`removeTransformer`移除已经注册的`Transformer`，并且再次触发`retransform`，使被代理class字节码还原到代理之前。其中`helloTransformer`必须是第一步定义的`Transformer`对象引用。
```java
inst.removeTransformer(helloTransformer);
inst.retransformClasses(clazz);
```
<a name="J5Y3r"></a>
### 4.2 通过热部署能力实现还原目标代理类
通过redefine，直接将代码进行重新加载，达到类覆盖的目的。实现方式为`Instrumentation#redefineClasses(ClassDefinition classDefinition)`方法，通过提供的类字节码，重新定义类。<br />这种方法不仅能够还原被代理的类，同时也能实现代理依赖的类，根据指定的类直接吗，重新加载为新的class，这样就可以实现agent.jar的热部署。
<a name="K61VA"></a>
## 5、完整Demo示范
<a name="Ss8nn"></a>
### 5.1 定义Agent代理类：HelloAgent
通过`mainAgent`代理类，实现类扩展功能，默认只能生效修改还未加载的类。<br />但是一般在对程序类进行代理时，大多已经运行一段时间，处理代码已经完成加载。在这个时候，需要将已经加载的类进行替换，在attach到目标进程后，通过调用`Instrumentation#redefineClasses`让jvm重新加载指定类，这样就完成已加载类型字节码修改。
```java
package com.vhicool.demo.agent;
import java.lang.instrument.Instrumentation;

/**
 * <p> agent代理类入口，同时支持静态、动态启动 </p>
 **/
public class HelloAgent {

    public static void premain(String agentArgs, Instrumentation inst){
        inst.addTransformer(new HelloTransformer(),true);
        System.out.println("Premain HelloAgent success");
    }
    public static void agentmain(String agentArgs, Instrumentation inst) {
        inst.addTransformer(new HelloTransformer());
        for (String clazz : HelloTransformer.INJECTED_CLASS) {
            System.out.println("Retransform classes : " + clazz);
            inst.retransformClasses(Class.forName(clazz));
        }
        System.out.println("Attach HelloAgent success");
    }
}
```
<a name="lBaUW"></a>
### 5.2 定义类转换器：Transformer
```java
import javassist.*;

import java.io.IOException;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.security.ProtectionDomain;
import java.util.ArrayList;
import java.util.List;

/**
 * <p> 定义转换器，在代理的方法执行前，输出日志 </p>
 **/
public class HelloTransformer implements ClassFileTransformer {

    public static final List<String> INJECTED_CLASS = new ArrayList<String>();

    static {
        //被代理的类
        INJECTED_CLASS.add("com.vhicool.test.example.ProcessTest");
    }

    @Override
    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined, ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {
        String realClassName = className.replace("/", ".");
        try {
            if (INJECTED_CLASS.contains(realClassName)) {
                System.out.println("Agent class : " + realClassName);
                ClassPool classPool = ClassPool.getDefault();
                CtClass ctClass = classPool.get(realClassName);
                CtMethod[] declaredMethods = ctClass.getDeclaredMethods();

                for (CtMethod ctMethod : declaredMethods) {
                    String method = ctMethod.getName();
                    ctMethod.insertBefore(String.format("System.out.println(\"Process method :  + %s\");", method));
                }
                return ctClass.toBytecode();
            }
        } catch (NotFoundException | CannotCompileException | IOException e) {
            e.printStackTrace();
        }
        return classfileBuffer;
    }

}
```
<a name="kKGR9"></a>
### 5.3 定义Agent注入入口
这一步，只有动态启动(attatch)模式需要，静态启动不需要定义此类。
```java
import com.sun.tools.attach.AgentInitializationException;
import com.sun.tools.attach.AgentLoadException;
import com.sun.tools.attach.AttachNotSupportedException;
import com.sun.tools.attach.VirtualMachine;

import java.io.IOException;

/**
 * <p> 这个类是将代理程序插入到目标代理程序进程 </p>
 **/
public class AttachAgent {

    public static void main(String[] args) throws IOException, AttachNotSupportedException, AgentLoadException, AgentInitializationException {
        String pid = args[0];
        String jvmJar = args.length > 1 ? args[1] : System.getProperty("user.dir") + "/agent.jar";
        System.out.println("Agent :" + jvmJar + " ,attach Pid :" + pid);
        VirtualMachine virtualMachine = VirtualMachine.attach(pid);
        virtualMachine.loadAgent(jvmJar);
        virtualMachine.detach();
    }
}
```
<a name="Oj6if"></a>
### 5.4 pom依赖
javassist是一个用来 处理 Java 字节码的类库。它可以在一个已经编译好的类中添加新的方法，或者是修改已有的方法。<br />maven-shade-plugin会将项目依赖打到jar中
```xml
<dependencies>
  <dependency>
    <groupId>org.javassist</groupId>
    <artifactId>javassist</artifactId>
    <version>3.28.0-GA</version>
  </dependency>
  <dependency>
    <groupId>jdk.tools</groupId>
    <artifactId>jdk.tools</artifactId>
    <version>1.8</version>
    <scope>system</scope>
    <systemPath>${env.JAVA_HOME}/lib/tools.jar</systemPath>
  </dependency>
</dependencies>
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-compiler-plugin</artifactId>
      <version>3.5.1</version>
      <configuration>
        <source>8</source>
        <target>8</target>
      </configuration>
    </plugin>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-jar-plugin</artifactId>
      <version>3.2.0</version>
      <configuration>
        <archive>
          <!--META-INF/MANIFEST.MF -->
          <manifest>
            <addClasspath>true</addClasspath>
          </manifest>
          <manifestEntries>
            <Agent-Class>com.vhicool.demo.agent.HelloAgent</Agent-Class>
            <Premain-Class>com.vhicool.demo.agent.HelloAgent</Premain-Class>
            <Can-Redefine-Classes>true</Can-Redefine-Classes>
            <Can-Retransform-Classes>true</Can-Retransform-Classes>
          </manifestEntries>
        </archive>
      </configuration>
    </plugin>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-shade-plugin</artifactId>
      <executions>
        <execution>
          <phase>package</phase>
          <goals>
            <goal>shade</goal>
          </goals>
          <configuration combine.self="override">
            <createDependencyReducedPom>false</createDependencyReducedPom>
            <shadedArtifactAttached>false</shadedArtifactAttached>
            <finalName>agent</finalName>
            <transformers>
              <transformer
                <resource>reference.conf</resource>
              </transformer>
              <transformer implementation="org.apache.maven.plugins.shade.resource.ServicesResourceTransformer"/>
              <transformer implementation="org.apache.maven.plugins.shade.resource.ApacheNoticeResourceTransformer">
                <projectName>agent</projectName>
                <encoding>UTF-8</encoding>
              </transformer>
              </transformers>
              </configuration>
              </execution>
              </executions>
              </plugin>
              </plugins>
              </build>
```
<a name="PFobd"></a>
### 5.5 定义一个需要被注入的程序
```java
public class HelloMain {
    public static void main(String[] args) throws InterruptedException {
        ProcessTest helloMain = new ProcessTest();
        for (int i = 0; ; i++) {
            helloMain.process(i + "");
            Thread.sleep(1000);
        }
    }
}

public class ProcessTest {
    public void process(String s) {
        System.out.println("输出 ：" + s);
    }
}
```
<a name="SoOPd"></a>
### 5.6 分别使用两种方式启动

- javaagent（静态启动方式） 静态启动是随着java主程序一起启动，伴随jvm的生命周期
```bash
java -javaagent:/data/gitlab/test-demo/agent/target/agent.jar -jar example-1.0-SNAPSHOT.jar
```
输出：
```
Process method :  + process
输出 ：0
Process method :  + process
输出 ：1
Process method :  + process
输出 ：2
```

- javaagent（动态启动方式）

启动主程序，此时程序正常输出结果
```bash
java -jar example.jar
```
其中需要先查看example.jar的pid。`com.vhicool.demo.AttachAgent`是attach模式启动程序，agent.jar 是代理程序
```bash
java -classpath .:$CLASS_PATH:/data/gitlab/test-demo/agent/target/agent.jar com.vhicool.demo.AttachAgent  57092 /data/gitlab/test-demo/agent/target/agent.jar
```
输出结果如下所示：
```
输出 ：39
Transform classes : com.vhicool.test.example.ProcessTest
Agent class : com.vhicool.test.example.ProcessTest
Attach HelloAgent success
Process method :  + process
输出 ：40
Process method :  + process
输出 ：41
```
