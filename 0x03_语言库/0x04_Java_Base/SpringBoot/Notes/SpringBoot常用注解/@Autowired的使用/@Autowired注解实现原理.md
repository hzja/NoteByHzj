Java @Autowired
<a name="UGjVQ"></a>
## 前言
使用Spring开发时，进行配置主要有两种方式，一是xml的方式，二是java config的方式。<br />Spring技术自身也在不断的发展和改变，从当前SpringBoot的火热程度来看，java config的应用是越来越广泛了，在使用java config的过程当中，不可避免的会有各种各样的注解打交道，其中，使用最多的注解应该就是`@Autowired`注解了。这个注解的功能就是注入一个定义好的bean。<br />那么，这个注解除了常用的属性注入方式之外还有哪些使用方式呢？它在代码层面又是怎么实现的呢？
<a name="CPlr3"></a>
## `@Autowired`注解用法
在分析这个注解的实现原理之前，不妨先来回顾一下`@Autowired`注解的用法。<br />将`@Autowired`注解应用于构造函数，如以下示例所示
```java
public class MovieRecommender {
 
    private final CustomerPreferenceDao customerPreferenceDao;
 
    @Autowired
    public MovieRecommender(CustomerPreferenceDao customerPreferenceDao) {
        this.customerPreferenceDao = customerPreferenceDao;
    }
 
    // ...
}
```
将`@Autowired`注释应用于`setter`方法
```java
public class SimpleMovieLister {
 
    private MovieFinder movieFinder;
 
    @Autowired
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }
 
    // ...
}
```
将`@Autowired`注释应用于具有任意名称和多个参数的方法
```java
public class MovieRecommender {
 
    private MovieCatalog movieCatalog;
 
    private CustomerPreferenceDao customerPreferenceDao;
 
    @Autowired
    public void prepare(MovieCatalog movieCatalog,
            CustomerPreferenceDao customerPreferenceDao) {
        this.movieCatalog = movieCatalog;
        this.customerPreferenceDao = customerPreferenceDao;
    }
 
    // ...
}
```
也可以将`@Autowired`应用于字段，或者将其与构造函数混合，如以下示例所示
```java
public class MovieRecommender {
 
    private final CustomerPreferenceDao customerPreferenceDao;
 
    @Autowired
    private MovieCatalog movieCatalog;
 
    @Autowired
    public MovieRecommender(CustomerPreferenceDao customerPreferenceDao) {
        this.customerPreferenceDao = customerPreferenceDao;
    }
 
    // ...
}
```
直接应用于字段是使用的最多的一种方式，但是使用构造方法注入从代码层面却是更加好的。除此之外，还有以下不太常见的几种方式<br />将`@Autowired`注释添加到需要该类型数组的字段或方法，则Spring会从`ApplicationContext`中搜寻符合指定类型的所有bean，如以下示例所示：
```java
public class MovieRecommender {
 
    @Autowired
    private MovieCatalog[] movieCatalogs;
 
    // ...
}
```
数组可以，可以马上举一反三，那容器也可以吗，答案是肯定的，下面是set以及map的例子：
```java
public class MovieRecommender {
 
    private Set<MovieCatalog> movieCatalogs;
 
    @Autowired
    public void setMovieCatalogs(Set<MovieCatalog> movieCatalogs) {
        this.movieCatalogs = movieCatalogs;
    }
 
    // ...
}

public class MovieRecommender {
 
    private Map<String, MovieCatalog> movieCatalogs;
 
    @Autowired
    public void setMovieCatalogs(Map<String, MovieCatalog> movieCatalogs) {
        this.movieCatalogs = movieCatalogs;
    }
 
    // ...
}
```
以上就是`@Autowired`注解的主要使用方式，经常使用Spring的话应该对其中常用的几种不会感到陌生。
<a name="jeE6p"></a>
## `@Autowired`注解的作用到底是什么
`@Autowired`这个注解经常在使用，现在，想问的是，它的作用到底是什么呢?<br />首先，从所属范围来看，事实上这个注解是属于Spring的容器配置的一个注解，与它同属容器配置的注解还有：`@Required`,`@Primary`, `@Qualifier`等等。因此`@Autowired`注解是一个用于容器(container)配置的注解。<br />其次，可以直接从字面意思来看，`@Autowired`注解来源于英文单词autowire,这个单词的意思是自动装配的意思。自动装配又是什么意思？这个词语本来的意思是指的一些工业上的用机器代替人口，自动将一些需要完成的组装任务，或者别的一些任务完成。而在Spring的世界当中，自动装配指的就是使用将Spring容器中的bean自动的和需要这个bean的类组装在一起。<br />接下来，就来看一下这个注解背后到底做了些什么工作。
<a name="ISjhi"></a>
## `@Autowired`注解是如何实现的
事实上，要回答这个问题必须先弄明白的是java是如何支持注解这样一个功能的。<br />java的注解实现的核心技术是反射，通过一些例子以及自己实现一个注解来理解它工作的原理。
<a name="W4poa"></a>
### 例如注解`@Override`
`@Override`注解的定义如下:
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.SOURCE)
public @interface Override {
}
```
`@Override`注解使用java官方提供的注解，它的定义里面并没有任何的实现逻辑。注意，所有的注解几乎都是这样的，注解只能是被看作元数据，它不包含任何业务逻辑。 注解更像是一个标签，一个声明，表面被注释的这个地方，将具有某种特定的逻辑。<br />那么，问题接踵而至，注解本身不包含任何逻辑，那么注解的功能是如何实现的呢？答案必然是别的某个地方对这个注解做了实现。以`@Override`注解为例，他的功能是重写一个方法，而他的实现者就是JVM，java虚拟机，java虚拟机在字节码层面实现了这个功能。<br />但是对于开发人员，虚拟机的实现是无法控制的东西，也不能用于自定义注解。所以，如果是自己想定义一个独一无二的注解的话，则需要自己为注解写一个实现逻辑，换言之，需要实现自己注解特定逻辑的功能。
<a name="KvYYz"></a>
### 自己实现一个注解
在自己写注解之前有一些基础知识需要掌握，那就是写注解这个功能首先是需要java支持的，java在jdk5当中支持了这一功能，并且在java.lang.annotation包中提供了四个注解，仅用于编写注解时使用，他们是：<br />![2021-06-26-11-08-41-889085.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624677110049-fc64a708-e597-4a07-a7a0-0e198310d82d.png#clientId=ueeda0736-d73d-4&from=ui&id=uad38de6d&originHeight=734&originWidth=419&originalType=binary&ratio=3&size=21649&status=done&style=shadow&taskId=u0226ac00-699b-46bd-9cfe-127801260ba)<br />下面开始自己实现一个注解，注解仅支持 `primitives`, `string`和 `enumerations`这三种类型。注解的所有属性都定义为方法，也可以提供默认值。先实现一个最简单的注解。
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
 
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface SimpleAnnotation {
    String value();
}
```
上面这个注释里面只定义了一个字符传，它的目标注释对象是方法，保留策略是在运行期间。下面定义一个方法来使用这个注解：
```java
public class UseAnnotation {

    @SimpleAnnotation("testStringValue")
    public void testMethod(){
        //do something here
    }

}
```
在这里使用了这个注解，并把字符串赋值为:`testStringValue`,到这里,定义一个注解并使用它，就已经全部完成。<br />简单的不敢相信。但是，细心一想的话，虽然写了一个注解也用了它，可是它并没有产生任何作用啊。也没有对这里方法产生任何效果啊。是的现在确实是这样的，原因在于前面提到的一点，还没有为这个注解实现它的逻辑，现在就来为这个注解实现逻辑。<br />应该怎么做呢？不妨自己来想一想。首先，想给标注了这个注解的方法或字段实现功能，必须得知道，到底有哪些方法，哪些字段使用了这个注解吧，因此，这里很容易想到，这里应该会用到反射。<br />其次，利用反射，利用反射拿到这样目标之后，得为他实现一个逻辑，这个逻辑是这些方法本身逻辑之外的逻辑，想起了代理，aop等知识，相当于就是在为这些方法做一个增强。事实上的实现主借的逻辑也大概就是这个思路。梳理一下大致步骤如下:

