Java SpringBoot JTA<br />首先，到底什么是分布式事务呢，比如在执行一个业务逻辑的时候有两步分别操作A数据源和B数据源，当在A数据源执行数据更改后，在B数据源执行时出现运行时异常，那么必须要让B数据源的操作回滚，并回滚对A数据源的操作；这种情况在支付业务时常常出现；比如买票业务在最后支付失败，那之前的操作必须全部回滚，如果之前的操作分布在多个数据源中，那么这就是典型的分布式事务回滚；<br />了解了什么是分布式事务，那分布式事务在java的解决方案就是JTA(即Java Transaction API)；SpringBoot官方提供了 Atomikos or Bitronix的解决思路；<br />其实，大多数情况下很多公司是使用消息队列的方式实现分布式事务。<br />这里重点讲解SpringBoot环境下，整合 Atomikos +mysql+mybatis+tomcat/jetty；
<a name="Hnk7K"></a>
##### 一、项目依赖
pom.xml中添加atomikos的SpringBoot相关依赖：
```xml
<!--分布式事务-->
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-jta-atomikos</artifactId>
</dependency>
```
点进去会发现里面整合好了：transactions-jms、transactions-jta、transactions-jdbc、javax.transaction-api
<a name="OrBEG"></a>
##### 二、把数据源的相关配置项单独提炼到一个application.yml中：
注意：

