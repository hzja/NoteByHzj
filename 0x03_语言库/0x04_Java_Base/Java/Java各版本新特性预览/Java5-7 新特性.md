Java
<a name="Q8hRW"></a>
### Java 5 新特性
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602511874656-38d95747-9aeb-4d15-a77c-9762a789e1ed.png#height=487&id=QcZBM&originHeight=487&originWidth=929&originalType=binary&ratio=1&size=0&status=done&style=none&width=929)
<a name="6raso"></a>
#### 1、泛型
泛型本质是参数化类型，解决不确定具体对象类型的问题。
```java
List<String> strList=new ArrayList<String>();
```
<a name="6Q3MX"></a>
#### 2、增强循环（for-each）
for-each循环简化了集合的遍历。
```java
String [] str = {"Hello","World","Fcant"};
for (String temp:str) {
     System.out.println(temp);
}
```
<a name="ZNyqd"></a>
#### 3、自动封箱拆箱

- 自动装箱：就是将基本数据类型自动转换成对应的包装类。
- 自动拆箱：就是将包装类自动转换成对应的基本数据类型。

包装类型有：Integer,Double,Float,Long,Short,Character和Boolean
```java
Integer i =666;  //自动装箱
int a= i;     //自动拆箱
```
<a name="dxawv"></a>
#### 4、枚举
关键字enum可以将一组具名的值的有限集合创建为一种新的类型，而这些具名的值可以作为常规的程序组件使用，这就是枚举类型。
```java
enum SeasonEnum {
    SPRING,SUMMER,FALL,WINTER;
}
```
<a name="6fYlI"></a>
#### 5、可变参数
在定义方法参数的时候不确定定义多少个，就可以定义为**「可变参数」**，它本质上是一个**「数组」**。
```java
public static void main(String[] args) throws Exception {
    String [] str = {"Hello","World","Fcant"};
    testVarargs(str);
    String str1 = "Hello,World";
    testVarargs(str1);
}
//可变参数String... args
private static void testVarargs(String... args) {
    for (String arg : args) {
        System.out.println(arg);
    }
}
```
<a name="ZhUzq"></a>
#### 6、注解
可以把注解理解为代码里的特殊标记，这些标记可以在编译，类加载，运行时被读取，并执行相应的处理。
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.SOURCE)
public @interface Override {
}
```
<a name="Ppf6w"></a>
#### 7、静态导入
通过import static类，就可以使用类里的静态变量或方法。看一下例子哈~
```java
import static java.lang.System.out; //静态导入System类的静态变量out
public class Test {
    public static void main(String[] args) throws Exception {
        String str1 = "Hello,World";
        System.out.println(str1); //常规写法
        out.println(str1);  //静态导入，可以直接使用out输出
    }
}
```
<a name="9tPCl"></a>
#### 8、线程并发库（JUC）
JDK5 丰富了线程处理功能，java.util.concurrent包提供了以下的类、接口：
> - 线程池：ExecutorService接口
> - 线程护斥：Lock 类
> - 线程通信：Condition接口
> - 同步队列：ArrayBlockingQueue类
> - 同步集合：ConcurrentHashMap类

<a name="9Ze13"></a>
### Java 6 新特性
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602511874637-f139f048-00a7-4d9a-b846-8af20faafe9b.png#height=425&id=vehE0&originHeight=425&originWidth=1080&originalType=binary&ratio=1&size=0&status=done&style=shadow&width=1080)
<a name="O32vh"></a>
#### 1、Desktop类和SystemTray类
JDK 6在java.awt包下，新增了两个类：`Desktop`类和`SystemTray`类
> - **「Desktop类」**：用来打开系统默认浏览器浏览指定的URL，打开系统默认邮件客户端发邮件等
> - **「SystemTray类」**：用来在系统托盘区创建一个托盘程序，如果在微软的Windows上，它被称为“任务栏”状态区域。

```java
//获取Desktop实例
Desktop desktop = Desktop.getDesktop();
desktop.browse(URI.create("https://www.google.com"));
```
<a name="hijv4"></a>
#### 2、使用JAXB2来实现对象与XML之间的映射
JAXB，即Java Architecture for XML Binding，可以实现对象与XML之间的映射，常用注解如下：
> - `@XmlRootElement`：注解在类上面，对应xml的跟元素，使用name属性定义根节点的名称。
> - `@XmlElement`：指定一个字段或get/set方法映射到xml的节点，使用name属性定义这个根节点的名称。
> - `@XmlAttribute`：将JavaBean对象的属性映射为xml的属性，使用name属性为生成的xml属性指定别名。
> - `@XmlAccessorType`：定义映射这个类中的何种类型都需要映射到xml。
> - `@XmlSchema`：将包映射到XML名称空间

**「举个例子」**
```java
public class JAXB2XmlTest {
    public static void main(String[] args) throws JAXBException, IOException {
        
        List<Singer> list = new ArrayList<>();
        list.add(new Singer("jay", 8));
        list.add(new Singer("eason", 10));
        SingerList singerList = new SingerList();
        singerList.setSingers(list);
        String str = JAXB2XmlTest.beanToXml(singerList, SingerList.class);
        String path = "C:\\jay.txt";
        BufferedWriter bfw = new BufferedWriter(new FileWriter(new File(path)));
        bfw.write(str);
        bfw.close();
    }
    private static String beanToXml(Object obj, Class<?> load) throws JAXBException {
        JAXBContext context = JAXBContext.newInstance(load);
        Marshaller marshaller = context.createMarshaller();
        marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
        marshaller.setProperty(Marshaller.JAXB_ENCODING, "GBK");
        StringWriter writer = new StringWriter();
        marshaller.marshal(obj,writer);
        return writer.toString();
    }
}
public class Singer {
    private String name;
    private int age;
    public Singer(String name, int age) {
        this.name = name;
        this.age = age;
    }
    @XmlAttribute(name="name")
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    @XmlAttribute(name="age")
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
}
@XmlRootElement(name="list")
public class SingerList {
    private List<Singer> singers;
    
