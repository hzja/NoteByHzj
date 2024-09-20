Java 
<a name="bRaXq"></a>
## 需求说明
由实施 / 用户 将别的系统的数据填入系统中的 Excel 模板，应用将文件内容读取、校对、转换之后产生欠费数据、票据、票据详情并存储到数据库中。导入 Excel 的需求在系统中还是很常见的。
<a name="XOGpb"></a>
### 一些细节

- 数据导入：导入使用的模板由系统提供，格式是 xlsx (支持 65535+行数据) ，用户按照表头在对应列写入相应的数据
- 数据校验：数据校验有两种：
   - 字段长度、字段正则表达式校验等，内存内校验不存在外部数据交互。对性能影响较小
   - 数据重复性校验，如票据号是否和系统已存在的票据号重复(需要查询数据库，十分影响性能)
- 数据插入：测试环境数据库使用 MySQL 5.7，未分库分表，连接池使用 Druid
<a name="Lt9qq"></a>
## 迭代记录
<a name="LD2tB"></a>
### 第一版：POI + 逐行查询校对 + 逐行插入
这个版本是最古老的版本，采用原生 POI，手动将 Excel 中的行映射成 ArrayList 对象，然后存储到 List ，代码执行的步骤如下：

1. 手动读取 Excel 成 List
2. 循环遍历，在循环中进行以下步骤
   1. 检验字段长度
   2. 一些查询数据库的校验，比如校验当前行欠费对应的房屋是否在系统中存在，需要查询房屋表
   3. 写入当前行数据
3. 返回执行结果，如果出错 / 校验不合格。则返回提示信息并回滚数据

显而易见的，这样实现一定是赶工赶出来的，后续可能用的少也没有察觉到性能问题，但是它最多适用于个位数/十位数级别的数据。存在以下明显的问题：

