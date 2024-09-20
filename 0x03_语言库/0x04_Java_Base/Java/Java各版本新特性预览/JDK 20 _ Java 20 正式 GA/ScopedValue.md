`ThreadLocal`是一种实现将变量在各线程之间隔离的方案，也叫线程局部变量表。在Java中每个线程都拥有一个`ThreadLocal`下的`ThreadLocalMap`类型的变量，它用来存储定义在线程中的`ThreadLocal`对象，`ThreadLocalMap`的键是一个弱引用，指向对应的`ThreadLocal`对象。<br />但值得每一位Java开发者注意的是`ThreadLocal`变量如果不及时`remove()`会造成严重的内存泄露问题。在JDK 20 Early-Access Build 28版本中便针对`ThreadLocal`类重新设计了一个`ScopedValue`类。<br />`ScopedValue`是一个JDK孵化功能在已发布的JDK20版本中需要手动配置才能使用，`ScopedValue`的作用是在某些情况下作为`ThreadLocal`的替代。在同一线程上运行的不同代码可以通过`ScopedValue`共享不可变的值。`ScopedValue`主要是为了解决虚拟线程使用`ThreadLocal`时可能存在的一些问题。<br />在本期文章中讲会介绍几个`ThreadLocal`在开发实战中的案例背景以及详细介绍在JDK19中提出的新的并发工具和JDK20正在孵化的`ScopedValue`类。
<a name="VB1TJ"></a>
## `ThreadLocal`
<a name="xY4Bk"></a>
### 基本概念
在引言中已经大致地介绍了一部分关于`ThreadLocal`的概念，`ThreadLocal`存在的意义不仅仅是为了实现隔离更重要的是为了解决对象的复用问题，这些思想在数据库连接池框架中都有体现。但是`ThreadLocal`又会导致内存泄漏问题，这是由于`ThreadLocalMap`中的`ThreadLocal`对象没有被JVM及时回收导致的，为了解决这个问题而使用了弱引用`WeakReference`，但是弱引用的`ThreadLocal`被设置为null后不及时通过`remove`方法来清理也同样会导致内存泄漏问题。
<a name="qcwXw"></a>
### 应用案例
以最常见的Spring应用为例，`ThreadLocal`在这些应用中完全可以大做文章。<br />在一些电商项目的Spring业务中会需要对每个请求进行线程隔离：
```java
@Service
public class ShoppingCartService {
    private ThreadLocal<ShoppingCart> cartHolder = new ThreadLocal<>();

    public ShoppingCart getCurrentCart() {
        ShoppingCart cart = cartHolder.get();
        if (cart == null) {
            cart = new ShoppingCart();
            cartHolder.set(cart);
        }
        return cart;
    }

    public void checkout() {
        // 获取当前购物车
        ShoppingCart cart = getCurrentCart();
        // 执行结账操作
        // 清除当前线程中购物车的信息，防止内存泄露
        cartHolder.remove();
    }
}

// 购物车类
class ShoppingCart {
    private List<Product> products = new ArrayList<>();

    public void addProduct(Product product) {
        products.add(product);
    }

    public List<Product> getProducts() {
        return products;
    }
}
```
这段代码中，ShoppingCartService 是一个 Spring Bean，用来管理购物车信息。在这个 Bean 里，使用了 `ThreadLocal<ShoppingCart>` 来保存每个线程的购物车信息。`getCurrentCart` 方法首先会从 `ThreadLocal` 中获取购物车信息，如果当前线程没有对应的购物车信息，那么就创建一个新的购物车，并保存到 `ThreadLocal` 中。checkout 方法用来执行结账操作，结账完成后，需要通过`cartHolder.remove();`清除当前线程中的购物车信息，以防止内存泄露。这样，即使在多线程环境下，每个线程都有自己独立的购物车信息，互不影响。这就是 ThreadLocal 在解决 Spring Bean 线程安全问题上的一个应用场景。<br />在业务逻辑中使用ThreadLocal是很常见的一种处理线程隔离数据的方法。不妨思考，如果一系列接口都需要先进行用户认证，然后再操作这个用户数据应该怎么做？这个问题是非常简单的，使用Spring Security整合JWT对前端传递的Token进行解析得到用户名后再校验用户即可。这完全可以封装成一个切面来处理，至于业务中又需要用到这个用户只需要再从Spring Security中去取就行了。但是一些业务会在其切面中对用户数据进行一些预处理，如更新访问接口时间戳。那么这再从Spring Security中去取就显得不妥了，因为这可能会导致和想要得到的对象预期不符。那么该怎么做呢？很显然是使用ThreadLocal来缓存这个用户对象，让这个User在整个http session过程中都处于仅存的一份状态：
```java
@Aspect
@Component
public class UserConsistencyAspect {
    // 每个UserVo启用线程隔离，在进入切面后开始创建，在业务逻辑中用完就被GC回收
    private static final ThreadLocal<UserVo> userHolder = new ThreadLocal<>();

    @Pointcut("@annotation(org.nozomi.common.annotation.GetUser)")
    public void userAuthPoint() {}

    @Around("userAuthPoint()")
    public Object injectUserFromRequest(ProceedingJoinPoint joinPoint) throws Throwable {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        UserVo operator = (UserVo) authentication.getPrincipal();
        if (operator == null) {
            return Response.fail("用户不存在");
        }
        userHolder.set(operator);
        return joinPoint.proceed();
    }

    /**  
  * 取出当前线程中的UserVo对象，这些UserVo是跟随http创建的线程而隔离的
  *
  * @return 当前线程的UserVo
  */
    public static UserVo getUser() {
        return userHolder.get();
    }
}
```
在业务中使用这个切面以及`UserConsistencyAspect.getUser()`方法就可以获取到这个http session中的User对象了。<br />在解决Spring Bean的线程不安全问题时会使用到ThreadLocal来保障Bean的线程安全：
```java
@Service
public class ProductService {

    private final ThreadLocal<Session> sessionThreadLocal = new ThreadLocal<>();

    public Product getProductById(String id) {
        Session session = getSession();
        return session.get(Product.class, id);
    }

    public void updateProduct(Product product) {
        Session session = getSession();
        session.update(product);
    }

    private Session getSession() {
        Session session = sessionThreadLocal.get();
        if (session == null) {
            session = sessionFactory.openSession();
            sessionThreadLocal.set(session);
        }
        return session;
    }

    public void closeSession() {
        Session session = sessionThreadLocal.get();
        if (session != null) {
            session.close();
            sessionThreadLocal.remove();
        }
    }
}
```
在很多情况下，开发者会使用Spring来管理数据库的会话或者事务，但是这样的Bean通常是线程不安全的，比如 Hibernate的`SessionFactory`或者MyBatis的`SqlSessionFactory`。这些工厂产生的Session是线程不安全的。在电商项目中，一个常见的场景是，可能会在一个请求处理的过程中需要多次和数据库进行交互。这个时候，为了保证在一个请求中使用同一个数据库会话（Session），通常会把这个 Session 放在一个`ThreadLocal`中。这样，即使在一个线程中的不同方法里，也可以获取到同一个Session。在这个例子中，每个线程都有自己的Session实例，存储在ThreadLocal中。不同的线程调用`getSession()`方法时，都会从ThreadLocal中获取到属于自己的Session。但是事实上这些session的处理已经在mybatis或hibernate中都已经通过ThreadLocal处理好了不需要开发者再在业务中对session进行隔离。这里的例子主要是为了解释 ThreadLocal 是如何工作的，并不是实际开发中推荐的做法。
<a name="qdW5w"></a>
## `StructuredTaskScope`
结构化并发编程式（Structured Concurrent）和虚拟线程（Virtual Threads）息息相关。要了解`ScopedValue`就必须先了解这两个概念，自JDK5以来一直保持着这样一种理念：不应该直接与线程交互。正确的模式是将任务作为`Runnable`或Callable提交给`ExecutorService`或Executor，然后对返回的Future进行操作。Loom项目中一直保留了这种模型，并添加了一些不错的功能。这里要介绍的第一个对象是Scope对象， 确切的类型是`StructuredTaskScope`。可以把这个对象看做一个虚拟线程启动器，以Callable的形式向它提交任务，将得到一个future返回，并且这个callable将在由作用域Scope创建的虚线程种执行。这很像`Executor`。但二者之间也有很大的区别。
```java
public static Weather readWeather() throws Exception {
    // try-with-resource
    try(var scope = new StructuredTaskScope<Weather>()) {
        Future<Weather> future = scope.fork(Weather::readWeatherFrom);
        scope.join();
        return future.resultNow();
    }
}
```
`StructuredTaskScope`实例是`AutoCloseable`（自动关闭）的，可以使用try-with-resource模式。通过fork()方法fork一个Callable类型的任务，`fork()`方法返回一个Future对象，调用`join()`方法阻塞调用，它将阻塞当前线程，直到所有提交（frok）给`StructuredTaskScope`的任务都完成。最后调用Future的`resultNow()`获取结果并返回。`resultNow()`将抛出异常，如果在Future完成前调用它，所以要在`join()`方法中调用并将其返回。
<a name="CZewF"></a>
## `ScopedValue`
<a name="XzCOD"></a>
### 基本概念
与结构化并发更相关的应该是JDK8提出的`CompletableFuture`。ScopedValue是基于结构化并发理念在JDK20中被孵化的一个功能，它显然不是为了取代ThreadLocal出现的，而是能让结构化并发中的虚拟线程也能各自享有外部的变量。其实结构化并发中也可以使用`ThreadLocal`，但是`ThreadLocal`本身存在一些很大的问题：

