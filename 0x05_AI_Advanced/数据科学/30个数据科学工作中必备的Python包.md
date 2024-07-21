数据科学Python<br />Python 可以说是最容易入门的编程语言，在numpy，scipy等基础包的帮助下，对于数据的处理和机器学习来说Python可以说是目前最好的语言，在各位大佬和热心贡献者的帮助下Python拥有一个庞大的社区支持技术发展，开发两个各种 Python 包来帮助数据人员的工作。
<a name="P9cdb"></a>
## 1、Knockknock
Knockknock是一个简单的Python包，它会在机器学习模型训练结束或崩溃时通知您。可以通过多种渠道获得通知，如电子邮件、Slack、Microsoft Teams等。<br />为了安装该包，使用以下代码。
```bash
pip install knockknock
```
例如，可以使用以下代码将机器学习建模训练状态通知到指定的电子邮件地址。
```python
from knockknock import email_sender
from sklearn.linear_model import LinearRegression
import numpy as np

@email_sender(recipient_emails=["<your_email@address.com>", "<your_second_email@address.com>"], sender_email="<sender_email@gmail.com>")
def train_linear_model(your_nicest_parameters):
  x = np.array([[1, 1], [1, 2], [2, 2], [2, 3]])
  y = np.dot(x, np.array([1, 2])) + 3
  regression = LinearRegression().fit(x, y)
return regression.score(x, y)
```
这样就可以在该函数出现问题或者完成时获得通知。
<a name="CvdGS"></a>
## 2、tqdm
当需要进行迭代或循环时，如果需要显示进度条?那么tqdm就是需要的。这个包将在笔记本或命令提示符中提供一个简单的进度计。<br />从安装包开始。
```bash
pip install tqdm
```
然后可以使用以下代码来显示循环过程中的进度条。
```python
from tqdm import tqdm
q = 0
for i in tqdm(range(10000000)):
    q = i +1
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1660783884835-dceb168a-e0a1-4ca9-a1da-19b205209e5c.gif#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u2aea7393&originHeight=556&originWidth=1078&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4b817bdd-3026-424d-9831-48bd3bc7f37&title=)<br />就像上面的gifg，它可以在notebook上显示一个很好的进度条。当有一个复杂的迭代并且想要跟踪进度时，它会非常有用。
<a name="VRq9b"></a>
## 3、Pandas-log
Panda -log可以对Panda的基本操作提供反馈，如.query、.drop、.merge等。它基于R的Tidyverse，可以使用它了解所有数据分析步骤。<br />安装包
```bash
pip install pandas-log
```
安装包之后，看看下面的示例。
```python
import pandas as pd
import numpy as np
import pandas_log
df = pd.DataFrame({"name": ['Alfred', 'Batman', 'Catwoman'],
                  "toy": [np.nan, 'Batmobile', 'Bullwhip'],
                  "born": [pd.NaT, pd.Timestamp("1940-04-25"),   pd.NaT]})
