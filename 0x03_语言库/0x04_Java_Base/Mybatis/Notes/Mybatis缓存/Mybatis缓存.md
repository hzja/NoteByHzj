Mybatis
<a name="GM2Fi"></a>
## 什么是缓存？
缓存就是存储数据的一个地方（称作：Cache），当程序要读取数据时，会首先从缓存中获取，有则直接返回，否则从其他存储设备中获取，**缓存最重要的一点就是从其内部获取数据的速度是非常快的，通过缓存可以加快数据的访问速度。比如从db中获取数据，中间需要经过网络传输耗时，db server从磁盘读取数据耗时等，如果这些数据直接放在jvm对应的内存中，访问是不是会快很多。**
<a name="uut6r"></a>
## Mybatis中的缓存
通常情况下Mybatis会访问数据库获取数据，中间涉及到网络通信，数据库从磁盘中读取数据，然后将数据返回给Mybatis，总的来说耗时还是挺长的，Mybatis为了加快数据查询的速度，在其内部引入了缓存来加快数据的查询速度。<br />Mybatis中分为一级缓存和二级缓存。<br />一级缓存是`SqlSession`级别的缓存，在操作数据库时需要构造 `sqlSession`对象，在对象中有一个数据结构（HashMap）用于存储缓存数据，不同的`sqlSession`之间的缓存数据区域（HashMap）是互相不影响的。<br />二级缓存是mapper级别的缓存，多个`SqlSession`去操作同一个Mapper的sql语句，多个`SqlSession`可以共用二级缓存，二级缓存是跨`SqlSession`的。<br />下面详细说一下一级缓存和二级缓存的各种用法和注意点。
<a name="a63vb"></a>
## 一级缓存
**一级缓存是**`**SqlSession**`**级别的缓存，每个**`**SqlSession**`**都有自己单独的一级缓存，多个**`**SqlSession**`**之间的一级缓存是相互隔离的，互不影响，Mybatis中一级缓存是默认自动开启的。**<br />**一级缓存工作原理：在同一个**`**SqlSession**`**中去多次去执行同样的查询，每次执行的时候会先到一级缓存中查找，如果缓存中有就直接返回，如果一级缓存中没有相关数据，Mybatis就会去db中进行查找，然后将查找到的数据放入一级缓存中，第二次执行同样的查询的时候，会发现缓存中已经存在了，会直接返回。一级缓存的存储介质是内存，是用一个HashMap来存储数据的，所以访问速度是非常快的。**
<a name="sIdjK"></a>
### 一级缓存案例
<a name="IgiLF"></a>
#### 案例sql脚本
```sql
DROP DATABASE IF EXISTS `javacode2018`;
CREATE DATABASE `javacode2018`;

USE `javacode2018`;

DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '用户id',
  name VARCHAR(32) NOT NULL DEFAULT '' COMMENT '用户名',
  age SMALLINT NOT NULL DEFAULT 1 COMMENT '年龄'
) COMMENT '用户表';
INSERT INTO t_user VALUES (1,'Java',30),(2,'张学友',50),(3,'刘德华',50);
```
<a name="ZAWVK"></a>
#### 下面是查询用户信息，返回一个list
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo9.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </where>
</select>
```
<a name="xPXRc"></a>
#### 对应的mapper接口方法
```java
List<UserModel> getList1(Map<String, Object> paramMap);
```
<a name="fdF3J"></a>
#### 测试用例
```java
/**
 * 一级缓存测试
 *
 * @throws IOException
 */