1. `ThreadLocal`变量是可变的，任何运行在当前线程中的代码都可以修改该变量的值，很容易产生一些难以调试的bug。
2. `ThreadLocal`变量的生命周期会很长。当使用`ThreadLocal`变量的`set`方法，为当前线程设置了值之后，这个值在线程的整个生命周期中都会保留，直到调用`remove`方法来删除。但是绝大部分开发人员不会主动调用`remove`来进行删除，这可能造成内存泄漏。
3. `ThreadLocal`变量可以被继承。如果一个子线程从父线程中继承`ThreadLocal`变量，那么该子线程需要独立存储父线程中的全部`ThreadLocal`变量，这会产生比较大的内存开销。

虚拟线程的特点是数量巨大，但是每个虚拟线程的生命周期较短，因此不容易产生内存泄漏问题。但是线程继承所带来的内存开销会更大。为了解决这些问题便孵化了`ScopedValue`，`ScopedValue`具备`ThreadLocal`的核心特征，也就是每个线程只有一个值。与`ThreadLocal`不同的是，`ScopedValue`是不可变的，并且有确定的作用域，这也是名字中scoped的含义。
<a name="P3Yt7"></a>
### 基本用法
`ScopedValue`对象用`jdk.incubator.concurrent`包中的`ScopedValue`类来表示。使用`ScopedValue`的第一步是创建`ScopedValue`对象，通过静态方法`newInstance`来完成，`ScopedValue`对象一般声明为`static final`。由于ScopedValue是孵化功能，要想使用需要在项目的第一级包目录的同级目录中创建一个java类module-info.java来将其引入模块中：
```java
module dioxide.cn.module {  
    requires jdk.incubator.concurrent;  
}
```
同时需要再启动参数VM Option中启用预览功能`--enable-preview`。下一步是指定`ScopedValue`对象的值和作用域，通过静态方法`where`来完成。`where`方法有 3 个参数：

