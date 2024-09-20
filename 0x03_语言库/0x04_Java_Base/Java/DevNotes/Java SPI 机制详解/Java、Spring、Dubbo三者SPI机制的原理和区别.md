Java
<a name="QWKuL"></a>
## 什么是SPI
SPI全称为Service Provider Interface，是一种动态替换发现的机制，一种解耦非常优秀的思想，SPI可以很灵活的让接口和实现分离，让api提供者只提供接口，第三方来实现，然后可以使用配置文件的方式来实现替换或者扩展，在框架中比较常见，提高框架的可扩展性。<br />简单来说SPI是一种非常优秀的设计思想，它的核心就是解耦、方便扩展。
<a name="bopMA"></a>
## Java SPI机制--`ServiceLoader`
`ServiceLoader`是Java提供的一种简单的SPI机制的实现，Java的SPI实现约定了以下两件事：

- 文件必须放在META-INF/services/目录底下
- 文件名必须为接口的全限定名，内容为接口实现的全限定名

这样就能够通过`ServiceLoader`加载到文件中接口的实现。
<a name="aq78I"></a>
### 来个demo
第一步，需要一个接口以及他的实现类
```java
public interface LoadBalance {
}

public class RandomLoadBalance implements LoadBalance{
}
```
第二步，在META-INF/services/目录创建一个文件名LoadBalance全限定名的文件，文件内容为`RandomLoadBalance`的全限定名<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695432-e506898f-e32f-454c-9711-ad933e3eafde.png#averageHue=%233f4244&clientId=ub1ded09a-fd8c-4&from=paste&id=u4b199c8c&originHeight=166&originWidth=704&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubbde2d6e-8d5f-4bb3-a178-9827de8528f&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695580-54ba5778-d3cb-4365-814b-8a23a54d9762.png#averageHue=%23344134&clientId=ub1ded09a-fd8c-4&from=paste&id=uef768049&originHeight=226&originWidth=838&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3089e4a8-1012-4ba8-bf33-5365c0ef661&title=)<br />测试类：
```java
public class ServiceLoaderDemo {

    public static void main(String[] args) {
        ServiceLoader<LoadBalance> loadBalanceServiceLoader = ServiceLoader.load(LoadBalance.class);
        Iterator<LoadBalance> iterator = loadBalanceServiceLoader.iterator();
        while (iterator.hasNext()) {
            LoadBalance loadBalance = iterator.next();
            System.out.println("获取到负载均衡策略:" + loadBalance);
        }
    }

}
```
测试结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695570-9b3c43f9-77d9-407b-9f65-c20680950260.png#averageHue=%23343434&clientId=ub1ded09a-fd8c-4&from=paste&id=u1cb560f4&originHeight=246&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8a29a177-2108-4679-ab6a-8db0ce62f41&title=)<br />此时就成功获取到了实现。<br />在实际的框架设计中，上面这段测试代码其实是框架作者写到框架内部的，而对于框架的使用者来说，要想自定义LoadBalance实现，嵌入到框架，仅仅只需要写接口的实现和spi文件即可。
<a name="NyK3p"></a>
### 实现原理
如下是`ServiceLoader`中一段核心代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695460-e47dbfb9-5a96-4053-ba2b-d79ffbfd12af.png#averageHue=%23302c2b&clientId=ub1ded09a-fd8c-4&from=paste&id=uaa9ad301&originHeight=953&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u70b264a0-7b70-41b6-9ee2-f6bb862b173&title=)<br />首先获取一个fullName，其实就是META-INF/services/接口的全限定名<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695790-b20fc07c-b182-46e6-bb92-1bf727c63d19.png#averageHue=%238a5830&clientId=ub1ded09a-fd8c-4&from=paste&id=u14930c8e&originHeight=344&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7b028b87-6eb4-4449-b6d8-526f42247b1&title=)<br />然后通过`ClassLoader`获取到资源，其实就是接口的全限定名文件对应的资源，然后交给parse方法解析资源<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695944-94cf56b0-67d6-44b4-a78f-83c22bb97cd8.png#averageHue=%232c2b2b&clientId=ub1ded09a-fd8c-4&from=paste&id=u69f67eb5&originHeight=797&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufa3e1f03-7690-405d-974e-7ac35db9b8f&title=)<br />parse方法其实就是通过IO流读取文件的内容，这样就可以获取到接口的实现的全限定名<br />再后面其实就是通过反射实例化对象，这里就不展示了。<br />所以其实不难发现ServiceLoader实现原理比较简单，总结起来就是通过IO流读取META-INF/services/接口的全限定名文件的内容，然后反射实例化对象。
<a name="oJ7SE"></a>
### 优缺点
由于Java的SPI机制实现的比较简单，所以他也有一些缺点。<br />第一点就是浪费资源，虽然例子中只有一个实现类，但是实际情况下可能会有很多实现类，而Java的SPI会一股脑全进行实例化，但是这些实现了不一定都用得着，所以就会白白浪费资源。<br />第二点就是无法对区分具体的实现，也就是这么多实现类，到底该用哪个实现呢？如果要判断具体使用哪个，只能依靠接口本身的设计，比如接口可以设计为一个策略接口，又或者接口可以设计带有优先级的，但是不论怎样设计，框架作者都得写代码进行判断。<br />所以总得来说就是`ServiceLoader`无法做到按需加载或者按需获取某个具体的实现。
<a name="a7AzW"></a>
### 使用场景
虽然说`ServiceLoader`可能有些缺点，但是还是有使用场景的，比如说：

