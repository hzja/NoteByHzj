Java Spring 事务
<a name="F2uZr"></a>
## 不用Spring管理事务？
先来看一下不用Spring管理事务时，各种框架是如何管理事务的，使用JDBC来管理事务<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356550-e323cdc1-e1b4-4b28-9c63-9dab94cb0403.png#averageHue=%232f2e2d&clientId=ua847436f-7ccb-4&from=paste&id=u8d5b6f17&originHeight=158&originWidth=365&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2ceab22c-967c-4e18-882f-cd825b6ab00&title=)<br />使用Hibernate来管理事务<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356442-018cd2b1-5e7d-452f-979b-12bf0870e05e.png#averageHue=%232d2d2c&clientId=ua847436f-7ccb-4&from=paste&id=u3ea5c88e&originHeight=190&originWidth=379&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u584ca90b-24ab-43fc-a0c0-e1c0d6e5dfb&title=)<br />业务逻辑和事务代码是耦合到一块的，并且和框架的具体api绑定了。当我们换一种框架来实现时，里面对事务控制的代码就要推倒重写，并不一定能保证替换后的api和之前的api有相同的行为。
<a name="gSzto"></a>
### 「统一的事务抽象」
基于这些问题，Spring抽象了一些事务相关的顶层接口。无论是全局事务还是本地事务，JTA，JDBC还是Hibernate，Spring都使用统一的编程模型。使得应用程序可以很容易的在全局事务与本地事务，或者不同事物框架之间进行切换。
<a name="rBOc7"></a>
### 「下图为Spring事物抽象的核心类」
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356692-286d3658-cad6-4282-82c2-f158d5425cae.png#averageHue=%23ebeabe&clientId=ua847436f-7ccb-4&from=paste&id=ubb2a4333&originHeight=279&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uede230ae-bd31-42f2-b201-4793b227ebd&title=)

| 常用api | 接口 |
| --- | --- |
| `PlatformTransactionManager` | 对事务进行管理 |
| `TransactionDefinition` | 定义事务的相关属性，例如隔离级别，传播行为 |
| `TransactionStatus` | 保存事务状态 |

针对不同的数据访问技术，使用不用的`PlatformTransactionManager`类即可

| 数据访问技术 | `PlatformTransactionManager`实现类 |
| --- | --- |
| JDBC/Mybatis | `DataSourceTransactionManager` |
| Hibernate | `HibernateTransactionManager` |
| Jpa | `JpaTransactionManager` |
| Jms | `JmsTransactionManager` |