@Test
public void level1CacheTest1() throws IOException {
    String mybatisConfig = "demo9/mybatis-config.xml";
    this.before(mybatisConfig);
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //第一次查询
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
        //第二次查询
        List<UserModel> userModelList2 = mapper.getList1(null);
        log.info("{}", userModelList2);
        log.info("{}", userModelList1 == userModelList2);
    }
}
```
**上面的代码在同一个SqlSession中去执行了2次获取用户列表信息，2次查询结果分别放在userModelList1和userModelList2，最终代码中也会判断这两个集合是否相等，下面运行一下看看会访问几次db？**
<a name="mVqIS"></a>
#### 运行输出
```
01:15.312 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
01:15.340 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
01:15.364 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 3
01:15.364 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50)]
01:15.367 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50)]
01:15.367 [main] INFO  c.j.chat05.demo9.Demo9Test - true
```
从输出中可以看出看到，sql只输出了一次，说明第一次会访问数据库，第二次直接从缓存中获取的，最后输出了一个true，也说明两次返回结果是同一个对象，第二次直接从缓存中获取数据的，加快了查询的速度。
<a name="ogGUp"></a>
### 清空一级缓存的3种方式
同一个`SqlSession`中查询同样的数据，Mybatis默认会从一级缓存中获取，如果缓存中没有，才会访问db，那么如何去情况一级缓存呢，强制让查询去访问db呢？<br />**让一级缓存失效有3种方式：**

1. `**SqlSession**`**中执行增、删、改操作，此时sqlsession会自动清理其内部的一级缓存**
2. **调用**`**SqlSession**`**中的clearCache方法清理其内部的一级缓存**
3. **设置Mapper xml中select元素的**`**flushCache**`**属性值为true，那么执行查询的时候会先清空一级缓存中的所有数据，然后去db中获取数据**

上面方式任何一种都会让当前`SqlSession`中的以及缓存失效，进而去db中获取数据，下面来分别演示这3种情况。
<a name="NWfQp"></a>
#### 方式1：增删改让一级缓存失效
当执行增删改操时，Mybatis会将当前`SqlSession`一级缓存中的所有数据都清除。
<a name="BS06j"></a>
##### 案例代码：
```java
/**
 * 增删改使一级缓存失效
 *
 * @throws IOException
 */
@Test
public void level1CacheTest2() throws IOException {
    String mybatisConfig = "demo9/mybatis-config.xml";
    this.before(mybatisConfig);
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //第一次查询
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
        //新增一条数据
        mapper.insert1(UserModel.builder().id(100).name("Tom").age(30).build());
        //第二次查询
        List<UserModel> userModelList2 = mapper.getList1(null);
        log.info("{}", userModelList2);
        log.info("{}", userModelList1 == userModelList2);
    }
}
```
上面同一个SqlSession中执行了3个操作，同样的查询执行了2次，2次查询中间夹了一个插入操作。
<a name="xbwm1"></a>
##### 运行输出
```
21:55.097 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
21:55.135 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
21:55.159 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 3
21:55.159 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50)]
21:55.161 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==>  Preparing: INSERT INTO t_user (id,name,age) VALUES (?,?,?) 
21:55.162 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==> Parameters: 100(Integer), Tom(String), 30(Integer)
21:55.165 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - <==    Updates: 1
21:55.166 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
21:55.166 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
21:55.167 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
21:55.168 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
21:55.168 [main] INFO  c.j.chat05.demo9.Demo9Test - false
```
从输出中可以看出2次查询都访问了db，并且两次查询的结果是不一样的，两个集合也不相等，插入数据让缓存失效是可以理解的，插入操作可能会改变数据库中的数据，所以如果再从缓存中去获取，可能获取到的数据和db中的数据不一致的情况，Mybatis为了避免这种情况，在执行插入操作的时候，会将`SqlSession`中的一级缓存清空。当然删除和修改也同样会改变db中的数据，如果在同一个`SqlSession`中去执行删除或者修改数据的时候，Mybatis也一样会清除一级缓存中的所有数据，删除和修改大家自己可以写2个例子试试，看看是否也会清理一级缓存中的数据。
<a name="FzvT7"></a>
#### 方式2：`SqlSession.clearCache`清理一级缓存
`SqlSession.clearCache()`方法会将当前`SqlSession`一级缓存中的所有数据清除。
<a name="DVpb6"></a>
##### 案例代码：
```java
/**
 * 调用sqlSession.clearCache()清理一级缓存
 *
 * @throws IOException
 */
