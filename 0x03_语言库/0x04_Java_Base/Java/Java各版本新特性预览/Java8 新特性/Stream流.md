Java Stream 
<a name="rmP6S"></a>
## 一、Stream流的介绍
Stream是Java8中处理集合的关键抽象概念，它可以指定对集合进行的操作，可以执行非常复杂的查找、过滤和映射数据等操作。使用Stream API对集合数据进行操作，就类似于使用SQL执行的数据库查询，也可以使用Stream API来并行执行操作。简而言之，Stream API提供了一种高效且易于使用的处理数据的方式。<br />Stream 使用一种类似用 SQL 语句从数据库查询数据的直观方式来提供一种对 Java 集合运算和表达的高阶抽象。<br />流（Stream）到底是什么呢？<br />流是数据渠道，用于操作数据源（集合、数组等）所生成的元素序列。“集合讲的是数据，流讲的是就算！”<br />注意：<br />①Stream不会存储元素。<br />②Stream不会改变源对象，相反，他们会返回一个持有结果的新Stream。<br />③Stream操作是延迟执行的，这意味着他们会等到需要结果的时候才执行。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1582190227845-7bc09c7b-7554-40c8-80cd-9c14f244ff93.png#averageHue=%23a1d256&height=213&id=ZQVbj&originHeight=213&originWidth=1159&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27179&status=done&style=shadow&title=&width=1159)
```sql
+--------------------+       +------+   +------+   +---+   +-------+
| stream of elements +-----> |filter+-> |sorted+-> |map+-> |collect|
+--------------------+       +------+   +------+   +---+   +-------+
```
<a name="zQNxB"></a>
## 二、Stream操作的三个步骤
<a name="Uv7R9"></a>
### 创建Stream
一个数据源（如：集合、数组），获取一个流<br />Java8中的Collection接口被扩展，提供了两个获取流的方法

- `stream()`：为集合创建串行流
- `parallelStream()`：为集合创建并行流
```java
// 返回一个顺序流
default Stream stream()
// 返回一个并行流
default Stream<E> parallelStream()
```
当然，流的来源可以是集合，数组，I/O channel， 产生器generator 等！
<a name="A3Atb"></a>
#### 创建Stream的四种方式
```java
// 1.可以通过Collection 系列集合提供的stream() 或 paralleStream()
List<String> list = new ArrayList<>();
Stream<String> stream = list.stream();

// 2.通过Arrays中的静态方法stream()获取数组流
Employee[] employees = new Employee[10];
Stream<Employee> employeeStream = Arrays.stream(employees);

// 3.通过Stream类中的静态方法of()
Stream<String> stringStream = Stream.of("aa", "bb", "cc");

// 4.创建无限流
// 迭代
Stream.iterate(0, (x) -> x + 2)
    .limit(100)
    .forEach(System.out::println);

// 生成
Stream.generate(() -> Math.random())
    .limit(5)
    .forEach(System.out::println);
```
<a name="toQTe"></a>
### 中间操作
一个中间操作链，对数据源的数据进行处理<br />多个中间操作可以连接起来形成一个流水线，除非流水线上触发终止操作，否则中间操作不会执行任何的处理！而在终止操作时一次性全部处理，称为“惰性求值”
<a name="Ik0qp"></a>
#### 1、筛选与切片
| **方法** | **描述** |
| --- | --- |
| `filter(Predicate p)` | 接收Lambda，从流中排除某些元素 |
| `distinct()` | 筛选，通过流所生成元素的`hashCode()`和`equals()`方法去除重复元素 |
| `limit(long maxSize)` | 截取流，使其元素不超过给定的数量 |
| `skip(long n)` | 跳过元素，返回一个扔掉了前n个元素的流，若流中元素不足n个，则返回一个空流。与`limit(n)`互补 |

