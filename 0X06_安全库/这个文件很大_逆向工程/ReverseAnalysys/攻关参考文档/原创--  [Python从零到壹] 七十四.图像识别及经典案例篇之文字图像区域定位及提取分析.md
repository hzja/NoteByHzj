# 原创
：  [Python从零到壹] 七十四.图像识别及经典案例篇之文字图像区域定位及提取分析

# [Python从零到壹] 七十四.图像识别及经典案例篇之文字图像区域定位及提取分析

欢迎大家来到“Python从零到壹”，在这里我将分享约200篇Python系列文章，带大家一起去学习和玩耍，看看Python这个有趣的世界。所有文章都将结合案例、代码和作者的经验讲解，真心想把自己近十年的编程经验分享给大家，希望对您有所帮助，文章中不足之处也请海涵。Python系列整体框架包括基础语法10篇、网络爬虫30篇、可视化分析10篇、机器学习20篇、大数据分析20篇、图像识别30篇、人工智能40篇、Python安全20篇、其他技巧10篇。您的关注、点赞和转发就是对秀璋最大的支持，知识无价人有情，希望我们都能在人生路上开心快乐、共同成长。

该系列文章主要讲解Python OpenCV图像处理和图像识别知识，前期主要讲解图像处理基础知识、OpenCV基础用法、常用图像绘制方法、图像几何变换等，中期讲解图像处理的各种运算，包括图像点运算、形态学处理、图像锐化、图像增强、图像平滑等，后期研究图像识别、图像分割、图像分类、图像特效处理以及图像处理相关应用。

<mark>**第一部分作者介绍了图像处理基础知识，第二部分介绍了图像运算和图像增强，接下来第三部分我们将详细讲解图像识别及图像处理经典案例，该部分属于高阶图像处理知识，能进一步加深我们的理解和实践能力。前文介绍了两种经典的图像去雾算法，这篇文章将详细讲解文字图像区域定位和提取分析。图像识别（Image Recognition）是通过算法和函数提取像素中的某些特征，并对图像进行识别和分类的过程。本文提供的案例将为读者提供深入的理解，也是最简单的图像识别案例。希望对大家有所帮助，如果有不足之处，还请海涵。**</mark>

#### 文章目录

**下载地址：记得点赞喔 O(∩_∩)O 前文赏析见文章末尾**

> 
作者新开的“娜璋AI安全之家”将专注于Python和安全技术，主要分享Web渗透、系统安全、人工智能、大数据分析、图像识别、恶意代码检测、CVE复现、威胁情报分析等文章。虽然作者是一名技术小白，但会保证每一篇文章都会很用心地撰写，希望这些基础性文章对你有所帮助，在Python和安全路上与大家一起进步。


---


## 一.OpenCV文字识别基本步骤

一个完整的文字图像识别系统是一个复杂的系统，主要包括图像灰度处理、图像平滑、图像增强、阈值分割、形态学处理、文字提取、边缘检测、锐化处理等步骤。本文提出了一种改进的图像增强及识别方法，其框架如图1所示。

具体步骤如下：

注意，在OpenCV中，通常采用findContours()函数寻找文字轮廓，定位并提取目标文字，接着调用drawContours()函数绘制相关轮廓，输出最终图像。

---


## 二.图像灰度和平滑处理

图像灰度转化是将彩色图像转换为灰度化图像的过程。灰度图像中每个像素仅具有一种样本颜色，其灰度是位于黑色与白色之间的多级色彩深度。式(1)是本文所采用的灰度处理公式。

式中，R、G、B分别为图像中每个像素点的三原色红、绿、蓝分量，Gray为灰度处理后该像素点的灰度值。它将原始RGB(R,G,B)颜色均匀地替换成新颜色RGB(Gray,Gray,Gray)，从而将彩色图像转化为灰度图像。由于人类的眼睛感官蓝色的敏感度最低，感官绿色的敏感度最高，因此将RGB三分量按照0.299、0.587、0.114比例加权平均能得到较合理的灰度图。

该部分实现代码如下：

