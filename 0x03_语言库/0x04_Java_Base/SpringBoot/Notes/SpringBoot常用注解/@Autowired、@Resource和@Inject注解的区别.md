Java SpringBoot @Autowired @Resource @Inject<br />在Spring中依赖注入可以使用`@Autowired`、`@Resource`和`@Inject`来完成，并且在一般的使用中是可以相互替换的（注意是一般），不过三者还是有区别，他们三者的区别如下：
<a name="iwRwM"></a>
## `@Autowired`

1. Spring本身替换的注解（org.springframework.beans.factory.annotation.Autowired），需要导入Spring相应的jar包才能使用
2. 可以标注的位置：构造器、方法、方法参数、变量域和注解上面
```java
@Target({ElementType.CONSTRUCTOR, ElementType.METHOD, ElementType.PARAMETER, ElementType.FIELD, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Autowired {
   boolean required() default true;
}
```

3. 在Spring容器解析`@Autowired`注解时，使用的后置处理器为`AutowiredAnnotationBeanPostProcessor`，在这个后置处理的注释中有这么一段：
```java
{@link org.springframework.beans.factory.config.BeanPostProcessor BeanPostProcessor}
* implementation that autowires annotated fields, setter methods, and arbitrary
* config methods. Such members to be injected are detected through annotations:
* by default, Spring's {@link Autowired @Autowired} and {@link Value @Value}
* annotations.
*
* <p>Also supports JSR-330's {@link javax.inject.Inject @Inject} annotation,
* if available, as a direct alternative to Spring's own {@code @Autowired}.
```

4.  `@Autowired`注解有一个`required`属性，当指定`required`属性为`false`时，意味着在容器中找相应类型的bean，如果找不到则忽略，而不报错（这一条是两个注解所没有的功能）。
5. 默认优先按照类型去容器中找对应的组件，找到就赋值，如果找到多个相同类型的组件，再将属性的名称作为组件的id去容器中查找，如果组件id对象的bean不存在，而且`required`属性为`true`，就报错。
6. 支持`@Primary`注解，让Spring进行自动装配的时候，默认使用首选的bean。
<a name="0uUr3"></a>
## `@Resource`

1. JSR250规范提供的注解（javax.annotation.Resource），不需要导入格外的包，这个注解在JDK的rt.jar包中
2. 可以标注的位置：`TYPE`(表示可以标注在接口、类、枚举)，`FIELD`(变量域)和`METHOD`(方法)上面。
```java
@Target({TYPE, FIELD, METHOD})
@Retention(RUNTIME)
public @interface Resource {
    String name() default "";
 
    String lookup() default "";
 
    Class<?> type() default java.lang.Object.class;
 
    enum AuthenticationType {
            CONTAINER,
            APPLICATION
    }
 
    AuthenticationType authenticationType() default AuthenticationType.CONTAINER;
 
    boolean shareable() default true;
 
    String mappedName() default "";
 
    String description() default "";
}
```

3. 在Spring容器解析`@Resource`注解时，使用的后置处理器为`CommonAnnotationBeanPostProcessor`，在这个后置处理的注释中有这么一段： 
```java
* {@link org.springframework.beans.factory.config.BeanPostProcessor} implementation
* that supports common Java annotations out of the box, in particular the JSR-250
* annotations in the {@code javax.annotation} package. These common Java
* annotations are supported in many Java EE 5 technologies (e.g. JSF 1.2),
* as well as in Java 6's JAX-WS.
*
* <p>This post-processor includes support for the {@link javax.annotation.PostConstruct}
* and {@link javax.annotation.PreDestroy} annotations - as init annotation
* and destroy annotation, respectively - through inheriting from
* {@link InitDestroyAnnotationBeanPostProcessor} with pre-configured annotation types.
*
* <p>The central element is the {@link javax.annotation.Resource} annotation
* for annotation-driven injection of named beans, by default from the containing
* Spring BeanFactory, with only {@code mappedName} references resolved in JNDI.
* The {@link #setAlwaysUseJndiLookup "alwaysUseJndiLookup" flag} enforces JNDI lookups
* equivalent to standard Java EE 5 resource injection for {@code name} references
* and default names as well. The target beans can be simple POJOs, with no special
* requirements other than the type having to match.
```

4. 默认是按照组件名称进行装配的

