机器学习库 Python
<a name="JYoJr"></a>
## TensorFlow
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530840099-cd97dfb8-3e38-461a-bb8f-a10fcf17abce.png#clientId=u10acdcab-ca13-4&from=paste&id=uc2af3608&originHeight=206&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucf28370b-d130-47bb-b986-9ca5af2a162&title=)
<a name="ODzBq"></a>
### 什么 TensorFlow
如果目前正在使用 Python 进行机器学习项目，那么必然会听说过这个流行的开源库 TensorFlow<br />该库由 Google 与 Brain Team 合作开发，TensorFlow 几乎是所有 Google 机器学习应用程序的一部分<br />TensorFlow 就像一个计算库，用于编写涉及大量张量操作的新算法，因为神经网络可以很容易地表示为计算图，它们可以使用 TensorFlow 作为对张量的一系列操作来实现。另外，张量是代表数据的 N 维矩阵，是机器学习的重要概念
<a name="hsW1v"></a>
### TensorFlow 的特点
TensorFlow 针对速度进行了优化，它利用 XLA 等技术进行快速线性代数运算
<a name="vUCLr"></a>
#### 响应式构造
使用 TensorFlow，可以轻松地可视化图表的每一部分，而这在使用 Numpy 或 SciKit 时是不太可能的
<a name="Lz21O"></a>
#### 灵活
Tensorflow 非常重要的功能之一是它的可操作性非常灵活，这意味着它具有很高的模块化功能，并且也提供了独立制作某些功能的选项
<a name="pxbZz"></a>
#### 易于训练
它很容易在 CPU 和 GPU 上进行训练以进行分布式计算
<a name="KVLOV"></a>
#### 并行神经网络训练
从某种意义上说，TensorFlow 提供了流水线，可以在多个 GPU 上训练多个神经网络，这使得模型在大规模系统上非常高效
<a name="nVEYi"></a>
#### 庞大的活跃社区
因为它是由谷歌开发的，那么已经有一个庞大的软件工程师团队不断致力于稳定性改进，其开发者社区非常活跃，不是一个人在战斗
<a name="tWwcg"></a>
#### 开源
这个机器学习库最好的一点是它是开源的，所以只要有互联网网络，任何人就都可以使用它
<a name="wa4IT"></a>
## Scikit-Learn
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530840111-85498bf5-9206-418c-84af-ee47d6c3088b.png#clientId=u10acdcab-ca13-4&from=paste&id=u627d54d5&originHeight=162&originWidth=300&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4cc843ef-bc90-4d3f-ac70-470c1f91c44&title=)
<a name="zyEkZ"></a>
### 什么是 Scikit-learn
它是一个与 NumPy 和 SciPy 相关联的 Python 库，它被认为是处理复杂数据的最佳库之一<br />在这个库中进行了很多优化改动，其中一项是交叉验证功能，提供了使用多个指标的能力。许多训练方法，如逻辑回归和最近邻，都得到了一些小的改进与优化
<a name="cVzQ3"></a>
### Scikit-Learn 的特点
<a name="jeMA1"></a>
#### 交叉验证
有多种方法可以检查监督模型在未见数据上的准确性
<a name="t5tJk"></a>
#### 无监督学习算法
产品中的算法种类繁多，包括聚类、因子分析、主成分分析，无监督神经网络
<a name="YV1aT"></a>
#### 特征提取
用于从图像和文本中提取特征（例如词袋）
<a name="OO7xu"></a>
## Numpy
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530840417-f1702ce5-0b48-4a0a-ad9b-5d592e58b438.png#clientId=u10acdcab-ca13-4&from=paste&id=ub53941bb&originHeight=428&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u38f78056-119c-4367-8534-e70af23f3db&title=)
<a name="r1wEg"></a>
### 什么是 Numpy
Numpy 被认为是 Python 中最流行的机器学习库之一<br />TensorFlow 和其他库都在内部使用 Numpy 对张量执行多项操作，数组接口是 Numpy 最好也是最重要的特性
<a name="d1W09"></a>
### Numpy 的特点
<a name="Cv9Ar"></a>
#### 交互的
Numpy 是可交互的且非常易于使用
<a name="UFW3k"></a>
#### 数学计算
可以使复杂的数学实现变得非常简单
<a name="tZnyX"></a>
#### 直观
使编码变得真正容易，并且容易掌握概念
<a name="rqa5L"></a>
#### 开源
使用广泛，因此有很多开源贡献者
<a name="EoPKd"></a>
## Keras
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530840619-61ccdbeb-5016-4171-92a0-0cc8f28e0ef7.png#clientId=u10acdcab-ca13-4&from=paste&id=u11aa4554&originHeight=313&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b8f7c75-3574-437d-83d4-52f871b98d3&title=)
<a name="J7ujq"></a>
### 什么是 Keras
Keras 被认为是 Python 中最酷的机器学习库之一，它提供了一种更简单的机制来表达神经网络。Keras 还提供了一些用于编译模型、处理数据集、图形可视化等最佳实用程序<br />在后端，Keras 在内部使用 Theano 或 TensorFlow。也可以使用一些最流行的神经网络，如 CNTK。当将 Keras 与其他机器学习库进行比较时，它的速度相对较慢。因为它通过使用后端基础设施创建计算图，然后利用它来执行操作。Keras 中的所有模型都是可移植的
<a name="q9mwU"></a>
### Keras 的特点
<a name="oQhos"></a>
#### 支持 CPU 和 GPU
它可以在 CPU 和 GPU 上流畅运行
<a name="Ea503"></a>
#### 模型全面
Keras 支持神经网络的几乎所有模型——全连接、卷积、池化、循环、嵌入等。此外，这些模型可以组合起来构建更复杂的模型
<a name="crrFe"></a>
#### 模块化
Keras 本质上是模块化的，具有令人难以置信的表现力、灵活性和创新性研究能力
<a name="jI8EL"></a>
#### 完全基于 Python
Keras 是一个完全基于 Python 的框架，易于调试和探索
<a name="pVqb2"></a>
## PyTorch
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530840721-259fd29f-5d24-4d75-a612-a6fc56ba6886.png#clientId=u10acdcab-ca13-4&from=paste&id=u16bc7c84&originHeight=205&originWidth=1025&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0435193d-0124-41b8-8d01-2b7d6cb5d00&title=)
<a name="n8lf0"></a>
### 什么是 PyTorch
PyTorch 是最大的机器学习库，允许开发人员在 GPU 加速的情况下执行张量计算，创建动态计算图，并自动计算梯度。除此之外，PyTorch 还提供了丰富的 API 来解决与神经网络相关的应用程序问题<br />这个机器学习库基于 Torch，它是一个用 C 语言实现的开源机器库，并在 Lua 中进行了封装<br />这个 Python 机器库于 2017 年推出，自成立以来，该库越来越受欢迎并吸引了越来越多的机器学习开发人员
<a name="X17qh"></a>
### PyTorch 的特点
<a name="ZaGd6"></a>
#### 混合前端
新的混合前端在 Eager 模式下提供易用性和灵活性，同时无缝过渡到图形模式以在 C++ 运行时环境中实现速度、优化和功能
<a name="sR3yF"></a>
#### 分布式训练
通过利用对异步执行集体操作和可从 Python 和 C++ 访问的点对点通信的本机支持来优化研究和生产中的性能
<a name="o119S"></a>
#### Python 优先
它被构建为深度集成到 Python 中，因此可以与流行的库和包（例如 Cython 和 Numba）一起使用
<a name="NlQEi"></a>
#### 众多库和工具
活跃的研究人员和开发人员社区建立了丰富的工具和库生态系统，用于扩展 PyTorch 并支持从计算机视觉到强化学习等领域的开发
<a name="mpQji"></a>
## LightGBM
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1645530841024-7b4d7af1-9f56-4337-974f-5c2098e4c840.jpeg#clientId=u10acdcab-ca13-4&from=paste&id=u57c85e6d&originHeight=359&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufbbbab80-357e-47dd-bcc0-d2b142187de&title=)
<a name="G9oM7"></a>
### 什么是 LightGBM
Gradient Boosting 是最好和最受欢迎的机器学习库之一，它通过使用重新定义的基本模型（即决策树）帮助开发人员构建新算法。因此，有一些特殊的库可用于快速有效地实现此方法<br />这些库是 LightGBM、XGBoost 和 CatBoost。所有这些库都是有助于解决常见问题，并且可以以几乎类似的方式使用
<a name="ugQ2F"></a>
### LightGBM 的特点
<a name="edLyF"></a>
#### 快速
非常快速的计算确保了高生产效率
<a name="Bt3OU"></a>
#### 直观
直观，因此对于用户非常友好
<a name="V19Ai"></a>
#### 训练更快
有比许多其他深度学习库更快的训练速度
<a name="V8eAr"></a>
#### 容错
考虑 NaN 值和其他规范值时不会产生错误
<a name="bYJa9"></a>
## Eli5
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1645530841154-5d8e0a07-cfda-4499-b20c-7a329e6a1eb1.jpeg#clientId=u10acdcab-ca13-4&from=paste&id=u091cfff2&originHeight=479&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue4970f84-2667-445c-922a-7f1ecdc6f99&title=)
<a name="Ih1XZ"></a>
### 什么是 Eli5
大多数情况下，机器学习模型预测的结果并不准确，而使用 Python 构建的 Eli5 机器学习库有助于克服这一问题。它结合了可视化和调试所有机器学习模型并跟踪算法的所有工作步骤
<a name="NR6yX"></a>
### Eli5 的特点
Eli5 还支持很多库，例如 XGBoost、lightning、scikit-learn 和 sklearn-crfsuite 等
<a name="NIxzu"></a>
## SciPy
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530841335-216e873d-2955-4291-9f80-a8b71d76d37f.png#clientId=u10acdcab-ca13-4&from=paste&id=ubccf7808&originHeight=267&originWidth=672&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5f8d3ff1-b2f2-417e-a646-76dafb67f5e&title=)
<a name="Hra8S"></a>
### 什么是 SciPy
SciPy 是一个面向应用程序开发人员和工程师的机器学习库。SciPy 库包含用于优化、线性代数、积分和统计的模块
<a name="xVZ08"></a>
### SciPy 的特点
SciPy 库的主要特点是它是使用 NumPy 开发的，它的数组最大限度地利用了 NumPy<br />此外，SciPy 使用其特定的子模块提供所有高效的数值例程，如优化、数值积分和许多其他程序<br />SciPy 的所有子模块中的所有功能都有很好的文档记录
<a name="tfthu"></a>
## Theano
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645530841456-dd657116-3363-4af6-83e8-984a8db9068e.png#clientId=u10acdcab-ca13-4&from=paste&id=u77cea6a9&originHeight=249&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubb5d8c10-954f-4195-b7c2-2c0e26d0799&title=)
<a name="tGpqK"></a>
### 什么是 Theano
Theano 是 Python 中用于计算多维数组的计算框架机器学习库。Theano 的工作原理与 TensorFlow 类似，但不如 TensorFlow 高效，因此它无法适应生产环境<br />此外，Theano 还可以用于类似于 TensorFlow 的分布式或并行环境
<a name="Aj63j"></a>
### Theano 的特点
<a name="Sm3Sn"></a>
#### 与 NumPy 紧密集成
能够在 Theano 编译的函数中使用完整的 NumPy 数组
<a name="uq4TZ"></a>
#### 高效的使用 GPU
执行数据密集型计算的速度比在 CPU 上快得多
<a name="fHWIV"></a>
#### 高效的符号微分
Theano 可以为具有一个或多个输入的函数求导
<a name="LGmEm"></a>
#### 速度和稳定性优化
即使 x 非常小，也可以获得 log(1+x) 的正确答案。当然这只是展示 Theano 稳定性的示例之一
<a name="yj0u7"></a>
#### 动态 C 代码生成
比以往更快地评估表达式，从而大大提高效率
<a name="TvCVU"></a>
#### 广泛的单元测试和自我验证
检测和诊断模型中的多种类型的错误和歧义
<a name="dejEN"></a>
## Pandas
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1645530841655-4ad63da9-6dbc-4820-87f9-8751670caad6.jpeg#clientId=u10acdcab-ca13-4&from=paste&id=u589b1f94&originHeight=675&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub0cf5a6e-7768-4b1e-8a7a-4d80239209d&title=)
<a name="KX3cy"></a>
### 什么是 Pandas
Pandas 是 Python 中的机器学习库，提供高级数据结构和各种分析工具。这个库的一大特点是能够使用一两个命令来转换复杂的数据操作。Pandas 具有许多用于分组、组合数据和过滤的内置方法，以及时间序列功能
<a name="wAluq"></a>
### Pandas 的特点
Pandas 使得操作数据的整个过程变得更加容易，对重新索引、迭代、排序、聚合、连接和可视化等操作的支持是 Pandas 的功能亮点之一
