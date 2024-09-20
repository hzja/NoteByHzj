Java 类加载器<br />在java中`Class.forName()`和`ClassLoader`都可以对类进行加载。`ClassLoader`就是遵循双亲委派模型最终调用启动类加载器的类加载器，实现的功能是“通过一个类的全限定名来获取描述此类的二进制字节流”，获取到二进制流后放到JVM中。`Class.forName()`方法实际上也是调用的`ClassLoader`来实现的。<br />`Class.forName(String className)`；这个方法的源码是
```java
@CallerSensitive
public static Class<?> forName(String className)
    throws ClassNotFoundException {
    Class<?> caller = Reflection.getCallerClass();
    return forName0(className, true, ClassLoader.getClassLoader(caller), caller);
}
```
最后调用的方法是`forName0`这个方法，在这个`forName0`方法中的第二个参数被默认设置为了`true`，这个参数代表是否对加载的类进行初始化，设置为`true`时会类进行初始化，代表会执行类中的静态代码块，以及对静态变量的赋值等操作。<br />也可以调用`Class.forName(String name, boolean initialize,ClassLoader loader)`方法来手动选择在加载类的时候是否要对类进行初始化。`Class.forName(String name, boolean initialize,ClassLoader loader)`的源码如下：
```java
/* @param name       fully qualified name of the desired class
 * @param initialize if {@code true} the class will be initialized.
 *                   See Section 12.4 of <em>The Java Language Specification</em>.
 * @param loader     class loader from which the class must be loaded
 * @return           class object representing the desired class
 *
 * @exception LinkageError if the linkage fails
 * @exception ExceptionInInitializerError if the initialization provoked
 *            by this method fails
 * @exception ClassNotFoundException if the class cannot be located by
 *            the specified class loader
 *
 * @see       java.lang.Class#forName(String)
 * @see       java.lang.ClassLoader
 * @since     1.2     */
@CallerSensitive
public static Class<?> forName(String name, boolean initialize,
                               ClassLoader loader)
    throws ClassNotFoundException
{
    Class<?> caller = null;
    SecurityManager sm = System.getSecurityManager();
    if (sm != null) {
        // Reflective call to get caller class is only needed if a security manager
        // is present.  Avoid the overhead of making this call otherwise.
        caller = Reflection.getCallerClass();
        if (sun.misc.VM.isSystemDomainLoader(loader)) {
            ClassLoader ccl = ClassLoader.getClassLoader(caller);
            if (!sun.misc.VM.isSystemDomainLoader(ccl)) {
                sm.checkPermission(
                    SecurityConstants.GET\_CLASSLOADER\_PERMISSION);
            }
        }
    }
    return forName0(name, initialize, loader, caller);
}
```
源码中的注释只摘取了一部分，其中对参数`initialize`的描述是：`if {@code true} the class will be initialized`.意思就是说：如果参数为`true`，则加载的类将会被初始化。
<a name="ptRCu"></a>
### 举例
下面还是举例来说明结果吧：<br />一个含有静态代码块、静态变量、赋值给静态变量的静态方法的类
```java
public class ClassForName {
    //静态代码块
    static {
        System.out.println("执行了静态代码块");
    }
    //静态变量
    private static String staticFiled = staticMethod();
    //赋值静态变量的静态方法
    public static String staticMethod(){
        System.out.println("执行了静态方法");
        return "给静态字段赋值了";
    }
}
```
使用`Class.forName()`的测试方法：
```java
@Test
public void test45(){
    try {
        ClassLoader.getSystemClassLoader().loadClass("com.fcant.test.ClassForName");
        System.out.println("#########-------------结束符------------##########");
    } catch (ClassNotFoundException e) {
        e.printStackTrace();
    }
}
```
运行结果：
```
执行了静态代码块
执行了静态方法
#########-------------结束符------------##########
```
使用`ClassLoader`的测试方法：
```java
@Test
public void test45(){
    try {
        ClassLoader.getSystemClassLoader().loadClass("com.eurekaclient2.client2.ClassForName");
        System.out.println("#########-------------结束符------------##########");
    } catch (ClassNotFoundException e) {
        e.printStackTrace();
    }
}
```
运行结果：
```
#########-------------结束符------------##########
```
:::info
根据运行结果得出`Class.forName`加载类时将类进了初始化，而`ClassLoader`的`loadClass`并没有对类进行初始化，只是把类加载到了虚拟机中。
:::
<a name="YosbE"></a>
### 应用场景
在熟悉的Spring框架中的IOC的实现就是使用的ClassLoader。<br />而在使用JDBC时通常是使用`Class.forName()`方法来加载数据库连接驱动。这是因为在JDBC规范中明确要求`Driver`(数据库驱动)类必须向`DriverManager`注册自己。<br />以MySQL的驱动为例解释：
```java
public class Driver extends NonRegisteringDriver implements java.sql.Driver {  
    // ~ Static fields/initializers  
    // ---------------------------------------------  
    //  
    // Register ourselves with the DriverManager  
    //  
    static {  
        try {  
            java.sql.DriverManager.registerDriver(new Driver());  
        } catch (SQLException E) {  
            throw new RuntimeException("Can't register driver!");  
        }  
    }  
    // ~ Constructors  
    // -----------------------------------------------------------  
    /** 
     * Construct a new driver and register it with DriverManager 
     *  
     * @throws SQLException 
     *             if a database error occurs. 
     */  
    public Driver() throws SQLException {  
        // Required for Class.forName().newInstance()      }  
}
```
看到`Driver`注册到`DriverManager`中的操作写在了静态代码块中，这就是为什么在写JDBC时使用`Class.forName()`的原因了。
