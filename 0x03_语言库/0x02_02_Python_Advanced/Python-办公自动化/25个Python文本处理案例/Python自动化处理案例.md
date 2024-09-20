Python
<a name="nZAAB"></a>
## 一：jpg与png互换格式
```python
from PIL import Image

img = Image.open('zhinan1.png')
img.save('zhinan2.jpg')

img1 = Image.open('zhinan2.jpg')
img1.save('zhinan3.png')
```
<a name="aFUhQ"></a>
## 二：拼写检查器
```python
#只适用于英文检查
import textblob
word = 'accomadation'
print('原文本：'+str(word))

checked = textblob.TextBlob(word)
print('纠错后：'+str(checked.correct()))
```
<a name="rpqVo"></a>
## 三：PDF加密解密
```python
# PDF加密
import pikepdf
 
pdf = pikepdf.open("test.pdf")
pdf.save('encrypt.pdf', encryption=pikepdf.Encryption(owner="your_password", user="your_password", R=4))
pdf.close()

# PDF解密
import pikepdf
 
pdf = pikepdf.open("encrypt.pdf",  password='your_password')
pdf.save("decrypt.pdf")
pdf.close()
```
<a name="sZ0wg"></a>
## 四：解压文件
```python
# 解压文件
from zipfile import ZipFile
 
unzip = ZipFile("file.zip", "r")
unzip.extractall("output Folder")
```
<a name="pzO3x"></a>
## 五：获取计算机信息‍
```python
import wmi
 
 
def System_spec():
    Pc = wmi.WMI()
    os_info = Pc.Win32_OperatingSystem()[0]
    processor = Pc.Win32_Processor()[0]
    Gpu = Pc.Win32_VideoController()[0]
    os_name = os_info.Name.encode('utf-8').split(b'|')[0]
    ram = float(os_info.TotalVisibleMemorySize) / 1048576
 
    print(f'操作系统: {os_name}')
    print(f'CPU: {processor.Name}')
    print(f'内存: {ram} GB')
    print(f'显卡: {Gpu.Name}')
 
    print("\n计算机信息如上 ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑")
 
 
System_spec()
```
<a name="CqFOS"></a>
## 六：合并表格
```python
import pandas as pd
 
# 文件名
filename = "test.xlsx"
# 表格数量
T_sheets = 5
 
df = []
for i in range(1, T_sheets+1):
    sheet_data = pd.read_excel(filename, sheet_name=i, header=None)
    df.append(sheet_data)
 
# 合并表格
output = "merged.xlsx"
df = pd.concat(df)
df.to_excel(output)
```
```python
# 合并指定的多个Excel文件
# 导入python-office库
import office
office.excel.merge2excel(dir_path=r'E:\python\excel',output_file ='test.xlsx')
# dir_path = 文件夹位置，把需要合并的多个excel文件放到同一个文件夹里
# output_file = 最终合并的excel文件位置
```
<a name="MMkUx"></a>
## 七：转成像素图
```python
# 图像转换
import cv2
 
# 读取图片
img = cv2.imread("img.jpg")
# 灰度
grey = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
invert = cv2.bitwise_not(grey)
# 高斯滤波
blur_img = cv2.GaussianBlur(invert, (7, 7), 0)
inverse_blur = cv2.bitwise_not(blur_img)
sketch_img = cv2.divide(grey, inverse_blur, scale=256.0)
# 保存
cv2.imwrite('sketch.jpg', sketch_img)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
<a name="d2ezq"></a>
## 八：提取PDF表格
```python
import camelot
 
tables = camelot.read_pdf("tables.pdf")
print(tables)
tables.export("extracted.csv", f="csv", compress=True)
```
<a name="MDOpE"></a>
## 九：word转PDF
```python
# 方法一
from docx2pdf import convert
convert(f"./{docx_name}.docx")

# 方法二(批量word转pdf)
# 导入python-office
import office  

# path里面填写存放word 后缀为.docx文件的位置
path = '.' 
# 程序自动将该目录下所有word自动转pdf
office.word.docx2pdf(path=path)  
```
<a name="ruuuU"></a>
## 十：照片添加水印
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

img = 'nan.png'
watermark_Image(img, 'zhinan.jpg','Python', pos=(10, 10))
```
<a name="Ot3Nh"></a>
## 十一：生成二维码
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
    image.save("src.png")

Generate_qrcode("Python")
```
<a name="RURdj"></a>
## 十二：简单制作网络应用
```python
from flask import Flask
app = Flask(__name__)

@app.route("/")
def home():
    return "Hello World!"
    
@app.route("/python")
def test():
    return "茫茫人海，感谢相遇"
    
if __name__ == "__main__":
    app.run(debug=True)
```
