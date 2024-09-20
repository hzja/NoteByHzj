Java<br />Java中的，如果一个对象实例不能被更改就是一个`Immutable`的对象，比如String等都是`Immutable`的对象。 `ImmutableMap`：一个不可变集合
<a name="acnRj"></a>
## 1、为什么使用它呢?
线程安全、更有效的利用内存、可作为常量
<a name="gZ0Uk"></a>
## 2、导入依赖
```xml
<dependency>
 <groupId>com.google.guava</groupId>
 <artifactId>guava</artifactId>
</dependency>
```
<a name="lzk9v"></a>
## 3、创建ImmutableMap
```java
Map<String,Object> immutableMap = new ImmutableMap.Builder<String,Object>().build();
```
<a name="Apgmh"></a>
## 4、在创建时放值
```java
Map<String,Object> immutableMap = new ImmutableMap.Builder<String,Object>()
 .put("k1","v1")
 .put("k2","v2")
 .build();
```
<a name="w8r9h"></a>
## 5、创建后不可变
```java
immutableMap.put("k1","v3");//会抛出java.lang.UnsupportedOperationException
```
ImmutableMap中key和value均不能为null，放入null值会抛出异常
<a name="TgZKj"></a>
## 6、ImmutableMap的使用场景
适合

1. 确定性的配置，比如根据不同的key值得到不同的请求url 
2. 写单元测试 

不适合：key, value为未知参数，可能有null产生的情况
<a name="bGBje"></a>
## 7、其他对应结构
| **可变集合类型** | **可变集合源: JDK or Guava?** | **Guava不可变集合** |
| --- | --- | --- |
| `Collection` | JDK | `ImmutableCollection` |
| `List` | JDK | `ImmutableList` |
| `Set` | JDK | `ImmutableSet` |
| `SortedSet`/`NavigableSet` | JDK | `ImmutableSortedSet` |
| `Map` | JDK | `ImmutableMap` |
| `SortedMap` | JDK | `ImmutableSortedMap` |
| `Multiset` | Guava | `ImmutableMultiset` |
| `SortedMultiset` | Guava | `ImmutableSortedMultiset` |
| `Multimap` | Guava | `ImmutableMultimap` |
| `ListMultimap` | Guava | `ImmutableListMultimap` |
| `SetMultimap` | Guava | `ImmutableSetMultimap` |
| `BiMap` | Guava | `ImmutableBiMap` |
| `ClassToInstanceMap` | Guava | `ImmutableClassToInstanceMap` |
| `Table` | Guava | `ImmutableTable` |

