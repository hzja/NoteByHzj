JavaStream
<a name="dZ70r"></a>
## 1、Stream 的`distinct()`方法
`distinct()`是Java 8 中 Stream 提供的方法，返回的是由该流中不同元素组成的流。`distinct()`使用 `hashCode()` 和 `eqauls()` 方法来获取不同的元素。<br />因此，需要去重的类必须实现 `hashCode()` 和 `equals()` 方法。换句话讲，可以通过重写定制的 `hashCode()` 和 `equals()` 方法来达到某些特殊需求的去重。<br />`distinct()` 方法声明如下：
```java
Stream<T> distinct();
```
<a name="MYqVT"></a>
### 1.1 对于 String 列表的去重
因为 String 类已经覆写了 `equals()` 和 `hashCode()` 方法，所以可以去重成功。
```java
@Test
public void listDistinctByStreamDistinct() {
	// 1. 对于 String 列表去重
	List<String> stringList = new ArrayList<String>() {{
		add("A");
		add("A");
		add("B");
		add("B");
		add("C");
	}};
	out.print("去重前：");
	for (String s : stringList) {
		out.print(s);
	}
	out.println();
	stringList = stringList.stream().distinct().collect(Collectors.toList());
	out.print("去重后：");
	for (String s : stringList) {
		out.print(s);
	}
	out.println();
}
```
结果如下：
```
去重前：AABBC
去重后：ABC
```
<a name="PI6RA"></a>
### 1.2 对于实体类列表的去重
注：代码中使用了 Lombok 插件的 `@Data`解，可自动覆写 `equals()` 以及 `hashCode()` 方法。
```java
/**
* 定义一个实体类
*/ 
@Data
public class Student {
	private String stuNo;
	private String name;
}

@Test
public void listDistinctByStreamDistinct() throws JsonProcessingException {
	ObjectMapper objectMapper = new ObjectMapper();
	// 1. 对于 Student 列表去重
	List<Student> studentList = getStudentList();
	out.print("去重前：");
	out.println(objectMapper.writeValueAsString(studentList));
	studentList = studentList.stream().distinct().collect(Collectors.toList());
	out.print("去重后：");
	out.println(objectMapper.writeValueAsString(studentList));
}
```
结果如下：
```
去重前：[{"stuNo":"001","name":"Tom"},{"stuNo":"002","name":"Mike"},{"stuNo":"001","name":"Tom"}]
去重后：[{"stuNo":"001","name":"Tom"},{"stuNo":"002","name":"Mike"}]
```
<a name="hcBWK"></a>
## 2、根据 `List<Object>` 中 Object 某个属性去重
<a name="HhJnI"></a>
### 2.1 新建一个列表出来
```java
@Test
public void distinctByProperty1() throws JsonProcessingException {
	// 这里第一种方法我通过新创建一个只有不同元素列表来实现根据对象某个属性去重
	ObjectMapper objectMapper = new ObjectMapper();
	List<Student> studentList = getStudentList();
	
	out.print("去重前        :");
	out.println(objectMapper.writeValueAsString(studentList));
	studentList = studentList.stream().distinct().collect(Collectors.toList());
	out.print("distinct去重后:");
	out.println(objectMapper.writeValueAsString(studentList));
	// 这里我们引入了两个静态方法，以及通过 TreeSet<> 来达到获取不同元素的效果
	// 1. import static java.util.stream.Collectors.collectingAndThen;
	// 2. import static java.util.stream.Collectors.toCollection;
	studentList = studentList.stream().collect(
		collectingAndThen(
			toCollection(() -> new TreeSet<>(Comparator.comparing(Student::getName))), ArrayList::new)
	);
	out.print("根据名字去重后 :");
	out.println(objectMapper.writeValueAsString(studentList));
}
```
结果如下：
```
去重前        :[{"stuNo":"001","name":"Tom"},{"stuNo":"001","name":"Tom"},{"stuNo":"003","name":"Tom"}]
distinct去重后:[{"stuNo":"001","name":"Tom"},{"stuNo":"003","name":"Tom"}]
根据名字去重后 :[{"stuNo":"001","name":"Tom"}]
```
<a name="cMo8G"></a>
### 2.2 通过 `filter()` 方法
首先创建一个方法作为 `Stream.filter()` 的参数，其返回类型为 Predicate，原理就是判断一个元素能否加入到 Set 中去，代码如下：
```java
private static <T> Predicate<T> distinctByKey(Function<? super T, ?> keyExtractor) {
    Set<Object> seen = ConcurrentHashMap.newKeySet();
    return t -> seen.add(keyExtractor.apply(t));
}
```
使用如下：
```java
@Test
public void distinctByProperty2() throws JsonProcessingException {
	// 这里第二种方法我们通过过滤来实现根据对象某个属性去重
	ObjectMapper objectMapper = new ObjectMapper();
	List<Student> studentList = getStudentList();
	
	out.print("去重前        :");
	out.println(objectMapper.writeValueAsString(studentList));
	studentList = studentList.stream().distinct().collect(Collectors.toList());
	out.print("distinct去重后:");
	out.println(objectMapper.writeValueAsString(studentList));
	// 这里我们将 distinctByKey() 方法作为 filter() 的参数，过滤掉那些不能加入到 set 的元素
	studentList = studentList.stream().filter(distinctByKey(Student::getName)).collect(Collectors.toList());
	out.print("根据名字去重后 :");
	out.println(objectMapper.writeValueAsString(studentList));
}
```
结果如下：
```
去重前        :[{"stuNo":"001","name":"Tom"},{"stuNo":"001","name":"Tom"},{"stuNo":"003","name":"Tom"}]
distinct去重后:[{"stuNo":"001","name":"Tom"},{"stuNo":"003","name":"Tom"}]
根据名字去重后 :[{"stuNo":"001","name":"Tom"}]
```
