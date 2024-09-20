Java SpringSpring Security<br />在实际项目中使用到了SpringSecurity作为安全框架，会遇到需要放行一些接口，使其能匿名访问的业务需求。但是每当需要当需要放行时，都需要在security的配置类中进行修改，感觉非常的不优雅。<br />例如这样：
```java
.antMatchers("/captchaImage").anonymous()
```
所以想通过自定义一个注解，来进行接口匿名访问。在实现需求前，先了解一下security的两种方行思路。<br />第一种就是在 `configure(WebSecurity web)`方法中配置放行，像下面这样：
```java
@Override
public void configure(WebSecurity web) throws Exception {
    web.ignoring().antMatchers("/css/**", "/js/**", "/index.html", "/img/**", "/fonts/**", "/favicon.ico", "/verifyCode");
}
```
第二种方式是在 `configure(HttpSecurity http)`方法中进行配置：
```java
@Override
protected void configure(HttpSecurity httpSecurity) throws Exception
{
	httpSecurity
	.authorizeRequests()
	.antMatchers("/hello").permitAll()
	.anyRequest().authenticated()
}
```
两种方式最大的区别在于，第一种方式是不走 Spring Security 过滤器链，而第二种方式走 Spring Security 过滤器链，在过滤器链中，给请求放行。<br />在使用 Spring Security 的时候，有的资源可以使用第一种方式额外放行，不需要验证，例如前端页面的静态资源，就可以按照第一种方式配置放行。<br />有的资源放行，则必须使用第二种方式，例如登录接口。大家知道，登录接口也是必须要暴露出来的，不需要登录就能访问到的，但是却不能将登录接口用第一种方式暴露出来，登录请求必须要走 Spring Security 过滤器链，因为在这个过程中，还有其他事情要做，具体的登录流程想了解的可以自行百度。<br />了解完了security的两种放行策略后，开始实现<br />首先创建一个自定义注解
```java
@Target({ElementType.METHOD}) //注解放置的目标位置,METHOD是可注解在方法级别上
@Retention(RetentionPolicy.RUNTIME) //注解在哪个阶段执行
@Documented //生成文档
public @interface IgnoreAuth {
}
```
这里说明一下，`@Target({ElementType.METHOD})`的实现方式，注解只能标记在带有`@RequestMapping`注解的方法上。具体为什么下面的实现方式看完就懂了。<br />接下来创建一个security的配置类SecurityConfig并继承`WebSecurityConfigurerAdapter`
```java
@EnableGlobalMethodSecurity(prePostEnabled = true, securedEnabled = true)
public class SecurityConfig extends WebSecurityConfigurerAdapter
{

    @Autowired
    private RequestMappingHandlerMapping requestMappingHandlerMapping;

    /**
     * @ description: 使用这种方式放行的接口，不走 Spring Security 过滤器链，
     *                无法通过 SecurityContextHolder 获取到登录用户信息的，
     *                因为它一开始没经过 SecurityContextPersistenceFilter 过滤器链。
     * @ dateTime: 2021/7/19 10:22
     */
    @Override
    public void configure(WebSecurity web) throws Exception {
        WebSecurity and = web.ignoring().and();
        Map<RequestMappingInfo, HandlerMethod> handlerMethods = requestMappingHandlerMapping.getHandlerMethods();
        handlerMethods.forEach((info, method) -> {
            // 带IgnoreAuth注解的方法直接放行
            if (StringUtils.isNotNull(method.getMethodAnnotation(IgnoreAuth.class))) {
                // 根据请求类型做不同的处理
                info.getMethodsCondition().getMethods().forEach(requestMethod -> {
                    switch (requestMethod) {
                        case GET:
                            // getPatternsCondition得到请求url数组，遍历处理
                            info.getPatternsCondition().getPatterns().forEach(pattern -> {
                                // 放行
                                and.ignoring().antMatchers(HttpMethod.GET, pattern);
                            });
                            break;
                        case POST:
                            info.getPatternsCondition().getPatterns().forEach(pattern -> {
                                and.ignoring().antMatchers(HttpMethod.POST, pattern);
                            });
                            break;
                        case DELETE:
                            info.getPatternsCondition().getPatterns().forEach(pattern -> {
                                and.ignoring().antMatchers(HttpMethod.DELETE, pattern);
                            });
                            break;
                        case PUT:
                            info.getPatternsCondition().getPatterns().forEach(pattern -> {
                                and.ignoring().antMatchers(HttpMethod.PUT, pattern);
                            });
                            break;
                        default:
                            break;
                    }
                });
            }
        });
    }
}
```
在这里使用Spring提供的`RequestMappingHandlerMapping`类，可以通过`requestMappingHandlerMapping.getHandlerMethods();`获取到所有的`RequestMappingInfo`信息。<br />以下是源码部分，可不看，看了可以加深理解<br />这里简单说一下`RequestMappingHandlerMapping`的工作流程，便于理解。通过翻看源码<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649226398844-05967210-4eed-4791-a390-3fdb68adf4d8.jpeg)<br />继承关系如上图所示。<br />`AbstractHandlerMethodMapping`实现了`InitializingBean`接口
```java
public interface InitializingBean {
    void afterPropertiesSet() throws Exception;
}
```
`AbstractHandlerMethodMapping`类中通过`afterPropertiesSet`方法调用`initHandlerMethods`进行初始化
```java
public void afterPropertiesSet() {
	this.initHandlerMethods();
}

protected void initHandlerMethods() {
	String[] var1 = this.getCandidateBeanNames();
	int var2 = var1.length;

	for(int var3 = 0; var3 < var2; ++var3) {
		String beanName = var1[var3];
		if (!beanName.startsWith("scopedTarget.")) {
			this.processCandidateBean(beanName);
		}
	}

	this.handlerMethodsInitialized(this.getHandlerMethods());
}
```
再调用`processCandidateBean`方法：
```java
protected void processCandidateBean(String beanName) {
	Class beanType = null;
	
	try {
		beanType = this.obtainApplicationContext().getType(beanName);
	} catch (Throwable var4) {
		if (this.logger.isTraceEnabled()) {
			this.logger.trace("Could not resolve type for bean '" + beanName + "'", var4);
		}
	}
	
	if (beanType != null && this.isHandler(beanType)) {
		this.detectHandlerMethods(beanName);
	}
}
```
通过调用方法中的`isHandler`方法是不是`requestHandler`方法，可以看到源码是通过`RequestMapping`，Controller 注解进行判断的。
```java
protected boolean isHandler(Class<?> beanType) {
    return AnnotatedElementUtils.hasAnnotation(beanType, Controller.class) || AnnotatedElementUtils.hasAnnotation(beanType, RequestMapping.class);
}
```
判断通过后，调用`detectHandlerMethods` 方法将handler注册到HandlerMethod的缓存中。
```java
protected void detectHandlerMethods(Object handler) {
	Class<?> handlerType = handler instanceof String ? this.obtainApplicationContext().getType((String)handler) : handler.getClass();
	if (handlerType != null) {
		Class<?> userType = ClassUtils.getUserClass(handlerType);
		Map<Method, T> methods = MethodIntrospector.selectMethods(userType, (method) -> {
			try {
				return this.getMappingForMethod(method, userType);
			} catch (Throwable var4) {
				throw new IllegalStateException("Invalid mapping on handler class [" + userType.getName() + "]: " + method, var4);
			}
		});
		if (this.logger.isTraceEnabled()) {
			this.logger.trace(this.formatMappings(userType, methods));
		}
		
		methods.forEach((method, mapping) -> {
			Method invocableMethod = AopUtils.selectInvocableMethod(method, userType);
			this.registerHandlerMethod(handler, invocableMethod, mapping);
		});
	}
}
```
通过`registerHandlerMethod`方法将handler放到`private final Map<T, HandlerMethod> mappingLookup = new LinkedHashMap();`map中。<br />而`requestMappingHandlerMapping.getHandlerMethods()`方法就是获取所有的HandlerMapping。
```java
public Map<T, HandlerMethod> getHandlerMethods() {
    this.mappingRegistry.acquireReadLock();

    Map var1;
    try {
        var1 = Collections.unmodifiableMap(this.mappingRegistry.getMappings());
    } finally {
        this.mappingRegistry.releaseReadLock();
    }

    return var1;
}
```
最后就是对map进行遍历，判断是否带有`IgnoreAuth.class`注解，然后针对不同的请求方式进行放行。
```java
handlerMethods.forEach((info, method) -> {
	// 带IgnoreAuth注解的方法直接放行
	if (StringUtils.isNotNull(method.getMethodAnnotation(IgnoreAuth.class))) {
		// 根据请求类型做不同的处理
		info.getMethodsCondition().getMethods().forEach(requestMethod -> {
			switch (requestMethod) {
				case GET:
					// getPatternsCondition得到请求url数组，遍历处理
					info.getPatternsCondition().getPatterns().forEach(pattern -> {
						// 放行
						and.ignoring().antMatchers(HttpMethod.GET, pattern);
					});
					break;
				case POST:
					info.getPatternsCondition().getPatterns().forEach(pattern -> {
						and.ignoring().antMatchers(HttpMethod.POST, pattern);
					});
					break;
				case DELETE:
					info.getPatternsCondition().getPatterns().forEach(pattern -> {
						and.ignoring().antMatchers(HttpMethod.DELETE, pattern);
					});
					break;
				case PUT:
					info.getPatternsCondition().getPatterns().forEach(pattern -> {
						and.ignoring().antMatchers(HttpMethod.PUT, pattern);
					});
					break;
				default:
					break;
			}
		});
	}
});
```
看到这里就能理解最开始的强调的需标记在带有`@RequestMapping`注解的方法上。这里使用到的是`configure(WebSecurity web)`的放行方式。它是不走security的过滤链，是无法通过 `SecurityContextHolder` 获取到登录用户信息的，这点问题是需要注意的。
