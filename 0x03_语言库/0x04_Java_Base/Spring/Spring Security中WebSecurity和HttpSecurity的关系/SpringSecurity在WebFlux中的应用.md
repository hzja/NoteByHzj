JavaSpringSpringSecurityWebFlux
<a name="SkeNZ"></a>
## 前言
网上webflux这一套的SpringSecurity操作资料貌似很少。<br />新项目是前后端分离的项目，前台vue，后端SpringCloud2.0，采用oauth2.0机制来获得用户，权限框架用的SpringSecurity。
<a name="ffcQN"></a>
## 一、前台登录
大概思路前台主要是配合项目中配置的clientId，clientSecret去第三方服务器拿授权码code，然后拿这个code去后端交互，后端根据code去第三方拿用户信息，由于第三方只保存用户信息，不管具体的业务权限，所以会在本地保存一份用户副本，用来做权限关联。<br />用户登录成功后，会把一些用户基本信息（脱敏）生成jwt返回给前端放到head中当`Authorization`，同时后端把一些相关联的菜单，权限等数据放到redis里做关联，为后面的权限控制做准备。
<a name="iGe06"></a>
## 二、SpringSecurity的webflux应用
如果用过SpringSecurity，`HttpSecurity`应该是比较熟悉的，基于Web允许为特定的http请求配置安全性。<br />WebFlux中`ServerHttpSecurity`与`HttpSecurity`提供的相似的类似，但仅适用于WebFlux。默认情况下，它将应用于所有请求，但可以使用`securityMatcher(ServerWebExchangeMatcher)`或其他类似方法进行限制。<br />项目比较特殊，就不能全展示了，大概写一写，开启Security如下：
```java
@EnableWebFluxSecurity
public class MyExplicitSecurityConfiguration {
    @Bean
    SecurityWebFilterChain webFluxSecurityFilterChain(ServerHttpSecurity http) throws Exception {
        http.securityContextRepository(new NoOpServerSecurityContextAutoRepository(tokenProvider)).httpBasic().disable()
            .formLogin().disable()
            .csrf().disable()
            .logout().disable();
        http.addFilterAt(corsFilter(), SecurityWebFiltersOrder.CORS)
            .authorizeExchange()
            .matchers(EndpointRequest.to("health", "info"))
            .permitAll()
            .and()
            .authorizeExchange()
            .pathMatchers(HttpMethod.OPTIONS)
            .permitAll()
            .and()
            .authorizeExchange()
            .pathMatchers(HttpMethod.PUT)
            .denyAll()
            .and()
            .authorizeExchange()
            .pathMatchers(HttpMethod.DELETE)
            .denyAll()
            .and()
            .authorizeExchange()
            .pathMatchers(HttpMethod.HEAD)
            .denyAll()
            .and()
            .authorizeExchange()
            .pathMatchers(HttpMethod.PATCH)
            .denyAll()
            .and()
            .authorizeExchange()
            .pathMatchers(HttpMethod.TRACE)
            .denyAll()
            .and()
            .authorizeExchange()
            .pathMatchers(excludedAuthPages).permitAll()
            .and()
            .authorizeExchange()
            .pathMatchers(authenticatedPages).authenticated()
            .and()
            .exceptionHandling()
            .accessDeniedHandler(new AccessDeniedEntryPointd())
            .and()
            .authorizeExchange()
            .and()
            .addFilterAt(webFilter(), SecurityWebFiltersOrder.AUTHORIZATION)
            .authorizeExchange()
            .pathMatchers("/**").access(new JwtAuthorizationManager(tokenProvider))
            .anyExchange().authenticated();
        return http.build();
    }
}
```
因为是前后端分离项目，所以没有常规的后端的登录操作，把这些`disable`掉。<br />`securityContextRepository`是个用于在请求之间保留`SecurityContext`策略接口，实现类是`WebSessionServerSecurityContextRepository`(session存储)，还有就是`NoOpServerSecurityContextRepository`（用于无状态应用），像JWT这种就用后者，不能用前者，是无状态的应用，没有主动`clear`的操作，会导致内存溢出等问题。<br />`build()`方法中会有一个初始化操作。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935134-83abefa9-d812-4ce8-8038-0c3b6f64c97a.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u8054625d&originHeight=282&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub26041a4-5bde-4161-92fc-e337f94ed35&title=)<br />初始化操作就设置成了`WebSessionServerSecurityContextRepository`，就自己在`SecurityWebFilterChain`中设置成`NoOpServerSecurityContextRepository`。<br />接下来为了满足自定义认证需求，自己配置一个`AuthenticationWebFilter`。
```java
public AuthenticationWebFilter webFilter() {
    AuthenticationWebFilter authenticationWebFilter = new AuthenticationWebFilter(new JWTReactiveAuthenticationManager(userCache, tokenProvider, coreUserApi));
    authenticationWebFilter.setServerAuthenticationConverter(new TokenAuthenticationConverter(guestList, tokenProvider));
    authenticationWebFilter.setRequiresAuthenticationMatcher(new NegatedServerWebExchangeMatcher(ServerWebExchangeMatchers.pathMatchers(excludedAuthPages)));
    authenticationWebFilter.setSecurityContextRepository(new NoOpServerSecurityContextAutoRepository(tokenProvider));
    return authenticationWebFilter;
}
```
几个特殊的类，稍微解释下。
<a name="ETJuO"></a>
#### `AuthenticationWebFilter`
一个执行特定请求身份验证的WebFilter，包含了一整套验证的流程操作，具体上源码看一眼基本能了解个大概。
```java
@Override
public Mono<Void> filter(ServerWebExchange exchange, WebFilterChain chain) {
    return this.requiresAuthenticationMatcher.matches(exchange)
        .filter( matchResult -> matchResult.isMatch())
        .flatMap( matchResult -> this.authenticationConverter.convert(exchange))
        .switchIfEmpty(chain.filter(exchange).then(Mono.empty()))
        .flatMap( token -> authenticate(exchange, chain, token))
        .onErrorResume(AuthenticationException.class, e -> this.authenticationFailureHandler
                       .onAuthenticationFailure(new WebFilterExchange(exchange, chain), e));
}

private Mono<Void> authenticate(ServerWebExchange exchange, WebFilterChain chain, Authentication token) {
return this.authenticationManagerResolver.resolve(exchange)
    .flatMap(authenticationManager -> authenticationManager.authenticate(token))
    .switchIfEmpty(Mono.defer(() -> Mono.error(new IllegalStateException("No provider found for " + token.getClass()))))
    .flatMap(authentication -> onAuthenticationSuccess(authentication, new WebFilterExchange(exchange, chain)));
}

protected Mono<Void> onAuthenticationSuccess(Authentication authentication, WebFilterExchange webFilterExchange) {
ServerWebExchange exchange = webFilterExchange.getExchange();
SecurityContextImpl securityContext = new SecurityContextImpl();
securityContext.setAuthentication(authentication);
return this.securityContextRepository.save(exchange, securityContext)
    .then(this.authenticationSuccessHandler
          .onAuthenticationSuccess(webFilterExchange, authentication))
    .subscriberContext(ReactiveSecurityContextHolder.withSecurityContext(Mono.just(securityContext)));
}
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935218-62069cfb-a3a1-41a6-add5-bcaa3c48f384.png#clientId=u30c73f4a-3dbb-4&from=paste&id=ucab54e19&originHeight=591&originWidth=825&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u42940050-31c7-40c9-9be1-04fc4685266&title=)
<a name="YdXLU"></a>
#### `ServerWebExchangeMatcher`
一个用来匹配URL用来验证的接口，代码中用的是他的实现类`NegatedServerWebExchangeMatcher`，这个类就是指一些设置的白名单的url就不要验证了，他还有许多实现类，具体可以参见源码，这里就不累述了。
<a name="wz5Hy"></a>
#### `ServerAuthenticationConverter`
一个用于从`ServerWebExchange`转换为用于通过提供的`org.springframework.security.authentication.ReactiveAuthenticationManager`进行身份验证的`Authentication`的策略。如果结果为`Mono.empty()`，则表明不进行任何身份验证尝试。这边自己实现了一个`TokenAuthenticationConverter`，主要功能就是通过JWT转换成`Authentication(UsernamePasswordAuthenticationToken)`。
<a name="l1dDN"></a>
#### `ReactiveAuthenticationManager`
对提供的`Authentication`进行身份验证，基本上核心的验证操作就在它提供的唯一方法`authenticate`里进行操作，根据conver那边转换过来的`Authentication`当参数进行具体的验证操作，简述如下：
```java
@Override
public Mono<Authentication> authenticate(final Authentication authentication) {
    if (authentication.isAuthenticated()) {
        return Mono.just(authentication);
    }
    return Mono.just(authentication)
            .switchIfEmpty(Mono.defer(this::raiseBadCredentials))
            .cast(UsernamePasswordAuthenticationToken.class)
            .flatMap(this::authenticateToken)
            .publishOn(Schedulers.parallel())
            .onErrorResume(e -> raiseBadCredentials())
            .switchIfEmpty(Mono.defer(this::raiseBadCredentials))
            .map(u -> {
                UsernamePasswordAuthenticationToken usernamePasswordAuthenticationToken = new UsernamePasswordAuthenticationToken(authentication.getPrincipal(), authentication.getName(), Collections.EMPTY_LIST);
                usernamePasswordAuthenticationToken.setDetails(u);
                return usernamePasswordAuthenticationToken;
            });
}
```
<a name="qH20u"></a>
#### `ServerSecurityContextRepository`
用于在请求之间保留`SecurityContext`，因为在登录成功后是需要保存一个登录的数据，用来后面的请求进行相关的操作。因为是无状态的，所以其实`NoOpServerSecurityContextRepository`是能满足需求的，不需要进行实际的save，但是load稍微要改造下，所以实现了`ServerSecurityContextRepository`，仿照`NoOpServerSecurityContextRepository`，实现了一个自定义的Repository。<br />为什么`load`要改造，虽然是无状态的，但是实际上每次请求，依然要区分到底是谁，为了后面的权限验证做准备，所以根据jwt可以生成一个`SecurityContext`放入`ReactiveSecurityContextHolder`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935159-1888dcf2-85cd-4b3d-8ccb-f928d0acefca.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u192be713&originHeight=207&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaeec801c-01d8-4217-bf33-8c948e68967&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935167-7973a5a9-b631-42b7-8d9a-92f12cea0481.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u7fb873b0&originHeight=568&originWidth=944&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc7b11532-985c-4260-a63f-004db1a192c&title=)
```java
public class NoOpServerSecurityContextAutoRepository
        implements ServerSecurityContextRepository {

    private TokenProvider tokenProvider;

    public NoOpServerSecurityContextAutoRepository(TokenProvider tokenProvider) {
        this.tokenProvider = tokenProvider;
    }

    public Mono<Void> save(ServerWebExchange exchange, SecurityContext context) {
        return Mono.empty();

    }

    public Mono<SecurityContext> load(ServerWebExchange exchange) {
        String token = exchange.getRequest().getHeaders().getFirst("Authorization");
        if (StrUtil.isNotBlank(token)) {
            SecurityContext securityContext = new SecurityContextImpl();
            securityContext.setAuthentication(new UsernamePasswordAuthenticationToken("password", token, Collections.EMPTY_LIST));
            return Mono.justOrEmpty(securityContext);
        } else {
            return Mono.empty();
        }
    }
}
```
<a name="btvtK"></a>
### 权限验证
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935170-b26441fa-c386-4855-b715-0e3388809e3c.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u33969f98&originHeight=90&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud3ff0028-a125-47b3-870b-f06d7d7f12c&title=)<br />权限验证是在图上配置的。大概的流程，可以看下面的截图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935440-5d1ae431-d1c0-4408-bd61-53523a1d930c.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u2acf0473&originHeight=314&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0cb16987-ff97-49e4-b1fc-f707de4e8bc&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935452-0035c44d-49bb-4795-a8d6-b41c5d37379a.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u000fd64a&originHeight=172&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u60314f83-305b-4d5f-b71a-5d2db64a899&title=)
<a name="Uvz67"></a>
#### `AuthorizationWebFilter`
跟到里面，发现了最主要的就是这个`AuthorizationWebFilter`，用来做权限验证的，然后在`filter`方法里面就看得很清楚了，他第一步就是拿的`ReactiveSecurityContextHolder.getContext()`，然后之前在`ReactorContextWebFilter`里的`load`操作就是从`NoOpServerSecurityContextAutoRepository`里塞到`ReactiveSecurityContextHolder`里，因为本质 来说SpringSecurity就是个filter集合，从`ReactorContextWebFilter`里`load`，然后在`AuthorizationWebFilter`取，这样就能拿到`Authentication`来做权限验证了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935449-10543999-8ac2-44b2-a64b-f893038b52db.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u1a0dfbb2&originHeight=511&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7b84652f-43c0-4f07-afb5-5c9d18512da&title=)
<a name="bUVx1"></a>
#### `ReactiveAuthorizationManager`
反应式授权管理器接口，可以确定`Authentication`是否有权访问特定对象。其实看源码就很清楚了，就是根据`Authentication`来做具体的权限验证。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667056935508-fff8f55f-851b-4614-9cd0-dc638f9e416b.png#clientId=u30c73f4a-3dbb-4&from=paste&id=u2bc62ebc&originHeight=428&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8fb6904c-cb2b-460c-a6fb-713cc628765&title=)<br />代码很清楚，主要是写check方法。所以这边自已实现了一个`JwtAuthorizationManager`类用来做具体的check，简单来说就是拿`Authentication`里的内容去redis里查对应的菜单权限。
