JVM<br />方法调用并不等同于方法中的代码被执行，方法调用阶段唯一的任务就是确定被调用方法的版本（即调用哪一个方法），暂时还未涉及方法内部的具体运行过程。<br />一切方法调用在Class文件里面存储的都只是符号引用，而不是方法在实际运行时内存布局中的入口地址（也就是之前说的直接引用）。
<a name="eawIv"></a>
## 解析
所有方法调用的目标方法在Class文件里面都是一个常量池中的符号引用，在类加载的解析阶段，会将其中的一部分符号引用转化为直接引用，这种解析能够成立的前提是：方法在程序真正运行之前就有一个可确定的调用版本，并且这个方法的调用版本在运行期是不可改变的。<br />换句话说，调用目标在程序代码写好、编译器进行编译那一刻就已经确定下来。这类方法的调用被称为解析（Resolution），在Java语言中符合这种要求的主要有静态方法和私有方法。
<a name="StcgK"></a>
### 方法调用指令

- `invokestatic`：用于调用静态方法。
- `invokespecial`：用于调用实例构造器`<init>()`方法、私有方法和父类中的方法。
- `invokevirtual`：用于调用所有的虚方法。
- `invokeinterface`：用于调用接口方法，会在运行时再确定一个实现该接口的对象。
- `invokedynamic`：先在运行时动态解析出调用点限定符所引用的方法，然后再执行该方法。

前面4条调用指令，分派逻辑都固化在Java虚拟机内部，而`invokedynamic`指令的分派逻辑是由用户设定的引导方法来决定的。
<a name="sGLSc"></a>
### 方法分类
在java语言中方法主要分为“虚方法”和“非虚方法”。

- 非虚方法：在类加载的时候就可以把符号引用解析为该方法的直接引用。比如：静态方法、私有方法、实例构造器、父类方法和被final修饰的方法。
- 虚方法：需要在运行时才能将符号引用转换成直接引用，如：分派。
<a name="hoSVu"></a>
## 分派
分派（Dispatch）它可能是静态的也可能是动态的，按照分派依据的宗量数可分为单分派和多分派。这两类分派方式两两组合就构成了静态单分派、静态多分派、动态单分派、动态多分派4种分派组合情况。
<a name="b1fzW"></a>
### 静态分派
依赖静态类型来决定方法执行版本的分派动作，都称为静态分派。静态分派的最典型应用表现就是方法重载，虚拟机（或者准确地说是编译器）在重载时是通过参数的静态类型来作为判定依据的。
```java
public class StaticDispatch {

    static abstract class Human {
    }

    static class Man extends Human {
    }

    static class Woman extends Human {
    }

    public void sayHello(Human guy) {
        System.out.println("hello,guy!");
    }

    public void sayHello(Man guy) {
        System.out.println("hello,gentleman!");
    }

    public void sayHello(Woman guy) {
        System.out.println("hello,lady!");
    }

    public static void main(String[] args) {
        Human man = new Man();
        Human woman = new Woman();
        StaticDispatch sr = new StaticDispatch();
        sr.sayHello(man);
        sr.sayHello(woman);
    }
}
```
运行结果：
```java
hello,guy!
hello,guy!
```
```java
Human man = new Man();
```
这里的Human就是变量的“静态类型”（Static Type），或者叫“外观类型”（Apparent Type）；Man就是变量的“实际类型”（Actual Type）或者叫“运行时类型”（Runtime Type）。
<a name="UiXD4"></a>
### 动态分派
把在运行期根据实际类型确定方法执行版本的分派过程称为动态分派。最典型的表现就是重写。
```java
public class DynamicDispatch {

    static abstract class Human {
        abstract void sayHello();
    }

    static class Man extends Human {
        public void sayHello() {
            System.out.println("hello,Man!");
        }
    }

    static class Woman extends Human {
        public void sayHello() {
            System.out.println("hello,Woman!");
        }
    }

    public static void main(String[] args) {
        Human man = new Man();
        Human woman = new Woman();
        man.sayHello();
        woman.sayHello();
    }
}
```
运行结果：
```java
hello,Man!
hello,Woman!
```
通过javap命令看下main方法的字节码：
```java
...
  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=2, locals=3, args_size=1
         0: new           #2                  // class com/xiaolyuh/DynamicDispatch$Man
         3: dup
         4: invokespecial #3                  // Method com/xiaolyuh/DynamicDispatch$Man."<init>":()V
         7: astore_1
         8: new           #4                  // class com/xiaolyuh/DynamicDispatch$Woman
        11: dup
        12: invokespecial #5                  // Method com/xiaolyuh/DynamicDispatch$Woman."<init>":()V
        15: astore_2
        16: aload_1
        17: invokevirtual #6                  // Method com/xiaolyuh/DynamicDispatch$Human.sayHello:()V
        20: aload_2
        21: invokevirtual #6                  // Method com/xiaolyuh/DynamicDispatch$Human.sayHello:()V
        24: return
      LineNumberTable:
        line 27: 0
        line 28: 8
        line 29: 16
        line 30: 20
        line 31: 24
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      25     0  args   [Ljava/lang/String;
            8      17     1   man   Lcom/xiaolyuh/DynamicDispatch$Human;
           16       9     2 woman   Lcom/xiaolyuh/DynamicDispatch$Human;
}
...
```
通过字节码可以发现：在main方法中，`sayHello()`方法的调用对应的符号引用是一样的，`com/xiaolyuh/DynamicDispatch$Human.sayHello:()V`。<br />在这里可以得出一个结论：在动态分派的情况下，在编译时期我们是无法确定方法的直接引用的，那么它是怎么实现重载方法的调用的呢？问题关键是在`invokevirtual`指令上，在执行`invokevirtual`指令时，`invokevirtual`指令会去确定方法的调用版本。
<a name="aRzQP"></a>
#### `invokevirtual`指令的运行过程

