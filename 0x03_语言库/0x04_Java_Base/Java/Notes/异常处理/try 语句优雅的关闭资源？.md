Java
<a name="ZFKI6"></a>
## 一、摘要
try-with-resources是 JDK 7 中引入的一个新的异常处理机制，**它能让开发人员不用显式的释放try-catch语句块中使用的资源**。<br />比如，以文件资源拷贝为示例，大家所熟悉的try-catch-finally写法如下：
```java
public class ResourceTest1 {

    public static void main(String[] args) {
        BufferedInputStream bin = null;
        BufferedOutputStream bout = null;
        try {
            bin = new BufferedInputStream(new FileInputStream(new File( "test.txt")));
            bout = new BufferedOutputStream(new FileOutputStream(new File( "out.txt")));
            int b;
            while ((b = bin.read()) != -1) {
                bout.write(b);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
         //关闭文件流
            if (bin != null) {
                try {
                    bin.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (bout != null) {
                try {
                    bout.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```
现在将其改成使用try-with-resources编程方式，可以发现只需要简单的几行代码就可以搞定，不用显式关闭资源，方式如下：
```java
public class ResourceTest2 {

    public static void main(String[] args) {
        try (BufferedInputStream bin = new BufferedInputStream(new FileInputStream(new File("test.txt")));
             BufferedOutputStream bout = new BufferedOutputStream(new FileOutputStream(new File("out.txt")))) {
            int b;
            while ((b = bin.read()) != -1) {
                bout.write(b);
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
在 JDK7 之前，在处理必须关闭的资源时，开发人员必须要牢记在try-catch语句中使用`finally`执行关闭资源的方法，否则随着程序不断运行，资源泄露将会累计成重大的生产事故，如果程序中同时打开了多个资源，关闭资源的代码竟然比业务代码还要多，使得代码更加难以清晰的阅读和管理。<br />因此在这样的背景下，try-with-resources由此诞生，它的设计初衷就是旨在减轻开发人员释放try块中使用的资源负担。<br />习惯了try-catch-finally写法的同学，可能会发出疑问，是不是所有涉及到资源的操作都可以用try-with-resources编程？使用这种编程方式有没有坑？如果有坑，使用的时候哪些地方应该需要注意呢？....<br />一起来看看try-with-resources编程原理。
<a name="OU0sN"></a>
## 二、实践解说
try-with-resources语句能确保每个资源在语句结束时被关闭，**但是有一个前提条件，那就是这个资源必须实现了**`**java.lang.AutoCloseable**`**接口，才可以被执行关闭**。<br />try-with-resources编程模式中，无需开发人员显式关闭资源的前提是，这个资源必须实现`java.lang.AutoCloseable`接口，并且重写`close`方法，否则无法在try-with-resources中进行声明变量。<br />下面可以关闭单个资源为例，代码如下：
```java
public class TryResourceDemo implements AutoCloseable {

    public void doSomething(){
        System.out.println("do something");
    }

    @Override
    public void close() throws Exception {
        System.out.println("resource is closed");
    }
}
```
```java
public class TryResourceTest {

    public static void main(String[] args) {
        try(TryResourceDemo res = new TryResourceDemo()) {
            res.doSomething();
        } catch(Exception ex) {
            ex.printStackTrace();
        }
    }
}
```
运行结果如下：
```java
do something
resource is closed
```
可以很清晰的看到，`close`方法被调用了！<br />下面再打开反编译后的`TryResourceTest.class`文件代码，编译器自动给代码加上了`finally`方法，并且会调用`close`方法，将资源关闭！
```java
public class TryResourceTest {

    public static void main(String[] args) {
        try {
            TryResourceDemo res = new TryResourceDemo();
            Throwable var2 = null;

            try {
                res.doSomething();
            } catch (Throwable var12) {
                var2 = var12;
                throw var12;
            } finally {
                if (res != null) {
                    if (var2 != null) {
                        try {
                            res.close();
                        } catch (Throwable var11) {
                            var2.addSuppressed(var11);
                        }
                    } else {
                        res.close();
                    }
                }

            }
        } catch (Exception var14) {
            var14.printStackTrace();
        }

    }
}
```
也就是说，使用try-with-resources编程，其实是编译器显式的给代码了添加`finally`方法，省去开发人员手动关闭资源的操作！
<a name="k7ZQo"></a>
## 三、资源关闭顺序
上面只介绍了关闭单个资源的场景，假如有多个资源时，try-with-resources是如何关闭的呢？<br />下面还是举例看结果。
```java
public class TryResourceDemo1 implements AutoCloseable {

    public void doSomething(){
        System.out.println("do something 1");
    }

