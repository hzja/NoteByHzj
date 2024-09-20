Python
<a name="Wv9Hs"></a>
## 一、前言
很多时候有这种需求，因为程序运行比较耗时，但是没有足够的时间等待。这个时候就可以用 Python 做一个远程控制电脑的小工具，实现远程控制电脑。当然，能做的操作十分有限，可以通过邮件和电脑进行通讯、然后让电脑以截图的方式为显示程序当前运行的状态。然后通过这个状态进行判断，用邮件发送一些预设好的指令。如果程序意外中断，可以进行让它重新运行，如果程序按照预期执行了，就可以远程关机。

## 二、使用 yagmail 进行邮件发送
使用 yagmail 发邮件非常简单，首先需要安装两个模块：
```bash
pip install yagmail
pip install keyring
```

### 2.1 发送一个简单的邮件
用 yagmail 来发一封简单的邮件测试一下：
```python
import yagmail
import keyring

sender = '发件邮箱'
password = '发件人密码'
receiver = '收件邮箱'

# 注册
yagmail.register(sender, password)

# 用 SMTP 服务器发邮件
yag = yagmail.SMTP(user=sender, host='smtp.163.com')

# 设置邮件的内容
contents = ['Do not go gentle into that good night!']

# 发送邮件
yag.send(receiver, '邮件主题', contents=contents)
```
这里密码可以使用真实密码，也可以使用授权码。授权码的获得需要到各个邮件的网页获取，按照指引获取授权码即可。<br />在连接邮箱服务器的时候，设置的一个 host，这个同样需要在邮箱网页中获取。其中 163 的邮箱地址为 smtp.163.com，QQ 邮箱服务器地址为 smtp.qq.com，其它邮箱的可以自己查找一下。<br />邮箱内容是一个列表，现在是放入了一句话。<br />在调用 send 方法发送邮件时，需要传入几个参数。第一个是收件人，第二个是邮件的主题，第三个就是邮件的内容了。邮件内容传入开始的内容列表。可以使用自己的邮箱测试一下。

