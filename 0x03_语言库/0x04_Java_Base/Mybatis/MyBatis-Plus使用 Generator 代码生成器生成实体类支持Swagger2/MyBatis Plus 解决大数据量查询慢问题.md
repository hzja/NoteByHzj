JavaMyBatis Plus<br />大数据量操作的场景大致如下：

- 数据迁移
- 数据导出
- 批量处理数据

在实际工作中当指定查询数据过大时，一般使用分页查询的方式一页一页的将数据放到内存处理。但有些情况不需要分页的方式查询数据或分很大一页查询数据时，如果一下子将数据全部加载出来到内存中，很可能会发生OOM(内存溢出)；而且查询会很慢，因为框架耗费大量的时间和内存去把数据库查询的结果封装成想要的对象（实体类）。<br />**举例：在业务系统需要从 MySQL 数据库里读取 100w 数据行进行处理，应该怎么做？**<br />做法通常如下：

- **常规查询：** 一次性读取 100w 数据到 JVM 内存中，或者分页读取
- **流式查询：** 建立长连接，利用服务端游标，每次读取一条加载到 JVM 内存（多次获取，一次一行）
- **游标查询：** 和流式一样，通过 fetchSize 参数，控制一次读取多少条数据（多次获取，一次多行）
<a name="qMPWU"></a>
## 常规查询
默认情况下，完整的检索结果集会将其存储在内存中。在大多数情况下，这是最有效的操作方式，并且由于 MySQL 网络协议的设计，因此更易于实现。<br />举例：<br />假设单表 100w 数据量，一般会采用分页的方式查询：
```java
@Mapper
public interface BigDataSearchMapper extends BaseMapper<BigDataSearchEntity> {
 
    @Select("SELECT bds.* FROM big_data_search bds ${ew.customSqlSegment} ")
    Page<BigDataSearchEntity> pageList(@Param("page") Page<BigDataSearchEntity> page, @Param(Constants.WRAPPER) QueryWrapper<BigDataSearchEntity> queryWrapper);
 
}
```
注：该示例使用的 MybatisPlus<br />该方式比较简单，如果在不考虑 LIMIT 深分页优化情况下，估计数据库服务器就崩了，或者能等上几十分钟或几小时，甚至几天时间检索数据
<a name="ZyMHj"></a>
## 流式查询
流式查询指的是查询成功后不是返回一个集合而是返回一个迭代器，应用每次从迭代器取一条查询结果。流式查询的好处是能够降低内存使用。<br />如果没有流式查询，想要从数据库取 100w 条记录而又没有足够的内存时，就不得不分页查询，而分页查询效率取决于表设计，如果设计的不好，就无法执行高效的分页查询。因此流式查询是一个数据库访问框架必须具备的功能。<br />MyBatis 中使用流式查询避免数据量过大导致 OOM ，但在流式查询的过程当中，数据库连接是保持打开状态的，因此要注意的是：

- 执行一个流式查询后，数据库访问框架就不负责关闭数据库连接了，需要应用在取完数据后自己关闭。
- 必须先读取（或关闭）结果集中的所有行，然后才能对连接发出任何其他查询，否则将引发异常。
<a name="aURjy"></a>
### MyBatis 流式查询接口
MyBatis 提供了一个叫 org.apache.ibatis.cursor.Cursor 的接口类用于流式查询，这个接口继承了 java.io.Closeable 和 java.lang.Iterable 接口，由此可知：

- Cursor 是可关闭的；
- Cursor 是可遍历的。

除此之外，Cursor 还提供了三个方法：

- `**isOpen()**`**：** 用于在取数据之前判断 Cursor 对象是否是打开状态。只有当打开时 Cursor 才能取数据；
- `**isConsumed()**`**：** 用于判断查询结果是否全部取完。
- `**getCurrentIndex()**`**：** 返回已经获取了多少条数据

