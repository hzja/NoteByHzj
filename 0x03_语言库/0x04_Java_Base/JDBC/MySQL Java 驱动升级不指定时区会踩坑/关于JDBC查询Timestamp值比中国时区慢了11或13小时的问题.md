JavaJDBC
<a name="YqIXX"></a>
## 问题背景
在写测试用例的时候发现JdbcTemplate查询出来的时间和预期时间相比相差11小时。于是猜想这肯定是服务器时区问题。然后在url上加了`serverTimeZone`参数，设置为上海时间，问题就好了。但是这样的问题在于公司项目其他地方的url也要都加上该参数，一个个修改就很麻烦。
<a name="BlcyX"></a>
## 解决方案
去设置MySQL数据库的时区即可。<br />首先，`**show global **variables **like **'%time_zone%';`查询MySQL数据库的时区结果如下：<br />![DM_20230418231620_001.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1681831011029-175fc8e7-99c5-4668-ae32-1cbd5f7d83fd.png#averageHue=%23110f0f&clientId=u2c89c167-b575-4&from=ui&id=ue2f219f6&originHeight=192&originWidth=613&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=13667&status=done&style=none&taskId=u58c005ef-c167-4ef1-ad70-2429f293c71&title=)<br />发现time_zone是SYSTEM。此时，就应该设置时区：
```bash
set global variables time_zone='+8:00';
```
问题分析-为什么会相差11小时呢？<br />查看JDBC源码即可。<br />首先，JDBC是使用`resultSet.getTimeStamp`方法获取类型为日期时间的字段的值，代码如下：
```java
@Override
public Timestamp getTimestamp(int columnIndex) throws SQLException {
    checkRowPos();
    checkColumnBounds(columnIndex);
    return this.thisRow.getValue(columnIndex - 1, this.defaultTimestampValueFactory);
}
```
可以看到这一行：
```java
this.thisRow.getValue(columnIndex - 1, this.defaultTimestampValueFactory)
```
它使用了`defaultTimestampValueFactory`变量，这是类的一个属性：
```java
private ValueFactory<Timestamp> defaultTimestampValueFactory;
```
在构造函数中初始化的代码：
```java
this.defaultTimestampValueFactory = new SqlTimestampValueFactory(pset, null, this.session.getServerSession().getDefaultTimeZone());
```
进到`getDefaultTimeZone`方法中，具体是`NativeServerSession`类，在该类发现了`setServerTimeZone`方法，然后在该方法上打上断点：
```java
public void setServerTimeZone(TimeZone serverTimeZone) {
    this.serverTimeZone = serverTimeZone;
}
```
debug过来，查看谁调用了该方法，发现是`NativeProtocol`的`configureTimezone`方法：<br />顾名思义，配置时区，所以，`NativeProtocol`的`configureTimezone`方法就是JDBC配置时区的。<br />`configureTimezone`方法的逻辑<br />该方法有如下代码：
```java
String configuredTimeZoneOnServer = this.serverSession.getServerVariable("time_zone");
if ("SYSTEM".equalsIgnoreCase(configuredTimeZoneOnServer)) {
    configuredTimeZoneOnServer = this.serverSession.getServerVariable("system_time_zone");
}
```
代码逻辑：<br />如果发现`time_zone`是SYSTEM，就依据`system_time_zone`的值。<br />但是，分析后面的逻辑发现如果`system_time_zone`的值是CST，最终会使用美国芝加哥的时间，它和中国时间相差13个小时。<br />通过之前`show global variables like '%time_zone%';`发现情况刚好符合这个逻辑，所以，查询出来的timestamp值和中国时间对不上。至于为什么是相差11小时而不是13小时就清楚原因了，总之，将数据库时区从SYSTEM修改为'+8:00'就能解决。
<a name="Cd3yh"></a>
## 补充
这里的MySQL的JDBC驱动mysql-connector-java版本是8.0.18的，使用的驱动类是com.mysql.cj.jdbc.Driver。
