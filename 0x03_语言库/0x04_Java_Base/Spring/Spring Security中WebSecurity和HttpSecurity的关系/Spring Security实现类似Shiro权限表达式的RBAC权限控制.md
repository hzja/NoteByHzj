Java Spring Spring Security
<a name="EMqlo"></a>
## 资源权限表达式
先来解释一下什么叫**资源权限表达式**。权限控制的核心就是清晰地表达出特定资源的某种操作，一个格式良好好的权限声明可以清晰表达出用户对该资源拥有的操作权限。<br />通常一个资源在系统中的标识是唯一的，比如**User**用来标识用户，**ORDER**标识订单。不管什么资源大都可以归纳出以下这几种操作<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650370394123-f849c522-2cd8-4895-9f26-ac191e50d7e5.png#clientId=u4fd207a8-03ba-4&from=paste&id=ub606f446&originHeight=468&originWidth=740&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u72fd9750-e866-4439-858e-5f53e418259&title=)<br />在 shiro权限声明通常对上面的这种资源操作关系用冒号分隔的方式进行表示。例如读取用户信息的操作表示为USER:READ，甚至还可以更加细一些，用USER:READ:123表示读取**ID**为123的用户权限。<br />资源操作定义好了，再把它和角色关联起来不就是基于RBAC的权限资源控制了吗？就像下面这样：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650370394141-23f716a8-a14f-4e2d-b1f4-6e5f7446d208.png#clientId=u4fd207a8-03ba-4&from=paste&id=u5b8acb35&originHeight=381&originWidth=520&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u415cfc32-2311-451d-8046-3087baf7f4b&title=)<br />这样资源和角色的关系可以进行CRUD操作进行动态绑定。
<a name="SWcYQ"></a>
## Spring Security中的实现
资源权限表达式的动态权限控制在Spring Security也是可以实现的。首先开启方法级别的注解安全控制。
```java
/**
 * 开启方法安全注解
 */
@EnableGlobalMethodSecurity(prePostEnabled = true,
        securedEnabled = true,
        jsr250Enabled = true)
public class MethodSecurityConfig {
    
}
```
<a name="D6eKW"></a>
### `MethodSecurityExpressionHandler`
`MethodSecurityExpressionHandler` 提供了一个对方法进行安全访问的门面扩展。它的实现类`DefaultMethodSecurityExpressionHandler`更是提供了针对方法的一系列扩展接口，这里我总结了一下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650370394072-0d785323-8c8f-49ee-a559-5520a864bdea.png#clientId=u4fd207a8-03ba-4&from=paste&id=ud8c8be31&originHeight=394&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc8673ab5-f457-4f0d-b3fd-92bf1ae1ae8&title=)<br />这里的`PermissionEvaluator`正好可以满足需要。
<a name="Eeamt"></a>
#### `PermissionEvaluator`
`PermissionEvaluator` 接口抽象了对一个用户是否有权限访问一个特定的领域对象的评估过程。
```java
public interface PermissionEvaluator extends AopInfrastructureBean {

	boolean hasPermission(Authentication authentication, 
						  Object targetDomainObject, Object permission);

	boolean hasPermission(Authentication authentication, 
						  Serializable targetId, String targetType, Object permission);	
}
```
这两个方法仅仅参数列表不同，这些参数的含义为：

- authentication 当前用户的认证信息，持有当前用户的角色权限。
- targetDomainObject 用户想要访问的目标领域对象，例如上面的USER。
- permission 这个当前方法设定的目标领域对象的权限，例如上面的READ。
- targetId 这种是对上面targetDomainObject 的具体化，比如ID为123的USER，我觉得还可以搞成租户什么的。
- targetType 是为了配合targetId 。

第一个方法是用来实现USER:READ的；第二个方法是用来实现USER:READ:123的。
<a name="Zx2E3"></a>
### 思路以及实现
`targetDomainObject:permission`不就是USER:READ的抽象吗？只要找出USER:READ对应的角色集合，和当前用户持有的角色进行比对，它们存在交集就证明用户有权限访问。借着这个思路实现了一个`PermissionEvaluator`:
```java
/**
 * 资源权限评估
 */
public class ResourcePermissionEvaluator implements PermissionEvaluator {
    private final BiFunction<String, String, Collection<? extends GrantedAuthority>> permissionFunction;

    public ResourcePermissionEvaluator(BiFunction<String, String, Collection<? extends GrantedAuthority>> permissionFunction) {
        this.permissionFunction = permissionFunction;
    }

    @Override
    public boolean hasPermission(Authentication authentication, Object targetDomainObject, Object permission) {
        //查询方法标注对应的角色
        Collection<? extends GrantedAuthority> resourceAuthorities = permissionFunction.apply((String) targetDomainObject, (String) permission);
        // 用户对应的角色
        Collection<? extends GrantedAuthority> userAuthorities = authentication.getAuthorities();
         // 对比 true 就能访问  false 就不能访问
        return userAuthorities.stream().anyMatch(resourceAuthorities::contains);
    }

    @Override
    public boolean hasPermission(Authentication authentication, Serializable targetId, String targetType, Object permission) {
        //todo
        System.out.println("targetId = " + targetId);
        return true;
    }
}
```
第二个方法没有实现，因为两个差不多，第二个可以想想具体的使用场景。
<a name="YTTk6"></a>
### 配置和使用
`PermissionEvaluator` 需要注入到**Spring IoC**，并且**Spring IoC**只能有一个该类型的**Bean**：
```java
@Bean
PermissionEvaluator resourcePermissionEvaluator() {
	return new ResourcePermissionEvaluator((targetDomainObject, permission) -> {
		//TODO 这里形式其实可以不固定
		String key = targetDomainObject + ":" + permission;
		//TODO  查询 key 和  authority 的关联关系
		//  模拟 permission 关联角色   根据key 去查 grantedAuthorities
		Set<SimpleGrantedAuthority> grantedAuthorities = new HashSet<>();
		grantedAuthorities.add(new SimpleGrantedAuthority("ROLE_ADMIN"));
		return "USER:READ".equals(key) ? grantedAuthorities : new HashSet<>();
	});
}
```
接下来写个接口，用`@PreAuthorize`注解标记，然后直接用`hasPermission('USER','READ')`来静态绑定该接口的访问权限表达式：
```java
@GetMapping("/postfilter")
@PreAuthorize("hasPermission('USER','READ')")
public Collection<String> postfilter(){
	List<String> list = new ArrayList<>();
	list.add("Fcant");
	list.add("Hello");
	list.add("World");
	return list;
}
```
然后定义一个用户：
```java
@Bean
UserDetailsService users() {
	UserDetails user = User.builder()
			.username("Fcant")
			.password("123456")
  .passwordEncoder(PasswordEncoderFactories.createDelegatingPasswordEncoder()::encode)
			.roles("USER")
			.authorities("ROLE_ADMIN","ROLE_USER")
			.build();
	return new InMemoryUserDetailsManager(user);
}
```
接下来肯定是正常能够访问接口的。当改变了`@PreAuthorize`中表达式的值或者移除了用户的`ROLE_ADMIN`权限，再或者`USER:READ`关联到了其它角色等等，都会返回403。<br />可以看看注解改成这样会是什么效果：
```java
@PreAuthorize("hasPermission('USER','READ') or hasRole('ADMIN')")
```
这样呢？
```java
@PreAuthorize("hasPermission('1234','USER','READ')")
```
或者让targetId动态化：
```java
@PreAuthorize("hasPermission(#id,'USER','READ')")
public Collection<String> postfilter(String id){

}
```
