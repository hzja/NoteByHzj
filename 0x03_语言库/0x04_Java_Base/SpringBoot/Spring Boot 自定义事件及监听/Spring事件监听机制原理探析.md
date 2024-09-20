JavaSpring
<a name="oR0wv"></a>
## 前言
Spring中提供了一套默认的事件监听机制，在容器初始化时便使用了这套机制。同时，Spring也提供了事件监听机制的接口扩展能力，开发者基于此可快速实现自定义的事件监听功能。<br />Spring的事件监听机制是在JDK事件监听的基础上进行的扩展，也是在典型观察者模式上的进一步抽象和改进。所以，结合Spring的事件监听机制与观察者模式来学习，可以达到理论与实践的完美融合。<br />本篇文章就以观察者模式和Spring事件监听机制作为切入点，结合具体的实例来对两者进行系统的学习和实践。
<a name="L7Fyl"></a>
## 观察者模式
**观察者模式**（Observer Pattern），也叫作**发布-订阅模式**（Publish/Subscribe）。<br />无论是观察者模式，还是Spring的事件监听机制，本质上都是在定义对象间一对多的依赖关系，使得每当一个对象（被观察者/事件）改变状态时，所有依赖于它的对象（观察者/事件监听器）都会得到通知，并被自动更新。<br />观察者模式的优点在于：观察者和被观察者之间是抽象耦合，不管是新增观察者或是被观察者，都非常容易扩展。这也符合面向对象所倡导的“**开闭原则**”：**对扩展开放，对修改关闭**。<br />观察者模式适用于以下三类场景：

- 关联行为场景，而且关联是可拆分的。
- 事件多级触发场景。
- 跨系统的消息交换场景，比如消息队列的处理机制。

在使用的过程中，也要综合考虑开发效率和运行效率的问题。通常，一个被观察者会对应多个观察者，那么在开发和调试的过程中会有一定的复杂度。<br />同时，因为被观察者存在关联、多级拆分，也就是会有多个观察者，而Java消息的通知（和Spring的事件监听机制）默认是顺序执行的，如果其中一个观察者执行时间过长或卡死，势必会影响整体的效率。此时，就需要考虑异步处理。
<a name="s4Rvi"></a>
### 观察者的角色定义
观察者模式是一个典型的**发布-订阅**模型，其中主要涉及四个角色：

- 抽象被观察者角色：内部持有所有观察者角色的引用，并对外提供新增、移除观察者角色、通知所有观察者的功能；
- 具体被观察者角色：当状态变更时，会通知到所有的观察者角色；
- 抽象观察者角色：抽象具体观察者角色的一些共性方法，如状态变更方法；
- 具体观察者角色：实现抽象观察者角色的方法；

