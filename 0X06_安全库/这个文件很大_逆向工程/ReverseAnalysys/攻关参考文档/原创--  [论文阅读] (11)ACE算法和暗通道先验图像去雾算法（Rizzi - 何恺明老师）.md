# 原创
：  [论文阅读] (11)ACE算法和暗通道先验图像去雾算法（Rizzi | 何恺明老师）

# [论文阅读] (11)ACE算法和暗通道先验图像去雾算法（Rizzi | 何恺明老师）

《娜璋带你读论文》系列主要是督促自己阅读优秀论文及听取学术讲座，并分享给大家，希望您喜欢。由于作者的英文水平和学术能力不高，需要不断提升，所以还请大家批评指正，非常欢迎大家给我留言评论，学术路上期待与您前行，加油。

<font color="red">**前一篇文章详细介绍和总结基于溯源图的APT攻击检测安全顶会内容，花了作者一个多月时间。这篇文章将讲解ACE去雾算法、暗通道先验去雾算法以及雾化生成算法，并且参考了两位计算机视觉大佬（Rizzi 何恺明）的论文。希望这篇文章对您有所帮助，这些大佬是真的值得我们去学习，献上小弟的膝盖~fighting！**</font>

#### 文章目录

**前文赏析：**

---


## 一.图像去雾

随着社会的发展，环境污染逐渐加剧，越来越多的城市频繁出现雾霾，这不仅给人们的身体健康带来危害，还给那些依赖图像信息的计算机视觉系统造成了不良影响，因为在雾天采集到的图像对比度和饱和度均较低，颜色易发生偏移与失真等。因此，寻找一种简单有效的图像去雾方法，对计算机视觉的后续研究至关重要。

> 
该部分主要从下列几篇论文摘取对图像去雾算法进行普及，引用及参考中文论文：



图像增强（Image Enhancement）是指按照某种特定的需求，突出图像中有用的信息，去除或者削弱无用的信息。图像增强的目的是使处理后的图像更适合人眼的视觉特性或易于机器识别。 在医学成像、遥感成像、人物摄影等领域，图像增强技术都有着广泛的应用。图像增强同时可以作为目标识别、目标跟踪、特征点匹配、图像融合、超分辨率重构等图像处理算法的预处理算法。

近些年来，出现了众多的单幅图像去雾算法，应用比较广泛的有：

<mark>其主要可以分为 3 类：基于图像增强的去雾算法、基于图像复原的去雾算法和基于 CNN 的去雾算法。</mark>

**(1) 基于图像增强的去雾算法**<br/> 通过图像增强技术突出图像细节，提升对比度，使之看起来更加清晰，这类算法的适用性较广。具体的算法有：

此外，在这类算法的基础上出现了众多的基于图像增强原理的改进算法。

**(2) 基于图像复原的去雾算法**<br/> 主要是基于大气散射物理学模型，通过对大量有雾图像和无雾图像进行观察总结，得到其中存在的一些映射关系，然后根据有雾图像的形成过程来进行逆运算，从而恢复清晰图像。其中最经典的要属何恺明大佬提出的：

**(3) 基于CNN的去雾算法**<br/> 使用 CNN 建立一个端到端的模型，通过有雾图像恢复出无雾图像，目前使用神经网络进行去雾的算法主要有两种思路：

CNN 因其强大的学习能力在多个领域得到应用，因此也出现了采用 CNN 进行去雾的算法。2016年CAI等首次提出了一种名为DehazeNet的去雾网络，用于估计有雾图像的透射率。DehazeNet 将有雾的模糊图像作为输入，输出其透射率，基于大气散射模型理论恢复出无雾的清晰图像。

下图是分别对直方图均衡化、暗通道先验去雾、DehazeNet和AOD-Net去雾算法进行测试，实验结果如图所示。由图可知，基于图像增强的直方图均衡化算法的去雾图像对比度明显增强，由于不考虑降质原因，在增加对比度的同时也对噪声进行了放大，出现细节丢失与色彩偏差现象。基于物理模型的暗通道去雾算法、基于神经网络的 DehazeNet 和 AOD-Net 算法的去雾效果较直方图均衡化算法更佳。

其他去雾算法对比结果如下图所示，比如城市和道路有无图像去雾效果对比。