```java
List<Employee> employees = Arrays.asList(
    new Employee("Fcant", 14, 99998.0),
    new Employee("Fcary", 10, 998.045),
    new Employee("Fcloi", 15, 934598.0),
    new Employee("Fcmop", 19, 56998.04),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcqyt", 17, 998.0645)
);

// 中间操作

/**
 * 筛选与切片
 * filter-接收Lambda，从流中排除某些元素
 * limit-截断流，使其元素不超过给定数量
 * skip-跳过元素，返回一个扔掉了前n个元素的流，若流中元素不足n个，则返回一个空流。与limit(n)互补
 * distinct-筛选，通过流所生成元素的hashCode()和equals()去除重复元素
 */
@Test
public void streamSelectOpTest() {
    System.out.println("--------------filter-----------");
    employees.stream()
        .filter(employee -> {
            System.out.println("Steam API 中间操作");
            return employee.getAge() > 15;
        })
        .forEach(System.out::println);

    System.out.println("--------------limit-----------");
    employees.stream()
        .filter(employee -> {
            System.out.println("短路！-使用limit找到符合条件的数据后面的不再迭代遍历");
            return employee.getSalary() > 5000;
        })
        .limit(2)
        .forEach(System.out::println);

    System.out.println("--------------skip-----------");
    employees.stream()
        .filter(employee -> {
            System.out.println("迭代遍历所有");
            return employee.getSalary() > 5000;
        })
        .skip(2)
        .forEach(System.out::println);

    // 使用distinct去重时需要重写hashCode()方法和equals()方法
    System.out.println("--------------distinct-----------");
    employees.stream()
        .filter(employee -> {
            System.out.println("迭代遍历所有");
            return employee.getSalary() > 5000;
        })
        .skip(1)
        .distinct()
        .forEach(System.out::println);
}
```
<a name="x6Ii5"></a>
#### 2、映射
| **方法** | **描述** |
| --- | --- |
| `map(Function f)` | 接收一个函数作为参数，该函数会被应用到每个元素上，并将其映射成一个新的元素 |
| `mapToDouble(ToDoubleFunction f)` | 接收一个函数作为参数，该函数会被应用到每个元素上，产生一个新的DoubleStream |
| `mapToInt(ToIntFunction f)` | 接收一个函数作为参数，该函数会被应用到每个元素上，产生一个新的IntStream |
| `mapToLong(ToLongFunction f)` | 接收一个函数作为参数，该函数会被应用到每个元素上，产生一个新的LongStream |
| `flatMap(Function f)` | 接收一个函数作为参数，将流中的每个值都换成另一个流，然后把所有流连接成一个流 |

```java
package com.fcant.java8.stream;

import com.fcant.java8.lambda.bean.Employee;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Stream;

/**
 * StreamTest
 * <p>
 * encoding:UTF-8
 *
 * 一、Stream的三个操作步骤
 * 1.创建Stream
 * 2.中间操作
 * 3.终止操作（终端操作）
 *
 * @author Fcant 下午 17:18:28 2020/2/20/0020
 */
public class StreamTest  {

    List<Employee> employees = Arrays.asList(
            new Employee("Fcant", 14, 99998.0),
            new Employee("Fcary", 10, 998.045),
            new Employee("Fcloi", 15, 934598.0),
            new Employee("Fcmop", 19, 56998.04),
            new Employee("Fcctr", 18, 945698.0),
            new Employee("Fcctr", 18, 945698.0),
            new Employee("Fcctr", 18, 945698.0),
            new Employee("Fcqyt", 17, 998.0645)
    );

    // 中间操作

    /**
     * 映射
     * map-接收Lambda，将元素转换成其他形式或提取信息，接收一个函数作为参数，该函数会被应用到每个元素上，并将其映射成一个新的元素
     * flatMap-接收一个函数作为参数，将流中的每个值都换成另一个流，然后把所有流连接成一个流
     */
    @Test
    public void refTest() {
        List<String> list = Arrays.asList("aaa", "bbb", "ccc", "ddd", "eee");
        list.stream()
                .map(x -> x.toUpperCase())
                .forEach(System.out::println);
        System.out.println("-------------员工姓名提取------------");
        employees.stream()
                .map(x -> x.getName())
                .forEach(System.out::println);

        System.out.println("-------------通过map将流整合成一个流------------");
        Stream<Stream<Character>> streamStream = list.stream()
                .map(StreamTest::filterCharacter);
        streamStream.forEach(sm -> sm.forEach(System.out::println));

        System.out.println("----------通过flatMap进行流的整合-----------");
        list.stream()
                .flatMap(StreamTest::filterCharacter)
                .forEach(System.out::println);
    }

    public static Stream<Character> filterCharacter(String s) {
        List<Character> characterList = new ArrayList<>();
        for (Character character : s.toCharArray()) {
            characterList.add(character);
        }
        return characterList.stream();
    }

}

```
<a name="p5dmr"></a>
#### 3、排序
| **方法** | **描述** |
| --- | --- |
| `sorted()` | 产生一个新流，其中按自然顺序排序 |
| `sorted(Comparator comp)` | 产生一个新流，其中按比较器顺序排序 |

