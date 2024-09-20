Python
<a name="pkNaE"></a>
## 1、提取背景音乐和修改音量
先从抖音上下载视频文件，然后利用moviepy依赖库提取到背景音乐。
```python
def get_audio_from_video(video_path):
    """
    从视频中提取音频
    :param video:
    :return:
    """
    file_path = './source/' + gene_random() + '.wav'
    video = VideoFileClip(video_path)
    audio = video.audio
    audio.write_audiofile(file_path)
    return file_path
```
一个短视频中，可能包含多个背景音乐，需要对背景音乐的音量进行调整。
```python
def handle_bgm(bgm_path, coefficient):
    music = AudioFileClip(bgm_path)

   # 音量调整为原来音量的倍数
   out_music = music.fx(afx.volumex, coefficient).fx(afx.audio_fadein, 0.5).fx(
        afx.audio_fadeout, 1)

   # 保存背景音乐
   out_music.write_audiofile('./source/output.wav')
```
<a name="jCTDL"></a>
## 2、变声
变声也是短视频创作中比较实用的一个技能，有三种方式可以实现。

- 使用 AU 做变调处理
- 修改调用百度云 API
- 使用librosa依赖库。
```python
import librosa

source = "./source/source.wav"

# 加载背景音乐
y, sr = librosa.load(source)

# 修改
librosa.effects.pitch_shift(y, sr, n_steps=6)
y = shrinkstep(10, y, sr)

# 生成新的背景音乐文件
outputpath = "./source/result.wav"
librosa.output.write_wav(outputpath, y, sr)
```
<a name="kERF2"></a>
## 3、视频转场
视频间加入转场使视频播放更加流畅，Python 通过下面 7 个步骤实现视频转场。

- 分离 2 段视频的视频 Clip、音频 Clip
- 统一视频的分辨率
- 分别对视频的开头和结尾加入转场效果，比如淡入淡出效果
- 合并 2 段视频
- 合并 2 段音频
- 设置音频文件
- 保存视频文件
```python
def transitions_animation(path_video1, path_video2):
    """
    两段视频中转场动画（以淡入淡出为例）
    注意：保证视频拍摄帧率一致
    :param video1:
    :param video2:
    :return:
    """
    # 获取视频时长
    clip_video1 = VideoFileClip(path_video1)
    duration_video1 = clip_video1.duration

    # 获取视频音频
    path_audio1 = get_audio_from_video(path_video1)
    path_audio2 = get_audio_from_video(path_video2)

    audio_video1 = AudioFileClip(path_audio1)
    audio_video2 = AudioFileClip(path_audio2)

    clip_video2 = VideoFileClip(path_video2)
    duration_video2 = clip_video2.duration

    print(f'两段视频的时长分别为:{duration_video1},{duration_video2}')

    # 统一视频分辨率
    w, h, fps = clip_video1.w, clip_video1.h, clip_video1.fps
    clip_video2_new = clip_video2.resize((w, h))

    # 转场时长，默认2s
    transitions_time = 2

    # 第一段视频执行淡出效果
    subVideo1_part1 = clip_video1.subclip(0, duration_video1 - 2)
    subVideo1_part2 = clip_video1.subclip(duration_video1 - 2).fadeout(2, (1, 1, 1))

    # 第二段视频执行淡入效果
    subVideo2_part1 = clip_video2_new.subclip(0, 3).fadein(3, (1, 1, 1))
    subVideo2_part2 = clip_video2_new.subclip(3)

    # 合并4段视频
    result_video = concatenate_videoclips([subVideo1_part1, subVideo1_part2, subVideo2_part1, subVideo2_part2])

    # 合并音频
    result_audio = concatenate_audioclips([audio_video1, audio_video2])

    # 视频设置音频文件
    final_clip = result_video.set_audio(result_audio)
    
    # pass 写入视频文件
```
<a name="xFn3W"></a>
## 4、坡度变速
为了使短视频更加酷炫，很多后期高手都会对视频进行了坡度变速操作，使得原本枯燥无味的视频变得生动很多。<br />常用的方式是：先慢速播放，然后恢复正常速度，中间加过渡音效。
```python
def change_video_speed(video_path, speed, start, end):
    """
    改变视频的速度
    :param video_path:视频路径
    :param speed:速度
    :param start:开始时间
    :param end:结束时间
    :return:
    """
    video = VideoFileClip(video_path)

    # 速度变换
    part1 = video.fl_time(lambda t: speed * t, apply_to=['mask', 'video', 'audio']).set_start(start).set_end(
        end / speed)

    # 余下时长恢复速度
    part2 = video.subclip(end)

    # 合成视频
    result_video = concatenate_videoclips([part1, part2])

    result_path = './source/result.mp4'
    result_video.write_videofile(result_path)
```
<a name="N08bO"></a>
## 5、鬼畜视频
鬼畜视频来源于 B 站，在抖音上很多搞笑类视频剪辑都会使用到鬼畜处理，包含：画面帧重复、画面坐标轴映射等。<br />使用 Python 实现鬼畜视频也很简单。
```python
def ghost_video(video_path, repeat_time, location):
    """
    生成鬼畜视频
    :param video_path: 视频路径
    :param repeat_time: 重复次数
    :param location: 处理位置
    :return:
    """
    video = VideoFileClip(video_path)

    # 视频前部分
    part1 = video.subclip(0, location)

    # 视频目标部分，进行重复操作
    target = video.subclip(location, location + 1)

    # 视频后部分
    part2 = video.subclip(location + repeat_time)

    targets = []

    # 分别加入 3 个部分
    targets.append(part1)

    for _ in range(repeat_time):
        targets.append(target)

    targets.append(part2)

    # 合成、生成视频
    pass
```
<a name="QGTTF"></a>
## 6、字幕水印
利用 ImageClip 和 TextClip 可以很方便的加入图片水印和视频字幕。
```python
# 加入图片水印
# 包含：水印的时长、位置、透明度等
logo = (ImageClip("./source/logo.png")
            .set_duration(video.duration)  
            .resize(height=50)  
            .margin(right=8, top=8, opacity=1)  
            .set_pos(("right", "top"))) 

final = CompositeVideoClip([video, logo])

# 加入文字字幕
clip = VideoFileClip(video_path)

# 字幕，文字内容、位置、展示时长
texpClip = TextClip(subtitle, fontsize=30, color='white').set_pos('bottom').set_duration(duration)

video = CompositeVideoClip([clip,texpClip.set_start(start)])
```
<a name="uAjqf"></a>
## 7、转码
需要说明的是，如果使用 AE 制作动画视频，最后的视频文件会非常大，使用ffmpeg依赖库可以快速进行转码和压缩文件。
```python
# 快速转码压缩
alias zh='ffmpeg -i source.mov -qscale 0 output.mp4'
```
<a name="JbNO0"></a>
## 8、其他
以上介绍的操作基本上囊括了视频剪辑创作中大部分内容。
