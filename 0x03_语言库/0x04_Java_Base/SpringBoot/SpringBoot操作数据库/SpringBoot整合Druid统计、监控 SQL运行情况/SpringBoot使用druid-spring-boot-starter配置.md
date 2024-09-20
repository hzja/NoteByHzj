Java SpringBoot Druid
<a name="Giy69"></a>
# 1、引入依赖
```xml
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>druid-spring-boot-starter</artifactId>
  <version>1.1.13</version>
</dependency>
```
<a name="a26c2bfb"></a>
# 2、YAML配置
```yaml
spring:
   datasource:
      driverClassName: com.mysql.cj.jdbc.Driver
      type: com.alibaba.druid.pool.DruidDataSource
      druid: # #
      	 url: ${mysql.url}?${jdbcUrlArgs}
         username: ${mysql.username}
         password: ${mysql.password}
         initial-size: 10
         max-active: 200
         min-idle: 10
         max-wait: 60000
         pool-prepared-statements: false
         validation-query: SELECT 1 FROM DUAL
         test-on-borrow: false
         test-on-return: false
         test-while-idle: true
         time-between-eviction-runs-millis: 60000
         min-evictable-idle-time-millis: 30000
         max-pool-prepared-statement-per-connection-size: 20
         aop-patterns: com.msyyt.crm.activity.*
         filter: # 状态监控
            stat:
               enabled: true
               db-type: mysql
               log-slow-sql: true
               slow-sql-millis: 2000
         web-stat-filter: # 监控过滤器
            enabled: true #是否启用 默认true
            exclusions:
            - '*.js'
            - '*.gif'
            - '*.jpg'
            - '*.png'
            - '*.css'
            - '*.ico'
            - /druid/*
         stat-view-servlet: # druid 监控页面
            enabled: true
            url-pattern: /druid/*
            reset-enable: false
            allow:  # 白名单
            deny:   # 黑名单
            login-username: admin
            login-password:  admin
```