- 不需要选择具体的实现，每个被加载的实现都需要被用到
- 虽然需要选择具体的实现，但是可以通过对接口的设计来解决
<a name="K1JuY"></a>
## Spring SPI机制--`SpringFactoriesLoader`
Spring都不陌生，他也提供了一种SPI的实现`SpringFactoriesLoader`。<br />Spring的SPI机制的约定如下：

- 配置文件必须在META-INF/目录下，文件名必须为spring.factories
- 文件内容为键值对，一个键可以有多个值，只需要用逗号分割就行，同时键值都需要是类的全限定名，键和值可以没有任何类与类之间的关系，当然也可以有实现的关系。

所以也可以看出，Spring的SPI机制跟Java的不论是文件名还是内容约定都不一样。
<a name="H4uNa"></a>
### 来个demo
在META-INF/目录下创建spring.factories文件，LoadBalance为键，`RandomLoadBalance`为值<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365695960-8ffa3717-ec17-41c9-9861-8a8e341eab7e.png#averageHue=%233e4143&clientId=ub1ded09a-fd8c-4&from=paste&id=u11a6d60b&originHeight=178&originWidth=646&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u88ee13ed-78f3-455b-b6e8-2da596607d4&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696123-5698e33f-ce36-4bce-aaf0-4c837e30f5d6.png#averageHue=%23807242&clientId=ub1ded09a-fd8c-4&from=paste&id=u309fc49b&originHeight=250&originWidth=896&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf07bccd6-68dd-4052-be33-c7c604a8e18&title=)<br />测试：
```java
public class SpringFactoriesLoaderDemo {

    public static void main(String[] args) {
        List<LoadBalance> loadBalances = SpringFactoriesLoader.loadFactories(LoadBalance.class, MyEnableAutoConfiguration.class.getClassLoader());
        for (LoadBalance loadBalance : loadBalances) {
            System.out.println("获取到LoadBalance对象:" + loadBalance);
        }
    }

}
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696108-b317f8fc-56a7-482d-9e4f-5cc8c6aa851e.png#averageHue=%23353535&clientId=ub1ded09a-fd8c-4&from=paste&id=u11f1f137&originHeight=210&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue51a2607-e8e4-435e-a6b9-968be7f5dcb&title=)<br />成功获取到了实现对象。
<a name="h6ZXv"></a>
### 核心原理
如下是`SpringFactoriesLoader`中一段核心代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696522-0578feeb-0e80-4b53-91d0-40e9868ab50e.png#averageHue=%232b2b2b&clientId=ub1ded09a-fd8c-4&from=paste&id=uc400283d&originHeight=598&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua1be95ca-54d6-48f1-a99d-b44af08e16e&title=)<br />其实从这可以看出，跟Java实现的差不多，只不过读的是META-INF/目录下spring.factories文件内容，然后解析出来键值对。
<a name="balgH"></a>
### 使用场景
Spring的SPI机制在内部使用的非常多，尤其在SpringBoot中大量使用，SpringBoot启动过程中很多扩展点都是通过SPI机制来实现的，这里举两个例子
<a name="ilg4Z"></a>
#### 1、自动装配
在SpringBoot3.0之前的版本，自动装配是通过SpringFactoriesLoader来加载的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696693-c19f2287-05f2-491e-a10f-0b386849f8cf.png#averageHue=%23332c2b&clientId=ub1ded09a-fd8c-4&from=paste&id=uc5a3794c&originHeight=358&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u494b7b4e-5202-44fb-a161-b27cbe9d3d4&title=)<br />但是SpringBoot3.0之后不再使用`SpringFactoriesLoader`，而是Spring重新从META-INF/spring/目录下的org.springframework.boot.autoconfigure.AutoConfiguration.imports文件中读取了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696648-87576584-84d4-4ec7-9fdc-cf70d3e6b39f.png#averageHue=%2356534a&clientId=ub1ded09a-fd8c-4&from=paste&id=uf55542f3&originHeight=286&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf98c6e4b-447b-4fb7-a4bf-13710746729&title=)<br />至于如何读取的，其实猜也能猜到就跟上面SPI机制读取的方式大概差不多，就是文件路径和名称不一样。
<a name="XOF9N"></a>
#### 2、`PropertySourceLoader`的加载
`PropertySourceLoader`是用来解析application配置文件的，它是一个接口<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696761-d5d39077-ab11-4be9-af48-2a04417274c9.png#averageHue=%232b2b2b&clientId=ub1ded09a-fd8c-4&from=paste&id=u37839df8&originHeight=608&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud64ba2df-7177-403c-b2a0-d756afa65c8&title=)<br />SpringBoot默认提供了 `PropertiesPropertySourceLoader` 和 `YamlPropertySourceLoader`两个实现，就是对应properties和yaml文件格式的解析。<br />SpringBoot在加载`PropertySourceLoader`时就用了SPI机制<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365696787-51a18ed2-dcbc-4274-a9b3-05d3259c23e8.png#averageHue=%23332d2b&clientId=ub1ded09a-fd8c-4&from=paste&id=u499de23f&originHeight=252&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u780f4e68-5f83-4526-a255-569e79a2772&title=)
<a name="glfSj"></a>
### 与Java SPI机制对比
首先Spring的SPI机制对Java的SPI机制对进行了一些简化，Java的SPI每个接口都需要对应的文件，而Spring的SPI机制只需要一个spring.factories文件。<br />其次是内容，Java的SPI机制文件内容必须为接口的实现类，而Spring的SPI并不要求键值对必须有什么关系，更加灵活。<br />第三点就是Spring的SPI机制提供了获取类限定名的方法`loadFactoryNames`，而Java的SPI机制是没有的。通过这个方法获取到类限定名之后就可以将这些类注入到Spring容器中，用Spring容器加载这些Bean，而不仅仅是通过反射。<br />但是Spring的SPI也同样没有实现获取指定某个指定实现类的功能，所以要想能够找到具体的某个实现类，还得依靠具体接口的设计。<br />所以`PropertySourceLoader`它其实就是一个策略接口，注释也有说，当配置文件是properties格式的时候，他可以找到解析properties格式的`PropertiesPropertySourceLoader`对象来解析配置文件。
<a name="Y7fwX"></a>
## Dubbo SPI机制--`ExtensionLoader`
`ExtensionLoader`是dubbo的SPI机制的实现类。每一个接口都会有一个自己的`ExtensionLoader`实例对象，这点跟Java的SPI机制是一样的。<br />同样地，Dubbo的SPI机制也做了以下几点约定：

- 接口必须要加@SPI注解
- 配置文件可以放在META-INF/services/、META-INF/dubbo/internal/ 、META-INF/dubbo/ 、META-INF/dubbo/external/这四个目录底下，文件名也是接口的全限定名
- 内容为键值对，键为短名称（可以理解为spring中Bean的名称），值为实现类的全限定名
<a name="eMMEp"></a>
### 先来个demo
首先在`LoadBalance`接口上`@SPI`注解
```java
@SPI
public interface LoadBalance {

}
```
然后，修改一下Java的SPI机制测试时配置文件内容，改为键值对，因为Dubbo的SPI机制也可以从META-INF/services/目录下读取文件，所以这里就没重写文件
```
random=com.sanyou.spi.demo.RandomLoadBalance
```
测试类：
```java
public class ExtensionLoaderDemo {

