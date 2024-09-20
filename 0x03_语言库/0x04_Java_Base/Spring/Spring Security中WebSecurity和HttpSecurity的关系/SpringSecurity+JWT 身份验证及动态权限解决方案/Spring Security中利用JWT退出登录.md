Java Spring Spring Security<br />在Spring Security中用JWT做退出登录的时无法获取当前用户，导致无法证明“我就是要退出的那个我”，业务失败！经过一番排查找到了原因。
<a name="XFxDo"></a>
## Session会话
之所以要说Session会话，是因为Spring Security默认配置就是有会话的，所以当登录以后Session就会由服务端保持直到退出登录。只要Session保持住，请求只要进入服务器就可以从`ServletRequest`中获取到当前的`HttpSession`，然后会根据`HttpSession`来加载当前的`SecurityContext`。相关的逻辑在Spring Security默认的过滤器`SecurityContextPersistenceFilter`中，有兴趣可以看相关的源码。<br />而且默认情况下`SecurityContextPersistenceFilter`的优先级是高于退出过滤器`LogoutFilter`的，所以能够保证有Session会话的情况下退出一定能够获取当前用户。
<a name="VuMDc"></a>
## 无Session会话
使用了JWT后，每次请求都要携带Bearer Token并且被专门的过滤器拦截解析之后才能将用户认证信息保存到`SecurityContext`中去。Token认证实现`JwtAuthenticationFilter`，相关逻辑为：
```java
// 当token匹配         
if (jwtToken.equals(accessToken)) {
    // 解析 权限集合  这里
    JSONArray jsonArray = jsonObject.getJSONArray("roles");
    List<String> roles = jsonArray.toList(String.class);
    String[] roleArr = roles.toArray(new String[0]);

    List<GrantedAuthority> authorities = AuthorityUtils.createAuthorityList(roleArr);
    User user = new User(username, "[PROTECTED]", authorities);
    // 构建用户认证token
    UsernamePasswordAuthenticationToken usernamePasswordAuthenticationToken = new UsernamePasswordAuthenticationToken(user, null, authorities);
    usernamePasswordAuthenticationToken.setDetails(new WebAuthenticationDetailsSource().buildDetails(request));
    // 放入安全上下文中
    SecurityContextHolder.getContext().setAuthentication(usernamePasswordAuthenticationToken);
} else {
    // token 不匹配
    if (log.isDebugEnabled()){
        log.debug("token : {}  is  not in matched", jwtToken);
    }

    throw new BadCredentialsException("token is not matched");
}

```
<a name="sKb7I"></a>
## 为什么退出登录无法获取当前用户
分析了两种情况下用户认证信息的安全上下文配置后，回到问题的本身。来看看为什么用JWT会出现无法获取当前认证信息的原因。在`HttpSecurity`中，配置`JwtAuthenticationFilter`的顺序的：
```java
httpSecurity.addFilterBefore(jwtAuthenticationFilter, UsernamePasswordAuthenticationFilter.class)
```
再看看Spring Security过滤器排序图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634178222802-610716a5-a266-4fdb-99d8-91a703026710.png#clientId=u934fd2d8-1d85-4&from=paste&height=422&id=uf3ffb7eb&originHeight=1266&originWidth=703&originalType=binary&ratio=1&rotation=0&showTitle=false&size=133126&status=done&style=shadow&taskId=u7dff54e7-1c36-4a96-9926-a0138a0ca72&title=&width=234.33333333333334)<br />Spring Security过滤器排序<br />也就说`LogoutFilter`执行退出的时候，JWT还没有被`JwtAuthenticationFilter`拦截，当然无法获取当前认证上下文`SecurityContext`。
<a name="wafio"></a>
## 解决方法
解决方法就是必须在`LogoutFilter`执行前去解析JWT并将成功认证的信息存到`SecurityContext`。可以这样配置：
```java
httpSecurity.addFilterBefore(jwtAuthenticationFilter, LogoutFilter.class)
```
这样问题就解决了，只要实现把当前JWT作废掉就退出登录了。
