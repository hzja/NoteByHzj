Python
<a name="pJJSA"></a>
## 简介
FFmpeg 是一套强大的音视频处理程序，也是很多音视频软件的基础，事实上，FFmpeg 已经成为业界音视频处理的标准了。但命令行使用 FFmpeg 是有一定学习成本的，而 ffmpeg-python 库则很好的解决了这个问题。<br />通过 pip 简单安装之后即可在 python 代码中使用 ffmpeg。
```bash
pip3 install ffmpeg-python
```
<a name="ZdVJ7"></a>
## 获取视频信息
```python
path = 'main.mp4'
probe = ffmpeg.probe(path)
video_stream = next((stream for stream in probe['streams'] if stream['codec_type'] == 'video'), None)
width = int(video_stream['width'])
height = int(video_stream['height'])
print(width, height)
```
可以通过 stream 来获取视频的一些基础信息，比如尺寸、时长、帧率等。
<a name="Hi8Z3"></a>
## 镜像处理
```python
# 左右镜像
ffmpeg.input(path).hflip().output('output.mp4').run()

# 上下镜像
ffmpeg.input(path).vflip().output('output.mp4').run()
```
可以简单的理解为英文单词横向（horizontal）和纵向（vertical）的首字母缩写。
<a name="gJWrx"></a>
## 添加水印
```python
main = ffmpeg.input(path)
logo = ffmpeg.input('logo.png')
ffmpeg.filter([main, logo], 'overlay', 0, 500).output('out.mp4').run()
```
这条命令的意思是，将 logo 水印图片放置在 main 视频的上方，坐标为（0,500）。可以把视频的左上角理解为原点（0,0）的位置，从原点向右和向下分别表示 x 轴和 y 轴。<br />当然，如果把 logo 做的足够大，比视频还要大，然后换一下双方的位置，那就会变成将视频放到 logo 上了，其实相当于给视频加了一个背景图。
```python
ffmpeg.filter([logo, main], 'overlay', 0, 500).output('out.mp4').run()
```
<a name="PUxbN"></a>
## 视频截取
```python
ffmpeg.input(path).trim(start_frame=10,end_frame=20).output('out3.mp4').run()
```
这条命令看起来就很容理解，start_frame 和 end_frame 分别代表起始结束帧。
<a name="Y6ONS"></a>
## 视频拼接
```python
base = ffmpeg.input(path)
ffmpeg.concat(
	base.trim(start_frame=10, end_frame=20),
	base.trim(start_frame=30, end_frame=40),
	base.trim(start_frame=50, end_frame=60)
).output('out3.mp4').run()
```
视频拼接使用 `concat` 函数即可。
