Java<br />在某些特殊的场景下，需要在 Java 程序中对 List 集合进行排序操作。比如从第三方接口中获取所有用户的列表，但列表默认是以用户编号从小到大进行排序的，而系统需要按照用户的年龄从大到小进行排序，这个时候，就需要对 List 集合进行自定义排序操作了。<br />List 排序的常见方法有以下 3 种：

1. 使用 Comparable 进行排序；
2. 使用 Comparator 进行排序；
3. 如果是 JDK 8 以上的环境，也可以使用 Stream 流进行排序。

下面分别来看各种排序方法的具体实现。
<a name="vraS8"></a>
## 1、使用 Comparable 排序
按照本文设计的场景，需要创建一个包含了用户列表的 List 集合，并按用户的年龄从大到小进行排序，具体实现代码如下：
```java
public class ListSortExample {
    public static void main(String[] args) {
        // 创建并初始化 List
        List<Person> list = new ArrayList<Person>() {{
            add(new Person(1, 30, "北京"));
            add(new Person(2, 20, "西安"));
            add(new Person(3, 40, "上海"));
        }};
        // 使用 Comparable 自定的规则进行排序
        Collections.sort(list);
        // 打印 list 集合
        list.forEach(p -> {
            System.out.println(p);
        });
    }
}

//  以下 set/get/toString 使用的是 lombok 的注解
@Getter
@Setter
@ToString
class Person implements Comparable<Person> {
    private int id;
    private int age;
    private String name;

    public Person(int id, int age, String name) {
        this.id = id;
        this.age = age;
        this.name = name;
    }

    @Override
    public int compareTo(Person p) {
        return p.getAge() - this.getAge();
    }
}
```
以上代码的执行结果，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987509-d4606e45-11fb-4102-b386-ed8f46a81545.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=ud5f297aa&originHeight=310&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u67081095-0903-4ea2-a152-629646b5b67&title=)本方法的核心代码如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987504-b31da5b4-9137-46ab-96ab-d5e94565f584.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=u0bd1b790&originHeight=654&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua85032f2-c0d7-4231-9e1e-22e2b4bafd7&title=)
<a name="j7eaI"></a>
## 2、使用 `Comparator` 排序
`Comparable` 是类内部的比较方法，而 `Comparator` 是排序类外部的比较器。使用 `Comparator` 比较器，无需修改原 Person 类，只需要扩充一个 Person 类的比较器就行了，`Comparator` 的实现方法有以下两种：

- 新建 `Comparator` 比较器；
- 使用 `Comparator` 匿名类比较器。

