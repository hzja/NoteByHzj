Java RateLimiter
<a name="RpC8u"></a>
## 概要
为了对系统资源的保护或者在网关限制流量，一般用限流算法进行控制。Google开源工具包Guava提供了限流工具类RateLimiter，该类基于令牌桶算法实现流量限制，使用十分方便。
<a name="Nfbwb"></a>
## RateLimiter原理分析
<a name="kxV1I"></a>
### 令牌桶算法
令牌桶算法的原理是系统会以一个恒定的速度往桶里放入令牌，而如果请求需要被处理，则需要先从桶里获取一个令牌，当桶里没有令牌可取时，则拒绝服务。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080689986-c77339ca-bb0b-4ebd-90bd-8d774f12cef5.webp#align=left&display=inline&height=694&originHeight=694&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="DyvIm"></a>
### 实现原理
RateLimiter有两种限流模式，一种为稳定模式(SmoothBursty:令牌生成速度恒定)，一种为渐进模式(SmoothWarmingUp:令牌生成速度缓慢提升直到维持在一个稳定值)。以下代码基于基本 guava:26.0-jre。<br />看看关键类的继承图<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080689919-3fa8ce40-fbf8-4313-bb94-c36b46770184.webp#align=left&display=inline&height=179&originHeight=179&originWidth=1080&size=0&status=done&style=none&width=1080)
<a name="prX79"></a>
#### SmoothBursty
<a name="tiSiX"></a>
##### 限流效果
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690040-1d52b805-ad3e-499f-af21-7a8d69d8715e.webp#align=left&display=inline&height=254&originHeight=254&originWidth=1080&size=0&status=done&style=none&width=1080)<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690059-801f23c0-f7a6-4718-bc3f-cefaeeeca3f4.webp#align=left&display=inline&height=281&originHeight=281&originWidth=1080&size=0&status=done&style=none&width=1080)<br />RateLimiter.create(5.0) 表示每秒产生5个令牌。输出的意思是这次获取令牌所需要等待的时间。
<a name="2xuw5"></a>
##### 属性
继承自SmoothRateLimiter的有以下属性
```java
/** The currently stored permits. */
  double storedPermits;
  /** The maximum number of stored permits. */
  double maxPermits;
  /**
   * The interval between two unit requests, at our stable rate. E.g., a stable rate of 5 permits
   * per second has a stable interval of 200ms.
   */
  double stableIntervalMicros;
  /**
   * The time when the next request (no matter its size) will be granted. After granting a request,
   * this is pushed further in the future. Large requests push this further than small requests.
   */
  private long nextFreeTicketMicros = 0L; // could be either in the past or future
```
storedPermits - 当前桶里有多少令牌。maxPermits - 桶可以最大存储多少令牌。stableIntervalMicros - 生成一个令牌的间隔，单位微秒。nextFreeTicketMicros - 这个比较难理解，也是关键，意思是下一个请求允许获取到令牌的微秒数。
<a name="fG4AA"></a>
##### 初始化
```java
public static RateLimiter create(double permitsPerSecond) {
    return create(permitsPerSecond, SleepingStopwatch.createFromSystemTimer());
}
@VisibleForTesting
static RateLimiter create(double permitsPerSecond, SleepingStopwatch stopwatch) {
    RateLimiter rateLimiter = new SmoothBursty(stopwatch, 1.0 /* maxBurstSeconds */);
    rateLimiter.setRate(permitsPerSecond);
    return rateLimiter;
}
```
SmoothBursty的两个构造参数，一个是stopwatch，这个类的作用是能够获取从初始化时到现在的时间，另一个参数 maxBurstSeconds是 hard code 为 1。
```java
public final void setRate(double permitsPerSecond) {
    checkArgument(
        permitsPerSecond > 0.0 && !Double.isNaN(permitsPerSecond), "rate must be positive");
    //这里用了 synchronized 锁，锁的范围是这个 rateLimiter 实例。
    synchronized (mutex()) {
        doSetRate(permitsPerSecond, stopwatch.readMicros());
    }
}

@Override
final void doSetRate(double permitsPerSecond, long nowMicros) {
    //resync 方法，它的作用是计算 storedPermits，等下会讲到；
    resync(nowMicros);
    //计算 stableIntervalMicros，单位是微秒，用1秒 / 入参的令牌数，意思就是每多少微秒生成一个令牌；
    double stableIntervalMicros = SECONDS.toMicros(1L) / permitsPerSecond;
    this.stableIntervalMicros = stableIntervalMicros;
    doSetRate(permitsPerSecond, stableIntervalMicros);
}
```
setRate方法用来初始化令牌生成速率；
```java
@Override
void doSetRate(double permitsPerSecond, double stableIntervalMicros) {
    double oldMaxPermits = this.maxPermits;
    //这里涉及一个重要的属性 maxPermits，它表示桶最大的存储令牌的数量，注意maxBurstSeconds hard code为1
    maxPermits = maxBurstSeconds * permitsPerSecond;
    if (oldMaxPermits == Double.POSITIVE_INFINITY) {
        // if we don't special-case this, we would get storedPermits == NaN, below
        storedPermits = maxPermits;
    } else {
        storedPermits =
            (oldMaxPermits == 0.0)
            ? 0.0 // initial state
            : storedPermits * maxPermits / oldMaxPermits;
    }
}
```
doSetRate 是模版方法，先看 SmoothBursty 的，等下讲到 SmoothWarmingUp 时会讲它的 doSetRate。这个方法有两个地方用到，一是初始化时，二是调用 RateLimiter 的实例方法 setRate 动态调整速率时。
<a name="GOzAr"></a>
##### 延迟计算
初始化就这么简单了。可能有人在想既然是令牌桶算法，应该有个类似定时器的东东来持续往桶放令牌才对啊，如果开启一个定时器无可厚非，但如果系统需要N个不同速率的桶来针对不同的场景或用户，就会极大的消耗系统资源。<br />RateLimiter用了一种类似于延迟计算的方法，把桶里令牌数量的计算放在下一个请求中计算，即桶里的令牌数 storedPermits 不是实时更新的，而是等到下一个请求过来时才更新的，具体来看看消费令牌的过程。
<a name="wqz05"></a>
##### 获取令牌acquire
主要有两个方法，一是 acquire，一是 tryAcquire。区别是如果桶里没有令牌，前者会阻塞，后者会直接返回 false。先看看 acquire 方法
```java
@CanIgnoreReturnValue
  //这个方法主要是获取令牌的同时，返回需要等待的时间，主要就是reserve方法，至于 stopwatch.sleepMicrosUninterruptibly 大家理解为 sleep 就好了。
  public double acquire(int permits) {
    long microsToWait = reserve(permits);
    stopwatch.sleepMicrosUninterruptibly(microsToWait);
    return 1.0 * microsToWait / SECONDS.toMicros(1L);
  }
  
 final long reserve(int permits) {
    checkPermits(permits
    //这里用 synchronized 锁，所以下面的逻辑大家不用考虑由并发产生的问题；
    synchronized (mutex()) {
      //stopwatch.readMicros() 的作用是获取从初始化到现在的系统时间微秒数。
      return reserveAndGetWaitLength(permits, stopwatch.readMicros());
    }
  }
  
  //获取令牌并等待
  final long reserveAndGetWaitLength(int permits, long nowMicros) {
    long momentAvailable = reserveEarliestAvailable(permits, nowMicros);
    return max(momentAvailable - nowMicros, 0);
  }
```
reserveEarliestAvailable 是整个 RateLimiter 的核心方法，它是 SmoothRateLimite 的一个模板方法。
```java
@Override
  final long reserveEarliestAvailable(int requiredPermits, long nowMicros) {
    resync(nowMicros);
    long returnValue = nextFreeTicketMicros;
    //storedPermitsToSpend 是可以消费的令牌数，最多也就取 storedPermits 这么多了；
    double storedPermitsToSpend = min(requiredPermits, this.storedPermits);
    //freshPermits 字面意思新鲜的令牌，我们理解为还没生成的或者将来会生成的令牌。假如我要10个令牌，但是桶里现在只有5个令牌，那么 freshPermits 值为 5 = 10 - 5；
    double freshPermits = requiredPermits - storedPermitsToSpend;
    //然后就通过 freshPermits 计算出需要等待的时间 waitMicros。storedPermitsToWaitTime 是一个模板方法，对 SmoothBursty 来说这个没啥用，它始终返回 0，所以 waitMicros = freshPermits * stableIntervalMicros；
    long waitMicros =
        storedPermitsToWaitTime(this.storedPermits, storedPermitsToSpend)
            + (long) (freshPermits * stableIntervalMicros);
    //更新 nextFreeTicketMicros，追加 waitMicros；
    this.nextFreeTicketMicros = LongMath.saturatedAdd(nextFreeTicketMicros, waitMicros);
    //storedPermits 追减用掉的令牌；
    this.storedPermits -= storedPermitsToSpend;
    return returnValue;
  }
```
reserveEarliestAvailable的返回值，注意了，这里返回的是更新前的 nextFreeTicketMicros，也就是上一个请求更新的 nextFreeTicketMicros。那么这个 waitMicros 等待时间也不是当前请求需要等待的时间，而是下一个请求需要等待的时间，这个涉及到 RateLimiter 一个很重要的设计理念，就是“预消费”，通俗点理解即“前人消费，后人买单”，理解好这点，是使用和理解 RateLimiter 的关键。<br />举一个例子来助于理解，桶的速率为每秒产生5个令牌，现在桶里有4个令牌，现在过来一个请求需要10个令牌，那么这个请求会被无阻塞允许，不需要等待，同时又过来一个请求，现在桶里已经没有令牌了，而且上一个请求还“欠下”6个令牌，那么这个请求需要等待 (10 - 4) / 5 秒的时间，才被允许执行。
```java
//刚刚说的延迟计算令牌数就在这里。这个方法是用来计算 storedPermits （桶里的令牌数），nowMicros 是当前的微秒数，nextFreeTicketMicros 上面说过了。
void resync(long nowMicros) {
    // if nextFreeTicket is in the past, resync to now
    if (nowMicros > nextFreeTicketMicros) {
        double newPermits = (nowMicros - nextFreeTicketMicros) / coolDownIntervalMicros();
        //令牌数不能超过 maxPermits； 
        storedPermits = min(maxPermits, storedPermits + newPermits);
        //把 nextFreeTicketMicros 置为当前时间。  
        nextFreeTicketMicros = nowMicros;
    }
}

//coolDownIntervalMicros 是一个模板方法，看 SmoothBursty 的，值等于 stableIntervalMicros
@Override
double coolDownIntervalMicros() {
    return stableIntervalMicros;
}
```
所以resync的意思就是如果当前时间大于 nextFreeTicketMicros，就用当前时间 - nextFreeTicketMicros / 每 stableIntervalMicros 生成一个令牌，即这个时间差可以生成多少个令牌；<br />用一个图来表示会更加清晰<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690007-6e5c9356-0839-4203-bd1d-47de733e93b2.webp#align=left&display=inline&height=474&originHeight=474&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="TiFp3"></a>
##### 为什么要“预消费”
RateLimiter 它是这样想的：
> Last, but not least: consider a RateLimiter with rate of 1 permit per second, currently completely unused, and an expensive acquire(100) request comes. It would be nonsensical to just wait for 100 seconds, and /then/ start the actual task. Why wait without doing anything? A much better approach is to /allow/ the request right away (as if it was an acquire(1) request instead), and postpone /subsequent/ requests as needed. In this version, we allow starting the task immediately, and postpone by 100 seconds future requests, thus we allow for work to get done in the meantime instead of waiting idly.

