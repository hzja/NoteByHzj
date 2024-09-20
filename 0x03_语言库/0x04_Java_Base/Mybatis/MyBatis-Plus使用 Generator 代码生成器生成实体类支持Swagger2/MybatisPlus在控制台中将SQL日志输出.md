JavaMybatis Plus<br />当使用MybatisPlus进行开发时，日志输出对于调试以及性能监测非常重要。MybatisPlus默认使用Log4j进行日志输出，但是如果项目不使用Log4j，可以通过配置`log-impl: org.apache.ibatis.logging.stdout.StdOutImpl`来将日志输出到控制台上。
<a name="HGvYz"></a>
## 1、在application.yml文件中添加MybatisPlus的配置文件
使用MybatisPlus自带的log-impl配置，可以在控制台打印出sql语句、执行结果的数据集、数据结果条数等详细信息，这种方法适合再调试的时候使用，因为这个展示的信息详细，更便于调试，查找问题进行优化。缺点就是如果执行的sql语句过多，则输出的日志就会很多。<br />首先，在application.yml或者application.properties中添加如下配置：
```yaml
mybatis-plus:
  config-location: classpath:mybatis/mybatis-config.xml
  mapper-locations: classpath*:mapper/**/*.xml
  type-aliases-package: com.example.entity
  global-config:
    db-config:
      id-type: auto
    banner: false
    sql-parser-cache: false
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl #开启sql日志
```
其中，log-impl指定了日志输出的实现类为`org.apache.ibatis.logging.stdout.StdOutImpl`，即控制台输出。在日志输出时，可以看到如下输出信息：
```
2023-04-12 23:03:27.720  INFO 1126 --- [nio-8019-exec-1] c.b.p.controller.HeroController          : ==>  Preparing: SELECT * FROM hero WHERE id = ? 
2023-04-12 23:03:27.721  INFO 1126 --- [nio-8019-exec-1] c.b.p.controller.HeroController          : ==> Parameters: 1(Integer)
2023-04-12 23:03:27.724  INFO 1126 --- [nio-8019-exec-1] c.b.p.controller.HeroController          : <==      Total: 1
```
可以看到，MybatisPlus将SQL语句和参数都输出到了控制台。<br />下面是一个完整的Controller示例代码，供参考：
```java
@RestController
@RequestMapping("/hero")
public class HeroController {

    @Autowired
    private HeroService heroService;

    @GetMapping("/{id}")
    public Hero getById(@PathVariable Integer id) {
        return heroService.getById(id);
    }
}
```
其中，HeroService中调用了MybatisPlus的`getById`方法：
```java
@Service
public class HeroServiceImpl extends ServiceImpl<HeroMapper, Hero> implements HeroService {

    @Override
    public Hero getById(Serializable id) {
        log.info("getById - id = {}", id);
        return super.getById(id);
    }
}
```
在getById方法中，加入了自定义的日志输出信息，以便日后调试。同时，配置了log-impl为控制台输出，可以方便地在控制台查看到SQL日志。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700099290473-2c86fdb4-f488-499b-83c2-547d321c3acc.png#averageHue=%232d2d2c&clientId=u3618e241-8e52-4&from=paste&id=u8b73f9a6&originHeight=264&originWidth=955&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u774ec98d-6a2b-424b-97aa-4336f76083b&title=)
<a name="sEJqE"></a>
## 2、在application.yml文件中使用log4j日志框架配置
使用这个方法可以再控制台或者日志文件中打印sql语句，这种方法比较适合再生产环境种使用，可以避免输出过多的无用信息，也可以使用日志级别来控制是否打印sql语句。com.example.classroomrealtimefeedback.mapper是mapper层的包名
```yaml
logging:
  level:
    com.example.classroomrealtimefeedback.mapper: debug
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700099311655-026de518-ecfa-4daf-b2ec-93d73f637f1f.png#averageHue=%23323131&clientId=u3618e241-8e52-4&from=paste&id=ua12378b1&originHeight=96&originWidth=960&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5a21a0e4-6772-4775-b13d-f7bba95f2c2&title=)
<a name="xnJfm"></a>
## 3 、使用P6spy插件
可以再控制台中打印出sql语句，并且再控制台中将输出的sql中的？部分替换位真实运行的值，这种方法适合需要复制sql语句到数据库工具中直接执行的场景，也可以通过spyproperties文件来配置是否开启慢sql记录、慢sql记录标准的参数。该插件有性能损耗，不建议生产环境使用。<br />引用依赖：
```xml
<dependency>
  <groupId>p6spy</groupId>
  <artifactId>p6spy</artifactId>
  <version>3.9.1</version>
</dependency>
```
修改.yml配置文件：这里的配置是将MySQL数据库和这个插件的配置合一起了。
```yaml
spring:
 datasource:
   driver-class-name: com.p6spy.engine.spy.P6SpyDriver
   url: jdbc:p6spy:mysql://127.0.0.1:3306/qcby?rewriteBatchedStatements=true&useUnicode=true&characterEncoding=utf-8
#mysql为数据库类型，
```
后面为数据库连接地址，配置spy.properties文件：
```properties
modulelist=com.baomidou.mybatisplus.extension.p6spy.MybatisPlusLogFactory,com.p6spy.engine.outage.P6OutageFactory
logMessageFormat=com.baomidou.mybatisplus.extension.p6spy.P6SpyLogger
appender=com.baomidou.mybatisplus.extension.p6spy.StdoutLogger
deregisterdrivers=true
useprefix=true
dateformat=yyyy-MM-dd HH:mm:ss
# 实际驱动可多个
#driverlist=org.h2.Driver
# 是否开启慢SQL记录
outagedetection=true
# 慢SQL记录标准 1 秒
outagedetectioninterval=1
```
输出结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700099311698-c3ba3f00-ee14-4298-b13e-989c60c352c2.png#averageHue=%23372e2d&clientId=u3618e241-8e52-4&from=paste&id=u306da2a3&originHeight=90&originWidth=932&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u354273cc-9876-401f-ac7a-64471216d40&title=)<br />可能出现的问题：

- 打印出sql为null，在`excludecategories`增加commit
- 批量操作不打印sql，去除`excludecategories`中的batch
<a name="BxUWD"></a>
## 参考
MybatisPlus官方文档：[https://baomidou.com](https://baomidou.com)
