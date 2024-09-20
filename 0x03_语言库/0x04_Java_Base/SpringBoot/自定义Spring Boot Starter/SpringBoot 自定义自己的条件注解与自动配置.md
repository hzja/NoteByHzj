JavaSpringBoot<br />Spring Boot的核心功能就是为整合第三方框架提供自动配置，而本文则带着大家实现了自己的自动配置和Starter，一旦真正掌握了本文的内容，就会对Spring Boot产生“一览众山小”的感觉。
<a name="ttfoF"></a>
## 自定义条件注解
在SpringBoot中，所有自定义条件注解其实都是基于`@Conditional`而来的，使用`@Conditional`定义新条件注解关键就是要有一个`Condition`实现类，该`Condition`实现类就负责条件注解的处理逻辑，该实现类所实现的`matches()`方法决定了条件注解的要求是否得到满足。<br />下面是自定义条件注解的`Condition`实现类的代码。<br />src/main/java/com/example/_003configtest/condition/MyCondition.java
```java
package com.example._003configtest.condition;

import com.example._003configtest.annotation.ConditionalCustom;
import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.core.env.Environment;
import org.springframework.core.type.AnnotatedTypeMetadata;

import java.util.Map;


public class MyCondition implements Condition {

    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        //获取@ConditionalCustom注解的全部属性,其中ConditionalCustom是自定义的注解
        Map<String, Object> annotationAttributes = metadata.getAnnotationAttributes(ConditionalCustom.class.getName());
        //获取注解的value属性值
        String[] vals = (String[]) annotationAttributes.get("value");
        //env是application.properties或application.yml中配置的属性
        Environment env = context.getEnvironment();
        //遍历每个value的每个属性值
        for (String val : vals) {
            //如果某个属性值对应的配置属性不存在，则返回false
            if(env.getProperty(val.toString())== null){
                return false;
            }
        }
        return true;
    }
}
```
从上面的逻辑可以看到，自定义条件注解的处理逻辑比较简单：就是要求value属性所指定的所有配置属性必须存在，至于这些配置属性的值是什么无所谓，这些配置属性是否有值也无所谓。<br />有了上面的Condition实现类之后，接下来即可基于`@Conditional`来定义自定义条件注解。下面是自定义条件注解的代码。<br />src/main/java/com/example/_003configtest/annotation/ConditionalCustom.java
```java
package com.example._003configtest.annotation;
import com.example._003configtest.condition.MyCondition;
import org.springframework.context.annotation.Conditional;

import java.lang.annotation.*;

@Target({ElementType.TYPE,ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
//只要通过@Conditional指定Condition实现类即可，该Condition实现类就会负责该条件注解的判断逻辑
@Conditional(MyCondition.class)
public @interface ConditionalCustom {

    String[] value() default {};

}
```
下面的配置类示范了如何使用该自定义的条件注解：<br />src/main/java/com/example/_003configtest/config/MyConfigTest.java
```java
// proxyBeanMethods = true  :单例模式,保证每个@Bean方法被调用多少次返回的组件都是同一个
// proxyBeanMethods = false :原型模式，每个@Bean方法被调用多少次返回的组件都是新创建的
@Configuration(proxyBeanMethods = true)
public class MyConfigTest {
    @Bean
    //只有当applicaion.properties或application.yml中org.test1,org.test2两个配置属性都存在时才生效
    @ConditionalCustom({"org.test1","org.test2"})
    public MyBean myBean(){
        return new MyBean();
    }
}
```
在application.properties文件中添加如下配置：
```
org.test1 = 1
org.test2 = 2
```
运行测试发现成功获得了容器中对应的类：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676940471118-65426d6b-524b-4776-ab60-91d8852aac5e.png#averageHue=%23090604&clientId=u33929ace-fb6c-4&from=paste&id=u01dbe85c&originHeight=50&originWidth=425&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uca53abcc-2b84-49f2-8d1b-183e589407c&title=)
<a name="eFGKw"></a>
## **自定义自动配置**
开发自己的自动配置很简单，其实也就两步：

- 使用`@Configuration`和条件注解定义自动配置类。
- 在META-INF/spring.factories文件中注册自动配置类。

为了清楚地演示Spring Boot自动配置的效果，避免引入第三方框架导致的额外复杂度，本例先自行开发一个funny框架，该框架的功能是用文件或数据库保存程序的输出信息。<br />新建一个Maven项目funny(注意不是用SpringInitializr创建项目)，为该项目添加mysql-connector-java和slf4j-api两个依赖。由于该项目是自己开发的框架，因此无须为该项目添加任何Spring Boot依赖。<br />下面是该项目的pom.xml文件代码：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>org.example</groupId>
    <artifactId>funny</artifactId>
    <version>1.0-SNAPSHOT</version>

    <!-- 定义所使用的Java版本和源代码使用的字符集-->
    <properties>
        <maven.compiler.source>8</maven.compiler.source>
        <maven.compiler.target>8</maven.compiler.target>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>

    <dependencies>

        <!-- MySQL数据库驱动依赖 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>8.0.27</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.slf4j/slf4j-api -->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>1.7.36</version>
        </dependency>
    </dependencies>


