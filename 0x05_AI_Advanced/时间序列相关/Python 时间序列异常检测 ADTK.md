<a name="AnbSK"></a>
## 1、adtk简介
智能运维AIOps的数据基本上都是**时间序列形式**的，而异常检测告警是AIOps中重要组成部分。最近在处理时间序列数据时有使用到adtk这个python库，在这里和大家做下分享。
<a name="T5SuR"></a>
### 什么是adtk?
adtk（Anomaly Detection Toolkit）是**无监督异常检测**的python工具包，它提供常用算法和处理函数：

- 简单有效的异常检测算法(**detector)**
- 异常特征加工(**transformers**)
- 处理流程控制(**Pipe**)
<a name="uq57L"></a>
## 2、安装
```bash
pip install adtk
```
<a name="OFjIT"></a>
## 3、adtk数据要求
时间序列的数据主要包括时间和相应的指标（如cpu，内存，数量等）。python中数据分析一般都是pandas的DataFrame，adtk要求输入数据的**索引必须**是DatetimeIndex。<br />pandas提供了时间序列的时间生成和处理方法。

- `pd.date_range`
```python
stamps = pd.date_range("2012-10-08 18:15:05", periods=4, freq="D")
# DatetimeIndex(['2012-10-08 18:15:05', '2012-10-09 18:15:05',
#           '2012-10-10 18:15:05', '2012-10-11 18:15:05'],
#          dtype='datetime64[ns]', freq='D')
```

- `pd.Timestamp`
```python
tmp = pd.Timestamp("2018-01-05") + pd.Timedelta("1 day")
print(tmp, tmp.timestamp(), tmp.strftime('%Y-%m-%d'))
# 2018-01-06 00:00:00 1515196800.0 2018-01-06
pd.Timestamp( tmp.timestamp(), unit='s', tz='Asia/Shanghai')
# Timestamp('2018-01-06 08:00:00+0800', tz='Asia/Shanghai')
```

- `pd.to_datetime`

adtk提供是`validate_series`来验证时间序列数据的有效性，如是否按时间顺序
```python
import pandas as pd
from adtk.data import validate_series
from adtk.visualization import plot
df = pd.read_csv('./data/nyc_taxi.csv', index_col="timestamp", parse_dates=True)
df = validate_series(df)
plot(df)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474408-a493a78a-14cd-4e8a-8e51-e9b8e166fa34.png#averageHue=%23f4f1ee&clientId=ubf02af9b-badd-4&from=paste&id=u00985277&originHeight=198&originWidth=214&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93390ab3-ba89-4e5f-b9bf-ccf3c8ab997&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474399-bce8b366-946c-401e-b8c3-6f25a9cbc10f.png#averageHue=%23d5e4ef&clientId=ubf02af9b-badd-4&from=paste&id=u980ef598&originHeight=266&originWidth=972&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub620f8b1-502a-4701-970d-f1263a96683&title=)
<a name="v6Deu"></a>
## 4、异常特征加工（transformers）
adtk中transformers提供了许多时间序列特征加工的方法：

- 一般获取时间序列的特征，通常会按照**时间窗口**在滑动，采集时间窗口上的**统计特征**；
- 还有对于季节性趋势做**分解**，区分哪些是季节性的部分，哪些是趋势的部分
- 时间序列**降维**映射：对于细粒度的时间序列数据，数据量大，对于检测算法来说效率不高。降维方法能保留时间序列的主要趋势等特征同时，降低维数，提供时间效率。这个对于用CNN的方式来进行时间序列分类特别有效，adtk主要提供基于pca的降维和重构方法,主要应用于多维时间序列。
<a name="IODsP"></a>
### 4.1 滑动窗口
atdk提供单个宽口`RollingAggregate`和2个窗口`DoubleRollingAggregate`的滑动方式。统计特征支持均值，中位数，汇总，最大值，最小值，分位数， 方差，标准差，偏度，峰度，直方图 等，`['mean', 'median', 'sum', 'min', 'max', 'quantile', 'iqr', 'idr', 'count', 'nnz', 'nunique', 'std', 'var', 'skew', 'kurt', 'hist']`其中

- '**iqr**': 是分位数 75% 和 25%差值
- '**idr**': 是分位数 90% 和 10%插值
<a name="jlQW5"></a>
#### RollingAggregate
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474369-af4db22b-d86e-4f9d-bf58-cc3b49f58471.png#averageHue=%23fceee2&clientId=ubf02af9b-badd-4&from=paste&id=u82960b93&originHeight=476&originWidth=957&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc346aef9-b153-4934-8d2e-5f4614c4ed1&title=)
<a name="sl4Nc"></a>
#### DoubleRollingAggregate
提供了**两个窗口**之间统计特征的差异特征，如前5分钟和后5分钟，均值的差值等。agg参数和`RollingAggregate`中一致，新增的参数diff主要衡量差距的函数：
```python
import pandas as pd
from adtk.data import validate_series
from adtk.transformer import DoubleRollingAggregate
s = pd.read_csv('./data/ec2_cpu_utilization_53ea38.csv', index_col="timestamp", parse_dates=True)
s = validate_series(s)

