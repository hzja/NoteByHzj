JavaLombok
<a name="b8yrt"></a>
### 以往异常捕获
何为SneakyThrows？从字面理解就是“偷偷摸摸的抛出”。对的，就是这个意思。该注解属于Lombok，它的作用为减少程序的异常捕获。现在写代码，如果遇到异常，通常需要try catch，或者直接`throws`抛给上一层。<br />例如：
```java
public static void main(String[] args) {
    Class clz = null;
    try {
        clz = Class.forName("com.woshild.derek_ld.lombok.SneakyThrowsTest");
        System.out.println(clz.getName());
        Thread.sleep(3000);
        System.out.println("3秒已过。。。");
    } catch (ClassNotFoundException e) {
        e.printStackTrace();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
}

public static void main(String[] args) throws ClassNotFoundException, InterruptedException {
    Class clz = Class.forName("com.woshild.derek_ld.lombok.SneakyThrowsTest");
    System.out.println(clz.getName());

    Thread.sleep(3000);
    System.out.println("3秒已过。。。");
}
```
以上两种情况都是日常几乎每天都要写的。比如`Thread.sleep`的异常，每次都需要捕获或抛出。很是烦躁。<br />再或者有时程序里有异常处理较多的时候，直接catch **「Exception e」** ，简单直接粗暴的捕获异常。<br />但如此这般，依然有些麻烦。
<a name="OQ7IX"></a>
### 初见`@SneakyThrows`
先看看`@SneakyThrows`是如何用的。
```java
@SneakyThrows
public static void main(String[] args) {
    Class clz = Class.forName("com.woshild.derek_ld.lombok.SneakyThrowsTest");
    System.out.println(clz.getName());

    Thread.sleep(3000);
    System.out.println("3秒已过。。。");
}
```
看到没有，直接在方法上面加上`@SneakyThrows`即可，再也不需要处理异常了，是不是很香？<br />再来看看生成的class文件长什么样。
```java
public static void main(String[] args) {
    try {
        Class clz = Class.forName("com.woshild.derek_ld.lombok.SneakyThrowsTest");
        System.out.println(clz.getName());
        Thread.sleep(3000L);
        System.out.println("3秒已过。。。");
    } catch (Throwable var2) {
        throw var2;
    }
}
```
看，`@SneakyThrows`直接把捕获异常的代码嵌入到了class文件里。<br />也就是说，并不是忽略了异常，而是类似于Lombok的`@Data`注解，在编译时就已经把处理的代码嵌入到了class内。<br />当然，也可以自定义需要`@SneakyThrows`处理的异常，比如例子中，只需要他处理`Thread.sleep`的异常，关于`Class.forName`的异常需要额外做处理。可以这么做：
```java
@SneakyThrows(InterruptedException.class)
public static void main(String[] args) {
    Class clz = null;
    try {
        clz = Class.forName("com.woshild.derek_ld.lombok.SneakyThrowsTest");
        System.out.println(clz.getName());
    } catch (ClassNotFoundException e) {
        //todo: ...
        log.error("[com.woshild.derek_ld.lombok.SneakyThrowsTest] class not found.", e);
    }

    Thread.sleep(3000);
    System.out.println("3秒已过。。。");
}
```
在看一下`@SneakyThrows`注解的定义：
```java
@Target({ElementType.METHOD, ElementType.CONSTRUCTOR})
@Retention(RetentionPolicy.SOURCE)
public @interface SneakyThrows {
    Class<? extends Throwable>[] value() default {Throwable.class};
}
```

- 它可用于方法和构造器。
- value是`Class<? extends Throwable>`，是`Throwable`异常的子类数组。
<a name="HplUd"></a>
### 原理
显然魔法 藏在`Lombok.sneakyThrow(t);`中。可能大家都会以为这个方法就是`new RuntimeException()`之类的。然而事实并非如此。阅读代码可以看出整个方法其实最核心的逻辑是`throw (T)t;`，利用泛型将传入的Throwable强转为`RuntimeException`。虽然事实上不是RuntimeException。但是没关系。因为JVM并不关心这个。泛型最后存储为字节码时并没有泛型的信息。这样写只是为了骗过javac编译器。源码中注释有解释。
```java
public static RuntimeException sneakyThrow(Throwable t) {
    if (t == null) throw new NullPointerException("t");
    return Lombok.<RuntimeException>sneakyThrow0(t);
}

private static <T extends Throwable> T sneakyThrow0(Throwable t) throws T {
    throw (T)t;
}
```