```java
List<Employee> employees = Arrays.asList(
    new Employee("Fcant", 14, 99998.0),
    new Employee("Fcary", 10, 998.045),
    new Employee("Fcloi", 15, 934598.0),
    new Employee("Fcmop", 19, 56998.04),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcqyt", 17, 998.0645)
);

// 中间操作

/**
 * 排序
 * sorted()-自然排序(Comparable)
 * sorted(Comparator com)-定制排序(Comparator)
 */
@Test
public void sortStreamTest() {
    System.out.println("-----------sort()自然排序------------");
    List<String> list = Arrays.asList("ccc", "ddd", "aaa", "bbb", "eee");
    list.stream()
        .sorted()
        .forEach(System.out::println);
    System.out.println("-----------sort()定制排序------------");
    employees.stream()
        .sorted((e1, e2) -> {
            if (e1.getAge().equals(e2.getAge())) {
                return e1.getName().compareTo(e2.getName());
            } else {
                return e1.getAge().compareTo(e2.getAge());
            }
        }).forEach(System.out::println);
}
```
<a name="eA3uF"></a>
### 终止操作（终端操作）
一个操作终止，执行中间操作链，并产生结果，终端操作会从流的流水线生成结果。其结果可以是任何不是流的值，例如：List、Integer甚至void。
<a name="mSDuC"></a>
#### 1、查找与匹配
| **方法** | **描述** |
| --- | --- |
| `allMatch(Predicate p)` | 检查是否匹配所有元素 |
| `anyMatch(Predicate p)` | 检查是否至少匹配一个元素 |
| `noneMatch(Predicate p)` | 检查是否没有匹配所有元素 |
| `findFirst()` | 返回第一个元素 |
| `findAny()` | 返回当前流中的任意元素 |
| `count()` | 返回当前流中元素的总个数 |
| `max(Comparator c)` | 返回流中最大值 |
| `min(Comparator c)` | 返回流中最小值 |
| `forEach(Consumer c)` | 内部迭代（使用Collection接口需要用户去做迭代，称为外部迭代。相反，Stream API使用内部迭代--即API内部完成了操作） |

