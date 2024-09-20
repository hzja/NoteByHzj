Java Optional
<a name="y7lxX"></a>
## 一、`Optional`类
Java应用中最常见的`bug`就是空值异常。在 Java 8 之前，Google Guava 引入了 `Optionals` 类来解决 `NullPointerException`，从而避免源码被各种 null 检查污染，以便开发者写出更加整洁的代码。Java 8 也将 `Optional `加入了官方库。<br />`Optional<T>`类(java.util.Option)是一个容器类，代表一个值存在或不存在，原来用`null`表示一个值不存在，现在`Optional`可以更好的表达这个概念。`Optional` 提供了一些有用的方法来避免显式的 `null` 检查，并且可以避免空指针异常。
<a name="VLSXG"></a>
### A、常用方法
<a name="lgfTl"></a>
#### ①、创建一个`Optional`实例
```java
Optional.of(T t)
```
<a name="QZMpn"></a>
#### ②、创建一个空的`Optional`实例
```java
Optional.empty()
```
<a name="CU2hx"></a>
#### ③、若`t`不为`null`，创建`Optional`实例，否则创建空实例
```java
Optional.ofNullable(T t)
```
<a name="UtjL3"></a>
#### ④、判断是否包含值
```java
isPresent()
```
<a name="HORA1"></a>
#### ⑤、如果调用对象包含值，返回该值，否则返回`t`
```java
orElse(T t)
```
<a name="jWpeG"></a>
#### ⑥、如果调用对象包含值，返回该值，否则返回`s`获取的值
```java
orElseGet(Supplier s)
```
<a name="51UGd"></a>
#### ⑦、如果有值对其处理，并返回处理后的`Optional`，否则返回`Optional.empty()`
```java
map(Function f)
```
<a name="EOi1f"></a>
#### ⑧、与`map`类似，要求返回值必须是`Optional`
```java
flatMap(Function mapper)
```
<a name="Ea2xF"></a>
### B、常用方法演示
```java
public class OptionalTest {

    /**
     * Optional容器类常用方法
     * 1.Optional.of(T t) 创建一个Optional实例
     * 2.Optional.empty() 创建一个空的Optional实例
     * 3.Optional.ofNullable(T t) 若t不为null，创建Optional实例，否则创建空实例
     * 4.isPresent() 判断是否包含值
     * 5.orElse(T t) 如果调用对象包含值，返回该值，否则返回t
     * 6.orElseGet(Supplier s) 如果调用对象包含值，返回该值，否则返回s获取的值
     * 7.map(Function f) 如果有值对其处理，并返回处理后的Optional，否则返回Optional.empty()
     * 8.flatMap(Function mapper) 与map类似，要求返回值必须是Optional
     *
     * @author Fcant 上午 10:30:11 2020/2/23/0023
     */
    @Test
    public void optionalTest() {
        Optional<Employee> employeeOptional = Optional.of(new Employee());
        Employee employee = employeeOptional.get();
        System.out.println(employee);

        // 构建null的Optional容器

        Optional<Employee> empty = Optional.empty();

        Optional<Employee> objectOptional = Optional.ofNullable(new Employee("Fcant", 18, 19999.0, Employee.Status.BUSY));
        if (objectOptional.isPresent()) {
            System.out.println(objectOptional.get());
        } else {
            // 如果为空，设置默认值
            Employee fcant = objectOptional.orElse(new Employee("Fcant", 18, 19999.0, Employee.Status.BUSY));
            System.out.println(fcant);
        }

        Employee orElseGetEmployee = objectOptional.orElseGet(() -> new Employee());
        System.out.println(orElseGetEmployee);

        Optional<String> empName = objectOptional.map(e -> e.getName());
        System.out.println(empName.get());

        // flatMap要求必须返回是Optional
        Optional<String> optionalS = objectOptional.flatMap(e -> Optional.of(e.getName()));
        System.out.println(optionalS.get());
    }
}
```
<a name="5836a054"></a>
### C、`Optional`的使用案例
<a name="MAX4o"></a>
#### ①、基础类
```java
/**
 * Godness
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 13:58:15 2020/2/23/0023
 */
public class Godness {
    private String name;

    public Godness(String name) {
        this.name = name;
    }

    public Godness() {
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Godness{" +
                "name='" + name + '\'' +
                '}';
    }
}
```

```java
/**
 * Man
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 13:59:35 2020/2/23/0023
 */
public class Man {
    private Godness godness;

    public Man() {
    }

    public Man(Godness godness) {
        this.godness = godness;
    }

    public Godness getGodness() {
        return godness;
    }

    public void setGodness(Godness godness) {
        this.godness = godness;
    }

    @Override
    public String toString() {
        return "Man{" +
                "godness=" + godness +
                '}';
    }
}
```
<a name="a48045e4"></a>
#### ②、测试类
```java
// 需求：获取一个男人心中女神的名字
public String getGodnessName(Man man) {
    if (man != null) {
        Godness godness = man.getGodness();
        if (godness != null) {
            return godness.getName();
        }
    }
    return "苍井空";
}

@Test
public void Test() {
    Man man = new Man();
    String name = getGodnessName(man);
    System.out.println(name);
}
```
<a name="J8ITR"></a>
#### ③、升级类
```java
/**
 * NewMan
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 14:05:04 2020/2/23/0023
 */
public class NewMan {

    private Optional<Godness> godness = Optional.empty();

    public NewMan(Optional<Godness> godness) {
        this.godness = godness;
    }

    public NewMan() {
    }

    public Optional<Godness> getGodness() {
        return godness;
    }

    public void setGodness(Optional<Godness> godness) {
        this.godness = godness;
    }

    @Override
    public String toString() {
        return "NewMan{" +
                "godness=" + godness +
                '}';
    }
}
```
<a name="aNkHc"></a>
#### ④、升级后的防空指针测试方法
```java
public String getGodnessName(Optional<NewMan> newMan) {
    return newMan.orElse(new NewMan())
        .getGodness()
        .orElse(new Godness("苍井空"))
        .getName();
}

@Test
public void Test() {
    Optional<NewMan> newMan = Optional.empty();
    String godnessName = getGodnessName(newMan);
    System.out.println(godnessName);
}
```

