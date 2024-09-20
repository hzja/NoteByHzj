Java Annotation
<a name="b6lIT"></a>
## 注解
`Annontation`是Java5开始引入的新特征，中文名称叫注解。它提供了一种安全的类似注释的机制，用来将任何的信息或元数据（metadata）与程序元素（类、方法、成员变量等）进行关联。为程序的元素（类、方法、成员变量）加上更直观更明了的说明，这些说明信息是与程序的业务逻辑无关，并且供指定的工具或框架使用。<br />`Annontation`像一种修饰符一样，应用于包、类型、构造方法、方法、成员变量、参数及本地变量的声明语句中。<br />Java注解是附加在代码中的一些元信息，用于一些工具在编译、运行时进行解析和使用，起到说明、配置的功能。注解不会也不能影响代码的实际逻辑，仅仅起到辅助性的作用。包含在 `java.lang.annotation`包中。<br />在java中，自定义一个注解非常简单，通过`@interface`就能定义一个注解，实现如下
```java
public @interface PrintMsg {
}
```
写个测试类加上自定义注解
```java
@PrintMsg
public class AnnotationTest {
    public static void main(String[] args) {
        System.out.println("annotation test OK!");
    }
}
```
可以发现写与不写这个注解的效果是相同的，注解只是一种**「标记」**，有没有它并不影响程序的运行。
<a name="xWVzb"></a>
## 注解的用处
1、生成文档。这是最常见的，也是java 最早提供的注解。常用的有`@param` `@return` 等<br />2、跟踪代码依赖性，实现替代配置文件功能。比如`Dagger 2` 依赖注入，未来java 开发，将大量注解配置，具有很大用处;<br />3、在编译时进行格式检查。如`@override` 放在方法前，如果这个方法并不是覆盖了超类方法，则编译时就能检查出。<br />4、使代码更加干净易读，易于维护修改。比如，以前使用spring的开发，都是基于xml文件实现了统一的配置管理，但是缺点也是显而易见的，就是随着项目的越来越大，xml文件会变得越来越复杂，维护成本也会越来越高。使用注解就可以提供更大的便捷性，易于维护修改。<br />5、自定义注解，作为额外信息的载体，存储有关程序的额外信息<br />注解元素可用的类型如下所示，如果使用了其他类型，编译器就会报错：

- 所有基本类型（int、float、boolean 等）
- String
- Class
- enum
- Annotation
- 以上类型的数组

如果没有给出 description 的值，在分析处理这个类的时候会使用该元素的默认值。元素的默认值不能有不确定的值，也就是说，元素要么有默认值，要么就在使用注解时提供元素的值<br />这里还有另外一个限制：任何非基本类型的元素，无论是在源代码声明时还是在注解接口中定义默认值时，都不能使用 null 作为值。如果希望表现一个元素的存在或者缺失的状态，可以自定义一些特殊的值，比如空字符串或者负数用于表达某个元素不存在
:::danger
注解不支持继承，不能使用 `extends` 关键字来继承 `@interface`
:::
<a name="JktRj"></a>
## 注解的原理
Java注解是附加在代码中的一些元信息，用于编译和运行时进行解析和使用，起到说明、配置的功能。<br />注解不会影响代码的实际逻辑，仅仅起到辅助性的作用。包含在`java.lang.annotation`包中。注解的定义类似于接口的定义，使用`@interface`来定义，定义一个方法即为注解类型定义了一个元素，方法的声明不允许有参数或throw语句，返回值类型被限定为原始数据类型、字符串`String`、`Class`、`enums`、注解类型，或前面这些的数组，方法可以有默认值。注解并不直接影响代码的语义，但是他可以被看做是程序的工具或者类库。它会反过来对正在运行的程序语义有所影响。注解可以从源文件、class文件或者在运行时通过反射机制多种方式被读取。<br />注解本质是一个继承了`Annotation` 的特殊接口，其具体实现类是Java 运行时生成的动态代理类。而通过反射获取注解时，返回的是Java 运行时生成的动态代理对象`$Proxy1`。通过代理对象调用自定义注解（接口）的方法，会最终调用`AnnotationInvocationHandler` 的`invoke`方法。该方法会从`memberValues` 这个Map 中索引出对应的值。而`memberValues` 的来源是Java 常量池。
:::success
**总结：注解其实就是一个实现了**`**Annotation**`**的接口，通过反射获取到的实际上是通过JDK动态代理生成的代理类，这个类实现了定义的注解接口**
:::
一般来说，注解一般分为三种类型：元注解，标准注解，自定义注解。
<a name="L4C9I"></a>
## 元注解
元注解：对注解进行注解，也就是对注解进行标记，元注解的背后处理逻辑由apt tool提供，对注解的行为做出一些限制，例如生命周期，作用范围等等。<br />`java.lang.annotation` 提供了四种元注解，专门注解其他的注解（在自定义注解的时候，需要使用到元注解）：

