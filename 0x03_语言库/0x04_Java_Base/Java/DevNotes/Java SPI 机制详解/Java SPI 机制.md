Java SPI<br />Java SPI 是基于**接口的编程＋策略模式＋约定配置文件**组合实现的动态加载机制，能够很方便的为某个接口寻找服务实现的机制。<br />来深入聊一下SPI。
<a name="qLZBf"></a>
## 什么是SPI？
SPI 全称：**Service Provider Interface**，是Java提供的一套用来被第三方实现或者扩展的接口，它可以用来启用框架扩展和替换组件。<br />面向的对象的设计里，一般推荐模块之间基于接口编程，模块之间不对实现类进行硬编码。一旦代码里涉及具体的实现类，就违反了可拔插的原则，如果需要替换一种实现，就需要修改代码。<br />为了实现在模块装配的时候不用在程序里动态指明，这就需要一种服务发现机制。java spi就是提供这样的一个机制：为某个接口寻找服务实现的机制。这有点类似IOC的思想，将装配的控制权移到了程序之外。<br />这是一种JDK内置的一种服务发现的机制，用于制定一些规范，实际实现方式交给不同的服务厂商。如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651800392211-01fab624-c8d9-48ff-842f-7711e478985b.png#clientId=u9ebe409c-dbf3-4&from=paste&id=u1f5b7e49&originHeight=359&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u15b8141a-e187-4975-86be-eed468114b7&title=)<br />解耦、可拔插、面向接口编程、动态类加载。<br />当服务的提供者提供了一种接口的实现之后，需要在**classpath**下的 **META-INF/services/** 目录里创建一个以服务接口命名的文件，这个文件里的内容就是这个接口的具体的实现类。<br />当其他的程序需要这个服务的时候，就可以通过查找这个jar包（一般都是以jar包做依赖）的META-INF/services/中的配置文件，配置文件中有接口的具体实现类名，可以根据这个类名进行加载实例化，就可以使用该服务了。JDK中查找服务的实现的工具类是：**java.util.ServiceLoader**。
<a name="Mjxl8"></a>
## SPI 的不足

1. 不能按需加载，需要遍历所有的实现，并实例化，然后在循环中才能找到需要的实现。如果不想用某些实现类，或者某些类实例化很耗时，它也被载入并实例化了，这就造成了浪费。
2. 获取某个实现类的方式不够灵活，只能通过 Iterator 形式获取，不能根据某个参数来获取对应的实现类。（Spring 的`BeanFactory`，`ApplicationContext` 就要高级一些了。）
3. 多个并发**多线程**使用 `ServiceLoader` 类的实例是不安全的。
<a name="RhXza"></a>
## API 与 SPI 区别？
API是调用并用于实现目标的类、接口、方法等的描述；<br />SPI是扩展和实现以实现目标的类、接口、方法等的描述；<br />换句话说，API 为操作提供特定的类、方法，SPI 通过操作来符合特定的类、方法。
<a name="YVv2a"></a>
## SPI和API的使用场景解析：

