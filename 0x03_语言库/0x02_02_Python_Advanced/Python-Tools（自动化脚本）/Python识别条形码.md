Python<br />条码在生活中随处可见，其可分为三类：一维条码、二维条码、三维条码<br />一维条码：<br />平时习惯称为条形码。条形码是将宽度不等的多个黑条和空白，按照一定的编码规则排列，用以表达一组信息的图形标识符。常见的条形码是由反射率相差很大的黑条（简称条）和白条（简称空）排成的平行线图案。<br />二维条码：<br />二维条码简称为二维码，常见的二维码为QR Code，QR全称Quick Response，是一个近几年来移动设备上超流行的一种编码方式，它比传统的Bar Code条形码能存更多的信息，也能表示更多的数据类型。<br />三维条码：<br />三维条码具有更大的信息容量、相同的识别便易性和较好的安全性。三维码的主要特征在于利用色彩或灰度（或称黑密度）表示不同的数据并进行编码。<br />其实Python的条码扫描库，一直都有一个很是出名，那就是zbar,但此库虽然牛，却已经停止维护了，如果是python3,则不能使用zbar库了，现在对于Python3来说使用比较多的是：pyzbar
:::tips
Mac系统的电脑单纯只安装pyzbar是有问题的。需要安装系统支持的zbar，使用：`brew install zbar`：
:::
然后再安装pyzbar，就没有问题了！
```python
pip install pyzbar
```
如果需要摄像头识别条形码或者二维码需要安装opencv和PIL，OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows、Android和Mac OS操作系统上。OpenCV-Python是OpenCV的Python的API接口，它拥有OpenCV C++ API的功能，同时也拥有Python语言的特性,可以做到跨平台使用。
```python
pip3 install opencv-python
```
<a name="OPRKm"></a>
### 步骤1
使用opencv2，初始化摄像头信息，调整摄像头识别图像的width和height。
```python
import cv2

capture = cv2.VideoCapture(0)

# 摄像头设置，
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 1024)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 1024)
capture.set(cv2.CAP_PROP_EXPOSURE, 0.1)
```
<a name="hrmwP"></a>
### 步骤2
测试cv2是否可以，调用摄像头识别图像，按esc退出
```python
while True:
    # 读取摄像头中的图像，ok为是否读取成功的判断参数
    ret,img = capture.read()
    cv2.imshow('frame', img)
    k = cv2.waitKey(1)
    if k == 27:    # 'ESC'关闭
        break
```
<a name="kw9MW"></a>
### 步骤3
如果上面的测试没有问题就开始进入识别系统，当摄像头读取到信息之后，要将其转成灰度图，这样可以更准确的识别到条形码的信息。之所以使用`while True`，是指扫描不成功的情况下，可以多次识别。
```python
import cv2
import csv
import pyzbar.pyzbar as pyzbar

barcodeData1 = ''

found = set()
capture = cv2.VideoCapture(0)

# 摄像头设置
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 1024)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 1024)
capture.set(cv2.CAP_PROP_EXPOSURE, 0.1)


while True:
    # 读取摄像头中的图像，ok为是否读取成功的判断参数
    ret, frame = capture.read()
    # 转为灰度图像
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    barcodes = pyzbar.decode(gray)
    print(barcodes)
```
如果识别出来条形码信息则打印如下：
```python
[Decoded(data=b'9787545152210', type='EAN13', 
         rect=Rect(left=677, top=116, width=195, height=136), 
         polygon=[Point(x=677, y=239), Point(x=677, y=249), 
                  Point(x=771, y=251), Point(x=871, y=252), 
                  Point(x=872, y=118), Point(x=872, y=116), 
                  Point(x=678, y=117)])
]
```
上面的信息包含了data即条形码的值，还有就是Rect矩形区域即识别出的条形码的区域，以及识别的一些像素点。<br />如果没有识别出条形码信息，则打印出来的是空列表[ ]。
<a name="aFWKG"></a>
### 步骤4
上面👆代码中的barcodes返回的是一个列表信息，可以对列表信息进行遍历。获取条形码值，type类型和矩形区域。并使用cv2将上图所示的矩形绘制出来。
```python
for barcode in barcodes:
    # 提取条形码的边界框的位置
    (x, y, w, h) = barcode.rect
    # 画出图像中条形码的边界框
    cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 255, 0), 2)
```
<a name="LW3lK"></a>
### 步骤5
data=b'9787545152210'是字节对象，此时需要将字节对象转成字符串。
```python
# 条形码数据为字节对象，所以如果想在输出图像上
# 画出来，就需要先将它转换成字符串
barcodeData = barcode.data.decode("utf-8")
barcodeType = barcode.type
```
<a name="vBFJ7"></a>
### 步骤6
读取商品信息表，判断识别出的条形码信息，是否在商品表中，为了避免扫描重复多次识别。使用了集合去重复条形码信息。
```python
code_set = set() 
# 存放条形码的集合    
# 避免重复读取        
if barcodeData not in code_set:            
    with open('shopping.csv', 'r') as rs:                
        reader = csv.reader(rs)                
        # 遍历超市库存文件                
        for line in reader:                    
            if barcodeData in line:  
                # 说明超市有这个商品                        
                print(f'本超市存在此商品,名称：{line[1]},价格:{line[3]}')                        
                break                    
            else:                        
                pass        
            else:            
                pass        
            code_set.add(barcodeData)
```
<a name="BiKRc"></a>
### 步骤7
打开摄像头，如果按键是ESC则退出，识别系统
```python
cv2.imshow('qrcode+barcode', frame)    
k = cv2.waitKey(1)   
if k == 27:        
    breakcv2.destroyAllWindows()
```
完整代码：
```python
import cv2import csvimport pyzbar.pyzbar as pyzbar
barcodeData1 = ''
code_set = set()
capture = cv2.VideoCapture(0)
# 摄像头设置
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 1024)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 1024)
capture.set(cv2.CAP_PROP_EXPOSURE, 0.1)
while True:    
    # 读取摄像头中的图像，ok为是否读取成功的判断参数    
    ret, frame = capture.read()    
    # 转为灰度图像    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)    
    barcodes = pyzbar.decode(gray)    
    print(barcodes)    
    for barcode in barcodes:        
        # 提取条形码的边界框的位置        
        # 画出图像中条形码的边界框        
        (x, y, w, h) = barcode.rect        
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 255, 0), 2)        
        # 条形码数据为字节对象，所以如果我们想在输出图像上        
        # 画出来，就需要先将它转换成字符串        
        barcodeData = barcode.data.decode("utf-8")        
        barcodeType = barcode.type        
        # print(barcodeData)        
        # 判断多次扫描一个条形码，只打印一次        
        if barcodeData == '' or barcodeData != barcodeData1:            
            barcodeData1 = barcodeData            
            print("Recognize result>>> type： {0}  content： {1}".format(barcodeType, barcodeData))        
        else:            
            pass        
        # 避免重复读取       
        if barcodeData not in code_set:            
            with open('shopping.csv', 'r') as rs:                
                reader = csv.reader(rs)                
                # 遍历超市库存文件                
                for line in reader:                     
                    if barcodeData in line:  # 说明超市有这个商品                        
                        print(f'本超市存在此商品,名称：{line[1]},价格:{line[3]}')                        
                        break                    
                     else:                        
                        pass        
        else:            
            pass        
        code_set.add(barcodeData)    
        cv2.imshow('qrcode+barcode', frame)    
        k = cv2.waitKey(1)    
        if k == 27:        
            breakcv2.destroyAllWindows()
```
