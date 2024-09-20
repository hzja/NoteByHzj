Java Lambda
<a name="lFRRd"></a>
# 1、Lambda表达式简介
Lambda 表达式，也称为闭包，是 Java 8 中最大和最令人期待的语言改变。它允许将函数当成参数传递给某个方法，或者把代码本身当作数据处理，函数式开发者非常熟悉这些概念。<br />很多JVM平台上的语言（Groovy、Scala等）从诞生之日就支持 Lambda 表达式，但是 Java 开发者没有选择，只能使用匿名内部类代替Lambda表达式。
```java
//匿名内部类方式排序
List<String> names = Arrays.asList( "a", "b", "d" );
Collections.sort(names, new Comparator<String>() {
    @Override
    public int compare(String s1, String s2) {
        return s1.compareTo(s2);
    }
});
```
Lambda 的设计可谓耗费了很多时间和很大的社区力量，最终找到一种折中的实现方案，可以实现简洁而紧凑的语言结构。<br />Lambda 表达式的语法格式：
```java
(parameters) -> expression
或
(parameters) ->{ statements; }
```
Lambda 编程风格，可以总结为四类：

- **可选类型声明**：不需要声明参数类型，编译器可以统一识别参数值
- **可选的参数圆括号**：一个参数无需定义圆括号，但多个参数需要定义圆括号
- **可选的大括号**：如果主体包含了一个语句，就不需要使用大括号
- **可选的返回关键字**：如果主体只有一个表达式返回值则编译器会自动返回值，大括号需要指定明表达式返回了一个数值
<a name="vFHPm"></a>
# 2、Lambda表达式的编程风格
<a name="8TMEx"></a>
## 2.1 可选类型声明
在使用过程中，可以不用显示声明参数类型，编译器可以统一识别参数类型，例如：
```java
Collections.sort(names, (s1, s2) -> s1.compareTo(s2));
```
上面代码中的参数`s1`、`s2`的类型是由编译器推理得出的，也可以显式指定该参数的类型，例如：
```java
Collections.sort(names, (String s1, String s2) -> s1.compareTo(s2));
```
运行之后，两者结果一致！
<a name="hfy5D"></a>
## 2.2 可选的参数圆括号
当方法那只有一个参数时，无需定义圆括号，例如：
```java
Arrays.asList( "a", "b", "d" ).forEach( e -> System.out.println( e ) );
```
但多个参数时，需要定义圆括号，例如：
```java
Arrays.asList( "a", "b", "d" ).sort( ( e1, e2 ) -> e1.compareTo( e2 ) );
```
<a name="2pcBo"></a>
## 2.3 可选的大括号
当主体只包含了一行时，无需使用大括号，例如：
```java
Arrays.asList( "a", "b", "c" ).forEach( e -> System.out.println( e ) );
```
当主体包含多行时，需要使用大括号，例如：
```java
Arrays.asList( "a", "b", "c" ).forEach( e -> {
    System.out.println( e );
    System.out.println( e );
} );
```
<a name="pkCXf"></a>
## 2.4 可选的返回关键字
如果表达式中的语句块只有一行，则可以不用使用`return`语句，返回值的类型也由编译器推理得出，例如：
```java
Arrays.asList( "a", "b", "d" ).sort( ( e1, e2 ) -> e1.compareTo( e2 ) );
```
如果语句块有多行，可以在大括号中指明表达式返回值，例如：
```java
Arrays.asList( "a", "b", "d" ).sort( ( e1, e2 ) -> {
    int result = e1.compareTo( e2 );
    return result;
} );
```
<a name="3W7Rp"></a>
## 2.5 变量作用域
还有一点需要了解的是，Lambda 表达式可以引用类成员和局部变量，但是会将这些变量隐式得转换成`final`，例如：
```java
String separator = ",";
Arrays.asList( "a", "b", "c" ).forEach(
    ( String e ) -> System.out.print( e + separator ) );
```
和
```java
final String separator = ",";
Arrays.asList( "a", "b", "c" ).forEach(
    ( String e ) -> System.out.print( e + separator ) );
```
两者等价！<br />同时，Lambda 表达式的局部变量可以不用声明为`final`，但是必须不可被后面的代码修改（即隐性的具有 `final` 的语义），例如：
```java
int num = 1;
Arrays.asList(1,2,3,4).forEach(e -> System.out.println(num + e));
num =2;
//报错信息：Local variable num defined in an enclosing scope must be final or effectively final
```
在 Lambda 表达式当中不允许声明一个与局部变量同名的参数或者局部变量，例如：
```java
int num = 1;
Arrays.asList(1,2,3,4).forEach(num -> System.out.println(num));
//报错信息：Variable 'num' is already defined in the scope
```
<a name="9gJSW"></a>
# 3、与Lambda表达式处理的对比案例
<a name="OGdgu"></a>
## A.匿名内部类和Lambda表达式比较
```java
// 原来的匿名内部类
@Test
public void comparatorTest() {
    Comparator<Integer> comparator = new Comparator<Integer>() {
        @Override
        public int compare(Integer o1, Integer o2) {
            return Integer.compare(o1, o2);
        }
    };
    TreeSet<Integer> treeSet = new TreeSet<>(comparator);
}

// Lambda表达式的写法
@Test
public void lambdaTest() {
    Comparator<Integer> comparator = (x, y) -> Integer.compare(x, y);
    TreeSet<Integer> treeSet = new TreeSet<>(comparator);
}
```
<a name="q5Vhk"></a>
## B.需求：对员工进行条件查询
<a name="MLDuT"></a>
### Employee.java
```java
package com.fcant.java8.lambda.bean;
/**
 * Employee
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 21:20:09 2020/2/18/0018
 */
public class Employee {
    private String name;
    private int age;
    private double salary;

    public Employee() {
    }

    public Employee(String name, int age, double salary) {
        this.name = name;
        this.age = age;
        this.salary = salary;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public double getSalary() {
        return salary;
    }

    public void setSalary(double salary) {
        this.salary = salary;
    }

    @Override
    public String toString() {
        return "Employee{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", salary=" + salary +
                '}';
    }
}
```
<a name="HM2UF"></a>
### 需求方法代码
```java
List<Employee> employees = Arrays.asList(
    new Employee("Fcant", 14, 99998.0),
    new Employee("Fcary", 10, 998.045),
    new Employee("Fcloi", 15, 934598.0),
    new Employee("Fcmop", 19, 56998.04),
    new Employee("Fcctr", 18, 945698.0),
    new Employee("Fcqyt", 17, 998.0645)
);

// 需求：获取当前公司中员工年龄大于17的员工的信息
public List<Employee> filterEmployeeByAge(List<Employee> list) {
    List<Employee> employees = new ArrayList<>();
    for (Employee employee : list) {
        if (employee.getAge() > 16) {
            employees.add(employee);
        }
    }
    return employees;
}

// 需求：获取当前公司中员工薪水大于5000的员工的信息
public List<Employee> filterEmployeeBySalary(List<Employee> list) {
    List<Employee> employees = new ArrayList<>();
    for (Employee employee : list) {
        if (employee.getSalary() > 5000) {
            employees.add(employee);
        }
    }
    return employees;
}
```
<a name="MErco"></a>
### 对以上重复性需求进行优化处理
<a name="HbTse"></a>
#### ①、优化方式一：策略设计模式
<a name="kSwKX"></a>
##### 将相差不大需求使用的统一接口来处理
```java
package com.fcant.java8.lambda.inter;

/**
 * ExPredicate
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 21:40:31 2020/2/18/0018
 */
public interface ExPredicate<T> {
    public boolean is(T t);
}
```
<a name="Wb3OB"></a>
##### 对接口进行不同的需求通过实现类来实现
```java
package com.fcant.java8.lambda.inter.impl;

import com.fcant.java8.lambda.bean.Employee;
import com.fcant.java8.lambda.inter.ExPredicate;

/**
 * FilterEmployeeByAge
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:28:35 2020/2/19/0019
 */
public class FilterEmployeeByAge implements ExPredicate<Employee> {

    @Override
    public boolean is(Employee employee) {
        return employee.getAge() > 15;
    }
}
```
```java
package com.fcant.java8.lambda.inter.impl;

import com.fcant.java8.lambda.bean.Employee;
import com.fcant.java8.lambda.inter.ExPredicate;

/**
 * FilterEmployeeBySalary
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:39:30 2020/2/19/0019
 */
public class FilterEmployeeBySalary implements ExPredicate<Employee> {
    @Override
    public boolean is(Employee employee) {
        return employee.getSalary() > 5000;
    }
}
```
```java
// 优化方式一:策略设计模式
public List<Employee> filterEmployee(List<Employee> list, ExPredicate<Employee> exPredicate) {
    List<Employee> employees = new ArrayList<>();
    for (Employee employee : list) {
        if (exPredicate.is(employee)) {
            employees.add(employee);
        }
    }
    return employees;
}

@Test
public void Test() {
    // List<Employee> employees = filterEmployeeByAge(this.employees);
    List<Employee> employeeListByAge = filterEmployee(this.employees, new FilterEmployeeByAge());
    for (Employee employee : employeeListByAge) {
        System.out.println(employee);
    }

    System.out.println("----------------根据薪资过滤---------------");

    List<Employee> employeeListBySalary = filterEmployee(this.employees, new FilterEmployeeBySalary());
    for (Employee employee : employeeListBySalary) {
        System.out.println(employee);
    }
}
```
<a name="LeTNM"></a>
#### ②、优化方式二：匿名内部类
```java
@Test
public void Test() {
    // 优化方式二：匿名内部类
    System.out.println("-------------匿名内部类-------------");
    List<Employee> filterEmployee = filterEmployee(this.employees, new ExPredicate<Employee>() {
        @Override
        public boolean is(Employee employee) {
            return employee.getSalary() < 5000;
        }
    });
    for (Employee employee : filterEmployee) {
        System.out.println(employee);
    }
}
```
<a name="DaMs0"></a>
#### ③、优化方式三：Lambda表达式
```java
@Test
public void Test() {
    // 优化方式三：Lambda表达式
    System.out.println("-------------Lambda表达式------------");
    List<Employee> employeesByLambda = filterEmployee(this.employees, (e) -> e.getSalary() <= 5000);
    employeesByLambda.forEach(System.out::println);
}
```
<a name="Z2q9R"></a>
#### ④、优化方式四：Stream流
```java
@Test
public void Test() {
    // 优化方式四：Stream流
    System.out.println("-----------------Stream流过滤---------------");
    employees.stream()
        .filter((e) -> e.getSalary() >= 5000)
        .limit(2)
        .forEach(System.out::println);
    employees.stream()
        .map(Employee::getName)
        .forEach(System.out::println);
}
```
<a name="M5UCm"></a>
# 4、Lambda表达式的基础语法
Java8中引入了一个新的操作符"->"该操作符称为箭头操作符或者Lambda操作符，箭头操作符将Lambda表达式拆分成两部分

