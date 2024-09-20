Java反射<br />反射 (Reflection) 是 Java 提供的一项较为高级的功能，它提供了一种动态的 API 可以获取任何 Java 类的包括属性、方法、构造器等信息。元素不必在 JVM 运行时进行确定，反射可以使得它们在运行时动态地进行创建或调用。<br />Java 的反射机制主要提供了以下功能：

- **在运行时判断任意一个对象所属的类；**
- **在运行时构造任意一个类的对象；**
- **在运行时判断任意一个类所具有的成员属性和方法；**
- **在运行时调用任意一个对象的方法；**
- **生成动态代理。**

**哪些地方会用到反射？**<br />面向开发，反射应用最广泛的是中间件和框架，比如：

- **JDBC的**`**Class.forName(driverClass)**`**加载驱动；**
- **Spring MVC 通过反射调用 controller 的方法，动态代理处理请求；**
- **Spring IOC 容器，在创建 Bean 实例时和依赖注入时的反射。**

面向安全研究，反射应用更多的是构造利用链触发 RCE，比如：

- **RMI 反序列化，反射调用远程方法；**
- **动态代理利用；**
- **通过**`**Class.forName()**`**获取类并反射调用方法绕过沙箱；**
- **反序列化中的利用链构造。**
<a name="uNsbj"></a>
## Class 类
可以直接`new`创建一个对象，以自定义 Person 类举个例子：
```java
Person person = new Person();
```
在运行上面语句的时候，首先 JVM 启动，代码会编译成一个 .class 文件，然后被类加载器`ClassLoader`加载进 JVM 的内存中，并为之创建一个 java.lang.Class 对象，Person 类会加载到方法区中，创建了的 Person 类的 Class 对象会到堆中。<br />理解 Java 的反射机制就是要理解 Class 类。在 Java 中，所有对象可分大致分为两种：Class 对象和实例对象。每个类的运行时的类型信息，用 Class 对象表示，又或者称之为字节码对象，它包含了与类相关的信息，而实例对象就是通过 Class 对象来创建的。<br />获取 Class 对象的方式：

1. **实例对象调用 Object 类的**`**getClass()**`**方法；**
2. **通过属性类名.class直接获取；**
3. **调用 Class 类的**`**forName()**`**方法；**
4. **使用类加载器 ClassLoader 的**`**getSystemClassLoader().loadClass()**`**方法。**
```java
package com.hhh.test;

import com.hhh.model.Person;

public class ReflectionTest {
    public static void main(String[] args) throws Exception {
        //1. 实例对象.getClass()
        Person person = new Person();
        Class clz1 = person.getClass();
        System.out.println(clz1);// class com.hhh.test.Person

        //2，类名.class属性
        System.out.println(Person.class);// class com.hhh.test.Person

        //3. Class.forName()
        Class clz2 = Class.forName("com.hhh.test.Person");
        System.out.println(clz2);// class com.hhh.test.Person
    }
}
```
打印输出的结果：类型 包名.类名<br />以上三种方式都能获取 Person 类的字节码对象，但同时也存在区别：

- **方法1需要创建一个实例对象才能获取类的信息；**
- **方法2则需要导入包否则无法通过编译；**
- **方法3只需传入类名的字符串，这个类名是类完整路径；**
- **方法4与其他方法不同的是，它不会执行类中的静态代码块。**

其中，来看方法3，`Class.forName` 有两个重载方法：
```java
public static Class<?> forName(String className)
public static Class<?> forName(String name, boolean initialize, ClassLoader loader)
```
参数解释：

- **className、name：类的完整路径名；**
- **initialize：如果为 true，则会在返回 Class 对象之前，对该类型做连接和初始化操作，即类加载机制的后两个操作，在初始化阶段，JVM 会按照源代码语句的先后顺序去执行类变量的赋值和静态代码块。从第一个重载方法的代码可知，initialize 默认为 true 需要初始化；**
- **loader：用选择的类加载器来请求这个类型，可以为 null。**

因此得知，`Class.forName`默认是需要初始化。再来看看方法4，`ClassLoader` 的`loadClass`也有两个重载方法：
```java
public Class<?> loadClass(String name)
protected Class<?> loadClass(String name, boolean resolve)
```
参数解释：