- `@Documented` – 注解是否将包含在JavaDoc中
- `@Retention` – 什么时候使用该注解
- `@Target` – 注解用于什么地方
- `@Inherited` – 是否允许子类继承该注解
| **注解** | **解释** |
| --- | --- |
| `@Target` | 表示注解可以用于哪些地方。可能的 ElementType 参数包括：<br />`CONSTRUCTOR`：构造器的声明<br />`FIELD`：字段声明（包括 enum 实例）<br />`LOCAL_VARIABLE`：局部变量声明<br />`METHOD`：方法声明<br />`PACKAGE`：包声明<br />`PARAMETER`：参数声明<br />`TYPE`：类、接口（包括注解类型）或者 enum 声明 |
| `@Retention` | 表示注解信息保存的时长。可选的 `RetentionPolicy` 参数包括：`SOURCE`：注解将被编译器丢弃CLASS：注解在 class 文件中可用，但是会被 VM 丢弃RUNTIME：VM 将在运行期也保留注解，因此可以通过反射机制读取注解的信息 |
| `@Documented` | 将此注解保存在 Javadoc 中 |
| `@Inherited` | 允许子类继承父类的注解 |
| `@Repeatable` | 允许一个注解可以被使用一次或者多次（Java8） |

<a name="Fysj1"></a>
### 1、`@Retention` – 定义该注解的生命周期

- `RetentionPolicy.SOURCE` : 在编译阶段丢弃。这些注解在编译结束之后就不再有任何意义，所以它们不会写入字节码。`@Override`, `@SuppressWarnings`都属于这类注解。
- `RetentionPolicy.CLASS` : 在类加载的时候丢弃。在字节码文件的处理中有用。注解默认使用这种方式
- `RetentionPolicy.RUNTIME` : 始终不会丢弃，运行期也保留该注解，因此可以使用反射机制读取该注解的信息。自定义的注解通常使用这种方式。
| 类型 | 作用 |
| --- | --- |
| SOURCE | 注解只在源码阶段保留，在编译器进行编译的时候这类注解被抹除，常见的`@Override`就属于这种注解 |
| CLASS | 注解在编译期保留，但是当Java虚拟机加载class文件时会被丢弃，这个也是`@Retention`的「默认值」。`@Deprecated`和`@NonNull`就属于这样的注解 |
| RUNTIME | 注解在运行期间仍然保留，在程序中可以通过反射获取，Spring中常见的`@Controller`、`@Service`等都属于这一类 |

<a name="0JKFL"></a>
### 2、`@Target` – 表示该注解用于什么地方。默认值为任何元素，表示该注解用于什么地方。可用的ElementType 参数包括

- `ElementType.CONSTRUCTOR`: 用于描述构造器
- `ElementType.FIELD`: 成员变量、对象、属性（包括enum实例）
- `ElementType.LOCAL_VARIABLE`: 用于描述局部变量
- `ElementType.METHOD`: 用于描述方法
- `ElementType.PACKAGE`: 用于描述包
- `ElementType.PARAMETER`: 用于描述参数
- `ElementType.ANNOTATION_TYPE`：说明该注解只能声明在一个注解类型前
- `ElementType.TYPE`: 用于描述类、接口(包括注解类型) 或enum声明
| 类型 | 作用的对象类型 |
| --- | --- |
| `TYPE` | 类、接口、枚举 |
| `FIELD` | 类属性 |
| `METHOD` | 方法 |
| `PARAMETER` | 参数类型 |
| `CONSTRUCTOR` | 构造方法 |
| `LOCAL_VARIABLE` | 局部变量 |
| `ANNOTATION_TYPE` | 注解 |
| `PACKAGE` | 包 |
| `TYPE_PARAMETER` | 1.8之后，泛型 |
| `TYPE_USE` | 1.8之后，除了`PACKAGE`之外任意类型 |