    @Override
    public void close() throws Exception {
        System.out.println("resource 1 is closed");
    }
}
```
```java
public class TryResourceDemo2 implements AutoCloseable {

    public void doSomething(){
        System.out.println("do something 2");
    }

    @Override
    public void close() throws Exception {
        System.out.println("resource 2 is closed");
    }
}
```
```java
public class TryResourceDemoTest {

    public static void main(String[] args) {
        try(TryResourceDemo1 demo1 = new TryResourceDemo1();
            TryResourceDemo2 demo2 = new TryResourceDemo2()) {
            System.out.println("do...");
            demo1.doSomething();
            demo2.doSomething();
        } catch(Exception ex) {
            ex.printStackTrace();
        }
    }
}
```
运行结果如下：
```java
do...
do something 1
do something 2
resource 2 is closed
resource 1 is closed
```
从结果上可以看出，try语句中越是最后使用的资源，越是最早被关闭。<br />关于这一点，大家可以从反编译的代码中找到原理！
<a name="s25Rx"></a>
## 四、异常处理机制
正常的情况下，`try`语句结束时会关闭相关的资源，假如语句内部执行时发生异常，同时又显式的调用了`finally`方法，执行的顺序又是怎样的呢？<br />下面继续举例看结果。
```java
public class TryThrowResourceDemoTest {

