Java<br />对象创建方式及Java中的深拷贝还是浅拷贝。
<a name="ILpGn"></a>
## 1、创建对象的5种方式
<a name="ItJbx"></a>
### 通过 `new` 关键字
这是最常用的一种方式，通过 `new` 关键字调用类的有参或无参构造方法来创建对象。比如 
```java
Object obj = new Object();
```
<a name="mHmo3"></a>
### 通过 Class 类的 `newInstance()` 方法
这种默认是调用类的无参构造方法创建对象。比如 
```java
Person p2 = (Person) Class. forName("com.ys.test. Person"). newInstance();
```
<a name="q78H8"></a>
### 通过 Constructor 类的 `newInstance` 方法
这和第二种方法类时，都是通过反射来实现。通过 `java.lang.relect.Constructor` 类的 `newInstance()` 方法指定某个构造器来创建对象。 `Person p3 = (Person) Person.class.getConstructors()[0].newInstance()`; 实际上第二种方法利用 Class 的 `newInstance()` 方法创建对象，其内部调用还是 Constructor 的 `newInstance()` 方法。
<a name="WftPR"></a>
### 利用 `Clone` 方法
Clone 是 Object 类中的一个方法，通过 对象A.clone() 方法会创建一个内容和对象 A 一模一样的对象 B，clone 克隆，顾名思义就是创建一个一模一样的对象出来。 
```java
Person p4 = (Person) p3.clone();
```
<a name="yq3sy"></a>
### 序列化
序列化是把堆内存中的 Java 对象数据，通过某种方式把对象存储到磁盘文件中或者传递给其他网络节点（在网络上传输）。而反序列化则是把磁盘文件中的对象数据或者把网络节点上的对象数据，恢复成Java对象模型的过程。
<a name="rZRJ6"></a>
## Java 基本复制方法
java赋值是复制**「对象引用」**，如果想要得到一个对象的==副本==，使用赋值操作是无法达到目的的：修改新对象的值会同时修改旧对象的值。
```java
public class Client{
    public static void main(String[] args) throws CloneNotSupportedException{
        Person person = new Person(15, "sowhat", new Address("河北", "建华南大街"));
        Person p1 = person;
        p1.setAge(45);
        System.out.println(p1.hashCode());
        System.out.println(person.hashCode());
        System.out.println("================");
        System.out.println(p1.display());
        System.out.println(person.display());
    }
}
```
<a name="syoeM"></a>
## Clone 方法
如果创建一个对象的新的副本，也就是说他们的初始状态完全一样，但以后可以**「改变各自的状态」**，而互不影响，就需要用到java中对象的复制，如原生的`clone()`方法。本次讲解的是 Java 的深拷贝和浅拷贝，其实现方式正是通过调用 Object 类的 `clone()` 方法来完成。在 Object.class 类中，源码为:
```java
/**
* ...
* performs a "shallow copy" of this object, not a "deep copy" operation.
* 上面这里已经说明了，clone()方法是浅拷贝，而不是深拷贝
* @see java.lang.Cloneable
*/
protected native Object clone() throws CloneNotSupportedException;
```
这是一个用 native 关键字修饰的方法，只需要知道用 native 修饰的方法就是告诉操作系统，这个方法不实现了，让操作系统去实现(参考JNI)。具体怎么实现不需要了解，只需要知道 `clone`方法的作用就是**「复制对象」**，产生一个新的对象。那么这个新的对象和原对象是==什么关系呢==？
<a name="Ye80g"></a>
## 基本类型和引用类型
这里再给大家普及一个概念，在 Java 中**「基本类型和引用类型」**的区别。在 Java 中数据类型可以分为两大类：基本类型和引用类型。基本类型也称为值类型，分别是字符类型 char，布尔类型 boolean以及数值类型 byte、short、int、long、float、double。引用类型则包括类、接口、数组、枚举等。 Java 将内存空间分为**「堆和栈」**。基本类型直接在栈 stack中存储数值，而引用类型是将引用放在栈中，实际存储的值是放在堆 heap中，通过栈中的引用指向堆中存放的数据。 <br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660726413241-69bbe45a-c232-4258-8d81-502d288dc664.jpeg)<br />上图定义的 a 和 b 都是基本类型，其值是**「直接存放在栈中」**的；而 c 和 d 是 String 声明的，这是一个引用类型，**「引用地址是存放在栈中，然后指向堆的内存空间」**。 下面 d = c；这条语句表示将 c 的引用赋值给 d，那么 c 和 d 将指向同一块堆内存空间。
<a name="hUiVo"></a>
## 浅拷贝
接下来用代码看看浅拷贝的效果。
```java
package mytest;

@Data//lombok注解
class Person implements Cloneable{
    private int age;
    private String name;
    private Address address;

    public Person(int age, String name, Address address)
    {
        this.age = age;
        this.name = name;
        this.address = address;
    }

    @Override
    protected Object clone() throws CloneNotSupportedException
    {
        return super.clone();
    }

    public String display()
    {
        return "Person [age=" + age + ", name=" + name + ", address=" + address + "]";
    }
}

@Data//lombok注解
class Address{
    private String province;
    private String street;

    public Address(String province, String street)
    {
        this.province = province;
        this.street = street;
    }
    @Override
    public String toString()
    {
        return "Address [province=" + province + ", street=" + street + "]";
    }
}

public class Client{
    public static void main(String[] args) throws CloneNotSupportedException{
        Person person = new Person(15, "sowhat", new Address("河北", "建华南大街"));
        Person clonePerson = (Person) person.clone();
        System.out.println(person);
        System.out.println(clonePerson);

        // 信息完全一样
        System.out.println(person.display());
        System.out.println(clonePerson.display());
        System.out.println("信息完全一致");

        System.out.println("原始年龄:" + person.getAge());
        System.out.println("克隆后原始年龄:" + clonePerson.getAge());
        System.out.println("年龄完全一样");

        System.out.println("原始名字哈希值:" + person.getName().hashCode());
        System.out.println("克隆后名字哈希值:" + clonePerson.getName().hashCode());
        System.out.println("字符串哈希值完全一样");

        clonePerson.setName("xiaomai");
        clonePerson.setAge(20);
        clonePerson.getAddress().setStreet("中山路");
        System.out.println(clonePerson.display());
        System.out.println(person.display());
        System.out.println("年龄跟姓名 是完全的深拷贝 副本跟原值无关的！");
        System.out.println("地址信息的修改是浅拷贝  ");

    }
}
```
结果如下：
```java
mytest.Person@15f550a 
mytest.Person@6b2d4a 
Person [age=15, name=sowhat, address=Address [province=河北, street=建华南大街]] 
Person [age=15, name=sowhat, address=Address [province=河北, street=建华南大街]]
信息完全一致 
原始年龄:15 
克隆后原始年龄:15 
年龄完全一样 
原始名字哈希值:-1432601412 
克隆后名字哈希值:-1432601412 
字符串哈希值完全一样 
Person [age=20, name=xiaomai, address=Address [province=河北, street=中山路]]
Person [age=15, name=sowhat, address=Address [province=河北, street=中山路]]
```
结论：

