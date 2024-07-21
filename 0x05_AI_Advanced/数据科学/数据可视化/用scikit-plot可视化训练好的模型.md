对训练好的模型进行可视化，功能比较简单易懂。<br />scikit-learn (sklearn)是Python环境下常见的机器学习库，包含了常见的分类、回归和聚类算法。在训练模型之后，常见的操作是对模型进行可视化，则需要使用Matplotlib进行展示。<br />scikit-plot是一个基于sklearn和Matplotlib的库，主要的功能是对训练好的模型进行可视化，功能比较简单易懂。<br />[https://scikit-plot.readthedocs.io](https://scikit-plot.readthedocs.io)
```bash
pip install scikit-plot
```
<a name="oUCI8"></a>
## 功能1：评估指标可视化
<a name="TKIFV"></a>
### `scikitplot.metrics.plot_confusion_matrix`快速展示模型预测结果和标签计算得到的混淆矩阵。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202327-0b5678fb-a916-4a51-996e-f8ed111aac27.png#averageHue=%23e5e9ee&clientId=u58d21ad4-6420-4&from=paste&id=u1108c798&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufa25e209-03de-4103-ac54-8d499854d78&title=)
<a name="oVTWf"></a>
### `scikitplot.metrics.plot_roc`快速展示模型预测的每个类别的ROC曲线。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202384-81c122b5-d120-4488-973f-fa727b60a7dd.png#averageHue=%23f4f3f2&clientId=u58d21ad4-6420-4&from=paste&id=u0cb320ed&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u05d1bf97-3bfc-4333-870e-cb88a83ef1b&title=)
<a name="Lxmg5"></a>
### `scikitplot.metrics.plot_ks_statistic`从标签和分数/概率生成 KS 统计图。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202350-25807b83-1a3c-46af-a18a-41e99c070012.png#averageHue=%23faf9f8&clientId=u58d21ad4-6420-4&from=paste&id=u6266e50c&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue4b61407-356d-4793-a2a6-f55bd02e21c&title=)
<a name="nvr04"></a>
### `scikitplot.metrics.plot_precision_recall`从标签和概率生成PR曲线
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202382-ef3a4575-6607-4dcb-8ccb-11529d8da1c8.png#averageHue=%23f1efef&clientId=u58d21ad4-6420-4&from=paste&id=uf0738938&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2e46cdb3-1d44-40ca-bfb9-aeecdc48276&title=)
<a name="hNeFt"></a>
### `scikitplot.metrics.plot_silhouette`对聚类结果进行silhouette analysis分析
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202399-04f94c12-4cfd-4718-9ca8-0e412fb267d3.png#averageHue=%23e8e6e2&clientId=u58d21ad4-6420-4&from=paste&id=u09b1e683&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u73e5d5b5-4534-48b3-94a8-5253467d068&title=)
<a name="DJY1l"></a>
### `scikitplot.metrics.plot_calibration_curve`绘制分类器的矫正曲线
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202738-640a1f3e-ef81-4639-9892-2ffb7d36bdf7.png#averageHue=%23f7f7f7&clientId=u58d21ad4-6420-4&from=paste&id=ue97a968b&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9586a7a5-e2f7-403d-9bc8-b300ee6cd16&title=)
<a name="nFzOy"></a>
## 功能2：模型可视化
<a name="px8SB"></a>
### `scikitplot.estimators.plot_learning_curve`生成不同训练样本下的训练和测试学习曲线图。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202753-91992ef4-33a0-4952-9ee3-403fb90042ce.png#averageHue=%23f9f8f8&clientId=u58d21ad4-6420-4&from=paste&id=ue9f9d4a6&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0f9ebe74-5bd7-4ab2-9c6b-3c1fd9edabf&title=)
<a name="fuKrx"></a>
### `scikitplot.estimators.plot_feature_importances`可视化特征重要性。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202880-6d894cfa-44cf-4951-a880-445471297723.png#averageHue=%23fad7d7&clientId=u58d21ad4-6420-4&from=paste&id=u1a4a10a3&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u208a078d-f404-44bb-8d1f-e3b42789b0d&title=)
<a name="AKnNt"></a>
## 功能3：聚类可视化
<a name="EN7qV"></a>
### `scikitplot.cluster.plot_elbow_curve`展示聚类的肘步图。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202812-ecf5d6bc-9fe7-47f4-87e6-01467b7759e9.png#averageHue=%23f9f9f9&clientId=u58d21ad4-6420-4&from=paste&id=u854fa9bc&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9f3bd6df-fad9-4273-bcdd-3566d94f9e2&title=)
<a name="GE4ES"></a>
## 功能4：降维可视化
<a name="QURa7"></a>
### `scikitplot.decomposition.plot_pca_component_variance`绘制 PCA 分量的解释方差比。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352202927-ccb5ecb2-f24e-4755-911d-42f330b1c8d2.png#averageHue=%23f7f7f7&clientId=u58d21ad4-6420-4&from=paste&id=ud35580af&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9fd47787-16f8-490d-8dac-930d7a2ca0d&title=)
<a name="tWmG9"></a>
### `scikitplot.decomposition.plot_pca_2d_projection`绘制PCA降维之后的散点图。
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683352203167-fd5b6043-7cd5-416d-a83b-f254a2403c45.png#averageHue=%23f2eae2&clientId=u58d21ad4-6420-4&from=paste&id=u64c6537f&originHeight=480&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub60d0d20-2489-41ec-94c7-47cdfa8e2a9&title=)
