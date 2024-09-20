JavaSpring
<a name="Qg64R"></a>
## 面试过程中的主要问题

1. 为什么需要使用事件这种模式？
2. Spring中实现事件有几种方式？
3. Spring中事件监听器消费事件是否支持异步模式？
4. Spring中事件监听器消费事件是否支持自定义顺序？

下面就一个个来介绍。
<a name="zII5V"></a>
## 为什么需要使用事件这种模式？
先来看一个业务场景：<br />产品经理：路人，这两天你实现一个注册的功能<br />我：注册功能比较简单，将用户信息入库就可以了，伪代码如下：
```java
public void registerUser(UserModel user){
    //插入用户信息到db，完成注册
    this.insertUser(user);
}
```
过了几天，产品经理：路人，注册成功之后，给用户发送一封注册成功的邮件<br />我：修改了一下上面注册代码，如下：
```java
public void registerUser(UserModel user){
    //插入用户信息到db，完成注册
    this.insertUser(user);
    //发送邮件
    this.sendEmailToUser(user);
}
```
由于修改了注册接口，所以所有调用这个方法的地方都需要重新测试一遍，让测试的兄弟们帮忙跑了一遍。<br />又过了几天，产品经理：路人，注册成功之后，给用户发一下优惠券<br />我：好的，又调整了一下代码
```java
public void registerUser(UserModel user){
    //插入用户信息到db，完成注册
    this.insertUser(user);
    //发送邮件
    this.sendEmailToUser(user);
    //发送优惠券
    this.sendCouponToUser(user);
}
```
我：测试的兄弟们，辛苦一下大家，注册接口又修改了，帮忙再过一遍。<br />过了一段时间，公司效益太好，产品经理：路人，注册的时候，取消给用户发送优惠券的功能。<br />我：又跑去调整了一下上面代码，将发送优惠券的功能干掉了，如下
```java
public void registerUser(UserModel user){
    //插入用户信息到db，完成注册
    this.insertUser(user);
    //发送邮件
    this.sendEmailToUser(user);
}
```
由于调整了代码，而注册功能又属于核心业务，所以需要让测试再次帮忙过一遍，又要麻烦测试来一遍了。<br />突然有一天，产品经理：路人，注册接口怎么这么慢啊，并且还经常失败？你这让公司要损失多少用户啊<br />我：赶紧跑去查看了一下运行日志，发现注册的时候给用户发送邮件不稳定，依赖于第三方邮件服务器，耗时比较长，并且容易失败。<br />跑去给产品经理说：由于邮件服务器不稳定的原因，导致注册不稳定。<br />产品经理：邮件可以不发，但是得确保注册功能必须可以用。<br />想了想，将上面代码改成了下面这样，发送邮件放在了子线程中执行：
```java
public void registerUser(UserModel user){
    //插入用户信息到db，完成注册
    this.insertUser(user);
    //发送邮件，放在子线程中执行，邮件的发送结果对注册逻辑不会有干扰作用
    new Thread(()->{
        this.sendEmailToUser(user);
    }).start();
}
```
又过了几天，产品经理又跑来了说：路人，最近效益不好，需要刺激用户消费，注册的时候继续发送优惠券。<br />我：倒，这是玩我么，反反复复让我调整注册的代码，让我改还好，让测试也反反复复来回搞，这是要玩死我们啊。<br />花了点时间，好好复盘整理了一下：发现问题不在于产品经理，从业务上来看，产品提的这些需求都是需求合理的，而结果代码反复调整、测试反复测试，以及一些次要的功能导致注册接口不稳定，这些问题归根到底，主要还是我的设计不合理导致的，将注册功能中的一些次要的功能耦合到注册的方法中了，并且这些功能可能会经常调整，导致了注册接口的不稳定性。<br />其实上面代码可以这么做：<br />找3个人：注册器、路人A、路人B。<br />注册器：负责将用户信息落库，落库成功之后，喊一声：用户XXX注册成功了。<br />路人A和路人B，竖起耳朵，当听到有人喊：XXX注册成功 的声音之后，立即行动做出下面反应：<br />路人A：负责给XXX发送一封注册邮件<br />路人B：负责给XXX发送优惠券<br />来看一下：<br />注册器只负责将用户信息落库，及广播一条用户注册成功的消息。<br />A和B相当于一个监听者，只负责监听用户注册成功的消息，当听到有这个消息产生的时候，A和B就去做自己的事情。<br />这里面注册器是感知不到A/B存在的，A和B也不用感知注册器的存在，A/B只用关注是否有人广播：XXX注册成功了的消息，当AB听到有人广播注册成功的消息，他们才做出反应，其他时间闲着休息。<br />这种方式就非常好：<br />当不想给用户发送优惠券的时候，只需要将B去掉就行了，此时基本上也不用测试，注册一下B的代码就行了。<br />若注册成功之后需要更多业务，比如还需要给用户增加积分，只需新增一个监听者C，监听到注册成功消息后，负责给用户添加积分，此时根本不用去调整注册的代码，开发者和测试人员只需要确保监听者C中的正确性就可以了。<br />上面这种模式就是事件模式。
<a name="T3ZQF"></a>
## 事件模式中的几个概念
**事件源**：事件的触发者，比如上面的注册器就是事件源。<br />**事件**：描述发生了什么事情的对象，比如上面的：xxx注册成功的事件<br />**事件监听器**：监听到事件发生的时候，做一些处理，比如上面的：路人A、路人B
<a name="LVN1S"></a>
## 下面使用事件模式实现用户注册的业务
先来定义和事件相关的几个类。
<a name="w46Pz"></a>
### 事件对象
表示所有事件的父类，内部有个source字段，表示事件源；自定义的事件需要继承这个类。
```java
package com.javacode2018.lesson003.demo1.test0.event;

/**
 * 事件对象
 */
public abstract class AbstractEvent {

    //事件源
    protected Object source;

    public AbstractEvent(Object source) {
        this.source = source;
    }

    public Object getSource() {
        return source;
    }

    public void setSource(Object source) {
        this.source = source;
    }
}
```
<a name="W8Qxq"></a>
### 事件监听器
使用一个接口来表示事件监听器，是个泛型接口，后面的类型E表示当前监听器需要监听的事件类型，此接口中只有一个方法，用来实现处理事件的业务；其定义的监听器需要实现这个接口。
```java
package com.javacode2018.lesson003.demo1.test0.event;

/**
 * 事件监听器
 *
 * @param <E> 当前监听器感兴趣的事件类型
 */
public interface EventListener<E extends AbstractEvent> {
    /**
     * 此方法负责处理事件
     *
     * @param event 要响应的事件对象
     */
    void onEvent(E event);
}
```
<a name="OxqpT"></a>
### 事件广播器

