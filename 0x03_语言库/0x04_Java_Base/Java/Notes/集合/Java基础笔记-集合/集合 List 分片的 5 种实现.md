JavaList<br />在实现 MyBatis 批量插入时遇到了一个问题，当批量插入的数据量比较大时，会导致程序执行报错，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034179-075b0b4a-38bf-47f6-8274-336aa741ad50.png#clientId=ua74ad660-aff4-4&from=paste&id=u5d82d9cd&originHeight=444&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u940848dc-c46d-42fa-92ff-bb979a49cc2&title=)原因是 MySQL 只能执行一定长度的 SQL 语句，但当插入的数据量较多时，会生成一条很长的 SQL，这样程序在执行时就会报错。<br />要解决这个问题，有两种方法：第一，设置 MySQL 可以执行 SQL 的最大长度；第二，将一个大 List 分成 N 个小 List 进行。由于无法准确的界定程序中最大的 SQL 长度，所以最优的解决方案还是第二种。
<a name="Fu7X0"></a>
## 简介
将一个 List 分成多个小 List 的过程，称之为分片，当然也可以叫做“List 分隔”，选一个喜欢的、好理解的叫法就行。<br />在 Java 中，分片的常见实现方法有以下几种：

1. 使用 Google 的 Guava 框架实现分片；
2. 使用 Apache 的 commons 框架实现分片；
3. 使用国产神级框架 Hutool 实现分片；
4. 使用 JDK 8 中提供 Stream 实现分片；
5. 自定义分片功能。

