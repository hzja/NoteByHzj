IDEA Plugins<br />在做源码分析的时候，常常会有这样一种需求，一堆源码看完后，画个时序图把思路捋一下，像下面这样：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248330-c917655d-89a8-41fc-97e1-e709d43b8923.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=ubf9b4c17&originHeight=738&originWidth=916&originalType=url&ratio=1&status=done&style=shadow&taskId=u7b65fa5f-c17f-4698-b5bb-602062a0e6d)<br />画图的工具有很多，相信大部分小伙伴也都会画。<br />然而，画图太费时间了！而且这个图跟代码关联性很大，就是照着代码画的。<br />那么问题就来了，能不能照着代码生成呢？要是能够照着代码生成，岂不是要爽很多。<br />来试试 IDEA 插件 SequenceDiagram。
<a name="Myscq"></a>
## 1、安装
先来时说说安装，IDEA 插件市场直接搜索并安装：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248385-fd57b9f0-a575-4dea-ade4-469990170ddb.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=uf8355c66&originHeight=658&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u2a4e410c-1692-4e99-a3b7-9164c92d7e6)<br />装好之后记得重启一下 IDEA。
<a name="Bop12"></a>
## 2、功能
再来说说这个插件都有哪些功能？

- 生成简单的时序图。
- 通过单击时序图上的文本来导航代码。
- 可以从时序图中删除某一个类。
- 可以将时序图导出为图像。
- 可以将时序图导出为 PlantUML 文件。

另外还有三个实验性的功能：

- 智能界面
- Lambda 表达式
- Kotlin 支持
<a name="ERQHG"></a>
## 3、用法
安装好 SequenceDiagram 之后，可以从如下几个地方启用它：

- 在导航工具栏中，出现了如下图标（IDEA 左下角）：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248415-29144ed4-cc5f-4ff6-b0a7-0bfbfd279573.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=u518c4909&originHeight=656&originWidth=904&originalType=url&ratio=1&status=done&style=shadow&taskId=u2725ecc1-8aff-422c-8370-ee7cc976247)

- 工具菜单中 Tools -> Sequence Diagram

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248408-320c3229-93ad-4ea3-9f8a-797e541b55f8.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=ubec74a1f&originHeight=1068&originWidth=758&originalType=url&ratio=1&status=done&style=shadow&taskId=ue93cf166-eea4-490c-b878-81155daa482)

- 在编辑器右键菜单中 Sequence Diagram ...

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248387-3b41b0e0-1d89-4d9c-a917-1f7781fe3230.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=uc718ad5a&originHeight=476&originWidth=1006&originalType=url&ratio=1&status=done&style=shadow&taskId=uc49ca64a-42fd-4dc8-9242-f817480541e)

- 在 IntentionAction 提示中 Generate sequence diagram（光标放在方法名上，按 alt+enter 或者 option+enter）：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248786-52d92240-8b6d-497b-bdaf-dd305e7a07a1.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=ufcc58a10&originHeight=284&originWidth=830&originalType=url&ratio=1&status=done&style=shadow&taskId=u32a76357-ca11-4f56-9437-34b57785f0d)<br />通过这些地方可以生成时序图，最终生成的时序图如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248756-69963ec8-d056-4a90-a91f-b752047d78b2.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=ud3d0bd7e&originHeight=518&originWidth=950&originalType=url&ratio=1&status=done&style=shadow&taskId=u2b9b888d-8263-41be-86b7-d65dbc24e12)<br />由图中可以清楚的看到方法的调用以及返回过程。<br />这张图中有几个细节，分别来看下。<br />先看左边一列按钮。<br />第一个三角是重新生成按钮（不好使，似乎有 BUG）。<br />第二个扳手是设置一些生成的细节，例如方法调用的深度，是否忽略 get/set，是否忽略私有方法/构造方法 等，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248876-20a951e2-f5c2-42d8-8668-c7cf38a28f91.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=u5276fece&originHeight=438&originWidth=840&originalType=url&ratio=1&status=done&style=shadow&taskId=u2760689f-66a9-4fd3-8f87-46530147ead)<br />下面的都是保存按钮了，可以保存为不同格式，可以自行尝试。<br />再看右边的图。<br />鼠标双击类名/方法名，会跳转到对应的类/方法上。<br />鼠标在类名/方法名上右键单击，可以从图中移除一个类/方法。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637030248806-8cb04d7b-6392-465a-ab4a-406daf19f1c8.webp#clientId=u44ebb9fd-fde7-4&from=paste&id=uae0f42e8&originHeight=434&originWidth=962&originalType=url&ratio=1&status=done&style=shadow&taskId=u273f9214-9e5e-4dc5-a9a5-ce16a2c8941)<br />比较简单却很管用的一个插件。
