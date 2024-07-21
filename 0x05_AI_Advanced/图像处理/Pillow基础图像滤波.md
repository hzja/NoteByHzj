PythonPillow<br />**ImageFilter 模块**包含一组预定义滤波器的定义，将其与`Image.filter**()**`方法一起使用。这些**滤波器用于改变图像的外观和感觉**。
```python
from PIL import Image, ImageFilter

im = Image.open('jungleSaf2.jpg')

im1 = im.filter(ImageFilter.BLUR)
im1.show()

im2 = im.filter(ImageFilter.MinFilter(3))
im2.show()

im3 = im.filter(ImageFilter.MinFilter) # same as MinFilter(3)
im3.show()
```
在上面的程序中，使用了`**MinFilter()**`方法，该方法用于**创建最小滤波器**。它选择**给定大小的窗口中的最低像素值**。
```python
ImageFilter.MinFilter(size=3)
```
**size：**核大小，以像素为单位。<br />如果保存上述程序并执行，它会使用标准 PNG 显示实用程序显示原始图像、模糊图像和带有 **MinFilter** 的模糊图像。
<a name="f87Ow"></a>
### 原始图像
![2022-07-23-22-38-52.568014700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587374955-303ba7d7-ffad-4978-84cc-1ce46c067e86.png#clientId=u7f4be8ae-edde-4&from=ui&id=ub3721de2&originHeight=382&originWidth=601&originalType=binary&ratio=1&rotation=0&showTitle=false&size=690325&status=done&style=none&taskId=uc0979a6b-411b-4e1f-baaf-a246ff56fef&title=)
<a name="Qphdq"></a>
### 图像模糊
![2022-07-23-22-38-52.673172400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587374951-92fa4cfe-ea5f-4bb5-a596-0d87a5b509d5.png#clientId=u7f4be8ae-edde-4&from=ui&id=qr9qi&originHeight=380&originWidth=589&originalType=binary&ratio=1&rotation=0&showTitle=false&size=673013&status=done&style=none&taskId=u997d621f-056a-4cd4-94bb-45634949ac6&title=)
<a name="P0bMa"></a>
### 使用MinFilter模糊的图像
![2022-07-23-22-38-52.803082200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587402195-704e2bb7-5a81-4f80-80cd-f36c90357895.png#clientId=u7f4be8ae-edde-4&from=ui&id=ub9f55eda&originHeight=372&originWidth=579&originalType=binary&ratio=1&rotation=0&showTitle=false&size=647656&status=done&style=none&taskId=u04e5c794-7d66-436e-9677-148fd9e0eeb&title=)
<a name="DoQVs"></a>
### pillow所提供的过滤器

   - BLUR
   - CONTOUR
   - DETAIL
   - EDGE_ENHANCE
   - EDGE_ENHANCE_MORE
   - EMBOSS
   - FIND_EDGES
   - SHARPEN
   - SMOOTH
   - SMOOTH_MORE
```python
#Import required image modules
from PIL import Image, ImageFilter

#Import all the enhancement filter from pillow

from PIL.ImageFilter import (
   BLUR, CONTOUR, DETAIL, EDGE_ENHANCE, EDGE_ENHANCE_MORE,
   EMBOSS, FIND_EDGES, SMOOTH, SMOOTH_MORE, SHARPEN
)

#Create image object
img = Image.open('images/cat.jpg')
#Applying the blur filter
img1 = img.filter(BLUR)
img1.show()
```
![2022-07-23-22-38-52.912415200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587402192-4c84cb11-c964-46ce-957c-33e48cdc6597.png#clientId=u7f4be8ae-edde-4&from=ui&id=rcwsB&originHeight=374&originWidth=583&originalType=binary&ratio=1&rotation=0&showTitle=false&size=655632&status=done&style=none&taskId=u3e08de37-214d-47ea-abdb-853cbc9a8a2&title=)<br />同样，对于`**image.filter()**`方法，可以传递以下任何参数来获取相应的输出：

- CONTOUR
- DETAIL
- EDGE_ENHANCE
- EDGE_ENHANCE_MORE
- EMBOSS
- FIND_EDGES
- SMOOTH
- SMOOTH_MORE
- SHARPEN
```python
#Import required image modules
from PIL import Image, ImageFilter

#Import all the enhancement filter from pillow
from PIL.ImageFilter import (
   BLUR, CONTOUR, DETAIL, EDGE_ENHANCE, EDGE_ENHANCE_MORE,
   EMBOSS, FIND_EDGES, SMOOTH, SMOOTH_MORE, SHARPEN
)
#Create image object
img = Image.open('images/cat.jpg')
#Applying the blur filter
img1 = img.filter(CONTOUR)
img1.save('images/ImageFilter_blur.jpg')
img1.show()
```
![2022-07-23-22-38-53.064369500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587460290-8cb2b725-00db-4e2d-892b-e3843796e73a.png#clientId=u7f4be8ae-edde-4&from=ui&id=uebda198c&originHeight=385&originWidth=603&originalType=binary&ratio=1&rotation=0&showTitle=false&size=698059&status=done&style=none&taskId=u3e116832-c202-455d-b5eb-f3c2e480af2&title=)
```python
img1 = img.filter(DETAIL)
img1.show()
```
![2022-07-23-22-38-53.166028600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587460268-dc995a57-3d49-4989-8064-d5a95c7d63ca.png#clientId=u7f4be8ae-edde-4&from=ui&id=XUf0s&originHeight=384&originWidth=601&originalType=binary&ratio=1&rotation=0&showTitle=false&size=693933&status=done&style=none&taskId=u8b52e3dd-5674-49b5-adc8-0936fc76390&title=)
```python
img1 = img.filter(EDGE_ENHANCE)
img1.show()
```
![2022-07-23-22-38-53.317267800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587564176-2f884c89-7822-4a67-af61-ef6a08f68ee2.png#clientId=u7f4be8ae-edde-4&from=ui&id=u863f46f5&originHeight=388&originWidth=598&originalType=binary&ratio=1&rotation=0&showTitle=false&size=697669&status=done&style=none&taskId=ube96744e-ed0b-4e07-b7f1-2e8bbe9621b&title=)
```python
img1 = img.filter(EDGE_ENHANCE_MORE)img1.show()
```
![2022-07-23-22-38-53.480105700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587564189-e55a9767-35a1-4336-b605-642c325fd13a.png#clientId=u7f4be8ae-edde-4&from=ui&id=xh0mt&originHeight=382&originWidth=596&originalType=binary&ratio=1&rotation=0&showTitle=false&size=684583&status=done&style=none&taskId=u4da654c2-8996-45ae-a38d-9ecc1369a64&title=)
```python
img1 = img.filter(EMBOSS)
img1.show()
```
![2022-07-23-22-38-53.648315000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587596880-d41f7b99-d319-41f9-bed7-53d8809a4b28.png#clientId=u7f4be8ae-edde-4&from=ui&id=u7f54f0a2&originHeight=383&originWidth=597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=687521&status=done&style=none&taskId=ue42f5f80-ebe8-4c49-a5e2-c1b770908db&title=)
```python
img1 = img.filter(FIND_EDGES)
img1.show()
```
![2022-07-23-22-38-53.721648800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587596879-8b403ab0-a6aa-498c-9292-aabbf5b00497.png#clientId=u7f4be8ae-edde-4&from=ui&id=jNjSR&originHeight=389&originWidth=597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=698297&status=done&style=none&taskId=uca468386-0dca-4050-b4cd-4b5b2d0c150&title=)
```python
img1 = img.filter(SMOOTH)
img1.show()
```
![2022-07-23-22-38-53.856459400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587628900-0d0f6605-65c2-4fb4-81b4-dbf75c45bc0f.png#clientId=u7f4be8ae-edde-4&from=ui&id=u28d7a5d8&originHeight=385&originWidth=595&originalType=binary&ratio=1&rotation=0&showTitle=false&size=688795&status=done&style=none&taskId=ub1cd202a-0b55-49ca-af78-bcc12c416a0&title=)
```python
img1 = img.filter(SHARPEN)
img1.show()
```
![2022-07-23-22-38-53.974472700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658587628907-2d01ec6d-aef3-4b7b-9e63-0f4e0d9f048e.png#clientId=u7f4be8ae-edde-4&from=ui&id=UkFLb&originHeight=384&originWidth=598&originalType=binary&ratio=1&rotation=0&showTitle=false&size=690477&status=done&style=none&taskId=u366a985e-59de-41e5-87cb-5aa8c528599&title=)

参考：<br />[https://www.tutorialspoint.com/python_pillow/python_pillow_adding_filters_to_an_image.htm](https://www.tutorialspoint.com/python_pillow/python_pillow_adding_filters_to_an_image.htm)
