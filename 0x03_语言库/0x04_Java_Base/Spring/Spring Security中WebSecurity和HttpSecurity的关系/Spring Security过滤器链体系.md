Java Spring Spring Security
<a name="dn4Xs"></a>
## Spring Security过滤器链体系
在学习Spring Security的时候有没有下面这两个疑问：

- Spring Security的登录是怎么配置的？
- Spring Security的访问控制是什么机制？
<a name="MMD8l"></a>
### SpringBootWebSecurityConfiguration
上面两个疑问的答案就在配置类`SpringBootWebSecurityConfiguration`中。可以按照下面这个思维导图去理解这个自动配置：![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644799742879-59092cf5-2e55-4b8c-ab39-c1d0000c3299.webp#clientId=u6200e704-5aaa-4&from=paste&id=u6f35e655&originHeight=379&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f14767f-1cc5-4705-a312-38aea0245c8&title=)<br />`SpringBootWebSecurityConfiguration`为Spring Boot应用提供了一套默认的Spring Security配置。
```java
@Bean
@Order(SecurityProperties.BASIC_AUTH_ORDER)
SecurityFilterChain defaultSecurityFilterChain(HttpSecurity http) throws Exception {
    http.authorizeRequests().anyRequest().authenticated().and().formLogin().and().httpBasic();
    return http.build();
}
```
这里的配置为：所有的请求都必须是认证用户发起的，同时开启表单登录功能以及Http Basic `Authentication`认证功能。访问/foo/bar时需要登录认证并且能够进行表单登录就是这个配置起作用了。这个是日常开发需要自定义的。这个`SecurityFilterChain`到底是什么呢？
<a name="G5ZWc"></a>
#### `SecurityFilterChain`
从上面看得出`HttpSecurity`就是一个构建类，它的使命就是构建出一个`SecurityFilterChain`：
```java
public interface SecurityFilterChain {
    //  当前请求是否匹配
    boolean matches(HttpServletRequest request);
    // 一揽子过滤器组成的有序过滤器链
    List<Filter> getFilters();
}
```
当一个请求`HttpServletRequest`进入`SecurityFilterChain`时，会通过`matches`方法来确定是否满足条件进入过滤器链。就好比VIP走的是VIP通道，享受的是VIP的一系列待遇；如果是普通用户，就走普通用户的通道并享受普通用户的待遇。![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644799742960-8ec2f0bd-f163-4b05-84df-0acbf4eaec9b.webp#clientId=u6200e704-5aaa-4&from=paste&id=u999b51e8&originHeight=400&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4b9cc6a1-c7dd-4847-81ed-44116454729&title=)不管用户是哪种角色，都走的是一个过滤器链，一个应用中存在1-n个`SecurityFilterChain`。那谁来管理多个`SecurityFilterChain`呢？<br />记住这个公式`HttpSecurity` ->`SecurityFilterChain`。
<a name="x2C0F"></a>
#### `FilterChainProxy`
`FilterChainProxy`是一个`GenericFilterBean`（即使Servlet Filter又是Spring Bean），它管理了所有注入Spring IoC容器的`SecurityFilterChain`。刚接触Spring Security的时候是这样配置`FilterChainProxy`的：
```xml
<bean id="myfilterChainProxy" class="org.springframework.security.web.FilterChainProxy">
    <constructor-arg>
        <util:list>
            <security:filter-chain pattern="/do/not/filter*" filters="none"/>
            <security:filter-chain pattern="/**" filters="filter1,filter2,filter3"/>
        </util:list>
    </constructor-arg>
</bean>
```
根据不同的请求路径匹配走不同的`SecurityFilterChain`。下面是示意图：![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644799742841-2a9aed3d-eb01-47bc-89ac-3a5ee3bd91e5.webp#clientId=u6200e704-5aaa-4&from=paste&id=uf72d4e0d&originHeight=761&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue19b7287-8855-40ef-bcb5-8c0242e6f37&title=)后面还会对遇到这个类，现在只需要明白上面这个图就行了。<br />请注意：在同一过滤器链中不建议有多个`FilterChainProxy`实例，而且不应将其作为单纯的过滤器使用，它只应该承担管理`SecurityFilterChain`的功能。
<a name="NRqkF"></a>
#### `DelegatingFilterProxy`
Servlet 容器和Spring IoC容器之间的Filter生命周期并不匹配。为了让Spring IoC容器管理Filter的生命周期，`FilterChainProxy`便交由Spring Web下的`DelegatingFilterProxy`来代理。而且`FilterChainProxy`不会在添加到应用程序上下文的任何过滤器Bean上调用标准Servlet过滤器生命周期方法，`FilterChainProxy`的生命周期方法会委托给`DelegatingFilterProxy`来执行。而`DelegatingFilterProxy`作为Spring IoC和Servlet的连接器存在。![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644799742877-9268008f-0892-448a-af13-f4c8cba615cb.webp#clientId=u6200e704-5aaa-4&from=paste&id=u37a62eab&originHeight=508&originWidth=741&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6bb90c1d-d74c-49b9-92a5-8380ee6418d&title=)
<a name="DPYLv"></a>
### 简单总结
上面的三个概念非常重要，涉及到Spring Security的整个过滤器链体系。
<a name="PTlcZ"></a>
## Spring Security过滤器链匹配到特定的请求
<a name="Kdfp9"></a>
### 如何拦截特定的请求
只有满足了`SecurityFilterChain`的`match`方法的请求才能被该`SecurityFilterChain`处理，那如何配置才能让一个`SecurityFilterChain`处理特定的路径呢？
<a name="FXBYC"></a>
### `RequestMatcher`
`HttpSecurity`内置了`RequestMatcher`属性来处理路径匹配问题。`RequestMatcher`可总结为以下几大类：![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644799513902-74b46c57-cdb5-4d71-aaa2-37f556125e5b.webp#clientId=u6200e704-5aaa-4&from=paste&id=u5443d49c&originHeight=462&originWidth=897&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46ab5f5f-4856-4212-ab0e-a66c3ddb927&title=)使用Ant路径：
```java
httpSecurity.antMatcher("/foo/**");
```
如果配置了全局的Servlet Path的话，例如/v1，配置ant路径的话就要/v1/foo/**，使用MVC风格可以保持一致：
```java
httpSecurity.mvcMatcher("/foo/**");
```
另外MVC风格可以自动匹配后缀，例如/foo/hello可以匹配/foo/hello.do、/foo/hello.action 等等。另外也可以使用正则表达式来进行路径匹配：
```java
httpSecurity.regexMatcher("/foo/.+");
```
如果上面的都满足不了需要的话，可以通过`HttpSecurity.requestMatcher`方法自定义匹配规则；如果想匹配多个规则的话可以借助于`HttpSecurity.requestMatchers`方法来自由组合匹配规则，就像这样：
```java
httpSecurity.requestMatchers(requestMatchers ->
                             requestMatchers.mvcMatchers("/foo/**")
                             .antMatchers("/admin/*get"));
```
一旦配置了路径匹配规则的话，可以发现默认的表单登录404了，因为默认是/login，加了前缀后当然访问不到了。
<a name="ggGbs"></a>
### 使用场景
比如后台管理系统和前端应用各自走不同的过滤器链，可以根据访问路径来配置各自的过滤器链。例如：
```java
/**
 * Admin 过滤器链.
 *
 * @param http the http
 * @return the security filter chain
 * @throws Exception the exception
 */
@Bean
SecurityFilterChain adminSecurityFilterChain(HttpSecurity http) throws Exception {
    http.requestMatchers(requestMatchers -> requestMatchers.mvcMatchers("/admin/**"))
        //todo 其它配置
        return http.build();
}

/**
 * App 过滤器链.
 *
 * @param http the http
 * @return the security filter chain
 * @throws Exception the exception
 */
@Bean
SecurityFilterChain appSecurityFilterChain(HttpSecurity http) throws Exception {
    http.requestMatchers(requestMatchers -> requestMatchers.mvcMatchers("/app/**"));
    //todo 其它配置
    return http.build();
}
```
另外也可以使用该特性降低不同规则URI之间的耦合性。