</project>
```
接下来为这个框架项目开发如下类。<br />src/main/java/io/WriterTemplate.java
```java
package io;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.Charset;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Objects;
import javax.sql.DataSource;


public class WriterTemplate {
    Logger log = LoggerFactory.getLogger(this.getClass());
    private final DataSource dataSource;
    private Connection conn;
    private File dest;
    private final Charset charset;
    private RandomAccessFile raf;

    public WriterTemplate(DataSource dataSource) throws SQLException {
        this.dataSource = dataSource;
        this.dest = null;
        this.charset = null;
        if(Objects.nonNull(this.dataSource)){
            log.debug("========获取数据库连接========");
            this.conn = dataSource.getConnection();
        }
    }

    public WriterTemplate(File dest,Charset charset) throws FileNotFoundException{
        this.dest = dest;
        this.charset = charset;
        this.dataSource = null;
        this.raf = new RandomAccessFile(this.dest,"rw");
    }

    public void write(String message) throws IOException,SQLException{
        if(Objects.nonNull(this.conn)){
            //查询当前数据库的fnny_message表是否存在
            ResultSet rs = conn.getMetaData().getTables(conn.getCatalog(),null,"funny_message",null);
            //如果funy_message表不存在，需要创建表
            if(!rs.next()){
                log.debug("~~~~~~~~~创建funny_message表~~~~~~~~~");
                conn.createStatement().execute("create table funny_message " + "(id int primary key auto_increment,message_text text)");
            }
            log.debug("~~~~~~~~~输出到数据表~~~~~~~~~");
            //往数据库中插入数据
            conn.createStatement().executeUpdate("insert into " + "funny_message values(null,'" + message + "')");
            rs.close();
        }
        else{
            log.debug("~~~~~~~~~输出到文件~~~~~~~~~");
            raf.seek(this.dest.length());
            raf.write((message + "\n").getBytes(this.charset));
        }
    }

