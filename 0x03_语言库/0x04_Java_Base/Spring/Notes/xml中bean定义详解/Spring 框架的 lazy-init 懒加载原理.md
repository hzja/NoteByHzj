JavaSpring<br />普通的bean的初始化是在容器启动初始化阶段执行的，而被lazy-init修饰的bean 则是在从容器里第一次进行`context.getBean("")`时进行触发。<br />Spring 启动的时候会把所有bean信息(包括XML和注解)解析转化成Spring能够识别的`BeanDefinition`并存到Hashmap里供下面的初始化时用。<br />接下来对每个`BeanDefinition`进行处理，如果是懒加载的则在容器初始化阶段不处理，其他的则在容器初始化阶段进行初始化并依赖注入。<br />**Spring 容器初始化和bean初始化，** 容器的初始化有可能包括bean的初始化主要取决于该bean是否是懒加载的，特此说明怕误会 。。。:)
<a name="n4s9l"></a>
## 一、先睹为快
话不多说先写个例子看下这属性到底有什么作用，定义了一个叫做coffee的普通bean，代码如下：
<a name="E4IJY"></a>
### 普通非懒加载bean的演示
```java
package com.test.spring;

public class Coffee {

	public Coffee() {
		System.out.println("正在初始化bean !!!调用无参构造函数");
	}

}
```
```xml
<bean name="coffee" class="com.test.spring.Coffee"/>
```
```java
@Test
public void testLazyInit() {
	
	System.out.println("开始初始化Spring容器 ");
	
	// 非懒加载的bean会在容器初始化时进行bean的初始化，后面会拿Spring启动时的源码进行分析
	ApplicationContext context = new ClassPathXmlApplicationContext("spring-beans.xml");
	
	// 非懒加载的bean 的构造函数会在这个位置打印
	System.out.println("Spring容器初始化完毕");
	
	System.out.println("开始从容器中获取Bean");
	
	Coffee coffee = context.getBean("coffee", Coffee.class);
	
	System.out.println("获取完毕  bean :" + coffee);
}
```
运行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658227798076-def10890-5c16-4f5b-bd0b-fc50e8b12b48.png#averageHue=%23342f2f&clientId=u30a22dac-7a85-4&from=paste&id=u80b60e0e&originHeight=274&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u833c44fb-8a68-4d6c-94a1-26e46fb25c5&title=)
<a name="tdzO0"></a>
### 非懒加载bean的演示
```xml
<bean name="coffee" class="com.test.spring.Coffee" lazy-init="true" />
```
```java
@Test
public void testLazyInit() {

	System.out.println("开始初始化Spring容器 ");

	// 在初始化容器阶段不会对懒加载的bean进行初始化
	ApplicationContext context = new ClassPathXmlApplicationContext("spring-beans.xml");

	System.out.println("Spring容器初始化完毕");
	System.out.println("开始从容器中获取Bean");

	// 在这一阶段会对懒加载的bean进行初始化
	Coffee coffee = context.getBean("coffee", Coffee.class);

	System.out.println("获取完毕  bean :" + coffee);


}
```
运行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658227798158-84e07ecc-3722-4f5f-8221-0962498b3ca6.png#averageHue=%23342e2d&clientId=u30a22dac-7a85-4&from=paste&id=u5e8bbc8d&originHeight=303&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1a50beb5-fcdd-4040-a1cc-565f0592b81&title=)
<a name="VSywd"></a>
## 二、原理分析
Spring 启动时主要干俩件事：

1. 初始化容器
2. 对bean进行初始化并依赖注入。（懒加载的bean不做第二件）

