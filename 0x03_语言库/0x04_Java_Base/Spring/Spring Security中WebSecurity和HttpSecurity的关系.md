Java Spring WebSecurity HttpSecurity
<a name="ye0fL"></a>
## Spring Security 5.4版本带来的新玩法
<a name="OBque"></a>
### 1. 前言
在以往Spring Security的中自定义配置都是声明一个配置类`WebSecurityConfigurerAdapter`，然后覆写（`@Override`）对应的几个方法就行了。然而这一切在Spring Security 5.4开始就得到了改变，从Spring Security 5.4 起不需要继承`WebSecurityConfigurerAdapter`就可以配置`HttpSecurity`了。相关的说明原文：

- Remove need for WebSecurityConfigurerAdapter #8805
- Configure HTTP Security without extending WebSecurityConfigurerAdapter #8804
<a name="ZafWF"></a>
### 2. 新的配置方式
旧的配置方式目前依然有效：
```java
@Configuration
static class SecurityConfig extends WebSecurityConfigurerAdapter {
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
            .antMatcher("/**")
            .authorizeRequests(authorize -> authorize
                               .anyRequest().authenticated()
                              );
    }
}
```
5.4.x版本有新的选择：
```java
@Bean
SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
 return http
   .antMatcher("/**")
   .authorizeRequests(authorize -> authorize
     .anyRequest().authenticated()
   )
   .build();
}
```
这种JavaConfig的方式看起来更加清爽舒服，而且和适配器解耦了。上面`filterChain`方法的参数是`HttpSecurity`类型。熟悉`@Bean`注解的同学应该会意识到一定有一个`HttpSecurity`类型的Spring Bean。在`HttpSecurityConfiguration`中有一个这样的Bean：
```java
@Bean({"org.springframework.security.config.annotation.web.configuration.HttpSecurityConfiguration.httpSecurity"})
@Scope("prototype")
HttpSecurity httpSecurity() throws Exception {
    // 省略掉
    return http;
}
```
初始化的内容已经忽略掉，注意到`HttpSecurity`被`@Scope("prototype")`标记。也就是这个`HttpSecurityBean`不是单例的，每一次请求都会构造一个新的实例。这个设定非常方便构建多个互相没有太多关联的`SecurityFilterChain`，进而能在一个安全体系中构建相互隔离的安全策略。比如后端管理平台用Session模式，前台应用端用Token模式。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1619230235774-526e927c-738e-4c3f-bb04-ebd4fd509fe2.webp#clientId=u543d8ff6-2555-4&from=paste&id=u6a0a454a&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=udbde48f8-b16d-403a-b704-c13071c100d)<br />多个SecurityFilterChain
<a name="KR6a5"></a>
### 3. 原理
Spring Security 有一个名为`springSecurityFilterChain`默认的过滤器链类（实际位置就是上图的Bean Filter位置），其类型为`FilterChainProxy`， 它代理了所有的`SecurityFilterChain`，关键的代理注入代码：
```java
for (SecurityFilterChain securityFilterChain : this.securityFilterChains) {
   this.webSecurity.addSecurityFilterChainBuilder(() -> securityFilterChain);
   for (Filter filter : securityFilterChain.getFilters()) {
      if (filter instanceof FilterSecurityInterceptor) {
         this.webSecurity.securityInterceptor((FilterSecurityInterceptor) filter);
         break;
      }
   }
}
```
那么`this.securityFilterChains`来自哪里呢？
```java
@Autowired(required = false)
void setFilterChains(List<SecurityFilterChain> securityFilterChains) {
   securityFilterChains.sort(AnnotationAwareOrderComparator.INSTANCE);
   this.securityFilterChains = securityFilterChains;
}
```
到这里就一目了然了吧，`SecurityFilterChain`类型的Bean会被加载到`this.securityFilterChains`中。
<a name="mb1DH"></a>
## HttpSecurity的本质
在Spring Security 5.4的新玩法中介绍了一种新的配置`HttpSecurity`的方式：
```java
@Bean
SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
    return http
            .antMatcher("/**")
            .authorizeRequests(authorize -> authorize
                    .anyRequest().authenticated()
            )
            .build();
}
```
其实就能够知道`HttpSecurity`是用来构建包含了一系列过滤器链的过滤器`SecurityFilterChain`，平常的配置就是围绕构建`SecurityFilterChain`进行。继续看这张图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1619230156539-b0b2ccfc-9c26-4ecc-9732-aae41db8547b.webp#clientId=u543d8ff6-2555-4&from=paste&id=uca39f8a6&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u69837262-6d2d-4f13-b954-58737dfac03)<br />安全过滤链<br />从上面这个图中可以看出构建好的还要交给`FilterChainProxy`来代理，是不是有点多此一举？
<a name="tmYm9"></a>
## WebSecurity的本质
在有些情况下这种确实多此一举， 不过更多时候可能需要配置多个`SecurityFilterChain`来实现对多种访问控制策略。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1619230156707-0a627a2f-2fa2-46b9-8cdb-369afdca6a9c.webp#clientId=u543d8ff6-2555-4&from=paste&id=ua3e5bb7a&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=uc2f68ef3-f565-40f5-a5c7-9b2736ab542)<br />多个SecurityFilterChain<br />为了精细化的管理多个`SecurityFilterChain`的生命周期，搞一个统一管理这些`SecurityFilterChain`的代理就十分必要了，这就是`WebSecurity`的意义。下面是`WebSecurity`的`build`方法的底层逻辑：
```java
@Override
protected Filter performBuild() throws Exception {
   Assert.state(!this.securityFilterChainBuilders.isEmpty(),
         () -> "At least one SecurityBuilder<? extends SecurityFilterChain> needs to be specified. "
               + "Typically this is done by exposing a SecurityFilterChain bean "
               + "or by adding a @Configuration that extends WebSecurityConfigurerAdapter. "
               + "More advanced users can invoke " + WebSecurity.class.getSimpleName()
               + ".addSecurityFilterChainBuilder directly");
    // 被忽略请求的个数 和 httpscurity的个数 构成了过滤器链集合的大小
   int chainSize = this.ignoredRequests.size() + this.securityFilterChainBuilders.size();
   List<SecurityFilterChain> securityFilterChains = new ArrayList<>(chainSize);
    // 初始化过滤器链集合中的 忽略请求过滤器链    
    for (RequestMatcher ignoredRequest : this.ignoredRequests) {
      securityFilterChains.add(new DefaultSecurityFilterChain(ignoredRequest));
   }
    // 初始化过滤器链集合中的 httpsecurity定义的过滤器链
   for (SecurityBuilder<? extends SecurityFilterChain> securityFilterChainBuilder : this.securityFilterChainBuilders) {
      securityFilterChains.add(securityFilterChainBuilder.build());
   }
   FilterChainProxy filterChainProxy = new FilterChainProxy(securityFilterChains);
   if (this.httpFirewall != null) {
       // 请求防火墙
      filterChainProxy.setFirewall(this.httpFirewall);
   }
   if (this.requestRejectedHandler != null) {
       // 请求拒绝处理器
      filterChainProxy.setRequestRejectedHandler(this.requestRejectedHandler);
   }
   filterChainProxy.afterPropertiesSet();

   Filter result = filterChainProxy;
   if (this.debugEnabled) {
      this.logger.warn("\n\n" + "********************************************************************\n"
            + "**********        Security debugging is enabled.       *************\n"
            + "**********    This may include sensitive information.  *************\n"
            + "**********      Do not use in a production system!     *************\n"
            + "********************************************************************\n\n");
      result = new DebugFilter(filterChainProxy);
   }
   this.postBuildAction.run();
   return result;
}
```
从上面中的源码可以看出，`WebSecurity`用来构建一个名为`springSecurityFilterChain`的`Spring BeanFilterChainProxy`。它的作用是来定义哪些请求忽略安全控制，哪些请求必须安全控制，在合适的时候清除`SecurityContext`以避免内存泄漏，同时也可以用来定义请求防火墙和请求拒绝处理器，另外开启Spring Seuciry Debug模式也是这里配置的。<br />同时还有一个作用可能是其它文章没有提及的，`FilterChainProxy`是Spring Security对Spring framework应用的唯一出口，然后通过它与一个`Servlet`在Spring的桥接代理`DelegatingFilterProxy`结合构成Spring对Servlet体系的唯一出口。这样就将Spring Security、Spring framework、Servlet API三者隔离了起来。
<a name="WerNU"></a>
## 总结
事实上可以认为，`WebSecurity`是Spring Security对外的唯一出口，而`HttpSecurity`只是内部安全策略的定义方式；`WebSecurity`对标`FilterChainProxy`，而`HttpSecurity`则对标`SecurityFilterChain`，另外它们的父类都是`AbstractConfiguredSecurityBuilder`。掌握了这些基本上就能知道它们之间的区别是什么了。
