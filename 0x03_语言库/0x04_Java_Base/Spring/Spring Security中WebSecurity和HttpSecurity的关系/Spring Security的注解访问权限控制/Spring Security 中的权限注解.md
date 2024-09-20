JavaSpringSpring Security
<a name="FOfBX"></a>
## 1、具体用法
先来看看 Spring Security 权限注解的具体用法，如下：
```java
@PreAuthorize("@ss.hasPermi('tienchin:channel:query')")
@GetMapping("/list")
public TableDataInfo getChannelList() {
    startPage();
    List<Channel> list = channelService.list();
    return getDataTable(list);
}
```
类似于上面这样，意思就是说，当前用户需要具备 `tienchin:channel:query` 权限，才能执行当前的接口方法。<br />那么要搞明白 `@PreAuthorize` 注解的原理，得从两个方面入手：

1. 首先明白 Spring 中提供的 SpEL。
2. 其次搞明白 Spring Security 中对方法注解的处理规则。

一个一个来看。
<a name="lCTdH"></a>
## 2、SpEL
Spring Expression Language（简称 SpEL）是一个支持查询和操作运行时对象导航图功能的强大的表达式语言。它的语法类似于传统 EL，但提供额外的功能，最出色的就是函数调用和简单字符串的模板函数。<br />SpEL 给 Spring 社区提供一种简单而高效的表达式语言，一种可贯穿整个 Spring 产品组的语言。这种语言的特性基于 Spring 产品的需求而设计，这是它出现的一大特色。<br />在离不开 Spring 框架的同时，其实也已经离不开 SpEL 了，因为它太好用、太强大了，SpEL 在整个 Spring 家族中也处于一个非常重要的位置。但是很多时候，对它的只了解一个大概，其实如果系统的学习过 SpEL，那么上面 Spring Security 那个注解其实很好理解。<br />先通过一个简单的例子来和大家捋一捋 SpEL。<br />为了省事，创建一个 Spring Boot 工程来和大家演示，创建的时候不用加任何额外的依赖，就最最基础的依赖即可。<br />代码如下：
```java
String expressionStr = "1 + 2";
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression(expressionStr);
```
expressionStr 是自定义的一个表达式字符串，这个字符串通过一个 ExpressionParser 对象将之解析为一个 Expression，接下来就可以执行这个 exp 了。<br />执行的时候有两种方式，对于上面这种不带任何额外变量的，可以直接执行，直接执行的方式如下：
```java
Object value = exp.getValue();
System.out.println(value.toString());
```
这个打印结果为 3。<br />Java中执行一个字符串表达式怎么办，js 中有 eval 函数很方便，Java 中也有 SpEL，一样也很方便。<br />不过很多时候，要执行的表达式可能比较复杂，这时候上面这种调用方式就不太够用了。<br />此时可以为要调用的表达式设置一个上下文环境，这个时候就会用到 `EvaluationContext` 或者它的子类，如下：
```java
StandardEvaluationContext context = new StandardEvaluationContext();
System.out.println(exp.getValue(context));
```
当然上面这个表达式不需要设置上下文环境，举一个需要设置上下文环境的例子。<br />例如现在有一个 User 类，如下：
```java
public class User {
    private Integer id;
    private String username;
    private String address;
    //省略 getter/setter
}
```
现在表达式是这样：
```java
String expression = "#user.username";
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression(expression);
StandardEvaluationContext ctx = new StandardEvaluationContext();
User user = new User();
user.setAddress("广州");
user.setUsername("javaboy");
user.setId(99);
ctx.setVariable("user", user);
String value = exp.getValue(ctx, String.class);
System.out.println("value = " + value);
```
这个表达式就表示获取 user 对象的 username 属性。将来创建一个 user 对象，放到 `StandardEvaluationContext` 中，并基于此对象执行表达式，就可以打印出来想要的结果。<br />如果将 user 对象设置为 rootObject，那么表达式中就不需要 user 了，如下：
```java
String expression = "username";
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression(expression);
StandardEvaluationContext ctx = new StandardEvaluationContext();
User user = new User();
user.setAddress("广州");
user.setUsername("javaboy");
user.setId(99);
ctx.setRootObject(user);
String value = exp.getValue(ctx, String.class);
System.out.println("value = " + value);
```
表达式就一个 username 字符串，将来执行的时候，会自动从 user 中找到 username 的值并返回。<br />当然表达式也可以是方法，例如在 User 类中添加如下两个方法：
```java
public String sayHello(Integer age) {
    return "hello " + username + ";age=" + age;
}
public String sayHello() {
    return "hello " + username;
}
```
就可以通过表达式调用这两个方法，如下：<br />调用有参的 sayHello：
```java
String expression = "sayHello(99)";
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression(expression);
StandardEvaluationContext ctx = new StandardEvaluationContext();
User user = new User();
user.setAddress("广州");
user.setUsername("javaboy");
user.setId(99);
ctx.setRootObject(user);
String value = exp.getValue(ctx, String.class);
System.out.println("value = " + value);
```
就直接写方法名然后执行就行了。<br />调用无参的 sayHello：
```java
String expression = "sayHello";
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression(expression);
StandardEvaluationContext ctx = new StandardEvaluationContext();
User user = new User();
user.setAddress("广州");
user.setUsername("javaboy");
user.setId(99);
ctx.setRootObject(user);
String value = exp.getValue(ctx, String.class);
System.out.println("value = " + value);
```
这些就都好懂了。<br />**甚至，表达式也可以涉及到 Spring 中的一个 Bean，例如向 Spring 中注册如下 Bean：**
```java
@Service("us")
public class UserService {
    public String sayHello(String name) {
        return "hello " + name;
    }
}
```
然后通过 SpEL 表达式来调用这个名为 us 的 bean 中的 sayHello 方法，如下：
```java
@Autowired
BeanFactory beanFactory;
@Test
void contextLoads() {
    String expression = "@us.sayHello('javaboy')";
    ExpressionParser parser = new SpelExpressionParser();
    Expression exp = parser.parseExpression(expression);
    StandardEvaluationContext ctx = new StandardEvaluationContext();
    ctx.setBeanResolver(new BeanFactoryResolver(beanFactory));
    String value = exp.getValue(ctx, String.class);
    System.out.println("value = " + value);
}
```
给配置的上下文环境设置一个 bean 解析器，这个 bean 解析器会自动跟进名字从 Spring 容器中找打响应的 bean 并执行对应的方法。<br />当然，关于 SpEL 的玩法还有很多，就不一一列举了。这里主要是方便大家理解 `@PreAuthorize` 注解的原理。
<a name="uWBJJ"></a>
## 3、`@PreAuthorize`
接下来就回到 Spring Security 中来看 `@PreAuthorize` 注解。<br />权限的实现方式千千万，又有各种不同的权限模型，然而归结到代码上，无非两种：

