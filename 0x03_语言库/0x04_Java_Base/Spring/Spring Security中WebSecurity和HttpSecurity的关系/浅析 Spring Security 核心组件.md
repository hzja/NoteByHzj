JavaSpringSpring Security
<a name="MJu6Z"></a>
## Spring Security的核心类
Spring Security的核心类主要包括以下几个：

- `**SecurityContextHolder**`：存放身份信息的容器
- `**Authentication**`：身份信息的抽象接口
- `**AuthenticationManager**`：身份认证器，认证的核心接口
- `**UserDetailsService**`：一般用于从数据库中加载身份信息
- `**UserDetails**`：相比`Authentication`，有更详细的身份信息
<a name="rVMkx"></a>
### `SecurityContextHolder`、`Securityontext`和`Authentication`
`SecurityContextHolder`用于存储安全上下文(security context)的信息，即一个存储身份信息，认证信息等的容器。`SecurityContextHolder`默认使用 ThreadLocal策略来存储认证信息，即一种与线程绑定的策略，每个线程执行时都可以获取该线程中的 安全上下文(security context)，各个线程中的安全上下文互不影响。而且如果说要在请求结束后清除安全上下文中的信息，利用该策略Spring Security也可以轻松搞定。<br />因为身份信息是与线程绑定的，所以可以在程序的任何地方使用静态方法获取用户信息，一个获取当前登录用户的姓名的例子如下：
```java
Object principal = SecurityContextHolder.getContext().getAuthentication().getPrincipal();
if (principal instanceof UserDetails) {
	String username = ((UserDetails)principal).getUsername();
} else {
	String username = principal.toString();
}
```
`getAuthentication()`方法返回了认证信息，准确的说是一个 `Authentication`实例，`Authentication`是 Spring Security 中的一个重要接口，直接继承自 Principal类，该接口表示对用户身份信息的抽象，接口源码如下：
```java
public interface Authentication extends Principal, Serializable { 
	//权限信息列表，默认是 GrantedAuthority接口的一些实现
	Collection<? extends GrantedAuthority> getAuthorities(); 
	//密码信息，用户输入的密码字符串，认证后通常会被移除，用于保证安全
	Object getCredentials();
	//细节信息，web应用中通常的接口为 WebAuthenticationDetails，它记录了访问者的ip地址和sessionId的值
	Object getDetails();
	//身份信息，返回UserDetails的实现类
	Object getPrincipal();
	//认证状态，默认为false,认证成功后为 true
	boolean isAuthenticated();
	//上述身份信息是否经过身份认证 
	void setAuthenticated(boolean var1) throws IllegalArgumentException;
}
```
<a name="Gw6At"></a>
### `AuthenticationManager`、`ProviderManager` 和 `AuthenticationProvider`
`AuthenticationManager`是身份认证器，认证的核心接口，接口源码如下：
```java
public interface AuthenticationManager {
	/**
     * Attempts to authenticate the passed {@link Authentication} object, returning a
     * fully populated <code>Authentication</code> object (including granted authorities)
     * @param authentication the authentication request object
     *
     * @return a fully authenticated object including credentials
     *
     * @throws AuthenticationException if authentication fails
     */
	Authentication authenticate(Authentication authentication)
		throws AuthenticationException;
}
```
该接口只有一个 `authenticate()`方法，用于身份信息的认证，如果认证成功，将会返回一个带了完整信息的`Authentication`，在之前提到的`Authentication`所有的属性都会被填充。<br />在Spring Security中，`AuthenticationManager`默认的实现类是 `ProviderManager`，`ProviderManager`并不是自己直接对请求进行验证，而是将其委派给一个 `AuthenticationProvider`列表。列表中的每一个 `AuthenticationProvider`将会被依次查询是否需要通过其进行验证，每个 provider的验证结果只有两个情况：抛出一个异常或者完全填充一个 `Authentication`对象的所有属性。`ProviderManager`中的部分源码如下：
```java
public class ProviderManager implements AuthenticationManager, MessageSourceAware, InitializingBean {
	//维护一个AuthenticationProvider 列表
	private List<AuthenticationProvider> providers = Collections.emptyList();
	private AuthenticationManager parent;
	//构造器，初始化 AuthenticationProvider 列表
	public ProviderManager(List<AuthenticationProvider> providers) {
		this(providers, null);
	}
	public ProviderManager(List<AuthenticationProvider> providers,
						   AuthenticationManager parent) {
		Assert.notNull(providers, "providers list cannot be null");
		this.providers = providers;
		this.parent = parent;
		checkState();
	}
	public Authentication authenticate(Authentication authentication)
		throws AuthenticationException {
		Class<? extends Authentication> toTest = authentication.getClass();
		AuthenticationException lastException = null;
		Authentication result = null;
		boolean debug = logger.isDebugEnabled();
		// AuthenticationProvider 列表中每个Provider依次进行认证
		for (AuthenticationProvider provider : getProviders()) {
			if (!provider.supports(toTest)) {
				continue;
			}
			...
				try { 
					//调用 AuthenticationProvider 的 authenticate()方法进行认证
					result = provider.authenticate(authentication);
					if (result != null) {
						copyDetails(authentication, result);
						break;
					}
				}
			...
				catch (AuthenticationException e) {
					lastException = e;
				}
		}
		// 如果 AuthenticationProvider 列表中的Provider都认证失败，且之前有构造一个 AuthenticationManager 实现类，那么利用AuthenticationManager 实现类 继续认证
		if (result == null && parent != null) {
			// Allow the parent to try.
			try {
				result = parent.authenticate(authentication);
			}
			...
				catch (AuthenticationException e) {
					lastException = e;
				}
		}
		//认证成功
		if (result != null) {
			if (eraseCredentialsAfterAuthentication
				&& (result instanceof CredentialsContainer)) {
				// Authentication is complete. Remove credentials and other secret data
				// from authentication
				//成功认证后删除验证信息
				((CredentialsContainer) result).eraseCredentials();
			}
			//发布登录成功事件
			eventPublisher.publishAuthenticationSuccess(result);
			return result;
		}

		// 没有认证成功，抛出一个异常
		if (lastException == null) {
			lastException = new ProviderNotFoundException(messages.getMessage(
				"ProviderManager.providerNotFound",
				new Object[] { toTest.getName() },
				"No AuthenticationProvider found for {0}"));
		}
		prepareException(lastException, authentication);
		throw lastException;
}
```
ProviderManager中的 `authenticationManager`列表依次去尝试认证，认证成功即返回，认证失败返回null，如果所有的 Provider都认证失败， `ProviderManager`将会抛出一个 `ProviderNotFoundException`异常。<br />事实上，`AuthenticationProvider`是一个接口，接口定义如下：
```java
public interface AuthenticationProvider {
	//认证方法
	Authentication authenticate(Authentication authentication)
		throws AuthenticationException;
	//该Provider是否支持对应的Authentication
	boolean supports(Class<?> authentication);
}
```
在 ProviderManager的 Javadoc曾提到,
> If more than one AuthenticationProvider supports the passed Authentication object, the first one able to successfully authenticate the Authentication object determines the result, overriding any possible AuthenticationException thrown by earlier supporting AuthenticationProvider s. On successful authentication, no subsequent AuthenticationProvider s will be tried. If authentication was not successful by any supporting AuthenticationProvider the last thrown AuthenticationException will be rethrown

