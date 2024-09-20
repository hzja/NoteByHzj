Java
<a name="FtQ89"></a>
## 1、`equals`常见面试题
先看几个常见的面试题，看看能不能都回答上来。

- 1、`equals`和==有什么区别？
- 2、`hashcode`相等的两个对象一定==相等吗？`equals`相等吗？
- 3、两个对象用`equals`比较相等，那它们的`hashcode`相等吗？

---

如果不重写`equals`和`hashcode`，那么它使用的是`Object`方法的实现。先简单看一下
```java
public boolean equals(Object obj) {
    return (this == obj);
}

public static int hashCode(Object o) {
    return o != null ? o.hashCode() : 0;
}
```
<a name="Qhzbw"></a>
## 2、为什么要重写`equals`
通过以上代码可以看出，Object提供的`equals`在进行比较的时候，并不是进行值比较，而是内存地址的比较。由此可以知晓，要使用`equals`对对象进行比较，那么就必须进行重写`equals`。
<a name="CjtAp"></a>
## 3、重写`equals`不重写`hashCode`会存在什么问题
先看下面这段话<br />每个覆盖了`equals`方法的类中，必须覆盖`hashCode`。如果不这么做，就违背了`hashCode`的通用约定，也就是上面注释中所说的。进而导致该类无法结合所以与散列的集合一起正常运作，这里指的是`HashMap`、`HashSet`、`HashTable`、`ConcurrentHashMap`。**来自 Effective Java 第三版**<br />结论：**如果重写**`**equals**`**不重写**`**hashCode**`**它与散列集合无法正常工作。**<br />既然这样那就拿最熟悉的HashMap来进行演示推导吧。`HashMap`中的key是不能重复的，如果重复添加，后添加的会覆盖前面的内容。那么看看`HashMap`是如何来确定key的唯一性的。
```java
static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```
查看代码发现，它是通过计算Map key的`hashCode`值来确定在链表中的存储位置的。那么这样就可以推测出，如果重写了`equals`但是没重写`hashCode`，那么可能存在元素重复的矛盾情况。<br />下面来演示一下
```java
public class Employee {

    private String name;

    private Integer age;

    public Employee(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Employee employee = (Employee) o;
        return Objects.equals(name, employee.name) &&
            Objects.equals(age, employee.age);
    }
    /*@Override
    public int hashCode() {
        return Objects.hash(name, age);
    }*/
}
```
```java
public static void main(String[] args) {

    Employee employee1 = new Employee("冰峰", 20);
    Employee employee2 = new Employee("冰峰", 22);
    Employee employee3 = new Employee("冰峰", 20);

    HashMap<Employee, Object> map = new HashMap<>();

    map.put(employee1, "1");
    map.put(employee2, "1");
    map.put(employee3, "1");

    System.out.println("equals：" + employee1.equals(employee3));
    System.out.println("hashCode：" + (employee1.hashCode() == employee3.hashCode()));
    System.out.println(JSONObject.toJSONString(map));
}
```
按正常情况来推测，map中只存在两个元素，employee2和employee3。<br />执行结果<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638796387557-b0d48e6e-db30-4c51-9e7c-cf19160eb9fe.webp#clientId=u80e2d9b7-5c90-4&from=paste&id=u14815d52&originHeight=95&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u78c3091a-4ab7-4b6c-96b4-5228287cff3&title=)<br />出现这种问题的原因就是因为没有重写hashCode，导致map在计算key的hash值的时候，绝对值相同的对象计算除了不一致的hash值。

---

接下来打开`hashCode`的注释代码，看看执行结果<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638796387318-c36b12e3-76c3-4781-95c8-ccb83eb642dc.webp#clientId=u80e2d9b7-5c90-4&from=paste&id=uf9142b33&originHeight=76&originWidth=730&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u720ddb7d-c830-454c-91d3-4ed7569da0b&title=)
<a name="wOCqq"></a>
## 4、总结
如果重写了`equals`就必须重写`hashCode`，如果不重写将引起与散列集合（`HashMap`、`HashSet`、`HashTable`、`ConcurrentHashMap`）的冲突。
