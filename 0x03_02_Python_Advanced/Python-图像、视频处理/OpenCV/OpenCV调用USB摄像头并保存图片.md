OpenCV
<a name="Oln13"></a>
## OpenCV调用USB摄像头
如果系统没有预装usb摄像头的驱动，那么根据所用的摄像头安装相应的驱动即可。<br />OpenCV调用USB摄像头的类是`VideoCapture`，`while True`的死循环一直开摄像头，`waitKey`来监听按键即可，最后使用`imwrite`保存图片。
```python
import cv2 as cv
# 调用内置摄像头，所以参数为0，如果有其他的摄像头可以调整参数为1，2
cap = cv.VideoCapture(0)
img_path = "./screenshot.jpg"
img_path_gray = "./screenshot_gray.jpg"
while True:
    # 从摄像头读取图片
    sucess, img = cap.read()
    # 转为灰度图片s
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # 显示摄像头
    cv.imshow('----------please enter "s" to take a picture----------', img)
    # 保持画面的持续,无限期等待输入
    k = cv.waitKey(1)
    # k == 27 通过esc键退出摄像 ESC(ASCII码为27)
    if k == 27:
        cv.destroyAllWindows()
        break
    elif k == ord("s"):
        # 通过s键保存图片，并退出。
        cv.imwrite(img_path, img)
        cv.imwrite(img_path_gray, gray)
        cv.destroyAllWindows()
        break
# 关闭摄像头
cap.release()
```
