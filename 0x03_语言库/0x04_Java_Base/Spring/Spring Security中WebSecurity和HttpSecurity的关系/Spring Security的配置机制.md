Java Spring Spring Security
<a name="zOtuZ"></a>
## Spring Security即将弃用配置类`WebSecurityConfigurerAdapter`
用过`WebSecurityConfigurerAdapter`的都知道对**Spring Security**十分重要，总管**Spring Security**的配置体系。但是马上这个类要废了，这个类将在5.7版本被`@Deprecated`所标记了，未来这个类将被移除。<br />既然马上要弃用了，总要有个过渡方案或者新玩法吧。这里把整套的替代方案再搞一遍。<br />版本需要**Spring Security 5.4.x**及以上。
<a name="EsUh0"></a>
### `HttpSecurity`新旧玩法对比
旧玩法：
```java
@Configuration
static class SecurityConfig extends WebSecurityConfigurerAdapter {
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
            .antMatcher("/**")
            .authorizeRequests(authorize -> authorize
                    .anyRequest().authenticated()
            );
    }
}
```
新玩法：
```java
@Bean
SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
    return http
            .antMatcher("/**")
            .authorizeRequests(authorize -> authorize
                    .anyRequest().authenticated()
            )
            .build();
}
```
<a name="VYfES"></a>
### `WebSecurity`新旧玩法对比
使用`WebSecurity.ignoring()`忽略某些URL请求，这些请求将被**Spring Security**忽略，这意味着这些URL将有受到 CSRF、XSS、Clickjacking 等攻击的可能。以下示例仅仅作为演示，请勿使用在生产环境。是不是又学到了呢？<br />旧玩法：
```java
@Configuration
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {

    @Override
    public void configure(WebSecurity web) {
        // 仅仅作为演示
        web.ignoring().antMatchers("/ignore1", "/ignore2");
    }

}
```
新玩法：
```java
@Configuration
public class SecurityConfiguration {

    @Bean
    public WebSecurityCustomizer webSecurityCustomizer() {
        // 仅仅作为演示
        return (web) -> web.ignoring().antMatchers("/ignore1", "/ignore2");
    }

}
```
如果需要忽略URL，请考虑通过`HttpSecurity.authorizeHttpRequests`的`permitAll`来实现。
<a name="uXfHJ"></a>
### `AuthenticationManager`新旧玩法对比
`AuthenticationManager`配置主要分为全局的（Global ）、本地的（Local）。
<a name="hr65g"></a>
#### 旧玩法
```java
@Configuration
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {

    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {
        auth.jdbcAuthentication();
    }
}
```
上面是通过`WebSecurityConfigurerAdapter`开启的是本地配置。开启全局配置需要覆写其`authenticationManagerBean()`方法并标记为Bean:
```java
@Bean(name name="myAuthenticationManager")
@Override
public AuthenticationManager authenticationManagerBean() throws Exception {
	return super.authenticationManagerBean();
}
```
<a name="qgOsg"></a>
#### 新玩法
本地配置通过`HttpSecurity.authenticationManager`实现：
```java
@Configuration
public class SecurityConfiguration {

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            .authorizeHttpRequests((authz) -> authz
                .anyRequest().authenticated()
            )
            .httpBasic(withDefaults())
            .authenticationManager(new CustomAuthenticationManager());
    }

}
```
全局配置摆脱了依赖`WebSecurityConfigurerAdapter.authenticationManagerBean()`方法，只需要定义一个`AuthenticationManager`类型的Bean即可：
```java
@Bean
AuthenticationManager ldapAuthenticationManager(
		BaseLdapPathContextSource contextSource) {
	LdapBindAuthenticationManagerFactory factory = 
		new LdapBindAuthenticationManagerFactory(contextSource);
	factory.setUserDnPatterns("uid={0},ou=people");
	factory.setUserDetailsContextMapper(new PersonContextMapper());
	return factory.createAuthenticationManager();
}
```
当然还可以通过自定义`GlobalAuthenticationConfigurerAdapter`并注入**Spring IoC**来修改`AuthenticationManagerBuilder`，不限制数量，但是要注意有排序问题。相关的思维导图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650241051811-68c8e1c0-e734-47dd-b651-9c01e569b4be.png#clientId=u5839b354-6fe0-4&from=paste&id=u0b0f6c23&originHeight=264&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u82ec268c-6b41-409b-a365-d085e899cba&title=)
<a name="KKcC3"></a>
## Spring Security 5.4版本带来的新玩法
在以往Spring Security的教程中自定义配置都是声明一个配置类`WebSecurityConfigurerAdapter`，然后覆写（`@Override`）对应的几个方法就行了。然而这一切在**Spring Security 5.4**开始就得到了改变，从Spring Security 5.4 起不需要继承`WebSecurityConfigurerAdapter`就可以配置`HttpSecurity` 了。相关的说明原文：