s_transformed = DoubleRollingAggregate(
    agg="median",
    window=5,
    diff="diff").transform(s)
```

- '**diff**': 后减去前
- '**rel_diff**': Relative difference between values of aggregated metric (right minus left divided left). Only applicable if the aggregated metric is scalar.
- '**abs_rel_diff**': （后-前）/前， 相对差值
- '**l1**': l1正则
- '**l2**': l2正则

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474373-bc47e1ac-8766-4dd3-95e9-faf9fc348994.png#averageHue=%23fcfcfc&clientId=ubf02af9b-badd-4&from=paste&id=u3c6a55f1&originHeight=267&originWidth=972&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u18b9fd3a-f149-4e05-8306-77b69c6a592&title=)
<a name="L79Vz"></a>
### 4.2 季节性拆解
时间序列可拆解成趋势性，季节性和残差部分。atdk中`ClassicSeasonalDecomposition`提供了这三个部分拆解，并移除趋势和季节性部分，返回残差部分。

- **freq**: 设置季节性的周期
- **trend**：可以设置是否保留趋势性
```python
from adtk.transformer import ClassicSeasonalDecomposition

s = pd.read_csv('./data/nyc_taxi.csv', index_col="timestamp", parse_dates=True)
s = validate_series(s)

s_transformed = ClassicSeasonalDecomposition().fit_transform(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474352-d9dea691-d391-464e-9196-7eebcd912171.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=u1d89c770&originHeight=260&originWidth=982&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u197119cd-bcc5-40c8-82ea-6457c5bd8f4&title=)
```python
s_transformed = ClassicSeasonalDecomposition(trend=True).fit_transform(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474732-aaad9a41-3462-4819-a7db-3baeb80a4c7d.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=u71e13fa6&originHeight=252&originWidth=975&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf218cf70-7173-4ccf-968b-0db71d5c84e&title=)
<a name="sy7hZ"></a>
### 4.3 降维和重构
adtk提供的pca对数据进行降维到主成分`PcaProjection`和重构方法`PcaReconstruction`。
```python
df = pd.read_csv('./data/generator.csv', index_col="Time", parse_dates=True)
df = validate_series(df)

from adtk.transformer import PcaProjection
s = PcaProjection(k=1).fit_transform(df)
plot(pd.concat([df, s], axis=1), ts_linewidth=1, ts_markersize=3, curve_group=[("Speed (kRPM)", "Power (kW)"), "pc0"]);
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474785-9cbc95fd-86b4-4453-ac13-b1b900c7f265.png#averageHue=%23fcfaf9&clientId=ubf02af9b-badd-4&from=paste&id=uaf249542&originHeight=330&originWidth=661&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b80dc66-5d73-4f24-afd9-64537e556e5&title=)
```python
from adtk.transformer import PcaReconstruction
df_transformed = PcaReconstruction(k=1).fit_transform(df).rename(columns={"Speed (kRPM)": "Speed reconstruction (kRPM)", "Power (kW)": "Power reconstruction (kW)"})
plot(pd.concat([df, df_transformed], axis=1), ts_linewidth=1, ts_markersize=3, curve_group=[("Speed (kRPM)", "Power (kW)"), ("Speed reconstruction (kRPM)", "Power reconstruction (kW)")]);
../_images/notebooks_demo_99_0.png
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474800-cde083f0-c278-46a4-8ae8-910c7fb27f31.png#averageHue=%23fbf8f7&clientId=ubf02af9b-badd-4&from=paste&id=ua08580d0&originHeight=329&originWidth=680&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u894807da-6906-4e07-a1a1-9c5fd8235aa&title=)
<a name="KJRkm"></a>
## 5、异常检测算法（detector）
adtk提供的主要是**无监督或者基于规则**的时间序列检测算法，可以用于常规的异常检测。
<a name="oONcY"></a>
### 检测离群点
离群点是和普通数据**差异极大**的数据点。adtk主要提供了包括 `adtk.detector.ThresholdAD` `adtk.detector.QuantileAD` `adtk.detector.InterQuartileRangeAD` `adtk.detector.GeneralizedESDTestAD`的检测算法。
<a name="Omyhl"></a>
#### ThresholdAD
```python
"""
adtk.detector.ThresholdAD(low=None, high=None)
参数：
low：下限，小于此值，视为异常
high：上限，大于此值，视为异常
原理：通过认为设定上下限来识别异常
总结：固定阈值算法
"""

