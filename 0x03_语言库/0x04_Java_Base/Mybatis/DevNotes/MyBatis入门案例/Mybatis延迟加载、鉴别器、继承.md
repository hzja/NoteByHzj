Mybatis
<a name="Y2pSg"></a>
## 建库建表
创建一个db：javacode2018<br />4张表：<br />t_user(用户表)<br />t_goods(商品表)<br />t_order(订单表)<br />t_order_detail(订单明细表)<br />表之间的关系：<br />t_order和t_user是一对一的关系，一条订单关联一个用户记录<br />t_order和t_order_detail是一对多关系，每个订单中可能包含多个子订单，每个子订单对应一个商品
```sql
DROP DATABASE IF EXISTS `javacode2018`;
CREATE DATABASE `javacode2018`;

USE `javacode2018`;

DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '用户id',
  name VARCHAR(32) NOT NULL DEFAULT '' COMMENT '用户名'
) COMMENT '用户表';
INSERT INTO t_user VALUES (1,'张学友'),(2,'Java');

DROP TABLE IF EXISTS t_goods;
CREATE TABLE t_goods(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '商品id',
  name VARCHAR(32) NOT NULL DEFAULT '' COMMENT '商品名称',
  price DECIMAL(10,2) NOT NULL DEFAULT 0 COMMENT '商品价格'
) COMMENT '商品信息表';
INSERT INTO t_goods VALUES (1,'Mybatis',8.88),(2,'Tom',16.66);

DROP TABLE IF EXISTS t_order;
CREATE TABLE t_order(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '订单id',
  user_id INT NOT NULL DEFAULT 0 COMMENT '用户id，来源于t_user.id',
  create_time BIGINT NOT NULL DEFAULT 0 COMMENT '订单创建时间(时间戳，秒)',
  up_time BIGINT NOT NULL DEFAULT 0 COMMENT '订单最后修改时间(时间戳，秒)'
) COMMENT '订单表';
INSERT INTO t_order VALUES (1,2,unix_timestamp(now()),unix_timestamp(now())),(2,1,unix_timestamp(now()),unix_timestamp(now())),(3,1,unix_timestamp(now()),unix_timestamp(now()));

DROP TABLE IF EXISTS t_order_detail;
CREATE TABLE t_order_detail(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '订单明细id',
  order_id INT NOT NULL DEFAULT 0 COMMENT '订单id，来源于t_order.id',
  goods_id INT NOT NULL DEFAULT 0 COMMENT '商品id，来源于t_goods.id',
  num INT NOT NULL DEFAULT 0 COMMENT '商品数量',
  total_price DECIMAL(12,2) NOT NULL DEFAULT 0 COMMENT '商品总金额'
) COMMENT '订单表';
INSERT INTO t_order_detail VALUES (1,1,1,2,17.76),(2,1,1,1,16.66),(3,2,1,1,8.88),(4,3,1,1,8.88);

select * from t_user;
select * from t_goods;
select * from t_order;
select * from t_order_detail;
```
<a name="MdMUR"></a>
## 延迟加载
延迟加载其实就是将数据加载时机推迟，比如推迟嵌套查询的执行时机，在Mybatis中经常用到关联查询，但是并不是任何时候都需要立即返回关联查询结果。比如查询订单信息，并不一定需要及时返回订单对应的用户信息或者订单详情信息等，这种情况需要一种机制，当需要查看关联的数据时，再去执行对应的查询，返回需要的结果，这种需求在Mybatis中可以使用延迟加载机制来实现。
<a name="tX2DA"></a>
### 延迟加载2种设置方式

1. 全局配置的方式
2. sqlmap中配置的方式

