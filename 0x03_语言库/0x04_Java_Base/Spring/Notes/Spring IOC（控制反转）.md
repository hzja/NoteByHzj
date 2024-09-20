Spring IOC <br />Spring容器的顶层接口是：`BeanFactory`，但使用更多的是它的子接口：`ApplicationContext`。<br />通常情况下，如果想要手动初始化通过`xml文件`配置的Spring容器时，代码是这样的：
```java
ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
User user = (User)applicationContext.getBean("name");
```
如果想要手动初始化通过配置类配置的Spring容器时，代码是这样的：
```java
AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Config.class);
User user = (User)applicationContext.getBean("name");
```
这两个类应该是最常见的入口了，它们却殊途同归，最终都会调用`refresh`方法，该方法才是Spring容器初始化的真正入口。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493154-a39d975f-ae78-47eb-b372-19243768bd9f.png#averageHue=%232f2d2c&height=300&id=rKBFt&originHeight=300&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493114-c6501cf9-1f2e-41cf-8faa-b063b6845fb2.png#averageHue=%23312d2c&height=206&id=KZKUM&originHeight=206&originWidth=1072&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1072)<br />其实调用`refresh`方法的类并非只有这两个，用一张图整体认识一下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493142-04e56fed-d1e0-414e-985e-403d84dcf82e.png#averageHue=%23f6f6f5&height=1030&id=znMLB&originHeight=1030&originWidth=1006&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1006)<br />虽说调用`refresh`方法的类有这么多，这里用`ClassPathXmlApplicationContext`类作为列子进行分析，因为它足够经典，而且难度相对来说要小一些。
<a name="12ki5"></a>
## refresh方法
`refresh`方法是`spring ioc`的真正入口，它负责初始化Spring容器。<br />既然这个方法的作用是初始化Spring容器，那方法名为啥不叫`init`？<br />答案很简单，因为它不只被调用一次。<br />在SpringBoot的`SpringAppication`类中的`run`方法会调用`refreshContext`方法，该方法会调用一次`refresh`方法。<br />在SpringCloud的`BootstrapApplicationListener`类中的`onApplicationEvent`方法会调用`SpringAppication`类中的`run`方法。也会调用一次`refresh`方法。
> 这是SpringBoot项目中如果引入了SpringCloud，则refresh方法会被调用两次的原因。

在`springmvc`的`FrameworkServlet`类中的`initWebApplicationContext`方法会调用`configureAndRefreshWebApplicationContext`方法，该方法会调用一次`refresh`方法，不过会提前判断容器是否激活。<br />所以这里的`refresh`表示重新构建的意思。<br />重点看看`refresh`的关键步骤：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493176-4926bf91-d5c8-4642-bf77-f028ee967d60.png#averageHue=%23eee3d8&height=744&id=Gvha5&originHeight=744&originWidth=1072&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1072)<br />其实上图中一眼看过去好像有很多方法，但是真正的核心的方法不多，其中最重要的：

- obtainFreshBeanFactory
- invokeBeanFactoryPostProcessors
- registerBeanPostProcessors
- finishBeanFactoryInitialization
<a name="inpyU"></a>
## 解析xml配置文件
`obtainFreshBeanFactory`方法会解析xml的bean配置，生成`BeanDefinition`对象，并且注册到Spring容器中（就是很多map集合中）。<br />经过几层调用，会调到`AbstractBeanDefinitionReader`类的`loadBeanDefinitions`方法：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493142-0a2353ac-87fa-4557-a2ab-61e5a28972de.png#averageHue=%232d2d2b&height=281&id=nM24d&originHeight=281&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />该方法会循环`locations`（applicationContext.xml文件路径）,调用另外一个`loadBeanDefinitions`方法，一个文件一个文件解析。<br />经过一些列的操作，会将`location`转换成`inputSource`和`resource`，然后再转换成`Document`对象，方便解析。![image.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1615857495339-b9eb8814-b470-4372-b541-a5db0ec30976.gif#averageHue=%23000000&height=1&id=OZd9I&originHeight=1&originWidth=1&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70&status=done&style=none&title=&width=1)![640.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857728074-13f6211f-928f-413d-9fdf-265730bc5672.png#averageHue=%232d2c2b&height=272&id=fDKU9&originHeight=272&originWidth=828&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29324&status=done&style=none&title=&width=828)<br />在解析xml文件时，需要判断是默认标签，还是自定义标签，处理逻辑不一样：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493175-914b61ee-14be-46c0-a3a8-d1111231fac3.png#averageHue=%23f9f8f7&height=886&id=VqUKd&originHeight=886&originWidth=970&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=970)<br />Spring的默认标签只有4种：

- `<import/>`
- `<alias/>`
- `<bean/>`
- `<beans/>`

