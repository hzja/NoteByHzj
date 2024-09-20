Java Spring
<a name="e253efa2"></a>
## Spring知识点总结思维导图
![Spring笔记.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565020250016-c391b2f4-329b-44c2-9138-2232c7f80cc9.png#averageHue=%23f1f1f6&height=7172&id=g1Yfw&originHeight=7172&originWidth=3550&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2655215&status=done&style=none&title=&width=3550)

附件原件<br />[Spring笔记.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1565020238115-fe0c1c95-d122-47f6-b455-2a79147867f5.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1565020238115-fe0c1c95-d122-47f6-b455-2a79147867f5.png%22%2C%22name%22%3A%22Spring%E7%AC%94%E8%AE%B0.png%22%2C%22size%22%3A2655215%2C%22ext%22%3A%22png%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1565020206026-7%22%2C%22type%22%3A%22image%2Fpng%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22mode%22%3A%22title%22%2C%22id%22%3A%22FXoQb%22%2C%22card%22%3A%22file%22%7D)
<a name="kMDcW"></a>
## 1、控制反转（IOC）
`IOC` 全称为：Inversion of Control。控制反转的基本概念是：不用创建对象，但是需要描述创建对象的方式。<br />简单的说本来在代码中创建一个对象是通过 `new` 关键字，而使用了 `Spring` 之后，不再需要自己去 `new` 一个对象了，而是直接通过容器里面去取出来，再将其自动注入到需要的对象之中，即：依赖注入。<br />也就说创建对象的控制权不在开发者手上了，全部交由 `Spring` 进行管理，开发者只需要注入就可以了，所以才称之为控制反转。
<a name="36IM6"></a>
## 2、依赖注入（DI）
依赖注入（Dependency Injection，DI）就是 `Spring` 为了实现控制反转的一种实现方式，所以有时候也将控制反转直接称之为依赖注入。
<a name="uoWLC"></a>
### 设值注入
<a name="KJ3WR"></a>
#### 定义
设值注入是指IoC容器通过成员变量的setter方法来注入被依赖对象。这种注入方式简单、直观，因而在Spring的依赖注入里大量使用。
<a name="6hTlv"></a>
#### 优势

1. 与传统的JavaBean的写法更相似，程序开发人员更容易理解、接受。通过setter方法设定依赖关系显得更加直观、自然。
2. 对于复杂的依赖关系，如果采用构造注入，会导致构造器过于臃肿，难以阅读。Spring在创建Bean实例时，需要同时实例化其依赖的全部实例，因而导致性能下降。而使用设值注入，则能避免这些问题。
3. 尤其在某些成员变量可选的情况下，多参数的构造器更加笨重。
<a name="EBu1X"></a>
### 构造注入
<a name="Y0q1e"></a>
#### 定义
利用构造器来设置依赖关系的方式，被称为构造注入。通俗来说，就是驱动Spring在底层以反射方式执行带指定参数的构造器，当执行带参数的构造器时，就可利用构造器参数对成员变量执行初始化——这就是构造注入的本质。
<a name="UG5hD"></a>
#### 优势

1. 构造注入可以在构造器中决定依赖关系的注入顺序，优先依赖的优先注入；
2. 对于依赖关系无需变化的Bean，构造注入更有用处。因为没有setter方法，所有的依赖关系全部在构造器内设定，无须担心后续的代码对依赖关系产生破坏；
3. 依赖关系只能在构造器中设定，则只有组件的创建者才能改变组件的依赖关系，对组件的调用者而言，组件内部的依赖关系完全透明，更符合高内聚的原则。
4. Notes建议采用设值注入为主，构造注入为辅的注入策略。对于依赖关系无须变化的注入，尽量采用构造注入；而其他依赖关系的注入，则考虑采用设值注入。
<a name="TzdwR"></a>
### JavaBean
每一个类实现了Bean的规范才可以由Spring来接管，Bean的规范是:

1. 必须是个公有(public)类
2. 有无参构造函数
3. 用公共方法暴露内部成员属性(getter,setter)

实现这样规范的类，被称为Java Bean。即是一种可重用的组件。
<a name="xWA0R"></a>
### Bean的生命周期

1. Spring对bean进行实例化；
2. Spring将值和bean的引用注入到bean对应的属性中；
3. 如果bean实现了`BeanNameAware`接口，Spring将bean的ID传递给setBean-`Name()`方法；
4. 如果bean实现了`BeanFactoryAware`接口，Spring将调用`setBeanFactory()`方法，将`BeanFactory`容器实例传入；
5. 如果bean实现了`ApplicationContextAware`接口，Spring将调用`setApplicationContext()`方法，将bean所在的应用上下文的引用传入进来；
6. 如果bean实现了`BeanPostProcessor`接口，Spring将调用它们的post-`ProcessBeforeInitialization()`方法；
7. 如果bean实现了`InitializingBean`接口，Spring将调用它们的after-`PropertiesSet()`方法。类似地，如果bean使用init-method声明了初始化方法，该方法也会被调用；
8. 如果bean实现了`BeanPostProcessor`接口，Spring将调用它们的post-`ProcessAfterInitialization()`方法；
9. 此时，bean已经准备就绪，可以被应用程序使用了，它们将一直驻留在应用上下文中，直到该应用上下文被销毁；
10. 如果bean实现了`DisposableBean`接口，Spring将调用它的`destroy()`接口方法。同样，如果bean使用destroy-method声明了销毁方法，该方法也会被调用。
<a name="9RXjx"></a>
### Bean的作用域
Spring定义了多种Bean作用域，可以基于这些作用域创建bean，包括：

- 单例（Singleton）：在整个应用中，只创建bean的一个实例。
- 原型（Prototype）：每次注入或者通过Spring应用上下文获取的时候，都会创建一个新的bean实例。
- 会话（Session）：在Web应用中，为每个会话创建一个bean实例。
- 请求（Rquest）：在Web应用中，为每个请求创建一个bean实例。
<a name="gRNEg"></a>
### 创建Bean的三种方式
<a name="HgTsP"></a>
#### 使用构造器创建Bean实例
使用构造器来创建Bean实例是最常见的情况，如果不采用构造注入，Spring底层会调用Bean类的无参数构造器来创建实例，因此要求该Bean类提供无参数的构造器。<br />采用默认的构造器创建Bean实例，Spring对Bean实例的所有属性执行默认初始化，即所有的基本类型的值初始化为0或false；所有的引用类型的值初始化为null。
<a name="lWv83"></a>
#### 使用静态工厂方法创建Bean
使用静态工厂方法创建Bean实例时，class属性也必须指定，但此时class属性并不是指定Bean实例的实现类，而是静态工厂类，Spring通过该属性知道由哪个工厂类来创建Bean实例。<br />除此之外，还需要使用factory-method属性来指定静态工厂方法，Spring将调用静态工厂方法返回一个Bean实例，一旦获得了指定Bean实例，Spring后面的处理步骤与采用普通方法创建Bean实例完全一样。如果静态工厂方法需要参数，则使用`< constructor-arg…/ >`元素指定静态工厂方法的参数。
<a name="kIASm"></a>
#### 调用实例工厂方法创建Bean
实例工厂方法与静态工厂方法只有一个不同：调用静态工厂方法只需使用工厂类即可，而调用实例工厂方法则需要工厂实例。使用实例工厂方法时，配置Bean实例的`< bean…/ >`元素无须class属性，配置实例工厂方法使用factory-bean指定工厂实例。采用实例工厂方法创建Bean的`< bean…/ >`元素时需要指定如下两个属性：<br />factory-bean：该属性的值为工厂Bean的id<br />factory-method：该属性指定实例工厂的工厂方法<br />若调用实例工厂方法时需要传入参数，则使用`< constructor-arg…/ >`元素确定参数值。
<a name="iUne1"></a>
#### 协调作用域不同步的Bean
当singleton作用域的Bean依赖于prototype作用域的Bean时，会产生不同步的现象，原因是因为当Spring容器初始化时，容器会预初始化容器中所有的singleton Bean，由于singleton Bean依赖于prototype Bean，因此Spring在初始化singleton Bean之前，会先创建prototypeBean——然后才创建singleton Bean，接下里将prototype Bean注入singleton Bean。解决不同步的方法有两种：

- 放弃依赖注入: singleton作用域的Bean每次需要prototype作用域的Bean时，主动向容器请求新的Bean实例，即可保证每次注入的prototype Bean实例都是最新的实例；
- 利用方法注入: 方法注入通常使用lookup方法注入，使用lookup方法注入可以让Spring容器重写容器中Bean的抽象或具体方法，返回查找容器中其他Bean的结果，被查找的Bean通常是一个non-singleton Bean。Spring通过使用JDK动态代理或cglib库修改客户端的二进制码，从而实现上述要求。

建议采用第二种方法，使用方法注入。为了使用lookup方法注入，大致需要如下两步：

1. 将调用者Bean的实现类定义为抽象类，并定义一个抽象方法来获取被依赖的Bean
2. 在`< bean…/ >`元素中添加`< lookup-method…/ >`子元素让Spring为调用者Bean的实现类实现指定的抽象方法Notes；

Spring会采用运行时动态增强的方式来实现`<lookup-method.../>`元素所指定的抽象方法，如果目标抽象类实现过接口，Spring会采用JDK动态代理来实现该抽象类，并为之实现抽象方法；如果目标抽象类没有实现过接口，Spring会采用cglib实现该抽象类，并为之实现抽象方法。Spring4.0的spring-core-xxx.jar包中已经集成了cglib类库。
<a name="Fz2D6"></a>
### 后处理器
Spring提供了两种常用的后处理器：<br />Bean后处理器: 这种后处理器会对容器中Bean进行后处理，对Bean进行额外加强；<br />容器后处理器: 这种后处理器会对IoC容器进行后处理，用于增强容器功能。
<a name="FOIAI"></a>
#### Bean后处理器
Bean后处理器是一种特殊的Bean，这种特殊的Bean并不对外提供服务，它甚至可以无须id属性，它主要负责对容器中的其他Bean执行后处理，例如为容器中的目标Bean生成代理等，这种Bean称为Bean后处理器。Bean后处理器会在Bean实例创建成功之后，对Bean实例进行进一步的增强处理。Bean后处理器必须实现`BeanPostProcessor`接口，同时必须实现该接口的两个方法。

1. `Object postProcessBeforeInitialization(Object bean, String name) throws BeansException`: 该方法的第一个参数是系统即将进行后处理的Bean实例，第二个参数是该Bean的配置id
2. `Object postProcessAfterinitialization(Object bean, String name) throws BeansException`: 该方法的第一个参数是系统即将进行后处理的Bean实例，第二个参数是该Bean的配置id。

容器中一旦注册了Bean后处理器，Bean后处理器就会自动启动，在容器中每个Bean创建时自动工作。<br />注意一点，如果使用`BeanFactory`作为Spring容器，则必须手动注册Bean后处理器，程序必须获取Bean后处理器实例，然后手动注册。
```java
BeanPostProcessor bp = (BeanPostProcessor)beanFactory.getBean("bp");
beanFactory.addBeanPostProcessor(bp);
Person p = (Person)beanFactory.getBean("person");
```
<a name="wZ5oB"></a>
#### 容器后处理器
Bean后处理器负责处理容器中的所有Bean实例，而容器后处理器则负责处理容器本身。容器后处理器必须实现`BeanFactoryPostProcessor`接口，并实现该接口的一个方法`postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory)`实现该方法的方法体就是对Spring容器进行的处理，这种处理可以对Spring容器进行自定义扩展，当然也可以对Spring容器不进行任何处理。<br />类似于`BeanPostProcessor`，`ApplicationContext`可自动检测到容器中的容器后处理器，并且自动注册容器后处理器。但若使用`BeanFactory`作为Spring容器，则必须手动调用该容器后处理器来处理BeanFactory容器。
<a name="6k433"></a>
### Spring中Bean的零配置
<a name="gL1tt"></a>
#### 搜索Bean类-注解式定义配置Bean
Spring提供如下几个Annotation来标注Spring Bean<br />`@Component`: 标注一个普通的Spring Bean类<br />`@Controller`: 标注一个控制器组件类<br />`@Service`: 标注一个业务逻辑组件类<br />`@Repository`: 标注一个DAO组件类<br />在Spring配置文件中做如下配置，指定自动扫描的包
```xml
<context:component-scan base-package="edu.shu.spring.domain"/>
```
<a name="Bchtj"></a>
#### 使用`@Resource`配置依赖
`@Resource`位于javax.annotation包下，是来自JavaEE规范的一个Annotation，Spring直接借鉴了该Annotation，通过使用该Annotation为目标Bean指定协作者Bean。使用`@Resource`与`< property…/ >`元素的ref属性有相同的效果。`@Resource`不仅可以修饰setter方法，也可以直接修饰实例变量，如果使用`@``Resource`修饰实例变量将会更加简单，此时Spring将会直接使用JavaEE规范的Field注入，此时连setter方法都可以不要。
<a name="DtXSg"></a>
#### 使用`@PostConstruct`和`@PreDestroy`定制生命周期行为
`@PostConstruct`和`@PreDestroy`同样位于javax.annotation包下，也是来自JavaEE规范的两个Annotation，Spring直接借鉴了它们，用于定制Spring容器中Bean的生命周期行为。它们都用于修饰方法，无须任何属性。其中前者修饰的方法时Bean的初始化方法；而后者修饰的方法时Bean销毁之前的方法。
<a name="07QjJ"></a>
#### Spring4.0增强的自动装配和精确装配
Spring提供了`@Autowired`注解来指定自动装配，`@Autowired`可以修饰setter方法、普通方法、实例变量和构造器等。当使用`@Autowired`标注setter方法时，默认采用byType自动装配策略。在这种策略下，符合自动装配类型的候选Bean实例常常有多个，这个时候就可能引起异常，为了实现精确的自动装配，Spring提供了`@Qualifier`注解，通过使用`@Qualifier`，允许根据Bean的id来执行自动装配。
<a name="da06c118"></a>
#### 条件化的Bean
一个或多个bean只有在应用的类路径下包含特定的库时才创建。某个bean只有当另外某个特定的bean也声明了之后才会创建。某个特定的环境变量设置之后，才会创建某个bean。<br />在Spring 4之前，很难实现这种级别的条件化配置，但是Spring 4引入了一个新的`@Conditional`注解，它可以用到带有`@Bean`注解的方法上。如果给定的条件计算结果为true，就会创建这个bean，否则的话，这个bean会被忽略。<br />通过`ConditionContext`，可以做到如下几点：

1. 借助`getRegistry()`返回的`BeanDefinitionRegistry`检查bean定义；
2. 借助`getBeanFactory()`返回的`ConfigurableListableBeanFactory`检查bean是否存在，甚至探查bean的属性；
3. 借助`getEnvironment()`返回的`Environment`检查环境变量是否存在以及它的值是什么；
4. 读取并探查`getResourceLoader()`返回的`ResourceLoader`所加载的资源；
5. 借助`getClassLoader()`返回的`ClassLoader`加载并检查类是否存在。
<a name="O3Jlw"></a>
#### 处理自动装配的歧义性
<a name="S0OHn"></a>
##### 标示首选的bean
在声明bean的时候，通过将其中一个可选的bean设置为首选（primary）bean能够避免自动装配时的歧义性。当遇到歧义性的时候，Spring将会使用首选的bean，而不是其他可选的bean。实际上，所声明就是“首选”的bean。
<a name="gj8sa"></a>
##### 限定自动装配的bean
设置首选bean的局限性在于`@Primary`无法将可选方案的范围限定到唯一一个无歧义性的选项中。它只能标示一个优先的可选方案。当首选bean的数量超过一个时，并没有其他的方法进一步缩小可选范围。<br />与之相反，Spring的限定符能够在所有可选的bean上进行缩小范围的操作，最终能够达到只有一个bean满足所规定的限制条件。如果将所有的限定符都用上后依然存在歧义性，那么可以继续使用更多的限定符来缩小选择范围。<br />`@Qualifier`注解是使用限定符的主要方式。它可以与`@Autowired`和`@Inject`协同使用，在注入的时候指定想要注入进去的是哪个bean。例如，想要确保要将IceCream注入到`setDessert()`之中：
```java
@Autowired
@Qualifier("iceCream")
public void setDessert(Dessert dessert){
  	this.dessert = dessert;
}
```
这是使用限定符的最简单的例子。为`@Qualifier`注解所设置的参数就是想要注入的bean的ID。所有使用`@Component`注解声明的类都会创建为bean，并且bean的ID为首字母变为小写的类名。因此，`@Qualifier("iceCream")`指向的是组件扫描时所创建的bean，并且这个bean是IceCream类的实例。<br />实际上，还有一点需要补充一下。更准确地讲，`@Qualifier("iceCream")`所引用的bean要具有String类型的“iceCream”作为限定符。如果没有指定其他的限定符的话，所有的bean都会给定一个默认的限定符，这个限定符与bean的ID相同。因此，框架会将具有“iceCream”限定符的bean注入到`setDessert()`方法中。这恰巧就是ID为iceCream的bean，它是IceCream类在组件扫描的时候创建的。<br />基于默认的bean ID作为限定符是非常简单的，但这有可能会引入一些问题。如果重构了IceCream类，将其重命名为Gelato的话，那此时会发生什么情况呢？如果这样的话，bean的ID和默认的限定符会变为gelato，这就无法匹配`setDessert()`方法中的限定符。自动装配会失败。<br />这里的问题在于`setDessert()`方法上所指定的限定符与要注入的bean的名称是紧耦合的。对类名称的任意改动都会导致限定符失效。
<a name="CUIVk"></a>
#### SpringEL
Value实现资源的注入
<a name="luNMM"></a>
#### Bean的初始化和销毁

1. Java配置方式：`initMethod`和`destoryMethod`
2. 注解：`@PostConstruct`和`@PreDestory`
<a name="9.Profile"></a>
#### Profile
提供在不同的环境下使用不同的配置
<a name="YcE4O"></a>
##### 激活Profile
Spring在确定哪个profile处于激活状态时，需要依赖两个独立的属性：`spring.profiles.active`和`spring.profiles.default`。如果设置了`spring.profiles.active`属性的话，那么它的值就会用来确定哪个profile是激活的。但如果没有设置`spring.profiles.active`属性的话，那Spring将会查找`spring.profiles.default`的值。如果`spring.profiles.active`和`spring.profiles.default`均没有设置的话，那就没有激活的profile，因此只会创建那些没有定义在profile中的bean。
<a name="9f5en"></a>
##### 使用profile进行测试
当运行集成测试时，通常会希望采用与生产环境（或者是生产环境的部分子集）相同的配置进行测试。但是，如果配置中的bean定义在了profile中，那么在运行测试时，就需要有一种方式来启用合适的profile。<br />Spring提供了`@ActiveProfiles`注解，可以使用它来指定运行测试时要激活哪个profile。在集成测试时，通常想要激活的是开发环境的profile。<br />比如Profile("dev")
<a name="be542002"></a>
#### Application Event
使用Application Event可以做到Bean与Bean之间的通信<br />Spring的事件需要遵循如下流程：

- 自定义事件，集成`ApplicationEvent`
- 定义事件监听器，实现`ApplicationListener`
- 使用容器发布事件
<a name="WwpXL"></a>
## 3、面向切面编程（AOP）
`AOP` 全称为：Aspect Oriented Programming。`AOP`是一种编程思想，其核心构造是方面（切面），即将那些影响多个类的公共行为封装到可重用的模块中，而使原本的模块内只需关注自身的个性化行为。<br />`AOP` 编程的常用场景有：Authentication（权限认证）、Auto Caching（自动缓存处理）、Error Handling（统一错误处理）、Debugging（调试信息输出）、Logging（日志记录）、Transactions（事务处理）等。
<a name="d9c6d7e7"></a>
### AOP的两类实现
<a name="mUPBQ"></a>
#### 静态AOP实现
 AOP框架在编译阶段对程序进行修改，即实现对目标类的增强，生成静态的AOP代理类，以AspectJ为代表
<a name="tnZhl"></a>
#### 动态AOP实现
AOP框架在运行阶段动态生成AOP代理，以实现对目标对象的增强，以Spring AOP为代表
<a name="QEku9"></a>
### 通知(Advice)
<a name="X36Jr"></a>
#### Spring切面可以应用5种类型的通知

1. 前置通知（Before）：在目标方法被调用之前调用通知功能；
2. 后置通知（After）：在目标方法完成之后调用通知，此时不会关心方法的输出是什么；
3. 返回通知（After-returning）：在目标方法成功执行之后调用通知；
4. 异常通知（After-throwing）：在目标方法抛出异常后调用通知；
5. 环绕通知（Around）：通知包裹了被通知的方法，在被通知的方法调用之前和调用之后执行自定义的行为。

对应注解：

| 注　　解 | 通　　知 |
| --- | --- |
| `@After` | 通知方法会在目标方法返回或抛出异常后调用 |
| `@AfterReturning` | 通知方法会在目标方法返回后调用 |
| `@AfterThrowing` | 通知方法会在目标方法抛出异常后调用 |
| `@Around` | 通知方法会将目标方法封装起来 |
| `@Before` | 通知方法会在目标方法调用之前执行 |

<a name="enfPG"></a>
#### 连接点（Join point）
连接点是在应用执行过程中能够插入切面的一个点。这个点可以是调用方法时、抛出异常时、甚至修改一个字段时。切面代码可以利用这些点插入到应用的正常流程之中，并添加新的行为。
<a name="omGLr"></a>
#### 切点（Pointcut）
如果说通知定义了切面的“什么”和“何时”的话，那么切点就定义了“何处” 。切点的定义会匹配通知所要织入的一个或多个连接点。通常使用明确的类和方法名称，或是利用正则表达式定义所匹配的类和方法名称来指定这些切点。有些AOP框架允许创建动态的切点，可以根据运行时的决策（比如方法的参数值）来决定是否应用通知。
<a name="gKREJ"></a>
#### 切面（Aspect）
通知+切点=切面
<a name="WEpe7"></a>
#### 引入（Introduction）
引入允许向现有的类添加新方法或属性
<a name="trEix"></a>
#### 织入（Weaving）
织入是把切面应用到目标对象并创建新的代理对象的过程。切面在指定的连接点被织入到目标对象中。在目标对象的生命周期里有多个点可以进行织入：

1. 编译期：切面在目标类编译时被织入。这种方式需要特殊的编译器。AspectJ的织入编译器就是以这种方式织入切面的。
2. 类加载期：切面在目标类加载到JVM时被织入。这种方式需要特殊的类加载器（`ClassLoader`），它可以在目标类被引入应用之前增强该目标类的字节码。AspectJ 5的加载时织入（load-time weaving，LTW）就支持以这种方式织入切面。
3. 运行期：切面在应用运行的某个时刻被织入。一般情况下，在织入切面时，AOP容器会为目标对象动态地创建一个代理对象。Spring AOP就是以这种方式织入切面的。
<a name="rzFgp"></a>
### Spring对AOP的支持

1. 基于代理的经典Spring AOP；
2. 纯POJO切面（4.x版本需要XML配置）；
3. `@AspectJ`注解驱动的切面；
4. 注入式AspectJ切面（适用于Spring各版本）。
```java
public interface Performance(){
 	 public void perform();
}
```
现在来写一个切点表达式，这个表达式能够设置当`perform()`方法执行时触发通知的调用。
```java
execution(* concert.Performance.perform(..))
//execution：在方法执行时触发
//*：返回任意类型
//concert.Performance：方法所属类
//perform：方法名
//(..)：使用任意参数
不仅如此，还可以写的更复杂一点

execution(* concert.Performance.perform(..)&&within(concert.*))
//增加了一个与操作，当concert包下的任意类方法被调用时也会触发
在切点中选择bean

execution(*concert.Performance.perform()) and bean('woodstock')
//限定bean id为woodstock
```
完整的切面如下：
```java
@Aspect
public class Audience{
      @Before("execution(**concert.Performance.perform(..))")
      public void silenceCellPhones(){
        	System.out.println("Silencing cell phones");
      }
      @Before("execution{** concert.Performance.perform{..}}")
      public void taskSeats(){
        	System.out.println("Talking seats");
      }
      @AfterReturning("execution{** concert.Performance.perform{..}}")
      public void applause(){
        	System.out.println("CLAP CLAP CLAP!!!");
      }
      @AfterThrowing("execution{** concert.Performance.perform{..}}")
      public void demanRefund(){
       		System.out.println("Demanding a refund");
      }
}
```
简化后
```java
@Aspect
public class Audience{
      //避免频繁使用切点表达式
      @Pointcut("execution(** concert.Performance.perform(..))")
      public void performance(){}

      @Before("performance()")
      public void silenceCellPhones(){
        System.out.println("Silencing cell phones");
      }
      @Before("performance()")
      public void taskSeats(){
        System.out.println("Talking seats");
      }
      @AfterReturning("performance()")
      public void applause(){
        System.out.println("CLAP CLAP CLAP!!!");
      }
      @AfterThrowing("performance()")
      public void demanRefund(){
        System.out.println("Demanding a refund");
      }
}
```
<a name="xPGxe"></a>
#### XML中声明切面
| AOP配置元素 | 用途 |
| --- | --- |
| `<aop:advisor>` | 定义AOP通知器 |
| `<aop:after>` | 定义AOP后置通知（不管被通知的方法是否执行成功） |
| `<aop:after-returning>` | 定义AOP返回通知 |
| `<aop:after-throwing>` | 定义AOP异常通知 |
| `<aop:around>` | 定义AOP环绕通知 |
| `<aop:aspect>` | 定义一个切面 |
| `<aop:aspectj-autoproxy>` | 启用`@AspectJ`<br />注解驱动的切面 |
| `<aop:before>` | 定义一个AOP前置通知 |
| `<aop:config>` | 顶层的AOP配置元素。大多数的`<aop:*>`<br />元素必须包含在`<aop:config>`<br />元素内 |
| `<aop:declare-parents>` | 以透明的方式为被通知的对象引入额外的接口 |
| `<aop:pointcut>` | 定义一个切点 |

<a name="kpgnb"></a>
#### 定义切面
```java
public class Audience{
      public void silenceCellPhones(){
        System.out.println("Silencing cell phones");
      }
      public void taskSeats(){
        System.out.println("Talking seats");
      }
      public void applause(){
        System.out.println("CLAP CLAP CLAP!!!");
      }
      public void demandRefund(){
        System.out.println("Demanding a refund");
      }
}
```
<a name="ZewpR"></a>
#### 通过xml配置切面
```xml
<aop:config>
  <aop:aspect ref="audience">
    <aop:before
      pointcut ="execution(** concert.Performance.perform(..))"
      method="sillenceCellPhones"/>
    <aop:before
      pointcut ="execution(** concert.Performance.perform(..))"
      method="taskSeats"/>
    <aop:after-returning
      pointcut ="execution(** concert.Performance.perform(..))"
      method="applause"/>
    <aop:After-throwing
        pointcut ="execution(** concert.Performance.perform(..))"
        method="demanRefund"/>
  </aop:aspect>
</aop:config>
```
AspectJ关于Spring AOP的AspectJ切点，最重要的一点就是Spring仅支持AspectJ切点指示器（pointcut designator）的一个子集。Spring是基于代理的，而某些切点表达式是与基于代理的AOP无关的。下表列出了          Spring AOP所支持的AspectJ切点指示器。<br />Spring借助AspectJ的切点表达式语言来定义Spring切面

| AspectJ指示器 | 描　　述 |
| --- | --- |
| `arg()` | 限制连接点匹配参数为指定类型的执行方法 |
| `@args()` | 限制连接点匹配参数由指定注解标注的执行方法 |
| `execution()` | 用于匹配是连接点的执行方法 |
| `this()` | 限制连接点匹配AOP代理的bean引用为指定类型的类 |
| `target` | 限制连接点匹配目标对象为指定类型的类 |
| `@target()` | 限制连接点匹配特定的执行对象，这些对象对应的类要具有指定类型的注解 |
| `within()` | 限制连接点匹配指定的类型 |
| `@within()` | 限制连接点匹配指定注解所标注的类型（当使用Spring AOP时，方法定义在由指定的注解所标注的类里） |
| `@annotation` | 限定匹配带有指定注解的连接点 |

<a name="hFQAE"></a>
## 4、Spring高级特性
由于Spring特殊的依赖注入技巧，导致Bean之间没有耦合度。<br />但是Bean有时需要使用spring容器本身的资源，这时Bean必须意识到Spring容器的存在。所以得使用Spring Aware，下面来看看Spring Aware提供的接口

| `BeanNameAware` | 获得到容器中Bean的名称 |
| --- | --- |
| `BeanFactory` | 获得当前的bean factory，这样可以调用容器的服务 |
| `ApplicationContextAware*` | 当前application context，这样可以调用容器的服务 |
| `MessageSourceAware` | 获得Message source |
| `ApplicationEventPublisherAware` | 应用时间发布器，可以发布时间， |
| `ResourceLoaderAware` | 获得资源加载器，可以获得外部资源文件 |

<a name="utO7x"></a>
### `@TaskExecutor`
这样可以实现多线程和并发编程。通过`@EnableAsync`开启对异步任务的支持，并通过实际执行的Bean的方法始中使用`@Async`注解来声明其是一个异步任务
<a name="1ae7fbf7"></a>
### `@Scheduled` 计划任务
首先通过在配置类注解`@EnableScheduling`来开启对计划任务的支持，然后在要执行计划任务的方法上注解`@Scheduled`，声明这是一个计划任务
<a name="UD5ir"></a>
### `@Conditional`
根据满足某一个特定条件创建一个特定的Bean。
<a name="JlrCe"></a>
### 组合注解与元注解
元注解就是可以注解到别的注解上的注解，被注解的注解称之为组合注解，组合注解具备注解其上的元注解的功能。
<a name="i1Cga"></a>
### `@Enable*`注解的工作原理
通过观察这些`@Enable*`注解的源码，可以发现所有的注解都有一个`@Import`注解，`@Import`是用来导入配置类的，这也就意外着这些自动开启的实现其实是导入了一些自动配置的Bean。这些导入配置的方式主要范围以下三种类型：

1. 第一类：直接导入配置类
2. 第二类：依据条件选择配置类
3. 第三类：动态注册Bean
<a name="FdKES"></a>
## 5、Spring的核心组件
<a name="EFxRr"></a>
### 核心组件
Spring 框架中的核心组件只有三个：Core、Context 和 Bean<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565057834199-27c7026d-de6f-452a-a4de-6a448fd961a7.png#averageHue=%237399d3&height=323&id=NkAqL&originHeight=887&originWidth=1585&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48188&status=done&style=none&title=&width=576.3636363636364)
<a name="XeyjJ"></a>
### 解析核心组件
<a name="HlhY9"></a>
#### Bean
Bean 组件在 Spring 的 org.springframework.beans 包下。这个包下的所有类主要解决了三件事：Bean 的定义、Bean 的创建以及对 Bean 的解析。<br />Spring Bean 的创建时典型的工厂模式，他的顶级接口是 `BeanFactory`，下图是这个工厂的继承层次关系<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565056032949-a4ad8564-3a19-4c12-8b6b-034c34dc6385.png#averageHue=%23fbfbfb&height=416&id=WYwJ3&originHeight=1143&originWidth=2639&originalType=binary&ratio=1&rotation=0&showTitle=false&size=153474&status=done&style=shadow&title=&width=959.6363636363636)<br />BeanFactory 有三个子类：`ListableBeanFactory`、`HierarchicalBeanFactory` 和 `AutowireCapableBeanFactory`。但是从上图中可以发现最终的默认实现类是 `DefaultListableBeanFactory`，查阅这些接口的源码和说明发现，每个接口都有他使用的场合，它主要是为了区分在 Spring 内部在操作过程中对象的传递和转化过程中，对对象的数据访问所做的限制。例如 `ListableBeanFactory` 接口表示这些 Bean 是可列表的，而 `HierarchicalBeanFactory` 表示的是这些 Bean 是有继承关系的，也就是每个 Bean 有可能有父 Bean。`AutowireCapableBeanFactory` 接口定义 Bean 的自动装配规则。这四个接口共同定义了 Bean 的集合、Bean 之间的关系、以及 Bean 行为。
<a name="Wapm4"></a>
#### Context
`ApplicationContext` 是 Context 的顶级父类，他除了能标识一个应用环境的基本信息外，他还继承了五个接口，这五个接口主要是扩展了 Context 的功能。下面是 Context 的类结构图<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565056478174-c13a3f81-67ab-4326-8044-5047286ca33d.png#averageHue=%23fdfdfd&height=465&id=XE2cx&originHeight=1278&originWidth=3640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=153550&status=done&style=shadow&title=&width=1323.6363636363637)<br />从上图中可以看出 `ApplicationContext` 继承了 BeanFactory，这也说明了 Spring 容器中运行的主体对象是 Bean，另外 `ApplicationContext` 继承了 `ResourceLoader` 接口，使得 `ApplicationContext` 可以访问到任何外部资源，这将在 Core 中详细说明。<br />`ApplicationContext` 的子类主要包含两个方面：

- `ConfigurableApplicationContext` 表示该 Context 是可修改的，也就是在构建 Context 中用户可以动态添加或修改已有的配置信息，它下面又有多个子类，其中最经常使用的是可更新的 Context，即 `AbstractRefreshableApplicationContext`类。
- `WebApplicationContext` 顾名思义，就是为 web 准备的 Context 他可以直接访问到 `ServletContext`，通常情况下，这个接口使用的少。

再往下分就是按照构建 Context 的文件类型，接着就是访问 Context 的方式。这样一级一级构成了完整的 Context 等级层次。<br />总体来说 `ApplicationContext` 必须要完成以下几件事：

1. 标识一个应用环境
2. 利用 `BeanFactory` 创建 Bean 对象
3. 保存对象关系表
4. 能够捕获各种事件

Context 作为 Spring 的 IOC 容器，基本上整合了 Spring 的大部分功能，或者说是大部分功能的基础。
<a name="ykdNv"></a>
#### Core
Core 组件作为 Spring 的核心组件，他其中包含了很多的关键类，其中一个重要组成部分就是定义了资源的访问方式。这种把所有资源都抽象成一个接口的方式很值得在以后的设计中拿来学习。下面就重要看一下这个部分在 Spring 的作用。

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565057129461-b55c64bd-4af8-49a8-9b17-0f6ed97d2a21.png#averageHue=%23fdfdfd&height=542&id=Cr4xo&originHeight=1490&originWidth=3645&originalType=binary&ratio=1&rotation=0&showTitle=false&size=196610&status=done&style=shadow&title=&width=1325.4545454545455)<br />从上图可以看出 `Resource` 接口封装了各种可能的资源类型，也就是对使用者来说屏蔽了文件类型的不同。对资源的提供者来说，如何把资源包装起来交给其他人用这也是一个问题，看到 `Resource` 接口继承了 `InputStreamSource` 接口，这个接口中有个 `getInputStream` 方法，返回的是 `InputStream` 类。这样所有的资源都被可以通过 `InputStream` 这个类来获取，所以也屏蔽了资源的提供者。另外还有一个问题就是加载资源的问题，也就是资源的加载者要统一，从上图中可以看出这个任务是由 `ResourceLoader` 接口完成，他屏蔽了所有的资源加载者的差异，只需要实现这个接口就可以加载所有的资源，他的默认实现是 `DefaultResourceLoader`。<br />Context 是把资源的加载、解析和描述工作委托给了 `ResourcePatternResolver` 类来完成，他相当于一个接头人，他把资源的加载、解析和资源的定义整合在一起便于其他组件使用。Core 组件中还有很多类似的方式。
