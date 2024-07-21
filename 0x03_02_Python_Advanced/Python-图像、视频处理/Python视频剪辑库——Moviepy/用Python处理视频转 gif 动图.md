Python
<a name="UnLW0"></a>
## 1、准备工作
需要准备用于生成gif的视频文件，另外，就是需要用到moviepy库，关于这个库更详细的介绍可以查看它的官方文档。<br />[https://zulko.github.io/moviepy/](https://zulko.github.io/moviepy/)<br />命令行键入pip安装
```bash
pip install moviepy
```
<a name="DAFy4"></a>
## 2、初探
```python
from moviepy.editor import *

video_path = "F:\PythonCool\视频\【咒术回战】第20集五条悟帅的有些过分了.mp4"
video = VideoFileClip(video_path)
clip = (video.subclip((2,4.5),(2,6.0))
        .resize(0.2))
clip.write_gif("动图搞定了.gif",fps=8)
```
原始视频素材是尺寸1920*1080，帧率是30。<br />本次转动图的参数设定如下：

- `subclip((2,4.5),(2,6.0)`  选取的是整个视频从2.45秒到2.60秒
- `resize(0.2)` 尺寸大小改为原始大小的0.2也就是1/5大小（毕竟动图尺寸大小其文件大小也太大）
- `write_gif("动图搞定了.gif",fps=8)` 保存gif的时候将帧率调整为了8

基于上述参数设定下，这个动图一共其实是`(2.60-2.45)*8=12`张图构成！
<a name="c0gyx"></a>
## 3、截取区域转动图
有时候可能只需要截取屏幕中指定矩形区域来进行动图的制作，这就要用到`crop`函数截图区域了。
```python
video.crop(
    x1=None,
    y1=None,
    x2=None,
    y2=None,
    width=None,
    height=None,
    x_center=None,
    y_center=None,
)
```
参数：<br />x1、y1：矩形区域左上角坐标<br />x2、y2：矩形区域右下角坐标<br />width、height：是宽度和高度<br />x_center、y_center：表示x1的坐标为x_center-width/2，x2的坐标为x_center+width/2，y_center类似处理<br />比如，截图人物所在区域制作动图代码如下：
```python
crop = (video.subclip((2,4.5),(2,6.0))
        .resize(0.2)
        .crop(x1=70,y1=0, x2=310,y2=216)) 
crop.write_gif("裁剪.gif",fps=8)
```
这里关于左上角和右下角坐标的确定可以采用`preview`方法预览视频，然后鼠标点击对应位置来获取。
```python
video.resize(0.2).preview()
```
<a name="eNiAn"></a>
## 4、固定区域转动图
对于上面那个动图中 的搞定了，希望它常驻，那么可以通过截图搞定了出现这一时刻的内容，然后覆盖到原来位置常驻即可。
```python
snapshot = (crop
            # 截图区域
            .crop(x1=100,y1=190, x2=140,y2=216) 
            # 截取的时间段（1秒的时候）
            .to_ImageClip(1)
            # 设置位置（左上角，和截取区域的左上角一致）
            .set_position((100, 190))
            .set_duration(crop.duration))
# 合并
composition = CompositeVideoClip([crop, snapshot])
composition.write_gif('覆盖.gif', fps=8)
```
<a name="o7nQh"></a>
## 5、添加自定义文本
对于上面这个操作，还有很多方法来完成，比如用一张带有搞定了的图片去覆盖（和上面的案例类似，只是不需要单独截图而是读取一张现成的）；另外，还可以通过添加蒙版再加上自定义文字的形式来玩。
```python
from moviepy.video.VideoClip import TextClip

# 蒙版
mask = (# 长宽为40*20的颜色为黑色的mask
     ColorClip((40, 20), (0, 0, 0))
     # 设置位置（同第4节）
        .set_pos((100, 190))
        .set_duration(crop.duration)
        )
# 文字
text = (TextClip("搞定啦",
                 fontsize=30, color='white',
                 font='SimHei', interline=-25)
        .set_pos((80, 160))
        .set_duration(crop.duration))

composition = CompositeVideoClip([crop, mask, text])
composition.write_gif('覆盖.gif', fps=8)
```
