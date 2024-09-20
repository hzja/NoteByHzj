Python
<a name="G4EXA"></a>
## 1、解析和提取 HTML
此自动化脚本将从网页 URL 中提取 HTML，然后还提供可用于解析 HTML 以获取数据的功能。这个很棒的脚本对于网络爬虫和那些想要解析 HTML 以获取重要数据的人来说是一种很好的享受。
```python
# Parse and Extract HTML
# pip install gazpacho
import gazpacho
# Extract HTML from URL
url = 'https://www.example.com/'
html = gazpacho.get(url)
print(html)
# Extract HTML with Headers
headers = {'User-Agent': 'Mozilla/5.0'}
html = gazpacho.get(url, headers=headers)
print(html)
# Parse HTML
parse = gazpacho.Soup(html)
# Find single tags
tag1 = parse.find('h1')
tag2 = parse.find('span')
# Find multiple tags
tags1 = parse.find_all('p')
tags2 = parse.find_all('a')
# Find tags by class
tag = parse.find('.class')
# Find tags by Attribute
tag = parse.find("div", attrs={"class": "test"})
# Extract text from tags
text = parse.find('h1').text
text = parse.find_all('p')[0].text
```
<a name="ddl2N"></a>
## 2、二维码扫描仪
拥有大量二维码图像或只想扫描二维码图像，那么此自动化脚本将帮助你。该脚本使用 Qrtools 模块，能够以编程方式扫描 QR 图像。
```python
# Qrcode Scanner
# pip install qrtools
from qrtools import Qr
def Scan_Qr(qr_img):
    qr = Qr()
    qr.decode(qr_img)
    print(qr.data)
    return qr.data
print("Your Qr Code is: ", Scan_Qr("qr.png"))
```
<a name="vdaxU"></a>
## 3、截图
现在，可以使用下面这个很棒的脚本以编程方式截取屏幕截图。使用此脚本，可以直接截屏或截取特定区域的屏幕截图。
```python
# Grab Screenshot
# pip install pyautogui
# pip install Pillow
from pyautogui import screenshot
import time
from PIL import ImageGrab
# Grab Screenshot of Screen
def grab_screenshot():
    shot = screenshot()
    shot.save('my_screenshot.png')
# Grab Screenshot of Specific Area
def grab_screenshot_area():
    area = (0, 0, 500, 500)
    shot = ImageGrab.grab(area)
    shot.save('my_screenshot_area.png')
# Grab Screenshot with Delay
def grab_screenshot_delay():
    time.sleep(5)
    shot = screenshot()
    shot.save('my_screenshot_delay.png')
```
<a name="yNhF2"></a>
## 4、创建有声读物
厌倦了手动将 PDF 书籍转换为有声读物，那么这个自动化脚本，它使用 GTTS 模块将 PDF 文本转换为音频。
```python
# Create Audiobooks
# pip install gTTS
# pip install PyPDF2
from PyPDF2 import PdfFileReader as reader
from gtts import gTTS
def create_audio(pdf_file):
    read_Pdf = reader(open(pdf_file, 'rb'))
for page in range(read_Pdf.numPages):
    text = read_Pdf.getPage(page).extractText()
tts = gTTS(text, lang='en')
tts.save('page' + str(page) + '.mp3')
create_audio('book.pdf')
```
<a name="hGfUr"></a>
## 5、PDF 编辑器
使用以下自动化脚本使用 Python 编辑 PDF 文件。该脚本使用 PyPDF4 模块，它是 PyPDF2 的升级版本，下面编写了 Parse Text、Remove pages 等常用功能。<br />当有大量 PDF 文件要编辑或需要以编程方式在 Python 项目中使用脚本时，这是一个方便的脚本。
```python
# PDF Editor
# pip install PyPDf4
import PyPDF4
# Parse the Text from PDF
def parse_text(pdf_file):
    reader = PyPDF4.PdfFileReader(pdf_file)
    for page in reader.pages:
        print(page.extractText())
# Remove Page from PDF
def remove_page(pdf_file, page_numbers):
    filer = PyPDF4.PdfReader('source.pdf', 'rb')
    out = PyPDF4.PdfWriter()
    for index in page_numbers:
        page = filer.pages[index] 
        out.add_page(page)
with open('rm.pdf', 'wb') as f:
        out.write(f)
# Add Blank Page to PDF
def add_page(pdf_file, page_number):
    reader = PyPDF4.PdfFileReader(pdf_file)
    writer = PyPDF4.PdfWriter()
    writer.addPage()
    with open('add.pdf', 'wb') as f:
        writer.write(f)
# Rotate Pages
def rotate_page(pdf_file):
    reader = PyPDF4.PdfFileReader(pdf_file)
    writer = PyPDF4.PdfWriter()
    for page in reader.pages:
        page.rotateClockwise(90)
        writer.addPage(page)
    with open('rotate.pdf', 'wb') as f:
        writer.write(f)
# Merge PDFs
def merge_pdfs(pdf_file1, pdf_file2):
    pdf1 = PyPDF4.PdfFileReader(pdf_file1)
    pdf2 = PyPDF4.PdfFileReader(pdf_file2)
    writer = PyPDF4.PdfWriter()
    for page in pdf1.pages:
        writer.addPage(page)
    for page in pdf2.pages:
        writer.addPage(page)
    with open('merge.pdf', 'wb') as f:
        writer.write(f)
```
<a name="ow4hN"></a>
## 6、迷你 Stackoverflow
作为一名程序员，每天都需要 StackOverflow，但不再需要在 Google 上搜索它。现在，在继续处理项目的同时，在 CMD 中获得直接解决方案。通过使用 Howdoi 模块，可以在命令提示符或终端中获得 StackOverflow 解决方案。可以在下面找到一些可以尝试的示例。
```bash
# Automate Stackoverflow
# pip install howdoi
# Get Answers in CMD
#example 1
> howdoi how do i install python3
# example 2
> howdoi selenium Enter keys
# example 3
> howdoi how to install modules
# example 4
> howdoi Parse html with python
# example 5
> howdoi int not iterable error
# example 6
> howdoi how to parse pdf with python
# example 7
> howdoi Sort list in python
# example 8
> howdoi merge two lists in python
# example 9
>howdoi get last element in list python
# example 10
> howdoi fast way to sort list
```
<a name="dr8rN"></a>
## 7、自动化手机
此自动化脚本使用 Python 中的 Android 调试桥 (ADB) 自动化智能手机。下面将展示如何自动执行常见任务，例如滑动手势、呼叫、发送短信等等。<br />可以了解有关 ADB 的更多信息，并探索更多令人兴奋的方法来实现手机自动化，让生活更轻松。
```python
# Automate Mobile Phones
# pip install opencv-python
import subprocess
def main_adb(cm):
    p = subprocess.Popen(cm.split(' '), stdout=subprocess.PIPE, shell=True)
    (output, _) = p.communicate()
    return output.decode('utf-8')
# Swipe 
def swipe(x1, y1, x2, y2, duration):
    cmd = 'adb shell input swipe {} {} {} {} {}'.format(x1, y1, x2, y2, duration)
    return main_adb(cmd)
# Tap or Clicking
def tap(x, y):
    cmd = 'adb shell input tap {} {}'.format(x, y)
    return main_adb(cmd)
# Make a Call
def make_call(number):
    cmd = f"adb shell am start -a android.intent.action.CALL -d tel:{number}"
    return main_adb(cmd)
# Send SMS
def send_sms(number, message):
    cmd = 'adb shell am start -a android.intent.action.SENDTO -d  sms:{} --es sms_body "{}"'.format(number, message)
    return main_adb(cmd)
# Download File From Mobile to PC
def download_file(file_name):
    cmd = 'adb pull /sdcard/{}'.format(file_name)
    return main_adb(cmd)
# Take a screenshot
def screenshot():
    cmd = 'adb shell screencap -p'
    return main_adb(cmd)
# Power On and Off
def power_off():
    cmd = '"adb shell input keyevent 26"'
    return main_adb(cmd)
```
<a name="uib83"></a>
## 8、监控 CPU/GPU 温度
可能使用 CPU-Z 或任何规格监控软件来捕获 Cpu 和 Gpu 温度，也可以通过编程方式进行。这个脚本使用 Pythonnet 和 OpenhardwareMonitor 来监控当前的 Cpu 和 Gpu 温度。<br />可以使用它在达到一定温度时通知自己，也可以在 Python 项目中使用它来简化日常生活。
```python
# Get CPU/GPU Temperature
# pip install pythonnet
import clr
clr.AddReference("OpenHardwareMonitorLib")
from OpenHardwareMonitorLib import *
spec = Computer()
spec.GPUEnabled = True
spec.CPUEnabled = True
spec.Open()
# Get CPU Temp
def Cpu_Temp():
    while True:
        for cpu in range(0, len(spec.Hardware[0].Sensors)):
            if "/temperature" in str(spec.Hardware[0].Sensors[cpu].Identifier):
                print(str(spec.Hardware[0].Sensors[cpu].Value))
# Get GPU Temp
def Gpu_Temp()
    while True:
        for gpu in range(0, len(spec.Hardware[0].Sensors)):
            if "/temperature" in str(spec.Hardware[0].Sensors[gpu].Identifier):
                print(str(spec.Hardware[0].Sensors[gpu].Value))
```
<a name="ocaEE"></a>
## 9、Instagram 上传机器人
Instagram 是一个著名的社交媒体平台，现在不需要通过智能手机上传照片或视频。可以使用以下脚本以编程方式执行此操作。
```python
# Upload Photos and Video on Insta
# pip install instabot
from instabot import Bot
def Upload_Photo(img):
    robot = Bot()
    robot.login(username="user", password="pass")
    robot.upload_photo(img, caption="Medium Article")
    print("Photo Uploaded")
def Upload_Video(video):
    robot = Bot()
    robot.login(username="user", password="pass")
    robot.upload_video(video, caption="Medium Article")
    print("Video Uploaded")
def Upload_Story(img):
    robot = Bot()
    robot.login(username="user", password="pass")
    robot.upload_story(img, caption="Medium Article")
    print("Story Photos Uploaded")
Upload_Photo("img.jpg")
Upload_Video("video.mp4")
```
<a name="hiUp9"></a>
## 10、视频水印
使用此自动化脚本为视频添加水印，该脚本使用 Moviepy，这是一个方便的视频编辑模块。在下面的脚本中，可以看到如何添加水印并且可以自由使用它。
```python
# Video Watermark with Python
# pip install moviepy
from moviepy.editor import *
clip = VideoFileClip("myvideo.mp4", audio=True) 
width,height = clip.size  
text = TextClip("WaterMark", font='Arial', color='white', fontsize=28)
set_color = text.on_color(size=(clip.w + text.w, text.h-10), color=(0,0,0), pos=(6,'center'), col_opacity=0.6)
set_textPos = set_color.set_pos( lambda pos: (max(width/30,int(width-0.5* width* pos)),max(5*height/6,int(100* pos))) )
Output = CompositeVideoClip([clip, set_textPos])
Output.duration = clip.duration
Output.write_videofile("output.mp4", fps=30, codec='libx264')
```