使用流式查询，则要保持对产生结果集的语句所引用的表的并发访问，因为其 查询会独占连接，所以必须尽快处理
<a name="UnM9I"></a>
### 为什么要用流式查询？
如果有一个很大的查询结果需要遍历处理，又不想一次性将结果集装入客户端内存，就可以考虑使用流式查询；<br />分库分表场景下，单个表的查询结果集虽然不大，但如果某个查询跨了多个库多个表，又要做结果集的合并、排序等动作，依然有可能撑爆内存；详细研究了sharding-sphere的代码不难发现，除了group by与order by字段不一样之外，其他的场景都非常适合使用流式查询，可以最大限度的降低对客户端内存的消耗。
<a name="sMbvd"></a>
## 游标查询
对大量数据进行处理时，为防止内存泄漏情况发生，也可以采用游标方式进行数据查询处理。这种处理方式比常规查询要快很多。<br />当查询百万级的数据的时候，还可以使用游标方式进行数据查询处理，不仅可以节省内存的消耗，而且还不需要一次性取出所有数据，可以进行逐条处理或逐条取出部分批量处理。一次查询指定 fetchSize 的数据，直到把数据全部处理完。<br />Mybatis 的处理加了两个注解：`@Options` 和 `@ResultType`
```java
@Mapper
public interface BigDataSearchMapper extends BaseMapper<BigDataSearchEntity> {
 
    // 方式一 多次获取，一次多行
    @Select("SELECT bds.* FROM big_data_search bds ${ew.customSqlSegment} ")
    @Options(resultSetType = ResultSetType.FORWARD_ONLY, fetchSize = 1000000)
    Page<BigDataSearchEntity> pageList(@Param("page") Page<BigDataSearchEntity> page, @Param(Constants.WRAPPER) QueryWrapper<BigDataSearchEntity> queryWrapper);
 
    // 方式二 一次获取，一次一行
    @Select("SELECT bds.* FROM big_data_search bds ${ew.customSqlSegment} ")
    @Options(resultSetType = ResultSetType.FORWARD_ONLY, fetchSize = 100000)
    @ResultType(BigDataSearchEntity.class)
    void listData(@Param(Constants.WRAPPER) QueryWrapper<BigDataSearchEntity> queryWrapper, ResultHandler<BigDataSearchEntity> handler);
 
}
```
<a name="MWX0w"></a>
### `@Options`

- `ResultSet.FORWORD_ONLY`：结果集的游标只能向下滚动
- `ResultSet.SCROLL_INSENSITIVE`：结果集的游标可以上下移动，当数据库变化时，当前结果集不变
- `ResultSet.SCROLL_SENSITIVE`：返回可滚动的结果集，当数据库变化时，当前结果集同步改变
- `fetchSize`：每次获取量
<a name="SKmTA"></a>
### `@ResultType`

- `@ResultType(BigDataSearchEntity.class)`：转换成返回实体类型
> 注意：返回类型必须为 void ，因为查询的结果在 `ResultHandler` 里处理数据，所以这个 hander 也是必须的，可以使用 lambda 实现一个依次处理逻辑。

<a name="avzu7"></a>
### 注意：
虽然上面的代码中都有 `@Options` 但实际操作却有不同：

- 方式一是多次查询，一次返回多条；
- 方式二是一次查询，一次返回一条；
<a name="pb4kn"></a>
### 原因：
Oracle 是从服务器一次取出 fetch size 条记录放在客户端，客户端处理完成一个批次后再向服务器取下一个批次，直到所有数据处理完成。<br />MySQL 是在执行 `ResultSet.next()` 方法时，会通过数据库连接一条一条的返回。flush buffer 的过程是阻塞式的，如果网络中发生了拥塞，send buffer 被填满，会导致 buffer 一直 flush 不出去，那 MySQL 的处理线程会阻塞，从而避免数据把客户端内存撑爆。
<a name="lXAB0"></a>
### 非流式查询和流式查询区别：

- 非流式查询：内存会随着查询记录的增长而近乎直线增长。
- 流式查询：内存会保持稳定，不会随着记录的增长而增长。其内存大小取决于批处理大小`BATCH_SIZE`的设置，该尺寸越大，内存会越大。所以BATCH_SIZE应该根据业务情况设置合适的大小。

另外要切记每次处理完一批结果要记得释放存储每批数据的临时容器，即上文中的`gxids.clear();`