方式1中会对所有关联查询起效，而方式2只会对相关设置的查询起效。
<a name="SfFoW"></a>
## 全局配置延迟加载
Mybatis配置文件中通过下面两个属性来控制延迟加载：
```xml
<settings>
    <!--打开延迟加载的开关  -->
    <setting name="lazyLoadingEnabled" value="true"/>
    <!-- 当为true的时候，调用任意延迟属性，会去加载所有延迟属性，如果为false，则调用某个属性的时候，只会加载指定的属性 -->
    <setting name="aggressiveLazyLoading" value="false"/>
</settings>
```
`lazyLoadingEnabled`：这个属性比较好理解，是否开启延迟加载，默认为false，如果需要开启延迟加载，将其设置为true<br />`aggressiveLazyLoading`：当为true的时候，调用任意延迟属性，会去加载所有延迟属性，如果为false，则调用某个属性的时候，只会加载指定的属性<br />下面来个案例感受一下效果。
<a name="z2xEZ"></a>
### 需求
通过订单id查询订单信息、订单用户信息、订单明细列表，而订单用户信息、订单明细列表采用延迟加载的方式获取。
<a name="nDxih"></a>
### Mybatis配置
```xml
<settings>
    <!--打开延迟加载的开关  -->
    <setting name="lazyLoadingEnabled" value="true"/>
    <!-- 当为true的时候，调用任意延迟属性，会去加载所有延迟属性，如果为false，则调用某个属性的时候，只会加载指定的属性 -->
    <setting name="aggressiveLazyLoading" value="true"/>
</settings>
```
<a name="m9xEz"></a>
### OrderMapper.xml
```xml
<resultMap id="orderModelMap1" type="com.javacode2018.chat05.demo5.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!--  通过订单中user_id作为条件，查询下单人信息 -->
    <association property="userModel" select="com.javacode2018.chat05.demo5.mapper.UserMapper.getById1" column="user_Id"/>
    <!--  通过订单id作为条件，查询详情列表 -->
    <collection property="orderDetailModelList" select="com.javacode2018.chat05.demo5.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
</resultMap>

<select id="getById1" resultMap="orderModelMap1">
    <![CDATA[
    SELECT
        a.id ,
        a.user_id,
        a.create_time,
        a.up_time
    FROM
        t_order a
    WHERE
        a.id = #{value}
    ]]>
</select>
```
上面的orderModelMap1元素下面有两个关联查询，也写一下。
<a name="PWVXZ"></a>
### UserMapper.xml
```xml
<!-- 根据用户id查询用户信息 -->
<select id="getById1" resultType="com.javacode2018.chat05.demo5.model.UserModel">
    <![CDATA[
    SELECT id,name FROM t_user where id = #{user_id}
    ]]>
</select>
```
<a name="b2d2H"></a>
### OrderDetailMapper.xml
```xml
<!-- 根据订单di查询订单明细列表 -->
<select id="getListByOrderId1" resultType="com.javacode2018.chat05.demo5.model.OrderDetailModel">
    <![CDATA[
    SELECT
        a.id,
        a.order_id AS orderId,
        a.goods_id AS goodsId,
        a.num,
        a.total_price AS totalPrice
    FROM
        t_order_detail a
    WHERE
        a.order_id = #{order_id}
    ]]>
</select>
```
<a name="lnbrM"></a>
### 对应的3个Model
```java
@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class OrderModel {
    private Integer id;
    private Integer userId;
    private Long createTime;
    private Long upTime;
    private UserModel userModel;
    //订单详情列表
    private List<OrderDetailModel> orderDetailModelList;
}

@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class UserModel {
    private Integer id;
    private String name;
}

@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class OrderDetailModel {
    private Integer id;
    private Integer orderId;
    private Integer goodsId;
    private Integer num;
    private Double totalPrice;
}
```
<a name="pKqk7"></a>
### 测试用例
```java
@Test
public void getById1() throws IOException {
    //指定mybatis全局配置文件
    mybatisConfig = "demo5/mybatis-config.xml";
    this.before();
    OrderModel orderModel = null;
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        orderModel = mapper.getById1(1);
    }
    log.info("-------分割线--------");
    log.info("{}", orderModel.getUserModel());
}
```
<a name="b5DZM"></a>
### 运行输出
```
01:55.343 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
01:55.372 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 1(Integer)
01:55.431 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
01:55.431 [main] INFO  c.j.chat05.demo5.Demo5Test - -------分割线--------
01:55.432 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ==>  Preparing: SELECT a.id, a.order_id AS orderId, a.goods_id AS goodsId, a.num, a.total_price AS totalPrice FROM t_order_detail a WHERE a.order_id = ? 
01:55.432 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ==> Parameters: 1(Integer)
01:55.435 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - <==      Total: 2
01:55.439 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ==>  Preparing: SELECT id,name FROM t_user where id = ? 
01:55.439 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ==> Parameters: 2(Integer)
01:55.441 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <==      Total: 1
01:55.441 [main] INFO  c.j.chat05.demo5.Demo5Test - UserModel(id=2, name=Java)
```
从日志中可以看出，总共有3次查询，后面2次查询在分割线之后出现的，说明是调用了`orderModel.getUserModel()`触发后面2次查询动作。<br />代码中调用的是获取用户信息，而订单列表信息也被加载了，这个主要是由于`aggressiveLazyLoading`被设置为true了，当使用到一个延迟加载的属性时，其他的延迟加载的属性也会被一起加载，所以触发了2个关联的查询。<br />下面看看将`aggressiveLazyLoading`设置为false的效果
```xml
<settings>
  <!--打开延迟加载的开关  -->
  <setting name="lazyLoadingEnabled" value="true"/>
  <!-- 当为true的时候，调用任意延迟属性，会去加载所有延迟属性，如果为false，则调用某个属性的时候，只会加载指定的属性 -->
  <setting name="aggressiveLazyLoading" value="false"/>
</settings>
```
再次运行测试用例输出
```
12:19.236 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
12:19.268 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 1(Integer)
12:19.336 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
12:19.337 [main] INFO  c.j.chat05.demo5.Demo5Test - -------分割线--------
12:19.338 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ==>  Preparing: SELECT id,name FROM t_user where id = ? 
12:19.338 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ==> Parameters: 2(Integer)
12:19.340 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <==      Total: 1
12:19.341 [main] INFO  c.j.chat05.demo5.Demo5Test - UserModel(id=2, name=Java)
```
<a name="YrN1F"></a>
## sqlmap中设置延迟加载
全局的方式会对所有的关联查询起效，影响范围比较大，Mybatis也提供了在关联查询中进行设置的方式，只会对当前设置的关联查询起效。<br />关联查询，一般使用association、collection，这两个元素都有个属性fetchType，通过这个属性可以指定关联查询的加载方式。<br />fetchType值有2种，eager：立即加载；lazy：延迟加载。<br />下面来实现一个需求：还是通过订单id查询订单信息，并获取关联的用户信息、订单详细列表，用户信息要求立即加载，而订单详情要求延迟加载。
<a name="Q0lpx"></a>
### mapper xml如下
```xml
<resultMap id="orderModelMap2" type="com.javacode2018.chat05.demo5.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!--  通过订单中user_id作为条件，查询下单人信息 -->
    <association property="userModel" fetchType="eager" select="com.javacode2018.chat05.demo5.mapper.UserMapper.getById1" column="user_Id"/>
    <!--  通过订单id作为条件，查询详情列表 -->
    <collection property="orderDetailModelList" fetchType="lazy" select="com.javacode2018.chat05.demo5.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
</resultMap>

<select id="getById2" resultMap="orderModelMap2">
    <![CDATA[
    SELECT
        a.id ,
        a.user_id,
        a.create_time,
        a.up_time
    FROM
        t_order a
    WHERE
        a.id = #{value}
    ]]>
</select>
```
重点注意上面配置中association、collection这2个元素的fetchType属性，eager表示立即加载，lazy表示延迟加载。
<a name="j0fz2"></a>
### 测试用例
```java
@Test
public void getById2() throws IOException {
    //指定mybatis全局配置文件
    mybatisConfig = "demo5/mybatis-config2.xml";
    this.before();
    OrderModel orderModel = null;
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        orderModel = mapper.getById2(1);
    }
    log.info("-------分割线--------");
    log.info("{}", orderModel.getOrderDetailModelList());
}
```
<a name="IOC2t"></a>
### 运行输出
```
36:54.284 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
36:54.321 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 1(Integer)
36:54.385 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====>  Preparing: SELECT id,name FROM t_user where id = ? 
36:54.385 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====> Parameters: 2(Integer)
36:54.387 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <====      Total: 1
36:54.389 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
36:54.390 [main] INFO  c.j.chat05.demo5.Demo5Test - -------分割线--------
36:54.392 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ==>  Preparing: SELECT a.id, a.order_id AS orderId, a.goods_id AS goodsId, a.num, a.total_price AS totalPrice FROM t_order_detail a WHERE a.order_id = ? 
36:54.392 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ==> Parameters: 1(Integer)
36:54.397 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - <==      Total: 2
36:54.398 [main] INFO  c.j.chat05.demo5.Demo5Test - [OrderDetailModel(id=1, orderId=1, goodsId=1, num=2, totalPrice=17.76), OrderDetailModel(id=2, orderId=1, goodsId=1, num=1, totalPrice=16.66)]
```
注意输出中的分割线，可以分析得出，用户信息是和订单信息一起立即查出来的，而订单详情，是在调用`orderModel.getOrderDetailModelList()`获取订单列表的时候，采取懒加载的。
<a name="scHBg"></a>
## 鉴别器(discriminator)
有时候，一个数据库查询可能会返回多个不同的结果集（但总体上还是有一定的联系的）， 鉴别器（discriminator）元素就是被设计来应对这种情况的，鉴别器的概念很好理解——它很像 Java 语言中的 switch 语句。<br />`discriminator`标签常用的两个属性如下：

