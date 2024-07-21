# 原创
：  万字长文告诉新手如何学习Python图像处理（上篇完结 四十四） | 「Python」有奖征文

# 万字长文告诉新手如何学习Python图像处理（上篇完结 四十四） | 「Python」有奖征文

该系列文章是讲解Python OpenCV图像处理知识，前期主要讲解图像入门、OpenCV基础用法，中期讲解图像处理的各种算法，包括图像锐化算子、图像增强技术、图像分割等，后期结合深度学习研究图像识别、图像分类应用。希望文章对您有所帮助，如果有不足之处，还请海涵~

这篇文章是图像处理的最后一篇文章，后面我们将进入新的章节。图像处理文章主要讲解的图像处理方法包括图像几何运算、图像量化采样、图像点运算、图像形态学处理、图像增强、图像平滑、图像锐化、图像特效、图像分割、傅里叶变换与霍夫变换、图像分类等。个人感觉如果你是编程初学者、Python初学者或图像处理爱好者，这个系列真心适合你学习，并且这篇文章算是Python图像处理的学习路线，希望您喜欢。

#### 文章目录

万字长文整理，希望对您有所帮助。同时，该部分知识均为作者查阅资料撰写总结，并且开设成了收费专栏，为小宝赚点奶粉钱，感谢您的抬爱。如果有问题随时私聊我，只望您能从这个系列中学到知识，一起加油。代码下载地址（如果喜欢记得star，一定喔）：

---


## 一.Python图像处理绪论

图像处理是通过计算机对图像进行分析以达到所需结果的技术。常见的方法包括图像变换、图像运算、图像增强、图像分割、图像复原、图像分类等，广泛应用于制造业、生物医学、商品防伪、文物修复、图像校验、模式识别、计算机视觉、人工智能、多媒体通信等领域。

随着大数据和人工智能风暴的来临，Python语言也变得越来越火热，其清晰的语法、丰富和强大的功能，让Python迅速运用于各个领域。该系列博客主要通过Python语言来实现各式各样的图像处理算法及案例，有效地辅助读者学习图像处理知识，并运用于自己的科研、工作或学习中。

### 1.数字图像处理

数字图像处理（Digital Image Processing）又称为计算机图像处理（Computer Image Processing），旨在将图像信号转换成数字信号并利用计算机对其进行处理的过程。其运用领域如图所示，涉及通信、生物医学、物理化学、经济等。

数字图像处理最早出现于20世纪50年代，当时的电子计算机已经发展到一定水平，人们开始利用计算机来处理图形和图像信息。数字图像处理作为一门学科大约形成于20世纪60年代初期。早期的图像处理的目的是改善图像的质量，常用的处理方法包括图像增强、复原、编码、压缩等。1979年，无损伤诊断技术获得了诺贝尔奖，说明它对人类作出了划时代的贡献。与此同时，图像处理技术在许多应用领域受到广泛重视并取得重大开拓性成就，包括航空航天、生物医学工程、工业检测、机器人视觉、公安司法、军事制导、文化艺术等领域，使图像处理成为一门引人注目、前景远大的新型学科。

随着图像处理技术的深入发展，从70年代中期开始，计算机技术和人工智能、思维科学研究迅速发展，数字图像处理向更高、更深层次发展。人们已开始研究如何用计算机系统解释图像，实现类似人类视觉系统理解外部世界，这被称为图像理解或计算机视觉。现如今，这项研究取得了不少重要的研究成果。数字图像处理在国民经济的许多领域已经得到广泛的应用。

---


### 2.Python

随着大数据、深度学习、人工智能、图像识别的迅速发展，图像处理和计算机视觉也变得越来越热门。由于Python具有语法清晰、代码友好、易读性好等特点，同时拥有强大的第三方库支持，包括网络爬取、信息传输、数据分析、图像处理、绘图可视化等库函数，该系列文章选择了Python作为图像处理的编程语言。

Python是Guido van Rossum在1989年开发的一门语言，它既是解释性编程语言，又是面向对象的语言，其操作性和可移植性高，被广泛应用于数据挖掘、图像处理、人工智能领域。Python具有语言清晰、容易学习、高效率的数据结构、丰富且功能强大的第三方包等优势，使其成为一种能在多种功能、多种平台上撰写脚本及快速开发的理想语言。

该系列文章主要通过Python调用OpenCV、Matplotlib、Numpy、Sklearn等第三方包实现图像处理，其优雅清晰的语法结构减少了读者的负担，从而大大增强程序的质量。

---


### 3.OpenCV

OpenCV（Open Source Computer Vision）直译为“开源计算机视觉库”，它是一个开放源代码的图像及视频分析库，是进行图像处理的一款必备工具。自1999年问世以来，它已经被图像处理和计算机视觉领域的学者和开发人员视为首选工具。OpenCV可以运行在Linux、Windows、Android和Mac 操作系统上。它是一个由C/C++语言编写而成的轻量级并且高效的库，同时提供了Python、Ruby、MATLAB等语言的接口，实现了图像处理和计算机视觉方面的很多通用算法。其官方地址为：

下图是OpenCV的Logo图，其设计目标是执行速度更快，更加关注实时应用。采用优化的C/C++代码编写而成，能够充分利用多核处理器的优势，构建一个简单易用的计算机视觉框架。OpenCV被广泛应用于产品检测、医学成像、立体视觉、图像识别、图像增强、图像恢复等领域。本书主要通过Python语言结合OpenCV库实现图像处理相关的算法及案例，并强化读者的印象。

---


### 4.内容分布

本系列为满足广泛的读者需求，结合Python语言实现各种图像处理，主要包括四部分内容，如图所示。

博客划分：

---


## 二.基础知识

Python图像处理第一部分为“基础知识”，主要包括Python基础知识、数字图像处理、Python图像处理入门。

### 1.Python基础

<mark>核心内容：</mark>

---


### 2.数字图像处理基础

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 数字图像处理技术广泛应用于各行各业，它主要是将现实物体离散化处理后转换为信号数字图像，从而更好地进行后续的图像处理和图像识别等操作。下图展示了图像信号数字化处理的过程。

下图展示了图像中某一点像素（205,89,68）所对应三原色像的素值，其中R表示红色分量、G表示绿色分量、B表示蓝色分量。

下面介绍OpenCV中常见的数据类型，包括点Point类、颜色Scalar类、尺寸Size类、矩形Rect类、矩阵Mat类，如下表所示。

<mark>经典示例：</mark><br/> **(1) 显示多张图像**<br/> 在OpenCV中，主要调用Matplotlib绘制显示多张图形，从而方便实验对比，如下代码所示。它调用cv2.imread()函数分别读取四张图片，并转换为RGB颜色空间，接着通过for循环分别设置各子图对应的图像、标题及坐标轴名称，其中plt.subplot(2,2)表示生成2×2张子图。

```
# -*- coding: utf-8 -*-
# By:Eastmount CSDN
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取图像
img1 = cv2.imread('lena.png')
img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2RGB)

img2 = cv2.imread('people.png')
img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2RGB)

img3 = cv2.imread('flower.png')
img3 = cv2.cvtColor(img3, cv2.COLOR_BGR2RGB)

img4 = cv2.imread('scenery.png')
img4 = cv2.cvtColor(img4, cv2.COLOR_BGR2RGB)

#显示四张图像
titles = ['lena', 'people', 'flower', 'scenery']
images = [img1, img2, img3, img4]
for i in range(4):
   plt.subplot(2, 2, i+1), plt.imshow(images[i], 'gray')
   plt.title(titles[i])
   plt.xticks([]),plt.yticks([])
plt.show()

```

显示效果如下图所示：

**(2) 几何图形绘制**

```
# -*- coding: utf-8 -*-
# By:Eastmount CSDN
import cv2
import numpy as np

#创建黑色图像
img = np.zeros((512,512,3), np.uint8)

#(1)绘制直线：图像、起点坐标、终点坐标、颜色、粗细
cv2.line(img, (0,0), (512,512), (55,255,155), 5)

#(2)绘制矩形：图像、左上角坐标、右下角坐标、颜色、粗细
cv2.rectangle(img, (20,300), (350,450), (255,0,0), 2)

#(3)绘制圆形：图像、圆心坐标、半径、颜色、粗细\填充
cv2.circle(img, (400,400), 50, (255,255,0), -1)

#(4)绘制椭圆：图像、圆心坐标、长轴和短轴、偏转角度20
# 圆弧起始角的角度、圆弧终结角的角度、颜色、线条粗细
cv2.ellipse(img, (320, 100), (100, 50), 20, 0, 360, (255, 0, 255), 2)

#(5)绘制多边形：图像、多边形曲线阵列、是否闭合、颜色、粗细
pts = np.array([[10,80], [120,80], [120,200], [30,250]])
cv2.polylines(img, [pts], True, (255, 255, 255), 5)

#(6)绘制多边形
pts = np.array([[50, 190], [380, 420], [255, 50], [120, 420], [450, 190]])
cv2.polylines(img, [pts], True, (0, 255, 255), 1)

#(7)绘制文字
font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img, 'I love Python!!! By:Eatmount CSDN',
            (10, 500), font, 0.6, (255, 255, 0), 2)

#显示图像
cv2.imshow("img", img)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()

```