```
然后尝试用下面的代码做一个简单的 pandas 操作记录。
```python
with pandas_log.enable():
  res = (df.drop("born", axis = 1)
            .groupby('name')
        )
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783884893-7265c2f9-d2b1-4fb5-a695-338a5686753a.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u79015626&originHeight=277&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5db753f4-dee7-4483-b13a-0147ee41dde&title=)<br />通过 pandas-log，可以获取所有的执行信息。
<a name="AYxCM"></a>
## 4、Emoji
顾名思义，Emoji 是一个支持 emoji 文本解析的 Python 包。通常，很难用 Python 处理表情符号，但 Emoji 包可以进行转换。<br />使用以下代码安装 Emoji 包。
```bash
pip install emoji
```
看看下面代码：
```python
import emoji
print(emoji.emojize('Python is :thumbs_up:'))

# Python is 👍
```
有了这个包，可以轻易的输出表情符号。
<a name="h0Ewi"></a>
## 5、TheFuzz
TheFuzz 使用 Levenshtein 距离来匹配文本以计算相似度。
```bash
pip install thefuzz
```
下面代码介绍如何使用 TheFuzz 进行相似性文本匹配。
```python
from thefuzz import fuzz, process

#Testing the score between two sentences
fuzz.ratio("Test the word", "test the Word!")
# 81

TheFuzz 还可以同时从多个单词中提取相似度分数。
choices = ["Atlanta Falcons", "New York Jets", "New York Giants", "Dallas Cowboys"]
process.extract("new york jets", choices, limit=2)
[('new york jets', 100), 
 ('new york Giants', 79)]
```
TheFuzz 适用于任何文本数据相似性检测，这个工作在nlp中非常重要。
<a name="cto03"></a>
## 6、Numerizer
Numerizer 可将写入的数字文本转换为对应的整数或浮点数。
```bash
pip install numerizer
```
然后尝试几个输入来进行转换。
```python
from numerizer import numerize
numerize('forty two')

# '42'
```
如果使用另一种书写风格，它也可以工作的。
```python
numerize('forty-two')

# '42'

numerize('nine and three quartes')

# '9.75'
```
如果输入不是数字的表达式，那么将会保留：
```python
numerize('maybe around nine and three quarters')

# 'maybe around 9.75'
```
<a name="QLqaK"></a>
## 7、PyAutoGUI
PyAutoGUI 可以自动控制鼠标和键盘。
```bash
pip install pyautogui
```
然后可以使用以下代码测试。
```python
import pyautogui
pyautogui.moveTo(10, 15)
pyautogui.click()
pyautogui.doubleClick()
pyautogui.press('enter')
```
上面的代码会将鼠标移动到某个位置并单击鼠标。当需要重复操作（例如下载文件或收集数据）时，非常有用。
<a name="tdEIP"></a>
## 8、Weightedcalcs
Weightedcalcs 用于统计计算。用法从简单的统计数据（例如加权平均值、中位数和标准变化）到加权计数和分布等。
```bash
pip install weightedcalcs
```
使用可用数据计算加权分布。
```python
import seaborn as sns
df = sns.load_dataset('mpg')
import weightedcalcs as wc
calc = wc.Calculator("mpg")
```
然后通过传递数据集并计算预期变量来进行加权计算。
```python
calc.distribution(df, "origin")
origin

# europe  0.208616
# japan   0.257042
# usa     0.534342
# Name: mpg, dtype: float64
```
<a name="QB6fk"></a>
## 9、scikit-posthocs
scikit-posthocs 是一个用于“事后”测试分析的 python 包，通常用于统计分析中的成对比较。该软件包提供了简单的类似 scikit-learn API 来进行分析。
```bash
pip install scikit-posthocs
```
然后从简单的数据集开始，进行 ANOVA 测试。
```python
import statsmodels.api as sa
import statsmodels.formula.api as sfa
import scikit_posthocs as sp
df = sa.datasets.get_rdataset('iris').data
df.columns = df.columns.str.replace('.', '')

lm = sfa.ols('SepalWidth ~ C(Species)', data=df).fit()
anova = sa.stats.anova_lm(lm)
print(anova)
```
```python
df     sum_sq     mean_sq   F        PR(>F)

C (Species) 2.0    11.344933  5.672467  49.1600  4.492017e-17

Residual    147.0  16.962000  0.115388  NaN      NaN
```
获得了 ANOVA 测试结果，但不确定哪个变量类对结果的影响最大，可以使用以下代码进行原因的查看。
```python
sp.posthoc_ttest(df, 
                 val_col='SepalWidth', 
                 group_col='Species', 
                 p_adjust='holm')  
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783884851-c8015bc2-d025-42fd-975d-1eaf2b1fe608.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u51f4f0c3&originHeight=238&originWidth=604&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u79653c27-aff2-4d77-a980-7b0bcf21810&title=)<br />使用 scikit-posthoc，简化了事后测试的成对分析过程并获得了 P 值
<a name="royvv"></a>
## 10、Cerberus
Cerberus 是一个用于数据验证的轻量级 python 包。
```bash
pip install cerberus
```
Cerberus 的基本用法是验证类的结构。
```python
from cerberus import Validator
schema = {'name': {'type': 'string'}, 
          'gender':{'type': 'string'}, 
          'age':{'type':'integer'}}
