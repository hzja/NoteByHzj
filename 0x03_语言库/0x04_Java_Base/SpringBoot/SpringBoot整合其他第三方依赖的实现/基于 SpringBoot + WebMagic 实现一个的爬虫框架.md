Java SpringBoot WebMagic<br />WebMagic是一个开源的java爬虫框架。WebMagic框架的具体如何使用请参考官方文档：[http://webmagic.io/docs/](http://webmagic.io/docs/)。<br />这里对spring boot+WebMagic+MyBatis做了整合，使用WebMagic爬取数据，然后通过MyBatis持久化爬取的数据到mysql数据库。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636616585749-8270299f-e000-4f58-bd79-1c938c8da2c8.webp#clientId=u47ad475e-531e-4&from=paste&id=u3442c202&originHeight=327&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u75314a1c-98e3-4119-b5c7-be3e3f0af12)
<a name="dEhQB"></a>
### 1、添加maven依赖
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>hyzx</groupId>
    <artifactId>qbasic-crawler</artifactId>
    <version>1.0.0</version>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>1.5.21.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>

    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.test.skip>true</maven.test.skip>
        <java.version>1.8</java.version>
        <maven.compiler.plugin.version>3.8.1</maven.compiler.plugin.version>
        <maven.resources.plugin.version>3.1.0</maven.resources.plugin.version>

        <mysql.connector.version>5.1.47</mysql.connector.version>
        <druid.spring.boot.starter.version>1.1.17</druid.spring.boot.starter.version>
        <mybatis.spring.boot.starter.version>1.3.4</mybatis.spring.boot.starter.version>
        <fastjson.version>1.2.58</fastjson.version>
        <commons.lang3.version>3.9</commons.lang3.version>
        <joda.time.version>2.10.2</joda.time.version>
        <webmagic.core.version>0.7.3</webmagic.core.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-devtools</artifactId>
            <scope>runtime</scope>
            <optional>true</optional>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-configuration-processor</artifactId>
            <optional>true</optional>
        </dependency>

        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>${mysql.connector.version}</version>
        </dependency>

        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid-spring-boot-starter</artifactId>
            <version>${druid.spring.boot.starter.version}</version>
        </dependency>

        <dependency>
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <version>${mybatis.spring.boot.starter.version}</version>
        </dependency>

        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>${fastjson.version}</version>
        </dependency>

        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>${commons.lang3.version}</version>
        </dependency>

        <dependency>
            <groupId>joda-time</groupId>
            <artifactId>joda-time</artifactId>
            <version>${joda.time.version}</version>
        </dependency>

        <dependency>
            <groupId>us.codecraft</groupId>
            <artifactId>webmagic-core</artifactId>
            <version>${webmagic.core.version}</version>
            <exclusions>
                <exclusion>
                    <groupId>org.slf4j</groupId>
                    <artifactId>slf4j-log4j12</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>${maven.compiler.plugin.version}</version>
                <configuration>
                    <source>${java.version}</source>
                    <target>${java.version}</target>
                    <encoding>${project.build.sourceEncoding}</encoding>
                </configuration>
            </plugin>

            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-resources-plugin</artifactId>
                <version>${maven.resources.plugin.version}</version>
                <configuration>
                    <encoding>${project.build.sourceEncoding}</encoding>
                </configuration>
            </plugin>

            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <configuration>
                    <fork>true</fork>
                    <addResources>true</addResources>
                </configuration>
                <executions>
                    <execution>
                        <goals>
                            <goal>repackage</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>

    <repositories>
        <repository>
            <id>public</id>
            <name>aliyun nexus</name>
            <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
            <releases>
                <enabled>true</enabled>
            </releases>
        </repository>
    </repositories>

    <pluginRepositories>
        <pluginRepository>
            <id>public</id>
            <name>aliyun nexus</name>
            <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
            <releases>
                <enabled>true</enabled>
            </releases>
            <snapshots>
                <enabled>false</enabled>
            </snapshots>
        </pluginRepository>
    </pluginRepositories>
</project>
```
<a name="kzEfs"></a>
### 2、项目配置文件 application.properties
配置mysql数据源，druid数据库连接池以及MyBatis的mapper文件的位置。
```
# mysql数据源配置
spring.datasource.name=mysql
spring.datasource.type=com.alibaba.druid.pool.DruidDataSource
spring.datasource.driver-class-name=com.mysql.jdbc.Driver
spring.datasource.url=jdbc:mysql://192.168.0.63:3306/gjhzjl?useUnicode=true&characterEncoding=utf8&useSSL=false&allowMultiQueries=true
spring.datasource.username=root
spring.datasource.password=root

# druid数据库连接池配置
spring.datasource.druid.initial-size=5
spring.datasource.druid.min-idle=5
spring.datasource.druid.max-active=10
spring.datasource.druid.max-wait=60000
spring.datasource.druid.validation-query=SELECT 1 FROM DUAL
spring.datasource.druid.test-on-borrow=false
spring.datasource.druid.test-on-return=false
spring.datasource.druid.test-while-idle=true
spring.datasource.druid.time-between-eviction-runs-millis=60000
spring.datasource.druid.min-evictable-idle-time-millis=300000
spring.datasource.druid.max-evictable-idle-time-millis=600000

# mybatis配置
mybatis.mapperLocations=classpath:mapper/**/*.xml
```
<a name="WEg0y"></a>
### 3、数据库表结构
```sql
CREATE TABLE `cms_content` (
  `contentId` varchar(40) NOT NULL COMMENT '内容ID',
  `title` varchar(150) NOT NULL COMMENT '标题',
  `content` longtext COMMENT '文章内容',
  `releaseDate` datetime NOT NULL COMMENT '发布日期',
  PRIMARY KEY (`contentId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='CMS内容表';
```
<a name="g4zCI"></a>
### 4、实体类
```java
import java.util.Date;

public class CmsContentPO {
    private String contentId;

    private String title;

    private String content;

    private Date releaseDate;

    public String getContentId() {
        return contentId;
    }

    public void setContentId(String contentId) {
        this.contentId = contentId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public Date getReleaseDate() {
        return releaseDate;
    }

    public void setReleaseDate(Date releaseDate) {
        this.releaseDate = releaseDate;
    }
}
```
<a name="bqesO"></a>
### 5、mapper接口
```java
public interface CrawlerMapper {
    int addCmsContent(CmsContentPO record);
}
```
<a name="g72fC"></a>
### 6、CrawlerMapper.xml文件
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.hyzx.qbasic.dao.CrawlerMapper">

    <insert id="addCmsContent" parameterType="com.hyzx.qbasic.model.CmsContentPO">
        insert into cms_content (contentId,
                                 title,
                                 releaseDate,
                                 content)
        values (#{contentId,jdbcType=VARCHAR},
                #{title,jdbcType=VARCHAR},
                #{releaseDate,jdbcType=TIMESTAMP},
                #{content,jdbcType=LONGVARCHAR})
    </insert>
</mapper>
```
<a name="jcrWG"></a>
### 7、知乎页面内容处理类ZhihuPageProcessor
主要用于解析爬取到的知乎html页面。
```java
@Component
public class ZhihuPageProcessor implements PageProcessor {

    private Site site = Site.me().setRetryTimes(3).setSleepTime(1000);

    @Override
    public void process(Page page) {
        page.addTargetRequests(page.getHtml().links().regex("https://www\\.zhihu\\.com/question/\\d+/answer/\\d+.*").all());
        page.putField("title", page.getHtml().xpath("//h1[@class='QuestionHeader-title']/text()").toString());
        page.putField("answer", page.getHtml().xpath("//div[@class='QuestionAnswer-content']/tidyText()").toString());
        if (page.getResultItems().get("title") == null) {
            // 如果是列表页，跳过此页，pipeline不进行后续处理
            page.setSkip(true);
        }
    }

    @Override
    public Site getSite() {
        return site;
    }
}
```
<a name="n4k7c"></a>
### 8、知乎数据处理类ZhihuPipeline
主要用于将知乎html页面解析出的数据存储到mysql数据库。
```java
@Component
public class ZhihuPipeline implements Pipeline {
    private static final Logger LOGGER = LoggerFactory.getLogger(ZhihuPipeline.class);

    @Autowired
    private CrawlerMapper crawlerMapper;

    public void process(ResultItems resultItems, Task task) {
        String title = resultItems.get("title");
        String answer = resultItems.get("answer");

        CmsContentPO contentPO = new CmsContentPO();
        contentPO.setContentId(UUID.randomUUID().toString());
        contentPO.setTitle(title);
        contentPO.setReleaseDate(new Date());
        contentPO.setContent(answer);

        try {
            boolean success = crawlerMapper.addCmsContent(contentPO) > 0;
            LOGGER.info("保存知乎文章成功：{}", title);
        } catch (Exception ex) {
            LOGGER.error("保存知乎文章失败", ex);
        }
    }
}
```
<a name="HVCnE"></a>
### 9、知乎爬虫任务类ZhihuTask
每十分钟启动一次爬虫。
```java
@Component
public class ZhihuTask {
    private static final Logger LOGGER = LoggerFactory.getLogger(ZhihuPipeline.class);

    @Autowired
    private ZhihuPipeline zhihuPipeline;

    @Autowired
    private ZhihuPageProcessor zhihuPageProcessor;

    private ScheduledExecutorService timer = Executors.newSingleThreadScheduledExecutor();

    public void crawl() {
        // 定时任务，每10分钟爬取一次
        timer.scheduleWithFixedDelay(() -> {
            Thread.currentThread().setName("zhihuCrawlerThread");

            try {
                Spider.create(zhihuPageProcessor)
                        // 从https://www.zhihu.com/explore开始抓
                        .addUrl("https://www.zhihu.com/explore")
                        // 抓取到的数据存数据库
                        .addPipeline(zhihuPipeline)
                        // 开启2个线程抓取
                        .thread(2)
                        // 异步启动爬虫
                        .start();
            } catch (Exception ex) {
                LOGGER.error("定时抓取知乎数据线程执行异常", ex);
            }
        }, 0, 10, TimeUnit.MINUTES);
    }
}
```
<a name="I0XUp"></a>
### 10、Spring boot程序启动类
```java
@SpringBootApplication
@MapperScan(basePackages = "com.hyzx.qbasic.dao")
public class Application implements CommandLineRunner {

    @Autowired
    private ZhihuTask zhihuTask;

    public static void main(String[] args) throws IOException {
        SpringApplication.run(Application.class, args);
    }

    @Override
    public void run(String... strings) throws Exception {
        // 爬取知乎数据
        zhihuTask.crawl();
    }
}
```
