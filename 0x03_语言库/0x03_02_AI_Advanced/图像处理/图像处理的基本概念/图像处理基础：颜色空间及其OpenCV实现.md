图像处理 OpenCV
<a name="th7Pj"></a>
## 颜色空间
> 颜色是一种连续的现象，它意味着有无数种颜色。但是，人类的眼睛和感知能力是有限的。所以，为了识别这些颜色，需要一种媒介或这些颜色的表示，这种颜色的表示被称为色彩空间。在技术术语中，一个颜色模型或颜色空间是一个特定的3-D坐标系统以及该系统中的一个子空间，其中每一种颜色都由一个单点表示。

<a name="3yR3m"></a>
## 颜色空间的类型
目前主要有五种类型的颜色模型。但是，我将只写一些常见的(RGB、HSV和HSL)。

1. RGB(Red Green Blue)
2. HSL(Hue Saturation Lightness)
3. HSV(Hue Saturation Value)
4. YUV(Luminance, blue–luminance, red–luminance)
5. CMYK(Cyan, Magenta, Yellow, Key)
<a name="Wi5wQ"></a>
### RGB颜色空间
RGB颜色空间是三维坐标系中红、绿、蓝坐标所表示的著名颜色空间之一。在更专业的术语中，**RGB**将颜色描述为由三个部分组成的元组。每个部分都可以取0到255之间的值，其中元组(0,0,0)表示黑色，元组(255,255,255)表示白色。元组的第0、第1和第2个部分分别表示红、绿、蓝的分量。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046352756-c856bff5-0378-4038-8f4c-c18b4c5e5d78.png#align=left&display=inline&height=435&originHeight=435&originWidth=865&size=0&status=done&style=shadow&width=865)
<a name="e0ec5d48"></a>
### RGB颜色空间的Python实现
这里导入了必要的库，cv2用于颜色空间转换，NumPy用于数组操作，Matplotlib用于显示图像，os用于访问图像目录，tqdm用于显示加载栏。
```python
hsl_img = cv2.cvtColor(X[0],cv2.COLOR_BGR2HLS)  #### CONVERTING BGR COLOR SPACE INTO HSL COLOR SPACE  ####
hsl_img_1 = hsl_img.copy()
hsl_img_2 = hsl_img.copy()
hsl_img_3 = hsl_img.copy()
hsl_img_1[:,:,1] = 0  #### HUE --> ZERO  ####
hsl_img_1[:,:,2] = 0
hsl_img_2[:,:,0] = 0  #### SATURATION --> ZERO ####
hsl_img_2[:,:,2] = 0
hsl_img_3[:,:,0] = 0  #### LIGHTNESS --> ZERO ####
hsl_img_3[:,:,1] = 0
```
设置两个空列表Z和X，分别用于存储带有各自图像的标签，然后指定图像大小和路径目录。在这之后，定义了两个函数，用于返回flower类型(**assign_lable**)和访问每个图像、读取和调整其大小(**make_train_data**)。
```python
Z,X=[],[]
IMG_SIZE=150
FLOWER_SUNFLOWER_DIR='../input/flowers-recognition/flowers/flowers/sunflower'
def assign_label(img,flower_type):
    return flower_type
def make_train_data(flower_type,DIR):
    for img in tqdm(os.listdir(DIR)):
        label=assign_label(img,flower_type)
        path = os.path.join(DIR,img)
        img = cv2.imread(path,cv2.IMREAD_COLOR)
        img = cv2.resize(img, (IMG_SIZE,IMG_SIZE))#Resizing the image
```
加载图像，然后在OpenCV以BGR格式读取图像时将BGR颜色空间转换为RGB颜色空间，但Maplotlib使用RGB格式来显示图像。这就是为什么需要转换颜色空间后，读取图像为RGB。<br />然后对固定图像进行三份拷贝，并将每份拷贝的任何双色通道设为零，分别用于访问红、绿、蓝通道。如果让第0个颜色通道都是0那么只会得到蓝色通道。
```python
make_train_data('Sunflower',FLOWER_SUNFLOWER_DIR)  #####Loading Sunflower Data
fix_img = cv2.cvtColor(X[0],cv2.COLOR_BGR2RGB)    ###########CONVERTING BGR COLOR SPACE INTO RGB COLOR SPACE #########
new_img_1 = fix_img.copy() 
new_img_2 = fix_img.copy()
new_img_3 = fix_img.copy()
new_img_1[:,:,0] = 0 # making R channel zero    ####For BLUE channel#####
new_img_1[:,:,1] = 0 #making G channel zero
new_img_2[:,:,1] = 0####For RED color Channel####
new_img_2[:,:,2] = 0
new_img_3[:,:,0] = 0###For GREEN Channel####
new_img_3[:,:,2] = 0
```
显示图像：
```python
f, axes = plt.subplots(1,3, figsize = (15,15))
list = [new_img_1,new_img_2,new_img_3]
i = 0
for ax in axes:
    ax.imshow(list[i])
    i+=1
```
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046352774-e097615d-960d-4d1b-aea8-5f89bec5b1f0.png#align=left&display=inline&height=301&originHeight=301&originWidth=875&size=0&status=done&style=none&width=875)
<a name="PDkMJ"></a>
### HSL颜色空间
HSL的一般含义是色调、饱和度和明度。可以将HSL以圆柱体的形式可视化，如图2(a)所示。围绕圆柱体的是不同的颜色，比如绿色、黄色、红色等等(我们真正想要的颜色)。饱和度是指颜色的多少，而明度是指颜色有多暗或多亮。正如下图所示，圆柱体的顶部全是白色，底部全是黑色。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608046352793-6e2212cb-c318-4d4d-b089-31dd5d479bb5.webp#align=left&display=inline&height=322&originHeight=322&originWidth=716&size=0&status=done&style=shadow&width=716)<br />图2：HSL颜色空间
<a name="059fl"></a>
### HSL颜色空间的Python实现
使用OpenCV函数`cvtColor()`将BGR颜色空间转换为HSL颜色空间，在这里需要传递图像，以及从哪个颜色空间到哪个颜色空间想要改变图像。然后再复制并使两个颜色通道为零，以便分别显示每个颜色通道。
```python
hsl_img = cv2.cvtColor(X[0],cv2.COLOR_BGR2HLS)  #### CONVERTING BGR COLOR SPACE INTO HSL COLOR SPACE  ####
hsl_img_1 = hsl_img.copy()
hsl_img_2 = hsl_img.copy()
hsl_img_3 = hsl_img.copy()
hsl_img_1[:,:,1] = 0  #### HUE --> ZERO  ####
hsl_img_1[:,:,2] = 0
hsl_img_2[:,:,0] = 0  #### SATURATION --> ZERO ####
hsl_img_2[:,:,2] = 0
hsl_img_3[:,:,0] = 0  #### LIGHTNESS --> ZERO ####
hsl_img_3[:,:,1] = 0
```
现在显示三个不同的颜色通道→
```python
f, axes = plt.subplots(1,3, figsize = (15,15))
list = [hsl_img_1,hsl_img_2,hsl_img_3]
i = 0
for ax in axes:
    ax.imshow(list[i])
    i+=1
```
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046352865-f4295a11-5774-4338-9192-e8fe08c2a700.png#align=left&display=inline&height=288&originHeight=288&originWidth=875&size=0&status=done&style=shadow&width=875)
<a name="8369294c"></a>
### HSV颜色空间
HSV这个名字来自于颜色模型的三个坐标，即色相、饱和度和值。它也是一个圆柱形的颜色模型，圆柱体的半径表示饱和度，垂直轴表示值，角度表示色调。对于观察者，色调是占主导地位的，饱和度是混合到色调中的白光的数量，value是chrome的强度，value较低颜色变得更加类似于黑色，value越高，颜色变得更加像颜色本身。通过改变这些参数，可以生成不同的颜色。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608046352785-844d3613-02f6-4cb9-9ade-9170263a5c3b.webp#align=left&display=inline&height=358&originHeight=358&originWidth=875&size=0&status=done&style=shadow&width=875)<br />图3：HSV颜色空间
<a name="W4hsT"></a>
### HSV颜色空间的Python实现
使用cvtColor()函数将色彩空间转换为HSV色彩空间。然后再复制并使两个通道置为零，以便分别显示每个通道。
```python
hsv_img = cv2.cvtColor(X[0],cv2.COLOR_BGR2HSV)
hsv_img_1 = hsv_img.copy()
hsv_img_2 = hsv_img.copy()
hsv_img_3 = hsv_img.copy()
hsv_img_1[:,:,1] = 0#HUE --> ZERO
hsv_img_1[:,:,2] = 0
hsv_img_2[:,:,0] = 0#SATURATION --> ZERO
hsv_img_2[:,:,2] = 0
hsv_img_3[:,:,0] = 0#VALUE --> ZERO
hsv_img_3[:,:,1] = 0
```
单独显示每个颜色通道：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1608046352920-2fca267f-ec71-4d6a-b636-ca4fde9b287d.png#align=left&display=inline&height=292&originHeight=292&originWidth=875&size=0&status=done&style=shadow&width=875)
