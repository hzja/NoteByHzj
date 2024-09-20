Java 反射
<a name="wtoVY"></a>
## 一、概念
<a name="j8d91"></a>
### 1.1 概念
简单说，JAVA反射机制是指在运行态可直接操作任意类或对象的所有属性和方法的功能。
<a name="iB8sZ"></a>
### 1.2 反射的用途

- 在运行时获取任意对象所属的类
```java
Class<?> clazz = Class.forName(String className);
```

- 在运行时构造任意类的对象
```java
Object obj = clazz.newInstance();
```

- 在运行时获取任意类所具有的成员变量和方法
```java
field.set(Object obj, Object value);
field.get(Object obj);
```

- 在运行时调用任意对象的方法 （最常见的需求，尤其是当该方法是私有方法或者隐藏方法）
```java
method.invoke(Object obj, Object... args);
```
反射还可以获取类的其他信息，包含modifiers（下面会介绍），以及superclass，实现的interfaces等。<br />针对动态语言，大致认同的一个定义是：“程序运行时，允许改变程序结构或变量类型，这种语言称为动态语言”。反射机制在运行时只能调用methods或改变fields内容，却无法修改程序结构或变量类型。从这个观点看，Perl，Python，Ruby是动态语言，C++，Java，C#不是动态语言。
<a name="xA2GX"></a>
## 二、反射
<a name="gj7pU"></a>
### 2.1 核心类
`java.lang.Class`：代表类<br />`java.lang.reflect.Constructor`: 代表类的构造方法<br />`java.lang.reflect.Field`: 代表类的属性<br />`java.lang.reflect.Method`: 代表类的方法<br />`java.lang.reflect.Modifier`：代表类、方法、属性的描述修饰符。<br />其中Modifier取值范围如下：<br />`public`, `protected`, `private`, `abstract`, `static`, `final`, `transient`, `volatile`, `synchronized`, `native`, `strictfp`, `interface`。<br />`Constructor`， `Field`， `Method`这三个类都继承`AccessibleObject`，该对象有一个非常重要的方法`setAccessible(boolean flag)`, 借助该方法，能直接调用非`Public`的属性与方法。
<a name="cyQvX"></a>
### 2.2 核心方法
<a name="wZMLd"></a>
#### 1.成员属性(Field)：
```java
getFields()：获得类的public类型的属性。
getDeclaredFields()：获得类的所有属性。
getField(String name)
getDeclaredField(String name)：获取类的特定属性
```
<a name="FjHnt"></a>
#### 2.成员方法(Method)：
```java
getMethods()：获得类的public类型的方法。
getDeclaredMethods()：获得类的所有方法。
getMethod(String name, Class[] parameterTypes)：获得类的特定方法
getDeclaredMethod(String name, Class[] parameterTypes)：获得类的特定方法
```
<a name="LK67X"></a>
#### 3.构造方法(Constructor)：
```java
getConstructors()：获得类的public类型的构造方法。
getDeclaredConstructors()：获得类的所有构造方法。
getConstructor(Class[] parameterTypes)：获得类的特定构造方法
getDeclaredConstructor(Class[] params)；获得类的特定方法
```
<a name="QhG9S"></a>
### 2.3 深入Class类
Java所有的类都是继承于Oject类，其内声明了多个应该被所有Java类覆写的方法：`hashCode()`、`equals()`、`clone()`、`toString()`、`notify()`、`wait()`、`getClass()`等，其中`getClass`返回的便是一个Class类的对象。Class类也同样是继承Object类，拥有相应的方法。<br />Java程序在运行时，运行时系统对每一个对象都有一项类型标识，用于记录对象所属的类。虚拟机使用运行时类型来选择相应方法去执行，保存所有对象类型信息的类便是Class类。<br />Class类没有公共构造方法，Class对象是在加载类时由 Java 虚拟机以及通过调用`ClassLoader`的`defineClass` 方法自动构造的，因此不能显式地声明一个Class对象。<br />虚拟机为每种类型管理一个独一无二的Class对象。也就是说，每个类（型）都有一个Class对象。运行程序时，Java虚拟机(JVM)首先检查是否所要加载的类对应的Class对象是否已经加载。如果没有加载，JVM就会根据类名查找.class文件，并将其Class对象载入。<br />基本的 Java 类型（boolean、byte、char、short、int、long、float 和 double）和关键字 void 也都对应一个 Class 对象。每个数组属于被映射为 Class 对象的一个类，所有具有相同元素类型和维数的数组都共享该 Class 对象。一般某个类的Class对象被载入内存，它就用来创建这个类的所有对象。
<a name="HMNZX"></a>
## 三、用处
<a name="ekYkI"></a>
### 1.如何通过反射获取一个类？
`Class.forName(String className);` (最常用)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634557904956-9169a97f-cdb5-4268-9a8a-6f9cd7f478f1.webp#clientId=u326af541-c521-4&from=paste&id=ua0f9fb26&originHeight=592&originWidth=722&originalType=url&ratio=1&status=done&style=none&taskId=u0d8aa8d2-574a-492f-9971-f76fe870f3f)
<a name="v6cPx"></a>
### 2.如何调用私有类，或者类的私有方法或属性？