Employee类的升级
```java
package com.fcant.java8.lambda.bean;

import java.util.Objects;

/**
 * Employee
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 21:20:09 2020/2/18/0018
 */
public class Employee {
    private String name;
    private Integer age;
    private Double salary;
    private Status status;

    public Employee(String name) {
        this.name = name;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Employee employee = (Employee) o;
        return age == employee.age &&
                Double.compare(employee.salary, salary) == 0 &&
                Objects.equals(name, employee.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, age, salary);
    }

    public Employee() {
    }

    public Employee(String name, Integer age, Double salary) {
        this.name = name;
        this.age = age;
        this.salary = salary;
    }

    public Employee(String name, Integer age, Double salary, Status status) {
        this.name = name;
        this.age = age;
        this.salary = salary;
        this.status = status;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public Double getSalary() {
        return salary;
    }

    public void setSalary(Double salary) {
        this.salary = salary;
    }

    public Status getStatus() {
        return status;
    }

    public void setStatus(Status status) {
        this.status = status;
    }

    @Override
    public String toString() {
        return "Employee{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", salary=" + salary +
                ", status=" + status +
                '}';
    }

    public enum Status{
        FREE,
        BUSY,
        VOCATION;
    }
}
```
案例
```java
List<Employee> employees = Arrays.asList(
    new Employee("Fcant", 14, 99998.0, Employee.Status.BUSY),
    new Employee("Fcary", 10, 998.045, Employee.Status.VOCATION),
    new Employee("Fcloi", 15, 934598.0, Employee.Status.FREE),
    new Employee("Fcmop", 19, 56998.04, Employee.Status.BUSY),
    new Employee("Fcctr", 18, 945698.0, Employee.Status.BUSY),
    new Employee("Fcctr", 18, 945698.0, Employee.Status.FREE),
    new Employee("Fcctr", 18, 945698.0, Employee.Status.VOCATION),
    new Employee("Fcqyt", 17, 998.0645, Employee.Status.FREE)
);

// 终止操作
/**
     * 查找与匹配
     * allMatch-检查是否匹配所有元素
     * anyMatch-检查是否至少匹配一个元素
     * noneMatch-检查是否没有匹配所有元素
     * findFirst-返回第一个元素
     * findAny-返回当前流中任意元素
     * count-返回流中元素的总个数
     * max-返回流中最大值
     * min-返回流中最小值
     */

@Test
public void streamFindTest() {
    boolean allMatch = employees.stream()
        .allMatch(e -> e.getStatus().equals(Employee.Status.BUSY));
    System.out.println(allMatch);

    boolean anyMatch = employees.stream()
        .anyMatch(e -> e.getStatus().equals(Employee.Status.BUSY));
    System.out.println(anyMatch);

    boolean noneMatch = employees.stream()
        .noneMatch(e -> e.getStatus().equals(Employee.Status.BUSY));
    System.out.println(noneMatch);

    Optional<Employee> firstEmployee = employees.stream()
        .sorted((e1, e2) -> -Double.compare(e1.getSalary(), e2.getSalary()))
        .findFirst();
    System.out.println(firstEmployee.get());

    Optional<Employee> optionalEmployee = employees.stream()
        .filter(e -> e.getStatus().equals(Employee.Status.FREE))
        .findAny();
    System.out.println(optionalEmployee.get());

    long count = employees.stream()
        .count();
    System.out.println(count);

    Optional<Employee> maxEmpSalary = employees.stream()
        .max((e1, e2) -> Double.compare(e1.getSalary(), e2.getSalary()));
    System.out.println(maxEmpSalary.get());

    Optional<Double> min = employees.stream()
        .map(Employee::getSalary)
        .min(Double::compare);
    System.out.println(min.get());
}
```
<a name="dgHx1"></a>
#### 2、规约
| **方法** | **描述** |
| --- | --- |
| `reduce(T iden, BinaryOperator b)` | 可以将流中元素反复结合起来，得到一个值。返回T |
| `reduce(BinaryOperator b)` | 可以将流中元素反复结合起来，得到一个值。返回`Optional<T>` |

备注：`Map`和`Reduce`的连接通常称为map-reduce模式，因Google用它来进行网络搜索而出名。
```java
/**
 * 规约
 * reduce(T identity, BinaryOperator)/reduce(BinaryOperator)-可以将流中元素反复结合起来，得到一个值
 */

@Test
public void streamReduceTest() {
    List<Integer> list = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    Integer sum = list.stream()
        .reduce(0, (x, y) -> x + y);
    System.out.println("该数组之和为：" + sum);

    System.out.println("----------------------");

    Optional<Double> optionalDouble = employees.stream()
        .map(Employee::getSalary)
        .reduce(Double::sum);
    System.out.println("员工的工资总和为：" + optionalDouble.get());
} 
```
<a name="BKGw3"></a>
#### 3、收集
| **方法** | **描述** |
| --- | --- |
| `Collection(Collector c)` | 将流转换为其他形式，接收一个Collector接口的实现，用于给Stream中元素做汇总的方法 |

Collector接口中方法的实现决定了如何对流执行收集操作（如收集到List、Set、Map）。但是`Collectors`实用类提供了很多静态方法，可以方便地创建常见收集器实例。

