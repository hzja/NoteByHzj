JavaCAS<br />C A S作为并发编程必不可少的基础知识，面试时C A S也是个高频考点，所以说C A S是必知必会，一起来深入理解C A S。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652748700847-ac1e3dcb-74f0-4445-ac5c-ed4f906567a6.png#clientId=u1724366a-ca6a-4&from=paste&id=uac3934c5&originHeight=431&originWidth=642&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uec51e06e-a79d-413f-9d4d-bd5521e1ca2&title=)
<a name="x2nKi"></a>
## CAS基本概念
C A S（compareAndSwap）也叫比较交换，是一种无锁原子算法，映射到操作系统就是一条cmpxchg硬件汇编指令（**保证原子性**），其作用是让C P U将内存值更新为新值，但是有个条件，内存值必须与期望值相同，并且C A S操作无需用户态与内核态切换，直接在用户态对内存进行读写操作（**意味着不会阻塞/线程上下文切换**）。<br />它包含3个参数C A S（V，E，N），V表示待更新的内存值，E表示预期值，N表示新值，当 V值等于E值时，才会将V值更新成N值，如果V值和E值不等，不做更新，这就是一次C A S的操作。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652748700824-96cd10cc-be32-42d9-9dd7-d20cbb57a456.png#clientId=u1724366a-ca6a-4&from=paste&id=u35b61b11&originHeight=703&originWidth=740&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46d46a8c-0214-428e-bec0-242a31bd66a&title=)<br />简单说，C A S需要额外给出一个期望值，也就是认为这个变量现在应该是什么样子的，如果变量不是想象的那样，说明它已经被别人修改过了，只需要重新读取，设置新期望值，再次尝试修改就好了。
<a name="YApR3"></a>
## CAS如何保证原子性
原子性是指一个或者多个操作在C P U执行的过程中不被中断的特性，要么执行，要不执行，不能执行到一半（**不可被中断的一个或一系列操作**）。<br />为了保证C A S的原子性，C P U提供了下面两种方式

- **总线锁定**
- **缓存锁定**
<a name="Pl536"></a>
### 总线锁定
总线（B U S）是计算机组件间的传输数据方式，也就是说C P U与其他组件连接传输数据，就是靠总线完成的，比如C P U对内存的读写。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652748700761-e1a1befa-d859-4da3-a103-a9aa7b90875a.png#clientId=u1724366a-ca6a-4&from=paste&id=u42f939af&originHeight=376&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4aba07b9-d1bb-4290-a0c4-bca9db6c458&title=)<br />**总线锁定**是指C P U使用了总线锁，所谓总线锁就是使用C P U提供的LOCK#信号，当C P U在总线上输出LOCK#信号时，其他C P U的总线请求将被阻塞。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652748700841-0b284311-f946-4048-87a7-3ff2f7320280.png#clientId=u1724366a-ca6a-4&from=paste&id=u55af1917&originHeight=763&originWidth=637&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u85486160-d8e5-4aed-ad5b-d9161ca2bab&title=)
<a name="rLlLc"></a>
### 缓存锁定
总线锁定方式虽然保证了原子性，但是在锁定期间，会导致大量阻塞，增加系统的性能开销，所以现代C P U为了提升性能，通过锁定范围缩小的思想设计出了缓存行锁定（**缓存行是C P U高速缓存存储的最小单位**）。<br />所谓**缓存锁定**是指C P U对**缓存行**进行锁定，当缓存行中的共享变量回写到内存时，其他C P U会通过总线嗅探机制感知该共享变量是否发生变化，如果发生变化，让自己对应的共享变量缓存行失效，重新从内存读取最新的数据，缓存锁定是基于缓存一致性机制来实现的，因为缓存一致性机制会阻止两个以上C P U同时修改同一个共享变量（**现代C P U基本都支持和使用缓存锁定机制**）。
<a name="KDgCI"></a>
## CAS的问题
C A S和锁都解决了原子性问题，和锁相比没有阻塞、线程上下文切换、死锁，所以C A S要比锁拥有更优越的性能，但是C A S同样存在缺点。<br />C A S的问题如下

