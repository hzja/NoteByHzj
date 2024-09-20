Evidently 是一个面向数据科学家和机器学习工程师的开源 Python 库。它有助于评估、测试和监控从验证到生产的数据和 ML 模型。它适用于表格、文本数据和嵌入。
<a name="I2BhP"></a>
## 简介
**Evidently** 是一个开源的 Python 工具，旨在帮助构建对机器学习模型的监控，以确保它们的质量和在生产环境运行的稳定性。<br />它可以用于模型生命周期的多个阶段：作为 notebook 中检查模型的仪表板，作为 pipeline 的一部分，或者作为部署后的监控。<br />Evidently 特别关注模型漂移，同时也提供了模型质量检查、数据质量检查和目标漂变监测等功能。此外，它还提供了多种内置的指标、可视化图形和测试，可以轻松地放入报告、仪表板或测试驱动的 pipeline 中。
<a name="VjxeJ"></a>
## 功能
Evidently采用了由 3 个组件组成的模块化方法：**报告**、**测试套件**和**监控仪表板**。<br />它们涵盖不同的使用场景：从临时分析到自动化管道测试和持续监控。
<a name="diaZm"></a>
### 1、测试套件：批量模型检查
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702189626751-0ee84d24-c343-435e-ab69-101008897317.png#averageHue=%23bc5026&clientId=u93d3d74d-9962-4&from=paste&id=u53bed287&originHeight=729&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3bae8637-cb82-4778-82f7-a4d4e0ef31b&title=)<br />测试执行结构化数据和机器学习模型质量检查，可以手动设置条件，也可以让 Evidently 根据参考数据集生成条件，返回明确的**通过**或**失败**结果。可以从 50 多个测试创建测试套件或运行预设之一。例如，测试数据稳定性或回归性能。<br />**输入**：一个或两个数据集，如 pandas.DataFrames 或 csv。<br />**获取输出**：在 Jupyter Notebook 或 Colab 中，导出 HTML、JSON 或 Python 字典。<br />**主要用例**：基于测试的机器学习监控，以将测试作为机器学习管道中的一个步骤来运行。例如，当收到一批新的数据、标签或生成预测时。可以根据结果构建条件工作流程，例如触发警报、重新训练或获取报告。
<a name="k20Mv"></a>
### 2、报告：交互式可视化
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702189626750-79e566a2-12d2-40e1-8bfb-1f051a983f3c.png#averageHue=%23dcd0ca&clientId=u93d3d74d-9962-4&from=paste&id=u0d7e5146&originHeight=579&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6b2a74b6-c76d-40d9-8280-ec395fda15a&title=)<br />计算各种指标并提供丰富的交互式可视化报告，可以根据各个指标创建自定义报告，或运行涵盖模型或数据性能特定方面的预设。例如，数据质量或分类性能。<br />**输入**：一个或两个数据集，如 pandas.DataFrames 或 csv。<br />**如何获取输出**：在 Jupyter Notebook 或 Colab 中，导出 HTML 文件、JSON 或 Python 字典。<br />**主要用例**：分析和探索，有助于直观地评估数据或模型性能。例如，在探索性数据分析期间、对训练集进行模型评估、调试模型质量衰减时或比较多个模型时。
<a name="yXlgh"></a>
### 3、机器学习监控仪表板
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702189626846-9b10952e-9660-48e3-a30f-1a733fadaa2d.png#averageHue=%23fcfcfc&clientId=u93d3d74d-9962-4&from=paste&id=u42bc5ac8&originHeight=794&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub33bdac7-e8e3-49ab-9755-caa0bb7b369&title=)<br />可以自行托管机器学习监控仪表板，以随着时间的推移可视化指标和测试结果。此功能位于报告和测试套件之上，必须将它们的输出存储为 Evidently JSON snapshots，作为 Evidently Monitoring UI 的数据源。<br />**输入**：snapshots，记录到对象存储中。<br />**输出**：可作为网络应用程序使用的自托管仪表板。<br />**主要用例**：当需要实时仪表板来查看一段时间内的所有模型和指标时，持续监控。
<a name="k4Nat"></a>
## 安装&使用
```bash
pip install evidently
pip install jupyter
# 安装 jupyter Nbextion
pip install jupyter_contrib_nbextensions
# 在 jupyter 扩展中安装并启用 evidently
jupyter nbextension install --sys-prefix --symlink --overwrite --py evidently
jupyter nbextension enable evidently --py --sys-prefix
```
大部分情况下，需要在Jupyter notebook中使用。<br />使用步骤很简单，一般分三步：1、导入模块   2、处理数据  3、获取报告<br />举个例子，先导入所需模块
```python
import pandas as pd
import numpy as np

from sklearn.datasets import fetch_california_housing

from evidently import ColumnMapping

from evidently.report import Report
from evidently.metrics.base_metric import generate_column_metrics
from evidently.metric_preset import DataDriftPreset, TargetDriftPreset, DataQualityPreset, RegressionPreset
from evidently.metrics import *

from evidently.test_suite import TestSuite
from evidently.tests.base_test import generate_column_tests
from evidently.test_preset import DataStabilityTestPreset, NoTargetPerformanceTestPreset
from evidently.tests import *
```
导入和处理数据
```python
data = fetch_california_housing(as_frame=True)
housing_data = data.frame

housing_data.rename(columns={'MedHouseVal': 'target'}, inplace=True)
housing_data['prediction'] = housing_data['target'].values + np.random.normal(0, 5, housing_data.shape[0])

reference = housing_data.sample(n=5000, replace=False)
current = housing_data.sample(n=5000, replace=False)
```
获取报告
```python
report = Report(metrics=[
    DataDriftPreset(),
])

report.run(reference_data=reference, current_data=current)
report
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702189626755-d6f0cee1-82aa-4b82-bfcc-90083a824e6f.png#averageHue=%23eae7e7&clientId=u93d3d74d-9962-4&from=paste&id=u88dfedfa&originHeight=685&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5bb55228-8853-432c-bdd1-191cb0744bc&title=)<br />官方提供了很多获取报告的代码模板：[https://docs.evidentlyai.com/presets/all-presets](https://docs.evidentlyai.com/presets/all-presets)<br />evidently 功能十分强大，这里展示的只是其能力的冰山一角。了解更多，可以参考以下链接：<br />官网：[https://www.evidentlyai.com/](https://www.evidentlyai.com/)<br />文档：[https://docs.evidentlyai.com/](https://docs.evidentlyai.com/)<br />开源地址：[https://github.com/evidentlyai/evidently](https://github.com/evidentlyai/evidently)
