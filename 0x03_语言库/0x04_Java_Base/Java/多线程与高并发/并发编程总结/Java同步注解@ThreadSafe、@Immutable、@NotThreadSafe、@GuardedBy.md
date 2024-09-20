Java
<a name="cVYQG"></a>
## 1、序
Java同步注解: `@ThreadSafe`、`@Immutable`、`@NotThreadSafe`、`@GuardedBy`<br />`@Immutable`、`@ThreadSafe`、`@NotThreadSafe`、`@GuardedBy` 都不是 JDK 的标准注释。如果需要使用它们，需要导入额外的依赖项：jcip-annotations.jar。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683592060502-9f50ce10-c1a6-4227-97fe-fe8037526e6d.png#averageHue=%23f6f6f6&clientId=u1a5dacdf-a304-4&from=paste&id=u3168a3d7&originHeight=774&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u10983eb8-11c9-4ec7-b238-a0d2ae20076&title=)
```xml
<!-- https://mvnrepository.com/artifact/net.jcip/jcip-annotations -->
<dependency>
  <groupId>net.jcip</groupId>
  <artifactId>jcip-annotations</artifactId>
  <version>1.0</version>
  <scope>provided</scope>
</dependency>
```
在Java并发编程中，这些专门为并发编程准备的 Annotation。主要包括三类：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683592060600-525f870f-c8a9-4044-a5eb-43a719a61cda.png#averageHue=%2346433a&clientId=u1a5dacdf-a304-4&from=paste&id=u1768a486&originHeight=160&originWidth=508&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7515a90c-b802-4a5a-9aa0-e82d4d111af&title=)
<a name="YGV6w"></a>
## 2、类 Annotation（注解）
就像名字一样，这些注解是针对类的。主有要以下三个：
<a name="vIZDk"></a>
### `@ThreadSafe`
表示这个类是线程安全的。具体是否真安全，那要看实现者怎么实现的了，反正打上这个标签只是表示一下。不线程安全的类打上这个注解也没事儿。
<a name="V1PXY"></a>
### `@Immutable`
表示类是不可变的，包含了`@ThreadSafe`的意思。
<a name="s8b3v"></a>
### `@NotThreadSafe`
表示这个类不是线程安全的。如果是线程安全的非要打上这个注解，那也不会报错。
<a name="LFa1c"></a>
### 小结
这三个注解，对用户和维护者是有益的，用户可以立即看出来这个类是否是线程安全的，维护者则是可以根据这个注解，重点检查线程安全方面。另外，代码分析工具可能会利用这个注解。<br />使用加锁的类中，应该指明那些状态变量由那些锁保护，以及那些锁被用于保护这些变量，一种常见的不安全性的常见原因是：某个线程安全的类一直通过加锁来保护器状态，但随后又对这个类进行了修改，并添加了一些未通过加锁来保护的新变量，或者没有正确的枷锁保护先有个状态变量的新方法。
<a name="b42Dz"></a>
## 3、域 Annotation（注解）
域注解是对类里面成员变量加的注解。
<a name="oF1SC"></a>
## 4、方法 Annotation（注解）
方法注解是对类里面方法加的注解。
<a name="WshZT"></a>
### 小结
域注解和方法注解都是用`@GuardedBy(lock)`来标识。<br />里面的lock是告诉维护者：这个状态变量，这个方法被哪个锁保护着。这样可以强烈的提示类的维护者注意这里。
<a name="I2fAM"></a>
## 5、`@GuardedBy(lock)` 使用形式
`@GuardedBy(lock)`，意味着有保护的字段或方法只能在线程持有锁时被某些线程访问。<br />描述哪个状态变量被哪个锁保护着，以及哪个锁保护这些变量的信息：<br />`@GuardedBy(lock)`：线程只有在持有了一个特定的锁（lock）后，才能访问某个域或方法；可以将锁定指定为以下类型：

- `@GuardedBy("this")`：受对象内部锁保护；即它是指对象中的内部锁（方法或域是这个对象的一个成员）。
- `@GuardedBy("fieldName")`：受与fieldName引用相关联的锁保护；即它是指与filedName引用的对象相关联的锁，或者是一个隐式锁（filedName没有引用一个Lock），或者是一个显示锁（filedName引用了一个Lock）。
- `@GuardedBy("ClassName.fieldName")`：受一个类的静态field的锁保存；即它是类似于`@GuardedBy("fieldName")`，不过所引用的锁对象是存储在另一个类（或本类）中的静态域。
- `@GuardedBy("methodName()")`：锁对象是 `methodName()` 方法的返回值，受这个锁保护；即它是指该方法返回值。
- `@GuardedBy("ClassName.class")`：受ClassName类的直接锁对象保护。而不是这个类的某个实例的锁对象；即它是指定类的Class对象用作锁定对象。
<a name="LGKBD"></a>
### 样例：
```java
import javax.annotation.concurrent.*;;  

@ThreadSafe  
public class Sequence {  

    @GuardedBy("this") private int value;  

    public synchronized int getNext() {  
        return value++;  
    }  
}
```
