PythonOpenCV<br />视频的每一帧就是一张图片，跟踪视频中的某一对象，分解下来，其实就是在每一帧的图片中找到那个对象。<br />既然要找到那个对象，就要先定义这个目标对象，既然是图片，那就有颜色，先了解下常用的 3 种颜色模型。<br />RGB。RGB 就是三原色光模式，又称 RGB 颜色模型或红绿蓝颜色模型，是一种加色模型，将红、绿、蓝三原色的色光以不同的比例相加，以合成产生各种色彩光。每一个像素点都有都有 3 个值表示颜色，这是最常见的颜色模型了。OpenCV 中的顺序是 BGR。<br />灰度图。简单的理解，就是黑白图，图像的每个像素点 只能有 一个值 表示颜色，像素值范围是 [0~255]，现有的成熟分析算法多是基于灰度图像。<br />HSV。这个模型中颜色的参数分别是色调（H）、饱和度（S）和明度（V）.HSV对用户来说是一种直观的颜色模型。可以从一种纯色彩开始，即指定色调H，并让V=S=1，然后可以通过向其中加入黑色和白色来得到需要的颜色。<br />以上三个模型，使用 HSV 定义一个目标对象更为方便。<br />OpenCV 可以很方便的从 BGR 转换为灰度图，从 BGR 转换为 HSV：
```python
import cv2 as cv
cv.cvtColor(input_image,  cv.COLOR_BGR2GRAY)
cv.cvtColor(input_image,  cv.COLOR_BGR2HSV)
```
现在知道如何将 BGR 图像转换为 HSV，可以使用它来提取彩色对象。在 HSV 中，表示颜色比在 BGR 颜色空间中更容易。<br />接下来，将在视频中追踪蓝色对象。<br />步骤：

1. 拍摄视频的每一帧
2. 从 BGR 转换为 HSV 颜色空间
3. 将 HSV 图像阈值设置为蓝色范围
4. 单独提取蓝色对象，可以在该图像上做任何想做的事情。

下面是详细注释的代码：
```python
import cv2 as cv
import numpy as np

# 从默认的摄像头读取视频
cap = cv.VideoCapture(0)
while True:
    # 获取视频的每一帧
    _, frame = cap.read()
    # BGR 转换为 HSV
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    # 用 HSV 定义目标对象的颜色范围。
    lower_blue = np.array([110,50,50])
    upper_blue = np.array([130,255,255])
    # 生成一个 HSV 遮罩只获取蓝色对象。
    mask = cv.inRange(hsv, lower_blue, upper_blue)


    #应用
    res = cv.bitwise_and(frame,frame, mask= mask)
    cv.imshow('frame',frame)
    cv.imshow('mask',mask)
    cv.imshow('res',res)
    k = cv.waitKey(5) & 0xFF
    if k == 27:
        break
cv.destroyAllWindows()
```
效果图如下，可以看到有一点噪音，单后面有技术可以消除。<br />![2022-07-21-11-55-37.495649400.jpg](./img/1658375780178-c4917ec4-a98c-463c-b787-b3230905e829.jpeg)
