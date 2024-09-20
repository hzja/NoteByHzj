Java<br />`**CompletableFuture**`是java8中新增的一个类，算是对`Future`的一种增强，用起来很方便，也是会经常用到的一个工具类，熟悉一下。
<a name="gazYP"></a>
## `CompletionStage`接口

- `CompletionStage`代表异步计算过程中的某一个阶段，一个阶段完成以后可能会触发另外一个阶段
- 一个阶段的计算执行可以是一个`Function`，`Consumer`或者`Runnable`。比如：`stage.thenApply(x -> square(x)).thenAccept(x -> System.out.print(x)).thenRun(() -> System.out.println())`
- 一个阶段的执行可能是被单个阶段的完成触发，也可能是由多个阶段一起触发
<a name="h1BCU"></a>
## `CompletableFuture`类

- 在Java8中，`CompletableFuture`提供了非常强大的`Future`的扩展功能，可以简化异步编程的复杂性，并且提供了函数式编程的能力，可以通过回调的方式处理计算结果，也提供了转换和组合 `CompletableFuture` 的方法。
- 它可能代表一个明确完成的`Future`，也有可能代表一个完成阶段（ `CompletionStage` ），它支持在计算完成以后触发一些函数或执行某些动作。
- 它实现了`Future`和`CompletionStage`接口

**常见的方法，熟悉一下：**
<a name="o8giW"></a>
### `runAsync` 和 `supplyAsync`方法
`CompletableFuture` 提供了四个静态方法来创建一个异步操作。
```java
public static CompletableFuture<Void> runAsync(Runnable runnable)
public static CompletableFuture<Void> runAsync(Runnable runnable, Executor executor)
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier)
public static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier, Executor executor)
```
没有指定`Executor`的方法会使用`ForkJoinPool.commonPool()` 作为它的线程池执行异步代码。如果指定线程池，则使用指定的线程池运行。以下所有的方法都类同。

- `runAsync`方法不支持返回值。
- `supplyAsync`可以支持返回值。

**示例代码**
```java
//无返回值
public static void runAsync() throws Exception {
    CompletableFuture<Void> future = CompletableFuture.runAsync(() -> {
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (InterruptedException e) {
        }
        System.out.println("run end ...");
    });

    future.get();
}

//有返回值
public static void supplyAsync() throws Exception {         
    CompletableFuture<Long> future = CompletableFuture.supplyAsync(() -> {
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (InterruptedException e) {
        }
        System.out.println("run end ...");
        return System.currentTimeMillis();
    });

    long time = future.get();
    System.out.println("time = "+time);
}
```
<a name="hJ0mR"></a>
### 计算结果完成时的回调方法
当`CompletableFuture`的计算结果完成，或者抛出异常的时候，可以执行特定的`Action`。主要是下面的方法：
```java
public CompletableFuture<T> whenComplete(BiConsumer<? super T,? super Throwable> action)
public CompletableFuture<T> whenCompleteAsync(BiConsumer<? super T,? super Throwable> action)
public CompletableFuture<T> whenCompleteAsync(BiConsumer<? super T,? super Throwable> action, Executor executor)
public CompletableFuture<T> exceptionally(Function<Throwable,? extends T> fn)
```
可以看到`Action`的类型是`BiConsumer<? super T,? super Throwable>`它可以处理正常的计算结果，或者异常情况。<br />`**whenComplete**`** 和 **`**whenCompleteAsync**`** 的区别：**

- `whenComplete`：是执行当前任务的线程执行继续执行 `whenComplete` 的任务。
- `whenCompleteAsync`：是执行把 `whenCompleteAsync` 这个任务继续提交给线程池来进行执行。

