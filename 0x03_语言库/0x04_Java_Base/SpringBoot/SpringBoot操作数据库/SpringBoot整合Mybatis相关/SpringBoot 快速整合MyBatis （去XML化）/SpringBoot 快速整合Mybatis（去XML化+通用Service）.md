JavaSpringBootMybatis
<a name="j1BdR"></a>
## 具体步骤
得益于Spring4的新特性，可以使用`@Autowired`注入泛型依赖。
<a name="i71yK"></a>
## 通用Mapper
在MyBatis项目中，引入通用Mapper的依赖，其他配置不变。
```xml
<dependency> 
  <groupId>tk.mybatis</groupId>
  <artifactId>mapper-spring-boot-starter</artifactId>
  <version>1.1.4</version>
</dependency>
```
<a name="x6y96"></a>
## 通用Service
BaseService可以根据实际需求来添加常用的CURD接口方法，例如：
```java
package com.hehe.tools;

public class BaseServiceImpl implements BaseService {
    @Autowired
    private Mapper<T> mapper;//泛型装配

    @Override
    public List<T> list(T entity) {
        return mapper.select(entity);
    }

    @Override
    public T get(T entity) {
        return  mapper.selectOne(entity);
    }

    @Override
    public int update(T entity) {
        return mapper.updateByPrimaryKeySelective(entity);
    }

    @Override
    public int save(T entity) {
        return mapper.insertSelective(entity);
    }

    @Override
    public int delete(T entity) {
        return mapper.deleteByPrimaryKey(entity);
    }
}
```
<a name="C2lBt"></a>
## 使用方法
在Mapper接口继承通用Mapper并指定泛型，如下：
```java
public interface UserMapper extends Mapper {
}
```
在Service实现类继承通用Service，如下：
```java
@Service  
public class UserServiceImpl extends BaseServiceImpl implements UserService {
}
```
最后在Controller 提供RestFul API，如下：
```java
@RestController  
@RequestMapping("/user/*")
public class UserController {
    @Autowired
    UserService userService;

    @RequestMapping("list")
    public List<User> list(User user) {
        return userService.list(user);
    }

    @RequestMapping("get")
    public User get(User user) {
        return userService.get(user);
    }

    @RequestMapping("update")
    public int update(User user) {
        return userService.update(user);
    }

    @RequestMapping("save")
    public int save(User user) {
        return userService.save(user);
    }

    @RequestMapping("delete")
    public int delete(User user) {
        return userService.delete(user);
    }
}
```
