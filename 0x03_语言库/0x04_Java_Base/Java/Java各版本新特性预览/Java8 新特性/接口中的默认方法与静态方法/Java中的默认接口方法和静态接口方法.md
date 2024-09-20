Java 默认接口方法 静态接口方法
<a name="Oaj6j"></a>
## 默认接口方法
java中的默认接口方法是这样定义的：
```java
public interface NewInterface {

    void otherMethod();

    default void doSomething() {
        System.out.println(" do something ");
    }
}

public class NewInterfaceImpl implements NewInterface {
    @Override
    public void otherMethod() {

    }
}
```
在接口中使用关键字`default`声明并提供具体实现，而且该方法不需要添加`public`关键字就可以公开调用，甚至可以在其实现类中覆写（`@Override`）。<br />通常基于抽象的概念设计中，抽象往往有多个实现，如果没有多个实现也不太必要去抽象。而且一个接口的实现必须实现该接口所有的抽象方法，这并不是一个可选择的行为，如果在一个接口中增加了新的方法就会影响到其所有的实现类，很可能有些实现类根本不需要这个功能，但是又不得不去实现该功能。
:::tips
接口默认方法是解决这个问题的有效方式。它允许在接口中直接添加一个功能实现，并且所有的实现类都可以使用这个功能实现而无需修改这些实现类。通过这种方式，可以巧妙地保留向后兼容性。另外这也是扩展函数式接口的唯一方法，如果没有默认方法，一旦函数式接口需要扩展就会引发“灾难性”的连锁反应。
:::
这样说来默认接口方法确实是一个非常好的特性，但是还是有一些需要注意的问题。由于 Java 允许类实现多个接口，当一个类实现多个定义相同默认方法的接口时会发生什么情况? 再定义一个带默认接口方法的接口，并且方法的名称也是doSomething:
```java
public interface AnotherInterface {
    void anotherMethod();

    default void doSomething() {
        System.out.println(" do another something ");
    }
}
```
这时候如果一个类同时实现了`NewInterface`和`AnotherInterface`<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631494830334-df9069b4-05eb-4b2b-9021-25a4ae086b28.webp#clientId=u69f81672-f4fb-4&from=paste&id=uf3a543a4&originHeight=408&originWidth=877&originalType=url&ratio=1&status=done&style=none&taskId=ue81dfd3e-adb1-4fac-ae8e-751c29cf8d9)<br />方法签名引起的冲突<br />因为方法签名相同，无法判断使用哪个方法，连编译都过不去。为了解决这个分歧，必须明确地为这些方法提供一个实现。
<a name="tHnRR"></a>
## 静态接口方法
从 Java 8 开始，还可以在接口中定义静态方法。由于静态接口方法不属于特定对象，因此它们不是接口实现类的一部分，不能通过实现类去调用静态接口方法，只能通过接口去调用。利用这个特性可以实现一些固定范式的功能，并且该功能不会因为接口的多态而改变，避免出现不良后果。
```java
@FunctionalInterface
public interface Customizer<T> {

    void customize(T t);

    static <T> Customizer<T> withDefaults() {
        return (t) -> {
        };
    }

}
```
这是Spring Security 自定义配置的关键接口，如果使用默认配置可以直接通过静态方法实现，如果想自定义就通过实现抽象接口，这样统一的范式就能够稳定下来了，一般作为一种功能的辅助手段，而且这种手段不能被改写。
<a name="Jc6QZ"></a>
## 两者的场景差异

- **默认接口方法** 提供默认的功能实现，不喜欢可以改。
- **静态接口方法** 同样提供一个默认的功能实现，子类无法进行覆写。