@Test
public void level1CacheTest3() throws IOException {
    String mybatisConfig = "demo9/mybatis-config.xml";
    this.before(mybatisConfig);
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //第一次查询
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
        //调用clearCache方法清理当前SqlSession中的缓存
        sqlSession.clearCache();
        //第二次查询
        List<UserModel> userModelList2 = mapper.getList1(null);
        log.info("{}", userModelList2);
        log.info("{}", userModelList1 == userModelList2);
    }
}
```
<a name="lENql"></a>
##### 运行输出
```
31:21.937 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
31:21.966 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
31:21.985 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
31:21.985 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
31:21.988 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
31:21.988 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
31:21.989 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
31:21.989 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
31:21.990 [main] INFO  c.j.chat05.demo9.Demo9Test - false
```
从输出中可以看出，2次同样的查询都访问了db。
<a name="ON0AC"></a>
#### 方式3：Select元素的`flushCache`置为true
将Mapper xml中select元素的`flushCache`属性置为true的时候，每次执行这个select元素对应的查询之前，Mybatis会将当前`SqlSession`中一级缓存中的所有数据都清除。
<a name="fWz8U"></a>
##### 案例代码
新增一个select元素的查询，将`flushCache`元素置为true，注意：select元素这个属性的默认值是false。
```xml
<select id="getList2" flushCache="true" resultType="com.javacode2018.chat05.demo9.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </where>
</select>
```
<a name="Q3Kya"></a>
##### 对应测试用例
```java
/**
 * 将Mapper xml中select元素的flushCache属性置为true的时候，每次执行这个select元素对应的查询之前，mybatis会将当前SqlSession中一级缓存中的所有数据都清除。
 *
 * @throws IOException
 */
