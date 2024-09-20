Java Stream<br />Java8 中增加了 Stream 处理，可以配合 Lambda 表达式来使用，让操作集合非常便利。虽然平时经常使用 Stream，但用到的方法其实非常少，来完整的介绍 Stream 的使用。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400168-b2f1ebbf-7c44-4ee7-b000-415fedecac9c.png#clientId=uad0cbc05-1409-4&from=paste&id=uf30cf525&originHeight=194&originWidth=525&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3073cf3c-5a73-4e21-848c-2129f265b43&title=)<br />Stream 提供的方法非常多，按照调用当前方法是否结束流处理，可以分为中间操作和结束操作。<br />对于中间操作，又可以分为有状态的操作和无状态操作：

- 无状态的操作是指当前元素的操作不受前面元素的影响。
- 有状态的操作是指当前元素的操作需要等所有元素处理完之后才能进行。

对于结束操作，又可以分为短路操作和非短路操作，具体如下：

- 短路操作是指不需要处理完全部的元素就可以结束。
- 非短路操作是指必须处理完所有元素才能结束。
<a name="U3LR3"></a>
## 1、创建 Stream
<a name="dbGhF"></a>
### 1.1 使用集合创建
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
Stream stream = list.stream();
```
<a name="aVWFx"></a>
### 1.2 使用数组创建
```java
String[] array={"ab", "abc", "abcd", "abcde", "abcdef" };
Stream<String> stream = Arrays.stream(array);
```
<a name="wEPn4"></a>
### 1.3 使用 Stream 静态方法
```java
Stream<String> stream = Stream.of("ab", "abc", "abcd", "abcde", "abcdef");

Stream<Integer> stream2 = Stream.iterate(0, (x) -> x + 3).limit(5);
stream2.forEach(r -> System.out.print(r + " "));

System.out.println();

Stream<Integer> stream3 = Stream.generate(new Random()::nextInt).limit(3);
stream3.forEach(r -> System.out.print(r + " "));
```
上面代码输出如下：
```
0 3 6 9 12 
-150231306 -1769565695 102740625
```
<a name="iziob"></a>
## 2、无状态操作
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400088-50637624-3121-4b8f-8a33-d608562f8119.png#clientId=uad0cbc05-1409-4&from=paste&id=u770f9fb3&originHeight=694&originWidth=557&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud0747237-ee1a-4230-bead-1973f3152e1&title=)
<a name="NGGKh"></a>
### 2.1 map
接收一个函数作为入参，把这个函数应用到每个元素上，执行结果组成一个新的 stream 返回。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400307-744b5f5e-c3d6-4266-ac87-1fb9f56a08fd.png#clientId=uad0cbc05-1409-4&from=paste&id=u1751b57a&originHeight=151&originWidth=321&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3f618772-0087-4fa8-9c55-2c526a89700&title=)<br />案例 1：对整数数组每个元素加 3 ：
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
List<Integer> newList = list.stream().map(x -> x + 3).collect(Collectors.toList());
System.out.println("newList:" + newList);
```
上面代码输出结果如下：
```
newList:[8, 5, 6, 4, 7]
```
案例 2：把字符串数组的每个元素转换为大写：
```java
List<String> list = Arrays.asList("ab", "abc", "abcd", "abcde", "abcdef");
List<String> newList = list.stream().map(String::toUpperCase).collect(Collectors.toList());
System.out.println("newList:" + newList);
```
上面代码输出结果如下：
```
newList:[AB, ABC, ABCD, ABCDE, ABCDEF]
```
<a name="bqBQt"></a>
### 2.2 `mapToXXX`
包括三个方法：`mapToInt`、`mapToDouble`、`mapToLong`<br />案例 3：把字符串数组转为整数数组：
```java
List<String> list = Arrays.asList("ab", "abc", "abcd", "abcde", "abcdef");
int[] newList = list.stream().mapToInt(r -> r.length()).toArray();
System.out.println("newList:" + Arrays.toString(newList));
```
上面代码输出结果如下：
```
newList:[2, 3, 4, 5, 6]
```
<a name="iYLbk"></a>
### 2.3 flatMap
flatMap接收函数作为入参，然后把集合中每个元素转换成一个 stream，再把这些 stream 组成一个新的 stream，是拆分单词很好的工具。如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400107-cc69f0fa-c7fa-469e-a1f8-359d9abe2b20.png#clientId=uad0cbc05-1409-4&from=paste&id=u43a2c1d2&originHeight=251&originWidth=741&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3e4a1681-cd6e-41e2-84f7-bdf3ae55534&title=)<br />案例 4：把一个字符串数组转成另一个字符串数组：
```java
List<String> list = Arrays.asList("ab-abc-abcd-abcde-abcdef", "5-2-3-1-4");
List<String> newList = list.stream().flatMap(s -> Arrays.stream(s.split("-"))).collect(Collectors.toList());
System.out.println("newList：" + newList);
```
上面代码输出结果：
```
newList：[ab, abc, abcd, abcde, abcdef, 5, 2, 3, 1, 4]
```
<a name="fx2D8"></a>
### 2.4 `flatMapToXXX`
类似于 `flatMap`，返回一个 XXXStream。<br />包括三个方法：`flatMapToInt`、`flatMapToLong`、`flatMapToDouble`<br />案例 5：对给定的二维整数数组求和:
```java
int[][] data = {{1,2},{3,4},{5,6}};
IntStream intStream = Arrays.stream(data).flatMapToInt(row -> Arrays.stream(row));
System.out.println(intStream.sum());
```
输出结果为：21。
<a name="x2HjM"></a>
### 2.5 filter
筛选功能，按照一定的规则将符合条件的元素提取到新的流中。<br />定义一个学生类，包含姓名、年龄、性别、考试成绩四个属性：
```java
class Student{
    private String name;
    private Integer age;
    private String sex;
    private Integer score;

    public Student(String name, Integer age, String sex, Integer score){
        this.name = name;
        this.age = age;
        this.score = score;
        this.sex = sex;
    }
    //省略getters/setters
}
```
案例 6：找出考试成绩在 90 分以上的学生姓名：
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 13,"male", 90));
students.add(new Student("Lucy", 15,"female", 100));
students.add(new Student("Jessie", 12,"female", 78));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 22,"female", 50));

