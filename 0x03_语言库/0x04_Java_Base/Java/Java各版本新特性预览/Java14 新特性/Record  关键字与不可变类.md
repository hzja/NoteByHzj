JavaRecord<br />Record 关键字在 Java14 过后才支持的，还在使用 Java 8 的一定没用过！
<a name="FEdRQ"></a>
## 不可变类
先看一下定义的不可变类，代码如下。
```java
package com.example.demo.immutable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class Teacher {
    private final String name;
    private final List<String> students;
    private final Address address;
    private final Map<String, String> metadata;

    public Teacher(String name, List<String> students, Address address, Map<String, String> metadata) {
        this.name = name;
        this.students = students;
        this.address = address;
        this.metadata = metadata;
    }

    public String getName() {
        return name;
    }

    public List<String> getStudents() {
        return new ArrayList<>(students);
        //    return students;
    }

    public Address getAddress() {
        //    return address;
        return address.clone();
    }

    public Map<String, String> getMetadata() {
        return new HashMap<>(metadata);
        //    return metadata;
    }
}
```
如果复制上面代码到 IDEA 中，并且刚好 JDK 版本是 Java14 之后的话，那么就会看到下面这个提示，提示可以将 Teacher 这个不可变类转换为 Record。怎么样是不是很懵，没关系，按照提示操作一下看看会发生什么。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669681945492-2996701b-4bb1-4e33-a75c-a01af74d54fd.jpeg#averageHue=%2341382b&clientId=u209f727e-824c-4&from=paste&id=uaa1e4f78&originHeight=429&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf45eedd1-2a3f-4c09-8c50-dea8a9512d0&title=)<br />点完之后代码会变成下面的样子
```java
package com.example.demo.immutable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public record Teacher(String name, List<String> students, Address address, Map<String, String> metadata) {

    @Override
    public List<String> students() {
        return new ArrayList<>(students);
        //    return students;
    }

    @Override
    public Address address() {
        //    return address;
        return address.clone();
    }

    @Override
    public Map<String, String> metadata() {
        return new HashMap<>(metadata);
        //    return metadata;
    }
}
```
仔细一看会发现，这是什么情况，record 是什么关键字，然后类名后面怎么还有参数？乍一看还以为变成一个方法了。此外之前的测试代码不用修改任何逻辑，照样可以正常运行，是不是很神奇？这就是 Record 关键字的特性。
<a name="IoovP"></a>
## Record 关键字
看完了 Record 关键字的 case ，来聊一下 Record 关键字是怎么用的，以及它有什么特性。

1. Record 关键定义的类是不可变类；
2. Record 定义的类需要将所有成员变量通过参数的形式定义；
3. Record 定义的类默认会生成全部参数的构造方法；
4. Record 定义的类中可以定义静态方法；
5. Record 定义的类可以提供紧凑的方式进行参数校验；

上面的五点里面前三点在之前的例子中都可以看出来，在定义和使用的时候可以明显的看到，如下所示。
```java
public record Teacher(String name, List<String> students, Address address, Map<String, String> metadata) {
}//1,2
Teacher teacher = new Teacher("Java", students, address, metadata);//3
```
下面看下第四点和第五点，关于第四点可以在 Record 类中定义静态方法用来默认初始化对象，如下所示，通过这种方式可以写出更简洁的代码。
```java
public static Teacher of() {
    return new Teacher("Java", new ArrayList<>(), new Address(), new HashMap<>());
}

public static Teacher of(String name) {
    return new Teacher(name, new ArrayList<>(), new Address(), new HashMap<>());
}
```
在使用的时候，就可以直接通过类名引用静态方法就可以了，如下所示
```java
Teacher teacher = Teacher.of();
```
接下来看看什么叫紧凑的方式进行参数校验，试想一下，如果需要校验在沟通 Teacher 对象的时候，student 成员变量不能为空，在以前的写法里面只要在构造方法里面进行一下判空就可以了，但是对于 Record 的形式，没有显示的创建构造方法，那应该如何进行判断呢？答案如下
```java
public Teacher {
    if (null == students || students.size() == 0) {
        throw new IllegalArgumentException();
    }
}
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669681945405-26f48c79-5474-4914-afda-dcd0230a0b1b.jpeg#averageHue=%232f302c&clientId=u209f727e-824c-4&from=paste&id=u8678b98f&originHeight=240&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1a2a2be2-4609-432d-9751-8906a8b5647&title=)<br />可以看到通过一种紧凑的构造方法的形式来进行了参数的校验，这种写法跟普通的构造方法是不一样的，没有方法参数，怎么样是不是很神奇。
<a name="wBnkY"></a>
## 总结
有的人说 Java 的 Record 的新特性是为了让大家不使用 Lombok 的，毕竟 Lombok 用起来是真的香，而且 Record  也只能是定义不可变类，在某些情况下使用还是有局限性的，不可变类的使用场景并不是很多。
