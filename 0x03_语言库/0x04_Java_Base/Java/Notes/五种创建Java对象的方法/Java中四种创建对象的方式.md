Java

- 调用new语句创建对象
- 调用对象的`clone()`方法
- 运用反射手段创建对象
- 运用反序列化手段
<a name="euUWl"></a>
### 调用`new`语句创建对象
```java
// 使用java语言的关键字 new 创建对象，初始化对象数据　
MyObject mo = new MyObject() ;　
```
<a name="uyW3K"></a>
### 调用对象的`clone()`方法
```java
MyObject anotherObject = new MyObject();
MyObject object = anotherObject.clone();
```
使用 `clone()`方法克隆一个对象的步骤：<br />1.被克隆的类要实现 Cloneable 接口。<br />2.被克隆的类要重写 `clone()`方法。<br />原型模式主要用于对象的复制，实现一个接口（实现 Cloneable 接口），重写一个方法（重写 Object 类中的 clone 方法），即完成了原型模式。<br />原型模式中的拷贝分为"浅拷贝"和"深拷贝":

- 浅拷贝: 对值类型的成员变量进行值的复制,对引用类型的成员变量只复制引用,不复制引用的对象.
- 深拷贝: 对值类型的成员变量进行值的复制,对引用类型的成员变量也进行引用对象的复制.

（Object 类的 clone 方法只会拷贝对象中的基本数据类型的值，对于数组、容器对象、引用对象等都不会拷贝，这就是浅拷贝。如果要实现深拷贝，必须将原型模式中的数组、容器对象、引用对象等另行拷贝。）
<a name="ivSSY"></a>
#### 原型模式的优点。

- 1.如果创建新的对象比较复杂时，可以利用原型模式简化对象的创建过程。
- 2.使用原型模式创建对象比直接 new 一个对象在性能上要好的多，因为Object 类的 clone 方法是一个本地方法，它直接操作内存中的二进制流，特别是复制大对象时，性能的差别非常明显。
<a name="zQwLh"></a>
#### 原型模式的使用场景。
因为以上优点，所以在需要重复地创建相似对象时可以考虑使用原型模式。<br />比如需要在一个循环体内创建对象，假如对象创建过程比较复杂或者循环次数很多的话，使用原型模式不但可以简化创建过程，而且可以使系统的整体性能提高很多。
<a name="OuL8N"></a>
### 运用反射手段创建对象
先介绍一下反射：
<a name="zonCA"></a>
#### 反射的定义

- 反射机制是在运行时， 对于任意一个类， 都能够知道这个类的所有属性和方法； 对于任意一个对象， 都能够调用它的任意一个方法。 在 java 中，只要给定类的名字， 那么就可以通过反射机制来获得类的所有信息。
- 反射机制主要提供了以下功能： 在运行时判定任意一个对象所属的类；在运行时创建对象； 在运行时判定任意一个类所具有的成员变量和方法； 在运行时调用任意一个对象的方法； 生成动态代理。
<a name="HHnSD"></a>
#### 哪里用到反射机制？
jdbc 中有一行代码：<br />`Class.forName('com.mysql.jdbc.Driver.class');//加载 MySql 的驱动类`。 这就是反射， 现在很多框架都用到反射机制， hibernate， struts 都是用反射机制实现的。
<a name="c9cAE"></a>
#### 反射的实现方式
在 Java 中实现反射最重要的一步， 也是第一步就是获取 Class 对象， 得到Class 对象后可以通过该对象调用相应的方法来获取该类中的属性、方法以及调用该类中的方法。<br />有 4 种方法可以得到 Class 对象：

- `Class.forName("类的路径");`
- 类名.class
- 对象名.getClass()
- 如果是基本类型的包装类， 则可以通过调用包装类的 Type 属性来获得该包装类的 Class 对象, Class clazz = Integer.TYPE;
<a name="yWWYH"></a>
#### 实现 Java 反射的类

- 1)`Class`： 它表示正在运行的 Java 应用程序中的类和接口。
- 2)`Field`： 提供有关类或接口的属性信息， 以及对它的动态访问权限。
- 3)`Constructor`： 提供关于类的单个构造方法的信息以及对它的访问权限
- 4)`Method`： 提供关于类或接口中某个方法信息。
:::danger
注意：Class类是Java反射中最重要的一个功能类，所有获取对象的信息(包括： 方法/属性/构造方法/访问权限)都需要它来实现。
:::
<a name="g4iBx"></a>
#### 反射机制的优缺点
优点：

- （1） 能够运行时动态获取类的实例， 大大提高程序的灵活性。
- （2） 与 Java 动态编译相结合， 可以实现无比强大的功能。

