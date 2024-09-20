Spring 设计模式
<a name="tq6KJ"></a>
## 代理模式
所谓代理，是指它与被代理对象实现了相同的接口，客户端必须通过代理才能与被代理的目标类进行交互，而代理一般在交互的过程中（交互前后），进行某些特定的处理，比如在调用这个方法前做前置处理，调用这个方法后做后置处理。<br />代理又分为静态代理和动态代理两种方式，Spring的AOP采用的是动态代理的方式<br />Spring通过动态代理对类进行方法级别的切面增强，动态生成目标对象的代理类，并在代理类的方法中设置拦截器，通过执行拦截器中的逻辑增强了代理方法的功能，从而实现AOP。
<a name="B3Osu"></a>
## 策略模式
Spring AOP是通过动态代理来实现的。<br />具体到代码实现，Spring支持两种动态代理实现方式，一种是JDK提供的动态代理实现方式，另一种是Cglib提供的动态代理实现方式。<br />Spring会在运行时动态地选择不同的动态代理实现方式。这个应用场景实际上就是策略模式的典型应用场景。<br />只需要定义一个策略接口，让不同的策略类都实现这一个策略接口。对应到Spring源码，AopProxy是策略接口，`JdkDynamicAopProxy`、`CglibAopProxy`是两个实现了`AopProxy`接口的策略类。<br />其中，AopProxy接口的定义如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727786769-8eaf2c30-dfb9-4640-ae9e-293ae0714f23.webp#clientId=uedb34ee1-c75a-4&from=paste&id=u2387f346&originHeight=516&originWidth=908&originalType=url&ratio=1&status=done&style=none&taskId=ua56ddc4e-a2cf-468c-a7a6-b7519fa780b)<br />在策略模式中，策略的创建一般通过工厂方法来实现。对应到Spring源码，`AopProxyFactory`是一个工厂类接口，`DefaultAopProxyFactory`是一个默认的工厂类，用来创建AopProxy对象。<br />源码如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727786821-fdfd1834-16fa-4645-9525-2cc72c885ab8.webp#clientId=uedb34ee1-c75a-4&from=paste&id=ufe824fe7&originHeight=766&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uad4f307b-fbcc-4069-a4a5-c513b753473)<br />策略模式的典型应用场景，一般是通过环境变量、状态值、计算结果等动态地决定使用哪个策略。<br />对应到Spring源码中，可以参看刚刚给出的`DefaultAopProxyFactory`类中的`createAopProxy()`函数的代码实现。<br />其中，第10行代码是动态选择哪种策略的判断条件。
<a name="mJrAM"></a>
## 装饰器模式
缓存一般都是配合数据库来使用的。如果写缓存成功，但数据库事务回滚了，那缓存中就会有脏数据。<br />为了解决这个问题，需要将缓存的写操作和数据库的写操作，放到同一个事务中，要么都成功，要么都失败。<br />实现这样一个功能，Spring使用到了装饰器模式。<br />`TransactionAwareCacheDecorator`增加了对事务的支持，在事务提交、回滚的时候分别对Cache的数据进行处理。<br />`TransactionAwareCacheDecorator`实现Cache接口，并且将所有的操作都委托给`targetCache`来实现，对其中的写操作添加了事务功能。这是典型的装饰器模式的应用场景和代码实现。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727786809-3968b1f9-6264-4c1c-a189-f9071947e1a9.webp#clientId=uedb34ee1-c75a-4&from=paste&id=u361dfafc&originHeight=1658&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uc9ed8c90-d248-485f-9bff-efcf1813359)
<a name="M8kn1"></a>
## 单例模式
单例模式是指一个类在整个系统运行过程中，只允许产生一个实例<br />在Spring中，Bean可以被定义为两种模式：`Prototype`（多例）和`Singleton`（单例），Spring Bean默认是单例模式。<br />那Spring是如何实现单例模式的呢？<br />答案是通过单例注册表的方式，具体来说就是使用了HashMap。简化代码如下：
```java
public class DefaultSingletonBeanRegistry {
    
    //使用了线程安全容器ConcurrentHashMap，保存各种单实例对象
    private final Map singletonObjects = new ConcurrentHashMap;

    protected Object getSingleton(String beanName) {
    //先到HashMap中拿Object
    Object singletonObject = singletonObjects.get(beanName);
    
    //如果没拿到通过反射创建一个对象实例，并添加到HashMap中
    if (singletonObject == null) {
      singletonObjects.put(beanName,
                           Class.forName(beanName).newInstance());
   }
   
   //返回对象实例
   return singletonObjects.get(beanName);
  }
}
```
上面的代码逻辑比较清晰，先到HashMap去拿单实例对象，没拿到就创建一个添加到HashMap。
<a name="gohxG"></a>
## 简单工厂模式
有这样一个场景：<br />当A对象需要调用B对象的方法时，需要在A中new一个B的实例，它的缺点是一旦需求发生变化，比如需要使用C类来代替B时，就要改写A类的方法。<br />假如应用中有100个类以的方式耦合了B，那改起来就费劲了。<br />使用简单工厂模式：<br />简单工厂模式又叫静态工厂方法，其实质是由一个工厂类根据传入的参数，动态决定应该创建哪一个产品类。<br />其中Spring中的BeanFactory就是简单工厂模式的体现，BeanFactory是Spring IOC容器中的一个核心接口，它的定义如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727786789-c5503f8f-0efb-44ef-a1e0-b64023874da9.webp#clientId=uedb34ee1-c75a-4&from=paste&id=uaea5ac7a&originHeight=686&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u3202d70b-8519-4f9b-bbad-2e746201f54)<br />可以通过它的具体实现类（比如`ClassPathXmlApplicationContext`）来获取Bean：
```java
BeanFactory bf = new ClassPathXmlApplicationContext("spring.xml");
FlyFish flyFishBean = (FlyFish) bf.getBean("flyfishBean");
```
从上面代码可以看到，使用者不需要自己来new对象，而是通过工厂类的方法getBean来获取对象实例，这是典型的简单工厂模式，只不过Spring是用反射机制来创建Bean的。
<a name="xiffz"></a>
## 工厂方法模式
在简单工厂中，由工厂类进行所有的逻辑判断、实例创建；如果不想在工厂类中进行判断，可以为不同的产品提供不同的工厂，不同的工厂生产不同的产品，每一个工厂都只对应一个相应的对象，这就是工厂方法模式。<br />Spring中的`FactoryBean`就是这种思想的体现，`FactoryBean`可以理解为工厂Bean，先来看看它的定义：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727786830-6ff1a154-032f-4d63-b35e-4f0bff1913a2.webp#clientId=uedb34ee1-c75a-4&from=paste&id=u680ee90a&originHeight=552&originWidth=806&originalType=url&ratio=1&status=done&style=none&taskId=uc597342e-c8a3-4fd8-a2f4-7b2c72b712b)<br />定义一个类`FlyFishFactoryBean`来实现FactoryBean接口，主要是在`getObject`方法里new一个FlyFish对象。这样通过`getBean(id)` 获得的是该工厂所产生的FlyFish的实例，而不是`FlyFishFactoryBean`本身的实例，像下面这样：
```java
BeanFactory bf = new ClassPathXmlApplicationContext("spring.xml");
FlyFish flyFishBean = (FlyFish) bf.getBean("flyfishBean");
```
<a name="zBQm2"></a>
## 观察者模式
Spring中实现的观察者模式包含三部分：`Event`事件（相当于消息）、`Listener`监听者（相当于观察者）、`Publisher`发送者（相当于被观察者）<br />通过一个例子来看下Spring提供的观察者模式是怎么使用的
```java
// Event事件
public class DemoEvent extends ApplicationEvent {
    private String message;

    public DemoEvent(Object source, String message) {
        super(source);
    }

    public String getMessage() {
        return this.message;
    }
}

// Listener监听者
@Component
public class DemoListener implements ApplicationListener {
    @Override
    public void onApplicationEvent(DemoEvent demoEvent) {
        String message = demoEvent.getMessage();
        System.out.println(message);
    }
}

// Publisher发送者
@Component
public class DemoPublisher {
    @Autowired
    private ApplicationContext applicationContext;

    public void publishEvent(DemoEvent demoEvent) {
        this.applicationContext.publishEvent(demoEvent);
    }
}
```
从代码中，可以看出，主要包含三部分工作：

