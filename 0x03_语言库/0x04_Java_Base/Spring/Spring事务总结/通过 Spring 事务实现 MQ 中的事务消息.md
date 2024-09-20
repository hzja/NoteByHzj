JavaSpring
<a name="lpWBe"></a>
## 1、消息投递的5种方式
<a name="dH2nL"></a>
### 1.1、业务场景
电商中有这样的一个场景：商品下单之后，需给用户送积分，订单表和积分表分别在不同的db中，涉及到分布式事务的问题。<br />通过可靠消息来解决这个问题：

1. 商品下单成功之后送积分的操作，使用mq来实现
2. 商品下单成功之后，投递一条消息到mq，积分系统消费消息，给用户增加积分

主要讨论一下，商品下单及投递消息到mq的操作，如何实现？每种方式优缺点？
<a name="f21BU"></a>
### 1.2、方式一
<a name="XrG2M"></a>
#### 过程

- **step1**：开启本地事务
- **step2**：生成购物订单
- **step3**：投递消息到mq
- **step4**：提交本地事务

**这种方式是将发送消息放在了事务提交之前。**
<a name="zBXCZ"></a>
#### 可能存在的问题

- **step3发生异常**：导致step4失败，商品下单失败，直接影响到商品下单业务
- **step4发生异常，其他step成功**：商品下单失败，消息投递成功，给用户增加了积分
<a name="Lc7RZ"></a>
### 1.3、方式二
下面换种方式，将发送消息放到事务之后进行。
<a name="olyTi"></a>
#### 过程

- **step1**：开启本地事务
- **step2**：生成购物订单
- **step3**：提交本地事务
- **step4**：投递消息到mq
<a name="m7AFv"></a>
#### 可能会出现的问题
**step4发生异常，其他step成功**：导致商品下单成功，投递消息失败，用户未增加积分<br />上面两种是比较常见的做法，也是最容易出错的。
<a name="lLfIA"></a>
### 1.4、方式三

- **step1**：开启本地事务
- **step2**：生成购物订单
- **step3**：本地库中插入一条需要发送消息的记录t_msg_record
- **step3**：提交本地事务
- **step5**：新增一个定时器，轮询t_msg_record，将待发送的记录投递到mq中

这种方式借助了数据库的事务，业务和消息记录作为了一个原子操作，业务成功之后，消息日志必定是存在的。解决了前两种方式遇到的问题。如果业务系统比较单一，可以采用这种方式。<br />对于微服务化的情况，上面这种方式不是太好，每个服务都需要上面的操作；也不利于扩展。
<a name="HIJaB"></a>
### 1.5、方式四
增加一个**消息服务**及**消息库**，负责消息的落库、将消息发送投递到mq。

- **step1**：开启本地事务
- **step2**：生成购物订单
- **step3**：当前事务库插入一条日志：生成一个唯一的业务id（msg_order_id），将msg_order_id和订单关联起来保存到当前事务所在的库中
- **step4**：调用消息服务：携带msg_order_id，将消息先落地入库，此时消息的状态为待发送状态，返回消息id(msg_id)
- **step5**：提交本地事务
- **step6**：如果上面都成功，调用消息服务，将消息投递到mq中；如果上面有失败的情况，则调用消息服务取消消息的发送

能想到上面这种方式，已经算是有很大进步了，继续分析一下可能存在的问题：

1. 系统中增加了一个消息服务，商品下单操作依赖于该服务，业务对该服务依赖性比较高，当消息服务不可用时，整个业务将不可用。
2. 若step6失败，消息将处于待发送状态，此时业务方需要提供一个回查接口（通过msg_order_id查询）,验证业务是否执行成功；消息服务需新增一个定时任务，对于状态为待发送状态的消息做补偿处理，检查一下业务是否处理成功；从而确定消息是投递还是取消发送
3. step4依赖于消息服务，如果消息服务性能不佳，会导致当前业务的事务提交时间延长，**容易产生死锁，并导致并发性能降低**。通常是比较忌讳在事务中做远程调用处理的，远程调用的性能和时间往往不可控，会导致当前事务变为一个大事务，从而引发其他故障。
<a name="VhrKu"></a>
### 1.6、方式五
在以上方式中，继续改进，进而出现了更好的一种方式：

