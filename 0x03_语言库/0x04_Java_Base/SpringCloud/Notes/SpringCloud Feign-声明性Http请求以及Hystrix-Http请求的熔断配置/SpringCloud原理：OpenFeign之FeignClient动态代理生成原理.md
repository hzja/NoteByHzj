Java SpringCloudOpenFeignFeignClient
<a name="DoUbx"></a>
## 一、`@EnableFeignClinets`作用源码剖析
要使用feign，必须要使用`@EnableFeignClinets`来激活，这个注解其实就是整个feign的入口，接下来着重分析一下这个注解干了什么事。
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@Import(FeignClientsRegistrar.class)
public @interface EnableFeignClients {
}
```
这个注解通过`@Import`注解导入一个配置类`FeignClientsRegistrar.class`，`FeignClientsRegistrar`实现了`ImportBeanDefinitionRegistrar`接口，所以Spring Boot在启动的时候，会去调用`FeignClientsRegistrar`类中的`registerBeanDefinitions`来动态往spring容器中注入bean。<br />接下来看一下`registerBeanDefinitions`的实现
```java
@Override
public void registerBeanDefinitions(AnnotationMetadata metadata,
      BeanDefinitionRegistry registry)
    //这个方式是注入一些配置，就是对EnableFeignClients注解属性的解析
    registerDefaultConfiguration(metadata, registry);
    //这个方法是扫秒加了@FeignClient注解
    registerFeignClients(metadata, registry);
}
```
这里着重分析`registerFeignClients`，看一看是如何扫描`@FeignClient`注解的，然后扫描到之后又做了什么。
```java
public void registerFeignClients(AnnotationMetadata metadata,
								 BeanDefinitionRegistry registry) {
	ClassPathScanningCandidateComponentProvider scanner = getScanner();
	scanner.setResourceLoader(this.resourceLoader);
	
	Set<String> basePackages;
	
	Map<String, Object> attrs = metadata
		.getAnnotationAttributes(EnableFeignClients.class.getName());
	AnnotationTypeFilter annotationTypeFilter = new AnnotationTypeFilter(
		FeignClient.class);
	final Class<?>[] clients = attrs == null ? null
		: (Class<?>[]) attrs.get("clients");
	if (clients == null || clients.length == 0) {
		scanner.addIncludeFilter(annotationTypeFilter);
		basePackages = getBasePackages(metadata);
	}
	else {
		final Set<String> clientClasses = new HashSet<>();
		basePackages = new HashSet<>();
		for (Class<?> clazz : clients) {
			basePackages.add(ClassUtils.getPackageName(clazz));
			clientClasses.add(clazz.getCanonicalName());
		}
		AbstractClassTestingTypeFilter filter = new AbstractClassTestingTypeFilter() {
			@Override
			protected boolean match(ClassMetadata metadata) {
				String cleaned = metadata.getClassName().replaceAll("\\$", ".");
				return clientClasses.contains(cleaned);
			}
		};
		scanner.addIncludeFilter(
			new AllTypeFilter(Arrays.asList(filter, annotationTypeFilter)));
	}
	
	for (String basePackage : basePackages) {
		Set<BeanDefinition> candidateComponents = scanner
			.findCandidateComponents(basePackage);
		for (BeanDefinition candidateComponent : candidateComponents) {
			if (candidateComponent instanceof AnnotatedBeanDefinition) {
				// verify annotated class is an interface
				AnnotatedBeanDefinition beanDefinition = (AnnotatedBeanDefinition) candidateComponent;
				AnnotationMetadata annotationMetadata = beanDefinition.getMetadata();
				Assert.isTrue(annotationMetadata.isInterface(),
							  "@FeignClient can only be specified on an interface");
				
				Map<String, Object> attributes = annotationMetadata
					.getAnnotationAttributes(
					FeignClient.class.getCanonicalName());
				
				String name = getClientName(attributes);
				registerClientConfiguration(registry, name,
											attributes.get("configuration"));
				
				registerFeignClient(registry, annotationMetadata, attributes);
			}
		}
	}
}
```
这段代码分析一下，先获取到了一个`ClassPathScanningCandidateComponentProvider`这个对象，这个对象是按照一定的规则来扫描指定目录下的类的，符合这个规则的每个类，会生成一个`BeanDefinition`。<br />获取到`ClassPathScanningCandidateComponentProvider`对象，配置这个对象，指定这个对象需要扫描出来标有`@FeignClient`注解的类；随后解析`EnableFeignClients`注解，获取内部的属性，获取到指定的需要扫描包路径下，如果没有指定的，那么就默认是当前注解所在类的所在目录及子目录。<br />然后就遍历每个目录，找到每个标有`@FeignClient`注解的类，对每个类就生成一个`BeanDefinition`，可以把`BeanDefinition`看成对每个标有`@FeignClient`注解的类信息的封装。<br />拿到一堆`BeanDefinition`之后，会遍历`BeanDefinition`，然后调用`registerClientConfiguration`和`registerFeignClient`方法。<br />接下来分别剖析一下这两个方法的作用<br />`**registerClientConfiguration**`**：**
```java
private void registerClientConfiguration(BeanDefinitionRegistry registry, Object name,
										 Object configuration) {
	BeanDefinitionBuilder builder = BeanDefinitionBuilder
		.genericBeanDefinition(FeignClientSpecification.class);
	builder.addConstructorArgValue(name);
	builder.addConstructorArgValue(configuration);
	registry.registerBeanDefinition(
		name + "." + FeignClientSpecification.class.getSimpleName(),
		builder.getBeanDefinition());
  }
