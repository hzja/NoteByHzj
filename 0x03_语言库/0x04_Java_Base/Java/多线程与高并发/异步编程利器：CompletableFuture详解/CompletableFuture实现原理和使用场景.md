Java CompletableFuture
<a name="g64Oo"></a>
## 1、概述
`CompletableFuture`是jdk1.8引入的实现类。扩展了`Future`和`CompletionStage`，是一个可以在任务完成阶段触发一些操作`Future`。简单的来讲就是可以实现异步回调。
<a name="sQI8M"></a>
## 2、为什么引入`CompletableFuture`
对于jdk1.5的`Future`，虽然提供了异步处理任务的能力，但是获取结果的方式很不优雅，还是需要通过阻塞（或者轮训）的方式。如何避免阻塞呢？其实就是注册回调。<br />业界结合观察者模式实现异步回调。也就是当任务执行完成后去通知观察者。比如Netty的`ChannelFuture`，可以通过注册监听实现异步结果的处理。
<a name="J7egn"></a>
### Netty的`ChannelFuture`
```java
public Promise<V> addListener(GenericFutureListener<? extends Future<? super V>> listener) {
    checkNotNull(listener, "listener");
    synchronized (this) {
        addListener0(listener);
    }
    if (isDone()) {
        notifyListeners();
    }
    return this;
}
private boolean setValue0(Object objResult) {
    if (RESULT_UPDATER.compareAndSet(this, null, objResult) ||
        RESULT_UPDATER.compareAndSet(this, UNCANCELLABLE, objResult)) {
        if (checkNotifyWaiters()) {
            notifyListeners();
        }
        return true;
    }
    return false;
}
```
通过`addListener`方法注册监听。如果任务完成，会调用`notifyListeners`通知。<br />`CompletableFuture`通过扩展`Future`，引入函数式编程，通过回调的方式去处理结果。
<a name="JqHqk"></a>
## 3、功能
`CompletableFuture`的功能主要体现在他的`CompletionStage`。<br />可以实现如下等功能

- 转换（`thenCompose`）
- 组合（`thenCombine`）
- 消费（`thenAccept`）
- 运行（`thenRun`）。
- 带返回的消费（`thenApply`）

消费和运行的区别：<br />消费使用执行结果。运行则只是运行特定任务。具体其他功能大家可以根据需求自行查看。<br />`CompletableFuture`借助`CompletionStage`的方法可以实现链式调用。并且可以选择同步或者异步两种方式。<br />这里举个简单的例子来体验一下他的功能。
```java
public static void thenApply() {
    ExecutorService executorService = Executors.newFixedThreadPool(2);
    CompletableFuture cf = CompletableFuture.supplyAsync(() -> {
        try {
            //  Thread.sleep(2000);
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("supplyAsync " + Thread.currentThread().getName());
        return "hello";
    }, executorService).thenApplyAsync(s -> {
        System.out.println(s + "world");
        return "hhh";
    }, executorService);
    cf.thenRunAsync(() -> {
        System.out.println("ddddd");
    });
    cf.thenRun(() -> {
        System.out.println("ddddsd");
    });
    cf.thenRun(() -> {
        System.out.println(Thread.currentThread());
        System.out.println("dddaewdd");
    });
}
```
执行结果
```java
supplyAsync pool-1-thread-1
helloworld
ddddd
ddddsd
Thread[main,5,main]
dddaewdd
```
根据结果可以看到会有序执行对应任务。<br />注意：<br />如果是同步执行`cf.thenRun`。他的执行线程可能main线程，也可能是执行源任务的线程。如果执行源任务的线程在main调用之前执行完了任务。那么`cf.thenRun`方法会由main线程调用。<br />这里说明一下，如果是同一任务的依赖任务有多个：

- 如果这些依赖任务都是同步执行。那么假如这些任务被当前调用线程（main）执行，则是有序执行，假如被执行源任务的线程执行，那么会是倒序执行。因为内部任务数据结构为LIFO。
- 如果这些依赖任务都是异步执行，那么他会通过异步线程池去执行任务。不能保证任务的执行顺序。

