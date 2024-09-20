JavaSpringBoot
<a name="B2EKp"></a>
### 简介
Springboot 中使用 JdbcTemplate 实现多数据源比较简单。查看 JdbcTemplate 源码；可以发现 JdbcTemplate 提供了传入 DataSource 的方式构建不同的 JdbcTemplate 实例。通过该方式就可以实现多数据源。
```java
public JdbcTemplate() {
}

public JdbcTemplate(DataSource dataSource) {
    this.setDataSource(dataSource);
    this.afterPropertiesSet();
}

public JdbcTemplate(DataSource dataSource, boolean lazyInit) {
    this.setDataSource(dataSource);
    this.setLazyInit(lazyInit);
    this.afterPropertiesSet();
}
```
<a name="XAWxk"></a>
### pom.xml文件引入如下依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0">http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
 <modelVersion>4.0.0</modelVersion>

 <groupId>com.olive</groupId>
 <artifactId>jdbc-multip-datasource</artifactId>
 <version>0.0.1-SNAPSHOT</version>
 <packaging>jar</packaging>

 <name>jdbc-multip-datasource</name>
 <url>http://maven.apache.org</url>

 <parent>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-parent</artifactId>
  <version>2.5.14</version>
  <relativePath /> <!-- lookup parent from repository -->
 </parent>

 <properties>
  <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  <maven.compiler.source>8</maven.compiler.source>
  <maven.compiler.target>8</maven.compiler.target>
 </properties>

 <dependencies>
  <dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-test</artifactId>
   <scope>test</scope>
  </dependency>
  <dependency>
   <groupId>mysql</groupId>
   <artifactId>mysql-connector-java</artifactId>
  </dependency>

  <dependency>
   <groupId>org.projectlombok</groupId>
   <artifactId>lombok</artifactId>
  </dependency>
  
  <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
  </dependency>
  <dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-jdbc</artifactId>
  </dependency>
 </dependencies>
</project>
```
<a name="ZQ0eS"></a>
### 配置两个数据源
分别为第一个主数据源(primary)，第二数据源(second)，具体配置如下：
```yaml
# 基本配置
server:
  port: 8080

# 数据库
spring:
  datasource:
    primary:
      driver-class-name: com.mysql.jdbc.Driver
      jdbc-url: jdbc:mysql://127.0.0.1:3306/db01?characterEncoding=utf-8&allowMultiQueries=true&autoReconnect=true
      username: root
      password: root
    second:
      driver-class-name: com.mysql.cj.jdbc.Driver
      jdbc-url: jdbc:mysql://127.0.0.1:3306/crm72?characterEncoding=utf-8&allowMultiQueries=true&autoReconnect=true
      username: root
      password: root
```
<a name="TBcfx"></a>
### 配置数据源
DataSourceConfig配置
```java
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.jdbc.DataSourceBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import javax.sql.DataSource;

/**
 * @Description: 数据源配置
 */
@Configuration
public class DataSourceConfig {

    @Bean(name = "primaryDataSource")
    @Qualifier("primaryDataSource")
    @ConfigurationProperties(prefix = "spring.datasource.primary")
    @Primary
    public DataSource primaryDataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean(name = "secondDataSource")
    @Qualifier("secondDataSource")
    @ConfigurationProperties(prefix = "spring.datasource.second")
    public DataSource secondDataSource() {
        return DataSourceBuilder.create().build();
    }
}
```
构建 JdbcTemplateConfig 类
```java
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.core.JdbcTemplate;
import javax.sql.DataSource;

/**
 * @Description: 数据源配置
 * @since
 */
@Configuration
public class JdbcTemplateConfig {

    @Bean(name = "primaryJdbcTemplate")
    public JdbcTemplate primaryJdbcTemplate(
            @Qualifier("primaryDataSource") DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    @Bean(name = "secondJdbcTemplate")
    public JdbcTemplate secondaryJdbcTemplate(
            @Qualifier("secondDataSource") DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }
}
```
<a name="dTHA8"></a>
### 创建学生与老师实体类
StudentDO 实体类
```java
package com.olive.entity;

import java.io.Serializable;
import lombok.Data;

@Data
public class StudentDO implements Serializable{

    private Long id;

    private String name;

    private int sex;
    
    private String grade;
}
```
TeacherDO 实体类
```java
package com.olive.entity;

import java.io.Serializable;
import lombok.Data;

@Data
public class TeacherDO implements Serializable {

    private Long id;

    private String name;

    private int sex;

    private String office;
}
```
<a name="TnvBB"></a>
### 数据库持久类
<a name="L31ji"></a>
#### StudentRepository 类
```java
import com.olive.entity.TeacherDO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import com.olive.entity.StudentDO;

@Repository
public class StudentRepository {

    @Autowired
    @Qualifier("primaryJdbcTemplate")
    private JdbcTemplate jdbcTemplate;

    public boolean save(StudentDO studentDO) {
        int result =jdbcTemplate.update("INSERT INTO t_student (`user_name`, `sex`, `grade`) VALUES ( ?, ?, ?);",
                studentDO.getName(),
                studentDO.getSex(),
                studentDO.getGrade());
        return result>0;
    }

    public TeacherDO getById(Long id) {
        return jdbcTemplate.queryForObject("select * from t_student where id = ?",
                new BeanPropertyRowMapper<TeacherDO>(TeacherDO.class), new Object[]{id});
    }
}
```
<a name="ZWPFP"></a>
#### TeacherRepository 类
```java
package com.olive.dao;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import com.olive.entity.TeacherDO;

@Repository
public class TeacherRepository {

    @Autowired
    @Qualifier("secondJdbcTemplate")
    private JdbcTemplate jdbcTemplate;

    public boolean save(TeacherDO teacherDO) {
        int result = jdbcTemplate.update("INSERT INTO t_teacher (`user_name`, `sex`, `office`) VALUES ( ?, ?, ?);",
                                         teacherDO.getName(),
                                         teacherDO.getSex(),
                                         teacherDO.getOffice());
        return result>0;
    }

    public TeacherDO getById(Long id) {
        return jdbcTemplate.queryForObject("select * from t_teacher where id = ?",
                                           new BeanPropertyRowMapper<TeacherDO>(TeacherDO.class), new Object[]{id});
    }
}
```
<a name="VU3sD"></a>
### 创建SpringBoot引导类
```java
package com.olive;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class);
    }

}
```
<a name="TiaV6"></a>
### 测试
```java
package com.olive;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import com.olive.entity.StudentDO;
import com.olive.entity.TeacherDO;
import com.olive.dao.StudentRepository;
import com.olive.dao.TeacherRepository;

@SpringBootTest
public class JdbcTest {

    @Autowired
    StudentRepository studentRepository;

    @Autowired
    TeacherRepository teacherRepository;

    @Test
    public void userSave() {
        StudentDO studentDO = new StudentDO();
        studentDO.setName("BUG");
        studentDO.setSex(1);
        studentDO.setGrade("一年级");
        studentRepository.save(studentDO);

        TeacherDO teacherDO = new TeacherDO();
        teacherDO.setName("Java乐园");
        teacherDO.setSex(2);
        teacherDO.setOffice("语文");
        teacherRepository.save(teacherDO);
    }
}
```