```
# coding:utf8
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread("word.png" )

#转换成灰度图像
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#显示图像
cv2.imshow('Gray Image', gray)
cv2.waitKey(0)
cv2.destroyAllWindows()

```

输出结果如图2所示。

中值滤波（Median Blur）是一种非线性平滑方法，能够在消除噪声的同时保留边界信息。该方法通过计算每一个像素点某邻域范围内所有像素点灰度值的中值，来替换该像素点的灰度值，从而让周围的像素值更接近真实情况，消除孤立的噪声。

中值滤波算法的计算过程如图3所示。选择含有五个点的窗口，依次扫描该窗口中的像素，每个像素点所对应的灰度值按照升序或降序排列，然后获取最中间的值来替换该点的灰度值。

上图展示的是矩形窗口，常用的窗口还包括正方形、十字形、环形和圆形等，不同形状的窗口会带来不同的过滤效果，其中正方形和圆形窗口适合于外轮廓边缘较长的图像，十字形窗口适合于带尖角形状的图像。实现代码如下：

```
# coding:utf8
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread("word.png" )

#转换成灰度图像
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#中值滤波去除噪声
median = cv2.medianBlur(gray, 3)

#显示图像
cv2.imshow('Gray Image', gray)
cv2.imshow('Median Blur', median)
cv2.waitKey(0)
cv2.destroyAllWindows()

```

处理后的结果如图4所示，它是中值滤波处理后的图像，它有效地过滤掉了图像的噪声。

---


## 三.图像增强处理

直方图均衡化是图像灰度变化的一个重要处理，被广泛应用于图像增强领域。它是指通过某种灰度映射将原始图像的像素点均匀地分布在每一个灰度级上，其结果将产生一幅灰度级分布概率均衡的图像。直方图均衡化的中心思想是把原始图像的灰度直方图从比较集中的某个灰度区间转变为全范围均匀分布的灰度区间，通过该处理，增加了像素灰度值的动态范围，从而达到增强图像整体对比度的效果。该算法的处理过程如下：

首先，计算原始图像直方图的概率密度，如公式(2)所示。其中，表示第k个灰度级（k=0,1,2,…,L-1），L最大值为256；表示图像中灰度级为的像素个数；N表示图像中像素的总个数；为图像中第k个灰度级占总像素数的比例。

其次，通过灰度变换函数T计算新图像灰度级的概率密度。新图像灰度级的概率密度是原始图像灰度级概率密度的累积，如公式(3)所示。其中，k是新图像的灰度级，k=0,1,2,…,L-1；表示原始图像的第k个灰度级；为直方图均衡化处理后的第k个灰度级。

最后，计算新图像的灰度值。由于公式(3)计算所得的位于0至1之间，需要乘以图像的最大灰度级L，转换为最终的灰度值res，如公式(4)所示。

如果图像清晰度较差或存在噪声，可以进行图像直方图均衡化处理，图5表示原始图像及其直方图。

图6表示直方图均衡化处理后的图像及其直方图。从效果图可以看出，经过直方图均衡化处理，图像变得更加清晰，图像的灰度级分布也更加均匀。

核心代码如下：

---


## 四.文字边缘提取

图像数据在收集或传输过程中，受一些外界因素影响，会使得图像存在模糊和有噪声的情况，从而影响图像识别工作。此时需要通过图像锐化处理，加强原图像的高频部分，锐化突出图像的边缘细节，从而改善图像的对比度，使模糊的图像变得更加清晰。

Sobel算子是一种计算图像明暗程度近似值的差分算子，它根据图像边缘附近的明暗程度把该区域内超过某个值的特定点记为边缘。Sobel算子根据相邻点的距离远近为当前像素点赋予不同的权重，距离越近的像素点对当前像素的影响越大，反之越小，从而实现图像锐化并突出边缘轮廓。Sobel算子的边缘定位更加准确，常用于噪声较多、灰度渐变的图像。其模板如公式(5)所示：

其中，dx表示水平方向，dy表示垂直方向，Sobel算子的最终计算如公式(6)所示：

图7为Sobel算子的处理结果，由于其考虑各方面因素，对噪声较多的图像处理效果更好。

