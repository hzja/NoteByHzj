<a name="PQaKU"></a>
## 前言
Spring 是怎么找到 resources 下面的配置文件的？先说一个大概，后面再完善。<br />在 Spring 中对 resources 下面的配置文件加载是通过`new ClassPathReource("XXX")`来封装成 `ClassPathResource`，而 `ClassPathResource` 中是通过 class 或者 classLoader 底层的双亲委派机制方法进行加载，即 `AppClassLoader` 在父类加载器中类找到的时候，就会自己查找，在java.class.path路径下查找，找到就返回，没找到就让之类找，如果没有之类就会抛出异常。
<a name="kOelz"></a>
## 问题场景
```java
@Test
@SuppressWarnings(value = "deprecation")
public void testSimpleLoad() {
    XmlBeanFactory xmlBeanFactory = new XmlBeanFactory(new ClassPathResource("beanFactoryTest.xml"));
    MyTestBean myTestBean = (MyTestBean) xmlBeanFactory.getBean("myTestBean");
}
```
beanFactoryTest.xml
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans.xsd">
  <bean id="myTestBean" class="MyTestBean"/>
</beans>
```
MyTestBean.java
```java
public class MyTestBean {

    private String testStr = "testStr";
    public String getTestStr() {
        return testStr;
    }