- Remove need for WebSecurityConfigurerAdapter #8805
- Configure HTTP Security without extending WebSecurityConfigurerAdapter #8804
<a name="KxLkm"></a>
### 新的配置方式
旧的配置方式**目前依然有效**：
```java
@Configuration
static class SecurityConfig extends WebSecurityConfigurerAdapter {
	@Override
	protected void configure(HttpSecurity http) throws Exception {
		http
			.antMatcher("/**")
			.authorizeRequests(authorize -> authorize
							   .anyRequest().authenticated()
							  );
	}
}
```
**5.4.x**版本有新的选择：
```java
@Bean
SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
	return http
		.antMatcher("/**")
		.authorizeRequests(authorize -> authorize
						   .anyRequest().authenticated()
						  )
		.build();
}
```
这种JavaConfig的方式看起来更加清爽舒服，而且和适配器解耦了。上面`filterChain`方法的参数是`HttpSecurity`类型。熟悉`@Bean`注解的同学应该会意识到一定有一个`HttpSecurity`类型的**Spring Bean**。没错！就在`HttpSecurityConfiguration`中有一个这样的**Bean**：
```java
@Bean({"org.springframework.security.config.annotation.web.configuration.HttpSecurityConfiguration.httpSecurity"})
@Scope("prototype")
HttpSecurity httpSecurity() throws Exception {
    // 省略掉
    return http;
}
```
初始化的内容已经忽略掉，它不是本文关注的重点。可以注意到`HttpSecurity`被`@Scope("prototype")`标记。也就是这个`HttpSecurity` Bean不是单例的，每一次请求都会构造一个新的实例。这个设定非常方便构建多个互相没有太多关联的`SecurityFilterChain`，进而能在一个安全体系中构建相互隔离的安全策略。比如后端管理平台用Session模式，前台应用端用Token模式。![多个SecurityFilterChain](https://cdn.nlark.com/yuque/0/2022/png/396745/1650241366979-29e9d9a1-8f1e-4fbd-9307-633b7a3aa9f5.png#clientId=u5839b354-6fe0-4&from=paste&id=StZkE&originHeight=508&originWidth=741&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0cf77a82-e7f0-427f-acce-a82202a6a73&title=%E5%A4%9A%E4%B8%AASecurityFilterChain "多个SecurityFilterChain")
<a name="UpVGD"></a>
### 原理
Spring Security 有一个名为`springSecurityFilterChain`默认的过滤器链类（实际位置就是上图的 **Bean Filter**位置），其类型为`FilterChainProxy`， 它代理了所有的`SecurityFilterChain`，关键的代理注入代码：
```java
for (SecurityFilterChain securityFilterChain : this.securityFilterChains) {
	this.webSecurity.addSecurityFilterChainBuilder(() -> securityFilterChain);
	for (Filter filter : securityFilterChain.getFilters()) {
		if (filter instanceof FilterSecurityInterceptor) {
			this.webSecurity.securityInterceptor((FilterSecurityInterceptor) filter);
			break;
		}
	}
}
```
那么`this.securityFilterChains`来自哪里呢？
```java
@Autowired(required = false)
void setFilterChains(List<SecurityFilterChain> securityFilterChains) {
   securityFilterChains.sort(AnnotationAwareOrderComparator.INSTANCE);
   this.securityFilterChains = securityFilterChains;
}
```
到这里就一目了然了吧，`SecurityFilterChain`类型的Bean会被加载到`this.securityFilterChains`中。如果Spring Security 版本升级到 5.4.x，就可以尝试一下这种方式。
<a name="ksLJ7"></a>
## Spring Security的配置机制
从**Spring Security 5.4**版本开始会提供一个原型范围的`HttpSecurity`来构建过滤器链`SecurityFilterChain`：
```java
@Bean(HTTPSECURITY_BEAN_NAME)
@Scope("prototype")
HttpSecurity httpSecurity() throws Exception {
	WebSecurityConfigurerAdapter.LazyPasswordEncoder passwordEncoder = new WebSecurityConfigurerAdapter.LazyPasswordEncoder(
		this.context);
	AuthenticationManagerBuilder authenticationBuilder = new WebSecurityConfigurerAdapter.DefaultPasswordEncoderAuthenticationManagerBuilder(
		this.objectPostProcessor, passwordEncoder);
	authenticationBuilder.parentAuthenticationManager(authenticationManager());
	HttpSecurity http = new HttpSecurity(this.objectPostProcessor, authenticationBuilder, createSharedObjects());
	// @formatter:off
	http
		.csrf(withDefaults())
		.addFilter(new WebAsyncManagerIntegrationFilter())
		.exceptionHandling(withDefaults())
		.headers(withDefaults())
		.sessionManagement(withDefaults())
		.securityContext(withDefaults())
		.requestCache(withDefaults())
		.anonymous(withDefaults())
		.servletApi(withDefaults())
		.apply(new DefaultLoginPageConfigurer<>());
	http.logout(withDefaults());
	// @formatter:on
	return http;
}
```
这里会构建基于原型的HttpSecurityBean，并且初始化了一些默认配置供使用。涉及**Spring Security**的日常开发都是围绕这个类进行的，所以这个类是学习**Spring Security**的重中之重。<br />基于原型（`prototype`）的**Spring Bean**的一个典型应用场景，
<a name="s6Mwn"></a>
### 基本配置
日常使用的一些配置项如下：

| 方法 | 说明 |
| --- | --- |
| `requestMatchers()` | 为`SecurityFilterChain`提供URL拦截策略，具体还提供了`antMatcher`和`mvcMathcer` |
| `openidLogin()` | 用于基于 OpenId 的验证 |
| `headers()` | 将安全标头添加到响应，比如说简单的 XSS 保护 |
| `cors()` | 配置跨域资源共享（ CORS ） |
| `sessionManagement()` | 配置会话管理 |
| `portMapper()` | 配置一个PortMapper(HttpSecurity#(getSharedObject(class)))，其他提供SecurityConfigurer的对象使用 PortMapper 从 HTTP 重定向到 HTTPS 或者从 HTTPS 重定向到 HTTP。默认情况下，Spring Security使用一个PortMapperImpl映射 HTTP 端口8080到 HTTPS 端口8443，HTTP 端口80到 HTTPS 端口443 |
| `jee()` | 配置基于容器的预认证。在这种情况下，认证由Servlet容器管理 |
| `x509()` | 配置基于x509的预认证 |
| `rememberMe` | 配置“记住我”的验证 |
| `authorizeRequests()` | 基于使用`HttpServletRequest`限制访问 |
| `requestCache()` | 配置请求缓存 |
| `exceptionHandling()` | 配置错误处理 |
| `securityContext()` | 在`HttpServletRequests`之间的`SecurityContextHolder`上设置`SecurityContext`的管理。当使用`WebSecurityConfigurerAdapter`时，这将自动应用 |
| `servletApi()` | 将`HttpServletRequest`方法与在其上找到的值集成到`SecurityContext`中。当使用`WebSecurityConfigurerAdapter`时，这将自动应用 |
| `csrf()` | 添加 CSRF 支持，使用`WebSecurityConfigurerAdapter`时，默认启用 |
| `logout()` | 添加退出登录支持。当使用`WebSecurityConfigurerAdapter`时，这将自动应用。默认情况是，访问URL“/ logout”，使HTTP Session无效来清除用户，清除已配置的任何`#rememberMe()`身份验证，清除`SecurityContextHolder`，然后重定向到/login?success |
| `anonymous()` | 配置匿名用户的表示方法。当与`WebSecurityConfigurerAdapter`结合使用时，这将自动应用。默认情况下，匿名用户将使用`org.springframework.security.authentication.AnonymousAuthenticationToken`表示，并包含角色 `ROLE_ANONYMOUS` |
| authenticationManager() | 配置`AuthenticationManager` |
| authenticationProvider() | 添加`AuthenticationProvider` |
| formLogin() | 指定支持基于表单的身份验证。如果未指定`FormLoginConfigurer#loginPage(String)`，则将生成默认登录页面 |
| oauth2Login() | 根据外部OAuth 2.0或OpenID Connect 1.0提供程序配置身份验证 |
| oauth2Client() | OAuth2.0 客户端相关的配置 |
| oauth2ResourceServer() | OAuth2.0资源服务器相关的配置 |
| `requiresChannel()` | 配置通道安全。为了使该配置有用，必须提供至少一个到所需信道的映射 |
| `httpBasic()` | 配置 Http Basic 验证 |
| `addFilter()` | 添加一个已经在内置过滤器注册表注册过的过滤器实例或者子类 |
| `addFilterBefore()` | 在指定的Filter类之前添加过滤器 |
| `addFilterAt()` | 在指定的Filter类的位置添加过滤器 |
| `addFilterAfter()` | 在指定的Filter类的之后添加过滤器 |
| `and()` | 连接以上策略的连接器，用来组合安全策略。实际上就是“而且”的意思 |

<a name="bWwKT"></a>
### 高级玩法
建议先把上面的基本玩法有选择的弄明白，然后有精力的话去研究下`HttpSecurity`的高级玩法。
<a name="M0G1c"></a>
#### apply
这个方法用来把其它的一些配置合并到当前的配置中去，形成插件化，支持`SecurityConfigurerAdapter`或者`SecurityConfigurer`的实现。其实内置的一些配置都是以这种形式集成到`HttpSecurity`中去的。例如文章开头的配置中有默认登录页面相关的配置：
```java
httpSecurity.apply(new DefaultLoginPageConfigurer<>());
```
<a name="rDxPJ"></a>
#### `objectPostProcessor`
配置一个自定义`ObjectPostProcessor`。`ObjectPostProcessor`可以改变某些配置内部的机制，这些配置往往不直接对外提供操作接口。
<a name="q44yx"></a>
#### 获取、移除配置类
`getConfigurer`用来获取已经`apply`的配置类；`getConfigurers`用来获取已经`apply`某个类型的所有配置类。
<a name="hf8av"></a>
#### 配置、获取`SharedObject`
`SharedObject`是在配置中进行共享的一些对象，`HttpSecurity`共享了一些非常有用的对象可以供各个配置之间共享，比如`AuthenticationManager`。相关的方法有`setSharedObject`、`getSharedObject`、`getSharedObjects`。
<a name="titXm"></a>
#### 获取`SecurityFilterChain`
`HttpSecurity`也提供了构建目标对象`SecurityFilterChain`的实例的方法。可以通过`build()`来对配置进行初次构建；也可以通过`getObject()`来获取已经构建的实例；甚至可以使用`getOrBuild()`来进行直接获取实例或者构建实例。<br />所以新的配置都是这样的：
```java
@Bean
SecurityFilterChain securityFilterChain (HttpSecurity http) {
	http.cors();
	return http.build();
}
```
记住每一个`HttpSecurity`只能被构建成功一次。
