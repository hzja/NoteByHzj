Java AQS<br />Java中AQS是`AbstractQueuedSynchronizer`类，AQS依赖FIFO队列来提供一个框架，这个框架用于实现锁以及锁相关的同步器，比如信号量、事件等。<br />在AQS中，主要有两部分功能，一部分是操作`state`变量，第二部分是实现排队和阻塞机制。<br />注意，AQS并没有实现任何同步接口，它只是提供了类似`acquireInterruptible`的方法，调用这些方法可以实现锁和同步器。
<a name="bKZHs"></a>
## 管程模型
Java使用MESA管程模型来管理类的成员变量和方法，让这个类的成员变量和方法的操作是线程安全的。下图是MESA管程模型，里面除了定义共享变量外，还定义了条件变量和条件变量等待队列：<br />![2021-07-24-18-38-17-966757.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123414154-3285fa8c-100b-4a80-95da-7e223b9c7ff9.png#clientId=u521f3504-dd57-4&from=ui&id=u31918638&originHeight=610&originWidth=726&originalType=binary&ratio=1&size=78513&status=done&style=shadow&taskId=u770a1d2f-022a-4f66-bf5e-3d5e3ad98e9)<br />Java中的MESA模型有一点改进，就是管程内部只有一个条件变量和一个等待队列。下图是AQS的管程模型：<br />![2021-07-24-18-38-18-178755.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123422382-8051cb40-3b05-4427-8226-01581467feb1.png#clientId=u521f3504-dd57-4&from=ui&id=u5a1f1171&originHeight=548&originWidth=748&originalType=binary&ratio=1&size=52908&status=done&style=shadow&taskId=ufc6479db-2ae9-4fcf-a3d0-787ac3910fa)<br />AQS的管程模型依赖AQS中的FIFO队列实现入口等待队列，而`ConditionObject`则实现了条件队列，这个队列可以创建多个。本文主要讲解入口等待队列获取锁的几种方式。
<a name="Lpsgl"></a>
## 获取独占锁
<a name="aO9Df"></a>
### 独占, 忽略`interrupts`
```java
public final void acquire(int arg) {
    if (!tryAcquire(arg) &&
        acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        selfInterrupt();
}
```
这里的`tryAcquire`是抽象方法，有AQS的子类来实现，因为每个子类实现的锁是不一样的。
<a name="do1JS"></a>
#### 入队
上面的代码可以看到，获取锁失败后，会先执行`addWaiter`方法加入队列，然后执行`acquireQueued`方法自旋地获取锁直到成功。<br />`addWaiter`代码逻辑如下图，简单说就是把`node`入队，入队后返回`node`参数给`acquireQueued`方法：<br />![2021-07-24-18-38-18-296111.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123431923-e968e3d2-a52b-461d-9a9e-086c77392157.png#clientId=u521f3504-dd57-4&from=ui&id=uc5a1b172&originHeight=393&originWidth=958&originalType=binary&ratio=1&size=48561&status=done&style=shadow&taskId=ub0c197d9-b33d-491f-990d-57ee99484ba)<br />这里有一个点需要注意，如果队列为空，则新建一个Node作为队头。
<a name="Xf8ou"></a>
#### 入队后获取锁
`acquireQueued`自旋获取锁逻辑如下图：<br />![2021-07-24-18-38-18-682066.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123440356-88cd5dae-9131-4b89-9486-53ea76414db5.png#clientId=u521f3504-dd57-4&from=ui&id=u67f69140&originHeight=529&originWidth=496&originalType=binary&ratio=1&size=51429&status=done&style=shadow&taskId=uf653dafa-b415-4aa6-8e8c-1e317937a82)<br />这里有几个细节：

1. `waitStatus`
- `CANCELLED(1)`：当前节点取消获取锁。当等待超时或被中断(响应中断)，会触发变更为此状态，进入该状态后节点状态不再变化。
- `SIGNAL(-1)`：后面节点等待当前节点唤醒。
- `CONDITION(-2)`：`Condition`中使用，当前线程阻塞在`Condition`，如果其他线程调用了`Condition`的`signal`方法，这个结点将从等待队列转移到同步队列队尾，等待获取同步锁。
- `PROPAGATE(-3)`：共享模式，前置节点唤醒后面节点后，唤醒操作无条件传播下去。
- `0`：中间状态，当前节点后面的节点已经唤醒，但是当前节点线程还没有执行完成。
2. 获取锁失败后挂起

如果前置节点不是头节点，或者前置节点是头节点但当前节点获取锁失败，这时当前节点需要挂起，分三种情况，<br />前置节点`waitStatus=-1`，如下图：<br />![2021-07-24-18-38-18-867086.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123451201-f0e92431-f83b-4131-9980-e763da66e228.png#clientId=u521f3504-dd57-4&from=ui&id=ue408112d&originHeight=172&originWidth=508&originalType=binary&ratio=1&size=21203&status=done&style=shadow&taskId=u2309edfc-8e4c-46f0-9f82-f630e895982)<br />前置节点`waitStatus > 0`，如下图：<br />![2021-07-24-18-38-19-071076.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123461763-1367acc1-32ae-4da3-a1fe-4f52bfa656d8.png#clientId=u521f3504-dd57-4&from=ui&id=u7237795c&originHeight=377&originWidth=578&originalType=binary&ratio=1&size=47044&status=done&style=shadow&taskId=ubb8ac1f3-1a37-4fde-b253-40d492e567a)<br />前置节点`waitStatus < 0` 但不等于 -1，如下图：<br />![2021-07-24-18-38-19-411147.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123471424-66f8178f-2e2c-47a2-b800-e6c9036736cc.png#clientId=u521f3504-dd57-4&from=ui&id=u1ee6d6b9&originHeight=305&originWidth=553&originalType=binary&ratio=1&size=34962&status=done&style=shadow&taskId=u9bd5a296-ef90-4834-8f92-b5ce991468f)

3. 取消获取锁

如果获取锁抛出异常，则取消获取锁，如果当前节点是tail节点，分两种情况如下图：<br />![2021-07-24-18-38-19-810161.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123480574-8f6ecf9d-08e9-4f0a-986d-5f00a0fa8c0c.png#clientId=u521f3504-dd57-4&from=ui&id=u0840920b&originHeight=776&originWidth=631&originalType=binary&ratio=1&size=70598&status=done&style=shadow&taskId=u49c85fe6-14dd-48b2-9335-a236be39d04)<br />如果当前节点不是tail节点，也分两种情况，如下图：<br />![2021-07-24-18-38-20-108199.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123490068-00b72e41-818a-4dd8-89c4-5b5bccdbf488.png#clientId=u521f3504-dd57-4&from=ui&id=u1f681e8b&originHeight=774&originWidth=758&originalType=binary&ratio=1&size=81080&status=done&style=shadow&taskId=u8d5f3e30-ef78-404c-b266-e0f6b357e6b)

4. 对中断状态忽略
5. 如果前置节点的状态是 0 或 PROPAGATE，会被当前节点自旋过程中更新成-1，以便之后通知当前节点。
<a name="K3Gfj"></a>
### 独占 + 响应中断
对应方法`acquireInterruptibly(int arg)`。<br />跟忽略中断(`acquire`方法)不同的是要响应中断，下面两个地方响应中断：

- 获取锁之前会检查当前线程是否中断。
- 获取锁失败入队，在队列中自旋获取锁的过程中也会检查当前线程是否中断。

如果检查到当前线程已经中断，则抛出`InterruptedException`，当前线程退出。
<a name="sZg88"></a>
### 独占 + 响应中断 + 考虑超时
对应方法`tryAcquireNanos(int arg, long nanosTimeout)`。<br />这个方法具备了独占 + 响应中断 + 超时的功能，下面2个地方要判断是否超时：

- 自旋获取锁的过程中每次获取锁失败都要判断是否超时
- 获取锁失败park之前要判断超时时间是否大于自旋的阈值时间`(spinForTimeoutThreshold = 1ns)`

另外，park线程的操作使用`parkNanos`传入阻塞时间。
<a name="DGT0y"></a>
## 释放独占锁
独占锁释放分两步：释放锁，唤醒后继节点。<br />释放锁的方法 `tryRelease` 是抽象的，由子类去实现。<br />看一下唤醒后继节点的逻辑，首先需要满足两个条件：

- `head`节点不等于 null
- `head`节点`waitStatus`不等于0

这里有两种情况(在方法`unparkSuccessor`)：

- 情况一，后继节点`waitStatus <= 0`，直接唤醒后继节点，如下图：

![2021-07-24-18-38-20-267502.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123503919-2b39d38b-467f-4bb9-950b-4303291ca2cc.png#clientId=u521f3504-dd57-4&from=ui&id=u4168f4ca&originHeight=294&originWidth=296&originalType=binary&ratio=1&size=14525&status=done&style=shadow&taskId=u3628c20e-61c0-494e-8f51-ffb24ca9c85)

- 情况二：后继节点为空或者`waitStatus > 0`，从后往前查找最接近当前节点的节点进行唤醒，如下图：

![2021-07-24-18-38-20-598057.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123515699-4a79d756-e9b5-4900-b973-ad717edb7a57.png#clientId=u521f3504-dd57-4&from=ui&id=u187679e2&originHeight=293&originWidth=346&originalType=binary&ratio=1&size=15728&status=done&style=shadow&taskId=ua9ee6039-3004-4db1-866f-9105415eeee)
<a name="iuvB1"></a>
## 获取共享锁
之前讲了独占锁，下来谈共享锁，有什么不同呢？
<a name="LsiY4"></a>
### 共享，忽略`interrupts`
对应方法`acquireShared`，代码如下：
```java
public final void acquireShared(int arg) {
    if (tryAcquireShared(arg) < 0)
        doAcquireShared(arg);
}
```
<a name="L5iP7"></a>
#### `tryAcquireShared`
这里获取锁使用的方法是`tryAcquireShared`，获取的是共享锁。获取共享锁跟获取独占锁不同的是，会返回一个整数值，说明如下:

- 返回负数：获取锁失败。
- 返回0：获取锁成功但是之后再由线程来获取共享锁时就会失败。
- 返回正数:获取锁成功而且之后再有线程来获取共享锁时也可能会成功。所以需要把唤醒操作传播下去。

`tryAcquireShared`获取锁失败后(返回负数)，就需要入队后自旋获取，也就是执行方法`doAcquireShared`。
<a name="R08Vq"></a>
#### `doAcquireShared`
怎么判断队列中等待节点是在等待共享锁呢？`nextWaiter == SHARED`，这个参数值是入队新建节点的时候构造函数传入的。<br />自旋过程中，如果获取锁成功(返回正数)，首先把自己设置成新的head节点，然后把通知传播下去。如下图：<br />![2021-07-24-18-38-20-894075.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123527113-85817209-fad3-455d-a166-db92e809bc64.png#clientId=u521f3504-dd57-4&from=ui&id=uab8e3d9f&originHeight=333&originWidth=356&originalType=binary&ratio=1&size=15747&status=done&style=shadow&taskId=udb24335c-e928-4247-a45f-07fab599e07)<br />之后会唤醒后面节点并保证唤醒操作可以传播下去。但是需要满足四个条件中的一个：

- `tryAcquireShared`返回值大于0，有多余的锁，可以继续唤醒后继节点
- 旧的head节点`waitStatus < 0`，应该是其他线程释放共享锁过程中把它的状态更新成了-3
- 新的hade节点`waitStatus < 0`，只要不是tail节点，就可能是-1

这里会造成不必要的唤醒，因为唤醒后获取不到锁只能继续入队等待

- 当前节点的后继节点是空或者非空但正在等待共享锁

唤醒后面节点的操作，其实就是释放共享锁，对应方法是`doReleaseShared`，见释放共享锁一节。
<a name="Hih4d"></a>
#### 共享 + 响应中断
对应方法`acquireSharedInterruptibly(int arg)`。<br />跟共享忽略中断(`acquireShared`方法)不同的是要响应中断，下面两个地方响应中断：

- 获取锁之前会检查当前线程是否中断。
- 获取锁失败入队，在队列中自旋获取锁的过程中也会检查当前线程是否中断。

如果检查到当前线程已经中断，则抛出`InterruptedException`，当前线程退出。
<a name="bmofy"></a>
#### 共享 + 响应中断 + 考虑超时
对应方法`tryAcquireSharedNanos(int arg, long nanosTimeout)`。<br />这个方法具备了共享 + 响应中断 + 超时的功能，下面2个地方要判断是否超时：

- 自旋获取锁的过程中每次获取锁失败都要判断是否超时
- 获取锁失败park之前要判断超时时间是否大于自旋的阈值时间(`spinForTimeoutThreshold = 1ns`)

另外，park线程的操作使用`parkNanos`传入阻塞时间。
<a name="YPJIG"></a>
## 释放共享锁
释放共享锁代码如下：
```java
public final boolean releaseShared(int arg) {
    if (tryReleaseShared(arg)) {
        doReleaseShared();
        return true;
    }
    return false;
}
```
首先尝试释放共享锁，`tryReleaseShared`代码由子类来实现。释放成功后执行AQS中的`doReleaseShared`方法，是一个自旋操作。<br />自旋的条件是队列中至少有两个节点，这里分三种情况。<br />情况一：当前节点`waitStatus`是-1，如下图：<br />![2021-07-24-18-38-21-105064.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123537679-38edeafb-cbf7-490c-8489-b822e4776946.png#clientId=u521f3504-dd57-4&from=ui&id=ub7431b64&originHeight=340&originWidth=549&originalType=binary&ratio=1&size=30292&status=done&style=shadow&taskId=u6219d7d9-8964-4028-8839-0b51645fbfe)<br />情况二：当前节点`waitStatus`是0(被其他线程更新新成了中间状态)，如下图：<br />![2021-07-24-18-38-21-385061.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123545872-e3601d5c-5bda-4787-8858-a5173037d93d.png#clientId=u521f3504-dd57-4&from=ui&id=ub9d29777&originHeight=279&originWidth=439&originalType=binary&ratio=1&size=16611&status=done&style=shadow&taskId=u1a0eaaea-dd6e-4b58-aaf3-aeb5636b25e)<br />情况三：当前节点`waitStatus`是-3，为什么会这样呢？需要解释一下，head节点唤醒后继节点之前`waitStatus`已经被更新中间态0了，唤醒后继节点动作还没有执行，又被其他线程更成了-3，也就是其他线程释放锁执行了上面情况二。这时需要先把`waitStatus`再更成0(在方法`unparkSuccessor`)，如下图：<br />![2021-07-24-18-38-21-528098.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627123555666-ab2f934a-3498-4a22-9450-e23fd48a6f22.png#clientId=u521f3504-dd57-4&from=ui&id=u2ca928d3&originHeight=311&originWidth=378&originalType=binary&ratio=1&size=16526&status=done&style=shadow&taskId=u115d6ec6-71b0-491f-bb15-e560f373547)
<a name="zinwc"></a>
## 抽象方法
上面的讲解可以看出，如果要基于AQS来实现并发锁，可以根据需求重写下面四个方法来实现，这四个方法在AQS中没有具体实现：

- `tryAcquire(int arg)`：获取独占锁
- `tryRelease(int arg)`：释放独占锁
- `tryAcquireShared(int arg)`：获取共享锁
- `tryReleaseShared(int arg)`：释放共享锁

AQS的子类需要重写上面的方法来修改state值，并且定义获取锁或者释放锁时`state`值的变化。子类也可以定义自己的`state`变量，但是只有更新AQS中的state变量才会对同步起作用。<br />还有一个判断当前线程是否持有独占锁的方法 `isHeldExclusively`，也可以供子类重写后使用。
<a name="i32BF"></a>
## 总结
AQS使用FIFO队列实现了一个锁相关的并发器模板，可以基于这个模板来实现各种锁，包括独占锁、共享锁、信号量等。<br />AQS中，有一个核心状态是`waitStatus`，这个代表节点的状态，决定了当前节点的后续操作，比如是否等待唤醒，是否要唤醒后继节点。