    public static void main(String[] args) {
        ExtensionLoader<LoadBalance> extensionLoader = ExtensionLoader.getExtensionLoader(LoadBalance.class);
        LoadBalance loadBalance = extensionLoader.getExtension("random");
        System.out.println("获取到random键对应的实现类对象：" + loadBalance);
    }

}
```
通过`ExtensionLoader`的`getExtension`方法，传入短名称，这样就可以精确地找到短名称对的实现类。<br />所以从这可以看出Dubbo的SPI机制解决了前面提到的无法获取指定实现类的问题。<br />测试结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365697176-8bbe880a-aa3d-44e8-8a26-f324c28b95f0.png#averageHue=%23353535&clientId=ub1ded09a-fd8c-4&from=paste&id=u7b771df0&originHeight=235&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u961ab836-6bc2-4ca6-ba07-5136fe06abc&title=)<br />dubbo的SPI机制除了解决了无法获取指定实现类的问题，还提供了很多额外的功能，这些功能在dubbo内部用的非常多，接下来就来详细讲讲。
<a name="ZyHT2"></a>
### dubbo核心机制
<a name="OIwZ1"></a>
#### 1、自适应机制
自适应，自适应扩展类的含义是说，基于参数，在运行时动态选择到具体的目标类，然后执行。<br />每个接口有且只能有一个自适应类，通过`ExtensionLoader`的`getAdaptiveExtension`方法就可以获取到这个类的对象，这个对象可以根据运行时具体的参数找到目标实现类对象，然后调用目标对象的方法。<br />举个例子，假设上面的`LoadBalance`有个自适应对象，那么获取到这个自适应对象之后，如果在运行期间传入了random这个key，那么这个自适应对象就会找到random这个key对应的实现类，调用那个实现类的方法，如果动态传入了其它的key，就路由到其它的实现类。<br />自适应类有两种方式产生，第一种就是自己指定，在接口的实现类上加`@Adaptive`注解，那么这个这个实现类就是自适应实现类。
```java
@Adaptive
public class RandomLoadBalance implements LoadBalance{
}
```
除了自己代码指定，还有一种就是dubbo会根据一些条件动态生成一个自适应类，生成过程比较复杂，这里就不展开了。<br />自适应机制在Dubbo中用的非常多，而且很多都是自动生成的，如果不知道Dubbo的自适应机制，在读源码的时候可能都不知道为什么代码可以走到那里。
<a name="milw4"></a>
#### 2、IOC和AOP
一提到IOC和AOP，立马想到的都是Spring，但是IOC和AOP并不是Spring特有的概念，Dubbo也实现IOC和AOP的功能，但是是一个轻量级的。
<a name="KbHM8"></a>
##### 2.1、依赖注入
Dubbo依赖注入是通过setter注入的方式，注入的对象默认就是上面提到的自适应的对象，在Spring环境下可以注入Spring Bean。
```java
public class RoundRobinLoadBalance implements LoadBalance {

