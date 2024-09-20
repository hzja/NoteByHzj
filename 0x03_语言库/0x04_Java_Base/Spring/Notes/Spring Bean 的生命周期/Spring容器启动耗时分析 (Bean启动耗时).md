Java Spring
<a name="urnTq"></a>
### Spring bean 的生命周期

- 实例化（instantiate）, 用构造器创建一个对象
- 字段赋值（populate）
- 初始化(initialize)， 执行bean配置里的`init`方法或者`InitializingBean#afterPropertiesSet`方法
- 销毁（destruct）

实例化和字段赋值一般都很快，但是一些重型的bean被IOC容器创建时，需要调用远程服务或者执行耗时的操作，这些操作往往在init方法里实现。统计bean初始化耗时可以发现那些bean影响了系统的启动效率。业务方的bean可以推动业务优化，自己的bean也可以想方法优化性能。<br />那么如何统计初始化的耗时呢？
<a name="avlkj"></a>
### Spring bean初始化源码分析
`org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#initializeBean` 观察执行初始化方法的逻辑
```java
protected Object initializeBean(final String beanName, final Object bean, RootBeanDefinition mbd) {
	if (System.getSecurityManager() != null) {
		AccessController.doPrivileged(new PrivilegedAction<Object>() {
			@Override
			public Object run() {
				invokeAwareMethods(beanName, bean);
				return null;
			}
		}, getAccessControlContext());
	}
	else {
		invokeAwareMethods(beanName, bean);
	}
	
	Object wrappedBean = bean;
	if (mbd == null || !mbd.isSynthetic()) {
		// 初始化前spring提供的系统钩子
		wrappedBean = applyBeanPostProcessorsBeforeInitialization(wrappedBean, beanName);
	}
	
	try {
		// 执行初始化方法
		invokeInitMethods(beanName, wrappedBean, mbd);
	}
	catch (Throwable ex) {
		throw new BeanCreationException(
			(mbd != null ? mbd.getResourceDescription() : null),
			beanName, "Invocation of init method failed", ex);
	}
	if (mbd == null || !mbd.isSynthetic()) {
		// 初始化后spring提供的系统钩子
		wrappedBean = applyBeanPostProcessorsAfterInitialization(wrappedBean, beanName);
	}
	return wrappedBean;
}
```
`applyBeanPostProcessorsBeforeInitialization`做了什么？<br />取出所有实现`BeanPostProcessor`的bean，逐个执行一遍`postProcessBeforeInitialization`方法。<br />同理，`applyBeanPostProcessorsAfterInitialization`逻辑依然，只是执行的是`postProcessInitialization`方法。
```java
@Override
public Object applyBeanPostProcessorsBeforeInitialization(Object existingBean, String beanName)
	throws BeansException {
	
	Object result = existingBean;
	for (BeanPostProcessor beanProcessor : getBeanPostProcessors()) {
		result = beanProcessor.postProcessBeforeInitialization(result, beanName);
		if (result == null) {
			return result;
		}
	}
 return result;
```
<a name="ZHCs3"></a>
### Spring系统钩子 `BeanPostProcessor`
> Factory hook that allows for custom modification of new bean instances, e.g. checking for marker interfaces or wrapping them with proxies.
> ApplicationContexts can autodetect BeanPostProcessor beans in their bean definitions and apply them to any beans subsequently created. Plain bean factories allow for programmatic registration of post-processors, applying to all beans created through this factory

`BeanPostProcessor`接口仅仅提供两个方法，用在在初始化bean的时候进行定制开发。
```java
public interface BeanPostProcessor {
	
	Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException;
	
	Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException;
	
}
```
![2022-04-29-19-06-28-673418.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651230436789-05de0cac-026d-4423-88b9-8389f11d1112.png#clientId=ua5bc5b14-8503-4&from=ui&id=u4b9e49bd&originHeight=761&originWidth=629&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1439167&status=done&style=none&taskId=u2b97e48f-e5ef-4c44-8320-151ac9c459d&title=)
<a name="PlO9P"></a>
### Bean初始化耗时功能开发demo
简单demo
```java
package org.dubbo.server.service.tool;

import com.google.common.collect.Maps;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.stereotype.Component;

import java.util.Map;

@Component
public class TimeCostBeanPostProcessor implements BeanPostProcessor {
	
	Map<String, Long> costMap = Maps.newConcurrentMap();
	
	@Override
	public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
		costMap.put(beanName, System.currentTimeMillis());
		return bean;
	}
	
	@Override
	public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
		long start = costMap.get(beanName);
		long cost  = System.currentTimeMillis() - start;
		if (cost > 0) {
			costMap.put(beanName, cost);
			System.out.println("class: " + bean.getClass().getName()
							   + "\tbean: " + beanName
							   + "\ttime" + cost);
		}
		return bean;
	}
}
```
