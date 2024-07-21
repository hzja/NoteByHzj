Python<br />在日常的工作学习当中，总会遇到各式各样的问题，其中不少的问题都是一遍又一遍简单重复的操作，不妨直接用Python脚本来自动化处理，分享10个Python高级脚本，减少无谓的时间浪费，提高工作学习中的效率。
<a name="LdZ7e"></a>
## 给照片添加水印
给照片添加水印的代码多种多样，下面这种的或许是最为简单的形式，
```python
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw

def watermark_Image(img_path,output_path, text, pos):
    img = Image.open(img_path)
    drawing = ImageDraw.Draw(img)
    black = (10, 5, 12)
    drawing.text(pos, text, fill=black)
    img.show()
    img.save(output_path)

img = '2.png'
watermark_Image(img, 'watermarked_2.jpg','Fcant', pos=(10, 10))
```
<a name="PgFBy"></a>
## 检测文本文件的相似性
很多时候需要来检查两文件的相似性，到底存在着多少的雷同，或许以下的这个脚本文件可以派得上用场
```python
from difflib import SequenceMatcher

def file_similarity_checker(f1, f2):
    with open(f1, errors="ignore") as file1, open(f2, errors="ignore") as file2:
        f1_data = file1.read()
        f2_data = file2.read()
        checking = SequenceMatcher(None, f1_data, f2_data).ratio()
        print(f"These files are {checking*100} % similar")
        
file_1 = "路径1"
file_2 = "路径2"
file_similarity_checker(file_1, file_2)
```
<a name="aSCag"></a>
## 对文件内容进行加密
有时候手中文件的内容十分的重要、十分地机密，可以选择对此进行加密，代码如下
```python
from cryptography.fernet import Fernet

def encrypt(filename, key):
    fernet = Fernet(key)
    with open(filename, 'rb') as file:
        original = file.read()
    encrypted = fernet.encrypt(original)
    with open(filename, 'wb') as enc_file:
        enc_file.write(encrypted)
        
key = Fernet.generate_key()
filename = "file.txt"
encrypt(filename, key)
```
生成密钥，然后对文件内容进行加密，当然这个密钥后面在对文件进行解密的时候会派上用场，因此密钥一定要保存完好，解密的代码如下
```python
def decrypt(filename, key):
    fernet = Fernet(key)
    with open(filename, 'rb') as enc_file:
        encrypted = enc_file.read()
    decrypted = fernet.decrypt(encrypted)
    with open(filename, 'wb') as dec_file:
        dec_file.write(decrypted)

decrypt(filename, key)
```
上面的脚本，其中的密钥是一个随机生成的随机数，当然密钥也可以是自己指定的，代码如下
```python
import pyAesCrypt

def Encryption(input_file_path, output_file_path, key):
    pyAesCrypt.encryptFile(input_file_path, output_file_path, key)
    print("File has been decrypted")

def Decryption(input_file_path, output_file_path, key):
    pyAesCrypt.decryptFile(input_file_path, output_file_path, key)
    print("File has been decrypted")
```
<a name="D4BIj"></a>
## 将照片转换为PDF
有时候需要将照片转换成PDF格式，或者将照片依次添加到PDF文件当中去，代码如下
```python
import os
import img2pdf

with open("Output.pdf", "wb") as file:
    file.write(img2pdf.convert([i for i in os.listdir('文件路径') if i.endswith(".jpg")]))
```
<a name="ssp5F"></a>
## 修改照片的长与宽
要是想要修改照片的长度和宽度的话，下面的这个代码可以帮得上忙，代码如下
```python
from PIL import Image
import os
def img_resize(file, h, w):
  img = Image.open(file)
    Resize = img.resize((h,w), Image.ANTIALIAS)
    Resize.save('resized.jpg', 'JPEG', quality=90)
    
img_resize("文件路径", 400, 200)
```
<a name="gVbn7"></a>
## 对于照片的其他操作
除了上面修改照片的长度与宽度之外，针对照片还有其他的操作，例如
<a name="qvKjP"></a>
### 模糊化照片的内容
```python
img = Image.open('1.jpg')
blur = img.filter(ImageFilter.BLUR)
blur.save('output.jpg')
```
<a name="SWVpB"></a>
### 照片翻转90度
```python
img = Image.open('1.jpg')
rotate = img.rotate(90)
rotate.save('output.jpg')
```
<a name="PoIIY"></a>
### 照片进行锐化的处理
```python
img = Image.open('1.jpg')
sharp = img.filter(ImageFilter.SHARPEN)
sharp.save('output.jpg')
```
<a name="YCrCc"></a>
### 照片左右对称翻转
代码如下
```python
img = Image.open('1.jpg')
transpose = img.transpose(Image.FLIP_LEFT_RIGHT)
transpose.save('output.jpg')
```
<a name="Rb9wQ"></a>
### 照片进行灰度处理
```python
img = Image.open('1.jpg')
convert = img.convert('L')
convert.save('output.jpg')
```
<a name="YtI6d"></a>
## 测试网速
在开始测网速之前需要提前下载好依赖的模块
```bash
pip install speedtest-cli
```
然后开始尝试测试一下网速
```python
from speedtest import Speedtest

def Testing_Speed(net):
    download = net.download()
    upload = net.upload()
    print(f'下载速度: {download/(1024*1024)} Mbps')
    print(f'上传速度: {upload/(1024*1024)} Mbps')
    print("开始网速的测试 ...")

net = Speedtest()
Testing_Speed(net)
```
<a name="Bxpkz"></a>
## 货币汇率的转换
例如想要看一下美元与英镑之间的汇率转换，100美元可以换成多少的英镑，代码如下
```python
# 导入模块
from currency_converter import CurrencyConverter
from datetime import date
# 案例一
conv = CurrencyConverter()
c = conv.convert(100, 'USD', 'GBP')
print(round(c, 2)) # 保留两位小数
```
或者想要看一下美元与欧元之间的汇率转换，100美元可以换成多少的欧元，
```python
# 案例二
c = conv.convert(100, 'USD', 'EUR', date=date(2022, 3, 30))
print(round(c, 2)) # 44.1
```
<a name="ZLcbI"></a>
## 生成二维码
其中包括了二维码的生成以及二维码的解析，代码如下
```python
import qrcode
from PIL import Image
from pyzbar.pyzbar import decode

def Generate_qrcode(data):
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=10,
        border=4,)
    qr.add_data(data)
    qr.make(fit=True)
    image = qr.make_image(fill_color="black", back_color="white")
    image.save("qrcode.png")

Generate_qrcode("Hello")
```
再来看一下二维码的解析，代码如下
```python
def Decode_Qrcode(file_name):
    result = decode(Image.open(file_name))
    print("Data:", result[0][0].decode())

Decode_Qrcode("文件名")
```
<a name="rIKgf"></a>
## 制作一个简单的网页应用
调用的是Python当中的flask模块来制作网页应用，代码如下
```python
from flask import Flask
app = Flask(__name__)

@app.route("/")
def home():
	return "Hello World!"

@app.route("/python")
def test():
	return "Hello"

if __name__ == "__main__":
    app.run(debug=True)
```