- 负责事件监听器的管理（注册监听器&移除监听器，将事件和监听器关联起来）
- 负责事件的广播（将事件广播给所有的监听器，对该事件感兴趣的监听器会处理该事件）
```java
package com.javacode2018.lesson003.demo1.test0.event;

/**
 * 事件广播器：
 * 1.负责事件监听器的管理（注册监听器&移除监听器，将事件和监听器关联起来）
 * 2.负责事件的广播（将事件广播给所有的监听器，对该事件感兴趣的监听器会处理该事件）
 */
public interface EventMulticaster {

    /**
     * 广播事件给所有的监听器，对该事件感兴趣的监听器会处理该事件
     *
     * @param event
     */
    void multicastEvent(AbstractEvent event);

    /**
     * 添加一个事件监听器（监听器中包含了监听器中能够处理的事件）
     *
     * @param listener 需要添加监听器
     */
    void addEventListener(EventListener<?> listener);


    /**
     * 将事件监听器移除
     *
     * @param listener 需要移除的监听器
     */
    void removeEventListener(EventListener<?> listener);
}
```
<a name="cBxLe"></a>
### 事件广播默认实现
```java
package com.javacode2018.lesson003.demo1.test0.event;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * 事件广播器简单实现
 */
public class SimpleEventMulticaster implements EventMulticaster {

    private Map<Class<?>, List<EventListener>> eventObjectEventListenerMap = new ConcurrentHashMap<>();

    @Override
    public void multicastEvent(AbstractEvent event) {
        List<EventListener> eventListeners = this.eventObjectEventListenerMap.get(event.getClass());
        if (eventListeners != null) {
            for (EventListener eventListener : eventListeners) {
                eventListener.onEvent(event);
            }
        }
    }

    @Override
    public void addEventListener(EventListener<?> listener) {
        Class<?> eventType = this.getEventType(listener);
        List<EventListener> eventListeners = this.eventObjectEventListenerMap.get(eventType);
        if (eventListeners == null) {
            eventListeners = new ArrayList<>();
            this.eventObjectEventListenerMap.put(eventType, eventListeners);
        }
        eventListeners.add(listener);
    }

    @Override
    public void removeEventListener(EventListener<?> listener) {
        Class<?> eventType = this.getEventType(listener);
        List<EventListener> eventListeners = this.eventObjectEventListenerMap.get(eventType);
        if (eventListeners != null) {
            eventListeners.remove(listener);
        }
    }

    /**
     * 获取事件监听器需要监听的事件类型
     *
     * @param listener
     * @return
     */
    protected Class<?> getEventType(EventListener listener) {
        ParameterizedType parameterizedType = (ParameterizedType) listener.getClass().getGenericInterfaces()[0];
        Type eventType = parameterizedType.getActualTypeArguments()[0];
        return (Class<?>) eventType;
    }

}
```
**上面3个类支撑了整个时间模型，下面使用上面三个类来实现注册的功能，目标是：高内聚低耦合，让注册逻辑方便扩展。**
<a name="gIXkn"></a>
### 自定义用户注册成功事件类
继承了`AbstractEvent`类
```java
package com.javacode2018.lesson003.demo1.test0.userregister;

import com.javacode2018.lesson003.demo1.test0.event.AbstractEvent;

/**
 * 用户注册成功事件
 */
public class UserRegisterSuccessEvent extends AbstractEvent {
    //用户名
    private String userName;

    /**
     * 创建用户注册成功事件对象
     *
     * @param source   事件源
     * @param userName 当前注册的用户名
     */
    public UserRegisterSuccessEvent(Object source, String userName) {
        super(source);
        this.userName = userName;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }
}
```
<a name="rVMY6"></a>
### 用户注册服务
负责实现用户注册逻辑
```java
package com.javacode2018.lesson003.demo1.test0.userregister;

import com.javacode2018.lesson003.demo1.test0.event.EventMulticaster;

/**
 * 用户注册服务
 */
public class UserRegisterService {
    //事件发布者
    private EventMulticaster eventMulticaster; //@0

    /**
     * 注册用户
     *
     * @param userName 用户名
     */
    public void registerUser(String userName) { //@1
        //用户注册(将用户信息入库等操作)
        System.out.println(String.format("用户【%s】注册成功", userName)); //@2
        //广播事件
        this.eventMulticaster.multicastEvent(new UserRegisterSuccessEvent(this, userName)); //@3
    }

    public EventMulticaster getEventMulticaster() {
        return eventMulticaster;
    }

    public void setEventMulticaster(EventMulticaster eventMulticaster) {
        this.eventMulticaster = eventMulticaster;
    }
}
```
@0：事件发布者<br />@1：`registerUser`这个方法负责用户注册，内部主要做了2个事情<br />@2：模拟将用户信息落库<br />@3：使用事件发布者`eventPublisher`发布用户注册成功的消息：
<a name="MXGKI"></a>
### 下面使用Spring来将上面的对象组装起来
```java
package com.javacode2018.lesson003.demo1.test0.userregister;

import com.javacode2018.lesson003.demo1.test0.event.EventListener;
import com.javacode2018.lesson003.demo1.test0.event.EventMulticaster;
import com.javacode2018.lesson003.demo1.test0.event.SimpleEventMulticaster;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

import java.util.List;

@Configuration
@ComponentScan
public class MainConfig0 {

    /**
     * 注册一个bean：事件发布者
     *
     * @param eventListeners
     * @return
     */
    @Bean
    @Autowired(required = false)
    public EventMulticaster eventMulticaster(List<EventListener> eventListeners) { //@1
        EventMulticaster eventPublisher = new SimpleEventMulticaster();
        if (eventListeners != null) {
            eventListeners.forEach(eventPublisher::addEventListener);
        }
        return eventPublisher;
    }

    /**
     * 注册一个bean：用户注册服务
     *
     * @param eventMulticaster
     * @return
     */
    @Bean
    public UserRegisterService userRegisterService(EventMulticaster eventMulticaster) { //@2
        UserRegisterService userRegisterService = new UserRegisterService();
        userRegisterService.setEventMulticaster(eventMulticaster);
        return userRegisterService;
    }
}
```
上面有2个方法，负责向Spring容器中注册2个bean。<br />@1：向Spring容器中注册了一个bean：事件发布者，方法传入了`EventListener`类型的List，这个地方会将容器中所有的事件监听器注入进来，丢到`EventMulticaster`中。<br />@2：向Spring容器中注册了一个bean：用户注册服务
<a name="hmOla"></a>
### 来个测试用例模拟用户注册
```java
package com.javacode2018.lesson003.demo1;

import com.javacode2018.lesson003.demo1.test0.userregister.MainConfig0;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class EventTest {

    @Test
    public void test0() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig0.class);
        //获取用户注册服务
        com.javacode2018.lesson003.demo1.test0.userregister.UserRegisterService userRegisterService =
        context.getBean(com.javacode2018.lesson003.demo1.test0.userregister.UserRegisterService.class);
        //模拟用户注册
        userRegisterService.registerUser("Java");
    }

}
```
<a name="AldWR"></a>
### 运行输出
```
用户【Java】注册成功
```
<a name="WO5DA"></a>
### 添加注册成功发送邮件功能
下面添加一个注册成功发送邮件的功能，只需要自定义一个监听用户注册成功事件的监听器就可以了，其他代码不需要任何改动，如下
```java
package com.javacode2018.lesson003.demo1.test0.userregister;


import com.javacode2018.lesson003.demo1.test0.event.EventListener;
import org.springframework.stereotype.Component;

/**
 * 用户注册成功事件监听器->负责给用户发送邮件
 */
@Component
public class SendEmailOnUserRegisterSuccessListener implements EventListener<UserRegisterSuccessEvent> {
    @Override
    public void onEvent(UserRegisterSuccessEvent event) {
        System.out.println(String.format("给用户【%s】发送注册成功邮件!", event.getUserName()));
    }
}
```
上面这个类使用了`@Component`，会被自动扫描注册到Spring容器。
<a name="KrKpf"></a>
### 再次运行测试用例输出
```
用户【Java】注册成功
给用户【Java】发送注册成功邮件!
```
<a name="LZ2jk"></a>
### 小结
上面将注册的主要逻辑（用户信息落库）和次要的业务逻辑（发送邮件）通过事件的方式解耦了。次要的业务做成了可插拔的方式，比如不想发送邮件了，只需要将邮件监听器上面的`@Component`注释就可以了，非常方便扩展。<br />上面用到的和事件相关的几个类，都是自己实现的，其实这些功能在Spring中已经实现好了，用起来更容易一些，下面带大家来体验一下。
<a name="PwaAR"></a>
## Spring中实现事件模式
<a name="FjZpf"></a>
### 事件相关的几个类
Spring中事件相关的几个类需要先了解一下，下面来个表格，将Spring中事件相关的类和上面自定义的类做个对比，方便大家理解<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689522306985-a1935f1f-ebf1-4a8a-bac9-a770956b2fe0.png#averageHue=%23f2eeee&clientId=u932a8103-47f4-4&from=paste&id=u66d0017d&originHeight=212&originWidth=1028&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u556d2a3d-afa7-46e2-9bc7-236e013edf2&title=)<br />这些类和自定义的类中代码有点类似，有兴趣的可以去看一下源码，这里就不列出来了。
<a name="ecfwQ"></a>
### 硬编码的方式使用Spring事件3步骤
<a name="Jkn4a"></a>
#### 步骤1：定义事件
自定义事件，需要继承`ApplicationEvent`类，
<a name="pL9qc"></a>
#### 步骤2：定义监听器
自定义事件监听器，需要实现`ApplicationListener`接口，这个接口有个方法`onApplicationEvent`需要实现，用来处理感兴趣的事件。
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
<a name="p4cHb"></a>
#### 步骤3：创建事件广播器
创建事件广播器`ApplicationEventMulticaster`，这是个接口，可以自己实现这个接口，也可以直接使用系统提供的`SimpleApplicationEventMulticaster`，如下：
```java
ApplicationEventMulticaster applicationEventMulticaster = new SimpleApplicationEventMulticaster();
```
<a name="BRKZS"></a>
#### 步骤4：向广播器中注册事件监听器
将事件监听器注册到广播器`ApplicationEventMulticaster`中，如：
```java
ApplicationEventMulticaster applicationEventMulticaster = new SimpleApplicationEventMulticaster();
applicationEventMulticaster.addApplicationListener(new SendEmailOnOrderCreateListener());
```
<a name="NWO6N"></a>
#### 步骤5：通过广播器发布事件
广播事件，调用`ApplicationEventMulticaster#multicastEvent`方法广播事件，此时广播器中对这个事件感兴趣的监听器会处理这个事件。
```
applicationEventMulticaster.multicastEvent(new OrderCreateEvent(applicationEventMulticaster, 1L));
```
下面来个案例将这5个步骤串起来感受一下。
<a name="PssD6"></a>
### 案例
实现功能：电商中订单创建成功之后，给下单人发送一封邮件，发送邮件的功能放在监听器中实现。<br />下面上代码
<a name="X0uuf"></a>
#### 来个事件类：订单创建成功事件
```java
package com.javacode2018.lesson003.demo1.test1;

import org.springframework.context.ApplicationEvent;

/**
 * 订单创建事件
 */
public class OrderCreateEvent extends ApplicationEvent {
    //订单id
    private Long orderId;

    /**
     * @param source  事件源
     * @param orderId 订单id
     */
    public OrderCreateEvent(Object source, Long orderId) {
        super(source);
        this.orderId = orderId;
    }

    public Long getOrderId() {
        return orderId;
    }

    public void setOrderId(Long orderId) {
        this.orderId = orderId;
    }
}
```
<a name="feRqk"></a>
#### 来个监听器：负责监听订单成功事件，发送邮件
```java
package com.javacode2018.lesson003.demo1.test1;

import org.springframework.context.ApplicationListener;
import org.springframework.stereotype.Component;

/**
 * 订单创建成功给用户发送邮件
 */
@Component
public class SendEmailOnOrderCreateListener implements ApplicationListener<OrderCreateEvent> {
    @Override
    public void onApplicationEvent(OrderCreateEvent event) {
        System.out.println(String.format("订单【%d】创建成功，给下单人发送邮件通知!", event.getOrderId()));
    }
}
```
<a name="BrXgV"></a>
#### 测试用例
```java
@Test
public void test2() throws InterruptedException {
    //创建事件广播器
    ApplicationEventMulticaster applicationEventMulticaster = new SimpleApplicationEventMulticaster();
    //注册事件监听器
    applicationEventMulticaster.addApplicationListener(new SendEmailOnOrderCreateListener());
    //广播事件订单创建事件
    applicationEventMulticaster.multicastEvent(new OrderCreateEvent(applicationEventMulticaster, 1L));
}
```
<a name="U1VjL"></a>
#### 运行输出
```
订单【1】创建成功，给下单人发送邮件通知!
```
<a name="sXBM1"></a>
### `ApplicationContext`容器中事件的支持
上面演示了Spring中事件的使用，那么平时使用Spring的时候就这么使用？<br />非也非也，上面只是给大家演示了一下原理。<br />通常情况下，会使用以`ApplicationContext`结尾的类作为Spring的容器来启动应用，下面2个是比较常见的