from adtk.detector import ThresholdAD
threshold_ad = ThresholdAD(high=30, low=15)
anomalies = threshold_ad.detect(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474898-048a4e4b-7b46-45d7-bfa9-fe5b447d6161.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=u12269708&originHeight=181&originWidth=679&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud17152cc-fe42-47d9-842c-169fd469918&title=)
<a name="yKdQf"></a>
#### QuantileAD
```python
"""
adtk.detector.QuantileAD(low=None, high=None)
参数：
low：分位下限，范围(0,1)，当low=0.25时，表示Q1
high：分位上限，范围(0,1)，当low=0.25时，表示Q3
原理：通过历史数据计算出给定low与high对应的分位值Q_low,Q_high，小于Q_low或大于Q_high，视为异常
总结：分位阈值算法
"""

from adtk.detector import QuantileAD
quantile_ad = QuantileAD(high=0.99, low=0.01)
anomalies = quantile_ad.fit_detect(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683733164877-eea770a2-8fdd-46e5-b18e-7a0dfab71d11.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=u06a96f6e&originHeight=198&originWidth=658&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u538ff335-70a0-4226-b27e-bfca39c4d6d&title=)
<a name="AJiYt"></a>
#### InterQuartileRangeAD
```python
"""
adtk.detector.InterQuartileRangeAD(c=3.0)
参数：
c：分位距的系数，用来确定上下限，可为float，也可为(float,float)
原理：
当c为float时，通过历史数据计算出 Q3+c*IQR 作为上限值，大于上限值视为异常
当c=(float1,float2)时，通过历史数据计算出 (Q1-c1*IQR, Q3+c2*IQR) 作为正常范围，不在正常范围视为异常
总结：箱线图算法
"""

from adtk.detector import InterQuartileRangeAD
iqr_ad = InterQuartileRangeAD(c=1.5)
anomalies = iqr_ad.fit_detect(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683733177393-447497d5-3a5a-406f-ac17-cc70b874ecc5.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=ub38e0474&originHeight=180&originWidth=670&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8ccbb454-d884-46ca-a095-8b8739985a0&title=)
<a name="PW1Sg"></a>
#### GeneralizedESDTestAD
```python
"""
adtk.detector.GeneralizedESDTestAD(alpha=0.05)
参数：
alpha：显著性水平 (Significance level)，alpha越小，表示识别出的异常约有把握是真异常
原理:将样本点的值与样本的均值作差后除以样本标准差，取最大值，通过t分布计算阈值，对比阈值确定异常点

计算步骤简述：
设置显著水平alpha，通常取0.05
指定离群比例h，若h=5%，则表示50各样本中存在离群点数为2
计算数据集的均值mu与标准差sigma，将所有样本与均值作差，取绝对值，再除以标准差，找出最大值，得到esd_1
在剩下的样本点中，重复步骤3，可以得到h个esd值
为每个esd值计算critical value: lambda_i (采用t分布计算)
统计每个esd是否大于lambda_i，大于的认为你是异常
"""

from adtk.detector import GeneralizedESDTestAD
esd_ad = GeneralizedESDTestAD(alpha=0.3)
anomalies = esd_ad.fit_detect(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683733218214-1f25cecc-09cd-431a-ac4f-4e2694e97ff9.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=uf26eb03d&originHeight=181&originWidth=671&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u59f79142-5384-44f8-a63a-a2294a28e42&title=)
<a name="fAy7c"></a>
### 突变
Spike and Level Shift 异常的表现形式不是离群点，而是通过**和邻近点的比较**，即突增或者突降。adtk提供`adtk.detector.PersistAD` 和 `adtk.detector.LevelShiftAD`检测方法
<a name="SEnym"></a>
#### PersistAD
```python
"""
adtk.detector.PersistAD(window=1, c=3.0, side='both', min_periods=None, agg='median')
参数：
window：参考窗长度，可为int, str
c：分位距倍数，用于确定上下限范围
side：检测范围，为'positive'时检测突增，为'negative'时检测突降，为'both'时突增突降都检测
min_periods：参考窗中最小个数，小于此个数将会报异常，默认为None，表示每个时间点都得有值
agg：参考窗中的统计量计算方式，因为当前值是与参考窗中产生的统计量作比较，所以得将参考窗中的数据计算成统计量，默认'median'，表示去参考窗的中位值

原理：
用滑动窗口遍历历史数据，将窗口后的一位数据与参考窗中的统计量做差，得到一个新的时间序列s1;
计算s1的(Q1-c*IQR, Q3+c*IQR) 作为正常范围；
若当前值与它参考窗中的统计量之差，不在2中的正常范围内，视为异常。

调参：
window：越大，模型越不敏感，不容易被突刺干扰
c：越大，对于波动大的数据，正常范围放大较大，对于波动较小的数据，正常范围放大较小
min_periods：对缺失值的容忍程度，越大，越不允许有太多的缺失值
agg：统计量的聚合方式，跟统计量的特性有关，如 'median'不容易受极端值影响
总结：先计算一条新的时间序列，再用箱线图作异常检测
"""