v = Validator(schema)
```
定义好需要验证的结构后，可以对实例进行验证。
```python
document = {'name': 'john doe', 'gender':'male', 'age': 15}
v.validate(document)

# True
```
如果匹配，则 Validator 类将输出True 。这样可以确保数据结构是正确的。
<a name="gPCZ7"></a>
## 11、ppscore
ppscore 用于计算与目标变量相关的变量的预测能力。该包计算可以检测两个变量之间的线性或非线性关系的分数。分数范围从 0（无预测能力）到 1（完美预测能力）。
```bash
pip install ppscore
```
使用 ppscore 包根据目标计算分数。
```python
import seaborn as sns
import ppscore as pps
df = sns.load_dataset('mpg')
pps.predictors(df, 'mpg')
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783884846-d6a6dc15-cd26-46c8-8968-6b828a35f98f.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u73394e07&originHeight=329&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uded594b0-b2cc-424a-9cb6-169edc1ca5a&title=)<br />结果进行了排序。排名越低变量对目标的预测能力越低。
<a name="TPKIV"></a>
## 12、Maya
Maya 用于尽可能轻松地解析 DateTime 数据。
```bash
pip install maya
```
然后可以使用以下代码轻松获得当前日期。
```python
import maya
now = maya.now()
print(now)
```
还可以为明天日期。
```python
tomorrow = maya.when('tomorrow')
tomorrow.datetime()

# datatime.datatime.(2022, 8, 8, 6, 44, 10, 141499, tzinfo=<UTC>)
```
<a name="bSPLg"></a>
## 13、Pendulum
Pendulum 是另一个涉及 DateTime 数据的 python 包。它用于简化任何 DateTime 分析过程。
```bash
pip install pendulum
```
可以对实践进行任何的操作。
```python
import pendulum
now = pendulum.now("Europe/Berlin")

now.in_timezone("Asia/Tokyo")

now.to_iso8601_string()

now.add(days=2)
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783884900-78b4f233-b82a-4a9e-9b70-aa6c24eef00e.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u1e52a808&originHeight=27&originWidth=728&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udcb69ebd-3e4d-438f-8043-febd8b878ab&title=)
<a name="tVVKn"></a>
## 14、category_encoders
category_encoders 是一个用于类别数据编码（转换为数值数据）的python包。该包是各种编码方法的集合，可以根据需要将其应用于各种分类数据。
```bash
pip install category_encoders
```
可以使用以下示例应用转换。
```python
from category_encoders import BinaryEncoder
import pandas as pd

