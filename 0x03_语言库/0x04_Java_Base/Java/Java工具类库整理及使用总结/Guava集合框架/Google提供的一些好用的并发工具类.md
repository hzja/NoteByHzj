Java<br />关于并发方面的，juc已经提供了很多好用的工具，而谷歌在此基础上做了扩展，使并发编程更容易，这些工具放在Guava jar包中。<br />这里演示几个简单的案例，见一下Guava的效果。
<a name="z29Q6"></a>
## Guava Maven配置
```xml
<dependency>
  <groupId>com.google.guava</groupId>
  <artifactId>guava</artifactId>
  <version>27.0-jre</version>
</dependency>
```
<a name="KNhEo"></a>
## Guava中常用几个类
`**MoreExecutors**`：提供了一些静态方法，是对juc中的`Executors`类的一个扩展。<br />`**Futures**`：也提供了很多静态方法，是对juc中`Future`的一个扩展。
<a name="uhrKi"></a>
## 案例1：异步执行任务完毕之后回调
```java
package com.example.chat34;

import com.google.common.util.concurrent.ListenableFuture;
import com.google.common.util.concurrent.ListeningExecutorService;
import com.google.common.util.concurrent.MoreExecutors;
import lombok.extern.slf4j.Slf4j;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

@Slf4j
public class Demo1 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        //创建一个线程池
        ExecutorService delegate = Executors.newFixedThreadPool(5);
        try {
            ListeningExecutorService executorService = MoreExecutors.listeningDecorator(delegate);
            //异步执行一个任务
            ListenableFuture<Integer> submit = executorService.submit(() -> {
                log.info("{}", System.currentTimeMillis());
                //休眠2秒，默认耗时
                TimeUnit.SECONDS.sleep(2);
                log.info("{}", System.currentTimeMillis());
                return 10;
            });
            //当任务执行完毕之后回调对应的方法
            submit.addListener(() -> {
                log.info("任务执行完毕了，我被回调了");
            }, MoreExecutors.directExecutor());
            log.info("{}", submit.get());
        } finally {
            delegate.shutdown();
        }
    }
}
```
输出：
```
14:25:50.055 [pool-1-thread-1] INFO com.example.chat34.Demo1 - 1567491950047
14:25:52.063 [pool-1-thread-1] INFO com.example.chat34.Demo1 - 1567491952063
14:25:52.064 [pool-1-thread-1] INFO com.example.chat34.Demo1 - 任务执行完毕了，我被回调了
14:25:52.064 [main] INFO com.example.chat34.Demo1 - 10
```
**说明：**<br />`ListeningExecutorService`接口继承于juc中的`ExecutorService`接口，对`ExecutorService`做了一些扩展，看其名字中带有Listening，说明这个接口自带监听的功能，可以监听异步执行任务的结果。通过`MoreExecutors.listeningDecorator`创建一个`ListeningExecutorService`对象，需传递一个`ExecutorService`参数，传递的`ExecutorService`负责异步执行任务。<br />`ListeningExecutorService`的`submit`方法用来异步执行一个任务，返回`ListenableFuture`，`ListenableFuture`接口继承于juc中的`Future`接口，对`Future`做了扩展，使其带有监听的功能。调用`submit.addListener`可以在执行的任务上添加监听器，当任务执行完毕之后会回调这个监听器中的方法。<br />`ListenableFuture`的`get`方法会阻塞当前线程直到任务执行完毕。<br />上面的还有一种写法，如下：
```java
package com.example.chat34;

import com.google.common.util.concurrent.*;
import lombok.extern.slf4j.Slf4j;
import org.checkerframework.checker.nullness.qual.Nullable;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

@Slf4j
public class Demo2 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService delegate = Executors.newFixedThreadPool(5);
        try {
            ListeningExecutorService executorService = MoreExecutors.listeningDecorator(delegate);
            ListenableFuture<Integer> submit = executorService.submit(() -> {
                log.info("{}", System.currentTimeMillis());
                TimeUnit.SECONDS.sleep(4);
                //int i = 10 / 0;
                log.info("{}", System.currentTimeMillis());
                return 10;
            });
            Futures.addCallback(submit, new FutureCallback<Integer>() {
                @Override
                public void onSuccess(@Nullable Integer result) {
                    log.info("执行成功:{}", result);
                }

                @Override
                public void onFailure(Throwable t) {
                    try {
                        TimeUnit.MILLISECONDS.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    log.error("执行任务发生异常:" + t.getMessage(), t);
                }
            }, MoreExecutors.directExecutor());
            log.info("{}", submit.get());
        } finally {
            delegate.shutdown();
        }
    }
}
```
输出：
```
14:26:07.938 [pool-1-thread-1] INFO com.example.chat34.Demo2 - 1567491967936
14:26:11.944 [pool-1-thread-1] INFO com.example.chat34.Demo2 - 1567491971944
14:26:11.945 [main] INFO com.example.chat34.Demo2 - 10
14:26:11.945 [pool-1-thread-1] INFO com.example.chat34.Demo2 - 执行成功:10
```
上面通过调用`Futures`的静态方法`addCallback`在异步执行的任务中添加回调，回调的对象是一个`FutureCallback`，此对象有2个方法，任务执行成功调用`onSuccess`，执行失败调用`onFailure`。<br />失败的情况可以将代码中`int i = 10 / 0;`注释去掉，执行一下可以看看效果。
<a name="Uiip8"></a>
## 示例2：获取一批异步任务的执行结果
```java
package com.example.chat34;

import com.google.common.util.concurrent.Futures;
import com.google.common.util.concurrent.ListenableFuture;
import com.google.common.util.concurrent.ListeningExecutorService;
import com.google.common.util.concurrent.MoreExecutors;
import lombok.extern.slf4j.Slf4j;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.stream.Collectors;

@Slf4j
public class Demo3 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        log.info("star");
        ExecutorService delegate = Executors.newFixedThreadPool(5);
        try {
            ListeningExecutorService executorService = MoreExecutors.listeningDecorator(delegate);
            List<ListenableFuture<Integer>> futureList = new ArrayList<>();
            for (int i = 5; i >= 0; i--) {
                int j = i;
                futureList.add(executorService.submit(() -> {
                    TimeUnit.SECONDS.sleep(j);
                    return j;
                }));
            }
            //获取一批任务的执行结果
            List<Integer> resultList = Futures.allAsList(futureList).get();
            //输出
            resultList.forEach(item -> {
                log.info("{}", item);
            });
        } finally {
            delegate.shutdown();
        }
    }
}
```
输出：
```
14:26:35.970 [main] INFO com.example.chat34.Demo3 - star
14:26:41.137 [main] INFO com.example.chat34.Demo3 - 5
14:26:41.138 [main] INFO com.example.chat34.Demo3 - 4
14:26:41.138 [main] INFO com.example.chat34.Demo3 - 3
14:26:41.138 [main] INFO com.example.chat34.Demo3 - 2
14:26:41.138 [main] INFO com.example.chat34.Demo3 - 1
14:26:41.138 [main] INFO com.example.chat34.Demo3 - 0
```
结果中按顺序输出了6个异步任务的结果，此处用到了`Futures.allAsList`方法，看一下此方法的声明：
```java
public static <V> ListenableFuture<List<V>> allAsList(
    Iterable<? extends ListenableFuture<? extends V>> futures)
```
传递一批`ListenableFuture`，返回一个`ListenableFuture<List<V>>`，内部将一批结果转换为了一个`ListenableFuture`对象。
<a name="d0dVg"></a>
## 示例3：一批任务异步执行完毕之后回调
异步执行一批任务，最后计算其和
```java
package com.example.chat34;

import com.google.common.util.concurrent.*;
import lombok.extern.slf4j.Slf4j;
import org.checkerframework.checker.nullness.qual.Nullable;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

@Slf4j
public class Demo4 {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        log.info("star");
        ExecutorService delegate = Executors.newFixedThreadPool(5);
        try {
            ListeningExecutorService executorService = MoreExecutors.listeningDecorator(delegate);
            List<ListenableFuture<Integer>> futureList = new ArrayList<>();
            for (int i = 5; i >= 0; i--) {
                int j = i;
                futureList.add(executorService.submit(() -> {
                    TimeUnit.SECONDS.sleep(j);
                    return j;
                }));
            }
            ListenableFuture<List<Integer>> listListenableFuture = Futures.allAsList(futureList);
            Futures.addCallback(listListenableFuture, new FutureCallback<List<Integer>>() {
                @Override
                public void onSuccess(@Nullable List<Integer> result) {
                    log.info("result中所有结果之和：" + result.stream().reduce(Integer::sum).get());
                }

                @Override
                public void onFailure(Throwable t) {
                    log.error("执行任务发生异常:" + t.getMessage(), t);
                }
            }, MoreExecutors.directExecutor());
        } finally {
            delegate.shutdown();
        }
    }
}
```
输出：
```
14:47:04.819 [main] INFO com.example.chat34.Demo4 - star
14:47:09.933 [pool-1-thread-1] INFO com.example.chat34.Demo4 - result中所有结果之和：15
```
代码中异步执行了一批任务，所有任务完成之后，回调了上面的`onSuccess`方法，内部对所有的结果进行sum操作。
<a name="eSk31"></a>
## 总结
通过Guava提供的一些工具类，方便异步执行任务并进行回调。

