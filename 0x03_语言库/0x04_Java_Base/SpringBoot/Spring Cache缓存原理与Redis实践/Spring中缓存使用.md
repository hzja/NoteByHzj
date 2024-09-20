JavaSpring
<a name="cbt8D"></a>
## 背景
缓存主要用来提升系统查询速度。<br />比如电商中商品详情信息，这些信息通常不会经常变动但是会高频访问，可以将这些信息从db中拿出来放在缓存中（比如redis中、本地内存中），当获取的时候，先从缓存中获取，缓存中没有的时候，再从db中获取，然后将其再丢到缓存中，当商品信息被变更之后，可以将缓存中的信息剔除或者将最新的数据丢到缓存中。<br />Spring中提供了一整套的缓存解决方案，使用起来特别的容易，主要通过注解的方式使用缓存，常用的有5个注解，一个个来介绍。
<a name="UBPjO"></a>
## `@EnableCaching`：启用缓存功能
开启缓存功能，配置类中需要加上这个注解，有了这个注解之后，Spring才知道你需要使用缓存的功能，其他和缓存相关的注解才会有效，Spring中主要是通过aop实现的，通过aop来拦截需要使用缓存的方法，实现缓存的功能。
<a name="omqFl"></a>
## `@Cacheable`：赋予缓存功能
<a name="ly7cz"></a>
### 作用
`@Cacheable`可以标记在一个方法上，也可以标记在一个类上。当标记在一个方法上时表示该方法是支持缓存的，当标记在一个类上时则表示该类所有的方法都是支持缓存的。对于一个支持缓存的方法，Spring会在其被调用后将其返回值缓存起来，以保证下次利用同样的参数来执行该方法时可以直接从缓存中获取结果，而不需要再次执行该方法。Spring在缓存方法的返回值时是以键值对进行缓存的，值就是方法的返回结果，至于键的话，Spring又支持两种策略，默认策略和自定义策略，这个稍后会进行说明。需要注意的是当一个支持缓存的方法在对象内部被调用时是不会触发缓存功能的。`@Cacheable`可以指定三个属性，value、key和`condition`。
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
public @interface Cacheable {
    String[] value() default {};
    String[] cacheNames() default {};
    String key() default "";
    String condition() default "";
    String unless() default "";
}
```
<a name="bcbBp"></a>
### value属性：指定Cache名称
value和cacheNames属性作用一样，必须指定其中一个，表示当前方法的返回值是会被缓存在哪个Cache上的，对应Cache的名称。其可以是一个Cache也可以是多个Cache，当需要指定多个Cache时其是一个数组。<br />可以将Cache想象为一个HashMap，系统中可以有很多个Cache，每个Cache有一个名字，需要将方法的返回值放在哪个缓存中，需要通过缓存的名称来指定。
<a name="XewZh"></a>
#### 案例1
下面list方法加上了缓存的功能，将其结果放在缓存cache1中。
```java
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.List;

@Component
public class ArticleService {

    @Cacheable(cacheNames = {"cache1"})
    public List<String> list() {
        System.out.println("获取文章列表!");
        return Arrays.asList("spring", "mysql", "java高并发", "maven");
    }
}
```
下面来个配置类MainConfig1，必须加上`@EnableCaching`注解用来启用缓存功能。<br />然后在配置类中需要定义一个bean：缓存管理器，类型为`CacheManager`，`CacheManager`这个是个接口，有好几个实现（比如使用redis、`ConcurrentMap`来存储缓存信息），此处使用`ConcurrentMapCacheManager`，内部使用`ConcurrentHashMap`将缓存信息直接存储在本地jvm内存中，不过线上环境一般是集群的方式，可以通过Redis实现。
```java
import org.springframework.cache.CacheManager;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.cache.concurrent.ConcurrentMapCacheManager;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@EnableCaching //@0
@ComponentScan
@Configuration
public class MainConfig1 {

