Java Map<br />在 Jdk 8 中 Map 接口提供了一些新的便利的方法。Jdk 8 中引进的 Map 方法有：`getOrDefault(Object, V)`，`putIfAbsent(K, V)`，`remove(Object, Object)`，`replace(K, V)`，`andreplace(K, V, V)`。
<a name="zxhIs"></a>
## Map 范例
<a name="aTyUd"></a>
### `Map.getOrDefault(Object key, V defaultValue)`
Map 的新方法 `getOrDefault(Object key, V defaultValue)` 允许调用者在代码语句中规定获得在 map 中符合提供的键的值，否则在没有找到提供的键的匹配项的时候返回一个 “默认值”。
```java
default V getOrDefault(Object key, V defaultValue) {
    V v;
    return (((v = get(key)) != null) || containsKey(key))
        ? v
        : defaultValue;
}
```
<a name="dBp5a"></a>
### `Map.putIfAbsent(K key, V value)`
Map 的新方法 `putIfAbsent(K key, V value)` 允许调用者在代码语句中规定添加在 map 中符合提供的键的值，若 map 中有对应 K 映射的 V 且不为 null 则直接返回，否则执行 put。
```java
default V putIfAbsent(K key, V value) {
    V v = get(key);
    if (v == null) {
        v = put(key, value);
    }

    return v;
}
```
<a name="PP1Ut"></a>
### `Map.computeIfAbsent(K key, Function< ? super K, ? extends V> mappingFunction)`
`Map.computeIfAbsent` 方法原型 `V computeIfAbsent(K key, Function< ? super K, ? extends V> mappingFunction)`，如果指定的 key 不存在或相关的 value 为 null 时，设置 key 与关联一个计算出的非 null 值，计算出的值为 null 的话什么也不做(不会去删除相应的 key)。如果 key 存在并且对应 value 不为 null 的话什么也不做。同样，方法的返回值也是最终的 `map.get(key)`。
```java
default V computeIfAbsent(K key, Function<? super K, ? extends V> mappingFunction) {
    Objects.requireNonNull(mappingFunction);
    V v;
    if ((v = get(key)) == null) {
        V newValue;
        if ((newValue = mappingFunction.apply(key)) != null) {
            put(key, newValue);
            return newValue;
        }
    }

    return v;
}
```
<a name="S0NYu"></a>
### `Map.computeIfPresent(K key, BiFunction< ? super K, ? super V, ? extends V> remappingFunction)`
`Map.computeIfPresent` 方法原型 `V computeIfPresent(K key, BiFunction< ? super K, ? super V, ? extends V> remappingFunction)`，如果指定的 key 存在并且相关联的 value 不为 null 时，根据旧的 key 和 value 计算 newValue 替换旧值，newValue 为 null 则从 map 中删除该 key；key 不存在或相应的值为 null 时则什么也不做，方法的返回值为最终的 `map.get(key)`。
```java
default V computeIfPresent(K key,
        BiFunction<? super K, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    V oldValue;
    if ((oldValue = get(key)) != null) {
        V newValue = remappingFunction.apply(key, oldValue);
        if (newValue != null) {
            put(key, newValue);
            return newValue;
        } else {
            remove(key);
            return null;
        }
    } else {
        return null;
    }
}
```
<a name="mtOMx"></a>
### `Map.remove(Object key, Object value)`
Map 的新方法 `remove(Object key, Object value)` 超越了长期有效的 `Map.remove(Object key)`方法，只有在提供的键和值都匹配的时候才会删除该 map 项（之前的有效版本只是查找 “键” 的匹配来删除）。
```java
default boolean remove(Object key, Object value) {
    Object curValue = get(key);
    if (!Objects.equals(curValue, value) ||
        (curValue == null && !containsKey(key))) {
        return false;
    }
    remove(key);
    return true;
}
```
<a name="WZs7T"></a>
### `Map.replace(K key, V value)` / `Map.replace(K key, V oldValue, V newValue)`
`Map.replace(K key, V value)` 方法只有在指定的键已经存在并且有与之相关的映射值时才会将指定的键映射到指定的值（新值）。
```java
default V replace(K key, V value) {
    V curValue;
    if (((curValue = get(key)) != null) || containsKey(key)) {
        curValue = put(key, value);
    }
    return curValue;
}
```
`Map.replace(K key, V oldValue, V newValue)` 方法接受一个额外的（第三个）参数，只有在指定的键和值都匹配的情况下才会替换。
```java
default boolean replace(K key, V oldValue, V newValue) {
    Object curValue = get(key);
    if (!Objects.equals(curValue, oldValue) ||
        (curValue == null && !containsKey(key))) {
        return false;
    }
    put(key, newValue);
    return true;
}
```
<a name="AWGdV"></a>
### `Map.merge(K key, V value, BiFunction< ? super V, ? super V, ? extends V> remappingFunction)`
`Map.merge(K key, V value, BiFunction< ? super V, ? super V, ? extends V> remappingFunction)` 方法，如果指定的 key 不存在，或相应的值为 null 时，则设置 value 为相关联的值。否则根据 key 对应的旧值和 value 计算出新的值 newValue，newValue 为 null 时，删除该 key，否则设置 key 对应的值为 newValue。方法的返回值也是最终的 `map.get(key)` 值。
```java
default V merge(K key, V value,
        BiFunction<? super V, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    Objects.requireNonNull(value);
    V oldValue = get(key);
    V newValue = (oldValue == null) ? value :
               remappingFunction.apply(oldValue, value);
    if(newValue == null) {
        remove(key);
    } else {
        put(key, newValue);
    }
    return newValue;
}
```
<a name="asaaP"></a>
### `Map.forEach(BiConsumer< ? super K, ? super V> action)`
```java
Map.forEach(BiConsumer< ? super K, ? super V> action) 方法用于遍历 map。
// JDK8 之前的实现方式 方式一 这是最常见的并且在大多数情况下也是最可取的遍历方式
for (Map.Entry<String, String> entry : statesAndCapitals.entrySet()) {
    System.out.println("Key =" + entry.getKey() + ", Value =" + entry.getValue());
}

// JDK8 之前的实现方式 方法二 在 for-each 循环中遍历 keys 或 values
// 遍历 map 中的键
for (String key : statesAndCapitals.keySet()) {
    System.out.println("Key =" + key);
}
// 遍历 map 中的值
for (String value : statesAndCapitals.values()) {
    System.out.println("Value =" + value);
}


// JDK8 之前的实现方式 方法三使用 Iterator 遍历
Iterator<Map.Entry<String, String>> entries = statesAndCapitals.entrySet().iterator();
while (entries.hasNext()) {
    Map.Entry<String, String> entry = entries.next();
    System.out.println("Key =" + entry.getKey() + ", Value =" + entry.getValue());
}

// JDK8 之前的实现方式 通过键找值遍历（效率低）
for (String key : statesAndCapitals.keySet()) {
    String value = statesAndCapitals.get(key);
    System.out.println("Key =" + key + ", Value =" + value);
}

// JDK8 的实现方式
statesAndCapitals.forEach((s, s2) -> System.out.println("Key =" + s + ", Value =" + s2));
```
<a name="pJroC"></a>
### `Map.sorted`
`Map.sorted` 方法用于排序 map。
```java
/**
 * 1. 第一层括弧实际是定义了一个匿名内部类 (Anonymous Inner Class)
 * 2. 第二层括弧实际上是一个实例初始化块 (instance initializer block), 这个块在内部匿名类构造时被执行, 这个块之所以被叫做 “实例初始化块” 是因为它们被定义在了一个类的实例范围内
 * 3. 如果 value 为 java 对象, 则需要实现 Comparable 接口, 重写 compareTo 方法
 */
Map<String, Integer> map = new HashMap<String, Integer>() {
    {
        put("A", 3);
        put("B", 5);
        put("C", 1);
        put("D", 1);
        put("E", 9);
    }
};
System.out.println(map);


// 1. 根据 value 对 map 进行 ASC 排序
Map<String, Integer> sortedMap = new LinkedHashMap<>();
map.entrySet()
    .stream()
    .sorted(Map.Entry.comparingByValue())
    .forEachOrdered(x -> sortedMap.put(x.getKey(), x.getValue()));
System.out.println(sortedMap);

// 2. 根据 value 对 map 进行 DESC 排序
Map<String, Integer> sortedMap2 = new LinkedHashMap<>();
map.entrySet()
    .stream()
    // DESC .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
    .sorted(Collections.reverseOrder(Map.Entry.comparingByValue()))
    .forEachOrdered(x -> sortedMap2.put(x.getKey(), x.getValue()));
System.out.println(sortedMap2);

// 3.Collectors.toMap 直接返回排好序的 map
Map<String, Integer> sortedMap3;
sortedMap3 = map.entrySet()
    .stream()
    .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
    // .collect(Collectors.toMap(x -> x.getKey(), x -> x.getValue(), (x1, x2) -> x2, LinkedHashMap::new));
    .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, (x1, x2) -> x2, LinkedHashMap::new));
System.out.println(sortedMap3);
```
