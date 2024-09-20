Java
<a name="ubBon"></a>
## 引言
在并发编程中，经常需要处理多线程的任务，这些任务往往具有依赖性，异步性，且需要在所有任务完成后获取结果。Java 8 引入了 CompletableFuture 类，它带来了一种新的编程模式，能够以函数式编程的方式处理并发任务，显著提升了代码的可读性和简洁性。<br />在这里将深入探讨 CompletableFuture 的设计原理，详细介绍其 API 的使用方式，并通过具体的示例来展示其在并发任务处理中的应用。也将探讨其与 Future，CompletableFuture 以及 Java 并发包中其他工具的对比，理解何时以及为什么需要使用 CompletableFuture。一起踏上这个富有挑战性的学习之旅吧！
<a name="ffkSy"></a>
## Java 并发编程的演进
自从诞生以来，Java 就一直致力于提供强大的并发和异步编程工具。在最初的 JDK 1.4 时期，Java 开发者需要使用低级的并发控制工具，如 synchronized 和 wait/notify，这些工具虽然功能强大，但使用起来非常复杂。<br />为了简化并发编程，Java 在 JDK 1.5 中引入了JUC包，提供了一系列高级的并发控制工具，如 `ExecutorService`、`Semaphore` 和 `Future`。<br />先来看下，`Future`到底是怎么进行异步编程的
<a name="gJLvt"></a>
## Future的异步编程之旅
先看一下这个需求。
<a name="onhBC"></a>
### 一个复杂的需求
假设正在为一家在线旅行社工作，用户可以在网站上搜索并预订飞机票和酒店。以下是需要处理的一系列操作：