enc = BinaryEncoder(cols=['origin']).fit(df)
numeric_dataset = enc.transform(df)
numeric_dataset.head()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783885305-6d4247e7-fd3b-4cdd-9411-4ba1990a4846.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u9968ea09&originHeight=238&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u955f9c6a-bc8e-4ecb-9f21-4ee77dd9b0b&title=)
<a name="KkyYz"></a>
## 15、scikit-multilearn
scikit-multilearn 可以用于特定于多类分类模型的机器学习模型。该软件包提供 API 用于训练机器学习模型以预测具有两个以上类别目标的数据集。
```bash
pip install scikit-multilearn
```
利用样本数据集进行多标签KNN来训练分类器并度量性能指标。
```python
from skmultilearn.dataset import load_dataset
from skmultilearn.adapt import MLkNN
import sklearn.metrics as metrics

X_train, y_train, feature_names, label_names = load_dataset('emotions', 'train')
X_test, y_test, _, _ = load_dataset('emotions', 'test')

classifier = MLkNN(k=3)
prediction = classifier.fit(X_train, y_train).predict(X_test)

metrics.hamming_loss(y_test, prediction)
```
```python
emotions:train - exists, not redownloading  
  
emotions:test - exists, not redownloading   

0.2953795379537954
```
<a name="JowNz"></a>
## 16、Multiset
Multiset类似于内置的`set`函数，但该包允许相同的字符多次出现。
```bash
pip install multiset
```
可以使用下面的代码来使用 Multiset 函数。
```python
from multiset import Multiset
set1 = Multiset('aab')
set1

# Multiset({'a': 2, 'b':1})
```
<a name="IXuBT"></a>
## 17、Jazzit
Jazzit 可以在代码出错或等待代码运行时播放音乐。
```bash
pip install jazzit
```
使用以下代码在错误情况下尝试示例音乐。
```python
from jazzit import error_track

@error_track("curb_your_enthusiasm.mp3", wait=5)
def run():
  for num in reversed(range(10)):
      print(10/num)
```
这个包虽然没什么用，但是它的功能是不是很有趣。
<a name="OuDIY"></a>
## 18、handcalcs
handcalcs 用于简化notebook中的数学公式过程。它将任何数学函数转换为其方程形式。
```bash
pip install handcalcs
```
使用以下代码来测试 handcalcs 包。使用 `%%render` 魔术命令来渲染 Latex 。
```python
import handcalcs.render
from math import sqrt
%%rendera = 4
b = 6
c = sqrt(3*a + b/7)
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660784734730-f15092a2-6065-427c-aa22-decd581f51b3.png#clientId=ub7bcb8d1-e5c7-4&from=paste&height=140&id=uc565c31a&originHeight=350&originWidth=803&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15480&status=done&style=shadow&taskId=u96741d25-129d-4aea-ad9d-ad66b95b469&title=&width=321.2)
<a name="yUU7Q"></a>
## 19、NeatText
NeatText 可简化文本清理和预处理过程。它对任何 NLP 项目和文本机器学习项目数据都很有用。
```bash
pip install neattext
```
使用下面的代码，生成测试数据
```python
import neattext as nt 
mytext = "This is the word sample but ,our WEBSITE is https://exaempleeele.com 😊✨."
docx = nt.TextFrame(text=mytext)
```
TextFrame 用于启动 NeatText 类然后可以使用各种函数来查看和清理数据。
```python
docx.describe()
```
```python
Key       Value

Length    : 72

vowels    : 21

consonants: 33

stopwords : 5

punctuations: 6

special_char: 6

tokens(whitespace): 11

tokens(words): 13
```
使用 `describe` 函数，可以显示每个文本统计信息。进一步清理数据，可以使用以下代码。
```python
docx.normalize()
```
<a name="RXIhF"></a>
## 20、Combo
Combo 是一个用于机器学习模型和分数组合的 python 包。该软件包提供了一个工具箱，允许将各种机器学习模型训练成一个模型。也就是可以对模型进行整合。
```bash
pip install combo
```
使用来自 scikit-learn 的乳腺癌数据集和来自 scikit-learn 的各种分类模型来创建机器学习组合。
```python
from sklearn.tree import DecisionTreeClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier

from sklearn.model_selection import train_test_split
from sklearn.datasets import load_breast_cancer

from combo.models.classifier_stacking import Stacking
from combo.utils.data import evaluate_print
```
接下来，看一下用于预测目标的单个分类器。
```python
# Define data file and read X and y
random_state = 42
X, y = load_breast_cancer(return_X_y=True)X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.4,random_state=random_state)
# initialize a group of clfs
classifiers = [DecisionTreeClassifier(random_state=random_state),
                 LogisticRegression(random_state=random_state),
                 KNeighborsClassifier(),
                 RandomForestClassifier(random_state=random_state),
                 GradientBoostingClassifier(random_state=random_state)]
clf_names = ['DT', 'LR', 'KNN', 'RF', 'GBDT']

for i, clf in enumerate(classifiers):
  clf.fit(X_train, y_train)
  y_test_predict = clf.predict(X_test)
  evaluate_print(clf_names[i] + '   |   ', y_test, y_test_predict)
  print()
