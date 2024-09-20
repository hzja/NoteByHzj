Java SpringBoot
<a name="rfD36"></a>
## 一、介绍
在实际的软件系统开发过程中，由于业务的需求，在代码层面实现数据的脱敏还是远远不够的，往往还需要在数据库层面针对某些关键性的敏感信息，例如：身份证号、银行卡号、手机号、工资等信息进行加密存储，实现真正意义的数据混淆脱敏，以满足信息安全的需要。<br />那在实际的研发过程中，如何实践呢？
<a name="grOU8"></a>
## 二、方案实践
在此，提供三套方案以供大家选择。

- 通过 SQL 函数实现加解密
- 对 SQL 进行解析拦截，实现数据加解密
- 自定义一套脱敏工具
<a name="yfWe2"></a>
### 2.1、通过 SQL 函数实现加解密
最简单的方法，莫过于直接在数据库层面操作，通过函数对某个字段进行加、解密，例如如下这个案例！
```sql
-- 对“你好，世界”进行加密
select HEX(AES_ENCRYPT('你好，世界','ABC123456'));

-- 解密，输出：你好，世界
select AES_DECRYPT(UNHEX('A174E3C13FE16AA0FD071A4BBD7CD7C5'),'ABC123456');
```
采用Mysql内置的AES协议加、解密函数，密钥是ABC123456，可以很轻松的对某个字段实现加、解密。<br />如果是很小的需求，需要加密的数据就是指定的信息，此方法可行。<br />但是当需要加密的表字段非常多的时候，这个使用起来就比较鸡肋了，例如更改加密算法或者不同的部署环境配置不同的密钥，这个时候就不得不把所有的代码进行更改一遍。
<a name="Pvh3F"></a>
### 2.2、对 SQL 进行解析拦截，实现数据加解密
通过上面的方案，可以发现最大的痛点就是加密算法和密钥都写死在SQL上了，因此可以将这块的服务从抽出来，在JDBC层面，当sql执行的时候，对其进行拦截处理。<br />Apache ShardingSphere 框架下的数据脱敏模块，它就可以实现这一需求，如果是SpringBoot项目，可以实现无缝集成，对原系统的改造会非常少。<br />下面以用户表为例，来看看采用ShardingSphere如何实现！
<a name="MCuSu"></a>
#### 2.2.1、创建用户表
```sql
CREATE TABLE user (
  id bigint(20) NOT NULL COMMENT '用户ID',
  email varchar(255)  NOT NULL DEFAULT '' COMMENT '邮件',
  nick_name varchar(255)  DEFAULT NULL COMMENT '昵称',
  pass_word varchar(255)  NOT NULL DEFAULT '' COMMENT '二次密码',
  reg_time varchar(255)  NOT NULL DEFAULT '' COMMENT '注册时间',
  user_name varchar(255)  NOT NULL DEFAULT '' COMMENT '用户名',
  salary varchar(255) DEFAULT NULL COMMENT '基本工资',
  PRIMARY KEY (id) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
```
<a name="lezhj"></a>
#### 2.2.2、创建 springboot 项目并添加依赖包
```xml
<dependencies>
    <!--spring boot核心-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>
    <!--spring boot 测试-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
    </dependency>
    <!--springmvc web-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <!--mysql 数据源-->
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
    </dependency>
    <!--mybatis 支持-->
    <dependency>
        <groupId>org.mybatis.spring.boot</groupId>
        <artifactId>mybatis-spring-boot-starter</artifactId>
        <version>2.0.0</version>
    </dependency> 
    <!--shardingsphere数据分片、脱敏工具-->
    <dependency>
        <groupId>org.apache.shardingsphere</groupId>
        <artifactId>sharding-jdbc-spring-boot-starter</artifactId>
        <version>4.1.0</version>
    </dependency>
    <dependency>
        <groupId>org.apache.shardingsphere</groupId>
        <artifactId>sharding-jdbc-spring-namespace</artifactId>
        <version>4.1.0</version>
    </dependency>
</dependencies>
```
<a name="rkESY"></a>
#### 2.2.3、添加脱敏配置
在application.properties文件中，添加shardingsphere相关配置，即可实现针对某个表进行脱敏
```properties
server.port=8080

logging.path=log

#shardingsphere数据源集成
spring.shardingsphere.datasource.name=ds
spring.shardingsphere.datasource.ds.type=com.zaxxer.hikari.HikariDataSource
spring.shardingsphere.datasource.ds.driver-class-name=com.mysql.cj.jdbc.Driver
spring.shardingsphere.datasource.ds.jdbc-url=jdbc:mysql://127.0.0.1:3306/test
spring.shardingsphere.datasource.ds.username=xxxx
spring.shardingsphere.datasource.ds.password=xxxx

#加密方式、密钥配置
spring.shardingsphere.encrypt.encryptors.encryptor_aes.type=aes
spring.shardingsphere.encrypt.encryptors.encryptor_aes.props.aes.key.value=hkiqAXU6Ur5fixGHaO4Lb2V2ggausYwW
#plainColumn表示明文列，cipherColumn表示脱敏列
spring.shardingsphere.encrypt.tables.user.columns.salary.plainColumn=
spring.shardingsphere.encrypt.tables.user.columns.salary.cipherColumn=salary
#spring.shardingsphere.encrypt.tables.user.columns.pass_word.assistedQueryColumn=
spring.shardingsphere.encrypt.tables.user.columns.salary.encryptor=encryptor_aes

#sql打印
spring.shardingsphere.props.sql.show=true
spring.shardingsphere.props.query.with.cipher.column=true


#基于xml方法的配置
mybatis.mapper-locations=classpath:mapper/*.xml
```
其中下面的配置信息是关键的一部，`spring.shardingsphere.encrypt.tables`是指要脱敏的表，user是表名，salary表示user表中的真实列，其中`plainColumn`指的是明文列，`cipherColumn`指的是脱敏列，如果是新工程，只需要配置脱敏列即可！
```properties
spring.shardingsphere.encrypt.tables.user.columns.salary.plainColumn=
spring.shardingsphere.encrypt.tables.user.columns.salary.cipherColumn=salary
#spring.shardingsphere.encrypt.tables.user.columns.pass_word.assistedQueryColumn=
spring.shardingsphere.encrypt.tables.user.columns.salary.encryptor=encryptor_aes
```
<a name="BFTCw"></a>
#### 2.2.4、编写数据持久层
```xml
<mapper namespace="com.example.shardingsphere.mapper.UserMapperXml" >

    <resultMap id="BaseResultMap" type="com.example.shardingsphere.entity.UserEntity" >
        <id column="id" property="id" jdbcType="BIGINT" />
        <result column="email" property="email" jdbcType="VARCHAR" />
        <result column="nick_name" property="nickName" jdbcType="VARCHAR" />
        <result column="pass_word" property="passWord" jdbcType="VARCHAR" />
        <result column="reg_time" property="regTime" jdbcType="VARCHAR" />
        <result column="user_name" property="userName" jdbcType="VARCHAR" />
        <result column="salary" property="salary" jdbcType="VARCHAR" />
    </resultMap>

    <select id="findAll" resultMap="BaseResultMap">
        SELECT * FROM user
    </select>
    
    <insert id="insert" parameterType="com.example.shardingsphere.entity.UserEntity">
        INSERT INTO user(id,email,nick_name,pass_word,reg_time,user_name, salary)
        VALUES(#{id},#{email},#{nickName},#{passWord},#{regTime},#{userName}, #{salary})
    </insert>
</mapper>
```
```java
public interface UserMapperXml {


    /**
     * 查询所有的信息
     * @return
     */
    List<UserEntity> findAll();

    /**
     * 新增数据
     * @param user
     */
    void insert(UserEntity user);
}

public class UserEntity {

    private Long id;

    private String email;

    private String nickName;

    private String passWord;

    private String regTime;

    private String userName;

    private String salary;

 //省略set、get...

}
```
<a name="VUNiu"></a>
#### 2.2.5、最后来测试一下程序运行情况
编写启用服务程序
```java
@SpringBootApplication
@MapperScan("com.example.shardingsphere.mapper")
public class ShardingSphereApplication {

    public static void main(String[] args) {
        SpringApplication.run(ShardingSphereApplication.class, args);
    }
}
```
编写单元测试
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ShardingSphereApplication.class)
public class UserTest {

