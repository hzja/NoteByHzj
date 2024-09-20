Python
<a name="KjjPB"></a>
## 一、场景
如果经常刷抖音和微信朋友圈，一定发现了最近九宫格短视频很火！<br />从朋友圈九宫格图片，到九宫格视频，相比传统的图片视频，前者似乎更有个性和逼格。<br />除了传统的剪辑软件可以实现，是否有其他更加快捷方便的方式？比如：一键生成，批量生成？<br />这里使用 Python 一键生成九宫格短视频！
<a name="GFuvh"></a>
## 二、准备
在开始实战之前，使用 pip 安装 2 个依赖，分别是：<br />1、视频处理依赖 `moviepy`<br />2、图片处理依赖 PIL
```bash
# 安装两个依赖
# 视频处理
pip3 install moviepy

# 图片处理依赖
pip3 install Pillow
```
<a name="u4ZlE"></a>
## 三、实战一下
在实战之前，先准备一段原始视频素材<br />下面通过 6 个步骤，将原始视频转换为九宫格视频
<a name="tBwTq"></a>
### 1、新建处理文件夹
新建一个临时文件夹和一个视频输出文件夹
```python
def mkdir_folder(file_path):
    """
    创建一个文件夹，如果不存在就创建；否则不做处理
    :param file_path:
    :return:
    """
    if os.path.exists(file_path):
        return

    os.mkdir(file_path)

# 新建临时文件夹和输出文件夹
mkdir_folder(self.path_temp)
mkdir_folder(self.path_output)
```
<a name="oGNs1"></a>
### 2、获取视频的音频文件及视频基础信息
首先，根据原始视频，使用 moviepy 构建一个 VideoFileClip 对象，从而获取到视频的宽、高、帧率、时长等信息
```python
self.video_raw_clip = VideoFileClip(file_path)

# 宽、高
self.video_width, self.video_height = self.video_raw_clip.w, self.video_raw_clip.h

# 帧率
self.fps = self.video_raw_clip.fps

# 视频时长
self.during = self.video_raw_clip.duration
```
接着，从视频中提取 BGM 音频对象，并写入到文件中
```python
def get_audio_from_video(video_raw_clip, output_path):
    """
    从视频中提取音频
    :param video_raw_clip: 视频Clip对象
    :param output_path: 输出音频文件完整路径
    :return:
    """
    audio = video_raw_clip.audio
    audio.write_audiofile(output_path)

    return output_path
```
<a name="HmN9j"></a>
### 3、处理视频帧
使用原始视频 Clip 对象的 `iter_frames()` 方法，循环获取所有的视频帧图片<br />需要指出的是，为了保证后面视频合成的便捷性，这里对视频帧的文件名按顺序进行命令
```python
i = 1
for frame in self.video_raw_clip.iter_frames():
     image = Image.fromarray(frame)

     # 视频帧图片保存的临时路径（完整路径）
     frame_file_complete_path = self.path_temp + "%04d.jpg" % i

     i += 1
```
视频每一帧都被裁剪成 9 张图片，可以显式指定图片之间的距离，然后计算出新画布的宽和高，最后绘制一个白底背景的图片
```python
# 1、剪成9张图片，计算每张图片的宽、高
item_width = int(self.video_width / 3)
item_height = int(self.video_height / 3)

# 2、新的宽、高
item_width_new = self.video_width + self.item_space * 2
item_height_new = self.video_height + self.item_space * 2

# 3、重新建一个画布背景
new_image = Image.new(image.mode, (item_width_new, item_height_new),
                              color='white')
```
接着，获取每一块区域的坐标值，针对横向、纵向第 2、3 个图片区域加上间隔偏移，粘贴到上面新建的图片上即可
```python
# 4、裁剪图片，然后粘贴到新的画布中去
# i:横向、j：纵向
for i in range(0, 3):
   for j in range(0, 3):
        # 裁剪区域
        box = (j * item_width, i * item_height, (j + 1) * item_width, (i + 1) * item_height)

        # 根据区域，裁剪图片
        crop_image = image.crop(box)

        # 横向、纵向第2块和第3块，要加上偏移距离
        x = 0 if j == 0 else (item_width + self.item_space) * j
        y = 0 if i == 0 else (item_height + self.item_space) * i

        # 将9张图片，按照上面计算的坐标值，粘贴到背景中去
        new_image.paste(crop_image, (int(x), int(y)))

        # 保存图片到本地
        new_image.save(frame_file_complete_path)
```
<a name="KnOz6"></a>
### 4、一篮子图片重新合成视频
把上一步生成的帧图片，按照原视频的帧率转为视频<br />需要注意的是，为了保证生成的视频不会错乱，最好对帧图片按照名称进行一次排序
```python
def pics_to_video(pics_path, output_path, fps):
    """
    图片转为视频
    pics_to_video('./../gif_temp/', './../video_temp/temp1.mp4', 20)
    :param pics_path:
    :param output_path:
    :return:
    """
    image_paths = list(map(lambda x: pics_path + x, os.listdir(pics_path)))

    # 注意：这里必须进行一次排序，保证所有帧的顺序是一致
    image_paths = sort_strings_with_emb_numbers(image_paths)

    # 过滤掉非图片
    image_paths = list(filter(lambda image_path: image_path.endswith('.jpg'), image_paths))

    # 图片剪辑类
    clip = ImageSequenceClip(image_paths,
                             fps=fps)

    clip.write_videofile(output_path)
```
<a name="sv83K"></a>
### 5、加入 BGM 背景音乐
将原始视频的音频文件设置到上一步生成的视频文件，然后写入一个新的文件中去
```python
def video_with_audio(path_video_raw, path_bgm_raw, output):
    """
    视频合成音频
    :return:
    """
    videoclip = VideoFileClip(path_video_raw)
    audioclip = AudioFileClip(path_bgm_raw)

    # 设置视频音频，并写入到文件中去
    videoclip.set_audio(audioclip).write_videofile(output,
                                                   codec='libx264',
                                                   audio_codec='aac',
                                                   temp_audiofile='temp-audio.m4a',
                                                   remove_temp=True
                                                   )
```
<a name="ByCue"></a>
### 6、删除临时文件
利用 shutil将上面视频处理的临时文件，包含帧图片、临时视频文件删除掉
```python
def remove_folder(file_path):
    """
    删除文件夹
    :param file_path:
    :return:
    """
    shutil.rmtree(file_path)

# 删除临时文件
remove_folder(self.path_temp)
```
