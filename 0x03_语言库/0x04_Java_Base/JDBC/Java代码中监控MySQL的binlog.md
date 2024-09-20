Java MySQL<br />有这样一个业务场景，需要关注一个业务系统数据库中某几张表的数据，当数据发生新增或修改时，将它同步到另一个业务系统数据库中的表中。<br />一提到数据库的同步，估计第一时间想到的就是基于binlog的主从复制了，但是放在场景中，还有几个问题：

- 第一，并不是需要复制所有表的数据，复制对象只有少量的几张表
- 第二，也是比较麻烦的，两个业务系统数据库表结构可能不一致。例如，要同步数据库1的A表中的某些字段到数据库2的B表中，在这一过程中，A表和B表的字段并不是完全相同

这样的话，只能通过代码的方式，首先获取到数据库1表中数据的变动，再通过手动映射的方式，插入到数据库2的表中。但是，获取变动数据的这一过程，还是离不开binlog，因此就需要在代码中对binlog进行一下监控。<br />先说结论，最终使用了一个开源工具mysql-binlog-connector-java，用来监控binlog变化并获取数据，获取数据后再手动插入到另一个库的表中，基于它来实现了数据的同步。这个工具的git项目地址如下：<br />[https://github.com/shyiko/mysql-binlog-connector-java](https://github.com/shyiko/mysql-binlog-connector-java)<br />在正式开始前，还是先简单介绍一下mysql的binlog，binlog是一个二进制文件，它保存在磁盘中，是用来记录数据库表结构变更、表数据修改的二进制日志。其实除了数据复制外，它还可以实现数据恢复、增量备份等功能。<br />启动项目前，首先需要确保mysql服务已经启用了binlog：
```sql
show variables like 'log_bin';
```
如果为值为OFF，表示没有启用，那么需要首先启用binlog，修改配置文件：
```sql
log_bin=mysql-bin
binlog-format=ROW
server-id=1
```
对参数做一个简要说明：

- 在配置文件中加入了log_bin配置项后，表示启用了binlog
- binlog-format是binlog的日志格式，支持三种类型，分别是`STATEMENT`、`ROW`、`MIXED`，在这里使用ROW模式
- server-id用于标识一个sql语句是从哪一个server写入的，这里一定要进行设置，否则在后面的代码中会无法正常监听到事件

在更改完配置文件后，重启mysql服务。再次查看是否启用binlog，返回为`ON`，表示已经开启成功。<br />在Java项目中，首先引入maven坐标：
```xml
<dependency>
    <groupId>com.github.shyiko</groupId>
    <artifactId>mysql-binlog-connector-java</artifactId>
    <version>0.21.0</version>
</dependency>
```
写一段简单的示例，看看它的具体使用方式：
```java
public static void main(String[] args) {
    BinaryLogClient client = new BinaryLogClient("127.0.0.1", 3306, "hydra", "123456");
    client.setServerId(2);

    client.registerEventListener(event -> {
        EventData data = event.getData();
        if (data instanceof TableMapEventData) {
            System.out.println("Table:");
            TableMapEventData tableMapEventData = (TableMapEventData) data;
            System.out.println(tableMapEventData.getTableId()+": ["+tableMapEventData.getDatabase() + "-" + tableMapEventData.getTable()+"]");
        }
        if (data instanceof UpdateRowsEventData) {
            System.out.println("Update:");
            System.out.println(data.toString());
        } else if (data instanceof WriteRowsEventData) {
            System.out.println("Insert:");
            System.out.println(data.toString());
        } else if (data instanceof DeleteRowsEventData) {
            System.out.println("Delete:");
            System.out.println(data.toString());
        }
    });

    try {
        client.connect();
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```
首先，创建一个`BinaryLogClient`客户端对象，初始化时需要传入`mysql`的连接信息，创建完成后，给客户端注册一个监听器，来实现它对`binlog`的监听和解析。在监听器中，暂时只对4种类型的事件数据进行了处理，除了`WriteRowsEventData`、`DeleteRowsEventData`、`UpdateRowsEventData`对应增删改操作类型的事件数据外，还有一个`TableMapEventData`类型的数据，包含了表的对应关系，在后面的例子中再具体说明。<br />在这里，客户端监听到的是数据库级别的所有事件，并且可以监听到表的`DML`语句和`DDL`语句，所以只需要处理关心的事件数据就行，否则会收到大量的冗余数据。<br />启动程序，控制台输出：
```
com.github.shyiko.mysql.binlog.BinaryLogClient openChannelToBinaryLogStream
信息: Connected to 127.0.0.1:3306 at mysql-bin.000002/1046 (sid:2, cid:10)
```
连接mysql的`binlog`成功，接下来，在数据库中插入一条数据，这里操作的数据库名字是`tenant`，表是`dept`：
```sql
insert into dept VALUES(8,"人力","","1");
```
这时，控制台就会打印监听到事件的数据：
```
Table:
108: [tenant-dept]
Insert:
WriteRowsEventData{tableId=108, includedColumns={0, 1, 2, 3}, rows=[
    [8, 人力, , 1]
]}
```
监听到的事件类型数据有两类，第一类是`TableMapEventData`，通过它可以获取操作的数据库名称、表名称以及表的id。之所以要监听这个事件，是因为之后监听的实际操作中返回数据中包含了表的`id`，而没有表名等信息，所以如果想知道具体的操作是在哪一张表的话，就要先维护一个`id`与表的对应关系。<br />第二个打印出来的监听事件数据是`WriteRowsEventData`，其中记录了`insert`语句作用的表，插入涉及到的列，以及实际插入的数据。另外，如果只需要对特定的一张或几张表进行处理的话，也可以提前设置表的名单，在这里根据表id到表名的映射关系，实现数据的过滤，<br />接下来，再执行一条`update`语句：
```sql
update dept set tenant_id=3 where id=8 or id=9
```
控制台输出：
```
Table:
108: [tenant-dept]
Update:
UpdateRowsEventData{tableId=108, includedColumnsBeforeUpdate={0, 1, 2, 3}, includedColumns={0, 1, 2, 3}, rows=[
    {before=[8, 人力, , 1], after=[8, 人力, , 3]},
    {before=[9, 人力, , 1], after=[9, 人力, , 3]}
]}
```
在执行`update`语句时，可能会作用于多条数据，因此在实际修改的数据中，可能包含多行记录，这一点体现在上面的`rows`中，包含了`id`为8和9的两条数据。<br />最后，再执行一条`delete`语句：
```sql
delete from dept where tenant_id=3
```
控制台打印如下，`rows`中同样返回了生效的两条数据：
```
Table:
108: [tenant-dept]
Delete:
DeleteRowsEventData{tableId=108, includedColumns={0, 1, 2, 3}, rows=[
    [8, 人力, , 3],
    [9, 人力, , 3]
]}
```
简单的使用原理介绍完成后，再回到原先的需求上，需要将一张表中新增或修改的数据同步到另一张表中，问题还有一个，就是如何将返回的数据对应到所在的列上。这时应该怎么实现呢？<br />以`update`操作为例，要对提取的数据后进行一下处理，更改上面例子中的方法：
```java
if (data instanceof UpdateRowsEventData) {
    System.out.println("Update:");
    UpdateRowsEventData updateRowsEventData = (UpdateRowsEventData) data;
    for (Map.Entry<Serializable[], Serializable[]> row : updateRowsEventData.getRows()) {
        List<Serializable> entries = Arrays.asList(row.getValue());
        System.out.println(entries);
        JSONObject dataObject = getDataObject(entries);
        System.out.println(dataObject);
    }
}
```
在将`data`强制转换为`UpdateRowsEventData`后，可以使用`getRows`方法获取到更新的行数据，并且能够取到每一列的值。<br />之后，调用了一个自己实现的`getDataObject`方法，用它来实现数据到列的绑定过程：
```java
private static JSONObject getDataObject(List message) {
    JSONObject resultObject = new JSONObject();
    String format = "{\"id\":\"0\",\"dept_name\":\"1\",\"comment\":\"2\",\"tenant_id\":\"3\"}";
    JSONObject json = JSON.parseObject(format);
    for (String key : json.keySet()) {
        resultObject.put(key, message.get(json.getInteger(key)));
    }
    return resultObject;
}
```
在`format`字符串中，提前维护了一个数据库表的字段顺序的字符串，标识了每个字段位于顺序中的第几个位置。通过上面这个函数，能够实现数据到列的填装过程，再执行一条`update`语句来查看一下结果：
```sql
update dept set tenant_id=3,comment="1" where id=8
```
控制台打印结果如下：
```
Table:
108: [tenant-dept]
Update:
[8, 人力, 1, 3]
{"tenant_id":3,"dept_name":"人力","comment":"1","id":8}
```
可以看到，将修改后的这一条记录中的属性填装到了它对应的列中，之后再根据具体的业务逻辑，就可以根据字段名取出数据，将数据同步到其他的表了。
