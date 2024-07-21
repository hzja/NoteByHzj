Python 图片处理<br />在python中，图片处理的模块主要用到 pillow 模块它可以完成图像归档和图像处理两方面功能需求：

- 图像归档：对图像进行批处理、生成图像预览、图像格式转换等；
- 图像处理：图像基本处理、像素处理、颜色处理等。

首先安装一下模块：
```bash
pip install pillow
```
从 PIL中导入 Image类
```python
from PIL import Image
```
九宫格图片切分的主要逻辑如下：

1. 打开待处理的图片
2. 切分图片
3. 保存切分完成的图片
<a name="tnwxY"></a>
### 1、打开待处理的图片
```python
pic = input("请输入图片文件名：")
im = Image.open(pic)
```
<a name="3Ku8z"></a>
### 2、切分图片
```python
width = im.size[0]//3
height = im.size[1]//3
x = 0
y = 0
crop = im.crop((x, y, x+width, y+height))
```
<a name="V46Nm"></a>
### 3、保存图片
```python
filename = 1
crop.save(str(filename) + '.jpg')
```
<a name="gkhxF"></a>
### 4、完整代码
```python
from PIL import Image
pic = input("请输入图片文件名：")
im = Image.open(pic)
width = im.size[0]//3
height = im.size[1]//3
x = 0
y = 0
filename = 1 
for i in range(3):
    for j in range(3):
        crop = im.crop((x, y, x+width, y+height))
        crop.save(str(filename) + '.jpg')
        x += width
        filename += 1
    x = 0
    y += height
```
<a name="H4h8Q"></a>
### 5、九宫格图片处理升级版
```python
from PIL import Image


# 先将image填充为正方形
def fill_image(img):
    width, height = img.size
    # 选取长和宽中较大值作为新图片的
    new_image_length = width if width > height else height
    # 生成新图片[白底]
    new_image = Image.new(img.mode, (new_image_length, new_image_length), color='white')
    # 将之前的图粘贴在新图上，居中
    if width > height:
        # 原图宽大于高，则填充图片的竖直维度
        # #(x,y)二元组表示粘贴上图相对下图的起始位置,是个坐标点
        new_image.paste(img, (0, int((new_image_length - height) / 2)))
    else:
        new_image.paste(img, (int((new_image_length - width) / 2), 0))
    return new_image


def cut_image(img):
    width, height = img.size
    # 一行放3张图
    item_width = int(width / 3)
    box_list = []
    # (left, upper, right, lower)
    for i in range(0, 3):
        for j in range(0, 3):
            print((i*item_width, j*item_width, (i+1)*item_width, (j+1)*item_width))
            box = (j*item_width, i*item_width, (j+1)*item_width, (i+1)*item_width)
            box_list.append(box)
    img_list = [img.crop(box) for box in box_list]

    return img_list


def save_images(img_list):
    index = 1
    for img in img_list:
        img.save("./"+str(index) + '.png', 'PNG')
        index += 1


if __name__ == '__main__':
    image = Image.open('./mv.jpg')
    image.show()
    image = fill_image(image)
    image_list = cut_image(image)
    save_images(image_list)
```