| 工厂方法 | 返回类型 | 作用 |
| --- | --- | --- |
| `toList` | `List<T>` | 把流中所有项目收集到一个 List |
| `toSet` | `Set<T>` | 把流中所有项目收集到一个 Set，删除重复项 |
| `toCollection` | `Collection<T>` | 把流中所有项目收集到给定的供应源创建的集合<br />`menuStream.collect(toCollection(), ArrayList::new)` |
| `counting` | Long | 计算流中元素的个数 |
| `sumInt` | Integer | 对流中项目的一个整数属性求和 |
| `averagingInt` | Double | 计算流中项目 Integer 属性的平均值 |
| `summarizingInt` | IntSummaryStatistics | 收集关于流中项目 Integer 属性的统计值，例如最大、最小、 总和与平均值 |
| `joining` | String | 连接对流中每个项目调用 toString 方法所生成的字符串<br />`collect(joining(", "))` |
| `maxBy` | `Optional<T>` | 一个包裹了流中按照给定比较器选出的最大元素的 Optional， 或如果流为空则为 `Optional.empty()` |
| `minBy` | `Optional<T>` | 一个包裹了流中按照给定比较器选出的最小元素的 Optional， 或如果流为空则为 `Optional.empty()` |
| `reducing` | 归约操作产生的类型 | 从一个作为累加器的初始值开始，利用 BinaryOperator 与流 中的元素逐个结合，从而将流归约为单个值<br />`累加int totalCalories = menuStream.collect(reducing(0, Dish::getCalories, Integer::sum));` |
| `collectingAndThen` | 转换函数返回的类型 | 包裹另一个收集器，对其结果应用转换函数<br />`int howManyDishes = menuStream.collect(collectingAndThen(toList(), List::size))` |
| `groupingBy` | `Map<K, List<T>>` | 根据项目的一个属性的值对流中的项目作问组，并将属性值作 为结果 Map 的键 |
| `partitioningBy` | `Map<Boolean,List<T>>` | 根据对流中每个项目应用谓词的结果来对项目进行分区 |