上面的结论是通过阅读源代码得到的。下面深入源代码。
<a name="W87Kd"></a>
## 4、源码追踪
<a name="scVUk"></a>
### 创建`CompletableFuture`
创建的方法有很多，甚至可以直接`new`一个。来看一下`supplyAsync`异步创建的方法。
```java
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier,
                                                   Executor executor) {
    return asyncSupplyStage(screenExecutor(executor), supplier);
}
static Executor screenExecutor(Executor e) {
    if (!useCommonPool && e == ForkJoinPool.commonPool())
        return asyncPool;
    if (e == null) throw new NullPointerException();
    return e;
}
```
入参`Supplier`，带返回值的函数。如果是异步方法，并且传递了执行器，那么会使用传入的执行器去执行任务。否则采用公共的ForkJoin并行线程池，如果不支持并行，新建一个线程去执行。<br />这里需要注意`ForkJoin`是通过守护线程去执行任务的。所以必须有非守护线程的存在才行。
<a name="ZojwF"></a>
### `asyncSupplyStage`方法
```java
static <U> CompletableFuture<U> asyncSupplyStage(Executor e,
                                                 Supplier<U> f) {
    if (f == null) throw new NullPointerException();
    CompletableFuture<U> d = new CompletableFuture<U>();
    e.execute(new AsyncSupply<U>(d, f));
    return d;
}
```
这里会创建一个用于返回的`CompletableFuture`。<br />然后构造一个`AsyncSupply`，并将创建的`CompletableFuture`作为构造参数传入。<br />那么，任务的执行完全依赖`AsyncSupply`。
<a name="vK9gj"></a>
### `AsyncSupply#run`
```java
public void run() {
    CompletableFuture<T> d; Supplier<T> f;
    if ((d = dep) != null && (f = fn) != null) {
        dep = null; fn = null;
        if (d.result == null) {
            try {
                d.completeValue(f.get());
            } catch (Throwable ex) {
                d.completeThrowable(ex);
            }
        }
        d.postComplete();
    }
}
```

1. 该方法会调用`Supplier`的`get`方法。并将结果设置到`CompletableFuture`中。应该清楚这些操作都是在异步线程中调用的。
2. `d.postComplete`方法就是通知任务执行完成。触发后续依赖任务的执行，也就是实现`CompletionStage`的关键点。

在看`postComplete`方法之前先来看一下创建依赖任务的逻辑。
<a name="kOkIa"></a>
### `thenAcceptAsync`方法
```java
public CompletableFuture<Void> thenAcceptAsync(Consumer<? super T> action) {
    return uniAcceptStage(asyncPool, action);
}
private CompletableFuture<Void> uniAcceptStage(Executor e,
                                               Consumer<? super T> f) {
    if (f == null) throw new NullPointerException();
    CompletableFuture<Void> d = new CompletableFuture<Void>();
    if (e != null || !d.uniAccept(this, f, null)) {
        # 1
        UniAccept<T> c = new UniAccept<T>(e, d, this, f);
        push(c);
        c.tryFire(SYNC);
    }
    return d;
}
```
上面提到过。`thenAcceptAsync`是用来消费`CompletableFuture`的。该方法调用`uniAcceptStage`。
<a name="k79I4"></a>
#### `uniAcceptStage`逻辑：

1. 构造一个`CompletableFuture`，主要是为了链式调用。
2. 如果为异步任务，直接返回。因为源任务结束后会触发异步线程执行对应逻辑。
3. 如果为同步任务（e==null），会调用`d.uniAccept`方法。这个方法在这里逻辑：如果源任务完成，调用f，返回true。否则进入if代码块（Mark 1）。
4. 如果是异步任务直接进入if（Mark 1）。
<a name="K3o8j"></a>
#### Mark1逻辑：

1. 构造一个`UniAccept`，将其`push`入栈。这里通过CAS实现乐观锁实现。
2. 调用`c.tryFire`方法。
```java
final CompletableFuture<Void> tryFire(int mode) {
    CompletableFuture<Void> d; CompletableFuture<T> a;
    if ((d = dep) == null ||
        !d.uniAccept(a = src, fn, mode > 0 ? null : this))
        return null;
    dep = null; src = null; fn = null;
    return d.postFire(a, mode);
}
```

