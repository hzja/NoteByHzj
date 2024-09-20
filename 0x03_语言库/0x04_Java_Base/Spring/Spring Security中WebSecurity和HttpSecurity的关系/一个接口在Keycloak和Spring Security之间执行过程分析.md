Java Spring Security<br />对Keycloak适配Spring Security的执行流程做一个分析，简单了解一下其定制的一些Spring Security过滤器。
<a name="ytm5g"></a>
## /admin/foo的执行流程
在适配了Keycloak和Spring Security的Spring Boot应用中，编写了一个/admin/foo的接口并对这个接口进行了权限配置：
```java
@Override
protected void configure(HttpSecurity http) throws Exception {
    super.configure(http);
    http
        .authorizeRequests()
        .antMatchers("/customers*").hasRole("USER")
        .antMatchers("/admin/**").hasRole("base_user")
        .anyRequest().permitAll();
}
```
这是典型的Spring Security配置，拥有`base_user`角色的用户有权限访问`/admin/**`。这里需要大家明白的是所谓的用户和`base_user`角色目前都由Keycloak平台管理，而应用目前只能控制资源的访问策略。为了探明执行的流程开启了所有的日志打印，当访问/admin/foo时经过了以下过滤器：
```java
Security filter chain: [
  WebAsyncManagerIntegrationFilter
  SecurityContextPersistenceFilter
  HeaderWriterFilter
  CsrfFilter
  KeycloakPreAuthActionsFilter
  KeycloakAuthenticationProcessingFilter
  LogoutFilter
  RequestCacheAwareFilter
  SecurityContextHolderAwareRequestFilter
  KeycloakSecurityContextRequestFilter
  KeycloakAuthenticatedActionsFilter
  AnonymousAuthenticationFilter
  SessionManagementFilter
  ExceptionTranslationFilter
  FilterSecurityInterceptor
]
```
这里除了Spring Security常规的内置过滤器外还加入了Keycloak适配器的几个过滤器，结合执行流程来认识一下它们。
<a name="iSvkI"></a>
### `KeycloakPreAuthActionsFilter`
这个过滤器的作用是暴露一个`Keycloak`适配器对象`PreAuthActionsHandler`给Spring Security。而这个适配器的作用就是拦截处理一个Keycloak的职能请求处理接口，这些内置接口都有特定的后缀:
```java
// 退出端点
public static final String K_LOGOUT = "k_logout";
// 重置什么公钥的？
public static final String K_PUSH_NOT_BEFORE = "k_push_not_before";
// 测试用的
public static final String K_TEST_AVAILABLE = "k_test_available";
// 获取 jwk 相关的
public static final String K_JWKS = "k_jwks";
```
一般不深入底层可以不管这个过滤器。
<a name="Lw2Hi"></a>
### `KeycloakAuthenticationEntryPoint`
`KeycloakAuthenticationEntryPoint`是`AuthenticationEntryPoint`的实现，配置于`KeycloakWebSecurityConfigurerAdapter`。<br />当请求被过滤器`FilterSecurityInterceptor`时发现当前的用户是个匿名用户，不符合/admin/foo的访问控制要求而抛出了`AccessDeniedException`。会通过`ExceptionTranslationFilter`传递给`KeycloakAuthenticationEntryPoint`处理401异常。
```java
@Override
public void commence(HttpServletRequest request, HttpServletResponse response, AuthenticationException authException) throws IOException, ServletException {
    HttpFacade facade = new SimpleHttpFacade(request, response);
    if (apiRequestMatcher.matches(request) || adapterDeploymentContext.resolveDeployment(facade).isBearerOnly()) {
        commenceUnauthorizedResponse(request, response);
    } else {
        commenceLoginRedirect(request, response);
    }
}
```
它执行了两种策略：

