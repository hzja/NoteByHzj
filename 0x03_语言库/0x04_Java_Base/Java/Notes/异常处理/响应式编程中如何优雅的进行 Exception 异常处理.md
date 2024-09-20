初识响应式编程的时候，除了从命令式的思维方式转变为函数式的编程方式外，其中有一个很大的不适应的地方就是在面对异常时该怎么处理，尤其是面对检查异常（Checked Exception)时更是不知所措。<br />在遇到异常时，通用的处理方式就是打日志、降级兜底、重试三板斧，本文通过Project Reactor的文档以及源码来深入解读，在reactor中是如何优雅地实现这异常处理三板斧。<br />在介绍怎么使用前，先回顾下在用reactor编程的时候，遇到的几个问题：

- 遇到异常时，如果能处理，该怎么兜底/降级
- 遇到无法处理的异常时，该怎么打印日志，并往外抛
- 遇到声明了检查异常的方法时，该怎么处理
- 如果调用失败了（如请求超时），该如何重试
- 如果出现异常了，流里面的后续数据还会继续发送吗
<a name="xP3hF"></a>
## 1、异常处理的底层机制
在回答这些问题，就需要首先对reactor处理异常的机制要有理解。先说结论，如文档上说的：
> Before you learn about error-handling operators, you must keep in mind that any error in a reactive sequence is a terminal event. Even if an error-handling operator is used, it does not let the original sequence continue. Rather, it converts the onError signal into the start of a new sequence (the fallback one). In other words, it replaces the terminated sequence upstream of it.

即，**一旦出现了异常，那原先的数据流就会直接结束了，是没有办法再恢复的。所以如果要降级兜底，那只能再替换一个新的流。或者重试，但其实也是相当于创建了新流，只是数据和原先的一样。**<br />那为什么原先的流就结束了呢？ 或者说怎么就结束了呢?<br />先抛开异常处理的话题，回到最基础的层面，如果要主动结束一个流，该怎么结束呢？比如一个网络连接，如果出现异常了，流结束了，该怎么释放资源呢？ 每次订阅调用的时候，都会返回一个Disposable对象，如`Disposable disposable = Flux.just(1,2,3).subscribe()`。<br />所以，如果要主动结束一个流，其实就是调用`Disposable`对象的`dispose`方法。再深入下去，就会发现，其`dispose`方法内部其实调用的是由`publisher`产生的`subscription`的`cancel`方法。只有调用`cancel`方法，才能完美的结束`publisher`并释放资源。**所以，要想结束一个流，只有调用**`**subscription**`**的**`**cancel**`**方法。**<br />所以，当出现异常时，原先的流会结束的原因，其实就是调用了`subscription`的`cancel`方法了。那是何时调用的呢，以FluxMap为例，看下源码。
```java
public void onNext(T t) {
    if (done) {
        Operators.onNextDropped(t, actual.currentContext());
        return;
    }
    R v;
    try {
        v = Objects.requireNonNull(mapper.apply(t),
                "The mapper returned a null value.");
    }
    catch (Throwable e) {
        Throwable e_ = Operators.onNextError(t, e, actual.currentContext(), s);
        if (e_ != null) {
            onError(e_);
        }
        else {
            s.request(1);
        }
        return;
    }
    actual.onNext(v);
}
```
可以看到，在数据处理`onNext()`的方法内部，通常都有类似的try-catch结构。当出现异常时，会先对异常进行处理，确定是否需要处理异常以及怎么处理，其逻辑都在`Operators.onNextError`这个方法里。若要处理异常时，则会进入onError的流程里。<br />下面重点来看看`Operators.onNextError`这个方法，它主要主要包含了两件事：

1. **这个异常要不要吃掉，当做非异常处理**：具体可以参见`Operators.onNextErrorStrategy`方法
2. **这个异常要不要往下传，即如果是严重异常时，则直接抛出；否则，对订阅进行cancel**。详见`Operators.onOperatorError`方法。

减少篇幅起见，这里只贴下`Operators.onOperatorError`的方法，其实`onNextErrorStrategy`方法也很重要，它会从context里拿出对指定异常能处理的`strategy`来进行处理，通过传入的value以及产生的error决定是否要抛出这个异常。
```java
public static Throwable onOperatorError(@Nullable Subscription subscription,
        Throwable error,
        @Nullable Object dataSignal, Context context) {

    Exceptions.throwIfFatal(error);
    if(subscription != null) {
        subscription.cancel();
    }

    Throwable t = Exceptions.unwrap(error);
    BiFunction<? super Throwable, Object, ? extends Throwable> hook =
            context.getOrDefault(Hooks.KEY_ON_OPERATOR_ERROR, null);
    if (hook == null) {
        hook = Hooks.onOperatorErrorHook;
    }
    if (hook == null) {
        if (dataSignal != null) {
            if (dataSignal != t && dataSignal instanceof Throwable) {
                t = Exceptions.addSuppressed(t, (Throwable) dataSignal);
            }
            //do not wrap original value to avoid strong references
            /*else {
            }*/
        }
        return t;
    }
    return hook.apply(error, dataSignal);
}
```
从`onOperatorError`的源码里可以看到：

