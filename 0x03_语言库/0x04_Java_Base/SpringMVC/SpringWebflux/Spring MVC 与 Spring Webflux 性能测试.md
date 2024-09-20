自 Spring 诞生以来，开发者创建 Java 企业应用程序就变得更加容易。它提供了在企业环境中使用 Java 语言所需的一切，支持 Groovy 和 Kotlin 作为 JVM 上的替代语言，并且可以根据应用程序的需求灵活地创建多种架构。<br />在 Spring 4.0 以前，Spring 框架中包含的原始 Web 框架是 Spring Web MVC，它是专门为 Servlet API 和 Servlet 容器构建的。响应式 Web 框架 Spring WebFlux 是在 5.0 版本中添加的。它是完全非阻塞的，支持 Reactive Streams 背压，运行在 Netty、Undertow、Servlet 容器等服务器上。<br />这两个 Web 框架名称相似（spring-webmvc 和 spring-webflux），并在 Spring 框架中并存。每个模块都是可选的。应用程序可以使用其中一个模块，或者在某些情况下，同时使用两者，例如在 Spring MVC 控制器中可以使用带有响应式编程功能的 WebClient 对象。<br />本文将给大家介绍使用响应式编程带来的潜在性能优势。使用一个简单的 hello world 案例。
<a name="baLkP"></a>
## 测试设置
<a name="XmPVo"></a>
### 配置
测试在一台 16G 内存的 MacBook Pro M1 上执行。<br />软件版本如下：

- Go 1.20.2
- Spring Boot 3.0.5
- Java 17

