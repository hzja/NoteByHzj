Java
<a name="LOa0N"></a>
# 1、jdk7之前的关闭流的操作
```java
/**
 * jdk7以前关闭流的方式
 *
 * @author fcant
 * */
public class CloseResourceBeforeJava7 {
    private static final String FileName = "file.txt";

    public static void main(String[] args) throws IOException {
        FileInputStream inputStream = null;

        try {
            inputStream = new FileInputStream(FileName);
            char c1 = (char) inputStream.read();
            System.out.println("c1=" + c1);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
    }
}
```
<a name="7Zr9L"></a>
# 2、jdk7之后try-with-resource关闭资源
实现了 `**java.lang.AutoCloseable**`接口（其中，它包括实现了 `**java.io.Closeable**` 的所有对象），可以使用作为资源。实现`java.lang.AutoCloseable`接口的`Resource`类：
```java
/**
 * 资源类
 *
 * @author fcant
 * */
public class Resource implements AutoCloseable {
    public void sayHello() {
        System.out.println("hello");
    }

    @Override
    public void close() throws Exception {
        System.out.println("Resource is closed");
    }
}
```
```java
/**
 * jdk7及以后关闭流的方式
 *
 * @author fcant
 * */
public class CloseResourceAfterJava7 {
    public static void main(String[] args) {
        try(Resource resource = new Resource()) {
            resource.sayHello();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
输出：
```
hello
Resource is closed
```
当存在多个打开资源的时候：
```java
/**
 * 资源2
 *
 * @author fcant
 * */
public class Resource2 implements AutoCloseable {
    public void sayhello() {
        System.out.println("Resource say hello");
    }

    @Override
    public void close() throws Exception {
        System.out.println("Resource2 is closed");
    }
}
```
```java
/**
 * jdk7及以后关闭流的方式
 *
 * @author fcant
 * */
public class CloseResourceAfterJava7 {
    public static void main(String[] args) {
        try(Resource resource = new Resource(); Resource2 resource2 = new Resource2()) {
            resource.sayHello();
            resource2.sayhello();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
输出：
```
hello
Resource say hello
Resource2 is closed
Resource is closed
```
<a name="ett5z"></a>
# 3、关于异常屏蔽
<a name="bUFPE"></a>
## A.Java7之前-不使用`catch`抛出异常的写法
将上面的测试类进行修改**使用方法抽取异常处理不使用**`**catch**`**抛出异常**
```java
/**
 * CloseResourceAfterJava7
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:04 2020/6/26/0026
 */
public class CloseResourceAfterJava7 {

    public static void main(String[] args) {
        try {
            errorTest();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void errorTest() throws Exception {
        Resource resource = null;
        try {
            resource = new Resource();
            resource.sayHello();
        }

        finally {
            if (resource != null) {
                resource.close();
            }
        }
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593180885466-b3988337-96ed-44cb-b437-22cc2b717454.png#height=610&id=gQ48U&originHeight=1829&originWidth=3093&originalType=binary&ratio=1&rotation=0&showTitle=false&size=609756&status=done&style=shadow&title=&width=1031)
<a name="LLaZ9"></a>
## B.Java7之前-使用`catch`抛出异常的写法
```java
/**
 * CloseResourceAfterJava7
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:04 2020/6/26/0026
 */
public class CloseResourceAfterJava7 {

    public static void main(String[] args) {
        try {
            errorTest();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void errorTest() throws Exception {
        Resource resource = null;
        try {
            resource = new Resource();
            resource.sayHello();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (resource != null) {
                resource.close();
            }
        }
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593180989165-6c625a3e-21a5-4a53-b27d-bf65e9a984bb.png#height=611&id=ol7xf&originHeight=1834&originWidth=3102&originalType=binary&ratio=1&rotation=0&showTitle=false&size=693651&status=done&style=shadow&title=&width=1034)
<a name="lBGeL"></a>
## C.Java7之后-显示被`Suppressed`屏蔽（未使用`catch`抛出）[即A中的问题]的异常
<a name="bkzVe"></a>
### ①不使用catch处理异常
```java
/**
 * CloseResourceAfterJava7
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:04 2020/6/26/0026
 */
public class CloseResourceAfterJava7 {

    public static void main(String[] args) {
        try {
            errorTest();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void errorTest() throws Exception {
        try (Resource resource = new Resource();) {
            resource.sayHello();
        } 
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593181710576-00858c20-6e66-42f8-8260-6bac3923d125.png#height=616&id=YR9rL&originHeight=1847&originWidth=3099&originalType=binary&ratio=1&rotation=0&showTitle=false&size=594263&status=done&style=shadow&title=&width=1033)
<a name="dtKwI"></a>
### ②使用`catch`处理异常与不使用`catch`一样
```java
/**
 * CloseResourceAfterJava7
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:04 2020/6/26/0026
 */
public class CloseResourceAfterJava7 {

    public static void main(String[] args) {
        try {
            errorTest();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void errorTest() throws Exception {
        try (Resource resource = new Resource();) {
            resource.sayHello();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593181825783-b9109656-f4e2-491a-90b0-3d0f45c00b15.png#height=609&id=XSde7&originHeight=1826&originWidth=3095&originalType=binary&ratio=1&rotation=0&showTitle=false&size=636053&status=done&style=shadow&title=&width=1031.6666666666667)
<a name="N03Rw"></a>
## D.分析编译后的class文件查看`AutoCloseable`怎么处理屏蔽的异常
以下是测试类编译后的class源码文件，并非研究操作的功能源码
```java
public class CloseResourceAfterJava7 {
    public CloseResourceAfterJava7() {
    }

    public static void main(String[] args) {
        try {
            errorTest();
        } catch (Exception var2) {
            var2.printStackTrace();
        }

    }

    private static void errorTest() throws Exception {
        Resource resource = new Resource();
        Throwable var1 = null;

        try {
            resource.sayHello();
        } catch (Throwable var10) {
            var1 = var10;
            throw var10;
        } finally {
            if (resource != null) {
                if (var1 != null) {
                    try {
                        resource.close();
                    } catch (Throwable var9) {
                        var1.addSuppressed(var9);
                    }
                } else {
                    resource.close();
                }
            }

        }

    }
}

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593182370093-819b706d-37f7-402e-933e-428bfd26dd10.png#height=488&id=WtQlf&originHeight=1464&originWidth=1939&originalType=binary&ratio=1&rotation=0&showTitle=false&size=238351&status=done&style=shadow&title=&width=646.3333333333334)<br />可以发现编译以后生成了try-catch-finally语句块 `finally`中的`var1.addSuppressed(var9);`<br />这里可以看到`AutoCloseable`底层已经转换了try catch，所以可以不用`catch`抛出异常。<br />另外如果写的不够优雅，IDEA也会提示进行转换的<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593182777232-89bfe74c-7c8a-4164-9cec-c2503417ccde.png#height=251&id=xwmHL&originHeight=754&originWidth=1692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=106486&status=done&style=shadow&title=&width=564)