```
这里的作用就是拿出在`@FeignClient`指定的配置类，也就是`configuration`属性，然后构建一个bean class为`FeignClientSpecification`，传入配置。这个类的最主要作用就是将每个Feign的客户端的配置类封装成一个`FeignClientSpecification`的`BeanDefinition`，注册到spring容器中。记住这个`FeignClientSpecification`，后面会有用。<br />**registerFeignClient：**
```java
private void registerFeignClient(BeanDefinitionRegistry registry,
								 AnnotationMetadata annotationMetadata, Map<String, Object> attributes) {
	String className = annotationMetadata.getClassName();
	BeanDefinitionBuilder definition = BeanDefinitionBuilder
		.genericBeanDefinition(FeignClientFactoryBean.class);
	validate(attributes);
	definition.addPropertyValue("url", getUrl(attributes));
	definition.addPropertyValue("path", getPath(attributes));
	String name = getName(attributes);
	definition.addPropertyValue("name", name);
	String contextId = getContextId(attributes);
	definition.addPropertyValue("contextId", contextId);
	definition.addPropertyValue("type", className);
	definition.addPropertyValue("decode404", attributes.get("decode404"));
	definition.addPropertyValue("fallback", attributes.get("fallback"));
	definition.addPropertyValue("fallbackFactory", attributes.get("fallbackFactory"));
	definition.setAutowireMode(AbstractBeanDefinition.AUTOWIRE_BY_TYPE);
	
	String alias = contextId + "FeignClient";
	AbstractBeanDefinition beanDefinition = definition.getBeanDefinition();
	
	boolean primary = (Boolean) attributes.get("primary"); // has a default, won't be
	// null
	
	beanDefinition.setPrimary(primary);
	
	String qualifier = getQualifier(attributes);
	if (StringUtils.hasText(qualifier)) {
		alias = qualifier;
	}
	
	BeanDefinitionHolder holder = new BeanDefinitionHolder(beanDefinition, className,
														   new String[] { alias });
	BeanDefinitionReaderUtils.registerBeanDefinition(holder, registry);
}
```
`registerFeignClient`这个方法很重要，来说一下大概做了哪些事。重新构造了一个`BeanDefinition`，这个`BeanDefinition`的指定的class类型是`FeignClientFactoryBean`，这个类实现了`FactoryBean`接口，对spring有一定了解的小伙伴应该知道，spring在生成bean的时候，判断`BeanDefinition`中bean的class如果是`FactoryBean`的实现的话，会调用这个实现类的`getObject`来获取对象。<br />到这一步，`@EnableFeignClinets`的作用就说完了。这个类的主要作用是扫描指定（不指定就默认路径下的）所有加了`@FeignClient`注解的类，然后每个类都会生成一个`BeanDefinition`，随后遍历每个`BeanDefinition`，然后取出每个`@FeignClient`注解的属性，构造新的`BeanDefinition`，传入`FeignClientFactoryBean`的class，随后注入到spring容器中；同时有配置类的也会将配置类构件出一个bean class为`FeignClientSpecification`的`BeanDefinition`注入到spring容器中。<br />为了便于理解，这里画个图来总结一下这个注解干了什么事。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650963990924-170e0dc7-a607-434d-9155-ad1437fe383c.png#clientId=u2cb1149e-1038-4&from=paste&id=u0c027da9&originHeight=363&originWidth=972&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10cde412-6959-4b81-94ff-0afea3b3e52&title=)
<a name="wyo51"></a>
## 二、Feign客户端接口动态代理的生成源码剖析
<a name="BmBjc"></a>
### （1）`FeignAutoConfiguration`源码剖析
`FeignAutoConfiguration`是feign在整个springcloud的配置类，拎出这里面比较核心的代码。
```java
@Autowired(required = false)
private List<FeignClientSpecification> configurations = new ArrayList<>();
@Bean
public FeignContext feignContext() {
    FeignContext context = new FeignContext();
    context.setConfigurations(this.configurations);
    return context;
}
```
注入了一堆`FeignClientSpecification`，`FeignClientSpecification`这玩意就是上文提到的调用`registerClientConfiguration`的时候注入到spring容器中的，一个Feign客户端的配置一个`FeignClientSpecification`，所以是个集合，然后封装到`FeignContext`中，最后将`FeignContext`注入到spring容器中。<br />`FeignContext`也是很重要的一个东西，来分析一下它的源码
```java
public class FeignContext extends NamedContextFactory<FeignClientSpecification> {
	public FeignContext() {
		super(FeignClientsConfiguration.class, "feign", "feign.client.name");
	}
	
}
```
`FeignContext`继承了`NamedContextFactory`，构造的时候，传入了`FeignClientsConfiguration`，这个也很重要，来分析它们的作用。
<a name="a7WYs"></a>
### （2）`NamedContextFactory`源码剖析
先来说结论，`NamedContextFactory`的作用是用来进行配置隔离的，ribbon和feign的配置隔离都依赖这个抽象类。<br />何为配置隔离，因为每个Feign客户端都有可能有自己的配置，从`@FeignClient`注解的属性`configuration`可以看出，所以写了这个类，用来隔离每个客户端的配置，这就是为什么在构造`FeignContext`传入一堆`FeignClientSpecification`的原因，这里封装了每个客户端的配置类。<br />那是怎么实现的呢，拎出来一部分核心的源码，不重要的就忽略了。
```java
public abstract class NamedContextFactory<C extends NamedContextFactory.Specification>
        implements DisposableBean, ApplicationContextAware {

    private final String propertySourceName;

    private final String propertyName;

    private Map<String, AnnotationConfigApplicationContext> contexts = new ConcurrentHashMap<>();
    private Map<String, C> configurations = new ConcurrentHashMap<>();

    //父类 ApplicationContext ，也就是springboot所使用的ApplicationContext
    private ApplicationContext parent;
    // 这个是默认的额配置类
    private Class<?> defaultConfigType;

    public NamedContextFactory(Class<?> defaultConfigType, String propertySourceName,
                               String propertyName) {
        this.defaultConfigType = defaultConfigType;
        this.propertySourceName = propertySourceName;
        this.propertyName = propertyName;
    }

    @Override
    public void setApplicationContext(ApplicationContext parent) throws BeansException {
        this.parent = parent;
    }

    public void setConfigurations(List<C> configurations) {
        for (C client : configurations) {
            this.configurations.put(client.getName(), client);
        }
    }

    public Set<String> getContextNames() {
        return new HashSet<>(this.contexts.keySet());
    }

    protected AnnotationConfigApplicationContext getContext(String name) {
        if (!this.contexts.containsKey(name)) {
            synchronized (this.contexts) {
                if (!this.contexts.containsKey(name)) {
                    this.contexts.put(name, createContext(name));
                }
            }
        }
        return this.contexts.get(name);
    }

    protected AnnotationConfigApplicationContext createContext(String name) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        if (this.configurations.containsKey(name)) {
            for (Class<?> configuration : this.configurations.get(name)
                    .getConfiguration()) {
                context.register(configuration);
            }
        }
        for (Map.Entry<String, C> entry : this.configurations.entrySet()) {
            if (entry.getKey().startsWith("default.")) {
                for (Class<?> configuration : entry.getValue().getConfiguration()) {
                    context.register(configuration);
                }
            }
        }
        context.register(PropertyPlaceholderAutoConfiguration.class,
                this.defaultConfigType);
        context.getEnvironment().getPropertySources().addFirst(new MapPropertySource(
                this.propertySourceName,
                Collections.<String, Object>singletonMap(this.propertyName, name)));
        if (this.parent != null) {
// Uses Environment from parent as well as beans
            context.setParent(this.parent);
// jdk11 issue
// https://github.com/spring-cloud/spring-cloud-netflix/issues/3101
            context.setClassLoader(this.parent.getClassLoader());
        }
        context.setDisplayName(generateDisplayName(name));
        context.refresh();
        return context;
    }

    /**
     * Specification with name and configuration.
     */
    public interface Specification {

        String getName();

        Class<?>[] getConfiguration();

    }

}
```
分析一下每个成员变量的作用：

- `**contexts**`**：** 一个客户端一个对应的`AnnotationConfigApplicationContext`
- `**configurations**`**:** 一个客户端一个配置类的封装，对应到Feign的就是`FeignClientSpecification`
- `**parent**`**：** springboot真正启动的就是这个`ApplicationContext`
- `**defaultConfigType**`**：** 默认的配置类，对应Feign就是构造`FeignContext`是传入的`FeignClientsConfiguration`

分析一下核心的方法：<br />`**getContext**`：这个方法很简单，就是根据客户端名称从contexts获取对应的`AnnotationConfigApplicationContext`，获取不到就去创建一个，然后放入`contexts`<br />`**createContext**`：就是直接new了一个`AnnotationConfigApplicationContext`对象，然后按照按照配置的优先级顺序，一步步放入配置类，最后放入parent容器，也就是说每个客户端对应的容器，都有一个共同的父容器，同时如果每个客户端对应的容器获取不到的配置，都会再次从父容器中获取。这个结论还是很重要的。<br />其实所谓的配置隔离就是为每个客户端构建一个`AnnotationConfigApplicationContext`，然后基于这个`ApplicationContext`来解析配置类，这样就实现了配置隔离。<br />不知道大家有么有遇到过这个坑，就是在spring cloud环境中，监听类似`ContextRefreshedEvent`这种事件的时候，这个事件会无缘无故地触发很多次，其实就是这个原因就在这，因为spring的事件是有传播机制的，每个客户端对应的容器都要进行refresh，refresh完就会发这个事件，然后这个事件就会传给parent容器，也就是springboot启动的容器，就会再次触发，所以如果客户端很多，那么就会触发很多次。解决办法就是进行唯一性校验，只能启动一次就行了。
<a name="G2LFN"></a>
### （3）`FeignClientsConfiguration`源码剖析
说完`NamedContextFactory`，接下来说一下`FeignClientsConfiguration`的作用。<br />这是一个默认的配置类，里面配置了很多bean，这些bean都是生成Feign客户端动态代理的需要的，说几个重要的。
```java
@Bean
@ConditionalOnMissingBean
public Contract feignContract(ConversionService feignConversionService) {
    return new SpringMvcContract(this.parameterProcessors, feignConversionService);
}
```
这个的主要作用是用来解析`@FeignClient`接口中每个方法使用的springmvc的注解的，这也就是为什么`FeignClient`可以识别springmvc注解的原因。
```java
@Bean
@Scope("prototype")
@ConditionalOnMissingBean
public Feign.Builder feignBuilder(Retryer retryer) {
    return Feign.builder().retryer(retryer);
}
```
用来构建动态代理的类，通过这个类的`target`方法，就能生成`Feign`动态代理
```java
@Configuration(proxyBeanMethods = false)
@ConditionalOnClass({ HystrixCommand.class, HystrixFeign.class })
protected static class HystrixFeignConfiguration {
	
