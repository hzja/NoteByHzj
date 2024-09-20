JavaRedis<br />开始之前需要有Redis安装，本机Docker运行Redis，主要命令如下
```bash
docker pull redis
docker run --name my_redis -d -p 6379:6379 redis
docker exec -it my_redis bash
redis-cli
```
前面两个命令是启动redis docker，后两个是连接到docker，在使用redis-cli去查看redis里面的内容，主要查看存在redis里面的数据。
<a name="TkafB"></a>
## RedisTemplate
先从RedisTemplate开始，这个是最好理解的一种方式，在工作中也使用过这种方式，先看代码示例，先定义一个POJO类
```java
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class Book implements Serializable {
    private Long id;
    private String name;
    private String author;
}
```
一个很简单的BOOK类，三个字段：id、name和author。再来一个RedisTemplate的Bean
```java
@Bean
public RedisTemplate<String, Book> redisTemplate(RedisConnectionFactory redisConnectionFactory) {
    RedisTemplate<String, Book> template = new RedisTemplate<>();
    template.setConnectionFactory(redisConnectionFactory);
    return template;
}
```
再定义一个使用这个RedisTemplate的Service类
```java
public Optional<Book> findOneBook(String name) {
    HashOperations<String, String, Book> hashOperations = redisTemplate.opsForHash();
    if (redisTemplate.hasKey(CACHE) && hashOperations.hasKey(CACHE, name)) {
        log.info("Get book {} from Redis.", name);
        return Optional.of(hashOperations.get(CACHE, name));
    }

    Optional<Book> book = bookRepository.getBook(name);
    log.info("Book Found: {}", book);
    if (book.isPresent()) {
        log.info("Put book {} to Redis.", name);
        hashOperations.put(CACHE, name, book.get());
        redisTemplate.expire(CACHE, 10, TimeUnit.MINUTES);
    }
    return book;
}
```
使用Hash来存储这个Book信息，在上面的方法中查找书名存不存在Redis中，如果存在就直接返回，如果不存在就去持久化存储中找，找到就再通过Template写入到Redis中，这是缓存的通用做法。使用起来感觉很方便。<br />这里为了简单没有使用持久化存储，就硬编码了几条数据，代码如下
```java
@Repository
public class BookRepository {
    Map<String, Book> bookMap = new HashMap<>();
    public BookRepository(){
        bookMap.put("apache kafka", Book.builder()
                .name("apache kafka").id(1L).author("zhangsan")
                .build());
        bookMap.put("python", Book.builder()
                .name("python").id(2L).author("lisi")
                .build());
    }

    public Optional<Book> getBook(String name){
        if(bookMap.containsKey(name)){
            return Optional.of(bookMap.get(name));
        }
        else{
            return Optional.empty();
        }
    }
}
```
调用`bookService.findOneBook("python")`和`bookService.findOneBook("apache kafka");`来把数据写入到换存中<br />来看下存储在Redis的数据长什么样子。
```bash
127.0.0.1:6379> keys *
1) "\xac\xed\x00\x05t\x00\x04book"
127.0.0.1:6379> type "\xac\xed\x00\x05t\x00\x04book"
hash
127.0.0.1:6379> hgetall "\xac\xed\x00\x05t\x00\x04book"
1) "\xac\xed\x00\x05t\x00\x06python"
2) "\xac\xed\x00\x05sr\x00&com.ken.redistemplatesample.model.Book=\x19\x96\xfb\x7f\x7f\xda\xbe\x02\x00\x03L\x00\x06authort\x00\x12Ljava/lang/String;L\x00\x02idt\x00\x10Ljava/lang/Long;L\x00\x04nameq\x00~\x00\x01xpt\x00\x04lisisr\x00\x0ejava.lang.Long;\x8b\xe4\x90\xcc\x8f#\xdf\x02\x00\x01J\x00\x05valuexr\x00\x10java.lang.Number\x86\xac\x95\x1d\x0b\x94\xe0\x8b\x02\x00\x00xp\x00\x00\x00\x00\x00\x00\x00\x02t\x00\x06python"
3) "\xac\xed\x00\x05t\x00\x0capache kafka"
4) "\xac\xed\x00\x05sr\x00&com.ken.redistemplatesample.model.Book=\x19\x96\xfb\x7f\x7f\xda\xbe\x02\x00\x03L\x00\x06authort\x00\x12Ljava/lang/String;L\x00\x02idt\x00\x10Ljava/lang/Long;L\x00\x04nameq\x00~\x00\x01xpt\x00\bzhangsansr\x00\x0ejava.lang.Long;\x8b\xe4\x90\xcc\x8f#\xdf\x02\x00\x01J\x00\x05valuexr\x00\x10java.lang.Number\x86\xac\x95\x1d\x0b\x94\xe0\x8b\x02\x00\x00xp\x00\x00\x00\x00\x00\x00\x00\x01t\x00\x0capache kafka"
```
可以看到数据被存在了key是“\xac\xed\x00\x05t\x00\x04book”的一个Hash表中，Hash里面有两条记录。大家发现一个问题没有？<br />就是这个key不是预期的用“book”做key，而是多了一串16进制的码，这是因为RedisTemplate使用了默认的`JdkSerializationRedisSerializer`去序列化key和value，如果大家都用Java语言那没有问题，如果有人用Java语言写，有人用别的语言读，那就有问题，就像开始的时候用`hgetall "book"`始终拿不到数据那样。<br />`RedisTemplate`也提供了`StringRedisTemplate`来方便大家需要使用String来序列化redis里面的数据。简单看下代码
```java
@Bean
public StringRedisTemplate stringRedisTemplate(RedisConnectionFactory redisConnectionFactory)
{
    StringRedisTemplate template = new StringRedisTemplate();
    template.setConnectionFactory(redisConnectionFactory);
    return template;
}

public Optional<String> getBookString(String name){
    HashOperations<String, String, String> hashOperations = stringRedisTemplate.opsForHash();
    if (stringRedisTemplate.hasKey(STRINGCACHE) && hashOperations.hasKey(STRINGCACHE, name)) {
        log.info("Get book {} from Redis.", name);
        return Optional.of(hashOperations.get(STRINGCACHE, name));
    }

    Optional<Book> book = bookRepository.getBook(name);
    log.info("Book Found: {}", book);
    if (book.isPresent()) {
        log.info("Put book {} to Redis.", name);
        hashOperations.put(STRINGCACHE, name, book.get().getAuthor());
        stringRedisTemplate.expire(STRINGCACHE, 10, TimeUnit.MINUTES);
        return Optional.of(book.get().getAuthor());
    }
    return Optional.empty();
}
```
使用上就没有那么方便，就得自己写需要存的是哪个字段，读出来是哪个字段。
```bash
127.0.0.1:6379> keys *
1) "string_book"
127.0.0.1:6379> hgetall string_book
1) "python"
2) "lisi"
3) "apache kafka"
4) "zhangsan"
```
```bash
127.0.0.1:6379> keys *
1) "\xac\xed\x00\x05t\x00\x04book"
2) "string_book"
127.0.0.1:6379> memory usage "string_book"
(integer) 104
127.0.0.1:6379> memory usage "\xac\xed\x00\x05t\x00\x04book"
(integer) 712
```
使用客户端读出来看起来就比较清爽一些。也可以看到占用的Size会小很多，这个例子相差7倍，如果是数据量大，这个还是比较大的浪费。
<a name="V14Os"></a>
## JPA Repository
使用JPA Repository来访问DataBase的时候，增删改查那样的操作能够很方便的实现，基本就是定义个接口，代码都不用写，Spring就完成了大部分的工作，那么访问Redis是不是也可以这样呢？答案是肯定的，来看代码，首先还是定义一个POJO
```java
@RedisHash(value = "cache-book", timeToLive = 600)
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class CacheBook implements Serializable {

    @Id
    private Long userId;

    @Indexed
    private String name;

    private String author;
}
```
这个类与上面template上面的类的区别就是加了两个注解，在类开头加了`@RedisHash(value = "cache-book", timeToLive = 600)`在字段上面加了`@Id`和`@Indexed`定义一个`Repository`的接口
```java
public interface CacheBookRepository extends CrudRepository<CacheBook, Long> {
    Optional<CacheBook> findOneByName(String name);
}
```
再定义一个service和上面那个例子template一样，缓存中有就到缓存中拿，没有就到持久化存储中找，并写入缓存
```java
@Slf4j
@Service
public class BookService {
    private static final String CACHE = "repository-book";
    @Autowired
    private CacheBookRepository cacheRepository;

    @Autowired
    private BookRepository bookRepository;

    public Optional<CacheBook> findOneBook(String name) {
        Optional<CacheBook> optionalCacheBook = cacheRepository.findOneByName(name);
        if(!optionalCacheBook.isPresent())
        {
            Optional<CacheBook> book = bookRepository.getBook(name);
            log.info("Book Found: {}", book);
            if (book.isPresent()) {
                log.info("Put book {} to Redis.", name);
                cacheRepository.save(book.get());
            }
            return book;
        }
        return optionalCacheBook;
    }
}
```
代码很简单，简单到不敢相信是真的。还是一样，调用这个方法，来看存在Redis里面的数据
```bash
127.0.0.1:6379> keys *
1) "repository-book:2"
2) "repository-book:2:idx"
3) "repository-book"
4) "repository-book:name:apache kafka"
5) "repository-book:name:python"
6) "repository-book:1:idx"
7) "repository-book:1"
```
感觉存的内容有些多，来看下各自存什么数据，首先看最短的一个
```bash
127.0.0.1:6379> smembers repository-book
1) "1"
2) "2"
```
它里面存的是id所有的value，可以用来判断id是否存在 再来看
```bash
127.0.0.1:6379> hgetall repository-book:2
1) "_class"
2) "com.ken.redisrepositorysample.model.CacheBook"
3) "author"
4) "lisi"
5) "name"
6) "python"
7) "userId"
8) "2"
```
这个是数据存放的地方
```bash
127.0.0.1:6379> smembers repository-book:1:idx
1) "repository-book:name:apache kafka"
127.0.0.1:6379> smembers "repository-book:name:apache kafka"
1) "1"
```
另外两个都是set，存在在它们里面的数据是索引信息。由此可以看出通过JPA Repository 的方式，代码很少，而且存储的数据也很通用，是比较理想的访问方法。
<a name="jgxA0"></a>
## Cache
已经看了两种方式，在访问的时候遵循这样的模式：缓存中有就从缓存中返回，没有就从持久化存储中找，然后写入缓存，这部分代码也不想自己写，那么Cache就是救星。<br />先看代码，这次使用内存数据库H2作为持久化存储，放一个schema.sql在resouces下面
```sql
drop table t_book if exists;

create table t_book (
  id bigint auto_increment,
  create_time timestamp,
  update_time timestamp,
  name varchar(255),
  author varchar(200),
  primary key (id)
);

insert into t_book (name, author, create_time, update_time) values ('python', 'zhangsan', now(), now());
insert into t_book (name, author, create_time, update_time) values ('hadoop', 'lisi', now(), now());
insert into t_book (name, author, create_time, update_time) values ('java', 'wangwu', now(), now());
```
然后定义POJO
```java
@Entity
@Table(name = "T_BOOK")
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class CacheBook implements Serializable {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String name;

    private String author;

    @Column(updatable = false)
    @CreationTimestamp
    private Date createTime;
    @UpdateTimestamp
    private Date updateTime;
}
```
完全是和数据库绑定的代码，和缓存没有任何关系 一个Repository来访问数据库
```java
public interface BookRepository extends JpaRepository<CacheBook, Long> {
}
```
定义一个service来调用它
```java
@Slf4j
@Service
@CacheConfig(cacheNames = "cache-book")
public class BookService {

    @Autowired
    private BookRepository bookRepository;

    @Cacheable
    public List<CacheBook> findAllCoffee() {
        return bookRepository.findAll();
    }

    @CacheEvict
    public void reloadCoffee() {
    }
}
```
这里就比较关键了，在类上加上了注解`@CacheConfig(cacheNames = "cache-book")`在方法上面加上了`Cacheable`和`CacheEvict`，`Cacheable`这个方法就是用来实现逻辑，有就从缓存中拿，没有就从数据库拿的，`CacheEvict`是调用这个方法的时候清除缓存。<br />然后再启动入口程序的地方加上注解`@EnableJpaRepositories @EnableCaching(proxyTargetClass = true)`在配置文件application.properties中加上
```
spring.jpa.hibernate.ddl-auto=none
spring.jpa.properties.hibernate.show_sql=true
spring.jpa.properties.hibernate.format_sql=true

management.endpoints.web.exposure.include=*

spring.cache.type=redis
spring.cache.cache-names=cache-book
spring.cache.redis.time-to-live=600000
spring.cache.redis.cache-null-values=false

spring.redis.host=localhost
```
这样就可以了，感觉就是通过配置下就把缓存给完成了，非常的简单，来看Redis中是怎么存的
```bash
127.0.0.1:6379> keys *
1) "cache-book::SimpleKey []"
127.0.0.1:6379> get "cache-book::SimpleKey []"
"\xac\xed\x00\x05sr\x00\x13java.util.ArrayListx\x81\xd2\x1d\x99\xc7a\x9d\x03\x00\x01I\x00\x04sizexp\x00\x00\x00\x03w\x04\x00\x00\x00\x03sr\x00(com.ken.rediscachesample.model.CacheBook\xec\xcbR=\xe1U\x9b\xf7\x02\x00\x05L\x00\x06authort\x00\x12Ljava/lang/String;L\x00\ncreateTimet\x00\x10Ljava/util/Date;L\x00\x02idt\x00\x10Ljava/lang/Long;L\x00\x04nameq\x00~\x00\x03L\x00\nupdateTimeq\x00~\x00\x04xpt\x00\bzhangsansr\x00\x12java.sql.Timestamp&\x18\xd5\xc8\x01S\xbfe\x02\x00\x01I\x00\x05nanosxr\x00\x0ejava.util.Datehj\x81\x01KYt\x19\x03\x00\x00xpw\b\x00\x00\x01\x84\xff]\x85\xb0x\b-\x81\x80sr\x00\x0ejava.lang.Long;\x8b\xe4\x90\xcc\x8f#\xdf\x02\x00\x01J\x00\x05valuexr\x00\x10java.lang.Number\x86\xac\x95\x1d\x0b\x94\xe0\x8b\x02\x00\x00xp\x00\x00\x00\x00\x00\x00\x00\x01t\x00\x06pythonsq\x00~\x00\bw\b\x00\x00\x01\x84\xff]\x85\xb0x\b-\x81\x80sq\x00~\x00\x02t\x00\x04lisisq\x00~\x00\bw\b\x00\x00\x01\x84\xff]\x85\xb0x\b<\xbf\xd8sq\x00~\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x02t\x00\x06hadoopsq\x00~\x00\bw\b\x00\x00\x01\x84\xff]\x85\xb0x\b<\xbf\xd8sq\x00~\x00\x02t\x00\x06wangwusq\x00~\x00\bw\b\x00\x00\x01\x84\xff]\x85\xb0x\b<\xbf\xd8sq\x00~\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x03t\x00\x04javasq\x00~\x00\bw\b\x00\x00\x01\x84\xff]\x85\xb0x\b<\xbf\xd8x"
```
看到没有，就是当成Redis里面的String来存的，如果数据量比较小，那是非常的方便，如果数据量大，这种方式就有些问题了。
<a name="qVyHr"></a>
## 总结
看了这三种方式，这里仅仅是做了个入门，每个里面都有很多细节的地方需要去研究和使用，整体的感觉是要想使用的简单，那么存储在Redis中的数据就要量少，量大后，就需要自己来定制了，那基本上要用RedisTemplate来做一些工作。