<a name="1091fb51"></a>
### 3、`@Documented` – 一个简单的Annotations 标记注解，表示是否将注解信息添加在java 文档中。
<a name="AvE05"></a>
### 4、`@Inherited` – 定义该注释和子类的关系
`@Inherited` 元注解是一个标记注解，`@Inherited` 阐述了某个被标注的类型是被继承的。如果一个使用了`@Inherited` 修饰的`annotation` 类型被用于一个class，则这个`annotation` 将被用于该class 的子类。
<a name="jJwIs"></a>
### 5、`@Repeatable` - 表示该注解可以重复标记
<a name="o6wiD"></a>
## 常见标准的Annotation
注解的语法十分简单，只要在现有语法中添加 `@` 符号即可，`java.lang` 包提供了如下五种注解：
<a name="eZeUv"></a>
### 1、`@Override`
`java.lang.Override`是一个标记类型注解，它被用作标注方法。它说明了被标注的方法重载了父类的方法，起到了断言的作用。如果使用了这种注解在一个没有覆盖父类方法的方法时，java 编译器将以一个编译错误来警示。
<a name="fbGxi"></a>
### 2、`@Deprecated`
`Deprecated` 也是一种标记类型注解。当一个类型或者类型成员使用`@Deprecated` 修饰的话，编译器将不鼓励使用这个被标注的程序元素。所以使用这种修饰具有一定的“延续性”：如果在代码中通过继承或者覆盖的方式使用了这个过时的类型或者成员，虽然继承或者覆盖后的类型或者成员并不是被声明为`@Deprecated`，但编译器仍然要报警。
<a name="h4VhQ"></a>
### 3、`@SuppressWarnings`
`SuppressWarning` 不是一个标记类型注解。它有一个类型为`String[]` 的成员，这个成员的值为被禁止的警告名。对于javac 编译器来讲，被`-Xlint` 选项有效的警告名也同样对`@SuppressWarings` 有效，同时编译器忽略掉无法识别的警告名。<br />`@SuppressWarnings("unchecked")`
<a name="2V2K8"></a>
### 4、`@SafeVarargs `
禁止对具有泛型可变参数的方法或构造函数的调用方发出警告
<a name="FHcFr"></a>
### 5、`@FunctionalInterface`
声明接口类型为函数式接口
<a name="hMgiG"></a>
## 注解的属性
除了元注解之外，还能给注解添加属性，注解中的属性以`无参方法的形式定义`，方法名为属性名，返回值为成员变量的类型：生命周期改为Runtime，使得运行期存在可以被获取
```java
@Retention(RetentionPolicy.RUNTIME)
public @interface PrintMsg {
    int count() default 1;
    String name() default "my name is PrintMsg";
}

@PrintMsg(count = 2021)
public class AnnotationTest {
    public static void main(String[] args) {
        //通过反射获取该注解
        PrintMsg annotation = AnnotationTest.class.getAnnotation(PrintMsg.class);
        System.out.println(annotation.count());
        System.out.println(annotation.name());
    }
}
```
输出如下：
```
2021
my name is PrintMsg
```
<a name="Shtwi"></a>
## 注解的类型
按照注解的生命周期以及处理方式的不同，通常将注解分为**「运行时注解」**和**「编译时注解」**

- 运行时注解的本质是实现了`Annotation`接口的特殊接口，JDK在运行时为其创建代理类，注解方法的调用实际是通过`AnnotationInvocationHandler`的`invoke`方法，`AnnotationInvocationHandler`其中维护了一个`Map`，`Map`中存放的是方法名与返回值的映射，对注解中自定义方法的调用其实最后就是用方法名去查`Map`并且放回的一个过程
- 编译时注解通过注解处理器来支持，而注解处理器的实际工作过程由JDK在编译期提供支持，有兴趣可以看看javac的源码
<a name="kNXjn"></a>
### 运行时注解
运行时注解的产生作用的步骤如下：

1. 对annotation的反射调用使得动态代理创建实现该注解的一个类。
2. 代理背后真正的处理对象为`AnnotationInvocationHandler`，这个类内部维护了一个map，这个map的键值对形式为`<注解中定义的方法名，对应的属性名>`。
3. 任何对annotation的自定义方法的调用（抛开动态代理类继承自object的方法），最终都会实际调用`AnnotatiInvocationHandler`的`invoke`方法，并且该`invoke`方法对于这类方法的处理很简单，拿到传递进来的方法名，然后去查`map`。
4. map中`memeberValues`的初始化是在`AnnotationParser`中完成的，在方法调用前就会初始化好，缓存在map里面。
5. `AnnotationParser`最终是通过`ConstantPool`对象从常量池中拿到对应的数据的，再往下`ConstantPool`对象就不深入了。
<a name="rqtBu"></a>
### 编译时的注解
由于编译时注解的很多处理逻辑内化在Javac中，这里不做过多探讨，仅对《深入理解JVM》中的知识点进行梳理和总结。在JDK5中，Java语言提供了对于注解的支持，此时的注解只在程序运行时发挥作用，但是在JDK6中，JDK新加入了一组`插入式注解处理器`的标准API，这组API使得对于注解的处理可以提前至编译期，从而影响到前端编译器的工作！常用的Lombok就是通过注解处理器来实现的
<a name="5oTPl"></a>
#### 「自定义简单注解处理器」
实现自己的注解处理器，首先需要继承抽象类`javax.annotation.processing.AbstractProcessor`，只有`process()`方法需要实现，`process()`方法如下：
```java
//返回值表示是否修改Element元素
public abstract boolean process(Set<? extends TypeElement> annotations,
                                RoundEnvironment roundEnv);
```

