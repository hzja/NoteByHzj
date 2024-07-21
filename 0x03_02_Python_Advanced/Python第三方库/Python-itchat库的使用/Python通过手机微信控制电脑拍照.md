Python 微信 itchat
```python
"""
@author:Fcant
@description：手机微信控制电脑拍照
@date: 2019-02-15/0015 下午 21:03
"""

import os
import cv2
import time
import itchat

sendMsg = u"{消息助手}:暂时无法回复"
usageMsg = u"使用方法：\n1.运行CMD命令：cmd xxx（xxx为命令）\n" \
           u"-例如关机命令：\ncmd shutdown -s -t 0 \n" \
           u"2.获取当前电脑用户：cap\n3.启用消息助手（默认关闭）：ast\n" \
           u"4.关闭消息助手：astc"
# 消息助手开关
flag = 0

nowTime = time.localtime()
filename = str(nowTime.tm_mday) + str(nowTime.tm_hour) + str(nowTime.tm_min) + str(nowTime.tm_sec)
myfile = open(filename, 'w')


@itchat.msg_register('Text')
def text_reply(msg):
    global flag
    message = msg['Text']
    fromName = msg['FromUserName']
    toName = msg['ToUserName']

    if toName == 'filehelper':
        if message == "cap":
            cap = cv2.VideoCapture(0)
            ret, img = cap.read()
            cv2.imwrite("winxinTemp.jpg", img)
            itchat.send('@img@%s' % u'winxinTemp.jpg', 'filehelper')
            cap.release()
        if message[0:3] == "cmd":
            os.system(message.strip(message[0:4]))
        if message == "ast":
            flag = 1
            itchat.send("消息助手已开启", "filehelper")
        if message == "astc":
            flag = 0
            itchat.send("消息助手已关闭", "filehelper")
    elif flag == 1:
        itchat.send(sendMsg, fromName)
        myfile.write(message)
        myfile.write("\n")
        myfile.flush()


if __name__ == '__main__':
    itchat.auto_login()
    itchat.send(usageMsg, 'filehelper')
    itchat.run()
```
