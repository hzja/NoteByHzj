Java Stream<br />Stream 是Java SE 8类库中新增的关键抽象，它被定义于 java.util.stream （这个包里有若干流类型：`Stream<T>`代表对象引用流，此外还有一系列特化流，如 `IntStream`，`LongStream`，`DoubleStream`等。<br />Java 8 引入的的`Stream`主要用于取代部分`Collection`的操作，每个流代表一个值序列，流提供一系列常用的聚集操作，可以便捷的在它上面进行各种运算。集合类库也提供了便捷的方式使我们可以以操作流的方式使用集合、数组以及其它数据结构；
<a name="IZbtp"></a>
## stream 的操作种类
![2021-05-15-10-59-06-279867.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048059281-b1cb6461-010e-4024-bec1-764525830da6.png#clientId=u6025a0e3-cb66-4&from=ui&id=ub071c467&originHeight=305&originWidth=621&originalType=binary&size=569517&status=done&style=shadow&taskId=u5508d139-9618-4c1f-b5f9-1d35e4be60f)
<a name="ErioW"></a>
### ①中间操作

- 当数据源中的数据上了流水线后，这个过程对数据进行的所有操作都称为“中间操作”；
- 中间操作仍然会返回一个流对象，因此多个中间操作可以串连起来形成一个流水线；
- stream 提供了多种类型的中间操作，如 `filter`、`distinct`、`map`、`sorted` 等等；
<a name="wd4Q7"></a>
### ②终端操作

- 当所有的中间操作完成后，若要将数据从流水线上拿下来，则需要执行终端操作；
- stream 对于终端操作，可以直接提供一个中间操作的结果，或者将结果转换为特定的 `collection`、`array`、`String` 等；
<a name="UJ7kx"></a>
## stream 的特点
<a name="BCNUt"></a>
### ①只能遍历一次
数据流的从一头获取数据源，在流水线上依次对元素进行操作，当元素通过流水线，便无法再对其进行操作，可以重新在数据源获取一个新的数据流进行操作；
<a name="H4V2a"></a>
### ②采用内部迭代的方式
对`Collection`进行处理，一般会使用` Iterator` 遍历器的遍历方式，这是一种外部迭代；<br />而对于处理Stream，只要申明处理方式，处理过程由流对象自行完成，这是一种内部迭代，对于大量数据的迭代处理中，内部迭代比外部迭代要更加高效；
<a name="miMHz"></a>
## stream 相对于 `Collection` 的优点

- 无存储：流并不存储值；流的元素源自数据源（可能是某个数据结构、生成函数或I/O通道等等），通过一系列计算步骤得到；
- 函数式风格：对流的操作会产生一个结果，但流的数据源不会被修改；
- 惰性求值：多数流操作（包括过滤、映射、排序以及去重）都可以以惰性方式实现。这使得我们可以用一遍遍历完成整个流水线操作，并可以用短路操作提供更高效的实现；
- 无需上界：不少问题都可以被表达为无限流（infinite stream）：用户不停地读取流直到满意的结果出现为止（比如说，枚举 完美数 这个操作可以被表达为在所有整数上进行过滤）；集合是有限的，但流可以表达为无线流；
- 代码简练：对于一些`collection`的迭代处理操作，使用 stream 编写可以十分简洁，如果使用传统的 collection 迭代操作，代码可能十分啰嗦，可读性也会比较糟糕；
<a name="ococB"></a>
## stream 和 iterator 迭代的效率比较
上面 stream 的优点说了那么多，stream 函数式的写法是很舒服，那么 steam 的效率到底怎样呢？<br />先说结论：

- 传统 iterator (for-loop) 比 stream(JDK8) 迭代性能要高，尤其在小数据量的情况下；

- 在多核情景下，对于大数据量的处理，parallel stream 可以有比 iterator 更高的迭代处理效率；<br />我分别对一个随机数列 List （数量从 10 到 10000000）进行映射、过滤、排序、规约统计、字符串转化场景下，对使用 stream 和 iterator 实现的运行效率进行了统计，测试代码 基准测试代码链接<br />测试环境如下：
```java
System：Ubuntu 16.04 xenial

CPU：Intel Core i7-8550U

RAM：16GB

JDK version：1.8.0_151

JVM：HotSpot(TM) 64-Bit Server VM (build 25.151-b12, mixed mode)

JVM Settings:

    -Xms1024m

    -Xmx6144m

    -XX:MaxMetaspaceSize=512m

    -XX:ReservedCodeCacheSize=1024m

    -XX:+UseConcMarkSweepGC

    -XX:SoftRefLRUPolicyMSPerMB=100
```
<a name="LG4UG"></a>
### 1. 映射处理测试
把一个随机数列（`List<Integer>`）中的每一个元素自增1后，重新组装为一个新的`List<Integer>`，测试的随机数列容量从 10 - 10000000，跑10次取平均时间；
```java
//stream
List<Integer> result = list.stream()
    .mapToInt(x -> x)
    .map(x -> ++x)
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
//iterator
List<Integer> result = new ArrayList<>();
for(Integer e : list){
    result.add(++e);
}
//parallel stream
List<Integer> result = list.parallelStream()
    .mapToInt(x -> x)
    .map(x -> ++x)
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
```
![2021-05-15-10-59-06-342696.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048085745-492f9f2d-0ad7-44a3-bc81-ed388a7693b2.png#clientId=u6025a0e3-cb66-4&from=ui&id=uc9202686&originHeight=482&originWidth=782&originalType=binary&size=1133169&status=done&style=shadow&taskId=u7f6221aa-a287-4f08-8405-deea7a41fa1)
<a name="XOyep"></a>
### 2. 过滤处理测试
取出一个随机数列（`List<Integer>`）中的大于 200 的元素，并组装为一个新的`List<Integer>`，测试的随机数列容量从 10 - 10000000，跑10次取平均时间；
```java
//stream
List<Integer> result = list.stream()
    .mapToInt(x -> x)
    .filter(x -> x > 200)
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
//iterator
List<Integer> result = new ArrayList<>(list.size());
for(Integer e : list){
    if(e > 200){
        result.add(e);
    }
}
//parallel stream
List<Integer> result = list.parallelStream()
    .mapToInt(x -> x)
    .filter(x -> x > 200)
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
```
![2021-05-15-10-59-06-389571.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048095537-1cacbf76-7ca8-4de7-8979-0b7958632630.png#clientId=u6025a0e3-cb66-4&from=ui&id=u1fbd2ff9&originHeight=432&originWidth=792&originalType=binary&size=1028603&status=done&style=shadow&taskId=u58f8ff88-d1ba-4879-9768-44d94fe868e)
<a name="mG3c5"></a>
### 3. 自然排序测试
对一个随机数列（`List<Integer>`）进行自然排序，并组装为一个新的`List<Integer>`，iterator 使用的是 `Collections#sort` API（使用归并排序算法实现），测试的随机数列容量从 10 - 10000000，跑10次取平均时间；
```java
//stream
List<Integer> result = list.stream()
    .mapToInt(x->x)
    .sorted()
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
//iterator
List<Integer> result = new ArrayList<>(list);
Collections.sort(result);
//parallel stream
List<Integer> result = list.parallelStream()
    .mapToInt(x->x)
    .sorted()
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
```
![2021-05-15-10-59-06-510249.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048105623-57c72589-6e76-48b3-8f83-10af6f33dd9f.png#clientId=u6025a0e3-cb66-4&from=ui&id=uf6adba23&originHeight=431&originWidth=784&originalType=binary&size=1015858&status=done&style=shadow&taskId=u0441b47e-154e-4ac3-91d2-7e0e7f54f60)
<a name="Ow5CM"></a>
### 4. 归约统计测试
获取一个随机数列（`List<Integer>`）的最大值，测试的随机数列容量从 10 - 10000000，跑10次取平均时间；
```java
//stream
int max = list.stream()
    .mapToInt(x -> x)
    .max()
    .getAsInt();
//iterator
int max = -1;
for(Integer e : list){
    if(e > max){
        max = e;
    }
}
//parallel stream
int max = list.parallelStream()
    .mapToInt(x -> x)
    .max()
    .getAsInt();
```
![2021-05-15-10-59-06-613971.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048114863-afa1d2bc-534b-4cbd-bf17-9a995e69cc8c.png#clientId=u6025a0e3-cb66-4&from=ui&id=u065c4014&originHeight=409&originWidth=790&originalType=binary&size=971389&status=done&style=shadow&taskId=u9f8baa49-1e6a-4393-8ee1-165a7c2f206)
<a name="S91O5"></a>
### 5. 字符串拼接测试
获取一个随机数列（`List<Integer>`）各个元素使用“,”分隔的字符串，测试的随机数列容量从 10 - 10000000，跑10次取平均时间；
```java
//stream
String result = list.stream().map(String::valueOf).collect(Collectors.joining(","));
//iterator
StringBuilder builder = new StringBuilder();
for(Integer e : list){
    builder.append(e).append(",");
}
String result = builder.length() == 0 ? "" : builder.substring(0,builder.length() - 1);
//parallel stream
String result = list.stream().map(String::valueOf).collect(Collectors.joining(","));
```
![2021-05-15-10-59-06-803470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048124428-e8baca78-fa63-487a-8603-08d6ec590c20.png#clientId=u6025a0e3-cb66-4&from=ui&id=u15e5516a&originHeight=477&originWidth=796&originalType=binary&size=1141480&status=done&style=shadow&taskId=u04192680-c576-4b8f-8231-65d7bf1da18)
<a name="Zbs8T"></a>
### 6. 混合操作测试
对一个随机数列（`List<Integer>`）进行去空值，除重，映射，过滤，并组装为一个新的`List<Integer>`，测试的随机数列容量从 10 - 10000000，跑10次取平均时间；
```java
//stream
List<Integer> result = list.stream()
    .filter(Objects::nonNull)
    .mapToInt(x -> x + 1)
    .filter(x -> x > 200)
    .distinct()
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
//iterator
HashSet<Integer> set  = new HashSet<>(list.size());
for(Integer e : list){
    if(e != null && e > 200){
        set.add(e + 1);
    }
}
List<Integer> result = new ArrayList<>(set);
//parallel stream
List<Integer> result = list.parallelStream()
    .filter(Objects::nonNull)
    .mapToInt(x -> x + 1)
    .filter(x -> x > 200)
    .distinct()
    .boxed()
    .collect(Collectors.toCollection(ArrayList::new));
```
![2021-05-15-10-59-06-893227.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621048134062-b7eef015-dd73-405b-9002-197cf9a88a72.png#clientId=u6025a0e3-cb66-4&from=ui&id=u55f533ba&originHeight=467&originWidth=802&originalType=binary&size=1125967&status=done&style=shadow&taskId=u5ebff956-5fde-4f33-9e50-eadb8500268)<br />实验结果总结<br />从以上的实验来看，可以总结处以下几点：

- 在少低数据量的处理场景中（size<=1000），stream 的处理效率是不如传统的 iterator 外部迭代器处理速度快的，但是实际上这些处理任务本身运行时间都低于毫秒，这点效率的差距对普通业务几乎没有影响，反而 stream 可以使得代码更加简洁；
- 在大数据量（szie>10000）时，stream 的处理效率会高于 `iterator`，特别是使用了并行流，在cpu恰好将线程分配到多个核心的条件下（当然parallel stream 底层使用的是 JVM 的 `ForkJoinPool`，这东西分配线程本身就很玄学），可以达到一个很高的运行效率，然而实际普通业务一般不会有需要迭代高于10000次的计算；
- Parallel Stream 受引 CPU 环境影响很大，当没分配到多个cpu核心时，加上引用 `forkJoinPool` 的开销，运行效率可能还不如普通的 Stream；
<a name="E9FL9"></a>
## 使用 Stream 的建议

- 简单的迭代逻辑，可以直接使用 `iterator`，对于有多步处理的迭代逻辑，可以使用 `stream`，损失一点几乎没有的效率，换来代码的高可读性是值得的；
- 单核 cpu 环境，不推荐使用 parallel stream，在多核 cpu 且有大数据量的条件下，推荐使用 paralle stream；
- stream 中含有装箱类型，在进行中间操作之前，最好转成对应的数值流，减少由于频繁的拆箱、装箱造成的性能损失；