    //关闭资源
    public void close() throws SQLException,IOException{
        if(this.conn != null){
            this.conn.close();
        }

        if(this.raf != null){
            this.raf.close();
        }
    }
}
```
该工具类根据是否传入 DataSource 来决定输出目标：如果为该工具类传入了DataSource，它就会向该数据源所连接的数据库中的funny_message表输出内容（如果该表不存在，该工具类将会自动建表）；如果没有为该工具类传入DataSource，它就会向指定文件输出内容。<br />接下来使用install打包到maven仓库：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676940471175-0203821c-6361-4bf5-abd7-4e4e2a9ba5b7.png#averageHue=%23647058&clientId=u33929ace-fb6c-4&from=paste&id=u6a7dbb36&originHeight=469&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1a363599-aeac-464c-bba0-e865483fab8&title=)<br />有了该框架之后，接下来为该框架开发自动配置。如果为整合现有的第三方框架开发自动配置，则可直接从这一步开始（因为框架已经存在了，直接为框架开发自动配置即可）。<br />同样新建一个Maven项目funny-spring-boot-starter(为了方便可以用SpringInitializr创建项目)，这个项目是自定义Starter项目，因此必须要有Spring Boot支持，将前面Spring Boot项目中的pom.xml文件复制过来，保留其中的spring-boot-starter依赖，并添加刚刚开发的funny框架的依赖。<br />此外，由于该项目不是Spring Boot应用，因此不需要主类，也不需要运行，故删除其中的spring-boot-maven-plugin插件。修改后的pom.xml文件内容如下。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.example</groupId>
    <artifactId>funny-spring-boot-starter</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>funny-spring-boot-starter</name>
    <description>funny-spring-boot-starter</description>

    <properties>
        <java.version>1.8</java.version>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <spring-boot.version>2.3.7.RELEASE</spring-boot.version>
    </properties>

    <dependencies>
        <!-- Spring Boot Starter依赖-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>

        <!-- 依赖自定义的funny框架，如果正在为其他第三方框架开发自动配置，则此处应该填写被整合的第三方框架的坐标。-->
        <dependency>
            <groupId>org.example</groupId>
            <artifactId>funny</artifactId>
            <version>1.0-SNAPSHOT</version>
        </dependency>
    </dependencies>

    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-dependencies</artifactId>
                <version>${spring-boot.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>

</project>
```
接下来定义如下自动配置类。<br />src/main/java/com/example/funnyspringbootstarter/autoconfig/FunnyAutoConfiguration.java
```java
package com.example.funnyspringbootstarter.autoconfig;

import io.WriterTemplate;
import org.springframework.boot.autoconfigure.AutoConfigureAfter;
import org.springframework.boot.autoconfigure.AutoConfigureOrder;
import org.springframework.boot.autoconfigure.condition.ConditionalOnClass;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.boot.autoconfigure.condition.ConditionalOnSingleCandidate;
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.sql.DataSource;
import javax.xml.crypto.Data;
import java.io.File;
import java.io.FileNotFoundException;
import java.nio.charset.Charset;
import java.sql.SQLException;

@Configuration
//当WriteTemplate类存在时配置生效
@ConditionalOnClass(WriterTemplate.class)
//FunnyProperties是自定义的类，后面会定义，这里表示启动FunnyProperties
@EnableConfigurationProperties(FunnyProperties.class)
//让该自动配置类位于DataSourceAutoConfiguration自动配置类之后处理
@AutoConfigureAfter(DataSourceAutoConfiguration.class)
public class FunnyAutoConfiguration {
    private final FunnyProperties properties;
 //FunnyProperties类负责加载配置属性
    public FunnyAutoConfiguration(FunnyProperties properties) {
        this.properties = properties;
    }

    @Bean(destroyMethod = "close")
    //当单例的DataSource Bean存在时配置生效
    @ConditionalOnSingleCandidate(DataSource.class)
    //只有当容器中没有WriterTemplate Bean时，该配置才会生效
    @ConditionalOnMissingBean
    //通过@AutoConfigureOrder注解指定该配置方法比下一个配置WriterTemplate的方法的优先级更高
    @AutoConfigureOrder(99)
    public WriterTemplate writerTemplate(DataSource dataSource) throws SQLException{
        return new WriterTemplate(dataSource);
    }


    @Bean(destroyMethod = "close")
    //只有当前面的WriteTemplate配置没有生效时，该方法的配置才会生效
    @ConditionalOnMissingBean
    @AutoConfigureOrder(199)
    public WriterTemplate writerTemplate2() throws FileNotFoundException{
        File f = new File(this.properties.getDest());
        Charset charset = Charset.forName(this.properties.getCharset());
        return new WriterTemplate(f,charset);
    }
}
```
在`FunnyAutoConfiguration` 自动配置类中定义了两个`@Bean`方法，这两个`@Bean` 方法都用于自动配置 WriterTemplate。为了指定它们的优先级，程序使用了`@AutoConfigureOrder` 注解修饰它们，该注解指定的数值越小，优先级越高。<br />`FunnyAutoConfiguration` 自动配置类中的@Bean 方法同样使用了`@ConditionalOnMissingBean`、`@ConditionalOnSingleCandidate`等条件注解修饰，从而保证只有当容器中不存在`WriterTemplate`时，该自动配置类才会配置WriterTemplate Bean，且优先配置基于DataSource的`WriterTemplate`。<br />上面的自动配置类还用到了FunnyProperties属性处理类，该类的代码如下：
```java
package com.example.funnyspringbootstarter.autoconfig;

import org.springframework.boot.context.properties.ConfigurationProperties;

@ConfigurationProperties(prefix = FunnyProperties.FUNNY_PREFIX)
public class FunnyProperties {
    public static final String FUNNY_PREFIX = "org.test";
    private String dest;
    private String charset;

    public String getDest() {
        return dest;
    }

    public void setDest(String dest) {
        this.dest = dest;
    }

    public String getCharset() {
        return charset;
    }

    public void setCharset(String charset) {
        this.charset = charset;
    }
}
```
上面的属性处理类负责处理以“org.test”开头的属性，这个“org.test”是必要的，它相当于这一组配置属性的“命名空间”，通过这个命名空间可以将这些配置属性与其他框架的配置属性区分开。<br />有了上面的自动配置类之后，接下来使用如下META-INF/spring.factories文件来注册自动配置类。<br />src/main/resources/META-INF/spring.factories
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration = \
  com.example.funnyspringbootstarter.autoconfig.FunnyAutoConfiguration