- 利用反射机制获取一个类的Class对象
- 通过这个class对象可以去获取他的每一个方法`method`，或字段`Field`等等
- `Method`，Field等类提供了类似于`getAnnotation`的方法来获取这个一个字段的所有注解
- 拿到注解之后，可以判断这个注解是否是要实现的注解，如果是则实现注解逻辑

现在来实现一下这个逻辑，代码如下:
```java
private static void annotationLogic() {

    Class useAnnotationClass = UseAnnotation.class;
    for(Method method : useAnnotationClass.getMethods()) {
        SimpleAnnotation simpleAnnotation = (SimpleAnnotation)method.getAnnotation(SimpleAnnotation.class);
        if(simpleAnnotation != null) {
            System.out.println(" Method Name : " + method.getName());
            System.out.println(" value : " + simpleAnnotation.value());
            System.out.println(" --------------------------- ");
        }
    }
}
```
在这里实现的逻辑就是打印几句话。从上面的实现逻辑不难发现，借助于java的反射可以直接拿到一个类里所有的方法，然后再拿到方法上的注解，当然，也可以拿到字段上的注解。借助于反射可以拿到几乎任何属于一个类的东西。<br />一个简单的注解就实现完了。现在再回过头来，看一下`@Autowired`注解是如何实现的。
<a name="HN3Qm"></a>
## `@Autowired`注解实现逻辑分析
知道了上面的知识，不难想到，上面的注解虽然简单，但是`@Autowired`和他最大的区别应该仅仅在于注解的实现逻辑，其他利用反射获取注解等等步骤应该都是一致的。先来看一下`@Autowired`这个注解在Spring的源代码里的定义是怎样的，如下所示：
```java
package org.springframework.beans.factory.annotation;
 
import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
 
@Target({ElementType.CONSTRUCTOR, ElementType.METHOD, ElementType.PARAMETER, ElementType.FIELD, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Autowired {
    boolean required() default true;
}
```
阅读代码可以看到，`Autowired`注解可以应用在构造方法，普通方法，参数，字段，以及注解这五种类型的地方，它的保留策略是在运行时。下面直接来看Spring对这个注解进行的逻辑实现.<br />在Spring源代码当中，Autowired注解位于包org.springframework.beans.factory.annotation之中，该包的内容如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624677531015-d527f0b6-21ee-40a8-9575-ffbec1230462.png#clientId=ueeda0736-d73d-4&from=paste&id=u1abe5195&originHeight=1402&originWidth=1462&originalType=binary&ratio=3&size=183852&status=done&style=shadow&taskId=u074e7e8a-0de3-4937-810e-0e8faaf0198)<br />经过分析，不难发现Spring对`autowire`注解的实现逻辑位于类：`AutowiredAnnotationBeanPostProcessor`之中,已在上图标红。其中的核心处理代码如下:
```java
private InjectionMetadata buildAutowiringMetadata(final Class<?> clazz) {
    LinkedList<InjectionMetadata.InjectedElement> elements = new LinkedList<>();
    Class<?> targetClass = clazz;//需要处理的目标类

    do {
        final LinkedList<InjectionMetadata.InjectedElement> currElements = new LinkedList<>();

        /*通过反射获取该类所有的字段，并遍历每一个字段，并通过方法findAutowiredAnnotation遍历每一个字段的所用注解，并如果用autowired修饰了，则返回auotowired相关属性*/  

        ReflectionUtils.doWithLocalFields(targetClass, field -> {
            AnnotationAttributes ann = findAutowiredAnnotation(field);
            if (ann != null) {//校验autowired注解是否用在了static方法上
                if (Modifier.isStatic(field.getModifiers())) {
                    if (logger.isWarnEnabled()) {
                        logger.warn("Autowired annotation is not supported on static fields: " + field);
                    }
                    return;
                }//判断是否指定了required
                boolean required = determineRequiredStatus(ann);
                currElements.add(new AutowiredFieldElement(field, required));
            }
        });
        //和上面一样的逻辑，但是是通过反射处理类的method
        ReflectionUtils.doWithLocalMethods(targetClass, method -> {
            Method bridgedMethod = BridgeMethodResolver.findBridgedMethod(method);
            if (!BridgeMethodResolver.isVisibilityBridgeMethodPair(method, bridgedMethod)) {
                return;
            }
            AnnotationAttributes ann = findAutowiredAnnotation(bridgedMethod);
            if (ann != null && method.equals(ClassUtils.getMostSpecificMethod(method, clazz))) {
                if (Modifier.isStatic(method.getModifiers())) {
                    if (logger.isWarnEnabled()) {
                        logger.warn("Autowired annotation is not supported on static methods: " + method);
                    }
                    return;
                }
                if (method.getParameterCount() == 0) {
                    if (logger.isWarnEnabled()) {
                        logger.warn("Autowired annotation should only be used on methods with parameters: " +
                                    method);
                    }
                }
                boolean required = determineRequiredStatus(ann);
                PropertyDescriptor pd = BeanUtils.findPropertyForMethod(bridgedMethod, clazz);
                currElements.add(new AutowiredMethodElement(method, required, pd));
            }
        });
        //用@Autowired修饰的注解可能不止一个，因此都加在currElements这个容器里面，一起处理  
        elements.addAll(0, currElements);
        targetClass = targetClass.getSuperclass();
    }
    while (targetClass != null && targetClass != Object.class);

    return new InjectionMetadata(clazz, elements);
}
```
在源代码里加了注释，结合注释就能看懂它做的事情了，最后这个方法返回的就是包含所有带有`autowire`注解修饰的一个`InjectionMetadata`集合。这个类由两部分组成:
```java
public InjectionMetadata(Class<?> targetClass, Collection<InjectedElement> elements) {
    this.targetClass = targetClass;
    this.injectedElements = elements;
}
```
一是处理的目标类，二就是上述方法获取到的所以elements集合。<br />有了目标类，与所有需要注入的元素集合之后，就可以实现`autowired`的依赖注入逻辑了，实现的方法如下:
```java
@Override
public PropertyValues postProcessPropertyValues(
    PropertyValues pvs, PropertyDescriptor[] pds, Object bean, String beanName) throws BeanCreationException {

    InjectionMetadata metadata = findAutowiringMetadata(beanName, bean.getClass(), pvs);
    try {
        metadata.inject(bean, beanName, pvs);
    }
    catch (BeanCreationException ex) {
        throw ex;
    }
    catch (Throwable ex) {
        throw new BeanCreationException(beanName, "Injection of autowired dependencies failed", ex);
    }
    return pvs;
}
```
它调用的方法是`InjectionMetadata`中定义的`inject`方法，如下
```java
public void inject(Object target, @Nullable String beanName, @Nullable PropertyValues pvs) throws Throwable {
    Collection<InjectedElement> checkedElements = this.checkedElements;
    Collection<InjectedElement> elementsToIterate =
        (checkedElements != null ? checkedElements : this.injectedElements);
    if (!elementsToIterate.isEmpty()) {
        for (InjectedElement element : elementsToIterate) {
            if (logger.isTraceEnabled()) {
                logger.trace("Processing injected element of bean '" + beanName + "': " + element);
            }
            element.inject(target, beanName, pvs);
        }
    }
}
```
其逻辑就是遍历，然后调用`inject`方法，`inject`方法其实现逻辑如下:
```java
/**
 * Either this or {@link #getResourceToInject} needs to be overridden.
 */
protected void inject(Object target, @Nullable String requestingBeanName, @Nullable PropertyValues pvs)
    throws Throwable {

    if (this.isField) {
        Field field = (Field) this.member;
        ReflectionUtils.makeAccessible(field);
        field.set(target, getResourceToInject(target, requestingBeanName));
    }
    else {
        if (checkPropertySkipping(pvs)) {
            return;
        }
        try {
            Method method = (Method) this.member;
            ReflectionUtils.makeAccessible(method);
            method.invoke(target, getResourceToInject(target, requestingBeanName));
        }
        catch (InvocationTargetException ex) {
            throw ex.getTargetException();
        }
    }
}
```
在这里的代码当中也可以看到，是`inject`也使用了反射技术并且依然是分成字段和方法去处理的。在代码里面也调用了`makeAccessible`这样的可以称之为暴力破解的方法，但是反射技术本就是为框架等用途设计的，这也无可厚非。<br />对于字段的话，本质上就是去`set`这个字段的值，即对对象进行实例化和赋值，例如下面代码:
```java
@Autowired
ObjectTest objectTest;
```
那么在这里实现的就相当于给这个objecTest引用赋值了。<br />对于方法的话，本质就是去调用这个方法，因此这里调用的是`method.invoke`.<br />getResourceToInject方法的参数就是要注入的bean的名字，这个方法的功能就是根据这个bean的名字去拿到它。<br />以上，就是`@Autowire`注解实现逻辑的全部分析。结合源代码再看一遍的话，会更加清楚一点。下面是Spring容器如何实现`@AutoWired`自动注入的过程的图:<br />![2021-06-26-11-08-42-622988.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624677276669-989707eb-db39-4e3a-8725-afbc7e67faa2.png#clientId=ueeda0736-d73d-4&from=ui&id=u849bfbb2&originHeight=385&originWidth=1080&originalType=binary&ratio=3&size=1249883&status=done&style=shadow&taskId=u8b9c3f0f-c600-49a9-bc3d-bacc4126d22)<br />总结起来一句话：使用`@Autowired`注入的bean对于目标类来说，从代码结构上来讲也就是一个普通的成员变量，`@Autowired`和Spring一起工作，通过反射为这个成员变量赋值，也就是将其赋为期望的类实例。
<a name="kwktf"></a>
## 问题
<a name="Ap1dY"></a>
### 注解的有效周期是什么？
各种注释之间的第一个主要区别是，它们是在编译时使用，然后被丢弃（如`@Override`），还是被放在编译的类文件中，并在运行时可用（如Spring的`@Component`）。这是由注释的“`@Retention`”策略决定的。如果正在编写自己的注释，则需要决定该注释在运行时（可能用于自动配置）还是仅在编译时（用于检查或代码生成）有用。<br />当用注释编译代码时，编译器看到注释就像看到源元素上的其他修饰符一样，比如访问修饰符（`public`/`private`）或.。当遇到注释时，它运行一个注释处理器，就像一个插件类，表示对特定的注释感兴趣。注释处理器通常使用反射API来检查正在编译的元素，并且可以简单地对它们执行检查、修改它们或生成要编译的新代码。<br />`@Override`是一个示例；它使用反射API来确保能够在其中一个超类中找到方法签名的匹配，如果不能，则使用`@Override`会导致编译错误。
<a name="Kz6cK"></a>
### 注入的bean和用它的bean的关系是如何维护的？
无论以何种方式注入，注入的bean就相当于类中的一个普通对象应用，这是它的实例化是Spring去容器中找符合的bean进行实例化，并注入到类当中的。他们之间的关系就是普通的一个对象持有另一个对象引用的关系。只是这些对象都是Spring当中的bean而已。
<a name="TTSmu"></a>
### 为什么注入的bean不能被定义为`static`的？
从设计的角度来说 ，使用静态字段会鼓励使用静态方法。静态方法是`evil`的。依赖注入的主要目的是让容器为创建对象并进行连接。而且，它使测试更加容易。<br />一旦开始使用静态方法，就不再需要创建对象的实例，并且测试变得更加困难。同样，不能创建给定类的多个实例，每个实例都注入不同的依赖项（因为该字段是隐式共享的，并且会创建全局状态）。<br />静态变量不是Object的属性，而是Class的属性。Spring的`autowire`是在对象上完成的，这样使得设计很干净。 在Spring当中也可以将bean对象定义为单例，这样就能从功能上实现与静态定义相同的目的。<br />但是从纯粹技术的层面，可以这样做：<br />将`@Autowired`可以与`setter`方法一起使用，然后可以让`setter`修改静态字段的值。但是这种做法非常不推荐。
