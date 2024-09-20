JavaSpringBoot<br />在SpringBoot中结合loom虚拟线程。还将在JMeter的帮助下做一些负载测试，看看虚拟线程和普通线程的响应时间如何。<br />首先，虚拟线程是 Project Loom 的一部分。<br />此外，Loom 不会加速内存计算，例如并行流，这不是 Loom 的目标。<br />快速设置 Spring Boot 项目。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>SpringBoot-starter-parent</artifactId>
    <version>3.1.0</version>
    <relativePath/> <!-- lookup parent from repository -->
  </parent>
  <groupId>org.anil</groupId>
  <artifactId>virtualthread</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <name>virtualthread</name>
  <description>virtualthread</description>
  <properties>
    <java.version>20</java.version>
    <tomcat.version>11.0.0-M4</tomcat.version>
  </properties>
  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>SpringBoot-starter-data-jpa</artifactId>
    </dependency>
    <!-- https://mvnrepository.com/artifact/org.apache.commons/commons-lang3 -->
    <dependency>
      <groupId>org.apache.commons</groupId>
      <artifactId>commons-lang3</artifactId>
      <version>3.12.0</version>
    </dependency>

    <!-- https://mvnrepository.com/artifact/mysql/mysql-connector-java -->

    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
      <scope>runtime</scope>
      <version>8.0.33</version>
    </dependency>


    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>SpringBoot-starter-web</artifactId>
    </dependency>

    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>SpringBoot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>
    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
      <version>1.18.24</version>
      <scope>compile</scope>
    </dependency>
  </dependencies>

  <build>
    <plugins>
      <plugin>
        <groupId>org.springframework.boot</groupId>
        <artifactId>SpringBoot-maven-plugin</artifactId>
      </plugin>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <configuration>
          <compilerArgs>
            <arg>--enable-preview</arg>
          </compilerArgs>
          <source>20</source>
          <target>20</target>
        </configuration>

      </plugin>
    </plugins>
  </build>

</project>
```
由于 Project Loom 处于预览阶段，需要启用预览功能。
```java
package org.anil.virtualthread;

import lombok.extern.slf4j.Slf4j;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.embedded.tomcat.TomcatProtocolHandlerCustomizer;
import org.springframework.context.annotation.Bean;

import java.util.concurrent.Executors;

@SpringBootApplication
@Slf4j
public class VirtualthreadApplication {

    public static void main(String[] args) {
        SpringApplication.run(VirtualthreadApplication.class, args);
    }

    @Bean
    public TomcatProtocolHandlerCustomizer<?> protocolHandlerVirtualThreadExecutorCustomizer() {
        return protocolHandler -> {
            log.info("Configuring " + protocolHandler + " to use VirtualThreadPerTaskExecutor");
            protocolHandler.setExecutor(Executors.newVirtualThreadPerTaskExecutor());
        };
    }

}
```
到目前为止，需要为 Tomcat 服务器配置虚拟线程设置。将来，这可能会在自动配置本身中得到解决。
```java
package org.anil.virtualthread;

import org.apache.commons.lang3.RandomStringUtils;
import org.apache.commons.lang3.RandomUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class HomeController {

    @Autowired
    ProductRepository productRepository;


    @GetMapping("/thread")
    public List<Product> checkThread() throws InterruptedException {
        Thread.sleep(1000);
        return productRepository.findAll();
    }


    @PostMapping("/save")
    public String saveProduct() throws InterruptedException {
        for(int i=0; i< 1000; i++){
            Product product = new Product();
            product.setProductName(RandomStringUtils.randomAlphanumeric(5));
            product.setPrice(RandomUtils.nextLong(10,1000));
            product.setPrice(1L);
            productRepository.save(product);
        }
        return "anil";
    }
}
```
有一个GetMapping返回所有结果，数据库中有 1000 条数据。已经让线程休眠 1 秒。看看Product实体和ProductRepository。
```java
package org.anil.virtualthread;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Getter;
import lombok.Setter;

@Entity
@Getter
@Setter
public class Product {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String productName;
    private Long price;
}
```
```java
package org.anil.virtualthread;

import org.springframework.data.jpa.repository.JpaRepository;

