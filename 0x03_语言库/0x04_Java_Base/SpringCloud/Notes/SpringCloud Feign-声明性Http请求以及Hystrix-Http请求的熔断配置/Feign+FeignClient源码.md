Java Fegin
<a name="onKhF"></a>
## **一个简单的Feign**
现在来实现一个简单的Feign，首先是要使用注解`@FeignClient`
```java
@FeignClient(value = "xdclass-hi",configuration = FeignConfig.class)
public interface SchedualServiceHi {
    @GetMapping(value = "/hi")
    String sayHiFromClientOne(@RequestParam(value = "name") String name);
}
```
<a name="XSiXE"></a>
## **FeignClient注解源码**
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface FeignClient {

@AliasFor("name")
String value() default "";
 
@AliasFor("value")
String name() default "";
 
@AliasFor("value")
String name() default "";
String url() default "";
boolean decode404() default false;

Class<?>[] configuration() default {};
Class<?> fallback() default void.class;

Class<?> fallbackFactory() default void.class;
}

String path() default "";

boolean primary() default true;
```
FeignClient注解被`@Target(ElementType.TYPE)`修饰，表示`FeignClient`的注解作用在所对应的目标接口上。`@Retention`这个注解会在class字节码文件里，在运行的时候通过反射就可以获取到。<br />`@Documented`表示这个注解会包含在javadoc里。
<a name="ob2yC"></a>
## **FeignClient配置**
默认的配置类是叫`FeignClientsConfiguration`，这一个类它是在netflix-core的jar包下，可以发现它是一个配置类，有很多相关的配置，如：bean，`feignRetryer`、`FeignLoggerFactory`等等。
```java
@Configuration
public class FeignClientsConfiguration {
	
	...//省略代码
		
	@Bean
	@ConditionalOnMissingBean
	public Decoder feignDecoder() {
	return new ResponseEntityDecoder(new SpringDecoder(this.messageConverters));
	}
	
	@Bean
	@ConditionalOnMissingBean
	public Encoder feignEncoder() {
		return new SpringEncoder(this.messageConverters);
	}
	
	@Bean
	@ConditionalOnMissingBean
	public Contract feignContract(ConversionService feignConversionService) {
		return new SpringMvcContract(this.parameterProcessors, feignConversionService);
	}
	
	...//省略代码
}
```
<a name="T1K1P"></a>
## **Feign的工作原理**
feign它是一个伪的客户端，就是它不会对任何的请求进行处理。Feign是通过处理注解这种来生成request的，从而来简化掉HTTP API开发的目的，就是开发可以用注解的这种方式来制定request api模板。在进行发送请求之前，feign就开始通过处理注解的这种方式来替换request的参数，这种方式就更加直接、易理解。
```java
private void registerDefaultConfiguration(AnnotationMetadata metadata,
										  BeanDefinitionRegistry registry) {
	Map<String, Object> defaultAttrs = metadata
		.getAnnotationAttributes(EnableFeignClients.class.getName(), true);
	
	if (defaultAttrs != null && defaultAttrs.containsKey("defaultConfiguration")) {
		String name;
		if (metadata.hasEnclosingClass()) {
			name = "default." + metadata.getEnclosingClassName();
		}
		else {
			name = "default." + metadata.getClassName();
		}
		registerClientConfiguration(registry, name,
									defaultAttrs.get("defaultConfiguration"));
	}
 }
```
首先可以看到会先检测是否有`@EnableFeignClients`的这个注解，如果是有这个注解的话，那么就会开启包来进行扫描。扫描被`@FeignClient`注解接口。源码如上<br />接着程序启动之后就通过包来进行扫描，如果这个类是有`@FeignClient`注解的话，那么就会把注解的信息取出来，连同类名也一起取出了，并且给`BeanDefinitionBuilder`，然后得到了`beanDefinition`，最后把`beanDefinition`注入到ioc容器里。
<a name="ttjNF"></a>
## **Client组件**
Client它是一个非常重要的一个组件，Feign最终发送的request请求并且接受response的响应，它都是由Client组件来完成的。先看看`FeignRibbonClient`的自动配置类，主要是在启动的时候注入bean。
```java
@ConditionalOnClass({ ILoadBalancer.class, Feign.class })
@Configuration
@AutoConfigureBefore(FeignAutoConfiguration.class)
public class FeignRibbonClientAutoConfiguration {
	