- 基于 URL 地址的权限处理
- 基于方法注解的权限处理

`@PreAuthorize` 注解当然对应的是后者。来看一个例子：
```java
@PreAuthorize("@ss.hasPermi('tienchin:channel:query')")
@GetMapping("/list")
public TableDataInfo getChannelList() {
    startPage();
    List<Channel> list = channelService.list();
    return getDataTable(list);
}
```
注解好说，里边的 `@ss.hasPermi('tienchin:channel:query')` 是啥意思呢？

- ss 是一个注册在 Spring 容器中的 bean，对应的类位于 `org.javaboy.tienchin.framework.web.service.PermissionService` 中。
- 很明显，`hasPermi` 就是这个类中的方法。

这个 hasPermi 方法的逻辑其实很简单：
```java
public boolean hasPermi(String permission) {
	if (StringUtils.isEmpty(permission)) {
		return false;
	}
	LoginUser loginUser = SecurityUtils.getLoginUser();
	if (StringUtils.isNull(loginUser) || CollectionUtils.isEmpty(loginUser.getPermissions())) {
		return false;
	}
	return hasPermissions(loginUser.getPermissions(), permission);
}
private boolean hasPermissions(Set<String> permissions, String permission) {
	return permissions.contains(ALL_PERMISSION) || permissions.contains(StringUtils.trim(permission));
}
```
这个判断逻辑很简单，就是获取到当前登录的用户，判断当前登录用户的权限集合中是否具备当前请求所需要的权限。具体的判断逻辑没啥好说的，就是看集合中是否存在某个字符串。<br />那么这个方法是在哪里调用的呢？<br />大家知道，Spring Security 中处理权限的过滤器是 `FilterSecurityInterceptor`，所有的权限处理最终都会来到这个过滤器中。在这个过滤器中，将会用到各种投票器、表决器之类的工具，这里就不细说了，之前的 Spring Security 系列教程都有详细介绍。<br />在投票器中，可以看到专门处理 `@PreAuthorize` 注解的类 `PreInvocationAuthorizationAdviceVoter`，来看下他里边的核心方法：
```java
@Override
public int vote(Authentication authentication, MethodInvocation method, Collection<ConfigAttribute> attributes) {
	PreInvocationAttribute preAttr = findPreInvocationAttribute(attributes);
	if (preAttr == null) {
		return ACCESS_ABSTAIN;
	}
	return this.preAdvice.before(authentication, method, preAttr) ? ACCESS_GRANTED : ACCESS_DENIED;
}
```
框架的源码写的就是好，一看名字就知道他想干嘛了！这里就进入到最后一句，调用了一个 Advice 中到前置通知，来判断权限是否满足：
```java
public boolean before(Authentication authentication, MethodInvocation mi, PreInvocationAttribute attr) {
	PreInvocationExpressionAttribute preAttr = (PreInvocationExpressionAttribute) attr;
	EvaluationContext ctx = this.expressionHandler.createEvaluationContext(authentication, mi);
	Expression preFilter = preAttr.getFilterExpression();
	Expression preAuthorize = preAttr.getAuthorizeExpression();
	if (preFilter != null) {
		Object filterTarget = findFilterTarget(preAttr.getFilterTarget(), ctx, mi);
		this.expressionHandler.filter(filterTarget, preFilter, ctx);
	}
	return (preAuthorize != null) ? ExpressionUtils.evaluateAsBoolean(preAuthorize, ctx) : true;
}
```
现在，当看到这个 before 方法的时候，应该会觉得比较熟悉了吧。

1. 首先获取到 preAttr 对象，这个对象里边其实就保存着 `@PreAuthorize` 注解中的内容。
2. 接下来跟进当前登录用户信息 authentication 创建一个上下文对象，此时创建出来的上下文对象中就包含了当前用户具备哪些权限。
3. 获取过滤器。
4. 获取到权限注解。
5. 最后执行表达式，去查看当前用户权限中是否包含请求所需要的权限。
