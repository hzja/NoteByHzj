Java Spring Bean<br />![2022-07-14-13-54-21.840743100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657778156018-28d0533c-8733-4649-ab83-f863a1de2bcf.png#clientId=ua6327e76-d7d4-4&from=ui&id=u38c81304&originHeight=565&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1834200&status=done&style=shadow&taskId=u3fa04421-80d2-439a-ab45-9887bade9da&title=)<br />这里首先声明一下，Spring将管理的一个个的依赖对象称之为Bean，这从xml配置文件中也可以看出。<br />Spring IOC容器就好像一个生产产品的流水线上的机器，Spring创建出来的Bean就好像是流水线的终点生产出来的一个个精美绝伦的产品。既然是机器，总要先启动，Spring也不例外。因此Bean的一生从总体上来说可以分为两个阶段：

1. 容器启动阶段
2. Bean实例化阶段

容器的启动阶段做了很多的预热工作，为后面Bean的实例化做好了充分的准备，首先看一下容器的启动阶段都做了哪些预热工作。
<a name="TN29w"></a>
## 1、容器启动阶段
<a name="pvDvU"></a>
### 1、配置元信息
Spring IOC容器将对象实例的创建与对象实例的使用分离，业务中需要依赖哪个对象不再依靠自己手动创建，只要向Spring要，Spring就会以注入的方式交给需要的依赖对象。既然将对象创建的任务交给了Spring，那么Spring就需要知道创建一个对象所需要的一些必要的信息。而这些必要的信息可以是Spring过去支持最完善的xml配置文件，或者是其他形式的例如properties的磁盘文件，也可以是现在主流的注解，甚至是直接的代码硬编码。总之，这些创建对象所需要的必要信息称为配置元信息。
```xml
<bean id="role" class="com.wbg.springxmlbean.entity.Role">
    <!-- property元素是定义类的属性，name属性定义的是属性名称 value是值
    相当于：
    Role role=new Role();
    role.setId(1);
    role.setRoleName("高级工程师");
    role.setNote("重要人员");-->
    <property name="id" value="1"/>
    <property name="roleName" value="高级工程师"/>
    <property name="note" value="重要人员"/>
</bean>
```
<a name="oJuFA"></a>
### 2、`BeanDefination`
在Java世界中，万物皆对象，散落于程序代码各处的注解以及保存在磁盘上的xml或者其他文件等等配置元信息，在内存中总要以一种对象的形式表示，就好比人对应到Java世界中就是一个Person类，而Spring选择在内存中表示这些配置元信息的方式就是`BeanDefination`，这里不会去分析`BeanDefination`的代码，感兴趣的可以去看相关源码，这里只是需要知道配置元信息被加载到内存之后是以`BeanDefination`的形存在的即可。
<a name="vETzX"></a>
### 3、`BeanDefinationReader`
开发者是看得懂Spring中xml配置文件中一个个的Bean定义，但是Spring是如何看懂这些配置元信息的呢？这个就要靠`BeanDefinationReader`了。<br />不同的`BeanDefinationReader`就像葫芦兄弟一样，各自拥有各自的本领。如果要读取xml配置元信息，那么可以使用`XmlBeanDefinationReader`。如果要读取`properties`配置文件，那么可以使用`PropertiesBeanDefinitionReader`加载。而如果要读取注解配置元信息，那么可以使用 `AnnotatedBeanDefinitionReader`加载。也可以很方便的自定义`BeanDefinationReader`来自己控制配置元信息的加载。例如配置元信息存在于三界之外，那么可以自定义From天界之外`BeanDefinationReader`。<br />总的来说，`BeanDefinationReader`的作用就是加载配置元信息，并将其转化为内存形式的`BeanDefination`，存在某一个地方，至于这个地方在哪里，不要着急，接着往下看！
<a name="kX80c"></a>
### 4、`BeanDefinationRegistry`
执行到这里，总算不遗余力的将存在于各处的配置元信息加载到内存，并转化为BeanDefination的形式，这样需要创建某一个对象实例的时候，找到相应的`BeanDefination`然后创建对象即可。那么需要某一个对象的时候，去哪里找到对应的`BeanDefination`呢？这种通过Bean定义的id找到对象的`BeanDefination`的对应关系或者说映射关系又是如何保存的呢？这就引出了`BeanDefinationRegistry`了。<br />Spring通过`BeanDefinationReader`将配置元信息加载到内存生成相应的`BeanDefination`之后，就将其注册到`BeanDefinationRegistry`中，`BeanDefinationRegistry`就是一个存放`BeanDefination`的大篮子，它也是一种键值对的形式，通过特定的Bean定义的id，映射到相应的`BeanDefination`。
<a name="mcS8F"></a>
### 5、`BeanFactoryPostProcessor`
`BeanFactoryPostProcessor`是容器启动阶段Spring提供的一个扩展点，主要负责对注册到`BeanDefinationRegistry`中的一个个的`BeanDefination`进行一定程度上的修改与替换。例如配置元信息中有些可能会修改的配置信息散落到各处，不够灵活，修改相应配置的时候比较麻烦，这时可以使用占位符的方式来配置。例如配置Jdbc的`DataSource`连接的时候可以这样配置:
```xml
<bean id="dataSource"  
    class="org.apache.commons.dbcp.BasicDataSource"  
    destroy-method="close">  
    <property name="maxIdle" value="${jdbc.maxIdle}"></property>  
    <property name="maxActive" value="${jdbc.maxActive}"></property>  
    <property name="maxWait" value="${jdbc.maxWait}"></property>  
    <property name="minIdle" value="${jdbc.minIdle}"></property>  
  
    <property name="driverClassName"  
        value="${jdbc.driverClassName}">  
    </property>  
    <property name="url" value="${jdbc.url}"></property>  
  
    <property name="username" value="${jdbc.username}"></property>  
    <property name="password" value="${jdbc.password}"></property>  
</bean> 
```
`BeanFactoryPostProcessor`就会对注册到`BeanDefinationRegistry`中的`BeanDefination`做最后的修改，替换$占位符为配置文件中的真实的数据。<br />至此，整个容器启动阶段就算完成了，容器的启动阶段的最终产物就是注册到`BeanDefinationRegistry`中的一个个`BeanDefination`了，这就是Spring为Bean实例化所做的预热的工作。再通过一张图的形式回顾一下容器启动阶段都是搞了什么事吧。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653874628986-cf557410-eb90-4092-ba1d-8692c7208cfe.jpeg)
<a name="dN7CY"></a>
## 2、Bean实例化阶段
需要指出，容器启动阶段与Bean实例化阶段存在多少时间差，Spring把这个决定权交给了程序员。如果选择懒加载的方式，那么直到伸手向Spring要依赖对象实例之前，其都是以`BeanDefinationRegistry`中的一个个的`BeanDefination`的形式存在，也就是Spring只有在需要依赖对象的时候才开启相应对象的实例化阶段。而如果不是选择懒加载的方式，容器启动阶段完成之后，将立即启动Bean实例化阶段，通过隐式的调用所有依赖对象的getBean方法来实例化所有配置的Bean并保存起来。<br />接下来就聊一聊Bean实例化过程的那些事儿~
<a name="mKs97"></a>
### 1、对象创建策略
到了这个时候，Spring就开始真刀真枪的干了，对象的创建采用了策略模式，借助前面`BeanDefinationRegistry`中的`BeanDefination`,可以使用反射的方式创建对象，也可以使用CGlib字节码生成创建对象。同时可以灵活的配置来告诉Spring采用什么样的策略创建指定的依赖对象。Spring中Bean的创建是策略设计模式的经典应用。这个时候，内存中应该已经有一个想要的具体的依赖对象的实例了，但是故事的发展还没有想象中的那么简单。<br />关于策略模式有不了解的可以查阅相关书籍，或者网上相关资料，这是设计模式相关的内容，本文主要关注Bean实例化的整体流程，设计模式相关知识不在讨论。
<a name="yWwxi"></a>
### 2、`BeanWrapper`——对象的外衣
Spring中的Bean并不是以一个个的本来模样存在的，由于Spring IOC容器中要管理多种类型的对象，因此为了统一对不同类型对象的访问，Spring给所有创建的Bean实例穿上了一层外套，这个外套就是`BeanWrapper`(关于`BeanWrapper`的具体内容感兴趣的请查阅相关源码)。`BeanWrapper`实际上是对反射相关API的简单封装，使得上层使用反射完成相关的业务逻辑大大的简化，要获取某个对象的属性，调用某个对象的方法，现在不需要在写繁杂的反射API了以及处理一堆麻烦的异常，直接通过`BeanWrapper`就可以完成相关操作，简直不要太爽了。
<a name="fcvos"></a>
### 3、设置对象属性
上一步包裹在`BeanWrapper`中的对象还是一个少不经事的孩子，需要为其设置属性以及依赖对象。