    public static void main(String[] args) {
        AutoCloseable obj1 = null;
        AutoCloseable obj2 = null;
        try (TryResourceDemo1 demo1 = new TryResourceDemo1();
             TryResourceDemo2 demo2 = new TryResourceDemo2();) {
            System.out.println("do...");
            obj1 = demo1;
            System.out.println(1 / 0);
            obj2 = demo2;
            System.out.println("over...");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                System.out.println("before finally close");
                if (obj1 != null) {
                    obj1.close();
                }
                if (obj2 != null) {
                    obj2.close();
                }
                System.out.println("after finally close");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
```
运行结果如下：
```java
do...
resource 2 is closed
resource 1 is closed
before finally close
resource 1 is closed
after finally close
java.lang.ArithmeticException: / by zero
 at com.example.java.trywithresources.a.TryThrowResourceDemoTest.main(TryThrowResourceDemoTest.java:18)
```
可以很清晰的看到，可以得出如下结论：

1. 只要实现了`AutoCloseable`接口的类，并且在try里声明了对象变量，在try结束后，**不管是否发生异常，close方法都会被调用**
2. 其次，在try里越晚声明的对象，会越早被`close`掉
3. try结束后自动调用的`close`方法，这个动作会早于`finally`里调用的方法
<a name="gi8Kz"></a>
## 五、压制异常处理
大部分情况，通常不会担心资源的`close`会发生异常，现在假设如果try里声明的资源对象，当执行`close`方法抛异常时，他们的执行顺序又是怎样的呢？又如何获取这种异常呢？<br />还是眼见为实，下面以举例看结果。
```java
public class TryThrowableResourceDemo1 implements AutoCloseable {

    public void doSomething(){
        System.out.println("do something 1");
        throw new NullPointerException("TryThrowableResourceDemo1: doSomething() NullPointerException");
    }

    @Override
    public void close() throws Exception {
        System.out.println("TryThrowableResourceDemo1 is closed");
        throw new NullPointerException("TryThrowableResourceDemo1: close() NullPointerException");
    }
}
```
```java
public class TryThrowableResourceDemo2 implements AutoCloseable {

    public void doSomething(){
        System.out.println("do something 2");
        throw new NullPointerException("TryThrowableResourceDemo2: doSomething() NullPointerException");
    }

    @Override
    public void close() throws Exception {
        System.out.println("TryThrowableResourceDemo2 is closed");
        throw new NullPointerException("TryThrowableResourceDemo2: close() NullPointerException");
    }
}
```
```java
public class TryThrowableResourceDemoTest {

    public static void main(String[] args) {
        try (TryThrowableResourceDemo1 demo1 = new TryThrowableResourceDemo1();
             TryThrowableResourceDemo2 demo2 = new TryThrowableResourceDemo2()) {
            System.out.println("do...");
            demo1.doSomething();
            demo2.doSomething();
        } catch (Exception e) {
            System.out.println("gobal: exception");
            System.out.println(e.getMessage());
            Throwable[] suppressed = e.getSuppressed();
            for (int i = 0; i < suppressed.length; i++){
                System.out.println(suppressed[i].getMessage());
            }
        }
    }
}
```
运行结果如下：
```java
do...
do something 1
TryThrowableResourceDemo2 is closed
TryThrowableResourceDemo1 is closed
gobal: exception
TryThrowableResourceDemo1: doSomething() NullPointerException
TryThrowableResourceDemo2: close() NullPointerException
TryThrowableResourceDemo1: close() NullPointerException
```
从运行结果可以很清晰的看到，对于try语句块内的异常，可以通过`e.getMessage()`获取，对于`close()`方法抛出的异常，其实编译器对这部分的异常进行特殊处理，将其放入到集合数组中了，因此需要通过`e.getSuppressed()`方法来获取。<br />具体反编译后的代码如下：
```java
public class TryThrowableResourceDemoTest {
    
    public static void main(String[] args) {
        try {
            TryThrowableResourceDemo1 demo1 = new TryThrowableResourceDemo1();
            Throwable var34 = null;

            try {
                TryThrowableResourceDemo2 demo2 = new TryThrowableResourceDemo2();
                Throwable var4 = null;

                try {
                    System.out.println("do...");
                    demo1.doSomething();
                    demo2.doSomething();
                } catch (Throwable var29) {
                    var4 = var29;
                    throw var29;
                } finally {
                    if (demo2 != null) {
                        if (var4 != null) {
                            try {
                                demo2.close();
                            } catch (Throwable var28) {
                                var4.addSuppressed(var28);
                            }
                        } else {
                            demo2.close();
                        }
                    }

                }
            } catch (Throwable var31) {
                var34 = var31;
                throw var31;
            } finally {
                if (demo1 != null) {
                    if (var34 != null) {
                        try {
                            demo1.close();
                        } catch (Throwable var27) {
                            var34.addSuppressed(var27);
                        }
                    } else {
                        demo1.close();
                    }
                }

            }
        } catch (Exception var33) {
            System.out.println("gobal: exception");
            System.out.println(var33.getMessage());
            Throwable[] suppressed = var33.getSuppressed();

            for(int i = 0; i < suppressed.length; ++i) {
                System.out.println(suppressed[i].getMessage());
            }
        }

    }
}
```
<a name="m4ZVN"></a>
## 六、关闭资源的坑
在实际的使用中，不管是使用try-with-resource编程还是使用try-catch-finally编程**，一定需要了解资源的**`**close**`**方法内部的实现逻辑，否则还是可能会导致资源泄露**。<br />举个例子，在 Java BIO 中采用了大量的装饰器模式。当调用装饰器的 close 方法时，本质上是调用了装饰器包装的流对象的 close 方法。比如：
```java
public class TryWithResource {
    public static void main(String[] args) {
        try (FileInputStream fin = new FileInputStream(new File("input.txt"));
                GZIPOutputStream out = new GZIPOutputStream(new FileOutputStream(new File("out.txt")))) {
            byte[] buffer = new byte[4096];
            int read;
            while ((read = fin.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
在上述代码中，从`FileInputStream`中读取字节，并且写入到`GZIPOutputStream`中。`GZIPOutputStream`实际上是`FileOutputStream`的装饰器。<br />由于try-with-resource的特性，实际编译之后的代码会在后面带上`finally`代码块，并且在里面调用`fin.close()`方法和out.close()方法。<br />再来看`GZIPOutputStream`类的`close`方法。
```java
public void close() throws IOException {
    if (!closed) {
        finish();
        if (usesDefaultDeflater)
            def.end();
        out.close();
        closed = true;
    }
}
```
在调用out变量的`close`方法之前，`GZIPOutputStream`还做了`finish`操作，该操作还会继续往`FileOutputStream`中写压缩信息，此时如果出现异常，则`out.close()`方法会被略过，**而out变量实际上代表的是被装饰的**`**FileOutputStream**`**类，这个才是最底层的资源关闭方法**。<br />正确的做法应该是在try-with-resource中单独声明最底层的资源，保证对应的`close`方法一定能够被调用。在刚才的例子中，需要单独声明每个`FileInputStream`以及`FileOutputStream`，改成如下方式：
```java
public class TryWithResource {
    public static void main(String[] args) {
        try (FileInputStream fin = new FileInputStream(new File("input.txt"));
                FileOutputStream fout = new FileOutputStream(new File("out.txt"));
                GZIPOutputStream out = new GZIPOutputStream(fout)) {
            byte[] buffer = new byte[4096];
            int read;
            while ((read = fin.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
编译器会自动生成`fout.close()`的代码，这样肯定能够保证真正的流被关闭。
<a name="fFabW"></a>
## 七、小结
在处理必须关闭的资源时，使用try-with-resources语句替代try-catch-finally语句，编写的代码更简洁，更清晰，同时也省去了手动显式释放资源的烦恼。<br />因此在实际编程过程中，推荐大家采用这种方式编写，同时要关注`close`方法内部的实现逻辑，避免资源泄露，服务宕机！
