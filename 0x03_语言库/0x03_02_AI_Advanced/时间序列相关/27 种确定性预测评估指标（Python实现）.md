时间序列<br />在时间序列预测中，评价指标的多样性为模型性能的评估带来多角度的参考意义。这里列举了当前已知的**27种**确定性预测评估指标及其Python的实现，其中Python的评估指标函数实现基于numpy库（调用方法：`import numpy as np`）。
<a name="LPzBk"></a>
## 1、误差
```python
def _error(actual: np.ndarray, predicted: np.ndarray):
    """ Simple error """
    return actual - predicted
```
<a name="SCTMI"></a>
## 2、误差百分比
```python
def _percentage_error(actual: np.ndarray, predicted: np.ndarray):
    """ Percentage error """
    return (actual - predicted) / actual
```
<a name="JP0LG"></a>
## 3、均方误差 MSE
```python
def mse(actual: np.ndarray, predicted: np.ndarray):
    """ Mean Squared Error """
    return np.mean(np.square(_error(actual, predicted)))
```
<a name="jC9U7"></a>
## 4、均方根误差 RMSE
```python
def rmse(actual: np.ndarray, predicted: np.ndarray):
    """ Root Mean Squared Error """
    return np.sqrt(mse(actual, predicted))
```
<a name="d0rnr"></a>
## 5、标准化均方根误差 NRMSE
```python
def nrmse(actual: np.ndarray, predicted: np.ndarray):
    """ Normalized Root Mean Squared Error """
    return rmse(actual, predicted) / (actual.max() - actual.min())
```
<a name="Zx8XI"></a>
## 6、平均误差 ME
```python
def me(actual: np.ndarray, predicted: np.ndarray):
    """ Mean Error """
    return np.mean(_error(actual, predicted))
```
<a name="DOaiR"></a>
## 7、平均绝对误差 MAE
```python
def mae(actual: np.ndarray, predicted: np.ndarray):
    """ Mean Absolute Error """
    return np.mean(np.abs(_error(actual, predicted)))
```
<a name="pPSuF"></a>
## 8、中位数绝对误差 MedAE
```python
def mdae(actual: np.ndarray, predicted: np.ndarray):
    """ Median Absolute Error """
    return np.median(np.abs(_error(actual, predicted)))
```
<a name="sOx4w"></a>
## 9、平均百分比误差 MPE
```python
def mpe(actual: np.ndarray, predicted: np.ndarray):
    """ Mean Percentage Error """
    return np.mean(_percentage_error(actual, predicted))
```
<a name="QWEUH"></a>
## 10、平均绝对百分比误差 MAPE
```python
def mape(actual: np.ndarray, predicted: np.ndarray):
    """
    Mean Absolute Percentage Error
    Properties:
        + Easy to interpret
        + Scale independent
        - Biased, not symmetric
        - Undefined when actual[t] == 0
    Note: result is NOT multiplied by 100
    """
    return np.mean(np.abs(_percentage_error(actual, predicted)))
```
<a name="vLwr5"></a>
## 11、中位数绝对误差百分比 MedAPE
```python
def mdape(actual: np.ndarray, predicted: np.ndarray):
    """
    Median Absolute Percentage Error
    Note: result is NOT multiplied by 100
    """
    return np.median(np.abs(_percentage_error(actual, predicted)))
```
<a name="V5iYc"></a>
## 12、对称平均绝对误差百分比 SMAPE
```python
def smape(actual: np.ndarray, predicted: np.ndarray):
    """
    Symmetric Mean Absolute Percentage Error
    Note: result is NOT multiplied by 100
    """
    return np.mean(2.0 * np.abs(actual - predicted) / ((np.abs(actual) + np.abs(predicted)) + EPSILON))
```
<a name="XJvNM"></a>
## 13、对称中位数绝对误差百分比 SMDAPE
```python
def smdape(actual: np.ndarray, predicted: np.ndarray):
    """
    Symmetric Median Absolute Percentage Error
    Note: result is NOT multiplied by 100
    """
    return np.median(2.0 * np.abs(actual - predicted) / (np.abs(actual) + np.abs(predicted)))
```
<a name="l8tkA"></a>
## 14、平均反正切绝对百分比误差 MAAPE
```python
def maape(actual: np.ndarray, predicted: np.ndarray):
    """
    Mean Arctangent Absolute Percentage Error
    Note: result is NOT multiplied by 100
    """
    return np.mean(np.arctan(np.abs((actual - predicted) / actual))
```
<a name="AzD2W"></a>
## 15、平均绝对比例误差 MASE
```python
def mase(actual: np.ndarray, predicted: np.ndarray, seasonality: int = 1):
    """
    Mean Absolute Scaled Error
    Baseline (benchmark) is computed with naive forecasting (shifted by @seasonality)
    """
    return mae(actual, predicted) / mae(actual[seasonality:], _naive_forecasting(actual, seasonality))
```
<a name="HhuPm"></a>
## 16、标准化绝对误差 NAE
```python
def std_ae(actual: np.ndarray, predicted: np.ndarray):
    """ Normalized Absolute Error """
    __mae = mae(actual, predicted)
    return np.sqrt(np.sum(np.square(_error(actual, predicted) - __mae))/(len(actual) - 1))
```
<a name="DwNUJ"></a>
## 17、标准化绝对百分比误差 NAPE
```python
def std_ape(actual: np.ndarray, predicted: np.ndarray):
    """ Normalized Absolute Percentage Error """
    __mape = mape(actual, predicted)
    return np.sqrt(np.sum(np.square(_percentage_error(actual, predicted) - __mape))/(len(actual) - 1))
```
<a name="zyDj8"></a>
## 18、均方根误差百分比 RMSPE
```python
def rmspe(actual: np.ndarray, predicted: np.ndarray):
    """
    Root Mean Squared Percentage Error
    Note: result is NOT multiplied by 100
    """
    return np.sqrt(np.mean(np.square(_percentage_error(actual, predicted))))
```
<a name="BWf1y"></a>
## 19、中位数方根误差百分比 RMedSPE
```python
def rmdspe(actual: np.ndarray, predicted: np.ndarray):
    """
    Root Median Squared Percentage Error
    Note: result is NOT multiplied by 100
    """
    return np.sqrt(np.median(np.square(_percentage_error(actual, predicted))))
```
<a name="Lh2Zw"></a>
## 20、均方根比例误差 RMSSE
```python
def rmsse(actual: np.ndarray, predicted: np.ndarray, seasonality: int = 1):
    """ Root Mean Squared Scaled Error """
    q = np.abs(_error(actual, predicted)) / mae(actual[seasonality:], _naive_forecasting(actual, seasonality))
    return np.sqrt(np.mean(np.square(q)))
```
<a name="OF2gd"></a>
## 21、积分标准方根误差 INRSE
```python
def inrse(actual: np.ndarray, predicted: np.ndarray):
    """ Integral Normalized Root Squared Error """
    return np.sqrt(np.sum(np.square(_error(actual, predicted))) / np.sum(np.square(actual - np.mean(actual))))
```
<a name="v4M7b"></a>
## 22、相对方根误差 RRSE
```python
def rrse(actual: np.ndarray, predicted: np.ndarray):
    """ Root Relative Squared Error """
    return np.sqrt(np.sum(np.square(actual - predicted)) / np.sum(np.square(actual - np.mean(actual))))
```
<a name="gaveI"></a>
## 23、平均相对误差 MRE
```python
def mre(actual: np.ndarray, predicted: np.ndarray, benchmark: np.ndarray = None):
    """ Mean Relative Error """
    return np.mean(_relative_error(actual, predicted, benchmark))
```
<a name="F3XoQ"></a>
## 24、相对绝对值误差 RAE
```python
def rae(actual: np.ndarray, predicted: np.ndarray):
    """ Relative Absolute Error (aka Approximation Error) """
    return np.sum(np.abs(actual - predicted)) / (np.sum(np.abs(actual - np.mean(actual))) + EPSILON)
```
<a name="kMvJC"></a>
## 25、平均相对绝对值误差 MRAE
```python
def mrae(actual: np.ndarray, predicted: np.ndarray, benchmark: np.ndarray = None):
    """ Mean Relative Absolute Error """
    return np.mean(np.abs(_relative_error(actual, predicted, benchmark)))
```
<a name="yMiCM"></a>
## 26、中位数相对绝对值误差 MedAE
```python
def mdrae(actual: np.ndarray, predicted: np.ndarray, benchmark: np.ndarray = None):
    """ Median Relative Absolute Error """
    return np.median(np.abs(_relative_error(actual, predicted, benchmark)))
```
<a name="autbz"></a>
## 27、平均方向精度 MDA
```python
def mda(actual: np.ndarray, predicted: np.ndarray):
	""" Mean Directional Accuracy """
    return np.mean((np.sign(actual[1:] - actual[:-1]) == np.sign(predicted[1:] - predicted[:-1])).astype(int))
```
