时间序列
<a name="KKNVj"></a>
## 时间序列王器-Kats
<a name="pvQvO"></a>
### 简介
时间序列分析建模是数据科学和机器学习的一个重要的领域，在电子商务、金融、供应链管理、医学、气象、能源、天文等诸多领域有着广泛的应用。而对于时间序列的分析以及建模目前也有非常多的技术，但相对散乱，本次FaceBook开源了Kats，它是第一个开发标准并连接时间序列分析各个领域的综合Python库，用户可以在这里探索其时间序列数据的基本特征，预测未来值，监视异常，并将其合并到ML模型和pipeline中。<br />在上个月，FaceBook开园了一款新的分析时间序列数据的library--Kats，Kats是一款：

- 轻量级的、易于使用的、通用的时间序列分析框架；

包括

- 预测、异常检测、多元分析和特征提取/嵌入。

Kats是第一个用于一般时间序列分析的综合Python库，它提供了经典和高级的时间序列数据建模技术。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628659435879-6d12f068-1477-4ca6-8521-78b0642265e3.png#clientId=ud3ad0df2-65c0-4&from=paste&height=123&id=u172a9832&originHeight=370&originWidth=2096&originalType=binary&ratio=1&size=66947&status=done&style=none&taskId=u86f7b37b-6913-4aae-a2bd-41b90672236&width=698.6666666666666)<br />[https://github.com/facebookresearch/Kats](https://github.com/facebookresearch/Kats)
<a name="W3Kuj"></a>
## Kats
<a name="WR88E"></a>
### 1. Kats的优势
Kats是一个分析时间序列数据的工具箱，特点是：

- 轻量级；
- 易于使用；
- 通用；

可以用来执行时间序列分析。
<a name="DSwFB"></a>
### 2. Kats的功能
Kats的核心四大功能包括：

- 模型预测：Kats提供了一套完整的预测工具，包括10+个单独的预测模型、ensembling、自监督学习（meta-learning）模型、backtesting、超参数调整和经验预测区间。
- 检测：Kats支持检测时间序列数据的各种模式的功能，包括季节性、异常值、变化点和缓慢的趋势变化检测。
- 特征提取与嵌入：Kats中的时间序列特征（TSFeature）提取模块可以产生65个具有明确统计定义的特征，这些特征可以应用于大多数机器学习（ML）模型，如分类和回归。
- 使用的功能：Kats还提供了一组有用的实用程序，例如时间序列模拟器。
<a name="jlNOS"></a>
## 代码
<a name="X95ey"></a>
### 1、预测案例
```python
# !pip install kats

import pandas as pd

from kats.consts import TimeSeriesData
from kats.models.prophet import ProphetModel, ProphetParams

# take `air_passengers` data as an example
air_passengers_df = pd.read_csv(
    "./data/air_passengers.csv",
    header=0,
    names=["time", "passengers"],
)
air_passengers_df.head()
```
| <br /> | time | passengers |
| --- | --- | --- |
| 0 | 1949-01-01 | 112 |
| 1 | 1949-02-01 | 118 |
| 2 | 1949-03-01 | 132 |
| 3 | 1949-04-01 | 129 |
| 4 | 1949-05-01 | 121 |

```python
# convert to TimeSeriesData object
air_passengers_ts = TimeSeriesData(air_passengers_df)
# create a model param instance
params = ProphetParams(seasonality_mode='multiplicative') # additive mode gives worse results
# create a prophet model instance
m = ProphetModel(air_passengers_ts, params)
# fit model simply by calling m.fit()
m.fit()
# make prediction for next 30 month
fcst = m.predict(steps=30, freq="MS")

INFO:fbprophet:Disabling weekly seasonality. Run prophet with weekly_seasonality=True to override this.
INFO:fbprophet:Disabling daily seasonality. Run prophet with daily_seasonality=True to override this.

fcst.tail()
```
| <br /> | time | fcst | fcst_lower | fcst_upper |
| --- | --- | --- | --- | --- |
| 25 | 1963-02-01 | 504.226854 | 488.814245 | 518.102516 |
| 26 | 1963-03-01 | 585.357522 | 569.323618 | 600.734632 |
| 27 | 1963-04-01 | 563.669453 | 548.299180 | 578.251332 |
| 28 | 1963-05-01 | 586.795969 | 571.121988 | 602.562586 |
| 29 | 1963-06-01 | 668.096474 | 650.864940 | 685.632700 |

