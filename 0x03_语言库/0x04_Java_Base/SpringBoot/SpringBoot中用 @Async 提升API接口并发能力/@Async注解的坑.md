Java SpringBoot<br />在实际的项目中，对于一些用时比较长的代码片段或者函数，可以采用异步的方式来执行，这样就不会影响整体的流程了。比如在一个用户请求中需要上传一些文件，但是上传文件的耗时会相对来说比较长，这个时候如果上传文件的成功与否不影响主流程的话，就可以把上传文件的操作异步化，在SpringBoot中比较常见的方式就是把要异步执行的代码片段封装成一个函数，然后在函数头使用`@Async`注解，就可以实现代码的异步执行（当然首先得在启动类上加上`@EnableAsync`注解了）。<br />具体的使用方式就不再演示了，网上教大家使用@Async的很多。在实际开发过程中遇到的一个坑分享一下，<br />那么这个坑是什么呢？就是如果在同一个类里面调用一个自己的被`@Async`修饰的函数时，这个函数将不会被异步执行，它依然是同步执行的！所以如果没有经过测试就想当然的以为只要在方法头加上`@Async`就能达到异步的效果，那么很有可能会得到相反的效果。这个是很要命的。<br />首先先看一个正确使用的方式，建一个SpringBoot项目，如果是用Intellij IDEA新建的项目，记得勾上web的依赖。<br />项目建好后，在启动类上加上`@EnableAsync`注解：
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableAsync;

@SpringBootApplication
@EnableAsync
public class AsyncdemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(AsyncdemoApplication.class, args);
    }

}
```
然后再新建一个类Task，用来放三个异步任务doTaskOne、doTaskTwo、doTaskThree：
```java
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

import java.util.Random;

/**
 * @author https://www.chuckfang.top
 * @date Created on 2019/11/12 11:34
 */
@Component
public class Task {

    public static Random random = new Random();

    @Async
    public void doTaskOne() throws Exception {
        System.out.println("开始做任务一");
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        System.out.println("完成任务一，耗时：" + (end - start) + "毫秒");
    }

    @Async
    public void doTaskTwo() throws Exception {
        System.out.println("开始做任务二");
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        System.out.println("完成任务二，耗时：" + (end - start) + "毫秒");
    }

    @Async
    public void doTaskThree() throws Exception {
        System.out.println("开始做任务三");
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        System.out.println("完成任务三，耗时：" + (end - start) + "毫秒");
    }
}
```
在单元测试类上注入Task，在测试用例上测试这三个方法的执行过程：
```java
@SpringBootTest
class AsyncdemoApplicationTests {

    public static Random random = new Random();

    @Autowired
    Task task;

    @Test
    void contextLoads() throws Exception {
        task.doTaskOne();
        task.doTaskTwo();
        task.doTaskThree();
        Thread.sleep(10000);
    }
}
```
为了让这三个方法执行完，需要再单元测试用例上的最后一行加上一个延时，不然等函数退出了，异步任务还没执行完。<br />启动看看效果：
```
开始做任务三
开始做任务二
开始做任务一
完成任务一，耗时：4922毫秒
完成任务三，耗时：6778毫秒
完成任务二，耗时：6960毫秒
```
可以看到三个任务确实是异步执行的，那再看看错误的使用方法。<br />可以在测试类里面把这三个函数再写一遍，并在测试用例上调用测试类自己的方法：
```java
@SpringBootTest
class AsyncdemoApplicationTests {

    public static Random random = new Random();

    @Test
    void contextLoads() throws Exception {
        doTaskOne();
        doTaskTwo();
        doTaskThree();
        Thread.sleep(10000);
    }

    @Async
    public void doTaskOne() throws Exception {
        System.out.println("开始做任务一");
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        System.out.println("完成任务一，耗时：" + (end - start) + "毫秒");
    }

    @Async
    public void doTaskTwo() throws Exception {
        System.out.println("开始做任务二");
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        System.out.println("完成任务二，耗时：" + (end - start) + "毫秒");
    }

    @Async
    public void doTaskThree() throws Exception {
        System.out.println("开始做任务三");
        long start = System.currentTimeMillis();
        Thread.sleep(random.nextInt(10000));
        long end = System.currentTimeMillis();
        System.out.println("完成任务三，耗时：" + (end - start) + "毫秒");
    }
}
```
再看看效果
```
开始做任务一
完成任务一，耗时：9284毫秒
开始做任务二
完成任务二，耗时：8783毫秒
开始做任务三
完成任务三，耗时：943毫秒
```
它们竟然是顺序执行的！也就是同步执行，并没有达到异步的效果，这要是在生产上使用，岂不凉凉。<br />这种问题如果不进行测试还是比较难发现的，特别是想要异步执行的代码并不会执行太久，也就是同步执行也察觉不出来，或者说根本发现不了它是不是异步执行。这种错误也很容易犯，特别是把一个类里面的方法提出来想要异步执行的时候，并不会想着新建一个类来放这个方法，而是会在当前类上直接抽取为一个方法，然后在方法头上加上`@Async`注解，以为这样就完事了，其实并没有起到异步的作用！<br />其实`@Async`的这个性质在官网上已经有过说明了，官网：[https://www.baeldung.com/spring-async](https://www.baeldung.com/spring-async)是这样说的：
> First – let’s go over the rules – **@Async** has two limitations:
> - it must be applied to **public** methods only
> - self-invocation – calling the async method from within the same class – won’t work
> 
The reasons are simple – **「the method needs to be *public*」** so that it can be proxied. And **「self-invocation doesn’t work」** because it bypasses the proxy and calls the underlying method directly.

在一开始就提到了@`Async`的两个限制，其中第二个就是调用自己类上的异步方法是不起作用的。下面也讲了原因，就是这种使用方式绕过了代理而直接调用了方法，所以肯定是同步的了。从这里，也知道了另外一个知识点，就是@`Async`注解其实是通过代理的方式来实现异步调用的。