1. 原对象与新对象是两个不同的对象。
2. 拷贝出来的新对象与原对象内容一致
3. 接着将新对象里面的信息进行了修改，然后输出发现原对象里面的部分信息也跟着变了。其中 基本类型跟 String类型的改变不会影响到 原始对象的改变。而其他的Ojbect 类型改变的时候会影响到原始数据。上面的结论称为浅拷贝**「浅拷贝」**：创建一个新对象，然后将当前对象的非静态字段复制到该对象，如果字段类型是值类型（基本类型跟String）的，那么对该字段进行**「复制」**；如果字段是引用类型的，**「则只复制该字段的引用而不复制引用指向的对象(也就是只复制对象的地址)」**。此时新对象里面的引用类型字段相当于是原始对象里面引用类型字段的一个副本，原始对象与新对象里面的引用字段指向的是同一个对象。因此，修改clonePerson里面的address内容时，原person里面的address内容会跟着改变。
<a name="AlaBq"></a>
## 深拷贝
了解了浅拷贝，那么深拷贝是什么也就很清楚了。那么该如何实现深拷贝呢？Object 类提供的 clone 是只能实现 浅拷贝的。，即将**「引用类型的属性内容也拷贝一份新的」**。那么，实现深拷贝这里收集到两种方式：**「第一种」**是给需要拷贝的引用类型也实现Cloneable接口并覆写`clone`方法；**「第二种」**则是利用序列化。接下来分别对两种方式进行演示。
<a name="vgx4a"></a>
### 深拷贝-`clone`方式
对于以上演示代码，利用clone方式进行深拷贝无非就是将Address类也实现`Cloneable`，然后对Person的clone方法进行调整。让每个引用类型属性内部都重写`clone()` 方法，既然引用类型不能实现深拷贝，那么将每个引用类型都拆分为基本类型，分别进行浅拷贝。比如上面的例子，Person 类有一个引用类型 Address(其实String 也是引用类型，但是String类型有点特殊，后面会详细讲解)，在 Address 类内部也重写 clone 方法。如下：
```java
package mytest;

@Data//lombok注解
class Person implements Cloneable{
    private int age;
    private String name;
    private Address address;
    protected int abc = 12;

    public Person(int age, String name, Address address){
        this.age = age;
        this.name = name;
        this.address = address;
    }

    @Override  // clone 重载
    protected Object clone() throws CloneNotSupportedException{
        Person person = (Person) super.clone();
        //手动对address属性进行clone，并赋值给新的person对象
        person.address = (Address) address.clone();
        return person;
    }

    public String display()
    {
        return "Person [age=" + age + ", name=" + name + ", address=" + address + "]";
    }
}

@Data//lombok注解
class Address implements  Cloneable{
    private String province;
    private String street;

    public Address(String province, String street)
    {
        this.province = province;
        this.street = street;
    }
    // 深拷贝时添加
    @Override
    protected Object clone() throws CloneNotSupportedException
    {
        return super.clone();
    }

    @Override
    public String toString()
    {
        return "Address [province=" + province + ", street=" + street + "]";
    }
}

public class Client{
    public static void main(String[] args) throws CloneNotSupportedException{
        Person person = new Person(15, "sowhat", new Address("河北", "建华南大街"));
        Person p1 = person;
        p1.setAge(45);
        System.out.println(p1.hashCode());
        System.out.println(person.hashCode());

        System.out.println(p1.display());
        System.out.println(person.display());
        System.out.println("-----------");

        Person clonePerson = (Person) person.clone();
        System.out.println(person);
        System.out.println(clonePerson);

        // 信息完全一样
        System.out.println(person.display());
        System.out.println(clonePerson.display());
        System.out.println("信息完全一致");

        System.out.println("原始年龄:" + person.getAge());
        System.out.println("克隆后原始年龄:" + clonePerson.getAge());
        System.out.println("年龄完全一样");

        System.out.println("原始名字哈希值:" + person.getName().hashCode());
        System.out.println("克隆后名字哈希值:" + clonePerson.getName().hashCode());
        System.out.println("字符串哈希值完全一样");

        clonePerson.setName("sowhat1412");
        clonePerson.setAge(20);
        clonePerson.getAddress().setStreet("中山路");
        System.out.println(clonePerson.display());
        System.out.println(person.display());
        System.out.println("年龄跟姓名 是完全的深拷贝 副本跟原值无关的！");
        System.out.println("地址信息的修改是浅拷贝  ");

    }
}
```
但是这种做法有个弊端，这里Person 类只有一个 Address 引用类型，而 Address 类没有，所以只用重写 Address 类的clone 方法，但是如果 Address 类也存在一个引用类型，那么也要重写其`clone` 方法，这样下去，有多少个引用类型，就要重写多少次，如果存在很多引用类型，那么代码量显然会很大，所以这种方法不太合适。
<a name="PzOFX"></a>
### 利用序列化
序列化是将对象写到流中便于传输，而反序列化则是把对象从流中读取出来。这里写到流中的对象则是原始对象的一个拷贝，因为原始对象还存在 JVM 中，所以可以利用对象的序列化产生克隆对象，然后通过反序列化获取这个对象。注意每个需要序列化的类都要实现 Serializable 接口，如果有某个属性不需要序列化，可以将其声明为 transient，即将其排除在克隆属性之外。
```java
package mytest;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
* 利用序列化和反序列化进行对象的深拷贝
*/
class DeepClone implements Serializable{
    private static final long serialVersionUID = 1412L;

    public Object deepClone() throws Exception{
        //序列化
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(bos);
        oos.writeObject(this);
        //反序列化
        ByteArrayInputStream bis = new ByteArrayInputStream(bos.toByteArray());
        ObjectInputStream ois = new ObjectInputStream(bis);
        return ois.readObject();
    }
}

@Data
class Person extends DeepClone{
    private static final long serialVersionUID = 1L;
    private int age;
    private String name;
    private Address address;

    public Person(int age, String name, Address address){
        this.age = age;
        this.name = name;
        this.address = address;
    }

    public String display(){
        return "Person [age=" + age + ", name=" + name + ", address=" + address + "]";
    }
}

@Data
class Address extends DeepClone{
    private static final long serialVersionUID = 1412L;
    private String province;
    private String street;

    public Address(String province, String street)
    {
        this.province = province;
        this.street = street;
    }

    @Override
    public String toString()
    {
        return "Address [province=" + province + ", street=" + street + "]";
    }

    public void setStreet(String street)
    {
        this.street = street;
    }
}

public class Client{
    public static void main(String[] args) throws Exception{
        Person person = new Person(15, "sowhat", new Address("河北", "建华南大街"));

        Person clonePerson = (Person) person.deepClone();

        System.out.println(person);
        System.out.println(clonePerson);

        System.out.println(person.display());
        System.out.println(clonePerson.display());

        clonePerson.setName("sowhat1412");
        clonePerson.setAge(20);
        Address address = clonePerson.getAddress();
        address.setStreet("中山路");
        System.out.println(clonePerson.display());
        System.out.println(person.display());

    }
}
```
