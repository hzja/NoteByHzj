JavaSpringBoot
<a name="OgEiH"></a>
## 什么是循环依赖？
循环依赖是指在Spring Boot 应用程序中，两个或多个类之间存在彼此依赖的情况，形成一个循环依赖链。<br />在这种情况下，当一个类在初始化时需要另一个类的实例，而另一个类又需要第一个类的实例时，就会出现循环依赖问题。这会导致应用程序无法正确地初始化和运行，因为Spring Boot 无法处理这种循环依赖关系。
<a name="Act3B"></a>
## 问题及症状
在2.6.0之前，Spring Boot会自动处理循环依赖的问题。2.6.0及之后的版本会默认检查循环依赖，存在该问题则会报错。<br />ComponentA类注入ComponentB类，ComponentB类注入ComponentA类，就会发生循环依赖的问题。
<a name="VmU73"></a>
### ComponentA
```java
import org.springframework.stereotype.Service;
import javax.annotation.Resource;

@Service
public class ComponentA {

    @Resource
    private ComponentB componentB;

}
```
<a name="nhbmR"></a>
### ComponentB
```java
import org.springframework.stereotype.Service;
import javax.annotation.Resource;

@Service
public class ComponentB {

    @Resource
    private ComponentA componentA;

}
```
<a name="uMDeS"></a>
### 错误
现在，2.6.0 这个版本已经默认禁止 Bean 之间的循环引用，则基于上面的代码，会报错：
```
***************************
APPLICATION FAILED TO START
***************************

Description:

The dependencies of some of the beans in the application context form a cycle:

┌─────┐
|  componentA
↑     ↓
|  componentB
└─────┘


Action:

Relying upon circular references is discouraged and they are prohibited by default. Update your application to remove the dependency cycle between beans. As a last resort, it may be possible to break the cycle automatically by setting spring.main.allow-circular-references to true.
```
<a name="Ctse8"></a>
## 解决方法
循环依赖是指两个或更多的组件之间存在着互相依赖的关系。在Spring Boot应用程序中，循环依赖通常是由以下几种情况引起的：

- **构造函数循环依赖：** 两个或更多的组件在它们的构造函数中互相依赖。
- **属性循环依赖：** 两个或更多的组件在它们的属性中互相依赖。
- **方法循环依赖：** 两个或更多的组件在它们的方法中互相依赖。

Spring Boot提供了一些解决循环依赖的方法：

- **构造函数注入：** 在构造函数中注入依赖项，而不是在属性中注入。
- **Setter注入：** 使用setter方法注入依赖项，而不是在构造函数中注入。
- **延迟注入：** 使用`@Lazy`注解延迟加载依赖项。
- `**@Autowired**`**注解的required属性：** 将`required`属性设置为false，以避免出现循环依赖问题。
- `**@DependsOn**`**注解：** 使用`@DependsOn`注解指定依赖项的加载顺序，以避免出现循环依赖问题
<a name="kMpKq"></a>
### 构造器注入的案例
假设有以下两个类:
```java
public class A {
    private B b;

    public A() {
        // ...
    }

    public void setB(B b) {
        this.b = b;
    }
}

public class B {
    private A a;

    public B() {
        // ...
    }

    public void setA(A a) {
        this.a = a;
    }
}
```
通过构造函数注入可以避免循环依赖，改造后的代码如下:
```java
public class A {
    private B b;

    public A(B b) {
        this.b = b;
    }
}

public class B {
    private A a;

    public B(A a) {
        this.a = a;
    }
}
```
这样，在创建 A 实例时，只需要将 B 实例传递给 A 的构造函数即可，不需要再通过 setter 方法将 B 实例注入到 A 中。同理，在创建 B 实例时，只需要将 A 实例传递给 B 的构造函数即可，不需要再通过 setter 方法将 A 实例注入到 B 中。这样可以避免循环依赖。
<a name="rQu5Q"></a>
### 延迟注入的案例
假设有如下情景：<br />类A依赖于类B，同时类B也依赖于类A。这样就形成了循环依赖。<br />为了解决这个问题，可以使用`@Lazy`注解，将类A或类B中的其中一个延迟加载。<br />例如，可以在类A中使用`@Lazy`注解，将类A延迟加载，这样在启动应用程序时，Spring容器不会立即加载类A，而是在需要使用类A的时候才会进行加载。这样就避免了循环依赖的问题。<br />示例代码如下：
```java
@Component
public class A {

    private final B b;

    public A(@Lazy B b) {
        this.b = b;
    }

    //...
}

@Component
public class B {

    private final A a;

    public B(A a) {
        this.a = a;
    }

    //...
}
```
在类A中，使用了`@Lazy`注解，将类B延迟加载。这样在启动应用程序时，Spring容器不会立即加载类B，而是在需要使用类B的时候才会进行加载。<br />这样就避免了类A和类B之间的循环依赖问题。
<a name="z1Bcw"></a>
### 接口隔离的案例
假设有两个类A和B，它们之间存在循环依赖：
```java
public class A {
    private final B b;
    public A(B b) {
        this.b = b;
    }
}

public class B {
    private final A a;
    public B(A a) {
        this.a = a;
    }
}
```
这时候，如果直接在Spring Boot中注入A和B，就会出现循环依赖的问题。为了解决这个问题，可以使用接口隔离。<br />首先，定义一个接口，包含A和B类中需要使用的方法：
```java
public interface Service {
    void doSomething();
}
```
然后，在A和B类中分别注入Service接口：
```java
public class A {
    private final Service service;
    public A(Service service) {
        this.service = service;
    }
}

public class B {
    private final Service service;
    public B(Service service) {
        this.service = service;
    }
}
```
最后，在Spring Boot中注入Service实现类：
```java
@Service
public class ServiceImpl implements Service {
    private final A a;
    private final B b;
    public ServiceImpl(A a, B b) {
        this.a = a;
        this.b = b;
    }
    @Override
    public void doSomething() {
        // ...
    }
}
```
通过这种方式，A和B类不再直接依赖于彼此，而是依赖于同一个接口。同时，Spring Boot也能够正确地注入A、B和ServiceImpl，避免了循环依赖的问题。
