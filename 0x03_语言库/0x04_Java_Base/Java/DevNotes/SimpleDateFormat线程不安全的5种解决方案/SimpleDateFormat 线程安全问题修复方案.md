<a name="uq5HW"></a>
## 问题介绍
在日常的开发过程中，不可避免地会使用到 JDK8 之前的 Date 类，在格式化日期或解析日期时就需要用到 `SimpleDateFormat` 类，但由于该类并不是线程安全的，所以可以发现对该类的不恰当使用会导致日期解析异常，从而影响线上服务可用率。<br />以下是对 `SimpleDateFormat` 类不恰当使用的示例代码：
```java
package com.jd.threadsafe;

import java.text.SimpleDateFormat;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @Date: 2023/7/25 10:47
 * @Desc: SimpleDateFormat 线程安全问题复现
 * @Version: V1.0
 **/
public class SimpleDateFormatTest {
    private static final AtomicBoolean STOP = new AtomicBoolean();
    private static final SimpleDateFormat FORMATTER = new SimpleDateFormat("yyyy-M-d"); // 非线程安全

    public static void main(String[] args) {
        Runnable runnable = () -> {
            int count = 0;
            while (!STOP.get()) {
                try {
                    FORMATTER.parse("2023-7-15");
                } catch (Exception e) {
                    e.printStackTrace();
                    if (++count > 3) {
                        STOP.set(true);
                    }
                }
            }
        };

        new Thread(runnable).start();
        new Thread(runnable).start();
    }
}
```
以上代码模拟了多线程并发使用 `SimpleDateFormat` 实例的场景，此时可观察到如下异常输出：
```
java.lang.NumberFormatException: For input string: ""
	at java.lang.NumberFormatException.forInputString(NumberFormatException.java:65)
	at java.lang.Long.parseLong(Long.java:601)
	at java.lang.Long.parseLong(Long.java:631)
	at java.text.DigitList.getLong(DigitList.java:195)
	at java.text.DecimalFormat.parse(DecimalFormat.java:2082)
	at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:1869)
	at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514)
	at java.text.DateFormat.parse(DateFormat.java:364)
	at com.jd.threadsafe.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:21)
	at java.lang.Thread.run(Thread.java:750)
java.lang.NumberFormatException: For input string: ""
	at java.lang.NumberFormatException.forInputString(NumberFormatException.java:65)
	at java.lang.Long.parseLong(Long.java:601)
	at java.lang.Long.parseLong(Long.java:631)
	at java.text.DigitList.getLong(DigitList.java:195)
	at java.text.DecimalFormat.parse(DecimalFormat.java:2082)
	at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:2162)
	at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514)
	at java.text.DateFormat.parse(DateFormat.java:364)
	at com.jd.threadsafe.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:21)
	at java.lang.Thread.run(Thread.java:750)
java.lang.NumberFormatException: multiple points
	at sun.misc.FloatingDecimal.readJavaFormatString(FloatingDecimal.java:1890)
	at sun.misc.FloatingDecimal.parseDouble(FloatingDecimal.java:110)
	at java.lang.Double.parseDouble(Double.java:538)
	at java.text.DigitList.getDouble(DigitList.java:169)
	at java.text.DecimalFormat.parse(DecimalFormat.java:2087)
	at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:1869)
	at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514)
	at java.text.DateFormat.parse(DateFormat.java:364)
	at com.jd.threadsafe.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:21)
	at java.lang.Thread.run(Thread.java:750)
java.lang.NumberFormatException: multiple points
	at sun.misc.FloatingDecimal.readJavaFormatString(FloatingDecimal.java:1890)
	at sun.misc.FloatingDecimal.parseDouble(FloatingDecimal.java:110)
	at java.lang.Double.parseDouble(Double.java:538)
	at java.text.DigitList.getDouble(DigitList.java:169)
	at java.text.DecimalFormat.parse(DecimalFormat.java:2087)
	at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:1869)
	at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514)
	at java.text.DateFormat.parse(DateFormat.java:364)
	at com.jd.threadsafe.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:21)
	at java.lang.Thread.run(Thread.java:750)
java.lang.NumberFormatException: For input string: ""
	at java.lang.NumberFormatException.forInputString(NumberFormatException.java:65)
	at java.lang.Long.parseLong(Long.java:601)
	at java.lang.Long.parseLong(Long.java:631)
	at java.text.DigitList.getLong(DigitList.java:195)
	at java.text.DecimalFormat.parse(DecimalFormat.java:2082)
	at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:1869)
	at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514)
	at java.text.DateFormat.parse(DateFormat.java:364)
	at com.jd.threadsafe.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:21)
	at java.lang.Thread.run(Thread.java:750)
```
以上异常的根本原因是因为 `SimpleDateFormat` 是**有状态**的，如 `SimpleDateFormat` 类中含有**非线程安全的** `NumberFormat` 成员变量：
```java
/**
 * The number formatter that <code>DateFormat</code> uses to format numbers
 * in dates and times.  Subclasses should initialize this to a number format
 * appropriate for the locale associated with this <code>DateFormat</code>.
 * @serial
 */
protected NumberFormat numberFormat;
```
从 `NumberFormat` 的 Java Doc 中能看到如下描述：
> Synchronization Number formats are generally not synchronized. It is recommended to create separate format instances for each thread. If multiple threads access a format concurrently, it must be synchronized externally.

