scikit<br />对图像进行形态学变换。变换对象一般为**灰度图或二值图**，功能函数放在morphology子模块内。
<a name="L8Oic"></a>
## 1、膨胀(dilation)
原理：一般对**二值图像**进行操作。找到**像素值为1**的点，将它的**邻近像素点都设置成这个值**。**1值表示白，0值表示黑**，因此**膨胀操作**可以**扩大白色值范围**，压缩黑色值范围。一般用来扩充边缘或填充小的孔洞。<br />功能函数：
```python
skimage.morphology.dilation(image, selem=None)
```
selem表示结构元素，用于设定局部区域的形状和大小。
```python
from skimage import data
import skimage.morphology as sm
import matplotlib.pyplot as plt
img=data.checkerboard()
dst1=sm.dilation(img,sm.square(5))  #用边长为5的正方形滤波器进行膨胀滤波
dst2=sm.dilation(img,sm.square(15))  #用边长为15的正方形滤波器进行膨胀滤波
 
plt.figure('morphology',figsize=(8,8))
plt.subplot(131)
plt.title('origin image')
plt.imshow(img,plt.cm.gray)
 
plt.subplot(132)
plt.title('morphological image')
plt.imshow(dst1,plt.cm.gray)
 
plt.subplot(133)
plt.title('morphological image')
plt.imshow(dst2,plt.cm.gray)
```
分别用边长为5或15的正方形滤波器对棋盘图片进行膨胀操作，结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658588482400-fa14585f-d6ad-40d4-8208-b4a0ce6819e7.png#clientId=u8b5aafe5-30a5-4&from=paste&height=195&id=u0d271486&originHeight=488&originWidth=1341&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80333&status=done&style=shadow&taskId=u9f3c628e-5184-4be6-b2c1-5e438a7f719&title=&width=536.4)<br />可见滤波器的大小，对操作结果的影响非常大。**一般设置为奇数**。<br />除了正方形的滤波器外，滤波器的形状还有一些，现列举如下：
```python
morphology.square: 正方形
morphology.disk:  平面圆形
morphology.ball: 球形
morphology.cube: 立方体形
morphology.diamond: 钻石形
morphology.rectangle: 矩形
morphology.star: 星形
morphology.octagon: 八角形
morphology.octahedron: 八面体
```
注意，如果处理图像为二值图像(只有0和1两个值)，则可以调用：
```python
skimage.morphology.binary_dilation(image, selem=None)
```
用此函数比处理灰度图像要快。
<a name="QZF1x"></a>
## 2、腐蚀(erosion)
函数：
```python
skimage.morphology.erosion(image, selem=None)
```
selem表示结构元素，用于设定局部区域的形状和大小。<br />和膨胀相反的操作，**将0值扩充到邻近像素**。**扩大黑色部分，减小白色部分**。可用来提取骨干信息，去掉毛刺，去掉孤立的像素。
```python
from skimage import data
import skimage.morphology as sm
import matplotlib.pyplot as plt
img=data.checkerboard()
dst1=sm.erosion(img,sm.square(5))  #用边长为5的正方形滤波器进行膨胀滤波
dst2=sm.erosion(img,sm.square(25))  #用边长为25的正方形滤波器进行膨胀滤波
 
plt.figure('morphology',figsize=(8,8))
plt.subplot(131)
plt.title('origin image')
plt.imshow(img,plt.cm.gray)
 
plt.subplot(132)
plt.title('morphological image')
plt.imshow(dst1,plt.cm.gray)
 
plt.subplot(133)
plt.title('morphological image')
plt.imshow(dst2,plt.cm.gray)
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658588589740-af8a1bbe-cd2d-4948-90be-d8a9f2accd9f.png#clientId=u8b5aafe5-30a5-4&from=paste&height=191&id=u3c59c1a0&originHeight=477&originWidth=1309&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75573&status=done&style=shadow&taskId=ub51e4606-d07d-4e6d-b794-26bdec2bf40&title=&width=523.6)<br />注意，如果处理图像为二值图像(只有0和1两个值)，则可以调用：
```python
skimage.morphology.binary_erosion(image, selem=None)
```
用此函数比处理灰度图像要快。
<a name="DRyO6"></a>
## 3、开运算(opening)
函数：
```python
skimage.morphology.openning(image, selem=None)
```
selem表示结构元素，用于设定局部区域的形状和大小。<br />**先腐蚀再膨胀**，可以消除小物体或小斑块。
```python
from skimage import io,color
import skimage.morphology as sm
import matplotlib.pyplot as plt
img=color.rgb2gray(io.imread('d:/pic/mor.png'))
dst=sm.opening(img,sm.disk(9))  #用边长为9的圆形滤波器进行膨胀滤波
 