运行结果如下图所示：

---


### 3.Python图像处理入门

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark>

OpenCV读取的彩色图像由蓝色（B）、绿色（G）、红色（R）三原色组成，每一种颜色可以认为是一个通道分量，如图所示。<br/> <img alt="在这里插入图片描述" height="250" src="https://img-blog.csdnimg.cn/419857c5201044f785d10824b4c88ba5.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="650"/>

图像的算法运算与逻辑运算，主要包括图像加法、图像减法、图像与运算、图像或运算、图像异或运算与图像非运算。

<mark>经典示例：</mark><br/> **(1) 图片拆分与合并**

```
# -*- coding:utf-8 -*-
# By: Eastmount CSDN 2021-08-19
import cv2
import numpy as np

#读取图片
img = cv2.imread("Lena.png")
rows, cols, chn = img.shape
print(rows, cols, chn)
cv2.imshow("img", img)

#(1)拆分通道
b, g, r = cv2.split(img)
cv2.imshow("B", b)
cv2.imshow("G", g)
cv2.imshow("R", r)

#(2)合并通道
m = cv2.merge([b, g, r])
cv2.imshow("Merge", m)

#(3)显示蓝色通道
b_ = cv2.split(img)[0]
g_ = np.zeros((rows,cols), dtype=img.dtype) #设置g、r通道为0
r_ = np.zeros((rows,cols), dtype=img.dtype)
m = cv2.merge([b_, g_, r_])
cv2.imshow("MergeBlue", m)
cv2.imwrite("MergeBlue.png", m)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()

```

输出结果如下图所示：

**(2) 图像算数与逻辑运算**

```
# -*- coding:utf-8 -*-
# By: Eastmount CSDN 2021-08-19
import cv2  
import numpy as np
import matplotlib.pyplot as plt
 
#读取图片
src = cv2.imread("Lena.png")

#获取图像宽和高
rows, cols = src.shape[:2]

#BGR转换为RGB
img = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)

#(1)OpenCV加法运算
m = np.ones(img.shape, dtype="uint8")*100 #图像各像素加100
result1 = cv2.add(img, m)

#(2)OpenCV减法运算
m = np.ones(img.shape, dtype="uint8")*50  #图像各像素减50
result2 = cv2.subtract(img, m)

#(3)画圆形
circle = np.zeros((rows, cols, 3), dtype="uint8")
cv2.circle(circle, (int(rows/2.0), int(cols/2)), 100, (255,0,0), -1)
print(circle.shape,img.size, circle.size)

#(4)OpenCV图像与运算
result4 = cv2.bitwise_and(img, circle)

#(5)OpenCV图像或运算
result5 = cv2.bitwise_or(img, circle)

#(6)OpenCV图像异或运算
result6 = cv2.bitwise_xor(img, circle)

#(7)OpenCV图像非运算
result7 = cv2.bitwise_not(img)

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['KaiTi']  #指定默认字体
plt.rcParams['axes.unicode_minus'] = False   #解决保存图像是负号

#显示九张图像
titles = ['原图', 'RGB', '加法', '减法', '圆形', '与运算', '或运算', '异或运算', '非运算']
images = [src, img, result1, result2, circle, result4, result5, result6, result7]
for i in range(9):
   plt.subplot(3, 3, i+1), plt.imshow(images[i], 'gray')
   plt.title(titles[i])
   plt.xticks([]),plt.yticks([])
plt.show()

```

输出结果如下图所示：

**(3) 图像融合处理与获取图像ROI区域**

```
#encoding:utf-8
# By: Eastmount CSDN 2021-08-19
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
src1 = cv2.imread('lena-hd.png')
src2 = cv2.imread('na.png')
src3 = cv2.imread("Lena.png")
print(src3.shape)

#图像融合
res1 = cv2.addWeighted(src1, 0.6, src2, 0.8, 10)

#显示ROI区域
face = np.ones((100, 100, 3)) #定义150×150矩阵 3对应BGR
face = src3[200:300, 180:280]

#显示ROI区域
res3 = src2.copy()
res3[150:250, 150:250] = face

#显示图像
cv2.imshow("src1", src1)
cv2.imshow("src2", src2)
cv2.imshow("src3", src3)
cv2.imshow("res1", res1)
cv2.imshow("res2", face)
cv2.imshow("res3", res3)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()

```

输出结果如下图所示：

**(4) 图像类型转换**

```
# -*- coding: utf-8 -*-
# By: Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取原始图像
img_BGR = cv2.imread('na.png')

#BGR转换为RGB
img_RGB = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2RGB)

#灰度化处理
img_GRAY = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2GRAY)

#BGR转HSV
img_HSV = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2HSV)

#BGR转YCrCb
img_YCrCb = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2YCrCb)

#BGR转HLS
img_HLS = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2HLS)

#BGR转XYZ
img_XYZ = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2XYZ)

#BGR转LAB
img_LAB = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2LAB)

#BGR转YUV
img_YUV = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2YUV)

#调用matplotlib显示处理结果
titles = ['BGR', 'RGB', 'GRAY', 'HSV', 'YCrCb', 'HLS', 'XYZ', 'LAB', 'YUV']  
images = [img_BGR, img_RGB, img_GRAY, img_HSV, img_YCrCb,
          img_HLS, img_XYZ, img_LAB, img_YUV]  
for i in range(9):  
   plt.subplot(3, 3, i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()

```

显示结果如下图所示：

---


## 三.图像运算

Python图像处理第二部分为“图像运算”，主要包括Python图像几何变换、Python图像量化采样、Python图像点运算和Python图像形态学。

### 1.Python图像几何变换

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 一个几何变换需要两部分运算：首先是空间变换所需的运算，如平移、缩放、旋转和正平行投影等， 需要用它来表示输出图像与输入图像之间的像素映射关系；此外，还需要使用灰度插值算法， 因为按照这种变换关系进行计算， 输出图像的像素可能被映射到输入图像的非整数坐标上。

图像平移是将图像中的所有像素点按照给定的平移量进行水平或垂直方向上的移动。假设原始像素的位置坐标为（x0，y0），经过平移量（△x，△y）后，坐标变为（x1, y1），如图所示。

图像旋转是指图像以某一点为中心旋转一定的角度，形成一幅新的图像的过程。图像旋转变换会有一个旋转中心，这个旋转中心一般为图像的中心，旋转之后图像的大小一般会发生改变。图8表示原始图像的坐标(x0, y0)旋转至(x1, y1)的过程。

图像镜像变换是图像旋转变换的一种特殊情况，通常包括垂直方向和水平方向的镜像。水平镜像通常是以原图像的垂直中轴为中心，将图像分为左右两部分进行堆成变换。如图所示：

垂直镜像通常是以原图像的水平中轴线为中心，将图像划分为上下两部分进行堆成变换的过程，示意图如图所示。

<mark>经典示例：</mark>

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test.bmp')
image = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

#图像平移矩阵
M = np.float32([[1, 0, 100], [0, 1, 80]])
rows, cols = image.shape[:2]
img1 = cv2.warpAffine(image, M, (cols, rows))

#图像缩小
img2 = cv2.resize(image, (200,100))

#图像放大
img3 = cv2.resize(image, None, fx=1.1, fy=1.1)

#绕图像的中心旋转
#源图像的高、宽 以及通道数
rows, cols, channel = image.shape
#函数参数：旋转中心 旋转度数 scale
M = cv2.getRotationMatrix2D((cols/2, rows/2), 30, 1) 
#函数参数：原始图像 旋转参数 元素图像宽高
img4 = cv2.warpAffine(image, M, (cols, rows))

#图像翻转
img5 = cv2.flip(image, 0)   #参数=0以X轴为对称轴翻转 
img6 = cv2.flip(image, 1)   #参数&gt;0以Y轴为对称轴翻转

#图像的仿射
pts1 = np.float32([[50,50],[200,50],[50,200]])
pts2 = np.float32([[10,100],[200,50],[100,250]])
M = cv2.getAffineTransform(pts1,pts2)
img7 = cv2.warpAffine(image, M, (rows,cols))

#图像的透射
pts1 = np.float32([[56,65],[238,52],[28,237],[239,240]])
pts2 = np.float32([[0,0],[200,0],[0,200],[200,200]])
M = cv2.getPerspectiveTransform(pts1,pts2)
img8 = cv2.warpPerspective(image,M,(200,200))


