JavaDubbo<br />这里从为什么需要自适应扩展的提问引出自己如何实现以及推理 Dubbo 如何实现，这些铺垫可以帮助读者更好的理解后文对 Dubbo 自适应扩展源码的解读。<br />很多人在学习SPI的时候将`@SPI`和`@Adaptive`注解混在一起学习，最后学得晕晕乎乎看完之后似懂非懂，如果你也有这种困扰，请继续阅读。<br />并不是说不该将这两个内容一起学习，而是要有个先后顺序再加上自己的推理。是先有SPI机制，然后才有的自适应扩展，自适应扩展是基于SPI机制的高级特性。<br />[![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1673528786310-21e78f6e-abc4-4655-91c3-f266d390c2f5.jpeg#averageHue=%23eff1eb&clientId=u7bfd1ffe-e477-4&from=paste&id=u6251fb26&originHeight=528&originWidth=529&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua80717da-9f6a-4257-98eb-dff3326a8b9&title=)](https://mp.weixin.qq.com/s?__biz=MzI3MTUyMTY2Mw==&mid=2247488954&idx=1&sn=abf906d8cfe3fa24599b1895162d462a&chksm=eac1d7aaddb65ebc6f7b5f19d5ded5eec1027f0c64b41c17fbb766fdc11bc44c49156b6863b6&scene=21#wechat_redirect)

<a name="HcDAX"></a>
## 为什么需要自适应扩展点
在 Dubbo 中，很多拓展都是通过 SPI 机制进行加载的，比如 Protocol、Cluster、LoadBalance 等。有时，有些拓展并不想在框架启动阶段被加载，而是希望在拓展方法被调用时，根据运行时参数进行加载。这听起来有些矛盾。拓展未被加载，那么拓展方法就无法被调用（静态方法除外）。拓展方法未被调用，拓展就无法被加载。对于这个矛盾的问题，Dubbo 通过自适应拓展机制很好的解决了。<br />对于这个问题，以之前demo为例进行进行推演：
```java
public interface MySPI {
    void say();
}
public class HelloMySPI implements MySPI{
    @Override
    public void say() {
        System.out.println("HelloMySPI say:hello");
    }
}
public class GoodbyeMySPI implements MySPI {
    @Override
    public void say() {
        System.out.println("GoodbyeMySPI say:Goodbye");
    }
}
```
现在要增加一个接口 Person，他可以和人打招呼。他有一个实现类是Man，他可以动态的跟人说hello或者goodbye。
```java
public interface Person {

    void greeting();
}

public class Man implements Person{
    private MySPI adaptive;

    public void setAdaptive(MySPI adaptive) {
        this.adaptive = adaptive;
    }

    @Override
    public void greeting(URL url) {
        adaptive.say(url);
    }

}
```
但是adaptive成员要么是HelloMySPI的实例化对象，要么是GoodbyeMySPI的实例化对象，怎么实现动态的去根据需要获取呢？解决这个问题就可以增加一个代理，作为自适应类。所以增加自适应扩展实现如下：
```java
public class AdaptiveMySPI implements MySPI {

    @Override
    public void say() {
        // 1.通过某种方式获得拓展实现的名称
        String mySpiName;

        // 2.通过 SPI 加载具体的 mySpi
        MySPI myspi = ExtensionLoader.getExtensionLoader(MySPI.class).getExtension(mySpiName);

        // 3.调用目标方法
        myspi.say();
    }
}
```
将代理类AdaptiveMySPI作为Man的成员对象，这样就可以实现按需调用了。按需加载如何实现呢？之前在`getExtension()`方法中提到过，只要在根据名字查找的时候，才会按照需要懒加载，所以这个问题天然被Dubbo SPI 解决了。那么剩下的关键就是如何按需调用，也就是如何获得名字。

1. 可以在当前线程的上下文中获得，比如通过ThreadLocal保存。
2. 可以通过接口参数传递，但是这样就需要实现自适应扩展的接口按照约定去定义参数，否则就无法拿到名字，这样对于被代理的接口是有一定限制的。

Dubbo 用的是第二种方式，也就是他总有办法从参数中动态拿到扩展类名。
<a name="mT8FL"></a>
## 模拟原理 demo
再具体一些 Dubbo 是怎么实现的呢？<br />自适应拓展机制的实现逻辑比较复杂，首先 Dubbo 会为拓展接口生成具有代理功能的代码。然后通过 javassist 或 jdk 编译这段代码，得到 Class 类。最后再通过反射创建代理类，整个过程比较复杂。为了让大家对自适应拓展有一个感性的认识，按照之前的知识，下面继续对之前demo为例进行改造：
```java
@SPI
public interface MySPI {
    void say(URL url);
}
public class HelloMySPI implements MySPI{
    @Override
    public void say(URL url) {
        System.out.println("HelloMySPI say:hello");
    }
}
public class GoodbyeMySPI implements MySPI {
    @Override
    public void say(URL url) {
        System.out.println("GoodbyeMySPI say:Goodbye");
    }
}
public class AdaptiveMySPI implements MySPI {

    @Override
    public void say(URL url) {
        if (url == null) {
            throw new IllegalArgumentException("url == null");
        }

        // 1.从 URL 中获取 mySpi 名称
        String mySpiName = url.getParameter("myspi.type");
        if (mySpiName == null) {
            throw new IllegalArgumentException("MySPI == null");
        }

        // 2.通过 SPI 加载具体的 mySpi
        MySPI myspi = ExtensionLoader.getExtensionLoader(MySPI.class).getExtension(mySpiName);

        // 3.调用目标方法
        myspi.say(url);
    }
}
@SPI("man")
public interface Person {

    void greeting(URL url);
}
public class Man implements Person {
    private MySPI adaptive = = ExtensionLoader.getExtensionLoader(MySPI.class).getExtension(“adaptive”);

    public void setAdaptive(MySPI adaptive) {
        this.adaptive = adaptive;
    }

    @Override
    public void greeting(URL url) {
        adaptive.say(url);
    }
}
public static void main(String[] args) {
        ExtensionLoader<Person> extensionLoader = ExtensionLoader.getExtensionLoader(Person.class);
        Person hello = extensionLoader.getExtension("man");
        hello.greeting(URL.valueOf("dubbo://192.168.0.101:100?myspi.type=hello"));
        hello.greeting(URL.valueOf("dubbo://192.168.0.101:100?myspi.type=goodbye"));
    }
//输出
HelloMySPI say:hello
GoodbyeMySPI say:Goodbye
```
大家与之前的代码对比就可以发现区别，MySPI 的方法增加了 URL 参数，因为 dubbo 中url就是作为一个配置总线贯穿整个调用链路的。这样便可以拿到扩展名，动态调用和加载了。<br />比如demo中的一个URL
```
dubbo://192.168.0.101:100?myspi.type=hello
```
AdaptiveMySPI 动态的人从 url中拿到了myspi.type=hello，然后根据name 拿到了扩展实现，以此完成动态调用。<br />上面的示例展示了自适应拓展类的核心实现 —- 在拓展接口的方法被调用时，通过 SPI 加载具体的拓展实现类，并调用拓展对象的同名方法。所以接下来的关键就在于，自适应拓展类是如何生成的，Dubbo 是怎么做的。
<a name="aJQZM"></a>
## `@Adaptive` 注解
关于Dubbo的自适应扩展，一定避不开这个关键注解`@Adaptive`。
```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD})
public @interface Adaptive {
    String[] value() default {};
}
```
从上面的代码中可知，Adaptive 可注解在类或方法上。

- 当 Adaptive 注解在类上时，Dubbo 不会为该类生成代理类。Adaptive 注解在类上的情况很少，在 Dubbo 中，仅有两个类被 `Adaptive` 注解了，分别是 `AdaptiveCompiler` 和 `AdaptiveExtensionFactory`。此种情况，表示拓展的加载逻辑由人工编码完成。
- 注解在方法（接口方法）上时，Dubbo 则会为该方法生成代理逻辑。更多时候，Adaptive 是注解在接口方法上的，表示拓展的加载逻辑需由框架自动生成。

Adaptive 注解的地方不同，相应的处理逻辑也是不同的。注解在类上时，处理逻辑比较简单，本文就不分析了。注解在接口方法上时，处理逻辑较为复杂，本章将会重点分析此块逻辑。<br />有没有想过为什么可以设计加载在类上？<br />按照上述的推理，实现自适应扩展总需要按照一定的约束和规范去设计方法参数，这样才能拿到参数去动态的加载扩展类。如果就是没有按照规范去设计参数呢？那就需要自己去想办法实现了，无法生成代理类，这就是加载在类上的原因。
<a name="Q0TX7"></a>
## 源码解读
<a name="kh0z5"></a>
### 获得自适应类
在 Dubbo 中如何获得一个自适应扩展类呢？只需要一行代码。
```java
MySPI adaptive = ExtensionLoader.getExtensionLoader(MySPI.class).getAdaptiveExtension();
@SPI
public interface MySPI {

    @Adaptive(value = {"myspi.type"})
    void say(URL url);
}
```
`getExtensionLoader()`之前分析过了，这里直接从`getAdaptiveExtension()`开始。<br />`getAdaptiveExtension()` 方法是获取自适应拓展的入口方法，首先会检查缓存`cachedAdaptiveInstance`，缓存未命中，则会执行双重检查，调用 `createAdaptiveExtension()` 方法创建自适应拓展。
```java
public T getAdaptiveExtension() {
    // 从缓存中查找自适应扩展类实例，命中直接返回
    Object instance = cachedAdaptiveInstance.get();
   //缓存没有命中
    if (instance == null) {
            …… 异常处理
        //双重检查
        synchronized (cachedAdaptiveInstance) {
            instance = cachedAdaptiveInstance.get();
            if (instance == null) {
                try {
                    // 创建自适应实例
                    instance = createAdaptiveExtension();
                    cachedAdaptiveInstance.set(instance);
                } catch (Throwable t) {
                   …… 异常处理
    return (T) instance;
}
```
`createAdaptiveExtension()`会首先查找只适应扩展类，然后通过反射进行实例化，再调用`injectExtension`对扩展实例中注入依赖。<br />你可能会问直接返回依赖不就行了？为什么还需要注入？<br />这是因为任何利用Dubbo SPI 机制加载的用户创建类都是有可能有成员依赖于其他拓展类的，用户实现的自适应扩展类也不例外。而另一种Dubbo 自己生成的自适应扩展类则不可能出现依赖其他类的情况。<br />这里只关注重点方法`getAdaptiveExtensionClass()`。

1. 首先`getExtensionClasses()`会获取该接口所有的拓展类，
2. 然后会检查缓存是否为空，`cachedAdaptiveClass`缓存着自适应扩展类的类型。
3. 如果缓存中不存在则调用`createAdaptiveExtensionClass`开始创建自适应扩展类。
```java
// 用于缓存自适应扩展类的类型
private volatile Class<?> cachedAdaptiveClass = null;
private Class<?> getAdaptiveExtensionClass() {
    // 加载所有的拓展类配置
    getExtensionClasses();
    if (cachedAdaptiveClass != null) {
        return cachedAdaptiveClass;
    }
    // 创建自适应拓展类
    return cachedAdaptiveClass = createAdaptiveExtensionClass();
}
```
`getExtensionClasses`之前讲过，你肯定会奇怪为什么还需要加载所有扩展类。在这里有个关键逻辑，在调用 loadResource 方法时候会解析`@Adaptive` 注解，如果被标注了，就表示这个类是一个自适应扩展类实现，会被设置到缓存`cacheAdaptiveClass`中。<br />所以有两个原因：<br />1、是自定义自适应扩展类需要SPI机制加载<br />2、是设置缓存
<a name="qv7fL"></a>
### 生成自适应扩展类
非自己实现的自适应扩展类，都要走`createAdaptiveExtensionClass`逻辑。<br />主要逻辑如下：

1. 动态生成自适应扩展类代码
2. 获取类加载器和编译器类（Dubbo 默认使用 javassist 作为编译器）
3. 编译、加载动态生成的类
```java
private Class<?> createAdaptiveExtensionClass() {
    // 动态生成自适应扩展类代码
    String code = new AdaptiveClassCodeGenerator(type, cachedDefaultName).generate();
    // 获取类加载器
    ClassLoader classLoader = findClassLoader();
    // 获取编译器类 ⚠️ AdaptiveCompiler 也是自己定义的
    org.apache.dubbo.common.compiler.Compiler compiler =  ExtensionLoader.getExtensionLoader(org.apache.dubbo.common.compiler.Compiler.class).getAdaptiveExtension();
    //编译、加载、生产Class
    return compiler.compile(code, classLoader);
}
```
生成自适应扩展类的代码都在`AdaptiveClassCodeGenerator`中，`generate()`方法会生成和返回一个自适应扩展类。之前的版本代码其实比较复杂，逻辑都写在了一起，并没有 `generate` 方法，进入Apache 孵化之后对代码结构进行了调整，结构清晰了许多。<br />主要逻辑如下：

1. 检查接口是否有方法被`@Adaptive`修饰。
2. 生产`class`头部的`package`信息。
3. 生成依赖类的`import`信息。
4. 生成方法声明信息。
5. 遍历接口方法依次生成实现方法。
6. 类结束用`}`收尾，类信息转换为字符串返回。
```java
public String generate() {
    // no need to generate adaptive class since there's no adaptive method found.
    // 检查接口是否有注解了Adaptive的方法，至少需要有一个
    if (!hasAdaptiveMethod()) {
        throw new IllegalStateException("No adaptive method exist on extension " + type.getName() + ", refuse to create the adaptive class!");
    }

    StringBuilder code = new StringBuilder();
    // 生成package信息
    code.append(generatePackageInfo());
    // 生成依赖类的import信息
    code.append(generateImports());
    // 生成类的声明信息 public class 接口名$Adaptive implements 接口名
    code.append(generateClassDeclaration());

    // 遍历接口方法 按需生产实现类
    Method[] methods = type.getMethods();
    for (Method method : methods) {
        code.append(generateMethod(method));
    }
    // 类结尾
    code.append("}");

    if (logger.isDebugEnabled()) {
        logger.debug(code.toString());
    }
    // 转换为字符串返回
    return code.toString();
}
```
下面依次介绍上述步骤的主要逻辑。
<a name="YUd5X"></a>
#### 检查`@Adaptive`注解
遍历接口方法依次检查是否被`@Adaptive` 标注，至少需要有一个方法被注解，否则抛出异常。
```java
private boolean hasAdaptiveMethod() {
    return Arrays.stream(type.getMethods()).anyMatch(m -> m.isAnnotationPresent(Adaptive.class));
}
```
<a name="RiQVS"></a>
#### 生成package和import信息
按照接口的路径名生成对应的package信息，并且生成导入信息，目前只是`import`了`ExtensionLoader`这个类。
```java
private static final String CODE_PACKAGE = "package %s;\n";
private String generatePackageInfo() {
    return String.format(CODE_PACKAGE, type.getPackage().getName());
}
private static final String CODE_IMPORTS = "import %s;\n";
private String generateImports() {
    return String.format(CODE_IMPORTS, ExtensionLoader.class.getName());
}
// import org.apache.dubbo.common.extension.ExtensionLoader;
```
<a name="pgkcV"></a>
#### 生成类的声明信息
生成的类名 = 拓展接口名+$Adaptive，实现的接口就是拓展接口的全限定名。比如 `public class MySPI$Adaptive implements org.daley.spi.demo.MySPI`
```java
private String generateClassDeclaration() {
    return String.format(CODE_CLASS_DECLARATION, type.getSimpleName(), type.getCanonicalName());
}
private static final String CODE_CLASS_DECLARATION = "public class %s$Adaptive implements %s {\n";
```
<a name="Rtjk2"></a>
### 生成方法体
`generateMethod`方法是自适应拓展类生成代理类的核心逻辑所在。它主要会分别拿到方法返回类型、方法名、生成方法体、生成方法参数、生成方法异常，然后按照方法的模板的占位符生成代理方法。很明显，重中之重是生成方法体内容。
```java
private String generateMethod(Method method) {
    // 分别拿到方法返回类型、方法名、方法体、方法参数、方法异常
    String methodReturnType = method.getReturnType().getCanonicalName();
    String methodName = method.getName();
    // 生成方法体
    String methodContent = generateMethodContent(method);
    String methodArgs = generateMethodArguments(method);
    String methodThrows = generateMethodThrows(method);
    // 按照方法模板替换占位符，生成方法内容
    return String.format(CODE_METHOD_DECLARATION, methodReturnType, methodName, methodArgs, methodThrows, methodContent);
}
```
`generateMethodContent`主要做了如下几件事：

1. 检查是否被`@Adaptive`注解，如果没有被注解则生产一段抛出异常的代码。如果被注解，则继续后面逻辑。
2. 找到URL类型参数的index，并且生成检查URL参数是否为空的逻辑。
3. 如果没有URL参数，则检查是否方法参数有public类型无参get方法可以直接拿到URL。
4. 拿到`@Adaptive` 注解配置的value，如果没有配置就用接口名默认。
5. 检查是否有`Invocation`类型参数。
6. 根据不同的情况拿到拓展名。
7. 根据扩展名从`getExtension`中拿到真正的扩展类。
8. 执行扩展类目标方法，按需返回结果。

这些步骤逻辑都不算复杂，需要格外注意的是第6点，这里详细再说明下。
```java
private String generateMethodContent(Method method) {
    Adaptive adaptiveAnnotation = method.getAnnotation(Adaptive.class);
    StringBuilder code = new StringBuilder(512);
    // 检查是否被@Adaptive注解
    if (adaptiveAnnotation == null) {
        // 不是自适应方法生成的代码是一段抛出异常的代码。
        return generateUnsupported(method);
    } else {
        // 找到 URL.class 类型参数位置
        int urlTypeIndex = getUrlTypeIndex(method);

        // found parameter in URL type
        if (urlTypeIndex != -1) {
            // Null Point check
            // 找到 URL 生成代码逻辑
            code.append(generateUrlNullCheck(urlTypeIndex));
        } else {
            // did not find parameter in URL type
            // 未找到 URL 生成代码逻辑
            // 再找找是否有方法参数有get方法可以返回URL.class的，并且还是不需要入参的public方法
            code.append(generateUrlAssignmentIndirectly(method));
        }
        // 拿到 Adaptive注解的value
        String[] value = getMethodAdaptiveValue(adaptiveAnnotation);

        // 有 Invocation 类型参数
        boolean hasInvocation = hasInvocationArgument(method);

        // 检查参数不为null
        code.append(generateInvocationArgumentNullCheck(method));

        code.append(generateExtNameAssignment(value, hasInvocation));
        // check extName == null?
        code.append(generateExtNameNullCheck(value));
        // getExtension 根据name拿到扩展类
        code.append(generateExtensionAssignment());

        // return statement 生成方法调用语句并在必要时返回
        code.append(generateReturnAndInvocation(method));
    }

    return code.toString();
}
```
在`generateExtNameAssignment`中会有如下几种不同的情况：是否最后一个参数，是否有Invocation类型参数、是否配置了名字为protocol的注解value。<br />将上面的三种条件组合，生成对应不同的代码，核心其实都是如何正确的从URL参数中拿到动态扩展名，具体已做注释。
```java
private String generateExtNameAssignment(String[] value, boolean hasInvocation) {
    // TODO: refactor it
    String getNameCode = null;
    // 从最后一个开始遍历
    for (int i = value.length - 1; i >= 0; --i) {
        if (i == value.length - 1) {
            // 如果是最后一个参数，设置了默认拓展名
            if (null != defaultExtName) {
                // 配置的value不等于"protocol"
                if (!"protocol".equals(value[i])) {
                    if (hasInvocation) {
                        // 有invocation 根据配置名字从url获取getMethodParameter
                        getNameCode = String.format("url.getMethodParameter(methodName, \"%s\", \"%s\")", value[i], defaultExtName);
                    } else {
                        // 没有invocation，getParameter获取参数
                        getNameCode = String.format("url.getParameter(\"%s\", \"%s\")", value[i], defaultExtName);
                    }
                } else {
                    // 直接取协议名
                    getNameCode = String.format("( url.getProtocol() == null ? \"%s\" : url.getProtocol() )", defaultExtName);
                }
            } else {
                //没有设置默认拓展名，和上面的区别就是 没有默认值处理的逻辑。上面获取不到可以直接用默认值。
                if (!"protocol".equals(value[i])) {
                    if (hasInvocation) {
                        getNameCode = String.format("url.getMethodParameter(methodName, \"%s\", \"%s\")", value[i], defaultExtName);
                    } else {
                        getNameCode = String.format("url.getParameter(\"%s\")", value[i]);
                    }
                } else {
                    getNameCode = "url.getProtocol()";
                }
            }
        } else {
            // 如果不是最后一个参数
            if (!"protocol".equals(value[i])) {
                if (hasInvocation) {
                    getNameCode = String.format("url.getMethodParameter(methodName, \"%s\", \"%s\")", value[i], defaultExtName);
                } else {
                    getNameCode = String.format("url.getParameter(\"%s\", %s)", value[i], getNameCode);
                }
            } else {
                getNameCode = String.format("url.getProtocol() == null ? (%s) : url.getProtocol()", getNameCode);
            }
        }
    }

    return String.format(CODE_EXT_NAME_ASSIGNMENT, getNameCode);
}
```
对于上述条件，可以生成如下几种情况的代码。
```java
String extName = (url.getProtocol() == null ? "dubbo" : url.getProtocol());
String extName = url.getMethodParameter(methodName, "loadbalance", "random");
String extName = url.getParameter("client", url.getParameter("transporter", "netty"));
```
还有一个疑问点是如果有多个参数怎么办？按照代码逻辑，最终的表现效果就是如果有多个参数，且非Invocation，会生成多层嵌套代码，并且以最外层也就是最左边的参数为准，右边的参数作为默认值。<br />举个例子：
```java
@Adaptive(value = {"protocol","param2","myspi.type"})
void say(URL url);
```
生成代码如下：
```java
url.getProtocol() == null ? (url.getParameter("param2", url.getParameter("myspi.type"))) : url.getProtocol()
```
当上述一切代码执行完成后，就生成了最终的代理类，并且经过编译和加载最终完成实例化，可以被程序所调用，实现动态按需调用。<br />最终生成的代理类如下：
```java
package org.daley.spi.demo;
import org.apache.dubbo.common.extension.ExtensionLoader;

public class MySPI$Adaptive implements org.daley.spi.demo.MySPI {
    public void say(org.apache.dubbo.common.URL arg0)  {
        if (arg0 == null) throw new IllegalArgumentException("url == null");
        org.apache.dubbo.common.URL url = arg0;
        String extName = url.getProtocol() == null ?(url.getParameter("param2",url.getParameter("myspi.type"))) : url.getProtocol();
        if(extName == null) throw new IllegalStateException("Failed to get extension  (org.daley.spi.demo.MySPI) name from url (" + url.toString() + ") use keys([protocol, param2, myspi.type])");
        org.daley.spi.demo.MySPI extension = (org.daley.spi.demo.MySPI)ExtensionLoader.getExtensionLoader(org.daley.spi.demo.MySPI.class).getExtension(extName);
        extension.say(arg0);
    }
}
```
到这里，Dubbo 自适应扩展的原理就讲解结束了。