大致意思是：
> 如果有多个 `AuthenticationProvider` 都支持同一个`Authentication` 对象，那么**第一个 能够成功验证**`**Authentication**`**的 Provder** 将填充其属性并返回结果，从而覆盖早期支持的 `AuthenticationProvider`抛出的任何可能的 `AuthenticationException`。一旦成功验证后，将不会尝试后续的 `AuthenticationProvider`。如果所有的 `AuthenticationProvider`都没有成功验证 `Authentication`，那么将抛出最后一个Provider抛出的`AuthenticationException`。(`AuthenticationProvider`可以在Spring Security配置类中配置)

**PS**：<br />当然有时候有多个不同的 `AuthenticationProvider`，它们分别支持不同的 Authentication对象，那么当一个具体的 `AuthenticationProvier`传进入 `ProviderManager`的内部时，就会在 `AuthenticationProvider`列表中挑选其对应支持的provider对相应的 Authentication对象进行验证。<br />不同的登录方式认证逻辑是不一样的，即 `AuthenticationProvider`会不一样，如果使用用户名和密码登录，那么在Spring Security 提供了一个 `AuthenticationProvider`的简单实现 `DaoAuthenticationProvider`，这也是框架最早的 provider，它使用了一个 `UserDetailsService`来查询用户名、密码和 `GrantedAuthority`，一般要实现`UserDetailsService`接口，并在Spring Security配置类中将其配置进去，这样也促使使用`DaoAuthenticationProvider`进行认证，然后该接口返回一个`UserDetails`，它包含了更加详细的身份信息，比如从数据库拿取的密码和权限列表，`AuthenticationProvider` 的认证核心就是加载对应的 `UserDetails`来检查用户输入的密码是否与其匹配，即UserDetails和Authentication两者的密码（关于 `UserDetailsService`和`UserDetails`的介绍在下面小节介绍。）。**而如果是使用第三方登录，比如QQ登录，那么就需要设置对应的 **`**AuthenticationProvider**`，这里就不细说了。
<a name="bi6YL"></a>
#### 认证成功后清除验证信息
在上面`ProviderManager`的源码中还发现一点，在认证成功后清除验证信息，如下：
```java
if (eraseCredentialsAfterAuthentication
    && (result instanceof CredentialsContainer)) {
  // Authentication is complete. Remove credentials and other secret data
  // from authentication
  //成功认证后删除验证信息
  ((CredentialsContainer) result).eraseCredentials();
}
```
从 Spring Security 3.1之后，在请求认证成功后 `ProviderManager`将会删除 `Authentication`中的认证信息，准确的说，一般删除的是 密码信息，这可以保证密码的安全。跟了一下源码，实际上执行删除操作的步骤如下：
```java
public class UsernamePasswordAuthenticationToken extends AbstractAuthenticationToken {
	public void eraseCredentials() {
		super.eraseCredentials();
		//使密码为null
		this.credentials = null;
	}
}
public abstract class AbstractAuthenticationToken implements Authentication, CredentialsContainer {
	...
		public void eraseCredentials() {
		//擦除密码
		this.eraseSecret(this.getCredentials());
		this.eraseSecret(this.getPrincipal());
		this.eraseSecret(this.details);
	}

	private void eraseSecret(Object secret) {
		if (secret instanceof CredentialsContainer) {
			((CredentialsContainer)secret).eraseCredentials();
		}
	}
}
```
从源码就可以看出实际上就是擦除密码操作。
<a name="DdetP"></a>
### UserDetailsService 和 UserDetails
**UserDetailsService简单说就是加载对应的UserDetails的接口(一般从数据库)，而UserDetails包含了更详细的用户信息**，定义如下：
```java
public interface UserDetails extends Serializable {

	Collection<? extends GrantedAuthority> getAuthorities();

	String getPassword();

	String getUsername();

	boolean isAccountNonExpired();

	boolean isAccountNonLocked();

	boolean isCredentialsNonExpired();

	boolean isEnabled();
}
```
`UserDetails` 接口与 Authentication接口相似，它们都有 username、`authorities`。它们的区别如下：