	@Bean
	@ConditionalOnMissingBean
	public Client feignClient(CachingSpringLoadBalancerFactory cachingFactory,
							  SpringClientFactory clientFactory) {
		return new LoadBalancerFeignClient(new Client.Default(null, null),
										   cachingFactory, clientFactory);
	}
	
}
```
如果少了配置feignClient的情况，那么将会自动的注入`new Client.Default()`，使用的是`HttpURLConnection`
```java
@Override
public Response execute(Request request, Options options) throws IOException {
	HttpURLConnection connection = convertAndSend(request, options);
	return convertResponse(connection).toBuilder().request(request).build();
}
```
来看看feign是怎么使用HttpClient的
```java
@ConditionalOnClass({ ILoadBalancer.class, Feign.class })
@Configuration
@AutoConfigureBefore(FeignAutoConfiguration.class)
public class FeignRibbonClientAutoConfiguration {
	...//省略代码
		
		@Configuration
		@ConditionalOnClass(ApacheHttpClient.class)
		@ConditionalOnProperty(value = "feign.httpclient.enabled", matchIfMissing = true)
		protected static class HttpClientFeignLoadBalancedConfiguration {
			
			@Autowired(required = false)
			private HttpClient httpClient;
			
			@Bean
			@ConditionalOnMissingBean(Client.class)
			public Client feignClient(CachingSpringLoadBalancerFactory cachingFactory,
									  SpringClientFactory clientFactory) {
				ApacheHttpClient delegate;
				if (this.httpClient != null) {
					delegate = new ApacheHttpClient(this.httpClient);
				}
				else {
					delegate = new ApacheHttpClient();
				}
				return new LoadBalancerFeignClient(delegate, cachingFactory, clientFactory);
			}
		}
	
	...//省略代码
}
```
从代码可以知道，只需要在pom文件加上`HttpClient`的calsspath就可以了，也需要加上`feign.httpclient.enabled`是true。
<a name="vqAAU"></a>
## **Feign它的负载均衡是怎么实现的？**
通过上述的`FeignRibbonClientAutoConfiguration`类配置Client的类型(httpurlconnection，okhttp和httpclient)时候，可知最终向容器注入的是`LoadBalancerFeignClient`，即负载均衡客户端。
```java
@Override
public Response execute(Request request, Request.Options options) throws IOException {
	try {
		URI asUri = URI.create(request.url());
		String clientName = asUri.getHost();
		URI uriWithoutHost = cleanUrl(request.url(), clientName);
		FeignLoadBalancer.RibbonRequest ribbonRequest = new FeignLoadBalancer.RibbonRequest(
			this.delegate, request, uriWithoutHost);
		
		IClientConfig requestConfig = getClientConfig(options, clientName);
		return lbClient(clientName).executeWithLoadBalancer(ribbonRequest,
															requestConfig).toResponse();
	}
	catch (ClientException e) {
		IOException io = findIOException(e);
		if (io != null) {
			throw io;
		}
		throw new RuntimeException(e);
	}
}
```
里面有个`executeWithLoadBalancer`的方法，这个就是通过负载均衡的方式来请求
```java
public T executeWithLoadBalancer(final S request, final IClientConfig requestConfig) throws ClientException {
	RequestSpecificRetryHandler handler = getRequestSpecificRetryHandler(request, requestConfig);
	LoadBalancerCommand<T> command = LoadBalancerCommand.<T>builder()
		.withLoadBalancerContext(this)
		.withRetryHandler(handler)
		.withLoadBalancerURI(request.getUri())
		.build();
	
	try {
		return command.submit(
			new ServerOperation<T>() {
				@Override
				public Observable<T> call(Server server) {
					URI finalUri = reconstructURIWithServer(server, request.getUri());
					S requestForServer = (S) request.replaceUri(finalUri);
					try {
						return Observable.just(AbstractLoadBalancerAwareClient.this.execute(requestForServer, requestConfig));
					} 
					catch (Exception e) {
						return Observable.error(e);
					}
				}
			})
			.toBlocking()
			.single();
	} catch (Exception e) {
		Throwable t = e.getCause();
		if (t instanceof ClientException) {
			throw (ClientException) t;
		} else {
			throw new ClientException(e);
		}
	}
	
} 
```
最终负载均衡交给`loadBalancerContext`来处理，即之前讲述的Ribbon。
<a name="mmide"></a>
## **总结**
**通过**`**@EnableFeignCleints**`**注解开启FeignCleint**<br />**根据Feign的规则实现接口，并加上**`**@FeignCleint**`**注解**<br />**程序启动后，会进行扫描，所有注解的**`**@FeignCleint**`**的类，并将这些信息注入到ioc容器中。**<br />**RequesTemplate在生成Request**<br />**Request交给Client去处理，其中Client可以是**`**HttpUrlConnection**`**、**`**HttpClient**`**也可以是Okhttp**<br />**最后Client被封装到**`**LoadBalanceClient**`**类，这个类结合类Ribbon做到了负载均衡。**
