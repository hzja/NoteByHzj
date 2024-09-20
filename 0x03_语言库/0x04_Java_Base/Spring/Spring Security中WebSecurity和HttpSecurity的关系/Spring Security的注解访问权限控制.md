Java Spring Spring Security<br />**Spring Security**提供基于注解的访问控制。
<a name="E40J6"></a>
## 开启方法注解访问控制
**Spring Security**默认是关闭方法注解的，开启它只需要通过引入`@EnableGlobalMethodSecurity`注解即可:
```java
/**
 * 开启方法安全注解
 *
 */
@EnableGlobalMethodSecurity(prePostEnabled = true,
        securedEnabled = true,
        jsr250Enabled = true)
public class MethodSecurityConfig {
}
```
`@EnableGlobalMethodSecurity` 提供了 `prePostEnabled` 、`securedEnabled` 和 `jsr250Enabled` 三种方式，可以根据需要选择使用这三种的一种或者其中几种。
<a name="YSBoO"></a>
## `@EnableGlobalMethodSecurity`
`@EnableGlobalMethodSecurity`的思维导图梳理相关的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650016153424-d4432807-a98b-4d9c-9f9b-c6b2c65f15a2.png#clientId=u4854b669-cfd6-4&from=paste&id=u13cf2a8f&originHeight=599&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub799ebcd-514f-41f8-a8be-f438454917d&title=)<br />黄色区域是注解访问控制的基本知识点，需要重点掌握，先来看看基本的用法。
<a name="i7SRM"></a>
### `@PreAuthorize`和`@PostAuthorize`
`prePostEnabled`等于`true`时启用。<br />在标记的方法调用之前或者之后，通过**SpEL**表达式来计算是否可以调用方法或者调用后是否可以返回结果。总结了一些常用的表达式样例：

| SpEL表达式 | 说明 |
| --- | --- |
| `principal.username ne 'fcant'` | 当前principal的username不能是fcant |
| `authentication.authorities.contains('ROLE_ADMIN')` | 当前Authentication的authorities包含ROLE_ADMIN |
| `hasRole('ADMIN')` | 当前用户必须有角色ROLE_ADMIN，等同于上面 |
| `hasAnyRole('ADMIN','USER')` | 当前用户角色必须有ROLE_ADMIN或者ROLE_USER |
| `hasAuthority('ROLE_ADMIN')` | 同hasRole |
| `hasAnyAuthority('ROLE_ADMIN','ROLE_USER')` | 同hasAnyRole |
| `#requestParam eq 'fcant'` | 当前请求参数requestParam(可以是对象，这里是字符串示例)值等于fcant |

其它表达式可参考SpEL官方文档。<br />如果用户fcant访问下面这个接口，方法不但不执行还会**403**。
```java
/**
 *  当前用户名不是fcant才能访问 否则403
 * @param req req
 * @return map
 */
@GetMapping("/prepost")
@PreAuthorize("authentication.principal.username ne 'fcant'")
public  Map<String, String> prepost(String req){
	Map<String, String> map = Collections.singletonMap("req", req);
	System.out.println("map = " + map);
	return map;
}
```
如果方法上的注解改为`@PostAuthorize("authentication.principal.username ne 'fcant'")`，控制台会打印<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650016153525-05b85573-a7fb-40c6-89d6-fb67eeafb23a.png#clientId=u4854b669-cfd6-4&from=paste&id=u6807208a&originHeight=201&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufd0f9bd4-e91c-4d34-bf75-78d84e17af7&title=)<br />从日志上可以明显看出方法确实执行了，但是还是给了**403**。
<a name="bXCHy"></a>
### `@PreFilter`和`@PostFilter`
`prePostEnabled`等于`true`时启用。<br />这两个注解可以看做`@PreAuthorize`和`@PostAuthorize`的加强版。它们除了能实现`@PreAuthorize`和`@PostAuthorize`外还具有过滤请求响应数据的能力。限定处理的数据类型有Collection、数组、Map、Stream。举个例子：
```java
// ids =  ["fcant","fcant","jetty"]
@PostMapping("/prefilter")
@PreFilter(value = "hasRole('ADMIN') and filterObject.startsWith('f')",
		   filterTarget = "ids")
public Collection<String> preFilter(@RequestBody Collection<String> ids){
	 // ids = ["fcant"]
	System.out.println("ids = " + ids);
	return ids;
}
```
上面的接口方法有两层含义：

- 当前用户必须持有角色`ROLE_ADMIN`，否则方法不执行。**这一条件不存在的话方法一定会执行。**
- 如果方法执行的话，入参`ids`集合中不包含`f`开头的元素都会被移除，返回值为fcant。

过滤元素的底层是`java.util.Collection#remove(Object)`；另外多个入参需要使用`filterTarget`指定参数名称。<br />`@PostFilter`也很好理解，拿下面的方法来说：
```java
@GetMapping("/postfilter")
@PostFilter("hasRole('ADMIN') and filterObject.startsWith('f')")
public Collection<String> postfilter(){
	List<String> list = new ArrayList<>();
	list.add("fcantcn");
	list.add("fcant");
	list.add("fcant");
	// list = ["fcant"]
	System.out.println("list = " + list);
	return list;
}
```
无论是否满足`@PostFilter`表达式的条件，都会打印`list = ["fcant"]`，条件成立返回list，不成立则返回**403**。<br />这两个注解用来控制请求和相应中集合、流中的数据。
<a name="WjJEn"></a>
### `@Secured`
`securedEnabled`等于`true`时启用。<br />该注解功能要简单得多，默认情况下只能基于角色（默认需要带前缀 `ROLE_`）集合来进行访问控制决策。该注解的机制是只要其声明的角色集合（value）中包含当前用户持有的任一角色就可以访问，也就是用户的角色集合和 `@Secured` 注解的角色集合要存在非空的交集。不支持使用 **SpEL** 表达式进行决策。过于简单不再演示。<br />`@Secured`等同于`hasAnyAuthority`。
<a name="ibZn0"></a>
### JSR-250
jsr250Enabled等于`true`时启用。<br />启用 **JSR-250** 安全控制注解，这属于 **JavaEE** 的安全规范（现为 **jakarta** 项目）。**Spring Security**中使用了**JavaEE** 安全注解中的以下三个：

- `**@DenyAll**` 拒绝所有的访问
- `**@PermitAll**` 同意所有的访问
- `**@RolesAllowed**` 用法和上面的 `@Secured` 一样。
<a name="ahTCU"></a>
## 注解控制的优劣
使用注解的好处就是绑定了接口方法，控制粒度非常细，甚至能做一些数据层面的访问控制。劣势在于它是静态织入Java代码中的，灵活性难以把握。
