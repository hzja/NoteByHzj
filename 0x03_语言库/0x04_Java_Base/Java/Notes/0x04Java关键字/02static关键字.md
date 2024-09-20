# Java static

考察代码执行顺序：

```java
public class Parent {
    static {
        System.out.println("Parent static initial block");
    }

    {
        System.out.println("Parent initial block");
    }

    public Parent() {
        System.out.println("Parent constructor block");

    }
}

public class Child extends Parent {
    static {
        System.out.println("Child static initial block");
    }

    {
        System.out.println("Child initial block");
    }
    
    private Hobby hobby = new Hobby();

    public Child() {
        System.out.println("Child constructor block");
    }
}

public class Hobby {
    static{
        System.out.println("Hobby static initial block");
    }

    public Hobby() {
        System.out.println("hobby constructor block");
    }
}
```
​	当执行`new Child()`时，上述代码输出什么？

​	相信有不少同学遇到过这类问题，可能查过资料之后接着就忘了，再次遇到还是答不对。接下来通过4个步骤，拆解一下这段代码的执行顺序，并借此总结规律。


## 1、编译器优化了啥？
下面两段代码对比一下编译前后的变化：

编译前的Child.java

```java
public class Child extends Parent {
    static {
        System.out.println("Child static initial block");
    }
    {
        System.out.println("Child initial block");
    }

    private Hobby hobby = new Hobby();

    public Child() {
        System.out.println("Child constructor block");
    }
}
```
编译后的Child.class
```java
public class Child extends Parent {
    private Hobby hobby;

    public Child() {
        System.out.println("Child initial block");
        this.hobby = new Hobby();
        System.out.println("Child constructor block");
    }

    static {
        System.out.println("Child static initial block");
    }
}
```
​	通过对比可以看到，编译器把初始化块和实例字段的赋值操作，移动到了构造函数代码之前，并且保留了相关代码的先后顺序。事实上，如果构造函数有多个，初始化代码也会被复制多份移动过去。

​	

​	据此可以得出第一条优先级顺序：

- 初始化代码 > 构造函数代码



## 2、`static` 有啥作用？
类的加载过程可粗略分为三个阶段：加载 -> 链接 -> 初始化

初始化阶段可被8种情况触发：

~~~ tex
1. 使用 `new` 关键字实例化对象的时候
2. 读取或设置一个类型的静态字段（常量除外）
3. 调用一个类型的静态方法
4. 使用反射调用类的时候
5. 当初始化类的时候，如果发现父类还没有进行过初始化，则先触发其父类初始化
6. 虚拟机启动时，会先初始化主类（包含`main()`方法的那个类）
7. 当初次调用 MethodHandle 实例时，初始化该 MethodHandle 指向的方法所在的类。
8. 如果接口中定义了默认方法(`default` 修饰的接口方法)，该接口的实现类发生了初始化，则该接口要在其之前被初始化
~~~

​	其中的2,3条目是被`static`代码触发的。

​	其实初始化阶段就是执行类构造器`<clinit>` 方法的过程，这个方法是编译器自动生成的，里面收集了`static`修饰的所有类变量的赋值动作和静态语句块（`static{}` 块），并且保留这些代码出现的先后顺序。

​	根据条目5，JVM 会保证在子类的`<clinit>`方法执行前，父类的`<clinit>`方法已经执行完毕。



:::success
	小结一下：访问类变量或静态方法，会触发类的初始化，而类的初始化就是执行`<clinit>`，也就是执行 `static` 修饰的赋值动作和`static{}`块，并且 JVM 保证先执行父类初始化，再执行子类初始化。
:::



由此得出第二条优先级顺序：

- 父类的`static`代码 > 子类的`static`代码



## 3、static 代码只执行一次
都知道`static`代码(静态方法除外)只执行一次。

有没有想过，这个机制是如何保证的呢？

答案是：双亲委派模型。



JDK8 及之前的双亲委派模型是：

​	应用程序类加载器 → 扩展类加载器 → 启动类加载器

平时开发中写的类，默认都是由 应用程序类加载器加载，它会委派给其父类：扩展类加载器。而扩展类加载器又会委派给其父类：启动类加载器。只有当父类加载器反馈无法完成这个加载请求时，子加载器才会尝试自己去完成加载，这个过程就是双亲委派。三者的父子关系并不是通过继承，而是通过组合模式实现的。



该过程的实现也很简单，下面展示关键实现代码：

```java
protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException
{
    // 首先检查该类是否被加载过
    // 如果加载过，直接返回该类
    Class<?> c = findLoadedClass(name);
    if (c == null) {
        try {
            if (parent != null) {
                c = parent.loadClass(name, false);
            } else {
                c = findBootstrapClassOrNull(name);
            }
        } catch (ClassNotFoundException e) {
            // 如果父类抛出ClassNotFoundException
            // 说明父类无法完成加载请求
        }

        if (c == null) {
            // 如果父类无法加载，转由子类加载
            c = findClass(name);
        }
    }
    if (resolve) {
        resolveClass(c);
    }
    return c;
}
```
结合注释相信大家很容易看懂。

由双亲委派的代码可知，同一个类加载器下，一个类只能被加载一次，也就限定了它只能被初始化一次。所以类中的 `static`代码(静态方法除外)只在类初始化时执行一次



## 4、`<init>`和`<clinit>`
​	前面已经介绍了编译器自动生成的类构造器：`<clinit>`方法，它会收集`static`修饰的所有类变量的赋值动作和静态语句块（`static{}` 块）并保留代码的出现顺序，它会在类初始化时执行



​	相应的，编译器还会生成一个`<init>`方法，它会收集实例字段的赋值动作、初始化语句块({}块)和构造器(Constructor)中的代码，并保留代码的出现顺序，它会在 `new` 指令之后接着执行



​	所以，当`new`一个类时，如果JVM未加载该类，则先对其进行初始化，再进行实例化。



​	至此，第三条优先级规则也就呼之欲出了：

- 静态代码(static{}块、静态字段赋值语句) > 初始化代码({}块、实例字段赋值语句)

## 5、规律实践
​	将前文的三条规则合并，总结出如下两条：

1. 静态代码(`static{}`块、静态字段赋值语句) > 初始化代码({}块、实例字段赋值语句) > 构造函数代码
2. 父类的`static`代码 > 子类的`static`代码



​	根据前文总结，初始化代码和构造函数代码被编译器收集到了`<init>`中，静态代码被收集到了`<clinit>`中，所以再次对上述规律做合并：

​					父类`<clinit>`  >  子类`<clinit> ` >  父类 `<init>`  >  子类 `<init>`



对应到开篇的问题，来实践一下：

​	当执行`new Child()`时，`new`关键字触发了 Child 类的初始化 ，JVM 发现其有父类，则先初始化 Parent 类，开始执行Parent类的`<clinit>`方法，然后执行 Child 类的`<clinit>`方法(还记得`<clinit>`里面收集了什么吗？)。

​	然后开始实例化 一个Child类的对象，此时准备执行 Child 的`<init>`方法，发现它有父类，优先执行父类的`<init>`方法，然后再执行子类的`<init>`(还记得`<init>`里面收集了什么吗？)。



​	相信看到这里，各位心中已经对开篇的问题有答案了，不妨先手写一下输出顺序，然后写代码亲自验证一下。



## 结束语
​	平时开发中经常用到`static`，每次写的时候，心里总会打两个问号，为什么要用`static`?不用行不行？

可以看出，`static`的应用远远不止类变量，静态方法那么简单。