1. `AnnotationConfigApplicationContext`
2. `ClassPathXmlApplicationContext`

来看一个类图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689522307011-ad6c0195-aaf8-4a2d-98ea-fb053bdc96a0.png#averageHue=%23faf9ee&clientId=u932a8103-47f4-4&from=paste&id=u77cf5c08&originHeight=275&originWidth=752&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue32d171f-598f-417f-8b29-0b265e1821f&title=)<br />对这个图来解释一下：

1. `AnnotationConfigApplicationContext`和`ClassPathXmlApplicationContext`都继承了`AbstractApplicationContext`
2. `AbstractApplicationContext`实现了`ApplicationEventPublisher`接口
3. `AbstractApplicationContext`内部有个`ApplicationEventMulticaster`类型的字段

上面第三条，说明了`AbstractApplicationContext`内部已经集成了事件广播器`ApplicationEventMulticaster`，说明`AbstractApplicationContext`内部是具体事件相关功能的，这些功能是通过其内部的`ApplicationEventMulticaster`来实现的，也就是说将事件的功能委托给了内部的`ApplicationEventMulticaster`来实现。
<a name="FbqIi"></a>
### `ApplicationEventPublisher`接口
上面类图中多了一个新的接口`ApplicationEventPublisher`，来看一下源码
```java
@FunctionalInterface
public interface ApplicationEventPublisher {

    default void publishEvent(ApplicationEvent event) {
        publishEvent((Object) event);
    }

    void publishEvent(Object event);

}
```
这个接口用来发布事件的，内部定义2个方法都是用来发布事件的。<br />Spring中不是有个`ApplicationEventMulticaster`接口么，此处怎么又来了一个发布事件的接口？<br />这个接口的实现类中，比如`AnnotationConfigApplicationContext`内部将这2个方法委托给`ApplicationEventMulticaster#multicastEvent`进行处理了。<br />所以调用`AbstractApplicationContext`中的`publishEvent`方法，也实现广播事件的效果，不过使用`AbstractApplicationContext`也只能通过调用`publishEvent`方法来广播事件。
<a name="ZYkOK"></a>
### 获取`ApplicationEventPublisher`对象
如果想在普通的bean中获取`ApplicationEventPublisher`对象，需要实现`ApplicationEventPublisherAware`接口
```java
public interface ApplicationEventPublisherAware extends Aware {
    void setApplicationEventPublisher(ApplicationEventPublisher applicationEventPublisher);
}
```
Spring容器会自动通过上面的`setApplicationEventPublisher`方法将`ApplicationEventPublisher`注入进来，此时就可以使用这个来发布事件了。
<a name="MZXaN"></a>
### Spring为了简化事件的使用，提供了2种使用方式

