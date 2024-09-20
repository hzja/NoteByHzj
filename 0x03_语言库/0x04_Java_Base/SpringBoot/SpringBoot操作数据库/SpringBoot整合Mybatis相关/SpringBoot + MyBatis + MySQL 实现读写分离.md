JavaSpringBootMybatis
<a name="DuGYs"></a>
## 引言
读写分离要做的事情就是对于一条SQL该选择哪个数据库去执行，至于谁来做选择数据库这件事儿，无非两个，要么中间件做，要么程序自己做。因此，一般来讲，读写分离有两种实现方式。第一种是依靠中间件（比如：MyCat），也就是说应用程序连接到中间件，中间件做SQL分离；第二种是应用程序自己去做分离。这里选择程序自己来做，主要是利用Spring提供的路由数据源，以及AOP<br />然而，应用程序层面去做读写分离最大的弱点（不足之处）在于无法动态增加数据库节点，因为数据源配置都是写在配置中的，新增数据库意味着新加一个数据源，必然改配置，并重启应用。当然，好处就是相对简单。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652926768916-b31cd891-3ad5-434b-991f-d367cbdc3e61.jpeg#clientId=udc33e030-3824-4&from=paste&id=u8766b85e&originHeight=423&originWidth=584&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc3a695ac-c50b-4647-8af7-c38d5228950&title=)
<a name="HEEUv"></a>
## `AbstractRoutingDataSource`
基于特定的查找key路由到特定的数据源。它内部维护了一组目标数据源，并且做了路由key与目标数据源之间的映射，提供基于key查找数据源的方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652926768947-b74d9ac3-4542-40e2-9868-c5ae376aa50f.png#clientId=udc33e030-3824-4&from=paste&id=u25feedb2&originHeight=345&originWidth=602&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc11263db-e7c4-4bb5-8047-557d990ac01&title=)
<a name="YYvYC"></a>
## 实践
<a name="LsJUI"></a>
### Maven依赖
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
<a name="Weuxk"></a>
### 数据源配置
<a name="S5Xjm"></a>
#### application.yml
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
<a name="hKEVr"></a>
#### 多数据源配置
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
这里配置了4个数据源，1个master，2两个slave，1个路由数据源。前3个数据源都是为了生成第4个数据源，而且后续只用这最后一个路由数据源。
<a name="Cljiw"></a>
#### MyBatis配置
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
<a name="U9VaH"></a>
### 设置路由key / 查找数据源
目标数据源就是那前3个这个是知道的，但是使用的时候是如果查找数据源的呢？<br />首先定义一个枚举来代表这三个数据源
```java
package com.cjs.example.enums;

public enum DBTypeEnum {

    MASTER, SLAVE1, SLAVE2;

}
```
接下来，通过`ThreadLocal`将数据源设置到每个线程上下文中
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
<a name="kyg7D"></a>
#### 获取路由key
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
<a name="NGgqe"></a>
#### 设置路由key
默认情况下，所有的查询都走从库，插入/修改/删除走主库。通过方法名来区分操作类型（CRUD）
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
<a name="tupXY"></a>
## 测试
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
查看控制台<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652926769114-3b292b62-1caf-400e-9fd4-7eea1694cc34.png#clientId=udc33e030-3824-4&from=paste&id=u53fcfd21&originHeight=215&originWidth=825&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufd20988f-fa59-4ffe-88f7-d5caa3479d8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652926768910-d940f747-549f-4a18-97b9-ab47af7a5958.png#clientId=udc33e030-3824-4&from=paste&id=u0c6f110d&originHeight=349&originWidth=815&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u662b4197-8b77-4767-b108-9caaf090429&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652926768948-c7337de0-cd9d-4104-8d7a-e62d5d7173e0.png#clientId=udc33e030-3824-4&from=paste&id=ue6a7d479&originHeight=197&originWidth=682&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u06059feb-249e-4e40-81f6-7a76fe514e1&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652926769240-f4835314-04a5-4f43-a60a-5d30515bc735.png#clientId=udc33e030-3824-4&from=paste&id=udad4047b&originHeight=119&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u74777792-2abe-4db0-b93a-77c102ca9f2&title=)
<a name="DpJQz"></a>
## 工程结构
<a name="Ih7Pw"></a>
## ![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652926769274-b4e14267-72ab-4b93-bd75-ad5a15ee7b2b.png#clientId=udc33e030-3824-4&from=paste&id=ua3247a2a&originHeight=1499&originWidth=535&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue4a5cbce-98ee-4ef4-b2bc-8ab49a1750e&title=)
