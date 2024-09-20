Java 序列化和反序列化三连问：

- 什么是 Java 序列化和反序列化？
- 为什么需要 Java 序列化和反序列化？
- 如何实现 Java 序列化和反序列化？
<a name="WJULP"></a>
### 是什么
一句话就能够说明白什么是 Java 序列化和反序列化？**「Java 序列化是将 Java 对象转换为字节序列的过程，而 Java 反序列化则是将字节序列恢复为 Java 对象的过程。」**

- **「序列化」**：任何需要保存到磁盘或者在网络进行传输的 Java 对象都需要支持序列化，序列化后的字节流保存了 Java 对象的状态及相关的描述信息，反序列化能够根据这些信息“复刻”出一个一模一样的对象。序列化的核心作用就是对象状态的保存。
- **「反序列化」**：反序列化就是根据磁盘中保存的或者网络上传输的字节流中所保存的对象状态和相关描述信息，通过反序列化重建对象。

所以，从本质上来说，序列化就是将对象的状态和相关描述信息按照一定的格式写入到字节流中，而反序列化则是从字节流中重建这个对象。
<a name="N2mMJ"></a>
### 为什么
为什么需要 Java 序列化和反序列化呢？有两个原因：

1. 持久化。即将该对象保存到磁盘中。一般来说是不需要持久化 Java 对象的，但是如果遇到特殊情况，需要将 Java 对象持久化到磁盘中，以便于在重启 JVM 时可以重建这些 Java 对象。所以可以通过序列化的方式将 Java 对象转换成字节流，然后将这些字节流保存到磁盘中实现持久化。在应用程序重启时，可以读取这些字节流进行反序列化还原 Java 对象。
2. 网络传输：都知道网络上传输的对象是二进制字节流，是无法传输一个 Java 对象给一个应用的，所以在传输前需要对 Java 对象进行序列化将其转换为字节流。而接收方则根据字节流中所包含的信息重建该 Java 对象。
<a name="hHbTS"></a>
### 怎么做？
在 Java 中，如果一个对象要想实现序列化，它有两种方式：

1. 实现 `Serializable` 接口
2. 实现 `Externalizable` 接口

这两个接口是如何工作的呢？又有什么区别呢？下面分别介绍。
<a name="sFvl7"></a>
## Java 如何实现序列化和反序列化
<a name="V9fp1"></a>
### Serializable 接口
Serializable 接口只是一个标记接口，不用实现任何方法。一个对象只要实现了该接口，就意味着该对象是可序列化的。
<a name="Td3li"></a>
### 序列化
Java 对象序列化的步骤如下：

1. 对象实现 `Serializable` 接口
2. 创建一个 `ObjectOutputStream` 输出流
3. 调用 `ObjectOutputStream` 对象的 `writeObject()` 输出可序列化对象

如下：
```java
@Data
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class Person implements Serializable {

    private String name;

    private Integer age;

    private Float height;
}

public class Serializable01 {
    public static void main(String[] args) throws Exception {
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("person01.txt"));
        Person person01 = new Person("张三",35,175.4F);
        oos.writeObject(person01);
    }
}
```
用 idea 打开 person01.txt 文件就可以得到如下内容：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699365019166-ae79a247-ecf1-4003-8eab-6f2e15e1c6c0.png#averageHue=%23f0f0ef&clientId=u6568b562-2913-4&from=paste&height=121&id=u0698cf9b&originHeight=303&originWidth=2770&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=56507&status=done&style=shadow&taskId=ua3913d5a-e824-4465-918b-088eebdcccc&title=&width=1108)<br />从这个文件中基本上可以看清楚 Person01 对象的字节流的轮廓。
<a name="T2vBo"></a>
### 反序列化
Java 反序列化步骤如下：

