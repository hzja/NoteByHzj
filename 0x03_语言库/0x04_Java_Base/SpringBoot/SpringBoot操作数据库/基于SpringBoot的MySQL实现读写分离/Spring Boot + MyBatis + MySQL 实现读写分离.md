Java SpringBoot Mybatis MySQL 读写分离
<a name="KiyE2"></a>
## 1、引言
读写分离要做的事情就是对于一条SQL该选择哪个数据库去执行，至于谁来做选择数据库这件事儿，无非两个，要么中间件做，要么程序自己做。<br />因此，一般来讲，读写分离有两种实现方式。第一种是依靠中间件（比如：MyCat），也就是说应用程序连接到中间件，中间件做SQL分离；第二种是应用程序自己去做分离。这里选择程序自己来做，主要是利用Spring提供的路由数据源，以及AOP<br />然而，应用程序层面去做读写分离最大的弱点（不足之处）在于无法动态增加数据库节点，因为数据源配置都是写在配置中的，新增数据库意味着新加一个数据源，必然改配置，并重启应用。当然，好处就是相对简单。<br />![2021-09-09-10-35-32-213461.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631155379392-9cc96baf-a210-4bda-ae3c-88097972f979.jpeg#clientId=u7112f53d-f7de-4&from=ui&id=u02f2e9a9&originHeight=423&originWidth=584&originalType=binary&ratio=1&size=47412&status=done&style=shadow&taskId=uae68c93a-00e0-4747-a73f-0238d5eb071)
<a name="ChkHH"></a>
## 2、`AbstractRoutingDataSource`
基于特定的查找key路由到特定的数据源。它内部维护了一组目标数据源，并且做了路由key与目标数据源之间的映射，提供基于key查找数据源的方法。<br />![2021-09-09-10-35-32-334501.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631155379405-64c0638c-e2ff-4ec5-98f0-45eb8c6f5253.png#clientId=u7112f53d-f7de-4&from=ui&id=EZ00N&originHeight=345&originWidth=602&originalType=binary&ratio=1&size=19657&status=done&style=shadow&taskId=u2d5abbab-2e2e-45fb-b1c9-32365862037)
<a name="agCR5"></a>
## 3、实践
<a name="E34Kz"></a>
#### 3.1. maven依赖
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.cjs.example</groupId>
    <artifactId>cjs-datasource-demo</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <packaging>jar</packaging>

    <name>cjs-datasource-demo</name>
    <description></description>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.0.5.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>

    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-aop</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-jdbc</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <version>1.3.2</version>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>3.8</version>
        </dependency>

        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>


            <!--<plugin>
                <groupId>org.mybatis.generator</groupId>
                <artifactId>mybatis-generator-maven-plugin</artifactId>
                <version>1.3.5</version>
                <dependencies>
                    <dependency>
                        <groupId>mysql</groupId>
                        <artifactId>mysql-connector-java</artifactId>
                        <version>5.1.46</version>
                    </dependency>
                </dependencies>
                <configuration>
                    <configurationFile>${basedir}/src/main/resources/myBatisGeneratorConfig.xml</configurationFile>
                    <overwrite>true</overwrite>
                </configuration>
                <executions>
                    <execution>
                        <id>Generate MyBatis Artifacts</id>
                        <goals>
                            <goal>generate</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>-->

        </plugins>
    </build>
</project>
```
<a name="xr1pl"></a>
#### 3.2. 数据源配置
application.yml
```yaml
spring:
  datasource:
    master:
      jdbc-url: jdbc:mysql://192.168.102.31:3306/test
      username: root
      password: 123456
      driver-class-name: com.mysql.jdbc.Driver
    slave1:
      jdbc-url: jdbc:mysql://192.168.102.56:3306/test
      username: pig   # 只读账户
      password: 123456
      driver-class-name: com.mysql.jdbc.Driver
    slave2:
      jdbc-url: jdbc:mysql://192.168.102.36:3306/test
      username: pig   # 只读账户
      password: 123456
      driver-class-name: com.mysql.jdbc.Driver
```
多数据源配置
```java
package com.cjs.example.config;

import com.cjs.example.bean.MyRoutingDataSource;
import com.cjs.example.enums.DBTypeEnum;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.jdbc.DataSourceBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.sql.DataSource;
import java.util.HashMap;
import java.util.Map;

/**
 * 关于数据源配置，参考SpringBoot官方文档第79章《Data Access》
 * 79. Data Access
 * 79.1 Configure a Custom DataSource
 * 79.2 Configure Two DataSources
 */

@Configuration
public class DataSourceConfig {

    @Bean
    @ConfigurationProperties("spring.datasource.master")
    public DataSource masterDataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean
    @ConfigurationProperties("spring.datasource.slave1")
    public DataSource slave1DataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean
    @ConfigurationProperties("spring.datasource.slave2")
    public DataSource slave2DataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean
    public DataSource myRoutingDataSource(@Qualifier("masterDataSource") DataSource masterDataSource,
                                          @Qualifier("slave1DataSource") DataSource slave1DataSource,
                                          @Qualifier("slave2DataSource") DataSource slave2DataSource) {
        Map<Object, Object> targetDataSources = new HashMap<>();
        targetDataSources.put(DBTypeEnum.MASTER, masterDataSource);
        targetDataSources.put(DBTypeEnum.SLAVE1, slave1DataSource);
        targetDataSources.put(DBTypeEnum.SLAVE2, slave2DataSource);
        MyRoutingDataSource myRoutingDataSource = new MyRoutingDataSource();
        myRoutingDataSource.setDefaultTargetDataSource(masterDataSource);
        myRoutingDataSource.setTargetDataSources(targetDataSources);
        return myRoutingDataSource;
    }

}
```
这里，配置了4个数据源，1个master，2两个slave，1个路由数据源。前3个数据源都是为了生成第4个数据源，而且后续只用这最后一个路由数据源。<br />MyBatis配置
```java
package com.cjs.example.config;

import org.apache.ibatis.session.SqlSessionFactory;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.annotation.Resource;
import javax.sql.DataSource;

@EnableTransactionManagement
@Configuration
public class MyBatisConfig {

    @Resource(name = "myRoutingDataSource")
    private DataSource myRoutingDataSource;

    @Bean
    public SqlSessionFactory sqlSessionFactory() throws Exception {
        SqlSessionFactoryBean sqlSessionFactoryBean = new SqlSessionFactoryBean();
        sqlSessionFactoryBean.setDataSource(myRoutingDataSource);
        sqlSessionFactoryBean.setMapperLocations(new PathMatchingResourcePatternResolver().getResources("classpath:mapper/*.xml"));
        return sqlSessionFactoryBean.getObject();
    }

    @Bean
    public PlatformTransactionManager platformTransactionManager() {
        return new DataSourceTransactionManager(myRoutingDataSource);
    }
}
```
由于Spring容器中现在有4个数据源，所以需要为事务管理器和MyBatis手动指定一个明确的数据源。
<a name="ip0Ph"></a>
#### 3.3. 设置路由key / 查找数据源
目标数据源就是那前3个这个是知道的，但是使用的时候是如果查找数据源的呢？<br />首先，定义一个枚举来代表这三个数据源
```java
package com.cjs.example.enums;

public enum DBTypeEnum {

    MASTER, SLAVE1, SLAVE2;

}
```
接下来，通过ThreadLocal将数据源设置到每个线程上下文中
```java
package com.cjs.example.bean;

import com.cjs.example.enums.DBTypeEnum;

import java.util.concurrent.atomic.AtomicInteger;

public class DBContextHolder {

    private static final ThreadLocal<DBTypeEnum> contextHolder = new ThreadLocal<>();

    private static final AtomicInteger counter = new AtomicInteger(-1);

    public static void set(DBTypeEnum dbType) {
        contextHolder.set(dbType);
    }

    public static DBTypeEnum get() {
        return contextHolder.get();
    }

    public static void master() {
        set(DBTypeEnum.MASTER);
        System.out.println("切换到master");
    }

    public static void slave() {
        //  轮询
        int index = counter.getAndIncrement() % 2;
        if (counter.get() > 9999) {
            counter.set(-1);
        }
        if (index == 0) {
            set(DBTypeEnum.SLAVE1);
            System.out.println("切换到slave1");
        }else {
            set(DBTypeEnum.SLAVE2);
            System.out.println("切换到slave2");
        }
    }
}
```
获取路由key
```java
package com.cjs.example.bean;

import org.springframework.jdbc.datasource.lookup.AbstractRoutingDataSource;
import org.springframework.lang.Nullable;

public class MyRoutingDataSource extends AbstractRoutingDataSource {
    @Nullable
    @Override
    protected Object determineCurrentLookupKey() {
        return DBContextHolder.get();
    }
}
```
设置路由key<br />默认情况下，所有的查询都走从库，插入/修改/删除走主库。通过方法名来区分操作类型（CRUD）
```java

package com.cjs.example.aop;

import com.cjs.example.bean.DBContextHolder;
import org.apache.commons.lang3.StringUtils;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

@Aspect
@Component
public class DataSourceAop {

    @Pointcut("!@annotation(com.cjs.example.annotation.Master) " +
            "&& (execution(* com.cjs.example.service..*.select*(..)) " +
            "|| execution(* com.cjs.example.service..*.get*(..)))")
    public void readPointcut() {

    }

    @Pointcut("@annotation(com.cjs.example.annotation.Master) " +
            "|| execution(* com.cjs.example.service..*.insert*(..)) " +
            "|| execution(* com.cjs.example.service..*.add*(..)) " +
            "|| execution(* com.cjs.example.service..*.update*(..)) " +
            "|| execution(* com.cjs.example.service..*.edit*(..)) " +
            "|| execution(* com.cjs.example.service..*.delete*(..)) " +
            "|| execution(* com.cjs.example.service..*.remove*(..))")
    public void writePointcut() {

    }

    @Before("readPointcut()")
    public void read() {
        DBContextHolder.slave();
    }

    @Before("writePointcut()")
    public void write() {
        DBContextHolder.master();
    }


    /**
     * 另一种写法：if...else...  判断哪些需要读从数据库，其余的走主数据库
     */
//    @Before("execution(* com.cjs.example.service.impl.*.*(..))")
//    public void before(JoinPoint jp) {
//        String methodName = jp.getSignature().getName();
//
//        if (StringUtils.startsWithAny(methodName, "get", "select", "find")) {
//            DBContextHolder.slave();
//        }else {
//            DBContextHolder.master();
//        }
//    }
}
```
有一般情况就有特殊情况，特殊情况是某些情况下需要强制读主库，针对这种情况，定义一个主键，用该注解标注的就读主库
```java
package com.cjs.example.annotation;

public @interface Master {
}
```
例如，假设有一张表member
```java
package com.cjs.example.service.impl;

import com.cjs.example.annotation.Master;
import com.cjs.example.entity.Member;
import com.cjs.example.entity.MemberExample;
import com.cjs.example.mapper.MemberMapper;
import com.cjs.example.service.MemberService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
public class MemberServiceImpl implements MemberService {

    @Autowired
    private MemberMapper memberMapper;

    @Transactional
    @Override
    public int insert(Member member) {
        return memberMapper.insert(member);
    }

    @Master
    @Override
    public int save(Member member) {
        return memberMapper.insert(member);
    }

    @Override
    public List<Member> selectAll() {
        return memberMapper.selectByExample(new MemberExample());
    }

    @Master
    @Override
    public String getToken(String appId) {
        //  有些读操作必须读主数据库
        //  比如，获取微信access_token，因为高峰时期主从同步可能延迟
        //  这种情况下就必须强制从主数据读
        return null;
    }
}
```
<a name="waH2j"></a>
## 4、测试
```java
package com.cjs.example;

import com.cjs.example.entity.Member;
import com.cjs.example.service.MemberService;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

@RunWith(SpringRunner.class)
@SpringBootTest
public class CjsDatasourceDemoApplicationTests {

    @Autowired
    private MemberService memberService;

    @Test
    public void testWrite() {
        Member member = new Member();
        member.setName("zhangsan");
        memberService.insert(member);
    }

    @Test
    public void testRead() {
        for (int i = 0; i < 4; i++) {
            memberService.selectAll();
        }
    }

    @Test
    public void testSave() {
        Member member = new Member();
        member.setName("wangwu");
        memberService.save(member);
    }

    @Test
    public void testReadFromMaster() {
        memberService.getToken("1234");
    }

}
```
查看控制台<br />![2021-09-09-10-35-32-446502.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631154979818-e2b2b076-7a55-4c31-aa26-06a0551adc1a.png#clientId=u7112f53d-f7de-4&from=ui&id=u4d020903&originHeight=215&originWidth=825&originalType=binary&ratio=1&size=32749&status=done&style=shadow&taskId=u136b5241-9ebf-496f-bc0a-dd4740834b8)![2021-09-09-10-35-32-587466.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631154979817-ba0cd5f9-fd37-4728-a038-bb329489321d.png#clientId=u7112f53d-f7de-4&from=ui&id=ub4d7f564&originHeight=349&originWidth=815&originalType=binary&ratio=1&size=45852&status=done&style=shadow&taskId=u33e99f96-2cbb-41a6-b1ef-0031a290b3d)![2021-09-09-10-35-32-713467.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631154979825-ee33a3fd-e23a-4737-b868-71cccac14b6e.png#clientId=u7112f53d-f7de-4&from=ui&id=ubac991ae&originHeight=197&originWidth=682&originalType=binary&ratio=1&size=27444&status=done&style=shadow&taskId=u0cdb6ddb-83bc-4e0e-85c3-413588bcc69)![2021-09-09-10-35-32-830461.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631154979976-da498b6c-90f3-44f8-ae40-2ae361f61448.png#clientId=u7112f53d-f7de-4&from=ui&id=u3856175b&originHeight=119&originWidth=638&originalType=binary&ratio=1&size=17479&status=done&style=shadow&taskId=uc5df0edf-15a2-4a87-8cbf-9f86d6c366d)
<a name="j7pWC"></a>
## 5、工程结构
![2021-09-09-10-35-32-969460.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631154979848-a754e3b9-866d-4e66-9339-09082a4a186a.png#clientId=u7112f53d-f7de-4&from=ui&id=BJMBF&originHeight=1499&originWidth=535&originalType=binary&ratio=1&size=82344&status=done&style=shadow&taskId=u9376a5e0-ed88-46ec-ae3e-98a76a2a274)
