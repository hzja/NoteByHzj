Java Spring
<a name="r7ZPp"></a>
## 事件通知机制
可以先将其理解为“**事件通知机制**”，即每当孙悟空将金箍棒敲在地上时，就相当于给土地发了一封 email 的通知，告诉他俺老孙来了，赶快出来接驾。当土地收到通知之后就会立即现身了。<br />大家都知道 Spring 已经提供好了**事件监听、订阅**的实现，接下来用代码来实现一下这个场景。<br />首先要定义一个事件，来记录下孙悟空敲地的动作。
```java
@Getter
public class MonkeyKingEvent extends ApplicationEvent {

    private MonkeyKing monkeyKing;

    public MonkeyKingEvent(MonkeyKing monkeyKing) {
        super("monkeyKing");
        this.monkeyKing = monkeyKing;
    }

}
```
其中 MonkeyKing 是定义好的孙悟空的实体类
```java
@Data
public class MonkeyKing {

    /**
     * 是否敲地，默认为否
     **/
    private boolean knockGround = false;

}
```
然后需要实现 `ApplicationListener` 来监听孙悟空敲地的动作。
```java
@Component
public class MyGuardianListener implements ApplicationListener<MonkeyKingEvent> {

    @Override
    public void onApplicationEvent(MonkeyKingEvent event) {
        boolean knockGround = event.getMonkeyKing().isKnockGround();
        if(knockGround){
            MyGuardian.appear();
        }else{
            MyGuardian.seclusion();
        }
    }
}
```
最后来验证下整个流程。
```java
@PostMapping
public void testEvent(@RequestParam boolean knockGround) {
    MonkeyKing monkeyKing = new MonkeyKing();
    monkeyKing.setKnockGround(knockGround);
    MonkeyKingEvent monkeyKingEvent = new MonkeyKingEvent(monkeyKing);
    //发布孙悟空敲地的动作事件
    applicationEventPublisher.publishEvent(monkeyKingEvent);
}
```
当调用`testEvent()`方法传入knockGround为 true 时，打印
```java
土地公公出现了
```
传入为false时，打印
```java
土地公公遁地了
```
这样就简单实现了“孙悟空召唤土地”的功能。
<a name="SKJb2"></a>
### 观察者模式
说是**事件通知机制**也好，**事件监听-订阅**的实现也罢，其实它内部的最终实现原理依赖的是观察者模式。看到这，先不要胆怯，不要觉得设计模式晦涩难懂、久攻不下。用通俗易懂的小故事来认识一下观察者模式。<br />故事是这样的，上边只说了孙悟空敲地的动作，但是是否还记得孙悟空将金箍棒往天上一指，便换来雷公电母、龙王等为其施法布雨？闭上双眼，与虎力大仙比试的场景仍历历在目。<br />由此可见，不光土地能收到孙悟空的通知，连雷公电母和龙王也是可以接收到的。在这里，把孙悟空比作主题，也就是大家说的被观察者和 Subject的概念，把雷公电母和龙王以及土地比作观察者。<br />以下是代码逻辑：<br />首先，定义一个主题的基础类，里边会记录所有订阅该主题的观察者列表，还包含了增加、删除以及通知观察者的方法。
```java
public class Subject {

    //观察者列表
    private Vector<Observer> vector = new Vector();

    /**
     * 增加观察者
     **/
    public void addObserver(Observer observer){
        vector.add(observer);
    }

    /**
     *  删除观察者
     **/
    public void deleteObserver(Observer observer){
        vector.remove(observer);
    }

    /**
     *  通知所有观察者
     **/
    public void notifyObserver(String goldenCudgel) {
        for(Observer observer : vector) {
             observer.update(goldenCudgel);
         }
    }

}
```
然后，定义一个观察者的接口，包含观察者收到通知之后的“动作”。
```java
public interface Observer {
    void update(String goldenCudgel);
}
```
这时候再分别定义出“土地”、“雷公电母”、“龙王”的观察者实体类，实现具体的打雷下雨等动作。<br />**“雷公电母”、“龙王”等实现与“土地”类似，故此处仅展示观察者“土地”。**
```java
@Component
public class MyGuardianObserver implements Observer {

    @Override
    public void update(String goldenCudgel) {
        if(upGoldenCudgel(goldenCudgel)) {
            System.out.println("土地公公出现了");
        }
    }

    public boolean upGoldenCudgel(String goldenCudgel){
        if(Objects.equals(goldenCudgel,"down")){
            return true;
        }
        return false;
    }

}
```
接着，就可以定义被观察者的具体实现类“孙悟空”了
```java
public class MonkeyKingSubject extends Subject{
    
    public void doGoldenCudgel(String goldenCudgel){
        notifyObserver(goldenCudgel);
    }

}
```
最后来做个测试看看他们能不能响应孙悟空的通知。
```java
@PostMapping
public void observerTest(){
    MonkeyKingSubject subject = new MonkeyKingSubject();
    subject.addObserver(new ThunderGodObserver());
    subject.addObserver(new MyGuardianObserver());
    subject.addObserver(new DragonKingObserver());

    subject.doGoldenCudgel("up");
    System.out.println("我是分割线-----------------------------");
    subject.doGoldenCudgel("down");
}
```
结果展示
```java
雷公电母发出电闪雷鸣
龙王前来下雨
我是分割线-----------------------------
土地公公出现了
```
<a name="N4JQo"></a>
### 总结
观察者模式与事件通知机制都是在一对多的关系中，当一个对象被修改时，则会自动通知依赖它的对象，两者之间相互独立，互相解耦，这样既省去了反复检索状态的资源消耗，也能够得到最高的反馈速度。<br />当然它的缺点也不容忽视：

