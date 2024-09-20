JavaLambda
<a name="sG560"></a>
## 1、情景展示
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764606537-68230816-658a-41f8-b1e5-5dc5266efd8e.png#clientId=u2388dc9b-1ab9-4&from=paste&id=u52322cc8&originHeight=331&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81d2d186-115a-496a-a089-15db7018733&title=)<br />如上图所示，想要终止for循环，使用return。<br />执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764606631-e545f934-6475-4c0e-b5fa-c63daf4d5c06.png#clientId=u2388dc9b-1ab9-4&from=paste&id=u41144dec&originHeight=93&originWidth=595&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u075355d0-0dfc-4814-8eac-14c36f04e60&title=)<br />可以看到，只有赵六没被打印出来，后续的数组元素依旧被执行了。<br />也就是说，关键字"return"，在这里执行的效果相当于普通for循环里的关键词continue"。
<a name="ClzIW"></a>
## 2、原因分析
在普通for循环里面，想要提前结束(终止)循环体使用"break"；<br />结束本轮循环，进行下一轮循环使用"continue"；<br />另外，在普通for里，如果使用"return"，不仅强制结束for循环体，还会提前结束包含这个循环体的整个方法。<br />而在Java8中的`forEach()`中，"break"或"continue"是不被允许使用的，而return的意思也不是原来return代表的含义了。<br />来看看源码：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764606647-7a1af15c-f7b1-4deb-b1fc-66c03193066b.png#clientId=u2388dc9b-1ab9-4&from=paste&id=ub4798275&originHeight=163&originWidth=564&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8b0d1c46-5168-45c8-8b4d-d3ee1151ae0&title=)<br />`forEach()`，说到底是一个方法，而不是循环体，结束一个方法的执行用什么？当然是return啦；<br />java8的`forEach()`和JavaScript的`forEach()`用法是何其的相似，感兴趣的可以去了解下<br />Java不是万能的，不要再吐槽它垃圾了。
<a name="le0rD"></a>
## 3、解决方案
<a name="RObEv"></a>
### 方案一：使用原始的foreach循环
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764606501-6318b457-0e51-41bf-9e7b-2385c0684c5b.png#clientId=u2388dc9b-1ab9-4&from=paste&id=ua83ecec5&originHeight=260&originWidth=883&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u485af34a-6aaf-482a-b4da-77483a1aac6&title=)<br />使用过eclipse的老铁们应该知道，当输入：foreach，再按快捷键：Alt+/，就会出现foreach的代码提示。<br />如上图所示，这种格式的for循环才是真正意义上的foreach循环。<br />在idea中输入，按照上述操作是不会有任何代码提示的，那如何才能在idea中，调出来呢？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764606553-9f9c2123-66e6-4d7d-a9bb-2223871017dd.png#clientId=u2388dc9b-1ab9-4&from=paste&id=ue1f3012d&originHeight=199&originWidth=718&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8c6c477c-8b53-47e4-9f1f-143778671bc&title=)<br />for循环可以提前终止。
<a name="i9lvt"></a>
#### 方式一：break
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764607049-7ae11ad9-26b7-4e1e-9290-91a609da32b3.png#clientId=u2388dc9b-1ab9-4&from=paste&id=ufba4c00c&originHeight=468&originWidth=869&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u973107f7-d03c-48a6-a439-df54d3dfcf9&title=)
<a name="Xer0w"></a>
#### 方式二：return(不推荐使用)
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764607135-b0c8bade-34df-4337-b20d-ab1130696677.png#clientId=u2388dc9b-1ab9-4&from=paste&id=u52bdc0e0&originHeight=492&originWidth=886&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d645f72-ffcf-4d42-a217-cc528e94b45&title=)
<a name="WLJUV"></a>
### 方案二：抛出异常
要想结束一个方法的执行，正常的逻辑是：使用return；<br />但是，在实际运行中，往往有很多不突发情况导致代码提前终止，比如：空指针异常，其实，也可以通过抛出假异常的方式来达到终止`forEach()`方法的目的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764607063-3763007a-e411-4d9f-a3ad-e2fbe86a4d34.png#clientId=u2388dc9b-1ab9-4&from=paste&id=ud9b23c8c&originHeight=539&originWidth=830&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u688d430f-8941-44e7-8c47-19049401fab&title=)<br />如果觉得这种方式不友好，可以再包装一层。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657764607162-b910bfd7-7099-4b20-b5e9-cc1a0300cf87.png#clientId=u2388dc9b-1ab9-4&from=paste&id=u89910646&originHeight=553&originWidth=877&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua6de347f-88d9-4616-97a9-afacc329ed8&title=)<br />这样，就完美了。<br />这里，需要注意的一点是：要确保`forEach()`方法体内不能有其它代码可能会抛出的异常与自己手动抛出并捕获的异常一样；<br />否则，当真正该因异常导致代码终止的时候，因为咱们手动捕获了并且没做任何处理，岂不是搬起石头砸自己的脚吗？