    @Autowired
    private UserMapperXml userMapperXml;

    @Test
    public void insert() throws Exception {
        UserEntity entity = new UserEntity();
        entity.setId(3l);
        entity.setEmail("123@123.com");
        entity.setNickName("阿三");
        entity.setPassWord("123");
        entity.setRegTime("2021-10-10 00:00:00");
        entity.setUserName("张三");
        entity.setSalary("2500");
        userMapperXml.insert(entity);
    }

    @Test
    public void query() throws Exception {
        List<UserEntity> dataList = userMapperXml.findAll();
        System.out.println(JSON.toJSONString(dataList));
    }
}
```
插入数据后，如下图，数据库存储的数据已被加密！<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635383619778-7054aa15-4ef8-4fa9-895e-9af1e060840a.webp#averageHue=%23c8cec6&clientId=ud4642cdc-9a2c-4&errorMessage=unknown%20error&from=paste&id=u2a11765f&originHeight=354&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=shadow&taskId=u79ffb1ae-8dae-4a1a-89f6-b43f83d48e7&title=)<br />继续来看看，运行查询服务，结果如下图，数据被成功解密！<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635383619811-a5868187-61ef-4172-aa3f-be00da982c02.webp#averageHue=%23fefdfd&clientId=ud4642cdc-9a2c-4&errorMessage=unknown%20error&from=paste&id=u085efd7b&originHeight=572&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=shadow&taskId=u8e29a1d4-26fa-4a28-b8d8-2877ce254af&title=)<br />采用配置方式，最大的好处就是直接通过配置脱敏列就可以完成对某些数据表字段的脱敏，非常方便。
<a name="Ud99M"></a>
### 2.3、自定义一套脱敏工具
可能会觉得shardingsphere配置虽然简单，但是还是不放心，里面的很多规则自己无法掌控，想自己开发一套数据库的脱敏工具。<br />方案也是有的，例如如下这套实践方案，以Mybatis为例：

- 首先编写一套加解密的算法工具类
- 通过Mybatis的typeHandler插件，实现特定字段的加解密

实践过程如下：
<a name="Gaone"></a>
#### 2.3.1、加解密工具类
```java
public class AESCryptoUtil {