- 定义一个继承`ApplicationEvent`的事件（`DemoEvent`）；
- 定义一个实现了`ApplicationListener`的监听器（`DemoListener`）；
- 定义一个发送者（DemoPublisher），发送者调用`ApplicationContext`来发送事件消息。

在Spring的实现中，观察者注册到了哪里呢？又是如何注册的呢？<br />Spring把观察者注册到了`ApplicationContext`对象中。<br />实际上，具体到源码来说，`ApplicationContext`只是一个接口，具体的代码实现包含在它的实现类`AbstractApplicationContext`中。把跟观察者模式相关的代码，如下。只需要关注它是如何发送事件和注册监听者就好。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727787257-9c85b413-f90d-4c3f-9005-979e55906143.webp#clientId=uedb34ee1-c75a-4&from=paste&id=ub6d41993&originHeight=1829&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uda6f94ab-93e0-412e-8dfd-978d1828868)<br />从上面的代码中，可以发现，真正的消息发送，实际上是通过`ApplicationEventMulticaster`这个类来完成的。<br />下面这个类的源码只摘抄了最关键的一部分，也就是`multicastEvent()`这个消息发送函数，它通过线程池，支持异步非阻塞、同步阻塞这两种类型的观察者模式。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727787338-987d3bee-8bb0-4a50-9159-97284b1c0691.webp#clientId=uedb34ee1-c75a-4&from=paste&id=ua8ffaa7b&originHeight=658&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=udb1e40ca-d946-408a-ae61-1582f438d4a)<br />借助Spring提供的观察者模式的骨架代码，如果要在Spring下实现某个事件的发送和监听，只需要做很少的工作，定义事件、定义监听器、往`ApplicationContext`中发送事件就可以了，剩下的工作都由Spring框架来完成。<br />实际上，这也体现了Spring框架的扩展性，也就是在不需要修改任何代码的情况下，扩展新的事件和监听。
<a name="eeA7F"></a>
## 模板模式
经常在面试中被问到的一个问题：<br />请说下Spring Bean的创建过程包含哪些主要的步骤。<br />这其中就涉及模板模式。它也体现了Spring的扩展性。利用模板模式，Spring能让用户定制Bean的创建过程。<br />下面是Spring Bean的整个生命周期，一张图，清晰明了：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727787325-14172030-5634-452d-a1f4-71a7f36f1dee.webp#clientId=uedb34ee1-c75a-4&from=paste&id=uf600c853&originHeight=918&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=udb351ed9-c3ec-42d0-82e7-9566872c79e)<br />如果仔细看过源码会发现，实际上，这里的模板模式的实现，并不是标准的抽象类的实现方式，而是有点类似Callback回调的实现方式，也就是将要执行的函数封装成对象（比如，初始化方法封装成`InitializingBean`对象），传递给模板（BeanFactory）来执行。<br />观察者模式和模板模式，这两种模式能够帮助创建扩展点，让框架的使用者在不修改源码的情况下，基于扩展点定制化框架功能。
<a name="Ajhrn"></a>
## 适配器模式
在Spring MVC中，定义一个Controller最常用的方式是，通过`@Controller`注解来标记某个类是Controller类，通过`@RequesMapping`注解来标记函数对应的URL<br />不过，还可以通过让类实现Controller接口或者Servlet接口，来定义一个Controller。<br />针对这三种定义方式，写了三段示例代码，如下所示：
```java
// 方法一：通过@Controller、@RequestMapping来定义
@Controller
public class DemoController {
    @RequestMapping("/FlyFish")
    public ModelAndView getEmployeeName() {
        ModelAndView model = new ModelAndView("FlyFish");        
        model.addObject("message", "FlyFish");       
        return model; 
    }  
}

// 方法二：实现Controller接口 + xml配置文件:配置DemoController与URL的对应关系
public class DemoController implements Controller {
    @Override
    public ModelAndView handleRequest(HttpServletRequest req, HttpServletResponse resp) throws Exception {
        ModelAndView model = new ModelAndView("FlyFish");
        model.addObject("message", "FlyFish");
        return model;
    }
}

// 方法三：实现Servlet接口 + xml配置文件:配置DemoController类与URL的对应关系
public class DemoServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        this.doPost(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.getWriter().write("Hello World.");
    }
}
```
在应用启动的时候，Spring容器会加载这些Controller类，并且解析出URL对应的处理函数，封装成Handler对象，存储到`HandlerMapping`对象中。当有请求到来的时候，`DispatcherServlet`从`HanderMapping`中，查找请求URL对应的Handler，然后调用执行Handler对应的函数代码，最后将执行结果返回给客户端。<br />但是，不同方式定义的Controller，其函数的定义（函数名、入参、返回值等）是不统一的。<br />`DispatcherServlet`调用的是`service()`方法，`DispatcherServlet`需要根据不同类型的Controller，调用不同的函数。<br />Spring利用适配器模式，将不同方式定义的Controller类中的函数，适配为统一的函数定义。<br />再具体看下Spring的代码实现。<br />Spring定义了统一的接口HandlerAdapter，并且对每种Controller定义了对应的适配器类。<br />这些适配器类包括：`AnnotationMethodHandlerAdapter`、`SimpleControllerHandlerAdapter`、`SimpleServletHandlerAdapter`等。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635727787277-b406cc33-c6f1-46bc-920d-cb806f6ae520.webp#clientId=uedb34ee1-c75a-4&from=paste&id=u6a878a3d&originHeight=1069&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u4e9f5c40-1305-41c4-9346-cfe9f7f1a62)<br />在`DispatcherServlet`类中，就不需要区分对待不同的Controller对象了，统一调用`HandlerAdapter`的`handle()`函数就可以了
