Python MoviePy<br />VideoClip大部分的api还是比较容易理解的，其中有3个fl相关的函数还是要重点说一下。通过这3个函数，可以实现自定义特效。
<a name="JNdf7"></a>
## 1、`fl`
```python
fl(fun, apply_to=[], keep_duration=True)
```
<a name="apECt"></a>
### code1
```python
# 在某一个视频上的每一帧画上一个矩形
import  cv2
from moviepy.editor import *

def fun(gf, t):
    # gf函数其实就是get_frame函数, 可以通过时间来获取到具体的某一帧
    frame = gf(t)
    # 我们可以在该帧上绘制一个矩形
    cv2.rectangle(frame, (200, 200), (400, 400), color=(255, 0, 0),thickness=3)
    return frame

video = VideoFileClip('../demo/result.mp4')
out = video.fl(fun)
out.write_videofile('../demo/result_fl.mp4')
```
可以发现在视频的每一帧上，在(200, 200)的位置上有一个200x200的矩形。
<a name="zLOaW"></a>
### code2
fl函数既可以对帧进行控制，还能对时间进行控制，比如想要让视频快进
```python
from moviepy.editor import *

def fun(gf, t):
    frame = gf(t*(1+0.5))
    return frame

video = VideoFileClip('../demo/result.mp4')
out = video.fl(fun)
out.write_videofile('../demo/result_fl.mp4')
```
这个视频被加快了0.5倍，比如视频输出的第4秒的时候，实际上是获取了原视频的第6秒(4*(1+0.5))的那一帧，当然视频时长没有变，而多出来的画面为原视频的最后一帧。
<a name="HQKGv"></a>
## 2、`fl_image`
```python
# 源码
def fl_image(self, image_func, apply_to=None):
    """
    Modifies the images of a clip by replacing the frame
    `get_frame(t)` by another frame,  `image_func(get_frame(t))`
    """
    if apply_to is None:
        apply_to = []
    return self.fl(lambda gf, t: image_func(gf(t)), apply_to)
```
<a name="Ad3iY"></a>
### code
```python
from moviepy.editor import *

vide = video = VideoFileClip('../demo/result.mp4')
out = video.fl_image(lambda image: image[500:,:])
out.write_videofile('../demo/result_fl_image.mp4')
```
上面代码其实就是把视频的上面500个像素给截掉了，如果原视频是1920_1080的分辨率，那么处理过后视频的分辨率就变成了1920_580了
<a name="wsz0q"></a>
## 3、`fl_time`
```python
# 源码
def fl_time(self, t_func, apply_to=None, keep_duration=False):
    if apply_to is None:
        apply_to = []

    return self.fl(lambda gf, t: gf(t_func(t)), apply_to,
                   keep_duration=keep_duration)
```
<a name="RmlXA"></a>
### code
```python
from moviepy.editor import *

video = VideoFileClip('../demo/result.mp4')
out = video.fl_time(lambda t: t*2).set_duration(video.duration /2)
out.write_videofile('../demo/result_fl_time.mp4')
```
上面代码让视频以2倍的速度播放，当然视频时长也就变为原来的一半。
<a name="T2jfx"></a>
## 4、综合练习
找一个时长大于10秒的视频，截取前2秒慢放2倍,变为4秒视频，在截取4秒创建一个50*50的白色矩形，在4秒内围绕视频四周跑一圈，最后整合成一个完整的视频<br />解析: 截取：`subClip`, 合成：`concatenate_videoclips`<br />![](./img/1620123947555-7b0efce9-93b7-44c7-8bfb-7e022efa2ada.gif)
```python
import  cv2
from moviepy.editor import *
import numpy as np
from PIL import ImageFont, Image, ImageDraw
from moviepy.video.tools.drawing import circle


video = VideoFileClip('../demo/result.mp4')
# 第一个片段
first_seg = video.subclip(0,2).fl_time(lambda t : t/2).set_duration(4)

# 第二个片段
# 先创建一个50*50的白色矩形
rectangle = np.zeros((50, 50, 3))
rectangle.fill(255)
def fl(gf, t):
    frame = gf(t)
    h, w = frame.shape[:2]
    if t < 1:
        frame[0:50,int(t*(w-50)):int(t*(w-50))+50] = rectangle
    elif t >=1 and t <2:
        t = t -1
        frame[int(t*(h-50)):int(t*(h-50)+50), w-50:w] = rectangle
    elif t >=2 and t <3:
        t = t - 2
        frame[h - 50:h + 50, int((1-t)*(w-50)):int((1-t)*(w-50))+50] = rectangle
    elif t >=3:
        t = t - 3
        frame[int((1-t) * (h - 50)):int((1-t) * (h - 50) + 50), 0:50] = rectangle
    return frame
second_seg = video.subclip(3, 7).fl(fl)

out = concatenate_videoclips([first_seg, second_seg]).set_fps(2)
out.write_videofile('../demo/exercise.mp4')

# out.write_gif('../demo/exercise.gif')
```
