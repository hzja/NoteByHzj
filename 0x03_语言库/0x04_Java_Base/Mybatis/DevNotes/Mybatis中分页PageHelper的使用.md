分页PageHelper
<a name="Q3J7M"></a>
## 一、技术文档
<a name="W5IiP"></a>
### 1、基于SpringBoot

- SpringBoot 官方文档：[https://spring.io/projects/spring-boot](https://spring.io/projects/spring-boot)
- SpringBoot 中文社区：[https://springboot.io/](https://springboot.io/)
<a name="qSmJW"></a>
### 2、基于MyBatis

- MyBatis 官方文档：[https://mybatis.org/mybatis-3/zh/index.html](https://mybatis.org/mybatis-3/zh/index.html)
<a name="gDzBa"></a>
### 3、集成PageHelper

- PageHelper 开源仓库：[https://github.com/pagehelper/Mybatis-PageHelper](https://github.com/pagehelper/Mybatis-PageHelper)
<a name="zT0xF"></a>
## 二、开发依赖
<a name="SpringBoot"></a>
### SpringBoot
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
<a name="MyBatis"></a>
### MyBatis
```xml
<dependency>
  <groupId>org.mybatis.spring.boot</groupId>
  <artifactId>mybatis-spring-boot-starter</artifactId>
  <version>2.1.3</version>
</dependency>
```
<a name="PageHelper"></a>
### PageHelper
```xml
<dependency>
  <groupId>com.github.pagehelper</groupId>
  <artifactId>pagehelper-spring-boot-starter</artifactId>
  <version>1.3.0</version>
</dependency>
```
<a name="DGqZk"></a>
## 三、应用讲解
<a name="T0QXI"></a>
### 1、基本使用
在实际项目运用中，**PageHelper**的使用非常便利快捷，仅通过`PageInfo` + `PageHelper`两个类，就足以完成分页功能，然而往往这种最简单的集成使用方式，却在很多实际应用场景中，没有得到充分的开发利用。<br />下面是最常见的使用方式：
```java
public PageInfo<ResponseEntityDto> page(RequestParamDto param) {
    PageHelper.startPage(param.getPageNum(), param.getPageSize());
    List<ResoinseEntityDto> list = mapper.selectManySelective(param);
    PageInfo<ResponseEntityDto> pageInfo = (PageInfo<ResponseEntityDto>)list;
    return pageInfo;
} 
```
在某种程度上而言，上述写法的确是符合PageHelper的使用规范：
> 在集合查询前使用`PageHelper.startPage(pageNum,pageSize)`，并且_中间不能穿插执行其他SQL_

但是作为Developer，往往只有在追求完美和极致的道路上才能够寻得突破和机遇; 以下是合理且规范的基本使用：
```java
public PageInfo<ResponseEntityDto> page(RequestParamDto param) {
    return PageHelper.startPage(param.getPageNum(), param.getPageSize())
        .doSelectPageInfo(() -> list(param))
    } 
public List<ResponseEntityDto> list(RequestParamDto param) {
    return mapper.selectManySelective(param);
}
```
<a name="ASzun"></a>
#### FAQ
<a name="pnEqj"></a>
##### 1、为什么要重新声明一个list函数?
答：往往在很多实际业务应用场景中，**分页查询**是基于大数据量的表格展示需求来进行的。然而很多时候，**譬如**：内部服务的互相调用，`OpenAPI`的提供。甚至在某些前后端分离联调的业务场景中，是同样需要一个非分页集合查询接口来提供服务的。另外，暂时以上因素抛开不谈，可以根据上述写法来定义和规范某些东西**譬如**：分页和集合查询的**分离和解耦**(解耦详情请看进阶使用)，分页请求的请求和响应与实际业务**参数的分离**(详情请看进阶使用)等等…
<a name="aFaYz"></a>
##### 2、`doSelectPageInfo`是什么?
答：`doSelectPageInfo`是`PageHelper.startPage()`函数返回的默认`Page`实例内置的函数，该函数可以用以`Lambda`的形式通过额外的`Function`来进行查询而不需要再进行多余的`PageInfo`与`List`转换，而`doSelectPageInfo`的参数则是`PageHelper`内置的`Function(ISelect)`接口用以达到转换`PageInfo`的目的
<a name="lQ6vw"></a>
##### 3、这种写法的代码量看起来不少反多?
答：正如同①中所描述的，就代码量而言，确实没有更进一步的简化，但是再某些业务场景中，在已具有`list`函数接口的情况下，是一种更直观的优化(优化详情请看进阶使用)
<a name="yvn7R"></a>
### 2、进阶使用
先看代码，再谈解析：
```java
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import java.util.List;
/**
 * @param <Param> 泛型request
 * @param <Result> 泛型response
 */
public interface BaseService<Param, Result> {
    /**
     * 分页查询
     *
     * @param param 请求参数DTO
     * @return 分页集合
     */
    default PageInfo<Result> page(PageParam<Param> param) {
        return PageHelper.startPage(param).doSelectPageInfo(() -> list(param.getParam()));
    }
    /**
     * 集合查询
     *
     * @param param 查询参数
     * @return 查询响应
     */
    List<Result> list(Param param);
}
```
可以看到`BaseService`可以作为全局`Service`通用接口的封装和声明 而作为通用分页接口page函数却在此处利用`interface`特有关键字`default` 直接声明了`page`函数的方法体`body`
```java
import com.github.pagehelper.IPage;
import lombok.Data;
import lombok.experimental.Accessors;
@Data // 为省略冗余代码使用lombok 实际应有常规Getter/Setter Construction toString等
@Accessors(chain = true) // 此lombok注解是为了实现 Entity伪Build 譬如: entity.setX(x).setY(y)
public class PageParam<T>  implements IPage {
    //  description = "页码", defaultValue =  1
    private Integer pageNum = 1;
    // description = "页数", defaultValue = 20
    private Integer pageSize = 20;
    // description = "排序", example = "id desc"
    private String orderBy;
    //  description = "参数"
    private T param;
    public PageParam<T> setOrderBy(String orderBy) {
        this.orderBy = orderBy; // 此处可优化 优化详情且看解析
        return this;
    }
}
```
在`BaseService`中看到了一个新的`PageParam`，参考了`PageInfo`用以包装/声明/分离分页参数和业务参数，且参数类型为泛型，即支持任何数据类型的业务参数 同时也可以看到`PageParam`实现了`IPage`接口，并且多了一个`orderBy`属性字段
```java
import common.base.BaseService;
import dto.req.TemplateReqDto;
import dto.resp.TemplateRespDto;
public interface TemplateService extends BaseService<TemplateReqDto, TemplateeRespDto> {
    // 同为interface接口, 业务Service只需要继承BaseService
    // 并根据实际使用场景声明请求参数和响应结果的Entity实体即可
}
```
在实际应用中，只需要声明通用的业务查询请求参数和响应结果即可
```java
import dto.req.TemplateReqDto;
import dto.resp.TemplateRespDto;
import service.TemplateService;
import persistence.mapper.TemplateMapper;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import java.util.List;
@Slf4j // 基于lombok自动生成logger日志记录实例
@Service // SpringBoot中注册Service Bean的注解
@RequiredArgsConstructor // 基于lombok根据类所有final属性生成构造函数 即可完成Spring构造注入
public class TemplateServiceImpl implements TemplateService {
    private final TemplateMapper mapper;
    @Override
    public List<TemplateRespDto> list(TemplateReqDto param) {
        return mapper.selectManySelective(param) // 可根据实际情况将实体做转换
    }
}
```
:::info
实现类中也只需要重写`list`方法体，将实际业务场景中需要处理的业务逻辑处理和查询方法写入其中，并不需要关心分页功能
:::
```java
@Slf4j // 同上
@RestController // SpringBoot中注册Controller Bean的注解
@RequiredArgsConstructor // 同上
public class TemplateController {
    public final TemplateService service;
    /**
     * 分页查询
     *
     * @param pageParam 分页查询参数
     * @return 分页查询响应
     */
    @PostMapping(path = "page")
    public PageInfo<Result> page(@RequestBody PageParam<Param> pageParam) {
        return service.page(pageParam);
    }
    /**
     * 集合查询
     *
     * @param listParam 集合查询参数
     * @return 集合查询响应
     */
    @PostMapping(path = "list")
    public List<Result> list(@RequestBody Param listParam) {
        return service.list(listParam);
    }
}
```
:::info
最后编码`Controller`接口时，也只需要直接调用`service.page`即可，而请求参数直接用`PageParam`包装，将分页参数和业务参数分离，在前后端接口联调中，保持这种分离规范，可以很大程度上的降低沟通和开发成本
:::
<a name="EwmGH"></a>
#### FAQ
<a name="UV37u"></a>
##### 1、`BaseService`作为`interface`，`page`为什么可以声明方法体?
答：`Java8`中新特性之一就是为`interface`接口类增加了`static/default`方法，即声明方法后，其子类或实现都将默认具有这些方法，可以直接调用 而在此处为`Page`方法声明`default`是因为`page`函数只关注_分页参数和分页响应_，脱离了业务场景，方法体大相径庭，所以索性抽象定义出来，免去了其实现的复杂冗余过程
<a name="N0jPt"></a>
##### 2、`PageParam`的声明有什么意义?实现`IPage`是为了什么?
答：`PageParam`是参考`PageInfo`编写的类(不确定往后`PageHelper`是否会封装此类) 编写此类的目的就是为了分离分页和业务数据，让开发者专注于业务的实现和开发，同时也是对分页查询`API`的一种规范，无论是请求还是响应都将分页相关的数据抽离出来，单独使用 而实现`IPage`则是因为`IPage`作为`PageHelper`内置的`interface`，在不了解它更多意义上的作用前，可以作为分页参数声明的一种规范，而`IPage`中也只声明了三个方法，分别是`pageNum/pageSize/orderBy`的`Getter`方法，另外在源码分析中，将会提到实现此接口更深层的意义
<a name="BYvSr"></a>
##### 3、`PageParam`中除了常规的`pageNum/pageSize`，为什么还需要一个`orderBy`?
答：常规的分页查询中只需要`pageNum/pageSize`即可完成分页的目的，但是往往伴随着分页查询的还有筛选排序，而`orderBy`则是专注基于SQL的动态传参排序
<a name="aGCPP"></a>
##### 4、`orderBy`如何使用?会有什么问题吗?
答：`orderBy`和`pageNum/pageSize`一样，都是`Pagehelper`通过`MyBatis`拦截器，在query查询中注入进去的，所以在前端传参时，`orderBy`参数应为数据库`column desc/asc`这种形式，多字段排序则可以用逗号(,)拼接，譬如：`columnA desc,columnB`，但是另外一方面又存在两个问题，第一就是大多数数据库表字段设计中，都会使用蛇形case命名，而非常规开发中的驼峰case命名，所以存在一层转换，而这种转换可以分配给前端传参时，也可以分配给后端接参时。第二就是这样赤裸裸的将排序字段暴露在接口中，会存在**order by SQL注入**的风险，所以在实际使用过程中，需要通过某些手段去校验和排查`orderBy`的传参是否合法，譬如用正则表达式匹配参数值只能含有`order by`语法中必要的值，例如字段名，`desc or asc`，不允许包含特殊字符/数据库关键字等
<a name="yrc5Y"></a>
##### 5、`pageNum/pageSize`一定需要给默认值吗?
答：通过阅读**PageHelper**源码，得知在`Page`查询参数为null时，它并不会赋予它们默认值，并不进行额外的处理，以至于导致分页失败，而给默认值，也是为了谨防前后端调试接口过程中可能会出现的各种意外
<a name="xLsYY"></a>
### 3、源码分析
首先看`PageHelper.startPage(param)`过程中发生了什么：
```java
public static <E> Page<E> startPage(Object params) {
    Page<E> page = PageObjectUtil.getPageFromObject(params, true);
    Page<E> oldPage = getLocalPage();
    if (oldPage != null && oldPage.isOrderByOnly()) {
        page.setOrderBy(oldPage.getOrderBy());
    }
    setLocalPage(page);
    return page;
}
```
> 这是`PageHelper`继承(extend)的抽象类`PageMethod`中的一个静态方法

再看代码第一行 `Page<E> page = PageObjectUtil.getPageFromObject(params, true)`发生了什么：
```java
public static <T> Page<T> getPageFromObject(Object params, boolean required) {
    if (params == null) {
        throw new PageException("无法获取分页查询参数!");
    } else if (params instanceof IPage) {
        IPage pageParams = (IPage)params;
        Page page = null;
        if (pageParams.getPageNum() != null && pageParams.getPageSize() != null) {
            page = new Page(pageParams.getPageNum(), pageParams.getPageSize());
        }
        if (StringUtil.isNotEmpty(pageParams.getOrderBy())) {
            if (page != null) {
                page.setOrderBy(pageParams.getOrderBy());
            } else {
                page = new Page();
                page.setOrderBy(pageParams.getOrderBy());
                page.setOrderByOnly(true);
            }
        }
        return page;
    } else {
        ... // 此处只截取了部分代码片段, 以上是较为重要的一块
    }
}
```
可以看到在此方法中，会先判断`params`是否为null，再而通过`instanceof`判断是否为`IPage`的子类或实现类 如果以上两个`if/else` 皆不满足，则`PageHelper`则会在省略贴出的代码中通过大量的反射代码来获取`pageNum/pageSize`以及`orderBy`。总所皆知，反射在Java中虽然广泛应用，并且作为语言独有特性之一，深受广大开发者的喜爱，但是反射在某种程度上，是需要性能成本的，甚至于现阶段很多主流的框架和技术，都在尽量减少反射的运用，以防止框架性能过差，被市场淘汰。那么到此为止，也终于解释并知道了为什么`PageParam`要实现`IPage`接口了，在此处的代码中可以直接通过接口获取到分页参数，而不需要通过有损性能的反射获取`PageHelper`需要的参数<br />继续看`startPage`中的后续代码：
```java
public abstract class PageMethod {
    protected static final ThreadLocal<Page> LOCAL_PAGE = new ThreadLocal();
    protected static boolean DEFAULT_COUNT = true;
    public PageMethod() {
    }
    protected static void setLocalPage(Page page) {
        LOCAL_PAGE.set(page);
    }
    public static <T> Page<T> getLocalPage() {
        return (Page)LOCAL_PAGE.get();
    }
    ...
    ...
}
```
可以看到`PageHelper`继承的抽象类`PageMethod`中声明了一个`Page`的线程本地变量，而`getLocalPage()`则是为了获取当前线程中的`Page`而接下来`if (oldPage != null && oldPage.isOrderByOnly())`则是判断是否存在旧分页数据 此处的`isOrderByOnly`通过`getPageFromObject()`函数可以知道，当只存在`orderBy`参数时，即为`true`也就是说，当存在旧分页数据并且旧分页数据只有排序参数时，就将旧分页数据的排序参数列入新分页数据的排序参数 然后将新的分页数据`page`存入本地线程变量中 实际应用场景中，这种情况还是比较少，仅排序而不分页，所以某种角度上而言，仅当了解便好<br />接下来再看`doSelectPageInfo(ISelect)` 中发生了什么：
```java
public <E> PageInfo<E> doSelectPageInfo(ISelect select) {
    select.doSelect();
    return this.toPageInfo();
}
```
可以看到，该方法的实现非常简单明了，就是通过注册声明`ISelect`接口由开发自定义集合查询方式并由它内部执行，随后便返回`PageInfo`实体 前面有提到，`PageHelper`基于`MyBatis`拦截器达到分页的目的，那么为什么此处的`ISelect.doSelect()`执行，就可以返回`PageInfo`实体呢? 实际上这便是拦截器的妙用所在，在`select.doSelect()`执行时，会触发`PageHelper`自定义的`MyBatis`查询拦截器，并通过解析SQL和SQL参数，根据数据库类型，进行分页，譬如MySQL的`limit`，Oracle的`Rownum`等，同时还会在定义的查询SQL之前，`PageHelper`会重新生成一条`select count(*)`的SQL率先执行，已达到它定义`Page`内置分页参数的目的
```java
@Intercepts({@Signature(
    type = Executor.class,
    method = "query",
    args = {MappedStatement.class, Object.class, RowBounds.class, ResultHandler.class}
), @Signature(
    type = Executor.class,
    method = "query",
    args = {MappedStatement.class, Object.class, RowBounds.class, ResultHandler.class, CacheKey.class, BoundSql.class}
)})
public class PageInterceptor implements Interceptor {
    private volatile Dialect dialect;
    private String countSuffix = "_COUNT";
    protected Cache<String, MappedStatement> msCountMap = null;
    private String default_dialect_class = "com.github.pagehelper.PageHelper";
    public PageInterceptor() {
    }
    public Object intercept(Invocation invocation) throws Throwable {
    ...
    ...
    }
}
```
以上便是`PageHelper`内置的自定义**MyBatis**拦截器。
<a name="dO5s4"></a>
### 拓展
`PageHelper`不仅有`pageNum/pageSize/orderBy`这几个参数，更还有`pageSizeZero, reasonable`参数等用以更进阶的分页查询定义。
<a name="rPAbN"></a>
## 四、总结
PageHelper作为GitHub上现在近10K的开源分页框架，也许代码深度和广度不及主流市场框架和技术，虽然在功能的实现和原理上，造轮子的难度不高，源码也很清晰，但是在很大程度上解决了很多基于MyBatis的分页技术难题，简化并提示了广大开发者的效率，这才是开发者们在开发的路上应该向往并为之拼搏的方向和道路。作为受益者，也不应当仅仅是对其进行基本的使用，开发之余，也应该关注一些框架的拓展，对框架的底层有一定程度上的了解，并为之拓展和优化
