Python
<a name="eLskb"></a>
### 1、使用 Python 进行速度测试
这个高级脚本使用 Python 测试 Internet 速度。只需安装速度测试模块并运行以下代码。
```python
# pip install pyspeedtest
# pip install speedtest
# pip install speedtest-cli
#method 1
import speedtest
speedTest = speedtest.Speedtest() 
print(speedTest.get_best_server())
#Check download speed
print(speedTest.download())
#Check upload speed
print(speedTest.upload())
# Method 2
import pyspeedtest
st = pyspeedtest.SpeedTest()
st.ping()
st.download()
st.upload()
```
<a name="b5wvg"></a>
### 2、在谷歌上搜索
可以从 Google 搜索引擎中提取重定向 URL，安装以下提及模块并遵循代码。
```python
# pip install google
from googlesearch import search
query = "Medium.com"

for url in search(query):
    print(url)
```
<a name="Mdp5o"></a>
### 3、制作网络机器人
该脚本将使用 Python 自动化网站。可以构建一个可控制任何网站的网络机器人。查看下面的代码，这个脚本在网络抓取和网络自动化中很方便。
```python
# pip install selenium
import time
from selenium import webdriver
from selenium.webdriver.common.keys 
import Keysbot = webdriver.Chrome("chromedriver.exe")
bot.get('http://www.google.com')
search = bot.find_element_by_name('q')
search.send_keys("@codedev101")
search.send_keys(Keys.RETURN)
time.sleep(5)
bot.quit()
```
<a name="wM2Bw"></a>
### 4、获取歌曲歌词
这个高级脚本将展示如何从任何歌曲中获取歌词。首先，必须从 Lyricsgenius 网站获得免费的 API 密钥，然后，必须遵循以下代码。
```python
# pip install lyricsgenius
import lyricsgenius
api_key = "xxxxxxxxxxxxxxxxxxxxx"
genius = lyricsgenius.Genius(api_key)
artist = genius.search_artist("Pop Smoke", 
max_songs=5,sort="title")
song = artist.song("100k On a Coupe")
print(song.lyrics)
```
<a name="FA1MT"></a>
### 5、获取照片的Exif数据
使用 Python Pillow 模块获取任何照片的 Exif 数据。查看下面提到的代码。提供了两种方法来提取照片的 Exif 数据。
```python
# Get Exif of Photo
# Method 1
# pip install pillow
import PIL.Image
import PIL.ExifTags
img = PIL.Image.open("Img.jpg")
exif_data = 
{
    PIL.ExifTags.TAGS[i]: j
    for i, j in img._getexif().items()
    if i in PIL.ExifTags.TAGS
}
print(exif_data)
# Method 2
# pip install ExifRead
import exifread
filename = open(path_name, 'rb')
tags = exifread.process_file(filename)
print(tags)
```
<a name="KPmSG"></a>
### 6、提取图像中的 OCR 文本
OCR 是一种从数字和扫描文档中识别文本的方法。许多开发人员使用它来读取手写数据，下面的 Python 代码可以将扫描的图像转换为 OCR 文本格式。<br />注意：必须从 Github 下载 tesseract.exe
```python
# pip install pytesseract
import pytesseract
from PIL import Image

pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

t=Image.open("img.png")
text = pytesseract.image_to_string(t, config='')
print(text)
```
<a name="HHK6j"></a>
### 7、将照片转换为Cartonize
这个简单的高级脚本会将照片转换为 Cartonize 格式。查看下面的示例代码并尝试一下。
```python
# pip install opencv-python
import cv2

img = cv2.imread('img.jpg')
grayimg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
grayimg  = cv2.medianBlur(grayimg, 5)

edges = cv2.Laplacian(grayimg , cv2.CV_8U, ksize=5)
r,mask =cv2.threshold(edges,100,255,cv2.THRESH_BINARY_INV)
img2 = cv2.bitwise_and(img, img, mask=mask)
img2 = cv2.medianBlur(img2, 5)

cv2.imwrite("cartooned.jpg", mask)
```
<a name="sYDVB"></a>
### 8、清空回收站
这个简单的脚本可以用 Python 清空回收站，查看下面的代码以了解如何操作。
```python
# pip install winshell
import winshell
try:
    winshell.recycle_bin().empty(confirm=False, /show_progress=False, sound=True)
    print("Recycle bin is emptied Now")
except:
    print("Recycle bin already empty")
```
<a name="ZgHTZ"></a>
### 9、Python 图像增强
使用 Python Pillow 库增强照片以使其看起来更好。在下面的代码中，实现了四种方法来增强任何照片。
```python
# pip install pillow
from PIL import Image,ImageFilter
from PIL import ImageEnhance

im = Image.open('img.jpg')

# Choose your filter
# add Hastag at start if you don't want to any filter below
en = ImageEnhance.Color(im)
en = ImageEnhance.Contrast(im)
en = ImageEnhance.Brightness(im)
en = ImageEnhance.Sharpness(im)# result
en.enhance(1.5).show("enhanced")
```
<a name="q5PCo"></a>
### 10、获取 Window 版本
这个简单的脚本将获得当前使用的完整窗口版本。
```python
# Window Versionimport wmi
data = wmi.WMI()
for os_name in data.Win32_OperatingSystem():
  print(os_name.Caption)
# Microsoft Windows 11 Home
```
<a name="TgZgc"></a>
### 11、将 PDF 转换为图像
使用以下代码将所有 Pdf 页转换为图像。
```python
# PDF to Images
import fitz
pdf = 'sample_pdf.pdf'
doc = fitz.open(pdf)

for page in doc:
    pix = page.getPixmap(alpha=False)
    pix.writePNG('page-%i.png' % page.number)
```
<a name="iUyVZ"></a>
### 12、转换：十六进制到 RGB
该脚本将简单地将 Hex 转换为 RGB。查看下面的示例代码。
```python
# Conversion: Hex to RGB
def Hex_to_Rgb(hex):
    h = hex.lstrip('#')
    return tuple(int(h[i:i+2], 16) for i in (0, 2, 4))
print(Hex_to_Rgb('#c96d9d'))  # (201, 109, 157)
print(Hex_to_Rgb('#fa0515')) # (250, 5, 21)
```
<a name="T63tC"></a>
### 13、网站状态
可以使用 Python 检查网站是否正常运行。检查以下代码，显示200 ，表示网站已启动，如果显示为 404 ，则表示网站已关闭。
```python
# pip install requests
#method 1
import urllib.request
from urllib.request import Request, urlopenreq = Request('https://medium.com/@pythonians', headers={'User-Agent': 'Mozilla/5.0'})
webpage = urlopen(req).getcode()
print(webpage)  # 200
# method 2
import requests
r = requests.get("https://medium.com/@pythonians")
print(r.status_code) # 200
```
