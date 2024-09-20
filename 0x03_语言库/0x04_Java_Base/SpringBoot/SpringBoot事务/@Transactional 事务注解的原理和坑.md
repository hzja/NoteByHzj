JavaSpring@Transactional<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874619377-e065425c-8be9-411f-99ab-ab003e4acc2e.png#clientId=u9f547560-fee7-4&from=paste&id=ucb9eb8c2&originHeight=555&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5f1f5262-0ba6-4767-bbb1-23f93c2f4a1&title=)
<a name="qjSbY"></a>
## 项目准备
下面是 DB 数据和 DB 操作接口：

| **uid** | **uname** | **usex** |
| --- | --- | --- |
| 1 | 张三 | 女 |
| 2 | 陈恒 | 男 |
| 3 | 楼仔 | 男 |

```java
// 提供的接口
public interface UserDao {
    // select * from user_test where uid = "#{uid}"
    public MyUser selectUserById(Integer uid);
    // update user_test set uname =#{uname},usex = #{usex} where uid = #{uid}
    public int updateUser(MyUser user);
}
```
基础测试代码，`testSuccess()` 是事务生效的情况：
```java
@Service
public class UserController {
    @Autowired
    private UserDao userDao;

    public void update(Integer id) {
        MyUser user = new MyUser();
        user.setUid(id);
        user.setUname("张三-testing");
        user.setUsex("女");
        userDao.updateUser(user);
    }

    public MyUser query(Integer id) {
        MyUser user = userDao.selectUserById(id);
        return user;
    }

    // 正常情况
    @Transactional(rollbackFor = Exception.class)
    public void testSuccess() throws Exception {
        Integer id = 1;
        MyUser user = query(id);
        System.out.println("原记录:" + user);
        update(id);
        throw new Exception("事务生效");
    }
}
```
<a name="EfECW"></a>
## 事务不生效的几种 Case
主要讲解 4 种事务不生效的 Case：