    @XmlElement(name="singer")
    public List<Singer> getSingers() {
        return singers;
    }
    public void setSingers(List<Singer> singers) {
        this.singers = singers;
    }
}
```
**「运行效果：」**
```xml
<?xml version="1.0" encoding="GBK" standalone="yes"?>
<list>
    <singer age="8" name="jay"/>
    <singer age="10" name="eason"/>
</list>
```
<a name="fC2Ec"></a>
#### 3、轻量级 Http Server API
JDK 6中提供了简单的Http Server API，可以构建嵌入式Http服务器，同时支持Http和Https协议。HttpServer会调用HttpHandler实现类的回调方法来处理客户端请求，这里用户只需实现HttpHandler接口就可以了。
```java
/**
 * 根据Java提供的API实现Http服务器
 */
public class MyHttpServer {
    /**
     * @param args
     * @throws IOException
     */
    public static void main(String[] args) throws IOException {
        //创建HttpServer服务器
        HttpServer httpServer = HttpServer.create(new InetSocketAddress(8080), 10);
        //将 /jay请求交给MyHandler处理器处理
        httpServer.createContext("/", new MyHandler());
        httpServer.start();
    }
}
public class MyHandler implements HttpHandler {
    public void handle(HttpExchange httpExchange) throws IOException {
        //请求头
        Headers headers = httpExchange.getRequestHeaders();
        Set<Map.Entry<String, List<String>>> entries = headers.entrySet();
        StringBuffer response = new StringBuffer();
        for (Map.Entry<String, List<String>> entry : entries){
            response.append(entry.toString() + "\n");
        }
        //设置响应头属性及响应信息的长度
        httpExchange.sendResponseHeaders(200, response.length());
        //获得输出流
        OutputStream os = httpExchange.getResponseBody();
        os.write(response.toString().getBytes());
        os.close();
    }
}
```
<a name="WouzZ"></a>
#### 4、插入式注解处理API
> JDK 6提供了插入式注解处理API，可以让定义的注解在编译期而不是运行期生效，从而可以在编译期修改字节码。lombok框架就是使用该特性来实现的，Lombok通过注解的方式，在编译时自动为属性生成构造器、getter/setter、`equals`、`hashcode`、`toString`等方法，大大简化了代码的开发。

<a name="Jp5tX"></a>
#### 5、STAX
STAX，是JDK6中一种处理XML文档的API。
```java
public class STAXTest {
    public static void main(String[] args) throws Exception {
        XMLInputFactory xmlInputFactory = XMLInputFactory.newInstance();
        XMLEventReader xmlEventReader = xmlInputFactory.createXMLEventReader(new FileInputStream("C:\\jay.xml"));
        XMLEvent event = null;
        StringBuffer stringBuffer = new StringBuffer();
        while (xmlEventReader.hasNext()) {
            event = xmlEventReader.nextEvent();
            stringBuffer.append(event.toString());
        }
        System.out.println("xml文档解析结果：");
        System.out.println(stringBuffer);
    }
}
```
**「运行结果：」**
```xml
xml文档解析结果：
<?xml version="1.0" encoding='GBK' standalone='yes'?><list>
    <singer name='jay' age='8'></singer>
    <singer name='eason' age='10'></singer>
