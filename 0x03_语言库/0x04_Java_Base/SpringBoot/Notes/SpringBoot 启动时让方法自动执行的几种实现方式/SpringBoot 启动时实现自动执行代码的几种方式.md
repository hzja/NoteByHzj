Java SpringBoot
<a name="imiU2"></a>
## 前言
目前开发的SpringBoot项目在启动的时候需要预加载一些资源。而如何实现启动过程中执行代码，或启动成功后执行，是有很多种方式可以选择，可以在`static`代码块中实现，也可以在构造方法里实现，也可以使用`@PostConstruct`注解实现。<br />当然也可以去实现Spring的`ApplicationRunner`与`CommandLineRunner`接口去实现启动后运行的功能。在这里整理一下，在这些位置执行的区别以及加载顺序。
<a name="FScqu"></a>
## Java自身的启动时加载方式
<a name="hU8cn"></a>
### `static`代码块
`static`静态代码块，在类加载的时候即自动执行。
<a name="ddSZA"></a>
### 构造方法
在对象初始化时执行。执行顺序在`static`静态代码块之后。
<a name="XbnNc"></a>
## Spring启动时加载方式
<a name="utEvA"></a>
### `@PostConstruct`注解
`PostConstruct`注解使用在方法上，这个方法在对象依赖注入初始化之后执行。
<a name="yRxP1"></a>
### `ApplicationRunner`和`CommandLineRunner`
SpringBoot提供了两个接口来实现Spring容器启动完成后执行的功能，两个接口分别为`CommandLineRunner`和`ApplicationRunner`。<br />这两个接口需要实现一个run方法，将代码在run中实现即可。这两个接口功能基本一致，其区别在于run方法的入参。`ApplicationRunner`的run方法入参为`ApplicationArguments`，为`CommandLineRunner`的run方法入参为String数组。
<a name="XJInN"></a>
### 何为`ApplicationArguments`
官方文档解释为：
> Provides access to the arguments that were used to run a SpringApplication.

在Spring应用运行时使用的访问应用参数。即可以获取到`SpringApplication.run(…)`的应用参数。
<a name="z0WWb"></a>
### Order注解
当有多个类实现了`CommandLineRunner`和`ApplicationRunner`接口时，可以通过在类上添加`@Order`注解来设定运行顺序。
<a name="Z8Vlw"></a>
### 代码测试
为了测试启动时运行的效果和顺序，编写几个测试代码来运行看看。<br />TestPostConstruct
```java
@Component
public class TestPostConstruct {

    static {
        System.out.println("static");
    }
    public TestPostConstruct() {
        System.out.println("constructer");
    }

    @PostConstruct
    public void init() {
        System.out.println("PostConstruct");
    }
}
```
TestApplicationRunner
```java
@Component
@Order(1)
public class TestApplicationRunner implements ApplicationRunner{
    @Override
    public void run(ApplicationArguments applicationArguments) throws Exception {
        System.out.println("order1:TestApplicationRunner");
    }
}
```
TestCommandLineRunner
```java
@Component
@Order(2)
public class TestCommandLineRunner implements CommandLineRunner {
    @Override
    public void run(String... strings) throws Exception {
        System.out.println("order2:TestCommandLineRunner");
    }
}
```
<a name="mYC3Z"></a>
## 总结
Spring应用启动过程中，肯定是要自动扫描有`@Component`注解的类，加载类并初始化对象进行自动注入。加载类时首先要执行`static`静态代码块中的代码，之后再初始化对象时会执行构造方法。<br />在对象注入完成后，调用带有`@PostConstruct`注解的方法。当容器启动成功后，再根据`@Order`注解的顺序调用`CommandLineRunner`和`ApplicationRunner`接口类中的`run`方法。<br />因此，加载顺序为`static`>`constructer`>`@PostConstruct`>`CommandLineRunner`和`ApplicationRunner`。
