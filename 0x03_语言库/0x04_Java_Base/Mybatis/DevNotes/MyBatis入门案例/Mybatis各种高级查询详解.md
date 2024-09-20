Mybatis
<a name="mD4S2"></a>
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
INSERT INTO t_goods VALUES (1,'Mybatis',8.88),(2,'maven高手',16.66);

DROP TABLE IF EXISTS t_order;
CREATE TABLE t_order(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '订单id',
  user_id INT NOT NULL DEFAULT 0 COMMENT '用户id，来源于t_user.id',
  create_time BIGINT NOT NULL DEFAULT 0 COMMENT '订单创建时间(时间戳，秒)',
  up_time BIGINT NOT NULL DEFAULT 0 COMMENT '订单最后修改时间(时间戳，秒)'
) COMMENT '订单表';
INSERT INTO t_order VALUES (1,2,unix_timestamp(now()),unix_timestamp(now())),(2,1,unix_timestamp(now()),unix_timestamp(now()));

DROP TABLE IF EXISTS t_order_detail;
CREATE TABLE t_order_detail(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '订单明细id',
  order_id INT NOT NULL DEFAULT 0 COMMENT '订单id，来源于t_order.id',
  goods_id INT NOT NULL DEFAULT 0 COMMENT '商品id，来源于t_goods.id',
  num INT NOT NULL DEFAULT 0 COMMENT '商品数量',
  total_price DECIMAL(12,2) NOT NULL DEFAULT 0 COMMENT '商品总金额'
) COMMENT '订单表';
INSERT INTO t_order_detail VALUES (1,1,1,2,17.76),(2,1,1,1,16.66),(3,2,1,1,8.88);

select * from t_user;
select * from t_goods;
select * from t_order;
select * from t_order_detail;
```
<a name="PgY7C"></a>
## 单表查询(3种方式)
<a name="Svpop"></a>
### 需求
需要按照订单id查询订单信息。
<a name="rJrBj"></a>
### 方式1
<a name="HjVIL"></a>
#### 创建每个表对应的Model
db中表的字段是采用下划线分割的，model中是采用骆驼命名法来命名的，如OrderModel:
```java
package com.javacode2018.chat05.demo1.model;

import lombok.*;

import java.util.List;

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
}
```
其他几个Model也类似。
<a name="fxcbn"></a>
#### Mapper xml
```xml
<select id="getById" resultType="com.javacode2018.chat05.demo1.model.OrderModel">
    <![CDATA[
    SELECT a.id,a.user_id as userId,a.create_time createTime,a.up_time upTime FROM t_order a WHERE a.id = #{value}
    ]]>