@Test
public void level1CacheTest4() throws IOException {
    String mybatisConfig = "demo9/mybatis-config.xml";
    this.before(mybatisConfig);
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //查询1：getList1
        log.info("查询1 start");
        log.info("查询1：getList1->{}", mapper.getList1(null));
        //查询2：getList1
        log.info("查询2 start");
        log.info("查询2：getList1->{}", mapper.getList1(null));
        //查询3：getList2
        log.info("查询3 start");
        log.info("查询3：getList2->{}", mapper.getList2(null));
        //查询4：getList2
        log.info("查询4 start");
        log.info("查询4：getList2->{}", mapper.getList2(null));
        //查询5：getList1
        log.info("查询5 start");
        log.info("查询5：getList1->{}", mapper.getList1(null));
    }
}
```
注意上面的代码，代码中有5次查询，第1次、第2次、第5次查询调用的都是getList1，这个查询对应的mapper xml中的select元素的flushCache属性没有设置，默认是false；而第3次和第4次查询调用的是getList2，getList2这个查询对应的mapper xml中的select(id=getList2)，它的flushCache属性设置的是true，说明第3和第4次查询会清空当前一级缓存中所有数据。<br />**最终效果应该是查询1访问db拿去数据，然后将其丢到一级缓存中，查询2会直接从一级缓存中拿到数据，而查询3走的是getList2，发现**`**flushCache**`**为true，会先清空一级缓存中所有数据，也就是此时查询1放入缓存的数据会被清理掉，然后查询3会访问db获取数据，然后丢到缓存中；而查询4走的是getList2，发现**`**flushCache**`**为true，会先清空缓存，所以3放入一级缓存的数据会被清空，然后导致查询4也会访问db，查询5去一级缓存中查询数据，因为查询1和2放入缓存的数据都被查询3清空了，所以导致查询5发现一级缓存中没有数据，也会访问db去获取数据。**<br />来运行一下看看是否和分析的一致。
<a name="h0zSG"></a>
##### 运行输出
```
20:10.872 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询1 start
20:11.164 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
20:11.195 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
20:11.216 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
20:11.216 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询1：getList1->[UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
20:11.218 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询2 start
20:11.218 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询2：getList1->[UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
20:11.219 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询3 start
20:11.219 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==>  Preparing: SELECT id,name,age FROM t_user 
20:11.219 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==> Parameters: 
20:11.222 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - <==      Total: 4
20:11.222 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询3：getList2->[UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
20:11.222 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询4 start
20:11.223 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==>  Preparing: SELECT id,name,age FROM t_user 
20:11.223 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==> Parameters: 
20:11.225 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - <==      Total: 4
20:11.225 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询4：getList2->[UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
20:11.225 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询5 start
20:11.225 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
20:11.225 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
20:11.230 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
20:11.230 [main] INFO  c.j.chat05.demo9.Demo9Test - 查询5：getList1->[UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
```
大家认真看一下上面的输出，查询1/3/4/5访问了db，查询2从缓存中获取的，和上面分析的过程一致。
<a name="dp3EU"></a>
### 一级缓存使用总结

1. **一级缓存是**`**SqlSession**`**级别的，每个人**`**SqlSession**`**有自己的一级缓存，不同的**`**SqlSession**`**之间一级缓存是相互隔离的**
2. **Mybatis中一级缓存默认是自动开启的**
3. **当在同一个**`**SqlSession**`**中执行同样的查询的时候，会先从一级缓存中查找，如果找到了直接返回，如果没有找到会去访问db，然后将db返回的数据丢到一级缓存中，下次查询的时候直接从缓存中获取**
4. **一级缓存清空的3种方式（1：**`**SqlSession**`**中执行增删改会使一级缓存失效；2：调用**`**SqlSession.clearCache**`**方法会使一级缓存失效；3：Mapper xml中的select元素的**`**flushCache**`**属性置为true，那么执行这个查询会使一级缓存失效）**
<a name="YmHzS"></a>
## 二级缓存
<a name="jalyn"></a>
### 二级缓存的使用
一级缓存使用上存在局限性，必须要在同一个`SqlSession`中执行同样的查询，一级缓存才能提升查询速度，如果想在不同的`SqlSession`之间使用缓存来加快查询速度，此时需要用到二级缓存了。<br />二级缓存是mapper级别的缓存，每个mapper xml有个namespace，二级缓存和namespace绑定的，每个namespace关联一个二级缓存，多个`SqlSession`可以共用二级缓存，二级缓存是跨`SqlSession`的。<br />二级缓存默认是没有开启的，需要在Mybatis全局配置文件中进行开启：
```xml
<settings>
  <!-- 开启二级缓存 -->
  <setting name="cacheEnabled" value="true"/>
</settings>
```
上面配置好了以后，还需要在对应的mapper xml加上下面配置，表示这个mapper中的查询开启二级缓存：
```xml
<cache/>
```
配置就这么简单。
<a name="zyw1B"></a>
### 一二级缓存共存时查询原理
一二级缓存如果都开启的情况下，数据查询过程如下：

1. **当发起一个查询的时候，Mybatis会先访问这个namespace对应的二级缓存，如果二级缓存中有数据则直接返回，否则继续向下**
2. **查询一级缓存中是否有对应的数据，如果有则直接返回，否则继续向下**
3. **访问db获取需要的数据，然后放在当前**`**SqlSession**`**对应的二级缓存中，并且在本地内存中的另外一个地方存储一份（这个地方就叫**`**TransactionalCache**`**）**
4. **当**`**SqlSession**`**关闭的时候，也就是调用**`**SqlSession**`**的close方法的时候，此时会将**`**TransactionalCache**`**中的数据放到二级缓存中，并且会清空当前**`**SqlSession**`**一级缓存中的数据。**
<a name="JrVre"></a>
### 二级缓存案例
<a name="L8wbR"></a>
#### Mybatis全局配置文件开启二级缓存配置
```
<settings>
    <!-- 开启二级缓存 -->
    <setting name="cacheEnabled" value="true"/>
</settings>
```
<a name="xEUna"></a>
#### mapper xml中使用cache元素开启二级缓存
```xml
<!-- 启用二级缓存 -->
<cache/>

<select id="getList1" resultType="com.javacode2018.chat05.demo9.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </where>
</select>
```
<a name="EeNr5"></a>
#### 测试用例
```java
/**
 * 二级缓存测试
 *
 * @throws IOException
 */
@Test
public void level2CacheTest1() throws IOException {
    String mybatisConfig = "demo9/mybatis-config1.xml";
    this.before(mybatisConfig);
    for (int i = 0; i < 2; i++) {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            List<UserModel> userModelList1 = mapper.getList1(null);
            log.info("{}", userModelList1);
        }
    }
}
```
上面执行了2次查询，每次查询都是新的SqlSession，运行一下看看效果。
<a name="fjKc5"></a>
#### 执行输出
```
34:36.574 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.0
34:36.831 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
34:36.864 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
34:36.883 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
34:36.883 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
34:36.894 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.5
34:36.895 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
```
注意上面第一行日志输出：
```
Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.0
```
对这行做一个解释：com.javacode2018.chat05.demo9.mapper.UserMapper是上面查询访问的mapper xml的namesapce的值，去这个namespace对应的二级缓存中去查询数据，没有查询到，输出中的0.0表示命中率，这次没有命中，所以命中率为0<br />然后就去db中访问数据了，会将db中返回的数据放在一级缓存中，第一次运行完毕之后会自动调用SqlSession的close方法，然后db中返回的数据会被丢到二级缓存中，第二次查询的时候就直接从二级缓存中获取到数据返回了，所以第二次查询输出如下：
```
34:36.894 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.5
34:36.895 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=100, name=Tom, age=30)]
```
2次查询都去访问了二级缓存，第二次命中了，命中率为1/2=0.5
<a name="EJZtf"></a>
### 清空或者跳过二级缓存的3种方式
当二级缓存开启的时候，在某个mapper xml中添加cache元素之后，这个mapper xml中所有的查询都默认开启了二级缓存，那么如何清空或者跳过二级缓存呢？3种方式如下：

1. **对应的mapper中执行增删改查会清空二级缓存中数据**
2. **select元素的**`**flushCache**`**属性置为true，会先清空二级缓存中的数据，然后再去db中查询数据，然后将数据再放到二级缓存中**
3. **select元素的**`**useCache**`**属性置为true，可以使这个查询跳过二级缓存，然后去查询数据**

下面来演示一下每种方式对应的效果。
<a name="rCxPa"></a>
#### 方式1：增删改会清除二级缓存中的数据
下面主要演示一下新增对二级缓存的影响。
<a name="BX8Va"></a>
##### 案例代码
```java
com.javacode2018.chat05.demo9.Demo9Test#level2CacheTest2

/**
 * 增删改会清除二级缓存中的数据
 *
 * @throws IOException
 */
@Test
public void level2CacheTest2() throws IOException {
    String mybatisConfig = "demo9/mybatis-config1.xml";
    this.before(mybatisConfig);
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
    }
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //新增一条数据
        mapper.insert1(UserModel.builder().id(Integer.valueOf(System.nanoTime() % 100000 + "")).name("Tom").age(30).build());
    }
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
    }
}
```
上面使用了3个不同的`SqlSession`，第一次和第三次都调用了getList1执行查询，中间执行了一个插入操作，Mybatis执行插入的时候，会先清除当前namespace对应的二级缓存中的数据，所以上面2次查询最终都会访问db，来运行一下看看效果。
<a name="byvFL"></a>
##### 运行输出
```
23:00.620 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.0
23:00.900 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
23:00.924 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
23:00.948 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 3
23:00.948 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50)]
23:00.951 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==>  Preparing: INSERT INTO t_user (id,name,age) VALUES (?,?,?) 
23:00.953 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==> Parameters: 79600(Integer), Tom(String), 30(Integer)
23:00.955 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - <==    Updates: 1
23:00.959 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.0
23:00.959 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
23:00.959 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
23:00.961 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
23:00.961 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=79600, name=Tom, age=30)]
23:00.961 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==>  Preparing: INSERT INTO t_user (id,name,age) VALUES (?,?,?) 
23:00.962 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==> Parameters: 94100(Integer), Tom(String), 30(Integer)
23:00.967 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - <==    Updates: 1
```
从输出中可以看出，2次查询都访问了db。上面演示的是插入会清空二级缓存的数据，同样删除和修改也会先清除当前namespace对应的二级缓存中的数据。
<a name="LWYBT"></a>
#### 方式2：select元素的`flushCache`属性置为true
当将mapper xml中select元素的flushCache属性置为true，会先清空二级缓存中的数据，然后再去db中查询数据，然后将数据再放到二级缓存中。
<a name="sh28n"></a>
##### 代码
```xml
<select id="getList2" flushCache="true" resultType="com.javacode2018.chat05.demo9.model.UserModel"
  parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </where>
