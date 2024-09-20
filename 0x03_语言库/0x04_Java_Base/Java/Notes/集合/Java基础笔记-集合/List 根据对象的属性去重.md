Java List
<a name="EEmuM"></a>
### 一、去除List中重复的String
```java
public List<String> removeStringListDupli(List<String> stringList) {
    Set<String> set = new LinkedHashSet<>();
    set.addAll(stringList);

    stringList.clear();

    stringList.addAll(set);
    return stringList;
}
```
或使用Java8的写法：
```java
List<String> unique = list.stream().distinct().collect(Collectors.toList());
```
<a name="P0DCB"></a>
### 二、List中对象去重
比如现在有一个 Person类:
```java
public class Person {
	private Long id;
	
	private String name;
	
	public Person(Long id, String name) {
		this.id = id;
		this.name = name;
	}
	
	public Long getId() {
		return id;
	}
	
	public void setId(Long id) {
		this.id = id;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	@Override
	public String toString() {
		return "Person{" +
			"id=" + id +
			", name='" + name + '\'' +
			'}';
	}
}
```
重写Person对象的`equals()`方法和`hashCode()`方法:
```java
@Override
public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    Person person = (Person) o;

    if (!id.equals(person.id)) return false;
    return name.equals(person.name);

}

@Override
public int hashCode() {
    int result = id.hashCode();
    result = 31 * result + name.hashCode();
    return result;
}
```
下面对象去重的代码：
```java
Person p1 = new Person(1l, "jack");
Person p2 = new Person(3l, "jack chou");
Person p3 = new Person(2l, "tom");
Person p4 = new Person(4l, "hanson");
Person p5 = new Person(5l, "胶布虫");

List<Person> persons = Arrays.asList(p1, p2, p3, p4, p5, p5, p1, p2, p2);

List<Person> personList = new ArrayList<>();
// 去重
persons.stream().forEach(
	p -> {
		if (!personList.contains(p)) {
			personList.add(p);
		}
	}
);
System.out.println(personList);
```
List 的`contains()`方法底层实现使用对象的equals方法去比较的，其实重写`equals()`就好，但重写了equals最好将hashCode也重写了。
<a name="h31DW"></a>
### 三、根据对象的属性去重
下面要根据Person对象的id去重，那该怎么做呢？<br />写一个方法吧:
```java
public static List<Person> removeDupliById(List<Person> persons) {
    Set<Person> personSet = new TreeSet<>((o1, o2) -> o1.getId().compareTo(o2.getId()));
    personSet.addAll(persons);

    return new ArrayList<>(personSet);
}
```
通过`Comparator`比较器，比较对象属性，相同就返回0，达到过滤的目的。<br />再来看比较炫酷的Java8写法:
```java
import static java.util.Comparator.comparingLong;
import static java.util.stream.Collectors.collectingAndThen;
import static java.util.stream.Collectors.toCollection;

// 根据id去重
 List<Person> unique = persons.stream().collect(
            collectingAndThen(
                    toCollection(() -> new TreeSet<>(comparingLong(Person::getId))), ArrayList::new)
    );
```
还有一种写法:
```java
public static <T> Predicate<T> distinctByKey(Function<? super T, Object> keyExtractor) {
    Map<Object, Boolean> map = new ConcurrentHashMap<>();
    return t -> map.putIfAbsent(keyExtractor.apply(t), Boolean.TRUE) == null;
}

// remove duplicate
    persons.stream().filter(distinctByKey(p -> p.getId())).forEach(p -> System.out.println(p));

```