- `annotations`：这个注解处理器处理的注解集合
- `roundEnv`：当前round的抽象语法树结点，每一个结点都为一个Element，一共有18种Element包含了Java中 的所有元素：
   - `PACKAGE`（包）
   - `ENUM`（枚举）
   - `CLASS`（类）
   - `ANNOTATION_TYPE`（注解）
   - `INTERFACE`（接口）
   - `ENUM_CONSTANT`（枚举常量）
   - `FIELD`（字段）
   - `PARAMETER`（参数）
   - `LOCAL_VARIABLE`（本地变量）
   - `EXCEPTION_PARAMETER`（异常）
   - `METHOD`（方法）
   - `CONSTRUCTOR`（构造方法）
   - `STATIC_INIT`（静态代码块）
   - `INSTANCE_INIT`（实例代码块）
   - `TYPE_PARAMETER`（参数化类型，泛型尖括号中的）
   - `RESOURCE_VARIABLE`（资源变量，try-resource）
   - `MODULE`（模块）
   - `OTHER`（其他）

此外还有一个重要的实例变量`processingEnv`，它提供了上下文环境，需要创建新的代码，向编译器输出信息，获取其他工具类都可以通过它<br />实现一个简单的编译器注解处理器也非常简单，继承`AbstractProcessor`实现`process()`方法，在`process()`方法中实现自己的处理逻辑即可，此外需要两个注解配合一下：

- `@SupportedAnnotationTypes`：该注解处理器处理什么注解
- `@SupportedSourceVersion`：注解处理器支持的语言版本
<a name="YFjjP"></a>
#### 「实例」
```java
@SupportedAnnotationTypes("com.fcant.jdkStudy.annotation.PrintMsg")
@SupportedSourceVersion(SourceVersion.RELEASE_8)
public class PrintNameProcessor extends AbstractProcessor {
    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
        Messager messager = processingEnv.getMessager();
        for (Element element : roundEnv.getRootElements()) {
            messager.printMessage(Diagnostic.Kind.NOTE,"my name is "+element.toString());
        }
        //不修改语法树，返回false
        return false;
    }
}
```
输出如下：
```
D:\LinkSpace\Work\WorkSpace\Code\IDEA\cookcode\src\main\java>javac com\hustdj\jdkStudy\annotation\PrintMsg.java
D:\LinkSpace\Work\WorkSpace\Code\IDEA\cookcode\src\main\java>javac com\hustdj\jdkStudy\annotation\PrintNameProcessor.java
D:\LinkSpace\Work\WorkSpace\Code\IDEA\cookcode\src\main\java>javac -processor com.fcant.jdkStudy.annotation.PrintNameProcessor com\fcant\jdkStudy\annotation\AnnotationTest.java
警告: 来自注释处理程序 'com.fcant.jdkStudy.annotation.PrintNameProcessor' 的受支持 source 版本 'RELEASE_8' 低于 -source '1.9'
注: my name is com.fcant.jdkStudy.annotation.AnnotationTest
1 个警告
```
<a name="FZvOf"></a>
## 自定义注解
<a name="xXQRX"></a>
### 自定义注解类编写的一些规则