- 根据用户的搜索条件，查询所有可用的飞机票
- 对每一个飞机票，查询与之匹配的可用酒店
- 对每一个飞机票和酒店的组合，计算总价格
- 将所有的飞机票和酒店的组合按照价格排序
- 将结果返回给用户
<a name="uM1PK"></a>
### 实现
为了实现这个需求，首先，需要创建一个 `ExecutorService`：
```java
ExecutorService executor = Executors.newFixedThreadPool(10);
```
```java
// 1. 查询飞机票
Future<List<Flight>> futureFlights = executor.submit(() -> searchFlights(searchCondition));

List<Flight> flights;
try {
    flights = futureFlights.get();
} catch (InterruptedException | ExecutionException e) {
    // 处理异常
}

// 2. 对每个飞机票查询酒店
List<Future<List<Hotel>>> futureHotelsList = new ArrayList<>();
for (Flight flight : flights) {
    Future<List<Hotel>> futureHotels = executor.submit(() -> searchHotels(flight));
    futureHotelsList.add(futureHotels);
}

List<Future<List<TravelPackage>>> futureTravelPackagesList = new ArrayList<>();
for (Future<List<Hotel>> futureHotels : futureHotelsList) {
    List<Hotel> hotels;
    try {
        hotels = futureHotels.get();
    } catch (InterruptedException | ExecutionException e) {
        // 处理异常
    }
    
    // 3. 对每个飞机票和酒店的组合计算总价格
    for (Hotel hotel : hotels) {
        Future<List<TravelPackage>> futureTravelPackages = executor.submit(() -> calculatePrices(flight, hotel));
        futureTravelPackagesList.add(futureTravelPackages);
    }
}

List<TravelPackage> travelPackages = new ArrayList<>();
for (Future<List<TravelPackage>> futureTravelPackages : futureTravelPackagesList) {
    try {
        travelPackages.addAll(futureTravelPackages.get());
    } catch (InterruptedException | ExecutionException e) {
        // 处理异常
    }
}

// 4. 将所有的旅行套餐按照价格排序
travelPackages.sort(Comparator.comparing(TravelPackage::getPrice));

// 5. 返回结果
return travelPackages;
```
需求终于做完了（叹气声）。此时此刻，生在JDK8+的你，会不会感同身受呢。这还是在没有处理异常，没有很多业务代码的前提下。可以从上面代码最直观的看到什么？
<a name="qggtA"></a>
## 分析这趟Future异步编程之旅
从上面的 Future 的例子中，可以明显看到以下几点缺点：
<a name="iAUFk"></a>
### 回调地狱
Future 的实现必须在每一个 Future 完成后启动另一个 Future，这使得代码看起来像是在不断嵌套回调。这种方式会使得代码难以阅读和理解，特别是在涉及复杂的异步任务链时。
<a name="zU0g6"></a>
### 阻塞操作
虽然 `Future.get()` 可以得到任务的结果，但这是一个阻塞操作，它会阻止当前线程的执行，直到异步操作完成。这种设计对于要实现非阻塞的异步编程来说，是非常不理想的。
<a name="NIjMa"></a>
### 复杂的错误处理
在使用 Future 链式处理异步任务时，如果中间某个环节出现错误，错误处理的复杂性就会大大增加。需要在每个 Future 的处理过程中都增加异常处理代码，这使得代码变得更加复杂和难以维护。
<a name="dyf6X"></a>
### 无法表示任务间复杂关系
使用 Future 很难直观地表示出任务之间的依赖关系。例如，无法使用 Future 来表示某个任务需要在另外两个任务都完成后才能开始，或者表示多个任务可以并行执行但是必须在一个共同的任务之前完成。这种限制使得 Future 在处理复杂的异步任务链时变得非常困难。<br />因此，为了解决这些问题，`CompletableFuture` 被引入了 Java 8，提供了更强大和灵活的异步编程工具。
<a name="fawRi"></a>
## `CompletableFuture`的异步编程之旅
同样还是上面的例子，来看下它的实现代码：
```java
CompletableFuture.supplyAsync(() -> searchFlights())  // 1. 查询飞机票
   .thenCompose(flights -> {  // 2. 对每个飞机票查询酒店
       List<CompletableFuture<List<TravelPackage>>> travelPackageFutures = flights.stream()
               .map(flight -> CompletableFuture.supplyAsync(() -> searchHotels(flight))  // 查询酒店
                       .thenCompose(hotels -> {  // 3. 对每个飞机票和酒店的组合计算总价格
                           List<CompletableFuture<TravelPackage>> packageFutures = hotels.stream()
                                   .map(hotel -> CompletableFuture.supplyAsync(() -> new TravelPackage(flight, hotel)))
                                   .collect(Collectors.toList());

                           return CompletableFuture.allOf(packageFutures.toArray(new CompletableFuture[0]))
                                   .thenApply(v -> packageFutures.stream()
                                           .map(CompletableFuture::join)
                                           .collect(Collectors.toList()));
                       }))
               .collect(Collectors.toList());

       return CompletableFuture.allOf(travelPackageFutures.toArray(new CompletableFuture[0]))
               .thenApply(v -> travelPackageFutures.stream()
                       .flatMap(future -> future.join().stream())
                       .collect(Collectors.toList()));
   })
   .thenApply(travelPackages -> {  // 4. 将所有的旅行套餐按照价格排序
       return travelPackages.stream()
               .sorted(Comparator.comparing(TravelPackage::getPrice))
               .collect(Collectors.toList());
   })
   .exceptionally(e -> {  // 处理所有的异常
       // 处理异常
       return null;
   });
```
可能乍一看，感觉怎么比`Future`还要复杂。但是实际在业务中，它反而更加容易读懂。每一步，每一个操作都可以顺着`thenCompose`下去。
<a name="YA8f0"></a>
## 分析这趟`CompletableFuture`异步编程之旅
`CompletableFuture` 是 Java 8 中引入的，用于解决在使用 `Future` 时遇到的一些问题。它实现了 `Future` 和 `CompletionStage` 接口，并且提供了大量的方法来更好地控制和管理异步操作。来结合上面的例子来分析它的优点：
<a name="PikqG"></a>
### 链式编程
使用 `CompletableFuture` 中的 `supplyAsync` 方法来异步地开始查询航班的操作：
```java
CompletableFuture<List<Flight>> flightsFuture = CompletableFuture.supplyAsync(() -> 
    searchFlights(source, destination));
```
然后，使用 `thenCompose` 方法将查询航班和查询酒店的操作连在一起：
```java
CompletableFuture<List<TravelPackage>> travelPackagesFuture = flightsFuture.thenCompose(flights ->
    CompletableFuture.supplyAsync(() -> flights.stream()
        .map(flight -> searchHotels(flight))
        .collect(Collectors.toList())
    ));
```
<a name="b32h7"></a>
### 非阻塞操作
上述的 `thenCompose` 方法是非阻塞的，即查询酒店的操作会立即开始，而不需要等待查询航班的操作完成。
<a name="HIVoN"></a>
### 异常处理
使用 `exceptionally` 方法处理查询航班和查询酒店过程中可能出现的异常：
```java
CompletableFuture<List<TravelPackage>> travelPackagesFuture = flightsFuture.thenCompose(flights ->
    CompletableFuture.supplyAsync(() -> flights.stream()
        .map(flight -> searchHotels(flight))
        .collect(Collectors.toList())
    )).exceptionally(ex -> {
        System.out.println("失败了: " + ex);
        return new ArrayList<>();
    });
```
<a name="VDv2m"></a>
### 表示任务间复杂关系
使用 `CompletableFuture.allOf` 方法来表示所有的旅行套餐计算任务都必须在开始排序之前完成：
```java
CompletableFuture<List<TravelPackage>> sortedTravelPackagesFuture = travelPackagesFuture.thenApply(travelPackages ->
    travelPackages.stream()
        .flatMap(List::stream)
        .sorted(Comparator.comparing(TravelPackage::getPrice))
        .collect(Collectors.toList())
    );
```
<a name="GGhQ8"></a>
## `CompletableFuture`与`Future`的比较
<a name="yQg8i"></a>
### 异步执行与结果获取