1. **面相接口的方式**
2. **面相**`**@EventListener**`**注解的方式**
<a name="Vj36M"></a>
## 面相接口的方式
<a name="vIGOa"></a>
### 案例
实现用户注册成功后发布事件，然后在监听器中发送邮件的功能。
<a name="Fuonq"></a>
#### 用户注册事件
需要继承`ApplicationEvent`
```java
package com.javacode2018.lesson003.demo1.test2;

import org.springframework.context.ApplicationEvent;

/**
 * 用户注册事件
 */
public class UserRegisterEvent extends ApplicationEvent {
    //用户名
    private String userName;

    public UserRegisterEvent(Object source, String userName) {
        super(source);
        this.userName = userName;
    }

    public String getUserName() {
        return userName;
    }
}
```
<a name="mcjFN"></a>
#### 发送邮件监听器
需实现`ApplicationListener`接口
```java
package com.javacode2018.lesson003.demo1.test2;

import org.springframework.context.ApplicationListener;
import org.springframework.stereotype.Component;

/**
 * 用户注册成功发送邮件
 */
@Component
public class SendEmailListener implements ApplicationListener<UserRegisterEvent> {

    @Override
    public void onApplicationEvent(UserRegisterEvent event) {
        System.out.println(String.format("给用户【%s】发送注册成功邮件!", event.getUserName()));

    }
}
```
<a name="VJ8Ga"></a>
#### 用户注册服务
内部提供用户注册的功能，并发布用户注册事件
```java
package com.javacode2018.lesson003.demo1.test2;


import org.springframework.context.ApplicationEventPublisher;
import org.springframework.context.ApplicationEventPublisherAware;
import org.springframework.stereotype.Component;

/**
 * 用户注册服务
 */
@Component
public class UserRegisterService implements ApplicationEventPublisherAware {

    private ApplicationEventPublisher applicationEventPublisher;

    /**
     * 负责用户注册及发布事件的功能
     *
     * @param userName 用户名
     */
    public void registerUser(String userName) {
        //用户注册(将用户信息入库等操作)
        System.out.println(String.format("用户【%s】注册成功", userName));
        //发布注册成功事件
        this.applicationEventPublisher.publishEvent(new UserRegisterEvent(this, userName));
    }

    @Override
    public void setApplicationEventPublisher(ApplicationEventPublisher applicationEventPublisher) { //@1
        this.applicationEventPublisher = applicationEventPublisher;
    }
}
```
注意上面实现了`ApplicationEventPublisherAware`接口，Spring容器会通过@1将`ApplicationEventPublisher`注入进来，然后就可以使用这个来发布事件了。
<a name="XkUNz"></a>
#### 来个Spring配置类
```java
package com.javacode2018.lesson003.demo1.test2;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig2 {
}
```
<a name="rjbh3"></a>
#### 上测试用例
```java
@Test
public void test2() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig2.class);
    context.refresh();
    //获取用户注册服务
    UserRegisterService userRegisterService = context.getBean(UserRegisterService.class);
    //模拟用户注册
    userRegisterService.registerUser("Java");
}
```
<a name="zr4T6"></a>
#### 运行输出
```
用户【Java】注册成功
给用户【Java】发送注册成功邮件!
```
<a name="zRK8O"></a>
### 原理
Spring容器在创建bean的过程中，会判断bean是否为`ApplicationListener`类型，进而会将其作为监听器注册到`AbstractApplicationContext#applicationEventMulticaster`中，这块的源码在下面这个方法中，有兴趣的可以看一下
```
org.springframework.context.support.ApplicationListenerDetector#postProcessAfterInitialization
```
<a name="KwDTk"></a>
### 小结
从上面这个案例中可以看出，事件类、监听器类都是通过基于Spring中的事件相关的一些接口来实现事件的功能，这种方式就称作面相接口的方式。
<a name="b45UM"></a>
## 面相`@EventListener`注解方式
<a name="fioqN"></a>
### 用法
上面是通过接口的方式创建一个监听器，Spring还提供了通过`@EventListener`注解的方式来创建一个监听器，直接将这个注解标注在一个bean的方法上，那么这个方法就可以用来处理感兴趣的事件，使用更简单，如下，方法参数类型为事件的类型：
```java
@Component
public class UserRegisterListener {
    @EventListener
    public void sendMail(UserRegisterEvent event) {
        System.out.println(String.format("给用户【%s】发送注册成功邮件!", event.getUserName()));
    }
}
```
<a name="AJpx6"></a>
### 案例
注册成功之后：来2个监听器：一个负责发送邮件、一个负责发送优惠券。<br />其他代码都不上了，和上面案例中的一样，主要看监听器的代码，如下：
```java
package com.javacode2018.lesson003.demo1.test3;

import org.springframework.context.event.EventListener;
import org.springframework.stereotype.Component;

/**
 * 用户注册监听器
 */
@Component
public class UserRegisterListener {
    @EventListener
    public void sendMail(UserRegisterEvent event) {
        System.out.println(String.format("给用户【%s】发送注册成功邮件!", event.getUserName()));
    }

    @EventListener
    public void sendCompon(UserRegisterEvent event) {
        System.out.println(String.format("给用户【%s】发送优惠券!", event.getUserName()));
    }
}
```
这块案例代码
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig3.class);
    context.refresh();
    //获取用户注册服务
    UserRegisterService userRegisterService = context.getBean(UserRegisterService.class);
    //模拟用户注册
    userRegisterService.registerUser("Java");
}
```
运行结果
```
用户【Java】注册成功
给用户【Java】发送优惠券!
给用户【Java】发送注册成功邮件!
```
<a name="DPxmx"></a>
### 原理
Spring中处理`@EventListener`注解源码位于下面的方法中
```
org.springframework.context.event.EventListenerMethodProcessor#afterSingletonsInstantiated
```
`EventListenerMethodProcessor`实现了`SmartInitializingSingleton`接口，`SmartInitializingSingleton`接口中的`afterSingletonsInstantiated`方法会在所有单例的bean创建完成之后被Spring容器调用。
<a name="r63T9"></a>
### idea对注解的方式支持比较好
注解的方式实现监听器，idea对这块支持比较好，时间发布的地方会显示一个耳机，点击这个耳机的时候，Spring会列出这个事件有哪些监听器<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689522334772-b94382d1-5dde-4efb-b706-376207ad6700.png#averageHue=%23faf8f5&clientId=u932a8103-47f4-4&from=paste&id=uc69d5c25&originHeight=338&originWidth=1021&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8f50459a-276a-404f-ae45-f823de025f5&title=)<br />点击耳机列出了2个监听器，可以快速定位到监听器，如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689522334668-ffc47e29-085f-419c-be91-5558c35e2d01.png#averageHue=%23ead2a1&clientId=u932a8103-47f4-4&from=paste&id=u86532a1b&originHeight=194&originWidth=945&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud3eec7c7-c191-4b9a-983c-faee5f5de99&title=)<br />同样监听器的地方也有一个广播的图标，如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689522334677-c66f2652-fe44-4d93-ba30-63b2b4969bf0.png#averageHue=%23fcfbf9&clientId=u932a8103-47f4-4&from=paste&id=uc7d6d33a&originHeight=417&originWidth=1010&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf6aad28e-6d12-41b5-98bc-7f518272548&title=)<br />点击上面这个广播的图标，可以快速导航到事件发布的地方，相当方便。
<a name="MuhpM"></a>
## 监听器支持排序功能
如果某个事件有多个监听器，默认情况下，监听器执行顺序是无序的，不过可以为监听器指定顺序。
<a name="b356C"></a>
### 通过接口实现监听器的情况
如果自定义的监听器是通过`ApplicationListener`接口实现的，那么指定监听器的顺序有三种方式
<a name="d1YHe"></a>
#### 方式1：实现`org.springframework.core.Ordered`接口
需要实现一个`getOrder`方法，返回顺序值，值越小，顺序越高
```java
int getOrder();
```
<a name="xoAmJ"></a>
#### 方式2：实现`org.springframework.core.PriorityOrdered`接口
`PriorityOrdered`接口继承了方式一中的`Ordered`接口，所以如果你实现`PriorityOrdered`接口，也需要实现`getOrder`方法。
<a name="CbzID"></a>
#### 方式3：类上使用`@org.springframework.core.annotation.Order`注解
看一下这个注解的源码
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD, ElementType.FIELD})
@Documented
public @interface Order {

    int value() default Ordered.LOWEST_PRECEDENCE;

}
```
value属性用来指定顺序
<a name="fY7Ny"></a>
#### 这几种方式排序规则
```
PriorityOrdered#getOrder ASC,Ordered或@Order ASC
```
<a name="ux0l0"></a>
### 通过`@EventListener`实现事件监听器的情况
可以在标注`@EventListener`的方法上面使用`@Order(顺序值)`注解来标注顺序，如：
```java
@EventListener
@Order(1)
public void sendMail(com.javacode2018.lesson003.demo1.test3.UserRegisterEvent event) {
    System.out.println(String.format("给用户【%s】发送注册成功邮件!", event.getUserName()));
}
```
<a name="NWG2o"></a>
### 案例
```java
package com.javacode2018.lesson003.demo1.test4;

import org.springframework.context.event.EventListener;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

/**
 * 用户注册监听器
 */
@Component
public class UserRegisterListener {
    @EventListener
    @Order(1)
    public void sendMail(UserRegisterEvent event) {
        System.out.println(String.format("【%s】，给用户【%s】发送注册成功邮件!", Thread.currentThread(), event.getUserName()));
    }

    @EventListener
    @Order(0)
    public void sendCompon(UserRegisterEvent event) {
        System.out.println(String.format("【%s】，给用户【%s】发送优惠券!", Thread.currentThread(), event.getUserName()));
    }
}
```
上面会先发送优惠券、然后再发送邮件。<br />上面输出中顺便将线程信息也输出了。<br />对应测试用例
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig4.class);
    context.refresh();
    //获取用户注册服务
    UserRegisterService userRegisterService = context.getBean(UserRegisterService.class);
    //模拟用户注册
    userRegisterService.registerUser("Java");
}
```
运行输出
```
【Thread[main,5,main]】,用户【Java】注册成功
【Thread[main,5,main]】，给用户【Java】发送优惠券!
【Thread[main,5,main]】，给用户【Java】发送注册成功邮件!
```
从输出中可以看出上面程序的执行都在主线程中执行的，说明监听器中的逻辑和注册逻辑在一个线程中执行的，此时如果监听器中的逻辑比较耗时或者失败，直接会导致注册失败，通常将一些非主要逻辑可以放在监听器中执行，至于这些非主要逻辑成功或者失败，最好不要对主要的逻辑产生影响，所以最好能将监听器的运行和主业务隔离开，放在不同的线程中执行，主业务不用关注监听器的结果，Spring中支持这种功能，下面继续看。
<a name="D8qRq"></a>
## 监听器异步模式
<a name="GkSMq"></a>
### 先来看看到底如何实现？
监听器最终是通过`ApplicationEventMulticaster`内部的实现来调用的，所以关注的重点就是这个类，这个类默认有个实现类`SimpleApplicationEventMulticaster`，这个类是支持监听器异步调用的，里面有个字段：
```java
private Executor taskExecutor;
```
高并发比较熟悉的朋友对`Executor`这个接口是比较熟悉的，可以用来异步执行一些任务。<br />常用的线程池类`java.util.concurrent.ThreadPoolExecutor`就实现了`Executor`接口。<br />再来看一下`SimpleApplicationEventMulticaster`中事件监听器的调用，最终会执行下面这个方法
```java
@Override
public void multicastEvent(final ApplicationEvent event, @Nullable ResolvableType eventType) {
    ResolvableType type = (eventType != null ? eventType : resolveDefaultEventType(event));
    Executor executor = getTaskExecutor();
    for (ApplicationListener<?> listener : getApplicationListeners(event, type)) {
        if (executor != null) { //@1
            executor.execute(() -> invokeListener(listener, event));
        }
        else {
            invokeListener(listener, event);
        }
    }
}
```
上面的`invokeListener`方法内部就是调用监听器，从代码@1可以看出，如果当前`executor`不为空，监听器就会被异步调用，所以如果需要异步只需要让`executor`不为空就可以了，但是默认情况下`executor`是空的，此时需要给其设置一个值，下面需要看容器中是如何创建广播器的，在那个地方去干预。<br />通常使用的容器是`AbstractApplicationContext`类型的，需要看一下`AbstractApplicationContext`中广播器是怎么初始化的，就是下面这个方法，容器启动的时候会被调用，用来初始化`AbstractApplicationContext`中的事件广播器`applicationEventMulticaster`
```java
public static final String APPLICATION_EVENT_MULTICASTER_BEAN_NAME = "applicationEventMulticaster";