- 私有类：通过`getDeclaredConstructor`获取`constructor`，再调用`constructor.setAccessible(true);`
- 私有方法：通过`getDeclaredMethod`获取`method`，再调用`method.setAccessible(true);`
- 私有属性：通过`getDeclaredField`获取`field`，再调用`field.setAccessible(true);`
<a name="UjcvW"></a>
### 3.`@hide`标记是做什么的，反射能否调用`@hide`标记的类？
在Android的源码中，发现有很多被“`@hide`”标记的类，它的作用是使这个类或方法在生成SDK时不可见。那么应用程序便不可以直接调用。而反射机制可调用`@hide`标记的类或方法，如入无人之地，畅通无阻。
<a name="j9yru"></a>
### 4.如何通过反射调用内部类？
假设`com.reflect.Outer`类有一个内部类`inner`，调用方法如下：
```java
String className = "com.reflect.Outer$inner";
Class.forName(className);
```
<a name="MEgT2"></a>
## 五、反射用法
反射是为了在运行态能操作类和对象，接下来重点介绍如何反射使用。<br />对于正常方式来调用方法，往往只需要一行到两行代码，即可完成相应工作。而反射则显得比较繁琐，之所以繁琐仍然使用反射方式，是因为反射能干很多正常实例化对象的方式所无法做到的事。比如操作那些`private`的类、方法、属性，以及`@hide`标记过的类、方法、属性。<br />为了到达即能有反射的功效，同时调用方法简单易用，写了一个`ReflectUtils`类。对于方法调用，与正常对象的调用过程差不多。主要有以下4类需要用到反射的地方：

