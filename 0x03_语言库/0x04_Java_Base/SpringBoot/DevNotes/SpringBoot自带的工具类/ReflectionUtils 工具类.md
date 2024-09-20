Java Spring<br />ReflectionUtils是Spring框架中的反射工具类，它提供了一系列静态方法，可以方便地进行类、对象、方法、字段等反射操作。
<a name="IvzZE"></a>
## `handleReflectionException`异常处理
源码：
```java
public static void handleReflectionException(Exception ex) {
    if (ex instanceof NoSuchMethodException) {
        throw new IllegalStateException("Method not found: " + ex.getMessage());
    }
    if (ex instanceof IllegalAccessException) {
        throw new IllegalStateException("Could not access method: " + ex.getMessage());
    }
    if (ex instanceof InvocationTargetException) {
        handleInvocationTargetException((InvocationTargetException) ex);
    }
    if (ex instanceof RuntimeException) {
        throw (RuntimeException) ex;
    }
    throw new UndeclaredThrowableException(ex);
}
```
主要是将反射中的异常分成几个部分，规范化输出

- `boolean declaresException(Method method, Class<?> exceptionType)`判断方法上是否声明了指定的异常类型
<a name="ggroT"></a>
## `findField`查找字段

- `Field findField(Class<?> clazz, String name, Class<?> type)`

查找指定类的指定名称和指定类型的方法
```java
public static Field findField(Class<?> clazz, String name, Class<?> type) {
    Class<?> searchType = clazz;
    while (Object.class != searchType && searchType != null) {
        Field[] fields = getDeclaredFields(searchType);
        for (Field field : fields) {
            if ((name == null || name.equals(field.getName())) &&
                (type == null || type.equals(field.getType()))) {
                return field;
            }
        }
        searchType = searchType.getSuperclass();
    }
    return null;
}
```
获取所有的方法，然后循环遍历，知道找到满足条件的返回 其中`getDeclaredFields(searchType)`方法使用`ConcurrentReferenceHashMap`将Field缓存，并优先从缓存中取。

- `Field findField(Class<?> clazz, String name)`
<a name="ZbRGx"></a>
## 设置字段`setField`

- `void setField(Field field, Object target, Object value)`设置指定字段的值 直接使用Field.set/get方法，然后格式化处理了异常
- `Object getField(Field field, Object target)`获取指定字段的值
<a name="reuC3"></a>
## 查找方法`findMethod`

- `Method findMethod(Class<?> clazz, String name, Class<?>... paramTypes)`查找方法，方法的参数是一个可变长的Class
- `Method findMethod(Class<?> clazz, String name)`直接查，不指定参数
<a name="dARGO"></a>
## 调用方法`invokeMethod`

- `Object invokeMethod(Method method, Object target, Object... args)`调用方法
- `Object invokeMethod(Method method, Object target)`简单版本
<a name="BN7TA"></a>
## 判断类

- `boolean declaresException(Method method, Class<?> exceptionType)`方法上是否声明了指定的异常
- `boolean isPublicStaticFinal(Field field)`判断字段首付是`public static final`的
- `boolean isEqualsMethod(Method method)`判断方法是否是`equals`方法
- `boolean isHashCodeMethod(Method method)`判断方法是否是`hashcode`方法
- `boolean isToStringMethod(Method method)`判断方法是否是`toString`方法
- `boolean isObjectMethod(Method method)`判断方法是否是`Object`类上的方法
<a name="WfRqL"></a>
## 操作

- `void makeAccessible(Field field)`使私有的字段可写
- `void makeAccessible(Method method)`私有方法可调用
- `void makeAccessible(Constructor<?> ctor)`私有构造器可调用
- `void doWithLocalMethods(Class<?> clazz, MethodCallback mc)`遍历类上的方法，并执行回调
```java
public interface MethodCallback {
    void doWith(Method method) throws IllegalArgumentException, IllegalAccessException;
}
```