从 `SimpleDateFormat` 的 Java Doc 中能看到如下描述：
> Synchronization Date formats are not synchronized. It is recommended to create separate format instances for each thread. If multiple threads access a format concurrently, it must be synchronized externally.

<a name="bTXCM"></a>
## 修复方案一：加锁（不推荐）
```java
package com.jd.threadsafe;

import java.text.SimpleDateFormat;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @Date: 2023/7/25 10:47
 * @Desc: SimpleDateFormat 线程安全修复方案：加锁
 * @Version: V1.0
 **/
public class SimpleDateFormatLockTest {
    private static final AtomicBoolean STOP = new AtomicBoolean();
    private static final SimpleDateFormat FORMATTER = new SimpleDateFormat("yyyy-M-d"); // 非线程安全

    public static void main(String[] args) {
        Runnable runnable = () -> {
            int count = 0;
            while (!STOP.get()) {
                try {
                    synchronized (FORMATTER) {
                        FORMATTER.parse("2023-7-15");
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    if (++count > 3) {
                        STOP.set(true);
                    }
                }
            }
        };

        new Thread(runnable).start();
        new Thread(runnable).start();
    }

}
```
首先能想到的最简单的解决线程安全问题的修复方案即加锁，如以上修复方案，使用 `synchronized` 关键字对 `FORMATTER` 实例进行加锁，此时多线程进行日期格式化时退化为串行执行，保证了**正确性**但**牺牲了性能**，不推荐。
<a name="XLES9"></a>
## 修复方案二：栈封闭（不推荐）
如果按照文档中的推荐用法，可知推荐为每个线程创建独立的 `SimpleDateFormat` 实例，一种最简单的方式就是在方法调用时每次创建 `SimpleDateFormat` 实例，以实现**栈封闭**的效果，如以下示例代码：
```java
package com.jd.threadsafe;

import java.text.SimpleDateFormat;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @Date: 2023/7/25 10:47
 * @Desc: SimpleDateFormat 线程安全修复方案：栈封闭
 * @Version: V1.0
 **/
public class SimpleDateFormatStackConfinementTest {
    private static final AtomicBoolean STOP = new AtomicBoolean();

    public static void main(String[] args) {
        Runnable runnable = () -> {
            int count = 0;
            while (!STOP.get()) {
                try {
                    new SimpleDateFormat("yyyy-M-d").parse("2023-7-15");
                } catch (Exception e) {
                    e.printStackTrace();
                    if (++count > 3) {
                        STOP.set(true);
                    }
                }
            }
        };

        new Thread(runnable).start();
        new Thread(runnable).start();
    }

}
```
即将共用的 `SimpleDateFormat` 实例调整为每次创建新的实例，该修复方案保证了正确性但每次方法调用需要创建 `SimpleDateFormat` 实例，并未复用 `SimpleDateFormat` 实例，**存在 GC 损耗**，所以并不推荐。
<a name="FalgY"></a>
## 修复方案三：`ThreadLocal`（推荐）
如果日期格式化操作是应用里的高频操作，且需要优先保证性能，那么建议每个线程复用 `SimpleDateFormat` 实例，此时可引入 `ThreadLocal` 类来解决该问题：
```java
package com.jd.threadsafe;

import java.text.SimpleDateFormat;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @Date: 2023/7/25 10:47
 * @Desc: SimpleDateFormat 线程安全修复方案：ThreadLocal
 * @Version: V1.0
 **/
public class SimpleDateFormatThreadLocalTest {
    private static final AtomicBoolean STOP = new AtomicBoolean();
    private static final ThreadLocal<SimpleDateFormat> SIMPLE_DATE_FORMAT_THREAD_LOCAL = ThreadLocal.withInitial(() -> new SimpleDateFormat("yyyy-M-d"));

    public static void main(String[] args) {
        Runnable runnable = () -> {
            int count = 0;
            while (!STOP.get()) {
                try {
                    SIMPLE_DATE_FORMAT_THREAD_LOCAL.get().parse("2023-7-15");
                } catch (Exception e) {
                    e.printStackTrace();
                    if (++count > 3) {
                        STOP.set(true);
                    }
                }
            }
        };

        new Thread(runnable).start();
        new Thread(runnable).start();
    }

}
```
执行上述代码，不会再观察到异常输出，因为已为每个线程创建了独立的 `SimpleDateFormat` 实例，即在**线程维度**复用了 `SimpleDateFormat` 实例，在线程池等池化场景下相比上方**栈封闭**的修复方案降低了 GC 损耗，同时也规避了线程安全问题。<br />以上使用 `ThreadLocal` 在线程维度复用非线程安全的实例可认为是一种通用的**模式**，可在 JDK 及不少开源项目中看到类似的模式实现，如在 JDK 最常见的 String 类中，对字符串进行编解码所需要用到的 `StringDecoder` 及 `StringEncoder` 即使用了 ThreadLocal 来规避线程安全问题：
```java
/**
 * Utility class for string encoding and decoding.
 */
class StringCoding {

    private StringCoding() { }

    /** The cached coders for each thread */
    private final static ThreadLocal<SoftReference<StringDecoder>> decoder =
    new ThreadLocal<>();
    private final static ThreadLocal<SoftReference<StringEncoder>> encoder =
    new ThreadLocal<>();

    // ...
}
```
参考：JDK8 - StringCoding<br />在 Dubbo 的 ThreadLocalKryoFactory 类中，在对非线程安全类 Kryo 的使用中，也使用了 ThreadLocal 类来规避线程安全问题：
```java
package org.apache.dubbo.common.serialize.kryo.utils;

import com.esotericsoftware.kryo.Kryo;

public class ThreadLocalKryoFactory extends AbstractKryoFactory {

    private final ThreadLocal<Kryo> holder = new ThreadLocal<Kryo>() {
        @Override
        protected Kryo initialValue() {
            return create();
        }
    };

    @Override
    public void returnKryo(Kryo kryo) {
        // do nothing
    }

    @Override
    public Kryo getKryo() {
        return holder.get();
    }
}
```
参考：Dubbo - ThreadLocalKryoFactory<br />类似地，在 HikariCP 的 ConcurrentBag 类中，也用到了 ThreadLocal 类来规避线程安全问题，此处不再进一步展开。
<a name="NT3A2"></a>
## 修复方案四：`FastDateFormat`（推荐）
针对 `SimpleDateFormat` 类的线程安全问题，apache commons-lang 提供了 `FastDateFormat` 类。其部分 Java Doc 如下：
> FastDateFormat is a fast and thread-safe version ofSimpleDateFormat. To obtain an instance of FastDateFormat, use one of the static factory methods:getInstance(String, TimeZone, Locale),getDateInstance(int, TimeZone, Locale),getTimeInstance(int, TimeZone, Locale), orgetDateTimeInstance(int, int, TimeZone, Locale) Since FastDateFormat is thread safe, you can use a static member instance: private static final FastDateFormat DATE_FORMATTER = FastDateFormat.getDateTimeInstance(FastDateFormat.LONG, FastDateFormat.SHORT); This class can be used as a direct replacement toSimpleDateFormatin most formatting and parsing situations. This class is especially useful in multi-threaded server environments.SimpleDateFormatis not thread-safe in any JDK version, nor will it be as Sun have closed the bug/RFE. All patterns are compatible with SimpleDateFormat (except time zones and some year patterns - see below).