1. 对象实现 `Serializable` 接口
2. 创建一个 `ObjectInputStream` 对象
3. 调用 `ObjectInputStream` 对象的 `readObject()`
```java
ObjectInputStream ois = new ObjectInputStream(new FileInputStream("person01.txt"));
Person person011 = (Person01) ois.readObject();
System.out.println("person01.txt 反序列化内容：" + person011.toString());
```
运行结果
```
person01.txt 反序列化内容：Person01(name=张三, age=35, height=175.4)
```
反序列化生成的对象和序列化的对象内容一模一样，完全还原了序列化时的对象。
<a name="HhKEO"></a>
### 成员为引用的序列化
上面的例子 Person 的成员变量都是基本类型，如果成员变量为引用类型呢？<br />去掉 Person 类实现的 Serializable 接口，然后定义一个 Women 类。
```java
public class Person {

    private String name;

    private Integer age;

    private Float height;
}

public class Woman implements Serializable {

    private String hairColor;

    private Person person;
}
```
再来序列化 Woman 这类
```java
public class Serializable02 {

    public static void main(String[] args) throws Exception {
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("woman.txt"));

        Person person = new Person("李四",30,180F);
        Woman woman = new Woman("黄颜色",person);

        oos.writeObject(woman);
    }
}
```
执行时，会发现程序会抛出异常：
```
java.io.NotSerializableException: com.sike.javacore.serializer.serializable.dto.Person
...
```
**「所以，一个可序列化的类，如果它含有引用类型的成员变量，那么这个引用类型也必须是可序列化的。」**
<a name="izoHp"></a>
## 自定义序列化
有些时候并不需要将一个对象的所有属性全部序列化，这个时候可以使用 transient 关键字来选择不需要序列化的字段。<br />**「transient」** 的作用就是用来标识一个成员变量在序列化应该被忽略。
```java
public class Person_1 implements Serializable {
    private String name;

    // 标识为 transient
    private transient Integer age;

    private Float height;
}
```
将 age 属性标识为 transient。
```java
public class Serializable03 {
    public static void main(String[] args) throws Exception {
        // 先序列化
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("person_1.txt"));
        Person_1 person = new Person_1("王五",32,180F);
        oos.writeObject(person);
        System.out.println("原对象：" + person);

        // 再反序列化
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream("person_1.txt"));
        Person_1 person1 = (Person_1) ois.readObject();
        System.out.println("序列化后对象：" + person1);
    }
}
```
运行结果：
```
原对象：Person_1(name=王五, age=32, height=180.0)
序列化后对象：Person_1(name=王五, age=null, height=180.0)
```
从运行结果可以看出，用 transient 标识的属性，在进行序列化时会将该字段忽略，然后在反序列化的时候，被 transient 标识的属性会被设置为默认值。
<a name="MH0N8"></a>
### `Externalizable` 接口
一个类除了实现 `Serializable` 接口外来实现序列化，还有一种更加灵活的方式来实现序列化：**「实现 **`**Externalizable**`** 接口」**。<br />`Externalizable` 接口是 `Serializable` 的子类，它提供了 `writeExternal()` 和 `readExternal()` 方法让类能够更加灵活地实现序列化。
```java
public interface Externalizable extends java.io.Serializable {
    void writeExternal(ObjectOutput out) throws IOException;

    void readExternal(ObjectInput in) throws IOException, ClassNotFoundException;
}
```
一个类如果实现了 Externalizable 接口，即必须要实现 `writeExternal()` 和 `readExternal()` 两个方法。在这两个方法里面可以做自己任何想做的事情。
```java
public class Student implements Externalizable {
    private String name;
    private int age;
    private int grade;

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeObject(name);
        out.writeInt(age - 2);      // 年龄我虚报 2 岁
        // 成绩我不报了
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        this.name = (String) in.readObject();
        this.age = in.readInt();
    }
}

public class Serializable04 {
    public static void main(String[] args) throws Exception {
        // 先序列化
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("student.txt"));
        Student student = new Student("小明",15,55);
        oos.writeObject(student);
        System.out.println("序列化对象内容：" + student);

        ObjectInputStream ois = new ObjectInputStream(new FileInputStream("student.txt"));
        Student student1 = (Student) ois.readObject();
        System.out.println("序列化后的内容：" + student1);
    }
}
```
运行结果：
```
序列化对象内容：Student(name=小明, age=15, grade=55)
序列化后的内容：Student(name=小明, age=13, grade=0)
```
根据运行结果看到，`Externalizable` 接口可以实现自定义的序列化和反序列化。<br />但是使用 `Externalizable` 接口时要注意，`writeExternal()` 方法和 `readExternal()` 的顺序要一致，即 `writeExternal()` 是按照怎么样的顺序来 write 值的，`readExternal()` 就必须严格按照这个顺序来 read ，否则会报错。有兴趣的小伙伴可以 name 和 age 的顺序调整下，就知道了。
<a name="o8ZPZ"></a>
### `Serializable` 和 `Externalizable` 对比
| `serializable` | `Externalizable` |
| --- | --- |
| 系统自动存储 Java 对象必要的信息 | 程序员自己来实现 Java 对象的序列化，灵活度更加高 |
| 不需要的属性使用 transient 修饰 | 不需要的属性可以不写入对象 |
| 在反序列化的时候不走构造方法 | 反序列化时，先走无参构造方法得到一个空对象，在调用 readExternal() 方法来读取序列化文件中的内容给该空对象赋值 |