1. 这回`spring.datasource.type` 是`com.alibaba.druid.pool.xa.DruidXADataSource;`
2. `spring.jta.transaction-manager-id`的值在电脑中是唯一的，这个详细请阅读官方文档；

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649210093094-22a52fb3-e994-4bfc-b800-a7bae42ddfa4.png#clientId=ucfd949d7-1b71-4&from=paste&id=u0b28cba8&originHeight=87&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufd9fa213-bdb6-4dc9-aed7-bd4814a33ee&title=)<br />完整的yml文件如下：
```yaml
spring:
  datasource:
    type: com.alibaba.druid.pool.xa.DruidXADataSource
    druid:
    
      systemDB:
        name: systemDB
        url: jdbc:mysql://localhost:3306/springboot-mybatis?useUnicode=true&characterEncoding=utf-8
        username: root
        password: root
        # 下面为连接池的补充设置，应用到上面所有数据源中
        # 初始化大小，最小，最大
        initialSize: 5
        minIdle: 5
        maxActive: 20
        # 配置获取连接等待超时的时间
        maxWait: 60000
        # 配置间隔多久才进行一次检测，检测需要关闭的空闲连接，单位是毫秒
        timeBetweenEvictionRunsMillis: 60000
        # 配置一个连接在池中最小生存的时间，单位是毫秒
        minEvictableIdleTimeMillis: 30
        validationQuery: SELECT 1
        validationQueryTimeout: 10000
        testWhileIdle: true
        testOnBorrow: false
        testOnReturn: false
        # 打开PSCache，并且指定每个连接上PSCache的大小
        poolPreparedStatements: true
        maxPoolPreparedStatementPerConnectionSize: 20
        filters: stat,wall
        # 通过connectProperties属性来打开mergeSql功能；慢SQL记录
        connectionProperties: druid.stat.mergeSql=true;druid.stat.slowSqlMillis=5000
        # 合并多个DruidDataSource的监控数据
        useGlobalDataSourceStat: true
 
      businessDB:
        name: businessDB
 
        url: jdbc:mysql://localhost:3306/springboot-mybatis2?useUnicode=true&characterEncoding=utf-8
        username: root
        password: root
        # 下面为连接池的补充设置，应用到上面所有数据源中
        # 初始化大小，最小，最大
        initialSize: 5
        minIdle: 5
        maxActive: 20
        # 配置获取连接等待超时的时间
        maxWait: 60000
        # 配置间隔多久才进行一次检测，检测需要关闭的空闲连接，单位是毫秒
        timeBetweenEvictionRunsMillis: 60000
        # 配置一个连接在池中最小生存的时间，单位是毫秒
        minEvictableIdleTimeMillis: 30
        validationQuery: SELECT 1
        validationQueryTimeout: 10000
        testWhileIdle: true
        testOnBorrow: false
        testOnReturn: false
        # 打开PSCache，并且指定每个连接上PSCache的大小
        poolPreparedStatements: true
        maxPoolPreparedStatementPerConnectionSize: 20
        filters: stat,wall
        # 通过connectProperties属性来打开mergeSql功能；慢SQL记录
        connectionProperties: druid.stat.mergeSql=true;druid.stat.slowSqlMillis=5000
        # 合并多个DruidDataSource的监控数据
        useGlobalDataSourceStat: true
 
  #jta相关参数配置
  jta:
    log-dir: classpath:tx-logs
    transaction-manager-id: txManager
```
<a name="fJScF"></a>
##### 三、在DruidConfig.java中实现多个数据源的注册；分布式事务管理器的注册；druid的注册；
```java
package com.zjt.config;
 
import com.alibaba.druid.filter.stat.StatFilter;
import com.alibaba.druid.support.http.StatViewServlet;
import com.alibaba.druid.support.http.WebStatFilter;
import com.alibaba.druid.wall.WallConfig;
import com.alibaba.druid.wall.WallFilter;
import com.atomikos.icatch.jta.UserTransactionImp;
import com.atomikos.icatch.jta.UserTransactionManager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.jta.atomikos.AtomikosDataSourceBean;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.boot.web.servlet.ServletRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.core.env.Environment;
import org.springframework.transaction.jta.JtaTransactionManager;
 
import javax.sql.DataSource;
import javax.transaction.UserTransaction;
import java.util.Properties;
 
/**
 * Druid配置
 *
 * 
 */
@Configuration
public class DruidConfig {
    @Bean(name = "systemDataSource")
    @Primary
    @Autowired
    public DataSource systemDataSource(Environment env) {
        AtomikosDataSourceBean ds = new AtomikosDataSourceBean();
        Properties prop = build(env, "spring.datasource.druid.systemDB.");
        ds.setXaDataSourceClassName("com.alibaba.druid.pool.xa.DruidXADataSource");
        ds.setUniqueResourceName("systemDB");
        ds.setPoolSize(5);
        ds.setXaProperties(prop);
        return ds;
 
    }
 
    @Autowired
    @Bean(name = "businessDataSource")
    public AtomikosDataSourceBean businessDataSource(Environment env) {
 
        AtomikosDataSourceBean ds = new AtomikosDataSourceBean();
        Properties prop = build(env, "spring.datasource.druid.businessDB.");
        ds.setXaDataSourceClassName("com.alibaba.druid.pool.xa.DruidXADataSource");
        ds.setUniqueResourceName("businessDB");
        ds.setPoolSize(5);
        ds.setXaProperties(prop);
 
        return ds;
    }
 
 
    /**
     * 注入事物管理器
     * @return
     */
    @Bean(name = "xatx")
    public JtaTransactionManager regTransactionManager () {
        UserTransactionManager userTransactionManager = new UserTransactionManager();
        UserTransaction userTransaction = new UserTransactionImp();
        return new JtaTransactionManager(userTransaction, userTransactionManager);
    }
 
 
    private Properties build(Environment env, String prefix) {
 
        Properties prop = new Properties();
        prop.put("url", env.getProperty(prefix + "url"));
        prop.put("username", env.getProperty(prefix + "username"));
        prop.put("password", env.getProperty(prefix + "password"));
        prop.put("driverClassName", env.getProperty(prefix + "driverClassName", ""));
        prop.put("initialSize", env.getProperty(prefix + "initialSize", Integer.class));
        prop.put("maxActive", env.getProperty(prefix + "maxActive", Integer.class));
        prop.put("minIdle", env.getProperty(prefix + "minIdle", Integer.class));
        prop.put("maxWait", env.getProperty(prefix + "maxWait", Integer.class));
        prop.put("poolPreparedStatements", env.getProperty(prefix + "poolPreparedStatements", Boolean.class));
 
        prop.put("maxPoolPreparedStatementPerConnectionSize",
                env.getProperty(prefix + "maxPoolPreparedStatementPerConnectionSize", Integer.class));
 
        prop.put("maxPoolPreparedStatementPerConnectionSize",
                env.getProperty(prefix + "maxPoolPreparedStatementPerConnectionSize", Integer.class));
        prop.put("validationQuery", env.getProperty(prefix + "validationQuery"));
        prop.put("validationQueryTimeout", env.getProperty(prefix + "validationQueryTimeout", Integer.class));
        prop.put("testOnBorrow", env.getProperty(prefix + "testOnBorrow", Boolean.class));
        prop.put("testOnReturn", env.getProperty(prefix + "testOnReturn", Boolean.class));
        prop.put("testWhileIdle", env.getProperty(prefix + "testWhileIdle", Boolean.class));
        prop.put("timeBetweenEvictionRunsMillis",
                env.getProperty(prefix + "timeBetweenEvictionRunsMillis", Integer.class));
        prop.put("minEvictableIdleTimeMillis", env.getProperty(prefix + "minEvictableIdleTimeMillis", Integer.class));
        prop.put("filters", env.getProperty(prefix + "filters"));
 
        return prop;
    }
 
    @Bean
    public ServletRegistrationBean druidServlet() {
        ServletRegistrationBean servletRegistrationBean = new ServletRegistrationBean(new StatViewServlet(), "/druid/*");
 
        //控制台管理用户，加入下面2行 进入druid后台就需要登录
        //servletRegistrationBean.addInitParameter("loginUsername", "admin");
        //servletRegistrationBean.addInitParameter("loginPassword", "admin");
        return servletRegistrationBean;
    }
 
    @Bean
    public FilterRegistrationBean filterRegistrationBean() {
        FilterRegistrationBean filterRegistrationBean = new FilterRegistrationBean();
        filterRegistrationBean.setFilter(new WebStatFilter());
        filterRegistrationBean.addUrlPatterns("/*");
        filterRegistrationBean.addInitParameter("exclusions", "*.js,*.gif,*.jpg,*.png,*.css,*.ico,/druid/*");
        filterRegistrationBean.addInitParameter("profileEnable", "true");
        return filterRegistrationBean;
    }
 
    @Bean
    public StatFilter statFilter(){
        StatFilter statFilter = new StatFilter();
        statFilter.setLogSlowSql(true); //slowSqlMillis用来配置SQL慢的标准，执行时间超过slowSqlMillis的就是慢。
        statFilter.setMergeSql(true); //SQL合并配置
        statFilter.setSlowSqlMillis(1000);//slowSqlMillis的缺省值为3000，也就是3秒。
        return statFilter;
    }
 
    @Bean
    public WallFilter wallFilter(){
        WallFilter wallFilter = new WallFilter();
        //允许执行多条SQL
        WallConfig config = new WallConfig();
        config.setMultiStatementAllow(true);
        wallFilter.setConfig(config);
        return wallFilter;
    }
 
}
```
<a name="y6pNH"></a>
##### 四、分别配置每个数据源对应的sqlSessionFactory，以及MapperScan扫描的包：
MybatisDatasourceConfig.java
```java
package com.zjt.config;
 
import com.zjt.util.MyMapper;
import org.apache.ibatis.session.SqlSessionFactory;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.SqlSessionTemplate;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;
 
import javax.sql.DataSource;
 
/**
 * 
 * @description
 */
@Configuration
// 精确到 mapper 目录，以便跟其他数据源隔离
@MapperScan(basePackages = "com.zjt.mapper", markerInterface = MyMapper.class, sqlSessionFactoryRef = "sqlSessionFactory")
public class MybatisDatasourceConfig {
 
    @Autowired
    @Qualifier("systemDataSource")
    private DataSource ds;
 
    @Bean
    public SqlSessionFactory sqlSessionFactory() throws Exception {
        SqlSessionFactoryBean factoryBean = new SqlSessionFactoryBean();
        factoryBean.setDataSource(ds);
        //指定mapper xml目录
        ResourcePatternResolver resolver = new PathMatchingResourcePatternResolver();
        factoryBean.setMapperLocations(resolver.getResources("classpath:mapper/*.xml"));
        return factoryBean.getObject();
 
    }
 
    @Bean
    public SqlSessionTemplate sqlSessionTemplate() throws Exception {
        SqlSessionTemplate template = new SqlSessionTemplate(sqlSessionFactory()); // 使用上面配置的Factory
        return template;
    }
 
    //关于事务管理器，不管是JPA还是JDBC等都实现自接口 PlatformTransactionManager
    // 如果你添加的是 spring-boot-starter-jdbc 依赖，框架会默认注入 DataSourceTransactionManager 实例。
    //在Spring容器中，我们手工注解@Bean 将被优先加载，框架不会重新实例化其他的 PlatformTransactionManager 实现类。
    /*@Bean(name = "transactionManager")
    @Primary
    public DataSourceTransactionManager masterTransactionManager() {
        //MyBatis自动参与到spring事务管理中，无需额外配置，只要org.mybatis.spring.SqlSessionFactoryBean引用的数据源
        // 与DataSourceTransactionManager引用的数据源一致即可，否则事务管理会不起作用。
        return new DataSourceTransactionManager(ds);
    }*/
 
}
```
MybatisDatasource2Config.java
```java
package com.zjt.config;

import com.zjt.util.MyMapper;
import org.apache.ibatis.session.SqlSessionFactory;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.SqlSessionTemplate;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;
 
import javax.sql.DataSource;
 
/**
 * 
 * @description
 */
@Configuration
// 精确到 mapper 目录，以便跟其他数据源隔离
@MapperScan(basePackages = "com.zjt.mapper2", markerInterface = MyMapper.class, sqlSessionFactoryRef = "sqlSessionFactory2")
public class MybatisDatasource2Config {
 
    @Autowired
    @Qualifier("businessDataSource")
    private DataSource ds;
 
    @Bean
    public SqlSessionFactory sqlSessionFactory2() throws Exception {
        SqlSessionFactoryBean factoryBean = new SqlSessionFactoryBean();
        factoryBean.setDataSource(ds);
        //指定mapper xml目录
        ResourcePatternResolver resolver = new PathMatchingResourcePatternResolver();
        factoryBean.setMapperLocations(resolver.getResources("classpath:mapper2/*.xml"));
        return factoryBean.getObject();
 
    }
 
    @Bean
    public SqlSessionTemplate sqlSessionTemplate2() throws Exception {
        SqlSessionTemplate template = new SqlSessionTemplate(sqlSessionFactory2()); // 使用上面配置的Factory
        return template;
    }
 
    //关于事务管理器，不管是JPA还是JDBC等都实现自接口 PlatformTransactionManager
    // 如果你添加的是 spring-boot-starter-jdbc 依赖，框架会默认注入 DataSourceTransactionManager 实例。
    //在Spring容器中，我们手工注解@Bean 将被优先加载，框架不会重新实例化其他的 PlatformTransactionManager 实现类。
    /*@Bean(name = "transactionManager2")
    @Primary
    public DataSourceTransactionManager masterTransactionManager() {
        //MyBatis自动参与到spring事务管理中，无需额外配置，只要org.mybatis.spring.SqlSessionFactoryBean引用的数据源
        // 与DataSourceTransactionManager引用的数据源一致即可，否则事务管理会不起作用。
        return new DataSourceTransactionManager(ds);
    }*/
 
}
```
由于本例中只使用一个事务管理器：xatx，故就不在使用TxAdviceInterceptor.java和TxAdvice2Interceptor.java中配置的事务管理器了；有需求的可以自己配置其他的事务管理器；(见DruidConfig.java中查看)
<a name="b4jNz"></a>
##### 五、新建分布式业务测试接口JtaTestService.java和实现类JtaTestServiceImpl.java
其实就是一个很简单的`test01()`方法，在该方法中分别先后调用`classService.saveOrUpdateTClass(tClass);`和`teacherService.saveOrUpdateTeacher(teacher);`<br />实现先后操作两个数据源：然后可以自己debug跟踪事务的提交时机，此外，也可以在在两个方法全执行结束之后，手动制造一个运行时异常，来检查分布式事务是否全部回滚；<br />注意：<br />在实现类的方法中使用的是：
```java
@Transactional(transactionManager = "xatx", propagation = Propagation.REQUIRED, rollbackFor = { java.lang.RuntimeException.class })
```
从而指定了使用哪个事务管理器，事务隔离级别(一般都用这个默认的)，回滚的条件(一般可以使用Exception)，这三个可以自己根据业务实际修改；
```java
package com.zjt.service3;

import java.util.Map;
 
public interface JtaTestService {
 
    public Map<String,Object> test01();
 
}
```
```java
package com.zjt.service3.impl;

import com.zjt.entity.TClass;
import com.zjt.entity.Teacher;
import com.zjt.service.TClassService;
import com.zjt.service2.TeacherService;
import com.zjt.service3.JtaTestService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
 
import java.util.LinkedHashMap;
import java.util.Map;
 
@Service("jtaTestServiceImpl")
public class JtaTestServiceImpl implements JtaTestService{
 
    @Autowired
    @Qualifier("teacherServiceImpl")
    private TeacherService teacherService;
    @Autowired
    @Qualifier("tclassServiceImpl")
    private TClassService tclassService;
 
    @Override
    @Transactional(transactionManager = "xatx", propagation = Propagation.REQUIRED, rollbackFor = { java.lang.RuntimeException.class })
    public Map<String, Object> test01() {
        LinkedHashMap<String,Object> resultMap=new LinkedHashMap<String,Object>();
        TClass tClass=new TClass();
        tClass.setName("8888");
        tclassService.saveOrUpdateTClass(tClass);
 
        Teacher teacher=new Teacher();
        teacher.setName("8888");
        teacherService.saveOrUpdateTeacher(teacher);
 
        System.out.println(1/0);
 
        resultMap.put("state","success");
        resultMap.put("message","分布式事务同步成功");
        return resultMap;
    }
}
```
<a name="T3l8T"></a>
##### 六、建立JtaTestContoller.java，接受一个来自前端的http请求，触发JtaTestService 的test01方法:
```java
package com.zjt.web;

import com.zjt.service3.JtaTestService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.LinkedHashMap;
import java.util.Map;

@Controller
@RequestMapping("/jtaTest")
public class JtaTestContoller {
	
	@Autowired
	@Qualifier("jtaTestServiceImpl")
	private JtaTestService taTestService;
	
	
	
	@ResponseBody
	@RequestMapping("/test01")
	public Map<String,Object> test01(){
		LinkedHashMap<String,Object> resultMap=new LinkedHashMap<String,Object>();
		try {
			return taTestService.test01();
		}catch (Exception e){
			resultMap.put("state","fail");
			resultMap.put("message","分布式事务同步失败");
			return resultMap;
		}
	}
}
```
<a name="EVJjf"></a>
##### 七、在test.ftl中增加一个按钮来测试；
```javascript
//分布式事务测试
$("#JTATest").click(function(){
	$.ajax({
		type: "POST",
		url: "${basePath!}/jtaTest/test01",
		data: {}    ,
		async: false,
		error: function (request) {
			layer.alert("与服务器连接失败/(ㄒoㄒ)/~~");
			return false;
		},
		success: function (data) {
			if (data.state == 'fail') {
				layer.alert(data.message);
				return false;
			}else if(data.state == 'success'){
				layer.alert(data.message);
			}
		}
	});
});

<button class="layui-btn" id="JTATest">同时向班级和老师表插入名为8888的班级和老师</button>
```
<a name="ePK2K"></a>
##### 八、启动服务，验证结果：
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649210093143-c4627ab8-a5ba-4d12-8fb0-9896765ac993.png#clientId=ucfd949d7-1b71-4&from=paste&id=uc6983c9a&originHeight=539&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u181a7f62-34e0-4901-a594-409251fe01b&title=)<br />点击这个按钮，跳转到controller：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649210093112-3d118c8b-9d6a-487e-9488-f6076810fa81.png#clientId=ucfd949d7-1b71-4&from=paste&id=u200cbf65&originHeight=364&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud136ca27-6600-430a-a9c7-07c86ca8d29&title=)<br />当正常执行了sql语句之后，可以发现数据库并没有变化，因为整个方法的事务还没有走完，当走到1/0这步时：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649210093156-401793c1-7714-4e41-8062-75778b3d141f.png#clientId=ucfd949d7-1b71-4&from=paste&id=ub2631f69&originHeight=433&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u97fcf277-8c89-4b68-853b-f2c9ee352aa&title=)<br />抛出运行时异常，并被spring事务拦截器拦截，并捕获异常：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649210093165-976bc161-869d-470b-941b-da8e62f9438c.png#clientId=ucfd949d7-1b71-4&from=paste&id=ud5b2436f&originHeight=443&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua8723214-6d27-43d3-8345-eac8703e0e6&title=)<br />在`this.completeTransactionAfterThrowing(txInfo, var16);`方法中会将事务全部回滚：
```
22:09:04.243 logback [http-nio-8080-exec-5] INFO c.a.i.imp.CompositeTransactionImp - rollback() done of transaction 192.168.1.103.tm0000400006
```
此时，当再次打开数据库验证，依旧没有变化，证明分布式事务配置成功。
<a name="xdqte"></a>
##### 九、后记：
本文源代码：[https://github.com/zhaojiatao/springboot-zjt-chapter10-springboot-atomikos-mysql-mybatis-druid.git](https://github.com/zhaojiatao/springboot-zjt-chapter10-springboot-atomikos-mysql-mybatis-druid.git)<br />代码在tomcat和jetty环境下均可完成事务回滚。在事务回滚时可能报一个Transactional not active的警告，大部分人认为这只是一个警告，可以忽略。
