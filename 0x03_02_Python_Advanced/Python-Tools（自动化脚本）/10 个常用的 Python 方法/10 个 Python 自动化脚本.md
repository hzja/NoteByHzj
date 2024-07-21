Python
<a name="rQmlb"></a>
## 1、图片优化器
使用这个很棒的自动化脚本，可以帮助把图像处理的更好，可以像在 Photoshop 中一样编辑它们。<br />该脚本使用流行的是 Pillow 模块，可以在下面找到优化图像所需的大部分方法。

- 在图像编辑项目中使用
- 在 Python 项目中使用它
- 批量图像编辑

更多
```python
# Image Optimizing
# pip install Pillow
import PIL
# Croping 
im = PIL.Image.open("Image1.jpg")
im = im.crop((34, 23, 100, 100))
# Resizing
im = PIL.Image.open("Image1.jpg")
im = im.resize((50, 50))
# Flipping
im = PIL.Image.open("Image1.jpg")
im = im.transpose(PIL.Image.FLIP_LEFT_RIGHT)
# Rotating
im = PIL.Image.open("Image1.jpg")
im = im.rotate(360)
# Compressing
im = PIL.Image.open("Image1.jpg")
im.save("Image1.jpg", optimize=True, quality=90)
# Bluring
im = PIL.Image.open("Image1.jpg")
im = im.filter(PIL.ImageFilter.BLUR)
# Sharpening
im = PIL.Image.open("Image1.jpg")
im = im.filter(PIL.ImageFilter.SHARPEN)
# Set Brightness
im = PIL.Image.open("Image1.jpg")
im = PIL.ImageEnhance.Brightness(im)
im = im.enhance(1.5)
# Set Contrast
im = PIL.Image.open("Image1.jpg")
im = PIL.ImageEnhance.Contrast(im)
im = im.enhance(1.5)
# Adding Filters
im = PIL.Image.open("Image1.jpg")
im = PIL.ImageOps.grayscale(im)
im = PIL.ImageOps.invert(im)
im = PIL.ImageOps.posterize(im, 4)
# Saving
im.save("Image1.jpg")
```
<a name="pABbJ"></a>
## 2、视频优化器
通过使用以下自动化脚本，不仅可以使用 Python 来优化视频，还可以使用它来优化图像。该脚本使用 Moviepy 模块，允许修剪、添加音频、设置视频速度、添加 VFX 等等。

- 创建完整的视频编辑器
- 在 Python 项目中使用
- 修剪视频
- 从图像制作视频

更多
```python
# Video Optimizer
# pip install moviepy
import moviepy.editor as pyedit
# Load the Video
video = pyedit.VideoFileClip("vid.mp4")
# Trimming
vid1 = video.subclip(0, 10)
vid2 = video.subclip(20, 40)
final_vid = pyedit.concatenate_videoclips([vid1, vid2])
# Speed up the video
final_vid = final_vid.speedx(2)
# Adding Audio to the video
aud = pyedit.AudioFileClip("bg.mp3")
final_vid = final_vid.set_audio(aud)
# Reverse the Video
final_vid = final_vid.fx(pyedit.vfx.time_mirror)
# Merge two videos
vid1 = pyedit.VideoFileClip("vid1.mp4")
vid2 = pyedit.VideoFileClip("vid2.mp4")
final_vid = pyedit.concatenate_videoclips([vid1, vid2])
# Add VFX to Video
vid1 = final_vid.fx(pyedit.vfx.mirror_x)
vid2 = final_vid.fx(pyedit.vfx.invert_colors)
final_vid = pyedit.concatenate_videoclips([vid1, vid2])
# Add Images to Video
img1 = pyedit.ImageClip("img1.jpg")
img2 = pyedit.ImageClip("img2.jpg")
final_vid = pyedit.concatenate_videoclips([img1, img2])
# Save the video
final_vid.write_videofile("final.mp4")
```
<a name="hzD5q"></a>
## 3、PDF 转图片
这个小型自动化脚本可以方便地获取整个 PDF 页面并将它们转换为图像。该脚本使用流行的 PyMuPDF 模块，该模块以其 PDF 文本提取而闻名。

- 在你的 PDF 项目中使用它
- 批量 PDF 到图像

