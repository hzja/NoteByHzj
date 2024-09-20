Java Random 随机数生成不安全，如果同时泄漏第一个和第二个随机数，那么后面的随机数序列可以被破解。<br />Java Random类使用线性同余生成器（Linear Congruential Generator）算法来生成伪随机数。所谓伪随机数是指，如果使用相同的种子（seed）来生成随机数序列，那么得到的结果将是一样的。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700140478305-fc3e3ec1-8f3d-4426-9195-4270e3b0434b.png#averageHue=%23f8f7f7&clientId=u5df801b8-37af-4&from=paste&height=334&id=uf3d4bdae&originHeight=836&originWidth=2045&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=209594&status=done&style=shadow&taskId=u90deb20b-4f0b-48ed-a4fe-f058afa866f&title=&width=818)
<a name="MgjSQ"></a>
## Random种子相同，随机序列则相同
举个例子，下面的代码每次生成的随机数都是相同的：
```java
Random random = new Random(0);
int cnt = 10;
for (int i = 0; i < cnt; i++) {
    System.out.println(random.nextLong());
}
```
这意味着，当设置种子为 0 时，每次运行代码得到的随机数序列将是相同的。无论是在任何时间，还是在任何设备上，以下代码生成的随机数始终保持一致。这也意味着一旦黑客获得了你的种子“seed”，他们可以预测出你所生成的所有随机数。<br />要想生成不同的随机数序列，需要使用不同的种子。<br />幸好，Java提供的Random类并没有默认将种子值 0 作为随机数的初始值。相反，它采用了一种更为复杂的方式，使用动态的随机值作为默认种子。它通过将`seedUniquifier()`与`System.nanoTime()`进行异或运算，将系统当前纳秒时间和初始种子值进行结合。通过使用纳秒值计算种子，可以确保在不同时间构建的Random对象会得到不同的种子值。这样就能够保证生成的随机数具有更高的独立性和随机性。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1700139972656-999cfca7-9a01-4cca-8cf9-0505766af160.jpeg)<br />初始种子值如何生成呢？
```java
private static final AtomicLong seedUniquifier = new AtomicLong(8682522807148012L);
private static long seedUniquifier() {
    // L'Ecuyer, "Tables of Linear Congruential Generators of
    // Different Sizes and Good Lattice Structure", 1999
    for (;;) {
        long current = seedUniquifier.get();
        long next = current * 181783497276652981L;
        if (seedUniquifier.compareAndSet(current, next))
            return next;
    }
}
```
这段代码使用`AtomicLong` 计算种子，很明显是为了保证多线程场景下创建多个Random对象时，产生不同的种子值。在`seedUniquifier`方法中，使用了`AtomicLong.compareAndSet`方法来保证每次执行该方法时生成不同的初始种子值。通过这种方法，可以确保在多线程应用中，当出现种子冲突时，可以通过CAS操作（Compare and Swap）进行重试，以确保每个线程创建的`Random`对象的初始种子值是不同的。<br />在代码中，通过将不同的初始种子值与当前系统时间的纳秒进行异或运算，可以保证每次`Random`执行都会得到不同的种子。那么，这是否能保证种子不会被黑客猜到呢？
<a name="CQh44"></a>
## Seed可以被推测
无论使用何种随机种子、进行何种精心计算和保护，都存在可能被推测出的风险。
> 攻击者将根据观察到的输出值计算出种子。在java.util.Random的情况下，这比2^48时间要短得多。怀疑者可以尝试进行一次实验，其中展示了仅通过观察两个输出值就可以在大约2^16时间内预测Random输出的情况。在现代计算机上，预测给定时刻的随机数输出的时间甚至不到一秒钟。