```
# -*- coding: utf-8 -*-
# By:Eastmount
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread("word.png" )

#转换成灰度图像
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#中值滤波去除噪声
median = cv2.medianBlur(gray, 3)

#图像直方图均衡化
equalize = cv2.equalizeHist(median)

#Sobel算子锐化处理
sobel = cv2.Sobel(median, cv2.CV_8U, 1, 0, ksize = 3)

#显示图像
cv2.imshow('Sobel Image', sobel)
cv2.waitKey(0)
cv2.destroyAllWindows()

```

运行结果如下图所示。

---


## 五.阈值分割处理

图像二值化（Binarization）旨在提取图像中的目标物体，将背景以及噪声区分开来。通常会设置一个临界值，将图像的像素划分为两类，常见的二值化算法如公式(5)所示。

当某个像素点的灰度Gray(i,j)小于阈值T时，其像素设置为0，表示黑色；当灰度Gray(i,j)大于或等于阈值T时，其像素值为255，表示白色。Python OpenCV中提供了阈值函数threshold()实现二值化处理，当像素大于等于127时灰度值设置为最大值255，小于127的像素点灰度值设置为0。二值化处理为后续的水族文字识别提供有效支撑。

完整代码如下所示：

```
# -*- coding: utf-8 -*-
# By:Eastmount
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread("word.png" )

#转换成灰度图像
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#中值滤波去除噪声
median = cv2.medianBlur(gray, 3)

#图像直方图均衡化
equalize = cv2.equalizeHist(median)

#Sobel算子锐化处理
sobel = cv2.Sobel(median, cv2.CV_8U, 1, 0, ksize = 3)

#图像二值化处理
ret, binary = cv2.threshold(sobel, 0, 255,
                            cv2.THRESH_OTSU+cv2.THRESH_BINARY)

#显示图像
cv2.imshow('Binary Image', binary)
cv2.waitKey(0)
cv2.destroyAllWindows()

```

图8表示图像二值化处理后的效果图。

---


## 六.形态学处理

图像的腐蚀（Erosion）和膨胀（Dilation）是两种基本的形态学运算，主要用来寻找图像中的极小区域和极大区域。其中腐蚀类似于“领域被蚕食”，它将图像中的高亮区域或白色部分进行缩减细化，其运行结果比原图的高亮区域更小；膨胀类似于“领域扩张”，它将图像中的高亮区域或白色部分进行扩张，其运行结果比原图的高亮区域更大。

腐蚀的运算符是“－”，其定义如下：

该公式表示图像A用卷积模板B来进行腐蚀处理，通过模板B与图像A进行卷积计算，得出B覆盖区域的像素点最小值，并用这个最小值来替代参考点的像素值。如图9所示，将左边的原始图像A腐蚀处理为右边的效果图A-B。

膨胀的运算符是“⊕”，其定义如下：

该公式表示用B来对图像A进行膨胀处理，其中B是一个卷积模板，其形状可以为正方形或圆形，通过模板B与图像A进行卷积计算，扫描图像中的每一个像素点，用模板元素与二值图像元素做“与”运算，如果都为0，那么目标像素点为0，否则为1。从而计算B覆盖区域的像素点最大值，并用该值替换参考点的像素值实现图像膨胀。图10是将左边的原始图像A膨胀处理为右边的效果图A⊕B。

图11和图12是经过膨胀和腐蚀处理后的效果图，图像经过腐蚀处理过滤掉文字部分，再通过膨胀处理提取图像的背景轮廓。<br/> <img alt="在这里插入图片描述" height="420" src="https://img-blog.csdnimg.cn/direct/43ac4322f6a1496d93c2828dc1b1f8f3.png#pic_center" width="540"/>

---


## 七.文字区域提取

经过图像腐蚀和膨胀处理后，得到如图10所示的效果图，它将图像的背景轮廓提取出来，接着利用文字提取算法识别目标文字。算法流程如下：

完整代码如下：