public interface ProductRepository extends JpaRepository<Product,Long> {
}
```
application.yaml
```yaml
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    maxIdle: 1
    timeBetweenEvictionRunsMillis: 60000
    hikari:
      connection-timeout: 60000
      maximum-pool-size: 10
      minimum-idle: 5
    url: jdbc:mysql://localhost:3306/todos
    testWhileIdle: true
    username: root
    password: root1234
    validationQuery: SELECT 1
  flyway:
    baseline-version: 0
    enabled: true
    validate-on-migrate: false
  jpa:
    database: mysql
    generate-ddl: true
    hibernate:
      ddl-auto: none
      format_sql: true
    show-sql: true
```
现在，首先通过注释以下行来运行应用程序，**这将在普通线程上运行应用程序**。
```java
package org.anil.virtualthread;

import lombok.extern.slf4j.Slf4j;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.embedded.tomcat.TomcatProtocolHandlerCustomizer;
import org.springframework.context.annotation.Bean;

import java.util.concurrent.Executors;

@SpringBootApplication
@Slf4j
public class VirtualthreadApplication {

    public static void main(String[] args) {
        SpringApplication.run(VirtualthreadApplication.class, args);
    }

    //    @Bean
    //    public TomcatProtocolHandlerCustomizer<?> protocolHandlerVirtualThreadExecutorCustomizer() {
    //        return protocolHandler -> {
    //            log.info("Configuring " + protocolHandler + " to use VirtualThreadPerTaskExecutor");
    //            protocolHandler.setExecutor(Executors.newVirtualThreadPerTaskExecutor());
    //        };
    //    }
}
```
现在设置JMeter。将有 1000 个请求，该请求将在 3 秒内增加。并且这样的状态会持续200秒。每 3 秒，将触发 1000 个 GET ("/thread") 请求。还添加了响应时间图侦听器。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700377109055-e420b90a-055a-4596-aa63-2f4a8e77145d.png#averageHue=%233f4445&clientId=u15494d72-ec84-4&from=paste&id=ucd0022e3&originHeight=400&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u424c7c1b-21e5-4a50-898a-200c70bb25d&title=)<br />现在运行测试并等待 200 秒。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700377109061-aeaa65ad-2c02-4b99-8020-75244f53551d.png#averageHue=%23a3afa4&clientId=u15494d72-ec84-4&from=paste&id=u92aca882&originHeight=464&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u04318bdb-c9b3-4b84-97a4-f88971fca1f&title=)<br />从图中可以看到，一旦Tomcat的整个线程池被利用，响应时间从3600毫秒猛增到5200毫秒。从那时起，只有当以前的线程被释放时，它才保持这种状态。<br />现在在启用虚拟线程功能的情况下运行负载测试。
```java
package org.anil.virtualthread;

import lombok.extern.slf4j.Slf4j;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.embedded.tomcat.TomcatProtocolHandlerCustomizer;
import org.springframework.context.annotation.Bean;

import java.util.concurrent.Executors;

@SpringBootApplication
@Slf4j
public class VirtualthreadApplication {

    public static void main(String[] args) {
        SpringApplication.run(VirtualthreadApplication.class, args);
    }

    @Bean
    public TomcatProtocolHandlerCustomizer<?> protocolHandlerVirtualThreadExecutorCustomizer() {
        return protocolHandler -> {
            log.info("Configuring " + protocolHandler + " to use VirtualThreadPerTaskExecutor");
            protocolHandler.setExecutor(Executors.newVirtualThreadPerTaskExecutor());
        };
    }

}
```
现在运行测试并等待 200 秒。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700377109043-f23c608b-1201-47f8-8cdf-15cd6cab3acc.png#averageHue=%23c5cdc4&clientId=u15494d72-ec84-4&from=paste&id=u486c72fd&originHeight=468&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua2b50418-442f-4c72-a7e5-39a1fce9615&title=)<br />显然，现在并发 1000 个请求的响应时间几乎略高于 1000 毫秒，有时甚至会达到 1400 毫秒，这比使用普通线程时要好得多。<br />显然，当需要充分利用底层 CPU 时，应该开始在应用程序中采用虚拟线程，突然间可以看到，对于相同的硬件，应用程序的吞吐量增加了很多倍。<br />**这比切换到反应式编程要好得多，反应式编程意味着重写所有代码，这很难先学习，然后编写，甚至更难调试和分析。**<br />简而言之，更多用户可以使用该应用程序并与第一个用户同时获得响应。
