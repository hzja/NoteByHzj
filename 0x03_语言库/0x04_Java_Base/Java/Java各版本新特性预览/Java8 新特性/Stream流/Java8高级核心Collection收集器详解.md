Java Collection
<a name="J9SLm"></a>
## 什么是`Collector`
`Collector`它是一个可以变化的汇聚操作，它将输入元素累积到了一个可变的结果容器中去，在全部的元素处理完之后，`Collector`会把累积的结果转换成一个最终的结果来表示，`Collector`支持串行和并行两种方式。<br />`Collector`接口有三个泛型：`T`为输入元素的类型，`A`为积累结果容器的类型，`R`为最终生成结果的类型。
<a name="pBKCA"></a>
### `collect`方法的作用
一个终端的操作，用于对流中的数据进行归集操作，`collect`方法接受的参数是一个`Collector`<br />有两个重载的方法，在Stream接口里。
```java
//重载⽅法⼀
<R> R collect(Supplier<R> supplier, BiConsumer<R, ? super T>
              accumulator, BiConsumer<R, R>combiner);
//重载⽅法⼆
<R, A> R collect(Collector<? super T, A, R> collector);
```
<a name="UnlzV"></a>
### `Collectror`的作用
就是收集器，也就是一个接口，它的工具类`Collectors`提供链路很多工厂方法
<a name="FD7H2"></a>
### `Collectors`的作用
是一个工具类，提供很多常见的收集器的实现
<a name="dBhcU"></a>
#### `Collectors.toList()`
```java
public static <T> Collector<T, ?, List<T>> toList() {
    return new CollectorImpl<>((Supplier<List<T>>)
                               ArrayList::new, List::add,(left, right) -> {
                                   left.addAll(right); return left; }, CH_ID);
}
```
<a name="sQdX6"></a>
#### `toMap`和`toConcurrentMap`
`toMap`的操作可以将输入的元素给整理成Map，Collectors提供了三种对`toMap`重载的方法<br />1.`toMap(Function<? super T, ? extends K> keyMapper, Function<? super T, ? extends U> valueMapper)`：keyMapper和valueMapper分别提供结果Map的键和值<br />2.`toMap(Function<? super T, ? extends K> keyMapper, Function<? super T, ? extends U> valueMapper, BinaryOperator<U> mergeFunction)`：mergeFunction对键相同的值进行累积<br />3.`toMap(Function<? super T, ? extends K> keyMapper, Function<? super T, ? extends U> valueMapper, BinaryOperator<U> mergeFunction, Supplier<M> mapSupplier)`：mapSupplier可以指定结果的Map类型

