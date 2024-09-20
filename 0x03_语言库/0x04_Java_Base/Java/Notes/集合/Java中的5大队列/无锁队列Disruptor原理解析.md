Java 无锁队列 Disruptor
<a name="T1wFo"></a>
## 队列比较
队列<br />![2021-05-07-21-46-35-548188.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620396951881-6207dbde-f9a7-4a66-8412-d16e33c50a3a.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=ua2fc6ac1&originHeight=290&originWidth=815&originalType=binary&size=37178&status=done&style=shadow&taskId=ufb6e3736-11e7-4f01-8b7e-89afb5dd5c2)<br />队列比较<br />总结：<br />就性能而言，无锁(什么也不加) > CAS > LOCK;<br />从现实使用中考虑，一般选择有界队列（避免生产者速度过快，导致内存溢出）；同时，为了减少Java的垃圾回收对系统性能的影响，会尽量选择array/heap格式的数据结构。所以实际使用中用`ArrayBlockingQueue`多一些；<br />注：之后会将`ArrayBlockingQueue`和`Disruptor`做一些对比。
<a name="jz9c3"></a>
## Disruptor是什么
`Disruptor`是英国外汇交易公司LMAX开发的一个高性能队列，研发的初衷是解决内存队列的延迟问题（在性能测试中发现竟然与I/O操作处于同样的数量级）。基于`Disruptor`开发的系统单线程能支撑每秒600万订单，2010年在QCon演讲后，获得了业界关注。2011年，企业应用软件专家Martin Fowler专门撰写长文介绍。同年它还获得了Oracle官方的Duke大奖。<br />目前，包括Apache Storm、Camel、Log4j 2等在内的很多知名项目都应用了Disruptor以获取高性能。<br />![2021-05-07-21-46-35-760976.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620396982196-5ae88bce-6267-4b12-a9ee-4001732c3270.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=ucd76f980&originHeight=266&originWidth=472&originalType=binary&size=22588&status=done&style=shadow&taskId=u6c535505-5475-4691-9105-10bdf0a740c)![2021-05-07-21-46-35-928637.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620396982240-410c0c8e-7e61-4dbf-9faa-f27a615c708d.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=uf909840d&originHeight=155&originWidth=587&originalType=binary&size=15614&status=done&style=shadow&taskId=u6e2243e9-e007-4c95-b665-f0cee2acb4d)<br />数据来自：<br />[https://github.com/LMAX-Exchange/disruptor/wiki/Performance-Results](https://github.com/LMAX-Exchange/disruptor/wiki/Performance-Results)
<a name="rltrk"></a>
## Disruptor原理解析
<a name="ysFmd"></a>
### CPU缓存
![2021-05-07-21-46-36-200373.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397000419-6167a0fe-82b8-4820-a241-c2597544071b.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=u40b347fe&originHeight=456&originWidth=517&originalType=binary&size=74719&status=done&style=shadow&taskId=udb721d0f-77ae-496a-874e-474307d7479)<br />缓存层级越接近于 CPU core，容量越小，速度越快，当 CPU 执行运算的时候，它先去 L1 查找所需的数据，再去 L2，然后是 L3，最后如果这些缓存中都没有，所需的数据就要去主内存拿。走得越远，运算耗费的时间就越长。
<a name="sVD62"></a>
### 缓存行
![2021-05-07-21-46-36-337015.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397010873-2c850ff4-38e7-43e7-ac47-1b625745b2c5.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=ua05533d0&originHeight=372&originWidth=576&originalType=binary&size=23918&status=done&style=shadow&taskId=u40c643b2-afad-4000-9898-0bf15dee441)<br />缓存行 (Cache Line) 是 CPU Cache 中的最小单位，CPU Cache 由若干缓存行组成，一个缓存行的大小通常是 64 字节（这取决于 CPU），并且它有效地引用主内存中的一块地址。一个 Java 的 long 类型是 8 字节，因此在一个缓存行中可以存 8 个 long 类型的变量。<br />CPU每次从主存中拉取数据时，会把相邻的数据也存入同一个cache line。<br />在访问一个long数组的时候，如果数组中的一个值被加载到缓存中，它会自动加载另外7个。因此能非常快的遍历这个数组。事实上，可以非常快速的遍历在连续内存块中分配的任意数据结构。
<a name="O6DiF"></a>
### 利用CPU缓存-示例
![2021-05-07-21-46-36-521356.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397165347-0c936876-c5f8-4e26-8ddf-8abe5ce384ff.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=ub4b4cc90&originHeight=492&originWidth=1080&originalType=binary&size=64957&status=done&style=shadow&taskId=u3294406b-9b1c-4a2f-8f39-7e13ebf8a8e)
<a name="j4Ppw"></a>
### 伪共享
![2021-05-07-21-46-36-816360.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397177038-249011eb-a0e0-4041-8b64-78953bdf9242.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=u57330c14&originHeight=404&originWidth=442&originalType=binary&size=108062&status=done&style=shadow&taskId=u236806f0-02de-42b6-978d-63f9f4b6a5d)<br />如果多个线程的变量共享了同一个 CacheLine，任意一方的修改操作都会使得整个 CacheLine 失效（因为 CacheLine 是 CPU 缓存的最小单位），也就意味着，频繁的多线程操作，CPU 缓存将会彻底失效，降级为 CPU core 和主内存的直接交互。
<a name="Q0YCG"></a>
### 如何解决伪共享（字节填充）
![2021-05-07-21-46-36-966366.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397195314-fbd2b631-f2c8-4e5a-818b-d4e130a00349.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=u8440ae0a&originHeight=353&originWidth=519&originalType=binary&size=10967&status=done&style=none&taskId=u12e5a50b-c54c-4b91-a1ab-34066f03abe)![2021-05-07-21-46-37-058385.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397195296-9618808e-a3df-44f3-8375-95780a857c5d.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=u13efcd88&originHeight=353&originWidth=519&originalType=binary&size=10967&status=done&style=none&taskId=u9a7075cc-24f3-4cad-8736-33a98828691)
<a name="lKRZF"></a>
### RingBuffer
![2021-05-07-21-46-37-396522.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397206693-76873013-0c1d-473b-b8aa-4c3d39bd4f5a.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=ub4bf662d&originHeight=459&originWidth=575&originalType=binary&size=66986&status=done&style=shadow&taskId=u0187d331-6baa-4e24-9897-34b87f43952)<br />在`Disruptor`中采用了数组的方式保存了数据，上面也介绍了采用数组保存访问时很好的利用缓存，但是在`Disruptor`中进一步选择采用了环形数组进行保存数据，也就是RingBuffer。在这里先说明一下环形数组并不是真正的环形数组，在`RingBuffer`中是采用取余的方式进行访问的，比如数组大小为 10，0访问的是数组下标为0这个位置，其实10，20等访问的也是数组的下标为0的这个位置。<br />当然其不仅解决了数组快速访问的问题，也解决了不需要再次分配内存的问题，减少了垃圾回收，因为0，10，20等都是执行的同一片内存区域，这样就不需要再次分配内存，频繁的被JVM垃圾回收器回收。<br />实际上，在这些框架中取余并不是使用%运算，都是使用的&与运算，这就要求设置的大小一般是2的N次方也就是，10,100,1000等等，这样减去1的话就是，1，11，111，就能很好的使用index & (size -1),这样利用位运算就增加了访问速度。如果在Disruptor中不用2的N次方进行大小设置，他会抛出`buffersize`必须为2的N次方异常。
<a name="Tr88c"></a>
## Disruptor生产者和消费者
<a name="xfzJy"></a>
### 生产者写入数据
写入数据的步骤包括：<br />1.占位；<br />2.移动游标并填充数据；<br />需要考虑的问题：<br />1.如何避免生产者的生产速度过快而造成的新消息覆盖了未被消费的旧消息的问题；<br />2.如何解决多个生产者抢占生产位的问题;<br />![2021-05-07-21-46-37-866763.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397230429-cff8983f-3131-43a1-946f-d11f4f6233fd.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=u4ad9750a&originHeight=556&originWidth=1080&originalType=binary&size=178300&status=done&style=shadow&taskId=ue5d39766-7937-46da-8eb3-ffa21d10cdf)<br />1.如何避免生产者的生产速度过快而造成的新消息覆盖了未被消费的旧消息的问题；<br />答：生产者再获取占位之前需要查看当前最慢的消费者位置，如果当前要发布的位置比消费者大，就等待;<br />2.如何解决多个生产者抢占生产位的问题;<br />答：多个生产者通过CAS获取生产位；
<a name="UiIzi"></a>
### 消费者读取数据
![2021-05-07-21-46-38-241329.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397247068-0c8aa399-8f76-46b4-8a01-42d7844e5281.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=uab3f2794&originHeight=360&originWidth=732&originalType=binary&size=137755&status=done&style=shadow&taskId=uee5471c7-d8fc-43c5-b28a-b56e76ed86b)<br />说明：<br />1.一个消费者一个线程；<br />2.每个消费者都有一个游标表示已经消费到哪了（Sequence）；<br />3.消息者会等待(`waitFor`)新数据，直到生产者通知(`signal`)；<br />需要考虑的问题：<br />如何防止读取的时候，读到还未写的元素？<br />`WaitStrategy`（等待策略）：<br />`BlockingWaitStrategy`：默认策略，没有获取到任务的情况下线程会进入等待状态。cpu 消耗少，但是延迟高。<br />`TimeoutBlockingWaitStrategy`：相对于`BlockingWaitStrategy`来说，设置了等待时间，超过后抛异常。<br />`BusySpinWaitStrategy`：线程一直自旋等待。cpu 占用高，延迟低.<br />`YieldingWaitStrategy`：尝试自旋 100 次，然后调用 `Thread.yield()` 让出 cpu。cpu 占用高，延迟低。<br />`SleepingWaitStrategy`：尝试自旋 100 此，然后调用 `Thread.yield()` 100 次，如果经过这两百次的操作还未获取到任务，就会尝试阶段性挂起自身线程。此种方式是对cpu 占用和延迟的一种平衡，性能不太稳定。
<a name="CYywI"></a>
### 生产者写入数据示例1
![2021-05-07-21-46-38-527961.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397285584-03b48d9c-4bed-4800-83b8-3466cbc6ccf6.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=udfb6b18f&originHeight=489&originWidth=1080&originalType=binary&size=55801&status=done&style=shadow&taskId=u7b85b8c9-73f5-4b18-98bc-2719bd0ccdd)
<a name="PlV3p"></a>
### 生产者写入数据示例2
![2021-05-07-21-46-38-685894.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620397295050-c40284ab-a516-40e7-b5bc-02eec445f7de.png#clientId=u4aeaeef7-9fbf-4&from=ui&id=u20600991&originHeight=470&originWidth=1080&originalType=binary&size=52192&status=done&style=shadow&taskId=u4319a7ef-9bc8-428e-bef6-20a422cc1ce)
<a name="zC3xa"></a>
## 总结
`Disruptor`与`ArrayBlockingQueue`不同的地方：<br />1.增加缓存行补齐， 提升cache缓存命中率， 没有为伪共享和非预期的竞争；<br />2. 可选锁无关lock-free, 没有竞争所以非常快；<br />3. 环形数组中的元素不会被删除；<br />4. 支持多个消费者，每个消费者都可以获得相同的消息（广播）。而`ArrayBlockingQueue`元素被一个消费者取走后，其它消费者就无法从Queue中取到；