大概意思是，假设令牌产生的速率为1秒一个，系统平时是很空闲的，突然来了一个 expensive acquire(100) 的请求，难道要等100秒才执行吗？这毫无意义，不能充分利用资源，所以干脆可以直接允许好了，不要做无谓的等待。简单来说就是为了突发性。
<a name="LYtjr"></a>
##### 消费场景分析
分情况分析一下就清楚了：

1. nowMicros > nextFreeTicketMicros 这种场景发生在刚初始化时，或者桶里的令牌还有剩余。
   - 如果请求所需令牌 < 桶里的 即桶里令牌满足这次消费的话，那么 nextFreeTicketMicros 会移动到 nowMicros 的位置 令牌数 storedPermits = 原来 - 消费的 + 这段时间增加的。<br />

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690059-0a9ec4a9-99b0-44fe-b97e-c37879c46158.webp#align=left&display=inline&height=517&originHeight=517&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />

   - 如果请求所需令牌 >= 桶里的 这时会优先把桶里的令牌全部拿走，那么 storedPermits 就等于0了。如果还不够，就会发生预消费，那么 nextFreeTicketMicros 会后移，移动多少？就是需要产生“溢出”令牌数的时间。

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690047-22b57830-edfb-4d58-ac8d-0b5ef68ab490.webp#align=left&display=inline&height=422&originHeight=422&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />

