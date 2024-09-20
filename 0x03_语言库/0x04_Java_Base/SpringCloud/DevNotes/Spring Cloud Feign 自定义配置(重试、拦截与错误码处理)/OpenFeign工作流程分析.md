JavaOpenFeign<br />OpenFeign 组件的前身是 Netflix Feign 项目，它最早是作为 Netflix OSS 项目的一部分，由 Netflix 公司开发。后来 Feign 项目被贡献给了开源组织，于是才有了 Spring Cloud OpenFeign 组件。<br />OpenFeign 提供了一种声明式的远程调用接口，它可以大幅简化远程调用的编程体验。在了解 OpenFeign 的原理之前，先来体验一下 OpenFeign 的最终疗效。用一个Hello World 的小案例，看一下由 OpenFeign 发起的远程服务调用的代码风格是什么样的。
```java
String response = helloWorldService.hello("Vincent Y.");
```
这不就是本地方法调用吗？没错！使用 OpenFeign 组件来实现远程调用非常简单，就像使用本地方法一样，只要一行代码就能实现 **WebClient** 组件好几行代码干的事情。而且这段代码不包含任何业务无关的信息，完美实现了调用逻辑和业务逻辑之间的职责分离。<br />那么，OpenFeign 组件在底层是如何实现远程调用的呢？接下来了解OpenFeign 组件背后的工作流程。<br />OpenFeign 使用了一种“**动态代理**”技术来封装远程服务调用的过程，在上面的例子中看到的 **helloWorldService** 其实是一个特殊的接口，它是由 OpenFeign 组件中的`FeignClient` 注解所声明的接口，接口中的代码如下所示。
```java
@FeignClient(value = "hello-world-serv") 
public interface HelloWorldService { 
    @PostMapping("/sayHello") 
    String hello(String guestName); 
}
```
到这里一定恍然大悟了，原来**远程服务调用的信息被写在了 FeignClient 接口中**。<br />在上面的代码里，可以看到，服务的名称、接口类型、访问路径已经通过注解做了声明。<br />OpenFeign 通过解析这些注解标签生成一个“**动态代理类**”，这个代理类会将接口调用转化为一个远程服务调用的 Request，并发送给目标服务。<br />那么 OpenFeign 的动态代理是如何运作的呢？接下来深入了解这背后的流程。
<a name="egzLq"></a>
## OpenFeign 的动态代理
在项目初始化阶段，OpenFeign 会生成一个代理类，对所有通过该接口发起的远程调用进行动态代理。画了一个流程图，方便理解 OpenFeign 的动态代理流程：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684371702250-79d833d6-229b-4c97-a97f-06758e7af9e2.png#averageHue=%23fbfaf7&clientId=uf1fbe165-05bd-4&from=paste&id=u580b5aa1&originHeight=559&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc6b95364-6086-49b2-b2db-dda81d535bb&title=)<br />上图中的步骤 1 到步骤 3 是在项目启动阶段加载完成的，只有第 4 步“调用远程服务”是发生在项目的运行阶段。<br />下面来解释一下上图中的几个关键步骤。<br />首先，在项目启动阶段，`**OpenFeign**`** 框架会发起一个主动的扫包流程**，从指定的目录下扫描并加载所有被 `**@FeignClient**` 注解修饰的接口。<br />然后，**OpenFeign 会针对每一个 FeignClient 接口生成一个动态代理对象**，即图中的`**FeignProxyService**`，这个代理对象在继承关系上属于 FeignClient 注解所修饰的接口的实例。<br />接下来，**这个动态代理对象会被添加到 Spring 上下文中，并注入到对应的服务里**，也就是图中的 LocalService 服务。<br />最后，**LocalService 会发起底层方法调用**。实际上这个方法调用会被 OpenFeign 生成的代理对象接管，由代理对象发起一个远程服务调用，并将调用的结果返回给LocalService。<br />OpenFeign 是如何通过动态代理技术创建代理对象的？画了一张流程图梳理这个过程，可以参考一下。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684371702251-5fce82cc-dae7-40a7-aa38-ff8c191784bd.png#averageHue=%23fcfaf3&clientId=uf1fbe165-05bd-4&from=paste&id=u39991e41&originHeight=559&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u475085d4-94f1-4c56-998f-1528f13e60b&title=)<br />把 OpenFeign 组件加载过程的重要阶段画在了上图中。接下来梳理一下OpenFeign 动态代理类的创建过程。

