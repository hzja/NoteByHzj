Java <br />创建对象的时候，对象是在堆内存中创建的。但堆内存又分为新生代和老年代，新生代又细分为 Eden 空间、From Survivor 空间、To Survivor 空间。**那创建的对象到底在哪里**？
<a name="ujmgR"></a>
### 一、对象优先在 Eden 分配
堆内存分为新生代和老年代，新生代是用于存放使用后准备被回收的对象，老年代是用于存放生命周期比较长的对象。<br />大部分创建的对象，都属于生命周期比较短的，所以会存放在新生代。新生代又细分 Eden 空间、From Survivor 空间、To Survivor 空间，创建的对象优先在 Eden 分配。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772393641-e7c224d6-f1f2-42dc-a059-f8eed1042afa.png#clientId=u9e7c9417-3295-4&from=paste&id=u9328f11d&originHeight=301&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u73ca0186-495f-4537-b71f-d079a0ce1a8&title=)<br />随着对象的创建，Eden 剩余内存空间越来越少，就会触发 Minor GC，于是 Eden 的存活对象会放入 From Survivor 空间。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772393647-cf1afd05-e700-4282-ba14-e027d0977883.png#clientId=u9e7c9417-3295-4&from=paste&id=u4e87674f&originHeight=312&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub421d612-d5bf-45d4-902a-b569f955b0e&title=)<br />Minor GC 后，新对象依然会往 Eden 分配。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772393714-56fb14c7-95c0-4c53-aa3f-f2be850b50e3.png#clientId=u9e7c9417-3295-4&from=paste&id=ubfc06f8c&originHeight=299&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u83d40b03-71e0-45ce-a49e-4572c9b560b&title=)<br />Eden 剩余内存空间越来越少，又会触发 Minor GC，于是 Eden 和 From Survivor 的存活对象会放入 To Survivor 空间。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772393752-ecb7d36b-270e-452c-b0f0-22688eee93d1.png#clientId=u9e7c9417-3295-4&from=paste&id=u66b74896&originHeight=295&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6debd674-c53c-4f09-ba90-ecd3161628f&title=)
<a name="OMp3V"></a>
### 二、大对象直接进入老年代
在上面的流程中，如果一个对象很大，一直在 Survivor 空间复制来复制去，那很费性能，所以这些大对象直接进入老年代。<br />可以用 `XX:PretenureSizeThreshold` 来设置这些大对象的阈值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772393910-1c46695a-c9fe-4127-bd7b-778501068b5f.png#clientId=u9e7c9417-3295-4&from=paste&id=u8ea501d9&originHeight=309&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u43e7cac9-54e4-4399-87a4-49432a0cc9d&title=)
<a name="XpPNI"></a>
### 三、长期存活的对象将进入老年代
在上面的流程中，如果一个对象 Hello_A，已经经历了 15 次 Minor GC 还存活在 Survivor 空间中，那他即将转移到老年代。这个 15 可以通过 `-XX:MaxTenuringThreshold` 来设置的，默认是 15。<br />虚拟机为了给对象计算他到底经历了几次 Minor GC，会给每个对象定义了一个对象年龄计数器。如果对象在 Eden 中经过第一次 Minor GC 后仍然存活，移动到 Survivor 空间年龄加 1，在 Survivor 区中每经历过 Minor GC 后仍然存活年龄再加 1。年龄到了 15，就到了老年代。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772394034-e681feef-6330-4077-8bb9-4f50a8bdf035.png#clientId=u9e7c9417-3295-4&from=paste&id=u912d7294&originHeight=305&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7536214c-84fd-4065-9a54-faa61f84378&title=)
<a name="SLQgL"></a>
### 四、动态年龄判断
除了年龄达到 MaxTenuringThreshold 的值，还有另外一个方式进入老年代，那就是动态年龄判断：在 Survivor 空间中相同年龄所有对象大小的总和大于 Survivor 空间的一半，年龄大于或等于该年龄的对象就可以直接进入老年代。<br />比如 Survivor 是 100M，Hello1 和 Hello2 都是 3 岁，且总和超过了 50M，Hello3 是 4 岁，这个时候，这三个对象都将到老年代。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772394147-9bb11242-ea7b-4afb-951d-04412a6f7057.png#clientId=u9e7c9417-3295-4&from=paste&id=u981533bf&originHeight=305&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubcd0948d-1518-4678-8751-4aa7ccc36cc&title=)
<a name="dbPWs"></a>
### 五、空间分配担保
上面的流程提过，存活的对象都会放入另外一个 Survivor 空间，如果这些存活的对象比 Survivor 空间还大呢？整个流程如下：

- Minor GC 之前，虚拟机会先检查老年代最大可用的连续空间是否大于新生代所有对象总空间，如果大于，则发起 Minor GC。
- 如果小于，则看 HandlePromotionFailure 有没有设置，如果没有设置，就发起 full gc。
- 如果设置了 HandlePromotionFailure，则看老年代最大可用的连续空间是否大于历次晋升到老年代对象的平均大小，如果小于，就发起 full gc。
- 如果大于，发起 Minor GC。Minor GC 后，看 Survivor 空间是否足够存放存活对象，如果不够，就放入老年代，如果够放，就直接存放 Survivor 空间。如果老年代都不够放存活对象，担保失败（Handle Promotion Failure），发起 full gc。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645772394189-d4abe62f-56ca-4df9-a250-535e217e0660.png#clientId=u9e7c9417-3295-4&from=paste&id=u8dd4c25a&originHeight=585&originWidth=646&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ued6e1348-9f62-46ca-adf8-f0c07aa3a8a&title=)