```
经过上面步骤，自动配置开发完成,接下来使用install打包到maven仓库：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676940471091-832c46cf-8977-4d4c-be70-b2fadc8e05a2.png#averageHue=%23496544&clientId=u33929ace-fb6c-4&from=paste&id=uf9bea2c4&originHeight=436&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue1c049b6-73c0-4a65-a4ce-4f2ecdb5c58&title=)<br />有了自定义的Starter之后，接下来使用该Starter与使用Spring Boot官方Starter并没有任何区别。<br />首先新建一个Maven项目myfunnytest，在pom文件中引入该starter：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.example</groupId>
    <artifactId>myfunnytest</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>myfunnytest</name>
    <description>myfunnytest</description>

    <properties>
        <java.version>1.8</java.version>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <spring-boot.version>2.3.7.RELEASE</spring-boot.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>com.example</groupId>
            <artifactId>funny-spring-boot-starter</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency>

<!--        <dependency>-->
<!--            <groupId>org.springframework.boot</groupId>-->
<!--            <artifactId>spring-boot-starter</artifactId>-->
<!--        </dependency>-->

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
            <exclusions>
                <exclusion>
                    <groupId>org.junit.vintage</groupId>
                    <artifactId>junit-vintage-engine</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
    </dependencies>

    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-dependencies</artifactId>
                <version>${spring-boot.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>

    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.8.1</version>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                    <encoding>UTF-8</encoding>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <version>2.3.7.RELEASE</version>
                <configuration>
                    <mainClass>com.example.myfunnytest.MyfunnytestApplication</mainClass>
                </configuration>
                <executions>
                    <execution>
                        <id>repackage</id>
                        <goals>
                            <goal>repackage</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>

</project>
```
由于 funny-spring-boot-starter 本身需要依赖 spring-boot-starter，因此不再需要显式配置依赖spring-boot-starter。<br />在添加了上面的funny-spring-boot-starter依赖之后，该Starter包含的自动配置生效，它会尝试在容器中自动配置WriterTemplate，并且还会读取application.properties因此还需要在application.properties文件中进行配置。<br />src/main/resources/application.properties
```
# 应用名称
spring.application.name=myfunnytest
org.test.dest = F:/abc-12345.txt
org.test.charset=UTF-8
spring.datasource.url=jdbc:mysql://localhost:3306/springboot?useUnicode=true&characterEncoding=UTF-8&useJDBCCompliantTimezoneShift=true&useLegacyDateTimeCode=false&serverTimezone=GMT%2B8
spring.datasource.username=root
spring.datasource.password=root
```
该示例的主类很简单，它直接获取容器中的WriterTemplate Bean，并调用该Bean的write()方法执行输出。下面是该主类的代码:
```java
package com.example.myfunnytest;

import io.WriterTemplate;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ConfigurableApplicationContext;

@SpringBootApplication
public class MyfunnytestApplication {

    public static void main(String[] args) throws Exception{
        ConfigurableApplicationContext run = SpringApplication.run(MyfunnytestApplication.class, args);
        WriterTemplate writerTemplate = run.getBean(WriterTemplate.class);
        System.out.println(writerTemplate);
        writerTemplate.write("自动配置");
    }

}
```
运行该程序，由于当前Spring容器中没有DataSource Bean，因此FunnyAutoConfiguration将会自动配置输出到文件的WriterTemplate。因此，运行该程序，可以看到程序向“f:/abc-12345.txt”文件（由前面的org.test.dest属性配置）输出内容:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676940471098-16e50213-dd0e-4cbb-ba2c-6083e87a1d48.png#averageHue=%238a8682&clientId=u33929ace-fb6c-4&from=paste&id=ufa8557ca&originHeight=64&originWidth=169&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8cfa5945-00d1-4a82-949f-e11e72287f9&title=)<br />运行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676940471129-d4eecf74-9833-4457-9aea-eaff11011404.png#averageHue=%23050302&clientId=u33929ace-fb6c-4&from=paste&id=ud59b4346&originHeight=177&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5bfe654e-e6ee-4ad9-bcca-2e59914d48f&title=)<br />如果在项目的pom.xml文件中通过如下配置来添加依赖。
```xml
<!-- Spring Boot JDBC Starter依赖-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jdbc</artifactId>
</dependency>
```
此时为项目添加了spring-boot-starter-jdbc依赖，该依赖将会在容器中自动配置一个DataSource Bean，这个自动配置的DataSource Bean将导致FunnyAutoConfiguration会自动配置输出到数据库的WriterTemplate。因此，运行该程序，可以看到程序向数据库名为springboot数据库的funny_message表输出内容:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676940471713-15f03343-53e7-4e1a-8c95-69fe93f300e4.png#averageHue=%2341733b&clientId=u33929ace-fb6c-4&from=paste&id=u19cfd8ad&originHeight=329&originWidth=659&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5cbdc8ab-ed49-419a-b3a8-3e1cd4dd38c&title=)<br />Spring Boot的核心功能就是为整合第三方框架提供自动配置，而本文则带着大家实现了自己的自动配置和Starter，一旦真正掌握了本文的内容，就会对Spring Boot产生“一览众山小”的感觉。
