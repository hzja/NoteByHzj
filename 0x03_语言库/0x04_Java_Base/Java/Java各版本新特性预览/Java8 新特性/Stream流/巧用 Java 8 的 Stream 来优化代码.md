JavaStream<br />Java8的新特性主要是Lambda表达式和流，当流和Lambda表达式结合起来一起使用时，因为流申明式处理数据集合的特点，可以让代码变得简洁易读
<a name="oaAbR"></a>
## 流如何简化代码
如果有一个需求，需要对数据库查询到的菜肴进行一个处理：

- 筛选出卡路里小于400的菜肴
- 对筛选出的菜肴进行一个排序
- 获取排序后菜肴的名字

**菜肴：Dish.java**
```java
public class Dish {  
    private String name;  
    private boolean vegetarian;  
    private int calories;  
    private Type type;  

    // getter and setter  
}
```
**Java8以前的实现方式**
```java
private List<String> beforeJava7(List<Dish> dishList) {  
    List<Dish> lowCaloricDishes = new ArrayList<>();  

    //1.筛选出卡路里小于400的菜肴  
    for (Dish dish : dishList) {  
        if (dish.getCalories() < 400) {  
            lowCaloricDishes.add(dish);  
        }  
    }  

    //2.对筛选出的菜肴进行排序  
    Collections.sort(lowCaloricDishes, new Comparator<Dish>() {  
        @Override  
        public int compare(Dish o1, Dish o2) {  
            return Integer.compare(o1.getCalories(), o2.getCalories());  
        }  
    });  

    //3.获取排序后菜肴的名字  
    List<String> lowCaloricDishesName = new ArrayList<>();  
    for (Dish d : lowCaloricDishes) {  
        lowCaloricDishesName.add(d.getName());  
    }  

    return lowCaloricDishesName;  
}
```
**Java8之后的实现方式**
```java
private List<String> afterJava8(List<Dish> dishList) {  
    return dishList.stream()  
    .filter(d -> d.getCalories() < 400)  //筛选出卡路里小于400的菜肴  
    .sorted(comparing(Dish::getCalories))  //根据卡路里进行排序  
    .map(Dish::getName)  //提取菜肴名称  
    .collect(Collectors.toList()); //转换为List  
}
```
不拖泥带水，一气呵成，原来需要写24代码实现的功能现在只需5行就可以完成了<br />高高兴兴写完需求这时候又有新需求了，新需求如下：<br />对数据库查询到的菜肴根据菜肴种类进行分类，返回一个`Map<Type, List<Dish>>`的结果<br />这要是放在JDK8之前肯定会头皮发麻<br />**Java8以前的实现方式**
```java
private static Map<Type, List<Dish>> beforeJdk8(List<Dish> dishList) {  
    Map<Type, List<Dish>> result = new HashMap<>();  

    for (Dish dish : dishList) {  
        //不存在则初始化  
        if (result.get(dish.getType())==null) {  
            List<Dish> dishes = new ArrayList<>();  
            dishes.add(dish);  
            result.put(dish.getType(), dishes);  
        } else {  
            //存在则追加  
            result.get(dish.getType()).add(dish);  
        }  
    }  

    return result;  
}
```
还好jdk8有Stream，再也不用担心复杂集合处理需求<br />**Java8以后的实现方式**
```java
private static Map<Type, List<Dish>> afterJdk8(List<Dish> dishList) {  
    return dishList.stream().collect(groupingBy(Dish::getType));  
}
```
又是一行代码解决了需求，看到流的强大功能了吧，接下来将详细介绍流
<a name="woEEK"></a>
## 什么是流
流是从支持数据处理操作的源生成的元素序列，源可以是数组、文件、集合、函数。流不是集合元素，它不是数据结构并不保存数据，它的主要目的在于计算
<a name="PykHO"></a>
## 如何生成流
生成流的方式主要有五种
<a name="B3x3d"></a>
### 1、通过集合生成，应用中最常用的一种
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
Stream<Integer> stream = integerList.stream();
```
通过集合的stream方法生成流
<a name="mB0Yj"></a>
### 2、通过数组生成
```java
int[] intArr = new int[]{1, 2, 3, 4, 5};  
IntStream stream = Arrays.stream(intArr);
```
通过`Arrays.stream`方法生成流，并且该方法生成的流是数值流【即`IntStream`】而不是`Stream<Integer>`。补充一点使用数值流可以避免计算过程中拆箱装箱，提高性能。<br />Stream API提供了`mapToInt`、`mapToDouble`、`mapToLong`三种方式将对象流【即Stream】转换成对应的数值流，同时提供了`boxed`方法将数值流转换为对象流
<a name="t7cgv"></a>
### 3、通过值生成
```java
Stream<Integer> stream = Stream.of(1, 2, 3, 4, 5);
```
通过Stream的`of`方法生成流，通过Stream的`empty`方法可以生成一个空流
<a name="Z0E2R"></a>
### 4、通过文件生成
```java
Stream<String> lines = Files.lines(Paths.get("data.txt"), Charset.defaultCharset())
```
通过`Files.line`方法得到一个流，并且得到的每个流是给定文件中的一行
<a name="uQlcY"></a>
### 5、通过函数生成 提供了`iterate`和`generate`两个静态方法从函数中生成流
<a name="F6D7B"></a>
#### `iterator`
```java
Stream<Integer> stream = Stream.iterate(0, n -> n + 2).limit(5);
```
`iterate`方法接受两个参数，第一个为初始化值，第二个为进行的函数操作，因为`iterator`生成的流为无限流，通过`limit`方法对流进行了截断，只生成5个偶数
<a name="L8FBP"></a>
#### `generate`
```java
Stream<Double> stream = Stream.generate(Math::random).limit(5);
```
`generate`方法接受一个参数，方法参数类型为`Supplier`，由它为流提供值。`generate`生成的流也是无限流，因此通过`limit`对流进行了截断
<a name="FYFfP"></a>
## 流的操作类型
流的操作类型主要分为两种<br />**1.中间操作**<br />一个流可以后面跟随零个或多个中间操作。其目的主要是打开流，做出某种程度的数据映射/过滤，然后返回一个新的流，交给下一个操作使用。这类操作都是惰性化的，仅仅调用到这类方法，并没有真正开始流的遍历，真正的遍历需等到终端操作时，常见的中间操作有下面即将介绍的filter、map等<br />**2.终端操作**<br />一个流有且只能有一个终端操作，当这个操作执行后，流就被关闭了，无法再被操作，因此一个流只能被遍历一次，若想在遍历需要通过源数据在生成流。终端操作的执行，才会真正开始流的遍历。如下面即将介绍的count、collect等
<a name="TmPn3"></a>
## 流使用
流的使用将分为终端操作和中间操作进行介绍
<a name="bRmyx"></a>
### 中间操作
<a name="uzVGD"></a>
#### `filter`筛选
```java
List<Integer> integerList = Arrays.asList(1, 1, 2, 3, 4, 5);  
Stream<Integer> stream = integerList.stream().filter(i -> i > 3);
```
通过使用`filter`方法进行条件筛选，`filter`的方法参数为一个条件
<a name="L0BYh"></a>
#### `distinct`去除重复元素
```java
List<Integer> integerList = Arrays.asList(1, 1, 2, 3, 4, 5);  
Stream<Integer> stream = integerList.stream().distinct();
```
通过`distinct`方法快速去除重复的元素
<a name="giMIs"></a>
#### `limit`返回指定流个数
```java
List<Integer> integerList = Arrays.asList(1, 1, 2, 3, 4, 5);  
Stream<Integer> stream = integerList.stream().limit(3);
```
通过`limit`方法指定返回流的个数，`limit`的参数值必须`>=0`，否则将会抛出异常
<a name="cqDCW"></a>
#### `skip`跳过流中的元素
```java
List<Integer> integerList = Arrays.asList(1, 1, 2, 3, 4, 5);  
Stream<Integer> stream = integerList.stream().skip(2);
```
通过`skip`方法跳过流中的元素，上述例子跳过前两个元素，所以打印结果为2,3,4,5，`skip`的参数值必须>=0，否则将会抛出异常
<a name="j8BM8"></a>
#### `map`流映射
所谓流映射就是将接受的元素映射成另外一个元素
```java
List<String> stringList = Arrays.asList("Java 8", "Lambdas",  "In", "Action");  
Stream<Integer> stream = stringList.stream().map(String::length);
```
通过`map`方法可以完成映射，该例子完成中String -> Integer的映射，之前上面的例子通过`map`方法完成了Dish->String的映射
<a name="dauLM"></a>
#### `flatMap`流转换
将一个流中的每个值都转换为另一个流
```java
List<String> wordList = Arrays.asList("Hello", "World");  
List<String> strList = wordList.stream()  
        .map(w -> w.split(" "))  
        .flatMap(Arrays::stream)  
        .distinct()  
        .collect(Collectors.toList());
