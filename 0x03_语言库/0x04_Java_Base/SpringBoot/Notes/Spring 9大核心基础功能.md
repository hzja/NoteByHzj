JavaSpring<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958983856-641a295e-1f68-4f01-bc05-794f2c3b9a3a.png#averageHue=%23f0f6f2&clientId=u61bda670-b95d-4&from=paste&id=u376c4d64&originHeight=628&originWidth=626&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7ec7884d-6a93-4dad-b045-6998d1d960c&title=)
<a name="notYd"></a>
## 资源管理
资源管理是Spring的一个核心的基础功能，不过在说Spring的资源管理之前，先来简单说一下Java中的资源管理。
<a name="R8AWj"></a>
### Java资源管理
Java中的资源管理主要是通过java.net.URL来实现的，通过URL的`openConnection`方法可以对资源打开一个连接，通过这个连接读取资源的内容。<br />资源不仅仅指的是网络资源，还可以是本地文件、一个jar包等等。
<a name="iumLd"></a>
#### 1、来个Demo
举个例子，比如想到访问www.baidu.com这个百度首页网络资源，那么此时就可以这么写
```java
public class JavaResourceDemo {

    public static void main(String[] args) throws IOException {
        //构建URL 指定资源的协议为http协议
        URL url = new URL("http://www.baidu.com");
        //打开资源连接
        URLConnection urlConnection = url.openConnection();
        //获取资源输入流
        InputStream inputStream = urlConnection.getInputStream();
        //通过hutool工具类读取流中数据
        String content = IoUtil.read(new InputStreamReader(inputStream));
        System.out.println(content);
    }

}
```
解释一下上面代码的意思：

- 首先构建一个URL，指定资源的访问协议为http协议
- 通过URL打开一个资源访问连接，然后获取一个输入流，读取内容

运行结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958983813-7fe2aaac-2987-4018-b798-1ed7d35492a2.png#averageHue=%232e2e2e&clientId=u61bda670-b95d-4&from=paste&id=u67b20838&originHeight=312&originWidth=944&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u25369215-6d3a-4bd1-878e-b1d0a4acb7f&title=)<br />成功读取到百度首页的数据。<br />当然，也可以通过URL访问本地文件资源，在创建URL的时候只需要指定协议类型为`file://`和文件的路径就行了
```java
URL url = new URL("file://" + "文件的路径");
```
这种方式这里就不演示了。<br />其实这种方式实际上最终也是通过`FileInputStream`来读取文件数据的。
<a name="pBEXt"></a>
#### 2、原理
每种协议的URL资源都需要一个对应的一个`URLStreamHandler`来处理。<br />![URLStreamHandler](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958983871-a0a2b79a-f420-413e-97ab-370c4b2303b3.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u0824d856&originHeight=385&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u98a2636c-333f-4c06-b0ca-14f837c9acf&title=URLStreamHandler "URLStreamHandler")<br />比如说，http://协议有对应的`URLStreamHandler`的实现，file://协议的有对应的URLStreamHandler的实现。<br />Java除了支持http://和file://协议之外，还支持其它的协议，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958983853-dd18c026-b578-4126-9789-a070d9e1b5f9.png#averageHue=%23f5f5f5&clientId=u61bda670-b95d-4&from=paste&id=u0a40ed52&originHeight=151&originWidth=581&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u78c1bb36-05c0-4d59-9bf4-f274013d83c&title=)<br />对于的`URLStreamHandler`如下图所示<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958983887-34633974-72c5-49d6-9a95-f7f205e23d3e.png#averageHue=%23514d43&clientId=u61bda670-b95d-4&from=paste&id=u333f53af&originHeight=302&originWidth=736&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua0edb7d2-f9c7-4f77-9fb5-bb77bc91379&title=)<br />当在构建URL的时候，会去解析资源的访问协议，根据访问协议找到对应的`URLStreamHandler`的实现。<br />当然，除了Java本身支持的协议之外，还可以自己去扩展这个协议，大致只需要两步即可：

- 实现`URLConnection`，可以通过这个连接读取资源的内容
- 实现`URLStreamHandler`，通过`URLStreamHandler`可以获取到URLConnection

不过需要注意的是，`URLStreamHandler`的实现需要放在sun.net.www.protocol.协议名称包下，类名必须是`Handler`，这也是为什么截图中的实现类名都叫`Handler`的原因。<br />当然如果不放在指定的包下也可以，但是需要实现`java.net.URLStreamHandlerFactory`接口。
<a name="BVAnv"></a>
### Spring资源管理
虽然Java提供了标准的资源管理方式，但是Spring并没有用，而是自己搞了一套资源管理方式。
<a name="bTnyE"></a>
#### 1、资源抽象
在Spring中，资源大致被抽象为两个接口

- `Resource`：可读资源，可以获取到资源的输入流
- `WritableResource`：读写资源，除了资源输入流之外，还可以获取到资源的输出流
<a name="fa97B"></a>
##### `Resource`
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984187-b7cb910f-69ac-4c8b-94f2-1a962010a8b9.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=ubfc17cef&originHeight=496&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8b182b14-8bd1-4667-ab53-b1a6b32d5d2&title=)<br />`Resource`接口继承了`InputStreamSource`接口，而`InputStreamSource`接口可以获取定义了获取输入流的方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984452-23836bb2-5b3a-46fa-b640-5b39c2badec2.png#averageHue=%23302f2e&clientId=u61bda670-b95d-4&from=paste&id=uadd71b60&originHeight=336&originWidth=1030&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uee517c52-9f3a-4d84-a224-8636cf0fc13&title=)
<a name="tIzJj"></a>
##### `WritableResource`
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984556-fc666712-f9ff-4751-b092-0cdefcbcf004.png#averageHue=%2332312f&clientId=u61bda670-b95d-4&from=paste&id=u71075234&originHeight=517&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf0b4b701-98f8-4a2c-8852-544c87fb1f5&title=)<br />`WritableResource`继承了`Resource`接口，可以获取到资源的输出流，因为有的资源不仅可读，还可写，就比如一些本地文件的资源，往往都是可读可写的<br />`Resource`的实现很多，这里举几个常见的：

- `FileSystemResource`：读取文件系统的资源
- `UrlResource`：前面提到的Java的标准资源管理的封装，底层就是通过URL来访问资源
- `ClassPathResource`：读取classpath路径下的资源
- `ByteArrayResource`：读取静态字节数组的数据

