Pingouin库基于pandas、scipy和statsmodels，为用户提供了执行常见统计分析的功能。它支持各种统计方法和假设检验，例如 t-tests、ANOVA、correlation analysis 等。看一些示例代码，以更全面地了解如何使用Pingouin库进行统计分析。
<a name="sqDm8"></a>
## 安装Pingouin库
首先，确保已安装Pingouin库：
```bash
!pip install pingouin
```
<a name="HdXkW"></a>
## 示例1：独立样本t-检验
```python
import pandas as pd
from pingouin import ttest

# 创建示例数据集
data = pd.DataFrame({
    'Group': ['A'] * 20 + ['B'] * 20,
    'Values': list(range(20)) + list(range(10, 30))
})

# 执行独立样本t-检验
ttest_result = ttest(data=data, dv='Values', between='Group')
print(ttest_result)
```
以上示例演示了如何使用Pingouin进行独立样本t-检验。首先，创建包含两个组的示例数据集。然后使用`ttest()`函数进行独立样本t-检验，指定数据、因变量和分组变量，并打印结果。
<a name="Cq7TJ"></a>
## 示例2：相关性分析
```python
from pingouin import correlation

# 创建包含两个变量的示例数据集
data = pd.DataFrame({
    'X': range(20),
    'Y': range(20) + [5, 9, 3, 6, 8, 4, 7, 2, 1, 0]
})

# 计算Pearson相关系数和显著性
corr = correlation(data=data, x='X', y='Y', method='pearson')
print(corr)
```
这个示例展示了如何使用Pingouin执行Pearson相关性分析。创建一个包含两个变量的示例数据集，并使用`correlation()`函数计算了这两个变量之间的Pearson相关系数以及相关性的显著性。
<a name="XfFgs"></a>
## 示例3：双因素方差分析（ANOVA）
```python
from pingouin import anova

# 创建包含两个因素的示例数据集
data = pd.DataFrame({
    'Group': ['A'] * 20 + ['B'] * 20,
    'Values': list(range(20)) + list(range(10, 30)),
    'Category': ['X'] * 10 + ['Y'] * 10 + ['X'] * 10 + ['Y'] * 10
})

# 执行双因素ANOVA
anova_result = anova(data=data, dv='Values', between='Group', detailed=True)
print(anova_result)
```
这个示例展示了如何使用Pingouin执行双因素方差分析（ANOVA）。创建一个包含两个因素的示例数据集，并使用`anova()`函数执行双因素ANOVA。指定数据、因变量、分组变量以及`detailed=True`以获取更详细的分析结果。
<a name="rgAXa"></a>
## 配对样本t-检验
```python
from pingouin import ttest

# 创建示例数据集
data = pd.DataFrame({
    'Before': [5, 7, 3, 6, 2],
    'After': [8, 9, 6, 10, 5]
})

# 执行配对样本t-检验
paired_ttest_result = ttest(data=data, dv='Before', within='After', paired=True)
print(paired_ttest_result)
```
以上是如何使用Pingouin执行配对样本t-检验的示例。创建包含两列（前后两次观测）的示例数据集，并使用`ttest()`函数进行配对样本t-检验，指定数据、因变量和配对变量，并打印结果。
<a name="PWCdy"></a>
## 线性回归
```python
from pingouin import linear_regression

# 创建示例数据集
data = pd.DataFrame({
    'X': range(20),
    'Y': range(20) + [5, 9, 3, 6, 8, 4, 7, 2, 1, 0]
})

# 执行线性回归
regression_result = linear_regression(data=data, x='X', y='Y')
print(regression_result)
```
上述代码展示了如何使用Pingouin进行线性回归。创建包含两个变量的示例数据集，并使用`linear_regression()`函数执行线性回归分析，计算回归系数、显著性等，并打印结果。
<a name="apmKC"></a>
## 多因素方差分析（ANOVA）
```python
from pingouin import anova

# 创建包含多个因素的示例数据集
data = pd.DataFrame({
    'Group': ['A'] * 20 + ['B'] * 20,
    'Values': list(range(20)) + list(range(10, 30)),
    'Category': ['X'] * 10 + ['Y'] * 10 + ['X'] * 10 + ['Y'] * 10,
    'Color': ['Red'] * 20 + ['Blue'] * 20
})

# 执行多因素ANOVA
multifactor_anova_result = anova(data=data, dv='Values', between=['Group', 'Color'], detailed=True)
print(multifactor_anova_result)
```
这个示例展示了如何使用Pingouin进行多因素方差分析（ANOVA）。创建一个包含多个因素的示例数据集，并使用`anova()`函数执行多因素ANOVA。在此例中，指定了数据、因变量、多个分组变量以及`detailed=True`以获取更详细的分析结果。<br />Pingouin库还有更多功能，如非参数检验、协方差分析等。这些功能为用户提供了丰富的统计分析工具，有助于深入了解数据和进行科学实验分析。
