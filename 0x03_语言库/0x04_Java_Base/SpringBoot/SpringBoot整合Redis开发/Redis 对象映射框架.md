Java Spring Redis<br />Redis 官方博客宣布推出四个用于 Redis 的客户端工具库：Redis OM（对象映射库，Object Mapping），目标是让应用程序尽可能地容易使用 Redis 和 Redis 模块。<br />四个客户端工具库分别对应四种编程语言

- **Redis OM for .NET：**.Net 平台的 Redis OM，依赖 StackExchange.Redis 实现。借助 Redis OM .NET 可以实现对象操作的方式操作 Redis 数据，脱离 key/value 的操作方式。查询支持大部分.Neter 最爱的 LINQ。
- **Redis OM for Node.js：** 为 TypeScript 和 JavaScript 提供一级支持
- **Redis OM for Python：** 原生集成流行的 FastAPI 框架，将 FastAPI 与 Redis 结合可构建高性能 Web 服务。Redis OM Python 库还支持同步和异步使用
- **Redis OM for Spring：** 原生集成 Spring，扩展了 Spring Data Redis（提供熟悉的界面），添加了对 RedisBloo 的部分支持。

换言之，在之后的开发中，开发者可以通过 Redis OM 库直观地将域对象 (domain objects) 保存在 Redis，然后使用流畅的、以语言为中心的 API 进行查询。
<a name="HAHzc"></a>
## Redis OM for Spring
作为一枚Java开发，这里重点了解一下 Redis OM for Spring。
<a name="aREvn"></a>
### 简介
Redis OM for Spring提供了强大的存储库和基于强大的 Spring Data Redis (SDR) 框架构建的自定义对象映射抽象。

- `@Document` 将 Spring Data 模型映射到 Redis JSON 文档的注释
- `@RedisHash`通过以下方式增强 SDR `@EnableRedisEnhancedRepositories`：
   - 使用 Redis 的原生搜索引擎 (RediSearch) 进行二级索引
   - 将ULID用于带`@Id`注释的字段
- `RedisDocumentRepository` 自动实现存储库接口以实现复杂的查询功能，使用 `@EnableRedisDocumentRepositories`
- 声明性搜索索引通过 `@Indexable`
- 全文检索索引通过 `@Searchable`
- `@Bloom` 注释可以非常快速地确定一个值是否在集合中。
<a name="WYI6s"></a>
### 实践
通过案例来了解一下Redis OM for Spring的具体用法
<a name="QAfqI"></a>
#### 相关依赖
目前快照地址
```xml
<repositories>
	<repository>
		<id>snapshots-repo</id>
		<url>https://s01.oss.sonatype.org/content/repositories/snapshots/</url>
	</repository>
</repositories>
```
pom文件
```xml
<dependency>
  <groupId>com.redis.om</groupId>
  <artifactId>redis-om-spring</artifactId>
  <version>${version}</version>
</dependency>
```
<a name="GlVaa"></a>
#### SpringBoot配置
```java
package com.redis.om.documents;

import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.geo.Point;

import com.redis.om.documents.domain.Company;
import com.redis.om.documents.repositories.CompanyRepository;

@SpringBootApplication
@Configuration
@EnableRedisDocumentRepositories(basePackages = "com.redis.om.documents.*")
public class RomsDocumentsApplication {

  @Autowired
  CompanyRepository companyRepo;

  @Bean
  CommandLineRunner loadTestData() {
    return args -> {
      companyRepo.deleteAll();

      // 创建两组实体域 redis & microsoft
      Company redis = Company.of(
        "Redis", "https://redis.com", new Point(-122.066540, 37.377690), 526, 2011 
      );
      redis.setTags(Set.of("fast", "scalable", "reliable"));

      Company microsoft = Company.of(
        "Microsoft", "https://microsoft.com", new Point(-122.124500, 47.640160), 182268, 1975 
      );
      microsoft.setTags(Set.of("innovative", "reliable"));

      // 将创建的两组实体域持久化
      companyRepo.save(redis);
      companyRepo.save(microsoft);
    };
  }

  public static void main(String[] args) {
    SpringApplication.run(RomsDocumentsApplication.class, args);
  }
}
```
整体使用上非常清晰，重点是类上开启`@EnableRedisDocumentRepositories`注解，就可以注入可用于 CRUD 操作和自定义查询的`@Document`存储库 bean。
<a name="dThHG"></a>
#### 实体对象映射
这个使用SpringBoot非常熟悉，Redis OM Spring 也提供`@Document`注释来将模型保存为 JSON 文档
```java
package com.redis.om.documents.domain;

import java.util.HashSet;
import java.util.Set;
import org.springframework.data.annotation.Id;
import org.springframework.data.geo.Point;
import com.redis.om.spring.annotations.Document;
import com.redis.om.spring.annotations.Searchable;
import lombok.*;

@Data
@RequiredArgsConstructor(staticName = "of")
@AllArgsConstructor(access = AccessLevel.PROTECTED)
@Document
public class Company {
	@Id private String id;
	@Searchable private String name;
	@Indexed private Point location;
	@Indexed private Set<String> tags = new HashSet<String>();
	@Indexed private Integer numberOfEmployees;
	@Indexed private Integer yearFounded;
	private String url;
	private boolean publiclyListed;
	
	// ...
}
```
添加`@Document`注释即可，定义格式省去了自己做转换，Redis OM 库直观地将域对象 (domain objects) 保存在 Redis上。
<a name="vhbmH"></a>
#### 接口使用
可以使用流畅的、以语言为中心的 API 进行查询，更符合平常的编写习惯，看一组官方给的实现案例：
```java
package com.redis.om.documents.repositories;

import java.util.*;

import org.springframework.data.geo.Distance;
import org.springframework.data.geo.Point;
import org.springframework.data.repository.query.Param;

import com.redis.om.documents.domain.Company;
import com.redis.om.spring.annotations.Query;
import com.redis.om.spring.repository.RedisDocumentRepository;

public interface CompanyRepository extends RedisDocumentRepository<Company, String> {
	// find one by property
	Optional<Company> findOneByName(String name);
	
	// geospatial query
	Iterable<Company> findByLocationNear(Point point, Distance distance);
	
	// find by tag field, using JRediSearch "native" annotation
	@Query("@tags:{$tags}")
	Iterable<Company> findByTags(@Param("tags") Set<String> tags);
	
	// find by numeric property
	Iterable<Company> findByNumberOfEmployees(int noe);
	
	// find by numeric property range
	Iterable<Company> findByNumberOfEmployeesBetween(int noeGT, int noeLT);
	
	// starting with/ending with
	Iterable<Company> findByNameStartingWith(String prefix);
}
```
<a name="RoXI9"></a>
## 总结
整体上，Redis OM Spring节省了很多项目中整合Redis的步骤，可以更好的面向对象编程，省去不少数据格式的转换，同时也提供了更熟悉的API接口，大大的赞，不过这轮操作下来，Redis更像一个数据库了。<br />注意：Redis OM Spring 目前仅适用于 Jedis，同时 Redis OM 的一些高级特性依赖于两个可用 Redis 模块的核心特性：RediSearch和RedisJSON。<br />地址：[https://github.com/redis/redis-om-spring](https://github.com/redis/redis-om-spring)