1. 调用类的静态方法
2. 调用类的非静态方法
3. set/get类的静态属性
4. set/get类的非静态属性
<a name="EKlk6"></a>
### 5.1 `ReflectUtils`类用法
调用流程一般为先获取类或对象，再调用相应方法。针对上述4种需求，用法分别如下：
<a name="j8WiO"></a>
#### 1. 调用类的静态方法<br />对于参数方法，只需把参数，紧跟方法名后面，可以跟不定长的参数个数。
```java
Class<?> clazz = ReflectUtils.getClazz("com.fcant.model.Outer"); //获取class
ReflectUtils.invokeStaticMethod(clazz, "outerStaticMethod");  //无参方法
ReflectUtils.invokeStaticMethod(clazz, "outerStaticMethod","fcant"); //有参数方法
```
<a name="QyDz1"></a>
#### 2. 调用类的非静态方法
```java
Object obj = ReflectUtils.newInstance("com.fcant.model.Outer");  //实例化对象
ReflectUtils.invokeMethod(obj, "outerMethod");  //无参方法
ReflectUtils.invokeMethod(obj, "outerMethod", "fcant"); //有参方法 **3. set/get类的静态属性** 

ReflectUtils.getStaticField(clazz, "outerStaticField"); //get操作
ReflectUtils.setStaticField(clazz, "outerStaticField", "new value"); //set操作
```
<a name="KoUi4"></a>
#### 3. set/get类的非静态属性
```java
ReflectUtils.getField(obj, "outerField");  //get操作
ReflectUtils.setField(obj, "outerField", "new value"); //set操作
```
如果只知道类名，需先查看该类的所有方法详细参数信息，可以通过调用`dumpClass(String className)` ，返回值是String，记录着所有构造函数，成员方法，属性值的信息。
<a name="HQDud"></a>
### 5.2 核心代码
关于`ReflectUtils`类，列表部分核心方法。<br />先定义一个`Outer`类，包名假设为com.fcant.model，对于该类，非`public`，构造方法，成员方法，属性都是private：
```java
class Outer {
    private String outerField = "outer Field";
    private static String outerStaticField = "outer static Field";

    private Outer(){
        System.out.println("I'am outer construction without args");
    }

    private Outer(String outerField){
        this.outerField = outerField;
        System.out.println("I'am outer construction with args "+ this.outerField);
    }

    private void outerMethod(){
        System.out.println("I'am outer method");
    }

    private void outerMethod(String param){
        System.out.println("I'am outer method with param "+param);
    }

    private static void outerStaticMethod(){
        System.out.println("I'am outer static method");
    }

    private static void outerStaticMethod(String param){
        System.out.println("I'am outer static method with param "+param);
    }
}
```
构造函数，获取类的实例化对象：
```java
/** 
 * 实例化获取类名对应的类 * 
 * @param clazz 类 
 * @param constructorArgs 构造函数的各个参数 
 * @return 实例化对象 
 */
public static Object newInstance(Class clazz, Object... constructorArgs) {
    if (clazz == null) {
        return null;
    }

    Object object = null;

    int argLen = constructorArgs == null ? 0 : constructorArgs.length;
    Class<?>[] parameterTypes = new Class[argLen];
    for (int i = 0; i < argLen; i++) {
        parameterTypes[i] = constructorArgs[i].getClass();
    }

    try {
        Constructor constructor = clazz.getDeclaredConstructor(parameterTypes);
        if (!constructor.isAccessible()) {
            constructor.setAccessible(true);
        }
        object = constructor.newInstance(constructorArgs);

    } catch (Exception e) {
        e.printStackTrace();
    }

    return object;
}

```
对象方法的反射调用如下：
```java
/** 
 * 反射调用方法 * 
 * @param object 反射调用的对象实例 
 * @param methodName 反射调用的对象方法名 
 * @param methodArgs 反射调用的对象方法的参数列表 
 * @return 反射调用执行的结果 
 */
public static Object invokeMethod(Object object, String methodName, Object... methodArgs) {
    if (object == null) {
        return null;
    }

    Object result = null;
    Class<?> clazz = object.getClass();
    try {
        Method method = clazz.getDeclaredMethod(methodName, obj2class(methodArgs));
        if (method != null) {
            if (!method.isAccessible()) {
                method.setAccessible(true);  //当私有方法时，设置可访问
            }
            result = method.invoke(object, methodArgs);
        }
    } catch (Exception e) {
        e.printStackTrace();
    }

    return result;

}
```
对象属性值的反射获取方法：
```java
/**
 * 反射调用，获取属性值 * 
 * @param object 操作对象 
 * @param fieldName 对象属性 
 * @return 属性值 
 */
public static Object getField(Object object, String fieldName) {
    if (object == null) {
        return null;
    }

    Object result = null;
    Class<?> clazz = object.getClass();
    try {
        Field field = clazz.getDeclaredField(fieldName);
        if (field != null) {
            if (!field.isAccessible()) {
                field.setAccessible(true);
            }
            result = field.get(object);
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
    return result;
}
```
类属性的反射设置过程：
```java
/** 
 * 反射调用，设置属性值 
 * @param clazz 操作类 
 * @param fieldName 属性名 
 * @param value 属性的新值 
 * @return 设置是否成功 
 */
public static boolean setStaticField(Class clazz, String fieldName, Object value) {
    if (clazz == null) {
        return false;
    }

    Object result = null;
    try {
        Field field = clazz.getDeclaredField(fieldName);
        if (field != null) {
            if (!field.isAccessible()) {
                field.setAccessible(true);
            }
            field.set(null, value);
        }
    } catch (Exception e) {
        e.printStackTrace();
        return false;
    }
    return true;
}
```
<a name="rzsv3"></a>
## 六、内部类的反射用法
对于内部类，这里比较复杂，而内部类又分`static`内部类与非`static`内部类，两者的反射方式还是有区别的。`static`内部类与非`static`内部类的反射调用，根本区别在于构造方法不一样。下面通过代码来告诉如何正确。
<a name="duJAQ"></a>
### 6.1 `static`与非`static`内部类的反射差异
先定义一个包含两个内部类的类：
```java
class Outer {
    /** 
     * 普通内部类 
     */
    class Inner {
        private String innerField = "inner Field";

        private Inner(){
            System.out.println("I'am Inner construction without args");
        }

        private Inner(String innerField){
            this.innerField = innerField;
            System.out.println("I'am Inner construction with args "+ this.innerField);
        }

        private void innerMethod(){
            System.out.println("I'am inner method");
        }
    }

    /** * 静态内部类 */
    static class StaticInner {

        private String innerField = "StaticInner Field";
        private static String innerStaticField = "StaticInner static Field";

        private StaticInner(){
            System.out.println("I'am StaticInner construction without args");
        }

        private StaticInner(String innerField){
            this.innerField = innerField;
            System.out.println("I'am StaticInner construction with args "+ this.innerField);
        }

        private void innerMethod(){
            System.out.println("I'am StaticInner method");
        }

        private static void innerStaticMethod(){
            System.out.println("I'am StaticInner static method");
        }
    }
}
```
对于上面两个内部类，如果直接实例化内部类，该怎么做，抛开`private`等权限不够的问题，应该是这样的：

