<a name="TfpoM"></a>
## 认识Solon
Solon是一个参考了Spring Boot和Javalin的设计而创建的框架。它吸取了这两者的优点并避免了繁重的设计，使得它能够支持HTTP、WebSocket和Socket等三种通信信号接入。<br />Solon只需要2MB的大小就可以支撑完整的MVC开发，并且在启动方面只需要0.1秒的时间。相比之下，一个普通的Spring Boot MVC项目通常需要起步20MB以上的内存占用，并且启动时间要超过3秒，同时其QPS性能也较弱。<br />官方介绍：**相比SpringBoot，启动快 5 ～ 10 倍；qps 高 2～ 3 倍；运行时内存节省 1/3 ~ 1/2；打包可以缩到 1/2 ~ 1/10；同时支持 jdk8, jdk11, jdk17, jdk20, graalvm native。**

- **克制、简洁、高效、开放、生态**
- 支持 JDK8、JDK11、JDK17、JDK20
- Http、WebSocket、Socket 三种信号统一的开发体验（俗称：三源合一）
- 支持“注解”与“手动”两种模式，按需自由操控
- Not Servlet，可以适配任何基础通讯框架（最小 0.3m 运行rpc架构）
- 独特的 IOC/AOP 容器设计。不会因为插件变多而启动变很慢
- 支持 Web、Data、Job、Remoting、Cloud 等任何开发场景
- 兼顾 Handler + Context 和 Listener + Message 两种架构模式
- 强调插件式扩展，可扩展可切换；适应不同的应用场景
- 支持 GraalVm Native 打包
- 允许业务插件“热插”、“热拔”、“热管理”

![solon生态架构图](https://cdn.nlark.com/yuque/0/2023/png/396745/1688800090444-7f7a66cf-3600-470c-94ae-6c2490e7289b.png#averageHue=%23efefef&clientId=uf9f48f45-754a-4&from=paste&id=u130c5646&originHeight=731&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9fa1fe26-b527-4604-b1c8-0db30eaf722&title=solon%E7%94%9F%E6%80%81%E6%9E%B6%E6%9E%84%E5%9B%BE "solon生态架构图")<br />官网：[https://solon.noear.org/](https://solon.noear.org/)<br />项目地址：[https://gitee.com/noear/solon](https://gitee.com/noear/solon)
<a name="QGmkZ"></a>
## 上手Solon
web开发的模式和SpringBoot基本一致，但是注解和SpringBoot略有区别，具体区别可以看它的官方文档。<br />一个重要的区别是，Solon 不是基于 Servlet 的开发框架，虽然使用起来与 SpringBoot 相似的体验，但内部使用 Context 包装请求上下文（底层为：Context + Handler 架构）。
<a name="e0xM0"></a>
### 创建Maven项目，引入依赖
```xml
<parent>
  <groupId>org.noear</groupId>
  <artifactId>solon-parent</artifactId>
  <version>2.3.6</version>   
</parent>

<dependencies>
  <dependency>
    <groupId>org.noear</groupId>
    <artifactId>solon-web</artifactId>
  </dependency>
</dependencies>
```
<a name="keznl"></a>
### 编写启动类代码
```java
@SolonMain
public class App{
    public static void main(String[] args){
        long start = System.currentTimeMillis();
        Solon.start(App.class, args);
        long end = System.currentTimeMillis();
        System.out.println("启动耗时：" + (end - start) + "ms");
    }
}
```
<a name="EX75P"></a>
### 编写Controller
```java
@Controller
public class DemoController {

    @Mapping("/hello")
    public String hello(String name){
        return "Hello " + name;
    }

}
```
<a name="lh6Jv"></a>
### 运行，测试
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688800090464-42df965d-afa8-419f-a193-779bb9189c47.png#averageHue=%23302f2e&clientId=uf9f48f45-754a-4&from=paste&id=u9d1e75e0&originHeight=192&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udb6a1aae-df2b-4969-a111-dd3b5d40630&title=)<br />可以看出来，启动还是非常非常快的
<a name="TB7FW"></a>
### 打包
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688800090401-efd724df-a99f-4025-af2d-6df779b1b10c.png#averageHue=%23fcf9f7&clientId=uf9f48f45-754a-4&from=paste&id=u375d1831&originHeight=261&originWidth=734&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5836e05-80aa-4b61-ae49-df0f3122c97&title=)<br />**只有4kb！**<br />有点儿惊人，为此，专门去新建了个SpringBoot项目，也是简单的一个接口，打包<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688800090565-5745e5c8-9eb1-42f1-937e-ffc5696769f2.png#averageHue=%23fdfbfa&clientId=uf9f48f45-754a-4&from=paste&id=u20849db6&originHeight=265&originWidth=905&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uccd1cee0-cad4-4c03-8db5-b6f966dfd4c&title=)<br />这个项目也是4k，看来太简单的项目看不出来区别。<br />顺便测试了一下启动时间<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688800090758-8f16e884-3daf-440e-b8ef-9de91545ac8f.png#averageHue=%232d2d2c&clientId=uf9f48f45-754a-4&from=paste&id=u6484faa1&originHeight=185&originWidth=903&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc97c914-0c6d-4640-98f8-9eac2cdac2a&title=)<br />启动时间确实还是 Solon 更快！
<a name="qalRp"></a>
## 小结
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688800090982-349b8e23-c4b1-4fb1-b778-4d6bee2894bf.png#averageHue=%23f4f3f2&clientId=uf9f48f45-754a-4&from=paste&id=u3515a579&originHeight=1068&originWidth=329&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua66f08ef-0b14-47eb-ba9f-712615131d3&title=)<br />再来看看Solon的官方文档，文档写的很详细，功能很齐全，Solon的生态虽然没有Spring那么强大，但小型项目已经完全可以胜任。