1. `Annotation` 型定义为`@interface`，所有的`Annotation` 会自动继承`java.lang.Annotation`这一接口，并且不能再去继承别的类或是接口.
2. 参数成员只能用`public` 或默认(`default`) 这两个访问权修饰
3. 参数成员只能用基本类型`byte`、`short`、`char`、`int`、`long`、`float`、`double`、`boolean`八种基本数据类型和`String`、`Enum`、`Class`、`annotations`等数据类型，以及这一些类型的数组.
4. 要获取类方法和字段的注解信息，必须通过Java的反射技术来获取 Annotation 对象，因为除此之外没有别的获取注解对象的方法
5. 注解也可以没有定义成员，不过这样注解就没啥用了
:::info
PS：自定义注解需要使用到元注解
:::
<a name="3mwm8"></a>
### 自定义注解实例
FruitName.java
```java
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;
import static java.lang.annotation.ElementType.FIELD;
import static java.lang.annotation.RetentionPolicy.RUNTIME;
/**
 * 水果名称注解
 */
@Target(FIELD)
@Retention(RUNTIME)
@Documented
public @interface FruitName {
    String value() default "";
}
```
FruitColor.java
```java
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;
import static java.lang.annotation.ElementType.FIELD;
import static java.lang.annotation.RetentionPolicy.RUNTIME;
/**
 * 水果颜色注解
 */
@Target(FIELD)
@Retention(RUNTIME)
@Documented
public @interface FruitColor {
    /**
     * 颜色枚举
     */
    public enum Color{ BLUE,RED,GREEN};
    /**
     * 颜色属性
     */
    Color fruitColor() default Color.GREEN;
}
```
FruitProvider.java
```java
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;
import static java.lang.annotation.ElementType.FIELD;
import static java.lang.annotation.RetentionPolicy.RUNTIME;
/**
 * 水果供应者注解
 */
@Target(FIELD)
@Retention(RUNTIME)
@Documented
public @interface FruitProvider {
    /**
     * 供应商编号
     */
    public int id() default -1;
    /**
     * 供应商名称
     */
    public String name() default "";
    /**
     * 供应商地址
     */
    public String address() default "";
}
```
FruitInfoUtil.java
```java
import java.lang.reflect.Field;
/**
 * 注解处理器
 */
public class FruitInfoUtil {
    public static void getFruitInfo(Class<?> clazz){
        String strFruitName=" 水果名称：";
        String strFruitColor=" 水果颜色：";
        String strFruitProvicer="供应商信息：";
        Field[] fields = clazz.getDeclaredFields();
        for(Field field :fields){
            if(field.isAnnotationPresent(FruitName.class)){
                FruitName fruitName = (FruitName) field.getAnnotation(FruitName.class);
                strFruitName=strFruitName+fruitName.value();
                System.out.println(strFruitName);
            }
            else if(field.isAnnotationPresent(FruitColor.class)){
                FruitColor fruitColor= (FruitColor) field.getAnnotation(FruitColor.class);
                strFruitColor=strFruitColor+fruitColor.fruitColor().toString();
                System.out.println(strFruitColor);
            }
            else if(field.isAnnotationPresent(FruitProvider.class)){
                FruitProvider fruitProvider= (FruitProvider) field.getAnnotation(FruitProvider.class);
                strFruitProvicer=" 供应商编号："+fruitProvider.id()+" 供应商名称："+fruitProvider.name()+" 供应商地址："+fruitProvider.address();
                System.out.println(strFruitProvicer);
            }
        }
    }
}
```
Apple.java
```java
import test.FruitColor.Color;
/**
 * 注解使用
 */
public class Apple {
    @FruitName("Apple")
    private String appleName;
    @FruitColor(fruitColor=Color.RED)
    private String appleColor;
    @FruitProvider(id=1,name="陕西红富士集团",address="陕西省西安市延安路89号红富士大厦")
    private String appleProvider;
    public void setAppleColor(String appleColor) {
        this.appleColor = appleColor;
    }
    public String getAppleColor() {
        return appleColor;
    }
    public void setAppleName(String appleName) {
        this.appleName = appleName;
    }
    public String getAppleName() {
        return appleName;
    }
    public void setAppleProvider(String appleProvider) {
        this.appleProvider = appleProvider;
    }
    public String getAppleProvider() {
        return appleProvider;
    }
    public void displayName(){
        System.out.println("水果的名字是：苹果");
    }
}
```
FruitRun.java
```java
/**
 * 输出结果
 */
public class FruitRun {
    public static void main(String[] args) {
        FruitInfoUtil.getFruitInfo(Apple.class);
    }
}
```
运行结果是：
> 水果名称：Apple<br />水果颜色：RED<br />供应商编号：1 供应商名称：陕西红富士集团 供应商地址：陕西省西安市延安路89号红富士大厦

<a name="POxIL"></a>
### 自定义注解的实现过程

