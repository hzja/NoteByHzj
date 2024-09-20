Web 时区<br />在国际化的业务场景中，时区问题是常见的。<br />关于时区的概念，地球被划分为24个时区，北京时间为东八区，而美国的太平洋时间为西八区，和我们差了16个小时。<br />下面从一个案例说起，服务器和数据库部署在北京，而这时美国用户通过浏览器希望能查询北京时间下的“2020年7月1日8点-2020年7月1日18点”这10个小时的数据。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1598196368897-2501deb5-28ea-49fb-9cfe-1740be58e358.png#align=left&display=inline&height=54&originHeight=54&originWidth=484&size=0&status=done&style=shadow&width=484)<br />浏览器上选择时间区域查询数据<br />为了模拟浏览器在太平洋时间，只需将系统时间设置为太平洋时间即可。而系统时间的改变会影响到JVM的默认时区，所以为了让服务器程序仍处于北京时间，需要通过代码指定时区，如下：
```java
TimeZone.setDefault(TimeZone.getTimeZone("GMT+8"));
```
而数据库MySQL的时区也设置为北京时间，SQL如下：
```sql
set global time_zone = '+8:00';
set time_zone = '+8:00';
flush privileges;
```
下面，让点击查询，先看下发送的内容：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598197252678-441708b5-4df1-402f-bbd1-3342454dbba8.png#align=left&display=inline&height=106&originHeight=211&originWidth=1380&size=157931&status=done&style=shadow&width=690)<br />发送数据的格式<br />可以看到开始时间和结束时间都比界面上显示的时间多了8小时。这是因为使用的ElementUI组件的日期时间选择器，其默认时区为0时区，所以会将选择的时间根据浏览器的时区（西八区）转换成0时区的时间。最后传输的内容为时间+时区的字符串表示。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368852-ddbab6e0-66c3-4f44-a589-c2504da85040.webp#align=left&display=inline&height=340&originHeight=340&originWidth=938&size=0&status=done&style=shadow&width=938)时间-时区的字符串表示<br />前端把数据成功发出来了，下面看下后端接收数据的情况。后端使用的是SpringBoot，Controller的代码如下。
```java
@PostMapping("/time")
public List<Data> test(@RequestBody TimeDto dto) {
    Date startTime = dto.getStartTime();
    Date endTime = dto.getEndTime();
    System.out.println(startTime);
    System.out.println(endTime);
    // 格林时间（0）
    String format = "yyyy-MM-dd HH:mm:ss";
    SimpleDateFormat sdfGreen = new SimpleDateFormat(format);
    sdfGreen.setTimeZone(TimeZone.getTimeZone("GMT+0"));
    System.out.println("格林时间：" + sdfGreen.format(startTime) + "至" + sdfGreen.format(endTime));
    // 北京时间（+8）
    SimpleDateFormat sdfBeijing = new SimpleDateFormat(format);
    sdfBeijing.setTimeZone(TimeZone.getTimeZone("GMT+8"));
    System.out.println("北京时间：" + sdfBeijing.format(startTime) + "至" + sdfBeijing.format(endTime));
    // 太平洋时间（-8）
    SimpleDateFormat sdfPacific = new SimpleDateFormat(format);
    sdfPacific.setTimeZone(TimeZone.getTimeZone("GMT-8"));
    System.out.println("太平洋时间：" + sdfPacific.format(startTime) + "至" + sdfPacific.format(endTime));
    List<Data> dataList = queryDate(dto);
    return dataList;
}
/**
Thu Jul 02 00:00:00 GMT+08:00 2020
Thu Jul 02 10:00:00 GMT+08:00 2020
格林时间：2020-07-01 16:00:00至2020-07-02 02:00:00
北京时间：2020-07-02 00:00:00至2020-07-02 10:00:00
太平洋时间：2020-07-01 08:00:00至2020-07-01 18:00:00
**/
```
由于JVM时区为东八区，所以反序列化时得到的Date对象也是东八区的时间，即2号0点-2号10点。如果直接用startTime和endTime去查询，得到的将是北京时间2号0点到10点的数据，和预想的结果有差异。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368731-4b2e7df4-5c66-4133-a397-840d57237b96.webp#align=left&display=inline&height=703&originHeight=703&originWidth=1080&size=0&status=done&style=shadow&width=1080)时区问题导致的查询时间范围错误<br />那如何才能查询到北京时间1号8点-1号18点的数据呢。由于前端传输的太平洋时间在后台接收时发生时区转换，所以可以在前端直接传输需要查询的北京时间。也就是1号8点-1号18点。通过设置`el-date-picker`的`value-format`属性，指定选择的时间格式“yyyy-MM-dd HH:mm:ss”，这样传输的时间字符串将不具有时区属性。
```html
<el-date-picker
  v-model="dateTimeRange"
  type="datetimerange"
  range-separator="至"
  start-placeholder="开始日期"
  end-placeholder="结束日期"
  value-format="yyyy-MM-dd HH:mm:ss"
  >
</el-date-picker>
```
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368775-6bcb193c-b7d5-49ca-ae44-365d9dd513ec.webp#align=left&display=inline&height=85&originHeight=85&originWidth=479&size=0&status=done&style=shadow&width=479)<br />修正后的发送数据格式<br />而后端如果不修改，将报出以下错误，无法将该格式的时间转换成Date对象。
```
JSON parse error: Cannot deserialize value of type `java.util.Date` from String "2020-07-01 08:00:00": not a valid representation (error: Failed to parse Date value '2020-07-01 08:00:00': Cannot parse date "2020-07-01 08:00:00": while it seems to fit format 'yyyy-MM-dd'T'HH:mm:ss.SSSZ', parsing fails (leniency? null)); nested exception is com.fasterxml.jackson.databind.exc.InvalidFormatException: Cannot deserialize value of type `java.util.Date` from String "2020-07-01 08:00:00": not a valid representation (error: Failed to parse Date value '2020-07-01 08:00:00': Cannot parse date "2020-07-01 08:00:00": while it seems to fit format 'yyyy-MM-dd'T'HH:mm:ss.SSSZ', parsing fails (leniency? null))↵ at [Source: (PushbackInputStream); line: 1, column: 14] (through reference chain: com.chaycao.timezone.TimeDto["startTime"])
```
所以为能正确反序列化，需要为jackjson做反序列化提供额外的信息。加上`@JsonFormat`注解，指定时区和时间格式，便能达到期望的效果，得到的将是北京时间的1号8点和1号18点。所以，**在前后端传输发生的时区问题，注意时间数据的序列化和反序列化方式就能解决。**
```java
public class TimeDto {
    @JsonFormat(timezone = "GMT+8", pattern = "yyyy-MM-dd HH:mm:ss")
    Date startTime;
    @JsonFormat(timezone = "GMT+8", pattern = "yyyy-MM-dd HH:mm:ss")
    Date endTime;
    //...
}
```
下面再看下数据库中会发生的时区问题。<br />将MySQL的时区改为太平洋时间。
```sql
set global time_zone = '-8:00';
set time_zone = '-8:00';
flush privileges;
```
看下查询的结果是否会发生变化，查询的程序如下：
```java
private List<Data> queryDate(TimeDto dto) {
    DriverManagerDataSource dataSource = new DriverManagerDataSource();
    dataSource.setDriverClassName("com.mysql.cj.jdbc.Driver");
    dataSource.setUrl("jdbc:mysql://localhost/test?useSSL=false&useUnicode=true&characterEncoding=UTF8&allowPublicKeyRetrieval=true&serverTimezone=Asia/Shanghai");
    dataSource.setUsername("root");
    dataSource.setPassword("caoniezi");
    Date startTime = dto.getStartTime();
    Date endTime = dto.getEndTime();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "SELECT * FROM data WHERE create_time >= ? and create_time <= ?";
    List<Map<String, Object>> maps = jdbcTemplate.queryForList(
        sql,
        new Object[]{startTime, endTime});
    List<Data> dataList = new ArrayList<>();
    for (Map<String, Object> map : maps) {
        Data data = new Data();
        data.setId((Integer) map.get("id"));
        data.setContent((String) map.get("content"));
        data.setCreateTime((Date) map.get("create_time"));
        dataList.add(data);
    }
    return dataList;
}
```
查询的结果仍然是“D，E，F”，看来数据库时区的改变对于本次查询未产生影响。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368745-3d7c9686-fbcc-44c4-a443-b6f68913f7ab.webp#align=left&display=inline&height=772&originHeight=772&originWidth=1080&size=0&status=done&style=shadow&width=1080)修改MySQL时区后查询时间范围正确<br />这是因为在`create_time`字段的类型为datetime，而datetime是没有时区概念的，存储的是格式为YYYYMMDDHHMMSS（年月日时分秒）的整数，不会受到时区的影响。<br />而如果先将时区改回东八区，将`create_time`的类型改为timestamp，再把时区改为西八区。查询的结果是“H，I，J”。
```sql
set global time_zone = '+8:00';
set time_zone = '+8:00';
flush privileges; 
ALTER TABLE `data` MODIFY COLUMN `create_time` TIMESTAMP DEFAULT NULL;
set global time_zone = '-8:00';
set time_zone = '-8:00';
flush privileges;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598197295651-aeb467a5-c717-4c14-8105-ce3440487565.png#align=left&display=inline&height=573&originHeight=1145&originWidth=1644&size=362333&status=done&style=shadow&width=822)<br />修改create_time字段类型为timestamp<br />这是因为timestamp是有时区概念，存入的是自时间纪元以来的秒数，将类型改为timestamp时，`create_time`的值也会由东八区计算为0时区的时间秒数存储。当以西八区查询时，会减少16小时。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368941-a543f931-f89e-4a6c-9d94-ac1db8f8eee5.webp#align=left&display=inline&height=358&originHeight=358&originWidth=1080&size=0&status=done&style=shadow&width=1080)修改为timestamp后查询<br />那如何才能在西八区的数据库中查出想要的数据。<br />jdbc连接url中的serverTimezone参数，其作用是为驱动指定MySQL的时区，在之前的操作中，修改了MySQL的时区，而serverTimezone未修改，仍然是东八区。
```
jdbc:mysql://localhost/test?useSSL=false&useUnicode=true&characterEncoding=UTF8&allowPublicKeyRetrieval=true&serverTimezone=Asia/Shanghai
```
查询情况如下，MySQL驱动会根据指定的serverTimezone和JVM时区做转换，由于两者都是东八区，所以startTime和endTime的时间字符串不变，但是由于MySQL时区已变为西八区，查询结果就落到了H、I、J上。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368788-f19dc2cf-d091-4b1c-84eb-ec476e531601.webp#align=left&display=inline&height=411&originHeight=411&originWidth=1080&size=0&status=done&style=shadow&width=1080)serverTimezone为东八区的查询情况<br />下面把serverTimezone去掉，在未指定serverTimezone的情况下，驱动会根据MySQL的时区作为serverTimezone，然后做转换，这样得到的结果就是想要的。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598196368984-488d6187-7e75-4bf0-b6d6-cafb5d6da161.webp#align=left&display=inline&height=397&originHeight=397&originWidth=1080&size=0&status=done&style=shadow&width=1080)serverTimezone不指定的查询情况<br />但是这样做有一个问题，就是在查询datetime类型的数据时，也会发生转换，查询的结果将是30号16点到1号2点的数据。那么如何才能保证datetime类型、timestamp类型的数据都正确。首先serverTimezone是需要指定Asia/Shanghai的，不然datetime的数据会发生转换。而由于serverTimezone和MySQL时区不一致，查询的timestampe数据存在时区问题，所以最后的办法就是修改MySQL时区为东八区。**通过保证MySQL时区、serverTimezone和JVM时区三者一致，来保证时间数据读写的正确性**。