- `ScopedValue` 对象
- `ScopedValue` 对象所绑定的值
- `Runnable`或`Callable`对象，表示`ScopedValue`对象的作用域

在`Runnable`或`Callable`对象执行过程中，其中的代码可以用`ScopedValue`对象的get方法获取到where方法调用时绑定的值。这个作用域是动态的，取决于`Runnable`或`Callable`对象所调用的方法，以及这些方法所调用的其他方法。当`Runnable`或`Callable`对象执行完成之后，`ScopedValue`对象会失去绑定，不能再通过get方法获取值。在当前作用域中，`ScopedValue`对象的值是不可变的，除非再次调用where方法绑定新的值。这个时候会创建一个嵌套的作用域，新的值仅在嵌套的作用域中有效。使用作用域值有以下几个优势：

- 提高数据安全性：由于作用域值只能在当前范围内访问，因此可以避免数据泄露或被恶意修改。
- 提高数据效率：由于作用域值是不可变的，并且可以在线程之间共享，因此可以减少数据复制或同步的开销。
- 提高代码清晰度：由于作用域值只能在当前范围内访问，因此可以减少参数传递或全局变量的使用。

Java JEP 429是一个正在孵化的新特性，它提供了一种在线程内部和线程之间共享不可变数据的方式。目前，Java JEP 429 还处于孵化器阶段，并没有被正式纳入 Java 语言规范。
```java
public class Main {
    // 声明了一个静态的、最终的 ScopedValue<String> 实例
    // ScopedValue 是一个支持在特定范围内（如任务或线程）中传递值的类
    // 它的使用类似于 ThreadLocal，但更适合于结构化并发
    private static final ScopedValue<String> VALUE = ScopedValue.newInstance();

    public static void main(String[] args) throws Exception {
        System.out.println(Arrays.toString(stringScope()));
    }

    public static Object[] stringScope() throws Exception {
        return ScopedValue.where(VALUE, "value", () -> {
            // 使用 try-with-resource 来绑定结构化并发的作用域
            // 用于自动管理资源的生命周期，这是一个结构化任务范围
            // 在这个范围内创建的所有子任务都将被视为范围的一部分
            // 如果范围中的任何任务失败，所有其他任务都将被取消
            try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
                // 使用了 scope.fork 来创建两个并行的任务
                // 每个任务都在执行上下文中获取 VALUE 的值，并对其进行操作
                Future<String> user  = scope.fork(VALUE::get);
                Future<Integer> order = scope.fork(() -> VALUE.get().length());
                // join() 方法等待所有范围内的任务完成
                // throwIfFailed() 方法会检查所有任务的结果，如果任何任务失败，则会抛出异常
                scope.join().throwIfFailed();
                // 在所有任务完成后，使用 resultNow() 方法获取每个任务的结果，并将结果放入一个对象数组中
                return new Object[]{user.resultNow(), order.resultNow()};
            }
        });
    }
}
```
这段代码展示了如何使用`ScopedValue`和结构化并发来创建并执行多个并行任务，并安全地传递和操作任务上下文中的值。
<a name="ph5XL"></a>
### 源码分析
> A value that is set once and is then available for reading for a bounded period of execution by a thread. A ScopedValue allows for safely and efficiently sharing data for a bounded period of execution without passing the data as method arguments. ScopedValue defines the where(ScopedValue, Object, Runnable) method to set the value of a ScopedValue for the bouned period of execution by a thread of the runnable's run method. The unfolding execution of the methods executed by run defines a dynamic scope. The scoped value is bound while executing in the dynamic scope, it reverts to being unbound when the run method completes (normally or with an exception). Code executing in the dynamic scope uses the ScopedValue get method to read its value. Like a thread-local variable, a scoped value has multiple incarnations, one per thread. The particular incarnation that is used depends on which thread calls its methods.

