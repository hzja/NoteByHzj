Java 反射机制
<a name="cVZmi"></a>
## 动态语言与静态语言
<a name="CIBoF"></a>
### 动态语言
动态语言是一类在运行时可以改变其结构的语言：例如新的函数，对象，甚至代码可以被引进，已有的函数可以被删除或是其它结构上的变化。通俗点说就是在运行时代码可以根据某些条件改变自身结构<br />主要的动态语言有：Object-c、C#、JavaScript、PHP、Python等
<a name="qPRN1"></a>
### 静态语言
与动态语言相比，运行时结构不可变的语言就是静态语言。例如Java、C、C++<br />Java不是动态语言，但是Java可以称为“准动态语言”。即Java有一定的动态性，可以利用反射机制来获取类似于动态语言的 特性，Java的动态性让编程的时候更加灵活。
<a name="ypfPp"></a>
## Java反射机制概述
<a name="OzZYf"></a>
### 什么是反射
Java Reflection：Java反射是Java被视为动态语言的关键，反射机制运行程序在执行期借助于Reflection API 去的任何类内部的信息，并能直接操作任意对象的内部属性及方法。
```java
Class c = Class.forName("java.lang.String")
```
在加载完类后，在堆内存的方法区就产生了一个Class类型的对象（一个类只有一个Class对象），这个对象就包含了完整的类的结构信息，可以通过这个对象看到类的结构，这个对象就像一面镜子，透过这个镜子看到类的结构，所以形象的称之为：反射<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605158522731-0fb89b24-d081-4706-bd59-1b4fd433f37e.png#height=148&id=xqApr&originHeight=444&originWidth=2050&originalType=binary&ratio=1&size=96330&status=done&style=shadow&width=683.3333333333334)
:::tips
tip：反射可以获取到private修饰的成员变量和方法
:::
<a name="PHYIx"></a>
### 反射的应用

- 在运行时判断任意一个对象所属类
- 在运行时构造任意一个类的对象
- 在运行时判断任意一个类所具有的成员变量和方法
- 在运行时获取泛型信息
- 在运行时调用任意一个对象的成员变量和方法
- 在运行时候处理注解
- 生成动态代理
<a name="m7Osy"></a>
### Java反射的优缺点
<a name="IvZUd"></a>
#### 1、优点
反射可以在不知道会运行哪一个类的情况下，获取到类的信息，创建对象以及操作对象。这其实很方便于拓展，所以反射会是框架设计的灵魂，因为框架在设计的时候，为了降低耦合度，肯定是需要考虑拓展等功能的，不能将类型写死，硬编码。<br />降低耦合度，变得很灵活，在运行时去确定类型，绑定对象，体现了多态功能。
<a name="YPCSi"></a>
#### 2、缺点
反射可以修改权限，比如上面访问到`private`这些方法和属性，这是会破坏封装性的，有安全隐患，有时候，还会破坏单例的设计。对性能有影响。使用反射基本上是一种解释操作，可以告诉JVM，希望做什么并且它满足什么样的要求，这类操作总是慢于直接执行相同的操作。也就是说new创建和对象，比反射性能更高。
<a name="AB3SL"></a>
### 反射相关的主要API

