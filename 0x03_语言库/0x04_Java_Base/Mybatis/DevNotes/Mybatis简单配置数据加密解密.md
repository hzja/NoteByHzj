Java Mybatis
<a name="twpZp"></a>
## 一、背景
在数据库中有些时候会保存一些用户的敏感信息，比如：手机号、银行卡等信息，如果这些信息以明文的方式保存，那么是不安全的。假如：黑客黑进了数据库，或者离职人员导出了数据，那么就可能导致这些敏感数据的泄漏。因此就需要找到一种方法来解决这个问题。
<a name="TU10f"></a>
## 二、解决方案
由于系统中使用了Mybatis作为数据库持久层，因此决定使用Mybatis的TypeHandler或Plugin来解决。<br />TypeHandler :  需要在某些列上手动指定 typeHandler 来选择使用那个typeHandler或者根据`@MappedJdbcTypes` 和 `@MappedTypes`注解来自行推断。
```xml
<result column="phone" property="phone" 
typeHandler="com.huan.study.mybatis.typehandler.EncryptTypeHandler"/>
```
Plugin : 可以拦截系统中的 select、insert、update、delete等语句，也能获取到sql执行前的参数和执行后的数据。<br />经过考虑，决定使用TypeHandler来加解密数据。
<a name="pDbum"></a>
## 三、需求
有一张客户表customer，里面有客户手机号(phone)和客户地址(address)等字段，其中客户手机号(phone)是需要加密保存到数据库中的。<br />1、在添加客户信息时，自动将客户手机号加密保存到数据中。<br />2、在查询客户信息时，自动解密客户手机号。
<a name="RUVZD"></a>
## 四、实现思路
<a name="keHrj"></a>
#### 1、编写一个实体类，凡是此实体类的数据都表示需要加解密的
```java
public class Encrypt {
    private String value;

    public Encrypt() {
    }

    public Encrypt(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }
}
```
<a name="vQxrP"></a>
#### 2、编写一个加解密的TypeHandler

- 设置参数时，加密数据。
- 从数据库获取记录时，解密数据。
```java
import cn.hutool.crypto.SecureUtil;
import cn.hutool.crypto.symmetric.AES;
import org.apache.ibatis.type.BaseTypeHandler;
import org.apache.ibatis.type.JdbcType;
import org.apache.ibatis.type.MappedJdbcTypes;
import org.apache.ibatis.type.MappedTypes;

import java.nio.charset.StandardCharsets;
import java.sql.CallableStatement;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * 加解密TypeHandler
 */
@MappedJdbcTypes(JdbcType.VARCHAR)
@MappedTypes(Encrypt.class)
public class EncryptTypeHandler extends BaseTypeHandler<Encrypt> {

    private static final byte[] KEYS = "12345678abcdefgh".getBytes(StandardCharsets.UTF_8);

    /**
     * 设置参数
     */
    @Override
    public void setNonNullParameter(PreparedStatement ps, int i, Encrypt parameter, JdbcType jdbcType) throws SQLException {
        if (parameter == null || parameter.getValue() == null) {
            ps.setString(i, null);
            return;
        }
        AES aes = SecureUtil.aes(KEYS);
        String encrypt = aes.encryptHex(parameter.getValue());
        ps.setString(i, encrypt);
    }

    /**
     * 获取值
     */
    @Override
    public Encrypt getNullableResult(ResultSet rs, String columnName) throws SQLException {
        return decrypt(rs.getString(columnName));
    }

    /**
     * 获取值
     */
    @Override
    public Encrypt getNullableResult(ResultSet rs, int columnIndex) throws SQLException {
        return decrypt(rs.getString(columnIndex));
    }

    /**
     * 获取值
     */
    @Override
    public Encrypt getNullableResult(CallableStatement cs, int columnIndex) throws SQLException {
        return decrypt(cs.getString(columnIndex));
    }

    public Encrypt decrypt(String value) {
        if (null == value) {
            return null;
        }
        return new Encrypt(SecureUtil.aes(KEYS).decryptStr(value));
    }
}
```
注意⚠️：

- `@MappedTypes`：表示该处理器处理的java类型是什么。
- `@MappedJdbcTypes`：表示处理器处理的Jdbc类型。
<a name="Evw7x"></a>
#### 3、sql语句中写法
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.huan.study.mybatis.mappers.CustomerMapper">

    <resultMap id="BaseResultMapper" type="com.huan.study.mybatis.entity.Customer">
        <id column="id" property="id"/>
        <result column="phone" property="phone"/>
        <result column="address" property="address"/>
    </resultMap>

    <insert id="addCustomer">
        insert into customer(phone,address) values (#{phone},#{address})
    </insert>

    <select id="findCustomer" resultMap="BaseResultMapper">
        select * from customer where phone = #{phone}
    </select>

</mapper>
```
SQL中没有什么特殊的写法。
<a name="CkklN"></a>
#### 4、配置文件中指定Typehandler的包路径
```
mybatis.type-handlers-package=com.fcant.study.mybatis.typehandler
```
<a name="fjjgI"></a>
#### 5、编写后台代码

- 提供一个添加方法
- 提供一个根据手机号查询的方法

mapper层的截图。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1641136563290-a2e044c1-565c-4509-9b4b-5816560126b8.png#clientId=ua8a968c0-deb6-4&from=paste&height=166&id=u08d6ca07&originHeight=497&originWidth=3240&originalType=binary&ratio=1&rotation=0&showTitle=false&size=193345&status=done&style=shadow&taskId=u618bf19b-a43a-4f03-81fc-1fd3cfe83dd&title=&width=1080)
<a name="i8I4i"></a>
#### 6、测试结果
![2022-01-02-22-54-46-214640.jpeg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1641135537639-2c3b6dee-8456-4171-b873-262bfb7c7fd6.jpeg#clientId=u12b9c0c3-dc7a-4&from=ui&id=Fb618&originHeight=500&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=90874&status=done&style=shadow&taskId=u7ad32ac3-f9c1-4177-8e54-6590e8c403e&title=)<br />从测试结果中可知，添加数据时，需要加密的数据(phone)在数据库中已经加密了，在查询的时候，加密的数据已经自动解密了。