UML类图展示类观察者模式大体如下：<br />![观察者模式类图](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668500754341-38b4ddde-fa5d-4a3a-8202-250f6f4d22ad.jpeg#averageHue=%23f4f4f4&clientId=u8e372fdb-fb91-4&from=paste&id=ub113666f&originHeight=647&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u96eb54f4-6858-4abd-a7ee-9cad3bcf7a6&title=%E8%A7%82%E5%AF%9F%E8%80%85%E6%A8%A1%E5%BC%8F%E7%B1%BB%E5%9B%BE "观察者模式类图")<br />以具体的代码来展示一下观察者模式的实现。<br />第一，定义抽象观察者。
```java
/**
 * 抽象观察者角色
 **/
public abstract class AbstractObserver {

    /**
    * 接收消息
    * @param context 消息内容
    */
    public abstract void receiveMsg(String context);

}
```
第二，定义抽象被观察者。
```java
/**
 * 抽象主题（抽象被观察者角色）
 **/
public abstract class AbstractSubject {

    /**
    * 持有所有抽象观察者角色的集合引用
    */
    private final List<AbstractObserver> observers = new ArrayList<>();

    /**
    * 添加一个观察者
    * @param observer 观察者
    */
    public void addObserver(AbstractObserver observer){
        observers.add(observer);
    }

    /**
    * 移除一个观察者
    * @param observer 观察者
    */
    public void removeObserver(AbstractObserver observer){
        observers.remove(observer);
    }

    /**
    * 通知所有的观察者，执行观察者更新方法
    * @param context 通知内容
    */
    public void notifyObserver(String context){
        observers.forEach(observer -> observer.receiveMsg(context));
    }
}
```
第三，定义具体被观察者，实现了抽象被观察者。
```java
/**
* 具体被观察者
**/
public class ConcreteSubject extends AbstractSubject{

    /**
    * 被观察者发送消息
    * @param context 消息内容
    */
    public void sendMsg(String context){
        System.out.println("具体被观察者角色发送消息: " + context);
        super.notifyObserver(context);
    }
}
```
第四，定义具体观察者，实现了抽象观察者。
```java
/**
* 具体观察者角色实现类
**/
public class ConcreteObserver extends AbstractObserver{

    @Override
    public void receiveMsg(String context) {
        System.out.println("具体观察者角色接收消息: " + context);
    }
}
```
第五，使用演示类。
```java
public class ObserverPatternTest {

    public static void main(String[] args) {
        ConcreteSubject subject = new ConcreteSubject();
        subject.addObserver(new ConcreteObserver());
        subject.sendMsg("Hello World!");
    }
}
```
执行上述方法，控制台打印日志为：
```
具体被观察者角色发送消息: Hello World!
具体观察者角色接收消息: Hello World!
```
在上述代码实现中，被观察者发出消息后，观察者接收到具体的消息，如果添加了多个观察者，它们均会收到消息。也就是前面所说的，每当一个对象（被观察者/事件）改变状态时，所有依赖于它的对象（观察者/事件监听器）都会得到通知，并被自动更新。
<a name="vkobp"></a>
## Java中的事件机制
前面聊了观察者模式，这里再来看看Java中的事件机制。<br />在JDK 1.1及以后版本中，事件处理模型采用基于观察者模式的委派事件模型(DelegationEvent Model, DEM)，即一个Java组件所引发的事件并不由引发事件的对象自己来负责处理，而是委派给独立的事件处理对象负责。<br />这并不是说事件模型是基于Observer和Observable的，事件模型与Observer和Observable没有任何关系，Observer和Observable只是观察者模式的一种实现而已。<br />Java中的事件机制有三个角色参与：

- Event Source：事件源，发起事件的主体。
- Event Object：事件状态对象，传递的信息载体，可以是事件源本身，一般作为参数存在于listerner的方法之中。所有事件状态对象都将从Java中的EventObject派生而来；
- Event Listener：事件监听器，当监听到EventObject产生时，调用相应的方法进行处理。所有事件侦听器接口必须扩展EventListener接口；

UML类图展示类事件模式大体如下：<br />![事件监听](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668500754339-7455b619-be50-4509-a83d-eb4c91fa4a28.jpeg#averageHue=%23f5f5f5&clientId=u8e372fdb-fb91-4&from=paste&id=u3cf9c29b&originHeight=514&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ufdad9b83-85a8-47e9-aaa4-88f241766f6&title=%E4%BA%8B%E4%BB%B6%E7%9B%91%E5%90%AC "事件监听")<br />在上面的UML图中，`EventObject`一般作为`Listener`处理方法的参数传入，而`EventSource`是事件的触发者，通过此对象注册相关的`Listener`，然后向`Listener`触发事件。<br />通过UML图的对比可以看出，事件监听模式和观察者模式大同小异，它们属于同一类型模式，都属于回调机制，主动推送消息，但在使用场景上有所区别。<br />观察者(`Observer`)相当于事件监听者（监听器），被观察者(`Observable`)相当于事件源和事件，事件监听比观察者模式要复杂一些，多了`EventSource`角色的存在。<br />以具体的代码来展示一下Java中的事件机制实现。<br />第一，定义事件对象。
```java
/**
 * 事件对象
 *
 **/
public class DoorEvent extends EventObject {

    private int state;

    /**
    * Constructs a prototypical Event.
    *
    * @param source The object on which the Event initially occurred.
    * @throws IllegalArgumentException if source is null.
    */
    public DoorEvent(Object source) {
        super(source);
    }

    public DoorEvent(Object source, int state) {
        super(source);
        this.state = state;
    }

    // 省略getter/setter方法 
}
```
第二，定义事件监听器接口。
```java
/**
 * 事件监听器接口
 **/
public interface DoorListener extends EventListener {

    /**
    * 门处理事件
    * @param doorEvent 事件
    */
    void doorEvent(DoorEvent doorEvent);
}
```
第三，定义事件监听器的实现类。
```java
public class CloseDoorListener implements DoorListener{
    @Override
    public void doorEvent(DoorEvent doorEvent) {
        if(doorEvent.getState() == -1){
            System.out.println("门关上了");
        }
    }
}

public class OpenDoorListener implements DoorListener{
    @Override
    public void doorEvent(DoorEvent doorEvent) {
        if(doorEvent.getState() == 1){
            System.out.println("门打开了");
        }
    }
}
```
这里实现了门的开和关两个事件监听器类。<br />第四，定义事件源EventSource。
```java
public class EventSource {

    //监听器列表，监听器的注册则加入此列表
    private Vector<DoorListener> listenerList = new Vector<>();

    //注册监听器
    public void addListener(DoorListener eventListener) {
        listenerList.add(eventListener);
    }

    //撤销注册
    public void removeListener(DoorListener eventListener) {
        listenerList.remove(eventListener);
    }

    //接受外部事件
    public void notifyListenerEvents(DoorEvent event) {
        for (DoorListener eventListener : listenerList) {
            eventListener.doorEvent(event);
        }
    }
}
```
第五，测试类。
```java
public class EventTest {

    public static void main(String[] args) {
        EventSource eventSource = new EventSource();
        eventSource.addListener(new CloseDoorListener());
        eventSource.addListener(new OpenDoorListener());

        eventSource.notifyListenerEvents(new DoorEvent("关门事件", -1));
        eventSource.notifyListenerEvents(new DoorEvent("开门时间", 1));
    }
}
```
执行测试类，控制台打印：
```
门关上了
门打开了
```
事件成功触发。
<a name="YyVPX"></a>
## Spring中的事件机制
在了解了观察者模式和Java的事件机制之后，再来看看Spring中的事件机制。在Spring容器中，通过`ApplicationEvent`和`ApplicationListener`接口来实现事件监听机制。每次`Event`事件被发布到Spring容器中，都会通知对应的Listener。默认情况下，Spring的事件监听机制是同步的。<br />Spring的事件监听由三部分组成：

- 事件（`ApplicationEvent`)：该类继承自JDK中的`EventObject`，负责对应相应的监听器，事件源发生某事件是特定事件监听器被触发的原因；
- 监听器（`ApplicationListener`）：该类继承自JDK中的`EventListener`，对应于观察者模式中的观察者。监听器监听特定事件，并在内部定义了事件发生后的响应逻辑；
- 事件发布器（`ApplicationEventPublisher`）：对应于观察者模式中的被观察者/主题，负责通知观察者，对外提供发布事件和增删事件监听器的接口，维护事件和事件监听器之间的映射关系，并在事件发生时负责通知相关监听器。

通过上面的分析可以看出Spring的事件机制不仅是观察者模式的一种实现，也实现了JDK提供的事件接口。同时，除了发布者和监听者之外，还存在一个`EventMulticaster`的角色，负责把事件转发给监听者。<br />Spring事件机制的工作流程如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699685667506-52ad6b9a-dc70-4362-9a09-ac5648d1ab0a.jpeg)<br />Spring事件机制<br />在上述流程中，发布者调用`applicationEventPublisher.publishEvent(msg)`，将事件发送给`EventMultiCaster`。`EventMultiCaster`注册着所有的`Listener`，它会根据事件类型决定转发给那个Listener。<br />在Spring中提供了一些标准的事件，比如：`ContextRefreshEvent`、`ContextStartedEvent`、`ContextStoppedEvent`、`ContextClosedEvent`、`RequestHandledEvent`等。<br />关于Spring事件机制的具体实现和这些标准事件的作用，大家可以通过阅读源码来学习，这里不再详细展开。<br />下面来看看Spring事件机制涉及到的几个角色的源码及后续基于它们的实践。<br />第一，事件（`ApplicationEvent`）。
```java
public abstract class ApplicationEvent extends EventObject {

    /** use serialVersionUID from Spring 1.2 for interoperability. */
    private static final long serialVersionUID = 7099057708183571937L;

    /** System time when the event happened. */
    private final long timestamp;

    /**
    * Create a new {@code ApplicationEvent}.
    * @param source the object on which the event initially occurred or with
    * which the event is associated (never {@code null})
    */
    public ApplicationEvent(Object source) {
        super(source);
        this.timestamp = System.currentTimeMillis();
    }

    /**
    * Return the system time in milliseconds when the event occurred.
    */
    public final long getTimestamp() {
        return this.timestamp;
    }

}
```
事件可类比观察者中的被观察者实现类的角色，继承自JDK的`EventObject`。上述Spring中的标准事件都是直接或间接继承自该类。<br />第二，事件发布器（`ApplicationEventPublisher`）。
```java
@FunctionalInterface
public interface ApplicationEventPublisher {

    default void publishEvent(ApplicationEvent event) {
        publishEvent((Object) event);
    }

    void publishEvent(Object event);
}
```
通过实现`ApplicationEventPublisher`接口，并重写`publishEvent()`方法，可以自定义事件发布的逻辑。`ApplicationContext`继承了`ApplicationEventPublisher`接口。因此，可以通过实现`ApplicationContextAware`接口，注入`ApplicationContext`，然后通过`ApplicationContext`的`publishEvent()`方法来实现事件发布功能。<br />`ApplicationContext`容器本身仅仅是对外提供了事件发布的接口`publishEvent()`，真正的工作委托给了具体容器内部的`ApplicationEventMulticaster`对象。而`ApplicationEventMulticaster`对象可类比观察者模式中的抽象被观察者角色，负责持有所有观察者集合的引用、动态添加、移除观察者角色。<br />第三，事件监听器（`ApplicationListener`）。
```java
@FunctionalInterface
public interface ApplicationListener<E extends ApplicationEvent> extends EventListener {

    /**
    * Handle an application event.
    * @param event the event to respond to
    */
    void onApplicationEvent(E event);

}
```
事件监听器（`ApplicationListener`）对应于观察者模式中的具体观察者角色。当事件发布之后，就会执行事件监听器的逻辑。通过实现`ApplicationListener`接口，并重写`onApplicationEvent()`方法，就可以监听到事件发布器发布的事件。
<a name="k56As"></a>
## Spring事件监听案例
下面以具体的案例代码来说明如何自定义实现Spring事件监听。<br />第一，自定义定义事件对象，集成自`ApplicationEvent`。
```java
public class MyEvent extends ApplicationEvent {
    /**
    * Create a new {@code ApplicationEvent}.
    *
    * @param source the object on which the event initially occurred or with
    *               which the event is associated (never {@code null})
    */
    public MyEvent(Object source) {
        super(source);
    }

    private String context;

    public MyEvent(Object source, String context){
        super(source);
        this.context = context;
    }

    public String getContext() {
        return context;
    }

    public void setContext(String context) {
        this.context = context;
    }
}
```
第二，自定义`ApplicationListener`事件监听器。
```java
@Component
public class MyApplicationListener implements ApplicationListener<MyEvent> {
    @Override
    public void onApplicationEvent(MyEvent event) {
        // 监听到具体事件，处理对应具体逻辑
        System.out.println("event.getContext() = " + event.getContext());
    }

}
```
除了上述基于实现`ApplicationListener`接口的方式外，还可以使用`@EventListener`注解来实现，实现示例如下：
```java
@Component
public class MyApplicationListener{

    // 通过注解实现监听器
    @EventListener
    public void handleMyEvent(MyEvent event){
        // 监听到具体事件，处理对应具体逻辑
        System.out.println("event.getContext() = " + event.getContext());
    }
}
```
第三，使用及单元测试。
```java
@Slf4j
@SpringBootTest
public class SpringEventTest {

    @Autowired
    private ApplicationEventPublisher eventPublisher;

    @Test
    void testEvent() {
        eventPublisher.publishEvent(new MyEvent("自定义事件", "Hello World！"));
    }
}
```
执行单元测试，可看到控制台打印对应的事件信息。<br />通过上述方式已经成功实现了基于Spring的事件监听机制，但这其中还有一个问题：同步处理。默认情况下，上述事件是基于同步处理的，如果其中一个监听器阻塞，那么整个线程将处于等待状态。<br />那么，如何使用异步方式处理监听事件呢？只需两步即可。<br />第一步，在监听器类或方法上添加`@Async`注解，例如：
```java
@Component
@Async
public class MyApplicationListener implements ApplicationListener<MyEvent> {
    @Override
    public void onApplicationEvent(MyEvent event) {
        // 监听到具体事件，处理对应具体逻辑
        System.out.println("event.getContext() = " + event.getContext());
    }

}
```
第二步，在SpringBoot启动类（这里以SpringBoot项目为例）上添加`@EnableAsync`注解，例如：
```java
@SpringBootApplication
@EnableAsync
public class SpringBootMainApplication {

    public static void main(String[] args) {
        SpringApplication.run(SpringBootMainApplication.class, args);
    }
}
```
此时，就可以实现异步监听功能了。当然，`@Async`注解也可以指定已经配置好的线程池来处理异步请求，关于线程数的初始化这里就不再演示了。
<a name="p6jxY"></a>
## 小结
从观察者模式、Java事件机制延伸到Spring的事件监听机制，将三者融合在一起来讲解。通过这个案例，其实能够体会到一些经验性的知识，比如看似复杂的Spring事件监听机制实现只不过是观察者模式的一种实现，而其中又集成了Java的事件机制。这也就是所谓的融会贯通。<br />如果单纯的学习某一个设计模式，可能只会运用和识别它的简单实现，而实践中往往会对设计模式进行变种，甚至融合多种设计模式的优点于一体，这便是活学活用。
