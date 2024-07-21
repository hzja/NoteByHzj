Python <br />moviepy支持添加字幕、调整音量、片段链接等功能。下面看看详细的操作方法。
<a name="ytbdX"></a>
## 1、准备
开始之前，要确保Python和pip已经成功安装在电脑上。<br />(可选1) 如果用Python的目的是数据分析，可以直接安装Anaconda，它内置了Python和pip。<br />(可选2) 此外，推荐使用VSCode编辑器，它有许多的优点。<br />请选择以下任一种方式输入命令安装依赖：<br />1. Windows 环境 打开 Cmd (开始-运行-CMD)。<br />2. MacOS 环境 打开 Terminal (command+空格输入Terminal)。<br />3. 如果用的是 VSCode编辑器 或 Pycharm，可以直接使用界面下方的Terminal.
```python
pip install moviepy
```
<a name="WTi7o"></a>
## 2、视频剪辑
```python
from moviepy.editor import *


# 剪辑50-60秒的音乐 00:00:50 - 00:00:60
video =CompositeVideoClip([VideoFileClip("videoplayback.mp4").subclip(50,60)])


# 写入剪辑完成的音乐
video.write_videofile("done.mp4")
```
<a name="puV90"></a>
## 3、视频拼接
moviepy几行代码随随便便就能拼接许多片段：
```python
from moviepy.editor import VideoFileClip, concatenate_videoclips

clip1 =VideoFileClip("myvideo.mp4")

# 结合剪辑，你甚至能够完全自动化剪辑拼接视频的操作
clip2 =VideoFileClip("myvideo2.mp4").subclip(50,60)
clip3 =VideoFileClip("myvideo3.mp4")

final_clip = concatenate_videoclips([clip1,clip2,clip3])
final_clip.write_videofile("my_concatenation.mp4")
```
结合剪辑，甚至能够完全自动化剪辑拼接视频的操作。
<a name="qEsLy"></a>
## 4、逐帧变化
“那能完成针对每一帧图像的快速图像处理吗？”<br />反转视频每一帧的绿色和蓝色通道：
```python
from moviepy.editor import VideoFileClip

my_clip =VideoFileClip("videoplayback.mp4")


def scroll(get_frame, t):
    """
    处理每一帧图像
    """

    frame = get_frame(t)
    frame_region = frame[:,:,[0,2,1]]
    return frame_region


modifiedClip = my_clip.fl(scroll)

modifiedClip.write_videofile("test.mp4")
```
<a name="m2YQR"></a>
## 5、导出GIF
```python
from moviepy.editor import *

# 剪辑50-60秒的音乐 00:00:50 - 00:00:60
video = CompositeVideoClip([VideoFileClip("videoplayback.mp4").subclip(50,60)])

my_clip.write_gif('test.gif', fps=12)
```
moviepy还有更多的功能和技巧，详情请见官方文档：[https://zulko.github.io/moviepy/](https://zulko.github.io/moviepy/)
