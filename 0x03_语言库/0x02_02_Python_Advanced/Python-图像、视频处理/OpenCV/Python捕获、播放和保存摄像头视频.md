Python
<a name="Fq4bO"></a>
## 读取视频
要捕获视频，需要创建一个 VideoCapture 对象。它的参数可以是设备索引或视频文件的名称。所以，读取视频有两种方式，分别是从相机中读取视频和从文件中读取视频。
<a name="aos8w"></a>
### 从相机中读取视频
对于有摄像头的设备，例如带摄像头的笔记本电脑，可以直接调起电脑的摄像头，读取摄像头的视频流。
```python
import cv2 as cv
cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()
while True:
    # 逐帧捕获
    ret, frame = cap.read()
    # 如果正确读取帧，ret为True
    if not ret:
        break
    # 显示结果帧
    cv.imshow('frame', frame)
    if cv.waitKey(1) == ord('q'):
        break
# 完成所有操作后，释放捕获器
cap.release()
cv.destroyAllWindows()
```
这里向 `VideoCapture` 对象传入了参数 0，表示设备索引，设备索引就是指定哪个摄像头的数字。正常情况下，一个摄像头会被连接。所以简单地传0。可以通过传递1来选择第二个相机，以此类推。<br />`cap.isOpened()` 用来判断是否捕获到视频。<br />`cap.read()` 返回布尔值(True/ False)。如果正确读取了帧，它将为True。因此，可以通过检查此返回值来检查视频的结尾。<br />`cv.imshow` 方法用来显示视频的帧。播放视频的原理就是逐帧播放。<br />在最后，不要忘记通过 `cap.release()` 释放。<br />运行这段代码，就可以看到一个弹窗实时地播放电脑摄像头中的图像了。
<a name="Kl7LT"></a>
### 从文件中播放视频
与从相机捕获相同，只是用视频文件名更改摄像机索引。<br />另外，在显示视频时，可以通过 `cv.waitKey()` 来控制视频播放的速度。如果设置太小，则视频将非常快，相当于倍速播放；而如果太大，则视频将变得很慢，相当于延迟播放。正常情况下25毫秒就可以了。
```python
import cv2 as cv
cap = cv.VideoCapture('video.mp4')
while cap.isOpened():
    ret, frame = cap.read()
    # 如果正确读取帧，ret为True
    if not ret:
        break
    cv.imshow('frame', frame)
    if cv.waitKey(1) == ord('q'):
        break
cap.release()
cv.destroyAllWindows()
```
运行这段代码，就可以看到一个弹窗播放选择的视频文件了。
<a name="ls2Sy"></a>
## 保存视频
从相机读取视频，可以将视频保存到本地。捕捉一个视频，一帧一帧地处理，如果想要保存这个视频，非常简单，只需使用 `cv.VideoWriter()`。<br />`cv.VideoWriter()` 有5个参数：

- 参数1：输出文件名，例如: output.mp4。
- 参数2：FourCC 代码，FourCC 是用于指定视频编解码器的4字节代码。
- 参数3：帧率的数量。
- 参数4：帧大小。
- 参数5：颜色标志。如果为 True，正常颜色输出，否则就是灰色图像输出。

关于 FourCC 与视频格式的对照关系，这里列举了一些常见的格式：
```python
cv2.VideoWriter_fourcc('P','I','M','1') = MPEG-1 codec 
cv2.VideoWriter_fourcc('M','J','P','G') = motion-jpeg codec --> mp4v 
cv2.VideoWriter_fourcc('M', 'P', '4', '2') = MPEG-4.2 codec 
cv2.VideoWriter_fourcc('D', 'I', 'V', '3') = MPEG-4.3 codec 
cv2.VideoWriter_fourcc('D', 'I', 'V', 'X') = MPEG-4 codec --> avi 
cv2.VideoWriter_fourcc('U', '2', '6', '3') = H263 codec 
cv2.VideoWriter_fourcc('I', '2', '6', '3') = H263I codec 
cv2.VideoWriter_fourcc('F', 'L', 'V', '1') = FLV1 codec
```
保存视频的代码：
```python
import cv2 as cv
cap = cv.VideoCapture(0)
# 定义编解码器并创建VideoWriter对象
fourcc = cv.VideoWriter_fourcc(*'MJPG')
out = cv.VideoWriter('output.mp4', fourcc, 20.0, (640,  480))
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    frame = cv.flip(frame, 1)
    # 写翻转的框架
    out.write(frame)
    cv.imshow('frame', frame)
    if cv.waitKey(1) == ord('q'):
        break
# 完成工作后释放所有内容
cap.release()
out.release()
cv.destroyAllWindows()
```
运行这段代码，就可以在代码目录下找到一个 output.mp4 的视频文件了。
