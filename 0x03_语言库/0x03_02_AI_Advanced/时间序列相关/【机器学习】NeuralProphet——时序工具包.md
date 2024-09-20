机器学习 
<a name="fq6Om"></a>
## NeuralProphet
<a name="mCVTH"></a>
### 简介
几乎绝大多数做时间序列的朋友都了解Facebook的Prophet模型，因为其在准确性、可解释性等方面有着良好的性能，而且可以为用户自动化许多元素（如超参数选择或特征工程），因而它获得了广泛的应用。介绍一个最新的NeuralProphet时，从名字就可以看出，这个是神经网络和Prophet的结合，与传统的黑盒NN不同，就介绍的NeuralProphet模型集成了Prophet的所有优点，不仅具有不错的可解释性，还有优于Prophet的预测性能。
<a name="e2r3q"></a>
### NeuralProphet
<a name="NvKrW"></a>
#### 1、Prophet
Prophet如果认为是基本自回归的扩展（除了使用lagged的目标值，还对输入变量使用傅立叶变换，可以通过调模型拿到更好的结果）。

- Prophet可以使用额外的信息，不仅仅是target的延迟值；
- 模型能融入节假日信息；
- 可以自动检测趋势的变化；
<a name="eWAV2"></a>
#### 2、NeuralProphet
和许多黑盒子的NN不同，NeuralProphet保留了Prophet的所有优势，同时，通过引入改进的后端（Pytorch代替Stan）和使用自回归网络（AR网络），将神经网络的可扩展性与AR模型的可解释性结合起来，提高其准确性和可扩展性。

- AR网络——它是一个单层网络，经过训练可以模拟时间序列信号中的AR过程，但规模比传统模型大得多。
<a name="ZqyFe"></a>
#### 3、NeuralProphet VS Prophet

- NeuralProphet使用PyTorch的梯度下降进行优化，使得建模速度更快；
- 利用自回归网络对时间序列自相关进行建模；
- 滞后回归器使用单独的前馈神经网络建模；
- NeuralProphet具有可配置的前馈神经网络的非线性深层；
- 模型可调整到特定的预测范围（大于1）；
- 提供自定义的损失函数和度量策略；
<a name="NGjoy"></a>
### 代码
<a name="L8ZVX"></a>
#### 1、数据读取
```python
# !pip install neuralprophet
import pandas as pd
from fbprophet import Prophet
from neuralprophet import NeuralProphet
from sklearn.metrics import mean_squared_error

# plotting
import matplotlib.pyplot as plt

# settings
plt.style.use('seaborn')
plt.rcParams["figure.figsize"] = (16, 8)
df = pd.read_csv('./data/wp_log_peyton_manning.csv')
print(f'The dataset contains {len(df)} observations.')
df.head()
```
The dataset contains 2905 observations.

| <br /> | ds | y |
| --- | --- | --- |
| 0 | 2007-12-10 | 9.590761 |
| 1 | 2007-12-11 | 8.519590 |
| 2 | 2007-12-12 | 8.183677 |
| 3 | 2007-12-13 | 8.072467 |
| 4 | 2007-12-14 | 7.893572 |

```python
df.plot(x='ds', y='y', title='Log daily page views');
```
![2021-08-09-20-39-43-071915.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628512797413-24ca2d25-ebe7-44b8-87be-ac684373b5dc.png#clientId=ud4640863-4cf2-4&from=ui&id=u330e8676&originHeight=561&originWidth=1080&originalType=binary&ratio=1&size=126271&status=done&style=shadow&taskId=uf1e9e0af-ae27-412e-8893-53738158023)
<a name="KdvlM"></a>
#### 2、Prophet预测
```python
test_length = 365
df_train = df.iloc[:-test_length]
df_test = df.iloc[-test_length:]

prophet_model = Prophet()
prophet_model.fit(df_train)
future_df = prophet_model.make_future_dataframe(periods=test_length)
preds_df_1 = prophet_model.predict(future_df)
prophet_model.plot_components(preds_df_1);

prophet_model.plot(preds_df_1);
```
<a name="GfWbh"></a>
#### 3、NeuralProphet
```python
nprophet_model = NeuralProphet()
metrics = nprophet_model.fit(df_train, freq="D")
future_df = nprophet_model.make_future_dataframe(df_train, 
                                                 periods = test_length, 
                                                 n_historic_predictions=len(df_train))
preds_df_2 = nprophet_model.predict(future_df)
nprophet_model.plot(preds_df_2);
```
![2021-08-09-20-39-43-178928.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628512797502-66081950-5941-4e59-b1ed-ce6f78e424b7.png#clientId=ud4640863-4cf2-4&from=ui&id=cf3e4&originHeight=624&originWidth=1080&originalType=binary&ratio=1&size=272052&status=done&style=shadow&taskId=ude8c320b-280f-43d5-844c-1e9a22a765c)
<a name="pM27r"></a>
#### 4、效果比较

- NeuralProphet的效果比Prophet好了很多。
```python
df_test['prophet'] = preds_df_1.iloc[-test_length:].loc[:, 'yhat']
df_test['neural_prophet'] = preds_df_2.iloc[-test_length:].loc[:, 'yhat1']
df_test.set_index('ds', inplace=True)

print('MSE comparison ----')
print(f"Prophet:\t{mean_squared_error(df_test['y'], preds_df_1.iloc[-test_length:]['yhat']):.4f}")
print(f"NeuralProphet:\t{mean_squared_error(df_test['y'], preds_df_2.iloc[-test_length:]['yhat1']):.4f}")
```
```python
MSE comparison ----
Prophet:	0.3576
NeuralProphet:	0.3057
```
<a name="AdPy3"></a>
### 小结
NeuralProphet在时间序列预测的效果上面一般会比Prophet好很多，在遇到时间序列问题的时候强烈建议大家尝试一下。