更多
```python
# PDF to Images
# pip install PyMuPDF
import fitz
def pdf_to_images(pdf_file):
    doc = fitz.open(pdf_file)
    for p in doc:
        pix = p.get_pixmap()
        output = f"page{p.number}.png"
        pix.writePNG(output)
pdf_to_images("test.pdf")
```
<a name="PyMKn"></a>
## 4、获取 API 数据
需要从数据库中获取 API 数据或需要向服务器发送 API 请求。那么这个自动化脚本对你来说是一个方便的工具。使用 Urllib3 模块，可让你获取和发布 API 请求。
```python
# pip install urllib3
import urllib3
# Fetch API data
url = "https://api.github.com/users/psf/repos"
http = urllib3.PoolManager()
response = http.request('GET', url)
print(response.status)
print(response.data)
# Post API data
url = "https://httpbin.org/post"
http = urllib3.PoolManager()
response = http.request('POST', url, fields={'hello': 'world'})
print(response.status)
```
<a name="EDwTe"></a>
## 5、电池指示灯
这个方便的脚本可以让你设置你想要得到通知的电池百分比，该脚本使用 Pyler 进行通知，使用 Psutil 获取当前的电池百分比。
```python
# Battery Notifier
# pip instal plyer
from plyer import notification
import psutil
from time import sleep
while True:
    battery = psutil.sensors_battery()
    life = battery.percent
    if life < 50:
        notification.notify(
            title = "Battery Low",
            message = "Please connect to power source",
            timeout = 10
        )
    sleep(60)
```
<a name="lI91k"></a>
## 6、语法固定器
厌倦了校对长文章或文本，然后，可以试试这个自动化脚本，它将扫描你的文本并纠正语法错误，这个很棒的脚本使用 Happtransformer 模块，这是一个机器学习模块，经过训练可以修复文本中的语法错误。
```python
# Grammer Fixer
# pip install happytransformer
from happytransformer import HappyTextToText as HappyTTT
from happytransformer import TTSettings
def Grammer_Fixer(Text):
    Grammer = HappyTTT("T5","prithivida/grammar_error_correcter_v1")
    config = TTSettings(do_sample=True, top_k=10, max_length=100)
    corrected = Grammer.generate_text(Text, args=config)
    print("Corrected Text: ", corrected.text)
Text = "This is smple tet we how know this"
Grammer_Fixer(Text)
```
<a name="yc41J"></a>
## 7、拼写修正
这个很棒的脚本将帮助你纠正你的文本单词拼写错误。你可以在下面找到脚本，将告诉你如何修复句子中的单个单词或多个单词。
```python
# Spell Fixer
# pip install textblob
from textblob import *
# Fixing Paragraph Spells
def fix_paragraph_words(paragraph):
    sentence = TextBlob(paragraph)
    correction = sentence.correct()
    print(correction)
# Fixing Words Spells
def fix_word_spell(word):
    word = Word(word)
    correction = word.correct()
    print(correction)
fix_paragraph_words("This is sammple tet!!")
fix_word_spell("maangoo")
```
<a name="uuWfA"></a>
## 8、互联网下载器
可能使用下载软件从 Internet 下载照片或视频，但现在可以使用 Python IDM 模块创建自己的下载器。

- 下载 Google 相册
- 在你的项目中使用
- 下载视频和音乐

更多
```python
# Python Downloader
# pip install internetdownloadmanager
import internetdownloadmanager as idm
def Downloader(url, output):
    pydownloader = idm.Downloader(worker=20,
                                part_size=1024*1024*10,
                                resumable=True,)

    pydownloader .download(url, output)
Downloader("Link url", "image.jpg")
Downloader("Link url", "video.mp4")
```
<a name="WGxfd"></a>
## 9、获取世界新闻
使用此自动化脚本让你随时了解每日世界新闻，可以使用任何语言从任何国家/地区获取新闻。这个 API 可以每天免费获取 50 篇新闻文章。
```python
# World News Fetcher
# pip install requests
import requests
ApiKey = "YOUR_API_KEY"
url = "https://api.worldnewsapi.com/search-news?text=hurricane&api-key={ApiKey}"
headers = {
    'Accept': 'application/json'
}
response = requests.get(url, headers=headers)
print("News: ", response.json())
```
<a name="Yu5hy"></a>
## 10、PySide2 GUI
这个自动化脚本将帮助你使用 PySide2 Gui 模块创建你的 GUI 应用程序。可以在下面找到开始开发体面的现代应用程序所需的每种方法。<br />PySide2 还支持跨平台，对开发人员非常友好，请查看下面的代码。
```python
# PySide 2 
# pip install PySide2
from PySide6.QtWidgets import *
from PySide6.QtGui import *
import sys
app = QApplication(sys.argv)
window = QWidget()
# Resize the Window
window.resize(500, 500)
# Set the Window Title
window.setWindowTitle("PySide2 Window")
# Add Buttons
button = QPushButton("Click Me", window)
button.move(200, 200)
# Add Label Text
label = QLabel("Hello Medium", window)
label.move(200, 150)
# Add Input Box
input_box = QLineEdit(window)
input_box.move(200, 250)
print(input_box.text())
# Add Radio Buttons
radio_button = QRadioButton("Radio Button", window)
radio_button.move(200, 300)
# Add Checkbox
checkbox = QCheckBox("Checkbox", window)
checkbox.move(200, 350)
# Add Slider
slider = QSlider(window)
slider.move(200, 400)
# Add Progress Bar
progress_bar = QProgressBar(window)
progress_bar.move(200, 450)
# Add Image 
image = QLabel(window)
image.setPixmap(QPixmap("image.png"))
# Add Message Box
msg = QMessageBox(window)
msg.setText("Message Box")
msg.setStandardButtons(QMessageBox.Ok | QMessageBox.Cancel)
window.show()
sys.exit(app.exec())
```