	@Bean
	@Scope("prototype")
	@ConditionalOnMissingBean
	@ConditionalOnProperty(name = "feign.hystrix.enabled")
	public Feign.Builder feignHystrixBuilder() {
		return HystrixFeign.builder();
	}
	
}
```
这个是`FeignClientsConfiguration`的内部类，是用来整合hystrix的，`@ConditionalOnProperty(name = "feign.hystrix.enabled")`，当在配置文件配置了`feign.hystrix.enabled=true`的时候，就开启了hystrix整合了Feign，然后调用Feign的接口就有了限流、降级的功能。<br />其实hystrix整合Feign很简单，就是在构造动态代理的时候加了点东西而已。其实不光是hystrix，spring cloud alibaba中的sentinel在整合Feign的适合也是按照这个套路来的。
<a name="GHRRK"></a>
### （4）构建动态代理的过程源码剖析
说完了前置的内容，接下来就来看一看动态代理是如何生成的。从上面已经知道了，`@EnableFeignClinets`会扫描出每个加了`@FeignClient`注解的接口，然后生成对应的`BeanDefinition`，最后重新生成一个bean class为`FeignClientFactoryBean`的`BeanDefinition`，注册到spring容器。<br />接下来就会根据`BeanDefinition`来生成feign客户端的代理对象了。上面提到，是通过`FeignClientFactoryBean`的`getObject`方法来获取到代理对象，接下来，就来着重分析一下`getObject`方法的实现。
```java
@Override
public Object getObject() throws Exception {
    return getTarget();
}
```
`getObject`是调用`getTarget()`来获取代理对象的。<br />`getTarget`方法
```java
<T> T getTarget() {
	FeignContext context = this.applicationContext.getBean(FeignContext.class);
	Feign.Builder builder = feign(context);
	
	if (!StringUtils.hasText(this.url)) {
		if (!this.name.startsWith("http")) {
			this.url = "http://" + this.name;
		}
		else {
			this.url = this.name;
		}
		this.url += cleanPath();
		return (T) loadBalance(builder, context,
							   new HardCodedTarget<>(this.type, this.name, this.url));
	}
	if (StringUtils.hasText(this.url) && !this.url.startsWith("http")) {
		this.url = "http://" + this.url;
	}
	String url = this.url + cleanPath();
	Client client = getOptional(context, Client.class);
	if (client != null) {
		if (client instanceof LoadBalancerFeignClient) {
			// not load balancing because we have a url,
			// but ribbon is on the classpath, so unwrap
			client = ((LoadBalancerFeignClient) client).getDelegate();
		}
		if (client instanceof FeignBlockingLoadBalancerClient) {
			// not load balancing because we have a url,
			// but Spring Cloud LoadBalancer is on the classpath, so unwrap
			client = ((FeignBlockingLoadBalancerClient) client).getDelegate();
		}
		builder.client(client);
	}
	Targeter targeter = get(context, Targeter.class);
	return (T) targeter.target(this, builder, context,
							   new HardCodedTarget<>(this.type, this.name, url));
  }