<a name="JUFVj"></a>
## Java Random安全性分析
**「本文展示了根据两个随机数 预测第三个随机数的算法！强烈建议大家试试这段代码，如果随机数用于安全性较高的场景，例如兑换码、Token生成等，一定要及时更换」**！
```java
private static final long multiplier = 0x5DEECE66DL;
private static final long addend = 0xBL;
private static final long mask = (1L << 48) - 1;

@Test
public void test6() {
    int a = -117510532;
    int b = -1347210525;
    int c = 2076362838;//待预测值
    long oldseed = ((long) a << 16);
    for (int i = 0; i <= 0xffff; i++) {//暴力破解种子
        long nextseed = (oldseed * multiplier + addend) & mask;
        if ((int) (nextseed >>> 16) == b) {
            oldseed = nextseed;
            break;
        }
        oldseed += 1;
    }
    int next_int = (int) (((oldseed * multiplier + addend) & mask) >>> 16);
    System.out.println(next_int == c);
}
```
对于`nextLong`和`nextDouble`方法，只需要知道任意一个随机数即可预测后续随机数；而对于`nextInt`和`nextFloat`，如果同时泄漏第一个和第二个随机数，那么后面的随机数也都是可预测的。<br />Random 生成的随机数只有48位，这意味着在随机情况下，通过有限次数的尝试，使用当今先进的CPU，就可能在有限时间内破解。对于一些服务来说，如果很长时间不重新启动，也就意味着种子值(seed)很长时间不更新，这样预测种子值将变得更加容易。<br />然而有人可能会质疑，预测种子值和推测随机值有什么问题呢？在大多数情况下，比如随机将请求路由到一台机器或随机选中一个userId等场景，即使种子值被猜测到，也不会有太大的危险。但是在某些场景下，比如使用随机数生成密码、兑换码、推广码、各种Token等，如果这些随机数的应用被攻破，可能会带来难以估量的损失。例如，如果成功推测出兑换码码池中的随机数，那么生成的兑换码就不再安全可靠，比如兑换京东购物卡。<br />那么有什么方法可以安全地使用随机数呢？
<a name="QUMPb"></a>
## `SecureRandom` 生成安全的随机数
Java提供了 **「**`**SecureRandom**`**」** 随机数生成类，可以安全的生成随机数。`SecureRandom` 相比 Random有什么优势呢？

1. Random 最多生成48位随机值，但是`SecureRandom`最多可生成128位随机值。

对于使用`Random`的情况，需要约2^48次尝试才能破解它，但是由于今天先进的CPU，实际上可能很快就能破解。而对于`SecureRandom`，则需要约2^128次尝试才能破解，即使使用今天先进的计算机，也需要花费多年的时间。所以，`SecureRandom`提供了更高的安全性。

1. `SecureRandom` 不使用固定种子值。而是从操作系统/dev/random 随机数文件中不断获取新的种子值。