</select>
```
<a name="ZWNpm"></a>
##### 测试用例
```java
/**
 * 当将mapper xml中select元素的flushCache属性置为true，会先清空二级缓存中的数据，然后再去db中查询数据，然后将数据再放到二级缓存中
 *
 * @throws IOException
 */
@Test
public void level2CacheTest3() throws IOException {
    String mybatisConfig = "demo9/mybatis-config1.xml";
    this.before(mybatisConfig);
    //先查询2次getList1,getList1第二次会从二级缓存中拿到数据
    log.info("getList1查询");
    for (int i = 0; i < 2; i++) {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            List<UserModel> userModelList1 = mapper.getList1(null);
            log.info("{}", userModelList1);
        }
    }
    //getList2的flushCache为true，所以查询之前会先将对应的二级缓存中的所有数据清空，所以二次都会访问db
    log.info("getList2查询");
    for (int i = 0; i < 2; i++) {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            List<UserModel> userModelList1 = mapper.getList2(null);
            log.info("{}", userModelList1);
        }
    }

    //二级缓存中没有getList1需要查找的数据了，所以这次访问getList1会去访问db
    log.info("getList1查询");
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
    }
}
```
<a name="l4HAY"></a>
##### 运行输出
```
02:51.560 [main] INFO  c.j.chat05.demo9.Demo9Test - getList1查询
02:51.842 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
02:51.871 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
02:51.891 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
02:51.892 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
02:51.905 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.5
02:51.906 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
02:51.906 [main] INFO  c.j.chat05.demo9.Demo9Test - getList2查询
02:51.906 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.3333333333333333
02:51.907 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==>  Preparing: SELECT id,name,age FROM t_user 
02:51.907 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==> Parameters: 
02:51.909 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - <==      Total: 4
02:51.909 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
02:51.910 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.5
02:51.913 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==>  Preparing: SELECT id,name,age FROM t_user 
02:51.913 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - ==> Parameters: 
02:51.914 [main] DEBUG c.j.c.d.mapper.UserMapper.getList2 - <==      Total: 4
02:51.914 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
02:51.915 [main] INFO  c.j.chat05.demo9.Demo9Test - getList1查询
02:51.915 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.4
02:51.915 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
02:51.915 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
02:51.917 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
02:51.917 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
```
第一次查询访问db，第二次查询从二级缓存中获取了数据，第3和第4查询访问的是getList2，这个查询会清空二级缓存中的数据，直接去db中查询，查询4执行完毕之后，二级缓存中只有第四次查询的数据，第5次查询去getList1中获取数据，此时二级缓存中没有，所以直接去db中获取了。
<a name="wxWkQ"></a>
#### 方式3：select元素的`useCache`置为false跳过二级缓存，但是不会情况二级缓存数据
新增一个select3查询，将`useCache`置为false，如下：
```xml
<select id="getList3" useCache="false" resultType="com.javacode2018.chat05.demo9.model.UserModel"
  parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </where>