- `Future` 提供了一种在未来某个时间点获取结果的方式，但它的主要问题是在获取结果时，如果结果尚未准备好，会导致阻塞。另外，使用 `isDone()` 方法进行轮询也不是一个好的选择，因为它将消耗CPU资源。
- `CompletableFuture` 提供了非阻塞的结果获取方法，`thenApply`, `thenAccept`, `thenRun` 等方法可以在结果准备好后被自动执行，这样不需要手动检查和等待结果。
<a name="cRIpw"></a>
### 链式操作

- Future 不支持链式操作，无法在 `Future` 完成后自动触发另一个任务。
- `CompletableFuture` 提供了 `thenApply`, `thenAccept`, `thenRun`, `thenCompose`, `thenCombine` 等一系列方法，用于在当前任务完成后自动执行另一个任务，形成任务链。
<a name="kbSv5"></a>
### 异常处理

- 在 `Future` 中，只能通过 `get()` 方法获取异常，但是这种方式会阻塞线程，直到任务执行完毕。
- `CompletableFuture` 提供了 `exceptionally`, `handle` 等方法，可以用这些方法在发生异常时提供备用的结果，或者对异常进行处理。
<a name="HDxrf"></a>
### 任务组合

- `Future` 并未提供任何任务组合的方式。
- `CompletableFuture` 提供了 `allOf`, `anyOf`, `thenCombine` 等方法，可以通过这些方法来表示任务间的并行关系，或者汇聚关系。
<a name="IcwJC"></a>
### 灵活的任务执行控制

