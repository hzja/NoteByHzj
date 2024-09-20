JavaSpringSecurity
<a name="EYeSJ"></a>
## 1、什么是Spring Security？核心功能？
Spring Security是一个基于Spring框架的安全框架，提供了完整的安全解决方案，包括认证、授权、攻击防护等功能。<br />其核心功能包括：

- **认证**：提供了多种认证方式，如表单认证、HTTP Basic认证、OAuth2认证等，可以与多种身份验证机制集成。
- **授权**：提供了多种授权方式，如角色授权、基于表达式的授权等，可以对应用程序中的不同资源进行授权。
- **攻击防护**：提供了多种防护机制，如跨站点请求伪造（CSRF）防护、注入攻击防护等。
- **会话管理**：提供了会话管理机制，如令牌管理、并发控制等。
- **监视与管理**：提供了监视与管理机制，如访问日志记录、审计等。

Spring Security通过配置安全规则和过滤器链来实现以上功能，可以轻松地为Spring应用程序提供安全性和保护机制。
<a name="l8CV4"></a>
## 2、Spring Security的原理?
Spring Security是一个基于Spring框架的安全性认证和授权框架，它提供了全面的安全性解决方案，可以保护Web应用程序中的所有关键部分。<br />Spring Security的核心原理是拦截器（Filter）。Spring Security会在Web应用程序的过滤器链中添加一组自定义的过滤器，这些过滤器可以实现身份验证和授权功能。当用户请求资源时，Spring Security会拦截请求，并使用配置的身份验证机制来验证用户身份。如果身份验证成功，Spring Security会授权用户访问所请求的资源。<br />![Fcant_2023-08-29_09-30-23.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693276175077-640dc45c-0bd9-4cd4-a012-d6329ef3ad8e.png#averageHue=%23f5e2d9&clientId=u2bd35289-8c05-4&from=ui&id=oDqmv&originHeight=1331&originWidth=1947&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=7788388&status=done&style=none&taskId=uec0106ce-f888-4595-b38f-6e70f59180d&title=)<br />Spring Security的具体工作原理如下：

1. 用户请求Web应用程序的受保护资源。
2. Spring Security拦截请求，并尝试获取用户的身份验证信息。
3. 如果用户没有经过身份验证，Spring Security将向用户显示一个登录页面，并要求用户提供有效的凭据（用户名和密码）。
4. 一旦用户提供了有效的凭据，Spring Security将验证这些凭据，并创建一个已认证的安全上下文（SecurityContext）对象。
5. 安全上下文对象包含已认证的用户信息，包括用户名、角色和授权信息。
6. 在接下来的请求中，Spring Security将使用已经认证的安全上下文对象来判断用户是否有权访问受保护的资源。
7. 如果用户有权访问资源，Spring Security将允许用户访问资源，否则将返回一个错误信息。
<a name="sVIUu"></a>
## 3、有哪些控制请求访问权限的方法？
在Spring Security中，可以使用以下方法来控制请求访问权限：

- `permitAll()`：允许所有用户访问该请求，不需要进行任何身份验证。
- `denyAll()`：拒绝所有用户访问该请求。
- `anonymous()`：允许匿名用户访问该请求。
- `authenticated()`：要求用户进行身份验证，但是不要求用户具有任何特定的角色。
- `hasRole(String role)`：要求用户具有特定的角色才能访问该请求。
- `hasAnyRole(String... roles)`：要求用户具有多个角色中的至少一个角色才能访问该请求。
- `hasAuthority(String authority)`：要求用户具有特定的权限才能访问该请求。
- `hasAnyAuthority(String... authorities)`：要求用户具有多个权限中的至少一个权限才能访问该请求。

可以将这些方法应用于Spring Security的配置类或者在Spring Security注解中使用。
<a name="ornyg"></a>
## 4、`hasRole` 和 `hasAuthority` 有区别吗？
在Spring Security中，`hasRole`和`hasAuthority`都可以用来控制用户的访问权限，但它们有一些细微的差别。<br />`hasRole`方法是基于角色进行访问控制的。它检查用户是否有指定的角色，并且这些角色以"ROLE_"前缀作为前缀（例如"ROLE_ADMIN"）。<br />`hasAuthority`方法是基于权限进行访问控制的。它检查用户是否有指定的权限，并且这些权限没有前缀。<br />因此，使用`hasRole`方法需要在用户的角色名称前添加"ROLE_"前缀，而使用`hasAuthority`方法不需要这样做。<br />例如，假设用户有一个角色为"ADMIN"和一个权限为"VIEW_REPORTS"，可以使用以下方式控制用户对页面的访问权限：
```java
.antMatchers("/admin/**").hasRole("ADMIN")
.antMatchers("/reports/**").hasAuthority("VIEW_REPORTS")
```
在这个例子中，只有具有"ROLE_ADMIN"角色的用户才能访问/admin/路径下的页面，而具有"VIEW_REPORTS"权限的用户才能访问/reports/路径下的页面。
<a name="tgLSt"></a>
## 5、如何对密码进行加密？
在 Spring Security 中对密码进行加密通常使用的是密码编码器（PasswordEncoder）。`PasswordEncoder` 的作用是将明文密码加密成密文密码，以便于存储和校验。Spring Security 提供了多种常见的密码编码器，例如 `BCryptPasswordEncoder`、`SCryptPasswordEncoder`、`StandardPasswordEncoder` 等。<br />以 `BCryptPasswordEncoder` 为例，使用步骤如下：

1. 在 pom.xml 文件中添加 `BCryptPasswordEncoder` 的依赖：
```xml
<dependency>
  <groupId>org.springframework.security</groupId>
  <artifactId>spring-security-crypto</artifactId>
  <version>5.6.1</version>
</dependency>
```

2. 在 Spring 配置文件中注入 `BCryptPasswordEncoder`：
```java
@Configuration
public class SecurityConfig extends WebSecurityConfigurerAdapter {

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    // ...
}
```

3. 在使用密码的地方调用 `passwordEncoder.encode()` 方法对密码进行加密，例如注册时对密码进行加密：
```java
@Service
public class UserServiceImpl implements UserService {

    @Autowired
    private PasswordEncoder passwordEncoder;

    @Override
    public User register(User user) {
        String encodedPassword = passwordEncoder.encode(user.getPassword());
        user.setPassword(encodedPassword);
        // ...
        return user;
    }

    // ...
}
```
以上就是使用 `BCryptPasswordEncoder` 对密码进行加密的步骤。使用其他密码编码器的步骤类似，只需将 `BCryptPasswordEncoder` 替换为相应的密码编码器即可。
<a name="ybscL"></a>
## 6、Spring Security基于用户名和密码的认证模式流程？
请求的用户名密码可以通过表单登录，基础认证，数字认证三种方式从`HttpServletRequest`中获得，用于认证的数据源策略有内存，数据库，ldap，自定义等。<br />拦截未授权的请求，重定向到登录页面的过程：<br />当用户访问需要授权的资源时，Spring Security会检查用户是否已经认证（即是否已登录），如果没有登录则会重定向到登录页面。<br />重定向到登录页面时，用户需要输入用户名和密码进行认证。<br />表单登录的过程：

1. 用户在登录页面输入用户名和密码，提交表单。
2. Spring Security的`UsernamePasswordAuthenticationFilter`拦截表单提交的请求，并将用户名和密码封装成一个Authentication对象。
3. `AuthenticationManager`接收到`Authentication`对象后，会根据用户名和密码查询用户信息，并将用户信息封装成一个UserDetails对象。
4. 如果查询到用户信息，则将UserDetails对象封装成一个已认证的`Authentication`对象并返回，如果查询不到用户信息，则抛出相应的异常。
5. 认证成功后，用户会被重定向到之前访问的资源。如果之前访问的资源需要特定的角色或权限才能访问，则还需要进行授权的过程。

Spring Security的认证流程大致可以分为两个过程，首先是用户登录认证的过程，然后是用户访问受保护资源时的授权过程。在认证过程中，用户需要提供用户名和密码，Spring Security通过`UsernamePasswordAuthenticationFilter`将用户名和密码封装成`Authentication`对象，并交由`AuthenticationManager`进行认证。<br />如果认证成功，则认证结果会存储在`SecurityContextHolder`中。在授权过程中，Spring Security会检查用户是否有访问受保护资源的权限，如果没有则会重定向到登录页面进行认证。<br />拦截未授权的请求，重定向到登录页面<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693272103840-ba85041a-99ce-46d7-97cc-09c1eb09c6ca.png#averageHue=%233e2d25&clientId=uc46c63ea-e945-4&from=paste&id=qqnsi&originHeight=479&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub0102bb1-ef27-4acb-9256-294a818e80f&title=)<br />表单登录的过程，进行账号密码认证<br />![Fcant_2023-08-29_09-27-50.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693274384353-68defefc-4987-4ea1-9778-465ab9de6ba5.png#averageHue=%23bb754e&clientId=uc46c63ea-e945-4&from=ui&id=FNvkG&originHeight=1592&originWidth=1702&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=8143601&status=done&style=none&taskId=u5b8c3eb1-2b29-4ee7-945e-608aaf95654&title=)