#循环显示图形
titles = [ 'source', 'shift', 'reduction', 'enlarge', 'rotation', 'flipX', 'flipY', 'affine', 'transmission']  
images = [image, img1, img2, img3, img4, img5, img6, img7, img8]  
for i in range(9):  
   plt.subplot(3, 3, i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()  

```

输出结果如下图所示：

---


### 2.Python图像量化采样

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 所谓**量化（Quantization）**，就是将图像像素点对应亮度的连续变化区间转换为单个特定值的过程，即将原始灰度图像的空间坐标幅度值离散化。量化等级越多，图像层次越丰富，灰度分辨率越高，图像的质量也越好；量化等级越少，图像层次欠丰富，灰度分辨率越低，会出现图像轮廓分层的现象，降低了图像的质量。下图是将图像的连续灰度值转换为0至255的灰度级的过程。

如果量化等级为2，则将使用两种灰度级表示原始图片的像素（0-255），灰度值小于128的取0，大于等于128的取128；如果量化等级为4，则将使用四种灰度级表示原始图片的像素，新图像将分层为四种颜色，0-64区间取0，64-128区间取64，128-192区间取128，192-255区间取192，依次类推。

下图是对比不同量化等级的“Lena”图。其中（a）的量化等级为256，（b）的量化等级为64，（c）的量化等级为16，（d）的量化等级为8，（e）的量化等级为4，（f）的量化等级为2。

**图像采样（Image Sampling）** 处理是将一幅连续图像在空间上分割成M×N个网格，每个网格用一个亮度值或灰度值来表示，其示意图如图所示。

图像采样的间隔越大，所得图像像素数越少，空间分辨率越低，图像质量越差，甚至出现马赛克效应；相反，图像采样的间隔越小，所得图像像素数越多，空间分辨率越高，图像质量越好，但数据量会相应的增大。下图展示了不同采样间隔的“Lena”图，其中图(a)为原始图像，图(b)为128×128的图像采样效果，图©为64×64的图像采样效果，图(d)为32×32的图像采样效果，图(e)为16×16的图像采样效果，图(f)为8×8的图像采样效果。

图像金字塔是指由一组图像且不同分别率的子图集合，它是图像多尺度表达的一种，以多分辨率来解释图像的结构，主要用于图像的分割或压缩。一幅图像的金字塔是一系列以金字塔形状排列的分辨率逐步降低，且来源于同一张原始图的图像集合。如图所示，它包括了四层图像，将这一层一层的图像比喻成金字塔。图像金字塔可以通过梯次向下采样获得，直到达到某个终止条件才停止采样，在向下采样中，层级越高，则图像越小，分辨率越低。

<mark>经典示例：</mark><br/> **(1)量化对比**<br/> 下面的代码分别比较了量化等级为2、4、8的量化处理效果。

```
# -*- coding: utf-8 -*-
# BY:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread('lena-hd.png')

#获取图像高度和宽度
height = img.shape[0]
width = img.shape[1]

#创建一幅图像
new_img1 = np.zeros((height, width, 3), np.uint8)
new_img2 = np.zeros((height, width, 3), np.uint8)
new_img3 = np.zeros((height, width, 3), np.uint8)

#图像量化等级为2的量化处理
for i in range(height):
    for j in range(width):
        for k in range(3): #对应BGR三分量
            if img[i, j][k] &lt; 128:
                gray = 0
            else:
                gray = 128
            new_img1[i, j][k] = np.uint8(gray)

#图像量化等级为4的量化处理
for i in range(height):
    for j in range(width):
        for k in range(3): #对应BGR三分量
            if img[i, j][k] &lt; 64:
                gray = 0
            elif img[i, j][k] &lt; 128:
                gray = 64
            elif img[i, j][k] &lt; 192:
                gray = 128
            else:
                gray = 192
            new_img2[i, j][k] = np.uint8(gray)

#图像量化等级为8的量化处理
for i in range(height):
    for j in range(width):
        for k in range(3): #对应BGR三分量
            if img[i, j][k] &lt; 32:
                gray = 0
            elif img[i, j][k] &lt; 64:
                gray = 32
            elif img[i, j][k] &lt; 96:
                gray = 64
            elif img[i, j][k] &lt; 128:
                gray = 96
            elif img[i, j][k] &lt; 160:
                gray = 128
            elif img[i, j][k] &lt; 192:
                gray = 160
            elif img[i, j][k] &lt; 224:
                gray = 192
            else:
                gray = 224
            new_img3[i, j][k] = np.uint8(gray)

#用来正常显示中文标签
plt.rcParams['font.sans-serif']=['SimHei']

#显示图像
titles = ['(a) 原始图像', '(b) 量化-L2', '(c) 量化-L4', '(d) 量化-L8']  
images = [img, new_img1, new_img2, new_img3]  
for i in range(4):  
   plt.subplot(2,2,i+1), plt.imshow(images[i], 'gray'), 
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()

```

输出结果如下图所示：

**(2)KMeans量化**

```
# coding: utf-8
# BY:Eastmount CSDN 2021-08-20
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread('nv.png') 

#图像二维像素转换为一维
data = img.reshape((-1,3))
data = np.float32(data)

#定义中心 (type,max_iter,epsilon)
criteria = (cv2.TERM_CRITERIA_EPS +
            cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)

#设置标签
flags = cv2.KMEANS_RANDOM_CENTERS

#K-Means聚类 聚集成4类
compactness, labels, centers = cv2.kmeans(data, 4, None, criteria, 10, flags)


#图像转换回uint8二维类型
centers = np.uint8(centers)
res = centers[labels.flatten()]
dst = res.reshape((img.shape))

#图像转换为RGB显示
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
dst = cv2.cvtColor(dst, cv2.COLOR_BGR2RGB)

#用来正常显示中文标签
plt.rcParams['font.sans-serif']=['SimHei']

#显示图像
titles = ['原始图像', '聚类量化 K=4']  
images = [img, dst]  
for i in range(2):  
   plt.subplot(1,2,i+1), plt.imshow(images[i], 'gray'), 
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()

```

输出结果如下图所示：

**(3)彩色图像采样**

```
# -*- coding: utf-8 -*-
# BY:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread('scenery.png')

#获取图像高度和宽度
height = img.shape[0]
width = img.shape[1]

#采样转换成8*8区域
numHeight = int(height/8)
numwidth = int(width/8)

#创建一幅图像
new_img = np.zeros((height, width, 3), np.uint8)

#图像循环采样8*8区域
for i in range(8):
    #获取Y坐标
    y = i*numHeight
    for j in range(8):
        #获取X坐标
        x = j*numwidth
        #获取填充颜色 左上角像素点
        b = img[y, x][0]
        g = img[y, x][1]
        r = img[y, x][2]
        
        #循环设置小区域采样
        for n in range(numHeight):
            for m in range(numwidth):
                new_img[y+n, x+m][0] = np.uint8(b)
                new_img[y+n, x+m][1] = np.uint8(g)
                new_img[y+n, x+m][2] = np.uint8(r)
        
#显示图像
cv2.imshow("src", img)
cv2.imshow("Sampling", new_img)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()

```

输出结果如下图所示：

---


### 3.Python图像点运算

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像点运算（Point Operation）指对于一幅输入图像，将产生一幅输出图像，输出图像的每个像素点的灰度值由输入像素点决定。点运算实际上是灰度到灰度的映射过程，通过映射变换来达到增强或者减弱图像的灰度。还可以对图像进行求灰度直方图、线性变换、非线性变换以及图像骨架的提取。它与相邻的像素之间没有运算关系，是一种简单和有效的图像处理方法。

**图像灰度化**是将一幅彩色图像转换为灰度化图像的过程。彩色图像通常包括R、G、B三个分量，分别显示出红绿蓝等各种颜色，灰度化就是使彩色图像的R、G、B三个分量相等的过程。灰度图像中每个像素仅具有一种样本颜色，其灰度是位于黑色与白色之间的多级色彩深度，灰度值大的像素点比较亮，反之比较暗，像素值最大为255（表示白色），像素值最小为0（表示黑色）。

假设某点的颜色由RGB(R,G,B)组成，常见灰度处理算法如表所示：

**图像阈值化（Binarization）** 旨在剔除掉图像中一些低于或高于一定值的像素，从而提取图像中的物体，将图像的背景和噪声区分开来。图像阈值化可以理解为一个简单的图像分割操作，阈值又称为临界值，它的目的是确定出一个范围，然后这个范围内的像素点使用同一种方法处理，而阈值之外的部分则使用另一种处理方法或保持原样。

灰度化处理后的图像中，每个像素都只有一个灰度值，其大小表示明暗程度。阈值化处理可以将图像中的像素划分为两类颜色，常见的阈值化算法如公式所示：

像阈值化处理threshold()函数不同类型的处理算法如表所示。

<mark>经典示例：</mark><br/> **(1)固定阈值变化对比**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取图像
img=cv2.imread('miao.png')
grayImage=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  

#阈值化处理
ret,thresh1=cv2.threshold(grayImage,127,255,cv2.THRESH_BINARY)  
ret,thresh2=cv2.threshold(grayImage,127,255,cv2.THRESH_BINARY_INV)  
ret,thresh3=cv2.threshold(grayImage,127,255,cv2.THRESH_TRUNC)  
ret,thresh4=cv2.threshold(grayImage,127,255,cv2.THRESH_TOZERO)  
ret,thresh5=cv2.threshold(grayImage,127,255,cv2.THRESH_TOZERO_INV)

#显示结果
titles = ['Gray Image','BINARY','BINARY_INV','TRUNC','TOZERO','TOZERO_INV']  
images = [grayImage, thresh1, thresh2, thresh3, thresh4, thresh5]  
for i in range(6):  
   plt.subplot(2,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()

```

输出结果如下图所示：

**(2)灰度变化对比**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2
import math
import numpy as np  
import matplotlib.pyplot as plt

#读取图像
img = cv2.imread('miao.png')

#图像灰度转换
grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#获取图像高度和宽度
height = grayImage.shape[0]
width = grayImage.shape[1]

#图像灰度上移变换 DB=DA+50
result1 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        if (int(grayImage[i,j]+50) &gt; 255):
            gray = 255
        else:
            gray = int(grayImage[i,j]+50)
        result1[i,j] = np.uint8(gray)

#图像对比度增强变换 DB=DA*1.5
result2 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        if (int(grayImage[i,j]*1.5) &gt; 255):
            gray = 255
        else:
            gray = int(grayImage[i,j]*1.5)
        result2[i,j] = np.uint8(gray)

#图像对比度增强变换 DB=DA*0.8
result3 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        if (int(grayImage[i,j]*0.8) &gt; 255):
            gray = 255
        else:
            gray = int(grayImage[i,j]*0.8)
        result3[i,j] = np.uint8(gray)

#图像灰度反色变换 DB=255-DA
result4 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        gray = 255 - grayImage[i,j]
        result4[i,j] = np.uint8(gray)

#图像灰度非线性变换：DB=DA×DA/255
result5 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        gray = int(grayImage[i,j])*int(grayImage[i,j]) / 255
        result5[i,j] = np.uint8(gray)   

#图像灰度对数变换
result6 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        gray = 10* math.log(1 + grayImage[i, j])
        result6[i,j] = np.uint8(gray)

#图像灰度伽玛变换
result7 = np.zeros((height, width), np.uint8)
for i in range(height):
    for j in range(width):
        gray = 3*pow(grayImage[i, j], 0.8)
        if gray&gt;255:
            gray = 255
        result7[i,j] = np.uint8(gray)

#用来正常显示中文标签
plt.rcParams['font.sans-serif']=['SimHei']

#显示结果
titles = [u'原始图像', u'灰度图像', u'图像拉伸', u'对比度增强', u'对比度减弱',
          u'图像反色', u'非线性变换', u'对数变换', u'伽马变换']  
images = [img, grayImage, result1, result2, result3, result4, result5, result6, result7]  
for i in range(9):  
   plt.subplot(3,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()

```

输出结果如下图所示：

**(3)图像灰度伽玛变换去雾**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import numpy as np
import matplotlib.pyplot as plt
import cv2

#绘制曲线
def gamma_plot(c, v):
    x = np.arange(0, 256, 0.01)
    y = c*x**v
    plt.plot(x, y, 'r', linewidth=1)
    plt.rcParams['font.sans-serif']=['SimHei'] #正常显示中文标签
    plt.title('伽马变换函数')
    plt.xlim([0, 255]), plt.ylim([0, 255])
    plt.show()

#伽玛变换
def gamma(img, c, v):
    lut = np.zeros(256, dtype=np.float32)
    for i in range(256):
        lut[i] = c * i ** v
    output_img = cv2.LUT(img, lut) #像素灰度值的映射
    output_img = np.uint8(output_img+0.5)  
    return output_img

#读取原始图像
img = cv2.imread('2019.png')

#绘制伽玛变换曲线
gamma_plot(0.00000005, 4.0)

#图像灰度伽玛变换
output = gamma(img, 0.00000005, 4.0)

#显示图像
cv2.imshow('Imput', img)
cv2.imshow('Output', output)
cv2.waitKey(0)
cv2.destroyAllWindows()

```

输出结果如下图所示：

---


### 4.Python图像形态学

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 数学形态学是一门建立在格论和拓扑学基础之上的图像分析学科，是数学形态学图像处理的基本理论。其基本的运算包括：

常见的图像形态学运算包括腐蚀、膨胀、开运算、闭运算、梯度运算、顶帽运算和底帽运算等。主要通过MorphologyEx()函数实现，它能利用基本的膨胀和腐蚀技术，来执行更加高级形态学变换，如开闭运算、形态学梯度、顶帽、黑帽等，也可以实现最基本的图像膨胀和腐蚀。其函数原型如下：

图像的腐蚀（Erosion）和膨胀（Dilation）是两种基本的形态学运算，主要用来寻找图像中的极小区域和极大区域。图像腐蚀类似于“领域被蚕食”，它将图像中的高亮区域或白色部分进行缩减细化，其运行结果比原图的高亮区域更小。

设A，B为集合，A被B的腐蚀，记为A－B，其定义为：

该公式表示图像A用卷积模板B来进行腐蚀处理，通过模板B与图像A进行卷积计算，得出B覆盖区域的像素点最小值，并用这个最小值来替代参考点的像素值。如图1所示，将左边的原始图像A腐蚀处理为右边的效果图A-B。

图像膨胀处理效果如下图所示：

图像开运算是图像依次经过腐蚀、膨胀处理的过程，图像被腐蚀后将去除噪声，但同时也压缩了图像，接着对腐蚀过的图像进行膨胀处理，可以在保留原有图像的基础上去除噪声。其原理如图所示。

<mark>经典案例:</mark>

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np
import matplotlib.pyplot as plt

#读取图片
src = cv2.imread('na.png', cv2.IMREAD_UNCHANGED)
img = cv2.cvtColor(src,cv2.COLOR_BGR2RGB)

# 转化为灰度图
Grayimg = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

# 1、消除椒盐噪声：
# 中值滤波器
median = cv2.medianBlur(Grayimg, 5)
# 消除噪声图
cv2.imshow("median-image", median)

# 2、直方图均衡化：
equalize = cv2.equalizeHist(median)
cv2.imshow('hist', equalize)

# 3、二值化处理：
# 阈值为140
ret, binary = cv2.threshold(equalize, 127, 255,cv2.THRESH_BINARY)
cv2.imshow("binary-image",binary)
cv2.waitKey(0)

#设置卷积核
kernel = np.ones((10,10), np.uint8)
close = cv2.morphologyEx(binary, cv2.MORPH_CLOSE, kernel)

#图像开运算
kernel = np.ones((10,10), np.uint8) 
open1 = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", close)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()

#图像开运算
kernel = np.ones((10,10), np.uint8) 
gradient = cv2.morphologyEx(binary, cv2.MORPH_GRADIENT, kernel)

# Sobel算子 XY方向求梯度 cv2.CV_8U
x = cv2.Sobel(close, cv2.CV_32F, 1, 0, ksize = 3) #X方向
y = cv2.Sobel(close, cv2.CV_32F, 0, 1, ksize = 3) #Y方向
#absX = cv2.convertScaleAbs(x)   # 转回uint8    
#absY = cv2.convertScaleAbs(y)
#Sobel = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)
gradient = cv2.subtract(x, y)
sobel = cv2.convertScaleAbs(gradient)
cv2.imshow('Sobel', sobel)
cv2.waitKey(0)

#循环显示图形
titles = [ 'source', 'gray', 'median', 'equalize', 'binary', 'close', 'open', 'gradient', 'sobel']  
images = [img, Grayimg, median, equalize, binary, close, open1, gradient, sobel]  
for i in range(9):  
   plt.subplot(3, 3, i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()  

```

输出结果如下图所示：

---


## 四.图像增强

Python图像处理第三部分为“图像增强”，主要包括Python直方图统计、Python图像增强、Python图像平滑和Python图像锐化及边缘检测。

### 1.Python直方图统计

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像灰度直方图（Histogram）是灰度级分布的函数，是对图像中灰度级分布的统计。灰度直方图是将数字图像中的所有像素，按照灰度值的大小，统计其出现的频率并绘制相关图形。

如果灰度级为0-255（最小值0为黑色，最大值255为白色），同样可以绘制对应的直方图，如图2所示，左边是一幅灰度图像（Lena灰度图），右边是对应各像素点的灰度级频率。

<mark>经典示例：</mark><br/> **(1)Matplotlib绘制直方图**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np
import matplotlib.pyplot as plt
import matplotlib

#读取图像
src = cv2.imread('Lena.png')

#转换为RGB图像
img_rgb = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)

#获取BGR三个通道的像素值
b, g, r = cv2.split(src)
print(r,g,b)

plt.figure(figsize=(8, 6))

#设置字体
matplotlib.rcParams['font.sans-serif']=['SimHei']

#原始图像
plt.subplot(221)
plt.imshow(img_rgb)
plt.axis('off')
plt.title("(a)原图像")

#绘制蓝色分量直方图
plt.subplot(222)
plt.hist(b.ravel(), bins=256, density=1, facecolor='b', edgecolor='b', alpha=0.75)
plt.xlabel("x")
plt.ylabel("y")
plt.title("(b)蓝色分量直方图")

#绘制绿色分量直方图
plt.subplot(223)
plt.hist(g.ravel(), bins=256, density=1, facecolor='g', edgecolor='g', alpha=0.75)
plt.xlabel("x")
plt.ylabel("y")
plt.title("(c)绿色分量直方图")

#绘制红色分量直方图
plt.subplot(224)
plt.hist(r.ravel(), bins=256, density=1, facecolor='r', edgecolor='r', alpha=0.75)
plt.xlabel("x")
plt.ylabel("y")
plt.title("(d)红色分量直方图")
plt.show()

```

输出结果如下图所示：

**(2)OpenCV绘制直方图**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np
import matplotlib.pyplot as plt
import matplotlib

#读取图像
src = cv2.imread('Lena.png')

#转换为RGB图像
img_rgb = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)

#计算直方图
histb = cv2.calcHist([src], [0], None, [256], [0,255])
histg = cv2.calcHist([src], [1], None, [256], [0,255])
histr = cv2.calcHist([src], [2], None, [256], [0,255])

#设置字体
matplotlib.rcParams['font.sans-serif']=['SimHei']

#显示原始图像和绘制的直方图
plt.subplot(121)
plt.imshow(img_rgb, 'gray')
plt.axis('off')
plt.title("(a)Lena原始图像")

plt.subplot(122)
plt.plot(histb, color='b')
plt.plot(histg, color='g')
plt.plot(histr, color='r')
plt.xlabel("x")
plt.ylabel("y")
plt.title("(b)直方图曲线")
plt.show()

```

输出结果如下图所示：

**(3)灰度变换直方图对比**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np
import matplotlib.pyplot as plt

#读取图像
img = cv2.imread('lena-hd.png')

#图像灰度转换
grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#获取图像高度和宽度
height = grayImage.shape[0]
width = grayImage.shape[1]
result = np.zeros((height, width), np.uint8)

#图像灰度对数变换
for i in range(height):
    for j in range(width):
        gray = 42 * np.log(1.0 + grayImage[i,j])
        result[i,j] = np.uint8(gray)

#计算原图的直方图
hist = cv2.calcHist([img], [0], None, [256], [0,255])

#计算灰度变换的直方图
hist_res = cv2.calcHist([result], [0], None, [256], [0,255])

#原始图像
plt.figure(figsize=(8, 6))
plt.subplot(221), plt.imshow(img, 'gray'), plt.title("(a)"), plt.axis('off')

#绘制原始图像直方图
plt.subplot(222), plt.plot(hist), plt.title("(b)"), plt.xlabel("x"), plt.ylabel("y")

#灰度变换后的图像
plt.subplot(223), plt.imshow(result, 'gray'), plt.title("(c)"), plt.axis('off')

#灰度变换图像的直方图
plt.subplot(224), plt.plot(hist_res), plt.title("(d)"), plt.xlabel("x"), plt.ylabel("y")
plt.show()

```

输出结果如下图所示：

---


### 2.Python图像增强

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像增强（Image Enhancement）是指按照某种特定的需求，突出图像中有用的信息，去除或者削弱无用的信息。图像增强的目的是使处理后的图像更适合人眼的视觉特性或易于机器识别。图像增强通常划分为如图2所示的分类，其中最重要的是图像平滑和图像锐化处理。

<img alt="在这里插入图片描述" height="400" src="https://img-blog.csdnimg.cn/24b442bc5eed42efaa98f47aa5f8945e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="550"/><br/> 直方图均衡是图像增强处理中对比度变换调整中最典型的方法。该方法是空域增强中最常用、最简单有效的方法之一，其采用灰度统计特征，将原始图像中的灰度直方图从较为集中的某个灰度区间转变为均匀分布于整个灰度区域范围的变换方法。通常又划分为：

下图(a)表示直方图均衡化处理后的图像，图(b)表示其对应的直方图。从效果图可以看出，经过直方图均衡化处理，图像变得更加清晰，图像的灰度级分布也更加均匀。

自动色彩均衡（Automatic Color Enhancement，ACE） 算法是在Retinex算法的理论上提出的，它通过计算图像目标像素点和周围像素点的明暗程度及其关系来对最终的像素值进行校正，实现图像的对比度调整，产生类似人体视网膜的色彩恒常性和亮度恒常性的均衡，具有很好的图像增强效果。

ACE算法能有效进行图像去雾处理，实现图像的细节增强。

<mark>经典案例：</mark><br/> **(1)直方图均衡化对比**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('lena-hd.png')

#灰度转换
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 
#直方图均衡化处理
result = cv2.equalizeHist(gray)

#显示图像
plt.subplot(221)
plt.imshow(gray, cmap=plt.cm.gray), plt.axis("off"), plt.title('(a)') 
plt.subplot(222)
plt.imshow(result, cmap=plt.cm.gray), plt.axis("off"), plt.title('(b)') 
plt.subplot(223)
plt.hist(img.ravel(), 256), plt.title('(c)') 
plt.subplot(224)
plt.hist(result.ravel(), 256), plt.title('(d)') 
plt.show()

```

输出结果如图所示，图(a)为原始图像，对应的直方图为图©，图(b)和图(d)为直方图处理后的图像及对应直方图，它让图像的灰度值分布更加均衡。

**(2)局部直方图均衡化**

```
#encoding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('lena.png')

#灰度转换
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 
#局部直方图均衡化处理
clahe = cv2.createCLAHE(clipLimit=2, tileGridSize=(10,10))

#将灰度图像和局部直方图相关联, 把直方图均衡化应用到灰度图 
result = clahe.apply(gray)

#显示图像
plt.subplot(221)
plt.imshow(gray, cmap=plt.cm.gray), plt.axis("off"), plt.title('(a)') 
plt.subplot(222)
plt.imshow(result, cmap=plt.cm.gray), plt.axis("off"), plt.title('(b)') 
plt.subplot(223)
plt.hist(img.ravel(), 256), plt.title('(c)') 
plt.subplot(224)
plt.hist(result.ravel(), 256), plt.title('(d)') 
plt.show()

```

输出结果如下图所示：

---


### 3.Python图像平滑

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像平滑（smoothing）是一项简单且使用频率很高的图像处理方法，可以用来压制、弱化或消除图像中的细节、突变、边缘和噪声，最常见的是用来减少图像上的噪声。

何为图像噪声？噪声是妨碍人的感觉器官所接受信源信息理解的因素，是不可预测只能用概率统计方法认识的随机误差。从图1中，可以观察到噪声的特点：位置随机、大小不规则，将这种噪声称为随机噪声，这是一种常见的噪声类型。

一幅图像不可避免地要受到各种噪声源的干扰，所以噪声滤除往往是图像处理中的第一步，滤波效果好坏将直接影响后续处理结果，噪声滤除在图像处理中占有相当重要的地位。噪声滤除算法多种多样，可以从设计方法上分为两大类：

后文将详细介绍以下常用的一些滤波器，包括均值滤波、方框滤波、高斯吕波、中值滤波等，如表所示。

下图为这五种滤波的效果对比，从滤波的结果可以看出各种滤波算法对图像的作用非常不同，有些变化非常大，有些甚至跟原图一样。在实际应用时，应根据噪声的特点、期望的图像和边缘特征等来选择合适的滤波器，这样才能发挥图像滤波的最大优点。

<mark>经典示例：</mark>

```
# -*- coding: utf-8 -*-
# By：Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('te.png')
source = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
 
#均值滤波
result1 = cv2.blur(source, (5,5))
result2 = cv2.blur(source, (10,10))

#方框滤波
result3 = cv2.boxFilter(source, -1, (5,5), normalize=1)
result4 = cv2.boxFilter(source, -1, (2,2), normalize=0)

#高斯滤波
result5 = cv2.GaussianBlur(source, (3,3), 0)
result6 = cv2.GaussianBlur(source, (15,15), 0)

#中值滤波
result7 = cv2.medianBlur(source, 3)

#高斯双边滤波
result8 =cv2.bilateralFilter(source, 15, 150, 150)

#显示图形
titles = ['Source', 'Blur 5*5', 'Blur 10*10', 'BoxFilter 5*5',
          'BoxFilter 2*2', 'GaussianBlur 3*3', 'GaussianBlur 15*15',
          'medianBlur', 'bilateralFilter']  
images = [source, result1, result2, result3,
          result4, result5, result6, result7, result8]  
for i in range(9):  
   plt.subplot(3,3,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()  

```

输出结果如下图所示：

---


### 4.Python图像锐化

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 由于收集图像数据的器件或传输图像的通道存在一些质量缺陷，或者受其他外界因素的影响，使得图像存在模糊和有噪声的情况，从而影响到图像识别工作的开展。一般来说，图像的能量主要集中在其低频部分，噪声所在的频段主要在高频段，同时图像边缘信息主要集中在其高频部分。这将导致原始图像在平滑处理之后，图像边缘和图像轮廓模糊的情况出现。为了减少这类不利效果的影响，就需要利用图像锐化技术，使图像的边缘变得清晰。

图像锐化处理的目的是为了使图像的边缘、轮廓线以及图像的细节变得清晰，经过平滑的图像变得模糊的根本原因是图像受到了平均或积分运算，因此可以对其进行逆运算，从而使图像变得清晰。

图像锐化和边缘提取技术可以消除图像中的噪声，提取图像信息中用来表征图像的一些变量，为图像识别提供基础。通常使用灰度差分法对图像的边缘、轮廓进行处理，将其凸显。图像锐化的方法分为高通滤波和空域微分法。

<mark>经典案例：</mark>

```
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取图像
img = cv2.imread('nv.png')
lenna_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

#灰度化处理图像
grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#高斯滤波
gaussianBlur = cv2.GaussianBlur(grayImage, (3,3), 0)

#阈值处理
ret, binary = cv2.threshold(gaussianBlur, 127, 255, cv2.THRESH_BINARY)

#Roberts算子
kernelx = np.array([[-1,0],[0,1]], dtype=int)
kernely = np.array([[0,-1],[1,0]], dtype=int)
x = cv2.filter2D(binary, cv2.CV_16S, kernelx)
y = cv2.filter2D(binary, cv2.CV_16S, kernely)
absX = cv2.convertScaleAbs(x)     
absY = cv2.convertScaleAbs(y)    
Roberts = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)

#Prewitt算子
kernelx = np.array([[1,1,1],[0,0,0],[-1,-1,-1]], dtype=int)
kernely = np.array([[-1,0,1],[-1,0,1],[-1,0,1]], dtype=int)
x = cv2.filter2D(binary, cv2.CV_16S, kernelx)
y = cv2.filter2D(binary, cv2.CV_16S, kernely)
absX = cv2.convertScaleAbs(x)  
absY = cv2.convertScaleAbs(y)    
Prewitt = cv2.addWeighted(absX,0.5,absY,0.5,0)

#Sobel算子
x = cv2.Sobel(binary, cv2.CV_16S, 1, 0)
y = cv2.Sobel(binary, cv2.CV_16S, 0, 1)    
absX = cv2.convertScaleAbs(x)   
absY = cv2.convertScaleAbs(y)    
Sobel = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)

#拉普拉斯算法
dst = cv2.Laplacian(binary, cv2.CV_16S, ksize = 3)
Laplacian = cv2.convertScaleAbs(dst)

# Scharr算子
x = cv2.Scharr(gaussianBlur, cv2.CV_32F, 1, 0) #X方向
y = cv2.Scharr(gaussianBlur, cv2.CV_32F, 0, 1) #Y方向
absX = cv2.convertScaleAbs(x)       
absY = cv2.convertScaleAbs(y)
Scharr = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)

