JavaSpringBootShiro<br />数据库中密码相关字段都不是明文，肯定是加密之后的，传统方式一般是使用MD5加密。<br />单纯使用不加盐的MD5加密方式，当两个用户的密码相同时，会发现数据库中存在相同内容的密码，这样也是不安全的。希望即便是两个人的原始密码一样，加密后的结果也不一样。<br />下面进行shiro密码 加密加盐配置：
<a name="xhBDO"></a>
### 1、`ShiroConfig`中添加密码比较器
```java
/**
 * 配置密码比较器
 * @return
 */
@Bean("credentialsMatcher")
public RetryLimitHashedCredentialsMatcher retryLimitHashedCredentialsMatcher(){
    RetryLimitHashedCredentialsMatcher retryLimitHashedCredentialsMatcher = new RetryLimitHashedCredentialsMatcher();
    retryLimitHashedCredentialsMatcher.setRedisManager(redisManager());

    //如果密码加密,可以打开下面配置
    //加密算法的名称
    retryLimitHashedCredentialsMatcher.setHashAlgorithmName("MD5");
    //配置加密的次数
    retryLimitHashedCredentialsMatcher.setHashIterations(2);
    //是否存储为16进制
    retryLimitHashedCredentialsMatcher.setStoredCredentialsHexEncoded(true);

    return retryLimitHashedCredentialsMatcher;
}
```
<a name="vCWbq"></a>
### 2、将密码比较器配置给`ShiroRealm`
```java
/**
 *  身份认证realm; (这个需要自己写，账号密码校验；权限等)
 * @return
 */
@Bean
public ShiroRealm shiroRealm(){
    ShiroRealm shiroRealm = new ShiroRealm();
    shiroRealm.setCachingEnabled(true);
    //启用身份验证缓存，即缓存AuthenticationInfo信息，默认false
    shiroRealm.setAuthenticationCachingEnabled(true);
    //缓存AuthenticationInfo信息的缓存名称 在ehcache-shiro.xml中有对应缓存的配置
    shiroRealm.setAuthenticationCacheName("authenticationCache");
    //启用授权缓存，即缓存AuthorizationInfo信息，默认false
    shiroRealm.setAuthorizationCachingEnabled(true);
    //缓存AuthorizationInfo信息的缓存名称  在ehcache-shiro.xml中有对应缓存的配置
    shiroRealm.setAuthorizationCacheName("authorizationCache");
    //配置自定义密码比较器
    shiroRealm.setCredentialsMatcher(retryLimitHashedCredentialsMatcher());
    return shiroRealm;
}
```
<a name="wOt9j"></a>
### 3、密码比较器`RetryLimitHashedCredentialsMatcher`
自定义的密码比较器，跟前面博客中逻辑没有变化，唯一变的是 继承的类从 `SimpleCredentialsMatcher` 变为 `HashedCredentialsMatcher`<br />在密码比较器中做了：如果用户输入密码连续错误5次，将锁定账号。<br />`RetryLimitHashedCredentialsMatcher`完整内容如下：
```java
package com.shiro.config;

import java.util.concurrent.atomic.AtomicInteger;

import com.springboot.test.shiro.modules.user.dao.UserMapper;
import com.springboot.test.shiro.modules.user.dao.entity.User;
import org.apache.log4j.Logger;
import org.apache.shiro.authc.AuthenticationInfo;
import org.apache.shiro.authc.AuthenticationToken;
import org.apache.shiro.authc.LockedAccountException;
import org.apache.shiro.authc.credential.HashedCredentialsMatcher;
import org.springframework.beans.factory.annotation.Autowired;


/**
 * @description: 登陆次数限制
 */
public class RetryLimitHashedCredentialsMatcher extends HashedCredentialsMatcher {

    private static final Logger logger = Logger.getLogger(RetryLimitHashedCredentialsMatcher.class);

    public static final String DEFAULT_RETRYLIMIT_CACHE_KEY_PREFIX = "shiro:cache:retrylimit:";
    private String keyPrefix = DEFAULT_RETRYLIMIT_CACHE_KEY_PREFIX;
    @Autowired
    private UserMapper userMapper;
    private RedisManager redisManager;

    public void setRedisManager(RedisManager redisManager) {
        this.redisManager = redisManager;
    }

    private String getRedisKickoutKey(String username) {
        return this.keyPrefix + username;
    }

    @Override
    public boolean doCredentialsMatch(AuthenticationToken token, AuthenticationInfo info) {

        //获取用户名
        String username = (String)token.getPrincipal();
        //获取用户登录次数
        AtomicInteger retryCount = (AtomicInteger)redisManager.get(getRedisKickoutKey(username));
        if (retryCount == null) {
            //如果用户没有登陆过,登陆次数加1 并放入缓存
            retryCount = new AtomicInteger(0);
        }
        if (retryCount.incrementAndGet() > 5) {
            //如果用户登陆失败次数大于5次 抛出锁定用户异常  并修改数据库字段
            User user = userMapper.findByUserName(username);
            if (user != null && "0".equals(user.getState())){
                //数据库字段 默认为 0  就是正常状态 所以 要改为1
                //修改数据库的状态字段为锁定
                user.setState("1");
                userMapper.update(user);
            }
            logger.info("锁定用户" + user.getUsername());
            //抛出用户锁定异常
            throw new LockedAccountException();
        }
        //判断用户账号和密码是否正确
        boolean matches = super.doCredentialsMatch(token, info);
        if (matches) {
            //如果正确,从缓存中将用户登录计数 清除
            redisManager.del(getRedisKickoutKey(username));
        }{
            redisManager.set(getRedisKickoutKey(username), retryCount);
        }
        return matches;
    }

    /**
     * 根据用户名 解锁用户
     * @param username
     * @return
     */
    public void unlockAccount(String username){
        User user = userMapper.findByUserName(username);
        if (user != null){
            //修改数据库的状态字段为锁定
            user.setState("0");
            userMapper.update(user);
            redisManager.del(getRedisKickoutKey(username));
        }
    }

}
```
<a name="UZOaM"></a>
### 4、修改ShiroRealm中`doGetAuthenticationInfo`方法
```java
package com.springboot.shiro.realm;

import com.springboot.test.shiro.modules.user.dao.PermissionMapper;
import com.springboot.test.shiro.modules.user.dao.RoleMapper;
import com.springboot.test.shiro.modules.user.dao.entity.Permission;
import com.springboot.test.shiro.modules.user.dao.entity.Role;
import com.springboot.test.shiro.modules.user.dao.UserMapper;
import com.springboot.test.shiro.modules.user.dao.entity.User;
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.*;
import org.apache.shiro.authz.AuthorizationInfo;
import org.apache.shiro.authz.SimpleAuthorizationInfo;
import org.apache.shiro.realm.AuthorizingRealm;
import org.apache.shiro.subject.PrincipalCollection;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.Set;

/**
 * @description: 在Shiro中，最终是通过Realm来获取应用程序中的用户、角色及权限信息的
 * 在Realm中会直接从我们的数据源中获取Shiro需要的验证信息。可以说，Realm是专用于安全框架的DAO.
 */
public class ShiroRealm extends AuthorizingRealm {

    @Autowired
    private UserMapper userMapper;

    @Autowired
    private RoleMapper roleMapper;

    @Autowired
    private PermissionMapper permissionMapper;

    /**
     * 验证用户身份
     * @param authenticationToken
     * @return
     * @throws AuthenticationException
     */
    @Override
    protected AuthenticationInfo doGetAuthenticationInfo(AuthenticationToken authenticationToken) throws AuthenticationException {

        //获取用户名密码 第一种方式
        //String username = (String) authenticationToken.getPrincipal();
        //String password = new String((char[]) authenticationToken.getCredentials());

        //获取用户名 密码 第二种方式
        UsernamePasswordToken usernamePasswordToken = (UsernamePasswordToken) authenticationToken;
        String username = usernamePasswordToken.getUsername();
        String password = new String(usernamePasswordToken.getPassword());

        //从数据库查询用户信息
        User user = this.userMapper.findByUserName(username);

        //可以在这里直接对用户名校验,或者调用 CredentialsMatcher 校验
        if (user == null) {
            throw new UnknownAccountException("用户名或密码错误！");
        }
        //这里将 密码对比 注销掉,否则 无法锁定  要将密码对比 交给 密码比较器
        //if (!password.equals(user.getPassword())) {
        //    throw new IncorrectCredentialsException("用户名或密码错误！");
        //}
        if ("1".equals(user.getState())) {
            throw new LockedAccountException("账号已被锁定,请联系管理员！");
        }

        SimpleAuthenticationInfo info = new SimpleAuthenticationInfo(user, user.getPassword(),new MyByteSource(user.getUsername()),getName());
        return info;
    }

    /**
     * 授权用户权限
     * 授权的方法是在碰到<shiro:hasPermission name=''></shiro:hasPermission>标签的时候调用的
     * 它会去检测shiro框架中的权限(这里的permissions)是否包含有该标签的name值,如果有,里面的内容显示
     * 如果没有,里面的内容不予显示(这就完成了对于权限的认证.)
     *
     * shiro的权限授权是通过继承AuthorizingRealm抽象类，重载doGetAuthorizationInfo();
     * 当访问到页面的时候，链接配置了相应的权限或者shiro标签才会执行此方法否则不会执行
     * 所以如果只是简单的身份认证没有权限的控制的话，那么这个方法可以不进行实现，直接返回null即可。
     *
     * 在这个方法中主要是使用类：SimpleAuthorizationInfo 进行角色的添加和权限的添加。
     * authorizationInfo.addRole(role.getRole()); authorizationInfo.addStringPermission(p.getPermission());
     *
     * 当然也可以添加set集合：roles是从数据库查询的当前用户的角色，stringPermissions是从数据库查询的当前用户对应的权限
     * authorizationInfo.setRoles(roles); authorizationInfo.setStringPermissions(stringPermissions);
     *
     * 就是说如果在shiro配置文件中添加了filterChainDefinitionMap.put("/add", "perms[权限添加]");
     * 就说明访问/add这个链接必须要有“权限添加”这个权限才可以访问
     *
     * 如果在shiro配置文件中添加了filterChainDefinitionMap.put("/add", "roles[100002]，perms[权限添加]");
     * 就说明访问/add这个链接必须要有 "权限添加" 这个权限和具有 "100002" 这个角色才可以访问
     * @param principalCollection
     * @return
     */
    @Override
    protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principalCollection) {

        System.out.println("查询权限方法调用了！！！");

        //获取用户
        User user = (User) SecurityUtils.getSubject().getPrincipal();

        //获取用户角色
        Set<Role> roles =this.roleMapper.findRolesByUserId(user.getUid());
        //添加角色
        SimpleAuthorizationInfo authorizationInfo =  new SimpleAuthorizationInfo();
        for (Role role : roles) {
            authorizationInfo.addRole(role.getRole());
        }

        //获取用户权限
        Set<Permission> permissions = this.permissionMapper.findPermissionsByRoleId(roles);
        //添加权限
        for (Permission permission:permissions) {
            authorizationInfo.addStringPermission(permission.getPermission());
        }

        return authorizationInfo;
    }

    /**
     * 重写方法,清除当前用户的的 授权缓存
     * @param principals
     */
    @Override
    public void clearCachedAuthorizationInfo(PrincipalCollection principals) {
        super.clearCachedAuthorizationInfo(principals);
    }

    /**
     * 重写方法，清除当前用户的 认证缓存
     * @param principals
     */
    @Override
    public void clearCachedAuthenticationInfo(PrincipalCollection principals) {
        super.clearCachedAuthenticationInfo(principals);
    }

    @Override
    public void clearCache(PrincipalCollection principals) {
        super.clearCache(principals);
    }

    /**
     * 自定义方法：清除所有 授权缓存
     */
    public void clearAllCachedAuthorizationInfo() {
        getAuthorizationCache().clear();
    }

    /**
     * 自定义方法：清除所有 认证缓存
     */
    public void clearAllCachedAuthenticationInfo() {
        getAuthenticationCache().clear();
    }

    /**
     * 自定义方法：清除所有的  认证缓存  和 授权缓存
     */
    public void clearAllCache() {
        clearAllCachedAuthenticationInfo();
        clearAllCachedAuthorizationInfo();
    }

}
```
跟之前的 ShiroRealm 相比，唯一改变的了<br />`SimpleAuthenticationInfo info = new SimpleAuthenticationInfo(user, user.getPassword(),new MyByteSource(user.getUsername()),getName());`这一行代码，添加了 加盐参数。<br />注意：这里使用了 MyByteSource 而不是 `ByteSource.Util.bytes(user.getUsername())`。
<a name="aAd2T"></a>
### 5、下面是生成密码加密加盐的方法，可以在注册的时候对明文进行加密 加盐 入库
```java
package com.fcant.shiro.test;

import org.apache.shiro.crypto.hash.SimpleHash;
import org.apache.shiro.util.ByteSource;
import org.junit.Test;

/**
 * @description: 给 密码进行 加密加盐  盐值默认为 用户名
 */
public class PasswordSaltTest {

    @Test
    public void test() throws Exception {
        System.out.println(md5("123456","admin"));
    }

    public static final String md5(String password, String salt){
        //加密方式
        String hashAlgorithmName = "MD5";
        //盐：为了即使相同的密码不同的盐加密后的结果也不同
        ByteSource byteSalt = ByteSource.Util.bytes(salt);
        //密码
        Object source = password;
        //加密次数
        int hashIterations = 2;
        SimpleHash result = new SimpleHash(hashAlgorithmName, source, byteSalt, hashIterations);
        return result.toString();
    }

}
```
<a name="G9OFh"></a>
### 可能出现的问题
可能会发生这种情况，测试发现密码不对，具体原因debug都可以发现，这里直接把结果发出来：
<a name="TO9Vf"></a>
#### 第一种：
debug发现 传入的密码 经过加密加盐之后是对的，但是 从数据库中 获取的密码 却是明文，原因是在ShiroRealm中 `doGetAuthenticationInfo`方法中，最后返回的`SimpleAuthenticationInfo` 第二个参数 是密码，这个密码 不是从前台传过来的密码，而是从数据库中查询出来的
<a name="J5p6R"></a>
#### 第二种：
debug发现 传入的密码 经过加密加盐之后是对的，但是 从数据库中 获取的密码 却是更长的一段密文，原因是在`ShiroConfig`中配置的`RetryLimitHashedCredentialsMatcher`一个属性：
```java
//是否存储为16进制
retryLimitHashedCredentialsMatcher.setStoredCredentialsHexEncoded(true);
```
默认是true，如果改为false，则会出现 对比的时候从数据库拿出密码，然后转 `base64` 变成了另外一个更长的字符串，所以怎么对比都是不通过的。
