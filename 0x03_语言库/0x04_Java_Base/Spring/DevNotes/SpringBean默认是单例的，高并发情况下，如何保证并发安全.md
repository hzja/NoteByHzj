Java Spring<br />Spring的bean默认都是单例的，某些情况下，单例是并发不安全的，以Controller举例，问题根源在于，可能会在Controller中定义成员变量，如此一来，多个请求来临，进入的都是同一个单例的Controller对象，并对此成员变量的值进行修改操作，因此会互相影响，无法达到并发安全（不同于线程隔离的概念，后面会解释到）的效果。
<a name="QwfFw"></a>
## 一、抛出问题
首先来举个例子，证明单例的并发不安全性：
```java
@Controller
public class HomeController {
    private int i;
    @GetMapping("testsingleton1")
    @ResponseBody
    public int test1() {
        return ++i;
    }
}
```
多次访问此url，可以看到每次的结果都是自增的，所以这样的代码显然是并发不安全的。
<a name="VmTiV"></a>
## 二、解决方案
因此，为了让无状态的海量Http请求之间不受影响，可以采取以下几种措施：
<a name="D48eZ"></a>
#### 2.1 单例变原型
对web项目，可以Controller类上加注解`@Scope("prototype")`或`@Scope("request")`，对非web项目，在Component类上添加注解`@Scope("prototype")`。<br />优点：实现简单；<br />缺点：很大程度上增大了bean创建实例化销毁的服务器资源开销。
<a name="LDoCm"></a>
#### 2.2 线程隔离类`ThreadLocal`
有人想到了线程隔离类`ThreadLocal`，尝试将成员变量包装为`ThreadLocal`，以试图达到并发安全，同时打印出Http请求的线程名，修改代码如下：
```java
@Controller
public class HomeController {
    private ThreadLocal<Integer> i = new ThreadLocal<>();
    @GetMapping("testsingleton1")
    @ResponseBody
    public int test1() {
        if (i.get() == null) {
            i.set(0);
        }
        i.set(i.get().intValue() + 1);
        log.info("{} -> {}", Thread.currentThread().getName(), i.get());
        return i.get().intValue();
    }
}
```
多次访问此url测试一把，打印日志如下：
```java
[INFO ] 2019-12-03 11:49:08,226 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-1 -> 1
[INFO ] 2019-12-03 11:49:16,457 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-2 -> 1
[INFO ] 2019-12-03 11:49:17,858 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-3 -> 1
[INFO ] 2019-12-03 11:49:18,461 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-4 -> 1
[INFO ] 2019-12-03 11:49:18,974 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-5 -> 1
[INFO ] 2019-12-03 11:49:19,696 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-6 -> 1
[INFO ] 2019-12-03 11:49:22,138 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-7 -> 1
[INFO ] 2019-12-03 11:49:22,869 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-9 -> 1
[INFO ] 2019-12-03 11:49:23,617 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-8 -> 1
[INFO ] 2019-12-03 11:49:24,569 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-10 -> 1
[INFO ] 2019-12-03 11:49:25,218 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-1 -> 2
[INFO ] 2019-12-03 11:49:25,740 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-2 -> 2
[INFO ] 2019-12-03 11:49:43,308 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-3 -> 2
[INFO ] 2019-12-03 11:49:44,420 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-4 -> 2
[INFO ] 2019-12-03 11:49:45,271 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-5 -> 2
[INFO ] 2019-12-03 11:49:45,808 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-6 -> 2
[INFO ] 2019-12-03 11:49:46,272 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-7 -> 2
[INFO ] 2019-12-03 11:49:46,489 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-9 -> 2
[INFO ] 2019-12-03 11:49:46,660 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-8 -> 2
[INFO ] 2019-12-03 11:49:46,820 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-10 -> 2
[INFO ] 2019-12-03 11:49:46,990 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-1 -> 3
[INFO ] 2019-12-03 11:49:47,163 com.cjia.ds.controller.HomeController.test1(HomeController.java:50)
http-nio-8080-exec-2 -> 3
......
```
从日志分析出，二十多次的连续请求得到的结果有1有2有3等等，而期望不管并发请求有多少，每次的结果都是1；同时可以发现web服务器默认的请求线程池大小为10，这10个核心线程可以被之后不同的Http请求复用，所以这也是为什么相同线程名的结果不会重复的原因。
:::info
总结：ThreadLocal的方式可以达到线程隔离，但还是无法达到并发安全。
:::
<a name="zX1Yi"></a>
#### 2.3 尽量避免使用成员变量
有人说，单例bean的成员变量这么麻烦，能不用成员变量就尽量避免这么用，在业务允许的条件下，将成员变量替换为`RequestMapping`方法中的局部变量，多省事。这种方式自然是最恰当的，也是最推荐。代码修改如下：
```java
@Controller
public class HomeController {
    @GetMapping("testsingleton1")
    @ResponseBody
    public int test1() {
        int i = 0;
        // TODO biz code
        return ++i;
    }
}
```
但当很少的某种情况下，必须使用成员变量呢，该怎么处理？
<a name="LArRD"></a>
#### 2.4 使用并发安全的类
Java作为功能性超强的编程语言，API丰富，如果非要在单例bean中使用成员变量，可以考虑使用并发安全的容器，如`ConcurrentHashMap`、`ConcurrentHashSet`等等，将成员变量（一般可以是当前运行中的任务列表等这类变量）包装到这些并发安全的容器中进行管理即可。
<a name="dx4zQ"></a>
#### 2.5 分布式或微服务的并发安全
如果还要进一步考虑到微服务或分布式服务的影响，方式4便不足以处理了，所以可以借助于可以共享某些信息的分布式缓存中间件如Redis等，这样即可保证同一种服务的不同服务实例都拥有同一份共享信息（如当前运行中的任务列表等这类变量）。
<a name="veFBy"></a>
## 三、补充说明
Spring bean作用域有以下5个：

- `singleton`：单例模式，当Spring创建applicationContext容器的时候，Spring会欲初始化所有的该作用域实例，加上lazy-init就可以避免预处理；
- `prototype`：原型模式，每次通过`getBean`获取该bean就会新产生一个实例，创建后Spring将不再对其管理；

（下面是在web项目下才用到的）

- `request`：web开发的都应该明白request的域了吧，就是每次请求都新产生一个实例，和prototype不同就是创建后，接下来的管理，Spring依然在监听；
- `session`：每次会话，同上；
- global session：全局的web域，类似于servlet中的application。