</select>
```
注意上面的resultType，标识结果的类型。
<a name="r21cv"></a>
#### Mapper接口方法
```java
OrderModel getById(int id);
```
<a name="L4TOq"></a>
#### Mybatis全局配置文件
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- 引入外部jdbc配置 -->
    <properties resource="jdbc.properties"/>
    <!-- 环境配置，可以配置多个环境 -->
    <environments default="demo4">
        <environment id="demo4">
            <!-- 事务管理器工厂配置 -->
            <transactionManager type="JDBC"/>
            <!-- 数据源工厂配置，使用工厂来创建数据源 -->
            <dataSource type="POOLED">
                <property name="driver" value="${jdbc.driver}"/>
                <property name="url" value="${jdbc.url}"/>
                <property name="username" value="${jdbc.username}"/>
                <property name="password" value="${jdbc.password}"/>
            </dataSource>
        </environment>
    </environments>

    <mappers>
        <mapper resource="demo1/mapper/UserMapper.xml" />
        <mapper resource="demo1/mapper/GoodsMapper.xml" />
        <mapper resource="demo1/mapper/OrderMapper.xml" />
        <mapper resource="demo1/mapper/OrderDetailMapper.xml" />
    </mappers>
</configuration>
```
<a name="LbvLf"></a>
#### 测试用例
```java
@Before
public void before() throws IOException {
    //指定mybatis全局配置文件
    String resource = "demo1/mybatis-config.xml";
    //读取全局配置文件
    InputStream inputStream = Resources.getResourceAsStream(resource);
    //构建SqlSessionFactory对象
    SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
    this.sqlSessionFactory = sqlSessionFactory;
}

@Test
public void getById() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById(1);
        log.info("{}", orderModel);
    }
}
```
<a name="x7bdQ"></a>
#### 运行输出
```
35:59.211 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById - ==>  Preparing: SELECT a.id,a.user_id as userId,a.create_time createTime,a.up_time upTime FROM t_order a WHERE a.id = ? 
35:59.239 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById - ==> Parameters: 1(Integer)
35:59.258 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById - <==      Total: 1
35:59.258 [main] INFO  c.j.chat05.demo1.Demo1Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790)
```
<a name="d23nk"></a>
#### 原理
sql中使用了别名，将t_order中的字段转换成了和OrderModel中字段一样的名称，最后Mybatis内部会通过反射，将查询结果按照名称到OrderModel中查找同名的字段，然后进行赋值。
<a name="DBWiy"></a>
### 方式2
若项目中表对应的Model中的字段都是采用骆驼命名法，Mybatis中可以进行一些配置，可以使表中的字段和对应Model中骆驼命名法的字段进行自动映射。<br />需要在Mybatis全局配置文件中加入下面配置：
```xml
<settings>
  <!-- 是否开启自动驼峰命名规则映射，及从xx_yy映射到xxYy -->
  <setting name="mapUnderscoreToCamelCase" value="true"/>
</settings>
```
<a name="jwGZu"></a>
#### Mapper xml
```xml
<select id="getById1" resultType="com.javacode2018.chat05.demo1.model.OrderModel">
    <![CDATA[
    SELECT a.id,a.user_id,a.create_time,a.up_time FROM t_order a WHERE a.id = #{value}
    ]]>
</select>
```
注意上面的sql，没有写别名了，由于开启了自动骆驼命名映射，所以查询结果会按照下面的关系进行自动映射：

| sql对应的字段 | OrderModel中的字段 |
| --- | --- |
| id | id |
| user_id | userId |
| create_time | createTime |
| up_time | upTime |

<a name="J6vml"></a>
#### Mapper接口
```java
OrderModel getById1(int id);
```
<a name="CdOZ3"></a>
#### 测试用例
```java
@Test
public void getById1() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById1(1);
        log.info("{}", orderModel);
    }
}
```
<a name="gDN7L"></a>
#### 运行输出
```
59:44.884 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id,a.user_id,a.create_time,a.up_time FROM t_order a WHERE a.id = ? 
59:44.917 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 1(Integer)
59:44.935 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
59:44.935 [main] INFO  c.j.chat05.demo1.Demo1Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790)
```
输出中可以看出，sql中的字段是下划线的方式，OrderModel中的字段是骆驼命名法，结果也自动装配成功，这个就是开启`mapUnderscoreToCamelCase`产生的效果。
<a name="WUwtD"></a>
### 方式3
mapper xml中有个更强大的元素resultMap，通过这个元素可以定义查询结果的映射关系。
<a name="pyF55"></a>
#### Mapper xml
```xml
<resultMap id="orderModelMap2" type="com.javacode2018.chat05.demo1.model.OrderModel">
    <id column="id" property="id" />
    <result column="user_id" property="userId" />
    <result column="create_time" property="createTime" />
    <result column="up_time" property="upTime" />
</resultMap>

<select id="getById2" resultMap="orderModelMap2">
    <![CDATA[
    SELECT a.id,a.user_id,a.create_time,a.up_time FROM t_order a WHERE a.id = #{value}
    ]]>
</select>
```
上面resultMap有2个元素需要指定：

- id：resultMap标识
- type：将结果封装成什么类型，此处需要将结果分装为OrderModel