- 查询数据库的校验对每一行数据都要查询一次数据库，应用访问数据库来回的网络IO次数被放大了 n 倍，时间也就放大了 n 倍
- 写入数据也是逐行写入的，问题和上面的一样
- 数据读取使用原生 POI，代码十分冗余，可维护性差。
<a name="oRwAD"></a>
### 第二版：EasyPOI + 缓存数据库查询操作 + 批量插入
针对第一版分析的三个问题，分别采用以下三个方法优化
<a name="JQmHY"></a>
#### 缓存数据，以空间换时间
逐行查询数据库校验的时间成本主要在来回的网络IO中，优化方法也很简单。将参加校验的数据全部缓存到 HashMap 中。直接到 HashMap 去命中。<br />例如：校验行中的房屋是否存在，原本是要用 区域 + 楼宇 + 单元 + 房号 去查询房屋表匹配房屋ID，查到则校验通过，生成的欠单中存储房屋ID，校验不通过则返回错误信息给用户。而房屋信息在导入欠费的时候是不会更新的。并且一个小区的房屋信息也不会很多(5000以内)因此采用一条SQL，将该小区下所有的房屋以 区域/楼宇/单元/房号 作为 key，以 房屋ID 作为 value，存储到 HashMap 中，后续校验只需要在 HashMap 中命中
<a name="IQPbt"></a>
#### 自定义 SessionMapper
Mybatis 原生是不支持将查询到的结果直接写人一个 HashMap 中的，需要自定义 `SessionMapper`<br />`SessionMapper` 中指定使用 `MapResultHandler` 处理 SQL 查询的结果集
```java
@Repository
public class SessionMapper extends SqlSessionDaoSupport {

    @Resource
    public void setSqlSessionFactory(SqlSessionFactory sqlSessionFactory) {
        super.setSqlSessionFactory(sqlSessionFactory);
    }

    // 区域楼宇单元房号 - 房屋ID
    @SuppressWarnings("unchecked")
    public Map<String, Long> getHouseMapByAreaId(Long areaId) {
        MapResultHandler handler = new MapResultHandler();

 this.getSqlSession().select(BaseUnitMapper.class.getName()+".getHouseMapByAreaId", areaId, handler);
        Map<String, Long> map = handler.getMappedResults();
        return map;
    }
}
```
`MapResultHandler` 处理程序，将结果集放入 `HashMap`
```java
public class MapResultHandler implements ResultHandler {
    private final Map mappedResults = new HashMap();

    @Override
    public void handleResult(ResultContext context) {
        @SuppressWarnings("rawtypes")
        Map map = (Map)context.getResultObject();
        mappedResults.put(map.get("key"), map.get("value"));
    }

    public Map getMappedResults() {
        return mappedResults;
    }
}
```
示例 Mapper
```java
@Mapper
@Repository 
public interface BaseUnitMapper {
    // 收费标准绑定 区域楼宇单元房号 - 房屋ID
    Map<String, Long> getHouseMapByAreaId(@Param("areaId") Long areaId);
}
```
示例 Mapper.xml
```xml
<select id="getHouseMapByAreaId" resultMap="mapResultLong">
    SELECT
        CONCAT( h.bulid_area_name, h.build_name, h.unit_name, h.house_num ) k,
        h.house_id v
    FROM
        base_house h
    WHERE
        h.area_id = #{areaId}
    GROUP BY
        h.house_id
</select>
            
<resultMap id="mapResultLong" type="java.util.HashMap">
    <result property="key" column="k" javaType="string" jdbcType="VARCHAR"/>
    <result property="value" column="v" javaType="long" jdbcType="INTEGER"/>
</resultMap>
```
之后在代码中调用 SessionMapper 类对应的方法即可。
<a name="i3jXF"></a>
#### 使用 values 批量插入
MySQL insert 语句支持使用 `values (),(),()` 的方式一次插入多行数据，通过 mybatis foreach 结合 java 集合可以实现批量插入，代码写法如下：
```xml
<insert id="insertList">
    insert into table(colom1, colom2)
    values
    <foreach collection="list" item="item" index="index" separator=",">
     ( #{item.colom1}, #{item.colom2})
    </foreach>
</insert>
```
<a name="Baj3V"></a>
#### 使用 EasyPOI 读写 Excel
EasyPOI 采用基于注解的导入导出，修改注解就可以修改Excel，非常方便，代码维护起来也容易。
<a name="TV3la"></a>
### 第三版：EasyExcel + 缓存数据库查询操作 + 批量插入
第二版采用 EasyPOI 之后，对于几千、几万的 Excel 数据已经可以轻松导入了，不过耗时有点久(5W 数据 10分钟左右写入到数据库)不过由于后来导入的操作基本都是开发在一边看日志一边导入，也就没有进一步优化。但是好景不长，有新的数据需要迁入，票据 Excel 有 41w 行，这个时候使用 EasyPOI 在开发环境跑直接就 OOM 了，增大 JVM 内存参数之后，虽然不 OOM 了，但是 CPU 占用 100% 20 分钟仍然未能成功读取全部数据。故在读取大 Excel 时需要再优化速度。先上 GITHUB 找找别的开源项目。这时阿里 EasyExcel 映入眼帘：<br />![2021-05-14-23-09-07-348824.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621005366060-687ff80c-b269-4283-a816-421f5e006387.png#clientId=uf418a4cb-cf76-4&from=ui&id=ufb905e53&originHeight=413&originWidth=755&originalType=binary&size=47418&status=done&style=shadow&taskId=u96aa499d-abc8-4061-9db1-d6abde241d3)<br />EasyExcel 采用和 EasyPOI 类似的注解方式读写 Excel，因此从 EasyPOI 切换过来很方便，分分钟就搞定了。也确实如阿里大神描述的：41w行、25列、45.5m 数据读取平均耗时 50s，因此对于大 Excel 建议使用 EasyExcel 读取。
<a name="EBZAe"></a>
### 第四版：优化数据插入速度
在第二版插入的时候，使用了 values 批量插入代替逐行插入。每 30000 行拼接一个长 SQL、顺序插入。整个导入方法这块耗时最多。后来将每次拼接的行数减少到 10000、5000、3000、1000、500 发现执行最快的是 1000。结合网上一些对 `innodb_buffer_pool_size` 描述猜是因为过长的 SQL 在写操作的时候由于超过内存阈值，发生了磁盘交换。限制了速度，另外测试服务器的数据库性能也不怎么样，过多的插入也处理不过来。所以最终采用每次 1000 条插入。<br />每次 1000 条插入后，为了榨干数据库的 CPU，那么网络IO的等待时间就需要利用起来，这个需要多线程来解决，而最简单的多线程可以使用 并行流 来实现，接着将代码用并行流来测试了一下：<br />10w行的 excel、42w 欠单、42w记录详情、2w记录、16 线程并行插入数据库、每次 1000 行。插入时间 72s，导入总时间 95 s。<br />![2021-05-14-23-09-07-501816.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621005434804-99e5475c-bafa-4cc6-9440-7677c9f652a6.png#clientId=uf418a4cb-cf76-4&from=ui&id=u77092666&originHeight=183&originWidth=1004&originalType=binary&size=59545&status=done&style=none&taskId=u9d81ca86-9079-4199-ada6-4fc11368f6b)
<a name="mKdQo"></a>
#### 并行插入工具类
并行插入的代码封装了一个函数式编程的工具类
```java
/**
 * 功能：利用并行流快速插入数据
 *
 * @date 2020/7/1 9:25
 */
public class InsertConsumer {
    /**
     * 每个长 SQL 插入的行数，可以根据数据库性能调整
     */
    private final static int SIZE = 1000;

    /**
     * 如果需要调整并发数目，修改下面方法的第二个参数即可
     */
    static {
        System.setProperty("java.util.concurrent.ForkJoinPool.common.parallelism", "4");
    }

    /**
     * 插入方法
     *
     * @param list     插入数据集合
     * @param consumer 消费型方法，直接使用 mapper::method 方法引用的方式
     * @param <T>      插入的数据类型
     */
    public static <T> void insertData(List<T> list, Consumer<List<T>> consumer) {
        if (list == null || list.size() < 1) {
            return;
        }

        List<List<T>> streamList = new ArrayList<>();

        for (int i = 0; i < list.size(); i += SIZE) {
            int j = Math.min((i + SIZE), list.size());
            List<T> subList = list.subList(i, j);
            streamList.add(subList);
        }
        // 并行流使用的并发数是 CPU 核心数，不能局部更改。全局更改影响较大，斟酌
        streamList.parallelStream().forEach(consumer);
    }
}
```
这里多数使用到很多 Java8 的API方法使用起来很简单。
```java
InsertConsumer.insertData(feeList, arrearageMapper::insertList);
```
<a name="r3jKZ"></a>
## 其他影响性能的内容
<a name="K33nU"></a>
### 日志
避免在 for 循环中打印过多的 info 日志<br />在优化的过程中，还发现了一个特别影响性能的东西：info 日志，还是使用 41w行、25列、45.5m 数据，在 开始-数据读取完毕 之间每 1000 行打印一条 info 日志，缓存校验数据-校验完毕 之间每行打印 3+ 条 info 日志，日志框架使用 Slf4j 。打印并持久化到磁盘。下面是打印日志和不打印日志效率的差别<br />打印日志<br />![2021-05-14-23-09-07-637877.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621005479318-3436beb8-441d-434c-a404-77cc127b4cbc.png#clientId=uf418a4cb-cf76-4&from=ui&id=ua67df99e&originHeight=71&originWidth=1007&originalType=binary&size=23425&status=done&style=none&taskId=u5833a391-7fac-416c-b158-279f1d895ff)<br />不打印日志<br />![2021-05-14-23-09-07-753869.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621005488229-97d13704-626f-427a-b68d-6a3be6bbc2b7.png#clientId=uf418a4cb-cf76-4&from=ui&id=u978bb882&originHeight=67&originWidth=997&originalType=binary&size=23226&status=done&style=none&taskId=u8fb91b4e-d95c-42ff-90af-405bf210a2b)<br />缓存校验数据-校验完毕 不打印日志耗时仅仅是打印日志耗时的 1/10 ！
<a name="Wo7Om"></a>
### 总结
提升Excel导入速度的方法：

- 使用更快的 Excel 读取框架(推荐使用阿里 EasyExcel)
- 对于需要与数据库交互的校验、按照业务逻辑适当的使用缓存。用空间换时间
- 使用 `values(),(),()` 拼接长 SQL 一次插入多行数据
- 使用多线程插入数据，利用掉网络IO等待时间(推荐使用并行流，简单易用)
- 避免在循环中打印无用的日志