该修复方案相对来说代码改造最小，仅需在声明静态 `SimpleDateFormat` 实例代码处将 `SimpleDateFormat` 实例替换为 `FastDateFormat` 实例，示例代码如下：
```java
package com.jd.threadsafe;

import org.apache.commons.lang3.time.FastDateFormat;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @Date: 2023/7/6 20:05
 * @Desc: SimpleDateFormat 线程安全修复方案：FastDateFormat
 * @Version: V1.0
 **/
public class FastDateFormatTest {
    private static final AtomicBoolean STOP = new AtomicBoolean();
    private static final FastDateFormat FORMATTER = FastDateFormat.getInstance("yyyy-M-d");

    public static void main(String[] args) {
        Runnable runnable = () -> {
            int count = 0;
            while (!STOP.get()) {
                try {
                    FORMATTER.parse("2023-7-15");
                } catch (Exception e) {
                    e.printStackTrace();
                    if (++count > 3) {
                        STOP.set(true);
                    }
                }
            }
        };

        new Thread(runnable).start();
        new Thread(runnable).start();
    }

}
```
执行上述代码，不会再观察到异常输出，因为 `FastDateFormat` 是线程安全的实现，支持多线程并发调用。
<a name="WKHTW"></a>
## 总结
无论使用哪种修复方案，都需要在修改后进行充分的测试，保证修复后不影响原有业务逻辑，如通过单元测试、流量回放等方式来保证本次修复的正确性。
<a name="B17Hb"></a>
## 思考
代码里使用 `SimpleDateFormat` 类的原因是因为日期使用了 Date 类，与 Date 相配套的 JDK 格式化类即 `SimpleDateFormat` 类，如果在处理日期时使用 JDK8 引入的 `LocalDateTime` 等**不可变**日期类，那么格式化将使用配套的**线程安全**的 `DateTimeFormatter` 类，从根源上规避掉对非线程安全类 `SimpleDateFormat` 类的使用。