- `Authentication` 的 `getCredentials()` 与 `UserDetails` 中的 `getPassword()` 不一样，前者是用户提交的密码凭证，后者是用户正确的密码，(一般是从数据库中载入的密码)，`AuthenticationProvider`就会对两者进行对比。
- `Authentication` 中的 `getAuthorities()` 实际上是由 `UserDetails` 的 `getAuthorities()`传递形成的。
- `Authentication` 中的 `getUserDetails()` 中的 `UserDetails` 用户详细信息是经过 `AuthenticationProvider`认证之后填充的。
<a name="cuOnP"></a>
## 认证过程样本示例
下面来看一个官方文档提供的例子，代码如下：
```java
public class SpringSecuriryTestDemo {
    private static AuthenticationManager am = new SampleAuthenticationManager();

    public static void main(String[] args) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        while (true) {
            System.out.println("Please enter your username:");
            String name = in.readLine();
            System.out.println("Please enter your password:");
            String password = in.readLine();
            try {
                Authentication request = new UsernamePasswordAuthenticationToken(name, password);
                Authentication result = am.authenticate(request);
                SecurityContextHolder.getContext().setAuthentication(request);
                break;
            } catch (AuthenticationException e) {
                System.out.println("Authentication failed: " + e.getMessage());
            }
        }
        System.out.println("Successfully authenticated. Security context contains: " + SecurityContextHolder.getContext().getAuthentication());
    }
    static class SampleAuthenticationManager implements AuthenticationManager {
        static final List<GrantedAuthority> AUTHORITIES = new ArrayList<GrantedAuthority>();
        static {
            AUTHORITIES.add(new SimpleGrantedAuthority("ROLE_USER"));
        }
        @Override
        public Authentication authenticate(Authentication authentication) throws AuthenticationException {
            if (authentication.getName().equals(authentication.getCredentials())) {
                return new UsernamePasswordAuthenticationToken(authentication.getName(), authentication.getCredentials(), AUTHORITIES);
            }
            throw new BadCredentialsException("Bad Credentials");
        }
    }
}
```
测试如下：
```java
Please enter your username:
pjmike
Please enter your password:
123
Authentication failed: Bad Credentials
Please enter your username:
pjmike
Please enter your password:
pjmike
Successfully authenticated. 
Security context contains: org.springframework.security.authentication.UsernamePasswordAuthenticationToken@441d0230:
Principal: pjmike; 
Credentials: [PROTECTED];
Authenticated: true; Details: null; 
Granted Authorities: ROLE_USER
```
上面的例子很简单，不是源码，只是为了演示认证过程编写的Demo，而且也缺少过滤器链，但是麻雀虽小，五脏俱全，基本包括了Spring Security的核心组件，表达了Spring Security 认证的基本思想。解读一下：

- 用户名和密码被封装到 `UsernamePasswordAuthentication`的实例中(该类是 `Authentication`接口的实现)
- 该 `Authentication`传递给 `AuthenticationManager`进行身份验证
- 认证成功后，`AuthenticationManager`会返回一个完全填充的 `Authentication`实例，该实例包含权限信息，身份信息，细节信息，但是密码通常会被移除
- 通过调用 `SecurityContextHolder.getContext().setAuthentication(…)`传入上面返回的填充了信息的 `Authentication`对象

通过上面一个简单示例，大致明白了Spring Security的基本思想，但是要真正理清楚Spring Security的认证流程这还不够，需要深入源码去探究。
<a name="VyGE0"></a>
## Spring Security 的认证过程及相关过滤器
**Spring Security 的核心之一就是它的过滤器链，就从它的过滤器链入手，下图是Spring Security 过滤器链的一个执行过程，本文将依照该过程来逐步的剖析其认证过程**。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657374347557-ea8acc9e-29ac-4ab1-bdf8-69c3b392fb47.png#clientId=u8749cf32-0573-4&from=paste&id=uf3e349d8&originHeight=363&originWidth=907&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0ce03e58-e406-4385-a93a-02c25376e4e&title=)
<a name="RQotj"></a>
## 核心过滤器链简介
Spring Security 中的过滤器有很多，一般正常的项目中都有十几个过滤器，有时候还包含自定义的过滤器，当然不可能对每一个过滤器都进行分析，需要抓住重点，找比较关键的几个过滤器，它们在认证过程中扮演着重要角色，下面列举几个核心的过滤器：

