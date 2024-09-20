Class
<a name="pRWnJ"></a>
## Class类
下面通过Class.forName来获取一个实例对象
```java
/**
 * 反射Demo
 *
 * @author: Fcant
 */
public class ReflectionDemo {
    public static void main(String[] args) throws ClassNotFoundException {
        // 通过反射获取类的Class对象
        Class c1 = Class.forName("com.moxi.interview.study.annotation.User");
        Class c2 = Class.forName("com.moxi.interview.study.annotation.User");
        Class c3 = Class.forName("com.moxi.interview.study.annotation.User");
        System.out.println(c1.hashCode());
        System.out.println(c2.hashCode());
        System.out.println(c3.hashCode());
    }
}
/**
 * 实体类：pojo，entity
 */
class User {
    private String name;
    private int id;
    private int age;
    public User() {
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
    @Override
    public String toString() {
        return "User{" +
                "name='" + name + '\'' +
                ", id=" + id +
                ", age=" + age +
                '}';
    }
}
```
上面通过反射获取了三个对象，输出对应对象的hashcode码，会发现
```java
1173230247
1173230247
1173230247
```
它们的hashcode码是一样的，这就说明了：

- 一个类在内存中只有一个Class对象<br />
- 一个类被加载后，类的整体结构都会被封装在Class对象中<br />

在Object类中定义了以下的方法，此方法将被所有子类继承
```java
public final Class getClass()
```
以上方法的返回值的类型是一个Class类，此类是Java反射的源头，实际上所谓反射从程序的运行结果来看也很好理解，即：可以通过对象反射求出类的名称。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605158899834-e752169f-aff1-4f00-ac72-7a91d32222e1.png#align=left&display=inline&height=281&originHeight=843&originWidth=1497&size=96914&status=done&style=shadow&width=499)<br />也就是说，通过对象来获取到它的Class，相当于逆过程<br />通过对照镜子可以得到的信息：某个类的属性，方法和构造器，某个类到底实现了那些接口。对于每个类而言，JRE都为其保留一个不变的Class类型对象，一个CLass对象包含了特定某个结构的有关信息

- Class本身也是一个类<br />
- Class对象只能由系统建立对象<br />
- 一个加载的类在JVM中只会有一个Class实例<br />
- 一个Class对象对应的是一个加载到JVM中的一个.class文件<br />
- 每个类的实例都会记得自己是由哪个Class实例所生成<br />
- 通过Class可以完整地得到一个类中所有被加载的结构<br />
- Class类是Reflection的根源，针对任何想动态加载、运行的类、唯有先获得相应的Class对象
<a name="oc9QK"></a>
## Class类常用的方法

- ClassforName(String name)：返回指定类name的Class对象<br />
- newInstance()：调用缺省构造函数，返回Class对象的一个实例<br />
- getName()：返回此Class对象所表示的实体（类，接口，数组或void）的名称<br />
- getSuperClass()：返回当前Class对象的父类Class对象<br />
- getinterfaces()：返回当前对象的接口<br />
- getClassLoader()：返回该类的类加载器<br />
- getConstructors()：返回一个包含某些Constructor对象的数组<br />
- getMethod(String name, Class… T)：返回一个Method对象，此对象的形参类型为paramsType<br />
- getDeclaredFields()：返回Field对象的一个数组<br />
<a name="zvnNu"></a>
## 获取对象实例的方法

- 若已知具体的类，通过类的class属性获取，该方法最为安全可靠，程序性能最高<br />
- Class clazz = Person.class;<br />
- 已知某个类的实例，调用该实例的getClass()方法获取Class对象<br />
- Class clazz = person.getClass()<br />
- 已经一个类的全类名，且该类在类路径下，可以通过Class类的静态方法forName()获取，HIA可能抛出ClassNotFoundException<br />
- Class clazz = Class.forName(“demo01.Sutdent”)<br />
- 内置数据类型可以直接通过 类名.Type<br />
- 还可以利用ClassLoader<br />
```java
/**
 * Class类创建的方式
 *
 * @author: Fcant
 */
class Person {
    public String name;
    public Person() {
    }
    public Person(String name) {
        this.name = name;
    }
    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                '}';
    }
}
class Student extends Person{
    public Student() {
        this.name = "学生";
    }
}
class Teacher extends Person {
    public Teacher() {
        this.name = "老师";
    }
}
public class ClassCreateDemo {
    public static void main(String[] args) throws ClassNotFoundException {
        Person person = new Student();
        System.out.println("这个人是：" + person.name);
        // 方式1：通过对象获得
        Class c1 = person.getClass();
        System.out.println("c1:" + c1.hashCode());
        //方式2：通过forName获得
        Class c2 = Class.forName("com.moxi.interview.study.annotation.Student");
        System.out.println("c2:" + c2.hashCode());
        // 方式3：通过类名获取（最为高效）
        Class c3 = Student.class;
        System.out.println("c3:" + c3.hashCode());
        // 方式4：基本内置类型的包装类，都有一个Type属性
        Class c4 = Integer.TYPE;
        System.out.println(c4.getName());
        // 方式5：获取父类类型
        Class c5 = c1.getSuperclass();
    }
}
```
<a name="AAAuF"></a>
## 哪些类型可以有Class对象
class：外部类，成员（成员内部类，静态内部类），局部内部类，匿名内部类<br />interface：接口<br />[]：数组<br />enum：枚举<br />annotation：注解@interface<br />primitive type：基本数据类型<br />void
```java
/**
 * 获取Class的方式
 *
 * @author: 轻狂书生FS
 */
public class GetClassDemo {
    public static void main(String[] args) {
        Class c1 = Object.class; // 类
        Class c2 = Comparable.class; // 接口
        Class c3 = String[].class; // 数组
        Class c4 = int[][].class; // 二维数组
        Class c5 = Override.class; // 注解
        Class c6 = ElementType.class; // 枚举
        Class c7 = Integer.class; // 基本数据类型
        Class c8 = void.class; // void，空数据类型
        Class c9 = Class.class; // Class
        System.out.println(c1);
        System.out.println(c2);
        System.out.println(c3);
        System.out.println(c4);
        System.out.println(c5);
        System.out.println(c6);
        System.out.println(c7);
        System.out.println(c8);
        System.out.println(c9);
    }
}
```
最后运行结果为：
```java
class java.lang.Object
interface java.lang.Comparable
class [Ljava.lang.String;
class [[I
interface java.lang.Override
class java.lang.annotation.ElementType
class java.lang.Integer
void
class java.lang.Class
```
同时需要注意，只要类型和维度一样，那就是同一个Class对象
```java
int [] a = new int[10];
int [] b = new int[10];
System.out.println(a.getClass().hashCode());
System.out.println(b.getClass().hashCode());
```
这两个的hashcode是一样的
<a name="v5CCy"></a>
## Java内存分析
java内存分为以下三部分

- 堆<br />
- 存放new的对象和数组<br />
- 可以被所有的线程共享，不会存放别的对象引用<br />
- 栈<br />
- 存放基本变量（会包含这个基本类型的具体数值）<br />
- 引用对象的变量（会存放这个引用在对堆里面的具体地址）<br />
- 方法区<br />
- 可以被所有线程共享<br />
- 包含了所有的class和static变量<br />
