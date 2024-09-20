Java Spring
<a name="n44Jr"></a>
## 1、概念
SPI(Service Provider Interface)服务提供接口，简单来说就是用来解耦，实现插件的自由插拔，具体实现方案可参考JDK里的`ServiceLoader`（加载classpath下所有`META-INF/services/`目录下的对应给定接口包路径的文件，然后通过反射实例化配置的所有实现类，以此将接口定义和逻辑实现分离）<br />Spring在3.0.x的时候就已经引入了`spring.handlers`，很多博客讲Spring SPI的时候并没有提到spring.handlers，但是通过分析对比，其实`spring.handlers`也是一种SPI的实现，只不过它是基于xml的，而且在没有boot的年代，它几乎是所有三方框架跟spring整合的必选机制。<br />![2021-07-25-19-16-58-184731.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211919561-64eb28a9-5d53-46a8-8ca9-ed65af0ddd4c.png#clientId=u9b86f762-1bff-4&from=ui&id=uc5dc1960&originHeight=319&originWidth=1080&originalType=binary&ratio=1&size=61361&status=done&style=shadow&taskId=ud99a9d1d-41b6-477d-8365-c355ac8397d)<br />在3.2.x又新引入了`spring.factories`，它的实现跟JDK的SPI就基本是相似的了。<br />![2021-07-25-19-16-58-295757.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1627211929312-2ec58a41-cb6f-43f8-a685-23234cdbacb6.jpeg#clientId=u9b86f762-1bff-4&from=ui&id=ua792da15&originHeight=345&originWidth=1080&originalType=binary&ratio=1&size=30650&status=done&style=shadow&taskId=u9da5ca70-fd5d-446a-8870-69b7ae42ab9)<br />`spring.handlers`和`spring.factories`都可以归纳为Spring提供的SPI机制，通过这两种机制，可以在不修改Spring源码的前提下，非常轻松的做到对Spring框架的扩展开发。
<a name="IgfiG"></a>
## 2、实现
<a name="niM8L"></a>
### 2.1 先看看`spring.handlers` SPI
在Spring里有个接口`NamespaceHandlerResolver`，只有一个默认的实现类`DefaultNamespaceHandlerResolver`，而它的作用就是加载classpath下可能分散在各个jar包中的META-INF/spring.handlers文件，resolve方法中关键代码如下：
```java
//加载所有jar包中的META-INF/spring.handlers文件
Properties mappings=
    PropertiesLoaderUtils.loadAllProperties(this.handlerMappingsLocation, this.classLoader);

//把META-INF/spring.handlers中配置的namespaceUri对应实现类实例化
NamespaceHandler namespaceHandler =
    (NamespaceHandler) BeanUtils.instantiateClass(handlerClass);
```
`DefaultNamespaceHandlerResolver.resolve()`主要被用在`BeanDefinitionParserDelegate`的`parseCustomElement`和`decorateIfRequired`，所以`spring.handlers` SPI机制主要也是被用在bean的扫描和解析过程中。
<a name="arqai"></a>
### 2.2 再来看`spring.factories` SPI
```java
// 获取某个已定义接口的实现类，跟JDK的ServiceLoader SPI相似度为90%
List<BeanInfoFactory> beanInfoFactories = SpringFactoriesLoader.loadFactories(BeanInfoFactory.class, classLoader);

// spring.factories文件的格式为：key=value1,value2,value3
// 从所有jar文件中找到MET-INF/spring.factories文件（注意是：classpath下的所有jar包，所以可插拔、扩展性超强）
Enumeration<URL> urls = (classLoader != null ? classLoader.getResources(FACTORIES_RESOURCE_LOCATION) :
                         ClassLoader.getSystemResources(FACTORIES_RESOURCE_LOCATION));
List<String> result = new ArrayList<String>();
while (urls.hasMoreElements()) {
    URL url = urls.nextElement();
    Properties properties = PropertiesLoaderUtils.loadProperties(new UrlResource(url));
    String propertyValue = properties.getProperty(factoryClassName);
    for (String factoryName : StringUtils.commaDelimitedListToStringArray(propertyValue)) {
        result.add(factoryName.trim());
    }
}
return result;
```
更多细节，大家可以参考`SpringFactoriesLoader`类，Spring自3.2.x引入`spring.factories` SPI后其实一直没怎么利用起来，只有`CachedIntrospectionResults`（初始化bean的过程中）用到了，而且在几大核心jar包里，也只有bean包里才有用到。<br />真正把`spring.factories`发扬光大的，是到了Spring Boot，可以看到boot包里配置了非常多的接口实现类。大家跟踪boot的启动类`SpringApplication`可以发现，有很多地方都调用了`getSpringFactoriesInstances()`方法，这些就是SpringBoot留的扩展机会。<br />![2021-07-25-19-16-58-568739.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211940533-a4d69d5d-c2a6-4c59-b916-cf27fa7b3293.png#clientId=u9b86f762-1bff-4&from=ui&id=u27642189&originHeight=646&originWidth=1080&originalType=binary&ratio=1&size=347265&status=done&style=none&taskId=u1bd60b17-1d99-4478-8a96-0359b9dc16e)
<a name="JouKt"></a>
## 3、应用
先来看看Mybatis和Dubbo早期跟Spring整合的实现，他们无一例外都用到了spring.handlers SPI机制，以此来向IOC容器注入自己的Bean。<br />![2021-07-25-19-16-58-703729.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211957694-a1089a4d-6e07-434c-a07d-81dc368ac0f3.png#clientId=u9b86f762-1bff-4&from=ui&id=u67b09771&originHeight=255&originWidth=1080&originalType=binary&ratio=1&size=67698&status=done&style=none&taskId=udb138cc2-b6a0-42f2-98d9-22728058ab1)![2021-07-25-19-16-58-840729.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211957745-9c4f0b7c-3c73-4dad-8d0d-ef5f19513d60.png#clientId=u9b86f762-1bff-4&from=ui&id=u5a3d4591&originHeight=271&originWidth=1080&originalType=binary&ratio=1&size=65761&status=done&style=none&taskId=ue462038b-c545-4eb8-a9f8-0d6d4438ef0)<br />进入boot时代后，spring.factories SPI机制应用得更加广泛，可以在容器启动、环境准备、初始化、上下文加载等等环节轻轻松松的对Spring做扩展开发（例如：项目中用到spring.factories SPI机制对配置文件中的变量实现动态解密）。
<a name="oQOYs"></a>
## 4、实践（加载application.xyz配置文件）
Spring里有两种常见的配置文件类型：application.properties 和 application.yml，其中yml是近年兴起的，但是没有合适的编辑器时很容易把格式写错，导致上线出问题。所以有没有办法让Spring支持一种新的配置文件格式，既保留yml的简洁优雅，有能够有强制的格式校验，暂时想到了json格式。<br />![2021-07-25-19-16-59-044731.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211964714-e1c4f741-d65e-4b84-bea7-803ea421a557.png#clientId=u9b86f762-1bff-4&from=ui&id=uf2d707ca&originHeight=524&originWidth=1011&originalType=binary&ratio=1&size=56213&status=done&style=none&taskId=ud1e6db42-8967-43be-b313-67c811e1329)
```java
# 这是spring.factories中的配置
org.springframework.boot.env.PropertySourceLoader=top.hiccup.json.MyJsonPropertySourceLoader
```
```java
public class MyJsonPropertySourceLoader implements PropertySourceLoader {
    @Override
    public String[] getFileExtensions() {
        return new String[]{"xyz"};
    }
    @Override
    public List<PropertySource<?>> load(String name, Resource resource) throws IOException {

        BufferedReader reader = new BufferedReader(new InputStreamReader(resource.getInputStream()));
        StringBuilder sb = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            sb.append(line);
        }
        // 这里只是做了简单解析，没有做嵌套配置的解析
        JSONObject json = JSONObject.parseObject(sb.toString());
        List<PropertySource<?>> propertySources = new ArrayList<>();
        MapPropertySource mapPropertySource = new MapPropertySource(resource.getFilename(), json);
        propertySources.add(mapPropertySource);
        return propertySources;
    }
}

ConfigurableApplicationContext ctx = SpringApplication.run(BootTest.class, args);
Custom custom = ctx.getBean(Custom.class);
System.out.println(custom.name);
System.out.println(custom.age);
```
具体代码可以参考（[https://github.com/hiccup234/web-advanced/tree/master/configFile](https://github.com/hiccup234/web-advanced/tree/master/configFile)） ，运行得到结果如下：<br />![2021-07-25-19-16-59-141733.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627211971852-6217cb0b-1cf7-47e3-9a60-eb044be3c6ad.png#clientId=u9b86f762-1bff-4&from=ui&id=u150aeb01&originHeight=130&originWidth=574&originalType=binary&ratio=1&size=7013&status=done&style=none&taskId=u1c9f48b6-6ba8-444d-b3ef-4c8e243c900)<br />可见在不修改Spring源码的前提下，轻松通过Spring开放的扩展性实现了对新的配置文件类型的加载和解析。<br />这就是Spring SPI的魅力吧。
