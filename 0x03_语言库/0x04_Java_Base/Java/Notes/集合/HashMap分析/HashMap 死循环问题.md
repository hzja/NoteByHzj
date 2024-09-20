Java<br />HashMap 死循环是一个比较常见、比较经典的问题，在日常的面试中出现的频率比较高，所以接下来咱们通过图解的方式，彻底理解死循环的原因。
<a name="g2bOx"></a>
## 前置知识
死循环问题发生在 JDK 1.7 版本中，造成这个问题主要是由于 HashMap 自身的运行机制，加上并发操作，从而导致了死循环。在 JDK 1.7 中 HashMap 的底层数据实现是数组 + 链表的方式，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675171050755-b2cd7d04-9181-437c-80bc-60630e19026f.png#averageHue=%23fbfbfa&clientId=u5a9cc6e8-f34b-4&from=paste&id=u70fedbea&originHeight=588&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3e6f2773-d60c-4ef0-9e1c-a37fd614f2f&title=)而 HashMap 在数据添加时使用的是头插入，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675171050707-5231d551-2980-4df3-bf1f-7bc6429339f6.png#averageHue=%23fbfaf9&clientId=u5a9cc6e8-f34b-4&from=paste&id=u5ae15cdb&originHeight=751&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80248b5c-71c1-42c0-88e8-43aef7d9c26&title=)HashMap 正常情况下的扩容实现如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675171050698-1117d4e1-2071-4193-a793-fe259cfbc0db.png#averageHue=%23fbfbf9&clientId=u5a9cc6e8-f34b-4&from=paste&id=u5a4fb935&originHeight=415&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5f8cd9cf-7e0e-40ca-b21b-f529b70e84d&title=)旧 HashMap 的节点会依次转移到新 HashMap 中，旧 HashMap 转移的顺序是 A、B、C，而新 HashMap 使用的是头插法，所以最终在新 HashMap 中的顺序是 C、B、A，也就是上图展示的那样。有了这些前置知识之后，咱们来看死循环是如何诞生的？
<a name="dRKgC"></a>
## 死循环执行步骤1
死循环是因为并发 HashMap 扩容导致的，并发扩容的第一步，线程 T1 和线程 T2 要对 HashMap 进行扩容操作，此时 T1 和 T2 指向的是链表的头结点元素 A，而 T1 和 T2 的下一个节点，也就是 T1.next 和 T2.next 指向的是 B 节点，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675171050714-b001e6a9-0ad9-4da1-a100-5629731951ec.png#averageHue=%23f9f7f6&clientId=u5a9cc6e8-f34b-4&from=paste&id=u7917d06e&originHeight=671&originWidth=925&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3095c7a0-8904-4140-a91e-df85c70ba8e&title=)
<a name="HLNP8"></a>
## 死循环执行步骤2
死循环的第二步操作是，线程 T2 时间片用完进入休眠状态，而线程 T1 开始执行扩容操作，一直到线程 T1 扩容完成后，线程 T2 才被唤醒，扩容之后的场景如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675171050922-0690982d-7783-4ae4-8f7a-614733167587.png#averageHue=%23f9f7f6&clientId=u5a9cc6e8-f34b-4&from=paste&id=uacac094d&originHeight=422&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u61afb9b8-a84f-439d-b561-9104dabcce9&title=)从上图可知线程 T1 执行之后，因为是头插法，所以 HashMap 的顺序已经发生了改变，但线程 T2 对于发生的一切是不可知的，所以它的指向元素依然没变，如上图展示的那样，T2 指向的是 A 元素，T2.next 指向的节点是 B 元素。
<a name="ZnTD7"></a>
## 死循环执行步骤3
当线程 T1 执行完，而线程 T2 恢复执行时，死循环就建立了，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675171051183-f015bda8-9302-4042-9198-5d7fa11516b2.png#averageHue=%23faf7f7&clientId=u5a9cc6e8-f34b-4&from=paste&id=u208250fb&originHeight=680&originWidth=1004&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u543232d6-f520-417d-97ce-ffd55f61ec2&title=)因为 T1 执行完扩容之后 B 节点的下一个节点是 A，而 T2 线程指向的首节点是 A，第二个节点是 B，这个顺序刚好和 T1 扩完容完之后的节点顺序是相反的。**T1 执行完之后的顺序是 B 到 A，而 T2 的顺序是 A 到 B，这样 A 节点和 B 节点就形成死循环了**，这就是 HashMap 死循环导致的原因。
<a name="aCLDA"></a>
## 解决方案
HashMap 死循环的常用解决方案有以下 3 个：

- 使用线程安全容器 `ConcurrentHashMap` 替代（推荐使用此方案）。
- 使用线程安全容器 `Hashtable` 替代（性能低，不建议使用）。
- 使用 `synchronized` 或 Lock 加锁 `HashMap` 之后，再进行操作，相当于多线程排队执行（比较麻烦，也不建议使用）。
<a name="d3iNS"></a>
## 总结
HashMap 死循环发生在 JDK 1.7 版本中，形成死循环的原因是 HashMap 在 JDK 1.7 使用的是头插法，头插法 + 链表 + 多线程并发 + HashMap 扩容，这几个点加在一起就形成了 HashMap 的死循环，解决死锁可以采用线程安全容器 `ConcurrentHashMap` 替代。
