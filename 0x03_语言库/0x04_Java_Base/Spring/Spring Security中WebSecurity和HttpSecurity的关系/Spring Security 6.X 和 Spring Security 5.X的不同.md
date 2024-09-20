项目使用了SpringBoot3 ，因此 SpringSecurity也相应进行了升级 版本由5.4.5升级到了6.1.5 写法上发生了很大的变化，最显著的变化之一就是对 `WebSecurityConfigurerAdapter` 类的使用方式的改变。这个类在 Spring Security 中被广泛用于自定义安全配置。以下是主要的差异和写法上的变化：

- 废弃 `WebSecurityConfigurerAdapter`：
   - 在 Spring Security 5.x 版本中，`WebSecurityConfigurerAdapter` 是实现安全配置的常用方法。用户通过继承这个类，并覆盖其方法来自定义安全配置。
   - 到了 Spring Security 6.x，`WebSecurityConfigurerAdapter` 被标记为过时（deprecated），意味着它可能在未来的版本中被移除。这一变化是为了推动使用更现代的配置方法，即使用组件式配置。
- 使用组件式配置：
   - 在 Spring Security 6.x 中，推荐使用组件式配置。这意味着可以创建一个配置类，该类不再需要继承 `WebSecurityConfigurerAdapter`。
   - 可以直接定义一个或多个 `SecurityFilterChain` bean 来配置安全规则。这种方式更加灵活，并且与 Spring Framework 的整体风格更加一致。  
<a name="gsiZf"></a>
## 示例代码变化
<a name="PBbft"></a>
### 在 Spring Security 5.x 使用 `WebSecurityConfigurerAdapter` 的配置框架：
```java
@Configuration
@EnableWebSecurity
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
        .authorizeRequests()
        .anyRequest().authenticated()
        .and()
        .formLogin()
        .and()
        .httpBasic();
    }
}
```
<a name="kn1t3"></a>
### 在 Spring Security 6.x 使用组件式配置变成了这个样子：
```java
@Configuration
public class SecurityConfiguration {
    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
        .authorizeRequests()
        .anyRequest().authenticated()
        .and()
        .formLogin()
        .and()
        .httpBasic();
        return http.build();
    }
}
```
<a name="rXdrx"></a>
## 项目上具体的写法
<a name="valnB"></a>
### 在 Spring Security 5.4.5 使用 `WebSecurityConfigurerAdapter` 的配置：
```java
@Configuration
@Component
@EnableWebSecurity
@AllArgsConstructor
@EnableGlobalMethodSecurity(prePostEnabled = true)
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {

    @Value("${auth.whitelist:/login}")
    private final String[] URL_WHITELIST;
    private final AuthenticationFailureHandler loginFailureHandler;

    private final UserLoginSuccessHandler loginSuccessHandler;
    private final UserLogoutSuccessHandler logoutSuccessHandler;
    private final UserDetailsService userDetailsService;
    private final AccessDeniedHandler authAccessDeniedHandler;
    private final AuthenticationEntryPoint loginAuthenticationEntryPoint;

    @Bean
    BCryptPasswordEncoder bCryptPasswordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    public UserAuthenticationProvider myAuthenticationProvider() {
        UserAuthenticationProvider userAuthenticationProvider = new UserAuthenticationProvider(userDetailsService);
        return userAuthenticationProvider;
    }


    @Bean
    JwtAuthenticationFilter jwtAuthenticationFilter() throws Exception {
        JwtAuthenticationFilter jwtAuthenticationFilter2 = new JwtAuthenticationFilter(authenticationManager());
        return jwtAuthenticationFilter2;
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {

        http
        .cors()
        .and()
        .csrf().disable()
        .formLogin()
        .loginProcessingUrl("/login")
        .usernameParameter("userName")
        .passwordParameter("password")
        .successHandler(loginSuccessHandler)
        .failureHandler(loginFailureHandler)

        .and()
        .logout()
        .logoutSuccessHandler(logoutSuccessHandler)

        //禁用session
        .and()
        .sessionManagement()
        .sessionCreationPolicy(SessionCreationPolicy.STATELESS)

        //配置拦截规则
        .and()
        .authorizeRequests()
        .antMatchers(URL_WHITELIST).permitAll()

        .anyRequest().authenticated()

        //异常处理器
        .and()
        .exceptionHandling()
        .authenticationEntryPoint(loginAuthenticationEntryPoint)
        .accessDeniedHandler(authAccessDeniedHandler)

        .and()
        .addFilterAfter(jwtAuthenticationFilter(), ExceptionTranslationFilter.class);

    }
}
```
<a name="Rrphj"></a>
### 使用SpringSecurity6.1.5实现相同逻辑的配置变成了如下形式：
```java
@Configuration
@Component
@EnableWebSecurity
@RequiredArgsConstructor
public class SecurityConfiguration {

    @Value("${auth.whitelist:/login}")
    private final String[] URL_WHITELIST;

    private final CustomerUserDetailsService customUserDetailsService;
    private final CustomLoginSuccessHandler loginSuccessHandler;
    private final CustomLoginFailureHandler loginFailureHandler;
    private final AccessDeniedHandler authAccessDeniedHandler;
    private final AuthenticationEntryPoint loginAuthenticationEntryPoint;
    @Bean
    BCryptPasswordEncoder bCryptPasswordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    public JwtAuthenticationFilter jwtAuthenticationFilter() {
        return new JwtAuthenticationFilter(); // 实例化您的 JWT 过滤器
    }


    @Bean
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return new ProviderManager(new DaoAuthenticationProvider() {{
            setUserDetailsService(customUserDetailsService);
            setPasswordEncoder(bCryptPasswordEncoder());
        }});
    }


    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
        .csrf(csrf -> csrf.disable())
        .authorizeRequests(authz -> authz
                           .requestMatchers(URL_WHITELIST).permitAll() // 允许访问无需认证的路径
                           .anyRequest().authenticated()
                          )
        .formLogin(form -> form.
                   loginProcessingUrl("/login")
                   .usernameParameter("username")
                   .passwordParameter("password")
                   .successHandler(loginSuccessHandler)
                   .failureHandler(loginFailureHandler)
                  )
        .logout(Customizer.withDefaults())
        .sessionManagement(session -> session.sessionCreationPolicy(SessionCreationPolicy.STATELESS))
        .userDetailsService(customUserDetailsService)
        .exceptionHandling(exceptionHandle -> exceptionHandle
                           .authenticationEntryPoint(loginAuthenticationEntryPoint)
                           .accessDeniedHandler(authAccessDeniedHandler)
                          )
        .addFilterAfter(jwtAuthenticationFilter(), ExceptionTranslationFilter.class)
        .httpBasic(Customizer.withDefaults());

        return http.build();
    }
}
```
总之，从 Spring Security 5.x 迁移到 6.x，主要的改变是从继承 `WebSecurityConfigurerAdapter` 转向定义 `SecurityFilterChain` bean 的方式来配置安全性。这种变化旨在使配置更加模块化和灵活，并更好地符合 Spring 框架的整体设计哲学。