```java
List<Employee> employees = Arrays.asList(
    new Employee("Fcant", 14, 99998.0, Employee.Status.BUSY),
    new Employee("Fcary", 10, 998.045, Employee.Status.VOCATION),
    new Employee("Fcloi", 15, 934598.0, Employee.Status.FREE),
    new Employee("Fcmop", 19, 56998.04, Employee.Status.BUSY),
    new Employee("Fcctr", 18, 945698.0, Employee.Status.BUSY),
    new Employee("Fcctr", 18, 945698.0, Employee.Status.FREE),
    new Employee("Fcctr", 18, 945698.0, Employee.Status.VOCATION),
    new Employee("Fcqyt", 17, 998.0645, Employee.Status.FREE)
);

// 终止操作

/**
 * 收集
 * collect-将流转换为其他形式，接收一个Collector接口的实现，用于给Stream中元素做汇总的方法
 */

@Test
public void streamCollectTest() {
    employees.stream()
        .map(Employee::getName)
        .collect(Collectors.toList())
        .forEach(System.out::println);

    System.out.println("----------------------");
    employees.stream()
        .map(Employee::getName)
        .collect(Collectors.toSet())
        .forEach(System.out::println);

    System.out.println("-----------------------");
    employees.stream()
        .map(Employee::getName)
        .collect(Collectors.toCollection(HashSet::new))
        .forEach(System.out::println);

    // 总数
    Long count = employees.stream()
        .collect(Collectors.counting());
    System.out.println(count);

    // 平均值
    Double avg = employees.stream()
        .collect(Collectors.averagingDouble(Employee::getSalary));
    System.out.println(avg);

    // 总和
    Double sum = employees.stream()
        .collect(Collectors.summingDouble(Employee::getSalary));
    System.out.println(sum);

    // 最大值
    Optional<Employee> optionalEmployee = employees.stream()
        .collect(Collectors.maxBy((e1, e2) -> Double.compare(e1.getSalary(), e2.getSalary())));
    System.out.println(optionalEmployee.get());

    // 最小值
    Optional<Double> min = employees.stream()
        .map(Employee::getSalary)
        .collect(Collectors.minBy(Double::compare));
    System.out.println(min.get());

    // 分组
    Map<Employee.Status, List<Employee>> group = employees.stream()
        .collect(Collectors.groupingBy(Employee::getStatus));
    Employee.Status[] values = Employee.Status.values();
    for (int i = 0; i < values.length; i++) {
        System.out.println(values[i]);
        group.get(values[i]).forEach(System.out::println);
    }

    // 多级分组
    System.out.println("-----------多级分组----------");
    Map<Employee.Status, Map<String, List<Employee>>> emp = employees.stream()
        .collect(Collectors.groupingBy(Employee::getStatus, Collectors.groupingBy(e -> {
            if (((Employee) e).getAge() <= 15) {
                return "初中生";
            } else if (((Employee) e).getAge() <= 17) {
                return "高中生";
            } else {
                return "大学生";
            }
        })));
    System.out.println(emp);
    for (int i = 0; i < values.length; i++) {
        System.out.println(values[i]);
        group.get(values[i]).forEach(System.out::println);
    }

    // 分片、分区
    Map<Boolean, List<Employee>> collect = employees.stream()
        .collect(Collectors.partitioningBy(e -> e.getSalary() > 5000));
    System.out.println(collect);

    // 分组函数的另外一种获取使用
    DoubleSummaryStatistics empSalary = employees.stream()
        .collect(Collectors.summarizingDouble(Employee::getSalary));
    System.out.println("员工薪资总和为：" + empSalary.getSum());
    System.out.println("员工薪资有 " + empSalary.getCount() + " 条");
    System.out.println("员工薪资均值为：" + empSalary.getAverage());
    System.out.println("员工薪资最大值为：" + empSalary.getMax());
    System.out.println("员工薪资最小值为：" + empSalary.getMin());

    String empName = employees.stream()
        .map(Employee::getName)
        .collect(Collectors.joining("," , "==>", "<=="));
    System.out.println(empName);
}
```
<a name="DNKap"></a>
## 三、并行流与顺序流
<a name="ryfpr"></a>
### 并行流与顺序流
并行流就是把一个内容分成多个数据块，并用不同的线程分别处理每个数据块的流。<br />Java8中将并行进行了优化，可以声明性地通过`parallel()`与`sequential()`在并行流与顺序流之间进行切换。
<a name="scOmk"></a>
#### Fork/Join框架
Fork/Join框架：就是在必要情况下，将一个大任务，进行拆分（fork）成若干个小任务（拆到不可再拆时），再将一个个的小任务运算的结果进行join汇总。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1582377352439-3cec6eab-2ab8-45bf-901b-982cfc2acf53.png#averageHue=%23faf9e3&height=621&id=BsbIB&originHeight=621&originWidth=880&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50415&status=done&style=shadow&title=&width=880)
<a name="NqWop"></a>
#### Fork/Join框架与传统线程池的区别
采用“工作窃取”模式（work-stealing）：<br />当执行新的任务时它可以将其拆分分成更小的任务执行，并将小任务加到线程队列中，然后再从一个随机线程的队列中偷一个并把它放在自己的队列中。<br />相对于一般的线程池实现，fork/join框架的优势体现在对其中包含的任务的处理方式上，在一般的线程池中，如果一个线程正在执行的任务由于某些原因无法继续运行，那么该线程会处于等待状态，而在fork/join框架实现中，如果某个子问题由于等待另外一个子问题的完成而无法继续运行，那么处理该子问题的线程会主动寻找其他尚未运行的子问题来执行，这种方式减少了线程的等待时间，提高了性能。
```java
public class ForkJoinCalculate extends RecursiveTask<Long> {

    public static final long serialVersionUID = 134656970987L;

    private Long start;
    private Long end;

    public ForkJoinCalculate(Long start, Long end) {
        this.start = start;
        this.end = end;
    }

    public static final long THRESHOLD = 10000;

    @Override
    protected Long compute() {
        long length = end - start;
        if (length <= THRESHOLD) {
            long sum = 0;
            for (long i = start; i <= end; i++) {
                sum += i;
            }
            return sum;
        } else {
            long middle = (start + end) / 2;
            ForkJoinCalculate left = new ForkJoinCalculate(start, middle);
            // 拆分子任务，同时压入线程队列
            left.fork();

            ForkJoinCalculate right = new ForkJoinCalculate(middle + 1, end);
            right.fork();

            return left.join() + right.join();
        }
    }
}
```
```java
public class ForkJoinTest {

    // Fork/Join操作
    @Test
    public void forkJoinTest() {
        Instant start = Instant.now();
        ForkJoinPool pool = new ForkJoinPool();
        ForkJoinCalculate task = new ForkJoinCalculate(0L, 10000000L);
        Long sum = pool.invoke(task);
        System.out.println(sum);
        Instant end = Instant.now();
        System.out.println("耗费时间为：" + Duration.between(start, end).toMillis() + "毫秒");
    }

    // 常规行程操作
    @Test
    public void threadTest() {
        Instant start = Instant.now();
        Long sum = 0L;
        for(long i=0 ;i < 10000000L;i++){
            sum += i;
        }
        System.out.println(sum);
        Instant end = Instant.now();
        System.out.println("耗费时间为：" + Duration.between(start, end).toMillis() + "毫秒");
    }

    // Java8并行流的操作
    @Test
    public void parallelTest() {
        Instant start = Instant.now();
        LongStream.rangeClosed(0, 100000000L)
                .parallel()
                .reduce(0, Long::sum);
        Instant end = Instant.now();
        System.out.println("耗时为：" + Duration.between(start, end).toMillis() + "毫秒");
    }
}
```
<a name="fOJJZ"></a>
## 四、Stream操作小练习
<a name="R4eIP"></a>
### 单个案例
```java
import com.fcant.java8.lambda.bean.Employee;
import org.junit.Test;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

/**
 * StreamPractice
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 17:33:04 2020/2/22/0022
 */
public class StreamPractice {

    /**
     * 给定一个数字列表，如何返回一个由每个数的平方构成的列表，给定[1, 2, 3, 4, 5]
     */
    @Test
    public void areaTest() {
        Integer[] nums = new Integer[]{1, 2, 3, 4, 5};
        Arrays.stream(nums)
                .map(x -> x*x)
                .forEach(System.out::println);
    }

    List<Employee> employees = Arrays.asList(
            new Employee("Fcant", 14, 99998.0, Employee.Status.BUSY),
            new Employee("Fcary", 10, 998.045, Employee.Status.VOCATION),
            new Employee("Fcloi", 15, 934598.0, Employee.Status.FREE),
            new Employee("Fcmop", 19, 56998.04, Employee.Status.BUSY),
            new Employee("Fcctr", 18, 945698.0, Employee.Status.BUSY),
            new Employee("Fcctr", 18, 945698.0, Employee.Status.FREE),
            new Employee("Fcctr", 18, 945698.0, Employee.Status.VOCATION),
            new Employee("Fcqyt", 17, 998.0645, Employee.Status.FREE)
    );

    /**
     * 怎样通过Map或Reduce计算流中有多少Employee
     */
    @Test
    public void countEmployeeTest() {
        Optional<Integer> optionalInteger = employees.stream()
                .map(employee -> 1)
                .reduce(Integer::sum);
        System.out.println(optionalInteger.get());
    }
}
```
<a name="oy4Zv"></a>
### 关联案例
<a name="JYtAi"></a>
#### Trader.java
```java
public class Trader {

    private String name;
    private String city;

    public Trader() {
    }

    public Trader(String name, String city) {
        this.name = name;
        this.city = city;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    @Override
    public String toString() {
        return "Trader{" +
                "name='" + name + '\'' +
                ", city='" + city + '\'' +
                '}';
    }
}
```
<a name="TFWTH"></a>
#### Transaction.java
```java
public class Transaction {

    private Trader trader;
    private Integer year;
    private Integer value;

    public Trader getTrader() {
        return trader;
    }

    public void setTrader(Trader trader) {
        this.trader = trader;
    }

    public Integer getYear() {
        return year;
    }

    public void setYear(Integer year) {
        this.year = year;
    }

    public Integer getValue() {
        return value;
    }

    public void setValue(Integer value) {
        this.value = value;
    }

    public Transaction() {
    }

    public Transaction(Trader trader, Integer year, Integer value) {
        this.trader = trader;
        this.year = year;
        this.value = value;
    }

    @Override
    public String toString() {
        return "Transaction{" +
                "trader=" + trader +
                ", year=" + year +
                ", value=" + value +
                '}';
    }
}
```
<a name="QTzvZ"></a>
#### 测试案例
```java
import com.fcant.java8.lambda.bean.Trader;
import com.fcant.java8.lambda.bean.Transaction;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

/**
 * TransTest
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 17:53:18 2020/2/22/0022
 */
public class TransTest {

    List<Transaction> transactions = null;

    @Before
    public void before() {
        Trader roal = new Trader("Roal", "Cambridge");
        Trader mario = new Trader("Mario", "Milan");
        Trader alan = new Trader("Alan", "Cambridge");
        Trader brian = new Trader("Brian", "Cambridge");

        transactions = Arrays.asList(
                new Transaction(brian, 2019, 300),
                new Transaction(roal, 2013, 1000),
                new Transaction(roal, 2015, 400),
                new Transaction(mario, 2013, 710),
                new Transaction(mario, 2013, 700),
                new Transaction(alan, 2014, 950)
        );
    }

    // 1.找出2013年发生的所有交易，并按交易额排序（从低到高）
    @Test
    public void sortTest() {
        transactions.stream()
                .filter(t -> t.getYear() == 2013)
                .sorted((t1, t2) -> Integer.compare(t1.getValue(), t2.getValue()))
                .forEach(System.out::println);
    }

    // 2.交易员都在哪些不同的城市工作过
    @Test
    public void cityTest() {
        transactions.stream()
                .map(t -> t.getTrader().getCity())
                .distinct()
                .forEach(System.out::println);
    }

    // 3.查找所有来自剑桥的交易员，并按姓名排序
    @Test
    public void nameSortTest() {
        transactions.stream()
                .filter(t -> t.getTrader().getCity().equals("Cambridge"))
                .map(Transaction::getTrader)
                .distinct()
                .sorted((t1, t2) -> t1.getName().compareTo(t2.getCity()))
                .forEach(System.out::println);
    }

    // 4.返回所有交易员的姓名字符串，按字母排序
    @Test
    public void nameStrTest() {
        transactions.stream()
                .map(Transaction::getTrader)
                .map(Trader::getName)
                .distinct()
                .sorted(String::compareTo)
                .forEach(System.out::println);

        System.out.println("--------------------------");

        transactions.stream()
                .map(transaction -> transaction.getTrader().getName())
                .distinct()
                .sorted()
                .forEach(System.out::println);

        String traderName = transactions.stream()
                .map(transaction -> transaction.getTrader().getName())
                .distinct()
                .sorted()
                .reduce("", String::concat);
        System.out.println(traderName);
    }

    // 5.有没有交易员是在米兰工作的
    @Test
    public void baseTest() {
        boolean baseAddress = transactions.stream()
                .anyMatch(t -> t.getTrader().getCity().equals("Milan"));
        System.out.println(baseAddress);
    }

    // 6.打印生活在剑桥的交易员的所有交易额
    @Test
    public void lifeTest() {
        Optional<Integer> sum = transactions.stream()
                .filter(transaction -> transaction.getTrader().getCity().equals("Cambridge"))
                .map(Transaction::getValue)
                .reduce(Integer::sum);
        System.out.println(sum.get());
    }

    // 7.所有交易中，最高的交易额是多少
    @Test
    public void maxTest() {
        Optional<Integer> max = transactions.stream()
                .map(transaction -> transaction.getValue())
                .max(Integer::compare);
        System.out.println(max.get());
    }

    // 8.找到交易额最小的交易
    @Test
    public void minTest() {
        Optional<Transaction> min = transactions.stream()
                .min((t1, t2) -> Integer.compare(t1.getValue(), t2.getValue()));
        System.out.println(min);
    }

}
```