为什么说`@Autowired`是根据类型，`@Resource`是根据组件名称，下面使用代码来进行解释：         
```java
@Component
public class Student {
    private String num = "1";
    public String getNum() {
        return num;
    }
    public void setNum(String num) {
        this.num = num;
    }
}
@Configuration
@ComponentScan(basePackages = {"it.cast.autowired"})
public class Config {
 
    @Bean
    public Student student1(){
        Student student = new Student();
        student.setNum("2");
        return student;
    }
}
@Component
public class Teacher {
 
    @Resource   //注意这里使用的@Resource注解，Spring支持注入Map、Conllent类型的属性变量
    private Map<String,Student> student;
    public Map<String, Student> getStudent() {
        return student;
    }
 
    public void setStudent(Map<String, Student> student) {
        this.student = student;
    }
 
}
public class Test01 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(Config.class);
        Teacher teacher = context.getBean(Teacher.class);
        System.out.println(teacher.getStudent());
    }
}
 
//打印结果：
//Caused by: org.springframework.beans.factory.BeanNotOfRequiredTypeException: Bean named 'student' is expected to be of type 'java.util.Map' but was actually of type 'it.cast.autowired.Student'
// at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:392)
// at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:204)
// at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.resolveBeanByName(AbstractAutowireCapableBeanFactory.java:452)
// at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.autowireResource(CommonAnnotationBeanPostProcessor.java:527)
// at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.getResource(CommonAnnotationBeanPostProcessor.java:497)
// at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor$ResourceElement.getResourceToInject(CommonAnnotationBeanPostProcessor.java:637)
// at org.springframework.beans.factory.annotation.InjectionMetadata$InjectedElement.inject(InjectionMetadata.java:180)
// at org.springframework.beans.factory.annotation.InjectionMetadata.inject(InjectionMetadata.java:90)
// at org.springframework.context.annotation.CommonAnnotationBeanPostProcessor.postProcessProperties(CommonAnnotationBeanPostProcessor.java:322)
// ... 12 more
```

1. 可以看到使用`@Resource`标注的`Map<String,Student>`类型的时报错，为什么报错？因为Map类型的变量名为student，容器中已经存在了名称为student的bean，其类型为Student，所以会报错，大致步骤为：根据组件名称student去Spring容器中获取相应的Bean，在获取之后，会将获取到的Bean赋值给相应的属性。

如果此时将`@Resource`换成`@Autowired`时，其打印结果又会如何？
```java
@Component
public class Student {
    private String num = "1";
    public String getNum() {
        return num;
    }
    public void setNum(String num) {
        this.num = num;
    }
}
@Configuration
@ComponentScan(basePackages = {"it.cast.autowired"})
public class Config {
 
    @Bean
    public Student student1(){
        Student student = new Student();
        student.setNum("2");
        return student;
    }
}
@Component
public class Teacher {
 
    @Autowired  //注意这里使用的@Autowired注解，Spring支持注入Map、Conllent类型的属性变量
    private Map<String,Student> student;
    public Map<String, Student> getStudent() {
        return student;
    }
 
    public void setStudent(Map<String, Student> student) {
        this.student = student;
    }
 
}
public class Test01 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(Config.class);
        Teacher teacher = context.getBean(Teacher.class);
        System.out.println(teacher.getStudent());
    }
}
//其打印结果：
//{student=it.cast.autowired.Student@61009542, student1=it.cast.autowired.Student@77e9807f}
```
可以看到系统并没有报错，根据上面两组代码的对比，可以得出以下结论：
:::success
当使用`@Resource`注解时，是根据组件名称进行查找，当使用`@Autowired`注解时，是根据类型进行查找的。
:::

5. 支持`@Primary`注解，不过首先按照会按照名称进行注入bean，如果Spring IOC容器中没有该Bean，则按照`@Primary`注解标注的bean进行装配。