- `java.lang.Class`：代表一个类
- `java.lang.reflect.Method`：代表类的方法
- `java.lang.reflect.Field`：代表类的成员变量
- `java.lang.reflect.Constructor`：代表类的构造器
<a name="TYLo2"></a>
## 反射性能对比
编写代码来进行测试，使用反射的时候和不适用反射，在执行方法时的性能对比
```java
/**
 * 反射性能
 *
 * @author: Fcant
 */
public class ReflectionPerformance {
    /**
     * 普通方式调用
     */
    public static void test01() {
        User user = new User();
        long startTime = System.currentTimeMillis();
        for (int i = 0; i < 1000000000; i++) {
            user.getName();
        }
        long endTime = System.currentTimeMillis();
        System.out.println("普通方式执行10亿次getName的时间:" + (endTime - startTime) + " ms");
    }
    /**
     * 反射方式调用
     */
    public static void test02() throws Exception {
        Class clazz = Class.forName("com.moxi.interview.study.annotation.User");
        Method getName = clazz.getDeclaredMethod("getName", null);
        User user = (User) clazz.newInstance();
        long startTime = System.currentTimeMillis();
        for (int i = 0; i < 1000000000; i++) {
            getName.invoke(user, null);
        }
        long endTime = System.currentTimeMillis();
        System.out.println("反射方式执行10亿次getName的时间:" + (endTime - startTime) + " ms");
    }
    /**
     * 反射方式调用，关闭权限检查
     */
    public static void test03() throws Exception {
        Class clazz = Class.forName("com.moxi.interview.study.annotation.User");
        Method getName = clazz.getDeclaredMethod("getName", null);
        User user = (User) clazz.newInstance();
        long startTime = System.currentTimeMillis();
        getName.setAccessible(true);
        for (int i = 0; i < 1000000000; i++) {
            getName.invoke(user, null);
        }
        long endTime = System.currentTimeMillis();
        System.out.println("反射方式执行10亿次getName的时间:" + (endTime - startTime) + " ms");
    }
    public static void main(String[] args) throws Exception {
        test01();
        test02();
        test03();
    }
}
```
运行结果：
```
普通方式执行10亿次getName的时间:3 ms
反射方式执行10亿次getName的时间:2554 ms
反射方式执行10亿次getName的时间:1365 ms
```
上面分别是执行了 10亿次 getName的方法，从里面可以看出，通过直接实例化对象后，调用getName耗时最短，同时关闭了权限检查后的比不关闭能提高一倍的性能。
<a name="fuYBH"></a>
## 反射操作泛型
Java采用泛型擦除机制来引入泛型，Java中的泛型仅仅是给编译器Java才使用的，确保数据的安全性和免去强制类型转换的问题，但是一旦编译完成后，所有的泛型有关的类型全部被擦除<br />为了通过反射操作这些类型，Java新增了`ParameterizedType`，`GenericArrayType`，`TypeVariable`和`WildcardType`几种类型来代表不能被归一到Class类中的类型但是有何原始类型齐名的类型。

- `ParameterizedType`：表示一种参数化类型，比如`Collection`
- `GenericArrayType`：表示一种元素类型是参数化类型或者类型变量的数组类型
- `TypeVariable`：是各种类型变量的公共父接口
- `WildcardType`：代表一种通配符类型的表达式

下面通过代码来获取方法上的泛型，包括参数泛型，以及返回值泛型
```java
/**
 * 通过反射获取泛型
 *
 * @author: Fcant
 */
public class GenericityDemo {
    public void test01(Map<String, User> map, List<User> list) {
        System.out.println("test01");
    }
    public Map<String, User> test02() {
        System.out.println("test02");
        return null;
    }
    public static void main(String[] args) throws Exception{
        Method method = GenericityDemo.class.getMethod("test01", Map.class, List.class);
        // 获取所有的泛型，也就是参数泛型
        Type[] genericParameterTypes = method.getGenericParameterTypes();
        // 遍历打印全部泛型
        for (Type genericParameterType : genericParameterTypes) {
            System.out.println(" # " +genericParameterType);
            if(genericParameterType instanceof ParameterizedType) {
                Type[] actualTypeArguments = ((ParameterizedType) genericParameterType).getActualTypeArguments();
                for (Type actualTypeArgument : actualTypeArguments) {
                    System.out.println(actualTypeArgument);
                }
            }
        }
        // 获取返回值泛型
        Method method2 = GenericityDemo.class.getMethod("test02", null);
        Type returnGenericParameterTypes = method2.getGenericReturnType();
        // 遍历打印全部泛型
        if(returnGenericParameterTypes instanceof ParameterizedType) {
            Type[] actualTypeArguments = ((ParameterizedType) returnGenericParameterTypes).getActualTypeArguments();
            for (Type actualTypeArgument : actualTypeArguments) {
                System.out.println(actualTypeArgument);
            }
        }
    }
}
```
得到的结果
```
# java.util.Map<java.lang.String, com.moxi.interview.study.annotation.User>
class java.lang.String
class com.moxi.interview.study.annotation.User
 # java.util.List<com.moxi.interview.study.annotation.User>
class com.moxi.interview.study.annotation.User
###################
class java.lang.String
class com.moxi.interview.study.annotation.User
```
<a name="34ff01e0"></a>
### 反射操作注解
通过反射能够获取到 类、方法、字段等上的注解

