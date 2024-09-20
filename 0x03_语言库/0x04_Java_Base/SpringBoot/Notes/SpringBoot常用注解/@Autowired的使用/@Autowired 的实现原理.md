Java
<a name="NVNX2"></a>
## `@Autowired`使用
构造函数注入
```java
public Class Outer {
	private Inner inner;
	@Autowired
	public Outer(Inner inner) {
		this.inner = inner;
	}
}
```
属性注入
```java
public Class Outer {
	@Autowired
	private Inner inner;
}
```
方法注入
```java
public Class Outer {
	private Inner inner;
	public Inner getInner() {
		return inner;
	}
	@Autowired
	public void setInner(Inner inner) {
		this.inner = inner;
	}
}
```
目前绝大部分的代码都使用第2、第3种。第1种在bean实例化时完成，而第2、第3种的实现原理都是一样的，在属性填充时完成。这里将介绍第二第三种的是实现原理<br />在开始之前，如果自己设计`@Autowired`，应该怎么实现？做法还是比较简单的

- **通过反射查找bean的class下所有注解了**`**@Autowired**`**的字段和方法**
- **获取到字段，通过**`**getBean**`**(字段)获取到对应bean，然后再通过反射调用field的set将bean注入**
<a name="sbQ3s"></a>
## `@Autowired`源码分析
`AutowiredAnnotationBeanPostProcessor`类<br />该类是`@Autowired`的具体实现类，先预览一下类方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654957476859-f996cc3c-ff0a-46ae-9d92-14b6f9609032.png#clientId=u979cc668-29c3-4&from=paste&id=u57d72ccd&originHeight=795&originWidth=1065&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubabbf2ec-8042-49f1-90bd-4cb37887308&title=)<br />发现实际有机会介入bean的创建操作只有可能是后置处理器，用于后置处理的有3个方法，其中一个过时不用，分别是`postProcessMergedBeanDefinition`、`postProcessProperties`后置处理，再看一下这2个方法的具体代码
```java
public class AutowiredAnnotationBeanPostProcessor extends InstantiationAwareBeanPostProcessorAdapter
        implements MergedBeanDefinitionPostProcessor, PriorityOrdered, BeanFactoryAware {

    ...

    @Override
    public void postProcessMergedBeanDefinition(RootBeanDefinition beanDefinition, Class<?> beanType, String beanName) {
        // 1. 寻找bean中所有被@Autowired注释的属性，并将属性封装成InjectedElement类型
        InjectionMetadata metadata = findAutowiringMetadata(beanName, beanType, null);
        metadata.checkConfigMembers(beanDefinition);
    }

    ...

    @Override
    public PropertyValues postProcessProperties(PropertyValues pvs, Object bean, String beanName) {
        // 1. 寻找通过@Autowired注解的属性或者方法
        InjectionMetadata metadata = findAutowiringMetadata(beanName, bean.getClass(), pvs);
        try {
            // 2. 注入
            metadata.inject(bean, beanName, pvs);
        } catch (BeanCreationException ex) {
            throw ex;
        } catch (Throwable ex) {
            throw new BeanCreationException(beanName, "Injection of autowired dependencies failed", ex);
        }
        return pvs;
    }

    ...
}
```
跟猜想是一样的，首先先找出所有注解了`@Autowired`的属性或者方法，然后进行注入，当然`postProcessMergedBeanDefinition`后置处理器的调用肯定是在`postProcessProperties`之前的，这里回顾一下spring bean的创建过程。<br />2个处理器已用黄色标出<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654957476919-cc994dcb-d684-4a5f-a5ee-c798b375609f.png#clientId=u979cc668-29c3-4&from=paste&id=uf112a8a3&originHeight=2268&originWidth=1076&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc8aa302a-3d4d-4ea3-9090-65ee18f961e&title=)
<a name="qfJYg"></a>
### 1、查找所有`@Autowired`
```java
// 寻找bean中所有被@Autowired注释的属性，并将属性封装成InjectedElement类型
InjectionMetadata metadata = findAutowiringMetadata(beanName, beanType, null);

private InjectionMetadata findAutowiringMetadata(String beanName, Class<?> clazz, @Nullable PropertyValues pvs) {
	// Fall back to class name as cache key, for backwards compatibility with custom callers.
	// 获取缓存的key值，一般以beanName做key
	String cacheKey = (StringUtils.hasLength(beanName) ? beanName : clazz.getName());
	// Quick check on the concurrent map first, with minimal locking.
	// 从缓存中获取metadata
	InjectionMetadata metadata = this.injectionMetadataCache.get(cacheKey);
	// 检测metadata是否需要更新
	if (InjectionMetadata.needsRefresh(metadata, clazz)) {
		synchronized (this.injectionMetadataCache) {
			metadata = this.injectionMetadataCache.get(cacheKey);
			if (InjectionMetadata.needsRefresh(metadata, clazz)) {
				if (metadata != null) {
					metadata.clear(pvs);
				}
				// 通过clazz类，查找所有@Autowired的属性或者方法，并封装成InjectionMetadata类型
				metadata = buildAutowiringMetadata(clazz);
				// 将metadata加入缓存
				this.injectionMetadataCache.put(cacheKey, metadata);
			}
		}
	}
	return metadata;
}
```
可以看到spring依然在用缓存的方式提高性能，继续跟踪核心代码`buildAutowiringMetadata(clazz)`
```java
private InjectionMetadata buildAutowiringMetadata(final Class<?> clazz) {
	// 查看clazz是否有Autowired注解
	if (!AnnotationUtils.isCandidateClass(clazz, this.autowiredAnnotationTypes)) {
		return InjectionMetadata.EMPTY;
	}
	// 这里需要注意AutowiredFieldElement，AutowiredMethodElement均继承了InjectionMetadata.InjectedElement
	// 因此这个列表是可以保存注解的属性和被注解的方法的
	List<InjectionMetadata.InjectedElement> elements = new ArrayList<>();
	Class<?> targetClass = clazz;

	// 1. 通过do while循环，递归的往直接继承的父类寻找@Autowired
	do {
		final List<InjectionMetadata.InjectedElement> currElements = new ArrayList<>();

		// 2. 通过反射，获取所有属性，doWithLocalFields则是循环的对每个属性应用以下匿名方法
		ReflectionUtils.doWithLocalFields(targetClass, field -> {
			// 判断当前field属性是否含有@Autowired的注解
			MergedAnnotation<?> ann = findAutowiredAnnotation(field);
			if (ann != null) {
				// 返回该属性在类中的修饰符，如果等于static常量，则抛出异常，@Autowired不允许注解在静态属性上
				if (Modifier.isStatic(field.getModifiers())) {
					if (logger.isInfoEnabled()) {
						logger.info("Autowired annotation is not supported on static fields: " + field);
					}
					return;
				}
				// @Autowired有required属性，获取required的值，默认为true
				boolean required = determineRequiredStatus(ann);
				// 3. 将field封装成InjectedElement，并添加到集合中，这里用的是AutowiredFieldElement
				currElements.add(new AutowiredFieldElement(field, required));
			}
		});

		// 4. @Autowired可以注解在方法上
		ReflectionUtils.doWithLocalMethods(targetClass, method -> {
			Method bridgedMethod = BridgeMethodResolver.findBridgedMethod(method);
			if (!BridgeMethodResolver.isVisibilityBridgeMethodPair(method, bridgedMethod)) {
				return;
			}
			MergedAnnotation<?> ann = findAutowiredAnnotation(bridgedMethod);
			if (ann != null && method.equals(ClassUtils.getMostSpecificMethod(method, clazz))) {
				if (Modifier.isStatic(method.getModifiers())) {
					if (logger.isInfoEnabled()) {
						logger.info("Autowired annotation is not supported on static methods: " + method);
					}
					return;
				}
				if (method.getParameterCount() == 0) {
					if (logger.isInfoEnabled()) {
						logger.info("Autowired annotation should only be used on methods with parameters: " +
									method);
					}
				}
				boolean required = determineRequiredStatus(ann);
				PropertyDescriptor pd = BeanUtils.findPropertyForMethod(bridgedMethod, clazz);
				// 5. 将方法封装成InjectedElement，并添加到集合中，这里用的是AutowiredMethodElement
				currElements.add(new AutowiredMethodElement(method, required, pd));
			}
		});

		elements.addAll(0, currElements);
		// 返回直接继承的父类
		targetClass = targetClass.getSuperclass();
	}
	// 如果父类不为空则需要把父类的@Autowired属性或方法也找出
	while (targetClass != null && targetClass != Object.class);
	// 6. new InjectionMetadata(clazz, elements)，将找到的所有的待注入属性或方法生成metadata返回
	return InjectionMetadata.forElements(elements, clazz);
}
```