在开始ScopedValue的源码分析之前，先看一下Java doc的介绍：`ScopedValue` 是一个对象，它被设置一次后，在执行期间由一个线程有限期地读取。`ScopedValue` 允许在有限的执行期间内在不将数据作为方法参数传递的情况下安全、有效地共享数据。`ScopedValue` 定义了 `where(ScopedValue, Object, Runnable)` 方法，这个方法在一个线程执行 runnable 的 run 方法的有限执行期间内设置 ScopedValue 的值。由 run 执行的方法展开执行定义了一个动态作用域。在动态作用域中执行时，作用域值是绑定的，当 run 方法完成时（正常或异常），它恢复到未绑定状态。在动态作用域中执行的代码使用 `ScopedValue` 的 get 方法来读取其值。与线程局部变量类似，作用域值有多个化身，每个线程一个。使用哪个化身取决于哪个线程调用其方法。`ScopedValue` 的一个典型用法是在 final 和 static 字段中声明。字段的可访问性将决定哪些组件可以绑定或读取其值。ScopedValue中有3个内部类，分别是Snapshot、Carrier、Cache，他们在ScopedValue中起着至关重要的角色。
<a name="M331Y"></a>
#### `Snapshot`
> An immutable map from ScopedValue to values. Unless otherwise specified, passing a null argument to a constructor or method in this class will cause a NullPointerException to be thrown.

Snapshot是一个从ScopedValue到值的不可变映射。除非特别说明，否则将null参数传递给这个类的构造器或方法会导致抛出NullPointerException异常。这个类的主要用途是为ScopedValue实例创建一个不可变的映射，这样在运行时，无论其它代码如何修改原始的ScopedValue实例，Snapshot中的值都不会发生变化。它为了提供一个安全的方式来在多线程环境下共享值。
<a name="qSdHL"></a>
#### Carrier
> A mapping of scoped values, as keys, to values. A Carrier is used to accumlate mappings so that an operation (a Runnable or Callable) can be executed with all scoped values in the mapping bound to values. The following example runs an operation with k1 bound (or rebound) to v1, and k2 bound (or rebound) to v2. ScopedValue.where(k1, v1).where(k2, v2).run(() -> ... ); A Carrier is immutable and thread-safe. The where method returns a new Carrier object, it does not mutate an existing mapping. Unless otherwise specified, passing a null argument to a method in this class will cause a NullPointerException to be thrown.

`Carrier`类用于累积映射，以便可以执行一个操作（Runnable或Callable），在该操作中，映射中的所有scoped values都绑定到值。`Carrier`是不可变的，并且是线程安全的。`where`方法返回一个新的`Carrier`对象，不会改变现有的映射。这是用于在`ScopedValue`实例和对应值之间创建和保持映射关系的工具，使得这些映射关系可以在执行操作时被一并应用。
<a name="rHqrP"></a>
#### Cache
> A small fixed-size key-value cache. When a scoped value's get() method is invoked, we record the result of the lookup in this per-thread cache for fast access in future.