- 1.左侧：Lambda表达式的参数列表
- 2.右侧：Lambda表达式中所需执行的功能，即Lambda体
<a name="l0GFU"></a>
## A.语法格式一：无参数，无返回值
```java
() -> System.out.println("Hello Lambda!")
```
```java
// 1.无参数，无返回值
@Test
public void noParamNoReturnTest() {
    Runnable runnable = new Runnable() {
        @Override
        public void run() {
            System.out.println("Hello World");
        }
    };
    runnable.run();
    System.out.println("---------Lambda实现--------");
    Runnable r = () -> System.out.println("Hello Lambda!");
    r.run();
}
```
<a name="0bTxq"></a>
## B.语法格式二：有一个参数，无返回值
```java
(x) -> System.out.println(x);
```
```java
// 2.有一个参数，无返回值
@Test
public void oneParamNoReturnTest() {
    Consumer<String> consumer = (x) -> System.out.println(x);
    consumer.accept("Fv");
}
```
<a name="jiW2j"></a>
## C.语法格式三：有一个参数，无返回值时小括号可以省略不写
```java
x -> System.out.println(x);
```
```java
// 3.有一个参数，无返回值时可以省略括号
@Test
public void oneParamNoReturnLostTest() {
    Consumer<String> consumer = x -> System.out.println(x);
    consumer.accept("Fv");
}
```
<a name="BjnEF"></a>
## D.语法格式四：有一个或多个参数，有返回值，有多条处理语句
```java
 Comparator<Integer> comparator = (x, y) -> {
     System.out.println("函数式接口");
     return Integer.compare(x, y);
 };
```
```java
// 4.有一个或多个参数，有返回值，有多条处理语句
@Test
public void paramReturnTest() {
    Comparator<Integer> comparator = (x, y) -> {
        return Integer.compare(x, y);
    };
    System.out.println(comparator.compare(1, 12));
}
```
<a name="p5lWO"></a>
## E.语法格式五：若Lambda体中只有一条语句，return和大括号都可以不写
```java
(x, y) -> Integer.compare(x, y);
```
```java
// 5.只有一条语句，return和大括号都可以不写
@Test
public void paramReturnLostTest() {
    Comparator<Integer> comparator = (x, y) -> Integer.compare(x, y);
    System.out.println(comparator.compare(1, 12));
}
```
<a name="qYNrK"></a>
## F.语法格式六：Lambda表达式的参数列表的数据类型可以省略不写，因为JVM编译器通过上下文推断出，数据类型，即“类型推断”
```java
(Integer x, Integer y) -> Integer.compare(x, y);
```
```java
// 6.Lambda表达式的参数列表的数据类型可以省略不写
@Test
public void paramTypeReturnLostTest() {
    Comparator<Integer> comparator = (Integer x, Integer y) -> Integer.compare(x, y);
    System.out.println(comparator.compare(1, 12));
}
```
<a name="Mb5EW"></a>
# 5、Lambda案例
<a name="YT36o"></a>
## 案例A
调用`Collection.sort()`方法，通过定制排序比较两个Employee（先按年龄比，年龄相同按姓名比，使用Lambda作为参数传递）
```java
@Test
public void empTest() {
    Collections.sort(employees, (e1, e2) -> {
        if (e1.getAge() == e2.getAge()) {
            return e1.getName().compareTo(e2.getName());
        }else {
            return -Integer.compare(e1.getAge(), e2.getAge());
        }
    });
    employees.forEach(System.out::println);
}
```
<a name="TTUTT"></a>
## 案例B
①声明函数式接口，接口中声明抽象方法，`public String getValue(String str);`<br />②声明类TestLambda，类中编写方法使用接口作为参数，将一个字符串转换成大写，并作为方法的返回值。<br />③再将一个字符串的第二个和第四个索引位置进行截取子串
```java
package com.fcant.java8.lambda.inter;

/**
 * Func
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 20:24:19 2020/2/19/0019
 */
@FunctionalInterface
public interface Func<T> {
    String getValue(T t);
}
```
```java
// 案例二
// ①声明函数式接口，接口中声明抽象方法，public String getValue（String str）；
// ②声明类TestLambda，类中编写方法使用接口作为参数，将一个字符串转换成大写，并作为方法的返回值。
// ③再将一个字符串的第二个和第四个索引位置进行截取子串
@Test
public void functionTest() {
    System.out.println(option("agHdfC", (x) -> x.toUpperCase()));
    System.out.println(option("hsafdsfh", (x) -> x.substring(2, 5)));
}

public String option(String s, Func<String> func) {
    return func.getValue(s);
}
```
<a name="vHNjA"></a>
## 案例C
①声明一个带两个泛型的函数式接口，泛型类型为`<T, R>`,`T`为参数，`R`为返回值<br />②接口中声明对应的抽象方法<br />③在TestLambda类中声明方法，使用接口作为参数，计算两个long型参数的和<br />④再计算两个long型参数的乘积
```java
package com.fcant.java8.lambda.inter;

/**
 * Funca
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 20:32:15 2020/2/19/0019
 */
@FunctionalInterface
public interface Funca<T, R> {
    R op(T t1, T t2);
}
```
```java
// 案例三
// ①声明一个带两个泛型的函数式接口，泛型类型为<T, R>,T为参数，R为返回值
// ②接口中声明对应的抽象方法
// ③在TestLambda类中声明方法，使用接口作为参数，计算两个long型参数的和
// ④再计算两个long型参数的乘积
@Test
public void trTest() {
    System.out.println(ops(12l, 56l, (t1, t2) -> t1 + t2));
    System.out.println(ops(12l, 56l, (t1, t2) -> t1 * t2));
}

public Long ops(Long t1, Long t2, Funca<Long, Long> funca) {
    return funca.op(t1, t2);
}
```