- getAnnotation
- getAnnotations
<a name="yBLDG"></a>
### ORM对象关系映射
ORM即为：Object relationship Mapping，对象关系映射

- 类和表结构对应
- 属性和字段对应
- 对象和记录对应

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605163352498-1b047a5b-b726-4251-97c9-72f3be78090d.png#height=114&id=zxhOx&originHeight=343&originWidth=1172&originalType=binary&ratio=1&size=41856&status=done&style=shadow&width=390.6666666666667)<br />下面使用代码，模拟ORM框架的简单使用
```java
/**
 * ORMDemo
 *
 * @author: Fcant
 */
@TableKuang("db_student")
class Student2 {
    @FieldKuang(columnName = "db_id", type="int", length = 10)
    private int id;
    @FieldKuang(columnName = "db_age", type="int", length = 10)
    private int age;
    @FieldKuang(columnName = "db_name", type="varchar", length = 10)
    private String name;
    public Student2() {
    }
    public Student2(int id, int age, String name) {
        this.id = id;
        this.age = age;
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
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    @Override
    public String toString() {
        return "Student2{" +
                "id=" + id +
                ", age=" + age +
                ", name='" + name + '\'' +
                '}';
    }
}
/**
 * 自定义注解：类名的注解
 */
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@interface TableKuang {
    String value();
}
/**
 * 自定义注解：属性的注解
 */
@Target(ElementType.FIELD)
@Retention(RetentionPolicy.RUNTIME)
@interface FieldKuang {
    String columnName();
    String type();
    int length() default 0;
}
public class ORMDemo {
    public static void main(String[] args) throws Exception{
        // 获取Student 的 Class对象
        Class c1 = Class.forName("com.moxi.interview.study.annotation.Student2");
        // 通过反射，获取到全部注解
        Annotation [] annotations = c1.getAnnotations();
        for (Annotation annotation : annotations) {
            System.out.println(annotation);
        }
        // 获取注解的value值
        TableKuang tableKuang = (TableKuang)c1.getAnnotation(TableKuang.class);
        String value = tableKuang.value();
        System.out.println(value);
        // 获得类指定的注解
        Field f = c1.getDeclaredField("name");
        FieldKuang fieldKuang = f.getAnnotation(FieldKuang.class);
        System.out.println(fieldKuang.columnName());
        System.out.println(fieldKuang.type());
        System.out.println(fieldKuang.length());
    }
}
```
<a name="koPie"></a>
## 反射的具体使用
<a name="nFzE9"></a>
### 1、获取对象的包名以及类名
```java
package invocation;
public class MyInvocation {
    public static void main(String[] args) {
        getClassNameTest();
    }
    
    public static void getClassNameTest(){
        MyInvocation myInvocation = new MyInvocation();
        System.out.println("class: " + myInvocation.getClass());
        System.out.println("simpleName: " + myInvocation.getClass().getSimpleName());
        System.out.println("name: " + myInvocation.getClass().getName());
        System.out.println("package: " +
                "" + myInvocation.getClass().getPackage());
    }
}
```
运行结果：
```
class: class invocation.MyInvocation
simpleName: MyInvocation
name: invocation.MyInvocation
package: package invocation
```
由上面结果可以看到：

1. `getClass()`:打印会带着class+全类名    
2. `getClass().getSimpleName()`：只会打印出类名    
3. `getName()`：会打印全类名    
4. `getClass().getPackage()`:打印出package+包名

