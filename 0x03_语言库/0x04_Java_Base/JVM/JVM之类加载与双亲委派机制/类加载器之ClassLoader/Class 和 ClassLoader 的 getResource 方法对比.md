Java 
<a name="GWpPi"></a>
## 访问资源的主要方式
在Java中，通常可以通过以下方式来访问资源：

- `Class`的`getResource`方法;
- `ClassLoader`的`getResource`方法;
- `ClassLoader`的`getResources`方法; //获取批量资源
- `ClassLoader`的`getSystemResource`; //静态方法

在使用中，Class可通过直接引用类的class属性而获得，或是通过实例的 `getClass()` 方法来获得。获取`ClassLoader`的方式则比较多，常见以下几种：

- 调用Class的`getClassLoader`方法，如：`getClass().getClassLoader()`
- 由当前线程获取`ClassLoader`：`Thread.currentThread().getContextClassLoader()`
- 获取系统`ClassLoader`：`ClassLoader.getSystemClassLoader()`
<a name="f1IhR"></a>
## `Class.getResource` 与 `ClassLoader.getResource` 的区别
```java
public class ClassLoaderAndClassContrast {

    public static void main(String[] args) throws Exception {
        Class<ClassLoaderAndClassContrast> aClass = ClassLoaderAndClassContrast.class;
        ClassLoader classLoader = aClass.getClassLoader();

        URL resource = classLoader.getResource("cookies.properties");
        URL resource1 = aClass.getResource("dir/cookies.properties");
        if(resource!=null){
            byte[] bytes = FileCopyUtils.copyToByteArray(resource.openStream());
            System.out.println(new String(bytes));
        }
    }
}
```
两者最大的区别，是从哪里开始寻找资源。

- `ClassLoader`并不关心当前类的包名路径，它永远以classpath为基点来定位资源。需要注意的是在用`ClassLoader`加载资源时，路径不要以"/"开头，所有以"/"开头的路径都返回null；
- `Class.getResource`如果资源名是绝对路径(以"/"开头)，那么会以classpath为基准路径去加载资源，如果不以"/"开头，那么以这个类的Class文件所在的路径为基准路径去加载资源。

在实际开发过程中建议使用`Class.getResource`这个方法，但是如果想获取批量资源，那么就必须使用到`ClassLoader`的`getResources()`方法。
```java
public class ClassLoaderAndClassContrast {

    Class<ClassLoaderAndClassContrast> cls = ClassLoaderAndClassContrast.class;
    ClassLoader ldr = cls.getClassLoader();

    public static void println(Object s) {
        System.out.println(s);
    }

    void showResource(String name) {
        println("## Test resource for: “" + name + "” ##");
        println(String.format("ClassLoader#getResource(\"%s\")=%s", name, ldr.getResource(name)));
        println(String.format("Class#getResource(\"%s\")=%s", name, cls.getResource(name)));
    }

    public final void testForResource() throws Exception {
        showResource("");
        //对于Class来，返回这个类所在的路径
        showResource("/");
        showResource(cls.getSimpleName() + ".class");
        String n = cls.getName().replace('.', '/') + ".class";
        showResource(n);
        showResource("/" + n);
        showResource("java/lang/Object.class");
        showResource("/java/lang/Object.class");
    }

    public static void main(String[] args) throws Exception {
        println("java.class.path: " + System.getProperty("java.class.path"));
        println("user.dir: " + System.getProperty("user.dir"));
        println("");
        ClassLoaderAndClassContrast t = new ClassLoaderAndClassContrast();
        t.testForResource();
    }
}
```
<a name="i5wSA"></a>
## 正确使用`getResource`方法

- 避免使用 `Class.getResource("/")` 或 `ClassLoader.getResource("")`。应该传入一个确切的资源名，然后对输出结果作计算。比如，如果确实想获取当前类是从哪个类路径起点上执行的，以前面提到的test.App来说，可以调用 `App.class.getResource(App.class.getSimpleName() + ".class")`。如果所得结果不是 jar 协议的URL，说明 class 文件没有打包，将所得结果去除尾部的 "test/App.class"，即可获得 test.App 的类路径的起点；如果结果是 jar 协议的 URL，去除尾部的 "!/test/App.class"，和前面的 "jar:"，即是 test.App 所在的 jar 文件的url。
- 如果要定位与某个类同一个包的资源，尽量使用那个类的`getResource`方法并使用相对路径。如前文所述，要获取与 test.App.class 同一个包下的 App.js 文件，应使用 `App.class.getResource("App.js")` 。当然，事无绝对，用 `ClassLoader.getResource("test/App.js")` 也可以，这取决于面对的问题是什么。
- 如果对`ClassLoader`不太了解，那就尽量使用Class的`getResource`方法。
- 如果不理解或无法确定该传给`Class.getResource` 方法的相对路径，那就以类路径的顶层包路径为参考起点，总是传给它以 "/" 开头的路径吧。
- 不要假设调试环境就是最后的运行环境。代码可能不打包，也可能打包，得考虑这些情况，不要埋坑。
<a name="tLWpI"></a>
## 获取批量资源
使用`classLoader`的`getResources`方法可以获得批量资源
```java
ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
Enumeration<URL> resources = classLoader.getResources("META-INF/MANIFEST.MF");
```
<a name="F94V3"></a>
## Spring的ResourceLoader
在Spring框架中`ResourceLoader`和`ResourcePatternResolver`接口封装了获取资源的方法，可以直接拿来使用。`ResourceUtils`这个类中提供了很多判断资源类型的工具方法，可以直接使用。
```java
//前面三种的写法效果是一样的，必须从classpath基准目录开始写精确的匹配路径
//如果想匹配多个资源，需要以classpath*/打头，然后配合通配符使用
ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("beans.spring.xml");

ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("/beans.spring.xml");

ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("classpath:/beans.spring.xml");

ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("classpath*:/**/beans.spring.xml");
```

- `ResourceLoader` 接口
- `ResourcePatternResolver` 接口 --- 重点

给出一个例子
```java
String txt = "";
ResourcePatternResolver resolver = new PathMatchingResourcePatternResolver();
Resource[] resources = resolver.getResources("templates/layout/email.html");
Resource resource = resources[0];
//获得文件流，因为在jar文件中，不能直接通过文件资源路径拿到文件，但是可以在jar包中拿到文件流
InputStream stream = resource.getInputStream();
StringBuilder buffer = new StringBuilder();
byte[] bytes = new byte[1024];
try {
    for (int n; (n = stream.read(bytes)) != -1; ) {
        buffer.append(new String(bytes, 0, n));
    }
} catch (IOException e) {
    e.printStackTrace();
}
txt = buffer.toString();
```

- ResourceUtils
- ResourcePatternUtils
