Java Spring
<a name="PUyRn"></a>
## 什么是循环依赖？
![2021-08-27-23-35-34-592469.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078702432-661bba0a-92b4-4bc2-ab6f-cb694e2fc75c.png#clientId=u369db7a8-52da-4&from=ui&id=u233728fd&originHeight=287&originWidth=358&originalType=binary&ratio=1&size=2503&status=done&style=shadow&taskId=u0908896c-d198-4051-94ab-c9b4058904e)<br />从字面上来理解就是A依赖B的同时B也依赖了A，就像上面这样，或者C依赖与自己本身。体现到代码层次就是这个样子
```java
@Component
public class A {
    // A中注入了B
    @Autowired
    private B b;
}
---
@Component
public class B {
     // B中注入了A
     @Autowired
     private A a;
}
---
// 自己依赖自己
@Component
public class C {
     // C中注入了C
     @Autowired
     private C c;
}
```
虽然体现形式不一样，但是实际上都是循环依赖的问题。
<a name="AM5JN"></a>
### 什么情况下循环依赖可以被处理？
Spring解决循环依赖是有前置条件的

- 出现循环依赖的Bean必须要是单例(`singleton`)，如果依赖`prototype`则完全不会有此需求。
- 依赖注入的方式不能全是构造器注入的方式（只能解决`setter`方法的循环依赖，这是错误的）
<a name="ELxAF"></a>
##### 1. AB 均采用`setter`方法注入 结果OK
![2021-08-27-23-35-34-745473.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078717522-928786bf-ec5b-4b8d-83dc-776b3a055419.png#clientId=u369db7a8-52da-4&from=ui&id=u6149420f&originHeight=674&originWidth=1080&originalType=binary&ratio=1&size=146649&status=done&style=none&taskId=u1d6dc2fe-9605-43e6-86a7-00ac82ebf82)
<a name="KhLZf"></a>
##### 2. AB 均采用属性`Autowired`注入 结果ok
![2021-08-27-23-35-34-937477.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078717563-c01d84dd-8386-4c6a-95eb-9b2a9fbfde28.png#clientId=u369db7a8-52da-4&from=ui&id=ERjEz&originHeight=634&originWidth=1080&originalType=binary&ratio=1&size=154475&status=done&style=none&taskId=u04c6734e-fd06-44d0-9d76-3440c16b4f6)
<a name="lpxxI"></a>
##### 3. AB均采用构造器方法注入 出现循环依赖
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630078523123-d10f6d3a-dd0d-42e3-8bc6-fd8867995380.webp#clientId=u369db7a8-52da-4&from=paste&id=u24ec8aea&originHeight=650&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u8a175284-79d8-43fe-9dd0-71cd3e0e05b)
<a name="BX86A"></a>
##### 4. A中注入B的方式为setter方法，B中注入A的方式为构造器
![2021-08-27-23-35-35-273472.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078741096-ed64dd37-77f9-4832-b438-6f00b3f8b9fd.png#clientId=u369db7a8-52da-4&from=ui&id=u33fe0c6f&originHeight=663&originWidth=1080&originalType=binary&ratio=1&size=141736&status=done&style=none&taskId=uc5704f82-6c41-4646-8880-fd0c6b818c6)
<a name="iD9ny"></a>
##### 5. A中注入B的方式为构造器，B中注入A的方式为setter方法。
![2021-08-27-23-35-35-391470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078741127-8075e4ae-0c89-4c6e-bd24-9c63ef9562b0.png#clientId=u369db7a8-52da-4&from=ui&id=GwcTt&originHeight=668&originWidth=1080&originalType=binary&ratio=1&size=149617&status=done&style=none&taskId=u4c76b99c-2a0c-4879-9274-180de57f389)
<a name="XYUax"></a>
##### 结论
| 依赖情况 | 依赖注入方式 | 是否解决 |
| --- | --- | --- |
| AB相互依赖（循环依赖） | 均采用setter方法注入 | 是 |
| AB相互依赖（循环依赖） | 均采用属性自动注入 | 是 |
| AB相互依赖（循环依赖） | 均采用构造器注入 | 否 |
| AB相互依赖（循环依赖） | A中注入B的方式为setter方法，B中注入A的方式为构造器 | 是 |
| AB相互依赖（循环依赖） | B中注入A的方式为setter方法，A中注入B的方式为构造器,Spring在创建Bean时默认会根据自然排序进行创建，A会先于B进行创建 | 否 |

从上面的测试结果可以看到，不是只有在setter方法注入的情况下循环依赖才能被解决，即使存在构造器注入的场景下，循环依赖依然被可以被正常处理掉。
<a name="j45VU"></a>
## Spring循环依赖的通俗说
Spring bean 的创建，其本质上还是一个对象的创建，既然是对象，一定要明白一点就是，一个完整的对象包含两部分：当前对象实例化和对象属性的实例化。在Spring中，对象的实例化是通过反射实现的，而对象的属性则是在对象实例化之后通过一定的方式设置的。这个过程可以按照如下方式进行理解：<br />![2021-08-27-23-35-35-481473.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078761542-c6ab79fa-d51f-4261-a936-fe4bc400c926.png#clientId=u369db7a8-52da-4&from=ui&id=u009c48ed&originHeight=237&originWidth=292&originalType=binary&ratio=1&size=3116&status=done&style=shadow&taskId=u52bb31da-a7d6-456c-a389-5dc93b26ddf)<br />大致绘制依赖流程图如下：<br />![2021-08-27-23-35-35-647470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078761593-602f1edd-2f59-4d1f-b96a-a7a8d62ab3ff.png#clientId=u369db7a8-52da-4&from=ui&id=tCxF1&originHeight=373&originWidth=1080&originalType=binary&ratio=1&size=181316&status=done&style=shadow&taskId=ua7807df1-6c6e-4d0f-9050-b97fa8623cc)<br />图中`getBean()`表示调用Spring的`ApplicationContext.getBean()`方法，而该方法中的参数，则表示要尝试获取的目标对象。图中的黑色箭头表示一开始的方法调用走向，走到最后，返回了Spring中缓存的A对象之后，表示递归调用返回了，此时使用绿色箭头表示。从图中可以很清楚的看到，B对象的a属性是在第三步中注入的半成品A对象，而A对象的b属性是在第二步中注入的成品B对象，此时半成品的A对象也就变成了成品的A对象，因为其属性已经设置完成了。<br />到这里，Spring整个解决循环依赖问题的实现思路已经比较清楚了。对于整体过程只要理解两点：

- Spring是通过递归的方式获取目标bean及其所依赖的bean的；
- Spring实例化一个bean的时候，是分两步进行的，首先实例化目标bean，然后为其注入属性。

结合这两点，也就是说，Spring在实例化一个bean的时候，是首先递归的实例化其所依赖的所有bean，直到某个bean没有依赖其他bean，此时就会将该实例返回，然后反递归的将获取到的bean设置为各个上层bean的属性的。
<a name="ZKuhs"></a>
### Spring循环依赖进阶
一个对象一般创建过程有3部分组成：

1. 实例化：简单理解就是new了一个对象
2. 属性注入：为实例化中new出来的对象填充属性
3. 初始化：执行aware接口中的方法，初始化方法，完成AOP代理

Spring是通过**「三级缓存」**来解决上述问题的：

- `singletonObjects`：一级缓存 存储的是所有创建好了的单例Bean
- `earlySingletonObjects`：完成实例化，但是还未进行属性注入及初始化的对象
- `singletonFactories`：提前暴露的一个单例工厂，二级缓存中存储的就是从这个工厂中获取到的对象

然后接下来说下普通循环依赖跟带AOP的循环依赖。
<a name="hLjba"></a>
##### 普通循环依赖图
![2021-08-27-23-35-35-752472.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078761529-bb16ae0f-c1e9-47f5-b0ff-575b927484e1.png#clientId=u369db7a8-52da-4&from=ui&id=eqBME&originHeight=417&originWidth=930&originalType=binary&ratio=1&size=43964&status=done&style=shadow&taskId=u3d385aa0-c244-4cfa-b812-42904d93b4c)<br />结论：没有进行AOP的Bean间的循环依赖 从上图分析可以看出，这种情况下**「三级缓存根本没用」**！所以不会存在什么提高了效率的说法
<a name="JPQeH"></a>
##### 带AOP循环依赖
带AOP的跟不带AOP的其实几乎一样，只是在三级缓存中存放的是函数式接口，在需要调用时直接返回代理对象。三级缓存存在的意义：<br />只有真正发生循环依赖的时候，才去提前生成代理对象，否则只会创建一个工厂并将其放入到三级缓存中，但是不会去通过这个工厂去真正创建对象<br />![2021-08-27-23-35-35-848476.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078792257-53341730-d07a-4e38-a71d-4290ea51907a.png#clientId=u369db7a8-52da-4&from=ui&id=ub541a0ae&originHeight=450&originWidth=886&originalType=binary&ratio=1&size=47119&status=done&style=shadow&taskId=u6b233c8b-ff76-4f12-9a41-0e2112c4699)<br />是否可以用二级缓存而不用三级缓存？<br />答案：不可以，违背Spring在结合AOP跟Bean的生命周期的设计！Spring结合AOP跟Bean的生命周期(看下图)本身就是通过`AnnotationAwareAspectJAutoProxyCreator`这个后置处理器来完成的，在这个后置处理的`postProcessAfterInitialization`方法中对初始化后的Bean完成AOP代理。如果出现了循环依赖，那没有办法，只有给Bean先创建代理，但是没有出现循环依赖的情况下，设计之初就是让Bean在生命周期的**「最后一步完成代理而不是在实例化后就立马完成代理」**。<br />![2021-08-27-23-35-36-013478.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078792346-0e7b8a95-7d33-44a4-a223-6cc3f64221d1.png#clientId=u369db7a8-52da-4&from=ui&id=OPnuh&originHeight=625&originWidth=1080&originalType=binary&ratio=1&size=184758&status=done&style=shadow&taskId=u3668fbb8-4245-4eb0-973a-d8929913000)<br />使用了三级缓存的情况下，A、B的创建流程<br />![2021-08-27-23-35-36-189470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078792299-934d8f23-db79-4f1a-baa7-2d5be77d1b3e.png#clientId=u369db7a8-52da-4&from=ui&id=c5EWn&originHeight=232&originWidth=1080&originalType=binary&ratio=1&size=119690&status=done&style=shadow&taskId=u837a6746-4fbe-432c-b78a-4bec723dbe0)<br />不使用三级缓存，直接在二级缓存中<br />![2021-08-27-23-35-36-328472.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078815514-df2e7d92-528c-4882-9cdc-1ff5b7d14840.png#clientId=u369db7a8-52da-4&from=ui&id=u9ecaf4a3&originHeight=233&originWidth=1080&originalType=binary&ratio=1&size=119243&status=done&style=shadow&taskId=u5d4a0a85-4ccf-4526-8de1-a470caf6d36)<br />结论：上面两个流程的唯一区别在于为A对象创建代理的时机不同，使用三级缓存的情况下为A创建代理的时机是在B中需要注入A的时候，而不使用三级缓存的话在A实例化后就需要马上为A创建代理然后放入到二级缓存中去。三级缓存是无法提速的！
<a name="ZkVzG"></a>
## 回答模板
<a name="C4llQ"></a>
### Spring如何解决循环依赖的
答：Spring通过三级缓存解决了循环依赖，其中一级缓存为单例池（`singletonObjects`），二级缓存为早期曝光对象`earlySingletonObjects`，三级缓存为早期曝光对象工厂（`singletonFactories`）。<br />当A、B两个类发生循环引用时，在A完成实例化后，就使用实例化后的对象去创建一个对象工厂，添加到三级缓存中，如果A被AOP代理，那么通过这个工厂获取到的就是A代理后的对象，如果A没有被AOP代理，那么这个工厂获取到的就是A实例化的对象。<br />当A进行属性注入时，会去创建B，同时B又依赖了A，所以创建B的同时又会去调用`getBean(a)`来获取需要的依赖，此时的`getBean(a)`会从缓存中获取：<br />第一步：先获取到三级缓存中的工厂；<br />第二步：调用对象工工厂的`getObject`方法来获取到对应的对象，得到这个对象后将其注入到B中。紧接着B会走完它的生命周期流程，包括初始化、后置处理器等。<br />第三步：当B创建完后，会将B再注入到A中，此时A再完成它的整个生命周期。至此，循环依赖结束！
<a name="z7jyB"></a>
### 面试官：为什么要使用三级缓存呢？二级缓存能解决循环依赖吗？
答：如果要使用二级缓存解决循环依赖，意味着所有Bean在实例化后就要完成AOP代理，这样违背了Spring设计的原则，Spring在设计之初就是通过`AnnotationAwareAspectJAutoProxyCreator`这个后置处理器来在Bean生命周期的最后一步来完成AOP代理，而不是在实例化后就立马进行AOP代理。
<a name="CZO6W"></a>
## 跟踪核心大致流程
![2021-08-27-23-35-36-506484.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630078815584-3ce196bb-25c6-4878-a5a7-db1264fe5ee0.png#clientId=u369db7a8-52da-4&from=ui&id=Bqh4s&originHeight=1566&originWidth=1080&originalType=binary&ratio=1&size=268078&status=done&style=shadow&taskId=ub3d6c57e-7cea-4a97-a01f-635bf13f3f4)
