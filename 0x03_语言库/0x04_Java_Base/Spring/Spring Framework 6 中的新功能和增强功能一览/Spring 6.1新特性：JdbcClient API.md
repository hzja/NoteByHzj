Java
<a name="wba7Q"></a>
## 一、前言
Spring Framework 6.1 引入了新的 JdbcClient API，它是在 JdbcTemplate 之上的一个包装器，用于使用 fluent API 执行数据库操作。Spring Boot 3.2 将包含 Spring Framework 6.1，快速了解如何使用 JdbcClient 以简化的方式实现各种数据库操作。首先，前往 [https://start.spring.io/](https://start.spring.io/)，通过选择 Spring JDBC、MySQL 驱动和 Testcontainers starters 来创建一个 Spring Boot 应用程序。注意：在撰写本文时，Spring Boot 3.2.0-M2 已发布，因此将选择 3.2.0（M2）作为 Spring Boot 版本。
<a name="E6LAl"></a>
## 二、创建书签实体
创建一个 Bookmark 的 Java record 实体（java16+），如下所示：
```java
import java.time.Instant;

public record Bookmark(Long id, String title, String url, Instant createdAt) {

}
```
<a name="q1TFM"></a>
## 三、数据库脚本
Bookmark 实体对于应的 MySQL 数据库表。
```sql
CREATE TABLE `bookmarks` (
  `id` bigint NOT NULL AUTO_INCREMENT COMMENT '主键id',
  `title` varchar(255) NOT NULL COMMENT '标题',
  `url` varchar(255) NOT NULL COMMENT 'url地址',
  `created_at` timestamp NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB COMMENT='书签表';
```
<a name="kbOP4"></a>
## 四、使用 JdbcClient 实现 CRUD
使用 JdbcClient API 实现对 Bookmark 的 CRUD 操作。
```java
@Repository
@Transactional(readOnly = true)
public class BookmarkRepository {
    private final JdbcClient jdbcClient;

    public BookmarkRepository(JdbcClient jdbcClient) {
        this.jdbcClient = jdbcClient;
    }
    ...
    ...
    ...
}
```
<a name="VrnO7"></a>
### 4.1 查找所有书签
可以使用 JdbcClient 来获取所有书签，代码示例如下：
```java
public List<Bookmark> findAll() {
    String sql = "select id, title, url, created_at from bookmarks";
    return jdbcClient.sql(sql).query(Bookmark.class).list();
}
```
使用 JdbcClient API，它会通过使用 `SimplePropertyRowMapper` 来动态创建 `RowMapper`。它将执行将 bean 属性名映射到表列名的操作，将驼峰命名法转换为下划线表示法。如果需要更多对映射的控制，您可以自己创建一个 `RowMapper`，并按以下方式使用：
```java
public List<Bookmark> findAll() {
    String sql = "select id, title, url, created_at from bookmarks";
    return jdbcClient.sql(sql).query(new BookmarkRowMapper()).list();
}

static class BookmarkRowMapper implements RowMapper<Bookmark> {
    @Override
    public Bookmark mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new Bookmark(
                rs.getLong("id"),
                rs.getString("title"),
                rs.getString("url"),
                rs.getTimestamp("created_at").toInstant()
        );
    }
}
```
<a name="bVLYr"></a>
### 4.2 根据 ID 查找书签
可以使用 JdbcClient 按 id 获取一个书签，代码示例如下：
```java
public Optional<Bookmark> findById(Long id) {
    String sql = "select id, title, url, created_at from bookmarks where id = :id";
    return jdbcClient.sql(sql).param("id", id).query(Bookmark.class).optional();
    
    // 如果使用自己的 RowMapper，可以按以下方式进行操作：
    //return jdbcClient.sql(sql).param("id", id).query(new BookmarkRowMapper()).optional();
}
```
<a name="bnscL"></a>
### 4.3 新建一个新书签
可以使用 MySQL 的 `INSERT INTO ... RETURNING COL1, COL2`语法，然后使用 KeyHolder 获取生成的主键值。因此，可以将新行插入到 bookmarks 表中，并按如下方式获取生成的主键值：
```java
@Transactional
public Long save(Bookmark bookmark) {
    String sql = "insert into bookmarks(title, url, created_at) values(:title,:url,:createdAt) returning id";
    KeyHolder keyHolder = new GeneratedKeyHolder();
    jdbcClient.sql(sql)
                .param("title", bookmark.title())
                .param("url", bookmark.url())
                .param("createdAt", Timestamp.from(bookmark.createdAt()))
                .update(keyHolder);
    return keyHolder.getKeyAs(Long.class);
}
```
<a name="aTugO"></a>
### 4.4 更新书签
可以按如下方式更新一个书签：
```java
@Transactional
public void update(Bookmark bookmark) {
    String sql = "update bookmarks set title = :title, url = :url where id = :id";
    int count = jdbcClient.sql(sql)
            .param("title", bookmark.title())
            .param("url", bookmark.url())
            .param("id", bookmark.id())
            .update();
    if (count == 0) {
        throw new RuntimeException("Bookmark not found");
    }
}
```
<a name="Og1q8"></a>
### 4.5 删除书签
可以按如下方式删除一个书签：
```java
@Transactional
public void delete(Long id) {
    String sql = "delete from bookmarks where id = ?";
    int count = jdbcClient.sql(sql).param(1, id).update();
    if (count == 0) {
        throw new RuntimeException("Bookmark not found");
    }
}
```
<a name="Ez9OL"></a>
## 五、使用 Testcontainers 测试
应该始终确保数据库处于已知状态，以便可以编写可预测的断言。因此，创建一个名为src/test/resources/test_data.sql 的 sql 文件，并包含以下内容：
```sql
TRUNCATE TABLE bookmarks;

INSERT INTO bookmarks(title, url, created_at) VALUES
('How (not) to ask for Technical Help?','https://sivalabs.in/how-to-not-to-ask-for-technical-help', CURRENT_TIMESTAMP),
('Getting Started with Kubernetes','https://sivalabs.in/getting-started-with-kubernetes', CURRENT_TIMESTAMP),
('Few Things I learned in the HardWay in 15 years of my career','https://sivalabs.in/few-things-i-learned-the-hardway-in-15-years-of-my-career', CURRENT_TIMESTAMP),
('All the resources you ever need as a Java & Spring application developer','https://sivalabs.in/all-the-resources-you-ever-need-as-a-java-spring-application-developer', CURRENT_TIMESTAMP),
('SpringBoot Integration Testing using Testcontainers Starter','https://sivalabs.in/spring-boot-integration-testing-using-testcontainers-starter', CURRENT_TIMESTAMP),
('Testing SpringBoot Applications','https://sivalabs.in/spring-boot-testing', CURRENT_TIMESTAMP)
;
```
现在，可以在测试类中添加注解 `@Sql("/test-data.sql")`，这样在运行每个测试之前，指定的SQL 脚本将被执行。使用 Testcontainers 来测试 `BookmarkRepository`，代码如下所示：
```java
package com.sivalabs.bookmarks.domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.ImportAutoConfiguration;
import org.springframework.boot.autoconfigure.jdbc.JdbcClientAutoConfiguration;
import org.springframework.boot.test.autoconfigure.jdbc.JdbcTest;
import org.springframework.jdbc.core.simple.JdbcClient;
import org.springframework.test.context.jdbc.Sql;

import java.time.Instant;
import java.util.List;
import java.util.Optional;

import static org.assertj.core.api.Assertions.assertThat;

@JdbcTest(properties = {
   "spring.test.database.replace=none",
   "spring.datasource.url=jdbc:tc:mysql:5.7.34:///db"
})
@ImportAutoConfiguration(JdbcClientAutoConfiguration.class)
@Sql("/test-data.sql")
class BookmarkRepositoryTest {

    @Autowired
    JdbcClient jdbcClient;

    BookmarkRepository bookmarkRepository;

    @BeforeEach
    void setUp() {
        bookmarkRepository = new BookmarkRepository(jdbcClient);
    }

    @Test
    void shouldFindAllBookmarks() {
        List<Bookmark> bookmarks = bookmarkRepository.findAll();
        assertThat(bookmarks).isNotEmpty();
        assertThat(bookmarks).hasSize(6);
    }

    @Test
    void shouldCreateBookmark() {
        Bookmark bookmark = new Bookmark(null, "My Title", "https://sivalabs.in", Instant.now());
        Long id = bookmarkRepository.save(bookmark);
        assertThat(id).isNotNull();
    }

    @Test
    void shouldGetBookmarkById() {
        Bookmark bookmark = new Bookmark(null, "My Title", "https://sivalabs.in", Instant.now());
        Long id = bookmarkRepository.save(bookmark);

        Optional<Bookmark> bookmarkOptional = bookmarkRepository.findById(id);
        assertThat(bookmarkOptional).isPresent();
        assertThat(bookmarkOptional.get().id()).isEqualTo(id);
        assertThat(bookmarkOptional.get().title()).isEqualTo(bookmark.title());
        assertThat(bookmarkOptional.get().url()).isEqualTo(bookmark.url());
    }

    @Test
    void shouldEmptyWhenBookmarkNotFound() {
        Optional<Bookmark> bookmarkOptional = bookmarkRepository.findById(9999L);
        assertThat(bookmarkOptional).isEmpty();
    }

    @Test
    void shouldUpdateBookmark() {
        Bookmark bookmark = new Bookmark(null, "My Title", "https://sivalabs.in", Instant.now());
        Long id = bookmarkRepository.save(bookmark);

        Bookmark changedBookmark = new Bookmark(id, "My Updated Title", "https://www.sivalabs.in", bookmark.createdAt());
        bookmarkRepository.update(changedBookmark);

        Bookmark updatedBookmark = bookmarkRepository.findById(id).orElseThrow();
        assertThat(updatedBookmark.id()).isEqualTo(changedBookmark.id());
        assertThat(updatedBookmark.title()).isEqualTo(changedBookmark.title());
        assertThat(updatedBookmark.url()).isEqualTo(changedBookmark.url());
    }

    @Test
    void shouldDeleteBookmark() {
        Bookmark bookmark = new Bookmark(null, "My Title", "https://sivalabs.in", Instant.now());
        Long id = bookmarkRepository.save(bookmark);

        bookmarkRepository.delete(id);

        Optional<Bookmark> optionalBookmark = bookmarkRepository.findById(id);
        assertThat(optionalBookmark).isEmpty();
    }
}
```
使用了 Testcontainers 的特殊 JDBC URL 来启动 MySQL 数据库，并使用它运行测试。
<a name="QZudq"></a>
## 六、总结
新的 JdbcClient API 提供了一个 fluent API，用于使用 JDBC 实现数据访问层。虽然仍然可以使用传统的 JdbcTemplate，但强烈推荐以后使用 JdbcClient。