其中，第二种实现方法要更简洁一些，通过下面的具体代码，来观察一下二者的区别。
<a name="BVdWt"></a>
### 2.1 新建 Comparator 比较器
```java
public class ListSortExample2 {
    public static void main(String[] args) {
        // 创建并初始化 List
        List<Person> list = new ArrayList<Person>() {{
            add(new Person(1, 30, "北京"));
            add(new Person(2, 20, "西安"));
            add(new Person(3, 40, "上海"));
        }};
        // 使用 Comparator 比较器排序
        Collections.sort(list, new PersonComparator());
        // 打印 list 集合
        list.forEach(p -> {
            System.out.println(p);
        });
    }
}
/**
 * 新建 Person 比较器
 */
class PersonComparator implements Comparator<Person> {
    @Override
    public int compare(Person p1, Person p2) {
        return p2.getAge() - p1.getAge();
    }
}
@Getter
@Setter
@ToString
class Person {
    private int id;
    private int age;
    private String name;

    public Person(int id, int age, String name) {
        this.id = id;
        this.age = age;
        this.name = name;
    }
}
```
以上代码的执行结果，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987471-0c5f8d91-a112-4682-b0fb-6f5be9894f22.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=ue063ccc4&originHeight=297&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud8b02645-a52c-4b57-81e5-0a008120a16&title=)本方法的核心实现代码如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987508-7b442f2b-bdf4-4a2b-ab54-5b2de12aee96.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=ue25b125a&originHeight=975&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u30ae1344-ef0a-4990-8552-c96e65851e4&title=)
<a name="fC3iD"></a>
### 2.2 匿名类比较器
比较器 Comparator 可以使用更简洁的匿名类的方式，来实现排序功能，具体实现代码如下：
```java
public class ListSortExample2 {
    public static void main(String[] args) {
        // 创建并初始化 List
        List<Person> list = new ArrayList<Person>() {{
            add(new Person(1, 30, "北京"));
            add(new Person(2, 20, "西安"));
            add(new Person(3, 40, "上海"));
        }};
        // 使用匿名比较器排序
        Collections.sort(list, new Comparator<Person>() {
            @Override
            public int compare(Person p1, Person p2) {
                return p2.getAge() - p1.getAge();
            }
        });
        // 打印 list 集合
        list.forEach(p -> {
            System.out.println(p);
        });
    }
}
@Getter
@Setter
@ToString
class Person {
    private int id;
    private int age;
    private String name;
    public Person(int id, int age, String name) {
        this.id = id;
        this.age = age;
        this.name = name;
    }
}
```
以上代码的执行结果，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987476-458b10aa-5a9e-496c-996f-5e99f9409e1f.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=u4ec6fa3a&originHeight=297&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ef8608e-5356-49a2-97e3-8a1efe7bd82&title=)
<a name="JFxPl"></a>
## 3、使用 Stream 流排序
在 JDK 8 之后可以使用更加简单的方法 Stream 流来实现排序功能，它的实现只需要一行代码，具体实现如下：
```java
public class ListSortExample3 {
	public static void main(String[] args) {
		// 创建并初始化 List
		List<Person> list = new ArrayList<Person>() {{
			add(new Person(1, 30, "北京"));
			add(new Person(2, 20, "西安"));
			add(new Person(3, 40, "上海"));
		}};
		// 使用 Stream 排序
		list = list.stream().sorted(Comparator.comparing(Person::getAge).reversed())
			.collect(Collectors.toList());
		// 打印 list 集合
		list.forEach(p -> {
			System.out.println(p);
		});
	}
	@Getter
	@Setter
	@ToString
	static class Person {
		private int id;
		private int age;
		private String name;
		public Person(int id, int age, String name) {
			this.id = id;
			this.age = age;
			this.name = name;
		}
	}
}
```
其中 `reversed()` 表示倒序的意思，如果不使用此方法则是正序。<br />以上代码的执行结果，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987864-0da177f9-52df-4dbf-99c1-f74e200f49cb.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=ubfbdd5e9&originHeight=324&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub3f5b80e-d11d-4de8-89ff-da802093414&title=)
<a name="EK2PQ"></a>
### 扩展：排序字段为 null
使用 Stream 进行排序时，如果排序的字段出现 null 值就会导致异常发生，具体示例如下：
```java
public class ListSortExample4 {
    public static void main(String[] args) {
        // 创建并初始化 List
        List<Person> list = new ArrayList<Person>() {{
            add(new Person(30, "北京"));
            add(new Person(10, "西安"));
            add(new Person(40, "上海"));
            add(new Person(null, "上海")); // 年龄为 null 值
        }};
        // 按照[年龄]正序,但年龄中有一个 null 值
        list = list.stream().sorted(Comparator.comparing(Person::getAge))
                .collect(Collectors.toList());
        // 打印 list 集合
        list.forEach(p -> {
            System.out.println(p);
        });
    }
}
@Getter
@Setter
@ToString
class Person {
    private Integer age;
    private String name;

    public Person(Integer age, String name) {
        this.age = age;
        this.name = name;
    }
}
```
以上代码的执行结果，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987867-65c425d9-4855-4911-98d8-22aefc97ec66.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=ua16a9eb6&originHeight=307&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3111029a-5e0e-45e1-a3ca-c3dfd5b2cd3&title=)想要解决上述问题，需要给 Comparator.comparing 传递第二个参数：Comparator.nullsXXX，如下代码所示：
```java
public class ListSortExample4 {
    public static void main(String[] args) {
        // 创建并初始化 List
        List<Person> list = new ArrayList<Person>() {{
            add(new Person(30, "北京"));
            add(new Person(10, "西安"));
            add(new Person(40, "上海"));
            add(new Person(null, "上海"));
        }};
        // 按照[年龄]正序,但年龄中有一个 null 值
        list = list.stream().sorted(Comparator.comparing(Person::getAge,
                Comparator.nullsFirst(Integer::compareTo)))
                .collect(Collectors.toList());
        // 打印 list 集合
        list.forEach(p -> {
            System.out.println(p);
        });
    }
}
@Getter
@Setter
@ToString
class Person {
    private Integer age;
    private String name;

    public Person(Integer age, String name) {
        this.age = age;
        this.name = name;
    }
}
```
`Comparator.nullsFirst` 表示将排序字段中的 null 值放到集合最前面，如果想要将 null 值放到集合最后面可以使用 `Comparator.nullsLast`。<br />以上代码的执行结果，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651919987913-67ed1a1c-02a0-405f-b531-58c920539758.png#clientId=u6f5cb4d2-ccae-4&from=paste&id=uf7231697&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0e4a7d40-9548-4627-b730-8c13f52e274&title=)
<a name="P8lgA"></a>
## 总结
这里介绍了 3 种 List 排序的方法，前两种方法常用于 JDK 8 之前的版本，其中比较器 Comparator 有两种实现的写法，而在 JDK 8 之后的版本，就可以使用 `Comparator.comparing` 实现排序了，如果排序字段中可能出现 null 值，要使用 `Comparator.nullsXXX` 进行排序处理（否则会报错）。
