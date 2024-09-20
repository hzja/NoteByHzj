时间序列 Python
<a name="pgW4q"></a>
## 背景
任何事物在两个不同时刻都不可能保持完全相同的状态，但很多变化往往存在着一定的规律，例如 24 小时日出日落，潮起潮落，这些现象通常称为「周期」。<br />周期性，指时间序列中呈现出来的围绕长期趋势的一种波浪形或振荡式变动。准确提取周期信息，不仅能反映当前数据的规律，应用于相关场景，还可以预测未来数据变化趋势。<br />![2021-07-31-08-27-14-359295.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627691244496-3ad96d9e-318a-486d-9906-55ff6624cc9f.png#clientId=u8de22334-ce2b-4&from=ui&id=ud332ef26&originHeight=252&originWidth=1080&originalType=binary&ratio=1&size=50498&status=done&style=none&taskId=u991c0b55-81af-4dfb-8b43-e7fafac7fff)<br />时间序列示例<br />一般而言，时间序列周期性分为三种：

- 「符号性周期」，例如序列 `fbcnfkgbfops`中 `f` 周期为 4；
- 「部分周期性」，例如序列 `ansdcdmncdcacdascdmc` 中 `cd` 周期为 4；
- 「分段周期性」，例如上面给定的时间序列即为分段周期性；

针对时间序列的周期性检测问题，主要介绍「傅里叶变换」和「自相关系数」两种方法及其在实际数据中的效果；
<a name="wZfXi"></a>
## 傅里叶变换
傅里叶变换是一种将时域、空域数据转化为频域数据的方法，任何波形（时域）都可以看做是不同振幅、不同相位正弦波的叠加（频域）👇🏻 此处放上经典图！<br />![2021-07-31-08-27-14-477296.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1627691252075-5c66a6fe-c613-4497-b7cc-7a843871a591.jpeg#clientId=u8de22334-ce2b-4&from=ui&id=u8c449758&originHeight=682&originWidth=1080&originalType=binary&ratio=1&size=53258&status=done&style=shadow&taskId=u007df904-2f4f-412a-a614-a2b2a841b4e)<br />傅里叶变换<br />对于一条具备周期性的时间序列，它本身就很接近正弦波，所以它包含一个显著的正弦波，周期就是该正弦波的周期，而这个正弦波可以通过傅里叶变换找到，它将时序数据展开成三角函数的线性组合，得到每个展开项的系数，就是傅里叶系数。傅里叶系数越大，表明它所对应的正弦波的周期就越有可能是这份数据的周期。
<a name="s983u"></a>
## 自相关系数
自相关系数（Autocorrelation Function）度量的是同一事件不同时间的相关程度，不同相位差（lag）序列间的自相关系数可以用 Pearson 相关系数计算。其数学表达如下：<br />其中  表示相位  的数据延迟 「lag operator」。<br />![2021-07-31-08-27-14-601309.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1627691261980-1d421fdb-8d89-4e23-b196-ca4eff5db069.jpeg#clientId=u8de22334-ce2b-4&from=ui&id=ud6af0cd4&originHeight=586&originWidth=1080&originalType=binary&ratio=1&size=35372&status=done&style=shadow&taskId=u2982962b-7e0c-4d4c-829c-ac910cbf5ae)<br />自相关系数<br />当序列存在周期性时，遍历足够多的相位差，一定可以找到至少一个足够大的自相关系数，而它对应的相位差就是周期。所以对于检测时序周期来说，只需找到两个自相关系数达到一定阈值的子序列，它们起始时间的差值就是需要的周期。
<a name="HI7H3"></a>
## 实例说明
为了保证结果的可靠性，可以将傅里叶分析和自相关系数结合起来判断周期性。主要思路是：先通过傅里叶变换找到可能的周期，再用自相关系数做排除，从而得到最可能的周期。<br />给定一份周期性数据，时间间隔为 5 min。从这份数据中可以看出数据大体上具有周期为 1 day。<br />![2021-07-31-08-27-14-861759.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627691271067-6b4087bb-9636-444b-97f0-26e690eb4580.png#clientId=u8de22334-ce2b-4&from=ui&id=u9628b987&originHeight=503&originWidth=1080&originalType=binary&ratio=1&size=92094&status=done&style=none&taskId=u62b6aa74-222d-4d69-8000-17ab3443bce)<br />示例数据<br />下面使用傅里叶变换估计周期，代码如下所示
```python
from scipy.fftpack import fft, fftfreq

fft_series = fft(data["value"].values)
power = np.abs(fft_series)
sample_freq = fftfreq(fft_series.size)

pos_mask = np.where(sample_freq > 0)
freqs = sample_freq[pos_mask]
powers = power[pos_mask]

top_k_seasons = 3
# top K=3 index
top_k_idxs = np.argpartition(powers, -top_k_seasons)[-top_k_seasons:]
top_k_power = powers[top_k_idxs]
fft_periods = (1 / freqs[top_k_idxs]).astype(int)

print(f"top_k_power: {top_k_power}")
print(f"fft_periods: {fft_periods}")
```
取 top-3 振幅值为`top_k_power: [ 614.8105282 890.33273899 1831.167168 ]` 及其对应的周期 `fft_periods: [ 72 278 292]` 。📢 数据间隔为 5 min 所以真实周期应为 288，从傅里叶变换即可看出估计值 292 已经非常接近真实值。<br />现在来计算自相关系数，代码如下所示：
```python
from statsmodels.tsa.stattools import acf

# Expected time period
for lag in fft_periods:
    # lag = fft_periods[np.abs(fft_periods - time_lag).argmin()]
    acf_score = acf(data["value"].values, nlags=lag)[-1]
    print(f"lag: {lag} fft acf: {acf_score}")


expected_lags = np.array([timedelta(hours=12)/timedelta(minutes=5), timedelta(days=1)/timedelta(minutes=5), timedelta(days=7)/timedelta(minutes=5)]).astype(int)
for lag in expected_lags:
    acf_score = acf(data["value"].values, nlags=lag, fft=False)[-1]
    print(f"lag: {lag} expected acf: {acf_score}")
```
对应的输出如下：
```python
lag: 72 fft acf: 0.07405431832776994
lag: 278 fft acf: 0.7834457453491087
lag: 292 fft acf: 0.8259822269757922
lag: 144 expected acf: -0.5942986094704665
lag: 288 expected acf: 0.8410792774898174
lag: 2016 expected acf: 0.5936030431473589
```
通过自相关系数来得到显著分数最大值对应的周期，得出的结果为 292；<br />此处实验补充了预设的三个周期值：12 hour、1 day、7 day，发现算出来还是周期 288 对应的相关分数最大，但是傅里叶变换没有估计出周期值。<br />综上：算出来的还不如预设的值呢！直接根据先验知识「预设周期」然后计算自相关系数就行了！
