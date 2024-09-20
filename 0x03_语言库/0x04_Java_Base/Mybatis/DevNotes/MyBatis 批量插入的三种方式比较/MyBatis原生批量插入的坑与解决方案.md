JavaMyBatis<br />使用 「循环单次插入」的性能太低，使用「MyBatis Plus 批量插入」性能还行，但要额外的引入 MyBatis Plus 框架，使用「MyBatis 原生批量插入」性能最好，但在插入大量数据时会导致程序报错，那么，这里会提供一个更优的解决方案。
<a name="NDFm0"></a>
## 原生批量插入的“坑”
首先，来看一下 MyBatis 原生批量插入中的坑，当批量插入 10 万条数据时，实现代码如下：
```java
import com.example.demo.model.User;
import com.example.demo.service.impl.UserServiceImpl;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.ArrayList;
import java.util.List;

@SpringBootTest
class UserControllerTest {

    // 最大循环次数
    private static final int MAXCOUNT = 100000;

    @Autowired
    private UserServiceImpl userService;
    
    /**
     * 原生自己拼接 SQL，批量插入
     */
    @Test
    void saveBatchByNative() {
        long stime = System.currentTimeMillis(); // 统计开始时间
        List<User> list = new ArrayList<>();
        for (int i = 0; i < MAXCOUNT; i++) {
            User user = new User();
            user.setName("test:" + i);
            user.setPassword("123456");
            list.add(user);
        }
        // 批量插入
        userService.saveBatchByNative(list);
        long etime = System.currentTimeMillis(); // 统计结束时间
        System.out.println("执行时间：" + (etime - stime));
    }
}
```
核心文件 UserMapper.xml 中的实现代码如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.example.demo.mapper.UserMapper">
  <insert id="saveBatchByNative">
    INSERT INTO `USER`(`NAME`,`PASSWORD`) VALUES
    <foreach collection="list" separator="," item="item">
      (#{item.name},#{item.password})
    </foreach>
  </insert>

</mapper>
```
当开心地运行以上程序时，就出现了以下的一幕：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664029160620-6cfac433-578a-428d-80a3-6b3db9acb25a.png#clientId=u94bb1212-2c42-4&errorMessage=unknown%20error&from=paste&id=ue8984642&originHeight=444&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u63336ef5-f805-4a3b-b9a3-a3918d9c6cc&title=)程序竟然报错了！<br />这是因为使用 MyBatis 原生批量插入拼接的插入 SQL 大小是 4.56M，而默认情况下 MySQL 可以执行的最大 SQL 为 4M，那么在程序执行时就会报错了。
<a name="MHRv0"></a>
## 解决方案
以上的问题就是因为批量插入时拼接的 SQL 文件太大了，所以导致 MySQL 的执行报错了。那么第一时间想到的解决方案就是将大文件分成 N 个小文件，这样就不会因为 SQL 太大而导致执行报错了。也就是说，可以将待插入的 List 集合分隔为多个小 List 来执行批量插入的操作，而这个操作过程就叫做 List 分片。<br />有了处理思路之后，接下来就是实操了，那如何对集合进行分片操作呢？<br />分片操作的实现方式有很多种，这个后面再说，接下来使用最简单的方式，也就是 Google 提供的 Guava 框架来实现分片的功能。
<a name="YuqWU"></a>
### 分片 Demo 实战
要实现分片功能，第一步先要添加 Guava 框架的支持，在 pom.xml 中添加以下引用：
```xml
<!-- google guava 工具类 -->
<!-- https://mvnrepository.com/artifact/com.google.guava/guava -->
<dependency>
  <groupId>com.google.guava</groupId>
  <artifactId>guava</artifactId>
  <version>31.0.1-jre</version>
</dependency>
```
接下来写一个小小的 demo，将以下 7 个人名分为 3 组（每组最多 3 个），实现代码如下：
```java
import com.google.common.collect.Lists;

import java.util.Arrays;
import java.util.List;

/**
 * Guava 分片
 */
public class PartitionByGuavaExample {
    // 原集合
    private static final List<String> OLD_LIST = Arrays.asList(
            "唐僧,悟空,八戒,沙僧,曹操,刘备,孙权".split(","));

    public static void main(String[] args) {
        // 集合分片
        List<List<String>> newList = Lists.partition(OLD_LIST, 3);
        // 打印分片集合
        newList.forEach(i -> {
            System.out.println("集合长度：" + i.size());
        });
    }
}
```
以上程序的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664029160632-98acf010-7379-48e0-a08f-8c3e493eb26d.png#clientId=u94bb1212-2c42-4&errorMessage=unknown%20error&from=paste&id=u4ff3ea39&originHeight=308&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u37b8059c-9bfb-436c-a270-d7dba5227a4&title=)从上述结果可以看出，只需要使用 Guava 提供的 `Lists.partition` 方法就可以很轻松的将一个集合进行分片了。
<a name="dNLAq"></a>
### 原生批量插入分片实现
那接下来，就是改造 MyBatis 批量插入代码了，具体实现如下：
```java
@Test
void saveBatchByNativePartition() {
    long stime = System.currentTimeMillis(); // 统计开始时间
    List<User> list = new ArrayList<>();
    // 构建插入数据
    for (int i = 0; i < MAXCOUNT; i++) {
        User user = new User();
        user.setName("test:" + i);
        user.setPassword("123456");
        list.add(user);
    }
    // 分片批量插入
    int count = (int) Math.ceil(MAXCOUNT / 1000.0); // 分为 n 份，每份 1000 条
    List<List<User>> listPartition = Lists.partition(list, count);
    // 分片批量插入
    for (List<User> item : listPartition) {
        userService.saveBatchByNative(item);
    }
    long etime = System.currentTimeMillis(); // 统计结束时间
    System.out.println("执行时间：" + (etime - stime));
}
```
执行以上程序，最终的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664029160769-e2dbe46a-f169-4b80-9ec2-3cd0e4372909.png#clientId=u94bb1212-2c42-4&errorMessage=unknown%20error&from=paste&id=u2e9dcc77&originHeight=469&originWidth=1014&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=uc1f229d3-ad17-428f-8013-3b8a0621c77&title=)<br />从上图可以看出，之前批量插入时的异常报错不见了，并且此实现方式的执行效率竟比 MyBatis Plus 的批量插入的执行效率要高，MyBatis Plus 批量插入 10W 条数据的执行时间如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664029160743-336a2efd-9b55-46dd-a3b9-5bf94b07df5a.png#clientId=u94bb1212-2c42-4&errorMessage=unknown%20error&from=paste&id=u6e1f2a74&originHeight=231&originWidth=771&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=u5f758644-7d6a-4e79-9dad-0739020729b&title=)
<a name="xxepq"></a>
## 总结
这里演示了 MyBatis 原生批量插入时的问题：可能会因为插入的数据太多从而导致运行失败，可以通过分片的方式来解决此问题，分片批量插入的实现步骤如下：

1. 计算出分片的数量（分为 N 批）；
2. 使用 `Lists.partition` 方法将集合进行分片（分为 N 个集合）；
3. 循环将分片的集合进行批量插入的操作。
