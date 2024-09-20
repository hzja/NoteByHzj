Java Spring Security<br />![2021-08-03-22-58-16-790863.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628002709026-c584692a-6884-461c-b37f-fb768c4b8c1e.jpeg#clientId=u25188d7c-81db-4&from=ui&id=u10447f14&originHeight=512&originWidth=1080&originalType=binary&ratio=1&size=14844&status=done&style=shadow&taskId=u137609ca-dc70-4d8f-a6d9-121cf09efd5)<br />Keycloak对流行的Java应用提供了适配器。Keycloak同样提供Spring Security的适配器。
<a name="rhRHB"></a>
## 适配器集成
在Spring 应用中集成keycloak-spring-security-adapter：
```xml
<dependency>
    <groupId>org.keycloak</groupId>
    <artifactId>keycloak-spring-security-adapter</artifactId>
    <version>15.0.0</version>
</dependency>
```
在Spring Boot中可以这样集成：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-security</artifactId>
</dependency>
<dependency>
    <groupId>org.keycloak</groupId>
    <artifactId>keycloak-spring-boot-starter</artifactId>
    <version>15.0.0</version>
</dependency>       
```
然后就能利用Spring Security的特性来集成Keycloak。Keycloak 提供了一个 `KeycloakWebSecurityConfigurerAdapter` 作为创建`WebSecurityConfigurer` 实例的方便基类。可以编写了一个配置类来定制安全策略，就像这样：
```java
@KeycloakConfiguration
public class SecurityConfig extends KeycloakWebSecurityConfigurerAdapter
{
    /**
     *  注册了一个Keycloak的AuthenticationProvider
     */
    @Autowired
    public void configureGlobal(AuthenticationManagerBuilder auth) throws Exception {
        auth.authenticationProvider(keycloakAuthenticationProvider());
    }

    /**
     * 定义会话策略
     */
    @Bean
    @Override
    protected SessionAuthenticationStrategy sessionAuthenticationStrategy() {
        return new RegisterSessionAuthenticationStrategy(new SessionRegistryImpl());
    }

    /**
     * 常见的Spring Security安全策略
     */ 
    @Override
    protected void configure(HttpSecurity http) throws Exception
    {
        super.configure(http);
        http
                .authorizeRequests()
                .antMatchers("/customers*").hasRole("USER")
                .antMatchers("/admin/**").hasRole("base_user")
                .anyRequest().permitAll();
    }
}
```
注意：上面的配置并不能成功。<br />配置完上面的然后直接启动应用，结果并不像期望的那样：
```java
java.io.FileNotFoundException: Unable to locate Keycloak configuration file: keycloak.json
```
抛出找不到 keycloak.json文件的异常。Keycloak支持的每个Java适配器都可以通过一个简单的JSON文件进行配置，缺失的就是这个文件。
```json
{
  "realm" : "demo",
  "resource" : "customer-portal",
  "realm-public-key" : "MIGfMA0GCSqGSIb3D...31LwIDAQAB",
  "auth-server-url" : "https://localhost:8443/auth",
  "ssl-required" : "external",
  "use-resource-role-mappings" : false,
  "enable-cors" : true,
  "cors-max-age" : 1000,
  "cors-allowed-methods" : "POST, PUT, DELETE, GET",
  "cors-exposed-headers" : "WWW-Authenticate, My-custom-exposed-Header",
  "bearer-only" : false,
  "enable-basic-auth" : false,
  "expose-token" : true,
  "verify-token-audience" : true,
   "credentials" : {
      "secret" : "234234-234234-234234"
   },

   "connection-pool-size" : 20,
   "socket-timeout-millis": 5000,
   "connection-timeout-millis": 6000,
   "connection-ttl-millis": 500,
   "disable-trust-manager": false,
   "allow-any-hostname" : false,
   "truststore" : "path/to/truststore.jks",
   "truststore-password" : "geheim",
   "client-keystore" : "path/to/client-keystore.jks",
   "client-keystore-password" : "geheim",
   "client-key-password" : "geheim",
   "token-minimum-time-to-live" : 10,
   "min-time-between-jwks-requests" : 10,
   "public-key-cache-ttl": 86400,
   "redirect-rewrite-rules" : {
   "^/wsmaster/api/(.*)$" : "/api/$1"
   }
}
```
上面包含的客户端配置属性都可以在Keycloak控制台进行配置，见下图：<br />![2021-08-03-22-58-16-988856.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628002740407-1c961bb2-d87d-4dba-b2fe-0ba5e4f54975.png#clientId=u25188d7c-81db-4&from=ui&id=u8a89c24d&originHeight=257&originWidth=1080&originalType=binary&ratio=1&size=50983&status=done&style=shadow&taskId=uc3d37352-e63a-4388-8940-e9e08a8af55)<br />配置Keycloak客户端属性<br />也就是说需要的json文件和图中的配置项是对应的。比较人性化的是不需要自行编写这个json文件，Keycloak提供了下载客户端配置的方法，这里只使用了必要的配置项：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628002386561-cefe1c14-3c24-4a19-95cb-054cee64a87b.webp#clientId=u25188d7c-81db-4&from=paste&id=u35f7be88&originHeight=466&originWidth=993&originalType=url&ratio=1&status=done&style=shadow&taskId=ue4d46711-ba80-4e0a-a13b-9ec47f96e0a)<br />可以下载客户端json配置
<a name="yOILf"></a>
### Keycloak适配器的常用属性
在Spring Security集成Keycloak 适配器时需要引入一些额外的配置属性。一般会把它配置到Spring Boot的配置文件中。
<a name="Lw6Oh"></a>
#### realm
Keycloak领域名称，这是一个必须项。
<a name="WC5Tt"></a>
#### resource
应用的client_id，Keycloak服务器上注册的每个客户端都有一个独一无二的标识。这是一个必须项。
<a name="uY5yT"></a>
#### realm-public-key
PEM格式的realm公钥，不建议客户端配置。每次Keycloak Adapter会自动拉取它。
<a name="NgUN1"></a>
#### auth-server-url
Keycloak服务器的基本地址，格式通常是https://host:port/auth，这是一个必须项。
<a name="yw1qB"></a>
#### ssl-required
Keycloak 服务器的通信使用HTTPS的范围，是可选的，有三个选项：

- `external`，默认值，表示外部的请求都必须使用HTTPS。
- `all`，顾名思义，所有的都使用HTTPS。
- `none`，禁用HTTPS。
<a name="qtlGZ"></a>
#### confidential-port
Keycloak服务器的安全端口，默认 8443。
<a name="rGmq3"></a>
#### use-resource-role-mappings
如果设置为`true`，Keycloak Adapter将检查令牌携带的用户角色是否跟资源一致；否则会去查询realm中用户的角色。默认`false`。
<a name="RW43D"></a>
#### public-client
设置为`true`则不需要为客户端配置密码，否则需要配置`keycloak.credentials.secret`。生成`secret`的方法是在Keycloak控制台上修改对应客户端设置选项的访问类型为`confidential`，然后在安装中查看对应配置项。当访问类型不是`confidential`时该值为`false`。
<a name="zDPUV"></a>
#### enable-cors
开启跨域（cors）支持。可选项，默认false。如果设置为`true`就激活了`cors-`开头的配置项，都是常见的跨域配置项。
<a name="Ad3pa"></a>
#### bearer-only
对于服务，这应该设置为true。如果启用，适配器将不会尝试对用户进行身份验证，而只会验证不记名令牌。如果用户请求资源时没有携带`Bearer Token`将会401。这是可选的。默认值为false。
<a name="pS32T"></a>
#### autodetect-bearer-only
如果应用不仅仅是Web应用而且还提供API服务（现在通常是Restful Service），开启了这一配置后Keycloak服务器会通过请求标头相对“智能”地引导未认证的用户到登录页面还是返回401状态。比`bearer-only`更加智能一些。
<a name="it1bC"></a>
#### enable-basic-auth
为适配器开启Basic Authentication认证，如果开启就必须提供`secret`。默认`false`。
<a name="xOFQk"></a>
#### expose-token
JavaScript CORS 请求通过根路径下`/k_query_bearer_token`用来从服务器获取令牌的。
<a name="TkwUc"></a>
#### credentials
当客户端的访问类型(access type)为`Confidential`时，需要配置客户端令牌，目前支持secret和jwt类型。参考`public-client`中的描述。
<a name="lG1Do"></a>
### 引入客户端配置
虽然顺利拿到json文件，但是加载这个json配置却不太顺利，经过摸索需要实现一个`KeycloakConfigResolver`并注入Spring IoC，有下面两种实现方式。
<a name="YcW9b"></a>
#### 复用Spring Boot Adapter配置
直接复用Spring Boot的配置形式，先声明Spring Boot的`KeycloakConfigResolver`实现：
```java
/**
 * 复用spring boot 的方法
 *
 * @return the keycloak config resolver
 */
@Bean
public KeycloakConfigResolver keycloakConfigResolver() {
    return new KeycloakSpringBootConfigResolver();
}
```
然后复用Spring Boot的application.yaml的配置项：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628002386578-98e6da3f-ed11-4865-8aa1-49163d021447.webp#clientId=u25188d7c-81db-4&from=paste&id=u43b01d4c&originHeight=593&originWidth=1067&originalType=url&ratio=1&status=done&style=none&taskId=u7bc2e13c-3411-4d40-9d19-6fd10905205)<br />复用Spring Boot配置项<br />原来的角色资源映射约束失效。
<a name="O6vdO"></a>
#### 自定义实现
也可以自定义写解析，这个时候json形式已经不重要了，可以将json文件的内容存储到任何擅长的地方。
```java
/**
 * 自己写解析
 *
 * @return the keycloak config resolver
 */
@Bean
public KeycloakConfigResolver fileKeycloakConfigResolver() {
    return  new KeycloakConfigResolver() {
        @SneakyThrows
        @Override
        public KeycloakDeployment resolve(HttpFacade.Request request) {
            // json 文件放到resources 文件夹下
            ClassPathResource classPathResource = new ClassPathResource("./keycloak.json");
            AdapterConfig adapterConfig = new ObjectMapper().readValue(classPathResource.getFile(), AdapterConfig.class);

            return KeycloakDeploymentBuilder.build(adapterConfig);
        }
    };
}
```
<a name="rgTgh"></a>
### 角色命名策略
Spring Security会为每个角色添加`ROLE_`前缀，这需要声明`GrantedAuthoritiesMapper`的实现`SimpleAuthorityMapper`来完成这一功能。Keycloak在`KeycloakAuthenticationProvider`中配置该功能：
```java
KeycloakAuthenticationProvider authenticationProvider = keycloakAuthenticationProvider();
authenticationProvider.setGrantedAuthoritiesMapper(new SimpleAuthorityMapper());
```
<a name="TgsD3"></a>
### 完整的配置
applicaiton.yaml:
```yaml
keycloak:
# 声明客户端所在的realm
  realm: fcant.cn
# keycloak授权服务器的地址
  auth-server-url: http://localhost:8011/auth
# 客户端名称
  resource: springboot-client
# 声明这是一个公开的客户端，否则不能在keycloak外部环境使用，会403
  public-client: true
```
这里要结合Keycloak导出的json文件配置。<br />Spring Security配置：
```java
@KeycloakConfiguration
public class SecurityConfig extends KeycloakWebSecurityConfigurerAdapter {
    
    /**
     * 复用spring boot 的方法
     *
     * @return the keycloak config resolver
     */
    @Bean
    public KeycloakConfigResolver keycloakConfigResolver() {
        return new KeycloakSpringBootConfigResolver();
    }
    /**
     * 自己写解析
     *
     * @return the keycloak config resolver
     */
//    @Bean
    public KeycloakConfigResolver fileKeycloakConfigResolver() {
        return request -> {
            // json 文件放到resources 文件夹下
            ClassPathResource classPathResource = new ClassPathResource("./keycloak.json");
            AdapterConfig adapterConfig = null;
            try {
                adapterConfig = new ObjectMapper().readValue(classPathResource.getFile(), 
                        AdapterConfig.class);
            } catch (IOException e) {
                e.printStackTrace();
            }

            return KeycloakDeploymentBuilder.build(adapterConfig);
        };
    }
    /**
     *  配置{@link AuthenticationManager}
     *  这里会引入Keycloak的{@link AuthenticationProvider}实现
     *
     * @param auth the auth
     */
    @Autowired
    public void configureGlobal(AuthenticationManagerBuilder auth) {
        KeycloakAuthenticationProvider authenticationProvider = keycloakAuthenticationProvider();
        authenticationProvider.setGrantedAuthoritiesMapper(new SimpleAuthorityMapper());
        auth.authenticationProvider(authenticationProvider);
    }
    /**
     * 会话身份验证策略
     */
    @Bean
    @Override
    protected SessionAuthenticationStrategy sessionAuthenticationStrategy() {
        return new RegisterSessionAuthenticationStrategy(new SessionRegistryImpl());
    }
    /**
     * 配置 session 监听器 保证单点退出生效
     *
     * @return the servlet listener registration bean
     */
    @Bean
    public ServletListenerRegistrationBean<HttpSessionEventPublisher> httpSessionEventPublisher() {
        return new ServletListenerRegistrationBean<>(new HttpSessionEventPublisher());
    }
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        super.configure(http);
        http
                .authorizeRequests()
                .antMatchers("/customers*").hasRole("USER")
                .antMatchers("/admin/**").hasRole("base_user")
                .anyRequest().permitAll();
    }
}
```
<a name="W0nzG"></a>
## 调用流程
资源客户端springboot-client有一个接口/admin/foo，当未登录调用该接口时会转发到:<br />[http://localhost:8011/auth/realms/fcant.cn/protocol/openid-connect/auth?response_type=code&client_id=springboot-client&redirect_uri=http%3A%2F%2Flocalhost%3A8080%2Fsso%2Flogin&state=ec00d608-5ce7-47a0-acc8-8a20a2bfadfd&login=true&scope=openid](http://localhost:8011/auth/realms/felord.cn/protocol/openid-connect/auth?response_type=code&client_id=springboot-client&redirect_uri=http%3A%2F%2Flocalhost%3A8080%2Fsso%2Flogin&state=ec00d608-5ce7-47a0-acc8-8a20a2bfadfd&login=true&scope=openid)<br />输入正确的用户密码后才能得到期望的结果。<br />典型的authorazation code flow。
<a name="gVv3h"></a>
## 总结
Keycloak整合Spring Security的要点这里需要再梳理一下。在原生情况下，客户端的配置、用户的信息、角色信息都由Keycloak负责；客户端只负责角色和资源的映射关系。
