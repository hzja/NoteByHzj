Java Spring BeanFactory FactoryBean ObjectFactory
<a name="g2dJR"></a>
### 结论
`BeanFactory`就是对象工厂，用于实例化和保存对象。<br />`FactoryBean`是一个工厂对象，用于实例化创建过程比较复杂的对象。<br />`ObjectFactory`是某个特定的工厂，用于在项目启动时，延迟实例化对象，解决循环依赖问题。
<a name="GZGNQ"></a>
### `BeanFactory`
Spring最最核心的接口，Bean工厂，从名字上就能看出这是一个工厂。那么既然是工厂就是用来生产对象的。
<a name="UUsWB"></a>
### `FactoryBean`
从名字上能看出这是一个Bean。Bean就是Spring对对象的一种定义，一个Bean就是一个或者一些同类型的对象。<br />FactoryBean是一个接口。
```java
public interface FactoryBean<T> {

    //返回的对象实例
    T getObject() throws Exception;
    //Bean的类型
    Class<?> getObjectType();
    //true是单例，false是非单例  在Spring5.0中此方法利用了JDK1.8的新特性变成了default方法，返回true
    boolean isSingleton();
}
```
可以观察到`FactoryBean`接口有三个方法。<br />`getObject`用来返回实例化后的对象。<br />`getObjectType`用来返回对象的类型。<br />`isSingleton`用来标识对象是否为单例的，这里默认为true，Spring会将实例化后的对象放入`BeanFactory`容器中。<br />从方法上，能推测出，这个凡是实现了`FactoryBean`接口的类，负责返回这个java类的实例化对象。<br />从设计模式的角度来看这就是典型的工厂方法模式。由一个特定的工厂来生产特定的java类的实例化对象。<br />那么这种写法有哪些好处呢？<br />正常情况下，Spring中在实例化对象的时候，都是由`BeanFactory`从上下文中获取`BeanDefinition`信息，然后通过反射，调用这个java类的构造方法进行实例化。而现在这种形式，相当于将实例化的功能交给了`FactoryBean`去实现。这种方式主要使用在一些比较复杂的实例化过程中，并非简单地设置一些参数或者设置的参数过多，过程中可能需要做一些复杂的解析、判断和逻辑处理，这个时候交由Spring去通过反射进行实例化可能就不太灵活了，<br />Spring容器中有两种Bean，一种是普通的Bean对象，一种是实现了`FactoryBean`的工厂Bean对象。如果从`BeanFactory`中`getBean`的时候，获取到的Bean对象是工厂Bean，会自动的调用它的`getObject`方法返回真实实例化对象。<br />如果就是需要获取`FactoryBean`对象，需要在`getBean`的时候加上前缀'&'。<br />Spring自身就对`FactoryBean`有70多种实现，比较常见的就是Proxy，Jndi等场景。AOP中使用的`ProxyFactoryBean`。<br />Dubbo中使用的`ReferenceBean`。<br />Mybatis中使用的`SqlSessionFactoryBean`。
<a name="cxXkR"></a>
### `ObjectFactory`
```java
public interface ObjectFactory<T> {

    //返回的对象实例
    T getObject() throws BeansException;
}
```
这用于延迟查找的场景，它就是一个普通工厂，当得到 `ObjectFactory` 对象时，相当于 Bean 没有被创建，只有当 `getObject()` 方法时，才会触发 Bean 实例化等生命周期。<br />主要用于暂时性地获取某个 Bean Holder 对象，如果过早的加载，可能会引起一些意外的情况，比如当 Bean A 依赖 Bean B 时，如果过早地初始化 A，那么 B 里面的状态可能是中间状态，这时候使用 A 容易导致一些错误