protected void initApplicationEventMulticaster() {
    ConfigurableListableBeanFactory beanFactory = getBeanFactory();
    if (beanFactory.containsLocalBean(APPLICATION_EVENT_MULTICASTER_BEAN_NAME)) {
        this.applicationEventMulticaster =
        beanFactory.getBean(APPLICATION_EVENT_MULTICASTER_BEAN_NAME, ApplicationEventMulticaster.class);
    }
    else {
        this.applicationEventMulticaster = new SimpleApplicationEventMulticaster(beanFactory);
        beanFactory.registerSingleton(APPLICATION_EVENT_MULTICASTER_BEAN_NAME, this.applicationEventMulticaster);
    }
}
```
上面逻辑解释一下：判断Spring容器中是否有名称为`applicationEventMulticaster`的bean，如果有就将其作为事件广播器，否则创建一个`SimpleApplicationEventMulticaster`作为广播器，并将其注册到Spring容器中。<br />从上面可以得出结论：只需要自定义一个类型为`SimpleApplicationEventMulticaster`名称为`applicationEventMulticaster`的bean就可以了，顺便给`executor`设置一个值，就可以实现监听器异步执行了。
<a name="tAeP6"></a>
### 具体实现如下
```java
package com.javacode2018.lesson003.demo1.test5;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.event.ApplicationEventMulticaster;
import org.springframework.context.event.SimpleApplicationEventMulticaster;
import org.springframework.scheduling.concurrent.ThreadPoolExecutorFactoryBean;

