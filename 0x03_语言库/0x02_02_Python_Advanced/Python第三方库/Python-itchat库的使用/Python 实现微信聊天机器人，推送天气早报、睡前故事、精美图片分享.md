Python itchat
<a name="UbI1C"></a>
### 1、Python 登录微信
Python 中有个 itchat 包，这是个开源的微信个人号接口，非常简单就可以实现在 python 对微信的操作。<br />基本的登录、对话代码
```python
import itchat
itchat.auto_login(hotReload=True)  # 登录，会下载二维码给手机扫描登录，hotReload设置为True表示以后自动登录
itchat.send('hello my love', toUserName='filehelper') #发送信息给微信文件助手
friends = itchat.search_friends(name='好友昵称')  # 获取微信好友列表
userName = friends[0]['UserName']
itchat.send('hello my love', toUserName=userName)  # 发送信息给指定好友
itchat.run()  # 让itchat一直运行
```
详细了解 itchat 的应用可参考：[https://itchat.readthedocs.io/zh/latest/](https://itchat.readthedocs.io/zh/latest/)
<a name="KeF2r"></a>
### 2、获取天气早报信息
获取每日天气信息：<br />天气信息可以在阿凡达数据（[https://www.avatardata.cn/](https://www.avatardata.cn/)）中申请的免费数据接口，里面也有很多好玩有趣的数据，最好自己去注册个账号。　
```python
common = Common() #这是个自己封装的工具类
key = 'cc186c9881b94b42b886a6d634c632' #这个我修改了 嘻嘻
# 数据提供类
class DataUtil():
    # 获取天气信息
    def getWeatherData(self, cityname):
        # 阿凡达数据
        url = ' http://api.avatardata.cn/Weather/Query?key=' + key + '&cityname=' + cityname
        results = common.get(url)
        text = self.parseInfo_afd(results)
        print(text)
        return text
    # 简单的数据修饰封装
    def parseInfo_afd(self, jsons):
        # 将string 转换为字典对象
        jsonData = json.loads(jsons)
        textInfo = '早上好，今天又是元气满满的一天哟.\n'
        data = jsonData['result']['weather'][0]['date']
        week = jsonData['result']['weather'][0]['week']
        nongli = jsonData['result']['weather'][0]['nongli']
        city_name = jsonData['result']['realtime']['city_name']
        lowTemperature = jsonData['result']['weather'][0]['info']['dawn'][2]
        highTemperature = jsonData['result']['weather'][0]['info']['day'][2]
        weather = jsonData['result']['weather'][0]['info']['day'][1]
        wind = jsonData['result']['weather'][0]['info']['day'][4]
        textInfo = textInfo + '今天是' + data + '号\n'
        textInfo = textInfo + '农历:' + nongli + ',星期' + week + '\n'
        textInfo = textInfo + city_name + '气温：' + lowTemperature + '-' + highTemperature + '度，' + weather + ' ' + wind + '\n\n'
        textInfo = textInfo + '穿衣指数：' + jsonData['result']['life']['info']['chuanyi'][0] + ' - ' + jsonData['result']['life']['info']['chuanyi'][1] + '\n\n'
        textInfo = textInfo + '运动指数：' + jsonData['result']['life']['info']['yundong'][0] + ' - ' + jsonData['result']['life']['info']['yundong'][1] + '\n\n'
        textInfo = textInfo + '感冒指数：' + jsonData['result']['life']['info']['ganmao'][0] + ' - ' + jsonData['result']['life']['info']['ganmao'][1]  + '\n\n'
        textInfo = textInfo + '紫外线指数：' + jsonData['result']['life']['info']['ziwaixian'][0] + ' - ' + jsonData['result']['life']['info']['ziwaixian'][1]  + '\n\n'
        textInfo = textInfo + 'by：小可爱的贴心秘书' + '\n\n'
        return textInfo
```
<a name="kqpao"></a>
### 3、获取睡前故事
睡前故事的来源是出自《从你的全世界路过--张嘉佳》，这本书中都是些爱情相关的小故事。<br />用 night.n 来区分每晚发送的故事。
```python
# 提取故事的第一天
readBookStartDay = datetime.datetime(2019, 2, 17)
class DataUtil():
    def getBookInfo(self, filePath): #文件路径，
        radioList = [] #微信每次最多只能发送的字符是有限制的，我每25行发送一次信息
        row = 0
        tempInfo = textInfo = '睡前故事：张嘉佳 - 《从你的全世界路过》.\n\n'
        readFlag = False #是否读取
        today = datetime.datetime.now()
        dayCount = (today - readBookStartDay).days + 1
        for line in open(filePath):
            if (line.find('night.' + str(dayCount)) > -1): # 开始读数据
                readFlag = True
                continue
            if (line.find('night.' + str(dayCount+1)) > -1): # 读完一天数据结束
                break
            if readFlag:
                row += 1
                tempInfo += line
                # 微信每次最多只能发送的字符是有限制的，我每25行发送一次信息
                if row == 25:
                    radioList.append(tempInfo)
                    tempInfo = ''
                    row = 0
        tempInfo += '\n晚安\n' + 'by：小可爱的贴心秘书' + '\n'
        radioList.append(tempInfo)
        # common.txtToMp3(radioList) #文字生成语音 发送语音
        print(radioList)
        return radioList
```
<a name="oVSPy"></a>
### 4、获取精美壁纸
从必应（[https://cn.bing.com/?scope=web&FORM=QBLHCN](https://cn.bing.com/?scope=web&FORM=QBLHCN)）的官网抓取壁纸，必应的官网壁纸还是比较精美的。壁纸抓取分析看代码。
```python
class DataUtil():
    def getBingPhoto(self, index):
        # index 对应的是 必应 index天的壁纸
        url = ' http://www.bing.com/HPImageArchive.aspx?format=js&idx=' + index + '&n=1&nc=1469612460690&pid=hp&video=1'
        html = urllib.request.urlopen(url).read().decode('utf-8')
        photoData = json.loads(html)
        # 这是壁纸的 url
        photoUrl = 'https://cn.bing.com' + photoData['images'][0]['url']
        photoReason = photoData['images'][0]['copyright']
        photoReason = photoReason.split(' ')[0]
        photo = urllib.request.urlopen(photoUrl).read()
        # 下载壁纸刀本地
        with open('./bing.jpg', 'wb') as f:
            # img = open_url(photoUrl)
            if photo:
                f.write(photo)
        print("图片已保存")
        # 把壁纸的介绍写到壁纸上
        # 设置所使用的字体
        font = ImageFont.truetype("simhei.ttf",35)
        imageFile = "./bing.jpg"
        im1 = Image.open(imageFile)
        # 画图，把壁纸的介绍写到壁纸上
        draw = ImageDraw.Draw(im1)
        draw.text((im1.size[0]/2.5, im1.size[1]-50), photoReason, (255, 255, 255), font=font)  # 设置文字位置/内容/颜色/字体
        draw = ImageDraw.Draw(im1)  # Just draw it!
        # 另存图片
        im1.save("./bing.jpg")
```
<a name="1a3SG"></a>
### 5、整合数据，配置定时任务
数据都获取到了，也登录了微信，现在就需要把这些信息发送给需要的人了。<br />这里需要注意的是：

- 微信每条信息是有长度限制的<br />
- 微信在线需要单独占用一个线程，所以发送信息需要在另一个线程执行。<br />
```python
wechat = WeChat() #这里是封装的 itchat
# 开启微信登录线程，需要单独占个线程
_thread.start_new_thread(wechat.login, ( ))
# 配置定时任务
# 开启早间天气预报 定时任务
schedule.every().day.at("7:20").do(wechat.dailyInfo)
# 开启睡前故事 定时任务
schedule.every().day.at("21:30").do(wechat.readStory)
while True:
    schedule.run_pending()
    time.sleep(1)
```
微信登录方法
```python
class WeChat():
    def login(self):
        itchat.auto_login(hotReload=True)  # 登录，会下载二维码给手机扫描登录，hotReload设置为True表示以后自动登录
        itchat.send('hello my love', toUserName='filehelper') #发送信息给微信文件助手
        friends = itchat.search_friends(name='好友昵称')  # 获取微信好友列表
        userName = friends[0]['UserName']
        itchat.send('hello my love', toUserName=userName)  # 发送信息给指定好友
        itchat.run()  # 让itchat一直运行
```
微信每日天气预报方法
```python
class WeChat():
    # 推送每日早报
    def dailyInfo(self):
        print('dailyInfo do')
        jiujiang = dataUtil.getWeatherData('九江')
        # wechat.sendMessage(jiujiang, 'filehelper')
        yfei = wechat.getFriend('好友昵称')
        wechat.sendMessage(jiujiang, yfei)
```
微信发送睡前故事 和 精美壁纸方法
```python
# 推送睡前故事
def readStory(self):
    print('readStory do')
    stroy = dataUtil.getBookInfo('./从你的全世界路过.txt')
    dataUtil.getBingPhoto('0')
    # wechat.sendMessage(stroy, 'filehelper')
    # itchat.send_image('./bing.jpg',  'filehelper')
    yfei = wechat.getFriend('好友昵称')
    for txt in stroy:
        wechat.sendMessage(txt, yfei)
        # 发送壁纸
        itchat.send_image('./bing.jpg', toUserName=yfei)
```
<a name="zmSoO"></a>
### 6、自动聊天机器人（图灵机器人）
聊天机器人要考虑两个问题：

- 怎么微信自动回复信息？（itchat 提供的注解）<br />
- 自动回复什么内容？（调用图灵机器人）<br />

第一个问题，Itchat 提供了注解，可以自动回复信息，详细可参考：[https://itchat.readthedocs.io/zh/latest/](https://itchat.readthedocs.io/zh/latest/)<br />需要注意的是，自动回复方法，不能包含在类中，不然无法生效。
```python
# 聊天助手开关
OPEN_FLAG = 0
# 回复信息
@itchat.msg_register(['Text'])
def text_reply(msg):
    global OPEN_FLAG
    msgText = msg['Text']
    # print(msgText)
    if msgText == "皮皮过来":
        OPEN_FLAG = 1
        print('开启皮皮语音助手*')
        return '开启皮皮语音助手*'
    if msgText == "皮皮退下":
        OPEN_FLAG = 0
        print('关闭皮皮语音助手*')
        return '关闭皮皮语音助手*'
    if OPEN_FLAG == 1:
        # 为了保证在图灵Key出现问题的时候仍旧可以回复，这里设置一个默认回复
        defaultReply = '不想说话了！' + "*"
        # 如果图灵Key出现问题，那么reply将会是None
        reply = get_response(msg['Text']) + "*"
        # 有内容一般就是指非空或者非None，你可以用`if a: print('True')`来测试
        return reply or defaultReply
```
第二个问题，图灵机器人去官网（[http://www.turingapi.com/](http://www.turingapi.com/)）申请注册就可以了。<br />申请到了之后，只需要调用接口就好了。
```python
KEY = '71f9d9d2dd364ad8b28bd565270176'
# 图灵机器人
def get_response(msg):
    # 构造了要发送给服务器的数据
    apiUrl = 'http://www.tuling123.com/openapi/api'
    data = {
        'key': KEY,
        'info': msg,
        'userid': 'wechat-robot',
    }
    try:
        r = requests.post(apiUrl, data=data).json()
        return r.get('text')
    # 为了防止服务器没有正常响应导致程序异常退出，这里用try-except捕获了异常
    # 如果服务器没能正常交互（返回非json或无法连接），那么就会进入下面的return
    except Exception as e:
        print('插入时发生异常' + e)
        # 将会返回一个None
        return
```
