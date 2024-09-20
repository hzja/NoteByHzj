JavaSpring
<a name="U7hKB"></a>
## 大纲
![2023-05-10-10-49-37.025983000.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1683687370887-763bb02e-a942-4849-b7aa-5d918a406d07.png#averageHue=%23f7f7f7&clientId=u3bb89846-7435-4&from=ui&id=ufc93ee0e&originHeight=1034&originWidth=1080&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=3356696&status=done&style=none&taskId=u926c4c19-3879-4b60-b4af-497d9284387&title=)
<a name="AIiNX"></a>
## 数据库引擎不支持事务
在MySQL5.5版本之前，默认的引擎是MyISAM，MyISAM引擎是不支持事务的。从MySQL5.5版本开始，默认引擎是InnoDB，InnoDB完全支持ACID原则和事务。所以当事务失效时，可以手动查询下当前数据库的存储引擎，具体的SQL语句如下：
```sql
SHOW VARIABLES LIKE 'default_storage_engine%';
```
<a name="MLCba"></a>
## 类没有被Spring管理
```java
// @Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void saveUser(User user){
        // do something...
    }
}
```
对于上面的例子，类没有被`@Service`注解修饰，意味着当前类没有被Spring管理，事务自然就失效了。
<a name="iR24K"></a>
## 方法的访问修饰符不是`public`
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    protected void saveUser(User user){
        // do something...
    }
}
```
`@Transactional`注解只有修饰`public`方法时，事务才会生效，如果用在非`public`方法上，如`protected`、`private`，事务将失效。
<a name="myH7q"></a>
## 自身调用问题
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void saveUser(User user){
        // do something...
        updateUser(user);
    }

    @Transactional
    public void updateUser(User user){
        // do something...
    }
}
```
由于类发生了自身调用，即在`saveUser`方法中，使用了`this.updateUser()`进行方法调用，没有经过Spring的代理类，默认只有在外部调用事务方法时才生效。若程序存在自身调用且需要事务生效，可以通过获取代理类的方法进行调用，示例代码如下：
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void saveUser(User user){
        // do something...
        ((UserService)AopContext.currentProxy()).updateUser(user);
    }

    @Transactional
    public void updateUser(User user){
        // do something...
    }
}
```
<a name="ejRHS"></a>
## 数据源没有配置事务管理器
数据源没有配置事务管理器时，事务将不生效，可通过以下方法进行配置。
```java
@Bean
public PlatformTransactionManager transactionManager(DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```
<a name="BjWBX"></a>
## 不支持事务
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void saveUser(User user){
        // do something...
        ((UserService)AopContext.currentProxy()).updateUser(user);
    }

    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public void updateUser(User user){
        // do something...
    }
}
```
当`@Transactional`注解使用了错误的传播机制，会导致事务失效。`Propagation.NOT_SUPPORTED`表示不以事务运行，当前若存在事务时则挂起。
<a name="z37CL"></a>
## 异常被捕获
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void saveUser(User user){
        try {
            // do something...
            int i = 1/0;
        } catch (Exception e){
            e.printStackTrace();
        }
    }
}
```
当出现除0异常时，异常被catch捕获了，整个程序正常运行，没有抛出异常，最终导致事务无法回滚。
<a name="edS57"></a>
## 异常类型错误
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void saveUser(User user){
        throw new Exception("save user error");
    }
}
```
`@Transactional`注解中，回滚的默认异常是`RuntimeException`，由于在`saveUser()`方法中抛出异常不是`RuntimeException`，导致当前事务不会回滚。若要使事务生效，建议使用`@Transactional(rollbackFor = Exception.class)`进行修饰。