- 当请求时登录请求/sso/login或者是BearerOnly（这些属性上一篇可介绍了一部分哦）就直接返回标头含WWW-Authenticate 的401响应。
- 其它情况就跳一个OIDC认证授权请求。
```java
protected void commenceLoginRedirect(HttpServletRequest request, HttpServletResponse response) throws IOException {
    if (request.getSession(false) == null && KeycloakCookieBasedRedirect.getRedirectUrlFromCookie(request) == null) {
        // If no session exists yet at this point, then apparently the redirect URL is not
        // stored in a session. We'll store it in a cookie instead.
        response.addCookie(KeycloakCookieBasedRedirect.createCookieFromRedirectUrl(request.getRequestURI()));
    }

    String queryParameters = "";
    if (!StringUtils.isEmpty(request.getQueryString())) {
        queryParameters = "?" + request.getQueryString();
    }

    String contextAwareLoginUri = request.getContextPath() + loginUri + queryParameters;
    log.debug("Redirecting to login URI {}", contextAwareLoginUri);
    response.sendRedirect(contextAwareLoginUri);
}
```
接口明显走的上面的方法，很明显要跳登录页了。这时需要看看/admin/foo有没有缓存起来，因为登录完还要去执行/admin/foo的逻辑。如果Spring Security没有存Session或者Cookie中也没有就会把/admin/foo缓存到Cookie中，然后重定向到Keycloak授权页:<br />http://localhost:8011/auth/realms/fcant.cn/protocol/openid-connect/auth?response_type=code&client_id=CLIENT&redirect_uri=http%3A%2F%2Flocalhost%3A8080%2Fsso%2Flogin&state=STATE&login=true&scope=openid
<a name="isff1"></a>
### `KeycloakAuthenticationProcessingFilter`
上面是一个典型的 Authorization Code Flow模式。当输入帐号密码同意授权时，授权服务器会请求一个携带code和state的回调链接（这里是/sso/login）。负责拦截处理/sso/login的是`KeycloakAuthenticationProcessingFilter`。这个接口不单单处理登录，只要携带了授权头Authorization、`access_token`、Keycloak Cookie三种之一的它都会拦截处理。<br />在这个过滤器和熟悉的`UsernamePasswordAuthenticationFilter`一样都继承了`AbstractAuthenticationProcessingFilter`其实大致流程也很相似，只不过走的是Keycloak认证授权的API。认证授权成功就从Session中重新获取/admin/foo接口并跳转。整个简单的Keycloak认证授权过程就完成了。
<a name="RHPxf"></a>
### `KeycloakSecurityContextRequestFilter`
这个过滤器功能比较单一，它是用来判断是不是`RefreshableKeycloakSecurityContext`，可刷新的安全上下文，如果是就在`ServletRequest`对象中放个`RefreshableKeycloakSecurityContext`，后续其它过滤器会根据这个标记做一些事情。
<a name="Wmr9W"></a>
### `KeycloakAuthenticatedActionsFilter`
这个过滤器就是用来捕捉`KeycloakSecurityContextRequestFilter`放在请求对象`ServletRequest`中的`RefreshableKeycloakSecurityContext`的。核心就这些：
```java
public boolean handledRequest() {
    log.debugv("AuthenticatedActionsValve.invoke {0}", facade.getRequest().getURI());
    if (corsRequest()) return true;
    String requestUri = facade.getRequest().getURI();
    if (requestUri.endsWith(AdapterConstants.K_QUERY_BEARER_TOKEN)) {
        queryBearerToken();
        return true;
    }
    if (!isAuthorized()) {
        return true;
    }
    return false;
}
```
这里返回`true`就阻断不往下走了。主要是根据Keycloak提供的策略来判断是否已经授权，看上去逻辑还挺复杂的。<br />基于篇幅的原理，后续再详细介绍Keycloak的过滤器，先知道大致它们都干什么用的。
<a name="YdN4v"></a>
## 补充
其实要想搞清楚任何一个框架的运行流程，最好的办法就是从日志打印中提炼一些关键点。Keycloak Spring Security Adapter的运行流程如果想搞清楚，最好是自己先试一试。把开启Keycloak适配器的注解拆解开以打开Spring Security的日志：
```java
@Configuration
@ComponentScan(
        basePackageClasses = {KeycloakSecurityComponents.class}
)
@EnableWebSecurity(debug = true)
public class SecurityConfig extends KeycloakWebSecurityConfigurerAdapter {
//ignore
}
```
为了看到更多日志，把Spring Boot的org相关包的日志也调整为`debug`：
```yaml
logging:
  level:
    org : debug
```
然后代码运行的流程会在控制台Console非常清晰，极大方便了了解Keycloak的运行流程。Keycloak的流程简单了解一下就好，感觉非常平淡无奇，大部分也没有定制化的需要，重心其实不在这里，如何根据业务定制Keycloak的用户管理、角色管理等一系列管理API才是使用好它的关键。
