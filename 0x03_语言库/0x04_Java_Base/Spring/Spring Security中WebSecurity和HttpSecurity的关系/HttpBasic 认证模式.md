JavaSpring<br />spring security中的一种经典认证模式HttpBasic，在5.x版本之前作为Spring Security默认认证模式，但是在5.x版本中被放弃了，默认的是form login认证模式
<a name="J0lYQ"></a>
## **HttpBasic模式的应用场景**
HttpBasic登录验证模式是Spring Security实现登录验证最简单的一种方式，也可以说是最简陋的一种方式。<br />为什么是最简陋的？这种模式用来糊弄普通用户可以，但是稍微懂点技术的用户分分钟就可以将其破解，因为底层并未做任何的安全的设置，仅仅是将用户名:密码做了简单的base64加密传递给服务端，base64又是一种可逆的算法。<br />因此 HttpBasic 的应用场景非常少，对于不重要的数据，用户比较少但是又想设置一重障碍的时候就可以考虑使用这种
<a name="IonYs"></a>
## **整合Spring Security了解一下**
虽然这种认证模式不太重要，但是还是要了解，对于后面的学习至关重要，下面搭建一个项目演示一下
<a name="R8SuS"></a>
### 1、添加Maven依赖
直接添加Spring Security的依赖，如下：
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-security</artifactId>
</dependency>
```
<a name="f2sgx"></a>
### 2、Spring Security 添加配置
由于这里使用的是Spring Boot 2.x版本，此时的Spring Security 是5.x版本，默认的认证方式是form表单认证，因此需要配置一下HttpBasic认证模式，代码如下：
```java
/**
* @description Spring Security的配置类
*/
@Configuration
public class SecurityConfig extends WebSecurityConfigurerAdapter {
	@Override
	protected void configure(HttpSecurity http) throws Exception {
		http.httpBasic()//开启httpbasic认证
			.and()
			.authorizeRequests()
			.anyRequest()
			.authenticated();//所有请求都需要登录认证才能访问
	}
}
```
启动项目，在项目后台有这样的一串日志打印，冒号后面的就是默认密码。
```
Using generated security password: 00af0f93-7103-4c8a-87a4-23a050a4285c
```
可以通过浏览器进行登录验证，默认的用户名是**user**.（下面的登录框不是开发的，是HttpBasic模式自带的）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658028802730-96fff0ac-ca10-400f-b811-27f6000138c6.png#averageHue=%23fefefd&clientId=u81b6a5c7-29ca-4&from=paste&id=u11834a98&originHeight=302&originWidth=426&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua190a76a-b61f-467a-98e2-3a0adee0c70&title=)<br />当然也可以通过application.yml指定配置用户名密码，配置如下：
```yaml
spring:
    security:
      user:
        name: admin
        password: admin
```
<a name="StMfj"></a>
## **HttpBasic的原理**
整个流程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658028802630-b580d4b7-72c9-4c53-950c-29731f58b4bd.png#averageHue=%23f6f5f3&clientId=u81b6a5c7-29ca-4&from=paste&id=uec93cf11&originHeight=369&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8dc267ef-7001-466c-8bf7-519aafa496e&title=)

1. **首先，HttpBasic模式要求传输的用户名密码使用Base64模式进行加密。如果用户名是 admin ，密码是admin，则将字符串admin:admin使用Base64编码算法加密。加密结果可能是：YWtaW46YWRtaW4=。**
2. **然后，在Http请求中使用Authorization作为一个Header，Basic YWtaW46YWRtaW4=作为Header的值，发送给服务端。（注意这里使用Basic+空格+加密串）**
3. **服务器在收到这样的请求时，到达BasicAuthenticationFilter过滤器，将提取“ Authorization”的Header值，并使用用于验证用户身份的相同算法Base64进行解码。**
4. **解码结果与登录验证的用户名密码匹配，匹配成功则可以继续过滤器后续的访问。**

所以，HttpBasic模式真的是非常简单又简陋的验证模式，Base64的加密算法是可逆的，知道上面的原理，分分钟就破解掉。完全可以使用PostMan工具，发送Http请求进行登录验证。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658028802632-f3ce3ea3-9902-457b-8f5b-c5b7b060a76e.png#averageHue=%23f9f9f9&clientId=u81b6a5c7-29ca-4&from=paste&id=u1d83f77d&originHeight=416&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud7d005fd-69b0-40a3-a654-40c6d32b985&title=)<br />整个流程都在`BasicAuthenticationFilter#doFilterInternal()`这个方法中。