2. nowMicros < nextFreeTicketMicros 在上面有一个场景 nextFreeTicketMicros 会后移，移动了多少不知道，要看上一个请求，那么如果这段时间内有请求过来呢？这时当前的请求就要为上一个请求“买单”了，它需要等待到 nextFreeTicketMicros 这个时刻才能允许执行，但此时桶里令牌数是 0 的，所以这个请求也是会预消费的。
<a name="Dulw8"></a>
#### SmoothWarmingUp
SmoothBursty 是以一个固定的速率来产生令牌的，它具有突发性，这个可能适用大多数场景。而 SmoothWarmingUp 考虑的是譬如一个系统刚启动，但如果这时有大量请求过来，因为突发性，这些请求都会被允许，但此时系统可能没有那么多资源去响应，所以需要一个“热身”时间，SmoothWarmingUp 就派上用场了。它跟 SmoothBursty 的大概思路都是差不多的，只是个别地方有差别，主要就是之前提到几个模板方法。
<a name="FHCsa"></a>
##### 限流效果
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690039-706c50ea-b7c2-4711-997c-064a4181349e.webp#align=left&display=inline&height=271&originHeight=271&originWidth=1080&size=0&status=done&style=none&width=1080)<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690034-718ac8d4-4d3c-43ef-adc2-071d918cf7cd.webp#align=left&display=inline&height=301&originHeight=301&originWidth=1080&size=0&status=done&style=none&width=1080)<br />SmoothWarmingUp 的效果是刚开始产生令牌的速率比较慢，随着请求过来，会进入“热身”期，速率逐渐提升到 permitsPerSecond 这个速度；但是如果没有请求了，又会“冷却”下去，请求过来又要从“热身”开始。
<a name="2uioe"></a>
##### 初始化
初始化也是调用 create，不过参数列表有点不同
```java
//permitsPerSecond 是“热身”后的稳定速率；   
//warmupPeriod 是“热身”时间，如果这段时间内持续有请求过来消费令牌，就会达到一个稳定的速率，这时跟 SmoothBursty 效果一样；   
//unit 是 warmupPeriod 的单位；  
public static RateLimiter create(double permitsPerSecond, long warmupPeriod, TimeUnit unit) {
    checkArgument(warmupPeriod >= 0, "warmupPeriod must not be negative: %s", warmupPeriod);
    //coldFactor hard code 为 3；
    return create(
        permitsPerSecond, warmupPeriod, unit, 3.0, SleepingStopwatch.createFromSystemTimer());
}

@VisibleForTesting
static RateLimiter create(
    double permitsPerSecond,
    long warmupPeriod,
    TimeUnit unit,
    double coldFactor,
    SleepingStopwatch stopwatch) {
    RateLimiter rateLimiter = new SmoothWarmingUp(stopwatch, warmupPeriod, unit, coldFactor);
    rateLimiter.setRate(permitsPerSecond);
    return rateLimiter;
}

SmoothWarmingUp(
    SleepingStopwatch stopwatch, long warmupPeriod, TimeUnit timeUnit, double coldFactor) {
    super(stopwatch);
    this.warmupPeriodMicros = timeUnit.toMicros(warmupPeriod);
    this.coldFactor = coldFactor;
}
```
<a name="Dca6Z"></a>
##### “热身”速率函数及说明
由于接下来涉及到一些计算，先看看“热身”函数的定义及图像
```
/**
   * This implements the following function where coldInterval = coldFactor * stableInterval.
   *
   * <pre>
   *          ^ throttling
   *          |
   *    cold  +                  /
   * interval |                 /.
   *          |                / .
   *          |               /  .   ← "warmup period" is the area of the trapezoid between
   *          |              /   .     thresholdPermits and maxPermits
   *          |             /    .
   *          |            /     .
   *          |           /      .
   *   stable +----------/  WARM .
   * interval |          .   UP  .
   *          |          . PERIOD.
   *          |          .       .
   *        0 +----------+-------+--------------→ storedPermits
   *          0 thresholdPermits maxPermits
   * </pre>
   *
   * Before going into the details of this particular function, let's keep in mind the basics:
   *
   * <ol>
   *   <li>The state of the RateLimiter (storedPermits) is a vertical line in this figure.
   *   <li>When the RateLimiter is not used, this goes right (up to maxPermits)
   *   <li>When the RateLimiter is used, this goes left (down to zero), since if we have
   *       storedPermits, we serve from those first
   *   <li>When _unused_, we go right at a constant rate! The rate at which we move to the right is
   *       chosen as maxPermits / warmupPeriod. This ensures that the time it takes to go from 0 to
   *       maxPermits is equal to warmupPeriod.
   *   <li>When _used_, the time it takes, as explained in the introductory class note, is equal to
   *       the integral of our function, between X permits and X-K permits, assuming we want to
   *       spend K saved permits.
   * </ol>
   *
   * <p>In summary, the time it takes to move to the left (spend K permits), is equal to the area of
   * the function of width == K.
```
还是很简单的，说明一下。x 轴是 storedPermits，即桶里的令牌数。轴上主要刻有两个值，一是thresholdPermits，这个等下会讲到；一个是maxPermits；y 轴是生成一个令牌的间隔，单位微秒。轴上主要刻有两个值，一是stable interval；一个是 cold interval，coldInterval = coldFactor * stableInterval，由于 coldFactor hard code 为 3，所以 coldInterval 等于3倍的 stable interval。warmup period 是入参的“热身”时间。由这几个值构成的左边的长方形和右边的梯形。由于 x 轴是令牌数，y 轴是生成令牌的间隔，所以它们的乘积是一个时间。
<a name="44Ivp"></a>
##### doSetRate方法
```java
@Override
void doSetRate(double permitsPerSecond, double stableIntervalMicros) {
    double oldMaxPermits = maxPermits;
    //coldIntervalMicros - 固定 stableIntervalMicros * 3，这里的stableIntervalMicros跟SmoothBursty一样。
    double coldIntervalMicros = stableIntervalMicros * coldFactor;
    //thresholdPermits - 桶里令牌数的阈值，低于这个值之后就会进入稳定速率期；但高于这个值，又会回到“热身”期。
    thresholdPermits = 0.5 * warmupPeriodMicros / stableIntervalMicros;
    //maxPermits - 意思也是桶里允许最多的令牌
    maxPermits =
        thresholdPermits + 2.0 * warmupPeriodMicros / (stableIntervalMicros + coldIntervalMicros);
    //slope - 斜率，就是你在图形上看到那条斜线的斜率，这时用来方便已知 storedPermits 时，求出当前的 coldIntervalMicros。条件都已知了，斜率的计算不多说。  
    slope = (coldIntervalMicros - stableIntervalMicros) / (maxPermits - thresholdPermits);
    if (oldMaxPermits == Double.POSITIVE_INFINITY) {
        // if we don't special-case this, we would get storedPermits == NaN, below
        storedPermits = 0.0;
    } else {
        //还有一点注意，这里初始化时，桶里的令牌数为满，跟 SmoothBursty 不一样。
        storedPermits =
            (oldMaxPermits == 0.0)
            ? maxPermits // initial state is cold
            : storedPermits * maxPermits / oldMaxPermits;
    }
}
```
这里对几个参数的计算说明一下：

