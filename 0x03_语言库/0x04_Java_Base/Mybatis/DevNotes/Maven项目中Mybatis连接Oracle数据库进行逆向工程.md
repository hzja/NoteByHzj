Mybatis Maven Oracle
<a name="DF6SR"></a>
# 1、在项目中添加Oracle驱动包
<a name="jMfWZ"></a>
## A.jar包
[ojdbc-14.zip](https://www.yuque.com/attachments/yuque/0/2019/zip/396745/1566991323144-75ac3400-1c29-4074-a345-745447823812.zip?_lake_card=%7B%22uid%22%3A%22rc-upload-1566987903743-7%22%2C%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fzip%2F396745%2F1566991323144-75ac3400-1c29-4074-a345-745447823812.zip%22%2C%22name%22%3A%22ojdbc-14.zip%22%2C%22size%22%3A3297835%2C%22type%22%3A%22application%2Fzip%22%2C%22ext%22%3A%22zip%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22status%22%3A%22done%22%2C%22percent%22%3A0%2C%22id%22%3A%22zC2EM%22%2C%22card%22%3A%22file%22%7D)

<a name="PdZ2S"></a>
## B.Maven引入依赖

```xml
<dependency>
    <groupId>ojdbc</groupId>
    <artifactId>ojdbc</artifactId>
  <version>14</version>
</dependency>
```
如果Maven仓库没有该依赖包可以下载，可以将上面的jar包下载到本地，按照Maven的存储习惯将jar包放置对应的目录下面，并按照版本命名<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566991523926-10ab5b54-9f6d-43ad-94e6-1e839efd0f7b.png#align=left&display=inline&height=122&originHeight=336&originWidth=1703&size=70560&status=done&style=none&width=619.2727272727273)

<a name="udqMd"></a>
# 2、Maven中Mybatis-Generator逆向工程插件配置驱动依赖

```xml
<plugins>
    <!--Mybatis逆向工程的插件-->
    <plugin>
      <groupId>org.mybatis.generator</groupId>
      <artifactId>mybatis-generator-maven-plugin</artifactId>
      <version>1.3.7</version>
      <configuration>
        <!--指定本地的逆向工程设置文件-->
        <configurationFile>src/main/resources/generator.xml</configurationFile>
        <verbose>true</verbose>
        <overwrite>true</overwrite>
      </configuration>
      <!--在插件配置数据库驱动包以防止插件运行找不到驱动类-->
      <dependencies>
        <dependency>
          <groupId>ojdbc</groupId>
          <artifactId>ojdbc</artifactId>
          <version>14</version>
        </dependency>
      </dependencies>
  </plugin>
</plugins>
```

<a name="r3SuD"></a>
# 3、在逆向工程配置文件配置连接信息

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
        PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
        "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">

<generatorConfiguration>
    <context id="testTables" targetRuntime="MyBatis3">
        <commentGenerator>
            <!-- 是否去除自动生成的注释 true：是 ： false:否 -->
            <property name="suppressAllComments" value="false"/>
        </commentGenerator>
        <!--数据库连接的信息：驱动类、连接地址、用户名、密码 -->
        <!--com.mysql.cj.jdbc.Driver是最新版本MySQL驱动连接方式-->
        <!--?serverTimezone=UTC设置时区-->
        <jdbcConnection
                driverClass="oracle.jdbc.driver.OracleDriver"
                connectionURL="jdbc:oracle:thin:@127.0.0.1:1521:helowin"
                userId="monitor"
                password="monitor">
            <!--设置可以获取tables remarks信息-->
            <property name="useInformationSchema" value="true"/>
            <!--设置可以获取remarks信息-->
            <property name="remarks" value="true"/>
        </jdbcConnection>

        <!-- 默认false，把JDBC DECIMAL 和 NUMERIC 类型解析为 Integer，为 true时把JDBC DECIMAL 和
            NUMERIC 类型解析为java.math.BigDecimal -->
        <javaTypeResolver>
            <property name="forceBigDecimals" value="false"/>
        </javaTypeResolver>

        <!-- targetProject:生成POJO/Bean类的位置 -->
        <javaModelGenerator targetPackage="com.fcant.gh.bean"
                            targetProject=".\src\main\java">
            <!-- enableSubPackages:是否让schema作为包的后缀 -->
            <property name="enableSubPackages" value="false"/>
            <!-- 从数据库返回的值被清理前后的空格 -->
            <property name="trimStrings" value="true"/>
        </javaModelGenerator>

        <!-- targetProject:mapper映射文件生成的位置 -->
        <sqlMapGenerator targetPackage="mapper"
                         targetProject=".\src\main\resources">
            <!-- enableSubPackages:是否让schema作为包的后缀 -->
            <property name="enableSubPackages" value="false"/>
        </sqlMapGenerator>

        <!-- targetPackage：mapper接口生成的位置 -->
        <javaClientGenerator type="XMLMAPPER"
                             targetPackage="com.fcant.gh.mapper"
                             targetProject=".\src\main\java">
            <!-- enableSubPackages:是否让schema作为包的后缀 -->
            <property name="enableSubPackages" value="false"/>
        </javaClientGenerator>
        <!--指定数据库表的生成策略-->
        <table tableName="user" domainObjectName="User"
               enableCountByExample="false" enableUpdateByExample="false"
               enableDeleteByExample="false" enableSelectByExample="false"
               selectByExampleQueryId="false">
        </table>

    </context>
</generatorConfiguration>
```
<a name="mJ7uY"></a>
## A.更改的配置如下
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1567004681499-a51f1d16-d561-4d4b-8acf-abb2bacf51dd.png#align=left&display=inline&height=211&originHeight=580&originWidth=2104&size=241714&status=done&style=none&width=765.0909090909091)![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566992708301-0ecfb507-2714-4d65-b4ac-232a492805b8.png#align=left&display=inline&height=106&originHeight=292&originWidth=2174&size=113561&status=done&style=none&width=790.5454545454545)

<a name="SEJxx"></a>
# 4、运行插件完成逆向构建
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566992940938-257d151d-3f4c-45a0-9313-e60663e19770.png#align=left&display=inline&height=749&originHeight=2059&originWidth=3840&size=1270940&status=done&style=none&width=1396.3636363636363)

<a name="94KLs"></a>
# 5、建测试类进行测试
<a name="4aKjO"></a>
## I.MapperTest.java
```java
package com.fcant.gh.test;

import com.fcant.gh.bean.User;
import com.fcant.gh.mapper.UserMapper;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

/**
 * Test
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 下午 19:52 2019-08-28/0028
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:spring-mybatis.xml")
public class MapperTest {

    @Autowired
    UserMapper userMapper;

    @Test
    public void insertTest() {
        User user = new User();
        long i = 10;
        user.setId(i);
        user.setUserName("樊乘");
        user.setUserPhone("17826634");
        int insert = userMapper.insert(user);
        System.out.println(insert);
        User selectByPrimaryKey = userMapper.selectByPrimaryKey(i);
        System.out.println(selectByPrimaryKey);
    }
}

```
<a name="3WQXd"></a>
## I.异常
<a name="NWeHF"></a>
### A.ORA-00923: 未找到要求的 FROM 关键字
<a name="bUm37"></a>
#### 1.出现异常报错
```
2019-08-28 23:20:39invalid oracle validationQuery. SELECT 1, may should be : SELECT 1 FROM DUAL
2019-08-28 23:20:39init datasource error, url: jdbc:oracle:thin:@120.79.178.68:1521:helowin
java.sql.SQLException: ORA-00923: 未找到要求的 FROM 关键字
```

<a name="Nex1j"></a>
#### 2.解决办法如下
修改spring-mybatis.xml中的数据库连接池配置<br />MySQL和SQLServer常用的验证查询为"SELECT 1"，但是在Oracle中验证查询为"select 1 from dual"
```xml
<property name="validationQuery" value="select 1 from dual" />
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1567006978406-6b8b3038-f4ce-4100-be26-4e148f72767f.png#align=left&display=inline&height=503&originHeight=1384&originWidth=2703&size=747479&status=done&style=none&width=982.9090909090909)

<a name="mypAl"></a>
### B.Oracle操作表名为User的表时报ORA-00903:表名无效
<a name="7A0Uk"></a>
#### 1.问题分析
因为Oracle数据库操作USER数据库，与同用户数据库冲突，所以需要把要操作的数据库使用双引号引用起来，使用单引号不生效
<a name="Y2Ao1"></a>
#### 2.解决办法
使用双引号将冲突的数据库引用起来
<a name="ChRKZ"></a>
#### 3.使用单引号无效
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1567007528218-38b20c24-b3da-42e8-8169-b10daa6c4c8b.png#align=left&display=inline&height=749&originHeight=2059&originWidth=3840&size=1125708&status=done&style=none&width=1396.3636363636363)
<a name="VM0ac"></a>
#### 4.使用双引号成功
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1567007384319-022deabe-0a42-4757-b173-0c29a3002b30.png#align=left&display=inline&height=749&originHeight=2059&originWidth=3840&size=1134652&status=done&style=none&width=1396.3636363636363)
