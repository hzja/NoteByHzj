Python
<a name="eBpwj"></a>
## OpenCV模块
首先用OpenCV这个库来给图片添加水印，主要用到的函数是`cv2.putText()`方法，里面共有大概9个参数，当然并不需要将这个9个参数统统用到，<br />下面分别为大家来讲述一下`cv2.putText()`方法中9个参数的含义，介绍如下：

- `image`：印有水印的图片
- `text`：水印的具体文字
- `org`：水印放置的横纵坐标，(x坐标，y坐标)
- `font`、`fontScale`：水印的字体相关的参数
- `color`：水印的颜色
- `thinkness`、`lineType`、`bottomLeftOrigin`：印有水印的图片相关的设置，线条的粗细哇、线条的样式哇等等

下面来看一下具体的代码把
```python
img=cv2.imread("test2.jpg") # 导入我们需要添加水印的图片
RGB_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
blank_img = np.zeros(shape=(RGB_img.shape[0],RGB_img.shape[1],3), dtype=np.uint8)
font = cv2.FONT_HERSHEY_SIMPLEX
# 添加水印的文字内容
cv2.putText(blank_img,text='Learn Python',org=(40, 90),
            fontFace=font,fontScale= 2,
            color=(255,0,0),thickness=10,lineType=cv2.LINE_4)
```
然后将印有水印的图片和原图进行结合，代码如下
```python
blended = cv2.addWeighted(src1=RGB_img, alpha=0.7,
                          src2=blank_img, beta=1, gamma = 2)
plt.imshow(blended)
```
出来的结果是这样<br />![](./img/1637983344619-999b2411-8886-4be9-b432-eb1cfc18ea7d.webp)
<a name="Bw7k4"></a>
## PIL模块
PIL是Python的第三方图像处理库，其中Image模块作为PIL库的基本核心类，可以用来例如加载图像和创建新图像等用途，而说到给在图片上添加水印，要用到的方法就是`ImageDraw.Draw.text()`，里面有包括6个参数，其中经常被用到的也就下面四个

- `xy`：添加水印的坐标，x坐标和y坐标
- `text`：具体添加水印的文本内容
- `fill`、`font`：具体文本内容的样式、字体大小等等

下面来看一下具体的代码长什么样子
```python
im = Image.open('test1.jpg')
draw = ImageDraw.Draw(im)
text = "Learn Python"
# 设置字体、字体大小等等
font = ImageFont.truetype('arial.ttf', 33)
# 添加水印
draw.text((50, 50), text, font=font)
im.show()
# 保存图片
im.save('watermark2.jpg')
```
出来的结果是这个样子<br />![](./img/1637983344833-12ba56b0-e67e-468f-b50d-1887008edb2b.webp)
<a name="U8AQy"></a>
## filestools库介绍
介绍一个Python库，叫做filestools，其本质上也是对`ImageDraw.Draw.text()`方法做了进一步的封装，直接使用如下命令，安装后使用。
```bash
pip install filestools --index-url=http://mirrors.aliyun.com/pypi/simple -U
```
这个库一共集成了4个功能，4个库都已经全部迁移到了filestools库中，因此安装这一个库，就可以使用所有的4个功能了，只需要在使用对应功能，分别导入对应的模块即可。
```python
# 给图片加水印
from watermarker.marker import add_mark
```
`add_mark()`方法一共有8个参数，如果不知道怎么查看参数，在jupyter notebook可以输入如下命令，即可查看。<br />![](./img/1635295535078-cba113aa-0a63-447a-bef4-589e87ce9e6e.webp)<br />下面讲述`add_mark()`方法中8个参数的含义，介绍如下 ：

- file： 待添加水印的照片；
- mark： 使用哪些字作为水印；
- out： 添加水印后保存的位置；
- color： 水印字体的颜色，默认颜色#8B8B1B；
- size： 水印字体的大小，默认50；
- opacity： 水印字体的透明度，默认0.15；
- space： 水印字体之间的间隔, 默认75个空格；
- angle： 水印字体的旋转角度，默认30度；接下来，仅用一行代码，给图片添加水印。
<a name="ZgaGP"></a>
### 一行代码给图片加水印
原图长这样：<br />![](./img/1635295535522-6ebd4b82-5dd2-4e70-b0e7-6b93f2512559.webp)<br />使用下面这行代码，仅仅两行代码：
```python
from watermarker.marker import add_mark
add_mark(file=r"aixin.jpg", out=r"C:\Users\Administrator\Desktop\练习", mark="人生苦短，快学Python", opacity=0.2, angle=45, space=30)
```
加水印后长这样：<br />![](./img/1635295534869-f71ddfac-19b5-4f73-b64d-d728f99c8611.webp)<br />最后再为大家解释一下，这一行代码的含义。
```python
add_mark(file=r"aixin.jpg", out=r"C:\Users\Administrator\Desktop\练习", mark="人生苦短，快学Python",opacity=0.2, angle=45, space=30)
```
含义： 给当前工作环境下名为aixin.jpg的图片，添加一张水印。水印字样为快学Python，水印透明度是0.2，水印的旋转角度是45°，水印之间的间隔是30个空格。添加水印后，最后将处理后的图片保存在一个指定目录下。<br />如果要详细学习filestools库，参考下方网站：[https://pypi.org/project/filestools/](https://pypi.org/project/filestools/)
