Java<br />`naturalOrder`是比较器功能接口的静态方法。<br />Java 8中引入的`Comparator.naturalOrder`方法返回一个比较器，该比较器以自然顺序比较可比较对象。<br />对于自然排序，一个类需要实现`Comparable`并定义`compareTo`方法。<br />一个对象的集合根据`compareTo`方法以自然排序进行排序。<br />像Integer、String和Date这样的Java类实现了`Comparable`接口并覆盖了其`compareTo`方法，它们以词表顺序(lexicographic-order)进行排序。<br />从Java源代码中找到`naturalOrder`方法声明。
```java
static <T extends Comparable<? super T>> Comparator<T> naturalOrder()
```
为了扭转自然排序，可以使用`Comparator.reverseOrder`方法。<br />提供一些例子，说明如何使用`Comparator.naturalOrder`与`Stream.sorted`、`Collections.sort`、`List.sort`和`Arrays.sort`一起对对象的集合进行自然排序。
<a name="Fn5WI"></a>
### 与 `Stream.sorted` 一起使用
`Stream.sorted`返回一个由这个流的元素组成的流，根据提供的比较器进行排序。<br />StreamSortedDemo.java
```java
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
public class StreamSortedDemo {
	public static void main(String[] args) {
		List<Integer> numList = Arrays.asList(12, 10, 15, 8, 11);
		numList.stream().sorted(Comparator.naturalOrder()).forEach(n -> System.out.print(n + " "));
		System.out.println("\n-----------");
		
		List<String> strList = Arrays.asList("Varanasi", "Allahabad", "Kanpur", "Noida");
		strList.stream().sorted(Comparator.naturalOrder()).forEach(s -> System.out.print(s + " "));
		System.out.println("\n-----------"); 
		
		List<Student> stdList = Student.getStudentList();
		stdList.stream().sorted(Comparator.naturalOrder()).forEach(s -> System.out.print(s.getName() + " "));
		System.out.println("\n-----------"); 
		
		//reverse order of natural order using Comparator.reverseOrder()
		stdList = Student.getStudentList();
		stdList.stream().sorted(Comparator.reverseOrder()).forEach(s -> System.out.print(s.getName() + " ")); 
	}
} 
```
Student.java
```java
import java.util.Arrays;
import java.util.List;
public class Student implements Comparable<Student> {
	private String name;
	private int age;
	public Student(String name, int age) {
		this.name = name;
		this.age = age;
	}
	public String getName() {
		return name;
	}
	public int getAge() {
		return age;
	}
	@Override
	public int compareTo(Student s) {
		return name.compareTo(s.getName());
	}
	public static List<Student> getStudentList() {
		Student s1 = new Student("Ram", 18);
		Student s2 = new Student("Shyam", 22);
		Student s3 = new Student("Mohan", 19);
		Student s4 = new Student("Mahesh", 20);
		Student s5 = new Student("Krishna", 21);
		List<Student> list = Arrays.asList(s1, s2, s3, s4, s5);
		return list;
	}
} 
```
输出
```java
8 10 11 12 15 
-----------
Allahabad Kanpur Noida Varanasi 
-----------
Krishna Mahesh Mohan Ram Shyam 
-----------
Shyam Ram Mohan Mahesh Krish 
```
<a name="ZPKx2"></a>
### 与 `Collections.sort` 一起使用
`Collections.sort`根据给定的比较器实例对指定的列表进行排序。<br />CollectionsSortDemo.java
```java
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
public class CollectionsSortDemo {
	public static void main(String[] args) {
		List<Integer> numList = Arrays.asList(12, 10, 15, 8, 11);
		Collections.sort(numList, Comparator.naturalOrder());
		numList.forEach(n -> System.out.print(n + " "));
		System.out.println("\n-----------");
		
		List<String> strList = Arrays.asList("Varanasi", "Allahabad", "Kanpur", "Noida");
		Collections.sort(strList, Comparator.naturalOrder());
		strList.forEach(s -> System.out.print(s + " "));
		System.out.println("\n-----------"); 
		
		List<Student> stdList = Student.getStudentList();
		Collections.sort(stdList, Comparator.naturalOrder());
		stdList.forEach(s -> System.out.print(s.getName() + " ")); 
	}
}
```
<a name="owzZo"></a>
### 与 `List.sort` 一起使用
`List.sort`根据给定的比较器实例对这个列表进行排序。<br />ListSortDemo.java
```java
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
public class ListSortDemo {
	public static void main(String[] args) {
		List<Integer> numList = Arrays.asList(12, 10, 15, 8, 11);
		numList.sort(Comparator.naturalOrder());
		numList.forEach(n -> System.out.print(n + " "));
		System.out.println("\n-----------");
		
		List<String> strList = Arrays.asList("Varanasi", "Allahabad", "Kanpur", "Noida");
		strList.sort(Comparator.naturalOrder());
		strList.forEach(s -> System.out.print(s + " "));
		System.out.println("\n-----------"); 
		
		List<Student> stdList = Student.getStudentList();
		stdList.sort(Comparator.naturalOrder());
		stdList.forEach(s -> System.out.print(s.getName() + " "));
	}
}
```
<a name="VYkTo"></a>
### 与 `Arrays.sort` 一起使用
`Arrays.sort`根据指定的比较器引起的顺序对指定的对象阵列进行排序。<br />ArraysSortDemo.java
```java
import java.util.Arrays;
import java.util.Comparator;
public class ArraysSortDemo {
	public static void main(String[] args) {
		Student s1 = new Student("Ram", 18);
		Student s2 = new Student("Shyam", 22);
		Student s3 = new Student("Mohan", 19);
		
		Student[] stdArray = { s1, s2, s3 };
		Arrays.sort(stdArray, Comparator.naturalOrder());
		for (Student s : stdArray) {
			System.out.print(s.getName() + " ");
		}
	}
}
```