import java.util.concurrent.Executor;

@ComponentScan
@Configuration
public class MainConfig5 {
    @Bean
    public ApplicationEventMulticaster applicationEventMulticaster() { //@1
        //创建一个事件广播器
        SimpleApplicationEventMulticaster result = new SimpleApplicationEventMulticaster();
        //给广播器提供一个线程池，通过这个线程池来调用事件监听器
        Executor executor = this.applicationEventMulticasterThreadPool().getObject();
        //设置异步执行器
        result.setTaskExecutor(executor);//@1
        return result;
    }

    @Bean
    public ThreadPoolExecutorFactoryBean applicationEventMulticasterThreadPool() {
        ThreadPoolExecutorFactoryBean result = new ThreadPoolExecutorFactoryBean();
        result.setThreadNamePrefix("applicationEventMulticasterThreadPool-");
        result.setCorePoolSize(5);
        return result;
    }
}
```
@1：定义了一个名称为`applicationEventMulticaster`的事件广播器，内部设置了一个线程池用来异步调用监听器<br />这段代码对应的测试用例
```java
@Test
public void test5() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig5.class);
    context.refresh();
    //获取用户注册服务
    UserRegisterService userRegisterService = context.getBean(UserRegisterService.class);
    //模拟用户注册
    userRegisterService.registerUser("Java");
}
```
运行输出
```
当前线程【Thread[main,5,main]】,用户【Java】注册成功
当前线程【Thread[applicationEventMulticasterThreadPool-2,5,main]】,给用户【Java】发送注册成功邮件!
当前线程【Thread[applicationEventMulticasterThreadPool-1,5,main]】,给用户【Java】发放一些优惠券!
```
此时实现了监听器异步执行的效果。
<a name="vPXF1"></a>
## 关于事件使用建议

1. Spring中事件是使用接口的方式还是使用注解的方式？具体使用哪种方式都可以，不过在公司内部最好大家都统一使用一种方式
2. 异步事件的模式，通常将一些非主要的业务放在监听器中执行，因为监听器中存在失败的风险，所以使用的时候需要注意。如果只是为了解耦，但是被解耦的次要业务也是必须要成功的，可以使用消息中间件的方式来解决这些问题。