- column：该属性用于设置要进行鉴别比较值的列。
- javaType：该属性用于指定列的类型，保证使用相同的java类型来比较值。

`discriminator`标签可以有1个或多个case标签，case标签有一个比较重要的属性：

- value：该值为discriminator指定column用来匹配的值，当匹配的时候，结果会走这个case关联的映射。

使用鉴别器实现一个功能：通过订单id查询订单信息，当传入的订单id为1的时候，获取订单信息及下单人信息；当传入的订单id为2的时候，获取订单信息、下单人信息、订单明细信息；其他情况默认只查询订单信息。
<a name="eb7gG"></a>
### OrderMapper.xml
```xml
<resultMap id="orderModelMap1" type="com.javacode2018.chat05.demo6.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!-- 鉴别器 -->
    <discriminator javaType="int" column="id">
        <case value="1">
            <!--通过用户id查询用户信息-->
            <association property="userModel" select="com.javacode2018.chat05.demo6.mapper.UserMapper.getById1" column="user_Id"/>
        </case>
        <case value="2">
            <!--通过用户id查询用户信息-->
            <association property="userModel" select="com.javacode2018.chat05.demo6.mapper.UserMapper.getById1" column="user_Id"/>
            <!--通过订单id查询订单列表-->
            <collection property="orderDetailModelList" select="com.javacode2018.chat05.demo6.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
        </case>
    </discriminator>
</resultMap>

<select id="getById1" resultMap="orderModelMap1">
    <![CDATA[
    SELECT
        a.id ,
        a.user_id,
        a.create_time,
        a.up_time
    FROM
        t_order a
    WHERE
        a.id = #{value}
    ]]>
</select>
```
注意上面的discriminator，这部分是关键，discriminator内部的case会和每行查询结果中的id字段进行匹配，匹配成功了case内部的关联查询会被执行，未匹配上的，只会走discriminator外部默认配置的映射映射规则。
<a name="B3plr"></a>
### UserMapper.xml
```xml
<!-- 通过用户id查询用户信息 -->
<select id="getById1" resultType="com.javacode2018.chat05.demo6.model.UserModel">
    <![CDATA[
    SELECT id,name FROM t_user where id = #{user_id}
    ]]>
</select>
```
<a name="kuUSb"></a>
### OrderDetailMapper.xml
```xml
<!-- 通过订单id查询订单明细列表 -->
<select id="getListByOrderId1" resultType="com.javacode2018.chat05.demo6.model.OrderDetailModel">
    <![CDATA[
    SELECT
        a.id,
        a.order_id AS orderId,
        a.goods_id AS goodsId,
        a.num,
        a.total_price AS totalPrice
    FROM
        t_order_detail a
    WHERE
        a.order_id = #{order_id}
    ]]>
</select>
```
<a name="MXR9A"></a>
### 对应的3个Model类
```java
@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class OrderModel {
    private Integer id;
    private Integer userId;
    private Long createTime;
    private Long upTime;
    //用户信息
    private UserModel userModel;
    //订单详情列表
    private List<OrderDetailModel> orderDetailModelList;
}


@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class UserModel {
    private Integer id;
    private String name;
}

@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class OrderDetailModel {
    private Integer id;
    private Integer orderId;
    private Integer goodsId;
    private Integer num;
    private Double totalPrice;
}
```
<a name="owI7D"></a>
### 测试用例
```java
@Test
public void getById1() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        //查询订单为1的
        OrderModel orderModel = mapper.getById1(1);
        log.info("{}", orderModel);
        log.info("------------------------------------------------------------");
        //查询订单为2的
        orderModel = mapper.getById1(2);
        log.info("{}", orderModel);
        log.info("------------------------------------------------------------");
        //查询订单为3的
        orderModel = mapper.getById1(3);
        log.info("{}", orderModel);
    }
}
```
<a name="tWuoD"></a>
### 运行输出
```
58:16.413 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
58:16.457 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 1(Integer)
58:16.481 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====>  Preparing: SELECT id,name FROM t_user where id = ? 
58:16.481 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====> Parameters: 2(Integer)
58:16.488 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <====      Total: 1
58:16.489 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
58:16.489 [main] INFO  c.j.chat05.demo6.Demo6Test - OrderModel(id=1, userId=2, createTime=1578368161, upTime=1578368161, userModel=UserModel(id=2, name=Java), orderDetailModelList=null)
58:16.491 [main] INFO  c.j.chat05.demo6.Demo6Test - ------------------------------------------------------------
58:16.491 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
58:16.492 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 2(Integer)
58:16.493 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====>  Preparing: SELECT id,name FROM t_user where id = ? 
58:16.493 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====> Parameters: 1(Integer)
58:16.494 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <====      Total: 1
58:16.495 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ====>  Preparing: SELECT a.id, a.order_id AS orderId, a.goods_id AS goodsId, a.num, a.total_price AS totalPrice FROM t_order_detail a WHERE a.order_id = ? 
58:16.495 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ====> Parameters: 2(Integer)
58:16.505 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - <====      Total: 1
58:16.505 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
58:16.506 [main] INFO  c.j.chat05.demo6.Demo6Test - OrderModel(id=2, userId=1, createTime=1578368161, upTime=1578368161, userModel=UserModel(id=1, name=张学友), orderDetailModelList=[OrderDetailModel(id=3, orderId=2, goodsId=1, num=1, totalPrice=8.88)])
58:16.506 [main] INFO  c.j.chat05.demo6.Demo6Test - ------------------------------------------------------------
58:16.506 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
58:16.506 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 3(Integer)
58:16.508 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
58:16.509 [main] INFO  c.j.chat05.demo6.Demo6Test - OrderModel(id=3, userId=1, createTime=1578368161, upTime=1578368161, userModel=null, orderDetailModelList=null)
```
输出中可以看出，订单1查询了2次，订单2查询了3次，订单3查询了1次；鉴别器算是一个不错的功能。
<a name="klTzv"></a>
## 继承(`extends`)
继承在java是三大特性之一，可以起到重用代码的作用，而Mybatis也有继承的功能，和java中的继承的作用类似，主要在resultMap中使用，可以重用其他resultMap中配置的映射关系。
<a name="jHdSE"></a>
### 用法
```xml
<resultMap extends="被继承的resultMap的id"></resultMap>
```
<a name="yxh8w"></a>
### 案例
下面使用继承来对上面的鉴别器的案例改造一下，优化一下代码
<a name="IbnK9"></a>
#### OrderMapper.xml
```xml
<resultMap id="orderModelMap2" type="com.javacode2018.chat05.demo6.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!-- 鉴别器 -->
    <discriminator javaType="int" column="id">
        <case value="1" resultMap="orderModelMap3" />
        <case value="2" resultMap="orderModelMap4" />
    </discriminator>
</resultMap>

<resultMap id="orderModelMap3" type="com.javacode2018.chat05.demo6.model.OrderModel" extends="orderModelMap2">
    <!--通过用户id查询用户信息-->
    <association property="userModel" select="com.javacode2018.chat05.demo6.mapper.UserMapper.getById1" column="user_Id"/>
</resultMap>

<resultMap id="orderModelMap4" type="com.javacode2018.chat05.demo6.model.OrderModel" extends="orderModelMap3">
    <!--通过订单id查询订单列表-->
    <collection property="orderDetailModelList" select="com.javacode2018.chat05.demo6.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
</resultMap>

<select id="getById2" resultMap="orderModelMap2">
    <![CDATA[
    SELECT
        a.id ,
        a.user_id,
        a.create_time,
        a.up_time
    FROM
        t_order a
    WHERE
        a.id = #{value}
    ]]>
</select>
```
重点在于上面两个`extends`属性，上面orderModelMap3继承了orderModelMap2中配置的映射关系（除鉴别器之外），自己又加入了一个`association`去查询用户信息；orderModelMap4继承了orderModelMap3，自己又加入了一个查询订单列表的collection元素。上面使用extends做到了代码重用，其实和下面这块代码写法效果一样：
```xml
<resultMap id="orderModelMap2" type="com.javacode2018.chat05.demo6.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!-- 鉴别器 -->
    <discriminator javaType="int" column="id">
        <case value="1" resultMap="orderModelMap3" />
        <case value="2" resultMap="orderModelMap4" />
    </discriminator>
</resultMap>

<resultMap id="orderModelMap3" type="com.javacode2018.chat05.demo6.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!--通过用户id查询用户信息-->
    <association property="userModel" select="com.javacode2018.chat05.demo6.mapper.UserMapper.getById1" column="user_Id"/>
</resultMap>

<resultMap id="orderModelMap4" type="com.javacode2018.chat05.demo6.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <!--通过用户id查询用户信息-->
    <association property="userModel" select="com.javacode2018.chat05.demo6.mapper.UserMapper.getById1" column="user_Id"/>
    <!--通过订单id查询订单列表-->
    <collection property="orderDetailModelList" select="com.javacode2018.chat05.demo6.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
</resultMap>

<select id="getById2" resultMap="orderModelMap2">
    <![CDATA[
    SELECT
        a.id ,
        a.user_id,
        a.create_time,
        a.up_time
    FROM
        t_order a
    WHERE
        a.id = #{value}
    ]]>
</select>
```
<a name="aOqGu"></a>
#### 测试用例
```java
@Test
public void getById2() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        //查询订单为1的
        OrderModel orderModel = mapper.getById2(1);
        log.info("{}", orderModel);
        log.info("------------------------------------------------------------");
        //查询订单为2的
        orderModel = mapper.getById2(2);
        log.info("{}", orderModel);
        log.info("------------------------------------------------------------");
        //查询订单为3的
        orderModel = mapper.getById2(3);
        log.info("{}", orderModel);
    }
}
```
<a name="Fzxcb"></a>
#### 运行输出
```
39:55.936 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
39:55.969 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 1(Integer)
39:55.986 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====>  Preparing: SELECT id,name FROM t_user where id = ? 
39:55.987 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====> Parameters: 2(Integer)
39:55.992 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <====      Total: 1
39:55.993 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
39:55.993 [main] INFO  c.j.chat05.demo6.Demo6Test - OrderModel(id=1, userId=2, createTime=1578368161, upTime=1578368161, userModel=UserModel(id=2, name=Java), orderDetailModelList=null)
39:55.994 [main] INFO  c.j.chat05.demo6.Demo6Test - ------------------------------------------------------------
39:55.994 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
39:55.995 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 2(Integer)
39:55.995 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ====>  Preparing: SELECT a.id, a.order_id AS orderId, a.goods_id AS goodsId, a.num, a.total_price AS totalPrice FROM t_order_detail a WHERE a.order_id = ? 
39:55.996 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ====> Parameters: 2(Integer)
39:56.000 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - <====      Total: 1
39:56.001 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====>  Preparing: SELECT id,name FROM t_user where id = ? 
39:56.004 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====> Parameters: 1(Integer)
39:56.005 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <====      Total: 1
39:56.005 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
39:56.005 [main] INFO  c.j.chat05.demo6.Demo6Test - OrderModel(id=2, userId=1, createTime=1578368161, upTime=1578368161, userModel=UserModel(id=1, name=张学友), orderDetailModelList=[OrderDetailModel(id=3, orderId=2, goodsId=1, num=1, totalPrice=8.88)])
39:56.005 [main] INFO  c.j.chat05.demo6.Demo6Test - ------------------------------------------------------------
39:56.005 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
39:56.006 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 3(Integer)
39:56.007 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
39:56.007 [main] INFO  c.j.chat05.demo6.Demo6Test - OrderModel(id=3, userId=1, createTime=1578368161, upTime=1578368161, userModel=null, orderDetailModelList=null)
```
