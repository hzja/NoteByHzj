Python<br />在进行数据科学时，可能会浪费大量时间编码并等待计算机运行某些东西。所以选择了一些 Python 库，可以节省宝贵的时间。
<a name="JHfY4"></a>
## 1、Optuna
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658969804337-fea82928-6374-4d2a-984d-62b5cb35ac01.jpeg#clientId=ud11e3d41-078a-4&from=paste&id=udb33156a&originHeight=640&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ube52d747-979b-4f54-8c6a-3b8ed78b80e&title=)<br />Optuna 是一个开源的超参数优化框架，它可以自动为机器学习模型找到最佳超参数。<br />最基本的（也可能是众所周知的）替代方案是 sklearn 的 GridSearchCV，它将尝试多种超参数组合并根据交叉验证选择最佳组合。<br />GridSearchCV 将在先前定义的空间内尝试组合。例如，对于随机森林分类器，可能想要测试几个不同的树的最大深度。GridSearchCV 会提供每个超参数的所有可能值，并查看所有组合。<br />Optuna会在定义的搜索空间中使用自己尝试的历史来确定接下来要尝试的值。它使用的方法是一种称为“Tree-structured Parzen Estimator”的贝叶斯优化算法。<br />这种不同的方法意味着它不是无意义的地尝试每一个值，而是在尝试之前寻找最佳候选者，这样可以节省时间，否则这些时间会花在尝试没有希望的替代品上（并且可能也会产生更好的结果）。<br />最后，它与框架无关，这意味着您可以将它与 TensorFlow、Keras、PyTorch 或任何其他 ML 框架一起使用。
<a name="kpnWz"></a>
## 2、ITMO_FS
ITMO_FS 是一个特征选择库，它可以为 ML 模型进行特征选择。拥有的观察值越少，就越需要谨慎处理过多的特征，以避免过度拟合。所谓“谨慎”意思是应该规范模型。通常一个更简单的模型（更少的特征），更容易理解和解释。<br />ITMO_FS 算法分为 6 个不同的类别：监督过滤器、无监督过滤器、包装器、混合、嵌入式、集成（尽管它主要关注监督过滤器）。<br />“监督过滤器”算法的一个简单示例是根据特征与目标变量的相关性来选择特征。“backward selection”，可以尝试逐个删除特征，并确认这些特征如何影响模型预测能力。<br />这是一个关于如何使用 ITMO_FS 及其对模型分数的影响的普通示例：
```python
>>> from sklearn.linear_model import SGDClassifier 
>>> from ITMO_FS.embedded import MOS 
 
>>> X, y = make_classification(n_samples=300, n_features=10, random_state=0, n_informative=2) 
>>> sel = MOS() 
>>> trX = sel.fit_transform(X, y, smote=False) 
 
>>> cl1 = SGDClassifier() 
>>> cl1.fit(X, y) 
>>> cl1.score(X, y) 
0.9033333333333333 
 
>>> cl2 = SGDClassifier() 
>>> cl2.fit(trX, y) 
>>> cl2.score(trX, y) 
0.9433333333333334
```
ITMO_FS是一个相对较新的库，因此它仍然有点不稳定，但仍然建议尝试一下。
<a name="hZ29m"></a>
## 3、shap-hypetune
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658969804346-04d5673d-2efd-4fcf-a3b6-0be83ea6564c.png#clientId=ud11e3d41-078a-4&from=paste&id=u67ac9e41&originHeight=466&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u586c811b-4ed7-456c-b18b-64daa67059f&title=)<br />到目前为止，已经看到了用于特征选择和超参数调整的库，但为什么不能同时使用两者呢？这就是 shap-hypetune 的作用。<br />从了解什么是“SHAP”开始：<br />“SHAP（SHapley Additive exPlanations）是一种博弈论方法，用于解释任何机器学习模型的输出。”<br />SHAP 是用于解释模型的最广泛使用的库之一，它通过产生每个特征对模型最终预测的重要性来工作。<br />另一方面，shap-hypertune 受益于这种方法来选择最佳特征，同时也选择最佳超参数。为什么要合并在一起？因为没有考虑它们之间的相互作用，独立地选择特征和调整超参数可能会导致次优选择。同时执行这两项不仅考虑到了这一点，而且还节省了一些编码时间（尽管由于搜索空间的增加可能会增加运行时间）。<br />搜索可以通过 3 种方式完成：网格搜索、随机搜索或贝叶斯搜索（另外，它可以并行化）。<br />但是，shap-hypertune 仅适用于梯度提升模型！
<a name="F8DLx"></a>
## 4、PyCaret
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658969804349-c739b4a6-d68f-48a4-b3a0-ce509e290569.jpeg#clientId=ud11e3d41-078a-4&from=paste&id=uf12d8469&originHeight=511&originWidth=828&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1e462d90-153e-413a-9b1e-21d7b163c22&title=)<br />PyCaret 是一个开源、低代码的机器学习库，可自动执行机器学习工作流。它涵盖探索性数据分析、预处理、建模（包括可解释性）和 MLOps。<br />看看他们网站上的一些实际示例，看看它是如何工作的：
```python
# load dataset 
from pycaret.datasets import get_data 
diabetes = get_data('diabetes') 
 
# init setup 
from pycaret.classification import * 
clf1 = setup(data = diabetes, target = 'Class variable') 
 
# compare models 
best = compare_models()
```
只需几行代码，就可以尝试多个模型，并在整个主要分类指标中对它们进行了比较。<br />它还允许创建一个基本的应用程序来与模型进行交互：
```python
from pycaret.datasets import get_data 
juice = get_data('juice') 
from pycaret.classification import * 
exp_name = setup(data = juice,  target = 'Purchase') 
lr = create_model('lr') 
create_app(lr)
```
最后，可以轻松地为模型创建 API 和 Docker 文件：
```python
from pycaret.datasets import get_data 
juice = get_data('juice') 
from pycaret.classification import * 
exp_name = setup(data = juice,  target = 'Purchase') 
lr = create_model('lr') 
create_api(lr, 'lr_api') 
create_docker('lr_api')
```
没有比这更容易的了，对吧？<br />PyCaret是一个非常完整的库，在这里很难涵盖所有内容，建议现在下载并开始使用它来了解一些 其在实践中的能力。
<a name="zqdP9"></a>
## 5、floWeaver
FloWeaver 可以从流数据集中生成桑基图。如果不知道什么是桑基图，这里有一个例子：<br />在显示转化漏斗、营销旅程或预算分配的数据时，它们非常有用（上例）。入口数据应采用以下格式：“源 x 目标 x 值”，只需一行代码即可创建此类图（非常具体，但也非常直观）。
<a name="GYYRD"></a>
## 6、Gradio
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658969804340-363de95d-e0d7-46dc-ad19-e6b123ee6f33.jpeg#clientId=ud11e3d41-078a-4&from=paste&id=ub7e78307&originHeight=630&originWidth=630&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9901a24-51a5-462b-8d5b-1901660869d&title=)<br />如果阅读过敏捷数据科学，就会知道拥有一个让最终用户从项目开始就与数据进行交互的前端界面是多么有帮助。一般情况下在Python中最常用是 Flask，但它对初学者不太友好，它需要多个文件和一些 html、css 等知识。Gradio 允许您通过设置输入类型（文本、复选框等）、功能和输出来创建简单的界面。尽管它似乎不如 Flask 可定制，但它更直观。由于 Gradio 现在已经加入 Huggingface，可以在互联网上永久托管 Gradio 模型，而且是免费的！
<a name="VOacn"></a>
## 7、Terality
理解 Terality 的最佳方式是将其视为“Pandas ，但速度更快”。这并不意味着完全替换 pandas 并且必须重新学习如何使用df：Terality 与 Pandas 具有完全相同的语法。实际上，他们甚至建议“import Terality as pd”，并继续按照以前的习惯的方式进行编码。<br />它快多少？他们的网站有时会说它快 30 倍，有时快 10 到 100 倍。<br />另一个重要是 Terality 允许并行化并且它不在本地运行，这意味着您的 8GB RAM 笔记本电脑将不会再出现 MemoryErrors！<br />但它在背后是如何运作的呢？理解 Terality 的一个很好的比喻是可以认为他们在本地使用的 Pandas 兼容的语法并编译成 Spark 的计算操作，使用Spark进行后端的计算。所以计算不是在本地运行，而是将计算任务提交到了他们的平台上。<br />那有什么问题呢？每月最多只能免费处理 1TB 的数据。如果需要更多则必须每月至少支付 49 美元。1TB/月对于测试工具和个人项目可能绰绰有余，但如果需要它来实际公司使用，肯定是要付费的。
<a name="bII0g"></a>
## 8、torch-handle
如果是Pytorch的使用者，可以试试这个库。<br />torchhandle是一个PyTorch的辅助框架。它将PyTorch繁琐和重复的训练代码抽象出来，使得数据科学家们能够将精力放在数据处理、创建模型和参数优化，而不是编写重复的训练循环代码。使用torchhandle，可以让代码更加简洁易读，让开发任务更加高效。<br />torchhandle将Pytorch的训练和推理过程进行了抽象整理和提取，只要使用几行代码就可以实现PyTorch的深度学习管道。并可以生成完整训练报告，还可以集成tensorboard进行可视化。
```python
from collections import OrderedDict 
import torch 
from torchhandle.workflow import BaseConpython 
 
 
class Net(torch.nn.Module): 
    def __init__(self, ): 
        super().__init__() 
        self.layer = torch.nn.Sequential(OrderedDict([ 
            ('l1', torch.nn.Linear(10, 20)), 
            ('a1', torch.nn.ReLU()), 
            ('l2', torch.nn.Linear(20, 10)), 
            ('a2', torch.nn.ReLU()), 
            ('l3', torch.nn.Linear(10, 1)) 
        ])) 
 
    def forward(self, x): 
        x = self.layer(x) 
        return x 
 
num_samples, num_features = int(1e4), int(1e1) 
X, Y = torch.rand(num_samples, num_features), torch.rand(num_samples) 
dataset = torch.utils.data.TensorDataset(X, Y) 
trn_loader = torch.utils.data.DataLoader(dataset, batch_size=64, num_workers=0, shuffle=True) 
loaders = {"train": trn_loader, "valid": trn_loader} 
device = 'cuda' if torch.cuda.is_available() else 'cpu' 
 
model = {"fn": Net} 
criterion = {"fn": torch.nn.MSELoss} 
optimizer = {"fn": torch.optim.Adam, 
             "args": {"lr": 0.1}, 
             "params": {"layer.l1.weight": {"lr": 0.01}, 
                        "layer.l1.bias": {"lr": 0.02}} 
             } 
scheduler = {"fn": torch.optim.lr_scheduler.StepLR, 
             "args": {"step_size": 2, "gamma": 0.9} 
             } 
 
c = BaseConpython(model=model, 
                criterion=criterion, 
                optimizer=optimizer, 
                scheduler=scheduler, 
                conpython_tag="ex01") 
train = c.make_train_session(device, dataloader=loaders) 
train.train(epochs=10)
```
定义一个模型，设置数据集，配置优化器、损失函数就可以自动训练了，是不是和TF差不多了。
