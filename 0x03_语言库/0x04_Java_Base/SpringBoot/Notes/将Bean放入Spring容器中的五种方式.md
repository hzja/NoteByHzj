Java Spring
<a name="Dj3NF"></a>
## 将bean放入Spring容器中有哪些方式？
平时在开发中使用Spring的时候，都是将对象交由Spring去管理，那么将一个对象加入到Spring容器中，有哪些方式呢，下面就来总结一下
<a name="O8zx8"></a>
### `@Configuration` + `@Bean`
这种方式其实是最常用的一种方式，`@Configuration`用来声明一个配置类，然后使用 `@Bean` 注解，用于声明一个bean，将其加入到Spring容器中。<br />具体代码如下：
```java
@Configuration
public class MyConfiguration {
    @Bean
    public Person person() {
        Person person = new Person();
        person.setName("spring");
        return person;
    }
}
```
<a name="Sn0Jn"></a>
### `@Componet` + `@ComponentScan`
这种方式也是用的比较多的方式，`@Componet`中文译为组件，放在类名上面，然后`@ComponentScan`放置在配置类上，然后可以指定一个路径，进行扫描带有`@Componet`注解的bean，然后加至容器中。<br />具体代码如下：
```java
@Component
public class Person {
    private String name;

    public String getName() {

        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                '}';
    }
}

@ComponentScan(basePackages = "it.chusen.spring.*")
public class Demo1 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}
```
结果输出：
```
Person{name='null'}
```
表示成功将Person放置在了IOC容器中。
<a name="Xi437"></a>
### `@Import`注解导入
前两种方式，大家用的可能比较多，也是平时开发中必须要知道的，`@Import`注解用的可能不是特别多了，但是也是非常重要的，在进行Spring扩展时经常会用到，它经常搭配自定义注解进行使用，然后往容器中导入一个配置文件。关于`@Import`注解，会多介绍一点，它有三种使用方式会一一介绍。这是`@Import`注解的源码，表示只能放置在类上。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Import {

    /**
   * 用于导入一个class文件
     * {@link Configuration @Configuration}, {@link ImportSelector},
     * {@link ImportBeanDefinitionRegistrar}, or regular component classes to import.
     */
    Class<?>[] value();

}
```
<a name="qXPvv"></a>
#### `@Import`直接导入类
代码示例如下：
```java
public class Person {
    private String name;

    public String getName() {

        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                '}';
    }
}
/**
* 直接使用@Import导入person类，然后尝试从applicationContext中取，成功拿到
**/
@Import(Person.class)
public class Demo1 {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}
```
上述代码直接使用`@Import`导入了一个类，然后自动的就被放置在IOC容器中了。注意Person类上 就不需要任何的注解了，直接导入即可。
<a name="l7eQz"></a>
##### `@Import` + `ImportSelector`
其实在`@Import`注解的源码中，说的已经很清楚了，感兴趣的可以看下，实现一个`ImportSelector`的接口，然后实现其中的方法，进行导入。<br />代码如下：
```java
@Import(MyImportSelector.class)
public class Demo1 {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}

class MyImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{"it.chusen.spring.model.Person"};
    }
}
```
自定义了一个 `MyImportSelector` 实现了 `selectImports` 方法，然后将要导入的类的全限定名写在里面即可，实现起来也是非常简单。
<a name="h1689"></a>
#### `@Import` + `ImportBeanDefinitionRegistrar`
这种方式也需要实现 `ImportBeanDefinitionRegistrar` 中的方法，具体代码如下：
```java
@Import(MyImportBeanDefinitionRegistrar.class)
public class Demo1 {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}

class MyImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {

    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        // 构建一个beanDefinition, 关于beanDefinition后续会介绍，可以简单理解为bean的定义.
        AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder.rootBeanDefinition(Person.class).getBeanDefinition();
        // 将beanDefinition注册到Ioc容器中.
        registry.registerBeanDefinition("person", beanDefinition);
    }
}
```
上述实现其实和`Import`的第二种方式差不多，都需要去实现接口，然后进行导入。接触到了一个新的概念，`BeanDefinition`，可以简单理解为bean的定义(bean的元数据)，也是需要放在IOC容器中进行管理的，先有bean的元数据，`applicationContext`再根据bean的元数据去创建Bean。
<a name="qaBxP"></a>
#### `@Import` + `DeferredImportSelector`
这种方式也需要进行实现接口，其实它和`@Import`的第二种方式差不多，`DeferredImportSelector` 它是 `ImportSelector` 的子接口，所以实现的方法和第二种无异。只是Spring的处理方式不同，它和Spring Boot中的自动导入配置文件 延迟导入有关，非常重要。使用方式如下：
```java
@Import(MyDeferredImportSelector.class)
public class Demo1 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}
class MyDeferredImportSelector implements DeferredImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        // 也是直接将Person的全限定名放进去
        return new String[]{Person.class.getName()};
    }
}
```
关于`@Import`注解的使用方式，大概就以上三种，当然它还可以搭配`@Configuration`注解使用，用于导入一个配置类。
<a name="PfxYn"></a>
### 使用`FactoryBean`接口
`FactoryBean`接口和`BeanFactory`千万不要弄混了，从名字其实可以大概的区分开，`FactoryBean`, 后缀为bean，那么它其实就是一个bean，`BeanFactory`，顾名思义 bean工厂，它是IOC容器的顶级接口，这俩接口其实都很重要。<br />代码示例：
```java
@Configuration
public class Demo1 {
    @Bean
    public PersonFactoryBean personFactoryBean() {
        return new PersonFactoryBean();
    }

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}

class PersonFactoryBean implements FactoryBean<Person> {

    /**
     *  直接new出来Person进行返回.
     */
    @Override
    public Person getObject() throws Exception {
        return new Person();
    }
    /**
     *  指定返回bean的类型.
     */
    @Override
    public Class<?> getObjectType() {
        return Person.class;
    }
}
```
上述代码，使用`@Configuration` + `@Bean`的方式将 `PersonFactoryBean` 加入到容器中，注意，没有向容器中注入 Person，而是直接注入的 `PersonFactoryBean` 然后从容器中拿Person这个类型的bean，成功运行。
<a name="Wnnlw"></a>
### 使用 `BeanDefinitionRegistryPostProcessor`
其实这种方式也是利用到了 `BeanDefinitionRegistry`，在Spring容器启动的时候会执行 `BeanDefinitionRegistryPostProcessor`的 `postProcessBeanDefinitionRegistry`方法，大概意思就是等`beanDefinition`加载完毕之后，对`beanDefinition`进行后置处理，可以在此进行调整IOC容器中的`beanDefinition`，从而干扰到后面进行初始化bean。<br />具体代码如下：
```java
public class Demo1 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        MyBeanDefinitionRegistryPostProcessor beanDefinitionRegistryPostProcessor = new MyBeanDefinitionRegistryPostProcessor();
        applicationContext.addBeanFactoryPostProcessor(beanDefinitionRegistryPostProcessor);
        applicationContext.refresh();
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}

class MyBeanDefinitionRegistryPostProcessor implements BeanDefinitionRegistryPostProcessor {

    @Override
    public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
        AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder.rootBeanDefinition(Person.class).getBeanDefinition();
        registry.registerBeanDefinition("person", beanDefinition);
    }
    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {

    }
}
```
上述代码中，手动向`beanDefinitionRegistry`中注册了person的`BeanDefinition`。最终成功将person加入到`applicationContext`中。
<a name="yrDLC"></a>
## 小结
介绍了向Spring容器中加入bean的几种方式.

- `@Configuration` + `@Bean`
- `@ComponentScan` + `@Component`
- `@Import` 配合接口进行导入
- 使用`FactoryBean`。
- 实现`BeanDefinitionRegistryPostProcessor`进行后置处理。

其实向Spring中加入bean的方式有很多种，这里简要介绍了上面这几种。