1. 找到操作数栈顶的第一个元素所指向的对象的实际类型，记作C。
2. 如果在类型C中找到与常量中的描述符和简单名称都相符的方法，则进行访问权限校验，如果通过则返回这个方法的直接引用，查找过程结束；不通过则返回java.lang.IllegalAccessError异常。
3. 否则，按照继承关系从下往上依次对C的各个父类进行第二步的搜索和验证过程。4. 如果始终没有找到合适的方法，则抛出java.lang.AbstractMethodError异常。

正是因为invokevirtual指令执行的第一步就是在运行期确定接收者的实际类型，所以两次调用中的invokevirtual指令并不是把常量池中方法的符号引用解析到直接引用上就结束了，还会根据方法接收者的实际类型来选择方法版本，这个过程就是Java语言中方法重写的本质。<br />当子类声明了与父类同名的字段时，虽然在子类的内存中两个字段都会存在，但是子类的字段会遮蔽父类的同名字段
<a name="LTESG"></a>
#### 动态分派的实现
因为动态方法执行非常频繁，并且动态分派的方法版本选择需要在运行时，在实际接受者类型的方法元数据中搜索合适的目标方法，因此，Java虚拟机实现基于执行性能的考虑，虚拟机会为类型在方法区中建立一个虚方法表（Virtual Method Table，也称为vtable，与此对应的，在`invokeinterface`执行时也会用到接口方法表——Interface Method Table，简称itable），使用虚方法表索引来代替元数据查找以提高性能。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657507868126-df07cf5f-84f2-461c-8788-1cd2516a311b.jpeg)<br />虚方法表中存放着各个方法的实际入口地址。如果某个方法在子类中没有被重写，那子类的虚方法表中的地址入口和父类相同方法的地址入口是一致的，都指向父类的实现入口。如果子类中重写了这个方法，子类虚方法表中的地址也会被替换为指向子类实现版本的入口地址。<br />在图中，Son重写了来自Father的全部方法，因此Son的方法表没有指向Father类型数据的箭头。但是Son和Father都没有重写来自Object的方法，所以它们的方法表中所有从Object继承来的方法都指向了Object的数据类型。<br />虚方法表一般在类加载的连接阶段进行初始化，准备了类的变量初始值后，虚拟机会把该类的虚方法表也一同初始化完毕。
<a name="tS1Yx"></a>
### 单分派与多分派
方法的接收者与方法的参数统称为方法的宗量。分派基于多少种宗量，可以将分派划分为单分派和多分派两种。单分派是根据一个宗量对目标方法进行选择，多分派则是根据两个及以上的宗量对目标方法进行选择。

- 静态分派需要根据静态类型和方法参数两个宗量来确定方法调用，所以属于多分派。
- 动态分派只需要根据实际类型一个宗量来确定方法的调用，所以属于单分派。

在动态分派的过程中，方法签名是确定的，所以方法参数就不会变，方法调用就取决于参数的实际类型。
<a name="v93vd"></a>
## 总结
解析调用一定是个静态的过程，在编译期间就完全确定，在类加载的解析阶段就会把涉及的符号引用全部转变为明确的直接引用，不必延迟到运行期再去完成。分派（Dispatch）调用则要复杂许多，它可能是静态的也可能是动态的，按照分派依据的宗量数可分为单分派和多分派。<br />这两类分派方式两两组合就构成了静态单分派、静态多分派、动态单分派、动态多分派4种分派组合情况。