</list>ENDDOCUMENT
```
<a name="TI0zM"></a>
#### 6、Common Annotations
> Common annotations原本是Java EE 5.0(JSR 244)规范的一部分，现在SUN把它的一部分放到了Java SE 6.0中。随着Annotation元数据功能加入到Java SE 5.0里面，很多Java 技术都会用Annotation部分代替XML文件来配置运行参数。

以下列举Common Annotations 1.0里面的几个Annotations：

- `@Generated`：用于标注生成的源代码
- `@Resource`： 用于标注所依赖的资源，容器据此注入外部资源依赖，有基于字段的注入和基于setter方法的注入两种方式 。
- `@Resources`：同时标注多个外部依赖，容器会把所有这些外部依赖注入
- `@PostConstruct`：标注当容器注入所有依赖之后运行的方法，用来进行依赖注入后的初始化工作，只有一个方法可以标注为`PostConstruct `。
- `@PreDestroy`：当对象实例将要被从容器当中删掉之前，要执行的回调方法要标注为`PreDestroy`
<a name="TnpHf"></a>
#### 7、Compiler API
javac编译器可以把.java的源文件编译为.class文件，JDK 6的新特性Compiler API(JSR 199)也可以动态编译Java源文件。
```java
public class CompilerApiTest {
    public static void main(String[] args) throws Exception {
        JavaCompiler javaCompiler = ToolProvider.getSystemJavaCompiler();
        StandardJavaFileManager standardJavaFileManager = javaCompiler.getStandardFileManager(null,null,null);
        Iterable<? extends JavaFileObject> javaFileObjects = standardJavaFileManager.getJavaFileObjects("C:\\Singer.java");
        javaCompiler.getTask(null, standardJavaFileManager, null, null, null, javaFileObjects).call();
        standardJavaFileManager.close();
    }
}
```
运行结果：会在C目录生成Singer.class文件
<a name="6mycO"></a>
#### 8、对脚本语言的支持（如: ruby, groovy, javascript）
JDK6增加了对脚本语言的支持(JSR 223)，原理是将脚本语言编译成字节码，这样脚本语言也能享用Java平台的诸多优势，包括可移植性，安全等。JDK6实现包含了一个基于Mozilla Rhino的 脚本语言引擎，因此可以支持javascript，当然JDK也支持ruby等其他语言
```java
public class JavaScriptTest {
    public static void main(String[] args) throws Exception {
        ScriptEngineManager factory = new ScriptEngineManager();
        ScriptEngine engine = factory.getEngineByName("JavaScript");
        String script;
        try {
            script = "print('Hello')";
            engine.eval(script);// 执行脚本
        }catch (Exception e) {
            e.printStackTrace();
        }
    }
}
//output
Hello
```
<a name="3t830"></a>
### Java 7 新特性
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602511874674-2c7148af-e566-45b3-bb71-cd87ceb19e6b.png#height=385&id=QYXUm&originHeight=385&originWidth=1080&originalType=binary&ratio=1&size=0&status=done&style=shadow&width=1080)
<a name="RnH6O"></a>
#### 1、switch 支持String字符串类型。
```java
String singer = "jay";
switch (singer) {
    case "jay" :
        System.out.println("周杰伦");
        break;
    case "eason" :
        System.out.println("陈奕迅");
        break ;
    default :
        System.out.println("其他");
        break ;
}
```
<a name="zkXkS"></a>
#### 2、try-with-resources，资源自动关闭
JDK 7 之前：
```java
BufferedReader br = new BufferedReader(new FileReader("d:七里香.txt"));
try {
    return br.readLine();
} finally {
    br.close();
}
```
JDK 7 之后：
```java
/*
 * 声明在try括号中的对象称为资源，在方法执行完毕后会被自动关闭
 */