1. **项目加载**：在项目的启动阶段，`**EnableFeignClients**`** 注解**扮演了“启动开关”的角色，它使用 Spring 框架的 `**Import**`** 注解**导入了 `FeignClientsRegistrar` 类，开始了OpenFeign 组件的加载过程。
2. **扫包**：`**FeignClientsRegistrar**` 负责 `FeignClient` 接口的加载，它会在指定的包路径下扫描所有的 FeignClients 类，并构造 `FeignClientFactoryBean` 对象来解析FeignClient 接口。
3. **解析 **`**FeignClient**`** 注解**：`**FeignClientFactoryBean**` 有两个重要的功能，一个是解析FeignClient 接口中的请求路径和降级函数的配置信息；另一个是触发动态代理的构造过程。其中，动态代理构造是由更下一层的 `ReflectiveFeign` 完成的。
4. **构建动态代理对象**：`**ReflectiveFeign**` 包含了 OpenFeign 动态代理的核心逻辑，它主要负责创建出 `FeignClient` 接口的动态代理对象。`ReflectiveFeign` 在这个过程中有两个重要任务，一个是解析 `FeignClient` 接口上各个方法级别的注解，将其中的远程接口URL、接口类型（GET、POST 等）、各个请求参数等封装成元数据，并为每一个方法生成一个对应的 `MethodHandler` 类作为方法级别的代理；另一个重要任务是将这些`MethodHandler` 方法代理做进一步封装，通过 Java 标准的动态代理协议，构建一个实现了 `InvocationHandler` 接口的动态代理对象，并将这个动态代理对象绑定到`FeignClient` 接口上。这样一来，所有发生在 FeignClient 接口上的调用，最终都会由它背后的动态代理对象来承接。

`**MethodHandler**` 的构建过程涉及到了复杂的元数据解析，OpenFeign 组件将FeignClient 接口上的各种注解封装成元数据，并利用这些元数据把一个方法调用“翻译”成一个远程调用的 Request 请求。<br />那么上面说到的“**元数据的解析**”是如何完成的呢？<br />它依赖于 OpenFeign 组件中的Contract 协议解析功能。Contract 是 OpenFeign 组件中定义的顶层抽象接口，它有一系列的具体实现，其中和实战项目有关的是 `SpringMvcContract` 这个类，从这个类的名字中就能看出来，它是专门用来解析 Spring MVC 标签的。<br />`SpringMvcContract` 的继承结构是 `**SpringMvcContract->BaseContract->Contract**`。这里拿一段 `SpringMvcContract` 的代码，深入理解它是如何将注解解析为元数据的。这段代码的主要功能是解析 `FeignClient` 方法级别上定义的 Spring MVC 注解。
```java
// 解析FeignClient接口方法级别上的RequestMapping注解
protected void processAnnotationOnMethod(MethodMetadata data, Annotation methodAnnotation, Method method) {
    // 省略部分代码...

    // 如果方法上没有使用RequestMapping注解，则不进行解析
    // 其实GetMapping、PostMapping等注解都属于RequestMapping注解
    if (!RequestMapping.class.isInstance(methodAnnotation)
        && !methodAnnotation.annotationType().isAnnotationPresent(RequestMapping.class)) {
        return;
    }

    // 获取RequestMapping注解实例
    RequestMapping methodMapping = findMergedAnnotation(method, RequestMapping.class);
    // 解析Http Method定义，即注解中的GET、POST、PUT、DELETE方法类型
    RequestMethod[] methods = methodMapping.method();
    // 如果没有定义methods属性则默认当前方法是个GET方法
    if (methods.length == 0) {
        methods = new RequestMethod[] { RequestMethod.GET };
    }
    checkOne(method, methods, "method");
    data.template().method(Request.HttpMethod.valueOf(methods[0].name()));

    // 解析Path属性，即方法上写明的请求路径
    checkAtMostOne(method, methodMapping.value(), "value");
    if (methodMapping.value().length > 0) {
        String pathValue = emptyToNull(methodMapping.value()[0]);
        if (pathValue != null) {
            pathValue = resolve(pathValue);
            // 如果path没有以斜杠开头，则补上/
            if (!pathValue.startsWith("/") && !data.template().path().endsWith("/")) {
                pathValue = "/" + pathValue;
            }
            data.template().uri(pathValue, true);
            if (data.template().decodeSlash() != decodeSlash) {
                data.template().decodeSlash(decodeSlash);
            }
        }
    }

    // 解析RequestMapping中定义的produces属性
    parseProduces(data, method, methodMapping);

    // 解析RequestMapping中定义的consumer属性
    parseConsumes(data, method, methodMapping);

    // 解析RequestMapping中定义的headers属性
    parseHeaders(data, method, methodMapping);
    data.indexToExpander(new LinkedHashMap<>());
}
```
通过上面的方法，可以看到，`OpenFeign` 对 `RequestMappings` 注解的各个属性都做了解析。<br />如果在项目中使用的是 `GetMapping`、`PostMapping` 之类的注解，没有使用 `RequestMapping`，那么 OpenFeign 还能解析吗？当然可以。以 `GetMapping` 为例，它对 `RequestMapping` 注解做了一层封装。如果查看下面关于 `GetMapping` 注解的代码，会发现这个注解头上也挂了一个 `RequestMapping` 注解。因此 OpenFeign 可以正确识别 `GetMapping` 并完成加载。
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@RequestMapping(method = RequestMethod.GET)
public @interface GetMapping {
    // ...省略部分代码
}
```
<a name="BFBuH"></a>
## 总结
清楚了 OpenFeign 要解决的问题，还了解了 OpenFeign 的工作流程，这里面的重点是动态代理机制。OpenFeing 通过 Java 动态代理生成了一个“代理类”，这个代理类将接口调用转化成为了一个远程服务调用。
