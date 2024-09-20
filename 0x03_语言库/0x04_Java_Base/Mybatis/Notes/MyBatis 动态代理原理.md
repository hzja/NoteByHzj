JavaMybatis
<a name="Ze3qy"></a>
## 1、MyBatis简介
MyBatis是一个ORM工具，封装了JDBC的操作，简化业务编程；<br />Mybatis在web工程中，与Spring集成，提供业务读写数据库的能力。
<a name="EAxbT"></a>
## 2、使用步骤
<a name="xElEK"></a>
### 2.1 引入依赖
采用Maven包依赖管理，mybatis-3.5.5版本；同时需要数据库连接驱动
```xml
<dependency>
  <groupId>org.mybatis</groupId>
  <artifactId>mybatis</artifactId>
  <version>3.5.5</version>
</dependency>
<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
  <version>5.1.49</version>
</dependency>
```
<a name="PkTLH"></a>
### 2.2 配置文件
配置文件配置数据库连接源，及映射文件。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
 
<configuration>
    <environments default="development">
        <environment id="development">
            <transactionManager type="JDBC" />
            <!-- 数据库连接方式 -->
            <dataSource type="POOLED">
                <property name="driver" value="com.mysql.jdbc.Driver" />
                <property name="url" value="jdbc:mysql://localhost/user" />
                <property name="username" value="root" />
                <property name="password" value="123456" />
            </dataSource>
        </environment>
    </environments>
 
    <!-- 注册表映射文件 -->
    <mappers>
        <mapper resource="mybatis/User.xml"/>
    </mappers>
 
</configuration>
```
<a name="RrI16"></a>
### 2.3 接口定义
定义实体
```java
package com.xiongxin.mybatis.entity;
 
public class User {
 
