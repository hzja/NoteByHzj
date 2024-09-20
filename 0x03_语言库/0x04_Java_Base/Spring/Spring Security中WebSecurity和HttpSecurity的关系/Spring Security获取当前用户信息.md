Java Spring Security<br />在某些场景中需要获取当前的用户是谁？如果使用了Spring Secrity作为安全框架可以通过以下手段获取当前用户。
<a name="X0RgJ"></a>
## SecurityContext
无论是有状态的Session模式还是流行的JWT模式都可以通过`SecurityContext`来获取当前的用户：
```java
Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
String currentPrincipalName = authentication.getName();
```
当然这种方式是不够严谨的，如果接口允许匿名访问很可能返回一个匿名用户，而匿名用户并不能直接通过`getName`获取，所以需要优化上面的逻辑为：
```java
Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
if (!(authentication instanceof AnonymousAuthenticationToken)) {
    String currentUserName = authentication.getName();
    return currentUserName;
}else{
    throw RuntimeException("No User")
}
```
其实平常使用这种方式的最多，使用一个抽象的父类控制器来封装获取当前用户的方法。
<a name="mOcxw"></a>
## Principal
java.security.Principal对象也可以获取当前的用户信息，在Spring Security中该对象表现为`Authentication`对象，如果在Spring MVC接口中定义`Principal`对象也可以获取当前用户：
```java
@GetMapping("/currentusername")
public String currentUserName(Principal principal) {
        return principal.getName();
}
```
同理`Authentication`对象也是可以的：
```java
 @GetMapping("/currentusername")
 public String currentUserName(Authentication authentication) {
        return authentication.getName();
 }
```
<a name="BONqv"></a>
## AuthenticationPrincipal
很多时候自定义了用户对象`UserDetails`，可以通过Spring Security 4.0提供的注解`@AuthenticationPrincipal`来获取当前用户的自定义`UserDetails`对象。如果`CustomUser`是`UserDetails`的实现，那么可以：
```java
@GetMapping("/currentusername")
 public String currentUserName(@AuthenticationPrincipal CustomUser customUser) {
   return customUser.getUsername();
}
```
更简单点的话：
```java
@GetMapping("/currentusername")
 public String currentUserName(@AuthenticationPrincipal(expression = "username") String username) {
   return username;
}
```
这需要`CustomUser`包含一个`getUsername`方法。<br />甚至自定义一个注解也是可以的：
```java
@Target({ElementType.PARAMETER, ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@AuthenticationPrincipal
public @interface CurrentUser {}
```
<a name="ufWbx"></a>
## CurrentSecurityContext
Spring Security 5 提供了一个新的注解`@CurrentSecurityContext`来获取当前用户的安全上下文，可以：
```java
@GetMapping("/currentusername")
public String currentUserName(@CurrentSecurityContext(expression = "authentication") 
                              Authentication authentication) {
    return authentication.getName();
}
```
当然还可以通过`expression`参数声明SpEL表达式来获取其它属性，例如获取`Principal`对象：
```java
@GetMapping("/principal")
public String getPrincipal(@CurrentSecurityContext(expression = "authentication.principal") 
  Principal principal) { 
    return principal.getName(); 
}
```
<a name="o39lF"></a>
## HttpServletRequest
据说`HttpServletRequest`的`getUserPrincipal()`方法也可以。
