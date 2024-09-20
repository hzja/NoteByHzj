Java SimpleDateFormat
<a name="qQzDT"></a>
## 1、什么是线程不安全？
线程不安全也叫非线程安全，是指多线程执行中，程序的执行结果和预期的结果不符的情况就叫着线程不安全。
<a name="c362X"></a>
### 线程不安全的代码
`SimpleDateFormat`就是一个典型的线程不安全事例，接下来动手来实现一下。首先先创建 10 个线程来格式化时间，时间格式化每次传递的待格式化时间都是不同的，所以程序如果正确执行将会打印 10 个不同的值，接下来看具体的代码实现：
```java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SimpleDateFormatExample {
    // 创建 SimpleDateFormat 对象
    private static SimpleDateFormat simpleDateFormat = new SimpleDateFormat("mm:ss");

    public static void main(String[] args) {
        // 创建线程池
        ExecutorService threadPool = Executors.newFixedThreadPool(10);
        // 执行 10 次时间格式化
        for (int i = 0; i < 10; i++) {
            int finalI = i;
            // 线程池执行任务
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    // 创建时间对象
                    Date date = new Date(finalI * 1000);
                    // 执行时间格式化并打印结果
                    System.out.println(simpleDateFormat.format(date));
                }
            });
        }
    }
}
```
预期的正确结果是这样的（10 次打印的值都不同）：<br />![2021-05-18-07-41-20-635166.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295579812-592aa2c2-76a3-412b-a992-e085a0b6c142.png#clientId=ub87690a7-708d-4&from=ui&id=u762092ab&originHeight=647&originWidth=691&originalType=binary&size=37136&status=done&style=none&taskId=uaad4750f-1938-4f53-8d11-86f40c6c22d)<br />然而，以上程序的运行结果却是这样的：<br />![2021-05-18-07-41-20-736895.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295588700-215babea-be10-49f2-9063-678112ea94aa.png#clientId=ub87690a7-708d-4&from=ui&id=u86b75e4d&originHeight=650&originWidth=688&originalType=binary&size=38623&status=done&style=none&taskId=u9f275dda-2644-4541-ac4d-5f25c08401e)<br />从上述结果可以看出，当在多线程中使用`SimpleDateFormat`进行时间格式化是线程不安全的。
<a name="aDN66"></a>
## 2、解决方案
`SimpleDateFormat`线程不安全的解决方案总共包含以下 5 种：

1. 将`SimpleDateFormat`定义为局部变量；
2. 使用`synchronized`加锁执行；
3. 使用`Lock`加锁执行（和解决方案 2 类似）；
4. 使用`ThreadLocal`；
5. 使用JDK 8中提供的`DateTimeFormat`。

接下来分别来看每种解决方案的具体实现。
<a name="JfidX"></a>
### ① `SimpleDateFormat`改为局部变量
将`SimpleDateFormat`定义为局部变量时，因为每个线程都是独享`SimpleDateFormat`对象的，相当于将多线程程序变成“单线程”程序了，所以不会有线程不安全的问题，具体实现代码如下：
```java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SimpleDateFormatExample {
    public static void main(String[] args) {
        // 创建线程池
        ExecutorService threadPool = Executors.newFixedThreadPool(10);
        // 执行 10 次时间格式化
        for (int i = 0; i < 10; i++) {
            int finalI = i;
            // 线程池执行任务
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    // 创建 SimpleDateFormat 对象
                    SimpleDateFormat simpleDateFormat = new SimpleDateFormat("mm:ss");
                    // 创建时间对象
                    Date date = new Date(finalI * 1000);
                    // 执行时间格式化并打印结果
                    System.out.println(simpleDateFormat.format(date));
                }
            });
        }
        // 任务执行完之后关闭线程池
        threadPool.shutdown();
    }
}
```
以上程序的执行结果为：<br />![2021-05-18-07-41-20-822664.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295596079-f822bb80-834d-42e6-93f2-c22b07fb1621.png#clientId=ub87690a7-708d-4&from=ui&id=ufa7e57a9&originHeight=641&originWidth=689&originalType=binary&size=37053&status=done&style=none&taskId=ub04aa249-626c-4a0f-aab1-0714e27f943)<br />当打印的结果都不相同时，表示程序的执行是正确的，从上述结果可以看出，将`SimpleDateFormat`定义为局部变量之后，就可以成功的解决线程不安全问题了。
<a name="kCocg"></a>
### ② 使用`synchronized`加锁
锁是解决线程不安全问题最常用的手段，接下来先用`synchronized`来加锁进行时间格式化，实现代码如下：
```java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SimpleDateFormatExample2 {
    // 创建 SimpleDateFormat 对象
    private static SimpleDateFormat simpleDateFormat = new SimpleDateFormat("mm:ss");

    public static void main(String[] args) {
        // 创建线程池
        ExecutorService threadPool = Executors.newFixedThreadPool(10);
        // 执行 10 次时间格式化
        for (int i = 0; i < 10; i++) {
            int finalI = i;
            // 线程池执行任务
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    // 创建时间对象
                    Date date = new Date(finalI * 1000);
                    // 定义格式化的结果
                    String result = null;
                    synchronized (simpleDateFormat) {
                        // 时间格式化
                        result = simpleDateFormat.format(date);
                    }
                    // 打印结果
                    System.out.println(result);
                }
            });
        }
        // 任务执行完之后关闭线程池
        threadPool.shutdown();
    }
}
```
以上程序的执行结果为：<br />![2021-05-18-07-41-20-930379.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295603045-acafb72c-2bd2-4234-8beb-e972c346f8e8.png#clientId=ub87690a7-708d-4&from=ui&id=u658082e3&originHeight=638&originWidth=676&originalType=binary&size=37025&status=done&style=none&taskId=u2d0a6bd1-02ad-440d-a7b8-513abe69971)
<a name="eg1pX"></a>
### ③ 使用Lock加锁
在 Java 语言中，锁的常用实现方式有两种，除了`synchronized`之外，还可以使用手动锁`Lock`，接下来使用`Lock`来对线程不安全的代码进行改造，实现代码如下：
```java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Lock 解决线程不安全问题
 */
public class SimpleDateFormatExample3 {
    // 创建 SimpleDateFormat 对象
    private static SimpleDateFormat simpleDateFormat = new SimpleDateFormat("mm:ss");

    public static void main(String[] args) {
        // 创建线程池
        ExecutorService threadPool = Executors.newFixedThreadPool(10);
        // 创建 Lock 锁
        Lock lock = new ReentrantLock();
        // 执行 10 次时间格式化
        for (int i = 0; i < 10; i++) {
            int finalI = i;
            // 线程池执行任务
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    // 创建时间对象
                    Date date = new Date(finalI * 1000);
                    // 定义格式化的结果
                    String result = null;
                    // 加锁
                    lock.lock();
                    try {
                        // 时间格式化
                        result = simpleDateFormat.format(date);
                    } finally {
                        // 释放锁
                        lock.unlock();
                    }
                    // 打印结果
                    System.out.println(result);
                }
            });
        }
        // 任务执行完之后关闭线程池
        threadPool.shutdown();
    }
}
```
以上程序的执行结果为：<br />![2021-05-18-07-41-21-060030.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295609788-d4226be1-bd1d-44ed-b48e-e303e6dce383.png#clientId=ub87690a7-708d-4&from=ui&id=ue96ed302&originHeight=643&originWidth=685&originalType=binary&size=37035&status=done&style=none&taskId=ued20ceeb-8714-47b1-98b9-f39bb43ec5a)<br />从上述代码可以看出，手动锁的写法相比于`synchronized`要繁琐一些。
<a name="I8U2M"></a>
### ④ 使用`ThreadLocal`
加锁方案虽然可以正确的解决线程不安全的问题，但同时也引入了新的问题，加锁会让程序进入排队执行的流程，从而一定程度的降低了程序的执行效率，如下图所示：<br />![2021-05-18-07-41-21-197662.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295615978-b8bbf2a8-9717-49e2-abc1-3aabfb9295e5.png#clientId=ub87690a7-708d-4&from=ui&id=u143de3a9&originHeight=676&originWidth=1080&originalType=binary&size=219256&status=done&style=shadow&taskId=u407bd0a9-2a0c-4413-8849-b1a72b1b6d1)<br />那有没有一种方案既能解决线程不安全的问题，同时还可以避免排队执行呢？<br />答案是有的，可以考虑使用`ThreadLocal`。`ThreadLocal`翻译为中文是线程本地变量的意思，字如其人`ThreadLocal`就是用来创建线程的私有（本地）变量的，每个线程拥有自己的私有对象，这样就可以避免线程不安全的问题了，实现如下：<br />![2021-05-18-07-41-21-319335.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295629558-f1982655-931a-4d2f-b286-767fdc577a85.png#clientId=ub87690a7-708d-4&from=ui&id=ufa3c27ae&originHeight=449&originWidth=1080&originalType=binary&size=205782&status=done&style=shadow&taskId=u0205f00e-c500-46a6-a09f-c91c1334128)<br />知道了实现方案之后，接下来使用具体的代码来演示一下`ThreadLocal`的使用，实现代码如下：
```java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * ThreadLocal 解决线程不安全问题
 */
public class SimpleDateFormatExample4 {
    // 创建 ThreadLocal 对象，并设置默认值（new SimpleDateFormat）
    private static ThreadLocal<SimpleDateFormat> threadLocal =
            ThreadLocal.withInitial(() -> new SimpleDateFormat("mm:ss"));

    public static void main(String[] args) {
        // 创建线程池
        ExecutorService threadPool = Executors.newFixedThreadPool(10);
        // 执行 10 次时间格式化
        for (int i = 0; i < 10; i++) {
            int finalI = i;
            // 线程池执行任务
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    // 创建时间对象
                    Date date = new Date(finalI * 1000);
                    // 格式化时间
                    String result = threadLocal.get().format(date);
                    // 打印结果
                    System.out.println(result);
                }
            });
        }
        // 任务执行完之后关闭线程池
        threadPool.shutdown();
    }
}
```
以上程序的执行结果为：<br />![2021-05-18-07-41-21-403113.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295639492-1eab9a8f-2cdd-4c24-9bdf-f599c906b605.png#clientId=ub87690a7-708d-4&from=ui&id=uaeb94c33&originHeight=652&originWidth=689&originalType=binary&size=37155&status=done&style=none&taskId=u471d7c5d-19b0-4d14-a5fa-f0a70504411)
<a name="hSMpo"></a>
#### ThreadLocal和局部变量的区别
首先来说`ThreadLocal`不等于局部变量，这里的“局部变量”指的是像 2.1 示例代码中的局部变量，`ThreadLocal`和局部变量最大的区别在于：`ThreadLocal`属于线程的私有变量，如果使用的是线程池，那么`ThreadLocal`中的变量是可以重复使用的，而代码级别的局部变量，每次执行时都会创建新的局部变量，二者区别如下图所示：<br />![2021-05-18-07-41-21-546731.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295647369-a77c90ec-bf8f-457d-9de9-77ed46b94776.png#clientId=ub87690a7-708d-4&from=ui&id=ua5cfd2ec&originHeight=338&originWidth=1080&originalType=binary&size=186814&status=done&style=shadow&taskId=uab7b27f5-d164-4785-b1a6-356b52fd3c1)
<a name="M06oR"></a>
### ⑤ 使用`DateTimeFormatter`
以上 4 种解决方案都是因为`SimpleDateFormat`是线程不安全的，所以需要加锁或者使用`ThreadLocal`来处理，然而，JDK 8之后就有了新的选择，如果使用的是JDK 8+ 版本，就可以直接使用JDK 8中新增的、安全的时间格式化工具类`DateTimeFormatter`来格式化时间了，接下来具体实现一下。<br />使用`DateTimeFormatter`必须要配合JDK 8中新增的时间对象`LocalDateTime`来使用，因此在操作之前，可以先将`Date`对象转换成 `LocalDateTime`，然后再通过`DateTimeFormatter`来格式化时间，具体实现代码如下：
```java
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * DateTimeFormatter 解决线程不安全问题
 */
public class SimpleDateFormatExample5 {
    // 创建 DateTimeFormatter 对象
    private static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("mm:ss");

    public static void main(String[] args) {
        // 创建线程池
        ExecutorService threadPool = Executors.newFixedThreadPool(10);
        // 执行 10 次时间格式化
        for (int i = 0; i < 10; i++) {
            int finalI = i;
            // 线程池执行任务
            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    // 创建时间对象
                    Date date = new Date(finalI * 1000);
                    // 将 Date 转换成 JDK 8 中的时间类型 LocalDateTime
                    LocalDateTime localDateTime =
                            LocalDateTime.ofInstant(date.toInstant(), ZoneId.systemDefault());
                    // 时间格式化
                    String result = dateTimeFormatter.format(localDateTime);
                    // 打印结果
                    System.out.println(result);
                }
            });
        }
        // 任务执行完之后关闭线程池
        threadPool.shutdown();
    }
}
```
以上程序的执行结果为：<br />![2021-05-18-07-41-21-650452.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295660265-ed51d71c-5351-4277-987f-992ebf1be8a3.png#clientId=ub87690a7-708d-4&from=ui&id=u11d3664e&originHeight=653&originWidth=687&originalType=binary&size=37157&status=done&style=none&taskId=u32b30e75-6fe4-4a43-969b-50e386509fb)
<a name="H3SBd"></a>
## 3、线程不安全原因分析
要了解`SimpleDateFormat`为什么是线程不安全的？需要查看并分析`SimpleDateFormat`的源码才行，先从使用的方法`format`入手，源码如下：
```java
private StringBuffer format(Date date, StringBuffer toAppendTo,
                                FieldDelegate delegate) {
    // 注意此行代码
    calendar.setTime(date);

    boolean useDateFormatSymbols = useDateFormatSymbols();

    for (int i = 0; i < compiledPattern.length; ) {
        int tag = compiledPattern[i] >>> 8;
        int count = compiledPattern[i++] & 0xff;
        if (count == 255) {
            count = compiledPattern[i++] << 16;
            count |= compiledPattern[i++];
        }

        switch (tag) {
            case TAG_QUOTE_ASCII_CHAR:
                toAppendTo.append((char)count);
                break;

            case TAG_QUOTE_CHARS:
                toAppendTo.append(compiledPattern, i, count);
                i += count;
                break;

            default:
                subFormat(tag, count, delegate, toAppendTo, useDateFormatSymbols);
                break;
        }
    }
    return toAppendTo;
}
```
也许是好运使然，没想到刚开始分析第一个方法就找到了线程不安全的问题所在。<br />从上述源码可以看出，在执行`SimpleDateFormat.format`方法时，会使用`calendar.setTime`方法将输入的时间进行转换，那么想象一下这样的场景：

1. 线程 1 执行了`calendar.setTime(date)`方法，将用户输入的时间转换成了后面格式化时所需要的时间；
2. 线程 1 暂停执行，线程 2 得到CPU时间片开始执行；
3. 线程 2 执行了`calendar.setTime(date)`方法，对时间进行了修改；
4. 线程 2 暂停执行，线程 1 得出CPU时间片继续执行，因为线程 1 和线程 2 使用的是同一对象，而时间已经被线程 2 修改了，所以此时当线程 1 继续执行的时候就会出现线程安全的问题了。

正常的情况下，程序的执行是这样的：<br />![2021-05-18-07-41-21-883826.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295668750-1c66bfe5-5199-4455-82c9-6ca66ca6a621.png#clientId=ub87690a7-708d-4&from=ui&id=u882bd0d4&originHeight=883&originWidth=1080&originalType=binary&size=430575&status=done&style=shadow&taskId=u3db3e2fb-af01-4538-85aa-ff3cb374b5d)<br />非线程安全的执行流程是这样的：<br />![2021-05-18-07-41-22-061351.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621295678377-36fa7997-4617-4d44-b6b6-27a92c5010fb.png#clientId=ub87690a7-708d-4&from=ui&id=u8a283362&originHeight=913&originWidth=1080&originalType=binary&size=305095&status=done&style=shadow&taskId=u267ac89d-b05e-4721-a22d-9d182fc837a)<br />在多线程执行的情况下，线程 1 的date1和线程 2 的date2，因为执行顺序的问题，最终都被格式化成date2 formatted，而非线程 1date1 formatted和线程 2date2 formatted，这样就会导致线程不安全的问题。
<a name="HjNIF"></a>
## 4、各方案优缺点总结
如果使用的是JDK 8+版本，可以直接使用线程安全的`DateTimeFormatter`来进行时间格式化，如果使用的JDK 8以下版本或者改造老的`SimpleDateFormat`代码，可以考虑使用`synchronized`或`ThreadLocal`来解决线程不安全的问题。因为实现方案 1 局部变量的解决方案，每次执行的时候都会创建新的对象，因此不推荐使用。`synchronized`的实现比较简单，而使用`ThreadLocal`可以避免加锁排队执行的问题。
