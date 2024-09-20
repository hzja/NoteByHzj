JavaSpring
<a name="PPAGJ"></a>
### 1、普通对象
这没什么好说的，大家都这么用的，比如需要用到UserService，直接`@Autowired`就可以了。
```java
@Autowired
private UserService userService;
```
<a name="ufuQ2"></a>
### 2、Collection及其子接口
除了支持注入一个单一的对象之外，`@Autowired`还支持注入一个`Collection`对象。<br />比如说，现在有个消息通知的接口`MessageNotifier`。<br />这种接口一般都会有不同的实现，比如说通过邮件通知，或者app，短信等等，所以就有多种实现，此时如果需要注入`MessageNotifier`，就可以使用注入Collection的方式，比如
```java
@Autowired
private List<MessageNotifier> messageNotifiers;
```
不过这种方式有个规定，那就是注入的类型必须是`Collection`及其子接口，如果直接注入一个`ArrayList`，那么此时是不支持的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551160-725c23ce-d1c4-42d2-9082-f3648c753991.png#averageHue=%232e2b2a&clientId=ue4a35858-d49f-4&from=paste&id=u8d79db96&originHeight=724&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u987a907c-e46f-41a2-9b98-39558cc6d3d&title=)
<a name="jSmcC"></a>
### 3、数组
同理，`@Autowired`可实现了注入一个数组的功能。
```java
@Autowired
private MessageNotifier[] messageNotifiers;
```
代码如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551182-bdb14538-4427-48bf-9b2a-389fd12857dc.png#averageHue=%232c2b2b&clientId=ue4a35858-d49f-4&from=paste&id=ue93db89e&originHeight=867&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uac493ae0-368a-421e-a6e2-248ce561c05&title=)
<a name="khuKp"></a>
### 4、Map
同样的，`@Autowired`还可以注入一个Map。
```java
@Autowired
private Map<String, MessageNotifier> messageNotifierMap;
```
此时注入的map，key的类型就是bean的名称，这种方式可以配合策略模式使用。<br />不过，这种方式只支持注入的是Map接口，不支持子类型接口，代码如下。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551142-97107676-a34a-4210-8079-f0112796f2e5.png#averageHue=%232d2b2b&clientId=ue4a35858-d49f-4&from=paste&id=udb4ba770&originHeight=679&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0d478624-e07e-4a44-88cb-c4b16062a7e&title=)
<a name="w8l6R"></a>
### 5、`@Lazy`
当一个注入的字段加了`@Lazy`注解之后，那么此时就代表这个字段是延迟注入。
```java
@Autowired
@Lazy
private MessageNotifier messageNotifier;
```
延迟注入并不是不注入，而是注入目标对象类型的代理对象，真正的目标是当需要用到的时候在创建。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551171-a5df6359-f445-4e53-9b3e-650c52c4161e.png#averageHue=%2339322f&clientId=ue4a35858-d49f-4&from=paste&id=u14a8d4e0&originHeight=181&originWidth=471&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u83b1864c-8656-4163-bce4-75cef96646c&title=)<br />如图所示，当注入的MessageNotifier时加了`@Lazy`注解，那么此时注入的其实是`MessageNotifier`的代理对象，而真正的`MessageNotifier`对象并没有创建，图中代理对象称为`MessageNotifierProxy`。<br />由于注入的是对象是代理对象`MessageNotifierProxy`，那么真正被使用的就是`MessageNotifierProxy`，一旦调用了`MessageNotifierProxy`的方法，此时`MessageNotifierProxy`会去Spring容器中查找真正的`MessageNotifier`对象，然后再调用`MessageNotifier`对象的方法。<br />代码如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551546-ab5034cd-9e80-456f-869c-3480644931e6.png#averageHue=%23302c2b&clientId=ue4a35858-d49f-4&from=paste&id=u82f7ff27&originHeight=776&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf3a9223b-b909-410f-97a0-52d76a6291b&title=)<br />这就是`@Lazy`延迟注入的原理。并不是不注入，而是注入一个代理对象，可以理解为一个占位符，一个空壳子，先占着位置，等用到这个壳子的时候，这个壳子会去查找到真正的对象，调用真正对象的方法。
<a name="x5jD7"></a>
### 6、`Optional`
`Optional`是JDK1.8提供的一个api，可以优雅的解决判空的问题。<br />`@Autowired`也支持了注入`Optional`类型。
```java
@Autowired
private Optional<MessageNotifier> messageNotifier;
```
代码如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551513-dd0bbb28-f9af-4338-806b-f5aa98be5a84.png#averageHue=%23372c2a&clientId=ue4a35858-d49f-4&from=paste&id=ue487b528&originHeight=165&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6c32aaae-b55f-449b-89b2-05ac38c2718&title=)<br />注入`Optional`这种方式可以解决注入的对象不存在的导致异常问题，也就是安全注入。<br />比如说，`MessageNotifier`这个对象Spring容器中并没有，如果直接注入，此时会抛`NoSuchBeanDefinitionException`异常<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551643-830ff207-c16a-49a1-8827-cc1ae67c2546.png#averageHue=%233e3231&clientId=ue4a35858-d49f-4&from=paste&id=u71a619d6&originHeight=247&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaffd9496-c343-4a52-80be-55964d5cf72&title=)<br />而直接通过注入`Optional`的方式就可以解决这个问题。<br />除了通过`Optional`的方式之外，也可以直接把`@Autowired`的`required`的属性设置为false来解决注入对象不存在的问题。<br />那`Optional`存在的作用是啥？<br />其实`Optional`的作用仅仅是不用写为空的判断，这也是`Optional`这个类的作用作用，除了这个，跟直接`@Autowired`对象并没有其它区别。<br />注入`Optional`这种方式其实用的不多，在源码中几乎没有看见这种注入方式。
<a name="vKw41"></a>
### 7、`ObjectFactory`和`ObjectProvider`
`ObjectFactory`和`ObjectProvider`是Spring提供的两接口<br />![ObjectFactory](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551723-c461c4d2-466f-46a6-aeda-29ce31cdaa50.png#averageHue=%232d2c2b&clientId=ue4a35858-d49f-4&from=paste&id=u867fc498&originHeight=403&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5846644c-2fe3-49f4-ad9e-577eb60f8f3&title=ObjectFactory "ObjectFactory")<br />`ObjectProvider`继承了`ObjectFactory`<br />![ObjectProvider](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283551864-5afcffc6-c172-453d-8fb9-54e68240ecb9.png#averageHue=%2332302f&clientId=ue4a35858-d49f-4&from=paste&id=u327ae3d3&originHeight=427&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u02e2b895-7f21-4a7d-bf4b-b7da66ba6a9&title=ObjectProvider "ObjectProvider")<br />`@Autowired`也可以直接注入这两个接口。
```java
@Autowired
private ObjectFactory<MessageNotifier> messageNotifierObjectFactory;

@Autowired
private ObjectProvider<MessageNotifier> messageNotifierObjectProvider;
```
代码如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283552077-cf68d959-17f5-449b-85b3-408e1ff7885a.png#averageHue=%232d2b2b&clientId=ue4a35858-d49f-4&from=paste&id=udd59399c&originHeight=170&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74adc779-2077-4b72-823a-86079cd0944&title=)<br />从这段代码也可以看出，最终注入的其实是`DependencyObjectProvider`实现。<br />`ObjectFactory`也是用来做延迟注入的操作，跟`@Lazy`作用差不多，但是实现原理不一样。<br />用上面的例子来说，注入`ObjectFactory`的时候并有创建`MessageNotifier`对象。<br />当需要使用`MessageNotifier`的时候需要通过`ObjectFactory`的`getObject`方法获取，此时才会真正创建`MessageNotifier`对象。
```java
MessageNotifier messageNotifier = messageNotifierObjectFactory.getObject();
```
`getObject`实现如下<br />![getObject](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283552227-036641fb-8fdb-4790-9d77-38d5bff8a820.png#averageHue=%232f2c2b&clientId=ue4a35858-d49f-4&from=paste&id=uef0947fd&originHeight=690&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud9c3737b-0084-411a-9652-27e484b2837&title=getObject "getObject")<br />所以`@Async`注解导致的循环依赖异常不仅可以通过`@Lazy`注解解决，也可以通过注入`ObjectFactory`的方式解决。<br />同理，`ObjectProvider`也有延迟注入的功能，但是除了延迟注入之外，`ObjectProvider`额外提供了跟`Optional`安全注入的功能，这个功能`ObjectFactory`是没有的。<br />上面的例子中，当使用`ObjectFactory`的`getObject`方法时，如果Spring容器中不存在`MessageNotifier`对象，此时也会抛NoSuchBeanDefinitionException异常。<br />但是`ObjectProvider`额外提供的`getIfAvailable`方法就支持获取不存在的对象的功能，当通过`getIfAvailable`获取的对象不存在时，只会返回null，并不会出抛异常。<br />![getIfAvailable方法](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283552303-c958fac9-8d89-4e31-8adf-a797e54f4579.png#averageHue=%232d2c2b&clientId=ue4a35858-d49f-4&from=paste&id=u3dd92a69&originHeight=410&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u327fd83e-fa83-4ba6-8038-892ad5d287c&title=getIfAvailable%E6%96%B9%E6%B3%95 "getIfAvailable方法")<br />对比一下与`getObject`方法的实现，就是在获取对象的时候是否要求对象获取的对象不是必须的，这样获取不到就不会抛异常了。<br />`ObjectFactory`和`ObjectProvider`在框架内部中使用的还是比较多的。<br />就比如说，在MybatisPlus自动装配的时候就大量使用`ObjectProvider`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283552595-8dcdf29a-00bc-4fcf-85fc-a2e48a4a65c3.png#averageHue=%23312c2b&clientId=ue4a35858-d49f-4&from=paste&id=uffa6fd39&originHeight=459&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55652ba3-b2b1-4a91-8e61-285c18791e2&title=)<br />并且泛型类型就是数组或者是集合，跟前面说的都对应上了。<br />通过这种方式就可以安全的注入，当Spring容器有这些对象的时候MybatisPlus就使用这些，没有也不会报错。
<a name="kWEml"></a>
### 8、JSR-330 Provider
首先，来讲一下什么是JSR-330。<br />JSR是Java Specification Requests的缩写，是一种Java标准规范。<br />而330算是一个版本，除了330，听到的比较多的还有250。<br />这个规范定义了一些IOC的注解，熟知的比如`@Resource`、`@PostConstruct`、`@PreDestroy`注解都是JSR-250中提出的。<br />一些IOC的框架会基于这个标准来实现这些接口的功能，比如Spring、Dagger2等IOC框架都实现了这些注解的功能。<br />所以，如果不使用Spring框架，使用其它的IOC框架，那么`@Resource`、`@PostConstruct`、`@PreDestroy`注解都是可以生效的。<br />在JSR-330中，提出了javax.inject.Provider这个接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283552301-a691b28d-5ce6-4be1-8708-9e7c3335c5f1.png#averageHue=%232c2c2b&clientId=ue4a35858-d49f-4&from=paste&id=uca8574d3&originHeight=450&originWidth=998&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uce918c8f-79d7-474f-ad06-ec1b4166370&title=)<br />不过，想使用JSR-330这个接口，需要引入依赖
```xml
<dependency>
  <groupId>javax.inject</groupId>
  <artifactId>javax.inject</artifactId>
  <version>1</version>
</dependency>
```
Spring也支持注入这个类型的接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684283552517-d9e0f133-e714-47f8-acd9-801b301bb699.png#averageHue=%232c2b2b&clientId=ue4a35858-d49f-4&from=paste&id=u7828e622&originHeight=122&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u982bf06b-6515-4b64-bca3-fb7d0c44f9b&title=)<br />这个接口的功能跟前面提到的`ObjectFactory`功能是一样的，也支持延迟注入的功能。
<a name="Rzc9G"></a>
### 总结
到这Spring能够注入的Bean的8种类型就讲完了，其实这8种类型可以分为以下几种功能：

- 单一注入，就是注入一个单一的对象
- 集合注入，可以注入数组或者集合
- 延迟注入，比如`@Lazy`、`ObjectFactory`、`ObjectProvider`、JSR-330 Provider
- 安全注入，不存在不会抛异常，比如`Optional`、`ObjectProvider`

这几种方式并不是互斥的，比如说延迟注入也可以注入的是一个集合，前面举的MyBaisPlus自动装配时`ObjectProvider`的使用就是很好的例子。<br />同时虽然本文举例的是`@Autowird`注解和字段注入的方式，但上面提到的注入的Bean类型跟使用注解和注入方式没什么关系，`@Resource`注解，构造器注入，setter注入都是一样的。
