机器学习Python<br />找到好的机器学习模型只是项目的起点。在这里将介绍如何使用 scikit-learn 在 Python 中保存和加载机器学习模型，这允许将模型保存到文件并在以后加载它以进行预测。

- 用 pickle 保存模型
- 使用 joblib 保存模型
- 保存模型的技巧
<a name="iU8p4"></a>
## 用 pickle 保存模型
Pickle 是 Python 中序列化对象的标准方式。<br />可以使用 pickle 操作序列化机器学习算法并将序列化格式保存到文件中。<br />稍后可以加载此文件以反序列化模型并使用它进行新的预测。<br />下面的示例演示了如何在 Pima Indians onset of diabetes 数据集上训练逻辑回归模型，将模型保存到文件并加载它以测试集进行预测。测试数据集链接：[https://raw.githubusercontent.com/jbrownlee/Datasets/master/pima-indians-diabetes.data.csv](https://raw.githubusercontent.com/jbrownlee/Datasets/master/pima-indians-diabetes.data.csv)
```python
# Save Model Using Pickle
import pandas
from sklearn import model_selection
from sklearn.linear_model import LogisticRegression
import pickle
url = "https://raw.githubusercontent.com/jbrownlee/Datasets/master/pima-indians-diabetes.data.csv"
names = ['preg', 'plas', 'pres', 'skin', 'test', 'mass', 'pedi', 'age', 'class']
dataframe = pandas.read_csv(url, names=names)
array = dataframe.values
X = array[:,0:8]
Y = array[:,8]
test_size = 0.33
seed = 7
X_train, X_test, Y_train, Y_test = model_selection.train_test_split(X, Y, test_size=test_size, random_state=seed)
# Fit the model on training set
model = LogisticRegression()
model.fit(X_train, Y_train)
# save the model to disk
filename = 'finalized_model.sav'
pickle.dump(model, open(filename, 'wb'))

# some time later...

# load the model from disk
loaded_model = pickle.load(open(filename, 'rb'))
result = loaded_model.score(X_test, Y_test)
print(result)
```
运行示例会将模型保存到本地工作目录中的finalized_model.sav。<br />加载保存的模型并对其进行评估，可以估计模型在未见数据上的准确性。
```
0.755905511811
```
<a name="Xx8a8"></a>
## 使用 joblib 保存模型
Joblib 是 SciPy 生态系统的一部分，并提供用于流水线化 Python 作业的实用程序。<br />它提供实用程序来高效地保存和加载使用 NumPy 数据结构的 Python 对象。<br />这对于需要大量参数或存储整个数据集（如 K 最近邻）的一些机器学习算法很有用。<br />下面的示例演示了如何在 Pima Indians onset of diabetes 数据集上训练逻辑回归模型，使用 joblib 将模型保存到文件并加载它以测试集进行预测。
```python
# Save Model Using joblib
import pandas
from sklearn import model_selection
from sklearn.linear_model import LogisticRegression
import joblib
url = "https://raw.githubusercontent.com/jbrownlee/Datasets/master/pima-indians-diabetes.data.csv"
names = ['preg', 'plas', 'pres', 'skin', 'test', 'mass', 'pedi', 'age', 'class']
dataframe = pandas.read_csv(url, names=names)
array = dataframe.values
X = array[:,0:8]
Y = array[:,8]
test_size = 0.33
seed = 7
X_train, X_test, Y_train, Y_test = model_selection.train_test_split(X, Y, test_size=test_size, random_state=seed)
# Fit the model on training set
model = LogisticRegression()
model.fit(X_train, Y_train)
# save the model to disk
filename = 'finalized_model.sav'
joblib.dump(model, filename)

# some time later...

# load the model from disk
loaded_model = joblib.load(filename)
result = loaded_model.score(X_test, Y_test)
print(result)
```
运行该示例会将模型保存为文件finalized_model.sav并为模型中的每个 NumPy 数组创建一个文件（另外四个文件）。<br />加载模型后，将报告模型对未见数据的准确性的估计。
```
0.755905511811
```
<a name="nFQyE"></a>
## 保存模型的技巧
本节列出了最终确定机器学习模型时的一些重要注意事项。

- python 版本。注意python版本。几乎可以肯定，当稍后加载和反序列化模型时，需要使用相同的主要版本的 Python 来序列化模型。
- 库版本。在反序列化保存的模型时，机器学习项目中使用的所有主要库的版本几乎肯定需要相同。这个不限于NumPy的版本和scikit-learn的版本。
- 手动序列化。可能希望手动输出学习模型的参数，以便将来可以在 scikit-learn 或其他平台中直接使用它们。机器学习算法用于进行预测的算法通常比用于学习参数的算法简单得多，可以很容易地在可以控制的自定义代码中实现。