```
`map(w -> w.split(" "))`的返回值为`Stream<String[]>`，想获取`Stream<String>`，可以通过`flatMap`方法完成`Stream` ->`Stream`的转换
<a name="dSaln"></a>
### 终端操作
<a name="IC0dl"></a>
#### 元素匹配
提供了三种匹配方式
<a name="slifL"></a>
##### `allMatch`匹配所有
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
if (integerList.stream().allMatch(i -> i > 3)) {  
    System.out.println("值都大于3");  
}
```
通过`allMatch`方法实现
<a name="JEQ4B"></a>
##### `anyMatch`匹配其中一个
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
if (integerList.stream().anyMatch(i -> i > 3)) {  
    System.out.println("存在大于3的值");  
}
```
等同于
```java
for (Integer i : integerList) {  
    if (i > 3) {  
        System.out.println("存在大于3的值");  
        break;  
    }  
}
```
存在大于3的值则打印，java8中通过`anyMatch`方法实现这个功能
<a name="F0zpV"></a>
##### `noneMatch`全部不匹配
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
if (integerList.stream().noneMatch(i -> i > 3)) {  
    System.out.println("值都小于3");  
}
```
通过`noneMatch`方法实现
<a name="ZMRXV"></a>
#### 统计流中元素个数
<a name="MwNJA"></a>
##### 通过`count`
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
Long result = integerList.stream().count();
```
通过使用`count`方法统计出流中元素个数
<a name="pxcZd"></a>
##### 通过`counting`
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
Long result = integerList.stream().collect(counting());
```
最后一种统计元素个数的方法在与`collect`联合使用的时候特别有用
<a name="TiIrn"></a>
#### 查找
提供了两种查找方式
<a name="Af123"></a>
##### `findFirst`查找第一个
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
Optional<Integer> result = integerList.stream().filter(i -> i > 3).findFirst();
```
通过`findFirst`方法查找到第一个大于三的元素并打印
<a name="h6l2C"></a>
##### `findAny`随机查找一个
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
Optional<Integer> result = integerList.stream().filter(i -> i > 3).findAny();
```
通过`findAny`方法查找到其中一个大于三的元素并打印，因为内部进行优化的原因，当找到第一个满足大于三的元素时就结束，该方法结果和`findFirst`方法结果一样。提供`findAny`方法是为了更好的利用并行流，`findFirst`方法在并行上限制更多。
<a name="VS1oR"></a>
#### 规约
<a name="wLaX1"></a>
##### `reduce`将流中的元素组合起来
假设对一个集合中的值进行求和<br />jdk8之前
```java
int sum = 0;  
for (int i : integerList) {  
    sum += i;  
}
```
jdk8之后通过`reduce`进行处理
```java
int sum = integerList.stream().reduce(0, (a, b) -> (a + b));
```
一行就可以完成，还可以使用方法引用简写成：
```java
int sum = integerList.stream().reduce(0, Integer::sum);
```
`reduce`接受两个参数，一个初始值这里是0，一个`BinaryOperator<T> accumulator`来将两个元素结合起来产生一个新值，另外`reduce`方法还有一个没有初始化值的重载方法
<a name="ixNmL"></a>
#### 获取流中最小最大值
<a name="eQIoG"></a>
##### 通过min/max获取最小最大值
```java
Optional<Integer> min = menu.stream().map(Dish::getCalories).min(Integer::compareTo);  
Optional<Integer> max = menu.stream().map(Dish::getCalories).max(Integer::compareTo);
```
也可以写成：
```java
OptionalInt min = menu.stream().mapToInt(Dish::getCalories).min();  
OptionalInt max = menu.stream().mapToInt(Dish::getCalories).max();
```
min获取流中最小值，max获取流中最大值，方法参数为`Comparator<? super T> comparator`
<a name="UgxzN"></a>
##### 通过`minBy`/`maxBy`获取最小最大值
```java
Optional<Integer> min = menu.stream().map(Dish::getCalories).collect(minBy(Integer::compareTo));  
Optional<Integer> max = menu.stream().map(Dish::getCalories).collect(maxBy(Integer::compareTo));
```
`minBy`获取流中最小值，`maxBy`获取流中最大值，方法参数为`Comparator<? super T> comparator`
<a name="vcDvC"></a>
##### 通过`reduce`获取最小最大值
```java
Optional<Integer> min = menu.stream().map(Dish::getCalories).reduce(Integer::min);  
Optional<Integer> max = menu.stream().map(Dish::getCalories).reduce(Integer::max);
```
<a name="ICKGs"></a>
#### 收集
<a name="Qh3Tq"></a>
##### 求和
<a name="FQUFu"></a>
###### 通过`summingInt`
```java
int sum = menu.stream().collect(summingInt(Dish::getCalories));
```
如果数据类型为double、long，则通过`summingDouble`、`summingLong`方法进行求和
<a name="iZ2V6"></a>
###### 通过`reduce`
```java
int sum = menu.stream().map(Dish::getCalories).reduce(0, Integer::sum);
```
<a name="tEdnJ"></a>
###### 通过`sum`
```java
int sum = menu.stream().mapToInt(Dish::getCalories).sum();
```
在上面求和、求最大值、最小值的时候，对于相同操作有不同的方法可以选择执行。可以选择collect、reduce、min/max/sum方法，推荐使用min、max、sum方法。因为它最简洁易读，同时通过`mapToInt`将对象流转换为数值流，避免了装箱和拆箱操作
<a name="ZsqyR"></a>
##### 通过`averagingInt`求平均值
```java
double average = menu.stream().collect(averagingInt(Dish::getCalories));
```
如果数据类型为double、long，则通过`averagingDouble`、`averagingLong`方法进行求平均
<a name="j8GGM"></a>
##### 通过`summarizingInt`同时求总和、平均值、最大值、最小值
```java
IntSummaryStatistics intSummaryStatistics = menu.stream().collect(summarizingInt(Dish::getCalories));  
double average = intSummaryStatistics.getAverage();  //获取平均值  
int min = intSummaryStatistics.getMin();  //获取最小值  
int max = intSummaryStatistics.getMax();  //获取最大值  
long sum = intSummaryStatistics.getSum();  //获取总和
```
如果数据类型为double、long，则通过`summarizingDouble`、`summarizingLong`方法
<a name="FfNdt"></a>
##### 通过`foreach`进行元素遍历
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
integerList.stream().forEach(System.out::println);
```
而在jdk8之前实现遍历：
```java
for (int i : integerList) {  
    System.out.println(i);  
}
```
jdk8之后遍历元素来的更为方便，原来的for-each直接通过foreach方法就能实现了
<a name="NZrBK"></a>
##### 返回集合
```java
List<String> strings = menu.stream().map(Dish::getName).collect(toList());  
Set<String> sets = menu.stream().map(Dish::getName).collect(toSet());
```
只举例了一部分，还有很多其他方法 jdk8之前
```java
List<String> stringList = new ArrayList<>();  
Set<String> stringSet = new HashSet<>();  
for (Dish dish : menu) {  
    stringList.add(dish.getName());  
    stringSet.add(dish.getName());  
}
```
通过遍历和返回集合的使用发现流只是把原来的外部迭代放到了内部进行，这也是流的主要特点之一。内部迭代可以减少好多代码量
<a name="XxxOB"></a>
##### 通过`joining`拼接流中的元素
```java
String result = menu.stream().map(Dish::getName).collect(Collectors.joining(", "));
```
默认如果不通过map方法进行映射处理拼接的toString方法返回的字符串，joining的方法参数为元素的分界符，如果不指定生成的字符串将是一串的，可读性不强
<a name="atRwj"></a>
##### 进阶通过`groupingBy`进行分组
```java
Map<Type, List<Dish>> result = dishList.stream().collect(groupingBy(Dish::getType));
```
在collect方法中传入`groupingBy`进行分组，其中`groupingBy`的方法参数为分类函数。还可以通过嵌套使用`groupingBy`进行多级分类
```java
Map<Type, List<Dish>> result = menu.stream().collect(groupingBy(Dish::getType,  
        groupingBy(dish -> {  
            if (dish.getCalories() <= 400) return CaloricLevel.DIET;  
                else if (dish.getCalories() <= 700) return CaloricLevel.NORMAL;  
                else return CaloricLevel.FAT;  
        })));
```
<a name="mb8uO"></a>
##### 进阶通过`partitioningBy`进行分区
分区是特殊的分组，它分类依据是true和false，所以返回的结果最多可以分为两组
```java
Map<Boolean, List<Dish>> result = menu.stream().collect(partitioningBy(Dish :: isVegetarian))
```
等同于
```java
Map<Boolean, List<Dish>> result = menu.stream().collect(groupingBy(Dish :: isVegetarian))
```
这个例子可能并不能看出分区和分类的区别，甚至觉得分区根本没有必要，换个明显一点的例子：
```java
List<Integer> integerList = Arrays.asList(1, 2, 3, 4, 5);  
Map<Boolean, List<Integer>> result = integerList.stream().collect(partitioningBy(i -> i < 3));
```
返回值的键仍然是布尔类型，但是它的分类是根据范围进行分类的，分区比较适合处理根据范围进行分类
<a name="U7caB"></a>
## 总结
通过使用Stream API可以简化代码，同时提高了代码可读性，使代码更加简洁、优雅。