plt.figure('morphology',figsize=(8,8))
plt.subplot(121)
plt.title('origin image')
plt.imshow(img,plt.cm.gray)
plt.axis('off')
 
plt.subplot(122)
plt.title('morphological image')
plt.imshow(dst,plt.cm.gray)
plt.axis('off')
```
![2022-07-23-23-05-43.403421500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658588874709-4a6fc6c9-0eb3-4800-9698-78799176d586.png#clientId=u8b5aafe5-30a5-4&from=ui&id=u1dd3f593&originHeight=155&originWidth=481&originalType=binary&ratio=1&rotation=0&showTitle=false&size=224263&status=done&style=shadow&taskId=uc9f4165e-dcc5-4c8e-9c8a-9f9d561db6c&title=)<br />注意，如果处理图像为二值图像(只有0和1两个值)，则可以调用：
```python
skimage.morphology.binary_opening(image, selem=None)
```
用此函数比处理灰度图像要快。
<a name="UrM3M"></a>
## 4、闭运算(closing)
函数：
```python
skimage.morphology.closing(image, selem=None)
```
selem表示结构元素，用于设定局部区域的形状和大小。<br />**先膨胀再腐蚀**，可用来填充孔洞。
```python
from skimage import io,color
import skimage.morphology as sm
import matplotlib.pyplot as plt
img=color.rgb2gray(io.imread('d:/pic/mor.png'))
dst=sm.closing(img,sm.disk(9))  #用边长为5的圆形滤波器进行膨胀滤波
 
plt.figure('morphology',figsize=(8,8))
plt.subplot(121)
plt.title('origin image')
plt.imshow(img,plt.cm.gray)
plt.axis('off')
 
plt.subplot(122)
plt.title('morphological image')
plt.imshow(dst,plt.cm.gray)
plt.axis('off')
```
![2022-07-23-23-05-43.426937000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658588874719-e53e333e-b6fd-42f0-a8a4-e674962cb2df.png#clientId=u8b5aafe5-30a5-4&from=ui&id=Cbh95&originHeight=171&originWidth=487&originalType=binary&ratio=1&rotation=0&showTitle=false&size=250486&status=done&style=shadow&taskId=ud3651c97-17bb-44f9-b374-2e431a443c4&title=)<br />注意，如果处理图像为二值图像(只有0和1两个值)，则可以调用：
```python
skimage.morphology.binary_closing(image, selem=None)
```
用此函数比处理灰度图像要快。
<a name="Vltn2"></a>
## 5、白帽(white-tophat)
函数：
```python
skimage.morphology.white_tophat(image, selem=None)
```
selem表示结构元素，用于设定局部区域的形状和大小。<br />**将原图像减去它的开运算值**，返回比结构化元素小的白点。
```python
from skimage import io,color
import skimage.morphology as sm
import matplotlib.pyplot as plt
img=color.rgb2gray(io.imread('d:/pic/mor.png'))
dst=sm.white_tophat(img,sm.square(21))  
 
plt.figure('morphology',figsize=(8,8))
plt.subplot(121)
plt.title('origin image')
plt.imshow(img,plt.cm.gray)
plt.axis('off')
 
plt.subplot(122)
plt.title('morphological image')
plt.imshow(dst,plt.cm.gray)
plt.axis('off')
```
![2022-07-23-23-05-43.453890200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658589006235-906071f4-75d3-47ff-b1b0-72f4d26a0ffa.png#clientId=u8b5aafe5-30a5-4&from=ui&id=NRnlL&originHeight=174&originWidth=499&originalType=binary&ratio=1&rotation=0&showTitle=false&size=261148&status=done&style=shadow&taskId=ucf148397-7d8b-4fe7-b44a-cc447b6809d&title=)
```python
from skimage import io,color
import skimage.morphology as sm
import matplotlib.pyplot as plt
img=color.rgb2gray(io.imread('d:/pic/mor.png'))
dst=sm.black_tophat(img,sm.square(21))  
 
plt.figure('morphology',figsize=(8,8))
plt.subplot(121)
plt.title('origin image')
plt.imshow(img,plt.cm.gray)
plt.axis('off')
 
plt.subplot(122)
plt.title('morphological image')
plt.imshow(dst,plt.cm.gray)
plt.axis('off')
```
![2022-07-23-23-05-43.485070800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658589006245-2f2e326d-80ef-4369-b529-ea8a71048e5c.png#clientId=u8b5aafe5-30a5-4&from=ui&id=u0808b959&originHeight=168&originWidth=499&originalType=binary&ratio=1&rotation=0&showTitle=false&size=252148&status=done&style=shadow&taskId=u71333e57-22ce-4928-9cbe-efe4c1fb741&title=)