```
```python
DT   | Accuracy: 0.9386, ROC:0.9383, F1:0.9521
LR   | Accuracy: 0.9693, ROC:0.962,  F1:0.9766
KNN  | Accuracv: 0.9561, ROC:0.9519, F1:0.9662
RF   | Accuracy: 0.9781, ROC:0.9716, F1:0.9833
GBDT | Accuracy: 0.9605, ROC:0.9524, F1:0.9699
```
使用 Combo 包的 Stacking 模型。
```python
clf = Stacking(classifiers, n_folds=4, shuffle_data=False,
                 keep_original=True, use_proba=False,
                 random_state=random_state)
                 
clf.fit(X_train, y_train)
y_test_predict = clf.predict(X_test)

evaluate_print('Stacking | ', y_test, y_test_predict)
```
<a name="ttm3d"></a>
## 21、PyAztro
是否需要星座数据或只是对今天的运气感到好奇？可以使用 PyAztro 来获得这些信息！这个包有幸运数字、幸运标志、心情等等。这是人工智能算命的基础数据
```bash
pip install pyaztro
```
使用以下代码访问今天的星座信息。
```python
import pyaztro
pyaztro.Aztro(sign='gemini').description

"A very sexy visitor will cross your path soon. 
If not today, then within a few days. 
Once they arrive, lots of things will change. 
Your organized schedule, for one. 
Not that you'll mind, of coursel"
```
<a name="VztCa"></a>
## 22、Faker
Faker 可用于简化生成合成数据。许多开发人员使用这个包来创建测试的数据。
```bash
pip install Faker
```
要使用 Faker 包生成合成数据
```python
from faker import Faker
fake = Faker()
```
生成名字
```python
fake.name()

# 'Danielle Cobb'
```
每次从 Faker 类获取 .name 属性时，Faker 都会随机生成数据。
<a name="Alv5y"></a>
## 23、Fairlearn
Fairlearn 用于评估和减轻机器学习模型中的不公平性。该软件包提供了许多查看偏差所必需的 API。
```bash
pip install fairlearn
```
然后可以使用 Fairlearn 的数据集来查看模型中有多少偏差。
```python
from fairlearn.metrics import MetricFrame, selection_rate
from fairlearn.datasets import fetch_adult

data = fetch_adult(as_frame=True)
X = data.data
y_true = (data.target == '>50K') * 1
sex = X['sex']

selection_rates = MetricFrame(metrics=selection_rate,
                            y_true=y_true,
                            y_pred=y_true,
                            sensitive_features=sex)
                            
fig = selection_rates.by_group.plot.bar(
  legend=False, rot=0,
  title='Fraction earning over $50,000')