1. `Exceptions.throwIfFatal`决定是否直接抛出异常还是进入`onError`流程。比如JVM的异常，例如OutOfMemory之类的，就会直接抛出。
2. 接下来，就是前面想要的答案，**异常时数据流会结束的原因，就是默认会调用**`**subscription.cancel()**`**。**
3. 最后，是对异常处理的钩子hook，可以通过传入的`onNext`的数据`dataSingal`以及异常error进行处理，例如打印日志或者进行异常的转化，甚至是吃掉异常。
<a name="U77LD"></a>
## 2、实操 —— try-catch-finally的平替
在了解了reactor中对异常处理的机制后，看看有哪些操作符可以用来代替以前命令式编程中的try-catch-finally的结构。遇到异常时，通用的方式就是**打日志、降级兜底、重试三板斧**，下面具体看看在reactor中是怎么实现的。
```java
Flux.just(1,2,3)
    .doFinally(signalType -> System.out.println("Finally: [" + signalType + "]" ))
    .map(t -> {
        if (t == 2) {
          throw new IllegalArgumentException("Exception:"+t);
        }
        return t;
    })
    .doOnError(e -> System.out.println("log: error happened with [" + e.getMessage() + "]"))
    .doFinally(signalType -> System.out.println("Finally: [" + signalType + "]" ))
    .onErrorReturn(42)
//        .onErrorResume(e -> Flux.just(11,12,13))
    .subscribe(d -> System.out.println("data:" + d), e -> System.out.println("ERROR:" + e.getMessage()), () -> System.out.println("Completed!"));
```
这是一个使用示例，其输出如下，下面会对这些操作符进行介绍。
```java
data:1
Finally: [cancel]
log: error happened with [Exception:2]
data:42
Completed!
Finally: [onError]
```
<a name="HiJeR"></a>
### 1、降级兜底 - `onErrorReturn`/`onErrorResume`
当遇到异常的时候，可以使用`onErrorReturn`来处理，返回一个默认值，其底层实现其实用的还是`onErrorResume`。相比`onErrorReturn`只能返回一个默认值而言，`onErrorResume`更灵活，它可以根据不同的error类型，还实现不同的返回值，其`fallback`函数的入参是异常类型，返回的则是一个`Publisher`。所以从返回值也可以看出来，`onErrorReturn`/`onErrorResume`返回的是一个新的流，旧的流已经在发生异常的时候就结束了。
```java
public final Flux<T> onErrorResume(Predicate<? super Throwable> predicate,
        Function<? super Throwable, ? extends Publisher<? extends T>> fallback) {
    Objects.requireNonNull(predicate, "predicate");
    return onErrorResume(e -> predicate.test(e) ? fallback.apply(e) : error(e));
}
```
这是`onErrorResume`的实现，其提供了特别灵活的处理方式，predicate决定是否要进行fallback，fallback下可以根据异常类型返回任意的数据流。假如不需要对异常fallback，即predicate为false时，则直接返回FluxError的封装，进入onError阶段。<br />再补充一点，既然fallback是返回的一个新的流，那么即可以fallback返回一个单值，例如onErrorReturn那样，也可以返回多个值的数据流，例如：
```java
Flux.just(1,2,3)
    .map(t -> {
        if (t == 2) {
          throw new IllegalArgumentException("Exception:"+t);
        }
        return t;
    })
   .onErrorResume(e -> Flux.just(11,12,13))
   .subscribe(d -> System.out.println("data:" + d), e -> System.out.println("ERROR:" + e.getMessage()), () -> System.out.println("Completed!"));
```
输出结果会是：1 11 12 13。<br />其实，对于Flux而言，因为是多值的数据流，其很难根据异常error就进行合适的兜底，因为兜底往往取决于输入，而异常的时候往往丢失了数据data的信息了，所以**对于Flux的降级，**`**onErrorReturn**`**/**`**onErrorResume**`**实用性不是太强，因为返回的新的流很难替代旧的流，甚至你都不知道旧的Flux流有多少数据量。**<br />相比之下，`onErrorReturn`/`onErrorResume`用于单值的Mono就显得更为合适了。由于用法相同，这里就不过多赘述了。<br />所以，对于Flux流的降级兜底是个很困难的事情，有一种方式可以让`onErrorReturn`/`Resume`获取到当前的数据data，那就是利用前面小节中说到的，增加`Operators.onOperatorError`中的`onOperatorErrorHook`，把数据data塞入异常中再返回给`onErrorReturn`/`Resume`。<br />最后再看下`FluxOnErrorResume`是怎么实现降级的。
```java
public void onError(Throwable t) {
    if (!second) {
        second = true;

        Publisher<? extends T> p;

        try {
            //nextFactory即fallback函数
            p = Objects.requireNonNull(nextFactory.apply(t),
            "The nextFactory returned a null Publisher");
        }
        catch (Throwable e) {
            Throwable _e = Operators.onOperatorError(e, actual.currentContext());
            _e = Exceptions.addSuppressed(_e, t);
            actual.onError(_e);
            return;
        }
        //重新订阅一个新的流，其source就是fallback函数产生的publisher
        p.subscribe(this);
    }
    else {
        actual.onError(t);
    }
}
```
当上游出现异常时，例如先前示例中的FluxMap，就会进入onError阶段，此时正好被onErrorResume的onError阶段拦截，然后利用`fallback`函数产生新的流，再重新订阅`p.subscribe(this)`。**另外，也可以看出，新的流只会作用于onErrorResume之后的operator，前面的operator则不会有作用。**
<a name="iqQ3t"></a>
### 2、打印日志 - doOnError
打印日志就比较简单了，可以用doOnError方法来实现。doOnError的底层则用的FluxPeek来实现，其作用是覆写了所有的接口，如onNext,onError, cancel等，通过覆写来实现hook。几乎所有doOnXXX的方法都是依赖FluxPeek实现的，例如log、doOnNext、doOnError等等。<br />由于与本次的主题无关，不再赘述，感兴趣的可以自行翻看FluxPeek的实现。<br />**需要注意的是：虽然doOnXXX主要用于打印日志，但如果doOnXXX内部出错，也会导致整个流结束，进入onError阶段。所以，也是有副作用的，仍然在主流程中。**
<a name="GEteE"></a>
### 3、finally - `doFinally`
try-catch-finally中的关键字finally可以通过方法`doFinally`来平替。需要注意的是`doFinally`方法的执行顺序以及触发时机。<br />通常，finally的含义是保证100%被执行，也就是出错`onError`的时候执行，正常结束`onComplete`也执行。但在reactor中，除了这两个事件外，还未能保证doFinally百分之百执行，还需要增加cancel的情况。**其原因是，当出现异常后，对于异常的上游会走cancel流程，下游则走onError流程**。如先前的示例，触发doFinally的信号分别是：cancel与onError。<br />最后再说一下执行顺序，如先前的示例中那样，`doFinally`并不是按出现的顺序执行，也不是一定是在最后执行的（这个区别与finally关键词差别很大）。**其原因在于，当出现异常时，会先cancel掉原先的数据流，再调用onError处理（可以参见前面FluxMap的源码）**。<br />所以，示例中，"Finally: [cancel]"会先被打印，然后才是onErrorReturn的执行，即进入onError阶段。<br />**那为什么第二个**`**doFinally**`**虽然出现在**`**onErrorReturn**`**之前，但又是最后执行的呢？**<br />这是因为在实现doFinally的时候，先调用了下游的onError方法，再执行自身`doFinally`的方法，参见`FluxDoFinally`的实现：
```java
public void onError(Throwable t) {
    try {
        actual.onError(t);
    }
    finally {
        runFinally(SignalType.ON_ERROR);
    }
}
```
这样就能符合try-catch-finally的执行顺序了。<br />**所以，**`**doFinally**`**出现的位置很重要，若出现在异常前面，就会优先执行（不会像**`**finally**`**那样最后执行），若出现在异常后面，则会最后执行（类似**`**finally**`**）。**
<a name="nh6ly"></a>
### 4、try-with-resource
对于try-with-resource，reactor也给了替代的实现，那就是using操作符:
```java
public static <T, D> Flux<T> using(Callable<? extends D> resourceSupplier, Function<? super D, ? extends
        Publisher<? extends T>> sourceSupplier, Consumer<? super D> resourceCleanup, boolean eager) {
    return onAssembly(new FluxUsing<>(resourceSupplier,
            sourceSupplier,
            resourceCleanup,
            eager));
}
```
其中`resourceSupplier`是创建生成资源的函数，`sourceSupplier`则是针对生成的`resource`进行操作并产生数据流，`resourceCleanup`则是在结束后（不管成功还是失败）进行资源的释放。<br />以try-with-resource为例：
```java
try (SomeAutoCloseable disposableInstance = new SomeAutoCloseable()) {
  return disposableInstance.toString();
}
```
利用`using`函数，则可以写成：
```java
Flux.using(
        () -> new SomeAutoCloseable(), 
        disposableInstance -> Flux.just(disposableInstance.toString()), 
        AutoCloseable::close 
);
```
<a name="ibO8c"></a>
### 5、重试 - `retry` / `retryWhen`
除了以上方式处理异常时，还有一种常见的方式就是重试。比如，调用某个接口超时时，通常会重试一次，这个时候可以使用`retry`方法，如：
```java
Flux.interval(Duration.ofMillis(250))
    .map(input -> {
        if (input < 3) return "tick " + input;
        throw new RuntimeException("boom");
    })
    .retry(1)
    .subscribe(System.out::println, System.err::println);
```
会对Flux流执行两次，其结果是：0 1 2 0 1 2，即当遇到data为3时，会重试一次。<br />其基本思想很简单，就是拦截`onError`流程，计算重试的次数，如果重试未超过，则重新订阅：
```java
public void onError(Throwable t) {
    long r = remaining;
    if (r != Long.MAX_VALUE) {
        if (r == 0) {
            actual.onError(t);
            return;
        }
        remaining = r - 1;
    }
    resubscribe();
}
```
这里的remaining就是可以重试的次数，直到重试为0，再一次进入`actual.onError`。重新订阅的方法也很简单，就是把上游的source与下游的actual，再来一次subscribe：`source.subscribe(actual)`。<br />除了retry外，还有一个高级版本`retryWhen`，它除了能像retry那样重试固定的次数外，还能支持各种重试策略，由于retryWhen的源码相对复杂，这里不再叙述（毕竟本文不是源码解读），但除了重试策略有区别外，其重试的机制还是一样的，把上游与下游重新订阅。
<a name="qC8Y3"></a>
### 6、检查异常处理
在java中有一类异常是需要显示进行处理的，那就是检查异常（Checked Exception)，如IOException。在命令式编程中，可以通过throws关键字来声明，从而可以把异常往外抛，而不需要立即处理。**然而，遗憾的是，在reactor中，并没有类似的平替，不管任何情况，当遇到检查异常，reactor中都需要用try-catch来处理，这是唯一一个在reactor中没有找到命令式编程中的平替。**<br />与命令式编程有throws关键字声明不同，reactor中处理检查异常都必须用try-catch来处理，处理的方式有以下三种：

1. 捕获到异常并从中恢复。序列继续正常的进行。
2. 捕获异常，将其封装成一个 _不检查_ 的异常，然后将其抛出（中断序列）。
3. 如果需要返回一个 Flux（例如，在 flatMap 中），那么就用一个产生错误的 Flux 来封装异常，如下所示：`return Flux.error(checkedException)`。（这个序列也会终止。）

这三种方式中，其中最常见也最常用的方式就是第二种，将检查异常转化为非检查异常，如`throw new RuntimeException(e)`。但是reactor提供了辅助工具类Exceptions，进而可以相对优雅简洁的进行统一处理。<br />如以下这个例子（[**来自reactor的文档**](https://projectreactor.io/docs/core/release/reference/index.html#error.handling)）：
```java
public String convert(int i) throws IOException {
    if (i > 3) {
        throw new IOException("boom " + i);
    }
    return "OK " + i;
}
Flux<String> converted = Flux
    .range(1, 10)
    .map(i -> {
        try { return convert(i); }
        catch (IOException e) { throw Exceptions.propagate(e); }
    });
converted.subscribe(
    v -> System.out.println("RECEIVED: " + v),
    e -> {
        if (Exceptions.unwrap(e) instanceof IOException) {
            System.out.println("Something bad happened with I/O");
        } else {
            System.out.println("Something bad happened");
        }
    }
);
```
由于`convert`声明了检查异常IOException，所以必须要try-catch住，再利用`Exceptions.propagate`来封装为非检查异常。相比于直接用`throw new RuntimeException(e)`，利用Exceptions的好处在onError处理阶段可以用`Exceptions.unwrap()`方法来获取内部真实抛出的异常，体现了利用工具类的好处——简洁明了。
<a name="D1p5P"></a>
## 3、总结
本文先从reactor异常处理的底层机制讲起，讲清楚了一个基本概念：只要出现异常，不管如何处理，旧的流都已经结束，接下来处理的都是新的流。在这基础上，按命令式编程中的try-catch-finally的方式，用reactor的方式进行了一一替代介绍，希望通过对比的方式，能更好的掌握在reactor中如何优雅的处理异常。
<a name="Kn1UB"></a>
## 参考资料
reactor的文档: [_https://projectreactor.io/docs/core/release/reference/index.html#error.handling_](https://projectreactor.io/docs/core/release/reference/index.html#error.handling)
