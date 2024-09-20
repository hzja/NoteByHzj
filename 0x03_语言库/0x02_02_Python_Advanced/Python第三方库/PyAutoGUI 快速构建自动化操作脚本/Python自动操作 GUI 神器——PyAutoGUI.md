Python
<a name="u4jiX"></a>
## GUI 控制神器
pyautogui 是一个纯 Python 的 GUI 自动化工具，通过它可以让程序自动控制鼠标和键盘的一系列操作来达到自动化测试的目的。<br />安装：
```bash
pip3 install pyautogui
```
安装好了就可以直接使用了。
<a name="n1ad0"></a>
## 鼠标操作
<a name="uXWDZ"></a>
### 鼠标移动
桌面操作最基本的就是鼠标操作了，可以控制鼠标的移动：
```python
# 移动鼠标
pyautogui.moveTo(200,400,duration=2)
pyautogui.moveRel(200,500,duration=2)
```
整个桌面是以左上角为坐标轴的原点，所有的操作都以这个原点，来确定操作位置。<br />第一行是将鼠标移动到指定的像素(200,400)位置，第二行代码是将鼠标按照当前点向右移动200px，向下移动400px这个方向移动。<br />两行代码中都有一个共同的参数 `duration`，这个参数表示移动时间，即在指定时间内完成移动操作，单位是秒。<br />运行这两行代码，观察屏幕鼠标的变化，是不是很神奇？<br />还可以获取鼠标位置：
```python
print(pyautogui.position())
```
这个很好理解，就是获取鼠标在当前屏幕中的坐标位置，运行这行代码，会得到诸如下面的信息：
```python
Point(x=400, y=900)
```
<a name="zjt1j"></a>
### 鼠标点击
通常，鼠标有左右两个按键，高级点的鼠标中间还有个按键。<br />pyautogui针对这三个按键操作都有相应的处理：
```python
# 鼠标点击，默认左键
pyautogui.click(100,100)
# 单击左键
pyautogui.click(100,100,button='left')
# 单击右键
pyautogui.click(100,300,button='right')
# 单击中间 
pyautogui.click(100,300,button='middle')
```
鼠标点击，如果不指定 button 参数，默认是点击左键，前面两个参数就是点击坐标的位置。<br />运行这段代码，看看桌面会发生什么？<br />鼠标除了点击操作，还有双击操作：
```python
# 双击左键
pyautogui.doubleClick(10,10)
# 双击右键
pyautogui.rightClick(10,10)
# 双击中键
pyautogui.middleClick(10,10)
```
操作函数也很简单，相信大家一眼就能看明白，如果一眼看不明白，请多看几眼！<br />熟悉前端的小伙伴可能会马上联想到，鼠标操作有按下和释放的过程，屏幕操作也有对应的控制：
```python
# 鼠标按下
pyautogui.mouseDown()
# 鼠标释放
pyautogui.mouseUp()
```
<a name="GjUE0"></a>
### 鼠标拖动
可以控制鼠标拖动到指定坐标位置，并且设置操作时间：
```python
pyautogui.dragTo(100,300,duration=1)
```
这个运行效果和前面移动类似。<br />根据前面移动的经验，也有按照方向拖动鼠标：
```python
pyautogui.dragRel(100,300,duration=4) 
```
<a name="ZMCyf"></a>
### 鼠标滚动
在桌面操作中，有时候需要滚动鼠标到达向上或者向下的位置，这时候可以使用 scroll 这个函数来控制：
```python
pyautogui.scroll(30000)
```
参数是整数，表示向上或向下滚动多少个单位，这个单位根据不同的操作系统可能不一样。如果向上滚动，传入正整数，向下滚动传入负整数。
<a name="B2a2x"></a>
## 屏幕处理
<a name="qsdpc"></a>
### 获取屏幕截图
先来假设一个场景：现在要在屏幕上找到一个红色的点，会怎么做？通常的做法是拿到这个红色点的颜色值，然后再对屏幕上的点逐个进行比较，直到找到为止。<br />pyautogui 为这个操作场景提供了支持，分别有三个函数可以完成这三件事情。
```python
im = pyautogui.screenshot()
im.save('screenshot.png')
rgb = im.getpixel((100, 500))
print(rgb)
match = pyautogui.pixelMatchesColor(500,500,(12,120,400))
print(match)
```
第一个是获取屏幕截图函数，它可以返回一个 Pillow 的 image 对象; 第二个是获取屏幕截图中指定坐标点的颜色，返回 rgb 颜色值；第三个是将指定坐标点的颜色和目标的颜色进行比对，返回布尔值。<br />再来升级一下需求：<br />现在要在屏幕上找到 edge 浏览器的图标，会怎么做？<br />通常的做法是先知道 edge 浏览器的图标长啥样，是绿色还是蓝色，是胖的还是瘦的，对吧？然后再在屏幕上去进行图标的匹配，直到找到一个图标跟目标图标一样，就得到了结果。<br />代码如下：
```python
# 图像识别（一个）
oneLocation = pyautogui.locateOnScreen('1.png')
print(oneLocation)  

# 图像识别（多个）
allLocation = pyautogui.locateAllOnScreen('1.png')
print(list(allLocation))
```
可以在桌面上将某个应用的图标截取下来，保存为图片，然后使用上面几行代码来识别，识别成功，会返回类似下面的结果：
```python
Box(left=20, top=89, width=33, height=34)
[Box(left=20, top=89, width=33, height=34)]
```
这就是图片在桌面的位置，如果找不到图片，就会返回 None。
<a name="jruPy"></a>
## 键盘输入
<a name="hrsU7"></a>
### 键盘函数
键盘输入有下面几个常用的函数：

