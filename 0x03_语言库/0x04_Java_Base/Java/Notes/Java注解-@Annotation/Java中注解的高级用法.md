Java注解
<a name="cr21T"></a>
## Annotation
注解（Annotation），也叫元数据。一种代码级别的说明。它是JDK1.5及以后版本引入的一个特性，与类、接口、枚举是在同一个层次。它可以声明在包、类、字段、方法、局部变量、方法参数等的前面，用来对这些元素进行说明，注释。作用分类：

1. 编写文档：通过代码里标识的元数据生成文档【生成文档doc文档】
2. 代码分析：通过代码里标识的元数据对代码进行分析【使用反射】
3. 编译检查：通过代码里标识的元数据让编译器能够实现基本的编译检查【Override】

**注解不会改变程序的语义，只是作为注解（标识）存在，可以通过反射机制编程实现对这些元数据（用来描述数据的数据）的访问**
<a name="HEp2t"></a>
## 分类

- 运行期注解：程序运行时才会被解析到的注解，一般通过反射机制来实现，很多框架中都会用到，经常会看到一个注解和一些简单的配置来实现非常复杂的功能
- 编译期注解：一般用来解析类型元数据，根据特定注解解析并生成代码，或者生成一些描述性文件，比如properties、json等，比如为Pojo生成getter和setter方法
<a name="Wsuqd"></a>
## 关键注解
<a name="TPHdE"></a>
### `@java.lang.annotation.Retention`定义注解的有效时期
相关参数：

- `RetentionPolicy.SOURCE`：编译期生效，编译器会丢弃，编译后的class文件并不包含该注解
- `RetentionPolicy.CLASS`：注解会被保留在class文件中，但是运行期不会生效，被JVM忽略
- `RetentionPolicy.RUNTIME`：注解会被保留在class文件中，并且会在运行期生效，JVM会读取
<a name="nc2cU"></a>
### `@Target`定义注解作用对象，也就是注解是可以用在类、方法、参数还是其他地方
相关参数：<br />`ElementType.TYPE`：该注解只能运用到Class, Interface, enum上<br />`ElementType.FIELD`：该注解只能运用到Field上<br />`ElementType.METHOD`：该注解只能运用到方法上<br />`ElementType.PARAMETER`：该注解只能作用在参数上<br />`ElementType.CONSTRUCTOR`：该注解只能作用在构造方法上<br />`ElementType.LOCAL_VARIABLE`：该注解作用在变量或catch语句<br />`ElementType.ANNOTATION_TYPE`：该注解只能作用在注解上<br />`ElementType.PACKAGE`：该注解只能用在包上

---

Java中常见的内置注解：

- `@Override`
- `@Deprecated`
- `@SuppressWarnings`
<a name="v6AuK"></a>
## 继承关系
<a name="KTDyB"></a>
### `@Inherited`
如果某个注解上有`@Inherited`注解，当查找该类型的注解时，会先查找目标类型是否存在注解，如果有，直接返回；否则，继续在父类上寻找注解，停止的条件为在父类上找到该类型的注解或者父类为Object类型。
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Inherited
public @interface ClassMapper {

}
```
下面的示例中，如果ClassMapper没有`@Inherited`修饰，则返回null
```java
Child.class.getAnnotation(ClassMapper.class);
@Slf4j
public class ExtendAnnotationTests {
    @ClassMapper
    public class Demo { }

    public class Child extends Demo{  }
}
```
<a name="uRTPh"></a>
### 元注解（注解上的注解）
在Spring中，注解`@Service`与`@Component`都是用来标记类，交由Spring容器管理其对应的Bean，结果是等效的。主要是Spring将注解和元注解进行了合并
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.ANNOTATION_TYPE)
public @interface Mapper {

}

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Mapper
public @interface ClassMapper {

}
```
通过下面的方法可以拿到元注解，从而进行其他扩展。
```java
public class Tests {
    @Test
    public void test(){
        ClassMapper classMapper = Demo.class.getAnnotation(ClassMapper.class);
        log.info("classMapper: {}", classMapper);
        Mapper mapper = classMapper.annotationType().getAnnotation(Mapper.class);
        log.info("mapper: {}", mapper);
    }
}
```
<a name="EHOLQ"></a>
## 示例
示例主要针对`@java.lang.annotation.Retention`参数的三种情况，了解注解的生效时期：
<a name="LFsML"></a>
### `RetentionPolicy.RUNTIME`
该示例实现通过自定义注解`@SystemProperty`，实现为对象字段设置系统属性
<a name="oEQoc"></a>
#### 定义注解`@SystemProperty`
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
@Documented
public @interface SystemProperty {

    String value();
}
```
<a name="bfdU0"></a>
#### 定义对象工厂
主要作用是在运行时解析注解`@SystemProperty`，并实现系统属性注入的逻辑。前面说到，注解的作用主要是标记，针对`RetentionPolicy.RUNTIME`类型的注解，一般是在运行时通过反射实现注解标识的类、字段或方法等等元素的处理过程。<br />`ObjectFactory`是一个对象生产工厂，这样可以在运行期解析目标对象中的是否有`@SystemProperty`标识的字段，并对该字段进行值的设定，这是该注解设计的初衷，但是实现需要根据需求实现
```java
@Slf4j
public class ObjectFactory {
    // 省略 ...

