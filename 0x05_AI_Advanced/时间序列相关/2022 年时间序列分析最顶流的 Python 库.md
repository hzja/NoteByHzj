Python<br />时间序列分析是一种强大的工具，可用于从数据中提取有价值的信息并对未来事件进行预测。它可用于识别趋势、季节性模式和变量之间的其他关系。时间序列分析还可用于预测未来事件，例如销售、需求或价格变动。<br />如果你在 Python 中处理时间序列数据，可以使用许多不同的库。在本文中，我们将不分先后地回顾 Python 中最流行的库。
<a name="aK3Nh"></a>
### 1、Sktime
Sktime 是一个用于处理时间序列数据的 Python 工具包。它提供了一组用于处理时间序列数据的工具，包括用于处理、可视化和分析数据的工具。Sktime 被设计为易于使用和可扩展，因此可以轻松实现新的时间序列算法。<br />Sktime 提供了对 scikit-learn API 的扩展。它包括所有必要的方法和工具，用于有效解决涉及时间序列回归、预测和分类的问题。该库包含专门的机器学习算法以及时间序列的转换方法。这些是任何其他工具库所不具备的功能。<br />sktime 还提供相关库的接口，例如 scikit-learn、statsmodels、tsfresh、PyOD 和 fbprophet等。<br />例子：
```python
from sktime.datasets import load_airline
from sktime.forecasting.model_selection import temporal_train_test_split
# from sktime.utils.plotting.forecasting import plot_ys
y = load_airline()
y_train, y_test = temporal_train_test_split(y)
plt.title('Airline Data with Train and Test')
y_train.plot(label = 'train')
y_test.plot(label = 'test')
plt.legend()
```
要了解有关此库的更多信息，请查看此链接：[https://github.com/sktime/sktime](https://github.com/sktime/sktime)
<a name="xWzWK"></a>
### 2、pmdarima
pmdarima 是一个用于时间序列数据统计分析的 Python 库。它基于 ARIMA 模型，提供了多种用于分析、预测和可视化时间序列数据的工具。Pmdarima 还提供了多种用于处理季节性数据的工具，包括季节性测试和季节性分解工具。<br />时间序列分析中经常使用的预测模型之一是 ARIMA（自回归综合移动平均）。ARIMA 是一种预测算法，我们可以根据时间序列的过去值中的信息预测未来值，而无需任何其他信息。<br />pmdarima 是 ARIMA 模型的包装器，并带有一个自动功能，可以自动为 arima 模型找到最佳超参数 (p,d,q)。该库包括并具有以下特点：

- 相当于 R 的功能auto.arima
- 平稳性和季节性统计检验的集合
- 时间序列实用程序，例如差分和逆差分
- 许多内生和外生变换器和特征器，包括 Box-Cox 和傅里叶变换
- 季节性时间序列分解
- 交叉验证实用程序
- 丰富的内置时间序列数据集，用于原型设计和示例![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671409743543-8a054be5-58e8-4ac2-ac81-481b8769970b.png#averageHue=%23fbfbfb&clientId=u05a9f5b6-9b65-4&from=paste&id=uda931677&originHeight=480&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u548b129f-89c6-4d68-9e19-93548ca01b4&title=)

要了解有关此库的更多信息，请查看此链接：[https://github.com/alkaline-ml/pmdarima](https://github.com/alkaline-ml/pmdarima)
<a name="iNYlk"></a>
### 3、tsfresh
tsfresh 是一个 Python 包，可自动执行从时间序列中提取特征的过程。它基于这样一种思想，即时间序列中的信息可以分解为一组有意义的特征，称为特征。tsfresh 负责手动提取这些特征的繁琐工作，并提供自动特征选择和分类的工具。该包旨在与 pandas DataFrames 一起使用，并提供广泛的功能来处理时间序列数据，包括：

- 从时间序列中自动提取特征
- 自动特征选择
- 时间序列分解
- 降维
- 异常值检测
- 支持多种时间序列格式
- 支持缺失值
- 支持多种语言

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671409743640-bd8b3caa-2920-4771-ab95-aabe5b60e0a9.png#averageHue=%23fbf9f7&clientId=u05a9f5b6-9b65-4&from=paste&id=u93a32fa8&originHeight=452&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u45f5bbac-962f-4095-a836-c57ac152b05&title=)<br />要了解有关此库的更多信息，请查看此链接：[https://github.com/blue-yonder/tsfresh](https://github.com/blue-yonder/tsfresh)
<a name="jnK0z"></a>
### 4、Prophet
Facebook 的 Prophet 是一种预测工具，任何人都可以使用 CSV 格式的数据。Prophet 是 Facebook 核心数据科学团队发布的开源软件。它基于一个加性模型，其中非线性趋势与每年、每周和每天的季节性以及假期效应相吻合。它最适用于具有强烈季节性影响和多个季节历史数据的时间序列。Prophet 对缺失数据和趋势变化具有鲁棒性，通常可以很好地处理异常值。<br />根据官方文档，fbprophet 可以很好地处理具有显着季节性影响的时间序列数据和几个季节的先前数据。此外，fbprophet 声称它可以抵抗丢失数据，并且能够有效地管理异常值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671409743574-95d773a3-cbea-42c2-ad35-f2bf5c85549e.png#averageHue=%23f8f8f6&clientId=u05a9f5b6-9b65-4&from=paste&id=u0b26fb7b&originHeight=369&originWidth=630&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub958af5c-8825-44a1-80f6-437f8d3e553&title=)<br />要了解有关此库的更多信息，请查看此链接：[https://github.com/facebook/prophet](https://github.com/facebook/prophet)
<a name="QcnJ8"></a>
### 5、Statsforecast
Statsforecast 提供了一系列广泛使用的单变量时间序列预测模型，它包括大量基准模型:

- 在 .AutoARIMAPythonR 中最快最准确
- 在 .ETSPythonR 中最快最准确
- 用两行代码替换 FB-Prophet，提高速度和准确性。
- 与 sklearn 接口兼容。
- 包含和用于 ARIMA.exogenous variablesprediction intervals
- 通过.numba编译成高性能机器码
<a name="NT5gu"></a>
### 6、PyCaret
PyCaret 是 Python 中的开源低代码机器学习库，可自动执行机器学习工作流程。它是一种端到端的机器学习和模型管理工具，可以成倍地加快实验周期并提高您的工作效率。<br />与其他开源机器学习库相比，PyCaret 是一个替代性的低代码库，可用于仅用几行代码替换数百行代码。这使得实验呈指数级快速和高效。PyCaret 本质上是围绕多个机器学习库和框架的 Python 包装器，例如 scikit-learn、XGBoost、LightGBM、CatBoost、spaCy、Optuna、Hyperopt、Ray 等等。<br />虽然 PyCaret 不是专门的时间序列预测库，但它有一个专门用于时间序列预测的新模块。它仍处于预发布模式，但您可以通过安装带有标签的 pycaret 来尝试。<br />PyCaret 时间序列模块与现有 API 一致，功能齐全。统计测试、模型训练和选择（30 多种算法）、模型分析、自动超参数调整、实验记录、云端部署等。所有这一切只需要几行代码。
```bash
pip install --pre pycaret
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671409743669-6c0ca7ed-e797-48ed-a68d-2cc0c2a928c4.png#averageHue=%23f7f6f6&clientId=u05a9f5b6-9b65-4&from=paste&id=u4a5909eb&originHeight=355&originWidth=674&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u92131c65-c769-4d4b-94a3-b516c9a6431&title=)<br />要了解有关此库的更多信息，请查看此链接：[https://github.com/pycaret/pycaret](https://github.com/pycaret/pycaret)
<a name="uayau"></a>
### 结论
Python 中有许多可用的时间序列预测库（比我在这里介绍的要多）。每个都有自己的长处和短处，因此根据需要选择合适的一个很重要。
