JavaSpring<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235268595-0ecce300-e168-449f-9fa3-86371660cc87.png#averageHue=%23fbfbfb&clientId=u650b6b37-1020-4&from=paste&id=uf2d7776a&originHeight=842&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua9aacc65-e976-4680-9265-ea41ac96789&title=)
<a name="t5V1Y"></a>
# 1、项目准备
**需要搭建环境的同学，代码详见**：[https://github.com/lml200701158/program_demo/tree/main/spring-transaction](https://github.com/lml200701158/program_demo/tree/main/spring-transaction)<br />下面是 DB 数据和 DB 操作接口：

| uid | uname | usex |
| --- | --- | --- |
| 1 | 张三 | 女 |
| 2 | 陈恒 | 男 |
| 3 | 楼仔 | 男 |

```java
// 提供的接口
public interface UserDao {
    // select * from user_test where uid = "#{uid}"
    public MyUser selectUserById(Integer uid);
    // update user_test set uname =#{uname},usex = #{usex} where uid = #{uid}
    public int updateUser(MyUser user);
}
```
基础测试代码，`testSuccess()` 是事务生效的情况：
```java
@Service
public class Louzai {
    @Autowired
    private UserDao userDao;

    public void update(Integer id) {
        MyUser user = new MyUser();
        user.setUid(id);
        user.setUname("张三-testing");
        user.setUsex("女");
        userDao.updateUser(user);
    }

    public MyUser query(Integer id) {
        MyUser user = userDao.selectUserById(id);
        return user;
    }

    // 正常情况
    @Transactional(rollbackFor = Exception.class)
    public void testSuccess() throws Exception {
        Integer id = 1;
        MyUser user = query(id);
        System.out.println("原记录:" + user);
        update(id);
        throw new Exception("事务生效");
    }
}
```
执行入口：
```java
public class SpringMyBatisTest {
    public static void main(String[] args) throws Exception {
        String xmlPath = "applicationContext.xml";
        ApplicationContext applicationContext = new ClassPathXmlApplicationContext(xmlPath);
        Louzai uc = (Louzai) applicationContext.getBean("louzai");
        uc.testSuccess();
    }
}
```
输出：
```
16:44:38.267 [main] DEBUG org.springframework.beans.factory.support.DefaultListableBeanFactory - Creating shared instance of singleton bean 'org.springframework.transaction.interceptor.TransactionInterceptor#0'
16:44:38.363 [main] DEBUG org.springframework.beans.factory.support.DefaultListableBeanFactory - Creating shared instance of singleton bean 'txManager'
16:44:40.966 [main] DEBUG org.springframework.jdbc.datasource.DataSourceTransactionManager - Creating new transaction with name [com.mybatis.controller.Louzai.testSuccess]: PROPAGATION_REQUIRED,ISOLATION_DEFAULT,-java.lang.Exception
16:44:40.968 [main] DEBUG org.springframework.jdbc.datasource.DriverManagerDataSource - Creating new JDBC DriverManager Connection to [jdbc:mysql://127.0.0.1:3306/java_study?useUnicode=true&characterEncoding=UTF-8&useSSL=false&serverTimezone=Asia/Shanghai]
16:44:41.228 [main] DEBUG org.springframework.jdbc.datasource.DataSourceTransactionManager - Acquired Connection [com.mysql.cj.jdbc.ConnectionImpl@5b5caf08] for JDBC transaction
16:44:41.231 [main] DEBUG org.springframework.jdbc.datasource.DataSourceTransactionManager - Switching JDBC Connection [com.mysql.cj.jdbc.ConnectionImpl@5b5caf08] to manual commit
原记录:MyUser(uid=1, uname=张三, usex=女)
16:42:59.345 [main] DEBUG org.springframework.jdbc.datasource.DataSourceTransactionManager - Initiating transaction rollback
16:42:59.346 [main] DEBUG org.springframework.jdbc.datasource.DataSourceTransactionManager - Rolling back JDBC transaction on Connection [com.mysql.cj.jdbc.ConnectionImpl@70807224]
16:42:59.354 [main] DEBUG org.springframework.jdbc.datasource.DataSourceTransactionManager - Releasing JDBC Connection [com.mysql.cj.jdbc.ConnectionImpl@70807224] after transaction
Exception in thread "main" java.lang.Exception: 事务生效
 at com.mybatis.controller.Louzai.testSuccess(Louzai.java:34)
  // 异常日志省略...
```
<a name="rB3Vp"></a>
# 2、Spring 事务工作流程
为了方便大家能更好看懂后面的源码，先整体介绍一下源码的执行流程，有一个整体的认识，否则容易被绕进去。<br />整个 Spring 事务源码，其实分为 2 块，会结合上面的示例，给大家进行讲解。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235268722-f14489ca-658a-4dba-b53c-ead65c67bb0f.png#averageHue=%23faf8f5&clientId=u650b6b37-1020-4&from=paste&id=ue42cf481&originHeight=1275&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u50565037-b22b-4896-9741-47d7dbbf22e&title=)<br />第一块是**后置处理**，在创建 Louzai Bean 的后置处理器中，里面会做两件事情：<br />**获取 Louzai 的切面方法**：首先会拿到所有的切面信息，和 Louzai 的所有方法进行匹配，然后找到 Louzai 所有需要进行事务处理的方法，匹配成功的方法，还需要将事务属性保存到缓存 attributeCache 中。<br />**创建 AOP 代理对象**：结合 Louzai 需要进行 AOP 的方法，选择 Cglib 或 JDK，创建 AOP 代理对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235268692-f521c39b-487e-44ab-a6f9-7a5013980500.png#averageHue=%23f8f5f2&clientId=u650b6b37-1020-4&from=paste&id=u9bf35cde&originHeight=1248&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucd772f7b-008f-4298-b684-14799bab3bf&title=)<br />第二块是**事务执行**，整个逻辑比较复杂，只选取 4 块最核心的逻辑，分别为**从缓存拿到事务属性、创建并开启事务、执行业务逻辑、提交或者回滚事务。**
<a name="BEMe8"></a>
# 3、源码解读
注意：Spring 的版本是 5.2.15.RELEASE！！！<br />上面的知识都不难，下面才是重头戏，走一遍代码流程。
<a name="bx3Uz"></a>
## 3.1 代码入口
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235268592-465ada22-9447-4bec-b53b-8560f49b53d5.png#averageHue=%232f2d2d&clientId=u650b6b37-1020-4&from=paste&id=ub9dd900d&originHeight=874&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u16120f90-b4fa-4282-9bbe-81776bf1931&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235268520-72defdf2-2ec3-459e-ba42-729ba182baa5.png#averageHue=%23302e2d&clientId=u650b6b37-1020-4&from=paste&id=u7804774f&originHeight=666&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7754e98a-2eed-4426-b6fb-38ccff1c15d&title=)<br />这里需要多跑几次，把前面的 beanName 跳过去，只看 louzai。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269064-ff86b731-70be-4d0c-9eb7-8bcf1fd05acb.png#averageHue=%232e2c2c&clientId=u650b6b37-1020-4&from=paste&id=u5bad428f&originHeight=800&originWidth=996&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ude6759db-1a6d-45c4-bd11-0a2128bc49b&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269188-e5fab484-d1df-48ba-9521-cf6383812227.png#averageHue=%238b6e3d&clientId=u650b6b37-1020-4&from=paste&id=u122e727c&originHeight=101&originWidth=860&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue222280b-2a2d-4099-bc4c-a5341da1991&title=)<br />进入 `doGetBean()`，进入创建 Bean 的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269450-b2a3c325-2360-47ee-a106-482d85056e68.png#averageHue=%23302e2d&clientId=u650b6b37-1020-4&from=paste&id=u87b3defe&originHeight=322&originWidth=950&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u26c06ee2-4207-49a6-9f65-e7a5a5b9c9e&title=)<br />进入 `createBean()`，调用 `doCreateBean()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269212-633f0550-f927-4715-8094-17bb7ead1c6a.png#averageHue=%232e2c2c&clientId=u650b6b37-1020-4&from=paste&id=u3a2e6bb4&originHeight=508&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2be7cb00-0cdc-482d-9c98-54826f133ad&title=)<br />进入 `doCreateBean()`，调用 `initializeBean()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269291-ce73a8e3-972e-427a-9ecd-67709358ca3a.png#averageHue=%232e2c2b&clientId=u650b6b37-1020-4&from=paste&id=ue4141fc5&originHeight=584&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f345d13-ac88-48be-8a4e-959fe13bcc0&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269501-ab35a93d-271c-4c25-8b1a-eea63d0d4313.png#averageHue=%232d2b2b&clientId=u650b6b37-1020-4&from=paste&id=u5e829685&originHeight=697&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u43fd0a4c-9ded-496b-9378-c808fa98277&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269632-88872698-fbfd-45be-afeb-c673c54a1c66.png#averageHue=%23302d2b&clientId=u650b6b37-1020-4&from=paste&id=u0e2fe5f8&originHeight=417&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u591fa42c-7c7f-456c-80d7-cc5f3a665e0&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269648-6fa7dfff-5312-4e7c-a5e2-c3f9358afe83.png#averageHue=%234c5f3e&clientId=u650b6b37-1020-4&from=paste&id=u5384b7f0&originHeight=643&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5519957-343f-4cb7-b8b8-e3378df4d1b&title=)<br />对这个入口应该会非常熟悉，其实就是用来**创建代理对象。**
<a name="uvyY5"></a>
## 3.2 创建代理对象
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269800-c2848f85-ee12-439a-a909-1d775fc24651.png#averageHue=%23f9f7f5&clientId=u650b6b37-1020-4&from=paste&id=u34efe914&originHeight=1014&originWidth=1022&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u90fafd43-059d-4e46-8959-6f1f8252e75&title=)<br />**这里是重点！敲黑板！！！**

1. 先获取 louzai 类的所有切面列表；
2. 创建一个 AOP 的代理对象。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269863-0ccc432f-b3af-4cfe-bd06-5f2d09b8c8de.png#averageHue=%23755e3f&clientId=u650b6b37-1020-4&from=paste&id=u745a9150&originHeight=746&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u144fbbd3-9d8a-46dc-94b9-0d566b15295&title=)
<a name="PiCsm"></a>
### 3.2.1 获取切面列表
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269943-1e6a5bd9-e249-47b2-8936-178dd4226613.png#averageHue=%232f2d2b&clientId=u650b6b37-1020-4&from=paste&id=u4ce3a724&originHeight=315&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubdae9526-71b3-4558-a8ce-91edc63c82a&title=)<br />这里有 2 个重要的方法，先执行 `findCandidateAdvisors()`，待会还会再返回 `findEligibleAdvisors()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235269956-c3b78663-e13c-4616-8b66-3fd9cf0dff09.png#averageHue=%2359633b&clientId=u650b6b37-1020-4&from=paste&id=uf6c846a2&originHeight=724&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u38c9f4c2-38bb-4b7b-83c9-7efdd7d2205&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270176-2e759e2c-d422-4cb5-ba8e-c9856dbb8cff.png#averageHue=%23312e2c&clientId=u650b6b37-1020-4&from=paste&id=u11498166&originHeight=298&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u48ecea6e-3ba4-47db-a07a-2880e0e2723&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270241-278b09e4-728f-4237-b48f-39ea5a84e91d.png#averageHue=%23312d2b&clientId=u650b6b37-1020-4&from=paste&id=ub18793bd&originHeight=245&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uce32c303-7ff3-42fd-9bd0-1415be8ba50&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270348-eeb88aed-ed72-4b9b-acfe-058cac90e5fb.png#averageHue=%2353634a&clientId=u650b6b37-1020-4&from=paste&id=u62796cf7&originHeight=866&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8fabf7ab-ff10-4206-9187-dfe1f5b740f&title=)<br />依次返回，重新来到 `findEligibleAdvisors()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270341-447b3316-395f-4d4e-818c-0c5fac354a8d.png#averageHue=%2353634d&clientId=u650b6b37-1020-4&from=paste&id=u078d0459&originHeight=596&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub7355184-695b-4f4e-830d-868e06d4c28&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270412-301427d3-6f20-443e-8cc6-786a3aa749fa.png#averageHue=%232f2c2b&clientId=u650b6b37-1020-4&from=paste&id=u01afb036&originHeight=298&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u18afaff7-ec63-4ce6-932f-5acea0db006&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270495-ca683b1e-f42c-4661-9e1d-ccf5364e2347.png#averageHue=%232d2c2b&clientId=u650b6b37-1020-4&from=paste&id=u5526fe77&originHeight=572&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2d9206b2-6877-4beb-bb11-eaa636b8233&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270574-4378eb28-569b-4961-9f24-95a926179d97.png#averageHue=%23302c2b&clientId=u650b6b37-1020-4&from=paste&id=u1a0b71f4&originHeight=368&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0eb3f5fc-4135-4878-b7c9-188b58da41b&title=)<br />进入 `canApply()`，开始匹配 louzai 的切面。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270887-e73218f0-7fe9-4c27-be25-a080c12ec15d.png#averageHue=%23785a36&clientId=u650b6b37-1020-4&from=paste&id=u2736f81f&originHeight=683&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5cbaa373-1ff1-4bdb-b39f-ceb0b268f13&title=)<br />**这里是重点！敲黑板！！！**<br />这里只会匹配到 `Louzai.testSuccess()` 方法，直接进入匹配逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270783-a515b773-e609-47aa-8263-cc900d6fa9eb.png#averageHue=%237b7c7c&clientId=u650b6b37-1020-4&from=paste&id=ue7573027&originHeight=349&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufdec3290-9b68-4357-b2e0-848383550aa&title=)<br />如果匹配成功，还会把事务的属性配置信息放入 `attributeCache` 缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270892-17a8d300-9d95-47c0-a94d-6cc5a180666c.png#averageHue=%23312d2c&clientId=u650b6b37-1020-4&from=paste&id=u037f081c&originHeight=721&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u544cc7b9-4fc3-4d50-a88e-777158b37aa&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235270953-75265de2-00ca-41de-bf8d-e304232f26f3.png#averageHue=%23302d2c&clientId=u650b6b37-1020-4&from=paste&id=u30132e52&originHeight=437&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u573eda94-b99e-44a7-9112-992d2ff69e1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271123-85b453f2-dc38-47c1-9231-319a4403ae1b.png#averageHue=%238b723b&clientId=u650b6b37-1020-4&from=paste&id=ue943e206&originHeight=177&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7660f40f-e9f0-470b-9e35-f3a8afde135&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271452-55cf4adf-adbe-4bae-919c-d0a23ff20ecf.png#averageHue=%232d2c2c&clientId=u650b6b37-1020-4&from=paste&id=u83753d7a&originHeight=510&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u20d89000-f2ec-4924-bf00-e82ab2cd0e6&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271658-fffe22ce-7169-417b-b2d0-358096402d58.png#averageHue=%2352634d&clientId=u650b6b37-1020-4&from=paste&id=ubdb44515&originHeight=716&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u22093eca-7a8b-41bc-be34-9f12a7d246f&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271572-bb3617db-3c10-4017-aba7-cd5b99cc1f54.png#averageHue=%23706139&clientId=u650b6b37-1020-4&from=paste&id=u2985127b&originHeight=728&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u241c6825-909a-4a42-a3e0-38c4db61bd5&title=)<br />依次返回到 `getTransactionAttribute()`，再看看放入缓存中的数据。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271772-b5ab0678-9fc9-476f-a2f6-823f55858086.png#averageHue=%234f5f48&clientId=u650b6b37-1020-4&from=paste&id=u69c220e5&originHeight=837&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u41baca29-b84f-4357-8ffa-9e332d39e2d&title=)<br />再回到该小节开头，拿到 louzai 的切面信息，去创建 AOP 代理对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271755-546f8b5e-aaf6-474c-8de0-5a8e238e2270.png#averageHue=%23755e3f&clientId=u650b6b37-1020-4&from=paste&id=ua14f5443&originHeight=746&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u42490874-1d5d-41ea-a977-71eba552af6&title=)
<a name="ID2fy"></a>
### 3.2.2 创建 AOP 代理对象
创建 AOP 代理对象的逻辑，这里是通过 Cglib 创建。
<a name="v1dDX"></a>
## 3.3 事务执行
回到业务逻辑，通过 **louzai 的 AOP 代理对象**，开始执行主方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235271995-3ce03d8a-6b66-40fa-9c60-9e841f227ef9.png#averageHue=%232f2d2b&clientId=u650b6b37-1020-4&from=paste&id=uf942f4e6&originHeight=285&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u949689e5-004c-4774-9228-a3726cb11c1&title=)<br />因为代理对象是 Cglib 方式创建，所以通过 Cglib 来执行。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272034-b5710e66-f23c-46e7-8e6f-b143f3c81b36.png#averageHue=%232f2d2c&clientId=u650b6b37-1020-4&from=paste&id=u9e7e7229&originHeight=773&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua22de846-c931-4467-ba5b-f14f1421376&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272316-fb288075-0b6b-43cd-b5f7-5481a3573932.png#averageHue=%232e2c2b&clientId=u650b6b37-1020-4&from=paste&id=ua8821f25&originHeight=297&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub8690048-0ff3-4bcd-9804-6ac0a2e8828&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272345-fd2821d0-ce9a-4b34-9f25-ecd992705f89.png#averageHue=%232e2d2c&clientId=u650b6b37-1020-4&from=paste&id=u10710635&originHeight=730&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b648797-3e21-44f0-9dd0-42f8f3e7541&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272371-5eab0827-b1e0-43cc-aec5-19913550ce82.png#averageHue=%23322e2d&clientId=u650b6b37-1020-4&from=paste&id=ucc809ff8&originHeight=311&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u13cf573e-17e3-4ede-a7bc-753a5f84767&title=)<br />**这里是重点！敲黑板！！！**<br />下面的代码是事务执行的核心逻辑 `invokeWithinTransaction()`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272407-4ef2fa08-daf4-4039-b39d-57cc86d39785.png#averageHue=%23f8f5f2&clientId=u650b6b37-1020-4&from=paste&id=u30df5004&originHeight=1086&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7d942e6f-24b1-4d76-ab45-bdb6e0a3035&title=)
```java
protected Object invokeWithinTransaction(Method method, @Nullable Class<?> targetClass,
                                         final InvocationCallback invocation) throws Throwable {

    //获取我们的事务属源对象
    TransactionAttributeSource tas = getTransactionAttributeSource();
    //通过事务属性源对象获取到我们的事务属性信息
    final TransactionAttribute txAttr = (tas != null ? tas.getTransactionAttribute(method, targetClass) : null);
    //获取我们配置的事务管理器对象
    final PlatformTransactionManager tm = determineTransactionManager(txAttr);
    //从tx属性对象中获取出标注了@Transactionl的方法描述符
    final String joinpointIdentification = methodIdentification(method, targetClass, txAttr);

    //处理声明式事务
    if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {
        //有没有必要创建事务
        TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);

        Object retVal;
        try {
            //调用钩子函数进行回调目标方法
            retVal = invocation.proceedWithInvocation();
        }
        catch (Throwable ex) {
            //抛出异常进行回滚处理
            completeTransactionAfterThrowing(txInfo, ex);
            throw ex;
        }
        finally {
            //清空我们的线程变量中transactionInfo的值
            cleanupTransactionInfo(txInfo);
        }
        //提交事务
        commitTransactionAfterReturning(txInfo);
        return retVal;
    }
        //编程式事务
    else {
        // 这里不是我们的重点，省略...
    }
}
```
<a name="Zcm5u"></a>
## 3.3.1 获取事务属性
在 `invokeWithinTransaction()` 中，找到获取事务属性的入口。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272544-1a47318b-0cce-49bb-93c4-25bfd5785587.png#averageHue=%23302d2b&clientId=u650b6b37-1020-4&from=paste&id=u3e7caeb6&originHeight=275&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3360f73e-1acf-4811-9d94-c54b1761fe3&title=)<br />从 `attributeCache` 获取事务的缓存数据，缓存数据是在 “3.2.1 获取切面列表” 中保存的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272634-664ea33f-7b6c-4f9d-9db1-ca62bc6a247e.png#averageHue=%2380663b&clientId=u650b6b37-1020-4&from=paste&id=u835c31e3&originHeight=465&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub39035f3-c222-44fb-99f6-9fc0a7cffd2&title=)
<a name="Kx62K"></a>
## 3.3.2 创建事务
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272796-6e3e1f77-d534-49a0-bcb3-fd740589522d.png#averageHue=%23302c2b&clientId=u650b6b37-1020-4&from=paste&id=u68e31e03&originHeight=601&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u73049e2f-5164-4d39-b525-4feaed3abe1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272887-35be44f4-6124-492c-9e99-bbe654a75f51.png#averageHue=%23322d2b&clientId=u650b6b37-1020-4&from=paste&id=uf0f8bb43&originHeight=738&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue12159d3-0900-4896-8dff-c27c40b8ff7&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235272869-dc1141d8-d6b7-4bab-90a9-ff6839663fa8.png#averageHue=%232f2d2c&clientId=u650b6b37-1020-4&from=paste&id=u44146bed&originHeight=951&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9f49cbce-6acf-44ce-a29a-a78dede476e&title=)<br />通过 `doGetTransaction()` 获取事务。
```java
protected Object doGetTransaction() {
    //创建一个数据源事务对象
    DataSourceTransactionObject txObject = new DataSourceTransactionObject();
    //是否允许当前事务设置保持点
    txObject.setSavepointAllowed(isNestedTransactionAllowed());
    /**
     * TransactionSynchronizationManager 事务同步管理器对象(该类中都是局部线程变量)
     * 用来保存当前事务的信息,我们第一次从这里去线程变量中获取 事务连接持有器对象 通过数据源为key去获取
     * 由于第一次进来开始事务 我们的事务同步管理器中没有被存放.所以此时获取出来的conHolder为null
     */
    ConnectionHolder conHolder =
            (ConnectionHolder) TransactionSynchronizationManager.getResource(obtainDataSource());
    txObject.setConnectionHolder(conHolder, false);
    //返回事务对象
    return txObject;
}
```
通过 `startTransaction()` 开启事务。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273300-30a55c76-a79f-4688-a3f0-afe553a46146.png#averageHue=%232c2b2b&clientId=u650b6b37-1020-4&from=paste&id=ufe72a219&originHeight=388&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u262a6b49-ed3b-4710-922e-e8c48bfa1cf&title=)<br />下面是开启事务的详细逻辑，了解一下即可。
```java
protected void doBegin(Object transaction, TransactionDefinition definition) {
    //强制转化事务对象
    DataSourceTransactionObject txObject = (DataSourceTransactionObject) transaction;
    Connection con = null;

    try {
        //判断事务对象没有数据库连接持有器
        if (!txObject.hasConnectionHolder() ||
            txObject.getConnectionHolder().isSynchronizedWithTransaction()) {
            //通过数据源获取一个数据库连接对象
            Connection newCon = obtainDataSource().getConnection();
            if (logger.isDebugEnabled()) {
                logger.debug("Acquired Connection [" + newCon + "] for JDBC transaction");
            }
            //把我们的数据库连接包装成一个ConnectionHolder对象 然后设置到我们的txObject对象中去
            txObject.setConnectionHolder(new ConnectionHolder(newCon), true);
        }

        //标记当前的连接是一个同步事务
        txObject.getConnectionHolder().setSynchronizedWithTransaction(true);
        con = txObject.getConnectionHolder().getConnection();

        //为当前的事务设置隔离级别
        Integer previousIsolationLevel = DataSourceUtils.prepareConnectionForTransaction(con, definition);
        txObject.setPreviousIsolationLevel(previousIsolationLevel);

        //关闭自动提交
        if (con.getAutoCommit()) {
            txObject.setMustRestoreAutoCommit(true);
            if (logger.isDebugEnabled()) {
                logger.debug("Switching JDBC Connection [" + con + "] to manual commit");
            }
            con.setAutoCommit(false);
        }

        //判断事务为只读事务
        prepareTransactionalConnection(con, definition);
        //设置事务激活
        txObject.getConnectionHolder().setTransactionActive(true);

        //设置事务超时时间
        int timeout = determineTimeout(definition);
        if (timeout != TransactionDefinition.TIMEOUT_DEFAULT) {
            txObject.getConnectionHolder().setTimeoutInSeconds(timeout);
        }

        // 绑定我们的数据源和连接到我们的同步管理器上   把数据源作为key,数据库连接作为value 设置到线程变量中
        if (txObject.isNewConnectionHolder()) {
            TransactionSynchronizationManager.bindResource(obtainDataSource(), txObject.getConnectionHolder());
        }
    }

    catch (Throwable ex) {
        if (txObject.isNewConnectionHolder()) {
            //释放数据库连接
            DataSourceUtils.releaseConnection(con, obtainDataSource());
            txObject.setConnectionHolder(null, false);
        }
        throw new CannotCreateTransactionException("Could not open JDBC Connection for transaction", ex);
    }
}
```
最后返回到 `invokeWithinTransaction()`，得到 `txInfo` 对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273300-422a2062-624a-41b0-bd9f-b6711cb251b5.png#averageHue=%2353634d&clientId=u650b6b37-1020-4&from=paste&id=uad40659a&originHeight=804&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud36951cd-632a-4c24-a7c6-e1bfec195e7&title=)
<a name="JAmch"></a>
## 3.3.3 执行逻辑
还是在 `invokeWithinTransaction()` 中，开始执行业务逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273396-b1b4f9e0-29c1-4684-bcf2-0d17df7d1786.png#averageHue=%23312d2c&clientId=u650b6b37-1020-4&from=paste&id=u4533e497&originHeight=309&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uab0ab31b-1a5d-40ac-8844-489b7b2d110&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273513-c0eb9d3c-d2ff-4bfe-8c7a-259e0d3420b2.png#averageHue=%232e2c2b&clientId=u650b6b37-1020-4&from=paste&id=ubfdc2107&originHeight=373&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u75b03773-7746-42b2-a06e-8d5ddc6d9ea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273457-31035315-e301-41d6-901d-a098d0dc7a12.png#averageHue=%232e2d2c&clientId=u650b6b37-1020-4&from=paste&id=u10caa309&originHeight=233&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u063afe7c-36f9-433e-a8cf-115acd4cec3&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273942-7a223a59-677f-44c6-9e0a-fa062ab013c1.png#averageHue=%232d2c2b&clientId=u650b6b37-1020-4&from=paste&id=u860e9dbf&originHeight=316&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7333bc65-5c75-4f23-a746-161f724d446&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273911-50fb5c8c-adf3-47e8-9397-d08e86cafd6f.png#averageHue=%232c2b2b&clientId=u650b6b37-1020-4&from=paste&id=ufe5fa231&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u11f07878-a9a2-417e-82d8-fda8e31ac45&title=)<br />进入到真正的业务逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273995-1e3bc510-780d-4977-ae66-bd0c2bdd9c3e.png#averageHue=%232f2c2a&clientId=u650b6b37-1020-4&from=paste&id=u1f869c30&originHeight=296&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc569cd31-16e4-4fe5-97da-095bb31ebb0&title=)<br />执行完毕后抛出异常，依次返回，走后续的回滚事务逻辑。
<a name="H1At8"></a>
## 3.3.4 回滚事务
还是在 `invokeWithinTransaction()` 中，进入回滚事务的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235273939-6eeb65cb-90ac-4350-851f-7f5a0fb0086b.png#averageHue=%232f2c2c&clientId=u650b6b37-1020-4&from=paste&id=eAst3&originHeight=496&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u33767fa7-3d30-4600-96ef-23a902f0637&title=)。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235274135-432937ca-35ff-4783-bb96-4db392d9eb44.png#averageHue=%23322d2b&clientId=u650b6b37-1020-4&from=paste&id=u3e5ea573&originHeight=456&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u43b7e66e-5ec7-41ea-b3bf-26fef7fe6b4&title=)<br />执行回滚逻辑很简单，只看如何判断是否回滚。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235274321-d6d2fded-7397-4243-82d5-ee3126a58aac.png#averageHue=%238e6c38&clientId=u650b6b37-1020-4&from=paste&id=u5d994b28&originHeight=150&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3cd28911-6430-4c1e-a262-80eecf5b057&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235274324-4ef3c01f-31e5-43b5-80e7-de9ad8b4cd5a.png#averageHue=%232f2c2b&clientId=u650b6b37-1020-4&from=paste&id=u617aa750&originHeight=890&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uea1440e5-83d3-4301-a898-f1c993dfa77&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235274322-9cb8212c-6077-48bb-8a79-4dae9beae905.png#averageHue=%232c2b2b&clientId=u650b6b37-1020-4&from=paste&id=u96ed3949&originHeight=356&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud002a7fd-05ca-4eaf-8b96-a4283280b94&title=)<br />**如果抛出的异常类型，和事务定义的异常类型匹配，证明该异常需要捕获。**<br />之所以用递归，不仅需要判断抛出异常的本身，还需要判断它继承的父类异常，满足任意一个即可捕获。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668235274427-c4989df6-dffa-4756-ad16-66111d90b642.png#averageHue=%23586751&clientId=u650b6b37-1020-4&from=paste&id=u0ca0c185&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc19a77c8-4bd9-46bb-9a35-5d6df4df038&title=)<br />到这里，所有的流程结束。
<a name="vlSwi"></a>
# 4、结语
小节一下，先介绍了事务的使用示例，以及事务的执行流程。<br />之后再剖析了事务的源码，分为 2 块：

- 先匹配出 louzai 对象所有关于事务的切面列表，并将匹配成功的事务属性保存到缓存；
- 从缓存取出事务属性，然后创建、启动事务，执行业务逻辑，最后提交或者回滚事务。
