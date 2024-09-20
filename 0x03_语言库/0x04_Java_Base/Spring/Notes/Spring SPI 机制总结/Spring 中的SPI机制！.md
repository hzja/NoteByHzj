JavaSpringBootSPI
<a name="JA9W9"></a>
## 简介
SPI(Service Provider Interface)是JDK内置的一种服务提供发现机制，可以用来启用框架扩展和替换组件，主要用于框架中开发，例如Dubbo、Spring、Common-Logging，JDBC等采用采用SPI机制，针对同一接口采用不同的实现提供给不同的用户，从而提高了框架的扩展性。
<a name="Ljue5"></a>
## Java SPI实现
Java内置的SPI通过java.util.ServiceLoader类解析classPath和jar包的META-INF/services/目录下的以接口全限定名命名的文件，并加载该文件中指定的接口实现类，以此完成调用。
<a name="CSoEN"></a>
## 示例说明
<a name="RNzqF"></a>
### 创建动态接口
```java
public interface VedioSPI{
    void call();
}
```
<a name="iD1WQ"></a>
### 实现类1
```java
public class Mp3Vedio implements VedioSPI{
    @Override
    public void call(){
        System.out.println("this is mp3 call");
    }

}
```
<a name="Qzzgo"></a>
### 实现类2
```java
public class Mp4Vedio implements VedioSPI{
    @Override
    public void call(){
       System.out.println("this is mp4 call");
    }

}
```
在项目的source目录下新建META-INF/services/目录下，创建com.skywares.fw.juc.spi.VedioSPI文件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633000740-d63a093e-c845-4ce5-b225-7ab80376f3dd.png#averageHue=%23e5e9e4&clientId=u6e3c376e-2d7f-4&from=paste&id=ubd7c9db6&originHeight=176&originWidth=1058&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u63c686d6-f9fb-45bd-b67f-a731fcfde52&title=)
<a name="J2tsH"></a>
## 相关测试
```java
public class VedioSPITest{
    public static void main(String[] args){
        ServiceLoader<VedioSPI> serviceLoader =ServiceLoader.load(VedioSPI.class);
        
        serviceLoader.forEach(t->{
            t.call();
        });
    }
}
```
**说明：Java实现spi是通过**`**ServiceLoader**`**来查找服务提供的工具类。**
<a name="SwcN3"></a>
## 运行结果：
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633000775-b271bf2b-20f7-4d25-be04-d4d1686ccba3.png#averageHue=%23eae9e5&clientId=u6e3c376e-2d7f-4&from=paste&id=u19d7550f&originHeight=101&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf14735f1-3fb5-4129-ae36-533781e437d&title=)
<a name="UceYE"></a>
## 源码分析
上述只是通过简单的示例来实现下java的内置的SPI功能。其实现原理是`ServiceLoader`是Java内置的用于查找服务提供接口的工具类，通过调用`load()`方法实现对服务提供接口的查找，最后遍历来逐个访问服务提供接口的实现类。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633000739-8c03bcb6-029d-4fff-8f4d-36cf5a3da755.png#averageHue=%23f5f5f4&clientId=u6e3c376e-2d7f-4&from=paste&id=u2bd3c263&originHeight=807&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b3061b2-733b-4039-a2d5-edb733e8140&title=)<br />从源码可以发现：

- `ServiceLoader`类本身实现了`Iterable`接口并实现了其中的`iterator`方法，iterator方法的实现中调用了LazyIterator这个内部类中的方法，迭代器创建实例。
- 所有服务提供接口的对应文件都是放置在META-INF/services/目录下，`final`类型决定了PREFIX目录不可变更。

虽然java提供的SPI机制的思想非常好，但是也存在相应的弊端。具体如下：

- Java内置的方法方式只能通过遍历来获取
- 服务提供接口必须放到META-INF/services/目录下。