下面验证`@Resource`默认是按照组件名称进行装配的和持支`@Primary`注解的：
```java
@Configuration
@ComponentScan({"it.cast.resouce"})
public class ResourceConfig {
 
    @Primary   //标有Primary注解，使用@Autowired@Inject注解注解时，优先被加载
    @Bean
    public Y y1(){
        Y y = new Y();
        y.setI(0);
        return y;
    }
 
}
 
@Component
public class X {
    @Resource   //这里使用的是@Resource注解，该注解默认按照组件名称进行装配的，所以会优先加载id为y的bean
    private Y y;
 
    public Y getY() {
        return y;
    }
 
    public void setY(Y y) {
        this.y = y;
    }
}
 
@Component
public class Y {
    private int i = 2;
 
    public int getI() {
        return i;
    }
 
    public void setI(int i) {
        this.i = i;
    }
}
```
测试一下使用`@Resource`注解的打印结果：
```java
@Test
public void ResourceConfigTest(){
    AnnotationConfigApplicationContext context =
        new AnnotationConfigApplicationContext(ResourceConfig.class);

    X bean = context.getBean(X.class);
    System.out.println(bean.getY().getI());
}
//输出结果为：
//     2
//从而验证了@Resource默认按照名称进行加载
```
此时，将`@Resource`注解的属性名称换成y12，这个bean在容器里面没有的
```java
@Configuration
@ComponentScan({"it.cast.resouce"})
public class ResourceConfig {
 
    @Primary   //标有Primary注解，使用@Autowired@Inject注解注解时，优先被加载
    @Bean
    public Y y1(){
        Y y = new Y();
        y.setI(0);
        return y;
    }
 
}
 
@Component
public class X {
    @Resource       //这里使用的是@Resource注解，该注解默认按照组件名称进行装配的，所以会优先加载id为y12的bean，
    private Y y12;  //如果找不到则按Primary注解标注的bean进行注入
 
    public Y getY() {
        return y12;
    }
 
    public void setY(Y y) {
        this.y12 = y;
    }
}
 
@Component
public class Y {
    private int i = 2;
 
    public int getI() {
        return i;
    }
 
    public void setI(int i) {
        this.i = i;
    }
}
```
测试一下使用`@Resource`注解的打印结果： 
```java
@Test
public void ResourceConfigTest(){
    AnnotationConfigApplicationContext context =
        new AnnotationConfigApplicationContext(ResourceConfig.class);

    X bean = context.getBean(X.class);
    System.out.println(bean.getY().getI());
}
//输出结果为：
//     0
//由于没有找到id为y12的bean，所以注入了使用@Primary标注的bean，
//而且整个程序没有报错，所以验证了@Resource支持@Primary注解
```
此时，将`@Resource`注解换成`@Autowired`注解的打印结果：
```java
@Configuration
@ComponentScan({"it.cast.resouce"})
public class ResourceConfig {
 
    @Primary   //标有Primary注解，使用@Autowired@Inject注解注解时，优先被加载
    @Bean
    public Y y1(){
        Y y = new Y();
        y.setI(0);
        return y;
    }
 
}
 
@Component
public class X {
    @Autowired   
    private Y y;  //此时不管名称是y还是y12，都会使用标有Primary注解的bean
 
    public Y getY() {
        return y;
    }
 
    public void setY(Y y) {
        this.y = y;
    }
}
 
@Component
public class Y {
    private int i = 2;
 
    public int getI() {
        return i;
    }
 
    public void setI(int i) {
        this.i = i;
    }
}
```
测试一下使用`@Autowired`注解的打印结果： 
```java
@Test
public void ResourceConfigTest(){
    AnnotationConfigApplicationContext context =
        new AnnotationConfigApplicationContext(ResourceConfig.class);

    X bean = context.getBean(X.class);
    System.out.println(bean.getY().getI());
}
//输出结果为：
//     0
//从而验证了@Autowired支持@Primary注解
```
<a name="iLwiU"></a>
## `@Inject`

1. JSR330规范提供的注解（javax.inject.Inject），主要导入javax.inject包才能使用
2. 可以标注的位置：方法、构造器和变量域中
```java
@Target({ METHOD, CONSTRUCTOR, FIELD })
@Retention(RUNTIME)
@Documented
public @interface Inject {}  //该注解中没有任何属性
```

3. 在Spring容器解析`@Inject`注解时，使用的后置处理器和`@Autowired`是一样的，都是`AutowiredAnnotationBeanPostProcessor`。
4. 由于`@Inject`注解没有属性，在加载所需bean失败时，会报错。
:::info
**除了上面的不同点之后，**`**@Inject**`**和**`**@Autowired**`**完全等价。**
:::