对应的处理方法是：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493234-53d8a22b-62b0-4858-9620-c8eefd673206.png#averageHue=%23f4ece4&height=383&id=JDcx1&originHeight=383&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)<br />注意常见的：`<aop/>`、`<context/>`、`<mvc/>`等都是自定义标签。<br />从上图中处理`<bean/>`标签的`processBeanDefinition`方法开始，经过一系列调用，最终会调到`DefaultBeanDefinitionDocumentReader`类的`processBeanDefinition`方法。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493241-016f5611-a71d-4870-9fb6-f69dd3cb2074.png#averageHue=%23cfaf8b&height=386&id=Uvn3G&originHeight=386&originWidth=296&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=296)<br />这个方法包含了关键步骤：解析元素生成BeanDefinition 和 注册BeanDefinition。
<a name="Y2HpK"></a>
## 生成BeanDefinition
下面重点看看BeanDefinition是如何生成的。<br />上面的方法会调用`BeanDefinitionParserDelegate`类的`parseBeanDefinitionElement`方法：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493209-2c13dd3b-0fc4-4821-9617-6d459b65a928.png#averageHue=%23f8f6f5&height=978&id=GuDCd&originHeight=978&originWidth=804&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=804)<br />一个`<bean/>`标签会对应一个`BeanDefinition`对象。<br />该方法又会调用同名的重载方法：`processBeanDefinition`，真正创建`BeanDefinition`对象，并且解析一系列参数填充到对象中：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493228-7d79487a-94ff-40d2-b89a-7a409f06d209.png#averageHue=%23d4c28d&height=762&id=Sq1FD&originHeight=762&originWidth=332&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=332)<br />其实真正创建`BeanDefinition`的逻辑是非常简单的，直接`new`了一个对象：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493209-2ae72426-ff9c-4606-8730-5c4bd9cdbd9d.png#averageHue=%232d2d2c&height=420&id=dj0bQ&originHeight=420&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />真正复杂的地方是在前面的各种属性的解析和赋值上。
<a name="JGlMb"></a>
## 注册BeanDefinition
上面通过解析xml文件生成了很多`BeanDefinition`对象，下面就需要把`BeanDefinition`对象注册到Spring容器中，这样Spring容器才能初始化bean。<br />在`BeanDefinitionReaderUtils`类的`registerBeanDefinition`方法很简单，只有两个流程：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493236-477950e8-f9b3-4543-a178-7489d210eb5b.png#averageHue=%23caae7f&height=288&id=H31cd&originHeight=288&originWidth=352&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=352)<br />先看看`DefaultListableBeanFactory`类的`registerBeanDefinition`方法是如何注册`beanName`的：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493232-660cb66d-638b-48cb-a792-9992cd869533.png#averageHue=%23f8f5f3&height=736&id=g70bo&originHeight=736&originWidth=720&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=720)<br />接下来看看`SimpleAliasRegistry`类的`registerAlias`方法是如何注册`alias`别名的：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493316-9c9170b9-69c9-4576-9928-7fd33b92b5da.png#averageHue=%23f7f5f3&height=966&id=VNvyL&originHeight=966&originWidth=788&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=788)这样就能通过多个不同的`alias`找到同一个`name`，再通过`name`就能找到`BeanDefinition`。
<a name="9L7tO"></a>
## 修改BeanDefinition
上面`BeanDefinition`对象已经注册到Spring容器当中了，接下来，如果想要修改已经注册的`BeanDefinition`对象该怎么办呢？<br />`refresh`方法中通过`invokeBeanFactoryPostProcessors`方法修改`BeanDefinition`对象。<br />经过一系列的调用，最终会到`PostProcessorRegistrationDelegate`类的`invokeBeanFactoryPostProcessors`方法：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493226-b7eec95a-b81c-4e6b-95f8-1a866f7a1ec8.png#averageHue=%23f8f5f2&height=776&id=aEeXE&originHeight=776&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)<br />流程看起来很长，其实逻辑比较简单，主要是在处理`BeanDefinitionRegistryPostProcessor`和`BeanFactoryPostProcessor`。<br />而`BeanDefinitionRegistryPostProcessor`本身是一种特殊的`BeanFactoryPostProcessor`，它也会执行`BeanFactoryPostProcessor`的逻辑，只是加了一个额外的方法。![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493333-7707e411-aeea-48a0-8740-24a2403b6f3b.png#averageHue=%232e2d2c&height=383&id=kCulX&originHeight=383&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />`ConfigurationClassPostProcessor`可能是最重要的`BeanDefinitionRegistryPostProcessor`，它负责处理`@Configuration`注解。
<a name="GPd1p"></a>
## 注册BeanPostProcessor
处理完前面的逻辑，`refresh`方法接着会调用`registerBeanPostProcessors`注册`BeanPostProcessor`，它的功能非常强大，后面的文章会详细讲解。<br />经过一系列的调用，最终会到`PostProcessorRegistrationDelegate`类的`registerBeanPostProcessors`方法：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615857493367-16aa0d00-bf06-47c9-b4fa-a28aea9d02c5.png#averageHue=%23f3ece6&height=736&id=e3l9a&originHeight=736&originWidth=294&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=294)<br />注意，这一步只是注册`BeanPostProcessor`，真正的使用在后面。
<a name="ki8iO"></a>
## 总结
主要介绍了：

1. Spring容器初始化的入口
2. `refresh`方法的主要流程
3. 解析xml配置文件
4. 生成`BeanDefinition`
5. 注册`BeanDefinition`
6. 修改`BeanDefinition`
7. 注册`BeanPostProcessor`