最后，正如总结王道累老师总结的一样，目前针对有雾图像去雾的算法主要是从基于图像增强、图像复原和 CNN 3 个方向进行的。

上述 3 类去雾算法对于雾天图像都有着明显的去雾效果，尽管其在实际生活中已经得到了广泛的应用，但下述几点仍有可能是今后图像去雾领域的研究重点和难点：

---


## 二.ACE去雾算法

### 1.算法原理

该部分主要介绍参考作者书籍以及相关论文进行叙述，简单介绍ACE算法的原理知识。如果读者想详细了解其原理，推荐阅读英文原文，详见下面的参考文献，都是大佬。

> 
引用及参考中文论文：

英文原文：



图像对比度增强的算法在很多场合都有用处，特别是在医学图像中，这是因为在众多疾病的诊断中，医学图像的视觉检查时很有必要的。Retinex算法是代表性的图像增强算法，它根据人的视网膜和大脑皮层模拟对物体颜色的波长光线反射能力而形成，对复杂环境下的一维条码具有一定范围内的动态压缩，对图像边缘有着一定自适应的增强。

**自动色彩均衡（Automatic Color Enhancement，ACE）** 算法是Rizzi大神在Retinex算法的理论上提出的，它通过计算图像目标像素点和周围像素点的明暗程度及其关系来对最终的像素值进行校正，实现图像的对比度调整，产生类似人体视网膜的色彩恒常性和亮度恒常性的均衡，具有很好的图像增强效果。

ACE算法包括两个步骤：

**(1) 区域自适应滤波**<br/> 输入图像I(灰度图为例),该步是对单通道图像I中所有点p的区域自适应滤波，得到完成色差校正，空域重构后的中间结果图像，计算公式如下：

式中：Ic§-Ic(j)为p、j两个像素点间灰度差值，表达拟生物学上的侧抑制性；d(p,j)表示距离度量函数，使用两点间的欧氏距离，作用上控制点j对p的影响权重，映射出滤波的区域适应性；Sa(x)是亮度表现函数(奇函数)，本文算法选择经典Saturation函数。

不同亮度函数和参数的选择控制了对比度增强的程度，经典的Saturation函数在饱和前取越大的斜率，结果的对比度增强越明显，如图2所示，极限情况是sign函数形式，而Sign函数由于无差别过度增强放大，导致噪声同样得到放大效果不佳，最终选择Saturation函数作为相对亮度表现函数。公式如下：

**(2) 色调重整拉伸，对图像动态扩展**<br/> 将式(1)中得到的中间量拉伸映射到 [0, 255] 中，占满动态范围 [0, 255]（8位灰度图像），计算公式如下，式中：[minR,maxR]是中间量L(x)的全部定义域，该项使图像达到全局白平衡。

下图是条形码图像进行ACE图像增强后的效果图，通过图像增强后的图(b)对比度更强，改善了原图像的明暗程度，增强的同时保持了图像的真实性。

ACE算法英文介绍如下：

实验对比效果如下图所示，大家在写该主题论文的时候，注意和传统方法对比。

---


### 2.代码实现

由于OpenCV中暂时没有ACE算法包，下面的代码是借鉴“zmshy2128”老师的文章，修改实现的彩色直方图均衡化处理。后面有机会作者详细分析其代码实现过程。

