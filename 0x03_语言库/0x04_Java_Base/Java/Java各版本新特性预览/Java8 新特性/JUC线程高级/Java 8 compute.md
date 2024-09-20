Java<br />分享一个实用的 Java 8 开发技能，那就是 Map 接口中增加的 `compute` 方法，给 Map 集合计算更新用的。
<a name="SRdfh"></a>
## compute简介
如下所示，Java 8 在 Map 和 ConcurrentMap 接口中都增加了 3 个 `compute` 方法，说明也是支持多线程并发安全操作的。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631670398031-f4f65304-b85f-48f8-a39b-fc2774e9bc8b.png#clientId=ud7a1c4ec-aac9-4&from=paste&height=217&id=u6a83d50b&originHeight=289&originWidth=1355&originalType=binary&ratio=1&size=44466&status=done&style=shadow&taskId=u9b0c8b8c-d878-4187-974b-e7a159c75da&width=1016)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631670458993-dba9516e-7c00-439d-9017-824af94f9888.png#clientId=ud7a1c4ec-aac9-4&from=paste&height=150&id=u5cc28cfe&originHeight=200&originWidth=1298&originalType=binary&ratio=1&size=35457&status=done&style=shadow&taskId=u538306e0-f089-42c3-988b-073a0a3319f&width=974)<br />这三个方法的区别：

- `compute`：计算并更新值
- `computeIfAbsent`：Value不存在时才计算
- `computeIfPresent`：Value存在时才计算
<a name="zFFz4"></a>
## compute有啥用?
话说这有什么卵用？<br />先看看没用 Java 8 的一个小示例：
```java
private static void preJava8() {
    List<String> animals = Arrays.asList("dog", "cat", "cat", "dog", "fish", "dog");
    Map<String, Integer> map = new HashMap<>();
    for(String animal : animals){
        Integer count = map.get(animal);
        map.put(animal, count == null ? 1 : ++count);
    }
    System.out.println(map);
}
```
输出：
```
{cat=2, fish=1, dog=3}
```
这是一个统计一个列表中每个动物的数量，代码再怎么精简都需要一步 `get` 操作，判断集合中是否有元素再确定是初始化：1，还是需要 +1。<br />很多时候，这个 `get` 操作显然是毫无必要的，所以 Java 8 提供了 3 个 `compute` 方法，来看看怎么用吧！<br />**Java 8 compute 实现方式**：
```java
private static void inJava8() {
    List<String> animals = Arrays.asList("dog", "cat", "cat", "dog", "fish", "dog");
    Map<String, Integer> map = new HashMap<>();
    for(String animal : animals){
        map.compute(animal, (k, v) -> v == null ? 1 : ++v);
    }
    System.out.println(map);
}
```
使用 `compute` 方法一行搞定，省去了需要使用 `get` 取值再判断的冗余操作，直接就可以获取元素值并计算更新，是不是很方便呢？
<a name="N1Yps"></a>
## `compute`源码分析
这还是一个默认方法，为什么是默认方法，也是为了不改动其所有实现类。
```java
default V compute(K key,
                  BiFunction<? super K, ? super V, ? extends V> remappingFunction) {

    // 函数式接口不能为空    
    Objects.requireNonNull(remappingFunction);

    // 获取旧值
    V oldValue = get(key);

    // 获取计算的新值
    V newValue = remappingFunction.apply(key, oldValue);

    if (newValue == null) { // 新值为空
        // delete mapping
        if (oldValue != null || containsKey(key)) { // 旧值存在时
            // 移除该键值
            remove(key);
            return null;
        } else {
            // nothing to do. Leave things as they were.
            return null;
        }
    } else { // 新值不为空
        // 添加或者覆盖旧值
        put(key, newValue);
        return newValue;
    }
}
```
实现逻辑其实也很简单，其实就是结合了 Java 8 的函数式编程让代码变得更简单了。