<a name="D4DNz"></a>
#### `ArrayList::new`（创建一个ArrayList作为累加器）
<a name="aSRbD"></a>
#### `List::add`（对流中的元素操作就是直接添加到累加器中）
<a name="fzoWu"></a>
#### `reduce`操作（对子任务归集结果addAll,后一个子任务的结果直接全部添加到前一个子任务结果中）
<a name="wwr7N"></a>
#### CH_ID（是一个`unmodifiableSet`集合）
<a name="EtgtI"></a>
## Collector工作原理
Collector是通过四种方法来协同工作完成数据汇聚的操作<br />（1）`supplier`：创建新的结果容器<br />（2）`accumulator`：将输入元素合并到结果容器当中<br />（3）`combiner`：合并两个有结果的容器（并行流使用，将多个线程产生的结果容器合并）<br />（4）`finisher`：将结果容器转换成最终的表示<br />下面以串行为例子<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644578544930-a0a1f671-b5eb-4a55-9c75-31d91007087c.webp#clientId=u4644c4ec-0779-4&from=paste&id=u71be7210&originHeight=750&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf38844ce-6b7e-4d25-ab1e-0d6d1cc40a1&title=)<br />首先的话`supplier`会提供一个结果容器，然后`accumulator`就会向容器里累积元素，最后面的话`finisher`将结果容器转换成为最终的返回结构。如果结果容器的类型和最终返回结果的类型一致的话，那这个`finisher`就可以不用执行，这种行为属于可选操作。<br />但是`combiner`是和并行流有关的，这个在串行流并不会起作用。假如说并行流对元素的操作分在了三条线程完成，那么这三条线程会返回结果容器。此时此刻，`combiner`就可能会这样处理这三个线程的结果容器，会分别把容器进行合并，最终合并成为一个容器返回。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644578544936-d3c71d5a-6099-479f-a09d-3ffbc166e7ff.webp#clientId=u4644c4ec-0779-4&from=paste&id=u797b0985&originHeight=535&originWidth=1006&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueb93ad6a-fc96-4539-bdca-48407a6b4b2&title=)<br />除了上面的四种`Collector`方法，还有一种`characteristics`方法，这个方法用于给Collector设置特征值。枚举常量具有三个特征值：<br />（1）`Concurrent`： 表示的结果容器只有一个（并行流也是如此）。只有在并行流且收集器不具备此特征的情况下，`combiner`返回的`lambda`表达式才会执行。设置了此特性时意味着多个线程可用堆同一个结果容器调用，因此结果容器必须是线程安全的<br />（2）`Unordered`： 表示流中的元素无序<br />（3）`IDENTITY_FINISH`：表示中间结果容器类型与最终结果类型一致。设置此特性的时候`finiser`方法不会被调用
<a name="GO7pV"></a>
### 收集器`Joining`函数
拼接函数`Collectors.joining`
```java
//3种重载⽅法
Collectors.joining()
Collectors.joining("param")
Collectors.joining("param1", "param2", "param3")
```
其中⼀个的实现
```java
public static Collector<CharSequence, ?, String> joining() {
    return new CollectorImpl<CharSequence, StringBuilder, String>(
        StringBuilder::new, StringBuilder::append,
        (r1, r2) -> { r1.append(r2); return r1; },
        StringBuilder::toString, CH_NOID);
}
```
说明：该方法可以将Stream得到一个字符串，`joining`函数可以接受三个参数，分别表示元素之间的连接符、前缀和后缀。
```java
String result = Stream.of("springboot", "mysql", "html5", "css3").collect(Collectors.joining(",", "[", "]"));
```
<a name="Hs4J6"></a>
### 收集器 `partitioningBy`分组
分区是分组的一种特殊情况，该操作将输入元素分为两类（即键是true和false的Map）。`Collectors`提供两个重载的`partitioningBy()`方法：<br />`partitioningBy(Predicate<? super T> predicate)`：`predicate`提供分区依据<br />`partitioningBy(Predicate<? super T> predicate,Collector<? super T, A, D> downstream)`：`downstream`提供结果Map的值。
```java
public static <T>
Collector<T, ?, Map<Boolean, List<T>>> partitioningBy(Predicate<? super
                                                      T> predicate) {
    return partitioningBy(predicate, toList());
}
```
<a name="QcNot"></a>
#### 练习：根据list里面进行分组，字符串长度大于4的为⼀组，其他为另外⼀组
```java
List<String> list = Arrays.asList("java", "springboot","HTML5","nodejs","CSS3");
Map<Boolean, List<String>> result =
list.stream().collect(partitioningBy(obj -> obj.length() > 4));
```
<a name="XgwzY"></a>
### 收集器 `group by`分组
分组 `Collectors.groupingBy()`
```java
public static <T, K> Collector<T, ?, Map<K, List<T>>> groupingBy(Function<?
                                                                 super T, ? extends K> classifier) { 
    return groupingBy(classifier, toList());                                                                                             
}
```
<a name="lITI7"></a>
#### 练习：根据学生所在的省份，进行分组
```java
List<Student> students = Arrays.asList(new Student("⼴东", 23), new
                                       Student("⼴东", 24), new Student("⼴东", 23),new Student("北京", 22), new
                                       Student("北京", 20), new Student("北京", 20),new Student("海南", 25));
Map<String, List<Student>> listMap =
    students.stream().collect(Collectors.groupingBy(obj ->
                                                    obj.getProvince()));
listMap.forEach((key, value) -> {
    System.out.println("========");
    System.out.println(key);
    value.forEach(obj -> {
        System.out.println(obj.getAge());
    });
});
class Student {
    private String province;
    private int age;
    public String getProvince() {
        return province;
    }
    public void setProvince(String province) {
        this.province = province;
    }
    public int getAge() {
        return age;

        public void setAge(int age) {
            this.age = age;
        }
        public Student(String province, int age) {
            this.age = age;
            this.province = province;
        }
    }
```
<a name="nZXWZ"></a>
## 关于性能
对于Stream所提供的几个收集器可以满足绝大部分的开发需求了，`reduce`提供了很多各种的自定义，但是有的时候还是需要自定义`collector`才能够实现。值得铭记是这个收集器是有序的，所以的话不能够并行，这个`combiner`方法是可以不要的，返回`UnsupportedOperationException`来警示此收集器的非并行性。