<a name="Lr3w3"></a>
## 编程式事务管理
当我们使用Spring的事务时，可以使用编程式事务或者声明式事务。<br />当使用编程式事务的时候，可以直接使用事务的顶层接口，也可以使用模版类`TransactionTemplate`
<a name="E5J2W"></a>
### 使用`PlatformTransactionManager`
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356431-cc545327-1198-448e-893a-6c42e6098538.png#averageHue=%232d2c2b&clientId=ua847436f-7ccb-4&from=paste&id=ua78593f1&originHeight=538&originWidth=698&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub3af4f1e-dbe7-4020-96ae-3fb68d4824b&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356537-105ab449-9ef9-4681-88af-d90760f5481e.png#averageHue=%23332f2c&clientId=ua847436f-7ccb-4&from=paste&id=u4272a93b&originHeight=59&originWidth=386&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u11280d7a-414e-4b40-b36d-2bfc6698ea3&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356827-719f9caa-d696-4518-a1cf-142102606fe3.png#averageHue=%232f2e2b&clientId=ua847436f-7ccb-4&from=paste&id=ufd27db3e&originHeight=394&originWidth=688&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u88a677f8-f422-44af-997f-b7201372d30&title=)
<a name="sV0DP"></a>
### 使用`TransactionTemplate`
当直接使用`PlatformTransactionManager`来管理事务时，有很多模版代码。例如业务代码正常执行，提交事务，否则回滚事务。可以把这部分模版代码封装成一个模版类，这样使用起来就很方便了，如下所示![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356861-a4aaa3f9-678b-47ae-bfe0-47fcb602595f.png#averageHue=%23302e2b&clientId=ua847436f-7ccb-4&from=paste&id=u3b182a02&originHeight=491&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5948bf68-5a45-41ae-8e16-81ce9605b86&title=)如下图所示，`TransactionTemplate#execute`方法就是一个典型的模版方法![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356889-d44edf0c-3a29-491f-aba4-6af6680b20a9.png#averageHue=%23312c2b&clientId=ua847436f-7ccb-4&from=paste&id=u5a3637ae&originHeight=448&originWidth=974&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufe61fb23-fa46-4a54-acb8-6f4f9b52479&title=)可以传入如下2个接口的实现类来执行业务逻辑，`TransactionCallback`（需要返回执行结果）或`TransactionCallbackWithoutResult`（不需要返回结果）
<a name="pH62S"></a>
## 声明式事务管理
为了让使用更加简洁，Spring直接把事务代码的执行放到切面中了，只需要在业务代码方法上加上一个`@Transactional`注解即可，这种方式我们最常用哈
<a name="hMB9c"></a>
### 使用`@Transactional`注解
此时事务相关的定义就可以通过`@Transactional`注解来设置了

| 属性名 | 类型 | 描述 | 默认值 |
| --- | --- | --- | --- |
| value（和`transactionManager`互为别名） | String | 当在配置文件中有多个`PlatformTransactionManager`，用该属性指定选择哪个事务管理器 | 空字符串"" |
| `propagation` | 枚举：`Propagation` | 事务的传播行为 | `REQUIRED` |
| `isolation` | 枚举：`Isolation` | 事务的隔离度 | `DEFAULT` |
| `timeout` | int | 事务的超时时间。如果超过该时间限制但事务还没有完成，则自动回滚事务 | -1 |
| `readOnly` | boolean | 指定事务是否为只读事务 | false |
| `rollbackFor` | Class[] | 需要回滚的异常 | 空数组{} |
| `rollbackForClassName` | String[] | 需要回滚的异常类名 | 空数组{} |
| `noRollbackFor` | Class[] | 不需要回滚的异常 | 空数组{} |
| `noRollbackForClassName` | String[] | 不需要回滚的异常类名 | 空数组{} |

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852356962-38416ea9-2097-4962-90a8-ffa0a3996a1f.png#averageHue=%2333312c&clientId=ua847436f-7ccb-4&from=paste&id=uc3d873c0&originHeight=458&originWidth=990&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u198510a2-590d-4dad-8d2b-585255dc7a1&title=)
<a name="T3the"></a>
## 源码解析
需要在配置类上加上`@EnableTransactionManagement`注解，来开启Spring事务管理功能![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357200-75292065-3b8b-4d88-8080-dd87f877828f.png#averageHue=%2331302b&clientId=ua847436f-7ccb-4&from=paste&id=ua2794544&originHeight=224&originWidth=910&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u903b2c3e-0f81-4c12-9ac7-6158fec8e91&title=)
<a name="BKOmv"></a>
### 「`TransactionManagementConfigurationSelector#selectImports`」![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357132-21c3cdd2-cef3-40e9-b9fd-3698a0be5b9f.png#averageHue=%232f2c2b&clientId=ua847436f-7ccb-4&from=paste&id=u7f402ce0&originHeight=464&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc5c3e1f0-5b65-4781-bff5-474b68df788&title=)
往容器中注入`AutoProxyRegistrar`和`ProxyTransactionManagementConfiguration`这2个类，那这2个类有啥作用呢？（源码太多了，就不贴代码一步一步分析了，主要是理清思路）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357326-02e23797-a965-4e26-8537-b06f85a16415.png#averageHue=%23fbf7f2&clientId=ua847436f-7ccb-4&from=paste&id=u82374af1&originHeight=421&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7ed2ed8e-d9da-402e-9d78-ef978fac920&title=)<br />`AutoProxyRegistrar`主要就是往容器中注入一个类`InfrastructureAdvisorAutoProxyCreator`，这个类有什么作用呢？
<a name="kGWlb"></a>
### 「看一下继承关系，原来是继承自`AbstractAutoProxyCreator`，用来实现自动代理没跑了！」
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357279-fb3cb1b2-aaa6-4057-8f9c-0572bb779d0a.png#averageHue=%23303030&clientId=ua847436f-7ccb-4&from=paste&id=u1df1f86c&originHeight=450&originWidth=773&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3ef6f82d-a592-4631-8e19-91b40c163b3&title=)`BeanFactoryTransactionAttributeSourceAdvisor`主要就是往容器中注入了一个`Advisor`类，用来保存`Pointcut`和`Advice`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357300-31a79696-1a90-48a9-bc47-94dd7d6e2d81.png#averageHue=%23f4e6dc&clientId=ua847436f-7ccb-4&from=paste&id=u5a78de1f&originHeight=308&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5c9063b0-1391-4602-a5fe-a77af5502fe&title=)对应的`Pointcut`为`TransactionAttributeSourcePointcut`的实现类，是一个匿名内部类，即筛选的逻辑是通过`TransactionAttributeSourcePointcut`类来实现的<br />`BeanFactoryTransactionAttributeSourceAdvisor`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357462-841a816f-dfd6-46c8-8865-662dd92268b2.png#averageHue=%232d2c2b&clientId=ua847436f-7ccb-4&from=paste&id=u7e53803d&originHeight=214&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf00f29cd-834d-4918-8adb-e1c5bc437b2&title=)对应的`Advice`的实现类为`TransactionInterceptor`，即针对事务增强的逻辑都在这个类中。<br />筛选的逻辑就先不分析了，后面会再简单提一下<br />来看针对事务增强的逻辑，当执行被`@Transactional`标记的方法时，会调用到如下方法（`TransactionInterceptor#invoke`有点类似`@Around`）<br />`TransactionInterceptor#invoke`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357639-b6f7d800-41ad-4609-ba88-48a8f0def426.png#averageHue=%232f2f2e&clientId=ua847436f-7ccb-4&from=paste&id=ud481d530&originHeight=192&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7de3e552-dca2-49d6-8220-fcfef73a052&title=)`TransactionAspectSupport#invokeWithinTransaction`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357644-ef9d170c-7394-4095-8c01-1bf05e3c78d9.png#averageHue=%23312d2b&clientId=ua847436f-7ccb-4&from=paste&id=u38b33d51&originHeight=519&originWidth=775&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0c7f0b9f-e9e7-4b33-8084-0468f9013cb&title=)挑出这个方法比较重要的几个部分来分析（上图圈出来的部分）

1. 如果需要的话开启事务（和传播属性相关，我们后面会提到）
2. 执行业务逻辑
3. 如果发生异常则会滚事务
4. 如果正常执行则提交事务
<a name="O2WKj"></a>
### 「所以当发生异常需要会滚的时候，一定不要自己把异常try catch掉，不然事务会正常提交」
`TransactionAspectSupport#createTransactionIfNecessary`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357635-103bec0a-b438-4941-88ff-90a6841f88db.png#averageHue=%232e2d2c&clientId=ua847436f-7ccb-4&from=paste&id=u38ce9f17&originHeight=596&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ube67b5cd-067f-4791-8340-97b2894c498&title=)当开启事务的时候，可以看到各种传播属性的行为（即`@Transactional`方法调用`@Transactional`方法会发生什么？）<br />`AbstractPlatformTransactionManager#getTransaction`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357778-8ecef79a-5dc6-468b-9d9b-90e9c7010cb4.png#averageHue=%23322c2b&clientId=ua847436f-7ccb-4&from=paste&id=uc325dec2&originHeight=529&originWidth=904&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf95770b9-97dc-4a1e-891c-d1e941e2cb2&title=)<br />Spring事务的传播行为在`Propagation`枚举类中定义了如下几种选择
<a name="oDnRN"></a>
#### 「支持当前事务」

- `REQUIRED`：如果当前存在事务，则加入该事务。如果当前没有事务，则创建一个新的事务
- `SUPPORTS`：如果当前存在事务，则加入该事务 。如果当前没有事务， 则以非事务的方式继续运行
- `MANDATORY`：如果当前存在事务，则加入该事务 。如果当前没有事务，则抛出异常
<a name="tCLjZ"></a>
#### 「不支持当前事务」

- `REQUIRES_NEW`：如果当前存在事务，则把当前事务挂起，创建一个新事务
- `NOT_SUPPORTED`：如果当前存在事务，则把当前事务挂起，以非事务方式运行，
- `NEVER`：如果当前存在事务，则抛出异常
<a name="yC6X8"></a>
#### 「其他情况」

- `NESTED`：如果当前存在事务，则创建一个事务作为当前事务的嵌套事务来执行 。如果当前没有事务，则该取值等价于`REQUIRED`

以`NESTED`启动的事务内嵌于外部事务中 （如果存在外部事务的话），此时内嵌事务并不是一个独立的事务，它依赖于外部事务。只有通过外部事务的提交，才能引起内部事务的提交，嵌套的子事务不能单独提交
<a name="DsTQw"></a>
## 事务失效的场景有哪些？
因为经常使用声明式事务，如果一步消息就会导致事务失效，所以就从源码角度来盘一下事务为什么失效
<a name="Kc0nO"></a>
### 异常被你try catch了
首先就是上面刚提到的，**「异常被try catch了」**。因为声明式事物是通过目标方法是否抛出异常来决定是提交事物还是会滚事物的
<a name="HvzBr"></a>
### 自调用
当自调用时，方法执行不会经过代理对象，所以会导致事务失效
```java
// 事务失效
@Service
public class UserServiceV2Impl implements UserService {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Override
	public void addUser(String name, String location) {
		doAdd(name);
	}
	
	@Transactional
	public void doAdd(String name) {
		String sql = "insert into user (`name`) values (?)";
		jdbcTemplate.update(sql, new Object[]{name});
		throw new RuntimeException("保存用户失败");
	}
}
```
可以通过如下三种方式来解决自调用失效的场景
<a name="ZTnPZ"></a>
#### 「1、`@Autowired`注入代理对象，然后调用方法」
```java
// @Service
public class UserServiceV3Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private UserService userService;

    @Override
    public void addUser(String name, String location) {
        userService.doAdd(name);
    }

    @Override
    @Transactional
    public void doAdd(String name) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        throw new RuntimeException("保存用户失败");
    }
}
```
<a name="TP717"></a>
#### 「2、从`ApplicationContext`获取代理对象，然后调用方法」
```java
@Service
public class UserServiceV4Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private ApplicationContext applicationContext;

    @Override
    public void addUser(String name, String location) {
        UserService userService = applicationContext.getBean(UserService.class);
        userService.doAdd(name);
    }

    @Override
    @Transactional
    public void doAdd(String name) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        throw new RuntimeException("保存用户失败");
    }
}
```
<a name="I5wzq"></a>
#### 「3、进行如下设置`@EnableAspectJAutoProxy(exposeProxy = true)`，从`AopContext`中获取代理对象，然后调用方法」
```java
@Service
public class UserServiceV5Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Override
    public void addUser(String name, String location) {
        UserService userService = (UserService) AopContext.currentProxy();
        userService.doAdd(name);
    }

    @Override
    @Transactional
    public void doAdd(String name) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        throw new RuntimeException("保存用户失败");
    }
}
```
<a name="CoX6j"></a>
### 非`public`方法导致事务失效
先来猜一下为什么非`public`方法会导致事务失效？
<a name="TIR7A"></a>
#### 「难道是因为非`public`方法不会生成代理对象？」
给一个非`public`方法加上`@Transactional`，debug到如下代码看一下是否会生成代理对象<br />`AbstractAutoProxyCreator#wrapIfNecessary`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357917-7c5e5b99-d42f-4e36-a9b2-7c59b9d51d7e.png#averageHue=%232c2b2b&clientId=ua847436f-7ccb-4&from=paste&id=u91be491e&originHeight=249&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u64917036-08e1-4c8c-9869-d55757e8005&title=)
<a name="pvdMH"></a>
#### 「结论是不会生成代理对象，那为什么不会生成代理对象呢？」
应该就是不符合`Pointcut`的要求了呗，在前面已经提到了事务对应的`Pointcut`为`TransactionAttributeSourcePointcut`<br />`TransactionAttributeSourcePointcut#matches`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852357947-dcc1f79e-8eee-4572-9773-5955b622a7a9.png#averageHue=%232d2c2b&clientId=ua847436f-7ccb-4&from=paste&id=u0403237e&originHeight=186&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc346ab88-22bd-4d6d-8d9a-24429aa57a6&title=)`matches`方法返回false，为什么会返回false呢？<br />一直debug发现是如下代码导致的<br />`AbstractFallbackTransactionAttributeSource#computeTransactionAttribute`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852358049-a5370bc0-bc4f-44c8-8ebb-dedab9a737b5.png#averageHue=%232f2d2b&clientId=ua847436f-7ccb-4&from=paste&id=uabbd0e20&originHeight=180&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f05d121-670d-4ac1-aa9e-f73b5ce9f20&title=)即`public`方法能正常生成代理对象，而非`public`方法因为不符合`Pointcut`的要求，根本就不会生成代理对象
<a name="PCly9"></a>
### 异常类型不正确，默认只支持`RuntimeException`和`Error`，不支持检查异常
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852358119-04523fc9-5ac1-4101-84e0-2e3924ed6430.png#averageHue=%23fdfbfa&clientId=ua847436f-7ccb-4&from=paste&id=ua0e2b9a8&originHeight=564&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc0cb1512-d417-4889-a5bb-4f0a08dd3bf&title=)
<a name="tk65R"></a>
#### 「为什么不支持检查异常呢？」
拿出上面分析过的代码![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852358100-66ae3cfa-fedd-4585-96c7-58c1e01d5de5.png#averageHue=%23312d2b&clientId=ua847436f-7ccb-4&from=paste&id=uda9b9142&originHeight=519&originWidth=775&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaf5cf98f-d03f-4096-9596-169f740f39b&title=)当执行业务逻辑发生异常的时候，会调用到`TransactionAspectSupport#completeTransactionAfterThrowing`方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852358270-f18582e4-a4a0-4551-bf05-a851deebf15c.png#averageHue=%232c2b2b&clientId=ua847436f-7ccb-4&from=paste&id=u016f96d7&originHeight=236&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u48a1d166-64a2-49c6-8cbd-40e42f9c7fe&title=)可以看到对异常类型做了判断，根据返回的结果来决定是否会滚事务，会调用到如下方法进行判断<br />`RuleBasedTransactionAttribute#rollbackOn`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852358341-dea07467-27d7-4258-a41b-8938228eb55f.png#averageHue=%232e2c2b&clientId=ua847436f-7ccb-4&from=paste&id=ua1a0c593&originHeight=976&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u05fa36b2-a3b9-4e35-a147-6812d7682d6&title=)如果用户指定了回滚的异常类型，则根据用户指定的规则来判断，否则用默认的规则<br />`DefaultTransactionAttribute`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649852358334-9e3f651e-0754-45fe-b289-e1b01712b0b4.png#averageHue=%23302d2b&clientId=ua847436f-7ccb-4&from=paste&id=u7b289030&originHeight=140&originWidth=1076&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u22335b09-8d6f-4ce4-aa9f-c3c72059fa7&title=)默认的规则为只支持`RuntimeException`和`Error`<br />可以通过`@Transactiona`l属性指定回滚的类型，一般为`Exception`即可
```java
@Transactional(rollbackFor = Exception.class)
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1649852358470-b5e86a69-ba56-4aac-8b22-3cd8315aafe8.jpeg#averageHue=%23faede1&clientId=ua847436f-7ccb-4&from=paste&id=u73396626&originHeight=460&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u08ec1a93-3c58-4204-ae78-59012609640&title=)