1. 定义注解
2. 使用注解，根据自己定义的注解来达到一些目的
3. 读取注解的内容，也是比较重要的一部分，核心还是利用了反射的思想，得到使用注解的这个类，根据类中的getAnnotion的方法得到定义的注解，获得注解上的值。
<a name="xs8cV"></a>
## 注解的实现的原理
注解的实现的原理很大的一部分是基于反射实现。<br />反射可以获取到Class对象，进而获取到Constructor、Field、Method等实例，点开源码结构发现Class、Constructor、Field、Method等均实现了`AnnotatedElement`接口，`AnnotatedElement`接口的方法如下
```java
// 判断该元素是否包含指定注解，包含则返回true
boolean isAnnotationPresent(Class<? extends Annotation> annotationClass)
// 返回该元素上对应的注解，如果没有返回null
<T extends Annotation> T getAnnotation(Class<T> annotationClass);
// 返回该元素上的所有注解，如果没有任何注解则返回一个空数组
Annotation[] getAnnotations();
// 返回指定类型的注解，如果没有返回空数组
T[] getAnnotationsByType(Class<T> annotationClass)
// 返回指定类型的注解，如果没有返回空数组，只包含直接标注的注解，不包含inherited的注解
T getDeclaredAnnotation(Class<T> annotationClass)
// 返回指定类型的注解，如果没有返回空数组，只包含直接标注的注解，不包含inherited的注解
T[] getDeclaredAnnotationsByType
// 返回该元素上的所有注解，如果没有任何注解则返回一个空数组，只包含直接标注的注解，不包含inherited的注解
Annotation[] getDeclaredAnnotations();
```
通过一个实例说明一下注解的使用过程：
<a name="b8651ef9"></a>
### 定义注解
```java
@Documented
@Target({ElementType.TYPE, ElementType.FIELD,ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface MyAnnotaion {
    String getValue() default "this is myAnntaion";
    int order() default 0;
}
```
<a name="QQ2oe"></a>
### 使用注解
```java
@MyAnnotaion(getValue = "annotation on class")
public class Demo {
    @MyAnnotaion(getValue = "annotation on filed")
    public String name;
    @MyAnnotaion(getValue = "annotation on method")
    public void hello(){
    }
    @MyAnnotaion
    public void defaultMethod(){
    }
}
```
<a name="PRvlb"></a>
### 利用反射读取注解中的值
```java
public class TestDemo {
    public static void main(String[] args) throws NoSuchFieldException, NoSuchMethodException {
        /**
         * 获取类上的注解
         */
        Class<Demo> demoClass = Demo.class;
        Annotation[] annotaion = demoClass.getAnnotations();
        printAnnotation(annotaion);
        /**
         * 读取成员变量上的注解
         */
        Field name = demoClass.getField("name");
        Annotation[] getOnFiled = name.getAnnotations();
        printAnnotation(getOnFiled);
        /**
         * 读取方法上的注解
         */
        Method hello = demoClass.getMethod("hello", null);
        MyAnnotaion onMethod = hello.getAnnotation(MyAnnotaion.class);
        System.out.println(onMethod.getValue());
        /**
         * 获取默认方法上的注解
         */
        Method defaultMethod = demoClass.getMethod("defaultMethod", null);
        MyAnnotaion onDefaultMethod = defaultMethod.getAnnotation(MyAnnotaion.class);
        System.out.println(onDefaultMethod.getValue());
    }
    public static void printAnnotation(Annotation... annotations) {
        for (Annotation annotation : annotations) {
            System.out.println(annotation);
        }
    }
}
```
<a name="vcrSv"></a>
### 运行结果
```java
@annocation.MyAnnotaion(getValue=annotation on class, order=0)
@annocation.MyAnnotaion(getValue=annotation on filed, order=0)
annotation on method
this is myAnntaion
```
<a name="J3Gr5"></a>
## 注解处理器
如果没有用于读取注解的工具，那么注解不会比注释更有用。使用注解中一个很重要的作用就是创建与使用注解处理器。Java 拓展了反射机制的 API 用于创造这类工具。同时他还提供了 javac 编译器钩子在编译时使用注解。下面是一个非常简单的注解处理器，用它来读取被注解的 TestUtils 类，并且使用反射机制来寻找 `@TestAnnotation` 标记
```java
public class TestAnnotationTracker {
    
    public static void trackTestAnnotation(Class<?> cl) {
        for(Method m : cl.getDeclaredMethods()) {
            TestAnnotation ta = m.getAnnotation(TestAnnotation.class);
            if(ta != null) {
                System.out.println(ta.id() + "\n " + ta.description());
            }
        }
    }
    
    public static void main(String[] args) {
        trackTestAnnotation(TestUtils.class);
    }
}
```
这里用到了两个反射的方法：`getDeclaredMethods()` 和 `getAnnotation()`，`getAnnotation()` 方法返回指定类型的注解对象，在本例中就是 TestAnnotation，如果被注解的方法上没有该类型的注解，返回值就为 null。通过调用 `id()` 和 `description()` 方法来提取元素值
<a name="h3rFW"></a>
## 使用注解实现对象 - 数据库映射
当有些框架需要一些额外的信息才能与源代码协同工作，这种情况下注解就会变得十分有用。自定义例如对象/关系映射工具（Hibernate 和 MyBatis）通常都需要 XML 描述文件，而这些文件脱离于代码之外。除了定义 Java 类，还必须重复的提供某些信息，而例如类名和包名等信息已经在原始类中提供过了，经常会导致代码和描述文件的同步问题<br />假设现在想提供一些基本的对象/关系映射功能，能够自动生成数据库表。可以使用 XML 描述文件来指明类的名字、每个成员以及数据库映射的相关信息。但是，通过使用注解，可以把所有信息都保存在 JavaBean 源文件中。为此需要一些用于定义数据库表名称、数据库列以及将 SQL 类型映射到属性的注解。<br />首先创建一个用来映射数据库表的注解，用来修饰类、接口（包括注解类型）或者 enum 声明
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface DBTable {
    String name() default "";
}
```
如下是修饰字段的注解
```java
@Target(ElementType.FIELD)
@Retention(RetentionPolicy.RUNTIME)
public @interface Constraints {
    boolean primaryKey() default false;
    boolean allowNull() default true;
    boolean unique() default false;
}
```
```java
public @interface SQLString {
    int value() default 0;
    String name() default "";
    Constraints constraints() default @Constraints;
}
```
```java
@Target(ElementType.FIELD)
@Retention(RetentionPolicy.RUNTIME)
public @interface SQLInteger {
    String name() default "";
    Constraints constraints() default @Constraints;
}
```
`@Constraints` 代表了数据库通常提供的约束的一小部分，`primaryKey()`，`allowNull()` 和 `unique()` 元素都提供了默认值，大多数情况下，注解的使用者都不需要输入太多东西<br />另外两个 `@interface` 定义的是 SQL 类型。如果希望这个框架更有价值的话，应该为每个 SQL 类型都定义相应的注解。不过作为示例，两个元素足够了。这些 SQL 类型具有 `name()` 元素和 `constraints()` 元素。后者利用了嵌套注解的功能，将数据库列的类型约束信息嵌入其中。注意 `constraints()` 元素的默认值是 `@Constraints`，没有在括号中指明 `@Constraints` 元素的值，因此，`constraints()` 的默认值为所有元素都为默认值。如果要使得嵌入的 `@Constraints` 注解中的 `unique()` 元素为 `true`，并作为 `constraints()` 元素的默认值，可以像如下定义：
```java
public @interface Uniqueness {
    Constraints constraints() default @Constraints(unique = true);
}
```
下面是一个简单的，使用了如上注解的类
```java
@DBTable(name = "MEMBER")
public class Member {
    @SQLString(30)
    String firstName;
    @SQLString(50)
    String lastName;
    @SQLInteger
    Integer age;
    @SQLString(value = 30, constraints = @Constraints(primaryKey = true))
    String reference;
    static int memberCount;
    public String getReference() { return reference; }
    public String getFirstName() { return firstName; }
    public String getLastName() { return lastName; }
    @Override
    public String toString() { return reference; }
    public Integer getAge() { return age; }
}
```
类注解 `@DBTable` 注解给定了元素值 MEMBER，它将会作为表的名字。类的属性 firstName 和 lastName 都被注解为 `@SQLString` 类型并且给了默认元素值分别为 30 和 50，并在嵌入的 `@Constraint` 注解中设定 primaryKey 元素的值<br />下面是一个注解处理器的例子，它将读取一个类文件，检查上面的数据库注解，并生成用于创建数据库的 SQL 命令：
```java
public class TableCreator {
    
