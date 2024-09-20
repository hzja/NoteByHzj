Java<br />日常工作中 Map 绝对是 Java 程序员高频使用的一种数据结构，那 Map 都有哪些遍历方式呢？
<a name="Pw9U0"></a>
## 通过 entrySet 来遍历
<a name="TR8uN"></a>
### 1、通过 for 和 `map.entrySet()` 来遍历
第一种方式是采用 for 和 `Map.Entry` 的形式来遍历，通过遍历 `map.entrySet()` 获取每个 entry 的 key 和 value，代码如下。这种方式一般也是使用的比较多的一种方式，没有什么花里胡哨的用法，就是很朴素的获取 map 的 key 和 value。
```java
public static void testMap1(Map<Integer, Integer> map) {
    long sum = 0;
    for (Map.Entry<Integer, Integer> entry : map.entrySet()) {
        sum += entry.getKey() + entry.getValue();
    }
    System.out.println(sum);
}
```
看过 HashMap 源码的同学应该会发现，这个遍历方式在源码中也有使用，如下图所示，<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385966077-3acddecb-c562-4420-aa0b-a93c21661e34.jpeg#averageHue=%2338322a&clientId=u6207b743-0093-4&from=paste&id=u718e7e7a&originHeight=793&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u66abc24b-ed59-4557-ba6c-7a69023f080&title=)<br />`putMapEntries` 方法在调用 `putAll` 方法的时候会用到。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385966056-7aed5aea-3423-4db5-b03e-ee0799423dca.jpeg#averageHue=%23646261&clientId=u6207b743-0093-4&from=paste&id=u7860464e&originHeight=250&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue3651640-a336-4b37-821d-54024d74b68&title=)
<a name="bi84w"></a>
### 2、通过 for， Iterator 和 `map.entrySet()` 来遍历
第一个方法是直接通过 for 和 `entrySet()` 来遍历的，这次使用 `entrySet()` 的迭代器来遍历，代码如下。
```java
public static void testMap2(Map<Integer, Integer> map) {
    long sum = 0;
    for (Iterator<Map.Entry<Integer, Integer>> entries = map.entrySet().iterator(); entries.hasNext(); ) {
        Map.Entry<Integer, Integer> entry = entries.next();
        sum += entry.getKey() + entry.getValue();
    }
    System.out.println(sum);
}
```
<a name="JgGOK"></a>
### 3、通过 while，Iterator  和 `map.entrySet()` 来遍历
上面的迭代器是使用 for 来遍历，那自然可以想到还可以用 while 来进行遍历，所以代码如下所示。
```java
public static void testMap3(Map<Integer, Integer> map) {
    Iterator<Map.Entry<Integer, Integer>> it = map.entrySet().iterator();
    long sum = 0;
    while (it.hasNext()) {
        Map.Entry<Integer, Integer> entry = it.next();
        sum += entry.getKey() + entry.getValue();
    }
    System.out.println(sum);
}
```
这种方法跟上面的方法类似，只不过循环从 for 换成了 while，日常在开发的时候，很多场景都可以将 for 和 while 进行替换。2 和 3 都使用迭代器 Iterator，通过迭代器的 `next()`，方法来获取下一个对象，依次判断是否有 next。
<a name="INyVh"></a>
## 通过 keySet 来遍历
上面的这三种方式虽然代码的写法不同，但是都是通过遍历 `map.entrySet()` 来获取结果的，殊途同归。接下来看另外的一组。
<a name="ULSnI"></a>
### 4、通过 for 和 `map.keySet()` 来遍历
前面的遍历是通过 `map.entrySet()` 来遍历，这里通过 `map.keySet()` 来遍历，顾名思义前者是保存 entry 的集合，后者是保存 key 的集合，遍历的代码如下，因为是 key 的集合，所以如果想要获取 key 对应的 value 的话，还需要通过 `map.get(key)` 来获取。
```java
public static void testMap4(Map<Integer, Integer> map) {
    long sum = 0;
    for (Integer key : map.keySet()) {
        sum += key + map.get(key);
    }
    System.out.println(sum);
}
```
<a name="XSFRq"></a>
### 5、通过 for，Iterator 和 `map.keySet()` 来遍历
```java
public static void testMap5(Map<Integer, Integer> map) {
    long sum = 0;
    for (Iterator<Integer> key = map.keySet().iterator(); key.hasNext(); ) {
        Integer k = key.next();
        sum += k + map.get(k);
    }
    System.out.println(sum);
}
```
<a name="NdnO5"></a>
### 6、通过 while，Iterator 和 `map.keySet()` 来遍历
```java
public static void testMap6(Map<Integer, Integer> map) {
    Iterator<Integer> it = map.keySet().iterator();
    long sum = 0;
    while (it.hasNext()) {
        Integer key = it.next();
        sum += key + map.get(key);
    }
    System.out.println(sum);
}
```
可以看到这种方式相对于 `map.entrySet()` 方式，多了一步 get 的操作，这种场景比较适合只需要 key 的场景，如果也需要使用 value 的场景不建议使用 `map.keySet()` 来进行遍历，因为会多一步 `map.get()` 的操作。
<a name="AdqYm"></a>
## Java 8 的遍历方式
注意下面的几个遍历方法都是是 JDK 1.8 引入的，如果使用的 JDK 版本不是 1.8 以及之后的版本的话，是不支持的。
<a name="SzYGe"></a>
### 7、通过 `map.forEach()` 来遍历
JDK 中的 forEach 方法，使用率也挺高的。
```java
public static void testMap7(Map<Integer, Integer> map) {
    final long[] sum = {0};
    map.forEach((key, value) -> {
        sum[0] += key + value;
    });
    System.out.println(sum[0]);
}
```
该方法被定义在 `java.util.Map#forEach` 中，并且是通过 `default` 关键字来标识的，如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385966086-20ee9ca7-15ab-4751-9712-6f7a12515917.jpeg#averageHue=%23312f26&clientId=u6207b743-0093-4&from=paste&id=u2538f5f3&originHeight=997&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0851eb4f-f8c2-4b0f-9ac4-cdb34a7f1a9&title=)
<a name="WFQhN"></a>
### 8、Stream 遍历
```java
public static void testMap8(Map<Integer, Integer> map) {
    long sum = map.entrySet().stream().mapToLong(e -> e.getKey() + e.getValue()).sum();
    System.out.println(sum);
}
```
<a name="pz7IO"></a>
### 9、ParallelStream 遍历
```java
public static void testMap9(Map<Integer, Integer> map) {
    long sum = map.entrySet().parallelStream().mapToLong(e -> e.getKey() + e.getValue()).sum();
    System.out.println(sum);
}
```
这两种遍历方式都是 JDK 8 的 Stream 遍历方式，stream 是普通的遍历，parallelStream 是并行流遍历，在某些场景会提升性能，但是也不一定。
<a name="wjqp8"></a>
## 测试代码
上面的遍历方式有了，那么在日常开发中到底该使用哪一种呢？每一种的性能是怎么样的呢？为此这里通过下面的代码，来测试一下每种方式的执行时间。
```java
public static void main(String[] args) {
    int outSize = 1;
    int mapSize = 200;
    Map<Integer, Integer> map = new HashMap<>(mapSize);
    for (int i = 0; i < mapSize; i++) {
        map.put(i, i);
    }
    System.out.println("---------------start------------------");
    long totalTime = 0;
    for (int size = outSize; size > 0; size--) {
        long startTime = System.currentTimeMillis();
        testMap1(map);
        totalTime += System.currentTimeMillis() - startTime;
    }
    System.out.println("testMap1 avg time is :" + (totalTime / outSize));
    // 省略其他方法，代码跟上面一致
}
```
为了避免一些干扰，这里通过外层的 for 来进行多次计算，然后求平均值，当参数分别是 outSize = 1，mapSize = 200 的时候，测试的结果如下<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385966109-3b7b3916-4177-4fcf-9e09-7bcf31d5d7da.jpeg#averageHue=%23403f3f&clientId=u6207b743-0093-4&from=paste&id=u96b3e7e5&originHeight=524&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf4ae6eb2-c6b1-4ac3-8e9e-213088f9639&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385965989-56e30511-2f24-4611-b3fa-e050a3e71391.jpeg#averageHue=%23444343&clientId=u6207b743-0093-4&from=paste&id=ub3eb73b1&originHeight=725&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u011fe7e8-4df0-41c4-9cc3-2c94be86b80&title=)<br />当随着增大 mapSize 的时候，会发现，后面几个方法的性能是逐渐上升的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385966916-f7e62f28-790b-46ba-9726-260192bcbc9f.jpeg#averageHue=%23434342&clientId=u6207b743-0093-4&from=paste&id=uec0d2032&originHeight=750&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua5f61cbd-6dd1-40ff-9bda-33895c813eb&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668385967011-49a8782a-ccc4-4315-bd87-c5359f97d559.jpeg#averageHue=%233a2e21&clientId=u6207b743-0093-4&from=paste&id=u7eec4be7&originHeight=861&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub4ea190b-5b34-4068-a078-b524ce05c7b&title=)
<a name="YQcFE"></a>
## 总结
从上面的例子来看，当集合数量很少的时候，基本上普通的遍历就可以搞定，不需要使用 JDK 8 的高级 API 来进行遍历，当集合数量较大的时候，就可以考虑采用 JDK 8 的 forEach 或者 Stream 来进行遍历，这样的话效率更高。在普通的遍历方法中 `entrySet()` 的方法要比使用 `keySet()` 的方法好。