from adtk.detector import PersistAD
persist_ad = PersistAD(c=3.0, side='positive')
anomalies = persist_ad.fit_detect(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732474987-95d7b8cf-f6e3-40a6-ad28-19087ccf6e02.png#averageHue=%23fbfbfb&clientId=ubf02af9b-badd-4&from=paste&id=u00b81443&originHeight=183&originWidth=661&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u133b4328-81a9-4b44-a598-1509b053ff1&title=)
<a name="BCK0s"></a>
#### LevelShiftAD
```python
"""
adtk.detector.LevelShiftAD(window, c=6.0, side='both', min_periods=None)

参数：
window：支持(10,5)，表示使用两个相邻的滑动窗，左侧的窗中的中位值表示参考值，右侧窗中的中位值表示当前值
c：越大，对于波动大的数据，正常范围放大较大，对于波动较小的数据，正常范围放大较小，默认6.0
side：检测范围，为'positive'时检测突增，为'negative'时检测突降，为'both'时突增突降都检测
min_periods：参考窗中最小个数，小于此个数将会报异常，默认为None，表示每个时间点都得有值

原理：
该模型用于检测突变情况，相比于PersistAD，其抗抖动能力较强，不容易出现误报
"""

from adtk.detector import LevelShiftAD
level_shift_ad = LevelShiftAD(c=6.0, side='both', window=5)
anomalies = level_shift_ad.fit_detect(s)
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683733301598-3963cfeb-8f13-4ac4-9fde-e7b9c48c1fa4.png#averageHue=%23fcfbfb&clientId=ubf02af9b-badd-4&from=paste&id=u907ce018&originHeight=188&originWidth=675&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc0135d12-5859-4c43-88c7-7e589b3cc65&title=)
<a name="Zpvxh"></a>
### 季节性
<a name="CDKYH"></a>
#### `adtk.detector.SeasonalAD`
```python
"""
adtk.detector.SeasonalAD(freq=None, side='both', c=3.0, trend=False)
SeasonalAD主要是根据ClassicSeasonalDecomposition来处理，判断。
参数：
freq：季节性周期
c：越大，对于波动大的数据，正常范围放大较大，对于波动较小的数据，正常范围放大较小，默认6.0
side：检测范围，为'positive'时检测突增，为'negative'时检测突降，为'both'时突增突降都检测
trend：是否考虑趋势
"""

from adtk.detector import SeasonalAD
seasonal_ad = SeasonalAD(c=3.0, side="both")
anomalies = seasonal_ad.fit_detect(s)
plot(s, anomaly=anomalies, ts_markersize=1, anomaly_color='red', anomaly_tag="marker", anomaly_markersize=2);
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732475252-dba78439-8353-4724-a5c6-18c538b3a91d.png#averageHue=%23d9e5ee&clientId=ubf02af9b-badd-4&from=paste&id=uf48df623&originHeight=175&originWidth=672&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77c965ca-f955-4589-ad19-7cb1d5c5832&title=)
<a name="RTcWp"></a>
#### pipe 组合算法
```python
from adtk.pipe import Pipeline
steps = [
    ("deseasonal", ClassicSeasonalDecomposition()),
    ("quantile_ad", QuantileAD(high=0.995, low=0.005))
]
pipeline = Pipeline(steps)
anomalies = pipeline.fit_detect(s)
plot(s, anomaly=anomalies, ts_markersize=1, anomaly_markersize=2, anomaly_tag="marker", anomaly_color='red');
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683732475303-5eb2439a-b6d6-4ab1-896a-014aef708186.png#averageHue=%23d8e5ee&clientId=ubf02af9b-badd-4&from=paste&id=udb65e93a&originHeight=174&originWidth=657&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec364561-006d-4c97-9045-cafd8167908&title=)
<a name="uvMlf"></a>
## 6、总结
本文介绍了时间序列异常检测的无监督算法工具包ADTK。ADTK提供了简单的异常检测算法和时间序列特征加工函数。总结如下：

- adtk要求输入数据为**datetimeIndex**，`validate_series`来验证数据有效性，使得时间有序
- adtk单窗口和double窗口滑动，加工统计特征
- adtk分解时间序列的季节部分，获得时间序列的残差部分，可根据这个判断异常点
- adtk支持离群点、突变和季节性异常检测。通过`fit_detect` 获取异常点序列，也可以通过`Pipeline`联通多部异常检测算法