缺点：

- （1） 使用反射的性能较低。 java 反射是要解析字节码， 将内存中的对象进行解析。<br />解决方案：<br />1.由于 JDK 的安全检查耗时较多， 所以通过 `setAccessible(true)`的方式关闭安全检查来（取消对访问控制修饰符的检查） 提升反射速度。<br />2.需要多次动态创建一个类的实例的时候， 有缓存的写法会比没有缓存要快很多:<br />3.ReflectASM 工具类 ， 通过字节码生成的方式加快反射速度。
- （2） 使用反射相对来说不安全， 破坏了类的封装性， 可以通过反射获取这个<br />类的私有方法和属性。
<a name="RySpa"></a>
### 运用反序列化手段
<a name="x5iQT"></a>
#### 序列化与反序列化
Java序列化是指把Java对象转换为字节序列的过程；而Java反序列化是指把字节序列恢复为Java对象的过程。
<a name="nUL1n"></a>
#### 为什么需要序列化与反序列化
当两个进程进行远程通信时，可以相互发送各种类型的数据，包括文本、图片、音频、视频等， 而这些数据都会以二进制序列的形式在网络上传送。那么当两个Java进程进行通信时，能否实现进程间的对象传送呢？答案是可以的。如何做到呢？这就需要Java序列化与反序列化了。换句话说，一方面，发送方需要把这个Java对象转换为字节序列，然后在网络上传送；另一方面，接收方需要从字节序列中恢复出Java对象。基本原理和网络通信是一致的，通过特殊的编码方式：写入数据将对象以及其内部数据编码，存在在数组或者文件里面然后发送到目的地后，在进行解码，读出数据，再使用该数据。<br />当明晰了为什么需要Java序列化和反序列化后，很自然地会想Java序列化的好处。其好处一是实现了数据的持久化，通过序列化可以把数据永久地保存到硬盘上（通常存放在文件里），二是，利用序列化实现远程通信，即在网络上传送对象的字节序列。
<a name="WfaDY"></a>
#### 对象序列化

- `java.io.ObjectOutputStream`代表对象输出流，它的`writeObject(Object obj)`方法可对参数指定的obj对象进行序列化，把得到的字节序列写到一个目标输出流中。只有实现了`Serializable`和`Externalizable`接口的类的对象才能被序列化。
- `java.io.ObjectInputStream`代表对象输入流，它的`readObject()`方法从一个源输入流中读取字节序列，再把它们反序列化为一个对象，并将其返回。
```java
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/*
 * 序列化流：把对象按照流一样的方式存入文本或者在网络中传输;  对象 ---> 流 ：ObjectOutputStream
 * 反序列化流：把文本文件中的流对象数据或者网络中的流对象数据还原成对象。   流---> 对象 :ObjectInputStream
 */
public class ObjectStreamDemo {
    public static void main(String[] args) throws IOException {
        // 序列化数据其实就是把对象写到文本文件
        //write();
        read();
    }

    private static void read() throws IOException {
        // 创建反序列化流对象
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(
                "a.txt"));
        // 读取,还原对象
        try {
            Person p = (Person) ois.readObject();
            System.out.println(p.toString());
        } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        ois.close();
    }

    private static void write() throws IOException {
        // 创建序列化流对象
        // public ObjectOutputStream(OutputStream out)
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(
                "a.txt"));
        // 创建对象
        Person p = new Person("java", 20);
        oos.writeObject(p);
        // 释放资源
        oos.close();
    }
}
```
```java
import java.io.Serializable;

/*
 * NotSerializableException为序列化异常，
 * 该类需要实现一个接口：Serializable序列化接口，该接口中并没有任何方法，仅仅作为标识。
 * 类似于此的没有方法的接口是标记接口
 * 
 * !!!每一次去修改该类的时候都会生成一个新的序列化标识的值！，需要重新新，重新读，这是基本方法。
 * 想办法来固定该类的标识ID，人为设定。这样即使再次修改类的内容，只要ID固定了就可以保证，在读取的时候一直是匹配的。
 * 增加 generated serial version ID,在类里面直接点击黄色即可，增加一个变化的ID值
 */

/*
 * 当有的成员变量不需要被序列化时：如何解决。
 * 方法使用transient关键字声明不需要序列化的成员变量
 */
public class Person implements Serializable{

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -9164765814868887767L;

    private String name;
    private transient int age;

    public Person() {
        super();
    }

    public Person(String name, int age) {
        super();
        this.name = name;
        this.age = age;
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

    @Override
    public String toString() {
        return "Person [name=" + name + ", age=" + age + "]";
    }

}
```