List<String> nameList = students.stream().filter(x -> x.getScore() >= 90).map(Student::getName).collect(Collectors.toList());
System.out.print("考试成绩90分以上的学生姓名：" + nameList);
```
输出如下：
```
考试成绩90分以上的学生姓名：[Jack, Lucy, Allon]
```
<a name="SMuAc"></a>
### 2.6 peek
返回由 stream 中元素组成的新 stream，用给定的函数作用在新 stream 的每个元素上。传入的函数是一个 Consume 类型的，没有返回值，因此并不会改变原 stream 中元素的值。peek 主要用是 debug，可以方便地 查看流处理结果是否正确。<br />案例 7：过滤出 stream 中长度大于 3 的字符串并转为大写：
```java
Stream.of("one", "two", "three", "four")
             .filter(e -> e.length() > 3)
             .peek(e -> System.out.println("Filtered value: " + e))
             .map(String::toUpperCase)
             .peek(e -> System.out.println("Mapped value: " + e))
             .collect(Collectors.toList());
```
输出结果如下：
```
Filtered value: three 
Mapped value: THREE 
Filtered value: four 
Mapped value: FOUR
```
<a name="rRHhr"></a>
### 2.7 unordered
把一个有序的 stream 转成一个无序 stream ，如果原 stream 本身就是无序的，可能会返回原始的 stream。<br />案例 8：把一个有序数组转成无序
```java
Arrays.asList("1", "2", "3", "4", "5")
                .parallelStream()
                .unordered()
                .forEach(r -> System.out.print(r + " "));