#Canny算子
Canny = cv2.Canny(gaussianBlur, 50, 150)

#先通过高斯滤波降噪
gaussian = cv2.GaussianBlur(grayImage, (3,3), 0)
 
#再通过拉普拉斯算子做边缘检测
dst = cv2.Laplacian(gaussian, cv2.CV_16S, ksize = 3)
LOG = cv2.convertScaleAbs(dst)

#效果图
titles = ['Source Image', 'Binary Image', 'Roberts Image',
          'Prewitt Image','Sobel Image', 'Laplacian Image',
          'Scharr Image', 'Canny Image', 'LOG Image']  
images = [lenna_img, binary, Roberts,
          Prewitt, Sobel, Laplacian,
          Scharr, Canny, LOG]  
for i in np.arange(9):  
   plt.subplot(3,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()  

```

女神的运行结果如下图所示：

---


## 五.高阶图像处理

Python图像处理第四部分为“高阶图像处理”，主要包括Python图像特效、Python图像分割、Python傅里叶变换及霍夫变换和Python图像分类。

### 1.Python图像特效

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像怀旧特效是指图像经历岁月的昏暗效果，如图所示，左边“src”为原始图像，右边“dst”为怀旧特效图像。

怀旧特效是将图像的RGB三个分量分别按照一定比例进行处理的结果，其怀旧公式所示。

核心代码如下：

滤镜主要是用来实现图像的各种特殊效果，它在Photoshop中具有非常神奇的作用。滤镜通常需要同通道、图层等联合使用，才能取得最佳艺术效果。本小节将讲述一种基于颜色查找表（Look up Table）的滤镜处理方法，它通过将每一个原始颜色进行转换之后得到新的颜色。比如，原始图像的某像素点为红色（R-255, G-0, B-0），进行转换之后变为绿色（R-0, G-255, B-0），之后所有是红色的地方都会被自动转换为绿色，而颜色查找表就是将所有的颜色进行一次（矩阵）转换，很多的滤镜功能就是提供了这么一个转换的矩阵，在原始色彩的基础上进行颜色的转换。

假设现在存在一张新的滤镜颜色查找表，如下图所示，它是一张512×512大小，包含各像素颜色分布的图像。该图片建议大家去我github下载。

滤镜特效实现的Python代码如下所示，它通过自定义getBRG()函数获取颜色查找表中映射的滤镜颜色，再依次循环替换各颜色。

<mark>经典代码：</mark><br/> **(1) 图像流年特效**

```
#coding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2
import math
import numpy as np

#读取原始图像
img = cv2.imread('nv.png')

#获取图像行和列
rows, cols = img.shape[:2]

#新建目标图像
dst = np.zeros((rows, cols, 3), dtype="uint8")

#图像流年特效
for i in range(rows):
    for j in range(cols):
        #B通道的数值开平方乘以参数12
        B = math.sqrt(img[i,j][0]) * 12
        G =  img[i,j][1]
        R =  img[i,j][2]
        if B&gt;255:
            B = 255
        dst[i,j] = np.uint8((B, G, R))
        
#显示图像
cv2.imshow('src', img)
cv2.imshow('dst', dst)

cv2.waitKey()
cv2.destroyAllWindows()

```

输出结果如下图所示：

**(2) 图像滤镜特效**

```
#coding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2
import math
import numpy as np

#获取滤镜颜色
def getBGR(img, table, i, j):
    #获取图像颜色
    b, g, r = img[i][j]
    #计算标准颜色表中颜色的位置坐标
    x = int(g/4 + int(b/32) * 64)
    y = int(r/4 + int((b%32) / 4) * 64)
    #返回滤镜颜色表中对应的颜色
    return lj_map[x][y]

#读取原始图像
img = cv2.imread('nv.png')
lj_map = cv2.imread('table.png')

#获取图像行和列
rows, cols = img.shape[:2]

#新建目标图像
dst = np.zeros((rows, cols, 3), dtype="uint8")

#循环设置滤镜颜色
for i in range(rows):
    for j in range(cols):
        dst[i][j] = getBGR(img, lj_map, i, j)
        
#显示图像
cv2.imshow('src', img)
cv2.imshow('dst', dst)

cv2.waitKey()
cv2.destroyAllWindows()

```

女神照片的输出结果如下图所示：

**(3) 图像素描特效**

```
#coding:utf-8
#By:Eastmount CSDN 2021-08-20
import cv2
import numpy as np
 
def dodgeNaive(image, mask):
    # determine the shape of the input image
    width, height = image.shape[:2]
 
    # prepare output argument with same size as image
    blend = np.zeros((width, height), np.uint8)
 
    for col in range(width):
        for row in range(height):
            # do for every pixel
            if mask[col, row] == 255:
                # avoid division by zero
                blend[col, row] = 255
            else:
                # shift image pixel value by 8 bits
                # divide by the inverse of the mask
                tmp = (image[col, row] &lt;&lt; 8) / (255 - mask)
                # print('tmp={}'.format(tmp.shape))
                # make sure resulting value stays within bounds
                if tmp.any() &gt; 255:
                    tmp = 255
                    blend[col, row] = tmp
    return blend
 
def dodgeV2(image, mask):
    return cv2.divide(image, 255 - mask, scale=256)
 
def burnV2(image, mask):
    return 255 - cv2.divide(255 - image, 255 - mask, scale=256)
 
def rgb_to_sketch(src_image_name, dst_image_name):
    img_rgb = cv2.imread(src_image_name)
    img_gray = cv2.cvtColor(img_rgb, cv2.COLOR_BGR2GRAY)
 
    img_gray_inv = 255 - img_gray
    img_blur = cv2.GaussianBlur(img_gray_inv, ksize=(21, 21),
                                sigmaX=0, sigmaY=0)
    img_blend = dodgeV2(img_gray, img_blur)
 
    cv2.imshow('original', img_rgb)
    cv2.imshow('gray', img_gray)
    cv2.imshow('gray_inv', img_gray_inv)
    cv2.imshow('gray_blur', img_blur)
    cv2.imshow("pencil sketch", img_blend)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    cv2.imwrite(dst_image_name, img_blend)
 
if __name__ == '__main__':
    src_image_name = 'nv.png'
    dst_image_name = 'sketch_example.jpg'
    rgb_to_sketch(src_image_name, dst_image_name)

```

女神照片的输出结果如下图所示：

---


### 2.Python图像分割

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像分割（Image Segmentation）技术是计算机视觉领域的重要研究方向，是图像语义理解和图像识别的重要一环。它是指将图像分割成若干具有相似性质的区域的过程，研究方法包括基于阈值的分割方法、基于区域的分割方法、基于边缘的分割方法和基于特定理论的分割方法（含图论、聚类、深度语义等）。该技术广泛应用于场景物体分割、人体背景分割、三维重建、车牌识别、人脸识别、无人驾驶、增强现实等行业。如图1所示，它将鲜花颜色划分为四个层级。

图像分割的目标是根据图像中的物体将图像的像素分类，并提取感兴趣的目标。从数学角度来看，图像分割是将数字图像划分成互不相交的区域的过程。图像分割的过程也是一个标记过程，即把属于同一区域的像索赋予相同的编号。

<mark>经典案例：</mark><br/> **(1)基于纹理背景的图像分割**

```
# -*- coding: utf-8 -*-
#By:Eastmount CSDN 2021-08-20
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
import matplotlib

#读取图像
img = cv2.imread('nv.png')

#灰度化处理图像
grayImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#设置掩码、fgbModel、bgModel
mask = np.zeros(img.shape[:2], np.uint8)
bgdModel = np.zeros((1,65), np.float64)
fgdModel = np.zeros((1,65), np.float64)

#矩形坐标
rect = (100, 100, 500, 800)

#图像分割
cv2.grabCut(img, mask, rect, bgdModel, fgdModel, 5,
            cv2.GC_INIT_WITH_RECT)

#设置新掩码：0和2做背景
mask2 = np.where((mask==2)|(mask==0), 0, 1).astype('uint8')

#设置字体
matplotlib.rcParams['font.sans-serif']=['SimHei']

#显示原图
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
plt.subplot(1,2,1)
plt.imshow(img)
plt.title(u'(a)原始图像')
plt.xticks([]), plt.yticks([])

#使用蒙板来获取前景区域
img = img*mask2[:, :, np.newaxis]
plt.subplot(1,2,2)
plt.imshow(img)
plt.title(u'(b)目标图像')
plt.colorbar()
plt.xticks([]), plt.yticks([])
plt.show()

```

输出结果如下图所示：

**(2)基于均值漂移算法的图像分割**

```
# -*- coding: utf-8 -*-
#By:Eastmount CSDN 2021-08-20
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像灰度颜色
img = cv2.imread('scenery.png') 

#获取图像行和列
rows, cols = img.shape[:2]

#mask必须行和列都加2且必须为uint8单通道阵列
mask = np.zeros([rows+2, cols+2], np.uint8) 

spatialRad = 100 #空间窗口大小
colorRad = 100   #色彩窗口大小
maxPyrLevel = 2  #金字塔层数

#图像均值漂移分割
dst = cv2.pyrMeanShiftFiltering( img, spatialRad, colorRad, maxPyrLevel)

#图像漫水填充处理
cv2.floodFill(dst, mask, (30, 30), (0, 255, 255),
              (100, 100, 100), (50, 50, 50),
              cv2.FLOODFILL_FIXED_RANGE)

#显示图像
cv2.imshow('src', img)
cv2.imshow('dst', dst)
cv2.waitKey()
cv2.destroyAllWindows()

```

输出结果如下图所示：

**(3)图像漫水填充分割**

```
# -*- coding: utf-8 -*-
#By:Eastmount CSDN 2021-08-20
import cv2
import random
import sys
import numpy as np

#使用说明 点击鼠标选择种子点
help_message = '''USAGE: floodfill.py [&lt;image&gt;]
Click on the image to set seed point
Keys:
  f - toggle floating range
  c - toggle 4/8 connectivity
  ESC - exit
'''
 
if __name__ == '__main__':

    #输出提示文本
    print(help_message)

    #读取原始图像
    img = cv2.imread('scenery.png')

    #获取图像高和宽
    h, w = img.shape[:2]

    #设置掩码 长和宽都比输入图像多两个像素点 
    mask = np.zeros((h+2, w+2), np.uint8)

    #设置种子节点和4邻接
    seed_pt = None
    fixed_range = True
    connectivity = 4 

    #图像漫水填充分割更新函数
    def update(dummy=None):
        if seed_pt is None:
            cv2.imshow('floodfill', img)
            return
        
        #建立图像副本并漫水填充
        flooded = img.copy()
        mask[:] = 0 #掩码初始为全0
        lo = cv2.getTrackbarPos('lo', 'floodfill') #观察点像素邻域负差最大值
        hi = cv2.getTrackbarPos('hi', 'floodfill') #观察点像素邻域正差最大值
        print('lo=', lo, 'hi=', hi)

        #低位比特包含连通值 4 (缺省) 或 8
        flags = connectivity
        
        #考虑当前象素与种子象素之间的差（高比特也可以为0）
        if fixed_range:
            flags |= cv2.FLOODFILL_FIXED_RANGE
            
        #以白色进行漫水填充
        cv2.floodFill(flooded, mask, seed_pt,
                      (random.randint(0,255), random.randint(0,255),
                       random.randint(0,255)), (lo,)*3, (hi,)*3, flags)

        #选定基准点用红色圆点标出
        cv2.circle(flooded, seed_pt, 2, (0, 0, 255), -1)
        print("send_pt=", seed_pt)

        #显示图像
        cv2.imshow('floodfill', flooded)

    #鼠标响应函数
    def onmouse(event, x, y, flags, param):
        global seed_pt #基准点

        #鼠标左键响应选择漫水填充基准点
        if flags &amp; cv2.EVENT_FLAG_LBUTTON:
            seed_pt = x, y
            update()

    #执行图像漫水填充分割更新操作
    update()
    
    #鼠标更新操作
    cv2.setMouseCallback('floodfill', onmouse)

    #设置进度条
    cv2.createTrackbar('lo', 'floodfill', 20, 255, update)
    cv2.createTrackbar('hi', 'floodfill', 20, 255, update)

    #按键响应操作
    while True:
        ch = 0xFF &amp; cv2.waitKey()
        #退出
        if ch == 27:
            break
        #选定时flags的高位比特位0
        #此时邻域的选定为当前像素与相邻像素的差, 联通区域会很大
        if ch == ord('f'):
            fixed_range = not fixed_range 
            print('using %s range' % ('floating', 'fixed')[fixed_range])
            update()
        #选择4方向或则8方向种子扩散
        if ch == ord('c'):
            connectivity = 12-connectivity 
            print('connectivity =', connectivity)
            update()
    cv2.destroyAllWindows()

```

输出结果如下图所示：

---


### 3.Python傅里叶变换与霍夫变化

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 傅里叶变换（Fourier Transform，简称FT）常用于数字信号处理，它的目的是将时间域上的信号转变为频率域上的信号。随着域的不同，对同一个事物的了解角度也随之改变，因此在时域中某些不好处理的地方，在频域就可以较为简单的处理。同时，可以从频域里发现一些原先不易察觉的特征。傅里叶定理指出“任何连续周期信号都可以表示成（或者无限逼近）一系列正弦信号的叠加。”

傅里叶公式如下，其中w表示频率，t表示时间，为复变函数。它将时间域的函数表示为频率域的函数f(t)的积分。

傅里叶变换认为一个周期函数（信号）包含多个频率分量，任意函数（信号）f(t)可通过多个周期函数（或基函数）相加合成。从物理角度理解，傅里叶变换是以一组特殊的函数（三角函数）为正交基，对原函数进行线性变换，物理意义便是原函数在各组基函数的投影。如上图所示，它是由三条正弦曲线组合成。其函数如下。

霍夫变换（Hough Transform）是一种特征检测（Feature Extraction），被广泛应用在图像分析、计算机视觉以及数位影像处理。霍夫变换是在1959年由气泡室（Bubble Chamber）照片的机器分析而发明，发明者Paul Hough在1962年获得美国专利。现在广泛使用的霍夫变换是由Richard Duda和Peter Hart在1972年发明，并称之为广义霍夫变换。经典的霍夫变换是检测图片中的直线，之后，霍夫变换不仅能识别直线，也能够识别任何形状，常见的有圆形、椭圆形。

<mark>经典示例：</mark><br/> **(1)傅里叶变换与逆变换**

```
# -*- coding: utf-8 -*-
#By:Eastmount CSDN 2021-08-20
import numpy as np
import cv2
from matplotlib import pyplot as plt
import matplotlib

#读取图像
img = cv2.imread('Lena.png', 0)

#傅里叶变换
dft = cv2.dft(np.float32(img), flags = cv2.DFT_COMPLEX_OUTPUT)
dftshift = np.fft.fftshift(dft)
res1= 20*np.log(cv2.magnitude(dftshift[:,:,0], dftshift[:,:,1]))

#傅里叶逆变换
ishift = np.fft.ifftshift(dftshift)
iimg = cv2.idft(ishift)
res2 = cv2.magnitude(iimg[:,:,0], iimg[:,:,1])

#设置字体
matplotlib.rcParams['font.sans-serif']=['SimHei']

#显示图像
plt.subplot(131), plt.imshow(img, 'gray'), plt.title(u'(a)原始图像')
plt.axis('off')
plt.subplot(132), plt.imshow(res1, 'gray'), plt.title(u'(b)傅里叶变换处理')
plt.axis('off')
plt.subplot(133), plt.imshow(res2, 'gray'), plt.title(u'(b)傅里叶变换逆处理')
plt.axis('off')
plt.show()

```

输出结果如下图所示：

**(2)霍夫变换识别圆**

```
# -*- coding: utf-8 -*-
#By:Eastmount CSDN 2021-08-20
import cv2
import numpy as np
from matplotlib import pyplot as plt

#读取图像
img = cv2.imread('eyes.png')

#灰度转换
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#显示原始图像
plt.subplot(121), plt.imshow(gray, 'gray'), plt.title('Input Image')
plt.axis('off')

#霍夫变换检测圆
circles1 = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1, 20,
                            param1=100, param2=30,
                            minRadius=160, maxRadius=200)
