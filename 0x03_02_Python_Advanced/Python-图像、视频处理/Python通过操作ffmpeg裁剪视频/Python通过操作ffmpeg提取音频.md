Python
<a name="MZ73P"></a>
### FFmpeg 简介
FFmpeg 是一个自由软件，可以运行音频和视频多种格式的录影、转换、流功能，包含了 libavcodec ——这是一个用于多个项目中音频和视频的解码器库，以及 libavformat ——一个音频与视频格式转换库。<br />在 Python 中，有一个库跟 FFmpeg 对应，叫 ffmpy，利用这个库，就可以很轻松地从视频中提取音频了。<br />安装这个库的方式也很简单：
```bash
pip install ffmpy -i https://pypi.douban.com/simple
```
<a name="ULWb0"></a>
### 代码实现
只需要传入三个参数——视频地址、音频结果存放地址和音频的格式后缀，就可以调用 FFmpeg 提取音频了。
```python
def run_ffmpeg(video_path: str, audio_path: str, format: str):
    ff = FFmpeg(inputs={video_path: None},
                outputs={audio_path: '-f {} -vn'.format(format)})
    ff.run()
    return audio_path
```
然后，再写个接收参数的函数：
```python
def extract(video_path: str, tmp_dir: str, ext: str):
    file_name = '.'.join(os.path.basename(video_path).split('.')[0:-1])
    return run_ffmpeg(video_path, os.path.join(tmp_dir, '{}.{}'.format(uuid.uuid4(), ext)), ext)
```
最后来测试一下：
```python
if __name__ == '__main__':
    print(extract('C:/个人/video/test/bbc.mp4', 'C:/个人/video/test', 'wav'))
```
运行这个代码，就会在视频目录生成一个文件名为 uuid 的 wav 格式音频文件。<br />也可以选择输出 mp3 格式的音频。<br />其实，这个提取过程就等价于在命令行敲了一行命令：
```python
 ffmpeg -i C:/个人/video/test/bbc.mp4 -f wav -vn C:/个人/video/test\77350be1-b2ae-4fc8-af80-da4eda463fa9.wav
```
