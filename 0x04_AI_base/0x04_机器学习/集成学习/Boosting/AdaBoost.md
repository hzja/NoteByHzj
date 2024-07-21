<a name="ba04ae98"></a>
## AdaBoost算法

<a name="2c011106"></a>
### 算法步骤

假设给定一个二分类的训练数据集![](./img/d7522b06223a44717983137fb059db98.svg)其中，每个样本点由实例与标记组成。实例![](./img/b8d942fd4f6ece902f59792f63b42308.svg)，标记![](./img/21d04c1abc43fdb838223f91d09c54d3.svg)，![](./img/bd60d710ce19420dade12257b132cbda.svg)是实例空间，![](./img/12f27527bbcbebfdebc4ffda139ff725.svg)是标记集合。AdaBoost利用以下算法，从训练数据中学习一系列弱分类器或基本分类器，并将这些若分类器线性组合成为一个强分类器。

输入：训练数据集![](./img/d7522b06223a44717983137fb059db98.svg)，其中![](./img/b8d942fd4f6ece902f59792f63b42308.svg)，标记![](./img/21d04c1abc43fdb838223f91d09c54d3.svg)，弱学习算法；

输出：最终分类器![](./img/5d09697085e8b2d48446837da84789a3.svg)

（1）初始化训练数据的权值分布

![](./img/962108161127c7f399756c538ed75c8b.svg)

（2）对![](./img/71b9ad04aff480dd7485124a1a8feff3.svg)

- （a）使用具有权值分布![](./img/9ef394e713e227faae2af8c607f59aff.svg)的训练数据集学习，得到基本分类器
-                  ![](./img/59b0c2a6892cd66d86c68c07442d2d55.svg)
- （b）计算![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)在训练数据上的分类误差率
-                  ![](./img/65342244580824bd4b1aaef3caaa018f.svg)
- （c）计算![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)的系数
-                  ![](./img/c4dc6f2462426e7c43ac01eb8165471e.svg)
- （d）更新训练数据集的权值分布
-                ![](./img/4946ff625e2b4ed38afd896021837e86.svg)
-                 ![](./img/1909d638d9e0d86e4cfd491546e6fb72.svg)
-                 ![](./img/16ef2ca56096182035be0c43634b47c9.svg)

（3）构建基本分类器的线性组合![](./img/f8005cff1bb3644f66c4eefc9b7e2a17.svg)，得到最终分类器<br />![](./img/ffadf033b3a28675bc8708e6b48d1891.svg)

<a name="3c5eda7a"></a>
### 步骤说明

步骤（1）：假设训练数据集具有均匀的权值分布，即每个训练样本在基本分类器的学习中作用相同，这一假设保证第1步能够在原始数据上学习基本分类器![](./img/b972a919b5bd72488e6c04e7b10b4c9d.svg)

步骤（2）：AdaBoost反复学习基本分类器，在每一轮![](./img/a2668ed890275b6761f477b5638ca0fb.svg)顺次地执行下列操作