    private String username;
    private String password;
  	...getter&&setter
}
```
接口定义
```java
package com.xiongxin.mybatis.mapper;
import com.xiongxin.mybatis.entity.User;
import java.util.List;
public interface UserMapper {
    List<User> queryUser();
}
```
定义映射文件
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
 
		<mapper namespace="com.xiongxin.mybatis.mapper.UserMapper">
 
    <select id="queryUser" resultType="com.xiongxin.mybatis.entity.User">
        select * from tbl_user
    </select>
 
</mapper>
```
<a name="nKXq9"></a>
### 2.4 加载执行
```java
package com.xiongxin.mybatis;
 
import com.alibaba.fastjson.JSON;
import com.xiongxin.mybatis.entity.User;
import com.xiongxin.mybatis.mapper.UserMapper;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
 
import java.io.IOException;
import java.io.Reader;
import java.util.List;
 
public class TestMain {
 
    public static void main(String[] args) throws IOException {
        String resource = "mybatis-config.xml";
        //加载 mybatis 的配置文件（它也加载关联的映射文件）
        Reader reader = Resources.getResourceAsReader(resource);
        //构建 sqlSession 的工厂
        SqlSessionFactory sessionFactory = new SqlSessionFactoryBuilder().build(reader);
        //创建能执行映射文件中 sql 的 sqlSession
        SqlSession session = sessionFactory.openSession();
        UserMapper userMapper = session.getMapper(UserMapper.class);
        List<User> users = userMapper.queryUser();
        System.out.println(JSON.toJSONString(users));
    }
 
}
---------------------------------
..consule print..
[{"password":"password","username":"xiongxin"}]
```
到这里，这个Mybatis的使用环节结束。<br />整个实现过程中，并未编写Mapper的实现类，框架是如何在无实现类的场景下实现接口方法返回的呢？<br />这里就不得不说到接口的动态代理方法了。
<a name="ehD1c"></a>
## 3、原理解析
![2022-12-12-10-12-51.277358100.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670811713768-2d7e8911-38d0-4880-a090-8d56d5db5fd7.jpeg#averageHue=%23faf9f8&clientId=u14adbe90-17b6-4&from=ui&id=ueaa35686&originHeight=751&originWidth=861&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54757&status=done&style=none&taskId=u6bb32328-1710-4321-95f5-03f61da58fe&title=)<br />SqlSession接口的实现中，获取Mapper的代理实现类<br />![2022-12-12-10-12-51.499213500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670811713750-4ed49f48-8466-43ac-a705-c7212b64af38.png#averageHue=%23f8f8f7&clientId=u14adbe90-17b6-4&from=ui&id=UQxZW&originHeight=692&originWidth=2206&originalType=binary&ratio=1&rotation=0&showTitle=false&size=143631&status=done&style=none&taskId=ud32d668b-62da-4954-bd7a-d0d7e39a7fa&title=)<br />使用了JDK动态代理的功能<br />![2022-12-12-10-12-52.006234800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670811714168-2e787d68-e84d-4be8-a7b2-bacf16bba407.png#averageHue=%23f9f8f7&clientId=u14adbe90-17b6-4&from=ui&id=LU3Uj&originHeight=1152&originWidth=2532&originalType=binary&ratio=1&rotation=0&showTitle=false&size=255559&status=done&style=none&taskId=ue3f0aadc-c718-43b6-8271-6d3ac05b660&title=)<br />代理类执行方法调用<br />![2022-12-12-10-12-52.508928000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670811747861-3b8b91e9-7ef6-4655-95c6-ff8fb3d856da.png#averageHue=%23f8f8f8&clientId=u14adbe90-17b6-4&from=ui&id=uf070d51e&originHeight=656&originWidth=2202&originalType=binary&ratio=1&rotation=0&showTitle=false&size=127692&status=done&style=none&taskId=u0e3553d1-4c09-42b8-8c15-89db91cc4af&title=)<br />方法调用中执行`MethodInvoker`<br />![2022-12-12-10-12-52.824633400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670811747879-939d83a4-795c-45e2-9058-03a1cb24a87d.png#averageHue=%23f9f8f8&clientId=u14adbe90-17b6-4&from=ui&id=ift6M&originHeight=1436&originWidth=2586&originalType=binary&ratio=1&rotation=0&showTitle=false&size=305947&status=done&style=none&taskId=u1c8ed856-e8b0-4372-a8a5-fe1be479918&title=)<br />最终执行`execue`方法。<br />![2022-12-12-10-12-53.430175600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670811748214-60be6df3-612d-405a-bb1a-dcf3c4cbd8f3.png#averageHue=%23f7f7f7&clientId=u14adbe90-17b6-4&from=ui&id=rSOfS&originHeight=1822&originWidth=2284&originalType=binary&ratio=1&rotation=0&showTitle=false&size=405859&status=done&style=none&taskId=ua3b3432e-a706-4ee9-98c0-cd46ba2900e&title=)<br />获取返回结果Result
<a name="JQHvo"></a>
## 4、手撕框架
前置知识：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670811907526-9e1e75d5-6413-450a-a9be-34296f1d6e60.png#averageHue=%23f4f4f3&clientId=u27504013-64c0-4&from=paste&height=359&id=uce56dd8b&originHeight=898&originWidth=2494&originalType=binary&ratio=1&rotation=0&showTitle=false&size=169608&status=done&style=none&taskId=u07d3f9fe-a11b-4f19-8d05-c3ab79eba6b&title=&width=997.6)<br />源码:
```xml
<dependencies>
    <dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>fastjson</artifactId>
        <version>1.2.74</version>
    </dependency>
    <dependency>
        <groupId>com.h2database</groupId>
        <artifactId>h2</artifactId>
        <version>1.4.199</version>
    </dependency>
</dependencies>
```
```java
package com.dbutil.session;
 
import java.lang.annotation.Retention;
import java.lang.annotation.Target;
import java.lang.reflect.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
 
import static java.lang.annotation.ElementType.*;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

public class SqlSession {
 
    public static Connection getConnH2() throws Exception {
        String url = "jdbc:h2:mem:db_h2;MODE=MYSQL;INIT=RUNSCRIPT FROM './src/main/resources/schema.sql'";
        String user = "root";
        String password = "123456";
        //1.加载驱动程序
        Class.forName("org.h2.Driver");
        //2.获得数据库链接
        Connection conn = DriverManager.getConnection(url, user, password);
        return conn;
    }
 
 
 
    /**
     * 自定义注解
     */
    @Target({TYPE, FIELD, METHOD})
    @Retention(RUNTIME)
    public @interface QueryList {
        public String value();
    }
 
    /**
     * 动态代理
     *
     * @param mapperInterface
     * @param <T>
     * @return
     */
    public static <T> T getMapper(Class<T> mapperInterface) {
        return (T) Proxy.newProxyInstance(mapperInterface.getClassLoader(), new Class[]{mapperInterface}, new MapperInvocationHandler());
    }
 
    /**
     * 代理类方法
     */
    public static class MapperInvocationHandler implements InvocationHandler {
 
        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            String sql = method.getAnnotation(QueryList.class).value();
            Class<?> returnType = method.getReturnType();
            //返回类型为List
            if (returnType == List.class) {
                Type genericReturnType = method.getGenericReturnType();
                String typeName = genericReturnType.getTypeName();
                String replace = typeName.replace("java.util.List<", "").replace(">", "");
                //获取泛型类型
                Class<?> forName = Class.forName(replace);
                return SqlSession.queryList(sql, forName);
            }
            return null;
        }
    }
 
    /**
     * 结果集转换
     *
     * @param <T>
     */
    public interface ResultMap<T> {
        T convert(ResultSet resultSet) throws Exception;
    }
 
 
    /**
     * 创建连接并执行
     *
     * @param sql
     * @param resultMap
     * @param <T>
     * @return
     * @throws Exception
     */
    public static <T> List<T> queryList(String sql, ResultMap<T> resultMap) throws Exception {
        //jdbc数据库
        Connection conn = getConnH2();
        //3.通过数据库的连接操作数据库，实现增删改查（使用Statement类）
        Statement st = conn.createStatement();
        ResultSet rs = st.executeQuery(sql);
        List<T> list = new ArrayList<>();
        //4.处理数据库的返回结果(使用ResultSet类)
        while (rs.next()) {
            T convert = resultMap.convert(rs);
            list.add(convert);
        }
        //关闭资源
        rs.close();
        st.close();
        conn.close();
        return list;
    }
 
    /**
     * 查询数据集
     *
     * @param sql
     * @param returnType
     * @param <T>
     * @return
     * @throws Exception
     */
    public static <T> List<T> queryList(String sql, Class<T> returnType) throws Exception {
        List<T> list = SqlSession.queryList(sql, rs -> {
            T obj = returnType.newInstance();
            Field[] declaredFields = returnType.getDeclaredFields();
            for (Field declaredField : declaredFields) {
                Class<?> type = declaredField.getType();
                //类型为String时的处理方法
                if (type == String.class) {
                    String value = rs.getString(declaredField.getName());
                    String fieldName = declaredField.getName();
                    Method method = returnType.getDeclaredMethod(
                            "set".concat(fieldName.substring(0, 1).toUpperCase().concat(fieldName.substring(1))),
                            String.class);
                    method.invoke(obj, value);
                }
                if (type == Long.class) {
                    Long value = rs.getLong(declaredField.getName());
                    String fieldName = declaredField.getName();
                    Method method = returnType.getDeclaredMethod(
                            "set".concat(fieldName.substring(0, 1).toUpperCase().concat(fieldName.substring(1))),
                            Long.class);
                    method.invoke(obj, value);
                }
                //其他类型处理方法
            }
            return obj;
        });
        return list;
    }
}
```
schema.sql文件
```sql
drop table if exists user;
CREATE TABLE user
(
  id       int(11) NOT NULL AUTO_INCREMENT,
  username varchar(255) DEFAULT NULL,
  password varchar(255) DEFAULT NULL,
  PRIMARY KEY (id)
);
 
insert into user(id,username,password) values(1,'xiongxina','123456');
insert into user(id,username,password) values(2,'xiongxinb','123456');
insert into user(id,username,password) values(3,'xiongxinc','123456');
```
mapper定义
```java
package com.dbutil.mapper;
 
import com.dbutil.entity.UserEntity;
import com.dbutil.session.SqlSession;
 
import java.util.List;
 
public interface UserMapper {
 
    @SqlSession.QueryList("select * from user")
    List<UserEntity> queryUser();
}
```
使用：
```java
package com.dbutil;
 
import com.dbutil.entity.UserEntity;
import com.dbutil.mapper.UserMapper;
import com.dbutil.session.SqlSession;
 
import java.util.List;
 
public class UserService {
 
    public static void main(String[] args) throws Exception {
        UserMapper userMapper = SqlSession.getMapper(UserMapper.class);
        List<UserEntity> userEntities = userMapper.queryUser();
        for (UserEntity userEntity : userEntities) {
            System.out.println(userEntity);
        }
    }
}
```
