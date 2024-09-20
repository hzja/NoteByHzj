JavaSpringIOCDI<br />IoC 和 DI 都是 Spring 框架中的重要概念，就像玫瑰花与爱情一样，IoC 和 DI 通常情况下也是成对出现的。那 IoC 和 DI 什么关系和区别呢？接下来一起来看。
<a name="OWHgZ"></a>
## 1、IoC 介绍
IoC 是 Inversion of Control 的缩写，翻译成中文是“控制反转”的意思，**它不是一个具体的技术，而是一个实现对象解耦的思想。**<br />要解释什么是解耦？就要了解什么是耦合，所谓的**耦合是指：两个或两个以上对象存在依赖，当一方修改之后会影响另一方，那么就说这些对象间存在耦合。**而解耦就是解除两个或两个以上对象，修改之后影响另一方的问题。<br />那 IoC（控制反转）是如何实现解耦的呢？<br />来举一个例子，比如 A 对象中需要使用 B 对象的某个方法，那么通常的实现方法是这样的：
```java
class A {
    public void init() {
        // 调用 B 类中的 init 方法
        B b = new B();
        b.init();
    }
}
class B {
    public B() {
    }

    public void init() {
        System.out.println("你好，世界。");
    }
}
```
然而此时对象 A 和对象 B 是存在耦合的，因为一旦修改了 B 对象构造方法的参数之后，那么 A 对象里面的写法也要跟着改变，比如将构造方法改为以下代码时：
```java
class B {
    public B(String name) {
        System.out.println("姓名：" + name);
    }
    public void init() {
        System.out.println("你好，世界。");
    }
}
```
此时构造方法已经从原本无参构造方法变成了有参的构造方法，这里不考虑构造方法重载的情况，因为实际业务中，很可能是 B 类的构造方法写错了，忘记加参数了，于是后面又补充了一个参数，此时是不需要对构造方法进行重载的，那么此时，之前对象 A 里面的调用就会报错，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660809219707-3c509399-6100-46d9-ac8d-d4cc6e2f6af4.png#clientId=ub811586a-71c3-4&from=paste&id=u8e9ccb5e&originHeight=280&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u62497c1a-bc18-4a44-b5d2-5fdad6fc6af&title=)这就是开发中经常遇到的一个问题，那怎么解决呢？<br />可以通过将对象传递而并 new 对象的方式来解决，如下代码所示：
```java
class A {
    // 先定义一个需要依赖的 B 对象
    private B b;
    // 通过构造方法实现赋值（初始化）
    public A(B b) {
        this.b = b;
    }
    public void init() {
        // 调用 B 类中的 init 方法
        b.init();
    }
}
class B {
    public B(String name) {
        System.out.println("姓名：" + name);
    }
    public void init() {
        System.out.println("你好，世界。");
    }
}
```
这样改造之后，无论构造方法怎么修改，即使需要加更多的参数，而调用它的 A 类都无需做任何修改，这样就实现了对象的解耦。<br />那这个解耦的示例和 IoC 有什么关系呢？<br />**IoC 实现的思路和上述示例一样，就是通过将对象交给 Spring 中 IoC 容器管理，在其他类中不直接 new 对象，而是通过将对象传递到当前类的方式来实现解耦的。**
<a name="xbX19"></a>
### **小结**
**在 new 对象的时代，对象的管理权是由当前类控制的，而有了 IoC 之后，对象的管理权就交给非当前类的 IoC 容器管理了，此时对象的管理权就发生了反转和改变，这就是 IoC，这就是控制（权）反转。**
<a name="zS9Au"></a>
## 2、DI 介绍
DI 是 Dependency Injection 的缩写，翻译成中文是“依赖注入”的意思。**依赖注入不是一种设计实现，而是一种具体的技术**，它是**在 IoC 容器运行期间，动态地将某个依赖对象注入到当前对象的技术就叫做 DI（依赖注入）**。<br />在上述示例中，A 类在使用 B 类时，就是通过构造方法将依赖对象 B 引入的，这种实现方法就可以看作是通过构造方法实现依赖注入的手段，具体代码如下：
```java
class A {
    // 先定义一个需要依赖的 B 对象
    private B b;
    // 通过构造方法实现赋值
    public A(B b) {
        this.b = b;
    }
    public void init() {
        // 调用 B 类中的 init 方法
        b.init();
    }
}
```
<a name="Qg2fh"></a>
## 3、IoC VS DI
从上面的内容中可以看出，IoC 和 DI 虽然定义不同，但它们所做的事情都是一样的，都是用来实现对象解耦的，而二者又有所不同：**IoC 是一种设计思想，而 DI 是一种具体的实现技术**。<br />比如，今天心情比较好，想出去吃顿好的，那么“想吃顿好的”就是一种思想、就是 IoC。<br />但什么才是“好的（饭）”呢？海底捞还是韩式料理？具体吃什么是 DI。<br />**因此“今天心情比较好，想出去吃顿好的”是一种思想、是 IoC，而吃一顿海底捞而非韩式料理就是具体的实现、是 DI。**
<a name="X5Wmb"></a>
## 总结
IoC 和 DI 都是 Spring 框架中的重要概念，它们都是用来实现对象解耦的，其中 IoC（控制反转）是一种设计思想，而 DI（依赖注入）是一种具体的实现手段。
