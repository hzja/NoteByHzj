Java<br />如果说在日常开发中用到的最多的一个 Java 类是什么，相信大多数人的回答是是 String.class。说到 String 大家都知道 String 是一个不可变的类；虽然用的很多，那不知道小伙伴们有没有想过怎么样创建一个自己的不可变的类呢？这里来实践一下，创建一个自己的不可变的类。
<a name="KwYmz"></a>
## 特性
在手动编写代码之前，先了解一下不可变类都有哪些特性，

1. 定义类的时候需要使用 `final` 关键字进行修饰：之所以使用 `final` 进行修饰是因为这样可以避免被其他类继承，一旦有了子类继承就会破坏父类的不可变性机制；
2. 成员变量需要使用 `final` 关键词修饰，并且需要是 `private` 的：避免属性被外部修改；
3. 成员变量不可提供 `setter` 方法，只能提供 `getter` 方法：避免被外部修改，并且避免返回成员变量本身；
4. 提供所有字段的构造函数；
<a name="XrbyG"></a>
## 实际操作
知道了不可变类的一些基本特性之后，来实际写代码操作一下，以及会验证一下，如果不按照上面的要求来编写的话，会出现什么样的问题。<br />这里定义一个 Teacher 类来测试一下，按照上面提到的几点，给类和属性的定义都加上 `final` 代码如下所示。
```java
package com.example.demo.immutable;

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
        return students;
    }

    public Address getAddress() {
        return address;
    }

    public Map<String, String> getMetadata() {
        return metadata;
    }
}
```
```java
package com.example.demo.immutable;

public class Address {
    private String country;
    private String city;

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }
}
```
思考一下，上面的代码是否真正的做到了不可变，思考三秒钟，心里默默的数三下。为了回答这个问题，看下下面的测试代码。
```java
package com.example.demo;

import com.example.demo.immutable.Address;
import com.example.demo.immutable.Teacher;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * <br>
 * <b>Function：</b><br>
 * <b>Desc：</b>无<br>
 */
public class ImmutableDemo {

  public static void main(String[] args) {
    List<String> students = new ArrayList<>();
    students.add("鸭");
    students.add("鸭血");
    students.add("鸭血粉丝");
    Address address = new Address();
    address.setCountry("中国");
    address.setCity("深圳");
    Map<String, String> metadata = new HashMap<>();
    metadata.put("hobby", "篮球");
    metadata.put("age", "29");
    Teacher teacher = new Teacher("Java", students, address, metadata);
    System.out.println(teacher.getStudents().size());
    System.out.println(teacher.getMetadata().size());
    System.out.println(teacher.getAddress().getCity());

    // 修改属性
    teacher.getStudents().add("小明");
    teacher.getMetadata().put("weight", "120");
    teacher.getAddress().setCity("广州");

    System.out.println(teacher.getStudents().size());
    System.out.println(teacher.getMetadata().size());
    System.out.println(teacher.getAddress().getCity());
  }

}
```
运行的结果如下截图所示，通过测试可以发现，简单的只添加 `final` 关键字是不能解决不可变性的，当前的 teacher 实例已经被外层修改掉了成员变量。<br />为了解决这个问题，还需要对 Teacher 类进行改造，首先可以想到的就是需要将 students 和 metadata 两个成员变量不能直接返回给外层，否则外层的修改会直接影响到不可变类，那么就可以修改 getter 方法，拷贝一下成员变量进行返回，而不是直接返回，修改代码如下
```java
public List<String> getStudents() {
    return new ArrayList<>(students);
    //return students;
}
public Map<String, String> getMetadata() {
    return new HashMap<>(metadata);
    //return metadata;
}
```
再次运行上面的测试代码，可以看到这次的返回数据如下，这次 students 和 metadate 成员变量并没有被外层修改掉了。但是 address 成员变量还是有问题，没关系，接着往下看。<br />很自然的为了解决 address 的问题，想到了也是进行一个拷贝，再调用 getter 方法的时候返回一个拷贝对象，而不是直接返回成员变量。那就需要改造 Address 类，将其变成 Cloneable 的即可，实现 接口，然后覆盖一个 clone 方法，代码如下
```java
package com.example.demo.immutable;

public class Address implements Cloneable{
    ...// 省略
    @Override
    public Address clone() {
        try {
            return (Address) super.clone();
        } catch (CloneNotSupportedException e) {
            throw new AssertionError();
        }
    }
}
```
再修改 Teacher 的 `getAddress` 方法
```java
public Address getAddress() {
    //return address;
    return address.clone();
}
```
接下来再运行一下测试代码，结果如下，可以看到这次 teacher 实例的成员变量并没有被修改掉了，至此完成了一个不可变对象的创建！<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669441857690-b20ab9bf-4e15-4032-b0d5-1a487c676005.jpeg#averageHue=%2335352d&clientId=uadcc1c3b-cf05-4&from=paste&id=ucb4a7c02&originHeight=772&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u957e2a76-6250-43c2-8f94-95252c35d70&title=)
<a name="sE1eP"></a>
## String 的实现
前面看的是自定义实现不可变类的操作，接下来简单看一下 String 类是如何实现不可变的，通过源码可以看到 String 也使用了关键字 `final` 来避免被子类继承，以及对应存放具体值的成员变量也使用了 `final` 关键字。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669441857665-51c26145-f992-45b8-8966-14b5d4d1d337.jpeg#averageHue=%23302a20&clientId=uadcc1c3b-cf05-4&from=paste&id=u0f2a30ad&originHeight=685&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udcb89123-166f-4d81-ab15-9244d220721&title=)<br />并且对外提供的方法 `substring` 也是通过复制的形式对外提供的新的 String 对象。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669441857587-512e95cf-e6d0-43ac-9067-b4af5b619a7c.jpeg#averageHue=%23444444&clientId=uadcc1c3b-cf05-4&from=paste&id=ua21177d4&originHeight=301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2ce3ad45-0cbf-469d-8ed4-418e75a48ea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669441857927-5692475f-881c-45a2-b8f2-33f1ed61d4f3.jpeg#averageHue=%233e3d3d&clientId=uadcc1c3b-cf05-4&from=paste&id=u04b27bb6&originHeight=363&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udd863b21-457d-4348-8dc6-72182930e1b&title=)