- **step1**：生成一个全局唯一业务消息id（bus_msg_id)，调用消息服务，携带bus_msg_id，将消息先落地入库，此时消息的状态为待发送状态，返回消息id（msg_id）
- **step2**：开启本地事务
- **step3**：生成购物订单
- **step4**：当前事务库插入一条日志（将step3中的业务和bus_msg_id关联起来）
- **step5**：提交本地事务
- **step6**：分2种情况：如果上面都成功，调用消息服务，将消息投递到mq中；如果上面有失败的情况，则调用消息服务取消消息的发送

若step6失败，消息将处于待发送状态，此时业务方需要提供一个回查接口（通过bus_msg_id查询）,验证业务是否执行成功；<br />消息服务需新增一个定时任务，对于状态为待发送状态的消息做补偿处理，检查一下业务是否处理成功；从而确定消息是投递还是取消发送。<br />方式五和方式四对比，比较好的一个地方：将调用消息服务，消息落地操作，放在了事务之外进行，这点小的改进其实算是一个非常好的优化，减少了本地事务的执行时间，从而可以提升并发量，阿里消息中间件RocketMQ就支持方式5这种，大家可以去用用。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690101136687-8f3fb71d-f327-4129-af3f-88b6bf464801.png#averageHue=%23f5f5f5&clientId=u4726b209-f7f6-4&from=paste&id=ua92f7e40&originHeight=249&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2d355232-a8d5-413c-bdee-caf5891cc47&title=)<br />下面通过代码来实现方式4。
<a name="SdHAc"></a>
## 2、方式4代码实现
<a name="uKc8z"></a>
### 2.1、准备数据库
3张表：<br />t_user：业务库中的用户表，一会用来模拟用户注册，注册成功之后投递消息。<br />t_msg_order：消息订单表，这个表放在业务库中，业务操作中若需要发送消息，则在业务操作的事务中同时向t_msg_order表插入一条数据，若业务操作成功，那么t_msg_order表肯定也会成功插入一条数据，发送消息的时候携带上t_msg_order的id，消息服务可以通过这个id去业务库回查t_msg_order中的记录，如果记录存在，则说明业务操作成功了。<br />t_msg：消息表，所有发送的消息信息放在这个表中，主要字段有：消息内容，msg_order_id：来源于t_msg_order表的id
```sql
DROP DATABASE IF EXISTS javacode2018;
CREATE DATABASE if NOT EXISTS javacode2018;

USE javacode2018;
DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id INT PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(256) NOT NULL DEFAULT '' COMMENT '姓名'
);

DROP TABLE IF EXISTS t_msg;
CREATE TABLE t_msg(
  id INT PRIMARY KEY AUTO_INCREMENT,
  msg VARCHAR(256) NOT NULL DEFAULT '' COMMENT '消息内容，可以json格式的数据',
  msg_order_id BIGINT NOT NULL DEFAULT 0 COMMENT '消息订单id',
  status SMALLINT NOT NULL DEFAULT 0 COMMENT '消息状态,0:待投递，1：已发送，2：取消发送'
) COMMENT '消息表';

DROP TABLE IF EXISTS t_msg_order;
CREATE TABLE t_msg_order(
  id INT PRIMARY KEY AUTO_INCREMENT,
  ref_type BIGINT NOT NULL DEFAULT 0 COMMENT '关联业务类型',
  ref_id VARCHAR(256) NOT NULL DEFAULT '' COMMENT '关联业务id（ref_type & ref_id 唯一）'
) COMMENT '消息订单表,放在业务库中';

alter table t_msg_order add UNIQUE INDEX idx1 (ref_type,ref_id);
```
<a name="v8YAC"></a>
### 2.2、关键java代码
<a name="D4Vyn"></a>
#### 配置类MainConfig11
```java
package com.javacode2018.tx.demo11;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;

@ComponentScan
@EnableTransactionManagement
public class MainConfig11 {
    @Bean
    public DataSource dataSource() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义一个jdbcTemplate
    @Bean
    public JdbcTemplate jdbcTemplate(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义事务管理器transactionManager
    @Bean
    public PlatformTransactionManager transactionManager(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }

}
```
<a name="X9H3z"></a>
#### MsgModel
```java
package com.javacode2018.tx.demo11;

import lombok.*;

@Getter
@Setter
@Builder
@NoArgsConstructor
@AllArgsConstructor
@ToString
public class MsgModel {
    private Long id;
    //消息内容
    private String msg;
    //消息订单id
    private Long msg_order_id;
    //消息状态,0:待投递，1：已发送，2：取消发送
    private Integer status;
}
```
<a name="H96mR"></a>
#### MsgOrderModel
```java
package com.javacode2018.tx.demo11;

import lombok.*;

@Getter
@Setter
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class MsgOrderModel {
    private Long id;
    //关联业务类型
    private Integer ref_type;
    //关联业务id（ref_type & ref_id 唯一）
    private String ref_id;
}
```
<a name="mSfTc"></a>
#### MsgOrderService
提供了对t_msg_order表的一些操作，2个方法，一个用来插入数据，一个用来查询。
```java
package com.javacode2018.tx.demo11;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Objects;

@Component
public class MsgOrderService {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    /**
     * 插入消息订单
     *
     * @param ref_type
     * @param ref_id
     * @return
     */
    @Transactional
    public MsgOrderModel insert(Integer ref_type, String ref_id) {
        MsgOrderModel msgOrderModel = MsgOrderModel.builder().ref_type(ref_type).ref_id(ref_id).build();
        //插入消息
        this.jdbcTemplate.update("insert into t_msg_order (ref_type,ref_id) values (?,?)",
                                 ref_type,
                                 ref_id
                                );
        //获取消息订单id
        msgOrderModel.setId(this.jdbcTemplate.queryForObject("SELECT LAST_INSERT_ID()", Long.class));
        return msgOrderModel;
    }

    /**
     * 根据消息id获取消息
     *
     * @param id
     * @return
     */
    public MsgOrderModel getById(Long id) {
        List<MsgOrderModel> list = this.jdbcTemplate.query("select * from t_msg_order where id = ? limit 1", new BeanPropertyRowMapper<MsgOrderModel>(MsgOrderModel.class), id);
        return Objects.nonNull(list) && !list.isEmpty() ? list.get(0) : null;
    }

}
```
<a name="QUD9M"></a>
#### MsgService
消息服务，提供了对t_msg表的一些操作以及消息投递的一些方法

