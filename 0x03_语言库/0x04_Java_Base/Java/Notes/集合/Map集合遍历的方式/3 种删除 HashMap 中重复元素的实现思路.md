Java
<a name="e7O6m"></a>
## 为什么不是根据 Key 去重？
大家都知道，HashMap 的 key 是不会重复的，如果有重复就会用新值覆盖旧值。<br />向一个 HashMap 中插入元素时，HashMap 会根据这个 key 的 `equals` 和 `hashCode` 方法进行判断，如果两个 key 的值用 `equals` 方法比较相同，且 key 的 hashCode 值也相同，那么 HashMap 将认为这是同一个 key，后续插入相同 key 的键值对会将旧值替换为新值。<br />**需要注意的是：**<br />Java 中的基本数据类型和 String 等内置类，它们已经正确实现了 equals 和 hashCode 方法，可以直接用作 HashMap 的 key，而不会导致重复的 key 出现。<br />如果使用自定义类的对象作为 HashMap 的 key，需要保证这个类正确实现了 equals 和 hashCode 方法，否则可能会出现插入 "**重复 key**" 的情况，正常情况下，这是不符合规范和逻辑的。
<a name="Mxy4o"></a>
## HashMap 删除重复元素方案
以下 HashMap 初始测试数据：
```java
public Map<String, String> initMap = new HashMap<>() {{
    put("user1", "张三");
    put("user2", "李四");
    put("user3", "张三");
    put("user4", "李四");
    put("user5", "王五");
    put("user6", "赵六");
    put("user7", "李四");
    put("user8", "王五");
}};
```
<a name="g1DV9"></a>
### 1、新创建 Map 添加不重复元素
```java
/**
 * 新创建 Map 添加不重复元素
 */
@Test
public void removeDuplicated1() {
    Map<String, String> map = new HashMap<>();
    initMap.forEach((k, v) -> {
        if (!map.containsValue(v)) {
            map.put(k, v);
        }
    });
    System.out.println(map);
}
```
这种方法很原始，通过创建一个新 HashMap，添加元素前进行判断，如果元素在新 HashMap 中不存在才进行添加。
<a name="QDsF1"></a>
### 2、添加 Set 再删除重复元素
```java
/**
 * 添加 Set 再删除重复元素
 */
@Test
public void removeDuplicated2() {
    Set<String> set = new HashSet<>();
    Iterator<Map.Entry<String, String>> iterator = initMap.entrySet().iterator();
    while (iterator.hasNext()) {
        Map.Entry<String, String> entry = iterator.next();
        if (!set.add(entry.getValue())) {
            iterator.remove();
        }
    }
    System.out.println(initMap);
}
```
这种方法和第一种方法大同小异，通过创建一个 HashSet，然后遍历 HashMap，因为 HashSet 是不允许重复元素的，所以，如果 HashSet 能添加元素说明元素没有重复，否则说明元素重复了，然后删除即可。
<a name="o1Brs"></a>
### 3、使用 Stream 删除重复元素
```java
/**
 * 使用 Stream 删除重复元素
 */
@Test
public void removeDuplicated3() {
    Map<String, String> resultMap = initMap.entrySet().stream().collect(
            Collectors.toMap(Map.Entry::getValue, Map.Entry::getKey, (key1, key2) -> key1)
    ).entrySet().stream().collect(
            Collectors.toMap(Map.Entry::getValue, Map.Entry::getKey, (key1, key2) -> key1)
    );
    System.out.println(resultMap);
}
```
利用 Stream 的 collect 方法重新进行收集，这个方法也十分简单，一行代码搞定，为了可读性，文中代码进行了换行。<br />`Collectors.toMap` 方法返回的是一个Collector，它可以将元素累积到 Map 中，Map 的键和值将提供的映射函数应用到输入元素的结果，如果映射的键包含重复项，则值映射函数会使用提供的 merge 函数进行结果合并。<br />`**Collectors.toMap**`** 方法可以对 Key 进行去重合并，这也是为什么进行了两次 collect 收集的原因：**<br />**第一次收集：**<br />把 Value 作为 Key，Key 作为 Value，这样就能使用 Value 进行去重了，输出结果：
```
{李四=user2, 张三=user1, 王五=user8, 赵六=user6}
```
虽然能去重了，但是 HashMap 中的 Key 和 Value 值却颠倒了，所以需要第二次收集。<br />**第二次收集：**<br />现在的 Key 是之前的 Value，所以需要再相互换过来，输出结果：
```
{user1=张三, user2=李四, user8=王五, user6=赵六}
```
这个方法比较绕，虽然能一行代码搞定，但代码很冗余，不是很优雅，**最重要的是这两次的收集过程会创建两次新 Map，相对比较耗内存。**
<a name="Yo4pB"></a>
## 总结
这里总结了 3 种删除 HashMap 重复元素的方法：

- 新创建 Map 添加不重复元素
- **添加 Set 再删除重复元素（推荐）**
- 使用 Stream 删除重复元素

实际开发过程中，可能会使用不同的遍历方式，使用哪种删除方案可以根据不同的遍历方式进行选择，**但推荐使用 Set 方案，可以直接删除 Map 中的重复元素，不会创建新的 HashMap。**