```
# -*- coding: utf-8 -*-
# By:Eastmount
import cv2
import numpy as np
import matplotlib.pyplot as plt

#读取原始图像
img = cv2.imread("word.png" )

#转换成灰度图像
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#中值滤波去除噪声
median = cv2.medianBlur(gray, 3)

#图像直方图均衡化
equalize = cv2.equalizeHist(median)

#Sobel算子锐化处理
sobel = cv2.Sobel(median, cv2.CV_8U, 1, 0, ksize = 3)

#图像二值化处理
ret, binary = cv2.threshold(sobel, 0, 255,
                            cv2.THRESH_OTSU+cv2.THRESH_BINARY)

#膨胀和腐蚀处理 设置膨胀和腐蚀操作的核函数
element1 = cv2.getStructuringElement(cv2.MORPH_RECT, (30, 9))
element2 = cv2.getStructuringElement(cv2.MORPH_RECT, (24, 6))

#膨胀突出轮廓
dilation = cv2.dilate(binary, element2, iterations = 1)

#腐蚀去掉细节
erosion = cv2.erode(dilation, element1, iterations = 1)

#查找文字轮廓
region = []
contours, hierarchy = cv2.findContours(erosion,
                                       cv2.RETR_TREE,
                                       cv2.CHAIN_APPROX_SIMPLE)

#筛选面积
for i in range(len(contours)):
    #遍历所有轮廓
    cnt = contours[i]
    
    #计算轮廓面积
    area = cv2.contourArea(cnt) 
    
    #寻找最小矩形
    rect = cv2.minAreaRect(cnt)

    #轮廓的四个点坐标
    box = cv2.boxPoints(rect)
    box = np.int0(box)

    # 计算高和宽
    height = abs(box[0][1] - box[2][1])
    width = abs(box[0][0] - box[2][0])

    #过滤太细矩形
    if(height &gt; width * 1.5): 
        continue

    region.append(box)
   
#定位的文字用绿线绘制轮廓
for box in region:
    print(box)
    cv2.drawContours(img, [box], 0, (0, 255, 0), 2)

#显示图像
cv2.imshow('Gray Image', gray)
cv2.imshow('Median Blur', median)
cv2.imshow('Sobel Image', sobel)
cv2.imshow('Binary Image', binary)
cv2.imshow('Dilation Image', dilation)
cv2.imshow('Erosion Image', erosion)
cv2.imshow('Result Image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()

```

最终输出结果如图13所示，它是调用findContours()函数寻找轮廓，并过滤掉面积异常区域，采用函数drawContours()绘制文字轮廓，最终输出图像有效地将原图中所有文字区域定位并提取出来。

---


## 八.总结

本文讲解了文字图像区域定位及提取案例，主要采用OpenCV图像处理算法组合实现，包括图像灰度处理、图像平滑处理、图像增强处理、文字边缘提取、阈值分割处理、形态学处理和文字区域提取，这是非常经典的案例。此外，该方法是图像分割和图像识别前的重要环节，可以广泛应用于文字识别、车牌提取、区域定位等领域。

感谢在求学路上的同行者，不负遇见，勿忘初心。图像处理系列主要包括三部分，分别是：

> 
忙碌的11月，忙碌的2023。转眼四年过去，我和她都不容易，两人每次看“致谢”都会泪目，青春变了，唯有情感不变，希望一家人健康快乐。刚到寝室，要战斗了！ <br/><br/> <img alt="在这里插入图片描述" height="560" src="https://img-blog.csdnimg.cn/139d1b6497224e069e1449a76536d36b.png#pic_center" width="460"/>


(By:Eastmount 2024-01-12 夜于贵阳 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


**参考文献：**

---


## 前文赏析

尽管该部分占大量篇幅，但我舍不得删除，哈哈！

<mark>第一部分 基础语法</mark>

<mark>第二部分 网络爬虫</mark>

<mark>第三部分 数据分析和机器学习</mark>

<mark>第四部分 Python图像处理基础</mark>

<mark>第五部分 Python图像运算和图像增强</mark>

<mark>第六部分 Python图像识别和图像高阶案例</mark>

第七部分 NLP与文本挖掘

第八部分 人工智能入门知识

第九部分 网络攻防与AI安全

第十部分 知识图谱构建实战

扩展部分 人工智能高级案例
