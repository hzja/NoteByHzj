Python<br />模拟键盘操作执行自动化任务，常用的有 pyautowin 等自动化操作模块。但是这些模块有一个很大的缺点，编译的时候非常依赖 windows 的C语言底层模块。<br />这里介绍的这个模块叫做 keyboard 它有一个最大的优点：纯Python原生开发，编译时完全不需要依赖C语言模块。一行命令就能完成安装，非常方便。
<a name="a01Ts"></a>
## 1、准备
开始之前，要确保Python和pip已经成功安装在电脑上。
```bash
pip install keyboard
```
<a name="NxnNw"></a>
## 2、基本使用
<a name="uf7IW"></a>
### 按下并释放：
模拟键盘按下某些键或组合键后释放，比如shift + S，并增加空格：
```python
import keyboard

keyboard.press_and_release('shift+s, space')
```
<a name="izC8o"></a>
### 输入文字：
```python
import keyboard

keyboard.write('Python')
```
<a name="BjMPx"></a>
### 等待触发按键并响应：
```python
import keyboard

keyboard.add_hotkey('ctrl+shift+a', print, args=('触发了热键'))
keyboard.wait()
```
这样程序就会监控 "ctrl+shift+a" 组合键的触发情况，一旦组合键触发，就会执行第二个参数的函数，并将第三个参数传入第二个函数的参数中。
<a name="HQUZ3"></a>
### 等待触发某个按键：
```python
import keyboard

keyboard.add_hotkey('ctrl+shift+a', print, args=('触发了热键'))
keyboard.wait('esc')
```
这样，按下 ESC 就会终止等待，继续往下运行程序。
<a name="eJ4V2"></a>
## 3、高级功能
<a name="mQIhH"></a>
### 记录功能：
```python
import keyboard

recorded = keyboard.record(until='esc')
print(recorded)
```
可以看到，按下esc，它会记录所有执行过的键盘操作。
<a name="X9LRN"></a>
### 重放操作：
```python
import keyboard

recorded = keyboard.record(until='esc')
print(recorded)
keyboard.play(recorded, speed_factor=3)
```
`play`方法能完全模拟刚记录下的所有键盘操作。
<a name="QsKxu"></a>
### 替换操作：
```python
import keyboard

keyboard.add_abbreviation('@@', 'Python')
keyboard.wait('esc')
```
输入@@并按下空格，它会将刚输入的@@替换为 Python。