- （a）使用当前分布![](./img/9ef394e713e227faae2af8c607f59aff.svg)加权的训练数据集，学习基本分类器![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)
- （b）计算基本分类器![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)在加权训练数据集上的分类误差率：
-                 ![](./img/69a8ed3712d6428f69f25b6508c7d839.svg)
- ![](./img/553b855e9b2537af7e39e7c35cc9103e.svg)表示第![](./img/6f8f57715090da2632453988d9a1501b.svg)轮中第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)个实例的权值，![](./img/7f11b2cc7b875862f06d13d32d20575e.svg)。这表明，![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)在加权的训练数据集上的分类误差率是被![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)误分类样本的权值之和，由此可以看出数据权值分布![](./img/9ef394e713e227faae2af8c607f59aff.svg)与基本分类器![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)的分类误差率的关系。
- （c）计算基本分类器![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)的系数![](./img/9fdca6a007df796b924aa43bd2937fdc.svg)。![](./img/9fdca6a007df796b924aa43bd2937fdc.svg)表示![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)在最终分类器中的重要性。由![](./img/c4dc6f2462426e7c43ac01eb8165471e.svg)可知，当![](./img/b25824e2e5403b4568bc85bb8f83d312.svg)时，![](./img/73e014434e12faae85f24b60a7bdae48.svg)，并且![](./img/9fdca6a007df796b924aa43bd2937fdc.svg)随着![](./img/ad5460e91803c5f33707818d28c6af50.svg)的减小而增大，所以分类误差率越小的基本分类器在最终分类器中的作用越大。
- （d）更新训练数据的权值分布为下一轮作准备。
-                ![](./img/1909d638d9e0d86e4cfd491546e6fb72.svg)可写为
-                ![](./img/349bc821cffd4fd8d5653159f73ca3a2.svg)
- 由此可知，被基本分类器![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)误分类样本的权值得以扩大，而被正确分类的样本的权值却得以缩小。两相比较，由![](./img/c4dc6f2462426e7c43ac01eb8165471e.svg)知误分类样本的权值被放大![](./img/8067eb01e52416631db48112ce0869f8.svg)倍。因此，误分类样本在下一轮学习中起更大的作用。不改变所给的训练数据，而不断改变训练数据权值的分布，使得训练数据在基本分类器的学习中起不同的作用，这就是AdaBoost的一个特点

步骤（3）：线性组合![](./img/50bbd36e1fd2333108437a2ca378be62.svg)实现![](./img/69691c7bdcc3ce6d5d8a1361f22d04ac.svg)个基本分类器的加权表决。系数![](./img/9fdca6a007df796b924aa43bd2937fdc.svg)表示了基本分类器![](./img/edeb5520c7164b5f94eb2c06a833f9a4.svg)的重要性，这里，所有![](./img/9fdca6a007df796b924aa43bd2937fdc.svg)之和并不为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)。![](./img/50bbd36e1fd2333108437a2ca378be62.svg)的符号决定实例![](./img/9dd4e461268c8034f5c8564e155c67a6.svg)的类，![](./img/50bbd36e1fd2333108437a2ca378be62.svg)的绝对值表示分类的确信度。利用基本分类的线性组合构建最终分类器是AdaBoost的另一特点。

<a name="8cb94eb1"></a>
### 例子
| 序号 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| x | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
| y | 1 | 1 | 1 | -1 | -1 | -1 | 1 | 1 | 1 | -1 |


步骤（1）：初始化数据权值分布

![](./img/9dc2476d745f301b462e972f10db9828.svg)

步骤（2）：

对![](./img/f3fa7ed3e0e30cc639e9ce8cb1dff9ca.svg)：

- （a）在权值分布为![](./img/323b515dec6e9a6563cad1790f7590bc.svg)的训练数据上，阈值![](./img/9e3669d19b675bd57058fd4664205d2a.svg)取![](./img/8221435bcce913b5c2dc22eaf6cb6590.svg)时分类误差率最低，故基本分类器为
-                ![](./img/fb292e08c4be5d8613ac2a9941b3e1fc.svg)
- （b）![](./img/b972a919b5bd72488e6c04e7b10b4c9d.svg)在训练数据集上的误差率![](./img/0346739e26e80c69b8feb1dd203b9ea0.svg)(6,7,8样本错分，权值均为1)
- （c）计算![](./img/b972a919b5bd72488e6c04e7b10b4c9d.svg)的系数![](./img/277589a42e51bcb8174969158593bb4e.svg)
- （d）更新训练数据的权值分布
-         ![](./img/8511fa99a5f5115748eb44543e226e94.svg)
-         ![](./img/bfe68cfff95bb31e90d89c73dac58d42.svg)
-       ![](./img/2086502e78d4e20e37beed0460a394c7.svg)
- ![](./img/a659270cb359bd8a60e1d8fe6dcf748b.svg)
- 分类器![](./img/ffb07836b619b621b5b4dd16fe55eb39.svg)在训练数据集上有![](./img/eccbc87e4b5ce2fe28308fd9f2a7baf3.svg)个误分类点。

