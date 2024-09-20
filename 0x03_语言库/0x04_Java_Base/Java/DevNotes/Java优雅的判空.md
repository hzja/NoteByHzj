Java 判空
<a name="5b82c256"></a>
### 判空灾难
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603074571675-312d9334-13f3-4285-892f-fc6f227447be.webp#align=left&display=inline&height=232&originHeight=232&originWidth=1080&size=0&status=done&style=none&width=1080)<br />为了避免NullPointerException，经常会进行如下操作。
```java
if (data != null) {  
    do sth.  
}
```
如果一个类中多次使用某个对象，那可能要进行多次判空:<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603074571643-1c06aa1c-36ab-42fb-92a7-298a9a32c4ff.webp#align=left&display=inline&height=704&originHeight=704&originWidth=307&size=0&status=done&style=none&width=307)
<a name="IqJVW"></a>
### NullObject模式
对于项目中无数次的判空，对代码质量整洁度产生了十分之恶劣的影响，对于这种现象，称之为“判空灾难”。<br />那么，这种现象如何治理呢，先简单介绍一下NullObject模式。
<a name="CoA2U"></a>
#### 什么是NullObject模式
> In object-oriented computer programming, a null object is an object with no referenced value or with defined neutral ("null") behavior. The null object design pattern describes the uses of such objects and their behavior (or lack thereof).

以上解析来自Wikipedia。<br />NullObject模式首次发表在“ 程序设计模式语言 ”系列丛书中。一般的，在面向对象语言中，对对象的调用前需要使用判空检查，来判断这些对象是否为空，因为在空引用上无法调用所需方法。<br />空对象模式的一种典型实现方式如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603074571677-f98fc225-389c-4442-8508-a14362ddd4f8.webp#align=left&display=inline&height=151&originHeight=151&originWidth=332&size=0&status=done&style=shadow&width=332)<br />示例代码如下：<br />Nullable是空对象的相关操作接口，用于确定对象是否为空，因为在空对象模式中，对象为空会被包装成一个Object，成为Null Object，该对象会对原有对象的所有方法进行空实现…
```java
public interface Nullable {  
    boolean isNull();  
}
```
这个接口定义了业务对象的行为。
```java
public interface DependencyBase extends Nullable {  
    void Operation();  
}
```
这是该对象的真实类，实现了业务行为接口DependencyBase与空对象操作接口Nullable。
```java
public class Dependency implements DependencyBase, Nullable {  
    @Override  
    public void Operation() {  
        System.out.print("Test!");  
    }  
    @Override  
    public boolean isNull() {  
        return false;  
    }  
}
```
这是空对象，对原有对象的行为进行了空实现。
```java
public class NullObject implements DependencyBase{  
    @Override  
    public void Operation() {  
        // do nothing  
    }  
    @Override  
    public boolean isNull() {  
        return true;  
    }  
}
```
在使用时，可以通过工厂调用方式来进行空对象的调用，也可以通过其他如反射的方式对对象进行调用（一般多耗时几毫秒）在此不进行详细叙述。
```java
public class Factory {  
    public static DependencyBase get(Nullable dependencyBase){  
        if (dependencyBase == null){  
            return new NullObject();  
        }  
        return new Dependency();  
    }  
}
```
这是一个使用范例，通过这种模式，不再需要进行对象的判空操作，而是可以直接使用对象，也不必担心NPE（NullPointerException）的问题。
```java
public class Client {  
    public void test(DependencyBase dependencyBase){  
        Factory.get(dependencyBase).Operation();  
    }  
}
```
关于空对象模式，更具体的内容可以多找一找资料，上述只是对NullObject的简单介绍，下面要推荐的是一款协助判空的插件NR Null Object，用来优雅地进行判空，不再进行定义繁琐的空对象接口与空独享实现类。
<a name="ruB2g"></a>
### NR Null Object（插件）
NR Null Object是一款适用于Android Studio、IntelliJ IDEA、PhpStorm、WebStorm、PyCharm、RubyMine、AppCode、CLion、GoLand、DataGrip等IDEA的Intellij插件。其可以根据现有对象，便捷快速生成其空对象模式需要的组成成分，其包含功能如下：

1. 分析所选类可声明为接口的方法；<br />
2. 抽象出公有接口；<br />
3. 创建空对象，自动实现公有接口；<br />
4. 对部分函数进行可为空声明；<br />
5. 可追加函数进行再次生成；<br />
6. 自动的函数命名规范<br />

只需要在原有需要进行多次判空的对象中，右击弹出菜单，选择Generate，并选择NR Null Object即可自动生成相应的空对象组件。
<a name="LeA8M"></a>
#### 安装方式
可以直接通过IDEA的Preferences中的Plugins仓库进行安装。
> 选择 Preferences → Plugins → Browse repositories

> 搜索“NR Null Oject”或者“Null Oject”进行模糊查询，点击右侧的Install，restart IDEA即可。

<a name="EuoLU"></a>
### Optional
还有一种方式是使用Java8特性中的Optional来进行优雅地判空，Optional来自官方的介绍如下：
> A container object which may or may not contain a non-null value. If a value is present, isPresent() will return true and get() will return the value.