```
# -*- coding: utf-8 -*-
# By:Eastmount CSDN 2021-03-12
# 惨zmshy2128老师文章并修改成Python3代码
import cv2
import numpy as np
import math
import matplotlib.pyplot as plt

#线性拉伸处理
#去掉最大最小0.5%的像素值 线性拉伸至[0,1]
def stretchImage(data, s=0.005, bins = 2000):   
    ht = np.histogram(data, bins);
    d = np.cumsum(ht[0])/float(data.size)
    lmin = 0; lmax=bins-1
    while lmin&lt;bins:
        if d[lmin]&gt;=s:
            break
        lmin+=1
    while lmax&gt;=0:
        if d[lmax]&lt;=1-s:
            break
        lmax-=1
    return np.clip((data-ht[1][lmin])/(ht[1][lmax]-ht[1][lmin]), 0,1)

#根据半径计算权重参数矩阵
g_para = {}
def getPara(radius = 5):                        
    global g_para
    m = g_para.get(radius, None)
    if m is not None:
        return m
    size = radius*2+1
    m = np.zeros((size, size))
    for h in range(-radius, radius+1):
        for w in range(-radius, radius+1):
            if h==0 and w==0:
                continue
            m[radius+h, radius+w] = 1.0/math.sqrt(h**2+w**2)
    m /= m.sum()
    g_para[radius] = m
    return m

#常规的ACE实现
def zmIce(I, ratio=4, radius=300):                     
    para = getPara(radius)
    height,width = I.shape
    zh = []
    zw = []
    n = 0
    while n &lt; radius:
        zh.append(0)
        zw.append(0)
        n += 1
    for n in range(height):
        zh.append(n)
    for n in range(width):
        zw.append(n)
    n = 0
    while n &lt; radius:
        zh.append(height-1)
        zw.append(width-1)
        n += 1
    #print(zh)
    #print(zw)
    
    Z = I[np.ix_(zh, zw)]
    res = np.zeros(I.shape)
    for h in range(radius*2+1):
        for w in range(radius*2+1):
            if para[h][w] == 0:
                continue
            res += (para[h][w] * np.clip((I-Z[h:h+height, w:w+width])*ratio, -1, 1))
    return res

#单通道ACE快速增强实现
def zmIceFast(I, ratio, radius):
    print(I)
    height, width = I.shape[:2]
    if min(height, width) &lt;=2:
        return np.zeros(I.shape)+0.5
    Rs = cv2.resize(I, (int((width+1)/2), int((height+1)/2)))
    Rf = zmIceFast(Rs, ratio, radius)             #递归调用
    Rf = cv2.resize(Rf, (width, height))
    Rs = cv2.resize(Rs, (width, height))
 
    return Rf+zmIce(I,ratio, radius)-zmIce(Rs,ratio,radius)   

#rgb三通道分别增强 ratio是对比度增强因子 radius是卷积模板半径          
def zmIceColor(I, ratio=4, radius=3):               
    res = np.zeros(I.shape)
    for k in range(3):
        res[:,:,k] = stretchImage(zmIceFast(I[:,:,k], ratio, radius))
    return res

#主函数
if __name__ == '__main__':
    img = cv2.imread('car.png')
    res = zmIceColor(img/255.0)*255
    cv2.imwrite('car-Ice.jpg', res)

```

运行结果如图所示，ACE算法能有效进行图像去雾处理，实现图像的细节增强。

最后是目标检测去雾和女神去雾的效果，哈哈，继续加油！

---


## 三.暗通道先验去雾算法

该算法是计算机视觉领域何恺明大佬于2009年提出的图像去雾经典算法，并获取当年CVPR最佳论文。论文题目为《Single Image Haze Removal Using Dark Channel Prior》。下图是大佬的百科简介，是真的厉害，值得我们大家学习。

### 1.算法原理

言归正传，如果是图像处理或研究图像去雾领域的作者，建议大家认真阅读这篇英文原文，能在2009年提出该算法真的很惊艳。

> 
引用及参考中文论文：

<br/>
英文原文：



**暗通道先验（Dark Channel Prior, DCP）去雾算法** 依赖大气散射模型进行去雾处理，通过对大量有雾图像和无雾图像进行观察总结，得到其中存在的一些映射关系，然后根据有雾图像的形成过程来进行逆运算，从而恢复清晰图像。

算法实现过程及原理如下，参考何恺明老师和何涛老师的论文。

**(1) 大气散射模型**<br/> 在计算机视觉和计算机图形学中，方程所描述的大气散射模型被广泛使用。参数解释如下：

方程右边第一项为场景直接衰减项，第二项为环境光项。

**(2) 暗通道定义**<br/> 在绝大多数非天空的局部区域中，某些像素总会至少有一个颜色通道的值很低。对于一幅图像J(x)，其暗通道的数学定义表示如下：

其中，Ω(x)表示以x为中心的局部区域，上标c表示RGB三个通道。该公式的意义用代码表达也很简单，首先求出每个像素RGB分量中的最小值，存入一副和原始图像大小相同的灰度图中，然后再对这幅灰度图进行最小值滤波，滤波的半径由窗口大小决定。

**(3) 暗通道先验理论**<br/> 暗通道先验理论指出：对于非天空区域的无雾图像J(x)的暗通道趋于０，即：

实际生活中造成暗原色中低通道值主要有三个因素：