- **name：类的完整路径名；**
- **resolve：表示是否连接该类型。这里的连接内容，并设置默认会验证 .class 文件，为类的静态成员变量分配内存并初始化为默认值，以及类型常量池引用替换，而并不会对该类型执行初始化操作。从第一个重载方法可知，resolve 默认为 false 不连接。**

但容易令人造成疑惑的是，初始化时会执行哪些方法，执行的先后顺序是怎样的。可以来看个 demo。
```java
package com.hhh.model;

public class InitializationTest extends Test{

    {
        System.out.println("block of code");
    }

    static {
        System.out.println("static block code");
    }

    public InitializationTest(){
        super();
        System.out.println("constructor code");
    }
}

class Test{
    public Test(){
        System.out.println("Test");
    }
}
```
先看不同调用的方式的执行结果以及执行的先后顺序：

- `**InitializationTest it = new InitializationTest();**`**执行了**`**static{}**`**、**`**super()**`**、**`**{}**`**和构造方法；**
- `**Class.forName("com.hhh.model.InitializationTest");**`**只执行了**`**static{}**`**；**
- `**Class.forName("com.hhh.model.InitializationTest",false,InitializationTest.class.getClassLoader());**`**无输出结果；**
- `**ClassLoader.getSystemClassLoader().loadClass("com.hhh.model.InitializationTest");**`**无输出结果。**

