Java CAS算法 原子类
<a name="WhVbh"></a>
## 1、乐观锁
一般而言，在并发情况下必须通过一定的手段来保证数据的准确性，如果没有做好并发控制，就可能导致脏读、幻读和不可重复度等一系列问题。乐观锁是人们为了应付并发问题而提出的一种思想，具体的实现则有多种方式。<br />乐观锁假设数据一般情况下不会造成冲突，只在数据进行提交更新时，才会正式对数据的冲突与否进行检测，如果发现冲突了，则返回给用户错误的信息，让用户决定如何去做。乐观锁适用于读操作多的场景，可以提高程序的吞吐量。
<a name="Q61hk"></a>
## 2、CAS
CAS（Compare And Swap）比较并交换，是一种实现了乐观锁思想的并发控制技术。CAS 算法的过程是：它包含 3 个参数 CAS（V，E，N），V 表示要更新的变量（内存值），E 表示旧的预期值，N 表示即将更新的预期值。当且仅当 V 值等于 E 值时，才会将 V 的值设为 N，如果 V 值和 E 值不同，说明已经有其他线程做了更新，则当前线程什么也不做，并返回当前 V 的真实值。整个操作是原子性的。<br />当多个线程同时使用 CAS 操作一个变量时，只有一个会胜出，并成功更新，其余均会失败。失败的线程不会被挂起，仅是被告知失败，并允许再次尝试，当然也可以放弃本次操作，所以 CAS 算法是非阻塞的。基于上述原理，CAS 操作可以在不借助锁的情况下实现合适的并发处理。
<a name="N5qi3"></a>
## 3、ABA 问题
ABA 问题是 CAS 算法的一个漏洞。CAS 算法实现的一个重要前提是：取出内存中某时刻的数据，并在下一时刻比较并替换，在这个时间差内可能会导致数据的变化。<br />假设有两个线程，分别要对内存中某一变量做 CAS 操作，线程一先从内存中取出值 A，线程二也从内存中取出值 A，并把值从 A 变为 B 写回，然后又把值从 B 变为 A 写回，这时候线程一进行 CAS 操作，发现内存中的值还是 A，于是认为和预期值一致，操作成功。尽管线程一的 CAS 操作成功，但并不代表这个过程就没有问题。<br />ABA 问题会带来什么隐患呢？维基百科给出了详细的示例：假设现有一个用单链表实现的堆栈，栈顶为 A，A.next = B，现有线程一希望用 CAS 把栈顶替换为 B，但在此之前，线程二介入，将 A、B 出栈，再压入 D、C、A，整个过程如下<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603675947959-1a0309b6-1f97-45bb-addc-738d99814d04.png#averageHue=%23f9f9f9&height=365&id=V63Bk&originHeight=365&originWidth=728&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=728)<br />此时 B 处于游离转态，轮到线程一执行 CAS 操作，发现栈顶仍为 A，CAS 成功，栈顶变为 B，但实际上 B.next = null，即堆栈中只有 B 一个元素，C 和 D 并不在堆栈中，平白无故就丢了。简单来说，ABA 问题使我们漏掉某一段时间的数据监控，谁知道在这段时间内会发生什么有趣（可怕）的事呢？<br />可以通过版本号的方式来解决 ABA 问题，每次执行数据修改操作时，都会带上一个版本号，如果版本号和数据的版本一致，对数据进行修改操作并对版本号 +1，否则执行失败。因为每次操作的版本号都会随之增加，所以不用担心出现 ABA 问题。
<a name="XD5yl"></a>
## 4、使用 Java 模拟 CAS 算法
这仅仅是基于 Java 层面上的模拟，真正的实现要涉及到底层
```java
public class TestCompareAndSwap {
    private static CompareAndSwap cas = new CompareAndSwap();
    public static void main(String[] args) {
        for (int i = 0; i < 10; i++) {
            new Thread(new Runnable() {
                public void run() {
                    // 获取预估值
                    int expectedValue = cas.get();
                    boolean b = cas.compareAndSet(expectedValue, (int) (Math.random() * 101));
                    System.out.println(b);
                }
            });
        }
    }
}
class CompareAndSwap {
    private int value;
    // 获取内存值
    public synchronized int get() {
        return value;
    }
    // 比较
    public synchronized int compareAndSwap(int expectedValue, int newValue) {
        // 读取内存值
        int oldValue = value;
        // 比较
        if (oldValue == expectedValue) {
            this.value = newValue;
        }
        return oldValue;
    }
    // 设置
    public synchronized boolean compareAndSet(int expectedValue, int newValue) {
        return expectedValue == compareAndSwap(expectedValue, newValue);
    }
}
```
<a name="GELwb"></a>
## 5、原子类
原子包 java.util.concurrent.atomic 提供了一组原子类，原子类的操作具有原子性，一旦开始，就一直运行直到结束，中间不会有任何线程上下文切换。原子类的底层正是基于 CAS 算法实现线程安全。<br />Java 提供了十六个原子类，可以大致分为以下四种：
<a name="WU4ZS"></a>
### 1. 基本类型

