Java Stream<br />在Java8中，可以使用lambda表达式实例化比较器(Comparator)。<br />还可以颠倒自然顺序和比较器(Comparator)提供的顺序。<br />自然排序使用Comparable提供的排序，该排序必须由实例为流元素的类实现。<br />在本页中，将使用java 8 Stream `sorted()`方法对列表List, Map和Set进行排序。
<a name="jZ1Nz"></a>
### 1、使用Stream `sorted()`完成自然排序、比较器和反向排序
下面是`sorted()`方法的语法

- `sorted()`：它使用自然顺序对流中的元素进行排序。元素类必须实现Comparable接口。
- `sorted(Comparator<? super T> comparator)`：这里使用lambda表达式创建一个Comparator实例。可以按升序和降序对流元素进行排序。

下面的代码行将按自然顺序对列表进行排序。
```java
list.stream().sorted()
```
要反转自然顺序，Comparator提供`reverseOrder()`方法。
```java
list.stream().sorted(Comparator.reverseOrder())
```
下面的代码行使用Comparator对列表进行排序。
```java
list.stream().sorted(Comparator.comparing(Student::getAge))
```
为了颠倒顺序，Comparator提供`reversed()`方法。
```java
list.stream().sorted(Comparator.comparing(Student::getAge).reversed())
```
<a name="bQZRy"></a>
### 2、在List中使用Stream `sorted()`方法
下面对Student列表进行排序操作。首先，将按自然顺序排序，然后使用比较器(Comparator)。<br />下面是颠倒自然排序和比较器提供的排序的例子。<br />SortList.java
```java
package com.concretepage;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;
public class SortList {
	public static void main(String[] args) {
		List<Student> list = new ArrayList<Student>();
		list.add(new Student(1, "Mahesh", 12));
		list.add(new Student(2, "Suresh", 15));
		list.add(new Student(3, "Nilesh", 10));
		
		System.out.println("---Natural Sorting by Name---");
		List<Student> slist = list.stream().sorted().collect(Collectors.toList());
		slist.forEach(e -> System.out.println("Id:"+ e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));
		
		System.out.println("---Natural Sorting by Name in reverse order---");
		slist = list.stream().sorted(Comparator.reverseOrder()).collect(Collectors.toList());
		slist.forEach(e -> System.out.println("Id:"+ e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));  
		
		System.out.println("---Sorting using Comparator by Age---");
		slist = list.stream().sorted(Comparator.comparing(Student::getAge)).collect(Collectors.toList());
		slist.forEach(e -> System.out.println("Id:"+ e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));
		
		System.out.println("---Sorting using Comparator by Age with reverse order---");
		slist = list.stream().sorted(Comparator.comparing(Student::getAge).reversed()).collect(Collectors.toList());
		slist.forEach(e -> System.out.println("Id:"+ e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));
	}
} 
```
Student.java
```java
package com.concretepage;
public class Student implements Comparable<Student> {
	private int id;
	private String name;
	private int age;
	public Student(int id, String name, int age) {
		this.id = id;
		this.name = name;
		this.age = age;
	}
	public int getId() {
		return id;
	}
	public String getName() {
		return name;
	}
	public int getAge() {
		return age;
	}
	@Override
	public int compareTo(Student ob) {
		return name.compareTo(ob.getName());
	}
	@Override
	public boolean equals(final Object obj) {
		if (obj == null) {
			return false;
		}
		final Student std = (Student) obj;
		if (this == std) {
			return true;
		} else {
			return (this.name.equals(std.name) && (this.age == std.age));
		}
	}
	@Override
	public int hashCode() {
		int hashno = 7;
		hashno = 13 * hashno + (name == null ? 0 : name.hashCode());
		return hashno;
	} 
} 
```
输出
```java
---Natural Sorting by Name---
Id:1, Name: Mahesh, Age:12
Id:3, Name: Nilesh, Age:10
Id:2, Name: Suresh, Age:15
---Natural Sorting by Name in reverse order---
Id:2, Name: Suresh, Age:15
Id:3, Name: Nilesh, Age:10
Id:1, Name: Mahesh, Age:12
---Sorting using Comparator by Age---
Id:3, Name: Nilesh, Age:10
Id:1, Name: Mahesh, Age:12
Id:2, Name: Suresh, Age:15
---Sorting using Comparator by Age with reverse order---
Id:2, Name: Suresh, Age:15
Id:1, Name: Mahesh, Age:12
Id:3, Name: Nilesh, Age:10 
```
<a name="x7MJi"></a>
### 3、在Set中使用Stream `sorted()`方法
下面对Student类的集合(Set)进行排序操作，此类必须重写`equals()`和`hashCode()`方法来标识唯一的元素。对于自然排序，学生类需要实现Comparable接口。<br />在下面的例子中，将使用自然排序和比较器提供的排序对集合进行排序。<br />SortSet.java
```java
package com.concretepage;

import java.util.Comparator;
import java.util.HashSet;
import java.util.Set;
public class SortSet {
	public static void main(String[] args) {
		Set<Student> set = new HashSet<Student>();
		set.add(new Student(1, "Mahesh", 12));
		set.add(new Student(2, "Suresh", 15));
		set.add(new Student(3, "Nilesh", 10));
		
		System.out.println("---Natural Sorting by Name---");
		set.stream().sorted().forEach(e -> System.out.println("Id:"
															  + e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));
		
		System.out.println("---Natural Sorting by Name in reverse order---");
		set.stream().sorted(Comparator.reverseOrder()).forEach(e -> System.out.println("Id:"
																					   + e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));  
		
		System.out.println("---Sorting using Comparator by Age---");
		set.stream().sorted(Comparator.comparing(Student::getAge))
			.forEach(e -> System.out.println("Id:"+ e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));
		
		System.out.println("---Sorting using Comparator by Age in reverse order---");
		set.stream().sorted(Comparator.comparing(Student::getAge).reversed())
			.forEach(e -> System.out.println("Id:"+ e.getId()+", Name: "+e.getName()+", Age:"+e.getAge()));
	}
} 
```
输出
```java
---Natural Sorting by Name---
Id:1, Name: Mahesh, Age:12
Id:3, Name: Nilesh, Age:10
Id:2, Name: Suresh, Age:15
---Natural Sorting by Name in reverse order---
Id:2, Name: Suresh, Age:15
Id:3, Name: Nilesh, Age:10
Id:1, Name: Mahesh, Age:12
---Sorting using Comparator by Age---
Id:3, Name: Nilesh, Age:10
Id:1, Name: Mahesh, Age:12
Id:2, Name: Suresh, Age:15
---Sorting using Comparator by Age in reverse order---
Id:2, Name: Suresh, Age:15
Id:1, Name: Mahesh, Age:12
Id:3, Name: Nilesh, Age:10 
```
<a name="iiBr3"></a>
### 4、在Map中使用Stream `sorted()`方法
这里将按键和值对Map进行排序。<br />SortMap.java
```java
package com.concretepage;

import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
public class SortMap {
	public static void main(String[] args) {
		Map<Integer, String> map = new HashMap<>();
		map.put(15, "Mahesh");
		map.put(10, "Suresh");
		map.put(30, "Nilesh");
		
		System.out.println("---Sort by Map Value---");
		map.entrySet().stream().sorted(Comparator.comparing(Map.Entry::getValue))
			.forEach(e -> System.out.println("Key: "+ e.getKey() +", Value: "+ e.getValue()));
		
		System.out.println("---Sort by Map Key---");
		map.entrySet().stream().sorted(Comparator.comparing(Map.Entry::getKey))
			.forEach(e -> System.out.println("Key: "+ e.getKey() +", Value: "+ e.getValue()));
	}
} 
```
输出
```java
---Sort by Map Value---
Key: 15, Value: Mahesh
Key: 30, Value: Nilesh
Key: 10, Value: Suresh
---Sort by Map Key---
Key: 10, Value: Suresh
Key: 15, Value: Mahesh
Key: 30, Value: Nilesh 
```
下面要对值为自定义对象的Map进行排序。<br />SortMapOfCustomObject.java
```java
package com.concretepage;

import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
public class SortMapOfCustomObject {
	public static void main(String[] args) {
		Map<Integer, Student> map = new HashMap<>();
		map.put(1, new Student(1, "Mahesh", 12));
		map.put(2, new Student(2, "Suresh", 15));
		map.put(3, new Student(3, "Nilesh", 10));
		//Map Sorting by Value i.e student's natural ordering i.e by name
		map.entrySet().stream().sorted(Comparator.comparing(Map.Entry::getValue))
			.forEach(e -> {
				Integer key = (Integer)e.getKey();
				Student std = (Student)e.getValue();
				System.out.println("Key: " + key +", value: ("+ std.getId() +", "+ std.getName()+", "+ std.getAge()+")"); 
			});
	}
} 
```
输出
```java
Key: 1, value: (1, Mahesh, 12)
Key: 3, value: (3, Nilesh, 10)
Key: 2, value: (2, Suresh, 15) 
```
