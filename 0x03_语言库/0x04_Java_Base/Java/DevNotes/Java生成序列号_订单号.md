Java <br />用Java来生成序列号/订单号，列举几个在生活中比较常见的案例：

- 订单号
- 商品编号
- 交易单号
- 快递单号

**数据存储**使用的 **MySQL**。
<a name="tPRVe"></a>
## 理论知识
<a name="OQTai"></a>
### 什么是事务？
数据库**事务**（简称：事务）是数据库管理系统执行过程中的一个逻辑单位，由一个有限的数据库操作序列构成。
<a name="sx9ek"></a>
### 脏读、不可重复读、幻读
1、**脏读**：A 事务对数据修改但还没有提交到数据库，这个时候 B 事务来访问，那么 B 事务对数据就不是最新的，这种现象被成为脏读。<br />2、**不可重复读**：A 事务多次读取一个数据，这个时候在中途 B 事务修改了数据，导致 A 事务多次读到的结果不一致。<br />3、**幻读**：A 事务在前后两次查询同一个范围的时候、后一次查询看到了前一次查询未看到的行，因为 B 事务在后一次查询前新增加了一条数据。
<a name="R7rRz"></a>
### MySQL的四种隔离级别
按照隔离的级别由低到高，**越高的隔离，效率越差**，**不可重复读**，是 MySQL 的**默认隔离级别**。

| 事务隔离级别 | 脏读 | 不可重复读 | 幻读 |
| --- | --- | --- | --- |
| 读未提交(read-uncommitted) | 是 | 是 | 是 |
| 不可重复读(read-committed) | 否 | 是 | 是 |
| 可重复读(repeatable-read) | 否 | 否 | 是 |
| 串行化(serializable) | 否 | 否 | 否 |

1、**读未提交**：允许别的事务，去读取这个事务为提交之前的数据 缺点：可能会造成脏读、幻读、不可重复读。<br />2、**不可重复读**：并发条件下会出现问题，比如：A 用户读取数据, 随后 B 用户读出该数据并修改, 此时 A 用户再读取数据时发现前后两次的值不一致 缺点：可能会造成幻读、不可重复读。<br />3、**可重复读**：当使用可重复读隔离级别时，在事务执行期间会锁定该事务以任何方式引用的所有行。缺点：幻读<br />4、**串行化**：不会使用 **MySQL **的 mvcc 机制，在每一个 select 请求下获得读锁，在每一个 update 操作下尝试获得写锁。缺点：效率最差
<a name="aCaNs"></a>
### 两种悲观锁
**共享锁 (S 锁)**: 如果事务 T 对数据 A 加上共享锁后，则其他事务只能对 A 再加共享锁，不能加排他锁。获准共享锁的事务只能读数据，不能修改数据。<br />**排它锁 (X 锁)**：如果事务 T 对数据 A 加上排他锁后，则其他事务不能再对 A 加任任何类型的封锁。获准排他锁的事务既能读数据，又能修改数据。
<a name="Q1MPw"></a>
## Java 代码实现
```java
private  String createNewBidNumber() {
    //格式说明 CODE20201111xxx CODE+当前年月日+编号（具体长度看需求）
    String front="CODE";//前缀
    //当前时间编码
    Date date = new Date();
    String bidDate = new SimpleDateFormat("yyyyMMdd").format(date);
    Object bidService=null;//修改为自己的业务代码
    if (bidService != null){// 在数据表中查到了，说明现在这个订单不是今天的第一单
        String bid = bidService.getXXXX(); //取出ID，也就是业务号
        bid = bid.substring(10,13); // 取出后三位数，也就是自动生成的三位数 001
        int num = Integer.valueOf(bid);
        num ++;  // 加1
        if(num<10){
            String bidNum = String.format("%03d", num);//%03d 只是三位，不足补0
            String code = front+bidDate+bidNum;
            return code;
        }
        else if(num<100){
            String bidNum = String.format("%03d", num);//num<100,说明是两位数，前面要补一个0
            String code = front+bidDate+bidNum;
            return code;
        }
        else {
            String bidNum = String.valueOf(num);
            String code =front+bidDate+bidNum;
            return code;
        }
    }else {
        int number = 1;
        String bidNum = "00" + number;
        String code = front+bidDate+bidNum;
        return code;
    }
}
```