    private LoadBalance loadBalance;

    public void setLoadBalance(LoadBalance loadBalance) {
        this.loadBalance = loadBalance;
    }

}
```
如上代码，`RoundRobinLoadBalance`中有一个`setLoadBalance`方法，参数`LoadBalance`，在创建`RoundRobinLoadBalance`的时候，在非Spring环境底下，Dubbo就会找到`LoadBalance`自适应对象然后通过反射注入。<br />这种方式在Dubbo中也很常见，比如如下的一个场景<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365697394-950e6bde-73ee-46a6-ad89-21907650e419.png#averageHue=%23994f36&clientId=ub1ded09a-fd8c-4&from=paste&id=u9a2fb60b&originHeight=504&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u32f5f941-026d-452f-8984-7030358a83f&title=)<br />`RegistryProtocol`中会注入一个`Protocol`，其实这个注入的`Protocol`就是一个自适应对象。
<a name="wYdlf"></a>
##### 2.2、接口回调
Dubbo也提供了一些类似于Spring的一些接口的回调功能，比如说，如果你的类实现了`Lifecycle`接口，那么创建或者销毁的时候就会回调以下几个方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365697572-b56d5771-c5be-454b-a7c8-71b1d834dbf2.png#averageHue=%23302f2e&clientId=ub1ded09a-fd8c-4&from=paste&id=ue1345936&originHeight=446&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf005b422-88d8-465b-b936-6074896e077&title=)<br />在dubbo3.x的某个版本之后，dubbo提供了更多接口回调，比如说`ExtensionPostProcessor`、`ExtensionAccessorAware`，命名跟Spring的非常相似，作用也差不多。
<a name="yMZqB"></a>
##### 2.3、自动包装
自动包装其实就是aop的功能实现，对目标对象进行代理，并且这个aop功能在默认情况下就是开启的。<br />在Dubbo中SPI接口的实现中，有一种特殊的类，被称为Wrapper类，这个类的作用就是来实现AOP的。<br />判断Wrapper类的唯一标准就是这个类中必须要有这么一个构造参数，这个构造方法的参数只有一个，并且参数类型就是接口的类型，如下代码：
```java
public class RoundRobinLoadBalance implements LoadBalance {

