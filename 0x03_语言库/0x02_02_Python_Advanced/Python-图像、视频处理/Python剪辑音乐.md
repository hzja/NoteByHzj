Python<br />使用Python，只需要3行语句就能剪辑音乐，比如要剪辑《End of Time》33秒到1分10秒的音乐片段：
```bash
from pydub import AudioSegment
song = AudioSegment.from_mp3("end_of_time.mp3")
song[33,[object Object],1000].export('end_of_time_slice.mp3')
```
运行这个脚本，能瞬间完成剪辑。这个功能非常基本，处理速度也非常快。下面再介绍点 Pydub 模块的更高级玩法。
<a name="moRC7"></a>
## 1、准备
开始之前，要确保Python和pip已经成功安装在电脑上。<br />**请选择以下任一种方式输入命令安装依赖**：

1. Windows 环境 打开 Cmd (开始-运行-CMD)。
2. MacOS 环境 打开 Terminal (command+空格输入Terminal)。
3. 如果用的是 VSCode编辑器 或 Pycharm，可以直接使用界面下方的Terminal.
```bash
pip install pydub
```
看到 Successfully installed xxx 则说明安装成功。此外还需要安装ffmpeg。
<a name="Ih0bx"></a>
## 2、音量变化
可以修改上面剪辑的那段音乐的入场音量大小和退场音量大小：
```python
from pydub import AudioSegment

# 1秒=1000毫秒
SECOND = 1000
# 导入音乐
song = AudioSegment.from_mp3("end_of_time.mp3")

# 取33秒到70秒间的片段
song = song[33*SECOND:70*SECOND]

# 入场部分提高6分贝, 退场部分减少5分贝
ten_seconds = 10 * SECOND
last_five_seconds = -5 * SECOND
beginning = song[:ten_seconds] + 6
ending = song[last_five_seconds:] - 5

# 形成新片段
new_song = beginning + song[ten_seconds:last_five_seconds] + ending

# 导出音乐
new_song.export('end_of_time_slice.mp3')
```
效果和预期的一致，想想看，如果这个用专业的音乐编辑器来做，得画段切片、设置音量，然后保存，细思极恐，太费时间了！
<a name="FHemi"></a>
## 2、重复片段
如何重复开头说到的33秒到70秒这段音乐片段？很简单，只要会做乘法就行：
```python
from pydub import AudioSegment
song = AudioSegment.from_mp3("end_of_time.mp3")
(song[33*1000:63*1000])*2.export('end_of_time_slice.mp3')
```
在pydub中，片段和数量之间的运算里**加法调音量，乘法调长度**，记住了吗？
<a name="FTTik"></a>
## 3、渐进渐出
有时候耳朵需要时间来适应音量的变化，尤其是在看短视频的时候，音量忽大忽小实在是太没有用户体验了，这时候渐进渐出的设计就尤为关键：
```python
from pydub import AudioSegment

# 导入音乐
song = AudioSegment.from_mp3("end_of_time.mp3")

# 提取片段
song = song[33*1000:70*1000]

# 渐进渐出
awesome = song.fade_in(5000).fade_out(3000)

# 导出音乐
awesome.export('end_of_time_fade.mp3')
```
听听看，可优秀了！
<a name="umaMC"></a>
## 4、反转音乐
这可能是最有趣和最可能产生特别效果的操作，会完全不认得这首歌：
```python
from pydub import AudioSegment

# 导入音乐
song = AudioSegment.from_mp3("end_of_time.mp3")[33*1000:70*1000]

# 翻转音乐
backwards = song.reverse()

# 导出音乐
backwards.export("end_of_time_reverse.mp3")
```
当然，反转音乐后会变得“耳”不忍“听”，这也很正常，原来的“正向规律 ” 倒转后，音调节奏也变了。
