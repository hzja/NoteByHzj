Python<br />农历，是我国现行的传统历法。它是根据月相的变化周期，每一次月相朔望变化为一个月，参考太阳回归年为一年的长度，并加入二十四节气与设置闰月以使平均历年与回归年相适应。<br />对于处理数据来说，并不需要去详细研究农历与公历之间的转换关系。在Python中，ZhDate库支持农历-公历互相转换、日期加减以及全中文日期生成，内置了1900-2100年的农历数据，仅依赖Python内置模块。[_github.com/CutePandaSh/zhdate_](https://github.com/CutePandaSh/zhdate)<br />由于ZhDate属于Python的第三方开源模块，因此需要额外安装、导入后，才能使用。直接在命令行窗口中输入如下命令：
```bash
pip install zhdate
```
<a name="SKbuX"></a>
## 详细使用方法
zhdate库，不用网络接口直接本地计算中国农历，支持农历阳历互转。
<a name="bB45m"></a>
### 阳历与农历日期的相互转换
在交互式环境中输入如下命令：
```python
from zhdate import ZhDate

date1 = ZhDate(2022, 2, 2)  # 新建农历 2022年二月初二（龙抬头）的日期对象
print(date1)  # 直接返回农历日期字符串d
```
输出：
```python
农历2022年2月2日
```
在交互式环境中输入如下命令：
```python
t_date1 = date1.to_datetime()
t_date1
```
输出：
```python
datetime.datetime(2022, 3, 4, 0, 0)
```
在交互式环境中输入如下命令：
```python
from datetime import datetime

dt_date2 = datetime(2022, 2, 6)
date2 = ZhDate.from_datetime(dt_date2)  # 从阳历日期转换成农历日期对象
date2
```
输出：
```python
农历2022年1月6日
```
<a name="ftZUC"></a>
### 闰月
在交互式环境中输入如下命令：
```python
date3 = ZhDate(2020, 4, 15)  # 新建农历 2020年4月15日
print(date3.to_datetime())
```
输出：
```python
2020-05-07 00:00:00
```
在交互式环境中输入如下命令：
```python
date3 = ZhDate(2020, 4, 15)  # 新建农历 2020年闰4月15日
print(date3.to_datetime())
```
输出：
```python
2020-06-06 00:00:00
```
这一年农历4月15日的同学，可以分别在5月7日和6月6日分别过两次农历生日。
<a name="eF30D"></a>
### 其他
**中文输出**，在交互式环境中输入如下命令：
```python
dt_date4 = datetime(2022, 3, 4)
date4 = ZhDate.from_datetime(dt_date4)
print(date4.chinese())
```
输出：
```python
二零二二年二月初二 壬寅年 (虎年)
```
**当天的农历日期**，在交互式环境中输入如下命令：
```python
ZhDate.today()
```
输出：
```python
农历2022年2月7日
```
此外，zhdate模块也支持加减法。两个zhdate对象相减可以得到两个农历日期的差额，同时加整数也可以返回相隔天数以后的新农历对象。
<a name="pnhdZ"></a>
## 实战：计算节日距离天数
下面通过一个实战项目练习一下这个zhdate模块。<br />快速地了解今天距离某个节日还有多少天？可以使用zhdate和datetime库搭配解决这个问题。<br />在交互式环境中输入如下命令：
```python
today = datetime.now()
data_str = today.strftime('%Y-%m-%d')
data_str
```
输出：
```python
'2022-03-09'
```
**计算距离劳动节的天数**，在交互式环境中输入如下命令：
```python
oneDay = datetime(2022, 5, 1)
difference = oneDay.toordinal()-today.toordinal()
print(f'今天 {data_str} 距离五一劳动节还有 {difference} 天')
```
输出：
```python
今天 2022-03-09 距离五一劳动节还有 53 天
```
**计算距离中秋节的天数**，在交互式环境中输入如下命令：
```python
oneDay = ZhDate(2022, 8, 15).to_datetime()
difference = oneDay.toordinal()-today.toordinal()
print(f'今天 {data_str} 距离中秋节还有 {difference} 天')
```
输出：
```python
今天 2022-03-09 距离中秋节还有 185 天
```
也可以动手试一试，测试一下还距离其他节日多少天？实战时要注意该节日是按照公历统计还是农历设置的。
