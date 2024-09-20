JavaSpringCloudSpringCloud NacosRibbon<br />在 Nacos 中，服务调用主要是通过 RestTemplate + Ribbon 实现的，RestTemplate 是 Spring 提供的 Restful 请求实现类，而 Ribbon 是客户端负载均衡器，通过 Ribbon 可以获取服务实例的具体信息（IP 和端口号），之后再通过 RestTemplate 加服务实例的具体信息就可以完成一次服务调用了。<br />而 RestTemplate + Ribbon 调用服务的实现方式两种：通过代码的方式调用服务和通过注解方式调用服务。但两种实现方式的**原理都是一样的：都是通过注册中心，将可用服务列表拉取到本地（客户端），再通过客户端负载均衡器得到某个服务器的具体信息，然后请求此服务器即可**，如下图所示：<br />![2022-11-16-16-56-36.471573800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668589023357-0876953e-0874-4f65-ae77-f77f655612e0.png#averageHue=%23f9f8f6&clientId=u1ab68096-780f-4&from=ui&id=u577d0cc6&originHeight=716&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2324386&status=done&style=none&taskId=uabb6d1b2-d722-4a36-a2f3-4677f7aed01&title=)
<a name="BkEeI"></a>
## 1、代码方式调用
通过代码的方式调用服务在实际工作中并不常用，主要是写法太麻烦，但了解它对于后面理解注解调用方式有很大的帮助，所以这里重点来看一下。服务调用需要有两个角色：一个是服务提供者（Provider），另一个是服务调用者（Consumer），接下来创建一下这两个角色。
<a name="hauCg"></a>
### 1.1 创建服务提供者：Provider
**第一步**：先创建一个 Spring Boot 项目（Spring Cloud 项目是基于 Spring Boot 创建的），添加 spring-web 和 nacos-discovery 依赖，具体依赖信息如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<!-- 添加 Nacos 支持 -->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
**第二步**：设置 Nacos 相关配置，在 application.yml 中添加以下配置：<br />**第三步**：添加服务方法，如下代码所示：<br />然后使用相同的方法再创建 2 个服务提供者，最终对应的端口号分别为：<br />127.0.0.1:8081 127.0.0.1:8082 127.0.0.1:8083<br />这 3 个服务提供者分别打印的内容是“I'm Provider...”、“I'm Provider2...”、“I'm Provider3...”，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668588858873-2e180612-ad12-40ea-8cd3-63ff95f48f39.png#averageHue=%23e4e4db&clientId=u1ab68096-780f-4&from=paste&id=u0adfc61d&originHeight=576&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udceee5ad-9296-4523-99b1-cb2c3532f5a&title=)
<a name="qt3EU"></a>
### 1.2 创建服务调用者：Consumer
**本文的核心是服务调用者的实现代码**，它的创建方式和服务提供者的创建方式类似。**第一步**：创建一个 Spring Boot 项目，添加 spring-web 和 nacos-discovery 依赖，具体依赖内容如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<!-- 添加 Nacos 支持 -->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
可能有人会有疑问，本文标题是 Spring Cloud Alibaba Nacos + Ribbon，那**为什么不添加 Ribbon 的依赖呢？**这是**因为 Spring Cloud Alibaba Nacos 中已经内置了 Ribbon 框架了**，打开项目的依赖树就可以清楚的看到了，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668588858779-bfdec7fb-f5cc-40fb-a446-f1100dd11928.png#averageHue=%233e4144&clientId=u1ab68096-780f-4&from=paste&id=u1893469c&originHeight=534&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u73543467-8fcf-4d25-ae92-666ed7af06f&title=)**第二步**：设置 Nacos 相关配置，在 application.yml 中添加以下配置：<br />**第三步**：在项目启动类中，使用 Spring Java Config 的方式声明 RestTemplate 对象，如下代码所示：<br />**第四步**：使用 RestTemplate + Ribbon 的代码方式调用服务，首先使用 Ribbon 提供的 LoadBalancerClient 对象的 choose 方法，根据 Nacos 中的服务 id 获取某个健康的服务实例，服务实例中包含服务的 IP 地址和端口号，然后再使用 RestTemplate 根据获取到的 IP 和 端口号访问服务即可，具体实现代码如下：<br />以上程序的执行结果如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668588858882-2552040f-e128-45e8-af9b-c4ff0b15ce17.png#averageHue=%23e3e3db&clientId=u1ab68096-780f-4&from=paste&id=ub0f0caec&originHeight=184&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u11c44b9b-cdf8-4d2c-aac5-0f76fb8b6dd&title=)
<a name="tTvXP"></a>
## 2、注解方式调用
使用注解方式调用服务就简单多了，服务提供者的创建方法和上面相同，这里就不再赘述了，接下来创建一个注解方式的服务调用者 Consumer。**第一步**：创建一个 Spring Boot 项目，添加 spring-web 和 nacos-discovery 依赖，具体依赖内容如下：<br />**第二步**：设置 Nacos 相关配置，在 application.yml 中添加以下配置：<br />**第三步**：在项目启动类中，使用 Spring Java Config 的方式声明 RestTemplate 对象，此步骤中，**需要在 RestTemplate 对象上加上 **`**@LoadBalanced**`** 注解，加上此注解之后就可以让 RestTemplate 对象自动支持负载均衡了**，如下代码所示：<br />**第四步**：创建客户端请求方法，具体实现代码如下：<br />以上程序的执行结果如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668588859629-43523e2f-a2ca-405d-b1b8-ec0c7fe85a69.png#averageHue=%23dfe0d8&clientId=u1ab68096-780f-4&from=paste&id=u6b0e7fd4&originHeight=179&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5450b7bc-30d4-47fa-b391-601d4e62126&title=)
<a name="u4fdO"></a>
### 注解实现原理分析
通过上述代码可以看出，Nacos 实现调用服务的关键是通过 `@LoadBalanced`，它为 RestTemplate 赋予了负载均衡的能力，从而可以正确的调用到服务，那 `@LoadBalanced` 是如何实现的呢？要知道这个问题的答案，就得阅读 `LoadBalancerAutoConfiguration` 的源码。`LoadBalancerAutoConfiguration` 是实现客户端负载均衡器的自动装配类，随着 Spring 的启动而启动，它的源码内容有很多，这里截取部分核心的方法来看一下：<br />这里的 `this.restTemplates.iterator()` 既所有被 `@LoadBalanced` 注解修饰的 RestTemplate 对象，所有被 `@LoadBalanced` 修饰的 RestTemplate 对象会被强转为 `RestTemplateCustomizer` 对象，而这个对象的实现源码如下：<br />也就是所有被 `@LoadBalanced` 注解修饰的 RestTemplate 对象，会为其添加一个 `loadBalancerInterceptor` 的拦截器，拦截器的实现源码如下：<br />从上述源码可以看出，`**@LoadBalanced**`** 的执行流程是，被 **`**@LoadBalanced**`** 注解修饰的 **`**RestTemplate**`** 对象，会被 LoadBalancerInterceptor 拦截器所拦截，拦截之后使用 **`**LoadBalancerClient**`** 对象，按照负载均衡的策略获取一个健康的服务实例，然后再通过服务实例的 IP 和端口，调用实例方法，从而完成服务请求。**
<a name="J30qR"></a>
## 总结
Nacos 调用 Restful 服务是通过内置的 Ribbon 框架实现的，它有两种调用方法，通过代码的方式或通过注解的方式完成调用。其中注解的方式使用起来比较简单，只需要在 RestTemplate 对象上添加一个 `@LoadBalanced` 注解，就可以为请求对象赋予负载均衡的能力了。
