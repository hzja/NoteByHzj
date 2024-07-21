Python<br />介绍一个好玩且实用的东西，使用python来实现一个录音的功能，废话不多说，直接开始。
<a name="EsDZS"></a>
### 安装
使用 PIP 安装 PvRecorder：
```bash
pip3 install pvrecorder
```
<a name="kv3nL"></a>
### 查找可用的麦克风
一台计算机可以有多个麦克风, 例如，笔记本电脑有一个内置麦克风，可能还连接了一个耳机， 第一步是找到要录音的麦克风。
```python
from pvrecorder import PvRecorder

for index, device in enumerate(PvRecorder.get_audio_devices()):
   print(f"[{index}] {device}")
```
在 Dell XPS 笔记本电脑上运行上面的代码可以得到：
```
[0] Monitor of sof-hda-dsp HDMI3/DP3 Output
[1] Monitor of sof-hda-dsp HDMI2/DP2 Output
[2] Monitor of sof-hda-dsp HDMI1/DP1 Output
[3] Monitor of sof-hda-dsp Speaker + Headphones
[4] sof-hda-dsp Headset Mono Microphone + Headphones Stereo Microphone
[5] sof-hda-dsp Digital Microphone
```
记下目标麦克风的索引，将其传递给 PvRecorder 的构造函数， 不确定时，可以将 -1 传递给构造函数以使用默认麦克风。
<a name="BHgMM"></a>
### 录制音频
首先，创建一个 PvRecoder 实例， 需要提供一个 device_index（见上文）和一个 frame_length。frame_length 是希望在每次读取时接收的音频样本数， 将其设置为 512（16 kHz 音频的 32 毫秒）， 然后调用 `.start()` 开始录制，录制完成后，继续循环调用 `.read()` 以接收音频，调用 `.stop()` 停止记录，然后调用 `.delete()` 以在完成后释放资源。
```python
recorder = PvRecorder(device_index=-1, frame_length=512)

try:
   recorder.start()

   while True:
       frame = recorder.read()
       # Do something ...
except KeyboardInterrupt:
   recorder.stop()
finally:
   recorder.delete()
```
<a name="lJecs"></a>
### 将音频保存到文件
可以使用上面的代码片段中增加自己的逻辑做任何想做的事情， 无论是要检测唤醒词、识别语音命令、将语音转录为文本、为搜索索引音频等，还是将其保存到文件中， 比如要保存到文件中，可以使用下面的代码， 这段代码显示了如何将音频保存为 WAVE 文件格式。
```python
from pvrecorder import PvRecorder
import wave
import struct

recorder = PvRecorder(device_index=-1, frame_length=512)
audio = []

try:
   recorder.start()

   while True:
       frame = recorder.read()
       audio.extend(frame)
except KeyboardInterrupt:
   recorder.stop()
   with wave.open('audiotest', 'w') as f:
       f.setparams((1, 2, 16000, 512, "NONE", "NONE"))
       f.writeframes(struct.pack("h" * len(audio), *audio))
finally:
   recorder.delete()
```