对![](./img/4ae12f01f3c5aeee8f675424b1a9f29a.svg)：

- （a）在权值分布为![](./img/a701eb15aaebdd365911d0df1da9c8f7.svg)的训练数据上，阈值![](./img/9e3669d19b675bd57058fd4664205d2a.svg)取![](./img/845e85fa64a2d6037055beba19fe0bd8.svg)时分类误差率最低，故基本分类器为
-          ![](./img/d8e0c74ee27ed6d4428d41f430af77e1.svg)
- （b）![](./img/bfad482c3ba26559de68ddd8c0e7dac9.svg)在训练数据集上的误差率![](./img/ef0bf6d6d3705258a953929274e406af.svg)(4,5,6样本错分，权值均为0.07143)
- （c）计算![](./img/bfad482c3ba26559de68ddd8c0e7dac9.svg)的系数![](./img/5b691d967cbefa06a6de08f1b0f8ec86.svg)
- （d）更新训练数据的权值分布
- ![](./img/da69e167ff8eae674549101c48b79b64.svg)
- ![](./img/3bf02289709b6274e03e3362e6538c18.svg)
- 分类器![](./img/e9bac4097d9ce7eda7a91892dcae3694.svg)在训练数据集上有![](./img/eccbc87e4b5ce2fe28308fd9f2a7baf3.svg)个误分类点。

对![](./img/c4cfd89dad4555fcd5b85637f02557f0.svg)：

- （a）在权值分布为![](./img/75d3898b277ccd4965cf703c42b2b8b5.svg)的训练数据上，阈值![](./img/9e3669d19b675bd57058fd4664205d2a.svg)取![](./img/ad9995c0d305d650bc1a138f32113af7.svg)时分类误差率最低，故基本分类器为
-             ![](./img/8dd5e2ea3547e23b28a8f6df2d2fd149.svg)
- （b）![](./img/6934f4d82f8afd67b5d68310e746b399.svg)在训练数据集上的误差率![](./img/12b5b63e213932f4bbb5ae1839d5921f.svg)(4-9样本错分，权值见![](./img/75d3898b277ccd4965cf703c42b2b8b5.svg))
- （c）计算![](./img/6934f4d82f8afd67b5d68310e746b399.svg)的系数![](./img/d7254a77992329e3b0cb89f12b85a3ce.svg)
- （d）更新训练数据的权值分布
- ![](./img/cf8f49a75f6a7ded70088895fbc5a82a.svg)
- ![](./img/d14d5e9a346a868ffbd96828d55a1bfa.svg)
- 分类器![](./img/7eac78f6b5dbcb7ea2dcf8081ea01c6f.svg)在训练数据集上有![](./img/cfcd208495d565ef66e7dff9f98764da.svg)个误分类点。

步骤（3）：于是最终分类器为

![](./img/0e6be5565b74cbdbb5ae9a0f05c4e914.svg)

<a name="d1bbeee2"></a>
## AdaBoost训练误差分析

AdaBoost最基本的性质是它能在学习过程中不断减少训练误差，即在训练数据集上的分类学习误差率。关于这个问题有下面的定理：

AdaBoost的训练误差界：AdaBoost算法最终分类器的训练误差界为

![](./img/e7d915022f8bb52eee776a478aff2ccc.svg)

其中，![](./img/0036f0983e1969238f5fef1f9b724993.svg)，![](./img/81ff8f4ce4e6a6426d6602b8e0aeb1d2.svg)，![](./img/ad79a73afa6f2daf612848dfc50cd83e.svg)

证明如下：

（1）当![](./img/0e877b8ff3bea0d389f11254069a3995.svg)时，不等式左边每个误分权值为![](./img/c4ca4238a0b923820dcc509a6f75849b.svg)，不等式右边因为![](./img/1cafc7c713fdc9754430cd3d3a252d98.svg)，所以每个误分权值![](./img/ac92ed4879bb0299fecae080959e3313.svg)，所以不等式![](./img/5c359b0f5f69c0fce5a2c3374693eae8.svg)得证