try (BufferedReader br = new BufferedReader(new FileReader("d:七里香.txt")) {
   return br.readLine();
}
```
<a name="ZJEPq"></a>
#### 3、整数类型如（byte，short，int，long）能够用二进制来表示
```java
//0b或者0B表示二进制
int a = 0b010;
int b = 0B010;
```
<a name="4onR1"></a>
#### 4、数字常量支持下划线
```java
int a = 11_11;//a的值为1111，下划线不影响实际值，提升可读性
```
<a name="iFq7q"></a>
#### 5、泛型实例化类型自动推断，即”<>”
JDK 7 之前：
```java
Map<String, List<String>> map = new HashMap<String, List<String>>();
```
JDK 7之后：
```java
//不须声明类型,自动根据前面<>推断其类型
Map<String, List<String>> map = new HashMap<>();
```
<a name="qCw0R"></a>
#### 6、一个catch中捕获多个异常类型，用（|）分隔开
JDK 7之前
```java
try{
    //do something
} catch (FirstException e) {
    logger.error(e);
} catch (SecondException e) {
    logger.error(ex);
}
```
JDk 7之后
```java
try{
    //do something
} catch (FirstException | SecondException e) {
    logger.error(e);
}
```
<a name="64e8N"></a>
#### 7、增强的文件系统
Java7 提供了全新的NIO2.0 API，方便文件管理的编码。如，可以在java.nio.file包下使用Path、Paths、Files、WatchService等常用类型。
```java
Path path = Paths.get("C:\\jay\\七里香.txt"); //创建Path对象
byte[] bytes= Files.readAllBytes(path);  //读取文件
System.out.println(path.getFileName()); //获取当前文件名称
System.out.println(path.toAbsolutePath()); // 获取文件绝对路径
System.out.println(new String(bytes, "utf-8"));
```
<a name="3xkQT"></a>
#### 8、Fork/join 框架
Java7提供的一个用于并行执行任务的框架，是一个把大任务分割成若干个小任务，最终汇总每个小任务结果后得到大任务结果的框架。![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602511874693-7aa729e2-f956-439c-912c-4aa39f0a63b5.png#height=839&id=aDDVF&originHeight=839&originWidth=1080&originalType=binary&ratio=1&size=0&status=done&style=shadow&width=1080)<br />Fork/join计算1-1000累加值：
```java
public class ForkJoinPoolTest {
    private static final Integer DURATION_VALUE = 100;
    static class ForkJoinSubTask extends RecursiveTask<Integer>{
        // 子任务开始计算的值
        private Integer startValue;
        // 子任务结束计算的值
        private Integer endValue;
        private ForkJoinSubTask(Integer startValue , Integer endValue) {
            this.startValue = startValue;
            this.endValue = endValue;
        }
        @Override
        protected Integer compute() {
            //小于一定值DURATION,才开始计算
            if(endValue - startValue < DURATION_VALUE) {
                System.out.println("执行子任务计算：开始值 = " + startValue + ";结束值 = " + endValue);
                Integer totalValue = 0;
                for (int index = this.startValue; index <= this.endValue; index++) {
                    totalValue += index;
                }
                return totalValue;
            } else {
                // 将任务拆分，拆分成两个任务
                ForkJoinSubTask subTask1 = new ForkJoinSubTask(startValue, (startValue + endValue) / 2);
                subTask1.fork();
                ForkJoinSubTask subTask2 = new ForkJoinSubTask((startValue + endValue) / 2 + 1 , endValue);
                subTask2.fork();
                return subTask1.join() + subTask2.join();
            }
        }
    }
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        // Fork/Join框架的线程池
        ForkJoinPool pool = new ForkJoinPool();
        ForkJoinTask<Integer> taskFuture =  pool.submit(new ForkJoinSubTask(1,1000));
        Integer result = taskFuture.get();
        System.out.println("累加结果是:" + result);
    }
}
```
运行结果：
```
...
执行子任务计算：开始值 = 189;结束值 = 250
执行子任务计算：开始值 = 251;结束值 = 313
执行子任务计算：开始值 = 314;结束值 = 375
执行子任务计算：开始值 = 376;结束值 = 438
执行子任务计算：开始值 = 439;结束值 = 500
执行子任务计算：开始值 = 501;结束值 = 563
执行子任务计算：开始值 = 564;结束值 = 625
执行子任务计算：开始值 = 626;结束值 = 688
执行子任务计算：开始值 = 689;结束值 = 750
执行子任务计算：开始值 = 751;结束值 = 813
执行子任务计算：开始值 = 814;结束值 = 875
执行子任务计算：开始值 = 876;结束值 = 938
执行子任务计算：开始值 = 939;结束值 = 1000
累加结果是:500500
```