print(circles1)

#提取为二维
circles = circles1[0, :, :]

#四舍五入取整
circles = np.uint16(np.around(circles))

#绘制圆
for i in circles[:]: 
    cv2.circle(img, (i[0],i[1]), i[2], (255,0,0), 5) #画圆
    cv2.circle(img, (i[0],i[1]), 2, (255,0,255), 8) #画圆心

#显示处理图像
plt.subplot(122), plt.imshow(img), plt.title('Result Image')
plt.axis('off')
plt.show()

```

输出结果如下图所示：

---


### 4.Python图像分类

<mark>推荐文章：</mark>

<mark>核心内容：</mark>

<mark>经典知识：</mark><br/> 图像分类（Image Classification）是对图像内容进行分类的问题，它利用计算机对图像进行定量分析，把图像或图像中的区域划分为若干个类别，以代替人的视觉判断。

图像分类的传统方法是特征描述及检测，这类传统方法可能对于一些简单的图像分类是有效的，但由于实际情况非常复杂，传统的分类方法不堪重负。现在，广泛使用机器学习和深度学习的方法来处理图像分类问题，其主要任务是给定一堆输入图片，将其指派到一个已知的混合类别中的某个标签。

在下图中，图像分类模型将获取单个图像，并将为4个标签{cat，dog，hat，mug}分配对应的概率{0.6, 0.3, 0.05, 0.05}，其中0.6表示图像标签为猫的概率，其余类比。

那么，如何编写一个图像分类的算法呢？又怎么从众多图像中识别出猫呢？<br/> 这里所采取的方法和教育小孩看图识物类似，给出很多图像数据，让模型不断去学习每个类的特征。在训练之前，首先需要对训练集的图像进行分类标注，如图2所示，包括cat、dog、mug和hat四类。在实际工程中，可能有成千上万类别的物体，每个类别都会有上百万张图像。

图像分类是输入一堆图像的像素值数组，然后给它分配一个分类标签，通过训练学习来建立算法模型，接着使用该模型进行图像分类预测，具体流程如下：

<mark>经典示例：</mark>

```
# -*- coding: utf-8 -*-
# By: Eastmount CSDN 2021-08-20
import os
import cv2
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix, classification_report