</select>
```
对应的用例代码
```java
/**
 * select元素的useCache置为false跳过二级缓存，但是不会情况二级缓存数据
 *
 * @throws IOException
 */
@Test
public void level2CacheTest4() throws IOException {
    String mybatisConfig = "demo9/mybatis-config1.xml";
    this.before(mybatisConfig);
    //第一次查询访问getList1，会将数据丢到二级缓存中
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
    }

    //getList3对应的select的useCache为false，会跳过二级缓存，所以会直接去访问db
    for (int i = 0; i < 2; i++) {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            List<UserModel> userModelList1 = mapper.getList3(null);
            log.info("{}", userModelList1);
        }
    }

    //下面的查询又去执行了getList1，由于上面的第一次查询也是访问getList1会将数据放在二级缓存中，所以下面的查询会从二级缓存中获取到数据
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        List<UserModel> userModelList1 = mapper.getList1(null);
        log.info("{}", userModelList1);
    }
}
```
注意上面有4次查询，第一次查询访问getList1，会将数据丢到二级缓存中，而第二三次查询访问的是getList3，getList3对应的select的`useCache`为false，会跳过二级缓存，所以会直接去访问db，第四次查询也是访问getList1，由于第一次查询也是访问getList1会将数据放在二级缓存中，所以第4次查询直接从二级缓存中获取到了数据，运行输出：
```
13:38.454 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.0
13:38.852 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user 
13:38.898 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
13:38.929 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 4
13:38.930 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
13:38.941 [main] DEBUG c.j.c.d.mapper.UserMapper.getList3 - ==>  Preparing: SELECT id,name,age FROM t_user 
13:38.942 [main] DEBUG c.j.c.d.mapper.UserMapper.getList3 - ==> Parameters: 
13:38.945 [main] DEBUG c.j.c.d.mapper.UserMapper.getList3 - <==      Total: 4
13:38.945 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
13:38.946 [main] DEBUG c.j.c.d.mapper.UserMapper.getList3 - ==>  Preparing: SELECT id,name,age FROM t_user 
13:38.946 [main] DEBUG c.j.c.d.mapper.UserMapper.getList3 - ==> Parameters: 
13:38.952 [main] DEBUG c.j.c.d.mapper.UserMapper.getList3 - <==      Total: 4
13:38.952 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
13:38.957 [main] DEBUG c.j.chat05.demo9.mapper.UserMapper - Cache Hit Ratio [com.javacode2018.chat05.demo9.mapper.UserMapper]: 0.5
13:38.957 [main] INFO  c.j.chat05.demo9.Demo9Test - [UserModel(id=1, name=Java, age=30), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=5100, name=Tom, age=30)]
```
从输出中可以看出前面3次查询都访问db了，最后一次查询访问的是二级缓存命中了数据。4次查询，第1次和第4次会访问二级缓存，中间2次跳过了二级缓存，二级缓存命中了1次，所以最后一次输出的命中率是0.5
<a name="mU99J"></a>
## 总结

1. **一二级缓存访问顺序：一二级缓存都存在的情况下，会先访问二级缓存，然后再访问一级缓存，最后才会访问db，这个顺序大家理解一下**
2. **将mapper xml中select元素的**`**flushCache**`**属性置为false，最终会清除一级缓存所有数据，同时会清除这个select所在的namespace对应的二级缓存中所有的数据**
3. **将mapper xml中select元素的**`**useCache**`**置为false，会使这个查询跳过二级缓存**
4. **总体上来说使用缓存可以提升查询效率，这块知识掌握了，大家可以根据业务自行选择**