1. 会调用`d.uniAccept`方法。其实该方法判断源任务是否完成，如果完成则执行依赖任务，否则返回false。
2. 如果依赖任务已经执行，调用`d.postFire`，主要就是Fire的后续处理。根据不同模式逻辑不同。

这里简单说一下，其实mode有同步异步，和迭代。迭代为了避免无限递归。<br />这里强调一下`d.uniAccept`方法的第三个参数。<br />如果是异步调用（mode>0），传入null。否则传入this。<br />区别看下面代码。c不为null会调用`c.claim`方法。
```java
try {
    if (c != null && !c.claim())
        return false;
    @SuppressWarnings("unchecked") S s = (S) r;
    f.accept(s);
    completeNull();
} catch (Throwable ex) {
    completeThrowable(ex);
}

final boolean claim() {
    Executor e = executor;
    if (compareAndSetForkJoinTaskTag((short)0, (short)1)) {
        if (e == null)
            return true;
        executor = null; // disable
        e.execute(this);
    }
    return false;
}
```
<a name="RTwRd"></a>
#### `claim`方法是逻辑：

- 如果异步线程为null。说明同步，那么直接返回true。最后上层函数会调用`f.accept(s)`同步执行任务。
- 如果异步线程不为null，那么使用异步线程去执行this。

this的run任务如下。也就是在异步线程同步调用`tryFire`方法。达到其被异步线程执行的目的。
```java
public final void run(){ 
   tryFire(ASYNC); 
}
```
看完上面的逻辑，基本理解依赖任务的逻辑。<br />其实就是先判断源任务是否完成，如果完成，直接在对应线程执行以来任务（如果是同步，则在当前线程处理，否则在异步线程处理）<br />如果任务没有完成，直接返回，因为等任务完成之后会通过`postComplete`去触发调用依赖任务。
<a name="z0Eit"></a>
### `postComplete`方法
```java
final void postComplete() {
    /*
     * On each step, variable f holds current dependents to pop
     * and run.  It is extended along only one path at a time,
     * pushing others to avoid unbounded recursion.
     */
    CompletableFuture<?> f = this; Completion h;
    while ((h = f.stack) != null ||
           (f != this && (h = (f = this).stack) != null)) {
        CompletableFuture<?> d; Completion t;
        if (f.casStack(h, t = h.next)) {
            if (t != null) {
                if (f != this) {
                    pushStack(h);
                    continue;
                }
                h.next = null;    // detach
            }
            f = (d = h.tryFire(NESTED)) == null ? this : d;
        }
    }
}
```
在源任务完成之后会调用。<br />其实逻辑很简单，就是迭代堆栈的依赖任务。调用`h.tryFire`方法。NESTED就是为了避免递归死循环。因为FirePost会调用`postComplete`。如果是NESTED，则不调用。<br />堆栈的内容其实就是在依赖任务创建的时候加入进去的。上面已经提到过。
<a name="UNMjn"></a>
## 5、总结
基本上述源码已经分析了逻辑。<br />因为涉及异步等操作，需要理一下（这里针对全异步任务）：

1. 创建`CompletableFuture`成功之后会通过异步线程去执行对应任务。
2. 如果`CompletableFuture`还有依赖任务（异步），会将任务加入到`CompletableFuture`的堆栈保存起来。以供后续完成后执行依赖任务。

当然，创建依赖任务并不只是将其加入堆栈。如果源任务在创建依赖任务的时候已经执行完成，那么当前线程会触发依赖任务的异步线程直接处理依赖任务。并且会告诉堆栈其他的依赖任务源任务已经完成。<br />主要是考虑代码的复用。所以逻辑相对难理解。<br />`postComplete`方法会被源任务线程执行完源任务后调用。同样也可能被依赖任务线程后调用。<br />执行依赖任务的方法主要就是靠`tryFire`方法。因为这个方法可能会被多种不同类型线程触发，所以逻辑也绕一点。（其他依赖任务线程、源任务线程、当前依赖任务线程）

- 如果是当前依赖任务线程，那么会执行依赖任务，并且会通知其他依赖任务。
- 如果是源任务线程，和其他依赖任务线程，则将任务转换给依赖线程去执行。不需要通知其他依赖任务，避免死递归。