Spring MVC 与 Spring Webflux 的两种测试总共执行 500 万个请求。
<a name="Avsd2"></a>
### 代码
Spring MVC 与 Spring Webflux 的 hello world 代码如下：
<a name="iR2Gk"></a>
#### Spring Boot
传统的 Spring Boot 项目，单个 Java 文件
```java
package com.example.demo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.http.ResponseEntity;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
@RestController
public class DemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }

    @GetMapping("/")
    public String handleRequest() {
        return "Hello World!";
    }
}
```
<a name="Zz2lk"></a>
#### Spring Webflux
与传统的 Spring Boot 项目不同，Spring Webflux 至少需要四个 Java 文件。代码如下<br />HelloWorldRouter.java<br />HelloWorldClient.java<br />Application.java
<a name="HB7Xi"></a>
### 执行
每个测试都接受 500 万个请求执行。<br />测试中包含 25、100 和 300 个并发测试。<br />使用 Bombardier HTTP 测试工具进行负载测试。<br />Bombardier HTTP 是一个用 Go 编写的快速跨平台 HTTP 基准测试命令行工具。<br />下面是测试结果图表：<br />![请求耗时，越小越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026424-538d307a-5026-40cb-b10d-49aa0358d9e3.png#averageHue=%23befc5c&clientId=u0bf1a807-1b60-4&from=paste&id=ue0754cc1&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub53e93e2-2cd5-483d-b529-691092b7ae4&title=%E8%AF%B7%E6%B1%82%E8%80%97%E6%97%B6%EF%BC%8C%E8%B6%8A%E5%B0%8F%E8%B6%8A%E5%A5%BD "请求耗时，越小越好")<br />![每秒请求数，越大越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026451-aa248b4b-4b64-4056-8b9f-4f988b68a115.png#averageHue=%23e9fbcb&clientId=u0bf1a807-1b60-4&from=paste&id=ua76ee030&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u7fa37247-8775-41ed-9d94-e4331bbd343&title=%E6%AF%8F%E7%A7%92%E8%AF%B7%E6%B1%82%E6%95%B0%EF%BC%8C%E8%B6%8A%E5%A4%A7%E8%B6%8A%E5%A5%BD "每秒请求数，越大越好")<br />![响应时间/ms，越小越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026523-4843d389-459d-4fc4-be41-36e9235c8f68.png#averageHue=%23fcfcfc&clientId=u0bf1a807-1b60-4&from=paste&id=u62757b34&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uccfacc60-37ea-44c0-830e-69fad073c22&title=%E5%93%8D%E5%BA%94%E6%97%B6%E9%97%B4%2Fms%EF%BC%8C%E8%B6%8A%E5%B0%8F%E8%B6%8A%E5%A5%BD "响应时间/ms，越小越好")<br />![中值响应时间/ms，越小越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026398-47758d23-f3bb-4ac6-bfa7-20f74fdfec52.png#averageHue=%23fcfcfc&clientId=u0bf1a807-1b60-4&from=paste&id=u1c29f229&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6dfd07a6-e311-4541-95a4-21145d3799e&title=%E4%B8%AD%E5%80%BC%E5%93%8D%E5%BA%94%E6%97%B6%E9%97%B4%2Fms%EF%BC%8C%E8%B6%8A%E5%B0%8F%E8%B6%8A%E5%A5%BD "中值响应时间/ms，越小越好")<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026435-97631a65-6c1c-4d85-b25a-9b4d0c90df30.png#averageHue=%23fcfcfc&clientId=u0bf1a807-1b60-4&from=paste&id=ud4fd8dba&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0d6ea8d6-ffe9-4bd6-964b-2a17f2c29cf&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026872-3e41cc14-b335-4973-a5c9-ad12f13759a9.png#averageHue=%23fcfcfc&clientId=u0bf1a807-1b60-4&from=paste&id=ub8bb0396&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf04b0eb1-9d48-4267-8021-e8c57df1ba6&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026960-1d26cc76-3683-4bfc-a06c-967f8df3f8f9.png#averageHue=%23fcfcfc&clientId=u0bf1a807-1b60-4&from=paste&id=uc8284b38&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubee3342e-9014-41fa-8c03-8ebdcbce2cd&title=)<br />![最大响应时间/ms，越小越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265026968-1ef9e9ad-1472-47ab-9844-a496d29c1e62.png#averageHue=%23fcfcfc&clientId=u0bf1a807-1b60-4&from=paste&id=u95c6c62f&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5259023c-bda0-4895-ab35-52d676bd964&title=%E6%9C%80%E5%A4%A7%E5%93%8D%E5%BA%94%E6%97%B6%E9%97%B4%2Fms%EF%BC%8C%E8%B6%8A%E5%B0%8F%E8%B6%8A%E5%A5%BD "最大响应时间/ms，越小越好")<br />![平均CPU占用/%，越小越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265027071-dd10bd7b-48fe-4ff3-828d-316fc83035e6.png#averageHue=%23b6fc46&clientId=u0bf1a807-1b60-4&from=paste&id=uf19ac165&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua475922d-61b2-46b3-9383-cb14e9032fb&title=%E5%B9%B3%E5%9D%87CPU%E5%8D%A0%E7%94%A8%2F%25%EF%BC%8C%E8%B6%8A%E5%B0%8F%E8%B6%8A%E5%A5%BD "平均CPU占用/%，越小越好")<br />![平均内存占用/MBs，越小越好](https://cdn.nlark.com/yuque/0/2023/png/396745/1695265027282-16d8b488-d2cb-4f28-b34f-3b61839309fc.png#averageHue=%23b7fc49&clientId=u0bf1a807-1b60-4&from=paste&id=u336971d0&originHeight=600&originWidth=720&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u134a1beb-0557-499c-90cc-3a618ec852e&title=%E5%B9%B3%E5%9D%87%E5%86%85%E5%AD%98%E5%8D%A0%E7%94%A8%2FMBs%EF%BC%8C%E8%B6%8A%E5%B0%8F%E8%B6%8A%E5%A5%BD "平均内存占用/MBs，越小越好")
<a name="Kg6lb"></a>
## 分析
通过以上结果，很容易得出结论，Spring Webflux（响应式编程）确实比 Spring Boot（线程池）带来了一些显着的性能优势。Spring Webflux 在资源成本相当的情况下提供大约两倍的 RPS。<br />RPS：指客户端每秒发出的请求数，有些地方也叫做 QPS。<br />首先由于 Spring MVC 处理这些一次性请求花费的总时间太长，Spring MVC 的平均响应时间并不是那么好。<br />在低并发情况下，Spring Webflux 的中值响应时间更好。高并发时 Spring Boot 更好。<br />随着测量值移至第三个四分位和第 90 个百分位，Spring Webflux 变得更好。即使有差异，也只有 1-2 毫秒左右。
<a name="RrbDS"></a>
## 最后
Spring MVC 与 Spring Webflux：hello world 性能测试案例的获胜者是 Spring Webflux。<br />来源：[https://medium.com/deno-the-complete-reference/spring-boot-vs-spring-webflux-performance-comparison-for-hello-world-case-386da4e9c418](https://medium.com/deno-the-complete-reference/spring-boot-vs-spring-webflux-performance-comparison-for-hello-world-case-386da4e9c418)