    private static final Logger log = LoggerFactory.getLogger(AESCryptoUtil.class);

    private static final String DEFAULT_ENCODING = "UTF-8";
    private static final String AES = "AES";


    /**
     * 加密
     *
     * @param content 需要加密内容
     * @param key     任意字符串
     * @return
     * @throws Exception
     */
    public static String encryptByRandomKey(String content, String key) {
        try {
            //构造密钥生成器,生成一个128位的随机源,产生原始对称密钥
            KeyGenerator keygen = KeyGenerator.getInstance(AES);
            SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
            random.setSeed(key.getBytes());
            keygen.init(128, random);
            byte[] raw = keygen.generateKey().getEncoded();
            SecretKey secretKey = new SecretKeySpec(raw, AES);
            Cipher cipher = Cipher.getInstance(AES);
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            byte[] encrypted = cipher.doFinal(content.getBytes("utf-8"));
            return Base64.getEncoder().encodeToString(encrypted);
        } catch (Exception e) {
            log.warn("AES加密失败,参数:{}，错误信息:{}", content, e);
            return "";
        }
    }

    public static String decryptByRandomKey(String content, String key) {
        try {
            //构造密钥生成器,生成一个128位的随机源,产生原始对称密钥
            KeyGenerator generator = KeyGenerator.getInstance(AES);
            SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
            random.setSeed(key.getBytes());
            generator.init(128, random);
            SecretKey secretKey = new SecretKeySpec(generator.generateKey().getEncoded(), AES);
            Cipher cipher = Cipher.getInstance(AES);
            cipher.init(Cipher.DECRYPT_MODE, secretKey);
            byte[] encrypted = Base64.getDecoder().decode(content);
            byte[] original = cipher.doFinal(encrypted);
            return new String(original, DEFAULT_ENCODING);
        } catch (Exception e) {
            log.warn("AES解密失败,参数:{}，错误信息:{}", content, e);
            return "";
        }
    }

