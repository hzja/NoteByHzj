AI<br />使用tsod库完成简单的异常检测。<br />![2021-05-05-10-20-48-877544.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181308043-d4917cb5-7129-4de6-bced-28d7a6acc418.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=ua31871df&originHeight=288&originWidth=432&originalType=binary&size=29365&status=done&style=shadow&taskId=u272b5fa8-5b7e-475b-9d01-851c73358f5)
<a name="xeC3m"></a>
## tsod介绍
tsod可以完成时序数据的异常检测，是一个比较新的库，但使用起来非常方便。<br />[https://github.com/DHI/tsod](https://github.com/DHI/tsod)<br />[https://github.com/DHI/tsod/blob/main/notebooks/Getting%20started.ipynb](https://github.com/DHI/tsod/blob/main/notebooks/Getting%20started.ipynb)
<a name="o5ODQ"></a>
## 区间异常检测
如果能提前确定好指标的范围，则可以依次进行判定异常。
```python
# 最小值与最大值
rd = tsod.RangeDetector(min_value=0.01, max_value=2.0)

res = rd.detect(series)
series[res]
```
将识别结果进行展示：<br />![2021-05-05-10-20-49-074114.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181378947-bda59d03-028c-461c-94b8-05404b8e5ff9.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=uee0a037a&originHeight=662&originWidth=1008&originalType=binary&size=53486&status=done&style=shadow&taskId=u48c0c48f-f9e3-4fbc-a749-0413d77eb2b)
<a name="fEEWo"></a>
## 常数波动检测
```python
cd = tsod.ConstantValueDetector()

res = cd.detect(series)
series[res]
```
将识别结果进行展示：<br />![2021-05-05-10-20-49-267056.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181391041-993ddfce-7e46-470e-8774-1f5ac162fb5f.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=ube964e75&originHeight=686&originWidth=1038&originalType=binary&size=53496&status=done&style=shadow&taskId=u68905dab-b4d9-4447-b220-1c843e79430)
<a name="YDDvf"></a>
## 范围+常数组合检测
```python
combined = tsod.CombinedDetector([tsod.RangeDetector(max_value=2.0),
                                     tsod.ConstantValueDetector()])

res = combined.detect(series)
series[res]
```

将识别结果进行展示：<br />![2021-05-05-10-20-49-562474.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181403594-5c7d2c0f-83bb-4d1c-89a2-9994e7be4a82.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=u61758206&originHeight=692&originWidth=1038&originalType=binary&size=54114&status=done&style=shadow&taskId=u27a0fa4d-6aef-4b11-b54c-e298fc97214)
<a name="y81L5"></a>
## 梯度固定检测
```python
cgd = tsod.ConstantGradientDetector()
res = cgd.detect(series)
```
将识别结果进行展示：<br />![2021-05-05-10-20-49-780344.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181416579-ac9e2b30-a355-4a3c-802f-ee0168b0cb63.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=u57f65b57&originHeight=458&originWidth=1080&originalType=binary&size=78309&status=done&style=none&taskId=ua05f01e0-1657-4bcf-82ca-3599d70e580)
<a name="aplrS"></a>
## 滚动聚合加方差检测
```python
rsd = tsod.RollingStandardDeviationDetector(window_size=10, center=True)

rsd.fit(normal_data)
```
将识别结果进行展示：<br />![2021-05-05-10-20-50-001731.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181425809-add8f1a2-c904-4502-a066-4fc25e36ecbb.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=u531122b0&originHeight=555&originWidth=1080&originalType=binary&size=91434&status=done&style=shadow&taskId=u24ff1675-e57d-442f-8a66-a6fc5192eb3)
<a name="volME"></a>
## 一阶差分检测
```python
drd = tsod.DiffDetector()
drd.fit(normal_data)
```
将识别结果进行展示：<br />![2021-05-05-10-20-50-210331.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620181436273-8b1daab0-e958-4748-8574-230be4eed2ac.png#clientId=ue89bd6f4-8ee6-4&from=ui&id=u8501eb79&originHeight=644&originWidth=954&originalType=binary&size=38814&status=done&style=shadow&taskId=uf0a59951-fce6-41b6-bd1d-81850ffddf4)
