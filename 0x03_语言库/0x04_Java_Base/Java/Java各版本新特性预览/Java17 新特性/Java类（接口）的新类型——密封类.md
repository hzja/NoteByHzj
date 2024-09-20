Java 密封类<br />密封类是Java 17正式支持的一个新特性，它让Java中类的继承可以更加细粒度的进行控制。今天就来认识一下这个新的功能。
<a name="ZC8kf"></a>
## 密封类
在以往的Java类继承中，Java类的继承控制非常有限，仅能通过`final`关键字和访问控制符来控制类的继承。例如`final`类无法被集成；包私有类仅仅只能在该包下进行继承。<br />这显然是不够的。如果一个功能只允许出现在Phone和Pad上，不允许出现在Computer上。如果不对该功能的继承实现进行限制，开发人员将很容易滥用该功能的实现类，错误地重用一些代码。这就是密封类产生的原因。
<a name="bfYVN"></a>
### 密封类的声明
:::tips
密封类不仅仅可以是类，也可以是接口。文章中的密封类为统称
:::
密封类（接口）可以明确哪些类和接口可以对其扩展或实现。可以通过sealed修饰符来表明某个类是密封类。但是下面是一个错误的密封类声明：
```java
/**
 * 这是一个错误的示范
 */
public sealed interface SealedService {
   
    void doSomething();
}
```
密封类（接口）在声明的时候必须明确可继承（实现）的范围，所以上面的写法是错误的。必须用`permits`子句指定允许扩展密封类的类，而且`permits`关键字位于`extends`或者`implements`之后。
:::tips
简而言之，密封类明确了哪些其他类（或接口）可以扩展它们。
:::
下面是正确的写法：
```java
/**
 * 这是一个正确的示范，明确了可继承的子类为{@link SealedServiceImpl}
 * 该密封类接口同时实现了{@link SuperService}
 */
public sealed interface SealedService extends SuperService permits SealedServiceImpl {
    void doSomething();
}

/**
 * 密封类子类
 */
public final class SealedServiceImpl implements SealedService {
    @Override
    public void doSomething() {
        System.out.println("这是一个密封类子类");
    }
}
```
<a name="fd0b6"></a>
### 密封类子类的类型
在上面示例中，密封类（接口）的实现类用了`final`关键字标记，当然密封类的实现类还可以是密封类：
```java
/**
 * 密封类子类
 */
public sealed class SealedServiceImpl implements SealedService permits SonService {
    @Override
    public void doSomething() {
        System.out.println("这是一个密封类子类");
    }
}


public final class SonService extends SealedServiceImpl {
}
```
那么难道密封类（接口）的子类只能是`final`类或者密封类，就不能再扩展了？答案是否定的，只需要使用关键字`non-sealed`显式声明密封类的继承实现为非密封类就可以继续扩展了。
```java
public non-sealed class SealedServiceImpl implements SealedService {
    @Override
    public void doSomething() {

    }

    /**
     * 用{@code non-sealed}声明非密封类，就可以继续扩展了
     */
    static class NonSealedExtend extends SealedServiceImpl {

    }

}
```
总结一下，密封类的子类要么是`final Class`；要么是`sealed Class`；要么是non-sealed Class。
<a name="j12KP"></a>
### `permits` 声明的类必须是直接子类
密封类`permits`关键字声明的子类必须是直接实现类，为了证明这一点这样写：
```java
/**
 * 错误的示范
 */
public sealed interface SealedService extends SuperService permits SealedServiceImpl, SonService {
    void doSomething();
}

public sealed class SealedServiceImpl implements SealedService permits SonService {
    @Override
    public void doSomething() {
        System.out.println("这是一个密封类子类");
    }
}

public final class SonService extends SealedServiceImpl {
}
```
使用`SonService`间接实现了`SealedService`，结果报错，报错信息要求必须是直接的继承关系。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635773399965-57432e2c-4059-4d39-902f-c7edde71d9e9.webp#clientId=ufb4b869e-c2b7-4&from=paste&id=ub2a2405b&originHeight=457&originWidth=713&originalType=url&ratio=1&status=done&style=none&taskId=uddf853ce-c79e-4534-8d9a-5e7a0cd9c65)<br />错误的密封类继承实现<br />从上图可以看出`SonService`并非直接实现`SealedService`，这样会打破密封类的规则，所以无法编译通过。<br />密封类中`permits`关键字声明的子类必须是直接子类，不可间接实现。
<a name="fGLhE"></a>
### 密封类不支持匿名类和函数式接口
由于密封类必须明确继承实现关系，所以它不支持匿名类。
```java
/**
 * 密封类无法使用匿名类
 *
 * @return the sealed service
 */
public SealedService sealedService(){
    // 提示 Anonymous classes must not extend sealed classes
    return new SealedService() {
        @Override
        public void doSomething() {
            
        }
    };
}
```
同样也不支持函数式接口：
```java
/**
 * 错误的示范
 */
@FunctionalInterface
public sealed interface SealedService permits SealedServiceImpl {
    void doSomething();
}
```
<a name="dT8sT"></a>
## 总结
密封类已经在Java 17中正式转正，这也是Java 17的非常重要的特性之一。对于需要细粒度控制继承关系的场景来说是非常有用的。