- 对于基本类型的属性，如果配置元信息中有配置，那么将直接使用配置元信息中的设置值赋值即可，即使基本类型的属性没有设置值，那么得益于JVM对象实例化过程，属性依然可以被赋予默认的初始化零值。
- 对于引用类型的属性，Spring会将所有已经创建好的对象放入一个Map结构中，此时Spring会检查所依赖的对象是否已经被纳入容器的管理范围之内，也就是Map中是否已经有对应对象的实例了。如果有，那么直接注入，如果没有，那么Spring会暂时放下该对象的实例化过程，转而先去实例化依赖对象，再回过头来完成该对象的实例化过程。

这里有一个Spring中的经典问题，那就是Spring是如何解决循环依赖的？<br />这里简单提一下，Spring是通过三级缓存解决循环依赖，并且只能解决Setter注入的循环依赖，请大家思考一下如何解决？为何只能是Setter注入？详细内容可以查阅相关博客，文档，书籍。
<a name="s53DX"></a>
### 4、检查`Aware`相关接口
如果想要依赖`Spring`中的相关对象，使用`Spring`的相关API,那么可以实现相应的`Aware`接口，Spring IOC容器就会为自动注入相关依赖对象实例。Spring IOC容器大体可以分为两种，`BeanFactory`提供IOC思想所设想所有的功能，同时也融入AOP等相关功能模块，可以说`BeanFactory`是Spring提供的一个基本的IOC容器。`ApplicationContext`构建于`BeanFactory`之上，同时提供了诸如容器内的时间发布、统一的资源加载策略、国际化的支持等功能，是Spring提供的更为高级的IOC容器。<br />讲了这么多，其实就是想表达对于`BeanFactory`来说，这一步的实现是先检查相关的`Aware`接口，然后去Spring的对象池(也就是容器，也就是那个Map结构)中去查找相关的实例(例如对于`ApplicationContextAware`接口，就去找`ApplicationContext`实例)，也就是说必须要在配置文件中或者使用注解的方式，将相关实例注册容器中，`BeanFactory`才可以自动注入。<br />而对于`ApplicationContext`，由于其本身继承了一系列的相关接口，所以当检测到Aware相关接口，需要相关依赖对象的时候，`ApplicationContext`完全可以将自身注入到其中，`ApplicationContext`实现这一步是通过下面要讲到的东东——`BeanPostProcessor`。<br />![2021-05-01-23-03-14-473070.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619882373650-0813c535-3a00-4385-a28d-95ecfe42d457.png#clientId=u83ac295c-6787-4&from=ui&id=u43a8487c&originHeight=214&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=694771&status=done&style=shadow&taskId=u50c41fb8-1899-4760-9ed2-4f9c1112620&title=)<br />例如`ApplicationContext`继承自`ResourceLoader`和`MessageSource`，那么当实现`ResourceLoaderAware`和`MessageSourceAware`相关接口时，就将其自身注入到业务对象中即可。
<a name="ypj5h"></a>
### 5、`BeanPostProcessor`前置处理
唉？刚才那个是什么`Processor`来？相信刚看这两个东西的人肯定有点晕乎了，不过其实也好区分，只要记住`BeanFactoryPostProcessor`存在于容器启动阶段而`BeanPostProcessor`存在于对象实例化阶段，`BeanFactoryPostProcessor`关注***对象被创建之前*** 那些配置的修修改改，缝缝补补，而`BeanPostProcessor`阶段关注***对象已经被创建之后*** 的功能增强，替换等操作，这样就很容易区分了。<br />`BeanPostProcessor`与`BeanFactoryPostProcessor`都是Spring在Bean生产过程中强有力的扩展点。如果还对它感到很陌生，那么肯定知道Spring中著名的AOP(面向切面编程)，其实就是依赖`BeanPostProcessor`对Bean对象功能增强的。<br />`BeanPostProcessor`前置处理就是在要生产的Bean实例放到容器之前，允许程序员对Bean实例进行一定程度的修改，替换等操作。<br />前面讲到的`ApplicationContext`对于`Aware`接口的检查与自动注入就是通过`BeanPostProcessor`实现的，在这一步Spring将检查Bean中是否实现了相关的Aware接口，如果是的话，那么就将其自身注入Bean中即可。Spring中AOP就是在这一步实现的偷梁换柱，产生对于原生对象的代理对象，然后将对源对象上的方法调用，转而使用代理对象的相同方法调用实现的。
<a name="qV8qU"></a>
### 6、自定义初始化逻辑
在所有的准备工作完成之后，如果Bean还有一定的初始化逻辑，那么Spring将允许通过两种方式配置初始化逻辑：(1)InitializingBean <br />(2)配置init-method参数<br />一般通过配置init-method方法比较灵活。
<a name="NNsiK"></a>
### 7、BeanPostProcess后置处理
与前置处理类似，这里是在Bean自定义逻辑也执行完成之后，Spring又留了最后一个扩展点。可以在这里在做一些想要的扩展。
<a name="oPYuj"></a>
### 8、自定义销毁逻辑
这一步对应自定义初始化逻辑，同样有两种方式：(1)实现`DisposableBean`接口 (2)配置destory-method参数。<br />这里一个比较典型的应用就是配置`dataSource`的时候destory-method为数据库连接的close()方法。
<a name="vIB6Q"></a>
### 9、使用
经过了以上道道工序，终于可以享受Spring带来的便捷了，这个时候像对待平常的对象一样对待Spring产生的Bean实例！
<a name="SDoFQ"></a>
### 10、调用回调销毁接口
Spring的Bean在服务完之后，马上就要消亡了(通常是在容器关闭的时候)，别忘了自定义销毁逻辑，这时候Spring将以回调的方式调用自定义的销毁逻辑，然后Bean就这样走完了光荣的一生！<br />再通过一张图来一起看一看Bean实例化阶段的执行顺序是如何的？<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653874739468-c4dd522c-33bf-4d2a-9ba9-53e4e1046a77.jpeg)<br />需要指出，容器启动阶段与Bean实例化阶段之间的桥梁就是可以选择自定义配置的延迟加载策略，如果配置了Bean的延迟加载策略，那么只有在真实的使用依赖对象的时候，Spring才会开始Bean的实例化阶段。而如果没有开启Bean的延迟加载，那么在容器启动阶段之后，就会紧接着进入Bean实例化阶段，通过隐式的调用`getBean`方法，来实例化相关Bean。