    public static void main(String[] args) {
        String encryptResult = encryptByRandomKey("Hello World", "123456");
        System.out.println(encryptResult);
        String decryptResult = decryptByRandomKey(encryptResult, "123456");
        System.out.println(decryptResult);
    }
}
```
<a name="tIWrD"></a>
#### 2.3.2、针对 salary 字段进行单独解析
```xml
<mapper namespace="com.example.shardingsphere.mapper.UserMapperXml" >

    <resultMap id="BaseResultMap" type="com.example.shardingsphere.entity.UserEntity" >
        <id column="id" property="id" jdbcType="BIGINT" />
        <result column="email" property="email" jdbcType="VARCHAR" />
        <result column="nick_name" property="nickName" jdbcType="VARCHAR" />
        <result column="pass_word" property="passWord" jdbcType="VARCHAR" />
        <result column="reg_time" property="regTime" jdbcType="VARCHAR" />
        <result column="user_name" property="userName" jdbcType="VARCHAR" />
        <result column="salary" property="salary" jdbcType="VARCHAR"
                typeHandler="com.example.shardingsphere.handle.EncryptDataRuleTypeHandler"/>
    </resultMap>

    <select id="findAll" resultMap="BaseResultMap">
        select * from user
    </select>
    
    <insert id="insert" parameterType="com.example.shardingsphere.entity.UserEntity">
        INSERT INTO user(id,email,nick_name,pass_word,reg_time,user_name, salary)
        VALUES(
        #{id},
        #{email},
        #{nickName},
        #{passWord},
        #{regTime},
        #{userName},
        #{salary,jdbcType=INTEGER,typeHandler=com.example.shardingsphere.handle.EncryptDataRuleTypeHandler})
    </insert>
</mapper>
```
EncryptDataRuleTypeHandler解析器，内容如下：
```java
public class EncryptDataRuleTypeHandler implements TypeHandler<String> {

    private static final String EMPTY = "";

    /**
     * 写入数据
     * @param preparedStatement
     * @param i
     * @param data
     * @param jdbcType
     * @throws SQLException
     */
    @Override
    public void setParameter(PreparedStatement preparedStatement, int i, String data, JdbcType jdbcType) throws SQLException {
        if (StringUtils.isEmpty(data)) {
            preparedStatement.setString(i, EMPTY);
        } else {
            preparedStatement.setString(i, AESCryptoUtil.encryptByRandomKey(data, "123456"));
        }
    }

    /**
     * 读取数据
     * @param resultSet
     * @param columnName
     * @return
     * @throws SQLException
     */
    @Override
    public String getResult(ResultSet resultSet, String columnName) throws SQLException {
        return decrypt(resultSet.getString(columnName));
    }

    /**
     * 读取数据
     * @param resultSet
     * @param columnIndex
     * @return
     * @throws SQLException
     */
    @Override
    public String getResult(ResultSet resultSet, int columnIndex) throws SQLException {
        return decrypt(resultSet.getString(columnIndex));
    }

    /**
     * 读取数据
     * @param callableStatement
     * @param columnIndex
     * @return
     * @throws SQLException
     */
    @Override
    public String getResult(CallableStatement callableStatement, int columnIndex) throws SQLException {
        return decrypt(callableStatement.getString(columnIndex));
    }

    /**
     * 对数据进行解密
     * @param data
     * @return
     */
    private String decrypt(String data) {
        return AESCryptoUtil.decryptByRandomKey(data, "123456");
    }
}
```
<a name="wbmXt"></a>
#### 2.3.3、单元测试
再次运行单元测试，程序读写正常！<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635383619793-07d37297-3aef-4e8a-9d9c-af8659b12010.webp#averageHue=%23b6bfb4&clientId=ud4642cdc-9a2c-4&errorMessage=unknown%20error&from=paste&id=u9416ecb8&originHeight=295&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=shadow&taskId=u2a4cff79-8ca2-4e0a-a874-96555f72eeb&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635383619901-bde479f3-f2d1-4144-8e9a-afcd1729f864.webp#averageHue=%23eeeeee&clientId=ud4642cdc-9a2c-4&errorMessage=unknown%20error&from=paste&id=u0cb3e8f5&originHeight=220&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=shadow&taskId=u5b1e06ac-684d-42f4-86f6-7984dbeb851&title=)<br />通过如下的方式，也可以实现对数据表中某个特定字段进行数据脱敏处理！