- **类内部访问：A 类的 a1 方法没有标注 **`**@Transactional**`**，a2 方法标注 **`**@Transactional**`**，在 a1 里面调用 a2；**
- **私有方法：将 **`**@Transactional**`** 注解标注在非 **`**public**`** 方法上；**
- **异常不匹配：**`**@Transactional**`** 未设置 **`**rollbackFor**`** 属性，方法返回 Exception 等异常；**
- **多线程：主线程和子线程的调用，线程抛出异常。**
<a name="Zdful"></a>
### Case 1：类内部访问
在类 UserController 中新增一个方法 `testInteralCall()`：
```java
public void testInteralCall() throws Exception {
    testSuccess();
    throw new Exception("事务不生效：类内部访问");
}
```
这里 `testInteralCall()` 没有标注 `@Transactional`，再看一下测试用例：
```java
public static void main(String[] args) throws Exception {
    ApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
    UserController uc = (UserController) applicationContext.getBean("userController");
    try {
        uc.testSuccess();
    } finally {
        MyUser user =  uc.query(1);
        System.out.println("修改后的记录:" + user);
    }
}
// 输出：
// 原记录:MyUser(uid=1, uname=张三, usex=女)
// 修改后的记录:MyUser(uid=1, uname=张三-testing, usex=女)
```
从上面的输出可以看到，事务并没有回滚，这个是什么原因呢？<br />因为 `@Transactional` 的工作机制是基于 AOP 实现，AOP 是使用动态代理实现的，如果通过代理直接调用 `testSuccess()`，通过 AOP 会前后进行增强，增强的逻辑其实就是在 `testSuccess()` 的前后分别加上开启、提交事务的逻辑，后面的源码会进行剖析。<br />现在是通过 `testInteralCall()` 去调用 `testSuccess()`，`testSuccess()` 前后不会进行任何增强操作，也就是**类内部调用，不会通过代理方式访问。**
<a name="FXc75"></a>
### Case 2：私有方法
在私有方法上，添加 `@Transactional` 注解也不会生效：
```java
@Transactional(rollbackFor = Exception.class)
private void testPirvateMethod() throws Exception {
    Integer id = 1;
    MyUser user = query(id);
    System.out.println("原记录:" + user);
    update(id);
    throw new Exception("测试事务生效");
}
```
直接使用时，下面这种场景不太容易出现，因为 IDEA 会有提醒，文案为：Methods annotated with '@Transactional' must be overridable，**至于深层次的原理，源码部分会解读。**
<a name="p3oGn"></a>
### Case 3：异常不匹配
这里的 `@Transactional` 没有设置 `rollbackFor = Exception.class` 属性：
```java
@Transactional
public void testExceptionNotMatch() throws Exception {
    Integer id = 1;
    MyUser user = query(id);
    System.out.println("原记录:" + user);
    update(id);
    throw new Exception("事务不生效：异常不匹配");
}
```
测试方法：同 Case1
```java
// 输出：
// 原记录:User[uid=1,uname=张三,usex=女]
// 修改后的记录:User[uid=1,uname=张三-test,usex=女]
```
`@Transactional` 注解默认处理运行时异常，即只有抛出运行时异常时，才会触发事务回滚，否则并不会回滚，**至于深层次的原理，源码部分会解读。**
<a name="fYCQb"></a>
### Case 4：多线程
下面给出两个不同的姿势，一个是子线程抛异常，主线程 ok；一个是子线程 ok，主线程抛异常。
<a name="E8j3E"></a>
#### 父线程抛出异常
父线程抛出异常，子线程不抛出异常：
```java
public void testSuccess() throws Exception {
    Integer id = 1;
    MyUser user = query(id);
    System.out.println("原记录:" + user);
    update(id);
}

@Transactional(rollbackFor = Exception.class)
public void testMultThread() throws Exception {
    new Thread(new Runnable() {
        @SneakyThrows
        @Override
        public void run() {
            testSuccess();
        }
    }).start();
    throw new Exception("测试事务不生效");
}
```
父线程抛出线程，事务回滚，因为子线程是独立存在，和父线程不在同一个事务中，所以子线程的修改并不会被回滚，
<a name="RojrU"></a>
#### 子线程抛出异常
父线程不抛出异常，子线程抛出异常：
```java
public void testSuccess() throws Exception {
    Integer id = 1;
    MyUser user = query(id);
    System.out.println("原记录:" + user);
    update(id);
    throw new Exception("测试事务不生效");
}

@Transactional(rollbackFor = Exception.class)
public void testMultThread() throws Exception {
    new Thread(new Runnable() {
        @SneakyThrows
        @Override
        public void run() {
            testSuccess();
        }
    }).start();
}
```
由于子线程的异常不会被外部的线程捕获，所以父线程不抛异常，事务回滚没有生效。
<a name="ySf8V"></a>
## 源码解读
**下面从源码的角度，对 **`**@Transactional**`** 的执行机制和事务不生效的原因进行解读。**
<a name="lmgYs"></a>
### `@Transactional` 执行机制
只看最核心的逻辑，代码中的 `interceptorOrInterceptionAdvice` 就是 `TransactionInterceptor` 的实例，入参是 this 对象。<br />红色方框有一段注释，大致翻译为 “它是一个拦截器，所以只需调用即可：在构造此对象之前，将静态地计算切入点。”<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874619362-fbc6eae4-3f26-49ac-a621-4e9e6010f7b6.png#clientId=u9f547560-fee7-4&from=paste&id=uc0b573d3&originHeight=596&originWidth=787&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaca2092c-3c5d-4f39-ab61-139cf338097&title=)<br />this 是 `ReflectiveMethodInvocation` 对象，成员对象包含 `UserController` 类、`testSuccess()` 方法、入参和代理对象等。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874619368-5965ac1b-a49d-4c6e-905a-7c15c822f8b7.png#clientId=u9f547560-fee7-4&from=paste&id=u19ef9d45&originHeight=509&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2beba0fb-910a-4c7a-a986-eea84f51d0e&title=)<br />进入 `invoke()` 方法后：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874619356-35bc9922-3bc7-4f00-8e24-a0df28d53f21.png#clientId=u9f547560-fee7-4&from=paste&id=u5f26b5bb&originHeight=671&originWidth=1026&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u163afb8a-61ad-444a-a913-5ace67f2caf&title=)<br />**前方高能！！！这里就是事务的核心逻辑，包括判断事务是否开启、目标方法执行、事务回滚、事务提交。**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874619388-8a031e50-63e3-4359-a4b6-89e23b9a9b8b.png#clientId=u9f547560-fee7-4&from=paste&id=uaf2842c5&originHeight=939&originWidth=1048&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u12f52982-63a3-4e73-b207-8490328b9f7&title=)
<a name="dpa23"></a>
### `private` 导致事务不生效原因
在上面这幅图中，第一个红框区域调用了方法 `getTransactionAttribute()`，主要是为了获取 txAttr 变量，它是用于读取 `@Transactional` 的配置，如果这个 `txAttr = null`，后面就不会走事务逻辑，看一下这个变量的含义：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620149-6371c8c5-1793-4b84-8f52-b14a546e806d.png#clientId=u9f547560-fee7-4&from=paste&id=u56040105&originHeight=237&originWidth=806&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc421835c-c57c-45e1-bb91-6306d59372a&title=)<br />直接进入 `getTransactionAttribute()`，重点关注获取事务配置的方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620180-4e9176de-0dd6-458b-bad3-38d3e1c39bcd.png#clientId=u9f547560-fee7-4&from=paste&id=u3c5c38fa&originHeight=841&originWidth=964&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u67587fca-eb94-4cb3-ba4e-fb3abacd08b&title=)<br />**前方高能！！！这里就是 **`**private**`** 导致事务不生效的原因所在**，`allowPublicMethodsOnly()` 一直返回 false，所以重点只关注 `isPublic()` 方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620256-3509acde-2129-4eaa-bf02-e9666b0512a7.png#clientId=u9f547560-fee7-4&from=paste&id=uec251dcd&originHeight=316&originWidth=850&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf407b789-ec15-415e-9378-0980f00eb55&title=)<br />下面通过位与计算，判断是否为 Public，对应的几类修饰符如下：