（2）证等式部分![](./img/cbbe8eae979c6d6e8315decd5e379c32.svg)<br />![](./img/ba3af1a27031512e7155f7ff012b6e2b.svg)

由![](./img/1909d638d9e0d86e4cfd491546e6fb72.svg)和![](./img/16ef2ca56096182035be0c43634b47c9.svg)代入移项得到![](./img/65a9bb3219f1c27e62befafdba2032dd.svg)，代入需要证明式子得

![](./img/a84b9dbd2399201b0a2e12705fa7113c.svg)

这一定理说明，可以在每一轮选取适当的![](./img/3276bd33cf6cc9366c21aea09c8ed0a0.svg)使得![](./img/6a55edbc4b8d80c4a2ae42bd633b7285.svg)最小，从而使训练误差下降最快。

<a name="122f085e"></a>
## [Code实现](https://github.com/fengdu78/lihang-code/blob/master/code/%E7%AC%AC8%E7%AB%A0%20%E6%8F%90%E5%8D%87%E6%96%B9%E6%B3%95%28AdaBoost%29/Adaboost.ipynb)

<a name="0d830788"></a>
### 数据

```python
import numpy as np
import pandas as pd
from sklearn.datasets import load_iris
from sklearn.model_selection  import train_test_split
import matplotlib.pyplot as plt
%matplotlib inline

# data
def create_data():
    iris = load_iris()
    df = pd.DataFrame(iris.data, columns=iris.feature_names)
    df['label'] = iris.target
    df.columns = ['sepal length', 'sepal width', 'petal length', 'petal width', 'label']
    data = np.array(df.iloc[:100, [0, 1, -1]])
    for i in range(len(data)):
        if data[i,-1] == 0:
            data[i,-1] = -1
    # print(data)
    return data[:,:2], data[:,-1]

X, y = create_data()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

plt.scatter(X[:50,0],X[:50,1], label='0')
plt.scatter(X[50:,0],X[50:,1], label='1')
plt.legend()
```



<a name="6f8f142d"></a>
### 手写实现