```
每次执行输出的结果不一样，下面是一次输出的结果：
```
3 5 4 2 1
```
<a name="kDz8Q"></a>
## 3、有状态操作
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400179-25ae6672-39b8-482d-bdaf-8c3729f4c2df.png#clientId=uad0cbc05-1409-4&from=paste&id=u31e2cbce&originHeight=280&originWidth=424&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7b2fe093-3b72-4890-baef-b8e843b37a4&title=)
<a name="zYZXs"></a>
### 3.1 `distinct`
去重功能。<br />案例 9 ：去掉字符串数组中的重复字符串
```java
String[] array = { "a", "b", "b", "c", "c", "d", "d", "e", "e"};
List<String> newList = Arrays.stream(array).distinct().collect(Collectors.toList());
System.out.println("newList:" + newList);
```
输出结果：
```
newList:[a, b, c, d, e]
```
<a name="MkcoK"></a>
### 3.2 `limit`
限制从 stream 中获取前 n 个元素。<br />案例 10 ：从数组中获取前 5 个元素
```java
String[] array = { "c", "c", "a", "b", "b", "e", "e", "d", "d"};
List<String> newList = Arrays.stream(array).limit(5).collect(Collectors.toList());
System.out.println("newList:" + newList);
```
输出结果：
```
newList:[c, c, a, b, b]
```
<a name="TjICF"></a>
### 3.3 `skip`
跳过 Stream 中前 n 个元素<br />案例 11：从数组中获取第 5 个元素之后的元素
```java
String[] array = { "a", "b", "c", "d", "e", "f", "g", "h", "i"};
List<String> newList = Arrays.stream(array).skip(5).collect(Collectors.toList());
System.out.println("newList:" + newList);
```
输出结果：
```
newList:[f, g, h, i]
```
<a name="SAGzN"></a>
### 3.4 `sorted`
排序功能。<br />案例 12：对给定数组进行排序
```java
String[] array = { "c", "c", "a", "b", "b", "e", "e", "d", "d"};
List<String> newList = Arrays.stream(array).sorted().collect(Collectors.toList());
System.out.println("newList:" + newList);
```
输出结果：
```
newList:[a, b, b, c, c, d, d, e, e]
```
案例 13：按照学生成绩进行排序
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 13,"male", 90));
students.add(new Student("Lucy", 15,"female", 100));
students.add(new Student("Jessie", 12,"female", 78));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 22,"female", 50));

List<String> nameList = students.stream().sorted(Comparator.comparing(Student::getScore)).map(Student::getName).collect(Collectors.toList());
System.out.print("按成绩排序输出学生姓名：" + nameList);
```
输出结果：
```
考试成绩90分以上的学生姓名：[Alis, Jessie, Mike, Jack, Allon, Lucy]
```
<a name="jto38"></a>
## 4、短路操作
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400378-52a6960c-c67f-44bf-980f-aea909e7783d.png#clientId=uad0cbc05-1409-4&from=paste&id=ua66c2595&originHeight=340&originWidth=437&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u631541c9-c530-472e-9365-6d07a7d9bae&title=)
<a name="iZR8R"></a>
### 4.1 `findAny`
找出 stream 中任何一个满足过滤条件的元素。<br />案例 14：找出任何一个成绩高于 90 分的学生
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 13,"male", 90));
students.add(new Student("Lucy", 15,"female", 100));
students.add(new Student("Jessie", 12,"female", 78));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 22,"female", 50));