- **PUBLIC: 1**
- **PRIVATE: 2**
- **PROTECTED: 4**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620201-222cceaa-07ad-4276-8c03-edf1325c4492.png#clientId=u9f547560-fee7-4&from=paste&id=uc0cf5abf&originHeight=127&originWidth=930&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud58a116a-e5fd-4cee-8abe-f591ad76e6b&title=)<br />看到这里，是不是豁然开朗了，有没有觉得很有意思呢~~
<a name="d32r3"></a>
### 异常不匹配原因
继续回到事务的核心逻辑，因为主方法抛出 `Exception()` 异常，进入事务回滚的逻辑：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620212-1489d779-fb31-49e6-83b6-3649edadabd1.png#clientId=u9f547560-fee7-4&from=paste&id=ub8708dba&originHeight=773&originWidth=836&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uae6a25a0-e52b-426c-93c0-9da389cd113&title=)<br />进入 `rollbackOn()` 方法，判断该异常是否能进行回滚，这个需要判断主方法抛出的 `Exception()` 异常，是否在 `@Transactional` 的配置中：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620580-19ff0fc6-06ae-4d35-8623-43cec27af557.png#clientId=u9f547560-fee7-4&from=paste&id=u3a0d06da&originHeight=662&originWidth=891&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubd0e3228-fe56-4871-a95f-e1bc12e2020&title=)<br />进入 `getDepth()` 看一下异常规则匹配逻辑，因为对 `@Transactional` 配置了 `rollbackFor = Exception.class`，所以能匹配成功：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620607-a516a763-2551-4200-9056-a3af6806264d.png#clientId=u9f547560-fee7-4&from=paste&id=u7d518607&originHeight=743&originWidth=683&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u551234b2-feff-4566-ac41-3cc95ffb646&title=)<br />示例中的 winner 不为 null，所以会跳过下面的环节。但是当 `winner = null` 时，也就是没有设置 `rollbackFor` 属性时，会走默认的异常捕获方式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620754-b7347aa4-699a-4073-be64-f535d23c7d0a.png#clientId=u9f547560-fee7-4&from=paste&id=u95e3c0ec&originHeight=122&originWidth=672&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue64c070d-ba0a-41e8-a823-2757141b42e&title=)<br />**前方高能！！！这里就是异常不匹配原因的原因所在**，看一下默认的异常捕获方式：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660874620816-950969df-9cd6-46a3-851e-9a8b47cdc5db.png#clientId=u9f547560-fee7-4&from=paste&id=u7ffa87bc&originHeight=112&originWidth=777&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaf772315-6078-4381-8214-cad0d27064c&title=)<br />是不是豁然开朗，**当没有设置 **`**rollbackFor**`** 属性时，默认只对 **`**RuntimeException**`** 和 Error 的异常执行回滚。**
