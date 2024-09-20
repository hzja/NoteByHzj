Java Spring Security
<a name="yu1Eg"></a>
## Spring Security 测试环境
要想在单元测试中使用Spring Security，需要在Spring Boot项目中集成：
```xml
<dependency>
  <groupId>org.springframework.security</groupId>
  <artifactId>spring-security-test</artifactId>
  <scope>test</scope>
</dependency>
```
这样测试的上下文配置就能和Spring Security结合起来了。
<a name="JI991"></a>
## Spring Security 测试
所有的测试都是在Spring Boot Test下进行的，也就是`@SpringBootTest`注解的支持下。
<a name="O8HnE"></a>
### `@WithMockUser`
`@WithMockUser`注解可以在Spring Security安全上下文中模拟一个默认名称为user,默认密码为password，默认角色为USER的用户。当测试方法使用了该注解后，就能通过：
```java
Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
```
获取该模拟用户的信息，也就“假装”当前登录了用户user。当然也可以根据需要来自定义用户名、密码、角色：
```java
@SneakyThrows
@Test
@WithMockUser(username = "fcant",password = "fcant.cn",roles = {"ADMIN"})
void updatePassword() {

    mockMvc.perform(post("/user/update/password")
                    .contentType(MediaType.APPLICATION_JSON)
                    .content("{\n" +
                             "  \"newPassword\": \"12345\",\n" +
                             "  \"oldPassword\": \"12345\"\n" +
                             "}"))
        .andExpect(ResultMatcher.matchAll(status().isOk()))
        .andDo(print());
}
```
当然可以将`@WithMockUser`标记到整个测试类上，这样每个测试都将使用指定该用户。
<a name="NlopL"></a>
### `@WithAnonymousUser`
`@WithAnonymousUser`是用来模拟一种特殊的用户，也被叫做匿名用户。如果有测试匿名用户的需要，可以直接使用该注解。其实等同于`@WithMockUser(roles = {"ANONYMOUS"})`，也等同于`@WithMockUser(authorities = {"ROLE_ANONYMOUS"})`。
<a name="UHmON"></a>
### `@WithUserDetails`
虽然`@WithMockUser`是一种非常方便的方式，但可能并非在所有情况下都凑效。有时候魔改了一些东西使得安全上下文的验证机制发生了改变，比如定制了UserDetails，这一类注解就不好用了。但是通过`UserDetailsService`加载的用户往往还是可靠的。于是`@WithUserDetails`就派上了用场。
```java
@SneakyThrows
@Test
@WithUserDetails("fcant")
void updatePassword() {

    mockMvc.perform(post("/user/update/password")
            .contentType(MediaType.APPLICATION_JSON)
            .content("{\n" +
                    "  \"newPassword\": \"12345\",\n" +
                    "  \"oldPassword\": \"12345\"\n" +
                    "}"))
            .andExpect(ResultMatcher.matchAll(status().isOk()))
            .andDo(print());
}
```
当执行单元测试时，将通过`UserDetailsService`的`loadUserByUsername`方法查找用户名为fcant的用户并加载到安全上下文中。
<a name="wvR4S"></a>
### 自定义注解
其实还可以模拟@WithMockUser
```java
@Target({ ElementType.METHOD, ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
@WithSecurityContext(factory = WithMockUserSecurityContextFactory.class)
public @interface WithMockUser {

   String value() default "user";

   String username() default "";

   String[] roles() default { "USER" };
 
   String[] authorities() default {};
 
   String password() default "password";
 
   @AliasFor(annotation = WithSecurityContext.class)
   TestExecutionEvent setupBefore() default TestExecutionEvent.TEST_METHOD;

}
```
关键就在于`@WithSecurityContext`注解，只需要实现factory就行了，也就是:
```java
public interface WithSecurityContextFactory<A extends Annotation> {
 
   SecurityContext createSecurityContext(A annotation);
}
```
这里如法炮制就行。
<a name="n7ajh"></a>
## 总结
当项目中集成了Spring Security时如何单元测试，可以使用提供的模拟用户的注解，也可以模拟加载用户，甚至可以根据自己的需要来定制化。其实如果使用了JWT的话，可以在Spring MVC Mock测试中加入对应的请求头或者参数，也能顺利进行。
