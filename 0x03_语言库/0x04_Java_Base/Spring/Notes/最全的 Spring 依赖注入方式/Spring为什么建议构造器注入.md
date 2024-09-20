Java Spring
<a name="UB9fw"></a>
### `@Autowired`, `@Resource`, `@Inject` 三个注解的区别
Spring 支持使用`@Autowired`, `@Resource`, `@Inject` 三个注解进行依赖注入。下面来介绍一下这三个注解有什么区别。
<a name="sEkL5"></a>
#### `@Autowired`
`@Autowired`为Spring 框架提供的注解，需要导入包`org.springframework.beans.factory.annotation.Autowired`。<br />这里先给出一个示例代码，方便讲解说明：
```java
public interface Svc {

    void sayHello();
}

@Service
public class SvcA implements Svc {

    @Override
    public void sayHello() {
        System.out.println("hello, this is service A");
    }

}

@Service
public class SvcB implements Svc {

    @Override
    public void sayHello() {
        System.out.println("hello, this is service B");
    }

}

@Service
public class SvcC implements Svc {

    @Override
    public void sayHello() {
        System.out.println("hello, this is service C");
    }
}
```
测试类：
```java
@SpringBootTest
public class SimpleTest {

    @Autowired
    // @Qualifier("svcA")
    Svc svc;

    @Test
    void rc() {
        Assertions.assertNotNull(svc);
        svc.sayHello();
    }

}
```
装配顺序：

1. 按照`type`在上下文中查找匹配的bean

 查找`type`为Svc的bean

2. 如果有多个bean，则按照name进行匹配
1. 如果有`@Qualifier`注解，则按照`@Qualifier`指定的`name`进行匹配

    查找name为svcA的bean

1. 如果没有，则按照变量名进行匹配

    查找name为svc的bean