- **API （Application Programming Interface）**在大多数情况下，都是实现方制定接口并完成对接口的实现，调用方仅仅依赖接口调用，且无权选择不同实现。从使用人员上来说，API 直接被应用开发人员使用。
- **SPI （Service Provider Interface）**是调用方来制定接口规范，提供给外部来实现，调用方在调用时则选择自己需要的外部实现。 从使用人员上来说，SPI 被框架扩展人员使用。
<a name="tPvcS"></a>
## SPI 案例实现
下面来一个简单的案例实现：比如每个动物都有不同的叫声，作为声纹系统会定义一个接口，如下：
```java
public interface AnimalSay {
	void say();
}
```
在这个系统中并没有实现具体的实现，但是在处理业务逻辑时有需要用到该实例，此时就需要用到SPI去加载实现类，定义一个AnimalManagerLoader，实现如下：
```java
@Data
public class AnimalManagerLoader {
    private static final AnimalManagerLoader INSTANCE = new AnimalManagerLoader();

    private final List<AnimalSay> animalSays;

    private AnimalManagerLoader() {
        animalSays = load();
    }

    /**
     * 通过SPI加载实现类
     */
    private List<AnimalSay> load() {
        ArrayList<AnimalSay> animalSays = new ArrayList<>();
        Iterator<AnimalSay> iterator = ServiceLoader.load(AnimalSay.class).iterator();
        while (iterator.hasNext()){
            animalSays.add(iterator.next());
        }
        return animalSays;
    }

    public static AnimalManagerLoader getInstance() {
        return INSTANCE;
    }
}
```
此时就可以通过`**AnimalManagerLoader**`中的`load`方法去加载对应的实现类，封装到List集合中，调用如下：
```java
public static void main(String[] args) {
	AnimalManagerLoader animalManagerLoader = AnimalManagerLoader.getInstance();
	List<AnimalSay> animalSays = animalManagerLoader.getAnimalSays();
	for (AnimalSay animalSay : animalSays) {
		animalSay.say();
	}
}
```
那么此时提供声音的厂家就需要实现这个接口，比如狗狗的声纹厂家，实现如下：
```java
/**
 * 狗狗的声纹
 */
public class DogSay implements AnimalSay {
    public void say() {
        System.out.println("wang wang ~");
    }
}
```
猫咪的声纹如下：
```java
/**
 * 猫咪的声纹
 */
public class CatSay implements AnimalSay {
    @Override
    public void say() {
        System.out.println("miao miao ~");
    }
}
```
实现类定义了，就需要在 **/META-INF/services** 中定义一个 **com.myjszl.animal.api.AnimalSay**文件，内容如下：
```java
com.myjszl.dog.api.DogSay
com.myjszl.dog.api.CatSay
```
<a name="a3qj6"></a>
## SPI 应用场景
SPI扩展机制应用场景有很多，比如Common-Logging，JDBC，Dubbo、ShardingSphere等等。
<a name="pvvvl"></a>
### 1、JDBC场景
java中定义的java.sql.Driver接口，并没有具体的实现，实现方式而是交给不同的服务厂商：

1. 在MySQL的jar包mysql-connector-java-6.0.6.jar中，可以找到**META-INF/services**目录，该目录下会有一个名字为java.sql.Driver的文件，文件内容是com.mysql.cj.jdbc.Driver，这里面的内容就是针对Java中定义的接口的实现。
2. PostgreSQL的jar包PostgreSQL-42.0.0.jar中，也可以找到同样的配置文件，文件内容是org.postgresql.Driver，这是PostgreSQL对Java的java.sql.Driver的实现。
<a name="dlLPC"></a>
### 2、ShardingSphere的场景
在ShardingSphere中为了实现分布式事务提供了一个接口`**ShardingTransactionManager**`，但是在其架构中并未对其做出具体的实现，而是交给不同的厂商去实现，比如JTA强一致性事务的`**XAShardingTransactionManager**`，在其中**META-INF/services**就有一个**org.apache.shardingsphere.transaction.spi.ShardingTransactionManager**文件，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651800392220-4d5d0c75-8f3b-4b13-9baa-46b1468bbe2b.png#clientId=u9ebe409c-dbf3-4&from=paste&id=u39a0c148&originHeight=305&originWidth=890&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubc7ce26c-90cb-499f-b0b4-4b62087d66c&title=)<br />以上只是简单的列举了几个场景，实际应用场景很多，比如Spring、Spring Boot 中都有用到SPI设计。
<a name="ctnHR"></a>
### 3、Spring 场景
Spring中大量使用了SPI；比如：对servlet3.0规范对`**ServletContainerInitializer**`的实现、自动类型转换Type Conversion SPI(Converter SPI、Formatter SPI)等
<a name="XrDmM"></a>
### 4、SLFJ 日志门面 场景
SLF4J加载不同提供商的日志实现类，比如log4j、log4j2、logback.....
<a name="Jjk8F"></a>
## 总结
通过Java的SPI机制能够很方便的实现可插拔、解耦的功能设计，在日常的开发中要能想到该机制并能灵活的运用。