- `**keyDown()**`**：模拟按键按下**
- `**keyUP()**`**：模拟按键松开**
- `**press()**`**：模拟一次按键过程，即 keyDown 和 keyUP 的组合**
- `**typewrite()**`**：模拟键盘输出内容**

举个例子，大家平时输入感叹号（！）是怎么操作键盘的？<br />按住 shift 按键，然后再按住 1 按键，就可以了。用 pyautogui 控制就是：
```python
pyautogui.keyDown('shift')
pyautogui.press('1')
pyautogui.keyUp('shift')
```
运行上面的代码，如果鼠标是放在编辑框中，会得到一个感叹号！<br />还可以直接输出内容：
```python
pyautogui.typewrite('python', 1)
```
第一个参数是输出的内容，第二个参数是间隔时间，单位是秒。<br />运行上面代码，编辑器里面就会每隔1秒钟按顺序输出 python 的6个字母。
<a name="vG7L1"></a>
### 特殊符号
有时需要输入键盘的一些特殊的符号按键，比如 换行、方向键等，这些有相对应的键盘字符串表示：
```python
pyautogui.typewrite(['p','y','t','h','o','n','enter'])
```
运行上面代码，编辑器里面就会输出 python 之后换行。<br />其他特殊按键对应的字符串请参考官方说明。
<a name="SFiZK"></a>
### 快捷键
如果要复制一个内容，大部分情况下会使用快键键 ctrl + c，按照上面讲的，应该这么实现：
```python
pyautogui.keyDown('ctrl')
pyautogui.keyDown('c')
pyautogui.keyUp('c')
pyautogui.keyUp('ctrl')
```
这样写起来很麻烦，而且需要掌控按键的按下和释放的顺序。<br />pyautogui 提供了一个快捷的函数：
```python
pyautogui.hotkey('ctrl','c')
```
实现的效果和上面的4行代码相同。
<a name="OHZhK"></a>
## 信息框
当在模拟一个桌面操作的时候，如果有分支操作需要根据实际情况来判断，是不是需要有一个地方可以选择走哪个分支？<br />pyautogui 贴心地考虑到了这种情况，可以通过弹出一个选择框来中断当前的操作，选择操作分支。
```python
way = pyautogui.confirm('领导，该走哪条路？', buttons=['农村路', '水路', '陆路'])
print(way)
```
这里就是 HTML 页面的 confirm 选择框，选择了选项之后，可以获取到选择的选项，然后基于这个选项做判断，进入相应的操作分支。<br />除了选择确认框之外，还有其他一些提示信息框：
```python
# 警告框
alert = pyautogui.alert(text='警告！敌军来袭！', title='警告框')
print(alert)
# 密码框
password = pyautogui.password('请输入密码')
print(password)
# 普通输入框
input = pyautogui.prompt('请输入指令：')
print(input)
```
<a name="zoGcO"></a>
## 总结
pyautogui 的基本知识就给大家介绍到这里，这个 python 模块的功能十分强大，函数都非常简单，对 python 初学者比较友好。