- thresholdPermits 为什么 thresholdPermits = 0.5 * warmupPeriodMicros / stableIntervalMicros？先看看官方的注释
> Assuming we have saturated demand, the time to go from maxPermits to thresholdPermits is equal to warmupPeriod. And the time to go from thresholdPermits to 0 is warmupPeriod/2. (The reason that this is warmupPeriod/2 is to maintain the behavior of the original implementation where coldFactor was hard coded as 3.)

- 根据官方的注释，说“热身”的时间是稳定时间的2倍（这里表述不准确），即梯形面积为长方形面积的2倍，要保持跟 coldFactor 写死为3一样，原因是希望令牌速率提升的幅度跟它所需要的时间的比例保持一致，因为梯形面积是已知的，又知道长方形的面积和一条边长，容易求得 thresholdPermits。
- maxPermits 为什么 maxPermits = thresholdPermits + 2.0 * warmupPeriodMicros / (stableIntervalMicros + coldIntervalMicros) ？这个简单，利用梯形面积公式求出高，然后再加上 thresholdPermits。<br />

消费令牌的主要逻辑在 reserveEarliestAvailable 方法，里面有一个模板方法 storedPermitsToWaitTime，看看 SmoothWarmingUp 的实现。
```java
@Override
long storedPermitsToWaitTime(double storedPermits, double permitsToTake) {
    //availablePermitsAboveThreshold 表示多于 thresholdPermits 的可用令牌数；
    double availablePermitsAboveThreshold = storedPermits - thresholdPermits;
    long micros = 0;
    // measuring the integral on the right part of the function (the climbing line)
    //如果 availablePermitsAboveThreshold > 0，说明还在“热身”期，令牌的数量需要控制在 thresholdPermits；
    if (availablePermitsAboveThreshold > 0.0) {
        //permitsAboveThresholdToTake 表示这次允许取的最大的令牌数；
        double permitsAboveThresholdToTake = min(availablePermitsAboveThreshold, permitsToTake);
        // TODO(cpovirk): Figure out a good name for this variable.
        //length 表示在大梯形中，以 permitsToTime(availablePermitsAboveThreshold) 为右边的底部，以 permitsToTime(availablePermitsAboveThreshold - permitsAboveThresholdToTake) 为左边的底部，构成的小梯形中，这两条边的和，用于下面的计算；
        double length =
            permitsToTime(availablePermitsAboveThreshold)
            + permitsToTime(availablePermitsAboveThreshold - permitsAboveThresholdToTake);
        //micros 就是计算小梯形的面积了，permitsAboveThresholdToTake 就是这个小梯形的高了；
        micros = (long) (permitsAboveThresholdToTake * length / 2.0);
        permitsToTake -= permitsAboveThresholdToTake;
    }
    // measuring the integral on the left part of the function (the horizontal line)
    //如果多于 thresholdPermits 的令牌数不够，那么就会进入稳定期，使用稳定的速率。
    micros += (long) (stableIntervalMicros * permitsToTake);
    return micros;
}

//permitsToTime 就是利用斜率求出 y 轴的值。
private double permitsToTime(double permits) {
    return stableIntervalMicros + permits * slope;
}
```
看一下图像就清楚了<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608080690053-e2857f37-c210-470f-be67-eb7ed52b8b9f.webp#align=left&display=inline&height=542&originHeight=542&originWidth=1080&size=0&status=done&style=none&width=1080)<br />从 storedPermitsToWaitTime 看出，SmoothWarmingUp 会优先取出超过 thresholdPermits 的令牌，但即使有令牌可用，还是会阻塞请求，以这样来防止启动时的突发性。随着请求增加，令牌的减少，桶的令牌会达到 thresholdPermits，这时就相当于“热身”完了，跟 SmoothBursty 一样。但如果一直没有请求来消费令牌，令牌数增加，就会从新进去“热身”期了。
<a name="HeCjv"></a>
##### coolDownIntervalMicros
在 resync 方法中，还有一个模板方法 coolDownIntervalMicros，在 SmoothWarmingUp 的实现中为
```java
@Override
double coolDownIntervalMicros() {
    return warmupPeriodMicros / maxPermits;
}
```
这个方法是用于得出从上一个请求到当请求的时间内，可以生成令牌的时间间隔，在 SmoothBursty 的实现中它就是 stableIntervalMicros。
<a name="ThTyc"></a>
#### setRate的公平性考虑
RateLimiter 可以动态调整产生令牌的速率，但是这里涉及一个问题，如何处理当前被阻塞的请求以及后续请求？先看看官方的注释:
```
/**
   * Updates the stable rate of this {@code RateLimiter}, that is, the {@code permitsPerSecond}
   * argument provided in the factory method that constructed the {@code RateLimiter}. Currently
   * throttled threads will <b>not</b> be awakened as a result of this invocation, thus they do not
   * observe the new rate; only subsequent requests will.
   *
   * <p>Note though that, since each request repays (by waiting, if necessary) the cost of the
   * <i>previous</i> request, this means that the very next request after an invocation to {@code
   * setRate} will not be affected by the new rate; it will pay the cost of the previous request,
   * which is in terms of the previous rate.
   *
   * <p>The behavior of the {@code RateLimiter} is not modified in any other way, e.g. if the {@code
   * RateLimiter} was configured with a warmup period of 20 seconds, it still has a warmup period of
   * 20 seconds after this method invocation.
```
注释的意思说了当前被阻塞的线程不会因此醒过来，它们对速率的改变没有感知，接下来的请求才会适应新的速率。
> Note though that, since each request repays (by waiting, if necessary) the cost of the previous request, this means that the very next request after an invocation to {@code setRate} will not be affected by the new rate; it will pay the cost of the previous request, which is in terms of the previous rate.

