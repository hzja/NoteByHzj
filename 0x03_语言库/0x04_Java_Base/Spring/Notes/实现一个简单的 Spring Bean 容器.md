JavaSpring<br />面对复杂的源码，试着找到开头和结尾是一件非常具有挑战的事。为了让更多的初学者上手，从本文开始，将通过实践的方式带领读者逐步实现 Spring 框架的核心链路和功能逻辑。<br />简化上手、凸显重点、摒弃冗余，让更多的初学者都能在这场学习旅途中收获满满。旅途即将开始，准备好了吗？

- 本文难度：★☆☆☆☆
- 本文重点：基于 Spring Bean 容器的存储功能和读取功能，采用时间复杂度为O(log n) 的 HashMap 数据结构进行设计和实现。
<a name="SlEao"></a>
## 容器是什么
Spring Bean 包含并管理应用对象的配置和生命周期。从这个意义上讲，它是一种用于承载对象的容器，开发者可以设置每个 Bean 对象是如何被创建的，以及它们是如何互相关联、构建和使用的。使用这些 Bean 对象可以创建一个单独的实例，或者在需要时生成一个新的实例。<br />如果将一个 Bean 对象交给 Spring Bean 容器管理，则这个 Bean 对象会以类似零件的方式被拆解，然后存储到 Spring Bean 容器的定义中，便于 Spring Bean 容器管理，这相当于把对象解耦。<br />当一个 Bean 对象被定义和存储后，会由 Spring Bean 容器统一进行分配，这个过程包括 Bean 对象的初始化和属性填充等。最终，可以完整地使用一个被实例化后的Bean 对象。<br />本文将带领读者实现一个简单的 Spring Bean 容器，用于定义、存储和获取 Bean对象。
<a name="WdjIS"></a>
## 简单容器设计
将可以存储数据的数据结构称为容器，如 ArrayList、LinkedList、HashSet 等。但在 Spring Bean 容器中，需要一种可以用于存储对象和使用对象名称进行便捷索引的数据结构，所以选择 HashMap 数据结构是最合适的，下面简单介绍 HashMap。<br />HashMap 是一种基于扰动函数、负载因子和红黑树转换等技术形成的拉链寻址的数据结构，能让数据更加均匀地分布在哈希桶，以及碰撞时形成的链表和红黑树上。<br />HashMap 的数据结构会最大限度地使整个数据读取的复杂度范围为 O(1) ～ O(n)，也存在较多使用复杂度为 O(n) 的链表查找数据的情况。不过，经过 10 万个单词数据的扰动函数索引计算后，通过在寻址位置膨胀的方差稳定性对比验证得出，使用扰动函数的数据会更均匀地分布在各个哈希桶索引上，基于这些特性的 HashMap 非常适合用于实现Spring Bean 容器。<br />另外，实现一个简单的 Spring Bean 容器，还需要完成 Bean 对象的定义、注册和获取 3 个基本步骤，如图 1-1 所示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668323675910-d493ac37-d129-4d76-b223-2a9a250d7902.png#averageHue=%23f6f5f4&clientId=udd662df3-e487-4&from=paste&id=u514172f1&originHeight=556&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub65f541a-248c-46ab-8c0e-ca4caf64f4a&title=)
<a name="cUhjj"></a>
### 1、定义
`BeanDefifinition` 是在查阅 Spring 源码时经常看到的一个类，如 `singleton` 属性、`prototype` 属性和 BeanClassName 类型等。这里的实现会采用更加简单的方法，只需要定义一个 Object 类型用于存储任意类型的对象。
<a name="cxcT5"></a>
### 2、注册
注册过程相当于把数据存储到 HashMap 中，现在 HashMap 中存储的是被定义的Bean 对象的信息。
<a name="YWj1f"></a>
### 3、获取
最后就是获取对象。Bean 对象的名字就是 key。当 Spring Bean 容器初始化 Bean 对象后，Bean 对象就可以被直接获取。按照上述设计过程，来实现一个简单的 Spring Bean 容器。
<a name="mc6Yw"></a>
## 简单容器实现
<a name="oZZ1k"></a>
### 1、工程结构
```
spring-step-01
└── src
 ├── main
 │ └── java
 │ └── cn.bugstack.springframework
 │ ├── BeanDefinition.java
 │ └── BeanFactory.java
 └── test
 └── java
 └── cn.bugstack.springframework.test
 ├── bean
 │ └── UserService.java
 └── ApiTest.java
```
Spring Bean 容器中类的关系如图 1-2 所示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668323675919-9e96638d-1b24-4b35-8a95-351635bba2bb.png#averageHue=%23f3f0ec&clientId=udd662df3-e487-4&from=paste&id=u43c4a99d&originHeight=803&originWidth=432&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u752a2311-7486-4ef7-af23-5cdd715f1f6&title=)<br />Spring Bean 容器实现 的 内 容 非 常 简 单， 仅 包 括 一 个 简 单 的 `BeanFactory` 类和`BeanDefifinition` 类。这里的类名称与 Spring 源码中的类名称一致，只是实现起来会相对简化一些。在后续的扩展过程中，可以不断地添加内容。