综上来说，首先调用的是`static{}` ，接着是`super()`，然后是`{}`，最后是构造函数；而且`static{}`会在类的初始化的时候调用。<br />那么可以利用这种机制做一些坏坏的事情，当遇到以下情况的代码，如果方法中的 name 参数可控：
```java
public void example(String name) throws Exception {
    Class.forName(name);
}
```
可以写一个恶意类，将恶意代码写在静态代码块中，通过某种方式（比如 `URLClassLoader` 加载 .class 文件、`ClassLoader` 加载字节码等）加载类，等类初始化的时候调用执行。
```java
public class Evil{
    static {
        try {
            Runtime rt = Runtime.getRuntime();
            String[] cmd = {"touch", "/tmp/success"};
            Process p = runtime.exec(cmd);
            p.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="DoHYu"></a>
## 反射使用
以下分别列举说明在反射中获取与使用成员方法和属性等等常用的函数。
<a name="egPl7"></a>
### 通过反射获取成员方法并使用
反射中通过 Method 类描述方法。方法获取：

- `**public Method getMethod(String name, Class<?>... parameterTypes)**`**获取指定名称和参数类型的 public 成员方法，也包括父类的；**
- `**public Method getDeclaredMethod(String name, Class<?>... parameterTypes)**`**获取指定名称和参数类型的成员方法，该声明可以为 **`**public**`**、**`**private**`**、**`**protected**`**。对于父类方法，只能获取权限声明为 **`**public**`** 的。**

方法调用：

- `**public Object invoke(Object obj, Object... args)**`**调用用包装在当前 Method 对象中的方法，其中从第二个参数开始分别方法参数，而第一个参数obj是隐式参数：**
   - **如果该方法是普通方法，那么 obj 为该类的实例对象；**
   - **如果该方法是静态方法，那么 obj 可以为 null，表示可被忽略。**
<a name="ivKfp"></a>
### 通过反射获取成员属性并使用
反射中通过 Field 类描述成员属性。获取成员属性：

- `**public Field getField(String name)**`**获取指定名称的 public 成员属性，但不包括在父类声明的属性；**
- `**public Field getDeclaredField(String name)**`**获取指定名称的成员属性，该声明可以为 **`**public**`**、**`**private**`**、**`**protected**`**，但不包括在父类声明的属性。**

使用成员属性：

- `**public Object get(Object obj)**`**获取该字段的属性值。此外，对于属性值为基本数据类型，还可以使用**`**getInt**`**、**`**getDouble**`**等方法获取；**
- `**public void set(Object obj, Object value)**`**给该字段属性赋值。**
<a name="efuZa"></a>
### 通过反射获取构造方法并使用
反射中通过 Constructor 类描述构造方法：

- `**public Constructor<T> getConstructor(Class<?>... parameterTypes)**`**获取该类的 public 构造器，参数为该类构造方法中的参数的类型字节码对象；**
- `**public Constructor<T> getDeclaredConstructor(Class<?>... parameterTypes)**`**获取该类的 **`**public**`** 构造器，该声明可以为public、private、protected。**

使用构造器创建实例对象：

- `**public T newInstance(Object ... initargs)**`**动态地创建一个类的实例。该方法会返回一个泛型，比如**`**person.getClass().newInstance();**`**创建了一个与 person 对象相同类型的实例。**

`newInstance`方法是工厂设计模式，它使用类加载机制，在类完成加载和连接后，调用默认的构造方法（也就是无参构造方法）初始化新创建的对象。如果这个类没有默认的构造方法，就会抛出一个 InstantiationException 异常。<br />但是，还有其他情况，无法使用`newInstance`创建实例对象，那就是单例。<br />单例也是一种设计模式，最常见的命令执行使用的 Runtime 就是一个单例。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658235096131-d8532aa3-cdce-45e2-8a35-aa0704dfbeba.png#clientId=ubd167643-232f-4&from=paste&id=u42f6a9c4&originHeight=603&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6109daca-dded-48ac-ab41-e3ba59264b1&title=)<br />可见其构造方法是私有的，外部无法直接访问，需要通过`getRuntime`才能获取其实例对象。<br />那么其实很好理解，就是`getRuntime`作为类方法代替了`newInstance`方法获取实例对象。
```java
Class clz = Class.forName("java.lang.Runtime");
Object rt = clz.getMethod("getRuntime").invoke(null);
clz.getMethod("exec", String.class).invoke(rt, "calc.exe");
```
<a name="ikzLA"></a>
### `setAccessible`
从以上列举的方法可知，当需要获取 `private`、`protected` 权限的方法或属性等，都需要使用 getDeclared 形式的方法，但是直接调用`get`、`set`和`invoke`操作的话会抛出java.lang.IllegalAccessException异常，这时需要在获取之后和使用之前设置`setAccessible(true)`。<br />跟踪这个方法，进入到 `AccessibleObject` 类，来看看它的继承关系。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658235096133-7cc93456-861e-478d-9978-bc767ee87adb.png#clientId=ubd167643-232f-4&from=paste&id=ua50803d0&originHeight=224&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u41a5267d-c209-40be-a8a6-edc30f4f268&title=)<br />也就是说 `AccessibleObject` 类是 Method、Field、Constructor 类的基类。它提供反射对象可绕过 Java 语言权限控制检查的权限。<br />当 Method、Field、Constructor 被用来 get、set 对象域、invoke 调用方法或者 newinstance 产生初始化对象实例的时候会进行权限检查 (public、default (package)、protected、 private)。<br />将反射对象中的 accessible 标志位设置为 true，那么反射对象在使用的时候，不会去检查 Java 语言权限控制 (如 private)，这就意味着允许客户端拥有超级权限，比如 Java 对象序列化或者其他持久化机制等通常禁止的机制。<br />除了以上获取类的指定方法、构造方法、属性字段，还能通过返回数组的形式获取类的全部方法和属性，比如`getMethods()`、`getConstructors()`、`getFields()`等，这里就不一一介绍，需要自行学习了解。<br />最后，继续用 Person 类作为例子将以上内容串联起来。
```java
package com.hhh.test;

import java.lang.Class;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class ReflectionTest {
    public static void main(String[] args) throws Exception {
        Class clz = Class.forName("com.hhh.model.Person");
        Constructor constructor = clz.getConstructor();
        Object person = constructor.newInstance();

        Field nameField = clz.getField("name");
        System.out.println("name : " + nameField.get(person));// name : Tony
        Field ageField = clz.getDeclaredField("age");
        ageField.setAccessible(true);
        System.out.println("age : " + ageField.get(person));//age : 18
        ageField.set(person, 28);
        System.out.println("age : " + ageField.get(person));//age : 28

        Method helloMethod = clz.getMethod("hello", String.class);
        helloMethod.invoke(person, "Mike");//Hello Mike
        Method introduceMethod = clz.getDeclaredMethod("introduce");
        introduceMethod.setAccessible(true);
        introduceMethod.invoke(person);//I am Tony and 28 years old.
    }
}
```
```java
package com.hhh.model;

public class Person {

    public String name = "Tony";
    private int age = 18;

    public Person() {
    }

    public void hello(String name) {
        System.out.println("Hello " + name);
    }

    private void introduce() {
        System.out.println("I am " + this.name + " and " + this.age + " years old.");
    }
}
```