    public static void generateSql(String[] classnames) throws Exception {
        for (String className : classnames) {
            Class<?> cl = Class.forName(className);
            DBTable dbTable = cl.getAnnotation(DBTable.class);
            String tableName = dbTable.name();
            // 如果表名为空字符串，则使用类名
            if (tableName.length() < 1) {
                tableName = cl.getName().toUpperCase();
            }
            List<String> columnDefs = new ArrayList<>();
            for (Field field : cl.getDeclaredFields()) {
                String columnName = null;
                Annotation[] anns = field.getDeclaredAnnotations();
                // 该属性不是列
                if (anns.length < 1) {
                    continue;
                }
                // 处理整数类型
                if (anns[0] instanceof SQLInteger) {
                    SQLInteger sInt = (SQLInteger) anns[0];
                    // 如果列名为空字符串，则使用属性名
                    if (sInt.name().length() < 1) {
                        columnName = field.getName().toUpperCase();
                    } else {
                        columnName = sInt.name();
                    }
                    columnDefs.add(columnName + " INT" + getConstraints(sInt.constraints()));
                }
                // 处理字符串类型
                if (anns[0] instanceof SQLString) {
                    SQLString sString = (SQLString) anns[0];
                    if (sString.name().length() < 1) {
                        columnName = field.getName().toUpperCase();
                    } else {
                        columnName = sString.name();
                    }
                    columnDefs.add(columnName + " VARCHAR(" + sString.value() + ")" +
                            getConstraints(sString.constraints()));
                }
                // 构造并输出 sql 字符串
                StringBuilder createCommand = new StringBuilder("CREATE TABLE " + tableName + "(");
                for (String columnDef : columnDefs) {
                    createCommand.append("\n " + columnDef + ",");
                }
                String tableCreate = createCommand.substring(0, createCommand.length() - 1) + ");";
                System.out.println("Table Creation SQL for " + className + " is:\n" + tableCreate);
            }
        }
    }
    private static String getConstraints(Constraints con) {
        String constraints = "";
        if (!con.allowNull())
            constraints += " NOT NULL";
        if (con.primaryKey())
            constraints += " PRIMARY KEY";
        if (con.unique())
            constraints += " UNIQUE";
        return constraints;
    }
}
```
<a name="roqNy"></a>
## 编译时注解处理
当 `@Retention` 的 `RetentionPolicy` 参数被标注为 `SOURCE` 或 `CLASS`，此时无法通过反射去获取注解信息，因为注解在运行期是不存在的。使用 javac 可以创建编译时注解处理器，在编译时扫描和处理注解。可以自定义注解，并注册到对应的注解处理器。注解处理器可以生成 Java 代码，这些生成的 Java 代码会组成新的 Java 源文件，但不能修改已经存在的 Java 类，例如向已有的类中添加方法。如果注解处理器创建了新的源文件，在新一轮处理中注解处理器会检查源文件本身，在检测一轮之后持续循环，直到不再有新的源文件产生，然后编译所有的源文件<br />编写一个简单的注解处理器，如下是注解的定义
```java
@Retention(RetentionPolicy.SOURCE)
@Target({ElementType.TYPE, ElementType.METHOD,
        ElementType.CONSTRUCTOR,
        ElementType.ANNOTATION_TYPE,
        ElementType.PACKAGE, ElementType.FIELD,
        ElementType.LOCAL_VARIABLE})