比如，想要通过Spring的资源管理方式来访问前面提到百度首页网络资源，就可以这么写
```java
//构建资源
Resource resource = new UrlResource("http://www.baidu.com");
//获取资源输入流
InputStream inputStream = resource.getInputStream();
```
如果是一个本地文件资源，那么除了可以使用`UrlResource`，也可以使用`FileSystemResource`，都是可以的。
<a name="HIhtG"></a>
#### 2、资源加载
虽然`Resource`有很多实现，但是在实际使用中，可能无法判断使用具体的哪个实现，所以Spring提供了`ResourceLoader`资源加载器来根据资源的类型来加载资源。<br />![ResourceLoader](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984325-fb6d397a-90f3-4567-b305-123e74697972.png#averageHue=%232d2c2c&clientId=u61bda670-b95d-4&from=paste&id=ud4dc9c0a&originHeight=686&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u0aec9111-9661-40c4-b4f3-3841eba7913&title=ResourceLoader "ResourceLoader")<br />通过`getResource`方法，传入一个路径就可以加载到对应的资源，而这个路径不一定是本地文件，可以是任何可加载的路径。<br />`ResourceLoader`有个唯一的实现`DefaultResourceLoader`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984481-50137847-fc80-4bd9-926d-cd0c2baa4598.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u38e2a7bf&originHeight=346&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udd08e6a1-f58c-4756-8ac1-d28d32e69c7&title=)<br />比如对于上面的例子，就可以通过`ResourceLoader`来加载资源，而不用直接new具体的实现了
```java
//创建ResourceLoader
ResourceLoader resourceLoader = new DefaultResourceLoader();
//获取资源
Resource resource = resourceLoader.getResource("http://www.baidu.com");
```
除了`ResourceLoader`之外，还有一个`ResourcePatternResolver`可以加载资源<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984793-73ff9ea1-3561-41dc-bcd5-cb75f8e3b02c.png#averageHue=%232e2b2b&clientId=u61bda670-b95d-4&from=paste&id=u9a645e88&originHeight=573&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4ad98045-6349-47cb-a61e-2d35b232937&title=)<br />`ResourcePatternResolver`继承了`ResourceLoader`<br />通过`ResourcePatternResolver`提供的方法可以看出，他可以加载多个资源，支持使用通配符的方式，比如`classpath*:`，就可以加载所有classpath的资源。<br />`ResourcePatternResolver`只有一个实现`PathMatchingResourcePatternResolver`<br />![PathMatchingResourcePatternResolver](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984798-42f671a8-5454-47e3-a466-8cafd90faa48.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u89f4554f&originHeight=438&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua7095e02-0b90-46ea-ac43-8da2bea5ad7&title=PathMatchingResourcePatternResolver "PathMatchingResourcePatternResolver")
<a name="T0O7X"></a>
### 3、小结
到这就讲完了Spring的资源管理，这里总结一下本节大致的内容<br />Java的标准资源管理：

- `URL`
- `URLStreamHandler`

Spring的资源管理：

- 资源抽象：`Resource` 、`WritableResource`
- 资源加载：`ResourceLoader` 、`ResourcePatternResolver`

Spring的资源管理在Spring中用的很多，比如在SpringBoot中，application.yml的文件就是通过`ResourceLoader`加载成`Resource`，之后再读取文件的内容的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690958984933-14ad93e1-efc6-433b-99fc-2a5cea4f9263.png#averageHue=%23f4eee5&clientId=u61bda670-b95d-4&from=paste&id=u24deef5a&originHeight=302&originWidth=107&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4012ef09-19bb-41ca-9ae9-36be793c7c1&title=)
<a name="P7Onx"></a>
## 环境
SpringBoot配置文件是通过`ResourceLoader`来加载配置文件，读取文件的配置内容<br />那么当配置文件都加载完成之后，这个配置应该存到哪里，怎么能够读到呢？<br />这就引出了Spring框架中的一个关键概念，环境，它其实就是用于管理应用程序配置的。
<a name="QluxF"></a>
### 1、`Environment`
`Environment`就是环境抽象出来的接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959187659-ea67905f-f584-4988-99a5-dd89e5a5b1f1.png#averageHue=%23323130&clientId=u61bda670-b95d-4&from=paste&id=u3f20a742&originHeight=414&originWidth=1014&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u244f3bb3-fcb2-4367-ad7c-551fb8e1370&title=)<br />`Environment`继承`PropertyResolver`
```java
public interface PropertyResolver {

    boolean containsProperty(String key);

    String getProperty(String key);

    <T> T getProperty(String key, Class<T> targetType);

    <T> T getRequiredProperty(String key, Class<T> targetType) throws IllegalStateException;

    String resolvePlaceholders(String text);

}
```
如上是`PropertyResolver`提供的部分方法，这里简单说一下上面方法的作用

- `getProperty(String key)`，很明显是通过配置的key获取对应的value值
- `getProperty(String key, Class<T> targetType)`，这是获取配置，并转换成对应的类型，比如获取的是个字符串的"true"，这里就可以转换成布尔值的true，具体的底层实现留到下一节讲
- `resolvePlaceholders(String text)`，这类方法可以处理`${...}`占位符，也就是先取出`${...}`占位符中的key，然后再通过key获取到值

所以`Environment`主要有一下几种功能：

- 根据key获取配置
- 获取到指定类型的配置
- 处理占位符

来个demo<br />先在application.yml的配置文件中加入配置
```yaml
name: Tom
```
测试代码如下
```java
@SpringBootApplication
public class EnvironmentDemo {

    public static void main(String[] args) {
        ConfigurableApplicationContext applicationContext = SpringApplication.run(EnvironmentDemo.class, args);

        //从ApplicationContext中获取到ConfigurableEnvironment
        ConfigurableEnvironment environment = applicationContext.getEnvironment();

        //获取name属性对应的值
        String name = environment.getProperty("name");
        System.out.println("name = " + name);
    }

}
```
启动应用，获取到`ConfigurableEnvironment`对象，再获取到值<br />`ConfigurableEnvironment`是`Environment`子接口，通过命名也可以知道，他可以对`Environment`进行一些功能的配置。<br />运行结果：
```
name = Tom
```
<a name="xXEiA"></a>
### 2、配置属性源`PropertySource`
`PropertySource`是真正存配置的地方，属于配置的来源，它提供了一个统一的访问接口，使得应用程序可以以统一的方式获取配置获取到属性。<br />![PropertySource](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959187746-8a8cb32b-a924-449e-b488-9d3abfeba759.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=ufaef468c&originHeight=449&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uef778263-a465-401a-bb51-14bfcb3acaa&title=PropertySource "PropertySource")<br />来个简单demo
```java
public class PropertySourceDemo {

    public static void main(String[] args) {

        Map<String, Object> source = new HashMap<>();
        source.put("name", "Tom");

        PropertySource<Map<String, Object>> propertySource = new MapPropertySource("myPropertySource", source);

        Object name = propertySource.getProperty("name");

        System.out.println("name = " + name);
    }

}
```
简单说一下上面代码的意思

- 首先创建了一个map，就是配置来源，往里面添加了一个配置key-value
- 创建了一个`PropertySource`，使用的实现是`MapPropertySource`，需要传入配置map，所以最终获取到属性不用想就知道是从map中获取的

最后成获取到属性。<br />除了`MapPropertySource`之外，还有非常多的实现<br />![PropertySource实现](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959187648-c1253acf-ef0b-4d08-917b-9e1f1ecbcb85.png#averageHue=%23555147&clientId=u61bda670-b95d-4&from=paste&id=ufc18c974&originHeight=605&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc9ba6c4b-f013-48aa-a44e-ee502eb05fe&title=PropertySource%E5%AE%9E%E7%8E%B0 "PropertySource实现")<br />比如`CommandLinePropertySource`，它其实就封装了通过命令启动时的传递的配置参数<br />既然`PropertySource`才是真正存储配置的地方，那么`Environment`获取到的配置真正也就是从`PropertySource`获取的，并且他们其实是一对多的关系<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959187985-1ee2b201-75ea-482e-9358-0e582dbc4bf3.png#averageHue=%23f5f3ed&clientId=u61bda670-b95d-4&from=paste&id=u3e1f092e&originHeight=149&originWidth=477&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9361a497-9191-4ee3-9ee6-972c1bbbae6&title=)<br />其实很好理解一对多的关系，因为一个应用程序的配置可能来源很多地方，比如在SpringBoot环境底下，除了自定义的配置外，还有比如系统环境配置等等，这些都可以通过`Environment`获取到<br />当从`Environment`中获取配置的时候，会去遍历所有的`PropertySource`，一旦找到配置key对应的值，就会返回<br />所以，如果有多个`PropertySource`都含有同一个配置项的话，也就是配置key相同，那么获取到的配置是从排在前面的`PropertySource`的获取的<br />这就是为什么，在配置文件配置username属性时获取到的却是系统变量username对应的值，因为系统的`PropertySource`排在配置文件对应的`PropertySource`之前
<a name="KWI9D"></a>
### 3、SpringBoot是如何解析配置文件
SpringBoot是通过`PropertySourceLoader`来解析配置文件的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959187947-4cfa5dcc-c9fd-4c5a-8d96-a338b8bc12d3.png#averageHue=%232f2b2b&clientId=u61bda670-b95d-4&from=paste&id=u1afb08e7&originHeight=567&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u01134ab1-d838-4cb1-8bea-abc8a2e3e44&title=)<br />load方法的第二个参数就是前面提到的资源接口`Resource`<br />通过`Resource`就可以获取到配置文件的输入流，之后就可以读取到配置文件的内容，再把配置文件解析成多个`PropertySource`，之后把`PropertySource`放入到`Environment`中，这样就可以通过`Environment`获取到配置文件的内容了。<br />`PropertySourceLoader`默认有两个实现，分别用来解析properties和yml格式的配置文件<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959188296-721093f9-8ce9-4574-a7d9-253b3a240437.png#averageHue=%234a743b&clientId=u61bda670-b95d-4&from=paste&id=u656ddc43&originHeight=224&originWidth=738&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16064b71-c87a-4ade-955e-7be5922da5d&title=)<br />此时，上面的图就可以优化成这样<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690959188341-fe0f7847-40ae-496c-9ac3-2ad0fb110d67.png#averageHue=%23f9f7f5&clientId=u61bda670-b95d-4&from=paste&id=u355cf0c7&originHeight=361&originWidth=289&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3480c8e4-fc25-4834-9653-b41bd312437&title=)
<a name="TZ3y3"></a>
## 类型转换
`Environment`的`getProperty(String key, Class<T> targetType)`可以将配置的字符串转换成对应的类型，那么他是如何转换的呢？<br />这就要看Spring类型转换机制有关了
<a name="Rn1xe"></a>
### 1、类型转换API
Spring类型转换主要涉及到以下几个api：

- `PropertyEditor`
- `Converter`
- `GenericConverter`
- `ConversionService`
- `TypeConverter`

接下来会来详细介绍这几个api的原理和他们之间的关系。
<a name="tZyQ2"></a>
#### 1.1、`PropertyEditor`
`PropertyEditor`并不是Spring提供的api，而是JDK提供的api，他的主要作用其实就是将String类型的字符串转换成Java对象属性值。
```java
public interface PropertyEditor {

    void setValue(Object value);

    Object getValue();

    String getAsText();

    void setAsText(String text) throws java.lang.IllegalArgumentException;

}
```
就拿项目中常用的`@Value`来举例子，当通过`@Value`注解的方式将配置注入到字段时，大致步骤如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963987517-deec1ffc-1e47-45a6-b49f-bcf62464d2d1.png#averageHue=%231f1c19&clientId=u61bda670-b95d-4&from=paste&id=u41904dbe&originHeight=346&originWidth=267&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue38661ab-fd2f-4fa3-b000-add789efb41&title=)

- 取出`@Value`配置的key
- 根据`@Value`配置的key调用`Environment`的`resolvePlaceholders(String text)`方法，解析占位符，找到配置文件中对应的值
- 调用PropertyEditor将对应的值转换成注入的属性字段类型，比如注入的字段类型是数字，那么就会将字符串转换成数字

在转换的过程中，Spring会先调用`PropertyEditor`的`setAsText`方法将字符串传入，然后再调用`getValue`方法获取转换后的值。<br />Spring提供了很多`PropertyEditor`的实现，可以实现字符串到多种类型的转换<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963987634-466c6a56-8693-4cac-9151-f4aaef5ed8fd.png#averageHue=%23524e44&clientId=u61bda670-b95d-4&from=paste&id=ua8a88716&originHeight=690&originWidth=970&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue802535a-b93c-43fb-b4a7-f5e660945d5&title=)<br />在这么多实现中，有一个跟前面提到的`Resource`有关的实现`ResourceEditor`，它是将字符串转换成`Resource`对象<br />![ResourceEditor](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963987552-61818ee3-5670-4b71-a209-ec92005055bd.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=uf52bed49&originHeight=450&originWidth=1066&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u251acdbc-78f8-4181-9d16-539a14389a9&title=ResourceEditor "ResourceEditor")<br />也就是说，可以直接通过`@Value`的方式直接注入一个`Resource`对象，就像下面这样
```java
@Value("http://www.baidu.com")
private Resource resource;
```
其实归根到底，底层也是通过`ResourceLoader`来加载的，这个结论是不变的。<br />所以，如果想知道`@Value`到底支持注入哪些字段类型的时候，看看`PropertyEditor`的实现就可以了，当然如果Spring自带的都不满足要求，可以自己实现`PropertyEditor`，比如把String转成Date类型，Spring就不支持。
<a name="zjtYi"></a>
#### 1.2、`Converter`
由于`PropertyEditor`局限于字符串的转换，所以Spring在后续的版本中提供了叫`Converter`的接口，他也用于类型转换的，相比于`PropertyEditor`更加灵活、通用<br />![Converter](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963987520-428209c0-8d51-4c2d-b181-8d1a7d2b101e.png#averageHue=%232c2c2b&clientId=u61bda670-b95d-4&from=paste&id=ud166b496&originHeight=538&originWidth=1008&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua5a71df8-4ff5-454b-b935-0fed51931fd&title=Converter "Converter")<br />`Converter`是个接口，泛型S是被转换的对象类型，泛型T是需要被转成的类型。<br />同样地，Spring也提供了很多`Converter`的实现<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963987540-1f5abaea-a415-4ccb-8de6-52ac012630c4.png#averageHue=%23555146&clientId=u61bda670-b95d-4&from=paste&id=ucdbc5923&originHeight=640&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4a31e577-c601-4572-a9c2-6a0735176e4&title=)<br />这些主要包括日期类型的转换和String类型转换成其它的类型
<a name="yFAkY"></a>
#### 1.3、`GenericConverter`
`GenericConverter`也是类型转换的接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988086-2b88246a-f3d5-4823-b805-1fe519473163.png#averageHue=%232c2b2b&clientId=u61bda670-b95d-4&from=paste&id=ue45ec7cc&originHeight=482&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0d912f08-f4e5-408d-a74d-67b32026046&title=)<br />这个接口的主要作用是可以处理带有泛型类型的转换，主要的就是面向集合数组转换操作，从Spring默认提供的实现就可以看出<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988186-f6c76c54-a5f4-4a55-80fc-8230bc098dc1.png#averageHue=%23555148&clientId=u61bda670-b95d-4&from=paste&id=u16648bba&originHeight=733&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf48e59fe-bc01-4bcc-9364-8ee8bc4c6ec&title=)<br />那`Converter`跟`GenericConverter`有什么关系呢？<br />这里举个例子，假设现在需要将源集合`Collection<String>`转换成目标集合`Collection<Date>`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988182-7695b1f1-2515-42ae-9c66-1718400be278.png#averageHue=%23f2eee8&clientId=u61bda670-b95d-4&from=paste&id=u4cae3bba&originHeight=61&originWidth=301&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud2b219f0-6752-4ad2-9d18-6781b6f998d&title=)<br />假设现在有个String转换成Date类型的`Converter`，咱就叫`StringToDateConverter`，那么整个转换过程如下：

- 首先会找到`GenericConverter`的一个实现`CollectionToCollectionConverter`，从名字也可以看出来，是将一个几个转换成另一个集合
- 然后遍历源集合`Collection<String>`，取出元素
- 根据目标集合泛型Date，找到`StringToDateConverter`，将String转换成Date，将转换的Date存到一个新的集合
- 返回这个新的集合，这样就实现了集合到集合的转换

所以通过这就可以看出`Converter`和`GenericConverter`其实是依赖关系
<a name="AjWUe"></a>
#### 1.4、`ConversionService`
对于使用者来说，不论是`Converter`还是`GenericConverter`，其实都是类型转换的，并且类型转换的实现也很多，所以Spring为了方便使用`Converter`还是`GenericConverter`，提供了一个门面接口`ConversionService`<br />![ConversionService](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988155-7bcd7ab7-77fc-4bb7-b413-080853784cbe.png#averageHue=%23313130&clientId=u61bda670-b95d-4&from=paste&id=u8cd42771&originHeight=410&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub6057b5c-916a-4196-986c-c4f02eb1571&title=ConversionService "ConversionService")<br />可以直接通过`ConversionService`来进行类型转换，而不需要面向具体的`Converter`或者是`GenericConverter`<br />`ConversionService`有一个基本的实现`GenericConversionService`<br />![GenericConversionService](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988194-33a65de9-860f-40d5-8ec6-1303faaed471.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=uc219ee7d&originHeight=451&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uac56537e-cd71-4b8d-8ff0-b8994e3cd27&title=GenericConversionService "GenericConversionService")<br />同时`GenericConversionService`还实现了`ConverterRegistry`的接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988548-0ebeef5e-6e22-450d-a54d-bd7d639d52f5.png#averageHue=%23373230&clientId=u61bda670-b95d-4&from=paste&id=uedaa70ac&originHeight=526&originWidth=924&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u41caee49-84ea-4490-9a88-ad9e7b7b19e&title=)<br />`ConverterRegistry`提供了对`Converter`和`GenericConverter`进行增删改查的方法。<br />![ConverterRegistry](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988668-29002f5c-d99b-4011-a631-b486030f5671.png#averageHue=%23302f2f&clientId=u61bda670-b95d-4&from=paste&id=nEQui&originHeight=357&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uda557d92-d6e0-4264-982f-d7c19670f67&title=ConverterRegistry "ConverterRegistry")<br />这样就可以往`ConversionService`中添加`Converter`或者是`GenericConverter`了，因为最终还是通过`Converter`和`GenericConverter`来实现转换的<br />但是一般不直接用`GenericConversionService`，而是用`DefaultConversionService`或者是`ApplicationConversionService`（SpringBoot环境底下使用）<br />因为`DefaultConversionService`和`ApplicationConversionService`在创建的时候，会添加很多Spring自带的`Converter`和`GenericConverter`，就不需要手动添加了。
<a name="nJdwl"></a>
#### 1.5、`TypeConverter`
`TypeConverter`其实也是算是一个门面接口，他也定义了转换方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988733-d0481c97-2cbe-44d9-8176-79749a524b33.png#averageHue=%232e2d2b&clientId=u61bda670-b95d-4&from=paste&id=uf8a66a0f&originHeight=391&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucd3e35d5-b1fa-4050-8bf7-4f79ee55117&title=)<br />他是将`PropertyEditor`和`ConversionService`进行整合，方便同时使用`PropertyEditor`和`ConversionService`<br />`convertIfNecessary`方法会去调用`PropertyEditor`和`ConversionService`进行类型转换，值得注意的是，**优先使用**`**PropertyEditor**`**进行转换，如果没有找到对应的**`**PropertyEditor**`**，会使用**`**ConversionService**`**进行转换**<br />`TypeConverter`有个简单的实现`SimpleTypeConverter`，这里来个简单的demo
```java
public class TypeConverterDemo {

    public static void main(String[] args) {
        SimpleTypeConverter typeConverter = new SimpleTypeConverter();

        //设置ConversionService
        typeConverter.setConversionService(DefaultConversionService.getSharedInstance());

        //将字符串"true"转换成Boolean类型的true
        Boolean b = typeConverter.convertIfNecessary("true", Boolean.class);
        System.out.println("b = " + b);
    }
}
```
这里需要注意，`ConversionService`需要手动设置，但是`PropertyEditor`不需要，因为`SimpleTypeConverter`默认会去添加`PropertyEditor`的实现。
<a name="zYsvF"></a>
#### 小结
到这就讲完了类型转换的常见的几个api，这里再简单总结一下：

- `PropertyEditor`：String转换成目标类型
- `Converter`：用于一个类型转换成另一个类型
- `GenericConverter`：用于处理泛型的转换，主要用于集合
- `ConversionService`：门面接口，内部会调用`Converter`和`GenericConverter`
- `TypeConverter`：门面接口，内部会调用`PropertyEditor`和`ConversionService`

画张图来总结他们之间的关系<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988741-b2bf92dd-0ca9-471d-a437-da57292bb61c.png#averageHue=%23f7f0e8&clientId=u61bda670-b95d-4&from=paste&id=u2afb629b&originHeight=181&originWidth=461&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2e9dea48-cc21-4567-9152-8d0f3f9adc6&title=)<br />前面在举`@Value`的例子时说，类型转换是根据`PropertyEditor`来的，其实只说了一半，因为底层实际上是根据`TypeConverter`来转换的，所以`@Value`类型转换时也能使用`ConversionService`类转换，所以那张图实际上应该这么画才算对<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963988962-c38fa2ce-7c0d-4cc1-93d5-02969ad28266.png#averageHue=%23131110&clientId=u61bda670-b95d-4&from=paste&id=u0cd50596&originHeight=345&originWidth=429&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2ece5d38-d47e-444f-870c-2e41ee26ca8&title=)
<a name="XlB7p"></a>
### 2、`Environment`中到底是如何进行类型转换的？
`Environment`中到底是如何进行类型转换的，看看`Environment`类的接口体系<br />`Environment`有个子接口`ConfigurableEnvironment`中，前面也提到过<br />它继承了`ConfigurablePropertyResolver`接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963989023-3c65e677-e55a-4f81-884b-ed8e24876696.png#averageHue=%2334302f&clientId=u61bda670-b95d-4&from=paste&id=ue861ebe5&originHeight=444&originWidth=920&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u72bc763b-b7f9-44a8-ac37-4f89885126c&title=)<br />而`ConfigurablePropertyResolver`有一个`setConversionService`方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690963989225-fcdf257f-8e2f-4199-b305-076beb0e534b.png#averageHue=%233a3432&clientId=u61bda670-b95d-4&from=paste&id=u637bf309&originHeight=381&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue7602208-4786-4318-9b91-56fa0fb5007&title=)<br />所以从这可以看出，`Environment`底层实际上是通过`ConversionService`实现类型转换的<br />这其实也就造成了一个问题，因为`ConversionService`和`PropertyEditor`属于并列关系，那么就会导致`Environment`无法使用`PropertyEditor`来进行类型转换，也就会丧失部分Spring提供的类型转换功能，就比如无法通过`Environment`将String转换成`Resource`对象，因为Spring没有实现String转换成`Resource`的`Converter`<br />当然可以自己实现一个String转换成`Resource`的`Converter`，然后添加到`ConversionService`，之后`Environment`就支持String转换成`Resource`了。
<a name="QofFK"></a>
## 数据绑定
既然提到了类型转换，那么就不得不提到数据绑定了，他们是密不可分的，因为在数据绑定时，往往都会伴随着类型转换，<br />数据绑定的意思就是将一些配置属性跟Bean对象的属性进行绑定。<br />在ssm时代，一般通过xml方式声明Bean的时候，可以通过`<property/>`来设置Bean的属性
```xml
<bean class="com.sanyou.spring.core.basic.User">
  <property name="username" value="Tom"/>
</bean>
```
```java
@Data
public class User {
    private String username;
}
```
然后Spring在创建User的过程中，就会给username属性设置为Tom。<br />这就是数据绑定，将Tom绑定到username这个属性上。<br />数据绑定的核心api主要包括以下几个：

- `PropertyValues`
- `BeanWrapper`
- `DataBinder`
<a name="odIB6"></a>
#### 1、`PropertyValues`
这里先来讲一下`PropertyValue`（注意没有s）<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964537950-55a53b99-34e5-4ac6-82fd-25bcc4345bc8.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=ua476c157&originHeight=627&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc2de4946-5c32-4e28-bc4c-8ab8b0b9ac7&title=)<br />顾明思议，`PropertyValue`就是就是封装了属性名和对应的属性值，它就是数据绑定时属性值的来源。<br />以前面的提到的xml创建Bean为例，Spring在启动的时候会去解析xml中的`<property/>`标签，然后将name和value封装成`PropertyValue`<br />当创建User这个Bean的时候，到了属性绑定的阶段的时候，就会取出`PropertyValue`，设置到User的username属性上。<br />而`PropertyValues`，比`PropertyValue`多了一个s，也就是复数的意思，所以其实`PropertyValues`本质上就是`PropertyValue`的一个集合<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964537912-d88e8711-4ff4-4e5f-8096-a481fc13e03e.png#averageHue=%23332d2b&clientId=u61bda670-b95d-4&from=paste&id=u8721b340&originHeight=373&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u772dee09-f795-4a5a-9a4d-cade6642ba7&title=)<br />因为一个Bean可能有多个属性配置，所以就用`PropertyValues`来保存。
<a name="su03T"></a>
#### 2、`BeanWrapper`
`BeanWrapper`其实就数据绑定的核心api了，因为在Spring中涉及到数据绑定都是通过`BeanWrapper`来完成的，比如前面提到的Bean的属性的绑定，就是通过`BeanWrapper`来的<br />`BeanWrapper`是一个接口，他有一个唯一的实现`BeanWrapperImpl`。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964537918-0e7c1735-c9df-4556-8a77-22945329f13c.png#averageHue=%232b2b2b&clientId=u61bda670-b95d-4&from=paste&id=u4f9c9b74&originHeight=391&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u76718f3e-dcac-4cd9-8e21-56df5d305c0&title=)<br />先来个demo
```java
public class BeanWrapperDemo {

    public static void main(String[] args) {
        //创建user对象
        User user = new User();

        //创建BeanWrapper对象，把需要进行属性绑定的user对象放进去
        BeanWrapper beanWrapper = new BeanWrapperImpl(user);

        //进行数据绑定，将Tom这个属性值赋值到username这个属性上
        beanWrapper.setPropertyValue(new PropertyValue("username", "Tom"));

        System.out.println("username = " + user.getUsername());
    }

}
```
成功获取到，说明设置成功<br />`BeanWrapperImpl`也间接实现了`TypeConverter`接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964537929-dcbdc86e-031f-469b-8c87-0699fb12ff55.png#averageHue=%2332312f&clientId=u61bda670-b95d-4&from=paste&id=u6e3af972&originHeight=741&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u98f852bf-7470-4456-bac8-6e24dfd3453&title=)<br />当然底层还是通过前面提到的`ConversionService`和`PropertyEditor`实现的<br />所以当配置的类型跟属性的类型不同时，就可以对配置的类型进行转换，然后再绑定到属性上<br />这里简单说一下数据绑定和`@Value`的异同，因为这两者看起来好像是一样的，但实际还是有点区别的<br />相同点：

- 两者都会涉及到类型转换，`@Value`和数据绑定都会将值转换成目标属性对应的类型，并且都是通过`TypeConverter`来转换的

不同点：

- 1、发生时机不同，`@Value`比数据绑定更早，当`@Value`都注入完成之后才会发生数据绑定(属性赋值)
- 2、属性赋值方式不同，`@Value`是通过反射来的，而是数据绑定是通过setter方法来的，如果没有setter方法，属性是没办法绑定的
<a name="LIZdC"></a>
#### 3、`DataBinder`
`DataBinder`也是用来进行数据绑定的，它的底层也是间接通过`BeanWrapper`来实现的数据绑定的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964538495-b56c8d92-fe92-463c-bdcc-3f93d60c8ead.png#averageHue=%232c2b2b&clientId=u61bda670-b95d-4&from=paste&id=u50db4aa7&originHeight=407&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc073fe58-926d-4c90-8924-6f4f2378fb9&title=)<br />但是他相比于`BeanWrapper`多了一些功能，比如在数据绑定之后，可以对数据校验，比如可以校验字段的长度等等<br />说到数据校验，是不是想到了SpringMVC中的参数校验，通过`@Valid`配合一些诸如`@NotBlank`、`@NotNull`等注解，实现优雅的参数校验。<br />其实SpringMVC的参数校验就是通过`DataBinder`来的，所以`DataBinder`其实在SpringMVC中用的比较多，但是在Spring中确用的很少。<br />如果有兴趣，可以翻一下SpringMVC中关于请求参数处理的`HandlerMethodArgumentResolver`的实现，里面有的实现会用到`DataBinder`（`WebDataBinder`）来进行数据请求参数跟实体类的数据绑定、类型转换、数据校验等等。<br />平时写接口的时候，前端传来的参数String类型的时间字符串无法通过Spring框架本身转换成Date类型，有部分原因就是前面提到的Spring没有相关的`Converter`实现<br />总的来说，数据绑定在xml配置和SpringMVC中用的比较多的，并且数据绑定也是Spring Bean生命周期中一个很重要的环节。
<a name="iFDb0"></a>
## 泛型处理
Spring为了方便操作和处理泛型类型，提供了一个强大的工具类——`ResolvableType`。<br />泛型处理其实是一块相对独立的东西，因为它就只是一个工具类，只还不过这个工具类在Spring中却是无处不在！<br />`ResolvableType`提供了有一套灵活的API，可以在运行时获取和处理泛型类型等信息。<br />![ResolvableType](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964741249-4c034efc-0fa3-49c3-a099-731eb93ab39b.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u897949f6&originHeight=604&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc867ecdd-a51c-4293-9335-2c1be4bb953&title=ResolvableType "ResolvableType")<br />接下来就通过一个案例，来看一看如何通过`ResolvableType`快速简单的获取到泛型的<br />首先声明了一个MyMap类，继承HashMap，第一个泛型参数是Integer类型，第二个泛型参数是List类型，List的泛型参数又是String
```java
public class MyMap extends HashMap<Integer, List<String>> {

}
```
接下来就来演示一下如何获取到HashMap的泛型参数以及List的泛型参数<br />第一步，先来通过`ResolvableType#forClass`方法创建一个MyMap类型对应的`ResolvableType`
```java
//创建MyMap对应的ResolvableType
ResolvableType myMapType = ResolvableType.forClass(MyMap.class);
```
因为泛型参数是在父类HashMap中，所以得获取到父类HashMap对应的`ResolvableType`，通过`ResolvableType#getSuperType()`方法获取
```java
//获取父类HashMap对应的ResolvableType
ResolvableType hashMapType = myMapType.getSuperType();
```
接下来需要获取`HashMap`的泛型参数对应的`ResolvableType`类型，可以通过`ResolvableType#getGeneric(int... indexes)`就可以获取指定位置的泛型参数`ResolvableType`，方法参数就是指第几个位置的泛型参数，从0开始<br />比如获取第一个位置的对应的`ResolvableType`类型
```java
//获取第一个泛型参数对应的ResolvableType
ResolvableType firstGenericType = hashMapType.getGeneric(0);
```
现在有了第一个泛型参数的`ResolvableType`类型，只需要通过`ResolvableType#resolve()`方法就可以获取到`ResolvableType`类型对应的class类型，这样就可以获取到一个泛型参数的class类型
```java
//获取第一个泛型参数对应的ResolvableType对应的class类型，也就是Integer的class类型
Class<?> firstGenericClass = firstGenericType.resolve();
```
如果想获取到HashMap第二个泛型参数的泛型类型，也就是List泛型类型就可以这么写
```java
//HashMap第二个泛型参数的对应的ResolvableType，也就是List<String>
ResolvableType secondGenericType = hashMapType.getGeneric(1);
//HashMap第二个泛型参数List<String>的第一个泛型类型String对应的ResolvableType
ResolvableType secondFirstGenericType = secondGenericType.getGeneric(0);
//这样就获取到了List<String>的泛型类型String
Class<?> secondFirstGenericClass = secondFirstGenericType.resolve();
```
从上面的演示下来可以发现，其实每变化一步，其实就是获取对应泛型或者是父类等等对应的`ResolvableType`，父类或者是泛型参数又可能有泛型之类的，只需要一步一步获取就可以了，当需要获取到具体的class类型的时候，通过`ResolvableType#resolve()`方法就行了。<br />除了上面提到的通过`ResolvableType#forClass`方法创建`ResolvableType`之外，还可以通过一下几个方法创建：

- `forField(Field field)`：获取字段类型对应的`ResolvableType`
- `forMethodReturnType(Method method)`：获取方法返回值类型对应的`ResolvableType`
- `forMethodParameter(Method method, int parameterIndex)`：获取方法某个位置方法参数对应的`ResolvableType`
- `forConstructorParameter(Constructor<?> constructor, int parameterIndex)`：获取构造方法某个构造参数对应的`ResolvableType`

通过上面解释可以看出，对于一个类方法参数，方法返回值，字段等等都可以获取到对应的`ResolvableType`
<a name="NYEW1"></a>
## 国际化
国际化（Internationalization，简称i18n）也是Spring提供的一个核心功能，它其实也是一块相对独立的功能。<br />所谓的国际化，其实理解简单点就是对于不同的地区国家，输出的文本内容语言不同。<br />Spring的国际化其实主要是依赖Java中的国际化和文本处理方式。
<a name="U6Dca"></a>
### 1、Java中的国际化
<a name="QLGXm"></a>
#### `Locale`
`Locale`是Java提供的一个类，它可以用来标识不同的语言和地区，如en_US表示美国英语，zh_CN表示中国大陆中文等。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869132-57589e95-2e79-4c3a-86f3-703b171d092d.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u86225fd1&originHeight=396&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u65f6ea44-2118-4b9c-b65f-96a84dad30c&title=)<br />目前Java已经穷举了很多国家的地区`Locale`。<br />可以使用`Locale`类获取系统默认的`Locale`，也可以手动设置`Locale`，以适应不同的语言环境。
<a name="kGcfB"></a>
#### `ResourceBundle`
`ResourceBundle`是一个加载本地资源的一个类，他可以根据传入的Locale不同，加载不同的资源。<br />来个demo<br />首先准备资源文件，资源文件通常是.properties文件，文件名命名规则如下：<br />**basename_lang_country.properties**<br />basename无所谓，叫什么都可以，而lang和country是从Locale中获取的。<br />举个例子，看看英语地区的Locale<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869074-83746dc5-8063-4ffe-b475-a8d59f079771.png#averageHue=%23312c2b&clientId=u61bda670-b95d-4&from=paste&id=uf6e01999&originHeight=168&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3264184d-6378-4a17-a653-96501776f1e&title=)<br />从上图可以看出，英语Locale的lang为en，country为空字符串，那么此时英语地区对应资源文件就可以命名为：basename_en.properties，由于country为空字符串，可以省略<br />中国大陆Locale如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869096-ad41116c-d446-4b86-9111-668d61147d3f.png#averageHue=%232c2b2b&clientId=u61bda670-b95d-4&from=paste&id=ufad88d40&originHeight=143&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue60ef4ae-e414-4d02-94cb-dd8ce20b064&title=)<br />此时文件就可以命为：basename_zh_CN.properties<br />好了，现在既然知道了命名规则，就创建两个文件，basename就叫message，一个英语，一个中文，放在classpath路径下<br />中文资源文件：message_zh_CN.properties，内容为：
```
name=汤姆
```
英文资源文件：message_en.properties，内容为：
```
name=Tom
```
有了文件之后，就可以通过`ResourceBundle#getBundle(String baseName,Locale locale)`方法来获取获取`ResourceBundle`

- 第一个参数baseName就是文件名中的basename，对于demo来说，就是message
- 第二个参数就是地区，根据地区的不同加载不同地区的文件

测试一下
```java
public class ResourceBundleDemo {

    public static void main(String[] args) {

        //获取ResourceBundle，第一个参数baseName就是文件名称，第二个参数就是地区
        ResourceBundle chineseResourceBundle = ResourceBundle.getBundle("message", Locale.SIMPLIFIED_CHINESE);
        //根据name键取值
        String chineseName = chineseResourceBundle.getString("name");
        System.out.println("chineseName = " + chineseName);

        ResourceBundle englishResourceBundle = ResourceBundle.getBundle("message", Locale.ENGLISH);
        String englishName = englishResourceBundle.getString("name");
        System.out.println("englishName = " + englishName);

    }

}
```
其实运行结果成功获取了，只不过中文乱码了，这主要是因为`ResourceBundle`底层其实编码是ISO-8859-1，所以会导致乱码。<br />解决办法最简单就是把中文用Java Unicode序列来表示，之后就可以读出中文了。<br />除了这种方式之外，其实还可以继承`ResourceBundle`内部一个`Control`类<br />![Control](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869100-e339946d-2e22-47f6-9a07-b191daf6792e.png#averageHue=%2365412e&clientId=u61bda670-b95d-4&from=paste&id=udd982990&originHeight=438&originWidth=992&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u35b3f483-0638-4dba-a6d8-5f6bbaeaab6&title=Control "Control")<br />重写`newBundle`方法<br />![newBundle](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869602-be6563a1-e0f8-4b8c-9a32-5b7c59d42503.png#averageHue=%232e2c2b&clientId=u61bda670-b95d-4&from=paste&id=u504cff21&originHeight=841&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u158bd110-ffeb-463e-8f56-9880d664325&title=newBundle "newBundle")<br />`newBundle`是创建`ResourceBundle`对应核心方法，重写的时候就可以随心所欲让它支持其它编码方式。<br />有了新的`Control`之后，获取`ResourceBundle`时只需要通过`ResourceBundle#getBundle(String baseName, Locale targetLocale,Control control)`方法指定`Control`就可以了。<br />Spring实际上就是通过这种方式扩展，支持不同编码的，后面也有提到。
<a name="Un9de"></a>
#### `MessageFormat`
`MessageFormat`顾明思议就是把消息格式化。它可以接收一条包含占位符的消息模板，并根据**提供的参数**替换**占位符**，生成最终的消息。<br />`MessageFormat`对于将动态值插入到消息中非常有用，如欢迎消息、错误消息等。<br />先来个Demo
```java
public class MessageFormatDemo {

    public static void main(String[] args) {
        String message = MessageFormat.format("你好：{0}", "张三");
        System.out.println("message = " + message);
    }

}
```
解释一下上面这段代码：

- 你好：{0}其实就是前面提到的消息的模板，{0}就是**占位符**，中间的0代表消息格式化的时候将**提供的参数**第一个参数替换占位符的值
- 张三就是提供的参数，可以写很多个，但是demo只会取第一个参数，因为是{0}

所以输出结果为：
```
message = 你好：张三
```
成功格式化消息。
<a name="EYmEU"></a>
### 2、Spring国际化
Spring提供了一个国际化接口`MessageSource`<br />![MessageSource](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869738-4a7c47a3-ed26-4572-87a8-c20084a48ba5.png#averageHue=%2331312f&clientId=u61bda670-b95d-4&from=paste&id=ub46e600d&originHeight=348&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u09e1bd2e-e121-4556-a232-0654ef5d845&title=MessageSource "MessageSource")<br />他有一个基于`ResourceBundle` + `MessageFormat`的实现`ResourceBundleMessageSource`<br />![ResourceBundleMessageSource](https://cdn.nlark.com/yuque/0/2023/png/396745/1690964869807-1c74192a-5ac5-469a-843a-e835892b9c88.png#averageHue=%232c2b2b&clientId=u61bda670-b95d-4&from=paste&id=u033f2664&originHeight=523&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ucfb3f290-09c7-4c33-9b9d-6de73d192ea&title=ResourceBundleMessageSource "ResourceBundleMessageSource")<br />他的本质可以在资源文件存储**消息的模板**，然后通过`MessageFormat`来替换**占位符**，`MessageSource`的`getMessage`方法就可以传递具体的参数<br />来个demo<br />现在模拟登录欢迎语句，对于不同的人肯定要有不同的名字，所以资源文件需要存模板，需要在不同的资源文件加不同的模板<br />中文资源文件：message_zh_CN.properties
```
welcome=您好:{0}
```
英文资源文件：message_en.properties
```
welcome=hello:{0}
```
占位符，就是不同人不同名字<br />测试代码
```java
public class MessageSourceDemo {

    public static void main(String[] args) {
        ResourceBundleMessageSource messageSource = new ResourceBundleMessageSource();

        //Spring已经扩展了ResourceBundle的Control，支持资源文件的不同编码方式，但是需要设置一下
        messageSource.setDefaultEncoding("UTF-8");

        //添加 baseName，就是前面提到的文件中的basename
        messageSource.addBasenames("message");

        //中文，传个中文名字
        String chineseWelcome = messageSource.getMessage("welcome", new Object[]{"张三"}, Locale.SIMPLIFIED_CHINESE);
        System.out.println("chineseWelcome = " + chineseWelcome);

        //英文，英语国家肯定是英文名
        String englishWelcome = messageSource.getMessage("welcome", new Object[]{"Bob"}, Locale.ENGLISH);
        System.out.println("englishWelcome = " + englishWelcome);
    }

}
```
运行结果
```
chineseWelcome = 您好:张三
englishWelcome = hello:Bob
```
成功根据完成不同国家资源的加载和模板消息的格式化。
<a name="pTZIz"></a>
### 小结
这里来简单总结一下

- `Locale`：不同国家和地区的信息封装
- `ResourceBundle`：根据不同国家的Locale，加载对应的资源文件，这个资源文件的命名需要遵守basename_lang_country.properties命名规范
- `MessageFormat`：其实就是一个文本处理的方式，他可以解析模板，根据参数替换模板的占位符
- `MessageSource`：Spring提供的国际化接口，其实他底层主要是依赖Java的`ResourceBundle`和`MessageFormat`，资源文件存储模板信息，`MessageFormat`根据`MessageSource`方法的传参替换模板中的占位符
<a name="x5NbQ"></a>
## `BeanFactory`
Spring的核心就是IOC和AOP，而`BeanFactory`就是大名鼎鼎的IOC容器，他可以生产对象。
<a name="VuKXi"></a>
### 1、`BeanFactory`接口体系
`BeanFactory`本身是一个接口<br />![BeanFactory](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762143-518c4f0e-8124-476e-8ece-225b22a6fbf1.png#averageHue=%23323231&clientId=u61bda670-b95d-4&from=paste&id=u98b8164a&originHeight=529&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufa7c14ff-b630-4746-adc5-ec2e7f4f7fd&title=BeanFactory "BeanFactory")<br />从上面的接口定义可以看出从可以从`BeanFactory`获取到Bean。<br />他也有很多子接口，不同的子接口有着不同的功能

- `ListableBeanFactory`
- `HierarchicalBeanFactory`
- `ConfigurableBeanFactory`
- `AutowireCapableBeanFactory`
<a name="bMpaB"></a>
#### `ListableBeanFactory`
![ListableBeanFactory](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762113-6a371b0d-2b46-46e6-a045-475c54a06bb0.png#averageHue=%23716043&clientId=u61bda670-b95d-4&from=paste&id=ue98023ca&originHeight=403&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf08beda1-54a1-48f9-b2c3-d4e577802ff&title=ListableBeanFactory "ListableBeanFactory")<br />从提供的方法可以看出，提供了一些获取集合的功能，比如有的接口可能有多个实现，通过这些方法就可以获取这些实现对象的集合。
<a name="T86YL"></a>
#### `HierarchicalBeanFactory`
![HierarchicalBeanFactory](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762147-557d14fa-22a1-40d9-a3e5-c30409b88e06.png#averageHue=%232c2c2b&clientId=u61bda670-b95d-4&from=paste&id=ueea91a0c&originHeight=563&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ucaaf4783-9a60-467b-a992-fb36c314507&title=HierarchicalBeanFactory "HierarchicalBeanFactory")<br />从接口定义可以看出，可以获取到父容器，说明`BeanFactory`有子父容器的概念。
<a name="YCJgN"></a>
#### `ConfigurableBeanFactory`
![ConfigurableBeanFactory](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762138-6d904c0a-6d35-4caa-bb9e-8253934303eb.png#averageHue=%23393331&clientId=u61bda670-b95d-4&from=paste&id=u53c67d21&originHeight=545&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6d1b71e5-82b3-4b72-b72d-24f50c46e63&title=ConfigurableBeanFactory "ConfigurableBeanFactory")<br />从命名可以看出，可配置`BeanFactory`，所以可以对`BeanFactory`进行配置，比如截图中的方法，可以设置前面提到的类型转换的东西，这样在生成Bean的时候就可以类型属性的类型转换了。
<a name="OBm0d"></a>
#### `AutowireCapableBeanFactory`
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762153-eda1fcc3-9508-4005-b71a-30f00fdc46f3.png#averageHue=%23363533&clientId=u61bda670-b95d-4&from=paste&id=uea872d45&originHeight=401&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5837fb99-2ebc-4472-a0c2-71608c040e6&title=)<br />提供了自动装配Bean的实现、属性填充、初始化、处理获取依赖注入对象的功能。<br />比如`@Autowired`最终就会调用`AutowireCapableBeanFactory#resolveDependency`处理注入的依赖。<br />其实从这里也可以看出，Spring在`BeanFactory`的接口设计上面还是基于不同的职责进行接口的划分，其实不仅仅是在`BeanFactory`，前面提到的那些接口也基本符合这个原则。
<a name="PCmXv"></a>
### 2、`BeanDefinition`及其相关组件
<a name="qsuJH"></a>
#### `BeanDefinition`
`BeanDefinition`是Spring Bean创建环节中很重要的一个东西，它封装了Bean创建过程中所需要的元信息。
```java
public interface BeanDefinition extends AttributeAccessor, BeanMetadataElement {
    //设置Bean className
    void setBeanClassName(@Nullable String beanClassName);

    //获取Bean className
    @Nullable
    String getBeanClassName();

    //设置是否是懒加载
    void setLazyInit(boolean lazyInit);

    //判断是否是懒加载
    boolean isLazyInit();

    //判断是否是单例
    boolean isSingleton();

}
```
如上代码是`BeanDefinition`接口的部分方法，从这方法的定义名称可以看出，一个Bean所创建过程中所需要的一些信息都可以从`BeanDefinition`中获取，比如这个Bean的class类型，这个Bean是否是懒加载，这个Bean是否是单例的等等，因为有了这些信息，Spring才知道要创建一个什么样的Bean。
<a name="SGggB"></a>
#### 读取`BeanDefinition`
读取`BeanDefinition`大致分为以下几类

- `BeanDefinitionReader`
- `ClassPathBeanDefinitionScanner`

`**BeanDefinitionReader**`<br />![BeanDefinitionReader](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762698-e684468e-c77d-41c9-8c92-1920d19ec3f9.png#averageHue=%2334312f&clientId=u61bda670-b95d-4&from=paste&id=uc35b19f7&originHeight=739&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u814f8fda-7de1-41fe-acd7-078f584885d&title=BeanDefinitionReader "BeanDefinitionReader")<br />`BeanDefinitionReader`可以通过`loadBeanDefinitions(Resource resource)`方法来加载`BeanDefinition`，方法参数就是前面说的资源，比如可以将Bean定义在xml文件中，这个xml文件就是一个资源<br />`BeanDefinitionReader`的相关实现：

- `XmlBeanDefinitionReader`：读取xml配置的Bean
- `PropertiesBeanDefinitionReader`：读取properties文件配置的Bean，Bean可以定义在properties文件配置中
- `AnnotatedBeanDefinitionReader`：读取通过注解定义的Bean，比如`@Lazy`注解等等，`AnnotatedBeanDefinitionReader`不是`BeanDefinitionReader`的实现，但是作用是一样的

`**ClassPathBeanDefinitionScanner**`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965763026-79303030-c878-411b-a4e2-6f4bbdd4bd33.png#averageHue=%232c2c2b&clientId=u61bda670-b95d-4&from=paste&id=ub99eb104&originHeight=507&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc4665006-542b-4f77-b4fa-372e3671e0a&title=)<br />这个作用就是扫描指定包下通过`@Component`及其派生注解（`@Service`等等）注解定义的Bean，其实就是`@ComponentScan`注解的底层实现<br />`ClassPathBeanDefinitionScanner`这个类其实在很多其它框架中都有使用到，因为这个类可以扫描指定包下，生成`BeanDefinition`，对于那些需要扫描包来生成`BeanDefinition`来说，用的很多<br />比如说常见的MyBatis框架，他的注解`@MapperScan`可以扫描指定包下的`Mapper`接口，其实他也是通过继承`ClassPathBeanDefinitionScanner`来扫描`Mapper`接口的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965763017-6812144e-8cae-458d-b220-8b02e15cef21.png#averageHue=%23302d2c&clientId=u61bda670-b95d-4&from=paste&id=uad932218&originHeight=360&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud1612c68-80bb-4c48-af55-a6cf5de281e&title=)<br />`**BeanDefinitionRegistry**`<br />这个从命名就可以看出，是`BeanDefinition`的注册中心，也就是用来保存`BeanDefinition`的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965763017-22cf1d2c-6512-4d21-8efa-b8505ee4252c.png#averageHue=%23323130&clientId=u61bda670-b95d-4&from=paste&id=uf8ec9d63&originHeight=448&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7fc85670-1998-4b2a-8e9c-5953692fab4&title=)<br />提供了`BeanDefinition`的增删查的功能。<br />可以用一张图来把前面提到东西关联起来<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965762980-cf2c25ed-1ef6-4267-847f-85284c69ce27.png#averageHue=%23f8f6f4&clientId=u61bda670-b95d-4&from=paste&id=ua2ccd483&originHeight=331&originWidth=1051&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uee54faec-33ca-4e00-8258-265e666820c&title=)

- 通过`BeanDefinitionReader`或者是`ClassPathBeanDefinitionScanner`为每一个Bean生成一个`BeanDefinition`
- `BeanDefinition`生成之后，添加到`BeanDefinitionRegistry`中
- 当从`BeanFactory`中获取Bean时，会从`BeanDefinitionRegistry`中拿出需要创建的Bean对应的`BeanDefinition`，根据`BeanDefinition`的信息来生成Bean
- 当生成的Bean是单例的时候，Spring会将Bean保存到`SingletonBeanRegistry`中，也就是平时说的三级缓存中的第一级缓存中，以免重复创建，需要使用的时候直接从`SingletonBeanRegistry`中查找
<a name="Oc5da"></a>
### 3、`BeanFactory`核心实现
前面提到的`BeanFactory`体系都是一个接口，那么`BeanFactory`的实现类是哪个类呢？<br />`BeanFactory`真正底层的实现类，其实就只有一个，那就是`DefaultListableBeanFactory`这个类，这个类以及父类真正实现了`BeanFactory`及其子接口的所有的功能。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965763842-84cd4a3f-df24-4d8f-89f0-c694f39cabe0.png#averageHue=%232f2e2d&clientId=u61bda670-b95d-4&from=paste&id=ub3b8e274&originHeight=442&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u06f6d6c0-055c-4197-b6f6-0b84808edcb&title=)<br />并且接口的实现上可以看出，他也实现了`BeanDefinitionRegistry`，也就是说，在底层的实现上，其实`BeanFactory`跟`BeanDefinitionRegistry`的实现是同一个实现类。<br />上面说了这么多，来个demo
```java
public class BeanFactoryDemo {

    public static void main(String[] args) {
        //创建一个BeanFactory
        DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();

        //创建一个BeanDefinitionReader，构造参数是一个BeanDefinitionRegistry
        //因为DefaultListableBeanFactory实现了BeanDefinitionRegistry，所以直接把beanFactory当做构造参数传过去
        AnnotatedBeanDefinitionReader beanDefinitionReader = new AnnotatedBeanDefinitionReader(beanFactory);

        //读取当前类 BeanFactoryDemo 为一个Bean，让Spring生成这个Bean
        beanDefinitionReader.register(BeanFactoryDemo.class);

        //从容器中获取注册的BeanFactoryDemo的Bean
        BeanFactoryDemo beanFactoryDemo = beanFactory.getBean(BeanFactoryDemo.class);

        System.out.println("beanFactoryDemo = " + beanFactoryDemo);
    }

}
```
简单说一下上面代码的意思

- 创建一个`BeanFactory`，就是`DefaultListableBeanFactory`
- 创建一个`AnnotatedBeanDefinitionReader`，构造参数是一个`BeanDefinitionRegistry`，因为`BeanDefinitionReader`需要把读出来的`BeanDefinition`存到`BeanDefinitionRegistry`中，同时因为`DefaultListableBeanFactory`实现了`BeanDefinitionRegistry`，所以直接把`beanFactory`当做构造参数传过去
- 读取当前类 `BeanFactoryDemo` 为一个Bean，让Spring生成这个Bean
- 后面就是获取打印

运行结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690965764044-c616e163-f813-4e80-8578-edea2f0951fa.png#averageHue=%23383838&clientId=u61bda670-b95d-4&from=paste&id=u64bdcb8e&originHeight=256&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5f38672-0942-41e1-9d68-9987dc470ae&title=)<br />成功获取到注册的Bean
<a name="aw1MB"></a>
### 总结
主要讲了实现IOC的几个核心的组件<br />`BeanFactory`及其接口体系：

- `ListableBeanFactory`
- `HierarchicalBeanFactory`
- `ConfigurableBeanFactory`
- `AutowireCapableBeanFactory`

`BeanDefinition`及其相关组件：

- `BeanDefinition`
- `BeanDefinitionReader`和`ClassPathBeanDefinitionScanner`：读取资源，生成`BeanDefinition`
- `BeanDefinitionRegistry`：存储`BeanDefinition`

`BeanFactory`核心实现：

- `DefaultListableBeanFactory`：IOC容器，同时实现了`BeanDefinitionRegistry`接口
<a name="YlWqM"></a>
## `ApplicationContext`
先来看看`ApplicationContext`的接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966073423-766756ed-6efb-4b6d-970f-0e8091e244a4.png#averageHue=%23302c2b&clientId=u61bda670-b95d-4&from=paste&id=u2e523acf&originHeight=356&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud3c190d5-13ed-4f39-a211-caf5d76c2bd&title=)<br />`ApplicationContext`继承的几个接口，除了`EnvironmentCapable`和`ApplicationEventPublisher`之外，其余都是前面说的。<br />`EnvironmentCapable`这个接口比较简单，提供了获取`Environment`的功能<br />![EnvironmentCapable](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966073382-fb41908b-1444-4379-be22-fbabb0162cd8.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u6b78ed50&originHeight=306&originWidth=850&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u3e890865-6b64-4c48-944d-b3cbba4bf95&title=EnvironmentCapable "EnvironmentCapable")<br />说明了可以从`ApplicationContext`中获取到`Environment`，所以`EnvironmentCapable`也算是前面说过了<br />`ApplicationContext`也继承了`ListableBeanFactory`和`HierarchicalBeanFactory`，也就说明`ApplicationContext`其实他也是一个`BeanFactory`，所以说`ApplicationContext`是IOC容器的说法也没什么毛病，但是由于他还继承了其它接口，功能比`BeanFactory`多多了。<br />所以，`ApplicationContext`是一个集万千功能为一身的接口，一旦获取到了`ApplicationContext`（可以`@Autowired`注入），就可以用来获取Bean、加载资源、获取环境，还可以国际化一下，属实是个王炸。<br />虽然`ApplicationContext`继承了这些接口，但是`ApplicationContext`对于接口的实现是通过一种委派的方式，而真正的实现都是前面说的那些实现<br />什么叫委派呢，写一个例子就知道了
```java
public class MyApplicationContext implements ApplicationContext {

    private final ResourcePatternResolver resourcePatternResolver = new PathMatchingResourcePatternResolver();

    @Override
    public Resource[] getResources(String locationPattern) throws IOException {
        return resourcePatternResolver.getResources(locationPattern);
    }

}
```
如上，其实是一段伪代码<br />因为`ApplicationContext`继承了`ResourcePatternResolver`接口，所以实现了`getResources`方法，但是真正的实现其实是交给变量中的`PathMatchingResourcePatternResolver`来实现的，这其实就是委派，不直接实现，而是交给其它真正实现了这个接口的类来处理<br />同理，`ApplicationContext`对于`BeanFactory`接口的实现其实最终也是交由`DefaultListableBeanFactory`来委派处理的。<br />委派这种方式在Spring内部还是用的非常多的，前面提到的某些接口在的实现上也是通过委派的方式来的<br />`ApplicationContext`有一个子接口，`ConfigurableApplicationContext`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966073385-f7d90775-0bb3-450b-a129-b34d85c23d38.png#averageHue=%23695e47&clientId=u61bda670-b95d-4&from=paste&id=u44439ac6&originHeight=388&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud1326e59-e57f-486b-a635-728f10e8a74&title=)<br />从提供的方法看出，就是可以对`ApplicationContext`进行配置，比如设置`Environment`，同时也能设置parent，说明了`ApplicationContext`也有子父的概念<br />已经看到了很多以`Configurable`开头的接口，这就是命名习惯，表示了可配置的意思，提供的都是set、add之类的方法<br />`ApplicationContext`的实现很多，但是他有一个非常重要的抽象实现`AbstractApplicationContext`，因为其它的实现都是继承这个抽象实现<br />![AbstractApplicationContext](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966073434-99c826d6-839c-4879-8979-b103bc884d23.png#averageHue=%232c2b2b&clientId=u61bda670-b95d-4&from=paste&id=u8dc215c4&originHeight=503&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u49c1d99c-c783-40c5-9cdd-80bf5d7ab1d&title=AbstractApplicationContext "AbstractApplicationContext")<br />这个类主要是实现了一些继承的接口方法，通过委派的方式，比如对于`BeanFactory`接口的实现<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966073621-0235dc2c-3ab3-4060-822c-ab30a0967d3c.png#averageHue=%23665942&clientId=u61bda670-b95d-4&from=paste&id=u59a1d64a&originHeight=580&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub8c622f3-9c4b-4bb8-b082-5a9860f1c86&title=)<br />并且`AbstractApplicationContext`这个类也实现了一个非常核心的`refresh`方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966073976-de983e8c-9748-4c22-bb6c-1a35865c9e93.png#averageHue=%232d2d2d&clientId=u61bda670-b95d-4&from=paste&id=uf07fb6c3&originHeight=565&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1d4c1bd0-cb4f-4fbb-9ab0-6b789edcf0a&title=)<br />所有的`ApplicationContext`在创建之后必须调用这个`refresh`方法之后才能使用。
<a name="sh7iF"></a>
## 事件
Spring事件是一种观察者模式的实现，他的作用主要是用来解耦合的。<br />当发生了某件事，只要发布一个事件，对这个事件的监听者（观察者）就可以对事件进行响应或者处理。<br />举个例子来说，假设发生了火灾，可能需要打119、救人，那么就可以基于事件的模型来实现，只需要打119、救人监听火灾的发生就行了，当发生了火灾，通知这些打119、救人去触发相应的逻辑操作。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966265435-1e0376f0-7539-4ca8-b4e3-55c210530bed.png#averageHue=%23faf7f7&clientId=u61bda670-b95d-4&from=paste&id=u00513eb4&originHeight=171&originWidth=331&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7a19839b-a277-47af-92e6-7be30e0094c&title=)
<a name="Ho2V9"></a>
### 1、什么是Spring Event 事件
Spring Event 事件就是Spring实现了这种事件模型，只需要基于Spring提供的API进行扩展，就可以轻易地完成事件的发布与订阅<br />Spring事件相关api主要有以下几个：

- `ApplicationEvent`
- `ApplicationListener`
- `ApplicationEventPublisher`
<a name="i5ltF"></a>
#### `ApplicationEvent`
![ApplicationEvent](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966265561-1a7366f6-e35a-4191-b11a-d557daedc05d.png#averageHue=%232d2c2b&clientId=u61bda670-b95d-4&from=paste&id=u07a4b558&originHeight=924&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u52685217-aa67-403b-a364-a634169904c&title=ApplicationEvent "ApplicationEvent")<br />事件的父类，所有具体的事件都得继承这个类，构造方法的参数是这个事件携带的参数，监听器就可以通过这个参数来进行一些业务操作。
<a name="w8qNA"></a>
#### `ApplicationListener`
![ApplicationListener](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966266021-12ed2040-d232-4290-991f-fe872ffedab2.png#averageHue=%232c2c2b&clientId=u61bda670-b95d-4&from=paste&id=uca30aa27&originHeight=282&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u08add71a-b8f3-4f7c-9fce-45b354d8a55&title=ApplicationListener "ApplicationListener")<br />事件监听的接口，泛型是需要监听的事件类型，子类需要实现`onApplicationEvent`，参数就是监听的事件类型，`onApplicationEvent`方法的实现就代表了对事件的处理，当事件发生时，Spring会回调`onApplicationEvent`方法的实现，传入发布的事件。
<a name="k0rYQ"></a>
#### `ApplicationEventPublisher`
![ApplicationEventPublisher](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966265826-f8ae9a64-064c-4593-9bbd-0c474f049ac1.png#averageHue=%232c2c2b&clientId=u61bda670-b95d-4&from=paste&id=ube494bd5&originHeight=871&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u0fa7aa0d-4061-4c61-8a0f-e8419057184&title=ApplicationEventPublisher "ApplicationEventPublisher")<br />事件发布器通过`publishEvent`方法就可以发布一个事件，然后就可以触发监听这个事件的监听器的回调。<br />`ApplicationContext`继承了`ApplicationEventPublisher`，说明只要有`ApplicationContext`就可以来发布事件了。
<a name="ECLAq"></a>
#### 案例代码
就以上面的火灾为例<br />**创建一个火灾事件类**<br />火灾事件类继承`ApplicationEvent`
```java
// 火灾事件
public class FireEvent extends ApplicationEvent {

    public FireEvent(String source) {
        super(source);
    }

}
```
**创建火灾事件的监听器**<br />打119的火灾事件的监听器：
```java
public class Call119FireEventListener implements ApplicationListener<FireEvent> {

    @Override
    public void onApplicationEvent(FireEvent event) {
        System.out.println("打119");
    }

}
```
救人的火灾事件的监听器：
```java
public class SavePersonFireEventListener implements ApplicationListener<FireEvent> {

    @Override
    public void onApplicationEvent(FireEvent event) {
        System.out.println("救人");
    }

}
```
事件和对应的监听都有了，接下来进行测试：
```java
public class Application {

    public static void main(String[] args) {
        //创建一个Spring容器
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 事件监听器 注册到容器中
        applicationContext.register(Call119FireEventListener.class);
        applicationContext.register(SavePersonFireEventListener.class);
        applicationContext.refresh();

        // 发布着火的事件，触发监听
        applicationContext.publishEvent(new FireEvent("着火了"));
    }

}
```
将两个事件注册到Spring容器中，然后发布`FireEvent`事件<br />运行结果：
```
打119
救人
```
控制台打印出了结果，触发了监听。<br />如果现在需要对火灾进行救火，那么只需要去监听`FireEvent`，实现救火的逻辑，注入到Spring容器中，就可以了，其余的代码根本不用动。
<a name="RUc4u"></a>
### 2、Spring内置的事件
Spring内置的事件很多，这里罗列几个

| 事件类型 | 触发时机 |
| --- | --- |
| `ContextRefreshedEvent` | 在调用`ConfigurableApplicationContext` 接口中的`refresh()`方法时触发 |
| `ContextStartedEvent` | 在调用`ConfigurableApplicationContext`的`start()`方法时触发 |
| `ContextStoppedEvent` | 在调用`ConfigurableApplicationContext`的`stop()`方法时触发 |
| `ContextClosedEvent` | 当`ApplicationContext`被关闭时触发该事件，也就是调用`close()`方法触发 |

在`ApplicationContext(Spring容器)`启动的过程中，Spring会发布这些事件，如果需要这Spring容器启动的某个时刻进行什么操作，只需要监听对应的事件即可。
<a name="aI7dx"></a>
### 3、Spring事件的传播特性
Spring事件的传播是什么意思呢？<br />前面提到，`ApplicationContext`有子父容器的概念，而Spring事件的传播就是指当通过子容器发布一个事件之后，不仅可以触发在这个子容器的事件监听器，还可以触发在父容器的这个事件的监听器。
<a name="OFBiC"></a>
#### 上代码
```java
public class EventPropagateApplication {

    public static void main(String[] args) {

        // 创建一个父容器
        AnnotationConfigApplicationContext parentApplicationContext = new AnnotationConfigApplicationContext();
        //将 打119监听器 注册到父容器中
        parentApplicationContext.register(Call119FireEventListener.class);
        parentApplicationContext.refresh();

        // 创建一个子容器
        AnnotationConfigApplicationContext childApplicationContext = new AnnotationConfigApplicationContext();
        //将 救人监听器 注册到子容器中
        childApplicationContext.register(SavePersonFireEventListener.class);
        childApplicationContext.refresh();

        // 设置一下父容器
        childApplicationContext.setParent(parentApplicationContext);

        // 通过子容器发布着火的事件，触发监听
        childApplicationContext.publishEvent(new FireEvent("着火了"));

    }

}
```
创建了两个容器，父容器注册了打119的监听器，子容器注册了救人的监听器，然后将子父容器通过`setParent`关联起来，最后通过子容器，发布了着火的事件。<br />运行结果：
```
救人
打119
```
从打印的日志，的确可以看出，虽然是子容器发布了着火的事件，但是父容器的监听器也成功监听了着火事件。<br />而这种传播特性，从源码中也可以看出来<br />![事件传播源码](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966265842-afffed7b-a642-4eb2-8232-b9d2d383557b.png#averageHue=%23312d2c&clientId=u61bda670-b95d-4&from=paste&id=u36fbb41d&originHeight=492&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub4f62432-ab2e-43c3-b970-71afe3dcdb5&title=%E4%BA%8B%E4%BB%B6%E4%BC%A0%E6%92%AD%E6%BA%90%E7%A0%81 "事件传播源码")<br />如果父容器不为空，就会通过父容器再发布一次事件。
<a name="ajRJa"></a>
#### 传播特性的一个小坑
前面说过，在Spring容器启动的过程，会发布很多事件，如果需要有相应的扩展，可以监听这些事件。<br />但是，不知道你有没有遇到过这么一个坑，就是在SpringCloud环境下，监听这些**Spring事件**的监听器会执行很多次，这其实就是跟传播特性有关。<br />在SpringCloud环境下，为了使像`FeignClient`和`RibbonClient`这些不同服务的配置相互隔离，会为每个`FeignClient`或者是`RibbonClient`创建一个Spring容器，而这些容器都有一个公共的父容器，那就是SpringBoot项目启动时创建的容器<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966281111-740f20e6-436b-42d5-8012-f2bdbc683ae1.png#averageHue=%23f3f3f3&clientId=u61bda670-b95d-4&from=paste&id=uc1f61ab8&originHeight=152&originWidth=352&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9bdde887-5095-4e0d-b70d-d730d3dc242&title=)<br />假设监听了容器刷新的`ContextRefreshedEvent`事件，那么自己写的监听器就在SpringBoot项目启动时创建的容器中<br />每个服务的配置容器他也是Spring容器，启动时也会发布`ContextRefreshedEvent`，那么由于传播特性的关系，自定义的事件监听器就会触发执行多次<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690966281426-480d6f98-1803-48a3-870f-1714e1cce38f.png#averageHue=%231b1b1a&clientId=u61bda670-b95d-4&from=paste&id=u7dfe660d&originHeight=161&originWidth=351&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u983d7f9b-3e12-4087-a052-6123adde8e6&title=)<br />如何解决这个坑呢？<br />可以进行判断这些监听器有没有执行过，比如加一个判断的标志；或者是监听类似的事件，比如`ApplicationStartedEvent`事件，这种事件是在SpringBoot启动中发布的事件，而子容器不是SpringBoot，所以不会多次发这种事件，也就会只执行一次。
<a name="gdkfS"></a>
## 总结
简单地回顾一下几个核心功能：

- 资源管理：对资源进行统一的封装，方便资源读取和管理
- 环境：对容器或者是项目的配置进行管理
- 类型转换：将一种类型转换成另一种类型
- 数据绑定：将数据跟对象的属性进行绑定，绑定之前涉及到类型转换
- 泛型处理：一个操作泛型的工具类，Spring中到处可见
- 国际化：对Java的国际化进行了统一的封装
- `BeanFactory`：IOC容器
- `ApplicationContext`：一个集万千功能于一身的王炸接口，也可以说是IOC容器
- 事件：Spring提供的基于观察者模式实现的解耦合利器

当然除了上面，Spring还有很多其它核心功能，就比如AOP、SpEL表达式等等，由于AOP涉及到Bean生命周期，这里没有涉及到Bean生命周期的讲解；至于SpEL他是Spring提供的表达式语言，主要是语法，解析语法的一些东西。