### 2.2 发送附件
yagmail 非常智能，不需要像原始的 email 模块一样，先将附件上传到邮箱服务器，然后再发给收件人。yagmail 会自动识别内容中的资源文件，然后自动上传：
```python
import yagmail
import keyring

sender = '发件邮箱'
password = '发件人密码'
receiver = '收件邮箱'

# 注册
yagmail.register(sender, password)

# 用 SMTP 服务器发邮件
yag = yagmail.SMTP(user=sender, host='smtp.163.com')

# 设置邮件的内容
contents = ['im.jpg']

# 发送邮件
yag.send(receiver, '邮件主题', contents=contents)
```
其中 im.jpg 是本地的一张图片，在发送的时候 yagmail 会自动识别，然后发送出去。就完成了附件的发送。<br />也可以直接发送带有图片的邮件，而不是以附件的形式发送：
```python
import yagmail
import keyring

sender = '发件邮箱'
password = '发件人密码'
receiver = '收件邮箱'

# 注册
yagmail.register(sender, password)

yag = yagmail.SMTP(user=sender, host='smtp.163.com')
contents = [
    '这是一张图片',
    yagmail.inline('im.jpg')
]
yag.send(receiver, '邮件主题', contents=contents)
```
只要用 yagmail.inline 包含图片路径即可。不过需要注意，发送的内容不能单是一张图片。
<a name="wCvVA"></a>
## 三、邮件读取
邮件的读取需要使用到 imbox 模块。需要先使用 keyring 模块读取密码，keyring 可以读取到在 yagmail 中注册了的邮箱密码：
```python
import yagmail
import keyring
from imbox import Imbox

sender = '你的邮箱'
# 获取邮箱密码
password = keyring.get_password(sender)
# 读取邮箱邮件
with Imbox('imap.163.com', sender, password, ssl=True) as imbox:
    # 读取所有未读邮件
    all_msg = imbox.messages(unread=True)
```
Imbox 对象就是邮箱对象，里面包含了所有邮箱信息。调用 messages 方法就可以获取所有邮件，如果将 unread 设置为 True，则表示读取未读邮件。获取了所有邮件，就可以查看更多信息了：
```python
import yagmail
import keyring
from imbox import Imbox

sender = '你的邮箱'
# 获取邮箱密码
password = keyring.get_password(sender)
# 读取邮箱邮件
with Imbox('imap.163.com', sender, password, ssl=True) as imbox:
    # 读取所有未读邮件
    all_msg = imbox.messages(unread=True)
    # 遍历邮件
    for uid, message in all_msg:
        # 输出邮件的主题
        print(message.subject)
        # 标记已读
        imbox.mark_seen(uid)
        # 获取邮件的文本内容
        msg = message.body['plain']
```
要实现控制电脑就需要一直读取，可以使用 schedule 模块设置定时任务：
```python
import time
import schedule

def func():
    print('test')

schedule.every(2).seconds.do(func)
while True:
    schedule.run_pending()
    time.sleep(1)
```
这样的话，可以每两秒执行一次 func 函数。
<a name="gIDeG"></a>
## 四、开发电脑控制工具
定义一个 ComputerController 类，用于控制电脑。该类的最基本方法就发邮件和收邮件：
```python
import time
import yagmail
import keyring
import schedule
from imbox import Imbox


class ComputerController():

    TIME_SPACE = 10

    SMTP_163 = 'smtp.163.com'

    SMTP_QQ = 'smtp.qq.com'

    def __init__(self, user, receiver):
        self.user = user
        self.receiver = receiver
        # 注册
        yagmail.register(self.user, '你的密码')

    def send_main(self, msg):
        """发邮件"""
        yag = yagmail.SMTP(user=self.user, host=self.SMTP_163)
        contents = [
            msg
        ]
        yag.send(self.receiver, '来自电脑的邮件', contents=contents)


    def receive_mail(self):
        """收邮件"""

        # 读取密码
        password = keyring.get_password('yagmail', self.user)
        with Imbox('imap.163.com', 'sockwz@163.com', password, ssl=True) as imbox:
            all_msg = imbox.messages(unread=True)
            for uid, message in all_msg:
                if '110' == message.subject:
                    # 标记已读
                    imbox.mark_seen(uid)
                    msg = message.body['plain']
                    print(msg)

    def run_controller(self):
          # 定时读取邮件
        schedule.every(self.TIME_SPACE).seconds.do(controller.receive_mail)
        while True:
            schedule.run_pending()
            time.sleep(self.TIME_SPACE)
```
只需要创建这个类就可以了：
```python
if __name__ == '__main__':
    controller = ComputerController('发件邮箱', '收件邮箱')
    controller.run_controller()
```
需要传入收件邮箱和发件邮箱，另外还需要在 init 方法中，修改密码即可。
<a name="7tuN0"></a>
## 五、实现一些预设功能
下面实现一些小功能。
<a name="vpcbH"></a>
### 5.1 截图
```python
from PIL import ImageGrab
def grab_windows(self):
    im = ImageGrab.grab()
    im.save('status.jpg')
```
使用 Pillow 模块需要先安装：
```bash
pip install pillow
```
<a name="M53F6"></a>
### 5.2 关机
```python
import os
def shut_down(self):
    """关机"""
    os.system('shutdown /s /t 0')
```
这个是通用的两个功能。另外可以使用 pynput 模块控制键盘，进行一些操作。这个需要根据自己电脑的情况操作，比如使用 PyCharm 运行程序可以使用下列代码运行程序：
```python
def run_program(self):
    """运行程序"""
    with self.kb.pressed(Key.shift):
        self.kb.press(Key.f10)
        self.kb.release(Key.f10)
```
可以自己定制一些操作。整个程序完整代码如下：
```python

import os
import time
import yagmail
import keyring
import schedule
from imbox import Imbox
from PIL import ImageGrab
from pynput.keyboard import *

class ComputerController():

    TIME_SPACE = 10

    SMTP_163 = 'smtp.163.com'

    SMTP_QQ = 'smtp.qq.com'

    kb = Controller()

    def __init__(self, user, receiver):
        self.user = user
        self.receiver = receiver
        # 注册
        yagmail.register(self.user, '授权码或密码')

    def send_main(self, msg):
        """发邮件"""
        yag = yagmail.SMTP(user=self.user, host=self.SMTP_163)
        contents = [
            msg
        ]
        yag.send(self.receiver, '来自电脑的邮件', contents=contents)


    def receive_mail(self):
        """收邮件"""

        # 读取密码
        password = keyring.get_password('yagmail', self.user)
        with Imbox('imap.163.com', 'sockwz@163.com', password, ssl=True) as imbox:
            all_msg = imbox.messages(unread=True)
            for uid, message in all_msg:
                if '110' == message.subject:
                    # 标记已读
                    imbox.mark_seen(uid)
                    msg = message.body['plain']
                    if msg[0].__contains__('截图'):
                        # 截图
                        self.grab_windows()
                        # 发送截图
                        self.send_main('status.jpg')
                    if msg[0].__contains__('重启'):
                        self.restart_server()
                        time.sleep(3)
                        self.grab_windows()
                        self.send_main('status.jpg')
                    if msg[0].__contains__('关机'):
                        self.shut_down()

    def grab_windows(self):
        im = ImageGrab.grab()
        im.save('status.jpg')

    def restart_server(self):
        time.sleep(3)
        os.system('shutdown')
        time.sleep(3)
        os.system('startup')



    def run_program(self):
        """运行程序"""
        with self.kb.pressed(Key.shift):
            self.kb.press(Key.f10)
            self.kb.release(Key.f10)

    def shut_down(self):
        """关机"""
        os.system('shutdown /s /t 0')

    def run_controller(self):
        schedule.every(self.TIME_SPACE).seconds.do(controller.receive_mail)
        while True:
            schedule.run_pending()
            time.sleep(self.TIME_SPACE)


if __name__ == '__main__':
    controller = ComputerController('发件邮箱', '收件邮箱')
    controller.run_controller()
```
在读取邮件时只读取主题为 110 的邮件，这样可以避免干扰邮件。所以在发送邮件时需要将主题设置为 110，而邮件内容则是一些关键词。比如截图就是截图操作，关机就是关机操作。这个可以自己定义。
