Java
<a name="xGhb3"></a>
## Javassist介绍
ASM 是在指令层次上操作字节码的，直观感受是在指令层次上操作字节码的框架实现起来比较晦涩。于是乎，退而求其次，再简单介绍另外一类框架：强调源代码层次操作字节码的框架Javassist。<br />Javassist（Java Programming Assistant）是一个用于在运行时操作字节码的 Java 库，它允许开发人员动态生成、修改和分析 Java 类的字节码。Javassist提供了一种更高级别的 API，以 Java 代码的方式来操作字节码，而不需要直接操作复杂的字节码指令。这使得动态代码生成和修改变得更加容易和可维护。以下是 Javassist 的一些重要特点和使用方式：

1. **「动态代码生成：」** Javassist 允许在运行时通过编写 Java 代码的方式来生成新的类和方法。这种方式使动态代码生成变得非常直观和易于理解。
2. **「类修改和增强：」** 可以使用 Javassist 修改已经存在的类的字节码，例如添加、修改或删除字段、方法等。
3. **「字节码操作：」** Javassist 提供了一套 API，用于操作类的字节码指令，如创建方法、添加指令、修改参数等。这使得开发者能够以更高级别的抽象方式来进行字节码操作，而无需深入了解底层字节码细节。
4. **「AOP 支持：」** Javassist 可以用于实现 AOP（面向切面编程）的功能，通过在方法前后插入代码来实现横切关注点的处理。
5. **「简化反射：」** Javassist 可以避免使用繁琐的 Java 反射 API，通过生成字节码来直接调用方法和访问字段。
6. **「跨版本支持：」** Javassist 支持处理不同版本的 Java 字节码，因此可以在不同的 Java 版本间进行字节码操作。
<a name="y3ryl"></a>
## Javassist操作步骤
使用 Javassist 的基本步骤如下：

1. **「导入库：」** 首先，需要将 Javassist 库添加到项目的依赖中。
2. **「创建 ClassPool：」** ClassPool 是 Javassist 的核心类，用于加载和保存类文件。可以通过 ClassPool 加载要操作的类。从开发视角来看，ClassPool是一张保存CtClass信息的HashTable，key为类名，value为类名对应的CtClass对象。当需要对某个类进行修改时，就是通过pool.getCtClass(“className”)方法从pool中获取到相应的CtClass。
3. **「创建 CtClass：」** 使用 ClassPool 创建一个 CtClass 对象，该对象表示要操作的类。它是一个class文件在代码中的抽象表现形式，可以通过一个类的全限定名来获取一个CtClass对象，用来表示这个类文件。
4. **「进行修改：」** 在 CtClass 对象上进行修改，例如添加新方法、修改字段、添加注解等。
   - `CtMethod`、`CtField`：这两个比较好理解，对应的是类中的方法和属性。
5. **「保存修改：」** 将修改后的 `CtClass` 对象保存到类文件或加载到 ClassLoader 中。
6. **「使用生成的类：」** 修改后的类现在可以被实例化和使用，从而实现所需的功能。

下面写一个小Demo来展示Javassist简单、快速的特点。依然是对MathUtils中的`add()`方法做增强，在方法调用前后分别输出”start”和”end”，实现代码如下。需要做的就是从pool中获取到相应的`CtClass`对象和其中的方法，然后执行`method.insertBefore`和`insertAfter`方法，参数为要插入的Java代码，再以字符串的形式传入即可，实现起来也极为简单。<br />首先引用jar包：
```xml
<dependency>  
  <groupId>org.javassist</groupId>  
  <artifactId>javassist</artifactId>  
  <version>3.28.0-GA</version>  
</dependency>
```
下面编写一个简单的测试类。
```java
public class MathUtils {  
    public int add(int a, int b) {  
        return a + b;  
    }  
}
```
```java
public class JavassistTest {  
    public static void main(String[] args) throws NotFoundException, CannotCompileException, IllegalAccessException, InstantiationException, IOException {  
        ClassPool cp = ClassPool.getDefault();  
        CtClass cc = cp.get("com.demo.bytecode.MathUtils");  
        CtMethod m = cc.getDeclaredMethod("add");  
        m.insertBefore("{ System.out.println(\"start\"); }");  
        m.insertAfter("{ System.out.println(\"end\"); }");  
        Class c = cc.toClass();  
        cc.writeFile("/Users/xx/work/spring-demo/");  
        MathUtils h = (MathUtils) c.newInstance();  
        int result = h.add(1, 2);  
        System.out.println(result);  
    }  
}
```
运行后输出结果为：
```
start
end
3
```
打开生成的class文件，可以看到已经生成了相关代码：
```java
public class MathUtils {
    public MathUtils() {
    }
    public int add(int a, int b) {
        System.out.println("start");
        int var4 = a + b;
        System.out.println("end");
        return var4;
    }
}

```
`ClassPool`需要关注的方法：