接下来分别来看。
<a name="r5e3T"></a>
## 1、Google Guava
先在项目的 pom.xml 中添加框架支持，增加以下配置：
```xml
<!-- google guava 工具类 -->
<!-- https://mvnrepository.com/artifact/com.google.guava/guava -->
<dependency>
  <groupId>com.google.guava</groupId>
  <artifactId>guava</artifactId>
  <version>31.0.1-jre</version>
</dependency>
```
有了 Guava 框架之后，只需要使用 Lists.partition 方法即可实现分片，如下代码所示：
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
以上代码的执行结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034301-0693c371-104e-479f-9a73-929046f85f93.png#clientId=ua74ad660-aff4-4&from=paste&id=u7a8f2c23&originHeight=308&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud59bba1d-14f1-4cce-9054-0f8182759cf&title=)
<a name="b3xnm"></a>
## 2、apache commons
先在项目的 pom.xml 中添加框架支持，增加以下配置：
```xml
<!-- apache 集合工具类 -->
<!-- https://mvnrepository.com/artifact/org.apache.commons/commons-collections4 -->
<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-collections4</artifactId>
  <version>4.4</version>
</dependency>
```
有了  commons 框架之后，只需要使用 `ListUtils.partition` 方法即可实现分片，如下代码所示：
```java
import org.apache.commons.collections4.ListUtils;

import java.util.Arrays;
import java.util.List;

/**
 * commons.collections4 集合分片
 */
public class PartitionExample {
    // 原集合
    private static final List<String> OLD_LIST = Arrays.asList(
            "唐僧,悟空,八戒,沙僧,曹操,刘备,孙权".split(","));

    public static void main(String[] args) {
        // 集合分片
        List<List<String>> newList = ListUtils.partition(OLD_LIST, 3);
        newList.forEach(i -> {
            System.out.println("集合长度：" + i.size());
        });
    }
}
```
以上代码的执行结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034076-87144a7c-8bc1-4751-824e-2b42029bf969.png#clientId=ua74ad660-aff4-4&from=paste&id=ufbcaef66&originHeight=282&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9522902a-5870-4222-a1ed-03b6068a47b&title=)
<a name="xD6m7"></a>
## 3、Hutool
先在项目的 pom.xml 中添加框架支持，增加以下配置：
```xml
<!-- 工具类 hutool -->
<!-- https://mvnrepository.com/artifact/cn.hutool/hutool-all -->
<dependency>
  <groupId>cn.hutool</groupId>
  <artifactId>hutool-all</artifactId>
  <version>5.7.14</version>
</dependency>
```
有了 Hutool 框架之后，只需要使用 `ListUtil.partition` 方法即可实现分片，如下代码所示：
```java
import cn.hutool.core.collection.ListUtil;

import java.util.Arrays;
import java.util.List;

public class PartitionByHutoolExample {
    // 原集合
    private static final List<String> OLD_LIST = Arrays.asList(
        "唐僧,悟空,八戒,沙僧,曹操,刘备,孙权".split(","));

    public static void main(String[] args) {
        // 分片处理
        List<List<String>> newList = ListUtil.partition(OLD_LIST, 3);
        newList.forEach(i -> {
            System.out.println("集合长度：" + i.size());
        });
    }
}
```
以上代码的执行结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034085-41ed8b55-c8d7-44ce-84de-d290dfe17069.png#clientId=ua74ad660-aff4-4&from=paste&id=ub0e6b1aa&originHeight=359&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9d61873b-4b24-4bae-b82e-8ed512aca0e&title=)
<a name="DPqkP"></a>
## 4、JDK
Stream 通过 JDK 8 中的  Stream 来实现分片就无需添加任何框架了，具体的实现代码如下：
```java
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * JDK Stream Partition
 */
public class PartitionByStreamExample {
    // 原集合
    private static final List<Integer> OLD_LIST = Arrays.asList(
            1, 2, 3, 4, 5, 6);

    public static void main(String[] args) {
        // 集合分片：将大于 3 和小于等于 3 的数据分别分为两组
        Map<Boolean, List<Integer>> newMap = OLD_LIST.stream().collect(
                Collectors.partitioningBy(i -> i > 3)
        );
        // 打印结果
        System.out.println(newMap);
    }
}
```
以上代码的执行结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034192-da1acfc3-9ad9-42a1-8a04-cae5dcf49a0f.png#clientId=ua74ad660-aff4-4&from=paste&id=ue5694500&originHeight=282&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f79072b-5a75-4d0d-b7fc-ccee43fb747&title=)此方式的优点的无需添加任何框架，但缺点是只能实现简单的分片（将一个 List 分为两个），并且要有明确的分片条件。比如本篇案例中设置的分片条件就是数组是否大于 3，如果大于 3 就会被归为一组，否则就会被分到另一组。
<a name="xaPwP"></a>
## 5、自定义分片
如果不想引入第三方框架，并且使用 Stream 也无法满足需求，就可以考虑自己写代码来实现分片功能了。因为此方式不常用，所以咱们这里只给出关键方法。<br />自定义分片功能的关键实现方法是 JDK 自带的 subList 方法，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034706-f7f264ef-aa56-4c02-a40d-1f558400e0ad.png#clientId=ua74ad660-aff4-4&from=paste&id=u44c5dd32&originHeight=144&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u736e4c91-ac1c-4e1d-9b0e-770edd4ea89&title=)使用示例如下：
```java
import java.util.Arrays;
import java.util.List;

public class App {
    private static final List<String> _OLD_LIST = Arrays.asList(
        "唐僧,悟空,八戒,沙僧,曹操,刘备,孙权".split(","));

    public static void main(String[] args) {
        // 集合分隔
        List<String> list = _OLD_LIST.subList(0, 3);
        // 打印集合中的元素
        list.forEach(i -> {
            System.out.println(i);
        });
    }
}
```
以上代码的执行结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664183034716-8c08eca2-3b2f-47be-8d82-cc5e4940ddd8.png#clientId=ua74ad660-aff4-4&from=paste&id=ubade22f4&originHeight=278&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u19ddb46d-c377-4e8a-b4d9-1b95dc25393&title=)
<a name="G0VRx"></a>
## 总结
介绍了 5 种 List 分片的实现方法，其中最方便的实现方式是引入第三方框架，比如 Google 的 Guava、Apache 的 Commons 或者是国产开源的 Hutool 都可以，当然如果项目已经包含了以上任意一种，直接使用就行了。如果是简单的分片就可以考虑使用 JDK 的 Stream 或者是 List 内置的 subList 方法来实现分片功能了。
