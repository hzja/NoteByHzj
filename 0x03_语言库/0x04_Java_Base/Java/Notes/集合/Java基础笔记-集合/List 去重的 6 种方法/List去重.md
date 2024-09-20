在Java开发中，对List集合进行去重是一项基本的任务，而在实际应用中，List中的元素可能是复杂对象，因此需要考虑对对象属性进行去重的情况。本文将综合介绍Java List集合去重的几种方式，包括简单元素和对象属性的去重方法。
<a name="L2QLX"></a>
### 使用Set实现简单元素去重
对于List中包含的简单元素，可以使用Set来实现去重。
```java
List<String> originalList = Arrays.asList("apple", "banana", "orange", "apple", "banana");
Set<String> uniqueSet = new LinkedHashSet<>(originalList);
List<String> deduplicatedList = new ArrayList<>(uniqueSet);
```
这种方式适用于对简单数据类型的List进行去重，但不适用于复杂对象。
<a name="UYBeK"></a>
### 使用Java 8的Stream API实现简单元素去重
Java 8引入的Stream API提供了一种简洁的去重方式。
```java
List<String> originalList = Arrays.asList("apple", "banana", "orange", "apple", "banana");
List<String> deduplicatedList = originalList.stream().distinct().collect(Collectors.toList());
```
这种方式对于简单元素的去重更为简便。
<a name="rqqww"></a>
### 使用Set实现对象属性去重
如果List中的元素是对象，而且对象已正确实现了`equals`和`hashCode`方法，可以直接使用Set进行对象属性的去重。
```java
public class Fruit {
    private String name;

    // constructor, getters, setters

    @Override
    public boolean equals(Object o) {
        // implement equals based on attributes
    }

    @Override
    public int hashCode() {
        // implement hashCode based on attributes
    }
}

List<Fruit> originalList = Arrays.asList(
    new Fruit("apple"),
    new Fruit("banana"),
    new Fruit("orange"),
    new Fruit("apple"),
    new Fruit("banana"));

Set<Fruit> uniqueSet = new LinkedHashSet<>(originalList);
List<Fruit> deduplicatedList = new ArrayList<>(uniqueSet);
```
<a name="cfakQ"></a>
### 使用Java 8的Stream API实现对象属性去重
利用Stream API，可以通过对象的属性进行去重。
```java
List<Fruit> originalList = Arrays.asList(
    new Fruit("apple"),
    new Fruit("banana"),
    new Fruit("orange"),
    new Fruit("apple"),
    new Fruit("banana"));

List<Fruit> deduplicatedList = originalList.stream()
.distinct()
.collect(Collectors.toList());
```
这里假定`Fruit`类实现了`equals`和`hashCode`方法。
<a name="Dco2K"></a>
### **使用自定义方法实现对象属性去重**
自定义方法可以针对对象属性进行去重。
```java
public class FruitUtils {
    public static List<Fruit> deduplicateByProperty(List<Fruit> originalList, Function<Fruit, ?> propertyExtractor) {
        Map<Object, Boolean> seen = new HashMap<>();
        return originalList.stream()
        .filter(fruit -> seen.putIfAbsent(propertyExtractor.apply(fruit), Boolean.TRUE) == null)
        .collect(Collectors.toList());
    }
}

List<Fruit> originalList = Arrays.asList(
    new Fruit("apple"),
    new Fruit("banana"),
    new Fruit("orange"),
    new Fruit("apple"),
    new Fruit("banana"));

List<Fruit> deduplicatedList = FruitUtils.deduplicateByProperty(originalList, Fruit::getName);
```
这种方式允许根据具体需求提供属性提取器，使得去重逻辑更加灵活。
<a name="UMpmx"></a>
### 总结
在处理List集合去重时，选择合适的方法取决于元素的类型以及具体的需求。使用Set、Stream API或自定义方法都是可行的方式，而针对对象属性的去重需要确保对象正确实现了`equals`和`hashCode`方法，或者使用自定义方法提取属性进行去重。选择适合项目需求和数据结构的方式，有助于编写更高效、可维护的代码。