- `**SecurityContextPersistenceFilter**`：整个Spring Security 过滤器链的开端，它有两个作用：一是当请求到来时，检查Session中是否存在`SecurityContext`，如果不存在，就创建一个新的`SecurityContext`。二是请求结束时将`SecurityContext`放入 `Session`中，并清空 `SecurityContextHolder`。
- `**UsernamePasswordAuthenticationFilter**`：继承自抽象类 `AbstractAuthenticationProcessingFilter`，当进行表单登录时，该`Filter`将用户名和密码封装成一个 `UsernamePasswordAuthentication`进行验证。
- `**AnonymousAuthenticationFilter**`：匿名身份过滤器，当前面的Filter认证后依然没有用户信息时，该`Filter`会生成一个匿名身份——`AnonymousAuthenticationToken`。一般的作用是用于匿名登录。
- `**ExceptionTranslationFilter**`：异常转换过滤器，用于处理 `FilterSecurityInterceptor`抛出的异常。
- `**FilterSecurityInterceptor**`：过滤器链最后的关卡，从 `SecurityContextHolder`中获取 `Authentication`，比对用户拥有的权限和所访问资源需要的权限。
<a name="PaE2W"></a>
## 表单登录认证过程
当访问一个受保护的资源时，如果之前没有进行登录认证，那么系统将返回一个登录表单或者一个响应结果提示要先进行登录操作。这里的分析过程只针对表单登录，所以先在表单中填写用户名和密码进行登录验证。<br />上面已经简述了一堆核心过滤器，这里先从 `SecurityContextPersistenceFilter`这个过滤器的开端开始分析整个表单登录的认证过程。
<a name="UcxZT"></a>
### `SecurityContextPersistenceFilter`
当填写表单完毕后，点击登录按钮，请求先经过 `SecurityContextPersistenceFilter`过滤器，在前面就曾提到，该`Filter`有两个作用，其中之一就是在请求到来时，创建 `SecurityContext`安全上下文，来看看它内部是如何做的，部分源码如下：
```java
public class SecurityContextPersistenceFilter extends GenericFilterBean {

	static final String FILTER_APPLIED = "__spring_security_scpf_applied";
	//安全上下文存储的仓库
	private SecurityContextRepository repo;

	private boolean forceEagerSessionCreation = false;

	public SecurityContextPersistenceFilter() {
		//使用HttpSession来存储 SecurityContext
		this(new HttpSessionSecurityContextRepository());
	}

	public SecurityContextPersistenceFilter(SecurityContextRepository repo) {
		this.repo = repo;
	}

	public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain)
		throws IOException, ServletException {
		HttpServletRequest request = (HttpServletRequest) req;
		HttpServletResponse response = (HttpServletResponse) res;
		// 如果是第一次请求，request中肯定没有 FILTER_APPLIED属性
		if (request.getAttribute(FILTER_APPLIED) != null) {
			// 确保每个请求只应用一次过滤器
			chain.doFilter(request, response);
			return;
		}

		final boolean debug = logger.isDebugEnabled();
		// 在request 设置 FILTER_APPLIED 属性为 true，这样同一个请求再次访问时，就直接进入后续Filter的操作
		request.setAttribute(FILTER_APPLIED, Boolean.TRUE);

		if (forceEagerSessionCreation) {
			HttpSession session = request.getSession();

			if (debug && session.isNew()) {
				logger.debug("Eagerly created session: " + session.getId());
			}
		}
		// 封装 requset 和 response 
		HttpRequestResponseHolder holder = new HttpRequestResponseHolder(request,
																		 response);
		// 从存储安全上下文的仓库中载入 SecurityContext 安全上下文，其内部是从 Session中获取上下文信息
		SecurityContext contextBeforeChainExecution = repo.loadContext(holder);

		try {
			//安全上下文信息设置到 SecurityContextHolder 中，以便在同一个线程中，后续访问 SecurityContextHolder 能获取到 SecuritContext
			SecurityContextHolder.setContext(contextBeforeChainExecution);
			//进入下一个过滤器操作
			chain.doFilter(holder.getRequest(), holder.getResponse());
		}
		finally {
			// 请求结束后，清空安全上下文信息
			SecurityContext contextAfterChainExecution = SecurityContextHolder
				.getContext();
			// Crucial removal of SecurityContextHolder contents - do this before anything
			// else.
			SecurityContextHolder.clearContext();
			//将安全上下文信息存储到 Session中，相当于登录态的维护
			repo.saveContext(contextAfterChainExecution, holder.getRequest(),
							 holder.getResponse());
			request.removeAttribute(FILTER_APPLIED);

			if (debug) {
				logger.debug("SecurityContextHolder now cleared, as request processing completed");
			}
		}
	}

	public void setForceEagerSessionCreation(boolean forceEagerSessionCreation) {
		this.forceEagerSessionCreation = forceEagerSessionCreation;
	}
}
```
请求到来时，利用`HttpSessionSecurityContextRepository`读取安全上下文。这里是第一次请求，读取的安全上下文中是没有 `Authentication`身份信息的，将安全上下文设置到 `SecurityContextHolder`之后，进入下一个过滤器。<br />请求结束时，同样利用`HttpSessionSecurityContextRepository`该存储安全上下文的仓库将认证后的`SecurityContext`放入 `Session`中，这也是**登录态维护**的关键，具体的操作这里就不细说了。
<a name="Bw23p"></a>
### UsernamePasswordAuthenticationFilter
经过 `SecurityContextPersistenceFilter`过滤器后来到 `UsernamePasswordAuthenticationFilter`过滤器，因为假定的是第一次请求，所以 `SecurityContext`并没有包含认证过的 `Authentication`。**从此过滤器开始的操作对于表单登录来说是非常关键的，包含了表单登录的核心认证步骤**，下面画了一张在此过滤器中的认证过程图：<br />![filter-process](https://cdn.nlark.com/yuque/0/2022/png/396745/1657374347547-f9930cd7-c9ae-4e88-92a2-ec2474a5d4d2.png#clientId=u8749cf32-0573-4&from=paste&id=ub1d1f57f&originHeight=488&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5089c810-d319-4dbc-a9c7-b34ebe96b77&title=filter-process "filter-process")<br />`UsernamePasswordAuthenticationFilter`的父类是 `AbstractAuthenticationProcessingFilter`，首先进入父类的 `foFilter`方法，部分源码如下：
```java
public abstract class AbstractAuthenticationProcessingFilter extends GenericFilterBean
    implements ApplicationEventPublisherAware, MessageSourceAware {
  ...
    public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain)
      throws IOException, ServletException {

    HttpServletRequest request = (HttpServletRequest) req;
    HttpServletResponse response = (HttpServletResponse) res;
        ...
    Authentication authResult;

    try {
        //调用子类 UsernamePasswordAuthenticationFilter 的 attemptAuthentication 方法
      authResult = attemptAuthentication(request, response);
      if (authResult == null) {
        // return immediately as subclass has indicated that it hasn't completed
        // authentication
        //子类未完成认证，立刻返回
        return;
      }
      sessionStrategy.onAuthentication(authResult, request, response);
    }
    catch (InternalAuthenticationServiceException failed) {
      logger.error(
          "An internal error occurred while trying to authenticate the user.",
          failed);
      unsuccessfulAuthentication(request, response, failed);

      return;
    }
    catch (AuthenticationException failed) {
      //认证失败
      unsuccessfulAuthentication(request, response, failed);

      return;
    }

    // 认证成功
    if (continueChainBeforeSuccessfulAuthentication) {
        //继续调用下一个 Filter
      chain.doFilter(request, response);
    }
        //将成功认证后的Authentication写入 SecurityContext中
    successfulAuthentication(request, response, chain, authResult);
  }    
}
```
该`doFilter`方法中一个核心就是调用子类 `UsernamePasswordAuthenticationFilter`的`attemptAuthentication`方法，该方法进入真正的认证过程，并返回认证后的 `Authentication`，该方法的源码如下：
```java
public Authentication attemptAuthentication(HttpServletRequest request,
											HttpServletResponse response) throws AuthenticationException {
	//必须是POST请求
	if (postOnly && !request.getMethod().equals("POST")) {
		throw new AuthenticationServiceException(
			"Authentication method not supported: " + request.getMethod());
	}
	//获取表单中的用户名和密码
	String username = obtainUsername(request);
	String password = obtainPassword(request);

	if (username == null) {
		username = "";
	}

	if (password == null) {
		password = "";
	}

	username = username.trim();
	//将用户名和密码封装成一个 UsernamePasswordAuthenticationToken
	UsernamePasswordAuthenticationToken authRequest = new UsernamePasswordAuthenticationToken(
		username, password);

	// Allow subclasses to set the "details" property
	setDetails(request, authRequest);
	//核心部分，交给内部的AuthenticationManager去认证，并返回认证后的 Authentication
	return this.getAuthenticationManager().authenticate(authRequest);
}
```
该方法中有一个关键点就是 `his.getAuthenticationManager().authenticate(authRequest)`，调用内部的 `AuthenticationManager`去认证，在之前就介绍过`**AuthenticationManager**`**，它是身份认证的核心接口，它的实现类是 **`**ProviderManager**`**，而 **`**ProviderManager**`**又将请求委托给一个 **`**AuthenticationProvider**`**列表，列表中的每一个 **`**AuthenticationProvider**`**将会被依次查询是否需要通过其进行验证，每个 **`**provider**`**的验证结果只有两个情况：抛出一个异常或者完全填充一个 Authentication对象的所有属性**<br />下面来分析一个关键的 `AuthenticationProvider`，它就是 `DaoAuthenticationProvider`，它是框架最早的`provider`，也是最最常用的 `provider`。大多数情况下会依靠它来进行身份认证，它的父类是 `AbstractUserDetailsAuthenticationProvider`，认证过程首先会调用父类的 `authenticate`方法，核心源码如下：
```java
public Authentication authenticate(Authentication authentication)
	throws AuthenticationException {
	Assert.isInstanceOf(UsernamePasswordAuthenticationToken.class, authentication,
						messages.getMessage(
							"AbstractUserDetailsAuthenticationProvider.onlySupports",
							"Only UsernamePasswordAuthenticationToken is supported"));

	// Determine username
	String username = (authentication.getPrincipal() == null) ? "NONE_PROVIDED"
		: authentication.getName();

	boolean cacheWasUsed = true;
	UserDetails user = this.userCache.getUserFromCache(username);

	if (user == null) {
		cacheWasUsed = false;

		try {
			//1 调用子类  DaoAuthenticationProvider 的 retrieveUser()方法获取 UserDetails
			user = retrieveUser(username,
								(UsernamePasswordAuthenticationToken) authentication);
		}
		//没拿到UserDetails会抛出异常信息
		catch (UsernameNotFoundException notFound) {
			logger.debug("User '" + username + "' not found");

			if (hideUserNotFoundExceptions) {
				throw new BadCredentialsException(messages.getMessage(
					"AbstractUserDetailsAuthenticationProvider.badCredentials",
					"Bad credentials"));
			}
			else {
				throw notFound;
			}
		}

		Assert.notNull(user,
					   "retrieveUser returned null - a violation of the interface contract");
	}

	try {
		//2 对UserDetails的一些属性进行预检查，即判断用户是否锁定，是否可用以及用户是否过期
		preAuthenticationChecks.check(user);
		//3 对UserDetails附加的检查，对传入的Authentication与从数据库中获取的UserDetails进行密码匹配
		additionalAuthenticationChecks(user,
									   (UsernamePasswordAuthenticationToken) authentication);
	}
	catch (AuthenticationException exception) {
		if (cacheWasUsed) {
			// There was a problem, so try again after checking
			// we're using latest data (i.e. not from the cache)
			cacheWasUsed = false;
			user = retrieveUser(username,
								(UsernamePasswordAuthenticationToken) authentication);
			preAuthenticationChecks.check(user);
			additionalAuthenticationChecks(user,
										   (UsernamePasswordAuthenticationToken) authentication);
		}
		else {
			throw exception;
		}
	}
	//4 对UserDetails进行后检查，检查UserDetails的密码是否过期
	postAuthenticationChecks.check(user);

	if (!cacheWasUsed) {
		this.userCache.putUserInCache(user);
	}

	Object principalToReturn = user;

	if (forcePrincipalAsString) {
		principalToReturn = user.getUsername();
	}
	//5 上面所有检查成功后，用传入的用户信息和获取的UserDetails生成一个成功验证的Authentication
	return createSuccessAuthentication(principalToReturn, authentication, user);
}
```
从上面一大串源码中，提取几个关键的方法：

- `**retrieveUser(…)**`：调用子类 `DaoAuthenticationProvider` 的 `retrieveUser()`方法获取 UserDetails
- `**preAuthenticationChecks.check(user)**`：对从上面获取的UserDetails进行预检查，即判断用户是否锁定，是否可用以及用户是否过期
- `**additionalAuthenticationChecks(user,authentication)**`：对UserDetails附加的检查，对传入的`Authentication`与获取的UserDetails进行密码匹配
- `**postAuthenticationChecks.check(user)**`：对UserDetails进行后检查，即检查UserDetails的密码是否过期
- `**createSuccessAuthentication(principalToReturn, authentication, user)**`：上面所有检查成功后，利用传入的`Authentication`和获取的UserDetails生成一个成功验证的`Authentication`

`**retrieveUser(…)**`**方法**<br />接下来详细说说 `retrieveUser(...)`方法， `DaoAuthenticationProvider`的 `retrieveUser()` 源码如下：
```java

protected final UserDetails retrieveUser(String username,
										 UsernamePasswordAuthenticationToken authentication)
	throws AuthenticationException {
	prepareTimingAttackProtection();
	try {
		//经过UserDetailsService 获取 UserDetails
		UserDetails loadedUser = this.getUserDetailsService().loadUserByUsername(username);
		if (loadedUser == null) {
			throw new InternalAuthenticationServiceException(
				"UserDetailsService returned null, which is an interface contract violation");
		}
		return loadedUser;
	}
	catch (UsernameNotFoundException ex) {
		mitigateAgainstTimingAttack(authentication);
		throw ex;
	}
	catch (InternalAuthenticationServiceException ex) {
		throw ex;
	}
	catch (Exception ex) {
		throw new InternalAuthenticationServiceException(ex.getMessage(), ex);
	}
}
```
该方法最核心的部分就是调用内部的UserDetailsServices 加载 UserDetails，`UserDetailsServices`本质上就是加载UserDetails的接口，UserDetails包含了比`Authentication`更加详细的用户信息。**UserDetailsService常见的实现类有JdbcDaoImpl，**`**InMemoryUserDetailsManager**`**，前者从数据库加载用户，后者从内存中加载用户。也可以自己实现**`**UserDetailsServices**`**接口，比如是如果是基于数据库进行身份认证，那么可以手动实现该接口，而不用JdbcDaoImpl。**<br />`**additionalAuthenticationChecks()**`<br />UserDetails的预检查和后检查比较简单，这里就不细说了，下面来看一下密码匹配校验，代码如下：
```java
protected void additionalAuthenticationChecks(UserDetails userDetails,
											  UsernamePasswordAuthenticationToken authentication)
	throws AuthenticationException {
	if (authentication.getCredentials() == null) {
		logger.debug("Authentication failed: no credentials provided");

		throw new BadCredentialsException(messages.getMessage(
			"AbstractUserDetailsAuthenticationProvider.badCredentials",
			"Bad credentials"));
	}

	String presentedPassword = authentication.getCredentials().toString();
	//利用 PasswordEncoder编码器校验密码
	if (!passwordEncoder.matches(presentedPassword, userDetails.getPassword())) {
		logger.debug("Authentication failed: password does not match stored value");

		throw new BadCredentialsException(messages.getMessage(
			"AbstractUserDetailsAuthenticationProvider.badCredentials",
			"Bad credentials"));
	}
}
```
这个方法实际上是调用`DaoAuthenticationProvider`的`additionalAuthenticationChecks`方法，内部调用加密解密器进行密码匹配，如果匹配失败，则抛出一个 `BadCredentialsException`异常<br />最后通过`createSuccessAuthentication(..)`方法生成一个成功认证的 `Authentication`，简单说就是组合获取的`UserDetails`和传入的`Authentication`，得到一个完全填充的`Authentication`。<br />该`Authentication`最终一步一步向上返回，到`AbstractAuthenticationProcessingFilter`过滤器中，将其设置到 `SecurityContextHolder`。
<a name="bIsx3"></a>
### `AnonymousAuthenticationFilter`
匿名认证过滤器，它主要是针对匿名登录，如果前面的`Filter`，比如`UsernamePasswordAuthenticationFilter`执行完毕后，`SecurityContext`依旧没有用户信息，那么`AnonymousAuthenticationFilter`才会起作用，生成一个匿名身份信息——`AnonymousAuthenticationToken`
<a name="WhCkL"></a>
### `ExceptionTranslationFilter`
`ExceptionTranslationFilter` 简单的说就是处理 `FilterSecurityInterceptor` 抛出的异常，其内部 `doFilter`方法源码如下：
```java
public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain)
	throws IOException, ServletException {
	HttpServletRequest request = (HttpServletRequest) req;
	HttpServletResponse response = (HttpServletResponse) res;

	try {
		//直接进入下一个Filter
		chain.doFilter(request, response);

		logger.debug("Chain processed normally");
	}
	catch (IOException ex) {
		throw ex;
	}
	//真正的作用在这里，处理抛出的异常
	catch (Exception ex) {
		// Try to extract a SpringSecurityException from the stacktrace
		Throwable[] causeChain = throwableAnalyzer.determineCauseChain(ex);
		RuntimeException ase = (AuthenticationException) throwableAnalyzer
			.getFirstThrowableOfType(AuthenticationException.class, causeChain);
		//这里会处理 FilterSecurityInterceptor 抛出的AccessDeniedException
		if (ase == null) {
			ase = (AccessDeniedException) throwableAnalyzer.getFirstThrowableOfType(
				AccessDeniedException.class, causeChain);
		}

		if (ase != null) {
			if (response.isCommitted()) {
				throw new ServletException("Unable to handle the Spring Security Exception because the response is already committed.", ex);
			}
			handleSpringSecurityException(request, response, chain, ase);
		}
		else {
			// Rethrow ServletExceptions and RuntimeExceptions as-is
			if (ex instanceof ServletException) {
				throw (ServletException) ex;
			}
			else if (ex instanceof RuntimeException) {
				throw (RuntimeException) ex;
			}

			// Wrap other Exceptions. This shouldn't actually happen
			// as we've already covered all the possibilities for doFilter
			throw new RuntimeException(ex);
		}
	}
}
```
<a name="mllal"></a>
### `FilterSecurityInterceptor`
`FilterSecurityInterceptor` 过滤器是最后的关卡，之前的请求最终会来到这里，它的大致工作流程就是

- 封装请求信息
- 从系统中读取配置信息，即资源所需的权限信息
- 从 `SecurityContextHolder`中获取之前认证过的 `Authentication`对象，即表示当前用户所拥有的权限
- 然后根据上面获取到的三种信息，传入一个权限校验器中，对于当前请求来说，比对用户拥有的权限和资源所需的权限。若比对成功，则进入真正系统的请求处理逻辑，反之，会抛出相应的异常

下面画一张简易的流程图来阐述 `FilterSecurityInterceptor`的执行过程，如下：<br />![filter_processs](https://cdn.nlark.com/yuque/0/2022/png/396745/1657374347546-2b821c3c-e511-487d-8ab7-aeccac644c44.png#clientId=u8749cf32-0573-4&from=paste&id=HsLKc&originHeight=486&originWidth=647&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u7ed354ec-962f-42b5-9e40-930096c3337&title=filter_processs "filter_processs")<br />根据上图内容，再来看看 `FilterSecurityInterceptor`的源码：
```java
public class FilterSecurityInterceptor extends AbstractSecurityInterceptor implements
	Filter {
	...
		public void doFilter(ServletRequest request, ServletResponse response,
							 FilterChain chain) throws IOException, ServletException {
		// 封装request、response请求
		FilterInvocation fi = new FilterInvocation(request, response, chain);
		//调用核心方法
		invoke(fi);
	}  
	...
		public void invoke(FilterInvocation fi) throws IOException, ServletException {
		if ((fi.getRequest() != null)
			&& (fi.getRequest().getAttribute(FILTER_APPLIED) != null)
			&& observeOncePerRequest) {
			// filter already applied to this request and user wants us to observe
			// once-per-request handling, so don't re-do security checking
			fi.getChain().doFilter(fi.getRequest(), fi.getResponse());
		}
		else {
			// 判断当前请求之前是否经历过该过滤器
			if (fi.getRequest() != null && observeOncePerRequest) {
				//  如果当前请求已经经历过这个安全过滤器判断，那么不再执行后续逻辑，直接往下走，调用请求的处理方法
				fi.getRequest().setAttribute(FILTER_APPLIED, Boolean.TRUE);
			}
			//调用父类的方法，执行授权判断逻辑
			InterceptorStatusToken token = super.beforeInvocation(fi);

			try {
				fi.getChain().doFilter(fi.getRequest(), fi.getResponse());
			}
			finally {
				super.finallyInvocation(token);
			}

			super.afterInvocation(token, null);
		}
	}

}
```
源码中已经对请求进行了封装，然后进入核心部分， 调用父类的授权判断方法——`beforeInvocation(FilterInvocation)`，源码如下：
```java
protected InterceptorStatusToken beforeInvocation(Object object) {
	Assert.notNull(object, "Object was null");
	final boolean debug = logger.isDebugEnabled();

	if (!getSecureObjectClass().isAssignableFrom(object.getClass())) {
		throw new IllegalArgumentException(
			"Security invocation attempted for object "
			+ object.getClass().getName()
			+ " but AbstractSecurityInterceptor only configured to support secure objects of type: "
			+ getSecureObjectClass());
	}
	//读取Spring Security的配置信息，将其封装成 ConfigAttribute
	Collection<ConfigAttribute> attributes = this.obtainSecurityMetadataSource()
		.getAttributes(object);
	if (attributes == null || attributes.isEmpty()) {
		if (rejectPublicInvocations) {
			throw new IllegalArgumentException(
				"Secure object invocation "
				+ object
				+ " was denied as public invocations are not allowed via this interceptor. "
				+ "This indicates a configuration error because the "
				+ "rejectPublicInvocations property is set to 'true'");
		}
		...
			return null; // no further work post-invocation
	}
	...
		if (SecurityContextHolder.getContext().getAuthentication() == null) {
			credentialsNotFound(messages.getMessage(
				"AbstractSecurityInterceptor.authenticationNotFound",
				"An Authentication object was not found in the SecurityContext"),
								object, attributes);
		}
	//从SecurityContextHolder中获取Authentication
	Authentication authenticated = authenticateIfRequired();

	// 启动授权匹配
	try {
		this.accessDecisionManager.decide(authenticated, object, attributes);
	}
	catch (AccessDeniedException accessDeniedException) {
		publishEvent(new AuthorizationFailureEvent(object, attributes, authenticated,
												   accessDeniedException));

		throw accessDeniedException;
	}
	...

}
```
`beforeInvocation`的源码比较多，这里只保留了相对核心的部分，从源码就可以看出，拿到配置信息和用户信息后，连同请求信息一同传入`AccessDecisionManager`的 `decide(Authentication authentication, Object object,Collection<ConfigAttribute> configAttributes)`方法。该方法是最终执行授权校验逻辑的地方。<br />`AccessDecisionManager` 本身是一个接口，它的 实现类是 `AbstractAccessDecisionManager`，而 `AbstractAccessDecisionManager`也是一个抽象类，它的实现类有三个，常用的是 `AffirmativeBased`，最终的授权校验逻辑是 `AffirmativeBased` 实现的，部分源码如下：
```java
public void decide(Authentication authentication, Object object,
				   Collection<ConfigAttribute> configAttributes) throws AccessDeniedException {
	int deny = 0;
	//投票器执行投票
	for (AccessDecisionVoter voter : getDecisionVoters()) {
		int result = voter.vote(authentication, object, configAttributes);
		...
			switch (result) {
				case AccessDecisionVoter.ACCESS_GRANTED:
					return;
					
				case AccessDecisionVoter.ACCESS_DENIED:
					deny++;
					
					break;
					
				default:
					break;
			}
	}
	
	if (deny > 0) {
		throw new AccessDeniedException(messages.getMessage(
			"AbstractAccessDecisionManager.accessDenied", "Access is denied"));
	}
	...
}
```
该方法的逻辑比较简单，就是执行`AccessDecisionVoter`的校验逻辑，如果校验失败就抛出`AccessDeniedException`异常。对于`AccessDecisionVoter`的`vote`投票逻辑这里就不细说了，在 Spring Security 3.0以后，一般默认使用 `AccessDecisionVoter`接口的实现类`**WebExpressionVoter**`来完成最终的校验过程。