`getClass()`获取到的是一个对象，`getPackage()`也是。
<a name="W18k6"></a>
### 2、获取Class对象
在java中，一切皆对象。java中可以分为两种对象，实例对象和Class对象。这里说的获取Class对象，其实就是第二种，Class对象代表的是每个类在运行时的类型信息，指和类相关的信息。比如有一个`Student`类，用`Student student = new Student()`new一个对象出来，这个时候`Student`这个类的信息其实就是存放在一个对象中，这个对象就是**Class类的对象**，而student这个实例对象也会和**Class对象**关联起来。有三种方式可以获取一个类在运行时的Class对象，分别是

- `Class.forName("com.Student")`
- `student.getClass()`
- `Student.class`

实例代码如下：
```java
package invocation;
public class MyInvocation {
    public static void main(String[] args) {
        getClassTest();
    }
    public static void getClassTest(){
        Class<?> invocation1 = null;
        Class<?> invocation2 = null;
        Class<?> invocation3 = null;
        try {
            // 最常用的方法
            invocation1 = Class.forName("invocation.MyInvocation");
        }catch (Exception ex){
            ex.printStackTrace();
        }
        invocation2 = new MyInvocation().getClass();
        invocation3 = MyInvocation.class;
        System.out.println(invocation1);
        System.out.println(invocation2);
        System.out.println(invocation3);
    }
}
```
执行的结果如下,三个结果一样：
```
class invocation.MyInvocation
class invocation.MyInvocation
class invocation.MyInvocation
```
<a name="yJUZa"></a>
### 3、`getInstance()`获取指定类型的实例化对象
首先有一个Student类，后面都会沿用这个类，将不再重复。
```java
class Student{
    private int age;
    private String name;
    public Student() {
    }
    public Student(int age) {
        this.age = age;
    }
    public Student(String name) {
        this.name = name;
    }
    
    public Student(int age, String name) {
        this.age = age;
        this.name = name;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    @Override
    public String toString() {
        return "Student{" +
                "age=" + age +
                ", name='" + name + '\'' +
                '}';
    }
}
```
可以使用`getInstance()`方法构造出一个Student的对象：
```java
public static void getInstanceTest() {
    try {
        Class<?> stduentInvocation = Class.forName("invocation.Student");
        Student student = (Student) stduentInvocation.newInstance();
        student.setAge(9);
        student.setName("Hahs");
        System.out.println(student);
    }catch (Exception ex){
        ex.printStackTrace();
    }
}
```
输出结果如下：
```java
Student{age=9, name='Hahs'}
```
但是如果取消不写Student的无参构造方法呢？就会出现下面的报错：
```java
java.lang.InstantiationException: invocation.Student
 at java.lang.Class.newInstance(Class.java:427)
 at invocation.MyInvocation.getInstanceTest(MyInvocation.java:40)
 at invocation.MyInvocation.main(MyInvocation.java:8)
Caused by: java.lang.NoSuchMethodException: invocation.Student.<init>()
 at java.lang.Class.getConstructor0(Class.java:3082)
 at java.lang.Class.newInstance(Class.java:412)
 ... 2 more
```
这是因为重写了构造方法，而且是有参构造方法，如果不写构造方法，那么每个类都会默认有无参构造方法，重写了就不会有无参构造方法了，所以调用`newInstance()`的时候，会报没有这个方法的错误。值得注意的是,`newInstance()`是一个无参构造方法。
<a name="UpnRJ"></a>
### 4、通过构造函数对象实例化对象
除了`newInstance()`方法之外，其实还可以通过构造函数对象获取实例化对象，怎么理解？这里只构造函数对象，而不是构造函数，也就是构造函数其实就是一个对象，先获取构造函数对象，当然也可以使用来实例化对象。<br />可以先获取一个类的所有的构造方法，然后遍历输出：
```java
public static void testConstruct(){
    try {
        Class<?> stduentInvocation = Class.forName("invocation.Student");
        Constructor<?> cons[] = stduentInvocation.getConstructors();
        for(int i=0;i<cons.length;i++){
            System.out.println(cons[i]);
        }
    }catch (Exception ex){
        ex.printStackTrace();
    }
}
```
输出结果：
```java
public invocation.Student(int,java.lang.String)
public invocation.Student(java.lang.String)
public invocation.Student(int)
public invocation.Student()
```
取出一个构造函数可以获取到它的各种信息，包括参数，参数个数，类型等等：
```java
public static void constructGetInstance() {
    try {
        Class<?> stduentInvocation = Class.forName("invocation.Student");
        Constructor<?> cons[] = stduentInvocation.getConstructors();
        Constructor constructors = cons[0];
        System.out.println("name: " + constructors.getName());
        System.out.println("modifier: " + constructors.getModifiers());
        System.out.println("parameterCount: " + constructors.getParameterCount());
        System.out.println("构造参数类型如下：");
        for (int i = 0; i < constructors.getParameterTypes().length; i++) {
            System.out.println(constructors.getParameterTypes()[i].getName());
        }
    } catch (Exception ex) {
        ex.printStackTrace();
    }
}
```
输出结果,`modifier`是权限修饰符，1表示为`public`，可以知道获取到的构造函数是两个参数的，第一个是int，第二个是String类型，看来获取出来的顺序并不一定是书写代码的顺序。
```
name: invocation.Student
modifier: 1
parameterCount: 2
构造参数类型如下：
int
java.lang.String
```
既然可以获取到构造方法这个对象了，那么可不可以通过它去构造一个对象呢？<br />**答案肯定是可以！！！**下面用不同的构造函数来创建对象：
```java
public static void constructGetInstanceTest() {
    try {
        Class<?> stduentInvocation = Class.forName("invocation.Student");
        Constructor<?> cons[] = stduentInvocation.getConstructors();
        // 一共定义了4个构造器
        Student student1 = (Student) cons[0].newInstance(9,"Sam");
        Student student2 = (Student) cons[1].newInstance("Sam");
        Student student3 = (Student) cons[2].newInstance(9);
        Student student4 = (Student) cons[3].newInstance();
        System.out.println(student1);
        System.out.println(student2);
        System.out.println(student3);
        System.out.println(student4);
    } catch (Exception ex) {
        ex.printStackTrace();
    }
}
```
输出如下：
```java
Student{age=9, name='Sam'}
Student{age=0, name='Sam'}
Student{age=9, name='null'}
Student{age=0, name='null'}
```
构造器的顺序是必须一一对应的，要不会报一下的参数不匹配的错误：
```java
java.lang.IllegalArgumentException: argument type mismatch
 at sun.reflect.NativeConstructorAccessorImpl.newInstance0(Native Method)
 at sun.reflect.NativeConstructorAccessorImpl.newInstance(NativeConstructorAccessorImpl.java:62)
 at sun.reflect.DelegatingConstructorAccessorImpl.newInstance(DelegatingConstructorAccessorImpl.java:45)
 at java.lang.reflect.Constructor.newInstance(Constructor.java:423)
 at invocation.MyInvocation.constructGetInstanceTest(MyInvocation.java:85)
 at invocation.MyInvocation.main(MyInvocation.java:8)
```
<a name="pS7JT"></a>
### 5、获取类继承的接口
通过反射可以获取接口的方法，如果某个类实现了接口的方法，同样可以做到通过类名创建对象调用到接口的方法。<br />首先定义两个接口，一个`InSchool`:
```java
public interface InSchool {
    public void attendClasses();
}
```
一个`AtHome`:
```java
public interface AtHome {
    public void doHomeWork();
}
```
创建一个实现两个接口的类`Student.java`
```java
public class Student implements AtHome, InSchool {
    public void doHomeWork() {
        System.out.println("I am a student,I am doing homework at home");
    }
    public void attendClasses() {
        System.out.println("I am a student,I am attend class in school");
    }
}
```
测试代码如下：
```java
public class Test {
    public static void main(String[] args) throws Exception {
        Class<?> studentClass = Class.forName("invocation.Student");
        Class<?>[] interfaces = studentClass.getInterfaces();
        for (Class c : interfaces) {
            // 获取接口
            System.out.println(c);
            // 获取接口里面的方法
            Method[] methods = c.getMethods();
            // 遍历接口的方法
            for (Method method : methods) {
                // 通过反射创建对象
                Student student = (Student) studentClass.newInstance();
                // 通过反射调用方法
                method.invoke(student, null);
            }
        }
    }
}
```
结果如下：
```java
interface invocation.AtHome
I am a student,I am doing homework at home
interface invocation.InSchool
I am a student,I am attend class in school
```
可以看出其实可以获取到接口的数组，并且里面的顺序是继承的顺序，通过接口的**Class对象**，可以获取到接口的方法，然后通过方法反射调用实现类的方法，因为这是一个无参数的方法，所以只需要传null即可。
<a name="fNH74"></a>
### 6、获取父类相关信息
主要是使用`getSuperclass()`方法获取父类，当然也可以获取父类的方法，执行父类的方法,首先创建一个`Animal.java`:
```java
public class Animal {
    public void doSomething(){
        System.out.println("animal do something");
    }
}
```
`Dog.java`继承于`Animal.java`：
```java
public class Dog extends Animal{
    public void doSomething(){
        System.out.println("Dog do something");
    }
}
```
可以通过反射创建`Dog`对象，获取其父类`Animal`以及创建对象，当然也可以获取`Animal`的默认父类`Object`：
```java
public class Test {
    public static void main(String[] args) throws Exception {
        Class<?> dogClass = Class.forName("invocation02.Dog");
        System.out.println(dogClass);
        invoke(dogClass);
        Class<?> animalClass = dogClass.getSuperclass();
        System.out.println(animalClass);
        invoke(animalClass);
        Class<?> objectClass = animalClass.getSuperclass();
        System.out.println(objectClass);
        invoke(objectClass);
    }
    public static void invoke(Class<?> myClass) throws Exception {
        Method[] methods = myClass.getMethods();
        // 遍历接口的方法
        for (Method method : methods) {
            if (method.getName().equalsIgnoreCase("doSomething")) {
                // 通过反射调用方法
                method.invoke(myClass.newInstance(), null);
            }
        }
    }
}
```
输入如下：
```java
class invocation02.Dog
Dog do something
class invocation02.Animal
animal do something
class java.lang.Object
```
<a name="XFMbt"></a>
### 7、获取当前类的公有属性和私有属性以及更新
创建一个`Person.java`,里面有静态变量，非静态变量，以及`public`，`protected`,`private`不同修饰的属性。
```java
public class Person {
    public static String type ;
    private static String subType ;
    // 名字（公开）
    public String name;
    protected String gender;
    private String address;
    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                ", address='" + address + '\'' +
                '}';
    }
}
```
使用`getFields()`可以获取到public的属性，包括static属性，使用`getDeclaredFields()`可以获取所有声明的属性，不管是`public`，`protected`,`private`不同修饰的属性。<br />修改`public`属性,只需要`field.set(object，value)`即可，但是`private`属性不能直接set，否则会报以下的错误。
```
Exception in thread "main" java.lang.IllegalAccessException: Class invocation03.Tests can not access a member of class invocation03.Person with modifiers "private"
 at sun.reflect.Reflection.ensureMemberAccess(Reflection.java:102)
 at java.lang.reflect.AccessibleObject.slowCheckMemberAccess(AccessibleObject.java:296)
 at java.lang.reflect.AccessibleObject.checkAccess(AccessibleObject.java:288)
 at java.lang.reflect.Field.set(Field.java:761)
 at invocation03.Tests.main(Tests.java:21)
```
`private`默认是不允许外界操作其值的，这里可以使用`field.setAccessible(true);`，相当于打开了操作的权限。<br />那`static`的属性修改和非static的一样，但是怎么获取呢？如果是`public`修饰的，可以直接用类名获取到，如果是`private`修饰的，那么需要使用`filed.get(object)`,这个方法其实对上面说的所有的属性都可以的。测试代码如下
```java
public class Tests {
    public static void main(String[] args) throws Exception{
        Class<?> personClass = Class.forName("invocation03.Person");
        Field[] fields = personClass.getFields();
        // 获取公开的属性
        for(Field field:fields){
            System.out.println(field);
        }
        System.out.println("=================");
        // 获取所有声明的属性
        Field[] declaredFields = personClass.getDeclaredFields();
        for(Field field:declaredFields){
            System.out.println(field);
        }
        System.out.println("=================");
        Person person = (Person) personClass.newInstance();
        person.name = "Sam";
        System.out.println(person);
        // 修改public属性
        Field fieldName = personClass.getDeclaredField("name");
        fieldName.set(person,"Jone");
        // 修改private属性
        Field addressName = personClass.getDeclaredField("address");
        // 需要修改权限
        addressName.setAccessible(true);
        addressName.set(person,"东风路47号");
        System.out.println(person);
        // 修改static 静态public属性
        Field typeName = personClass.getDeclaredField("type");
        typeName.set(person,"人类");
        System.out.println(Person.type);
        // 修改静态 private属性
        Field subType = personClass.getDeclaredField("subType");
        subType.setAccessible(true);
        subType.set(person,"黄种人");
        System.out.println(subType.get(person));
    }
}
```
结果：
```java
public static java.lang.String invocation03.Person.type
public java.lang.String invocation03.Person.name
=================
public static java.lang.String invocation03.Person.type
private static java.lang.String invocation03.Person.subType
public java.lang.String invocation03.Person.name
protected java.lang.String invocation03.Person.gender
private java.lang.String invocation03.Person.address
=================
Person{name='Sam', address='null'}
Person{name='Jone' , address='东风路47号'}
人类
黄种人
```
从结果可以看出，不管是`public`，还是`protected`，`private`修饰的，都可以通过反射对其进行查询和修改，不管是静态变量还是非静态变量。`getDeclaredField()`可以获取到所有声明的属性，而`getFields()`则只能获取到`public`的属性。对于非public的属性，需要修改其权限才能访问和修改：`field.setAccessible(true)`。<br />获取属性值需要使用`field.get(object)`，值得注意的是：**每个属性，其本身就是对象**
<a name="el6LN"></a>
### 8、获取以及调用类的公有/私有方法
既然可以获取到公有属性和私有属性，那么执行公有方法和私有方法应该都不是什么问题？<br />先定义一个类，包含各种修饰符，以及是否包含参数，是否为静态方法，`Person.java`:
```java
public class Person {
    // 非静态公有无参数
    public void read(){
        System.out.println("reading...");
    }
    // 非静态公有无参数有返回
    public String getName(){
        return "Sam";
    }
    // 非静态公有带参数   
    public int readABookPercent(String name){
        System.out.println("read "+name);
        return 80;
    }
    // 私有有返回值
    private String getAddress(){
        return "东方路";
    }
    // 公有静态无参数无返回值
    public static void staticMethod(){
        System.out.println("static public method");
    }
    // 公有静态有参数
    public static void staticMethodWithArgs(String args){
        System.out.println("static public method:"+args);
    }
    // 私有静态方法
    private static void staticPrivateMethod(){
        System.out.println("static private method");
    }
}
```
首先获取里面所有的方法：
```java
public class Tests {
    public static void main(String[] args) throws Exception {
        Class<?> personClass = Class.forName("invocation03.Person");
        Method[] methods = personClass.getMethods();
        for (Method method : methods) {
            System.out.println(method);
        }
        System.out.println("=============================================");
        Method[] declaredMethods = personClass.getDeclaredMethods();
        for (Method method : declaredMethods) {
            System.out.println(method);
        }
    }
}
```
结果如下：
```java
publicjava.lang.String invocation03.Person.getName( )
public void invocation03.Person.read()
public int invocation03.Person.readABookPercent(java.lang.String)
public static void invocation03.Person.staticMethod()
public static void invocation03.Person.staticMethodwithArgs()
public final void java.lang.Object.wait(long, int) throws java.lang.InterruptedException
public final native void java.lang.Object.wait(long) throws java.lang.InterruptedException
public final void java.lang.Object.wait() throws java.lang.InterruptedException
public boolean java.lang.Object.equals(java.lang.Object)
public java.lang.String java.lang.Object.toString()
public native int java.lang.Object.hashCode()
public final native java.lang.Class java.lang.Object.getClass()
public final native void java.lang.Object.notify()
public final native void java.lang.Object.notifyAll()
=============================================
private java.lang.String invocation03.Person.getAddress()
public java.lang.String invocation03.Person.getName()
public void invocation03.Person.read()
public int invocation03.Person.readABookPercent(java.lang.String)
public static void invocation03.Person.staticMethod( )
public static void invocation03.Person.staticMethodwithArgs()
private static void invocation03.Person.staticPrivateMethod()
```
可以发现`getMethods()`确实可以获取所有的公有的方法，但是有一个问题，就是他会把父类的也获取到，也就是上面图片绿色框里面的，所有的类默认都继承了`Object`类，所以它把`Object`的那些方法都获取到了。而`getDeclaredMethods`确实可以获取到公有和私有的方法，不管是静态还是非静态，但是它是获取不到父类的方法的。<br />那如果想调用方法呢？先试试调用非静态方法：
```java
public class Tests {
    public static void main(String[] args) throws Exception {
        Class<?> personClass = Class.forName("invocation03.Person");
        Person person = (Person) personClass.newInstance();
        Method[] declaredMethods = personClass.getDeclaredMethods();
        for (Method method : declaredMethods) {
            if(method.getName().equalsIgnoreCase("read")){
                method.invoke(person,null);
                System.out.println("===================");
            }else if(method.getName().equalsIgnoreCase("getName")){
                System.out.println(method.invoke(person,null));
                System.out.println("===================");
            }else if(method.getName().equalsIgnoreCase("readABookPercent")){
                System.out.println(method.invoke(person,"Sam"));
                System.out.println("===================");
            }
        }
    }
}
```
结果如下，可以看出`method.invoke(person,null);`是调用无参数的方法，而`method.invoke(person,"Sam")`则是调用有参数的方法，要是有更多参数，也只需要在里面多加一个参数即可，返回值也同样可以获取到。
```java
read Sam
80 
===================
Sam
===================
reading...
===================
```
那么`private`方法呢？照着来试试
```java
public class Tests {
    public static void main(String[] args) throws Exception {
        Class<?> personClass = Class.forName("invocation03.Person");
        Person person = (Person) personClass.newInstance();
        Method[] declaredMethods = personClass.getDeclaredMethods();
        for (Method method : declaredMethods) {
            if(method.getName().equalsIgnoreCase("getAddress")){
                method.invoke(person,null);
            }
        }
    }
}
```
结果报错了：
```
Exception in thread "main" java.lang.IllegalAccessException: Class invocation03.Tests can not access a member of class invocation03.Person with modifiers "private"
 at sun.reflect.Reflection.ensureMemberAccess(Reflection.java:102)
 at java.lang.reflect.AccessibleObject.slowCheckMemberAccess(AccessibleObject.java:296)
 at java.lang.reflect.AccessibleObject.checkAccess(AccessibleObject.java:288)
 at java.lang.reflect.Method.invoke(Method.java:491)
 at invocation03.Tests.main(Tests.java:13)
```
一看就是没有权限，加上下面的代码就可以解决
```
method.setAccessible(true);
```
那么问题来了，上面说的都是非静态的，想要调用静态的方法。当然用上面的方法，对象也可以直接调用到类的方法的：
```java
static public method 
null
static public method:myArgs 
null
static private method
null
```
一点问题都没有，输出结果有几个null,因为这函数是无返回值的。<br />如果不想用遍历方法的方式，再去判断怎么办？能不能直接获取到想要的方法？那答案肯定是可以的。
```java
public class Tests {
    public static void main(String[] args) throws Exception {
        Class<?> personClass = Class.forName("invocation03.Person");
        Person person = (Person) personClass.newInstance();
        Method method = personClass.getMethod("readABookPercent", String.class);
        method.invoke(person, "唐诗三百首");
    }
}
```
结果和上面调用的完全一样，要是这个方法没有参数呢？那就给一个null就可以了。或者不给也可以。
```java
public class Tests {
    public static void main(String[] args) throws Exception {
        Class<?> personClass = Class.forName("invocation03.Person");
        Person person = (Person) personClass.newInstance();
        Method method = personClass.getMethod("getName",null);
        System.out.println(method.invoke(person));
    }
}
```