- 静态内部类：`Outer.StaticInner sInner = new Outer.StaticInner();`
- 非静态内部类： `Outer.Inner inner = new Outer().new Inner();`

这种差异，在于内部类的构造方法不一样。可以通过下面的方法`dumpClass()`来比较。
```java
/** * 获取类的所有 构造函数，属性，方法 * * @param className 类名 * @return */
public static String dumpClass(String className) {
    StringBuffer sb = new StringBuffer();
    Class<?> clazz;
    try {
        clazz = Class.forName(className);
    } catch (ClassNotFoundException e) {
        e.printStackTrace();
        return "";
    }

    Constructor<?>[] cs = clazz.getDeclaredConstructors();
    sb.append("------ Constructor ------> ").append("\n");
    for (Constructor<?> c : cs) {
        sb.append(c.toString()).append("\n");
    }

    sb.append("------ Field ------>").append("\n");
    Field[] fs = clazz.getDeclaredFields();
    for (Field f : fs) {
        sb.append(f.toString()).append("\n");
        ;
    }
    sb.append("------ Method ------>").append("\n");
    Method[] ms = clazz.getDeclaredMethods();
    for (Method m : ms) {
        sb.append(m.toString()).append("\n");
    }
    return sb.toString();
}
```
通过`dumpClass()`，对比可以发现，

- `static`内部类的默认构造函数：`private void com.fcant.model.Outer$StaticInner.innerMethod()`
- 非`static`内部类的默认构造函数：`private com.fcant.model.Outer$Inner(com.fcant.model.Outer)`，多了一个参数com.fcant.model.Outer，也就是说非`static`内部类保持了外部类的引用。从属性也会发现多了一个`final`属性`final com.fcant.model.Outer com.fcant.model.Outer$Inner.this$0`，这正是用于存储外部类的属性值。

正是这差异，导致两者的反射调用过程中构造方法的使用不一样。另外内部类的类名使用采用`$`符号，来连接外部类与内部类，格式为`outer$inner`。
<a name="gaa99"></a>
### 6.2 `static`内部类的 `ReflectUtils`类用法
<a name="txUvf"></a>
#### 1. 调用类的静态方法（先获取类，再调用）
```java
Class<?> clazz = ReflectUtils.getClazz("com.fcant.model.Outer$StaticInner"); //获取class
ReflectUtils.invokeStaticMethod(clazz, "innerStaticMethod");  //无参方法
ReflectUtils.invokeStaticMethod(clazz, "innerStaticMethod","fcant"); //有参数方法
```
<a name="HgPjv"></a>
#### 2. 调用类的非静态方法（先获取对象，再调用）
```java
Object obj = ReflectUtils.newInstance("com.fcant.model.Outer$StaticInner");  //实例化对象
ReflectUtils.invokeMethod(obj, "innerMethod");  //无参方法
ReflectUtils.invokeMethod(obj, "innerMethod", "fcant"); //有参方法
```
<a name="D8dgv"></a>
#### 3. set/get类的静态属性（先获取类，再调用）
```java
ReflectUtils.getStaticField(clazz, "innerField"); //get操作
ReflectUtils.setStaticField(clazz, "innerField", "new value"); //set操作
```
<a name="LEh8A"></a>
#### 4. set/get类的非静态属性（先获取对象，再调用）
```java
ReflectUtils.getField(obj, "innerField");  //get操作
ReflectUtils.setField(obj, "innerField", "new value"); //set操作 ### 2.2 非static内部类的 `ReflectUtils`类用法 非static内部类，不能定义静态方法和静态属性，故操作只有两项：
```
<a name="utnUa"></a>
#### 5. 调用类的非静态方法（先获取对象，再调用）
```java
// 获取外部类实例，这是static内部类所不需要的，注意点
Object outObj = ReflectUtils.newInstance("com.fcant.model.Outer"); 
Object obj = ReflectUtils.newInstance("com.fcant.model.Outer$Inner"， outObj);  //实例化对象
ReflectUtils.invokeMethod(obj, "innerMethod");  //无参方法
ReflectUtils.invokeMethod(obj, "innerMethod", "fcant"); //有参方法
```
<a name="HFacD"></a>
#### 6. set/get类的非静态属性（先获取对象，再调用）
```java
ReflectUtils.getField(obj, "innerField");  //get操作
ReflectUtils.setField(obj, "innerField", "new value"); //set操作
```