操作系统会收集各种随机数据，例如按键之间的间隔等。这些随机数据会被存储在文件中，对于linux和solaris系统来说，常见的文件路径是/dev/random和/dev/urandom。将这些随机数据作为种子，用于生成随机数或执行其他加密或随机化操作<br />`SecureRandom` 它会从/dev/random文件中获取随机种子值，每次调用nextBytes()都会获取不同的随机种子值。通过这种方式，即使攻击者观察输出也无法推断出任何信息。因为随机种子一直在变化，除非他能够控制/dev/random文件的内容（这是非常不可能的）
<a name="X2sfP"></a>
## `SecureRandom` 几种策略
<a name="dIpcI"></a>
### SHA1PRNG
SHA1PRNG是一种伪随机数生成器算法，在Java SecureRandom中，它被作为Windows下默认的随机数生成算法。该算法基于SHA-1算法，但通过添加额外的步骤来提高随机性。<br />SHA1PRNG算法使用一个种子来初始化随机数生成器。SHA1PRNG算法使用SHA-1算法来计算一个哈希值，这个哈希值会被用来产生随机数。<br />在SHA1PRNG算法中使用的seed是在系统启动时就指定的。
<a name="Mg3jw"></a>
### `NativePRNGBlocking`
Java SecureRandom中，NativePRNG 算法是Linux 下默认的随机数生成算法。<br />`NativePRNGBlocking` 初始播种时使用 /dev/random 中的 20 个字节初始化内部 SHA1PRNG 实例，<br />当调用 `nextBytes()`、nextInt() 等：使用内部 SHA1PRNG 实例的输出和从 /dev/random 读取的数据的进行 异或<br />`NativePRNGBlocking`每次计算随机数需要从/dev/random文件中获取数值。当/dev/random的随机数不足时，`NativePRNGBlocking`将会被阻塞。在桌面应用程序中，/dev/random文件很少会受到阻塞，因为它可以收集用户的鼠标、点击等事件。然而，在Web程序中，由于并发度较高，生成/dev/random数据可能会出现不足的情况。<br />例如有人使用 NativePRNGBlocking 算法，在线上环境服务启动时，一直被阻塞。就是因为/dev/random数据较少，NativePRNGBlocking 初始化被阻塞。
<a name="kiQqP"></a>
### `NativePRNGNonBlocking`
为了避免获取随机数被阻塞，`NativePRNGNonBlocking`选择从/dev/urandom中获取随机数。<br />在/dev/urandom 和 /dev/random之间有一些区别。<br />/dev/random通过收集系统上的环境噪声（如硬件噪声、磁盘活动等）来生成随机数。它只在系统上具有足够的环境噪声时才能生成高质量的随机数。<br />而/dev/urandom是一个伪随机数生成器设备文件，它使用内部熵池持续生成随机数，不管系统上的环境噪声有多少。因此，/dev/urandom生成随机数的速度比/dev/random快得多。<br />总结一下，/dev/urandom生成的随机数质量稍差，但是能稳定输出。而/dev/[random](http://mp.weixin.qq.com/s?__biz=MzIyODE5NjUwNQ==&mid=2653333191&idx=1&sn=f8ca3bb3d1d0138a508de673404f3809&chksm=f387af31c4f02627b99515e746c71363f666b7fd0766cc532d7d3c655cc805aa1dde957fd9bc&scene=21#wechat_redirect)生成的随机数质量较高，但是在系统噪音较少时生成随机数据较慢，可能会阻塞部分应用。<br />因此，建议大家一般情况下使用`NativePRNGNonBlocking`来读取/dev/urandom，这样可以换取稳定的随机数据输出，虽然牺牲了一些随机数的质量。<br />使用阻塞算法可能导致线上问题。
<a name="q1rGk"></a>
## 如何使用 `SecureRandom`
SecureRandom 和 Random使用方式类似，生成对象时，指定对应的策略即可。
```java
SecureRandom secureRandom = SecureRandom.getInstance("SHA1PRNG");
SecureRandom secureRandom = SecureRandom.getInstance("NativePRNGBlocking"); 
SecureRandom secureRandom = SecureRandom.getInstance("NativePRNGNonBlocking");

secureRandom.nextLong();
```
`SecureRandom`并不是线程安全的，可以使用`synchronized`关键字同步，或者 使用ThreadLocal 为每个线程保存一个 `SecureRandom`实例。
<a name="Cxa9o"></a>
## 各种随机策略性能对比
在单线程环境下，循环100W次生成随机数。Random算法表现出最强的性能，仅需10毫秒即可完成100W次循环。相比之下，其他三种`SecureRandom`算法的耗时是Random的10倍以上。
```
Random时间:                         27
SHA1PRNG时间:                       236
NativePRNGBlocking时间:             406
NativePRNGNonBlocking时间:          351
```
<a name="kyNqm"></a>
## 总结

- 在安全性要求较高的场景中，使用 Random 生成的随机数是不安全的。如果获得了其中两个随机数的情况下，后续的随机序列可以被破解。
- 为了提高安全性，需要在一些场景中使用`SecureRandom`来生成随机数，例如在生成随机密码、兑换码和推广码等场景中。
- SecureRandom算法使用系统随机文件/dev/random和/dev/urandom来生成随机数。由于每次生成的随机数都会与系统随机文件进行异或操作，所以随机种子一直在变化，使得随机数无法被暴力破解。
- 在Web系统中，使用`NativePRNGNonBlocking`非阻塞随机算法更加适合，因为它能提供更高的性能。
- 相比其他三种算法，Random 算法具有最强的性能，其他三种算法的耗时是 Random 算法的10倍以上。
| 名称\\特性 | 随机性 | 不可预测性 | 不可重现性 | 备注 |
| --- | --- | --- | --- | --- |
| 弱伪随机数 | ✅ | ❌ | ❌ | 不可用于安全系统 |
| 强伪随机数 | ✅ | ✅ | ❌ | 可用于安全系统 |
| 真随机数 | ✅ | ✅ | ✅ | 可用于安全系统 |

**「最后安全性较高的业务场景生成随机数时，建议使用 **`**SecureRandom**`**」**，不要再用 `Random`。
```java
SecureRandom secureRandom = SecureRandom.getInstance("NativePRNGNonBlocking");
secureRandom.nextLong();
```
