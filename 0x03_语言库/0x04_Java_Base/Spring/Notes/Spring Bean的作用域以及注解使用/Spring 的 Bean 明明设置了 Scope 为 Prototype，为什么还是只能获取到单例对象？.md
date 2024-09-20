JavaSpring<br />Spring 作为当下最火热的Java 框架，相信很多小伙伴都在使用，对于 Spring 中的 Bean 都知道默认是单例的，意思是说在整个 Spring 容器里面只存在一个实例，在需要的地方直接通过依赖注入或者从容器中直接获取，就可以直接使用。
<a name="u1lfs"></a>
## 测试原型
对于有些场景，可能需要对应的 Bean 是原型的，所谓原型就是希望每次在使用的时候获取到的是一个新的对象实例，而不是单例的，这种情况下很多小伙伴肯定会说，那还不简单，只要在对应的类上面加上 `@scope` 注解，将 value 设置成 Prototype 不就行了。如下所示<br />HelloService.java
```java
package com.example.demo.service;

import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

@Service
@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
public class HelloService {

  public String sayHello() {
    return "hello: " + this.hashCode();
  }
}
```
HelloController.java 代码如下：
```java
package com.example.demo.controller;

import com.example.demo.service.HelloService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Lookup;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

  @Autowired
  private HelloService service;

  @GetMapping(value = "/hello")
  public String hello() {
    return service.sayHello();
  }
}
```
简单描述一下上面的代码，其中 HelloService 类使用了注解 `Scope`，并将值设置为 `SCOPE_PROTOTYPE`，表示是原型类，在 HelloController 类中调用 HelloService 的 sayHello 方式，其中返回了当前实例的 hashcode。<br />通过访问 http://127.0.0.1:8080/hello 来获取返回值，如果说每次获取到的值都不一样，那就说明上面的代码是没有问题的，每次在获取的时候都会使用一个新的 HelloService 实例。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658277170891-4a94204d-dc26-473f-af5c-5acdde99a71e.jpeg#clientId=ue74eaa66-708c-4&from=paste&id=u511aab88&originHeight=260&originWidth=894&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud015c46c-7deb-452f-9eda-3dc96e9ad30&title=)<br />然而在电脑上，无论刷新浏览器多少次，最后的结果却没有发生任何变化，换句话说这里引用到的 HelloService 始终就是一个，并没有原型的效果。<br />那么问题来了，明明给 HelloService 类增加了原型注解，为什么这里没有效果呢？
<a name="RYH0t"></a>
## 原因分析
这样思考一下，首先通过浏览器访问接口的时候，访问到的是 HelloController 类中的方法，那么 HelloController 由于没有增加 `Scope` 的原型注解，所以肯定是单例的，那么单例的 HelloController 中的 HelloService 属性是什么怎么赋值的呢？<br />那自然是 Spring 在 HelloController 初始化的时候，通过依赖注入赋值的。Spring 注入依赖的赋值逻辑简单来说就是创建 Bean 的时候如果发现有依赖注入，则会在容器中获取或者创建一个依赖 Bean，此时对应属性的 Bean 是单例的，则容器中只会创建一个，如果对应的 Bean 是原型，那么每次都会创建一个新的 Bean，然后将创建的 Bean 赋值给对应的属性。<br />在这里 HelloService 类是原型的，所以在创建 HelloController Bean 的时候，会创建一个 HelloService 的 Bean 赋值到 service 属性上；到这里都没有问题，但是因为 HelloController Bean 是单例的，初始化的动作在整个生命周期中只会发生一次，所以即使 HelloService 类是原型的，也只会被依赖注入一次，因此上面的这种写入是达不到需要的效果的。
<a name="U9QKc"></a>
## 解法
<a name="ScKUW"></a>
### **解法一**
那如果把 HelloController 类也设置成原型呢？这样不就可以了么。给 HelloController 增加上注解 `@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)` 重启过后重新访问 http://127.0.0.1:8080/hello ，发现确实是可以的。也很好理解，因为此时 HelloController 是原型的，所以每次访问都会创建一个新的实例，初始化的过程中会被依赖注入新的 HelloService 实例。<br />但是不得不说，这种解法很不优雅，把 Controller 类设置成原型，并不友好，所以这里不推荐这种解法。
<a name="kLXMU"></a>
### **解法二**
除了将 HelloController 设置成原型，还有其他的解法，上面提到 HelloController 在初始化的时候会依赖注入 HelloService，那是不是可以换一个方式，让 HelloController 创建的时候不依赖注入 HelloService，而是在真正需要的时候再从容器中获取。如下所示：
```java
package com.example.demo.controller;

import com.example.demo.service.HelloService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

	@Autowired
	private ApplicationContext applicationContext;

	@GetMapping(value = "/hello")
	public String hello() {
		HelloService service = getService();
		return service.sayHello();
	}

	public HelloService getService() {
		return applicationContext.getBean(HelloService.class);
	}
}
```
通过测试这种方式也是可以的，每次从容器中重新获取的时候都是重新创建一个新的实例。
<a name="MaZDv"></a>
### **解法三**
上面解法二还是比较常规的，除了解法二之外还有一个解法，那就是使用 `Lookup` 注解，根据 Spring 的官方文档，可以看到下面的内容。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658277170754-a98289aa-f458-44bc-998a-4e33ada30f2d.jpeg#clientId=ue74eaa66-708c-4&from=paste&id=ub99411c2&originHeight=447&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue910d192-c5ff-4468-b3a8-421f9c1739e&title=)<br />简单来说就是通过使用 `Lookup` 注解的方法，可以被容器覆盖，然后通过  `BeanFactory` 返回指定类型的一个类实例，可以在单例类中使用获取到一个原型类，示例如下
```java
package com.example.demo.controller;

import com.example.demo.service.HelloService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Lookup;
import org.springframework.context.ApplicationContext;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

	@GetMapping(value = "/hello")
	public String hello() {
		HelloService service = getService();
		return service.sayHello();
	}

	@Lookup
	public HelloService getService() {
		return null;
	}
}
```
写法跟解法二比较相似，只不过不是显示的通过容器中获取一个原型 Bean 实例，而是通过 `Lookup` 的注解，让容器来覆盖对应的方法，返回一个原型实例对象。这里 getService 方法里面可以直接返回一个 null，因为这里面的代码是不会被执行到的。<br />打个断点调试，会发现通过 `Lookup` 注解的方法最终后走到`org.springframework.beans.factory.support.CglibSubclassingInstantiationStrategy.LookupOverrideMethodInterceptor#intercept` 这里，<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658277170806-76763a25-2fc4-40ea-b872-42c1dc68168b.jpeg#clientId=ue74eaa66-708c-4&from=paste&id=uf8f88dbc&originHeight=303&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8039678-7e11-40c2-9624-ff6d6235a39&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658277170759-2aeba4fe-0a30-4788-add0-b11dd5752866.jpeg#clientId=ue74eaa66-708c-4&from=paste&id=u49582347&originHeight=204&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3d42646c-9cea-4b97-9d53-ad46c2c4977&title=)<br />这里可以看到，动态从容器中获取实例。不过需要注意一点，那就是通过 `Lookup` 注解的方法是有要求的，因为是需要被重写，所以针对这个方法只能使用下面的这种定时定义，必须是 `public` 或者 `protected`，可以是抽象方法，而且方法不能有参数。
```java
<public|protected> [abstract] <return-type> theMethodName(no-arguments);
```
<a name="fBA0E"></a>
## 总结
通过几个例子，介绍了一下如何在单例类中获取原型类的实例，提供了三种解法，其中解法一不推荐，解法二和解法三异曲同工，感兴趣的小伙伴可以自己尝试一下。
