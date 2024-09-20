Java SpringBoot Actuator 可视化监控
<a name="gKIdE"></a>
### 简介
开启客户端Actuator认证， 到开启SpringBoot Admin认证， 一步一步配置， 每配一步， 检查对应的效果。<br />Note：

- SpringBoot版本：2.1.4
- SpringBoot Admin版本：2.1.5
<a name="Kr2jf"></a>
### 客户端认证：SpringBoot应用开启Actuator认证
<a name="bBBJv"></a>
#### 1、在Maven的pom.xml文件中添加 spring-boot-starter-security 依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-security</artifactId>
</dependency>
```
<a name="UIhya"></a>
#### 2、配置 Spring Security 认证信息
```yaml
spring:
  security:
    user:
      name: user
      password: password
```
<a name="Rbhov"></a>
#### 3、测试客户端认证
此时访问 http://localhost:9000 ， 显示如下 Spring Security 默认的登录页面<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639273426824-742e09fc-f38c-4a49-8ecc-ae0ede234865.webp#averageHue=%23e1e0da&clientId=u5d576d98-acf6-4&from=paste&id=ud8215862&originHeight=332&originWidth=543&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u71d70574-f036-48b4-9d51-7685d5ad8c3&title=)
<a name="BOPws"></a>
#### 4、测试管理端监控信息
访问 http://localhost:8000 ， 发现获取到的数据并不完整， 这是因为客户的应用虽然注册到了管理端， 但是管理端并未获得客户端的认证。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639273426839-482059b4-8b61-4a9c-b9c8-1a2f24e9e177.webp#averageHue=%235a8176&clientId=u5d576d98-acf6-4&from=paste&id=u885777f6&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b7bf4cf-474b-4125-86bc-fe9aedb2b30&title=)<br />在 application.yml 中增加当前实例注册到管理端的认证信息， 主要是metadata下的 user.name 与 user.password ;
```yaml
management:
  endpoints:
    web:
      exposure:
        include: "*"
        exclude: env,beans
  endpoint:        
    health:
      show-details: always # 访问/actuator/health时，显示详细信息，而不是仅仅显示"status": "UP"

spring:
  security:
    user:
      name: user
      password: password
  boot:
    admin:
      client:
        url: http://localhost:8000
        instance:
          name: ReactiveCrud
          metadata: # 这个name与password用于在注册到管理端时，使管理端有权限获取客户端端点数据
            user.name: ${spring.security.user.name}
            user.password: ${spring.security.user.password}
```
再次访问 http://localhost:8000 ， 得到如下信息：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639273426949-5bb729e5-dd0b-45f9-ba06-8d4ea9acad97.webp#averageHue=%23a4e4ca&clientId=u5d576d98-acf6-4&from=paste&id=u97c04fb3&originHeight=1292&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub8037d8d-a846-4208-8db2-1a0225b5958&title=)
<a name="RA2uy"></a>
### 管理端：SpringBoot Admin开启认证
以上， 客户端的Actuator通过 Spring Security 开启认证， 而不是让人随便访问， 同理， 管理端也不应该暴露在公网上。<br />同样， 在Maven的pom.xml文件中添加 spring-boot-starter-security 依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-security</artifactId>
</dependency>
```
配置 Spring Security 认证信息
```yaml
spring:
  security:
    user:
      name: admin
      password: admin
```
添加 Spring Security 认证路由
```java
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.web.authentication.SavedRequestAwareAuthenticationSuccessHandler;
import org.springframework.security.web.csrf.CookieCsrfTokenRepository;

import de.codecentric.boot.admin.server.config.AdminServerProperties;

@Configuration
public class SecuritySecureConfig extends WebSecurityConfigurerAdapter {
    private final String adminContextPath;

    public SecuritySecureConfig(AdminServerProperties adminServerProperties) {
        this.adminContextPath = adminServerProperties.getContextPath();
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        SavedRequestAwareAuthenticationSuccessHandler successHandler = new SavedRequestAwareAuthenticationSuccessHandler();
        successHandler.setTargetUrlParameter("redirectTo");
        successHandler.setDefaultTargetUrl(adminContextPath + "/");

        http.authorizeRequests().antMatchers(adminContextPath + "/assets/**").permitAll()
                .antMatchers(adminContextPath + "/login").permitAll().anyRequest().authenticated().and().formLogin()
                .loginPage(adminContextPath + "/login").successHandler(successHandler).and().logout()
                .logoutUrl(adminContextPath + "/logout").and().httpBasic().and().csrf()
                .csrfTokenRepository(CookieCsrfTokenRepository.withHttpOnlyFalse())
                .ignoringAntMatchers(adminContextPath + "/instances", adminContextPath + "/actuator/**");
    }
}
```
管理端登录 http://localhost:8000<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639273426841-0b8a34bb-6bd4-4a79-b75c-58e0af85a78d.webp#averageHue=%23f9faf6&clientId=u5d576d98-acf6-4&from=paste&id=uaf160eb3&originHeight=475&originWidth=560&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bf4fe36-fad3-4a8e-9eac-4562a836144&title=)<br />输入配置的用户信息后， 登录后发现， 页面是空的， 即没有任何应用注册上来！！这时， 由于管理端开启了认证， 那么客户端要想注册上来， 也必须提供认证信息。。<br />在客户端的 application.yml 中（注意， 是在客户端的配置文件）添加：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639273426961-ce81cd6b-b608-4f56-b65f-90bfea222fb4.webp#averageHue=%23052754&clientId=u5d576d98-acf6-4&from=paste&id=uf6fc5099&originHeight=310&originWidth=486&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bb57343-fc9e-40cd-babd-0064fc72f61&title=)<br />最后， 登录管理端 http://localhost:8000 ， 成功后的信息如下， 注意右上角的用户信息：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639273427418-766d5b2d-88bd-432c-8b6a-3aaaf0ddb2a2.webp#averageHue=%236abdd7&clientId=u5d576d98-acf6-4&from=paste&id=uf2b105a7&originHeight=998&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud9f7924e-6a80-40d5-87cc-5516fd53b53&title=)
<a name="q8aH4"></a>
### 附：客户端 application.yml 完整配置
```yaml
server:
  port: 9000

management:
  endpoints:
    web:
      exposure:
        include: "*"
        exclude: env,beans
  endpoint:        
    health:
      show-details: always

spring:
  security:
    user:
      name: user
      password: password
  boot:
    admin:
      client:
        url: http://localhost:8000
        username: admin # 这个username与password用于注册到管理端，使其通过认证
        password: admin
        instance:
          name: ReactiveCrud
          metadata: # 这个name与password用于在注册到管理端时，使管理端有权限获取客户端端点数据
            user.name: ${spring.security.user.name}
            user.password: ${spring.security.user.password}

info:
  app:
    name: chapter-mogo
```
<a name="Qs770"></a>
### 附：管理端 application.yml 完整配置
```yaml
server:
  port: 8000

spring:
  security:
    user:
      name: admin
      password: admin
```