- **只能保证一个共享变量的原子操作**
- **自旋时间太长（建立在自旋锁的基础上）**
- **ABA问题**
<a name="EtdbB"></a>
### 只能保证一个共享变量原子操作
C A S只能针对一个共享变量使用，如果多个共享变量就只能使用锁了，当然如果有办法把多个变量整成一个变量，利用C A S也不错，例如读写锁中state的高低位。
<a name="W1jyy"></a>
### 自旋时间太长
当一个线程获取锁时失败，不进行阻塞挂起，而是间隔一段时间再次尝试获取，直到成功为止，这种循环获取的机制被称为自旋锁(**spinlock**)。<br />自旋锁好处是，持有锁的线程在短时间内释放锁，那些等待竞争锁的线程就不需进入阻塞状态（**无需线程上下文切换/无需用户态与内核态切换**），它们只需要等一等（**自旋**），等到持有锁的线程释放锁之后即可获取，这样就避免了用户态和内核态的切换消耗。<br />自旋锁坏处显而易见，线程在长时间内持有锁，等待竞争锁的线程一直自旋，即CPU一直空转，资源浪费在毫无意义的地方，所以一般会限制自旋次数。<br />最后来说自旋锁的实现，实现自旋锁可以基于C A S实现，先定义`lockValue`对象默认值1，1代表锁资源空闲，0代表锁资源被占用，代码如下
```java
public class SpinLock {
    
    //lockValue 默认值1
    private AtomicInteger lockValue = new AtomicInteger(1);
    
    //自旋获取锁
    public void lock(){

        // 循环检测尝试获取锁
        while (!tryLock()){
            // 空转
        }

    }
    
    //获取锁
    public boolean tryLock(){
        // 期望值1，更新值0，更新成功返回true，更新失败返回false
        return lockValue.compareAndSet(1,0);
    }
    
    //释放锁
    public void unLock(){
        if(!lockValue.compareAndSet(1,0)){
            throw new RuntimeException("释放锁失败");
        }
    }

}
```
上面定义了`AtomicInteger`类型的`lockValue`变量，`AtomicInteger`是Java基于C A S实现的`Integer`原子操作类，还定义了3个函数`lock`、`tryLock`、`unLock`<br />tryLock函数-获取锁

- **期望值1，更新值0**
- **C A S更新**
- **如果期望值与**`**lockValue**`**值相等，则**`**lockValue**`**值更新为0，返回true，否则执行下面逻辑**
- **如果期望值与**`**lockValue**`**值不相等，不做任何更新，返回false**

unLock函数-释放锁

- **期望值0，更新值1**
- **C A S更新**
- **如果期望值与**`**lockValue**`**值相等，则**`**lockValue**`**值更新为1，返回true，否则执行下面逻辑**
- **如果期望值与**`**lockValue**`**值不相等，不做任何更新，返回false**

lock函数-自旋获取锁

- **执行**`**tryLock**`**函数，返回true停止，否则一直循环**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652748700882-8bbb5fd1-f0c8-4473-809e-a2c55ced5d13.png#clientId=u1724366a-ca6a-4&from=paste&id=u8db83c93&originHeight=683&originWidth=422&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u213d745b-08e8-489c-9873-a6e5b81a186&title=)<br />从上图可以看出，只有`tryLock`成功的线程（**把**`**lockValue**`**更新为0**），才会执行代码块，其他线程个`tryLock`自旋等待`lockValue`被更新成1，`tryLock`成功的线程执行unLock（**把**`**lockValue**`**更新为1**），自旋的线程才会`tryLock`成功。
<a name="vNCml"></a>
## ABA问题
C A S需要检查待更新的内存值有没有被修改，如果没有则更新，但是存在这样一种情况，如果一个值原来是A，变成了B，然后又变成了A，在C A S检查的时候会发现没有被修改。<br />假设有两个线程，线程1读取到内存值A，线程1时间片用完，切换到线程2，线程2也读取到了内存值A，并把它修改为B值，然后再把B值还原到A值，简单说，修改次序是A->B->A，接着线程1恢复运行，它发现内存值还是A，然后执行C A S操作，这就是著名的ABA问题，但是好像又看不出什么问题。<br />只是简单的数据结构，确实不会有什么问题，如果是复杂的数据结构可能就会有问题了（**使用AtomicReference可以把C A S使用在对象上**），以链表数据结构为例，两个线程通过C A S去删除头节点，假设现在链表有A->B节点<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652748701222-363218c0-4b8e-4c90-b293-3b1e7d067825.png#clientId=u1724366a-ca6a-4&from=paste&id=u579f8954&originHeight=858&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9420899d-f75d-42fd-8b72-9ba8759753d&title=)

- 线程1删除A节点，B节点成为头节点，正要执行C A S(A,A,B)时，时间片用完，切换到线程2
- 线程2删除A、B节点
- 线程2加入C、A节点，链表节点变成A->C
- 线程1重新获取时间片，执行C A S(A,A,B)
- 丢失C节点

要解决A B A问题也非常简单，只要追加版本号即可，每次改变时加1，即A —> B —> A，变成1A —> 2B —> 3A，在Java中提供了`AtomicStampedRdference`可以实现这个方案（**面试只要问了C A S，就一定会问ABA，这块一定要搞明白**）。