Optional<Student> studentFindAny = students.stream().filter(x -> x.getScore() > 90).findAny();
System.out.print("找出任意一个考试成绩在90分以上的学生姓名：" + studentFindAny.orElseGet(null).getName());
```
输出结果：
```
找出任意一个考试成绩在90分以上的学生姓名：Lucy
```
<a name="ZGYHQ"></a>
### 4.2 `anyMatch`
是否存在任意一个满足给定条件的元素。<br />案例 15：是否存在成绩高于 90 分的学生，是否存在成绩低于 50 分的学生。还是采用上面案例 14 中的学生集合。
```java
boolean result1 = students.stream().anyMatch(x -> x.getScore() > 90);
System.out.println("是否存在成绩高于 90 分的学生：" + result1);
boolean result2 = students.stream().anyMatch(x -> x.getScore() < 50);
System.out.print("是否存在成绩低于 50 分的学生：" + result2);
```
输出结果：
```
是否存在成绩高于 90 分的学生：true
是否存在成绩低于 50 分的学生：false
```
<a name="iE8DI"></a>
### 4.3 `allMatch`
是否集合中所有元素都满足给定的条件，如果集合是空，则返回 true。<br />案例 16：学生成绩是否都高于 90 分，是否都高于 50 分。还是采用上面案例 14 中的学生集合。
```java
boolean result1 = students.stream().allMatch(x -> x.getScore() > 90);
System.out.println("是否所有学生的成绩都高于90分：" + result1);
boolean result2 = students.stream().allMatch(x -> x.getScore() > 50);
System.out.print("是否所有学生的成绩都高于50分：" + result2);
```
输出结果：<br />是否所有学生的成绩都高于90分：false<br />是否所有学生的成绩都高于50分：true
<a name="s7Gfa"></a>
### 4.4 `noneMatch`
是否没有元素能匹配给定的条件，如果集合是空，则返回 true。<br />案例 17：是不是没有学生成绩在 90 分以上，是否没有学生成绩在 50 分以下。还是采用上面案例 14 中的学生集合。
```java
boolean result1 = students.stream().noneMatch(x -> x.getScore() > 90);
System.out.println("是不是没有学生成绩在 90 分以上：" + result1);
boolean result2 = students.stream().noneMatch(x -> x.getScore() < 50);
System.out.print("是不是没有学生成绩在 50 分以下：" + result2);
```
输出结果：
```
是不是没有学生成绩在 90 分以上：false
是不是没有学生成绩在 50 分以下：true
```
<a name="rM8iM"></a>
### 4.5 `findFirst`
找出第一个符合条件的元素。<br />案例 18：找出第一个成绩在 90 分以上的学生。还是采用上面案例 14 中的学生集合。
```java
Optional<Student> studentFindAny = students.stream().filter(x -> x.getScore() > 90).findFirst();
System.out.print("第一个成绩在 90 分以上的学生姓名：" + studentFindAny.orElseGet(null).getName());
```
输出结果：
```
第一个成绩在 90 分以上的学生姓名：Lucy
```
<a name="RWHPd"></a>
## 5、非短路操作
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400433-5c71183c-6c26-49bc-801a-63ad6a585af3.png#clientId=uad0cbc05-1409-4&from=paste&id=uf598671d&originHeight=520&originWidth=545&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uccf88a24-d92f-45bf-b3f9-c5026f9db8a&title=)
<a name="pw46D"></a>
### 5.1 `forEach`
遍历元素。<br />案例 19：遍历一个数组并打印
```java
List<Integer> array = Arrays.asList(5, 2, 3, 1, 4);
array.stream().forEach(System.out :: println);
```
输出结果：
```
5 2 3 1 4
```
<a name="FKQDT"></a>
### 5.2 `forEachOrdered`
按照给定集合中元素的顺序输出。主要使用场景是在并行流的情况下，按照给定的顺序输出元素。<br />案例 20：用并行流遍历一个数组并按照给定数组的顺序输出结果
```java
List<Integer> array = Arrays.asList(5, 2, 3, 1, 4);
array.parallelStream().forEachOrdered(System.out :: println);
```
输出结果：
```
5 2 3 1 4
```
<a name="UDKgT"></a>
### 5.3 `toArray`
返回包括给定 stream 中所有元素的数组。<br />案例 21：把给定字符串流转化成数组
```java
Stream<String> stream = Arrays.asList("ab", "abc", "abcd", "abcde", "abcdef").stream();
String[] newArray1 = stream.toArray(str -> new String[5]);
String[] newArray2 = stream.toArray(String[]::new);
Object[] newArray3 = stream.toArray();
```
<a name="CJTBv"></a>
### 5.4 `reduce`
规约操作，把一个流的所有元素合并成一个元素，比如求和、求乘积、求最大最小值等。<br />案例 22：求整数数组元素之和、乘积和最大值
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
Optional<Integer> sum = list.stream().reduce((x, y) -> x + y);
Optional<Integer> product = list.stream().reduce((x, y) -> x * y);
Optional<Integer> max = list.stream().reduce((x, y) -> x > y ? x : y);
System.out.println("数组元素之和：" + sum.get());
System.out.println("数组元素乘积：" + product.get());
System.out.println("数组元素最大值：" + max.get());
```
输出结果：
```
数组元素之和：15
数组元素乘积：120 
数组元素最大值：5
```
案例 23：求全班学生最高分、全班学生总分
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 13,"male", 90));
students.add(new Student("Lucy", 15,"female", 100));
students.add(new Student("Jessie", 12,"female", 78));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 22,"female", 50));
Optional<Integer> maxScore = students.stream().map(r -> r.getScore()).reduce(Integer::max);
Optional<Integer> sumScore = students.stream().map(r -> r.getScore()).reduce(Integer::sum);
System.out.println("全班学生最高分：" + maxScore.get());
System.out.println("全班学生总分：" + sumScore.get());
```
输出结果：
```
全班学生最高分：100 
全班学生总分：498
```
<a name="pEl3d"></a>
### 5.5 collect
把 stream 中的元素归集到新的集合或者归集成单个元素。
<a name="qWCIe"></a>
#### 5.5.1 归集成新集合
方法包括 `toList`、`toSet`、`toMap`。<br />案例 24：根据学生列表，归纳出姓名列表、不同分数列表、姓名分数集合，其中 Mike 和 Jessie 的分数都是 88。
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 13,"male", 90));
students.add(new Student("Lucy", 15,"female", 100));
students.add(new Student("Jessie", 12,"female", 88));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 22,"female", 50));
List<String> list = students.stream().map(r -> r.getName()).collect(Collectors.toList());
Set<Integer> set = students.stream().map(r -> r.getScore()).collect(Collectors.toSet());
Map<String, Integer> map = students.stream().collect(Collectors.toMap(Student::getName, Student::getScore));
System.out.println("全班学生姓名列表：" + list);
System.out.println("全班学生不同分数列表：" + set);
System.out.println("全班学生姓名分数集合：" + map);
```
输出结果：
```
全班学生姓名列表：[Mike, Jack, Lucy, Jessie, Allon, Alis] 
全班学生不同分数列表：[50, 100, 88, 90, 92] 
全班学生姓名分数集合：{Mike=88, Allon=92, Alis=50, Lucy=100, Jack=90, Jessie=88}
```
<a name="PrzTo"></a>
#### 5.5.2 统计功能
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400570-b12538a6-4a79-4105-9c15-69e014570f12.png#clientId=uad0cbc05-1409-4&from=paste&id=uce4373fa&originHeight=259&originWidth=379&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud29c517c-ad58-42db-add4-88e531979d5&title=)<br />统计功能包括如下方法：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648794400593-fc008187-4a1d-40b2-b3d4-39a63a2783f6.png#clientId=uad0cbc05-1409-4&from=paste&id=uf807cf41&originHeight=355&originWidth=901&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d7a4f58-6fb4-4513-8191-b8cf728b13e&title=)<br />案例 25：求总数、求和、最大/最小/平均值
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
long count = list.stream().collect(Collectors.counting());
int sum = list.stream().collect(Collectors.summingInt(r -> r));
double average = list.stream().collect(Collectors.averagingDouble(r -> r));
Optional<Integer> max = list.stream().collect(Collectors.maxBy(Integer::compare));
Optional<Integer> min = list.stream().collect(Collectors.maxBy((x, y) -> x > y ? y : x));
System.out.println("总数:" + count);
System.out.println("总和:" + sum);
System.out.println("平均值:" + average);
System.out.println("最大值:" + max.get());
System.out.println("最小值:" + min.get());
```
输出结果：
```
总数:5 
总和:15 
平均值:3.0 
最大值:5 
最小值:5
```
案例 26：求总和统计
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
IntSummaryStatistics statistics = list.stream().collect(Collectors.summarizingInt(r -> r));
System.out.println("综合统计结果：" + statistics.toString());
```
输出结果：
```
综合统计结果：IntSummaryStatistics{count=5, sum=15, min=1, average=3.000000, max=5}
```
<a name="GDwog"></a>
#### 5.5.3 分区和分组
主要包括两个函数：

