JavaLambda<br />日常工作中，Lambda 使用比较多的场景，就是集合类下的 Lambda 流操作，往往几行代码可以实现复杂代码。<br />接下来把 Lambda 流的常用方法用案列讲解一下。
<a name="tx88r"></a>
## ForEach
集合的遍历forEach方法
```java
public void testForEach(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};

    list.forEach(s-> System.out.println(s));
}
```
<a name="KKKXS"></a>
## Collect
将操作后的对象转化为新的对象
```java
public void testCollect(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("2");
    }};

    //转换为新的list
    List newList = list.stream().map(s -> Integer.valueOf(s)).collect(Collectors.toList());
}
```
<a name="SujX9"></a>
## Filter
Filter 为过滤的意思，只要满足 Filter 表达式的数据就可以留下来，不满足的数据被过滤掉
```java
public void testFilter() {
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};        

    list.stream()
        // 过滤掉希望留下来的值
        // 表示希望字符串是 1 能留下来
        // 其他的过滤掉
        .filter(str -> "1".equals(str))
        .collect(Collectors.toList());
}
```
<a name="NtgDd"></a>
## Map
map 方法可以进行一些流的转化，比如原来流中的元素是 A，通过 map 操作，可以使返回的流中的元素是 B
```java
public void testMap() {
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};
    //通过 map 方法list中元素转化成 小写
    List<String> strLowerList = list.stream()
        .map(str -> str.toLowerCase())
        .collect(Collectors.toList());
}
```
<a name="Bp1Bb"></a>
## MapToInt
mapToInt 方法的功能和 map 方法一样，只不过 mapToInt 返回的结果已经没有泛型，已经明确是 int 类型的流了，源码如下：
```java
public void testMapToInt() {
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};
    list.stream()
        .mapToInt(s->Integer.valueOf(s))
        // 一定要有 mapToObj，因为 mapToInt 返回的是 IntStream，因为已经确定是 int 类型了
        // 所有没有泛型的，而 Collectors.toList() 强制要求有泛型的流，所以需要使用 mapToObj
        // 方法返回有泛型的流
        .mapToObj(s->s)
        .collect(Collectors.toList());

    list.stream()
        .mapToDouble(s->Double.valueOf(s))
        // DoubleStream/IntStream 有许多 sum（求和）、min（求最小值）、max（求最大值）、average（求平均值）等方法
        .sum();
}
```
<a name="zF7S0"></a>
## Distinct
distinct 方法有去重的功能
```java
public void testDistinct(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("2");
    }};
    list.stream()
        .map(s -> Integer.valueOf(s))
        .distinct()
        .collect(Collectors.toList());
}
```
<a name="N7Sb8"></a>
## Sorted
Sorted 方法提供了排序的功能，并且允许自定义排序
```java
public void testSorted(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};
    list.stream()
        .map(s -> Integer.valueOf(s))
        // 等同于 .sorted(Comparator.naturalOrder()) 自然排序
        .sorted()
        .collect(Collectors.toList());

    // 自定义排序器
    list.stream()
        .map(s -> Integer.valueOf(s))
        // 反自然排序
        .sorted(Comparator.reverseOrder())
        .collect(Collectors.toList());
}
```
<a name="kdcMT"></a>
## groupingBy
groupingBy 是能够根据字段进行分组，toMap 是把 List 的数据格式转化成 Map 的格式
```java
public void testGroupBy(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("2");
    }};

    Map<String, List<String>> strList = list.stream().collect(Collectors.groupingBy(s -> {
        if("2".equals(s)) {
            return "2";
        }else {
            return "1";
        }
    }));
}
```
<a name="tS3uF"></a>
## FindFirst
findFirst 表示匹配到第一个满足条件的值就返回
```java
public void testFindFirst(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("2");
    }};

    list.stream()
        .filter(s->"2".equals(s))
        .findFirst()
        .get();

    // 防止空指针
    list.stream()
        .filter(s->"2".equals(s))
        .findFirst()
        // orElse 表示如果 findFirst 返回 null 的话，就返回 orElse 里的内容
        .orElse("3");

    Optional<String> str= list.stream()
        .filter(s->"2".equals(s))
        .findFirst();
    // isPresent 为 true 的话，表示 value != null
    if(str.isPresent()){
        return;
    }
}
```
<a name="JRV9z"></a>
## Reduce
reduce 方法允许在循环里面叠加计算值
```java
public void testReduce(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};

    list.stream()
        .map(s -> Integer.valueOf(s))
        // s1 和 s2 表示循环中的前后两个数
        .reduce((s1,s2) -> s1+s2)
        .orElse(0);

    list.stream()
        .map(s -> Integer.valueOf(s))
        // 第一个参数表示基数，会从 100 开始加
        .reduce(100,(s1,s2) -> s1+s2);
}
```
<a name="UbEZs"></a>
## Peek
peek 方法很简单，在 peek 方法里面做任意没有返回值的事情，比如打印日志
```java
public void testPeek(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};
    list.stream().map(s -> Integer.valueOf(s))
        .peek(s -> System.out.println(s))
        .collect(Collectors.toList());
}
```
<a name="TLM4d"></a>
## Limit
limit 方法会限制输出值个数，入参是限制的个数大小
```java
public void testLimit(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("3");
    }};
    list.stream()
        .map(s -> Integer.valueOf(s))
        .limit(2L)
        .collect(Collectors.toList());
}
```
<a name="DHWqW"></a>
## Max，Min
通过max、min方法，可以获取集合中最大、最小的对象
```java
public void testMaxMin(){
    List<String> list = new ArrayList<String>() {{
        add("1");
        add("2");
        add("2");
    }};

    list.stream().max(Comparator.comparing(s -> Integer.valueOf(s))).get();
    list.stream().min(Comparator.comparing(s -> Integer.valueOf(s))).get();
}
```