针对java的spi存在的问题，Spring的SPI机制沿用的SPI的思想，但对其进行扩展和优化。
<a name="BvJsU"></a>
## Spring SPI
Spring SPI沿用了Java SPI的设计思想，Spring采用的是spring.factories方式实现SPI机制，可以在不修改Spring源码的前提下，提供Spring框架的扩展性。
<a name="RZkBX"></a>
### Spring 示例
<a name="spbBS"></a>
#### 定义接口
```java
public interface DataBaseSPI{
   void getConnection();
}
```
<a name="a7mVu"></a>
#### 相关实现
```java
// DB2实现
public class DB2DataBase implements DataBaseSPI{
    @Override
    public void getConnection()
    {
        System.out.println("this database is db2");
    }

}

// Mysql实现
public class MysqlDataBase implements DataBaseSPI
{
    @Override
    public void getConnection()
    {
       System.out.println("this is mysql database");
    }

}
```

1. 在项目的META-INF目录下，新增spring.factories文件

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633000779-637c8465-0bae-4ce4-a206-1baf1d51202a.png#averageHue=%23e7e2d4&clientId=u6e3c376e-2d7f-4&from=paste&id=u7290d8da&originHeight=252&originWidth=688&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua1692a7b-30bc-4a88-9c7e-623a16a66ef&title=)

2. 填写相关的接口信息，内容如下：
```
com.skywares.fw.juc.springspi.DataBaseSPI = com.skywares.fw.juc.springspi.DB2DataBase, com.skywares.fw.juc.springspi.MysqlDataBase
```
说明多个实现采用逗号分隔。
<a name="q9Xr6"></a>
#### 相关测试类
```java
public class SpringSPITest{
    public static void main(String[] args){
         List<DataBaseSPI> dataBaseSPIs =SpringFactoriesLoader.loadFactories(DataBaseSPI.class, 
                 Thread.currentThread().getContextClassLoader());
         
         for(DataBaseSPI datBaseSPI:dataBaseSPIs){
            datBaseSPI.getConnection();
         }
    }
}
```
<a name="XbwEP"></a>
#### 输出结果
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633000750-0b837a43-87af-459b-9bfd-e868b848777e.png#averageHue=%23e7e5df&clientId=u6e3c376e-2d7f-4&from=paste&id=ua8e40eca&originHeight=154&originWidth=936&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc3cdd6c1-04d0-45f0-aaa6-8bad2fa73d4&title=)<br />从示例中可以看出，Spring 采用spring.factories实现SPI与java实现SPI非常相似，但是spring的spi方式针对java的spi进行的相关优化具体内容如下：

- Java SPI是一个服务提供接口对应一个配置文件，配置文件中存放当前接口的所有实现类，多个服务提供接口对应多个配置文件，所有配置都在services目录下；
- Spring factories SPI是一个spring.factories配置文件存放多个接口及对应的实现类，以接口全限定名作为key，实现类作为value来配置，多个实现类用逗号隔开，仅spring.factories一个配置文件。

那么spring是如何通过加载spring.factories来实现SpI的呢?可以通过源码来进一步分析。
<a name="hbR8T"></a>
### 源码分析
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633001905-dfdbf3f1-1384-481f-83d6-2e63a618bcba.png#averageHue=%23f4f3ee&clientId=u6e3c376e-2d7f-4&from=paste&id=u9053c20a&originHeight=342&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufd0194c7-9dc6-4167-b43b-780e8a55bbf&title=)<br />说明：`loadFactoryNames`解析spring.factories文件中指定接口的实现类的全限定名，具体实现如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633001984-e85d0daf-fca5-48cd-b036-73260a165f99.png#averageHue=%23efede8&clientId=u6e3c376e-2d7f-4&from=paste&id=uf72f804d&originHeight=537&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ued70ae54-a146-4120-8d0e-da603cb0b02&title=)<br />说明：获取所有jar包中META-INF/spring.factories文件路径，以枚举值返回。遍历spring.factories文件路径，逐个加载解析，整合factoryClass类型的实现类名称，获取到实现类的全类名称后进行类的实例话操作，其相关源码如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677633002607-86f38444-822f-47d8-bf4c-0a0e4707d3c0.png#averageHue=%23f1efeb&clientId=u6e3c376e-2d7f-4&from=paste&id=u6f97f738&originHeight=307&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1e37bc1e-1528-41b6-a68e-94273181923&title=)<br />说明：实例化是通过反射来实现对应的初始化。
<a name="Owlht"></a>
## 总结
SPI技术将服务接口与服务实现进行分离实现解耦，从而提升程序的可扩展性。
