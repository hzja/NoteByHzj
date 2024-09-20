Java Spring 
<a name="v494O"></a>
## 1、Spring 事件驱动组成
Spring 事件驱动由3个部分组成

- `ApplicationEvent`：表示事件本身，自定义事件需要继承该类。用来定义事件
- `ApplicationEventPublisherAware`：事件发送器，需要实现该接口。主要用来发布事件。`ApplicationContext` 也实现了该接口，可以用于发布事件。

Spring 4.2之后，`ApplicationEventPublisher`自动被注入到容器中，采用 `Autowired` 即可获取。

- `ApplicationListener`：事件监听器接口。监听类实现 `ApplicationListener`  里`onApplicationEvent`方法即可。

在 Spring 4.2 以后可以以更加简洁的方式来监听 `event` 的发布，监听事件不必再实现 `ApplicationListener` 接口了，只要在方法上添加注解 `@EventListener`即可。

<a name="KhNFa"></a>
## 2、使用步骤详解
Spring中使用事件非常简单，只需要以下的几个步骤：

- 定义事件：继承 `ApplicationEvent`
- 定义监听：要么实现 `ApplicationListener` 接口，要么在方法上添加 `@EventListener` 注解
- 发布事件：调用 `ApplicationContext.publishEvent()` 或者 `ApplicationEventPublisher.publishEvent()`
<a name="XF0ec"></a>
### 定义事件
```java
public class OrderCreateEvent extends ApplicationEvent {
    private final Order order;
    public OrderCreateEvent(Object source, Order order) {
        super(source);
        this.order = order;
    }
    public Order getOrder() {
        return order;
    }
}
```
<a name="znKGQ"></a>
### 发送事件：`publishEvent()`方法
```java
@Autowired
private ApplicationEventPublisher applicationEventPublisher;
public void save(Order order) {
    //生成订单号
    String orderNo = getOrderNo();
    order.setOrderNo(orderNo);
    log.info("订单保存成功：" + order);
    //发布订单创建事件
    applicationEventPublisher.publishEvent(new OrderCreateEvent(this, order));
}
```
<a name="Y2I0i"></a>
### 订阅事件（实现观察者有2种方式）

- 方式一：实现 `ApplicationListener` 接口
```java
@Component
public class OrderCreateEventListener implements ApplicationListener<OrderCreateEvent> {
    @Override
    public void onApplicationEvent(OrderCreateEvent event) {
        System.out.printf("实现ApplicationListener接口,监听OrderCreateEvent事件");
    }
}
```

- 方式二：通过`@EventListener`注解,该会根据方法参数类型来自动监听相应事件的发布。
```java
@Component
@Slf4j
public class OrderCreateEventListener {
    @EventListener (classes = {OrderCreateEvent.class}) //classes属性指定处理事件的类型
    @Async //异步监听
    @Order(0)//使用order指定顺序，越小优先级越高
    public void eventListener(OrderCreateEvent event) {
        log.info("通过注解@EventListener和@Async,异步监听OrderCreateEvent事件,orderId:" + event.getOrder().getOrderNo());
    }
}
```
如果要监听多个事件类型的发布，可以在`@EventListener(classes = {FaceEvent.class, ArmEvent.class})`指定，Spring 会多次调用此方法来处理多个事件。但是注意此时，方法参数不能有多个，否则会发生转换异常，可以将使用多个事件的父类作为唯一的方法参数来接收处理事件，但除非必要否则并不推荐监听多个事件的发布。<br />如果有多个监听器监听同一事件，可以在方法上使用 Spring 的 `@order` 注解来定义多个监听器的顺序，order越小，优先级越高。<br />`@EventListener` 还有一个属性，`condition()`里可以使用 SPEL 表达式来过滤监听到事件，即只有符合某种条件的才进行接收处理。比如：
```java
@EventListener(condition = "event.message == 'message'")
```
监听多个事件:
```java
@EventListener({FaceEvent.class,ArmEvent.class})
public void onApplicationEvent3(Object event) {
        if(event instanceof FaceEvent){
            LOGGER.info("===> B 收到人脸事件:  {}",((FaceEvent) event).getEventData());
        }else if(event instanceof ArmEvent){
            ArmEvent armEvent = (ArmEvent) event;
            LOGGER.info("===> B 收到臂膀事件:  {}",armEvent.getEventData());
        }
}
```
<a name="vjJAX"></a>
### 注意事项

- 事件没要处理的监听器，就会被抛弃。
- 一个事件可以同时被多个监听处理类监听处理。
- 默认情况下事件是同步的，即事件被 `publish` 后会等待 `Listener` 的处理。如果发布事件处的业务存在事务，监听器处理也会在相同的事务中。
- 如果对于事件的处理不想受到影响，可以`onApplicationEvent`方法上加`@Async`支持异步或者在有`@EventListener`的注解方法上加上`@Async`。注：启动类上同时要加上`@EnableAsync`
<a name="WNQRZ"></a>
### 利用`@TransactionalEventListener`实现监听事件时的事务隔离
很多时候，只有事务提交之后才会发布相应的事件处理其他逻辑，比如用户注册之后，发送邮件或者短信。这时候就可以用注解 `@TransactionalEventListener`。<br />`@TransactionalEventListener` 和 `@EventListener` 都可以监听事件，但前者可以对发布事件和监听事件进行一些事务上的隔离。<br />`@TransactionalEventListener` 是对 `@EventListener`的一个扩展，允许将事件的监听器绑定到事务的某个阶段。可以绑定到以下事务阶段：

- `AFTER_COMMIT`（默认）：事务提交后
- `AFTER_ROLLBACK`：事务回滚后
- `AFTER_COMPLETION`：事务完成，包括提交后和回滚后
- `BEFORE_COMMIT`：事务提交前

`@TransactionalEventListener` 指不和发布事件的方法在同一个事务内，发布事件的方法事务结束后才会执行本监听方法，监听逻辑内发生异常不会回滚发布事件方法的事务。<br />`@TransactionalEventListener`有一个属性为 `fallbackExecution`，默认为 false，指发布事件的方法没有事务控制时，监听器不进行监听事件，此为默认情况！ `fallbackExecution=true`，则指发布事件的方法没有事务控制时，监听方法仍可以监听事件进行处理。
