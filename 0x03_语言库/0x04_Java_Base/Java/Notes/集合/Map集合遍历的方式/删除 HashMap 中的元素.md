Java
<a name="QQKuD"></a>
## HashMap 删除元素方案
**假设有以下数据：**
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
一般删除 HashMap 集合中的元素，如果知道具体的 Key，并且需要根据 Key 删除元素，使用 remove 方法就可以了。但是如何根据 Value 删除 HashMap 集合中的元素呢？这才是必须掌握的技巧！
<a name="up8AB"></a>
### 1、使用 for 循环删除
```java
/**
 * 使用 for 循环删除
 */
@Test
public void remove1() {
    Set<Map.Entry<String, String>> entries = new CopyOnWriteArraySet<>(initMap.entrySet());
    for (Map.Entry<String, String> entry : entries) {
        if ("张三".equals(entry.getValue())) {
            initMap.remove(entry.getKey());
        }
    }
    System.out.println(initMap);
}
```
输出结果：
```
{user2=李四, user7=李四, user8=王五, user5=王五, user6=赵六, user4=李四}
```
使用 HashMap 中实现的 entrySet 方法获取元素的集合，然后再进行循环遍历，先根据 Value 值判断要删除的元素，然后再根据 Key 删除元素。<br />在之前的文章中知道，增强的 for 循环底层使用的迭代器 Iterator，而 HashMap 是 fail-fast 类型的错误机制，所以遍历时删除元素会出现 java.util.ConcurrentModificationException 并发修改异常。<br />所以，这里使用了线程安全的 CopyOnWriteArraySet 封装了一层，避免出现并发修改异常，java.util.concurrent 包中的并发集合类都被设计为 fail-safe（安全失败）类型的，比如 CopyOnWrite* 、ConcerrentHashMap 集合，遍历过程中结构发生变更是安全的，不会抛出以上异常。<br />**需要注意的是：**<br />虽然  CopyOnWriteArraySet 并发性能很好，但每次删除时都会复制一份同等集合，所以要考虑数据过多可能导致的内存消耗问题。具体使用和实现原理可以点击该 CopyOnWriteArraySet  关键字链接看之前的文章，这里不再撰述。
<a name="HhFRw"></a>
### 2、使用 forEach 循环删除
```java
/**
 * 使用 forEach 循环删除
 */
@Test
public void remove2() {
    ConcurrentHashMap<String, String> map = new ConcurrentHashMap<>(initMap);
    map.forEach((k, v) -> {
        if ("张三".equals(v)) {
            map.remove(k);
        }
    });
    System.out.println(map);
}
```
输出结果：
```
{user2=李四, user7=李四, user8=王五, user5=王五, user6=赵六, user4=李四}
```
使用 HashMap 自带的 forEach 循环删除指定值的元素，这里为什么使用线程安全的 ConcurrentHashMap 集合包装了一层，同样是为了避免并发修改异常。ConcurrentHashMap 在各版本中都使用了最优的锁设计方案，它的并发性能也是非常优异的。
<a name="Fp4sX"></a>
### 3、使用 Iterator 迭代器删除
```java
/**
 * 使用 Iterator 迭代器删除
 */
@Test
public void remove3() {
    Iterator<Map.Entry<String, String>> iterator = initMap.entrySet().iterator();
    while (iterator.hasNext()) {
        Map.Entry<String, String> entry = iterator.next();
        if ("张三".equals(entry.getValue())) {
            iterator.remove();
        }
    }
    System.out.println(initMap);
}
```
输出结果：
```
{user2=李四, user7=李四, user8=王五, user5=王五, user6=赵六, user4=李四}
```
这种方式即正常使用迭代器遍历删除，它不会发生并发修改异常。<br />**需要注意的是：**这种方法虽然不会发生并发修改异常，但 HashMap 并不是线程安全的，在迭代删除元素时，另一个线程可能会删除 HashMap 中的数据， 这时使用迭代器删除同样会导致并发修改异常。<br />所以，要保证线程安全的删除，在创建迭代器之前，可以先用线程安全的 ConcurrentHashMap 集合包装一层。或者使用  synchronized 关键字锁住整个 Map。<br />如果没有多线程修改环境，可以不用考虑。
<a name="oWaaB"></a>
### 4、使用 removeIf 删除
```java
/**
 * 使用 removeIf 删除
 */
@Test
public void remove4() {
    initMap.entrySet().removeIf(entry -> "张三".equals(entry.getValue()));
    System.out.println(initMap);
}
```
输出结果：
```
{user2=李四, user7=李四, user8=王五, user5=王五, user6=赵六, user4=李四}
```
使用 entrySet 的 removeIf 删除，它底层使用的是迭代器：
```java
default boolean removeIf(Predicate<? super E> filter) {
    Objects.requireNonNull(filter);
    boolean removed = false;
    final Iterator<E> each = iterator();
    while (each.hasNext()) {
        if (filter.test(each.next())) {
            each.remove();
            removed = true;
        }
    }
    return removed;
}
```
所以，它和方法 3 是一样的，只不过把条件写成了 Predicate 函数式接口而已。<br />**需要注意的是：**removeIf 虽然更方便了，但它仍然不是线程安全的，多线程场景参考方案同方法 3。
<a name="ghLBa"></a>
### 5、使用 Stream 删除
```java
/**
 * 使用 Stream 删除
 */
@Test
public void remove5() {
    Map<String, String> map = initMap.entrySet().stream()
    .filter(entry -> !"张三".equals(entry.getValue()))
    .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    System.out.println(map);
}
```
输出结果：
```
{user2=李四, user7=李四, user8=王五, user5=王五, user6=赵六, user4=李四}
```
利用 Stream 的 filter 方法进行过滤，这个方法也十分简单，一行代码搞定。
<a name="MdgWo"></a>
## 总结
本文总结了 5 种删除 HashMap 元素的方法：

- 使用 for 循环删除
- 使用 forEach 循环删除
- 使用 Iterator 迭代器删除
- 使用 removeIf 删除
- 使用 Stream 删除

实际开发过程中，可能会使用不同的遍历方式，所以重点要考虑多线程场景，**如果只是简单的删除元素，使用 removeIf 和 Stream 过滤是最省事的。**
