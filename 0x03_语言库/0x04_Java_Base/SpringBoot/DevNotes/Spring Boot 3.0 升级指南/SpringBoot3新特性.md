JavaSpringBoot<br />Spring Boot 已经正式发布了 3.0 的最终 RELEASE 版本，现在一起看下 Spring Boot 3.0 将会要哪些重大的变化。
<a name="yT5sx"></a>
## JAVA 17
比较关注的第一个最重要的事情就是，Spring Boot3 版本最低支持 Java17，还在万年 Java8 的同学，该升级就升级了吧，这里介绍一下关于 Java17 之后的一些重要改变。
<a name="rWWTL"></a>
### record
record 是在 Java14中引入的，[https://openjdk.org/jeps/395](https://openjdk.org/jeps/395) 。<br />以前写一个类需要写一堆 get、set 方法，后来有了lombok之后这些都省了，现在 Java 提供原生的写法。
```java
public record User() {}
```
本质上 record 修饰之后的类就是一个 `final` 类，而且他的父类不是 Object，也不是余某军，而是 `java.lang.Record`。<br />record 类属性只能声明在头部，所有的成员变量都是 `public final` 的，而且只能声明静态属性，但是可以声明成员方法和静态方法。
```java
public record User(String username) {
    static int id;
    public String getName(){
        return this.username;
    }
}
```
<a name="LZTn2"></a>
### text blocks
text blocks 文本块是在 Java13 引入的，并且在 Java15 版本成为永久特性，[https://openjdk.org/jeps/378](https://openjdk.org/jeps/378)。<br />以前复制一个多行的字符串到 Java 里，会自动加上行终止符。
```java
String sql = "SELECT\n" +
                "\t* \n" +
                "FROM\n" +
                "\tsys_user0 \n" +
                "WHERE\n" +
                "\tuser_name = 'abc'";
```
而有了文本块的功能之后，可以更方便的定义包含多行文本的字符串字面量，他使用三引号作为开始和结束的分隔符。
```java
String sqlBlock = """
                SELECT
                    *
                FROM
                    sys_user0
                WHERE
                    user_name = 'abc'
                """;
```
<a name="kiAqf"></a>
### switch表达式
switch表达式是在 Java12 中引入的，在 Java14 成为永久特性，[https://openjdk.org/jeps/361](https://openjdk.org/jeps/361)。<br />升级后的 switch 其实包含两个特性，一个是允许 case 使用多个常量，另外一个就是有返回值。<br />新增`case x->` 语法，使用方面更加简洁，而且不需要再每个 case 写一个 break了。
```java
String name = "xiao";
int ret = switch (name) {
    case "ai" -> 1;
    case "xiao", "xian" -> 2;
    default -> 0;
};
```
<a name="HiNZ5"></a>
### Pattern Matching 模式匹配
模式匹配可以简化`instanceof`代码。
```java
if (obj instanceof String s) {
    System.out.println(s.toLowerCase());
}
```
还可以在 switch-case 语句使用：
```java
static double getDoubleUsingSwitch(Object o) {
    return switch (o) {
        case Integer i -> i.doubleValue();
        case Float f -> f.doubleValue();
        case String s -> Double.parseDouble(s);
        default -> 0d;
    };
}
```
<a name="OLfHA"></a>
### sealed 密封类
sealed 在 Java15中引入，在 Java17成为永久特性。<br />sealed 密封类的主要作用就是限制类的继承。<br />比如有 Animal类，Dog 和 Cat 分别继承它，实现了 eat 方法，他们吃的动作是不一样的，但是不希望人能继承 Animal，不允许他去继承动物吃的行为，就可以像下面这样通过 sealed 和 permits 关键字限制它是一个密封类，只有猫和狗能够继承它。<br />需要注意，父类被定义为 sealed 之后，子类必须是 sealed、 non-sealed 或者 final。
```java
public abstract sealed class Animal permits Cat, Dog {

    public abstract void eat();
}

public non-sealed class Dog extends Animal{
    @Override
    public void eat() {
        System.out.println("dog eat");
    }
}

public non-sealed class Cat extends Animal{
    @Override
    public void eat() {
        System.out.println("cat eat");
    }
}
```
<a name="YWfzv"></a>
## Jakarta EE 9
另外一个很重要的变化就是本次升级之后，最低只支持 Jakarta EE 9，使用 Servlet5.0 和 JPA3.0 规范，不过最新版本RC2已经升级到了 JakartaEE 10，默认使用 Servlet6.0 和 JPA3.1 规范。<br />有些同学可能连 Jakarta 是什么都不知道，这个英文单词是印尼首都雅加达的意思，其实就是 JavaEE 改名之后就叫 JakartaEE，比如之前的`javax.servlet`包现在就叫`jakarta.servlet`。<br />也因此，代码中所有使用到比如 _HttpServletRequest_ 对象的 import 都需要修改。
```java
import javax.servlet.http.HttpServletRequest;
// 改为
import jakarta.servlet.http.HttpServletRequest;
```
<a name="JSFO1"></a>
## Spring Native
Spring Native 也是升级的一个重大特性，支持使用 GraalVM 将 Spring 的应用程序编译成本地可执行的镜像文件，可以显著提升启动速度、峰值性能以及减少内存使用。<br />传统的应用都是编译成字节码，然后通过 JVM 解释并最终编译成机器码来运行，而 Spring Native 则是通过 AOT 提前编译为机器码，在运行时直接静态编译成可执行文件，不依赖 JVM。
<a name="qr30a"></a>
### 演示
这里简单演示一下怎么使用，首先需要做一些准备工作：

1. [https://www.graalvm.org/downloads/](https://www.graalvm.org/downloads/) 下载GraalVM，指定JAVA_HOME，`export JAVA_HOME=/Users/user/Desktop/graalvm-ce-java17-22.3.0/Contents/Home`
2. 下载个新一点的 IDEA，比如使用的是最新的 EAP 版本
3. 下载个新一点的 JDK，比如使用 JDK17

然后通过 Spring Initialzr 创建一个新项目，使用最新版本Spring Boot 3.0.0-SNAPSHOT，勾选GraalVM Native Support，创建好项目之后添加一个测试的Controller。
```java
@RestController
public class TestController {

    @GetMapping("/")
    public String hello(){
        return "GraalVM ...";
    }
}
```
然后直接运行程序，发现启动时间花费了大概 1 秒。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669596581766-fc897d85-6a86-400c-a8e3-92ef63436f9a.jpeg#averageHue=%231e1e21&clientId=ue6e0b247-c08c-4&from=paste&id=u2c652839&originHeight=318&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubd67ca3f-d1ce-47af-9d4a-f3343b054de&title=)<br />然后执行命令，生成镜像文件：
```bash
./gradlew nativeCompile
```
这个过程挺耗时的，花了大概 2 分多钟才生成好。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669596581748-25caad0e-0660-4e39-b859-d311c2ba76a3.jpeg#averageHue=%230a2830&clientId=ue6e0b247-c08c-4&from=paste&id=u45cc33b4&originHeight=684&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9859026d-ad6e-4754-9486-43225947310&title=)<br />最后执行命令：
```bash
./build/native/nativeCompile/demo2
```
可以看到，最终启动时间是 0.082 秒，快了 10 多倍。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669596581765-49a205cb-6a26-4c20-80af-c15dc9bda5a6.jpeg#averageHue=%230a282f&clientId=ue6e0b247-c08c-4&from=paste&id=u308a07a2&originHeight=684&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u73f4228c-31ec-40fc-b25a-24c59a70004&title=)<br />这里使用的是 gradle，如果使用 maven 的话，使用如下的命令：
```bash
mvnw -Pnative native:compile
./target/demo2
```
<a name="aJAf8"></a>
## 其他依赖升级
Spring Boot 3 最低依赖 Spring6 版本，因此对应的 Spring 版本也该换了（不会有人还在用 Spring2 的吧），其他的依赖升级如下：

- Kotlin 1.7+
- Lombok 1.18.22+ (JDK17 support支持版本）
- Gradle 7.3+

另外，SpringBoot2.7引入了新的自动装配方式`META-INF/spring/org.springframework.boot.autoconfigure.AutoConfiguration.imports`，原来的写法spring.factories在 3.0 版本以下还能兼容，3.0 新版本之后，老的写法spring.factories不能使用了，中间件相关的开发同学要注意了。<br />其他一些关于配置的变化，Spring MVC 的一些小变化就不说了，更新日志到时候都可以看到。<br />最后，如果想升级的话，在新版本发布之后，会有一个基于Spring Boot 2.7 版本的迁移指南。