<a name="sn6H5"></a>
## serialVersionUID 版本号
先看一个例子。<br />先将 Student 对象序列化到本地磁盘 student.txt 文件中，然后在 Student 类里面增加一个字段，比如 className，用来表示所在的班级，然后再用刚刚已经序列化的 student.txt 来反序列化试图还原 Student 对象，这个时候会发现运行报错，抛出下面的异常：
```
Exception in thread "main" java.io.InvalidClassException: com.sike.javacore.serializer.serializable.dto.Student; local class incompatible: stream classdesc serialVersionUID = -1065600830313514941, local class serialVersionUID = 2126309100823681
```
异常信息说明：序列化前后的 serialVersionUID 不一致。一个是 serialVersionUID = -1065600830313514941，另外一个是 serialVersionUID = 2126309100823681。<br />为什么两个 serialVersionUID 会不一样呢？因为对 Student 类做了变更，即所谓的升级。<br />在实际开发中，Class 文件不可能一成不变，它是随着项目的升级，Class 文件也会 升级，但是不能因为升级了 Class 类就导致之前的序列化对象无法还原了，需要做到升级前后的兼容性。怎么保证呢？显示声明 serialVersionUID。<br />Java 序列化提供了一个 `private static final long serialVersionUID = xxxx` 的序列化版本号，只要版本号相同，就可以将原来的序列化对象还原。<br />类的序列化版本号 serialVersionUID 可以随意指定，如果不指定，则 JVM 会根据类信息自己生成一个版本号，但是这样就会无法保证类升级后的序列化了。同时，不指定版本号也不利于 JVM 间的移植，因为可能不同的 JVM 版本计算规则可能就不一样了，这样也会导致无法反序列化。所以，凡是实现 Serializable 接口的类，都需要显示声明一个 serialVersionUID 版本号。
<a name="wJm8v"></a>
## 缺点
说实在话，现在几乎不会有人使用 Java 原生的序列化了，有如下几个原因不得不嫌弃他。
<a name="tiUpy"></a>
### 无法跨语言
通过 Java 原生 Serializable 接口与 ObjectOutputStream 实现的序列化，只能通过 Java 语言自己的ObjectInputStream 来反序列化，其他语言，如 C、Python、Go 等等都无法对其进行反序列化，这不很坑么？<br />同时，跨平台支持也不是很好，客户端与服务端如果因为 JDK 的版本不同都有可能导致无法进行反序列化，这个就更加坑了。
<a name="R9P4y"></a>
### 序列化字节流太大
Java 序列化它需要将类的描述信息和属性进行序列化，如果不这样做，它根本无法还原，这就会导致序列化字节流变得很大。来做一个比较，一个是 Java 原生序列化，一个是通用的二进制编码。
```java
public class UserInfo implements Serializable {
    private static final long serialVersionUID = 1L;

    private Long id;

    private String userName;

    private String nickName;

    public byte[] codeC() {
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        byte[] userNameBytes = this.userName.getBytes();
        buffer.putInt(userNameBytes.length);
        buffer.put(userNameBytes);
        byte[] nickNameBytes = this.nickName.getBytes();
        buffer.putInt(nickNameBytes.length);
        buffer.put(nickNameBytes);
        buffer.putLong(this.id);
        buffer.flip();
        byte[] result = new byte[buffer.remaining()];
        buffer.get(result);
        return result;
    }
}
```
UserInfo 类有一个 `codeC()` 方法，该方法返回 UserInfo 的字节流。
```java
public class Serializable01 {
    public static void main(String[] args) throws Exception {
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("person01.txt"));
        Person person01 = new Person("张三",35,175.4F);
        oos.writeObject(person01);
        oos.close();

        ObjectInputStream ois = new ObjectInputStream(new FileInputStream("person01.txt"));
        Person person011 = (Person) ois.readObject();
        System.out.println("person01.txt 反序列化内容：" + person011.toString());
    }
}
```
运行结果：
```
原生 JDK 序列化生成的字节流大小：246
UserInfo 对象字节流大小：31
```
有 8 倍的差距，这差距还是有点儿大的。
<a name="zXwX6"></a>
### 序列化时间太长
还是上面那个类，把上面的程序改下：
```java
public class Serializable05 {
    public static void main(String[] args) throws Exception {
        UserInfo userInfo = new UserInfo(1001L,"zhangshan","张三");

        // 序列化
        long startTime = System.currentTimeMillis();
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        ObjectOutputStream out = new ObjectOutputStream(bout);
        out.writeObject(userInfo);
        out.flush();
        out.close();
        System.out.println("原生 JDK 序列化消耗时间：" + (System.currentTimeMillis() - startTime));
        bout.close();

        // 原生字节码
        startTime = System.currentTimeMillis();
        userInfo.codeC();
        System.out.println("UserInfo#codeC 消耗时间：" + (System.currentTimeMillis() - startTime));
    }
}
```
运行结果：
```
原生 JDK 序列化消耗时间：9
UserInfo#codeC 消耗时间：1
```
这差距依然很巨大啊！<br />所以，Java 原生序列化这么弱，也不得不嫌弃他！！！
<a name="cGpwh"></a>
## 总结
下面对 Java 序列化做一个总结。

1. 序列化的目的是为了将 Java 对象的状态持久化存储起来或者在网络上传输。
2. 对象的类名、实例变量（包括基本类型，数组，对其他对象的引用）都会被序列化；方法、类变量、transient实例变量都不会被序列化。
3. 如果要序列化的类中包含有引用类型的成员变量，那么该成员变量也需要支持序列化。
4. 反序列化时必须要有序列化对象的 Class 文件（这里埋坑了）。
5. 对于 Serializable 接口而言，它只是起到一个标识作用。实现了该接口就意味着该类支持序列化。
   - 如果不想要某个变量被序列化，使用 transient 修饰。
6. 对于 `Externalizable` 接口
   - `Externalizable` 接口是 `Serializable` 的子类，它提供了 `writeExternal()` 和 `readExternal()` 方法类实现自定义的序列化和反序列化。
   - `writeExternal()` 和 `readExternal()` 两个方法对属性的加工顺序要一致。
7. 建议所有实现了 Serializable 接口的类都显示申明 `serialVersionUID` 版本号。
