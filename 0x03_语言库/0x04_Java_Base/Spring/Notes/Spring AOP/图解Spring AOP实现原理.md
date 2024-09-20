JavaSpringAOP
<a name="Modka"></a>
### 准备工作

- **Service类： 有find方法跟save方法，find方法前后需要缓存操作，save方法前后需要事务操作**
- **CacheableAspect切面类： 用于在Service的find方法前后进行缓存操作**
- **TransctionlAspect切面类： 用于在Service的save方法进行事务操作**
- **LogAspect切面类： 记录所有Controller的请求日志**

![2022-06-12-21-27-09-820117.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040566346-f1d75090-043a-40ea-bd39-bcb75b7be3d8.png#clientId=u5fe428d1-79f8-4&from=ui&id=u461b051e&originHeight=213&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=691530&status=done&style=shadow&taskId=u0c490f49-218d-48fc-904c-e321f87098a&title=)
<a name="ImC8O"></a>
### Spring AOP自动代理时机
在service bean的创建过程中(也就是`getBean("service")`)，AOP通过`BeanPostProcess`后置处理器操作进行介入 分为2种情况：

- **用户自定义了**`**targetSource**`**，则bean的创建(实例化、填充、初始化)均由用户负责，Spring Ioc不会在管该代理目标对象traget，这种情况基本上不会发生，很多人用了几年Spring可能都不知道有它的存在**
- **正常情况下都是Spring Ioc完成代理对象target的实例化、填充、初始化。然后在初始化后置处理器中进行介入，对bean也就是service进行代理**

![2022-06-12-21-27-10-007122.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040567056-91ad8c58-1a43-4af0-a950-8f7befc9752e.png#clientId=u5fe428d1-79f8-4&from=ui&id=svfyV&originHeight=1237&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4015679&status=done&style=shadow&taskId=u006c9231-0943-44c8-8371-74ed9d4bf6d&title=)
<a name="w7oyK"></a>
### 创建代理操作`wrapIfNecessary`
所有的重点都在`wrapIfNecessary`做了什么操作<br />![2022-06-12-21-27-10-119320.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040566947-0b79ef9e-2bef-4338-b7aa-0ccb97098b5a.png#clientId=u5fe428d1-79f8-4&from=ui&id=xcayk&originHeight=1073&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3483295&status=done&style=shadow&taskId=ud38cab55-91d3-4ec1-ad1e-7b84097d19e&title=)
<a name="Mu1NL"></a>
### 代理的方法调用
创建完代理，代理如果调用方法呢？以Jdk动态代理为例子，方法的调用将会触发`invoke`方法。<br />![2022-06-12-21-27-10-307412.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040590042-0e442126-2c30-4e5a-ad15-dc9863f08d35.png#clientId=u5fe428d1-79f8-4&from=ui&id=ua2f1077f&originHeight=988&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3207357&status=done&style=shadow&taskId=ub97d1e33-c126-4978-8f64-bc1dc429289&title=)
<a name="r8wTS"></a>
### Spring AOP拦截器的执行顺序
从上面可以看出，Spring AOP的代理invoke方法，其实是拦截器的执行。<br />先了解一下Spring AOP的执行顺序，跟栈很像，后进先出<br />![2022-06-12-21-27-10-363011.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040589781-e5ef3d45-3030-4747-b3c8-a1b96e6b258e.png#clientId=u5fe428d1-79f8-4&from=ui&id=eBW3l&originHeight=494&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1603718&status=done&style=shadow&taskId=ub1ce358f-c338-43d7-ae88-b82ee1088bb&title=)
<a name="VSnpM"></a>
### AOP拦截器执行原理
那么这个拦截器链又如何保证before在after之前呢？而且还能保证find方法的执行顺序？<br />这部分实现原理通过几句话是解释不完的，只能给出图大家了解个大概<br />![2022-06-12-21-27-10-447294.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040604376-07c57925-67c0-42b5-8ae0-a384d66f65e9.png#clientId=u5fe428d1-79f8-4&from=ui&id=u902d8c95&originHeight=559&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1814730&status=done&style=shadow&taskId=uc10776d8-fa22-428b-b6e9-d378a9a01fd&title=)<br />具体拦截器源码：
```java
public class MethodBeforeAdviceInterceptor implements MethodInterceptor, BeforeAdvice, Serializable {

	private final MethodBeforeAdvice advice;

	@Override
	public Object invoke(MethodInvocation mi) throws Throwable {
		// 先调用before方法
		this.advice.before(mi.getMethod(), mi.getArguments(), mi.getThis());
		// 继续链式调用
		return mi.proceed();
	}

}

public class AfterReturningAdviceInterceptor implements MethodInterceptor, AfterAdvice, Serializable {

	private final AfterReturningAdvice advice;

	@Override
	public Object invoke(MethodInvocation mi) throws Throwable {
		// 先进行链式调用
		Object retVal = mi.proceed();
		// 再调用afterReturning方法
		this.advice.afterReturning(retVal, mi.getMethod(), mi.getArguments(), mi.getThis());
		return retVal;
	}

}
```
<a name="EGo1C"></a>
### 压轴题：Spring AOP遇上循环依赖
该部分难度系数十颗星，算是Spring源码最复杂的一块，这块弄懂可以说对Spring的理解已经达到阿里水准了。这里三言两语是不可能解释清楚的，只能贴个图，大家有个宏观上的认知。<br />![2022-06-12-21-27-10-609918.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1655040604514-aadfa62a-8d08-4510-8a20-f38ddafb3108.png#clientId=u5fe428d1-79f8-4&from=ui&id=y2KtO&originHeight=803&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2606806&status=done&style=shadow&taskId=ua14dc80d-2e5a-4e6f-ae26-daa518b73c1&title=)
<a name="XS7ga"></a>
### 总结
Spring AOP是Spring重要的组成部分之一，这里只是宏观上的一个认识，具体还是需要阅读源码。