一个可能包含也可能不包含非null值的容器对象。如果存在值， `isPresent()` 将返回 true，`get()` 将返回该值。<br />话不多说，举个例子。<br />有如下代码，需要获得Test2中的Info信息，但是参数为Test4，要一层层的申请，每一层都获得的对象都可能是空，最后的代码看起来就像这样。
```java
public String testSimple(Test4 test) {  
        if (test == null) {  
            return "";  
        }  
        if (test.getTest3() == null) {  
            return "";  
        }  
        if (test.getTest3().getTest2() == null) {  
            return "";  
        }  
        if (test.getTest3().getTest2().getInfo() == null) {  
            return "";  
        }  
        return test.getTest3().getTest2().getInfo();  
    }
```
但是使用Optional后，整个就都不一样了。
```java
public String testOptional(Test test) {  
    return Optional.ofNullable(test).flatMap(Test::getTest3)  
        .flatMap(Test3::getTest2)  
        .map(Test2::getInfo)  
        .orElse("");  
}
```
1、Optional.ofNullable(test)，如果test为空，则返回一个单例空Optional对象，如果非空则返回一个Optional包装对象，Optional将test包装；
```java
public static <T> Optional<T> ofNullable(T value) {  
    return value == null ? empty() : of(value);  
}
```
2、flatMap(Test::getTest3)判断test是否为空，如果为空，继续返回第一步中的单例Optional对象，否则调用Test的getTest3方法；
```java
public<U> Optional<U> flatMap(Function<? super T, Optional<U>> mapper) {  
    Objects.requireNonNull(mapper);  
    if (!isPresent())  
        return empty();  
    else {  
        return Objects.requireNonNull(mapper.apply(value));  
    }  
}
```
3、flatMap(Test3::getTest2)同上调用Test3的getTest2方法；<br />4、map(Test2::getInfo)同flatMap类似，但是flatMap要求Test3::getTest2返回值为Optional类型，而map不需要，flatMap不会多层包装，map返回会再次包装Optional；
```java
public<U> Optional<U> map(Function<? super T, ? extends U> mapper) {  
    Objects.requireNonNull(mapper);  
    if (!isPresent())  
        return empty();  
    else {  
        return Optional.ofNullable(mapper.apply(value));  
    }  
}
```
5、orElse("");获得map中的value，不为空则直接返回value，为空则返回传入的参数作为默认值。
```java
public T orElse(T other) {  
    return value != null ? value : other;  
}
```
使用Optional后代码瞬间变得非常整洁，或许看到这段代码会有很多疑问，针对复杂的一长串判空，Optional有它的优势，但是对于简单的判空使用Optional也会增加代码的阅读成本、编码量以及团队新成员的学习成本。毕竟Optional在现在还并没有像RxJava那样流行，它还拥有一定的局限性。<br />如果直接使用Java8中的Optional，需要保证安卓API级别在24及以上。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603074571719-18932d52-8a9d-4060-ac9c-59b584102162.webp#align=left&display=inline&height=145&originHeight=145&originWidth=679&size=0&status=done&style=none&width=679)<br />也可以直接引入Google的Guava。（Guava官方的提示）
> Guava is a set of core libraries that includes new collection types (such as multimap and multiset), immutable collections, a graph library, functional types, an in-memory cache, and APIs/utilities for concurrency, I/O, hashing, primitives, reflection, string processing, and much more!

引用方式，就像这样：
```groovy
dependencies {  
    compile 'com.google.guava:guava:27.0-jre'  
    // or, for Android:  
    api 'com.google.guava:guava:27.0-android'  
}
```
不过IDEA默认会显示黄色，提示将Guava表达式迁移到Java Api上。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603074571984-7ce32212-7ec6-4bd0-920e-1cfa346bb723.webp#align=left&display=inline&height=110&originHeight=110&originWidth=617&size=0&status=done&style=none&width=617)<br />当然，也可以通过在Preferences搜索"Guava"来Kill掉这个Yellow的提示。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603074571709-1b5f4ce6-1dc0-4d7c-813d-910b1a32b5f0.webp#align=left&display=inline&height=450&originHeight=450&originWidth=800&size=0&status=done&style=none&width=800)<br />关于Optional使用还有很多技巧，感兴趣可以查阅Guava和Java8相关书籍和文档。
<a name="RKAF3"></a>
#### 使用Optional具有如下优点

1. 将防御式编程代码完美包装<br />
2. 链式调用<br />
3. 有效避免程序代码中的空指针<br />
<a name="iHVUh"></a>
#### 但是也同样具有一些缺点：

1. 流行性不是非常理想，团队新成员需要学习成本<br />
2. 安卓中需要引入Guava，需要团队每个人处理IDEA默认提示，或者忍受黄色提示<br />
<a name="Hsuke"></a>
### Kotlin
当然，Kotlin以具有优秀的空安全性为一大特色，并可以与Java很好的混合使用，like this:
```kotlin
test1?.test2?.test3?.test4
```
如果已经开始使用了Kotlin，可以不用再写缭乱的防御判空语句。如果还没有使用Kotlin，并不推荐为了判空优雅而直接转向Kotlin。