    //@1：缓存管理器
    @Bean
    public CacheManager cacheManager() {
        //创建缓存管理器(ConcurrentMapCacheManager：其内部使用ConcurrentMap实现的)，构造器用来指定缓存的名称，可以指定多个
        ConcurrentMapCacheManager cacheManager = new ConcurrentMapCacheManager("cache1");
        return cacheManager;
    }

}
```
来个测试类，2次调用list方法看看效果
```java
import com.javacode2018.cache.demo1.ArticleService;
import com.javacode2018.cache.demo1.MainConfig1;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class CacheTest {

    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig1.class);
        context.refresh();
        ArticleService articleService = context.getBean(ArticleService.class);
        System.out.println(articleService.list());
        System.out.println(articleService.list());
    }

}
```
输出
```
获取文章列表!
[spring, mysql, java高并发, maven]
[spring, mysql, java高并发, maven]
```
从第一行可以看出，第一次进入到list方法内部了，第二次没有进入list方法内部，而是从缓存中获取的。
<a name="gRkl8"></a>
### key属性：自定义key
key属性用来指定Spring缓存方法的返回结果时对应的key的，上面说了可以将Cache理解为一个hashMap，缓存以key->value的形式存储在hashmap中，value就是需要缓存值（即方法的返回值）<br />key属性支持SpEL表达式；当没有指定该属性时，Spring将使用默认策略生成key（`org.springframework.cache.interceptor.SimpleKeyGenerator`），默认会方法参数创建key。<br />自定义策略是指可以通过SpEL表达式来指定key，这里的SpEL表达式可以使用方法参数及它们对应的属性，使用方法参数时可以直接使用“#参数名”或者“#p参数index”。<br />Spring还提供了一个root对象可以用来生成key，通过该root对象可以获取到以下信息。

| 属性名称 | 描述 | 示例 |
| --- | --- | --- |
| methodName | 当前方法名 | `#root.methodName` |
| method | 当前方法 | `#root.method.name` |
| target | 当前被调用的对象 | `#root.target` |
| targetClass | 当前被调用的对象的class | `#root.targetClass` |
| args | 当前方法参数组成的数组 | `#root.args[0]` |
| caches | 当前被调用的方法使用的Cache | `#root.caches[0].name` |

这里主要看一下自定义策略。
<a name="zcmmw"></a>
#### 案例2
ArticleService加入下面代码
```java
@Cacheable(cacheNames = {"cache1"}, key = "#root.target.class.name+'-'+#page+'-'+#pageSize")
public String getPage(int page, int pageSize) {
    String msg = String.format("page-%s-pageSize-%s", page, pageSize);
    System.out.println("从db中获取数据：" + msg);
    return msg;
}
```
com.javacode2018.cache.CacheTest新增测试用例
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    ArticleService articleService = context.getBean(ArticleService.class);

    //page=1,pageSize=10调用2次
    System.out.println(articleService.getPage(1, 10));
    System.out.println(articleService.getPage(1, 10));

    //page=2,pageSize=10调用2次
    System.out.println(articleService.getPage(2, 10));
    System.out.println(articleService.getPage(2, 10));

    {
        System.out.println("下面打印出cache1缓存中的key列表");
        ConcurrentMapCacheManager cacheManager = context.getBean(ConcurrentMapCacheManager.class);
        ConcurrentMapCache cache1 = (ConcurrentMapCache) cacheManager.getCache("cache1");
        cache1.getNativeCache().keySet().stream().forEach(System.out::println);
    }
}
```
运行输出
```
从db中获取数据：page-1-pageSize-10
page-1-pageSize-10
page-1-pageSize-10
从db中获取数据：page-2-pageSize-10
page-2-pageSize-10
page-2-pageSize-10
下面打印出cache1缓存中的key列表
com.javacode2018.cache.demo1.ArticleService-getPage-1-10
com.javacode2018.cache.demo1.ArticleService-getPage-2-10
```
<a name="FYZKY"></a>
### `condition`属性：控制缓存的使用条件
有时候，可能希望查询不走缓存，同时返回的结果也不要被缓存，那么就可以通过`condition`属性来实现，`condition`属性默认为空，表示将缓存所有的调用情形，其值是通过spel表达式来指定的，当为true时表示先尝试从缓存中获取；若缓存中不存在，则只需方法，并将方法返回值丢到缓存中；当为false的时候，不走缓存、直接执行方法、并且返回结果也不会丢到缓存中。<br />其值spel的写法和key属性类似。
<a name="Wr871"></a>
#### 案例3
ArticleService添加下面代码，方法的第二个参数cache用来控制是否走缓存，将`condition`的值指定为`#cache`
```java
/**
 * 通过文章id获取文章
 *
 * @param id    文章id
 * @param cache 是否尝试从缓存中获取
 * @return
 */
@Cacheable(cacheNames = "cache1", key = "'getById'+#id", condition = "#cache")
public String getById(Long id, boolean cache) {
    System.out.println("获取数据!");
    return "spring缓存:" + UUID.randomUUID().toString();
}
```
来个测试用例，4次调用getById方法，前面2次和最后一次cache参数都是true，第3次为false
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    ArticleService articleService = context.getBean(ArticleService.class);

    System.out.println(articleService.getById(1L, true));
    System.out.println(articleService.getById(1L, true));
    System.out.println(articleService.getById(1L, false));
    System.out.println(articleService.getById(1L, true));
}
```
运行输出
```
获取数据!
spring缓存:27e7c11a-26ed-4c8b-8444-78257daafed5
spring缓存:27e7c11a-26ed-4c8b-8444-78257daafed5
获取数据!
spring缓存:05ff7612-29cb-4863-b8bf-d1b7c2c192b7
spring缓存:27e7c11a-26ed-4c8b-8444-78257daafed5
```
从输出中可以看出，第1次和第3次，都进到方法里面去了，而2和4走了缓存，第一次执行完毕之后结果被丢到了缓存中，所以2和4这2次获取的结果和第1次是一样的。
<a name="flN86"></a>
### `unless`属性：控制是否需要将结果丢到缓存中
用于否决方法缓存的SpEL表达式。 与`condition`不同，此表达式是在调用方法后计算的，因此可以引用结果。 默认值为“”，这意味着缓存永远不会被否决。<br />前提是`condition`为空或者为true的情况下，`unless`才有效，`condition`为false的时候，`unless`无效，`unless`为true，方法返回结果不会丢到缓存中；`unless`为false，方法返回结果会丢到缓存中。<br />其值`spel`的写法和key属性类似。
<a name="xHdlv"></a>
#### 案例4
下面来个案例，当返回结果为null的时候，不要将结果进行缓存，ArticleService添加下面代码
```java
Map<Long, String> articleMap = new HashMap<>();
/**
 * 获取文章，先从缓存中获取，如果获取的结果为空，不要将结果放在缓存中
 *
 * @param id
 * @return
 */