<a name="Xs5m6"></a>
### 2、时间序列检测案例

- 异常改变点的检测
```python
import numpy as np
from kats.consts import TimeSeriesData
from kats.detectors.cusum_detection import CUSUMDetector

# simulate time series with increase
np.random.seed(10)
df_increase = pd.DataFrame(
    {
        'time': pd.date_range('2019-01-01', '2019-03-01'),
        'increase':np.concatenate([np.random.normal(1,0.2,30), np.random.normal(2,0.2,30)]),
    }
)

# convert to TimeSeriesData object
timeseries = TimeSeriesData(df_increase)

# run detector and find change points
change_points = CUSUMDetector(timeseries).detector()

df_increase.groupby('time')['increase'].first().plot()



change_points

[(TimeSeriesChangePoint(start_time: 2019-01-30 00:00:00, end_time: 2019-01-30 00:00:00, confidence: 1.0),
  <kats.detectors.cusum_detection.CUSUMMetadata at 0x7fa9449fe910>)]
```
<a name="CHd4s"></a>
### 3、时间序列特征
```python
# Initiate feature extraction class
from kats.tsfeatures.tsfeatures import TsFeatures
# take `air_passengers` data as an example
air_passengers_df = pd.read_csv(
    "./data/air_passengers.csv",
    header=0,
    names=["time", "passengers"],
)

# convert to TimeSeriesData object
air_passengers_ts = TimeSeriesData(air_passengers_df)

# calculate the TsFeatures
features = TsFeatures().transform(air_passengers_ts) 

features
```
```json
{
  'length': 144,
  'mean': 280.2986111111111,
  'var': 14291.97333140432,
  'entropy': 0.4287365561752448,
  'lumpiness': 3041164.5629058965,
  'stability': 12303.627266589507,
  'flat_spots': 2,
  'hurst': -0.08023291030513455,
  'std1st_der': 27.206287853461966,
  'crossing_points': 7,
  'binarize_mean': 0.4444444444444444,
  'unitroot_kpss': 0.12847508180149445,
  'heterogeneity': 126.06450625819339,
  'histogram_mode': 155.8,
  'linearity': 0.853638165603188,
  'trend_strength': 0.9383301875692747,
  'seasonality_strength': 0.3299338017939569,
  'spikiness': 111.69732482853489,
  'peak': 6,
  'trough': 3,
  'level_shift_idx': 118,
  'level_shift_size': 15.599999999999966,
  'y_acf1': 0.9480473407524915,
  'y_acf5': 3.392072131604336,
  'diff1y_acf1': 0.30285525815216935,
  'diff1y_acf5': 0.2594591065999471,
  'diff2y_acf1': -0.19100586757092733,
  'diff2y_acf5': 0.13420736423784568,
  'y_pacf5': 1.0032882494015292,
  'diff1y_pacf5': 0.21941234780081417,
  'diff2y_pacf5': 0.2610103428699484,
  'seas_acf1': 0.6629043863684492,
  'seas_pacf1': 0.1561695525558896,
  'firstmin_ac': 8,
  'firstzero_ac': 52,
  'holt_alpha': 0.995070674288148,
  'holt_beta': 0.0042131109997650676,
  'hw_alpha': 0.9999999850988388,
  'hw_beta': 6.860710750514223e-16,
  'hw_gamma': 1.3205838720422503e-08
}
```
