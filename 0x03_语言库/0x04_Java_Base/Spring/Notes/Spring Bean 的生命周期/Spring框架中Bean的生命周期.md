Java Spring
<a name="ftKGU"></a>
## 完整的bean生命周期
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219599628-aff2d423-31a7-4eb4-95f4-4a83bcad6eeb.jpeg#averageHue=%233a3a3a&clientId=uee3e8f3e-5320-4&from=paste&id=u9f9216e4&originHeight=1006&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u37675a6b-feef-4f6e-a727-c5225065dc0&title=)
<a name="Y0IiU"></a>
## 什么是Bean的生命周期
在Java中，万物皆对象，这些对象有生命周期：实例化 -> gc回收<br />而Bean同样也是Java中的对象，只是在这同时，Spring又赋予了它更多的意义。<br />于是将Bean从在Spring中创建开始，到Bean被销毁结束，这一过程称之为Bean的生命周期<br />那到底Bean在Spring中的创建过程是怎样的呢？
<a name="EpY49"></a>
## Bean的生命周期是怎样的
在Spring中，Bean的创建过程看起来复杂，但实际上逻辑分明。<br />如果将所有扩展性流程抛开，会发现只剩下两个流程：对象的实例化和属性填充
<a name="bDy2H"></a>
### 推导过程
开始时，只有两个流程：对象的实例化和属性填充<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219599644-8c01c457-d48b-49d6-8f54-027a64a352bc.jpeg#averageHue=%23363636&clientId=uee3e8f3e-5320-4&from=paste&id=u0e30fd9a&originHeight=416&originWidth=776&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9bd71f6-5b75-445c-9cd6-f8025e02fbf&title=)<br />对象的实例化就是在Java里使用类构造器进行创建对象。而一个类中可能有很多的构造器，那么怎么才能知道使用哪个构造器进行实例化对象呢？<br />所以说，在实例化之前，还得先做一件事情：确定候选的构造器，也称之为构造器推断
<a name="w0NZU"></a>
### 构造器推断
功能描述：找寻beanClass中所有符合候选条件的构造器。<br />负责角色：`AutowiredAnnotationBeanPostProcessor`<br />候选条件：构造器上添加了`@Autowired`注解<br />推断流程：<br />1、获取beanClass中的所有构造器进行遍历，判断构造器上是否标识`@Autowired`注解，是则将构造器添加到候选构造器集合中<br />2、并进一步判断`Autowired`注解中required属性是否为true(默认为true)，是则表示该beanClass已存在指定实例化的构造器，不可再有其他加了`@Autowired`注解的构造器，如果有则抛出异常。<br />3、如果`Autowired`注解中`required`属性为false，则可继续添加其他`@Autowired(required=false)`标识的构造器<br />4、如果候选构造器集合不为空(有`Autowired`标识的构造器)，并且beanClass中还有个空构造器，那么同样将空构造器也加入候选构造器集合中。<br />5、如果候选构造器集合为空，但是beanClass中只有一个构造器且该构造器有参，那么将该构造器加入候选构造器集合中。<br />流程图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219599676-9df1ddde-6c87-49b5-a8e3-362e570b9907.jpeg#averageHue=%23f7f5ee&clientId=uee3e8f3e-5320-4&from=paste&id=u03346b33&originHeight=883&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4d0d8658-a6bf-4385-8487-326532be4d3&title=)<br />当构造器遍历完毕之后，还有些许逻辑<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219599602-9b4b018d-66b1-400e-af7a-82ea327b2b39.jpeg#averageHue=%23f8f5f2&clientId=uee3e8f3e-5320-4&from=paste&id=uc4c4bee2&originHeight=888&originWidth=984&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf5b20cfe-406c-47ca-ba78-73ea466a55b&title=)<br />以上判断条件很多，但始终是围绕这一个逻辑：这个beanClass中有没有被`Autowired`标识的构造器，有的话required是true还是false，如果是true, 那其他的构造器都不要了。如果是false，那想加多少个构造器就加多少个。<br />咦，那要是没有`Autowired`标识的构造器呢？<br />框架嘛，都是要兜底的，这里就是看beanClass中是不是只有一个构造器且是有参的。<br />那要是只有个无参的构造器呢？<br />那确实就是没有候选的构造器了，但是Spring最后又兜底了一次，在没有候选构造器时默认使用无参构造器<br />那要是有很多个构造器呢？<br />Spring表示也不知道用哪个，同样进入兜底策略：使用无参构造器(没有将抛出异常)<br />那么这就是构造器推断流程了，将它加入到流程图中<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219599662-2eaea253-6dab-4a93-a9df-914db7e9f874.jpeg#averageHue=%23363535&clientId=uee3e8f3e-5320-4&from=paste&id=u67896935&originHeight=390&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub588ba3a-54e3-45b1-aa36-ecdb8a48253&title=)<br />在得到候选的构造器之后，就可以对对象进行实例化了，那么实例化的过程是怎样的呢？
<a name="rm3wa"></a>
### 对象实例化
功能描述：根据候选构造器集合中的构造器优先级对beanClass进行实例化。<br />负责角色：ConstructorResolver<br />对象实例化的过程主要有两个方面需要关注：<br />1、构造器的优先级是怎样的？<br />2、如果有多个构造器，但是有部分构造器的需要的bean并不存在于Spring容器中会发生什么？也就是出现了异常怎么处理？
<a name="Ubkyk"></a>
#### 1、构造器的优先级是怎样的？
在Java中，多个构造器称之为构造器重载，重载的方式有两种：参数的数量不同，参数的类型不同。<br />在Spring中，优先级则是由构造器的修饰符(public or private)和参数的数量决定。<br />规则如下：<br />1、public修饰的构造器  > private修饰的构造器<br />2、修饰符相同的情况下参数数量更多的优先<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219599947-58894d39-a133-489f-9df4-d1bf20806c9b.jpeg#averageHue=%23f9f9f7&clientId=uee3e8f3e-5320-4&from=paste&id=ucf550521&originHeight=998&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub46085a5-8920-4fd2-b4e8-0cac4e44634&title=)<br />这段流程很简单，代码只有两行：
```java
// 如果一个是public,一个不是,那么public优先
int result = Boolean.compare(Modifier.isPublic(e2.getModifiers()), Modifier.isPublic(e1.getModifiers()));
// 都是public，参数多的优先
return result != 0 ? result : Integer.compare(e2.getParameterCount(), e1.getParameterCount());
```
文中描述的规则是public > private, 只是为了更好的理解，实际上比较的是public和非public
<a name="XCIAe"></a>
#### 2、Spring是如何处理实例化异常的？
当一个beanClass中出现多个构造器，但是有部分构造器的需要的bean并不存在于Spring容器中，此时会发生什么呢？<br />比如以下案例中，InstanceA具有三个构造方法，其中InstanceB并未注入到Spring中
```java
@Component
public class InstanceA {
	
	@Autowired(required = false)
	public InstanceA(InstanceB instanceB){
		System.out.println("instance B ...");
	}
	
	@Autowired(required = false)
	public InstanceA(InstanceC instanceC){
		System.out.println("instance C ...");
	}
	
	@Autowired(required = false)
	public InstanceA(InstanceB instanceB, InstanceC instanceC, InstanceD InstanceD){
		System.out.println("instance B C D...");
	}
}
```
那么启动时是报错呢？还是选择只有InstanceC的构造器进行实例化？<br />运行结果会说明：Spring最终使用了只有InstanceC的构造器<br />这一部分的具体过程如下：<br />1、将根据优先级规则排序好的构造器进行遍历<br />2、逐个进行尝试查找构造器中的需要的bean是否都在Spring容器中，如果成功找到将该构造器标记为有效构造器，并立即退出遍历<br />3、否则记录异常继续尝试使用下一个构造器<br />4、当所有构造器都遍历完毕仍未找到有效的构造器，抛出记录的异常<br />5、使用有效构造器进行实例化<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600051-f9308ac4-41de-487c-a88e-cfd1ce1efa5c.jpeg#averageHue=%23faf9f9&clientId=uee3e8f3e-5320-4&from=paste&id=u319c6940&originHeight=748&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf2275340-1f50-45b3-bbae-581096434aa&title=)
<a name="aqH2D"></a>
### 推导过程
到这里，beanClass实例化了一个bean，接下来需要做的便是对bean进行赋值，Spring中可以进行赋值的对象不仅有通过`@Autowired`标识的属性，还可以是`@Value`,`@Resource`,`@Inject`等等。<br />为此，Spring为了达到可扩展性，将获取被注解标识的属性的过程与实际赋值的过程进行了分离。<br />该过程在Spring中被称为处理`beanDefinition`
<a name="ooCbl"></a>
### 处理`beanDefinition`
功能描述：处理`BeanDefintion`的元数据信息<br />负责角色：<br />1、`AutowiredAnnotationBeanPostProcessor`：处理`@Autowird`,`@Value`,`@Inject`注解<br />2、`CommonAnnotationBeanPostProcessor`：处理`@PostConstruct`,`@PreDestroy`,`@Resource`注解<br />这两个后置处理器的处理过程十分类似,  以`AutowiredAnnotationBeanPostProcessor`为例：<br />1、遍历beanClass中的所有`Field`、`Method`（java中统称为`Member`）<br />2、判断`Member`是否标识`@Autowird`,`@Value`,`@Inject`注解<br />3、是则将该`Member`保存，封装到一个叫做`InjectionMetadata`的类中<br />4、判断`Member`是否已经被解析过，比如一个`Member`同时标识了`@Autowired`和`@Resource`注解，那么这个`Member`就会被这两个后置处理器都处理一遍，就会造成重复保存<br />5、如果没被解析过就将该`Member`放置到已检查的元素集合中，用于后续填充属性时从这里直接拿到所有要注入的`Member`<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600057-559a315f-a6a9-44e0-a856-0dc2827eb50f.jpeg#averageHue=%23fafaf9&clientId=uee3e8f3e-5320-4&from=paste&id=u00495ea7&originHeight=942&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucb87ac27-c428-4bcf-8a54-94f548bff4b&title=)<br />其中，`AutowiredAnnotationBeanPostProcessor`和`InjectionMetadata`的结构如下<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600113-b082bd47-091a-45e5-928b-3caaa1ff451f.jpeg#averageHue=%23f4f3e0&clientId=uee3e8f3e-5320-4&from=paste&id=u963798ab&originHeight=739&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1428208b-13b5-43c5-aecf-6cea5fc4f5c&title=)<br />同样，将这一部分流程也加入到流程图中<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600197-1a6b07b5-2812-4f05-b37f-40e7e466889b.jpeg#averageHue=%23373737&clientId=uee3e8f3e-5320-4&from=paste&id=u00ece6ef&originHeight=457&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud9c1db1a-dc95-4723-a6b8-ef41d98bcc1&title=)<br />现在，beanClass中的可注入属性都找出来了，接下来就真的要进行属性填充了
<a name="VOHk7"></a>
### 属性填充
功能：对bean中需要自动装配的属性进行填充<br />角色：<br />1、`AutowiredAnnotationBeanPostProcessor`<br />2、`CommonAnnotationBeanPostProcessor`<br />在上一个流程中，已经找到了所有需要自动装配的`Member`，所以这一部流程就显得非常简单了<br />同样以`AutowiredAnnotationBeanPostProcessor`为例<br />1、使用beanName为key，从缓存中取出`InjectionMetadata`<br />2、遍历`InjectionMetadata`中的`checkedElements`集合<br />3、取出`Element`中的`Member`，根据`Member`的类型在Spring中获取Bean<br />4、使用反射将获取到的Bean设值到属性中<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600345-a101462a-5a8b-4fcf-96bf-8d993dd0f893.jpeg#averageHue=%23fbfbfb&clientId=uee3e8f3e-5320-4&from=paste&id=u2d3ffa84&originHeight=830&originWidth=940&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubb0e3451-eaf2-4b3b-bf7d-3f4464b59b6&title=)
<a name="CSuz0"></a>
### 推导过程
在Spring中，Bean填充属性之后还可以做一些初始化的逻辑，比如Spring的线程池`ThreadPoolTaskExecutor`在填充属性之后的创建线程池逻辑，`RedisTemplate`的设置默认值。<br />Spring的初始化逻辑共分为4个部分：<br />1、`invokeAwareMethods`：调用实现特定`Aware`接口的方法<br />2、`applyBeanPostProcessorsBeforeInitialization`：初始化前的处理<br />3、`invokeInitMethods`：调用初始化方法<br />4、`applyBeanPostProcessorsAfterInitialization`：初始化后的处理
<a name="B2ZRR"></a>
### `invokeAwareMethods`
这块逻辑非常简单，直接把源码粘出来给大家看看就明白了
```java
private void invokeAwareMethods(String beanName, Object bean) {
    if (bean instanceof Aware) {
        if (bean instanceof BeanNameAware) {
            ((BeanNameAware) bean).setBeanName(beanName);
        }
        if (bean instanceof BeanClassLoaderAware) {
            ClassLoader bcl = getBeanClassLoader();
            if (bcl != null) {
                ((BeanClassLoaderAware) bean).setBeanClassLoader(bcl);
            }
        }
        if (bean instanceof BeanFactoryAware) {
            ((BeanFactoryAware) bean).setBeanFactory(AbstractAutowireCapableBeanFactory.this);
        }
    }
 }
```
<a name="L71CX"></a>
### 初始化前的处理
功能：调用初始化方法前的一些操作<br />角色：<br />1、`InitDestroyAnnotationBeanPostProcessor`：处理`@PostContrust`注解<br />2、`ApplicationContextAwareProcessor`：处理一系列`Aware`接口的回调方法<br />这一步骤的功能没有太大的关联性，完全按照使用者自己的意愿决定想在初始化方法前做些什么，一个一个来过
<a name="VIXq1"></a>
#### 1、`InitDestroyAnnotationBeanPostProcessor`
这里的逻辑与属性填充过程非常相似，属性填充过程是取出自动装配相关的`InjectionMetadata`进行处理，而这一步则是取`@PostContrust`相关的`Metadata`进行处理，这个`Metadata`同样也是在处理`BeanDefinition`过程解析缓存的<br />1、取出处理`BeanDefinition`过程解析的`LifecycleMetadata`<br />2、遍历`LifecycleMetadata`中的`checkedInitMethods`集合<br />3、使用反射进行调用<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600415-e082c701-ce88-4433-8c98-1d67a7c6292b.jpeg#averageHue=%23f6f5f3&clientId=uee3e8f3e-5320-4&from=paste&id=u6aa7cb55&originHeight=702&originWidth=486&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u547e5c60-5807-42ff-99c0-d34adb2fbcd&title=)
<a name="Ain4S"></a>
#### 2、`ApplicationContextAwareProcessor`
这一步与`invokeAwareMethods`大同小异，只不过是其他的一些Aware接口，同样直接粘上代码
```java
private void invokeAwareInterfaces(Object bean) {
	if (bean instanceof EnvironmentAware) {
		((EnvironmentAware) bean).setEnvironment(this.applicationContext.getEnvironment());
	}
	if (bean instanceof EmbeddedValueResolverAware) {
		((EmbeddedValueResolverAware) bean).setEmbeddedValueResolver(this.embeddedValueResolver);
	}
	if (bean instanceof ResourceLoaderAware) {
		((ResourceLoaderAware) bean).setResourceLoader(this.applicationContext);
	}
	if (bean instanceof ApplicationEventPublisherAware) {
		((ApplicationEventPublisherAware) bean).setApplicationEventPublisher(this.applicationContext);
	}
	if (bean instanceof MessageSourceAware) {
		((MessageSourceAware) bean).setMessageSource(this.applicationContext);
	}
	if (bean instanceof ApplicationContextAware) {
		((ApplicationContextAware) bean).setApplicationContext(this.applicationContext);
	}
 }
```
<a name="BJhGK"></a>
### 初始化方法
在Spring中的初始化方法有两种<br />1、实现`InitializingBean`接口的`afterPropertiesSet`方法<br />2、`@Bean`注解中的`initMethod`属性<br />调用顺序是先调用`afterPropertiesSet`再`initMethod`<br />1、判断Bean是否实现`InitializingBean`接口<br />2、是则将Bean强转成`InitializingBean`，调用`afterPropertiesSet`方法<br />3、判断`BeanDefinition`中是否有`initMethod`<br />4、是则找到对应的`initMethod`，通过反射进行调用<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600472-36f01754-3044-4f76-8c13-a30ffd7cd31d.jpeg#averageHue=%23f9f8f7&clientId=uee3e8f3e-5320-4&from=paste&id=ufaad8187&originHeight=901&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0935d03d-a24f-4421-89a7-ca9922d6971&title=)
<a name="l4Z1P"></a>
### 初始化后的处理
在Spring的内置的后置处理器中，该步骤只有`ApplicationListenerDetector`有相应处理逻辑：将实现了`ApplicationListener`接口的bean添加到事件监听器列表中<br />如果使用了Aop相关功能，则会使用到`AbstractAutoProxyCreator`，进行创建代理对象。<br />`ApplicationListenerDetector`的流程如下<br />1、判断Bean是否是个`ApplicationListener`<br />2、是则将bean存放到`applicationContext`的监听器列表中
<a name="mIxRy"></a>
### 补充流程图
到这里，Bean的生命周期主要部分已经介绍完了，将流程图补充一下<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600502-a20781f0-19ab-455b-9f6d-321d03c6a591.jpeg#averageHue=%233a3a3a&clientId=uee3e8f3e-5320-4&from=paste&id=udc8de0a3&originHeight=735&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c1c7196-f5cd-454b-a342-a7de87b4c55&title=)<br />同样还有其他的一些逻辑
<a name="lDlsd"></a>
#### 1、中止创建Bean的过程
该过程处于Bean生命周期的最开始部分。<br />功能：由后置处理器返回Bean，达到中止创建Bean的效果<br />角色：无，Spring的内置后置处理器中，无实现。<br />Bean的生命周期十分复杂，Spring允许直接拦截，即在创建Bean之前由自定义的后置处理器直接返回一个Bean给Spring，那么Spring就会使用你给的Bean，不会再走Bean生命周期流程。<br />案例演示：
```java
@Component
public class Car {
	
	@Autowired
	private Person person;
	
	public void checkPerson(){
		if(person == null){
			System.out.println("person is null");
		}
	}
}
```
由于在`Person`属性上加了`@Autowired`，所以正常来说person必然不能为空，因为这是必须要注入的。<br />现在自定义一个`BeanPostProcessor`进行拦截
```java
@Component
public class InterruptBeanPostProcessor implements InstantiationAwareBeanPostProcessor {
	
	@Override
	public Object postProcessBeforeInstantiation(Class<?> beanClass, String beanName) throws BeansException {
		if("car".equals(beanName)){
			try {
				return beanClass.newInstance();
			} catch (InstantiationException | IllegalAccessException e) {
				e.printStackTrace();
			}
		}
		return null;
	}
}
```
测试结果如下<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600585-1605ee97-d847-411e-a1ad-adcc5a9eb223.jpeg#averageHue=%2340382b&clientId=uee3e8f3e-5320-4&from=paste&id=u018874c5&originHeight=351&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u26dd9328-f9bc-4c2d-bcb6-c3e4cfeeffa&title=)
<a name="SMi1Z"></a>
#### 2、提前缓存刚实例化的对象
该步骤跟随在Spring实例化bean之后，将bean进行缓存，其目的是为了解决循环依赖问题。<br />该过程暂时按下不表，单独提出放于循环依赖章节。
<a name="dh7u4"></a>
#### 3、中止填充属性操作
与中止创建Bean逻辑相同，Spring同样也允许在属性填充前进行拦截。在Spring的内置处理器中同样无该实现。<br />实现手段为实现`InstantiationAwareBeanPostProcessor`接口，在`postProcessAfterInstantiation`方法中返回false
```java
@Component
public class InterruptBeanPostProcessor implements InstantiationAwareBeanPostProcessor {
	
	@Override
	public boolean postProcessAfterInstantiation(Object bean, String beanName) throws BeansException {
		if(beanName.equals("car")){
			return false;
		}
		return true;
	}
}
```
<a name="wBXG1"></a>
#### 4、注册Bean的销毁方法
Spring中不仅有`@PostContrust`、`afterProperties`、`initMethod`这些bean创建时的初始化方法，同样也有bean销毁时的`@PreDestory`、`destroy`、`destroyMethod`。<br />所以在Bean的生命周期最后一步，Spring会将具备这些销毁方法的Bean注册到销毁集合中，用于系统关闭时进行回调。<br />比如线程池的关闭，连接池的关闭，注册中心的取消注册，都是通过它来实现的。
<a name="hvq7D"></a>
## 完整流程图
最后，附上开头的Bean生命周期的完整流程图，是不是就清晰了很多？<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1647219600870-2122a594-b72b-4e47-8d43-89daacf03984.jpeg#averageHue=%233a3a3a&clientId=uee3e8f3e-5320-4&from=paste&id=u4044d62a&originHeight=1006&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua2a6e8c2-a4aa-4892-8dd6-503381f4b19&title=)