    public void setTestStr(String testStr) {
        this.testStr = testStr;
    }
}
```
这段代码实现了如下功能：

1. 加载资源文件
2. 读取配置文件
3. 实例化Bean
4. 然后获取实例化的Bean

这里探究第一个功能，即加载资源文件
<a name="HVlfR"></a>
## 探究原理
<a name="sn6LQ"></a>
### 源码流程
`this.reader.loadBeanDefinitions(resource);`才是资源加载的真正实现
```java
public XmlBeanFactory(Resource resource, BeanFactory parentBeanFactory) throws BeansException {
    super(parentBeanFactory);

    this.reader.loadBeanDefinitions(resource);
}
```
封装资源文件，对参数使用 EncodedResource 进行封装。
```java
public int loadBeanDefinitions(Resource resource) throws BeanDefinitionStoreException {
    return loadBeanDefinitions(new EncodedResource(resource));
}
```
获取资源流 inputstream。得到 inpustream 后，就可以进行解析了
```java
public int loadBeanDefinitions(EncodedResource encodedResource) throws BeanDefinitionStoreException {
    Assert.notNull(encodedResource, "EncodedResource must not be null");
    if (logger.isTraceEnabled()) {
        logger.trace("Loading XML bean definitions from " + encodedResource);
    }

    Set<EncodedResource> currentResources = this.resourcesCurrentlyBeingLoaded.get();
    if (!currentResources.add(encodedResource)) {
        throw new BeanDefinitionStoreException(
            "Detected cyclic loading of " + encodedResource + " - check your import definitions!");
    }
    try (InputStream inputStream = encodedResource.getResource().getInputStream()) {
        InputSource inputSource = new InputSource(inputStream);
        if (encodedResource.getEncoding() != null) {
            inputSource.setEncoding(encodedResource.getEncoding());
        }
        return doLoadBeanDefinitions(inputSource, encodedResource.getResource());
    }
    catch (IOException ex) {
        throw new BeanDefinitionStoreException(
            "IOException parsing XML document from " + encodedResource.getResource(), ex);
    }
    finally {
        currentResources.remove(encodedResource);
        if (currentResources.isEmpty()) {
            this.resourcesCurrentlyBeingLoaded.remove();
        }
    }
}
```
然后通过`classLoader.getResourceAsStream(this.path)`来获取
```java
@Override
public InputStream getInputStream() throws IOException {
    InputStream is;
    if (this.clazz != null) {
       is = this.clazz.getResourceAsStream(this.path);

    } else if (this.classLoader != null) {
       is = this.classLoader.getResourceAsStream(this.path);
    } else {
       is = ClassLoader.getSystemResourceAsStream(this.path);
    }

    if (is == null) {
       throw new FileNotFoundException(getDescription() + " cannot be opened because it does not exist");
    }
    return is;
}
```
后面就不展开了，就是通过[双亲委派](http://mp.weixin.qq.com/s?__biz=MzIyODE5NjUwNQ==&mid=2653333453&idx=1&sn=f624eaf38f455555eaacae0b55454642&chksm=f387ae3bc4f0272defed4b665cecbc72e87d5e358ab9689c215bff8a384366b16a70a9d430e2&scene=21#wechat_redirect)机制，父类加载器没找到，而是先调用`AppClassLoader#findResource`，然后调用`BuiltinClassLoader#findResourceOnClassPath(name)`，然后通过`URLClassPath#findResource`获取对应的 loader，然后最后调用相应的`Loader#findResource`方法来获取到配置文件的 URL。
<a name="tpnqr"></a>
## 获取到资源输入流
什么是 `ClassPathResource`？在源码中，是怎么通过`ClassPathResource#getInputStream`获取到资源输入流呢？<br />Spring 对其内部使用到的资源实现了自己的抽象结构：Resource 接口封装底层资源，通过`ClassPathResource("XXXX")`加载 classpath 资源。<br />`ClassPathResource` 中的实现了 Resource 中的 `getInputStream` 方法，是通过 classLoader 提供的底层方法进行调用的。
<a name="OfjxD"></a>
## `ClassPathReource`的classLoader是什么呢？
目前所知的就是在加载配置文件的时候，在创建 `ClassPathResource` 实例时候，会指定其 [classLoader](http://mp.weixin.qq.com/s?__biz=MzIyODE5NjUwNQ==&mid=2653344898&idx=2&sn=f2d2127db9b93b7c216ed41d8a9431c3&chksm=f387dd74c4f05462f60a8b4fa5b25b1dc65ea2daddbc0f563ee6f527e22931b4d98870c7e573&scene=21#wechat_redirect) 为 `AppClassLoader`。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700485880920-32f1194b-a312-4e21-a004-adf25917460f.png#averageHue=%23fbfbfb&clientId=ueabe45d7-ddd5-4&from=paste&height=263&id=u58efa797&originHeight=658&originWidth=3177&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=196586&status=done&style=shadow&taskId=u8f750b43-a13d-4f67-a9dc-6926ba354ff&title=&width=1270.8)
<a name="ADoYI"></a>
## classLoader底层是怎么找到要加载的配置文件的呢？
JVM 在加载 ClassLoader 类的时候会调用静态块文件：<br />![DM_20231120222756_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700491667686-c798208e-0ce3-43a5-8fe9-a37f61ca36c9.png#averageHue=%23222327&clientId=ufea047b9-55a2-4&from=ui&id=IGbuf&originHeight=610&originWidth=865&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1586200&status=done&style=shadow&taskId=ud31ed6d1-9701-40be-ad74-bd42eb02e5c&title=)<br />通过`System.getProperty("java.class.path")`获取系统类路径，然后实例化一个 `URLClassPath`，进而实例化出一个 `AppClassLoader` 出来，而这个 `AppClassLoader` 就能加载所有的 class path 文件，至于这个 class path 是怎么来的，而系统属性中的java.class.path又是怎么赋初值的，这里先不说。
<a name="ML24n"></a>
## 总结
[Spring](http://mp.weixin.qq.com/s?__biz=MzIyODE5NjUwNQ==&mid=2653366943&idx=1&sn=602c196b2c10e5d04a6815883ab50242&chksm=f3862369c4f1aa7f2aae2e3f5f1c261394b1e01d72a88012ffb6d483d368f2d44f57f39ced5c&scene=21#wechat_redirect) 其实就是通过 Java 提过的类加载机制来加载的配置文件。Spring 通过接口类 Resource 抽象了所有内部使用到的底层资源，不同的资源有不同的实现，加载配置文件用到的`ClassPathResource` 就对应于 Classpath 资源，其实现便是通过 JAVA 类加载提供的底层方法进行的。
<a name="GYYJe"></a>
## 扩展

1. `System.getProperty("java.class.path")` 获取到的系统属性是怎么来的或者说是怎么进行赋值的呢？这一块通过看java.class.path对应的路径发现都是依赖的 java 和 resources 文件夹路径，所以猜测与编译过程中有关或者 Maven 有关
2. 最后探究下来其实就是通过 `URLClassPath` 中的 Loader 有关，而通过 debug 发现loader 其实有 `JarLoader`，`FileLoader` 等等，这些与类加载器是啥关系？之前一直以为类加载器就是加载.class文件的。
3. `Redource` 接口抽象了所有 spring 内部使用的底层资源：文件、Classpath、URL 资源、InputStream 资源，这里面用到了大量的模板方法来抽取公共方法，抽取抽象类，抽象接口。
4. 加载配置文件获取 `inputstream` 只是第一步，后面探究的就是怎么读取配置文件的了。