```python
class AdaBoost:
    def __init__(self, n_estimators=50, learning_rate=1.0):
        self.clf_num = n_estimators
        self.learning_rate = learning_rate
    
    def init_args(self, datasets, labels):
        
        self.X = datasets
        self.Y = labels
        self.M, self.N = datasets.shape
        
        # 弱分类器数目和集合
        self.clf_sets = []
        
        # 初始化weights
        self.weights = [1.0/self.M]*self.M
        
        # G(x)系数 alpha
        self.alpha = []
        
    def _G(self, features, labels, weights):
        m = len(features)
        error = 100000.0 # 无穷大
        best_v = 0.0
        # 单维features
        features_min = min(features)
        features_max = max(features)
        n_step = (features_max - features_min + self.learning_rate) // self.learning_rate
        # print('n_step:{}'.format(n_step))
        direct, compare_array = None, None
        for i in range(1, int(n_step)):
            v = features_min + self.learning_rate * i
            
            if v not in features:
                # 误分类计算
                compare_array_positive = np.array([1 if features[k] > v else -1 for k in range(m)])
                weight_error_positive = sum([weights[k] for k in range(m) if compare_array_positive[k] != labels[k]])
                
                compare_array_nagetive = np.array([-1 if features[k] > v else 1 for k in range(m)])
                weight_error_nagetive = sum([weights[k] for k in range(m) if compare_array_nagetive[k] != labels[k]])

                if weight_error_positive < weight_error_nagetive:
                    weight_error = weight_error_positive
                    _compare_array = compare_array_positive
                    direct = 'positive'
                else:
                    weight_error = weight_error_nagetive
                    _compare_array = compare_array_nagetive
                    direct = 'nagetive'
                    
                # print('v:{} error:{}'.format(v, weight_error))
                if weight_error < error:
                    error = weight_error
                    compare_array = _compare_array
                    best_v = v
        return best_v, direct, error, compare_array
        
    # 计算alpha
    def _alpha(self, error):
        return 0.5 * np.log((1-error)/error)
    
    # 规范化因子
    def _Z(self, weights, a, clf):
        return sum([weights[i]*np.exp(-1*a*self.Y[i]*clf[i]) for i in range(self.M)])
        
    # 权值更新
    def _w(self, a, clf, Z):
        for i in range(self.M):
            self.weights[i] = self.weights[i]*np.exp(-1*a*self.Y[i]*clf[i])/ Z
    
    # G(x)的线性组合
    def _f(self, alpha, clf_sets):
        pass
    
    def G(self, x, v, direct):
        if direct == 'positive':
            return 1 if x > v else -1 
        else:
            return -1 if x > v else 1 
    
    def fit(self, X, y):
        self.init_args(X, y)
        
        for epoch in range(self.clf_num):
            best_clf_error, best_v, clf_result = 100000, None, None
            # 根据特征维度, 选择误差最小的
            for j in range(self.N):
                features = self.X[:, j]
                # 分类阈值，分类误差，分类结果
                v, direct, error, compare_array = self._G(features, self.Y, self.weights)
                
                if error < best_clf_error:
                    best_clf_error = error
                    best_v = v
                    final_direct = direct
                    clf_result = compare_array
                    axis = j
                    
                # print('epoch:{}/{} feature:{} error:{} v:{}'.format(epoch, self.clf_num, j, error, best_v))
                if best_clf_error == 0:
                    break
                
            # 计算G(x)系数a
            a = self._alpha(best_clf_error)
            self.alpha.append(a)
            # 记录分类器
            self.clf_sets.append((axis, best_v, final_direct))
            # 规范化因子
            Z = self._Z(self.weights, a, clf_result)
            # 权值更新
            self._w(a, clf_result, Z)
            
#             print('classifier:{}/{} error:{:.3f} v:{} direct:{} a:{:.5f}'.format(epoch+1, self.clf_num, error, best_v, final_direct, a))
#             print('weight:{}'.format(self.weights))
#             print('\n')
    
    def predict(self, feature):
        result = 0.0
        for i in range(len(self.clf_sets)):
            axis, clf_v, direct = self.clf_sets[i]
            f_input = feature[axis]
            result += self.alpha[i] * self.G(f_input, clf_v, direct)
        # sign
        return 1 if result > 0 else -1
    
    def score(self, X_test, y_test):
        right_count = 0
        for i in range(len(X_test)):
            feature = X_test[i]
            if self.predict(feature) == y_test[i]:
                right_count += 1
        
        return right_count / len(X_test)

X = np.arange(10).reshape(10, 1)
y = np.array([1, 1, 1, -1, -1, -1, 1, 1, 1, -1])

clf = AdaBoost(n_estimators=3, learning_rate=0.5)
clf.fit(X, y)

X, y = create_data()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33)

clf = AdaBoost(n_estimators=10, learning_rate=0.2)
clf.fit(X_train, y_train)
clf.score(X_test, y_test)

# 100次结果
result = []
for i in range(1, 101):
    X, y = create_data()
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33)
    clf = AdaBoost(n_estimators=100, learning_rate=0.2)
    clf.fit(X_train, y_train)
    r = clf.score(X_test, y_test)
    # print('{}/100 score：{}'.format(i, r))
    result.append(r)

print('average score:{:.3f}%'.format(sum(result)))
```

<a name="904b59e8"></a>
### sklearn实现

[https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.AdaBoostClassifier.html](https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.AdaBoostClassifier.html)

```
from sklearn.ensemble import AdaBoostClassifier
clf = AdaBoostClassifier(n_estimators=100, learning_rate=0.5)
clf.fit(X_train, y_train)

clf.score(X_test, y_test)
```