- `partitioningBy`：把 stream 分成两个 map
- `groupingBy`：把 stream 分成多个 map

案例 27：将学生按照 80 分以上和以下分区
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 10,"male", 90));
students.add(new Student("Lucy", 12,"female", 100));
students.add(new Student("Jessie", 12,"female", 78));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 16,"female", 50));
Map<Boolean, List<Student>> partitionByScore = students.stream().collect(Collectors.partitioningBy(x -> x.getScore() > 80));
System.out.println("将学生按照考试成绩80分以上分区：");
partitionByScore.forEach((k,v ) -> {
    System.out.print(k ? "80分以上：" : "80分以下：");
    v.forEach(r -> System.out.print(r.getName() + ","));
    System.out.println();
});
System.out.println();
```
分区结果是把 Student 列表分成 key 只有 true 和 false 两个值的 map，输出如下：<br />将学生按照考试成绩80分以上分区：
```
80分以下：Jessie,Alis,
80分以上：Mike,Jack,Lucy,Allon,
```
案例 28：将学生按照性别、年龄分组
```java
Map<String, Map<Integer, List<Student>>> group = students.stream().collect(Collectors.groupingBy(Student::getSex, Collectors.groupingBy(Student::getAge)));
System.out.println("将学生按照性别、年龄分组：");
group.forEach((k,v ) -> {
    System.out.println(k +"：");
    v.forEach((k1,v1) -> {
        System.out.print("      " + k1 + ":" );
        v1.forEach(r -> System.out.print(r.getName() + ","));
        System.out.println();
    });
});
```
输出如下：
```
将学生按照性别、年龄分组：
female：
16:Allon,Alis, 
12:Lucy,Jessie, 
male：
10:Mike,Jack,
```
<a name="VCBMS"></a>
#### 5.5.4 连接
将 stream 中的元素用指定的连接符合并，连接符可以是空。<br />案例 29：输出所有学生的姓名，用逗号分隔，这里还是使用案例 27 中的学生列表
```java
String studentNames = students.stream().map(r -> r.getName()).collect(Collectors.joining(","));
System.out.println("所有学生姓名列表：" + studentNames);
```
输出如下：
```
所有学生姓名列表：Mike,Jack,Lucy,Jessie,Allon,Alis
```
<a name="yBWb1"></a>
#### 5.5.5 规约
在 5.4 节已经讲过规约了，这里的规约支持更强大的自定义规约。<br />案例 30：数组中每个元素加 1 后求总和
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
int listSum = list.stream().collect(Collectors.reducing(0, x -> x + 1, (sum, b) -> sum + b));
System.out.println("数组中每个元素加 1 后总和：" + listSum);
```
输出结果：
```
数组中每个元素加 1 后总和：20
```
<a name="d2qVN"></a>
### 5.6 `max`、`min`、`count`
stream 提供的方便统计的方法。<br />案例 31：统计整数数组中最大值、最小值、大于 3 的元素个数
```java
List<Integer> list = Arrays.asList(5, 2, 3, 1, 4);
System.out.println("数组元素最大值："+list.stream().max(Integer::compareTo).get());
System.out.println("数组元素最小值："+list.stream().min(Integer::compareTo).get());
System.out.println("数组中大于3的元素个数："+list.stream().filter(x -> x > 3).count());
```
输出结果：
```
数组元素最大值：5
数组元素最小值：1
数组中大于3的元素个数：2
```
案例 32：统计分数最高的学生姓名
```java
List<Student> students = new ArrayList<>();
students.add(new Student("Mike", 10, "male", 88));
students.add(new Student("Jack", 10,"male", 90));
students.add(new Student("Lucy", 12,"female", 100));
students.add(new Student("Jessie", 12,"female", 78));
students.add(new Student("Allon", 16,"female", 92));
students.add(new Student("Alis", 16,"female", 50));
Optional<Student> optional = students.stream().max(Comparator.comparing(r -> r.getScore()));
System.out.println("成绩最高的学生姓名：" + optional.get().getName());
```
输出结果：
```
成绩最高的学生姓名：Lucy
```
