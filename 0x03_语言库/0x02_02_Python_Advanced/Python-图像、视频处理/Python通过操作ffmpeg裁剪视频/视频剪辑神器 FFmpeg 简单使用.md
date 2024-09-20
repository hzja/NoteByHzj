FFmpeg 的用途之广泛，功能之强大远超想象，很多商业软件都在使用。也正是有由于其功能大而全，主要就一些视频音频的处理做下介绍，但也足够日常使用了。
<a name="gsDAi"></a>
## 安装
可以直接在官方下载对应的版本来安装，很简单。
<a name="IASs9"></a>
## 基本概念
容器：视频文件本身就是一个容器，像 MP4、AVI 等不同的格式就代表不同的容器。<br />编码格式：数据要经过编码才能形成固定的格式，软件才能读取，这也就是常见的文件了，像 H.262 H.264 H.265 都是常用的视频编码格式。<br />音频格式就是 MP3 咯。
<a name="kiSAB"></a>
## 简单实用
FFmpeg 的命令格式非常复杂，但基本可以简化成如下格式。
```bash
ffmpeg [a] [b] -i [c] [d] [e]
```
其中 a、b、c、d、e 分别代表全局参数、输入文件参数、输入文件、输出文件参数、输出文件。<br />是不是对照起来还挺对称的呢。<br />其中很多时候很多参数又都是可以省略的，比如 a b d 都可以省略。<br />于是，最简单的一个转换格式的命令就是下面这个样子了。
```bash
ffmpeg -i in.mp4 out.avi
```
有没有很简单，而且转换速度非常快。
<a name="yVRsU"></a>
## 常用命令
<a name="LFbDi"></a>
### 1、提取音频
```bash
ffmpeg -i input.flv -vn -codec copy out.mp3
```
其中 copy 表示直接复制原，不经过重新编码解码，这样比较快。
<a name="OEMww"></a>
### 2、截取视频
```bash
ffmpeg -y -ss 5 -t 5 -i 1.mp4 -c:v copy -c:a copy cut.mp4

ffmpeg -y -ss 5 -to 10 -i 1.mp4 -c:v copy -c:a copy cut.mp4
```
`-ss` 表示直接从第 0 秒开始截取，`-t` 表示持续多久，`-to` 表示结束时间。<br />因此，上面的两条命令都是表示将 1.mp4 从第 5 秒开始，截取 5 秒的片段。
<a name="HI510"></a>
### 3、单张图片转视频
```bash
ffmpeg -y -loop 1 -i bg.png -c:v libx264 -t 15 -pix_fmt yuv420p -vf scale=1080:1440 out.mp4
```
<a name="VIdgL"></a>
### 4、多张图片转视频
```bash
ffmpeg -f image2 -i %d.png -vcodec libx264 output2.mp4
```
其中 %d.png 表示 1.png 2.png ...<br />此方法需要先将图片重命名。
<a name="lqDEA"></a>
### 5、截图
```bash
ffmpeg -y -i input.mp4 -ss 00:00:00 -t 00:00:01 output_%3d.jpg
```
<a name="esyMa"></a>
### 6、转变分辨率
```bash
ffmpeg -i input.mp4 -vf scale=480:-1 output.mp4
```
将 input.mp4 转换为 480p。
<a name="rasfW"></a>
### 7、图片压缩
```bash
ffmpeg -i image_source -q compress_level out_source

ffmpeg -i 3.jpg -q 20 33.jpg
```
其中 compress_level 为压缩等级，正整数，数值越大压缩的越厉害，最终生成的文件占用空间越小。
<a name="uNy8C"></a>
### 8、按比例压缩
```bash
ffmpeg -i image_source -vf scale=width:height out_source

ffmpeg -i 3.jpg -vf scale=1080:1080 33.jpg

ffmpeg -i 3.jpg -vf scale=1080:-1 33.jpg
```
其中 width 为压缩后的图片宽度，height 为压缩后的图片高度。<br />当 width 或者 height 中的某一个为 -1 时，将按照图片原来的比例进行压缩。
<a name="DAADt"></a>
### 9、图片剪裁
```bash
ffmpeg -i image_source -vf crop=width:height:from_x:from_y out_source

ffmpeg -i 3.jpg -vf crop=1920:2240:0:0 33.jpg
```
width 和 height 分别是裁剪后的图片的宽度和高度，from_x 和 from_y 分别是裁剪的起始 x 轴和 y 轴的位置。
<a name="kqOjK"></a>
### 10、添加水印
```bash
ffmpeg -i image_source -vf drawtext=fontfile=font_ttf_path:fontcolor=font_color:fontsize=font_size:text=message_info:x=from_x:y=from_y  out_source

ffmpeg -i 3.jpg -vf drawtext=fontfile=Alibaba-PuHuiTi-Regular.ttf:fontcolor=#FF8C00:fontsize=50:text="这是水印":x=100:y=100 33.jpg
```
这个命令的参数有点多，这里做一下简单的说明。
```bash
font_ttf_path：字体路径
font_color：字体 RGB 色值
font_size：字体大小
message_info：水印文字内容
from_x：水印的起始 x 轴位置
from_y：水印的起始 y 轴位置
```
<a name="kQ1M3"></a>
### 11、镜像处理
图片左右镜像。
```bash
ffmpeg -i image_source -vf hflip out_source

ffmpeg -i 3.jpg -vf hflip 33.jpg
```
图片上下镜像。
```bash
ffmpeg -i image_source -vf vflip out_source

ffmpeg -i 3.jpg -vf vflip 33.jpg
```