1. `getDefault`：返回默认的`ClassPool` 是单例模式的，一般通过该方法创建ClassPool；
2. `appendClassPath`，`insertClassPath `：将一个`ClassPath`加到类搜索路径的末尾位置 或 插入到起始位置。通常通过该方法写入额外的类搜索路径，以解决多个类加载器环境中找不到类的尴尬；
3. toClass：将修改后的CtClass加载至当前线程的上下文类加载器中，CtClass的toClass方法是通过调用本方法实现。**「需要注意的是一旦调用该方法，则无法继续修改已经被加载的class」**；
4. get，getCtClass：根据类路径名获取该类的CtClass对象，用于后续的编辑。

`CtClass`需要关注的方法：

1. `freeze`：冻结一个类，使其不可修改；
2. `isFrozen`：判断一个类是否已被冻结；
3. `prune`：删除类不必要的属性，以减少内存占用。调用该方法后，许多方法无法将无法正常使用，慎用；
4. `defrost`：解冻一个类，使其可以被修改。如果事先知道一个类会被defrost， 则禁止调用 `prune` 方法；
5. `detach`：将该class从ClassPool中删除；
6. `writeFile`：根据CtClass生成 .class 文件；
7. `toClass`：通过类加载器加载该CtClass。

上面创建一个新的方法使用了`CtMethod`类。`CtMthod`代表类中的某个方法，可以通过CtClass提供的API获取或者CtNewMethod新建，通过CtMethod对象可以实现对方法的修改。<br />`CtMethod`中的一些重要方法：

