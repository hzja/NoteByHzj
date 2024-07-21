Python
<a name="fakUD"></a>
### ① 抓取知乎图片，只用30行代码
```python
from selenium import webdriver
import time
import urllib.request

driver = webdriver.Chrome()
driver.maximize_window()
driver.get("https://www.zhihu.com/question/29134042")
i = 0
while i < 10:
    driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
    time.sleep(2)
    try:
        driver.find_element_by_css_selector('button.QuestionMainAction').click()
        print("page" + str(i))
        time.sleep(1)
    except:
        break
result_raw = driver.page_source
content_list = re.findall("img src=\"(.+?)\" ", str(result_raw))
n = 0
while n < len(content_list):
    i = time.time()
    local = (r"%s.jpg" % (i))
    urllib.request.urlretrieve(content_list[n], local)
    print("编号：" + str(i))
    n = n + 1
```
<a name="BDvlv"></a>
### ② 没事闲的时候，听两个聊天机器人互相聊天
```python
from time import sleep
import requests
s = input("请主人输入话题：")
while True:
    resp = requests.post("http://www.tuling123.com/openapi/api",data={"key":"4fede3c4384846b9a7d0456a5e1e2943", "info": s, })
    resp = resp.json()
    sleep(1)
    print('小鱼：', resp['text'])
    s = resp['text']
    resp = requests.get("http://api.qingyunke.com/api.php", {'key': 'free', 'appid':0, 'msg': s})
    resp.encoding = 'utf8'
    resp = resp.json()
    sleep(1)
    print('菲菲：', resp['content'])
#网上还有一个据说智商比较高的小i机器人，用爬虫的功能来实现一下：

import urllib.request
import re

while True:
    x = input("主人：")
    x = urllib.parse.quote(x)
    link = urllib.request.urlopen(
        "http://nlp.xiaoi.com/robot/webrobot?&callback=__webrobot_processMsg&data=%7B%22sessionId%22%3A%22ff725c236e5245a3ac825b2dd88a7501%22%2C%22robotId%22%3A%22webbot%22%2C%22userId%22%3A%227cd29df3450745fbbdcf1a462e6c58e6%22%2C%22body%22%3A%7B%22content%22%3A%22" + x + "%22%7D%2C%22type%22%3A%22txt%22%7D")
    html_doc = link.read().decode()
    reply_list = re.findall(r'\"content\":\"(.+?)\\r\\n\"', html_doc)
    print("小i：" + reply_list[-1])

```
<a name="gwPEU"></a>
### ③ 分析唐诗的作者是李白还是杜甫
```python
import jieba
from nltk.classify import NaiveBayesClassifier

# 需要提前把李白的诗收集一下，放在libai.txt文本中。
text1 = open(r"libai.txt", "rb").read()
list1 = jieba.cut(text1)
result1 = " ".join(list1)
# 需要提前把杜甫的诗收集一下，放在dufu.txt文本中。
text2 = open(r"dufu.txt", "rb").read()
list2 = jieba.cut(text2)
result2 = " ".join(list2)

# 数据准备
libai = result1
dufu = result2


# 特征提取
def word_feats(words):
    return dict([(word, True) for word in words])


libai_features = [(word_feats(lb), 'lb') for lb in libai]
dufu_features = [(word_feats(df), 'df') for df in dufu]
train_set = libai_features + dufu_features
# 训练决策
classifier = NaiveBayesClassifier.train(train_set)

# 分析测试
sentence = input("请输入一句你喜欢的诗：")
print("\n")
seg_list = jieba.cut(sentence)
result1 = " ".join(seg_list)
words = result1.split(" ")

# 统计结果


lb = 0
df = 0
for word in words:
    classResult = classifier.classify(word_feats(word))
    if classResult == 'lb':
        lb = lb + 1
    if classResult == 'df':
        df = df + 1

# 呈现比例
x = float(str(float(lb) / len(words)))
y = float(str(float(df) / len(words)))
print('李白的可能性：%.2f%%' % (x * 100))
print('杜甫的可能性：%.2f%%' % (y * 100))
```
<a name="CV9QZ"></a>
### ④ 彩票随机生成35选7
```python
import random

temp = [i + 1 for i in range(35)]
random.shuffle(temp)
i = 0
list = []
while i < 7:
    list.append(temp[i])
    i = i + 1
list.sort()
print('\033[0;31;;1m')
print(*list[0:6], end="")
print('\033[0;34;;1m', end=" ")
print(list[-1])
```
<a name="Ucwud"></a>
### ⑤ 自动写检讨书
```python
import random
import xlrd

ExcelFile = xlrd.open_workbook(r'test.xlsx')
sheet = ExcelFile.sheet_by_name('Sheet1')
i = []
x = input("请输入具体事件：")
y = int(input("老师要求的字数："))
while len(str(i)) < y * 1.2:
    s = random.randint(1, 60)
    rows = sheet.row_values(s)
    i.append(*rows)
print(" "*8+"检讨书"+"\n"+"老师：")
print("我不应该" + str(x)+"，", *i)
print("再次请老师原谅！")
'''
以下是样稿：

请输入具体事件：抽烟
老师要求的字数：200
        检讨书
老师：
我不应该抽烟， 学校一开学就三令五申，一再强调校规校纪，提醒学生不要违反校规，可我却没有把学校和老师的话放在心上，没有重视老师说的话，没有重视学校颁布的重要事项，当成了耳旁风，这些都是不应该的。同时也真诚地希望老师能继续关心和支持我，并却对我的问题酌情处理。 无论在学习还是在别的方面我都会用校规来严格要求自己，我会把握这次机会。 但事实证明，仅仅是热情投入、刻苦努力、钻研学业是不够的，还要有清醒的政治头脑、大局意识和纪律观念，否则就会在学习上迷失方向，使国家和学校受损失。
再次请老师原谅！
'''

```
<a name="oIyuT"></a>
### ⑥ 屏幕录相机，抓屏软件
```python
from time import sleep
from PIL import ImageGrab

m = int(input("请输入想抓屏几分钟："))
m = m * 60
n = 1
while n < m:
    sleep(0.02)
    im = ImageGrab.grab()
    local = (r"%s.jpg" % (n))
    im.save(local, 'jpeg')
    n = n + 1
```
<a name="dcA4I"></a>
### ⑦ 制作Gif动图
```python
from PIL import Image

im = Image.open("1.jpg")
images = []
images.append(Image.open('2.jpg'))
images.append(Image.open('3.jpg'))
im.save('gif.gif', save_all=True, append_images=images, loop=1, duration=1, comment=b"aaabb")
```
