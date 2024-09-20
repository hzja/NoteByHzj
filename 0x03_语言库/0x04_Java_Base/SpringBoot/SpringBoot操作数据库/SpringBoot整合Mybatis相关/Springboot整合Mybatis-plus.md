Java SpringBoot Mybatis-plus
<a name="cJ6OS"></a>
## 一、添加依赖pom.xml
```xml
<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
</dependency>
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>druid-spring-boot-starter</artifactId>
  <version>1.1.20</version>
</dependency>
<dependency>
  <groupId>com.baomidou</groupId>
  <artifactId>mybatis-plus-boot-starter</artifactId>
  <version>3.3.2</version>
</dependency>
<!-- mybatis plus 代码生成器依赖 -->
<dependency>
  <groupId>com.baomidou</groupId>
  <artifactId>mybatis-plus-generator</artifactId>
  <version>3.3.2</version>
</dependency>
<!-- 代码生成器模板 -->
<dependency>
  <groupId>org.freemarker</groupId>
  <artifactId>freemarker</artifactId>
</dependency>
```
<a name="fEkvf"></a>
## 二、application.yml添加配置
```yaml
spring:
  #数据库配置
  datasource:
    url: jdbc:mysql://127.0.0.1:3306/user_role?useUnicode=true&useSSL=false&characterEncoding=utf-8
    username: root
    password: root
    # 使用druid数据源
    type: com.alibaba.druid.pool.DruidDataSource
    driver-class-name: com.mysql.jdbc.Driver
```
<a name="tjiQt"></a>
## 三、Application启动类配置`@MapperScan`
```java
@SpringBootApplication
@MapperScan("cn.com.vicente.demo.mapper")
public class BdDemoApplication {
    public static void main(String[] args) {
        SpringApplication.run(BdDemoApplication.class, args);
    }
}
```
到这里就引入了MyBatis-Plus了。
<a name="SqxT2"></a>
## 四、代码生成器
很多时候都不想写entity，mapper等文件，这个时候就可以使用代码生成器来自动生成对应的文件了。<br />需要修改几个地方：<br />1、数据库连接<br />2、文件需要放置的文件夹地址。<br />具体代码：
```java
import com.baomidou.mybatisplus.core.exceptions.MybatisPlusException;
import com.baomidou.mybatisplus.core.toolkit.StringPool;
import com.baomidou.mybatisplus.core.toolkit.StringUtils;
import com.baomidou.mybatisplus.generator.AutoGenerator;
import com.baomidou.mybatisplus.generator.InjectionConfig;
import com.baomidou.mybatisplus.generator.config.*;
import com.baomidou.mybatisplus.generator.config.po.TableInfo;
import com.baomidou.mybatisplus.generator.config.rules.NamingStrategy;
import com.baomidou.mybatisplus.generator.engine.FreemarkerTemplateEngine;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
```
演示例子，执行 main 方法控制台输入模块表名回车自动生成对应项目目录中
```java
public class CodeGenerator {
    /**
     * <p>
     * 读取控制台内容
     * </p>
     */
    public static String scanner(String tip) {
        Scanner scanner = new Scanner(System.in);
        StringBuilder help = new StringBuilder();
        help.append("请输入" + tip + "：");
        System.out.println(help.toString());
        if (scanner.hasNext()) {
            String ipt = scanner.next();
            if (StringUtils.isNotEmpty(ipt)) {
                return ipt;
            }
        }
        throw new MybatisPlusException("请输入正确的" + tip + "！");
    }
    public static void main(String[] args) {
        // 代码生成器
        AutoGenerator mpg = new AutoGenerator();
        // 全局配置
        GlobalConfig gc = new GlobalConfig();
        String projectPath = System.getProperty("user.dir");
        gc.setOutputDir(projectPath + "/src/main/java");
        gc.setAuthor("vicente");
        gc.setOpen(false);
        // service 命名方式
        gc.setServiceName("%sService");
        // service impl 命名方式
        gc.setServiceImplName("%sServiceImpl");
        // 自定义文件命名，注意 %s 会自动填充表实体属性！
        gc.setMapperName("%sMapper");
        gc.setXmlName("%sMapper");
        gc.setFileOverride(true);
        gc.setActiveRecord(true);
        // XML 二级缓存
        gc.setEnableCache(false);
        // XML ResultMap
        gc.setBaseResultMap(true);
        // XML columList
        gc.setBaseColumnList(false);
        // gc.setSwagger2(true); 实体属性 Swagger2 注解
        mpg.setGlobalConfig(gc);
        // 数据源配置
        DataSourceConfig dsc = new DataSourceConfig();
        dsc.setUrl("jdbc:mysql://127.0.0.1:3306/user_role?useUnicode=true&useSSL=false&characterEncoding=utf-8");
        // dsc.setSchemaName("public");
        dsc.setDriverName("com.mysql.jdbc.Driver");
        dsc.setUsername("root");
        dsc.setPassword("root");
        mpg.setDataSource(dsc);
        // 包配置
        PackageConfig pc = new PackageConfig();
        //pc.setModuleName(scanner("模块名"));
        pc.setParent("cn.com.vicente.demo");
        pc.setEntity("entity");
        pc.setService("service");
        pc.setServiceImpl("service.impl");
        mpg.setPackageInfo(pc);
        // 自定义配置
        InjectionConfig cfg = new InjectionConfig() {
            @Override
            public void initMap() {
                // to do nothing
            }
        };
        // 如果模板引擎是 freemarker
        String templatePath = "/templates/mapper.xml.ftl";
        // 如果模板引擎是 velocity
        // String templatePath = "/templates/mapper.xml.vm";
        // 自定义输出配置
        List<FileOutConfig> focList = new ArrayList<>();
        // 自定义配置会被优先输出
        focList.add(new FileOutConfig(templatePath) {
            @Override
            public String outputFile(TableInfo tableInfo) {
                // 自定义输出文件名 ， 如果 Entity 设置了前后缀、此处注意 xml 的名称会跟着发生变化！！
                String moduleName = pc.getModuleName()==null?"":pc.getModuleName();
                return projectPath + "/src/main/resources/mapper/" + moduleName
                    + "/" + tableInfo.getEntityName() + "Mapper" + StringPool.DOT_XML;
            }
        });
        /*
        cfg.setFileCreate(new IFileCreate() {
            @Override
            public boolean isCreate(ConfigBuilder configBuilder, FileType fileType, String filePath) {
                // 判断自定义文件夹是否需要创建
                checkDir("调用默认方法创建的目录");
                return false;
            }
        });
        */
        cfg.setFileOutConfigList(focList);
        mpg.setCfg(cfg);
        // 配置模板
        TemplateConfig templateConfig = new TemplateConfig();
        // 配置自定义输出模板
        //指定自定义模板路径，注意不要带上.ftl/.vm, 会根据使用的模板引擎自动识别
        // templateConfig.setEntity("templates/entity2.java");
        // templateConfig.setService();
        // templateConfig.setController();
        templateConfig.setXml(null);
        mpg.setTemplate(templateConfig);
        // 策略配置
        StrategyConfig strategy = new StrategyConfig();
        strategy.setNaming(NamingStrategy.underline_to_camel);
        strategy.setColumnNaming(NamingStrategy.underline_to_camel);
        //strategy.setSuperEntityClass("cn.com.bluemoon.demo.entity");
        strategy.setEntityLombokModel(true);
        strategy.setRestControllerStyle(true);
        // 公共父类
        //strategy.setSuperControllerClass("cn.com.bluemoon.demo.controller");
        // 写于父类中的公共字段
        //strategy.setSuperEntityColumns("id");
        strategy.setInclude(scanner("表名，多个英文逗号分割").split(","));
        strategy.setControllerMappingHyphenStyle(true);
        strategy.setTablePrefix(pc.getModuleName() + "_");
        mpg.setStrategy(strategy);
        mpg.setTemplateEngine(new FreemarkerTemplateEngine());
        mpg.execute();
    }
}
```
<a name="u1ApD"></a>
## 五、添加测试<br />
这里主要是MyBatis-Plus的CURD等方法。
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class SampleTest {
    private static Logger log = LoggerFactory.getLogger(SampleTest.class);
    @Autowired
    private MpUserService mpUserService;
    @Test
    public void test1() {
        // 插入新记录
        MpUser mpUser = new MpUser();
        //mpUser.setId(1L);
        mpUser.setEmail("test66@baomidou.com");
        mpUser.setAge(22);
        mpUser.setName("David Hong");
        mpUserService.save(mpUser);
        // 或者
        mpUser.insertOrUpdate();
        // 更新完成后，mpUser对象的id会被补全
        log.info("mpUser={}", mpUser.toString());
    }
    @Test
    public void test2() {
        // 通过主键id查询
        MpUser mpUser = mpUserService.getById(1);
        log.info("mpUser={}", mpUser.toString());
    }
    @Test
    public void test3() {
        // 条件查询，下面相当于xml中的 select * from mp_user where name = 'Tom' and age = '28' limit 1
        MpUser mpUser = mpUserService.getOne(new QueryWrapper<MpUser>().eq("name", "Tom").eq("age", "28").last("limit 1"));
        log.info("mpUser={}", mpUser.toString());
        // 批量查询
        List<MpUser> mpUserList = mpUserService.list();
        System.out.println("------------------------------all");
        mpUserList.forEach(System.out::println);
        // 分页查询 
        int pageNum = 1;
        int pageSize = 10;
        IPage<MpUser> mpUserIPage = mpUserService.page(new Page<>(pageNum, pageSize), new QueryWrapper<MpUser>().gt("age", "20"));
        // IPage to List
        List<MpUser> mpUserList1 = mpUserIPage.getRecords();
        System.out.println("------------------------------page");
        mpUserList1.forEach(System.out::println);
        // 总页数
        long allPageNum = mpUserIPage.getPages();
        System.out.println("------------------------------allPageNum");
        System.out.println(allPageNum);
    }
    @Test
    public void test4() {
        MpUser mpUser = mpUserService.getById(2);
        // 修改更新
        mpUser.setName("广东广州");
        //mpUserService.updateById(mpUser);
        // 或者
        mpUser.insertOrUpdate();
        // 通过主键id删除
        mpUserService.removeById(1);
        // 或者
        //mpUser.deleteById();
    }
}
```
<a name="W9QkY"></a>
## 六、数据分页
<a name="jV1JK"></a>
### 1、简单分页方法
```java
int pageNum = 1;
int pageSize = 10;
IPage<MpUser> mpUserIPage = mpUserService.page(new Page<>(pageNum, pageSize), new QueryWrapper<MpUser>().gt("age", "20"));
```
**上面的分页其实是调用BaseMapper的selectPage方法，这样的分页返回的数据确实是分页后的数据，但在控制台打印的SQL语句上看到其实并没有真正的物理分页，而是通过缓存来获得全部数据中再进行的分页，这样对于大数据量操作时是不可取的，那么接下来就叙述一下，真正实现物理分页的方法。**
<a name="3c0512cf"></a>
### 2、物理分页方法
新建一个MybatisPlusConfig配置类文件
```java
//Spring boot方式
@EnableTransactionManagement
@Configuration
@MapperScan("com.baomidou.cloud.service.*.mapper*")
public class MybatisPlusConfig {
    @Bean
    public PaginationInterceptor paginationInterceptor() {
        PaginationInterceptor paginationInterceptor = new PaginationInterceptor();
        // 设置请求的页面大于最大页后操作， true调回到首页，false 继续请求  默认false
        // paginationInterceptor.setOverflow(false);
        // 设置最大单页限制数量，默认 500 条，-1 不受限制
        // paginationInterceptor.setLimit(500);
        return paginationInterceptor;
    }
}
```
重新调用mpUserService.page可以看到数据有物理分页
<a name="3XIW8"></a>
### 3、XML自定义分页
<a name="Zfddk"></a>
#### UserMapper.java 方法内容
```java
public interface UserMapper{//可以继承或者不继承BaseMapper
    /**
     * <p>
     * 查询 : 根据state状态查询用户列表，分页显示
     * 注意!!: 如果入参是有多个,需要加注解指定参数名才能在xml中取值
     * </p>
     *
     * @param page 分页对象,xml中可以从里面进行取值,传递参数 Page 即自动分页,必须放在第一位(可以继承Page实现自己的分页对象)
     * @param state 状态
     * @return 分页对象
     */
    IPage<User> selectPageVo(Page page, @Param("age") Integer age);
}
```
<a name="JSfQs"></a>
#### UserMapper.xml
等同于编写一个普通 list 查询，MyBatis-plus 自动分页
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="cn.com.bluemoon.demo.mapper.MpUserMapper">
    <select id="selectPageVo" resultType="cn.com.bluemoon.demo.entity.MpUser">
      SELECT * FROM mp_user WHERE age=#{age}
    </select>
</mapper>
```
<a name="CHt84"></a>
#### UserServiceImpl.java 调用分页方法
```java
public IPage<User> selectUserPage(Page<User> page, Integer state) {
    // 不进行 count sql 优化，解决 MP 无法自动优化 SQL 问题，这时候需要自己查询 count 部分
    // page.setOptimizeCountSql(false);
    // 当 total 为小于 0 或者设置 setSearchCount(false) 分页插件不会进行 count 查询
    // 要点!! 分页返回的对象与传入的对象是同一个
    return baseMapper.selectPageVo(page, state);
}
```
<a name="STu5j"></a>
### 4、测试自定义方法
```java
@Test
public void test5() {
    Page<MpUser> mpUserPage = new Page<>(1,2);
    IPage<MpUser> iPage = mpUserService.selectUserPage(mpUserPage,22);
    System.out.println("总页数："+iPage.getPages());
    System.out.println("总记录数："+iPage.getTotal());
    List<MpUser> mpUserList1 = iPage.getRecords();
    mpUserList1.forEach(System.out::println);
}
```
<a name="v4StQ"></a>
## 七、打印sql日志
为了方便排查错误，很多时候需要打印MyBatis生成的sql语句，这时候就需要打印日志了。<br />在application.yml中添加：
```yaml
# Logger Config
logging:
  level:
    cn.com.vicente.demo: debug
```
或者
```yaml
mybatis-plus:
  configuration:
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl
```
<a name="4IC4s"></a>
## 八、逻辑删除
很多时候需要表的数据虽然删除了，但是还是希望不是真正删除数据，数据还是留在数据库中，只需要使用一个字段来做标志为即可，这时候就需要逻辑删除功能。<br />SpringBoot 配置方式：<br />application.yml 加入配置(如果默认值和mp默认的一样,该配置可无):
```yaml
mybatis-plus:
  global-config:
    db-config:
      logic-delete-value: 1 # 逻辑已删除值(默认为 1)
      logic-not-delete-value: 0 # 逻辑未删除值(默认为 0)
```
注册 Bean(3.1.1开始不再需要这一步)：
```java
import com.baomidou.mybatisplus.core.injector.ISqlInjector;
import com.baomidou.mybatisplus.extension.injector.LogicSqlInjector;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
@Configuration
public class MyBatisPlusConfiguration {
    @Bean
    public ISqlInjector sqlInjector() {
        return new LogicSqlInjector();
    }
}
```
实体类字段上加上`@TableLogic`注解
```java
@TableField(select = false)注解，可以不查询出deleted字段
@TableLogic
//@TableField(select = false)
private Integer deleted;
```
效果: 使用mp自带方法删除和查找都会附带逻辑删除功能 (自己写的xml不会)<br />example<br />删除时 `update user set deleted=1 where id =1 and deleted=0`<br />查找时 `select * from user where deleted=0`<br />附件说明<br />逻辑删除是为了方便数据恢复和保护数据本身价值等等的一种方案，但实际就是删除。<br />如果需要再查出来就不应使用逻辑删除，而是以一个状态去表示。
<a name="VBOa2"></a>
## 九、主键策略
MyBatis-plus 的主键生成的类型 默认类型 是 `IdType.ID_WORKER` 全局唯一ID，内容为空自动填充（默认配置），雪花算法
<a name="495f3086"></a>
### 1、局部主键策略实现
在实体类中 ID属性加注解
```java
@TableId(type = IdType.AUTO) 主键自增 数据库中需要设置主键自增
private Long id;
@TableId(type = IdType.NONE) 默认跟随全局策略走
private Long id;
@TableId(type = IdType.UUID) UUID类型主键
private Long id;
@TableId(type = IdType.ID_WORKER) 数值类型数据库中也必须是数值类型 否则会报错
private Long id;
@TableId(type = IdType.ID_WORKER_STR) 字符串类型   数据库也要保证一样字符类型
private Long id;
@TableId(type = IdType.INPUT) 用户自定义了  数据类型和数据库保持一致就行
private Long id;
```
<a name="CQGZ8"></a>
### 2、全局主键策略实现
需要在application.yml文件中添加
```yaml
mybatis-plus:
  global-config:
    db-config:
      id-type: uuid/none/input/id_worker/id_worker_str/auto
```
表示全局主键都采用该策略（如果全局策略和局部策略都有设置，局部策略优先级高）
<a name="lm99V"></a>
## 十、自动填充
很多时候表中都需要添加创建时间，创建人，修改时间，修改人来跟踪数据的来源和变动，但每次插入数据和修改数据的时候都要set这几个字段又感觉很麻烦，这个时候就系统系统能自动填充这几个字段了。<br />字段必须声明`@TableField`注解，属性fill选择对应策略，该申明告知 Mybatis-Plus 需要预留注入 SQL 字段
```java
@TableField(fill = FieldFill.INSERT)
private LocalDateTime createTime;
@TableField(fill = FieldFill.INSERT_UPDATE)
private LocalDateTime updateTime;
```
属性`fill`有四种对应策略，分别为：
```java
public enum FieldFill {
    /**
     * 默认不处理
     */
    DEFAULT,
    /**
     * 插入填充字段
     */
    INSERT,
    /**
     * 更新填充字段
     */
    UPDATE,
    /**
     * 插入和更新填充字段
     */
    INSERT_UPDATE
}
```
自定义实现类 MyMetaObjectHandler:
```java
@Component
public class MyMetaObjectHandler implements MetaObjectHandler {
    private static final Logger LOGGER = LoggerFactory.getLogger(MyMetaObjectHandler.class);
    @Override
    public void insertFill(MetaObject metaObject) {
        LOGGER.info("start insert fill ....");
        //this.setFieldValByName("createTime", LocalDateTime.now(), metaObject);
        this.setInsertFieldValByName("createTime", LocalDateTime.now(), metaObject);
        this.setInsertFieldValByName("updateTime", LocalDateTime.now(), metaObject);
    }
    @Override
    public void updateFill(MetaObject metaObject) {
        LOGGER.info("start update fill ....");
        this.setUpdateFieldValByName("updateTime", LocalDateTime.now(), metaObject);
    }
}
```
测试使用
```java
@Test
public void testInsert() {
    // 插入新记录
    MpUser mpUser = new MpUser();
    mpUser.setEmail("wm@baomidou.com");
    mpUser.setAge(28);
    mpUser.setName("王蒙");
    mpUserService.save(mpUser);
    log.info("mpUser={}", mpUser.toString());
}
@Test
public void testUpdate() {
    // 更新记录
    MpUser mpUser = new MpUser();
    mpUser.setId(1182478087497998337L);
    MpUser newUser = mpUser.selectById();
    System.out.println(mpUser == newUser);
    mpUser.setName("王天");
    mpUser.updateById();
    log.info("mpUser={}", mpUser.toString());
    log.info("newUser={}", newUser.toString());
}
```
自动填充优化<br />`insertFill`方法每次插入的时候都会调用，如果不存在createTime属性的话，每次插入都会白白调用了，浪费资源，所以可以判断是否存在该属性
```java
boolean hasCreateTime = metaObject.hasSetter(“createTime”);
if (hasCreateTime){
    this.setInsertFieldValByName(“createTime”, LocalDateTime.now(), metaObject);
}
```
希望，当更新时有设定时间，就用更新时设定的时间，当没有设定时就自动填充更新时间，可以这样设置
```java
Object fieldValue = getFieldValByName(“updateTime”, metaObject);
if (fieldValue == null){
    this.setUpdateFieldValByName(“updateTime”, LocalDateTime.now(), metaObject);
}
```