```
![2022-08-18-09-07-32.857357100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1660784904234-e6cdead7-d93a-4ba6-89e0-5ec461296381.png#clientId=ub7bcb8d1-e5c7-4&from=ui&id=uf44ec57e&originHeight=358&originWidth=476&originalType=binary&ratio=1&rotation=0&showTitle=false&size=512490&status=done&style=shadow&taskId=u522c6cb0-ca77-4ee1-afb9-a0abac23f38&title=)<br />Fairlearn API 有一个 selection_rate 函数，可以使用它来检测组模型预测之间的分数差异，以便可以看到结果的偏差。
<a name="HlRVM"></a>
## 24、tiobeindexpy
tiobeindexpy 用于获取 TIOBE 索引数据。TIOBE 指数是一个编程排名数据，对于开发人员来说是非常重要的因为不想错过编程世界的下一件大事。
```bash
pip install tiobeindexpy
```
可以通过以下代码获得当月前 20 名的编程语言排名。
```python
from tiobeindexpy import tiobeindexpy as tb
df = tb.top_20()
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783885323-dcb5b39b-6ba1-4018-b26d-93bc90bd9fd5.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=ud2acb711&originHeight=956&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub528b343-5705-4e60-a9bf-616a4a21c4c&title=)
<a name="cq7hJ"></a>
## 25、pytrends
pytrends 可以使用 Google API 获取关键字趋势数据。如果想要了解当前的网络趋势或与关键字相关的趋势时，该软件包非常有用。这个需要访问google，所以你懂的。
```bash
pip install pytrends
```
假设想知道与关键字“Present Gift”相关的当前趋势，
```python
from pytrends.request import TrendReq
import pandas as pd
pytrend = TrendReq()

keywords = pytrend.suggestions(keyword='Present Gift')
df = pd.DataFrame(keywords)
df
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783885388-90d0f7b2-210e-47ce-886f-f05b8e41c2e2.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=u61700cf0&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uee1695eb-1c93-4569-86e9-77a2ea60462&title=)<br />该包将返回与关键字相关的前 5 个趋势。
<a name="W4kms"></a>
## 26、visions
visions 是一个用于语义数据分析的 python 包。该包可以检测数据类型并推断列的数据应该是什么。
```bash
pip install visions
```
可以使用以下代码检测数据中的列数据类型。这里使用 seaborn 的 Titanic 数据集。
```python
import seaborn as sns
from visions.functional import detect_type, infer_type
from visions.typesets import CompleteSet
df = sns.load_dataset('titanic')
typeset = CompleteSet()

converting everything to strings
print(detect_type(df, typeset))
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660783885657-bab02767-7acb-4831-8ba7-e62008ccc1c1.png#clientId=ub7bcb8d1-e5c7-4&from=paste&id=ueed90f6e&originHeight=52&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u97085b93-950d-455d-8b89-8cd6bf5cb6b&title=)
<a name="jdwRW"></a>
## 27、Schedule
Schedule 可以为任何代码创建作业调度功能
```bash
pip install schedule
```
例如，想10 秒工作一次：
```python
import schedule
import time

def job():
  print("I'm working...")

schedule.every(10).seconds.do(job)

while True:
  schedule.run_pending()
  time.sleep(1)
```
```python
I'm working...  
I'm working...   
I'm working...   
I'm working...   
I'm working...   
I'm working...   
I'm working...   
I'm working...   
I'm working...   
I'm working...
```
<a name="G9jEx"></a>
## 28、autocorrect
autocorrect 是一个用于文本拼写更正的 python 包，可应用于多种语言。用法很简单，并且对数据清理过程非常有用。
```bash
pip install autocorrect
```
可以使用类似于以下代码进行自动更正。
```python
from autocorrect import Speller
spell = Speller()
spell("I'm not sleaspy and tehre is no place, I'm giong to.")

# "I'm not sleaspy and tehre is no place, 
# I'm giong to."
```
<a name="XHDLi"></a>
## 29、funcy
funcy 包含用于日常数据分析使用的精美实用功能。包中的功能太多了，无法全部展示出来，有兴趣的请查看他的文档。
```bash
pip install funcy
```
这里只展示一个示例函数，用于从可迭代变量中选择一个偶数，如下面的代码所示。
```python
from funcy import select, even
select(even, {i for i in range (20)})

# {0, 2, 4, 6, 8, 10, 12, 14, 16, 18}
```
<a name="ciaW4"></a>
## 30、IceCream
IceCream 可以使调试过程更容易。该软件包在打印/记录过程中提供了更详细的输出。
```bash
pip install icecream
```
可以使用下面代码
```python
from icecream import ic

def some_function(i):
    i = 4 + (1 * 2)/ 10 
    return i + 35

ic(some_function(121))

# 39.2
```
也可以用作函数检查器。
```python
def foo():
  ic()
  
if some_function(12):
  ic()
else:
  ic()

foo()
```
打印的详细程度非常适合分析
<a name="gm5Lm"></a>
## 总结
在本文中，总结了 30个在数据工作中有用的独特 Python 包。大多数软件包易于使用且简单明了，但有些可能功能较多需要进一步阅读其文档，如果有兴趣请去pypi网站搜索并查看该软件包的主页和文档。