- Future 在任务执行上相对较为死板，无法中途取消任务，也无法在任务结束后执行特定操作。
- `CompletableFuture` 提供了 `cancel`, `complete` 等方法，用于中途取消任务，或者提前完成任务。此外，`whenComplete` 和 `whenCompleteAsync` 方法允许在任务结束时，无论成功或失败，都可以执行特定的操作。
<a name="bw5hn"></a>
## 进阶 | 理解`CompletableFuture`原理
讲个生活中的例子：<br />可以把 CompletableFuture 想象成一家装配线生产车间。每一件零件（任务）的加工完成（Future 完成）都可能会触发下一步工作（下一步的操作），而每一步工作的完成都会通知车间（Future），以便开始下一个阶段的生产。这个过程就像一条流水线，每完成一个步骤就自动进行下一个。<br />带着这个场景，接着往下看。
<a name="est6s"></a>
### 任务链
`CompletableFuture` 的源码中，有一个内部类 `Completion`，代表了任务链中的一项任务。每当一个任务完成时，它都会尝试去完成依赖于它的任务，就像流水线上的工人完成了一部分工作后，就会把半成品传递给下一个工人。
```java
abstract static class Completion extends ForkJoinTask<Void> implements Runnable, AsynchronousCompletionTask {
    // ...
}
```
<a name="oVj21"></a>
### 结果容器
`CompletableFuture` 本身就是一个结果容器，它持有了执行的结果，包括正常的计算结果或者执行过程中出现的异常。
```java
volatile Object result; // The outcome of the computation
```
<a name="jcAEG"></a>
### 工作线程
所有的异步任务都会提交到 `ForkJoinPool.commonPool()` 中进行执行，当然也可以指定自定义的 `Executor` 来执行任务。
```java
static final ForkJoinPool ASYNC_POOL = ForkJoinPool.commonPool();
```
<a name="quMJc"></a>
### 任务触发
当一个任务完成后，`CompletableFuture` 会通过 `tryFire` 方法触发与之关联的下一个任务。这就好比工人完成了一部分工作后，通知流水线的下一位工人继续完成接下来的工作。
```java
final CompletableFuture<T> postFire(CompletableFuture<?> a, int mode) {
    // ...
    if (a != null && a.stack != null) {
        if (mode < 0)
            a.cleanStack();
        else
            a.postComplete();
    }
    if (b != null && b.stack != null) {
        if (mode < 0)
            b.cleanStack();
        else
            b.postComplete();
    }
    return null;
}
```
<a name="slJXc"></a>
## `CompletableFuture`的主要方法
`CompletableFuture`大多数方法都实现于一个`CompletionStage`接口。
<a name="Ukp6m"></a>
### `supplyAsync()`方法
这个方法用于异步执行一个供应函数，并返回一个`CompletableFuture`对象。在示例中，这个方法用于启动一个异步任务来查找航班。
```java
CompletableFuture<List<Flight>> flightsFuture = CompletableFuture.supplyAsync(() -> searchFlights(destination));
```
<a name="Fy78c"></a>
### `thenCompose()`方法
这个方法用于链接多个`CompletableFuture`对象，形成一个操作链。当一个操作完成后，`thenCompose()`方法会将操作的结果传递给下一个操作。在示例中，这个方法用于在找到航班之后查找酒店。
```java
CompletableFuture<List<Hotel>> hotelsFuture = flightsFuture.thenCompose(flights -> CompletableFuture.supplyAsync(() -> searchHotels(destination)));
```
<a name="gXeeG"></a>
### `thenCombine()`方法
这个方法用于将两个独立的`CompletableFuture`对象的结果合并为一个结果。在示例中，这个方法用于将查找航班和酒店的结果合并为一个旅行套餐。
```java
CompletableFuture<List<TravelPackage>> travelPackagesFuture = flightsFuture.thenCombine(hotelsFuture, (flights, hotels) -> createTravelPackages(flights, hotels));
```
<a name="PFLuW"></a>
### `thenAccept()`方法
这个方法在`CompletableFuture`对象完成计算后执行一个消费函数，接收计算结果作为参数，不返回新的计算值。在示例中，这个方法用于打印出所有的旅行套餐。
```java
travelPackagesFuture.thenAccept(travelPackages -> printTravelPackages(travelPackages));
```
<a name="nzi6u"></a>
### `allOf()`方法
这个方法用于将一个`CompletableFuture`对象的数组组合成一个新的`CompletableFuture`对象，这个新的`CompletableFuture`对象在数组中所有的`CompletableFuture`对象都完成时完成。在示例中，这个方法用于将每个航班与每个酒店的组合结果（也就是旅行套餐）组合在一起。
```java
CompletableFuture.allOf(packageFutures.toArray(new CompletableFuture[0]))
                .thenApply(v -> packageFutures.stream()
                        .map(CompletableFuture::join)
                        .collect(Collectors.toList()));
```
<a name="SnRDq"></a>
### `thenApply()`方法
这个方法用于对`CompletableFuture`的结果进行变换，并返回一个新的`CompletableFuture`对象。在示例中，这个方法用于将查询到的旅行套餐按照价格进行排序。
```java
.thenApply(travelPackages -> {  // 4. 将所有的旅行套餐按照价格排序
        return travelPackages.stream()
                .sorted(Comparator.comparing(TravelPackage::getPrice))
                .collect(Collectors.toList());
    })
```
<a name="k9jY1"></a>
### `exceptionally()`方法
这个方法用于处理`CompletableFuture`的异常情况。如果`CompletableFuture`的计算过程中抛出异常，那么这个方法会被调用。在示例中，这个方法用于处理查询旅行套餐过程中可能出现的任何异常。
```java
.exceptionally(e -> {  // 处理所有的异常
       // 处理异常
       return null;
   });
```
<a name="iSegw"></a>
## Java 并发编程的续章
JDK 1.5 的 Future 解决了许多并发编程的复杂性，但是它仍有一些局限性。Future 只能描述一个异步操作，并不能描述一个由多个步骤组成的异步操作。例如，当需要处理一个由多个异步操作序列组成的业务流程时，可能会发现代码被复杂的回调逻辑淹没，这就是人们常说的回调地狱。此外，Future 没有提供一种有效的方式来处理异步操作的结果，只能通过阻塞调用 `get()` 方法来获取结果。<br />为了解决这些问题，Java 在 JDK 1.8 中引入了 `CompletableFuture`。`CompletableFuture` 是 `Future` 的增强版，它不仅能表示一个异步操作，还可以通过 `thenCompose()`, `thenCombine()`, `allOf()` 等方法来描述一个由多个步骤组成的异步操作。通过这些方法，CompletableFuture 能以流畅的链式调用的方式来描述复杂的异步业务流程，这大大简化了异步编程的复杂性。
<a name="bV3Mk"></a>
## 常见面试题

- 请解释一下 Future 接口在 Java 中的用途？
- 解释一下 Future 的局限性是什么？
- 请解释一下 `CompletableFuture` 的用途以及它如何克服 `Future` 的局限性？
- 如何用 `CompletableFuture` 来表示一组并行的异步操作？
- 请解释一下 `CompletableFuture` 的 `thenApply()`，`thenCompose()`，和 `thenCombine()` 方法的作用及区别？
- 如果有一个耗时的异步操作需要执行，但是又不希望调用 `get()` 方法时阻塞，可以使用 `CompletableFuture` 的哪个方法来达到这个目的？
- 如何处理 `CompletableFuture` 的异常？
- 请解释一下 `CompletableFuture` 的工作原理？