#----------------------------------------------------------------------------------
# 第一步 切分训练集和测试集
#----------------------------------------------------------------------------------

X = [] #定义图像名称
Y = [] #定义图像分类类标
Z = [] #定义图像像素

for i in range(0, 10):
    #遍历文件夹，读取图片
    for f in os.listdir("photo/%s" % i):
        #获取图像名称
        X.append("photo//" +str(i) + "//" + str(f))
        #获取图像类标即为文件夹名称
        Y.append(i)

X = np.array(X)
Y = np.array(Y)

#随机率为100% 选取其中的30%作为测试集
X_train, X_test, y_train, y_test = train_test_split(X, Y,                                                   
test_size=0.3, random_state=1)

print(len(X_train), len(X_test), len(y_train), len(y_test))

#----------------------------------------------------------------------------------
# 第二步 图像读取及转换为像素直方图
#----------------------------------------------------------------------------------

#训练集
XX_train = []
for i in X_train:
    #读取图像
    #print i
    image = cv2.imread(i)
    
    #图像像素大小一致
    img = cv2.resize(image, (256,256),
                     interpolation=cv2.INTER_CUBIC)

    #计算图像直方图并存储至X数组
    hist = cv2.calcHist([img], [0,1], None,
                            [256,256], [0.0,255.0,0.0,255.0])

    XX_train.append(((hist/255).flatten()))