1. 如果一个被观察者对象有很多的直接和间接的观察者的话，将所有的观察者都通知到会花费很多时间;
2. 如果在观察者和观察目标之间有循环依赖的话，观察目标会触发它们之间进行循环调用，可能导致系统崩溃;
3. 观察者模式没有相应的机制让观察者知道所观察的目标对象是怎么发生变化的，而仅仅只是知道观察目标发生了变化;
<a name="XYI3q"></a>
## 源码解析
版本号：spring-framework-5.0.x<br />为了实现广播与监听的功能，Spring提供了两个重要的函数式接口：`ApplicationEventPublisher`和`ApplicationListener`。前者的`publishEvent()`方法提供了发送广播的能力；后者的`onApplicationEvent()`方法提供了监听并处理事件的能力。<br />接下来就来分析一下Spring是如何运用这两种能力的。<br />不知道大家对单例对象的初始化调用过程是否熟悉？主要调用方法流程如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649491848683-f7bda146-9155-4514-803b-02a7437617b6.png#clientId=u28a24c69-1d6a-4&from=paste&id=u5cfcb9d2&originHeight=616&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u76ea515b-0b15-4147-9cda-c561891eb25&title=)
<a name="yIzCx"></a>
### 发送广播
`applyBeanPostProcessorsBeforeInitialization`方法会去遍历该工厂创建的所有的Bean后置处理器，然后去依次执行后置处理器对应的`postProcessBeforeInitialization`方法。<br />在该方法的实现类中看到了两个熟悉的类名<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649491848679-15f3167d-fb20-4eb6-abf7-3b4139a0f608.png#clientId=u28a24c69-1d6a-4&from=paste&id=ud786fc98&originHeight=533&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufdd90325-d720-495a-9fa5-5fdf0d81ceb&title=)<br />这俩类是在`beanFactory`的准备工作过程中添加的两个bean的后置处理器，所以这个地方会依次去执行这两个类中的实现方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649491848693-395ee507-4e56-4cf4-aa46-19048ea2d45c.png#clientId=u28a24c69-1d6a-4&from=paste&id=u1c603717&originHeight=423&originWidth=1046&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u53e7b6f7-f27e-49bb-b4f1-7f9c9d9bf83&title=)<br />由于**蓝框**中类的实现方法是默认实现按照原样返回的给定的bean，所以此处不用过多分析，重点来看下**红框**中类的方法实现。<br />该方法中最重要的是`invokeAwareInterfaces`方法，它的作用是检测对应的bean是否实现了某个`Aware`接口，如果实现了的话就去进行相关的调用。
```java
if (bean instanceof ApplicationEventPublisherAware) {
    ((ApplicationEventPublisherAware) bean).setApplicationEventPublisher(this.applicationContext);
}
```
可以发现在`invokeAwareInterfaces`方法中出现了如上代码，这不就是和广播发送相关的吗？所以只要写一个类来实现`ApplicationEventPublisherAware`接口，就可以在该bean中注入一个`ApplicationEventPublisher`对象，也就获得了发送广播的能力。
<a name="SDkoL"></a>
### 监听消息
`applyBeanPostProcessorsAfterInitialization`方法**也**会去遍历该工厂创建的所有的Bean后置处理器，然后去依次执行后置处理器对应的`postProcessAfterInitialization`方法。<br />同样的，该方法的实现类中也有`ApplicationContextAwareProcessor`和`ApplicationListenerDetector`两个类，但是不同的是，前者的类的实现方法是默认实现按照原样返回的给定bean，而后者做了相关的处理。
```java
this.applicationContext.addApplicationListener((ApplicationListener<?>) bean);
```
上述代码是将实现了`ApplicationListener`接口的`bean`添加到监听器列表中，最终是保存在`AbstractApplicationEventMulticaster`的成员变量`defaultRetriever`的集合`applicationListeners`中。<br />猜想：当发送广播消息时，就直接找到集合中的这些监听器，然后调用每个监听器的`onApplicationEvent`方法完成事件的处理。
<a name="gkTHW"></a>
### 案例分析
在`refresh()`的`finishRefresh()`方法中，
```java
publishEvent(new ContextRefreshedEvent(this));
```
发送一条事件类型为`ContextRefreshedEvent`的广播消息，用来代表Spring容器初始化结束。通过分析发现，该方法中最主要的就是如下代码：
```java
//真正的广播交给 applicationEventMulticaster 来完成
getApplicationEventMulticaster().multicastEvent(applicationEvent, eventType);
```
`refresh()`的`initApplicationEventMulticaster()`将`applicationEventMulticaster`初始化为`SimpleApplicationEventMulticaster`<br />在实现类`SimpleApplicationEventMulticaster`的方法中，会找到已注册的`ApplicationListener`列表，然后分别调用`invokeListener`方法（将监听和事件作为参数传到方法并执行的过程就是发送广播的过程）。<br />底层调用的是`listener.onApplicationEvent(event);`方法，也就是各个监听实现类单独处理广播消息的逻辑。
<a name="zPmBH"></a>
### 消息与监听绑定
看到这儿，是不是已经发现了：消息类型和监听器的绑定发生在广播过程中。接下来就一探究竟<br />看一下`multicastEvent()`方法中的`getApplicationListeners(event, type)`方法。<br />在该方法中，用到了`ConcurrentHashMap`类型的缓存`retrieverCache`，所以每种类型的事件在广播的时候会触发**一次**绑定操作。它的**key**由事件的来源和类型确定，它的**value**中就包含了由事件来源和类型所确定的所有监听列表。<br />其中绑定的逻辑就出现在`retrieveApplicationListeners`方法中，大家可以去源码中查看。
<a name="SOtzA"></a>
## 实战教学
纸上得来终觉浅，绝知此事要躬行。为了更好地理解广播与监听的流程，当然得用实战来加以辅佐！
<a name="qw0r8"></a>
### 自定义事件
```java
public class MyEvent extends ApplicationContextEvent {
    public MyEvent(ApplicationContext source) {
        super(source);
    }
}
```
<a name="rTBYs"></a>
### 自定义广播
```java
@Component
public class MyPublisher implements ApplicationEventPublisherAware, ApplicationContextAware {
	
	private ApplicationEventPublisher applicationEventPublisher;
	
	private ApplicationContext applicationContext;
	
	@Override
	public void setApplicationEventPublisher(ApplicationEventPublisher applicationEventPublisher) {
		this.applicationEventPublisher = applicationEventPublisher;
	}
	
	@Override
	public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
		this.applicationContext = applicationContext;
	}
	
	//发送广播消息
	public void publishEvent(){
		System.out.println("我要开始发送消息了。。。");
		MyEvent myEvent = new MyEvent(applicationContext);
		applicationEventPublisher.publishEvent(myEvent);
	}
}
```
`MyPublisher`实现了`ApplicationEventPublisherAware`接口 ，在Spring初始化（见上文中的`invokeAwareInterfaces`）的时候会回调`setApplicationEventPublisher`方法，获取到初始化（添加bean后置处理器`ApplicationContextAwareProcessor`）时的`AbstractApplicationContext`，而`AbstractApplicationContext`又间接实现了`ApplicationEventPublisher`而获得发送能力。真正执行的是 `AbstractApplicationContext `类中的 `publishEvent` 方法。
<a name="I5u32"></a>
### 自定义监听
```java
@Component
public class MyEventListener implements ApplicationListener<MyEvent> {

    @Override
    public void onApplicationEvent(MyEvent event) {
        System.out.println("我监听到你的消息了");
    }
}
```
`MyEventListener`实现了`ApplicationListener`接口，在Spring初始化（见上文中的`addApplicationListener`）的时候会添加到`applicationListeners`中，在执行`publishEvent` 方法时就会走`MyEventListener`中的`onApplicationEvent`方法。
<a name="dCpho"></a>
### 客户端
```java
@RestController
@RequestMapping("/demo")
public class DemoTest {

    @Autowired
    private MyPublisher myPublisher;

    @RequestMapping("/test")
    public void test() {
        myPublisher.publishEvent();
    }
}
```
访问127.0.0.1:8008/demo/test就可以发送广播了，发送与监听内容如下：
```java
我要开始发送消息了。。。
我监听到你的消息了
```