- **外层 **`**do … while …**`** 的循环被用于递归的查找父类的**`**@Autowired**`**属性或方法**
- **通过反射的方式获取到所有属性并循环验证每一个属性是否被**`**@Autowired**`**注解**
- **将查找到包含**`**@Autowired**`**注解的filed封装成**`**AutowiredFieldElement**`**，加入到列表中**
- **循环查找在方法上的注解**
- **将找到的方法封装成**`**AutowiredMethodElement**`**，并加入列表**

这里需要特别强调一点，`InjectedElement`被`AutowiredFieldElement`、`AutowiredMethodElement`所继承，他们都有各自的inject函数，实现各自的注入。因此改ArrayList elements是拥有2种类型的属性<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654957476861-29c215ab-2e95-4d16-b483-fe49e5e8a55d.png#clientId=u979cc668-29c3-4&from=paste&id=ue8165677&originHeight=296&originWidth=914&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u905dce87-a97d-4037-a691-1801e590ee1&title=)

- **将找到的所有元素列表和clazz作为参数生成metadata数据返回**
<a name="irFo9"></a>
### 2、注入
```java
// 注入
metadata.inject(bean, beanName, pvs);

public void inject(Object target, @Nullable String beanName, @Nullable PropertyValues pvs) throws Throwable {
	// 获取所有需要被注入的元素
	Collection<InjectedElement> checkedElements = this.checkedElements;
	Collection<InjectedElement> elementsToIterate =
		(checkedElements != null ? checkedElements : this.injectedElements);
	// 迭代的元素不为空
	if (!elementsToIterate.isEmpty()) {
		for (InjectedElement element : elementsToIterate) {
			if (logger.isTraceEnabled()) {
				logger.trace("Processing injected element of bean '" + beanName + "': " + element);
			}
			// 循环注入，这里有可能是AutowiredFieldElement也可能AutowiredMethodElement，因此调用的inject是2个不同的方法
			element.inject(target, beanName, pvs);
		}
	}
}
```
利用for循环，遍历刚刚查到到的elements列表，进行注入。<br />在上面有特别提醒，这里的element有可能是`AutowiredFieldElement`类型、或`AutowiredMethodElement`类型。各自代表`@Autowired`注解在属性上、以及注解在方法上的2种不同元素。因此他们调用的`element.inject(target, beanName, pvs);`也是不一样的
<a name="n00T1"></a>
#### 2.1 字段注入(`AutowiredFieldElement`)
```java
private class AutowiredFieldElement extends InjectionMetadata.InjectedElement {
	@Override
	protected void inject(Object bean, @Nullable String beanName, @Nullable PropertyValues pvs) throws Throwable {
		Field field = (Field) this.member;
		Object value;
		if (this.cached) {
			value = resolvedCachedArgument(beanName, this.cachedFieldValue);
		}
		else {
			// 专门用于注入的包装类，包装构造函数参数，方法参数或字段
			DependencyDescriptor desc = new DependencyDescriptor(field, this.required);
			// 设置class
			desc.setContainingClass(bean.getClass());
			// 需要被自动注入的beanNames，这里只有可能 = 1，方法注入时才有可能为多个
			Set<String> autowiredBeanNames = new LinkedHashSet<>(1);
			Assert.state(beanFactory != null, "No BeanFactory available");
			TypeConverter typeConverter = beanFactory.getTypeConverter();// 获取类型转换器
			try {
				// 通过beanFactory获取属性对应的值，比如需要调用getBean("b")获取依赖的属性单例，并且通过自动转型转为需要的类型
				value = beanFactory.resolveDependency(desc, beanName, autowiredBeanNames, typeConverter);
			}
			catch (BeansException ex) {
				throw new UnsatisfiedDependencyException(null, beanName, new InjectionPoint(field), ex);
			}
			synchronized (this) {
				if (!this.cached) {
					if (value != null || this.required) {
						this.cachedFieldValue = desc;
						// 注册依赖，
						registerDependentBeans(beanName, autowiredBeanNames);
						// 因为是属性注入，因此这里只有可能等于1
						if (autowiredBeanNames.size() == 1) {
							String autowiredBeanName = autowiredBeanNames.iterator().next();
							if (beanFactory.containsBean(autowiredBeanName) &&
								beanFactory.isTypeMatch(autowiredBeanName, field.getType())) {
								// 缓存当前value
								this.cachedFieldValue = new ShortcutDependencyDescriptor(
									desc, autowiredBeanName, field.getType());
							}
						}
					}
					else {
						this.cachedFieldValue = null;
					}
					this.cached = true;
				}
			}
		}
		if (value != null) {
			// 通过反射，将value值设置到bean中
			ReflectionUtils.makeAccessible(field);
			field.set(bean, value);
		}
	}
}
```
上方大部分的工作都在做待注入bean的获取以及类型的转换，如果深究下去可以再把spring Ioc讲一遍，但是核心还是`getBean`(字段)获取到对应bean…这里就关心核心的语句，就是这2句
```java
if (value != null) {
    // 通过反射，将value值设置到bean中
    ReflectionUtils.makeAccessible(field);
    field.set(bean, value);
}
```
spring通过反射的方式，调用field的set进行属性的注入
<a name="wV0LI"></a>
#### 2.2 方法注入(`AutowiredMethodElement`)
```java
private class AutowiredMethodElement extends InjectionMetadata.InjectedElement {

	@Override
	protected void inject(Object bean, @Nullable String beanName, @Nullable PropertyValues pvs) throws Throwable {
		if (checkPropertySkipping(pvs)) {
			return;
		}
		// @Autowired标注在方法上
		Method method = (Method) this.member;
		Object[] arguments;
		if (this.cached) {
			// Shortcut for avoiding synchronization...
			// 有缓存
			arguments = resolveCachedArguments(beanName);
		}
		else {
			// 没缓存，直接获取方法上所有的参数
			int argumentCount = method.getParameterCount();
			arguments = new Object[argumentCount];
			DependencyDescriptor[] descriptors = new DependencyDescriptor[argumentCount];
			Set<String> autowiredBeans = new LinkedHashSet<>(argumentCount);
			Assert.state(beanFactory != null, "No BeanFactory available");
			TypeConverter typeConverter = beanFactory.getTypeConverter();
			// 循环所有参数
			for (int i = 0; i < arguments.length; i++) {
				MethodParameter methodParam = new MethodParameter(method, i);
				DependencyDescriptor currDesc = new DependencyDescriptor(methodParam, this.required);
				currDesc.setContainingClass(bean.getClass());
				descriptors[i] = currDesc;
				try {
					// 通过beanFactory，获取代注入的bean，并进行类型转换
					Object arg = beanFactory.resolveDependency(currDesc, beanName, autowiredBeans, typeConverter);
					if (arg == null && !this.required) {
						arguments = null;
						break;
					}
					arguments[i] = arg;
				}
				catch (BeansException ex) {
					throw new UnsatisfiedDependencyException(null, beanName, new InjectionPoint(methodParam), ex);
				}
			}
			synchronized (this) {
				if (!this.cached) {
					if (arguments != null) {
						DependencyDescriptor[] cachedMethodArguments = Arrays.copyOf(descriptors, arguments.length);
						// 注册依赖
						registerDependentBeans(beanName, autowiredBeans);
						// 如果自动注入的个数 = 参数个数，则缓存
						if (autowiredBeans.size() == argumentCount) {
							Iterator<String> it = autowiredBeans.iterator();
							Class<?>[] paramTypes = method.getParameterTypes();
							for (int i = 0; i < paramTypes.length; i++) {
								String autowiredBeanName = it.next();
								if (beanFactory.containsBean(autowiredBeanName) &&
									beanFactory.isTypeMatch(autowiredBeanName, paramTypes[i])) {
									// 缓存
									cachedMethodArguments[i] = new ShortcutDependencyDescriptor(
										descriptors[i], autowiredBeanName, paramTypes[i]);
								}
							}
						}
						// 缓存方法
						this.cachedMethodArguments = cachedMethodArguments;
					}
					else {
						this.cachedMethodArguments = null;
					}
					this.cached = true;
				}
			}
		}
		if (arguments != null) {
			try {
				// 反射调用注入方法，将获取到的所有bean作为参数
				ReflectionUtils.makeAccessible(method);
				method.invoke(bean, arguments);
			}
			catch (InvocationTargetException ex) {
				throw ex.getTargetException();
			}
		}
	}

}
```
这里与属性注入最大的区别在于，`@Autowired`注解在方法上，方法可以拥有多个参数，因此这里需要通过循环将一个个获取，而获取bean的方式于上面一样，本质都是通过getBean获取。<br />而核心语句还是2句
```java
// 反射调用注入方法，将获取到的所有bean作为参数
ReflectionUtils.makeAccessible(method);
method.invoke(bean, arguments);
```
与属性注入不同的是，当`@Autowired`注解在方法上，例如注解在`setter`方法上，则只需要直接调用该`setter`方法将参数数组传入即可以，即使用`invoke`触发方法，具体属性赋值的过程在`setter`方法中由用户自行编写。
