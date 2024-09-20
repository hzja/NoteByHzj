JavaSpring<br />Spring 事务是复杂一致性业务必备的知识点，掌握好 Spring 事务可以写出更好地代码。这里介绍 Spring 事务的诞生背景，从而可以更清晰地了解 Spring 事务存在的意义。<br />接着，会介绍如何快速使用 Spring 事务。接着，会介绍 Spring 事务的一些特性，从而更好地使用 Spring 事务。最后，会总结一些 Spring 事务常见的问题，避免大家踩坑。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661130266317-51e52795-a80c-4d92-a6e5-f50ee946f5c2.jpeg)
<a name="lAQEb"></a>
## 诞生背景
当聊起事务的时候，需要明白「事务」这个词代表着什么。<br />事务其实是一个并发控制单位，是用户定义的一个操作序列，这些操作要么全部完成，要不全部不完成，是一个不可分割的工作单位。事务有 ACID 四个特性，即：

1. **Atomicity（原子性）：事务中的所有操作，或者全部完成，或者全部不完成，不会结束在中间某个环节。**
2. **一致性（Consistency）：在事务开始之前和事务结束以后，数据库的完整性没有被破坏。**
3. **事务隔离（Isolation）：多个事务之间是独立的，不相互影响的。**
4. **持久性（Durability）：事务处理结束后，对数据的修改就是永久的，即便系统故障也不会丢失。**

而 Spring 事务，其实是事务在 Spring 中的实现。<br />明白了什么是事务之后，来聊聊：**为什么要有 Spring 事务？**<br />为了解释清楚这个问题，举个简单的例子：银行里叔叔要给小黑转 1000 块钱，这时候会有两个必要的操作：

1. **将叔叔的账户余额减少 1000 元。**
2. **将小黑的账户余额增加 1000 元。**

