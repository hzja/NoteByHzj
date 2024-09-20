<a name="zLQPr"></a>
# 1、项目版本

| **Dev** | **Version** |
| :---: | :---: |
| JDK | 8 |
| SpringBoot | 2.2.1 |
|  |  |
|  |  |


<a name="aSL2j"></a>
# 2、报错日志

```java
"C:\Program Files\Java\jdk1.8.0_192\bin\java.exe" -agentlib:jdwp=transport=dt_socket,address=127.0.0.1:52601,suspend=y,server=n -XX:TieredStopAtLevel=1 -noverify -Dspring.output.ansi.enabled=always -Dcom.sun.management.jmxremote -Dspring.jmx.enabled=true -Dspring.liveBeansView.mbeanDomain -Dspring.application.admin.enabled=true -javaagent:D:\DevTools\ideaIU-2019.2.4.win\plugins\java\lib\rt\debugger-agent.jar -Dfile.encoding=UTF-8 -classpath "C:\Program Files\Java\jdk1.8.0_192\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\deploy.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\access-bridge-64.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\cldrdata.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\dnsns.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\jaccess.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\jfxrt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\localedata.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\nashorn.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunec.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunjce_provider.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunmscapi.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunpkcs11.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\zipfs.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\javaws.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jce.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfr.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfxswt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\management-agent.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\plugin.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\resources.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\rt.jar;D:\Dome\SpringBootHD\SpringBootData\target\classes;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\mysql\mysql-connector-java\8.0.17\mysql-connector-java-8.0.17.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-data-jdbc\2.2.1.RELEASE\spring-boot-starter-data-jdbc-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\data\spring-data-jdbc\1.1.1.RELEASE\spring-data-jdbc-1.1.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\data\spring-data-relational\1.1.1.RELEASE\spring-data-relational-1.1.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\data\spring-data-commons\2.2.1.RELEASE\spring-data-commons-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-tx\5.2.1.RELEASE\spring-tx-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-context\5.2.1.RELEASE\spring-context-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-beans\5.2.1.RELEASE\spring-beans-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\slf4j\slf4j-api\1.7.29\slf4j-api-1.7.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-data-jpa\2.2.1.RELEASE\spring-boot-starter-data-jpa-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-aop\2.2.1.RELEASE\spring-boot-starter-aop-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-aop\5.2.1.RELEASE\spring-aop-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\aspectj\aspectjweaver\1.9.4\aspectjweaver-1.9.4.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\activation\jakarta.activation-api\1.2.1\jakarta.activation-api-1.2.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\persistence\jakarta.persistence-api\2.2.3\jakarta.persistence-api-2.2.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\transaction\jakarta.transaction-api\1.3.3\jakarta.transaction-api-1.3.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\hibernate\hibernate-core\5.4.8.Final\hibernate-core-5.4.8.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\jboss\logging\jboss-logging\3.4.1.Final\jboss-logging-3.4.1.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\javassist\javassist\3.24.0-GA\javassist-3.24.0-GA.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\net\bytebuddy\byte-buddy\1.10.2\byte-buddy-1.10.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\antlr\antlr\2.7.7\antlr-2.7.7.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\jboss\jandex\2.0.5.Final\jandex-2.0.5.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\classmate\1.5.1\classmate-1.5.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\dom4j\dom4j\2.1.1\dom4j-2.1.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\hibernate\common\hibernate-commons-annotations\5.1.0.Final\hibernate-commons-annotations-5.1.0.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\glassfish\jaxb\jaxb-runtime\2.3.2\jaxb-runtime-2.3.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\glassfish\jaxb\txw2\2.3.2\txw2-2.3.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\istack\istack-commons-runtime\3.0.8\istack-commons-runtime-3.0.8.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\jvnet\staxex\stax-ex\1.8.1\stax-ex-1.8.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\xml\fastinfoset\FastInfoset\1.2.16\FastInfoset-1.2.16.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\data\spring-data-jpa\2.2.1.RELEASE\spring-data-jpa-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-orm\5.2.1.RELEASE\spring-orm-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-aspects\5.2.1.RELEASE\spring-aspects-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-jdbc\2.2.1.RELEASE\spring-boot-starter-jdbc-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter\2.2.1.RELEASE\spring-boot-starter-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-logging\2.2.1.RELEASE\spring-boot-starter-logging-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\ch\qos\logback\logback-classic\1.2.3\logback-classic-1.2.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\ch\qos\logback\logback-core\1.2.3\logback-core-1.2.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\logging\log4j\log4j-to-slf4j\2.12.1\log4j-to-slf4j-2.12.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\logging\log4j\log4j-api\2.12.1\log4j-api-2.12.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\slf4j\jul-to-slf4j\1.7.29\jul-to-slf4j-1.7.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\annotation\jakarta.annotation-api\1.3.5\jakarta.annotation-api-1.3.5.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\yaml\snakeyaml\1.25\snakeyaml-1.25.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\zaxxer\HikariCP\3.4.1\HikariCP-3.4.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-jdbc\5.2.1.RELEASE\spring-jdbc-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-web\2.2.1.RELEASE\spring-boot-starter-web-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-json\2.2.1.RELEASE\spring-boot-starter-json-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\core\jackson-databind\2.10.0\jackson-databind-2.10.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\core\jackson-annotations\2.10.0\jackson-annotations-2.10.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\core\jackson-core\2.10.0\jackson-core-2.10.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\datatype\jackson-datatype-jdk8\2.10.0\jackson-datatype-jdk8-2.10.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\datatype\jackson-datatype-jsr310\2.10.0\jackson-datatype-jsr310-2.10.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\module\jackson-module-parameter-names\2.10.0\jackson-module-parameter-names-2.10.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-tomcat\2.2.1.RELEASE\spring-boot-starter-tomcat-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\tomcat\embed\tomcat-embed-core\9.0.27\tomcat-embed-core-9.0.27.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\tomcat\embed\tomcat-embed-el\9.0.27\tomcat-embed-el-9.0.27.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\tomcat\embed\tomcat-embed-websocket\9.0.27\tomcat-embed-websocket-9.0.27.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-validation\2.2.1.RELEASE\spring-boot-starter-validation-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\validation\jakarta.validation-api\2.0.1\jakarta.validation-api-2.0.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\hibernate\validator\hibernate-validator\6.0.18.Final\hibernate-validator-6.0.18.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-web\5.2.1.RELEASE\spring-web-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-webmvc\5.2.1.RELEASE\spring-webmvc-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-expression\5.2.1.RELEASE\spring-expression-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-devtools\2.2.1.RELEASE\spring-boot-devtools-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot\2.2.1.RELEASE\spring-boot-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-autoconfigure\2.2.1.RELEASE\spring-boot-autoconfigure-2.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\h2database\h2\1.4.200\h2-1.4.200.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\projectlombok\lombok\1.18.10\lombok-1.18.10.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\xml\bind\jakarta.xml.bind-api\2.3.2\jakarta.xml.bind-api-2.3.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-core\5.2.1.RELEASE\spring-core-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-jcl\5.2.1.RELEASE\spring-jcl-5.2.1.RELEASE.jar;D:\DevTools\ideaIU-2019.2.4.win\lib\idea_rt.jar" com.fcant.springbootdata.SpringBootDataApplication
Connected to the target VM, address: '127.0.0.1:52601', transport: 'socket'

  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::        (v2.2.1.RELEASE)

2019-12-04 23:19:30.168  INFO 19184 --- [  restartedMain] c.f.s.SpringBootDataApplication          : Starting SpringBootDataApplication on LAPTOP-CMGC7APE with PID 19184 (D:\Dome\SpringBootHD\SpringBootData\target\classes started by Fcant in D:\Dome\SpringBootHD)
2019-12-04 23:19:30.174  INFO 19184 --- [  restartedMain] c.f.s.SpringBootDataApplication          : No active profile set, falling back to default profiles: default
2019-12-04 23:19:30.234  INFO 19184 --- [  restartedMain] o.s.b.devtools.restart.ChangeableUrls    : The Class-Path manifest attribute in D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\glassfish\jaxb\jaxb-runtime\2.3.2\jaxb-runtime-2.3.2.jar referenced one or more files that do not exist: file:/D:/ProTools/apache-maven-3.6.1-bin/apache-maven-3.6.1/Repository/org/glassfish/jaxb/jaxb-runtime/2.3.2/jakarta.xml.bind-api-2.3.2.jar,file:/D:/ProTools/apache-maven-3.6.1-bin/apache-maven-3.6.1/Repository/org/glassfish/jaxb/jaxb-runtime/2.3.2/txw2-2.3.2.jar,file:/D:/ProTools/apache-maven-3.6.1-bin/apache-maven-3.6.1/Repository/org/glassfish/jaxb/jaxb-runtime/2.3.2/istack-commons-runtime-3.0.8.jar,file:/D:/ProTools/apache-maven-3.6.1-bin/apache-maven-3.6.1/Repository/org/glassfish/jaxb/jaxb-runtime/2.3.2/stax-ex-1.8.1.jar,file:/D:/ProTools/apache-maven-3.6.1-bin/apache-maven-3.6.1/Repository/org/glassfish/jaxb/jaxb-runtime/2.3.2/FastInfoset-1.2.16.jar,file:/D:/ProTools/apache-maven-3.6.1-bin/apache-maven-3.6.1/Repository/org/glassfish/jaxb/jaxb-runtime/2.3.2/jakarta.activation-api-1.2.1.jar
2019-12-04 23:19:30.235  INFO 19184 --- [  restartedMain] .e.DevToolsPropertyDefaultsPostProcessor : Devtools property defaults active! Set 'spring.devtools.add-properties' to 'false' to disable
2019-12-04 23:19:30.235  INFO 19184 --- [  restartedMain] .e.DevToolsPropertyDefaultsPostProcessor : For additional web related logging consider setting the 'logging.level.web' property to 'DEBUG'
2019-12-04 23:19:30.930  INFO 19184 --- [  restartedMain] .s.d.r.c.RepositoryConfigurationDelegate : Multiple Spring Data modules found, entering strict repository configuration mode!
2019-12-04 23:19:30.931  INFO 19184 --- [  restartedMain] .s.d.r.c.RepositoryConfigurationDelegate : Bootstrapping Spring Data repositories in DEFAULT mode.
2019-12-04 23:19:30.961  INFO 19184 --- [  restartedMain] .RepositoryConfigurationExtensionSupport : Spring Data JDBC - Could not safely identify store assignment for repository candidate interface com.fcant.springbootdata.repository.UserCartRepository. If you want this repository to be a JDBC repository, consider annotating your entities with one of these annotations: org.springframework.data.relational.core.mapping.Table.
2019-12-04 23:19:30.961  INFO 19184 --- [  restartedMain] .s.d.r.c.RepositoryConfigurationDelegate : Finished Spring Data repository scanning in 27ms. Found 0 repository interfaces.
2019-12-04 23:19:30.970  INFO 19184 --- [  restartedMain] .s.d.r.c.RepositoryConfigurationDelegate : Multiple Spring Data modules found, entering strict repository configuration mode!
2019-12-04 23:19:30.970  INFO 19184 --- [  restartedMain] .s.d.r.c.RepositoryConfigurationDelegate : Bootstrapping Spring Data repositories in DEFAULT mode.
2019-12-04 23:19:30.996  INFO 19184 --- [  restartedMain] .s.d.r.c.RepositoryConfigurationDelegate : Finished Spring Data repository scanning in 22ms. Found 1 repository interfaces.
2019-12-04 23:19:31.247  INFO 19184 --- [  restartedMain] trationDelegate$BeanPostProcessorChecker : Bean 'org.springframework.transaction.annotation.ProxyTransactionManagementConfiguration' of type [org.springframework.transaction.annotation.ProxyTransactionManagementConfiguration] is not eligible for getting processed by all BeanPostProcessors (for example: not eligible for auto-proxying)
2019-12-04 23:19:31.511  INFO 19184 --- [  restartedMain] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat initialized with port(s): 8090 (http)
2019-12-04 23:19:31.521  INFO 19184 --- [  restartedMain] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2019-12-04 23:19:31.522  INFO 19184 --- [  restartedMain] org.apache.catalina.core.StandardEngine  : Starting Servlet engine: [Apache Tomcat/9.0.27]
2019-12-04 23:19:31.630  INFO 19184 --- [  restartedMain] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2019-12-04 23:19:31.630  INFO 19184 --- [  restartedMain] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 1395 ms
2019-12-04 23:19:31.666  INFO 19184 --- [  restartedMain] com.zaxxer.hikari.HikariDataSource       : HikariPool-1 - Starting...
2019-12-04 23:19:31.778  INFO 19184 --- [  restartedMain] com.zaxxer.hikari.HikariDataSource       : HikariPool-1 - Start completed.
2019-12-04 23:19:31.782  INFO 19184 --- [  restartedMain] o.s.b.a.h2.H2ConsoleAutoConfiguration    : H2 console available at '/h2-console'. Database available at 'jdbc:mysql:///hls_train?serverTimezone=UTC'
2019-12-04 23:19:31.988  INFO 19184 --- [  restartedMain] o.hibernate.jpa.internal.util.LogHelper  : HHH000204: Processing PersistenceUnitInfo [name: default]
2019-12-04 23:19:32.047  INFO 19184 --- [  restartedMain] org.hibernate.Version                    : HHH000412: Hibernate Core {5.4.8.Final}
2019-12-04 23:19:32.154  INFO 19184 --- [  restartedMain] o.hibernate.annotations.common.Version   : HCANN000001: Hibernate Commons Annotations {5.1.0.Final}
2019-12-04 23:19:32.256  INFO 19184 --- [  restartedMain] org.hibernate.dialect.Dialect            : HHH000400: Using dialect: org.hibernate.dialect.MySQL57Dialect
2019-12-04 23:19:32.927  INFO 19184 --- [  restartedMain] o.h.e.t.j.p.i.JtaPlatformInitiator       : HHH000490: Using JtaPlatform implementation: [org.hibernate.engine.transaction.jta.platform.internal.NoJtaPlatform]
2019-12-04 23:19:32.940  INFO 19184 --- [  restartedMain] j.LocalContainerEntityManagerFactoryBean : Initialized JPA EntityManagerFactory for persistence unit 'default'
2019-12-04 23:19:33.220  WARN 19184 --- [  restartedMain] JpaBaseConfiguration$JpaWebConfiguration : spring.jpa.open-in-view is enabled by default. Therefore, database queries may be performed during view rendering. Explicitly configure spring.jpa.open-in-view to disable this warning
2019-12-04 23:19:33.345  INFO 19184 --- [  restartedMain] o.s.s.concurrent.ThreadPoolTaskExecutor  : Initializing ExecutorService 'applicationTaskExecutor'
2019-12-04 23:19:33.622  INFO 19184 --- [  restartedMain] o.s.b.d.a.OptionalLiveReloadServer       : LiveReload server is running on port 35729
2019-12-04 23:19:33.664  INFO 19184 --- [  restartedMain] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat started on port(s): 8090 (http) with context path ''
2019-12-04 23:19:33.666  INFO 19184 --- [  restartedMain] c.f.s.SpringBootDataApplication          : Started SpringBootDataApplication in 3.99 seconds (JVM running for 4.967)
2019-12-04 23:19:39.438  INFO 19184 --- [nio-8090-exec-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring DispatcherServlet 'dispatcherServlet'
2019-12-04 23:19:39.438  INFO 19184 --- [nio-8090-exec-1] o.s.web.servlet.DispatcherServlet        : Initializing Servlet 'dispatcherServlet'
2019-12-04 23:19:39.448  INFO 19184 --- [nio-8090-exec-1] o.s.web.servlet.DispatcherServlet        : Completed initialization in 10 ms
2019-12-04 23:19:39.650 ERROR 19184 --- [nio-8090-exec-1] o.hibernate.id.enhanced.TableStructure   : could not read a hi value

java.sql.SQLSyntaxErrorException: Table 'hls_train.hibernate_sequence' doesn't exist
	at com.mysql.cj.jdbc.exceptions.SQLError.createSQLException(SQLError.java:120) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.exceptions.SQLError.createSQLException(SQLError.java:97) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.exceptions.SQLExceptionsMapping.translateException(SQLExceptionsMapping.java:122) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.ClientPreparedStatement.executeInternal(ClientPreparedStatement.java:953) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.ClientPreparedStatement.executeQuery(ClientPreparedStatement.java:1003) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.zaxxer.hikari.pool.ProxyPreparedStatement.executeQuery(ProxyPreparedStatement.java:52) ~[HikariCP-3.4.1.jar:na]
	at com.zaxxer.hikari.pool.HikariProxyPreparedStatement.executeQuery(HikariProxyPreparedStatement.java) ~[HikariCP-3.4.1.jar:na]
	at org.hibernate.id.enhanced.TableStructure.executeQuery(TableStructure.java:216) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure.access$300(TableStructure.java:46) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure$1$1.execute(TableStructure.java:138) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure$1$1.execute(TableStructure.java:126) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.jdbc.WorkExecutor.executeReturningWork(WorkExecutor.java:55) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.jdbc.AbstractReturningWork.accept(AbstractReturningWork.java:34) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.resource.transaction.backend.jdbc.internal.JdbcIsolationDelegate.delegateWork(JdbcIsolationDelegate.java:57) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure$1.getNextValue(TableStructure.java:125) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.NoopOptimizer.generate(NoopOptimizer.java:40) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.SequenceStyleGenerator.generate(SequenceStyleGenerator.java:520) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.AbstractSaveEventListener.saveWithGeneratedId(AbstractSaveEventListener.java:120) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.saveTransientEntity(DefaultMergeEventListener.java:271) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.entityIsTransient(DefaultMergeEventListener.java:243) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.entityIsDetached(DefaultMergeEventListener.java:318) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.onMerge(DefaultMergeEventListener.java:172) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.onMerge(DefaultMergeEventListener.java:70) [hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.service.internal.EventListenerGroupImpl.fireEventOnEachListener(EventListenerGroupImpl.java:108) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.internal.SessionImpl.fireMerge(SessionImpl.java:775) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.internal.SessionImpl.merge(SessionImpl.java:762) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.orm.jpa.ExtendedEntityManagerCreator$ExtendedEntityManagerInvocationHandler.invoke(ExtendedEntityManagerCreator.java:368) ~[spring-orm-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at com.sun.proxy.$Proxy97.merge(Unknown Source) ~[na:na]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.orm.jpa.SharedEntityManagerCreator$SharedEntityManagerInvocationHandler.invoke(SharedEntityManagerCreator.java:314) ~[spring-orm-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at com.sun.proxy.$Proxy97.merge(Unknown Source) ~[na:na]
	at org.springframework.data.jpa.repository.support.SimpleJpaRepository.save(SimpleJpaRepository.java:557) ~[spring-data-jpa-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.data.repository.core.support.RepositoryComposition$RepositoryFragments.invoke(RepositoryComposition.java:371) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryComposition.invoke(RepositoryComposition.java:204) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryFactorySupport$ImplementationMethodExecutionInterceptor.invoke(RepositoryFactorySupport.java:657) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryFactorySupport$QueryExecutorMethodInterceptor.doInvoke(RepositoryFactorySupport.java:621) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryFactorySupport$QueryExecutorMethodInterceptor.invoke(RepositoryFactorySupport.java:605) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.data.projection.DefaultMethodInvokingMethodInterceptor.invoke(DefaultMethodInvokingMethodInterceptor.java:80) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:366) ~[spring-tx-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:99) ~[spring-tx-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.dao.support.PersistenceExceptionTranslationInterceptor.invoke(PersistenceExceptionTranslationInterceptor.java:139) ~[spring-tx-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.data.jpa.repository.support.CrudMethodMetadataPostProcessor$CrudMethodMetadataPopulatingMethodInterceptor.invoke(CrudMethodMetadataPostProcessor.java:178) ~[spring-data-jpa-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:93) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:212) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at com.sun.proxy.$Proxy105.save(Unknown Source) ~[na:na]
	at com.fcant.springbootdata.controller.UserCartController.create(UserCartController.java:45) ~[classes/:na]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.web.method.support.InvocableHandlerMethod.doInvoke(InvocableHandlerMethod.java:190) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.method.support.InvocableHandlerMethod.invokeForRequest(InvocableHandlerMethod.java:138) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.annotation.ServletInvocableHandlerMethod.invokeAndHandle(ServletInvocableHandlerMethod.java:106) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter.invokeHandlerMethod(RequestMappingHandlerAdapter.java:888) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter.handleInternal(RequestMappingHandlerAdapter.java:793) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.AbstractHandlerMethodAdapter.handle(AbstractHandlerMethodAdapter.java:87) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:1040) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.DispatcherServlet.doService(DispatcherServlet.java:943) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.FrameworkServlet.processRequest(FrameworkServlet.java:1006) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.FrameworkServlet.doPost(FrameworkServlet.java:909) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at javax.servlet.http.HttpServlet.service(HttpServlet.java:660) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.servlet.FrameworkServlet.service(FrameworkServlet.java:883) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at javax.servlet.http.HttpServlet.service(HttpServlet.java:741) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:231) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.tomcat.websocket.server.WsFilter.doFilter(WsFilter.java:53) ~[tomcat-embed-websocket-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.filter.RequestContextFilter.doFilterInternal(RequestContextFilter.java:100) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:119) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.filter.FormContentFilter.doFilterInternal(FormContentFilter.java:93) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:119) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.filter.CharacterEncodingFilter.doFilterInternal(CharacterEncodingFilter.java:201) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:119) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:202) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:96) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.authenticator.AuthenticatorBase.invoke(AuthenticatorBase.java:526) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:139) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:92) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:74) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.connector.CoyoteAdapter.service(CoyoteAdapter.java:343) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.coyote.http11.Http11Processor.service(Http11Processor.java:408) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.coyote.AbstractProcessorLight.process(AbstractProcessorLight.java:66) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.coyote.AbstractProtocol$ConnectionHandler.process(AbstractProtocol.java:861) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.tomcat.util.net.NioEndpoint$SocketProcessor.doRun(NioEndpoint.java:1579) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.tomcat.util.net.SocketProcessorBase.run(SocketProcessorBase.java:49) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) ~[na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) ~[na:1.8.0_192]
	at org.apache.tomcat.util.threads.TaskThread$WrappingRunnable.run(TaskThread.java:61) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at java.lang.Thread.run(Thread.java:748) ~[na:1.8.0_192]

2019-12-04 23:19:39.651  WARN 19184 --- [nio-8090-exec-1] o.h.engine.jdbc.spi.SqlExceptionHelper   : SQL Error: 1146, SQLState: 42S02
2019-12-04 23:19:39.651 ERROR 19184 --- [nio-8090-exec-1] o.h.engine.jdbc.spi.SqlExceptionHelper   : Table 'hls_train.hibernate_sequence' doesn't exist
2019-12-04 23:19:39.671 ERROR 19184 --- [nio-8090-exec-1] o.a.c.c.C.[.[.[/].[dispatcherServlet]    : Servlet.service() for servlet [dispatcherServlet] in context with path [] threw exception [Request processing failed; nested exception is org.springframework.dao.InvalidDataAccessResourceUsageException: error performing isolated work; SQL [n/a]; nested exception is org.hibernate.exception.SQLGrammarException: error performing isolated work] with root cause

java.sql.SQLSyntaxErrorException: Table 'hls_train.hibernate_sequence' doesn't exist
	at com.mysql.cj.jdbc.exceptions.SQLError.createSQLException(SQLError.java:120) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.exceptions.SQLError.createSQLException(SQLError.java:97) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.exceptions.SQLExceptionsMapping.translateException(SQLExceptionsMapping.java:122) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.ClientPreparedStatement.executeInternal(ClientPreparedStatement.java:953) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.mysql.cj.jdbc.ClientPreparedStatement.executeQuery(ClientPreparedStatement.java:1003) ~[mysql-connector-java-8.0.17.jar:8.0.17]
	at com.zaxxer.hikari.pool.ProxyPreparedStatement.executeQuery(ProxyPreparedStatement.java:52) ~[HikariCP-3.4.1.jar:na]
	at com.zaxxer.hikari.pool.HikariProxyPreparedStatement.executeQuery(HikariProxyPreparedStatement.java) ~[HikariCP-3.4.1.jar:na]
	at org.hibernate.id.enhanced.TableStructure.executeQuery(TableStructure.java:216) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure.access$300(TableStructure.java:46) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure$1$1.execute(TableStructure.java:138) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure$1$1.execute(TableStructure.java:126) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.jdbc.WorkExecutor.executeReturningWork(WorkExecutor.java:55) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.jdbc.AbstractReturningWork.accept(AbstractReturningWork.java:34) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.resource.transaction.backend.jdbc.internal.JdbcIsolationDelegate.delegateWork(JdbcIsolationDelegate.java:57) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.TableStructure$1.getNextValue(TableStructure.java:125) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.NoopOptimizer.generate(NoopOptimizer.java:40) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.id.enhanced.SequenceStyleGenerator.generate(SequenceStyleGenerator.java:520) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.AbstractSaveEventListener.saveWithGeneratedId(AbstractSaveEventListener.java:120) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.saveTransientEntity(DefaultMergeEventListener.java:271) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.entityIsTransient(DefaultMergeEventListener.java:243) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.entityIsDetached(DefaultMergeEventListener.java:318) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.onMerge(DefaultMergeEventListener.java:172) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.internal.DefaultMergeEventListener.onMerge(DefaultMergeEventListener.java:70) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.event.service.internal.EventListenerGroupImpl.fireEventOnEachListener(EventListenerGroupImpl.java:108) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.internal.SessionImpl.fireMerge(SessionImpl.java:775) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at org.hibernate.internal.SessionImpl.merge(SessionImpl.java:762) ~[hibernate-core-5.4.8.Final.jar:5.4.8.Final]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.orm.jpa.ExtendedEntityManagerCreator$ExtendedEntityManagerInvocationHandler.invoke(ExtendedEntityManagerCreator.java:368) ~[spring-orm-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at com.sun.proxy.$Proxy97.merge(Unknown Source) ~[na:na]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.orm.jpa.SharedEntityManagerCreator$SharedEntityManagerInvocationHandler.invoke(SharedEntityManagerCreator.java:314) ~[spring-orm-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at com.sun.proxy.$Proxy97.merge(Unknown Source) ~[na:na]
	at org.springframework.data.jpa.repository.support.SimpleJpaRepository.save(SimpleJpaRepository.java:557) ~[spring-data-jpa-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.data.repository.core.support.RepositoryComposition$RepositoryFragments.invoke(RepositoryComposition.java:371) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryComposition.invoke(RepositoryComposition.java:204) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryFactorySupport$ImplementationMethodExecutionInterceptor.invoke(RepositoryFactorySupport.java:657) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryFactorySupport$QueryExecutorMethodInterceptor.doInvoke(RepositoryFactorySupport.java:621) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.data.repository.core.support.RepositoryFactorySupport$QueryExecutorMethodInterceptor.invoke(RepositoryFactorySupport.java:605) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.data.projection.DefaultMethodInvokingMethodInterceptor.invoke(DefaultMethodInvokingMethodInterceptor.java:80) ~[spring-data-commons-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:366) ~[spring-tx-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:99) ~[spring-tx-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.dao.support.PersistenceExceptionTranslationInterceptor.invoke(PersistenceExceptionTranslationInterceptor.java:139) ~[spring-tx-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.data.jpa.repository.support.CrudMethodMetadataPostProcessor$CrudMethodMetadataPopulatingMethodInterceptor.invoke(CrudMethodMetadataPostProcessor.java:178) ~[spring-data-jpa-2.2.1.RELEASE.jar:2.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:93) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:186) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:212) ~[spring-aop-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at com.sun.proxy.$Proxy105.save(Unknown Source) ~[na:na]
	at com.fcant.springbootdata.controller.UserCartController.create(UserCartController.java:45) ~[classes/:na]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.web.method.support.InvocableHandlerMethod.doInvoke(InvocableHandlerMethod.java:190) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.method.support.InvocableHandlerMethod.invokeForRequest(InvocableHandlerMethod.java:138) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.annotation.ServletInvocableHandlerMethod.invokeAndHandle(ServletInvocableHandlerMethod.java:106) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter.invokeHandlerMethod(RequestMappingHandlerAdapter.java:888) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter.handleInternal(RequestMappingHandlerAdapter.java:793) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.mvc.method.AbstractHandlerMethodAdapter.handle(AbstractHandlerMethodAdapter.java:87) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:1040) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.DispatcherServlet.doService(DispatcherServlet.java:943) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.FrameworkServlet.processRequest(FrameworkServlet.java:1006) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.servlet.FrameworkServlet.doPost(FrameworkServlet.java:909) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at javax.servlet.http.HttpServlet.service(HttpServlet.java:660) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.servlet.FrameworkServlet.service(FrameworkServlet.java:883) ~[spring-webmvc-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at javax.servlet.http.HttpServlet.service(HttpServlet.java:741) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:231) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.tomcat.websocket.server.WsFilter.doFilter(WsFilter.java:53) ~[tomcat-embed-websocket-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.filter.RequestContextFilter.doFilterInternal(RequestContextFilter.java:100) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:119) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.filter.FormContentFilter.doFilterInternal(FormContentFilter.java:93) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:119) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.springframework.web.filter.CharacterEncodingFilter.doFilterInternal(CharacterEncodingFilter.java:201) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:119) ~[spring-web-5.2.1.RELEASE.jar:5.2.1.RELEASE]
	at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:193) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:166) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:202) ~[tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:96) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.authenticator.AuthenticatorBase.invoke(AuthenticatorBase.java:526) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:139) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:92) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:74) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.catalina.connector.CoyoteAdapter.service(CoyoteAdapter.java:343) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.coyote.http11.Http11Processor.service(Http11Processor.java:408) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.coyote.AbstractProcessorLight.process(AbstractProcessorLight.java:66) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.coyote.AbstractProtocol$ConnectionHandler.process(AbstractProtocol.java:861) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.tomcat.util.net.NioEndpoint$SocketProcessor.doRun(NioEndpoint.java:1579) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at org.apache.tomcat.util.net.SocketProcessorBase.run(SocketProcessorBase.java:49) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at org.apache.tomcat.util.threads.TaskThread$WrappingRunnable.run(TaskThread.java:61) [tomcat-embed-core-9.0.27.jar:9.0.27]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]
```
<a name="s6Qs1"></a>
# 3、错误分析
在使用Spring Data JPA时调用JpaRepository.save(Person)方法保存对象到mysql数据库中的时报错
<a name="LAdAX"></a>
# 4、解决方法
`@GeneratedValue`改为`@GeneratedValue(strategy=GenerationType.IDENTITY)`

```java
package com.fcant.springbootdata.bean;

import lombok.Data;
import org.hibernate.validator.constraints.Range;

import javax.persistence.*;
import javax.validation.constraints.NotBlank;

/**
 * UserCart
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 20:05 2019/12/4
 */
@Entity
@Data
@Table(name = "HLS_USER")
public class UserCart {
    @Id
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    private long id;
    @NotBlank
    @Column
    private String name;
    @NotBlank
    @Column
    private String account;
    @Column
    private String password;
    @Range(min = 1, max = 120)
    private Integer age;
}

```