public @interface Simple {
    String value() default "-default-";
}
```
`@Retention` 的参数为 `SOURCE`，这意味着注解不会存留在编译后的 class 文件，因为这对应编译时处理注解是没有必要的，在这里，javac 是唯一有机会处理注解的方式
```java
package annotations.simplest;
@Simple
public class SimpleTest {
    @Simple
    int i;
    @Simple
    public SimpleTest() {}
    @Simple
    public void foo() {
        System.out.println("SimpleTest.foo()");
    }
    @Simple
    public void bar(String s, int i, float f) {
        System.out.println("SimpleTest.bar()");
    }
    @Simple
    public static void main(String[] args) {
        @Simple
        SimpleTest st = new SimpleTest();
        st.foo();
    }
}
```
运行 main 方法，程序就会开始编译，如下是一个简单的处理器，作用就是把注解相关的信息打印出来
```java
package annotations.simplest;
import javax.annotation.processing.*;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.*;
import java.util.*;
@SupportedAnnotationTypes("annotations.simplest.Simple")
@SupportedSourceVersion(SourceVersion.RELEASE_8)
public class SimpleProcessor extends AbstractProcessor {
    
    @Override
    public boolean process(Set<? extends TypeElement> annotations,
     RoundEnvironment env) {
        for(TypeElement t : annotations) {
            System.out.println(t);
        }
        for(Element el : env.getElementsAnnotatedWith(Simple.class)) {
            display(el);
        }
        return false;
    }
    
    private void display(Element el) {
        System.out.println("==== " + el + " ====");
        System.out.println(el.getKind() + // 返回此元素的种类，字段，方法，或是类
                " : " + el.getModifiers() + // 返回此元素的修饰符
                " : " + el.getSimpleName() + // 返回此元素的简单名称
                " : " + el.asType()); // 返回此元素定义的类型
        // 如果元素为CLASS类型，动态向下转型为更具体的元素类型，并打印相关信息
        if(el.getKind().equals(ElementKind.CLASS)) {
            TypeElement te = (TypeElement)el;
            System.out.println(te.getQualifiedName());
            System.out.println(te.getSuperclass());
            System.out.println(te.getEnclosedElements());
        }
        // 如果元素为METHOD类型，动态向下转型为更具体的元素类型，并打印相关信息
        if(el.getKind().equals(ElementKind.METHOD)) {
            ExecutableElement ex = (ExecutableElement)el;
            System.out.print(ex.getReturnType() + " ");
            System.out.print(ex.getSimpleName() + "(");
            System.out.println(ex.getParameters() + ")");
        }
    }
}
```
使用 `@SupportedAnnotationTypes` 和 `@SupportedSourceVersion` 注解来确定支持哪些注解以及支持的 Java 版本<br />注解处理器需要继承抽象类 `javax.annotation.processing.AbstractProcessor`，唯一需要实现的方法就是 `process()`，这里是所有行为发生的地方。第一个参数获取到此注解处理器所要处理的注解集合，第二个参数保留了剩余信息，这里所做的事情只是打印了注解（只存在一个）。`process()` 中实现的第二个操作是循环所有被 `@Simple` 注解的元素，并且针对每一个元素调用 `display()` 方法。展示所有 Element 自身的基本信息。