这两个操作，要么一起都完成，要么都不完成。如果其中某个成功，另外一个失败，那么就会出现严重的问题。**而要保证这个操作的原子性，就必须通过 Spring 事务来完成，这就是 Spring 事务存在的原因。**<br />如果深入了解过 MySQL 事务，那么应该知道：MySQL 默认情况下，对于所有的单条语句都作为一个单独的事务来执行。要使用 MySQL 事务的时候，可以通过手动提交事务来控制事务范围。**Spring 事务的本质，其实就是通过 Spring AOP 切面技术，在合适的地方开启事务，接着在合适的地方提交事务或回滚事务，从而实现了业务编程层面的事务操作。**
<a name="rpFg6"></a>
## 使用指南
Spring 事务支持两种使用方式，分别是：声明式事务（注解方式）、编程式事务（代码方式）。一般来说，使用声明式事务比较多，这里就演示声明式事务的使用方法。
<a name="Uw71N"></a>
### 项目准备
为了较好地进行讲解，需要搭建一个具备数据库 CURD 功能的项目，并创建 tablea 和 tableb 两张表。<br />首先，创建 tablea 和 tableb 两张表，两张表都只有 id 和 name 两列，建表语句如下图所示。
```sql
CREATE TABLE `tablea` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1;

CREATE TABLE `tableb` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1;
```
接着，创建一个 SpringBoot 项目，随后加入 MyBatis 及 MySQL 的 POM 依赖。
```xml
<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
</dependency>
<dependency>
  <groupId>org.mybatis.spring.boot</groupId>
  <artifactId>mybatis-spring-boot-starter</artifactId>
  <version>2.1.0</version>
</dependency>
```
最后，创建对应的 controller 接口、service 接口、mapper 接口，代码如下所示。<br />创建 controller 接口：
```java
@SpringBootApplication
@RestController
@RequestMapping("/api")
public class SpringTransactionController {

    @Autowired
    private TransactionServiceA transactionServiceA;

    @RequestMapping("/spring-transaction")
    public String testTransaction() {
        transactionServiceA.methodA();
        return "SUCCESS";
    }
}
```
创建 TableService 接口。
```java
public interface TableService {
    void insertTableA(TableEntity tableEntity);
    void insertTableB(TableEntity tableEntity);
}
```
创建 Service 接口实现类 TransactionServiceA 类，在 `methodA()` 方法中先往 tablea 表格插入一条数据，随后会调用 TransactionServiceB 服务的 `methodB()` 方法。
```java
@Service
public class TransactionServiceA {

    @Autowired
    private TableService tableService;

    @Autowired
    private TransactionServiceB transactionServiceB;

    public void methodA(){
        System.out.println("methodA");
        tableService.insertTableA(new TableEntity());
        transactionServiceB.methodB();
    }
}
```
创建 TransactionServiceB 类实现，在 `methodB()` 方法中往 tableb 表格插入一条数据。
```java
@Service
public class TransactionServiceB {

    @Autowired
    private TableService tableService;

    public void methodB(){
        System.out.println("methodB");
        tableService.insertTableB(new TableEntity());
    }
}
```
创建 Mapper 接口方法：
```java
@Mapper
public interface TableMapper {
    @Insert("INSERT INTO tablea(id, name) " +
            "VALUES(#{id}, #{name})")
    @Options(useGeneratedKeys = true, keyProperty = "id")
    void insertTableA(TableEntity tableEntity);

    @Insert("INSERT INTO tableb(id, name) " +
            "VALUES(#{id}, #{name})")
    @Options(useGeneratedKeys = true, keyProperty = "id")
    void insertTableB(TableEntity tableEntity);
}
```
数据库表对应的 TableEntity：
```java
@Data
public class TableEntity {
    private static final long serialVersionUID = 1L;

    private Long id;

    private String name;

    public TableEntity() {
    }

    public TableEntity(String name) {
        this.name = name;
    }
}
```
最后，在配置文件中配置好数据库地址：
```yaml
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/test?serverTimezone=UTC&useUnicode=true&characterEncoding=utf-8&useSSL=true
    username: root
    password: root
    driver-class-name: com.mysql.cj.jdbc.Driver
# MyBatis 配置
mybatis:
  type-aliases-package: tech.shuyi.javacodechip.spring_transaction.model
  configuration:
    map-underscore-to-camel-case: true
```
最后，运行 SpringBoot 项目。通过浏览器访问地址：localhost:8080/api/spring-transaction，正常的话应该是接口请求成功。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661129113726-8bf9d6ab-2bfe-4cf3-977a-e994e24bcd6a.jpeg#clientId=uf5456370-d68d-4&from=paste&id=u673623f6&originHeight=142&originWidth=471&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u716573f9-e5d6-429d-8d55-219c6bde34d&title=)<br />查看数据库表，会看到 tablea 和 tableb 都插入了一条数据。<br />**到这里，用于测试 Spring 事务的 Demo 就准备完毕了！**
<a name="EzNzH"></a>
### 快速入门
使用声明式事务的方法很简单，其实就是在 Service 层对应方法上配置 `@Transactional` 注解即可。<br />假设业务需求是：往 tablea 和 tableb 插入的数据，要么都完成，要么都不完成。<br />这时候，应该怎么操作呢？<br />首先，需要在 TransactionServiceA 类的 `methodA()` 方法上配置 `@Transactional` 注解，同时也在 TransactionServiceB 类的 `methodB()` 方法上配置 `@Transactional` 注解。修改之后的 TransactionServiceA 和 TransactionServiceB 代码如下所示。
```java
// TransactionServiceA
@Transactional
public void methodA(){
    System.out.println("methodA");
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
}

// TransactionServiceB
@Transactional
public void methodB(){
    System.out.println("methodB");
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
可以看到，在 `methodB()` 中模拟了业务异常，看看是否 tablea 和 tableb 都没有插入数据。<br />修改之后重新启动项目，此时继续访问地址：localhost:8080/api/spring-transaction，会发现执行错误，并且控制台也报错了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661129113737-dfbd7467-69f7-4d1d-9549-17d0e091b413.jpeg#clientId=uf5456370-d68d-4&from=paste&id=u09cd90a7&originHeight=195&originWidth=682&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubf2dbb66-b41d-4ee2-befe-ddc6d6a612b&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661129113776-55bdfd68-67bc-48b1-a133-517c07db4774.jpeg#clientId=uf5456370-d68d-4&from=paste&id=u54a55fbe&originHeight=229&originWidth=788&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud763dae6-1647-4672-a8bf-5440a59065a&title=)<br />这时候查看数据库，会发现 tablea 和 tableb 都没有插入数据。这说明事务起作用了。
<a name="wcExH"></a>
## 事务传播类型
事务传播类型，指的是事务与事务之间的交互策略。例如：在事务方法 A 中调用事务方法 B，当事务方法 B 失败回滚时，事务方法 A 应该如何操作？这就是事务传播类型。Spring 事务中定义了 7 种事务传播类型，分别是：REQUIRED、SUPPORTS、MANDATORY、REQUIRES_NEW、NOT_SUPPORTED、NEVER、NESTED。其中最常用的只有 3 种，即：REQUIRED、REQUIRES_NEW、NESTED。<br />针对事务传播类型，要弄明白的是 4 个点：

1. **子事务与父事务的关系，是否会启动一个新的事务？**
2. **子事务异常时，父事务是否会回滚？**
3. **父事务异常时，子事务是否会回滚？**
4. **父事务捕捉异常后，父事务是否还会回滚？**
<a name="h2540"></a>
### `REQUIRED`
REQUIRED 是 Spring 默认的事务传播类型，该传播类型的特点是：**当前方法存在事务时，子方法加入该事务。此时父子方法共用一个事务，无论父子方法哪个发生异常回滚，整个事务都回滚。即使父方法捕捉了异常，也是会回滚。而当前方法不存在事务时，子方法新建一个事务。** 为了验证 REQUIRED 事务传播类型的特点，来做几个测试。<br />还是上面 methodA 和 methodB 的例子。当 methodA 不开启事务，methodB 开启事务，这时候 methodB 就是独立的事务，而 methodA 并不在事务之中。因此当 methodB 发生异常回滚时，methodA 中的内容就不会被回滚。用如下的代码就可以验证我们所说的。
```java
public void methodA(){
    System.out.println("methodA");
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
}

