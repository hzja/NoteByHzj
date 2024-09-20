JavaSpringSpringWebflux
<a name="uWpP8"></a>
### **Spring-webflux简介**
spring-webflux是spring在5.0版本后提供的一套响应式编程风格的web开发框架。<br />这个框架包含了spring-framework和spring mvc，它可以运行在Netty、Undertow以及3.1版本以上的Serlvet容器上。<br />可以在项目中同时使用spring-webmvc和spring-webflux，或者只用其中一个来开发web应用。
<a name="N7wNP"></a>
### **什么是“响应式”**
所谓响应式，举个例子，当调用一个api获取数据时，无需阻塞等待数据返回，而是当有数据返回时会进行告知。可见响应式是非阻塞的，意味着调用方法后，CPU可以去做别的事情，当接收到数据响应时CPU再回来处理，这种方式提高了系统的吞吐量。<br />而响应式编程，其实是为这种异步非阻塞的流式编程制定的一套标准。流式编程已不陌生了，Java8提供的stream api就是这种风格。这套标准包括对运行环境（JVM、JavaScript）以及网络协议相关的规范。
<a name="m79xF"></a>
### **Spring-webflux的响应式API**
Spring-webflux框架是基于Reactor这个开源项目开发的。Reactor框架是跟Spring紧密配合的。<br />它提供了两种API类型，分别是Mono和Flux；
```java
// Mono一般作用于单个对象
Mono<Person> person = personDao.getPerson(personId);
// Flux一般作用于多个对象
Flux<Person> people = personDao.listAllPeople();
```
尽管webflux框架基于Reactor，它也能与其他的响应式框架同时使用，比如RxJava。
<a name="aURDK"></a>
### **选择Spring-webmvc还是Spring-webflux呢**
这两个web框架分别代表着两种不同类型的编程流派，官方给出了一个图作为对比如下<br />![2022-08-10-09-42-51.950785500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660095890231-4110da0e-90df-49bb-b2bf-ec0f51d42cfb.png#clientId=u23776e3d-ebe0-4&from=ui&id=u93a75195&originHeight=621&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2015990&status=done&style=shadow&taskId=u989b7b34-76c2-4db3-b4c4-46c4343cb21&title=)<br />根据官方的建议有以下几点可以作为参考：

- **如果已经使用了Spring-webmvc进行开发，并且项目运行良好，就无需更改了；何况现在大多数的三方库都是阻塞的，并不能发挥出非阻塞的优势。**
- **webflux提供了相当多的选择；在服务层，可以使用(Netty, Tomcat, Jetty, Undertow, 和3.1版本以上的Servlet容器)作为web服务；在应用层，可以选择用**`**@Controller**`**定义还是使用函数编程定义；在编程风格上，可以选择用Reactor、RxJava或其他。**
- **如果钟爱Java8提供的lambda表达式这种轻量级、函数式的编程风格，那么建议选择用webflux；同时对于一些轻量级应用，或者复杂度比较低的微服务，建议使用webflux以便更好的进行控制。**
- **在微服务架构中，可以将webmvc和webflux项目混合使用。两个框架都可以使用**`**@Controller**`**这种注解的方式，使得项目的重用更加容易。**
- **评估一个项目是否应该选择webflux的最简单的方式是，依据项目中是否会使用很多的阻塞API，比如JDBC或者一些阻塞式的API就不适用与webflux项目。**
- **如果一个webmvc项目中有很多的外部系统调用，可以试试响应式的WebClient，它能直接从Controller的方法中返回响应式结果。**
- **响应式编程的学习路线是比较陡峭的，所以如果身在一个大型的团队中，要考虑投入的成本；不过可以用用WebClient来体验下响应式编程。**

Spring-webflux不仅可以支持在Tomcat、Jetty以及3.1版本以上的Servlet容器上，还能够运行在非Servlet的服务器之上，比如Netty、Undertow等。<br />使用SpringBoot构建一个webflux应用，默认就是使用Netty，因为Netty本身就是非阻塞式的实现。
<a name="ul4TQ"></a>
### **并发模型**
尽管webmvc和webflux都支持使用注解来定义一个Controller，但是其实现方式完全不同。<br />webmvc是一个Servlet应用，实现是阻塞式IO，其维护一个线程池来处理每一个用户请求，也就是当Servlet容器启动时，就会创建比如10个线程出来，因此系统吞吐量的瓶颈在于有限的连接数和阻塞的请求处理过程。<br />webflux可以基于netty这样的NIO网络框架，它只需要很少的几个工作线程（Event loop worker）就能够处理并响应请求。由于无需阻塞等待方法返回，CPU资源就得到了更好的利用。<br />webflux并不能让程序运行地更快；而是提高了并发处理请求的能力，即提高了系统吞吐量。
<a name="A6XFK"></a>
### **webflux代码示例**
下面让来看一下webflux的示例，总的来说使用上是非常便捷的。<br />用Springboot构建一个webflux应用非常简单，仅仅需要加入这么一个依赖
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-webflux</artifactId>
</dependency>
```
首先定义一个对象
```java
public class Person {
    private Integer id;
    private Integer age;
    private String name;
}
```
然后定义“PersonController”，响应式风格中不再使用`@RequestMapping`声明地址映射了，而是通过`RouterFunctions.route().GET()`方法
```java
@Configuration
public class PersonRouter {
    @Resource
    private PersonHandler personHandler;
    @Bean
    public RouterFunction<ServerResponse> personRoutes() {
        return RouterFunctions.route()
                .GET("/person/{id}", RequestPredicates.accept(MediaType.APPLICATION_JSON), personHandler::getPerson)
                .GET("/person", RequestPredicates.accept(MediaType.APPLICATION_JSON), personHandler::listPeople)
                .POST("/person", personHandler::createPerson)
                .build();
    }
}
```
在`PersonHandler`中处理对应的HTTP请求，等同于MVC架构中的Service层
```java
@Component
public class PersonHandler {

    @Resource
    private PersonRepository personDao;

    public Mono<ServerResponse> listPeople(ServerRequest request) {
        Flux<Person> people = personDao.listAllPeople();
        return ServerResponse.ok()
                .contentType(MediaType.APPLICATION_JSON)
                .body(people, Person.class);
    }

    public Mono<ServerResponse> createPerson(ServerRequest request) {
        Mono<Person> person = request.bodyToMono(Person.class);
        return ServerResponse.ok()
                .build(personDao.savePerson(person));
    }

    public Mono<ServerResponse> getPerson(ServerRequest request) {
        int personId = Integer.parseInt(request.pathVariable("id"));
        return personDao.getPerson(personId)
                .flatMap(person -> ServerResponse.ok().contentType(MediaType.APPLICATION_JSON).bodyValue(person))
                .switchIfEmpty(ServerResponse.notFound().build());
    }

}
```
通过启动日志可以证实Spring-webflux是默认使用Netty提供HTTP服务<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660095717655-1c9c2c4a-5e9e-4946-859d-763593f53c6f.png#clientId=u23776e3d-ebe0-4&from=paste&id=u11e754ed&originHeight=60&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c59f867-ea9c-4a2b-9df2-c5726f15ff8&title=)<br />项目启动之后浏览器访问http://localhost:8080/person/1就能发现，Spring-webflux项目已经正常工作了。