#测试集
XX_test = []
for i in X_test:
    #读取图像
    #print i
    image = cv2.imread(i)
    
    #图像像素大小一致
    img = cv2.resize(image, (256,256),
                     interpolation=cv2.INTER_CUBIC)

    #计算图像直方图并存储至X数组
    hist = cv2.calcHist([img], [0,1], None,
                            [256,256], [0.0,255.0,0.0,255.0])

    XX_test.append(((hist/255).flatten()))

#----------------------------------------------------------------------------------
# 第三步 基于朴素贝叶斯的图像分类处理
#----------------------------------------------------------------------------------

from sklearn.naive_bayes import BernoulliNB
clf = BernoulliNB().fit(XX_train, y_train)
predictions_labels = clf.predict(XX_test)

print('预测结果:')
print(predictions_labels)

print('算法评价:')
print(classification_report(y_test, predictions_labels))

#输出前10张图片及预测结果
k = 0
while k&lt;10:
    #读取图像
    print(X_test[k])
    image = cv2.imread(X_test[k])
    print(predictions_labels[k])
    #显示图像
    cv2.imshow("img", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    k = k + 1

```

实验所采用的数据集为Sort_1000pics数据集，该数据集包含了1000张图片，总共分为10大类，分别是人（第0类）、沙滩（第1类）、建筑（第2类）、大卡车（第3类）、恐龙（第4类）、大象（第5类）、花朵（第6类）、马（第7类）、山峰（第8类）和食品（第9类），每类100张。如图所示。

代码中对预测集的前十张图像进行了显示，其中“368.jpg”图像如图所示，其分类预测的类标结果为“3”，表示第3类大卡车，预测结果正确。

使用朴素贝叶斯算法进行图像分类实验，最后预测的结果及算法评价准确率（Precision）、召回率（Recall）和F值（F1-score）如图16所示。

---


## 六.总结

写到这里，本文就介绍完毕。如果你是一名Python初学者或想了解图像处理知识，真心推荐该系列，并且多实践多写代码，最后希望文章对您有所帮助。

> 
八月初又认识了很多朋友，也解答了许多学生和博友的问题，虽然自己技术不是很好，但真心感激这一路走来认识了许多伙伴，也享受相互交流的过程，感恩遇见，不负青春。<br/><br/> 这半个月交流让我印象深刻的是复旦、浙大和武大离职读博想当老师的朋友，我们交流了许多学习心得并鼓励前行；也有贵财计科、软工、电商几位学生工作的问题；以及家乡贵财、贵大、兴义师范、贵师大、贵理工选择考研或已经读研同学的咨询，我们相互学习；当然还有许多创业、工作、学习或寻求正能量的交流，包括来自广东、南航、中北、湖北工业、建筑转NLP、安全或AI行业的博友。最让我享受的是，许多我教过的学生或相互鼓励前行的博友，经常祝福我们一家并保持良好的情谊，这或许就是分享的魅力，觉不亚于一次holy shit。教育的方式多种多样，我享受这样的交流和鼓励方式，山川异域，携手同行。<br/><br/> 最后补充一句，父母能带给孩子最好的礼物，或许就是“处于逆境时的应对能力”，希望小珞珞能坚强、健康、快乐的成长，女神也能美丽和幸福一辈子。爱你们哟。<br/> <img alt="在这里插入图片描述" height="300" src="https://img-blog.csdnimg.cn/58aeb2f4922b489587ae8d0476942cec.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="650"/>


(By:Eastmount 2021-08-20 晚上12点 http://blog.csdn.net/eastmount/ )

---


参考文献：