    private final LoadBalance loadBalance;

    public RoundRobinLoadBalance(LoadBalance loadBalance) {
        this.loadBalance = loadBalance;
    }

}
```
此时`RoundRobinLoadBalance`就是一个`Wrapper`类。<br />当通过random获取`RandomLoadBalance`目标对象时，那么默认情况下就会对`RandomLoadBalance`进行包装，真正获取到的其实是`RoundRobinLoadBalance`对象，`RoundRobinLoadBalance`内部引用的对象是RandomLoadBalance。<br />_**测试一下**_<br />在配置文件中加入
```
roundrobin=com.sanyou.spi.demo.RoundRobinLoadBalance
```
测试结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365697654-3e8d186a-e898-4150-a694-aae900587262.png#averageHue=%234e5e44&clientId=ub1ded09a-fd8c-4&from=paste&id=ue563f17e&originHeight=336&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b30dfc2-ceac-41f4-880a-a5e2b63da38&title=)<br />从结果可以看出，虽然指定了random，但是实际获取到的是`RoundRobinLoadBalance`，而`RoundRobinLoadBalance`内部引用了`RandomLoadBalance`。<br />如果有很多的包装类，那么就会形成一个责任链条，一个套一个。<br />所以dubbo的aop跟spring的aop实现是不一样的，spring的aop底层是基于动态代理来的，而dubbo的aop其实算是静态代理，dubbo会自动组装这个代理，形成一条责任链。<br />到这其实已经知道，dubbo的spi接口的实现类已经有两种类型了：

- 自适应类
- Wrapper类

除了这两种类型，其实还有一种，叫做默认类，就是`@SPI`注解的值对应的实现类，比如
```java
@SPI("random")
public interface LoadBalance {

}
```
此时random这个key对应的实现类就是默认实现，通过`getDefaultExtension`这个方法就可以获取到默认实现对象。
<a name="ooLrw"></a>
#### 3、自动激活
所谓的自动激活，就是根据入参，动态地选择一批实现类返回给你。<br />自动激活的实现类上需要加上`Activate`注解，这里就又学习了一种实现类的分类。
```java
@Activate
public interface RandomLoadBalance {

}
```
此时`RandomLoadBalance`就属于可以被自动激活的类。<br />获取自动激活类的方法是`getActivateExtension`，所以根据这个方法的入参，可以动态选择一批实现类。<br />自动激活这个机制在Dubbo一个核心的使用场景就是Filter过滤器链中。<br />Filter是dubbo中的一个扩展点，可以在请求发起前或者是响应获取之后就行拦截，作用有点像Spring MVC中的`HandlerInterceptor`。<br />![Filter的一些实现类](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365697715-cdfe0029-b53b-4586-ba0b-67d05ab15363.png#averageHue=%23524e44&clientId=ub1ded09a-fd8c-4&from=paste&id=ub5a816cd&originHeight=532&originWidth=918&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc724552f-c38a-468f-b03d-c6308776c92&title=Filter%E7%9A%84%E4%B8%80%E4%BA%9B%E5%AE%9E%E7%8E%B0%E7%B1%BB "Filter的一些实现类")<br />如上Filter有很多实现，所以为了能够区分Filter的实现是作用于provider的还是consumer端，所以就可以用自动激活的机制来根据入参来动态选择一批Filter实现。<br />比如说`ConsumerContextFilter`这个Filter就作用于`Consumer`端。<br />![ConsumerContextFilter](https://cdn.nlark.com/yuque/0/2023/png/396745/1679365697723-dee3a38d-4a7d-4f05-9497-587f190757e4.png#averageHue=%232e2d2b&clientId=ub1ded09a-fd8c-4&from=paste&id=u66beb91c&originHeight=292&originWidth=1024&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u7ffefdee-2815-42c3-b684-74fae3254b6&title=ConsumerContextFilter "ConsumerContextFilter")
<a name="hRbSU"></a>
## 总结
通过以上分析可以看出，实现SPI机制的核心原理就是通过IO流读取指定文件的内容，然后解析，最后加入一些自己的特性。<br />最后总的来说，Java的SPI实现的比较简单，并没有什么其它功能；Spring得益于自身的ioc和aop的功能，所以也没有实现太复杂的SPI机制，仅仅是对Java做了一点简化和优化；但是dubbo的SPI机制为了满足自身框架的使用要求，实现的功能就比较多，不仅将ioc和aop的功能集成到SPI机制中，还提供注入自适应和自动激活等功能。