@Cacheable(cacheNames = "cache1", key = "'findById'+#id", unless = "#result==null")
public String findById(Long id) {
    this.articleMap.put(1L, "spring系列");
    System.out.println("----获取文章:" + id);
    return articleMap.get(id);
}
```
来个测试用例，4次调用findById，前面2次有数据，后面2次返回null，并将缓存中的key打印了出来
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    ArticleService articleService = context.getBean(ArticleService.class);

    System.out.println(articleService.findById(1L));
    System.out.println(articleService.findById(1L));
    System.out.println(articleService.findById(3L));
    System.out.println(articleService.findById(3L));

    {
        System.out.println("下面打印出缓cache1缓存中的key列表");
        ConcurrentMapCacheManager cacheManager = context.getBean(ConcurrentMapCacheManager.class);
        ConcurrentMapCache cache1 = (ConcurrentMapCache) cacheManager.getCache("cache1");
        cache1.getNativeCache().keySet().stream().forEach(System.out::println);
    }
}
```
运行输出
```
----获取文章:1
spring系列
spring系列
----获取文章:3
null
----获取文章:3
null
下面打印出缓cache1缓存中的key列表
findById1
```
可以看出文章id为1的结果被缓存了，文件id为3的没有被缓存。
<a name="QE9MP"></a>
### `condition`和`unless`对比
缓存的使用过程中有2个点：

1. 查询缓存中是否有数据
2. 如果缓存中没有数据，则去执行目标方法，然后将方法结果丢到缓存中。

Spring中通过`condition`和`unless`对这2点进行干预。<br />`condition`作用域上面2个过程，当为true的时候，会尝试从缓存中获取数据，如果没有，会执行方法，然后将方法返回值丢到缓存中；如果为false，则直接调用目标方法，并且结果不会放在缓存中。<br />而`unless`在`condition`为true的情况下才有效，用来判断上面第2点中，是否不要将结果丢到缓存中，如果为true，则结果不会丢到缓存中，如果为false，则结果会丢到缓存中，并且unless中可以使用spel表达式通过`#result`来获取方法返回值。
<a name="LhdpK"></a>
## `@CachePut`：将结果放入缓存
<a name="Iiz94"></a>
### 作用
`@CachePut`也可以标注在类或者方法上，被标注的方法每次都会被调用，然后方法执行完毕之后，会将方法结果丢到缓存中；当标注在类上，相当于在类的所有方法上标注了`@CachePut`。<br />有3种情况，结果不会丢到缓存

1. 当方法向外抛出的时候
2. `condition`的计算结果为false的时候
3. `unless`的计算结果为true的时候