```
先从ioc容器中获取到`FeignContext`,`FeignContext`里面封装了每个Feign的配置，起到配置隔离的作用。<br />然后获取到一个`Feign.Builder`，默认是在`FeignClientsConfiguration`中配置的。然后调用feign方法。
```java
protected Feign.Builder feign(FeignContext context) {
    FeignLoggerFactory loggerFactory = get(context, FeignLoggerFactory.class);
    Logger logger = loggerFactory.create(this.type);

    // @formatter:off
    Feign.Builder builder = get(context, Feign.Builder.class)
        // required values
        .logger(logger)
        .encoder(get(context, Encoder.class))
        .decoder(get(context, Decoder.class))
        .contract(get(context, Contract.class));
    // @formatter:on

    //这个是从配置文件中读取feign的配置
    configureFeign(context, builder);

    return builder;
  }
```
这个方法很简单，就是从每个`FeignClient`对应的ioc容器中获取到对应的组件，填充到 `Feign.Builder`中，默认都是`FeignClientsConfiguration`配置的。`configureFeign`这个方法不用去care它，它是默认从配置文件读取feign的配置，然后对`Feign.Builder`进行配置，有可能会覆盖从每个`FeignClient`对应的ioc容器中获取到对应的组件，所以配置文件的优先级是最高的，但是一般没人这么玩，所以就不用担心。<br />上面获取到的各种组件都是默认的，如果有需要替换什么组件，都可以实现，然后通过`@FeignClient`的`configuration`配置，就可以替换这些组件。<br />接下来就是走这段代码
```java
if (!StringUtils.hasText(this.url)) {
	if (!this.name.startsWith("http")) {
		this.url = "http://" + this.name;
	}
	else {
		this.url = this.name;
	}
	this.url += cleanPath();
	return (T) loadBalance(builder, context,
						   new HardCodedTarget<>(this.type, this.name, this.url));
}
```
这段代码就是判断有没有指定url，url在哪指定的呢，就是在`@FeignClient`注解中指定的url属性，这个属性是主要是进行feign直连，什么叫直连，就是不通过注册中心，直接访问服务提供者，这个url就是配置服务提供者的ip和端口。在springcloud环境下，一般这个是不配置的，因为得从注册中心发现服务所在的ip和端口列表。所以从这y也可以看出，没有注册中心，feign也是能够跑的，只要指定服务提供者的ip和端口就行。<br />所以，一般这个url是空的，也就是会进入这段代码。其实很简单，就是配置一个url，name是服务名，也是在`@FeignClient`配置的。那这段代码什么意思呢，举个例子来说，假如服务名是ServiceA，那么拼出来就是 http://ServiceA ，就是这么简单。得到url之后就会走loadBalance方法，传入一个`HardCodedTarget`参数，这个参数是封装了Feign客户端接口的类型、服务的名称、还有刚构建的url，接下来进入loadBalance。
```java
protected <T> T loadBalance(Feign.Builder builder, FeignContext context,
							HardCodedTarget<T> target) {
	Client client = getOptional(context, Client.class);
	if (client != null) {
		builder.client(client);
		Targeter targeter = get(context, Targeter.class);
		return targeter.target(this, builder, context, target);
	}
	
	throw new IllegalStateException(
		"No Feign Client for loadBalancing defined. Did you forget to include spring-cloud-starter-netflix-ribbon?");
}
```
这个方法一上来就从feign客户端对应的ioc容器中获取一个Client，但是`FeignClientsConfiguration`里面没有配置Client这个bean，那是从哪来呢？<br />其实loadBalance这个方法名就可以想到是负载均衡，所以Client需要整合负载均衡的功能，说到负载均衡，当前优先想到ribbon，所以就引入了`FeignRibbonClientAutoConfiguration`这个配置类，这个类是Feign整合ribbon的配置类，当然目前来说，负载均衡组件不止ribbon，还有springcloud自己实现的spring-cloud-starter-loadbalancer这个组件，其实原理都是一样的。<br />这里就默认获取到了Client，那么接下来就放入`Feign.Builder`中，接下来获取到Targeter，Targeter是通过`FeignAutoConfiguration`来配置的，默认是`DefaultTargeter`，如果整合`hystrix`就需是`HystrixTargeter`，当然这里就是默认的`DefaultTargeter`。<br />接下来就会调用`DefaultTargeter`的target方法
```java
@Override
public <T> T target(FeignClientFactoryBean factory, Feign.Builder feign,
					FeignContext context, Target.HardCodedTarget<T> target) {
	return feign.target(target);
}
```
就是直接调用`Feign.Builder`的tartget方法，那么就进入这个方法
```java
public <T> T target(Target<T> target) {
	return build().newInstance(target);
}