Cache是一个小型的固定大小的键值缓存。当调用一个scoped value的`get()`方法时，在这个每线程缓存中记录查找的结果，以便在将来快速访问。这个类的主要作用是优化性能。通过缓存`get()`方法的结果，可以避免在多次获取同一个`ScopedValue`的值时进行重复的查找操作。只有当`ScopedValue`的值被更改时，才需要更新缓存。
<a name="r2v2h"></a>
#### `where()`
`where()`方法是`ScopedValue`类的核心方法与入口，它接收三个参数。当操作完成时（正常或出现异常），`ScopedValue`将在当前线程中恢复为未绑定状态，或恢复为先前绑定时的先前值。
```
graph TB
    A("ScopedValue.where(key, value, op)")
    A --> B("ScopedValue.Carrier.of(key, value)")
    B --> C("ScopedValue.Carrier.where(key, value, prev)")
    C --> D("返回ScopedValue.Carrier对象")
```
作用域值旨在以结构化方式使用。如果op已经创建了一个`StructuredTaskScope`但没有关闭它，那么退出op会导致在动态范围内创建的每个`StructuredTaskScope`被关闭。这可能需要阻塞，直到所有子线程都完成了它们的子任务。关闭是按照创建它们的相反顺序完成的。<br />使用`ScopedValue.where(key, value, op);`等价于使用`ScopedValue.where(key, value).call(op);`
```java
public static <T, R> R where(ScopedValue<T> key,
                             T value,
                             Callable<? extends R> op) throws Exception {
    return where(key, value).call(op);
}
```
这个方法会将前两个参数委派给`Carrier.of(key, value);`方法
```java
/*  
 * 返回由单个绑定组成的新集合
 */  
static <T> Carrier of(ScopedValue<T> key, T value) {  
    return where(key, value, null);  
}
/**  
 * 向该map添加绑定，返回一个新的 Carrier 实例
 */  
private static final <T> Carrier where(ScopedValue<T> key, T value,  
                                       Carrier prev) {  
    return new Carrier(key, value, prev);  
}
```
在`Carrier`类中`where`方法会返回一个新的`Carrier`对象，这是一种责任链的设计模式
<a name="XJR0L"></a>
#### `call()`
`where`方法主要是构建`Carrier`对象，而后这些都会委派给后续的`Carrier`中的`call`方法来实现对`Callable`的一个调用。调用关系如下：
```
graph TB
 D("ScopedValue.Carrier")
    D --> E("ScopedValue.Carrier.call(op)")
    E -->|分支1| F("ScopedValue.Cache.invalidate()")
    E -->|分支2| G("ScopedValue.Carrier.runWith(newSnapshot, op)")
    G --> H("ScopedValueContainer.call(op)")
    H --> I("ScopedValueContainer.callWithoutScope(op)")
    I --> J("Callable.call()")
```
`call`方法调用链的方法中有很多细节是关于处理Snapshot和Cache的，这些内容可能在将来的Java版本中发生变化这里就不再赘述。
<a name="gB0Hd"></a>
## 小结
`ThreadLocal`与`ScopedValue`在Java并发编程中都起着至关重要的作用，他们分别适用于不同的场景，开发人员需要根据具体需求来选择使用。`ThreadLocal`主要用于普通并发编程。在Java中，每个线程都有自己的栈，栈中存储的是这个线程需要的局部变量。`ThreadLocal`则提供了一个独特的机制，使每个线程都可以拥有自己独立的一份数据，其他线程无法访问。这种机制非常适用于那些在处理并发编程中需要隔离线程状态或者实现线程间数据隔离的场景，例如数据库连接、Session会话等。<br />然而，`ThreadLocal`虽然能够实现线程级别的数据隔离，但它本身并不能解决更复杂的并发问题，例如异步任务的并发控制、异步任务之间的数据共享等问题。这就需要一种新的工具来解决，即`ScopedValue`。<br />`ScopedValue`是Java引入的新特性，它是为了支持结构化并发编程而设计的。结构化并发允许开发人员通过定义并发的结构，对并发程序的生命周期进行管理。`ScopedValue`提供了一种方法，使得一个值可以在一个定义好的执行范围（也就是一个“scope”）内，被并发任务共享。<br />在结构化并发编程中，`ScopedValue`主要用于实现并发任务间的数据共享，和`ThreadLocal`相比，`ScopedValue`可以更好地控制并发任务之间的数据共享，同时也可以更好地对并发任务的生命周期进行管理。例如，一个线程可以将一个值放入`ScopedValue`中，然后在该线程启动的所有子线程中都可以访问这个值。这样可以避免在异步并发任务中传递大量参数，简化了并发编程。