注意上面的select元素，有个resultMap，标识查询结果使用哪个resultMap进行映射，此处使用的是orderModelMap2，所以查询结果会按照orderModelMap2关联的resultMap进行映射。
<a name="IRja7"></a>
#### Mapper接口
```java
OrderModel getById2(int id);
```
<a name="zJWzC"></a>
#### 测试用例
```java
@Test
public void getById2() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById2(1);
        log.info("{}", orderModel);
    }
}
```
运行输出
```
14:12.518 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id,a.user_id,a.create_time,a.up_time FROM t_order a WHERE a.id = ? 
14:12.546 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 1(Integer)
14:12.564 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
14:12.564 [main] INFO  c.j.chat05.demo1.Demo1Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790)
```
<a name="miu1t"></a>
## 一对一关联查询(4种方式)
<a name="URUFy"></a>
### 需求
通过订单id查询订单的时候，将订单关联的用户信息也返回。<br />修改一下OrderModel代码，内部添加一个UserModel，如下：
```java
package com.javacode2018.chat05.demo2.model;

import lombok.*;

import java.util.List;

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

    //下单用户信息
    private UserModel userModel;
}
```
UserModel内容：
```java
package com.javacode2018.chat05.demo2.model;

import lombok.*;

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
```
<a name="p81TJ"></a>
### 方式1
<a name="ZHE7n"></a>
#### Mapper xml
```xml
<resultMap id="orderModelMap1" type="com.javacode2018.chat05.demo2.model.OrderModel">
    <id column="id" property="id" />
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <result column="user_id" property="userModel.id"/>
    <result column="name" property="userModel.name"/>
</resultMap>

<select id="getById1" resultMap="orderModelMap1">
    <![CDATA[
    SELECT
        a.id,
        a.user_id,
        a.create_time,
        a.up_time,
        b.name
    FROM
        t_order a,
        t_user b
    WHERE
        a.user_id = b.id
    AND a.id = #{value}
    ]]>
</select>
```
注意重点在于上面的这两行：
```xml
<result column="user_id" property="userModel.id"/>
<result column="name" property="userModel.name"/>
```
这个地方使用到了级联赋值，多级之间用.进行引用，此处只有一级，可以有很多级。
<a name="f6G0B"></a>
#### Mapper 接口
```java
OrderModel getById1(int id);
```
<a name="WFGDz"></a>
#### 测试用例
```java
@Before
public void before() throws IOException {
    //指定mybatis全局配置文件
    String resource = "demo2/mybatis-config.xml";
    //读取全局配置文件
    InputStream inputStream = Resources.getResourceAsStream(resource);
    //构建SqlSessionFactory对象
    SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
    this.sqlSessionFactory = sqlSessionFactory;
}

@Test
public void getById1() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById1(1);
        log.info("{}", orderModel);
    }
}
```
<a name="JMhHm"></a>
#### 运行输出
```
24:20.811 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id, a.user_id, a.create_time, a.up_time, b.name FROM t_order a, t_user b WHERE a.user_id = b.id AND a.id = ? 
24:20.843 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 1(Integer)
24:20.861 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 1
24:20.861 [main] INFO  c.j.chat05.demo2.Demo2Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790, userModel=UserModel(id=2, name=Java))
```
<a name="zU7wB"></a>
### 方式2
这次需要使用mapper xml中另外一个元素association，这个元素可以配置关联对象的映射关系，看示例。
<a name="jkY1N"></a>
#### Mapper xml
```xml
<resultMap id="orderModelMap2" type="com.javacode2018.chat05.demo2.model.OrderModel">
    <id column="id" property="id" />
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <association property="userModel">
        <id column="user_id" property="id"/>
        <result column="name" property="name" />
    </association>
</resultMap>

<select id="getById2" resultMap="orderModelMap2">
    <![CDATA[
    SELECT
        a.id,
        a.user_id,
        a.create_time,
        a.up_time,
        b.name
    FROM
        t_order a,
        t_user b
    WHERE
        a.user_id = b.id
    AND a.id = #{value}
    ]]>
</select>
```
注意上面下面这部分代码：
```xml
<association property="userModel">
  <id column="user_id" property="id"/>
  <result column="name" property="name" />
</association>
```
注意上面的property属性，这个就是配置sql查询结果和OrderModel.userModel对象的映射关系，将user_id和userModel中的id进行映射,name和userModel中的name进行映射。
<a name="e7OTd"></a>
#### Mapper接口
```java
OrderModel getById2(int id);
```
<a name="X2IFz"></a>
#### 测试用例
```java
@Test
public void getById2() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById2(1);
        log.info("{}", orderModel);
    }
}
```
<a name="qV3Qu"></a>
#### 运行结果
```
51:44.896 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id, a.user_id, a.create_time, a.up_time, b.name FROM t_order a, t_user b WHERE a.user_id = b.id AND a.id = ? 
51:44.925 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 1(Integer)
51:44.941 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
51:44.942 [main] INFO  c.j.chat05.demo2.Demo2Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790, userModel=UserModel(id=2, name=Java))
```
从结果的最后一行可以看出，所有字段的值映射都是ok的。
<a name="FPBES"></a>
### 方式3
先按照订单id查询订单数据，然后在通过订单中user_id去用户表查询用户数据，通过两次查询，组合成目标结果，Mybatis已经内置了这种操作，如下。
<a name="kkxUW"></a>
#### UserMapper.xml
先定义一个通过用户id查询用户信息的select元素，如下
```xml
<select id="getById" resultType="com.javacode2018.chat05.demo2.model.UserModel">
  <![CDATA[
  SELECT id,name FROM t_user where id = #{value}
  ]]>
</select>
```
<a name="i51g5"></a>
#### OrderModel.xml
```xml
<resultMap id="orderModelMap3" type="com.javacode2018.chat05.demo2.model.OrderModel">
    <id column="id" property="id" />
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <association property="userModel" select="com.javacode2018.chat05.demo2.mapper.UserMapper.getById" column="user_id" />
</resultMap>

<select id="getById3" resultMap="orderModelMap3">
    <![CDATA[
    SELECT
        a.id,
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
OrderModel.userModel属性的值来在于另外一个查询，这个查询是通过association元素的select属性指定的，此处使用的是
```
com.javacode2018.chat05.demo2.mapper.UserMapper.getById
```
这个查询是有条件的，条件通过association的column进行传递的，此处传递的是getById3查询结果中的user_id字段。
<a name="lH7rt"></a>
#### Mapper接口
```java
OrderModel getById3(int id);
```
<a name="brRUp"></a>
#### 测试用例
```java
@Test
public void getById3() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById3(1);
        log.info("{}", orderModel);
    }
}
```
<a name="dGgyZ"></a>
#### 运行输出
```
07:12.569 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById3 - ==>  Preparing: SELECT a.id, a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
07:12.600 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById3 - ==> Parameters: 1(Integer)
07:12.619 [main] DEBUG c.j.c.d.mapper.UserMapper.getById - ====>  Preparing: SELECT id,name FROM t_user where id = ? 
07:12.620 [main] DEBUG c.j.c.d.mapper.UserMapper.getById - ====> Parameters: 2(Integer)
07:12.625 [main] DEBUG c.j.c.d.mapper.UserMapper.getById - <====      Total: 1
07:12.625 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById3 - <==      Total: 1
07:12.625 [main] INFO  c.j.chat05.demo2.Demo2Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790, userModel=UserModel(id=2, name=Java))
```
从输出中可以看出有2次查询，先按照订单id查询订单，然后通过订单记录中用户id去用户表查询用户信息，最终执行了2次查询。<br />方式4<br />方式3中给第二个查询传递了一个参数，如果需要给第二个查询传递多个参数怎么办呢？可以这么写
```xml
<association property="属性" select="查询对应的select的id" column="{key1=父查询字段1,key2=父查询字段2,key3=父查询字段3}" />
```
这种相当于给子查询传递了一个map，子查询中 需要用过map的key获取对应的条件，看案例：<br />OrderMapper.xml
```xml
<resultMap id="orderModelMap4" type="com.javacode2018.chat05.demo2.model.OrderModel">
    <id column="id" property="id" />
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <association property="userModel" select="com.javacode2018.chat05.demo2.mapper.UserMapper.getById1" column="{uid1=user_id,uid2=create_time}" />
</resultMap>

