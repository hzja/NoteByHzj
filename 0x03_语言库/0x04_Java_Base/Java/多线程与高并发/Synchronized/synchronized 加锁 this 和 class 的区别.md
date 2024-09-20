Java synchronized<br />`synchronized` 是 Java 语言中处理并发问题的一种常用手段，它也被称之为“Java 内置锁”，由此可见其地位之高。然而 `synchronized` 却有着多种用法，当它修饰不同对象时，其意义也是不同的，下面一起来看。
<a name="Bb26O"></a>
## `synchronized` 用法
`synchronized` 可以用来修饰普通方法、静态方法和代码块。
<a name="mP8Lx"></a>
### ① 修饰普通方法
```java
/**
 * synchronized 修饰普通方法
 */
public synchronized void method() {
    // .......
}
```
当 `synchronized` 修饰普通方法时，被修饰的方法被称为同步方法，其作用范围是整个方法，作用的对象是调用这个方法的对象。
<a name="vAm4I"></a>
### ② 修饰静态方法
```java
/**
 * synchronized 修饰静态方法
 */
public static synchronized void staticMethod() {
    // .......
}
```
当 `synchronized` 修饰静态的方法时，其作用的范围是整个方法，作用对象是调用这个类的所有对象。
<a name="Vc0df"></a>
### ③ 修饰代码块
为了减少锁的粒度，可以选择在一个方法中的某个部分使用 `synchronized` 来修饰（一段代码块），从而实现对一个方法中的部分代码进行加锁，实现代码如下：
```java
public void classMethod() throws InterruptedException {
    // 前置代码...
    
    // 加锁代码
    synchronized (SynchronizedExample.class) {
        // ......
    }
    
    // 后置代码...
}
```
以上代码在执行时，被修饰的代码块称为同步语句块，其作用范围是大括号“{}”括起来的代码块，作用的对象是调用这个代码块的对象。<br />但以上代码，除了可以加锁 `class` 之外，还可以加锁 `this`，具体示例如下：
```java
public void classMethod() throws InterruptedException {
    // 前置处理代码...
    synchronized (this) {
        // ......
    }
    // 后置处理代码...
}
```
那问题来了，使用 `synchronized` 加锁 `this` 和 `class` 的区别是什么？不都是加锁同一个类吗？<br />答案还真不是，加锁 `this` 和 `class` 区别还是很大的。下面通过以下 4 个示例，来看二者之间的区别。
<a name="VPO8z"></a>
## 1、加锁 `class` 共享一个类实例
首先，创建 5 个线程，调用同一个对象下 `synchronized` 加锁的 `class` 代码，具体示例如下：
```java
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class SynchronizedExample {

    public static void main(String[] args) {
        // 创建当前类实例
        final SynchronizedExample example = new SynchronizedExample();
        // 创建 5 个线程执行任务
        for (int i = 0; i < 5; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // 调用 synchronized 修饰的 class 方法
                        example.classMethod();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }

    /**
     * synchronized 修饰的 class 方法
     * @throws InterruptedException
     */
    public void classMethod() throws InterruptedException {
        synchronized (SynchronizedExample.class) {
            System.out.println(String.format("当前执行线程:%s,执行时间:%s",
                    Thread.currentThread().getName(), new Date()));
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
```
以上程序的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628599326725-5f071acd-dee3-46e8-be29-8d0315af22ee.webp#clientId=u62093557-b30c-4&from=paste&id=u25082f36&originHeight=340&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ub999b0c5-9f7b-4045-89e4-74ee20a59ba)<br />从上述结果可以看出，这 5 个线程共享的是同一把锁。
<a name="c8sHW"></a>
## 2、加锁 `class` 创建多个实例
接下来，创建 5 个线程，调用不同对象下 `synchronized` 加锁的 class 代码，具体示例如下：
```java
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class SynchronizedExample {

    public static void main(String[] args) {
        // 创建 5 个线程执行任务
        for (int i = 0; i < 5; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // 创建类实例
                        SynchronizedExample example = new SynchronizedExample();
                        // 调用 synchronized 修饰的 class 方法
                        example.classMethod();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
    
    /**
     * synchronized 修饰的 class 方法
     * @throws InterruptedException
     */
    public void classMethod() throws InterruptedException {
        synchronized (SynchronizedExample.class) {
            System.out.println(String.format("当前执行线程:%s,执行时间:%s",
                    Thread.currentThread().getName(), new Date()));
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
```
以上程序的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628599326671-26ac8d27-4e74-4723-b6c6-17ee410925dc.webp#clientId=u62093557-b30c-4&from=paste&id=u55f9efdc&originHeight=352&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u706a1681-a3a6-4a95-ab12-53ccd52cb0e)<br />从上述结果可以看出，虽然是不同的对象，但它们使用的仍然是同一把锁。
<a name="JjCRM"></a>
## 3、加锁 `this` 共享一个类实例
接下来，创建 5 个线程，调用 `synchronized` 加锁 this 的示例。首先这 5 个线程调用同一个对象的加锁方法，示例代码如下：
```java
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class SynchronizedExample {

    public static void main(String[] args) {
        // 创建当前类实例
        final SynchronizedExample example = new SynchronizedExample();
        // 创建 5 个线程执行任务
        for (int i = 0; i < 5; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // 调用 synchronized 修饰的 this 方法
                        example.thisMethod();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
    
    /**
     * synchronized 修饰的 this 方法
     * @throws InterruptedException
     */
    public void thisMethod() throws InterruptedException {
        synchronized (this) {
            System.out.println(String.format("当前执行线程:%s,执行时间:%s",
                    Thread.currentThread().getName(), new Date()));
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
```
以上程序的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628599326797-d93fbc99-a3a0-44d0-a902-8d54eff737f0.webp#clientId=u62093557-b30c-4&from=paste&id=u9a0c585c&originHeight=338&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ua0189add-0898-4528-af58-31ba11b6848)<br />从上述结果可以看出，以上线程使用的都是同一把锁。
<a name="ncQVL"></a>
## 4、加锁 `this` 创建多个类实例
最后一个示例最为特殊，使用 `synchronized` 加锁 `this`，让这 5 个线程调用各自创建对象的方法，具体示例如下：
```java
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class SynchronizedExample {

    public static void main(String[] args) {
        // 创建 5 个线程执行任务
        for (int i = 0; i < 5; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // 创建（多个）类实例
                        SynchronizedExample example = new SynchronizedExample();
                        // 调用 synchronized 修饰的 this 方法
                        example.thisMethod();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
    
    /**
     * synchronized 修饰的 this 方法
     * @throws InterruptedException
     */
    public void thisMethod() throws InterruptedException {
        synchronized (this) {
            System.out.println(String.format("当前执行线程:%s,执行时间:%s",
                    Thread.currentThread().getName(), new Date()));
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
```
以上程序的执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628599326796-7d284c89-e3b1-4d09-9eb8-ac5212f286a1.webp#clientId=u62093557-b30c-4&from=paste&id=u1e502178&originHeight=344&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u52760e4e-23db-4af0-9695-0adf3b36fb7)<br />从上述结果可以看出，当使用 `synchronized` 加锁 `this` 时，如果线程调用的不是同一个对象，那么这些线程之间使用的锁都是自己独立的锁，这个结果就和 `synchronized` 加锁 `class` 的结果完全不同了。
<a name="bdeCI"></a>
## 总结
通过以上 4 个示例可以得出结论，当**使用 **`**synchronized**`** 加锁 **`**class**`** 时，无论共享一个对象还是创建多个对象，它们用的都是同一把锁**，而**使用 **`**synchronized**`** 加锁 **`**this**`** 时，只有同一个对象会使用同一把锁，不同对象之间的锁是不同的**。