public Feign build() {
	SynchronousMethodHandler.Factory synchronousMethodHandlerFactory =
		new SynchronousMethodHandler.Factory(client, retryer, requestInterceptors, logger,
											 logLevel, decode404, closeAfterDecode, propagationPolicy);
	ParseHandlersByName handlersByName =
		new ParseHandlersByName(contract, options, encoder, decoder, queryMapEncoder,
								errorDecoder, synchronousMethodHandlerFactory);
	return new ReflectiveFeign(handlersByName, invocationHandlerFactory, queryMapEncoder);
}
```
先调用`build`方法，这个方法就是将最开始填充到`Feign.Builder`给封装起来，构建了一个`ReflectiveFeign`，然后调用`ReflectiveFeign`的`newInstance`方法，传入`Target<T> target`，也就是前面传入的`HardCodedTarget`。
```java
@SuppressWarnings("unchecked")
@Override
public <T> T newInstance(Target<T> target) {
	Map<String, MethodHandler> nameToHandler = targetToHandlersByName.apply(target);
	Map<Method, MethodHandler> methodToHandler = new LinkedHashMap<Method, MethodHandler>();
	List<DefaultMethodHandler> defaultMethodHandlers = new LinkedList<DefaultMethodHandler>();
	
	for (Method method : target.type().getMethods()) {
		if (method.getDeclaringClass() == Object.class) {
			continue;
		} else if (Util.isDefault(method)) {
			DefaultMethodHandler handler = new DefaultMethodHandler(method);
			defaultMethodHandlers.add(handler);
			methodToHandler.put(method, handler);
		} else {
			methodToHandler.put(method, nameToHandler.get(Feign.configKey(target.type(), method)));
		}
	}
	InvocationHandler handler = factory.create(target, methodToHandler);
	T proxy = (T) Proxy.newProxyInstance(target.type().getClassLoader(),
										 new Class<?>[] {target.type()}, handler);
	
	for (DefaultMethodHandler defaultMethodHandler : defaultMethodHandlers) {
		defaultMethodHandler.bindTo(proxy);
	}
	return proxy;
}
```
这个方法来解释一下是来干什么的，其实很简单，通过Target拿到接口的类型，然后获取到所有的方法，遍历每个方法，处理之后放入`methodToHandler`中，然后通过`InvocationHandlerFactory`的create方法，传入`methodToHandler`和`Target`，获取到一个`InvocationHandler`，之后通过jdk的动态代理，生成一个代理对象，然后返回回去。`InvocationHandler`默认是`ReflectiveFeign.FeignInvocationHandler`，这里就不再继续翻下去了。<br />走到这里，终于看到了Feign客户端动态代理的生成，整个构造过程还是很复杂的。这里总结一下代理对象生成的过程，每个Feign客户端都有对应的一个spring容器，用来解析配置类，根据配置从容器获取到一个`Feign.Builder`，然后再从容器中获取每个组件，填充到`Feign.Builder`中，最后通过`Feign.Builder`的build方法来构造动态代理，构造的过程其实是属于feign包底下的。
<a name="ch9HJ"></a>
## 三、总结
本文主要是讲述了，在SpringCloud环境下，OpenFeign对于Feign客户端动态代理的的构造过程。最开始讲解了`@EnableFeignClinets`注解的作用开始，随后剖析了`FeignAutoConfiguration`和`FeignClientsConfiguration`配置类，同时提到了Feign对每个客户端都进行了配置的隔离，最后通过剖析`FeignClientFactoryBean`的`getObject`方法，来一步一步屡清楚动态代理的构建过程。<br />最后画一张图，来总结一下本文。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650963990851-e7167fc5-9919-4ce0-aeba-1d8b616cde0c.png#clientId=u2cb1149e-1038-4&from=paste&id=udda7d444&originHeight=754&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue7f3379f-a4ed-4a0a-8876-ea65a6cb829&title=)<br /> 