<select id="getById4" resultMap="orderModelMap4">
    <![CDATA[
    SELECT
        a.id,
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
UserMapper.xml
```xml
<select id="getById1" resultType="com.javacode2018.chat05.demo2.model.UserModel">
    <![CDATA[
    SELECT id,name FROM t_user where id = #{uid1} and id = #{uid2}
    ]]>
</select>
```
Mapper接口
```java
OrderModel getById4(int id);
```
测试用例
```java
@Test
public void getById4() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById4(1);
        log.info("{}", orderModel);
    }
}
```
运行输出
```
19:59.881 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById4 - ==>  Preparing: SELECT a.id, a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
19:59.914 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById4 - ==> Parameters: 1(Integer)
19:59.934 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====>  Preparing: SELECT id,name FROM t_user where id = ? and id = ? 
19:59.934 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - ====> Parameters: 2(Integer), 1577947790(Long)
19:59.939 [main] DEBUG c.j.c.d.mapper.UserMapper.getById1 - <====      Total: 0
19:59.939 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById4 - <==      Total: 1
19:59.939 [main] INFO  c.j.chat05.demo2.Demo2Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790, userModel=null)
```
输出中看一下第二个查询的条件，传过来的是第一个查询的user_id和create_time。
<a name="dBjDy"></a>
## 一对多查询(2种方式)
<a name="E5I6t"></a>
### 需求
根据订单id查询出订单信息，并且查询出订单明细列表。<br />先修改一下OrderModel代码，如下：
```java
package com.javacode2018.chat05.demo3.model;

import lombok.*;

import java.util.List;

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
    //订单详情列表
    private List<OrderDetailModel> orderDetailModelList;
}
```
OrderModel中添加了一个集合orderDetailModelList用来存放订单详情列表。
<a name="H4xJ9"></a>
### 方式1
<a name="nRRwl"></a>
#### OrderMapper.xml
```xml
<resultMap id="orderModelMap1" type="com.javacode2018.chat05.demo3.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <collection property="orderDetailModelList" ofType="com.javacode2018.chat05.demo3.model.OrderDetailModel">
        <id column="orderDetailId" property="id"/>
        <result column="order_id" property="orderId"/>
        <result column="goods_id" property="goodsId"/>
        <result column="num" property="num"/>
        <result column="total_price" property="totalPrice"/>
    </collection>
</resultMap>

<select id="getById1" resultMap="orderModelMap1">
    <![CDATA[
    SELECT
        a.id ,
        a.user_id,
        a.create_time,
        a.up_time,
        b.id orderDetailId,
        b.order_id,
        b.goods_id,
        b.num,
        b.total_price
    FROM
        t_order a,
        t_order_detail b
    WHERE
        a.id = b.order_id
        AND a.id = #{value}
    ]]>
</select>
```
注意上面的getById1中的sql，这个sql中使用到了t_order和t_order_detail连接查询，这个查询会返回多条结果，但是最后结果按照orderModelMap1进行映射，最后只会返回一个OrderModel对象，关键在于collection元素，这个元素用来定义集合中元素的映射关系，有2个属性需要注意：

- property：对应的属性名称
- ofType：集合中元素的类型，此处是OrderDetailModel

原理是这样的，注意orderModelMap1中有个
```xml
<id column="id" property="id"/>
```
查询出来的结果会按照这个配置中指定的column进行分组，即按照订单id进行分组，每个订单对应多个订单明细，订单明细会按照collection的配置映射为ofType元素指定的对象。<br />实际resultMap元素中的id元素可以使用result元素代替，只是用id可以提升性能，Mybatis可以通过id元素配置的列的值判断唯一一条记录，如果使用result元素，那么判断是否是同一条记录的时候，需要通过所有列去判断了，所以通过id可以提升性能，使用id元素在一对多中可以提升性能，在单表查询中使用id元素还是result元素，性能都是一样的。
<a name="do92s"></a>
#### Mapper接口
```java
OrderModel getById1(Integer id);
```
<a name="K1kfe"></a>
#### 测试用例
```java
@Before
public void before() throws IOException {
    //指定mybatis全局配置文件
    String resource = "demo3/mybatis-config.xml";
    //读取全局配置文件
    InputStream inputStream = Resources.getResourceAsStream(resource);
    //构建SqlSessionFactory对象
    SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
    this.sqlSessionFactory = sqlSessionFactory;
}

@Test
public void getById1() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        Integer id = 1;
        OrderModel orderModel = mapper.getById1(id);
        log.info("{}", orderModel);
    }
}
```
<a name="ZEMvA"></a>
#### 运行输出
```
03:52.092 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time, b.id orderDetailId, b.order_id, b.goods_id, b.num, b.total_price FROM t_order a, t_order_detail b WHERE a.id = b.order_id AND a.id = ? 
03:52.124 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - ==> Parameters: 1(Integer)
03:52.148 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById1 - <==      Total: 2
03:52.148 [main] INFO  c.j.chat05.demo3.Demo3Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790, orderDetailModelList=[OrderDetailModel(id=1, orderId=1, goodsId=1, num=2, totalPrice=17.76), OrderDetailModel(id=2, orderId=1, goodsId=1, num=1, totalPrice=16.66)])
```
注意最后一条输出，和期望的结果一致。
<a name="QvdWk"></a>
### 方式2
通过2次查询，然后对结果进行分装，先通过订单id查询订单信息，然后通过订单id查询订单明细列表，然后封装结果。Mybatis中默认支持这么玩，还是通过collection元素来实现的。
<a name="swbOm"></a>
#### OrderDetailMapper.xml
```xml
<select id="getListByOrderId1" resultType="com.javacode2018.chat05.demo3.model.OrderDetailModel" parameterType="int">
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
        a.order_id = #{value}
    ]]>
</select>
```
<a name="M6TUF"></a>
#### OrderMapper.xml
```xml
<resultMap id="orderModelMap2" type="com.javacode2018.chat05.demo3.model.OrderModel">
    <id column="id" property="id"/>
    <result column="user_id" property="userId"/>
    <result column="create_time" property="createTime"/>
    <result column="up_time" property="upTime"/>
    <collection property="orderDetailModelList" select="com.javacode2018.chat05.demo3.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
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
重点在于下面这句配置：
```xml
<collection property="orderDetailModelList" select="com.javacode2018.chat05.demo3.mapper.OrderDetailMapper.getListByOrderId1" column="id"/>
```
表示orderDetailModelList属性的值通过select属性指定的查询获取，即：
```
com.javacode2018.chat05.demo3.mapper.OrderDetailMapper.getListByOrderId1
```
查询参数是通过column属性指定的，此处使用getById2 sql中的id作为条件，即订单id。
<a name="jrgXA"></a>
#### Mapper接口
```java
OrderModel getById2(int id);
```
<a name="mQ2us"></a>
#### 测试用例
```java
@Test
public void getById2() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        OrderMapper mapper = sqlSession.getMapper(OrderMapper.class);
        OrderModel orderModel = mapper.getById2(1);
        log.info("{}", orderModel);
    }
}
```
<a name="qNO63"></a>
#### 运行输出
```
10:07.087 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==>  Preparing: SELECT a.id , a.user_id, a.create_time, a.up_time FROM t_order a WHERE a.id = ? 
10:07.117 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - ==> Parameters: 1(Integer)
10:07.135 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ====>  Preparing: SELECT a.id, a.order_id AS orderId, a.goods_id AS goodsId, a.num, a.total_price AS totalPrice FROM t_order_detail a WHERE a.order_id = ? 
10:07.136 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - ====> Parameters: 1(Integer)
10:07.141 [main] DEBUG c.j.c.d.m.O.getListByOrderId1 - <====      Total: 2
10:07.142 [main] DEBUG c.j.c.d.mapper.OrderMapper.getById2 - <==      Total: 1
10:07.142 [main] INFO  c.j.chat05.demo3.Demo3Test - OrderModel(id=1, userId=2, createTime=1577947790, upTime=1577947790, orderDetailModelList=[OrderDetailModel(id=1, orderId=1, goodsId=1, num=2, totalPrice=17.76), OrderDetailModel(id=2, orderId=1, goodsId=1, num=1, totalPrice=16.66)])
```
输出中有2次查询，先通过订单id查询订单信息，然后通过订单id查询订单明细，Mybatis内部对结果进行了组装。
<a name="Ckkk5"></a>
## 总结

1. Mybatis全局配置文件中通过`mapUnderscoreToCamelCase`可以开启sql中的字段和javabean中的骆驼命名法的字段进行自动映射
2. 掌握resultMap元素常见的用法
3. 一对一关联查询使用resultMap->association元素（2种方式）
4. 一对多查询使用resultMap->collection元素（2种方式）
5. resultMap中使用id元素主要在复杂的关联查询中可以提升效率，可以通过这个来判断记录的唯一性，如果没有这个，需要通过所有的result相关的列才能判断记录的唯一性

Mybatis提供了强大的关联查询，不过个人建议尽量少用，最好是采用单表的方式查询，在程序中通过多次查询，然后自己对结果进行组装。<br />Model中最好只定义一些和单表字段关联的属性，不要掺杂着其他对象的引用。