**示例代码**
```java
public static void whenComplete() throws Exception {
    CompletableFuture<Void> future = CompletableFuture.runAsync(() -> {
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (InterruptedException e) {
        }
        if(new Random().nextInt()%2>=0) {
            int i = 12/0;
        }
        System.out.println("run end ...");
    });

    future.whenComplete(new BiConsumer<Void, Throwable>() {
        @Override
        public void accept(Void t, Throwable action) {
            System.out.println("执行完成！");
        }

    });
    future.exceptionally(new Function<Throwable, Void>() {
        @Override
        public Void apply(Throwable t) {
            System.out.println("执行失败！"+t.getMessage());
            return null;
        }
    });

    TimeUnit.SECONDS.sleep(2);
}
```
<a name="DmSYL"></a>
### `thenApply` 方法
当一个线程依赖另一个线程时，可以使用 `thenApply` 方法来把这两个线程串行化。
```java
public <U> CompletableFuture<U> thenApply(Function<? super T,? extends U> fn)
public <U> CompletableFuture<U> thenApplyAsync(Function<? super T,? extends U> fn)
public <U> CompletableFuture<U> thenApplyAsync(Function<? super T,? extends U> fn, Executor executor)
```
```java
Function<? super T,? extends U>
```
T：上一个任务返回结果的类型<br />U：当前任务的返回值类型<br />**示例代码**
```java
private static void thenApply() throws Exception {
    CompletableFuture<Long> future = CompletableFuture.supplyAsync(new Supplier<Long>() {
        @Override
        public Long get() {
            long result = new Random().nextInt(100);
            System.out.println("result1="+result);
            return result;
        }
    }).thenApply(new Function<Long, Long>() {
        @Override
        public Long apply(Long t) {
            long result = t*5;
            System.out.println("result2="+result);
            return result;
        }
    });

    long result = future.get();
    System.out.println(result);
}
```
第二个任务依赖第一个任务的结果。
<a name="Ni3KV"></a>
### `handle` 方法
`handle` 是执行任务完成时对结果的处理。<br />`handle` 方法和 `thenApply` 方法处理方式基本一样。不同的是 `handle` 是在任务完成后再执行，还可以处理异常的任务。`thenApply` 只可以执行正常的任务，任务出现异常则不执行 `thenApply` 方法。
```java
public <U> CompletionStage<U> handle(BiFunction<? super T, Throwable, ? extends U> fn);
public <U> CompletionStage<U> handleAsync(BiFunction<? super T, Throwable, ? extends U> fn);
public <U> CompletionStage<U> handleAsync(BiFunction<? super T, Throwable, ? extends U> fn,Executor executor);
```
**示例代码**
```java
public static void handle() throws Exception{
    CompletableFuture<Integer> future = CompletableFuture.supplyAsync(new Supplier<Integer>() {

        @Override
        public Integer get() {
            int i= 10/0;
            return new Random().nextInt(10);
        }
    }).handle(new BiFunction<Integer, Throwable, Integer>() {
        @Override
        public Integer apply(Integer param, Throwable throwable) {
            int result = -1;
            if(throwable==null){
                result = param * 2;
            }else{
                System.out.println(throwable.getMessage());
            }
            return result;
        }
     });
    System.out.println(future.get());
}
```
从示例中可以看出，在 `handle` 中可以根据任务是否有异常来进行做相应的后续处理操作。而 `thenApply` 方法，如果上个任务出现错误，则不会执行 `thenApply` 方法。
<a name="Ks1ag"></a>
### `thenAccept` 消费处理结果
接收任务的处理结果，并消费处理，无返回结果。
```java
public CompletionStage<Void> thenAccept(Consumer<? super T> action);
public CompletionStage<Void> thenAcceptAsync(Consumer<? super T> action);
public CompletionStage<Void> thenAcceptAsync(Consumer<? super T> action,Executor executor);
```
**示例代码**
```java
public static void thenAccept() throws Exception{
    CompletableFuture<Void> future = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            return new Random().nextInt(10);
        }
    }).thenAccept(integer -> {
        System.out.println(integer);
    });
    future.get();
}
```
从示例代码中可以看出，该方法只是消费执行完成的任务，并可以根据上面的任务返回的结果进行处理。并没有后续的输错操作。
<a name="lv4Ab"></a>
### `thenRun` 方法
跟 `thenAccept` 方法不一样的是，不关心任务的处理结果。只要上面的任务执行完成，就开始执行 `thenAccept` 。
```java
public CompletionStage<Void> thenRun(Runnable action);
public CompletionStage<Void> thenRunAsync(Runnable action);
public CompletionStage<Void> thenRunAsync(Runnable action,Executor executor);
```
**示例代码**
```java
public static void thenRun() throws Exception{
    CompletableFuture<Void> future = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            return new Random().nextInt(10);
        }
    }).thenRun(() -> {
        System.out.println("thenRun ...");
    });
    future.get();
}
```
该方法同 `thenAccept` 方法类似。不同的是上个任务处理完成后，并不会把计算的结果传给 `thenRun` 方法。只是处理玩任务后，执行 `thenAccept` 的后续操作。
<a name="z6DE4"></a>
### `thenCombine` 合并任务
`thenCombine` 会把 两个 `CompletionStage` 的任务都执行完成后，把两个任务的结果一块交给 `thenCombine` 来处理。
```java
public <U,V> CompletionStage<V> thenCombine(CompletionStage<? extends U> other,BiFunction<? super T,? super U,? extends V> fn);
public <U,V> CompletionStage<V> thenCombineAsync(CompletionStage<? extends U> other,BiFunction<? super T,? super U,? extends V> fn);
public <U,V> CompletionStage<V> thenCombineAsync(CompletionStage<? extends U> other,BiFunction<? super T,? super U,? extends V> fn,Executor executor);
```
**示例代码**
```java
private static void thenCombine() throws Exception {
    CompletableFuture<String> future1 = CompletableFuture.supplyAsync(new Supplier<String>() {
        @Override
        public String get() {
            return "hello";
        }
    });
    CompletableFuture<String> future2 = CompletableFuture.supplyAsync(new Supplier<String>() {
        @Override
        public String get() {
            return "hello";
        }
    });
    CompletableFuture<String> result = future1.thenCombine(future2, new BiFunction<String, String, String>() {
        @Override
        public String apply(String t, String u) {
            return t+" "+u;
        }
    });
    System.out.println(result.get());
}
```
<a name="wWcxf"></a>
### `thenAcceptBoth`
当两个`CompletionStage`都执行完成后，把结果一块交给`thenAcceptBoth`来进行消耗
```java
public <U> CompletionStage<Void> thenAcceptBoth(CompletionStage<? extends U> other,BiConsumer<? super T, ? super U> action);
public <U> CompletionStage<Void> thenAcceptBothAsync(CompletionStage<? extends U> other,BiConsumer<? super T, ? super U> action);
public <U> CompletionStage<Void> thenAcceptBothAsync(CompletionStage<? extends U> other,BiConsumer<? super T, ? super U> action,     Executor executor);
```
**示例代码**
```java
private static void thenAcceptBoth() throws Exception {
    CompletableFuture<Integer> f1 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f1="+t);
            return t;
        }
    });

    CompletableFuture<Integer> f2 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f2="+t);
            return t;
        }
    });
    f1.thenAcceptBoth(f2, new BiConsumer<Integer, Integer>() {
        @Override
        public void accept(Integer t, Integer u) {
            System.out.println("f1="+t+";f2="+u+";");
        }
    });
}
```
<a name="MU42K"></a>
### `applyToEither` 方法
两个`CompletionStage`，谁执行返回的结果快，就用那个`CompletionStage`的结果进行下一步的转化操作。
```java
public <U> CompletionStage<U> applyToEither(CompletionStage<? extends T> other,Function<? super T, U> fn);
public <U> CompletionStage<U> applyToEitherAsync(CompletionStage<? extends T> other,Function<? super T, U> fn);
public <U> CompletionStage<U> applyToEitherAsync(CompletionStage<? extends T> other,Function<? super T, U> fn,Executor executor);
```
**示例代码**
```java
private static void applyToEither() throws Exception {
    CompletableFuture<Integer> f1 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f1="+t);
            return t;
        }
    });
    CompletableFuture<Integer> f2 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f2="+t);
            return t;
        }
    });

    CompletableFuture<Integer> result = f1.applyToEither(f2, new Function<Integer, Integer>() {
        @Override
        public Integer apply(Integer t) {
            System.out.println(t);
            return t * 2;
        }
    });

    System.out.println(result.get());
}
```
<a name="ZGieP"></a>
### `acceptEither` 方法
两个`CompletionStage`，谁执行返回的结果快，就用那个`CompletionStage`的结果进行下一步的消耗操作。
```java
public CompletionStage<Void> acceptEither(CompletionStage<? extends T> other,Consumer<? super T> action);
public CompletionStage<Void> acceptEitherAsync(CompletionStage<? extends T> other,Consumer<? super T> action);
public CompletionStage<Void> acceptEitherAsync(CompletionStage<? extends T> other,Consumer<? super T> action,Executor executor);
```
**示例代码**
```java
private static void acceptEither() throws Exception {
    CompletableFuture<Integer> f1 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f1="+t);
            return t;
        }
    });

    CompletableFuture<Integer> f2 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f2="+t);
            return t;
        }
    });
    f1.acceptEither(f2, new Consumer<Integer>() {
        @Override
        public void accept(Integer t) {
            System.out.println(t);
        }
    });
}
```
<a name="ZcZcU"></a>
### `runAfterEither` 方法
两个`CompletionStage`，任何一个完成了都会执行下一步的操作（Runnable）
```java
public CompletionStage<Void> runAfterEither(CompletionStage<?> other,Runnable action);
public CompletionStage<Void> runAfterEitherAsync(CompletionStage<?> other,Runnable action);
public CompletionStage<Void> runAfterEitherAsync(CompletionStage<?> other,Runnable action,Executor executor);
```
**示例代码**
```java
private static void runAfterEither() throws Exception {
    CompletableFuture<Integer> f1 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f1="+t);
            return t;
        }
    });

    CompletableFuture<Integer> f2 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f2="+t);
            return t;
        }
    });
    f1.runAfterEither(f2, new Runnable() {

        @Override
        public void run() {
            System.out.println("上面有一个已经完成了。");
        }
    });
}
```
<a name="P1ZPB"></a>
### `runAfterBoth`
两个`CompletionStage`，都完成了计算才会执行下一步的操作（Runnable）
```java
public CompletionStage<Void> runAfterBoth(CompletionStage<?> other,Runnable action);
public CompletionStage<Void> runAfterBothAsync(CompletionStage<?> other,Runnable action);
public CompletionStage<Void> runAfterBothAsync(CompletionStage<?> other,Runnable action,Executor executor);
```
**示例代码**
```java
private static void runAfterBoth() throws Exception {
    CompletableFuture<Integer> f1 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f1="+t);
            return t;
        }
    });

    CompletableFuture<Integer> f2 = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            try {
                TimeUnit.SECONDS.sleep(t);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("f2="+t);
            return t;
        }
    });
    f1.runAfterBoth(f2, new Runnable() {

        @Override
        public void run() {
            System.out.println("上面两个任务都执行完成了。");
        }
    });
}
```
<a name="b2C6o"></a>
### `thenCompose` 方法
`thenCompose` 方法允许对两个 `CompletionStage` 进行流水线操作，第一个操作完成时，将其结果作为参数传递给第二个操作。
```java
public <U> CompletableFuture<U> thenCompose(Function<? super T, ? extends CompletionStage<U>> fn);
public <U> CompletableFuture<U> thenComposeAsync(Function<? super T, ? extends CompletionStage<U>> fn) ;
public <U> CompletableFuture<U> thenComposeAsync(Function<? super T, ? extends CompletionStage<U>> fn, Executor executor) ;
```
**示例代码**
```java
private static void thenCompose() throws Exception {
    CompletableFuture<Integer> f = CompletableFuture.supplyAsync(new Supplier<Integer>() {
        @Override
        public Integer get() {
            int t = new Random().nextInt(3);
            System.out.println("t1="+t);
            return t;
        }
    }).thenCompose(new Function<Integer, CompletionStage<Integer>>() {
        @Override
        public CompletionStage<Integer> apply(Integer param) {
            return CompletableFuture.supplyAsync(new Supplier<Integer>() {
                @Override
                public Integer get() {
                    int t = param *2;
                    System.out.println("t2="+t);
                    return t;
                }
            });
        }
    });
    System.out.println("thenCompose result : "+f.get());
}
```