总之，自然景物中到处都是阴影或者彩色，这些景物的图像的暗原色总是很灰暗的，而有雾的图像较亮。因此，可以明显的看到暗通道先验理论的普遍性。

**(4) 公式变形**<br/> 根据大气散射模型，将第一个公式稍作处理，变形为下式：

假设每一个窗口的透射率t(x)为常数，记为t’(x)，并且A值已给定，对式两边同时进行两次最小值运算，可得：

其中，J(x)是要求的无雾图像，根据前述的暗通道先验理论可知：

因此可推导出：

**(5) 透射率计算**<br/> 将上式带入可得到透射率t’(x)的预估值，如下所示：

现实生活中，即便晴空万里，空气中也会存在一些颗粒，在眺望远处的景物时，人们还是能感觉到雾的存在。另外，雾的存在让人们感受到景深，因此在去雾的同时有必要保留一定程度的雾。可以通过引入一个0到1之 间 的 因 子 w（一 般取0.95）对预估透射率进行修正，如式所示：

以上的推导过程均假设大气光值A是已知的，在实际中，可以借助暗通道图从原始雾图中求取。具体步骤如下：

此外，由于透射率t偏小时，会造成J偏大，恢复的无雾图像整体向白场过度，因此有必要对透射率设置一个下限值t0（一般取值为0.1），当t值小于t0 时，取t=t0。将以上求得的透射率和大气光值代入公式，最终整理得到图像的恢复公式如下：

这就是暗通道先验去雾算法的原理过程，下面简单补充论文中的处理效果图。

<mark>再次膜拜偶像，极力推荐大家阅读论文。</mark>

---


### 2.算法实现

实现代码引用木老师的，感觉比我写得好，参考如下：

```
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 11 00:16:07 2021
@author: xiuzhang

参考资料：
https://blog.csdn.net/leviopku/article/details/83898619
"""

import sys
import cv2
import math
import numpy as np
 
def DarkChannel(im,sz):
    b,g,r = cv2.split(im)
    dc = cv2.min(cv2.min(r,g),b)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(sz,sz))
    dark = cv2.erode(dc,kernel)
    return dark
 
def AtmLight(im,dark):
    [h,w] = im.shape[:2]
    imsz = h*w
    numpx = int(max(math.floor(imsz/1000),1))
    darkvec = dark.reshape(imsz,1)
    imvec = im.reshape(imsz,3)
 
    indices = darkvec.argsort()
    indices = indices[imsz-numpx::]
 
    atmsum = np.zeros([1,3])
    for ind in range(1,numpx):
       atmsum = atmsum + imvec[indices[ind]]
 
    A = atmsum / numpx;
    return A
 
def TransmissionEstimate(im,A,sz):
    omega = 0.95
    im3 = np.empty(im.shape,im.dtype)
 
    for ind in range(0,3):
        im3[:,:,ind] = im[:,:,ind]/A[0,ind]

    transmission = 1 - omega*DarkChannel(im3,sz)
    return transmission
 
def Guidedfilter(im,p,r,eps):
    mean_I = cv2.boxFilter(im,cv2.CV_64F,(r,r))
    mean_p = cv2.boxFilter(p, cv2.CV_64F,(r,r))
    mean_Ip = cv2.boxFilter(im*p,cv2.CV_64F,(r,r))
    cov_Ip = mean_Ip - mean_I*mean_p
 
    mean_II = cv2.boxFilter(im*im,cv2.CV_64F,(r,r))
    var_I   = mean_II - mean_I*mean_I
 
    a = cov_Ip/(var_I + eps)
    b = mean_p - a*mean_I
 
    mean_a = cv2.boxFilter(a,cv2.CV_64F,(r,r))
    mean_b = cv2.boxFilter(b,cv2.CV_64F,(r,r))
 
    q = mean_a*im + mean_b
    return q
 
def TransmissionRefine(im,et):
    gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    gray = np.float64(gray)/255
    r = 60
    eps = 0.0001
    t = Guidedfilter(gray,et,r,eps)
 
    return t
 
def Recover(im,t,A,tx = 0.1):
    res = np.empty(im.shape,im.dtype)
    t = cv2.max(t,tx)
 
    for ind in range(0,3):
        res[:,:,ind] = (im[:,:,ind]-A[0,ind])/t + A[0,ind]
 
    return res
 
if __name__ == '__main__':
    
    fn = 'car-02.png'
    src = cv2.imread(fn)
    I = src.astype('float64')/255
    
    dark = DarkChannel(I,15)
    A = AtmLight(I,dark)
    te = TransmissionEstimate(I,A,15)
    t = TransmissionRefine(src,te)
    J = Recover(I,t,A,0.1)
    
    arr = np.hstack((I, J))
    cv2.imshow("contrast", arr)
    cv2.imwrite("car-02-dehaze.png", J*255 )
    cv2.imwrite("car-02-contrast.png", arr*255)
    cv2.waitKey();

```