1. 匹配不到，则报错。（`@Autowired(required=false)`，如果设置`required`为`false`(默认为`true`)，则注入失败时不会抛出异常）
<a name="FIwB9"></a>
#### `@Inject`
在Spring 的环境下，`@Inject`和`@Autowired` 是相同的 ，因为它们的依赖注入都是使用`AutowiredAnnotationBeanPostProcessor`来处理的。<br />![2021-07-25-18-58-10-130526.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211210819-d13bff96-36aa-430d-be9b-9f36295046b9.png#clientId=ub72be079-7e2c-4&from=ui&id=u98e62567&originHeight=553&originWidth=1080&originalType=binary&ratio=1&size=1795243&status=done&style=none&taskId=u32ea900d-7fac-4605-8e4f-b64a5f5a293)<br />`@Inject`是 JSR-330 定义的规范 ，如果使用这种方式，切换到`Guice`也是可以的。<br />Guice 是 google 开源的轻量级 DI 框架<br />如果硬要说两个的区别，首先`@Inject`是Java EE包里的，在SE环境需要单独引入。另一个区别在于`@Autowired`可以设置`required=false`而`@Inject`并没有这个属性。
<a name="o1OQJ"></a>
#### `@Resource`
`@Resource`是JSR-250定义的注解。Spring 在 `CommonAnnotationBeanPostProcessor`实现了对JSR-250的注解的处理，其中就包括`@Resource`。<br />![2021-07-25-18-58-10-269525.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211216448-f1e3a822-daa5-4323-85e6-bd0f76fab22b.png#clientId=ub72be079-7e2c-4&from=ui&id=u0790b225&originHeight=390&originWidth=1080&originalType=binary&ratio=1&size=1266112&status=done&style=none&taskId=u308700ee-8a17-4dfc-b5ed-b6a69e27775)<br />`@Resource`有两个重要的属性：`name`和`type`，而Spring 将`@Resource`注解的`name`属性解析为bean的名字，而`type`属性则解析为bean的类型。<br />装配顺序：

1. 如果同时指定了`name`和`type`，则从Spring上下文中找到唯一匹配的bean进行装配，找不到则抛出异常。
2. 如果指定了`name`，则从上下文中查找名称（id）匹配的bean进行装配，找不到则抛出异常。
3. 如果指定了`type`，则从上下文中找到类型匹配的唯一bean进行装配，找不到或是找到多个，都会抛出异常。
4. 如果既没有指定`name`，又没有指定`type`，则默认按照`byName`方式进行装配；如果没有匹配，按照`byType`进行装配。
<a name="XOqUF"></a>
### IDEA 提示 Field injection is not recommended
在使用IDEA 进行Spring 开发的时候，在字段上面使用`@Autowired`注解的时候，IDEA 会有警告提示：
> Field injection is not recommended
> Inspection info: Spring Team Recommends: "Always use constructor based dependency injection in your beans. Always use assertions for mandatory dependencies".

![2021-07-25-18-58-10-366524.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211223214-e52aac1d-5233-419f-8cb4-527fe880a5b2.png#clientId=ub72be079-7e2c-4&from=ui&id=u50d4b85e&originHeight=89&originWidth=1080&originalType=binary&ratio=1&size=288998&status=done&style=none&taskId=uc0a04706-9c0f-4d27-8142-be5b5e97186)<br />翻译过来就是这个意思：
> 不建议使用基于 field 的注入方式。
> Spring 开发团队建议：在Spring Bean 永远使用基于constructor 的方式进行依赖注入。对于必须的依赖，永远使用断言来确认。

比如如下代码：
```java
@Service
public class HelpService {
    @Autowired
    @Qualifier("svcB")
    private Svc svc;

    public void sayHello() {
        svc.sayHello();
    }
}

public interface Svc {
    void sayHello();
}

@Service
public class SvcB implements Svc {
    @Override
    public void sayHello() {
        System.out.println("hello, this is service B");
    }
}
```
将光标放到`@Autowired`处，使用`Alt` + `Enter` 快捷进行修改之后，代码就会变成基于Constructor的注入方式，修改之后 ：
```java
@Service
public class HelpService {
    private final Svc svc;

    @Autowired
    public HelpService(@Qualifier("svcB") Svc svc) {
        // Assert.notNull(svc, "svc must not be null");
        this.svc = svc;
    }

    public void sayHello() {
        svc.sayHello();
    }
}
```
如果按照Spring 团队的建议，如果svc是必须的依赖，应该使用`Assert.notNull(svc, "svc must not be null")`来确认。<br />修正这个警告提示固然简单，但是更重要是去理解为什么Spring 团队会提出这样的建议？直接使用这种基于 field 的注入方式有什么问题？

---

首先需要知道，Spring 中有这么3种依赖注入的方式 ：

- 基于 field 注入（属性注入）
- 基于 setter 注入
- 基于 constructor 注入（构造器注入）
<a name="uMVo4"></a>
#### 1. 基于 field 注入
所谓基于 field 注入，就是在bean的变量上使用注解进行依赖注入。本质上是通过反射的方式直接注入到field，所以private的成员也可以被注入具体的对象。这是平常开发中看的最多也是最熟悉的一种方式，同时，也正是 Spring 团队所不推荐的方式。比如：
```java
@Autowired
private Svc svc;
```
<a name="mViGL"></a>
#### 2. 基于 setter 方法注入
通过对应变量的`setXXX()`方法以及在方法上面使用注解，来完成依赖注入。比如：
```java
private Helper helper;
@Autowired
public void setHelper(Helper helper) {    
    this.helper = helper;
}
```
注：在 Spring 4.3 及以后的版本中，setter 上面的 `@Autowired` 注解是可以不写的。
<a name="ZV2G4"></a>
#### 3. 基于 constructor 注入
将各个必需的依赖全部放在带有注解构造方法的参数中，并在构造方法中完成对应变量的初始化，这种方式，就是基于构造方法的注入，也是日常最为推荐的一种使用方式。这种注入方式很直接，通过对象构建的时候建立关系，所以这种方式对对象创建的顺序会有要求，当然Spring会搞定这样的先后顺序，除非出现循环依赖，然后就会抛出异常。比如：
```java
private final Svc svc;

@Autowired
public HelpService(@Qualifier("svcB") Svc svc) {
    this.svc = svc;
}
```
在 Spring 4.3 及以后的版本中，如果这个类只有一个构造方法，那么这个构造方法上面也可以不写 `@Autowired` 注解。
<a name="BoF6Z"></a>
### 三种依赖注入的对比
在知道了Spring提供的三种依赖注入方式之后，继续回到开头说到的问题：IDEA为什么不推荐使用Field Injection呢？<br />可以从多个开发测试的考察角度来对比一下它们之间的优劣：
<a name="Jopjq"></a>
#### 可靠性
从对象构建过程和使用过程，看对象在各阶段的使用是否可靠来评判：

- Field Injection：不可靠
- Constructor Injection：可靠
- Setter Injection：不可靠

由于构造函数有严格的构建顺序和不可变性，一旦构建就可用，且不会被更改。
<a name="KvyuJ"></a>
#### 可维护性
主要从更容易阅读、分析依赖关系的角度来评判：

- Field Injection：差
- Constructor Injection：好
- Setter Injection：差

还是由于依赖关键的明确，从构造函数中可以显现的分析出依赖关系，对于如何去读懂关系和维护关系更友好。
<a name="ZvfCS"></a>
#### 可测试性
当在复杂依赖关系的情况下，考察程序是否更容易编写单元测试来评判

- Field Injection：差
- Constructor Injection：好
- Setter Injection：好

Constructor Injection和Setter Injection的方式更容易Mock和注入对象，所以更容易实现单元测试。
<a name="qQ5rl"></a>
#### 灵活性
主要根据开发实现时候的编码灵活性来判断：

- Field Injection：很灵活
- Constructor Injection：不灵活
- Setter Injection：很灵活

由于Constructor Injection对Bean的依赖关系设计有严格的顺序要求，所以这种注入方式不太灵活。相反Field Injection和Setter Injection就非常灵活，但也因为灵活带来了局面的混乱，也是一把双刃剑。
<a name="g0hLs"></a>
#### 循环关系的检测
对于Bean之间是否存在循环依赖关系的检测能力：

- Field Injection：不检测
- Constructor Injection：自动检测
- Setter Injection：不检测
<a name="ZpW1b"></a>
#### 性能表现
不同的注入方式，对性能的影响

- Field Injection：启动快
- Constructor Injection：启动慢
- Setter Injection：启动快

主要影响就是启动时间，由于Constructor Injection有严格的顺序要求，所以会拉长启动时间。<br />所以，综合上面各方面的比较，可以获得如下表格：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636074045197-7c797949-4580-4d71-b588-4f38689673bb.webp#clientId=udc963152-fd92-4&from=paste&id=u5207422a&originHeight=224&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uc62799f5-c6f5-4b78-955c-21b1744dd2e)<br />结果一目了然，Constructor Injection在很多方面都是由于其他两种方式的，所以Constructor Injection通常都是首选方案！<br />而Setter Injection比起Field Injection来说，大部分都一样，但因为可测试性更好，所以当要用`@Autowired`的时候，推荐使用Setter Injection的方式，这样IDEA也不会给出警告了。同时，也侧面也反映了，可测试性的重要地位！
<a name="n7PSC"></a>
#### 基于 field 注入的好处
这种方式非常的简洁，代码看起来很简单，通俗易懂。类可以专注于业务而不被依赖注入所污染。只需要把`@Autowired`扔到变量之上就好了，不需要特殊的构造器或者set方法，依赖注入容器会提供所需的依赖。
<a name="HVVgL"></a>
#### 基于 field 注入的坏处
基于 field 注入虽然简单，但是却会引发很多的问题。这些问题在平常开发阅读项目代码的时候就经常遇见。

- 容易违背了单一职责原则 使用这种基于 field 注入的方式，添加依赖是很简单的，就算类中有十几个依赖可能都觉得没有什么问题，普通的开发者很可能会无意识地给一个类添加很多的依赖。但是当使用构造器方式注入，到了某个特定的点，构造器中的参数变得太多以至于很明显地发现something is wrong。拥有太多的依赖通常意味着类要承担更多的责任，明显违背了单一职责原则（SRP：Single responsibility principle）。
- 依赖注入与容器本身耦合依赖注入框架的核心思想之一就是受容器管理的类不应该去依赖容器所使用的依赖。换句话说，这个类应该是一个简单的POJO(Plain Ordinary Java Object)能够被单独实例化并且也能为它提供它所需的依赖。这个问题具体可以表现在：
   - 类和依赖容器强耦合，不能在容器外使用
   - 类不能绕过反射（例如单元测试的时候）进行实例化，必须通过依赖容器才能实例化，这更像是集成测试
- 不能使用属性注入的方式构建不可变对象(final 修饰的变量)
<a name="a7X1l"></a>
#### Spring 开发团队的建议
> Since you can mix constructor-based and setter-based DI, it is a good rule of thumb to use constructors for mandatory dependencies and setter methods or configuration methods for optional dependencies.

简单来说，就是

- 强制依赖就用构造器方式
- 可选、可变的依赖就用setter 注入当然可以在同一个类中使用这两种方法。构造器注入更适合强制性的注入旨在不变性，Setter注入更适合可变性的注入。

看看Spring 这样推荐的理由，首先是基于构造方法注入，
> The Spring team generally advocates constructor injection as it enables one to implement application components as immutable objects and to ensure that required dependencies are not null. Furthermore constructor-injected components are always returned to client (calling) code in a fully initialized state. As a side note, a large number of constructor arguments is a bad code smell, implying that the class likely has too many responsibilities and should be refactored to better address proper separation of concerns.

Spring 团队提倡使用基于构造方法的注入，因为这样一方面可以将依赖注入到一个不可变的变量中 (注：`final` 修饰的变量) ，另一方面也可以保证这些变量的值不会是 null 。此外，经过构造方法完成依赖注入的组件 (注：比如各个 service)，在被调用时可以保证它们都完全准备好了 。与此同时，从代码质量的角度来看，一个巨大的构造方法通常代表着出现了代码异味，这个类可能承担了过多的责任 。<br />而对于基于 setter 的注入，他们是这么说的：
> Setter injection should primarily only be used for optional dependencies that can be assigned reasonable default values within the class. Otherwise, not-null checks must be performed everywhere the code uses the dependency. One benefit of setter injection is that setter methods make objects of that class amenable to reconfiguration or re-injection later.

基于 setter 的注入，则只应该被用于注入非必需的依赖，同时在类中应该对这个依赖提供一个合理的默认值。如果使用 setter 注入必需的依赖，那么将会有过多的 null 检查充斥在代码中。使用 setter 注入的一个优点是，这个依赖可以很方便的被改变或者重新注入 。
<a name="YPvwj"></a>
### 总结
最后给出两个结论：

1. 依赖注入的使用上，Constructor Injection是首选。
2. 使用`@Autowired`注解的时候，要使用Setter Injection方式，这样代码更容易编写单元测试。