| 方法 | 说明 |
| --- | --- |
| addMsg | 添加消息，消息会落库，处于待发送状态 |
| confirmSendMsg | 确定投递消息，事务成功后可以调用 |
| cancelSendMsg | 取消投递消息，事务回滚可以调用 |

代码：
```java
package com.javacode2018.tx.demo11;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Objects;

@Component
public class MsgService {

    //添加一条消息(独立的事务中执行)
    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public Long addMsg(String msg, Long msg_order_id, boolean isSend) {
        MsgModel msgModel = MsgModel.builder().msg(msg).msg_order_id(msg_order_id).status(0).build();
        //先插入消息
        Long msg_id = this.insert(msgModel).getId();
        if (isSend) {
            //如果需要投递，则调用投递的方法
            this.confirmSendMsg(msg_id);
        }
        return msg_id;
    }

    /**
     * 确认消息投递(不需要事务)
     *
     * @param msg_id 消息id
     */
    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public void confirmSendMsg(Long msg_id) {
        MsgModel msgModel = this.getById(msg_id);
        //向mq中投递消息
        System.out.println(String.format("投递消息：%s", msgModel));
        //将消息状态置为已投递
        this.updateStatus(msg_id, 1);
    }

    /**
     * 取消消息投递(不需要事务)
     *
     * @param msg_id 消息id
     */
    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public void cancelSendMsg(Long msg_id) {
        MsgModel msgModel = this.getById(msg_id);
        System.out.println(String.format("取消投递消息：%s", msgModel));
        //将消息状态置为取消投递
        this.updateStatus(msg_id, 2);
    }

    @Autowired
    private JdbcTemplate jdbcTemplate;

    /**
     * 插入消息
     *
     * @param msgModel
     * @return
     */
    private MsgModel insert(MsgModel msgModel) {
        //插入消息
        this.jdbcTemplate.update("insert into t_msg (msg,msg_order_id,status) values (?,?,?)",
                                 msgModel.getMsg(),
                                 msgModel.getMsg_order_id(),
                                 msgModel.getStatus());
        //获取消息id
        msgModel.setId(this.jdbcTemplate.queryForObject("SELECT LAST_INSERT_ID()", Long.class));
        System.out.println("插入消息：" + msgModel);
        return msgModel;
    }

    /**
     * 根据消息id获取消息
     *
     * @param id
     * @return
     */
    private MsgModel getById(Long id) {
        List<MsgModel> list = this.jdbcTemplate.query("select * from t_msg where id = ? limit 1", new BeanPropertyRowMapper<MsgModel>(MsgModel.class), id);
        return Objects.nonNull(list) && !list.isEmpty() ? list.get(0) : null;
    }

    /**
     * 更新消息状态
     *
     * @param id
     * @param status
     */
    private void updateStatus(long id, int status) {
        this.jdbcTemplate.update("update t_msg set status = ? where id = ?", status, id);
    }

}
```
<a name="MAcbo"></a>
#### 消息投递器MsgSender
消息投递器，给业务方使用，内部只有一个方法，用来发送消息。<br />若上下文没有事务，则消息落地之后立即投递；若存在事务，则消息投递分为2步走：消息先落地，事务执行完毕之后再确定是否投递，用到了事务扩展点：`TransactionSynchronization`，事务执行完毕之后会回调`TransactionSynchronization`接口中的`afterCompletion`方法，在这个方法中确定是否投递消息。
```java
package com.javacode2018.tx.demo11;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.transaction.support.TransactionSynchronization;
import org.springframework.transaction.support.TransactionSynchronizationAdapter;
import org.springframework.transaction.support.TransactionSynchronizationManager;


/**
 * 消息发送器，所有使用者调用send方法发送消息
 */
@Component
public class MsgSender {
    @Autowired
    private MsgOrderService msgOrderService;
    @Autowired
    private MsgService msgService;

    //发送消息
    public void send(String msg, int ref_type, String ref_id) {
        MsgOrderModel msgOrderModel = this.msgOrderService.insert(ref_type, ref_id);

        Long msg_order_id = msgOrderModel.getId();
        //TransactionSynchronizationManager.isSynchronizationActive 可以用来判断事务同步是否开启了
        boolean isSynchronizationActive = TransactionSynchronizationManager.isSynchronizationActive();
        /**
         * 若事务同步开启了，那么可以在事务同步中添加事务扩展点，则先插入消息，暂不发送，则在事务扩展点中添加回调
         * 事务结束之后会自动回调扩展点TransactionSynchronizationAdapter的afterCompletion()方法
         * 咱们在这个方法中确定是否投递消息
         */
        if (isSynchronizationActive) {
            final Long msg_id = this.msgService.addMsg(msg, msg_order_id, false);
            TransactionSynchronizationManager.registerSynchronization(new TransactionSynchronizationAdapter() {
                @Override
                public void afterCompletion(int status) {
                    //代码走到这里时，事务已经完成了（可能是回滚了、或者是提交了）
                    //看一下消息关联的订单是否存在，如果存在，说明事务是成功的，业务是执行成功的，那么投递消息
                    if (msgOrderService.getById(msg_order_id) != null) {
                        System.out.println(String.format("准备投递消息,{msg_id:%s}", msg_id));
                        //事务成功：投递消息
                        msgService.confirmSendMsg(msg_id);
                    } else {
                        System.out.println(String.format("准备取消投递消息，{msg_id:%s}", msg_id));
                        //事务是不：取消投递消息
                        msgService.cancelSendMsg(msg_id);
                    }
                }
            });
        } else {
            //无事务的，直接插入并投递消息
            this.msgService.addMsg(msg, msg_order_id, true);
        }
    }
}
```
<a name="TFK86"></a>
### 2.3、测试（3种场景）
<a name="VqLhO"></a>
#### 2.3.1、场景1：业务成功，消息投递成功
<a name="OTqJ8"></a>
##### UserService
下面的register方法是有事务的，内部会插入一条用户信息，然后会投递一条消息
```java
package com.javacode2018.tx.demo11;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class UserService {
    @Autowired
    private JdbcTemplate jdbcTemplate;
    //消息投递器
    @Autowired
    private MsgSender msgSender;

    /**
     * 模拟用户注册成功，顺便发送消息
     */
    @Transactional
    public void register(Long user_id, String user_name) {
        //先插入用户
        this.jdbcTemplate.update("insert into t_user(id,name) VALUES (?,?)", user_id, user_name);
        System.out.println(String.format("用户注册：[user_id:%s,user_name:%s]", user_id, user_name));
        //发送消息
        String msg = String.format("[user_id:%s,user_name:%s]", user_id, user_name);
        //调用投递器的send方法投递消息
        this.msgSender.send(msg, 1, user_id.toString());
    }

}
```
<a name="O504o"></a>
##### 测试类
```java
package com.javacode2018.tx.demo11;

import org.junit.Before;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;

public class Demo11Test {

    private AnnotationConfigApplicationContext context;
    private UserService userService;
    private JdbcTemplate jdbcTemplate;

    @Before
    public void before() {
        this.context = new AnnotationConfigApplicationContext(MainConfig11.class);
        userService = context.getBean(UserService.class);
        this.jdbcTemplate = context.getBean("jdbcTemplate", JdbcTemplate.class);
        jdbcTemplate.update("truncate table t_user");
        jdbcTemplate.update("truncate table t_msg");
        jdbcTemplate.update("truncate table t_msg_order");
    }

    @Test
    public void test1() {
        this.userService.register(1L, "Tom");
    }

}
```
<a name="qlMHI"></a>
##### 运行输出
```
用户注册：[user_id:1,user_name:Tom]
插入消息：MsgModel(id=1, msg=[user_id:1,user_name:Tom], msg_order_id=1, status=0)
准备投递消息,{msg_id:1}
投递消息：MsgModel(id=1, msg=[user_id:1,user_name:Tom], msg_order_id=1, status=0)
```
<a name="LN65y"></a>
#### 2.3.2、场景2：业务失败，消息取消投递
<a name="sOyS0"></a>
##### UserService中添加代码
手动抛出异常，让事务回滚。
```java
/**
 * 模拟用户注册失败，通过抛出异常让事务回滚，结果也会导致消息发送被取消
 *
 * @param user_id
 * @param user_name
 */
@Transactional
public void registerFail(Long user_id, String user_name) {
    this.register(user_id, user_name);
    throw new RuntimeException("故意失败!");
}
```
<a name="ZLrIE"></a>
##### Demo11Test添加用例
```java
@Test
public void test2() {
    this.userService.registerFail(1L, "张三");
}
```
<a name="lejcw"></a>
##### 运行输出
弹出了异常，信息比较多，截了关键的部分，如下，可以看出事务被回滚了，消息被取消投递了。
```
用户注册：[user_id:1,user_name:张三]
插入消息：MsgModel(id=1, msg=[user_id:1,user_name:张三], msg_order_id=1, status=0)
准备取消投递消息，{msg_id:1}
取消投递消息：MsgModel(id=1, msg=[user_id:1,user_name:张三], msg_order_id=1, status=0)

java.lang.RuntimeException: 故意失败!

 at com.javacode2018.tx.demo11.UserService.registerFail(UserService.java:44)
 at com.javacode2018.tx.demo11.UserService$$FastClassBySpringCGLIB$$5dd21f5c.invoke(<generated>)
```
<a name="cX91A"></a>
#### 2.3.3、嵌套事务
事务发送是跟随当前所在的事务的，当前事务提交了，消息一定会被投递出去，当前事务是不，消息会被取消投递。<br />下面看嵌套事务的代码
<a name="FTWxK"></a>
##### UserService中添加代码
注意下面方法的事务传播行为是：REQUIRES_NEW，当前如果有事务，会重启一个事务。
```java
//事务传播属性是REQUIRES_NEW,会在独立的事务中运行
@Transactional(propagation = Propagation.REQUIRES_NEW)
public void registerRequiresNew(Long user_id, String user_name) {
    this.register(user_id, user_name);
}
```
<a name="zvNvU"></a>
##### 添加一个类UserService1
```java
package com.javacode2018.tx.demo11;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

@Component
public class UserService1 {
    @Autowired
    private UserService userService;

    @Autowired
    private MsgSender msgSender;

    //嵌套事务案例
    @Transactional
    public void nested() {
        this.msgSender.send("消息1", 2, "1");
        //registerRequiresNew事务传播属性是REQUIRES_NEW:会在一个新事务中运行
        this.userService.registerRequiresNew(1L, "张三");
        //registerFail事务传播属性是默认的，会在当前事务中运行，registerFail弹出异常会导致当前事务回滚
        this.userService.registerFail(2L, "李四");
    }
}
```
nested 是外围方法，这个方法上有`@Transactional`，运行的时候会开启一个事务，内部3行代码：<br />@1：发送消息，会在当前事务中执行<br />@2：`registerRequiresNew`事务传播行为是`REQUIRES_NEW`，所以会重启一个事务<br />@3：`registerFail`事务传播行为是默认的`REQUIRED`，会参与到`nested()`开启的事务中运行，`registerFail`方法内部会抛出一个异常，最终会导致外部方法事务回滚。<br />上面方法需要投递3条消息，而@1和@3投递的消息由于事务回滚会导致消息被回滚，而@2在独立的事务中执行，@2的消息会投递成功，下面来看看执行结果，是不是和分析的一致。
<a name="E9OjB"></a>
##### Demo11Test添加用例
```java
@Test
public void test3() {
    UserService1 userService1 = this.context.getBean(UserService1.class);
    userService1.nested();
}
```
<a name="GfTL1"></a>
##### 运行输出
```
插入消息：MsgModel(id=1, msg=消息1, msg_order_id=1, status=0)
用户注册：[user_id:1,user_name:张三]
插入消息：MsgModel(id=2, msg=[user_id:1,user_name:张三], msg_order_id=2, status=0)
准备投递消息,{msg_id:2}
投递消息：MsgModel(id=2, msg=[user_id:1,user_name:张三], msg_order_id=2, status=0)
用户注册：[user_id:2,user_name:李四]
插入消息：MsgModel(id=3, msg=[user_id:2,user_name:李四], msg_order_id=3, status=0)
准备取消投递消息，{msg_id:1}
取消投递消息：MsgModel(id=1, msg=消息1, msg_order_id=1, status=0)
准备取消投递消息，{msg_id:3}
取消投递消息：MsgModel(id=3, msg=[user_id:2,user_name:李四], msg_order_id=3, status=0)

java.lang.RuntimeException: 故意失败!

 at com.javacode2018.tx.demo11.UserService.registerFail(UserService.java:44)
```
大家细看一下结果，和分析的是一致的。
<a name="wT1OJ"></a>
### 2.4、小结
事务消息分2步走，先落库，此时消息待投递，等到事务执行完毕之后，再确定是否投递，用到的关键技术点是事务扩展接口：`TransactionSynchronization`，事务执行完毕之后会自动回调接口中的`afterCompletion`方法。<br />**遗留的一个问题：消息补偿操作**<br />当事务消息刚落地，此时处于待投递状态，系统刚好down机了，此时系统恢复之后，需要有个定时器来处理这种消息，拿着消息中的msg_order_id去业务库查一下订单是否存在，如果存在，则投递消息，否则取消投递，这个留给大家去实现。
<a name="WPh8f"></a>
## 3、总结
总结回顾一下需要重点掌握的内容。<br />1、消息投递的5种方式的推演，要熟练掌握其优缺点<br />2、方式4中事务消息的代码实现，需要大家掌握<br />消息服务使用频率挺高的，通常作为系统中的基础服务使用，大家可以尝试一下开发一个独立的消息服务，提供给其他服务使用。