但是对于大多数bean来说，bean的初始化以及依赖注入就是在容器初始化阶段进行的，只有懒加载的bean是当应用程序第一次进行getBean时进行初始化并依赖注入。<br />下面贴出代码看下<br />Spring 容器初始化代码如下就一行：
```java
ApplicationContext context = new ClassPathXmlApplicationContext("spring-beans.xml");

public ClassPathXmlApplicationContext(String[] configLocations, boolean refresh, ApplicationContext parent)
	throws BeansException {

	super(parent);
	setConfigLocations(configLocations);
	if (refresh) {
		// Spring ioc 启动入口 了解了refresh 就了解了ioc
		refresh();
	}
}
```
Spring 初始化入口 `refresh`（省略了部分根本次无关的代码，望理解，太长了影响阅读体验）
```java
public void refresh() throws BeansException, IllegalStateException {
	synchronized (this.startupShutdownMonitor) {
		// Prepare this context for refreshing.
		prepareRefresh();

		// Prepare the bean factory for use in this context.
		prepareBeanFactory(beanFactory);

		try {
			// Allows post-processing of the bean factory in context subclasses.
			postProcessBeanFactory(beanFactory);

			// Invoke factory processors registered as beans in the context.
			invokeBeanFactoryPostProcessors(beanFactory);

			// Register bean processors that intercept bean creation.
			registerBeanPostProcessors(beanFactory);
			// Instantiate all remaining (non-lazy-init) singletons.
			// 初始化所有非 懒加载的bean！！！！
			finishBeanFactoryInitialization(beanFactory);

			// Last step: publish corresponding event.
			finishRefresh();
		}
	}
}
```
第20行则是跟本次主题有关的，就是说在容器启动的时候只处理**non-lazy-init bean，懒加载的bean在Spring启动阶段根本不做任何处理下面看下源码就明白了**<br />点进去第20行的`finishBeanFactoryInitialization(beanFactory)`里头有个初始化**non-lazy-init bean**的函数 `preInstantiateSingletons()`<br />具体逻辑如下

1. 对beanNames 集合遍历获取每个`BeanDefinition`
2. 判断是否是懒加载的，如果不是则继续处理(non-lazy-init bean 不做处理)
3. 判断是否是factorybean 如果不是则进行实例化并依赖注入
```java
public void preInstantiateSingletons() throws BeansException {
	// 所有beanDefinition集合
	List<String> beanNames = new ArrayList<String>(this.beanDefinitionNames);
	// 触发所有非懒加载单例bean的初始化
	for (String beanName : beanNames) {
		// 获取bean 定义
		RootBeanDefinition bd = getMergedLocalBeanDefinition(beanName);
		// 判断是否是懒加载单例bean，如果是单例的并且不是懒加载的则在Spring 容器
		if (!bd.isAbstract() && bd.isSingleton() && !bd.isLazyInit()) {
			// 判断是否是FactoryBean
			if (isFactoryBean(beanName)) {
				final FactoryBean<?> factory = (FactoryBean<?>) getBean(FACTORY_BEAN_PREFIX + beanName);
				boolean isEagerInit;
				if (System.getSecurityManager() != null && factory instanceof SmartFactoryBean) {
					isEagerInit = AccessController.doPrivileged(new PrivilegedAction<Boolean>() {
						@Override
						public Boolean run() {
							return ((SmartFactoryBean<?>) factory).isEagerInit();
						}
					}, getAccessControlContext());
				}
			}else {
				// 如果是普通bean则进行初始化依赖注入，此 getBean(beanName)接下来触发的逻辑跟
				// context.getBean("beanName") 所触发的逻辑是一样的
				getBean(beanName);
			}
		}
	}
}
```
`getBean()` 方法是实现bean 初始化以及依赖注入的函数
```java
@Override
public Object getBean(String name) throws BeansException {   
    return doGetBean(name, null, null, false);
}
```
<a name="RR2sO"></a>
## 三、总结
对于被修饰为lazy-init的bean Spring初始化阶段不会进行init并且依赖注入，当第一次进行getBean时候进行初始化并依赖注入<br />对于非懒加载的bean getBean的时候会从缓存里头取 因为容器初始化阶段已经初始化了
```java
// 容器启动初始化 会初始化并依赖注入非懒加载的bean
ApplicationContext context = new ClassPathXmlApplicationContext("spring-beans.xml");

// lazy-init bean会进行第一次初始化并依赖注入  其他的会从缓存里取
Coffee coffee = context.getBean("coffee", Coffee.class);
```
