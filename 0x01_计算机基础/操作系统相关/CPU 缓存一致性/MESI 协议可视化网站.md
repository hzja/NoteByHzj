<a name="VyaHa"></a>
## 在线体验 MESI 协议状态转换
地址：[https://www.scss.tcd.ie/Jeremy.Jones/VivioJS/caches/MESIHelp.htm](https://www.scss.tcd.ie/Jeremy.Jones/VivioJS/caches/MESIHelp.htm)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198124791-d2307b03-2fc0-4c02-a5ed-2102bcdb1d34.png#clientId=uf9f7f8e3-8529-4&from=paste&id=u3b87db21&originHeight=680&originWidth=1068&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub0efd66d-e032-480c-a1b6-ec2f6e99b82&title=)<br />下图即是 MESI 协议的状态图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198124692-2fb7f7b4-5f3d-486c-91e5-8e090ddaa284.png#clientId=uf9f7f8e3-8529-4&from=paste&id=uf04c50eb&originHeight=584&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8e2496f1-b25e-4ea7-ab07-50ed5b59210&title=)<br />MESI 协议的四种状态之间的流转过程，汇总成了下面的表格，可以更详细的看到每个状态转换的原因：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198124772-2dd6b219-1243-4321-b1d9-77d50f7535f3.png#clientId=uf9f7f8e3-8529-4&from=paste&id=u946e9222&originHeight=1697&originWidth=872&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud39b644e-daa6-4be4-a511-9f3f967e8f7&title=)<br />接下来说说，怎么玩这个网站。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198124689-f63b8f27-e73e-42d4-9d5e-1460880c5e73.png#clientId=uf9f7f8e3-8529-4&from=paste&id=uc7d91027&originHeight=674&originWidth=1059&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ec4a693-4a77-40a6-b6a0-869b8b3a285&title=)<br />看上图，共分为三个部分：

- 第一部分，内存。显示内存地址和数据；
- 第二部分，CPU 缓存。显示 CPU 缓存的变量数据和 MESI 协议状态，因为现在还没开始操作，所以显示的是空白。
- 第三部分，CPU 操作。共有三个 CPU，每个 CPU 都有各自的 Cache，CPU 操作分别「读」和「写」，这部分是手动操作的部分。

而且 CPU 和内存之间有三条总线，分别是：

- 数据总线（Data Bus）：在CPU与内存之间来回传送需要处理或是需要储存的数据。
- 地址总线（Adress Bus），用来指定在内存之中储存的数据的地址。
- shared，这个也不知道是什么总线，网上没搜到资料，网站上最细那条总线就是这个名字，作用是控制 Cache 的数据状态。

接下来，来演示下效果。<br />操作 CPU1 读取 a0 变量，此时页面会显示如下（实际上是有动画的效果的，这里就不录制动图了，建议大家自己去操作一遍，自己感受下过程）：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198124765-7190f72a-02c2-401a-9813-eb55520d83b2.png#clientId=uf9f7f8e3-8529-4&from=paste&id=u1fafd0f6&originHeight=645&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1e5b31f4-aef1-4ba2-af81-5160c95bbc9&title=)<br />可以看到，CPU1 核心从内存读取了 a0 变量的数据，并缓存在了 Cache1 里，而且此时状态为 E （独占）。<br />然后操作 CPU2 读取 a0 变量，此时页面会显示如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198125238-2af8dcf8-5099-4f98-913c-5954cc0bd4d6.png#clientId=uf9f7f8e3-8529-4&from=paste&id=u3292b084&originHeight=627&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5130c064-e64f-47aa-a824-060ebe469f6&title=)<br />可以看到，CPU1 和 CPU1 的 Cache 数据的状态都是 S（共享），和表格中表述的现象一致。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651198125225-d168f748-c203-4f7e-99ab-a448fbff730a.png#clientId=uf9f7f8e3-8529-4&from=paste&id=u8098cb3c&originHeight=216&originWidth=613&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua6518c68-ddf1-450e-be3e-ad5e48d1292&title=)<br />别看结果是静态的，实际上当操作完 CPU2 读取 a0 变量后，实际上是有动画效果的，这里有个小技巧，在动画的过程中，可以点击网站任意一个位置，就会暂停，然后再点击就开始，这样可以很清晰的知道这个状态过程是怎么通过这三个总线完成的。<br />大概简述下过程：

1. 点击 CPU2 的 read a0 操作；
2. CPU2 会向地址总线，发送读取 a0 变量的数据的请求，此时该请求不仅会被内存收到，也会被其他 CPU 核心收到。
3. CPU1 从地址总线得知，CPU2 发起的读取地址存在于 CPU1 Cache 里，于是 CPU1 Cache 里数据的状态会从 E（独占）变更为 S（共享）；
4. 内存收到 CPU2 发起的读取 a0 数据的请求后，就会通过数据总线将数据传递给 CPU2；
5. 最后数据会写到 CPU2 Cache 里，且状态是 S（共享）。

其他状态的变更，在这里就不介绍了，大家自己去网站体验下，这样会感触比较深。