假设速率降低了，如果需要对当前被阻塞的请求做调整的话，那么它们的阻塞时间会增加（这里假设的结果是增加），由于连锁反应，最后导致 nextFreeTicketMicros 会后移，这就对于改变速率后的请求不公平了。所以 RateLimiter 的做法是当前阻塞的请求还是按照原来时间等待，后续的请求用新的速率，这样实现也比较简单，对后续的请求也公平。
<a name="jgwx3"></a>
#### tryAcquire
补充说明一下tryAcquire，这方法实际应用比acquire 方法还要实用。
```java
public boolean tryAcquire(int permits, long timeout, TimeUnit unit) {
    long timeoutMicros = max(unit.toMicros(timeout), 0);
    checkPermits(permits);
    long microsToWait;
    synchronized (mutex()) {
        long nowMicros = stopwatch.readMicros();
        if (!canAcquire(nowMicros, timeoutMicros)) {
            return false;
        } else {
            microsToWait = reserveAndGetWaitLength(permits, nowMicros);
        }
    }
    stopwatch.sleepMicrosUninterruptibly(microsToWait);
    return true;
}

//判断就是 canAcquire 方法，很简单，就是判断 nextFreeTicketMicros 的位置，因为你最多也就需要等待到 nextFreeTicketMicros 这么长的时间嘛。
private boolean canAcquire(long nowMicros, long timeoutMicros) {
    return queryEarliestAvailable(nowMicros) - timeoutMicros <= nowMicros;
}

@Override
final long queryEarliestAvailable(long nowMicros) {
    return nextFreeTicketMicros;
}
```
tryAcquire 会先去判断是否能够在 timeout 的等待时间内能够获取到令牌，如果可以就阻塞等待，如果不能则直接返回false。
<a name="OxrKN"></a>
## 总结
Guava 的 RateLimiter 是一个高效低耗，简单易用，优秀的限流工具，它基于令牌桶算法，并且提供了一个很好的实现参考。