- `BeanDefifinition`：用于定义 Bean 对象，它的实现方式是以一个 Object 类型存储对象。
- `BeanFactory`：代表 Bean 对象的工厂，可以将 Bean 对象的定义存储到 Map 中以便获取 Bean 对象。
<a name="NQ6mu"></a>
### 2、Bean 对象的定义
源码详见：cn.bugstack.springframework.BeanDefifinition。
```java
public class BeanDefinition {
    private Object bean;
    public BeanDefinition(Object bean) {
        this.bean = bean;
    }
    public Object getBean() {
        return bean;
    }
}
```
在目前的 Bean 对象定义中，只有一个 Object 类型，因为它是所有类型的父类，可以存储任意类型的 Bean 对象。读者可以参考 Spring 源码中这个类的信息，其名称都是一样的。<br />在后续章节的实现过程中，会逐步完善 `BeanDefifinition` 类对 Bean 对象定义的相关属性字段，如`propertyValues`、`initMethodName`、`destroyMethodName`、`scope`、`beanClass`等。
<a name="ySkl8"></a>
### 3、Bean 工厂
源码详见：cn.bugstack.springframework.BeanFactory。
```java
public class BeanFactory {
    private Map<String, BeanDefinition> beanDefinitionMap = new ConcurrentHashMap<>();
    public Object getBean(String name) {
        return beanDefinitionMap.get(name).getBean();
    }
    public void registerBeanDefinition(String name, BeanDefinition beanDefinition) {
        beanDefinitionMap.put(name, beanDefinition);
    }
}
```
`BeanFactory` 类是用于生成和使用对象的 Bean 工厂，`BeanFactory` 类的实现过程包括Bean 对象的注册和获取，这里注册的是 Bean 对象的定义信息。<br />目前，`BeanFactory` 类的实现是非常简化的，但这种简化的实现却是整个 Spring Bean容器中关于 Bean 对象使用的最终体现，只不过在实现过程中只展示出了基本的核心原理。在后续的补充实现中，这个类的内容会不断增加。
<a name="yJrej"></a>
## 容器使用测试
<a name="Bw68O"></a>
### 1、事先准备
源码详见：cn.bugstack.springframework.test.bean.UserService。
```java
public class UserService {
    public void queryUserInfo(){
        System.out.println(" 查询用户信息 ");
    }
}
```
这里简单定义了一个 UserService 对象类，方便后续对 Spring Bean 容器进行测试。
<a name="YJWmO"></a>
### 2、测试实例
源码详见：cn.bugstack.springframework.test.ApiTest。
```java
@Test
public void test_BeanFactory(){
    // 1. 初始化 BeanFactory 接口
    BeanFactory beanFactory = new BeanFactory();
    // 2. 注册 Bean 对象
    BeanDefinition beanDefinition = new BeanDefinition(new UserService());
    beanFactory.registerBeanDefinition("userService", beanDefinition);
    // 3. 获取 Bean 对象
    UserService userService = (UserService) beanFactory.getBean("userService");
    userService.queryUserInfo();
}
```
在单元测试中，主要包括初始化 `BeanFactory` 接口、注册 Bean 对象、获取 Bean 对象 3 个步骤，在使用效果上贴近于 Spring 框架，但这里会更加简化一些。<br />在注册 Bean 对象的过程中，这里直接把 UserService 类实例化后作为入参信息传递给 `BeanDefifinition`。在后续的代码中，会将这部分内容放入 Bean 工厂中实现。
<a name="Zy8aZ"></a>
### 3、测试结果
查询用户信息
```
Process finished with exit code 0
```
从测试结果中可以看到，目前的 Spring Bean 容器案例已经有了雏形。
<a name="umtmE"></a>
## 总结
关于 Spring Bean 容器的一个简单实现已经完成了，这部分代码相对简单，稍加尝试就可以实现这部分内容。<br />但对于学习知识的过程来说，写代码只是最后的步骤，形成整体思路和设计方案才是更重要的。只有厘清了思路，才能真正地理解代码。
<a name="e82qs"></a>
### 学习 Spring 源码难吗？
**难！**<br />难到有1~2年编程经验的工程师，也不知道从哪里下手。<br />大部分资料和书籍都是从一个知识点直接透析到内核。<br />没有阅读源码经验的小白，根本没法如编写者般感同身受地学习，看时如云里雾里，忘时如过眼云烟。
<a name="g6Pgg"></a>
### 为啥会这样？
因为 Spring产生太久了，源码太大了，主干核心源码外的繁枝末节太多太多。就像将一个沙发左移、套个沙发罩、盖上一块布，再铺个小坐垫，沙发套、盖布、坐垫对初学源码的开发者来说，并没有那么重要，要了解的是最初的沙发、最开始的木板，看看它的结构。<br />所以，如果真的想了解Spring 这个源码级复杂框架的设计和实现精髓，就应该像开发一个项目一样，从零写一遍。只有写了这个项目，才能知道具体细节是如何处理的，各类设计是如何实现的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668323675828-92fc789b-10b1-4b79-a3f8-1154879cf7b7.png#averageHue=%23fbfaf8&clientId=udd662df3-e487-4&from=paste&id=udf48e533&originHeight=1500&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua0e3f7af-0989-4665-9278-2cb2049aa16&title=)