    public static <T> T getObject(Class<T> type, Object... args){
        Constructor<T> constructor = findTypeConstructor(type, args);
        T object = constructor.newInstance(args);
        // 通过反射找到对象中@SystemProperty的字段，并根据其设置参数将系统属性设定到该对象字段中
        processFieldAnnotations(object, type, SystemProperty.class);
        return object;
    }

    // 省略 ...  
}
```
<a name="Mwie0"></a>
#### 验证
可以查看对象中被注解标识的属性被设置上去了
```java
@Slf4j
public class RuntimeAnnotationTests {
    @Test
    public void run(){
        Demo demo = ObjectFactory.getObject(Demo.class);
        log.info(">> result: {}", demo.user);
    }

    @Data
    public static class Demo{
        @SystemProperty("user.name")
        private String user;
    }
}
```

---

<a name="s1GOv"></a>
### `RetentionPolicy.CLASS`
该示例主要实现，编译器判断通过`@FinalClass`注解标记的类是否为`final`类型
<a name="LYIpH"></a>
#### 定义注解
```java
@Retention(RetentionPolicy.CLASS)
@Target(ElementType.TYPE)
@Documented
public @interface FinalClass {

}
```
<a name="Mie4j"></a>
#### 编写`AbstractProcessor`的实现
```java
@SupportedAnnotationTypes({FinalClassProcessor.FINAL_CLASS})
@SupportedSourceVersion(SourceVersion.RELEASE_8)
@AutoService(Processor.class)
public class FinalClassProcessor extends AbstractProcessor {

    public static final String FINAL_CLASS = "com.sucl.blog.jdk.annotation.compile.FinalClass";

    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
        TypeElement annotationType = this.processingEnv.getElementUtils().getTypeElement(FINAL_CLASS);
        if( annotationType != null ){
            for (Element element : roundEnv.getElementsAnnotatedWith(annotationType)) {
                if( element instanceof TypeElement ){
                    TypeElement typeElement = (TypeElement) element;
                    if( !typeElement.getModifiers().contains(Modifier.FINAL) ){
                        String message = String.format("类【%s】必须为final类型", typeElement);
                        this.processingEnv.getMessager().printMessage(Diagnostic.Kind.ERROR, message);
                    }
                }
            }
        }
        return true;
    }
}
```
<a name="t3z4C"></a>
#### 使`FinalClassProcessor`生效
<a name="koVyt"></a>
##### 基于google auto-service 3.1 添加依赖
```xml
<dependency>
  <groupId>com.google.auto.service</groupId>
  <artifactId>auto-service</artifactId>
  <version>1.1.0</version>
</dependency>
```
在Processor通过注解`@AutoService`标识
```java
@AutoService(Processor.class)
public class FinalClassProcessor extends AbstractProcessor{}
```
<a name="Z64iS"></a>
##### 基于Maven插件
```xml
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-compiler-plugin</artifactId>
  <configuration>
    <annotationProcessors>
      <annotationProcessor>
        com.sucl.blog.jdk.annotation.compile.FinalClassProcessor
      </annotationProcessor>
    </annotationProcessors>
  </configuration>
</plugin>
```
<a name="Ae2ja"></a>
#### 验证
打包，在项目中引入该jar，定义一个类，类似下面这样，当该类没有`final`修饰时，通过`maven install`命令，可以看到控制台打印自定义的错误信息
```
@FinalClass
public final class ProcessorFinder {}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697603795573-57d396fe-b766-46c9-a181-80dc1d890713.png#averageHue=%23fbf7f5&clientId=ufd57bfab-fb6d-4&from=paste&id=u6bdf322f&originHeight=150&originWidth=841&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub2ccc62a-a43c-408b-b934-08f3161102a&title=)
:::danger
_**注意：RetentionPolicy.CLASS的使用需要达打成jar包才行，不然无法再编译时处理注解**_
:::

---

<a name="bPzfn"></a>
### `RetentionPolicy.SOURCE`
定义一个注解，通过打包后的结果观察该注解的状态
<a name="fr12p"></a>
#### 定义注解
```java
@Retention(RetentionPolicy.SOURCE)
@Target(ElementType.TYPE)
@Documented
public @interface System {

}
```
<a name="bfh22"></a>
#### 定义测试类，并通过`@System`修饰
```java
@System
public class SystemProvider {

}
```
<a name="RmvFh"></a>
#### 打包，借助maven-source-plugin同时将源码打包
```xml
<plugins>
  <plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-source-plugin</artifactId>
    <version>3.2.1</version>
    <executions>
      <execution>
        <id>attach-sources</id>
        <goals>
          <goal>jar</goal>
        </goals>
      </execution>
    </executions>
  </plugin>
</plugins>
```
<a name="lMdvl"></a>
#### 在源码包中，可以看到该注解仍然存在，但是class文件中却没有
在基于SpringBoot开发项目时，一般通过 `**@ConfigurationProperties**` 配合 **spring-boot-configuration-processor**，可以实现在项目打包时生成一个spring-configuration-metadata.json的配置描述文件，这样在编写application.yml配置时，就会得到配置提示，其实现方式就是基于 `ConfigurationMetadataAnnotationProcessor`。
<a name="W1ckl"></a>
## 结束语
注解本身没有含义，主要作用是标记目标元素，后续拿到该标识的元数据，进行一系列的处理。注解的使用是非常广泛的，各种框架中都使用频繁，基于注解可以将很多抽象功能提取出来，通过简单的标识来实现各种复杂的功能。