实现效果如下图所示：

如果想和后续目标汽车检测结合，同样可以先去雾再进行检测，如下图所示：

---


## 四.图像噪声和雾生成

图像处理总少不了噪声添加或生成，下面补充两个简单的椒盐噪声和雾气模拟生成的代码。这与本文的实验紧密相关，能为我们提供更多的GAN生成样本。后面人工智能系列文章，GAN我们看看能不能学习真实雾化场景的图像，值得期待，哈哈！

### 1.加盐噪声

原图是一张风景图像：

代码如下：

```
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("fj.png", cv2.IMREAD_UNCHANGED)
rows, cols, chn = img.shape

#加噪声
for i in range(50000):    
    x = np.random.randint(0, rows) 
    y = np.random.randint(0, cols)    
    img[x,y,:] = 210

cv2.imshow("noise", img)
           
#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
cv2.imwrite('fj-res.png',img)

```

输出结果如下图所示：

---


### 2.雾的模拟生成

代码如下：

```
import numpy as np
import cv2 as cv
import os
import random
 
file = ['fj.png']
output = 'fj-wu.png'

for file_img in file:
    #打开图像
    img = cv.imread(file_img)
    mask_img = cv.imread(file_img)
    
    #雾的颜色
    mask_img[:, :] = (166, 178, 180) 
    
    #里面参数可调，主要调整雾的浓度
    image = cv.addWeighted(img,
                           round(random.uniform(0.03, 0.28), 2),
                           mask_img, 1, 0) 

    #保存的文件夹
    cv.imwrite(output, image) 

```

输出结果如下图所示，效果还不错。

---


## 五.总结

这篇文章就写到这里了，希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，真心推荐原文。学安全两年，认识了很多安全大佬和朋友，希望大家一起进步。同时非常感谢参考文献中的大佬们，感谢老师、实验室小伙伴们的教导和交流，深知自己很菜，得努力前行。感恩遇见，且行且珍惜，小珞珞太可爱了，哈哈。

如果你是一名Python初学者或想了解图像处理知识，推荐作者《Python图像处理》系列，并且多实践多写代码，最后希望文章对您有所帮助。

> 
八月初又认识了很多朋友，也解答了许多学生和博友的问题，虽然自己技术不是很好，但真心感激这一路走来认识了许多伙伴，也享受相互交流的过程，感恩遇见，不负青春。<br/><br/> 这半个月交流让我印象深刻的是复旦、浙大和武大离职读博想当老师的朋友，我们交流了许多学习心得并鼓励前行；也有贵财计科、软工、电商几位学生工作的问题；以及家乡贵财、贵大、兴义师范、贵师大、贵理工选择考研或已经读研同学的咨询，我们相互学习；当然还有许多创业、工作、学习或寻求正能量的交流，包括来自广东、南航、中北、湖北工业、建筑转NLP、安全或AI行业的博友。最让我享受的是，许多我教过的学生或相互鼓励前行的博友，经常祝福我们一家并保持良好的情谊，这或许就是分享的魅力，觉不亚于一次holy shit。教育的方式多种多样，我享受这样的交流和鼓励方式，山川异域，携手同行。<br/><br/> 最后补充一句，父母能带给孩子最好的礼物，或许就是“处于逆境时的应对能力”，希望小珞珞能坚强、健康、快乐的成长，女神也能美丽和幸福一辈子。爱你们哟。<br/><br/> <img alt="在这里插入图片描述" height="300" src="https://img-blog.csdnimg.cn/58aeb2f4922b489587ae8d0476942cec.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="650"/>


(By:Eastmount 2021-11-04 晚上12点 http://blog.csdn.net/eastmount/ )

---

