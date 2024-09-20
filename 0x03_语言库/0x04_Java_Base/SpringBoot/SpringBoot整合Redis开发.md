Java SpringBoot Redis
> <a name="w8w8W"></a>
# 前言
> Redis安装参考

[Window安装Redis](https://www.yuque.com/fcant/sql/redis-install-package?view=doc_embed)
<a name="bsx3V"></a>
# 1、JavaBean对象缓存
<a name="UeDKx"></a>
## I、SpringBoot提供的Redis缓存使用注解
<a name="bYnoj"></a>
### A、`@CachePut`
将方法结果返回存放到缓存中。
<a name="ajlgu"></a>
### B、`@Cacheable`
先从缓存中通过定义的键查询，如果可以查到数据，则返回，否则执行该方法，返回数据，并且将返回的结果保存到缓存中。
<a name="hbB7W"></a>
### C、`@CacheEvict`
通过定义的键移除缓存，它有一个boolean类型的配置项beforeInvocation，表示在方法之前或者之后移除缓存。默认是false，所以默认为方法之后将缓存移除。
<a name="k6rIt"></a>
## II、SpringBoot整合RedisTemplate案例
<a name="Yr0pe"></a>
### A、配置缓存管理RedisConfig.java
```java
package org.hand.train.springboot.springboot.config;

import org.hand.train.springboot.springboot.bean.UserInfo;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.data.redis.cache.RedisCacheConfiguration;
import org.springframework.data.redis.cache.RedisCacheManager;
import org.springframework.data.redis.cache.RedisCacheWriter;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.serializer.Jackson2JsonRedisSerializer;
import org.springframework.data.redis.serializer.RedisSerializationContext;

/**
 * RedisConfig
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 19:20 2019/12/5
 */
@Configuration
public class RedisConfig {

    @Bean
    public RedisTemplate<Object, UserInfo> redisUserTemplate(RedisConnectionFactory redisConnectionFactory) {
        RedisTemplate<Object, UserInfo> template = new RedisTemplate<>();
        template.setConnectionFactory(redisConnectionFactory);
        Jackson2JsonRedisSerializer<UserInfo> serializer = new Jackson2JsonRedisSerializer<UserInfo>(UserInfo.class);
        template.setDefaultSerializer(serializer);
        return template;
    }

    /**
     * 配置主缓存管理@Primary，使用时默认用主缓存，其他的相应配置也可以在类@CacheConfig 方法@Cacheable引入
     * @author Fcant 19:44 2019/12/5
     */
    @Primary
    @Bean
    public RedisCacheManager userCacheManager(RedisTemplate<Object, UserInfo> redisEmpTemplate) {
        RedisCacheWriter redisCacheWriter = RedisCacheWriter.nonLockingRedisCacheWriter(redisEmpTemplate.getConnectionFactory());
        RedisCacheConfiguration redisCacheConfiguration = RedisCacheConfiguration.defaultCacheConfig()
                .serializeValuesWith(RedisSerializationContext.SerializationPair.fromSerializer(redisEmpTemplate.getValueSerializer()));
        return new RedisCacheManager(redisCacheWriter,redisCacheConfiguration);
    }

}
```
<a name="5LnWP"></a>
### B、缓存配置CacheConfig.java
```java
package org.hand.train.springboot.springboot.config;

import org.springframework.cache.interceptor.KeyGenerator;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.lang.reflect.Method;
import java.util.Arrays;

/**
 * CacheConfig
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 22:09 2019/12/5
 */
@Configuration
public class CacheConfig {

    @Bean("keyGenerator")
    public KeyGenerator keyGenerator() {
        return new KeyGenerator() {
            @Override
            public Object generate(Object o, Method method, Object... objects) {
                return method.getName() + "[" + Arrays.asList(objects).toString() + "]";
            }
        };
    }
}
```
<a name="XzJfC"></a>
### C、在ServiceImpl实现类上配置主机使用缓存
```java
package org.hand.train.springboot.springboot.service.impl;

import org.hand.train.springboot.springboot.bean.UserInfo;
import org.hand.train.springboot.springboot.mapper.IUserMapper;
import org.hand.train.springboot.springboot.service.IUserService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.annotation.CacheConfig;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.CachePut;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * UserServiceImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 17:27 2019/12/3
 */
@Service
@CacheConfig(cacheNames = "user", cacheManager = "userCacheManager")
public class UserServiceImpl implements IUserService {

    /**
     * 数据库操作失败的数字返回值
     */
    private final int FAILURE_CODE = 0;

    private static final Logger LOGGER = LoggerFactory.getLogger(UserServiceImpl.class);

    @Autowired
    IUserMapper userMapper;

    @Cacheable(cacheNames = "user", key = "#id")
    @Override
    public UserInfo selectUserById(int id) {
        LOGGER.info("从数据库查询ID为 {} 的用户" , id);
        return userMapper.selectUserById(id);
    }

    /**
     * 添加用户
     * 此处缓存主键使用Mybatis回填的自增主键
     *
     * @param userInfo
     * @return UserInfo
     * @author Fcant 9:22 2019/12/6
     */
    @CachePut(cacheNames = "user", key = "#result.userId")
    @Transactional(rollbackFor = Exception.class)
    @Override
    public UserInfo addUser(UserInfo userInfo) {
        int i = userMapper.addUser(userInfo);
        if (i == FAILURE_CODE) {
            return null;
        }
        return userInfo;
    }

    @CachePut(cacheNames = "user", key = "#userInfo.userId")
    @Transactional(rollbackFor = Exception.class)
    @Override
    public UserInfo updateUser(UserInfo userInfo) {
        int i = userMapper.updateUser(userInfo);
        if (i == FAILURE_CODE) {
            return null;
        }
        return userInfo;
    }

    @CacheEvict(beforeInvocation = true, cacheNames = "user", key = "#userInfo.userId")
    @Transactional(rollbackFor = Exception.class)
    @Override
    public UserInfo delUser(UserInfo userInfo) {
        int i = userMapper.delUser(userInfo);
        if (i == FAILURE_CODE) {
            return null;
        }
        userInfo = userMapper.selectUserById(userInfo.getUserId());
        return userInfo;
    }
}
```
<a name="JKTJH"></a>
# 2、List集合缓存
<a name="QW7hn"></a>
## I、自定义处理List集合的缓存接口-IUserCache.java
```java
package org.hand.train.springboot.springboot.cache;

import org.hand.train.springboot.springboot.bean.UserInfo;

import java.util.List;

/**
 * UserCache
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 10:18 2019/12/6
 */
public interface IUserCache {

    /**
     * 将查询到的Users的List集合缓存到Redis
     *
     * @param key
     * @param userInfoList
     * @author Fcant 10:21 2019/12/6
     */
    void listUsersSave(String key, List<UserInfo> userInfoList);

    /**
     * 从缓存中获取用户集合
     *
     * @param key
     * @return List<UserInfo>
     * @author Fcant 10:25 2019/12/6
     */
    List<UserInfo> getUsersList(String key);
}

```
<a name="9VHgw"></a>
## II、实现缓存处理的接口-UserCacheImpl.java
```java
package org.hand.train.springboot.springboot.cache.impl;

import com.google.gson.Gson;
import org.hand.train.springboot.springboot.bean.UserInfo;
import org.hand.train.springboot.springboot.cache.IUserCache;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.core.ValueOperations;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * IUserCacheImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 10:26 2019/12/6
 */
@Service
public class UserCacheImpl implements IUserCache {

    private final StringRedisTemplate redisTemplate;

    public UserCacheImpl(StringRedisTemplate redisTemplate) {
        this.redisTemplate = redisTemplate;
    }

    /**
     * 将查询到的Users的List集合缓存到Redis
     *
     * @param key 缓存的Key
     * @param userInfoList 缓存的集合
     * @author Fcant 10:21 2019/12/6
     */
    @Override
    public void listUsersSave(String key, List<UserInfo> userInfoList) {
        Gson gson = new Gson();
        ValueOperations<String, String> stringStringValueOperations = redisTemplate.opsForValue();
        stringStringValueOperations.set(key, gson.toJson(userInfoList));
    }

    /**
     * 从缓存中获取用户集合
     *
     * @param key 获取缓存的key
     * @return List<UserInfo>
     * @author Fcant 10:25 2019/12/6
     */
    @Override
    public List<UserInfo> getUsersList(String key) {
        ValueOperations<String,String> vo = redisTemplate.opsForValue();
        String userList = vo.get(key);
        Gson gson = new Gson();
        List<UserInfo> userInfoList = gson.fromJson(userList, List.class);
        return userInfoList == null ? new ArrayList<>() : userInfoList;
    }
}

```
<a name="l7efS"></a>
## III、在Service层使用
```java
package org.hand.train.springboot.springboot.service.impl;

import org.hand.train.springboot.springboot.bean.UserInfo;
import org.hand.train.springboot.springboot.cache.IUserCache;
import org.hand.train.springboot.springboot.mapper.IUserMapper;
import org.hand.train.springboot.springboot.service.IUserService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * UserServiceImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 17:27 2019/12/3
 */
@Service
public class UserServiceImpl implements IUserService {

    /**
     * 数据库操作失败的数字返回值
     */
    private final int FAILURE_CODE = 0;

    private static final Logger LOGGER = LoggerFactory.getLogger(UserServiceImpl.class);

    @Autowired
    private IUserCache userCache;

    @Autowired
    IUserMapper userMapper;

    @Override
    public List<UserInfo> selectAllUser() {
        List<UserInfo> usersList;
        usersList = userCache.getUsersList("userList");
        if (usersList.isEmpty()) {
            LOGGER.info("从数据库查询所有用户信息");
            usersList = userMapper.selectAllUser();
            userCache.listUsersSave("userList", usersList);
        }
        return usersList;
    }

}
```
<a name="aItTZ"></a>
# 3、本次整合的目录结构
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575618445229-aeea9e09-994e-4dda-a1af-bd8dd359322c.png#averageHue=%23fcfbfa&height=616&id=SysCC&originHeight=616&originWidth=369&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26650&status=done&style=shadow&title=&width=369)