- `AtomicBoolean`<br />原子更新布尔类型，内部使用 int 类型的 value 存储 1 和 0 表示 true 和 false，底层也是对 int 类型的原子操作
- `AtomicInteger`<br />原子更新 int 类型
- `AtomicLong`<br />原子更新 long 类型
<a name="9zLtm"></a>
### 2. 引用类型

- `AtomicReference`<br />原子更新引用类型，通过泛型指定要操作的类
- `AtomicMarkableReference`<br />原子更新引用类型，内部维护一个 Pair 类型（静态内部类）的成员属性，其中有一个 boolean 类型的标志位，避免 ABA 问题
```java
private static class Pair<T> {
    final T reference;
    final boolean mark;
    private Pair(T reference, boolean mark) {
        this.reference = reference;
        this.mark = mark;
    }
    static <T> Pair<T> of(T reference, boolean mark) {
        return new Pair<T>(reference, mark);
    }
}
private volatile Pair<V> pair
```

- `AtomicStampedReference`<br />原子更新引用类型，内部维护一个 Pair 类型（静态内部类）的成员属性，其中有一个 int 类型的邮戳（版本号），避免 ABA 问题
```java
private static class Pair<T> {
    final T reference;
    final int stamp;
    private Pair(T reference, int stamp) {
        this.reference = reference;
        this.stamp = stamp;
    }
    static <T> Pair<T> of(T reference, int stamp) {
        return new Pair<T>(reference, stamp);
    }
}
private volatile Pair<V> pair
```
<a name="TyKCx"></a>
### 3. 数组类型

- `AtomicIntegerArray`<br />原子更新 int 数组中的元素
- `AtomicLongArray`<br />原子更新 long 数组中的元素
- `AtomicReferenceArray`<br />原子更新 Object 数组中的元素
<a name="3WfF4"></a>
### 4. 对象属性类型
用于解决对象的属性的原子操作

- `AtomicIntegerFieldUpdater`<br />原子更新对象中的 int 类型字段
- `AtomicLongFieldUpdater`<br />原子更新对象中的 long 类型字段
- `AtomicReferenceFieldUpdater`<br />原子更新对象中的引用类型字段

之前提到的三种类型的使用都比较简单，查阅对应 API 即可，而对象属性类型则有一些限制：

- 字段必须是 volatile 类型的，在线程之间共享变量时保证立即可见
- 只能是实例变量，不能是类变量，也就是说不能加 static 关键字
- 只能是可修改变量，不能使用 final 变量
- 该对象字段能够被直接操作，因为它是基于反射实现的
<a name="UqshH"></a>
### 5. 高性能原子类
Java8 新增的原子类，使用分段的思想，把不同的线程 hash 到不同的段上去更新，最后再把这些段的值相加得到最终的值。以下四个类都继承自 Striped64，对并发的优化在 Striped64 中实现<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603675947936-c2d1e434-c98a-416d-9230-4ab89a61dbd2.png#averageHue=%23f8f8f8&height=671&id=vbk4G&originHeight=671&originWidth=680&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=680)

- `LongAccumulator`<br />long 类型的聚合器，需要传入一个 long 类型的二元操作，可以用来计算各种聚合操作，包括加乘等
- `LongAdder`<br />long 类型的累加器，`LongAccumulator` 的特例，只能用来计算加法，且从 0 开始计算
- `DoubleAccumulator`<br />double 类型的聚合器，需要传入一个 double 类型的二元操作，可以用来计算各种聚合操作，包括加乘等
- `DoubleAdder`<br />double 类型的累加器，`DoubleAccumulator` 的特例，只能用来计算加法，且从 0 开始计算
