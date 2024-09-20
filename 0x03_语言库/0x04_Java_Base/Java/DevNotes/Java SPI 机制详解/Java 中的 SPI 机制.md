Java<br />在日常工作中经常会听到 SPI，而且很多框架都使用了 SPI 的技术，那么问题来了，到底什么是 SPI 呢？
<a name="lmEjk"></a>
## SPI 概念
SPI 全称是 Service Provider Interface，是一种 JDK 内置的动态加载实现扩展点的机制，通过 SPI 技术可以动态获取接口的实现类，不用自己来创建。<br />这里提到了接口和实现类，那么 SPI 技术上具体有哪些技术细节呢？

1. 接口：需要有一个功能接口；
2. 实现类：接口只是规范，具体的执行需要有实现类才行，所以不可缺少的需要有实现类；
3. 配置文件：要实现 SPI 机制，必须有一个与接口同名的文件存放于类路径下面的  META-INF/services 文件夹中，并且文件中的每一行的内容都是一个实现类的全路径；
4. 类加载器 ServiceLoader：JDK 内置的一个类加载器，用于加载配置文件中的实现类；
<a name="rBX1g"></a>
## 举个栗子
上面说了 SPI 的几个概念，接下来就通过一个栗子来带大家感受一下具体的用法。
<a name="jQzTz"></a>
### 第一步
创建一个接口，这里创建一个解压缩的接口，其中定义了压缩和解压的两个方法。
```java
package com.example.demo.spi;

/**
 * <br>
 * <b>Function：</b><br>
 * <b>Author：</b>@author ziyou<br>
 * <b>Date：</b>2022-10-08 21:31<br>
 * <b>Desc：</b>无<br>
 */
public interface Compresser {
  byte[] compress(byte[] bytes);
  byte[] decompress(byte[] bytes);
}
```
<a name="OXly9"></a>
### 第二步
再写两个对应的实现类，分别是 GzipCompresser.java 和 WinRarCompresser.java 代码如下
```java
package com.example.demo.spi.impl;

import com.example.demo.spi.Compresser;

import java.nio.charset.StandardCharsets;

/**
 * <br>
 * <b>Function：</b><br>
 * <b>Author：</b>@author ziyou<br>
 * <b>Date：</b>2022-10-08 21:33<br>
 * <b>Desc：</b>无<br>
 */
public class GzipCompresser implements Compresser {
  @Override
  public byte[] compress(byte[] bytes) {
    return"compress by Gzip".getBytes(StandardCharsets.UTF_8);
  }
  @Override
  public byte[] decompress(byte[] bytes) {
    return "decompress by Gzip".getBytes(StandardCharsets.UTF_8);
  }
}
```
```java
package com.example.demo.spi.impl;

import com.example.demo.spi.Compresser;

import java.nio.charset.StandardCharsets;

/**
 * <br>
 * <b>Function：</b><br>
 * <b>Author：</b>@author ziyou<br>
 * <b>Date：</b>2022-10-08 21:33<br>
 * <b>Desc：</b>无<br>
 */
public class WinRarCompresser implements Compresser {
  @Override
  public byte[] compress(byte[] bytes) {
    return "compress by WinRar".getBytes(StandardCharsets.UTF_8);
  }

  @Override
  public byte[] decompress(byte[] bytes) {
    return "decompress by WinRar".getBytes(StandardCharsets.UTF_8);
  }
}
```
<a name="J5vcb"></a>
### 第三步
创建配置文件，接着在 resources 目录下创建一个名为 META-INF/services 的文件夹，在其中创建一个名为 com.example.demo.spi.Compresser 的文件，其中的内容如下：
```
com.example.demo.spi.impl.WinRarCompresser
com.example.demo.spi.impl.GzipCompresser
```
注意该文件的名称必须是接口的全路径，文件里面的内容每一行都是一个实现类的全路径，多个实现类就写在多行里面，效果如下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665274909099-7c8a46b0-ca88-4cec-b68b-dc081f03c959.png#clientId=u540a7a3a-4a11-4&from=paste&id=u440200f2&originHeight=541&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua87484d3-d552-45b4-9eab-f553e6bf37c&title=)
<a name="r0y36"></a>
### 第四步
有了上面的接口，实现类和配置文件，接下来就可以使用 ServiceLoader 动态加载实现类，来实现 SPI 技术了，如下所示：
```java
package com.example.demo;

import com.example.demo.spi.Compresser;

import java.nio.charset.StandardCharsets;
import java.util.ServiceLoader;

public class TestSPI {
    public static void main(String[] args) {
        ServiceLoader<Compresser> compressers = ServiceLoader.load(Compresser.class);
        for (Compresser compresser : compressers) {
            System.out.println(compresser.getClass());
        }
    }
}
```
运行的结果如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665274909099-edef04f6-348d-40f5-81b0-622ac59b015b.png#clientId=u540a7a3a-4a11-4&from=paste&id=ub8a743c6&originHeight=280&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u46fd38b7-6627-4b1e-b7a0-63992d732ec&title=)<br />可以看到正常的获取到了接口的实现类，并且可以直接使用实现类的解压缩方法。
<a name="C6tQT"></a>
## 原理
知道了如何使用 SPI 接下来研究一下是如何实现的，通过上面的测试可以看到，核心的逻辑是 `ServiceLoader.load()` 方法，这个方法有点类似于 Spring 中的根据接口获取所有实现类一样。<br />点开 ServiceLoader 可以看到有一个常量 PREFIX，如下所示，这也是为什么必须在这个路径下面创建配置文件，因为 JDK 代码里面会从这个路径里面去读取配置文件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665274909129-22d8d2b3-e83e-425e-90f2-9b4290f7aa7e.png#clientId=u540a7a3a-4a11-4&from=paste&id=uec866fc8&originHeight=292&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud4f33855-a43e-4cc4-ba09-93dfa2f3828&title=)<br />同时又因为在读取文件的时候使用了 class 的路径名称，因为使用 `load` 方法的时候只会传递一个 class，所以文件名也必须是接口的全路径。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665274909090-093b9b00-bf3e-41c3-9eab-3ad3412ecf58.png#clientId=u540a7a3a-4a11-4&from=paste&id=ua24e6d7e&originHeight=519&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u85b80a42-a7dd-45b7-a8e0-409edc8de53&title=)<br />通过 `load` 方法可以看到底层构造了一个 java.util.ServiceLoader.LazyIterator 迭代器。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665274909113-b899e93d-d798-4679-a6ec-736b3980f0ef.png#clientId=u540a7a3a-4a11-4&from=paste&id=u3ca642f7&originHeight=208&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u811b82c0-457e-4f58-a1c8-f83016e87ba&title=)<br />在迭代器中的 `parse` 方法中，就获取了配置文件中的实现类名称集合，然后在通过反射创建出具体的实现类对象存放到 `LinkedHashMap<String,S> providers = new LinkedHashMap<>();` 中。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665274909806-b74494fe-6668-4a7c-9839-60ef891b4441.png#clientId=u540a7a3a-4a11-4&from=paste&id=uba95e277&originHeight=727&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua9abc17f-1c3e-49a9-8f5b-789efd88e55&title=)
<a name="nHw6G"></a>
## 常用的框架
SPI 技术的使用非常广泛，比如在 Dubble，不过 Dubble 中的 SPI 有经过改造的，还有很常见的数据库的驱动中也使用了 SPI，还有 SLF4J 用来加载不同提供商的日志实现类以及 Spring 框架等。
<a name="z59ro"></a>
## 优缺点
前面介绍了 SPI 的原理和使用，那 SPI 有什么优缺点呢？
<a name="QtqH5"></a>
### 优点
优点当然是解耦，服务方只要定义好接口规范就好了，具体的实现可以由不同的 Jar 进行实现，只要按照规范实现功能就可以被直接拿来使用，在某些场合会被进行热插拔使用，实现了解耦的功能。
<a name="ncyDl"></a>
### 缺点
一个很明显的缺点那就是做不到按需加载，通过源码看到了是会将所有的实现类都进行创建的，这种做法会降低性能，如果某些实现类实现很耗时了话将影响加载时间。同时实现类的命名也没有规范，让使用者不方便引用。
