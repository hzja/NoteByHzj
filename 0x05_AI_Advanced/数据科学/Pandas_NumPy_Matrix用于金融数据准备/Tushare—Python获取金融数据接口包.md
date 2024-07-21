Python<br />Tushare是一个免费、开源的python财经数据接口包。<br />主要实现对股票等金融数据从数据采集、清洗加工到数据存储的过程，数据多样易获取，在数据分析，机器学习，股票预测等领域都可以被广泛的应用。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624353596915-2e4209aa-6a6d-421b-b6c8-687eda92a021.png#clientId=u23a2f9f3-8fbf-4&from=paste&height=600&id=ua130f732&originHeight=1800&originWidth=3798&originalType=binary&ratio=3&size=8341071&status=done&style=shadow&taskId=u3b040660-8583-4824-b08b-a586dfebb97&width=1266)<br />从tushare获取到的数据为dataframe格式，无需重新对数据进行清洗，可以直接将数据存储至csv或者数据库中。<br />目前tushare有免费版本和升级的需要付费的Pro版本，pro版本提供的数据更多，免费的版本就够用了，而且每次使用都收到提示，“本接口即将停止更新，请尽快使用Pro版接口：[https://tushare.pro/document/2](https://tushare.pro/document/2)”。不知道何时会停止更新。<br />首先先安装tushare，直接输入：`pip install tushare`，几秒钟就安装好了<br />下面分别介绍一下两种版本的使用方法
<a name="tZU3h"></a>
### 免费版
获取股票指定日期的交易数据:
```python
import tushare as ts
ts.get_k_data('000001', start='2020-05-07', end='2021-05-11') 
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624353144268-2642eba0-4172-4c30-b593-79b696089ef3.webp#clientId=u23a2f9f3-8fbf-4&from=paste&id=udddeae80&originHeight=436&originWidth=623&originalType=url&ratio=3&status=done&style=shadow&taskId=u1a61f53a-cfed-4ccb-a450-5413fb65be0)<br />获取某支股票历史数据
```python
ts.get_hist_data('600848')
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624353144213-7244465a-2783-4af8-b754-2af3b0c02514.webp#clientId=u23a2f9f3-8fbf-4&from=paste&id=u83639d7d&originHeight=464&originWidth=992&originalType=url&ratio=3&status=done&style=shadow&taskId=u65531d02-27a3-432b-a887-3dabc388cc5)<br />输入参数为：
```python
code：股票代码，即6位数字代码，或者指数代码（sh=上证指数 sz=深圳成指 hs300=沪深300指数 sz50=上证50 zxb=中小板 cyb=创业板）
start：开始日期，格式YYYY-MM-DD
end：结束日期，格式YYYY-MM-DD
ktype：数据类型，D=日k线 W=周 M=月 5=5分钟 15=15分钟 30=30分钟 60=60分钟，默认为D
retry_count：当网络异常后重试次数，默认为3
pause:重试时停顿秒数，默认为0
```
返回值说明：
```python
返回值说明：
date：日期
open：开盘价
high：最高价
close：收盘价
low：最低价
volume：成交量
price_change：价格变动
p_change：涨跌幅
ma5：5日均价
ma10：10日均价
ma20:20日均价
v_ma5:5日均量
v_ma10:10日均量
v_ma20:20日均量
turnover:换手率[注：指数无此项]
```
不过遗憾的是，某些数据接口已经关闭了，例如获取当前交易所有股票的行情数据
```python
ts.get_today_all()
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624353144230-7402eb9e-2c47-4bad-afb5-525a68880cbb.webp#clientId=u23a2f9f3-8fbf-4&from=paste&id=u0a366533&originHeight=72&originWidth=684&originalType=url&ratio=3&status=done&style=shadow&taskId=u217f1c12-80aa-4731-b0fe-e973735b83e)<br />遇到这种情况不得不用付费版的了
<a name="JjbWQ"></a>
### Pro版本
Pro版本数据更全面，但是不同的数据接口，对应了不同的积分，基本上注册后即有100积分，再去完善你的资料，会获得20积分，120积分可解锁一些基础数据接口。<br />官方文档链接：[https://tushare.pro/document/2](https://tushare.pro/document/2)<br />反正目前能够使用的接口只有daily,new_share,share_float<br />首先需要先注册用户，获取token，注册地址为：[https://tushare.pro/register?reg=451783](https://tushare.pro/register?reg=451783)<br />注册成功后，依次点击个人主页-接口TOKEN，即可获取token值<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624353529381-faedccd1-27f2-4a28-bc43-c04d3828a39d.png#clientId=u23a2f9f3-8fbf-4&from=paste&height=559&id=ub03b6ddd&originHeight=1677&originWidth=3798&originalType=binary&ratio=3&size=198711&status=done&style=shadow&taskId=u43f9a592-0a45-4733-94e1-f0cd967b806&width=1266)<br />设置token值并初始化接口
```python
import tushare as ts
ts.set_token('f4126fb90d1d945a342f035c0c46520f39587c507b8c675a07cc1bad')
api = ts.pro_api()#初始化接口
```
调用日线行情：
```python
api.daily(ts_code='000001.SZ', start_date='20201101', end_date='20210722')
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624353144812-b8d30a96-e123-43f7-ae41-8ed8033ae6fe.webp#clientId=u23a2f9f3-8fbf-4&from=paste&id=uec95b21a&originHeight=398&originWidth=807&originalType=url&ratio=3&status=done&style=shadow&taskId=u20271a62-412b-46c7-9a27-aff788d92d8)<br />调用股票基础信息：
```python
api.stock_basic()
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624353144998-22942061-e4c9-47ea-8ab4-dc8848ab47f1.webp#clientId=u23a2f9f3-8fbf-4&from=paste&id=uc69d4146&originHeight=254&originWidth=882&originalType=url&ratio=3&status=done&style=shadow&taskId=uc677dd31-9c83-4cd4-a274-df3a70d7931)<br />因为积分不够，大部分接口访问不了，网站提供了多种方式增加积分：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624353697061-d6f97596-325c-4e42-adf4-8b1347c94a2f.png#clientId=u23a2f9f3-8fbf-4&from=paste&height=601&id=u7aa0d211&originHeight=1802&originWidth=2195&originalType=binary&ratio=3&size=670172&status=done&style=shadow&taskId=ue73ebc62-f2bd-4f3e-8e97-d65b173f5ca&width=731.6666666666666)