@Transactional
public void methodB(){
    System.out.println("methodB");
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
最终的结果是：tablea 插入了数据，tableb 没有插入数据，符合了我们的猜想。<br />当 methodA 开启事务，methodB 也开启事务。按照结论，此时 methodB 会加入 methodA 的事务。此时，验证当父子事务分别回滚时，另外一个事务是否会回滚。<br />先验证第一个：当父方法事务回滚时，子方法事务是否会回滚？
```java
@Transactional
public void methodA(){
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
    throw new RuntimeException();
}

@Transactional
public void methodB(){
    tableService.insertTableB(new TableEntity());
}
```
结果是：talbea 和 tableb 都没有插入数据，即：父事务回滚时，子事务也回滚了。<br />继续验证第二个：当子方法事务回滚时，父方法事务是否会回滚？
```java
@Transactional
public void methodA(){
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
}

@Transactional
public void methodB(){
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
结果是：talbea 和 tableb 都没有插入数据，即：子事务回滚时，父事务也回滚了。<br />继续验证第三个：当字方法事务回滚时，父方法捕捉了异常，父方法事务是否会回滚？
```java
@Transactional
public void methodA() {
    tableService.insertTableA(new TableEntity());
    try {
        transactionServiceB.methodB();
    } catch (Exception e) {
        System.out.println("methodb occur exp.");
    }
}
    
@Transactional
public void methodB() {
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
结果是：talbea 和 tableb 都没有插入数据，即：子事务回滚时，父事务也回滚了。所以说，这也进一步验证了之前所说的：REQUIRED 传播类型，它是父子方法共用同一个事务的。
<a name="rsoIP"></a>
### `REQUIRES_NEW`
REQUIRES_NEW 也是常用的一个传播类型，该传播类型的特点是：**无论当前方法是否存在事务，子方法都新建一个事务。此时父子方法的事务时独立的，它们都不会相互影响。但父方法需要注意子方法抛出的异常，避免因子方法抛出异常，而导致父方法回滚。** 为了验证 REQUIRES_NEW 事务传播类型的特点，来做几个测试。<br />首先，来验证一下：当父方法事务发生异常时，子方法事务是否会回滚？
```java
@Transactional
public void methodA(){
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
    throw new RuntimeException();
}

@Transactional(propagation = Propagation.REQUIRES_NEW)
public void methodB(){
    tableService.insertTableB(new TableEntity());
}
```
结果是：tablea 没有插入数据，tableb 插入了数据，即：父方法事务回滚了，但子方法事务没回滚。这可以证明父子方法的事务是独立的，不相互影响。<br />下面，来看看：当子方法事务发生异常时，父方法事务是否会回滚？
```java
@Transactional
public void methodA(){
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
}

@Transactional(propagation = Propagation.REQUIRES_NEW)
public void methodB(){
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
结果是：tablea 没有插入了数据，tableb 没有插入数据。<br />从这个结果来看，貌似是子方法事务回滚，导致父方法事务也回滚了。但不是说父子事务都是独立的，不会相互影响么？怎么结果与此相反呢？<br />其实是因为子方法抛出了异常，而父方法并没有做异常捕捉，此时父方法同时也抛出异常了，于是 Spring 就会将父方法事务也回滚了。如果在父方法中捕捉异常，那么父方法的事务就不会回滚了，修改之后的代码如下所示。
```java
@Transactional
public void methodA(){
    tableService.insertTableA(new TableEntity());
    // 捕捉异常
    try {
        transactionServiceB.methodB();
    } catch (Exception e) {
        e.printStackTrace();
    }
}
@Transactional(propagation = Propagation.REQUIRES_NEW)
public void methodB(){
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
结果是：tablea 插入了数据，tableb 没有插入数据。这正符合刚刚所说的：父子事务是独立的，并不会相互影响。<br />这其实就是上面所说的：父方法需要注意子方法抛出的异常，避免因子方法抛出异常，而导致父方法回滚。因为如果执行过程中发生 RuntimeException 异常和 Error 的话，那么 Spring 事务是会自动回滚的。
<a name="ohXcb"></a>
### `NESTED`
NESTED 也是常用的一个传播类型，该方法的特性与 REQUIRED 非常相似，其特性是：**当前方法存在事务时，子方法加入在嵌套事务执行。当父方法事务回滚时，子方法事务也跟着回滚。当子方法事务发送回滚时，父事务是否回滚取决于是否捕捉了异常。如果捕捉了异常，那么就不回滚，否则回滚。**<br />可以看到 NESTED 与 REQUIRED 的区别在于：父方法与子方法对于共用事务的描述是不一样的，REQUIRED 说的是共用同一个事务，而 NESTED 说的是在嵌套事务执行。这一个区别的具体体现是：**在子方法事务发生异常回滚时，父方法有着不同的反应动作。**<br />对于 REQUIRED 来说，无论父子方法哪个发生异常，全都会回滚。而 REQUIRED 则是：父方法发生异常回滚时，子方法事务会回滚。而子方法事务发送回滚时，父事务是否回滚取决于是否捕捉了异常。<br />为了验证 NESTED 事务传播类型的特点，来做几个测试。<br />首先，来验证一下：当父方法事务发生异常时，子方法事务是否会回滚？
```java
@Transactional
public void methodA() {
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
    throw new RuntimeException();
}
@Transactional(propagation = Propagation.NESTED)
public void methodB() {
    tableService.insertTableB(new TableEntity());
}
```
结果是：tablea 和 tableb 都没有插入数据，即：父子方法事务都回滚了。这说明父方法发送异常时，子方法事务会回滚。<br />接着，继续验证一下：当子方法事务发生异常时，如果父方法没有捕捉异常，父方法事务是否会回滚？
```java
@Transactional
public void methodA() {
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
}
@Transactional(propagation = Propagation.NESTED)
public void methodB() {
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
结果是：tablea 和 tableb 都没有插入数据，即：父子方法事务都回滚了。这说明子方法发送异常回滚时，如果父方法没有捕捉异常，那么父方法事务也会回滚。<br />最后，验证一下：当子方法事务发生异常时，如果父方法捕捉了异常，父方法事务是否会回滚？
```java
@Transactional
public void methodA() {
    tableService.insertTableA(new TableEntity());
    try {
        transactionServiceB.methodB();
    } catch (Exception e) {
        
    }
}
@Transactional(propagation = Propagation.NESTED)
public void methodB() {
    tableService.insertTableB(new TableEntity());
    throw new RuntimeException();
}
```
结果是：tablea 插入了数据，tableb 没有插入数据，即：父方法事务没有回滚，子方法事务回滚了。这说明子方法发送异常回滚时，如果父方法捕捉了异常，那么父方法事务就不会回滚。<br />看到这里，相信大家已经对 REQUIRED、REQUIRES_NEW 和 NESTED 这三个传播类型有了深入的理解了。最后来总结一下：

| **事务传播类型** | **特性** |
| --- | --- |
| REQUIRED | 当前方法存在事务时，子方法加入该事务。此时父子方法共用一个事务，无论父子方法哪个发生异常回滚，整个事务都回滚。即使父方法捕捉了异常，也是会回滚。而当前方法不存在事务时，子方法新建一个事务。 |
| REQUIRES_NEW | 无论当前方法是否存在事务，子方法都新建一个事务。此时父子方法的事务时独立的，它们都不会相互影响。但父方法需要注意子方法抛出的异常，避免因子方法抛出异常，而导致父方法回滚。 |
| NESTED | 当前方法存在事务时，子方法加入在嵌套事务执行。当父方法事务回滚时，子方法事务也跟着回滚。当子方法事务发送回滚时，父事务是否回滚取决于是否捕捉了异常。如果捕捉了异常，那么就不回滚，否则回滚。 |

<a name="v6MIv"></a>
## 应该怎么用？
看完了事务的传播类型，对 Spring 事务又有了深刻的理解。<br />看到这里，应该也明白：使用事务，不再是简单地使用 `@Transactional` 注解就可以，还需要根据业务场景，选择合适的传播类型。那么再升华一下使用 Spring 事务的方法论。一般来说，使用 Spring 事务的步骤为：

1. **根据业务场景，分析要达成的事务效果，确定使用的事务传播类型。**
2. **在 Service 层使用 **`**@Transactional**`** 注解，配置对应的 **`**propogation**`** 属性。**

下次遇到要使用事务的情况，记得按照这样的步骤去做～
<a name="JWhUH"></a>
## Spring 事务失效
<a name="yQq3P"></a>
###  什么时候 Spring 事务会失效？

1. 若同一类中的其他没有 `@Transactional` 注解的方法内部调用有 `@Transactional` 注解的方法，有 `@Transactional` 注解的方法的事务会失效。这是由于 Spring AOP 代理的原因造成的，因为只有当 `@Transactional` 注解的方法在类以外被调用的时候，Spring 事务管理才生效。
2. 另外，如果直接调用，不通过对象调用，也是会失效的。因为 Spring 事务是通过 AOP 实现的。
3. `@Transactional` 注解只有作用到 `public` 方法上事务才生效。
4. 被 `@Transactional` 注解的方法所在的类必须被 Spring 管理。
5. 底层使用的数据库必须支持事务机制，否则不生效。
<a name="jnIwL"></a>
## 彩蛋
Spring 事务执行过程中，如果抛出非 RuntimeException 和非 Error 错误的其他异常，那么是不会回滚的哦。例如下面的代码执行后，tablea 和 tableb 两个表格，都会插入一条数据。
```java
@Transactional
public void methodA() throws Exception {
    tableService.insertTableA(new TableEntity());
    transactionServiceB.methodB();
}
@Transactional
public void methodB() throws Exception {
    tableService.insertTableB(new TableEntity());
    // 非 RuntimeException
    throw new Exception();
}
```