源码和`Cacheable`类似，包含的参数类似的。
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
public @interface CachePut {
    String[] value() default {};
    String[] cacheNames() default {};
    String key() default "";
    String condition() default "";
    String unless() default "";
}
```

- `value`和`cacheNames`：用来指定缓存名称，可以指定多个
- `key`：缓存的key，spel表达式，写法参考`@Cacheable`中的key
- `condition`：spel表达式，写法和`@Cacheable`中的`condition`一样，当为空或者计算结果为true的时候，方法的返回值才会丢到缓存中；否则结果不会丢到缓存中
- `unless`：当`condition`为空或者计算结果为true的时候，`unless`才会起效；true：结果不会被丢到缓存，false：结果会被丢到缓存。
<a name="f2VVx"></a>
### 案例5
来个案例，实现新增文章的操作，然后将文章丢到缓存中，注意下面`@CachePut`中的cacheNames、key 2个参数和案例4中findById方法上`@Cacheable`中的一样，说明他们共用一个缓存，key也是一样的，那么当`add`方法执行完毕之后，再去调用`findById`方法，则可以从缓存中直接获取到数据。
```java
@CachePut(cacheNames = "cache1", key = "'findById'+#id")
public String add(Long id, String content) {
    System.out.println("新增文章:" + id);
    this.articleMap.put(id, content);
    return content;
}
```
测试用例
```java
@Test
public void test5() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    ArticleService articleService = context.getBean(ArticleService.class);

    //新增3个文章，由于add方法上面有@CachePut注解，所以新增之后会自动丢到缓存中
    articleService.add(1L, "java高并发系列");
    articleService.add(2L, "Maven高手系列");
    articleService.add(3L, "MySQL高手系列");

    //然后调用findById获取，看看是否会走缓存
    System.out.println("调用findById方法，会尝试从缓存中获取");
    System.out.println(articleService.findById(1L));
    System.out.println(articleService.findById(2L));
    System.out.println(articleService.findById(3L));

    {
        System.out.println("下面打印出cache1缓存中的key列表");
        ConcurrentMapCacheManager cacheManager = context.getBean(ConcurrentMapCacheManager.class);
        ConcurrentMapCache cache1 = (ConcurrentMapCache) cacheManager.getCache("cache1");
        cache1.getNativeCache().keySet().stream().forEach(System.out::println);
    }
}
```
运行输出
```
新增文章:1
新增文章:2
新增文章:3
调用findById方法，会尝试从缓存中获取
java高并发系列
Maven高手系列
MySQL高手系列
下面打印出缓cache1缓存中的key列表
findById3
findById2
findById1
```
看几眼输出结果，然后再来看一下findById方法的代码
```java
@Cacheable(cacheNames = "cache1", key = "'findById'+#id", unless = "#result==null")
public String findById(Long id) {
    this.articleMap.put(1L, "spring系列");
    System.out.println("----获取文章:" + id);
    return articleMap.get(id);
}
```
输出中并没有----这样的内容，说明调用`findById`方法获取结果是从缓存中得到的。
<a name="LJBzw"></a>
## `@CacheEvict`：缓存清理
<a name="bgBRU"></a>
### 作用
用来清除缓存的，`@CacheEvict`也可以标注在类或者方法上，被标注在方法上，则目标方法被调用的时候，会清除指定的缓存；当标注在类上，相当于在类的所有方法上标注了`@CacheEvict`。<br />来看一下源码，每个参数的注释大家详细看一下。
```java
public @interface CacheEvict {

    /**
     * cache的名称，和cacheNames效果一样
     */
    String[] value() default {};

    /**
     * cache的名称，和cacheNames效果一样
     */
    String[] cacheNames() default {};

    /**
     * 缓存的key，写法参考上面@Cacheable注解的key
     */
    String key() default "";

    /**
     * @CacheEvict 注解生效的条件，值为spel表达式，写法参考上面 @Cacheable注解中的condition
     */
    String condition() default "";

    /**
     * 是否清理 cacheNames 指定的缓存中的所有缓存信息，默认是false
     * 可以将一个cache想象为一个HashMap，当 allEntries 为true的时候，相当于HashMap.clear()
     * 当 allEntries 为false的时候，只会干掉key对应的数据，相当于HashMap.remove(key)
     */
    boolean allEntries() default false;

