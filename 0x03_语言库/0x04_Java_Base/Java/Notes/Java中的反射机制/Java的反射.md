Java
<a name="aEjjs"></a>
## 一、什么是反射？
Java实例运行中获取自身的Class信息，操作一个类的任意field和method（甚至private修饰的）。
<a name="zKjNG"></a>
## 二、如何获取Class对象
| 方式 | 作用 | 应用场景 |
| --- | --- | --- |
| `Class.forName("全限定名")` | 通过指定的字符串路径 | 多用于配置文件，将类名定义在配置文件中 |
| `类名.class` | 通过类名的属性class | 多用于参数的传递 |
| `对象.getClass()` | 通过对象的`getClass()` | 对象获取字节码 |

<a name="gXFQJ"></a>
## 三、基本使用
<a name="t1FCL"></a>
### 3.1创建对象
```java
// 无参构造函数
public class GetClassConstructorDemo1 {
    public static void main(String[] args) throws Exception {
        //获取类的构造方法对象，并通过运行构造方法获取对象
        Class<?> c1 = Class.forName("com.abc.clazz.Student");
        //获取无参数构造
        Constructor<?> constructor = c1.getConstructor();
        //运行构造方法，创建对象
        Object obj = constructor.newInstance();
        Student s = (Student) obj;
        System.out.println("s = " + s);//Student{name='null', age=0}
    }
}

// 有参构造函数
public class GetClassConstructorDemo2 {
    public static void main(String[] args) throws Exception {
        //获取类的构造方法对象，并通过运行构造方法获取对象
        Class<?> c1 = Class.forName("com.abc.clazz.Student");
        //获取有参数构造方法
        Constructor<?> c2 = c1.getConstructor(String.class, int.class);
        Object o2 = c2.newInstance("Mark", 12);
        Student s2 = (Student) o2;
        System.out.println("s2 = " + s2);//Student{name='Mark', age=12}
    }
}

// 最简单的方式
public class GetClassConstructorDemo3 {
    public static void main(String[] args) throws Exception {
        //获取类的构造方法对象，并通过运行构造方法获取对象
        Class<?> c1 = Class.forName("com.abc.clazz.Student");
        Object o3 = c1.newInstance();
        Student s3 = (Student) o3;
        System.out.println("s3 = " + s3);//Student{name='null', age=0}
    }
}
```
<a name="lzCqB"></a>
### 3.2执行成员方法
<a name="oLcGI"></a>
#### Class类中与Method相关方法
```java
1. Method[] getMethods()
    获取所有的public修饰的成员方法，包括父类中。
2. Method getMethod("方法名", 方法的参数类型... 类型) 
    根据方法名和参数类型获得一个方法对象，只能是获取public修饰的
```
<a name="ksgCK"></a>
#### Method类中关键方法
```java
Object invoke(Object obj, Object... args) 
```
返回值Object，表示调用方法后，该方法的返回值<br />根据参数args调用对象obj的该成员方法  <br />如果`obj=null`，则表示该方法是静态方法
```java
// 反射获取无参方法，并执行
public class GetClassMethodDemo1 {
    public static void main(String[] args) throws Exception {
        //反射获取目标类的字节码对象，并执行其成员方法
        Class<?> c1 = Class.forName("com.abc.clazz.Student");
        Object obj = c1.newInstance();

        //反射获取无参数方法：Student的study方法
        Method studyMethod = c1.getMethod("study");
        //执行study方法
        studyMethod.invoke(obj);
    }
}

// 反射获取有参方法，并执行
public class GetClassMethodDemo2 {
    public static void main(String[] args) throws Exception {
        //反射获取目标类的字节码对象，并执行其成员方法
        Class<?> c1 = Class.forName("com.abc.clazz.Student");
        Object obj = c1.newInstance();

        //反射获取有参数方法：Student的eat方法
        Method eatMethod = c1.getMethod("eat", String.class, double.class);
        //调用eat方法，传递实际参数
        eatMethod.invoke(obj, "西红柿炒鸡蛋", 9.9);
    }
}
```
<a name="WFSLM"></a>
## 小结
Java反射功能虽小，意义重大，它是各种容器实现的核心。因此，了解框架的底层机制有助于丰富自己的编程思维。
