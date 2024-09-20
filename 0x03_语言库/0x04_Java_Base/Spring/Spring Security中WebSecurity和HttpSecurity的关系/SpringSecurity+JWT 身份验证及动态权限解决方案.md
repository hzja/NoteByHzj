JavaSpringSecurityJWT<br />写了一个SpringSecurity集合JWT完成身份验证的Demo，并按照自己的想法完成了动态权限问题。在写这个Demo之初，使用的是SpringSecurity自带的注解权限，但是这样权限就显得不太灵活，在实现之后，感觉也挺复杂的，欢迎大家给出建议。
<a name="ziHAt"></a>
### **认证流程及授权流程**
画了个建议的认证授权流程图，后面会结合代码进行解释整个流程。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659577598527-277f0439-f065-4f52-b6b5-b103f4a93998.jpeg)
<a name="hyeD3"></a>
### **一、登录认证阶段**
实现`SpringSecurity`的`UsernamePasswordAuthenticationFilter`接口（`public class TokenLoginFilter extends UsernamePasswordAuthenticationFilter`），在它的实现类的构造方法里设置登录的请求路径和请求方式。
```java
this.setPostOnly(false);
// 认证路径 - 发送什么请求，就会进行认证
this.setRequiresAuthenticationRequestMatcher(new AntPathRequestMatcher("/service_auth/admin/index/login","POST"));
```
当前端发起配置的请求时，请求会被拦截，进入到`attemptAuthentication`方法进行验证，在这个方法里可以从request中取出账号、密码，从而调用`AuthenticationManager`的`authenticate`去校验账号、密码是否正确。
```java
@Override
public Authentication attemptAuthentication(HttpServletRequest request, HttpServletResponse response)
        throws AuthenticationException {
    try {
        User user = new ObjectMapper().readValue(request.getInputStream(), User.class);
        // 也可以直接获取账号密码
        String username = obtainUsername(request);
        String password = obtainPassword(request);
        log.info("TokenLoginFilter-attemptAuthentication：尝试认证，用户名:{}, 密码:{}", username, password);
        // 在authenticate里去进行校验的，校验过程中会去把UserDetailService里返回的SecurityUser(UserDetails)里的账号密码和这里传的账号密码进行比对
        // 并在UserDetailService里将权限进行赋予
        // 校验通过，会进入到successfulAuthentication方法
        return authenticationManager.authenticate(new UsernamePasswordAuthenticationToken(user.getUsername(), user.getPassword(), new ArrayList<>()));
    } catch (IOException e) {
        throw new RuntimeException(e);
    }
}
```
<a name="lpqfm"></a>
#### 那么这个authenticate方法是怎么验证账号密码正确性的呢？
打上断点，跟随源码，进入到`authenticate`方法内部：<br />![2022-08-04-09-42-59.446396700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577414594-8cc985a6-4ce1-4fc8-b2e2-12f24c211f70.png#clientId=u32e8e6fd-93f5-4&from=ui&id=ud9184fbf&originHeight=66&originWidth=395&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3285&status=done&style=none&taskId=uc2b73e5d-95e6-40f0-a301-a1b8280f8de&title=)<br />然后进入这个方法内部，继续往下走，看到一段核心代码：<br />![2022-08-04-09-42-59.512049600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577414624-75792416-22f2-4286-870b-2a5e3f191551.png#clientId=u32e8e6fd-93f5-4&from=ui&id=QClON&originHeight=78&originWidth=684&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5697&status=done&style=none&taskId=ubdd29c82-a351-4788-a73e-8224ac7d718&title=)<br />进入retrieveUser方法里，然后往下走，看到一句核心代码，这个核心代码就是获取用户信息的：<br />![2022-08-04-09-42-59.578174600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577429313-428f7815-5565-4a4e-b74a-67b86a7ac35e.png#clientId=u32e8e6fd-93f5-4&from=ui&id=ubfc5fc8c&originHeight=71&originWidth=655&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4819&status=done&style=none&taskId=uec0cd2bd-5621-40f8-bcb1-b5493c501c1&title=)<br />这里注意，调用了`UserDetailsService`的`loadUserByUsername`方法，传入的就是前端传过来的username，意思就是要根据这个username去获取`UserDetails`对象，所以就要去查询数据库，所以就要实现`UserDetailsService`接口并重写`loadUserByUsername`方法。
```java
@Service("userDetailsService")
@Slf4j
public class UserDetailServiceImpl implements UserDetailsService {
    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        log.info("根据username去数据库查询用户信息，username:{}", username);
        // 1、从数据库中取出用户信息 - 这里模拟，直接new一个User对象
        User user = new User();
        user.setUsername(username);
        // 111111经过加密后
        user.setPassword("96e79218965eb72c92a549dd5a330112");
        SecurityUser securityUser = new SecurityUser(user);
        // 可以根据查出来的user.getId()去查询这个用户对应的权限集合 - 这里模拟，直接new一个结合
        List<String> authorities = new ArrayList<>();
        // 将权限赋予用户
        securityUser.setPermissionValueList(authorities);
        return securityUser;
    }
}
```
在这个方法里，通过查询数据库，获取用户username、password和其对应的权限并设置到UserDetails对象里(代码里的`SecurityUser`是自己`implements UserDetails`的，也就是它的子类)。<br />获取到userDetails对象后，回到之前的代码(retriveUser所在的地方)里，这个user经过包装，里面包含从数据库里取出的username、password。<br />![2022-08-04-09-42-59.653022900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577429210-3eb76280-1194-459a-a286-545965f31de4.png#clientId=u32e8e6fd-93f5-4&from=ui&id=ytL4m&originHeight=72&originWidth=706&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6108&status=done&style=none&taskId=uc090789b-d7e5-4440-a723-6b01b07ef20&title=)<br />接着往下看，看到核心代码：<br />![2022-08-04-09-42-59.715743000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577443496-9cbae31b-3482-4d67-845c-55e210dce20e.png#clientId=u32e8e6fd-93f5-4&from=ui&id=u28c81031&originHeight=48&originWidth=711&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3849&status=done&style=none&taskId=u469639f3-ebdc-43c5-a7d5-b02e3c98a74&title=)<br />注意这个`additionalAuthenticationChecks`方法，进入到这个方法内部：<br />![2022-08-04-09-43-00.055497700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577443503-1b25c956-426b-4b4f-809f-b13343727f36.png#clientId=u32e8e6fd-93f5-4&from=ui&id=g0geV&originHeight=99&originWidth=677&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13276&status=done&style=none&taskId=u20a80015-f589-4216-850c-0990a59f3bf&title=)<br />可以发现，这是对比密码的，即前端传过来的密码和数据库中存储的已经加密过的密码是否能匹配上。然后回到之前的代码里，直接到结尾，返回一个对象。<br />![2022-08-04-09-43-00.126228500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659577443521-0a5fda64-e143-46c7-94a4-e51abd78e268.png#clientId=u32e8e6fd-93f5-4&from=ui&id=XxcJ9&originHeight=126&originWidth=614&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8234&status=done&style=none&taskId=u75da56d9-1587-4744-8ff7-46f0395ee74&title=)<br />在账号、密码验证完之后的一系列操作里，`SpringSecurity`自己再对数据进行一些封装放到`SecurityContextHolder`里。<br />至此，用户的认证流程已经走完。
<a name="C8fvC"></a>
#### 认证成功之后
认证成功之后，要告诉前端登录认证通过，会进入`UsernamePasswordAuthenticationFilter`的`successfulAuthentication`方法里。
```java
/**
 * 登录成功
 * @param request request
 * @param response response
 * @param chain chain
 * @param auth auth
 * @throws IOException
 * @throws ServletException
 */
@Override
protected void successfulAuthentication(HttpServletRequest request, HttpServletResponse response, FilterChain chain,
                                        Authentication auth) throws IOException, ServletException {
    log.info("TokenLoginFilter-successfulAuthentication：认证通过！");
    SecurityUser user = (SecurityUser) auth.getPrincipal();
    // 创建token
    String token = tokenManager.createToken(user.getCurrentUserInfo().getUsername());
    log.info("创建的Token为：{}", token);
    // 这里建议，以username为Key，权限集合为value将权限存入Redis，因为权限在后面会频繁被取出来用
//        redisTemplate.opsForValue().set(user.getCurrentUserInfo().getUsername(), user.getPermissionValueList());
    // 响应给前端调用处
    ResponseUtil.out(response, ResponseResult.ok().data("token", token));
}
```
在这个方法里，创建一个token，并相应给前端调用者。`ResponseUtil`是封装的一个响应工具，`tokenManager`是JWT工具，这里不做过多解释。<br />因为这个Demo是基于前后端分离的，因此只需响应给前端结果(比如这里的token)即可，让前端来跳转。如果不是前后端分离的，可以在这里进行页面跳转。
<a name="cScrr"></a>
#### 如果认证失败
认证失败，会进入`UsernamePasswordAuthenticationFilter`的`unsuccessfulAuthentication`方法里。
```java
/**
 * 登录失败
 * @param request
 * @param response
 * @param e
 * @throws IOException
 * @throws ServletException
 */
@Override
protected void unsuccessfulAuthentication(HttpServletRequest request, HttpServletResponse response,
                                          AuthenticationException e) throws IOException, ServletException {
    log.info("TokenLoginFilter-unsuccessfulAuthentication：认证失败！");
    // 响应给前端调用处
    ResponseUtil.out(response, ResponseResult.error());
}
```
在这个方法里，直接响应给前端错误情况即可。因为这个Demo是基于前后端分离的，因此只需响应给前端结果、状态码即可，让前端来跳转。如果不是前后端分离的，可以在这里进行页面跳转。
<a name="vk7lw"></a>
### **二、授权阶段 - 如果要做权限控制**
继承`BasicAuthenticationFilter`类，重写`doFilterInternal`过滤器，在这个过滤器里获取token并验证，并进行动态权限控制。
```java
@Slf4j
public class TokenAuthenticationFilter extends BasicAuthenticationFilter {
    private TokenManager tokenManager;

    private AntPathMatcher antPathMatcher = new AntPathMatcher();

    public TokenAuthenticationFilter(AuthenticationManager authManager, TokenManager tokenManager) {
        super(authManager);
        this.tokenManager = tokenManager;
    }

    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        UsernamePasswordAuthenticationToken authentication = null;
        try {
            log.info("授权过滤器，验证Token...");
            authentication = getAuthentication(request);
        } catch (ExpiredJwtException e) {
            // 可能token过期
            log.info("异常捕获：{}",e.getMessage());
            ResponseUtil.out(response, ResponseResult.unauthorized());
        }
        if (authentication != null) {
            String url = request.getRequestURI();
            // setAuthentication设置不设置都行，如果需要用注解来控制权限，则必须设置
            SecurityContextHolder.getContext().setAuthentication(authentication);
            UserServiceImpl userService = new UserServiceImpl();
            List<Menu> menuList = userService.getAllMenus();
            // 遍历所有菜单
            for (Menu menu : menuList) {
                // 如果url匹配上了
                if (antPathMatcher.match(menu.getPattern(), url) && menu.getRoles().size() > 0){
                    log.info("URL匹配上了，请求URL:{}，匹配上的URL:{}", url, menu.getPattern());
                    List<String> stringList = new ArrayList<>();
                    for (GrantedAuthority authority : authentication.getAuthorities()) {
                        String authority1 = authority.getAuthority();
                        stringList.add(authority1);
                    }
                    for (Role role : menu.getRoles()) {
                        if (stringList.contains(role.getName())) {
                            log.info("角色匹配，角色为:{}", role.getName());
                            chain.doFilter(request, response);
                            return;
                        }
                    }
                    // 没有权限
                    log.info("URL匹配上了，但无权访问，请求URL:{}，匹配上的URL:{}", url, menu.getPattern());
                    ResponseUtil.out(response, ResponseResult.noPermission());
                    return;
                }
            }
            // url没有匹配上菜单，可以访问
            log.info("URL未匹配上，所有人都可以访问！");
            chain.doFilter(request, response);
        } else {
            // 没有登录
            log.info("用户Token无效！");
        }
    }

    private UsernamePasswordAuthenticationToken getAuthentication(HttpServletRequest request) {
        // token置于header里
        String token = request.getHeader("X-Token");
        log.info("X-Token:{}", token);
        if (token != null && !"".equals(token.trim())) {
            // 根据token获取用户名
            String userName = tokenManager.getUserFromToken(token);
            // 这里可以根据用户名去Redis中取出权限集合
            // 不应该从SecurityContextHolder获取，会出现问题，如果你换一个token(这个token也是有效的)来调用方法，从这里取，这权限还是之前token登录时存进来的(经过测试)
            // 为什么呢？猜测是：因为JWT是无状态的，你没有办法在注销的时候，将SpringSecurity全局对象里的东西清理
            // 如果你先用账号2登录获取一个token2，然后用账号1登录获取一个token1，用token1去调用一次api的时候从SecurityContextHolder获取一次权限，然后用token2去调用一次api获取一次权限，会发现这个权限居然是token1拥有的(测试过)
//            Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
//            List<String> permissionValueList = (List<String>) redisTemplate.opsForValue().get(userName);
            // 这里直接模拟从Redis中取出权限
            List<String> permissionValueList = new ArrayList<>();
            // 权限 - 为了测试根据权限控制访问权限
            permissionValueList.add("admin.test");
            // 角色 - 为了测试根据角色控制访问权限
            permissionValueList.add("ROLE_admin");
            // 需要将权限转换成SpringSecurity认识的
            Collection<GrantedAuthority> authorities = new ArrayList<>();
            for(String permissionValue : permissionValueList) {
                if(StringUtils.isEmpty(permissionValue)) {
                    continue;
                }
                SimpleGrantedAuthority authority = new SimpleGrantedAuthority(permissionValue);
                authorities.add(authority);
            }
            if (!StringUtils.isEmpty(userName)) {
                log.info("授权过滤器：授权完成！");
                return new UsernamePasswordAuthenticationToken(userName, token, authorities);
            }
            return null;
        }
        return null;
    }
}
```
如果不做动态权限，则可以省略那一部分对比url的代码。但是用户的拥有的权限，建议存储在Redis里。每次进入到这个过滤器，就将其取出来封装成Security认识的，放到`SecurityContextHolder`里，这个时候权限是定死了的，可以在配置文件里进行配置，也可以使用注解在Controller里进行控制。<br />如果要做动态权限，则可以根据自己的逻辑修改那一部分对比url的代码。基于角色控制权限，某个角色拥有某些权限(可访问接口)的这个原则来做。<br />在写这一部分代码的时候，感觉还是有点复杂的。用户的权限、所有菜单都可以存放在Redis里，两者发生改变的时候就清除Redis里的数据，下一次读取的时候先从数据库里读取，然后将其放到Redis缓存里，这一部分逻辑，由于一开始只是打算写一个小Demo（如果真要写的话，还需要创建相关的数据库表、连接Redis之类的，有点麻烦），所以没有写，如果有兴趣可以自己去实现以下，这个并不难。
<a name="j5N3s"></a>
### **三、注销阶段**
注销的时候，应该将Redis里存储的权限进行删除(如果有的话)。
```java
public class TokenLogoutHandler implements LogoutHandler {
    /** Token管理类 */
    private TokenManager tokenManager;

    public TokenLogoutHandler(TokenManager tokenManager) {
        this.tokenManager = tokenManager;
    }

    /**
     * 登出业务处理
     * @param request request
     * @param response response
     * @param authentication
     */
    @Override
    public void logout(HttpServletRequest request, HttpServletResponse response, Authentication authentication) {
        String token = request.getHeader("X-Token");
        if (token != null) {
            /* tokenManager.removeToken(token); */
            try{
                String userName = tokenManager.getUserFromToken(token);
            }catch (ExpiredJwtException e){
                // 可能token过期了，所以得捕获
                ResponseUtil.out(response, ResponseResult.ok());
            }
            // 清空当前用户缓存中的权限数据
            // 如果权限使用userName作为key存在Redis中，可以通过userName将缓存清空
            // ....
        }
        ResponseUtil.out(response, ResponseResult.ok());
    }

}
```
<a name="KbGXh"></a>
### **四、未授权处理**
如果使用了注解或是在配置文件中手动配置了权限管理，即让SpringSecurity管理权限，当访问到没有权限访问的接口时，回来到这里。
```java
@Slf4j
public class UnauthorizedEntryPoint implements AuthenticationEntryPoint {
    /**
     * 未授权返回错误码
     * @param request request
     * @param response response
     * @param authException authException
     * @throws IOException
     * @throws ServletException
     */
    @Override
    public void commence(HttpServletRequest request, HttpServletResponse response,
                         AuthenticationException authException) throws IOException, ServletException {
        // 响应错误码
        ResponseUtil.out(response, ResponseResult.error());
    }
}
```
<a name="diERn"></a>
### **五、SpringSecurity配置文件核心代码**
这里主要是想说一件事，就是在认证、授权、退出处理器的配置时，如果这个类里需要使用到其他类(对象)，可以通过构造方法的方式传进去，因为它们没有被Spring管理，是没有办法使用`@Autowired`注入的。
```java
/**
 * 配置设置 - 更多配置项见官方文档
 * @param http http
 * @throws Exception
 */
@Override
protected void configure(HttpSecurity http) throws Exception {
    http.exceptionHandling()
            .authenticationEntryPoint(new UnauthorizedEntryPoint())
            // 允许跨域
            .and().cors()
            .and().csrf().disable()
            .authorizeRequests()
            .anyRequest().authenticated()
            // 退出请求路径
            .and().logout().logoutUrl("/service_auth/admin/index/logout")
            // 退出处理器
            .addLogoutHandler(new TokenLogoutHandler(tokenManager)).and()
            // 认证过滤器
            .addFilter(new TokenLoginFilter(authenticationManager(), tokenManager))
            // 授权过滤器
            .addFilter(new TokenAuthenticationFilter(authenticationManager(), tokenManager)).httpBasic();
}
```
<a name="A2YzJ"></a>
### **总结**
核心其实就是前面贴出的一、二(认证和授权)，其实还是有优化空间的，对于SpringSecurity没有了解很深，所以只能写成这样。<br />如果读者感兴趣，可以下载源码阅读，源码里注释也是非常详细的。

- [https://github.com/liuchengyin01/JwtWithSpringSecurityDemo](https://github.com/liuchengyin01/JwtWithSpringSecurityDemo)
- [https://gitee.com/liuchengyin_vae/JwtWithSpringSecurityDemo](https://gitee.com/liuchengyin_vae/JwtWithSpringSecurityDemo)
