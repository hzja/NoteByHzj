IDEA IDEA Plugins Mybatis<br />**使用IDEA Plugins-Easy Code生成数据表对应的实体类、Dao层、Mapper映射文件、Service层以及Controller层**
<a name="SyHnU"></a>
## 1、在IDEA安装Easy Code插件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597415292329-7cf197c2-487b-49ed-9f58-3805b7204e1b.png#averageHue=%23d4aa6d&height=585&id=WakEm&originHeight=1756&originWidth=2368&originalType=binary&ratio=1&rotation=0&showTitle=false&size=318712&status=done&style=shadow&title=&width=789.3333333333334)
<a name="Nqt64"></a>
## 2、在IDEA中使用DateBase连接数据库
参考<br />[通过IDEA连接数据表生成Java POJO对象](https://www.yuque.com/fcant/java/fqlhgh?inner=ZviG4&view=doc_embed)
<a name="Fz6xV"></a>
## 3、生成逆向工程的代码
<a name="yvxNw"></a>
### 选择逆向的表结构
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597415549459-7a090247-4549-4366-be9f-6dd1753a664c.png#averageHue=%23f5f4f4&height=687&id=A11wC&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=798568&status=done&style=shadow&title=&width=1280)
<a name="9BNGZ"></a>
### 配置逆向生成的路径
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597415876231-eb03bb80-fbd8-4e1e-a94e-1f03bd78301e.png#averageHue=%23f6f6f5&height=687&id=JXiJc&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=888608&status=done&style=shadow&title=&width=1280)
<a name="FNDZu"></a>
### 提示创建不存在的包路径
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597415965124-c6ba349f-592f-4dfd-b7b5-bc0e881b0123.png#averageHue=%23f6f5f5&height=687&id=U3IDx&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=813612&status=done&style=shadow&title=&width=1280)
<a name="UXqTH"></a>
### 逆向生成的分层
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597416215327-a1c12684-1d9f-4657-b8d5-3c804216c323.png#averageHue=%23f8f7f6&height=687&id=V3EGA&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=878662&status=done&style=shadow&title=&width=1280)
<a name="Tavo9"></a>
### Dao层添加 `@Mapper` 注解
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597416344071-a310bddb-26ae-42e4-98a3-4c0e67319235.png#averageHue=%23f8f7f6&height=687&id=rsEhf&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=848577&status=done&style=shadow&title=&width=1280)
<a name="TmXu3"></a>
### 启动类添加 `@MapperScan` 注解进行Dao层接口扫描
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597416479471-f4b90eec-0d9d-4fe1-9273-e18db6cb4b8d.png#averageHue=%23f9f8f7&height=687&id=ZXnq6&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=643616&status=done&style=shadow&title=&width=1280)
<a name="UcKLi"></a>
### 在配置文件配置数据源信息-此处使用阿里的Druid数据源管理
<a name="pVhLw"></a>
#### Druid的依赖
```xml
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>druid-spring-boot-starter</artifactId>
  <version>1.1.13</version>
</dependency>
```
<a name="tVNUX"></a>
#### 数据源配置
```yaml
mysql:
  url: jdbc:mysql:///test?serverTimezone=UTC
  username: root
  password: 123456
  driverClassName: com.mysql.cj.jdbc.Driver
spring:
  application:
    name: ServiceActiviti
  datasource:
    type: com.alibaba.druid.pool.DruidDataSource
    url: ${mysql.url}
    driver-class-name: ${mysql.driverClassName}
    username: ${mysql.username}
    password: ${mysql.password}
    druid: # #
      url: ${mysql.url}
      username: ${mysql.username}
      password: ${mysql.password}
      driver-class-name: ${mysql.driverClassName}
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
mybatis:
  mapper-locations: classpath:/mapper/*Dao.xml
  typeAliasesPackage: com.vue.demo.entity

```
<a name="2ff01983"></a>
### 启动项目，使用HTTP Client成功访问接口
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597417298057-9bc7a68f-2f76-4a0b-88b6-a4cf443b25b5.png#averageHue=%23faf9f8&height=687&id=ljkRn&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=689207&status=done&style=shadow&title=&width=1280)