1. `insertBefore`：在方法的起始位置插入代码；
2. `insterAfter`：在方法的所有 return 语句前插入代码以确保语句能够被执行，除非遇到exception；
3. `insertAt`：在指定的位置插入代码；
4. `setBody`：将方法的内容设置为要写入的代码，当方法被 `abstract`修饰时，该修饰符被移除；
5. `make`：创建一个新的方法。
<a name="pyr48"></a>
## 使用Javassist写个Bean Copy的工具
首先定义一个对象转换的接口，生成的转换类实现这个接口
```java
public interface Converter {  
    /**  
    * 将一个对象复制到另一个对象  
    *  
    * @param from from  
    * @param to to  
    */  
    void convert(Object from, Object to);  
}
```
实现一个工具类CopyUtil生成转换器。
```java
public class CopyUtil {  

    private static final ConcurrentHashMap<ConverterKey, Converter> CACHE = new ConcurrentHashMap<>(32);  
    private static final AtomicInteger ID = new AtomicInteger();  
    private static final ClassPool pool = ClassPool.getDefault();  
    private static final CtClass converterInterface;  

    static {  
        try {  
            converterInterface = pool.getCtClass(Converter.class.getName());  
        } catch (NotFoundException e) {  
            throw new RuntimeException(e);  
        }  
    }  

    public static void copy(Object from, Object to) {  
        Class<?> fromClass = from.getClass();  
        Class<?> toClass = to.getClass();  

        Converter converter = getConverter(fromClass, toClass);  
        converter.convert(from, to);  
    }  

    /**  
    * 从缓存获取converter  
    *  
    * @param fromClass 源类  
    * @param toClass 目标类  
    * @return 转换器  
    */  
    private static Converter getConverter(Class<?> fromClass, Class<?> toClass) {  
        ConverterKey key = new ConverterKey(fromClass, toClass);  
        return CACHE.computeIfAbsent(key, CopyUtil::generateConverter);  
    }  

    /**  
    * 使用javassist生成一个转换器  
    *  
    * @param key key  
    * @return converter  
    */  
    private static Converter generateConverter(ConverterKey key) {  

        Class<?> fromClass = key.fromClass;  
        Class<?> toClass = key.toClass;  

        CtClass converterClass = pool.makeClass("BeanConverter" + ID.getAndIncrement());  

        try {  
            converterClass.addInterface(converterInterface);  
            // 创建一个新的方法  
            CtMethod convertMethod = CtNewMethod.make(generateMethod(fromClass, toClass), converterClass);  
            converterClass.addMethod(convertMethod);  

            Class<?> type = converterClass.toClass(CopyUtil.class.getClassLoader(), CopyUtil.class.getProtectionDomain());  
            return (Converter) type.newInstance();  
        } catch (Exception e) {  
            throw new RuntimeException("- generate converter error", e);  
        }  
    }  

    /**  
    * 生成转换器方法  
    *  
    * @param fromClass 原始类  
    * @param toClass 目标类  
    * @return 方法代码  
    */  
    private static String generateMethod(Class<?> fromClass, Class<?> toClass) {  
        String prefix = "public void convert(Object from, Object to) {\n";  
        // 对象转换  
        String castFromCode = fromClass.getName() + " a = (" + fromClass.getName() + ") from;\n";  
        String castToCode = toClass.getName() + " b = (" + toClass.getName() + ") to;\n";  
        String postfix = "}\n";  
        // 获取原始类字段  
        Set<String> fromFields = getFields(fromClass);  
        // 获取目标类字段  
        Set<String> toFields = getFields(toClass);  

        fromFields.retainAll(toFields);  

        StringBuilder code = new StringBuilder();  
        for (String field : fromFields) {  
            field = StringUtils.capitalize(field);  
            code.append("b.set").append(field).append("(a.get").append(field).append("());\n");  
        }  

            return prefix + castFromCode + castToCode + code + postfix;  
        }  

            /**  
            * 获取一个类（包含父类）的所有属性  
            *  
            * @param type type  
            * @return 属性list  
            */  
            private static Set<String> getFields(Class<?> type) {  

            Field[] fields = type.getDeclaredFields();  
            Set<String> fieldSet = Stream.of(fields).map(Field::getName).collect(toSet());  

            Class<?> parent = type.getSuperclass();  
            if (type.equals(Object.class) || parent.equals(Object.class)) {  
            return fieldSet;  
        }  

            Set<String> parentFieldSet = getFields(parent);  
            fieldSet.addAll(parentFieldSet);  

            return fieldSet;  
        }  

            /**  
            * 用于缓存的键  
            */  
            @EqualsAndHashCode  
            @AllArgsConstructor  
            private static class ConverterKey {  
            Class<?> fromClass;  
            Class<?> toClass;  
        }  
        }
```
下面试一下
```java
public static void main(String[] args) {  
    Person person1 = new Person("zhangsan", 25);  
    Person person2 = new Person();  
    CopyUtil.copy(person1, person2);  
    System.out.println(person2.getName());  
    System.out.println(person2.getAge());  
}
```
结果正常输出：
```
zhangsan
25
```
<a name="QNJaI"></a>
## 总结
总体而言，Javassist 是通过在运行时操作字节码，使用高级别的 API 和类抽象（如 CtClass、CtMethod 等），使得动态代码生成和修改变得更加直观和容易。这种方式使开发人员可以在不直接操作底层字节码指令的情况下，实现对 Java 类的动态操作。
