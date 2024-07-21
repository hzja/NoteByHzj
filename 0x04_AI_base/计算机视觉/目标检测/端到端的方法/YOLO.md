所谓端到端的目标检测方法，这些方法无需区域提名。YOLO的全称是You Only Look Once，顾名思义，就是只看一次，进一步把目标判定和目标识别合二为一，所以识别性能有了很大提升，达到每秒45帧，而在快速版YOLO（Fast YOLO，卷积层更少）中，可以达到每秒155帧。

<a name="941781dd"></a>
## YOLO步骤

（1）把输入图片缩放到448*448大小<br />（2）运行卷积网络<br />（3）对模型置信度卡阈值，得到目标位置与类别

![YOLO1.png](./img/1598598718345-81149c18-a89c-46c2-b8e7-d0658c735bd8.png)

YOLO网络的模型如上图所示，将448*448大小的图片切成![](./img/e7b715525432f379cc1ee40265233aaf.svg)的网格，目标中心点所在的格子负责该目标的相关检测，每个网络预测![](./img/9d5ed678fe57bcca610140957afab571.svg)个边框及其置信度，以及![](./img/0d61f8370cad1d412f80b84d143e1257.svg)种类别的概率。在YOLO中![](./img/ea62ac6ea6a7d17a57b65853dc566af1.svg)，![](./img/c9eea70aabf5b4b44b33f7bb1d86ce72.svg)，![](./img/0d61f8370cad1d412f80b84d143e1257.svg)取决于数据集中物体类别数量，比如VOC数据集就是![](./img/45eaf6e8e701d70ec0b7adf85958b7ce.svg)。对VOC数据集来说，YOLO就是把图片统一缩放到448*448，然后每张图平均划分为![](./img/94e0951cff3c5f9a1dd128bdc0105ac2.svg)个小格子，每个格子预测![](./img/c81e728d9d4c2f636f067f89cc14862c.svg)个矩形框及其置信度，以及20种类别的概率。较大的物体可能会由多个网格单元提名，YOLO采用了NMS（Non-Maximum Suppression，非最大抑制）的方法来过滤结果。NMS将mAP提升了2~3个百分点。

![YOLO2.png](./img/1598598732409-a105cdf3-e54c-4d83-a401-fefc236697dd.png)

YOLO简化了整个目标检测流程，速度的提升也很大，但是YOLO还是有不少可改进的地方，比如![](./img/e7b715525432f379cc1ee40265233aaf.svg)的网格就是一个比较启发式的策略，如果两个小目标同时落入一个格子中，模型也只能预测一个；另一个问题是损失函数对不同大小的bbox未做区分。

<a name="Source"></a>
## Source

[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)<br />[https://github.com/pjreddie/darknet/wiki/YOLO:-Real-Time-Object-Detection](https://github.com/pjreddie/darknet/wiki/YOLO:-Real-Time-Object-Detection)<br />[https://arxiv.org/abs/1506.02640](https://arxiv.org/abs/1506.02640)
