Mybatis Maven MySQL
<a name="JTJzY"></a>
## 1、使用Maven插件进行逆向工程
<a name="cpK42"></a>
### A.配置逆向工程文件generator.xml
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
                driverClass="com.mysql.cj.jdbc.Driver"
                connectionURL="jdbc:mysql:///ssm?serverTimezone=UTC"
                userId="root"
                password="123456">
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
        <javaModelGenerator targetPackage="com.fcant.ssmcom.bean"
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
                             targetPackage="com.fcant.ssmcom.mapper"
                             targetProject=".\src\main\java">
            <!-- enableSubPackages:是否让schema作为包的后缀 -->
            <property name="enableSubPackages" value="false"/>
        </javaClientGenerator>
        <!--指定数据库表的生成策略-->
        <table tableName="ss_product" domainObjectName="Product"
               enableCountByExample="false" enableUpdateByExample="false"
               enableDeleteByExample="false" enableSelectByExample="false"
               selectByExampleQueryId="false">
        </table>

    </context>
</generatorConfiguration>
```
<a name="EHsF8"></a>
### B.需要自行更改以下配置项
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792423620-65514be5-39b9-4ec0-9979-46342d476ff9.png#height=1872&id=GfPGX&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1212260&status=done&style=shadow&title=&width=3490.9090152456756)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792555854-e933c6eb-cfd2-45d6-a473-a72c64f1e001.png#height=1872&id=feGeA&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1259085&status=done&style=shadow&title=&width=3490.9090152456756)
<a name="QZgKo"></a>
### C.在pom文件添加Mybatis逆向工程插件和相应的依赖包-Mybatis-Generator
```xml
 <dependencies>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.4.6</version>
        </dependency>

        <dependency>
            <groupId>org.mybatis.generator</groupId>
            <artifactId>mybatis-generator-core</artifactId>
            <version>1.3.7</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/com.baomidou/mybatis-plus -->
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus</artifactId>
            <version>3.1.2</version>
        </dependency>

        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>2.0.2</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/tk.mybatis/mapper -->
        <dependency>
            <groupId>tk.mybatis</groupId>
            <artifactId>mapper</artifactId>
            <version>4.1.5</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/tk.mybatis/mapper -->
        <dependency>
            <groupId>tk.mybatis</groupId>
            <artifactId>mapper</artifactId>
            <version>4.1.5</version>
        </dependency>
    </dependencies>

    <build>
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
                <!--在插件配置数据库驱动包以防止插件运行找不到驱动包-->
                <dependencies>
                    <dependency>
                        <groupId>mysql</groupId>
                        <artifactId>mysql-connector-java</artifactId>
                        <version>8.0.13</version>
                    </dependency>
                </dependencies>
            </plugin>
        </plugins>
    </build>
```
<a name="vg9KH"></a>
### D.需要自行更改的地方
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792670970-b2de72fc-b2c5-44c6-8a4b-e59bd793b995.png#height=1212&id=Xgl8Z&originHeight=1333&originWidth=2863&originalType=binary&ratio=1&rotation=0&showTitle=false&size=458434&status=done&style=shadow&title=&width=2602.7272163146795)
<a name="lZlFI"></a>
### E.执行逆向工程
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792762028-07df1357-226d-4a46-b4cd-9837351048f6.png#height=1872&id=Nierb&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1162810&status=done&style=shadow&title=&width=3490.9090152456756)
<a name="FZqlf"></a>
## 2、使用配置类进行逆向工程
<a name="vBPi3"></a>
### A.导入相关jar包
[lib.zip](https://www.yuque.com/attachments/yuque/0/2019/zip/396745/1564794234356-7a6aa68f-e578-4c61-b844-86a105bfadbe.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fzip%2F396745%2F1564794234356-7a6aa68f-e578-4c61-b844-86a105bfadbe.zip%22%2C%22name%22%3A%22lib.zip%22%2C%22size%22%3A9533637%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1564792146446-17%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22zIRbT%22%2C%22card%22%3A%22file%22%7D)
<a name="Xo67O"></a>
### B.配置逆向工程文件mdb.xml-将该文件放至src目录下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
        PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
        "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">

<generatorConfiguration>
    <context id="testTables" targetRuntime="MyBatis3">
        <commentGenerator>
            <!-- 是否去除自动生成的注释 true：是 ： false:否 -->
          	<!-- <property name="suppressDate" value="true"/> -->
            <property name="suppressAllComments" value="true"/>
        </commentGenerator>
        <!--数据库连接的信息：驱动类、连接地址、用户名、密码 -->
        <!--com.mysql.cj.jdbc.Driver是最新版本MySQL驱动连接方式-->
        <!--?serverTimezone=UTC设置时区-->
        <jdbcConnection
                driverClass="com.mysql.cj.jdbc.Driver"
                connectionURL="jdbc:mysql:///ssm?serverTimezone=UTC"
                userId="root"
                password="123456">
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
        <javaModelGenerator targetPackage="com.fcant.ssmcom.bean"
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
                             targetPackage="com.fcant.ssmcom.mapper"
                             targetProject=".\src\main\java">
            <!-- enableSubPackages:是否让schema作为包的后缀 -->
            <property name="enableSubPackages" value="false"/>
        </javaClientGenerator>
        <!--指定数据库表的生成策略-->
        <!--schema属性标签可以解决JDBC连接MySQL找不到主键的问题-->
        <!--catalog属性标签解决JDBC对catalog的支持性而不能识别数据库的主键，但是会在映射文件表前自动生成"数据库名称.."-->
        <!--        <table tableName="busdata" domainObjectName="BusData" schema="itbus"></table>-->
        <table tableName="ss_product" domainObjectName="Product"
               enableCountByExample="false" enableUpdateByExample="false"
               enableDeleteByExample="false" enableSelectByExample="false"
               selectByExampleQueryId="false">
        </table>

    </context>
</generatorConfiguration>
```
<a name="PvBTa"></a>
### C.需要自行更改以下配置项
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792423620-65514be5-39b9-4ec0-9979-46342d476ff9.png#height=1872&id=MMr0s&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1212260&status=done&style=shadow&title=&width=3490.9090152456756)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792555854-e933c6eb-cfd2-45d6-a473-a72c64f1e001.png#height=1872&id=HirUZ&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1259085&status=done&style=shadow&title=&width=3490.9090152456756)
<a name="S9OFA"></a>
### D.配置逆向工程工具类
```java
/**
*Generator
*
*@description:Mybatis逆向工程工具类，在测试包运行降低耦合
*@author:Fcscanf@樊乘乘
*@date:2018-08-05下午16:51
*/
public class Generator {
    public static void main (String[] args) throws IOException,XMLParserException,SQLException,InterruptedException,InvalidConfigurationException{
        List<String> warings = new ArrayList<String>();
        boolean overwrite = true;
        File configFile = new File ("mdb.xml");
        ConfigurationParser configurationParser =new ConfigurationParser(warings);
        Configuration configuration = configurationParser.parseConfiguration(configFile);
        DefaultShellCallback callback = new DefaultShellCallback(overwrite);
        MyBatisGenerator myBatisGenerator =new MyBatisGenerator(configuration,callback,warings);
        myBatisGenerator.generate(null);
    }	
}

```
<a name="eOaBK"></a>
### E.运行工具类执行逆向工程
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564793880175-889bd2d3-6fc4-4121-8632-25ebb50da640.png#height=1277&id=f15x5&originHeight=1405&originWidth=2635&originalType=binary&ratio=1&rotation=0&showTitle=false&size=900699&status=done&style=shadow&title=&width=2395.4544935344675)