    /**
     * 何事执行清除操作（方法执行前 or 方法执行成功之后）
     * true：@CacheEvict 标注的方法执行之前，执行清除操作
     * false：@CacheEvict 标注的方法执行成功之后，执行清除操作，当方法弹出异常的时候，不会执行清除操作
     */
    boolean beforeInvocation() default false;
}
```
<a name="OegLq"></a>
### condition属性
`@CacheEvict` 注解生效的条件，值为spel表达式，写法参考上面 `@Cacheable`注解中的`condition`
<a name="s0c9Q"></a>
### 会清除哪些缓存？
默认情况下会清除`cacheNames`指定的缓存中key参数指定的缓存信息。<br />但是当 allEntries 为true的时候，会清除 cacheNames 指定的缓存中的所有缓存信息。
<a name="wRBGa"></a>
### 具体什么时候清除缓存？
这个是通过 `beforeInvocation` 参数控制的，这个参数默认是false，默认会在目标方法成功执行之后执行清除操作，若方法向外抛出了异常，不会执行清理操作；<br />如果 beforeInvocation  为true，则方法被执行之前就会执行缓存清理操作，方法执行之后不会再执行了。
<a name="xNj6h"></a>
### 案例6
ArticleService中新增个方法，使用`@CacheEvict`标注，这个方法执行完毕之后，会清理cache1中key=findById+参数id的缓存信息，注意`cacheNames`和key两个参数的值和findById中这2个参数的值一样。
```java
@CacheEvict(cacheNames = "cache1", key = "'findById'+#id") //@1
public void delete(Long id) {
    System.out.println("删除文章：" + id);
    this.articleMap.remove(id);
}
```
新增测试用例，注释比较清晰，就不解释了
```java
@Test
public void test6() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    ArticleService articleService = context.getBean(ArticleService.class);

    //第1次调用findById，缓存中没有，则调用方法，将结果丢到缓存中
    System.out.println(articleService.findById(1L));
    //第2次调用findById，缓存中存在，直接从缓存中获取
    System.out.println(articleService.findById(1L));

    //执行删除操作，delete方法上面有@CacheEvict方法，会清除缓存
    articleService.delete(1L);

    //再次调用findById方法，发现缓存中没有了，则会调用目标方法
    System.out.println(articleService.findById(1L));
}
```
运行输出
```
----获取文章:1
spring系列
spring系列
删除文章：1
----获取文章:1
spring系列
```
调用了3次findById，第1次，缓存中没有，所以进到方法内部了，然后将结果丢到缓存了，第2次缓存中有，所以从缓存获取，然后执行了delete方法，这个方法执行完毕之后，会清除缓存中文章id为1L的文章信息，最后执行第三次`findById`方法，此时缓存中没有发现数据，然后进到目标方法内部了，目标方法内部输出了----内容。
<a name="Wlwbx"></a>
## `@Caching`：缓存注解组
当在类上或者同一个方法上同时使用`@Cacheable`、`@CachePut`和`@CacheEvic`这几个注解中的多个的时候，此时可以使用`@Caching`这个注解来实现。
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
public @interface Caching {

    Cacheable[] cacheable() default {};

    CachePut[] put() default {};

    CacheEvict[] evict() default {};

}
```
<a name="zVewF"></a>
## `@CacheConfig`：提取公共配置
这个注解标注在类上，可以将其他几个缓存注解（`@Cacheable`、`@CachePut`和`@CacheEvic`）的公共参数给提取出来放在`@CacheConfig`中。<br />比如当一个类中有很多方法都需要使用（`@Cacheable`、`@CachePut`和`@CacheEvic`）这些缓存注解的时候，大家可以看一下这3个注解的源码，他们有很多公共的属性，比如：`cacheNames`、`keyGenerator`、`cacheManager`、`cacheResolver`，若这些属性值都是一样的，可以将其提取出来，放在`@CacheConfig`中，不过这些注解（`@Cacheable`、`@CachePut`和`@CacheEvic`）中也可以指定属性的值对`@CacheConfig`中的属性值进行覆盖。
```java
@CacheConfig(cacheNames = "cache1")
public class ArticleService {
    @Cacheable(key = "'findById'+#id")
    public String findById(Long id) {
        this.articleMap.put(1L, "spring系列");
        System.out.println("----获取文章:" + id);
        return articleMap.get(id);
    }
}
```
<a name="z8ZDd"></a>
## 原理
Spring中的缓存主要是利用Spring中aop实现的，通过Aop对需要使用缓存的bean创建代理对象，通过代理对象拦截目标方法的执行，实现缓存功能。<br />重点在于`@EnableCaching`这个注解，可以从`@Import`这个注解看起
```java
@Import(CachingConfigurationSelector.class)
public @interface EnableCaching {
}
```
最终会给需要使用缓存的bean创建代理对象，并且会在代理中添加一个拦截器`org.springframework.cache.interceptor.CacheInterceptor`，这个类中的`invoke`方法是关键，会拦截所有缓存相关的目标方法的执行，大家可以去细看一下。