- `void doWithMethods(Class<?> clazz, MethodCallback mc, MethodFilter mf)`增加了一个方法过滤器
```java
public interface MethodFilter {
    boolean matches(Method method);
}
```
<a name="BD8Cz"></a>
## 方法详解
<a name="MLBpV"></a>
### `shallowCopyFieldState(final Object src, final Object dest)`将源对象的属性值浅拷贝到目标对象中
该方法会遍历源对象的所有可读属性，并将其值拷贝到目标对象的对应属性中。拷贝的方式是通过直接赋值来实现的，因此是浅拷贝，即拷贝的是属性的引用而不是属性的副本。
```java
Person source = new Person();
source.setName("John");
source.setAge(25);
PersonVO target = new PersonVO();
// 浅拷贝
ReflectionUtils.shallowCopyFieldState(source, target);        
System.out.println("Name: " + target.getName());
System.out.println("Age: " + target.getAge());
```
运行结果：
```
Name: John
Age: 25
```
<a name="kYsv0"></a>
### `handleReflectionException(Exception ex)`用于处理反射操作中可能抛出的异常
它接受一个Exception类型的参数ex，表示要处理的异常。
```java
Person example = new Person();
try {
    Method method = Person.class.getMethod("nonExistingMethod");
    ReflectionUtils.invokeMethod(method, example);
} catch (Exception ex) {
    // 处理反射操作中的异常
    ReflectionUtils.handleReflectionException(ex);
}
```
运行结果：
```
Exception in thread "main" java.lang.IllegalStateException: Method not found: com.cloudfish.test.Person.nonExistingMethod()
    at org.springframework.util.ReflectionUtils.handleReflectionException(ReflectionUtils.java:109)
    at com.cloudfish.test.ReflectionUtilsTest.main(ReflectionUtilsTest.java:26)
```
在示例中，调用了一个不存在的方法，抛异常了。
<a name="Hik0W"></a>
### `handleInvocationTargetException(InvocationTargetException ex)`处理给定的调用目标异常
`InvocationTargetException`是Java反射中的一个异常类，它表示在调用方法时发生了异常。通常情况下，可以使用`getCause()`方法获取原始的异常对象。
<a name="wvXqn"></a>
### `rethrowRuntimeException(Throwable ex)`重新抛出一个运行时异常
<a name="QIvox"></a>
### `rethrowException(Throwable ex)`重新抛出一个异常
重新抛出的异常分为三种类型：运行时异常`RuntimeException`、`Error`、`UndeclaredThrowableException`。
<a name="LM4hH"></a>
### `ReflectionUtils.accessibleConstructor(Classclazz, Class<?>... parameterTypes)`用于获取指定类的可访问构造函数
```java
try {
    Constructor<Person> constructor = ReflectionUtils.accessibleConstructor(Person.class, String.class, int.class);
    Person example = constructor.newInstance("John Doe", 25);
    System.out.println("Name: " + example.getName());
    System.out.println("Age: " + example.getAge());
} catch (Exception ex) {
    ex.printStackTrace();
}
```
运行结果：
```
Name: John Doe
Age: 25
```
<a name="N666Y"></a>
### `Method findMethod(Class<?> clazz, String name)`获取指定类的指定方法
```java
Method getNameMethod = ReflectionUtils.findMethod(Person.class, "getName");
System.out.println("getName method: " + getNameMethod);
```
运行结果：
```
setName method: public java.lang.String com.cloudfish.test.Person.getName()
```
注意：该方法只能获取没有参数的方法。
<a name="grcsK"></a>
### `findMethod(Class clazz, String name, @Nullable Class... paramTypes)`获取指定类的指定方法，该方法还有一个或多个参数类型
```java
Method setNameMethod = ReflectionUtils.findMethod(Person.class, "setName", String.class);
System.out.println("setName method: " + setNameMethod);
```
运行结果：
```
setName method: public void com.cloudfish.test.Person.setName(java.lang.String)
```
<a name="NskuG"></a>
### `invokeMethod(Method method, @Nullable Object target)`用于调用指定方法，并传入目标对象（可选）
```java
Method setNameMethod = ReflectionUtils.findMethod(Person.class, "setName", String.class);
System.out.println("setName method: " + setNameMethod);        

Person example = new Person();
// 设置访问权限才可访问
setNameMethod.setAccessible(true);
ReflectionUtils.invokeMethod(setNameMethod, example, "eeefdfdfd");          
System.out.println(example.getName());
```
运行结果：
```
setName method: public void com.cloudfish.test.Person.setName(java.lang.String)
eeefdfdfd
```
<a name="qDZi4"></a>
### `declaresException(Method method, Class<?> exceptionType)`用于检查指定方法是否声明了指定的异常类型
```java
Method method = ReflectionUtils.findMethod(Person.class, "getName");
boolean declaresIOException = ReflectionUtils.declaresException(method, IOException.class);
System.out.println("Declares IOException: " + declaresIOException);

boolean declaresNullPointerException = ReflectionUtils.declaresException(method, NullPointerException.class);
System.out.println("Declares NullPointerException: " + declaresNullPointerException);
```
运行结果：
```
Declares IOException: false
Declares NullPointerException: false
```
<a name="Dog2H"></a>
### `doWithLocalMethods(Class<?> clazz, MethodCallback mc)`用于迭代指定类的所有本地方法，并对每个方法执行回调操作
```java
ReflectionUtils.doWithLocalMethods(Person.class, new PersonCallback());
```
回调类：
```java
// 需要实现MethodCallback 方法
class PersonCallback implements MethodCallback {
    @Override
    public void doWith(Method method) throws IllegalArgumentException, IllegalAccessException {
        System.out.println("Method name: " + method.getName());
    }
}
```
运行结果：
```
Method name: hashCode
Method name: getAddress
Method name: getName
Method name: setName
Method name: setAddress
Method name: getAge
Method name: setAge
```
打印了person中所有的方法。
<a name="AXNj8"></a>
### `doWithMethods(Class<?> clazz, MethodCallback mc)`用于迭代指定类的所有方法，并对每个方法执行回调操作
```java
ReflectionUtils.doWithMethods(Person.class, new PersonCallback());
```
运行结果：
```
Method name: hashCode
Method name: getAddress
Method name: getName
Method name: setName
Method name: setAge
Method name: getAge
Method name: setAddress
Method name: finalize
Method name: wait
Method name: wait
Method name: wait
Method name: equals
Method name: toString
Method name: hashCode
Method name: getClass
Method name: clone
Method name: notify
Method name: notifyAll
Method name: registerNatives
```
<a name="MoIlj"></a>
### `doWithMethods(Class<?> clazz, MethodCallback mc, @Nullable MethodFilter mf)`用于迭代指定类的所有方法，并对每个方法执行回调操作
```java
ReflectionUtils.doWithMethods(Person.class, new PersonCallback(), new PersonMethodFilter());

// 需要实现MethodFilter接口
class PersonMethodFilter implements MethodFilter {

    @Override
    public boolean matches(Method method) {
        return Modifier.isPublic(method.getModifiers());
    }    
}
```
运行结果：
```
Method name: hashCode
Method name: getAddress
Method name: getName
Method name: setName
Method name: setAge
Method name: setAddress
Method name: getAge
Method name: wait
Method name: wait
Method name: wait
Method name: equals
Method name: toString
Method name: hashCode
Method name: getClass
Method name: notify
Method name: notifyAll
```
<a name="odW46"></a>
### `getAllDeclaredMethods(Class<?> leafClass)`用于获取指定类及其所有父类中声明的所有方法
```java
Class<?> leafClass = PersonVO.class;
Method[] methods = ReflectionUtils.getAllDeclaredMethods(leafClass);
for (Method method : methods) {
    System.out.println("Method name: " + method.getName());
}
```
运行结果：
```
Method name: getHobbies
Method name: setHobbies
Method name: hashCode
Method name: getAddress
Method name: getName
Method name: setName
Method name: setAddress
Method name: getAge
Method name: setAge
Method name: finalize
Method name: wait
Method name: wait
Method name: wait
Method name: equals
Method name: toString
Method name: hashCode
Method name: getClass
Method name: clone
Method name: notify
Method name: notifyAll
Method name: registerNatives
```
<a name="FrcOf"></a>
### `getUniqueDeclaredMethods(Class<?> leafClass)`用于获取指定类及其所有父类中声明的所有唯一方法
```java
Class<?> leafClass = PersonVO.class;
Method[] methods = ReflectionUtils.getUniqueDeclaredMethods(leafClass);
for (Method method : methods) {
    System.out.println("Method name: " + method.getName());
}
```
运行结果：
```
Method name: getHobbies
Method name: setHobbies
Method name: hashCode
Method name: getAddress
Method name: getName
Method name: setName
Method name: setAge
Method name: getAge
Method name: setAddress
Method name: finalize
Method name: wait
Method name: wait
Method name: wait
Method name: equals
Method name: toString
Method name: getClass
Method name: clone
Method name: notify
Method name: notifyAll
Method name: registerNatives
```
<a name="HnfZX"></a>
### `isEqualsMethod(@Nullable Method method)`用于判断给定的方法是否是equals方法
```java
try {
    Class<?> clazz = PersonVO.class;
    Method method = clazz.getMethod("equals", Object.class);
    boolean isEquals = ReflectionUtils.isEqualsMethod(method);
    System.out.println("Is equals method: " + isEquals);
} catch (NoSuchMethodException | SecurityException e) {
    e.printStackTrace();
}
```
运行结果：
```
Is equals method: true
```
<a name="EGmm7"></a>
### `isHashCodeMethod(@Nullable Method method)`用于判断给定的方法是否是`hashCode`方法
<a name="sCzvB"></a>
### `isToStringMethod(@Nullable Method method)`用于判断给定的方法是否是`toString`方法
<a name="tYWVm"></a>
### `isObjectMethod(@Nullable Method method)`用于判断给定的方法是否是Object类中的方法
```java
try {
    Class<?> clazz = PersonVO.class;
    Method method = clazz.getMethod("getName");    
    boolean isObjectMethod = ReflectionUtils.isObjectMethod(method);
    System.out.println("Is Object method: " + isObjectMethod);
} catch (NoSuchMethodException | SecurityException e) {
    e.printStackTrace();
}
```
运行结果：
```
Is Object method: false
```
<a name="AZEMB"></a>
### `isCglibRenamedMethod(Method renamedMethod)`用于判断给定的方法是否是Cglib的类中的renamed方法
<a name="jD47F"></a>
### `findField(Class<?> clazz, String name)`用于在给定的类及其父类中查找指定名称的字段
```java
Class<?> clazz = PersonVO.class;
Field field = ReflectionUtils.findField(clazz, "name");
System.out.println("Field name: " + field.getName());
```
运行结果：
```
Field name: name
```
<a name="xa6xl"></a>
### `findField(Class clazz, @Nullable String name, @Nullable Class type)`用于在给定的类及其父类中查找指定名称、类型的字段
查找指定类的指定名称和指定类型的方法
```java
public static Field findField(Class<?> clazz, String name, Class<?> type) {
    Class<?> searchType = clazz;
    while (Object.class != searchType && searchType != null) {
        Field[] fields = getDeclaredFields(searchType);
        for (Field field : fields) {
            if ((name == null || name.equals(field.getName())) &&
                (type == null || type.equals(field.getType()))) {
                return field;
            }
        }
        searchType = searchType.getSuperclass();
    }
    return null;
}
```
获取所有的方法，然后循环遍历，知道找到满足条件的返回 其中`getDeclaredFields(searchType)`方法使用`ConcurrentReferenceHashMap`将Field缓存，并优先从缓存中取。
<a name="xxILu"></a>
### `setField(Field field, @Nullable Object target, @Nullable Object value)`用于设置指定字段的值
```java
try {
    Person myObject = new Person();
    Field field = Person.class.getDeclaredField("name");
    // 设置为可以访问
    field.setAccessible(true);
    ReflectionUtils.setField(field, myObject, "Hello, World!");
    System.out.println("Field value: " + myObject.getName());
} catch (NoSuchFieldException | SecurityException e) {
    e.printStackTrace();
}
```
运行结果：
```
Field value: Hello, World!
```
<a name="LGR4w"></a>
### `getField(Field field, @Nullable Object target)`用于获取指定字段的值
```java
try {
    Person myObject = new Person("aaaa",10);
    Field field = Person.class.getDeclaredField("name");
    field.setAccessible(true);
    Object value = ReflectionUtils.getField(field, myObject);
    System.out.println("Field value: " + value);
} catch (NoSuchFieldException | SecurityException e) {
    e.printStackTrace();
}
```
运行结果：
```
Field value: aaaa
```
<a name="NSl4S"></a>
### `doWithLocalFields(Class<?> clazz, FieldCallback fc)`用于对指定类的本地字段（即不包括父类的字段）进行操作
```java
Class<?> clazz = PersonVO.class;
MyFieldCallback fieldCallback = new MyFieldCallback();
ReflectionUtils.doWithLocalFields(clazz, fieldCallback);
//FieldCallback 类
class MyFieldCallback implements FieldCallback {
    @Override
    public void doWith(Field field) throws IllegalArgumentException, IllegalAccessException {
        System.out.println("Field name: " + field.getName());
        System.out.println("Field type: " + field.getType());
    }
}
```
运行结果：
```
Field name: hobbies
Field type: interface java.util.List
```
<a name="D0Bi7"></a>
### `doWithFields(Class<?> clazz, FieldCallback fc)`用于对指定类的字段（包括父类的字段）进行操作
```java
Class<?> clazz = PersonVO.class;
MyFieldCallback fieldCallback = new MyFieldCallback();
ReflectionUtils.doWithFields(clazz, fieldCallback);
```
运行结果：
```
Field name: hobbies
Field type: interface java.util.List
Field name: name
Field type: class java.lang.String
Field name: age
Field type: int
Field name: address
Field type: class java.lang.String
```
<a name="zXN3G"></a>
### `doWithFields(Class<?> clazz, FieldCallback fc, @Nullable FieldFilter ff)`用于对指定类的符合条件的字段进行操作
```java
Person myObject = new Person();
// 使用doWithFields方法对字段进行操作
ReflectionUtils.doWithFields(Person.class, new FieldCallback() {
    @Override
    public void doWith(Field field) throws IllegalArgumentException, IllegalAccessException {
        // 在这里可以对每个字段进行自定义操作
        field.setAccessible(true); // 设置字段可访问，因为字段是私有的
        Object value = field.get(myObject); // 获取字段的值
        System.out.println("Field name: " + field.getName());
        System.out.println("Field type: " + field.getType());
        System.out.println("Field value: " + value);
    }
}, new FieldFilter() {
    @Override
    public boolean matches(Field field) {
        // 在这里可以定义过滤条件，只处理满足条件的字段
        return field.getType() == String.class; // 只处理类型为String的字段
    }
});
```
使用匿名内部类实现了`FieldCallback`接口，并在`doWith`方法中定义了对每个字段的操作。在`doWith`方法中，首先将字段设置为可访问状态，然后使用`field.get(myObject)`方法获取字段的值，并打印字段的名称、类型和值。同时，还使用匿名内部类实现了`FieldFilter`接口，并在`matches`方法中定义了过滤条件。在这个示例中，只处理类型为String的字段。运行结果：
```
Field name: name
Field type: class java.lang.String
Field value: null
Field name: address
Field type: class java.lang.String
Field value: null
```
<a name="W0UC0"></a>
### `isPublicStaticFinal(Field field)`用于判断一个字段是否是`public`、`static`和`final`修饰的
```java
Field[] fields = Person.class.getDeclaredFields();
for (Field field : fields) {
    if (ReflectionUtils.isPublicStaticFinal(field)) {
        System.out.println(field.getName() + " is public, static, and final.");
    }
}
```
由于在person对象中，并没有这样的字段，所以没有打印结果。<br />**最后总结一下，**`**ReflectionUtils**`**工具类的作用：**

- 获取类的信息：`ReflectionUtils`可以通过类的全限定名获取对应的Class对象，进而获取类的各种信息，如类名、包名、父类、接口等。
- 创建对象：`ReflectionUtils`可以通过`Class`对象创建实例，即通过反射实现动态创建对象的功能。
- 调用方法：`ReflectionUtils`可以通过`Method`对象调用类的方法，包括无参方法和有参方法，可以通过方法名和参数类型来定位方法。
- 访问字段：`ReflectionUtils`可以通过`Field`对象访问类的字段，包括获取字段值和设置字段值。
- 调用构造方法：`ReflectionUtils`可以通过`Constructor`对象调用类的构造方法，包括无参构造方法和有参构造方法
